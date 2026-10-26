// Splitter-owned scrollbar lookup (release x64 CView slot 32).
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxole.h"

CScrollBar* CView::GetScrollBarCtrl(int axis) const {
    if (GetStyle() & (axis == SB_HORZ ? WS_HSCROLL : WS_VSCROLL)) return nullptr;
    CSplitterWnd* splitter = GetParentSplitter(this, TRUE);
    if (!splitter) return nullptr;
    UINT id = static_cast<UINT>(::GetDlgCtrlID(m_hWnd));
    if (id - 0xe900u > 0xffu) return nullptr;
    UINT scrollId = axis == SB_HORZ ? 0xea00u + (id & 15u) : 0xea10u + ((id - 0xe900u) >> 4);
    return static_cast<CScrollBar*>(splitter->GetDlgItem(static_cast<int>(scrollId)));
}
// Symbol: ?GetScrollBarCtrl@CView@@UEBAPEAVCScrollBar@@H@Z
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(const CView* self, int axis) { return self->CView::GetScrollBarCtrl(axis); }
