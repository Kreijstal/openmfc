// COleFrameHook — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp
//
// Every retail behaviour described below was decoded from the retail image
// rather than inferred, the way phase4/src/core/ole/COleControl.cpp describes.
// (The exceptions, each flagged where it sits: the typed C++ overrides
// COleFrameHook::OnResizeBorder and ::OnGetClipboardData have no retail
// counterpart at all, and the deviations OpenMFC's narrower class layout
// forces are called out at each site.)  Two facts about the tooling matter
// for reading the citations:
//
//   * `disas.py --u` reads mfc140u.dll and `disas.py` reads mfc140.dll.  The
//     BODIES are byte-identical between the two images, but every export sits
//     at a different RVA.  mfc140u_rva_symbols.json resolves only 15 of this
//     class's 33 functions, while the mfc140 (ANSI) map resolves 32, so most
//     bodies below were read at their ANSI RVA.  Each comment names the image
//     it is quoting; where both are known both are given.
//   * The COleFrameHook class vftable is at mfc140u .rdata 0x32c0f0 (planted
//     by the ctor at mfc140u 0x247db4) and at mfc140 .rdata 0x329f30 (ANSI
//     ctor 0x246214).  Slots 22..33 are this class's own virtuals; slot 0 is
//     CCmdTarget::GetRuntimeClass and slot 16 is COleFrameHook's own
//     GetInterfaceMap (mfc140u 0x2492f0).  The ANSI vftable names all twelve
//     of the own virtuals outright, because the mfc140 map resolves all
//     twelve bodies.  The mfc140u map resolves only THREE of the twelve —
//     OnActivate (slot 24), OnDocActivate (25) and OnContextHelp (26); it
//     does NOT resolve OnRecalcLayout, so do not expect `disas.py --u` to
//     name it.  The two vftables hold the same twelve functions at the same
//     slot indices, and in RVA order the inter-function byte deltas are
//     identical in both images (0x80, 0xd0, 0x30, 0xb0, 0x40, 0x80, 0xd0,
//     0x50, 0x30, 0x30, 0x40), with the three doubly-named ones landing on
//     the same slots.  That is what names the other nine mfc140u addresses.
//     Slot -> (mfc140u RVA / mfc140 RVA):
//        22 OnRecalcLayout          0x247ef0 / 0x246350
//        23 OnPreTranslateMessage   0x2481e0 / 0x246640
//        24 OnActivate              0x2482b0 / 0x246710
//        25 OnDocActivate           0x247f70 / 0x2463d0
//        26 OnContextHelp           0x248040 / 0x2464a0
//        27 OnEnableModeless        0x248300 / 0x246760
//        28 OnUpdateFrameTitle      0x248330 / 0x246790
//        29 OnPaletteChanged        0x248360 / 0x2467c0
//        30 OnQueryNewPalette       0x2483a0 / 0x246800
//        31 OnInitMenuPopup         0x248160 / 0x2465c0
//        32 OnInitMenu              0x248120 / 0x246580
//        33 OnMenuSelect            0x248070 / 0x2464d0
//     Non-virtual: NotifyAllInPlace 0x2483e0 / 0x246840,
//     DoContextSensitiveHelp 0x248560 / 0x2469c0,
//     DoEnableModeless 0x248580 / 0x2469e0.
//
// RETAIL MEMBER LAYOUT.  Retail's declaration lives in MFC's private
// src/oleimpl2.h, which is not on this host, so the layout was reconstructed
// from the constructor (mfc140u 0x247d90), the destructor (mfc140u 0x247e80)
// and the nested-interface method prologues.  The total size is not guessed:
// COleClientItem::XOleIPSite::GetWindowContext (mfc140 0x248270) allocates the
// hook with `mov $0x90,%ecx ; call operator new` at 0x248392, so
// sizeof(COleFrameHook) == 0x90.  See RetailCOleFrameHook below, whose
// static_asserts pin every offset cited in this file.
//
// COleFrameHook is NOT a DECLARE_DYNAMIC class in retail: vftable slot 0 is
// CCmdTarget::GetRuntimeClass, and a scan of mfc140u for a CRuntimeClass whose
// m_lpszClassName points at a bare "COleFrameHook" string finds none -- there
// is no such string in the image at all.  The name occurs three times, every
// one of them inside a decorated RTTI type descriptor, which a CRuntimeClass
// never points at: ".?AVXOleCommandTarget@COleFrameHook@@" (mfc140u 0x3bc570),
// ".?AVXOleInPlaceFrame@COleFrameHook@@" (0x3bc5a8) and ".?AVCOleFrameHook@@"
// (0x3bc5e0) -- the third is the class's own type descriptor, which an earlier
// revision of this comment wrongly said did not exist.  OpenMFC's afxole.h declares
// DECLARE_DYNAMIC(COleFrameHook) and OlecoreSupport.cpp:175 defines the
// descriptor; that is an OpenMFC addition, harmless but not retail.
//
// WHAT OPENMFC MODELS, AND WHAT IT DOES NOT.  include/openmfc/afxole.h:871
// declares only three members — m_pFrameWnd, m_pActiveItem and
// m_lpActiveUIWindow — followed by char _oleframehook_padding[48].  Mapping
// them onto retail:
//   retail +0x40 m_pFrameWnd       -> OpenMFC m_pFrameWnd        (matches)
//   retail +0x50 m_pActiveItem     -> OpenMFC m_pActiveItem      (matches by name)
//   retail +0x48 m_lpActiveObject  -> OpenMFC m_lpActiveUIWindow (MISNAMED AND
//       MISTYPED: retail's member is an IOleInPlaceActiveObject*, set by
//       XOleInPlaceFrame::SetActiveObject at `mov %rbx,-0x38(%rdi)` inside
//       mfc140 0x247c50, and every use below calls IOleInPlaceActiveObject
//       slots.  OpenMFC types it LPOLEINPLACEUIWINDOW.  The two interfaces
//       agree only through slot 4 (IOleWindow::GetWindow and
//       ContextSensitiveHelp); slots 5+ are different methods, so the cast in
//       ActiveObject() below is only safe because nothing in this tree ever
//       stores an IOleInPlaceUIWindow there — the constructor sets it to NULL
//       and XOleInPlaceFrame::SetActiveObject is not reachable (see below).
//       A headerRequest asks for it to be renamed and retyped.)
// Retail members OpenMFC has no declaration for, hence the stubs below:
//   +0x58 m_hWndFrame, +0x60 (unread by anything decoded here),
//   +0x68 the in-place accelerator table, +0x70 m_nModelessCount,
//   +0x78 CString m_strObjName, +0x80 XOleInPlaceFrame sub-object,
//   +0x88 XOleCommandTarget sub-object.
//
// THE NESTED INTERFACES.  Retail compiles every XOleInPlaceFrame /
// XOleCommandTarget method with a METHOD_PROLOGUE that backs the incoming
// interface pointer up to the containing COleFrameHook by a fixed byte
// offset.  Both offsets are established by the constructor, which plants the
// two sub-object vftables at exactly those offsets (mfc140u 0x247dde and
// 0x247de5):
//   XOleInPlaceFrame   this - 0x80   vftable mfc140u .rdata 0x32c040, 15 slots
//                                    = IOleInPlaceFrame.  The back-up is also
//                                    visible literally as `lea -0x80(%rcx),%rdi`
//                                    at mfc140 0x24793f in
//                                    XOleInPlaceFrame::ContextSensitiveHelp.
//   XOleCommandTarget  this - 0x88   vftable mfc140u .rdata 0x32c0c0, 5 slots
//                                    = IOleCommandTarget, whose slot 4 is
//                                    XOleCommandTarget::Exec itself.  No `lea`
//                                    appears in these two bodies: Exec reads
//                                    m_pActiveItem as `mov -0x38(%rcx),%rdi`
//                                    (mfc140 0x2477db) and QueryStatus the same
//                                    at 0x24783b, and 0x88 - 0x38 == 0x50
//                                    agrees with the vftable placement.
// OpenMFC has neither sub-object: COleFrameHook's declaration has no nested
// classes, the constructor plants no interface vftables, and the interface map
// registered at phase4/src/detail/InterfaceMapsSupport.cpp:38-39 forwards
// straight to CCmdTarget with no entries of its own, so OpenMFC's
// COleFrameHook cannot hand out an IOleInPlaceFrame or an IOleCommandTarget at
// all.  There is therefore no pointer in this build from which the -0x80 /
// -0x88 back-up would land on a COleFrameHook, and all fourteen nested methods
// below are left as documented stubs rather than given bodies that would
// compute a garbage `this`.  Their parameter lists ARE corrected, because the
// auto-generated ones omitted the interface `this` entirely.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl__ exports called by the bodies in this file ----
// (C++ methods of other classes exist in this DLL only as impl__ thunks.)
// core/window/CWnd.cpp:786
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
// core/ole/COleClientItem.cpp:654 -- currently a documented stub that always
// returns NULL; calling it is still the faithful transcription of retail.
extern "C" CWnd* MS_ABI impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(
    COleClientItem* pThis);
// core/ole/COleClientItem.cpp:2161
extern "C" int MS_ABI impl__OnUpdateFrameTitle_COleClientItem__UEAAHXZ(
    COleClientItem* pThis);

// ---- forward declarations of exports defined later in this file ----
// NotifyAllInPlace's second parameter is a pointer to member function
// `BOOL (COleFrameHook::*)(BOOL)`.  Retail invokes it with `mov %r15,%rax ;
// call *__guard_dispatch_icall_fptr` after loading rcx with the hook and edx
// with the BOOL (mfc140u 0x2484e8..0x2484f5), i.e. it is a plain 8-byte code
// pointer taking (COleFrameHook*, BOOL) -- the MSVC representation of a
// pointer to a non-virtual member of a single-inheritance class.
typedef int(MS_ABI* PFN_HOOKNOTIFY)(COleFrameHook*, int);

extern "C" int MS_ABI impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
    COleFrameHook* pThis, int bParam, PFN_HOOKNOTIFY pfn);
extern "C" int MS_ABI impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z(
    COleFrameHook* pThis, int bEnter);
extern "C" int MS_ABI impl__DoEnableModeless_COleFrameHook__QEAAHH_Z(
    COleFrameHook* pThis, int bEnable);

namespace {

// ---------------------------------------------------------------------------
// Retail mfc140u COleFrameHook layout, as a file-local view.  This struct is
// documentation with proof attached -- it is NEVER used to access an OpenMFC
// COleFrameHook, whose layout is different (see the header comment).  Each
// offset is followed by the instruction that establishes it.
// ---------------------------------------------------------------------------
#pragma pack(push, 8)
struct RetailCOleFrameHook {
    void* vftable;                 // +0x00  mfc140u 0x18032c0f0, planted at 0x247db4
    char  ccmdtarget[0x38];        // +0x08  CCmdTarget tail; +0x38 is m_pModuleState,
                                   //        read as `mov -0x48(%rcx),%rdx` and handed
                                   //        to AFX_MAINTAIN_STATE2 by MOST of the
                                   //        XOleInPlaceFrame methods
                                   //        (mfc140 0x247a42 in GetBorder,
                                   //        0x80-0x48 == 0x38).  Not all: GetWindow
                                   //        (0x247910), SetStatusText (0x247ea0)
                                   //        and both XOleCommandTarget methods
                                   //        (0x2477d0, 0x247830) have no state
                                   //        prologue at all.
    void* m_pFrameWnd;             // +0x40  ctor `mov %rdi,0x40(%rsi)`      0x247dfd
    void* m_lpActiveObject;        // +0x48  ctor `mov %rcx,0x48(%rsi)` = 0  0x247df5
                                   //        stored by XOleInPlaceFrame::SetActiveObject
                                   //        `mov %rbx,-0x38(%rdi)`   mfc140 0x247c83
    void* m_pActiveItem;           // +0x50  ctor `mov %rbx,0x50(%rsi)`      0x247df9
    void* m_hWndFrame;             // +0x58  ctor `mov 0x40(%rdi),%rax ; mov %rax,0x58(%rsi)`
                                   //        0x247e01 -- i.e. pFrameWnd->m_hWnd.
                                   //        Read back by XOleInPlaceFrame::GetWindow
                                   //        as `mov -0x28(%rcx),%rax` (mfc140 0x24791b)
    void* unknown_0x60;            // +0x60  ctor `mov %rcx,0x60(%rsi)` = 0  0x247e09
                                   //        no reader appears in the bodies decoded here
    void* m_hAccelTable;           // +0x68  ctor `mov %rcx,0x68(%rsi)` = 0  0x247e0d;
                                   //        XOleInPlaceFrame::TranslateAccelerator
                                   //        (mfc140 0x247ff5) swaps it into
                                   //        m_pFrameWnd+0xf8 around PreTranslateMessage
    unsigned int m_nModelessCount; // +0x70  ctor `mov %ecx,0x70(%rsi)` = 0  0x247e11;
                                   //        DoEnableModeless is its only user
    unsigned int pad_0x74;
    void* m_strObjName;            // +0x78  a CString: the ctor fills it from the ATL
                                   //        string manager's nil string
                                   //        (`... ; add $0x18,%rax ; mov %rax,0x78(%rsi)`
                                   //        0x247dcf) and the dtor does the CStringData
                                   //        `lock xadd` release on it across
                                   //        0x247eb0..0x247ed3 (the xadd itself is
                                   //        at 0x247ebb).  Its only writer decoded
                                   //        here is XOleInPlaceFrame::SetActiveObject,
                                   //        which Empty()s it and then assigns
                                   //        pszObjName (mfc140 0x247c9c..0x247cb6);
                                   //        SetStatusText does NOT touch it (it builds
                                   //        a temporary CString), so it is named here
                                   //        for the object name it holds.
    void* m_xOleInPlaceFrame;      // +0x80  ctor plants 0x18032c040        0x247dde
    void* m_xOleCommandTarget;     // +0x88  ctor plants 0x18032c0c0        0x247de5
};
#pragma pack(pop)

static_assert(offsetof(RetailCOleFrameHook, m_pFrameWnd) == 0x40, "retail +0x40");
static_assert(offsetof(RetailCOleFrameHook, m_lpActiveObject) == 0x48, "retail +0x48");
static_assert(offsetof(RetailCOleFrameHook, m_pActiveItem) == 0x50, "retail +0x50");
static_assert(offsetof(RetailCOleFrameHook, m_hWndFrame) == 0x58, "retail +0x58");
static_assert(offsetof(RetailCOleFrameHook, m_hAccelTable) == 0x68, "retail +0x68");
static_assert(offsetof(RetailCOleFrameHook, m_nModelessCount) == 0x70, "retail +0x70");
static_assert(offsetof(RetailCOleFrameHook, m_strObjName) == 0x78, "retail +0x78");
static_assert(offsetof(RetailCOleFrameHook, m_xOleInPlaceFrame) == 0x80,
              "retail +0x80 -- XOleInPlaceFrame METHOD_PROLOGUE back-up offset");
static_assert(offsetof(RetailCOleFrameHook, m_xOleCommandTarget) == 0x88,
              "retail +0x88 -- XOleCommandTarget METHOD_PROLOGUE back-up offset");
static_assert(sizeof(RetailCOleFrameHook) == 0x90,
              "sizeof(COleFrameHook) == 0x90 -- the operator new argument at "
              "mfc140 0x248392 inside XOleIPSite::GetWindowContext");

// OpenMFC's own three members must stay in the order this file assumes.
static_assert(offsetof(COleFrameHook, m_pActiveItem) ==
                  offsetof(COleFrameHook, m_pFrameWnd) + sizeof(void*),
              "afxole.h COleFrameHook member order changed");
static_assert(offsetof(COleFrameHook, m_lpActiveUIWindow) ==
                  offsetof(COleFrameHook, m_pActiveItem) + sizeof(void*),
              "afxole.h COleFrameHook member order changed");

// The slot OpenMFC calls m_lpActiveUIWindow is retail's m_lpActiveObject; see
// the header comment for why the reinterpret_cast is currently safe and why a
// headerRequest asks for the member to be retyped.
inline IOleInPlaceActiveObject* ActiveObject(const COleFrameHook* p) {
    return reinterpret_cast<IOleInPlaceActiveObject*>(p->m_lpActiveUIWindow);
}

// Retail guards almost every body with
//     m_lpActiveObject != NULL && m_pActiveItem->m_pInPlaceFrame == this
// (e.g. mfc140u 0x248040..0x248052 in OnContextHelp).  The second half
// distinguishes the container frame's hook (retail COleClientItem+0xa8) from
// the doc frame's hook (+0xb0); OpenMFC's COleClientItem declares neither
// member -- see the layout note at core/ole/COleClientItem.cpp:1497 -- so the
// test degrades here to "the hook has an active item at all".  That is weaker
// than retail: a doc-frame hook (retail item+0xb0) would pass it too.  It is
// harmless today only because nothing in OpenMFC constructs a COleFrameHook
// (the exported ctor thunk below is the only way in) and nothing ever stores
// m_lpActiveUIWindow, so every guarded body returns at the first test.  The
// deviation is called out again at each call site.
inline bool ActiveForThisFrame(const COleFrameHook* p) {
    return p != nullptr && ActiveObject(p) != nullptr && p->m_pActiveItem != nullptr;
}

}  // namespace

// Symbol: ??0COleFrameHook@@QEAA@PEAVCFrameWnd@@PEAVCOleClientItem@@@Z
// COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
// An MSVC constructor export takes `this` in RCX and returns it; the caller
// has already allocated the storage (retail's own caller does exactly that:
// `mov $0x90,%ecx ; call operator new ; ... ; call 0x1802461f0` at mfc140
// 0x248392..0x2483b1 inside COleClientItem::XOleIPSite::GetWindowContext).
// This thunk used to be declared (CFrameWnd*, COleClientItem*) and to heap-
// allocate with a plain `new`, which read the caller's `this` as pFrameWnd,
// pFrameWnd as pItem, dropped pItem entirely and leaked a second object while
// leaving the caller's storage unconstructed.  Constructing in the caller's
// storage is how the other ctor thunks in this tree work too (placement new at
// e.g. core/ole/Thunks.cpp:1148; core/gdi/CRectTracker.cpp:230 initialises
// pThis by hand instead).
extern "C" void* MS_ABI impl___0COleFrameHook__QEAA_PEAVCFrameWnd__PEAVCOleClientItem___Z(
    void* pThis, CFrameWnd* pFrameWnd, COleClientItem* pItem) {
    if (pThis == nullptr)
        return nullptr;
    return new (pThis) COleFrameHook(pFrameWnd, pItem);
}
// Retail ctor, mfc140u 0x247d90: CCmdTarget::CCmdTarget, plant the three
// vftables, then
//     m_lpActiveObject = NULL; m_pActiveItem = pItem; m_pFrameWnd = pFrameWnd;
//     m_hWndFrame = pFrameWnd->m_hWnd; [+0x60] = [+0x68] = NULL;
//     m_nModelessCount = 0; m_strObjName = <empty>;
//     pFrameWnd->[+0x120] = this;              // 0x247e14
// The last statement installs the hook on the frame window (MFC's
// CFrameWnd::m_pNotifyHook).  OpenMFC's CFrameWnd declares no such member, and
// the five members between +0x58 and +0x78 are likewise undeclared, so only
// the three OpenMFC has are set here.  The padding memset keeps the undeclared
// tail deterministic.
COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
    : m_pFrameWnd(pFrameWnd), m_pActiveItem(pItem), m_lpActiveUIWindow(nullptr) {
    memset(_oleframehook_padding, 0, sizeof(_oleframehook_padding));
}
// Retail dtor, mfc140u 0x247e80: restore the vftable, clear the frame's
// back-pointer (`if (m_pFrameWnd != NULL && m_pFrameWnd->[+0x120] == this)
// m_pFrameWnd->[+0x120] = NULL;`, 0x247e93..0x247ea5), release
// m_strObjName's CStringData, then tail-jump to ~CCmdTarget.  It does not
// clear m_pFrameWnd / m_pActiveItem / m_lpActiveObject.  Neither the frame
// back-pointer nor the CString is modelled in OpenMFC, so nothing is left to
// do here; the three assignments this body used to make were not retail's.
COleFrameHook::~COleFrameHook() {
}
// Retail OnDocActivate, mfc140u 0x247f70 (vftable slot 25).  Its first act is
//     if (m_lpActiveObject == NULL) return TRUE;
// (mfc140u 0x247f88 / mfc140 0x2463e8: `lea 0x1(%rcx),%eax` with rcx already
// known zero).  Everything after that is unreachable here, and this comment
// used to skip its first step, so for the record it is:
//     m_lpActiveObject->OnDocWindowActivate(bActivate);      // vtbl 0x38, slot 7
//     COleFrameHook* pHook = m_pActiveItem->[+0xa8];         // 0x247fa1
//     pHook->m_pFrameWnd->[+0x1d0] |= 2;                     // 0x247fac
//     if (bActivate) {
//         pHook->m_pFrameWnd->[+0x120] = pHook;              // 0x248021
//     } else {
//         IOleInPlaceFrame* f = &pHook->m_xOleInPlaceFrame;  // pHook + 0x80
//         f->SetBorderSpace(NULL);                           // vtbl 0x38, slot 7
//         if (m_pActiveItem->[+0xb0] != NULL)                // the doc-frame hook
//             ((IOleInPlaceFrame*)(m_pActiveItem->[+0xb0] + 0x80))->SetBorderSpace(NULL);
//         f->SetMenu(NULL, NULL, NULL);                      // vtbl 0x50, slot 10
//         if (pHook != this) pHook->m_pFrameWnd->[+0x120] = NULL;   // 0x248014
//     }
//     return bActivate;                                      // NOT TRUE  (0x24802c)
// which needs COleClientItem+0xa8 / +0xb0 and the hook's own XOleInPlaceFrame
// sub-object, none of which OpenMFC declares.  m_lpActiveUIWindow is always
// NULL in this build (the ctor sets it and nothing in the DLL ever stores
// there), so the retail early-out is the only reachable path and TRUE is the
// faithful answer.  The exported thunk for this symbol lives in
// core/ole/Thunks.cpp, not here.
BOOL COleFrameHook::OnDocActivate(BOOL bActivate) {
    (void)bActivate;
    return TRUE;
}
// Retail's override is `BOOL OnContextHelp(BOOL bEnter)`
// (?OnContextHelp@COleFrameHook@@UEAAHH@Z, vftable slot 26).  OpenMFC's
// afxole.h declares it with no parameter, so this typed overload cannot be
// given retail's body -- there is no bEnter to forward.  The real behaviour is
// implemented in the exported thunk below; a headerRequest asks for the
// declaration to take BOOL bEnter.
BOOL COleFrameHook::OnContextHelp() {
    return FALSE;
}
// mfc140u exports no ?OnResizeBorder@COleFrameHook@@ and no
// ?OnGetClipboardData@COleFrameHook@@ (checked against
// mfc_complete_ordinal_mapping.json, which lists 33 COleFrameHook symbols, and
// against both RVA maps), and neither appears in the class vftable at
// mfc140u .rdata 0x32c0f0, whose own slots are exactly the twelve listed in
// the header comment.  These two virtuals are OpenMFC additions with no retail
// counterpart to transcribe, so they keep their existing FALSE bodies.
BOOL COleFrameHook::OnResizeBorder(LPCRECT lpRect, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bMainWindow) {
    (void)lpRect; (void)lpUIWindow; (void)bMainWindow;
    return FALSE;
}
BOOL COleFrameHook::OnGetClipboardData(COleDataSource* pDataSource) {
    (void)pDataSource;
    return FALSE;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?OnContextHelp@COleFrameHook@@UEAAHH@Z
// COleFrameHook::OnContextHelp(BOOL bEnter) -- vftable slot 26,
// mfc140u 0x248040 / mfc140 0x2464a0, transcribed whole:
//     if (m_lpActiveObject == NULL)              goto ret_true;   // 0x248040
//     if (m_pActiveItem->m_pInPlaceFrame != this) goto ret_true;  // 0x248047
//     return NotifyAllInPlace(bEnter,
//                             &COleFrameHook::DoContextSensitiveHelp); // jmp 0x2483e0
//   ret_true: return TRUE;                                            // 0x248060
// The r8 the tail-jump passes is 0x180248560, which IS
// COleFrameHook::DoContextSensitiveHelp (its own entry, per the mfc140u map).
// Deviation: the m_pInPlaceFrame test becomes a null test on m_pActiveItem --
// see ActiveForThisFrame().
extern "C" int MS_ABI impl__OnContextHelp_COleFrameHook__UEAAHH_Z(COleFrameHook* pThis,
                                                                  int bEnter) {
    if (!ActiveForThisFrame(pThis))
        return TRUE;
    return impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
        pThis, bEnter, &impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z);
}

// Symbol: ?DoContextSensitiveHelp@COleFrameHook@@QEAAHH@Z
// mfc140u 0x248560 / mfc140 0x2469c0, the whole body:
//     mov 0x48(%rcx),%rcx              ; m_lpActiveObject
//     mov (%rcx),%rax ; mov 0x20(%rax),%rax ; call *...
//     not %eax ; shr $0x1f,%eax        ; return SUCCEEDED(hr)
// vtable byte 0x20 is slot 4 of IOleInPlaceActiveObject, i.e.
// IOleWindow::ContextSensitiveHelp, and rdx (bEnter) is passed through
// untouched.  Deviation: retail dereferences m_lpActiveObject without a null
// test because its only callers (OnContextHelp -> NotifyAllInPlace) have
// already established it; the guard below is unreachable on those paths and
// answers TRUE, the value that lets NotifyAllInPlace's walk continue.
extern "C" int MS_ABI impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z(
    COleFrameHook* pThis, int bEnter) {
    IOleInPlaceActiveObject* pActiveObject = pThis ? ActiveObject(pThis) : nullptr;
    if (pActiveObject == nullptr)
        return TRUE;
    return SUCCEEDED(pActiveObject->ContextSensitiveHelp(bEnter)) ? TRUE : FALSE;
}

// Symbol: ?DoEnableModeless@COleFrameHook@@QEAAHH@Z
// STUB.  mfc140u 0x248580 / mfc140 0x2469e0 is a reference-counted enable:
//     UINT n = m_nModelessCount;                       // [this+0x70]
//     if (!bEnable) { m_nModelessCount = n + 1;
//                     if (n == 0) m_lpActiveObject->EnableModeless(FALSE); }
//     else          { if (n == 0) return TRUE;
//                     m_nModelessCount = n - 1;
//                     if (n - 1 == 0) m_lpActiveObject->EnableModeless(TRUE); }
//     return TRUE;                                     // every path
// (vtable byte 0x48 is slot 9, IOleInPlaceActiveObject::EnableModeless.)
// The counter is retail's member at +0x70, which OpenMFC's COleFrameHook does
// not declare, and without it the nesting cannot be reproduced: calling
// EnableModeless on every request instead of on the 0<->1 transition would
// re-enable the UI underneath a still-open nested modal dialog.  TRUE is
// retail's return value on every path, so only the side effect is missing.
// See headerRequests for the member.
extern "C" int MS_ABI impl__DoEnableModeless_COleFrameHook__QEAAHH_Z(COleFrameHook* pThis,
                                                                     int bEnable) {
    (void)pThis;
    (void)bEnable;
    return TRUE;
}

// Symbol: ?NotifyAllInPlace@COleFrameHook@@QEAAHHP81@EAAHH@Z@Z
// STUB.  mfc140u 0x2483e0 / mfc140 0x246840 broadcasts a hook callback to
// every in-place item hosted inside this frame:
// What the disassembly literally establishes is the offsets, the vtable bytes
// and the three callees the map does name; the MFC class names attached to the
// enumeration steps below come from MFC's documented API shape.
//     HWND hwndFrame = m_hWndFrame;                       // [this+0x58]
//     void* pApp     = AfxGetModuleState()->[+0x08];      // the module's app object
//     void* pMgr     = pApp->[+0xb8];                     // its document manager
//     if (pMgr == NULL) return TRUE;
//     for (POSITION p = pMgr->vf[0x30](); p != NULL; )              // slot 6
//     { void* pT = pMgr->vf[0x38](&p);             // slot 7 -- a doc template
//       for (POSITION q = pT->vf[0xb8](); q != NULL; )              // slot 23
//       { CObject* pDoc = pT->vf[0xc0](&q);                         // slot 24
//         if (!pDoc->IsKindOf(RUNTIME_CLASS(COleDocument))) continue;
//         for (POSITION r = pDoc->vf[0x238]();;)                    // slot 71
//         { COleClientItem* pItem =            // call mfc140u 0x180254120, which the
//               pDoc->GetNextItemOfKind(       // mfc140u map DOES name:
//                   r, RUNTIME_CLASS(COleClientItem));  // ?GetNextItemOfKind@
//                                              //   COleDocument@@IEBAPEAVCDocItem@@
//                                              //   AEAPEAU__POSITION@@PEAUCRuntimeClass@@@Z
//           if (pItem == NULL) break;
//           COleFrameHook* pHook = pItem->[+0xa8];
//           if (pHook == NULL || pHook->m_lpActiveObject == NULL) continue;
//           CView* pView = pItem->[+0x98];
//           if (pView == NULL) continue;
//           if (!AfxIsDescendant(hwndFrame, pView->m_hWnd)) continue;
//           if (!(*pfn)(pHook, bParam)) return FALSE;   // 0x2484f5
//         } } }
//     return TRUE;
// (The two RUNTIME_CLASS operands are mfc140u 0x18032ea80 = COleDocument
// (m_nObjectSize 0x1e8) and 0x18032e788 = COleClientItem (0xf0), read straight
// out of the descriptors; the callees at 0x180133930, 0x180234cf0, 0x18029d3a0
// resolve to AfxGetModuleState, CObject::IsKindOf and AfxIsDescendant, and the
// `call *0x1802c7b30` used for every indirect call is
// __guard_dispatch_icall_fptr, not an import.)
// The inner test needs COleClientItem+0xa8 (the item's own frame hook) and
// +0x98 (its view), and the outer walk needs m_hWndFrame at +0x58; OpenMFC
// declares none of the three -- see core/ole/COleClientItem.cpp:1497 for the
// same finding about the item.  With no enumerable in-place item the loop body
// never runs, and TRUE is exactly what retail returns in that case, which is
// what makes OnContextHelp and OnEnableModeless above correct compositions
// today.  The callback is still invoked through the real signature so that
// filling this in later needs no signature change.
extern "C" int MS_ABI impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
    COleFrameHook* pThis, int bParam, PFN_HOOKNOTIFY pfn) {
    (void)pThis;
    (void)bParam;
    (void)pfn;
    return TRUE;
}

// Symbol: ?OnActivate@COleFrameHook@@UEAAXH@Z
// COleFrameHook::OnActivate(BOOL bActive) -- vftable slot 24,
// mfc140u 0x2482b0 / mfc140 0x246710, transcribed whole:
//     if (m_lpActiveObject == NULL) return;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return;
//     if (!m_pFrameWnd->IsWindowEnabled()) return;          // call 0x1802a9b00
//     m_lpActiveObject->OnFrameWindowActivate(bActive);     // vtable 0x30 = slot 6
// 0x1802a9b00 is ?IsWindowEnabled@CWnd@@QEBAHXZ per the mfc140u map.
// Deviation: the m_pInPlaceFrame test becomes a null test (ActiveForThisFrame).
extern "C" void MS_ABI impl__OnActivate_COleFrameHook__UEAAXH_Z(COleFrameHook* pThis,
                                                                int bActive) {
    if (!ActiveForThisFrame(pThis))
        return;
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(pThis->m_pFrameWnd))
        return;
    ActiveObject(pThis)->OnFrameWindowActivate(bActive);
}

// Symbol: ?OnEnableModeless@COleFrameHook@@UEAAXH@Z
// COleFrameHook::OnEnableModeless(BOOL bEnable) -- vftable slot 27,
// mfc140u 0x248300 / mfc140 0x246760, transcribed whole:
//     if (m_lpActiveObject == NULL) return;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return;
//     NotifyAllInPlace(bEnable, &COleFrameHook::DoEnableModeless);
//                                  // call at mfc140u 0x24831f (mfc140 0x24677f)
// The r8 the call passes is mfc140u 0x180248580 (mfc140 0x1802469e0), which is
// DoEnableModeless's own entry in each image's map.  The
// BOOL NotifyAllInPlace returns is discarded (this override returns void).
// Deviation: the m_pInPlaceFrame test becomes a null test; and the callback
// itself is the documented stub above, so the walk currently has no effect.
extern "C" void MS_ABI impl__OnEnableModeless_COleFrameHook__UEAAXH_Z(COleFrameHook* pThis,
                                                                      int bEnable) {
    if (!ActiveForThisFrame(pThis))
        return;
    impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(
        pThis, bEnable, &impl__DoEnableModeless_COleFrameHook__QEAAHH_Z);
}

// Symbol: ?OnInitMenu@COleFrameHook@@UEAAXPEAVCMenu@@@Z
// STUB.  vftable slot 32, mfc140u 0x248120 / mfc140 0x246580, whole body:
//     COleClientItem* pItem = m_pActiveItem;
//     if (pItem != NULL && pItem->IsKindOf(RUNTIME_CLASS(COleDocObjectItem)))
//         pItem->[+0x108] = 0;
// (the RUNTIME_CLASS operand is mfc140u 0x18032eab0 / mfc140 0x18032c8f0 --
// two addresses for the SAME descriptor in the two images, do not "correct"
// either against the other -- whose m_lpszClassName reads "COleDocObjectItem"
// and m_nObjectSize 0x118; pMenu is never touched.)  The single thing the
// function does is clear a flag at item+0x108 -- the doc-object "menu message
// forwarding armed" bit that OnMenuSelect sets and OnInitMenuPopup /
// OnPreTranslateMessage read.  +0x108 lies past sizeof(COleClientItem) (the
// COleClientItem descriptor, mfc140u 0x18032e788, gives m_nObjectSize 0xf0),
// so it is a COleDocObjectItem member, which is why every access to it is
// behind the IsKindOf test.  OpenMFC's COleDocObjectItem declares no such
// member, so there is nothing to clear.  See headerRequests.
extern "C" void MS_ABI impl__OnInitMenu_COleFrameHook__UEAAXPEAVCMenu___Z(COleFrameHook* pThis,
                                                                          CMenu* pMenu) {
    (void)pThis;
    (void)pMenu;
}

// Symbol: ?OnInitMenuPopup@COleFrameHook@@UEAAHPEAVCMenu@@HH@Z
// STUB.  vftable slot 31, mfc140u 0x248160 / mfc140 0x2465c0, whole body:
//     if (bSysMenu) return FALSE;
//     COleClientItem* pItem = m_pActiveItem;
//     if (pItem == NULL) return FALSE;
//     if (!pItem->IsKindOf(RUNTIME_CLASS(COleDocObjectItem))) return FALSE;
//     if (pItem->[+0x108] == 0) return FALSE;
//     CWnd* pWnd = pItem->GetInPlaceWindow();      // call mfc140u 0x1802492a0
//     if (pWnd == NULL) return FALSE;
//     ::SendMessage(pWnd->m_hWnd, WM_INITMENUPOPUP,
//                   (WPARAM)pMenu->m_hMenu, (LPARAM)LOWORD(nIndex));
//     return TRUE;
// (?GetInPlaceWindow@COleClientItem@@QEAAPEAVCWnd@@XZ is mfc140u 0x2492a0 /
// mfc140 0x247700; the SendMessage import slot is mfc140u 0x1802c7120 =
// USER32!SendMessageW, mfc140 0x1802c5378 = SendMessageA.)  Gated entirely on
// the doc-object flag at COleDocObjectItem+0x108, which OpenMFC does not
// declare, and whose only retail setter (OnMenuSelect) is itself a stub here,
// so with the flag permanently absent every retail path returns FALSE anyway.
extern "C" int MS_ABI impl__OnInitMenuPopup_COleFrameHook__UEAAHPEAVCMenu__HH_Z(
    COleFrameHook* pThis, CMenu* pMenu, int nIndex, int bSysMenu) {
    (void)pThis;
    (void)pMenu;
    (void)nIndex;
    (void)bSysMenu;
    return FALSE;
}

// Symbol: ?OnMenuSelect@COleFrameHook@@UEAAHIIPEAUHMENU__@@@Z
// STUB.  vftable slot 33, mfc140u 0x248070 / mfc140 0x2464d0, whole body:
//     COleClientItem* pItem = m_pActiveItem;
//     if (pItem == NULL) return FALSE;
//     if (!pItem->IsKindOf(RUNTIME_CLASS(COleDocObjectItem))) return FALSE;
//     CWnd* pWnd = pItem->GetInPlaceWindow();      // call mfc140u 0x1802492a0
//     if (nFlags & MF_POPUP) {
//         HMENU h = pItem->[+0xf0] ? *(HMENU*)(pItem->[+0xf0] + 8) : NULL;
//         if (h != hSysMenu) return FALSE;
//         pItem->[+0x108] = (nItemID != 0);
//         if (nItemID == 0) return FALSE;
//     } else if (pItem->[+0x108] == 0) return FALSE;
//     if (pWnd == NULL) return FALSE;
//     ::SendMessage(pWnd->m_hWnd, WM_MENUSELECT,
//                   MAKEWPARAM(LOWORD(nItemID), LOWORD(nFlags)), (LPARAM)hSysMenu);
//     return TRUE;
// This is the function that ARMS the +0x108 flag the other three read.  It
// needs item+0x108 and item+0xf0 (a CMenu* whose +8 is its HMENU); both lie at
// or past sizeof(COleClientItem) == 0xf0, i.e. they are COleDocObjectItem
// members (sizeof 0x118), read only after the IsKindOf test.  OpenMFC's
// COleDocObjectItem declares neither.  See headerRequests.
extern "C" int MS_ABI impl__OnMenuSelect_COleFrameHook__UEAAHIIPEAUHMENU_____Z(
    COleFrameHook* pThis, unsigned int nItemID, unsigned int nFlags, HMENU hSysMenu) {
    (void)pThis;
    (void)nItemID;
    (void)nFlags;
    (void)hSysMenu;
    return FALSE;
}

// Symbol: ?OnPaletteChanged@COleFrameHook@@UEAAXPEAVCWnd@@@Z
// COleFrameHook::OnPaletteChanged(CWnd* pFocusWnd) -- vftable slot 29,
// mfc140u 0x248360 / mfc140 0x2467c0, transcribed whole:
//     CWnd* pWnd = m_pActiveItem->GetInPlaceWindow();  // call mfc140u 0x1802492a0
//     if (pWnd == NULL) return;
//     ::SendMessage(pWnd->m_hWnd, WM_PALETTECHANGED /* 0x311 */,
//                   (WPARAM)(pFocusWnd ? pFocusWnd->m_hWnd : NULL), 0);
// The import slot is mfc140u 0x1802c7120 = USER32!SendMessageW (the same call
// site reads mfc140 0x1802c5378 = SendMessageA in the ANSI twin), so this is
// ::SendMessage with UNICODE picking SendMessageW.
// Deviation: retail loads m_pActiveItem and calls straight through it with no
// null test; the guard below returns instead of faulting.
// COleClientItem::GetInPlaceWindow is a documented stub in this tree today
// (core/ole/COleClientItem.cpp:654 always returns NULL), so this body is a
// no-op until that lands -- but the composition is retail's.
extern "C" void MS_ABI impl__OnPaletteChanged_COleFrameHook__UEAAXPEAVCWnd___Z(
    COleFrameHook* pThis, CWnd* pFocusWnd) {
    if (!pThis || pThis->m_pActiveItem == nullptr)
        return;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(pThis->m_pActiveItem);
    if (pWnd == nullptr)
        return;
    ::SendMessage(pWnd->m_hWnd, WM_PALETTECHANGED,
                  reinterpret_cast<WPARAM>(pFocusWnd ? pFocusWnd->m_hWnd : nullptr), 0);
}

// Symbol: ?OnPreTranslateMessage@COleFrameHook@@UEAAHPEAUtagMSG@@@Z
// COleFrameHook::OnPreTranslateMessage(MSG* pMsg) -- vftable slot 23,
// mfc140u 0x2481e0 / mfc140 0x246640.  Retail, whole:
//     if (m_lpActiveObject == NULL) return FALSE;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return FALSE;
//     UINT msg = pMsg->message;
//     if ((UINT)(msg - 0x100) <= 9)                      // WM_KEYFIRST .. 0x109
//         return m_lpActiveObject->TranslateAccelerator(pMsg) == S_OK; // vtbl 0x28 = slot 5
//     if (msg != WM_COMMAND) return FALSE;              // cmp $0x111 -- WM_COMMAND,
//                                                       // not WM_SYSCOMMAND (0x112)
//     if (!m_pActiveItem->IsKindOf(RUNTIME_CLASS(COleDocObjectItem))) return FALSE;
//     LRESULT lr = 0;
//     if (m_pActiveItem->[+0x108] != 0) {
//         CWnd* pWnd = m_pActiveItem->GetInPlaceWindow();
//         if (pWnd != NULL)
//             lr = ::SendNotifyMessage(pWnd->m_hWnd, WM_COMMAND,
//                                      pMsg->wParam, pMsg->lParam);
//     }
//     return lr != 0;
// (the import slot is mfc140u 0x1802c6fb8 = USER32!SendNotifyMessageW /
// mfc140 0x1802c5048 = SendNotifyMessageA -- SendNotify, not Send; the
// `test %eax,%eax ; sete %cl` after the TranslateAccelerator call tests
// hr == 0, i.e. S_OK exactly, not SUCCEEDED.  GetInPlaceWindow here is
// mfc140u 0x1802492a0, and the RUNTIME_CLASS operand mfc140u 0x18032eab0 =
// COleDocObjectItem.)
// DEVIATION, and it is a real one: the WM_COMMAND arm is NOT implemented,
// because it is gated on the doc-object flag at COleDocObjectItem+0x108 that
// OpenMFC does not declare (same gap as OnMenuSelect above).  With that flag
// unrepresentable -- and its only retail setter, OnMenuSelect, a stub -- the
// arm can only ever produce retail's `lr == 0` answer, which is the FALSE
// this body returns; but a future COleDocObjectItem that models the flag
// must restore the arm, not just flip a condition.  Also a deviation: the
// null test on pMsg below, which retail does not make.  Plus the
// ActiveForThisFrame() weakening of the m_pInPlaceFrame test.
extern "C" int MS_ABI impl__OnPreTranslateMessage_COleFrameHook__UEAAHPEAUtagMSG___Z(
    COleFrameHook* pThis, MSG* pMsg) {
    if (!ActiveForThisFrame(pThis) || pMsg == nullptr)
        return FALSE;
    if (static_cast<unsigned int>(pMsg->message - WM_KEYFIRST) <= 9u)
        return ActiveObject(pThis)->TranslateAccelerator(pMsg) == S_OK ? TRUE : FALSE;
    return FALSE;
}

// Symbol: ?OnQueryNewPalette@COleFrameHook@@UEAAHXZ
// COleFrameHook::OnQueryNewPalette() -- vftable slot 30,
// mfc140u 0x2483a0 / mfc140 0x246800, transcribed whole:
//     CWnd* pWnd = m_pActiveItem->GetInPlaceWindow();  // call mfc140u 0x1802492a0
//     if (pWnd == NULL) return FALSE;          // falls out with eax == 0
//     return ::SendMessage(pWnd->m_hWnd, WM_QUERYNEWPALETTE /* 0x30f */, 0, 0) != 0;
//                                              // slot mfc140u 0x1802c7120
// Deviation: the same missing null test on m_pActiveItem as OnPaletteChanged.
// GetInPlaceWindow is a stub in this tree today, so this returns FALSE until
// that lands.
extern "C" int MS_ABI impl__OnQueryNewPalette_COleFrameHook__UEAAHXZ(COleFrameHook* pThis) {
    if (!pThis || pThis->m_pActiveItem == nullptr)
        return FALSE;
    CWnd* pWnd = impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(pThis->m_pActiveItem);
    if (pWnd == nullptr)
        return FALSE;
    return ::SendMessage(pWnd->m_hWnd, WM_QUERYNEWPALETTE, 0, 0) != 0 ? TRUE : FALSE;
}

// Symbol: ?OnRecalcLayout@COleFrameHook@@UEAAXXZ
// STUB.  vftable slot 22, mfc140u 0x247ef0 / mfc140 0x246350, whole body:
//     if (m_lpActiveObject == NULL) return;
//     RECT rc;
//     m_pFrameWnd->NegotiateBorderSpace(1 /* borderGet */, &rc);   // vtbl 0x348
//     m_lpActiveObject->ResizeBorder(&rc, &m_xOleInPlaceFrame,     // vtbl 0x40 = slot 8
//                                    m_pActiveItem->m_pInPlaceFrame == this);
// The second argument is the address of the hook's OWN XOleInPlaceFrame
// sub-object (this+0x80).  OpenMFC's COleFrameHook has no such sub-object and
// its interface map (detail/InterfaceMapsSupport.cpp:38-39) exposes no
// IOleInPlaceUIWindow at all, so there is no pointer to pass; passing NULL
// instead is not an option because ResizeBorder's pUIWindow is the object the
// server calls back on.  The NegotiateBorderSpace(borderGet) call alone has no
// observable effect (it only fills the local RECT), so nothing partial is
// worth emitting.  See headerRequests.
extern "C" void MS_ABI impl__OnRecalcLayout_COleFrameHook__UEAAXXZ(COleFrameHook* pThis) {
    (void)pThis;
}

// Symbol: ?OnUpdateFrameTitle@COleFrameHook@@UEAAHXZ
// COleFrameHook::OnUpdateFrameTitle() -- vftable slot 28,
// mfc140u 0x248330 / mfc140 0x246790, transcribed whole:
//     if (m_lpActiveObject == NULL) return FALSE;
//     if (m_pActiveItem->m_pInPlaceFrame != this) return FALSE;
//     return m_pActiveItem->vf[0x170]();       // tail jmp through the vtable
// COleClientItem vftable byte 0x170 is OnUpdateFrameTitle; that slot is
// identified, and its retail base body (`xor %eax,%eax ; ret`) transcribed, at
// core/ole/COleClientItem.cpp:2155.
// Deviations: the m_pInPlaceFrame test becomes a null test
// (ActiveForThisFrame), and the virtual dispatch is devirtualized -- OpenMFC's
// COleClientItem does not declare OnUpdateFrameTitle virtual (it is not in the
// class declaration in afxole.h at all), so the base thunk is called directly
// and a COleDocObjectItem override would not be reached.
extern "C" int MS_ABI impl__OnUpdateFrameTitle_COleFrameHook__UEAAHXZ(COleFrameHook* pThis) {
    if (!ActiveForThisFrame(pThis))
        return FALSE;
    return impl__OnUpdateFrameTitle_COleClientItem__UEAAHXZ(pThis->m_pActiveItem);
}

// ---------------------------------------------------------------------------
// COleFrameHook::XOleInPlaceFrame (IOleInPlaceFrame) and
// COleFrameHook::XOleCommandTarget (IOleCommandTarget).
//
// All fourteen below are STUBS, for one shared reason stated once here rather
// than fourteen times: each retail body begins by backing the incoming
// interface pointer up to the containing COleFrameHook -- `lea -0x80(%rcx)`
// for XOleInPlaceFrame (mfc140 0x24793f) and -0x88 for XOleCommandTarget
// (mfc140 0x2477db reads m_pActiveItem as `mov -0x38(%rcx),%rdi`, and
// 0x88 - 0x38 == 0x50) -- and OpenMFC's COleFrameHook contains neither
// sub-object.  Its constructor plants no interface vftables, and the interface
// map at detail/InterfaceMapsSupport.cpp:38-39 forwards to CCmdTarget with no
// entries of its own, so nothing in this build can ever produce a pointer for
// which that back-up is meaningful.  A body doing the arithmetic anyway would
// synthesise a `this` pointing 0x80 bytes before a live object.
//
// What IS fixed here is the parameter lists: the auto-generated ones omitted
// the interface `this` entirely, which would have shifted every argument by
// one register had any of these ever been reached.
//
// Retail behaviour, so that filling these in later needs no second pass
// (mfc140 RVAs; H = the recovered COleFrameHook, all vtable slots are
// IOleInPlaceFrame's unless noted):
//   0x247910 GetWindow            *phwnd = H->m_hWndFrame (read as -0x28(%rcx));
//                                 return *phwnd ? S_OK : E_FAIL; E_POINTER if !phwnd
//   0x247930 ContextSensitiveHelp pFrame = H->m_pFrameWnd->GetTopLevelFrame()
//                                 (AfxThrowInvalidArgException if NULL).
//                                 !fEnterMode: pFrame->ExitHelpMode() (vtbl 0x378);
//                                 S_OK.
//                                 fEnterMode: S_OK if pFrame->[+0x104] already
//                                 set; else E_UNEXPECTED unless
//                                 pFrame->CanEnterHelpMode() and
//                                 H->OnContextHelp(TRUE) (H's vtbl 0xd0), then
//                                 ::PostMessage(pFrame->m_hWnd, WM_COMMAND (0x111),
//                                 ID_CONTEXT_HELP (0xe145), 0) -- WM_COMMAND with
//                                 a command ID, not WM_SYSCOMMAND/SC_CONTEXTHELP;
//                                 the import slot mfc140 0x1802c52d8 resolves
//                                 to PostMessageA (PostMessageW in mfc140u)
//   0x247a20 GetBorder            H->m_pActiveItem->vf[0x178](m_pFrameWnd, FALSE);
//                                 then three H->m_pFrameWnd->NegotiateBorderSpace
//                                 calls (vtbl 0x348): (3, NULL), (1, lprectBorder),
//                                 (3, &<frame+0x110 saved beforehand>); then
//                                 vf[0x178](m_pFrameWnd, TRUE) again if the first
//                                 vf[0x178] call returned nonzero; S_OK
//   0x247b30 RequestBorderSpace   H->m_pFrameWnd->NegotiateBorderSpace(2 /*request*/);
//                                 returns S_OK or OLE_E_INVALIDRECT (0x800401a1)
//   0x247ba0 SetBorderSpace       NegotiateBorderSpace(3 /*set*/, pborderwidths);
//                                 if nonzero: clear bits 0xc of
//                                 m_pFrameWnd->[+0x1d0] and call
//                                 m_pFrameWnd->RecalcLayout(FALSE) (vtbl 0x300);
//                                 then always
//                                 H->m_pActiveItem->vf[0x178](m_pFrameWnd,
//                                 pborderwidths == NULL); S_OK
//   0x247c50 SetActiveObject      RELEASE(H->m_lpActiveObject); H->m_lpActiveObject =
//                                 pActiveObject (AddRef'd); H->m_strObjName.Empty();
//                                 then ONLY if pszObjName && pActiveObject:
//                                 H->m_strObjName = pszObjName and
//                                 H->m_pActiveItem->vf[0x170]() (OnUpdateFrameTitle)
//   0x247d00 InsertMenus          H->m_pActiveItem->vf[0x158] (OnInsertMenus),
//                                 with CMenu::FromHandle(hmenuShared)
//   0x247d80 SetMenu              H->m_pActiveItem->vf[0x160] (OnSetMenu)
//   0x247e20 RemoveMenus          H->m_pActiveItem->vf[0x168] (OnRemoveMenus)
//   0x247ea0 SetStatusText        ::SendMessage(H->m_pFrameWnd->m_hWnd,
//                                 0x362 /* AFX WM_SETMESSAGESTRING */, 0, (LPARAM)text)
//   0x247f40 EnableModeless       H->m_pFrameWnd->BeginModalState() (vtbl 0x218)
//                                 when !fEnable, EndModalState() (0x220) when fEnable
//   (CFrameWnd vtbl names above: read out of the mfc140 CFrameWnd vftable at
//   .rdata 0x337fc8, whose byte 0x348 holds NegotiateBorderSpace, 0x218
//   BeginModalState, 0x220 EndModalState, 0x228 PreTranslateMessage, 0x300
//   RecalcLayout and 0x378 ExitHelpMode.)
//   0x247fc0 TranslateAccelerator swaps H->m_hAccelTable into
//                                 H->m_pFrameWnd->[+0xf8], copies the 0x30-byte MSG to
//                                 the stack, calls m_pFrameWnd->vf[0x228]
//                                 (PreTranslateMessage), returns S_OK/S_FALSE
//   0x2477d0 Exec (XOleCommandTarget)        } both: if m_pActiveItem is a
//   0x247830 QueryStatus (XOleCommandTarget) } COleDocObjectItem, forward it to an
//                                 unexported helper (mfc140 0x258128 / 0x257e68)
//                                 and return its result, else return
//                                 OLECMDERR_E_UNKNOWNGROUP (0x80040104)
// ---------------------------------------------------------------------------

// Symbol: ?ContextSensitiveHelp@XOleInPlaceFrame@COleFrameHook@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceFrame_COleFrameHook__UEAAJH_Z(
    void* pThis, int fEnterMode) {
    (void)pThis;
    (void)fEnterMode;
    return E_NOTIMPL;
}

// Symbol: ?EnableModeless@XOleInPlaceFrame@COleFrameHook@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XOleInPlaceFrame_COleFrameHook__UEAAJH_Z(
    void* pThis, int fEnable) {
    (void)pThis;
    (void)fEnable;
    return E_NOTIMPL;
}

// Symbol: ?Exec@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
extern "C" long MS_ABI impl__Exec_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    void* pThis, const GUID* pguidCmdGroup, unsigned long nCmdID,
    unsigned long nCmdExecOpt, VARIANT* pvaIn, VARIANT* pvaOut) {
    (void)pThis;
    (void)pguidCmdGroup;
    (void)nCmdID;
    (void)nCmdExecOpt;
    (void)pvaIn;
    (void)pvaOut;
    return OLECMDERR_E_UNKNOWNGROUP;
}

// Symbol: ?GetBorder@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUtagRECT@@@Z
extern "C" long MS_ABI impl__GetBorder_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* lprectBorder) {
    (void)pThis;
    (void)lprectBorder;
    return E_NOTIMPL;
}

// Symbol: ?GetWindow@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceFrame_COleFrameHook__UEAAJPEAPEAUHWND_____Z(
    void* pThis, HWND* phwnd) {
    (void)pThis;
    // Retail's only unconditional step is the E_POINTER check at 0x247910,
    // which needs no `this`; everything after it reads m_hWndFrame.
    if (phwnd == nullptr)
        return E_POINTER;
    *phwnd = nullptr;
    return E_FAIL;
}

// Symbol: ?InsertMenus@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@PEAUtagOleMenuGroupWidths@@@Z
extern "C" long MS_ABI impl__InsertMenus_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU____PEAUtagOleMenuGroupWidths___Z(
    void* pThis, HMENU hmenuShared, OLEMENUGROUPWIDTHS* lpMenuWidths) {
    (void)pThis;
    (void)hmenuShared;
    (void)lpMenuWidths;
    return E_NOTIMPL;
}

// Symbol: ?QueryStatus@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KQEAU_tagOLECMD@@PEAU_tagOLECMDTEXT@@@Z
extern "C" long MS_ABI impl__QueryStatus_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KQEAU_tagOLECMD__PEAU_tagOLECMDTEXT___Z(
    void* pThis, const GUID* pguidCmdGroup, unsigned long cCmds, OLECMD* prgCmds,
    OLECMDTEXT* pCmdText) {
    (void)pThis;
    (void)pguidCmdGroup;
    (void)cCmds;
    (void)prgCmds;
    (void)pCmdText;
    return OLECMDERR_E_UNKNOWNGROUP;
}

// Symbol: ?RemoveMenus@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@@Z
extern "C" long MS_ABI impl__RemoveMenus_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU_____Z(
    void* pThis, HMENU hmenuShared) {
    (void)pThis;
    (void)hmenuShared;
    return E_NOTIMPL;
}

// Symbol: ?RequestBorderSpace@XOleInPlaceFrame@COleFrameHook@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__RequestBorderSpace_XOleInPlaceFrame_COleFrameHook__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* pborderwidths) {
    (void)pThis;
    (void)pborderwidths;
    return E_NOTIMPL;
}

// Symbol: ?SetActiveObject@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUIOleInPlaceActiveObject@@PEB_W@Z
extern "C" long MS_ABI impl__SetActiveObject_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUIOleInPlaceActiveObject__PEB_W_Z(
    void* pThis, IOleInPlaceActiveObject* pActiveObject, const wchar_t* pszObjName) {
    (void)pThis;
    (void)pActiveObject;
    (void)pszObjName;
    return E_NOTIMPL;
}

// Symbol: ?SetBorderSpace@XOleInPlaceFrame@COleFrameHook@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__SetBorderSpace_XOleInPlaceFrame_COleFrameHook__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* pborderwidths) {
    (void)pThis;
    (void)pborderwidths;
    return E_NOTIMPL;
}

// Symbol: ?SetMenu@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@PEAXPEAUHWND__@@@Z
extern "C" long MS_ABI impl__SetMenu_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU____PEAXPEAUHWND_____Z(
    void* pThis, HMENU hmenuShared, void* holemenu, HWND hwndActiveObject) {
    (void)pThis;
    (void)hmenuShared;
    (void)holemenu;
    (void)hwndActiveObject;
    return E_NOTIMPL;
}

// Symbol: ?SetStatusText@XOleInPlaceFrame@COleFrameHook@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__SetStatusText_XOleInPlaceFrame_COleFrameHook__UEAAJPEB_W_Z(
    void* pThis, const wchar_t* pszStatusText) {
    (void)pThis;
    (void)pszStatusText;
    return E_NOTIMPL;
}

// Symbol: ?TranslateAcceleratorW@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUtagMSG@@G@Z
// (mfc140u exports this as TranslateAcceleratorW; the ANSI twin's symbol, and
// therefore the body quoted above, is TranslateAcceleratorA at mfc140 0x247fc0.)
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUtagMSG__G_Z(
    void* pThis, MSG* lpmsg, unsigned short wID) {
    (void)pThis;
    (void)lpmsg;
    (void)wID;
    return S_FALSE;
}
