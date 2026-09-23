// CHtmlControlSite — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CHtmlControlSite is the control site CHtmlView installs under its hosted
// WebBrowser control.  It is private to MFC's implementation: no header in the
// on-host retail atlmfc/include tree declares it (afxhtml.h included -- grep
// finds the name nowhere under the MSVC install), and neither do the OpenMFC
// public headers.  Retail's exports show a COleControlSite plus one MFC nested
// interface class, XDocHostUIHandler (the X<name> nesting of the mangled
// names), whose fifteen IDocHostUIHandler methods forward to the owning
// CHtmlView's On* virtuals.
// The sibling core/view/CBrowserControlSite.cpp does the same job for
// CDHtmlDialog, but finds its handler through a member of its own; this class
// finds its view through the control container instead.
//
// Everything below was decoded from mfc140u.dll, the image OpenMFC
// reimplements (mangled name -> RVA -> objdump; see the header comment of
// core/ole/COleControl.cpp).  Every RVA quoted here is an mfc140u RVA unless
// it says otherwise.
//
// Retail object layout, read out of the constructor at mfc140u 0x27ad80:
//
//     +0x000  COleControlSite base                           sizeof == 0x180
//             (ctor calls ??0COleControlSite at 0x2394e0, then stores the
//              CHtmlControlSite primary vftable 0x180333f20 at +0)
//     +0x180  m_xDocHostUIHandler: the XDocHostUIHandler vftable pointer
//             (ctor stores 0x180333e88 there)                total 0x188
//
// The 0x188 total matches the `operator new(0x188)` in
// CHtmlView::CreateControlSite (0x27b150), quoted in core/view/CHtmlView.cpp.
//
// Provenance of the fifteen method RVAs.  The mfc140u symbol map
// (mfc140u_rva_symbols.json, what disas.py --u reads) names only eight of them
// (EnableModeless, FilterDataObject, GetDropTarget, GetExternal, GetHostInfo,
// GetOptionKeyPath, HideUI, OnDocWindowActivate).  All fifteen were identified
// three independent ways:
//   * the XDocHostUIHandler vftable at mfc140u 0x180333e88 (the one the ctor
//     installs) holds 18 entries in IDocHostUIHandler order: the three
//     IUnknown methods, which identical-code folding shares with
//     CBrowserControlSite (0x210e30 / 0x210e60 / 0x210e70 -- all three open
//     with `add $-0x180,%rcx`, the same sub-object offset), then 0x27ae40,
//     0x27ae80, 0x27aef0, 0x27af40, 0x27af70, 0x27afa0, 0x27afd0, 0x27b000,
//     0x27b030, 0x27b060, 0x27b090, 0x27b0c0, 0x27ae10, 0x27b0f0, 0x27b120;
//     each of the eight named ones sits in the slot its name belongs to; and
//   * the mfc140 (ANSI) symbol map names all fifteen (TranslateAccelerator
//     under its ...A spelling), and this block sits at a constant +0x1370
//     from there to mfc140u (GetExternal 0x279aa0 -> 0x27ae10, ShowContextMenu
//     0x279ad0 -> 0x27ae40, ... FilterDataObject 0x279db0 -> 0x27b120), which
//     lands every ANSI name on the vftable slot above; and
//   * resolving each export ordinal (mfc_complete_ordinal_mapping.json,
//     ordinals 4278..14179) directly through mfc140u.dll's own export address
//     table (ordinal base 256, 14109 entries) gives exactly the same fifteen
//     RVAs, e.g. UpdateUI ordinal 14179 -> 0x27af70, ShowContextMenu 13810 ->
//     0x27ae40, OnFrameWindowActivate 9969 -> 0x27b000.
//
// What every method does.  rcx on entry is the +0x180 sub-object, and each
// body chases the same two pointers:
//     mov -0x140(%rcx),%rax    ; site+0x40  = COleControlSite::m_pCtrlCont
//     mov 0x40(%rax),%rcx      ; cont+0x40  = COleControlContainer::m_pWnd
//     test %rcx,%rcx ; jne     ; only m_pWnd is null-checked
//     mov $0x80004005,%eax     ; m_pWnd == NULL -> return E_FAIL
//     mov (%rcx),%rax ; mov 0xNNN(%rax),%rax ; dispatch through
//         __guard_dispatch_icall_fptr (mfc140u 0x1802c7b30, the load config's
//         GuardCFDispatchFunctionPointer)
// i.e. `CHtmlView* pView = (CHtmlView*)m_pCtrlCont->m_pWnd; if (!pView)
// return E_FAIL; return pView->OnXxx(args...);` with every argument passed
// through untouched.  The cast is a plain reinterpretation (retail CHtmlView
// reaches CWnd through a single-inheritance chain, CFormView -> CScrollView ->
// CView -> CWnd, so both pointers share an address); there is no IsKindOf
// check.  Apart from GetHostInfo (below), the methods with at most three
// arguments counting the sub-object tail-jump with no frame; the five with
// four or more --
// ShowContextMenu, ShowUI, ResizeBorder, TranslateAccelerator, TranslateUrl --
// open a 0x38-byte frame (0x48 for ShowUI), re-store their stack arguments
// into the callee's 0x20/0x28(%rsp) slots (only ShowContextMenu and ShowUI
// have any), call, and return the HRESULT.  GetHostInfo alone additionally
// wraps the whole body in a module-state switch (an AFX_MAINTAIN_STATE2 on the
// site's m_pModuleState) and therefore opens a frame too -- see its own comment.
//
// The retail CHtmlView slots 0x388..0x3f8 (slots 113..127).  They are the
// fifteen HRESULT virtuals afxhtml.h declares consecutively, in declaration
// order: OnShowContextMenu, OnGetExternal, OnGetHostInfo, OnShowUI, OnHideUI,
// OnUpdateUI, OnEnableModeless, OnDocWindowActivate, OnFrameWindowActivate,
// OnResizeBorder, OnTranslateAccelerator, OnGetOptionKeyPath,
// OnFilterDataObject, OnTranslateUrl, OnGetDropTarget.  Checked against the
// retail CHtmlView vftable at mfc140u 0x1803340e8 (slot 0 =
// CHtmlView::GetRuntimeClass): slots 113, 114, 116 and 123-127 hold the folded
// `mov $1,%eax; ret` at 0x3a60 and slots 115 and 117-122 the folded
// `xor %eax,%eax; ret` at 0x71e0 -- exactly the RVAs the fifteen
// CHtmlView::On* exports resolve to under that name-to-slot assignment
// (OnGetHostInfo and OnHideUI..OnResizeBorder are the seven S_OK ones).  The
// forwarder that targets each slot is also the one with the matching name
// (GetExternal -> +0x390, FilterDataObject -> +0x3e8, GetDropTarget -> +0x3f8,
// and so on).
//
// DISPATCH MODEL, and its limit.  This file calls those virtuals by retail
// slot index through the view's own vptr, exactly as retail does.  That is
// right for any view whose vftable was laid out by MSVC against the retail
// afxhtml.h -- an application's CHtmlView-derived class -- which is the object
// retail expects here.  It is NOT right for a CHtmlView whose vftable mingw
// built from OpenMFC's afxwin.h: that class declares only two of these
// fifteen virtuals (OnGetOptionKeyPath, OnTranslateUrl) and at unrelated
// slots, so dispatching by name is not available and dispatching by retail
// slot on such an object would call whatever lies at that index.  Nothing can
// distinguish the two from here without a new cross-TU C++ symbol reference,
// so no check is made.
//
// REACHABILITY.  In this tree nothing hands out a pointer to the +0x180
// sub-object yet: the constructor below does not install the XDocHostUIHandler
// vftable, CHtmlView::CreateControlSite allocates only sizeof(COleControlSite)
// (0x180) bytes, and the CHtmlControlSite interface map
// (detail/InterfaceMapsSupport.cpp) has no IDocHostUIHandler entry -- retail's
// map (GetInterfaceMap, mfc140u 0x27ad70, returns 0x1803340d0) chains to
// COleControlSite's map and holds exactly one entry, IID_IDocHostUIHandler
// {bd3f23c0-d43e-11cf-893b-00aa00bdce1a} at offset 0x180.  The
// bodies below are therefore reachable only through their exports until those
// three gaps are closed (reported separately).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"

namespace {

// Byte offset of the XDocHostUIHandler sub-object inside CHtmlControlSite.
// Retail value (ctor 0x27ad80 stores its vftable at +0x180; the folded
// IUnknown methods open with `add $-0x180,%rcx`).  It is the end of the
// COleControlSite base in retail, and OpenMFC's COleControlSite is the same
// size, which is what keeps the sub-object at the same place.
const ptrdiff_t kDocHostOffset = 0x180;
static_assert(sizeof(COleControlSite) == 0x180,
              "CHtmlControlSite keeps XDocHostUIHandler at +0x180 (retail "
              "mfc140u ctor 0x27ad80); OpenMFC's COleControlSite must stay "
              "0x180 bytes for that to remain the end of the base.");

// The pointer chase every method opens with.  Retail reads site+0x40 and
// then container+0x40.  OpenMFC's COleControlSite and COleControlContainer do
// NOT keep those members at retail's offsets (their CCmdTarget base is not
// retail-sized), so the chase goes through the named members instead -- the
// same retail-to-OpenMFC mapping core/ole/COleControlSite.cpp documents
// ("retail +0x40 m_pCtrlCont -> COleControlSite::m_pCtrlCont") and
// core/ole/COleControlContainer.cpp uses for m_pWnd.  The site that owns the
// sub-object is always one OpenMFC constructed (the ctor below), so its
// members are at OpenMFC's offsets.
//
// DEVIATIONS: the sub-object pointer and m_pCtrlCont are null-checked here;
// retail checks neither and would fault.  A null at either point is answered
// with retail's own "no view" result, E_FAIL.
inline CWnd* ViewOf(void* pDocHost) {
    if (!pDocHost) return nullptr;
    COleControlSite* pSite = reinterpret_cast<COleControlSite*>(
        static_cast<char*>(pDocHost) - kDocHostOffset);
    COleControlContainer* pCont = pSite->m_pCtrlCont;   // retail: site+0x40
    return pCont ? pCont->m_pWnd : nullptr;             // retail: cont+0x40
}

// Retail CHtmlView vftable byte offsets (see the file header for how each was
// identified).  Slot index = offset / 8.
enum : unsigned {
    kOnShowContextMenu      = 0x388,  // slot 113
    kOnGetExternal          = 0x390,  // slot 114
    kOnGetHostInfo          = 0x398,  // slot 115
    kOnShowUI               = 0x3a0,  // slot 116
    kOnHideUI               = 0x3a8,  // slot 117
    kOnUpdateUI             = 0x3b0,  // slot 118
    kOnEnableModeless       = 0x3b8,  // slot 119
    kOnDocWindowActivate    = 0x3c0,  // slot 120
    kOnFrameWindowActivate  = 0x3c8,  // slot 121
    kOnResizeBorder         = 0x3d0,  // slot 122
    kOnTranslateAccelerator = 0x3d8,  // slot 123
    kOnGetOptionKeyPath     = 0x3e0,  // slot 124
    kOnFilterDataObject     = 0x3e8,  // slot 125
    kOnTranslateUrl         = 0x3f0,  // slot 126
    kOnGetDropTarget        = 0x3f8,  // slot 127
};

// `mov (%rcx),%rax ; mov off(%rax),%rax` -- fetch the entry at that byte
// offset of the view's vftable, as an MS-ABI member function taking the view
// as its first argument.
template <typename Fn>
inline Fn ViewSlot(CWnd* pView, unsigned byteOffset) {
    void* const* vtbl = *reinterpret_cast<void* const* const*>(pView);
    return reinterpret_cast<Fn>(vtbl[byteOffset / sizeof(void*)]);
}

typedef long (MS_ABI* PfnVoid)(CWnd*);
typedef long (MS_ABI* PfnBool)(CWnd*, int);
typedef long (MS_ABI* PfnShowContextMenu)(CWnd*, unsigned long, POINT*, IUnknown*, IDispatch*);
typedef long (MS_ABI* PfnGetHostInfo)(CWnd*, DOCHOSTUIINFO*);
typedef long (MS_ABI* PfnShowUI)(CWnd*, unsigned long, IOleInPlaceActiveObject*,
                                 IOleCommandTarget*, IOleInPlaceFrame*, IOleInPlaceUIWindow*);
typedef long (MS_ABI* PfnResizeBorder)(CWnd*, const RECT*, IOleInPlaceUIWindow*, int);
typedef long (MS_ABI* PfnTranslateAccelerator)(CWnd*, MSG*, const GUID*, unsigned long);
typedef long (MS_ABI* PfnGetOptionKeyPath)(CWnd*, wchar_t**, unsigned long);
typedef long (MS_ABI* PfnGetDropTarget)(CWnd*, IDropTarget*, IDropTarget**);
typedef long (MS_ABI* PfnGetExternal)(CWnd*, IDispatch**);
typedef long (MS_ABI* PfnTranslateUrl)(CWnd*, unsigned long, wchar_t*, wchar_t**);
typedef long (MS_ABI* PfnFilterDataObject)(CWnd*, IDataObject*, IDataObject**);

}  // namespace

// CHtmlControlSite::CHtmlControlSite(COleControlContainer*) -- retail 0x27ad80:
//     COleControlSite::COleControlSite(pCtrlCont);        ; call 0x2394e0
//     this[0x000] = &CHtmlControlSite::`vftable'          ; 0x180333f20
//     this[0x180] = &XDocHostUIHandler `vftable'          ; 0x180333e88
//     return this;
// The body here constructs the COleControlSite base only and writes neither
// CHtmlControlSite vftable: the object keeps the vptr OpenMFC's own
// COleControlSite constructor installs, and +0x180 is not touched.  Writing +0x180 is not safe as things stand: the only in-tree
// caller, CHtmlView::CreateControlSite (core/view/CHtmlView.cpp), allocates
// sizeof(COleControlSite) == 0x180 bytes, not retail's 0x188.
// Symbol: ??0CHtmlControlSite@@QEAA@PEAVCOleControlContainer@@@Z
extern "C" void* MS_ABI impl___0CHtmlControlSite__QEAA_PEAVCOleControlContainer___Z(void* pThis, void* pContainer) {
    if (pThis == nullptr) {
        return pThis;
    }
    return new (pThis) COleControlSite(static_cast<COleControlContainer*>(pContainer));
}
// CHtmlControlSite::~CHtmlControlSite -- retail 0x27ae00 stores the primary
// vftable (0x180333f20) at +0 and tail-jumps to ~COleControlSite (0x2396b0).
// Symbol: ??1CHtmlControlSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlControlSite__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    static_cast<COleControlSite*>(pThis)->~COleControlSite();
}

//-----------------------------------------------------------------------------
// The fifteen XDocHostUIHandler methods, in marker order.  Each is the
// forwarder described in the file header: `pThis` is the +0x180 sub-object;
// the view is m_pCtrlCont->m_pWnd; no view -> E_FAIL; otherwise the retail
// CHtmlView slot named on each is called with the arguments unchanged and its
// HRESULT returned.
//-----------------------------------------------------------------------------

// retail 0x27afa0 -> CHtmlView +0x3b8 OnEnableModeless (tail jump)
// Symbol: ?EnableModeless@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(
    void* pThis, int fEnable) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnBool>(pView, kOnEnableModeless)(pView, fEnable);
}

// retail 0x27b120 -> CHtmlView +0x3e8 OnFilterDataObject (tail jump)
// Symbol: ?FilterDataObject@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUIDataObject@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__FilterDataObject_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUIDataObject__PEAPEAU3__Z(
    void* pThis, IDataObject* pDO, IDataObject** ppDORet) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnFilterDataObject>(pView, kOnFilterDataObject)(pView, pDO, ppDORet);
}

// retail 0x27b0c0 -> CHtmlView +0x3f8 OnGetDropTarget (tail jump)
// Symbol: ?GetDropTarget@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUIDropTarget@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__GetDropTarget_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUIDropTarget__PEAPEAU3__Z(
    void* pThis, IDropTarget* pDropTarget, IDropTarget** ppDropTarget) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnGetDropTarget>(pView, kOnGetDropTarget)(pView, pDropTarget, ppDropTarget);
}

// retail 0x27ae10 -> CHtmlView +0x390 OnGetExternal (tail jump)
// Symbol: ?GetExternal@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExternal_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAPEAUIDispatch___Z(
    void* pThis, IDispatch** ppDispatch) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnGetExternal>(pView, kOnGetExternal)(pView, ppDispatch);
}

// retail 0x27ae80 -> CHtmlView +0x398 OnGetHostInfo.  The only one of the
// fifteen with a module-state switch (the _AFXDLL expansion of
// METHOD_PROLOGUE_EX / AFX_MANAGE_STATE -- the source macro itself is an
// inference; the instructions are not): before anything else it loads
// `mov -0x148(%rcx),%rdx` -- site+0x38, the site's CCmdTarget::m_pModuleState
// -- and constructs an AFX_MAINTAIN_STATE2 on it (call 0x133170, which is
// ??0AFX_MAINTAIN_STATE2), then does the usual view lookup and slot call
// (E_FAIL when there is no view), and finally restores the previous module
// state with the inlined destructor (if the saved thread-state pointer at
// 0x28(%rsp) is non-null, write the saved module state at 0x20(%rsp) back to
// its +8) on both paths.
// DEVIATION: the module-state switch is not reproduced.  OpenMFC's CCmdTarget
// has no m_pModuleState member -- it is 0x20 bytes, so site+0x38 in an OpenMFC
// COleControlSite is one of the site's own interface pointers, not a module
// state -- and there is no other per-site record of the owning module to
// switch to.  The lookup, the dispatch and the result are retail's.
// Symbol: ?GetHostInfo@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__GetHostInfo_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAU_DOCHOSTUIINFO___Z(
    void* pThis, DOCHOSTUIINFO* pInfo) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnGetHostInfo>(pView, kOnGetHostInfo)(pView, pInfo);
}

// retail 0x27b090 -> CHtmlView +0x3e0 OnGetOptionKeyPath (tail jump)
// Symbol: ?GetOptionKeyPath@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__GetOptionKeyPath_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAPEA_WK_Z(
    void* pThis, wchar_t** pchKey, unsigned long dw) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnGetOptionKeyPath>(pView, kOnGetOptionKeyPath)(pView, pchKey, dw);
}

// retail 0x27af40 -> CHtmlView +0x3a8 OnHideUI (tail jump)
// Symbol: ?HideUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__HideUI_XDocHostUIHandler_CHtmlControlSite__UEAAJXZ(void* pThis) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnVoid>(pView, kOnHideUI)(pView);
}

// retail 0x27afd0 -> CHtmlView +0x3c0 OnDocWindowActivate (tail jump)
// Symbol: ?OnDocWindowActivate@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(
    void* pThis, int fActivate) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnBool>(pView, kOnDocWindowActivate)(pView, fActivate);
}

// retail 0x27b000 -> CHtmlView +0x3c8 OnFrameWindowActivate (tail jump)
// Symbol: ?OnFrameWindowActivate@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(
    void* pThis, int fActivate) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnBool>(pView, kOnFrameWindowActivate)(pView, fActivate);
}

// retail 0x27b030 -> CHtmlView +0x3d0 OnResizeBorder (0x38 frame, call)
// Symbol: ?ResizeBorder@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_XDocHostUIHandler_CHtmlControlSite__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    void* pThis, const RECT* prcBorder, IOleInPlaceUIWindow* pUIWindow, int fFrameWindow) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnResizeBorder>(pView, kOnResizeBorder)(pView, prcBorder, pUIWindow, fFrameWindow);
}

// retail 0x27ae40 -> CHtmlView +0x388 OnShowContextMenu.  0x38 frame; the
// fifth argument (pdispReserved, caller's 0x28(%rsp) == 0x60(%rsp) after the
// frame) is re-stored at 0x20(%rsp) for the callee -- pass-through, no reorder.
// Symbol: ?ShowContextMenu@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__ShowContextMenu_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(
    void* pThis, unsigned long dwID, POINT* ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnShowContextMenu>(pView, kOnShowContextMenu)(
        pView, dwID, ppt, pcmdtReserved, pdispReserved);
}

// retail 0x27aef0 -> CHtmlView +0x3a0 OnShowUI.  0x48 frame; the two stack
// arguments (pFrame at 0x70(%rsp), pDoc at 0x78(%rsp) after the frame) are
// re-stored at 0x20/0x28(%rsp) -- pass-through, no reorder.
// Symbol: ?ShowUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__ShowUI_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(
    void* pThis, unsigned long dwID, IOleInPlaceActiveObject* pActiveObject,
    IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnShowUI>(pView, kOnShowUI)(
        pView, dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

// retail 0x27b060 -> CHtmlView +0x3d8 OnTranslateAccelerator (0x38 frame,
// call).  mfc140.dll names this entry point ?TranslateAcceleratorA@... at
// 0x279cf0; mfc140u exports the ...W spelling implemented here.
// Symbol: ?TranslateAcceleratorW@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUtagMSG__PEBU_GUID__K_Z(
    void* pThis, MSG* lpMsg, const GUID* pguidCmdGroup, unsigned long nCmdID) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnTranslateAccelerator>(pView, kOnTranslateAccelerator)(
        pView, lpMsg, pguidCmdGroup, nCmdID);
}

// retail 0x27b0f0 -> CHtmlView +0x3f0 OnTranslateUrl (0x38 frame, call)
// Symbol: ?TranslateUrl@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__TranslateUrl_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEA_WPEAPEA_W_Z(
    void* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnTranslateUrl>(pView, kOnTranslateUrl)(pView, dwTranslate, pchURLIn, ppchURLOut);
}

// retail 0x27af70 -> CHtmlView +0x3b0 OnUpdateUI (tail jump)
// Symbol: ?UpdateUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__UpdateUI_XDocHostUIHandler_CHtmlControlSite__UEAAJXZ(void* pThis) {
    CWnd* pView = ViewOf(pThis);
    if (!pView) return E_FAIL;
    return ViewSlot<PfnVoid>(pView, kOnUpdateUI)(pView);
}
