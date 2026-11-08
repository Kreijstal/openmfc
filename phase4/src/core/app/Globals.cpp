// Globals — OpenMFC implementation.
// Sources: appcore.cpp, global_settingsstore_sp.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/RegcoreSupport.h"

// CSettingsStoreSP static runtime-class storage.

#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();



extern "C" CObject* MS_ABI impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ();

// Symbol: ?AfxGetInstanceHandleHelper@@YAPEAUHINSTANCE__@@XZ
HINSTANCE AFXAPI AfxGetInstanceHandleHelper() {
    // Real MFC exposes a helper symbol used by some import libraries.
    // OpenMFC keeps it aligned with AfxGetInstanceHandle state.
    return AfxGetInstanceHandle();
}
// Symbol: ?AfxGetAppModuleState@@YAPEAVAFX_MODULE_STATE@@XZ
AFX_MODULE_STATE* AFXAPI AfxGetAppModuleState() {
    return reinterpret_cast<AFX_MODULE_STATE*>(g_appModuleStateStorage);
}
// AfxWinInit implementation
// Symbol: ?AfxWinInit@@YAHPEAUHINSTANCE__@@0PEA_WH@Z
BOOL AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPWSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;

    auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    module->m_hCurrentInstanceHandle=hInstance;
    module->m_hCurrentResourceHandle=hInstance;
    if(auto* app=module->m_pCurrentWinApp) {
        app->m_hInstance=hInstance;app->m_lpCmdLine=lpCmdLine;app->m_nCmdShow=nCmdShow;
    }

    return TRUE;
}
// Symbol: ?m_pRTIDefault@CSettingsStoreSP@@1PEAUCRuntimeClass@@EA
extern "C" CRuntimeClass* impl__m_pRTIDefault_CSettingsStoreSP__1PEAUCRuntimeClass__EA =
    impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();
