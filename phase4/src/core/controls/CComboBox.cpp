// CComboBox — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/CtrlcoreSupport.h"

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
    return CComboBox::GetThisClass();
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
// Symbol: ?Create@CComboBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CComboBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CComboBox@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?AddString@CComboBox@@QEAAHPEB_W@Z
asm(".globl \"?AddString@CComboBox@@QEAAHPEB_W@Z\"\n"
    ".set \"?AddString@CComboBox@@QEAAHPEB_W@Z\", impl__AddString_CComboBox__QEAAHPEB_W_Z\n");
// Symbol: ?DeleteString@CComboBox@@QEAAHI@Z
asm(".globl \"?DeleteString@CComboBox@@QEAAHI@Z\"\n"
    ".set \"?DeleteString@CComboBox@@QEAAHI@Z\", impl__DeleteString_CComboBox__QEAAHI_Z\n");
// Symbol: ?InsertString@CComboBox@@QEAAHHPEB_W@Z
asm(".globl \"?InsertString@CComboBox@@QEAAHHPEB_W@Z\"\n"
    ".set \"?InsertString@CComboBox@@QEAAHHPEB_W@Z\", impl__InsertString_CComboBox__QEAAHHPEB_W_Z\n");
// Symbol: ?FindString@CComboBox@@QEBAHPEB_WH@Z
asm(".globl \"?FindString@CComboBox@@QEBAHPEB_WH@Z\"\n"
    ".set \"?FindString@CComboBox@@QEBAHPEB_WH@Z\", impl__FindString_CComboBox__QEBAHPEB_WH_Z\n");
// Symbol: ?GetLBText@CComboBox@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CComboBox* pThis, int nIndex, CString* pText) {
    if (!pText) return;
    if (!pThis) {
        pText->Empty();
        return;
    }
    pThis->GetLBText(nIndex, *pText);
}
void CComboBox::GetLBText(int nIndex, CString& rString) const {
    rString.Empty();
    if (!m_hWnd) return;
    int nLen = (int)::SendMessageW(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
    if (nLen < 0) return;
    wchar_t* pBuf = rString.GetBuffer(nLen + 1);
    int nCopied = (int)::SendMessageW(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)pBuf);
    rString.ReleaseBuffer(nCopied >= 0 ? nCopied : 0);
}
