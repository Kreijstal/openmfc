#include "CListCtrlSupport.h"

namespace openmfc { namespace detail { namespace clistctrl {
HWND ListCtrlHwnd(const CListCtrl* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}
BOOL ListCtrlGetItem(HWND hWnd, int nItem, UINT mask, LVITEMW* pItem) {
    if (!hWnd || !pItem || nItem < 0) return FALSE;
    *pItem = {};
    pItem->mask = mask;
    pItem->iItem = nItem;
    return (BOOL)::SendMessageW(hWnd, LVM_GETITEMW, 0, (LPARAM)pItem);
}
} } }  // namespace openmfc::detail::clistctrl
