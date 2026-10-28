// Native release x64 CWinThread idle loop and pumped-message storage.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"

extern "C" BOOL MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(MSG*);

BOOL CWinThread::InitInstance() { return FALSE; }
int CWinThread::ExitInstance() {
    return static_cast<int>(AfxGetThreadState()->m_msgCur.wParam);
}
int CWinThread::Run() {
    BOOL idle = TRUE;
    // Unsigned storage preserves the native DWORD wrap at LONG_MAX.
    DWORD count = 0;
    MSG* message = &AfxGetThreadState()->m_msgCur;
    for (;;) {
        while (idle && !::PeekMessageW(message,nullptr,0,0,PM_NOREMOVE)) {
            BOOL more = OnIdle(static_cast<LONG>(count++));
            idle = idle && more;
        }
        do {
            if (!PumpMessage()) return ExitInstance();
            if (IsIdleMessage(message)) {
                idle = TRUE;
                count = 0;
            }
        } while (::PeekMessageW(message,nullptr,0,0,PM_NOREMOVE));
    }
}
// Symbol: ?InitInstance@CWinThread@@UEAAHXZ
extern "C" BOOL MS_ABI impl__InitInstance_CWinThread__UEAAHXZ(CWinThread* self) {
    return self->CWinThread::InitInstance();
}
// Symbol: ?ExitInstance@CWinThread@@UEAAHXZ
extern "C" int MS_ABI impl__ExitInstance_CWinThread__UEAAHXZ(CWinThread* self) {
    return self->CWinThread::ExitInstance();
}
// Symbol: ?Run@CWinThread@@UEAAHXZ
extern "C" int MS_ABI impl__Run_CWinThread__UEAAHXZ(CWinThread* self) {
    return self->CWinThread::Run();
}
// Symbol: ?AfxInternalPumpMessage@@YAHXZ
extern "C" BOOL MS_ABI impl__AfxInternalPumpMessage__YAHXZ() {
    auto* state = AfxGetThreadState();
    MSG* message = &state->m_msgCur;
    if (!::GetMessageW(message,nullptr,0,0)) return FALSE;
    // MFC's private WM_KICKIDLE is consumed without translation/dispatch.
    if (message->message != 0x036a && !impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(message)) {
        ::TranslateMessage(message);
        ::DispatchMessageW(message);
    }
    return TRUE;
}
BOOL CWinThread::PumpMessage() { return impl__AfxInternalPumpMessage__YAHXZ(); }
// Symbol: ?PumpMessage@CWinThread@@UEAAHXZ
extern "C" BOOL MS_ABI impl__PumpMessage_CWinThread__UEAAHXZ(CWinThread* self) {
    return self->CWinThread::PumpMessage();
}

// Symbol: ?AfxInternalIsIdleMessage@@YAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z(MSG* message) {
    switch (message->message) {
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE: {
        auto* state = AfxGetThreadState();
        if (state->m_ptCursorLast.x == message->pt.x && state->m_ptCursorLast.y == message->pt.y &&
            state->m_nMsgLast == message->message) return FALSE;
        state->m_ptCursorLast.x = message->pt.x;
        state->m_ptCursorLast.y = message->pt.y;
        state->m_nMsgLast = message->message;
        break;
    }
    case WM_PAINT:
    case 0x0118: // WM_SYSTIMER
        return FALSE;
    }
    return TRUE;
}
BOOL CWinThread::IsIdleMessage(MSG* message) {
    return impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z(message);
}
// Symbol: ?IsIdleMessage@CWinThread@@UEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__IsIdleMessage_CWinThread__UEAAHPEAUtagMSG___Z(CWinThread* self,MSG* message) {
    return self->CWinThread::IsIdleMessage(message);
}
// Symbol: ?AfxIsIdleMessage@@YAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z(MSG* message) {
    auto* thread = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    return thread ? thread->IsIdleMessage(message) : impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z(message);
}
