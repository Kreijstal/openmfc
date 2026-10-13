// CComboBox — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/WindowMessageSupport.h"

IMPLEMENT_DYNAMIC(CComboBox,CWnd)
// Symbol: ?CompareItem@CComboBox@@UEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
extern "C" int MS_ABI impl__CompareItem_CComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    CComboBox* pThis, COMPAREITEMSTRUCT* pCompare) {
    (void)pThis;(void)pCompare;return 0;
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
    (void)pDraw;
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
    (void)pMeasure;
}
// Symbol: ?OnChildNotify@CComboBox@@MEAAHI_K_JPEA_J@Z
extern "C" BOOL MS_ABI impl__OnChildNotify_CComboBox__MEAAHI_K_JPEA_J_Z(CComboBox* self,UINT message,WPARAM wparam,LPARAM lparam,LRESULT* result) {return self->CComboBox::OnChildNotify(message,wparam,lparam,result);}
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

void CComboBox::DrawItem(DRAWITEMSTRUCT*){}
void CComboBox::MeasureItem(MEASUREITEMSTRUCT*){}
int CComboBox::CompareItem(COMPAREITEMSTRUCT*){return 0;}
void CComboBox::DeleteItem(DELETEITEMSTRUCT*){}
extern "C" BOOL MS_ABI impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(CWnd*,UINT,WPARAM,LPARAM,LRESULT*);
BOOL CComboBox::OnChildNotify(UINT message,WPARAM wparam,LPARAM lparam,LRESULT* result) {
    switch(message) {
    case WM_DRAWITEM:DrawItem(reinterpret_cast<DRAWITEMSTRUCT*>(lparam));break;
    case WM_MEASUREITEM:MeasureItem(reinterpret_cast<MEASUREITEMSTRUCT*>(lparam));break;
    case WM_COMPAREITEM:*result=CompareItem(reinterpret_cast<COMPAREITEMSTRUCT*>(lparam));break;
    case WM_DELETEITEM:DeleteItem(reinterpret_cast<DELETEITEMSTRUCT*>(lparam));break;
    default:return impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(this,message,wparam,lparam,result);
    }
    return TRUE;
}
