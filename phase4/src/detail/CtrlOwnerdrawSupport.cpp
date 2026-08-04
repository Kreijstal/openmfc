#include "CtrlOwnerdrawSupport.h"

namespace openmfc { namespace detail { namespace ctrlownerdraw {
void FillOwnerDrawItem(DRAWITEMSTRUCT* pDraw) {
    if (!pDraw || !pDraw->hDC) return;
    HBRUSH brush = ::GetSysColorBrush((pDraw->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW);
    ::FillRect(pDraw->hDC, &pDraw->rcItem, brush);
}
int ListBoxItemFromPoint_CtrlOwnerdraw(HWND hWnd, CPoint pt, int* pOutside) {
    if (pOutside) *pOutside = TRUE;
    if (!hWnd) return LB_ERR;
    DWORD hit = (DWORD)::SendMessageW(hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
    if (pOutside) *pOutside = HIWORD(hit) ? TRUE : FALSE;
    return (int)LOWORD(hit);
}
HWND TreeCtrlHwnd(const CTreeCtrl* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}
BOOL TreeCtrlGetItem(HWND hWnd, HTREEITEM hItem, UINT mask, TVITEMW* pItem) {
    if (!hWnd || !hItem || !pItem) return FALSE;
    *pItem = {};
    pItem->mask = mask;
    pItem->hItem = hItem;
    return (BOOL)::SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)pItem);
}
BOOL TreeCtrlGetItemEx(HWND hWnd, HTREEITEM hItem, UINT mask, TVITEMEXW* pItem) {
    if (!hWnd || !hItem || !pItem) return FALSE;
    *pItem = {};
    pItem->mask = mask;
    pItem->hItem = hItem;
    return (BOOL)::SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)pItem);
}
} } }  // namespace openmfc::detail::ctrlownerdraw
