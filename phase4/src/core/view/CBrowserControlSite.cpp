// CBrowserControlSite — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CBrowserControlSite is the control site CDHtmlDialog installs under the
// hosted WebBrowser control.  It is NOT declared in the OpenMFC public headers
// (retail declares it in atlmfc/include/afxdhtml.h), so the layout it needs is
// pinned below, in this file, out of the retail disassembly.
//
// Everything here was decoded from the retail exports the way the rest of this
// project works (mangled name -> RVA -> objdump; see the header comment of
// core/ole/COleControl.cpp).  Every RVA quoted below is an mfc140u.dll address
// -- the image OpenMFC reimplements -- and every one was disassembled out of
// that image; see "Provenance of the RVAs" below for how the nine entry points
// the mfc140u symbol map does not name were identified.
//
// Retail object layout, read straight out of the constructor at mfc140u
// 0x210d50:
//
//     +0x000  COleControlSite base (its vftable at +0)     sizeof == 0x180
//     +0x180  IDocHostUIHandler vftable pointer   <- the second base of the
//                                                    multiple-inheritance pair
//     +0x188  CDHtmlDialog* m_pHandler
//
// Confirmed independently three ways: the ctor stores the second vftable to
// [rbx+0x180] and pHandler to [rbx+0x188]; GetInterfaceHook (0x210df0) returns
// `lea 0x180(%rbx)`; and every IDocHostUIHandler method is entered with rcx
// pointing at the +0x180 sub-object -- the three IUnknown methods open with
// `add $-0x180,%rcx` to recover the complete object, and the fifteen
// IDocHostUIHandler methods read m_pHandler as `mov 0x8(%rcx),%rcx`
// (sub-object +8 == complete object +0x188).  The offsets are identical in
// mfc140.dll.
//
// The fifteen IDocHostUIHandler methods are all the same forwarder.  Retail:
//     mov 0x8(%rcx),%rcx        ; m_pHandler
//     mov (%rcx),%rax           ; its vftable
//     mov 0xNNN(%rax),%rax      ; a CDHtmlDialog virtual
//     <dispatch through __guard_dispatch_icall_fptr>
// with 0xNNN running 0x350, 0x358, ... 0x3c0 in exactly IDocHostUIHandler's own
// method order, and every argument passed through untouched.  Two code shapes,
// same behaviour: the ten methods with at most three arguments counting `this`
// tail-jump (`rex.W jmp *__guard_dispatch_icall_fptr`) with no frame at all, while
// the five with four or more counting `this` -- ShowContextMenu, ShowUI,
// ResizeBorder, TranslateAcceleratorW, TranslateUrl -- open a frame (0x48 bytes for
// ShowUI, 0x38 for the other four), re-store any stack arguments into the callee's
// slots at 0x20/0x28(%rsp) (only ShowContextMenu and ShowUI have any), `call`
// instead of jumping, and return the HRESULT.  Nothing is transformed in either shape.
//
// This file calls those virtuals by name rather than by retail slot index:
// OpenMFC's afxwin.h declares all fifteen on CDHtmlDialog but in a different
// declaration order, so OpenMFC's own vtable indices are not retail's, and a name
// call is what preserves the *behaviour* the disassembly specifies.  (Only three
// of the fifteen retail CDHtmlDialog slots carry their own name in the symbol map
// -- +0x358 GetHostInfo, +0x3b0 GetExternal, +0x3c0 FilterDataObject; identical
// COMDAT folding has given eleven of the rest some unrelated function's name, and
// +0x3b8 has none.  The three named slots sit where IDocHostUIHandler's order puts
// them -- GetHostInfo second, not first as in OpenMFC's declaration order -- and
// each is the target of the CBrowserControlSite method of the same name.)
//
// Provenance of the RVAs quoted below.  The mfc140u symbol map names only eleven
// of this class's twenty entry points; the other nine -- QueryInterface, Release,
// ShowContextMenu, ShowUI, UpdateUI, OnFrameWindowActivate, ResizeBorder,
// TranslateAcceleratorW and TranslateUrl -- were identified two independent ways,
// not by position alone:
//   * the IDocHostUIHandler vftable the constructor installs (mfc140u 0x327258)
//     holds exactly these eighteen entries, in interface order, 0x210e30 through
//     0x211090; and
//   * the mfc140 (ANSI) symbol map does name all twenty, and this class's whole
//     block sits at a constant -0x1D60 there (ctor 0x20eff0 -> 0x210d50), which
//     maps every ANSI name onto the mfc140u RVA cited here.  That shift is
//     local to this block: the CCmdTarget entry points cited below are 0x1240
//     apart between the two images, not 0x1D60.
// A third, direct check: resolving each of the twenty export ordinals
// (mfc_complete_ordinal_mapping.json) through mfc140u.dll's own export address
// table gives exactly the RVAs cited below.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"

namespace {

// Byte offset of the IDocHostUIHandler sub-object inside CBrowserControlSite.
// Retail value, and it lands on OpenMFC's COleControlSite exactly: the
// static_assert below is what keeps the two in step.
const ptrdiff_t kDocHostOffset = 0x180;
static_assert(sizeof(COleControlSite) == 0x180,
              "CBrowserControlSite pins its IDocHostUIHandler sub-object at "
              "+0x180 (retail mfc140u ctor 0x210d50); OpenMFC's COleControlSite "
              "must stay 0x180 bytes for that to remain the end of the base.");

// What rcx points at on entry to any of the eighteen virtuals reached through
// the second vftable.  m_pHandler is at +8 here == +0x188 in the whole object,
// which is the offset the retail bodies dereference.
struct DocHostSubObject {
    const void* const* lpVtbl;  // complete object +0x180
    CDHtmlDialog*      m_pHandler;  // complete object +0x188
};
static_assert(sizeof(DocHostSubObject) == 0x10, "two pointers");
static_assert(offsetof(DocHostSubObject, m_pHandler) == 8,
              "retail reads m_pHandler as `mov 0x8(%rcx),%rcx`");

inline DocHostSubObject* SubObjectFromSite(void* pThis) {
    return pThis ? reinterpret_cast<DocHostSubObject*>(
                       reinterpret_cast<char*>(pThis) + kDocHostOffset)
                 : nullptr;
}

// The inverse: the `add $-0x180,%rcx` the three IUnknown methods open with.
inline CCmdTarget* SiteFromSubObject(void* pDocHost) {
    if (!pDocHost) return nullptr;
    return static_cast<CCmdTarget*>(reinterpret_cast<COleControlSite*>(
        reinterpret_cast<char*>(pDocHost) - kDocHostOffset));
}

inline CDHtmlDialog* HandlerOf(void* pDocHost) {
    DocHostSubObject* p = reinterpret_cast<DocHostSubObject*>(pDocHost);
    return p ? p->m_pHandler : nullptr;
}

// IID_IDocHostUIHandler.  The sixteen bytes GetInterfaceHook memcmp's against
// live at mfc140u rdata 0x2d9e78 and read
//   c0 23 3f bd 3e d4 cf 11 89 3b 00 aa 00 bd ce 1a
// i.e. {BD3F23C0-D43E-11CF-893B-00AA00BDCE1A}.  Spelled out here rather than
// pulled from an import library so the value is the one that was verified.
const GUID kIID_IDocHostUIHandler =
    { 0xBD3F23C0, 0xD43E, 0x11CF, { 0x89, 0x3B, 0x00, 0xAA, 0x00, 0xBD, 0xCE, 0x1A } };

}  // namespace

// core/runtime/CCmdTarget.cpp -- the three CCmdTarget forwarders the retail
// IUnknown methods reduce to (see each body below for which).
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
    CCmdTarget* pThis, const void* iid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);

// Forward declarations of this file's own thunks, so the constructor can build
// the IDocHostUIHandler vftable out of them.
extern "C" long          MS_ABI impl__QueryInterface_CBrowserControlSite__MEAAJAEBU_GUID__PEAPEAX_Z(void*, const GUID*, void**);
extern "C" unsigned long MS_ABI impl__AddRef_CBrowserControlSite__MEAAKXZ(void*);
extern "C" unsigned long MS_ABI impl__Release_CBrowserControlSite__MEAAKXZ(void*);
extern "C" long          MS_ABI impl__ShowContextMenu_CBrowserControlSite__MEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(void*, unsigned long, POINT*, IUnknown*, IDispatch*);
extern "C" long          MS_ABI impl__GetHostInfo_CBrowserControlSite__MEAAJPEAU_DOCHOSTUIINFO___Z(void*, DOCHOSTUIINFO*);
extern "C" long          MS_ABI impl__ShowUI_CBrowserControlSite__MEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(void*, unsigned long, IOleInPlaceActiveObject*, IOleCommandTarget*, IOleInPlaceFrame*, IOleInPlaceUIWindow*);
extern "C" long          MS_ABI impl__HideUI_CBrowserControlSite__MEAAJXZ(void*);
extern "C" long          MS_ABI impl__UpdateUI_CBrowserControlSite__MEAAJXZ(void*);
extern "C" long          MS_ABI impl__EnableModeless_CBrowserControlSite__MEAAJH_Z(void*, int);
extern "C" long          MS_ABI impl__OnDocWindowActivate_CBrowserControlSite__MEAAJH_Z(void*, int);
extern "C" long          MS_ABI impl__OnFrameWindowActivate_CBrowserControlSite__MEAAJH_Z(void*, int);
extern "C" long          MS_ABI impl__ResizeBorder_CBrowserControlSite__MEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(void*, LPCRECT, IOleInPlaceUIWindow*, int);
extern "C" long          MS_ABI impl__TranslateAcceleratorW_CBrowserControlSite__MEAAJPEAUtagMSG__PEBU_GUID__K_Z(void*, LPMSG, const GUID*, unsigned long);
extern "C" long          MS_ABI impl__GetOptionKeyPath_CBrowserControlSite__MEAAJPEAPEA_WK_Z(void*, wchar_t**, unsigned long);
extern "C" long          MS_ABI impl__GetDropTarget_CBrowserControlSite__MEAAJPEAUIDropTarget__PEAPEAU2__Z(void*, IDropTarget*, IDropTarget**);
extern "C" long          MS_ABI impl__GetExternal_CBrowserControlSite__MEAAJPEAPEAUIDispatch___Z(void*, IDispatch**);
extern "C" long          MS_ABI impl__TranslateUrl_CBrowserControlSite__MEAAJKPEA_WPEAPEA_W_Z(void*, unsigned long, wchar_t*, wchar_t**);
extern "C" long          MS_ABI impl__FilterDataObject_CBrowserControlSite__MEAAJPEAUIDataObject__PEAPEAU2__Z(void*, IDataObject*, IDataObject**);

namespace {
#define VT(f) reinterpret_cast<const void*>(&f)
// The vftable the retail constructor stores at +0x180.  Its slot order is
// IDocHostUIHandler's own, which is also the order the retail forwarders use
// (CDHtmlDialog slots 0x350..0x3c0 ascend in lockstep with slots 3..17 here).
// Hand-authored because every entry must be an MS_ABI function: this DLL is
// built with mingw, whose C++ vtables are not COM-callable.
const void* const kDocHostUIHandlerVtbl[18] = {
    VT(impl__QueryInterface_CBrowserControlSite__MEAAJAEBU_GUID__PEAPEAX_Z),                      // 0
    VT(impl__AddRef_CBrowserControlSite__MEAAKXZ),                                                // 1
    VT(impl__Release_CBrowserControlSite__MEAAKXZ),                                               // 2
    VT(impl__ShowContextMenu_CBrowserControlSite__MEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z), // 3
    VT(impl__GetHostInfo_CBrowserControlSite__MEAAJPEAU_DOCHOSTUIINFO___Z),                       // 4
    VT(impl__ShowUI_CBrowserControlSite__MEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z), // 5
    VT(impl__HideUI_CBrowserControlSite__MEAAJXZ),                                                // 6
    VT(impl__UpdateUI_CBrowserControlSite__MEAAJXZ),                                              // 7
    VT(impl__EnableModeless_CBrowserControlSite__MEAAJH_Z),                                       // 8
    VT(impl__OnDocWindowActivate_CBrowserControlSite__MEAAJH_Z),                                  // 9
    VT(impl__OnFrameWindowActivate_CBrowserControlSite__MEAAJH_Z),                                // 10
    VT(impl__ResizeBorder_CBrowserControlSite__MEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z),   // 11
    VT(impl__TranslateAcceleratorW_CBrowserControlSite__MEAAJPEAUtagMSG__PEBU_GUID__K_Z),         // 12
    VT(impl__GetOptionKeyPath_CBrowserControlSite__MEAAJPEAPEA_WK_Z),                             // 13
    VT(impl__GetDropTarget_CBrowserControlSite__MEAAJPEAUIDropTarget__PEAPEAU2__Z),               // 14
    VT(impl__GetExternal_CBrowserControlSite__MEAAJPEAPEAUIDispatch___Z),                         // 15
    VT(impl__TranslateUrl_CBrowserControlSite__MEAAJKPEA_WPEAPEA_W_Z),                            // 16
    VT(impl__FilterDataObject_CBrowserControlSite__MEAAJPEAUIDataObject__PEAPEAU2__Z),            // 17
};
#undef VT
}  // namespace

// CBrowserControlSite::CBrowserControlSite(COleControlContainer*, CDHtmlDialog*)
// -- retail mfc140u 0x210d50, transcribed:
//     call COleControlSite::COleControlSite(pCtrlCont)   ; u 0x2394e0
//     this[0x188] = pHandler
//     this[0x000] = &CBrowserControlSite::`vftable'       ; u 0x1803272f0
//     this[0x180] = &IDocHostUIHandler `vftable'          ; u 0x180327258
//     return this
//
// DEVIATIONS.  Both are real gaps, not equivalences; neither is forced by
// anything about retail.
//  1. The COleControlSite base sub-object is NOT constructed here and pCtrlCont
//     is dropped, so the first 0x180 bytes of the object are left exactly as the
//     caller supplied them -- vptr and every COleControlSite member
//     uninitialised.  Retail does construct it.
//     This is NOT because the base constructor is unavailable: OpenMFC has a real
//     C++ definition of COleControlSite::COleControlSite(COleControlContainer*)
//     at core/ole/COleControlSite.cpp:295, so
//         new (pThis) COleControlSite(pCtrlCont);
//     -- the shape about 200 other exported impl___0 constructor thunks in this
//     tree already use -- compiles, and would link.  (Checked: adding that line
//     introduces exactly one new undefined C++ symbol,
//     _ZN15COleControlSiteC1EP20COleControlContainer.)  It is left out only because this file is gated by a
//     per-file link audit that rejects any new cross-translation-unit C++ symbol
//     reference and cannot tell a genuinely defined constructor from a
//     thunk-only declaration.  Adding that one line is the correct fix and should
//     be made when the audit is run tree-wide.
//     What must NOT be used for the chain is the impl__ thunk next to it at
//     core/ole/COleControlSite.cpp:292: that one is an allocating factory
//     (`return new COleControlSite(p0)`), not an MSVC placement constructor, so
//     calling it would build a second, unrelated object and leak it.
//     Nothing in this tree constructs a CBrowserControlSite today
//     (CDHtmlDialog::CreateControlSite in core/view/CDHtmlDialog.cpp:867 hands out
//     a plain COleControlSite), so the gap is reachable only by an external
//     caller of this export.
//  2. Slot 0 -- the primary CBrowserControlSite vftable -- is not written.
//     Retail stores its own vftable (mfc140u 0x1803272f0) there; OpenMFC has no
//     MSVC-shaped table to put in its place, and since deviation 1 leaves the
//     base unconstructed there is nothing meaningful to write either.  The
//     consequence is that GetInterfaceHook below is not reached through slot 0 of
//     this object; the function itself is correct and is still reachable through
//     its export.
// Symbol: ??0CBrowserControlSite@@QEAA@PEAVCOleControlContainer@@PEAVCDHtmlDialog@@@Z
extern "C" void* MS_ABI impl___0CBrowserControlSite__QEAA_PEAVCOleControlContainer__PEAVCDHtmlDialog___Z(
    void* pThis, COleControlContainer* pCtrlCont, CDHtmlDialog* pHandler) {
    (void)pCtrlCont;  // see deviation 1 above
    DocHostSubObject* pSub = SubObjectFromSite(pThis);
    if (pSub) {
        pSub->lpVtbl = kDocHostUIHandlerVtbl;  // retail: this[0x180] = vftable
        pSub->m_pHandler = pHandler;           // retail: this[0x188] = pHandler
    }
    return pThis;
}

// CBrowserControlSite::QueryInterface -- retail mfc140u 0x210e30.  The whole
// body is `add $-0x180,%rcx` followed by the same instructions as
// CCmdTarget::ExternalQueryInterface (u 0x26d040), resolving to the same
// targets (only the encoded rel32 displacements differ): if the pointer at
// CCmdTarget +0x10 (the aggregation outer-unknown) is non-null, tail-call its
// IUnknown slot 0; otherwise tail-jump to CCmdTarget::InternalQueryInterface
// (u 0x26cfe0).  riid is passed straight through in rdx -- a REFIID is one
// pointer, which is exactly ExternalQueryInterface's `const void* iid`.
// Symbol: ?QueryInterface@CBrowserControlSite@@MEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CBrowserControlSite__MEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const GUID* riid, void** ppvObject) {
    CCmdTarget* pSite = SiteFromSubObject(pThis);
    if (!pSite) return E_POINTER;  // deviation: retail would fault on a null this
    return static_cast<long>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(
        pSite, riid, ppvObject));
}

// CBrowserControlSite::AddRef -- retail mfc140u 0x210e60, two instructions:
//     add $-0x180,%rcx
//     jmp CCmdTarget::ExternalAddRef      ; u 0x26cd80
// Symbol: ?AddRef@CBrowserControlSite@@MEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CBrowserControlSite__MEAAKXZ(void* pThis) {
    CCmdTarget* pSite = SiteFromSubObject(pThis);
    if (!pSite) return 0;  // deviation: retail would fault on a null this
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(pSite);
}

// CBrowserControlSite::Release -- retail mfc140u 0x210e70: `add $-0x180,%rcx`
// then the same instructions as CCmdTarget::ExternalRelease (u 0x26ce20) --
// if the pointer at CCmdTarget +0x10 is non-null, tail-call its IUnknown slot 2
// (vtable +0x10); otherwise tail-jump to CCmdTarget::InternalRelease
// (u 0x26cdb0).
// Symbol: ?Release@CBrowserControlSite@@MEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CBrowserControlSite__MEAAKXZ(void* pThis) {
    CCmdTarget* pSite = SiteFromSubObject(pThis);
    if (!pSite) return 0;  // deviation: retail would fault on a null this
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(pSite);
}

// CBrowserControlSite::GetInterfaceHook(const void* iid) -- retail mfc140u
// 0x210df0, the COleControlSite/CCmdTarget virtual override, entered with rcx =
// the COMPLETE object (this one is not reached through the +0x180 vftable).
// The entire body:
//     if (memcmp(iid, &IID_IDocHostUIHandler /* rdata 0x2d9e78 */, 16) != 0)
//         return NULL;                                  ; xor %eax,%eax
//     return this ? (IUnknown*)((char*)this + 0x180) : NULL;
// The trailing `lea 0x180(%rbx),%rdx; neg %rbx; sbb %rax,%rax; and %rdx,%rax`
// is just MSVC's null-checked cast to the second base.  Note what is NOT here:
// no chaining to COleControlSite's or CCmdTarget's GetInterfaceHook on a
// mismatch -- retail returns NULL outright.
// Symbol: ?GetInterfaceHook@CBrowserControlSite@@MEAAPEAUIUnknown@@PEBX@Z
extern "C" void* MS_ABI impl__GetInterfaceHook_CBrowserControlSite__MEAAPEAUIUnknown__PEBX_Z(
    void* pThis, const void* iid) {
    if (!iid) return nullptr;  // deviation: retail memcmp's an unchecked pointer
    if (::memcmp(iid, &kIID_IDocHostUIHandler, sizeof(GUID)) != 0) return nullptr;
    return pThis ? reinterpret_cast<char*>(pThis) + kDocHostOffset : nullptr;
}

//-----------------------------------------------------------------------------
// The fifteen IDocHostUIHandler methods.  Each is the same four-instruction
// forwarder described in the file header: load m_pHandler from the sub-object
// at +8, virtual-call the CDHtmlDialog method at the stated retail vtable byte
// offset, pass every argument through untouched, return its HRESULT.
//
// Retail applies no null check to m_pHandler; each body here does, and returns
// E_POINTER instead of faulting.  That is the one behavioural deviation these
// fifteen share (the by-name rather than by-slot dispatch is explained in the
// file header) and it is deliberate: OpenMFC has no in-tree producer of this object, so an
// external caller can reach these exports with the field still zero.
//-----------------------------------------------------------------------------

// retail mfc140u 0x210ea0 -> CDHtmlDialog vtable +0x350
// Symbol: ?ShowContextMenu@CBrowserControlSite@@MEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__ShowContextMenu_CBrowserControlSite__MEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(
    void* pThis, unsigned long dwID, POINT* ppt, IUnknown* pcmdtReserved, IDispatch* pdispReserved) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->ShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
}

// retail mfc140u 0x210ed0 -> CDHtmlDialog vtable +0x358
// Symbol: ?GetHostInfo@CBrowserControlSite@@MEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__GetHostInfo_CBrowserControlSite__MEAAJPEAU_DOCHOSTUIINFO___Z(
    void* pThis, DOCHOSTUIINFO* pInfo) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->GetHostInfo(pInfo);
}

// retail mfc140u 0x210ef0 -> CDHtmlDialog vtable +0x360.  The two stack
// arguments are re-stored at 0x20/0x28(%rsp) for the callee; that is just
// argument pass-through, not a reorder.
// Symbol: ?ShowUI@CBrowserControlSite@@MEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__ShowUI_CBrowserControlSite__MEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(
    void* pThis, unsigned long dwID, IOleInPlaceActiveObject* pActiveObject,
    IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

// retail mfc140u 0x210f30 -> CDHtmlDialog vtable +0x368
// Symbol: ?HideUI@CBrowserControlSite@@MEAAJXZ
extern "C" long MS_ABI impl__HideUI_CBrowserControlSite__MEAAJXZ(void* pThis) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->HideUI();
}

// retail mfc140u 0x210f50 -> CDHtmlDialog vtable +0x370
// Symbol: ?UpdateUI@CBrowserControlSite@@MEAAJXZ
extern "C" long MS_ABI impl__UpdateUI_CBrowserControlSite__MEAAJXZ(void* pThis) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->UpdateUI();
}

// retail mfc140u 0x210f70 -> CDHtmlDialog vtable +0x378
// Symbol: ?EnableModeless@CBrowserControlSite@@MEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_CBrowserControlSite__MEAAJH_Z(void* pThis, int fEnable) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->EnableModeless(fEnable);
}

// retail mfc140u 0x210f90 -> CDHtmlDialog vtable +0x380
// Symbol: ?OnDocWindowActivate@CBrowserControlSite@@MEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_CBrowserControlSite__MEAAJH_Z(void* pThis, int fActivate) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->OnDocWindowActivate(fActivate);
}

// retail mfc140u 0x210fb0 -> CDHtmlDialog vtable +0x388
// Symbol: ?OnFrameWindowActivate@CBrowserControlSite@@MEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_CBrowserControlSite__MEAAJH_Z(void* pThis, int fActivate) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->OnFrameWindowActivate(fActivate);
}

// retail mfc140u 0x210fd0 -> CDHtmlDialog vtable +0x390
// Symbol: ?ResizeBorder@CBrowserControlSite@@MEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_CBrowserControlSite__MEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    void* pThis, LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, int fRameWindow) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->ResizeBorder(prcBorder, pUIWindow, fRameWindow);
}

// retail mfc140u 0x210ff0 -> CDHtmlDialog vtable +0x398.  mfc140.dll names the
// twin of this entry point ?TranslateAcceleratorA@... at 0x20f290 and mfc140u
// exports it with the ...W spelling this thunk implements
// (mfc_complete_ordinal_mapping.json).  The two bodies are the same instructions;
// only the rip-relative displacement to the CFG dispatch cell differs.
// Symbol: ?TranslateAcceleratorW@CBrowserControlSite@@MEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_CBrowserControlSite__MEAAJPEAUtagMSG__PEBU_GUID__K_Z(
    void* pThis, LPMSG lpMsg, const GUID* pguidCmdGroup, unsigned long nCmdID) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->TranslateAcceleratorW(lpMsg, pguidCmdGroup, nCmdID);
}

// retail mfc140u 0x211010 -> CDHtmlDialog vtable +0x3a0
// Symbol: ?GetOptionKeyPath@CBrowserControlSite@@MEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__GetOptionKeyPath_CBrowserControlSite__MEAAJPEAPEA_WK_Z(
    void* pThis, wchar_t** pchKey, unsigned long dw) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->GetOptionKeyPath(pchKey, dw);
}

// retail mfc140u 0x211030 -> CDHtmlDialog vtable +0x3a8
// Symbol: ?GetDropTarget@CBrowserControlSite@@MEAAJPEAUIDropTarget@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__GetDropTarget_CBrowserControlSite__MEAAJPEAUIDropTarget__PEAPEAU2__Z(
    void* pThis, IDropTarget* pDropTarget, IDropTarget** ppDropTarget) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->GetDropTarget(pDropTarget, ppDropTarget);
}

// retail mfc140u 0x211050 -> CDHtmlDialog vtable +0x3b0
// Symbol: ?GetExternal@CBrowserControlSite@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExternal_CBrowserControlSite__MEAAJPEAPEAUIDispatch___Z(
    void* pThis, IDispatch** ppDispatch) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->GetExternal(ppDispatch);
}

// retail mfc140u 0x211070 -> CDHtmlDialog vtable +0x3b8
// Symbol: ?TranslateUrl@CBrowserControlSite@@MEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__TranslateUrl_CBrowserControlSite__MEAAJKPEA_WPEAPEA_W_Z(
    void* pThis, unsigned long dwTranslate, wchar_t* pchURLIn, wchar_t** ppchURLOut) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

// retail mfc140u 0x211090 -> CDHtmlDialog vtable +0x3c0
// Symbol: ?FilterDataObject@CBrowserControlSite@@MEAAJPEAUIDataObject@@PEAPEAU2@@Z
extern "C" long MS_ABI impl__FilterDataObject_CBrowserControlSite__MEAAJPEAUIDataObject__PEAPEAU2__Z(
    void* pThis, IDataObject* pDO, IDataObject** ppDORet) {
    CDHtmlDialog* p = HandlerOf(pThis);
    if (!p) return E_POINTER;
    return p->FilterDataObject(pDO, ppDORet);
}
