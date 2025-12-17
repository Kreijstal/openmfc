// Registration Core Implementation
//
// Implements AfxRegisterClass, AfxRegisterWndClass and related window class registration.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Global State
// =============================================================================

// Keep track of registered class atoms for cleanup
static ATOM g_registeredClasses[256];
static int g_numRegisteredClasses = 0;

// Standard MFC window class names
static const wchar_t* AFX_WNDCLASS_MAIN = L"AfxFrameOrView140u";
static const wchar_t* AFX_WNDCLASS_MDI  = L"AfxMDIFrame140u";
static const wchar_t* AFX_WNDCLASS_CTRL = L"AfxControlBar140u";

// =============================================================================
// AfxRegisterClass
// =============================================================================

// AfxRegisterClass - Register a window class with MFC-style defaults
// Symbol: ?AfxRegisterClass@@YAHPEAUtagWNDCLASSW@@@Z
// Ordinal: 2311
extern "C" int MS_ABI stub__AfxRegisterClass__YAHPEAUtagWNDCLASSW___Z(WNDCLASSW* lpWndClass) {
    if (!lpWndClass) return FALSE;

    // Set default instance handle if not specified
    if (!lpWndClass->hInstance) {
        lpWndClass->hInstance = AfxGetInstanceHandle();
    }

    // Register the class
    ATOM atom = ::RegisterClassW(lpWndClass);
    if (atom == 0) {
        // Check if already registered
        DWORD err = ::GetLastError();
        if (err == ERROR_CLASS_ALREADY_EXISTS) {
            return TRUE;  // Already registered, that's ok
        }
        return FALSE;
    }

    // Track the registered class
    if (g_numRegisteredClasses < 256) {
        g_registeredClasses[g_numRegisteredClasses++] = atom;
    }

    return TRUE;
}

// =============================================================================
// AfxRegisterWndClass
// =============================================================================

namespace {
constexpr size_t kWndClassNameSlots = 16;
constexpr size_t kWndClassNameLen = 128;

thread_local wchar_t g_wndClassNames[kWndClassNameSlots][kWndClassNameLen];
thread_local size_t g_wndClassNameIndex = 0;

wchar_t* NextWndClassNameBuffer() {
    auto& slot = g_wndClassNames[g_wndClassNameIndex++ % kWndClassNameSlots];
    slot[0] = L'\0';
    return slot;
}
} // namespace

// AfxRegisterWndClass - Create and register a window class with given style/cursor/brush/icon
// Symbol: ?AfxRegisterWndClass@@YAPEB_WIPEAUHICON__@@PEAUHBRUSH__@@0@Z
// Ordinal: 2316
// NOTE: The returned pointer is valid for a small number of subsequent calls on the
// same thread (thread-local ring buffer). Callers that cache class names should copy.
extern "C" const wchar_t* MS_ABI stub__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = ::GetModuleHandle(nullptr);
    }

    // Generate a unique class name based on parameters
    // This mimics MFC's approach of creating class names like "Afx:00400000:b:XXXX:YYYY"
    wchar_t* className = NextWndClassNameBuffer();
    int written = _snwprintf(className, kWndClassNameLen, L"OpenMfc:%p:%x:%p:%p:%p",
             (void*)hInst, nClassStyle, (void*)hCursor, (void*)hbrBackground, (void*)hIcon);
    if (written < 0 || written >= (int)kWndClassNameLen) {
        className[kWndClassNameLen - 1] = L'\0';
    }

    // Check if already registered
    WNDCLASSW existingClass;
    if (::GetClassInfoW(hInst, className, &existingClass)) {
        return className;  // Already registered
    }

    // Fill in the WNDCLASS structure
    WNDCLASSW wndClass = {};
    wndClass.style = nClassStyle;
    wndClass.lpfnWndProc = ::DefWindowProcW;  // Default window proc
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInst;
    wndClass.hIcon = hIcon ? hIcon : ::LoadIconW(nullptr, IDI_APPLICATION);
    wndClass.hCursor = hCursor ? hCursor : ::LoadCursorW(nullptr, IDC_ARROW);
    wndClass.hbrBackground = hbrBackground;
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = className;

    // Register the class
    ATOM atom = ::RegisterClassW(&wndClass);
    if (atom == 0) {
        DWORD err = ::GetLastError();
        if (err == ERROR_CLASS_ALREADY_EXISTS) {
            return className;  // That's fine
        }
        return nullptr;  // Registration failed
    }

    // Track the registered class
    if (g_numRegisteredClasses < 256) {
        g_registeredClasses[g_numRegisteredClasses++] = atom;
    }

    return className;
}

// =============================================================================
// AfxUnregisterClass (internal cleanup)
// =============================================================================

// Not exported, but useful for internal cleanup
void AfxUnregisterAllClasses() {
    HINSTANCE hInst = AfxGetInstanceHandle();
    for (int i = 0; i < g_numRegisteredClasses; i++) {
        ::UnregisterClassW(MAKEINTRESOURCEW(g_registeredClasses[i]), hInst);
    }
    g_numRegisteredClasses = 0;
}

// =============================================================================
// Additional Window Registration Helpers
// =============================================================================

// AfxRegisterClass (Ex version) - Extended register
// Symbol: ?AfxRegisterClass@@YAHPEAUtagWNDCLASSEXW@@@Z
extern "C" int MS_ABI stub__AfxRegisterClass__YAHPEAUtagWNDCLASSEXW___Z(WNDCLASSEXW* lpWndClass) {
    if (!lpWndClass) return FALSE;

    // Set default instance handle if not specified
    if (!lpWndClass->hInstance) {
        lpWndClass->hInstance = AfxGetInstanceHandle();
    }

    // Ensure size is set
    if (lpWndClass->cbSize == 0) {
        lpWndClass->cbSize = sizeof(WNDCLASSEXW);
    }

    // Register the class
    ATOM atom = ::RegisterClassExW(lpWndClass);
    if (atom == 0) {
        DWORD err = ::GetLastError();
        if (err == ERROR_CLASS_ALREADY_EXISTS) {
            return TRUE;
        }
        return FALSE;
    }

    if (g_numRegisteredClasses < 256) {
        g_registeredClasses[g_numRegisteredClasses++] = atom;
    }

    return TRUE;
}

// =============================================================================
// AfxGetInstanceHandle (if not defined elsewhere)
// =============================================================================

// AfxGetInstanceHandle - Get the application instance handle
// Symbol: ?AfxGetInstanceHandle@@YAPEAUHINSTANCE__@@XZ
extern "C" HINSTANCE MS_ABI stub__AfxGetInstanceHandle__YAPEAUHINSTANCE____XZ() {
    return AfxGetInstanceHandle();
}

// AfxGetResourceHandle - Get the resource module handle
// Symbol: ?AfxGetResourceHandle@@YAPEAUHINSTANCE__@@XZ
extern "C" HINSTANCE MS_ABI stub__AfxGetResourceHandle__YAPEAUHINSTANCE____XZ() {
    return AfxGetResourceHandle();
}

// AfxSetResourceHandle - Set the resource module handle
// Symbol: ?AfxSetResourceHandle@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI stub__AfxSetResourceHandle__YAXPEAUHINSTANCE____Z(HINSTANCE hInstResource) {
    AfxSetResourceHandle(hInstResource);
}

// =============================================================================
// AfxGetMainWnd
// =============================================================================

// AfxGetMainWnd - Get the main window
// Symbol: ?AfxGetMainWnd@@YAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI stub__AfxGetMainWnd__YAPEAVCWnd__XZ() {
    return AfxGetMainWnd();
}

// AfxGetApp - Get the application object
// Symbol: ?AfxGetApp@@YAPEAVCWinApp@@XZ
extern "C" CWinApp* MS_ABI stub__AfxGetApp__YAPEAVCWinApp__XZ() {
    return AfxGetApp();
}

// =============================================================================
// AfxGetModuleState / AfxSetModuleState (Simplified stubs)
// =============================================================================

// These are used for DLL state management in real MFC
// We provide minimal stubs for link compatibility

struct AFX_MODULE_STATE {
    // Simplified - real MFC has many members
    HINSTANCE m_hCurrentInstanceHandle;
    HINSTANCE m_hCurrentResourceHandle;
    CWinApp* m_pCurrentWinApp;
};

static AFX_MODULE_STATE g_moduleState;

// AfxGetModuleState
// Symbol: ?AfxGetModuleState@@YAPEAUAFX_MODULE_STATE@@XZ
extern "C" AFX_MODULE_STATE* MS_ABI stub__AfxGetModuleState__YAPEAUAFX_MODULE_STATE__XZ() {
    // Populate from global state
    g_moduleState.m_hCurrentInstanceHandle = AfxGetInstanceHandle();
    g_moduleState.m_hCurrentResourceHandle = AfxGetResourceHandle();
    g_moduleState.m_pCurrentWinApp = AfxGetApp();
    return &g_moduleState;
}

// AfxGetStaticModuleState
// Symbol: ?AfxGetStaticModuleState@@YAPEAUAFX_MODULE_STATE@@XZ
extern "C" AFX_MODULE_STATE* MS_ABI stub__AfxGetStaticModuleState__YAPEAUAFX_MODULE_STATE__XZ() {
    return stub__AfxGetModuleState__YAPEAUAFX_MODULE_STATE__XZ();
}

// =============================================================================
// Module Thread State
// =============================================================================

struct AFX_MODULE_THREAD_STATE {
    // Simplified
    int m_nTempMapLock;
};

static AFX_MODULE_THREAD_STATE g_moduleThreadState;

// AfxGetModuleThreadState
// Symbol: ?AfxGetModuleThreadState@@YAPEAUAFX_MODULE_THREAD_STATE@@XZ
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI stub__AfxGetModuleThreadState__YAPEAUAFX_MODULE_THREAD_STATE__XZ() {
    return &g_moduleThreadState;
}
