// Window Core Implementation
//
// Implements CWnd, CFrameWnd, and related window creation/management.
// This is essential for Hello World applications.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>
#include <cstdio>

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

// Track temporary CWnd wrappers allocated by OpenMfcAttachCWnd
// These need to be deleted when the underlying window is destroyed
static std::set<CWnd*> g_tempWrappers;

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

// OpenMFC window class name
static const wchar_t* g_szOpenMFCClass = L"OpenMFC_Window";
static ATOM g_atomOpenMFCClass = 0;

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
// Symbol: ?FromHandle@CWnd@@SAPAV1@PAUHWND__@@@Z
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPAV1_PAUHWND_____Z(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}

CWnd* CWnd::FromHandle(HWND hWnd) {
    return OpenMfcAttachCWnd(hWnd);
}

// Symbol: ?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ() {
    return &CWnd::classCWnd;
}

#ifdef __GNUC__
asm(".globl \"?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ\n");
#endif

IMPLEMENT_DYNAMIC(CFrameWnd, CWnd)

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

// CWnd::Create
// Symbol: ?Create@CWnd@@UAAHPB_W0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z
// Ordinal: 3182
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

// CWnd::ShowWindow
// Symbol: ?ShowWindow@CWnd@@QAAHH@Z
// Ordinal: 13870
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::ShowWindow(pThis->m_hWnd, nCmdShow);
}

// CWnd::UpdateWindow
// Symbol: ?UpdateWindow@CWnd@@QAAXXZ
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

// CWnd::DestroyWindow
// Symbol: ?DestroyWindow@CWnd@@UAAHXZ
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }

    HWND hWnd = pThis->m_hWnd;
    g_hwndMap.erase(hWnd);
    pThis->m_hWnd = nullptr;

    return ::DestroyWindow(hWnd);
}

// CWnd::PreCreateWindow
// Symbol: ?PreCreateWindow@CWnd@@UAAHAAUtagCREATESTRUCTW@@@Z
// Ordinal: 11813
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs)
{
    (void)pThis;
    // Default implementation - allow creation
    // Derived classes override to modify cs
    return TRUE;
}

// CWnd::DefWindowProcW
// Symbol: ?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z (x64)
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (pThis && pThis->m_hWnd) {
        return ::DefWindowProcW(pThis->m_hWnd, message, wParam, lParam);
    }
    return 0;
}

// CWnd::WindowProc
// Symbol: ?WindowProc@CWnd@@MEAA_JI_K_J@Z (x64)
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Default - just call DefWindowProcW
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}

// CWnd::GetSafeHwnd
// Symbol: ?GetSafeHwnd@CWnd@@QBAPAUHWND__@@XZ
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
// Symbol: ??0CFrameWnd@@QAA@XZ
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
// Symbol: ??1CFrameWnd@@UAA@XZ
// Ordinal: 1129
extern "C" void MS_ABI impl___1CFrameWnd__UEAA_XZ(CFrameWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_hwndMap.erase(pThis->m_hWnd);
        ::DestroyWindow(pThis->m_hWnd);
        pThis->m_hWnd = nullptr;
    }
}

// CFrameWnd::Create
// Symbol: ?Create@CFrameWnd@@UAAHPB_W0KABUtagRECT@@PAVCWnd@@0KPAUCCreateContext@@@Z
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

// CFrameWnd::PreCreateWindow
// Symbol: ?PreCreateWindow@CFrameWnd@@MAAHAAUtagCREATESTRUCTW@@@Z
// Ordinal: 11791
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs)
{
    (void)pThis;
    // Frame windows typically don't need modification
    return TRUE;
}

// CFrameWnd::LoadFrame
// Symbol: ?LoadFrame@CFrameWnd@@UAAHIKPAVCWnd@@PAUCCreateContext@@@Z
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
    (void)lpCreateStruct;
    return 0;
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
        // Call through virtual WindowProc
        // In real MFC this would use the vtable, but for now use default
        LRESULT result = DefWindowProcW(hWnd, message, wParam, lParam);

        // WM_NCDESTROY is the final message - clean up the wrapper
        // This must come after DefWindowProc since the window is still valid during the call
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

// Symbol: ?AfxWinMain@@YAHPAUHINSTANCE__@@0PA_WH@Z
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
    memset(_mdiframe_padding, 0, sizeof(_mdiframe_padding));
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
    return nullptr;
}

void CWnd::DeleteTempMap()
{
}

void* CWnd::FromHandlePermanent(HWND p0)
{
    (void)p0;
    return nullptr;
}

const MSG* CWnd::GetCurrentMessage()
{
    return nullptr;
}

void* CWnd::GetDescendantWindow(HWND p0, int p1, int p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return nullptr;
}

void* CWnd::GetSafeOwner(void* p0, HWND* p1)
{
    (void)p0;
    (void)p1;
    return nullptr;
}

HWND CWnd::GetSafeOwner_(void* p0, void** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

const AFX_INTERFACEMAP* CWnd::GetThisInterfaceMap()
{
    return nullptr;
}

const AFX_MSGMAP* CWnd::GetThisMessageMap()
{
    return nullptr;
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
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

int CWnd::ModifyStyleEx(HWND p0, DWORD p1, DWORD p2, UINT p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
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
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::accDoDefaultAction(VARIANT p0)
{
    (void)p0;
    return 0;
}

long CWnd::accHitTest(long p0, long p1, VARIANT* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

long CWnd::accLocation(long* p0, long* p1, long* p2, long* p3, VARIANT p4)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return 0;
}

long CWnd::accNavigate(long p0, VARIANT p1, void* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

long CWnd::accSelect(long p0, VARIANT p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::CalcWindowRect(RECT* p0, UINT p1)
{
    (void)p0;
    (void)p1;
}

int CWnd::CheckAutoCenter()
{
    return 0;
}

int CWnd::ContinueModal()
{
    return 0;
}

long CWnd::CreateAccessibleProxy(ULONGLONG p0, LONGLONG p1, LONGLONG* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

void CWnd::EndModalLoop(int p0)
{
    (void)p0;
}

long CWnd::EnsureStdObj()
{
    return 0;
}

long CWnd::get_accChild(VARIANT p0, IDispatch** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accChildCount(long* p0)
{
    (void)p0;
    return 0;
}

long CWnd::get_accDefaultAction(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accDescription(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accFocus(VARIANT* p0)
{
    (void)p0;
    return 0;
}

long CWnd::get_accHelp(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accHelpTopic(WCHAR** p0, VARIANT p1, long* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

long CWnd::get_accKeyboardShortcut(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accName(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accParent(IDispatch** p0)
{
    (void)p0;
    return 0;
}

long CWnd::get_accRole(VARIANT p0, VARIANT* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accSelection(VARIANT* p0)
{
    (void)p0;
    return 0;
}

long CWnd::get_accState(VARIANT p0, VARIANT* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::get_accValue(VARIANT p0, WCHAR** p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

CScrollBar* CWnd::GetScrollBarCtrl(int p0) const
{
    (void)p0;
    return nullptr;
}

void CWnd::HtmlHelpW(ULONGLONG p0, UINT p1)
{
    (void)p0;
    (void)p1;
}

int CWnd::IsFrameWnd() const
{
    return 0;
}

int CWnd::OnAmbientProperty(COleControlSite* p0, long p1, VARIANT* p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

LONGLONG CWnd::OnToolHitTest(CPoint p0, TOOLINFOW* p1) const
{
    (void)p0;
    (void)p1;
    return 0;
}

void CWnd::PreSubclassWindow()
{
}

int CWnd::PreTranslateMessage(MSG* p0)
{
    (void)p0;
    return 0;
}

long CWnd::put_accName(VARIANT p0, WCHAR* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::put_accValue(VARIANT p0, WCHAR* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

long CWnd::SetProxy(IAccessibleProxy* p0)
{
    (void)p0;
    return 0;
}

void CWnd::WinHelpInternal(ULONGLONG p0, UINT p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::WinHelpW(ULONGLONG p0, UINT p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::ActivateTopParent()
{
}

int CWnd::Attach(HWND p0)
{
    (void)p0;
    return 0;
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
    (void)p0;
}

void CWnd::CheckDlgButton(int p0, UINT p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::CheckRadioButton(int p0, int p1, int p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
}

void CWnd::ClientToScreen(RECT* p0) const
{
    (void)p0;
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

int CWnd::CreateControlContainer(COleControlContainer** p0)
{
    (void)p0;
    return 0;
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

int CWnd::CreateRunDlgIndirect(const DLGTEMPLATE* p0, void* p1, HINSTANCE p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

LONGLONG CWnd::Default()
{
    return 0;
}

LONGLONG CWnd::DefWindowProcW(UINT p0, ULONGLONG p1, LONGLONG p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

HWND CWnd::Detach()
{
    return 0;
}

int CWnd::DoD2DPaint()
{
    return 0;
}

void CWnd::EnableScrollBarCtrl(int p0, int p1)
{
    (void)p0;
    (void)p1;
}

int CWnd::EnableToolTips(int p0)
{
    (void)p0;
    return 0;
}

int CWnd::EnableTrackingToolTips(int p0)
{
    (void)p0;
    return 0;
}

int CWnd::ExecuteDlgInit(void* p0)
{
    (void)p0;
    return 0;
}

int CWnd::ExecuteDlgInit(const WCHAR* p0)
{
    (void)p0;
    return 0;
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
    (void)p0;
    (void)p1;
    return 0;
}

COleControlContainer* CWnd::GetControlContainer()
{
    return nullptr;
}

IUnknown* CWnd::GetControlUnknown()
{
    return nullptr;
}

int CWnd::GetDlgCtrlID() const
{
    return 0;
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
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

IUnknown* CWnd::GetDSCCursor()
{
    return nullptr;
}

DWORD CWnd::GetExStyle() const
{
    return 0;
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

const AFX_MSGMAP* CWnd::GetMessageMap() const
{
    return nullptr;
}

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
    (void)p0;
    return nullptr;
}

CFrameWnd* CWnd::GetParentFrame() const
{
    return nullptr;
}

void* CWnd::GetParentOwner() const
{
    return nullptr;
}

COleControlSiteOrWnd* CWnd::GetPrevDlgGroupItem(void* p0) const
{
    (void)p0;
    return nullptr;
}

void CWnd::GetProperty(long p0, WORD p1, void* p2) const
{
    (void)p0;
    (void)p1;
    (void)p2;
}

CHwndRenderTarget* CWnd::GetRenderTarget()
{
    return nullptr;
}

DWORD CWnd::GetStyle() const
{
    return 0;
}

void* CWnd::GetSuperWndProcAddr()
{
    return nullptr;
}

CFrameWnd* CWnd::GetTopLevelFrame() const
{
    return nullptr;
}

void* CWnd::GetTopLevelOwner() const
{
    return nullptr;
}

void* CWnd::GetTopLevelParent() const
{
    return nullptr;
}

long CWnd::GetWindowLessChildCount()
{
    return 0;
}

int CWnd::GetWindowPlacement(WINDOWPLACEMENT* p0) const
{
    (void)p0;
    return 0;
}

long CWnd::GetWindowedChildCount()
{
    return 0;
}

int CWnd::HandleFloatingSysCommand(UINT p0, INT_PTR p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::InitControlContainer(int p0)
{
    (void)p0;
    return 0;
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
    (void)p0;
    return 0;
}

int CWnd::IsTopParentActive() const
{
    return 0;
}

int CWnd::IsTouchWindow() const
{
    return 0;
}

int CWnd::MessageBoxW(const WCHAR* p0, const WCHAR* p1, UINT p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
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
    return 0;
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
    (void)p0;
}

void CWnd::ScrollWindow(int p0, int p1, const RECT* p2, const RECT* p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
}

int CWnd::ScrollWindowEx(int p0, int p1, const RECT* p2, const RECT* p3, CRgn* p4, RECT* p5, UINT p6)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    return 0;
}

int CWnd::SendChildNotifyLastMsg(LONGLONG* p0)
{
    (void)p0;
    return 0;
}

LONGLONG CWnd::SendDlgItemMessageW(int p0, UINT p1, ULONGLONG p2, LONGLONG p3)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

void CWnd::SetDlgCtrlID(int p0)
{
    (void)p0;
}

void CWnd::SetDlgItemInt(int p0, UINT p1, int p2)
{
    (void)p0;
    (void)p1;
    (void)p2;
}

void CWnd::SetDlgItemTextW(int p0, const WCHAR* p1)
{
    (void)p0;
    (void)p1;
}

void* CWnd::SetFocus()
{
    return nullptr;
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
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
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
    (void)p0;
    return 0;
}

int CWnd::SubclassDlgItem(UINT p0, void* p1)
{
    (void)p0;
    (void)p1;
    return 0;
}

int CWnd::SubclassWindow(HWND p0)
{
    (void)p0;
    return 0;
}

HWND CWnd::UnsubclassWindow()
{
    return 0;
}

int CWnd::UpdateData(int p0)
{
    (void)p0;
    return 0;
}

void CWnd::UpdateDialogControls(void* p0, int p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::EnableD2DSupport(int p0, int p1)
{
    (void)p0;
    (void)p1;
}

void CWnd::EnableDynamicLayout(int p0)
{
    (void)p0;
}

CDCRenderTarget* CWnd::GetDCRenderTarget()
{
    return nullptr;
}

void CWnd::InitDynamicLayout()
{
}

int CWnd::LoadDynamicLayoutResource(const WCHAR* p0)
{
    (void)p0;
    return 0;
}

void CWnd::ResizeDynamicLayout()
{
}

int CWnd::IsD2DSupportEnabled()
{
    return 0;
}

void CWnd::OnDrawIconicThumbnailOrLivePreview(CDC*& p0, CRect p1, CSize p2, int p3, int*& p4)
{
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
}

