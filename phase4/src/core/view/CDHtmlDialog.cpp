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
// Symbol: ?GetControlProperty@CDHtmlDialog@@QEAA?AUtagVARIANT@@PEAUIDispatch@@J@Z
extern "C" void* MS_ABI impl__GetControlProperty_CDHtmlDialog__QEAA_AUtagVARIANT__PEAUIDispatch__J_Z(void* pThis, void* p0, void* p1, void* p2) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    return nullptr;
}

// Symbol: ?ConnectDHtmlElementEvents@CDHtmlDialog@@IEAAJ_K@Z
extern "C" long MS_ABI impl__ConnectDHtmlElementEvents_CDHtmlDialog__IEAAJ_K_Z(unsigned __int64 p0) {
    return 0;
}

// Symbol: ?ConnectDHtmlEvents@CDHtmlDialog@@IEAAJPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__ConnectDHtmlEvents_CDHtmlDialog__IEAAJPEAUIUnknown___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@IEAAXPEB_W0AEAUtagVARIANT@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__IEAAXPEB_W0AEAUtagVARIANT__H_Z(const wchar_t* p0, const wchar_t* p1, void* /*struct*/* p2, int p3) {}

// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@IEAAXPEB_WJAEAUtagVARIANT@@H@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__IEAAXPEB_WJAEAUtagVARIANT__H_Z(const wchar_t* p0, long p1, void* /*struct*/* p2, int p3) {}

// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_W1AEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_W1AEAUtagVARIANT___Z(void* /*class*/* p0, const wchar_t* p1, const wchar_t* p2, void* /*struct*/* p3) {}

// Symbol: ?DDX_DHtml_AxControl@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__DDX_DHtml_AxControl_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAUtagVARIANT___Z(void* /*class*/* p0, const wchar_t* p1, long p2, void* /*struct*/* p3) {}

// Symbol: ?DDX_DHtml_CheckBox@CDHtmlDialog@@IEAAXPEB_WAEAHH@Z
extern "C" void MS_ABI impl__DDX_DHtml_CheckBox_CDHtmlDialog__IEAAXPEB_WAEAHH_Z(const wchar_t* p0, int* p1, int p2) {}

// Symbol: ?DDX_DHtml_CheckBox@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAH@Z
extern "C" void MS_ABI impl__DDX_DHtml_CheckBox_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAFH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAFH_Z(const wchar_t* p0, long p1, short* p2, int p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAHH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAHH_Z(const wchar_t* p0, long p1, int* p2, int p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAJH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAJH_Z(const wchar_t* p0, long p1, long* p2, int p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAKH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAKH_Z(const wchar_t* p0, long p1, unsigned long* p2, int p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEAMH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEAMH_Z(const wchar_t* p0, long p1, float* p2, int p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@IEAAXPEB_WJAEANH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__IEAAXPEB_WJAEANH_Z(const wchar_t* p0, long p1, double* p2, int p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAF@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAF_Z(void* /*class*/* p0, const wchar_t* p1, long p2, short* p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAH_Z(void* /*class*/* p0, const wchar_t* p1, long p2, int* p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAJ@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAJ_Z(void* /*class*/* p0, const wchar_t* p1, long p2, long* p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAK@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAK_Z(void* /*class*/* p0, const wchar_t* p1, long p2, unsigned long* p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAM@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAM_Z(void* /*class*/* p0, const wchar_t* p1, long p2, float* p3) {}

// Symbol: ?DDX_DHtml_ElementText@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WJAEAN@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementText_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WJAEAN_Z(void* /*class*/* p0, const wchar_t* p1, long p2, double* p3) {}

// Symbol: ?DDX_DHtml_ElementTextFloatFormat@CDHtmlDialog@@IEAAXPEB_WJPEAXNHH@Z
extern "C" void MS_ABI impl__DDX_DHtml_ElementTextFloatFormat_CDHtmlDialog__IEAAXPEB_WJPEAXNHH_Z(const wchar_t* p0, long p1, void* p2, double p3, int p4, int p5) {}

// Symbol: ?DDX_DHtml_ElementTextWithFormat@CDHtmlDialog@@IEAAXPEB_WJ0IHZZ
extern "C" void MS_ABI impl__DDX_DHtml_ElementTextWithFormat_CDHtmlDialog__IEAAXPEB_WJ0IHZZ(const wchar_t* p0, long p1, const wchar_t* p2, unsigned int p3, int p4) {}

// Symbol: ?DDX_DHtml_Radio@CDHtmlDialog@@IEAAXPEB_WAEAJH@Z
extern "C" void MS_ABI impl__DDX_DHtml_Radio_CDHtmlDialog__IEAAXPEB_WAEAJH_Z(const wchar_t* p0, long* p1, int p2) {}

// Symbol: ?DDX_DHtml_Radio@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAJ@Z
extern "C" void MS_ABI impl__DDX_DHtml_Radio_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAJ_Z(void* /*class*/* p0, const wchar_t* p1, long* p2) {}

// Symbol: ?DDX_DHtml_SelectIndex@CDHtmlDialog@@IEAAXPEB_WAEAJH@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectIndex_CDHtmlDialog__IEAAXPEB_WAEAJH_Z(const wchar_t* p0, long* p1, int p2) {}

// Symbol: ?DDX_DHtml_SelectIndex@CDHtmlDialog@@QEAAXPEAVCDataExchange@@PEB_WAEAJ@Z
extern "C" void MS_ABI impl__DDX_DHtml_SelectIndex_CDHtmlDialog__QEAAXPEAVCDataExchange__PEB_WAEAJ_Z(void* /*class*/* p0, const wchar_t* p1, long* p2) {}

// Symbol: ?DestroyModeless@CDHtmlDialog@@QEAAXXZ
extern "C" void MS_ABI impl__DestroyModeless_CDHtmlDialog__QEAAXXZ() {}

// Symbol: ?DisconnectDHtmlElementEvents@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__DisconnectDHtmlElementEvents_CDHtmlDialog__IEAAXXZ() {}

// Symbol: ?DisconnectDHtmlEvents@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__DisconnectDHtmlEvents_CDHtmlDialog__IEAAXXZ() {}

// Symbol: ?GetDHtmlEventMap@CDHtmlDialog@@MEAAPEBUDHtmlEventMapEntry@@XZ
extern "C" void* MS_ABI impl__GetDHtmlEventMap_CDHtmlDialog__MEAAPEBUDHtmlEventMapEntry__XZ() {
    return nullptr;
}

// Symbol: ?GetEventSinkMap@CDHtmlDialog@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" void* MS_ABI impl__GetEventSinkMap_CDHtmlDialog__MEBAPEBUAFX_EVENTSINKMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetThisEventSinkMap@CDHtmlDialog@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" void* MS_ABI impl__GetThisEventSinkMap_CDHtmlDialog__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return nullptr;
}

// Symbol: ?Initialize@CDHtmlDialog@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CDHtmlDialog__IEAAXXZ() {}

// Symbol: ?IsSinkedElement@CDHtmlDialog@@IEAAHPEAUIDispatch@@@Z
extern "C" int MS_ABI impl__IsSinkedElement_CDHtmlDialog__IEAAHPEAUIDispatch___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroyModeless@CDHtmlDialog@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnDestroyModeless_CDHtmlDialog__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnDocumentReadyStateChange@CDHtmlDialog@@QEAAJPEAUIHTMLElement@@@Z
extern "C" long MS_ABI impl__OnDocumentReadyStateChange_CDHtmlDialog__QEAAJPEAUIHTMLElement___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnSize@CDHtmlDialog@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CDHtmlDialog__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}
