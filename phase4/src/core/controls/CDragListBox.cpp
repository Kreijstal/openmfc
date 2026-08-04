// CDragListBox — OpenMFC implementation.
// Sources: ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"

// Symbol: ?OnChildNotify@CDragListBox@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CDragListBox__MEAAHI_K_JPEA_J_Z(
    CDragListBox* pThis, UINT p0, ULONGLONG p1, LONGLONG p2, LONGLONG* p3) {
    if (!pThis) {
        return FALSE;
    }
    return impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(
        pThis, p0, static_cast<unsigned __int64>(p1), static_cast<__int64>(p2), p3);
}
// Symbol: ?PreSubclassWindow@CDragListBox@@UEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CDragListBox__UEAAXXZ(CDragListBox* pThis) {
    g_dragSourceItem.erase(pThis);
    impl__PreSubclassWindow_CWnd__UEAAXXZ(pThis);
}
// Symbol: ?BeginDrag@CDragListBox@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__BeginDrag_CDragListBox__UEAAHVCPoint___Z(CDragListBox* pThis, CPoint pt) {
    return pThis ? pThis->BeginDrag(pt) : FALSE;
}
// Symbol: ?CancelDrag@CDragListBox@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__CancelDrag_CDragListBox__UEAAXVCPoint___Z(CDragListBox* pThis, CPoint pt) {
    if (pThis) pThis->CancelDrag(pt);
}
// Symbol: ?Dragging@CDragListBox@@UEAAIVCPoint@@@Z
extern "C" unsigned int MS_ABI impl__Dragging_CDragListBox__UEAAIVCPoint___Z(CDragListBox* pThis, CPoint pt) {
    return pThis ? pThis->Dragging(pt) : (unsigned int)LB_ERR;
}
// Symbol: ?Dropped@CDragListBox@@UEAAXHVCPoint@@@Z
extern "C" void MS_ABI impl__Dropped_CDragListBox__UEAAXHVCPoint___Z(CDragListBox* pThis, int nSrcIndex, CPoint pt) {
    if (pThis) pThis->Dropped(nSrcIndex, pt);
}
// Symbol: ?DrawInsert@CDragListBox@@UEAAXH@Z
extern "C" void MS_ABI impl__DrawInsert_CDragListBox__UEAAXH_Z(CDragListBox* pThis, int nItem) {
    if (pThis) pThis->DrawInsert(nItem);
}
// Symbol: ?DrawSingle@CDragListBox@@QEAAXH@Z
extern "C" void MS_ABI impl__DrawSingle_CDragListBox__QEAAXH_Z(CDragListBox* pThis, int nItem) {
    if (pThis) pThis->DrawSingle(nItem);
}
// Symbol: ??1CDragListBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CDragListBox__UEAA_XZ(CDragListBox* pThis) {
    if (pThis) pThis->~CDragListBox();
}
int CDragListBox::BeginDrag(CPoint pt) {
    int nIndex = ListBoxItemFromPoint_Ctrlcore(m_hWnd, pt);
    if (nIndex == LB_ERR) nIndex = GetCurSel();
    if (nIndex == LB_ERR) return FALSE;
    g_dragSourceItem[this] = nIndex;
    return TRUE;
}
CDragListBox::~CDragListBox() {
    g_dragSourceItem.erase(this);
}
void CDragListBox::CancelDrag(CPoint) {
    g_dragSourceItem.erase(this);
}
unsigned int CDragListBox::Dragging(CPoint pt) {
    return (unsigned int)ListBoxItemFromPoint_Ctrlcore(m_hWnd, pt);
}
void CDragListBox::Dropped(int nSrcIndex, CPoint pt) {
    int nDst = ListBoxItemFromPoint_Ctrlcore(m_hWnd, pt);
    if (nDst == LB_ERR) nDst = nSrcIndex;
    if (nDst != LB_ERR) SetCurSel(nDst);
    g_dragSourceItem.erase(this);
}
void CDragListBox::DrawInsert(int nItem) {
    if (m_hWnd && nItem >= 0) ::SendMessageW(m_hWnd, LB_SETTOPINDEX, nItem, 0);
}
void CDragListBox::DrawSingle(int nItem) {
    if (!m_hWnd || nItem < 0) return;
    RECT rc = {};
    if (::SendMessageW(m_hWnd, LB_GETITEMRECT, nItem, (LPARAM)&rc) != LB_ERR) {
        ::InvalidateRect(m_hWnd, &rc, TRUE);
    }
}
