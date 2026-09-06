// CDocObjectServer — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// ---------------------------------------------------------------------------
// CDocObjectServer's nested interface parts, decoded from retail mfc140.
//
// Retail object layout.  Every X* method opens with MFC's METHOD_PROLOGUE_EX,
// so each one reaches the owning CDocObjectServer at a fixed negative
// displacement from the interface sub-object it was invoked on.  Those
// displacements agree with the constructor at RVA 0x255da0 and with the
// shipping afxdocob.h declaration order (that header is on this host at
// atlmfc/include/afxdocob.h and was read; the member names and their order
// below are quoted from it, not guessed):
//
//   +0x00  CObject vptr                         (CCmdTarget base is 0x40 bytes)
//   +0x08  CCmdTarget::m_dwRef                  (EnumViews decrements it directly)
//   +0x38  CCmdTarget::m_pModuleState           (AFX_MANAGE_STATE argument)
//   +0x40  LPOLEDOCUMENTSITE  m_pDocSite        (ctor stores the 2nd argument)
//   +0x48  COleServerDoc*     m_pOwner          (ctor stores the 1st argument)
//   +0x50  LONG               m_nFirstPage      (ctor stores -1)
//   +0x58  LPCONTINUECALLBACK m_pContinueCallback
//   +0x60  LPOLEINPLACESITE   m_pViewSite       (ctor stores NULL)
//   +0x68  XOleObject         m_xOleObject       vtable RVA 0x32d1e8
//   +0x70  XOleDocument       m_xOleDocument     vtable RVA 0x32d2b0
//   +0x78  XOleDocumentView   m_xOleDocumentView vtable RVA 0x32d130
//   +0x80  XOleCommandTarget  m_xOleCommandTarget vtable RVA 0x32d1b8
//   +0x88  XPrint             m_xPrint           vtable RVA 0x32d0f8
//
// Total 0x90 = 144 bytes, which is the size this repo already records for the
// class (phase4/src/detail/COleControlModuleSupport.h: OR_DESC(CDocObjectServer,
// 144, ...)).  Each vtable RVA above was read out of .rdata with objdump -s and
// every slot was matched back to an exported method name; the slot numbers cited
// in the per-function comments below come from those dumps, not from guesswork.
//
// The retail constructor (0x255da0) does exactly: CCmdTarget::CCmdTarget()
// (0x1dc320), store the class vptr plus the five interface-part vptrs,
// m_pDocSite = pDocSite, m_pOwner = pDoc, m_pViewSite = NULL,
// m_nFirstPage = -1, and additionally `pDoc->[0x1c0] = 1` -- a BOOL inside the
// COleDocument/CDocument base of the owning document that has NOT been
// identified here.  The four data-member stores ARE reproduced below (see the
// constructor's own comment for exactly which parts are and are not), so
// m_pDocSite / m_pOwner / m_nFirstPage / m_pViewSite may be relied on by the
// bodies in this file.  The six vptr stores are not, so nothing here may
// dispatch through a CDocObjectServer or nested-interface-part vtable.
//
// Owning-document offsets used by the forwarders.  Retail reads several fields
// of *m_pOwner.  Those were named by reading the real afxole.h on this host:
// COleServerDoc's five BEGIN_INTERFACE_PART members are, in declaration order,
// PersistStorage / OleObject / DataObject / OleInPlaceObject /
// OleInPlaceActiveObject, each an 8-byte vptr-only sub-object.  The
// disassembly pins m_xOleObject at +0x278, so the run is +0x270 .. +0x290 and
// the trailing data members fall out of it:
//
//   m_pOwner + 0x250   COleIPFrameWnd* m_pInPlaceFrame
//   m_pOwner + 0x268   CDocObjectServer* m_pDocObjectServer
//   m_pOwner + 0x278   COleServerDoc::m_xOleObject          (IOleObject)
//   m_pOwner + 0x288   COleServerDoc::m_xOleInPlaceObject   (IOleInPlaceObject)
//
// Why most of these stay stubs.  The whole IOleObject part, plus much of
// IOleDocument / IOleDocumentView / IPrint, is a pure forwarder into the
// *owning document's* own nested interface parts at m_pOwner + 0x278 /
// m_pOwner + 0x288.  OpenMFC's COleServerDoc (include/openmfc/afxole.h) is a
// placeholder built from COleLinkingDoc plus `char _coleserverdoc_padding[96]`;
// it has no interface parts and no relationship to the retail layout, so
// `m_pOwner + 0x278` names nothing here.  (COleServerDoc's own IOleObject entry
// points do exist in this DLL as impl__ thunks in
// phase4/src/core/ole/COleServerDoc.cpp, and -- checked, not assumed -- their
// first parameter is the interface SUB-OBJECT pointer, `pThisItf`, exactly as
// retail's is; today none of them dereferences it.  The load-bearing objection
// is that there is no such sub-object to hand them: OpenMFC's COleServerDoc
// embeds no interface parts, so `m_pOwner + 0x278` is off the end of the object
// and forwarding would mean passing a pointer that becomes a fault the moment
// those thunks acquire real bodies.  Contrast OnSetItemRects / OnExecOleCmd
// below, whose thunks take a plain COleServerDoc* and ARE forwarded.)  Those
// forwards are therefore left as documented stubs.
//
// Where a forward's target IS reachable -- OnSetItemRects, SetRect,
// OnExecOleCmd and OnCloseDocument go to plain COleServerDoc/CDocument methods
// rather than to an interface part -- it is made, and the way it deviates from
// retail's virtual dispatch is spelled out at the function.
//
// Signatures.  Every entry point in this file now carries its real signature,
// with the interface sub-object pointer (`pThis`, RCX on entry) first.  The
// stub generator had emitted most of them WITHOUT the implicit `this`, so `p0`
// was silently receiving the interface pointer, every later argument was
// shifted by one, and the last one was dropped off the end.  That is corrected
// throughout; the bodies that are still stubs ignore every argument, so the
// parameter names below are documentation of the ABI and nothing more.
//
// Return values of the stubs.  The generated bodies return 0 = S_OK, and that
// was left alone because nothing here establishes a better answer.  Be aware
// that for the entry points with [out] parameters it is a hazard rather than a
// no-op: the caller is told the call succeeded and then reads an out pointer
// that was never written.  Where the retail body DOES pin the out parameter the
// store is reproduced and the return value follows it: CreateView and EnumViews
// NULL theirs unconditionally, and EnumViews / GetDocument report E_NOINTERFACE
// rather than S_OK when they have nothing to hand back.
// ---------------------------------------------------------------------------

// ---- sibling impl_ exports called by the bodies in this file ----
// (see the link rule at the top of the campaign briefing: C++ methods of other
// classes exist in this DLL only as impl__ thunks, so they must be called by
// their thunk name.  Every declaration below matches a definition that exists
// today -- detail/DocviewSupport.cpp, featurepack/outlookbar/, core/ole/
// RuntimeClasses.cpp, core/ole/COleServerDoc.cpp.)
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(
    const CView* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__OnSetItemRects_COleServerDoc__MEAAXPEBUtagRECT__0_Z(
    COleServerDoc* pThis, const RECT* lpPosRect, const RECT* lpClipRect);
extern "C" long MS_ABI impl__OnExecOleCmd_COleServerDoc__MEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    COleServerDoc* pThis, const GUID* pguidCmdGroup, unsigned long nCmdID,
    unsigned long nCmdExecOpt, void* pvarargIn, void* pvarargOut);
// CCmdTarget::GetInterface is a real, implemented body in this DLL
// (phase4/src/core/runtime/CCmdTarget.cpp:899), not a stub, and it touches only
// the interface map and the candidate sub-object's first dword -- no CCmdTarget
// data member.  CreateView and EnumViews below both go through it because that
// is the one CCmdTarget helper that is SAFE on a CDocObjectServer in this DLL:
// the constructor below deliberately does not run the CCmdTarget base
// construction, so m_dwRef (+0x08) and m_pOuterUnknown (+0x10) hold whatever
// the caller's storage held.
//
// ::ExternalQueryInterface -- which is what retail's XOleDocument::QueryInterface
// (vtable 0x32d2b0 slot 0 = 0x256e00) reduces to, METHOD_PROLOGUE_EX plus the
// m_pOuterUnknown test at 0x256e28 and the fall-through to
// InternalQueryInterface at 0x26bda0 -- is deliberately NOT called from this
// file.  Its OpenMFC body reads m_pOuterUnknown at server + 0x10 and, when that
// word is non-zero, calls QueryInterface through it; on an object whose
// CCmdTarget base was never constructed that is an indirect call through
// uninitialised storage.  EnumViews below therefore calls GetInterface and maps
// the result to S_OK / E_NOINTERFACE itself, which is what
// InternalQueryInterface does for the non-aggregated case, minus the reference
// counting that the same uninitialised-base argument rules out.
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
    CCmdTarget* pThis, const void* iid);
// AfxThrowInvalidArgException -- exported only as an impl__ thunk
// (phase4/src/detail/MfcExceptionsSupport.cpp:35, a real throw).  ApplyViewState
// and SaveViewState below reproduce retail's NULL-stream throw with it.
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

// CDocObjectServer member offsets, relative to the interface sub-object that
// the caller invoked.  Derived above; used only where retail itself uses them.
constexpr long kFirstPageFromXPrint = -0x38;   // m_nFirstPage (+0x50) from m_xPrint (+0x88)

// CDocObjectServer member offsets, relative to the CDocObjectServer itself.
// Every one of these is read or written by a retail body disassembled below;
// the constructor at 0x255da0 writes all four (see its transcription).
constexpr long kOff_m_pDocSite   = 0x40;
constexpr long kOff_m_pOwner     = 0x48;
constexpr long kOff_m_nFirstPage = 0x50;
constexpr long kOff_m_pViewSite  = 0x60;

// Offsets of the nested interface sub-objects inside the CDocObjectServer.
// A retail X* method reaches the server by subtracting its own part's offset;
// the displacements the disassembly actually uses are quoted per function.
constexpr long kOff_m_xOleDocument      = 0x70;
constexpr long kOff_m_xOleDocumentView  = 0x78;
constexpr long kOff_m_xOleCommandTarget = 0x80;

// IID_IOleDocumentView.  Read out of retail .rdata at 0x2d7ef0 -- the operand of
// the `lea` at 0x256ece (CreateView) and at 0x256fbd (EnumViews) -- with
// objdump -s; the sixteen bytes there are
//     c6 bc 22 b7  68 4e  1b 10  a2 bc 00 aa 00 40 47 70
// i.e. {B722BCC6-4E68-101B-A2BC-00AA00404770}.  Spelled out here rather than
// taken from the platform SDK because this DLL links no uuid library.
const GUID kIID_IOleDocumentView = {
    0xB722BCC6, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };

// IID_IUnknown.  Read the same way out of retail .rdata at 0x2d79a8 -- the
// operand of the `lea` at 0x258e28 (GetDocument) -- with objdump -s:
//     00 00 00 00  00 00  00 00  c0 00 00 00 00 00 00 46
// i.e. {00000000-0000-0000-C000-000000000046}.
const GUID kIID_IUnknown = {
    0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

// this (an interface sub-object) -> the owning CDocObjectServer.
inline void* ServerFromPart(void* pPart, long nPartOffset) {
    return static_cast<char*>(pPart) - nPartOffset;
}

// Typed access to a CDocObjectServer member at a fixed offset.  CDocObjectServer
// is not declared in include/openmfc, so there is no struct to go through: the
// offsets above are the only description of the layout this DLL has.
template <class T>
inline T& Member(void* pServer, long nOffset) {
    return *reinterpret_cast<T*>(static_cast<char*>(pServer) + nOffset);
}

}  // namespace

// ---------------------------------------------------------------------------
// CDocObjectServer's own (non-interface-part) entry points.
// ---------------------------------------------------------------------------

// Symbol: ??0CDocObjectServer@@QEAA@PEAVCOleServerDoc@@PEAUIOleDocumentSite@@@Z
// CDocObjectServer::CDocObjectServer(COleServerDoc*, IOleDocumentSite*) --
// retail (0x255da0), in program order:
//     CCmdTarget::CCmdTarget()            (call 0x1dc320)
//     *(void**)this            = &vtbl_CDocObjectServer   (.rdata 0x32d2e8)
//     *(void**)(this + 0x68)   = &vtbl_XOleObject         (.rdata 0x32d1e8)
//     *(void**)(this + 0x70)   = &vtbl_XOleDocument       (.rdata 0x32d2b0)
//     *(void**)(this + 0x78)   = &vtbl_XOleDocumentView   (.rdata 0x32d130)
//     *(void**)(this + 0x80)   = &vtbl_XOleCommandTarget  (.rdata 0x32d1b8)
//     *(void**)(this + 0x88)   = &vtbl_XPrint             (.rdata 0x32d0f8)
//     m_pDocSite   (+0x40) = pDocSite       (the 3rd register argument, R8)
//     m_pViewSite  (+0x60) = NULL
//     m_pOwner     (+0x48) = pDoc           (the 2nd register argument, RDX)
//     pDoc->[0x1c0]        = 1
//     m_nFirstPage (+0x50) = -1
//     return this
// The four data-member stores ARE reproduced below: they are inside the 144
// bytes this repo records for the class (detail/COleControlModuleSupport.h,
// OR_DESC(CDocObjectServer, 144, ...)).  Three of the four are read by an
// implemented body in this file -- m_pDocSite by ReleaseDocSite, m_pOwner by
// GetControllingFrame / OnCloseDocument / OnExecOleCmd / OnSetItemRects /
// SetRect, m_pViewSite by GetInPlaceSite.  m_nFirstPage is written by
// SetInitialPageNum but its only retail reader, GetPageInfo, is still a stub,
// so today nothing reads back the -1.
//
// Three things retail does are deliberately NOT reproduced, and no body in this
// file may assume them:
//   * the CCmdTarget base construction -- so m_dwRef (+0x08) and
//     m_pModuleState (+0x38) stay whatever the caller's storage held;
//   * the six vptr stores -- this DLL builds no vtable for CDocObjectServer or
//     for any of the five nested interface classes, so there is nothing to
//     store.  Anything that dispatches through one of those vtables therefore
//     stays a stub below;
//   * `pDoc->[0x1c0] = 1` -- that offset is inside the owning COleServerDoc,
//     whose OpenMFC layout (include/openmfc/afxole.h: COleLinkingDoc plus
//     `char _coleserverdoc_padding[96]`) is unrelated to the retail one, so the
//     store would land on an unidentified field.
extern "C" void* MS_ABI impl___0CDocObjectServer__QEAA_PEAVCOleServerDoc__PEAUIOleDocumentSite___Z(
    void* pThis, void* pDoc, void* pDocSite) {
    if (pThis == nullptr) {
        return pThis;   // deviation: retail has no such guard (it would fault)
    }
    Member<void*>(pThis, kOff_m_pDocSite) = pDocSite;
    Member<void*>(pThis, kOff_m_pViewSite) = nullptr;
    Member<void*>(pThis, kOff_m_pOwner) = pDoc;
    Member<LONG>(pThis, kOff_m_nFirstPage) = -1;
    return pThis;
}

// Symbol: ??1CDocObjectServer@@UEAA@XZ
// CDocObjectServer::~CDocObjectServer -- retail (0x255e90):
//     *(void**)this = &vtbl_CDocObjectServer   (.rdata 0x32d2e8)
//     CDocObjectServer::ReleaseDocSite(this)   (call 0x255ec0)
//     tail jump to CCmdTarget::~CCmdTarget     (0x1dc360)
// The ReleaseDocSite call is reproduced -- it is the release that pairs with the
// m_pDocSite the constructor above seeds.  The vptr store and the base
// destructor are not: this DLL has no CDocObjectServer vtable (see the
// constructor) and the constructor runs no CCmdTarget base construction, so
// running the base destructor here would tear down state that was never built.
extern "C" void MS_ABI impl__ReleaseDocSite_CDocObjectServer__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl___1CDocObjectServer__UEAA_XZ(void* pThis) {
    impl__ReleaseDocSite_CDocObjectServer__QEAAXXZ(pThis);
}

// STUB: CDocObjectServer::ActivateDocObject -- retail (0x255f50) is
//     if (m_pOwner->[0x268] != NULL)
//         m_pDocSite->vtbl[0x18/8 = 3](NULL);   // IOleDocumentSite::ActivateMe
// m_pOwner + 0x268 is COleServerDoc::m_pDocObjectServer (see the layout note at
// the top of this file).  OpenMFC's COleServerDoc has no such member and no
// relationship to the retail layout, so the guard cannot be evaluated.  It is
// left unimplemented rather than approximated: substituting `m_pDocSite != NULL`
// for it would call ActivateMe in the one case retail deliberately skips, and a
// spurious activation is worse than none.
// Symbol: ?ActivateDocObject@CDocObjectServer@@QEAAXXZ
extern "C" void MS_ABI impl__ActivateDocObject_CDocObjectServer__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// The five CDocObjectServer::Do* printing helpers are, in retail, one-instruction
// forwarders into a CView virtual.  Each was disassembled separately and each is
// literally `mov (%rdx),%rax; <shuffle the args down one>; jmp *0xNN(%rax)`:
// the helper's own `this` (RCX) is never touched, and the CView* it is handed
// becomes the receiver.  The slots, with the CView declaration order they match:
//     DoPrepareDC       0x256070 -> pView->vtbl[0x320/8 = 100]  OnPrepareDC
//     DoPreparePrinting 0x256050 -> pView->vtbl[0x350/8 = 106]  OnPreparePrinting
//     DoBeginPrinting   0x2560b0 -> pView->vtbl[0x358/8 = 107]  OnBeginPrinting
//     DoPrint           0x256090 -> pView->vtbl[0x360/8 = 108]  OnPrint
//     DoEndPrinting     0x2560d0 -> pView->vtbl[0x368/8 = 109]  OnEndPrinting
// (Slots 106..109 are consecutive and CView declares exactly
// OnPreparePrinting / OnBeginPrinting / OnPrint / OnEndPrinting in that order,
// which is what fixes the naming; OnPrepareDC is the earlier slot 100.)
//
// Three of the five are reproduced here as ordinary C++ virtual calls on the
// CView, which is how this repo models retail virtual dispatch (compare
// CDocument::UpdateAllViews in detail/DocviewSupport.cpp).  Two are not, because
// OpenMFC's CView (include/openmfc/afxwin.h) declares no OnPrepareDC and no
// OnPrint -- see the header request filed with this change.
//
// One deviation applies to all of them: retail dereferences pView with no null
// check (it would fault); the guards below return without dispatching instead.

// Symbol: ?DoBeginPrinting@CDocObjectServer@@IEAAXPEAVCView@@PEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__DoBeginPrinting_CDocObjectServer__IEAAXPEAVCView__PEAVCDC__PEAUCPrintInfo___Z(
    void* pThis, CView* pView, void* pDC, void* pInfo) {
    (void)pThis;   // retail ignores it too
    if (pView == nullptr) return;
    pView->OnBeginPrinting(pDC, pInfo);
}

// Symbol: ?DoEndPrinting@CDocObjectServer@@IEAAXPEAVCView@@PEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__DoEndPrinting_CDocObjectServer__IEAAXPEAVCView__PEAVCDC__PEAUCPrintInfo___Z(
    void* pThis, CView* pView, void* pDC, void* pInfo) {
    (void)pThis;   // retail ignores it too
    if (pView == nullptr) return;
    pView->OnEndPrinting(pDC, pInfo);
}

// STUB: retail (0x256070) tail-jumps to pView->vtbl[0x320/8 = 100], CView::
// OnPrepareDC(CDC*, CPrintInfo*).  OpenMFC's CView declares no OnPrepareDC, so
// there is no virtual to dispatch to and no impl__ thunk for one either.
// Symbol: ?DoPrepareDC@CDocObjectServer@@IEAAXPEAVCView@@PEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__DoPrepareDC_CDocObjectServer__IEAAXPEAVCView__PEAVCDC__PEAUCPrintInfo___Z(
    void* pThis, CView* pView, void* pDC, void* pInfo) {
    (void)pThis;
    (void)pView;
    (void)pDC;
    (void)pInfo;
}

// Symbol: ?DoPreparePrinting@CDocObjectServer@@IEAAHPEAVCView@@PEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__DoPreparePrinting_CDocObjectServer__IEAAHPEAVCView__PEAUCPrintInfo___Z(
    void* pThis, CView* pView, void* pInfo) {
    (void)pThis;   // retail ignores it too
    if (pView == nullptr) return 0;
    return pView->OnPreparePrinting(pInfo);
}

// STUB: retail (0x256090) tail-jumps to pView->vtbl[0x360/8 = 108], CView::
// OnPrint(CDC*, CPrintInfo*).  OpenMFC's CView declares no OnPrint, so there is
// no virtual to dispatch to and no impl__ thunk for one either.
// Symbol: ?DoPrint@CDocObjectServer@@IEAAXPEAVCView@@PEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__DoPrint_CDocObjectServer__IEAAXPEAVCView__PEAVCDC__PEAUCPrintInfo___Z(
    void* pThis, CView* pView, void* pDC, void* pInfo) {
    (void)pThis;
    (void)pView;
    (void)pDC;
    (void)pInfo;
}

// Symbol: ?GetControllingFrame@CDocObjectServer@@QEBAPEAVCOleDocIPFrameWnd@@XZ
// CDocObjectServer::GetControllingFrame() const -- retail (0x255fd0), used by
// XOleCommandTarget::QueryStatus and ::Exec below:
//     POSITION pos = m_pOwner->vtbl[0xe0/8 = 28]();       // GetFirstViewPosition
//     if (pos == NULL) return NULL;
//     CView* pView = m_pOwner->vtbl[0xe8/8 = 29](&pos);   // GetNextView
//     if (pView == NULL) return NULL;
//     CFrameWnd* pFrame = CWnd::GetParentFrame(pView);    // call 0x28c680
//     if (pFrame == NULL) return NULL;
//     if (!CObject::IsKindOf(pFrame, &classCOleDocIPFrameWnd))  // call 0x233310
//         return NULL;
//     return pFrame;
// Slots 28 and 29 were confirmed by dumping the vtable at .rdata 0x2efe28 --
// that is COleServerDocEx's (its slot 0 is ?GetThisClass@COleServerDocEx@@),
// whose leading slots are COleServerDoc's and CDocument's: +0xe0 holds
// ?GetFirstViewPosition@CDocument@@ and +0xe8 holds ?GetNextView@CDocument@@.  The CRuntimeClass the IsKindOf test uses is
// the one at .rdata 0x32cf30, whose name pointer (0x347318) reads
// "COleDocIPFrameWnd".
//
// Deviations, all of them forced and none of them silent:
//   * the two CDocument calls are ordinary C++ virtual calls here, which is how
//     this repo models retail virtual dispatch, but they go through OpenMFC's
//     CDocument vtable, not retail's slot 28/29;
//   * retail's CWnd::GetParentFrame is a non-virtual call, so the CView thunk
//     (which itself calls the qualified CWnd::GetParentFrame) is used;
//   * the m_pOwner null check has no counterpart in retail, which would fault.
extern "C" CFrameWnd* MS_ABI impl__GetControllingFrame_CDocObjectServer__QEBAPEAVCOleDocIPFrameWnd__XZ(
    const void* pThis) {
    if (pThis == nullptr) return nullptr;
    void* pServer = const_cast<void*>(pThis);
    COleServerDoc* pOwnerDoc = Member<COleServerDoc*>(pServer, kOff_m_pOwner);
    if (pOwnerDoc == nullptr) return nullptr;
    CDocument* pOwner = pOwnerDoc;   // single, non-virtual inheritance: same address

    void* pos = pOwner->GetFirstViewPosition();
    if (pos == nullptr) return nullptr;
    CView* pView = pOwner->GetNextView(pos);
    if (pView == nullptr) return nullptr;

    CFrameWnd* pFrame = impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(pView);
    if (pFrame == nullptr) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pFrame),
            impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pFrame;
}

// STUB: CDocObjectServer::OnActivateView -- retail (0x2586a0) is the in-place
// activation path and is far outside what this DLL can express.  What was read
// from the disassembly, so the next reader does not have to start over:
//     if (m_pOwner->[0x230] == NULL) return S_OK;         // 0x2586ce, early out
//     ...build three CStrings from the module state (0x3ab440 / +0x18)...
//     if (!m_pOwner->vtbl[0x330/8 = 102](&str)) goto fail;
//     AfxGetThread()-relative fetch (0x1345b0) feeding a 0xe005 dispatch
//         (0x1cbc00) with the three strings;
//     pViewSite = m_pViewSite (+0x60); if (pViewSite == NULL) goto fail;
//     pViewSite->AddRef(); if (pViewSite->vtbl[0x30/8 = 6]() != S_OK) goto fail;
//     pViewSite->vtbl[0x18/8 = 3](&hwnd)  ...  and on from there.
// It needs the retail COleServerDoc layout (+0x230, vtable slot 102), the
// in-place frame and the CWnd side of activation, none of which exist here.
// Symbol: ?OnActivateView@CDocObjectServer@@MEAAJXZ
extern "C" long MS_ABI impl__OnActivateView_CDocObjectServer__MEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnApplyViewState@CDocObjectServer@@MEAAXAEAVCArchive@@@Z
// CDocObjectServer::OnApplyViewState(CArchive&) -- the default implementation is
// EMPTY, and that is decoded, not assumed.  This export has no RVA of its own in
// the map, but the CDocObjectServer class vtable (.rdata 0x32d2e8) resolves it:
//     slot 22 (0x32d398) -> 0x2820      slot 23 (0x32d3a0) -> 0x2820
//     slot 24 (0x32d3a8) -> 0x2586a0    ?OnActivateView@CDocObjectServer@@
//     slot 25 (0x32d3b0) -> 0x255f20    ?OnCloseDocument@CDocObjectServer@@
// RVA 0x2820 disassembles to a single `ret` -- an empty void body that every
// such body in the image was COMDAT-folded into (the RVA map happens to name it
// ?UpdateModifiedFlag@CRichEditDoc@@UEAAXXZ, which is one of the folded-in
// bodies, not this one).  afxdocob.h on this host declares CDocObjectServer's
// new virtuals in the order OnApplyViewState, OnSaveViewState, OnActivateView,
// OnCloseDocument, and slots 24/25 carry those last two by name, so 22 is
// OnApplyViewState and 23 is OnSaveViewState.  Retail reads nothing from the
// archive and touches no member.
extern "C" void MS_ABI impl__OnApplyViewState_CDocObjectServer__MEAAXAEAVCArchive___Z(
    void* pThis, CArchive& ar) {
    (void)pThis;
    (void)ar;
}

// Symbol: ?OnCloseDocument@CDocObjectServer@@UEAAXXZ
// CDocObjectServer::OnCloseDocument() -- retail (0x255f20):
//     CDocObjectServer::ReleaseDocSite(this)      (call 0x255ec0)
//     tail jump to m_pOwner->vtbl[0x118/8 = 35]() // COleServerDoc::OnCloseDocument
// Slot 35 was confirmed by dumping the vtable at .rdata 0x2efe28 (that is
// COleServerDocEx's; its leading slots are COleServerDoc's): +0x118 = 0x2eff40
// holds ?OnCloseDocument@COleServerDoc@@UEAAXXZ.
// Deviations: the forward is an ordinary C++ virtual call through OpenMFC's
// CDocument vtable rather than retail slot 35, and m_pOwner is null-checked
// (retail would fault).
extern "C" void MS_ABI impl__OnCloseDocument_CDocObjectServer__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__ReleaseDocSite_CDocObjectServer__QEAAXXZ(pThis);
    COleServerDoc* pOwnerDoc = Member<COleServerDoc*>(pThis, kOff_m_pOwner);
    if (pOwnerDoc == nullptr) return;
    CDocument* pOwner = pOwnerDoc;   // single, non-virtual inheritance
    pOwner->OnCloseDocument();
}

// Symbol: ?OnExecOleCmd@CDocObjectServer@@IEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
// CDocObjectServer::OnExecOleCmd -- retail (0x255f80); XOleCommandTarget::Exec
// below calls this first:
//     if (m_pOwner == NULL) return E_NOTIMPL;            // 0x255f90, literal 0x80004001
//     return m_pOwner->vtbl[0x2c8/8 = 89](pguidCmdGroup, nCmdID, nCmdExecOpt,
//                                        pvarargIn, pvarargOut);
// Slot 89 is COleServerDoc::OnExecOleCmd: afxdocob.h's sibling header afxole.h
// declares `virtual HRESULT OnExecOleCmd(...)` on COleServerDoc (line 1241 of
// the copy on this host), and slot 89 of the vtable at .rdata 0x2efe28
// (COleServerDocEx's; its leading slots are COleServerDoc's) is 0xf540, a
// COMDAT-folded `mov $0x80004001,%eax; ret`.
//
// The forward IS reproduced, to COleServerDoc's own impl__ thunk
// (phase4/src/core/ole/COleServerDoc.cpp:727), which exists and returns
// E_NOTIMPL transcribed from the Unicode image.  (An earlier note here claimed
// "there is no impl__ thunk for one" and returned E_NOTIMPL unconditionally;
// that claim was false -- the thunk is exported and defined.)
// PARTIAL, in exactly one respect, the same one as OnSetItemRects below: retail
// dispatches virtually, so a document that overrides OnExecOleCmd is bypassed
// here.  m_pOwner is null-checked, which is retail's own first test.
extern "C" long MS_ABI impl__OnExecOleCmd_CDocObjectServer__IEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    void* pThis, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdExecOpt,
    VARIANTARG* pvarargIn, VARIANTARG* pvarargOut) {
    if (pThis == nullptr) {
        return E_NOTIMPL;   // deviation: retail has no such guard
    }
    COleServerDoc* pOwner = Member<COleServerDoc*>(pThis, kOff_m_pOwner);
    if (pOwner == nullptr) {
        return E_NOTIMPL;   // retail 0x255f90
    }
    return impl__OnExecOleCmd_COleServerDoc__MEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
        pOwner, pguidCmdGroup, nCmdID, nCmdExecOpt, pvarargIn, pvarargOut);
}

// Symbol: ?OnSaveViewState@CDocObjectServer@@MEAAXAEAVCArchive@@@Z
// CDocObjectServer::OnSaveViewState(CArchive&) -- empty, on exactly the evidence
// written out under OnApplyViewState above: class vtable slot 23 (.rdata
// 0x32d3a0) holds 0x2820, a single `ret`.  Retail writes nothing to the archive
// and touches no member.
extern "C" void MS_ABI impl__OnSaveViewState_CDocObjectServer__MEAAXAEAVCArchive___Z(
    void* pThis, CArchive& ar) {
    (void)pThis;
    (void)ar;
}

// Symbol: ?OnSetItemRects@CDocObjectServer@@IEAAXPEAUtagRECT@@0@Z
// CDocObjectServer::OnSetItemRects -- retail (0x258e60) is a single tail jump:
//     m_pOwner->vtbl[0x300/8 = 96](lpPosRect, lpClipRect)
// Slot 96 was confirmed by dumping the vtable at .rdata 0x2efe28 (that is
// COleServerDocEx's; its leading slots are COleServerDoc's): +0x300 = 0x2f0128
// holds ?OnSetItemRects@COleServerDoc@@MEAAXPEBUtagRECT@@0@Z.
// PARTIAL.  OpenMFC's COleServerDoc does not declare that virtual, so the
// forward is made to COleServerDoc's own impl__ thunk instead.  Two consequences
// are worth spelling out: a client override of OnSetItemRects is bypassed
// (retail dispatches virtually), and that thunk
// (phase4/src/core/ole/COleServerDoc.cpp) is itself an empty stub today, so this
// currently does nothing observable -- but it does pick up an implementation
// there automatically.  m_pOwner is null-checked; retail would fault.
extern "C" void MS_ABI impl__OnSetItemRects_CDocObjectServer__IEAAXPEAUtagRECT__0_Z(
    void* pThis, RECT* lpPosRect, RECT* lpClipRect) {
    if (pThis == nullptr) return;
    COleServerDoc* pOwner = Member<COleServerDoc*>(pThis, kOff_m_pOwner);
    if (pOwner == nullptr) return;
    impl__OnSetItemRects_COleServerDoc__MEAAXPEBUtagRECT__0_Z(pOwner, lpPosRect, lpClipRect);
}

// Symbol: ?ReleaseDocSite@CDocObjectServer@@QEAAXXZ
// CDocObjectServer::ReleaseDocSite() -- retail (0x255ec0), transcribed in full:
//     rcx = m_pDocSite (+0x40)
//     if (rcx != NULL) {
//         rcx->vtbl[0x10/8 = 2]();      // IUnknown::Release
//         m_pDocSite = NULL;            // 0x255edf, inside the if
//     }
// The store is genuinely inside the branch (the `je` at 0x255ed0 targets
// 0x255ee7, past it), which is observationally the same as an unconditional
// store.  Also called by the destructor above, by OnCloseDocument, by SetDocSite
// and -- in retail -- by XOleObject::SetClientSite before it re-QIs the new
// client site for IID_IOleDocumentSite.  The pThis null check is the only
// deviation; retail has none.
extern "C" void MS_ABI impl__ReleaseDocSite_CDocObjectServer__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    IUnknown*& pDocSite = Member<IUnknown*>(pThis, kOff_m_pDocSite);
    if (pDocSite != nullptr) {
        pDocSite->Release();
        pDocSite = nullptr;
    }
}

// Symbol: ?SetDocSite@CDocObjectServer@@QEAAXPEAUIOleDocumentSite@@@Z
// CDocObjectServer::SetDocSite(IOleDocumentSite*) -- retail (0x255ef0), the
// whole body:
//     CDocObjectServer::ReleaseDocSite(this)   (call 0x255ec0)
//     m_pDocSite (+0x40) = pDocSite
// Note that retail does NOT AddRef the incoming pointer even though
// ReleaseDocSite releases the old one -- the caller's reference is taken over.
// That asymmetry is retail's, not a simplification here.  The pThis null check
// is the only deviation.
extern "C" void MS_ABI impl__SetDocSite_CDocObjectServer__QEAAXPEAUIOleDocumentSite___Z(
    void* pThis, IOleDocumentSite* pDocSite) {
    if (pThis == nullptr) return;
    impl__ReleaseDocSite_CDocObjectServer__QEAAXXZ(pThis);
    Member<IOleDocumentSite*>(pThis, kOff_m_pDocSite) = pDocSite;
}

// Forward declarations: CloseView dispatches through this same interface's
// vtable, and inside this DLL those entry points exist only as impl__ thunks.
extern "C" long MS_ABI impl__Show_XOleDocumentView_CDocObjectServer__UEAAJH_Z(void* pThis, int bShow);
extern "C" long MS_ABI impl__SetInPlaceSite_XOleDocumentView_CDocObjectServer__UEAAJPEAUIOleInPlaceSite___Z(
    void* pThis, IOleInPlaceSite* pSite);

// STUB: retail (0x257800) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x98/8 = 19 = IOleObject::Advise] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?Advise@XOleObject@CDocObjectServer@@UEAAJPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XOleObject_CDocObjectServer__UEAAJPEAUIAdviseSink__PEAK_Z(
    void* pThis, IAdviseSink* pAdvSink, unsigned long* pdwConnection) {
    (void)pThis;
    (void)pAdvSink;
    (void)pdwConnection;
    return 0;
}

// STUB: retail (0x259280).  Whole body, in program order:
//     AFX_MANAGE_STATE(server + 0x38)          // server = this - 0x78
//     COleStreamFile file;                     // ctor 0x263ac0, (&file, NULL)
//     if (pstm == NULL) AfxThrowInvalidArgException();   // test 0x2592b5,
//                                              //   call 0x225b80 at 0x2593bc
//     hr = S_OK                                // xor %ebx,%ebx, 0x2592be --
//                                              //   ONLY on the non-NULL path
//     file[+0x28] = pstm; file[+0x10] = 0      // 0x2592c0/0x2592c5, no AddRef;
//                                              //   +0x28 is cleared again at
//                                              //   0x259332 before the file is
//                                              //   destroyed, which is why the
//                                              //   destructor never Releases it
//     CFileException fe(0, -1, NULL);          // ctor 0x21cbe8, unexported
//     CArchive ar(&file, 3, 0x1000, NULL);     // ctor 0x1cf500; nMode 3 =
//                                              //   CArchive::load|bNoFlushOnDelete
//     server->vtbl[0xb0/8 = 22](&ar);          // OnApplyViewState
//     ar.Close()                               // 0x1cfb90
//     ~CArchive(); ~fe; ~COleStreamFile();     // 0x1cf6b0 / inline / 0x263c30
//     return hr
// The local at 0x80(%rsp) is named CFileException on evidence, not on a guess:
// its constructor (0x21cbe8) writes m_bAutoDelete=1 at +0x08, a vptr at +0x00,
// its 2nd argument to +0x10 and its 3rd to +0x14, and builds a CString at +0x18
// from its 4th; the caller passes (0, -1, NULL).  That is exactly
// CFileException(int cause, LONG lOsError, LPCTSTR lpszFileName) with all three
// of its default arguments, over the layout m_cause/m_lOsError/m_strFileName.
// The `lock xadd` near 0x25936c is that CString's CStringData::nRefs (m_pszData
// at +0x18, minus 0x18, plus 0x10) being released by the destructor.
// Slot 22 is OnApplyViewState: the class vtable at .rdata 0x32d2e8 has
// OnActivateView (0x2586a0) at slot 24 and OnCloseDocument (0x255f20) at 25,
// and afxdocob.h on this host declares CDocObjectServer's new virtuals in the
// order OnApplyViewState, OnSaveViewState, OnActivateView, OnCloseDocument --
// so 22 = OnApplyViewState and 23 = OnSaveViewState.  (Slots 22 and 23 both
// hold 0x2820, an empty void body those two were COMDAT-folded into, so the
// RVA map cannot name them on its own.)
//
// The catch block, and where the NULL-stream throw goes.  A previous note here
// claimed "the AfxThrowInvalidArgException does NOT escape to the caller --
// there is a catch continuation at 0x259339 that converts it to an HRESULT".
// That is WRONG, and the disassembly refutes it: 0x259339 is
// `mov 0x130(%rsp),%ebx` falling straight into 0x259340, which destroys the
// CArchive at rsp+0xa0, then the CFileException at rsp+0x80 (it stores that
// object's vptr at 0x25934e and releases its CString at 0x25936c), then the
// COleStreamFile.  On the NULL-pstm path NONE of those exist: the CFileException
// constructor runs at 0x2592da and the CArchive constructor at 0x2592fc, both
// AFTER the `test %rsi,%rsi; je 0x2593bc` at 0x2592b5.  So 0x259339 can only be
// the continuation of the try/catch around the CArchive + OnApplyViewState leg;
// no path in [0x259280, 0x2593c2) catches the argument throw, and 0x2593bc is
// `call 0x225b80` followed by int3 padding, i.e. noreturn.  The function's
// personality routine is __CxxFrameHandler4 (unwind info at .rdata 0x38e9ec ->
// handler 0x2b5ba0, an import thunk for VCRUNTIME140_1!__CxxFrameHandler4), so
// this is a genuine C++ try/catch and the unwind funclets that destroy the
// COleStreamFile on the throw path live outside the function body.
// CONCLUSION: ApplyViewState(NULL) THROWS out to the caller.  That much is
// decoded and is reproduced below.  What is still unknown is the HRESULT the
// catch funclet stores at 0x130(%rsp) -- but that leg is entered only if the
// CArchive work throws, and for a stock CDocObjectServer OnApplyViewState is an
// empty body (see its entry above), so it cannot be reached.
// PARTIAL, therefore, in exactly one respect that is named rather than papered
// over: the COleStreamFile + CArchive wrapper and the virtual dispatch to
// OnApplyViewState are not reproduced (that needs CArchive-over-IStream plumbing
// and a real CDocObjectServer vtable, neither of which this DLL builds).  For
// the base class that is observationally nothing -- retail reads no byte of the
// stream and touches no member -- so S_OK is retail's answer here; for a derived
// class that overrides OnApplyViewState it is a silent skip.
// Symbol: ?ApplyViewState@XOleDocumentView@CDocObjectServer@@UEAAJPEAUIStream@@@Z
extern "C" long MS_ABI impl__ApplyViewState_XOleDocumentView_CDocObjectServer__UEAAJPEAUIStream___Z(
    void* pThis, IStream* pstm) {
    (void)pThis;
    if (pstm == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();     // retail 0x2592b8/0x2593bc
    }
    return S_OK;                                        // retail 0x2592be, hr = 0
}

// Symbol: ?Clone@XOleDocumentView@CDocObjectServer@@UEAAJPEAUIOleInPlaceSite@@PEAPEAUIOleDocumentView@@@Z
// IOleDocumentView::Clone.  This export has no RVA of its own in the map, but
// slot 15 of the XOleDocumentView vtable (.rdata 0x18032d1a8, inside the vtable
// at 0x18032d130) holds 0x180258f50 -- byte-identical to the SetRectComplex
// slot at 0x18032d170, i.e. the two bodies were COMDAT-folded.  That body
// (disassembled below, under SetRectComplex) is AFX_MANAGE_STATE followed by
// `return E_NOTIMPL`.
extern "C" long MS_ABI impl__Clone_XOleDocumentView_CDocObjectServer__UEAAJPEAUIOleInPlaceSite__PEAPEAUIOleDocumentView___Z(
    void* pThis, IOleInPlaceSite* pIPSiteNew, IOleDocumentView** ppViewNew) {
    (void)pThis;
    (void)pIPSiteNew;
    (void)ppViewNew;
    return E_NOTIMPL;
}

// STUB: retail (0x2572b0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x30/8 = 6 = IOleObject::Close] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?Close@XOleObject@CDocObjectServer@@UEAAJK@Z
extern "C" long MS_ABI impl__Close_XOleObject_CDocObjectServer__UEAAJK_Z(
    void* pThis, unsigned long dwSaveOption) {
    (void)pThis;
    (void)dwSaveOption;
    return 0;
}

// Symbol: ?CloseView@XOleDocumentView@CDocObjectServer@@UEAAJK@Z
// IOleDocumentView::CloseView -- retail (0x2590d0):
//     AFX_MANAGE_STATE(this[-0x40])
//     this->vtbl[0x48/8 = 9](this, 0)     -> Show(FALSE)
//     this->vtbl[0x18/8 = 3](this, 0)     -> SetInPlaceSite(NULL)
//     return (HRESULT of SetInPlaceSite)  -- eax is not reloaded afterwards
// dwReserved is ignored.  Slots 9 and 3 of the XOleDocumentView vtable
// (.rdata RVA 0x32d130) hold 0x258f80 (Show) and 0x258d10 (SetInPlaceSite);
// the dispatch is transcribed as direct calls because XOleDocumentView is a
// private nested class with a single vtable, so nothing can override those
// slots.
// NOTE: both callees are still stubs below (they need the owning document's
// retail interface-part layout), so today this reduces to returning S_OK.
extern "C" long MS_ABI impl__CloseView_XOleDocumentView_CDocObjectServer__UEAAJK_Z(
    void* pThis, DWORD dwReserved) {
    (void)dwReserved;
    impl__Show_XOleDocumentView_CDocObjectServer__UEAAJH_Z(pThis, FALSE);
    return impl__SetInPlaceSite_XOleDocumentView_CDocObjectServer__UEAAJPEAUIOleInPlaceSite___Z(
        pThis, nullptr);
}

// Symbol: ?CreateView@XOleDocument@CDocObjectServer@@UEAAJPEAUIOleInPlaceSite@@PEAUIStream@@KPEAPEAUIOleDocumentView@@@Z
// IOleDocument::CreateView -- retail (0x256e80), transcribed in program order.
// m_xOleDocument sits at server + 0x70, which is the -0x70 of the
// `lea -0x70(%rcx),%rdi` at 0x256e8d:
//     AFX_MANAGE_STATE(server + 0x38)
//     hr = E_FAIL                          // 0x80004005 into esi, 0x256eb0
//     *ppView = NULL                       // 0x256eb5, unconditional
//     if (dwReserved != 0)     goto done   // 0x256ebc
//     if (m_pDocSite == NULL)  goto done   // 0x256ec0, server + 0x40
//     if (m_pViewSite != NULL) goto done   // 0x256ec7, server + 0x60
//     pView = CCmdTarget::GetInterface(server, &IID_IOleDocumentView)  // 0x256ed8
//     hr = pView->SetInPlaceSite(pIPSite)              // slot 0x18/8 = 3
//     if (hr == S_OK) { pView->AddRef(); *ppView = pView; }  // slot 0x8/8 = 1
//     if (pstm != NULL) hr = pView->ApplyViewState(pstm)     // slot 0x70/8 = 14
//   done:
//     return hr
// Two things here are easy to misread and are stated deliberately:
//   * the ApplyViewState leg runs even when SetInPlaceSite FAILED -- the `jne`
//     at 0x256ef7 skips only the AddRef and the store into *ppView and lands on
//     the `test %rbp,%rbp` at 0x256f0c -- and its HRESULT then overwrites hr;
//   * the AddRef is guarded by `test %eax,%eax; jne`, i.e. exactly S_OK, not
//     SUCCEEDED().
// Slots 3 and 14 are IOleDocumentView's declaration order (SetInPlaceSite,
// GetInPlaceSite, GetDocument, SetRect, GetRect, SetRectComplex, Show,
// UIActivate, Open, CloseView, SaveViewState, ApplyViewState, Clone); the same
// numbering puts Clone at slot 15, which is where the Clone entry above found
// it in the XOleDocumentView vtable.
// PARTIAL, in exactly one place, and it is the last step this DLL can take:
// retail dereferences the GetInterface result with no null check.
// CCmdTarget::GetInterface is a real implementation here
// (phase4/src/core/runtime/CCmdTarget.cpp), but every g_imap_* in
// detail/InterfaceMapsSupport.cpp is still the bare g_ifaceEnd terminator, so it
// returns NULL for every IID and this body stops there and returns the seeded
// E_FAIL.  The rest of the path is written out and will start running by itself
// once those maps carry CDocObjectServer's nested parts.  The module-state push
// is not reproduced (uniform across this file), and pThis/ppView are
// null-checked, which retail does not do.
extern "C" long MS_ABI impl__CreateView_XOleDocument_CDocObjectServer__UEAAJPEAUIOleInPlaceSite__PEAUIStream__KPEAPEAUIOleDocumentView___Z(
    void* pThis, IOleInPlaceSite* pIPSite, IStream* pstm, unsigned long dwReserved,
    IOleDocumentView** ppView) {
    long hr = E_FAIL;                                   // 0x256eb0
    if (ppView == nullptr || pThis == nullptr) {
        return hr;   // deviation: retail has no such guard (it would fault)
    }
    *ppView = nullptr;                                  // 0x256eb5
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocument);
    if (dwReserved != 0) return hr;                                      // 0x256ebc
    if (Member<void*>(pServer, kOff_m_pDocSite) == nullptr) return hr;   // 0x256ec0
    if (Member<void*>(pServer, kOff_m_pViewSite) != nullptr) return hr;  // 0x256ec7

    IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
        reinterpret_cast<CCmdTarget*>(pServer), &kIID_IOleDocumentView);
    if (pUnk == nullptr) {
        // Deviation, named above: retail assumes the interface map answers.
        return hr;
    }
    IOleDocumentView* pView = static_cast<IOleDocumentView*>(pUnk);
    hr = pView->SetInPlaceSite(pIPSite);
    if (hr == S_OK) {
        pView->AddRef();
        *ppView = pView;
    }
    if (pstm != nullptr) {
        hr = pView->ApplyViewState(pstm);
    }
    return hr;
}

// STUB: retail (0x2574b0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x58/8 = 11 = IOleObject::DoVerb] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?DoVerb@XOleObject@CDocObjectServer@@UEAAJJPEAUtagMSG@@PEAUIOleClientSite@@JPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" long MS_ABI impl__DoVerb_XOleObject_CDocObjectServer__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(
    void* pThis, long iVerb, MSG* lpmsg, IOleClientSite* pActiveSite, long lindex,
    HWND hwndParent, const RECT* lprcPosRect) {
    (void)pThis;
    (void)iVerb;
    (void)lpmsg;
    (void)pActiveSite;
    (void)lindex;
    (void)hwndParent;
    (void)lprcPosRect;
    return 0;
}

// STUB: retail (0x2578d0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0xa8/8 = 21 = IOleObject::EnumAdvise] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?EnumAdvise@XOleObject@CDocObjectServer@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_CDocObjectServer__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, IEnumSTATDATA** ppenumAdvise) {
    (void)pThis;
    (void)ppenumAdvise;
    return 0;
}

// STUB: retail (0x257540) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x60/8 = 12 = IOleObject::EnumVerbs] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?EnumVerbs@XOleObject@CDocObjectServer@@UEAAJPEAPEAUIEnumOLEVERB@@@Z
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_CDocObjectServer__UEAAJPEAPEAUIEnumOLEVERB___Z(
    void* pThis, IEnumOLEVERB** ppenumOleVerb) {
    (void)pThis;
    (void)ppenumOleVerb;
    return 0;
}

// Symbol: ?EnumViews@XOleDocument@CDocObjectServer@@UEAAJPEAPEAUIEnumOleDocumentViews@@PEAPEAUIOleDocumentView@@@Z
// IOleDocument::EnumViews -- retail (0x256f90).  A CDocObjectServer has exactly
// one view, so this does not build an enumerator at all:
//     AFX_MANAGE_STATE(server + 0x38)      // this - 0x38, m_xOleDocument = +0x70
//     *ppEnum = NULL                       // 0x256fb6, unconditional
//     hr = this->vtbl[0](&IID_IOleDocumentView, ppView)   // slot 0, 0x256fd0
//     this->vtbl[8/8 = 1]()                // XOleDocument::AddRef, 0x256fe2
//     lock decl -0x68(%rsi)                // 0x256fed
//     return hr
// The last two steps do NOT cancel out.  XOleDocument::AddRef (0x256d60,
// unexported, reached through vtable 0x32d2b0 slot 1) was disassembled: it
// reads m_pOwner at CDocObjectServer + 0x48 (`mov -0x28(%rbx),%rcx` at
// 0x256d78) and calls CCmdTarget::ExternalAddRef (0x26bb40) on the OWNING
// DOCUMENT, while the `lock decl` at 0x256fed decrements CDocObjectServer's own
// CCmdTarget::m_dwRef at +0x08 -- the count the successful QueryInterface had
// just raised.  Net effect: the view handed back is kept alive by the document's
// reference count, not by the server's.
// PARTIAL.  The two stores retail makes unconditionally are reproduced:
// *ppEnum = NULL, and *ppView written by the QueryInterface (NULL on failure --
// retail's InternalQueryInterface assigns the GetInterface result through the
// out parameter before testing it).
// How the QI is reproduced, and why NOT the obvious way.  Retail's
// XOleDocument::QueryInterface (vtable slot 0 = 0x256e00) is METHOD_PROLOGUE_EX
// plus an inlined CCmdTarget::ExternalQueryInterface, so calling this DLL's
// impl__ExternalQueryInterface_CCmdTarget thunk looks like the exact
// transcription -- and it is NOT SAFE HERE.  That body reads m_pOuterUnknown at
// server + 0x10 and, when the word is non-zero, calls QueryInterface through it;
// the constructor above deliberately does not run the CCmdTarget base
// construction, so +0x10 holds whatever the caller's storage held and that is an
// indirect call through uninitialised memory.  (This is the same objection that
// rules out the `lock decl` on m_dwRef at +0x08, below.)  What is reproduced
// instead is the non-aggregated leg that ExternalQueryInterface falls through
// to: CCmdTarget::GetInterface, which reads only the interface map and the
// candidate sub-object's first dword, plus the S_OK / E_NOINTERFACE mapping
// InternalQueryInterface applies to its result.
// In this DLL GetInterface answers NULL for every IID (g_imap_CDocObjectServer
// in phase4/src/detail/InterfaceMapsSupport.cpp is the bare terminator), so
// today this reports E_NOINTERFACE with *ppView NULL where retail returns S_OK.
// The AddRef/decrement pair is deliberately NOT reproduced, for two independent
// reasons: the QI cannot succeed here, so raising the owning document's
// reference count would be a straight leak with no view handed out to balance
// it; and, again, this DLL's CDocObjectServer constructor does not run the
// CCmdTarget base construction, so m_dwRef at +0x08 is not initialised and must
// not be decremented.  The AddRef that retail's InternalQueryInterface itself
// performs on a successful QI is skipped for exactly the same reason (it is
// CCmdTarget::ExternalAddRef, which also reads +0x10).
extern "C" long MS_ABI impl__EnumViews_XOleDocument_CDocObjectServer__UEAAJPEAPEAUIEnumOleDocumentViews__PEAPEAUIOleDocumentView___Z(
    void* pThis, void** ppEnum /* IEnumOleDocumentViews** */,
    IOleDocumentView** ppView) {
    if (pThis == nullptr || ppEnum == nullptr) {
        return E_POINTER;   // deviation: retail stores through ppEnum unchecked
    }
    *ppEnum = nullptr;                                  // 0x256fb6
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocument);
    IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
        reinterpret_cast<CCmdTarget*>(pServer), &kIID_IOleDocumentView);
    if (ppView != nullptr) {
        *ppView = static_cast<IOleDocumentView*>(pUnk);
    }
    return pUnk != nullptr ? S_OK : E_NOINTERFACE;
}

// Symbol: ?Exec@XOleCommandTarget@CDocObjectServer@@UEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
// IOleCommandTarget::Exec -- retail (0x258230); m_xOleCommandTarget sits at
// server + 0x80, which is the -0x80 the disassembly uses:
//     hr = CDocObjectServer::OnExecOleCmd(server, ...)   (call 0x255f80)
//     if (hr == E_NOTIMPL)
//         hr = helper_258128(CDocObjectServer::GetControllingFrame(server), ...)
//     return hr
// The helper at 0x258128 (no exported symbol) was disassembled as well:
//     if (pTarget == NULL)             return 0x80040102;  // OLECMDERR_E_NOHELP
//     COleCmdUI cmdUI(prgCmd, 1, pguidCmdGroup);           // ctor 0x257b20
//     if (nCmdExecOpt == 3)            return 0x80040101;  // OLECMDERR_E_DISABLED
//     if (!cmdUI.DoUpdate(pTarget, TRUE))                  // 0x257c50
//                                      return 0x80040100;  // OLECMDERR_E_NOTSUPPORTED
//     if (!(cmdUI.m_flags & 2))        return 0x80040101;  // OLECMDERR_E_DISABLED
//     return pTarget->vtbl[0x28/8 = 5](...) ? S_OK : E_FAIL;   // OnCmdMsg
// PARTIAL.  Everything above except the last three lines is reproduced.
// COleCmdUI::DoUpdate routes the command through CCmdTarget::OnCmdMsg with the
// CN_OLECOMMAND notify code (0xfffffffd in the disassembly at 0x257c7b); this
// DLL implements no such routing, so no command can ever find a handler, and the
// answer retail gives in exactly that case is OLECMDERR_E_NOTSUPPORTED.  That is
// what is returned here.  The gap is real and named: a frame that WOULD have
// handled the command in retail is not consulted.
extern "C" long MS_ABI impl__Exec_XOleCommandTarget_CDocObjectServer__UEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    void* pThis, const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdExecOpt,
    VARIANTARG* pvarargIn, VARIANTARG* pvarargOut) {
    if (pThis == nullptr) {
        return E_NOTIMPL;   // deviation: retail has no such guard
    }
    void* pServer = ServerFromPart(pThis, kOff_m_xOleCommandTarget);

    long hr = impl__OnExecOleCmd_CDocObjectServer__IEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
        pServer, pguidCmdGroup, nCmdID, nCmdExecOpt, pvarargIn, pvarargOut);
    if (hr != E_NOTIMPL) {
        return hr;
    }

    CFrameWnd* pTarget =
        impl__GetControllingFrame_CDocObjectServer__QEBAPEAVCOleDocIPFrameWnd__XZ(pServer);
    if (pTarget == nullptr) {
        return OLECMDERR_E_NOHELP;        // 0x80040102, helper 0x258128 at 0x25813f
    }
    if (nCmdExecOpt == OLECMDEXECOPT_SHOWHELP) {
        return OLECMDERR_E_DISABLED;      // 0x80040101, helper 0x258128 at 0x2581e0
    }
    // COleCmdUI::DoUpdate is not implemented in this DLL -- see the note above.
    return OLECMDERR_E_NOTSUPPORTED;      // 0x80040100, helper 0x258128 at 0x25819e
}

// STUB: retail (0x2571e0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x20/8 = 4 = IOleObject::GetClientSite] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetClientSite@XOleObject@CDocObjectServer@@UEAAJPEAPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__GetClientSite_XOleObject_CDocObjectServer__UEAAJPEAPEAUIOleClientSite___Z(
    void* pThis, IOleClientSite** ppClientSite) {
    (void)pThis;
    (void)ppClientSite;
    return 0;
}

// STUB: retail (0x257440) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x50/8 = 10 = IOleObject::GetClipboardData] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetClipboardData@XOleObject@CDocObjectServer@@UEAAJKPEAPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_CDocObjectServer__UEAAJKPEAPEAUIDataObject___Z(
    void* pThis, unsigned long dwReserved, IDataObject** ppDataObject) {
    (void)pThis;
    (void)dwReserved;
    (void)ppDataObject;
    return 0;
}

// Symbol: ?GetDocMiscStatus@XOleDocument@CDocObjectServer@@UEAAJPEAK@Z
// IOleDocument::GetDocMiscStatus -- retail (0x256f50):
//     AFX_MANAGE_STATE(this[-0x38]); movl $0x0,(%rdx); xor %eax,%eax; ret
// The whole body is "store zero through the out parameter, return S_OK"; no
// member of the server or of the owning document is consulted.  Retail does not
// null-check pdwStatus (it would fault); the guard here is the only deviation.
extern "C" long MS_ABI impl__GetDocMiscStatus_XOleDocument_CDocObjectServer__UEAAJPEAK_Z(
    void* pThis, DWORD* pdwStatus) {
    (void)pThis;
    if (pdwStatus == nullptr) {
        return E_POINTER;
    }
    *pdwStatus = 0;
    return S_OK;
}

// Symbol: ?GetDocument@XOleDocumentView@CDocObjectServer@@UEAAJPEAPEAUIUnknown@@@Z
// IOleDocumentView::GetDocument -- retail (0x258e00), the whole body:
//     AFX_MANAGE_STATE(this[-0x40])                 // server + 0x38
//     rcx = this - 0x8                              // the SIBLING m_xOleDocument
//                                                   //   part: 0x78 - 0x8 = 0x70
//     return rcx->vtbl[0](&IID_IUnknown, ppunk)     // slot 0 = QueryInterface
// The IID at .rdata 0x2d79a8 was dumped with objdump -s and reads
// {00000000-0000-0000-C000-000000000046} = IID_IUnknown.
// PARTIAL, in exactly the same one respect as EnumViews above, and reproduced
// the same way.  XOleDocument::QueryInterface (vtable 0x32d2b0 slot 0 =
// 0x256e00) is METHOD_PROLOGUE_EX plus an inlined CCmdTarget::
// ExternalQueryInterface, whose OpenMFC body reads m_pOuterUnknown at
// server + 0x10 -- storage this DLL's CDocObjectServer constructor never
// initialises -- so the non-aggregated leg is reproduced directly instead:
// CCmdTarget::GetInterface plus InternalQueryInterface's S_OK / E_NOINTERFACE
// mapping and its unconditional store through the out parameter.  The
// ExternalAddRef on success is skipped for the same reason (it reads +0x10 too).
// With g_imap_CDocObjectServer still the bare terminator, GetInterface answers
// NULL for every IID including IID_IUnknown, so today this returns E_NOINTERFACE
// with *ppunk NULL where retail returns S_OK.  That is strictly better than the
// stub it replaces, which returned S_OK and never wrote *ppunk at all.
extern "C" long MS_ABI impl__GetDocument_XOleDocumentView_CDocObjectServer__UEAAJPEAPEAUIUnknown___Z(
    void* pThis, IUnknown** ppunk) {
    if (pThis == nullptr || ppunk == nullptr) {
        return E_POINTER;   // deviation: retail passes ppunk straight through
    }
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
    IUnknown* pUnk = impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(
        reinterpret_cast<CCmdTarget*>(pServer), &kIID_IUnknown);
    *ppunk = pUnk;
    return pUnk != nullptr ? S_OK : E_NOINTERFACE;
}

// STUB: retail (0x257790) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x90/8 = 18 = IOleObject::GetExtent] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetExtent@XOleObject@CDocObjectServer@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetExtent_XOleObject_CDocObjectServer__UEAAJKPEAUtagSIZE___Z(
    void* pThis, unsigned long dwDrawAspect, SIZE* psizel) {
    (void)pThis;
    (void)dwDrawAspect;
    (void)psizel;
    return 0;
}

// Symbol: ?GetInPlaceSite@XOleDocumentView@CDocObjectServer@@UEAAJPEAPEAUIOleInPlaceSite@@@Z
// IOleDocumentView::GetInPlaceSite -- retail (0x258da0), the whole body:
//     AFX_MANAGE_STATE(this[-0x40])            // server + 0x38, m_pModuleState
//     rcx = m_pViewSite;                       // this - 0x18 = server + 0x60
//     if (rcx != NULL) rcx->vtbl[0x8/8 = 1](); // IUnknown::AddRef
//     *ppIPSite = m_pViewSite;                 // unconditional, NULL included
//     return S_OK;
// m_xOleDocumentView sits at server + 0x78, so the -0x18 the disassembly uses is
// m_pViewSite at +0x60, and -0x40 is m_pModuleState at +0x38.
// This is now safe to reproduce because the constructor above seeds
// m_pViewSite = NULL.  SetInPlaceSite is still a stub, so in this DLL the answer
// is always (NULL, S_OK) -- which is exactly what retail returns for a view that
// has never had an in-place site set.  Deviations: the module-state push is not
// reproduced (that is uniform across this file), and ppIPSite is null-checked,
// which retail does not do.
extern "C" long MS_ABI impl__GetInPlaceSite_XOleDocumentView_CDocObjectServer__UEAAJPEAPEAUIOleInPlaceSite___Z(
    void* pThis, IOleInPlaceSite** ppIPSite) {
    if (pThis == nullptr || ppIPSite == nullptr) {
        return E_POINTER;
    }
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
    IOleInPlaceSite* pViewSite = Member<IOleInPlaceSite*>(pServer, kOff_m_pViewSite);
    if (pViewSite != nullptr) {
        pViewSite->AddRef();
    }
    *ppIPSite = pViewSite;
    return S_OK;
}

// STUB: retail (0x257930) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0xb0/8 = 22 = IOleObject::GetMiscStatus] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetMiscStatus@XOleObject@CDocObjectServer@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_CDocObjectServer__UEAAJKPEAK_Z(
    void* pThis, unsigned long dwAspect, unsigned long* pdwStatus) {
    (void)pThis;
    (void)dwAspect;
    (void)pdwStatus;
    return 0;
}

// STUB: retail (0x257380) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x40/8 = 8 = IOleObject::GetMoniker] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetMoniker@XOleObject@CDocObjectServer@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleObject_CDocObjectServer__UEAAJKKPEAPEAUIMoniker___Z(
    void* pThis, unsigned long dwAssign, unsigned long dwWhichMoniker, IMoniker** ppmk) {
    (void)pThis;
    (void)dwAssign;
    (void)dwWhichMoniker;
    (void)ppmk;
    return 0;
}

// STUB: retail (0x256250) reaches the active view through two virtuals of the
// OWNING DOCUMENT -- m_pOwner (read at this-0x40) vtable slots 0xe0/8 = 28 and
// 0xe8/8 = 29, which take no argument / take &POSITION and match
// CDocument::GetFirstViewPosition and CDocument::GetNextView.  It then builds a
// CPrintInfo on the stack (0x281530), writes two constants into it (1 at +0x08
// and 0x10 at +0x48 of that CPrintInfo -- fields not identified here), and calls
// the view virtual at slot 0x350/8 = 106 with &printInfo, testing the result as
// a BOOL; that signature and use match CView::OnPreparePrinting.  Only then does
// it report the page range.  The page numbers do NOT come out of the
// CPrintDialog directly (an earlier note in this file said "m_pPD->[0x30]",
// which is wrong -- it is one indirection short): retail loads
// pInfo->m_pPD (the CPrintInfo's first member) and then m_pPD->[0x130], which
// is CPrintDialog's `PRINTDLG& m_pd` reference, and reads WORD fields of that
// PRINTDLG -- +0x2c nFromPage, +0x2e nToPage, +0x30 nMinPage.  So
// *pnFirstPage = (m_nFirstPage != -1 ? m_nFirstPage : m_pd.nMinPage), and
// *pcPages = (m_pd.nToPage == 0xffff ? 0xffff : nToPage - nFromPage + 1).
// Returns E_UNEXPECTED (0x8000ffff) both when there is no view (0x256367) and
// when OnPreparePrinting returns FALSE (0x256308).  Needs CView/CPrintInfo
// printing infrastructure.
// Symbol: ?GetPageInfo@XPrint@CDocObjectServer@@UEAAJPEAJ0@Z
extern "C" long MS_ABI impl__GetPageInfo_XPrint_CDocObjectServer__UEAAJPEAJ0_Z(
    void* pThis, long* pnFirstPage, long* pcPages) {
    (void)pThis;
    (void)pnFirstPage;
    (void)pcPages;
    return 0;
}

// STUB: retail (0x258ef0) copies the 16-byte RECT at
// m_pOwner->m_pInPlaceFrame (+0x250) + 0x248 into *prcView with one movups and
// returns S_OK.  m_pInPlaceFrame's own +0x248 field is a COleIPFrameWnd member
// that has not been identified; either way OpenMFC lays out neither object.
// Symbol: ?GetRect@XOleDocumentView@CDocObjectServer@@UEAAJPEAUtagRECT@@@Z
extern "C" long MS_ABI impl__GetRect_XOleDocumentView_CDocObjectServer__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* prcView) {
    (void)pThis;
    (void)prcView;
    return 0;
}

// STUB: retail (0x257640) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x78/8 = 15 = IOleObject::GetUserClassID] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetUserClassID@XOleObject@CDocObjectServer@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetUserClassID_XOleObject_CDocObjectServer__UEAAJPEAU_GUID___Z(
    void* pThis, CLSID* pClsid) {
    (void)pThis;
    (void)pClsid;
    return 0;
}

// STUB: retail (0x2576a0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x80/8 = 16 = IOleObject::GetUserType] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?GetUserType@XOleObject@CDocObjectServer@@UEAAJKPEAPEA_W@Z
extern "C" long MS_ABI impl__GetUserType_XOleObject_CDocObjectServer__UEAAJKPEAPEA_W_Z(
    void* pThis, unsigned long dwFormOfType, wchar_t** pszUserType) {
    (void)pThis;
    (void)dwFormOfType;
    (void)pszUserType;
    return 0;
}

// STUB: retail (0x2573e0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x48/8 = 9 = IOleObject::InitFromData] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?InitFromData@XOleObject@CDocObjectServer@@UEAAJPEAUIDataObject@@HK@Z
extern "C" long MS_ABI impl__InitFromData_XOleObject_CDocObjectServer__UEAAJPEAUIDataObject__HK_Z(
    void* pThis, IDataObject* pDataObject, int fCreation, unsigned long dwReserved) {
    (void)pThis;
    (void)pDataObject;
    (void)fCreation;
    (void)dwReserved;
    return 0;
}

// STUB: retail (0x2575f0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x70/8 = 14 = IOleObject::IsUpToDate] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?IsUpToDate@XOleObject@CDocObjectServer@@UEAAJXZ
extern "C" long MS_ABI impl__IsUpToDate_XOleObject_CDocObjectServer__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// STUB: retail (0x259060) forwards to (m_pOwner + 0x278)->vtbl[0x58/8 = 11],
// IOleObject::DoVerb, with iVerb = -2 (OLEIVERB_OPEN) and every other argument
// zero.  Same missing COleServerDoc interface part as the IOleObject group.
// Symbol: ?Open@XOleDocumentView@CDocObjectServer@@UEAAJXZ
extern "C" long MS_ABI impl__Open_XOleDocumentView_CDocObjectServer__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// STUB: retail (0x2563b0) is the full IPrint::Print pump.  Its argument
// validation returns E_POINTER (0x80004003) when pcPagesPrinted, pptd or
// ppPageSet is NULL and E_INVALIDARG (0x80070057) when *pptd is NULL; it then
// stores nFirstPage into m_nFirstPage, zeroes *pcPagesPrinted, walks to the
// active view through the owning document's GetFirstViewPosition /
// GetNextView (slots 28 / 29, E_UNEXPECTED if there is none) and drives a
// CPrintInfo + printer DC through the view virtuals at slots 0x358/8 = 107 and
// 0x368/8 = 109 -- one slot after and three after the slot GetPageInfo uses for
// OnPreparePrinting, which in CView's declaration order is OnBeginPrinting and
// OnEndPrinting -- plus 0x360/8 = 108 (OnPrint) and 0x320/8 = 100
// (OnPrepareDC), with the IContinueCallback's slot 4 (FContinuePrinting,
// 0x256aa4) polled per page.  Correction to an earlier note here: retail Print
// does NOT call this class's DoPreparePrinting / DoBeginPrinting / DoPrint /
// DoEndPrinting helpers -- those are one-instruction tail-jumps into exactly
// the same CView slots (0x256050 -> 0x350, 0x2560b0 -> 0x358,
// 0x256090 -> 0x360, 0x2560d0 -> 0x368, 0x256070 -> 0x320; they ignore their
// own `this`) and the compiler inlined them here, so the disassembly dispatches
// on the view directly.  The blocker is the CView/CDC/CPrintInfo printing
// subsystem, not those helpers.
// Symbol: ?Print@XPrint@CDocObjectServer@@UEAAJKPEAPEAUtagDVTARGETDEVICE@@PEAPEAUtagPAGESET@@PEAUtagSTGMEDIUM@@PEAUIContinueCallback@@JPEAJ4@Z
extern "C" long MS_ABI impl__Print_XPrint_CDocObjectServer__UEAAJKPEAPEAUtagDVTARGETDEVICE__PEAPEAUtagPAGESET__PEAUtagSTGMEDIUM__PEAUIContinueCallback__JPEAJ4_Z(
    void* pThis, unsigned long grfFlags, DVTARGETDEVICE** pptd, PAGESET** ppPageSet,
    STGMEDIUM* pstgmOptions, IContinueCallback* pcallback, long nFirstPage,
    long* pcPagesPrinted, long* pnLastPage) {
    (void)pThis;
    (void)grfFlags;
    (void)pptd;
    (void)ppPageSet;
    (void)pstgmOptions;
    (void)pcallback;
    (void)nFirstPage;
    (void)pcPagesPrinted;
    (void)pnLastPage;
    return 0;
}

// Symbol: ?QueryStatus@XOleCommandTarget@CDocObjectServer@@UEAAJPEBU_GUID@@KQEAU_tagOLECMD@@PEAU_tagOLECMDTEXT@@@Z
// IOleCommandTarget::QueryStatus -- retail (0x2580c0); m_xOleCommandTarget sits
// at server + 0x80, which is the -0x80 the disassembly uses:
//     pTarget = CDocObjectServer::GetControllingFrame(server)   (call 0x255fd0)
//     return helper_257e68(pTarget, pguidCmdGroup, cCmds, prgCmds, pCmdText)
// The helper at 0x257e68 (no exported symbol) was disassembled:
//     if (prgCmds == NULL) return E_POINTER;               // 0x257ea4/0x257ead
//     if (pTarget == NULL) {                               // 0x257eb6
//         for (i = 0; i < cCmds; ++i) prgCmds[i].cmdf = 0;  // 0x257ec4 loop
//         return S_OK;
//     }
//     COleCmdUI cmdUI(prgCmds, cCmds, pguidCmdGroup);      // ctor 0x257b20
//     for (i = 0; i < cCmds; ++i) {
//         cmdUI.m_nID = prgCmds[i].cmdID;
//         cmdUI.DoUpdate(pTarget, TRUE);                   // 0x257c50
//     }
//     ...then the pCmdText branch (0x257f2c onwards), which loads a string and
//        fills pCmdText->rgwz/cwActual...
// PARTIAL.  The E_POINTER test and the whole NULL-target path are exact
// transcriptions.  The COleCmdUI walk is not reproduced: COleCmdUI::DoUpdate
// routes through CCmdTarget::OnCmdMsg with the CN_OLECOMMAND notify code and
// this DLL implements no such routing, so no command could report support.  This
// body therefore takes the NULL-target path for a non-NULL target as well.  That
// is an approximation, not a transcription -- name it plainly: retail would run
// DoUpdate per command and would also fill pCmdText, and this does neither.
extern "C" long MS_ABI impl__QueryStatus_XOleCommandTarget_CDocObjectServer__UEAAJPEBU_GUID__KQEAU_tagOLECMD__PEAU_tagOLECMDTEXT___Z(
    void* pThis, const GUID* pguidCmdGroup, ULONG cCmds, OLECMD* prgCmds,
    OLECMDTEXT* pCmdText) {
    (void)pThis;
    (void)pguidCmdGroup;
    (void)pCmdText;
    if (prgCmds == nullptr) {
        return E_POINTER;
    }
    // GetControllingFrame is deliberately not called: its only use here is to
    // choose between the two paths above, and with no OLE command routing in
    // this DLL both paths produce the zero-fill below.
    for (ULONG i = 0; i < cCmds; ++i) {
        prgCmds[i].cmdf = 0;
    }
    return S_OK;
}

// IOleDocumentView::SaveViewState -- retail (0x259130) is instruction-for-
// instruction the ApplyViewState body above with two operands changed: the
// CArchive nMode is 2 (CArchive::store | CArchive::bNoFlushOnDelete -- store is
// 0) at 0x25919b instead of 3, and the virtual is slot 0xb8/8 = 23 at 0x2591c0
// instead of 22, which is OnSaveViewState by the declaration-order argument
// recorded under ApplyViewState above.  The NULL-stream test is at 0x259165 and
// the AfxThrowInvalidArgException at 0x25926c, and the catch continuation is at
// 0x2591e9 -- and it has the same shape and therefore the same reading as
// ApplyViewState's: it falls into ~CArchive / ~CFileException / ~COleStreamFile,
// objects the NULL path never constructs (their constructors are at 0x25918a and
// 0x2591ac, after the test), so it belongs to the try around the archive leg and
// the argument throw escapes to the caller.  That throw is reproduced below.
// PARTIAL, exactly as ApplyViewState: the COleStreamFile + CArchive wrapper and
// the dispatch to OnSaveViewState are not reproduced, so a derived class's
// override is silently skipped.  For the base class OnSaveViewState is the empty
// body at 0x2820, so nothing is ever put into the archive.  One asymmetry worth
// keeping: nMode 2 makes this a STORING archive and the ar.Close() at 0x2591d5
// (call 0x1cfb90) is where a storing archive flushes.  0x1cfb90 was not
// disassembled, so "an empty OnSaveViewState leaves the buffer empty and
// therefore writes no byte to the stream" is stated as the expectation it is,
// not as a decoded fact.
// Symbol: ?SaveViewState@XOleDocumentView@CDocObjectServer@@UEAAJPEAUIStream@@@Z
extern "C" long MS_ABI impl__SaveViewState_XOleDocumentView_CDocObjectServer__UEAAJPEAUIStream___Z(
    void* pThis, IStream* pstm) {
    (void)pThis;
    if (pstm == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();      // retail 0x259168/0x25926c
    }
    return S_OK;                                         // retail 0x25916e, hr = 0
}

// STUB: retail (0x257140) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x18/8 = 3 = IOleObject::SetClientSite] -- the owning COleServerDoc's IOleObject part.
// On success it then calls CDocObjectServer::ReleaseDocSite (0x255ec0) and, when
// pClientSite is non-NULL, QueryInterfaces it for the IID at .rdata 0x2d7ee0
// (verified there as {B722BCC7-4E68-101B-A2BC-00AA00404770} = IID_IOleDocumentSite)
// straight into m_pDocSite (+0x40), overwriting the returned HRESULT.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?SetClientSite@XOleObject@CDocObjectServer@@UEAAJPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__SetClientSite_XOleObject_CDocObjectServer__UEAAJPEAUIOleClientSite___Z(
    void* pThis, IOleClientSite* pClientSite) {
    (void)pThis;
    (void)pClientSite;
    return 0;
}

// STUB: retail (0x2579a0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0xb8/8 = 23 = IOleObject::SetColorScheme] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?SetColorScheme@XOleObject@CDocObjectServer@@UEAAJPEAUtagLOGPALETTE@@@Z
extern "C" long MS_ABI impl__SetColorScheme_XOleObject_CDocObjectServer__UEAAJPEAUtagLOGPALETTE___Z(
    void* pThis, LOGPALETTE* pLogpal) {
    (void)pThis;
    (void)pLogpal;
    return 0;
}

// STUB: retail (0x257710) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x88/8 = 17 = IOleObject::SetExtent] -- the owning COleServerDoc's IOleObject part.
// It first returns E_FAIL outright when m_pOwner->m_pDocObjectServer (+0x268)
// is non-NULL.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?SetExtent@XOleObject@CDocObjectServer@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__SetExtent_XOleObject_CDocObjectServer__UEAAJKPEAUtagSIZE___Z(
    void* pThis, unsigned long dwDrawAspect, SIZE* psizel) {
    (void)pThis;
    (void)dwDrawAspect;
    (void)psizel;
    return 0;
}

// STUB: retail (0x257240) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x28/8 = 5 = IOleObject::SetHostNames] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?SetHostNames@XOleObject@CDocObjectServer@@UEAAJPEB_W0@Z
extern "C" long MS_ABI impl__SetHostNames_XOleObject_CDocObjectServer__UEAAJPEB_W0_Z(
    void* pThis, const wchar_t* szContainerApp, const wchar_t* szContainerObj) {
    (void)pThis;
    (void)szContainerApp;
    (void)szContainerObj;
    return 0;
}

// Symbol: ?SetInPlaceSite@XOleDocumentView@CDocObjectServer@@UEAAJPEAUIOleInPlaceSite@@@Z
// IOleDocumentView::SetInPlaceSite -- retail (0x258d10):
//     AFX_MANAGE_STATE(this[-0x40])
//     if (m_pOwner->m_pInPlaceFrame != NULL)          // m_pOwner + 0x250
//         (m_pOwner + 0x288)->vtbl[0x28/8 = 5]()      // IOleInPlaceObject::InPlaceDeactivate
//     if (m_pViewSite) m_pViewSite->Release()         // this - 0x18, slot 2
//     m_pViewSite = pIPSite; if (pIPSite) pIPSite->AddRef()
//     return S_OK
// STUB: the first step needs the retail COleServerDoc layout, which OpenMFC's
// COleServerDoc does not have.  The reference-counting half is deliberately not
// implemented on its own -- swapping m_pViewSite without the owner-side
// deactivation would leave the two halves of the state inconsistent.  (The
// constructor above now does seed m_pViewSite = NULL, so a release here would no
// longer touch uninitialised storage; the objection is the split state, not the
// seeding.)  Signature corrected so CloseView can dispatch to it.
extern "C" long MS_ABI impl__SetInPlaceSite_XOleDocumentView_CDocObjectServer__UEAAJPEAUIOleInPlaceSite___Z(
    void* pThis, IOleInPlaceSite* pIPSite) {
    (void)pThis;
    (void)pIPSite;
    return 0;
}

// Symbol: ?SetInitialPageNum@XPrint@CDocObjectServer@@UEAAJJ@Z
// IPrint::SetInitialPageNum -- retail (0x256210):
//     AFX_MANAGE_STATE(this[-0x50]); mov %edx,-0x38(this); xor %eax,%eax; ret
// -0x38 from m_xPrint (+0x88) is m_nFirstPage (+0x50), the LONG the constructor
// at 0x255da0 seeds with -1 and that GetPageInfo (0x256250) reads back at the
// same displacement (0x256319): it reports m_nFirstPage when it is not -1, and
// otherwise falls back to the print dialog's nMinPage (m_pPD->m_pd + 0x30),
// not to anything the document holds.  This is a plain store into the caller's
// own CDocObjectServer, so it needs nothing from the owning document.  The
// offset is spelled out as a named constant above (the constructor writes the
// same member through kOff_m_nFirstPage; kFirstPageFromXPrint is the same
// member reached from the m_xPrint sub-object, 0x50 - 0x88 = -0x38).
// One deliberate deviation from retail: the null-pointer guard below, which
// retail does not have (it would simply fault).
extern "C" long MS_ABI impl__SetInitialPageNum_XPrint_CDocObjectServer__UEAAJJ_Z(
    void* pThis, LONG nFirstPage) {
    if (pThis == nullptr) {
        return E_UNEXPECTED;
    }
    *reinterpret_cast<LONG*>(static_cast<char*>(pThis) + kFirstPageFromXPrint) = nFirstPage;
    return S_OK;
}

// STUB: retail (0x257310) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x38/8 = 7 = IOleObject::SetMoniker] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?SetMoniker@XOleObject@CDocObjectServer@@UEAAJKPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__SetMoniker_XOleObject_CDocObjectServer__UEAAJKPEAUIMoniker___Z(
    void* pThis, unsigned long dwWhichMoniker, IMoniker* pmk) {
    (void)pThis;
    (void)dwWhichMoniker;
    (void)pmk;
    return 0;
}

// Symbol: ?SetRect@XOleDocumentView@CDocObjectServer@@UEAAJPEAUtagRECT@@@Z
// IOleDocumentView::SetRect -- retail (0x258e80), the whole body:
//     AFX_MANAGE_STATE(this[-0x40])
//     m_pOwner->vtbl[0x300/8 = 96](prcView, prcView)   // this[-0x30] = +0x48
//     return S_OK                                      // E_UNEXPECTED if it throws
// Slot 96 of the vtable at .rdata 0x2efe28 (COleServerDocEx's, whose leading
// slots are COleServerDoc's) is +0x300 = 0x2f0128 =
// ?OnSetItemRects@COleServerDoc@@MEAAXPEBUtagRECT@@0@Z, the same virtual
// CDocObjectServer::OnSetItemRects above forwards to; note that retail passes
// prcView as BOTH the position and the clip rectangle.
// PARTIAL, for the same two reasons spelled out at OnSetItemRects: the forward
// goes to COleServerDoc's impl__ thunk rather than through a vtable, so a
// client override is bypassed, and that thunk is an empty stub today (it
// guards on an m_pInPlaceFrame OpenMFC does not model), so this currently has
// no observable effect -- but it will pick one up automatically.  Retail's
// TRY/CATCH -> E_UNEXPECTED is not reproduced; nothing here can throw.  The
// signature is corrected: the generated stub's single parameter was silently
// receiving `this` and dropping prcView.
extern "C" long MS_ABI impl__SetRect_XOleDocumentView_CDocObjectServer__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* prcView) {
    if (pThis == nullptr) {
        return E_UNEXPECTED;   // deviation: retail has no such guard
    }
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
    COleServerDoc* pOwner = Member<COleServerDoc*>(pServer, kOff_m_pOwner);
    if (pOwner == nullptr) {
        return E_UNEXPECTED;   // deviation: retail would fault
    }
    impl__OnSetItemRects_COleServerDoc__MEAAXPEBUtagRECT__0_Z(pOwner, prcView, prcView);
    return S_OK;
}

// Symbol: ?SetRectComplex@XOleDocumentView@CDocObjectServer@@UEAAJPEAUtagRECT@@000@Z
// IOleDocumentView::SetRectComplex -- retail (0x258f50):
//     AFX_MANAGE_STATE(this[-0x40]); mov $0x80004001,%eax; ret
// Nothing else: no parameter is read and no member is touched.
extern "C" long MS_ABI impl__SetRectComplex_XOleDocumentView_CDocObjectServer__UEAAJPEAUtagRECT__000_Z(
    void* pThis, RECT* prcView, RECT* prcHScroll, RECT* prcVScroll, RECT* prcSizeBox) {
    (void)pThis;
    (void)prcView;
    (void)prcHScroll;
    (void)prcVScroll;
    (void)prcSizeBox;
    return E_NOTIMPL;
}

// Symbol: ?Show@XOleDocumentView@CDocObjectServer@@UEAAJH@Z
// IOleDocumentView::Show -- retail (0x258f80):
//     bShow != 0 : call COleServerDoc::ActivateInPlace (0x265b50) with m_pOwner
//                  in RCX, then `neg/sbb/not/and $0x80004005` -- i.e. map its
//                  BOOL result to S_OK (non-zero) / E_FAIL (zero)
//     bShow == 0 : call (m_pOwner + 0x288)->vtbl[0x28/8 = 5], which is
//                  IOleInPlaceObject::InPlaceDeactivate on the owning document
// STUB: both arms need the retail COleServerDoc layout (m_pOwner + 0x288 is one
// of that object's nested interface parts), which OpenMFC's COleServerDoc does
// not have.  Signature corrected so CloseView can dispatch to it.
extern "C" long MS_ABI impl__Show_XOleDocumentView_CDocObjectServer__UEAAJH_Z(void* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return 0;
}

// Symbol: ?UIActivate@XOleDocumentView@CDocObjectServer@@UEAAJH@Z
// IOleDocumentView::UIActivate -- retail (0x258ff0):
//     AFX_MANAGE_STATE(server + 0x38)          // server = this - 0x78
//     if (fUIActivate != 0)                    // test %ebx,%ebx; je, 0x25900e
//         return server->vtbl[0xc0/8 = 24]();  // CDocObjectServer::OnActivateView
//     return (m_pOwner + 0x288)->vtbl[0x30/8 = 6]();   // IOleInPlaceObject::UIDeactivate
// Both arms are one tail call and the HRESULT is returned unchanged.  Slot 24 is
// OnActivateView: the class vtable at .rdata 0x32d2e8 holds 0x2586a0 there, and
// 0x2586a0 is the RVA the map gives for
// ?OnActivateView@CDocObjectServer@@MEAAJXZ.  m_pOwner + 0x288 is
// COleServerDoc::m_xOleInPlaceObject (see the layout note at the top).
// PARTIAL.
//   * fUIActivate != 0 IS reproduced, as a direct call to OnActivateView's own
//     impl__ thunk (defined above in this file) instead of a dispatch through
//     the CDocObjectServer vtable, which this DLL does not build.  Consequence,
//     stated plainly: a derived class that overrides OnActivateView is bypassed.
//     Today that thunk is itself a documented stub returning S_OK, so this arm
//     has no observable effect yet -- it picks one up automatically.
//   * fUIActivate == 0 is NOT reproduced: it needs the owning document's nested
//     IOleInPlaceObject part, which OpenMFC's COleServerDoc does not have.  The
//     S_OK returned on that path is the generated stub's value, not retail's --
//     retail returns whatever the document's UIDeactivate returns.
extern "C" long MS_ABI impl__UIActivate_XOleDocumentView_CDocObjectServer__UEAAJH_Z(
    void* pThis, int fUIActivate) {
    if (pThis == nullptr) {
        return E_UNEXPECTED;   // deviation: retail has no such guard
    }
    if (fUIActivate != 0) {
        void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
        return impl__OnActivateView_CDocObjectServer__MEAAJXZ(pServer);
    }
    return S_OK;   // NOT retail -- see the note above.
}

// STUB: retail (0x257870) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0xa0/8 = 20 = IOleObject::Unadvise] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?Unadvise@XOleObject@CDocObjectServer@@UEAAJK@Z
extern "C" long MS_ABI impl__Unadvise_XOleObject_CDocObjectServer__UEAAJK_Z(
    void* pThis, unsigned long dwConnection) {
    (void)pThis;
    (void)dwConnection;
    return 0;
}

// STUB: retail (0x2575a0) is AFX_MANAGE_STATE followed by a straight forward to
// (m_pOwner + 0x278)->vtbl[0x68/8 = 13 = IOleObject::Update] -- the owning COleServerDoc's IOleObject part.
// Arguments are passed straight through and the HRESULT is returned unchanged.
// OpenMFC's COleServerDoc has no such interface part, so the forward is not
// reproducible; see the layout note at the top of this file.
// Symbol: ?Update@XOleObject@CDocObjectServer@@UEAAJXZ
extern "C" long MS_ABI impl__Update_XOleObject_CDocObjectServer__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}
