// Globals — OpenMFC implementation.
// Sources: appcore.cpp, global_settingsstore_sp.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"

// CSettingsStoreSP static runtime-class storage.

#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();



extern "C" CObject* MS_ABI impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ();

// AfxGetThread - exported stub for MSVC
// Symbol: ?AfxGetThread@@YAPEAVCWinThread@@XZ
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ() {
    return AfxGetThread();
}
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
// Symbol: ?AfxGetThreadState@@YAPEAV_AFX_THREAD_STATE@@XZ
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() {
    return &g_threadStateStorage;
}
// AfxWinInit implementation
// Symbol: ?AfxWinInit@@YAHPEAUHINSTANCE__@@0PEA_WH@Z
BOOL AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPWSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;

    g_hInstance = hInstance;
    g_hResource = hInstance;

    if (g_pApp != nullptr) {
        g_pApp->m_hInstance = hInstance;
        g_pApp->m_lpCmdLine = lpCmdLine;
        g_pApp->m_nCmdShow = nCmdShow;
    }

    // Set global app pointer if it exists
    // CWinApp constructor sets itself as the global app usually,
    // but we ensure it here if needed.
    if (g_pApp == nullptr && AfxGetApp() != nullptr) {
        g_pApp = AfxGetApp();
    }

    return TRUE;
}
// AfxGetMainWnd implementation
// Symbol: ?AfxGetMainWnd@@YAPEAVCWnd@@XZ
CWnd* AFXAPI AfxGetMainWnd() {
    CWinThread* pThread = AfxGetThread();
    return pThread ? pThread->m_pMainWnd : nullptr;
}
// Symbol: ?m_pRTIDefault@CSettingsStoreSP@@1PEAUCRuntimeClass@@EA
extern "C" CRuntimeClass* impl__m_pRTIDefault_CSettingsStoreSP__1PEAUCRuntimeClass__EA =
    impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ();
