// Release x64 window-procedure exception handling and virtual thread routing.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"

// Symbol: ?AfxInternalProcessWndProcException@@YA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" LRESULT MS_ABI impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(
    CException*, const MSG* message) {
    if (message->message == WM_CREATE)
        return -1;
    if (message->message == WM_PAINT)
        ::ValidateRect(message->hwnd, nullptr);
    return 0;
}

// Symbol: ?ProcessWndProcException@CWinThread@@UEAA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" LRESULT MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(
    CWinThread*, CException* exception, const MSG* message) {
    return impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(exception, message);
}

// Symbol: ?AfxProcessWndProcException@@YA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" LRESULT MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(
    CException* exception, const MSG* message) {
    auto* thread = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    return thread ? thread->ProcessWndProcException(exception, message) :
        impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(exception, message);
}
