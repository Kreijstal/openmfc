#include "openmfc/afxstat_.h"
#include "RegcoreSupport.h"
extern CWinApp* g_pApp;

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
    return ResolveCurrentModuleState()->m_pCurrentWinApp;
}
AFX_MODULE_STATE g_moduleStateStatic;
AFX_MODULE_STATE* ResolveCurrentModuleState() {
    AFX_MODULE_STATE* state=AfxGetThreadState()->m_pModuleState;
    return state ? state : &g_moduleStateStatic;
}
void RefreshStaticModuleState() {
    g_moduleStateStatic.m_hCurrentInstanceHandle = AfxGetInstanceHandle();
    g_moduleStateStatic.m_hCurrentResourceHandle = AfxGetResourceHandle();
    g_moduleStateStatic.m_pCurrentWinApp = g_pApp;
}
} } }  // namespace openmfc::detail::regcore
