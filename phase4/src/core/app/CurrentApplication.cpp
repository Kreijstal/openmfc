// Native current-thread and module-scoped application handles.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
CWinThread* AFXAPI AfxGetThread() {
    return impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
}
HINSTANCE AFXAPI AfxGetInstanceHandle() {
    return impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_hCurrentInstanceHandle;
}
HINSTANCE AFXAPI AfxGetResourceHandle() {
    return impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_hCurrentResourceHandle;
}
void AFXAPI AfxSetResourceHandle(HINSTANCE resource) {
    impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_hCurrentResourceHandle=resource;
}
CWnd* AFXAPI AfxGetMainWnd() {
    auto* thread=AfxGetThread();
    return thread ? thread->GetMainWnd() : nullptr;
}
// Symbol: ?AfxGetThread@@YAPEAVCWinThread@@XZ
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ() {return AfxGetThread();}
