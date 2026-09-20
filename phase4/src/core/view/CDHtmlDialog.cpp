// CDHtmlDialog — OpenMFC implementation.
// Sources: viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ViewrichSupport.h"

// Symbol: ??0CDHtmlDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDHtmlDialog__QEAA_XZ(CDHtmlDialog* pThis) {
    new (pThis) CDHtmlDialog();
    return pThis;
}
// Symbol: ??0CDHtmlDialog@@QEAA@IIPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDHtmlDialog__QEAA_IIPEAVCWnd___Z(
    CDHtmlDialog* pThis, unsigned int nIDTemplate, unsigned int nHtmlResID, CWnd* pParentWnd) {
    new (pThis) CDHtmlDialog(nIDTemplate, nHtmlResID, pParentWnd);
    return pThis;
}
// Symbol: ??0CDHtmlDialog@@QEAA@PEB_W0PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDHtmlDialog__QEAA_PEB_W0PEAVCWnd___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszTemplateName,
    const wchar_t* lpszHtmlResID, CWnd* pParentWnd) {
    new (pThis) CDHtmlDialog(lpszTemplateName, lpszHtmlResID, pParentWnd);
    return pThis;
}
// Symbol: ??1CDHtmlDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CDHtmlDialog__UEAA_XZ(CDHtmlDialog* pThis) {
    pThis->~CDHtmlDialog();
}
// Symbol: ?OnInitDialog@CDHtmlDialog@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CDHtmlDialog__UEAAHXZ(CDHtmlDialog* pThis) {
    return pThis->CDHtmlDialog::OnInitDialog();
}
// Symbol: ?LoadFromResource@CDHtmlDialog@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadFromResource_CDHtmlDialog__QEAAHI_Z(
    CDHtmlDialog* pThis, unsigned int nHtmlResID) {
    return pThis->LoadFromResource(nHtmlResID);
}
// Symbol: ?LoadFromResource@CDHtmlDialog@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CDHtmlDialog__QEAAHPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszHtmlResID) {
    return pThis->LoadFromResource(lpszHtmlResID);
}
// Symbol: ?Navigate@CDHtmlDialog@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate_CDHtmlDialog__QEAAXPEB_WK00PEAXK_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                    lpvPostData, dwPostDataLen);
}
// Symbol: ?GetCurrentUrl@CDHtmlDialog@@QEAAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetCurrentUrl_CDHtmlDialog__QEAAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CDHtmlDialog* pThis, CString& strUrl) {
    pThis->GetCurrentUrl(strUrl);
}
// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, long, CString* pValue, int bSaveAndValidate) {
    DDXDHtmlText(pThis, lpszId, pValue, bSaveAndValidate);
}
// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* lpszId, long, CString* pValue) {
    DDXDHtmlText(pThis, lpszId, pValue, DHtmlSaveAndValidate(pDX));
}
// Symbol: ?DDX_DHtml_SelectString@CDHtmlDialog@@IEAAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectString_CDHtmlDialog__IEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, CString* pValue, int bSaveAndValidate) {
    DDXDHtmlText(pThis, lpszId, pValue, bSaveAndValidate);
}
// Symbol: ?DDX_DHtml_SelectString@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectString_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* lpszId, CString* pValue) {
    DDXDHtmlText(pThis, lpszId, pValue, DHtmlSaveAndValidate(pDX));
}
// Symbol: ?DDX_DHtml_SelectValue@CDHtmlDialog@@IEAAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectValue_CDHtmlDialog__IEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, CString* pValue, int bSaveAndValidate) {
    DDXDHtmlText(pThis, lpszId, pValue, bSaveAndValidate);
}
// Symbol: ?DDX_DHtml_SelectValue@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectValue_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* lpszId, CString* pValue) {
    DDXDHtmlText(pThis, lpszId, pValue, DHtmlSaveAndValidate(pDX));
}
// Symbol: ?GetElement@CDHtmlDialog@@QEAAJPEB_WPEAPEAUIDispatch@@PEAH@Z
extern "C" long MS_ABI impl__GetElement_CDHtmlDialog__QEAAJPEB_WPEAPEAUIDispatch__PEAH_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, IDispatch** ppDisp, int* pfCollection) {
    return pThis->GetElement(lpszElementId, ppDisp, pfCollection);
}
// Symbol: ?GetElement@CDHtmlDialog@@QEAAJPEB_WPEAPEAUIHTMLElement@@@Z
extern "C" long MS_ABI impl__GetElement_CDHtmlDialog__QEAAJPEB_WPEAPEAUIHTMLElement___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, IHTMLElement** ppElement) {
    return pThis->GetElement(lpszElementId, ppElement);
}
// Symbol: ?GetElementInterface@CDHtmlDialog@@QEAAJPEB_WAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetElementInterface_CDHtmlDialog__QEAAJPEB_WAEBU_GUID__PEAPEAX_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, REFIID riid, void** ppUnk) {
    return pThis->GetElementInterface(lpszElementId, riid, ppUnk);
}
// Symbol: ?GetElementProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEB_WJ@Z
extern "C" void MS_ABI impl__GetElementProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEB_WJ_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, const wchar_t* lpszElementId, long lCookie) {
    new (pRet) VARIANT(pThis->GetElementProperty(lpszElementId, lCookie));
}
// Symbol: ?SetElementProperty@CDHtmlDialog@@QEAAXPEB_WJPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetElementProperty_CDHtmlDialog__QEAAXPEB_WJPEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, long dispId, VARIANT* pVar) {
    pThis->SetElementProperty(lpszElementId, (DISPID)dispId, pVar);
}
// Symbol: ?GetElementText@CDHtmlDialog@@QEAAPEA_WPEB_W@Z
extern "C" wchar_t* MS_ABI impl__GetElementText_CDHtmlDialog__QEAAPEA_WPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId) {
    return pThis->GetElementText(lpszElementId);
}
// Symbol: ?SetElementText@CDHtmlDialog@@QEAAXPEB_WPEA_W@Z
extern "C" void MS_ABI impl__SetElementText_CDHtmlDialog__QEAAXPEB_WPEA_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, wchar_t* lpszText) {
    pThis->SetElementText(lpszElementId, lpszText);
}
// Symbol: ?GetElementHtml@CDHtmlDialog@@QEAAPEA_WPEB_W@Z
extern "C" wchar_t* MS_ABI impl__GetElementHtml_CDHtmlDialog__QEAAPEA_WPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId) {
    return pThis->GetElementHtml(lpszElementId);
}
// Symbol: ?SetElementHtml@CDHtmlDialog@@QEAAXPEB_WPEA_W@Z
extern "C" void MS_ABI impl__SetElementHtml_CDHtmlDialog__QEAAXPEB_WPEA_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszElementId, wchar_t* lpszHtml) {
    pThis->SetElementHtml(lpszElementId, lpszHtml);
}
// Symbol: ?SetElementHtml@CDHtmlDialog@@QEAAXPEAUIUnknown@@PEA_W@Z
extern "C" void MS_ABI impl__SetElementHtml_CDHtmlDialog__QEAAXPEAUIUnknown__PEA_W_Z(
    CDHtmlDialog* pThis, IUnknown* punkElem, wchar_t* lpszHtml) {
    pThis->SetElementHtml(punkElem, lpszHtml);
}
// Symbol: ?SetElementText@CDHtmlDialog@@QEAAXPEAUIUnknown@@PEA_W@Z
extern "C" void MS_ABI impl__SetElementText_CDHtmlDialog__QEAAXPEAUIUnknown__PEA_W_Z(
    CDHtmlDialog* pThis, IUnknown* punkElem, wchar_t* lpszText) {
    pThis->SetElementText(punkElem, lpszText);
}
// Symbol: ?GetControlDispatch@CDHtmlDialog@@QEAAJPEB_WPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetControlDispatch_CDHtmlDialog__QEAAJPEB_WPEAPEAUIDispatch___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, IDispatch** ppDisp) {
    return pThis->GetControlDispatch(lpszId, ppDisp);
}
// Symbol: ?GetControlProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEB_W0@Z
extern "C" void MS_ABI impl__GetControlProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEB_W0_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, const wchar_t* lpszId, const wchar_t* lpszPropName) {
    new (pRet) VARIANT(pThis->GetControlProperty(lpszId, lpszPropName));
}
// Symbol: ?GetControlProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEB_WJ@Z
extern "C" void MS_ABI impl__GetControlProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEB_WJ_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, const wchar_t* lpszId, long dispId) {
    new (pRet) VARIANT(pThis->GetControlProperty(lpszId, (DISPID)dispId));
}
// Symbol: ?SetControlProperty@CDHtmlDialog@@QEAAXPEB_W0PEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetControlProperty_CDHtmlDialog__QEAAXPEB_W0PEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, const wchar_t* lpszPropName, VARIANT* pVar) {
    pThis->SetControlProperty(lpszId, lpszPropName, pVar);
}
// Symbol: ?SetControlProperty@CDHtmlDialog@@QEAAXPEB_WJPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetControlProperty_CDHtmlDialog__QEAAXPEB_WJPEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, long dispId, VARIANT* pVar) {
    pThis->SetControlProperty(lpszId, (DISPID)dispId, pVar);
}
// Symbol: ?SetControlProperty@CDHtmlDialog@@QEAAXPEAUIDispatch@@JPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__SetControlProperty_CDHtmlDialog__QEAAXPEAUIDispatch__JPEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, long dispId, VARIANT* pVar) {
    pThis->SetControlProperty(pDisp, (DISPID)dispId, pVar);
}
// Symbol: ?GetEvent@CDHtmlDialog@@QEAAJPEAPEAUIHTMLEventObj@@@Z
extern "C" long MS_ABI impl__GetEvent_CDHtmlDialog__QEAAJPEAPEAUIHTMLEventObj___Z(
    CDHtmlDialog* pThis, IHTMLEventObj** ppEventObj) {
    return pThis->GetEvent(ppEventObj);
}
// Symbol: ?GetDHtmlDocument@CDHtmlDialog@@UEAAJPEAPEAUIHTMLDocument2@@@Z
extern "C" long MS_ABI impl__GetDHtmlDocument_CDHtmlDialog__UEAAJPEAPEAUIHTMLDocument2___Z(
    CDHtmlDialog* pThis, IHTMLDocument2** ppDocument) {
    return pThis->GetDHtmlDocument(ppDocument);
}
// Symbol: ?OnDDXError@CDHtmlDialog@@IEAAXPEB_WIH@Z
extern "C" void MS_ABI impl__OnDDXError_CDHtmlDialog__IEAAXPEB_WIH_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId, unsigned int nErrorID, int bSaveAndValidate) {
    static_cast<CDHtmlDialogAccess*>(pThis)->OnDDXError(lpszId, nErrorID, bSaveAndValidate);
}
// Symbol: ?OnBeforeNavigate@CDHtmlDialog@@UEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnBeforeNavigate_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, const wchar_t* lpszURL) {
    pThis->OnBeforeNavigate(pDisp, lpszURL);
}
// Symbol: ?OnNavigateComplete@CDHtmlDialog@@UEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, const wchar_t* lpszURL) {
    pThis->OnNavigateComplete(pDisp, lpszURL);
}
// Symbol: ?OnDocumentComplete@CDHtmlDialog@@UEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnDocumentComplete_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z(
    CDHtmlDialog* pThis, IDispatch* pDisp, const wchar_t* lpszURL) {
    pThis->OnDocumentComplete(pDisp, lpszURL);
}
// Symbol: ?FindSinkForObject@CDHtmlDialog@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__FindSinkForObject_CDHtmlDialog__IEAAHPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId) {
    return static_cast<CDHtmlDialogAccess*>(pThis)->FindSinkForObject(lpszId);
}
// Symbol: ?SetFocusToElement@CDHtmlDialog@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetFocusToElement_CDHtmlDialog__IEAAXPEB_W_Z(
    CDHtmlDialog* pThis, const wchar_t* lpszId) {
    static_cast<CDHtmlDialogAccess*>(pThis)->SetFocusToElement(lpszId);
}
// Symbol: ?Select_FindString@CDHtmlDialog@@IEAAJPEAUIHTMLSelectElement@@PEA_WH@Z
extern "C" long MS_ABI impl__Select_FindString_CDHtmlDialog__IEAAJPEAUIHTMLSelectElement__PEA_WH_Z(
    CDHtmlDialog* pThis, IHTMLSelectElement* pSelect, wchar_t* lpszFind, int bExact) {
    return static_cast<CDHtmlDialogAccess*>(pThis)->Select_FindString(pSelect, lpszFind, bExact);
}
// Symbol: ?GetHostInfo@CDHtmlDialog@@UEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__GetHostInfo_CDHtmlDialog__UEAAJPEAU_DOCHOSTUIINFO___Z(
    CDHtmlDialog* pThis, DOCHOSTUIINFO* pInfo) {
    return pThis->GetHostInfo(pInfo);
}
// Symbol: ?GetOptionKeyPath@CDHtmlDialog@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__GetOptionKeyPath_CDHtmlDialog__UEAAJPEAPEA_WK_Z(
    CDHtmlDialog* pThis, wchar_t** ppszKey, unsigned long dwReserved) {
    return pThis->GetOptionKeyPath(ppszKey, dwReserved);
}
// Symbol: ?TranslateUrl@CDHtmlDialog@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__TranslateUrl_CDHtmlDialog__UEAAJKPEA_WPEAPEA_W_Z(
    CDHtmlDialog* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    return pThis->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}
// Symbol: ?ShowContextMenu@CDHtmlDialog@@UEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__ShowContextMenu_CDHtmlDialog__UEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(
    CDHtmlDialog* pThis, unsigned long dwID, POINT* ppt, IUnknown* pReserved, IDispatch* pDisp) {
    return pThis->ShowContextMenu(dwID, ppt, pReserved, pDisp);
}
// Symbol: ?ShowUI@CDHtmlDialog@@UEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__ShowUI_CDHtmlDialog__UEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(
    CDHtmlDialog* pThis, unsigned long dwID, IOleInPlaceActiveObject* pActiveObject,
    IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame,
    IOleInPlaceUIWindow* pDoc) {
    return pThis->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}
// Symbol: ?HideUI@CDHtmlDialog@@UEAAJXZ
extern "C" long MS_ABI impl__HideUI_CDHtmlDialog__UEAAJXZ(CDHtmlDialog* pThis) {
    return pThis->HideUI();
}
// Symbol: ?UpdateUI@CDHtmlDialog@@UEAAJXZ
extern "C" long MS_ABI impl__UpdateUI_CDHtmlDialog__UEAAJXZ(CDHtmlDialog* pThis) {
    return pThis->UpdateUI();
}
// Symbol: ?EnableModeless@CDHtmlDialog@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_CDHtmlDialog__UEAAJH_Z(
    CDHtmlDialog* pThis, int fEnable) {
    return pThis->EnableModeless(fEnable);
}
// Symbol: ?OnDocWindowActivate@CDHtmlDialog@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_CDHtmlDialog__UEAAJH_Z(
    CDHtmlDialog* pThis, int fActivate) {
    return pThis->OnDocWindowActivate(fActivate);
}
// Symbol: ?OnFrameWindowActivate@CDHtmlDialog@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_CDHtmlDialog__UEAAJH_Z(
    CDHtmlDialog* pThis, int fActivate) {
    return pThis->OnFrameWindowActivate(fActivate);
}
// Symbol: ?ResizeBorder@CDHtmlDialog@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_CDHtmlDialog__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    CDHtmlDialog* pThis, const RECT* prcBorder, IOleInPlaceUIWindow* pUIWindow, int fRameWindow) {
    return pThis->ResizeBorder(prcBorder, pUIWindow, fRameWindow);
}
// Symbol: ?TranslateAcceleratorW@CDHtmlDialog@@UEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_CDHtmlDialog__UEAAJPEAUtagMSG__PEBU_GUID__K_Z(
    CDHtmlDialog* pThis, LPMSG lpMsg, const GUID* pguidCmdGroup, unsigned long nCmdID) {
    return pThis->TranslateAcceleratorW(lpMsg, pguidCmdGroup, nCmdID);
}
// Symbol: ?GetDropTarget@CDHtmlDialog@@UEAAJPEAUIDropTarget@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__GetDropTarget_CDHtmlDialog__UEAAJPEAUIDropTarget__PEAPEAU2__Z(
    CDHtmlDialog* pThis, IDropTarget* pDropTarget, IDropTarget** ppDropTarget) {
    return pThis->GetDropTarget(pDropTarget, ppDropTarget);
}
// Symbol: ?GetExternal@CDHtmlDialog@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExternal_CDHtmlDialog__UEAAJPEAPEAUIDispatch___Z(
    CDHtmlDialog* pThis, IDispatch** ppDispatch) {
    return pThis->GetExternal(ppDispatch);
}
// Symbol: ?FilterDataObject@CDHtmlDialog@@UEAAJPEAUIDataObject@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__FilterDataObject_CDHtmlDialog__UEAAJPEAUIDataObject__PEAPEAU2__Z(
    CDHtmlDialog* pThis, IDataObject* pDO, IDataObject** ppDORet) {
    return pThis->FilterDataObject(pDO, ppDORet);
}
// Symbol: ?IsExternalDispatchSafe@CDHtmlDialog@@UEAAHXZ
extern "C" int MS_ABI impl__IsExternalDispatchSafe_CDHtmlDialog__UEAAHXZ(CDHtmlDialog* pThis) {
    return SUCCEEDED(pThis->IsExternalDispatchSafe()) ? TRUE : FALSE;
}
// Symbol: ?CanAccessExternal@CDHtmlDialog@@UEAAHXZ
extern "C" int MS_ABI impl__CanAccessExternal_CDHtmlDialog__UEAAHXZ(CDHtmlDialog* pThis) {
    return SUCCEEDED(pThis->CanAccessExternal()) ? TRUE : FALSE;
}
// Symbol: ?SetExternalDispatch@CDHtmlDialog@@QEAAXPEAUIDispatch@@@Z
extern "C" void MS_ABI impl__SetExternalDispatch_CDHtmlDialog__QEAAXPEAUIDispatch___Z(
    CDHtmlDialog* pThis, IDispatch* pDisp) {
    pThis->SetExternalDispatch(pDisp);
}
// Symbol: ?SetHostFlags@CDHtmlDialog@@QEAAXK@Z
extern "C" void MS_ABI impl__SetHostFlags_CDHtmlDialog__QEAAXK_Z(
    CDHtmlDialog* pThis, unsigned long dwFlags) {
    pThis->SetHostFlags(dwFlags);
}
// Symbol: ?OnDestroy@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDHtmlDialog__IEAAXXZ(CDHtmlDialog* pThis) {
    static_cast<CDHtmlDialogAccess*>(pThis)->OnDestroy();
}
CDHtmlDialog::CDHtmlDialog()
    : m_nHtmlResID(0), m_pBrowser(nullptr), m_pCtrlWnd(nullptr), m_bCreated(FALSE),
      m_pExternalDispatch(nullptr), m_dwHostFlags(0) {
    memset(_dhtmldialog_padding, 0, sizeof(_dhtmldialog_padding));
}
CDHtmlDialog::CDHtmlDialog(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd)
    : CDialog(nIDTemplate, pParentWnd), m_nHtmlResID(nHtmlResID),
      m_pBrowser(nullptr), m_pCtrlWnd(nullptr), m_bCreated(FALSE),
      m_pExternalDispatch(nullptr), m_dwHostFlags(0) {
    memset(_dhtmldialog_padding, 0, sizeof(_dhtmldialog_padding));
}
CDHtmlDialog::CDHtmlDialog(const wchar_t* lpszTemplateName, const wchar_t* lpszHtmlResID, CWnd* pParentWnd)
    : CDialog(lpszTemplateName, pParentWnd), m_nHtmlResID(0),
      m_pBrowser(nullptr), m_pCtrlWnd(nullptr), m_bCreated(FALSE),
      m_pExternalDispatch(nullptr), m_dwHostFlags(0) {
    if (lpszHtmlResID) m_strHtmlResID = lpszHtmlResID;
    memset(_dhtmldialog_padding, 0, sizeof(_dhtmldialog_padding));
}
CDHtmlDialog::~CDHtmlDialog() {
    if (m_pExternalDispatch) {
        m_pExternalDispatch->Release();
        m_pExternalDispatch = nullptr;
    }
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
        m_pBrowser->Release();
        m_pBrowser = nullptr;
    }
}
BOOL CDHtmlDialog::Create(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    return CDialog::Create(lpszTemplateName, pParentWnd);
}
BOOL CDHtmlDialog::OnInitDialog() {
    BOOL bRet = CDialog::OnInitDialog();

    // Create WebBrowser control in the dialog
    HRESULT hr = CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_IWebBrowser2, (void**)&m_pBrowser);
    if (SUCCEEDED(hr) && m_pBrowser) {
        // Set up client site
        CHtmlViewClientSite* pSite = new CHtmlViewClientSite(m_hWnd);
        pSite->SetBrowser(m_pBrowser);

        IOleObject* pOleObject = nullptr;
        hr = m_pBrowser->QueryInterface(IID_IOleObject, (void**)&pOleObject);
        if (SUCCEEDED(hr)) {
            pOleObject->SetClientSite(pSite);
            OleSetContainedObject(pOleObject, TRUE);

            RECT rcClient;
            ::GetClientRect(m_hWnd, &rcClient);
            pOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr, pSite, 0, m_hWnd, &rcClient);
            pOleObject->Release();
        }
        pSite->Release();

        m_pBrowser->put_Visible(VARIANT_TRUE);
        m_bCreated = TRUE;

        // Load HTML if specified
        if (m_nHtmlResID)
            LoadFromResource(m_nHtmlResID);
        else if (!m_strHtmlResID.IsEmpty())
            LoadFromResource(m_strHtmlResID);
    }

    return bRet;
}
BOOL CDHtmlDialog::LoadFromResource(UINT nHtmlResID) {
    if (m_pBrowser) {
        CString strUrl = L"res://";
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            wchar_t numBuf[32];
            swprintf(numBuf, 32, L"%u", nHtmlResID);
            strUrl += numBuf;
            BSTR bstrUrl = SysAllocString(strUrl);
            m_pBrowser->Navigate(bstrUrl, nullptr, nullptr, nullptr, nullptr);
            SysFreeString(bstrUrl);
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CDHtmlDialog::LoadFromResource(const wchar_t* lpszHtmlResID) {
    if (m_pBrowser && lpszHtmlResID) {
        CString strUrl = L"res://";
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            strUrl += lpszHtmlResID;
            BSTR bstrUrl = SysAllocString(strUrl);
            m_pBrowser->Navigate(bstrUrl, nullptr, nullptr, nullptr, nullptr);
            SysFreeString(bstrUrl);
            return TRUE;
        }
    }
    return FALSE;
}
void CDHtmlDialog::Navigate(const wchar_t* lpszURL, DWORD dwFlags,
                            const wchar_t* lpszTargetFrameName,
                            const wchar_t* lpszHeaders,
                            void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    BSTR bstrUrl = SysAllocString(lpszURL);
    m_pBrowser->Navigate(bstrUrl, nullptr, nullptr, nullptr, nullptr);
    SysFreeString(bstrUrl);
    (void)dwFlags; (void)lpszTargetFrameName; (void)lpszHeaders;
    (void)lpvPostData; (void)dwPostDataLen;
}
long CDHtmlDialog::GetElement(const wchar_t* lpszElementId, IDispatch** ppDisp, int* pfCollection) {
    (void)pfCollection;
    if (!m_pBrowser || !lpszElementId || !ppDisp) return E_POINTER;
    *ppDisp = nullptr;

    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr;

    IHTMLDocument3* pDoc3 = nullptr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
    pDocDisp->Release();

    if (SUCCEEDED(hr) && pDoc3) {
        BSTR bstrId = SysAllocString(lpszElementId);
        IHTMLElement* pElem = nullptr;
        hr = pDoc3->getElementById(bstrId, &pElem);
        SysFreeString(bstrId);
        if (SUCCEEDED(hr) && pElem) {
            hr = pElem->QueryInterface(IID_IDispatch, (void**)ppDisp);
            pElem->Release();
        }
        pDoc3->Release();
    }

    return hr;
}
long CDHtmlDialog::GetElement(const wchar_t* lpszElementId, IHTMLElement** ppElement) {
    if (!ppElement) return E_POINTER;
    *ppElement = nullptr;
    if (!m_pBrowser || !lpszElementId) return E_POINTER;

    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr ? hr : E_FAIL;

    IHTMLDocument3* pDoc3 = nullptr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
    pDocDisp->Release();
    if (SUCCEEDED(hr) && pDoc3) {
        BSTR bstrId = SysAllocString(lpszElementId);
        hr = pDoc3->getElementById(bstrId, ppElement);
        SysFreeString(bstrId);
        pDoc3->Release();
    }
    return hr;
}
HRESULT CDHtmlDialog::SetElementProperty(const wchar_t* lpszElementId,
                                         DISPID dispId, VARIANT* pVar) {
    if (!m_pBrowser || !lpszElementId || !pVar) return E_POINTER;

    LPDISPATCH pDisp = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pDisp, nullptr);
    if (FAILED(hr) || !pDisp) return hr;

    DISPPARAMS dp = { pVar, nullptr, 1, 0 };
    hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
    pDisp->Release();
    return hr;
}
VARIANT CDHtmlDialog::GetElementProperty(const wchar_t* lpszElementId, long lCookie) {
    VARIANT v; VariantInit(&v);
    if (!m_pBrowser || !lpszElementId) return v;

    IDispatch* pDisp = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pDisp);
    if (FAILED(hr) || !pDisp) return v;

    DISPPARAMS dp = {};
    DISPID dispId = lCookie;
    hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
                       &dp, &v, nullptr, nullptr);
    pDisp->Release();
    if (FAILED(hr)) {
        VariantClear(&v);
    }
    return v;
}
HRESULT CDHtmlDialog::GetElementProperty(const wchar_t* lpszElementId,
                                          DISPID dispId, VARIANT* pVar) {

    LPDISPATCH pDisp = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pDisp);
    if (FAILED(hr) || !pDisp) return hr;

    DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
    hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYGET, &dp, pVar, nullptr, nullptr);
    pDisp->Release();
    return hr;
}
HRESULT CDHtmlDialog::OnDDXError(const wchar_t* lpszId, const wchar_t* lpszError) {
    if (lpszId && lpszError && *lpszError) {
        SetFocusToElement(lpszId);
    }
    return S_OK;
}
void CDHtmlDialog::GetCurrentUrl(CString& strUrl) {
    strUrl.Empty();
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationURL(&bstr)) && bstr) {
            strUrl = bstr;
            SysFreeString(bstr);
        }
    }
}
long CDHtmlDialog::GetElementInterface(const wchar_t* lpszElementId, REFIID riid, void** ppUnk) {
    if (!ppUnk) return E_POINTER;
    *ppUnk = nullptr;
    IHTMLElement* pElem = nullptr;
    HRESULT hr = GetElement(lpszElementId, &pElem);
    if (SUCCEEDED(hr) && pElem) {
        hr = pElem->QueryInterface(riid, ppUnk);
        pElem->Release();
    }
    return hr;
}
wchar_t* CDHtmlDialog::GetElementText(const wchar_t* lpszElementId) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return nullptr;
    BSTR bstr = nullptr;
    pElem->get_innerText(&bstr);
    pElem->Release();
    return bstr; // caller responsible for SysFreeString
}
void CDHtmlDialog::SetElementText(const wchar_t* lpszElementId, wchar_t* lpszText) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return;
    BSTR bstr = SysAllocString(lpszText ? lpszText : L"");
    pElem->put_innerText(bstr);
    SysFreeString(bstr);
    pElem->Release();
}
wchar_t* CDHtmlDialog::GetElementHtml(const wchar_t* lpszElementId) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return nullptr;
    BSTR bstr = nullptr;
    pElem->get_innerHTML(&bstr);
    pElem->Release();
    return bstr; // caller responsible for SysFreeString
}
void CDHtmlDialog::SetElementHtml(const wchar_t* lpszElementId, wchar_t* lpszHtml) {
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszElementId, &pElem)) || !pElem) return;
    BSTR bstr = SysAllocString(lpszHtml ? lpszHtml : L"");
    pElem->put_innerHTML(bstr);
    SysFreeString(bstr);
    pElem->Release();
}
void CDHtmlDialog::SetElementHtml(IUnknown* punkElem, wchar_t* lpszHtml) {
    if (!punkElem) return;
    IHTMLElement* pElem = nullptr;
    if (SUCCEEDED(punkElem->QueryInterface(IID_IHTMLElement, (void**)&pElem)) && pElem) {
        BSTR bstr = SysAllocString(lpszHtml ? lpszHtml : L"");
        pElem->put_innerHTML(bstr);
        SysFreeString(bstr);
        pElem->Release();
    }
}
void CDHtmlDialog::SetElementText(IUnknown* punkElem, wchar_t* lpszText) {
    if (!punkElem) return;
    IHTMLElement* pElem = nullptr;
    if (SUCCEEDED(punkElem->QueryInterface(IID_IHTMLElement, (void**)&pElem)) && pElem) {
        BSTR bstr = SysAllocString(lpszText ? lpszText : L"");
        pElem->put_innerText(bstr);
        SysFreeString(bstr);
        pElem->Release();
    }
}
long CDHtmlDialog::GetControlDispatch(const wchar_t* lpszId, IDispatch** ppDisp) {
    return GetElement(lpszId, ppDisp, nullptr);
}
VARIANT CDHtmlDialog::GetControlProperty(const wchar_t* lpszId, const wchar_t* lpszPropName) {
    VARIANT v; VariantInit(&v);
    if (!lpszId || !lpszPropName) return v;
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return v;
    // Get property by name via GetIDsOfNames + Invoke
    BSTR bstrProp = SysAllocString(lpszPropName);
    DISPID dispid = DISPID_UNKNOWN;
    if (SUCCEEDED(pDisp->GetIDsOfNames(IID_NULL, &bstrProp, 1, LOCALE_USER_DEFAULT, &dispid))) {
        DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
        pDisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                      DISPATCH_PROPERTYGET, &dp, &v, nullptr, nullptr);
    }
    SysFreeString(bstrProp);
    pDisp->Release();
    return v;
}
VARIANT CDHtmlDialog::GetControlProperty(const wchar_t* lpszId, DISPID dispId) {
    VARIANT v; VariantInit(&v);
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return v;
    DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
    pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                  DISPATCH_PROPERTYGET, &dp, &v, nullptr, nullptr);
    pDisp->Release();
    return v;
}
void CDHtmlDialog::SetControlProperty(const wchar_t* lpszId, const wchar_t* lpszPropName, VARIANT* pVar) {
    if (!lpszId || !lpszPropName || !pVar) return;
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return;
    BSTR bstrProp = SysAllocString(lpszPropName);
    DISPID dispid = DISPID_UNKNOWN;
    if (SUCCEEDED(pDisp->GetIDsOfNames(IID_NULL, &bstrProp, 1, LOCALE_USER_DEFAULT, &dispid))) {
        DISPID namedId = DISPID_PROPERTYPUT;
        DISPPARAMS dp = { pVar, &namedId, 1, 1 };
        pDisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                      DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
    }
    SysFreeString(bstrProp);
    pDisp->Release();
}
void CDHtmlDialog::SetControlProperty(const wchar_t* lpszId, DISPID dispId, VARIANT* pVar) {
    if (!lpszId || !pVar) return;
    IDispatch* pDisp = nullptr;
    if (FAILED(GetControlDispatch(lpszId, &pDisp)) || !pDisp) return;
    DISPID namedId = DISPID_PROPERTYPUT;
    DISPPARAMS dp = { pVar, &namedId, 1, 1 };
    pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                  DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
    pDisp->Release();
}
void CDHtmlDialog::SetControlProperty(IDispatch* pDisp, DISPID dispId, VARIANT* pVar) {
    if (!pDisp || !pVar) return;
    DISPID namedId = DISPID_PROPERTYPUT;
    DISPPARAMS dp = { pVar, &namedId, 1, 1 };
    pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                  DISPATCH_PROPERTYPUT, &dp, nullptr, nullptr, nullptr);
}
long CDHtmlDialog::GetEvent(IHTMLEventObj** ppEventObj) {
    if (!ppEventObj) {
        return E_POINTER;
    }
    *ppEventObj = nullptr;
    if (!m_pBrowser) {
        return S_FALSE;
    }

    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) {
        return hr;
    }

    IHTMLDocument2* pDoc2 = nullptr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc2);
    if (FAILED(hr) || !pDoc2) {
        pDocDisp->Release();
        return hr;
    }

    IHTMLWindow2* pWindow = nullptr;
    hr = pDoc2->get_parentWindow((IHTMLWindow2**)&pWindow);
    pDoc2->Release();
    pDocDisp->Release();
    if (FAILED(hr) || !pWindow) {
        return hr;
    }

    IHTMLEventObj* pEvent = nullptr;
    hr = pWindow->get_event(&pEvent);
    pWindow->Release();
    if (FAILED(hr)) {
        return hr;
    }
    if (!pEvent) {
        return S_FALSE;
    }

    *ppEventObj = pEvent;
    return S_OK;
}
HRESULT CDHtmlDialog::GetDHtmlDocument(IHTMLDocument2** ppDocument) {
    if (!ppDocument) return E_POINTER;
    *ppDocument = nullptr;
    if (!m_pBrowser) return E_FAIL;
    LPDISPATCH pDocDisp = nullptr;
    HRESULT hr = m_pBrowser->get_Document(&pDocDisp);
    if (FAILED(hr) || !pDocDisp) return hr;
    hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)ppDocument);
    pDocDisp->Release();
    return hr;
}
void CDHtmlDialog::OnBeforeNavigate(IDispatch*, const wchar_t* lpszURL) {
    if (lpszURL && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszURL);
    }
}
void CDHtmlDialog::OnNavigateComplete(IDispatch*, const wchar_t* lpszURL) {
    if (lpszURL && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszURL);
    }
}
void CDHtmlDialog::OnDocumentComplete(IDispatch*, const wchar_t* lpszURL) {
    if (lpszURL && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszURL);
    }
}
int CDHtmlDialog::FindSinkForObject(const wchar_t* lpszId) {
    if (!lpszId || !*lpszId || !m_pBrowser) return -1;
    IHTMLElement* pElem = nullptr;
    if (SUCCEEDED(GetElement(lpszId, &pElem)) && pElem) {
        pElem->Release();
        return 0;
    }
    return -1;
}
void CDHtmlDialog::SetFocusToElement(const wchar_t* lpszId) {
    if (!lpszId || !m_pBrowser) return;
    IHTMLElement* pElem = nullptr;
    if (FAILED(GetElement(lpszId, &pElem)) || !pElem) return;
    IHTMLElement2* pElem2 = nullptr;
    HRESULT hr = pElem->QueryInterface(IID_IHTMLElement2, (void**)&pElem2);
    pElem->Release();
    if (FAILED(hr) || !pElem2) return;
    pElem2->focus();
    pElem2->Release();
}
long CDHtmlDialog::Select_FindString(IHTMLSelectElement* pSelect, wchar_t* lpszFind, int bExact) {
    if (!pSelect || !lpszFind || !*lpszFind) {
        return -1;
    }
    IDispatch* pDispatch = nullptr;
    if (FAILED(pSelect->get_options(&pDispatch)) || !pDispatch) return -1;
    IHTMLElementCollection* pOptions = nullptr;
    HRESULT hr = pDispatch->QueryInterface(IID_IHTMLElementCollection, (void**)&pOptions);
    pDispatch->Release();
    if (FAILED(hr) || !pOptions) return -1;

    long count = 0;
    pOptions->get_length(&count);
    for (long i = 0; i < count; ++i) {
        VARIANT idx;
        VARIANT name;
        IDispatch* pItem = nullptr;
        BSTR bstrText = nullptr;
        VariantInit(&idx);
        VariantInit(&name);
        idx.vt = VT_I4;
        idx.lVal = i;
        HRESULT hrItem = pOptions->item(name, idx, &pItem);
        VariantClear(&idx);
        VariantClear(&name);
        if (FAILED(hrItem) || !pItem) continue;

        IHTMLOptionElement* pOpt = nullptr;
        HRESULT hrOpt = pItem->QueryInterface(IID_IHTMLOptionElement, (void**)&pOpt);
        pItem->Release();
        if (FAILED(hrOpt) || !pOpt) continue;
        HRESULT hrText = pOpt->get_text(&bstrText);
        pOpt->Release();
        if (FAILED(hrText) || !bstrText) continue;
        bool matched = false;
        if (bExact) {
            matched = (wcscmp(bstrText, lpszFind) == 0);
        } else {
            matched = (wcsstr(bstrText, lpszFind) != nullptr);
        }
        SysFreeString(bstrText);
        if (matched) {
            pOptions->Release();
            return i;
        }
    }
    pOptions->Release();
    return -1;
}
HRESULT CDHtmlDialog::GetHostInfo(DOCHOSTUIINFO* pInfo) {
    if (!pInfo) return E_POINTER;
    pInfo->cbSize = sizeof(DOCHOSTUIINFO);
    pInfo->dwFlags = m_dwHostFlags;
    pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
    return S_OK;
}
HRESULT CDHtmlDialog::GetOptionKeyPath(wchar_t** ppwszPathKey, DWORD) {
    if (!ppwszPathKey) {
        return E_POINTER;
    }
    const wchar_t kBrowserKey[] = L"Software\\Microsoft\\Internet Explorer\\Main";
    size_t cch = (sizeof(kBrowserKey) / sizeof(kBrowserKey[0]));
    wchar_t* p = static_cast<wchar_t*>(CoTaskMemAlloc(cch * sizeof(wchar_t)));
    if (!p) {
        return E_OUTOFMEMORY;
    }
    memcpy(p, kBrowserKey, cch * sizeof(wchar_t));
    *ppwszPathKey = p;
    return S_OK;
}
HRESULT CDHtmlDialog::TranslateUrl(DWORD, wchar_t* pchURLIn, wchar_t** ppOut) {
    if (!ppOut) {
        return E_POINTER;
    }
    *ppOut = nullptr;
    if (!pchURLIn) {
        return S_OK;
    }
    *ppOut = CoTaskMemDuplicateW(pchURLIn);
    return *ppOut ? S_OK : E_OUTOFMEMORY;
}
HRESULT CDHtmlDialog::ShowContextMenu(DWORD, POINT*, IUnknown*, IDispatch*) { return S_OK; }
HRESULT CDHtmlDialog::ShowUI(DWORD, IOleInPlaceActiveObject*, IOleCommandTarget*,
                              IOleInPlaceFrame*, IOleInPlaceUIWindow*) { return S_OK; }
HRESULT CDHtmlDialog::HideUI() { return S_OK; }
HRESULT CDHtmlDialog::UpdateUI() { return S_OK; }
HRESULT CDHtmlDialog::EnableModeless(BOOL) { return S_OK; }
HRESULT CDHtmlDialog::OnDocWindowActivate(BOOL) { return S_OK; }
HRESULT CDHtmlDialog::OnFrameWindowActivate(BOOL) { return S_OK; }
HRESULT CDHtmlDialog::ResizeBorder(LPCRECT, IOleInPlaceUIWindow*, BOOL) { return S_OK; }
HRESULT CDHtmlDialog::TranslateAcceleratorW(LPMSG, const GUID*, DWORD) { return S_FALSE; }
HRESULT CDHtmlDialog::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppOut) {
    if (!ppOut) return E_POINTER;
    *ppOut = nullptr;
    if (!pDropTarget) return S_FALSE;
    pDropTarget->AddRef();
    *ppOut = pDropTarget;
    return S_OK;
}
HRESULT CDHtmlDialog::GetExternal(IDispatch** ppOut) {
    if (!ppOut) return E_POINTER;
    *ppOut = nullptr;
    if (!m_pExternalDispatch) return S_FALSE;
    m_pExternalDispatch->AddRef();
    *ppOut = m_pExternalDispatch;
    return S_OK;
}
HRESULT CDHtmlDialog::FilterDataObject(IDataObject* pDataObject, IDataObject** ppOut) {
    if (!ppOut) return E_POINTER;
    *ppOut = nullptr;
    if (!pDataObject) return S_FALSE;
    pDataObject->AddRef();
    *ppOut = pDataObject;
    return S_OK;
}
HRESULT CDHtmlDialog::IsExternalDispatchSafe() { return m_pExternalDispatch ? S_OK : S_FALSE; }
HRESULT CDHtmlDialog::CanAccessExternal() { return m_pExternalDispatch ? S_OK : S_FALSE; }
HRESULT CDHtmlDialog::CreateControlSite(COleControlContainer* pContainer,
                                       COleControlSite** pSite,
                                       UINT nID, REFCLSID clsid) {
    if (!pSite) {
        return E_POINTER;
    }
    (void)clsid;
    *pSite = nullptr;
    if (!pContainer) {
        return E_INVALIDARG;
    }
    COleControlSite* created = pContainer->CreateSite(pContainer);
    if (!created) {
        return E_OUTOFMEMORY;
    }
    CWnd* containerWnd = pContainer->GetWnd();
    if (containerWnd) {
        containerWnd->AttachControlSite(created, nID);
    }
    *pSite = created;
    return S_OK;
}
void CDHtmlDialog::SetExternalDispatch(IDispatch* pDispatch) {
    if (m_pExternalDispatch) {
        m_pExternalDispatch->Release();
        m_pExternalDispatch = nullptr;
    }
    m_pExternalDispatch = pDispatch;
    if (m_pExternalDispatch) {
        m_pExternalDispatch->AddRef();
    }
}
void CDHtmlDialog::SetHostFlags(DWORD dwFlags) {
    m_dwHostFlags = dwFlags;
}
void CDHtmlDialog::OnDDXError(const wchar_t* lpszId, UINT nErrorID, int) {
    wchar_t szError[128];
    swprintf(szError, 128, L"DDX validation failed (id=%u)", nErrorID);
    OnDDXError(lpszId, szError);
}
void CDHtmlDialog::OnDestroy() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
    }
    CDialog::OnDestroy();
}

// === Moved from ManualThunks.cpp ===
//
// The bodies below are transcribed from retail mfc140u.dll (all RVAs are mfc140u
// function ENTRIES unless a specific instruction is named).  Retail keeps its
// state in a layout this header does not have: a CDHtmlEventSink subobject at
// +0x130 (IDispatch sink vptr), m_spHtmlDoc at +0x160, m_wndBrowser (CWnd) at
// +0x178, m_dwDHtmlEventSinkCookie at +0x260, m_ControlSinks at +0x268 and
// m_SinkedElements at +0x278 (CSimpleArray<T*> each).  OpenMFC's CDHtmlDialog
// instead owns a raw IWebBrowser2* (m_pBrowser) and reaches the document
// through GetDHtmlDocument(), so every "m_spHtmlDoc"/"GetDHtmlDocument via the
// sink vtable (slot 9, +0x48)" in retail is rendered as pThis->GetDHtmlDocument()
// here, and the element/control event-sink members simply do not exist -- the
// functions that need them stay stubs and say so.

#include <cstdarg>
#include <cfloat>

// Sibling thunks that live in other translation units (definitions verified):
//   impl__Default_CWnd__IEAA_JXZ                       core/window/Thunks.cpp
//   impl__DestroyWindow_CWnd__UEAAHXZ                  core/window/CWnd.cpp
//   impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ
//                                                      core/runtime/CCmdTarget.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ();
// Forward declarations of the two format workers defined further down in this
// file (the numeric DDX_DHtml_ElementText overloads call them):
extern "C" void MS_ABI impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, const wchar_t* lpszFormat,
    unsigned int nIDPrompt, int bSave, ...);
extern "C" void MS_ABI impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, void* pData, double value,
    int nSizeGcvt, int bSave);

namespace {

// afxres.h prompt IDs used by the numeric DDX_DHtml_ElementText overloads.  The
// values are the immediates retail passes (0xf110 at 0x2127a0/0x2127e0/0x212750,
// 0xf117 at 0x212820, 0xf111 at 0x2134b0) and match the shipping afxres.h.
constexpr unsigned int kAfxIdpParseInt  = 0xF110;   // AFX_IDP_PARSE_INT
constexpr unsigned int kAfxIdpParseReal = 0xF111;   // AFX_IDP_PARSE_REAL
constexpr unsigned int kAfxIdpParseUInt = 0xF117;   // AFX_IDP_PARSE_UINT

// Retail's `UINT __declspec(selectany) CDHtmlDialog::WM_DESTROYMODELESS = 0`
// (afxdhtml.h) -- a per-image static that DestroyModeless fills lazily.  The
// OpenMFC header has no such static, so it lives here at file scope.
UINT g_wmDestroyModeless = 0;

// _AfxSimpleScanf, as inlined into DDX_DHtml_ElementTextWithFormat (mfc140u
// 0x2132b0, instructions 0x21331d..0x2133e5).  Only "%d", "%u", "%ld", "%lu",
// "%sd" style formats are understood: a leading 'l' is skipped, a leading 's'
// selects the short* store; the value is parsed with wcstol/wcstoul (IAT slots
// 0x1802c7468 / 0x1802c7458) after skipping blanks/tabs, must be followed only
// by blanks/tabs and the terminator, and a zero result is only accepted when
// the first non-blank character was '0'.
bool DHtmlSimpleScanf(const wchar_t* lpszText, const wchar_t* lpszFormat, va_list pData) {
    lpszFormat++;                       // skip '%'
    bool bShort = false;
    if (*lpszFormat == L'l') {
        lpszFormat++;
    } else if (*lpszFormat == L's') {
        bShort = true;
        lpszFormat++;
    }
    while (*lpszText == L' ' || *lpszText == L'\t') lpszText++;
    const wchar_t chFirst = lpszText[0];
    long l;
    if (*lpszFormat == L'd') {
        l = wcstol(lpszText, const_cast<wchar_t**>(&lpszText), 10);
    } else {
        if (*lpszText == L'-') return false;
        l = static_cast<long>(wcstoul(lpszText, const_cast<wchar_t**>(&lpszText), 10));
    }
    if (l == 0 && chFirst != L'0') return false;
    while (*lpszText == L' ' || *lpszText == L'\t') lpszText++;
    if (*lpszText != L'\0') return false;
    if (bShort) {
        if (static_cast<short>(l) != l) return false;
        *va_arg(pData, short*) = static_cast<short>(l);
    } else {
        *va_arg(pData, long*) = l;
    }
    return true;
}

// Retail CDHtmlDialog::GetElement(LPCTSTR, IDispatch**, BOOL*) (mfc140u
// 0x214290), transcribed here because DDX_DHtml_Radio depends on its
// collection semantics: it resolves the id through document.all.item(name),
// which yields an IHTMLElementCollection when several elements (a radio
// group) share the name, and reports that through *pbCollection.  The
// CDHtmlDialog::GetElement defined earlier in this file goes through
// getElementById instead and never reports a collection, so it cannot serve
// the radio walk.  Deviations from retail: a NULL ppdisp returns E_POINTER
// instead of AfxThrowInvalidArgException, and a failed SysAllocStringLen
// returns E_OUTOFMEMORY instead of AfxThrowMemoryException.
HRESULT DHtmlGetElementByName(CDHtmlDialog* pThis, const wchar_t* szElementId,
                              IDispatch** ppdisp, BOOL* pbCollection) {
    if (!ppdisp) return E_POINTER;
    IHTMLElementCollection* pAll = nullptr;
    IHTMLElementCollection* pColl = nullptr;
    IDispatch* pdispElem = nullptr;
    IHTMLDocument2* pDoc = nullptr;
    VARIANT varName, varIndex;
    VariantInit(&varName);
    VariantInit(&varIndex);
    *ppdisp = nullptr;
    if (pbCollection) *pbCollection = FALSE;
    HRESULT hr = pThis->GetDHtmlDocument(&pDoc);
    if (!pDoc) return hr;
    if (!szElementId) {
        pDoc->Release();
        return E_INVALIDARG;
    }
    varName.vt = VT_BSTR;
    varName.bstrVal = ::SysAllocStringLen(szElementId, static_cast<UINT>(wcslen(szElementId)));
    if (!varName.bstrVal) {
        pDoc->Release();
        return E_OUTOFMEMORY;
    }
    hr = pDoc->get_all(&pAll);                          // IHTMLDocument2 slot 8 (+0x40)
    if (pAll) {
        hr = pAll->item(varName, varIndex, &pdispElem); // IHTMLElementCollection slot 11 (+0x58)
        if (!pdispElem) {
            hr = E_NOINTERFACE;
        } else {
            pdispElem->QueryInterface(IID_IHTMLElementCollection, reinterpret_cast<void**>(&pColl));
            if (pColl && pbCollection) *pbCollection = TRUE;
        }
    }
    if (SUCCEEDED(hr)) {
        *ppdisp = pdispElem;
        if (pdispElem) pdispElem->AddRef();
    }
    VariantClear(&varIndex);
    VariantClear(&varName);
    if (pdispElem) pdispElem->Release();
    if (pColl) pColl->Release();
    if (pAll) pAll->Release();
    pDoc->Release();
    return hr;
}

// --- event sink map ---------------------------------------------------------
// Local mirrors of the _AFXDLL AFX_DISPMAP_ENTRY / AFX_EVENTSINKMAP_ENTRY /
// AFX_EVENTSINKMAP layouts (same definitions core/runtime/CCmdTarget.cpp keeps
// privately; strides pinned below).
struct DHtmlDispMapEntry {
    const wchar_t*  lpszName;     // +0x00
    long            lDispID;      // +0x08
    const char*     lpszParams;   // +0x10
    unsigned short  vt;           // +0x18
    const void*     pfn;          // +0x20
    const void*     pfnSet;       // +0x28
    size_t          nPropOffset;  // +0x30
    int             flags;        // +0x38  (afxDispCustom == 0)
};
struct DHtmlEventSinkEntry {
    DHtmlDispMapEntry dispEntry;  // +0x00
    unsigned int      nCtrlIDFirst; // +0x40
    unsigned int      nCtrlIDLast;  // +0x44
};
struct DHtmlEventSinkMap {
    const void* (MS_ABI* pfnGetBaseMap)();   // +0x00
    const DHtmlEventSinkEntry* lpEntries;    // +0x08
    unsigned int*              lpEntryCount; // +0x10
};
static_assert(sizeof(DHtmlDispMapEntry) == 0x40, "AFX_DISPMAP_ENTRY stride is 0x40");
static_assert(sizeof(DHtmlEventSinkEntry) == 0x48, "AFX_EVENTSINKMAP_ENTRY stride is 0x48");
static_assert(offsetof(DHtmlEventSinkEntry, nCtrlIDFirst) == 0x40, "nCtrlIDFirst at +0x40");
static_assert(sizeof(DHtmlEventSinkMap) == 0x18, "AFX_EVENTSINKMAP is three pointers");

// The three private handlers the retail map points at (mfc140u 0x212100 =
// _OnBeforeNavigate2, 0x212170 = _OnNavigateComplete2, 0x2121e0 =
// _OnDocumentComplete; none is exported).  Each builds a CString from
// URL->bstrVal (VARIANT +0x8; CStringT(LPCWSTR) ctor, 0xdcb0) and forwards to
// the virtual OnBeforeNavigate / OnNavigateComplete / OnDocumentComplete
// (retail vtable slots +0x330 / +0x338 / +0x340; here the OpenMFC C++
// virtual).  Retail dereferences URL unconditionally; the NULL guard below is
// an OpenMFC addition.
void MS_ABI DHtml_OnBeforeNavigate2(CDHtmlDialog* pThis, IDispatch* pDisp, VARIANT* URL,
                                    VARIANT*, VARIANT*, VARIANT*, VARIANT*, BOOL*) {
    CString strUrl(URL ? URL->bstrVal : nullptr);
    pThis->OnBeforeNavigate(pDisp, strUrl);
}
void MS_ABI DHtml_OnNavigateComplete2(CDHtmlDialog* pThis, IDispatch* pDisp, VARIANT* URL) {
    CString strUrl(URL ? URL->bstrVal : nullptr);
    pThis->OnNavigateComplete(pDisp, strUrl);
}
void MS_ABI DHtml_OnDocumentComplete(CDHtmlDialog* pThis, IDispatch* pDisp, VARIANT* URL) {
    CString strUrl(URL ? URL->bstrVal : nullptr);
    pThis->OnDocumentComplete(pDisp, strUrl);
}

// CDHtmlDialog's event sink map, byte-for-byte from retail mfc140u .rdata at
// 0x3276b0 = { CCmdTarget::GetThisEventSinkMap (0x1de860), entries 0x327590,
// count 0x3b29c0 (initially 0xffffffff, filled lazily) }.  Entries, in order:
//   { L"", 252 /*NavigateComplete2*/, "\tL"      (VTS_DISPATCH VTS_PVARIANT),
//     VT_BOOL, _OnNavigateComplete2, 0, 0, afxDispCustom }, AFX_IDC_BROWSER, -1
//   { L"", 250 /*BeforeNavigate2*/,   "\tLLLLLK" (VTS_DISPATCH 5xVTS_PVARIANT
//     VTS_PBOOL), VT_BOOL, _OnBeforeNavigate2, ... },        AFX_IDC_BROWSER, -1
//   { L"", 259 /*DocumentComplete*/,  "\tL", VT_BOOL, _OnDocumentComplete, ... },
//                                                            AFX_IDC_BROWSER, -1
//   terminator { NULL, DISPID_UNKNOWN, NULL, VT_VOID, NULL, NULL, (size_t)-1,
//     afxDispCustom }, (UINT)-1, 0
// AFX_IDC_BROWSER == 102 (afxres.h; 0x66 in the retail entries).
constexpr unsigned int kAfxIdcBrowser = 102;
const DHtmlEventSinkEntry g_dhtmlEventSinkEntries[] = {
    { { L"", 252, "\tL",      VT_BOOL, reinterpret_cast<const void*>(&DHtml_OnNavigateComplete2), nullptr, 0, 0 },
      kAfxIdcBrowser, 0xffffffffu },
    { { L"", 250, "\tLLLLLK", VT_BOOL, reinterpret_cast<const void*>(&DHtml_OnBeforeNavigate2),   nullptr, 0, 0 },
      kAfxIdcBrowser, 0xffffffffu },
    { { L"", 259, "\tL",      VT_BOOL, reinterpret_cast<const void*>(&DHtml_OnDocumentComplete),  nullptr, 0, 0 },
      kAfxIdcBrowser, 0xffffffffu },
    { { nullptr, -1, nullptr, VT_VOID, nullptr, nullptr, static_cast<size_t>(-1), 0 },
      0xffffffffu, 0 },
};
unsigned int g_dhtmlEventSinkEntryCount = 0xffffffffu;
const DHtmlEventSinkMap g_dhtmlEventSinkMap = {
    &impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ,
    g_dhtmlEventSinkEntries,
    &g_dhtmlEventSinkEntryCount,
};

} // namespace

// CDHtmlDialog::GetControlProperty(IDispatch*, DISPID) -- mfc140u 0x214b90,
// transcribed.  Hidden return slot in RDX (pRet), this in RCX, pdispControl in
// R8, dispid in R9D:
//     pRet->vt = VT_EMPTY;                       ; only vt is written
//     if (pdispControl) {
//         DISPPARAMS dp = { NULL, NULL, 0, 0 };
//         pdispControl->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT (0x400),
//                              DISPATCH_PROPERTYGET (2), &dp, pRet, NULL, NULL);
//     }                                          ; IDispatch slot 6 (+0x30)
//     return pRet;                               ; mov %rbx,%rax
// The Invoke HRESULT is discarded, as in retail.  Deviation: the whole VARIANT
// is VariantInit'ed rather than only vt, so a failed Invoke leaves a clean
// VT_EMPTY.
// Symbol: ?GetControlProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEAUIDispatch@@J@Z
extern "C" VARIANT* MS_ABI impl__GetControlProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEAUIDispatch__J_Z(
    VARIANT* pRet, CDHtmlDialog* pThis, IDispatch* pdispControl, long dispid) {
    (void)pThis;
    VariantInit(pRet);
    if (pdispControl) {
        DISPPARAMS dp = { nullptr, nullptr, 0, 0 };
        pdispControl->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
                             &dp, pRet, nullptr, nullptr);
    }
    return pRet;
}

// CDHtmlDialog::ConnectDHtmlElementEvents(DWORD_PTR dwThunkOffset) -- mfc140u
// 0x213b20.  Retail walks GetDHtmlEventMap() (sink vtable slot 7 at +0x130)
// and, per DHTMLEVENTMAPENTRY_ELEMENT/CONTROL entry, creates a
// CDHtmlElementEventSink / CDHtmlControlSink whose handler is the
// CDHtmlEventSink subobject (this + 0x130), Advise()s it on the element and
// appends it to m_SinkedElements (+0x278) / m_ControlSinks (+0x268).  OpenMFC's
// CDHtmlDialog has no CDHtmlEventSink base and neither CSimpleArray, so there
// is nothing to sink into; left a stub (returns S_OK, i.e. "nothing to hook").
// Symbol: ?ConnectDHtmlElementEvents@CDHtmlDialog@@IEAAJ_K@Z
extern "C" long MS_ABI impl__ConnectDHtmlElementEvents_CDHtmlDialog__IEAAJ_K_Z(
    CDHtmlDialog* pThis, unsigned __int64 dwThunkOffset) {
    (void)pThis; (void)dwThunkOffset;
    return S_OK;
}

// CDHtmlDialog::ConnectDHtmlEvents(IUnknown* punkDoc) -- mfc140u 0x213a00:
//     return AfxConnectionAdvise-style helper (0x21579c)(punkDoc,
//                (IUnknown*)(CDHtmlEventSink*)this /* +0x130 */,
//                DIID_HTMLDocumentEvents (0x34c7b8),
//                &m_dwDHtmlEventSinkCookie /* +0x260 */);
// i.e. Advise the dialog's own IDispatch sink on the document's
// HTMLDocumentEvents connection point.  There is no IDispatch sink subobject
// and no cookie member in the OpenMFC layout, so this stays a stub.
// Symbol: ?ConnectDHtmlEvents@CDHtmlDialog@@IEAAJPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__ConnectDHtmlEvents_CDHtmlDialog__IEAAJPEAUIUnknown___Z(
    CDHtmlDialog* pThis, IUnknown* punkDoc) {
    (void)pThis; (void)punkDoc;
    return S_OK;
}

// CDHtmlDialog::DDX_DHtml_AxControl(LPCTSTR, LPCTSTR, VARIANT&, BOOL) -- mfc140u
// 0x2131f0, transcribed:
//     if (bSave) var = GetControlProperty(szId, szPropName);   ; 0x214cc0, 24-byte copy
//     else       SetControlProperty(szId, szPropName, &var);   ; tail-jump 0x214f60
// The raw struct copy (no VariantClear of the previous value) is retail's.
// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@IEAAXPEB_W0AEAUtagVARIANT@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__IEAAXPEB_W0AEAUtagVARIANT__H_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, const wchar_t* szPropName, VARIANT* pVar, int bSave) {
    if (bSave)
        *pVar = pThis->GetControlProperty(szId, szPropName);
    else
        pThis->SetControlProperty(szId, szPropName, pVar);
}

// CDHtmlDialog::DDX_DHtml_AxControl(LPCTSTR, DISPID, VARIANT&, BOOL) -- mfc140u
// 0x2131b0, transcribed:
//     if (bSave) var = GetControlProperty(szId, dispid);       ; 0x214c00
//     else       SetControlProperty(szId, dispid, &var);       ; tail-jump 0x214e90
// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@IEAAXPEB_WJAEAUtagVARIANT@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__IEAAXPEB_WJAEAUtagVARIANT__H_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, VARIANT* pVar, int bSave) {
    if (bSave)
        *pVar = pThis->GetControlProperty(szId, static_cast<DISPID>(dispid));
    else
        pThis->SetControlProperty(szId, static_cast<DISPID>(dispid), pVar);
}

// Public CDataExchange overloads -- mfc140u 0x2125a0 / 0x212580 / 0x2124e0 /
// 0x212500 / 0x212560 are each a register shuffle that loads
// pDX->m_bSaveAndValidate (+0x00, no NULL check) and tail-jumps to the
// protected overload.  DHtmlSaveAndValidate() is the same read with a NULL
// guard, as the sibling ElementText/Select* wrappers above already use.
// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_W1AEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_W1AEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, const wchar_t* szPropName, VARIANT* pVar) {
    impl__DDX_DHtml_AxControl_CDHtmlDialog__IEAAXPEB_W0AEAUtagVARIANT__H_Z(
        pThis, szId, szPropName, pVar, DHtmlSaveAndValidate(pDX));
}

// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAUtagVARIANT___Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, VARIANT* pVar) {
    impl__DDX_DHtml_AxControl_CDHtmlDialog__IEAAXPEB_WJAEAUtagVARIANT__H_Z(
        pThis, szId, dispid, pVar, DHtmlSaveAndValidate(pDX));
}

// CDHtmlDialog::DDX_DHtml_CheckBox(LPCTSTR, int&, BOOL) -- mfc140u 0x2128c0,
// transcribed:
//     CComVariant varUnused;                       ; memset 24 + VariantClear, never read
//     CComPtr<IHTMLDocument2> spDoc; GetDHtmlDocument(&spDoc);   ; sink slot 9
//     if (spDoc) {
//         CComPtr<IHTMLOptionButtonElement> spOpt;
//         GetElementInterface(szId, IID_IHTMLOptionButtonElement (0x34c7f8), &spOpt);
//         if (spOpt) {
//             if (bSave) {
//                 VARIANT_BOOL vbChecked, vbIndeterminate;
//                 if (SUCCEEDED(spOpt->get_checked(&vbChecked))            ; slot 13 (+0x68)
//                  && SUCCEEDED(spOpt->get_indeterminate(&vbIndeterminate)))  ; slot 23 (+0xb8)
//                     value = (vbIndeterminate ? 2 : 0) | (vbChecked ? 1 : 0);
//             } else {
//                 if (SUCCEEDED(spOpt->put_checked((value & 1) ? VARIANT_TRUE : VARIANT_FALSE)))   ; slot 12 (+0x60)
//                     spOpt->put_indeterminate((value & 2) ? VARIANT_TRUE : VARIANT_FALSE);        ; slot 22 (+0xb0)
//             }
//         }
//     }
// The unused CComVariant is dropped here (no observable effect).
// Symbol: ?DDX_DHtml_CheckBox@CDHtmlDialog@@IEAAXPEB_WAEAHH@Z
extern "C" void MS_ABI impl__DDX_DHtml_CheckBox_CDHtmlDialog__IEAAXPEB_WAEAHH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, int* pValue, int bSave) {
    IHTMLDocument2* pDoc = nullptr;
    pThis->GetDHtmlDocument(&pDoc);
    if (!pDoc) return;
    IHTMLOptionButtonElement* pOpt = nullptr;
    pThis->GetElementInterface(szId, IID_IHTMLOptionButtonElement, reinterpret_cast<void**>(&pOpt));
    if (pOpt) {
        if (bSave) {
            VARIANT_BOOL vbChecked = VARIANT_FALSE, vbIndeterminate = VARIANT_FALSE;
            if (SUCCEEDED(pOpt->get_checked(&vbChecked)) &&
                SUCCEEDED(pOpt->get_indeterminate(&vbIndeterminate))) {
                *pValue = (vbIndeterminate ? 2 : 0) | (vbChecked ? 1 : 0);
            }
        } else {
            if (SUCCEEDED(pOpt->put_checked((*pValue & 1) ? VARIANT_TRUE : VARIANT_FALSE)))
                pOpt->put_indeterminate((*pValue & 2) ? VARIANT_TRUE : VARIANT_FALSE);
        }
        pOpt->Release();
    }
    pDoc->Release();
}

// Symbol: ?DDX_DHtml_CheckBox@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAH@Z
extern "C" void MS_ABI impl__DDX_DHtml_CheckBox_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAH_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, int* pValue) {
    impl__DDX_DHtml_CheckBox_CDHtmlDialog__IEAAXPEB_WAEAHH_Z(pThis, szId, pValue, DHtmlSaveAndValidate(pDX));
}

// Numeric DDX_DHtml_ElementText overloads.  Each retail body (mfc140u 0x212750
// short / 0x2127a0 int / 0x2127e0 long / 0x212820 DWORD) is:
//     if (bSave) DDX_DHtml_ElementTextWithFormat(szId, dispid, <scan fmt>,  nIDPrompt, bSave, &value);
//     else       DDX_DHtml_ElementTextWithFormat(szId, dispid, <print fmt>, nIDPrompt, bSave,  value);
// with (scan fmt, print fmt, nIDPrompt):  short ("%sd", "%hd", AFX_IDP_PARSE_INT)
// -- strings 0x34c760 / 0x34c510; int ("%d", "%d", AFX_IDP_PARSE_INT) -- 0x33eefc;
// long ("%ld", "%ld", AFX_IDP_PARSE_INT) -- 0x3402f0; DWORD ("%lu", "%lu",
// AFX_IDP_PARSE_UINT) -- 0x34c518.  The float (0x212860) and double (0x212890)
// overloads call DDX_DHtml_ElementTextFloatFormat(szId, dispid, &value,
// (double)value, FLT_DIG (6) / DBL_DIG (15), bSave).
// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAFH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAFH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, short* pValue, int bSave) {
    if (bSave)
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%sd", kAfxIdpParseInt, bSave, pValue);
    else
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%hd", kAfxIdpParseInt, bSave, static_cast<int>(*pValue));
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAHH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAHH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, int* pValue, int bSave) {
    if (bSave)
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%d", kAfxIdpParseInt, bSave, pValue);
    else
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%d", kAfxIdpParseInt, bSave, *pValue);
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAJH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAJH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, long* pValue, int bSave) {
    if (bSave)
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%ld", kAfxIdpParseInt, bSave, pValue);
    else
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%ld", kAfxIdpParseInt, bSave, *pValue);
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAKH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAKH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, unsigned long* pValue, int bSave) {
    if (bSave)
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%lu", kAfxIdpParseUInt, bSave, pValue);
    else
        impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
            pThis, szId, dispid, L"%lu", kAfxIdpParseUInt, bSave, *pValue);
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAMH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAMH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, float* pValue, int bSave) {
    impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(
        pThis, szId, dispid, pValue, static_cast<double>(*pValue), FLT_DIG, bSave);
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEANH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEANH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, double* pValue, int bSave) {
    impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(
        pThis, szId, dispid, pValue, *pValue, DBL_DIG, bSave);
}

// Public CDataExchange overloads (mfc140u 0x2123e0 short / 0x212400 int /
// 0x212420 long / 0x212440 DWORD): load pDX->m_bSaveAndValidate and tail-jump
// to the protected overload.  The float (0x212460) and double (0x2124a0) ones
// call DDX_DHtml_ElementTextFloatFormat directly with FLT_DIG / DBL_DIG.
// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAF@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAF_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, short* pValue) {
    impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAFH_Z(pThis, szId, dispid, pValue, DHtmlSaveAndValidate(pDX));
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAH_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, int* pValue) {
    impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAHH_Z(pThis, szId, dispid, pValue, DHtmlSaveAndValidate(pDX));
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAJ@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAJ_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, long* pValue) {
    impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAJH_Z(pThis, szId, dispid, pValue, DHtmlSaveAndValidate(pDX));
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAK@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAK_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, unsigned long* pValue) {
    impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAKH_Z(pThis, szId, dispid, pValue, DHtmlSaveAndValidate(pDX));
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAM@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAM_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, float* pValue) {
    impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(
        pThis, szId, dispid, pValue, static_cast<double>(*pValue), FLT_DIG, DHtmlSaveAndValidate(pDX));
}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAN@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAN_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long dispid, double* pValue) {
    impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(
        pThis, szId, dispid, pValue, *pValue, DBL_DIG, DHtmlSaveAndValidate(pDX));
}

// CDHtmlDialog::DDX_DHtml_ElementTextFloatFormat(LPCTSTR, DISPID, void*, double,
// int, BOOL) -- mfc140u 0x2134b0, transcribed:
//     CString strText;
//     if (bSave) {
//         DDX_DHtml_ElementText(szId, dispid, strText, bSave);            ; 0x2125c0
//         // _AfxSimpleFloatParse, inlined (0x213518..0x21357e):
//         p = strText; skip ' '/'\t'; chFirst = *p;
//         d = wcstod(p, &p);                                              ; IAT 0x1802c7478
//         if (d == 0.0 && chFirst != '0')  goto error;
//         skip ' '/'\t'; if (*p != '\0')    goto error;
//         if (nSizeGcvt == FLT_DIG) *(float*)pData = (float)d; else *(double*)pData = d;
//     } else {
//         strText.Format(L"%.*g" (0x34c628), nSizeGcvt, value);           ; CStringT::Format 0xda00
//         DDX_DHtml_ElementText(szId, dispid, strText, FALSE);
//     }
//     return;
//   error: OnDDXError(szId, AFX_IDP_PARSE_REAL, bSave);                   ; 0x213230, noreturn in retail
// Retail's OnDDXError ends in AfxThrowUserException; OpenMFC's (earlier in this
// file) reports and returns, so control simply falls out of the function here.
// Symbol: ?DDX_DHtml_ElementTextFloatFormat@CDHtmlDialog@@IEAAXPEB_WJPEAXNHH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, void* pData, double value,
    int nSizeGcvt, int bSave) {
    CString strText;
    if (bSave) {
        impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pThis, szId, dispid, &strText, bSave);
        const wchar_t* p = strText.GetString();
        while (*p == L' ' || *p == L'\t') p++;
        const wchar_t chFirst = p[0];
        wchar_t* pEnd = nullptr;
        const double d = wcstod(p, &pEnd);
        p = pEnd;
        bool bOk = !(d == 0.0 && chFirst != L'0');
        if (bOk) {
            while (*p == L' ' || *p == L'\t') p++;
            bOk = (*p == L'\0');
        }
        if (!bOk) {
            impl__OnDDXError_CDHtmlDialog__IEAAXPEB_WIH_Z(pThis, szId, kAfxIdpParseReal, bSave);
            return;
        }
        if (nSizeGcvt == FLT_DIG)
            *static_cast<float*>(pData) = static_cast<float>(d);
        else
            *static_cast<double*>(pData) = d;
    } else {
        strText.Format(L"%.*g", nSizeGcvt, value);
        impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pThis, szId, dispid, &strText, FALSE);
    }
}

// CDHtmlDialog::DDX_DHtml_ElementTextWithFormat(LPCTSTR, DISPID, LPCTSTR, UINT,
// BOOL, ...) -- mfc140u 0x2132b0, transcribed:
//     va_list pData; va_start(pData, bSave);
//     CString strText;
//     if (bSave) {
//         DDX_DHtml_ElementText(szId, dispid, strText, bSave);            ; 0x2125c0
//         if (!_AfxSimpleScanf(strText, lpszFormat, pData)) {             ; inlined, see DHtmlSimpleScanf
//             SetFocusToElement(szId);                                    ; 0x213890
//             OnDDXError(szId, nIDPrompt, bSave);                         ; 0x213230, noreturn in retail
//         }
//     } else {
//         strText.FormatV(lpszFormat, pData);                             ; CStringT::FormatV 0xd8a0
//         DDX_DHtml_ElementText(szId, dispid, strText, bSave);
//     }
// As with the float variant, OpenMFC's OnDDXError returns, so the error path
// falls out of the function instead of unwinding.
// Symbol: ?DDX_DHtml_ElementTextWithFormat@CDHtmlDialog@@IEAAXPEB_WJ0IHZZ
extern "C" void MS_ABI impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(
    CDHtmlDialog* pThis, const wchar_t* szId, long dispid, const wchar_t* lpszFormat,
    unsigned int nIDPrompt, int bSave, ...) {
    va_list pData;
    va_start(pData, bSave);
    CString strText;
    if (bSave) {
        impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pThis, szId, dispid, &strText, bSave);
        if (!DHtmlSimpleScanf(strText.GetString(), lpszFormat, pData)) {
            impl__SetFocusToElement_CDHtmlDialog__IEAAXPEB_W_Z(pThis, szId);
            impl__OnDDXError_CDHtmlDialog__IEAAXPEB_WIH_Z(pThis, szId, nIDPrompt, bSave);
        }
    } else {
        strText.FormatV(lpszFormat, pData);
        impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pThis, szId, dispid, &strText, bSave);
    }
    va_end(pData);
}

// CDHtmlDialog::DDX_DHtml_Radio(LPCTSTR, long&, BOOL) -- mfc140u 0x212a10,
// transcribed:
//     CComVariant varUnused;                                   ; memset/VariantClear only
//     CComPtr<IHTMLDocument2> spDoc; GetDHtmlDocument(&spDoc); ; sink slot 9
//     if (spDoc) {
//         CComPtr<IHTMLOptionButtonElement> spOpt; CComPtr<IDispatch> spdispColl, spdispItem;
//         CComPtr<IHTMLElementCollection> spColl; BOOL bIsCollection = FALSE;
//         CComVariant varIndex(VT_I4, 0), varEmpty;
//         GetElement(szId, &spdispColl, &bIsCollection);       ; 0x214290
//         if (spdispColl && bIsCollection) {
//             spdispColl->QueryInterface(IID_IHTMLElementCollection (0x34c7e8), &spColl);
//             if (spColl) {
//                 if (bSave) value = -1;
//                 long nCount = 0; spColl->get_length(&nCount);              ; slot 9 (+0x48)
//                 for (long i = 0; i < nCount; i++) {
//                     spdispItem.Release(); spOpt.Release();
//                     varIndex.lVal = i;
//                     spColl->item(varIndex, varEmpty, &spdispItem);         ; slot 11 (+0x58)
//                     if (!spdispItem) break;
//                     spdispItem->QueryInterface(IID_IHTMLOptionButtonElement (0x34c7f8), &spOpt);
//                     if (bSave) {
//                         VARIANT_BOOL vb; if (FAILED(spOpt->get_checked(&vb))) break;   ; slot 13
//                         if (vb) { value = varIndex.lVal; break; }
//                     } else if (varIndex.lVal == value) {
//                         spOpt->put_checked(VARIANT_TRUE); break;           ; slot 12
//                     }
//                 }
//             }
//         }
//     }
// Deviations: retail does not test spOpt after the QueryInterface and would
// fault on a non-radio member of the collection; this body skips such an item
// (continues the loop).  The element lookup goes through the file-local
// DHtmlGetElementByName (retail GetElement 0x214290) because this file's
// CDHtmlDialog::GetElement never reports a collection -- see its comment.
// Symbol: ?DDX_DHtml_Radio@CDHtmlDialog@@IEAAXPEB_WAEAJH@Z
extern "C" void MS_ABI impl__DDX_DHtml_Radio_CDHtmlDialog__IEAAXPEB_WAEAJH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long* pValue, int bSave) {
    IHTMLDocument2* pDoc = nullptr;
    pThis->GetDHtmlDocument(&pDoc);
    if (!pDoc) return;

    IDispatch* pdispColl = nullptr;
    IHTMLElementCollection* pColl = nullptr;
    BOOL bIsCollection = FALSE;
    VARIANT varIndex, varEmpty;
    VariantInit(&varIndex);
    VariantInit(&varEmpty);
    varIndex.vt = VT_I4;
    varIndex.lVal = 0;

    DHtmlGetElementByName(pThis, szId, &pdispColl, &bIsCollection);
    if (pdispColl && bIsCollection) {
        pdispColl->QueryInterface(IID_IHTMLElementCollection, reinterpret_cast<void**>(&pColl));
        if (pColl) {
            if (bSave) *pValue = -1;
            long nCount = 0;
            pColl->get_length(&nCount);
            for (long i = 0; i < nCount; i++) {
                IDispatch* pdispItem = nullptr;
                IHTMLOptionButtonElement* pOpt = nullptr;
                varIndex.lVal = i;
                pColl->item(varIndex, varEmpty, &pdispItem);
                if (!pdispItem) break;
                pdispItem->QueryInterface(IID_IHTMLOptionButtonElement, reinterpret_cast<void**>(&pOpt));
                pdispItem->Release();
                if (!pOpt) continue;                      // deviation: retail would fault here
                bool bDone = false;
                if (bSave) {
                    VARIANT_BOOL vbChecked = VARIANT_FALSE;
                    if (FAILED(pOpt->get_checked(&vbChecked))) {
                        bDone = true;
                    } else if (vbChecked) {
                        *pValue = varIndex.lVal;
                        bDone = true;
                    }
                } else if (varIndex.lVal == *pValue) {
                    pOpt->put_checked(VARIANT_TRUE);
                    bDone = true;
                }
                pOpt->Release();
                if (bDone) break;
            }
            pColl->Release();
        }
    }
    VariantClear(&varEmpty);
    VariantClear(&varIndex);
    if (pdispColl) pdispColl->Release();
    pDoc->Release();
}

// Symbol: ?DDX_DHtml_Radio@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAJ@Z
extern "C" void MS_ABI impl__DDX_DHtml_Radio_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAJ_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long* pValue) {
    impl__DDX_DHtml_Radio_CDHtmlDialog__IEAAXPEB_WAEAJH_Z(pThis, szId, pValue, DHtmlSaveAndValidate(pDX));
}

// CDHtmlDialog::DDX_DHtml_SelectIndex(LPCTSTR, long&, BOOL) -- mfc140u 0x2130e0,
// transcribed:
//     CComPtr<IHTMLDocument2> spDoc; GetDHtmlDocument(&spDoc);          ; sink slot 9
//     if (spDoc) {
//         CComPtr<IHTMLSelectElement> spSelect;
//         GetElementInterface(szId, IID_IHTMLSelectElement (0x34c7d8), &spSelect);   ; 0x2145e0
//         if (spSelect) {
//             if (bSave) spSelect->get_selectedIndex(&value);            ; slot 17 (+0x88)
//             else       spSelect->put_selectedIndex(value);             ; slot 16 (+0x80)
//         }
//     }
// Symbol: ?DDX_DHtml_SelectIndex@CDHtmlDialog@@IEAAXPEB_WAEAJH@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectIndex_CDHtmlDialog__IEAAXPEB_WAEAJH_Z(
    CDHtmlDialog* pThis, const wchar_t* szId, long* pValue, int bSave) {
    IHTMLDocument2* pDoc = nullptr;
    pThis->GetDHtmlDocument(&pDoc);
    if (!pDoc) return;
    IHTMLSelectElement* pSelect = nullptr;
    pThis->GetElementInterface(szId, IID_IHTMLSelectElement, reinterpret_cast<void**>(&pSelect));
    if (pSelect) {
        if (bSave)
            pSelect->get_selectedIndex(pValue);
        else
            pSelect->put_selectedIndex(*pValue);
        pSelect->Release();
    }
    pDoc->Release();
}

// Symbol: ?DDX_DHtml_SelectIndex@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAJ@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectIndex_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAJ_Z(
    CDHtmlDialog* pThis, void* pDX, const wchar_t* szId, long* pValue) {
    impl__DDX_DHtml_SelectIndex_CDHtmlDialog__IEAAXPEB_WAEAJH_Z(pThis, szId, pValue, DHtmlSaveAndValidate(pDX));
}

// CDHtmlDialog::DestroyModeless() -- mfc140u 0x211c80, transcribed:
//     if (WM_DESTROYMODELESS == 0)
//         WM_DESTROYMODELESS = ::RegisterWindowMessage(L"DHtmlDialogDestroy" (0x34c710));  ; IAT 0x1802c7270
//     ::PostMessage(m_hWnd /* +0x40 */, WM_DESTROYMODELESS, 0, 0);                        ; IAT 0x1802c72b0 (tail-jump)
// Symbol: ?DestroyModeless@CDHtmlDialog@@QEAAXXZ
extern "C" void MS_ABI impl__DestroyModeless_CDHtmlDialog__QEAAXXZ(CDHtmlDialog* pThis) {
    if (g_wmDestroyModeless == 0)
        g_wmDestroyModeless = ::RegisterWindowMessage(L"DHtmlDialogDestroy");
    ::PostMessage(pThis->m_hWnd, g_wmDestroyModeless, 0, 0);
}

// CDHtmlDialog::DisconnectDHtmlElementEvents() -- mfc140u 0x214120.  Retail:
//     if (GetDHtmlEventMap() /* sink slot 7 (+0x38) on the +0x130 vptr */) {
//         for each CDHtmlElementEventSink* in m_SinkedElements (+0x278/+0x280):
//             Unadvise(sink->m_spunkElem /* +0x10 */, IID_IDispatch (0x34c848), sink->m_dwCookie /* +0x18 */);
//             delete sink;                     ; virtual deleting dtor, slot 7 (+0x38), flag 1
//         free(array) (IAT 0x1802c74e8); m_SinkedElements = {};
//         for each CDHtmlControlSink* in m_ControlSinks (+0x268/+0x270):
//             Unadvise(sink->m_spunkObj /* +0x18 */, sink->m_iid /* +0x20 */, sink->m_dwCookie /* +0x10 */);
//             if (sink) delete sink;           ; slot 7 (+0x38), flag 1
//         free(array); m_ControlSinks = {};
//     }
// "Unadvise" here is the non-exported helper at 0x2156d0 (punk, REFIID, DWORD
// cookie by value: QI IConnectionPointContainer, FindConnectionPoint,
// IConnectionPoint::Unadvise); the exported
// CDHtmlEventSink::DisconnectFromConnectionPoint (0x20f7a0) is only a
// register shuffle that dereferences its DWORD& and tail-jumps to it.
// Neither array nor the sink subobject exists in the OpenMFC layout, and
// ConnectDHtmlElementEvents above never creates a sink, so there is nothing to
// disconnect; left a stub.
// Symbol: ?DisconnectDHtmlElementEvents@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__DisconnectDHtmlElementEvents_CDHtmlDialog__IEAAXXZ(CDHtmlDialog* pThis) {
    (void)pThis;
}

// CDHtmlDialog::DisconnectDHtmlEvents() -- mfc140u 0x213a20.  Retail:
//     CComPtr<IHTMLDocument2> spDoc; GetDHtmlDocument(&spDoc);          ; sink slot 9 (+0x48)
//     if (spDoc) {
//         DWORD dwCookie = m_dwDHtmlEventSinkCookie;                    ; +0x260, read up front
//         if (SUCCEEDED(spDoc->QueryInterface(IID_IConnectionPointContainer (0x34c858), &spCPC))
//          && SUCCEEDED(spCPC->FindConnectionPoint(DIID_HTMLDocumentEvents (0x34c7b8), &spCP)))  ; slot 4 (+0x20)
//             spCP->Unadvise(dwCookie);                                 ; slot 6 (+0x30), HRESULT dropped
//         spCP.Release(); spCPC.Release();
//         DisconnectDHtmlElementEvents();                               ; 0x214120, runs even if the QI/Find failed
//     }
// ConnectDHtmlEvents never Advise()s here (no sink subobject, no cookie
// member), so there is no cookie to Unadvise; left a stub.
// Symbol: ?DisconnectDHtmlEvents@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__DisconnectDHtmlEvents_CDHtmlDialog__IEAAXXZ(CDHtmlDialog* pThis) {
    (void)pThis;
}

// CDHtmlDialog::GetDHtmlEventMap() -- mfc140u export ordinal 5224 resolves
// (export-address table, read 2026-09-20) to RVA 0x71e0, whose whole body is
// `xor %eax,%eax ; ret`; it is ICF-folded with other trivial returners, which
// is why the RVA symbol map has no entry for it.  The CDHtmlEventSink vtable
// the CDHtmlDialog ctor installs at +0x130 (mfc140u 0x326e30, store at
// 0x2110d4 inside ??0CDHtmlDialog@@QEAA@XZ) carries the same 0x71e0 in slot 7.
// So the base dialog contributes no DHTML event map and `return NULL` is the
// complete retail body, not a placeholder.  (The free export
// ?GetDHtmlEventMap@@YAPEBUDHtmlEventMapEntry@@XZ at 0x212020, which builds a
// one-entry DISPID_READYSTATECHANGE map, is a different function and is not
// what the CDHtmlDialog vtable points at.)
// Symbol: ?GetDHtmlEventMap@CDHtmlDialog@@MEAAPEBUDHtmlEventMapEntry@@XZ
extern "C" const void* MS_ABI impl__GetDHtmlEventMap_CDHtmlDialog__MEAAPEBUDHtmlEventMapEntry__XZ(CDHtmlDialog* pThis) {
    (void)pThis;
    return nullptr;
}

// CDHtmlDialog::GetEventSinkMap() const -- mfc140u 0x20f760:
//     lea 0x1803276b0,%rax ; ret
// GetThisEventSinkMap() resolves (ordinal 7192) to the same 0x20f760 body.
// Both return the map reproduced above as g_dhtmlEventSinkMap.
// Symbol: ?GetEventSinkMap@CDHtmlDialog@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetEventSinkMap_CDHtmlDialog__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CDHtmlDialog* pThis) {
    (void)pThis;
    return &g_dhtmlEventSinkMap;
}

// Symbol: ?GetThisEventSinkMap@CDHtmlDialog@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CDHtmlDialog__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return &g_dhtmlEventSinkMap;
}

// CDHtmlDialog::Initialize() -- mfc140u 0x212090, transcribed (retail offsets):
//     m_dwHostFlags (+0x288) = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO (0xc);
//     m_bUseHtmlTitle (+0x168) = FALSE; m_bAttachedControl (+0x16c) = FALSE;   ; one 8-byte store
//     if (m_pBrowserApp (+0x158)) { m_pBrowserApp = NULL; old->Release(); }
//     m_dwDHtmlEventSinkCookie (+0x260) = 0;
//     m_szHtmlResID (+0x148) = NULL;
//     m_nHtmlResID (+0x150) = 0;
// Mapped onto the OpenMFC members: m_dwHostFlags, m_pBrowser, m_nHtmlResID and
// m_strHtmlResID (a CString here where retail keeps an LPTSTR, so it is
// emptied rather than nulled).  m_bUseHtmlTitle, m_bAttachedControl and the
// sink cookie have no counterpart in this layout and are not represented.
// Symbol: ?Initialize@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CDHtmlDialog__IEAAXXZ(CDHtmlDialog* pThis) {
    pThis->m_dwHostFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO;
    if (pThis->m_pBrowser) {
        IWebBrowser2* pOld = pThis->m_pBrowser;
        pThis->m_pBrowser = nullptr;
        pOld->Release();
    }
    pThis->m_strHtmlResID.Empty();
    pThis->m_nHtmlResID = 0;
}

// CDHtmlDialog::IsSinkedElement(IDispatch*) -- mfc140u 0x214070.  Retail:
//     if (!pdispElem) AfxThrowInvalidArgException();
//     pdispElem->QueryInterface(IID_IUnknown (0x34c838), &spUnk);    ; COM identity
//     if (!spUnk) return FALSE;
//     for i in [0, m_SinkedElements.GetSize() /* +0x280 */):
//         if (m_SinkedElements[i] /* +0x278 */->m_spunkElem /* +0x10 */ == spUnk) return TRUE;
//     return FALSE;
// m_SinkedElements does not exist in this layout and nothing is ever sinked
// (see ConnectDHtmlElementEvents), so the only reachable answer is FALSE.
// Symbol: ?IsSinkedElement@CDHtmlDialog@@IEAAHPEAUIDispatch@@@Z
extern "C" int MS_ABI impl__IsSinkedElement_CDHtmlDialog__IEAAHPEAUIDispatch___Z(
    CDHtmlDialog* pThis, IDispatch* pdispElem) {
    (void)pThis; (void)pdispElem;
    return FALSE;
}

// CDHtmlDialog::OnDestroyModeless(WPARAM, LPARAM) -- mfc140u 0x2123a0,
// transcribed:
//     this->vtable[0xd0/8]();     ; virtual CWnd::DestroyWindow (slot 26)
//     return 0;
// Retail dispatches DestroyWindow virtually; OpenMFC's C++ vtable is unrelated
// to retail's, so the CWnd::DestroyWindow thunk is called directly (the same
// convention CMFCPopupMenuBar.cpp uses for this slot).
// Symbol: ?OnDestroyModeless@CDHtmlDialog@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnDestroyModeless_CDHtmlDialog__IEAA_J_K_J_Z(
    CDHtmlDialog* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam; (void)lParam;
    impl__DestroyWindow_CWnd__UEAAHXZ(pThis);
    return 0;
}

// CDHtmlDialog::OnDocumentReadyStateChange(IHTMLElement*) -- mfc140u 0x212250,
// transcribed (the IHTMLElement argument is never read):
//     CComPtr<IHTMLDocument2> spDoc; GetDHtmlDocument(&spDoc);           ; sink slot 9
//     if (spDoc) {
//         BSTR bstrState = NULL; spDoc->get_readyState(&bstrState);       ; slot 22 (+0xb0)
//         if (bstrState) {
//             BSTR b = SysAllocString(L"complete" (0x34c738));            ; OLEAUT32 #2; NULL -> AfxThrowOleException(E_OUTOFMEMORY)
//             int cmp = VarBstrCmp(bstrState, b, LOCALE_USER_DEFAULT, 0); ; OLEAUT32 #314
//             SysFreeString(b);                                           ; OLEAUT32 #6
//             if (cmp == VARCMP_EQ)
//                 ConnectDHtmlElementEvents((CDHtmlEventSink*)this - this /* 0x130 */);   ; 0x213b20
//             else {
//                 b = SysAllocString(L"loading" (0x34c750)); cmp = VarBstrCmp(...); SysFreeString(b);
//                 if (cmp == VARCMP_EQ) DisconnectDHtmlElementEvents();  ; 0x214120
//             }
//             SysFreeString(bstrState);
//         }
//     }
//     return S_OK;
// Deviations: a failed SysAllocString returns E_OUTOFMEMORY instead of
// throwing; the thunk offset passed to ConnectDHtmlElementEvents is 0 because
// this layout has no CDHtmlEventSink subobject (and that callee is a stub).
// Symbol: ?OnDocumentReadyStateChange@CDHtmlDialog@@QEAAJPEAUIHTMLElement@@@Z
extern "C" long MS_ABI impl__OnDocumentReadyStateChange_CDHtmlDialog__QEAAJPEAUIHTMLElement___Z(
    CDHtmlDialog* pThis, IHTMLElement* pElement) {
    (void)pElement;
    IHTMLDocument2* pDoc = nullptr;
    pThis->GetDHtmlDocument(&pDoc);
    if (!pDoc) return S_OK;
    BSTR bstrState = nullptr;
    pDoc->get_readyState(&bstrState);
    HRESULT hr = S_OK;
    if (bstrState) {
        BSTR bstrComplete = ::SysAllocString(L"complete");
        if (!bstrComplete) {
            hr = E_OUTOFMEMORY;
        } else {
            const HRESULT cmp = ::VarBstrCmp(bstrState, bstrComplete, LOCALE_USER_DEFAULT, 0);
            ::SysFreeString(bstrComplete);
            if (cmp == VARCMP_EQ) {
                impl__ConnectDHtmlElementEvents_CDHtmlDialog__IEAAJ_K_Z(pThis, 0);
            } else {
                BSTR bstrLoading = ::SysAllocString(L"loading");
                if (!bstrLoading) {
                    hr = E_OUTOFMEMORY;
                } else {
                    const HRESULT cmp2 = ::VarBstrCmp(bstrState, bstrLoading, LOCALE_USER_DEFAULT, 0);
                    ::SysFreeString(bstrLoading);
                    if (cmp2 == VARCMP_EQ)
                        impl__DisconnectDHtmlElementEvents_CDHtmlDialog__IEAAXXZ(pThis);
                }
            }
        }
        ::SysFreeString(bstrState);
    }
    pDoc->Release();
    return hr;
}

// CDHtmlDialog::OnSize(UINT, int, int) -- mfc140u 0x211dc0, transcribed:
//     CWnd::Default();                                                   ; 0x28ac80
//     if (!m_bAttachedControl /* +0x16c */ && m_wndBrowser.m_hWnd /* +0x178+0x40 */)
//         m_wndBrowser.MoveWindow(0, 0, cx, cy, TRUE);                   ; 0x2a9a10
// OpenMFC does not wrap the WebBrowser control's HWND in a CWnd (m_pCtrlWnd is
// never populated) and never attaches a template control, so the second half
// is rendered as: take the in-place window of m_pBrowser (IOleInPlaceObject::
// GetWindow) and ::MoveWindow it to (0,0,cx,cy) -- the same Win32 call retail
// makes on m_wndBrowser's handle.  Deviation from retail noted accordingly.
// Symbol: ?OnSize@CDHtmlDialog@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CDHtmlDialog__IEAAXIHH_Z(
    CDHtmlDialog* pThis, unsigned int nType, int cx, int cy) {
    (void)nType;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (!pThis->m_pBrowser) return;
    IOleInPlaceObject* pInPlace = nullptr;
    if (SUCCEEDED(pThis->m_pBrowser->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&pInPlace))) && pInPlace) {
        HWND hwndBrowser = nullptr;
        if (SUCCEEDED(pInPlace->GetWindow(&hwndBrowser)) && hwndBrowser)
            ::MoveWindow(hwndBrowser, 0, 0, cx, cy, TRUE);
        pInPlace->Release();
    }
}
