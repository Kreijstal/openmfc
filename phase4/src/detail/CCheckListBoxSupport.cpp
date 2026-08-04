#include "CCheckListBoxSupport.h"

namespace openmfc { namespace detail { namespace cchecklistbox {
std::unordered_map<const CCheckListBox*, ItemStateMap> g_checkStates;
std::unordered_map<const CCheckListBox*, ItemStateMap> g_itemEnabledStates;
std::unordered_map<const CCheckListBox*, unsigned int> g_checkStyles;
int ListBoxItemFromPoint_CtrlChecklist(HWND hWnd, CPoint pt) {
    if (!hWnd) return LB_ERR;
    DWORD hit = (DWORD)::SendMessageW(hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
    if (HIWORD(hit) != 0) return LB_ERR;
    return (int)LOWORD(hit);
}
CRect CheckListBoxCheckRect(const RECT& itemRect) {
    int itemHeight = itemRect.bottom - itemRect.top;
    int box = ::GetSystemMetrics(SM_CXMENUCHECK);
    if (box <= 0) box = 13;
    if (itemHeight > 0 && box > itemHeight) box = itemHeight;

    CRect checkRect;
    checkRect.left = itemRect.left + 2;
    checkRect.top = itemRect.top + ((itemHeight - box) / 2);
    checkRect.right = checkRect.left + box;
    checkRect.bottom = checkRect.top + box;
    return checkRect;
}
int CheckListBoxMinimumItemHeight(HWND hWnd) {
    int height = ::GetSystemMetrics(SM_CYMENUCHECK);
    if (height <= 0) height = 13;
    if (hWnd) {
        LRESULT itemHeight = ::SendMessageW(hWnd, LB_GETITEMHEIGHT, 0, 0);
        if (itemHeight > height && itemHeight < 0x10000) height = (int)itemHeight;
    }
    return height + 2;
}
int CheckListBoxVariantIndex(const VARIANT& varChild) {
    if (varChild.vt == VT_I4 || varChild.vt == VT_INT) return (int)varChild.lVal;
    if (varChild.vt == VT_EMPTY) return (int)CHILDID_SELF;
    return -1;
}
LRESULT CheckListBoxSend(CCheckListBox* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    return (pThis && pThis->m_hWnd) ? ::SendMessageW(pThis->m_hWnd, message, wParam, lParam) : LB_ERR;
}
} } }  // namespace openmfc::detail::cchecklistbox
