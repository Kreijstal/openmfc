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
// m_nFirstPage = -1, and additionally `pDoc->[0x1c0] = 1`, which is
// COleDocument::m_bCompoundFile = TRUE (retail COleDocument layout, decoded
// from ??0COleDocument at mfc140u 0x253e90 against the real afxole.h member
// order and recorded in core/ole/COleDocument.cpp: +0x1c0 is m_bCompoundFile).
// OpenMFC's COleDocument declares that member too (afxole.h, public), so the
// store IS reproduced, through the member rather than the retail offset.  The
// four CDocObjectServer data-member stores are reproduced as well (see the
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
// The owning document's interface parts, and how the forwards into them are
// made.  The whole IOleObject part, plus Open / Show / SetInPlaceSite /
// UIActivate of IOleDocumentView, is a forwarder into the *owning document's*
// own nested interface parts at m_pOwner + 0x278 (COleServerDoc::m_xOleObject)
// and m_pOwner + 0x288 (COleServerDoc::m_xOleInPlaceObject), each a vptr-only
// 8-byte sub-object that retail dispatches through.
//
// An earlier version of this file left every one of those as a stub on the
// claim that "m_pOwner + 0x278 is off the end of the object".  That claim was
// measured and is false.  OpenMFC's COleServerDoc (include/openmfc/afxole.h)
// is COleLinkingDoc (0x230 bytes), then BOOL m_bEmbedded at +0x230, then
// `char _coleserverdoc_padding[96]` at +0x234 .. +0x294, for sizeof == 0x298
// (measured with the phase4 compiler) -- retail's size exactly, because
// retail's five interface parts occupy +0x270 .. +0x298.  Both construction
// paths zero-fill that padding: COleServerDoc::COleServerDoc
// (core/ole/COleServerDoc.cpp:441-443, memset) is what the exported ctor thunk
// in core/ole/Thunks.cpp:1178 placement-news.  So every retail owner offset
// this file touches -- +0x250 m_pInPlaceFrame, +0x268 m_pDocObjectServer,
// +0x278 m_xOleObject, +0x288 m_xOleInPlaceObject -- lies inside that
// padding: addressable, and zero unless COleServerDoc.cpp starts to model it.
// (+0x230, retail's m_lpClientSite, is NOT padding -- it is OpenMFC's
// m_bEmbedded -- and nothing in this file reads it.)  The static_assert on
// sizeof(COleServerDoc) in the namespace below pins the size.
//
// Retail reaches the parts VIRTUALLY (`mov (%rcx),%rax; call *0xNN(%rax)`,
// through the part's vptr and the CFG dispatch pointer).  In this DLL the
// padding holds no vptr, so the dispatch is made as a direct call to the
// COleServerDoc part's own impl__ thunk (core/ole/COleServerDoc.cpp) with the
// SAME `this` retail passes: the sub-object address m_pOwner + 0x278 or
// + 0x288.  That direct call is the retail call, not an approximation of it:
// XOleObject and XOleInPlaceObject are BEGIN_INTERFACE_PART nested classes
// (declared under `public:` in the real afxole.h, read on this host, so "public
// nested", not private) that nothing derives from, hence one vtable each and
// no slot can be overridden; and the thunks -- checked, not assumed --
// take the interface sub-object pointer `pThisItf` first, as retail does.
// Today every one of those thunks ignores pThisItf and reads no document
// state, but they do NOT all return S_OK (checked against
// core/ole/COleServerDoc.cpp, not assumed): ActivateInPlace returns FALSE;
// GetMoniker writes *ppmk = NULL and returns E_FAIL (E_POINTER for a NULL
// ppmk); SetColorScheme returns E_NOTIMPL; SetExtent returns E_INVALIDARG for
// a NULL size and E_FAIL otherwise; SetMoniker returns E_FAIL; the remaining
// nineteen targets (the other seventeen IOleObject thunks plus
// InPlaceDeactivate and UIDeactivate) return S_OK.  So five of the forwards
// below now report a failure where the bare `return 0` stubs they replace
// reported S_OK: GetMoniker, SetColorScheme, SetExtent, SetMoniker, and
// Show(TRUE), which maps ActivateInPlace's FALSE to E_FAIL.  Each of the five
// is the determinate retail outcome for a document with no client site / no
// embedded item, as that file documents (retail ActivateInPlace, mfc140u
// 0x266da0, returns FALSE at 0x267483 when m_lpClientSite is NULL) -- and the
// rest are observationally unchanged.  All of them pick up an implementation
// there automatically.
//
// Where a forward's target is a COleServerDoc / CDocument method rather than
// an interface part -- OnSetItemRects, SetRect, OnExecOleCmd, OnCloseDocument,
// GetRect -- it goes to that class's thunk (or, for OnCloseDocument, through
// OpenMFC's CDocument vtable).  Only GetItemPosition (GetRect's target) is
// non-virtual in retail; OnSetItemRects, OnExecOleCmd and OnCloseDocument are
// retail VIRTUALS, and where the direct thunk call bypasses that dispatch it
// is spelled out at the function.
//
// Signatures.  Every entry point in this file now carries its real signature,
// with the interface sub-object pointer (`pThis`, RCX on entry) first.  The
// stub generator had emitted most of them WITHOUT the implicit `this`, so `p0`
// was silently receiving the interface pointer, every later argument was
// shifted by one, and the last one was dropped off the end.  That is corrected
// throughout; the bodies that are still stubs ignore every argument, so the
// parameter names below are documentation of the ABI and nothing more.
//
// Return values of the remaining stubs.  DoPrepareDC and DoPrint are void
// no-ops.  OnActivateView returns 0 = S_OK, which is retail's own early-out
// value for a document with no client site (see its comment).  GetPageInfo
// and Print are PARTIAL rather than bare stubs: the argument validation and
// the stores retail makes before it needs the printing subsystem are
// reproduced, and the leg this DLL cannot run reports E_UNEXPECTED -- retail's
// own failure code on that path -- instead of the generated S_OK, which told
// the caller the call had succeeded and left the out pointers unwritten.
// Where a retail body DOES pin its out parameter the store is reproduced and
// the return value follows it: CreateView and EnumViews NULL theirs
// unconditionally, and EnumViews / GetDocument report E_NOINTERFACE rather
// than S_OK when they have nothing to hand back.
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
// (mfc140 vtable 0x32d2b0 slot 0 = 0x256e00 in mfc140; NOT the mfc140u
// 0x256e00, which is ReleaseDocSite) reduces to, METHOD_PROLOGUE_EX plus the
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

// The owning COleServerDoc's entry points that the forwards below target.
// Every declaration matches a definition that exists today in
// phase4/src/core/ole/COleServerDoc.cpp (the definitions there spell the COM
// interface pointers as void*, the same ABI; the types here are the ones the
// mangled names describe).  The X* thunks take the interface SUB-OBJECT
// pointer first -- m_pOwner + 0x278 for XOleObject, m_pOwner + 0x288 for
// XOleInPlaceObject -- which is what the bodies below hand them.
extern "C" int MS_ABI impl__ActivateInPlace_COleServerDoc__QEAAHXZ(COleServerDoc* pThis);
extern "C" void MS_ABI impl__GetItemPosition_COleServerDoc__QEBAXPEAUtagRECT___Z(
    const COleServerDoc* pThis, RECT* lpRect);
extern "C" long MS_ABI impl__Advise_XOleObject_COleServerDoc__UEAAJPEAUIAdviseSink__PEAK_Z(
    void* pThisItf, IAdviseSink* pAdvSink, unsigned long* pdwConnection);
extern "C" long MS_ABI impl__Close_XOleObject_COleServerDoc__UEAAJK_Z(
    void* pThisItf, unsigned long dwSaveOption);
extern "C" long MS_ABI impl__DoVerb_XOleObject_COleServerDoc__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(
    void* pThisItf, long iVerb, MSG* lpmsg, IOleClientSite* pActiveSite, long lindex,
    HWND hwndParent, const RECT* lprcPosRect);
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_COleServerDoc__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThisItf, IEnumSTATDATA** ppenumAdvise);
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_COleServerDoc__UEAAJPEAPEAUIEnumOLEVERB___Z(
    void* pThisItf, IEnumOLEVERB** ppenumOleVerb);
extern "C" long MS_ABI impl__GetClientSite_XOleObject_COleServerDoc__UEAAJPEAPEAUIOleClientSite___Z(
    void* pThisItf, IOleClientSite** ppClientSite);
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_COleServerDoc__UEAAJKPEAPEAUIDataObject___Z(
    void* pThisItf, unsigned long dwReserved, IDataObject** ppDataObject);
extern "C" long MS_ABI impl__GetExtent_XOleObject_COleServerDoc__UEAAJKPEAUtagSIZE___Z(
    void* pThisItf, unsigned long dwDrawAspect, SIZE* psizel);
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_COleServerDoc__UEAAJKPEAK_Z(
    void* pThisItf, unsigned long dwAspect, unsigned long* pdwStatus);
extern "C" long MS_ABI impl__GetMoniker_XOleObject_COleServerDoc__UEAAJKKPEAPEAUIMoniker___Z(
    void* pThisItf, unsigned long dwAssign, unsigned long dwWhichMoniker, IMoniker** ppmk);
extern "C" long MS_ABI impl__GetUserClassID_XOleObject_COleServerDoc__UEAAJPEAU_GUID___Z(
    void* pThisItf, CLSID* pClsid);
extern "C" long MS_ABI impl__GetUserType_XOleObject_COleServerDoc__UEAAJKPEAPEA_W_Z(
    void* pThisItf, unsigned long dwFormOfType, wchar_t** pszUserType);
extern "C" long MS_ABI impl__InitFromData_XOleObject_COleServerDoc__UEAAJPEAUIDataObject__HK_Z(
    void* pThisItf, IDataObject* pDataObject, int fCreation, unsigned long dwReserved);
extern "C" long MS_ABI impl__IsUpToDate_XOleObject_COleServerDoc__UEAAJXZ(void* pThisItf);
extern "C" long MS_ABI impl__SetClientSite_XOleObject_COleServerDoc__UEAAJPEAUIOleClientSite___Z(
    void* pThisItf, IOleClientSite* pClientSite);
extern "C" long MS_ABI impl__SetColorScheme_XOleObject_COleServerDoc__UEAAJPEAUtagLOGPALETTE___Z(
    void* pThisItf, LOGPALETTE* pLogpal);
extern "C" long MS_ABI impl__SetExtent_XOleObject_COleServerDoc__UEAAJKPEAUtagSIZE___Z(
    void* pThisItf, unsigned long dwDrawAspect, SIZE* psizel);
extern "C" long MS_ABI impl__SetHostNames_XOleObject_COleServerDoc__UEAAJPEB_W0_Z(
    void* pThisItf, const wchar_t* szContainerApp, const wchar_t* szContainerObj);
extern "C" long MS_ABI impl__SetMoniker_XOleObject_COleServerDoc__UEAAJKPEAUIMoniker___Z(
    void* pThisItf, unsigned long dwWhichMoniker, IMoniker* pmk);
extern "C" long MS_ABI impl__Unadvise_XOleObject_COleServerDoc__UEAAJK_Z(
    void* pThisItf, unsigned long dwConnection);
extern "C" long MS_ABI impl__Update_XOleObject_COleServerDoc__UEAAJXZ(void* pThisItf);
extern "C" long MS_ABI impl__InPlaceDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ(
    void* pThisItf);
extern "C" long MS_ABI impl__UIDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ(
    void* pThisItf);

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
constexpr long kOff_m_xOleObject        = 0x68;
constexpr long kOff_m_xOleDocument      = 0x70;
constexpr long kOff_m_xOleDocumentView  = 0x78;
constexpr long kOff_m_xOleCommandTarget = 0x80;
constexpr long kOff_m_xPrint            = 0x88;

// Offsets inside the OWNING COleServerDoc that retail bodies in this file read,
// each named at the function that reads it and in the layout note at the top
// (they are also the offsets core/ole/COleServerDoc.cpp records in its own
// header comment).  All four sit inside OpenMFC's zero-filled
// _coleserverdoc_padding (+0x234 .. +0x294), which the static_assert pins by
// size: retail's COleServerDoc is 0x298 bytes and so is OpenMFC's.
constexpr long kOwnerOff_m_pInPlaceFrame     = 0x250;   // COleIPFrameWnd*
constexpr long kOwnerOff_m_pDocObjectServer  = 0x268;   // CDocObjectServer*
constexpr long kOwnerOff_m_xOleObject        = 0x278;   // XOleObject part (IOleObject)
constexpr long kOwnerOff_m_xOleInPlaceObject = 0x288;   // XOleInPlaceObject part
static_assert(sizeof(COleServerDoc) == 0x298,
              "OpenMFC COleServerDoc must be retail-sized (0x298) for the owner "
              "offsets above to lie inside the object");

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

// IID_IOleDocumentSite.  Read out of mfc140u .rdata at RVA 0x2d9f90 -- the
// operand of the `lea` at 0x258109 inside XOleObject::SetClientSite (0x2580b0):
//     c7 bc 22 b7  68 4e  1b 10  a2 bc 00 aa 00 40 47 70
// i.e. {B722BCC7-4E68-101B-A2BC-00AA00404770}.
const GUID kIID_IOleDocumentSite = {
    0xB722BCC7, 0x4E68, 0x101B, { 0xA2, 0xBC, 0x00, 0xAA, 0x00, 0x40, 0x47, 0x70 } };

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

// A sub-object / member address inside the owning COleServerDoc, at one of the
// kOwnerOff_* offsets above.
inline void* OwnerAt(COleServerDoc* pOwner, long nOffset) {
    return reinterpret_cast<char*>(pOwner) + nOffset;
}

// ---- the IOleObject forwards ----
// Nineteen XOleObject entry points of CDocObjectServer (Advise, Close, DoVerb,
// EnumAdvise, EnumVerbs, GetClientSite, GetClipboardData, GetExtent,
// GetMiscStatus, GetMoniker, GetUserClassID, GetUserType, InitFromData,
// IsUpToDate, SetColorScheme, SetHostNames, SetMoniker, Unadvise, Update) have
// the identical retail shape; each was disassembled from mfc140u and its RVA,
// displacement and slot are cited at the function:
//     AFX_MANAGE_STATE(server->m_pModuleState)   // `mov -0x30(%rcx),%rdx`:
//                                                //   m_xOleObject is at +0x68,
//                                                //   so -0x30 is +0x38
//     rcx = server->m_pOwner + 0x278             // `mov -0x20(%rbx),%rcx` (+0x48)
//                                                //   then `add $0x278,%rcx`
//     return rcx->vtbl[slot](args...)            // IOleObject slot, called via
//                                                //   the CFG dispatch pointer
// i.e. a straight forward -- arguments untouched, HRESULT returned unchanged --
// to the owning COleServerDoc's IOleObject part.  OwnerOleObjectPart() computes
// that `this`; each body then calls the matching COleServerDoc thunk (see the
// layout note at the top for why the direct call is the retail call).  The
// module-state push is not reproduced (uniform across this file).  One
// deviation, shared by all of them: pThis and m_pOwner are null-checked and
// E_UNEXPECTED is returned; retail dereferences both and would fault.
// SetClientSite and SetExtent do more than forward and are commented
// individually.
inline void* OwnerOleObjectPart(void* pThis) {
    if (pThis == nullptr) return nullptr;
    void* pServer = ServerFromPart(pThis, kOff_m_xOleObject);
    COleServerDoc* pOwner = Member<COleServerDoc*>(pServer, kOff_m_pOwner);
    return pOwner != nullptr ? OwnerAt(pOwner, kOwnerOff_m_xOleObject) : nullptr;
}

// The same for the XOleDocumentView part (server + 0x78): the owning document,
// or NULL.  Open / Show / SetInPlaceSite / UIActivate / GetRect use it.
inline COleServerDoc* OwnerFromDocumentViewPart(void* pThis) {
    if (pThis == nullptr) return nullptr;
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
    return Member<COleServerDoc*>(pServer, kOff_m_pOwner);
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
//     pDoc->m_bCompoundFile = TRUE          (`movl $0x1,0x1c0(%rdi)` at mfc140u
//                                            0x256d5c; +0x1c0 of a COleDocument
//                                            is m_bCompoundFile, see the top note)
//     m_nFirstPage (+0x50) = -1
//     return this
// Re-read in mfc140u at RVA 0x256ce0: the same stores, base ctor at 0x1de3f0.
// The four data-member stores ARE reproduced below: they are inside the 144
// bytes this repo records for the class (detail/COleControlModuleSupport.h,
// OR_DESC(CDocObjectServer, 144, ...)).  Three of the four are read by an
// implemented body in this file -- m_pDocSite by ReleaseDocSite /
// ActivateDocObject / SetClientSite, m_pOwner by GetControllingFrame /
// OnCloseDocument / OnExecOleCmd / OnSetItemRects / SetRect and by every
// forward into the owning document's interface parts, m_pViewSite by
// GetInPlaceSite / SetInPlaceSite / CreateView.  m_nFirstPage is written by
// SetInitialPageNum and by XPrint::Print, but its only retail reader,
// GetPageInfo, reads it on the OnPreparePrinting leg that is not reproduced,
// so today nothing reads back the -1.
//
// The m_bCompoundFile store is reproduced through OpenMFC's own member
// (COleDocument::m_bCompoundFile, at +0x180 here rather than retail's +0x1c0 --
// OpenMFC's COleDocument keeps only that BOOL and m_bRemember of the retail
// members, in its own order, so the retail offset must not be used).  An
// earlier note here called +0x1c0 "an unidentified field"; it is identified
// (top note) and a doc-object server's document is a compound file in retail,
// which core/ole/COleDocument.cpp consults on every save path.
//
// Two things retail does are deliberately NOT reproduced, and no body in this
// file may assume them:
//   * the CCmdTarget base construction -- so m_dwRef (+0x08) and
//     m_pModuleState (+0x38) stay whatever the caller's storage held;
//   * the six vptr stores -- this DLL builds no vtable for CDocObjectServer or
//     for any of the five nested interface classes, so there is nothing to
//     store.  Anything that dispatches through one of those vtables therefore
//     stays a stub below.
// Deviations: the pThis guard, and pDoc is null-checked before the
// m_bCompoundFile store (retail would fault on a NULL pDoc).
extern "C" void* MS_ABI impl___0CDocObjectServer__QEAA_PEAVCOleServerDoc__PEAUIOleDocumentSite___Z(
    void* pThis, void* pDoc, void* pDocSite) {
    if (pThis == nullptr) {
        return pThis;   // deviation: retail has no such guard (it would fault)
    }
    Member<void*>(pThis, kOff_m_pDocSite) = pDocSite;                  // 0x256d47
    Member<void*>(pThis, kOff_m_pViewSite) = nullptr;                  // 0x256d50
    Member<void*>(pThis, kOff_m_pOwner) = pDoc;                        // 0x256d58
    if (pDoc != nullptr) {
        static_cast<COleServerDoc*>(pDoc)->m_bCompoundFile = TRUE;     // 0x256d5c
    }
    Member<LONG>(pThis, kOff_m_nFirstPage) = -1;                       // 0x256d66
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

// Symbol: ?ActivateDocObject@CDocObjectServer@@QEAAXXZ
// CDocObjectServer::ActivateDocObject() -- retail RVA 0x256e90 (mfc140u; the
// same body sits at 0x255f50 in mfc140), the whole body:
//     if (m_pOwner->m_pDocObjectServer != NULL)   // +0x48, then `cmpq $0,0x268(%rax)`
//         m_pDocSite->vtbl[0x18/8 = 3](NULL);     // +0x40; IOleDocumentSite::ActivateMe,
//                                                 //   via the CFG dispatch pointer
// Slot 3 is ActivateMe: IOleDocumentSite declares exactly one method after
// IUnknown's three.  m_pOwner + 0x268 is COleServerDoc::m_pDocObjectServer and
// is read exactly as retail reads it; in this DLL that word is inside the
// zero-filled padding of OpenMFC's COleServerDoc (layout note at the top) and
// nothing writes it, so the guard is false and ActivateMe is not called --
// which is also what retail does for a document that was never handed a
// doc-object server.  It starts activating the moment COleServerDoc.cpp models
// the member.  Deviations: the pThis / m_pOwner / m_pDocSite null guards;
// retail has none (a NULL m_pDocSite would fault there).
extern "C" void MS_ABI impl__ActivateDocObject_CDocObjectServer__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    COleServerDoc* pOwner = Member<COleServerDoc*>(pThis, kOff_m_pOwner);
    if (pOwner == nullptr) return;
    if (*static_cast<void**>(OwnerAt(pOwner, kOwnerOff_m_pDocObjectServer)) == nullptr) {
        return;                                                    // 0x256ea0
    }
    IOleDocumentSite* pDocSite = Member<IOleDocumentSite*>(pThis, kOff_m_pDocSite);
    if (pDocSite == nullptr) return;   // deviation: retail would fault
    pDocSite->ActivateMe(nullptr);                                 // 0x256eaf
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

// STUB: retail (0x256070 in mfc140; the same seven instructions at RVA
// 0x256fb0 in mfc140u, re-read there) tail-jumps to pView->vtbl[0x320/8 = 100],
// CView::OnPrepareDC(CDC*, CPrintInfo*), through the CFG dispatch pointer.
// OpenMFC's CView (include/openmfc/afxwin.h, re-checked) still declares no
// OnPrepareDC virtual.  An impl__ thunk for the BASE body does exist
// (core/view/CView.cpp: ?OnPrepareDC@CView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z),
// but calling it here would replace retail's virtual dispatch with the base
// implementation and silently bypass the override that every printing view
// supplies -- a wrong answer rather than a missing one -- so this stays a stub
// until the header carries the virtual (header request filed).
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

// STUB: retail (0x256090 in mfc140; the same seven instructions at RVA
// 0x256fd0 in mfc140u, re-read there) tail-jumps to pView->vtbl[0x360/8 = 108],
// CView::OnPrint(CDC*, CPrintInfo*), through the CFG dispatch pointer.
// OpenMFC's CView (include/openmfc/afxwin.h, re-checked) still declares no
// OnPrint virtual.  An impl__ thunk for the BASE body does exist
// (core/view/CView.cpp: ?OnPrint@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z), but
// a direct call to it would bypass the OnPrint override that is the whole point
// of the helper, so this stays a stub until the header carries the virtual
// (header request filed).
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

// STUB: CDocObjectServer::OnActivateView -- retail (0x2586a0 in mfc140; RVA
// 0x259620 in mfc140u, 326 instructions up to the `ret` at 0x259b65, re-read
// there: the first test is `mov 0x48(%rcx),%rax; cmpq $0,0x230(%rax); jne` at
// 0x25964e with `xor %eax,%eax` on the fall-through) is the in-place
// activation path and is far outside what this DLL can express.  Note that
// +0x230 of the owner is retail's m_lpClientSite but OpenMFC's m_bEmbedded
// (layout note at the top), so the early-out cannot be read from the object
// here.  It does not need to be: OpenMFC's COleServerDoc has no client site
// at all (its XOleObject::SetClientSite thunk in core/ole/COleServerDoc.cpp
// stores nothing, and no member or side table holds one), so every document
// this DLL builds is one retail would early-out on, and the S_OK below is
// that early-out's value, not a guess.  What was read from the disassembly,
// so the next reader does not have to start over:
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
// bodies, not this one).  The same holds in mfc140u, read directly this time:
// the export directory maps ordinal 8670 (OnApplyViewState) and ordinal 10963
// (OnSaveViewState) both to RVA 0x27d0 (mfc140u), which is a single `ret`
// (the mfc140u map names that address ?AddDockSite@CFrameWndEx@@QEAAXXZ, again
// one of the folded-in empties).  afxdocob.h on this host declares CDocObjectServer's
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
// CDocObjectServer::ReleaseDocSite() -- retail (0x255ec0 in mfc140; the same
// body at RVA 0x256e00 in mfc140u, re-read there), transcribed in full:
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

// Symbol: ?Advise@XOleObject@CDocObjectServer@@UEAAJPEAUIAdviseSink@@PEAK@Z
// IOleObject::Advise -- retail RVA 0x258770 (mfc140u; the same body at 0x257800 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x98/8 = 19, IOleObject::Advise, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__Advise_XOleObject_CDocObjectServer__UEAAJPEAUIAdviseSink__PEAK_Z(
    void* pThis, IAdviseSink* pAdvSink, unsigned long* pdwConnection) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__Advise_XOleObject_COleServerDoc__UEAAJPEAUIAdviseSink__PEAK_Z(
        pPart, pAdvSink, pdwConnection);
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
// IOleDocumentView::Clone.  This export has no RVA of its own in the mfc140
// map, but slot 15 of the XOleDocumentView vtable (.rdata 0x18032d1a8, inside
// the vtable at 0x18032d130) holds 0x180258f50 -- byte-identical to the
// SetRectComplex slot at 0x18032d170, i.e. the two bodies were COMDAT-folded.
// That body (disassembled below, under SetRectComplex) is AFX_MANAGE_STATE
// followed by `return E_NOTIMPL`.  In mfc140u the export resolves to its own
// RVA 0x259ed0, which was disassembled: AFX_MANAGE_STATE(this - 0x40) and then
// `mov $0x80004001,%eax` -- neither parameter is read.  The body below IS the
// retail body, not a placeholder.
extern "C" long MS_ABI impl__Clone_XOleDocumentView_CDocObjectServer__UEAAJPEAUIOleInPlaceSite__PEAPEAUIOleDocumentView___Z(
    void* pThis, IOleInPlaceSite* pIPSiteNew, IOleDocumentView** ppViewNew) {
    (void)pThis;
    (void)pIPSiteNew;
    (void)ppViewNew;
    return E_NOTIMPL;
}

// Symbol: ?Close@XOleObject@CDocObjectServer@@UEAAJK@Z
// IOleObject::Close -- retail RVA 0x258220 (mfc140u; the same body at 0x2572b0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x30/8 = 6, IOleObject::Close, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__Close_XOleObject_CDocObjectServer__UEAAJK_Z(
    void* pThis, unsigned long dwSaveOption) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__Close_XOleObject_COleServerDoc__UEAAJK_Z(pPart, dwSaveOption);
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
// BEGIN_INTERFACE_PART nested class that nothing derives from -- a single
// vtable, so nothing can override those slots.
// Both callees are implemented below; Show(FALSE) forwards to the owning
// document's InPlaceDeactivate and SetInPlaceSite(NULL) releases m_pViewSite,
// so this returns the S_OK SetInPlaceSite always ends on.
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

// Symbol: ?DoVerb@XOleObject@CDocObjectServer@@UEAAJJPEAUtagMSG@@PEAUIOleClientSite@@JPEAUHWND__@@PEBUtagRECT@@@Z
// IOleObject::DoVerb -- retail RVA 0x258420 (mfc140u; the same body at 0x2574b0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x58/8 = 11, IOleObject::DoVerb, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__DoVerb_XOleObject_CDocObjectServer__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(
    void* pThis, long iVerb, MSG* lpmsg, IOleClientSite* pActiveSite, long lindex,
    HWND hwndParent, const RECT* lprcPosRect) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__DoVerb_XOleObject_COleServerDoc__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(
        pPart, iVerb, lpmsg, pActiveSite, lindex, hwndParent, lprcPosRect);
}

// Symbol: ?EnumAdvise@XOleObject@CDocObjectServer@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
// IOleObject::EnumAdvise -- retail RVA 0x258840 (mfc140u; the same body at 0x2578d0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0xa8/8 = 21, IOleObject::EnumAdvise, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_CDocObjectServer__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, IEnumSTATDATA** ppenumAdvise) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__EnumAdvise_XOleObject_COleServerDoc__UEAAJPEAPEAUIEnumSTATDATA___Z(
        pPart, ppenumAdvise);
}

// Symbol: ?EnumVerbs@XOleObject@CDocObjectServer@@UEAAJPEAPEAUIEnumOLEVERB@@@Z
// IOleObject::EnumVerbs -- retail RVA 0x2584b0 (mfc140u; the same body at 0x257540 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x60/8 = 12, IOleObject::EnumVerbs, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_CDocObjectServer__UEAAJPEAPEAUIEnumOLEVERB___Z(
    void* pThis, IEnumOLEVERB** ppenumOleVerb) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__EnumVerbs_XOleObject_COleServerDoc__UEAAJPEAPEAUIEnumOLEVERB___Z(
        pPart, ppenumOleVerb);
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
// XOleDocument::QueryInterface (mfc140 vtable slot 0 = 0x256e00 in mfc140) is
// METHOD_PROLOGUE_EX plus an inlined CCmdTarget::ExternalQueryInterface, so
// calling this DLL's
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

// Symbol: ?GetClientSite@XOleObject@CDocObjectServer@@UEAAJPEAPEAUIOleClientSite@@@Z
// IOleObject::GetClientSite -- retail RVA 0x258150 (mfc140u; the same body at 0x2571e0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x20/8 = 4, IOleObject::GetClientSite, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetClientSite_XOleObject_CDocObjectServer__UEAAJPEAPEAUIOleClientSite___Z(
    void* pThis, IOleClientSite** ppClientSite) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetClientSite_XOleObject_COleServerDoc__UEAAJPEAPEAUIOleClientSite___Z(
        pPart, ppClientSite);
}

// Symbol: ?GetClipboardData@XOleObject@CDocObjectServer@@UEAAJKPEAPEAUIDataObject@@@Z
// IOleObject::GetClipboardData -- retail RVA 0x2583b0 (mfc140u; the same body at 0x257440 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x50/8 = 10, IOleObject::GetClipboardData, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_CDocObjectServer__UEAAJKPEAPEAUIDataObject___Z(
    void* pThis, unsigned long dwReserved, IDataObject** ppDataObject) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetClipboardData_XOleObject_COleServerDoc__UEAAJKPEAPEAUIDataObject___Z(
        pPart, dwReserved, ppDataObject);
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
// the same way.  XOleDocument::QueryInterface (mfc140 vtable 0x32d2b0 slot 0 =
// 0x256e00 in mfc140) is METHOD_PROLOGUE_EX plus an inlined CCmdTarget::
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

// Symbol: ?GetExtent@XOleObject@CDocObjectServer@@UEAAJKPEAUtagSIZE@@@Z
// IOleObject::GetExtent -- retail RVA 0x258700 (mfc140u; the same body at 0x257790 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x90/8 = 18, IOleObject::GetExtent, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetExtent_XOleObject_CDocObjectServer__UEAAJKPEAUtagSIZE___Z(
    void* pThis, unsigned long dwDrawAspect, SIZE* psizel) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetExtent_XOleObject_COleServerDoc__UEAAJKPEAUtagSIZE___Z(
        pPart, dwDrawAspect, psizel);
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
// This is safe to reproduce because the constructor above seeds
// m_pViewSite = NULL and SetInPlaceSite below keeps it reference-counted.
// Deviations: the module-state push is not reproduced (that is uniform across
// this file), and ppIPSite is null-checked, which retail does not do.
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

// Symbol: ?GetMiscStatus@XOleObject@CDocObjectServer@@UEAAJKPEAK@Z
// IOleObject::GetMiscStatus -- retail RVA 0x2588a0 (mfc140u; the same body at 0x257930 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0xb0/8 = 22, IOleObject::GetMiscStatus, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_CDocObjectServer__UEAAJKPEAK_Z(
    void* pThis, unsigned long dwAspect, unsigned long* pdwStatus) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetMiscStatus_XOleObject_COleServerDoc__UEAAJKPEAK_Z(pPart, dwAspect, pdwStatus);
}

// Symbol: ?GetMoniker@XOleObject@CDocObjectServer@@UEAAJKKPEAPEAUIMoniker@@@Z
// IOleObject::GetMoniker -- retail RVA 0x2582f0 (mfc140u; the same body at 0x257380 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x40/8 = 8, IOleObject::GetMoniker, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetMoniker_XOleObject_CDocObjectServer__UEAAJKKPEAPEAUIMoniker___Z(
    void* pThis, unsigned long dwAssign, unsigned long dwWhichMoniker, IMoniker** ppmk) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetMoniker_XOleObject_COleServerDoc__UEAAJKKPEAPEAUIMoniker___Z(
        pPart, dwAssign, dwWhichMoniker, ppmk);
}

// PARTIAL: IPrint::GetPageInfo -- retail (0x256250 in mfc140; RVA 0x257190 in
// mfc140u, re-read there instruction by instruction), in program order.  The
// XPrint part sits at server + 0x88, so the `-0x50` / `-0x40` / `-0x38` the
// body uses are m_pModuleState (+0x38), m_pOwner (+0x48) and m_nFirstPage
// (+0x50):
//     AFX_MANAGE_STATE(server + 0x38)
//     pos = m_pOwner->vtbl[0xe0/8 = 28]();        // GetFirstViewPosition, 0x2571d6
//     if (pos == NULL) return E_UNEXPECTED;       // 0x2572a7
//     pView = m_pOwner->vtbl[0xe8/8 = 29](&pos);  // GetNextView, 0x2571fd
//     if (pView == NULL) return E_UNEXPECTED;     // 0x2572a7
//     CPrintInfo info;                            // ctor 0x2830a0 = ??0CPrintInfo@@QEAA@XZ
//     info.m_bDocObject = TRUE;                   // +0x08, 0x257219
//     info.m_dwFlags = 0x10;                      // +0x48, 0x257226
//     if (!pView->vtbl[0x350/8 = 106](&info))     // OnPreparePrinting, 0x25723e
//         hr = E_UNEXPECTED;                      // 0x257248
//     else {
//         PRINTDLG& pd = *info.m_pPD->m_pd;       // m_pPD is +0x00 of the CPrintInfo,
//                                                 //   m_pd the reference at +0x130 of
//                                                 //   the CPrintDialog
//         if (pnFirstPage) *pnFirstPage = m_nFirstPage != -1 ? m_nFirstPage
//                                                            : pd.nMinPage;   // +0x30
//         if (pcPages) *pcPages = pd.nToPage == 0xffff ? 0xffff               // +0x2e
//                                : pd.nToPage - pd.nFromPage + 1;             // +0x2c
//         hr = S_OK;
//     }
//     ~CPrintInfo();                              // 0x283160
//     return hr;
// The CPrintInfo field names come from the real afxext.h on this host
// (m_pPD +0x00, m_bDocObject +0x08, m_nOffsetPage +0x44, m_dwFlags +0x48);
// the PRINTDLG WORD offsets are nFromPage/nToPage/nMinPage.  An earlier note
// said the pages came from "m_pPD->[0x30]", one indirection short.
//
// What is reproduced: the view walk, through OpenMFC's CDocument virtuals as
// GetControllingFrame above does it, and the E_UNEXPECTED for a document with
// no view.  What is NOT, and why: OpenMFC declares no CPrintInfo type and its
// ??0CPrintInfo thunk (core/view/CPrintInfo.cpp) only registers the address --
// it allocates no m_pPD -- so neither OnPreparePrinting nor the m_pPD->m_pd
// reads can be made.  That leg therefore reports E_UNEXPECTED, which is
// retail's own answer when OnPreparePrinting fails, with the out parameters
// left unwritten exactly as retail leaves them on that path.  The generated
// `return 0` told the caller the page range had been filled in.  Deviation:
// the pThis / m_pOwner null guards; retail would fault.
// Symbol: ?GetPageInfo@XPrint@CDocObjectServer@@UEAAJPEAJ0@Z
extern "C" long MS_ABI impl__GetPageInfo_XPrint_CDocObjectServer__UEAAJPEAJ0_Z(
    void* pThis, long* pnFirstPage, long* pcPages) {
    (void)pnFirstPage;   // written only on the OnPreparePrinting leg (see above)
    (void)pcPages;
    if (pThis == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    void* pServer = ServerFromPart(pThis, kOff_m_xPrint);
    COleServerDoc* pOwnerDoc = Member<COleServerDoc*>(pServer, kOff_m_pOwner);
    if (pOwnerDoc == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    CDocument* pOwner = pOwnerDoc;   // single, non-virtual inheritance
    void* pos = pOwner->GetFirstViewPosition();
    if (pos == nullptr) return E_UNEXPECTED;                                 // 0x2572a7
    CView* pView = pOwner->GetNextView(pos);
    if (pView == nullptr) return E_UNEXPECTED;                               // 0x2572a7
    // CPrintInfo / OnPreparePrinting leg not reproducible here (see above).
    return E_UNEXPECTED;                                                     // 0x257248
}

// Symbol: ?GetRect@XOleDocumentView@CDocObjectServer@@UEAAJPEAUtagRECT@@@Z
// IOleDocumentView::GetRect -- retail RVA 0x259e70 (mfc140u; the same body at
// 0x258ef0 in mfc140), the whole body.  server = this - 0x78:
//     AFX_MANAGE_STATE(server + 0x38)                        // -0x40
//     *prcView = *(RECT*)(m_pOwner->m_pInPlaceFrame + 0x248) // -0x30 = +0x48, then
//                                                            //   +0x250, then one
//                                                            //   movups/movdqu pair
//     return S_OK                                            // xor %eax,%eax
// That 16-byte copy is COleServerDoc::GetItemPosition inlined: its own retail
// body (core/ole/COleServerDoc.cpp cites it, mfc140u 0x265e00) is the same
// load of m_pInPlaceFrame + 0x248 with the same absent null check.
// GetItemPosition is non-virtual, so the forward to its impl__ thunk below IS
// the retail call; no dispatch is bypassed.  PARTIAL only in that the thunk is
// itself a documented partial today: it reports an empty rect because OpenMFC
// models no in-place frame.  When it starts copying the real rect, so does
// this.  Deviations: pThis / m_pOwner / prcView null guards (retail faults on
// a NULL frame, and m_pInPlaceFrame is never set in this DLL, so the thunk's
// empty rect is what a caller sees today rather than a fault).
extern "C" long MS_ABI impl__GetRect_XOleDocumentView_CDocObjectServer__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* prcView) {
    COleServerDoc* pOwner = OwnerFromDocumentViewPart(pThis);
    if (pOwner == nullptr || prcView == nullptr) {
        return E_UNEXPECTED;   // deviation: retail would fault
    }
    impl__GetItemPosition_COleServerDoc__QEBAXPEAUtagRECT___Z(pOwner, prcView);
    return S_OK;
}

// Symbol: ?GetUserClassID@XOleObject@CDocObjectServer@@UEAAJPEAU_GUID@@@Z
// IOleObject::GetUserClassID -- retail RVA 0x2585b0 (mfc140u; the same body at 0x257640 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x78/8 = 15, IOleObject::GetUserClassID, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetUserClassID_XOleObject_CDocObjectServer__UEAAJPEAU_GUID___Z(
    void* pThis, CLSID* pClsid) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetUserClassID_XOleObject_COleServerDoc__UEAAJPEAU_GUID___Z(pPart, pClsid);
}

// Symbol: ?GetUserType@XOleObject@CDocObjectServer@@UEAAJKPEAPEA_W@Z
// IOleObject::GetUserType -- retail RVA 0x258610 (mfc140u; the same body at 0x2576a0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x80/8 = 16, IOleObject::GetUserType, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__GetUserType_XOleObject_CDocObjectServer__UEAAJKPEAPEA_W_Z(
    void* pThis, unsigned long dwFormOfType, wchar_t** pszUserType) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__GetUserType_XOleObject_COleServerDoc__UEAAJKPEAPEA_W_Z(
        pPart, dwFormOfType, pszUserType);
}

// Symbol: ?InitFromData@XOleObject@CDocObjectServer@@UEAAJPEAUIDataObject@@HK@Z
// IOleObject::InitFromData -- retail RVA 0x258350 (mfc140u; the same body at 0x2573e0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x48/8 = 9, IOleObject::InitFromData, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__InitFromData_XOleObject_CDocObjectServer__UEAAJPEAUIDataObject__HK_Z(
    void* pThis, IDataObject* pDataObject, int fCreation, unsigned long dwReserved) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__InitFromData_XOleObject_COleServerDoc__UEAAJPEAUIDataObject__HK_Z(
        pPart, pDataObject, fCreation, dwReserved);
}

// Symbol: ?IsUpToDate@XOleObject@CDocObjectServer@@UEAAJXZ
// IOleObject::IsUpToDate -- retail RVA 0x258560 (mfc140u; the same body at 0x2575f0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x70/8 = 14, IOleObject::IsUpToDate, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__IsUpToDate_XOleObject_CDocObjectServer__UEAAJXZ(
    void* pThis) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__IsUpToDate_XOleObject_COleServerDoc__UEAAJXZ(pPart);
}

// Symbol: ?Open@XOleDocumentView@CDocObjectServer@@UEAAJXZ
// IOleDocumentView::Open -- retail RVA 0x259fe0 (mfc140u; the same body at
// 0x259060 in mfc140), the whole body.  server = this - 0x78: the module state
// is `-0x40(%rcx)` = +0x38 and m_pOwner is `-0x30(%rbx)` = +0x48:
//     AFX_MANAGE_STATE(server + 0x38)
//     return (m_pOwner + 0x278)->vtbl[0x58/8 = 11](   // IOleObject::DoVerb
//         -2,      // OLEIVERB_OPEN: `lea -0x2(%rbx),%edx` with rbx zeroed
//         NULL,    // lpmsg        (r8d zeroed)
//         NULL,    // pActiveSite  (r9d zeroed)
//         0,       // lindex       ([rsp+0x20] = ebx = 0)
//         NULL,    // hwndParent   ([rsp+0x28] = rbx = 0)
//         NULL)    // lprcPosRect  ([rsp+0x30] = rbx = 0)
// Forwarded to COleServerDoc's XOleObject::DoVerb thunk with the sub-object
// address as `this` (layout note at the top); the HRESULT is returned
// unchanged.  Deviation: the pThis / m_pOwner null guard; retail would fault.
extern "C" long MS_ABI impl__Open_XOleDocumentView_CDocObjectServer__UEAAJXZ(
    void* pThis) {
    COleServerDoc* pOwner = OwnerFromDocumentViewPart(pThis);
    if (pOwner == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__DoVerb_XOleObject_COleServerDoc__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(
        OwnerAt(pOwner, kOwnerOff_m_xOleObject), OLEIVERB_OPEN, nullptr, nullptr, 0,
        nullptr, nullptr);
}

// PARTIAL: IPrint::Print -- retail (0x2563b0 in mfc140; RVA 0x2572f0 in
// mfc140u, re-read there) is the full IPrint print pump.  Its opening, in
// program order (XPrint part at server + 0x88, so `-0x50` / `-0x40` / `-0x38`
// of `this` are m_pModuleState, m_pOwner and m_nFirstPage; the stack
// arguments are pstgmOptions, pcallback, nFirstPage, pcPagesPrinted,
// pnLastPage in that order):
//     AFX_MANAGE_STATE(server + 0x38)
//     if (pcPagesPrinted == NULL || pptd == NULL || ppPageSet == NULL)
//         return E_POINTER;                       // 0x25734b..0x257360 -> 0x257c7e
//     if (*pptd == NULL) return E_INVALIDARG;     // 0x257366 -> 0x25736e, 0x80070057
//     m_nFirstPage = nFirstPage;                  // 0x25737f, `mov %edi,-0x38(%r14)`
//     *pcPagesPrinted = 0;                        // 0x257383
//     pos = m_pOwner->vtbl[0xe0/8 = 28]();        // GetFirstViewPosition, 0x257398
//     if (pos == NULL) return E_UNEXPECTED;       // 0x257c76
//     pView = m_pOwner->vtbl[0xe8/8 = 29](&pos);  // GetNextView, 0x2573bf
//     if (pView == NULL) return E_UNEXPECTED;     // 0x257c76
//     CPrintInfo info;                            // ctor 0x2830a0; m_bDocObject = TRUE,
//     info.m_dwFlags = grfFlags;                  //   m_nOffsetPage = nFirstPage
//     info.m_pPD->m_pd.hDC = _AfxOleCreateDC(*pptd);   // unexported helper 0x260690:
//                                                 //   CreateDC from the four
//                                                 //   td*Offset WORDs at +4..+0xa
//     if (info.m_pPD->m_pd.hDC == NULL) {         // 0x257409
//         if (grfFlags & PRINTFLAG_MAYBOTHERUSER) AfxMessageBox(0xf106);  // 0x257420
//         return E_UNEXPECTED;                    // 0x257425
//     }
//     ...the print pump: CPrintDialog / printer CDC and CView slots
//     0x350/8 = 106 (OnPreparePrinting), 0x358/8 = 107 (OnBeginPrinting),
//     0x320/8 = 100 (OnPrepareDC), 0x360/8 = 108 (OnPrint), 0x368/8 = 109
//     (OnEndPrinting), with the IContinueCallback's slot 4 (FContinuePrinting)
//     polled per page, and *pcPagesPrinted accumulated at 0x257a86...
// Correction to an earlier note here: retail Print does NOT call this class's
// DoPreparePrinting / DoBeginPrinting / DoPrint / DoEndPrinting helpers --
// those are one-instruction tail-jumps into exactly the same CView slots
// (0x256050 -> 0x350, 0x2560b0 -> 0x358, 0x256090 -> 0x360, 0x2560d0 -> 0x368,
// 0x256070 -> 0x320 in mfc140; they ignore their own `this`) and the compiler
// inlined them here, so the disassembly dispatches on the view directly.
//
// What is reproduced: the three E_POINTER tests, the E_INVALIDARG test, the
// m_nFirstPage store, the *pcPagesPrinted = 0 store and the view walk (through
// OpenMFC's CDocument virtuals, as GetControllingFrame above does it) with its
// E_UNEXPECTED.  What is NOT, and why: the pump needs a CPrintInfo with a real
// CPrintDialog behind m_pPD (OpenMFC's ??0CPrintInfo in core/view/CPrintInfo.cpp
// allocates none, and no CPrintInfo type is declared), a printer CDC, and the
// two CView virtuals (OnPrepareDC, OnPrint) that DoPrepareDC / DoPrint are
// stubbed for.  That leg therefore reports E_UNEXPECTED -- retail's own answer
// when no printer DC can be created for *pptd -- with *pcPagesPrinted already
// zero, which is also what retail leaves there on that path; the
// AfxMessageBox for PRINTFLAG_MAYBOTHERUSER is not reproduced.  The generated
// `return 0` told the caller the job had printed and left *pcPagesPrinted
// unwritten.  Deviation: the pThis / m_pOwner null guards; retail would fault.
// Symbol: ?Print@XPrint@CDocObjectServer@@UEAAJKPEAPEAUtagDVTARGETDEVICE@@PEAPEAUtagPAGESET@@PEAUtagSTGMEDIUM@@PEAUIContinueCallback@@JPEAJ4@Z
extern "C" long MS_ABI impl__Print_XPrint_CDocObjectServer__UEAAJKPEAPEAUtagDVTARGETDEVICE__PEAPEAUtagPAGESET__PEAUtagSTGMEDIUM__PEAUIContinueCallback__JPEAJ4_Z(
    void* pThis, unsigned long grfFlags, DVTARGETDEVICE** pptd, PAGESET** ppPageSet,
    STGMEDIUM* pstgmOptions, IContinueCallback* pcallback, long nFirstPage,
    long* pcPagesPrinted, long* pnLastPage) {
    (void)grfFlags;        // consumed only by the print pump (see above)
    (void)pstgmOptions;
    (void)pcallback;
    (void)pnLastPage;
    if (pcPagesPrinted == nullptr || pptd == nullptr || ppPageSet == nullptr) {
        return E_POINTER;                                                    // 0x257c7e
    }
    if (*pptd == nullptr) {
        return E_INVALIDARG;                                                 // 0x25736e
    }
    if (pThis == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    void* pServer = ServerFromPart(pThis, kOff_m_xPrint);
    Member<LONG>(pServer, kOff_m_nFirstPage) = static_cast<LONG>(nFirstPage);   // 0x25737f
    *pcPagesPrinted = 0;                                                     // 0x257383
    COleServerDoc* pOwnerDoc = Member<COleServerDoc*>(pServer, kOff_m_pOwner);
    if (pOwnerDoc == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    CDocument* pOwner = pOwnerDoc;   // single, non-virtual inheritance
    void* pos = pOwner->GetFirstViewPosition();
    if (pos == nullptr) return E_UNEXPECTED;                                 // 0x257c76
    CView* pView = pOwner->GetNextView(pos);
    if (pView == nullptr) return E_UNEXPECTED;                               // 0x257c76
    // CPrintInfo / printer-DC / print-pump leg not reproducible here (see above).
    return E_UNEXPECTED;                                                     // 0x257425
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

// Symbol: ?SetClientSite@XOleObject@CDocObjectServer@@UEAAJPEAUIOleClientSite@@@Z
// IOleObject::SetClientSite -- retail RVA 0x2580b0 (mfc140u; the same body at
// 0x257140 in mfc140), in program order.  server = this - 0x68 (the
// `lea -0x68(%rcx),%rdi` at 0x2580bf):
//     AFX_MANAGE_STATE(server + 0x38)
//     hr = (m_pOwner + 0x278)->vtbl[0x18/8 = 3](pClientSite)   // IOleObject::SetClientSite
//     if (hr == S_OK) {                          // `test %eax,%eax; jne` at 0x2580f1: exactly 0
//         CDocObjectServer::ReleaseDocSite(server)                // call 0x256e00
//         if (pClientSite != NULL)                                // 0x2580fd
//             hr = pClientSite->QueryInterface(&IID_IOleDocumentSite,
//                                              &m_pDocSite)      // slot 0; out = server + 0x40
//     }
//     return hr
// The IID is the `lea` operand at 0x258109, .rdata RVA 0x2d9f90 (mfc140u), whose
// sixteen bytes read c7 bc 22 b7 68 4e 1b 10 a2 bc 00 aa 00 40 47 70 =
// {B722BCC7-4E68-101B-A2BC-00AA00404770}, IID_IOleDocumentSite.  Note that the
// QueryInterface writes straight into m_pDocSite (NULL on failure, per COM) and
// its HRESULT replaces the SetClientSite one, and that a NULL pClientSite
// leaves m_pDocSite released and NULL with hr == S_OK.  The forward is to
// COleServerDoc's own XOleObject::SetClientSite thunk with the sub-object
// address as `this` (layout note at the top).  Deviation: the pThis / m_pOwner
// null guard; retail would fault.
extern "C" long MS_ABI impl__SetClientSite_XOleObject_CDocObjectServer__UEAAJPEAUIOleClientSite___Z(
    void* pThis, IOleClientSite* pClientSite) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    long hr = impl__SetClientSite_XOleObject_COleServerDoc__UEAAJPEAUIOleClientSite___Z(
        pPart, pClientSite);
    if (hr == S_OK) {
        void* pServer = ServerFromPart(pThis, kOff_m_xOleObject);
        impl__ReleaseDocSite_CDocObjectServer__QEAAXXZ(pServer);          // 0x2580f8
        if (pClientSite != nullptr) {
            hr = pClientSite->QueryInterface(
                kIID_IOleDocumentSite,
                reinterpret_cast<void**>(&Member<IOleDocumentSite*>(pServer, kOff_m_pDocSite)));
        }
    }
    return hr;
}

// Symbol: ?SetColorScheme@XOleObject@CDocObjectServer@@UEAAJPEAUtagLOGPALETTE@@@Z
// IOleObject::SetColorScheme -- retail RVA 0x258910 (mfc140u; the same body at 0x2579a0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0xb8/8 = 23, IOleObject::SetColorScheme, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__SetColorScheme_XOleObject_CDocObjectServer__UEAAJPEAUtagLOGPALETTE___Z(
    void* pThis, LOGPALETTE* pLogpal) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__SetColorScheme_XOleObject_COleServerDoc__UEAAJPEAUtagLOGPALETTE___Z(
        pPart, pLogpal);
}

// Symbol: ?SetExtent@XOleObject@CDocObjectServer@@UEAAJKPEAUtagSIZE@@@Z
// IOleObject::SetExtent -- retail RVA 0x258680 (mfc140u; the same body at
// 0x257710 in mfc140), in program order:
//     AFX_MANAGE_STATE(server + 0x38)                    // this - 0x30
//     if (m_pOwner->m_pDocObjectServer != NULL)          // this - 0x20 -> +0x268, 0x2586a9
//         return E_FAIL;                                 // 0x80004005 at 0x2586b3
//     return (m_pOwner + 0x278)->vtbl[0x88/8 = 17](dwDrawAspect, psizel)   // IOleObject::SetExtent
// The guard reads m_pOwner + 0x268 exactly as retail does; in this DLL that is
// zero-filled padding (layout note at the top), so the forward is always taken
// -- as it is in retail for a document that owns no doc-object server.  The
// target thunk (core/ole/COleServerDoc.cpp) is a documented partial that
// returns E_INVALIDARG for a NULL psizel and otherwise E_FAIL, retail's
// "item refused the extent" outcome, so this no longer reports the S_OK the
// bare stub did.  Deviation: the pThis / m_pOwner null guard; retail would
// fault.
extern "C" long MS_ABI impl__SetExtent_XOleObject_CDocObjectServer__UEAAJKPEAUtagSIZE___Z(
    void* pThis, unsigned long dwDrawAspect, SIZE* psizel) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    COleServerDoc* pOwner = Member<COleServerDoc*>(
        ServerFromPart(pThis, kOff_m_xOleObject), kOff_m_pOwner);
    if (*static_cast<void**>(OwnerAt(pOwner, kOwnerOff_m_pDocObjectServer)) != nullptr) {
        return E_FAIL;                                                       // 0x2586b3
    }
    return impl__SetExtent_XOleObject_COleServerDoc__UEAAJKPEAUtagSIZE___Z(
        pPart, dwDrawAspect, psizel);
}

// Symbol: ?SetHostNames@XOleObject@CDocObjectServer@@UEAAJPEB_W0@Z
// IOleObject::SetHostNames -- retail RVA 0x2581b0 (mfc140u; the same body at 0x257240 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x28/8 = 5, IOleObject::SetHostNames, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__SetHostNames_XOleObject_CDocObjectServer__UEAAJPEB_W0_Z(
    void* pThis, const wchar_t* szContainerApp, const wchar_t* szContainerObj) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__SetHostNames_XOleObject_COleServerDoc__UEAAJPEB_W0_Z(
        pPart, szContainerApp, szContainerObj);
}

// Symbol: ?SetInPlaceSite@XOleDocumentView@CDocObjectServer@@UEAAJPEAUIOleInPlaceSite@@@Z
// IOleDocumentView::SetInPlaceSite -- retail RVA 0x259c90 (mfc140u; the same
// body at 0x258d10 in mfc140), transcribed in full.  server = this - 0x78:
//     AFX_MANAGE_STATE(server + 0x38)                  // -0x40
//     rcx = m_pOwner                                   // -0x30 = +0x48
//     if (m_pOwner->m_pInPlaceFrame != NULL)           // `cmpq $0,0x250(%rcx)` at 0x259cb2
//         (m_pOwner + 0x288)->vtbl[0x28/8 = 5]();      // IOleInPlaceObject::InPlaceDeactivate,
//                                                      //   result discarded
//     if (m_pViewSite != NULL) m_pViewSite->Release(); // -0x18 = +0x60; slot 0x10/8 = 2
//     m_pViewSite = pIPSite;                           // 0x259ce6
//     if (pIPSite != NULL) pIPSite->AddRef();          // slot 0x8/8 = 1
//     return S_OK;                                     // xor %eax,%eax at 0x259d17
// The guard reads m_pOwner + 0x250 exactly as retail does; in this DLL that
// word is zero-filled padding (layout note at the top), so the deactivation is
// skipped -- retail's own behaviour for a document with no in-place frame --
// and it starts firing when COleServerDoc.cpp models the frame.  The
// deactivation goes to COleServerDoc's own XOleInPlaceObject thunk with the
// sub-object address as `this`.  The reference-counting half is exact and is
// safe because the constructor seeds m_pViewSite = NULL.  Deviation: the pThis
// / m_pOwner null guard; retail would fault.
extern "C" long MS_ABI impl__SetInPlaceSite_XOleDocumentView_CDocObjectServer__UEAAJPEAUIOleInPlaceSite___Z(
    void* pThis, IOleInPlaceSite* pIPSite) {
    COleServerDoc* pOwner = OwnerFromDocumentViewPart(pThis);
    if (pOwner == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    if (*static_cast<void**>(OwnerAt(pOwner, kOwnerOff_m_pInPlaceFrame)) != nullptr) {
        impl__InPlaceDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ(       // 0x259cca
            OwnerAt(pOwner, kOwnerOff_m_xOleInPlaceObject));
    }
    void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
    IOleInPlaceSite*& pViewSite = Member<IOleInPlaceSite*>(pServer, kOff_m_pViewSite);
    if (pViewSite != nullptr) {
        pViewSite->Release();                                                   // 0x259ce0
    }
    pViewSite = pIPSite;                                                        // 0x259ce6
    if (pIPSite != nullptr) {
        pIPSite->AddRef();                                                      // 0x259cf9
    }
    return S_OK;
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

// Symbol: ?SetMoniker@XOleObject@CDocObjectServer@@UEAAJKPEAUIMoniker@@@Z
// IOleObject::SetMoniker -- retail RVA 0x258280 (mfc140u; the same body at 0x257310 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x38/8 = 7, IOleObject::SetMoniker, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__SetMoniker_XOleObject_CDocObjectServer__UEAAJKPEAUIMoniker___Z(
    void* pThis, unsigned long dwWhichMoniker, IMoniker* pmk) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__SetMoniker_XOleObject_COleServerDoc__UEAAJKPEAUIMoniker___Z(
        pPart, dwWhichMoniker, pmk);
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
// IOleDocumentView::Show -- retail RVA 0x259f00 (mfc140u; the same body at
// 0x258f80 in mfc140), transcribed in full.  server = this - 0x78:
//     AFX_MANAGE_STATE(server + 0x38)                 // -0x40
//     rcx = m_pOwner                                  // -0x30 = +0x48
//     if (bShow != 0) {                               // 0x259f21
//         BOOL b = COleServerDoc::ActivateInPlace(m_pOwner);   // call 0x266da0, non-virtual
//         hr = b ? S_OK : E_FAIL;                     // neg/sbb/not/and $0x80004005,
//                                                     //   0x259f2a .. 0x259f30
//     } else {
//         hr = (m_pOwner + 0x288)->vtbl[0x28/8 = 5]();   // IOleInPlaceObject::InPlaceDeactivate
//     }
//     return hr
// ActivateInPlace is a plain (non-virtual) member, so the call to its thunk is
// the retail call; the InPlaceDeactivate arm goes to COleServerDoc's own
// XOleInPlaceObject thunk with the sub-object address as `this` (layout note
// at the top).  Today both targets are documented stubs in
// core/ole/COleServerDoc.cpp -- ActivateInPlace returns FALSE, so bShow != 0
// yields E_FAIL here, which is retail's own answer when in-place activation
// fails; InPlaceDeactivate returns S_OK, which is retail's value on every
// normal path.  Deviation: the pThis / m_pOwner null guard; retail would fault.
extern "C" long MS_ABI impl__Show_XOleDocumentView_CDocObjectServer__UEAAJH_Z(void* pThis, int bShow) {
    COleServerDoc* pOwner = OwnerFromDocumentViewPart(pThis);
    if (pOwner == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    if (bShow != 0) {
        return impl__ActivateInPlace_COleServerDoc__QEAAHXZ(pOwner) ? S_OK : E_FAIL;
    }
    return impl__InPlaceDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ(
        OwnerAt(pOwner, kOwnerOff_m_xOleInPlaceObject));
}

// Symbol: ?UIActivate@XOleDocumentView@CDocObjectServer@@UEAAJH@Z
// IOleDocumentView::UIActivate -- retail (0x258ff0):
//     AFX_MANAGE_STATE(server + 0x38)          // server = this - 0x78
//     if (fUIActivate != 0)                    // test %ebx,%ebx; je, 0x25900e
//         return server->vtbl[0xc0/8 = 24]();  // CDocObjectServer::OnActivateView
//     return (m_pOwner + 0x288)->vtbl[0x30/8 = 6]();   // IOleInPlaceObject::UIDeactivate
// Both arms are a single call (through the CFG dispatch pointer, followed by
// the module-state restore) whose HRESULT is returned unchanged.  Slot 24 is
// OnActivateView: the class vtable at .rdata 0x32d2e8 holds 0x2586a0 there, and
// 0x2586a0 is the RVA the map gives for
// ?OnActivateView@CDocObjectServer@@MEAAJXZ.  m_pOwner + 0x288 is
// COleServerDoc::m_xOleInPlaceObject (see the layout note at the top).
// Re-read in mfc140u at RVA 0x259f70 (same body): `lea -0x78(%rcx),%rdi`,
// `test %ebx,%ebx; je` at 0x259f8e, slot 0xc0 on the server for the non-zero
// arm and `add $0x288` / slot 0x30 on m_pOwner for the zero arm.
// PARTIAL, in one respect:
//   * fUIActivate != 0 IS reproduced, as a direct call to OnActivateView's own
//     impl__ thunk (defined above in this file) instead of a dispatch through
//     the CDocObjectServer vtable, which this DLL does not build.  Consequence,
//     stated plainly: a derived class that overrides OnActivateView is bypassed.
//     Today that thunk is itself a documented stub returning S_OK, so this arm
//     has no observable effect yet -- it picks one up automatically.
//   * fUIActivate == 0 is reproduced as a direct call to COleServerDoc's own
//     XOleInPlaceObject::UIDeactivate thunk with the sub-object address
//     m_pOwner + 0x288 as `this` (layout note at the top: a nested interface
//     class with one vtable, so the direct call is the retail dispatch).  That
//     thunk is a documented stub returning S_OK today, which is also retail's
//     value on every normal path of UIDeactivate.
// Deviations: the pThis / m_pOwner null guards; retail would fault.
extern "C" long MS_ABI impl__UIActivate_XOleDocumentView_CDocObjectServer__UEAAJH_Z(
    void* pThis, int fUIActivate) {
    if (pThis == nullptr) {
        return E_UNEXPECTED;   // deviation: retail has no such guard
    }
    if (fUIActivate != 0) {
        void* pServer = ServerFromPart(pThis, kOff_m_xOleDocumentView);
        return impl__OnActivateView_CDocObjectServer__MEAAJXZ(pServer);
    }
    COleServerDoc* pOwner = OwnerFromDocumentViewPart(pThis);
    if (pOwner == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__UIDeactivate_XOleInPlaceObject_COleServerDoc__UEAAJXZ(
        OwnerAt(pOwner, kOwnerOff_m_xOleInPlaceObject));
}

// Symbol: ?Unadvise@XOleObject@CDocObjectServer@@UEAAJK@Z
// IOleObject::Unadvise -- retail RVA 0x2587e0 (mfc140u; the same body at 0x257870 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0xa0/8 = 20, IOleObject::Unadvise, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__Unadvise_XOleObject_CDocObjectServer__UEAAJK_Z(
    void* pThis, unsigned long dwConnection) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__Unadvise_XOleObject_COleServerDoc__UEAAJK_Z(pPart, dwConnection);
}

// Symbol: ?Update@XOleObject@CDocObjectServer@@UEAAJXZ
// IOleObject::Update -- retail RVA 0x258510 (mfc140u; the same body at 0x2575a0 in
// mfc140).  The IOleObject-forward shape described above the helper: module
// state from server + 0x38 (`-0x30` of the part), m_pOwner from server + 0x48
// (`-0x20`), then `add $0x278` and slot 0x68/8 = 13, IOleObject::Update, on the
// owning document's XOleObject part, arguments and HRESULT untouched.
extern "C" long MS_ABI impl__Update_XOleObject_CDocObjectServer__UEAAJXZ(
    void* pThis) {
    void* pPart = OwnerOleObjectPart(pThis);
    if (pPart == nullptr) return E_UNEXPECTED;   // deviation: retail would fault
    return impl__Update_XOleObject_COleServerDoc__UEAAJXZ(pPart);
}
