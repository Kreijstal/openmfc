// Native thread/window routing before TranslateMessage and DispatchMessage.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
extern "C" {
BOOL MS_ABI impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(CWinThread*,MSG*);
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
CWnd* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd*);
}

// Symbol: ?AfxInternalPreTranslateMessage@@YAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(MSG* message) {
    auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    if(thread && !message->hwnd &&
        impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(thread,message))return TRUE;
    // Thread-message callbacks and module lookups can change the active thread.
    thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    CWnd* main=thread ? thread->GetMainWnd() : nullptr;
    if(CWnd::WalkPreTranslateTree(main ? main->m_hWnd : nullptr,message))return TRUE;
    if(main) {
        auto* target=impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(message->hwnd);
        if(impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(target)!=main)
            return main->PreTranslateMessage(message);
    }
    return FALSE;
}
BOOL CWinThread::PreTranslateMessage(MSG* message) {
    return impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(message);
}
// Symbol: ?PreTranslateMessage@CWinThread@@UEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__PreTranslateMessage_CWinThread__UEAAHPEAUtagMSG___Z(
    CWinThread*,MSG* message) {
    return impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(message);
}
// Symbol: ?AfxPreTranslateMessage@@YAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(MSG* message) {
    auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    return thread ? thread->PreTranslateMessage(message) :
        impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(message);
}
