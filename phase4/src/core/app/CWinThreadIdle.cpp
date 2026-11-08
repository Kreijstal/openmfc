// Release x64 idle command updates, delayed frame visibility and map cleanup.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/WindowMessageSupport.h"
#include <cstdint>

extern "C" BOOL MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int);

BOOL CWinThread::OnIdle(LONG count) {
    if (count <= 0) {
        CWnd* main = m_pMainWnd;
        if (main && main->m_hWnd && ::IsWindowVisible(main->m_hWnd)) {
            impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(main, main->m_hWnd, 0x363, 1, 0);
            CWnd::SendMessageToDescendants(main->m_hWnd, 0x363, 1, 0, TRUE, TRUE);
        }
        auto* state = openmfc::detail::modulethreadstate::ForModule(m_pModuleState);
        auto* frame = static_cast<CFrameWnd*>(state->m_frameList.GetHead());
        while (frame) {
            if (frame->m_hWnd && frame != main) {
                auto& data = frame->m_retailMiddle;
                if (data.m_nShowDelay == 0)
                    frame->ShowWindow(SW_HIDE);
                if (::IsWindowVisible(frame->m_hWnd) || data.m_nShowDelay >= 0) {
                    impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(frame, frame->m_hWnd, 0x363, 1, 0);
                    CWnd::SendMessageToDescendants(frame->m_hWnd, 0x363, 1, 0, TRUE, TRUE);
                }
                if (data.m_nShowDelay > 0)
                    frame->ShowWindow(data.m_nShowDelay);
                data.m_nShowDelay = -1;
            }
            frame = frame->m_retailMiddle.m_pNextFrameWnd;
        }
    } else {
        auto* captured = openmfc::detail::modulethreadstate::ForModule(m_pModuleState);
        if (!captured->m_nTempMapLock) {
            auto& lock = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_nTempMapLock;
            lock = static_cast<int>(static_cast<uint32_t>(lock) + 1);
            impl__AfxUnlockTempMaps__YAHH_Z(TRUE);
        }
    }
    return count < 1;
}

// Symbol: ?OnIdle@CWinThread@@UEAAHJ@Z
extern "C" BOOL MS_ABI impl__OnIdle_CWinThread__UEAAHJ_Z(CWinThread* self, LONG count) {
    return self->CWinThread::OnIdle(count);
}

static_assert(sizeof(CFrameWnd) == 0x1d8 && sizeof(CFrameWnd::RetailMiddleStorage) == 0x64 &&
              offsetof(CFrameWnd, m_nIDHelp) == 0x164 && offsetof(CFrameWnd, m_pViewActive) == 0x170 &&
              offsetof(CFrameWnd, m_retailMiddle) +
                  offsetof(CFrameWnd::RetailMiddleStorage, m_pNextFrameWnd) == 0x108 &&
              offsetof(CFrameWnd, m_retailMiddle) +
                  offsetof(CFrameWnd::RetailMiddleStorage, m_nShowDelay) == 0x160,
              "native idle frame links and visibility command");
