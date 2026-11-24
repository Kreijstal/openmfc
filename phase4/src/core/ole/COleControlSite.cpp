// COleControlSite — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#define ASSERT_CONTROLSITE_TAIL(member, offset) \
    static_assert(offsetof(COleControlSite, m_retailTail) + \
                      offsetof(COleControlSite::RetailTailStorage, member) == (offset), \
                  "COleControlSite::RetailTailStorage::" #member)
static_assert(sizeof(COleControlSite) == 0x1a0,
              "COleControlSite's current OpenMFC size remains 0x1a0");
static_assert(offsetof(COleControlSite, m_retailTail) == 0x78,
              "COleControlSite tail begins at +0x78");
ASSERT_CONTROLSITE_TAIL(m_reserved_78, 0x78);
ASSERT_CONTROLSITE_TAIL(m_pObject, 0x80);
ASSERT_CONTROLSITE_TAIL(m_pInPlaceObject, 0x88);
ASSERT_CONTROLSITE_TAIL(m_pActiveObject, 0x90);
ASSERT_CONTROLSITE_TAIL(m_pWindowlessObject, 0x98);
ASSERT_CONTROLSITE_TAIL(m_dwEventSink, 0xa0);
ASSERT_CONTROLSITE_TAIL(m_dwPropNotifySink, 0xa4);
ASSERT_CONTROLSITE_TAIL(m_dwStyleMask, 0xa8);
ASSERT_CONTROLSITE_TAIL(m_dwStyleRetail, 0xac);
ASSERT_CONTROLSITE_TAIL(m_dwMiscStatus, 0xb0);
ASSERT_CONTROLSITE_TAIL(m_bIsWindowless, 0xb4);
ASSERT_CONTROLSITE_TAIL(m_ctlInfo, 0xb8);
ASSERT_CONTROLSITE_TAIL(m_bIgnoreNotify, 0xd0);
ASSERT_CONTROLSITE_TAIL(m_dwNotifyDBEvents, 0xd4);
ASSERT_CONTROLSITE_TAIL(m_pDataSourceControl, 0xd8);
ASSERT_CONTROLSITE_TAIL(m_pBindings, 0xe0);
ASSERT_CONTROLSITE_TAIL(m_pDSCSiteOrRowSource, 0xe8);
ASSERT_CONTROLSITE_TAIL(m_defdispid, 0xf0);
ASSERT_CONTROLSITE_TAIL(m_dwType, 0xf4);
ASSERT_CONTROLSITE_TAIL(m_strDataFieldStorage, 0xf8);
ASSERT_CONTROLSITE_TAIL(m_bIsDirty, 0x100);
ASSERT_CONTROLSITE_TAIL(m_reserved_104, 0x104);
ASSERT_CONTROLSITE_TAIL(m_varResult, 0x108);
ASSERT_CONTROLSITE_TAIL(m_dispDriverStorage, 0x118);
ASSERT_CONTROLSITE_TAIL(m_pDC, 0x128);
ASSERT_CONTROLSITE_TAIL(m_reserved_130, 0x130);
ASSERT_CONTROLSITE_TAIL(m_xOleClientSiteVftable, 0x138);
ASSERT_CONTROLSITE_TAIL(m_xOleIPSiteVftable, 0x140);
ASSERT_CONTROLSITE_TAIL(m_xOleControlSiteVftable, 0x148);
ASSERT_CONTROLSITE_TAIL(m_xAmbientPropsVftable, 0x150);
ASSERT_CONTROLSITE_TAIL(m_xPropertyNotifySinkVftable, 0x158);
ASSERT_CONTROLSITE_TAIL(m_xEventSinkVftable, 0x160);
ASSERT_CONTROLSITE_TAIL(m_xBoundObjectSiteVftable, 0x168);
ASSERT_CONTROLSITE_TAIL(m_xNotifyDBEventsVftable, 0x170);
ASSERT_CONTROLSITE_TAIL(m_xRowsetNotifyVftable, 0x178);
ASSERT_CONTROLSITE_TAIL(m_reserved_180, 0x180);
#undef ASSERT_CONTROLSITE_TAIL

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
//   retail +0x50  m_pWndCtrl (CWnd*)   -> OleControlSiteState::windowControl
//   retail +0x58  control id           -> OleControlSiteState::controlId and
//                                          the shared control-site id accessor
//   retail +0x5c  position RECT        -> OleControlSiteState::rect
//   retail +0x6c  event IID            -> OleControlSiteState::eventIID
//   retail +0x80  IOleObject*          -> COleControlSite::m_lpObject
//   retail +0x88  IOleInPlaceObject*   -> COleControlSite::m_lpInPlaceObject
//   retail +0xa8  style mask           -> no OpenMFC equivalent (no mask applied)
//   retail +0xac  m_dwStyle            -> COleControlSite::m_dwStyle
//   retail +0xb0  cached misc status   -> queried from IOleObject on demand
//   retail +0xb4  "windowless-active"  -> approximated by (m_hWnd == NULL)
//   retail +0x98  IOleInPlaceObjectWindowless -> no OpenMFC equivalent
//   retail +0xb8  cached CONTROLINFO   -> OleControlSiteState::controlInfo
//   retail +0xd8  CDataSourceControl*  -> OleControlSiteState::dataSourceControl
//   retail +0xe0  CDataBoundProperty*  -> OleControlSiteState::bindings
//   retail +0x138 nested IOleClientSite-> detail::olecore::ControlSiteAdapter
//
// Retail vtable slots referenced below were read out of the COleControlSite
// vftable at RVA 0x32b308 (mfc140u), installed by the retail constructor:
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
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
extern "C" void* MS_ABI impl___0CDataSourceControl__QEAA_PEAVCOleControlSite___Z(void*, void*);
extern "C" long MS_ABI impl__Initialize_CDataSourceControl__QEAAJXZ(void*);
extern "C" void* MS_ABI impl___1CDataSourceControl__UEAA_XZ(void*);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void*);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(
    void*, AFX_MODULE_STATE*);
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void*);
extern "C" void MS_ABI impl__OnUIActivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(
    COleControlContainer*, COleControlSite*);
extern "C" void MS_ABI impl__OnUIDeactivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(
    COleControlContainer*, COleControlSite*);                       // core/ole/COleControlContainer.cpp
extern "C" void MS_ABI impl__DetachWindow_COleControlSite__QEAAXXZ(COleControlSite*);
extern "C" void MS_ABI impl__EnableDSC_COleControlSite__UEAAXXZ(void*);
extern "C" int MS_ABI impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(void*, void*);
extern "C" long MS_ABI impl__CreateControlCommon_COleControlSite__MEAAJPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(
    COleControlSite*, CWnd*, const GUID&, const CControlCreationInfo&, const wchar_t*,
    DWORD, const POINT*, const SIZE*, UINT, CFile*, BOOL, wchar_t*);
extern "C" long MS_ABI impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(void*);
extern "C" long MS_ABI impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
    void*, unsigned long, unsigned long, void*, int);   // core/db/CDataSourceControl.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);         // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd*);                    // core/window/Thunks.cpp
extern "C" CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC);             // core/gdi/CDC.cpp
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject*, HGDIOBJ);    // core/gdi/CGdiObject.cpp
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ);  // core/gdi/CGdiObject.cpp
extern "C" int MS_ABI impl__SelectClipRgn_CDC__QEAAHPEAVCRgn__H_Z(CDC*, CRgn*, int); // core/gdi/CDC.cpp
extern "C" int MS_ABI impl__Attach_CDC__QEAAHPEAUHDC_____Z(CDC*, HDC);               // core/gdi/CDCHandleMap.cpp
extern "C" HDC MS_ABI impl__Detach_CDC__QEAAPEAUHDC____XZ(CDC*);                     // core/gdi/CDCHandleMap.cpp
extern "C" void MS_ABI impl__HIMETRICtoDP_CDC__QEBAXPEAUtagSIZE___Z(const CDC*, SIZE*); // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__DPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(const CDC*, SIZE*); // core/gdi/CDC.cpp

namespace {

// Retail stores the position RECT at +0x5c and CONTROLINFO at +0xb8. Both
// now live in OleControlSiteState, keyed by the site. This small local list is
// retained only for the separately allocated CDataSourceControl, whose cleanup
// is tied to this translation unit's placement thunks.
//
// The list is plain singly linked rather than a std::map on purpose:
// std::map's insert/erase pull in the out-of-line libstdc++ red-black-tree
// helpers, and this translation unit is checked for exactly that kind of new
// external reference.  Sites are few, so a linear scan is adequate, and
// RemoveSiteExtra() below is called from ~COleControlSite (defined in this
// file) so entries do not outlive their site.
struct SiteExtra {
    const COleControlSite* site;
    SiteExtra* next;
    void* dataSourceControl;
};

// GetControlInfo entry RVA 0x23b1b0 (mfc140u) writes sizeof(CONTROLINFO) == 0x18 into ctlInfo.cb.
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
    p->dataSourceControl = nullptr;
    g_pSiteExtraHead = p;
    return *p;
}

void RemoveSiteExtra(const COleControlSite* pSite) {
    SiteExtra** pp = &g_pSiteExtraHead;
    while (*pp) {
        if ((*pp)->site == pSite) {
            SiteExtra* dead = *pp;
            *pp = dead->next;
            if (dead->dataSourceControl) {
                impl___1CDataSourceControl__UEAA_XZ(dead->dataSourceControl);
                impl___3_YAXPEAX_Z(dead->dataSourceControl);
            }
            delete dead;
            return;
        }
        pp = &(*pp)->next;
    }
}

// The site position rect (retail +0x5c).  Seeded once from the live window so
// that sites created through OpenMFC's CreateControl() still report a rect.
RECT& SiteRect(COleControlSite* pSite) {
    OleControlSiteState* state = GetOleControlSiteState(pSite, true);
    if (!state->rectValid) {
        state->rect = GetSitePositionRect(pSite);
        state->rectValid = true;
    }
    return state->rect;
}

// The control id retail keeps at +0x58. The shared per-site state is also the
// copy COleControlContainer reads through TryGetControlSiteId.
UINT SiteControlId(COleControlSite* pSite) {
    if (!pSite) return 0;
    OleControlSiteState* state = GetOleControlSiteState(pSite, true);
    return state->controlId;
}

void SetSiteControlId(COleControlSite* pSite, UINT nID) {
    if (!pSite) return;
    GetOleControlSiteState(pSite, true)->controlId = nID;
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

class SiteModuleStateGuard {
public:
    explicit SiteModuleStateGuard(AFX_MODULE_STATE* state) {
        impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(storage_, state);
    }
    ~SiteModuleStateGuard() {
        impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(storage_);
    }
    SiteModuleStateGuard(const SiteModuleStateGuard&) = delete;
    SiteModuleStateGuard& operator=(const SiteModuleStateGuard&) = delete;

private:
    void* storage_[2]{};
};

// AFX_EVENT (afxpriv2.h), the record COleControlSite::OnEvent routes.  The
// layout is the one the retail builders write: e.g. XNotifyDBEvents::FireEvent
// (mfc140u 0x240800) stores kind at +0x00, dispid at +0x04, zeroes +0x08..+0x1f,
// clears the HRESULT at +0x24 and writes DSCSTATE/DSCREASON at +0x28/+0x2c;
// XEventSink::Invoke (0x23d1f0) fills the three pointers at +0x08/+0x10/+0x18.
// Same layout as detail/CCmdTargetEventsSupport.h, which CCmdTarget::OnEvent
// reads; kept local because that header is not included here.
struct SiteEvent {
    int           m_eventKind;      // +0x00 event / propRequest / propChanged / propDSCNotify
    DISPID        m_dispid;         // +0x04
    DISPPARAMS*   m_pDispParams;    // +0x08
    EXCEPINFO*    m_pExcepInfo;     // +0x10
    UINT*         m_puArgError;     // +0x18
    BOOL          m_bPropChanged;   // +0x20 (no retail builder in this file writes it)
    HRESULT       m_hResult;        // +0x24
    int           m_nDSCState;      // +0x28
    int           m_nDSCReason;     // +0x2c
};
static_assert(offsetof(SiteEvent, m_dispid) == 0x04, "AFX_EVENT::m_dispid");
static_assert(offsetof(SiteEvent, m_pDispParams) == 0x08, "AFX_EVENT::m_pDispParams");
static_assert(offsetof(SiteEvent, m_pExcepInfo) == 0x10, "AFX_EVENT::m_pExcepInfo");
static_assert(offsetof(SiteEvent, m_puArgError) == 0x18, "AFX_EVENT::m_puArgError");
static_assert(offsetof(SiteEvent, m_hResult) == 0x24, "AFX_EVENT::m_hResult");
static_assert(offsetof(SiteEvent, m_nDSCState) == 0x28, "AFX_EVENT::m_nDSCState");
static_assert(offsetof(SiteEvent, m_nDSCReason) == 0x2c, "AFX_EVENT::m_nDSCReason");
static_assert(sizeof(SiteEvent) == 0x30, "AFX_EVENT size");

SiteEvent MakeSiteEvent(int kind, DISPID dispid) {
    SiteEvent e;
    e.m_eventKind = kind;
    e.m_dispid = dispid;
    e.m_pDispParams = nullptr;
    e.m_pExcepInfo = nullptr;
    e.m_puArgError = nullptr;
    e.m_bPropChanged = FALSE;
    e.m_hResult = S_OK;
    e.m_nDSCState = 0;   // dscNoState
    e.m_nDSCReason = 0;  // dscNoReason
    return e;
}

// CDataSourceControl as COleControlSite's notification sinks touch it.  The
// layout and the vtable slot order are those pinned in core/db/
// CDataSourceControl.cpp (retail vftable mfc140u 0x18032b4d0, whose order
// OpenMFC's g_dataSourceControlVtbl reproduces):
//   +0x88 m_bUpdateInProgress, +0xa0 m_pRowset (CRowset, m_hRow at +0x18);
//   slot 2 BindProp(CDataBoundProperty*, BOOL), slot 3 BindProp(COleControlSite*,
//   BOOL), slot 4 BindColumns, slot 5 UpdateControls, slot 6 UpdateCursor.
struct SiteDscView {
    void** vfptr;
    BYTE   m_reserved_08[0x80];
    DWORD  m_bUpdateInProgress;   // +0x88
    DWORD  m_reserved_8c;
    void*  m_reserved_90[2];
    BYTE*  m_pRowset;             // +0xa0
};
static_assert(offsetof(SiteDscView, m_bUpdateInProgress) == 0x88, "CDataSourceControl +0x88");
static_assert(offsetof(SiteDscView, m_pRowset) == 0xa0, "CDataSourceControl +0xa0");
constexpr size_t kRowsetHRowOffset = 0x18;   // CRowset::m_hRow, asserted in CDataSourceControl.cpp

using DscBindPropFn = void (MS_ABI*)(void*, void*, int);
using DscNoArgFn = long (MS_ABI*)(void*);
using DscVoidFn = void (MS_ABI*)(void*);

void DscBindPropProperty(void* pDsc, void* pProperty, int bBind) {   // slot 2
    reinterpret_cast<DscBindPropFn>(static_cast<SiteDscView*>(pDsc)->vfptr[2])(pDsc, pProperty, bBind);
}
void DscBindPropSite(void* pDsc, void* pSite, int bBind) {           // slot 3
    reinterpret_cast<DscBindPropFn>(static_cast<SiteDscView*>(pDsc)->vfptr[3])(pDsc, pSite, bBind);
}
void DscBindColumns(void* pDsc) {                                    // slot 4
    reinterpret_cast<DscVoidFn>(static_cast<SiteDscView*>(pDsc)->vfptr[4])(pDsc);
}
long DscUpdateControls(void* pDsc) {                                 // slot 5
    return reinterpret_cast<DscNoArgFn>(static_cast<SiteDscView*>(pDsc)->vfptr[5])(pDsc);
}
long DscUpdateCursor(void* pDsc) {                                   // slot 6
    return reinterpret_cast<DscNoArgFn>(static_cast<SiteDscView*>(pDsc)->vfptr[6])(pDsc);
}

// Retail site+0xd8 (m_pDataSourceControl).  EnableDSC stores the object in the
// RetailTailStorage member at that very offset (and in SiteExtra, which owns it).
void* SiteDataSourceControl(COleControlSite* pSite) {
    return pSite->m_retailTail.m_pDataSourceControl;
}

// The owner of an MFC nested interface part: retail METHOD_PROLOGUE subtracts
// the part's fixed offset (table in the nested-interface banner below).
COleControlSite* SiteFromPart(void* pPart, size_t offset) {
    return reinterpret_cast<COleControlSite*>(static_cast<char*>(pPart) - offset);
}

HWND SiteContainerHwnd(COleControlSite* pSite) {
    // retail: m_pCtrlCont(+0x40)->m_pWnd(+0x40)->m_hWnd(+0x40), no null checks
    return pSite->m_pCtrlCont->m_pWnd->m_hWnd;
}

// Container members retail COleControlContainer keeps at +0xd0 (the site that
// holds the mouse capture; written by XOleIPSite::SetCapture 0x23cb30, read by
// XOleIPSite::GetCapture) and +0xe0 (the count of windowless-active sites;
// incremented by XOleIPSite::OnInPlaceActivateEx 0x23c640, decremented by
// OnInPlaceDeactivate 0x23c4e0).  OpenMFC's container declares neither, so
// they live in this container-keyed list; the external helpers after this
// namespace expose them to core/ole/COleControlContainer.cpp.
struct OccContainerSiteState {
    const COleControlContainer* owner;
    OccContainerSiteState* next;
    COleControlSite* captureSite;
    int windowlessCount;
};
OccContainerSiteState* g_occContainerSiteStateHead = nullptr;

OccContainerSiteState* FindOccContainerSiteState(const COleControlContainer* pCont, bool create) {
    for (OccContainerSiteState* p = g_occContainerSiteStateHead; p; p = p->next) {
        if (p->owner == pCont) return p;
    }
    if (!create) return nullptr;
    OccContainerSiteState* p = new OccContainerSiteState();
    p->owner = pCont;
    p->next = g_occContainerSiteStateHead;
    p->captureSite = nullptr;
    p->windowlessCount = 0;
    g_occContainerSiteStateHead = p;
    return p;
}

// A destroyed site must not stay recorded as a container's capture site.
void ForgetCaptureSite(const COleControlSite* pSite) {
    for (OccContainerSiteState* p = g_occContainerSiteStateHead; p; p = p->next) {
        if (p->captureSite == pSite) p->captureSite = nullptr;
    }
}

}  // namespace

namespace openmfc { namespace detail { namespace olecore {
COleControlSite* GetOccContainerCaptureSite(const COleControlContainer* pCont) {
    OccContainerSiteState* p = FindOccContainerSiteState(pCont, false);
    return p ? p->captureSite : nullptr;
}
void SetOccContainerCaptureSite(COleControlContainer* pCont, COleControlSite* pSite) {
    if (pCont) FindOccContainerSiteState(pCont, true)->captureSite = pSite;
}
int GetOccContainerWindowlessCount(const COleControlContainer* pCont) {
    OccContainerSiteState* p = FindOccContainerSiteState(pCont, false);
    return p ? p->windowlessCount : 0;
}
void AdjustOccContainerWindowlessCount(COleControlContainer* pCont, int delta) {
    if (pCont) FindOccContainerSiteState(pCont, true)->windowlessCount += delta;
}
void ReleaseOccContainerSiteState(const COleControlContainer* pCont) {
    OccContainerSiteState** pp = &g_occContainerSiteStateHead;
    while (*pp) {
        if ((*pp)->owner == pCont) {
            OccContainerSiteState* dead = *pp;
            *pp = dead->next;
            delete dead;
            return;
        }
        pp = &(*pp)->next;
    }
}
// Defined in core/ole/COleControlContainer.cpp (retail container +0xd8).
void SetOccContainerFocusSite(COleControlContainer* pCont, COleControlSite* pSite);
} } }

























// Symbol: ??0COleControlSite@@QEAA@PEAVCOleControlContainer@@@Z
// COleControlSite::COleControlSite(COleControlContainer* pCtrlCont)
extern "C" COleControlSite* MS_ABI impl___0COleControlSite__QEAA_PEAVCOleControlContainer___Z(
    COleControlSite* pThis, COleControlContainer* pCtrlCont) {
    return ::new (static_cast<void*>(pThis)) COleControlSite(pCtrlCont);
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
    ForgetCaptureSite(this);
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
// COleControlSite::CreateControl(CWnd*, const CControlCreationInfo&, ...) —
// mfc140u 0x239c00 is a forwarder to CreateControlCommon (vtable slot 0x198):
//   return CreateControlCommon(pWndCtrl, creationInfo.m_clsid (+0x10), creationInfo,
//                              NULL, dwStyle, ppt, psize, nID, NULL, FALSE, NULL);
// OpenMFC calls the CreateControlCommon thunk directly (no retail vtable).
extern "C" long MS_ABI impl__CreateControl_COleControlSite__QEAAJPEAVCWnd__AEBVCControlCreationInfo__KPEBUtagPOINT__PEBUtagSIZE__I_Z(
    COleControlSite* pThis, CWnd* pWndCtrl, const CControlCreationInfo& creationInfo,
    DWORD dwStyle, const POINT* ppt, const SIZE* psize, UINT nID) {
    const GUID& clsid = *reinterpret_cast<const GUID*>(
        reinterpret_cast<const BYTE*>(&creationInfo) + 0x10);
    return impl__CreateControlCommon_COleControlSite__MEAAJPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(
        pThis, pWndCtrl, clsid, creationInfo, nullptr, dwStyle, ppt, psize, nID,
        nullptr, FALSE, nullptr);
}


// COleControlSite::CreateControl(CWnd*, REFCLSID, ...) — mfc140u 0x239b30
// builds a default CControlCreationInfo on the stack (m_hk = NullHandle (2) at
// +0x00, m_nHandle = 0 at +0x08; the CLSID at +0x10 is left unwritten) and
// forwards every argument to CreateControlCommon (vtable slot 0x198).  The
// local record follows the layout pinned in detail/CControlCreationInfoSupport.h;
// its unread CLSID is zeroed here.
extern "C" long MS_ABI impl__CreateControl_COleControlSite__UEAAJPEAVCWnd__AEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(
    COleControlSite* pThis, CWnd* pWndCtrl, const GUID& clsid, const wchar_t* lpszWindowName,
    DWORD dwStyle, const POINT* ppt, const SIZE* psize, UINT nID, CFile* pPersist,
    BOOL bStorage, wchar_t* bstrLicKey) {
    struct CreationInfoRecord {
        int m_hk;
        int m_pad;
        INT_PTR m_nHandle;
        CLSID m_clsid;
    };
    static_assert(offsetof(CreationInfoRecord, m_nHandle) == 0x08, "CControlCreationInfo::m_nHandle");
    static_assert(offsetof(CreationInfoRecord, m_clsid) == 0x10, "CControlCreationInfo::m_clsid");
    static_assert(sizeof(CreationInfoRecord) == 0x20, "CControlCreationInfo size");
    CreationInfoRecord info = {};
    info.m_hk = 2;   // CControlCreationInfo::NullHandle
    info.m_nHandle = 0;
    return impl__CreateControlCommon_COleControlSite__MEAAJPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(
        pThis, pWndCtrl, clsid, *reinterpret_cast<const CControlCreationInfo*>(&info),
        lpszWindowName, dwStyle, ppt, psize, nID, pPersist, bStorage, bstrLicKey);
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

// COleControlSite::BindDefaultProperty() — mfc140u 0x23c050:
//   if (m_pDSCSite(+0xe8)) {
//       m_pDSCSite->m_pDataSourceControl(+0xd8)->BindProp(this, FALSE);   // slot 3
//       m_pDSCSite->m_pDataSourceControl->BindColumns();                  // slot 4
//       m_pDSCSite = NULL;
//   }
//   if (pDSCWnd) {
//       pDSCWnd->m_pCtrlSite(CWnd+0xd0)->EnableDSC();                     // slot 0x178
//       m_pDSCSite = pDSCWnd->m_pCtrlSite;
//       m_defdispid(+0xf0) = dispid;  m_dwType(+0xf4) = vtProp;
//       m_strDataField(+0xf8).SetString(lpszFieldName, lpszFieldName ? wcslen : 0);
//       m_pDSCSite->m_pDataSourceControl->BindProp(this, TRUE);           // slot 3
//       m_pDSCSite->m_pDataSourceControl->BindColumns();                  // slot 4
//   }
// +0xd8/+0xe8/+0xf0/+0xf4 are RetailTailStorage fields at those offsets.  The
// tail's +0xf8 CString storage is raw (never constructed), so the field name is
// kept in OleControlSiteState::dataField.  EnableDSC is called through its
// thunk.  Retail dereferences a null pDSCWnd->m_pCtrlSite; that case returns.
extern "C" void MS_ABI impl__BindDefaultProperty_COleControlSite__UEAAXJGPEB_WPEAVCWnd___Z(
    void* pThis, long dispid, unsigned short vtProp, const wchar_t* lpszFieldName, void* pDSCWnd) {
    COleControlSite* pSite = static_cast<COleControlSite*>(pThis);
    COleControlSite::RetailTailStorage& tail = pSite->m_retailTail;
    if (tail.m_pDSCSiteOrRowSource) {
        COleControlSite* pOld = static_cast<COleControlSite*>(tail.m_pDSCSiteOrRowSource);
        DscBindPropSite(SiteDataSourceControl(pOld), pSite, FALSE);
        DscBindColumns(SiteDataSourceControl(pOld));
        tail.m_pDSCSiteOrRowSource = nullptr;
    }
    if (!pDSCWnd) return;
    COleControlSite* pDscSite = static_cast<CWnd*>(pDSCWnd)->m_pCtrlSite;
    if (!pDscSite) return;
    impl__EnableDSC_COleControlSite__UEAAXXZ(pDscSite);
    tail.m_pDSCSiteOrRowSource = pDscSite;
    tail.m_defdispid = dispid;
    tail.m_dwType = vtProp;
    GetOleControlSiteState(pSite, true)->dataField = lpszFieldName ? lpszFieldName : L"";
    DscBindPropSite(SiteDataSourceControl(pDscSite), pSite, TRUE);
    DscBindColumns(SiteDataSourceControl(pDscSite));
}

// COleControlSite::BindProperty() — mfc140u 0x23c170.  The bound-property
// list head is m_pBindings (+0xe0), whose nodes are 0x28-byte
// CDataBoundProperty records (layout pinned in core/ole/CDataBoundProperty.cpp:
// m_pClientSite +0x00, m_ctlid +0x08, m_dispid +0x0c, m_pDSCSite +0x10,
// m_bIsDirty +0x1c, m_pNext +0x20).
//   if (pWndDSC && dispid != DISPID_UNKNOWN (-1)) {
//       node = new CDataBoundProperty(m_pBindings, dispid, 0);   // inlined ctor
//       m_pBindings = node;
//       node->m_pDSCSite = pWndDSC->m_pCtrlSite(CWnd+0xd0);
//       node->m_pClientSite = this;
//       node->m_pDSCSite->EnableDSC();                              // slot 0x178
//       node->m_pDSCSite->m_pDataSourceControl(+0xd8)->BindProp(node, TRUE);  // slot 2
//   } else {
//       walk m_pBindings for the first node with (dispid == -1 || node->m_dispid
//       == dispid) && (!pWndDSC || node->m_pDSCSite == pWndDSC->m_pCtrlSite);
//       unlink it; if (node->m_pDSCSite && its m_pDataSourceControl)
//           ->BindProp(node, FALSE);                                // slot 2
//       operator delete(node, 0x28);
//   }
extern "C" void MS_ABI impl__BindProperty_COleControlSite__UEAAXJPEAVCWnd___Z(
    void* pThis, long dispid, void* pDSCWnd) {
    struct BoundPropertyNode {
        COleControlSite* m_pClientSite;
        WORD m_ctlid;
        DISPID m_dispid;
        COleControlSite* m_pDSCSite;
        BOOL m_bOwnXferOut;
        BOOL m_bIsDirty;
        BoundPropertyNode* m_pNext;
    };
    static_assert(offsetof(BoundPropertyNode, m_ctlid) == 0x08, "CDataBoundProperty::m_ctlid");
    static_assert(offsetof(BoundPropertyNode, m_dispid) == 0x0c, "CDataBoundProperty::m_dispid");
    static_assert(offsetof(BoundPropertyNode, m_pDSCSite) == 0x10, "CDataBoundProperty::m_pDSCSite");
    static_assert(offsetof(BoundPropertyNode, m_bIsDirty) == 0x1c, "CDataBoundProperty::m_bIsDirty");
    static_assert(offsetof(BoundPropertyNode, m_pNext) == 0x20, "CDataBoundProperty::m_pNext");
    static_assert(sizeof(BoundPropertyNode) == 0x28, "CDataBoundProperty size");

    COleControlSite* pSite = static_cast<COleControlSite*>(pThis);
    void*& head = pSite->m_retailTail.m_pBindings;
    CWnd* pWnd = static_cast<CWnd*>(pDSCWnd);
    if (pWnd && dispid != -1) {
        BoundPropertyNode* node = static_cast<BoundPropertyNode*>(impl___2_YAPEAX_K_Z(0x28));
        if (!node) return;
        node->m_ctlid = 0;
        node->m_pDSCSite = nullptr;
        node->m_bIsDirty = FALSE;
        node->m_pNext = static_cast<BoundPropertyNode*>(head);
        node->m_pClientSite = nullptr;
        node->m_dispid = dispid;
        head = node;
        node->m_pDSCSite = pWnd->m_pCtrlSite;
        node->m_pClientSite = pSite;
        impl__EnableDSC_COleControlSite__UEAAXXZ(node->m_pDSCSite);
        DscBindPropProperty(SiteDataSourceControl(node->m_pDSCSite), node, TRUE);
        return;
    }
    BoundPropertyNode* prev = nullptr;
    for (BoundPropertyNode* node = static_cast<BoundPropertyNode*>(head); node; node = node->m_pNext) {
        if ((dispid == -1 || node->m_dispid == dispid) &&
            (!pWnd || node->m_pDSCSite == pWnd->m_pCtrlSite)) {
            if (prev) prev->m_pNext = node->m_pNext;
            else head = node->m_pNext;
            if (node->m_pDSCSite) {
                void* pDsc = SiteDataSourceControl(node->m_pDSCSite);
                if (pDsc) DscBindPropProperty(pDsc, node, FALSE);
            }
            impl___3_YAXPEAX_Z(node);
            return;
        }
        prev = node;
    }
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
// Not implemented. Retail entry RVA 0x239c90 (mfc140u) writes the control
// window/id/rect/style, misc-status and sink-cookie fields now represented by
// RetailCOleControlSiteLayout and OleControlSiteState. It also back-links
// pWndCtrl->m_pCtrlSite (+0xd0), dispatches virtual CreateOrLoad slots 0x1a0/
// 0x1a8, and advises nested property/event sink interfaces at +0x158/+0x160.
// OpenMFC still lacks the compatible nested interface vtables and CWnd site
// backlink, so this body remains an explicit stub rather than a partial guess.
// Signature retained with the mangled export's reference and aggregate types.
extern "C" long MS_ABI impl__CreateControlCommon_COleControlSite__MEAAJPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_W_Z(
    COleControlSite* pThis, CWnd* pWndCtrl, const GUID& clsid,
    const CControlCreationInfo& creationInfo, const wchar_t* lpszWindowName,
    DWORD dwStyle, const POINT* ppt, const SIZE* psize, UINT nID, CFile* pPersist,
    BOOL bStorage, wchar_t* bstrLicKey) {
    (void)pThis; (void)pWndCtrl; (void)clsid; (void)creationInfo; (void)lpszWindowName;
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
// The +0xb0 misc-status cache and +0x6c event IID are now represented in
// OleControlSiteState. The object/interface slots are modeled, but compatible
// nested vtables and a complete persist/load adapter are still absent.
extern "C" long MS_ABI impl__CreateOrLoad_COleControlSite__MEAAJAEBU_GUID__PEAVCFile__HPEA_W_Z(
    COleControlSite* pThis, const GUID& clsid, CFile* pPersist, BOOL bStorage,
    wchar_t* bstrLicKey) {
    (void)pThis; (void)clsid; (void)pPersist; (void)bStorage; (void)bstrLicKey;
    return E_FAIL;
}

// Symbol: ?CreateOrLoad@COleControlSite@@MEAAJAEBVCControlCreationInfo@@@Z
// COleControlSite::CreateOrLoad(const CControlCreationInfo&) — retail 0x238d40
// (mfc140u.dll 0x23a740) is exactly two instructions:
//   mov $0x8000ffff,%eax ; ret
// i.e. an unconditional E_UNEXPECTED, with the creation info never read.  A
// derived site is expected to override it; the base never creates anything.
extern "C" long MS_ABI impl__CreateOrLoad_COleControlSite__MEAAJAEBVCControlCreationInfo___Z(
    COleControlSite* pThis, const CControlCreationInfo& creationInfo) {
    (void)pThis; (void)creationInfo;
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
// Retail mfc140u.dll RVA 0x23c000 returns if the site's +0xd8 pointer is
// already set; otherwise it allocates 0xb8 bytes, constructs
// CDataSourceControl with the site when allocation succeeds, stores the
// pointer, then calls Initialize even if allocation returned null. OpenMFC has
// the CDataSourceControl constructor/destructor thunks, but no +0xd8 site
// member, so SiteExtra holds the association. Its existing Initialize thunk is
// still a stub; this reproduces EnableDSC's allocation/construction/dispatch
// path without claiming the data-source initialization itself is implemented.
// This wrapper also returns for a null pThis, whereas retail dereferences it.
extern "C" void MS_ABI impl__EnableDSC_COleControlSite__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    SiteExtra& extra = SiteExtraOf(static_cast<COleControlSite*>(pThis));
    if (extra.dataSourceControl) return;

    void* pDataSourceControl = impl___2_YAPEAX_K_Z(0xb8);
    if (pDataSourceControl) {
        impl___0CDataSourceControl__QEAA_PEAVCOleControlSite___Z(pDataSourceControl, pThis);
        extra.dataSourceControl = pDataSourceControl;
        GetOleControlSiteState(static_cast<COleControlSite*>(pThis), true)->dataSourceControl =
            pDataSourceControl;
        // retail +0xd8, which RetailTailStorage overlays at the same offset
        static_cast<COleControlSite*>(pThis)->m_retailTail.m_pDataSourceControl = pDataSourceControl;
    }
    impl__Initialize_CDataSourceControl__QEAAJXZ(pDataSourceControl);
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
    CONTROLINFO& info = GetOleControlSiteState(pThis, true)->controlInfo;
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
// member; the id is read from OleControlSiteState, which SetSiteControlId()
// keeps in step with the shared per-site accessor that COleControlContainer reads.
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
// COleControlSite::GetStyle() — retail entry RVA 0x23b820 (mfc140u):
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
// The CONTROLINFO cache lives in OleControlSiteState; GetControlInfo fills it.
extern "C" int MS_ABI impl__IsMatchingMnemonic_COleControlSite__QEAAHPEAUtagMSG___Z(
    COleControlSite* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) return FALSE;
    const CONTROLINFO& info = GetOleControlSiteState(pThis, true)->controlInfo;
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

// COleControlSite::OnEvent() — mfc140u 0x23b550:
//   if (m_pWndCtrl(+0x50) &&
//       m_pWndCtrl->OnCmdMsg(m_nID(+0x58), CN_EVENT (-2), pEvent, NULL))  // slot 5
//       return TRUE;
//   if (!m_pCtrlCont(+0x40) || !m_pCtrlCont->m_pWnd(+0x40))
//       AfxThrowInvalidArgException();
//   return m_pCtrlCont->m_pWnd->OnCmdMsg(m_nID, CN_EVENT, pEvent, NULL);
// m_pWndCtrl and m_nID live in OleControlSiteState (see the map at the top of
// the file); OnCmdMsg is reached through CWnd vtable slot 5, as retail does.
extern "C" int MS_ABI impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(void* pThis, void* pEvent) {
    using OnCmdMsgFn = int (MS_ABI*)(void*, unsigned int, int, void*, void*);
    COleControlSite* pSite = static_cast<COleControlSite*>(pThis);
    OleControlSiteState* state = GetOleControlSiteState(pSite, false);
    CWnd* pWndCtrl = state ? state->windowControl : nullptr;
    const UINT nID = SiteControlId(pSite);
    if (pWndCtrl) {
        OnCmdMsgFn pfn = reinterpret_cast<OnCmdMsgFn>((*reinterpret_cast<void***>(pWndCtrl))[5]);
        if (pfn(pWndCtrl, nID, -2, pEvent, nullptr)) return TRUE;
    }
    if (!pSite->m_pCtrlCont || !pSite->m_pCtrlCont->m_pWnd) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    CWnd* pWnd = pSite->m_pCtrlCont->m_pWnd;
    OnCmdMsgFn pfn = reinterpret_cast<OnCmdMsgFn>((*reinterpret_cast<void***>(pWnd))[5]);
    return pfn(pWnd, nID, -2, pEvent, nullptr);
}

// COleControlSite::QuickActivate() — mfc140u 0x23a470.  Throws for a null
// m_pObject (+0x80); QueryInterface(IID_IQuickActivate) failing returns FALSE.
// It then fills a 0x70-byte QACONTAINER whose pClientSite, pPropertyNotifySink
// and pUnkEventSink are the site's nested interface parts at +0x138, +0x158 and
// +0x160, gathers eight ambient properties (table at 0x18034d860) plus
// DISPID_AMBIENT_BACKCOLOR/FORECOLOR/APPEARANCE/LOCALEID/FONT through the
// container window's CWnd::OnAmbientProperty (vtable byte 0x230), calls
// IQuickActivate::QuickActivate(&qaContainer, &qaControl) and on success caches
// qaControl.dwMiscStatus/dwEventCookie/dwPropNotifyCookie into +0xb0/+0xa0/+0xa4
// and returns TRUE.
// STUB: OpenMFC's COleControlSite constructor installs no COM vtables at the
// +0x138/+0x158/+0x160 parts, so there are no IOleClientSite/IPropertyNotifySink/
// event-sink pointers to hand the control.
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

// COleControlSite::SetFocus(MSG*) — mfc140u 0x23bf60:
//   if ((m_dwMiscStatus(+0xb0) & OLEMISC_NOUIACTIVATE) && !m_bIsWindowless(+0xb4))
//       return CWnd::FromHandle(::SetFocus(m_hWnd(+0x48)));
//   CWnd* pOld = CWnd::FromHandle(::GetFocus());
//   if (!(m_dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME))
//       if (SUCCEEDED(DoVerb(OLEIVERB_UIACTIVATE, pMsg)))     // vtable slot 0xc8
//           m_pCtrlCont(+0x40)->m_pSiteFocus(+0xd8) = this;
//   return pOld;
// The windowless-active flag is the RetailTailStorage member at +0xb4 (set by
// XOleIPSite::OnInPlaceActivateEx); the container's focus site is modelled by
// core/ole/COleControlContainer.cpp (SetOccContainerFocusSite).  The misc
// status is queried on demand (SiteMiscStatus).
extern "C" CWnd* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__PEAUtagMSG___Z(
    COleControlSite* pThis, MSG* pMsg) {
    if (!pThis) return nullptr;
    const DWORD dwMisc = SiteMiscStatus(pThis);
    if ((dwMisc & OLEMISC_NOUIACTIVATE) && !pThis->m_retailTail.m_bIsWindowless) {
        return CWnd::FromHandle(::SetFocus(pThis->m_hWnd));
    }
    CWnd* pOldFocus = CWnd::FromHandle(::GetFocus());
    if (!(dwMisc & OLEMISC_INVISIBLEATRUNTIME)) {
        if (SUCCEEDED(impl__DoVerb_COleControlSite__UEAAJJPEAUtagMSG___Z(pThis, OLEIVERB_UIACTIVATE, pMsg)) &&
            pThis->m_pCtrlCont) {
            SetOccContainerFocusSite(pThis->m_pCtrlCont, pThis);
        }
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
// COleControlSite::ShowWindow() — ANSI twin mfc140.dll RVA 0x23a380;
// --u has no mapped RVA for this export. Retail reads m_hWnd at +0x48;
// this OpenMFC declaration places the C++ member at +0x68 (measured with the
// phase4 compiler), so the body intentionally accesses the local member rather
// than a raw retail offset; see the header request in the result.
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
// Retail does NOT call ::ShowWindow here. The call at mfc140.dll RVA 0x23a398
// uses IAT slot 0x1802c5350, resolved by iat.py to USER32!IsWindowVisible.
// CWnd::ShowWindow (ANSI mfc140.dll RVA 0x2a79e0) routes a CWnd without a
// control site to the distinct USER32 ShowWindow thunk at mfc140.dll RVA
// 0x2c52b8, and routes a sited control through slot 0x150, which is this method.
// Retail shows/hides the OLE control by verb alone and samples visibility on
// entry; this existing body preserves that behavior.
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

// COleControlSite::XNotifyDBEvents::AboutToDo() — mfc140u 0x2405c0: module
// state scope, FireEvent(..., dscAboutToDo (4)); a negative HRESULT is
// returned, anything else becomes S_OK (`cmovs`).
extern "C" long MS_ABI impl__AboutToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    SiteModuleStateGuard stateGuard(SiteFromPart(pThisItf, 0x170)->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 4);
    return hr < 0 ? hr : S_OK;
}

// COleControlSite::XOleIPSite::AdjustRect() — mfc140u 0x23c710, the body the
// linker folded for XOleIPSite AdjustRect/GetFocus/ScrollRect/
// OnDefWindowMessage: AFX_MAINTAIN_STATE2 on the owning site's m_pModuleState
// (`mov -0x108(%rcx)` = part+0x140 -> site+0x38) around `return E_NOTIMPL`.
// lprcRect is neither read nor written.
extern "C" long MS_ABI impl__AdjustRect_XOleIPSite_COleControlSite__UEAAJPEAUtagRECT___Z(
    void* pThisItf, RECT* lprcRect) {
    (void)lprcRect;
    SiteModuleStateGuard stateGuard(SiteFromPart(pThisItf, 0x140)->m_pModuleState);
    return E_NOTIMPL;
}

// Symbol: ?CanInPlaceActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// The mfc140u lookup has no mapped RVA for this folded export. In the ANSI
// twin, the XOleIPSite vftable at RVA 0x328fa8 (mfc140) slot 5 points to the
// shared function body at RVA 0x7260 (mfc140), `xor %eax,%eax; ret`, so it
// returns S_OK without touching state. It remains a metric stub because its
// body is only a bare constant.
extern "C" long MS_ABI impl__CanInPlaceActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThis) {
    (void)pThis;
    return S_OK;
}

// Symbol: ?CanWindowlessActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// Retail mfc140u entry RVA 0x23c6e0 loads the owning site's m_pModuleState
// (the XOleIPSite part is at +0x140; the load is from site+0x38), enters an
// AFX_MAINTAIN_STATE2 scope, and returns S_OK. The state scope is reproduced
// through the existing constructor/destructor thunks.
extern "C" long MS_ABI impl__CanWindowlessActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    return S_OK;
}

// COleControlSite::XNotifyDBEvents::Cancelled() — mfc140u 0x2404c0: module
// state scope, FireEvent(..., dscCancelled (2)), and `cmovs`: the HRESULT is
// returned only when negative, S_OK otherwise.
extern "C" long MS_ABI impl__Cancelled_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    SiteModuleStateGuard stateGuard(SiteFromPart(pThisItf, 0x170)->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 2);
    return hr < 0 ? hr : S_OK;
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
// function's. For those the body was found by reading the sub-object's
// vftable out of .rdata instead of by name. All vtable and folded-body RVAs in
// the following list are from mfc140.dll (ANSI twin), not mfc140u.dll. The
// vftables are at
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
// OpenMFC's COleControlSite RetailTailStorage reserves pointer-sized words at
// +0x138..+0x178, but the constructor does not install compatible vtables or
// publish these parts through an interface map. Its client site remains the
// separate heap-allocated detail::olecore::ControlSiteAdapter. Thus retail
// callbacks are not currently exposed as valid OLE interfaces, although the
// asserted part offsets let a thunk recover its owner when directly called
// with a nested-part pointer. Methods that need missing sink, binding, or
// container state remain stubs; constant-only returns remain metric stubs.
// Where a stub's return value differs from the generated `return 0`, the
// reason is stated at that function; out-parameters are not reported as
// successful unless the body also writes them.
//=============================================================================

// Symbol: ?DeactivateAndUndo@XOleIPSite@COleControlSite@@UEAAJXZ
// Retail mfc140u entry RVA 0x23c560 reads m_lpInPlaceObject at site+0x88,
// throws AfxThrowInvalidArgException when it is null, otherwise calls the
// IOleInPlaceObject vtable slot at byte offset 0x30 (UIDeactivate), ignores
// that HRESULT, and returns S_OK. The owner is recovered from the +0x140
// storage offset. OpenMFC reserves that offset but does not publish a valid
// XOleIPSite interface; this body assumes a direct call with site+0x140.
extern "C" long MS_ABI impl__DeactivateAndUndo_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x140);
    if (!pSite->m_lpInPlaceObject) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_UNEXPECTED;
    }
    pSite->m_lpInPlaceObject->UIDeactivate();
    return S_OK;
}

// COleControlSite::XNotifyDBEvents::DidEvent() — mfc140u 0x240760:
//   scope; hr = FireEvent(..., dscDidEvent (7)); if (FAILED(hr)) return hr;
//   if (dsc = m_pDataSourceControl(+0xd8)) && (dwEventWhat & 0x80) {
//       saved = dsc->m_bUpdateInProgress; dsc->m_bUpdateInProgress = TRUE;
//       dsc->GetBoundClientRow(); dsc->m_bUpdateInProgress = saved;
//       dsc->UpdateControls();                               // slot 5
//   }
//   return S_OK;
extern "C" long MS_ABI impl__DidEvent_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x170);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 7);
    if (hr < 0) return hr;
    SiteDscView* pDsc = static_cast<SiteDscView*>(SiteDataSourceControl(pSite));
    if (pDsc && (dwEventWhat & 0x80)) {
        const DWORD saved = pDsc->m_bUpdateInProgress;
        pDsc->m_bUpdateInProgress = 1;
        impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(pDsc);
        pDsc->m_bUpdateInProgress = saved;
        DscUpdateControls(pDsc);
    }
    return S_OK;
}

// Symbol: ?DiscardUndoState@XOleIPSite@COleControlSite@@UEAAJXZ
// The mfc140u lookup has no mapped RVA. In the ANSI twin, the XOleIPSite
// vftable at RVA 0x328fa8 (mfc140) slot 12 points to the shared function body
// at RVA 0x7260 (mfc140), `xor %eax,%eax; ret` (S_OK). It remains a bare-return
// metric stub.
extern "C" long MS_ABI impl__DiscardUndoState_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_OK;
}

// COleControlSite::XNotifyDBEvents::FailedToDo() — mfc140u 0x240630: module
// state scope, FireEvent(..., dscFailedToDo (5)); negative HRESULT returned,
// otherwise S_OK.
extern "C" long MS_ABI impl__FailedToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    SiteModuleStateGuard stateGuard(SiteFromPart(pThisItf, 0x170)->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 5);
    return hr < 0 ? hr : S_OK;
}

// COleControlSite::XNotifyDBEvents::FireEvent() — mfc140u 0x240800.  Returns
// S_OK unless (dwEventWhat & 3) and cReasons are both non-zero.  Otherwise it
// scans rgReasons (0x38-byte DBNOTIFYREASON stride, dwReason at +0) and, for
// the first reason in the table below, raises an AFX_EVENT of kind 3
// (propDSCNotify), dispid -1, DSCSTATE nState and the mapped DSCREASON on the
// site (part-0x170) through OnEvent, returning the event's HRESULT.  Reasons
// outside the table are skipped; if none matches it returns S_OK.
//   1->3   2,20->5   3,9,10,23->6   6,7,8,11,12,15,16,19,21->7   24->2
//   25->1   31->4
// (decoded from the jump ladder 0x240827..0x2408ea and its targets.)
extern "C" long MS_ABI impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons, int dscState) {
    if (!(dwEventWhat & 3) || cReasons == 0) return S_OK;
    for (unsigned long i = 0; i < cReasons; ++i) {
        const DWORD dwReason = *reinterpret_cast<const DWORD*>(
            static_cast<const BYTE*>(rgReasons) + static_cast<size_t>(i) * 0x38);
        int dscReason;
        switch (dwReason) {
        case 1:  dscReason = 3; break;
        case 2: case 20: dscReason = 5; break;
        case 3: case 9: case 10: case 23: dscReason = 6; break;
        case 6: case 7: case 8: case 11: case 12: case 15: case 16: case 19: case 21:
            dscReason = 7; break;
        case 24: dscReason = 2; break;
        case 25: dscReason = 1; break;
        case 31: dscReason = 4; break;
        default: continue;
        }
        SiteEvent event = MakeSiteEvent(3, -1);
        event.m_nDSCState = dscState;
        event.m_nDSCReason = dscReason;
        impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(SiteFromPart(pThisItf, 0x170), &event);
        return event.m_hResult;
    }
    return S_OK;
}

// COleControlSite::XOleIPSite::GetCapture() — mfc140u 0x23c750 (ANSI twin
// 0x23ad50): after the module-state prologue,
//   return (m_pCtrlCont->m_pSiteCapture(+0xd0) == site) ? S_OK : S_FALSE;
// (`cmp %rbx,0xd0(%rcx); setne %al`).  The capture site is the
// OccContainerSiteState entry SetCapture writes.
extern "C" long MS_ABI impl__GetCapture_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    return GetOccContainerCaptureSite(pSite->m_pCtrlCont) == pSite ? S_OK : S_FALSE;
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

// COleControlSite::XOleIPSite::GetDC() — mfc140u 0x23c7a0:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);  CRgn rgn;
//   if (!phDC) AfxThrowInvalidArgException();
//   *phDC = NULL;
//   if (m_pDC(+0x130) != NULL) return E_FAIL;          // one DC at a time
//   m_pDC = CDC::FromHandle(::GetDC(m_pCtrlCont->m_pWnd->m_hWnd));
//   if (!m_pDC) return E_FAIL;
//   RECT rc; lprcRect ? ::IntersectRect(&rc, lprcRect, &m_rect(+0x5c)) : rc = m_rect;
//   rgn.Attach(::CreateRectRgnIndirect(&rc));
//   m_pDC->SelectClipRgn(&rgn, RGN_AND);
//   *phDC = m_pDC->GetSafeHdc();  return S_OK;           // ~CRgn deletes the region
// grfFlags is never read.  The DC cache is OleControlSiteState::dc (see
// ReleaseDC) and the position rect is SiteRect().
extern "C" long MS_ABI impl__GetDC_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__KPEAPEAUHDC_____Z(
    void* pThisItf, const RECT* lprcRect, unsigned long grfFlags, HDC* phDC) {
    (void)grfFlags;
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    CRgn rgn;
    if (!phDC) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_POINTER;
    }
    *phDC = nullptr;
    OleControlSiteState* state = GetOleControlSiteState(pSite, true);
    if (state->dc) return E_FAIL;
    state->dc = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(::GetDC(SiteContainerHwnd(pSite)));
    if (!state->dc) return E_FAIL;
    RECT rc;
    const RECT& siteRect = SiteRect(pSite);
    if (lprcRect) {
        ::IntersectRect(&rc, lprcRect, &siteRect);
    } else {
        rc = siteRect;
    }
    impl__Attach_CGdiObject__QEAAHPEAX_Z(&rgn, ::CreateRectRgnIndirect(&rc));
    impl__SelectClipRgn_CDC__QEAAHPEAVCRgn__H_Z(state->dc, &rgn, RGN_AND);
    *phDC = state->dc ? state->dc->m_hDC : nullptr;
    return S_OK;
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
// The mfc140u lookup has no mapped RVA. The ANSI twin's XOleIPSite vftable
// at RVA 0x328fa8 (mfc140) slot 21 points to shared function entry RVA
// 0x23ad10 (mfc140), which enters AFX_MAINTAIN_STATE2 using site+0x38 and
// returns E_NOTIMPL. The state scope is reproduced with the existing thunks.
extern "C" long MS_ABI impl__GetFocus_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
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

// COleControlSite::XOleIPSite::InvalidateRect() — mfc140u 0x23c8f0:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);  RECT rc = {0};
//   if (!lprcRect) rc = m_rect(+0x5c);
//   else { ::IntersectRect(&rc, lprcRect, &m_rect); ::EqualRect(&rc, lprcRect); }
//   if (!::IsRectEmpty(&rc))
//       ::InvalidateRect(m_pCtrlCont->m_pWnd->m_hWnd, &rc, fErase);
//   return S_OK;
// The EqualRect result is discarded by retail; the call is kept.
extern "C" long MS_ABI impl__InvalidateRect_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__H_Z(
    void* pThisItf, const RECT* lprcRect, int fErase) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    RECT rc = {};
    const RECT& siteRect = SiteRect(pSite);
    if (!lprcRect) {
        rc = siteRect;
    } else {
        ::IntersectRect(&rc, lprcRect, &siteRect);
        ::EqualRect(&rc, lprcRect);
    }
    if (!::IsRectEmpty(&rc)) {
        ::InvalidateRect(SiteContainerHwnd(pSite), &rc, fErase);
    }
    return S_OK;
}

// COleControlSite::XOleIPSite::InvalidateRgn() — mfc140u 0x23c9c0:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);  CRgn scratch;  RECT box = {0}, isect = {0};
//   if (hRgn) {
//       CGdiObject* pRgn = CGdiObject::FromHandle(hRgn);           // 0x2a3ea0
//       ::GetRgnBox(pRgn->m_hObject, &box);
//       ::IntersectRect(&isect, &box, &m_rect(+0x5c));
//       if (::EqualRect(&isect, &box)) {
//           ::InvalidateRgn(m_pCtrlCont->m_pWnd->m_hWnd, pRgn->m_hObject, fErase);
//           return S_OK;
//       }
//   }
//   return this->InvalidateRect(NULL, fErase);       // part vtable byte 0xc8
// OpenMFC installs no vtable at the XOleIPSite part, so the last line calls
// the InvalidateRect thunk directly.
extern "C" long MS_ABI impl__InvalidateRgn_XOleIPSite_COleControlSite__UEAAJPEAUHRGN____H_Z(
    void* pThisItf, HRGN hRgn, int fErase) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (hRgn) {
        CGdiObject* pRgn = impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(hRgn);
        HRGN hWrapped = static_cast<HRGN>(pRgn->m_hObject);
        RECT box = {};
        RECT isect = {};
        ::GetRgnBox(hWrapped, &box);
        ::IntersectRect(&isect, &box, &SiteRect(pSite));
        if (::EqualRect(&isect, &box)) {
            ::InvalidateRgn(SiteContainerHwnd(pSite), hWrapped, fErase);
            return S_OK;
        }
    }
    return impl__InvalidateRect_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT__H_Z(
        pThisItf, nullptr, fErase);
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

// COleControlSite::XEventSink::Invoke() — mfc140u 0x23d1f0:
//   site = this-0x160; AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   AFX_EVENT event(AFX_EVENT::event (0), dispIdMember, pDispParams, pExcepInfo, puArgErr);
//   site->OnEvent(&event);
//   if (pVarResult) VariantClear(pVarResult);               // oleaut32 #9
//   return event.m_hResult;
// riid, lcid and wFlags are not read.
extern "C" long MS_ABI impl__Invoke_XEventSink_COleControlSite__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThisItf, long dispIdMember, const GUID* riid, unsigned long lcid,
    unsigned short wFlags, void* pDispParams, void* pVarResult,
    void* pExcepInfo, unsigned int* puArgErr) {
    (void)riid; (void)lcid; (void)wFlags;
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x160);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    SiteEvent event = MakeSiteEvent(0, dispIdMember);
    event.m_pDispParams = static_cast<DISPPARAMS*>(pDispParams);
    event.m_pExcepInfo = static_cast<EXCEPINFO*>(pExcepInfo);
    event.m_puArgError = puArgErr;
    impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(pSite, &event);
    if (pVarResult) ::VariantClear(static_cast<VARIANT*>(pVarResult));
    return event.m_hResult;
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

// COleControlSite::XNotifyDBEvents::OKToDo() — mfc140u 0x240410:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);           // part-0x138 = site+0x38
//   hr = FireEvent(dwEventWhat, cReasons, rgReasons, dscOKToDo (1));
//   if (FAILED(hr)) return hr;
//   reason = rgReasons[0].dwReason;
//   if (reason == 0x1e || reason == 2 || reason == 3) return S_OK;
//   if (!(dwEventWhat & 1) && reason != 1 && reason != 0x14) return S_OK;
//   if (m_pDataSourceControl(+0xd8) && (dwEventWhat & 1))
//       return m_pDataSourceControl->UpdateCursor();      // slot 6
//   return S_OK;
extern "C" long MS_ABI impl__OKToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x170);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 1);
    if (hr < 0) return hr;
    const DWORD reason = *static_cast<const DWORD*>(rgReasons);
    if (reason == 0x1e || reason == 2 || reason == 3) return S_OK;
    if (!(dwEventWhat & 1) && reason != 1 && reason != 0x14) return S_OK;
    void* pDsc = SiteDataSourceControl(pSite);
    if (pDsc && (dwEventWhat & 1)) return DscUpdateCursor(pDsc);
    return S_OK;
}

// COleControlSite::XPropertyNotifySink::OnChanged() — mfc140u 0x23cee0:
//   site = this-0x158; AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   if (m_bIgnoreNotify(+0xd0)) return S_OK;
//   if (!m_pDataSourceControl(+0xd8)) {
//       AFX_EVENT event(AFX_EVENT::propChanged (2), dispid);
//       OnEvent(&event);
//       if (event.m_hResult != S_OK) return event.m_hResult;
//   }
//   if (dispid != m_defdispid(+0xf0)) return S_OK;
//   VariantClear(&m_varResult(+0x108));
//   if (!m_pObject(+0x80)) AfxThrowInvalidArgException();
//   if (FAILED(m_pObject->QueryInterface(IID_IDispatch, &pDisp))) return S_OK;
//   EXCEPINFO excep = {0}; DISPPARAMS params = {0}; UINT argErr;
//   if (!pDisp) AfxThrowInvalidArgException();
//   hr = pDisp->Invoke(dispid, IID_NULL, 0, DISPATCH_PROPERTYGET, &params,
//                      &m_varResult, &excep, &argErr);
//   SysFreeString(excep.bstrSource / bstrDescription / bstrHelpFile);
//   pDisp->Release();
//   if (SUCCEEDED(hr)) m_bIsDirty(+0x100) = TRUE;
//   return S_OK;
// The +0xd0/+0xd8/+0xf0/+0x100/+0x108 members are the RetailTailStorage
// fields at those offsets; m_pObject is OpenMFC's m_lpObject.
extern "C" long MS_ABI impl__OnChanged_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(
    void* pThisItf, long dispid) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x158);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    COleControlSite::RetailTailStorage& tail = pSite->m_retailTail;
    if (tail.m_bIgnoreNotify) return S_OK;
    if (!SiteDataSourceControl(pSite)) {
        SiteEvent event = MakeSiteEvent(2, dispid);
        impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(pSite, &event);
        if (event.m_hResult != S_OK) return event.m_hResult;
    }
    if (dispid != tail.m_defdispid) return S_OK;
    VARIANT* pResult = reinterpret_cast<VARIANT*>(tail.m_varResult);
    ::VariantClear(pResult);
    if (!pSite->m_lpObject) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return S_OK;
    }
    IDispatch* pDisp = nullptr;
    if (FAILED(pSite->m_lpObject->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pDisp)))) {
        return S_OK;
    }
    EXCEPINFO excep;
    memset(&excep, 0, sizeof(excep));
    DISPPARAMS params;
    memset(&params, 0, sizeof(params));
    UINT argErr = 0;
    if (!pDisp) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return S_OK;
    }
    HRESULT hr = pDisp->Invoke(dispid, IID_NULL, 0, DISPATCH_PROPERTYGET, &params, pResult,
                               &excep, &argErr);
    if (excep.bstrSource) ::SysFreeString(excep.bstrSource);
    if (excep.bstrDescription) ::SysFreeString(excep.bstrDescription);
    if (excep.bstrHelpFile) ::SysFreeString(excep.bstrHelpFile);
    pDisp->Release();
    if (SUCCEEDED(hr)) tail.m_bIsDirty = TRUE;
    return S_OK;
}

// Symbol: ?OnControlInfoChanged@XOleControlSite@COleControlSite@@UEAAJXZ
// Retail mfc140u entry RVA 0x23cc50 subtracts 0x148 from the XOleControlSite
// part, calls GetControlInfo (entry RVA 0x23b1b0, mfc140u), then returns S_OK.
// OpenMFC's getter refreshes the modeled side-table CONTROLINFO cache.
extern "C" long MS_ABI impl__OnControlInfoChanged_XOleControlSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x148);
    impl__GetControlInfo_COleControlSite__IEAAXXZ(pSite);
    return S_OK;
}

// COleControlSite::XOleIPSite::OnDefWindowMessage() — the folded body at
// mfc140u 0x23c710 (see AdjustRect): module-state scope, E_NOTIMPL; *plResult
// is not written.
extern "C" long MS_ABI impl__OnDefWindowMessage_XOleIPSite_COleControlSite__UEAAJI_K_JPEA_J_Z(
    void* pThisItf, unsigned int msg, unsigned long long wParam, long long lParam,
    long long* plResult) {
    (void)msg; (void)wParam; (void)lParam; (void)plResult;
    SiteModuleStateGuard stateGuard(SiteFromPart(pThisItf, 0x140)->m_pModuleState);
    return E_NOTIMPL;
}

// COleControlSite::XRowsetNotify::OnFieldChange() — mfc140u 0x2409c0:
//   site = this-0x178; AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   state = ePhase: OKTODO(0)->1, ABOUTTODO(1)->4, SYNCHAFTER(2)->6,
//           FAILEDTODO(3)->5, other->0, DIDEVENT(4)->7 where additionally
//       if (!m_pDataSourceControl(+0xd8)) return S_OK;
//       if (!dsc->m_bUpdateInProgress) { = TRUE; GetBoundClientRow();
//                                        = FALSE; UpdateControls(); }  // slot 5
//   AFX_EVENT event(propDSCNotify (3), -1); m_nDSCState = state;
//   m_nDSCReason = 6 (dscFieldChange?: the literal is 6);
//   OnEvent(&event);  return event.m_hResult;
extern "C" long MS_ABI impl__OnFieldChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___K1QEA_KKKH_Z(
    void* pThisItf, void* pRowset, unsigned __int64 hRow, unsigned __int64 cColumns,
    unsigned __int64* rgColumns, unsigned long eReason, unsigned long eNotifyPhase,
    int fIsRecoverable) {
    (void)pRowset; (void)hRow; (void)cColumns; (void)rgColumns; (void)eReason; (void)fIsRecoverable;
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x178);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    int dscState = 0;
    switch (eNotifyPhase) {
    case 0: dscState = 1; break;
    case 1: dscState = 4; break;
    case 2: dscState = 6; break;
    case 3: dscState = 5; break;
    case 4: {
        SiteDscView* pDsc = static_cast<SiteDscView*>(SiteDataSourceControl(pSite));
        if (!pDsc) return S_OK;
        dscState = 7;
        if (pDsc->m_bUpdateInProgress == 0) {
            pDsc->m_bUpdateInProgress = 1;
            impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(pDsc);
            pDsc->m_bUpdateInProgress = 0;
            DscUpdateControls(pDsc);
        }
        break;
    }
    default: break;
    }
    SiteEvent event = MakeSiteEvent(3, -1);
    event.m_nDSCState = dscState;
    event.m_nDSCReason = 6;
    impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(pSite, &event);
    return event.m_hResult;
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
// The mfc140u lookup has no mapped RVA. The ANSI twin's XOleIPSite vftable at
// RVA 0x328fa8 (mfc140) slot 6 points to the shared function body at RVA
// 0x7260 (mfc140), an unconditional S_OK. This no-state function remains a
// bare-return metric stub.
extern "C" long MS_ABI impl__OnInPlaceActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return S_OK;
}

// COleControlSite::XOleIPSite::OnInPlaceActivateEx() — mfc140u 0x23c640:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   if (dwFlags & ACTIVATE_WINDOWLESS) {
//       site->m_pObject(+0x80)->QueryInterface(IID_IOleInPlaceObjectWindowless
//           {1c2056cc-5ef4-101b-8bc8-00aa003e3b29}, &site->m_pWindowlessObject(+0x98));
//       site->m_bIsWindowless(+0xb4) = TRUE;     // whatever the QI returned
//       ++m_pCtrlCont->windowless count(+0xe0);
//   }
//   if (pfNoRedraw) *pfNoRedraw = FALSE;
//   return S_OK;
// OpenMFC's IOleObject is m_lpObject (see the map at the top of the file).
extern "C" long MS_ABI impl__OnInPlaceActivateEx_XOleIPSite_COleControlSite__UEAAJPEAHK_Z(
    void* pThisItf, int* pfNoRedraw, unsigned long dwFlags) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (dwFlags & ACTIVATE_WINDOWLESS) {
        pSite->m_lpObject->QueryInterface(IID_IOleInPlaceObjectWindowless,
                                          &pSite->m_retailTail.m_pWindowlessObject);
        pSite->m_retailTail.m_bIsWindowless = TRUE;
        AdjustOccContainerWindowlessCount(pSite->m_pCtrlCont, 1);
    }
    if (pfNoRedraw) *pfNoRedraw = FALSE;
    return S_OK;
}

// COleControlSite::XOleIPSite::OnInPlaceDeactivate() — mfc140u 0x23c4e0:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   site->DetachWindow();                                   // 0x23b4d0
//   if (site->m_bIsWindowless(+0xb4)) {
//       if (site->m_pWindowlessObject(+0x98)) { ->Release(); = NULL; }
//       --m_pCtrlCont->windowless count(+0xe0);
//       site->m_bIsWindowless = FALSE;
//   }
//   return S_OK;
// The flag and interface are the RetailTailStorage members at +0xb4/+0x98;
// the container count is the OccContainerSiteState entry (see top of file).
extern "C" long MS_ABI impl__OnInPlaceDeactivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    impl__DetachWindow_COleControlSite__QEAAXXZ(pSite);
    if (pSite->m_retailTail.m_bIsWindowless) {
        IUnknown* pWindowless = static_cast<IUnknown*>(pSite->m_retailTail.m_pWindowlessObject);
        if (pWindowless) {
            pWindowless->Release();
            pSite->m_retailTail.m_pWindowlessObject = nullptr;
        }
        AdjustOccContainerWindowlessCount(pSite->m_pCtrlCont, -1);
        pSite->m_retailTail.m_bIsWindowless = FALSE;
    }
    return S_OK;
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

// COleControlSite::XOleIPSite::OnPosRectChange() — mfc140u 0x23c590, no
// module-state scope:
//   if (!m_pCtrlCont || !m_pCtrlCont->m_pWnd || !m_pInPlaceObject(+0x88))
//       AfxThrowInvalidArgException();
//   RECT clip = {0}; ::GetClientRect(m_pCtrlCont->m_pWnd->m_hWnd, &clip);
//   RECT tmp; ::CopyRect(&tmp, lprcPosRect); m_rect(+0x5c) = tmp;
//   return m_pInPlaceObject->SetObjectRects(&m_rect, &clip);   // slot 7
extern "C" long MS_ABI impl__OnPosRectChange_XOleIPSite_COleControlSite__UEAAJPEBUtagRECT___Z(
    void* pThisItf, const RECT* lprcPosRect) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    if (!pSite->m_pCtrlCont || !pSite->m_pCtrlCont->m_pWnd || !pSite->m_lpInPlaceObject) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_UNEXPECTED;
    }
    RECT clip = {};
    ::GetClientRect(pSite->m_pCtrlCont->m_pWnd->m_hWnd, &clip);
    RECT tmp;
    ::CopyRect(&tmp, lprcPosRect);
    RECT& siteRect = SiteRect(pSite);
    siteRect = tmp;
    return pSite->m_lpInPlaceObject->SetObjectRects(&siteRect, &clip);
}

// COleControlSite::XPropertyNotifySink::OnRequestEdit() — mfc140u 0x23d0c0:
//   site = this-0x158; AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   if (m_bIgnoreNotify(+0xd0) || m_pDataSourceControl(+0xd8)) return S_OK;
//   AFX_EVENT event(AFX_EVENT::propRequest (1), dispid);
//   OnEvent(&event);  return event.m_hResult;
// Both members are the RetailTailStorage fields at those offsets.
extern "C" long MS_ABI impl__OnRequestEdit_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(
    void* pThisItf, long dispid) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x158);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (pSite->m_retailTail.m_bIgnoreNotify || SiteDataSourceControl(pSite)) return S_OK;
    SiteEvent event = MakeSiteEvent(1, dispid);
    impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(pSite, &event);
    return event.m_hResult;
}

// COleControlSite::XRowsetNotify::OnRowChange() — mfc140u 0x240ac0:
//   site = this-0x178; AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   state = ePhase: 0->1, 1->4, 2->6, 3->5, 4->7, other->0;
//   reason = eReason: 4->7, 6->3, 8->5, 13->2, other->0;
//   dsc = m_pDataSourceControl(+0xd8); if (!dsc) return S_OK;
//   if (reason == 2 && state == 4) { dsc->UpdateCursor(); goto fire; }  // slot 6
//   if ((reason == 7 && cRows == 1) || (state == 6 && reason == 5)) {
//       dsc->UpdateCursor();
//       dsc->m_pRowset(+0xa0)->m_hRow(+0x18) = rghRows[0];
//       saved = dsc->m_bUpdateInProgress; = TRUE; GetBoundClientRow(); = saved;
//       if (!saved) dsc->UpdateControls();                              // slot 5
//   }
//   if (reason == 0) return S_OK;
// fire:
//   AFX_EVENT event(propDSCNotify (3), -1); state/reason as above;
//   OnEvent(&event);  return FAILED(event.m_hResult) ? event.m_hResult : S_OK;
extern "C" long MS_ABI impl__OnRowChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___KQEB_KKKH_Z(
    void* pThisItf, void* pRowset, unsigned __int64 cRows, const unsigned __int64* rghRows,
    unsigned long eReason, unsigned long eNotifyPhase, int fIsRecoverable) {
    (void)pRowset; (void)fIsRecoverable;
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x178);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    int dscState = 0;
    switch (eNotifyPhase) {
    case 0: dscState = 1; break;
    case 1: dscState = 4; break;
    case 2: dscState = 6; break;
    case 3: dscState = 5; break;
    case 4: dscState = 7; break;
    default: break;
    }
    int dscReason = 0;
    switch (eReason) {
    case 4:  dscReason = 7; break;
    case 6:  dscReason = 3; break;
    case 8:  dscReason = 5; break;
    case 13: dscReason = 2; break;
    default: break;
    }
    SiteDscView* pDsc = static_cast<SiteDscView*>(SiteDataSourceControl(pSite));
    if (!pDsc) return S_OK;
    if (dscReason == 2 && dscState == 4) {
        DscUpdateCursor(pDsc);
    } else {
        if ((dscReason == 7 && cRows == 1) || (dscState == 6 && dscReason == 5)) {
            DscUpdateCursor(pDsc);
            *reinterpret_cast<unsigned __int64*>(pDsc->m_pRowset + kRowsetHRowOffset) = rghRows[0];
            const DWORD saved = pDsc->m_bUpdateInProgress;
            pDsc->m_bUpdateInProgress = 1;
            impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(pDsc);
            pDsc->m_bUpdateInProgress = saved;
            if (saved == 0) DscUpdateControls(pDsc);
        }
        if (dscReason == 0) return S_OK;
    }
    SiteEvent event = MakeSiteEvent(3, -1);
    event.m_nDSCState = dscState;
    event.m_nDSCReason = dscReason;
    impl__OnEvent_COleControlSite__IEAAHPEAUAFX_EVENT___Z(pSite, &event);
    return event.m_hResult < 0 ? event.m_hResult : S_OK;
}

// COleControlSite::XRowsetNotify::OnRowsetChange() — mfc140u 0x240c60.  Under
// the module-state scope it does nothing unless eReason == 0xe
// (DBREASON_ROWSET_CHANGED) and the site has a CDataSourceControl (+0xd8);
// then it throws AfxThrowInvalidArgException unless both dsc->m_pRowset (+0xa0)
// and dsc->m_pDynamicAccessor (+0xa8) exist, and rebinds the ATL
// CDynamicAccessor in place: the unexported ATL helpers at 0x2386ec and
// 0x23892c (column-info fetch and bind), CoTaskMemFree/free of the accessor's
// +0x28/+0x30/+0x10/+0x20 buffers, and a CoTaskMemAlloc'd, zeroed 0x40-byte-
// per-column buffer at dsc+0x28 when the column count (dsc+0x20) changed.
// Always returns S_OK.
// STUB: needs an ATL OLE DB CDynamicAccessor implementation (0x2386ec/0x23892c);
// OpenMFC's CDataSourceControl never builds m_pRowset/m_pDynamicAccessor.
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
// The mfc140u lookup has no mapped RVA. Its ANSI-twin entry RVA 0x23a9a0
// (mfc140) first enters AFX_MAINTAIN_STATE2 using the module state at site+0x38,
// then subtracts 0x140, validates m_pCtrlCont, calls the container's vtable
// slot at byte offset 0xd0 with the site, and returns S_OK. OpenMFC dispatches
// to the verified COleControlContainer thunk instead of the retail slot; when
// no container exists it raises the same invalid-argument exception.
extern "C" long MS_ABI impl__OnUIActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (!pSite->m_pCtrlCont) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_UNEXPECTED;
    }
    impl__OnUIActivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(
        pSite->m_pCtrlCont, pSite);
    return S_OK;
}

// COleControlSite::XOleIPSite::OnUIDeactivate() — mfc140u 0x23c4b0, no
// module-state scope: site = this-0x140; m_pCtrlCont(+0x40)->OnUIDeactivate(site)
// through container vtable byte 0xd8 (no null check); return S_OK.  fUndoable
// is ignored.  OpenMFC dispatches to the container's thunk, as OnUIActivate does.
extern "C" long MS_ABI impl__OnUIDeactivate_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fUndoable) {
    (void)fUndoable;
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    impl__OnUIDeactivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(pSite->m_pCtrlCont, pSite);
    return S_OK;
}

// COleControlSite::XOleIPSite::ReleaseDC() — mfc140u 0x23cad0.  The caller's
// HDC is ignored; the site's cached CDC (retail site+0x130, part-0x10) is
// released against the container window and the cache cleared:
//   ::ReleaseDC(m_pCtrlCont->m_pWnd->m_hWnd, cachedDC->m_hDC);  m_pDC = NULL;
//   return S_OK;
// The cache is OleControlSiteState::dc, filled by XOleIPSite::GetDC.  (The
// RetailTailStorage member named m_pDC sits at +0x128, which is not the offset
// these two methods use, so it is not touched.)  Retail dereferences a null
// cache; that case only clears it here.
extern "C" long MS_ABI impl__ReleaseDC_XOleIPSite_COleControlSite__UEAAJPEAUHDC_____Z(
    void* pThisItf, HDC hDC) {
    (void)hDC;
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    OleControlSiteState* state = GetOleControlSiteState(pSite, true);
    if (state->dc) ::ReleaseDC(SiteContainerHwnd(pSite), state->dc->m_hDC);
    state->dc = nullptr;
    return S_OK;
}

// Symbol: ?RequestNewObjectLayout@XOleClientSite@COleControlSite@@UEAAJXZ
// No Unicode mapped RVA is available. In the ANSI twin, XOleClientSite
// vftable RVA 0x3290f8 slot 8 points to folded body RVA 0xf540, which returns
// E_NOTIMPL. The existing constant body remains a metric stub.
extern "C" long MS_ABI impl__RequestNewObjectLayout_XOleClientSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;
}

// Symbol: ?RequestUIActivate@XOleIPSite@COleControlSite@@UEAAJXZ
// The mfc140u lookup has no mapped RVA. The ANSI-twin entry RVA 0x23ace0
// (mfc140) reads the owning site's m_pModuleState at site+0x38 through the
// XOleIPSite part, enters AFX_MAINTAIN_STATE2, and returns S_OK. The same
// existing state thunks reproduce that scope here.
extern "C" long MS_ABI impl__RequestUIActivate_XOleIPSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    return S_OK;
}

// Symbol: ?SaveObject@XOleClientSite@COleControlSite@@UEAAJXZ
// No Unicode mapped RVA is available. In the ANSI twin, XOleClientSite
// vftable RVA 0x3290f8 slot 3 points to folded body RVA 0xf540, disassembled
// as `mov $0x80004001,%eax; ret` (E_NOTIMPL). The constant body is retained
// as a metric stub, not reported as an implementation.
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

// COleControlSite::XOleIPSite::ScrollRect() — the folded body at mfc140u
// 0x23c710 (see AdjustRect): module-state scope, E_NOTIMPL, no argument read.
extern "C" long MS_ABI impl__ScrollRect_XOleIPSite_COleControlSite__UEAAJHHPEBUtagRECT__0_Z(
    void* pThisItf, int dx, int dy, const RECT* prcScroll, const RECT* prcClip) {
    (void)dx; (void)dy; (void)prcScroll; (void)prcClip;
    SiteModuleStateGuard stateGuard(SiteFromPart(pThisItf, 0x140)->m_pModuleState);
    return E_NOTIMPL;
}

// COleControlSite::XOleIPSite::SetCapture() — mfc140u 0x23cb30:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   if (fCapture) { CWnd::FromHandle(::SetCapture(m_pCtrlCont->m_pWnd->m_hWnd));
//                   m_pCtrlCont->m_pSiteCapture(+0xd0) = site; }
//   else          { m_pCtrlCont->m_pSiteCapture = NULL; ::ReleaseCapture(); }
//   return S_OK;
// The container's capture site is the OccContainerSiteState entry.
extern "C" long MS_ABI impl__SetCapture_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fCapture) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (fCapture) {
        impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(SiteContainerHwnd(pSite)));
        SetOccContainerCaptureSite(pSite->m_pCtrlCont, pSite);
    } else {
        SetOccContainerCaptureSite(pSite->m_pCtrlCont, nullptr);
        ::ReleaseCapture();
    }
    return S_OK;
}

// COleControlSite::XOleIPSite::SetFocus() — mfc140u 0x23cbb0:
//   AFX_MAINTAIN_STATE2(site->m_pModuleState);
//   if (fFocus) { m_pCtrlCont->m_pSiteFocus(+0xd8) = site;
//                 CWnd::SetFocus(m_pCtrlCont->m_pWnd); }        // 0x2a9b60
//   else        m_pCtrlCont->m_pSiteFocus = NULL;
//   return S_OK;
// The focus site is modelled by core/ole/COleControlContainer.cpp
// (SetOccContainerFocusSite).
extern "C" long MS_ABI impl__SetFocus_XOleIPSite_COleControlSite__UEAAJH_Z(
    void* pThisItf, int fFocus) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x140);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (fFocus) {
        SetOccContainerFocusSite(pSite->m_pCtrlCont, pSite);
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(pSite->m_pCtrlCont->m_pWnd);
    } else {
        SetOccContainerFocusSite(pSite->m_pCtrlCont, nullptr);
    }
    return S_OK;
}

// Symbol: ?ShowObject@XOleClientSite@COleControlSite@@UEAAJXZ
// The mfc140u lookup has no mapped RVA. The ANSI-twin entry RVA 0x23a900
// (mfc140) enters AFX_MAINTAIN_STATE2 using the module state at site+0x38,
// subtracts 0x138 to recover the site, calls AttachWindow (entry RVA 0x239a30,
// mfc140), and returns S_OK. The site overlay reserves the interface-part
// offset, so this calls the local AttachWindow thunk. That thunk captures the
// in-place HWND but does not attach a missing CWnd wrapper or set its absent
// m_pCtrlSite backlink.
extern "C" long MS_ABI impl__ShowObject_XOleClientSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pThisItf) - 0x138);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    if (!pSite->m_lpInPlaceObject) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_UNEXPECTED;
    }
    impl__AttachWindow_COleControlSite__QEAAXXZ(pSite);
    return S_OK;
}

// Symbol: ?ShowPropertyFrame@XOleControlSite@COleControlSite@@UEAAJXZ
// No Unicode mapped RVA is available. In the ANSI twin, XOleControlSite
// vftable RVA 0x3290a0 slot 9 points to folded body RVA 0xf540, which returns
// E_NOTIMPL. The current bare constant is retained as a metric stub; retail
// itself does not show a property frame through this interface method.
extern "C" long MS_ABI impl__ShowPropertyFrame_XOleControlSite_COleControlSite__UEAAJXZ(void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;
}

// COleControlSite::XNotifyDBEvents::SyncAfter() — mfc140u 0x2406a0:
//   scope; hr = FireEvent(..., dscSyncAfter (6)); if (FAILED(hr)) return hr;
//   dsc = m_pDataSourceControl(+0xd8);
//   if (dsc && !dsc->m_bUpdateInProgress(+0x88)) {
//       reason = rgReasons[0].dwReason;
//       if (reason != 0x1e && reason != 0x1f && (dwEventWhat & 3) &&
//           reason != 2 && reason != 3) {
//           dsc->m_bUpdateInProgress = TRUE;  dsc->GetBoundClientRow();
//           dsc->m_bUpdateInProgress = FALSE; dsc->UpdateControls();   // slot 5
//       }
//   }
//   return S_OK;
extern "C" long MS_ABI impl__SyncAfter_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x170);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 6);
    if (hr < 0) return hr;
    SiteDscView* pDsc = static_cast<SiteDscView*>(SiteDataSourceControl(pSite));
    if (pDsc && pDsc->m_bUpdateInProgress == 0) {
        const DWORD reason = *static_cast<const DWORD*>(rgReasons);
        if (reason != 0x1e && reason != 0x1f && (dwEventWhat & 3) && reason != 2 && reason != 3) {
            pDsc->m_bUpdateInProgress = 1;
            impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(pDsc);
            pDsc->m_bUpdateInProgress = 0;
            DscUpdateControls(pDsc);
        }
    }
    return S_OK;
}

// COleControlSite::XNotifyDBEvents::SyncBefore() — mfc140u 0x240530:
//   scope; hr = FireEvent(..., dscSyncBefore (3)); if (FAILED(hr)) return hr;
//   reason = rgReasons[0].dwReason;
//   if ((reason == 2 || reason == 3 || reason == 0x14) && m_pDataSourceControl
//       && (dwEventWhat & 0x80))          // `test %sil,%sil; jns`
//       m_pDataSourceControl->UpdateCursor();            // slot 6, result ignored
//   return S_OK;
extern "C" long MS_ABI impl__SyncBefore_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThisItf, unsigned long dwEventWhat, unsigned long cReasons, void* rgReasons) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x170);
    SiteModuleStateGuard stateGuard(pSite->m_pModuleState);
    long hr = impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
        pThisItf, dwEventWhat, cReasons, rgReasons, 3);
    if (hr < 0) return hr;
    const DWORD reason = *static_cast<const DWORD*>(rgReasons);
    if (reason == 2 || reason == 3 || reason == 0x14) {
        void* pDsc = SiteDataSourceControl(pSite);
        if (pDsc && (dwEventWhat & 0x80)) DscUpdateCursor(pDsc);
    }
    return S_OK;
}

// COleControlSite::XOleControlSite::TransformCoords() — mfc140u 0x23cc70, no
// module-state scope:
//   CDC dc; dc.Attach(::GetDC(site->m_hWnd(+0x48)));      // part 0x148 - 0x100
//   if (dwFlags & XFORMCOORDS_HIMETRICTOCONTAINER) {
//       CSize sz(*pPtlHimetric); dc.HIMETRICtoDP(&sz);
//       if (dwFlags & XFORMCOORDS_SIZE)  ptf = ((float)labs(sz.cx), (float)labs(sz.cy));
//       else if (dwFlags & XFORMCOORDS_POSITION) ptf = ((float)sz.cx, (float)sz.cy);
//       else hr = E_INVALIDARG;
//   } else if (dwFlags & XFORMCOORDS_CONTAINERTOHIMETRIC) {
//       CSize sz((int)ptf.x, (int)ptf.y);                   // cvttss2si
//       dc.DPtoHIMETRIC(&sz);
//       if (dwFlags & XFORMCOORDS_SIZE)  ptl = (labs(sz.cx), labs(sz.cy));
//       else if (dwFlags & XFORMCOORDS_POSITION) ptl = sz;
//       else hr = E_INVALIDARG;
//   } else hr = E_INVALIDARG;
//   ~CDC: if (m_hDC) ::DeleteDC(Detach());   return hr;
// Retail really frees the ::GetDC handle with DeleteDC (the inlined ~CDC);
// that is reproduced.
extern "C" long MS_ABI impl__TransformCoords_XOleControlSite_COleControlSite__UEAAJPEAU_POINTL__PEAUtagPOINTF__K_Z(
    void* pThisItf, void* pPtlHimetric, void* pPtfContainer, unsigned long dwFlags) {
    COleControlSite* pSite = SiteFromPart(pThisItf, 0x148);
    POINTL* ptl = static_cast<POINTL*>(pPtlHimetric);
    POINTF* ptf = static_cast<POINTF*>(pPtfContainer);
    HRESULT hr = S_OK;
    CDC dc;
    impl__Attach_CDC__QEAAHPEAUHDC_____Z(&dc, ::GetDC(pSite->m_hWnd));
    if (dwFlags & XFORMCOORDS_HIMETRICTOCONTAINER) {
        SIZE sz = { ptl->x, ptl->y };
        impl__HIMETRICtoDP_CDC__QEBAXPEAUtagSIZE___Z(&dc, &sz);
        if (dwFlags & XFORMCOORDS_SIZE) {
            ptf->x = static_cast<float>(labs(sz.cx));
            ptf->y = static_cast<float>(labs(sz.cy));
        } else if (dwFlags & XFORMCOORDS_POSITION) {
            ptf->x = static_cast<float>(sz.cx);
            ptf->y = static_cast<float>(sz.cy);
        } else {
            hr = E_INVALIDARG;
        }
    } else if (dwFlags & XFORMCOORDS_CONTAINERTOHIMETRIC) {
        SIZE sz = { static_cast<LONG>(ptf->x), static_cast<LONG>(ptf->y) };
        impl__DPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(&dc, &sz);
        if (dwFlags & XFORMCOORDS_SIZE) {
            ptl->x = labs(sz.cx);
            ptl->y = labs(sz.cy);
        } else if (dwFlags & XFORMCOORDS_POSITION) {
            ptl->x = sz.cx;
            ptl->y = sz.cy;
        } else {
            hr = E_INVALIDARG;
        }
    } else {
        hr = E_INVALIDARG;
    }
    return hr;
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
