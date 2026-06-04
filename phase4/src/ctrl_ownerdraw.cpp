// Standard control owner-draw and runtime-class exported wrappers.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>

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
