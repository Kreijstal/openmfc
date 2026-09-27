// CHtmlEditCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Retail layout (mfc140u, read from the disassembly cited on each body below):
//   class CHtmlEditCtrl : public CWnd, public CHtmlEditCtrlBase<CHtmlEditCtrl>
//   (afxhtml.h:1563).  CHtmlEditCtrlBase is an empty mix-in placed at
//   +0xe8 == sizeof(CWnd): the event handler at RVA 0x27d800 (mfc140u) casts
//   it back with `lea 0xe8(%rcx); neg; sbb; and %rcx`, the null-preserving
//   base-to-derived adjustment, which is `this` for any non-null object.
//   CHtmlEditCtrl declares no data members of its own (afxhtml.h:1563-1592).
//
//   OpenMFC has no C++ CHtmlEditCtrl class (include/openmfc does not declare
//   it), so every body here takes `void* pThis`.
//
//   Retail CHtmlEditCtrl vftable: 0x180333b98 (mfc140u), stored by both the
//   constructor (RVA 0x27d760) and the destructor (RVA 0x27d7e0).  Slots read
//   out of that table for the Create transcription below:
//     slot 26 (+0x0d0) -> 0x28baf0  CWnd::DestroyWindow
//     slot 92 (+0x2e0) -> 0x27db70  CHtmlEditCtrl::GetStartDocument
//
// Own vptr.  ??0CWnd@@QEAA@XZ (core/window/CtorDtorPlacement.cpp) placement-
// news OpenMFC's CWnd, so an object built by the constructor here carries
// OpenMFC's mingw CWnd vtable; there is no MSVC-layout CHtmlEditCtrl vftable
// in this image.  Same convention as featurepack/controls/CMFCPreviewCtrlImpl.cpp:
// the constructor records that vptr (g_ownVptr) and the destructor stores it
// back first -- retail's own "vfptr = &vftable" store -- so that ??1CWnd@@'s
// C++ virtual-destructor call dispatches on OpenMFC's table and not on a
// client-derived class's MSVC vftable.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <exdisp.h>
#include <mshtml.h>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void*     MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                               // core/window/CtorDtorPlacement.cpp
extern "C" void      MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                               // core/window/CtorDtorPlacement.cpp
extern "C" IUnknown* MS_ABI impl__GetControlUnknown_CWnd__QEAAPEAUIUnknown__XZ(CWnd* pThis);  // core/window/Thunks.cpp
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ(); // core/runtime/CCmdTarget.cpp
extern "C" void      MS_ABI impl__AfxThrowMemoryException__YAXXZ();                            // detail/MfcExceptionsSupport.cpp

// Sibling defined below and called before its definition.
extern "C" int MS_ABI impl__GetDHtmlDocument_CHtmlEditCtrl__QEBAHPEAPEAUIHTMLDocument2___Z(
    const void* pThis, IHTMLDocument2** ppDocument);

namespace {

void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run

// GUIDs the retail bodies reference by address (bytes read from mfc140u .rdata):
//   0x18034f0b0 = {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}  IID_IWebBrowser2
//   0x18034f0a0 = {332C4425-26CB-11D0-B483-00C04FD90119}  IID_IHTMLDocument2
const GUID kIID_IWebBrowser2   = { 0xD30C1661, 0xCDAF, 0x11D0, { 0x8A,0x3E,0x00,0xC0,0x4F,0xC9,0xE2,0x6E } };
const GUID kIID_IHTMLDocument2 = { 0x332C4425, 0x26CB, 0x11D0, { 0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19 } };

// Mirror of the private AFX_EVENTSINKMAP structs pinned in
// core/runtime/CCmdTarget.cpp (AFX_DISPMAP_ENTRY 0x40 bytes, entry 0x48);
// the same mirror core/view/CHtmlView.cpp keeps.
enum EvDispFlags_ { evDispCustom_ = 0 };
struct EvDispMapEntry_ {
    const wchar_t*  lpszName;     // +0x00
    long            lDispID;      // +0x08
    const char*     lpszParams;   // +0x10
    unsigned short  vt;           // +0x18
    const void*     pfn;          // +0x20
    const void*     pfnSet;       // +0x28
    size_t          nPropOffset;  // +0x30
    EvDispFlags_    flags;        // +0x38
};
struct EvSinkMapEntry_ {
    EvDispMapEntry_ dispEntry;    // +0x00
    unsigned int    nCtrlIDFirst; // +0x40
    unsigned int    nCtrlIDLast;  // +0x44
};
struct EvSinkMap_ {
    const void* (MS_ABI* pfnGetBaseMap)();
    const EvSinkMapEntry_* lpEntries;
    unsigned int* lpEntryCount;
};
static_assert(sizeof(EvDispMapEntry_) == 0x40, "AFX_DISPMAP_ENTRY is 0x40 bytes");
static_assert(offsetof(EvDispMapEntry_, vt) == 0x18, "AFX_DISPMAP_ENTRY::vt at +0x18");
static_assert(offsetof(EvDispMapEntry_, pfn) == 0x20, "AFX_DISPMAP_ENTRY::pfn at +0x20");
static_assert(offsetof(EvDispMapEntry_, nPropOffset) == 0x30, "AFX_DISPMAP_ENTRY::nPropOffset at +0x30");
static_assert(sizeof(EvSinkMapEntry_) == 0x48, "AFX_EVENTSINKMAP_ENTRY stride is 0x48");
static_assert(offsetof(EvSinkMapEntry_, nCtrlIDFirst) == 0x40, "nCtrlIDFirst at +0x40");
static_assert(sizeof(EvSinkMap_) == 0x18, "AFX_EVENTSINKMAP is three pointers");

// CHtmlEditCtrl::_OnNavigateComplete2(LPDISPATCH, VARIANT*) -- protected, not
// exported; retail body at RVA 0x27d800 (mfc140u), reached only through the
// event-sink entry below.  It is the inlined CHtmlEditCtrlBase::SetDesignMode(TRUE);
// neither argument is read:
//     CComPtr<IHTMLDocument2> spDoc;  GetDHtmlDocument(&spDoc);   // direct call 0x27d8b0, result ignored
//     if (spDoc) {
//         CComBSTR bstr(L"On");          // 0x18034f068; SysAllocString (OLEAUT32 #2, IAT 0x1802c69c8)
//                                        // NULL -> AtlThrow(E_OUTOFMEMORY) (0x333c) -> AfxThrowMemoryException (0x2276c0)
//         spDoc->put_designMode(bstr);   // IHTMLDocument2 vslot 0x98/8 = 19
//     }                                  // SysFreeString (OLEAUT32 #6, IAT 0x1802c69d8), then Release
// Minor difference: the document is released before the out-of-memory throw
// (retail releases it during unwinding).
void MS_ABI OnNavigateComplete2_(void* pThis, IDispatch* pDisp, VARIANT* pURL) {
    (void)pDisp;
    (void)pURL;
    IHTMLDocument2* pDoc = nullptr;
    impl__GetDHtmlDocument_CHtmlEditCtrl__QEBAHPEAPEAUIHTMLDocument2___Z(pThis, &pDoc);
    if (pDoc == nullptr)
        return;
    BSTR bstr = ::SysAllocString(L"On");
    if (bstr == nullptr) {
        pDoc->Release();
        impl__AfxThrowMemoryException__YAXXZ();
        return;
    }
    pDoc->put_designMode(bstr);
    ::SysFreeString(bstr);
    pDoc->Release();
}

// Retail event-sink map, 0x180333ae0 (mfc140u) = { 0x1801de860, 0x180333b00, 0x1803b29d8 }:
//   pfnGetBaseMap 0x1801de860 = CCmdTarget::GetThisEventSinkMap (ordinal 7191,
//                 ICF-folded with CCmdTarget::GetEventSinkMap) -- CWnd declares
//                 no event-sink map of its own, so BEGIN_EVENTSINK_MAP(CHtmlEditCtrl, CWnd)
//                 resolves to CCmdTarget's.
//   entries at 0x180333b00, two 0x48-byte records:
//     [0] lpszName 0x18033d19c (L""), lDispID 252 (DISPID_NAVIGATECOMPLETE2),
//         lpszParams 0x18034c69c = "\x09\x4c" (VTS_DISPATCH VTS_PVARIANT),
//         vt VT_BOOL, pfn 0x18027d800 (_OnNavigateComplete2), pfnSet NULL,
//         nPropOffset 0, afxDispCustom, nCtrlIDFirst = nCtrlIDLast = 0xffffffff
//         -- the shape ON_EVENT_REFLECT(CHtmlEditCtrl, 252, _OnNavigateComplete2,
//         VTS_DISPATCH VTS_PVARIANT) (afxdisp.h:787)
//     [1] terminator: lpszName NULL, lDispID -1, vt VT_VOID (0x18),
//         nPropOffset (size_t)-1, nCtrlIDFirst 0xffffffff, nCtrlIDLast 0
//   entry count at 0x1803b29d8 (.data) holds 0xffffffff.
// OpenMFC's handler address stands in for retail's 0x18027d800.
const wchar_t kEvName_[] = L"";
const EvSinkMapEntry_ g_evsinkEntries_CHtmlEditCtrl[] = {
    { { kEvName_, 252 /*DISPID_NAVIGATECOMPLETE2*/, "\x09\x4c", VT_BOOL,
        reinterpret_cast<const void*>(&OnNavigateComplete2_), nullptr, 0, evDispCustom_ }, 0xffffffffu, 0xffffffffu },
    { { nullptr, -1 /*DISPID_UNKNOWN*/, nullptr, VT_VOID, nullptr, nullptr, (size_t)-1, evDispCustom_ }, 0xffffffffu, 0u },
};
unsigned int g_evsinkEntryCount_CHtmlEditCtrl = 0xffffffffu;
const EvSinkMap_ g_eventSinkMap_CHtmlEditCtrl = {
    &impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ,
    g_evsinkEntries_CHtmlEditCtrl,
    &g_evsinkEntryCount_CHtmlEditCtrl
};

} // namespace

// Retail ??0CHtmlEditCtrl@@QEAA@XZ (entry RVA 0x27d760, mfc140u), fully transcribed:
//     CWnd::CWnd();                                   // call 0x28a700
//     vfptr = &CHtmlEditCtrl::`vftable';              // 0x180333b98
//     return this;
// DEVIATION: no MSVC-layout vftable exists for this class here, so `this`
// keeps the vptr ??0CWnd@@ installs (recorded as g_ownVptr, file header).
// Symbol: ??0CHtmlEditCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlEditCtrl__QEAA_XZ(void* pThis) {
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    return pThis;
}

// Retail ??1CHtmlEditCtrl@@UEAA@XZ (entry RVA 0x27d7e0, mfc140u), fully transcribed:
//     vfptr = &CHtmlEditCtrl::`vftable';              // 0x180333b98
//     CWnd::~CWnd();                                  // tail jump 0x28b740
// DEVIATION: the own-vftable store becomes the g_ownVptr store (file header).
// Symbol: ??1CHtmlEditCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlEditCtrl__UEAA_XZ(void* pThis) {
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CWnd__UEAA_XZ(pThis);
}

// STUB.  Retail Create (entry RVA 0x27d9c0, mfc140u) is:
//     AfxEnableControlContainer(NULL);                               // call 0x237260
//     BOOL bRet = FALSE;
//     if (CreateControl(CLSID_WebBrowser /*0x1802d9ed8 = {8856F961-340A-11D0-A96B-00C04FD705A2}*/,
//                       lpszWindowName, WS_CHILD|WS_VISIBLE /*0x50000000; dwStyle is NOT used*/,
//                       rect, pParentWnd, nID, NULL, FALSE, NULL)) { // call 0x234f20
//         CComQIPtr<IWebBrowser2> spBrowser(GetControlUnknown());    // inlined: m_pCtrlSite(+0xd0)->m_pObject(+0x80)
//         if (spBrowser) {
//             CComVariant vEmpty;                                    // memset 0x18 + VariantInit (OLEAUT32 #8)
//             LPCTSTR szDoc = GetStartDocument();                    // vslot 0x2e0/8 = 92
//             if (szDoc) {
//                 CComBSTR bstrStart(szDoc);                         // SysAllocString; NULL -> AtlThrow(E_OUTOFMEMORY)
//                 bRet = spBrowser->Navigate(bstrStart, &vEmpty, &vEmpty, &vEmpty, &vEmpty) == S_OK;  // vslot 0x58/8 = 11
//             } else
//                 bRet = TRUE;
//         }                                                          // VariantClear (OLEAUT32 #9), Release
//     }
//     if (!bRet) DestroyWindow();                                    // vslot 0xd0/8 = 26 -- on EVERY failure path
//     return bRet;                                                   // pContext is never read
// Not implemented: the whole body hinges on CWnd::CreateControl(REFCLSID, ...,
// const RECT&, ...), and OpenMFC's thunk for it
// (core/window/CWnd.cpp, impl__CreateControl_CWnd__QEAAHAEBU_GUID__PEB_WKAEBUtagRECT__PEAV1_IPEAVCFile__HPEA_W_Z)
// forwards to a C++ CWnd::CreateControl that dynamic_casts `this` to
// COleControl -- it cannot host a WebBrowser control in an arbitrary CWnd, and
// on a client object carrying an MSVC vftable that dynamic_cast reads MSVC RTTI
// as Itanium RTTI.  Returning FALSE here is the honest outcome until that
// sibling hosts controls.  The previous placeholder parameter list (which
// omitted `this`) is corrected to the mangled signature.
// Symbol: ?Create@CHtmlEditCtrl@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@HPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CHtmlEditCtrl__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__HPEAUCCreateContext___Z(
    void* pThis, const wchar_t* lpszWindowName, unsigned long dwStyle, const RECT& rect,
    CWnd* pParentWnd, int nID, CCreateContext* pContext) {
    (void)pThis;
    (void)lpszWindowName;
    (void)dwStyle;
    (void)rect;
    (void)pParentWnd;
    (void)nID;
    (void)pContext;
    return FALSE;
}

// Retail GetDHtmlDocument (entry RVA 0x27d8b0, mfc140u):
//     if (ppDocument == NULL) return FALSE;
//     *ppDocument = NULL;
//     CComQIPtr<IWebBrowser2> spBrowser(GetControlUnknown());   // QI slot 0, IID at 0x18034f0b0;
//                                                               // a failed QI leaves spBrowser NULL
//     BOOL bRet = FALSE;
//     if (spBrowser) {
//         CComPtr<IDispatch> spDisp;
//         if (SUCCEEDED(spBrowser->get_Document(&spDisp)) && spDisp != NULL)   // vslot 0x90/8 = 18
//             bRet = spDisp->QueryInterface(IID_IHTMLDocument2, (void**)ppDocument) == S_OK;  // `sete`
//     }                                        // spDisp / spBrowser released on every path that set them
//     return bRet;
// DEVIATION: retail inlines CWnd::GetControlUnknown -- the instruction
// sequence `mov 0xd0(%rcx); test; mov 0x80(%r9)` (m_pCtrlSite->m_pObject) is the
// whole body of the export at RVA 0x2a92c0 (mfc140u).  OpenMFC's CWnd does not
// keep m_pCtrlSite at +0xd0 (that offset lies in the zero-filled
// _cwnd_padding2; core/ole/COccManager.cpp recovers the site link by other
// means), so the lookup is made through that export's thunk instead.
// CAVEAT (reviewer-verified): that thunk forwards to the C++
// CWnd::GetControlUnknown in core/window/CWnd.cpp, which is NOT equivalent to
// retail.  It searches the container this window OWNS
// (g_controlContainerMap[this], the one CreateControlContainer makes for this
// window's children), whereas retail reads m_pCtrlSite, the site in the
// PARENT's container that hosts this window.  A WebBrowser hosted the retail
// way therefore resolves to NULL through it today, and this function returns
// FALSE.  The fix belongs in CWnd::GetControlUnknown (a parent-container
// lookup like SiteOfWnd in core/ole/COccManager.cpp), not here; nothing can
// host the control yet anyway, because Create below is still a stub.
// Symbol: ?GetDHtmlDocument@CHtmlEditCtrl@@QEBAHPEAPEAUIHTMLDocument2@@@Z
extern "C" int MS_ABI impl__GetDHtmlDocument_CHtmlEditCtrl__QEBAHPEAPEAUIHTMLDocument2___Z(
    const void* pThis, IHTMLDocument2** ppDocument) {
    if (ppDocument == nullptr)
        return FALSE;
    *ppDocument = nullptr;
    IUnknown* pUnk = impl__GetControlUnknown_CWnd__QEAAPEAUIUnknown__XZ(
        static_cast<CWnd*>(const_cast<void*>(pThis)));
    IWebBrowser2* pBrowser = nullptr;
    if (pUnk != nullptr &&
        FAILED(pUnk->QueryInterface(kIID_IWebBrowser2, reinterpret_cast<void**>(&pBrowser))))
        pBrowser = nullptr;
    if (pBrowser == nullptr)
        return FALSE;
    int bRet = FALSE;
    IDispatch* pDisp = nullptr;
    HRESULT hr = pBrowser->get_Document(&pDisp);
    if (SUCCEEDED(hr) && pDisp != nullptr)
        bRet = pDisp->QueryInterface(kIID_IHTMLDocument2, reinterpret_cast<void**>(ppDocument)) == S_OK;
    if (pDisp != nullptr)
        pDisp->Release();
    pBrowser->Release();
    return bRet;
}

// Retail GetEventSinkMap (entry RVA 0x27d7f0, mfc140u): `lea 0x180333ae0,%rax ; ret`.
// Symbol: ?GetEventSinkMap@CHtmlEditCtrl@@MEBAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetEventSinkMap_CHtmlEditCtrl__MEBAPEBUAFX_EVENTSINKMAP__XZ(const void* pThis) {
    (void)pThis;
    return &g_eventSinkMap_CHtmlEditCtrl;
}

// Retail GetStartDocument (entry RVA 0x27db70, mfc140u; also slot 92 of the
// CHtmlEditCtrl vftable and ICF-shared with CHtmlEditView::GetStartDocument):
//     lea 0x18034f030 -> L"about:blank";  ret
// Symbol: ?GetStartDocument@CHtmlEditCtrl@@UEAAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetStartDocument_CHtmlEditCtrl__UEAAPEB_WXZ(void* pThis) {
    (void)pThis;
    return L"about:blank";
}

// Retail GetThisEventSinkMap: export ordinal 7193 resolves to the same RVA
// 0x27d7f0 (mfc140u) as GetEventSinkMap (identical-code folding).
// Symbol: ?GetThisEventSinkMap@CHtmlEditCtrl@@KAPEBUAFX_EVENTSINKMAP@@XZ
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CHtmlEditCtrl__KAPEBUAFX_EVENTSINKMAP__XZ() {
    return &g_eventSinkMap_CHtmlEditCtrl;
}
