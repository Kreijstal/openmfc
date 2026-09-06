// CMFCRibbonPanelMenuBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Status of this file: every export below is still a documented STUB.
// ---------------------------------------------------------------------------
// All 37 remaining exports were located in the retail mfc140 export map and
// disassembled (RVAs are cited at each definition, and the retail bodies were
// read in full).  None of them could be transcribed, for one structural
// reason: OpenMFC does not model this class at all.
//
//   * `CMFCRibbonPanelMenuBar` is not declared in include/openmfc/ (no public
//     declaration, and there is no phase4/src/detail/…Support.h shadow
//     struct for it).  The only trace of it in the tree is the CRuntimeClass
//     descriptor -- MFC_TOOLBARS_DESC(CMFCRibbonPanelMenuBar, 5520, 0xFFFF,
//     &classCMFCPopupMenuBar) in featurepack/*/RuntimeClasses.cpp -- whose
//     object size 5520 == 0x1590 matches the retail layout harvested below.
//   * Retail keeps all of its own state between +0x12e8 and +0x158f, i.e. past
//     the end of everything OpenMFC's clean-room CMFCToolBar/CMFCPopupMenuBar
//     chain lays out, and the five constructor thunks above initialise none of
//     it.  Reading those offsets would read uninitialised memory.
//   * Worse, nearly every body dispatches through the RETAIL vtables of the
//     objects it reaches (panel slots +0x180/+0x188/+0x238/+0x240/+0x248/
//     +0x1a8/+0x1c8/+0x1e8/+0x210/+0x228, ribbon-bar slot +0x6d8, element
//     slots +0x280/+0x288/+0x340/+0x360/+0x438/+0x448/+0x4c0) and writes back
//     into those objects at retail offsets (e.g. AddButtons/CommonInit store
//     `this` into the panel at +0x160, AdjustLocations writes the panel at
//     +0xd4/+0xf8).  OpenMFC's clean-room CMFCRibbonPanel (afxmfc.h) is a
//     vptr + CString + HICON + a 64-byte pad -- under 0x60 bytes -- with an
//     unrelated vtable, so a store at panel+0x160 lands past the end of the
//     object and those calls would jump to the wrong slots.
//
// So the bodies stay empty on purpose (see BRIEFING §6: an accurate documented
// stub beats a plausible wrong body).  What this pass DID change:
//
//   * every thunk now carries its real, typed signature -- the generated stubs
//     were missing the implicit `this` entirely and mistyped the by-value
//     CPoint/CSize parameters;
//   * CalcSize now honours the hidden-return-slot ABI.  Retail (RVA 0x11f590)
//     takes `this` in RCX and the CSize return slot in RDX and returns that
//     slot pointer in RAX; the previous stub returned nullptr and never wrote
//     the slot, so an MSVC caller read an uninitialised CSize.  It now zeroes
//     the slot and returns it;
//   * each definition records the retail RVA plus what the retail body
//     actually does, so the next pass can implement from evidence.
//
// Adversarial review pass: every note below was re-checked instruction by
// instruction against the retail bodies.  Eleven claims were wrong and are
// corrected in place, called out where they occur: +0x1580 was labelled a
// "palette/gallery element" when it is a CMFCRibbonCategory*; +0x1568 was
// labelled a "parent category" when it is the SOURCE CMFCRibbonPanel*;
// CalcSize's long path was described as a visual-manager walk it does not
// perform; PreTranslateMessage's base was named CMFCPopupMenuBar's when
// 0x152fa0 is CMFCToolBar's; OnChangeHighlighted was said to store pHot at
// +0x1570 when it stores the dropped-down button; OnClickButton's early-out
// RTTI was called CMFCRibbonPanelMenu when it is CMFCRibbonMiniToolBar;
// RestoreDelayedSubMenu was said to pass a rect where retail passes a by-value
// CPoint; SetPreferedSize's minimum width was attributed to the wrong object;
// OnTimer said "three timer cases" and listed four; and both OnDestroy and
// PreTranslateMessage claimed their base handler had no impl__ thunk in this
// tree when both thunks exist.  The bodies themselves are unchanged: they
// were, and remain, documented stubs.
//
// Harvested retail layout (offsets read out of the disassembled bodies; the
// NAMES are descriptive labels chosen here, not PDB names):
//
//   +0x0040  m_hWnd                      (CWnd)
//   +0x0150  accessibility data block    (OnSetAccData zeroes +0x180..+0x19f)
//   +0x111c  flag; AdjustLocations returns at once when it is non-zero
//   +0x1154  last hot index              (OnMouseMove)
//   +0x12e8  tooltip control pointer     (its CWnd::m_hWnd at +0x40)
//   +0x1350  set to 1 by CommonInit
//   +0x1420  mouse-tracking-active flag
//   +0x1424  CommonInit zeroes 8 bytes here; AddButtons stores dword 1
//   +0x1430  tooltip-enable flag A       (OnCreate / OnUpdateToolTips)
//   +0x1434  8-byte field, CommonInit stores 1
//   +0x1438  tooltip-enable flag B
//   +0x143c  8-byte field, CommonInit stores 0; dtor tests it as a flag
//   +0x1444  drag/scroll-timer-armed flag
//   +0x1448  prefered size cx            (SetPreferedSize)
//   +0x144c  prefered size cy            (SetPreferedSize)
//   +0x1450  8-byte CSize returned by CalcSize when +0x1580 is set
//   +0x1458  16-byte RECT (SetRectEmpty'd by CommonInit/OnLButtonDown/Up)
//   +0x1468  last mouse x, -1 == none    (CommonInit stores -1)
//   +0x146c  last mouse y, -1 == none    (CommonInit stores -1)
//   +0x1470  embedded vertical scroll bar (own vtable 0x1802f6218; OnCreate
//            creates it with style 0x50000001 = WS_CHILD|WS_VISIBLE|SBS_VERT,
//            its m_hWnd is at +0x1470+0x40; destroyed by ~dtor)
//   +0x14b0  HWND passed to SetScrollPos(hWnd, SB_CTL, pos, TRUE) by OnVScroll
//   +0x1558  parent CMFCRibbonBar
//   +0x1560  owned CMFCRibbonPanel       (allocated 0x6f8 bytes by the XZ /
//            CMFCRibbonBar / CMFCRibbonGallery ctors; the CMFCRibbonPanel ctor
//            stores a CRuntimeClass::CreateObject clone of its argument here)
//   +0x1568  SOURCE CMFCRibbonPanel      (the argument of the CMFCRibbonPanel
//            ctor, stored verbatim at RVA 0x11ea33 -- NOT a category)
//   +0x1570  delayed sub-menu element
//   +0x1578  pending-open element
//   +0x1580  owned CMFCRibbonCategory    (a CreateObject clone of the argument
//            of the CMFCRibbonCategory ctor, stored at RVA 0x11edc6; the ctor
//            then sets clone+0x768 = this)
//   +0x1588  pressed element
//   sizeof   0x1590 (5520)
//
// Identification evidence for +0x1568 / +0x1580 (both were mislabelled in an
// earlier pass of this file, so the proof is recorded here):
//   * +0x1580 is passed as `this` to CMFCRibbonCategory::HitTest (RVA 0xef0e0),
//     ::HitTestEx (0xef1d0), ::GetFocused (0xf0b00) and ::GetDroppedDown
//     (0xefdd0).  It is a CMFCRibbonCategory*, not a palette/gallery element.
//   * +0x1560 is passed as `this` to CMFCRibbonPanel::GetDroppedDown
//     (0x11c6e0), ::GetFocused (0x11dc50), ::GetElements (0x11c450),
//     ::RemoveAll (0x1179a0), ::GetPaltteMinSize (0x11d6a0) and
//     ::ScrollPalette (0x11d5a0).
//   * GetTopLevelRibbonBar dereferences +0x1568 as `panel->+0x158 -> +0x760`,
//     and DoPaint (0x11fbc2) shows panel+0x158 IS the panel's category, so
//     +0x1568 is a panel whose category carries the ribbon bar at +0x760.
//   * panel+0x168 is a CMFCRibbonGallery: OnVScroll calls
//     CMFCRibbonGallery::GetMenuRowHeight (0x1156f0) on it.
//
// This table is NOT exhaustive: the bodies also touch +0x111c, +0x1364,
// +0x13a4, +0x1428, +0x142c and +0x1440, which are described at the individual
// functions below but have no entry here.
//
// Retail is 4-byte packed: CommonInit stores 8-byte values at +0x1424, +0x1434
// and +0x143c, which are not 8-byte aligned.
//
// Timer IDs used by the retail bodies: 0xec1b (pending open), 0xec1c (delayed
// sub-menu), 0xec1d (drag scroll), 0xec07 (tooltip hot-item).
//
// Caveat on the notes below: Win32 API names (KillTimer, RedrawWindow, ...)
// are INFERRED from each IAT slot's call shape -- argument count, the constant
// message/metric ids passed, and the fact that the same slot is used the same
// way at several sites.  They were not read out of an import table, and the
// field names above are labels chosen here, not PDB names.  The RVAs, the
// offsets, the vtable slot numbers and the control flow are read directly out
// of the disassembly.


#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CMFCRibbonPanelMenuBar@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, void* pRibbonBar, const void* elements, unsigned int unknown) {
    (void)pRibbonBar;
    (void)elements;
    (void)unknown;
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonCategory@@VCSize@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonCategory__VCSize___Z(
    void* pThis, void* pCategory, const void* size) {
    (void)pCategory;
    (void)size;
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonGallery@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonGallery___Z(
    void* pThis, void* pGallery) {
    (void)pGallery;
    return pThis;
}
// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pPanel;
    return pThis;
}

// ~CMFCRibbonPanelMenuBar(): retail RVA 0x11f1a0.  Installs its own vptr, then
// (a) for the owned panel (+0x1560) and the owned category (+0x1580): if the
// parent ribbon bar (+0x1558) still points at it through +0xc10 it calls
// CMFCRibbonBar::DeactivateKeyboardFocus(pBar, FALSE) (RVA 0xe28e0 -- what
// that call does was not itself disassembled), then calls the object's
// scalar-deleting destructor (vtable slot 1) with the delete flag; (b) if the
// ribbon bar has a top-level window at +0xb08
// and either +0x434 or +0x468 is 0, clears that window's +0x22c/+0x214, takes
// its rect at +0x1a0, overwrites rc.bottom with ribbonBar+0x1454, then
// InflateRect(&rc,1,1) and RedrawWindow(bar->m_hWnd, &rc, NULL, 0x105);
// (c) if +0x143c is non-zero and the global at 0x1803b6fe8 holds a window that
// is still valid and visible, calls the RVA 0xbb290 helper with NULL;
// (d) destroys the embedded CScrollBar at +0x1470 (~CScrollBar, RVA
// 0x2925b0) and chains to
// ~CMFCPopupMenuBar.
// TODO(clean-room): every step needs the retail panel/ribbon-bar layouts and
// their vtables; nothing is freed here.
// Symbol: ??1CMFCRibbonPanelMenuBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonPanelMenuBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// AddButtons(CMFCRibbonBar* pRibbonBar, const CArray<CMFCRibbonBaseElement*,
// CMFCRibbonBaseElement*>& arButtons, int nType): retail RVA 0x11eee0.
// Stores 1 at +0x1424, pRibbonBar at +0x1558, `this` into the panel at +0x160,
// nType into the panel at +0xc0, 2 into the panel at +0xf0 and +0xf4, calls a
// panel reset helper (RVA 0x1179a0), then for each element of the CArray
// (m_pData at +0x8, m_nSize at +0x10) clones it through its RTTI
// (CRuntimeClass::CreateObject), runs clone vtable slots +0x2e0 and +0x2f0
// with the source element, sets clone+0x12c = 1, calls clone slot +0x2e8 with
// `this`, and adds the clone to the panel through panel slot +0x1a8.
// TODO(clean-room): needs the retail panel layout/vtable and the ribbon
// element clone protocol; no elements are added.
// Symbol: ?AddButtons@CMFCRibbonPanelMenuBar@@IEAAXPEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void MS_ABI impl__AddButtons_CMFCRibbonPanelMenuBar__IEAAXPEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, void* pRibbonBar, const void* pArButtons, int nType) {
    (void)pThis;
    (void)pRibbonBar;
    (void)pArButtons;
    (void)nType;
}

// AdjustLocations(): retail RVA 0x11f360.  Returns immediately when +0x111c is
// non-zero.  Otherwise GetClientRect(m_hWnd, &rc), constructs a client DC on
// this window (RVA 0x2a1a60) and selects the ribbon bar's font into it
// (SendMessageW(bar->m_hWnd, WM_GETFONT (0x31), 0, 0) ->
// CGdiObject::FromHandle (RVA 0x2a1de0 -- CGdiObject's, not CFont's) ->
// CDC::SelectObject (RVA 0x2a0670); a NULL old font throws
// AfxThrowInvalidArgException).
// It then either lays the owned category (+0x1580) out -- copying rc into
// category+0xc0 and calling category slot +0x170 -- or, when +0x1580 is NULL
// and the panel (+0x1560) is not, copies +0x13a4 to panel+0xd4 and +0x1364 to
// panel+0xf8, calls panel slots +0x1e8 (with the DC and rc) and +0x210, and if
// GetTopLevelRibbonBar() still owns this panel through +0xc10 calls
// CMFCRibbonBar::ShowKeyTips(TRUE) (RVA 0xe3ed0) before clearing panel+0xd4.
// Either way it restores the old font and destroys the DC (RVA 0x2a1b20).
// TODO(clean-room): needs the retail panel/category layouts and vtables.
// Symbol: ?AdjustLocations@CMFCRibbonPanelMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// CalcSize(BOOL bVertDock) -> CSize: retail RVA 0x11f590.  MS x64 ABI for a
// CSize return: RCX = this, RDX = the hidden CSize slot, R8D = bVertDock, and
// the slot pointer comes back in RAX -- confirmed at 0x11f5b5..0x11f5d1, where
// retail's fast path copies the 8-byte value at +0x1450 into (RDX) and returns
// RDX.  That fast path is taken when the owned category (+0x1580) is set; the
// long path measures the menu entirely through the owned panel (+0x1560):
// it copies +0x1430 into panel+0xc4, builds a CClientDC on the RIBBON BAR
// (+0x1558, RVA 0x2a1a60), selects the bar's font into it (WM_GETFONT ->
// CGdiObject::FromHandle -> CDC::SelectObject; a NULL old font throws), then
// either calls panel slot +0x1e8 and takes panel+0x10c..+0x118 as the size
// (clamped against +0x1448/+0x144c), or -- when +0x1428 is 0 -- walks the
// panel's palette rows (+0x6b0/+0x6b8) and element array (+0x6d8/+0x6e0,
// rects at element+0xc8) through panel slot +0x1a0.  No visual manager and no
// category are involved on either path: an earlier pass of this file claimed
// they were, but 0x11f590..0x11fa23 contains no such code.  The bVertDock
// argument in R8D is never read by retail.
// TODO(clean-room): the measurement needs the retail panel layout/vtable, so
// the slot is zeroed.  Writing and returning the slot is itself the fix here:
// the previous generated stub returned nullptr and left the caller's CSize
// uninitialised.
// Symbol: ?CalcSize@CMFCRibbonPanelMenuBar@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__CalcSize_CMFCRibbonPanelMenuBar__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bVertDock) {
    (void)pThis;
    (void)bVertDock;
    if (pRet == nullptr) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}

// CloseDelayedSubMenu(): retail RVA 0x1213e0.  If the delayed sub-menu element
// (+0x1570) is set: KillTimer(m_hWnd, 0xec1c), call that element's vtable slot
// +0x340, and clear +0x1570.  Does nothing when +0x1570 is NULL.
// TODO(clean-room): needs the +0x1570 member and the retail element vtable.
// Symbol: ?CloseDelayedSubMenu@CMFCRibbonPanelMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// CommonInit(): retail RVA 0x11f0e0 -- the shared tail of all five
// constructors.  If the owned panel (+0x1560) is set it stores `this` into the
// panel at +0x160; then zeroes +0x1580, +0x1450, +0x1570, +0x1578, +0x1588,
// +0x1424, +0x142c, +0x1568, +0x1558, +0x1420, +0x12e8, +0x1448, +0x143c and
// +0x1444, stores 1 at +0x1350 and +0x1434, -1 at +0x1468 and +0x146c, and
// SetRectEmpty()s the RECT at +0x1458.
// TODO(clean-room): the field writes alone are transcribable, but the very
// first store goes into a CMFCRibbonPanel at retail offset +0x160 while
// OpenMFC's clean-room CMFCRibbonPanel is under 0x60 bytes -- that store would
// land past the end of the object -- and the rest of the class has no OpenMFC
// declaration to write into.  Left empty until the class is modelled.
// Symbol: ?CommonInit@CMFCRibbonPanelMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// DoPaint(CDC* pDC): retail RVA 0x11fa30.  Builds a memory-DC paint helper
// over pDC (RVA 0x69d80) and picks either the memory DC or pDC from it, asks
// pDC for its clip box through vtable slot +0xb0 and substitutes the window
// rect when that box is empty, selects the ribbon bar's font (SendMessageW(
// bar->m_hWnd, WM_GETFONT (0x31), 0, 0) -> CGdiObject::FromHandle ->
// CDC::SelectObject)
// and returns if that fails, sets bkMode TRANSPARENT (RVA 0x2a07a0 with 1),
// then GetClientRect + InflateRect(rc, 3, 3) and asks the visual manager
// (AFX global getter RVA 0x97f4) to fill the popup background -- slot +0x438
// with the owned category (+0x1580), or with the SOURCE panel's category
// (panel+0x158) after temporarily pointing that category's +0x768 at `this`,
// or slot +0x558 when +0x1430 is set, or slot +0x78 otherwise.  It finishes
// with the owned category's slot +0x178 or the panel's slot +0x1e0, restores
// the font and tears the helper down.
// TODO(clean-room): the whole body walks the retail panel/category/element
// layouts and the retail visual-manager vtable; nothing is drawn.
// Symbol: ?DoPaint@CMFCRibbonPanelMenuBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCRibbonPanelMenuBar__UEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// FindByOrigin(CMFCRibbonBaseElement* pOriginal) const: retail RVA 0x120f90.
// Returns NULL when the owned panel (+0x1560) is NULL.  Otherwise it fills a
// local CArray<CMFCRibbonBaseElement*> through CMFCRibbonPanel::GetElements
// (RVA 0x11c450) and walks
// it in order, calling each element's vtable slot +0x360 with pOriginal and
// returning the first non-NULL result; NULL if none matches.
// TODO(clean-room): needs the panel element collection and the retail element
// vtable.  The NULL terminal below is retail's own no-panel result, but with
// the panel unmodelled it is reached unconditionally.
// Symbol: ?FindByOrigin@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBaseElement@@PEAV2@@Z
extern "C" void* MS_ABI impl__FindByOrigin_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__PEAV2__Z(
    const void* pThis, void* pOriginal) {
    (void)pThis;
    (void)pOriginal;
    return nullptr;
}

// GetDroppedDown() const: retail RVA 0x121b10.  Calls
// CMFCRibbonCategory::GetDroppedDown (RVA 0xefdd0) on the owned category
// (+0x1580); when +0x1580 is NULL it calls CMFCRibbonPanel::GetDroppedDown
// (RVA 0x11c6e0) on the owned panel (+0x1560) instead.  The
// result is returned only if CObject::IsKindOf (RVA 0x233310) against the
// descriptor at 0x1802fe9d0 succeeds -- the declared return type says that
// descriptor is CMFCRibbonButton -- else NULL.
// TODO(clean-room): needs the +0x1580/+0x1560 members and the retail
// category/panel helpers.
// Symbol: ?GetDroppedDown@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonButton@@XZ
extern "C" void* MS_ABI impl__GetDroppedDown_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonButton__XZ(
    const void* pThis) {
    (void)pThis;
    return nullptr;
}

// GetFocused() const: retail RVA 0x121bd0.  Tail-calls
// CMFCRibbonCategory::GetFocused (RVA 0xf0b00) on the owned category when
// +0x1580 is set, otherwise CMFCRibbonPanel::GetFocused (RVA 0x11dc50) on
// +0x1560.
// TODO(clean-room): needs the +0x1580/+0x1560 members and those helpers.
// Symbol: ?GetFocused@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetFocused_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__XZ(
    const void* pThis) {
    (void)pThis;
    return nullptr;
}

// GetTopLevelRibbonBar() const: retail RVA 0x121060.  Returns the parent
// ribbon bar at +0x1558 when it is non-NULL; otherwise it returns
// *(void**)(*(void**)(srcPanel + 0x158) + 0x760), where srcPanel is the SOURCE
// panel at +0x1568 and panel+0x158 is that panel's CMFCRibbonCategory (the
// same field DoPaint reads at 0x11fbc2).  Retail does NOT null-check +0x1568
// or panel+0x158 on that path.
// TODO(clean-room): needs +0x1558/+0x1568 and the retail panel and category
// layouts.
// Symbol: ?GetTopLevelRibbonBar@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBar@@XZ
extern "C" void* MS_ABI impl__GetTopLevelRibbonBar_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBar__XZ(
    const void* pThis) {
    (void)pThis;
    return nullptr;
}

// HitTest(CPoint point) const: retail RVA 0x121b60.  CPoint is passed by value
// packed into RDX (8-byte trivially-copyable class).  With an owned category
// (+0x1580) it tail-calls CMFCRibbonCategory::HitTest (RVA 0xef0e0) with a
// trailing 1; otherwise it tail-calls the owned panel's vtable slot +0x180
// with a trailing 0.  Retail does not null-check +0x1560 on that path.
// TODO(clean-room): needs the +0x1580/+0x1560 members and the retail vtables.
// Symbol: ?HitTest@CMFCRibbonPanelMenuBar@@QEBAPEAVCMFCRibbonBaseElement@@VCPoint@@@Z
extern "C" void* MS_ABI impl__HitTest_CMFCRibbonPanelMenuBar__QEBAPEAVCMFCRibbonBaseElement__VCPoint___Z(
    const void* pThis, POINT point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// HitTestEx(CPoint point) const: retail RVA 0x121ba0.  Same dispatch as
// HitTest but returning an index: CMFCRibbonCategory::HitTestEx (RVA 0xef1d0)
// when +0x1580 is set, otherwise the owned panel's vtable slot +0x188.
// TODO(clean-room): needs those members and vtables.  Retail has no shared
// "not found" terminal here -- 0 below is the stub's own value, NOT retail's
// -1 miss value, because there is nothing to hit-test against.
// Symbol: ?HitTestEx@CMFCRibbonPanelMenuBar@@QEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTestEx_CMFCRibbonPanelMenuBar__QEBAHVCPoint___Z(
    const void* pThis, POINT point) {
    (void)pThis;
    (void)point;
    return 0;
}

// OnChangeHighlighted(CMFCRibbonBaseElement* pHot): retail RVA 0x120430.
// Returns at once unless the owned panel (+0x1560) is set and panel+0xb8 is
// non-zero.  It then resolves the currently dropped-down button (RVA 0x11c6e0
// + IsKindOf(CMFCRibbonButton)) and the new hot button, and depending on
// whether they differ either arms the delayed-sub-menu timer 0xec1c -- storing
// the CURRENTLY DROPPED-DOWN button (NOT pHot) at +0x1570 and setting that
// button's +0x20c = 1, at 0x120565/0x12056f -- or kills the timer and closes
// the popup through the dropped-down button's slot +0x340; it also stores the
// new hot button at +0x1578 and manages timer 0xec1b for it, and forwards to
// the parent CMFCRibbonPanelMenu (RTTI 0x180308f60) through that window's
// +0x19b8 object's slot +0x408.
// TODO(clean-room): needs the retail element/menu layouts and vtables.
// Symbol: ?OnChangeHighlighted@CMFCRibbonPanelMenuBar@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__OnChangeHighlighted_CMFCRibbonPanelMenuBar__UEAAXPEAVCMFCRibbonBaseElement___Z(
    void* pThis, void* pHot) {
    (void)pThis;
    (void)pHot;
}

// OnClickButton(CMFCRibbonButton* pButton, CPoint point): retail RVA 0x1202c0.
// Clears pButton+0x144 and pButton+0x13c, RedrawWindow(m_hWnd, &pButton->rect
// (+0xc8), NULL, 0x105), then gives the panel's owner (panel+0x168) a chance
// to swallow the click through its slot +0x540.  Otherwise it runs the
// button's CMFCRibbonBaseElement::NotifyCommand(TRUE) (RVA 0x10af0).  It then
// returns early when +0x1438 is set AND the parent window IsKindOf the RTTI
// descriptor at 0x1803064f8 -- which is CMFCRibbonMiniToolBar, NOT
// CMFCRibbonPanelMenu as an earlier pass of this file claimed.  Otherwise,
// when NotifyCommand returned TRUE and the parent window is a
// CMFCRibbonPanelMenu (0x180308f60) whose CMFCPopupMenu::GetParentPopupMenu
// (RVA 0xb7b10) is one too, it sets that parent menu's +0x2f58 = 1; either way
// it finishes by calling slot +0xd0 on AFXGetParentFrame(this) (RVA 0x6bb10).
// TODO(clean-room): needs the retail button/panel/menu layouts and vtables.
// Symbol: ?OnClickButton@CMFCRibbonPanelMenuBar@@UEAAXPEAVCMFCRibbonButton@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnClickButton_CMFCRibbonPanelMenuBar__UEAAXPEAVCMFCRibbonButton__VCPoint___Z(
    void* pThis, void* pButton, POINT point) {
    (void)pThis;
    (void)pButton;
    (void)point;
}

// OnContextMenu(CWnd* pWnd, CPoint point): retail RVA 0x121720.  Bails out
// when the parent window matches the RTTI descriptor at 0x1803aa448 and its
// +0x19b8 object has +0x134 != 0.  Otherwise it cancels the drag-scroll timer
// 0xec1d (clearing +0x1444/+0x1588 and emptying the RECT at +0x1458), checks
// the own vtable slot +0x8a0 predicate plus +0x1434/+0x1438/+0x1430 and a
// GetSystemMetrics(0x17)/GetKeyState gate, closes any dropped-down button, and
// finally calls the ribbon bar's context-menu virtual (bar vtable slot +0x6d8)
// with either the focused element (point == (-1,-1)) or HitTest(point).
// TODO(clean-room): needs the retail bar/menu layouts and vtables.
// Symbol: ?OnContextMenu@CMFCRibbonPanelMenuBar@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCRibbonPanelMenuBar__IEAAXPEAVCWnd__VCPoint___Z(
    void* pThis, void* pWnd, POINT point) {
    (void)pThis;
    (void)pWnd;
    (void)point;
}

// OnCreate(CREATESTRUCT* lpcs): retail RVA 0x120930.  NOTE: the RVA map used
// here is mfc140.dll, the ANSI-typedef build, so it only carries the
// ...PEAUtagCREATESTRUCTA@@@Z spelling; the W spelling OpenMFC exports (it
// mirrors mfc140u.dll) is absent from that map.  Calls the base OnCreate
// and propagates its -1; then, when the tooltip flags at +0x1438/+0x1430
// allow it, creates the tooltip control at +0x12e8 with style 0x200, sends it
// TTM_SETMAXTIPWIDTH-style message 0x418 with 0x280 and registers this window
// with it; when panel+0x168 is set, creates the embedded scroll bar at +0x1470
// through its vtable slot +0x2d8 (style 0x50000001 = WS_CHILD|WS_VISIBLE|
// SBS_VERT, parent `this`) and stores it into panel+0x150; sets +0x1440 and
// +0x143c to 1 when the ribbon bar's +0x414 is >= 0; GetCursorPos into the
// +0x1468 pair.  Returns 0.
// TODO(clean-room): every step needs unmodelled state; returns 0, which is
// retail's success value, so window creation is not failed artificially.
// Symbol: ?OnCreate@CMFCRibbonPanelMenuBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCRibbonPanelMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, void* lpcs) {
    (void)pThis;
    (void)lpcs;
    return 0;
}

// OnDestroy(): retail RVA 0x120aa0.  If the tooltip control pointer at
// +0x12e8 is non-NULL it destroys it (RVA 0x180ac0), then tail-calls the base
// CMFCPopupMenuBar::OnDestroy (RVA 0xbe310).
// TODO(clean-room): +0x12e8 is unmodelled, so nothing is destroyed.  The base
// thunk DOES exist -- impl__OnDestroy_CMFCPopupMenuBar__IEAAXXZ at
// phase4/src/featurepack/menu/CMFCPopupMenuBar.cpp:162, contrary to an earlier
// pass of this file -- but it is itself an empty stub declared with no
// parameters, so chaining to it would add a link dependency for no behaviour.
// Symbol: ?OnDestroy@CMFCRibbonPanelMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// OnDrawMenuBorder(CDC* pDC): retail RVA 0x1208f0.  A pure forwarder: it
// tail-calls the owned category's vtable slot +0x1c0 when +0x1580 is set,
// otherwise the owned panel's vtable slot +0x228 (+0x1560), passing pDC and
// `this`.  Retail does not null-check +0x1560 on the second path.
// TODO(clean-room): needs those members and the retail vtables.
// Symbol: ?OnDrawMenuBorder@CMFCRibbonPanelMenuBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCRibbonPanelMenuBar__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// OnKey(UINT nChar): retail RVA 0x121550.  For VK_F10 with Shift down
// (GetKeyState(VK_SHIFT)) or VK_APPS it synthesises a context menu at (-1,-1)
// via OnContextMenu(this, this, CPoint(-1,-1)) (RVA 0x121720) and returns
// TRUE.  Otherwise it calls CMFCRibbonBar::ProcessKey (RVA 0xe3b40) on +0x1558
// -- with no null check on that member -- and returns TRUE if consumed;
// failing that it forwards to the panel's slot +0x1c8 (+0x1560) or, if there
// is no panel, to the owned category's slot +0x1e0 (+0x1580), each wrapped in
// an exception frame that restores the global at 0x1803b6ff0.  Returns FALSE
// when neither a panel nor a category is present.
// TODO(clean-room): needs the retail bar/panel/category layouts and vtables.
// FALSE below is retail's no-panel-no-category terminal (0x12163d).
// Symbol: ?OnKey@CMFCRibbonPanelMenuBar@@UEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonPanelMenuBar__UEAAHI_Z(
    void* pThis, unsigned int nChar) {
    (void)pThis;
    (void)nChar;
    return 0;
}

// OnLButtonDblClk(UINT nFlags, CPoint point): retail RVA 0x120230.  Calls the
// base handler (RVA 0xbfda0), then -- only if the own vtable slot +0x8a8
// predicate returns TRUE -- closes the dropped-down button via
// GetDroppedDown() and its slot +0x340.  Finally, unconditionally, it
// HitTest(point)s and calls the hit element's slot +0x448 with point.
// TODO(clean-room): needs the retail element vtable and the base handler.
// Symbol: ?OnLButtonDblClk@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// OnLButtonDown(UINT nFlags, CPoint point): retail RVA 0x120120.  Calls the
// base handler (RVA 0xbed00), closes any dropped-down button (slot +0x340),
// re-runs OnMouseMove (RVA 0x11fd20), clears +0x1588 and empties the RECT at
// +0x1458.  It then asks the owned category (slot +0x188) or the panel
// (slot +0x240)
// for the element under point; if the window is still valid it stores that
// element at +0x1588 and, when the element's slot +0x2a8 accepts a repeat
// interval (default 0x64), starts timer 0xec1d with it, sets +0x1444 = 1 and
// copies the element rect (+0xc8) into +0x1458.
// TODO(clean-room): needs the retail panel/category/element layouts and
// vtables.
// Symbol: ?OnLButtonDown@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// OnLButtonUp(UINT nFlags, CPoint point): retail RVA 0x120040.  If the
// drag-scroll flag +0x1444 is set it kills timer 0xec1d, clears +0x1444 and
// +0x1588 and empties the RECT at +0x1458.  It then calls the base handler
// (RVA 0xbedf0) and, while the window is still alive, forwards the click to
// the owned category (slot +0x190) or the panel (slot +0x248); if the window
// survives that too it re-runs OnMouseMove with the current cursor position.
// TODO(clean-room): needs the retail panel/category layouts and vtables.
// Symbol: ?OnLButtonUp@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// OnMouseLeave(): retail RVA 0x11ff50.  GetCursorPos + ScreenToClient +
// GetClientRect, and only when the cursor is OUTSIDE the client rect does it
// do anything: it calls OnMouseMove(0, (-1,-1)) (RVA 0x11fd20), clears the
// tracking flag +0x1420, tells the panel to un-highlight through slot +0x238
// with (-1,-1) and RedrawWindow(m_hWnd, NULL, NULL, 0x185).  On both paths it
// ends by storing 0 into +0x1420.
// TODO(clean-room): needs +0x1420, the retail panel vtable and OnMouseMove.
// Symbol: ?OnMouseLeave@CMFCRibbonPanelMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCRibbonPanelMenuBar__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// OnMouseMove(UINT nFlags, CPoint point): retail RVA 0x11fd20.  Calls the base
// handler (RVA 0xbf080); if the panel is set and tooltips are globally enabled
// it hit-tests through panel slot +0x188 and, when the index changed from
// +0x1154, restarts the tooltip timer 0xec07 with 500 ms.  It then bails out
// when the panel or the owned category reports a dropped-down element
// (CMFCRibbonPanel::GetDroppedDown / CMFCRibbonCategory::GetDroppedDown), then
// applies the 10-pixel mouse-move
// threshold against the last position in +0x1468/+0x146c, maintains the
// TrackMouseEvent registration behind the +0x1420 flag, and finally forwards
// to the owned category's slot +0x198 or the panel's slot +0x238, redrawing
// when the panel's +0xa8 says so.
// TODO(clean-room): needs the retail panel/category layouts and vtables.
// Symbol: ?OnMouseMove@CMFCRibbonPanelMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonPanelMenuBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// OnNeedTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult): retail RVA
// 0x120b50.  Returns FALSE unless the tooltip control at +0x12e8 exists, its
// m_hWnd matches pNMHDR->hwndFrom and (when a popup menu is active, global
// 0x1803b6fe8) this bar's parent is that popup.  It returns TRUE at once when
// GetTopLevelRibbonBar() is non-NULL and its +0x450 is 0.  Otherwise it
// HitTest()s the cursor position, returns FALSE on a miss, and returns TRUE
// without a tooltip when the element's slot +0x280 and the panel's slot +0x178
// both say so.  Then it takes the element's text through slot +0x190 into the
// global string at 0x1803bb1f8, returns FALSE if that came back empty, and --
// when the control at +0x12e8 IsKindOf the descriptor at 0x180319df0 -- fills
// it in: sizes from ribbonBar+0x41c/+0x420 into tip+0x1a0/+0x1a4, the
// description through element slot +0x198 into tip slot +0x2e8 when
// ribbonBar+0x454 is set, tip+0x138 = the element if it IsKindOf 0x1802fe9d0
// else NULL, and the screen-mapped element rect (+0xc8) into tip+0x198/+0x19c.
// Finally it stores the global string pointer into pNMHDR+0x18 (the
// TOOLTIPTEXT lpszText field) and returns TRUE.
// TODO(clean-room): needs +0x12e8, HitTest and the retail element layout.
// FALSE below is retail's "no tooltip control" terminal.
// Symbol: ?OnNeedTipText@CMFCRibbonPanelMenuBar@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCRibbonPanelMenuBar__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    void* pThis, unsigned int id, void* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)id;
    (void)pNMHDR;
    (void)pResult;
    return 0;
}

// OnSetAccData(long lVal): retail RVA 0x121650.  Unpacks lVal into a POINT
// (x = LOWORD, y = HIWORD), ScreenToClient()s it, and HitTest()s it; returns
// FALSE when nothing is hit.  Otherwise it resets this window's accessibility
// data block based at +0x150 -- zeroing 8 bytes at +0x180, 4 at +0x188, 16 at
// +0x18c and 8 at +0x19c, then running the RVA 0x3430 empty-string helper over
// +0x150, +0x158, +0x160, +0x168, +0x170, +0x150 again and +0x178 -- and
// delegates to the hit element's vtable slot +0x168 with (this, &block).
// TODO(clean-room): needs HitTest, the +0x150 accessibility block and the
// retail element vtable.  FALSE below is retail's no-hit terminal.
// Symbol: ?OnSetAccData@CMFCRibbonPanelMenuBar@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCRibbonPanelMenuBar__UEAAHJ_Z(
    void* pThis, long lVal) {
    (void)pThis;
    (void)lVal;
    return 0;
}

// OnSize(UINT nType, int cx, int cy): retail RVA 0x120ad0.  Calls the base
// handler (RVA 0x153ec0), then -- only when the tooltip control at +0x12e8
// exists and has a window -- updates that tooltip's tool rect for this bar
// with the new (0, 0, cx, cy) rectangle (RVA 0x274010, tool id from RVA
// 0x2a78b0).
// TODO(clean-room): needs +0x12e8 and the base handler.
// Symbol: ?OnSize@CMFCRibbonPanelMenuBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCRibbonPanelMenuBar__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy) {
    (void)pThis;
    (void)nType;
    (void)cx;
    (void)cy;
}

// OnTimer(UINT_PTR nIDEvent): retail RVA 0x121090.  GetCursorPos +
// ScreenToClient first, then four timer cases:
//   0xec1b -- kills the timer, pops the tooltip (+0x12e8), and unless the
//             cursor is still inside the delayed element's rect (+0x1570,
//             +0xc8) runs own slot +0x890 and opens the pending element
//             (+0x1578, slots +0x1b0 then +0x288), clearing +0x1578;
//   0xec1c -- kills the timer; returns if +0x1570 is NULL, else resolves the
//             parent window (kept only if it IsKindOf 0x1803aa448), returns if
//             the cursor is inside the delayed element's rect, otherwise calls
//             element slot +0x340, clears +0x1570 and routes the parent
//             through RVA 0xb7ee0;
//   0xec07 -- kills the timer; if the cursor is inside the client rect and
//             HitTestEx() still returns the index cached at +0x1154, packs the
//             cursor position into a LPARAM for own slot +0x4b8 and, on
//             success, NotifyWinEvent(0x8005, m_hWnd, -4, index + 1);
//   0xec1d -- if the cursor left the RECT at +0x1458 it clears +0x1588, kills
//             the timer and empties that rect; otherwise, if +0x1588 is set
//             and the cursor is inside its rect (+0xc8), it calls that
//             element's slot +0x2b0 and kills the timer when it returns 0.
// TODO(clean-room): needs the retail element/menu layouts and vtables.
// Symbol: ?OnTimer@CMFCRibbonPanelMenuBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCRibbonPanelMenuBar__IEAAX_K_Z(
    void* pThis, unsigned __int64 nIDEvent) {
    (void)pThis;
    (void)nIDEvent;
}

// OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler): retail RVA
// 0x1207e0.  Builds a stack command-UI object (vtable 0x1802fde80) whose
// fields are zeroed except for `this`, then passes it to the owned category's
// vtable slot +0x1a0 when +0x1580 is set, or to the owned panel's slot +0x200
// otherwise (retail does not null-check +0x1560 there).  It then calls the
// base update helper (RVA 0x28f8c0) with (pTarget, bDisableIfNoHndler).
// Finally, when bDisableIfNoHndler is non-zero and +0x1440 is set, and no
// button is currently dropped down, it notifies the ribbon bar (+0x1558) via
// RVA 0xe29c0, clears +0x1440 and calls the RVA 0xbb290 helper with NULL.
// TODO(clean-room): needs the retail command-UI object layout and the panel /
// category vtables.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonPanelMenuBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonPanelMenuBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* pTarget, int bDisableIfNoHndler) {
    (void)pThis;
    (void)pTarget;
    (void)bDisableIfNoHndler;
}

// OnUpdateToolTips(WPARAM wp, LPARAM lp): retail RVA 0x120e40.  Does work only
// when (wp & 0x200) is set and the tooltip flags at +0x1438/+0x1430 allow it:
// it recreates the tooltip control at +0x12e8 for this bar, sends it message
// 0x418 with 0x280, and re-registers this window's client rect as its tool.
// The function ALWAYS returns 0 -- there is a single `xor eax,eax` terminal at
// 0x120ee4 reached by both paths -- so the 0 below is retail's return value,
// even though the tooltip work itself is missing.
// Symbol: ?OnUpdateToolTips@CMFCRibbonPanelMenuBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCRibbonPanelMenuBar__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wp, __int64 lp) {
    (void)pThis;
    (void)wp;
    (void)lp;
    return 0;
}

// OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar): retail RVA
// 0x121940.  Handles the message only when the owned panel is set, the
// scroll bar is the embedded scroll bar at +0x1470 (pScrollBar->m_hWnd ==
// (+0x1470)->m_hWnd) and panel+0x168 is set: it GetScrollInfo()s that bar
// (SB_CTL, fMask 0x17), computes the new offset from
// panel+0xf8 for each SB_* code (SB_LINEUP..SB_THUMBTRACK/SB_ENDSCROLL),
// clamps it to [0, nMax - nPage], applies it (RVA 0x11d5a0), calls
// SetScrollPos(*(HWND*)(this+0x14b0), SB_CTL, pos, TRUE) and
// RedrawWindow(m_hWnd, NULL, NULL, 0x105).  Otherwise it forwards to the
// default handling under the re-entrancy guard at 0x1803bcba8.
// TODO(clean-room): needs the retail panel layout and the +0x1470 scroll bar.
// Symbol: ?OnVScroll@CMFCRibbonPanelMenuBar@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CMFCRibbonPanelMenuBar__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int nSBCode, unsigned int nPos, void* pScrollBar) {
    (void)pThis;
    (void)nSBCode;
    (void)nPos;
    (void)pScrollBar;
}

// PopTooltip(): retail RVA 0x120f00.  If the tooltip control at +0x12e8 is
// non-NULL and its m_hWnd (+0x40) is non-NULL, SendMessageW(hWndTip, 0x41c
// (TTM_POP), 0, 0).  Nothing else.
// TODO(clean-room): the only blocker is +0x12e8, which no OpenMFC constructor
// initialises -- reading it today would dereference uninitialised memory.
// Symbol: ?PopTooltip@CMFCRibbonPanelMenuBar@@QEAAXXZ
extern "C" void MS_ABI impl__PopTooltip_CMFCRibbonPanelMenuBar__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// PreTranslateMessage(MSG* pMsg): retail RVA 0x121bf0.
//   * WM_KEYDOWN (0x100) with wParam == VK_TAB (9): calls own vtable slot
//     +0x810 with 9 and returns TRUE if it was handled;
//   * WM_LBUTTONDOWN (0x201): if GetFocus()'s CWnd IsKindOf the RTTI
//     descriptor at 0x180303988, maps the cursor position into this
//     bar's client area and calls slot +0x510 on that window's +0xe8 object;
//   * everything else (and the fall-through of both cases) calls
//     CMFCToolBar::PreTranslateMessage and returns its result.  RVA 0x152fa0
//     is CMFCToolBar's, NOT CMFCPopupMenuBar's, as an earlier pass of this
//     file claimed.  The VK_TAB case is the only early return.
// TODO(clean-room): a base thunk DOES exist here --
// impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z at
// phase4/src/featurepack/toolbar/CMFCToolBar.cpp:1319 -- so the earlier "not
// reachable as an impl__ thunk" reason was wrong.  It is still not chained to:
// that thunk reads OpenMFC's CMFCToolBar members off `pThis`, and
// CMFCRibbonPanelMenuBar has no OpenMFC declaration at all (its five ctors
// initialise nothing), so any object arriving here is retail-shaped and those
// member reads would land on the wrong offsets.  FALSE below is therefore the
// stub's own value; it is NOT retail's fall-through, which is whatever the
// base returns.
// Symbol: ?PreTranslateMessage@CMFCRibbonPanelMenuBar@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCRibbonPanelMenuBar__MEAAHPEAUtagMSG___Z(
    void* pThis, void* pMsg) {
    (void)pThis;
    (void)pMsg;
    return 0;
}

// RestoreDelayedSubMenu(): retail RVA 0x121430.  Runs only when both the
// delayed sub-menu element (+0x1570) and the owned panel (+0x1560) are set.
// It clears element+0x20c, remembers the panel's current highlight
// (panel+0x3e8) and re-highlights the delayed element through panel slot
// +0x238, passing (bHighlight = 1, CPoint).  The third argument is the 8-byte
// value LOADED FROM element+0xc8 -- the rect's top-left corner packed as a
// by-value CPoint, not a pointer to the rect; the same slot takes (-1,-1) from
// OnMouseLeave.  If the highlight actually
// changed it InvalidateRect()s the old and the new element rects, then kills
// timer 0xec1c, clears +0x1570 and calls UpdateWindow(m_hWnd).
// TODO(clean-room): needs the retail panel/element layouts and vtables.
// Symbol: ?RestoreDelayedSubMenu@CMFCRibbonPanelMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__RestoreDelayedSubMenu_CMFCRibbonPanelMenuBar__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// SetActive(BOOL bActive): retail RVA 0x120f30.  CWnd::FromHandle(GetParent(
// m_hWnd)); if that object IsKindOf(CMFCRibbonPanelMenu) (descriptor
// 0x180308f60) it calls its vtable slot +0x438 with bActive.  Does nothing
// otherwise.
// TODO(clean-room): CWnd::FromHandle and CObject::IsKindOf are reachable, but
// slot +0x438 belongs to the retail CMFCRibbonPanelMenu vtable, which OpenMFC
// does not lay out -- calling it would jump into an unrelated slot.
// Symbol: ?SetActive@CMFCRibbonPanelMenuBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetActive_CMFCRibbonPanelMenuBar__QEAAXH_Z(
    void* pThis, int bActive) {
    (void)pThis;
    (void)bActive;
}

// SetPreferedSize(CSize size): retail RVA 0x11f510.  CSize arrives by value
// packed in RDX (cx in the low dword, cy in the high dword -- retail homes RDX
// and reads cy back from the home slot at 0x11f566).  It computes a minimum
// width: when the owned panel (+0x1560) is set and its gallery owner at
// panel+0x168 is non-NULL, the minimum is panel->GetPaltteMinSize().cx (RVA
// 0x11d6a0, retail's own spelling) minus GetSystemMetrics(2 /* SM_CXVSCROLL */)
// minus 2, else 0.  The call takes the PANEL as `this`; panel+0x168 is only
// the guard, not the receiver.  Then it stores
// size.cy at +0x144c and max(size.cx, minimum) at +0x1448.
// TODO(clean-room): needs +0x1448/+0x144c and the retail panel layout.
// Symbol: ?SetPreferedSize@CMFCRibbonPanelMenuBar@@IEAAXVCSize@@@Z
extern "C" void MS_ABI impl__SetPreferedSize_CMFCRibbonPanelMenuBar__IEAAXVCSize___Z(
    void* pThis, SIZE size) {
    (void)pThis;
    (void)size;
}
