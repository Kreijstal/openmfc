// Native module lookup preserves module-local application and handle state.
#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"

// Symbol: ?AfxGetModuleState@@YAPEAVAFX_MODULE_STATE@@XZ
// Ordinal: 2212
// Native lookup preserves the selected module's registered application.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    return ResolveCurrentModuleState();
}
// Native module switching lives with the thread state in _AFX_THREAD_STATE.cpp.
// Symbol: ?AfxGetStaticModuleState@@YAPEAUAFX_MODULE_STATE@@XZ
// Ordinal: 2211
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetStaticModuleState__YAPEAUAFX_MODULE_STATE__XZ() {
    return &g_moduleStateStatic;
}
