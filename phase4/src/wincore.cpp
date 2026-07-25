// Window Core Implementation
//
// Implements CWnd, CFrameWnd, and related window creation/management.
// This is essential for Hello World applications.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include <windows.h>
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

// =============================================================================
// Global State
// =============================================================================

// Map HWND to CWnd* for message routing
#include <map>
#include <set>
#include <vector>
static std::map<HWND, CWnd*> g_hwndMap;
static std::map<const CWnd*, COleControlContainer*> g_controlContainerMap;
struct CWndRuntimeState {
    bool toolTipsEnabled = false;
    bool trackingToolTipsEnabled = false;
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
    int progressMin = 0;
    int progressMax = 100;
    int progressPos = 0;
    DWORD dockingStyle = 0;
    int modalDepth = 0;
    HWND modalOwner = nullptr;
    std::vector<CControlBar*> controlBars;
};
std::map<CFrameWnd*, CFrameWndRuntimeState> g_frameWndRuntimeStates;

// Track temporary CWnd wrappers allocated by OpenMfcAttachCWnd
// These need to be deleted when the underlying window is destroyed
static std::set<CWnd*> g_tempWrappers;

static CWndRuntimeState& GetWindowRuntimeState(CWnd* pWnd) {
    return g_wndRuntimeStates[pWnd];
}

static CFrameWndRuntimeState& GetFrameRuntimeState(CFrameWnd* pWnd) {
    return g_frameWndRuntimeStates[pWnd];
}

static void CleanupWindowRuntimeState(CWnd* pWnd) {
    if (!pWnd) {
        return;
    }
    g_wndRuntimeStates.erase(pWnd);
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
    (void)pThis;
    // Default implementation - allow creation
    // Derived classes override to modify cs
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
    (void)pThis;
    // Frame windows typically don't need modification
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
        hIcon = ::LoadIconW(nullptr, IDI_APPLICATION);
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
    (void)p0;
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
    (void)p0;
    return nullptr;
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
    (void)p0;
    (void)p1;
    return nullptr;
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
    return nullptr;
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
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return 0;
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
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::SendMessageToDescendants(HWND p0, UINT p1, ULONGLONG p2, LONGLONG p3, int p4, int p5)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
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
    (void)p0;
}

void CWnd::AttachControlSite(COleControlSite* p0, UINT p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::BindDefaultProperty(long p0, WORD p1, const WCHAR* p2, void* p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
}

void CWnd::BindProperty(long p0, void* p1)
{
    (void)p0;
    (void)p1;
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
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    return 0;
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
    (void)pThis;
    (void)clsid;
    (void)pWindowName;
    (void)dwStyle;
    (void)rect;
    (void)pParentWnd;
    (void)nID;
    (void)pPersist;
    (void)bStorage;
    (void)pLicKey;
    return FALSE;
}

int CWnd::CreateControl(const GUID*& p0, const WCHAR* p1, DWORD p2, const POINT* p3, const SIZE* p4, void* p5, UINT p6, CFile* p7, int p8, WCHAR* p9)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    (void)p9;
    return 0;
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
    (void)pThis;
    (void)clsid;
    (void)pWindowName;
    (void)dwStyle;
    (void)pPoint;
    (void)pSize;
    (void)pParentWnd;
    (void)nID;
    (void)pPersist;
    (void)bStorage;
    (void)pLicKey;
    return FALSE;
}

int CWnd::CreateControl(const CControlCreationInfo*& p0, DWORD p1, const POINT* p2, const SIZE* p3, void* p4, UINT p5)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    return 0;
}

int CWnd::CreateControl(const WCHAR* p0, const WCHAR* p1, DWORD p2, const RECT*& p3, void* p4, UINT p5, CFile* p6, int p7, WCHAR* p8)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    return 0;
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
    (void)pThis;
    (void)lpszClass;
    (void)pWindowName;
    (void)dwStyle;
    (void)rect;
    (void)pParentWnd;
    (void)nID;
    (void)pPersist;
    (void)bStorage;
    (void)pLicKey;
    return FALSE;
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
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

int CWnd::CreateDlg(const WCHAR* p0, void* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::CreateDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

// Symbol: ?CreateDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* pThis, const DLGTEMPLATE* pTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    (void)pThis;
    (void)pTemplate;
    (void)pParentWnd;
    (void)hInst;
    return FALSE;
}

int CWnd::CreateRunDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
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
    (void)p0;
    (void)p1;
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
    (void)p0;
}

void* CWnd::FindSiteOrWnd(const COleControlSiteOrWnd* p0) const
{
    (void)p0;
    return nullptr;
}

void* CWnd::FindSiteOrWndWithFocus() const
{
    return nullptr;
}

long CWnd::GetAccessibilityHitTest(long p0, long p1, VARIANT* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

long CWnd::GetAccessibilityLocation(VARIANT p0, long* p1, long* p2, long* p3, long* p4)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return 0;
}

long CWnd::GetAccessibleChild(VARIANT p0, IDispatch** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::GetAccessibleChildCount()
{
    return 0;
}

long CWnd::GetAccessibleName(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
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
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

int CWnd::GetDlgItemTextW(int p0, void*& p1, void** p2, void* p3) const
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
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
    return nullptr;
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
    (void)p0;
    return 0;
}

DWORD CWnd::GetGestureStatus(CPoint p0)
{
    (void)p0;
    return 0;
}

const AFX_INTERFACEMAP* CWnd::GetInterfaceMap() const
{
    return nullptr;
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
    (void)p0;
    return nullptr;
}

COleControlSiteOrWnd* CWnd::GetNextDlgTabItem(void* p0, int p1) const
{
    (void)p0;
    (void)p1;
    return nullptr;
}

_AFX_OCC_DIALOG_INFO* CWnd::GetOccDialogInfo()
{
    return nullptr;
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
    (void)p0;
    return nullptr;
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
    auto it = g_wndRuntimeStates.find(this);
    if (it == g_wndRuntimeStates.end() || !it->second.d2dSupportEnabled) {
        return nullptr;
    }
    return nullptr;
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
    return m_pfnSuper;
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
    return 0;
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
    return 0;
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
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
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
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::OnCharToItem(UINT p0, CListBox* p1, UINT p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

int CWnd::OnChildNotify(UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

int CWnd::OnCompareItem(int p0, COMPAREITEMSTRUCT* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

HBRUSH CWnd::OnCtlColor(CDC* p0, void* p1, UINT p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

int CWnd::OnDeleteItem(int p0, DELETEITEMSTRUCT* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::OnDestroy()
{
}

void CWnd::OnDevModeChange(void* p0, const WCHAR* p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::OnDisplayChange(UINT p0, int p1, int p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
}

LONGLONG CWnd::OnDragList(ULONGLONG p0, LONGLONG p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::OnDrawItem(int p0, DRAWITEMSTRUCT* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::OnEnterIdle(UINT p0, void* p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::OnGesture(void* p0, ULONGLONG p1, LONGLONG p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
}

int CWnd::OnGesturePan(void* p0, CPoint p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::OnGesturePressAndTap(void* p0, CPoint p1, DWORD p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

int CWnd::OnGestureRotate(void* p0, CPoint p1, DWORD p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

int CWnd::OnGestureTwoFingerTap(void* p0, CPoint p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::OnGestureZoom(void* p0, CPoint p1, DWORD p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

LONGLONG CWnd::OnGetObject(ULONGLONG p0, LONGLONG p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::OnHScroll(UINT p0, UINT p1, CScrollBar* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
}

void CWnd::OnHelp()
{
}

void CWnd::OnHelpFinder()
{
}

void CWnd::OnHelpIndex()
{
}

void CWnd::OnHelpInfo(HELPINFO* p0)
{
    (void)p0;
}

void CWnd::OnHelpUsing()
{
}

void CWnd::OnMeasureItem(int p0, MEASUREITEMSTRUCT* p1)
{
    (void)p0;
    (void)p1;
}

LONGLONG CWnd::OnNTCtlColor(ULONGLONG p0, LONGLONG p1)
{
    (void)p0;
    (void)p1;
    return 0;
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
}

int CWnd::OnParentNotify(UINT p0, LONGLONG p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::OnSetFocus(void* p0)
{
    (void)p0;
}

void CWnd::OnSettingChange(UINT p0, const WCHAR* p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::OnSysColorChange()
{
}

LONGLONG CWnd::OnTabletQuerySystemGestureStatus(ULONGLONG p0, LONGLONG p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::OnTouchInput(ULONGLONG p0, ULONGLONG p1, TOUCHINPUT* p2, UINT p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
}

int CWnd::OnTouchInputs(ULONGLONG p0, ULONGLONG p1, TOUCHINPUT* p2, UINT p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

LONGLONG CWnd::OnTouchMessage(ULONGLONG p0, LONGLONG p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::OnVKeyToItem(UINT p0, CListBox* p1, UINT p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

void CWnd::OnVScroll(UINT p0, UINT p1, CScrollBar* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
}

void CWnd::PaintWindowlessControls(CDC* p0)
{
    (void)p0;
}

void CWnd::PostNcDestroy()
{
}

void CWnd::PreTranslateInput(MSG* p0)
{
    (void)p0;
}

void CWnd::PrepareForHelp()
{
}

int CWnd::ReflectChildNotify(UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

int CWnd::RegisterTouchWindow(ULONGLONG p0)
{
    (void)p0;
    return 0;
}

void CWnd::RemoveRadioCheckFromGroup(const CObject* p0)
{
    (void)p0;
}

void CWnd::RepositionBars(UINT p0, UINT p1, UINT p2, UINT p3, RECT* p4, RECT* p5, int p6)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
}

LONGLONG CWnd::RunModalLoop(DWORD p0)
{
    (void)p0;
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
    (void)nBar;
    if (!pThis || !pThis->m_hWnd) {
        return nullptr;
    }
    return nullptr;
}

// Symbol: ?IsFrameWnd@CWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis) {
    (void)pThis;
    return FALSE;
}

// Symbol: ?OnAmbientProperty@CWnd@@UEAAHPEAVCOleControlSite@@JPEAUtagVARIANT@@@Z
extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar)
{
    (void)pThis;
    (void)pSite;
    (void)dispid;
    (void)pVar;
    return FALSE;
}

// Symbol: ?OnToolHitTest@CWnd@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI) {
    (void)pThis;
    (void)point;
    if (pTI) {
        std::memset(pTI, 0, sizeof(*pTI));
    }
    return -1;
}

// Symbol: ?PreSubclassWindow@CWnd@@UEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CWnd__UEAAXXZ(CWnd* pThis) {
    (void)pThis;
}

// Symbol: ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg) {
    return CWnd_PreTranslateMessageCompat(pThis, pMsg);
}

// Symbol: ?OnCommand@CWnd@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return FALSE;
}

// Symbol: ?OnFinalRelease@CWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_CWnd__UEAAXXZ(CWnd* pThis) {
    (void)pThis;
}

// Symbol: ?OnNotify@CWnd@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(
    CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    if (pResult) {
        *pResult = 0;
    }
    return FALSE;
}

// Symbol: ?OnWndMsg@CWnd@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) {
        *pResult = 0;
    }
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

void CWnd::SetDlgCtrlID(int p0)
{
    if (!m_hWnd) {
        return;
    }
    ::SetWindowLongPtrW(m_hWnd, GWL_ID, static_cast<LONG_PTR>(p0));
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
    (void)p0;
    return 0;
}

void CWnd::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* p0)
{
    (void)p0;
}

int CWnd::SetProperty(long p0, WORD p1, void* p2)
{
    auto& state = GetWindowRuntimeState(this);
    state.properties[{p0, p1}] = p2;
    return TRUE;
}

int CWnd::SetScrollInfo(int p0, SCROLLINFO* p1, int p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

int CWnd::SetScrollPos(int p0, int p1, int p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

void CWnd::SetScrollRange(int p0, int p1, int p2, int p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
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
    (void)p0;
    (void)p1;
    return 0;
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
    (void)p0;
    return TRUE;
}

void CWnd::UpdateDialogControls(void* p0, int p1)
{
    (void)p0;
    (void)p1;
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
    return nullptr;
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
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
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
    if (dwStyle == 0 && m_hWnd) {
        ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
    }
    return nullptr;
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
    DockControlBar(pBar, static_cast<CDockBar*>(nullptr), lpRect);
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
    SetMessageText(L"");
    if (m_hWnd && ::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
    }
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
    (void)state;
    if (!m_hWnd) {
        return;
    }
    auto& frameState = GetFrameRuntimeState(const_cast<CFrameWnd*>(this));
    std::wstring text = L"Dock state contains ";
    text += std::to_wstring(static_cast<unsigned long long>(frameState.controlBars.size()));
    text += L" control bar(s)";
    const_cast<CFrameWnd*>(this)->SetMessageText(text.c_str());
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
void CFrameWnd::LoadAccelTable(const wchar_t* lpszAccelTable) {
    m_hAccelTable = nullptr;
    if (!lpszAccelTable) {
        return;
    }
    m_hAccelTable = ::LoadAcceleratorsW(AfxGetResourceHandle(), lpszAccelTable);
}
void CFrameWnd::LoadBarState(const wchar_t* lpszProfileName) {
    if (!lpszProfileName) {
        return;
    }
    auto& state = GetFrameRuntimeState(this);
    if (state.menuBarVisibility == 0) {
        OnShowMenuBar();
    }
    std::wstring profileName = std::wstring(L"LoadBarState: ");
    profileName += lpszProfileName;
    SetMessageText(profileName.c_str());
}
int CFrameWnd::NegotiateBorderSpace(unsigned int nBorderCmd, RECT* lpRectBorder) {
    if (!m_hWnd || !lpRectBorder || nBorderCmd == 0) {
        return 0;
    }
    return 0;
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
void CFrameWnd::OnChevronPushed(unsigned int nIndex, NMHDR* pNMHDR, __int64* lResult) {
    (void)nIndex;
    (void)pNMHDR;
    if (lResult) {
        *lResult = 0;
    }
    SetMessageText(L"Chevron overflow command");
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
    (void)pData;
    if (!m_hWnd) {
        return;
    }
    SetMessageText(pWnd ? L"DDE execute" : L"System DDE execute");
}
void CFrameWnd::OnDDEInitiate(CWnd* pWnd, unsigned int nAtomApp, unsigned int nAtomTopic) {
    if (!m_hWnd) {
        return;
    }
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

    std::wstring message = pWnd ? L"DDE initiate" : L"System DDE initiate";
    if (appName[0] || topicName[0]) {
        message += L" : ";
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
}
void CFrameWnd::OnDDETerminate(CWnd* pWnd) {
    if (!m_hWnd) {
        return;
    }
    SetMessageText(pWnd ? L"DDE terminate" : L"System DDE terminate");
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

    UINT count = ::DragQueryFileW(hDropInfo, 0xFFFFFFFF, nullptr, 0);
    if (count > 0) {
        wchar_t filePath[MAX_PATH] = {};
        if (::DragQueryFileW(hDropInfo, 0, filePath, MAX_PATH) > 0) {
            std::wstring dropped(filePath);
            SetMessageText((std::wstring(L"Dropped: ") + dropped).c_str());
        }
    } else {
        SetMessageText(L"No files dropped");
    }
    ::DragFinish(hDropInfo);
}

void CFrameWnd::OnEnable(int bEnable) {
    if (!m_hWnd) {
        return;
    }

    if (!::EnableWindow(m_hWnd, bEnable)) {
        SetMessageText(L"Frame window disabled");
    }
}
void CFrameWnd::OnEndSession(int bEnding) {
    if (!m_hWnd) {
        return;
    }
    SetMessageText(bEnding ? L"Session ending" : L"Session ended");
}
void CFrameWnd::OnEnterIdle(unsigned int nWhy, CWnd* pWho) {
    (void)pWho;
    if (!m_hWnd) {
        return;
    }
    if (nWhy == MSGF_MENU || nWhy == MSGF_DIALOGBOX) {
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
    (void)nFlags;
    if (!m_hWnd) {
        return MNC_IGNORE;
    }
    if (pMenu) {
        SetMessageText(L"Menu char");
    } else {
        SetMessageText(nChar ? L"Menu char" : L"");
    }
    return 0;
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
    (void)wParam;
    (void)lParam;
    SetMessageText(L"");
    return 0;
}
int CFrameWnd::OnQueryEndSession() {
    SetMessageText(L"Query end session");
    return 1;
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
        HCURSOR hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
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
    (void)pState;
    if (bPreview) {
        SetMessageText(L"Print preview");
    } else {
        SetMessageText(L"");
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
    if (!hMenuToShow && m_nIDHelp) {
        HINSTANCE hInst = AfxGetInstanceHandle();
        hMenuToShow = ::LoadMenuW(hInst ? hInst : ::GetModuleHandleW(nullptr),
                                  MAKEINTRESOURCEW(m_nIDHelp));
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

void CFrameWnd::OnToolTipText(unsigned int nID, NMHDR* pNMHDR, __int64* lResult) {
    (void)nID;
    if (!pNMHDR || !lResult) {
        return;
    }

    TOOLTIPTEXTW* pTTT = reinterpret_cast<TOOLTIPTEXTW*>(pNMHDR);
    if (pNMHDR->code != TTN_NEEDTEXTW && pNMHDR->code != TTN_NEEDTEXTA) {
        *lResult = 0;
        return;
    }

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
            lstrcpynW(pTTT->lpszText, text.GetString(), 80);
        }
    }

    *lResult = 0;
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
    if (!m_hWnd) {
        return;
    }
    if (!hMenuAlt) {
        SetMessageText(L"Menu inactive");
        return;
    }
    SetMessageText(L"Menu active");
}

void CFrameWnd::OnUpdateFrameTitle(int bAddToTitle) {
    CDocument* pDoc = GetActiveDocument();
    if (!bAddToTitle && pDoc) {
        SetMessageText(pDoc->GetTitle());
        return;
    }
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
    DockControlBar(pBar, static_cast<CDockBar*>(nullptr), lpRect);
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
    if (!m_hWnd) {
        return;
    }
    if (!lpszProfileName) {
        return;
    }
    std::wstring profileName = std::wstring(L"Save bar state: ") + lpszProfileName;
    ::SendMessageW(m_hWnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(profileName.c_str()));
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
    (void)state;
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
    auto& state = GetFrameRuntimeState(this);
    if (state.menuBarVisibility == dwStyle) {
        return;
    }
    state.menuBarVisibility = dwStyle;
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
