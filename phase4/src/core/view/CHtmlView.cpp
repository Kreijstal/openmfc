// CHtmlView — OpenMFC implementation.
// Sources: viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ViewrichSupport.h"

// Symbol: ??0CHtmlView@@IEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlView__IEAA_XZ(CHtmlView* pThis) {
    new (pThis) CHtmlView();
    return pThis;
}
// Symbol: ??1CHtmlView@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlView__UEAA_XZ(CHtmlView* pThis) {
    pThis->~CHtmlView();
}
// Symbol: ?Create@CHtmlView@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CHtmlView__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    CHtmlView* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext) {
    return pThis->CHtmlView::Create(lpszClassName, lpszWindowName, dwStyle, rect,
                                     pParentWnd, nID, pContext);
}
// Symbol: ?OnDraw@CHtmlView@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CHtmlView__UEAAXPEAVCDC___Z(CHtmlView* pThis, CDC* pDC) {
    pThis->CHtmlView::OnDraw(pDC);
}
// Symbol: ?Navigate@CHtmlView@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                    lpvPostData, dwPostDataLen);
}
// Symbol: ?Navigate2@CHtmlView@@QEAAXPEB_WK00PEAXK@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEB_WK00PEAXK_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName, const wchar_t* lpszHeaders,
    void* lpvPostData, unsigned long dwPostDataLen) {
    pThis->Navigate2(lpszURL, dwFlags, lpszTargetFrameName, lpszHeaders,
                     lpvPostData, dwPostDataLen);
}
// Symbol: ?Navigate2@CHtmlView@@QEAAXPEFAU_ITEMIDLIST@@KPEB_W@Z
extern "C" void MS_ABI impl__Navigate2_CHtmlView__QEAAXPEFAU_ITEMIDLIST__KPEB_W_Z(
    CHtmlView* pThis, LPITEMIDLIST pIDL, unsigned long dwFlags,
    const wchar_t* lpszTargetFrameName) {
    pThis->Navigate2(pIDL, dwFlags, lpszTargetFrameName);
}
// Symbol: ?GoBack@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoBack_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoBack(); }
// Symbol: ?GoForward@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoForward_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoForward(); }
// Symbol: ?GoHome@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoHome_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoHome(); }
// Symbol: ?GoSearch@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__GoSearch_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->GoSearch(); }
// Symbol: ?Stop@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__Stop_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->Stop(); }
// Symbol: ?Refresh@CHtmlView@@QEAAXXZ
extern "C" void MS_ABI impl__Refresh_CHtmlView__QEAAXXZ(CHtmlView* pThis) { pThis->Refresh(); }
// Symbol: ?Refresh2@CHtmlView@@QEAAXH@Z
extern "C" void MS_ABI impl__Refresh2_CHtmlView__QEAAXH_Z(CHtmlView* pThis, int nLevel) {
    pThis->Refresh2(nLevel);
}
// Symbol: ?GetBusy@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetBusy_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetBusy();
}
// Symbol: ?GetReadyState@CHtmlView@@QEBA?AW4tagREADYSTATE@@XZ
extern "C" READYSTATE MS_ABI impl__GetReadyState_CHtmlView__QEBA_AW4tagREADYSTATE__XZ(
    const CHtmlView* pThis) {
    return pThis->GetReadyState();
}
// Symbol: ?GetLocationName@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocationName_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetLocationName());
}
// Symbol: ?GetLocationURL@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocationURL_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetLocationURL());
}
// Symbol: ?GetFullName@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFullName_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetFullName());
}
// Symbol: ?GetType@CHtmlView@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetType_CHtmlView__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHtmlView* pThis) {
    new (pRet) CString(pThis->GetType());
}
// Symbol: ?GetHtmlDocument@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetHtmlDocument_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetHtmlDocument();
}
// Symbol: ?GetApplication@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetApplication_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetApplication();
}
// Symbol: ?GetContainer@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetContainer_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetContainer();
}
// Symbol: ?GetParentBrowser@CHtmlView@@QEBAPEAUIDispatch@@XZ
extern "C" IDispatch* MS_ABI impl__GetParentBrowser_CHtmlView__QEBAPEAUIDispatch__XZ(
    const CHtmlView* pThis) {
    return pThis->GetParentBrowser();
}
// Symbol: ?GetLeft@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetLeft_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetLeft();
}
// Symbol: ?GetTop@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetTop_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetTop();
}
// Symbol: ?GetWidth@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetWidth_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetWidth();
}
// Symbol: ?GetHeight@CHtmlView@@QEBAJXZ
extern "C" long MS_ABI impl__GetHeight_CHtmlView__QEBAJXZ(const CHtmlView* pThis) {
    return pThis->GetHeight();
}
// Symbol: ?GetVisible@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetVisible_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetVisible();
}
// Symbol: ?GetFullScreen@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetFullScreen_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetFullScreen();
}
// Symbol: ?GetMenuBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetMenuBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetMenuBar();
}
// Symbol: ?GetAddressBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetAddressBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetAddressBar();
}
// Symbol: ?GetStatusBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetStatusBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetStatusBar();
}
// Symbol: ?GetToolBar@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetToolBar_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetToolBar();
}
// Symbol: ?GetOffline@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetOffline_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetOffline();
}
// Symbol: ?GetSilent@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetSilent_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetSilent();
}
// Symbol: ?GetTheaterMode@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetTheaterMode_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetTheaterMode();
}
// Symbol: ?GetRegisterAsBrowser@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetRegisterAsBrowser_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetRegisterAsBrowser();
}
// Symbol: ?GetRegisterAsDropTarget@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetRegisterAsDropTarget_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetRegisterAsDropTarget();
}
// Symbol: ?GetTopLevelContainer@CHtmlView@@QEBAHXZ
extern "C" int MS_ABI impl__GetTopLevelContainer_CHtmlView__QEBAHXZ(const CHtmlView* pThis) {
    return pThis->GetTopLevelContainer();
}
// Symbol: ?ExecWB@CHtmlView@@QEAAXW4OLECMDID@@W4OLECMDEXECOPT@@PEAUtagVARIANT@@2@Z
extern "C" void MS_ABI impl__ExecWB_CHtmlView__QEAAXW4OLECMDID__W4OLECMDEXECOPT__PEAUtagVARIANT__2_Z(
    CHtmlView* pThis, OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
    VARIANT* pvaIn, VARIANT* pvaOut) {
    pThis->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}
// Symbol: ?LoadFromResource@CHtmlView@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CHtmlView__QEAAHPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszResource) {
    return pThis->LoadFromResource(lpszResource);
}
// Symbol: ?LoadFromResource@CHtmlView@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadFromResource_CHtmlView__QEAAHI_Z(
    CHtmlView* pThis, unsigned int nRes) {
    return pThis->LoadFromResource(nRes);
}
// Symbol: ?Print@CHtmlView@@QEAAXXZ (not in mapping but keep for completeness)

// Symbol: ?GetProperty@CHtmlView@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetProperty_CHtmlView__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CHtmlView* pThis, const wchar_t* lpszProperty, CString& strValue) {
    return pThis->GetProperty(lpszProperty, strValue);
}
// Symbol: ?PutProperty@CHtmlView@@QEAAXPEB_WAEBUtagVARIANT@@@Z
extern "C" void MS_ABI impl__PutProperty_CHtmlView__QEAAXPEB_WAEBUtagVARIANT___Z(
    CHtmlView* pThis, const wchar_t* lpszProperty, const VARIANT& vtValue) {
    pThis->PutProperty(lpszProperty, vtValue);
}
// Symbol: ?GetSource@CHtmlView@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetSource_CHtmlView__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CHtmlView* pThis, CString& strRef) {
    return pThis->GetSource(strRef);
}
// Symbol: ?OnGetOptionKeyPath@CHtmlView@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__OnGetOptionKeyPath_CHtmlView__UEAAJPEAPEA_WK_Z(
    CHtmlView* pThis, wchar_t** ppszKey, unsigned long dwReserved) {
    return pThis->OnGetOptionKeyPath(ppszKey, dwReserved);
}
// CHtmlView event sink stubs
// Symbol: ?OnBeforeNavigate2@CHtmlView@@UEAAXPEB_WK0AEAVCByteArray@@0PEAH@Z
extern "C" void MS_ABI impl__OnBeforeNavigate2_CHtmlView__UEAAXPEB_WK0AEAVCByteArray__0PEAH_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, unsigned long nFlags,
    const wchar_t* lpszTargetFrameName, CByteArray& baPostData,
    const wchar_t* lpszHeaders, BOOL* pbCancel) {
    pThis->OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName,
                             baPostData, lpszHeaders, pbCancel);
}
// Symbol: ?OnNavigateComplete2@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete2_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszURL) {
    pThis->OnNavigateComplete2(lpszURL);
}
// Symbol: ?OnDocumentComplete@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnDocumentComplete_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszURL) {
    pThis->OnDocumentComplete(lpszURL);
}
// Symbol: ?OnProgressChange@CHtmlView@@UEAAXJJ@Z
extern "C" void MS_ABI impl__OnProgressChange_CHtmlView__UEAAXJJ_Z(
    CHtmlView* pThis, long lProgress, long lProgressMax) {
    pThis->OnProgressChange(lProgress, lProgressMax);
}
// Symbol: ?OnTitleChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnTitleChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszText) {
    pThis->OnTitleChange(lpszText);
}
// Symbol: ?OnStatusTextChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnStatusTextChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszText) {
    pThis->OnStatusTextChange(lpszText);
}
// Symbol: ?OnCommandStateChange@CHtmlView@@UEAAXJH@Z
extern "C" void MS_ABI impl__OnCommandStateChange_CHtmlView__UEAAXJH_Z(
    CHtmlView* pThis, long lCommand, int bEnable) {
    pThis->OnCommandStateChange(lCommand, bEnable);
}
// Symbol: ?OnDownloadBegin@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDownloadBegin_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnDownloadBegin();
}
// Symbol: ?OnDownloadComplete@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDownloadComplete_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnDownloadComplete();
}
// Symbol: ?OnFullScreen@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnFullScreen_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bFullScreen) {
    pThis->OnFullScreen(bFullScreen);
}
// Symbol: ?OnMenuBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnMenuBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bMenuBar) {
    pThis->OnMenuBar(bMenuBar);
}
// Symbol: ?OnNavigateError@CHtmlView@@UEAAXPEB_W0KPEAH@Z
extern "C" void MS_ABI impl__OnNavigateError_CHtmlView__UEAAXPEB_W0KPEAH_Z(
    CHtmlView* pThis, const wchar_t* lpszURL, const wchar_t* lpszFrame,
    unsigned long dwError, BOOL* pbCancel) {
    pThis->OnNavigateError(lpszURL, lpszFrame, dwError, pbCancel);
}
// Symbol: ?OnNewWindow2@CHtmlView@@UEAAXPEAPEAUIDispatch@@PEAH@Z
extern "C" void MS_ABI impl__OnNewWindow2_CHtmlView__UEAAXPEAPEAUIDispatch__PEAH_Z(
    CHtmlView* pThis, IDispatch** ppDisp, BOOL* bCancel) {
    pThis->OnNewWindow2(ppDisp, bCancel);
}
// Symbol: ?OnPropertyChange@CHtmlView@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnPropertyChange_CHtmlView__UEAAXPEB_W_Z(
    CHtmlView* pThis, const wchar_t* lpszProperty) {
    pThis->OnPropertyChange(lpszProperty);
}
// Symbol: ?OnQuit@CHtmlView@@UEAAXXZ
extern "C" void MS_ABI impl__OnQuit_CHtmlView__UEAAXXZ(CHtmlView* pThis) {
    pThis->OnQuit();
}
// Symbol: ?OnStatusBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnStatusBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bStatusBar) {
    pThis->OnStatusBar(bStatusBar);
}
// Symbol: ?OnTheaterMode@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnTheaterMode_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bTheaterMode) {
    pThis->OnTheaterMode(bTheaterMode);
}
// Symbol: ?OnToolBar@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnToolBar_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bToolBar) {
    pThis->OnToolBar(bToolBar);
}
// Symbol: ?OnVisible@CHtmlView@@UEAAXH@Z
extern "C" void MS_ABI impl__OnVisible_CHtmlView__UEAAXH_Z(CHtmlView* pThis, int bVisible) {
    pThis->OnVisible(bVisible);
}
// Symbol: ?OnTranslateUrl@CHtmlView@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__OnTranslateUrl_CHtmlView__UEAAJKPEA_WPEAPEA_W_Z(
    CHtmlView* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    return pThis->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}
// Symbol: ?OnUpdateEditCopy@CHtmlView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditCopy_CHtmlView__IEAAXPEAVCCmdUI___Z(CHtmlView*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateEditCut@CHtmlView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditCut_CHtmlView__IEAAXPEAVCCmdUI___Z(CHtmlView*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateEditPaste@CHtmlView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditPaste_CHtmlView__IEAAXPEAVCCmdUI___Z(CHtmlView*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
CHtmlView::CHtmlView()
    : m_pBrowser(nullptr), m_pControlWnd(nullptr), m_bCreated(FALSE) {
    memset(_htmlview_padding, 0, sizeof(_htmlview_padding));
}
CHtmlView::~CHtmlView() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
        m_pBrowser->Release();
        m_pBrowser = nullptr;
    }
}
BOOL CHtmlView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                       DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                       UINT nID, CCreateContext* pContext) {
    if (!CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
        return FALSE;

    // Create WebBrowser control via COM
    HRESULT hr = CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_IWebBrowser2, (void**)&m_pBrowser);
    if (FAILED(hr) || !m_pBrowser) return FALSE;

    // Set up client site for in-place activation
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

    // Release site (browser holds a ref via AddRef)
    pSite->Release();

    m_pBrowser->put_Visible(VARIANT_TRUE);
    m_bCreated = TRUE;
    return TRUE;
}
void CHtmlView::OnDraw(void* /*pDC*/) {
    // WebBrowser renders itself
}
void CHtmlView::Navigate(const wchar_t* lpszURL, DWORD dwFlags,
                         const wchar_t* lpszTargetFrameName,
                         const wchar_t* lpszHeaders,
                         void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vPostData, vHeaders;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vPostData); VariantInit(&vHeaders);

    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;

    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    if (lpszHeaders) { vHeaders.vt = VT_BSTR; vHeaders.bstrVal = SysAllocString(lpszHeaders); }
    if (lpvPostData && dwPostDataLen > 0) {
        vPostData.vt = VT_ARRAY | VT_UI1;
        SAFEARRAYBOUND sab = { dwPostDataLen, 0 };
        SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, &sab);
        if (psa) {
            void* pvData = nullptr;
            SafeArrayAccessData(psa, &pvData);
            if (pvData) memcpy(pvData, lpvPostData, dwPostDataLen);
            SafeArrayUnaccessData(psa);
            vPostData.parray = psa;
        }
    }

    BSTR bstrUrl = SysAllocString(lpszURL);
    m_pBrowser->Navigate(bstrUrl, &vFlags, &vTarget, &vPostData, &vHeaders);
    SysFreeString(bstrUrl);

    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vPostData); VariantClear(&vHeaders);
}
void CHtmlView::Navigate2(LPITEMIDLIST pIDL, DWORD dwFlags, const wchar_t* lpszTargetFrameName) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vEmpty;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vEmpty);
    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;
    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    // For PIDL navigation: pass empty URL; PIDL needs CoTaskMemAlloc + ILSaveToStream
    (void)pIDL;
    VARIANT vUrl;
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(L"");
    m_pBrowser->Navigate2(&vUrl, &vFlags, &vTarget, &vEmpty, &vEmpty);
    VariantClear(&vUrl);
    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vEmpty);
}
void CHtmlView::Navigate2(const wchar_t* lpszURL, DWORD dwFlags,
                          const wchar_t* lpszTargetFrameName,
                          const wchar_t* lpszHeaders,
                          void* lpvPostData, DWORD dwPostDataLen) {
    if (!m_pBrowser) return;
    VARIANT vFlags, vTarget, vPostData, vHeaders;
    VariantInit(&vFlags); VariantInit(&vTarget); VariantInit(&vPostData); VariantInit(&vHeaders);
    vFlags.vt = VT_I4; vFlags.lVal = dwFlags;
    if (lpszTargetFrameName) { vTarget.vt = VT_BSTR; vTarget.bstrVal = SysAllocString(lpszTargetFrameName); }
    if (lpszHeaders) { vHeaders.vt = VT_BSTR; vHeaders.bstrVal = SysAllocString(lpszHeaders); }
    VARIANT vUrl;
    vUrl.vt = VT_BSTR;
    vUrl.bstrVal = SysAllocString(lpszURL);
    m_pBrowser->Navigate2(&vUrl, &vFlags, &vTarget, &vPostData, &vHeaders);
    VariantClear(&vUrl);
    VariantClear(&vFlags); VariantClear(&vTarget); VariantClear(&vPostData); VariantClear(&vHeaders);
    (void)lpvPostData; (void)dwPostDataLen;
}
void CHtmlView::GoBack() { if (m_pBrowser) m_pBrowser->GoBack(); }
void CHtmlView::GoForward() { if (m_pBrowser) m_pBrowser->GoForward(); }
void CHtmlView::GoHome() { if (m_pBrowser) m_pBrowser->GoHome(); }
void CHtmlView::GoSearch() { if (m_pBrowser) m_pBrowser->GoSearch(); }
void CHtmlView::Stop() { if (m_pBrowser) m_pBrowser->Stop(); }
void CHtmlView::Refresh() { if (m_pBrowser) m_pBrowser->Refresh(); }
void CHtmlView::Refresh2(int nLevel) { if (m_pBrowser) { VARIANT v; v.vt = VT_I4; v.lVal = nLevel; m_pBrowser->Refresh2(&v); } }
BOOL CHtmlView::GetBusy() const {
    if (!m_pBrowser) return FALSE;
    VARIANT_BOOL b = VARIANT_FALSE;
    m_pBrowser->get_Busy(&b);
    return b != VARIANT_FALSE;
}
READYSTATE CHtmlView::GetReadyState() const {
    if (!m_pBrowser) return READYSTATE_UNINITIALIZED;
    READYSTATE rs = READYSTATE_UNINITIALIZED;
    m_pBrowser->get_ReadyState(&rs);
    return rs;
}
CString CHtmlView::GetLocationName() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationName(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
CString CHtmlView::GetLocationURL() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_LocationURL(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
CString CHtmlView::GetFullName() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_FullName(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
CString CHtmlView::GetType() const {
    CString str;
    if (m_pBrowser) {
        BSTR bstr = nullptr;
        if (SUCCEEDED(m_pBrowser->get_Type(&bstr)) && bstr) {
            str = bstr;
            SysFreeString(bstr);
        }
    }
    return str;
}
LPDISPATCH CHtmlView::GetHtmlDocument() const {
    LPDISPATCH pDisp = nullptr;
    if (m_pBrowser) m_pBrowser->get_Document(&pDisp);
    return pDisp;
}
void CHtmlView::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdexecopt,
                       VARIANT* pvaIn, VARIANT* pvaOut) {
    if (m_pBrowser) m_pBrowser->ExecWB(cmdID, cmdexecopt, pvaIn, pvaOut);
}
BOOL CHtmlView::LoadFromResource(const wchar_t* lpszResource) {
    if (lpszResource && m_pBrowser) {
        CString strUrl = L"res://";
        // Try to get module path for res:// protocol
        wchar_t buf[MAX_PATH];
        if (::GetModuleFileNameW(AfxGetInstanceHandle(), buf, MAX_PATH)) {
            strUrl += buf;
            strUrl += L"/";
            strUrl += lpszResource;
            Navigate(strUrl);
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CHtmlView::LoadFromResource(UINT nRes) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%u", nRes);
    return LoadFromResource(buf);
}
void CHtmlView::Print() {
    if (m_pBrowser) {
        VARIANT vIn, vOut;
        VariantInit(&vIn); VariantInit(&vOut);
        m_pBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, &vIn, &vOut);
    }
}
void CHtmlView::PrintPreview() {
    if (m_pBrowser) {
        VARIANT vIn, vOut;
        VariantInit(&vIn); VariantInit(&vOut);
        m_pBrowser->ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DONTPROMPTUSER, &vIn, &vOut);
    }
}
void CHtmlView::OnBeforeNavigate2(const wchar_t* lpszURL, DWORD,
                                 const wchar_t*, CByteArray&, const wchar_t*,
                                 BOOL* pbCancel) {
    if (pbCancel) {
        *pbCancel = FALSE;
    }
    if (lpszURL && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszURL);
    }
}
void CHtmlView::OnNavigateComplete2(const wchar_t* lpszURL) {
    if (lpszURL && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszURL);
    }
}
void CHtmlView::OnDocumentComplete(const wchar_t* lpszURL) {
    if (lpszURL && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszURL);
    }
}
void CHtmlView::OnProgressChange(long nProgress, long nProgressMax) {
    if (m_hWnd && nProgressMax > 0) {
        wchar_t buf[128];
        swprintf(buf, 128, L"Loading... %ld / %ld", nProgress, nProgressMax);
        ::SetWindowTextW(m_hWnd, buf);
    }
}
void CHtmlView::OnTitleChange(const wchar_t* lpszText) {
    if (lpszText && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszText);
    }
}
void CHtmlView::OnStatusTextChange(const wchar_t* lpszText) {
    if (lpszText && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszText);
    }
}
void CHtmlView::OnCommandStateChange(long lCommand, BOOL bEnable) {
    if (m_hWnd) {
        wchar_t buf[96];
        swprintf(buf, 96, L"Command %ld %s", lCommand, bEnable ? L"enabled" : L"disabled");
        ::SetWindowTextW(m_hWnd, buf);
    }
}
void CHtmlView::OnDownloadBegin() {
    if (m_hWnd) {
        ::SetWindowTextW(m_hWnd, L"Download started");
    }
}
void CHtmlView::OnDownloadComplete() {
    if (m_hWnd) {
        ::SetWindowTextW(m_hWnd, L"Download complete");
    }
}
void CHtmlView::OnFullScreen(BOOL bFullScreen) {
    if (m_pBrowser) m_pBrowser->put_FullScreen(bFullScreen);
}
void CHtmlView::OnMenuBar(BOOL bMenuBar) {
    if (m_pBrowser) m_pBrowser->put_MenuBar(bMenuBar);
}
void CHtmlView::OnNavigateError(const wchar_t* lpszURL, const wchar_t* lpszFrame, DWORD dwError, BOOL* pbCancel) {
    if (pbCancel) {
        *pbCancel = FALSE;
    }
    if (!m_hWnd || !lpszURL) return;
    (void)lpszFrame; (void)dwError;
    ::SetWindowTextW(m_hWnd, lpszURL);
}
void CHtmlView::OnNewWindow2(LPDISPATCH*, BOOL* bCancel) {
    if (bCancel) {
        *bCancel = FALSE;
    }
}
void CHtmlView::OnPropertyChange(const wchar_t* lpszProperty) {
    if (lpszProperty && m_hWnd) {
        ::SetWindowTextW(m_hWnd, lpszProperty);
    }
}
void CHtmlView::OnQuit() {
    if (m_pBrowser) {
        m_pBrowser->Stop();
        m_pBrowser->put_Visible(VARIANT_FALSE);
    }
}
void CHtmlView::OnStatusBar(BOOL bStatusBar) {
    if (m_pBrowser) m_pBrowser->put_StatusBar(bStatusBar);
}
void CHtmlView::OnTheaterMode(BOOL bTheaterMode) {
    if (m_pBrowser) m_pBrowser->put_TheaterMode(bTheaterMode);
}
void CHtmlView::OnToolBar(BOOL bToolBar) {
    if (m_pBrowser) m_pBrowser->put_ToolBar(bToolBar);
}
void CHtmlView::OnVisible(BOOL bVisible) {
    if (m_pBrowser) m_pBrowser->put_Visible(bVisible ? VARIANT_TRUE : VARIANT_FALSE);
}
HRESULT CHtmlView::OnTranslateUrl(DWORD, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    if (!ppchURLOut) return E_POINTER;
    *ppchURLOut = nullptr;
    if (!pchURLIn) {
        return S_OK;
    }
    *ppchURLOut = CoTaskMemDuplicateW(pchURLIn);
    return *ppchURLOut ? S_OK : E_OUTOFMEMORY;
}
BOOL CHtmlView::GetSource(CString& strRef) {
    strRef.Empty();
    if (!m_pBrowser) return FALSE;

    LPDISPATCH pDocDisp = nullptr;
    if (FAILED(m_pBrowser->get_Document(&pDocDisp)) || !pDocDisp) return FALSE;

    IHTMLDocument3* pDoc3 = nullptr;
    HRESULT hr = pDocDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3);
    pDocDisp->Release();
    if (FAILED(hr) || !pDoc3) return FALSE;

    IHTMLElement* pRoot = nullptr;
    BSTR bstrSource = nullptr;
    hr = pDoc3->get_documentElement(&pRoot);
    pDoc3->Release();
    if (FAILED(hr) || !pRoot) return FALSE;

    hr = pRoot->get_outerHTML(&bstrSource);
    pRoot->Release();
    if (SUCCEEDED(hr) && bstrSource) {
        strRef = bstrSource;
        SysFreeString(bstrSource);
        return TRUE;
    }
    if (bstrSource) {
        SysFreeString(bstrSource);
    }
    return FALSE;
}
HRESULT CHtmlView::OnGetOptionKeyPath(wchar_t** ppwszPathKey, DWORD) {
    if (!ppwszPathKey) {
        return E_POINTER;
    }
    *ppwszPathKey = CoTaskMemDuplicateW(L"Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl");
    return *ppwszPathKey ? S_OK : E_OUTOFMEMORY;
}
LPDISPATCH CHtmlView::GetApplication() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Application(&p);
    return p;
}
LPDISPATCH CHtmlView::GetContainer() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Container(&p);
    return p;
}
LPDISPATCH CHtmlView::GetParentBrowser() const {
    LPDISPATCH p = nullptr;
    if (m_pBrowser) m_pBrowser->get_Parent(&p);
    return p;
}
long CHtmlView::GetLeft() const   { long v = 0; if (m_pBrowser) m_pBrowser->get_Left(&v);   return v; }
long CHtmlView::GetTop() const    { long v = 0; if (m_pBrowser) m_pBrowser->get_Top(&v);    return v; }
long CHtmlView::GetWidth() const  { long v = 0; if (m_pBrowser) m_pBrowser->get_Width(&v);  return v; }
long CHtmlView::GetHeight() const { long v = 0; if (m_pBrowser) m_pBrowser->get_Height(&v); return v; }
BOOL CHtmlView::GetVisible() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Visible(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetFullScreen() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_FullScreen(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetMenuBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_MenuBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetAddressBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_AddressBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetStatusBar() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_StatusBar(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetToolBar() const {
    int v = 0;
    if (m_pBrowser) m_pBrowser->get_ToolBar(&v);
    return v != 0;
}
BOOL CHtmlView::GetOffline() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Offline(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetSilent() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_Silent(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetTheaterMode() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_TheaterMode(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetRegisterAsBrowser() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_RegisterAsBrowser(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetRegisterAsDropTarget() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_RegisterAsDropTarget(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetTopLevelContainer() const {
    VARIANT_BOOL b = VARIANT_FALSE;
    if (m_pBrowser) m_pBrowser->get_TopLevelContainer(&b);
    return b != VARIANT_FALSE;
}
BOOL CHtmlView::GetProperty(const wchar_t* lpszProperty, CString& strValue) {
    if (!m_pBrowser || !lpszProperty) return FALSE;
    VARIANT v;
    VariantInit(&v);
    BSTR bstrProp = SysAllocString(lpszProperty);
    HRESULT hr = m_pBrowser->GetProperty(bstrProp, &v);
    SysFreeString(bstrProp);
    if (SUCCEEDED(hr) && v.vt == VT_BSTR && v.bstrVal) {
        strValue = v.bstrVal;
        VariantClear(&v);
        return TRUE;
    }
    VariantClear(&v);
    return FALSE;
}
void CHtmlView::PutProperty(const wchar_t* lpszProperty, const VARIANT& vtValue) {
    if (!m_pBrowser || !lpszProperty) return;
    BSTR bstrProp = SysAllocString(lpszProperty);
    m_pBrowser->PutProperty(bstrProp, const_cast<VARIANT&>(vtValue));
    SysFreeString(bstrProp);
}
