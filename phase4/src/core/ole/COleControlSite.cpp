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
