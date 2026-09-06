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

//=============================================================================
// COleControlSite — retail-decoded implementations
//
// Everything from the "Moved from ManualThunks.cpp" marker down was decoded
// from the retail export (mangled name -> RVA -> objdump), the method this
// project uses elsewhere (see the header of core/ole/COleControl.cpp).  Each
// function names the RVA it was transcribed from.  Unqualified RVAs are
// mfc140.dll; the handful of entry points that exist only in the Unicode build
// (the ...W overloads and the XOleIPSite nested-interface methods) say
// "mfc140u.dll" explicitly and carry that DLL's RVAs, which do not match
// mfc140.dll's.  A few members also sit at different offsets between the two
// builds; the map below is mfc140.dll's.
//
// The retail member layout is NOT OpenMFC's.  The offsets quoted in the
// comments below are the ones the shipping code actually dereferences; this is
// how they were mapped onto OpenMFC state:
//
//   retail +0x40  m_pCtrlCont          -> COleControlSite::m_pCtrlCont
//   retail +0x48  m_hWnd               -> COleControlSite::m_hWnd
//   retail +0x50  m_pWndCtrl (CWnd*)   -> no OpenMFC equivalent
//   retail +0x58  control id           -> SiteExtra::controlId below, mirrored
//                                          into the shared detail::olecore
//                                          control-site id table on every write
//   retail +0x5c  position RECT        -> SiteExtra::rect below
//   retail +0x80  IOleObject*          -> COleControlSite::m_lpObject
//   retail +0x88  IOleInPlaceObject*   -> COleControlSite::m_lpInPlaceObject
//   retail +0xa8  style mask           -> no OpenMFC equivalent (no mask applied)
//   retail +0xac  m_dwStyle            -> COleControlSite::m_dwStyle
//   retail +0xb0  cached misc status   -> queried from IOleObject on demand
//   retail +0xb4  "windowless-active"  -> approximated by (m_hWnd == NULL)
//   retail +0x98  IOleInPlaceObjectWindowless -> no OpenMFC equivalent
//   retail +0xb8  cached CONTROLINFO   -> SiteExtra::ctlInfo below
//   retail +0xd8  CDataSourceControl*  -> no OpenMFC equivalent
//   retail +0xe0  CDataBoundProperty*  -> no OpenMFC equivalent
//   retail +0x138 nested IOleClientSite-> detail::olecore::ControlSiteAdapter
//
// Retail vtable slots referenced below were read out of the COleControlSite
// vftable at RVA 0x325130:
//   0xc8 DoVerb, 0xd0 InvokeHelperV, 0xd8 SetPropertyV, 0xe0 InvokeHelper,
//   0xe8 GetProperty, 0xf8 SafeSetProperty, 0x140 MoveWindow,
//   0x148 SetWindowPos, 0x150 ShowWindow, 0x158 IsWindowEnabled,
//   0x168 SetFocus(MSG*), 0x170 SetFocus(), 0x178 EnableDSC,
//   0x190 QuickActivate, 0x1a0/0x1a8 the two CreateOrLoad overloads.
//
// On the flag at retail +0xb4: it is the site's windowless-active flag.  The
// evidence is three independent uses.
//   * XOleIPSite::OnInPlaceDeactivate (0x23aae0) clears it, and only when it
//     was set does it release the interface at +0x98 and decrement a counter
//     in the container (+0xe0).
//   * The site loop inside COleControlContainer::HandleWindowlessMessage
//     (0x234ae0; the test is at 0x234c1b) visits a site only when the flag is
//     set, hit-tests the site rect at +0x5c, and then dispatches through +0x98
//     rather than through any window -- via that interface's vtable slot 9
//     (0x48) with four arguments (message, wParam, lParam, LRESULT*), i.e.
//     IOleInPlaceObjectWindowless::OnWindowMessage, which is also what
//     identifies the interface held at +0x98.
//   * COccManager::UIDeactivateIfNecessary (0x2402d0; the test is at 0x24033f)
//     does the opposite: it only maps the site's m_hWnd(+0x48) to a CWnd when
//     the flag is clear.
// A windowless control has no HWND, so OpenMFC substitutes "m_hWnd == NULL"
// for it in GetStyle, IsWindowEnabled and SetFocus.  That implication only
// runs one way -- a site that simply has not been activated yet also has a
// null m_hWnd -- so the substitution is an approximation, and each user says
// so.
//=============================================================================

// Forward declarations for the exports in this file that other exports here
// dispatch to (retail reaches them through the COleControlSite vtable slots
// listed above).
extern "C" long  MS_ABI impl__DoVerb_COleControlSite__UEAAJJPEAUtagMSG___Z(COleControlSite*, long, MSG*);
extern "C" int   MS_ABI impl__ShowWindow_COleControlSite__UEAAHH_Z(COleControlSite*, int);
extern "C" void  MS_ABI impl__MoveWindow_COleControlSite__UEAAXHHHH_Z(COleControlSite*, int, int, int, int);
extern "C" int   MS_ABI impl__SetExtent_COleControlSite__IEAAHXZ(COleControlSite*);
extern "C" CWnd* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__XZ(COleControlSite*);

namespace {

// Retail stores a position RECT at +0x5c and a cached CONTROLINFO at +0xb8.
// OpenMFC's COleControlSite has neither member and the per-site side table in
// phase4/src/detail/OlecoreSupport.h belongs to another file, so they are kept
// here in a side table keyed the same way.  Folding the two tables together
// belongs in OlecoreSupport.h and needs that file's owner.
//
// The table is a plain singly linked list rather than a std::map on purpose:
// std::map's insert/erase pull in the out-of-line libstdc++ red-black-tree
// helpers, and this translation unit is checked for exactly that kind of new
// external reference.  Sites are few, so a linear scan is adequate, and
// RemoveSiteExtra() below is called from ~COleControlSite (defined in this
// file) so entries do not outlive their site.
struct SiteExtra {
    const COleControlSite* site;
    SiteExtra* next;
    RECT rect;
    CONTROLINFO ctlInfo;
    UINT controlId;
    bool rectValid;
};

// Retail writes sizeof(CONTROLINFO) == 0x18 into ctlInfo.cb (0x2397da).
static_assert(sizeof(CONTROLINFO) == 0x18, "CONTROLINFO must match the retail 0x18-byte cb");
// Retail sizes its accelerator scratch buffer as cAccel * 6 (0x239885).
static_assert(sizeof(ACCEL) == 6, "ACCEL must be the packed 6-byte Win32 layout");

SiteExtra* g_pSiteExtraHead = nullptr;

SiteExtra& SiteExtraOf(COleControlSite* pSite) {
    for (SiteExtra* p = g_pSiteExtraHead; p; p = p->next) {
        if (p->site == pSite) return *p;
    }
    SiteExtra* p = new SiteExtra();
    p->site = pSite;
    p->next = g_pSiteExtraHead;
    p->rect.left = p->rect.top = p->rect.right = p->rect.bottom = 0;
    memset(&p->ctlInfo, 0, sizeof(p->ctlInfo));
    p->controlId = 0;
    p->rectValid = false;
    g_pSiteExtraHead = p;
    return *p;
}

void RemoveSiteExtra(const COleControlSite* pSite) {
    SiteExtra** pp = &g_pSiteExtraHead;
    while (*pp) {
        if ((*pp)->site == pSite) {
            SiteExtra* dead = *pp;
            *pp = dead->next;
            delete dead;
            return;
        }
        pp = &(*pp)->next;
    }
}

// The site position rect (retail +0x5c).  Seeded once from the live window so
// that sites created through OpenMFC's CreateControl() still report a rect.
RECT& SiteRect(COleControlSite* pSite) {
    SiteExtra& extra = SiteExtraOf(pSite);
    if (!extra.rectValid) {
        extra.rect = GetSitePositionRect(pSite);
        extra.rectValid = true;
    }
    return extra.rect;
}

// The control id retail keeps at +0x58.  OpenMFC's authoritative copy is the
// shared detail::olecore per-site table (COleControlContainer.cpp reads it
// through TryGetControlSiteId to resolve GetDlgItem), but this translation
// unit may not add an out-of-line reference to that table's accessors, so it
// mirrors the id here.  SetSiteControlId() below is the single writer and
// always updates both, and a grep of phase4/src shows CreateControl() and
// SetDlgCtrlID() -- both in this file -- are the only writers in the tree.
UINT SiteControlId(COleControlSite* pSite) {
    if (!pSite) return 0;
    return SiteExtraOf(pSite).controlId;
}

void SetSiteControlId(COleControlSite* pSite, UINT nID) {
    if (!pSite) return;
    SiteExtraOf(pSite).controlId = nID;
    SetControlSiteId(pSite, nID);
}

// Retail caches the object's misc status in a member at +0xb0 when the control
// is created.  OpenMFC keeps no such member, so it is asked for on demand.
DWORD SiteMiscStatus(COleControlSite* pSite) {
    DWORD dwMisc = 0;
    if (pSite && pSite->m_lpObject) {
        pSite->m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &dwMisc);
    }
    return dwMisc;
}

template <class TInterface>
TInterface* SiteQueryObject(COleControlSite* pSite, REFIID iid) {
    TInterface* p = nullptr;
    if (pSite && pSite->m_lpObject) {
        if (FAILED(pSite->m_lpObject->QueryInterface(iid, reinterpret_cast<void**>(&p)))) {
            p = nullptr;
        }
    }
    return p;
}

// COleControlSite::SetPropertyV — retail 0x239ca0.  The retail body rewrites
// the VARTYPE into MFC's one-byte VTS parameter code and picks PROPERTYPUT vs
// PROPERTYPUTREF from it:
//    vt & VT_BYREF -> vt = (vt & ~VT_BYREF) | 0x40   (MFC's byref marker)
//    vt == VT_BSTR -> parameter code 0x0e, PROPERTYPUT
//    vt & 0x8000   -> strip the bit, PROPERTYPUTREF
//    vt == VT_DISPATCH -> PROPERTYPUTREF
//    otherwise     -> PROPERTYPUT
// OpenMFC's MakeDispatchVariant() uses plain VT_ codes, where VT_BSTR already
// means "an LPCWSTR argument" — the same thing retail's 0x0e means — so the
// VT_BSTR rewrite is a no-op here.  MakeDispatchVariant does not decode the
// 0x40 byref marker; a byref put therefore fails with DISP_E_TYPEMISMATCH
// rather than being marshalled.
HRESULT SitePutPropertyV(COleControlSite* pSite, DISPID dispid, VARTYPE vtProp, va_list argList) {
    if (!pSite) return E_POINTER;
    WORD wFlags = DISPATCH_PROPERTYPUT;
    unsigned vt = vtProp;
    if (vt & VT_BYREF) {
        vt = (vt & ~static_cast<unsigned>(VT_BYREF)) | 0x40u;
    }
    if (vt == VT_BSTR) {
        // retail: parameter code 0x0e; OpenMFC spells the same thing VT_BSTR.
    } else if (vt & 0x8000u) {
        vt &= 0x7fffu;
        wFlags = DISPATCH_PROPERTYPUTREF;
    } else if (vt == VT_DISPATCH) {
        wFlags = DISPATCH_PROPERTYPUTREF;
    }
    BYTE params[2] = { static_cast<BYTE>(vt), 0 };
    return pSite->InvokeHelperV(dispid, wFlags, VT_EMPTY, nullptr, params, argList);
}

// COleControlSite::SafeSetProperty — retail 0x239de0.  The straight-line path
// is `call SetPropertyV (slot 0xd8); mov $0x1,%eax`, and the exception
// continuation at 0x239e10 reloads eax from a stack slot that only the catch
// funclet writes, so the failure value is not visible in the linear
// disassembly; the documented contract is nonzero on success and 0 when the
// put throws.  OpenMFC's InvokeHelperV reports failure as an HRESULT instead of
// throwing, so success is the SUCCEEDED() of that call.
BOOL SiteSafeSetProperty(COleControlSite* pSite, DISPID dispid, int vtProp, ...) {
    va_list argList;
    va_start(argList, vtProp);
    HRESULT hr = SitePutPropertyV(pSite, dispid, static_cast<VARTYPE>(vtProp), argList);
    va_end(argList);
    return SUCCEEDED(hr);
}

// The unexported style helper at retail 0x288bf8 (MFC calls it AfxModifyStyle;
// there is no export at that RVA, so the name is not verified here).  Both
// ModifyStyle and ModifyStyleEx call it once the property route has been
// tried, differing only in the GWL_STYLE/GWL_EXSTYLE offset they pass.
BOOL SiteModifyWindowStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags) {
    DWORD dwStyle = static_cast<DWORD>(::GetWindowLongW(hWnd, nStyleOffset));
    DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
    if (dwStyle == dwNewStyle) return FALSE;
    ::SetWindowLongW(hWnd, nStyleOffset, static_cast<LONG>(dwNewStyle));
    if (nFlags != 0) {
        // retail ORs 0x17 = SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE
        ::SetWindowPos(hWnd, nullptr, 0, 0, 0, 0,
                       nFlags | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    return TRUE;
}

}  // namespace

























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
    RemoveSiteExtra(this);
}
BOOL COleControlSite::CreateControl(CWnd* pWndCtrl, REFCLSID clsid, const wchar_t* lpszWindowName,
                                     DWORD dwStyle, const RECT& rect, UINT nID, CFile* pPersist,
                                     BOOL bStorage, BSTR bstrLicKey) {
    (void)lpszWindowName; (void)pPersist; (void)bStorage;
    m_dwStyle = dwStyle;
    SetSiteControlId(this, nID);

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
// COleControlSite::GetProperty() — retail vtable slot 0xe8 (0x239d70); the
// other members of this class reach the control's property through it.  It is
// the same lookup OpenMFC's C++ COleControlSite::GetProperty performs
// (InvokeHelper with DISPATCH_PROPERTYGET), which is defined in this file.
extern "C" void MS_ABI impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(
    COleControlSite* pThis, long dispid, unsigned short vtProp, void* pvProp) {
    if (!pThis) return;
    pThis->GetProperty(dispid, static_cast<VARTYPE>(vtProp), pvProp);
}

// Symbol: ?AttachWindow@COleControlSite@@QEAAXXZ
// COleControlSite::AttachWindow() — retail 0x239a30:
//   m_pInPlaceObject(+0x88)->GetWindow(&hWnd) through IOleWindow slot 3; on
//   success, and only when the handle differs from m_hWnd(+0x48), it stores
//   the handle in m_hWnd and then, if the retail m_pWndCtrl(+0x50) exists,
//   sets that CWnd's m_hWnd(+0x40), enters it into the handle map returned by
//   the unexported helper at 0x2890e8 (called with bCreate = 1; no export
//   resolves that RVA, so the name is not verified here -- MFC's own name for
//   it is afxMapHWND), calls ?AttachControlSite@CWnd@@IEAAXPEAVCHandleMap@@@Z
//   (0x2a7c90), and back-links pWndCtrl->m_pCtrlSite (CWnd+0xd0).  OpenMFC's
//   COleControlSite has no m_pWndCtrl and its CWnd has no m_pCtrlSite, so only
//   the handle capture is reproduced.  Retail calls
//   ?AfxThrowInvalidArgException@@YAXXZ (0x225b80) when there is no in-place
//   object; OpenMFC returns instead.
extern "C" void MS_ABI impl__AttachWindow_COleControlSite__QEAAXXZ(COleControlSite* pThis) {
    if (!pThis || !pThis->m_lpInPlaceObject) return;
    HWND hWnd = nullptr;
    if (FAILED(pThis->m_lpInPlaceObject->GetWindow(&hWnd))) return;
    if (hWnd == pThis->m_hWnd) return;
    pThis->m_hWnd = hWnd;
}

// Symbol: ?BindDefaultProperty@COleControlSite@@UEAAXJGPEB_WPEAVCWnd@@@Z
// Not implemented.  Retail mfc140u.dll 0x23c050 keeps the default binding in
// four members OpenMFC's COleControlSite does not have: a data-source-control
// site pointer (+0xe8), the bound dispid (+0xf0), its VARTYPE (+0xf4) and the
// field-name CString (+0xf8).  It unbinds whatever is already there (two
// virtual calls on site(+0xe8)->(+0xd8)), then, when pDSCWnd is non-null,
// takes pDSCWnd->m_pCtrlSite (CWnd+0xd0), calls EnableDSC() on it (vtable slot
// 0x178), stores it plus the dispid/VARTYPE/field name, and rebinds.  With no
// member to hold any of that, there is nothing to bind to here.
extern "C" void MS_ABI impl__BindDefaultProperty_COleControlSite__UEAAXJGPEB_WPEAVCWnd___Z(
    void* pThis, long dispid, unsigned short vtProp, const wchar_t* lpszFieldName, void* pDSCWnd) {
    (void)pThis; (void)dispid; (void)vtProp; (void)lpszFieldName; (void)pDSCWnd;
}

// Symbol: ?BindProperty@COleControlSite@@UEAAXJPEAVCWnd@@@Z
// Not implemented.  Retail 0x23a770 (mfc140u.dll 0x23c170) allocates a
// 0x28-byte CDataBoundProperty node, pushes it onto the site's bound-property
// list at +0xe0 (dispid at node+0xc, DSC site at node+0x10, next at node+0x20),
// points it at the data-source control reached through pWndDSC->m_pCtrlSite
// (CWnd+0xd0) and calls EnableDSC() on that site (vtable slot 0x178); a null
// pWndDSC or dispid == -1 takes the other branch, which walks the list and
// unlinks the matching node instead.  OpenMFC has no CDataBoundProperty and no
// bound-property list.
extern "C" void MS_ABI impl__BindProperty_COleControlSite__UEAAXJPEAVCWnd___Z(
    void* pThis, long dispid, void* pDSCWnd) {
    (void)pThis; (void)dispid; (void)pDSCWnd;
}

// Symbol: ?ConnectSink@COleControlSite@@IEAAKAEBU_GUID@@PEAUIUnknown@@@Z
// COleControlSite::ConnectSink() — retail 0x239400:
//   m_pObject(+0x80)->QueryInterface(IID_IConnectionPointContainer) (the GUID
//   at 0x2d7bf8 is B196B284-BAB4-101A-B69C-00AA00341D07), then
//   FindConnectionPoint(iid) (slot 4) and Advise(pUnkSink, &cookie) (slot 5).
//   The cookie slot starts at zero, so a failed FindConnectionPoint or Advise
//   yields 0.  Both interfaces are released before returning.
extern "C" unsigned long MS_ABI impl__ConnectSink_COleControlSite__IEAAKAEBU_GUID__PEAUIUnknown___Z(
    COleControlSite* pThis, const GUID* piid, IUnknown* pUnkSink) {
    if (!pThis || !piid) return 0;
    IConnectionPointContainer* pCPC =
        SiteQueryObject<IConnectionPointContainer>(pThis, IID_IConnectionPointContainer);
    if (!pCPC) return 0;
    DWORD dwCookie = 0;
    IConnectionPoint* pCP = nullptr;
    if (SUCCEEDED(pCPC->FindConnectionPoint(*piid, &pCP)) && pCP) {
        pCP->Advise(pUnkSink, &dwCookie);
        pCP->Release();
    }
    pCPC->Release();
    return dwCookie;
}

// Symbol: ?CreateControlCommon@COleControlSite@@MEAAJPEAVCWnd@@AEBU_GUID@@AEBVCControlCreationInfo@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_W@Z
// Not implemented.  Retail mfc140u.dll 0x239c90 (the ANSI twin is mfc140.dll
// 0x238290) is ~485 instructions (0x238290..0x2389d0) and writes a dozen
// members OpenMFC has no counterpart for: it back-links pWndCtrl->m_pCtrlSite
// (CWnd+0xd0) and stores the CWnd at +0x50, fans out to one of two CreateOrLoad overloads through
// vtable slots 0x1a0/0x1a8 depending on the creation info's kind word, caches
// the control id at +0x58 and the position rect at +0x5c, computes the style
// mask at +0xa8 and m_dwStyle at +0xac from the cached misc status at +0xb0,
// and advises the nested property-notify and event sinks at +0x158/+0x160,
// parking their cookies at +0xa0/+0xa4.  OpenMFC's client site is a separate
// heap-allocated detail::ControlSiteAdapter with none of those subobjects, so
// a body here would be a guess rather than a transcription.
extern "C" long MS_ABI impl__CreateControlCommon_COleControlSite__MEAAJPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(
    void* pThis, void* pWndCtrl, const GUID* pClsid, const void* pCreateInfo, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const POINT* ppt, const SIZE* psize, unsigned int nID, void* pPersist,
    int bStorage, wchar_t* bstrLicKey) {
    (void)pThis; (void)pWndCtrl; (void)pClsid; (void)pCreateInfo; (void)lpszWindowName;
    (void)dwStyle; (void)ppt; (void)psize; (void)nID; (void)pPersist; (void)bStorage; (void)bstrLicKey;
    return E_FAIL;
}

// Symbol: ?CreateOrLoad@COleControlSite@@MEAAJAEBU_GUID@@PEAVCFile@@HPEA_W@Z
// Not implemented.  Retail 0x238d50 (mfc140u.dll 0x23a750) is ~340
// instructions (0x238d50..0x2392a0).  What was actually decoded of it: when
// m_pObject(+0x80) is still null it creates the object, taking CoGetClassObject +
// IClassFactory2::CreateInstanceLic when bstrLicKey has a non-zero
// SysStringLen and plain CoCreateInstance otherwise; then it fills the event
// IID member at +0x6c by calling GetEventIID (0x239570), tries
// QuickActivate through vtable slot 0x190, and on failure falls back to
// IOleObject::GetMiscStatus(DVASPECT_CONTENT) into the cached misc status at
// +0xb0, honouring OLEMISC_SETCLIENTSITEFIRST (0x20000) by calling
// SetClientSite with the nested client site at this+0x138 before anything
// else.  The remainder handles the persist/load path and the sink advises.
// The members it depends on -- the +0xb0 misc-status cache, the +0x6c event
// IID and the nested interface subobjects -- have no OpenMFC counterpart, so
// this is left stubbed rather than half-transcribed.
extern "C" long MS_ABI impl__CreateOrLoad_COleControlSite__MEAAJAEBU_GUID__PEAVCFile__HPEA_W_Z(
    void* pThis, const GUID* pClsid, void* pPersist, int bStorage, wchar_t* bstrLicKey) {
    (void)pThis; (void)pClsid; (void)pPersist; (void)bStorage; (void)bstrLicKey;
    return E_FAIL;
}

// Symbol: ?CreateOrLoad@COleControlSite@@MEAAJAEBVCControlCreationInfo@@@Z
// COleControlSite::CreateOrLoad(const CControlCreationInfo&) — retail 0x238d40
// (mfc140u.dll 0x23a740) is exactly two instructions:
//   mov $0x8000ffff,%eax ; ret
// i.e. an unconditional E_UNEXPECTED, with the creation info never read.  A
// derived site is expected to override it; the base never creates anything.
extern "C" long MS_ABI impl__CreateOrLoad_COleControlSite__MEAAJAEBVCControlCreationInfo___Z(
    void* pThis, const void* pCreateInfo) {
    (void)pThis; (void)pCreateInfo;
    return static_cast<long>(0x8000FFFFL);  // E_UNEXPECTED
}

// Symbol: ?DetachWindow@COleControlSite@@QEAAXXZ
// COleControlSite::DetachWindow() — retail 0x239ad0 clears m_hWnd(+0x48) and
// then, if the retail m_pWndCtrl(+0x50) is attached, unsubclasses/detaches that
// CWnd and clears its m_pCtrlSite back-pointer (CWnd+0xd0).  OpenMFC has no
// m_pWndCtrl, so only the handle is cleared.
extern "C" void MS_ABI impl__DetachWindow_COleControlSite__QEAAXXZ(COleControlSite* pThis) {
    if (!pThis) return;
    pThis->m_hWnd = nullptr;
}

// Symbol: ?DisconnectSink@COleControlSite@@IEAAXAEBU_GUID@@K@Z
// COleControlSite::DisconnectSink() — retail 0x2394c0: a zero cookie returns
// immediately; otherwise QueryInterface(IID_IConnectionPointContainer) on
// m_pObject(+0x80), FindConnectionPoint(iid) (slot 4) and Unadvise(cookie)
// (slot 6), releasing both interfaces.
extern "C" void MS_ABI impl__DisconnectSink_COleControlSite__IEAAXAEBU_GUID__K_Z(
    COleControlSite* pThis, const GUID* piid, unsigned long dwCookie) {
    if (!dwCookie || !pThis || !piid) return;
    IConnectionPointContainer* pCPC =
        SiteQueryObject<IConnectionPointContainer>(pThis, IID_IConnectionPointContainer);
    if (!pCPC) return;
    IConnectionPoint* pCP = nullptr;
    if (SUCCEEDED(pCPC->FindConnectionPoint(*piid, &pCP)) && pCP) {
        pCP->Unadvise(dwCookie);
        pCP->Release();
    }
    pCPC->Release();
}

// Symbol: ?DoVerb@COleControlSite@@UEAAJJPEAUtagMSG@@@Z
// COleControlSite::DoVerb() — retail 0x2392b0:
//   m_pObject(+0x80)->DoVerb(nVerb, lpMsg, (IOleClientSite*)(this+0x138), 0,
//                            m_pCtrlCont(+0x40)->m_pWnd(+0x40)->m_hWnd(+0x40),
//                            (RECT*)(this+0x5c))
//   (IOleObject vtable slot 11 = 0x58).  A null object, container or container
//   window makes retail call AfxThrowInvalidArgException.
//   OpenMFC's client site is the heap-allocated detail::ControlSiteAdapter
//   rather than a nested subobject, and the parent HWND comes from
//   GetSiteParentWindow().
extern "C" long MS_ABI impl__DoVerb_COleControlSite__UEAAJJPEAUtagMSG___Z(
    COleControlSite* pThis, long nVerb, MSG* lpMsg) {
    if (!pThis || !pThis->m_lpObject) return E_UNEXPECTED;
    HWND hwndParent = GetSiteParentWindow(pThis);
    if (!hwndParent) return E_UNEXPECTED;
    ControlSiteAdapter* pAdapter = GetControlSiteAdapter(pThis, true);
    if (!pAdapter) return E_OUTOFMEMORY;
    RECT rc = SiteRect(pThis);
    return pThis->m_lpObject->DoVerb(nVerb, lpMsg, static_cast<IOleClientSite*>(pAdapter),
                                     0, hwndParent, &rc);
}

// Symbol: ?EnableDSC@COleControlSite@@UEAAXXZ
// Not implemented.  Retail 0x23a600 (mfc140u.dll 0x23c000) lazily allocates a
// 0xb8-byte CDataSourceControl (ctor ??0CDataSourceControl@@QEAA@PEAVCOleControlSite@@@Z
// at 0x23b890), stores it at +0xd8 and calls
// ?Initialize@CDataSourceControl@@QEAAJXZ (0x23bc90).  OpenMFC has neither the
// class nor a member to hold it, so there is nothing to enable.
extern "C" void MS_ABI impl__EnableDSC_COleControlSite__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?FreezeEvents@COleControlSite@@QEAAXH@Z
// COleControlSite::FreezeEvents() — retail 0x2399c0: QueryInterface
// IID_IOleControl (GUID at 0x2d7c38 = B196B288-BAB4-101A-B69C-00AA00341D07) on
// m_pObject(+0x80), then IOleControl::FreezeEvents(bFreeze) (vtable slot 6 =
// 0x30) and Release.
extern "C" void MS_ABI impl__FreezeEvents_COleControlSite__QEAAXH_Z(COleControlSite* pThis, int bFreeze) {
    IOleControl* pControl = SiteQueryObject<IOleControl>(pThis, IID_IOleControl);
    if (!pControl) return;
    pControl->FreezeEvents(bFreeze);
    pControl->Release();
}

// Symbol: ?GetControlInfo@COleControlSite@@IEAAXXZ
// COleControlSite::GetControlInfo() — retail 0x2397b0: zero the cached
// CONTROLINFO at +0xb8, set cb = 0x18, then QueryInterface(IID_IOleControl) on
// m_pObject(+0x80) and IOleControl::GetControlInfo(&m_ctlInfo) (slot 3 = 0x18).
// OpenMFC keeps the cache in the file-local side table instead of a member.
extern "C" void MS_ABI impl__GetControlInfo_COleControlSite__IEAAXXZ(COleControlSite* pThis) {
    if (!pThis) return;
    CONTROLINFO& info = SiteExtraOf(pThis).ctlInfo;
    memset(&info, 0, sizeof(info));
    info.cb = sizeof(CONTROLINFO);
    IOleControl* pControl = SiteQueryObject<IOleControl>(pThis, IID_IOleControl);
    if (!pControl) return;
    pControl->GetControlInfo(&info);
    pControl->Release();
}

// Symbol: ?GetCursor@COleControlSite@@IEAAJJPEAPEAUIUnknown@@PEAPEAX@Z
// Not implemented.  Retail 0x23e8f0 (mfc140u.dll 0x2402f0) clears *ppReserved,
// walks the bound-property list at +0xe0 for the node whose dispid (node+0x0c)
// matches, and on a hit stores CDataBoundProperty::GetCursor() (0x23e880) into
// *ppUnkCursor.  It returns 0 on every path, and on a miss it leaves
// *ppUnkCursor untouched.  OpenMFC has no bound-property list, so the miss
// path is the only one reachable; *ppUnkCursor is cleared here rather than
// left undefined, which is the one deliberate departure from retail.
extern "C" long MS_ABI impl__GetCursor_COleControlSite__IEAAJJPEAPEAUIUnknown__PEAPEAX_Z(
    void* pThis, long dispid, IUnknown** ppUnkCursor, void** ppReserved) {
    (void)pThis; (void)dispid;
    if (ppReserved) *ppReserved = nullptr;   // retail clears this out-parameter first
    if (ppUnkCursor) *ppUnkCursor = nullptr; // see the note above: retail does not
    return S_OK;                             // retail returns 0 on every path
}

// Symbol: ?GetDefBtnCode@COleControlSite@@QEAAKXZ
// COleControlSite::GetDefBtnCode() — retail 0x239340:
//   if (!(m_dwMiscStatus(+0xb0) & OLEMISC_ACTSLIKEBUTTON)) return 0;
//   return ((~m_dwStyle(+0xac) & 1) + 1) << 4;
// i.e. DLGC_DEFPUSHBUTTON (0x10) when BS_DEFPUSHBUTTON is set in m_dwStyle and
// DLGC_UNDEFPUSHBUTTON (0x20) when it is not.
extern "C" unsigned long MS_ABI impl__GetDefBtnCode_COleControlSite__QEAAKXZ(COleControlSite* pThis) {
    if (!pThis) return 0;
    if (!(SiteMiscStatus(pThis) & OLEMISC_ACTSLIKEBUTTON)) return 0;
    return (pThis->m_dwStyle & BS_DEFPUSHBUTTON) ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON;
}

// Symbol: ?GetDlgCtrlID@COleControlSite@@UEBAHXZ
// COleControlSite::GetDlgCtrlID() — retail 0x23a1a0 is a single load of the
// control id at +0x58 (`mov 0x58(%rcx),%eax; ret`).  OpenMFC has no such
// member; the id is read from the file-local SiteExtra mirror at the top of
// this file, which SetSiteControlId() keeps in step with the shared
// detail::olecore per-site table that COleControlContainer reads.
extern "C" int MS_ABI impl__GetDlgCtrlID_COleControlSite__UEBAHXZ(COleControlSite* pThis) {
    return static_cast<int>(SiteControlId(pThis));
}

// Symbol: ?GetEventIID@COleControlSite@@QEAAHPEAU_GUID@@@Z
// COleControlSite::GetEventIID() — retail 0x239570:
//   *pIID = GUID_NULL;
//   if IProvideClassInfo2 (0x2d7c28 = A6BC3AC0-DBAA-11CE-9DE3-00AA004BB851) is
//       available, call GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, pIID) (slot 4);
//   if that left a non-null GUID (memcmp against GUID_NULL) return TRUE;
//   otherwise fall back to IProvideClassInfo (0x2d7c18 = B196B283-...):
//       GetClassInfo -> ITypeInfo::GetTypeAttr, scan cImplTypes (TYPEATTR+0x34)
//       for the first implemented type whose GetImplTypeFlags(i) & 7 == 3
//       (IMPLTYPEFLAG_FDEFAULT|IMPLTYPEFLAG_FSOURCE), then
//       GetRefTypeOfImplType -> GetRefTypeInfo -> GetTypeAttr and copy that
//       type's guid into *pIID.  Only the first matching entry is examined.
//   The result is again memcmp'd against GUID_NULL to build the BOOL return.
extern "C" int MS_ABI impl__GetEventIID_COleControlSite__QEAAHPEAU_GUID___Z(
    COleControlSite* pThis, GUID* pIID) {
    if (!pIID) return FALSE;
    memset(pIID, 0, sizeof(GUID));
    if (!pThis || !pThis->m_lpObject) return FALSE;

    IProvideClassInfo2* pPCI2 = SiteQueryObject<IProvideClassInfo2>(pThis, IID_IProvideClassInfo2);
    if (pPCI2) {
        pPCI2->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, pIID);
        pPCI2->Release();
    }
    static const GUID kNullGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
    if (memcmp(pIID, &kNullGuid, sizeof(GUID)) != 0) return TRUE;

    IProvideClassInfo* pPCI = SiteQueryObject<IProvideClassInfo>(pThis, IID_IProvideClassInfo);
    if (!pPCI) return FALSE;
    ITypeInfo* pClassInfo = nullptr;
    if (SUCCEEDED(pPCI->GetClassInfo(&pClassInfo)) && pClassInfo) {
        TYPEATTR* pAttr = nullptr;
        if (SUCCEEDED(pClassInfo->GetTypeAttr(&pAttr)) && pAttr) {
            for (UINT i = 0; i < pAttr->cImplTypes; ++i) {
                int flags = 0;
                if (FAILED(pClassInfo->GetImplTypeFlags(static_cast<UINT>(i), &flags))) continue;
                if ((flags & 7) != (IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE)) continue;
                HREFTYPE href = 0;
                ITypeInfo* pEventInfo = nullptr;
                if (SUCCEEDED(pClassInfo->GetRefTypeOfImplType(static_cast<UINT>(i), &href)) &&
                    SUCCEEDED(pClassInfo->GetRefTypeInfo(href, &pEventInfo)) && pEventInfo) {
                    TYPEATTR* pEventAttr = nullptr;
                    if (SUCCEEDED(pEventInfo->GetTypeAttr(&pEventAttr)) && pEventAttr) {
                        *pIID = pEventAttr->guid;
                        pEventInfo->ReleaseTypeAttr(pEventAttr);
                    }
                    pEventInfo->Release();
                }
                break;
            }
            pClassInfo->ReleaseTypeAttr(pAttr);
        }
        pClassInfo->Release();
    }
    pPCI->Release();
    return memcmp(pIID, &kNullGuid, sizeof(GUID)) != 0;
}

// Symbol: ?GetExStyle@COleControlSite@@UEBAKXZ
// COleControlSite::GetExStyle() — retail 0x239f10:
//   DWORD ex = ::GetWindowLong(m_hWnd(+0x48), GWL_EXSTYLE);
//   short appearance = 0;
//   GetProperty(DISPID_APPEARANCE, VT_I2, &appearance);   // vtable slot 0xe8
//   if (appearance == 1) ex |= 0x200;                     // WS_EX_CLIENTEDGE
//   return ex;
extern "C" unsigned long MS_ABI impl__GetExStyle_COleControlSite__UEBAKXZ(COleControlSite* pThis) {
    if (!pThis) return 0;
    DWORD dwExStyle = static_cast<DWORD>(::GetWindowLongW(pThis->m_hWnd, GWL_EXSTYLE));
    short nAppearance = 0;
    pThis->GetProperty(DISPID_APPEARANCE, VT_I2, &nAppearance);
    if (nAppearance == 1) dwExStyle |= WS_EX_CLIENTEDGE;
    return dwExStyle;
}

// Symbol: ?GetStyle@COleControlSite@@UEBAKXZ
// COleControlSite::GetStyle() — retail 0x239e20:
//   DWORD style = m_dwStyle(+0xac);
//   if (*(int*)(this+0xb4) == 0)
//       style |= ::GetWindowLong(m_hWnd, GWL_STYLE) & 0x18000000;  // VISIBLE|DISABLED
//   BOOL enabled = TRUE;
//   GetProperty(DISPID_ENABLED, VT_BOOL, &enabled);
//   enabled ? style &= ~WS_DISABLED : style |= WS_DISABLED;
//   short border = 0;
//   GetProperty(DISPID_BORDERSTYLE, VT_I2, &border);
//   if (border == 1) style |= WS_BORDER;
// The gate at +0xb4 is the windowless-active flag identified in the file header
// above; OpenMFC has no such member and gates on "the site has a window"
// instead, which is a one-way approximation of it.
extern "C" unsigned long MS_ABI impl__GetStyle_COleControlSite__UEBAKXZ(COleControlSite* pThis) {
    if (!pThis) return 0;
    DWORD dwStyle = pThis->m_dwStyle;
    if (pThis->m_hWnd) {
        dwStyle |= static_cast<DWORD>(::GetWindowLongW(pThis->m_hWnd, GWL_STYLE)) &
                   static_cast<DWORD>(WS_VISIBLE | WS_DISABLED);
    }
    BOOL bEnabled = TRUE;
    pThis->GetProperty(DISPID_ENABLED, VT_BOOL, &bEnabled);
    if (bEnabled) {
        dwStyle &= ~static_cast<DWORD>(WS_DISABLED);
    } else {
        dwStyle |= static_cast<DWORD>(WS_DISABLED);
    }
    short nBorder = 0;
    pThis->GetProperty(DISPID_BORDERSTYLE, VT_I2, &nBorder);
    if (nBorder == 1) dwStyle |= static_cast<DWORD>(WS_BORDER);
    return dwStyle;
}

// Symbol: ?GetWindowTextW@COleControlSite@@UEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// COleControlSite::GetWindowText(CString&) — retail mfc140u.dll 0x23bb60 is
// four instructions: GetProperty(DISPID_CAPTION (-518), VT_BSTR (8), &str)
// through vtable slot 0xe8, and nothing else.  (The ANSI twin
// ?GetWindowTextA@COleControlSite@@... at mfc140.dll 0x23a160 is identical.)
// Retail's InvokeHelperV writes the CString itself (the VT_BSTR return arm at
// 0x25151b assigns from the BSTR and then frees it), while OpenMFC's shared
// CopyDispatchResult hands VT_BSTR back as a raw BSTR, so the BSTR is adopted
// and copied into the caller's string here.  One divergence follows from that:
// retail throws out of a failed property get and leaves the caller's CString
// untouched, whereas OpenMFC swallows the HRESULT and this body then assigns an
// empty string.
extern "C" void MS_ABI impl__GetWindowTextW_COleControlSite__UEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COleControlSite* pThis, CString* pString) {
    if (!pString) return;
    BSTR bstr = nullptr;
    if (pThis) pThis->GetProperty(DISPID_CAPTION, VT_BSTR, &bstr);
    *pString = bstr ? bstr : L"";
    if (bstr) SysFreeString(bstr);
}

// Symbol: ?IsDefaultButton@COleControlSite@@QEAAHXZ
// COleControlSite::IsDefaultButton() — retail 0x239320:
//   (m_dwMiscStatus(+0xb0) & OLEMISC_ACTSLIKEBUTTON) &&
//   (m_dwStyle(+0xac) & BS_DEFPUSHBUTTON)
extern "C" int MS_ABI impl__IsDefaultButton_COleControlSite__QEAAHXZ(COleControlSite* pThis) {
    if (!pThis) return FALSE;
    if (!(SiteMiscStatus(pThis) & OLEMISC_ACTSLIKEBUTTON)) return FALSE;
    return (pThis->m_dwStyle & BS_DEFPUSHBUTTON) ? TRUE : FALSE;
}

// Symbol: ?IsMatchingMnemonic@COleControlSite@@QEAAHPEAUtagMSG@@@Z
// COleControlSite::IsMatchingMnemonic() — retail 0x239850:
//   if (m_ctlInfo.cAccel(+0xc8) == 0 || m_ctlInfo.hAccel(+0xc0) == NULL) return FALSE;
//   ACCEL* p = new ACCEL[cAccel];                       // cAccel * 6 bytes
//   int n = ::CopyAcceleratorTable(hAccel, p, cAccel);
//   UINT want = (pMsg->message == WM_SYSCHAR) ? FALT : 0;
//   for (i = 0; i < n; i++)
//       if ((p[i].fVirt & ~FNOINVERT) == want && p[i].key == LOWORD(pMsg->wParam))
//           { found = TRUE; break; }
//   delete[] p; return found;
// The CONTROLINFO cache lives in the file-local side table here; it is filled
// by GetControlInfo().
extern "C" int MS_ABI impl__IsMatchingMnemonic_COleControlSite__QEAAHPEAUtagMSG___Z(
    COleControlSite* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) return FALSE;
    const CONTROLINFO& info = SiteExtraOf(pThis).ctlInfo;
    if (info.cAccel == 0 || info.hAccel == nullptr) return FALSE;
    std::vector<ACCEL> accel(static_cast<size_t>(info.cAccel));
    int nCopied = ::CopyAcceleratorTableW(info.hAccel, accel.data(), info.cAccel);
    const BYTE want = (pMsg->message == WM_SYSCHAR) ? static_cast<BYTE>(FALT) : static_cast<BYTE>(0);
    for (int i = 0; i < nCopied; ++i) {
        if (static_cast<BYTE>(accel[static_cast<size_t>(i)].fVirt & ~static_cast<BYTE>(FNOINVERT)) != want) continue;
        if (accel[static_cast<size_t>(i)].key == static_cast<WORD>(pMsg->wParam)) return TRUE;
    }
    return FALSE;
}

// Symbol: ?IsWindowEnabled@COleControlSite@@UEBAHXZ
// COleControlSite::IsWindowEnabled() — retail 0x23a3f0:
//   int enabled = 1;
//   if (*(int*)(this+0xb4) == 0) enabled = ::IsWindowEnabled(m_hWnd(+0x48));
//   GetProperty(DISPID_ENABLED, VT_BOOL, &enabled);   // vtable slot 0xe8
//   return enabled;                                   // the raw 4-byte slot
// The scratch slot really is a 4-byte int seeded with 1 (0x23a3f9), and
// GetProperty is asked for VT_BOOL (r8d == 0xb at 0x23a41b); the slot is
// returned verbatim, with no normalisation of its own.  The dispatch-result
// copy that fills it was decoded: COleControlSite::GetProperty (0x239d70)
// -> InvokeHelper (slot 0xe0) -> InvokeHelperV (0x239bf0) -> the shared
// COleDispatchDriver::InvokeHelperV (0x250ef0), whose VT_BOOL return arm at
// 0x251568 is `cmp %si,0x58(%rsp); setne %cl; mov %ecx,(pvRet)` -- a full
// 4-byte 0 or 1, not 0xFFFF -- after the result has been coerced to the
// requested VARTYPE by VariantChangeType (oleaut32 ordinal 12, called at
// 0x2514a4).  So retail's return is 0 or 1 and this body matches it.  One
// difference does survive, and it is not in this file: OpenMFC's shared
// CopyDispatchResult (detail/OlecoreSupport.cpp) neither coerces nor uses
// retail's `!= 0` test -- it compares the raw boolVal against VARIANT_TRUE --
// so a control answering VARIANT_BOOL 1 reads as FALSE here and TRUE in
// retail.  As in GetStyle, the +0xb4 windowless-active flag is replaced by
// "the site has a window".
extern "C" int MS_ABI impl__IsWindowEnabled_COleControlSite__UEBAHXZ(COleControlSite* pThis) {
    if (!pThis) return FALSE;
    BOOL bEnabled = TRUE;
    if (pThis->m_hWnd) bEnabled = ::IsWindowEnabled(pThis->m_hWnd);
    pThis->GetProperty(DISPID_ENABLED, VT_BOOL, &bEnabled);
    return bEnabled ? TRUE : FALSE;
}

// Symbol: ?ModifyStyle@COleControlSite@@UEAAHKKI@Z
// COleControlSite::ModifyStyle() — retail 0x239f80:
//   m_dwStyle = ((m_dwStyle & ~dwRemove) | dwAdd) & m_dwStyleMask(+0xa8);
//   if ((dwRemove|dwAdd) & WS_DISABLED)
//       if (SafeSetProperty(DISPID_ENABLED, VT_BOOL, ~dwAdd & WS_DISABLED))
//           dwRemove &= ~WS_DISABLED, dwAdd &= ~WS_DISABLED;
//   if ((dwRemove|dwAdd) & WS_BORDER)
//       if (SafeSetProperty(DISPID_BORDERSTYLE, VT_I2, dwAdd & WS_BORDER))
//           dwRemove &= ~WS_BORDER, dwAdd &= ~WS_BORDER;
//   return m_hWnd ? AfxModifyStyle(m_hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags)
//                 : TRUE;
// The literal values retail passes to SafeSetProperty are kept: the border
// value really is dwAdd & WS_BORDER (0x800000), not a normalised 0/1.
// OpenMFC has no style mask (retail +0xa8), so no mask is applied.
extern "C" int MS_ABI impl__ModifyStyle_COleControlSite__UEAAHKKI_Z(
    COleControlSite* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags) {
    if (!pThis) return FALSE;
    pThis->m_dwStyle = (pThis->m_dwStyle & ~dwRemove) | dwAdd;
    if ((dwRemove | dwAdd) & static_cast<unsigned long>(WS_DISABLED)) {
        if (SiteSafeSetProperty(pThis, DISPID_ENABLED, VT_BOOL,
                                static_cast<int>(~dwAdd & static_cast<unsigned long>(WS_DISABLED)))) {
            dwRemove &= ~static_cast<unsigned long>(WS_DISABLED);
            dwAdd    &= ~static_cast<unsigned long>(WS_DISABLED);
        }
    }
    if ((dwRemove | dwAdd) & static_cast<unsigned long>(WS_BORDER)) {
        if (SiteSafeSetProperty(pThis, DISPID_BORDERSTYLE, VT_I2,
                                static_cast<int>(dwAdd & static_cast<unsigned long>(WS_BORDER)))) {
            dwRemove &= ~static_cast<unsigned long>(WS_BORDER);
            dwAdd    &= ~static_cast<unsigned long>(WS_BORDER);
        }
    }
    if (!pThis->m_hWnd) return TRUE;
    return SiteModifyWindowStyle(pThis->m_hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);
}

// Symbol: ?ModifyStyleEx@COleControlSite@@UEAAHKKI@Z
// COleControlSite::ModifyStyleEx() — retail 0x23a060:
//   if ((dwRemove|dwAdd) & WS_EX_CLIENTEDGE)
//       if (SafeSetProperty(DISPID_APPEARANCE, VT_I2, dwAdd & WS_EX_CLIENTEDGE))
//           dwRemove &= ~WS_EX_CLIENTEDGE, dwAdd &= ~WS_EX_CLIENTEDGE;
//   return m_hWnd ? AfxModifyStyle(m_hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags)
//                 : TRUE;
// Note that retail never touches m_dwStyle here, and that the appearance value
// it passes is the raw dwAdd & 0x200 rather than 0/1.
extern "C" int MS_ABI impl__ModifyStyleEx_COleControlSite__UEAAHKKI_Z(
    COleControlSite* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags) {
    if (!pThis) return FALSE;
    if ((dwRemove | dwAdd) & static_cast<unsigned long>(WS_EX_CLIENTEDGE)) {
        if (SiteSafeSetProperty(pThis, DISPID_APPEARANCE, VT_I2,
                                static_cast<int>(dwAdd & static_cast<unsigned long>(WS_EX_CLIENTEDGE)))) {
            dwRemove &= ~static_cast<unsigned long>(WS_EX_CLIENTEDGE);
            dwAdd    &= ~static_cast<unsigned long>(WS_EX_CLIENTEDGE);
        }
    }
    if (!pThis->m_hWnd) return TRUE;
    return SiteModifyWindowStyle(pThis->m_hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
}

// Symbol: ?MoveWindow@COleControlSite@@UEAAXHHHH@Z
// COleControlSite::MoveWindow() — retail 0x23a1c0:
//   RECT saved = m_rect(+0x5c);
//   ::SetRect(&m_rect, x, y, x + cx, y + cy);
//   if (!SetExtent()) { m_rect = saved; return; }        // SetExtent may resize m_rect
//   ::SetRect(&m_rect, x, y, x + Width(m_rect), y + Height(m_rect));
//   m_pInPlaceObject(+0x88)->SetObjectRects(&m_rect, &m_rect);   // slot 7 = 0x38
//   A null in-place object or object makes retail throw.
extern "C" void MS_ABI impl__MoveWindow_COleControlSite__UEAAXHHHH_Z(
    COleControlSite* pThis, int x, int y, int cx, int cy) {
    if (!pThis || !pThis->m_lpInPlaceObject || !pThis->m_lpObject) return;
    RECT& rect = SiteRect(pThis);
    const RECT saved = rect;
    ::SetRect(&rect, x, y, x + cx, y + cy);
    if (!impl__SetExtent_COleControlSite__IEAAHXZ(pThis)) {
        rect = saved;
        return;
    }
    ::SetRect(&rect, x, y, x + (rect.right - rect.left), y + (rect.bottom - rect.top));
    pThis->m_lpInPlaceObject->SetObjectRects(&rect, &rect);
}

// Symbol: ?OnEvent@COleControlSite@@IEAAHPEAUAFX_EVENT@@@Z
// Not implemented.  Retail 0x239b50 (mfc140u.dll 0x23b550) routes the event
// through OnCmdMsg(m_nID(+0x58), CN_EVENT (-2), pEvent, NULL) — vtable slot
// 0x28 — first on the retail m_pWndCtrl(+0x50); if that returns non-zero it
// returns TRUE, otherwise it repeats the same call on
// m_pCtrlCont(+0x40)->m_pWnd(+0x40) and returns *that* call's result verbatim
// (0x239bdd jumps past the `mov $1,%eax`).  With neither window it throws.
// OpenMFC has no m_pWndCtrl and no CN_EVENT command route carrying an
// AFX_EVENT, so no handler can ever claim the event; FALSE is what retail's
// own second branch produces in exactly that situation, so that is what is
// returned rather than an invented TRUE.
extern "C" int MS_ABI impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(void* pThis, void* pEvent) {
    (void)pThis; (void)pEvent;
    return FALSE;
}

// Symbol: ?QuickActivate@COleControlSite@@UEAAHXZ
// Not implemented.  Retail 0x238a70 zeroes a 0x70-byte QACONTAINER, writes
// cbSize = 0x70, and fills three of its fields with pointers to nested
// interface subobjects embedded in the retail COleControlSite: pClientSite =
// this+0x138, pPropertyNotifySink = this+0x158, pUnkEventSink = this+0x160
// (pAdviseSink is left NULL).  It then reads eight (dispid, flag-bit) pairs
// out of the table at 0x346c50 — reference confirmed at 0x238b5c — asks the
// container window for each one through CWnd::OnAmbientProperty (its vtable
// slot 0x230) and ORs the flag bit into QACONTAINER::dwAmbientFlags (+0x28)
// when the ambient came back non-zero, fills colorFore (+0x2c) and colorBack
// (+0x30) from DISPID_AMBIENT_FORECOLOR/BACKCOLOR, and calls
// IQuickActivate::QuickActivate (the IID blob at 0x2d7d88 decodes to
// CF51ED10-62FE-11CF-BF86-00A0C9034836).  OpenMFC's client site is a separate
// heap-allocated adapter with a different layout and has no property-notify or
// event subobject to point at, so the QACONTAINER cannot be filled faithfully
// and this is left stubbed rather than approximated.
extern "C" int MS_ABI impl__QuickActivate_COleControlSite__UEAAHXZ(void* pThis) {
    (void)pThis;
    return FALSE;
}

// Symbol: ?SafeSetProperty@COleControlSite@@UEAAHJGZZ
// COleControlSite::SafeSetProperty() — retail 0x239de0 forwards the varargs to
// SetPropertyV (vtable slot 0xd8) and returns TRUE (mov $0x1,%eax at
// 0x239e09); the exception continuation at 0x239e10 returns whatever the catch
// funclet left at [rsp+0x30], which the documented contract says is 0.
// OpenMFC's InvokeHelperV reports failure as an HRESULT instead of throwing, so
// the SUCCEEDED() of that HRESULT is the return.  See SiteSafeSetProperty at
// the top of this file.
extern "C" int MS_ABI impl__SafeSetProperty_COleControlSite__UEAAHJGZZ(
    COleControlSite* pThis, long dispid, unsigned short vtProp, ...) {
    va_list argList;
    va_start(argList, vtProp);
    HRESULT hr = SitePutPropertyV(pThis, dispid, static_cast<VARTYPE>(vtProp), argList);
    va_end(argList);
    return SUCCEEDED(hr);
}

// Symbol: ?SendMnemonic@COleControlSite@@QEAAXPEAUtagMSG@@@Z
// COleControlSite::SendMnemonic() — retail 0x239920:
//   if (!(m_dwMiscStatus(+0xb0) & OLEMISC_NOUIACTIVATE)) SetFocus();  // slot 0x170
//   QueryInterface(IID_IOleControl) on m_pObject(+0x80), then
//   IOleControl::OnMnemonic(pMsg) (vtable slot 4 = 0x20) and Release.
extern "C" void MS_ABI impl__SendMnemonic_COleControlSite__QEAAXPEAUtagMSG___Z(
    COleControlSite* pThis, MSG* pMsg) {
    if (!pThis) return;
    if (!(SiteMiscStatus(pThis) & OLEMISC_NOUIACTIVATE)) {
        impl__SetFocus_COleControlSite__UEAAPEAVCWnd__XZ(pThis);
    }
    IOleControl* pControl = SiteQueryObject<IOleControl>(pThis, IID_IOleControl);
    if (!pControl) return;
    pControl->OnMnemonic(pMsg);
    pControl->Release();
}

// Symbol: ?SetDefaultButton@COleControlSite@@QEAAXH@Z
// COleControlSite::SetDefaultButton() — retail 0x239360:
//   if (!(m_dwMiscStatus(+0xb0) & OLEMISC_ACTSLIKEBUTTON)) return;
//   if (!!(m_dwStyle(+0xac) & BS_DEFPUSHBUTTON) == !!bDefault) return;
//   m_dwStyle ^= BS_DEFPUSHBUTTON;
//   QueryInterface(IID_IOleControl) on m_pObject(+0x80), then
//   IOleControl::OnAmbientPropertyChange(DISPID_AMBIENT_DISPLAYASDEFAULT)
//   (vtable slot 5 = 0x28) and Release.
extern "C" void MS_ABI impl__SetDefaultButton_COleControlSite__QEAAXH_Z(
    COleControlSite* pThis, int bDefault) {
    if (!pThis) return;
    if (!(SiteMiscStatus(pThis) & OLEMISC_ACTSLIKEBUTTON)) return;
    const BOOL bCurrent = (pThis->m_dwStyle & BS_DEFPUSHBUTTON) ? TRUE : FALSE;
    if (bCurrent == (bDefault ? TRUE : FALSE)) return;
    pThis->m_dwStyle ^= static_cast<DWORD>(BS_DEFPUSHBUTTON);
    IOleControl* pControl = SiteQueryObject<IOleControl>(pThis, IID_IOleControl);
    if (!pControl) return;
    pControl->OnAmbientPropertyChange(DISPID_AMBIENT_DISPLAYASDEFAULT);
    pControl->Release();
}

// Symbol: ?SetDlgCtrlID@COleControlSite@@UEAAHH@Z
// COleControlSite::SetDlgCtrlID() — retail 0x23a1b0 returns the old id at +0x58
// and stores the new one (`mov 0x58(%rcx),%eax; mov %edx,0x58(%rcx); ret`).
// OpenMFC has no such member, so SetSiteControlId() writes both the file-local
// mirror this file reads and the shared detail::olecore table that
// COleControlContainer::GetDlgItem resolves ids through.
extern "C" int MS_ABI impl__SetDlgCtrlID_COleControlSite__UEAAHH_Z(COleControlSite* pThis, int nID) {
    if (!pThis) return 0;
    const UINT nOld = SiteControlId(pThis);
    SetSiteControlId(pThis, static_cast<UINT>(nID));
    return static_cast<int>(nOld);
}

// Symbol: ?SetExtent@COleControlSite@@IEAAHXZ
// COleControlSite::SetExtent() — retail 0x237f70:
//   SIZE size = { Width(m_rect(+0x5c)), Height(m_rect) };
//   CClientDC dc(NULL); dc.DPtoHIMETRIC(&size);
//   HRESULT hr = m_pObject(+0x80)->SetExtent(DVASPECT_CONTENT, &size);  // slot 17
//   if (SUCCEEDED(hr) &&
//       SUCCEEDED(m_pObject->GetExtent(DVASPECT_CONTENT, &size))) {     // slot 18
//       dc.HIMETRICtoDP(&size);
//       m_rect.right  = m_rect.left + size.cx;
//       m_rect.bottom = m_rect.top  + size.cy;
//   }
//   return SUCCEEDED(hr);      // hr is SetExtent's; GetExtent's is discarded
// (0x237fd9 latches SetExtent's HRESULT into ebx and 0x238022 turns only that
// one into the BOOL -- a failing GetExtent leaves the rect alone but still
// returns TRUE.)
// CDC::DPtoHIMETRIC/HIMETRICtoDP are the MFC MulDiv conversions against
// LOGPIXELSX/LOGPIXELSY of the screen DC; they are inlined here because CDC's
// members are not linkable from inside the DLL.
extern "C" int MS_ABI impl__SetExtent_COleControlSite__IEAAHXZ(COleControlSite* pThis) {
    if (!pThis || !pThis->m_lpObject) return FALSE;
    RECT& rect = SiteRect(pThis);
    SIZE size;
    size.cx = rect.right - rect.left;
    size.cy = rect.bottom - rect.top;

    HDC hdcScreen = ::GetDC(nullptr);
    int nPixelsX = hdcScreen ? ::GetDeviceCaps(hdcScreen, LOGPIXELSX) : 96;
    int nPixelsY = hdcScreen ? ::GetDeviceCaps(hdcScreen, LOGPIXELSY) : 96;
    if (hdcScreen) ::ReleaseDC(nullptr, hdcScreen);
    if (nPixelsX <= 0) nPixelsX = 96;
    if (nPixelsY <= 0) nPixelsY = 96;

    size.cx = ::MulDiv(size.cx, 2540, nPixelsX);   // DPtoHIMETRIC
    size.cy = ::MulDiv(size.cy, 2540, nPixelsY);

    const HRESULT hr = pThis->m_lpObject->SetExtent(DVASPECT_CONTENT, &size);
    if (SUCCEEDED(hr) && SUCCEEDED(pThis->m_lpObject->GetExtent(DVASPECT_CONTENT, &size))) {
        size.cx = ::MulDiv(size.cx, nPixelsX, 2540);   // HIMETRICtoDP
        size.cy = ::MulDiv(size.cy, nPixelsY, 2540);
        rect.right  = rect.left + size.cx;
        rect.bottom = rect.top + size.cy;
    }
    return SUCCEEDED(hr);
}

// Symbol: ?SetFocus@COleControlSite@@UEAAPEAVCWnd@@PEAUtagMSG@@@Z
// COleControlSite::SetFocus(MSG*) — retail 0x23a560:
//   if ((m_dwMiscStatus(+0xb0) & OLEMISC_NOUIACTIVATE) && *(int*)(this+0xb4) == 0)
//       return CWnd::FromHandle(::SetFocus(m_hWnd(+0x48)));
//   CWnd* pOld = CWnd::FromHandle(::GetFocus());
//   if (!(m_dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME))
//       if (SUCCEEDED(DoVerb(OLEIVERB_UIACTIVATE, pMsg)))     // vtable slot 0xc8
//           m_pCtrlCont(+0x40)->m_pSiteUIActive(+0xd8) = this;
//   return pOld;
// OpenMFC's COleControlContainer has no UI-active-site member, so that
// bookkeeping is dropped; the +0xb4 windowless-active flag is replaced by "the site
// has a window", as elsewhere in this class.
extern "C" CWnd* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__PEAUtagMSG___Z(
    COleControlSite* pThis, MSG* pMsg) {
    if (!pThis) return nullptr;
    const DWORD dwMisc = SiteMiscStatus(pThis);
    if ((dwMisc & OLEMISC_NOUIACTIVATE) && pThis->m_hWnd) {
        return CWnd::FromHandle(::SetFocus(pThis->m_hWnd));
    }
    CWnd* pOldFocus = CWnd::FromHandle(::GetFocus());
    if (!(dwMisc & OLEMISC_INVISIBLEATRUNTIME)) {
        impl__DoVerb_COleControlSite__UEAAJJPEAUtagMSG___Z(pThis, OLEIVERB_UIACTIVATE, pMsg);
    }
    return pOldFocus;
}

// Symbol: ?SetFocus@COleControlSite@@UEAAPEAVCWnd@@XZ
// COleControlSite::SetFocus() — retail 0x23a4d0 is the same body as
// SetFocus(MSG*) above with a null message pointer.
extern "C" CWnd* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__XZ(COleControlSite* pThis) {
    return impl__SetFocus_COleControlSite__UEAAPEAVCWnd__PEAUtagMSG___Z(pThis, nullptr);
}

// Symbol: ?SetPropertyV@COleControlSite@@UEAAXJGPEAD@Z
// COleControlSite::SetPropertyV() — retail 0x239ca0.  The VARTYPE-to-parameter
// -code and PROPERTYPUT/PROPERTYPUTREF rules are transcribed in
// SitePutPropertyV() at the top of this file.  The exported signature takes the
// va_list as a char*, which is exactly what a Microsoft-x64 va_list is.
extern "C" void MS_ABI impl__SetPropertyV_COleControlSite__UEAAXJGPEAD_Z(
    COleControlSite* pThis, long dispid, unsigned short vtProp, char* pArgList) {
    va_list argList;
    memcpy(&argList, &pArgList, sizeof(argList));
    SitePutPropertyV(pThis, dispid, static_cast<VARTYPE>(vtProp), argList);
}

// Symbol: ?SetWindowPos@COleControlSite@@UEAAHPEBVCWnd@@HHHHI@Z
// COleControlSite::SetWindowPos() — retail 0x23a280:
//   if (nFlags & SWP_HIDEWINDOW) ShowWindow(SW_HIDE);                  // slot 0x150
//   if ((nFlags & (SWP_NOSIZE|SWP_NOMOVE)) != (SWP_NOSIZE|SWP_NOMOVE))
//       MoveWindow(nFlags & SWP_NOMOVE ? m_rect.left : x,              // slot 0x140
//                  nFlags & SWP_NOMOVE ? m_rect.top  : y,
//                  nFlags & SWP_NOSIZE ? Width(m_rect)  : cx,
//                  nFlags & SWP_NOSIZE ? Height(m_rect) : cy);
//   if (nFlags & SWP_SHOWWINDOW) ShowWindow(SW_SHOW);
//   nFlags = (nFlags & ~(SWP_SHOWWINDOW|SWP_HIDEWINDOW)) | SWP_NOSIZE | SWP_NOMOVE;
//   return ::SetWindowPos(m_hWnd(+0x48),
//                         pWndInsertAfter ? pWndInsertAfter->m_hWnd : NULL,
//                         x, y, cx, cy, nFlags);      // a tail jump in retail
extern "C" int MS_ABI impl__SetWindowPos_COleControlSite__UEAAHPEBVCWnd__HHHHI_Z(
    COleControlSite* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags) {
    if (!pThis) return FALSE;
    if (nFlags & SWP_HIDEWINDOW) {
        impl__ShowWindow_COleControlSite__UEAAHH_Z(pThis, SW_HIDE);
    }
    if ((nFlags & (SWP_NOSIZE | SWP_NOMOVE)) != (SWP_NOSIZE | SWP_NOMOVE)) {
        const RECT& rect = SiteRect(pThis);
        const int mx  = (nFlags & SWP_NOMOVE) ? rect.left : x;
        const int my  = (nFlags & SWP_NOMOVE) ? rect.top : y;
        const int mcx = (nFlags & SWP_NOSIZE) ? (rect.right - rect.left) : cx;
        const int mcy = (nFlags & SWP_NOSIZE) ? (rect.bottom - rect.top) : cy;
        impl__MoveWindow_COleControlSite__UEAAXHHHH_Z(pThis, mx, my, mcx, mcy);
    }
    if (nFlags & SWP_SHOWWINDOW) {
        impl__ShowWindow_COleControlSite__UEAAHH_Z(pThis, SW_SHOW);
    }
    nFlags = (nFlags & ~static_cast<unsigned int>(SWP_SHOWWINDOW | SWP_HIDEWINDOW)) |
             SWP_NOSIZE | SWP_NOMOVE;
    HWND hwndAfter = pWndInsertAfter ? pWndInsertAfter->GetSafeHwnd() : nullptr;
    return ::SetWindowPos(pThis->m_hWnd, hwndAfter, x, y, cx, cy, nFlags);
}

// Symbol: ?SetWindowTextW@COleControlSite@@UEAAXPEB_W@Z
// COleControlSite::SetWindowText() — retail mfc140u.dll 0x23bae0:
//   if (!lpsz) AfxThrowInvalidArgException();               // 0x227720
//   if (!SafeSetProperty(DISPID_CAPTION (-518), VT_BSTR (8), lpsz))  // slot 0xf8
//       SafeSetProperty(DISPID_TEXT (-517), VT_BSTR, lpsz);
// (The ANSI twin ?SetWindowTextA@COleControlSite@@UEAAXPEBD@Z at mfc140.dll
// 0x23a0e0 is the same body.)  OpenMFC returns instead of throwing on a null
// string.
extern "C" void MS_ABI impl__SetWindowTextW_COleControlSite__UEAAXPEB_W_Z(
    COleControlSite* pThis, const wchar_t* lpszString) {
    if (!pThis || !lpszString) return;
    if (!SiteSafeSetProperty(pThis, DISPID_CAPTION, VT_BSTR, lpszString)) {
        SiteSafeSetProperty(pThis, DISPID_TEXT, VT_BSTR, lpszString);
    }
}

// Symbol: ?ShowWindow@COleControlSite@@UEAAHH@Z
// COleControlSite::ShowWindow() — retail 0x23a380:
//   BOOL bWasVisible = ::IsWindowVisible(m_hWnd(+0x48));
//   switch (nCmdShow) {
//     case SW_HIDE:                                     DoVerb(OLEIVERB_HIDE, NULL); break;
//     case SW_SHOWNORMAL: case SW_SHOWNOACTIVATE: case SW_SHOW:
//                                                       DoVerb(OLEIVERB_SHOW, NULL); break;
//     default: no verb;
//   }
//   return bWasVisible;
// (the switch is compiled as the chain nCmdShow==0, ==1, ==4, ==5; DoVerb is
// vtable slot 0xc8.)
//
// Retail does NOT call ::ShowWindow here, and nCmdShow never reaches the window
// manager.  The one-argument call at 0x23a398 goes to the import thunk at
// 0x2c5350, which mfc140.dll's import directory names IsWindowVisible;
// ::ShowWindow is a different thunk, 0x2c52b8.  The two are told apart by
// CWnd::ShowWindow (0x2a79e0), which tail-jumps to 0x2c52b8 when the CWnd has
// no control site (CWnd+0xd0) and to *this* method through vtable slot 0x150
// when it has one — so an OLE control is shown and hidden by the verb alone,
// and this entry point only samples the visibility the site had on entry.
// Calling ::ShowWindow here would hide or show the control's window behind the
// control's back, which is exactly what retail avoids.
extern "C" int MS_ABI impl__ShowWindow_COleControlSite__UEAAHH_Z(COleControlSite* pThis, int nCmdShow) {
    if (!pThis) return FALSE;
    const BOOL bWasVisible = ::IsWindowVisible(pThis->m_hWnd);
    long nVerb = 0;
    bool bDoVerb = true;
    switch (nCmdShow) {
    case SW_HIDE:           nVerb = OLEIVERB_HIDE; break;
    case SW_SHOWNORMAL:
    case SW_SHOWNOACTIVATE:
    case SW_SHOW:           nVerb = OLEIVERB_SHOW; break;
    default:                bDoVerb = false; break;
    }
    if (bDoVerb) {
        impl__DoVerb_COleControlSite__UEAAJJPEAUtagMSG___Z(pThis, nVerb, nullptr);
    }
    return bWasVisible;
}

// Symbol: ?AboutToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// Not implemented.  Retail 0x23ebc0 is a nested-interface method.  Its
// METHOD_PROLOGUE compiles down to a single load: `mov -0x138(%rcx),%rdx`
// fetches the owning site's m_pModuleState (the site sits at a fixed negative
// displacement from the subobject; only the module-state pointer is actually
// materialised), which is handed to the AFX_MAINTAIN_STATE2 constructor at
// 0x133df0.  It then calls ?FireEvent@XNotifyDBEvents@COleControlSite@@ at
// 0x23ee00 -- on the subobject itself, not on the site -- with phase 4 pushed
// at [rsp+0x20], and returns that HRESULT only when it is negative (cmovs),
// S_OK otherwise.  OpenMFC never constructs this nested subobject: its client
// site is the separate heap-allocated detail::ControlSiteAdapter, so `this`
// here would not point at anything usable.
extern "C" long MS_ABI impl__AboutToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThis; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return S_OK;
}

// Symbol: ?AdjustRect@XOleIPSite@COleControlSite@@UEAAJPEAUtagRECT@@@Z
// COleControlSite::XOleIPSite::AdjustRect() — retail mfc140u.dll 0x23c710
// (this symbol has no RVA in mfc140.dll's map; the Unicode build has it).
// The whole body is METHOD_PROLOGUE around `return E_NOTIMPL;` — the rectangle
// argument is never touched.  The prologue compiles to one load,
// `mov -0x108(%rcx),%rdx`, which fetches the owning site's m_pModuleState (the
// site itself is never materialised) and hands it to the AFX_MAINTAIN_STATE2
// constructor at 0x133170; the epilogue pops that state again.  The module
// state juggling has no OpenMFC equivalent and nothing observable depends on
// it here, so only the return value is reproduced.  mfc140u folds this body
// with XOleIPSite::ScrollRect, so an RVA-to-name lookup on 0x23c710 answers
// with that name instead; the symbol-to-RVA direction is the reliable one.
extern "C" long MS_ABI impl__AdjustRect_XOleIPSite_COleControlSite__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* prc) {
    (void)pThis; (void)prc;
    return E_NOTIMPL;   // retail: mov $0x80004001,%eax at 0x23c738
}

// Symbol: ?CanInPlaceActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::CanInPlaceActivate() — retail mfc140u.dll
// 0x71e0 is `xor %eax,%eax; ret`, i.e. an unconditional S_OK.  (That RVA is a
// two-instruction body shared by identical-code folding, so it carries no
// site state at all.)  Nothing else to transcribe.
extern "C" long MS_ABI impl__CanInPlaceActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThis) {
    (void)pThis;
    return S_OK;
}

// Symbol: ?CanWindowlessActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::CanWindowlessActivate() — retail mfc140u.dll
// 0x23c6e0: the same METHOD_PROLOGUE as AdjustRect above (the load at
// this-0x108 is the owning site's m_pModuleState, fed to AFX_MAINTAIN_STATE2
// at 0x133170) around `return S_OK;` (xor %eax,%eax at 0x23c708).  The site
// never refuses windowless activation, so only the return value is reproduced.
// mfc140u folds this body with XOleIPSite::RequestUIActivate, which is why the
// RVA reverse-maps to that name.
extern "C" long MS_ABI impl__CanWindowlessActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThis) {
    (void)pThis;
    return S_OK;
}

// Symbol: ?Cancelled@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// Not implemented, for the same reason as XNotifyDBEvents::AboutToDo above.
// Retail 0x23eac0 is the identical body with phase 2 instead of phase 4.
extern "C" long MS_ABI impl__Cancelled_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThis; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return S_OK;
}

// Symbol: ?ContextSensitiveHelp@XOleIPSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleIPSite::ContextSensitiveHelp() — retail mfc140u.dll
// 0xf4d0 is `mov $0x80004001,%eax; ret`: E_NOTIMPL, with fEnterMode ignored.
// (An identical-code-folded two-instruction body, so there is no site state
// involved.)
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThis, int fEnterMode) {
    (void)pThis; (void)fEnterMode;
    return E_NOTIMPL;
}

//=============================================================================
// COleControlSite nested COM interface implementations — XOleClientSite,
// XOleIPSite, XOleControlSite, XAmbientProps, XPropertyNotifySink, XEventSink,
// XBoundObjectSite, XNotifyDBEvents, XRowsetNotify.
//
// Every entry point below is an MFC nested-interface method.  Each opens with
// METHOD_PROLOGUE, which recovers the owning COleControlSite from the address
// of an 8-byte (vftable-pointer-only) sub-object embedded in it.  The
// sub-object offsets were read straight out of those prologues in retail
// mfc140.dll:
//
//   site+0x138  XOleClientSite       (ShowObject 0x23a900: lea -0x138(%rcx))
//   site+0x140  XOleIPSite           (OnUIActivate 0x23a9a0: lea -0x140(%rcx))
//   site+0x148  XOleControlSite      (OnControlInfoChanged 0x23b250:
//                                     add $-0x148,%rcx)
//   site+0x150  XAmbientProps        (Invoke 0x23b3e0: lea -0x150(%rcx))
//   site+0x158  XPropertyNotifySink  (OnChanged 0x23b4e0: lea -0x158(%rcx))
//   site+0x160  XEventSink           (Invoke 0x23b7f0: lea -0x160(%rcx))
//   site+0x168  XBoundObjectSite     (GetCursor 0x23e930: lea -0x168(%rcx))
//   site+0x170  XNotifyDBEvents      (OKToDo 0x23ea10 reads -0x138(%rcx) for
//                                     m_pModuleState, which CCmdTarget keeps
//                                     at site+0x38)
//   site+0x178  XRowsetNotify        (OnFieldChange 0x23efc0: lea -0x178(%rcx))
//
// Roughly a third of these symbols have no RVA of their own in
// mfc140_rva_symbols.json, because the linker folded their bodies into another
// function's.  For those the body was found by reading the sub-object's
// vftable out of .rdata instead of by name.  The vftables are at
//   0x328e40 XRowsetNotify   0x328e78 XNotifyDBEvents  0x328ed0 XEventSink
//   0x328f10 XBoundObjectSite 0x328f38 XAmbientProps   0x328f78 XPropertyNotifySink
//   0x328fa8 XOleIPSite       0x3290a0 XOleControlSite 0x3290f8 XOleClientSite
// and the three folded one-liners they point at are
//   0xf540  `mov $0x80004001,%eax; ret`  -> return E_NOTIMPL;
//   0x7260  `xor %eax,%eax; ret`         -> return S_OK;
//   0x3ae0  `mov $0x1,%eax; ret`         -> returns 1: S_FALSE where the caller
//                                           wants an HRESULT (XOleIPSite::Scroll,
//                                           slot 9) and a refcount of 1 in the
//                                           AddRef of the sinks, whose Release
//                                           is the 0x7260 that returns 0
// The RVA map names those three after whichever symbol won the fold
// (CDHtmlDialog::UpdateUI, COleControl::XOleObject::Update,
// CPaneTrackingWnd::OnEraseBkgnd); the names carry no meaning here.
// A fourth shared body, 0x23ad10, is METHOD_PROLOGUE around `return E_NOTIMPL;`
// and serves XOleIPSite's GetFocus, ScrollRect, AdjustRect and
// OnDefWindowMessage.
//
// OpenMFC's COleControlSite has none of these sub-objects: it declares no
// vftable members at +0x138..+0x178, its client site is the separate
// heap-allocated detail::olecore::ControlSiteAdapter, and no other file in
// phase4/src references any of these thunks or hands such an interface pointer
// out (checked by grep).  So `this` here can never be
// turned back into a COleControlSite, and the METHOD_PROLOGUE subtraction
// would land in unrelated memory.  Consequently:
//   * a method whose entire retail body is a constant return is transcribed
//     exactly;
//   * every other one is left as generated, with the decode written down.  Its
//     `this` parameter is spelled out only to document the real calling
//     convention — the stub body never dereferences it.
// Where a stub's return value differs from the generated `return 0`, the
// reason is stated at that function; those are the ones with an out-parameter,
// where reporting S_OK without writing the parameter would hand the caller an
// uninitialised pointer.
//=============================================================================

// Symbol: ?DeactivateAndUndo@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::DeactivateAndUndo() — retail 0x23ab60:
//     mov  -0xb8(%rcx),%rcx       ; site+0x88 = m_lpInPlaceObject
//     test %rcx,%rcx ; je 0x23ab84 ; -> AfxThrowInvalidArgException (0x225b80)
//     mov  (%rcx),%rax
//     mov  0x30(%rax),%rax        ; vftable byte 0x30 = slot 6 = UIDeactivate
//     call *__guard_dispatch
//     xor  %eax,%eax ; ret
// i.e. `m_lpInPlaceObject->UIDeactivate(); return S_OK;`, with a null
// m_lpInPlaceObject throwing CInvalidArgException rather than returning.  Left
// as generated: the site cannot be recovered from the sub-object pointer.
extern "C" long MS_ABI impl__DeactivateAndUndo_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return 0;   // retail: S_OK after IOleInPlaceObject::UIDeactivate()
}

// Symbol: ?DidEvent@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// COleControlSite::XNotifyDBEvents::DidEvent() — retail 0x23ed60.  After the
// module-state prologue it calls XNotifyDBEvents::FireEvent (0x23ee00) with
// phase 7 pushed at [rsp+0x20] and returns that HRESULT when it is negative.
// Otherwise, if the site's CDataSourceControl at site+0xd8 is non-null and bit
// 7 (0x80) of dwEventWhat is set (`test %bpl,%bpl; jns`), it sets that
// object's +0x88 flag to 1, calls CDataSourceControl::GetBoundClientRow
// (0x23cb00), restores +0x88, calls its vftable byte 0x28 (slot 5), and
// returns S_OK.
// Left as generated: OpenMFC models neither CDataSourceControl nor the
// sub-object layout.
extern "C" long MS_ABI impl__DidEvent_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThisItf; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return 0;
}

// Symbol: ?DiscardUndoState@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::DiscardUndoState() — this symbol has no RVA of
// its own; slot 12 of the XOleIPSite vftable (0x328fa8) points at the folded
// body 0x7260, `xor %eax,%eax; ret`.  An unconditional S_OK that touches no
// state, so it is transcribed exactly.
extern "C" long MS_ABI impl__DiscardUndoState_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_OK;
}

// Symbol: ?FailedToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// COleControlSite::XNotifyDBEvents::FailedToDo() — retail 0x23ec30 is the
// module-state prologue, then XNotifyDBEvents::FireEvent (0x23ee00) with phase
// 5 at [rsp+0x20], then `xor %edx,%edx; test %eax,%eax; cmovs %eax,%edx`, i.e.
// the HRESULT is propagated only when it is negative and S_OK otherwise.  Same
// FireEvent dependency as the rest of this interface; left as generated.
extern "C" long MS_ABI impl__FailedToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThisItf; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return 0;
}

// Symbol: ?FireEvent@XNotifyDBEvents@COleControlSite@@QEAAJKKQEAUtagDBNOTIFYREASON@@W4DSCSTATE@@@Z
// COleControlSite::XNotifyDBEvents::FireEvent() — retail 0x23ee00.  This is
// the shared worker the seven notification entry points of this sub-object
// call (slots 3..9 of the vftable at 0x328e78: OKToDo, Cancelled, SyncBefore,
// AboutToDo, FailedToDo, SyncAfter, DidEvent), and its only non-virtual
// (public) method.  It
// returns S_OK immediately (0x23eef8) unless (dwEventWhat & 3) is non-zero and
// dwEventWhen is non-zero; then it indexes the caller's DBNOTIFYREASON array
// with a 0x38-byte stride, maps the reason code through a jump ladder onto one
// of a handful of MFC event codes, builds an AFX_EVENT on the stack (kind 3 at
// +0x00, dispid -1 at +0x04, the DSCSTATE argument from [rsp+0x80] at +0x28
// and the event code at +0x2c) and calls COleControlSite::OnEvent (0x239b50)
// on the recovered site (`lea -0x170(%r11),%rcx`), returning the AFX_EVENT
// result field at +0x24.  Left as generated: it needs both the sub-object
// layout and OpenMFC's absent data-source-control plumbing.
// The generated parameter list was wrong (13 parameters from a bad demangle);
// the real one is (this, DWORD dwEventWhat, DWORD dwEventWhen,
// DBNOTIFYREASON* const, DSCSTATE).  Nothing outside this file declares this
// thunk, so correcting it here is safe.
extern "C" long MS_ABI impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason, int dscState) {
    (void)pThisItf; (void)dwEventWhat; (void)dwEventWhen; (void)pReason; (void)dscState;
    return 0;
}

// Symbol: ?GetCapture@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::GetCapture() — retail 0x23ad50, after the
// module-state prologue, is exactly:
//     mov 0x40(%rbx),%rcx          ; rbx = site; m_pCtrlCont
//     xor %eax,%eax
//     cmp %rbx,0xd0(%rcx)          ; m_pCtrlCont->+0xd0 (the capture site)
//     setne %al                    ; -> S_OK when it is this site, else S_FALSE
// That the container member at +0xd0 really is the capture site is confirmed
// by XOleIPSite::SetCapture (0x23b130), which stores the site into it beside
// ::SetCapture and NULLs it beside ::ReleaseCapture.
// COleControlContainer has no +0xd0 capture-site member in OpenMFC and the
// site is unrecoverable here, so nothing can be looked up.  This returns
// S_FALSE rather than the generated 0: S_OK is retail's "this site holds the
// mouse capture" answer, which a windowless control acts on, and no OpenMFC
// site ever holds it -- S_FALSE is what retail answers for every site that has
// not captured, which here is all of them.
extern "C" long MS_ABI impl__GetCapture_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_FALSE;
}

// Symbol: ?GetContainer@XOleClientSite@COleControlSite@@UEAAJPEAPEAUIOleContainer@@@Z
// COleControlSite::XOleClientSite::GetContainer() — retail 0x23a8e0 is a
// four-instruction tail call:
//     mov -0xf8(%rcx),%rcx         ; site+0x40 = m_pCtrlCont
//     mov %rdx,%r8                 ; ppContainer
//     lea 0x1802d7ac8(%rip),%rdx   ; &IID_IOleContainer
//     jmp 0x18026bda0              ; CCmdTarget::InternalQueryInterface
// i.e. `return m_pCtrlCont->InternalQueryInterface(&IID_IOleContainer,
// ppContainer);`, with no null check on either the container or ppContainer.
// Left as generated except for the return value: the container is
// unreachable from here, and answering S_OK while leaving *ppContainer
// untouched would hand the caller an uninitialised interface pointer, so this
// reports E_FAIL.  That is a deviation from retail, which would return
// whatever InternalQueryInterface answered.
extern "C" long MS_ABI impl__GetContainer_XOleClientSite_COleControlSite__UEAAJPEAPEAUIOleContainer___Z(
    void* pThisItf, void** ppContainer) {
    (void)pThisItf; (void)ppContainer;
    return E_FAIL;
}

// Symbol: ?GetCursor@XBoundObjectSite@COleControlSite@@UEAAJJPEAPEAUICursor@@PEAPEAX@Z
// COleControlSite::XBoundObjectSite::GetCursor() — retail 0x23e930 is the
// module-state prologue followed by a straight forward of all three arguments
// to COleControlSite::GetCursor (0x23e8f0) on the recovered site, returning
// its HRESULT.  OpenMFC exports that protected helper as a stub of its own and
// cannot recover the site here, so this is left as generated apart from the
// return value: it has two out-parameters, so it reports E_FAIL rather than a
// success it did not produce.
extern "C" long MS_ABI impl__GetCursor_XBoundObjectSite_COleControlSite__UEAAJJPEAPEAUICursor__PEAPEAX_Z(
    void* pThisItf, long lIndex, void** ppCursor, void** ppUnk) {
    (void)pThisItf; (void)lIndex; (void)ppCursor; (void)ppUnk;
    return E_FAIL;
}

// Symbol: ?GetDC@XOleIPSite@COleControlSite@@UEAAJPEBUtagRECT@@KPEAPEAUHDC__@@@Z
// COleControlSite::XOleIPSite::GetDC() — retail 0x23ada0.  A null phDC jumps
// to 0x23aee0, which is `call AfxThrowInvalidArgException` (0x225b80), so that
// case throws rather than returning.  Otherwise it clears *phDC, and if the
// site's cached CDC at site+0x130 is null it builds one with
// CDC::FromHandle(::GetDC(m_pCtrlCont->m_pWnd->m_hWnd)) (USER32 import at
// 0x2c4eb8, CDC::FromHandle at 0x2a0390) and stores it there; if that still
// yields nothing it returns E_FAIL (0x23ae46).  With a DC in hand it takes the
// caller's rectangle intersected with the site's position rect at site+0x5c
// (::IntersectRect, 0x2c4d48) — or that position rect verbatim when lprcRect
// is null — turns it into a region (::CreateRectRgnIndirect, GDI32 import at
// 0x2c41a0) attached to a stack CRgn (CGdiObject::Attach, 0x2a1e10), calls
// CDC::SelectClipRgn(&rgn, RGN_AND) (0x2a1580) on the cached CDC -- the mode
// argument is `mov $0x1,%r8d` at 0x23ae8e, and RGN_AND is 1, RGN_COPY is 5 --
// and hands back that CDC's m_hDC (+0x8).  grfFlags is never read.
// Left as generated except for the return value: it has an out-parameter, and
// E_FAIL is retail's own "no DC" answer, so that is what is reported here.
// Retail clears *phDC at 0x23adf8, before any of the work that can fail, so it
// reaches 0x23ae46 with the out-parameter already NULL; this does the same.
// A null phDC throws there and merely returns here -- OpenMFC has no site to
// validate against, so there is nothing to report an invalid argument about.
extern "C" long MS_ABI impl__GetDC_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__KPEAPEAUHDC_____Z(
    void* pThisItf, const RECT* lprcRect, unsigned long grfFlags, HDC* phDC) {
    (void)pThisItf; (void)lprcRect; (void)grfFlags;
    if (phDC != nullptr) *phDC = nullptr;
    return E_FAIL;   // retail's own failure exit at 0x23ae46
}

// Symbol: ?GetExtendedControl@XOleControlSite@COleControlSite@@UEAAJPEAPEAUIDispatch@@@Z
// COleControlSite::XOleControlSite::GetExtendedControl() — no RVA of its own;
// slot 5 of the XOleControlSite vftable (0x3290a0) is the folded body 0xf540,
// `mov $0x80004001,%eax; ret`.  An unconditional E_NOTIMPL that reads nothing
// and never writes ppDisp, so it is transcribed exactly.
extern "C" long MS_ABI impl__GetExtendedControl_XOleControlSite_COleControlSite__UEAAJPEAPEAUIDispatch___Z(
    void* pThisItf, void** ppDisp) {
    (void)pThisItf; (void)ppDisp;
    return E_NOTIMPL;
}

// Symbol: ?GetFocus@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::GetFocus() — no RVA of its own; slot 21 of the
// XOleIPSite vftable (0x328fa8) points at 0x23ad10, the body shared with
// ScrollRect, AdjustRect and OnDefWindowMessage: METHOD_PROLOGUE (the load at
// this-0x108 is the site's m_pModuleState, fed to AFX_MAINTAIN_STATE2 at
// 0x133df0) around `mov $0x80004001,%eax`, i.e. E_NOTIMPL.  No site state is
// read, so only the return value is reproduced.
extern "C" long MS_ABI impl__GetFocus_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;
}

// Symbol: ?GetIDsOfNames@XAmbientProps@COleControlSite@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
// COleControlSite::XAmbientProps::GetIDsOfNames() — no RVA of its own; slot 5
// of the XAmbientProps vftable (0x328f38) is the folded 0xf540, an
// unconditional E_NOTIMPL that touches no argument.  Transcribed exactly.
extern "C" long MS_ABI impl__GetIDsOfNames_XAmbientProps_COleControlSite__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThisItf, const GUID* riid, wchar_t** rgszNames, unsigned int cNames,
    unsigned long lcid, long* rgDispId) {
    (void)pThisItf; (void)riid; (void)rgszNames; (void)cNames; (void)lcid; (void)rgDispId;
    return E_NOTIMPL;
}

// Symbol: ?GetIDsOfNames@XEventSink@COleControlSite@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
// COleControlSite::XEventSink::GetIDsOfNames() — no RVA of its own; slot 5 of
// the XEventSink vftable (0x328ed0) is the same folded 0xf540 E_NOTIMPL.
extern "C" long MS_ABI impl__GetIDsOfNames_XEventSink_COleControlSite__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThisItf, const GUID* riid, wchar_t** rgszNames, unsigned int cNames,
    unsigned long lcid, long* rgDispId) {
    (void)pThisItf; (void)riid; (void)rgszNames; (void)cNames; (void)lcid; (void)rgDispId;
    return E_NOTIMPL;
}

// Symbol: ?GetMoniker@XOleClientSite@COleControlSite@@UEAAJKKPEAPEAUIMoniker@@@Z
// COleControlSite::XOleClientSite::GetMoniker() — no RVA of its own; slot 4 of
// the XOleClientSite vftable (0x3290f8) is the folded 0xf540, an unconditional
// E_NOTIMPL that never writes ppmk.  Transcribed exactly.
extern "C" long MS_ABI impl__GetMoniker_XOleClientSite_COleControlSite__UEAAJKKPEAPEAUIMoniker___Z(
    void* pThisItf, unsigned long dwAssign, unsigned long dwWhichMoniker, void** ppmk) {
    (void)pThisItf; (void)dwAssign; (void)dwWhichMoniker; (void)ppmk;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfo@XAmbientProps@COleControlSite@@UEAAJIKPEAPEAUITypeInfo@@@Z
// COleControlSite::XAmbientProps::GetTypeInfo() — no RVA of its own; slot 4 of
// the XAmbientProps vftable (0x328f38) is the folded 0xf540 E_NOTIMPL.
extern "C" long MS_ABI impl__GetTypeInfo_XAmbientProps_COleControlSite__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThisItf, unsigned int iTInfo, unsigned long lcid, void** ppTInfo) {
    (void)pThisItf; (void)iTInfo; (void)lcid; (void)ppTInfo;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfo@XEventSink@COleControlSite@@UEAAJIKPEAPEAUITypeInfo@@@Z
// COleControlSite::XEventSink::GetTypeInfo() — no RVA of its own; slot 4 of
// the XEventSink vftable (0x328ed0) is the folded 0xf540 E_NOTIMPL.
extern "C" long MS_ABI impl__GetTypeInfo_XEventSink_COleControlSite__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThisItf, unsigned int iTInfo, unsigned long lcid, void** ppTInfo) {
    (void)pThisItf; (void)iTInfo; (void)lcid; (void)ppTInfo;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfoCount@XAmbientProps@COleControlSite@@UEAAJPEAI@Z
// COleControlSite::XAmbientProps::GetTypeInfoCount() — no RVA of its own; slot
// 3 of the XAmbientProps vftable (0x328f38) is the folded 0xf540 E_NOTIMPL.
// Note retail does not write *pctinfo, so neither does this.
extern "C" long MS_ABI impl__GetTypeInfoCount_XAmbientProps_COleControlSite__UEAAJPEAI_Z(
    void* pThisItf, unsigned int* pctinfo) {
    (void)pThisItf; (void)pctinfo;
    return E_NOTIMPL;
}

// Symbol: ?GetTypeInfoCount@XEventSink@COleControlSite@@UEAAJPEAI@Z
// COleControlSite::XEventSink::GetTypeInfoCount() — no RVA of its own; slot 3
// of the XEventSink vftable (0x328ed0) is the folded 0xf540 E_NOTIMPL, and
// *pctinfo is left untouched there too.
extern "C" long MS_ABI impl__GetTypeInfoCount_XEventSink_COleControlSite__UEAAJPEAI_Z(
    void* pThisItf, unsigned int* pctinfo) {
    (void)pThisItf; (void)pctinfo;
    return E_NOTIMPL;
}

// Symbol: ?GetWindow@XOleIPSite@COleControlSite@@UEAAJPEAPEAUHWND__@@@Z
// COleControlSite::XOleIPSite::GetWindow() — retail 0x23a970 is eight
// instructions and no prologue:
//     mov -0x100(%rcx),%rax   ; site+0x40 = m_pCtrlCont (not null-checked)
//     mov 0x40(%rax),%rax     ; COleControlContainer::m_pWnd (+0x40, pinned by
//                             ; the ctor at 0x233920)
//     test %rax,%rax ; je .L  ; only m_pWnd is checked
//     mov 0x40(%rax),%rax     ; CWnd::m_hWnd (+0x40)
//   .L: mov %rax,(%rdx)       ; *phWnd = it, even when null; phWnd unchecked
//     neg/sbb/not/and $0x80004005 -> S_OK when non-null, E_FAIL when null
// Left as generated except for the return value: the container is unreachable
// from the sub-object pointer, and E_FAIL is retail's own answer for "no
// window", so that is reported rather than an S_OK with *phWnd unwritten.
// Retail reaches that E_FAIL having already stored NULL through phWnd (the
// unconditional `mov %rax,(%rdx)` with rax == 0), so this does the same; the
// one deviation is the null check, which retail does not make.
extern "C" long MS_ABI impl__GetWindow_XOleIPSite_COleControlSite__UEAAJPEAPEAUHWND_____Z(
    void* pThisItf, HWND* phWnd) {
    (void)pThisItf;
    if (phWnd != nullptr) *phWnd = nullptr;
    return E_FAIL;
}

// Symbol: ?GetWindowContext@XOleIPSite@COleControlSite@@UEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagRECT@@2PEAUtagOIFI@@@Z
// COleControlSite::XOleIPSite::GetWindowContext() — retail 0x23a9e0, fully
// decoded:
//     if (ppDoc) *ppDoc = NULL;
//     hr = m_pCtrlCont->InternalQueryInterface(&IID_IOleInPlaceFrame, ppFrame);
//     if (FAILED(hr)) return E_FAIL;                       ; 0x23aa2c
//     ::CopyRect(lprcPosRect, &site->rect@+0x5c);          ; import 0x2c5240
//     ::GetClientRect(m_pCtrlCont->m_pWnd->m_hWnd, lprcClipRect); ; 0x2c5358
//     lpFrameInfo->fMDIApp        = FALSE;   ; +0x04
//     lpFrameInfo->hwndFrame      = m_pCtrlCont->m_pWnd->m_hWnd;  ; +0x08
//     lpFrameInfo->haccel         = NULL;    ; +0x10
//     lpFrameInfo->cAccelEntries  = 0;       ; +0x18
//     return S_OK;
// (lpFrameInfo->cb at +0x00 is left as the caller set it.)  Left as generated
// except for the return value: it has two interface out-parameters, and E_FAIL
// is retail's own failure exit, so that is what is reported.  Retail clears
// *ppDoc before anything else and so reaches that exit with ppDoc already
// NULL; this reproduces that store.  *ppFrame is left alone, exactly as on the
// retail failure path, where only InternalQueryInterface would have touched it.
extern "C" long MS_ABI impl__GetWindowContext_XOleIPSite_COleControlSite__UEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagRECT__2PEAUtagOIFI___Z(
    void* pThisItf, void** ppFrame, void** ppDoc, RECT* lprcPosRect,
    RECT* lprcClipRect, void* lpFrameInfo) {
    (void)pThisItf; (void)ppFrame; (void)lprcPosRect;
    (void)lprcClipRect; (void)lpFrameInfo;
    if (ppDoc != nullptr) *ppDoc = nullptr;
    return E_FAIL;
}

// Symbol: ?InvalidateRect@XOleIPSite@COleControlSite@@UEAAJPEBUtagRECT@@H@Z
// COleControlSite::XOleIPSite::InvalidateRect() — retail 0x23aef0.  After the
// module-state prologue it forms a working rectangle: with a null lprcRect it
// copies the site's position rect at site+0x5c wholesale (a 16-byte SSE move);
// otherwise it calls ::IntersectRect(&rc, lprcRect, &site->rect) (USER32
// import at 0x2c4d48) and then ::EqualRect(&rc, lprcRect) (0x2c52e8) whose
// result is immediately overwritten and never used.  If rc is not empty
// (::IsRectEmpty, 0x2c52c8) it calls
// ::InvalidateRect(m_pCtrlCont->m_pWnd->m_hWnd, &rc, fErase) through the
// import at 0x2c5380.  It always returns S_OK.
// Left as generated (S_OK), which matches retail's return; the invalidation
// itself needs the site rect and the container window.
extern "C" long MS_ABI impl__InvalidateRect_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__H_Z(
    void* pThisItf, const RECT* lprcRect, int fErase) {
    (void)pThisItf; (void)lprcRect; (void)fErase;
    return 0;   // retail: unconditional S_OK
}

// Symbol: ?InvalidateRgn@XOleIPSite@COleControlSite@@UEAAJPEAUHRGN__@@H@Z
// COleControlSite::XOleIPSite::InvalidateRgn() — retail 0x23afc0.  With a
// non-null hRgn it wraps it (CGdiObject::FromHandle, 0x2a1de0), takes its
// bounding box (::GetRgnBox, GDI32 import at 0x2c41a8), computes
// ::IntersectRect(&isect, &box, &site->rect@+0x5c) (0x2c4d48) and asks
// ::EqualRect(&isect, &box) (0x2c52e8) — i.e. "does the region lie entirely
// inside the site rect?".  If it does, it calls
// ::InvalidateRgn(m_pCtrlCont->m_pWnd->m_hWnd, hRgn, fErase) (import 0x2c4f80)
// and returns S_OK.  Otherwise — and immediately, for a null hRgn — it falls
// back to `return this->InvalidateRect(NULL, fErase);` through the
// sub-object's own vftable byte 0xc8 (slot 25), invalidating the whole site
// rect.  Left as generated: both paths need the site.
extern "C" long MS_ABI impl__InvalidateRgn_XOleIPSite_COleControlSite__UEAAJPEAUHRGN____H_Z(
    void* pThisItf, HRGN hRgn, int fErase) {
    (void)pThisItf; (void)hRgn; (void)fErase;
    return 0;
}

// Symbol: ?Invoke@XAmbientProps@COleControlSite@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
// COleControlSite::XAmbientProps::Invoke() — retail 0x23b3e0.  The whole body,
// after the module-state prologue, is:
//     rcx = site->m_pCtrlCont(+0x40);         if null -> AfxThrowInvalidArgException
//     rcx = m_pCtrlCont->m_pWnd(+0x40);       if null -> AfxThrowInvalidArgException
//     BOOL b = pWnd->vftable slot 70 (byte 0x230)(site, dispIdMember, pVarResult);
//             ; = CWnd::OnAmbientProperty(COleControlSite*, DISPID, VARIANT*)
//     neg/sbb/not/and $0x80020003  ->  b ? S_OK : DISP_E_MEMBERNOTFOUND
// riid, lcid, wFlags, pDispParams, pExcepInfo and puArgErr are all ignored.
// OpenMFC does export the CWnd handler (impl__OnAmbientProperty_CWnd__...),
// but the site — the first argument it needs — cannot be recovered here, so
// this is left as generated apart from the return value: retail's
// DISP_E_MEMBERNOTFOUND is the answer for "no ambient property available",
// which is exactly the state this stub is in.
extern "C" long MS_ABI impl__Invoke_XAmbientProps_COleControlSite__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThisItf, long dispIdMember, const GUID* riid, unsigned long lcid,
    unsigned short wFlags, void* pDispParams, void* pVarResult,
    void* pExcepInfo, unsigned int* puArgErr) {
    (void)pThisItf; (void)dispIdMember; (void)riid; (void)lcid; (void)wFlags;
    (void)pDispParams; (void)pVarResult; (void)pExcepInfo; (void)puArgErr;
    return static_cast<long>(0x80020003L);   // DISP_E_MEMBERNOTFOUND
}

// Symbol: ?Invoke@XEventSink@COleControlSite@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
// COleControlSite::XEventSink::Invoke() — retail 0x23b7f0.  After the
// module-state prologue it fills an AFX_EVENT on the stack and dispatches it:
//     ev.m_eventKind(+0x00) = 0            ; event
//     ev.m_dispid  (+0x04) = dispIdMember
//     ev.+0x08 = pDispParams   ev.+0x10 = pExcepInfo   ev.+0x18 = puArgErr
//     ev.+0x24 = 0 (the result the method returns)     ev.+0x2c = 0
//     COleControlSite::OnEvent(&ev);       ; 0x239b50, on the recovered site
//     if (pVarResult) ::VariantClear(pVarResult);  ; OLEAUT32 import at
//                                                  ; 0x2c4a18, ordinal 9
//     return ev.+0x24;
// riid, lcid and wFlags are ignored.  Left as generated: OnEvent needs the
// site.  The generated S_OK is the value the AFX_EVENT carries when OnEvent
// leaves the result field alone, so the return value is retail-faithful even
// though nothing is dispatched.
extern "C" long MS_ABI impl__Invoke_XEventSink_COleControlSite__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThisItf, long dispIdMember, const GUID* riid, unsigned long lcid,
    unsigned short wFlags, void* pDispParams, void* pVarResult,
    void* pExcepInfo, unsigned int* puArgErr) {
    (void)pThisItf; (void)dispIdMember; (void)riid; (void)lcid; (void)wFlags;
    (void)pDispParams; (void)pVarResult; (void)pExcepInfo; (void)puArgErr;
    return 0;
}

// Symbol: ?LockInPlaceActive@XOleControlSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleControlSite::LockInPlaceActive() — no RVA of its own;
// slot 4 of the XOleControlSite vftable (0x3290a0) is the folded 0xf540, an
// unconditional E_NOTIMPL with fLock ignored.  Transcribed exactly.
extern "C" long MS_ABI impl__LockInPlaceActive_XOleControlSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fLock) {
    (void)pThisItf; (void)fLock;
    return E_NOTIMPL;
}

// Symbol: ?OKToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// COleControlSite::XNotifyDBEvents::OKToDo() — retail 0x23ea10.  It calls
// XNotifyDBEvents::FireEvent (0x23ee00) with phase 1 at [rsp+0x20] and returns
// that HRESULT when it is negative.  Otherwise it inspects *pReason: reason
// 0x1e, or 2..3, short-circuits to S_OK; for the rest it requires either
// (dwEventWhat & 1) or reason 1 or 0x14, and then — only if the object at
// site+0xd8 (CDataSourceControl* per the map in this file's header) is
// non-null and (dwEventWhat & 1) is set — returns that object's vftable byte
// 0x30 (slot 6).  Everything else returns S_OK.
// Left as generated: neither the sub-object layout nor the data-bound-property
// plumbing exists in OpenMFC.
extern "C" long MS_ABI impl__OKToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThisItf; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return 0;
}

// Symbol: ?OnChanged@XPropertyNotifySink@COleControlSite@@UEAAJJ@Z
// COleControlSite::XPropertyNotifySink::OnChanged() — retail 0x23b4e0.  It
// returns S_OK immediately when the site's +0xd0 flag is set.  With no
// CDataSourceControl at site+0xd8 it raises an AFX_EVENT of kind 2 (property
// changed) carrying the dispid through COleControlSite::OnEvent (0x239b50) --
// but it returns that event's result field only when the field came back
// NON-zero; on a zero result it falls through (`test %eax,%eax; je 0x23b580`
// at 0x23b562) into the second half, exactly where a non-null
// CDataSourceControl jumps to.  That second half returns S_OK unless dispid
// equals the bound dispid cached at site+0xf0; for that dispid it re-reads the
// bound value: it calls
// ::VariantClear on the cached VARIANT at site+0x108 (the OLEAUT32 import at
// 0x2c4a18, brought in by ordinal 9), queries m_lpObject(+0x80) for
// IID_IDispatch (0x2d7a98) and calls that interface's Invoke — vftable byte
// 0x30 — as Invoke(dispid, IID_NULL, 0, DISPATCH_PROPERTYGET, &emptyParams,
// &site->variant@+0x108, &excepInfo, &argErr), with the two stack buffers
// zeroed by memset (0x2c5468) beforehand.
// Left as generated: all of this hangs off the retail site layout.
extern "C" long MS_ABI impl__OnChanged_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(
    void* pThisItf, long dispid) {
    (void)pThisItf; (void)dispid;
    return 0;
}

// Symbol: ?OnControlInfoChanged@XOleControlSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleControlSite::OnControlInfoChanged() — retail 0x23b250
// is five instructions:
//     add $-0x148,%rcx           ; recover the site
//     call 0x1802397b0           ; COleControlSite::GetControlInfo()
//     xor %eax,%eax ; ret        ; return S_OK
// The return value is transcribed; the GetControlInfo() call is not, because
// the site cannot be recovered from the sub-object pointer.  OpenMFC does
// implement GetControlInfo (it refreshes the cached CONTROLINFO in the side
// table at the top of this file), so this is the one thing missing here.
extern "C" long MS_ABI impl__OnControlInfoChanged_XOleControlSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_OK;   // retail: S_OK, after pThis->GetControlInfo()
}

// Symbol: ?OnDefWindowMessage@XOleIPSite@COleControlSite@@UEAAJI_K_JPEA_J@Z
// COleControlSite::XOleIPSite::OnDefWindowMessage() — no RVA of its own; slot
// 29 of the XOleIPSite vftable (0x328fa8) points at 0x23ad10, the shared
// METHOD_PROLOGUE-plus-`return E_NOTIMPL` body described at GetFocus above.
// plResult is never written.  Transcribed exactly.
extern "C" long MS_ABI impl__OnDefWindowMessage_XOleIPSite_COleControlSite__UEAAJI_K_JPEA_J_Z(
    void* pThisItf, unsigned int msg, unsigned __int64 wParam, __int64 lParam,
    __int64* plResult) {
    (void)pThisItf; (void)msg; (void)wParam; (void)lParam; (void)plResult;
    return E_NOTIMPL;
}

// Symbol: ?OnFieldChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@_K1QEA_KKKH@Z
// COleControlSite::XRowsetNotify::OnFieldChange() — retail 0x23efc0.  It maps
// the eNotifyPhase argument (the 7th, read from the stack at [rbp+0x58])
// through a ladder onto an MFC event code — 0->1, 1->4, 2->6, 3->5, 4->7 — and
// for phase 4 it also refreshes the CDataSourceControl at site+0xd8 (setting
// its +0x88 flag to 1, calling CDataSourceControl::GetBoundClientRow at
// 0x23cb00, clearing the flag, then its vtable slot 5), bailing out with S_OK
// when that object is absent and skipping the refresh when its +0x88 flag was
// already set.  It then raises an AFX_EVENT of kind 3 with dispid -1 and that
// event code through COleControlSite::OnEvent (0x239b50) and returns the
// event's result field.  Left as generated: OpenMFC has no CDataSourceControl.
extern "C" long MS_ABI impl__OnFieldChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___K1QEA_KKKH_Z(
    void* pThisItf, void* pRowset, unsigned __int64 hRow, unsigned __int64 cColumns,
    unsigned __int64* rgColumns, unsigned long eReason, unsigned long eNotifyPhase,
    int fIsRecoverable) {
    (void)pThisItf; (void)pRowset; (void)hRow; (void)cColumns; (void)rgColumns;
    (void)eReason; (void)eNotifyPhase; (void)fIsRecoverable;
    return 0;
}

// Symbol: ?OnFocus@XOleControlSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleControlSite::OnFocus() — no RVA of its own; slot 8 of
// the XOleControlSite vftable (0x3290a0) is the folded body 0x7260,
// `xor %eax,%eax; ret`.  An unconditional S_OK with fGotFocus ignored and no
// state touched, so it is transcribed exactly.
extern "C" long MS_ABI impl__OnFocus_XOleControlSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fGotFocus) {
    (void)pThisItf; (void)fGotFocus;
    return S_OK;
}

// Symbol: ?OnInPlaceActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::OnInPlaceActivate() — no RVA of its own; slot 6
// of the XOleIPSite vftable (0x328fa8) is the folded body 0x7260, an
// unconditional S_OK that touches no state.  (Slot 5, CanInPlaceActivate, is
// the same body — see that function above.)  Transcribed exactly.
extern "C" long MS_ABI impl__OnInPlaceActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_OK;
}

// Symbol: ?OnInPlaceActivateEx@XOleIPSite@COleControlSite@@UEAAJPEAHK@Z
// COleControlSite::XOleIPSite::OnInPlaceActivateEx() — retail 0x23ac40, fully
// decoded.  After the module-state prologue:
//     if (dwFlags & 1) {            ; ACTIVATE_WINDOWLESS
//         site->m_lpObject(+0x80)->QueryInterface(
//             IID_IOleInPlaceObjectWindowless (0x2d7d18),
//             &site->+0x98);        ; the windowless interface slot
//         site->+0xb4 = 1;          ; the windowless-active flag
//         ++site->m_pCtrlCont(+0x40)->+0xe0;   ; container's windowless count
//     }
//     if (pfNoRedraw) *pfNoRedraw = FALSE;
//     return S_OK;
// This is one of the three uses of +0xb4/+0x98 cited in the file header above.
// Left as generated: OpenMFC's COleControlSite has neither member, and the
// site is unrecoverable here.  The generated S_OK matches retail's
// unconditional return; *pfNoRedraw is not written.
extern "C" long MS_ABI impl__OnInPlaceActivateEx_XOleIPSite_COleControlSite__UEAAJPEAHK_Z(
    void* pThisItf, int* pfNoRedraw, unsigned long dwFlags) {
    (void)pThisItf; (void)pfNoRedraw; (void)dwFlags;
    return 0;
}

// Symbol: ?OnInPlaceDeactivate@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::OnInPlaceDeactivate() — retail 0x23aae0, fully
// decoded.  After the module-state prologue:
//     COleControlSite::DetachWindow();          ; 0x239ad0, on the site
//     if (site->+0xb4) {                        ; windowless-active
//         if (site->+0x98) { Release(); site->+0x98 = NULL; }
//         --site->m_pCtrlCont(+0x40)->+0xe0;
//         site->+0xb4 = 0;
//     }
//     return S_OK;
// Left as generated: DetachWindow is implemented in OpenMFC but needs the
// site, and the +0xb4/+0x98 members do not exist here.  The generated S_OK
// matches retail's unconditional return.
extern "C" long MS_ABI impl__OnInPlaceDeactivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return 0;
}

// Symbol: ?OnInPlaceDeactivateEx@XOleIPSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleIPSite::OnInPlaceDeactivateEx() — no RVA of its own;
// slot 16 of the XOleIPSite vftable (0x328fa8) points at 0x12ab40, a
// three-instruction forwarding thunk:
//     mov (%rcx),%rax ; mov 0x58(%rax),%rax ; rex.W jmp *__guard_dispatch
// i.e. a tail call through the sub-object's own vftable byte 0x58 = slot 11 =
// OnInPlaceDeactivate, with fNoRedraw dropped.  Transcribed as a direct call
// to this file's OnInPlaceDeactivate thunk; that is the one deviation — retail
// dispatches virtually, and OpenMFC has no vftable here to dispatch through,
// so a derived override could not be reached.
extern "C" long MS_ABI impl__OnInPlaceDeactivateEx_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fNoRedraw) {
    (void)fNoRedraw;
    return impl__OnInPlaceDeactivate_XOleIPSite_COleControlSite__UEAAJXZ(pThisItf);
}

// Symbol: ?OnPosRectChange@XOleIPSite@COleControlSite@@UEAAJPEBUtagRECT@@@Z
// COleControlSite::XOleIPSite::OnPosRectChange() — retail 0x23ab90, fully
// decoded.  It first validates three things, throwing CInvalidArgException
// (AfxThrowInvalidArgException, 0x225b80) if any fails: m_pCtrlCont(+0x40),
// m_pCtrlCont->m_pWnd(+0x40) and m_lpInPlaceObject(+0x88).  Then:
//     RECT clip = {0,0,0,0};
//     ::GetClientRect(m_pCtrlCont->m_pWnd->m_hWnd, &clip);   ; import 0x2c5358
//     ::CopyRect(&tmp, lprcPosRect);                         ; import 0x2c5240
//     site->rect(+0x5c) = tmp;                               ; 16-byte store
//     return m_lpInPlaceObject->SetObjectRects(&site->rect, &clip);
//                                                            ; vtable byte 0x38
// Note the returned HRESULT is SetObjectRects's, not a fixed S_OK.  Left as
// generated: the site, its position rect and its in-place object are all out
// of reach from the sub-object pointer.
extern "C" long MS_ABI impl__OnPosRectChange_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT___Z(
    void* pThisItf, const RECT* lprcPosRect) {
    (void)pThisItf; (void)lprcPosRect;
    return 0;
}

// Symbol: ?OnRequestEdit@XPropertyNotifySink@COleControlSite@@UEAAJJ@Z
// COleControlSite::XPropertyNotifySink::OnRequestEdit() — retail 0x23b6c0.
// After the module-state prologue it returns S_OK at once if either the site's
// +0xd0 flag is set or its CDataSourceControl at site+0xd8 is non-null.
// Otherwise it builds an AFX_EVENT of kind 1 (request edit) carrying the
// dispid, zeroes the rest, calls COleControlSite::OnEvent (0x239b50) and
// returns the event's result field at +0x24.  Left as generated: OnEvent needs
// the site.  The generated S_OK is both retail's early-out value and the value
// the AFX_EVENT carries when nothing objects.
extern "C" long MS_ABI impl__OnRequestEdit_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(
    void* pThisItf, long dispid) {
    (void)pThisItf; (void)dispid;
    return 0;
}

// Symbol: ?OnRowChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@_KQEB_KKKH@Z
// COleControlSite::XRowsetNotify::OnRowChange() — retail 0x23f0c0.  It maps
// the phase argument through the same 0->1, 1->4, 2->6, 3->5, 4->7 ladder as
// OnFieldChange, and separately maps the reason argument (4, 6, 8, 0xd) onto a
// second code, then drives the CDataSourceControl at site+0xd8 accordingly
// before raising the event through COleControlSite::OnEvent (0x239b50).  With
// no CDataSourceControl it returns S_OK without doing anything (0x23f231).  Left as
// generated for the same reason as the rest of XRowsetNotify.
extern "C" long MS_ABI impl__OnRowChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___KQEB_KKKH_Z(
    void* pThisItf, void* pRowset, unsigned __int64 cRows, const unsigned __int64* rghRows,
    unsigned long eReason, unsigned long eNotifyPhase, int fIsRecoverable) {
    (void)pThisItf; (void)pRowset; (void)cRows; (void)rghRows; (void)eReason;
    (void)eNotifyPhase; (void)fIsRecoverable;
    return 0;
}

// Symbol: ?OnRowsetChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@KKH@Z
// COleControlSite::XRowsetNotify::OnRowsetChange() — retail 0x23f260.  It does
// nothing unless eReason == 0xe; then it takes the CDataSourceControl at
// site+0xd8 (reached as -0xa0 from this sub-object at site+0x178) and rebuilds
// its row cache: it releases the accessor and column bindings held at +0x10,
// +0x20, +0x28 and +0x30 of the cache block at CDataSourceControl+0xa8,
// re-creates them through the two helpers at 0x236cec/0x236f2c, and if the
// column count changed reallocates a 0x40-bytes-per-column buffer.  Left as
// generated: none of that structure exists in OpenMFC.
extern "C" long MS_ABI impl__OnRowsetChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset__KKH_Z(
    void* pThisItf, void* pRowset, unsigned long eReason, unsigned long eNotifyPhase,
    int fIsRecoverable) {
    (void)pThisItf; (void)pRowset; (void)eReason; (void)eNotifyPhase; (void)fIsRecoverable;
    return 0;
}

// Symbol: ?OnShowWindow@XOleClientSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleClientSite::OnShowWindow() — no RVA of its own; slot 7
// of the XOleClientSite vftable (0x3290f8) is the folded body 0x7260, an
// unconditional S_OK with fShow ignored.  (COleClientItem's same-named method
// at 0x245e70 does real work; COleControlSite's does not — they are different
// functions.)  Transcribed exactly.
extern "C" long MS_ABI impl__OnShowWindow_XOleClientSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fShow) {
    (void)pThisItf; (void)fShow;
    return S_OK;
}

// Symbol: ?OnUIActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::OnUIActivate() — retail 0x23a9a0 is:
//     lea -0x140(%rcx),%rdx        ; the site
//     mov 0x40(%rdx),%rcx          ; m_pCtrlCont
//     test %rcx,%rcx ; je throw    ; AfxThrowInvalidArgException (0x225b80)
//     call *0xd0(vftable)          ; slot 26 = COleControlContainer::OnUIActivate
//                                  ; (0x234650; the slot was read out of that
//                                  ;  class's vftable at 0x328be8), passed the
//                                  ;  site as its argument
//     xor %eax,%eax ; ret
// Left as generated: the container call needs the site.  The generated S_OK
// matches retail's unconditional return.
extern "C" long MS_ABI impl__OnUIActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return 0;
}

// Symbol: ?OnUIDeactivate@XOleIPSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleIPSite::OnUIDeactivate() — retail 0x23aab0 is the same
// shape as OnUIActivate above but through slot 27 (byte 0xd8) =
// COleControlContainer::OnUIDeactivate (0x2346a0), and with no null check on
// m_pCtrlCont.  fUndoable is ignored.  Returns S_OK unconditionally.  Left as
// generated for the same reason.
extern "C" long MS_ABI impl__OnUIDeactivate_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fUndoable) {
    (void)pThisItf; (void)fUndoable;
    return 0;
}

// Symbol: ?ReleaseDC@XOleIPSite@COleControlSite@@UEAAJPEAUHDC__@@@Z
// COleControlSite::XOleIPSite::ReleaseDC() — retail 0x23b0d0.  After the
// module-state prologue it ignores the caller's HDC entirely and releases the
// site's own cached CDC instead:
//     ::ReleaseDC(m_pCtrlCont->m_pWnd->m_hWnd, site->cachedDC(+0x130)->m_hDC(+0x8));
//                                              ; import at 0x2c4ec0
//     site->+0x130 = NULL;
//     return S_OK;
// Left as generated: OpenMFC has no +0x130 CDC cache and cannot recover the
// site.  The generated S_OK matches retail's unconditional return.
extern "C" long MS_ABI impl__ReleaseDC_XOleIPSite_COleControlSite__UEAAJPEAUHDC_____Z(
    void* pThisItf, HDC hDC) {
    (void)pThisItf; (void)hDC;
    return 0;
}

// Symbol: ?RequestNewObjectLayout@XOleClientSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleClientSite::RequestNewObjectLayout() — no RVA of its
// own; slot 8 of the XOleClientSite vftable (0x3290f8) is the folded 0xf540,
// an unconditional E_NOTIMPL.  Transcribed exactly.
extern "C" long MS_ABI impl__RequestNewObjectLayout_XOleClientSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;
}

// Symbol: ?RequestUIActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleIPSite::RequestUIActivate() — retail 0x23ace0 is
// METHOD_PROLOGUE (the module state at this-0x108, handed to
// AFX_MAINTAIN_STATE2 at 0x133df0) around `xor %eax,%eax`, i.e. an
// unconditional S_OK.  No site state is read, so the body is reproduced
// exactly.  This is the body that mfc140u folds with CanWindowlessActivate,
// which is why that function above cites the same shape.
extern "C" long MS_ABI impl__RequestUIActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_OK;
}

// Symbol: ?SaveObject@XOleClientSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleClientSite::SaveObject() — no RVA of its own; slot 3 of
// the XOleClientSite vftable (0x3290f8) is the folded body 0xf540,
// `mov $0x80004001,%eax; ret`.  An unconditional E_NOTIMPL that reads nothing,
// so it is transcribed exactly.  (The slot numbering is pinned by the named
// entries in the same vftable: slot 5 GetContainer 0x23a8e0, slot 6 ShowObject
// 0x23a900.)
extern "C" long MS_ABI impl__SaveObject_XOleClientSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;
}

// Symbol: ?Scroll@XOleIPSite@COleControlSite@@UEAAJUtagSIZE@@@Z
// COleControlSite::XOleIPSite::Scroll() — no RVA of its own; slot 9 of the
// XOleIPSite vftable (0x328fa8) is the folded body 0x3ae0,
// `mov $0x1,%eax; ret`.  As an HRESULT that is S_FALSE: the site refuses to
// scroll and touches nothing.  Transcribed exactly; the generated `return 0`
// (S_OK) was the opposite answer and would tell a windowless control the
// container had scrolled it.
extern "C" long MS_ABI impl__Scroll_XOleIPSite_COleControlSite__UEAAJUtagSIZE___Z(
    void* pThisItf, SIZE scrollExtant) {
    (void)pThisItf; (void)scrollExtant;
    return S_FALSE;
}

// Symbol: ?ScrollRect@XOleIPSite@COleControlSite@@UEAAJHHPEBUtagRECT@@0@Z
// COleControlSite::XOleIPSite::ScrollRect() — retail 0x23ad10, the body slot
// 27 of the XOleIPSite vftable (0x328fa8) shares with slots 21, 28 and 29
// (GetFocus, AdjustRect, OnDefWindowMessage): METHOD_PROLOGUE around
// `mov $0x80004001,%eax`, i.e. E_NOTIMPL.  No argument and no site member is
// read.  Transcribed exactly; the generated `return 0` claimed success.
// The generated parameter list was also wrong (four ints and no `this`); the
// mangled name is (this, int, int, const RECT*, const RECT*), and nothing
// outside this file declares this thunk.
extern "C" long MS_ABI impl__ScrollRect_XOleIPSite_COleControlSite__UEAAJHHPEBUtagRECT__0_Z(
    void* pThisItf, int dx, int dy, const RECT* prcScroll, const RECT* prcClip) {
    (void)pThisItf; (void)dx; (void)dy; (void)prcScroll; (void)prcClip;
    return E_NOTIMPL;
}

// Symbol: ?SetCapture@XOleIPSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleIPSite::SetCapture() — retail 0x23b130.  After the
// module-state prologue:
//     if (fCapture) {
//         CWnd::FromHandle(::SetCapture(m_pCtrlCont->m_pWnd->m_hWnd));
//                                  ; USER32 import 0x2c5278, CWnd::FromHandle
//                                  ; at 0x289180 (result discarded)
//         m_pCtrlCont->+0xd0 = site;          ; the container's capture site
//     } else {
//         m_pCtrlCont->+0xd0 = NULL;
//         ::ReleaseCapture();                 ; import 0x2c5280
//     }
//     return S_OK;                            ; unconditional
// Left as generated: the site cannot be recovered from the sub-object pointer
// and COleControlContainer has no +0xd0 member in OpenMFC.  The generated
// S_OK is retail's unconditional return; the capture itself is not taken.
extern "C" long MS_ABI impl__SetCapture_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fCapture) {
    (void)pThisItf; (void)fCapture;
    return 0;   // retail: unconditional S_OK
}

// Symbol: ?SetFocus@XOleIPSite@COleControlSite@@UEAAJH@Z
// COleControlSite::XOleIPSite::SetFocus() — retail 0x23b1b0, the same shape as
// SetCapture above but through the container's focus-site member at +0xd8:
//     if (fFocus) { m_pCtrlCont->+0xd8 = site;
//                   CWnd::SetFocus(m_pCtrlCont->m_pWnd); }   ; 0x2a7a70
//     else        { m_pCtrlCont->+0xd8 = NULL; }
//     return S_OK;                                           ; unconditional
// Left as generated for the same reason; the generated S_OK is retail's return.
extern "C" long MS_ABI impl__SetFocus_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fFocus) {
    (void)pThisItf; (void)fFocus;
    return 0;   // retail: unconditional S_OK
}

// Symbol: ?ShowObject@XOleClientSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleClientSite::ShowObject() — retail 0x23a900:
//     lea -0x138(%rcx),%rbx        ; the site
//     ... METHOD_PROLOGUE ...
//     call 0x180239a30             ; COleControlSite::AttachWindow()
//     xor %eax,%eax ; ret          ; S_OK
// Left as generated: AttachWindow needs the site, which cannot be recovered
// from the sub-object pointer.  The generated S_OK is retail's unconditional
// return; only the AttachWindow side effect is missing.
extern "C" long MS_ABI impl__ShowObject_XOleClientSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return 0;   // retail: S_OK after COleControlSite::AttachWindow()
}

// Symbol: ?ShowPropertyFrame@XOleControlSite@COleControlSite@@UEAAJXZ
// COleControlSite::XOleControlSite::ShowPropertyFrame() — no RVA of its own;
// slot 9 of the XOleControlSite vftable (0x3290a0) is the folded 0xf540, an
// unconditional E_NOTIMPL.  Transcribed exactly; the generated `return 0` told
// the control the container had shown its property frame.
extern "C" long MS_ABI impl__ShowPropertyFrame_XOleControlSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;
}

// Symbol: ?SyncAfter@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// COleControlSite::XNotifyDBEvents::SyncAfter() — retail 0x23eca0.  After the
// module-state prologue it calls XNotifyDBEvents::FireEvent (0x23ee00) with
// phase 6 at [rsp+0x20] and returns that HRESULT when it is negative.
// Otherwise, when the CDataSourceControl at site+0xd8 is non-null and its
// +0x88 flag is clear, and *pReason is neither 0x1e nor 0x1f, and
// (dwEventWhat & 3) is set, and *pReason is not 2 or 3, it sets +0x88, calls
// CDataSourceControl::GetBoundClientRow (0x23cb00), clears +0x88 and calls
// that object's vftable byte 0x28 (slot 5).  Returns S_OK in every one of
// those cases.  Left as generated: the sub-object layout and the
// data-source-control plumbing are both absent from OpenMFC.
// The generated parameter list was wrong (five parameters from a bad
// demangle); the mangled name is (this, DWORD, DWORD, DBNOTIFYREASON* const),
// and nothing outside this file declares this thunk.
extern "C" long MS_ABI impl__SyncAfter_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThisItf; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return 0;   // retail: S_OK unless FireEvent failed
}

// Symbol: ?SyncBefore@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
// COleControlSite::XNotifyDBEvents::SyncBefore() — retail 0x23eb30, the same
// shape with phase 3 at [rsp+0x20].  On a non-negative FireEvent result it
// looks at *pReason: only reason 2, 3 or 0x14 continues, and then only when
// the CDataSourceControl at site+0xd8 is non-null and bit 7 (0x80) of
// dwEventWhat is set (`test %sil,%sil; jns`) does it call that object's
// vftable byte 0x30 (slot 6).
// Every one of those paths returns S_OK.  Left as generated, and the same
// five-parameter demangle was corrected here.
extern "C" long MS_ABI impl__SyncBefore_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long dwEventWhen, void* pReason) {
    (void)pThisItf; (void)dwEventWhat; (void)dwEventWhen; (void)pReason;
    return 0;   // retail: S_OK unless FireEvent failed
}

// Symbol: ?TransformCoords@XOleControlSite@COleControlSite@@UEAAJPEAU_POINTL@@PEAUtagPOINTF@@K@Z
// COleControlSite::XOleControlSite::TransformCoords() — retail 0x23b270.  It
// builds a stack CDC (vftable 0x338fb0) and attaches ::GetDC(site->m_hWnd)
// to it (USER32 import 0x2c4eb8; the site's m_hWnd is reached as -0x100 from
// this sub-object at site+0x148, i.e. site+0x48; CDC::Attach at 0x2a03c0).
// Then, on dwFlags:
//   XFORMCOORDS_HIMETRICTOCONTAINER (4): copy *pPtlHimetric, run
//     CDC::HIMETRICtoDP (0x2a3460), and write the pair into pPtfContainer as
//     floats -- through labs() first when XFORMCOORDS_SIZE (2) is set, plain
//     when XFORMCOORDS_POSITION (1) is;
//   XFORMCOORDS_CONTAINERTOHIMETRIC (8): the inverse, cvttss2si then
//     CDC::DPtoHIMETRIC (0x2a3390), back into pPtlHimetric;
//   neither, or neither SIZE nor POSITION: return E_INVALIDARG (0x80070057).
// Every converting path returns S_OK.  Left as generated: the site, and hence
// the DC to scale against, cannot be recovered from the sub-object pointer,
// and the generated S_OK matches retail's success return.
extern "C" long MS_ABI impl__TransformCoords_XOleControlSite_COleControlSite__UEAAJPEAU_POINTL__PEAUtagPOINTF__K_Z(
    void* pThisItf, void* pPtlHimetric, void* pPtfContainer, unsigned long dwFlags) {
    (void)pThisItf; (void)pPtlHimetric; (void)pPtfContainer; (void)dwFlags;
    return 0;   // retail: S_OK on every converting path
}

// Symbol: ?TranslateAcceleratorW@XOleControlSite@COleControlSite@@UEAAJPEAUtagMSG@@K@Z
// COleControlSite::XOleControlSite::TranslateAccelerator() — no RVA of its
// own; slot 7 of the XOleControlSite vftable (0x3290a0) is the folded 0xf540,
// an unconditional E_NOTIMPL that reads neither pMsg nor grfModifiers.
// Transcribed exactly; the generated `return 0` claimed the container had
// consumed the keystroke.
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleControlSite_COleControlSite__UEAAJPEAUtagMSG__K_Z(
    void* pThisItf, MSG* pMsg, unsigned long grfModifiers) {
    (void)pThisItf; (void)pMsg; (void)grfModifiers;
    return E_NOTIMPL;
}
