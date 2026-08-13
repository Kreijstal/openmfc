// COleControlSite — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COleControlSite@@QEAA@PEAVCOleControlContainer@@@Z
// COleControlSite::COleControlSite(COleControlContainer* pCtrlCont)
extern "C" void* MS_ABI impl___0COleControlSite__QEAA_PEAVCOleControlContainer___Z(void* p0) {
    return new COleControlSite(static_cast<COleControlContainer*>(p0));
}
COleControlSite::COleControlSite(COleControlContainer* pCtrlCont)
    : m_pCtrlCont(pCtrlCont), m_pControl(nullptr),
      m_lpObject(nullptr), m_lpInPlaceObject(nullptr), m_lpDispatch(nullptr),
      m_hWnd(nullptr), m_bInPlaceActive(FALSE), m_dwStyle(0) {
    memset(_olecontrolsite_padding, 0, sizeof(_olecontrolsite_padding));
}
COleControlSite::~COleControlSite() {
    DestroyControl();
    RemoveControlSiteState(this);
}
BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, REFCLSID clsid, const wchar_t* lpszWindowName,
                                     DWORD dwStyle, const RECT& rect, UINT nID, CFile* pPersist,
                                     BOOL bStorage, BSTR bstrLicKey) {
    (void)lpszWindowName; (void)pPersist; (void)bStorage;
    m_dwStyle = dwStyle;
    SetControlSiteId(this, nID);

    // Use CoCreateInstance (with optional license via IClassFactory2)
    IUnknown* pUnk = nullptr;
    HRESULT hr;
    if (bstrLicKey) {
        IClassFactory2* pCF2 = nullptr;
        hr = CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, nullptr, IID_IClassFactory2,
                              reinterpret_cast<void**>(&pCF2));
        if (SUCCEEDED(hr) && pCF2) {
            hr = pCF2->CreateInstanceLic(nullptr, nullptr, IID_IUnknown, bstrLicKey,
                                          reinterpret_cast<void**>(&pUnk));
            pCF2->Release();
        } else {
            hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_IUnknown,
                                  reinterpret_cast<void**>(&pUnk));
        }
    } else {
        hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_IUnknown,
                              reinterpret_cast<void**>(&pUnk));
    }
    if (FAILED(hr) || !pUnk) return FALSE;

    // Get IOleObject
    hr = pUnk->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&m_lpObject));
    if (FAILED(hr)) { pUnk->Release(); return FALSE; }

    // Get IDispatch
    pUnk->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&m_lpDispatch));

    // Get IOleInPlaceObject
    pUnk->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject));

    pUnk->Release();

    ControlSiteAdapter* pAdapter = GetControlSiteAdapter(this, true);
    if (!pAdapter) {
        DestroyControl();
        return FALSE;
    }
    hr = m_lpObject->SetClientSite(static_cast<IOleClientSite*>(pAdapter));
    if (FAILED(hr)) {
        DestroyControl();
        return FALSE;
    }
    OleSetContainedObject(m_lpObject, TRUE);

    // Set host names
    m_lpObject->SetHostNames(L"OpenMFC", lpszWindowName ? lpszWindowName : L"");

    // Activate in-place if we have a parent window
    HWND hParent = pWndCtrl ? pWndCtrl->GetSafeHwnd() : nullptr;
    if (!hParent) {
        hParent = GetSiteParentWindow(this);
    }
    if (hParent) {
        hr = m_lpObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr,
                                static_cast<IOleClientSite*>(pAdapter), 0, hParent, &rect);
        if (SUCCEEDED(hr)) {
            m_bInPlaceActive = TRUE;
            // Retrieve the in-place window handle
            if (m_lpInPlaceObject) {
                m_lpInPlaceObject->GetWindow(&m_hWnd);
                m_lpInPlaceObject->SetObjectRects(&rect, &rect);
            }
        }
    }

    return TRUE;
}
BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, const wchar_t* lpszProgID,
                                     DWORD dwStyle, const RECT& rect, UINT nID) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(pWndCtrl, clsid, L"", dwStyle, rect, nID);
}
BOOL COleControlSite::DestroyControl() {
    if (m_lpObject) { m_lpObject->SetClientSite(nullptr); }
    if (m_lpObject) { m_lpObject->Release(); m_lpObject = nullptr; }
    if (m_lpInPlaceObject) { m_lpInPlaceObject->Release(); m_lpInPlaceObject = nullptr; }
    if (m_lpDispatch) { m_lpDispatch->Release(); m_lpDispatch = nullptr; }
    m_pControl = nullptr;
    m_hWnd = nullptr;
    m_bInPlaceActive = FALSE;
    return TRUE;
}
void COleControlSite::Activate(BOOL bActivate) {
    if (bActivate) {
        if (!m_bInPlaceActive && m_lpObject) {
            HWND hParent = m_hWnd ? ::GetParent(m_hWnd) : nullptr;
            if (!hParent && m_pCtrlCont && m_pCtrlCont->GetWnd())
                hParent = m_pCtrlCont->GetWnd()->GetSafeHwnd();
            if (hParent) {
                RECT rc = {};
                if (m_hWnd) {
                    rc = GetSitePositionRect(this);
                } else {
                    ::GetClientRect(hParent, &rc);
                }
                ControlSiteAdapter* pAdapter = GetControlSiteAdapter(this, true);
                if (!pAdapter) {
                    return;
                }
                HRESULT hr = m_lpObject->DoVerb(OLEIVERB_INPLACEACTIVATE, nullptr,
                                                static_cast<IOleClientSite*>(pAdapter), 0, hParent, &rc);
                if (SUCCEEDED(hr)) {
                    m_bInPlaceActive = TRUE;
                    if (m_lpInPlaceObject && !m_hWnd)
                        m_lpInPlaceObject->GetWindow(&m_hWnd);
                    if (m_lpInPlaceObject)
                        m_lpInPlaceObject->SetObjectRects(&rc, &rc);
                }
            }
        }
    } else {
        Deactivate();
    }
}
void COleControlSite::Deactivate() {
    if (m_bInPlaceActive && m_lpInPlaceObject) {
        m_lpInPlaceObject->InPlaceDeactivate();
    }
    m_bInPlaceActive = FALSE;
}
BOOL COleControlSite::IsInPlaceActive() const {
    return m_bInPlaceActive;
}
void COleControlSite::SetProperty(DISPID dwDispID, VARTYPE vtProp, ...) {
    BYTE params[2] = { static_cast<BYTE>(vtProp), 0 };
    va_list args;
    va_start(args, vtProp);
    InvokeHelperV(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr,
                  params, args);
    va_end(args);
}
void COleControlSite::GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) {
    InvokeHelper(dwDispID, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr);
}
HRESULT COleControlSite::InvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                                       void* pvRet, const BYTE* pbParamInfo, ...) {
    va_list args;
    va_start(args, pbParamInfo);
    HRESULT hr = InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
    va_end(args);
    return hr;
}
HRESULT COleControlSite::InvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
                                        void* pvRet, const BYTE* pbParamInfo, va_list argList) {
    if (!m_lpDispatch) return E_POINTER;

    int cParams = CountDispatchParams(pbParamInfo);
    std::vector<VARIANTARG> params(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        HRESULT hr = MakeDispatchVariant(static_cast<VARTYPE>(pbParamInfo[i]), &argList, &params[static_cast<size_t>(i)]);
        if (FAILED(hr)) {
            for (int j = 0; j < i; ++j) VariantClear(&params[static_cast<size_t>(j)]);
            return hr;
        }
    }

    std::vector<VARIANTARG> reversed(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        reversed[static_cast<size_t>(i)] = params[static_cast<size_t>(cParams - i - 1)];
    }

    DISPPARAMS dispParams = {};
    dispParams.cArgs = cParams;
    dispParams.rgvarg = cParams ? reversed.data() : nullptr;
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) {
        dispParams.cNamedArgs = 1;
        dispParams.rgdispidNamedArgs = &dispidNamed;
    }

    VARIANT result;
    VariantInit(&result);
    EXCEPINFO excep = {};
    UINT argErr = 0;
    HRESULT hr = m_lpDispatch->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, wFlags,
                                      &dispParams, &result, &excep, &argErr);

    for (int i = 0; i < cParams; ++i) {
        reversed[static_cast<size_t>(i)].vt = VT_EMPTY;
        VariantClear(&params[static_cast<size_t>(i)]);
    }

    if (FAILED(hr)) {
        if (excep.bstrDescription) SysFreeString(excep.bstrDescription);
        if (excep.bstrSource) SysFreeString(excep.bstrSource);
        if (excep.bstrHelpFile) SysFreeString(excep.bstrHelpFile);
        VariantClear(&result);
        return hr;
    }

    hr = CopyDispatchResult(vtRet, pvRet, &result);
    VariantClear(&result);
    return hr;
}
BOOL COleControlSite::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    if (!pvProp) {
        return FALSE;
    }

    if (m_lpDispatch &&
        SUCCEEDED(InvokeHelper(dwDispid, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr))) {
        return TRUE;
    }

    switch (dwDispid) {
    case DISPID_AMBIENT_USERMODE:
        if (vtProp == VT_BOOL) {
            *static_cast<VARIANT_BOOL*>(pvProp) = VARIANT_TRUE;
            return TRUE;
        }
        break;
    case DISPID_AMBIENT_BACKCOLOR:
        if (vtProp == VT_COLOR || vtProp == VT_I4) {
            *static_cast<long*>(pvProp) = static_cast<long>(GetSysColor(COLOR_WINDOW));
            return TRUE;
        }
        break;
    case DISPID_AMBIENT_FORECOLOR:
        if (vtProp == VT_COLOR || vtProp == VT_I4) {
            *static_cast<long*>(pvProp) = static_cast<long>(GetSysColor(COLOR_WINDOWTEXT));
            return TRUE;
        }
        break;
    default:
        break;
    }

    return FALSE;
}
long COleControlSite::GetWindow(HWND__** phWnd) {
    if (!phWnd) return E_POINTER;
    *phWnd = m_hWnd;
    return m_hWnd ? S_OK : E_FAIL;
}
CWnd* COleControlSite::GetWindow() const {
    return CWnd::FromHandle(m_hWnd);
}
long COleControlSite::GetContainer(IOleContainer** ppContainer) {
    if (!ppContainer) return E_POINTER;
    *ppContainer = nullptr;
    return E_NOINTERFACE;
}
void COleControlSite::ShowPropertyFrame() {
    if (m_pControl) {
        m_pControl->ShowPropertyPages();
        return;
    }
    if (!m_lpObject) {
        return;
    }

    ISpecifyPropertyPages* pSPP = nullptr;
    if (FAILED(m_lpObject->QueryInterface(IID_ISpecifyPropertyPages,
                                          reinterpret_cast<void**>(&pSPP))) || !pSPP) {
        return;
    }

    CAUUID pages = {};
    if (SUCCEEDED(pSPP->GetPages(&pages)) && pages.cElems > 0 && pages.pElems) {
        IUnknown* pUnk = nullptr;
        m_lpObject->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnk));
        HWND hwndOwner = m_hWnd ? m_hWnd : GetSiteParentWindow(this);
        OleCreatePropertyFrame(hwndOwner, 0, 0, nullptr,
                               pUnk ? 1u : 0u, pUnk ? &pUnk : nullptr,
                               pages.cElems, pages.pElems,
                               LOCALE_USER_DEFAULT, 0, nullptr);
        if (pUnk) {
            pUnk->Release();
        }
    }
    CoTaskMemFree(pages.pElems);
    pSPP->Release();
}
void COleControlSite::EnableWindow(BOOL bEnable) {
    if (m_hWnd) {
        ::EnableWindow(m_hWnd, bEnable);
    }
}
COleControlContainer* COleControlSite::GetContainer() const {
    return m_pCtrlCont;
}
COleControl* COleControlSite::GetControl() const {
    return m_pControl;
}
LPOLEOBJECT COleControlSite::GetOleObject() {
    return m_lpObject;
}
LPOLEINPLACEOBJECT COleControlSite::GetInPlaceObject() {
    return m_lpInPlaceObject;
}
LPDISPATCH COleControlSite::GetDispatch() {
    return m_lpDispatch;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?CreateControl@COleControlSite@@QEAAJPEAVCWnd@@AEBVCControlCreationInfo@@KPEBUtagPOINT@@PEBUtagSIZE@@I@Z
extern "C" long MS_ABI impl__CreateControl_COleControlSite__QEAAJPEAVCWnd__AEBVCControlCreationInfo__KPEBUtagPOINT__PEBUtagSIZE__I_Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4, void* p5) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    return 0L;
}


// Symbol: ?CreateControl@COleControlSite@@UEAAJPEAVCWnd@@AEBU_GUID@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_W@Z
extern "C" long MS_ABI impl__CreateControl_COleControlSite__UEAAJPEAVCWnd__AEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    (void)p9;
    return 0L;
}


// Symbol: ?GetProperty@COleControlSite@@UEBAXJGPEAX@Z
extern "C" void MS_ABI impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(void* pThis, void* p0, void* p1, void* p2) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
}

// Symbol: ?AttachWindow@COleControlSite@@QEAAXXZ
extern "C" void MS_ABI impl__AttachWindow_COleControlSite__QEAAXXZ() {}

// Symbol: ?BindDefaultProperty@COleControlSite@@UEAAXJGPEB_WPEAVCWnd@@@Z
extern "C" void MS_ABI impl__BindDefaultProperty_COleControlSite__UEAAXJGPEB_WPEAVCWnd___Z(long p0, unsigned short p1, const wchar_t* p2, void* /*class*/* p3) {}

// Symbol: ?BindProperty@COleControlSite@@UEAAXJPEAVCWnd@@@Z
extern "C" void MS_ABI impl__BindProperty_COleControlSite__UEAAXJPEAVCWnd___Z(long p0, void* /*class*/* p1) {}

// Symbol: ?ConnectSink@COleControlSite@@IEAAKAEBU_GUID@@PEAUIUnknown@@@Z
extern "C" unsigned long MS_ABI impl__ConnectSink_COleControlSite__IEAAKAEBU_GUID__PEAUIUnknown___Z(const void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?CreateControlCommon@COleControlSite@@MEAAJPEAVCWnd@@AEBU_GUID@@AEBVCControlCreationInfo@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_W@Z
extern "C" long MS_ABI impl__CreateControlCommon_COleControlSite__MEAAJPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(void* /*class*/* p0, const void* /*struct*/* p1, const void* /*class*/* p2, const wchar_t* p3, unsigned long p4, const void* /*struct*/* p5, const void* /*struct*/* p6, unsigned int p7, void* /*class*/* p8, int p9, wchar_t* p10) {
    return 0;
}

// Symbol: ?CreateOrLoad@COleControlSite@@MEAAJAEBU_GUID@@PEAVCFile@@HPEA_W@Z
extern "C" long MS_ABI impl__CreateOrLoad_COleControlSite__MEAAJAEBU_GUID__PEAVCFile__HPEA_W_Z(const void* /*struct*/* p0, void* /*class*/* p1, int p2, wchar_t* p3) {
    return 0;
}

// Symbol: ?CreateOrLoad@COleControlSite@@MEAAJAEBVCControlCreationInfo@@@Z
extern "C" long MS_ABI impl__CreateOrLoad_COleControlSite__MEAAJAEBVCControlCreationInfo___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?DetachWindow@COleControlSite@@QEAAXXZ
extern "C" void MS_ABI impl__DetachWindow_COleControlSite__QEAAXXZ() {}

// Symbol: ?DisconnectSink@COleControlSite@@IEAAXAEBU_GUID@@K@Z
extern "C" void MS_ABI impl__DisconnectSink_COleControlSite__IEAAXAEBU_GUID__K_Z(const void* /*struct*/* p0, unsigned long p1) {}

// Symbol: ?DoVerb@COleControlSite@@UEAAJJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__DoVerb_COleControlSite__UEAAJJPEAUtagMSG___Z(long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?EnableDSC@COleControlSite@@UEAAXXZ
extern "C" void MS_ABI impl__EnableDSC_COleControlSite__UEAAXXZ() {}

// Symbol: ?FreezeEvents@COleControlSite@@QEAAXH@Z
extern "C" void MS_ABI impl__FreezeEvents_COleControlSite__QEAAXH_Z(int p0) {}

// Symbol: ?GetControlInfo@COleControlSite@@IEAAXXZ
extern "C" void MS_ABI impl__GetControlInfo_COleControlSite__IEAAXXZ() {}

// Symbol: ?GetCursor@COleControlSite@@IEAAJJPEAPEAUIUnknown@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetCursor_COleControlSite__IEAAJJPEAPEAUIUnknown__PEAPEAX_Z(long p0, void* /*struct*/** p1, void** p2) {
    return 0;
}

// Symbol: ?GetDefBtnCode@COleControlSite@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetDefBtnCode_COleControlSite__QEAAKXZ() {
    return 0;
}

// Symbol: ?GetDlgCtrlID@COleControlSite@@UEBAHXZ
extern "C" int MS_ABI impl__GetDlgCtrlID_COleControlSite__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetEventIID@COleControlSite@@QEAAHPEAU_GUID@@@Z
extern "C" int MS_ABI impl__GetEventIID_COleControlSite__QEAAHPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetExStyle@COleControlSite@@UEBAKXZ
extern "C" unsigned long MS_ABI impl__GetExStyle_COleControlSite__UEBAKXZ() {
    return 0;
}

// Symbol: ?GetStyle@COleControlSite@@UEBAKXZ
extern "C" unsigned long MS_ABI impl__GetStyle_COleControlSite__UEBAKXZ() {
    return 0;
}

// Symbol: ?GetWindowTextW@COleControlSite@@UEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetWindowTextW_COleControlSite__UEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, void** p1, void* p2) {}

// Symbol: ?IsDefaultButton@COleControlSite@@QEAAHXZ
extern "C" int MS_ABI impl__IsDefaultButton_COleControlSite__QEAAHXZ() {
    return 0;
}

// Symbol: ?IsMatchingMnemonic@COleControlSite@@QEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsMatchingMnemonic_COleControlSite__QEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?IsWindowEnabled@COleControlSite@@UEBAHXZ
extern "C" int MS_ABI impl__IsWindowEnabled_COleControlSite__UEBAHXZ() {
    return 0;
}

// Symbol: ?ModifyStyle@COleControlSite@@UEAAHKKI@Z
extern "C" int MS_ABI impl__ModifyStyle_COleControlSite__UEAAHKKI_Z(unsigned long p0, unsigned long p1, unsigned int p2) {
    return 0;
}

// Symbol: ?ModifyStyleEx@COleControlSite@@UEAAHKKI@Z
extern "C" int MS_ABI impl__ModifyStyleEx_COleControlSite__UEAAHKKI_Z(unsigned long p0, unsigned long p1, unsigned int p2) {
    return 0;
}

// Symbol: ?MoveWindow@COleControlSite@@UEAAXHHHH@Z
extern "C" void MS_ABI impl__MoveWindow_COleControlSite__UEAAXHHHH_Z(int p0, int p1, int p2, int p3) {}

// Symbol: ?OnEvent@COleControlSite@@IEAAHPEAUAFX_EVENT@@@Z
extern "C" int MS_ABI impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?QuickActivate@COleControlSite@@UEAAHXZ
extern "C" int MS_ABI impl__QuickActivate_COleControlSite__UEAAHXZ() {
    return 0;
}

// Symbol: ?SafeSetProperty@COleControlSite@@UEAAHJGZZ
extern "C" int MS_ABI impl__SafeSetProperty_COleControlSite__UEAAHJGZZ(long p0, unsigned short p1) {
    return 0;
}

// Symbol: ?SendMnemonic@COleControlSite@@QEAAXPEAUtagMSG@@@Z
extern "C" void MS_ABI impl__SendMnemonic_COleControlSite__QEAAXPEAUtagMSG___Z(void* /*struct*/* p0) {}

// Symbol: ?SetDefaultButton@COleControlSite@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDefaultButton_COleControlSite__QEAAXH_Z(int p0) {}

// Symbol: ?SetDlgCtrlID@COleControlSite@@UEAAHH@Z
extern "C" int MS_ABI impl__SetDlgCtrlID_COleControlSite__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?SetExtent@COleControlSite@@IEAAHXZ
extern "C" int MS_ABI impl__SetExtent_COleControlSite__IEAAHXZ() {
    return 0;
}

// Symbol: ?SetFocus@COleControlSite@@UEAAPEAVCWnd@@PEAUtagMSG@@@Z
extern "C" void* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__PEAUtagMSG___Z(void* /*struct*/* p0) {
    return nullptr;
}

// Symbol: ?SetFocus@COleControlSite@@UEAAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__XZ() {
    return nullptr;
}

// Symbol: ?SetPropertyV@COleControlSite@@UEAAXJGPEAD@Z
extern "C" void MS_ABI impl__SetPropertyV_COleControlSite__UEAAXJGPEAD_Z(long p0, unsigned short p1, char* p2) {}

// Symbol: ?SetWindowPos@COleControlSite@@UEAAHPEBVCWnd@@HHHHI@Z
extern "C" int MS_ABI impl__SetWindowPos_COleControlSite__UEAAHPEBVCWnd__HHHHI_Z(const void* /*class*/* p0, int p1, int p2, int p3, int p4, unsigned int p5) {
    return 0;
}

// Symbol: ?SetWindowTextW@COleControlSite@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetWindowTextW_COleControlSite__UEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?ShowWindow@COleControlSite@@UEAAHH@Z
extern "C" int MS_ABI impl__ShowWindow_COleControlSite__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?AboutToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__AboutToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?AdjustRect@XOleIPSite@COleControlSite@@UEAAJPEAUtagRECT@@@Z
extern "C" long MS_ABI impl__AdjustRect_XOleIPSite_COleControlSite__UEAAJPEAUtagRECT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?CanInPlaceActivate@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__CanInPlaceActivate_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?CanWindowlessActivate@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__CanWindowlessActivate_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?Cancelled@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__Cancelled_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?ContextSensitiveHelp@XOleIPSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleIPSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?DeactivateAndUndo@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__DeactivateAndUndo_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?DidEvent@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__DidEvent_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?DiscardUndoState@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__DiscardUndoState_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?FailedToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__FailedToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?FireEvent@XNotifyDBEvents@COleControlSite@@QEAAJKKQEAUtagDBNOTIFYREASON@@W4DSCSTATE@@@Z
extern "C" long MS_ABI impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4, int /*enum*/ p5, char p6, void* p7, void* p8, void* p9, void* p10, void** p11, unsigned char p12) {
    return 0;
}

// Symbol: ?GetCapture@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__GetCapture_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?GetContainer@XOleClientSite@COleControlSite@@UEAAJPEAPEAUIOleContainer@@@Z
extern "C" long MS_ABI impl__GetContainer_XOleClientSite_COleControlSite__UEAAJPEAPEAUIOleContainer___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetCursor@XBoundObjectSite@COleControlSite@@UEAAJJPEAPEAUICursor@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetCursor_XBoundObjectSite_COleControlSite__UEAAJJPEAPEAUICursor__PEAPEAX_Z(long p0, void* /*struct*/** p1, void** p2) {
    return 0;
}

// Symbol: ?GetDC@XOleIPSite@COleControlSite@@UEAAJPEBUtagRECT@@KPEAPEAUHDC__@@@Z
extern "C" long MS_ABI impl__GetDC_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__KPEAPEAUHDC_____Z(const void* /*struct*/* p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetExtendedControl@XOleControlSite@COleControlSite@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExtendedControl_XOleControlSite_COleControlSite__UEAAJPEAPEAUIDispatch___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetFocus@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__GetFocus_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?GetIDsOfNames@XAmbientProps@COleControlSite@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_XAmbientProps_COleControlSite__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(const void* /*struct*/* p0, wchar_t** p1, unsigned int p2, unsigned long p3, long* p4) {
    return 0;
}

// Symbol: ?GetIDsOfNames@XEventSink@COleControlSite@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_XEventSink_COleControlSite__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(const void* /*struct*/* p0, wchar_t** p1, unsigned int p2, unsigned long p3, long* p4) {
    return 0;
}

// Symbol: ?GetMoniker@XOleClientSite@COleControlSite@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleClientSite_COleControlSite__UEAAJKKPEAPEAUIMoniker___Z(unsigned long p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetTypeInfo@XAmbientProps@COleControlSite@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_XAmbientProps_COleControlSite__UEAAJIKPEAPEAUITypeInfo___Z(unsigned int p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetTypeInfo@XEventSink@COleControlSite@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_XEventSink_COleControlSite__UEAAJIKPEAPEAUITypeInfo___Z(unsigned int p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetTypeInfoCount@XAmbientProps@COleControlSite@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_XAmbientProps_COleControlSite__UEAAJPEAI_Z(unsigned int* p0) {
    return 0;
}

// Symbol: ?GetTypeInfoCount@XEventSink@COleControlSite@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_XEventSink_COleControlSite__UEAAJPEAI_Z(unsigned int* p0) {
    return 0;
}

// Symbol: ?GetWindow@XOleIPSite@COleControlSite@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleIPSite_COleControlSite__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetWindowContext@XOleIPSite@COleControlSite@@UEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagRECT@@2PEAUtagOIFI@@@Z
extern "C" long MS_ABI impl__GetWindowContext_XOleIPSite_COleControlSite__UEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagRECT__2PEAUtagOIFI___Z(void* /*struct*/** p0, void* /*struct*/** p1, void* /*struct*/* p2, void* p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?InvalidateRect@XOleIPSite@COleControlSite@@UEAAJPEBUtagRECT@@H@Z
extern "C" long MS_ABI impl__InvalidateRect_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__H_Z(const void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?InvalidateRgn@XOleIPSite@COleControlSite@@UEAAJPEAUHRGN__@@H@Z
extern "C" long MS_ABI impl__InvalidateRgn_XOleIPSite_COleControlSite__UEAAJPEAUHRGN____H_Z(void* /*struct*/* p0, int p1) {
    return 0;
}

// Symbol: ?Invoke@XAmbientProps@COleControlSite@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_XAmbientProps_COleControlSite__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(long p0, const void* /*struct*/* p1, unsigned long p2, unsigned short p3, void* /*struct*/* p4, void* /*struct*/* p5, void* /*struct*/* p6, unsigned int* p7) {
    return 0;
}

// Symbol: ?Invoke@XEventSink@COleControlSite@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_XEventSink_COleControlSite__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(long p0, const void* /*struct*/* p1, unsigned long p2, unsigned short p3, void* /*struct*/* p4, void* /*struct*/* p5, void* /*struct*/* p6, unsigned int* p7) {
    return 0;
}

// Symbol: ?LockInPlaceActive@XOleControlSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__LockInPlaceActive_XOleControlSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OKToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__OKToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?OnChanged@XPropertyNotifySink@COleControlSite@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnControlInfoChanged@XOleControlSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__OnControlInfoChanged_XOleControlSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?OnDefWindowMessage@XOleIPSite@COleControlSite@@UEAAJI_K_JPEA_J@Z
extern "C" long MS_ABI impl__OnDefWindowMessage_XOleIPSite_COleControlSite__UEAAJI_K_JPEA_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2, __int64* p3) {
    return 0;
}

// Symbol: ?OnFieldChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@_K1QEA_KKKH@Z
extern "C" long MS_ABI impl__OnFieldChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___K1QEA_KKKH_Z(void* /*struct*/* p0, unsigned __int64 p1, unsigned __int64 p2, void* p3, unsigned char p4, unsigned __int64* p5, unsigned long p6, unsigned long p7, int p8) {
    return 0;
}

// Symbol: ?OnFocus@XOleControlSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFocus_XOleControlSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnInPlaceActivate@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__OnInPlaceActivate_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?OnInPlaceActivateEx@XOleIPSite@COleControlSite@@UEAAJPEAHK@Z
extern "C" long MS_ABI impl__OnInPlaceActivateEx_XOleIPSite_COleControlSite__UEAAJPEAHK_Z(int* p0, unsigned long p1) {
    return 0;
}

// Symbol: ?OnInPlaceDeactivate@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__OnInPlaceDeactivate_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?OnInPlaceDeactivateEx@XOleIPSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnInPlaceDeactivateEx_XOleIPSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnPosRectChange@XOleIPSite@COleControlSite@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__OnPosRectChange_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnRequestEdit@XPropertyNotifySink@COleControlSite@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?OnRowChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@_KQEB_KKKH@Z
extern "C" long MS_ABI impl__OnRowChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___KQEB_KKKH_Z(void* /*struct*/* p0, unsigned __int64 p1, void* p2, unsigned char p3, void* p4, unsigned __int64 p5, unsigned long p6, unsigned long p7, int p8) {
    return 0;
}

// Symbol: ?OnRowsetChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@KKH@Z
extern "C" long MS_ABI impl__OnRowsetChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset__KKH_Z(void* /*struct*/* p0, unsigned long p1, unsigned long p2, int p3) {
    return 0;
}

// Symbol: ?OnShowWindow@XOleClientSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnShowWindow_XOleClientSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnUIActivate@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__OnUIActivate_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?OnUIDeactivate@XOleIPSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnUIDeactivate_XOleIPSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?ReleaseDC@XOleIPSite@COleControlSite@@UEAAJPEAUHDC__@@@Z
extern "C" long MS_ABI impl__ReleaseDC_XOleIPSite_COleControlSite__UEAAJPEAUHDC_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RequestNewObjectLayout@XOleClientSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__RequestNewObjectLayout_XOleClientSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?RequestUIActivate@XOleIPSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__RequestUIActivate_XOleIPSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?SaveObject@XOleClientSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__SaveObject_XOleClientSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?Scroll@XOleIPSite@COleControlSite@@UEAAJUtagSIZE@@@Z
extern "C" long MS_ABI impl__Scroll_XOleIPSite_COleControlSite__UEAAJUtagSIZE___Z(void* /*struct*/ p0) {
    return 0;
}

// Symbol: ?ScrollRect@XOleIPSite@COleControlSite@@UEAAJHHPEBUtagRECT@@0@Z
extern "C" long MS_ABI impl__ScrollRect_XOleIPSite_COleControlSite__UEAAJHHPEBUtagRECT__0_Z(int p0, int p1, const void* /*struct*/* p2, int p3) {
    return 0;
}

// Symbol: ?SetCapture@XOleIPSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__SetCapture_XOleIPSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?SetFocus@XOleIPSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__SetFocus_XOleIPSite_COleControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?ShowObject@XOleClientSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__ShowObject_XOleClientSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?ShowPropertyFrame@XOleControlSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__ShowPropertyFrame_XOleControlSite_COleControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?SyncAfter@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__SyncAfter_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?SyncBefore@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__SyncBefore_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(unsigned long p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?TransformCoords@XOleControlSite@COleControlSite@@UEAAJPEAU_POINTL@@PEAUtagPOINTF@@K@Z
extern "C" long MS_ABI impl__TransformCoords_XOleControlSite_COleControlSite__UEAAJPEAU_POINTL__PEAUtagPOINTF__K_Z(void* /*struct*/* p0, void* /*struct*/* p1, unsigned long p2) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XOleControlSite@COleControlSite@@UEAAJPEAUtagMSG@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleControlSite_COleControlSite__UEAAJPEAUtagMSG__K_Z(void* /*struct*/* p0, unsigned long p1) {
    return 0;
}
