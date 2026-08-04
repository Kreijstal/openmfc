#include "RegcoreSupport.h"

thread_local wchar_t g_wndClassNames[kWndClassNameSlots][kWndClassNameLen];
namespace openmfc { namespace detail { namespace regcore {
ATOM g_registeredClasses[256];
int g_numRegisteredClasses = 0;
thread_local size_t g_wndClassNameIndex = 0;
wchar_t* NextWndClassNameBuffer() {
    auto& slot = g_wndClassNames[g_wndClassNameIndex++ % kWndClassNameSlots];
    slot[0] = L'\0';
    return slot;
}
void AfxUnregisterAllClasses() {
    HINSTANCE hInst = AfxGetInstanceHandle();
    for (int i = 0; i < g_numRegisteredClasses; i++) {
        ::UnregisterClassW(MAKEINTRESOURCEW(g_registeredClasses[i]), hInst);
    }
    g_numRegisteredClasses = 0;
}
extern "C" int MS_ABI impl__AfxRegisterClass__YAHPEAUtagWNDCLASSEXW___Z(WNDCLASSEXW* lpWndClass) {
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
extern "C" HINSTANCE MS_ABI impl__AfxGetInstanceHandle__YAPEAUHINSTANCE____XZ() {
    return AfxGetInstanceHandle();
}
extern "C" HINSTANCE MS_ABI impl__AfxGetResourceHandle__YAPEAUHINSTANCE____XZ() {
    return AfxGetResourceHandle();
}
extern "C" void MS_ABI impl__AfxSetResourceHandle__YAXPEAUHINSTANCE____Z(HINSTANCE hInstResource) {
    AfxSetResourceHandle(hInstResource);
}
// AfxGetMainWnd is implemented in appcore.cpp; this unit had a second, unlinked copy.
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ() {
    return AfxGetApp();
}
AFX_MODULE_STATE g_moduleStateStatic;
thread_local AFX_MODULE_STATE* g_pCurrentModuleState = nullptr;
AFX_MODULE_STATE* ResolveCurrentModuleState() {
    if (!g_pCurrentModuleState) {
        g_pCurrentModuleState = &g_moduleStateStatic;
    }
    return g_pCurrentModuleState;
}
void RefreshStaticModuleState() {
    g_moduleStateStatic.m_hCurrentInstanceHandle = AfxGetInstanceHandle();
    g_moduleStateStatic.m_hCurrentResourceHandle = AfxGetResourceHandle();
    g_moduleStateStatic.m_pCurrentWinApp = AfxGetApp();
}
thread_local AFX_MODULE_THREAD_STATE_Regcore g_moduleThreadState;
} } }  // namespace openmfc::detail::regcore
