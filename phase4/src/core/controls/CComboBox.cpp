// CComboBox — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, ctrlcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/WindowMessageSupport.h"

extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);

IMPLEMENT_DYNAMIC(CComboBox,CWnd)
// Symbol: ?CompareItem@CComboBox@@UEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
// Stub retained: the exact export is unresolved by disas.py in both mfc140 and mfc140u,
// so its retail body could not be verified and must not be inferred from a sibling class.
extern "C" int MS_ABI impl__CompareItem_CComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    CComboBox* pThis, COMPAREITEMSTRUCT* pCompare) {
    (void)pThis;(void)pCompare;return 0;
}
// Symbol: ?DeleteItem@CComboBox@@UEAAXPEAUtagDELETEITEMSTRUCT@@@Z
// Stub retained: the exact export is unresolved by disas.py in both mfc140 and mfc140u,
// so its retail body could not be verified and must not be inferred from a sibling class.
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
// Retail sizes from CB_GETLBTEXTLEN, terminates at that bound, then scans the result;
// a negative length throws E_INVALIDARG (GetLBText entry RVA 0x294250 (mfc140u)).
void CComboBox::GetLBText(int nIndex, CString& rString) const {
    int nLen = (int)::SendMessageW(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
    if (nLen < 0) {
        impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
        return;
    }

    wchar_t* pBuf = rString.GetBuffer(nLen);
    pBuf[nLen] = L'\0';
    (void)::SendMessageW(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)pBuf);

    int nCopied = 0;
    while (nCopied < nLen && pBuf[nCopied] != L'\0') ++nCopied;
    rString.ReleaseBuffer(nCopied);
}

void CComboBox::DrawItem(DRAWITEMSTRUCT*){}
void CComboBox::MeasureItem(MEASUREITEMSTRUCT*){}
int CComboBox::CompareItem(COMPAREITEMSTRUCT*){return 0;}
void CComboBox::DeleteItem(DELETEITEMSTRUCT*){}
BOOL CComboBox::OnChildNotify(UINT message,WPARAM wparam,LPARAM lparam,LRESULT* result) {
    switch(message) {
    case WM_DRAWITEM:DrawItem(reinterpret_cast<DRAWITEMSTRUCT*>(lparam));break;
    case WM_MEASUREITEM:MeasureItem(reinterpret_cast<MEASUREITEMSTRUCT*>(lparam));break;
    case WM_COMPAREITEM:*result=CompareItem(reinterpret_cast<COMPAREITEMSTRUCT*>(lparam));break;
    case WM_DELETEITEM:DeleteItem(reinterpret_cast<DELETEITEMSTRUCT*>(lparam));break;
    // Retail's fallback tail-jumps to CWnd::OnChildNotify (CComboBox entry RVA 0x2941d0,
    // CWnd entry RVA 0x28f0d0; both mfc140u). Use a qualified base call to avoid redispatch.
    default:return CWnd::OnChildNotify(message,wparam,lparam,result);
    }
    return TRUE;
}
