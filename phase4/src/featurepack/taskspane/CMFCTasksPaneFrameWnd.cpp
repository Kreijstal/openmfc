// CMFCTasksPaneFrameWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body below marked "decoded from retail" was transcribed from the
// retail export disassembly (disas.py against mfc140.dll, the ANSI twin of the
// mfc140u.dll OpenMFC reimplements).  Function bodies are byte-identical
// between the two images; only the RVAs differ.  Every RVA quoted is a
// FUNCTION ENTRY unless the text says otherwise.  The mfc140.dll RVA is the
// one quoted throughout, and every export's mfc140u entry is added beside it.
// Seven of those mfc140u entries (OnNcPaint, OnNeedTipText, OnPressButtons,
// OnTrackCaptionButtons, SetCaptionButtons, SetTaskPaneCaptionButtons,
// StopCaptionButtonsTracking) are NOT in mfc140u_rva_symbols.json; they were
// read straight from mfc140u.dll's export address table by ordinal (ordrva.py)
// and each disassembles to the same body as its mfc140.dll twin.
// EVERY address in this file not explicitly labelled "mfc140u" -- callee
// entries, .rdata vftables, .data runtime-class descriptors -- is an
// mfc140.dll address and must not be looked up in mfc140u.
//
// ---------------------------------------------------------------------------
// Retail CMFCTasksPaneFrameWnd instance layout
//
// The shipping header (atlmfc/include/afxtaskspaneframewnd.h, class at line
// 32) declares a single data member, `BOOL m_bMenuBtnPressed` (line 61), after
// the CPaneFrameWnd base.
// The retail constructor ??0CMFCTasksPaneFrameWnd@@QEAA@XZ (RVA 0x14a6d0
// mfc140.dll / 0x14c060 mfc140u) is:
//
//     CPaneFrameWnd::CPaneFrameWnd();                  // call 0xad6d0 (mfc140.dll)
//     m_bMenuBtnPressed /*+0x258*/ = FALSE;            // movl $0x0,0x258(%rbx)
//     vfptr = &CMFCTasksPaneFrameWnd::vftable;         // mov %rax,(%rbx)
//
// and ?CreateObject@CMFCTasksPaneFrameWnd@@SAPEAVCObject@@XZ (RVA 0x14a660
// mfc140.dll / 0x14bff0 mfc140u) allocates `mov $0x260,%ecx` bytes, which is
// also the m_nObjectSize customize/RuntimeClasses.cpp records (608).  So:
//   +0x000  CPaneFrameWnd sub-object (retail sizeof 0x258)
//   +0x258  BOOL m_bMenuBtnPressed
//   sizeof == 0x260
// Retail OnPressButtons writes the same +0x258 dword (TRUE around the
// OnPressOtherButton call, then FALSE); retail OnTrackCaptionButtons and
// StopCaptionButtonsTracking read it.  (This file's OnPressButtons never
// reaches the write -- see its note.)
//
// The CPaneFrameWnd sub-object is NOT retail-shaped in OpenMFC: the public
// class (include/openmfc/afxmfc.h) is a CMiniFrameWnd subclass with a trailing
// 128-byte `_paneframewnd_padding`, sizeof 632 == 0x278 under the phase4
// mingw flags (measured; the padding starts at +0x1f4 because mingw reuses
// CMiniFrameWnd's tail padding).  That is LARGER than this class's retail
// 0x260, so OpenMFC's CPaneFrameWnd constructor cannot be run over a
// CMFCTasksPaneFrameWnd allocation: its body (docking/CPaneFrameWnd.cpp)
// memsets the whole padding array, +0x1f4..+0x274, i.e. 0x14 bytes past the
// end of the block (see the constructor below).  The retail CPaneFrameWnd
// members this class's bodies reach -- the caption-button CObList
// m_lstCaptionButtons at +0x180 (head +0x188, count +0x198), the tooltip control pointer at +0x1b8, m_dwCaptionButtons at
// +0x12c -- have no counterpart in this object; docking/CPaneFrameWnd.cpp
// keeps m_dwCaptionButtons in a C++ side table (CPaneFrameWndSupport.h
// `rollupFlags`) and has no caption-button list at all.  Bodies that need
// those members follow that file's convention: a missing list is read as an
// EMPTY list and a missing tooltip control as a NULL one, and each body says
// exactly which part of the retail code that removes.
//
// Retail CMFCTasksPaneFrameWnd vftable (mfc140.dll .rdata 0x312bb8, the
// address the constructor stores at +0), slots the bodies below touch:
//   0x328 SetCaptionButtons (this class)   0x360 CPaneFrameWnd::GetPane
//   0x388 CalcBorderSize (this class)      0x438 OnDrawBorder (this class)
//   0x440 OnDrawCaptionButtons (this)      0x460 OnPressButtons (this class)
//   0x470 AddButton (this class)           0x478 OnTrackCaptionButtons (this)
//   0x480 StopCaptionButtonsTracking (this class)
// Retail CMFCTasksPane vftable (mfc140.dll .rdata 0x311408, stored by
// ??0CMFCTasksPane@@QEAA@XZ): 0x750 OnPressBackButton, 0x758
// OnPressForwardButton, 0x768 OnPressOtherButton.
// Virtual calls are made statically to the exported thunk (this DLL has no
// retail-shaped vtable for these objects); each body says where.
// An indirect `call *0x1802c5bd0` in the mfc140.dll listings is the CFG
// dispatch (__guard_dispatch_icall_fptr), not an import.
// ---------------------------------------------------------------------------

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <type_traits>

// ---------------------------------------------------------------------------
// Sibling exports.  Each was located with grep before being declared here; the
// declaration follows the mangled name.
// ---------------------------------------------------------------------------
// docking/CPaneFrameWnd.cpp
extern "C" void  MS_ABI impl__OnDrawBorder_CPaneFrameWnd__MEAAXPEAVCDC___Z(void* pThis, void* pDC);
extern "C" void  MS_ABI impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(void* pThis, CPoint point);
extern "C" void  MS_ABI impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnPressButtons_CPaneFrameWnd__MEAAXI_Z(void* pThis, unsigned int uiID);
extern "C" void  MS_ABI impl__AddButton_CPaneFrameWnd__MEAAXI_Z(void* pThis, unsigned int uiID);
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);
// taskspane/CMFCTasksPane.cpp
extern "C" void  MS_ABI impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(CMFCTasksPane* pThis);
extern "C" void  MS_ABI impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(CMFCTasksPane* pThis);
// taskspane/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ();
// core/runtime/CObject.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
// detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

// Layout of the one member this class adds (see the header note above).
struct S_CMFCTasksPaneFrameWnd {
    unsigned char _base[0x258];   // 0x000  CPaneFrameWnd (retail sizeof 0x258)
    BOOL          m_bMenuBtnPressed; // 0x258
    unsigned char _tail[4];       // 0x25c  alignment padding to sizeof 0x260
};
static_assert(offsetof(S_CMFCTasksPaneFrameWnd, m_bMenuBtnPressed) == 0x258,
              "CMFCTasksPaneFrameWnd::m_bMenuBtnPressed: movl $0,0x258(%rbx) in the ctor (0x14a6d0 mfc140.dll)");
static_assert(sizeof(S_CMFCTasksPaneFrameWnd) == 0x260,
              "CMFCTasksPaneFrameWnd sizeof: CreateObject news 0x260 (0x14a660 mfc140.dll)");

// CalcBorderSize hands OpenMFC's CRect to ::SetRect as a RECT, and
// OnTrackCaptionButtons receives CPoint by value (an 8-byte class, which the
// MS x64 ABI passes in a register only if it is trivially copyable; otherwise
// it would travel by hidden reference and mismatch retail callers).
static_assert(sizeof(CRect) == sizeof(RECT), "CRect must have RECT's layout");
static_assert(sizeof(CPoint) == 8 && std::is_trivially_copyable<CPoint>::value,
              "CPoint by value must travel in one register");

inline S_CMFCTasksPaneFrameWnd* Self(void* pThis) {
    return static_cast<S_CMFCTasksPaneFrameWnd*>(pThis);
}

// Retail caption-button ids this class handles (AddButton / OnPressButtons /
// OnNcPaint compare CMFCCaptionButton::m_nHit, +0x20, against them).
constexpr unsigned int kHitBack    = 0x17;   // back button
constexpr unsigned int kHitForward = 0x18;   // forward button
constexpr unsigned int kHitMenu    = 0x19;   // CMFCCaptionMenuButton (tasks menu)

}  // namespace

// Symbol: ??0CMFCTasksPaneFrameWnd@@QEAA@XZ
// Retail (RVA 0x14a6d0 mfc140.dll / 0x14c060 mfc140u) -- see the layout note at
// the top of this file: base ctor, m_bMenuBtnPressed = FALSE, vfptr store.
// PARTIAL: only the m_bMenuBtnPressed store is made.  The CPaneFrameWnd base
// constructor is NOT run: OpenMFC's ??0CPaneFrameWnd@@ (docking/Thunks.cpp)
// placement-news a 0x278-byte object whose constructor memsets its padding
// through +0x274, and a CMFCTasksPaneFrameWnd allocation is 0x260 bytes
// (retail CreateObject; any client compiled against the shipping headers), so
// it would write 0x14 bytes past the end of the block.  No vfptr is
// stored either (no retail-shaped vtable exists in this DLL).
extern "C" void* MS_ABI impl___0CMFCTasksPaneFrameWnd__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    Self(pThis)->m_bMenuBtnPressed = FALSE;
    return pThis;
}
// Symbol: ??1CMFCTasksPaneFrameWnd@@UEAA@XZ
// Retail (RVA 0x14a750 mfc140.dll / 0x14c0e0 mfc140u):
//     vfptr = &CMFCTasksPaneFrameWnd::vftable;
//     CPaneFrameWnd::~CPaneFrameWnd();              // tail jump 0xad890 (mfc140.dll)
// STUB: this must stay paired with the constructor above, which does not
// construct the CPaneFrameWnd base (it cannot fit in the 0x260-byte object);
// running OpenMFC's ~CPaneFrameWnd over members that were never constructed
// would tear down garbage.
extern "C" void MS_ABI impl___1CMFCTasksPaneFrameWnd__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddButton@CMFCTasksPaneFrameWnd@@MEAAXI@Z
// Retail (RVA 0x14a8b0 mfc140.dll / 0x14c240 mfc140u):
//     for (node = m_lstCaptionButtons.m_pNodeHead /*0x188*/; node; node = node->pNext)
//         if (((CMFCCaptionButton*)node->data)->m_nHit /*+0x20*/ == nHit) break;
//     if (<found a non-NULL button>) return;
//     switch (nHit) {
//     case 0x17: case 0x18:  p = new CMFCCaptionButton   /*0x38 bytes*/, m_nHit = nHit; break;
//     case 0x19:             p = new CMFCCaptionMenuButton /*0x48 bytes*/, m_nHit = 0x19; break;
//     default:               CPaneFrameWnd::AddButton(nHit); return;   // call 0xb1510
//     }
//     m_lstCaptionButtons.AddHead(p);          // call 0x230440 with this+0x180
// (The list is a CObList per afxpaneframewnd.h; the symbol map names the
// shared entry 0x230440 ?AddHead@CPtrList@@ -- identical code folded.  The
// vfptrs stored are .rdata 0x2ddd78, whose slot +0x30 is
// CMFCCaptionButton::OnDraw, and 0x2dddc0, whose slot +0x30 is
// CMFCCaptionMenuButton::OnDraw.)
// PARTIAL: there is no caption-button list in this object layout (see the file
// header), so the search finds nothing and the three task-pane buttons
// (0x17/0x18/0x19) have nowhere to go -- they are NOT created.  Only the
// default branch, a direct (non-virtual) call to the base, is reproduced.
extern "C" void MS_ABI impl__AddButton_CMFCTasksPaneFrameWnd__MEAAXI_Z(void* pThis, unsigned int nHit) {
    if (pThis == nullptr) return;
    if (nHit == kHitBack || nHit == kHitForward || nHit == kHitMenu) return;
    impl__AddButton_CPaneFrameWnd__MEAAXI_Z(pThis, nHit);
}

// Symbol: ?CalcBorderSize@CMFCTasksPaneFrameWnd@@UEBAXAEAVCRect@@@Z
// Decoded from retail (RVA 0x14ac90 mfc140.dll / 0x14c620 mfc140u), in full:
//     ::SetRect(&rectBorderSize, 3, 3, 3, 3);
// (import slot 0x1802c5398 in mfc140.dll resolves to USER32!SetRect).  The
// frame's own state is not consulted.
extern "C" void MS_ABI impl__CalcBorderSize_CMFCTasksPaneFrameWnd__UEBAXAEAVCRect___Z(const void* pThis, CRect* pRectBorderSize) {
    (void)pThis;
    // OpenMFC's CRect (include/openmfc/afxwin.h) does not derive from RECT but
    // has the same four-int layout, so it is handed to ::SetRect as one.
    ::SetRect(reinterpret_cast<RECT*>(pRectBorderSize), 3, 3, 3, 3);
}

// Symbol: ?CreateObject@CMFCTasksPaneFrameWnd@@SAPEAVCObject@@XZ
// Retail (RVA 0x14a660 mfc140.dll / 0x14bff0 mfc140u):
//     void* p = operator new(0x260);          // call 0x2840 (??2@YAPEAX_K@Z)
//     if (p) CMFCTasksPaneFrameWnd::CMFCTasksPaneFrameWnd(p);   // call 0x14a6d0
//     return p;
// STUB: the constructor above cannot build the CPaneFrameWnd base or install a
// vfptr (see its note), so the object this would return has no vtable; the
// first virtual call a CRuntimeClass::CreateObject caller makes on it would
// fault.  NULL is the documented failure value of CreateObject.
extern "C" void* MS_ABI impl__CreateObject_CMFCTasksPaneFrameWnd__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?OnDrawBorder@CMFCTasksPaneFrameWnd@@MEAAXPEAVCDC@@@Z
// Decoded from retail (RVA 0x14ab20 mfc140.dll / 0x14c4b0 mfc140u), in full: a
// single `jmp` to CPaneFrameWnd::OnDrawBorder (0xaff70 mfc140.dll), i.e. the
// override only forwards to the base.  Reproduced as the same direct call.
extern "C" void MS_ABI impl__OnDrawBorder_CMFCTasksPaneFrameWnd__MEAAXPEAVCDC___Z(void* pThis, void* pDC) {
    impl__OnDrawBorder_CPaneFrameWnd__MEAAXPEAVCDC___Z(pThis, pDC);
}

// Symbol: ?OnDrawCaptionButtons@CMFCTasksPaneFrameWnd@@MEAAXPEAVCDC@@@Z
// Retail (RVA 0x14ab30 mfc140.dll / 0x14c4c0 mfc140u), in full:
//     for (node = m_lstCaptionButtons.m_pNodeHead /*0x188*/; node; node = node->pNext) {
//         CMFCCaptionButton* b = (CMFCCaptionButton*)node->data;
//         BOOL b4 = (b->m_nHit /*+0x20*/ == 9) ? (<this dword +0xfc> == 0) : TRUE;
//         b->OnDraw(pDC, FALSE, TRUE, b4, FALSE);        // button vtable +0x30
//     }
// (CMFCCaptionButton::OnDraw(CDC*, BOOL bActive, BOOL bHorz, BOOL bMaximized,
// BOOL bDisabled) is slot +0x30 of the CMFCCaptionButton vftable at .rdata
// 0x2ddd78; the arguments are r8 = 0, r9 = 1, [rsp+0x20] = b4, [rsp+0x28] = 0.)
// There is no base-class call.
// STUB: the loop is the whole body and there is no caption-button list in this
// object layout (see the file header), so there is nothing to draw.
extern "C" void MS_ABI impl__OnDrawCaptionButtons_CMFCTasksPaneFrameWnd__MEAAXPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?OnNcPaint@CMFCTasksPaneFrameWnd@@IEAAXXZ
// Retail (RVA 0x14aa10 mfc140.dll / 0x14c3a0 mfc140u, by ordinal 10644):
//     CWnd* p = GetPane();                                   // vtable +0x360
//     CMFCTasksPane* pTP = (p && p->IsKindOf(RUNTIME_CLASS(CMFCTasksPane)))  // 0x233310, RTC 0x1803aa568
//                          ? (CMFCTasksPane*)p : NULL;
//     BOOL bMultiPage = pTP && pTP->m_lstTasksPanes.m_nCount /*+0x620*/ > 1;
//     BOOL bNavBar    = pTP && pTP->m_bUseNavigationToolbar /*+0x4e4*/;
//     for each caption button b in the list at 0x188:
//         if (b->m_nHit is 0x17, 0x18 or 0x19)
//             b->m_bHidden /*+0x10*/ = !(bMultiPage && !bNavBar);
//         if (b->m_nHit == 0x17)
//             b->m_bEnabled /*+0x14*/ = pTP && pTP->m_iActivePage /*+0x504*/ > 0;
//         else if (b->m_nHit == 0x18)
//             b->m_bEnabled /*+0x14*/ = pTP && pTP->m_iActivePage
//                                   < pTP->m_arrHistoryStack.m_nSize /*+0x788*/ - 1;
// (Member names are the shipping headers': afxtaskspane.h for CMFCTasksPane,
// afxcaptionbutton.h for CMFCCaptionButton, whose declaration order -- vfptr,
// m_bPushed, m_bFocused, m_bHidden, m_bEnabled, m_bDroppedDown, m_bLeftAlign,
// m_nHit -- puts m_bHidden at +0x10, m_bEnabled at +0x14 and m_nHit at +0x20.
// The CMFCTasksPane offsets agree with detail/CMFCTasksPaneSupport.h, which
// spells two of them differently: the +0x620 count lives inside its
// m_lstTasksPanes blob, and +0x788 is its m_nHistorySize.)
//     UpdateTooltips();                                      // direct call 0xb1870
//     CPaneFrameWnd::OnNcPaint();                            // tail jump 0xaf9d0
// Both trailing calls are UNCONDITIONAL and are reproduced.  The loop is not:
// there is no caption-button list in this object layout (see the file header),
// so it has nothing to iterate, and the GetPane/IsKindOf lookup only feeds that
// loop, so it is omitted too.
extern "C" void MS_ABI impl__OnNcPaint_CMFCTasksPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(pThis);
    impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ(pThis);
}

// Symbol: ?OnNeedTipText@CMFCTasksPaneFrameWnd@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
// Retail (RVA 0x14ad00 mfc140.dll / 0x14c690 mfc140u, by ordinal 10662).  Arguments: rcx this, edx id, r8 pNMH,
// r9 pResult (pResult is never dereferenced here; r9 is only passed through,
// untouched, on the tail jump to the base).
//     ENSURE(pNMH != NULL);                    // NULL -> AfxThrowInvalidArgException (0x225b80)
//     if (m_pToolTip /*0x1b8*/ == NULL || m_pToolTip->m_hWnd /*+0x40*/ == NULL
//         || pNMH->hwndFrom != m_pToolTip->m_hWnd)
//         return FALSE;                        // NOT forwarded to the base
//     idx = pNMH->idFrom - 1; if idFrom != 0 and idx is inside the
//     caption-button list (count 0x198) and that button's m_nHit is
//     0x17/0x18/0x19, copy a per-button literal -- "Back" (0x17), "Forward"
//     (0x18) or "Other Tasks Pane" (0x19), read at .rdata 0x33e9a0/0x33e9a8/
//     0x33e9b0 mfc140.dll -- into a static CString (.data 0x1803bb228
//     mfc140.dll) with CSimpleStringT::SetString, store its buffer in
//     ((NMTTDISPINFO*)pNMH)->lpszText (+0x18) and return TRUE;
//     otherwise tail-jump to CPaneFrameWnd::OnNeedTipText(id, pNMH, pResult) (0xb4120).
// Reproduced: the ENSURE, then retail's m_pToolTip == NULL path.  This object
// layout has no tooltip control at 0x1b8 (see the file header; the base
// UpdateTooltips in docking/CPaneFrameWnd.cpp reproduces the same NULL path),
// so that path is the one retail takes, and it returns FALSE directly.
extern "C" int MS_ABI impl__OnNeedTipText_CMFCTasksPaneFrameWnd__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    void* pThis, unsigned int id, NMHDR* pNMH, __int64* pResult) {
    (void)pThis;
    (void)id;
    (void)pResult;
    if (pNMH == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    return FALSE;
}

// Symbol: ?OnPressButtons@CMFCTasksPaneFrameWnd@@MEAAXI@Z
// Retail (RVA 0x14abb0 mfc140.dll / 0x14c540 mfc140u, by ordinal 10816):
//     CWnd* p = GetPane();                                          // vtable +0x360
//     if (p != NULL && p->IsKindOf(RUNTIME_CLASS(CMFCTasksPane))) { // 0x233310, RTC 0x1803aa568
//         switch (nHit) {
//         case 0x17: pTP->OnPressBackButton();    break;            // pane vtable +0x750
//         case 0x18: pTP->OnPressForwardButton(); break;            // pane vtable +0x758
//         case 0x19:
//             b = <caption button with m_nHit 0x19 in the list at 0x188>;
//             if (b != NULL) {
//                 m_bMenuBtnPressed /*0x258*/ = TRUE;
//                 pTP->OnPressOtherButton(b, this);                 // pane vtable +0x768
//                 m_bMenuBtnPressed = FALSE;
//             }
//         }
//     }
//     CPaneFrameWnd::OnPressButtons(nHit);          // tail jump 0xb4430, reached on EVERY path
// Deviations: GetPane and the two CMFCTasksPane virtuals are called through
// their exported thunks, so an override in a further-derived class is not
// reached (the same choice taskspane/CMFCTasksPane.cpp's own OnPressButtons
// makes for these two slots).  The 0x19 branch finds no button: there is no
// caption-button list in this object layout (see the file header), so it ends
// at retail's b == NULL test and OnPressOtherButton is never reached.
extern "C" void MS_ABI impl__OnPressButtons_CMFCTasksPaneFrameWnd__MEAAXI_Z(void* pThis, unsigned int nHit) {
    if (pThis == nullptr) return;
    CWnd* pPane = static_cast<CWnd*>(impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    CRuntimeClass* pTasksPaneClass = impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ();
    if (pPane != nullptr && pTasksPaneClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pTasksPaneClass)) {
        CMFCTasksPane* pTasksPane = reinterpret_cast<CMFCTasksPane*>(pPane);
        switch (nHit) {
        case kHitBack:
            impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(pTasksPane);
            break;
        case kHitForward:
            impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(pTasksPane);
            break;
        case kHitMenu:
            // Retail looks the menu button up in the list at 0x188; none exists
            // in this layout, so this is retail's "not found" path.
            break;
        default:
            break;
        }
    }
    impl__OnPressButtons_CPaneFrameWnd__MEAAXI_Z(pThis, nHit);
}

// Symbol: ?OnTrackCaptionButtons@CMFCTasksPaneFrameWnd@@MEAAXVCPoint@@@Z
// Decoded from retail (RVA 0x14acc0 mfc140.dll / 0x14c650 mfc140u, by ordinal
// 11420), in full:
//     if (!m_bMenuBtnPressed /*0x258*/)
//         CPaneFrameWnd::OnTrackCaptionButtons(point);   // direct call 0xb2170
// i.e. hot-tracking is suppressed while retail OnPressButtons has the flag set,
// which is only for the duration of its CMFCTasksPane::OnPressOtherButton call.
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CMFCTasksPaneFrameWnd__MEAAXVCPoint___Z(void* pThis, CPoint point) {
    if (pThis == nullptr) return;
    if (Self(pThis)->m_bMenuBtnPressed == FALSE) {
        impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(pThis, point);
    }
}

// Symbol: ?SetCaptionButtons@CMFCTasksPaneFrameWnd@@UEAAXK@Z
// Retail (RVA 0x14a770 mfc140.dll / 0x14c100 mfc140u, by ordinal 12999):
//     RemoveAllCaptionButtons();                             // direct call 0xb1810
//     if (dwButtons & 2) {
//         CWnd* p = GetPane();                               // vtable +0x360
//         if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane))     // 0x233310, RTC 0x1802da490
//               && p-><CBasePane vtable +0x3a0>())
//             AddButton(0x14);                               // vtable +0x470
//     }
//     if (dwButtons & 1) AddButton(9);                       // vtable +0x470
//     if (dwButtons & 4) AddButton(8);
//     AddButton(0x17); AddButton(0x18); AddButton(0x19);     // unconditional
//     m_dwCaptionButtons /*0x12c*/ = dwButtons | 0x70;
//     SetCaptionButtonsToolTips();                           // direct call 0xb4300
//     ArrangeCaptionButtons();                               // direct call 0xb1990
//     ::SendMessage(m_hWnd /*+0x40*/, WM_NCPAINT, 0, 0);     // tail jump via import
//                                                            // slot 0x1802c5378 (SendMessageA
//                                                            // in mfc140.dll -> W in mfc140u)
// STUB: the m_dwCaptionButtons store is not made from this translation unit.
// In this build that field is docking/CPaneFrameWnd.cpp's side-table word
// PaneFrameState::rollupFlags (detail/CPaneFrameWndSupport.h), reachable only
// through the C++ helper openmfc::detail::cpaneframewnd::GetState (defined in
// detail/CPaneFrameWndSupport.cpp).  That helper would link, but it is a
// mangled C++ symbol, and this campaign's per-file link audit rejects every
// new C++ (_Z) undefined symbol; there is no extern "C" accessor for the word.
// Without the store the mask that SetRollUpTimer, KillRollupTimer and
// SetTaskPaneCaptionButtons read back would be wrong, so a body that ran the
// rest of the sequence would leave the frame in a state retail never
// produces.
extern "C" void MS_ABI impl__SetCaptionButtons_CMFCTasksPaneFrameWnd__UEAAXK_Z(void* pThis, unsigned long dwButtons) {
    (void)pThis;
    (void)dwButtons;
}

// Symbol: ?SetTaskPaneCaptionButtons@CMFCTasksPaneFrameWnd@@QEAAXXZ
// Retail (RVA 0x14a9f0 mfc140.dll / 0x14c380 mfc140u, by ordinal 13655), in full:
//     SetCaptionButtons(m_dwCaptionButtons /*0x12c*/ | 0x70);   // tail jump, vtable +0x328
// STUB: m_dwCaptionButtons is not readable from this translation unit (see
// SetCaptionButtons above), and the SetCaptionButtons it would call is itself
// a stub for the same reason.
extern "C" void MS_ABI impl__SetTaskPaneCaptionButtons_CMFCTasksPaneFrameWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?StopCaptionButtonsTracking@CMFCTasksPaneFrameWnd@@MEAAXXZ
// Decoded from retail (RVA 0x14ace0 mfc140.dll / 0x14c670 mfc140u, by ordinal
// 13915), in full:
//     if (!m_bMenuBtnPressed /*0x258*/)
//         CPaneFrameWnd::StopCaptionButtonsTracking();   // direct call 0xb2280
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CMFCTasksPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (Self(pThis)->m_bMenuBtnPressed == FALSE) {
        impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(pThis);
    }
}
