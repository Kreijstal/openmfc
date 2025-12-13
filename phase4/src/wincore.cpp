// Window Core Implementation
//
// Implements CWnd, CFrameWnd, and related window creation/management.
// This is essential for Hello World applications.

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

// =============================================================================
// Global State
// =============================================================================

// Map HWND to CWnd* for message routing
#include <map>
static std::map<HWND, CWnd*> g_hwndMap;

// Global app pointer (defined in appcore.cpp)
extern CWinApp* g_pApp;

// OpenMFC window class name
static const wchar_t* g_szOpenMFCClass = L"OpenMFC_Window";
static ATOM g_atomOpenMFCClass = 0;

// =============================================================================
// CWnd Implementation
// =============================================================================

IMPLEMENT_DYNAMIC(CFrameWnd, CWnd)

// CWnd::Create
// Symbol: ?Create@CWnd@@UAAHPB_W0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z
// Ordinal: 3182
extern "C" int MS_ABI stub__Create_CWnd__UAAHPB_W0KABUtagRECT__PAV1_IPAUCCreateContext___Z(
    CWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    void* pContext)
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

    if (!pThis->PreCreateWindow(cs)) {
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
extern "C" int MS_ABI stub__ShowWindow_CWnd__QAAHH_Z(CWnd* pThis, int nCmdShow) {
    if (!pThis || !pThis->m_hWnd) {
        return FALSE;
    }
    return ::ShowWindow(pThis->m_hWnd, nCmdShow);
}

// CWnd::UpdateWindow
// Symbol: ?UpdateWindow@CWnd@@QAAXXZ
extern "C" void MS_ABI stub__UpdateWindow_CWnd__QAAXXZ(CWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        ::UpdateWindow(pThis->m_hWnd);
    }
}

// CWnd::DestroyWindow
// Symbol: ?DestroyWindow@CWnd@@UAAHXZ
extern "C" int MS_ABI stub__DestroyWindow_CWnd__UAAHXZ(CWnd* pThis) {
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
extern "C" int MS_ABI stub__PreCreateWindow_CWnd__UAAHAAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs)
{
    (void)pThis;
    // Default implementation - allow creation
    // Derived classes override to modify cs
    return TRUE;
}

// CWnd::DefWindowProc
// Symbol: ?DefWindowProc@CWnd@@MAAJAIH_J@Z
extern "C" LRESULT MS_ABI stub__DefWindowProc_CWnd__MAAJAIH_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (pThis && pThis->m_hWnd) {
        return ::DefWindowProcW(pThis->m_hWnd, message, wParam, lParam);
    }
    return 0;
}

// CWnd::WindowProc
// Symbol: ?WindowProc@CWnd@@MAAJAIH_J@Z
extern "C" LRESULT MS_ABI stub__WindowProc_CWnd__MAAJAIH_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Default - just call DefWindowProc
    return stub__DefWindowProc_CWnd__MAAJAIH_J_Z(pThis, message, wParam, lParam);
}

// CWnd::GetSafeHwnd
// Symbol: ?GetSafeHwnd@CWnd@@QBAPAUHWND__@@XZ
extern "C" HWND MS_ABI stub__GetSafeHwnd_CWnd__QBAPAUHWND____XZ(const CWnd* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}

// =============================================================================
// CFrameWnd Implementation
// =============================================================================

// CFrameWnd constructor
// Symbol: ??0CFrameWnd@@QAA@XZ
// Ordinal: 502
extern "C" void MS_ABI stub___0CFrameWnd__QAA_XZ(CFrameWnd* pThis) {
    // Zero initialize
    memset(pThis, 0, sizeof(CFrameWnd));
    // Set up vtable pointer would happen automatically in real MFC
}

// CFrameWnd destructor
// Symbol: ??1CFrameWnd@@UAA@XZ
// Ordinal: 1129
extern "C" void MS_ABI stub___1CFrameWnd__UAA_XZ(CFrameWnd* pThis) {
    if (pThis && pThis->m_hWnd) {
        g_hwndMap.erase(pThis->m_hWnd);
        ::DestroyWindow(pThis->m_hWnd);
        pThis->m_hWnd = nullptr;
    }
}

// CFrameWnd::Create
// Symbol: ?Create@CFrameWnd@@UAAHPB_W0KABUtagRECT@@PAVCWnd@@0KPAUCCreateContext@@@Z
// Ordinal: 3091
extern "C" int MS_ABI stub__Create_CFrameWnd__UAAHPB_W0KABUtagRECT__PAVCWnd__0KPAUCCreateContext___Z(
    CFrameWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    const wchar_t* lpszMenuName,
    DWORD dwExStyle,
    void* pContext)
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
    if (!pThis->PreCreateWindow(cs)) {
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
extern "C" int MS_ABI stub__PreCreateWindow_CFrameWnd__MAAHAAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs)
{
    (void)pThis;
    // Frame windows typically don't need modification
    return TRUE;
}

// CFrameWnd::LoadFrame
// Symbol: ?LoadFrame@CFrameWnd@@UAAHIKPAVCWnd@@PAUCCreateContext@@@Z
// Ordinal: 8105
extern "C" int MS_ABI stub__LoadFrame_CFrameWnd__UAAHIKPAVCWnd__PAUCCreateContext___Z(
    CFrameWnd* pThis,
    UINT nIDResource,
    DWORD dwDefaultStyle,
    CWnd* pParentWnd,
    void* pContext)
{
    (void)nIDResource;  // Resource loading not yet implemented
    (void)pContext;

    RECT rect = {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};

    return stub__Create_CFrameWnd__UAAHPB_W0KABUtagRECT__PAVCWnd__0KPAUCCreateContext___Z(
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
extern "C" int MS_ABI stub__AfxWinMain__YAHPAUHINSTANCE____0PA_WH_Z(
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
