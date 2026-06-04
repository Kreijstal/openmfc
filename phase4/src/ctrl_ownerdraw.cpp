// Standard control owner-draw and runtime-class exported wrappers.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__VKeyToItem_CListBox__UEAAHII_Z(CListBox* pThis, UINT nKey, UINT nIndex);

namespace {
static void FillOwnerDrawItem(DRAWITEMSTRUCT* pDraw) {
    if (!pDraw || !pDraw->hDC) return;
    HBRUSH brush = ::GetSysColorBrush((pDraw->itemState & ODS_SELECTED) ? COLOR_HIGHLIGHT : COLOR_WINDOW);
    ::FillRect(pDraw->hDC, &pDraw->rcItem, brush);
}

static int ListBoxItemFromPoint(HWND hWnd, CPoint pt, int* pOutside) {
    if (pOutside) *pOutside = TRUE;
    if (!hWnd) return LB_ERR;
    DWORD hit = (DWORD)::SendMessageW(hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
    if (pOutside) *pOutside = HIWORD(hit) ? TRUE : FALSE;
    return (int)LOWORD(hit);
}

static HWND TreeCtrlHwnd(const CTreeCtrl* pThis) {
    return pThis ? pThis->m_hWnd : nullptr;
}

static BOOL TreeCtrlGetItem(HWND hWnd, HTREEITEM hItem, UINT mask, TVITEMW* pItem) {
    if (!hWnd || !hItem || !pItem) return FALSE;
    *pItem = {};
    pItem->mask = mask;
    pItem->hItem = hItem;
    return (BOOL)::SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)pItem);
}

static BOOL TreeCtrlGetItemEx(HWND hWnd, HTREEITEM hItem, UINT mask, TVITEMEXW* pItem) {
    if (!hWnd || !hItem || !pItem) return FALSE;
    *pItem = {};
    pItem->mask = mask;
    pItem->hItem = hItem;
    return (BOOL)::SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)pItem);
}
}  // namespace

// Symbol: ?DrawItem@CButton@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CButton__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CButton* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}

// Symbol: ?GetRuntimeClass@CButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CButton__UEBAPEAUCRuntimeClass__XZ(const CButton* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CButton::GetThisClass();
}

// Symbol: ?GetThisClass@CButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CButton__SAPEAUCRuntimeClass__XZ() {
    return CButton::GetThisClass();
}

// Symbol: ?OnChildNotify@CButton@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CButton__MEAAHI_K_JPEA_J_Z(
    CButton* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam;
    if (pResult) *pResult = 0;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CButton__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?CompareItem@CComboBox@@UEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
extern "C" int MS_ABI impl__CompareItem_CComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    CComboBox* pThis, COMPAREITEMSTRUCT* pCompare) {
    (void)pThis;
    (void)pCompare;
    return 0;
}

// Symbol: ?DeleteItem@CComboBox@@UEAAXPEAUtagDELETEITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DeleteItem_CComboBox__UEAAXPEAUtagDELETEITEMSTRUCT___Z(
    CComboBox* pThis, DELETEITEMSTRUCT* pDelete) {
    (void)pThis;
    (void)pDelete;
}

// Symbol: ?DrawItem@CComboBox@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CComboBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CComboBox* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}

// Symbol: ?GetRuntimeClass@CComboBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CComboBox__UEBAPEAUCRuntimeClass__XZ(const CComboBox* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CComboBox::GetThisClass();
}

// Symbol: ?GetThisClass@CComboBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CComboBox__SAPEAUCRuntimeClass__XZ() {
    return CComboBox::GetThisClass();
}

// Symbol: ?MeasureItem@CComboBox@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CComboBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    CComboBox* pThis, MEASUREITEMSTRUCT* pMeasure) {
    (void)pThis;
    if (pMeasure && pMeasure->itemHeight == 0) pMeasure->itemHeight = 16;
}

// Symbol: ?OnChildNotify@CComboBox@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CComboBox__MEAAHI_K_JPEA_J_Z(
    CComboBox* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) *pResult = 0;
    switch (message) {
    case WM_DRAWITEM:
        if (!lParam) return FALSE;
        impl__DrawItem_CComboBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    case WM_MEASUREITEM:
        if (!lParam) return FALSE;
        impl__MeasureItem_CComboBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(pThis, (MEASUREITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    case WM_COMPAREITEM:
        if (!lParam) return FALSE;
        if (pResult) *pResult = impl__CompareItem_CComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(pThis, (COMPAREITEMSTRUCT*)lParam);
        return TRUE;
    case WM_DELETEITEM:
        if (!lParam) return FALSE;
        impl__DeleteItem_CComboBox__UEAAXPEAUtagDELETEITEMSTRUCT___Z(pThis, (DELETEITEMSTRUCT*)lParam);
        return TRUE;
    default:
        (void)wParam;
        return FALSE;
    }
}

// Symbol: ?CharToItem@CListBox@@UEAAHII@Z
extern "C" int MS_ABI impl__CharToItem_CListBox__UEAAHII_Z(CListBox* pThis, UINT nChar, UINT nIndex) {
    (void)pThis;
    (void)nChar;
    (void)nIndex;
    return -1;
}

// Symbol: ?CompareItem@CListBox@@UEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
extern "C" int MS_ABI impl__CompareItem_CListBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    CListBox* pThis, COMPAREITEMSTRUCT* pCompare) {
    (void)pThis;
    (void)pCompare;
    return 0;
}

// Symbol: ?DeleteItem@CListBox@@UEAAXPEAUtagDELETEITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DeleteItem_CListBox__UEAAXPEAUtagDELETEITEMSTRUCT___Z(
    CListBox* pThis, DELETEITEMSTRUCT* pDelete) {
    (void)pThis;
    (void)pDelete;
}

// Symbol: ?DrawItem@CListBox@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CListBox* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}

// Symbol: ?GetRuntimeClass@CListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CListBox__UEBAPEAUCRuntimeClass__XZ(const CListBox* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CListBox::GetThisClass();
}

// Symbol: ?GetThisClass@CListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CListBox__SAPEAUCRuntimeClass__XZ() {
    return CListBox::GetThisClass();
}

// Symbol: ?ItemFromPoint@CListBox@@QEBAIVCPoint@@AEAH@Z
extern "C" UINT MS_ABI impl__ItemFromPoint_CListBox__QEBAIVCPoint__AEAH_Z(
    const CListBox* pThis, CPoint pt, int* pOutside) {
    return (UINT)ListBoxItemFromPoint(pThis ? pThis->m_hWnd : nullptr, pt, pOutside);
}

// Symbol: ?MeasureItem@CListBox@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    CListBox* pThis, MEASUREITEMSTRUCT* pMeasure) {
    (void)pThis;
    if (pMeasure && pMeasure->itemHeight == 0) pMeasure->itemHeight = 16;
}

// Symbol: ?OnChildNotify@CListBox@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CListBox__MEAAHI_K_JPEA_J_Z(
    CListBox* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pResult) *pResult = 0;
    switch (message) {
    case WM_DRAWITEM:
        if (!lParam) return FALSE;
        impl__DrawItem_CListBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    case WM_MEASUREITEM:
        if (!lParam) return FALSE;
        impl__MeasureItem_CListBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(pThis, (MEASUREITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    case WM_COMPAREITEM:
        if (!lParam) return FALSE;
        if (pResult) *pResult = impl__CompareItem_CListBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(pThis, (COMPAREITEMSTRUCT*)lParam);
        return TRUE;
    case WM_DELETEITEM:
        if (!lParam) return FALSE;
        impl__DeleteItem_CListBox__UEAAXPEAUtagDELETEITEMSTRUCT___Z(pThis, (DELETEITEMSTRUCT*)lParam);
        return TRUE;
    case WM_CHARTOITEM:
        if (pResult) *pResult = impl__CharToItem_CListBox__UEAAHII_Z(pThis, (UINT)LOWORD(wParam), (UINT)HIWORD(wParam));
        return TRUE;
    case WM_VKEYTOITEM:
        if (pResult) *pResult = impl__VKeyToItem_CListBox__UEAAHII_Z(pThis, (UINT)LOWORD(wParam), (UINT)HIWORD(wParam));
        return TRUE;
    default:
        return FALSE;
    }
}

// Symbol: ?VKeyToItem@CListBox@@UEAAHII@Z
extern "C" int MS_ABI impl__VKeyToItem_CListBox__UEAAHII_Z(CListBox* pThis, UINT nKey, UINT nIndex) {
    (void)pThis;
    (void)nKey;
    (void)nIndex;
    return -1;
}

// Symbol: ?DrawItem@CStatic@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CStatic__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CStatic* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}

// Symbol: ?GetRuntimeClass@CProgressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CProgressCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CProgressCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CProgressCtrl::GetThisClass();
}

// Symbol: ?GetThisClass@CProgressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CProgressCtrl__SAPEAUCRuntimeClass__XZ() {
    return CProgressCtrl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CScrollBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CScrollBar__UEBAPEAUCRuntimeClass__XZ(
    const CScrollBar* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CScrollBar::GetThisClass();
}

// Symbol: ?GetThisClass@CScrollBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CScrollBar__SAPEAUCRuntimeClass__XZ() {
    return CScrollBar::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CSliderCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSliderCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CSliderCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CSliderCtrl::GetThisClass();
}

// Symbol: ?GetThisClass@CSliderCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSliderCtrl__SAPEAUCRuntimeClass__XZ() {
    return CSliderCtrl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CSpinButtonCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSpinButtonCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CSpinButtonCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CSpinButtonCtrl::GetThisClass();
}

// Symbol: ?GetThisClass@CSpinButtonCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSpinButtonCtrl__SAPEAUCRuntimeClass__XZ() {
    return CSpinButtonCtrl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CStatic@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStatic__UEBAPEAUCRuntimeClass__XZ(
    const CStatic* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CStatic::GetThisClass();
}

// Symbol: ?GetThisClass@CStatic@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatic__SAPEAUCRuntimeClass__XZ() {
    return CStatic::GetThisClass();
}

// Symbol: ?OnChildNotify@CStatic@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CStatic__MEAAHI_K_JPEA_J_Z(
    CStatic* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam;
    if (pResult) *pResult = 0;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CStatic__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?DrawItem@CTabCtrl@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CTabCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    CTabCtrl* pThis, DRAWITEMSTRUCT* pDraw) {
    (void)pThis;
    FillOwnerDrawItem(pDraw);
}

// Symbol: ?GetItemState@CTabCtrl@@QEBAKHK@Z
extern "C" DWORD MS_ABI impl__GetItemState_CTabCtrl__QEBAKHK_Z(
    const CTabCtrl* pThis, int nItem, DWORD dwMask) {
    if (!pThis || !pThis->m_hWnd) return 0;
    TCITEMW item = {};
    item.mask = TCIF_STATE;
    item.dwStateMask = dwMask;
    return ::SendMessageW(pThis->m_hWnd, TCM_GETITEMW, nItem, (LPARAM)&item) ? item.dwState : 0;
}

// Symbol: ?GetMessageMap@CTabCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTabCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const CTabCtrl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetRuntimeClass@CTabCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTabCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CTabCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CTabCtrl::GetThisClass();
}

// Symbol: ?GetThisClass@CTabCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTabCtrl__SAPEAUCRuntimeClass__XZ() {
    return CTabCtrl::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CTabCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTabCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?InsertItem@CTabCtrl@@QEAAJIHPEB_WH_J@Z
extern "C" __int64 MS_ABI impl__InsertItem_CTabCtrl__QEAAJIHPEB_WH_J_Z(
    CTabCtrl* pThis, int nItem, int nMask, const wchar_t* lpszItem, int nImage, LPARAM lParam) {
    if (!pThis || !pThis->m_hWnd) return -1;
    TCITEMW item = {};
    item.mask = (UINT)nMask;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.lParam = lParam;
    return (__int64)::SendMessageW(pThis->m_hWnd, TCM_INSERTITEMW, nItem, (LPARAM)&item);
}

// Symbol: ?InsertItem@CTabCtrl@@QEAAJIHPEB_WH_JKK@Z
extern "C" __int64 MS_ABI impl__InsertItem_CTabCtrl__QEAAJIHPEB_WH_JKK_Z(
    CTabCtrl* pThis,
    int nItem,
    int nMask,
    const wchar_t* lpszItem,
    int nImage,
    LPARAM lParam,
    DWORD dwState,
    DWORD dwStateMask) {
    if (!pThis || !pThis->m_hWnd) return -1;
    TCITEMW item = {};
    item.mask = (UINT)nMask;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.lParam = lParam;
    item.dwState = dwState;
    item.dwStateMask = dwStateMask;
    return (__int64)::SendMessageW(pThis->m_hWnd, TCM_INSERTITEMW, nItem, (LPARAM)&item);
}

// Symbol: ?OnChildNotify@CTabCtrl@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CTabCtrl__MEAAHI_K_JPEA_J_Z(
    CTabCtrl* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    (void)wParam;
    if (pResult) *pResult = 0;
    if (message == WM_DRAWITEM && lParam) {
        impl__DrawItem_CTabCtrl__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, (DRAWITEMSTRUCT*)lParam);
        if (pResult) *pResult = TRUE;
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?OnDestroy@CTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTabCtrl__IEAAXXZ(CTabCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?SetItemState@CTabCtrl@@QEAAHHKK@Z
extern "C" int MS_ABI impl__SetItemState_CTabCtrl__QEAAHHKK_Z(
    CTabCtrl* pThis, int nItem, DWORD dwMask, DWORD dwState) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    TCITEMW item = {};
    item.mask = TCIF_STATE;
    item.dwStateMask = dwMask;
    item.dwState = dwState;
    return (int)::SendMessageW(pThis->m_hWnd, TCM_SETITEMW, nItem, (LPARAM)&item);
}

// Symbol: ?CreateDragImage@CTreeCtrl@@QEAAPEAVCImageList@@PEAU_TREEITEM@@@Z
extern "C" CImageList* MS_ABI impl__CreateDragImage_CTreeCtrl__QEAAPEAVCImageList__PEAU_TREEITEM___Z(
    CTreeCtrl* pThis, HTREEITEM hItem) {
    HIMAGELIST hImage = (HIMAGELIST)::SendMessageW(TreeCtrlHwnd(pThis), TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem);
    if (!hImage) return nullptr;
    CImageList* pImageList = new CImageList();
    pImageList->Attach(hImage);
    return pImageList;
}

// Symbol: ?CreateEx@CTreeCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CTreeCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CTreeCtrl* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::CreateEx(dwExStyle, L"SysTreeView32", L"", dwStyle, pRect->left, pRect->top,
        pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(uintptr_t)nID, nullptr);
}

// Symbol: ?GetCheck@CTreeCtrl@@QEBAHPEAU_TREEITEM@@@Z
extern "C" int MS_ABI impl__GetCheck_CTreeCtrl__QEBAHPEAU_TREEITEM___Z(const CTreeCtrl* pThis, HTREEITEM hItem) {
    UINT state = (UINT)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMSTATE, (WPARAM)hItem, TVIS_STATEIMAGEMASK);
    return ((state >> 12) - 1) > 0 ? TRUE : FALSE;
}

// Symbol: ?GetItemData@CTreeCtrl@@QEBA_KPEAU_TREEITEM@@@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CTreeCtrl__QEBA_KPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMW item = {};
    return TreeCtrlGetItem(TreeCtrlHwnd(pThis), hItem, TVIF_PARAM, &item) ? (uintptr_t)item.lParam : 0;
}

// Symbol: ?GetItemExpandedImageIndex@CTreeCtrl@@QEBAHPEAU_TREEITEM@@@Z
extern "C" int MS_ABI impl__GetItemExpandedImageIndex_CTreeCtrl__QEBAHPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMEXW item = {};
    return TreeCtrlGetItemEx(TreeCtrlHwnd(pThis), hItem, TVIF_EXPANDEDIMAGE, &item) ? item.iExpandedImage : -1;
}

// Symbol: ?GetItemImage@CTreeCtrl@@QEBAHPEAU_TREEITEM@@AEAH1@Z
extern "C" int MS_ABI impl__GetItemImage_CTreeCtrl__QEBAHPEAU_TREEITEM__AEAH1_Z(
    const CTreeCtrl* pThis, HTREEITEM hItem, int* pImage, int* pSelectedImage) {
    if (pImage) *pImage = 0;
    if (pSelectedImage) *pSelectedImage = 0;
    TVITEMW item = {};
    if (!TreeCtrlGetItem(TreeCtrlHwnd(pThis), hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE, &item)) return FALSE;
    if (pImage) *pImage = item.iImage;
    if (pSelectedImage) *pSelectedImage = item.iSelectedImage;
    return TRUE;
}

// Symbol: ?GetItemPartRect@CTreeCtrl@@QEAAHPEAU_TREEITEM@@W4_TVITEMPART@@PEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetItemPartRect_CTreeCtrl__QEAAHPEAU_TREEITEM__W4_TVITEMPART__PEAUtagRECT___Z(
    CTreeCtrl* pThis, HTREEITEM hItem, TVITEMPART partID, RECT* pRect) {
    if (!pRect) return FALSE;
    TVGETITEMPARTRECTINFO info = {};
    info.hti = hItem;
    info.prc = pRect;
    info.partID = partID;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMPARTRECT, 0, (LPARAM)&info);
}

// Symbol: ?GetItemRect@CTreeCtrl@@QEBAHPEAU_TREEITEM@@PEAUtagRECT@@H@Z
extern "C" int MS_ABI impl__GetItemRect_CTreeCtrl__QEBAHPEAU_TREEITEM__PEAUtagRECT__H_Z(
    const CTreeCtrl* pThis, HTREEITEM hItem, RECT* pRect, int bTextOnly) {
    if (!pRect) return FALSE;
    *(HTREEITEM*)pRect = hItem;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMRECT, bTextOnly ? TRUE : FALSE, (LPARAM)pRect);
}

// Symbol: ?GetItemState@CTreeCtrl@@QEBAIPEAU_TREEITEM@@I@Z
extern "C" UINT MS_ABI impl__GetItemState_CTreeCtrl__QEBAIPEAU_TREEITEM__I_Z(
    const CTreeCtrl* pThis, HTREEITEM hItem, UINT nStateMask) {
    return (UINT)::SendMessageW(TreeCtrlHwnd(pThis), TVM_GETITEMSTATE, (WPARAM)hItem, nStateMask);
}

// Symbol: ?GetItemStateEx@CTreeCtrl@@QEBAIPEAU_TREEITEM@@@Z
extern "C" UINT MS_ABI impl__GetItemStateEx_CTreeCtrl__QEBAIPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMEXW item = {};
    return TreeCtrlGetItemEx(TreeCtrlHwnd(pThis), hItem, TVIF_STATEEX, &item) ? item.uStateEx : 0;
}

// Symbol: ?GetMessageMap@CTreeCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTreeCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const CTreeCtrl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetRuntimeClass@CTreeCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTreeCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CTreeCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CTreeCtrl::GetThisClass();
}

// Symbol: ?GetThisClass@CTreeCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTreeCtrl__SAPEAUCRuntimeClass__XZ() {
    return CTreeCtrl::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CTreeCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTreeCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?HitTest@CTreeCtrl@@QEBAPEAU_TREEITEM@@VCPoint@@PEAI@Z
extern "C" HTREEITEM MS_ABI impl__HitTest_CTreeCtrl__QEBAPEAU_TREEITEM__VCPoint__PEAI_Z(
    const CTreeCtrl* pThis, CPoint pt, UINT* pFlags) {
    TVHITTESTINFO hit = {};
    hit.pt.x = pt.x;
    hit.pt.y = pt.y;
    HTREEITEM hItem = (HTREEITEM)::SendMessageW(TreeCtrlHwnd(pThis), TVM_HITTEST, 0, (LPARAM)&hit);
    if (pFlags) *pFlags = hit.flags;
    return hItem;
}

// Symbol: ?InsertItem@CTreeCtrl@@QEAAPEAU_TREEITEM@@IPEB_WHHII_JPEAU2@2@Z
extern "C" HTREEITEM MS_ABI impl__InsertItem_CTreeCtrl__QEAAPEAU_TREEITEM__IPEB_WHHII_JPEAU2_2_Z(
    CTreeCtrl* pThis,
    UINT nMask,
    const wchar_t* lpszItem,
    int nImage,
    int nSelectedImage,
    UINT nState,
    UINT nStateMask,
    LPARAM lParam,
    HTREEITEM hParent,
    HTREEITEM hInsertAfter) {
    TVINSERTSTRUCTW insert = {};
    insert.hParent = hParent;
    insert.hInsertAfter = hInsertAfter;
    insert.item.mask = nMask;
    insert.item.pszText = const_cast<wchar_t*>(lpszItem);
    insert.item.iImage = nImage;
    insert.item.iSelectedImage = nSelectedImage;
    insert.item.state = nState;
    insert.item.stateMask = nStateMask;
    insert.item.lParam = lParam;
    return (HTREEITEM)::SendMessageW(TreeCtrlHwnd(pThis), TVM_INSERTITEMW, 0, (LPARAM)&insert);
}

// Symbol: ?ItemHasChildren@CTreeCtrl@@QEBAHPEAU_TREEITEM@@@Z
extern "C" int MS_ABI impl__ItemHasChildren_CTreeCtrl__QEBAHPEAU_TREEITEM___Z(
    const CTreeCtrl* pThis, HTREEITEM hItem) {
    TVITEMW item = {};
    return TreeCtrlGetItem(TreeCtrlHwnd(pThis), hItem, TVIF_CHILDREN, &item) ? (item.cChildren != 0) : FALSE;
}

// Symbol: ?OnDestroy@CTreeCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTreeCtrl__QEAAXXZ(CTreeCtrl* pThis) {
    (void)pThis;
}

// Symbol: ?RemoveImageList@CTreeCtrl@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CTreeCtrl__IEAAXH_Z(CTreeCtrl* pThis, int nImageList) {
    ::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETIMAGELIST, nImageList, 0);
}

// Symbol: ?SetCheck@CTreeCtrl@@QEAAHPEAU_TREEITEM@@H@Z
extern "C" int MS_ABI impl__SetCheck_CTreeCtrl__QEAAHPEAU_TREEITEM__H_Z(
    CTreeCtrl* pThis, HTREEITEM hItem, int fCheck) {
    TVITEMW item = {};
    item.mask = TVIF_STATE;
    item.hItem = hItem;
    item.stateMask = TVIS_STATEIMAGEMASK;
    item.state = INDEXTOSTATEIMAGEMASK(fCheck ? 2 : 1);
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETITEMW, 0, (LPARAM)&item);
}

// Symbol: ?SetItem@CTreeCtrl@@QEAAHPEAU_TREEITEM@@IPEB_WHHII_J@Z
extern "C" int MS_ABI impl__SetItem_CTreeCtrl__QEAAHPEAU_TREEITEM__IPEB_WHHII_J_Z(
    CTreeCtrl* pThis,
    HTREEITEM hItem,
    UINT nMask,
    const wchar_t* lpszItem,
    int nImage,
    int nSelectedImage,
    UINT nState,
    UINT nStateMask,
    LPARAM lParam) {
    TVITEMW item = {};
    item.mask = nMask;
    item.hItem = hItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.iSelectedImage = nSelectedImage;
    item.state = nState;
    item.stateMask = nStateMask;
    item.lParam = lParam;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETITEMW, 0, (LPARAM)&item);
}

// Symbol: ?SetItemEx@CTreeCtrl@@QEAAHPEAU_TREEITEM@@IPEB_WHHII_JIPEAUHWND__@@H@Z
extern "C" int MS_ABI impl__SetItemEx_CTreeCtrl__QEAAHPEAU_TREEITEM__IPEB_WHHII_JIPEAUHWND____H_Z(
    CTreeCtrl* pThis,
    HTREEITEM hItem,
    UINT nMask,
    const wchar_t* lpszItem,
    int nImage,
    int nSelectedImage,
    UINT nState,
    UINT nStateMask,
    LPARAM lParam,
    UINT uStateEx,
    HWND hwnd,
    int iExpandedImage) {
    TVITEMEXW item = {};
    item.mask = nMask;
    item.hItem = hItem;
    item.pszText = const_cast<wchar_t*>(lpszItem);
    item.iImage = nImage;
    item.iSelectedImage = nSelectedImage;
    item.state = nState;
    item.stateMask = nStateMask;
    item.lParam = lParam;
    item.uStateEx = uStateEx;
    item.hwnd = hwnd;
    item.iExpandedImage = iExpandedImage;
    return (int)::SendMessageW(TreeCtrlHwnd(pThis), TVM_SETITEMW, 0, (LPARAM)&item);
}

// Symbol: ?GetRuntimeClass@CColorDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CColorDialog__UEBAPEAUCRuntimeClass__XZ(const CColorDialog* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CColorDialog::GetThisClass();
}

// Symbol: ?GetThisClass@CColorDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CColorDialog__SAPEAUCRuntimeClass__XZ() {
    return CColorDialog::GetThisClass();
}

// Symbol: ?OnColorOK@CColorDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnColorOK_CColorDialog__MEAAHXZ(CColorDialog* pThis) {
    (void)pThis;
    return FALSE;
}

// Symbol: ?GetRuntimeClass@CFindReplaceDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFindReplaceDialog__UEBAPEAUCRuntimeClass__XZ(
    const CFindReplaceDialog* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CFindReplaceDialog::GetThisClass();
}

// Symbol: ?GetThisClass@CFindReplaceDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFindReplaceDialog__SAPEAUCRuntimeClass__XZ() {
    return CFindReplaceDialog::GetThisClass();
}

// Symbol: ?PostNcDestroy@CFindReplaceDialog@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CFindReplaceDialog__MEAAXXZ(CFindReplaceDialog* pThis) {
    if (pThis) pThis->m_hWnd = nullptr;
}
