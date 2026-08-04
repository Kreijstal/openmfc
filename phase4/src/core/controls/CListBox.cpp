// CListBox — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

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
    return CListBox::GetThisClass();
}
// Symbol: ?GetThisClass@CListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CListBox__SAPEAUCRuntimeClass__XZ() {
    return CListBox::GetThisClass();
}
// Symbol: ?ItemFromPoint@CListBox@@QEBAIVCPoint@@AEAH@Z
extern "C" UINT MS_ABI impl__ItemFromPoint_CListBox__QEBAIVCPoint__AEAH_Z(
    const CListBox* pThis, CPoint pt, int* pOutside) {
    return (UINT)ListBoxItemFromPoint_CtrlOwnerdraw(pThis ? pThis->m_hWnd : nullptr, pt, pOutside);
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
// Symbol: ?GetText@CListBox@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CListBox* pThis, int nIndex, CString* rString) {
    if (!rString) {
        return;
    }
    rString->Empty();
    int len = impl__GetTextLen_CListBox__QEBAHH_Z(pThis, nIndex);
    if (len <= 0) {
        return;
    }
    wchar_t* buffer = rString->GetBuffer(len + 1);
    int copied = impl__GetText_CListBox__QEBAHPEA_WH_Z(pThis, nIndex, buffer);
    rString->ReleaseBuffer(copied > 0 ? copied : 0);
}
