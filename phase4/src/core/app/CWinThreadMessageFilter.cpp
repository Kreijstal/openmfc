// Release x64 dialog keyboard filtering and help while tracking a frame menu.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include <cstring>
extern "C" {
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd*);
BOOL MS_ABI impl__IsTracking_CFrameWnd__QEAAHXZ(CFrameWnd*);
}
namespace {
CWnd* CurrentMainWindow() {
    auto* thread = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    return thread ? thread->GetMainWnd() : nullptr;
}
}

// Symbol: ?ProcessMessageFilter@CWinThread@@UEAAHHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(
    CWinThread* self, int code, MSG* message) {
    if (!message || (code != MSGF_DIALOGBOX && code != MSGF_MENU))
        return FALSE;
    if (code == MSGF_MENU) {
        auto* target = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(message->hwnd);
        auto* frame = target ? impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(target) : nullptr;
        if (frame && impl__IsTracking_CFrameWnd__QEAAHXZ(frame) && frame->m_retailMiddle.m_bHelpMode) {
            CWnd* main = CurrentMainWindow();
            if (self->m_pMainWnd &&
                ((message->message == WM_KEYDOWN && message->wParam == VK_RETURN) ||
                  message->message == WM_LBUTTONUP)) {
                ::SendMessageW(main->m_hWnd, WM_COMMAND, 0xe146, 0);
                return TRUE;
            }
        }
    }
    // Native consults the current thread even when this lookup's return value
    // is unused. Overrides may change the active window or the message.
    (void)CurrentMainWindow();
    if (code != MSGF_DIALOGBOX || !self->m_pActiveWnd ||
        message->message < WM_KEYFIRST || message->message > WM_KEYLAST)
        return FALSE;
    auto* state = AfxGetThreadState();
    if (state->m_bInMsgFilter)
        return FALSE;
    state->m_bInMsgFilter = TRUE;
    MSG copy;
    std::memcpy(&copy, message, sizeof(copy));
    if (self->m_pActiveWnd->IsWindowEnabled() && self->PreTranslateMessage(&copy)) {
        state->m_bInMsgFilter = FALSE;
        return TRUE;
    }
    state->m_bInMsgFilter = FALSE;
    return FALSE;
}

static_assert(offsetof(_AFX_THREAD_STATE, m_bInMsgFilter) == 0x1a0 && sizeof(MSG) == 48,
              "native filter reentrancy flag and copied message");
