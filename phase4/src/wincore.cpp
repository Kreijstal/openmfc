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
static std::map<HWND, CWnd*> g_hwndMap;

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

CWnd* OpenMfcAttachCWnd(HWND hWnd) {
    if (!hWnd) {
        return nullptr;
    }
    if (CWnd* existing = OpenMfcLookupCWnd(hWnd)) {
        return existing;
    }
    CWnd* wrapper = new CWnd();
    wrapper->m_hWnd = hWnd;
    g_hwndMap[hWnd] = wrapper;
    return wrapper;
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
    wchar_t* buffer = rString->GetBuffer(length);
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
    (void)lpszMenuName;  // Menu support not yet implemented
    (void)dwExStyle;     // Extended style could be used
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
    cs.hMenu = nullptr;  // TODO: Load menu from resource
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
    (void)nIDResource;  // Resource loading not yet implemented
    (void)pContext;

    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};

    return impl__Create_CFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__0KPEAUCCreateContext___Z(
        pThis,
        nullptr,           // Default class
        L"OpenMFC Window", // Default title
        dwDefaultStyle ? dwDefaultStyle : WS_OVERLAPPEDWINDOW,
        rect,
        pParentWnd,
        nullptr,           // No menu
        0,                 // No extended style
        pContext
    );
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
        // Special handling for WM_DESTROY
        if (message == WM_DESTROY) {
            g_hwndMap.erase(hWnd);
        }

        // Call through virtual WindowProc
        // In real MFC this would use the vtable, but for now use default
        return DefWindowProcW(hWnd, message, wParam, lParam);
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
