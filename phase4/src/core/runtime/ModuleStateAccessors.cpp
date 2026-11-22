// Native module lookup preserves module-local application and handle state.
#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"

// Symbol: ?AfxGetModuleState@@YAPEAVAFX_MODULE_STATE@@XZ
// Native lookup preserves the selected module's registered application.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    return ResolveCurrentModuleState();
}
// Symbol: ?AfxGetAppModuleState@@YAPEAVAFX_MODULE_STATE@@XZ
AFX_MODULE_STATE* AFXAPI AfxGetAppModuleState() {
    return &g_moduleStateStatic;
}
extern "C" AFX_MODULE_STATE* AFXAPI impl__AfxGetAppModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    return AfxGetAppModuleState();
}
// Native module switching lives with the thread state in _AFX_THREAD_STATE.cpp.
// Internal access to the process-wide fallback state.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetStaticModuleState__YAPEAUAFX_MODULE_STATE__XZ() {
    return &g_moduleStateStatic;
}
