#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "detail/WindowModalSupport.h"

extern "C" BOOL MS_ABI impl__AfxPumpMessage__YAHXZ();
extern "C" BOOL MS_ABI impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z(MSG*);
extern "C" void MS_ABI impl__AfxPostQuitMessage__YAXH_Z(int);

using namespace openmfc::detail;

int CWnd::RunModalLoop(DWORD flags) {
    constexpr UINT wmSysTimer = 0x0118;
    bool idle = true;
    DWORD idleCount = 0;
    bool showOnIdle = (flags & mlfShowOnIdle) && !(GetStyle() & WS_VISIBLE);
    const HWND parent = ::GetParent(m_hWnd);
    m_nFlags |= wfModalLoop | wfContinueModal;
    MSG* message = &AfxGetThreadState()->m_msgCur;

    for (;;) {
        while (idle && !::PeekMessageW(message, nullptr, 0, 0, PM_NOREMOVE)) {
            if (showOnIdle) {
                ShowWindow(SW_SHOWNORMAL);
                UpdateWindow();
                showOnIdle = false;
            }
            if (!(flags & mlfNoIdleMessage) && parent && idleCount == 0) {
                ::SendMessageW(parent, WM_ENTERIDLE, MSGF_DIALOGBOX, reinterpret_cast<LPARAM>(m_hWnd));
            }
            idle = !(flags & mlfNoKickIdle) &&
                   ::SendMessageW(m_hWnd, wmKickIdle, 0, static_cast<LONG>(idleCount++)) != 0;
        }

        do {
            if (!impl__AfxPumpMessage__YAHXZ()) {
                impl__AfxPostQuitMessage__YAXH_Z(0);
                return -1;
            }
            if (showOnIdle && (message->message == wmSysTimer || message->message == WM_SYSKEYDOWN)) {
                ShowWindow(SW_SHOWNORMAL);
                UpdateWindow();
                showOnIdle = false;
            }
            if (!ContinueModal()) {
                m_nFlags &= ~(wfModalLoop | wfContinueModal);
                return m_nModalResult;
            }
            if (impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z(message)) {
                idle = true;
                idleCount = 0;
            }
        } while (::PeekMessageW(message, nullptr, 0, 0, PM_NOREMOVE));
    }
}

// Symbol: ?RunModalLoop@CWnd@@QEAAHK@Z
extern "C" int MS_ABI impl__RunModalLoop_CWnd__QEAAHK_Z(CWnd* window, DWORD flags) {
    return window->RunModalLoop(flags);
}

// Symbol: ?ContinueModal@CWnd@@UEAAHXZ
extern "C" BOOL MS_ABI impl__ContinueModal_CWnd__UEAAHXZ(CWnd* window) {
    return window ? window->m_nFlags & wfContinueModal : FALSE;
}

// Symbol: ?EndModalLoop@CWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__EndModalLoop_CWnd__UEAAXH_Z(CWnd* window, int result) {
    if (!window) return;
    window->m_nModalResult = result;
    if (window->m_nFlags & wfContinueModal) {
        window->m_nFlags &= ~wfContinueModal;
        ::PostMessageW(window->m_hWnd, WM_NULL, 0, 0);
    }
}
