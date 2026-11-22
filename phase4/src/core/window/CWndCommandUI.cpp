#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"

extern "C" int AFXAPI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget*, UINT, int, void*, void*);

namespace {
constexpr int reflectedUpdate = static_cast<int>(MAKELONG(CN_UPDATE_COMMAND_UI, WM_COMMAND + WM_REFLECT_BASE));

struct BorrowedWindowHandle {
    HWND& handle;
    ~BorrowedWindowHandle() { handle = nullptr; }
};

bool CanDisableWithoutHandler(HWND child) {
    if (!(::SendMessageW(child, WM_GETDLGCODE, 0, 0) & DLGC_BUTTON)) return false;
    switch (::GetWindowLongW(child, GWL_STYLE) & BS_TYPEMASK) {
    case BS_AUTOCHECKBOX:
    case BS_AUTO3STATE:
    case BS_GROUPBOX:
    case BS_AUTORADIOBUTTON:
        return false;
    default:
        return true;
    }
}
}

void CWnd::UpdateDialogControls(CCmdTarget* target, BOOL disableIfNoHandler) {
    if (!m_hWnd) return;

    CCmdUI command;
    CWnd control;
    const BorrowedWindowHandle borrowed{control.m_hWnd};
    for (HWND child = ::GetTopWindow(m_hWnd); child; child = ::GetWindow(child, GW_HWNDNEXT)) {
        control.m_hWnd = child;
        command.m_nID = static_cast<UINT>(::GetDlgCtrlID(child));
        command.m_pOther = &control;

        // The child and dialog handle local updates through their message maps.
        // The owner target then gets the usual virtual command-routing path.
        auto* wrapper = static_cast<CWnd*>(CWnd::FromHandlePermanent(child));
        if (wrapper && impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
                           wrapper, 0, reflectedUpdate, &command, nullptr)) {
            continue;
        }
        if (impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
                this, command.m_nID, CN_UPDATE_COMMAND_UI, &command, nullptr)) {
            continue;
        }
        command.DoUpdate(target, disableIfNoHandler && CanDisableWithoutHandler(child));
    }
}

// Symbol: ?UpdateDialogControls@CWnd@@QEAAXPEAVCCmdTarget@@H@Z
extern "C" void AFXAPI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
    CWnd* window, CCmdTarget* target, BOOL disableIfNoHandler) {
    window->UpdateDialogControls(target, disableIfNoHandler);
}
