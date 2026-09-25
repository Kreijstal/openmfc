// CDHtmlElementEventSink — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CDHtmlElementEventSink (afxdhtml.h) is the IDispatch sink CDHtmlDialog::
// ConnectDHtmlElementEvents (RVA 0x213b20, mfc140u) creates for each event-map entry of type
// DHTMLEVENTMAPENTRY_ELEMENT (== 3; the DHTML_EVENT_ELEMENT macro, e.g. DHTML_EVENT_ONRESIZE --
// NOT DHTML_EVENT_ONCLICK, which expands to a DHTMLEVENTMAPENTRY_NAME entry) and advises on
// the element with IID_IDispatch.  Every body below is transcribed from the retail code,
// disassembled with `disas.py --u` (function bodies are byte-identical between mfc140.dll and
// mfc140u.dll; every RVA quoted here is mfc140u's).  The mfc140u_rva_symbols.json map has no
// name for AddRef, Release, QueryInterface, GetIDsOfNames, GetTypeInfo, GetTypeInfoCount or
// UnAdvise.  AddRef/Release (0x3a60), GetIDsOfNames (0xf4d0) and GetTypeInfo/GetTypeInfoCount
// (0x20f860/0x20f850) are bodies the linker folded with other exports; QueryInterface
// (0x2109b0) and UnAdvise (0x210d30) are NOT folded (one export each) -- the map simply lacks
// them.  All RVAs were read from the mfc140u export directory (ures.py) and agree with the
// vftable dump below.  Deviations from retail are marked DEVIATION.
//
// ---- layout, pinned from the retail constructor / deleting destructor -------------------
// No OpenMFC header declares this class, so the layout is pinned here (S_DHEES).
//   +0x00 vfptr   +0x08 m_pHandler (CDHtmlEventSink*)   +0x10 m_spunkElem (CComPtr<IUnknown>)
//   +0x18 m_dwCookie (DWORD)                            sizeof 0x20
// The constructor (RVA 0x210960) writes +0x08/+0x10/+0x18; the scalar deleting destructor
// (RVA 0x215560) passes 0x20 to the sized operator delete.
//
// ---- the vftable -------------------------------------------------------------------------
// Retail vftable RVA 0x3274a8 (mfc140u; the address the constructor installs), dumped with
// vtdump_u.py:
//   0 QueryInterface   0x2109b0      1 AddRef / 2 Release   0x3a60 (`mov $1,%eax; ret`)
//   3 GetTypeInfoCount 0x20f850      4 GetTypeInfo  0x20f860   (both shared with
//                                           CDHtmlControlSink and CDHtmlEventSink)
//   5 GetIDsOfNames    0xf4d0 (`mov $E_NOTIMPL,%eax; ret`)
//   6 Invoke           0x210a30      7 scalar deleting destructor 0x215560 (not exported)
// There is no exported ??_7CDHtmlElementEventSink@@6B@, so a client that does
// `new CDHtmlElementEventSink(...)` relies on the exported constructor to install it.  The
// constructor below installs g_DHEES_Vtbl, an MSVC-layout table of this file's thunks (the
// mechanism core/view/CDHtmlControlSink.cpp uses).  The object is handed to COM
// (IConnectionPoint::Advise takes it as an IUnknown*; its IDispatch vfptr is at +0), so the
// table must be MSVC-layout.
//
// ---- the event handler -------------------------------------------------------------------
// m_pHandler is a CDHtmlEventSink -- in practice the CDHtmlEventSink subobject of a client
// CDHtmlDialog, i.e. a client MSVC object.  Invoke calls two of its virtuals, read off the
// call sites: GetDHtmlEventMap, slot 7 (+0x38; CDHtmlSinkHandler's last pure virtual), and
// GetDHtmlDocument, slot 9 (+0x48; CDHtmlEventSink adds DHtmlEventHook at slot 8).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <ocidl.h>
#include <oleauto.h>

// ---------------------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition; parameter lists follow the
// mangled names.
// ---------------------------------------------------------------------------------------
//   detail/MfcExceptionsSupport.cpp:603 : ?AfxThrowMemoryException@@YAXXZ
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
//   detail/MemcoreSupport.cpp:12 : ??3@YAXPEAX@Z (operator delete -> free)
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* p);

// Forward declarations of this file's own thunks placed in the vftable before their
// definitions.  Signatures derived from the mangled names.
extern "C" long MS_ABI impl__QueryInterface_CDHtmlElementEventSink__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__AddRef_CDHtmlElementEventSink__UEAAKXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__Release_CDHtmlElementEventSink__UEAAKXZ(void* pThis);
extern "C" long MS_ABI impl__GetTypeInfoCount_CDHtmlElementEventSink__UEAAJPEAI_Z(void* pThis, unsigned int* pctinfo);
extern "C" long MS_ABI impl__GetTypeInfo_CDHtmlElementEventSink__UEAAJIKPEAPEAUITypeInfo___Z(void* pThis, unsigned int iTInfo, unsigned long lcid, ITypeInfo** ppTInfo);
extern "C" long MS_ABI impl__GetIDsOfNames_CDHtmlElementEventSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid, long* rgDispId);
extern "C" long MS_ABI impl__Invoke_CDHtmlElementEventSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(void* pThis, long dispIdMember, const GUID* riid, unsigned long lcid, unsigned short wFlags, DISPPARAMS* pdispparams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr);

namespace {

struct S_DHEES {
    void* vfptr;          // +0x00
    void* m_pHandler;     // +0x08  CDHtmlEventSink*
    IUnknown* m_spunkElem; // +0x10  CComPtr<IUnknown>
    DWORD m_dwCookie;     // +0x18
};
static_assert(offsetof(S_DHEES, m_pHandler) == 0x08, "CDHtmlElementEventSink::m_pHandler");
static_assert(offsetof(S_DHEES, m_spunkElem) == 0x10, "CDHtmlElementEventSink::m_spunkElem");
static_assert(offsetof(S_DHEES, m_dwCookie) == 0x18, "CDHtmlElementEventSink::m_dwCookie");
static_assert(sizeof(S_DHEES) == 0x20, "sizeof(CDHtmlElementEventSink): 0x20 per the retail deleting dtor");

// DHtmlEventMapEntry (afxdhtml.h): nType, dispId, szName, then a 16-byte pointer-to-member
// (CDHtmlSinkHandler has two bases, so its PMFs use the multiple-inheritance form: code
// pointer, then a 32-bit this-adjustment).  Retail Invoke walks the map with a 0x20 stride,
// reads +0 / +4 / +8, calls through the pointer at +0x10 and adds the sign-extended int at
// +0x18 to m_pHandler.
struct DHEES_EventMapEntry {
    int nType;
    long dispId;
    const wchar_t* szName;
    void* pfnCode;
    int nThisAdjust;
    int nPad;
};
static_assert(offsetof(DHEES_EventMapEntry, dispId) == 0x04, "DHtmlEventMapEntry::dispId");
static_assert(offsetof(DHEES_EventMapEntry, szName) == 0x08, "DHtmlEventMapEntry::szName");
static_assert(offsetof(DHEES_EventMapEntry, pfnCode) == 0x10, "DHtmlEventMapEntry::pfnEventFunc (code)");
static_assert(offsetof(DHEES_EventMapEntry, nThisAdjust) == 0x18, "DHtmlEventMapEntry::pfnEventFunc (adjustor)");
static_assert(sizeof(DHEES_EventMapEntry) == 0x20, "DHtmlEventMapEntry size");
constexpr int kDHTMLEVENTMAPENTRY_ELEMENT = 3;  // afxdhtml.h enum DHtmlEventMapEntryType
constexpr int kDHTMLEVENTMAPENTRY_END = 5;

// GUID constants, read from the retail .rdata the code references (mfc140u RVAs):
//   0x34c838 IID_IUnknown, 0x34c848 IID_IDispatch, 0x34c798 IID_IHTMLElement,
//   0x34c808 DIID_HTMLElementEvents, 0x34c858 IID_IConnectionPointContainer
// (names matched against the Windows SDK 10.0.26100.0 MsHTML.h / ocidl.h).
const GUID kIID_IUnknown = {0x00000000, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
const GUID kIID_IDispatch = {0x00020400, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
const GUID kIID_IHTMLElement = {0x3050F1FF, 0x98B5, 0x11CF, {0xBB, 0x82, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x0B}};
const GUID kDIID_HTMLElementEvents = {0x3050F33C, 0x98B5, 0x11CF, {0xBB, 0x82, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x0B}};
const GUID kIID_IConnectionPointContainer = {0xB196B284, 0xBAB4, 0x101A, {0xB6, 0x9C, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07}};

// MSHTML vtable slots the retail Invoke calls (byte offset / 8), cross-checked against the
// C vtable structs in the SDK MsHTML.h:
constexpr int kSlot_IHTMLElement_get_id = 13;              // +0x68
constexpr int kSlot_IHTMLDocument2_get_parentWindow = 108; // +0x360
constexpr int kSlot_IHTMLWindow2_get_event = 53;           // +0x1a8
constexpr int kSlot_IHTMLEventObj_get_srcElement = 7;      // +0x38
// CDHtmlEventSink virtuals (see the file header).
constexpr int kSlot_GetDHtmlEventMap = 7;                  // +0x38
constexpr int kSlot_GetDHtmlDocument = 9;                  // +0x48

inline S_DHEES* Self(void* p) { return static_cast<S_DHEES*>(p); }

inline void* VSlot(void* pObj, int slot) { return (*static_cast<void***>(pObj))[slot]; }

// Every MSHTML getter used here has the shape HRESULT (IFace*, T** / BSTR*).
HRESULT CallGetter(void* pObj, int slot, void* pOut) {
    using PFN = HRESULT(STDMETHODCALLTYPE*)(void*, void*);
    return reinterpret_cast<PFN>(VSlot(pObj, slot))(pObj, pOut);
}

const DHEES_EventMapEntry* GetHandlerEventMap(void* pHandler) {
    using PFN = const DHEES_EventMapEntry* (MS_ABI*)(void*);
    return reinterpret_cast<PFN>(VSlot(pHandler, kSlot_GetDHtmlEventMap))(pHandler);
}

inline void ReleaseIf(IUnknown* p) {
    if (p != nullptr)
        p->Release();
}

// Scalar deleting destructor, vftable slot 7.  Retail RVA 0x215560 (mfc140u, not exported):
// re-install the vftable, release m_spunkElem if non-null (IUnknown slot 2, +0x10); if
// (flags & 1) operator delete(this, 0x20) -- the sized-delete helper at 0x2b77b0 tail-jumps
// to 0x27c0, the shared free-forwarding operator delete body.
void* MS_ABI DHEES_ScalarDeletingDtor(void* pThis, unsigned int flags);

void* const g_DHEES_Vtbl[8] = {
    reinterpret_cast<void*>(&impl__QueryInterface_CDHtmlElementEventSink__UEAAJAEBU_GUID__PEAPEAX_Z),
    reinterpret_cast<void*>(&impl__AddRef_CDHtmlElementEventSink__UEAAKXZ),
    reinterpret_cast<void*>(&impl__Release_CDHtmlElementEventSink__UEAAKXZ),
    reinterpret_cast<void*>(&impl__GetTypeInfoCount_CDHtmlElementEventSink__UEAAJPEAI_Z),
    reinterpret_cast<void*>(&impl__GetTypeInfo_CDHtmlElementEventSink__UEAAJIKPEAPEAUITypeInfo___Z),
    reinterpret_cast<void*>(&impl__GetIDsOfNames_CDHtmlElementEventSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z),
    reinterpret_cast<void*>(&impl__Invoke_CDHtmlElementEventSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z),
    reinterpret_cast<void*>(&DHEES_ScalarDeletingDtor),
};

void* MS_ABI DHEES_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    S_DHEES* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DHEES_Vtbl);
    ReleaseIf(s->m_spunkElem);
    if (flags & 1)
        impl___3_YAXPEAX_Z(pThis);
    return pThis;
}

// AtlAdvise, retail RVA 0x21579c (mfc140u, not exported): E_INVALIDARG for a null object,
// else QI(IConnectionPointContainer) -> FindConnectionPoint (slot 4) ->
// IConnectionPoint::Advise (slot 5), releasing both interfaces; returns the last HRESULT.
HRESULT DHEES_AtlAdvise(IUnknown* pUnkCP, IUnknown* pUnk, const IID& iid, DWORD* pdw) {
    if (pUnkCP == nullptr)
        return E_INVALIDARG;
    IConnectionPointContainer* pCPC = nullptr;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pUnkCP->QueryInterface(kIID_IConnectionPointContainer, reinterpret_cast<void**>(&pCPC));
    if (SUCCEEDED(hr)) {
        hr = pCPC->FindConnectionPoint(iid, &pCP);
        if (SUCCEEDED(hr))
            hr = pCP->Advise(pUnk, pdw);
    }
    ReleaseIf(pCP);
    ReleaseIf(pCPC);
    return hr;
}

// AtlUnadvise, retail RVA 0x2156d0 (mfc140u, not exported): the same shape, ending in
// IConnectionPoint::Unadvise (slot 6) with the cookie by value.
HRESULT DHEES_AtlUnadvise(IUnknown* pUnkCP, const IID& iid, DWORD dw) {
    if (pUnkCP == nullptr)
        return E_INVALIDARG;
    IConnectionPointContainer* pCPC = nullptr;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pUnkCP->QueryInterface(kIID_IConnectionPointContainer, reinterpret_cast<void**>(&pCPC));
    if (SUCCEEDED(hr)) {
        hr = pCPC->FindConnectionPoint(iid, &pCP);
        if (SUCCEEDED(hr))
            hr = pCP->Unadvise(dw);
    }
    ReleaseIf(pCP);
    ReleaseIf(pCPC);
    return hr;
}

}  // namespace

// CDHtmlElementEventSink::CDHtmlElementEventSink(CDHtmlEventSink* pHandler, IDispatch* pdisp)
// -- retail RVA 0x210960 (mfc140u).  Transcribed:
//   vfptr = vftable; m_spunkElem = NULL; m_pHandler = pHandler;
//   pdisp->QueryInterface(IID_IUnknown, (void**)&m_spunkElem);   // slot 0; no null check on
//                                                                 // pdisp, HRESULT dropped
//   m_dwCookie = 0; return this;
// Symbol: ??0CDHtmlElementEventSink@@QEAA@PEAVCDHtmlEventSink@@PEAUIDispatch@@@Z
extern "C" void* MS_ABI impl___0CDHtmlElementEventSink__QEAA_PEAVCDHtmlEventSink__PEAUIDispatch___Z(
    void* pThis, void* pEventSink, void* pDispatch) {
    S_DHEES* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DHEES_Vtbl);
    s->m_spunkElem = nullptr;
    s->m_pHandler = pEventSink;
    static_cast<IDispatch*>(pDispatch)->QueryInterface(kIID_IUnknown,
                                                       reinterpret_cast<void**>(&s->m_spunkElem));
    s->m_dwCookie = 0;
    return pThis;
}

// CDHtmlElementEventSink::AddRef -- export RVA 0x3a60 (mfc140u), a `mov $1,%eax; ret` body
// the linker shares with 100 other exports.  No reference count: the sink's lifetime is
// owned by CDHtmlDialog -- DisconnectDHtmlElementEvents (RVA 0x214120, mfc140u) unadvises
// each element sink and calls its vftable slot 7 (deleting destructor) with flag 1.
// Symbol: ?AddRef@CDHtmlElementEventSink@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CDHtmlElementEventSink__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// CDHtmlElementEventSink::Advise(LPUNKNOWN pUnkObj, REFIID iid) -- retail RVA 0x210d10
// (mfc140u): a register shuffle that tail-jumps to AtlAdvise (0x21579c) as
//   AtlAdvise(pUnkObj, (IUnknown*)this, iid, &m_dwCookie).
// Symbol: ?Advise@CDHtmlElementEventSink@@QEAAJPEAUIUnknown@@AEBU_GUID@@@Z
extern "C" long MS_ABI impl__Advise_CDHtmlElementEventSink__QEAAJPEAUIUnknown__AEBU_GUID___Z(
    void* pThis, IUnknown* pUnkObj, const GUID* iid) {
    return DHEES_AtlAdvise(pUnkObj, static_cast<IUnknown*>(pThis), *iid, &Self(pThis)->m_dwCookie);
}

// CDHtmlElementEventSink::GetIDsOfNames -- export RVA 0xf4d0 (mfc140u), a shared
// `mov $0x80004001,%eax; ret` body: E_NOTIMPL, no argument touched.
// Symbol: ?GetIDsOfNames@CDHtmlElementEventSink@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_CDHtmlElementEventSink__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid,
    long* rgDispId) {
    (void)pThis; (void)riid; (void)rgszNames; (void)cNames; (void)lcid; (void)rgDispId;
    return E_NOTIMPL;
}

// CDHtmlElementEventSink::GetTypeInfo -- export RVA 0x20f860 (mfc140u), the body it shares
// with CDHtmlControlSink::GetTypeInfo: *ppTInfo = NULL (no null check); return E_NOTIMPL.
// Symbol: ?GetTypeInfo@CDHtmlElementEventSink@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_CDHtmlElementEventSink__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int iTInfo, unsigned long lcid, ITypeInfo** ppTInfo) {
    (void)pThis; (void)iTInfo; (void)lcid;
    *ppTInfo = nullptr;
    return E_NOTIMPL;
}

// CDHtmlElementEventSink::GetTypeInfoCount -- export RVA 0x20f850 (mfc140u), the body it
// shares with CDHtmlControlSink::GetTypeInfoCount: *pctinfo = 0 (no null check); return
// E_NOTIMPL.
// Symbol: ?GetTypeInfoCount@CDHtmlElementEventSink@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_CDHtmlElementEventSink__UEAAJPEAI_Z(
    void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    *pctinfo = 0;
    return E_NOTIMPL;
}

// CDHtmlElementEventSink::Invoke -- retail RVA 0x210a30 (mfc140u).  Transcribed:
//   CComPtr<IHTMLElement> spElem; CComBSTR bstrId; CComPtr<IHTMLDocument2> spDoc;
//   CComPtr<IHTMLWindow2> spWindow; CComPtr<IHTMLEventObj> spEventObj;
//   CComPtr<IHTMLElement> spSrcElem;
//   if (pVarResult) VariantInit(pVarResult);                          // OLEAUT32 #8
//   hr = m_spunkElem->QueryInterface(IID_IHTMLElement, &spElem);      // no null check on
//                                                                     // m_spunkElem
//   if (spElem) {                          // the test is on spElem, not on hr
//     if (FAILED(hr = spElem->get_id(&bstrId)))                        goto done;
//     if (FAILED(hr = m_pHandler->GetDHtmlDocument(&spDoc)))           goto done;  // slot 9
//     if (FAILED(hr = spDoc->get_parentWindow(&spWindow)))             goto done;
//     if (FAILED(hr = spWindow->get_event(&spEventObj)))               goto done;
//     if (FAILED(hr = spEventObj->get_srcElement(&spSrcElem)))         goto done;
//     pMap = m_pHandler->GetDHtmlEventMap();                           // slot 7
//     hr = DISP_E_MEMBERNOTFOUND;
//     for (i = 0; pMap[i].nType != DHTMLEVENTMAPENTRY_END; i++)
//       if (pMap[i].nType == DHTMLEVENTMAPENTRY_ELEMENT && pMap[i].dispId == dispIdMember
//           && pMap[i].szName != NULL
//           && wcscmp(CComBSTR(pMap[i].szName), bstrId) == 0) {        // SysAllocString #2,
//                                                                      // SysFreeString #6;
//                                                                      // alloc failure ->
//                                                                      // AfxThrowMemoryException
//         pMap = m_pHandler->GetDHtmlEventMap();                       // fetched again
//         HRESULT hrEvt = (m_pHandler->*pMap[i].pfnEventFunc)(spSrcElem);
//         if (pVarResult) { pVarResult->vt = VT_BOOL;
//                           pVarResult->boolVal = hrEvt == 0 ? VARIANT_TRUE : VARIANT_FALSE; }
//         hr = S_OK; break;
//       }
//   }
// done:
//   SysFreeString(bstrId); release spEventObj, spSrcElem, spElem, spDoc, spWindow (that
//   order); return hr;
// No null checks on spDoc / spWindow / spEventObj after a successful getter, and none on
// m_pHandler.  riid, lcid, wFlags, pdispparams, pExcepInfo and puArgErr are never read.
// The IAT slots were resolved with iatu.py (VariantInit, SysAllocString, SysFreeString are
// OLEAUT32 ordinals 8 / 2 / 6; wcscmp from api-ms-win-crt-string); the throw helper at
// 0x333c calls AfxThrowMemoryException (0x2276c0) for E_OUTOFMEMORY.
// DEVIATION: retail passes bstrId straight to wcscmp; a NULL BSTR (an element with no id)
// would fault there.  Here a NULL bstrId compares as the empty string, which is what a NULL
// BSTR means under OLE Automation.
// DEVIATION: on the SysAllocString failure path retail's unwinder releases the locals as the
// exception propagates; here they are released explicitly before AfxThrowMemoryException.
// Symbol: ?Invoke@CDHtmlElementEventSink@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_CDHtmlElementEventSink__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long dispIdMember, const GUID* riid, unsigned long lcid, unsigned short wFlags,
    DISPPARAMS* pdispparams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr) {
    (void)riid; (void)lcid; (void)wFlags; (void)pdispparams; (void)pExcepInfo; (void)puArgErr;
    S_DHEES* s = Self(pThis);

    IUnknown* spElem = nullptr;
    BSTR bstrId = nullptr;
    IUnknown* spDoc = nullptr;
    IUnknown* spWindow = nullptr;
    IUnknown* spEventObj = nullptr;
    IUnknown* spSrcElem = nullptr;

    auto cleanup = [&]() {
        SysFreeString(bstrId);
        ReleaseIf(spEventObj);
        ReleaseIf(spSrcElem);
        ReleaseIf(spElem);
        ReleaseIf(spDoc);
        ReleaseIf(spWindow);
    };

    if (pVarResult != nullptr)
        VariantInit(pVarResult);

    HRESULT hr = s->m_spunkElem->QueryInterface(kIID_IHTMLElement, reinterpret_cast<void**>(&spElem));
    if (spElem == nullptr) {
        cleanup();
        return hr;
    }
    hr = CallGetter(spElem, kSlot_IHTMLElement_get_id, &bstrId);
    if (FAILED(hr)) { cleanup(); return hr; }
    hr = CallGetter(s->m_pHandler, kSlot_GetDHtmlDocument, &spDoc);
    if (FAILED(hr)) { cleanup(); return hr; }
    hr = CallGetter(spDoc, kSlot_IHTMLDocument2_get_parentWindow, &spWindow);
    if (FAILED(hr)) { cleanup(); return hr; }
    hr = CallGetter(spWindow, kSlot_IHTMLWindow2_get_event, &spEventObj);
    if (FAILED(hr)) { cleanup(); return hr; }
    hr = CallGetter(spEventObj, kSlot_IHTMLEventObj_get_srcElement, &spSrcElem);
    if (FAILED(hr)) { cleanup(); return hr; }

    const DHEES_EventMapEntry* pMap = GetHandlerEventMap(s->m_pHandler);
    hr = DISP_E_MEMBERNOTFOUND;
    for (int i = 0; pMap[i].nType != kDHTMLEVENTMAPENTRY_END; i++) {
        if (pMap[i].nType != kDHTMLEVENTMAPENTRY_ELEMENT || pMap[i].dispId != dispIdMember)
            continue;
        if (pMap[i].szName == nullptr)
            continue;
        BSTR bstrName = SysAllocString(pMap[i].szName);
        if (bstrName == nullptr) {
            cleanup();
            impl__AfxThrowMemoryException__YAXXZ();
            return E_OUTOFMEMORY;  // not reached
        }
        const bool bMatch = std::wcscmp(bstrName, bstrId != nullptr ? bstrId : L"") == 0;
        SysFreeString(bstrName);
        if (!bMatch)
            continue;

        const DHEES_EventMapEntry* pMap2 = GetHandlerEventMap(s->m_pHandler);
        using PFN_Event = HRESULT (MS_ABI*)(void*, void*);
        void* pTarget = static_cast<char*>(s->m_pHandler) + pMap2[i].nThisAdjust;
        const HRESULT hrEvt = reinterpret_cast<PFN_Event>(pMap2[i].pfnCode)(pTarget, spSrcElem);
        if (pVarResult != nullptr) {
            pVarResult->vt = VT_BOOL;
            pVarResult->boolVal = hrEvt == 0 ? VARIANT_TRUE : VARIANT_FALSE;
        }
        hr = S_OK;
        break;
    }
    cleanup();
    return hr;
}

// CDHtmlElementEventSink::QueryInterface -- export RVA 0x2109b0 (mfc140u; vftable slot 0).
// ppvObj == NULL -> E_POINTER; *ppvObj = NULL; iid equal (memcmp, 16 bytes) to IID_IUnknown
// or IID_IDispatch -> *ppvObj = this, S_OK -- with NO AddRef (AddRef is a constant anyway);
// otherwise E_NOINTERFACE.
// Symbol: ?QueryInterface@CDHtmlElementEventSink@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CDHtmlElementEventSink__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* riid, void** ppvObj) {
    if (ppvObj == nullptr)
        return E_POINTER;
    *ppvObj = nullptr;
    if (std::memcmp(riid, &kIID_IUnknown, sizeof(GUID)) != 0 &&
        std::memcmp(riid, &kIID_IDispatch, sizeof(GUID)) != 0)
        return E_NOINTERFACE;
    *ppvObj = pThis;
    return S_OK;
}

// CDHtmlElementEventSink::Release -- see AddRef: the same shared `return 1` body (export RVA
// 0x3a60, mfc140u).
// Symbol: ?Release@CDHtmlElementEventSink@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CDHtmlElementEventSink__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// CDHtmlElementEventSink::UnAdvise(LPUNKNOWN pUnkObj, REFIID iid) -- export RVA 0x210d30
// (mfc140u): tail-jumps to AtlUnadvise (0x2156d0) as
//   AtlUnadvise(pUnkObj, DIID_HTMLElementEvents /* .rdata 0x34c808 */, m_dwCookie).
// The iid argument is NOT used: retail loads the constant DIID_HTMLElementEvents into the
// REFIID register and never reads the caller's second argument.  (CDHtmlDialog does not call
// this member: Connect/DisconnectDHtmlElementEvents call AtlAdvise/AtlUnadvise directly with
// IID_IDispatch, .rdata 0x34c848.)
// Symbol: ?UnAdvise@CDHtmlElementEventSink@@QEAAJPEAUIUnknown@@AEBU_GUID@@@Z
extern "C" long MS_ABI impl__UnAdvise_CDHtmlElementEventSink__QEAAJPEAUIUnknown__AEBU_GUID___Z(
    void* pThis, IUnknown* pUnkObj, const GUID* iid) {
    (void)iid;
    return DHEES_AtlUnadvise(pUnkObj, kDIID_HTMLElementEvents, Self(pThis)->m_dwCookie);
}
