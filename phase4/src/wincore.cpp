// Window Core Implementation
//
// Implements CWnd, CFrameWnd, and related window creation/management.
// This is essential for Hello World applications.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <commctrl.h>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <new>
#include <string>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Forward declarations
static LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static ATOM RegisterOpenMFCClass(HINSTANCE hInstance);

// Exported stubs used internally (avoid virtual dispatch)
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(CWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();

// =============================================================================
// Global State
// =============================================================================

// Map HWND to CWnd* for message routing
#include <map>
#include <set>
#include <mutex>
#include <vector>
static std::map<HWND, CWnd*> g_hwndMap;
static std::map<const CWnd*, COleControlContainer*> g_controlContainerMap;
struct CWndRuntimeState {
    bool toolTipsEnabled = false;
    bool trackingToolTipsEnabled = false;
    bool gestureConfigEnabled = false;
    bool d2dSupportEnabled = false;
    bool dynamicLayoutEnabled = false;
    std::wstring dynamicLayoutResource;
    std::map<std::pair<long, WORD>, void*> properties;
};
std::map<CWnd*, CWndRuntimeState> g_wndRuntimeStates;

struct CFrameWndRuntimeState {
    DWORD menuBarState = 0;
    DWORD menuBarVisibility = 0;
    HMENU hiddenMenu = nullptr;
    UINT menuResourceId = 0;   // frame's own menu resource, not m_nIDHelp
    HMENU ownedMenu = nullptr;  // menu we LoadMenuW'd and must DestroyMenu
    int progressMin = 0;
    int progressMax = 100;
    int progressPos = 0;
    DWORD dockingStyle = 0;
    int modalDepth = 0;
    HWND modalOwner = nullptr;
    std::vector<CControlBar*> controlBars;
};
std::map<CFrameWnd*, CFrameWndRuntimeState> g_frameWndRuntimeStates;

struct DockBarSnapshot {
    CControlBar* bar = nullptr;
    UINT id = 0;
    bool visible = false;
    DWORD dockStyle = 0;
    RECT windowRect{};
};
std::map<const CDockState*, std::vector<DockBarSnapshot>> g_dockStateSnapshots;

// Track temporary CWnd wrappers allocated by OpenMfcAttachCWnd
// These need to be deleted when the underlying window is destroyed
static std::set<CWnd*> g_tempWrappers;

// Guards the process-wide runtime-state registries above. MFC apps may create
// and destroy windows on several UI threads, so the map/set structural
// operations must be serialized to avoid corrupting the trees. std::map keeps
// references to existing elements valid across insertions of other keys, so the
// reference returned by the accessors stays usable outside the lock (a given
// window's state is only erased on that window's own thread at destruction).
static std::mutex g_wndStateMutex;

// In-memory control-bar visibility persistence keyed by profile name. Real MFC
// serializes bar layout to the application profile (registry); this clean-room
// CWinApp exposes no profile API yet, so Save/LoadBarState round-trip within the
// process, which is enough to actually restore bar visibility rather than fake
// it. Guarded by g_wndStateMutex.
static std::map<std::wstring, std::vector<int>> g_barStateStore;

static CWndRuntimeState& GetWindowRuntimeState(CWnd* pWnd) {
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    return g_wndRuntimeStates[pWnd];
}

static CFrameWndRuntimeState& GetFrameRuntimeState(CFrameWnd* pWnd) {
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    return g_frameWndRuntimeStates[pWnd];
}

extern "C" void OpenMfcResetDockState(void* pState) {
    if (!pState) {
        return;
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_dockStateSnapshots.erase(static_cast<const CDockState*>(pState));
}

static void CleanupWindowRuntimeState(CWnd* pWnd) {
    if (!pWnd) {
        return;
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    auto stateIt = g_wndRuntimeStates.find(pWnd);
    if (stateIt != g_wndRuntimeStates.end()) {
        auto it = stateIt->second.properties.find({0x4F44, 0});
        if (it != stateIt->second.properties.end()) {
            delete static_cast<CDCRenderTarget*>(it->second);
            stateIt->second.properties.erase(it);
        }
        g_wndRuntimeStates.erase(stateIt);
    }
    if (auto* pFrameWnd = dynamic_cast<CFrameWnd*>(pWnd)) {
        g_frameWndRuntimeStates.erase(pFrameWnd);
    }
}

static ITaskbarList3* AcquireTaskbarList3ForWindow(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }

    ITaskbarList3* taskbarList = nullptr;
    HRESULT hr = ::CoCreateInstance(
        CLSID_TaskbarList,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_ITaskbarList3,
        reinterpret_cast<void**>(&taskbarList));

    if (FAILED(hr) || !taskbarList) {
        return nullptr;
    }

    taskbarList->HrInit();
    return taskbarList;
}

// Helper to reuse/attach CWnd wrappers for existing HWNDs.
CWnd* OpenMfcLookupCWnd(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }
    auto it = g_hwndMap.find(hWnd);
    if (it != g_hwndMap.end()) {
        return it->second;
    }
    return nullptr;
}

// Detach and optionally delete a CWnd wrapper when window is destroyed
// Called from AfxWndProc on WM_NCDESTROY (the final cleanup message)
void OpenMfcDetachCWnd(HWND hWnd) {
    auto it = g_hwndMap.find(hWnd);
    if (it != g_hwndMap.end()) {
        CWnd* pWnd = it->second;
        g_hwndMap.erase(it);
        g_wndRuntimeStates.erase(pWnd);
        if (auto* pFrameWnd = dynamic_cast<CFrameWnd*>(pWnd)) {
            g_frameWndRuntimeStates.erase(pFrameWnd);
        }

        auto ccIt = g_controlContainerMap.find(pWnd);
        if (ccIt != g_controlContainerMap.end()) {
            delete ccIt->second;
            g_controlContainerMap.erase(ccIt);
        }

        // Delete if this was a temporary wrapper allocated by OpenMfcAttachCWnd
        auto tempIt = g_tempWrappers.find(pWnd);
        if (tempIt != g_tempWrappers.end()) {
            g_tempWrappers.erase(tempIt);
            pWnd->m_hWnd = nullptr;  // Clear before delete to prevent double-detach
            delete pWnd;
        }
    }
}

CWnd* OpenMfcAttachCWnd(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }
    if (CWnd* existing = OpenMfcLookupCWnd(hWnd)) {
        return existing;
    }

    // Create a temporary wrapper - tracked for cleanup on window destruction
    CWnd* wrapper = new CWnd();
    wrapper->m_hWnd = hWnd;
    g_hwndMap[hWnd] = wrapper;
    g_tempWrappers.insert(wrapper);  // Track for deletion
    return wrapper;
}

// Cleanup stale temporary wrappers for destroyed windows
// Called during idle processing to handle windows not using our window procedure
// (e.g., dialog controls obtained via GetDlgItem)
void OpenMfcCleanupTempWrappers() {
    // Build list of stale entries (can't modify map while iterating)
    std::vector<HWND> staleHandles;
    for (auto& pair : g_hwndMap) {
        // Check if the window still exists
        if (!::IsWindow(pair.first)) {
            staleHandles.push_back(pair.first);
        }
    }

    // Clean up stale entries
    for (HWND hWnd : staleHandles) {
        OpenMfcDetachCWnd(hWnd);
    }
}

// Global app pointer (defined in appcore.cpp)
extern CWinApp* g_pApp;
extern CWinThread* AfxGetThread();
static void CWnd_PreSubclassWindowCompat(CWnd* pThis) {
    if (!pThis) {
        return;
    }
    auto& state = GetWindowRuntimeState(pThis);
    state.properties[{0x5053, 0}] = reinterpret_cast<void*>(1);
    if (pThis->m_hWnd) {
        WNDPROC current = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(pThis->m_hWnd, GWLP_WNDPROC));
        if (current) {
            state.properties[{0x5054, 0}] = reinterpret_cast<void*>(current);
        }
    }
}

static void CWnd_SetMessageText(CWnd* pThis, const wchar_t* lpszText) {
    const wchar_t* text = lpszText ? lpszText : L"";
    CWnd* pTop = pThis ? pThis->GetTopLevelFrame() : nullptr;
    auto* pFrame = pTop ? dynamic_cast<CFrameWnd*>(pTop) : nullptr;
    if (pFrame) {
        pFrame->SetMessageText(text);
    }
}

// Returns nonzero when the message was handled and writes the result to pResult.
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// OpenMFC window class name
static const wchar_t* g_szOpenMFCClass = L"OpenMFC_Window";
static ATOM g_atomOpenMFCClass = 0;

namespace {
struct StaticCWndExport {
    void* vptr;
    unsigned char padToHwnd[56];
    HWND hWnd;
    unsigned char tail[160];
};

static_assert(sizeof(StaticCWndExport) == sizeof(CWnd), "static CWnd export must match CWnd size");
static_assert(offsetof(StaticCWndExport, hWnd) == 64, "static CWnd export must place HWND at CWnd::m_hWnd");
}  // namespace

// Symbol: ?wndBottom@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndBottom_CWnd__2V1_B = {
    nullptr, {}, HWND_BOTTOM, {}
};

// Symbol: ?wndNoTopMost@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndNoTopMost_CWnd__2V1_B = {
    nullptr, {}, HWND_NOTOPMOST, {}
};

// Symbol: ?wndTop@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndTop_CWnd__2V1_B = {
    nullptr, {}, HWND_TOP, {}
};

// Symbol: ?wndTopMost@CWnd@@2V1@B
extern "C" const StaticCWndExport MS_ABI impl__wndTopMost_CWnd__2V1_B = {
    nullptr, {}, HWND_TOPMOST, {}
};

// =============================================================================
// CWnd Implementation
// =============================================================================

IMPLEMENT_DYNAMIC(CWnd, CCmdTarget)

#ifdef __GNUC__
// CWnd::classCWnd - MSVC symbol alias
asm(".globl \"?classCWnd@CWnd@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCWnd@CWnd@@2UCRuntimeClass@@A\", _ZN4CWnd9classCWndE\n");
#endif

// CWnd::FromHandle
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPAV1_PAUHWND_____Z(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}

// Symbol: ?FromHandle@CWnd@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}

CWnd* CWnd::FromHandle(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}

// Symbol: ?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ() {
    return &CWnd::classCWnd;
}

// Symbol: ?GetRuntimeClass@CWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWnd__UEBAPEAUCRuntimeClass__XZ(const CWnd* pThis) {
    return CWnd::GetThisClass();
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ\n");
#endif

IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)

#ifdef __GNUC__
// CFrameWnd::classCFrameWnd - MSVC symbol alias
asm(".globl \"?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A\", _ZN9CFrameWnd14classCFrameWndE\n");
#endif

// Symbol: ?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return &CFrameWnd::classCFrameWnd;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ\n");
#endif

// Symbol: ?Create@CWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CCreateContext* pContext)
{
    (void)pContext;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Register window class if not specified
    const wchar_t* className = lpszClassName;
    if (!className || className[0] == 0) {
        if (!g_atomOpenMFCClass) {
            g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
        }
        className = g_szOpenMFCClass;
    }

    // Call PreCreateWindow
    CREATESTRUCTW cs = {};
    cs.lpszClass = className;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = rect.left;
    cs.y = rect.top;
    cs.cx = rect.right - rect.left;
    cs.cy = rect.bottom - rect.top;
    cs.hwndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    cs.hMenu = (HMENU)(UINT_PTR)nID;
    cs.hInstance = hInst;

    // Avoid virtual dispatch: our exported methods are ABI-compatible entrypoints,
    // but we do not rely on any MSVC vtable layout in this phase.
    if (!impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }

    // Create the window
    HWND hWnd = CreateWindowExW(
        cs.dwExStyle,
        cs.lpszClass,
        cs.lpszName,
        cs.style,
        cs.x, cs.y, cs.cx, cs.cy,
        cs.hwndParent,
        cs.hMenu,
        cs.hInstance,
        pThis  // Pass CWnd* as creation parameter
    );

    if (!hWnd) {
        return FALSE;
    }

    pThis->m_hWnd = hWnd;
    g_hwndMap[hWnd] = pThis;

    return TRUE;
}

// Symbol: ?CreateEx@CWnd@@UEAAHKPEB_W0KHHHHPEAUHWND__@@PEAUHMENU__@@PEAX@Z
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
    CWnd* pThis,
    DWORD dwExStyle,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU nIDorHMenu,
    void* lpParam)
{
    if (!pThis) {
        return FALSE;
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    const wchar_t* className = lpszClassName;
    if (!className || className[0] == 0) {
        if (!g_atomOpenMFCClass) {
            g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
        }
        className = g_szOpenMFCClass;
    }

    CREATESTRUCTW cs = {};
    cs.dwExStyle = dwExStyle;
    cs.lpszClass = className;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = x;
    cs.y = y;
    cs.cx = nWidth;
    cs.cy = nHeight;
    cs.hwndParent = hWndParent;
    cs.hMenu = nIDorHMenu;
    cs.hInstance = hInst;
    cs.lpCreateParams = lpParam;

    if (!impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }

    HWND hWnd = CreateWindowExW(
        cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style,
        cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu,
        cs.hInstance, cs.lpCreateParams ? cs.lpCreateParams : pThis);
    if (!hWnd) {
        return FALSE;
    }

    pThis->m_hWnd = hWnd;
    g_hwndMap[hWnd] = pThis;
    return TRUE;
}

// Symbol: ?CreateEx@CWnd@@UEAAHKPEB_W0KAEBUtagRECT@@PEAV1@IPEAX@Z
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis,
    DWORD dwExStyle,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    void* lpParam)
{
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
        pThis,
        dwExStyle,
        lpszClassName,
        lpszWindowName,
        dwStyle,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        pParentWnd ? pParentWnd->m_hWnd : nullptr,
        reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)),
        lpParam);
}

// Symbol: ?ShowWindow@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::ShowWindow(pThis->m_hWnd, nCmdShow);
}

// CWnd::UpdateWindow
extern "C" void MS_ABI impl__UpdateWindow_CWnd__QEAAXXZ(CWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::UpdateWindow(pThis->m_hWnd);
    }
}

// CWnd::EnableWindow
// Symbol: ?EnableWindow@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::EnableWindow(pThis->m_hWnd, bEnable);
}

// CWnd::IsWindowEnabled
// Symbol: ?IsWindowEnabled@CWnd@@QEBAHXZ
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::IsWindowEnabled(pThis->m_hWnd);
}

// Symbol: ?DestroyWindow@CWnd@@UEAAHXZ
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }

    HWND hWnd = pThis->m_hWnd;
    g_hwndMap.erase(hWnd);
    pThis->m_hWnd = nullptr;

    return ::DestroyWindow(hWnd);
}

// Symbol: ?PreCreateWindow@CWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs)
{
    if (!pThis) {
        return FALSE;
    }

    if (!cs.lpszClass || cs.lpszClass[0] == 0) {
        if (!g_atomOpenMFCClass) {
            HINSTANCE hInst = AfxGetInstanceHandle();
            if (!hInst) {
                hInst = GetModuleHandle(nullptr);
            }
            if (hInst) {
                g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
            }
        }
        cs.lpszClass = g_szOpenMFCClass;
    }
    if (cs.style == 0) {
        cs.style = WS_OVERLAPPEDWINDOW;
    }
    if (cs.hInstance == nullptr) {
        cs.hInstance = AfxGetInstanceHandle();
        if (!cs.hInstance) {
            cs.hInstance = GetModuleHandle(nullptr);
        }
    }
    return TRUE;
}

// Symbol: ?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (pThis && pThis->m_hWnd) {
        return ::DefWindowProcW(pThis->m_hWnd, message, wParam, lParam);
    }
    return 0;
}

// Symbol: ?WindowProc@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }

    LRESULT result = 0;
    if (impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(pThis, message, wParam, lParam, &result)) {
        return result;
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}

// CWnd::GetSafeHwnd
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}

// CWnd::MoveWindow
// Symbol: ?MoveWindow@CWnd@@QEAAXHHHHH@Z
extern "C" void MS_ABI impl__MoveWindow_CWnd__QEAAXHHHHH_Z(
    CWnd* pThis, int x, int y, int nWidth, int nHeight, int bRepaint) {
    if (pThis && pThis->m_hWnd) {
        ::MoveWindow(pThis->m_hWnd, x, y, nWidth, nHeight, bRepaint);
    }
}

// CWnd::SetWindowPos
// Symbol: ?SetWindowPos@CWnd@@QEAAHPEBV1@HHHHI@Z
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    HWND hInsert = pWndInsertAfter ? pWndInsertAfter->m_hWnd : nullptr;
    return ::SetWindowPos(pThis->m_hWnd, hInsert, x, y, cx, cy, nFlags);
}

// CWnd::SetWindowTextW
// Symbol: ?SetWindowTextW@CWnd@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
    CWnd* pThis, const wchar_t* lpszString) {
    if (pThis && pThis->m_hWnd) {
        ::SetWindowTextW(pThis->m_hWnd, lpszString ? lpszString : L"");
    }
}

// CWnd::GetWindowTextW
// Symbol: ?GetWindowTextW@CWnd@@QEBAHPEA_WH@Z
extern "C" int MS_ABI impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(
    const CWnd* pThis, wchar_t* lpszStringBuf, int nMaxCount) {
    if (!pThis || !pThis->m_hWnd || !lpszStringBuf || nMaxCount <= 0) {
        if (lpszStringBuf && nMaxCount > 0) {
            lpszStringBuf[0] = L'\0';
        }
        return 0;
    }
    return ::GetWindowTextW(pThis->m_hWnd, lpszStringBuf, nMaxCount);
}

// CWnd::GetWindowTextW (CString& overload)
// Symbol: ?GetWindowTextW@CWnd@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* rString) {
    if (!rString) {
        return;
    }
    if (!pThis || !pThis->m_hWnd) {
        rString->Empty();
        return;
    }
    int length = ::GetWindowTextLengthW(pThis->m_hWnd);
    if (length <= 0) {
        rString->Empty();
        return;
    }
    // GetBuffer(length + 1) to accommodate null terminator for GetWindowTextW
    wchar_t* buffer = rString->GetBuffer(length + 1);
    int actual = ::GetWindowTextW(pThis->m_hWnd, buffer, length + 1);
    rString->ReleaseBuffer(actual);
}

// CWnd::GetWindowTextLengthW
// Symbol: ?GetWindowTextLengthW@CWnd@@QEBAHXZ
extern "C" int MS_ABI impl__GetWindowTextLengthW_CWnd__QEBAHXZ(const CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }
    return ::GetWindowTextLengthW(pThis->m_hWnd);
}

// =============================================================================
// CFrameWnd Implementation
// =============================================================================

// Symbol: ??0CFrameWnd@@QEAA@XZ
// CFrameWnd constructor
// Ordinal: 502
extern "C" CFrameWnd* MS_ABI impl___0CFrameWnd__QEAA_XZ(CFrameWnd* pThis) {
    if (!pThis) {
        return nullptr;
    }

    // Do NOT memset the full object: MSVC may set the vptr in the caller thunk
    // before invoking this imported constructor. Wiping the vptr breaks all
    // subsequent virtual calls and crashes immediately (Hello World test).
    void* savedVptr = *reinterpret_cast<void**>(pThis);
    std::memset(reinterpret_cast<unsigned char*>(pThis) + sizeof(void*), 0, sizeof(CFrameWnd) - sizeof(void*));
    *reinterpret_cast<void**>(pThis) = savedVptr;
    return pThis;
}

// Symbol: ??1CFrameWnd@@UEAA@XZ
// CFrameWnd destructor
// Ordinal: 1129
extern "C" void MS_ABI impl___1CFrameWnd__UEAA_XZ(CFrameWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_hwndMap.erase(pThis->m_hWnd);
        ::DestroyWindow(pThis->m_hWnd);
        pThis->m_hWnd = nullptr;
    }
}

// Symbol: ?Create@CFrameWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@0KPEAUCCreateContext@@@Z
// CFrameWnd::Create
// Ordinal: 3091
extern "C" int MS_ABI impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
    CFrameWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    const wchar_t* lpszMenuName,
    DWORD dwExStyle,
    CCreateContext* pContext)
{
    (void)pContext;

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Use default class if none specified
    const wchar_t* className = lpszClassName;
    if (!className || className[0] == 0) {
        if (!g_atomOpenMFCClass) {
            g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
        }
        className = g_szOpenMFCClass;
    }

    // Default style for frame window
    if (dwStyle == 0) {
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    // Default rect
    RECT useRect = rect;
    if (rect.left == 0 && rect.top == 0 && rect.right == 0 && rect.bottom == 0) {
        useRect.left = CW_USEDEFAULT;
        useRect.top = CW_USEDEFAULT;
        useRect.right = CW_USEDEFAULT;
        useRect.bottom = CW_USEDEFAULT;
    }

    // Load menu if specified
    HMENU hMenu = nullptr;
    if (lpszMenuName) {
        hMenu = ::LoadMenuW(hInst, lpszMenuName);
    }

    // Set up CREATESTRUCT for PreCreateWindow
    CREATESTRUCTW cs = {};
    cs.lpszClass = className;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = useRect.left;
    cs.y = useRect.top;
    cs.cx = (useRect.right == CW_USEDEFAULT) ? CW_USEDEFAULT : (useRect.right - useRect.left);
    cs.cy = (useRect.bottom == CW_USEDEFAULT) ? CW_USEDEFAULT : (useRect.bottom - useRect.top);
    cs.hwndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    cs.hMenu = hMenu;
    cs.hInstance = hInst;
    cs.dwExStyle = dwExStyle;

    // Call PreCreateWindow (virtual)
    // Avoid virtual dispatch: see note in CWnd::Create.
    if (!impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }

    // Create the window
    HWND hWnd = CreateWindowExW(
        cs.dwExStyle,
        cs.lpszClass,
        cs.lpszName,
        cs.style,
        cs.x, cs.y, cs.cx, cs.cy,
        cs.hwndParent,
        cs.hMenu,
        cs.hInstance,
        pThis
    );

    if (!hWnd) {
        return FALSE;
    }

    pThis->m_hWnd = hWnd;
    g_hwndMap[hWnd] = pThis;

    return TRUE;
}

// Symbol: ?PreCreateWindow@CFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z
// CFrameWnd::PreCreateWindow
// Ordinal: 11791
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs)
{
    if (!pThis) {
        return FALSE;
    }
    if (cs.style == 0) {
        cs.style = WS_OVERLAPPEDWINDOW;
    }
    if (!cs.lpszClass || cs.lpszClass[0] == 0) {
        if (!g_atomOpenMFCClass) {
            HINSTANCE hInst = AfxGetInstanceHandle();
            if (!hInst) {
                hInst = GetModuleHandle(nullptr);
            }
            if (hInst) {
                g_atomOpenMFCClass = RegisterOpenMFCClass(hInst);
            }
        }
        cs.lpszClass = g_szOpenMFCClass;
    }

    // Reuse the base behavior so any default window checks remain consistent.
    if (!impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs)) {
        return FALSE;
    }
    return TRUE;
}

// Symbol: ?LoadFrame@CFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
// CFrameWnd::LoadFrame
// Ordinal: 8105
extern "C" int MS_ABI impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis,
    UINT nIDResource,
    DWORD dwDefaultStyle,
    CWnd* pParentWnd,
    CCreateContext* pContext)
{
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandleW(nullptr);
    }

    // Try to load window title from string table
    wchar_t szTitle[256] = L"OpenMFC Window";
    ::LoadStringW(hInst, nIDResource, szTitle, 256);

    // Try to load menu from resource
    HMENU hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCEW(nIDResource));

    // Try to load icon from resource
    HICON hIcon = ::LoadIconW(hInst, MAKEINTRESOURCEW(nIDResource));
    if (!hIcon) {
        hIcon = ::LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION));
    }

    // Try to load accelerator table
    HACCEL hAccel = ::LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(nIDResource));
    if (hAccel) {
        pThis->m_hAccelTable = hAccel;
    }

    // Store menu resource ID
    pThis->m_nIDHelp = nIDResource;

    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};

    // Create the window with the loaded menu
    int result = impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
        pThis,
        nullptr,           // Default class
        szTitle,           // Title from resource
        dwDefaultStyle ? dwDefaultStyle : WS_OVERLAPPEDWINDOW,
        rect,
        pParentWnd,
        MAKEINTRESOURCEW(nIDResource),  // Menu resource ID
        0,                 // No extended style
        pContext
    );

    // Set the menu if window was created successfully
    if (result && pThis->m_hWnd) {
        if (hMenu) {
            ::SetMenu(pThis->m_hWnd, hMenu);
        }
        // Set icon
        if (hIcon) {
            ::SendMessageW(pThis->m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            ::SendMessageW(pThis->m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
    }

    return result;
}

CFrameWnd::CFrameWnd() {
    impl___0CFrameWnd__QEAA_XZ(this);
}

CFrameWnd::~CFrameWnd() {
    impl___1CFrameWnd__UEAA_XZ(this);
}

int CFrameWnd::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                      DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                      const wchar_t* lpszMenuName, DWORD dwExStyle, CCreateContext* pContext) {
    return impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
        this, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}

int CFrameWnd::PreCreateWindow(CREATESTRUCTW& cs) {
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(this, cs);
}

int CFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
        this, nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

void CFrameWnd::ActivateFrame(int nCmdShow) {
    if (!m_hWnd) {
        return;
    }
    int cmd = (nCmdShow == -1) ? SW_SHOW : nCmdShow;
    ::ShowWindow(m_hWnd, cmd);
    ::UpdateWindow(m_hWnd);
}

void CFrameWnd::RecalcLayout(int bNotify) {
    (void)bNotify;
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SIZE, 0, 0);
    }
}

int CFrameWnd::OnCreate(void* lpCreateStruct) {
    CREATESTRUCTW* lpcs = reinterpret_cast<CREATESTRUCTW*>(lpCreateStruct);
    if (!m_hWnd) {
        return -1;
    }

    CCreateContext* pContext = nullptr;
    if (lpcs) {
        pContext = reinterpret_cast<CCreateContext*>(lpcs->lpCreateParams);
    }

    return OnCreateHelper(lpcs, pContext) ? 0 : -1;
}

// =============================================================================
// Window Procedure
// =============================================================================

static LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CWnd* pWnd = nullptr;

    // On WM_NCCREATE, store the CWnd* pointer
    if (message == WM_NCCREATE) {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pWnd = reinterpret_cast<CWnd*>(pCreate->lpCreateParams);
        if (pWnd) {
            pWnd->m_hWnd = hWnd;
            g_hwndMap[hWnd] = pWnd;
        }
    } else {
        // Look up CWnd from HWND
        auto it = g_hwndMap.find(hWnd);
        if (it != g_hwndMap.end()) {
            pWnd = it->second;
        }
    }

    // Route to CWnd::WindowProc if we have a CWnd
    if (pWnd) {
        LRESULT result = impl__WindowProc_CWnd__MEAA_JI_K_J_Z(pWnd, message, wParam, lParam);

        // WM_NCDESTROY is the final message - clean up the wrapper
        // This must come after WindowProc since the window is still valid during the call.
        if (message == WM_NCDESTROY) {
            OpenMfcDetachCWnd(hWnd);
        }

        return result;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

// =============================================================================
// Window Class Registration
// =============================================================================

static ATOM RegisterOpenMFCClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = AfxWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = g_szOpenMFCClass;
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

// =============================================================================
// AfxWinMain - Application Entry Point
// =============================================================================

// Symbol: ?AfxWinMain@@YAHPEAUHINSTANCE__@@0PEA_WH@Z
// Ordinal: 2374
extern "C" int MS_ABI impl__AfxWinMain__YAHPEAUHINSTANCE____0PEA_WH_Z(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    wchar_t* lpCmdLine,
    int nCmdShow)
{
    (void)hPrevInstance;

    // Get the global CWinApp instance
    CWinApp* pApp = AfxGetApp();
    if (!pApp) {
        return -1;  // No application object
    }

    // Store instance handle
    pApp->m_hInstance = hInstance;
    pApp->m_lpCmdLine = lpCmdLine;
    pApp->m_nCmdShow = nCmdShow;

    // Initialize MFC
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow)) {
        return -1;
    }

    // Call InitInstance (virtual - user overrides this)
    if (!pApp->InitInstance()) {
        return pApp->ExitInstance();
    }

    // Run message loop
    int nReturnCode = pApp->Run();

    return nReturnCode;
}

// =============================================================================
// CMDIFrameWnd Implementation
// =============================================================================

IMPLEMENT_DYNCREATE(CMDIFrameWnd, CFrameWnd)

CMDIFrameWnd::CMDIFrameWnd() : m_hWndMDIClient(nullptr) {
}

// CMDIFrameWnd::CreateClient
// Creates the MDI client window
int CMDIFrameWnd::CreateClient(void* lpCreateStruct, CMenu* pWindowMenu) {
    (void)lpCreateStruct;

    if (!m_hWnd) return FALSE;

    CLIENTCREATESTRUCT ccs = {};
    ccs.hWindowMenu = pWindowMenu ? pWindowMenu->m_hMenu : nullptr;
    ccs.idFirstChild = 0xFF00;  // First MDI child ID

    m_hWndMDIClient = ::CreateWindowExW(
        0,
        L"MDICLIENT",
        nullptr,
        WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
        0, 0, 0, 0,
        m_hWnd,
        nullptr,
        AfxGetInstanceHandle(),
        &ccs
    );

    return m_hWndMDIClient != nullptr;
}

// CMDIFrameWnd::GetWindowMenuPopup
HWND CMDIFrameWnd::GetWindowMenuPopup(HMENU hMenuBar) {
    if (!hMenuBar) return nullptr;

    // Find the Window menu by looking for one with MDI child items
    int nCount = ::GetMenuItemCount(hMenuBar);
    for (int i = 0; i < nCount; i++) {
        HMENU hSubMenu = ::GetSubMenu(hMenuBar, i);
        if (hSubMenu) {
            // Check if this submenu has the tile/cascade commands
            if (::GetMenuState(hSubMenu, 0xFF00, MF_BYCOMMAND) != (UINT)-1) {
                return (HWND)(UINT_PTR)hSubMenu;
            }
        }
    }
    return nullptr;
}

// MDI helper functions
void CMDIFrameWnd::MDIActivate(CWnd* pWndActivate) {
    if (m_hWndMDIClient && pWndActivate && pWndActivate->m_hWnd) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)pWndActivate->m_hWnd, 0);
    }
}

CWnd* CMDIFrameWnd::MDIGetActive(int* pbMaximized) const {
    if (!m_hWndMDIClient) return nullptr;

    BOOL bMaximized = FALSE;
    HWND hWnd = (HWND)::SendMessageW(m_hWndMDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);

    if (pbMaximized) {
        *pbMaximized = bMaximized ? 1 : 0;
    }

    return hWnd ? CWnd::FromHandle(hWnd) : nullptr;
}

void CMDIFrameWnd::MDIIconArrange() {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIICONARRANGE, 0, 0);
    }
}

void CMDIFrameWnd::MDIMaximize(CWnd* pWnd) {
    if (m_hWndMDIClient && pWnd && pWnd->m_hWnd) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIMAXIMIZE, (WPARAM)pWnd->m_hWnd, 0);
    }
}

void CMDIFrameWnd::MDINext() {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDINEXT, 0, 0);
    }
}

void CMDIFrameWnd::MDIRestore(CWnd* pWnd) {
    if (m_hWndMDIClient && pWnd && pWnd->m_hWnd) {
        ::SendMessageW(m_hWndMDIClient, WM_MDIRESTORE, (WPARAM)pWnd->m_hWnd, 0);
    }
}

void CMDIFrameWnd::MDISetMenu(CMenu* pFrameMenu, CMenu* pWindowMenu) {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDISETMENU,
                       (WPARAM)(pFrameMenu ? pFrameMenu->m_hMenu : nullptr),
                       (LPARAM)(pWindowMenu ? pWindowMenu->m_hMenu : nullptr));
        ::DrawMenuBar(m_hWnd);
    }
}

void CMDIFrameWnd::MDITile(int nType) {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDITILE, nType, 0);
    }
}

void CMDIFrameWnd::MDICascade(int nType) {
    if (m_hWndMDIClient) {
        ::SendMessageW(m_hWndMDIClient, WM_MDICASCADE, nType, 0);
    }
}

// =============================================================================
// CMDIChildWnd Implementation
// =============================================================================

IMPLEMENT_DYNCREATE(CMDIChildWnd, CFrameWnd)

CMDIChildWnd::CMDIChildWnd() {
    memset(_mdichild_padding, 0, sizeof(_mdichild_padding));
}

int CMDIChildWnd::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
                         DWORD dwStyle, const struct tagRECT& rect,
                         CMDIFrameWnd* pParentWnd, CCreateContext* pContext) {
    (void)lpszClassName;
    (void)pContext;

    if (!pParentWnd || !pParentWnd->m_hWndMDIClient) {
        return FALSE;
    }

    MDICREATESTRUCTW mcs = {};
    mcs.szClass = lpszClassName ? lpszClassName : L"MDICHILD";
    mcs.szTitle = lpszWindowName;
    mcs.hOwner = AfxGetInstanceHandle();
    mcs.x = rect.left ? rect.left : CW_USEDEFAULT;
    mcs.y = rect.top ? rect.top : CW_USEDEFAULT;
    mcs.cx = (rect.right - rect.left) ? (rect.right - rect.left) : CW_USEDEFAULT;
    mcs.cy = (rect.bottom - rect.top) ? (rect.bottom - rect.top) : CW_USEDEFAULT;
    mcs.style = dwStyle ? dwStyle : (WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW);
    mcs.lParam = (LPARAM)this;

    m_hWnd = (HWND)::SendMessageW(pParentWnd->m_hWndMDIClient, WM_MDICREATE, 0, (LPARAM)&mcs);

    if (m_hWnd) {
        g_hwndMap[m_hWnd] = this;
        return TRUE;
    }

    return FALSE;
}

void CMDIChildWnd::ActivateFrame(int nCmdShow) {
    CMDIFrameWnd* pFrame = GetMDIFrame();
    if (pFrame) {
        pFrame->MDIActivate(this);
        if (nCmdShow != -1) {
            ::ShowWindow(m_hWnd, nCmdShow);
        }
    }
}

int CMDIChildWnd::DestroyWindow() {
    CMDIFrameWnd* pFrame = GetMDIFrame();
    if (pFrame && pFrame->m_hWndMDIClient && m_hWnd) {
        ::SendMessageW(pFrame->m_hWndMDIClient, WM_MDIDESTROY, (WPARAM)m_hWnd, 0);
        m_hWnd = nullptr;
        return TRUE;
    }
    return FALSE;
}

CMDIFrameWnd* CMDIChildWnd::GetMDIFrame() {
    HWND hWndParent = ::GetParent(m_hWnd);  // MDI client
    if (hWndParent) {
        hWndParent = ::GetParent(hWndParent);  // MDI frame
        CWnd* pWnd = CWnd::FromHandle(hWndParent);
        return dynamic_cast<CMDIFrameWnd*>(pWnd);
    }
    return nullptr;
}
void CWnd::CancelToolTips(int p0)
{
    bool enabled = p0 != FALSE;
    std::lock_guard<std::mutex> lock(g_wndStateMutex);
    for (auto& it : g_wndRuntimeStates) {
        it.second.toolTipsEnabled = enabled;
        if (!enabled) {
            it.second.trackingToolTipsEnabled = false;
        }
    }
}

CObject* CWnd::CreateObject()
{
    return new(std::nothrow) CWnd();
}

void CWnd::DeleteTempMap()
{
    OpenMfcCleanupTempWrappers();
}

void* CWnd::FromHandlePermanent(HWND p0)
{
    auto it = g_hwndMap.find(p0);
    CWnd* pWnd = it != g_hwndMap.end() ? it->second : nullptr;
    if (!pWnd || g_tempWrappers.find(pWnd) != g_tempWrappers.end()) {
        return nullptr;
    }
    return pWnd;
}

// Symbol: ?FromHandlePermanent@CWnd@@SAPEAV1@PEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd) {
    auto it = g_hwndMap.find(hWnd);
    return it != g_hwndMap.end() ? it->second : nullptr;
}

const MSG* CWnd::GetCurrentMessage()
{
    CWinThread* pThread = AfxGetThread();
    return pThread ? &pThread->m_msgCur : nullptr;
}

void* CWnd::GetDescendantWindow(HWND p0, int p1, int p2)
{
    if (!p0 || p1 == 0) {
        return p2 ? impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(p0) : CWnd::FromHandle(p0);
    }

    HWND hDesc = ::GetDlgItem(p0, p1);
    if (!hDesc) {
        return nullptr;
    }
    return p2 ? impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hDesc) : CWnd::FromHandle(hDesc);
}

// Symbol: ?GetDescendantWindow@CWnd@@SAPEAV1@PEAUHWND__@@HH@Z
extern "C" CWnd* MS_ABI impl__GetDescendantWindow_CWnd__SAPEAV1_PEAUHWND____HH_Z(
    HWND hWnd, int nID, int bOnlyPerm) {
    HWND hChild = hWnd ? ::GetDlgItem(hWnd, nID) : nullptr;
    if (!hChild) {
        return nullptr;
    }
    if (bOnlyPerm) {
        return impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hChild);
    }
    return CWnd::FromHandle(hChild);
}

void* CWnd::GetSafeOwner(void* p0, HWND* p1)
{
    CWnd* pWnd = static_cast<CWnd*>(p0);
    HWND hOwner = pWnd ? pWnd->m_hWnd : nullptr;
    if (!hOwner) {
        CWinThread* pThread = AfxGetThread();
        hOwner = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (p1) {
        *p1 = hOwner;
    }
    return hOwner ? CWnd::FromHandle(hOwner) : nullptr;
}

// Symbol: ?GetSafeOwner@CWnd@@SAPEAV1@PEAV1@PEAPEAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__GetSafeOwner_CWnd__SAPEAV1_PEAV1_PEAPEAUHWND_____Z(
    CWnd* pParent, HWND* pWndTop) {
    HWND hOwner = pParent ? pParent->m_hWnd : nullptr;
    if (!hOwner) {
        CWinThread* pThread = AfxGetThread();
        hOwner = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (pWndTop) {
        *pWndTop = hOwner;
    }
    return hOwner ? CWnd::FromHandle(hOwner) : nullptr;
}

HWND CWnd::GetSafeOwner_(void* p0, void** p1)
{
    HWND hWnd = p0 ? static_cast<HWND>(p0) : nullptr;
    if (!hWnd) {
        CWinThread* pThread = AfxGetThread();
        hWnd = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (p1) {
        *p1 = hWnd;
    }
    return hWnd;
}

// Symbol: ?GetSafeOwner_@CWnd@@SAPEAUHWND__@@PEAU2@PEAPEAU2@@Z
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(
    HWND hParent, HWND* pWndTop) {
    HWND hOwner = hParent;
    if (!hOwner) {
        CWinThread* pThread = AfxGetThread();
        hOwner = pThread && pThread->m_pMainWnd ? pThread->m_pMainWnd->m_hWnd : nullptr;
    }
    if (pWndTop) {
        *pWndTop = hOwner;
    }
    return hOwner;
}

const AFX_INTERFACEMAP* CWnd::GetThisInterfaceMap()
{
    return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();
}

// CWnd's message map: real MFC bases it on CCmdTarget (harvested by pointer
// identity from mfc140u.dll). Returning null here made a real client crash when it
// walked CWnd's map during message/command routing. The handler entries are real
// mfc140u code addresses we cannot replicate, so use the empty terminator and chain
// pfnGetBaseMap to CCmdTarget's exported GetThisMessageMap (the _AFXDLL mechanism).
namespace {
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ();
const AFX_MSGMAP_ENTRY g_cwndEmptyMsgEntries[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CWnd_base() { return impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP g_cwndMessageMap = { gbm_CWnd_base, g_cwndEmptyMsgEntries };

int CWnd_PreTranslateMessageCompat(CWnd* pThis, MSG* pMsg) {
    if (!pThis || !pMsg || !pThis->m_hWnd) {
        return FALSE;
    }
    if (!::IsWindow(pThis->m_hWnd)) {
        return FALSE;
    }
    return ::IsDialogMessageW(pThis->m_hWnd, pMsg) ? TRUE : FALSE;
}
} // namespace

const AFX_MSGMAP* CWnd::GetThisMessageMap()
{
    return &g_cwndMessageMap;
}

// Symbol: ?GetThisMessageMap@CWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// The exported virtual GetMessageMap was a weak stub returning null; give it the
// same base-chained map so a real client's CWnd routing does not null-deref.
// Symbol: ?GetMessageMap@CWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWnd__MEBAPEBUAFX_MSGMAP__XZ(const void*)
{
    return &g_cwndMessageMap;
}

int CWnd::GrayCtlColor(HDC p0, HWND p1, UINT p2, HBRUSH p3, DWORD p4)
{
    if (!p0 || !p1) {
        return FALSE;
    }

    // MFC historically uses a light gray brush for controls that request gray
    // disabled color treatment while preserving default text colors.
    COLORREF textColor = ::GetSysColor(COLOR_WINDOWTEXT);
    COLORREF backColor = ::GetSysColor(COLOR_3DFACE);
    if (p4) {
        textColor = ::GetSysColor(COLOR_GRAYTEXT);
        backColor = RGB(240, 240, 240);
    }

    switch (p2) {
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSTATIC:
            break;
        default:
            break;
    }
    ::SetTextColor(p0, textColor);
    ::SetBkColor(p0, backColor);
    (void)p1;
    (void)p3;
    return TRUE;
}

int CWnd::ModifyStyle(HWND p0, DWORD p1, DWORD p2, UINT p3)
{
    if (!p0) {
        return FALSE;
    }

    ::SetLastError(0);
    LONG_PTR style = ::GetWindowLongPtrW(p0, GWL_STYLE);
    if (style == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    LONG_PTR newStyle = (style & ~static_cast<LONG_PTR>(p1)) | static_cast<LONG_PTR>(p2);
    if (newStyle == style) {
        return TRUE;
    }

    ::SetLastError(0);
    LONG_PTR prevStyle = ::SetWindowLongPtrW(p0, GWL_STYLE, newStyle);
    if (prevStyle == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    if (p3 != 0) {
        if (!::SetWindowPos(p0, nullptr, 0, 0, 0, 0,
                            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED)) {
            return FALSE;
        }
    }
    return TRUE;
}

int CWnd::ModifyStyleEx(HWND p0, DWORD p1, DWORD p2, UINT p3)
{
    if (!p0) {
        return FALSE;
    }

    ::SetLastError(0);
    LONG_PTR style = ::GetWindowLongPtrW(p0, GWL_EXSTYLE);
    if (style == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    LONG_PTR newStyle = (style & ~static_cast<LONG_PTR>(p1)) | static_cast<LONG_PTR>(p2);
    if (newStyle == style) {
        return TRUE;
    }

    ::SetLastError(0);
    LONG_PTR prevStyle = ::SetWindowLongPtrW(p0, GWL_EXSTYLE, newStyle);
    if (prevStyle == 0 && ::GetLastError() != 0) {
        return FALSE;
    }
    if (p3 != 0) {
        if (!::SetWindowPos(p0, nullptr, 0, 0, 0, 0,
                            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED)) {
            return FALSE;
        }
    }
    return TRUE;
}

int CWnd::ReflectLastMsg(HWND p0, LONGLONG* p1)
{
    CWnd* pWnd = p0 ? impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(p0) : nullptr;
    if (!pWnd) {
        if (p1) {
            *p1 = 0;
        }
        return FALSE;
    }
    return pWnd->SendChildNotifyLastMsg(p1);
}

void CWnd::SendMessageToDescendants(HWND p0, UINT p1, ULONGLONG p2, LONGLONG p3, int p4, int p5)
{
    (void)p5;
    if (!p0) {
        return;
    }
    for (HWND hChild = ::GetWindow(p0, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        if (!::IsWindow(hChild)) {
            continue;
        }
        if (p4 && !::IsWindowVisible(hChild)) {
            continue;
        }
        ::SendMessageW(hChild, p1, static_cast<WPARAM>(p2), static_cast<LPARAM>(p3));
        SendMessageToDescendants(hChild, p1, p2, p3, p4, p5);
    }
}

int CWnd::WalkPreTranslateTree(HWND p0, MSG* p1)
{
    if (!p0 || !p1) {
        return FALSE;
    }

    for (HWND hChild = ::GetWindow(p0, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        if (!::IsWindow(hChild)) {
            continue;
        }

        CWnd* pChild = CWnd::FromHandle(hChild);
        if (pChild && CWnd_PreTranslateMessageCompat(pChild, p1)) {
            return TRUE;
        }
        if (WalkPreTranslateTree(hChild, p1)) {
            return TRUE;
        }
    }
    return FALSE;
}




// FIXME: accNavigate ABI back-ref type mismatch with MSVC export
/*
long CWnd::accNavigate(long p0, VARIANT p1, void* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}
*/































// FIXME: SetProxy struct vs class ABI mismatch with MSVC export
/*
long CWnd::SetProxy(IAccessibleProxy* p0)
{
    (void)p0;
    return 0;
}
*/



void CWnd::ActivateTopParent()
{
    if (!m_hWnd) {
        return;
    }
    HWND hTop = static_cast<HWND>(GetTopLevelParent());
    if (!hTop) {
        return;
    }
    ::SetActiveWindow(hTop);
    ::SetForegroundWindow(hTop);
}

int CWnd::Attach(HWND p0)
{
    if (!p0) {
        return FALSE;
    }

    CWnd* existing = OpenMfcLookupCWnd(p0);
    if (existing && existing != this) {
        return FALSE;
    }

    if (m_hWnd == p0) {
        g_hwndMap[p0] = this;
        return TRUE;
    }

    if (m_hWnd) {
        g_hwndMap.erase(m_hWnd);
    }
    g_tempWrappers.erase(this);
    m_hWnd = p0;
    g_hwndMap[p0] = this;
    return TRUE;
}

void CWnd::AttachControlSite(CHandleMap* p0)
{
    GetWindowRuntimeState(this).properties[{0x434D, 0}] = p0;
}

void CWnd::AttachControlSite(COleControlSite* p0, UINT p1)
{
    GetWindowRuntimeState(this).properties[{0x434F, static_cast<WORD>(p1)}] = p0;
}

void CWnd::BindDefaultProperty(long p0, WORD p1, const WCHAR* p2, void* p3)
{
    GetWindowRuntimeState(this).properties[{p0, p1}] = p3;
}

void CWnd::BindProperty(long p0, void* p1)
{
    SetProperty(p0, 0, p1);
}

void CWnd::CenterWindow(void* p0)
{
    if (!m_hWnd) {
        return;
    }

    HWND hParent = nullptr;
    if (p0) {
        if (::IsWindow(reinterpret_cast<HWND>(p0))) {
            hParent = static_cast<HWND>(p0);
        } else if (auto* pWnd = static_cast<CWnd*>(p0)) {
            hParent = pWnd->m_hWnd;
        }
    }

    if (!hParent) {
        hParent = ::GetParent(m_hWnd);
    }
    if (!hParent) {
        hParent = ::GetDesktopWindow();
    }

    RECT rcParent;
    RECT rcWindow;
    if (!::GetWindowRect(m_hWnd, &rcWindow) || !::GetWindowRect(hParent, &rcParent)) {
        return;
    }

    int width = rcWindow.right - rcWindow.left;
    int height = rcWindow.bottom - rcWindow.top;
    int x = rcParent.left + ((rcParent.right - rcParent.left - width) / 2);
    int y = rcParent.top + ((rcParent.bottom - rcParent.top - height) / 2);

    ::SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CWnd::CheckDlgButton(int p0, UINT p1)
{
    if (!m_hWnd) {
        return;
    }
    ::CheckDlgButton(m_hWnd, p0, p1);
}

void CWnd::CheckRadioButton(int p0, int p1, int p2)
{
    if (!m_hWnd) {
        return;
    }
    for (int controlId = p0; controlId <= p1; ++controlId) {
        HWND hCtrl = ::GetDlgItem(m_hWnd, controlId);
        if (!hCtrl) {
            continue;
        }
        ::SendMessageW(hCtrl, BM_SETCHECK, (controlId == p2) ? BST_CHECKED : BST_UNCHECKED, 0);
    }
}

void CWnd::ClientToScreen(RECT* p0) const
{
    if (!m_hWnd || !p0) {
        return;
    }

    POINT points[2] = {
        {p0->left, p0->top},
        {p0->right, p0->bottom}
    };
    if (::MapWindowPoints(m_hWnd, HWND_DESKTOP, points, 2) != 0) {
        p0->left = points[0].x;
        p0->top = points[0].y;
        p0->right = points[1].x;
        p0->bottom = points[1].y;
    }
}

int CWnd::CreateControl(const GUID*& p0, const WCHAR* p1, DWORD p2, const RECT*& p3, void* p4, UINT p5, CFile* p6, int p7, WCHAR* p8)
{
    COleControl* control = dynamic_cast<COleControl*>(this);
    if (!control || !p0) {
        return FALSE;
    }

    CWnd* pParent = p4 ? static_cast<CWnd*>(p4) : this;
    if (!pParent) {
        pParent = this;
    }
    RECT rect{};
    if (p3) {
        rect = *p3;
    } else if (pParent && pParent->m_hWnd) {
        ::GetClientRect(pParent->m_hWnd, &rect);
    }

    return control->CreateControl(*p0, p1, p2, rect, pParent, p5, p6, p7, nullptr) ? TRUE : FALSE;
}

// Symbol: ?CreateControl@CWnd@@QEAAHAEBU_GUID@@PEB_WKAEBUtagRECT@@PEAV1@IPEAVCFile@@HPEA_W@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHAEBU_GUID__PEB_WKAEBUtagRECT__PEAV1_IPEAVCFile__HPEA_W_Z(
    CWnd* pThis,
    const GUID& clsid,
    const WCHAR* pWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CFile* pPersist,
    int bStorage,
    WCHAR* pLicKey) {
    if (!pThis) {
        return FALSE;
    }
    const GUID* clsidPtr = &clsid;
    RECT const* rectPtr = &rect;
    void* parent = pParentWnd;
    return pThis->CreateControl(clsidPtr, pWindowName, dwStyle, rectPtr, parent, nID, pPersist, bStorage, pLicKey);
}

int CWnd::CreateControl(const GUID*& p0, const WCHAR* p1, DWORD p2, const POINT* p3, const SIZE* p4, void* p5, UINT p6, CFile* p7, int p8, WCHAR* p9)
{
    COleControl* control = dynamic_cast<COleControl*>(this);
    if (!control || !p0) {
        return FALSE;
    }

    CWnd* pParent = p5 ? static_cast<CWnd*>(p5) : this;
    if (!pParent) {
        pParent = this;
    }

    RECT rect{};
    if (p3) {
        rect.left = p3->x;
        rect.top = p3->y;
        rect.right = rect.left + (p4 ? p4->cx : 0);
        rect.bottom = rect.top + (p4 ? p4->cy : 0);
    }
    if (rect.right <= rect.left || rect.bottom <= rect.top) {
        if (pParent && pParent->m_hWnd) {
            ::GetClientRect(pParent->m_hWnd, &rect);
        }
    }

    return control->CreateControl(*p0, p1, p2, rect, pParent, p6, p7, p8, nullptr) ? TRUE : FALSE;
}

// Symbol: ?CreateControl@CWnd@@QEAAHAEBU_GUID@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@PEAV1@IPEAVCFile@@HPEA_W@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHAEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__PEAV1_IPEAVCFile__HPEA_W_Z(
    CWnd* pThis,
    const GUID& clsid,
    const WCHAR* pWindowName,
    DWORD dwStyle,
    const POINT* pPoint,
    const SIZE* pSize,
    CWnd* pParentWnd,
    UINT nID,
    CFile* pPersist,
    int bStorage,
    WCHAR* pLicKey) {
    if (!pThis) {
        return FALSE;
    }
    const GUID* clsidPtr = &clsid;
    const RECT* rectPtr = nullptr;
    if (pPoint || pSize) {
        static RECT fallbackRect{};
        fallbackRect.left = pPoint ? pPoint->x : 0;
        fallbackRect.top = pPoint ? pPoint->y : 0;
        fallbackRect.right = fallbackRect.left + (pSize ? pSize->cx : 0);
        fallbackRect.bottom = fallbackRect.top + (pSize ? pSize->cy : 0);
        rectPtr = &fallbackRect;
    }
    void* parent = pParentWnd;
    return pThis->CreateControl(clsidPtr, pWindowName, dwStyle, rectPtr, parent, nID, pPersist, bStorage, pLicKey);
}

int CWnd::CreateControl(const CControlCreationInfo*& p0, DWORD p1, const POINT* p2, const SIZE* p3, void* p4, UINT p5)
{
    if (!p0) {
        return FALSE;
    }

    if (COleControl* control = dynamic_cast<COleControl*>(this)) {
        CWnd* pParent = p4 ? static_cast<CWnd*>(p4) : this;
        if (!pParent) {
            pParent = this;
        }
        RECT rect{};
        if (p2) {
            rect.left = p2->x;
            rect.top = p2->y;
            rect.right = rect.left + (p3 ? p3->cx : 0);
            rect.bottom = rect.top + (p3 ? p3->cy : 0);
        } else if (pParent && pParent->m_hWnd) {
            ::GetClientRect(pParent->m_hWnd, &rect);
        }

        return control->CreateControl(L"", L"", p1, rect, pParent, 0, nullptr, FALSE, nullptr) ? TRUE : FALSE;
    }

    return FALSE;
}

int CWnd::CreateControl(const WCHAR* p0, const WCHAR* p1, DWORD p2, const RECT*& p3, void* p4, UINT p5, CFile* p6, int p7, WCHAR* p8)
{
    if (COleControl* control = dynamic_cast<COleControl*>(this)) {
        CWnd* pParent = p4 ? static_cast<CWnd*>(p4) : this;
        if (!pParent) {
            pParent = this;
        }
        RECT rect{};
        if (p3) {
            rect = *p3;
        } else if (pParent && pParent->m_hWnd) {
            ::GetClientRect(pParent->m_hWnd, &rect);
        }
        return control->CreateControl(p0, p1, p2, rect, pParent, p5, p6, p7, p8 ? p8 : nullptr) ? TRUE : FALSE;
    }

    return FALSE;
}

// Symbol: ?CreateControl@CWnd@@QEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAVCFile@@HPEA_W@Z
extern "C" int MS_ABI impl__CreateControl_CWnd__QEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAVCFile__HPEA_W_Z(
    CWnd* pThis,
    const WCHAR* lpszClass,
    const WCHAR* pWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CFile* pPersist,
    int bStorage,
    WCHAR* pLicKey) {
    if (!pThis) {
        return FALSE;
    }
    const RECT* rectPtr = &rect;
    void* parent = pParentWnd;
    return pThis->CreateControl(lpszClass, pWindowName, dwStyle, rectPtr, parent, nID, pPersist, bStorage, pLicKey);
}

int CWnd::CreateControlContainer(COleControlContainer** p0)
{
    if (!p0) {
        return FALSE;
    }

    auto it = g_controlContainerMap.find(this);
    if (it != g_controlContainerMap.end()) {
        *p0 = it->second;
        return TRUE;
    }

    COleControlContainer* pContainer = new(std::nothrow) COleControlContainer(this);
    if (!pContainer) {
        *p0 = nullptr;
        return FALSE;
    }

    g_controlContainerMap[this] = pContainer;
    *p0 = pContainer;
    return TRUE;
}

int CWnd::CreateControlSite(COleControlContainer* p0, COleControlSite** p1, UINT p2, const GUID*& p3)
{
    (void)p3;
    if (!p1) {
        return FALSE;
    }
    *p1 = nullptr;

    COleControlContainer* pContainer = p0;
    if (!pContainer) {
        if (!CreateControlContainer(&pContainer) || !pContainer) {
            return FALSE;
        }
    }

    COleControlSite* pSite = pContainer->CreateSite(pContainer);
    if (!pSite) {
        return FALSE;
    }

    AttachControlSite(pSite, p2);
    *p1 = pSite;
    if (CWnd* pParent = pContainer->GetWnd()) {
        pSite->m_hWnd = pParent->m_hWnd;
    }
    return TRUE;
}

int CWnd::CreateDlg(const WCHAR* p0, void* p1)
{
    if (!p0) {
        return FALSE;
    }
    CWnd* pParent = p1 ? static_cast<CWnd*>(p1) : nullptr;
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandleW(nullptr);
    }
    HRSRC hTemplate = ::FindResourceW(hInst, p0, MAKEINTRESOURCEW(RT_DIALOG));
    if (!hTemplate) {
        return FALSE;
    }
    HGLOBAL hRes = ::LoadResource(hInst, hTemplate);
    if (!hRes) {
        return FALSE;
    }
    const DLGTEMPLATE* pTemplate = static_cast<const DLGTEMPLATE*>(::LockResource(hRes));
    if (!pTemplate) {
        return FALSE;
    }
    return CreateDlgIndirect(pTemplate, pParent, hInst);
}

int CWnd::CreateDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    if (!p0) {
        return FALSE;
    }
    CWnd* pParent = p1 ? static_cast<CWnd*>(p1) : nullptr;
    HINSTANCE hInst = p2 ? p2 : AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandleW(nullptr);
    }
    HWND hWnd = ::CreateDialogIndirectParamW(
        hInst,
        p0,
        pParent ? pParent->m_hWnd : nullptr,
        reinterpret_cast<DLGPROC>(AfxWndProc),
        reinterpret_cast<LPARAM>(this));
    if (!hWnd) {
        return FALSE;
    }
    m_hWnd = hWnd;
    g_hwndMap[hWnd] = this;
    return TRUE;
}

// Symbol: ?CreateDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* pThis, const DLGTEMPLATE* pTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    if (!pThis) {
        return FALSE;
    }
    return pThis->CreateDlgIndirect(pTemplate, pParentWnd, hInst);
}

int CWnd::CreateRunDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    return CreateDlgIndirect(p0, p1, p2);
}

// Symbol: ?CreateRunDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateRunDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* pThis, const DLGTEMPLATE* pTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    return impl__CreateDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
        pThis, pTemplate, pParentWnd, hInst);
}

LONGLONG CWnd::Default()
{
    if (!m_hWnd) {
        return 0;
    }
    // Re-dispatch the message currently being handled, as real MFC does via the
    // thread's current MSG, rather than default-processing a bogus zero message.
    const MSG* pMsg = GetCurrentMessage();
    if (pMsg && pMsg->message != 0) {
        return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
            this, pMsg->message, pMsg->wParam, pMsg->lParam);
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(this, 0, 0, 0);
}

LONGLONG CWnd::DefWindowProcW(UINT p0, ULONGLONG p1, LONGLONG p2)
{
    if (!m_hWnd) {
        return 0;
    }
    if (m_pfnSuper) {
        return reinterpret_cast<LRESULT>(::CallWindowProcW(m_pfnSuper, m_hWnd, p0, p1, p2));
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(this, p0, p1, p2);
}

HWND CWnd::Detach()
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWnd = m_hWnd;
    m_hWnd = nullptr;
    g_hwndMap.erase(hWnd);
    CleanupWindowRuntimeState(this);
    return hWnd;
}

int CWnd::DoD2DPaint()
{
    return m_hWnd ? 1 : 0;
}

void CWnd::EnableScrollBarCtrl(int p0, int p1)
{
    if (!m_hWnd) {
        return;
    }
    UINT flags = ESB_DISABLE_BOTH;
    if (p0) {
        flags = p1 ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH;
    } else if (p1) {
        flags = ESB_ENABLE_BOTH;
    }
    ::EnableScrollBar(m_hWnd, p0, flags);
}

int CWnd::EnableToolTips(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    GetWindowRuntimeState(this).toolTipsEnabled = p0 != FALSE;
    return TRUE;
}

int CWnd::EnableTrackingToolTips(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    GetWindowRuntimeState(this).trackingToolTipsEnabled = p0 != FALSE;
    return TRUE;
}

int CWnd::ExecuteDlgInit(void* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    if (!p0) {
        return FALSE;
    }
    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);
    return TRUE;
}

int CWnd::ExecuteDlgInit(const WCHAR* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    if (!p0) {
        return FALSE;
    }
    ::SetWindowTextW(m_hWnd, p0);
    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);
    return TRUE;
}

void CWnd::FilterToolTipMessage(MSG* p0)
{
    if (!p0 || !m_hWnd) {
        return;
    }
    if (!GetWindowRuntimeState(this).toolTipsEnabled) {
        return;
    }
    if (p0->message == WM_MOUSEMOVE || p0->message == WM_LBUTTONUP || p0->message == WM_RBUTTONUP) {
        ::SendMessageW(m_hWnd, WM_MOUSEMOVE, p0->wParam, p0->lParam);
    }
}

void* CWnd::FindSiteOrWnd(const COleControlSiteOrWnd* p0) const
{
    if (!p0) {
        return nullptr;
    }
    auto it = g_wndRuntimeStates.find(const_cast<CWnd*>(this));
    if (it != g_wndRuntimeStates.end()) {
        for (const auto& entry : it->second.properties) {
            if (entry.second == p0) {
                return entry.second;
            }
        }
    }
    return const_cast<COleControlSiteOrWnd*>(p0);
}

void* CWnd::FindSiteOrWndWithFocus() const
{
    HWND hFocus = ::GetFocus();
    if (!hFocus) {
        return nullptr;
    }
    return FindSiteOrWnd(reinterpret_cast<const COleControlSiteOrWnd*>(CWnd::FromHandle(hFocus)));
}

long CWnd::GetAccessibilityHitTest(long p0, long p1, VARIANT* p2)
{
    if (!m_hWnd || !p2) {
        return 0;
    }
    // Default to returning self unless a child window explicitly owns the point.
    POINT pt = {p0, p1};
    ::ClientToScreen(m_hWnd, &pt);

    VARIANT result = {};
    result.vt = VT_I4;
    result.lVal = 0; // CHILDID_SELF

    HWND hHit = ::WindowFromPoint(pt);
    if (hHit && hHit != m_hWnd) {
        HWND hChild = ::GetWindow(m_hWnd, GW_CHILD);
        while (hChild) {
            RECT rc{};
            if (::GetWindowRect(hChild, &rc) && pt.x >= rc.left && pt.x < rc.right && pt.y >= rc.top && pt.y < rc.bottom) {
                result.lVal = static_cast<long>(::GetDlgCtrlID(hChild));
                break;
            }
            hChild = ::GetWindow(hChild, GW_HWNDNEXT);
        }
    }

    *p2 = result;
    return 0;
}

long CWnd::GetAccessibilityLocation(VARIANT p0, long* p1, long* p2, long* p3, long* p4)
{
    if (!m_hWnd || !p1 || !p2 || !p3 || !p4) {
        return 0;
    }

    HWND hTarget = m_hWnd;
    if (p0.vt == VT_I4 && p0.lVal > 0) {
        hTarget = ::GetDlgItem(m_hWnd, static_cast<int>(p0.lVal));
    }
    if (!hTarget) {
        return 0;
    }

    RECT rc{};
    if (!::GetWindowRect(hTarget, &rc)) {
        return 0;
    }
    *p1 = rc.left;
    *p2 = rc.top;
    *p3 = rc.right;
    *p4 = rc.bottom;
    return 0;
}

long CWnd::GetAccessibleChild(VARIANT p0, IDispatch** p1)
{
    if (!p1) {
        return 0;
    }
    *p1 = nullptr;

    if (!m_hWnd) {
        return 0;
    }

    // Child enumeration for accessibility can return NULL for unsupported children.
    // We expose child HWND/ID data through VARIANT, while there is no concrete
    // IDispatch wrapper in this clean-room port, so return S_FALSE by not
    // producing a dispatch pointer.
    (void)p0;
    return 0;
}

long CWnd::GetAccessibleChildCount()
{
    if (!m_hWnd) {
        return 0;
    }
    long count = 0;
    for (HWND hChild = ::GetWindow(m_hWnd, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        ++count;
    }
    return count;
}

long CWnd::GetAccessibleName(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    if (!p1) {
        return 0;
    }

    *p1 = nullptr;
    if (!m_hWnd) {
        return 0;
    }
    int len = ::GetWindowTextLengthW(m_hWnd);
    if (len <= 0) {
        return 0;
    }
    ++len;
    WCHAR* text = static_cast<WCHAR*>(::CoTaskMemAlloc(static_cast<size_t>(len) * sizeof(WCHAR)));
    if (!text) {
        return 0;
    }
    int copied = ::GetWindowTextW(m_hWnd, text, len);
    if (copied <= 0) {
        ::CoTaskMemFree(text);
        return 0;
    }
    text[copied] = L'\0';
    *p1 = text;
    return 1;
}

int CWnd::GetCheckedRadioButton(int p0, int p1) const
{
    if (!m_hWnd) {
        return 0;
    }
    for (int id = p0; id <= p1; ++id) {
        HWND hBtn = ::GetDlgItem(m_hWnd, id);
        if (!hBtn) {
            continue;
        }
        LRESULT check = ::SendMessageW(hBtn, BM_GETCHECK, 0, 0);
        if (check == BST_CHECKED) {
            return id;
        }
    }
    return 0;
}

COleControlContainer* CWnd::GetControlContainer()
{
    auto it = g_controlContainerMap.find(this);
    if (it != g_controlContainerMap.end()) {
        return it->second;
    }
    return nullptr;
}

IUnknown* CWnd::GetControlUnknown()
{
    if (!m_hWnd) {
        return nullptr;
    }

    auto it = g_controlContainerMap.find(this);
    if (it == g_controlContainerMap.end() || !it->second) {
        return nullptr;
    }

    COleControlContainer* pContainer = it->second;
    CPtrList& sites = pContainer->m_listSites;
    for (CPtrList::POSITION pos = sites.GetHeadPosition(); pos != nullptr;) {
        COleControlSite* pSite = static_cast<COleControlSite*>(sites.GetNext(pos));
        if (!pSite) {
            continue;
        }
        if ((pSite->m_hWnd == m_hWnd || pSite->m_pControl == this) && pSite->m_lpObject) {
            return pSite->m_lpObject;
        }
    }

    return nullptr;
}

int CWnd::GetDlgCtrlID() const
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<int>(::GetDlgCtrlID(m_hWnd));
}

UINT CWnd::GetDlgItemInt(int p0, int* p1, int p2) const
{
    if (!m_hWnd) {
        return 0;
    }
    BOOL translated = FALSE;
    UINT value = ::GetDlgItemInt(m_hWnd, p0, &translated, p2 != FALSE);
    if (p1) {
        *p1 = translated ? 1 : 0;
    }
    return value;
}

int CWnd::GetDlgItemTextW(int p0, void*& p1, void** p2, void* p3) const
{
    if (!m_hWnd || !p1) {
        return 0;
    }

    HWND hItem = ::GetDlgItem(m_hWnd, p0);
    if (!hItem) {
        return 0;
    }

    // Backward-compatible "wide string" text extraction path:
    // If p2 is provided as a pointer-to-pointer, return freshly allocated text.
    int len = ::GetWindowTextLengthW(hItem);
    if (len <= 0) {
        *reinterpret_cast<void**>(p2) = nullptr;
        return 0;
    }

    ++len;
    WCHAR* text = static_cast<WCHAR*>(::CoTaskMemAlloc(static_cast<size_t>(len) * sizeof(WCHAR)));
    if (!text) {
        return 0;
    }
    int copied = ::GetWindowTextW(hItem, text, len);
    if (copied <= 0) {
        ::CoTaskMemFree(text);
        text = nullptr;
        copied = 0;
    }

    if (copied > 0) {
        if (p3) {
            // caller-provided destination pointer
            auto* dest = static_cast<WCHAR*>(p3);
            ::lstrcpynW(dest, text, static_cast<int>(copied + 1));
        }
        p1 = text;
        if (p2) {
            *reinterpret_cast<void**>(p2) = text;
        }
    } else {
        if (p2) {
            *reinterpret_cast<void**>(p2) = nullptr;
        }
    }
    return copied;
}

int CWnd::GetDlgItemTextW(int p0, WCHAR* p1, int p2) const
{
    if (!m_hWnd || !p1 || p2 <= 0) {
        return 0;
    }
    return ::GetDlgItemTextW(m_hWnd, p0, p1, p2);
}

// Symbol: ?GetDlgItemTextW@CWnd@@QEBAHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetDlgItemTextW_CWnd__QEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, int nID, CString* rString) {
    if (!rString) {
        return 0;
    }
    rString->Empty();
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }

    HWND hItem = ::GetDlgItem(pThis->m_hWnd, nID);
    int length = hItem ? ::GetWindowTextLengthW(hItem) : 0;
    if (length <= 0) {
        return 0;
    }

    wchar_t* buffer = rString->GetBuffer(length + 1);
    int copied = ::GetWindowTextW(hItem, buffer, length + 1);
    rString->ReleaseBuffer(copied > 0 ? copied : 0);
    return copied > 0 ? copied : 0;
}

IUnknown* CWnd::GetDSCCursor()
{
    return GetControlUnknown();
}

DWORD CWnd::GetExStyle() const
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<DWORD>(::GetWindowLongPtrW(m_hWnd, GWL_EXSTYLE));
}

int CWnd::GetGestureConfig(CGestureConfig* p0)
{
    if (!m_hWnd) {
        return 0;
    }
    (void)p0;
    return GetWindowRuntimeState(this).gestureConfigEnabled ? TRUE : FALSE;
}

DWORD CWnd::GetGestureStatus(CPoint p0)
{
    (void)p0;
    return GetWindowRuntimeState(this).d2dSupportEnabled ? 1u : 0u;
}

const AFX_INTERFACEMAP* CWnd::GetInterfaceMap() const
{
    return GetThisInterfaceMap();
}

// GetMessageMap is provided by DECLARE_MESSAGE_MAP macro
/*
const AFX_MSGMAP* CWnd::GetMessageMap() const
{
    return nullptr;
}
*/

COleControlSiteOrWnd* CWnd::GetNextDlgGroupItem(void* p0) const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStart = p0 ? static_cast<CWnd*>(p0)->m_hWnd : nullptr;
    HWND hWndNext = ::GetNextDlgGroupItem(m_hWnd, hWndStart, TRUE);
    return hWndNext ? reinterpret_cast<COleControlSiteOrWnd*>(CWnd::FromHandle(hWndNext)) : nullptr;
}

COleControlSiteOrWnd* CWnd::GetNextDlgTabItem(void* p0, int p1) const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStart = p0 ? static_cast<CWnd*>(p0)->m_hWnd : nullptr;
    HWND hWndNext = ::GetNextDlgTabItem(m_hWnd, hWndStart, p1 != FALSE);
    return hWndNext ? reinterpret_cast<COleControlSiteOrWnd*>(CWnd::FromHandle(hWndNext)) : nullptr;
}

_AFX_OCC_DIALOG_INFO* CWnd::GetOccDialogInfo()
{
    auto it = g_wndRuntimeStates.find(this);
    if (it == g_wndRuntimeStates.end()) {
        return nullptr;
    }
    return reinterpret_cast<_AFX_OCC_DIALOG_INFO*>(it->second.properties[{0x4F43, 0}]);
}

COleControlSite* CWnd::GetOleControlSite(UINT p0) const
{
    COleControlContainer* pContainer = const_cast<CWnd*>(this)->GetControlContainer();
    if (!pContainer) {
        return nullptr;
    }
    return pContainer->FindItem(p0);
}

CFrameWnd* CWnd::GetParentFrame() const
{
    CWnd* pParent = GetParent();
    while (pParent) {
        if (auto* pFrame = dynamic_cast<CFrameWnd*>(pParent)) {
            return pFrame;
        }
        pParent = pParent->GetParent();
    }
    return nullptr;
}

void* CWnd::GetParentOwner() const
{
    if (!m_hWnd) {
        return nullptr;
    }
    return static_cast<void*>(GetTopLevelOwner());
}

COleControlSiteOrWnd* CWnd::GetPrevDlgGroupItem(void* p0) const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStart = p0 ? static_cast<CWnd*>(p0)->m_hWnd : nullptr;
    HWND hWndPrev = ::GetNextDlgGroupItem(m_hWnd, hWndStart, FALSE);
    return hWndPrev ? reinterpret_cast<COleControlSiteOrWnd*>(CWnd::FromHandle(hWndPrev)) : nullptr;
}

void CWnd::GetProperty(long p0, WORD p1, void* p2) const
{
    if (!p2) {
        return;
    }
    auto it = g_wndRuntimeStates.find(const_cast<CWnd*>(this));
    if (it == g_wndRuntimeStates.end()) {
        *reinterpret_cast<void**>(p2) = nullptr;
        return;
    }

    auto propIt = it->second.properties.find({p0, p1});
    if (propIt == it->second.properties.end()) {
        *reinterpret_cast<void**>(p2) = nullptr;
        return;
    }
    *reinterpret_cast<void**>(p2) = propIt->second;
}

CHwndRenderTarget* CWnd::GetRenderTarget()
{
    // CHwndRenderTarget is ABI-compatible with the render-target family in this
    // port and shares the same runtime-slot allocation strategy as CDCRenderTarget.
    return reinterpret_cast<CHwndRenderTarget*>(GetDCRenderTarget());
}

DWORD CWnd::GetStyle() const
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<DWORD>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
}

void* CWnd::GetSuperWndProcAddr()
{
    return reinterpret_cast<void*>(m_pfnSuper);
}

CFrameWnd* CWnd::GetTopLevelFrame() const
{
    CWnd* pWnd = const_cast<CWnd*>(this)->GetTopLevelParent() ? CWnd::FromHandle(
        static_cast<HWND>(const_cast<CWnd*>(this)->GetTopLevelParent())) : nullptr;
    while (pWnd) {
        CFrameWnd* pFrame = dynamic_cast<CFrameWnd*>(pWnd);
        if (pFrame) {
            return pFrame;
        }
        pWnd = pWnd->GetParent();
    }
    return nullptr;
}

void* CWnd::GetTopLevelOwner() const
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hOwner = ::GetWindow(m_hWnd, GW_OWNER);
    if (!hOwner) {
        return m_hWnd;
    }
    HWND hPrevOwner = hOwner;
    while ((hPrevOwner = ::GetWindow(hPrevOwner, GW_OWNER)) != nullptr) {
        hOwner = hPrevOwner;
    }
    return hOwner;
}

void* CWnd::GetTopLevelParent() const
{
    if (!m_hWnd) {
        return nullptr;
    }
    CWnd* pParent = GetParent();
    if (!pParent) {
        return m_hWnd;
    }
    CWnd* pTop = pParent;
    while (pTop && pTop->GetParent()) {
        pTop = pTop->GetParent();
    }
    return pTop ? pTop->m_hWnd : m_hWnd;
}

long CWnd::GetWindowLessChildCount()
{
    if (!m_hWnd) {
        return 0;
    }
    long count = 0;
    for (HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD); hWnd; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) {
        if ((::GetWindowLongPtrW(hWnd, GWL_STYLE) & WS_CLIPSIBLINGS) == 0) {
            ++count;
        }
    }
    return count;
}

int CWnd::GetWindowPlacement(WINDOWPLACEMENT* p0) const
{
    if (!m_hWnd || !p0) {
        return FALSE;
    }
    p0->length = sizeof(*p0);
    return ::GetWindowPlacement(m_hWnd, p0);
}

long CWnd::GetWindowedChildCount()
{
    if (!m_hWnd) {
        return 0;
    }
    long count = 0;
    for (HWND hWnd = ::GetWindow(m_hWnd, GW_CHILD); hWnd; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT)) {
        ++count;
    }
    return count;
}

int CWnd::HandleFloatingSysCommand(UINT p0, INT_PTR p1)
{
    if (!m_hWnd) {
        return FALSE;
    }
    return static_cast<int>(::SendMessageW(m_hWnd, WM_SYSCOMMAND, p0, p1));
}

int CWnd::InitControlContainer(int p0)
{
    COleControlContainer* pContainer = nullptr;
    int result = CreateControlContainer(&pContainer);
    (void)p0;
    return result;
}

void CWnd::InvokeHelper(long p0, WORD p1, void* p2, VARIANT* p3)
{
    if (p3) {
        // Keep return storage in a known state even when no automation target is
        // attached to this window.
        ::VariantClear(p3);
        ::VariantInit(p3);
        p3->vt = VT_EMPTY;
    }

    if (p0 == 0) {
        return;
    }

    IDispatch* pDispatch = nullptr;
    if (IUnknown* pUnknown = GetControlUnknown()) {
        if (FAILED(pUnknown->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pDispatch))) || !pDispatch) {
            return;
        }
    } else {
        // Best-effort fallback: forward well-known automation-style verbs to the
        // window message stream when no control dispatch is present.
        if (m_hWnd && p1 == DISPATCH_METHOD && p3 && p3->vt == VT_EMPTY) {
            p3->vt = VT_I4;
            p3->lVal = static_cast<LONG>(::SendMessageW(m_hWnd, WM_COMMAND, static_cast<WPARAM>(p0), 0));
        }
        return;
    }

    if (!p3) {
        DISPPARAMS noParams = {};
        pDispatch->Invoke(p0, IID_NULL, LOCALE_USER_DEFAULT, p1, &noParams, nullptr, nullptr, nullptr);
        pDispatch->Release();
        return;
    }

    DISPPARAMS noParams = {};
    VARIANT result;
    VariantInit(&result);
    EXCEPINFO excep = {};
    UINT argErr = 0;
    HRESULT hr = pDispatch->Invoke(static_cast<DISPID>(p0), IID_NULL, LOCALE_USER_DEFAULT,
                                   p1, &noParams, &result, &excep, &argErr);
    pDispatch->Release();

    if (FAILED(hr)) {
        if (p3) {
            p3->vt = VT_ERROR;
            p3->scode = static_cast<SCODE>(hr);
        }
        return;
    }

    *p3 = result;
}

UINT CWnd::IsDlgButtonChecked(int p0) const
{
    if (!m_hWnd) {
        return BST_UNCHECKED;
    }
    HWND hControl = ::GetDlgItem(m_hWnd, p0);
    if (!hControl) {
        return BST_UNCHECKED;
    }
    return static_cast<UINT>(::SendMessageW(hControl, BM_GETCHECK, 0, 0));
}

int CWnd::IsTopParentActive() const
{
    if (!m_hWnd) {
        return FALSE;
    }
    HWND hWndTop = static_cast<HWND>(GetTopLevelParent());
    if (!hWndTop) {
        return FALSE;
    }
    return (::GetActiveWindow() == hWndTop || ::GetForegroundWindow() == hWndTop) ? TRUE : FALSE;
}

int CWnd::IsTouchWindow() const
{
    if (!m_hWnd) {
        return FALSE;
    }
    return ::IsTouchWindow(m_hWnd, nullptr) ? TRUE : FALSE;
}

int CWnd::MessageBoxW(const WCHAR* p0, const WCHAR* p1, UINT p2)
{
    return static_cast<int>(::MessageBoxW(m_hWnd, p0, p1, p2));
}

LONGLONG CWnd::OnActivateTopLevel(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    // Keep the frame visually responsive when activated from shell-driven flows.
    if (p0) {
        ::ShowWindow(m_hWnd, SW_SHOWNA);
        ::BringWindowToTop(m_hWnd);
        if (p1 != 0) {
            ::UpdateWindow(m_hWnd);
        }
    }
    return 0;
}

int CWnd::OnCharToItem(UINT p0, CListBox* p1, UINT p2)
{
    if (!p1 || !p1->m_hWnd) {
        return -1;
    }

    wchar_t ch[2] = { static_cast<wchar_t>(p0), 0 };
    int start = static_cast<int>(p2);
    if (start < -1) {
        start = -1;
    }

    LRESULT result = ::SendMessageW(p1->m_hWnd, LB_SELECTSTRING, static_cast<WPARAM>(start), reinterpret_cast<LPARAM>(ch));
    return static_cast<int>(result);
}

int CWnd::OnChildNotify(UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3)
{
    if (!m_hWnd || !p3) {
        return 0;
    }

    if (p0 == WM_COMMAND) {
        return CWnd::OnCommand(static_cast<uintptr_t>(p1), p2);
    }
    if (p0 == WM_NOTIFY) {
        intptr_t result = static_cast<intptr_t>(*p3);
        int handled = CWnd::OnNotify(static_cast<uintptr_t>(p1), p2, &result);
        *p3 = result;
        return handled;
    }

    return 0;
}

int CWnd::OnCompareItem(int p0, COMPAREITEMSTRUCT* p1)
{
    (void)p0;
    if (!p1 || !m_hWnd) {
        return 0;
    }

    if (p1->itemData1 == p1->itemData2) {
        return 0;
    }
    if (p1->itemData1 > p1->itemData2) {
        return 1;
    }
    if (p1->itemData1 < p1->itemData2) {
        return -1;
    }
    return 0;
}

HBRUSH CWnd::OnCtlColor(CDC* p0, void* p1, UINT p2)
{
    if (!p0 || !p0->m_hDC) {
        return nullptr;
    }

    auto* pWnd = reinterpret_cast<CWnd*>(p1);
    UINT style = pWnd && pWnd->m_hWnd ? static_cast<UINT>(::GetWindowLongPtrW(pWnd->m_hWnd, GWL_STYLE)) : 0;

    switch (p2) {
        case CTLCOLOR_STATIC:
            if ((style & SS_BLACKFRAME) == 0) {
                ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_WINDOWTEXT));
                ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_BTNFACE));
            }
            return ::GetSysColorBrush(COLOR_BTNFACE);
        case CTLCOLOR_LISTBOX:
            ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_WINDOWTEXT));
            ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_WINDOW));
            return ::GetSysColorBrush(COLOR_WINDOW);
        case CTLCOLOR_EDIT:
            ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_WINDOWTEXT));
            ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_WINDOW));
            return ::GetSysColorBrush(COLOR_WINDOW);
        case CTLCOLOR_BTN:
            ::SetTextColor(p0->m_hDC, GetSysColor(COLOR_BTNTEXT));
            ::SetBkColor(p0->m_hDC, GetSysColor(COLOR_BTNFACE));
            return ::GetSysColorBrush(COLOR_BTNFACE);
        default:
            return ::GetSysColorBrush(COLOR_WINDOW);
    }
}

int CWnd::OnDeleteItem(int p0, DELETEITEMSTRUCT* p1)
{
    (void)p0;
    if (!p1) {
        return 0;
    }
    // No action if the item data is not a heap-allocated pointer. We keep this
    // lightweight by not guessing ownership and only cleaning up if p1 explicitly
    // marks an item as dynamic data.
    if (p1->itemData) {
        return 1;
    }
    return 0;
}

void CWnd::OnDestroy()
{
    if (m_hWnd) {
        CleanupWindowRuntimeState(this);
    }
}

void CWnd::OnDevModeChange(void* p0, const WCHAR* p1)
{
    if (!m_hWnd) {
        return;
    }
    // p1 is typically a form driver description string on Windows.
    (void)p0;
    if (p1) {
        ::SetWindowTextW(m_hWnd, p1);
    }
    ::RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

void CWnd::OnDisplayChange(UINT p0, int p1, int p2)
{
    if (!m_hWnd) {
        return;
    }
    (void)p0;
    (void)p1;
    (void)p2;
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
    ::UpdateWindow(m_hWnd);
}

LONGLONG CWnd::OnDragList(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    const DRAGLISTINFO* pInfo = reinterpret_cast<const DRAGLISTINFO*>(static_cast<INT_PTR>(p1));
    HWND hList = (pInfo && pInfo->hWnd) ? pInfo->hWnd : reinterpret_cast<HWND>(static_cast<INT_PTR>(p0));
    if (!hList || !::IsWindow(hList)) {
        return 0;
    }

    UINT notification = pInfo ? pInfo->uNotification : static_cast<UINT>(p0);
    CPoint localPt{};
    if (pInfo) {
        localPt = CPoint(pInfo->ptCursor.x, pInfo->ptCursor.y);
    } else {
        POINT ptCursor{};
        ::GetCursorPos(&ptCursor);
        if (hList) {
            ::ScreenToClient(hList, &ptCursor);
            localPt = CPoint(ptCursor.x, ptCursor.y);
        }
    }

    int item = static_cast<int>(::SendMessageW(hList, LB_ITEMFROMPOINT, 0, MAKELPARAM(localPt.x, localPt.y)));
    if (item == LB_ERR) {
        return 0;
    }
    item = LOWORD(item);

    if (item >= 0) {
        item = LOWORD(item);
    }

    switch (notification) {
    case DL_BEGINDRAG:
        ::SendMessageW(hList, LB_SETTOPINDEX, static_cast<WPARAM>(item), 0);
        break;
    case DL_DRAGGING:
        ::SendMessageW(hList, LB_SETTOPINDEX, static_cast<WPARAM>(item), 0);
        break;
    case DL_DROPPED:
        ::SendMessageW(hList, LB_SETCURSEL, static_cast<WPARAM>(item), 0);
        break;
    case DL_CANCELDRAG:
        ::SendMessageW(hList, LB_SETCURSEL, static_cast<WPARAM>(-1), 0);
        break;
    default:
        break;
    }

    ::InvalidateRect(hList, nullptr, TRUE);
    return 1;
}

int CWnd::OnDrawItem(int p0, DRAWITEMSTRUCT* p1)
{
    (void)p0;
    if (!p1 || !p1->hDC) {
        return 0;
    }

    HDC hdc = p1->hDC;
    RECT rc = p1->rcItem;
    const WCHAR* label = reinterpret_cast<const WCHAR*>(p1->itemData);
    if (!label) {
        label = L"";
    }

    COLORREF textColor = (p1->itemState & ODS_SELECTED) ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_WINDOWTEXT);
    COLORREF backColor = (p1->itemState & ODS_SELECTED) ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW);
    ::SetTextColor(hdc, textColor);
    ::SetBkColor(hdc, backColor);
    HBRUSH hBg = ::CreateSolidBrush(backColor);
    if (hBg) {
        ::FillRect(hdc, &rc, hBg);
        ::DeleteObject(hBg);
    }
    ::ExtTextOutW(hdc, rc.left + 2, rc.top + 1, 0, &rc, label, static_cast<UINT>(lstrlenW(label)), nullptr);
    if (p1->itemState & ODS_FOCUS) {
        ::DrawFocusRect(hdc, &rc);
    }
    return 1;
}

void CWnd::OnEnterIdle(UINT p0, void* p1)
{
    if (!m_hWnd) {
        return;
    }

    CWnd* pWho = static_cast<CWnd*>(p1);
    CString statusText;
    switch (p0) {
    case MSGF_MENU:
    case MSGF_DIALOGBOX:
    case MSGF_MESSAGEBOX:
        statusText = L"Message loop idle";
        break;
    default:
        statusText = L"Idle";
        break;
    }

    if (pWho && pWho->m_hWnd && pWho != this) {
        int titleLen = pWho->GetWindowTextLengthW();
        if (titleLen > 0) {
            CString titleText;
            wchar_t* pBuf = titleText.GetBuffer(titleLen + 1);
            int copied = pWho->GetWindowTextW(pBuf, titleLen + 1);
            titleText.ReleaseBuffer(copied);
            statusText = titleText.GetString();
        } else {
            statusText = L"Idle";
        }
    }

    CWnd_SetMessageText(this, statusText.GetString());
}

void CWnd::OnGesture(void* p0, ULONGLONG p1, LONGLONG p2)
{
    (void)p0;
    (void)p1;

    if (!m_hWnd || p2 == 0) {
        return;
    }

    HGESTUREINFO hGesture = reinterpret_cast<HGESTUREINFO>(static_cast<INT_PTR>(p2));
    GESTUREINFO gestureInfo {};
    gestureInfo.cbSize = sizeof(gestureInfo);
    if (!::GetGestureInfo(hGesture, &gestureInfo)) {
        return;
    }

    POINT ptScreen = { gestureInfo.ptsLocation.x, gestureInfo.ptsLocation.y };
    POINT ptClient = ptScreen;
    ::ScreenToClient(m_hWnd, &ptClient);
    CPoint point(ptClient.x, ptClient.y);

    int handled = FALSE;
    switch (gestureInfo.dwID) {
    case GID_PAN:
        handled = OnGesturePan(p0, point);
        break;
    case GID_PRESSANDTAP:
        handled = OnGesturePressAndTap(p0, point, gestureInfo.dwFlags);
        break;
    case GID_ROTATE:
        handled = OnGestureRotate(p0, point, static_cast<DWORD>(gestureInfo.ullArguments));
        break;
    case GID_TWOFINGERTAP:
        handled = OnGestureTwoFingerTap(p0, point);
        break;
    case GID_ZOOM:
        handled = OnGestureZoom(p0, point, static_cast<DWORD>(gestureInfo.ullArguments));
        break;
    default:
        break;
    }

    if (!handled) {
        CWnd_SetMessageText(this, L"Unhandled gesture message");
    }
    ::CloseGestureInfoHandle(hGesture);
}

int CWnd::OnGesturePan(void* p0, CPoint p1)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGesturePan(p0, childPt);
        }
    }

    (void)p0;
    CWnd_SetMessageText(this, L"Gesture pan");
    return TRUE;
}

int CWnd::OnGesturePressAndTap(void* p0, CPoint p1, DWORD p2)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGesturePressAndTap(p0, childPt, p2);
        }
    }

    HWND hContextWnd = hChild ? hChild : m_hWnd;
    if (hContextWnd) {
        ::PostMessageW(hContextWnd, WM_CONTEXTMENU, reinterpret_cast<WPARAM>(m_hWnd), MAKELPARAM(pt.x, pt.y));
    }
    CWnd_SetMessageText(this, L"Gesture press and tap");
    (void)p0;
    (void)p2;
    return TRUE;
}

int CWnd::OnGestureRotate(void* p0, CPoint p1, DWORD p2)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGestureRotate(p0, childPt, p2);
        }
    }

    CString angleText;
    angleText.Format(L"Gesture rotate: %lu", static_cast<unsigned long>(p2));
    CWnd_SetMessageText(this, angleText.GetString());
    (void)p0;
    (void)p2;
    return TRUE;
}

int CWnd::OnGestureTwoFingerTap(void* p0, CPoint p1)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGestureTwoFingerTap(p0, childPt);
        }
    }

    HWND hContextWnd = hChild ? hChild : m_hWnd;
    if (hContextWnd) {
        ::PostMessageW(hContextWnd, WM_CONTEXTMENU, reinterpret_cast<WPARAM>(m_hWnd), MAKELPARAM(pt.x, pt.y));
    }
    CWnd_SetMessageText(this, L"Gesture two-finger tap");
    (void)p0;
    return TRUE;
}

int CWnd::OnGestureZoom(void* p0, CPoint p1, DWORD p2)
{
    if (!m_hWnd) {
        return FALSE;
    }

    POINT pt = {p1.x, p1.y};
    ::ClientToScreen(m_hWnd, &pt);
    HWND hChild = ::WindowFromPoint(pt);
    if (hChild && hChild != m_hWnd) {
        CWnd* pChild = CWnd::FromHandle(hChild);
        RECT rc{};
        if (pChild && pChild->m_hWnd && ::GetWindowRect(hChild, &rc)) {
            CPoint childPt{pt.x - rc.left, pt.y - rc.top};
            return pChild->OnGestureZoom(p0, childPt, p2);
        }
    }

    if (p2) {
        ::SendMessageW(m_hWnd, WM_MOUSEWHEEL, MAKEWPARAM(0, p2 > 0 ? 120 : -120), MAKELPARAM(pt.x, pt.y));
    }
    CWnd_SetMessageText(this, L"Gesture zoom");
    (void)p0;
    (void)p2;
    return TRUE;
}

LONGLONG CWnd::OnGetObject(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    if (p0 == 0 || p1 == 0) {
        return 0;
    }

    return static_cast<LONGLONG>(
        ::DefWindowProcW(m_hWnd, WM_GETOBJECT, static_cast<WPARAM>(p0), static_cast<LPARAM>(p1)));
}

void CWnd::OnHScroll(UINT p0, UINT p1, CScrollBar* p2)
{
    if (p2 && p2->m_hWnd) {
        ::SendMessageW(p2->m_hWnd, WM_HSCROLL, MAKELONG(p0, LOWORD(p1)), 0);
        return;
    }

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_HSCROLL, MAKEWPARAM(p0, LOWORD(p1)), 0);
    }
}

void CWnd::OnHelp()
{
    if (!m_hWnd) {
        return;
    }
    auto* pFrame = GetTopLevelFrame();
    if (pFrame) {
        static_cast<CFrameWnd*>(pFrame)->SetMessageText(L"Help requested");
    }
    MessageBoxW(L"Help requested", L"Help", MB_OK | MB_ICONINFORMATION);
}

void CWnd::OnHelpFinder()
{
    OnHelp();
}

void CWnd::OnHelpIndex()
{
    OnHelp();
}

BOOL CWnd::OnHelpInfo(HELPINFO* p0)
{
    if (!p0) {
        return FALSE;
    }
    HELPINFO info = *p0;
    if (!info.dwContextId) {
        return FALSE;
    }
    CString help;
    help.Format(L"Help requested (context: %lu)", static_cast<unsigned long>(info.dwContextId));
    if (m_hWnd) {
        MessageBoxW(help.GetString(), L"Help", MB_OK | MB_ICONINFORMATION);
        return TRUE;
    }
    return FALSE;
}

void CWnd::OnHelpUsing()
{
    OnHelp();
}

void CWnd::OnMeasureItem(int p0, MEASUREITEMSTRUCT* p1)
{
    if (!p1) {
        return;
    }
    p1->itemWidth = p0;
    p1->itemHeight = 16;
}

LONGLONG CWnd::OnNTCtlColor(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }

    (void)p0;
    if (p1 >= 0 && p1 <= 0x0F) {
        return static_cast<LONGLONG>(reinterpret_cast<LONG_PTR>(::GetSysColorBrush(static_cast<int>(p1))));
    }

    return static_cast<LONGLONG>(reinterpret_cast<LONG_PTR>(::GetSysColorBrush(COLOR_WINDOW)));
}

int CWnd::OnNcDestroy()
{
    if (m_hWnd) {
        CleanupWindowRuntimeState(this);
    }
    return 1;
}

void CWnd::OnPaint()
{
    if (!m_hWnd) {
        return;
    }
    PAINTSTRUCT ps {};
    HDC hdc = ::BeginPaint(m_hWnd, &ps);
    if (!hdc) {
        return;
    }
    HBRUSH brush = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
    ::FillRect(hdc, &ps.rcPaint, brush);
    ::EndPaint(m_hWnd, &ps);
}

int CWnd::OnParentNotify(UINT p0, LONGLONG p1)
{
    if (!m_hWnd) {
        return 0;
    }
    return static_cast<int>(::SendMessageW(m_hWnd, p0, static_cast<WPARAM>(p1), static_cast<LPARAM>(p1)));
}

void CWnd::OnSetFocus(void* p0)
{
    if (!p0) {
        return;
    }
    if (m_hWnd) {
        ::SetFocus(m_hWnd);
    }
}

void CWnd::OnSettingChange(UINT p0, const WCHAR* p1)
{
    if (!m_hWnd) {
        return;
    }
    ::SendMessageW(m_hWnd, WM_SETTINGCHANGE, static_cast<WPARAM>(p0), reinterpret_cast<LPARAM>(p1));
}

void CWnd::OnSysColorChange()
{
    if (!m_hWnd) {
        return;
    }
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
}

LONGLONG CWnd::OnTabletQuerySystemGestureStatus(ULONGLONG p0, LONGLONG p1)
{
    (void)p1;
    if (!m_hWnd) {
        return 0;
    }
    return GetWindowRuntimeState(this).gestureConfigEnabled ? static_cast<LONGLONG>(p0) : 0;
}

void CWnd::OnTouchInput(ULONGLONG p0, ULONGLONG p1, TOUCHINPUT* p2, UINT p3)
{
    if (!m_hWnd || !p2) {
        return;
    }

    (void)p0;
    (void)p1;
    // Convert coordinates into pixels (TouchInput stores 1/100 pixel units).
    for (UINT i = 0; i < p3; ++i) {
        TOUCHINPUT& input = p2[i];
        POINT pt = { static_cast<LONG>(input.x / 100), static_cast<LONG>(input.y / 100) };
        ::ScreenToClient(m_hWnd, &pt);
        input.x = static_cast<long>(pt.x);
        input.y = static_cast<long>(pt.y);
    }
}

int CWnd::OnTouchInputs(ULONGLONG p0, ULONGLONG p1, TOUCHINPUT* p2, UINT p3)
{
    if (!m_hWnd || !p2 || p3 == 0) {
        return FALSE;
    }

    for (UINT i = 0; i < p3; ++i) {
        OnTouchInput(p0, p1, &p2[i], 1);
    }
    return static_cast<int>(p3);
}

LONGLONG CWnd::OnTouchMessage(ULONGLONG p0, LONGLONG p1)
{
    if (!m_hWnd || p0 == 0 || p1 == 0) {
        return 0;
    }

    UINT nInputs = static_cast<UINT>(p0);
    if (nInputs == 0) {
        return 0;
    }

    HTOUCHINPUT hInput = reinterpret_cast<HTOUCHINPUT>(static_cast<INT_PTR>(p1));
    std::vector<TOUCHINPUT> inputs(nInputs);
    int result = 0;
    if (::GetTouchInputInfo(hInput, nInputs, inputs.data(), sizeof(TOUCHINPUT))) {
        result = OnTouchInputs(p0, static_cast<ULONGLONG>(p1), inputs.data(), nInputs);
    }
    ::CloseTouchInputHandle(hInput);
    return result;
}

int CWnd::OnVKeyToItem(UINT p0, CListBox* p1, UINT p2)
{
    if (!p1 || !p1->m_hWnd) {
        return -1;
    }

    int itemCount = static_cast<int>(::SendMessageW(p1->m_hWnd, LB_GETCOUNT, 0, 0));
    if (itemCount <= 0) {
        return -1;
    }

    int current = static_cast<int>(p2);
    if (p0 == VK_UP) {
        return current > 0 ? current - 1 : -1;
    }
    if (p0 == VK_DOWN) {
        return (current + 1 < itemCount) ? current + 1 : -1;
    }
    if (p0 == VK_HOME) {
        return itemCount > 0 ? 0 : -1;
    }
    if (p0 == VK_END) {
        return itemCount > 0 ? itemCount - 1 : -1;
    }

    UINT mapped = ::MapVirtualKeyW(p0, MAPVK_VK_TO_CHAR);
    if (!mapped || mapped == 0xFF) {
        return -1;
    }

    wchar_t keyText[2] = { static_cast<wchar_t>(mapped & 0xFFFF), 0 };
    if (!keyText[0]) {
        return -1;
    }
    return static_cast<int>(::SendMessageW(p1->m_hWnd, LB_FINDSTRING, current, reinterpret_cast<LPARAM>(keyText)));
}

void CWnd::OnVScroll(UINT p0, UINT p1, CScrollBar* p2)
{
    if (p2 && p2->m_hWnd) {
        ::SendMessageW(p2->m_hWnd, WM_VSCROLL, MAKEWPARAM(p1, 0), 0);
    }
}

BOOL CWnd::PaintWindowlessControls(CDC* p0)
{
    (void)p0;
    if (!m_hWnd) {
        return FALSE;
    }
    return ::InvalidateRect(m_hWnd, nullptr, FALSE);
}

void CWnd::PostNcDestroy()
{
    if (m_hWnd) {
        CleanupWindowRuntimeState(this);
        g_hwndMap.erase(m_hWnd);
        m_hWnd = nullptr;
    }
}

BOOL CWnd::PreTranslateInput(MSG* p0)
{
    if (!m_hWnd || !p0) {
        return FALSE;
    }

    // Translate system-level input to the same path used for MFC's
    // CWnd::PreTranslateMessage().
    return CWnd_PreTranslateMessageCompat(this, p0);
}

void CWnd::PrepareForHelp()
{
    OnHelp();
}

int CWnd::ReflectChildNotify(UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3)
{
    if (OnChildNotify(p0, p1, p2, p3)) {
        return TRUE;
    }
    return 0;
}

int CWnd::RegisterTouchWindow(ULONGLONG p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    return ::RegisterTouchWindow(m_hWnd, static_cast<ULONG_PTR>(p0)) ? TRUE : FALSE;
}

void CWnd::RemoveRadioCheckFromGroup(const CObject* p0)
{
    (void)p0;
    if (!m_hWnd) {
        return;
    }
    for (HWND hChild = ::GetWindow(m_hWnd, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        DWORD style = static_cast<DWORD>(::GetWindowLongPtrW(hChild, GWL_STYLE));
        if (style & BS_AUTORADIOBUTTON) {
            ::SendMessageW(hChild, BM_SETCHECK, BST_UNCHECKED, 0);
        }
    }
}

void CWnd::RepositionBars(UINT p0, UINT p1, UINT p2, UINT p3, RECT* p4, RECT* p5, int p6)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p5;
    (void)p6;
    if (m_hWnd && p4) {
        ::MoveWindow(m_hWnd, p4->left, p4->top, p4->right - p4->left, p4->bottom - p4->top, TRUE);
    }
}

LONGLONG CWnd::RunModalLoop(DWORD p0)
{
    (void)p0;
    if (!m_hWnd) {
        return 0;
    }
    MSG msg {};
    while (::GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (!CWnd_PreTranslateMessageCompat(this, &msg)) {
            ::TranslateMessage(&msg);
            ::DispatchMessageW(&msg);
        }
        if (msg.message == WM_QUIT) {
            return static_cast<LONGLONG>(msg.wParam);
        }
    }
    return 0;
}

void CWnd::ScreenToClient(RECT* p0) const
{
    if (!m_hWnd || !p0) {
        return;
    }

    POINT points[2] = {
        {p0->left, p0->top},
        {p0->right, p0->bottom}
    };

    if (::MapWindowPoints(HWND_DESKTOP, m_hWnd, points, 2) != 0) {
        p0->left = points[0].x;
        p0->top = points[0].y;
        p0->right = points[1].x;
        p0->bottom = points[1].y;
    }
}

void CWnd::ScrollWindow(int p0, int p1, const RECT* p2, const RECT* p3)
{
    if (!m_hWnd) {
        return;
    }

    ::ScrollWindow(m_hWnd, p0, p1, p2, p3);
}

int CWnd::ScrollWindowEx(int p0, int p1, const RECT* p2, const RECT* p3, CRgn* p4, RECT* p5, UINT p6)
{
    if (!m_hWnd) {
        return FALSE;
    }

    HRGN hRgn = p4 ? static_cast<HRGN>(p4->m_hObject) : nullptr;
    return ::ScrollWindowEx(m_hWnd, p0, p1, p2, p3, hRgn, p5, p6);
}

int CWnd::SendChildNotifyLastMsg(LONGLONG* p0)
{
    const MSG* pMsg = GetCurrentMessage();
    if (!pMsg) {
        if (p0) {
            *p0 = 0;
        }
        return FALSE;
    }

    LONGLONG result = 0;
    if (!OnChildNotify(pMsg->message,
                       static_cast<ULONGLONG>(pMsg->wParam),
                       static_cast<LONGLONG>(pMsg->lParam),
                       &result)) {
        if (p0) {
            *p0 = 0;
        }
        return FALSE;
    }

    if (p0) {
        *p0 = result;
    }
    return TRUE;
}

// Symbol: ?GetScrollPos@CWnd@@QEBAHH@Z
extern "C" int MS_ABI impl__GetScrollPos_CWnd__QEBAHH_Z(const CWnd* pThis, int nBar) {
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }
    return ::GetScrollPos(pThis->m_hWnd, nBar);
}

// Symbol: ?GetScrollInfo@CWnd@@QEAAHHPEAUtagSCROLLINFO@@I@Z
extern "C" int MS_ABI impl__GetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__I_Z(
    CWnd* pThis, int nBar, SCROLLINFO* pScrollInfo, UINT nMask) {
    if (!pThis || !pThis->m_hWnd || !pScrollInfo) {
        return FALSE;
    }
    pScrollInfo->fMask = nMask;
    return ::GetScrollInfo(pThis->m_hWnd, nBar, pScrollInfo);
}

// Symbol: ?GetScrollLimit@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__GetScrollLimit_CWnd__QEAAHH_Z(CWnd* pThis, int nBar) {
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }
    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    if (!::GetScrollInfo(pThis->m_hWnd, nBar, &si)) {
        return 0;
    }
    int page = si.nPage > 0 ? static_cast<int>(si.nPage) - 1 : 0;
    return si.nMax - page;
}

// Symbol: ?GetScrollRange@CWnd@@QEBAXHPEAH0@Z
extern "C" void MS_ABI impl__GetScrollRange_CWnd__QEBAXHPEAH0_Z(
    const CWnd* pThis, int nBar, int* pMinPos, int* pMaxPos) {
    if (pMinPos) {
        *pMinPos = 0;
    }
    if (pMaxPos) {
        *pMaxPos = 0;
    }
    if (pThis && pThis->m_hWnd) {
        ::GetScrollRange(pThis->m_hWnd, nBar, pMinPos, pMaxPos);
    }
}

// Symbol: ?IsDialogMessageW@CWnd@@QEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg) {
    if (!pThis || !pThis->m_hWnd || !pMsg) {
        return FALSE;
    }
    return ::IsDialogMessageW(pThis->m_hWnd, pMsg);
}

// Symbol: ?SendChildNotifyLastMsg@CWnd@@QEAAHPEA_J@Z
extern "C" int MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult) {
    return pThis ? pThis->SendChildNotifyLastMsg(pResult) : FALSE;
}

// Symbol: ?GetScrollBarCtrl@CWnd@@UEBAPEAVCScrollBar@@H@Z
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(const CWnd* pThis, int nBar) {
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    if (nBar != SB_HORZ && nBar != SB_VERT) {
        return nullptr;
    }

    constexpr UINT AFX_IDW_HSCROLL = 0xE812;
    constexpr UINT AFX_IDW_VSCROLL = 0xE811;
    UINT id = (nBar == SB_HORZ) ? AFX_IDW_HSCROLL : AFX_IDW_VSCROLL;
    HWND hScrollBar = ::GetDlgItem(pThis->m_hWnd, static_cast<int>(id));
    if (!hScrollBar) {
        return nullptr;
    }
    return static_cast<CScrollBar*>(CWnd::FromHandle(hScrollBar));
}

// Symbol: ?IsFrameWnd@CWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis) {
    return (pThis && dynamic_cast<const CFrameWnd*>(pThis) != nullptr) ? TRUE : FALSE;
}

// Symbol: ?OnAmbientProperty@CWnd@@UEAAHPEAVCOleControlSite@@JPEAUtagVARIANT@@@Z
extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar)
{
    if (!pThis || !pVar) {
        return FALSE;
    }
    (void)pThis;

    const VARTYPE requested = pVar->vt;
    VariantInit(pVar);
    auto set_bool = [&](BOOL value) -> int {
        pVar->vt = VT_BOOL;
        pVar->boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
        return TRUE;
    };
    auto set_long = [&](long value) -> int {
        pVar->vt = VT_I4;
        pVar->lVal = value;
        return TRUE;
    };
    auto set_bstr = [&](const wchar_t* value) -> int {
        pVar->vt = VT_BSTR;
        pVar->bstrVal = ::SysAllocString(value ? value : L"");
        return TRUE;
    };

    auto site_bool = [&](VARIANT_BOOL* out) -> bool {
        if (!pSite) {
            return false;
        }
        return pSite->GetAmbientProperty(dispid, VT_BOOL, out) == TRUE;
    };
    auto site_long = [&](long* out) -> bool {
        if (!pSite) {
            return false;
        }
        return pSite->GetAmbientProperty(dispid, VT_I4, out) == TRUE;
    };
    auto site_bstr = [&](BSTR* out) -> bool {
        if (!pSite) {
            return false;
        }
        return pSite->GetAmbientProperty(dispid, VT_BSTR, out) == TRUE;
    };

    switch (dispid) {
    case DISPID_AMBIENT_USERMODE:
        if ((requested == VT_BOOL || requested == VT_EMPTY) && pSite) {
            VARIANT_BOOL site = VARIANT_FALSE;
            if (site_bool(&site)) {
                return set_bool(site == VARIANT_TRUE);
            }
            return set_bool(TRUE);
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2) {
            long value = 1;
            if (pSite && site_long(&value)) {
                return set_long(value);
            }
            return set_long(1);
        }
        if (requested == VT_EMPTY) {
            return set_bool(TRUE);
        }
        return FALSE;

    case DISPID_AMBIENT_BACKCOLOR:
    case DISPID_AMBIENT_FORECOLOR:
        if (requested == VT_BSTR) {
            return FALSE;
        }
        if ((requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) && pSite) {
            long color = 0;
            if (site_long(&color)) {
                return set_long(color);
            }
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) {
            long color = (dispid == DISPID_AMBIENT_BACKCOLOR)
                ? static_cast<long>(::GetSysColor(COLOR_WINDOW))
                : static_cast<long>(::GetSysColor(COLOR_WINDOWTEXT));
            return set_long(color);
        }
        return FALSE;

    case DISPID_AMBIENT_DISPLAYNAME:
    case DISPID_AMBIENT_SCALEUNITS:
        if ((requested == VT_BSTR || requested == VT_EMPTY)) {
            if (pSite) {
                BSTR value = nullptr;
                if (site_bstr(&value)) {
                    return set_bstr(value);
                }
            }
            return set_bstr(L"");
        }
        return FALSE;

    case DISPID_AMBIENT_UIDEAD:
    case DISPID_AMBIENT_SHOWGRABHANDLES:
    case DISPID_AMBIENT_SHOWHATCHING:
    case DISPID_AMBIENT_DISPLAYASDEFAULT:
    case DISPID_AMBIENT_SUPPORTSMNEMONICS:
        if ((requested == VT_BOOL || requested == VT_EMPTY) && pSite) {
            VARIANT_BOOL value = VARIANT_FALSE;
            if (site_bool(&value)) {
                return set_bool(value == VARIANT_TRUE);
            }
        }
        if (requested == VT_BOOL || requested == VT_EMPTY) {
            BOOL value = (dispid == DISPID_AMBIENT_SHOWHATCHING || dispid == DISPID_AMBIENT_UIDEAD) ? FALSE : TRUE;
            return set_bool(value);
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2) {
            long value = (dispid == DISPID_AMBIENT_SHOWHATCHING || dispid == DISPID_AMBIENT_UIDEAD) ? 0 : 1;
            if (pSite && site_long(&value)) {
                return set_long(value);
            }
            return set_long(value);
        }
        return FALSE;

    case DISPID_AMBIENT_LOCALEID:
        if (requested == VT_BSTR) {
            return FALSE;
        }
        if ((requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) && pSite) {
            long locale = 0;
            if (site_long(&locale)) {
                return set_long(locale);
            }
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_EMPTY) {
            return set_long(static_cast<long>(::GetUserDefaultLCID()));
        }
        return FALSE;

    case DISPID_AMBIENT_APPEARANCE:
        if (requested == VT_BOOL) {
            VARIANT_BOOL value = VARIANT_FALSE;
            if (site_bool(&value)) {
                return set_bool(value == VARIANT_TRUE);
            }
            return set_bool(FALSE);
        }
        if ((requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2) && pSite) {
            long value = 0;
            if (site_long(&value)) {
                return set_long(value);
            }
        }
        if (requested == VT_I4 || requested == VT_I2 || requested == VT_UI4 || requested == VT_UI2 || requested == VT_EMPTY) {
            return set_long(0);
        }
        return FALSE;

    case DISPID_AMBIENT_FONT:
    case DISPID_AMBIENT_TEXTALIGN:
        return FALSE;

    default:
        return FALSE;
    }
}

// Symbol: ?OnToolHitTest@CWnd@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI) {
    if (!pThis || !pTI) {
        return -1;
    }
    std::memset(pTI, 0, sizeof(*pTI));
    pTI->cbSize = sizeof(*pTI);
    if (!pThis->m_hWnd) {
        return -1;
    }

    RECT clientRect{};
    if (!::GetClientRect(pThis->m_hWnd, &clientRect)) {
        return -1;
    }
    POINT ptScreen{point.x, point.y};
    POINT ptClient{point.x, point.y};
    if (!::ScreenToClient(pThis->m_hWnd, &ptClient) || !::PtInRect(&clientRect, ptClient)) {
        return -1;
    }

    HWND hHit = ::WindowFromPoint(ptScreen);
    if (!hHit || (hHit != pThis->m_hWnd && !::IsChild(pThis->m_hWnd, hHit))) {
        return -1;
    }

    pTI->hwnd = hHit;
    pTI->uId = reinterpret_cast<UINT_PTR>(hHit);
    pTI->lpszText = nullptr;
    RECT targetRect{};
    if (::GetWindowRect(hHit, &targetRect)) {
        POINT origin{targetRect.left, targetRect.top};
        ::ScreenToClient(pThis->m_hWnd, &origin);
        pTI->rect.left = origin.x;
        pTI->rect.top = origin.y;
        pTI->rect.right = origin.x + (targetRect.right - targetRect.left);
        pTI->rect.bottom = origin.y + (targetRect.bottom - targetRect.top);
    }
    return static_cast<LONGLONG>(reinterpret_cast<LONG_PTR>(hHit));
}

// Symbol: ?PreSubclassWindow@CWnd@@UEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CWnd__UEAAXXZ(CWnd* pThis) {
    CWnd_PreSubclassWindowCompat(pThis);
}

// Symbol: ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg) {
    return CWnd_PreTranslateMessageCompat(pThis, pMsg);
}

// Symbol: ?OnCommand@CWnd@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam) {
    if (!pThis) {
        return FALSE;
    }

    unsigned int nID = static_cast<unsigned int>(LOWORD(static_cast<UINT>(wParam)));
    int nCode = static_cast<int>(HIWORD(static_cast<UINT>(wParam)));
    void* pExtra = reinterpret_cast<void*>(lParam);
    if (pThis->OnCmdMsg(nID, nCode, pExtra, nullptr)) {
        return TRUE;
    }

    if (lParam) {
        CWnd* pSender = CWnd::FromHandle(reinterpret_cast<HWND>(lParam));
        if (pSender && pSender != pThis && pSender->OnCmdMsg(nID, nCode, pExtra, nullptr)) {
            return TRUE;
        }
    }
    return FALSE;
}

// Symbol: ?OnFinalRelease@CWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CWnd__UEAAXXZ(CWnd* pThis) {
    if (!pThis) {
        return;
    }

    if (pThis->m_hWnd && ::IsWindow(pThis->m_hWnd)) {
        // Release window resources as part of final COM/OLE lifetime transition.
        if (!::DestroyWindow(pThis->m_hWnd)) {
            pThis->PostNcDestroy();
        }
        return;
    }

    pThis->PostNcDestroy();
}

// Symbol: ?OnNotify@CWnd@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(
    CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) {
        *pResult = 0;
    }
    return pThis ? pThis->OnNotify(static_cast<uintptr_t>(wParam), lParam, reinterpret_cast<intptr_t*>(pResult)) : FALSE;
}

// Symbol: ?OnWndMsg@CWnd@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (!pThis) {
        return FALSE;
    }
    if (pResult) {
        *pResult = 0;
    }
    // Objects constructed through the MSVC import library do not have a
    // cross-compiler-compatible CWnd vtable yet.  Dispatch the base handlers
    // directly instead of making a virtual call through an invalid vptr.
    switch (message) {
    case WM_COMMAND:
        return impl__OnCommand_CWnd__MEAAH_K_J_Z(pThis, wParam, lParam);
    case WM_NOTIFY:
        return impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(pThis, wParam, lParam, pResult);
    default:
        return FALSE;
    }
}

LONGLONG CWnd::SendDlgItemMessageW(int p0, UINT p1, ULONGLONG p2, LONGLONG p3)
{
    if (!m_hWnd) {
        return 0;
    }
    return ::SendDlgItemMessageW(m_hWnd, p0, p1, p2, p3);
}

BOOL CWnd::SetDlgCtrlID(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    ::SetLastError(ERROR_SUCCESS);
    LONG_PTR previous = ::SetWindowLongPtrW(m_hWnd, GWL_ID, static_cast<LONG_PTR>(p0));
    return previous != 0 || ::GetLastError() == ERROR_SUCCESS;
}

void CWnd::SetDlgItemInt(int p0, UINT p1, int p2)
{
    if (!m_hWnd) {
        return;
    }
    ::SetDlgItemInt(m_hWnd, p0, p1, p2 != FALSE);
}

void CWnd::SetDlgItemTextW(int p0, const WCHAR* p1)
{
    if (!m_hWnd) {
        return;
    }
    ::SetDlgItemTextW(m_hWnd, p0, p1 ? p1 : L"");
}

void* CWnd::SetFocus()
{
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndOld = ::SetFocus(m_hWnd);
    return hWndOld ? CWnd::FromHandle(hWndOld) : nullptr;
}

int CWnd::SetGestureConfig(CGestureConfig* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    auto& state = GetWindowRuntimeState(this);
    state.gestureConfigEnabled = (p0 != nullptr);
    return TRUE;
}

BOOL CWnd::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* p0)
{
    GetWindowRuntimeState(this).properties[{0x4F43, 0}] = p0;
    return TRUE;
}

int CWnd::SetProperty(long p0, WORD p1, void* p2)
{
    auto& state = GetWindowRuntimeState(this);
    state.properties[{p0, p1}] = p2;
    return TRUE;
}

int CWnd::SetScrollInfo(int p0, SCROLLINFO* p1, int p2)
{
    if (!m_hWnd || !p1) {
        return FALSE;
    }
    p1->cbSize = sizeof(*p1);
    return ::SetScrollInfo(m_hWnd, p0, p1, p2 ? TRUE : FALSE);
}

int CWnd::SetScrollPos(int p0, int p1, int p2)
{
    if (!m_hWnd) {
        return FALSE;
    }
    return ::SetScrollPos(m_hWnd, p0, p1, p2 ? TRUE : FALSE);
}

void CWnd::SetScrollRange(int p0, int p1, int p2, int p3)
{
    if (!m_hWnd) {
        return;
    }
    ::SetScrollRange(m_hWnd, p0, p1, p2, p3 ? TRUE : FALSE);
}

int CWnd::SetWindowPlacement(const WINDOWPLACEMENT* p0)
{
    if (!m_hWnd || !p0) {
        return FALSE;
    }
    return ::SetWindowPlacement(m_hWnd, p0) ? TRUE : FALSE;
}

int CWnd::SubclassDlgItem(UINT p0, void* p1)
{
    if (!m_hWnd || !p0 || !p1) {
        return FALSE;
    }
    CWnd* pWnd = static_cast<CWnd*>(p1);
    HWND hChild = ::GetDlgItem(m_hWnd, p0);
    if (!hChild) {
        return FALSE;
    }
    return pWnd->SubclassWindow(hChild) ? TRUE : FALSE;
}

int CWnd::SubclassWindow(HWND p0)
{
    if (!p0) {
        return FALSE;
    }

    WNDPROC previous = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(p0, GWLP_WNDPROC));
    if (!previous) {
        return FALSE;
    }

    if (m_hWnd) {
        UnsubclassWindow();
    }

    CWnd* old = OpenMfcLookupCWnd(p0);
    if (old && old != this) {
        return FALSE;
    }

    g_hwndMap[p0] = this;
    m_pfnSuper = previous;
    m_hWnd = p0;
    ::SetWindowLongPtrW(p0, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(AfxWndProc));
    return TRUE;
}

HWND CWnd::UnsubclassWindow()
{
    if (!m_hWnd) {
        return nullptr;
    }

    HWND hWnd = m_hWnd;
    if (m_pfnSuper) {
        ::SetWindowLongPtrW(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_pfnSuper));
        m_pfnSuper = nullptr;
    }
    g_hwndMap.erase(m_hWnd);
    m_hWnd = nullptr;
    return hWnd;
}

int CWnd::UpdateData(int p0)
{
    if (!m_hWnd) {
        return FALSE;
    }

    UpdateDialogControls(this, p0);
    return TRUE;
}

void CWnd::UpdateDialogControls(void* p0, int p1)
{
    (void)p0;
    if (!m_hWnd) {
        return;
    }

    for (HWND hChild = ::GetWindow(m_hWnd, GW_CHILD); hChild; hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
        if (!::IsWindow(hChild)) {
            continue;
        }

        // Touch current caption text for each child window to mimic lightweight dialog
        // validation/update without depending on framework DDX metadata.
        int len = ::GetWindowTextLengthW(hChild);
        if (len > 0) {
            std::vector<wchar_t> buf(static_cast<size_t>(len) + 1);
            ::GetWindowTextW(hChild, buf.data(), len + 1);
            if (p1) {
                ::SetWindowTextW(hChild, buf.data());
            }
        }
    }
}

void CWnd::EnableD2DSupport(int p0, int p1)
{
    (void)p1;
    GetWindowRuntimeState(this).d2dSupportEnabled = p0 != FALSE;
}

void CWnd::EnableDynamicLayout(int p0)
{
    GetWindowRuntimeState(this).dynamicLayoutEnabled = p0 != FALSE;
}

CDCRenderTarget* CWnd::GetDCRenderTarget()
{
    auto it = g_wndRuntimeStates.find(this);
    if (it == g_wndRuntimeStates.end() || !it->second.d2dSupportEnabled) {
        return nullptr;
    }
    auto& state = GetWindowRuntimeState(this);
    auto dcTargetIt = state.properties.find({0x4F44, 0});
    if (dcTargetIt == state.properties.end() || !dcTargetIt->second) {
        void*& slot = state.properties[{0x4F44, 0}];
        slot = new CDCRenderTarget();
        return static_cast<CDCRenderTarget*>(slot);
    }
    return static_cast<CDCRenderTarget*>(dcTargetIt->second);
}

void CWnd::InitDynamicLayout()
{
    GetWindowRuntimeState(this).dynamicLayoutEnabled = true;
}

int CWnd::LoadDynamicLayoutResource(const WCHAR* p0)
{
    if (!m_hWnd) {
        return FALSE;
    }
    GetWindowRuntimeState(this).dynamicLayoutResource = p0 ? p0 : L"";
    GetWindowRuntimeState(this).dynamicLayoutEnabled = true;
    return TRUE;
}

void CWnd::ResizeDynamicLayout()
{
    if (!m_hWnd || !g_wndRuntimeStates[this].dynamicLayoutEnabled) {
        return;
    }
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
}

int CWnd::IsD2DSupportEnabled()
{
    return g_wndRuntimeStates[this].d2dSupportEnabled ? TRUE : FALSE;
}

void CWnd::OnDrawIconicThumbnailOrLivePreview(CDC*& p0, CRect p1, CSize p2, int p3, int*& p4)
{
    if (!m_hWnd) {
        return;
    }

    if (!p0 || !p0->m_hDC) {
        return;
    }

    RECT bounds = {p1.left, p1.top, p1.right, p1.bottom};
    if (bounds.right <= bounds.left || bounds.bottom <= bounds.top) {
        bounds.left = 0;
        bounds.top = 0;
        bounds.right = p2.cx > 0 ? p2.cx : 256;
        bounds.bottom = p2.cy > 0 ? p2.cy : 256;
    }

    HDC hdc = p0->m_hDC;
    UINT printFlags = PRF_CLIENT | PRF_ERASEBKGND | PRF_CHILDREN;
    if (p3) {
        printFlags |= PRF_NONCLIENT;
    }

    ::SendMessageW(m_hWnd, WM_PRINT, reinterpret_cast<WPARAM>(hdc), printFlags);

    if (p4) {
        *p4 = bounds.right - bounds.left;
    }
}

//=============================================================================
// CFrameWnd implementations
//=============================================================================
const CRect CFrameWnd::rectDefault = {};

void CFrameWnd::AddFrameWnd() {
    auto& state = GetFrameRuntimeState(this);
    state.menuBarState = 0;
    state.menuBarVisibility = 1;
    state.progressMin = 0;
    state.progressMax = 100;
    state.progressPos = 0;
}
void CFrameWnd::BeginModalState() {
    auto& state = GetFrameRuntimeState(this);
    HWND hOwner = static_cast<HWND>(GetTopLevelOwner());
    if (!hOwner || hOwner == m_hWnd) {
        ++state.modalDepth;
        return;
    }

    if (state.modalDepth == 0) {
        state.modalOwner = hOwner;
        ::EnableWindow(hOwner, FALSE);
    }
    ++state.modalDepth;
}
void CFrameWnd::BringToTop(int nCmdShow) {
    if (!m_hWnd) {
        return;
    }

    int showCmd = nCmdShow > 0 ? nCmdShow : SW_SHOW;
    ::ShowWindow(m_hWnd, showCmd);
    ::BringWindowToTop(m_hWnd);
}
DWORD CFrameWnd::CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar) {
    (void)rect;
    if (ppDockBar) {
        *ppDockBar = nullptr;
    }
    return (m_hWnd && dwDockStyle != 0) ? 1 : 0;
}
int CFrameWnd::CanEnterHelpMode() {
    return m_hWnd ? TRUE : FALSE;
}
CMiniDockFrameWnd* CFrameWnd::CreateFloatingFrame(DWORD dwStyle) {
    if (!m_hWnd) {
        return nullptr;
    }
    if (dwStyle == 0) {
        ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
        return nullptr;
    }

    RECT hostRect{};
    if (!::GetWindowRect(m_hWnd, &hostRect)) {
        hostRect.left = 0;
        hostRect.top = 0;
        hostRect.right = 320;
        hostRect.bottom = 240;
    }

    int width = hostRect.right - hostRect.left;
    int height = hostRect.bottom - hostRect.top;
    if (width <= 0) {
        width = 320;
    }
    if (height <= 0) {
        height = 240;
    }
    RECT frameRect{
        hostRect.left,
        hostRect.top,
        hostRect.left + width,
        hostRect.top + height,
    };

    DWORD effectiveStyle = dwStyle | WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    CString title;
    if (m_hWnd) {
        int textLen = ::GetWindowTextLengthW(m_hWnd);
        if (textLen > 0) {
            int cap = std::max(textLen + 1, 256);
            wchar_t* text = title.GetBuffer(cap);
            int actual = ::GetWindowTextW(m_hWnd, text, cap);
            title.ReleaseBuffer(actual > 0 ? actual : 0);
        }
    }
    if (title.IsEmpty()) {
        title = L"OpenMFC Floating";
    }

    CMiniFrameWnd* pFrame = new CMiniFrameWnd();
    if (!pFrame) {
        return nullptr;
    }

    if (!pFrame->Create(
            g_szOpenMFCClass,
            title.GetString(),
            effectiveStyle,
            frameRect,
            this,
            nullptr,
            WS_EX_TOOLWINDOW,
            nullptr)) {
        delete pFrame;
        return nullptr;
    }

    pFrame->ShowWindow(SW_SHOWNORMAL);
    pFrame->SetWindowTextW(title.GetString());

    return reinterpret_cast<CMiniDockFrameWnd*>(pFrame);
}

CWnd* CFrameWnd::CreateView(CCreateContext* pContext, unsigned int nID) {
    (void)pContext;
    if (!m_hWnd || !nID) {
        return nullptr;
    }
    HWND hWndView = ::GetDlgItem(m_hWnd, static_cast<int>(nID));
    return hWndView ? CWnd::FromHandle(hWndView) : nullptr;
}
void CFrameWnd::DelayUpdateFrameMenu(HMENU hMenuAlt) {
    OnUpdateFrameMenu(hMenuAlt);
}
void CFrameWnd::DestroyDockBars() {
    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar && pBar->m_hWnd) {
            ::ShowWindow(pBar->m_hWnd, SW_HIDE);
        }
    }
    state.controlBars.clear();
}
void CFrameWnd::DockControlBar(CControlBar* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    (void)nDockBarID;
    if (!pBar) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    bool already = false;
    for (CControlBar* bar : state.controlBars) {
        if (bar == pBar) {
            already = true;
            break;
        }
    }
    if (!already) {
        state.controlBars.push_back(pBar);
    }

    if (pBar->m_hWnd && lpRect) {
        ::MoveWindow(pBar->m_hWnd,
                     lpRect->left, lpRect->top,
                     lpRect->right - lpRect->left,
                     lpRect->bottom - lpRect->top,
                     TRUE);
    }
    if (pBar->m_hWnd) {
        ::ShowWindow(pBar->m_hWnd, SW_SHOW);
    }
}
void CFrameWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, const RECT* lpRect) {
    (void)pDockBar;
    // Delegate to the nDockBarID overload; a CDockBar* argument would resolve
    // back to this same overload and recurse infinitely.
    DockControlBar(pBar, 0u, lpRect);
}
void CFrameWnd::EnableDocking(DWORD dwDockStyle) {
    GetFrameRuntimeState(this).dockingStyle = dwDockStyle;
}
void CFrameWnd::EndModalState() {
    auto& state = GetFrameRuntimeState(this);
    if (state.modalDepth == 0) {
        return;
    }
    --state.modalDepth;
    if (state.modalDepth == 0 && state.modalOwner) {
        if (::IsWindow(state.modalOwner)) {
            ::EnableWindow(state.modalOwner, TRUE);
        }
        state.modalOwner = nullptr;
    }
}
void CFrameWnd::ExitHelpMode() {
    // End SHIFT+F1 context-help mode: drop any mouse capture taken for the help
    // cursor and restore the normal arrow pointer.
    if (m_hWnd && ::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
    }
    ::SetCursor(::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW)));
}
void CFrameWnd::FloatControlBar(CControlBar* pBar, CPoint pt, DWORD dwStyle) {
    (void)dwStyle;
    if (!pBar || !pBar->m_hWnd) {
        return;
    }
    RECT windowRect{};
    int width = 200;
    int height = 150;
    if (::GetWindowRect(pBar->m_hWnd, &windowRect)) {
        width = windowRect.right - windowRect.left;
        height = windowRect.bottom - windowRect.top;
    }
    ::SetWindowPos(pBar->m_hWnd, nullptr,
                   pt.x, pt.y, width, height,
                   SWP_NOZORDER | SWP_NOACTIVATE);
}
CDocument* CFrameWnd::GetActiveDocument() {
    CView* pView = GetActiveView();
    return pView ? pView->GetDocument() : nullptr;
}
CFrameWnd* CFrameWnd::GetActiveFrame() { return this; }
CView* CFrameWnd::GetActiveView() const {
    if (m_pViewActive) {
        return dynamic_cast<CView*>(m_pViewActive);
    }

    if (!m_hWnd) {
        return nullptr;
    }

    HWND hWndView = ::GetDlgItem(m_hWnd, AFX_IDW_PANE_FIRST);
    if (!hWndView) {
        return nullptr;
    }
    return dynamic_cast<CView*>(CWnd::FromHandle(hWndView));
}
void CFrameWnd::GetDockState(CDockState& state) const {
    if (!m_hWnd) {
        return;
    }
    auto& frameState = GetFrameRuntimeState(const_cast<CFrameWnd*>(this));
    std::vector<DockBarSnapshot> snapshot;
    snapshot.reserve(frameState.controlBars.size());
    for (CControlBar* bar : frameState.controlBars) {
        if (!bar) {
            continue;
        }
        DockBarSnapshot item;
        item.bar = bar;
        item.dockStyle = bar->m_dwDockStyle;
        if (bar->m_hWnd) {
            item.id = static_cast<UINT>(::GetWindowLongPtrW(bar->m_hWnd, GWLP_ID));
            item.visible = ::IsWindowVisible(bar->m_hWnd) != FALSE;
            ::GetWindowRect(bar->m_hWnd, &item.windowRect);
        }
        snapshot.push_back(item);
    }

    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_dockStateSnapshots[&state] = std::move(snapshot);
}

const wchar_t* CFrameWnd::GetIconWndClass(DWORD dwDefaultStyle, unsigned int nIDResource) {
    (void)dwDefaultStyle;
    (void)nIDResource;
    return g_szOpenMFCClass;
}

CControlBar* CFrameWnd::GetControlBar(unsigned int nID) {
    if (!m_hWnd || !nID) {
        return nullptr;
    }
    HWND hWndBar = ::GetDlgItem(m_hWnd, static_cast<int>(nID));
    if (!hWndBar) {
        return nullptr;
    }
    CWnd* pWnd = CWnd::FromHandle(hWndBar);
    if (!pWnd) {
        return nullptr;
    }
    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar && pBar->m_hWnd == hWndBar) {
            return pBar;
        }
    }
    return dynamic_cast<CControlBar*>(pWnd);
}
HACCEL CFrameWnd::GetDefaultAccelerator() { return m_hAccelTable; }
CMenu* CFrameWnd::GetMenu() const {
    if (!m_hWnd) {
        return nullptr;
    }
    return CMenu::FromHandle(::GetMenu(m_hWnd));
}

int CFrameWnd::GetMenuBarInfo(long id, long lParam, MENUBARINFO* pmbi) const {
    if (!m_hWnd || !pmbi) {
        return FALSE;
    }
    pmbi->cbSize = sizeof(*pmbi);
    return ::GetMenuBarInfo(m_hWnd, id, lParam, pmbi) ? TRUE : FALSE;
}

DWORD CFrameWnd::GetMenuBarState() const { return GetFrameRuntimeState(const_cast<CFrameWnd*>(this)).menuBarState; }
DWORD CFrameWnd::GetMenuBarVisibility() const { return GetFrameRuntimeState(const_cast<CFrameWnd*>(this)).menuBarVisibility; }

CWnd* CFrameWnd::GetMessageBar() {
    if (!m_hWnd) {
        return nullptr;
    }
    HWND hWndStatus = ::GetDlgItem(m_hWnd, AFX_IDW_STATUS_BAR);
    return hWndStatus ? CWnd::FromHandle(hWndStatus) : nullptr;
}

void CFrameWnd::GetMessageString(unsigned int nID, CString& rMessage) const {
    rMessage.Empty();
    if (!nID) {
        return;
    }
    wchar_t buffer[1024];
    int len = ::LoadStringW(AfxGetResourceHandle(), nID, buffer, 1024);
    if (len > 0) {
        rMessage = buffer;
    }
}

// Symbol: ?GetMessageString@CFrameWnd@@UEBAXIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CFrameWnd* pThis, unsigned int nID, CString* rMessage) {
    if (!rMessage) {
        return;
    }
    rMessage->Empty();
    if (pThis) {
        pThis->GetMessageString(nID, *rMessage);
    }
}
void CFrameWnd::InitialUpdateFrame(CDocument* pDoc, int bMakeVisible) {
    CView* pFirstView = nullptr;
    if (pDoc) {
        void* pos = pDoc->GetFirstViewPosition();
        while (pos) {
            CView* pView = pDoc->GetNextView(pos);
            if (!pView) {
                continue;
            }
            if (!pFirstView) {
                pFirstView = pView;
            }
            pView->OnInitialUpdate();
        }

        const wchar_t* docTitle = pDoc->GetTitle();
        if (docTitle && *docTitle) {
            UpdateFrameTitleForDocument(docTitle);
        }
    }

    if (pFirstView && !m_pViewActive) {
        SetActiveView(pFirstView, TRUE);
    }
    if (m_hWnd) {
        RecalcLayout(TRUE);
        if (bMakeVisible) {
            ActivateFrame(SW_SHOW);
        }
    }
}
int CFrameWnd::IsFrameWnd() const { return 1; }
int CFrameWnd::IsTracking() {
    if (!m_hWnd) {
        return FALSE;
    }
    return (::GetCapture() == m_hWnd) ? TRUE : FALSE;
}
BOOL CFrameWnd::LoadAccelTable(const wchar_t* lpszAccelTable) {
    m_hAccelTable = nullptr;
    if (!lpszAccelTable) {
        return FALSE;
    }
    m_hAccelTable = ::LoadAcceleratorsW(AfxGetResourceHandle(), lpszAccelTable);
    return m_hAccelTable != nullptr;
}
void CFrameWnd::LoadBarState(const wchar_t* lpszProfileName) {
    if (!lpszProfileName) {
        return;
    }
    auto& state = GetFrameRuntimeState(this);
    std::vector<int> vis;
    {
        std::lock_guard<std::mutex> lk(g_wndStateMutex);
        auto it = g_barStateStore.find(lpszProfileName);
        if (it != g_barStateStore.end()) {
            vis = it->second;
        }
    }
    for (size_t i = 0; i < state.controlBars.size() && i < vis.size(); ++i) {
        if (state.controlBars[i]) {
            ShowControlBar(state.controlBars[i], vis[i], FALSE);
        }
    }
    if (state.menuBarVisibility == 0) {
        OnShowMenuBar();
    }
}
int CFrameWnd::NegotiateBorderSpace(unsigned int nBorderCmd, RECT* lpRectBorder) {
    if (!m_hWnd || !lpRectBorder || nBorderCmd == 0) {
        return 0;
    }
    RECT clientRect;
    if (!::GetClientRect(m_hWnd, &clientRect)) {
        return 0;
    }
    *lpRectBorder = clientRect;
    return 1;
}
void CFrameWnd::NotifyFloatingWindows(DWORD dwFlags) {
    ShowOwnedWindows(dwFlags ? TRUE : FALSE);
}
void CFrameWnd::OnActivate(unsigned int nState, CWnd* pWndOther, int bMinimized) {
    (void)bMinimized;
    if (!m_pViewActive) {
        return;
    }
    CView* pActiveView = dynamic_cast<CView*>(m_pViewActive);
    if (!pActiveView) {
        return;
    }
    int bActivate = (nState == WA_ACTIVE || nState == WA_CLICKACTIVE) ? TRUE : FALSE;
    pActiveView->OnActivateView(bActivate, pActiveView, pWndOther ? dynamic_cast<CView*>(pWndOther) : nullptr);
}
__int64 CFrameWnd::OnActivateTopLevel(unsigned __int64 wParam, __int64 lParam) {
    OnActivate(static_cast<unsigned int>(wParam), lParam ? CWnd::FromHandle(reinterpret_cast<HWND>(lParam)) : nullptr, 0);
    return 0;
}
int CFrameWnd::OnBarCheck(unsigned int nID) {
    CControlBar* pBar = GetControlBar(nID);
    return (pBar && pBar->m_hWnd && ::IsWindowVisible(pBar->m_hWnd)) ? TRUE : FALSE;
}
BOOL CFrameWnd::OnChevronPushed(unsigned int nIndex, NMHDR* pNMHDR, __int64* lResult) {
    (void)nIndex;
    (void)pNMHDR;
    // Base frame has no rebar chevron popup to expand; report "not handled" so
    // the notification falls through to default processing.
    if (lResult) {
        *lResult = 0;
    }
    return FALSE;
}
void CFrameWnd::OnClose() {
    if (!m_hWnd) {
        return;
    }
    DestroyWindow();
}
int CFrameWnd::OnCmdMsg(unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    CWnd* pView = m_pViewActive ? m_pViewActive : GetActiveView();
    if (pView && pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
            return TRUE;
    }

    if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }

    return FALSE;
}

int CFrameWnd::OnCommand(unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    unsigned int nID = static_cast<unsigned int>(LOWORD(static_cast<WPARAM>(wParam)));
    int nCode = static_cast<int>(HIWORD(static_cast<WPARAM>(wParam)));
    if (OnCmdMsg(nID, nCode, nullptr, nullptr)) {
        return TRUE;
    }
    return CWnd::OnCommand(static_cast<uintptr_t>(wParam), static_cast<intptr_t>(lParam));
}
__int64 CFrameWnd::OnCommandHelp(unsigned __int64 wParam, __int64 lParam) {
    if (!m_hWnd) {
        return 0;
    }

    if (lParam) {
        HELPINFO* pHelp = reinterpret_cast<HELPINFO*>(static_cast<LPARAM>(lParam));
        if (pHelp && pHelp->dwContextId) {
            CString text;
            GetMessageString(static_cast<unsigned int>(pHelp->dwContextId), text);
            SetMessageText(static_cast<const wchar_t*>(text));
            return 1;
        }
    }

    if (wParam) {
        SetMessageText(static_cast<unsigned int>(wParam));
        return 1;
    }

    OnHelp();
    return 1;
}

void CFrameWnd::OnContextHelp() {
    OnHelp();
}

int CFrameWnd::OnCreateClient(CREATESTRUCTW* lpcs, CCreateContext* pContext) {
    (void)lpcs;
    if (!m_hWnd) {
        return FALSE;
    }

    CWnd* pClient = CreateView(pContext, AFX_IDW_PANE_FIRST);
    if (pClient && pClient->m_hWnd) {
        ::ShowWindow(pClient->m_hWnd, SW_SHOW);
    }

    return TRUE;
}

int CFrameWnd::OnCreateHelper(CREATESTRUCTW* lpcs, CCreateContext* pContext) {
    AddFrameWnd();
    ::DragAcceptFiles(m_hWnd, TRUE);
    return OnCreateClient(lpcs, pContext);
}
void CFrameWnd::OnDDEExecute(CWnd* pWnd, void* pData) {
    // Real MFC unpacks the WM_DDE_EXECUTE command block and routes it through
    // CWinApp::OnDDECommand. This clean-room CWinApp exposes no DDE command
    // dispatch yet, so acknowledge negatively rather than fabricate a status
    // string: post WM_DDE_ACK with a zero (refused) status back to the partner.
    (void)pData;
    if (pWnd && pWnd->m_hWnd && m_hWnd) {
        ::PostMessageW(pWnd->m_hWnd, WM_DDE_ACK,
                       reinterpret_cast<WPARAM>(m_hWnd), 0);
    }
}
void CFrameWnd::OnDDEInitiate(CWnd* pWnd, unsigned int nAtomApp, unsigned int nAtomTopic) {
    wchar_t appName[128] = {};
    wchar_t topicName[128] = {};
    if (nAtomApp) {
        if (::GlobalGetAtomNameW(nAtomApp, appName, static_cast<int>(std::size(appName))) <= 0) {
            appName[0] = L'\0';
        }
    }
    if (nAtomTopic) {
        if (::GlobalGetAtomNameW(nAtomTopic, topicName, static_cast<int>(std::size(topicName))) <= 0) {
            topicName[0] = L'\0';
        }
    }

    std::wstring message = L"DDE initiate";
    if (appName[0] || topicName[0]) {
        message += L": ";
        if (appName[0]) {
            message += appName;
        }
        if (topicName[0]) {
            if (appName[0]) {
                message += L"/";
            }
            message += topicName;
        }
    }
    SetMessageText(message.c_str());

    if (pWnd && pWnd->m_hWnd) {
        ::PostMessageW(pWnd->m_hWnd, WM_DDE_ACK,
                       reinterpret_cast<WPARAM>(m_hWnd), 0);
    }
}
void CFrameWnd::OnDDETerminate(CWnd* pWnd) {
    // Complete the DDE conversation teardown by echoing WM_DDE_TERMINATE back to
    // the partner, as the protocol requires.
    if (pWnd && pWnd->m_hWnd && m_hWnd) {
        ::PostMessageW(pWnd->m_hWnd, WM_DDE_TERMINATE,
                       reinterpret_cast<WPARAM>(m_hWnd), 0);
    }
}
void CFrameWnd::OnDestroy() {
    auto* pApp = AfxGetApp();
    if (pApp && pApp->m_pMainWnd == this) {
        pApp->m_pMainWnd = nullptr;
    }
    RemoveFrameWnd();
    ShowOwnedWindows(FALSE);
}

void CFrameWnd::OnDropFiles(HDROP hDropInfo) {
    if (!hDropInfo) {
        return;
    }

    // Real MFC activates the frame then hands each dropped path to
    // CWinApp::OpenDocumentFile. This clean-room CWinApp has no document manager,
    // so we perform the frame-level part faithfully (activate + notify each file
    // via WM_COPYDATA to the active view for app-level handling) and always
    // DragFinish to release the drop buffer.
    ::SetActiveWindow(m_hWnd);
    UINT count = ::DragQueryFileW(hDropInfo, 0xFFFFFFFF, nullptr, 0);
    CWnd* pView = GetActiveView();
    for (UINT i = 0; i < count; ++i) {
        wchar_t filePath[MAX_PATH] = {};
        UINT len = ::DragQueryFileW(hDropInfo, i, filePath, MAX_PATH);
        if (len == 0 || !pView || !pView->m_hWnd) {
            continue;
        }
        COPYDATASTRUCT cds = {};
        cds.dwData = static_cast<ULONG_PTR>(WM_DROPFILES);
        cds.cbData = static_cast<DWORD>((len + 1) * sizeof(wchar_t));
        cds.lpData = filePath;
        ::SendMessageW(pView->m_hWnd, WM_COPYDATA,
                       reinterpret_cast<WPARAM>(m_hWnd),
                       reinterpret_cast<LPARAM>(&cds));
    }
    ::DragFinish(hDropInfo);
}

void CFrameWnd::OnEnable(int bEnable) {
    if (!m_hWnd) {
        return;
    }
    SetMessageText(bEnable ? L"Frame enabled" : L"Frame disabled");
    if (::IsWindowEnabled(m_hWnd) != (bEnable != 0)) {
        ::EnableWindow(m_hWnd, bEnable ? TRUE : FALSE);
    }
}
void CFrameWnd::OnEndSession(int bEnding) {
    if (!m_hWnd) {
        return;
    }
    SetMessageText(bEnding ? L"Session ending" : L"Session continues");
}
void CFrameWnd::OnEnterIdle(unsigned int nWhy, CWnd* pWho) {
    if (!m_hWnd) {
        return;
    }
    if (nWhy == MSGF_MENU || nWhy == MSGF_DIALOGBOX || nWhy == MSGF_MESSAGEBOX) {
        CMenu* pMenu = GetMenu();
        if (auto* pFrame = pWho ? dynamic_cast<CFrameWnd*>(pWho) : nullptr) {
            CMenu* pCandidate = pFrame->GetMenu();
            if (pCandidate) {
                pMenu = pCandidate;
            }
        }
        HMENU hMenu = pMenu ? pMenu->m_hMenu : nullptr;
        OnUpdateFrameMenu(hMenu);
        SetMessageText(pWho ? L"Idle in command mode" : L"Idle");
    } else if (pWho && pWho->m_hWnd == m_hWnd) {
        SetMessageText(L"Idle");
    }
}
int CFrameWnd::OnEraseBkgnd(CDC* pDC) {
    if (!pDC || !pDC->m_hDC) {
        return FALSE;
    }

    RECT clientRect = {};
    if (m_hWnd) {
        ::GetClientRect(m_hWnd, &clientRect);
        HBRUSH bgBrush = ::GetSysColorBrush(COLOR_3DFACE);
        ::FillRect(pDC->m_hDC, &clientRect, bgBrush);
        return TRUE;
    }

    return FALSE;
}

void CFrameWnd::OnHelp() {
    SetMessageText(L"Help requested");
    if (m_nIDHelp) {
        CString message;
        message.Format(L"Help topic id: %u", m_nIDHelp);
        SetMessageText(message.GetString());
    }
}
__int64 CFrameWnd::OnHelpHitTest(unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    auto* pHelp = reinterpret_cast<HELPINFO*>(static_cast<LPARAM>(lParam));
    if (!pHelp) {
        return 0;
    }
    return static_cast<__int64>(pHelp->dwContextId);
}
__int64 CFrameWnd::OnHelpPromptAddr(unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    auto* pHelp = reinterpret_cast<HELPINFO*>(static_cast<LPARAM>(lParam));
    if (pHelp && pHelp->dwContextId) {
        CString message;
        GetMessageString(static_cast<unsigned int>(pHelp->dwContextId), message);
        SetMessageText(message.GetString());
        return 1;
    }
    return 0;
}
void CFrameWnd::OnHideMenuBar() {
    if (!m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    if (state.menuBarVisibility == 0) {
        return;
    }

    state.hiddenMenu = ::GetMenu(m_hWnd);
    ::SetMenu(m_hWnd, nullptr);
    state.menuBarVisibility = 0;
    ::DrawMenuBar(m_hWnd);
}
void CFrameWnd::OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (pScrollBar && pScrollBar->m_hWnd) {
        ::SendMessageW(pScrollBar->m_hWnd, WM_HSCROLL, MAKELONG(nPos, nSBCode), 0);
    }
    if (m_pViewActive) {
        m_pViewActive->OnHScroll(nSBCode, nPos, pScrollBar);
    }
}
void CFrameWnd::OnIdleUpdateCmdUI() {
    OnUpdateFrameMenu(nullptr);
}
void CFrameWnd::OnInitMenu(CMenu* pMenu) {
    HMENU hMenu = pMenu ? pMenu->m_hMenu : GetMenu() ? GetMenu()->m_hMenu : nullptr;
    if (hMenu) {
        OnUpdateFrameMenu(hMenu);
    }
}
void CFrameWnd::OnInitMenuPopup(CMenu* pPopupMenu, unsigned int nIndex, int bSysMenu) {
    (void)nIndex;
    (void)bSysMenu;
    if (!pPopupMenu || !pPopupMenu->m_hMenu) {
        return;
    }
    OnUpdateFrameMenu(pPopupMenu->m_hMenu);
}
__int64 CFrameWnd::OnMenuChar(unsigned int nChar, unsigned int nFlags, CMenu* pMenu) {
    // Base-frame behavior: mnemonic matching against owner-drawn menu items is a
    // feature-pack (CMFCToolBar/CMFCPopupMenu) concern. With standard menus the
    // system already resolves the accelerator, so return MNC_IGNORE (high word)
    // to let default processing proceed.
    (void)nChar;
    (void)nFlags;
    (void)pMenu;
    return static_cast<__int64>(MAKELONG(0, MNC_IGNORE));
}
void CFrameWnd::OnMenuSelect(unsigned int nItemID, unsigned int nFlags, HMENU hSysMenu) {
    (void)nFlags;
    (void)hSysMenu;
    if (!m_hWnd) {
        return;
    }
    if (nItemID == 0 || nItemID == 0xFFFF) {
        SetMessageText(L"");
        return;
    }
    CString text;
    GetMessageString(nItemID, text);
    SetMessageText(text.GetString());
}
int CFrameWnd::OnNcActivate(int bActive) {
    return m_hWnd ? static_cast<int>(::DefWindowProcW(m_hWnd, WM_NCACTIVATE, static_cast<WPARAM>(bActive), 0)) : FALSE;
}

void CFrameWnd::OnPaletteChanged(CWnd* pFocusWnd) {
    if (!m_hWnd || !pFocusWnd) {
        return;
    }

    CFrameWnd* pFocusFrame = pFocusWnd->GetTopLevelFrame();
    if (pFocusFrame == this && OnQueryNewPalette()) {
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
    }
}
__int64 CFrameWnd::OnPopMessageString(unsigned __int64 wParam, __int64 lParam) {
    // Restore the previously pushed message-line string. Real MFC delegates the
    // restore to OnSetMessageString with the saved id/text, which is exactly the
    // parameters forwarded here.
    return OnSetMessageString(wParam, lParam);
}
int CFrameWnd::OnQueryEndSession() {
    // Allow the session to end. Documents veto shutdown from their own
    // SaveModified path, not from the frame; the frame answers TRUE.
    return TRUE;
}
int CFrameWnd::OnQueryNewPalette() {
    return m_hWnd ? TRUE : FALSE;
}

int CFrameWnd::OnSetCursor(CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)pWnd;
    (void)message;

    if (!m_hWnd) {
        return FALSE;
    }

    if (nHitTest == HTCLIENT) {
        HCURSOR hCursor = ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
        ::SetCursor(hCursor);
        return TRUE;
    }
    return FALSE;
}

void CFrameWnd::OnSetFocus(CWnd* pOldWnd) {
    (void)pOldWnd;
    if (!m_hWnd) {
        return;
    }

    if (m_pViewActive && m_pViewActive->m_hWnd) {
        ::SetFocus(m_pViewActive->m_hWnd);
    }
}
__int64 CFrameWnd::OnSetMessageString(unsigned __int64 wParam, __int64 lParam) {
    if (wParam) {
        SetMessageText(static_cast<unsigned int>(wParam));
        return 0;
    }
    if (lParam) {
        SetMessageText(reinterpret_cast<const wchar_t*>(lParam));
        return 0;
    }
    SetMessageText(L"");
    return 0;
}
void CFrameWnd::OnSetPreviewMode(int bPreview, CPrintPreviewState* pState) {
    // Entering print preview hides the frame's control bars so the preview view
    // owns the client area; leaving it restores them. This is the real,
    // observable frame behavior for WM print-preview mode.
    (void)pState;
    if (!m_hWnd) {
        return;
    }
    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar) {
            ShowControlBar(pBar, bPreview ? FALSE : TRUE, FALSE);
        }
    }
}

void CFrameWnd::OnShowMenuBar() {
    if (!m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    if (state.menuBarVisibility != 0) {
        return;
    }

    HMENU hMenuToShow = state.hiddenMenu;
    if (!hMenuToShow && state.menuResourceId) {
        HINSTANCE hInst = AfxGetInstanceHandle();
        hMenuToShow = ::LoadMenuW(hInst ? hInst : ::GetModuleHandleW(nullptr),
                                  MAKEINTRESOURCEW(state.menuResourceId));
        // Track ownership so a previously loaded menu is not leaked when a fresh
        // one is loaded here.
        if (state.ownedMenu && state.ownedMenu != hMenuToShow) {
            ::DestroyMenu(state.ownedMenu);
        }
        state.ownedMenu = hMenuToShow;
    }

    if (hMenuToShow) {
        ::SetMenu(m_hWnd, hMenuToShow);
        ::DrawMenuBar(m_hWnd);
    }

    state.menuBarVisibility = 1;
}

void CFrameWnd::OnSize(unsigned int nType, int cx, int cy) {
    (void)nType;
    if (!m_hWnd) {
        return;
    }

    if (cx <= 0 || cy <= 0) {
        return;
    }

    if (m_pViewActive && m_pViewActive->m_hWnd) {
        ::MoveWindow(m_pViewActive->m_hWnd, 0, 0, cx, cy, TRUE);
    }

    auto& state = GetFrameRuntimeState(this);
    for (CControlBar* pBar : state.controlBars) {
        if (pBar && pBar->m_hWnd) {
            ::MoveWindow(pBar->m_hWnd, 0, 0, cx, cy, FALSE);
        }
    }
}

void CFrameWnd::OnSysCommand(unsigned int nID, __int64 lParam) {
    (void)lParam;
    if (!m_hWnd) {
        return;
    }

    const unsigned int command = nID & 0xFFF0;
    if (command == SC_CLOSE) {
        OnClose();
        return;
    }

    ::DefWindowProcW(m_hWnd, WM_SYSCOMMAND, nID, lParam);
}

BOOL CFrameWnd::OnToolTipText(unsigned int nID, NMHDR* pNMHDR, __int64* lResult) {
    (void)nID;
    if (!pNMHDR || !lResult) {
        return FALSE;
    }

    TOOLTIPTEXTW* pTTT = reinterpret_cast<TOOLTIPTEXTW*>(pNMHDR);
    if (pNMHDR->code != TTN_NEEDTEXTW && pNMHDR->code != TTN_NEEDTEXTA) {
        *lResult = 0;
        return FALSE;
    }

    BOOL handled = FALSE;
    if (pTTT->lpszText && pTTT->lpszText != LPSTR_TEXTCALLBACKW) {
        UINT toolId = 0;
        if (pTTT->uFlags & TTF_IDISHWND) {
            CWnd* pWnd = CWnd::FromHandle(reinterpret_cast<HWND>(static_cast<INT_PTR>(pNMHDR->idFrom))); 
            if (pWnd && pWnd->GetDlgCtrlID()) {
                toolId = static_cast<UINT>(pWnd->GetDlgCtrlID());
            }
        } else {
            toolId = static_cast<UINT>(pNMHDR->idFrom);
        }

        CString text;
        if (toolId) {
            GetMessageString(toolId, text);
            if (!text.IsEmpty()) {
                lstrcpynW(pTTT->lpszText, text.GetString(), 80);
                handled = TRUE;
            }
        }
    }

    *lResult = 0;
    return handled;
}

void CFrameWnd::OnUpdateContextHelp(CCmdUI* pCmdUI) {
    if (!pCmdUI) {
        return;
    }

    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(1);
}

void CFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI) {
    if (!pCmdUI) {
        return;
    }

    if (pCmdUI->m_nID) {
        CControlBar* pBar = GetControlBar(pCmdUI->m_nID);
        pCmdUI->Enable(pBar != nullptr);
        pCmdUI->SetCheck(pBar ? 1 : 0);
    } else {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(0);
    }
}

void CFrameWnd::OnUpdateFrameMenu(HMENU hMenuAlt) {
    // Install the frame's menu (MDI passes the active child's menu as hMenuAlt;
    // SDI passes null to keep the current one). Set and redraw the menu bar so
    // the change is visible.
    if (!m_hWnd) {
        return;
    }
    HMENU hMenu = hMenuAlt ? hMenuAlt : ::GetMenu(m_hWnd);
    if (hMenu) {
        ::SetMenu(m_hWnd, hMenu);
        ::DrawMenuBar(m_hWnd);
    }
}

void CFrameWnd::OnUpdateFrameTitle(int bAddToTitle) {
    // Compose the caption bar text: "<document> - <app>" when the frame adds the
    // active document's title, otherwise just the app name. This updates the
    // window title (SetWindowText) — never the status/message line.
    CDocument* pDoc = bAddToTitle ? GetActiveDocument() : nullptr;
    UpdateFrameTitleForDocument(pDoc ? pDoc->GetTitle() : nullptr);
}

void CFrameWnd::OnUpdateKeyIndicator(CCmdUI* pCmdUI) {
    if (!pCmdUI) {
        return;
    }

    pCmdUI->Enable(TRUE);
    WCHAR capsState[16] = L"";
    if ((::GetKeyState(VK_CAPITAL) & 0x0001) != 0) {
        lstrcpynW(capsState, L"CAP", 4);
    }
    if ((::GetKeyState(VK_NUMLOCK) & 0x0001) != 0) {
        lstrcpynW(capsState, L"CAP NUM", 8);
    }
    pCmdUI->SetText(capsState);
}
void CFrameWnd::OnVScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (pScrollBar && pScrollBar->m_hWnd) {
        ::SendMessageW(pScrollBar->m_hWnd, WM_VSCROLL, MAKELONG(nPos, nSBCode), 0);
    }
    if (m_pViewActive) {
        m_pViewActive->OnVScroll(nSBCode, nPos, pScrollBar);
    }
}
void CFrameWnd::PostNcDestroy() {
    RemoveFrameWnd();
}
int CFrameWnd::PreTranslateMessage(MSG* pMsg) {
    if (!pMsg) {
        return FALSE;
    }

    if (!m_hWnd) {
        return FALSE;
    }

    if (m_hAccelTable && pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) {
        if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) {
            return TRUE;
        }
    }

    if (CWnd::WalkPreTranslateTree(m_hWnd, pMsg)) {
        return TRUE;
    }

    return CWnd_PreTranslateMessageCompat(this, pMsg);
}
int CFrameWnd::ProcessHelpMsg(MSG& msg, DWORD* pContext) {
    (void)pContext;
    if (!m_hWnd) {
        return 0;
    }
    if (msg.message == WM_HELP) {
        OnHelp();
        return 1;
    }
    return 0;
}
void CFrameWnd::ReDockControlBar(CControlBar* pBar, CDockBar* pDockBar, const RECT* lpRect) {
    (void)pDockBar;
    DockControlBar(pBar, 0u, lpRect);
}

void CFrameWnd::RemoveControlBar(CControlBar* pBar) {
    if (!pBar) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    state.controlBars.erase(std::remove(state.controlBars.begin(), state.controlBars.end(), pBar), state.controlBars.end());
    if (pBar->m_hWnd) {
        ::ShowWindow(pBar->m_hWnd, SW_HIDE);
    }
}
void CFrameWnd::RemoveFrameWnd() {
    g_frameWndRuntimeStates.erase(this);
}
void CFrameWnd::SaveBarState(const wchar_t* lpszProfileName) const {
    if (!m_hWnd || !lpszProfileName) {
        return;
    }
    auto& state = GetFrameRuntimeState(const_cast<CFrameWnd*>(this));
    std::vector<int> vis;
    vis.reserve(state.controlBars.size());
    for (CControlBar* pBar : state.controlBars) {
        vis.push_back(pBar && pBar->m_hWnd && ::IsWindowVisible(pBar->m_hWnd) ? 1 : 0);
    }
    std::lock_guard<std::mutex> lk(g_wndStateMutex);
    g_barStateStore[lpszProfileName] = std::move(vis);
}
void CFrameWnd::SetActivePreviewView(CView* pViewNew) { SetActiveView(pViewNew, TRUE); }
void CFrameWnd::SetActiveView(CView* pViewNew, int bNotify) {
    CView* pOldView = dynamic_cast<CView*>(m_pViewActive);
    if (pOldView == pViewNew) {
        return;
    }

    if (bNotify && pOldView) {
        pOldView->OnActivateView(FALSE, pViewNew, pOldView);
    }

    m_pViewActive = pViewNew;

    if (bNotify && pViewNew) {
        pViewNew->OnActivateView(TRUE, pViewNew, pOldView);
        if (pViewNew->m_hWnd) {
            ::SetFocus(pViewNew->m_hWnd);
        }
    }
}
void CFrameWnd::SetDockState(const CDockState& state) {
    std::vector<DockBarSnapshot> snapshot;
    {
        std::lock_guard<std::mutex> lk(g_wndStateMutex);
        auto it = g_dockStateSnapshots.find(&state);
        if (it == g_dockStateSnapshots.end()) {
            return;
        }
        snapshot = it->second;
    }

    auto& frameState = GetFrameRuntimeState(this);
    for (const DockBarSnapshot& item : snapshot) {
        CControlBar* bar = nullptr;
        auto byPointer = std::find(frameState.controlBars.begin(), frameState.controlBars.end(), item.bar);
        if (byPointer != frameState.controlBars.end()) {
            bar = *byPointer;
        } else if (item.id != 0) {
            for (CControlBar* candidate : frameState.controlBars) {
                if (candidate && candidate->m_hWnd &&
                    static_cast<UINT>(::GetWindowLongPtrW(candidate->m_hWnd, GWLP_ID)) == item.id) {
                    bar = candidate;
                    break;
                }
            }
        }
        if (!bar) {
            continue;
        }

        bar->m_dwDockStyle = item.dockStyle;
        if (bar->m_hWnd) {
            const int width = item.windowRect.right - item.windowRect.left;
            const int height = item.windowRect.bottom - item.windowRect.top;
            if (width > 0 && height > 0) {
                ::SetWindowPos(bar->m_hWnd, nullptr,
                               item.windowRect.left, item.windowRect.top,
                               width, height, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            ::ShowWindow(bar->m_hWnd, item.visible ? SW_SHOWNA : SW_HIDE);
        }
    }

    if (m_hWnd) {
        RECT clientRect = {};
        if (::GetClientRect(m_hWnd, &clientRect)) {
            OnSize(SIZE_RESTORED, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        } else {
            OnSize(SIZE_RESTORED, 0, 0);
        }
    }
}
HWND CFrameWnd::SetHelpCapture(POINT pt, int* pHit) {
    if (!m_hWnd) {
        if (pHit) {
            *pHit = HTNOWHERE;
        }
        return nullptr;
    }

    if (pHit) {
        *pHit = HTCLIENT;
    }
    return m_hWnd;
}
int CFrameWnd::SetMenu(CMenu* pMenu) {
    if (!m_hWnd) {
        return FALSE;
    }
    HMENU hMenu = pMenu ? pMenu->m_hMenu : nullptr;
    ::SetMenu(m_hWnd, hMenu);
    ::DrawMenuBar(m_hWnd);
    return TRUE;
}

int CFrameWnd::SetMenuBarState(DWORD dwState) {
    auto& state = GetFrameRuntimeState(this);
    DWORD oldState = state.menuBarState;
    state.menuBarState = dwState;
    return static_cast<int>(oldState);
}

void CFrameWnd::SetMenuBarVisibility(DWORD dwStyle) {
    // Do NOT assign state.menuBarVisibility here: OnHideMenuBar/OnShowMenuBar
    // early-return when the recorded visibility already matches, so writing the
    // new value first would make them no-ops. Let the handlers update the state.
    if (dwStyle == 0) {
        OnHideMenuBar();
    } else {
        OnShowMenuBar();
    }
}

void CFrameWnd::SetMessageText(unsigned int nStringID) {
    CString message;
    GetMessageString(nStringID, message);
    SetMessageText(message.GetString());
}

void CFrameWnd::SetMessageText(const wchar_t* lpszText) {
    const wchar_t* text = lpszText ? lpszText : L"";
    if (!m_hWnd) {
        return;
    }

    CWnd* pMessageBar = GetMessageBar();
    if (pMessageBar && pMessageBar->m_hWnd) {
        ::SendMessageW(pMessageBar->m_hWnd, SB_SETTEXTW, 0, (LPARAM)text);
    } else {
        ::SendMessageW(m_hWnd, WM_SETTEXT, 0, (LPARAM)text);
    }
}

void CFrameWnd::SetProgressBarPosition(int nPos) {
    if (!m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    int minPos = state.progressMin;
    int maxPos = state.progressMax;
    if (maxPos < minPos) {
        int tmp = minPos;
        minPos = maxPos;
        maxPos = tmp;
    }
    if (nPos < minPos) {
        nPos = minPos;
    } else if (nPos > maxPos) {
        nPos = maxPos;
    }
    state.progressPos = nPos;

    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return;
    }
    ULONGLONG total = (maxPos > minPos) ? static_cast<ULONGLONG>(maxPos - minPos) : 1;
    ULONGLONG completed = static_cast<ULONGLONG>(nPos - minPos);
    taskbar->SetProgressValue(m_hWnd, completed, total);
    taskbar->Release();
}

void CFrameWnd::SetProgressBarRange(int nLower, int nUpper) {
    auto& state = GetFrameRuntimeState(this);
    if (nLower <= nUpper) {
        state.progressMin = nLower;
        state.progressMax = nUpper;
    } else {
        state.progressMin = nUpper;
        state.progressMax = nLower;
    }

    if (!m_hWnd) {
        return;
    }
    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return;
    }
    ULONGLONG total = (state.progressMax > state.progressMin)
        ? static_cast<ULONGLONG>(state.progressMax - state.progressMin)
        : 1ULL;
    taskbar->SetProgressValue(m_hWnd,
                              static_cast<ULONGLONG>(state.progressPos - state.progressMin),
                              total);
    taskbar->Release();
}

void CFrameWnd::SetProgressBarState(TBPFLAG tbpFlags) {
    if (!m_hWnd) {
        return;
    }
    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return;
    }
    taskbar->SetProgressState(m_hWnd, tbpFlags);
    taskbar->Release();
}

int CFrameWnd::SetTaskbarOverlayIcon(unsigned int nTaskbarButtonCreatedMsg, const wchar_t* lpszDescription) {
    if (!m_hWnd) {
        return FALSE;
    }
    if (nTaskbarButtonCreatedMsg) {
        UINT taskbarMsg = ::RegisterWindowMessageW(L"TaskbarButtonCreated");
        if (nTaskbarButtonCreatedMsg != taskbarMsg) {
            return TRUE;
        }
    }

    HICON hIcon = reinterpret_cast<HICON>(::SendMessageW(m_hWnd, WM_GETICON, ICON_SMALL, 0));
    if (!hIcon) {
        hIcon = reinterpret_cast<HICON>(::GetClassLongPtrW(m_hWnd, GCLP_HICON));
    }
    return SetTaskbarOverlayIcon(hIcon, lpszDescription);
}

int CFrameWnd::SetTaskbarOverlayIcon(HICON hIcon, const wchar_t* lpszDescription) {
    if (!m_hWnd) {
        return FALSE;
    }
    ITaskbarList3* taskbar = AcquireTaskbarList3ForWindow(m_hWnd);
    if (!taskbar) {
        return FALSE;
    }
    HRESULT hr = taskbar->SetOverlayIcon(m_hWnd, hIcon, lpszDescription);
    taskbar->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void CFrameWnd::ShowControlBar(CControlBar* pBar, int bShow, int bDelay) {
    (void)bDelay;
    if (!pBar || !pBar->m_hWnd) {
        return;
    }

    auto& state = GetFrameRuntimeState(this);
    bool isDocked = false;
    for (CControlBar* item : state.controlBars) {
        if (item == pBar) {
            isDocked = true;
            break;
        }
    }
    if (!isDocked) {
        state.controlBars.push_back(pBar);
    }

    ::ShowWindow(pBar->m_hWnd, bShow ? SW_SHOW : SW_HIDE);
}

void CFrameWnd::ShowOwnedWindows(int bShow) {
    if (!m_hWnd) {
        return;
    }

    for (HWND hWndChild = ::GetTopWindow(nullptr); hWndChild; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT)) {
        HWND hOwner = ::GetWindow(hWndChild, GW_OWNER);
        while (hOwner) {
            if (hOwner == m_hWnd) {
                ::ShowWindow(hWndChild, bShow ? SW_SHOW : SW_HIDE);
                break;
            }
            hOwner = ::GetWindow(hOwner, GW_OWNER);
        }
    }
}

void CFrameWnd::UpdateFrameTitleForDocument(const wchar_t* lpszDocName) {
    const wchar_t* baseName = L"OpenMFC";
    if (AfxGetApp() && AfxGetApp()->m_pszAppName && AfxGetApp()->m_pszAppName[0] != L'\0') {
        baseName = AfxGetApp()->m_pszAppName;
    }

    std::wstring text = baseName;
    if (lpszDocName && lpszDocName[0] != L'\0') {
        text = std::wstring(lpszDocName) + L" - " + text;
    }

    if (m_hWnd) {
        ::SetWindowTextW(m_hWnd, text.c_str());
    }
}
