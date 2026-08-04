// CVSListBox — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// Symbol: ?GetMessageMap@CVSListBox@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CVSListBox__MEBAPEBUAFX_MSGMAP__XZ(const CVSListBox* pThis) {
    (void)pThis;
    return CListBox::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CVSListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBox__UEBAPEAUCRuntimeClass__XZ(const CVSListBox* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBox::GetThisClass();
}
// Symbol: ?GetThisClass@CVSListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBox__SAPEAUCRuntimeClass__XZ() {
    return CVSListBox::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CVSListBox@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBox__KAPEBUAFX_MSGMAP__XZ() {
    return CListBox::GetThisMessageMap();
}
// Symbol: ?OnCreateList@CVSListBox@@MEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__OnCreateList_CVSListBox__MEAAPEAVCWnd__XZ(CVSListBox* pThis) {
    return pThis;
}
// Symbol: ?OnDblclkList@CVSListBox@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDblclkList_CVSListBox__IEAAXPEAUtagNMHDR__PEA_J_Z(CVSListBox* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
}
// Symbol: ?OnEndLabelEdit@CVSListBox@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnEndLabelEdit_CVSListBox__IEAAXPEAUtagNMHDR__PEA_J_Z(CVSListBox* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
}
// Symbol: ?OnGetdispinfo@CVSListBox@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnGetdispinfo_CVSListBox__IEAAXPEAUtagNMHDR__PEA_J_Z(CVSListBox* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
}
// Symbol: ?OnInitControl@CVSListBox@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnInitControl_CVSListBox__IEAA_J_K_J_Z(CVSListBox* pThis, unsigned __int64, __int64) {
    return pThis ? TRUE : FALSE;
}
// Symbol: ?OnItemChanged@CVSListBox@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnItemChanged_CVSListBox__IEAAXPEAUtagNMHDR__PEA_J_Z(CVSListBox* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
}
// Symbol: ?OnKeyDown@CVSListBox@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnKeyDown_CVSListBox__IEAAXPEAUtagNMHDR__PEA_J_Z(CVSListBox* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) *pResult = 0;
}
// Symbol: ?OnSizeList@CVSListBox@@MEAAXXZ
extern "C" void MS_ABI impl__OnSizeList_CVSListBox__MEAAXXZ(CVSListBox* pThis) {
    if (pThis && pThis->GetSafeHwnd()) {
        ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
    }
}
// Symbol: ?PreTranslateMessage@CVSListBox@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CVSListBox__UEAAHPEAUtagMSG___Z(CVSListBox* pThis, MSG* pMsg) {
    (void)pThis;
    (void)pMsg;
    return FALSE;
}
// Symbol: ?AddItem@CVSListBox@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@_KH@Z
extern "C" int MS_ABI impl__AddItem_CVSListBox__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___KH_Z(
    CVSListBox* pThis, const CString* p0, uintptr_t p1, int p2) {
    return pThis && p0 ? pThis->AddItem(*p0, p1, p2) : LB_ERR;
}
// Symbol: ?RemoveItem@CVSListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__RemoveItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0) {
    return pThis ? pThis->RemoveItem(p0) : LB_ERR;
}
// Symbol: ?GetCount@CVSListBox@@UEBAHXZ
extern "C" int MS_ABI impl__GetCount_CVSListBox__UEBAHXZ(const CVSListBox* pThis) {
    return pThis ? pThis->GetCount() : 0;
}
// Symbol: ?GetSelItem@CVSListBox@@UEBAHXZ
extern "C" int MS_ABI impl__GetSelItem_CVSListBox__UEBAHXZ(const CVSListBox* pThis) {
    return pThis ? pThis->GetSelItem() : LB_ERR;
}
// Symbol: ?SelectItem@CVSListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__SelectItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0) {
    return pThis ? pThis->SelectItem(p0) : LB_ERR;
}
// Symbol: ?EditItem@CVSListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__EditItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0) {
    return pThis ? pThis->EditItem(p0) : LB_ERR;
}
// Symbol: ?GetItemData@CVSListBox@@UEBA_KH@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CVSListBox__UEBA_KH_Z(const CVSListBox* pThis, int p0) {
    return pThis ? pThis->GetItemData(p0) : 0;
}
// Symbol: ?SetItemData@CVSListBox@@UEAAXH_K@Z
extern "C" void MS_ABI impl__SetItemData_CVSListBox__UEAAXH_K_Z(CVSListBox* pThis, int p0, uintptr_t p1) {
    if (pThis) pThis->SetItemData(p0, p1);
}
// Symbol: ?GetItemText@CVSListBox@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetItemText_CVSListBox__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* __ret, const CVSListBox* pThis, int p0) {
    new(__ret) CString(pThis ? pThis->GetItemText(p0) : CString());
}
// Symbol: ?SetItemText@CVSListBox@@MEAAXHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetItemText_CVSListBox__MEAAXHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CVSListBox* pThis, int p0, const CString* p1) {
    if (pThis && p1) pThis->SetItemText(p0, *p1);
}
int CVSListBox::AddItem(const CString& strText, uintptr_t dwData, int bSelect) {
    int nIndex = CListBox::AddString((const wchar_t*)strText);
    if (nIndex >= 0) {
        CListBox::SetItemData(nIndex, dwData);
        if (bSelect) CListBox::SetCurSel(nIndex);
    }
    return nIndex;
}
int CVSListBox::RemoveItem(int nIndex) {
    return CListBox::DeleteString((unsigned int)nIndex);
}
int CVSListBox::GetCount() const {
    return CListBox::GetCount();
}
int CVSListBox::GetSelItem() const {
    return CListBox::GetCurSel();
}
int CVSListBox::SelectItem(int nIndex) {
    return CListBox::SetCurSel(nIndex);
}
int CVSListBox::EditItem(int nIndex) {
    return CListBox::SetCurSel(nIndex);
}
uintptr_t CVSListBox::GetItemData(int nIndex) const {
    return CListBox::GetItemData(nIndex);
}
void CVSListBox::SetItemData(int nIndex, uintptr_t dwData) {
    (void)CListBox::SetItemData(nIndex, dwData);
}
CString CVSListBox::GetItemText(int nIndex) const {
    int nLen = CListBox::GetTextLen(nIndex);
    if (nLen < 0) return CString();
    CString str;
    wchar_t* pBuf = str.GetBuffer(nLen + 1);
    int nCopied = CListBox::GetText(nIndex, pBuf);
    str.ReleaseBuffer(nCopied >= 0 ? nCopied : 0);
    return str;
}
void CVSListBox::SetItemText(int nIndex, const CString& strText) {
    uintptr_t dwData = CListBox::GetItemData(nIndex);
    int nSel = CListBox::GetCurSel();
    if (CListBox::DeleteString((unsigned int)nIndex) >= 0) {
        int nNew = CListBox::InsertString(nIndex, (const wchar_t*)strText);
        if (nNew >= 0) {
            CListBox::SetItemData(nNew, dwData);
            if (nSel == nIndex) CListBox::SetCurSel(nNew);
        }
    }
}
