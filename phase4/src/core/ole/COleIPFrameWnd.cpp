// COleIPFrameWnd — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);

// Symbol: ?OnUpdateControlBarMenu@COleIPFrameWnd@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateControlBarMenu_COleIPFrameWnd__IEAAXPEAVCCmdUI___Z(COleIPFrameWnd*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
COleIPFrameWnd::COleIPFrameWnd()
    : CFrameWnd(), m_pResizeBar(nullptr) {
    memset(_oleipframewnd_padding, 0, sizeof(_oleipframewnd_padding));
}
COleIPFrameWnd::~COleIPFrameWnd() {
}
BOOL COleIPFrameWnd::OnCreateAggregates() {
    return TRUE;
}
BOOL COleIPFrameWnd::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc) {
    return TRUE;
}
void COleIPFrameWnd::OnRequestPositionChange(LPCRECT lpRect) {
    if (!m_hWnd || !lpRect) return;

    const int width = lpRect->right - lpRect->left;
    const int height = lpRect->bottom - lpRect->top;
    ::SetWindowPos(m_hWnd, nullptr, lpRect->left, lpRect->top,
                   width > 0 ? width : 0, height > 0 ? height : 0,
                   SWP_NOZORDER | SWP_NOACTIVATE);

    if (m_pResizeBar && m_pResizeBar->GetSafeHwnd()) {
        m_pResizeBar->MoveWindow(lpRect->left, lpRect->top,
                                 width > 0 ? width : 0, height > 0 ? height : 0, TRUE);
    }
}
