// CMFCTabCtrl — OpenMFC implementation.
// Sources: global_mfc-feature-8_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// CMFCTabCtrl exports, wave "mfc-feature-8".
//
// Legacy notes below retain absolute VAs from the original mfc140u.dll
// (Unicode) disassembly; they are not RVAs. New citations name their image
// explicitly because mfc140.dll (ANSI twin) and mfc140u.dll have different
// export RVAs. Unavailable layouts and unmapped entries remain partial/stubbed.
// The class in include/openmfc/afxmfc.h is modeled as a thin CMFCBaseTabCtrl subclass with
// only 64 bytes of opaque padding, so none of the retail member offsets the
// disassembly touches (tab header rect 0x44c, style flags 0x3c0..0x3cc,
// close-button/scroll flags 0x3f4/0x3f8/0x3fc/0x400, scroll offsets 0x414/
// 0x418/0x420, drag rects 0x47c..0x494, the internal tab array at 0x108/0x110,
// the drop-target flag at 0x3d8, the MDI client-area check, ...) are named in
// the headers. Following the wave-1/2 house rules, bodies that only read such
// unmodeled state are emitted as type-correct conservative versions that keep
// the safe visible side effects (bounds checks, base handler delegation,
// capture release, scroll-bar hiding) and return the retail terminal values,
// with a `// TODO(clean-room)` marker. No member offsets are invented and no
// header files are touched.
//
// Cross-file impl helpers referenced here (defined in wincore.cpp/thunks.cpp):
//   impl__OnCommand_CWnd__MEAAH_K_J_Z
//   impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z
//   impl__OnDestroy_CWnd__IEAAXXZ
//   impl__OnHScroll_CWnd__IEAAXIIPEAVCScrollBar___Z
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include <windows.h>

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//------------------------------------------------------------------------------
// Base-class message handlers / helpers defined in sibling translation units.
//------------------------------------------------------------------------------
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(
    CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnHScroll_CWnd__IEAAXIIPEAVCScrollBar___Z(
    CWnd* pThis, unsigned int p0, unsigned int p1, CScrollBar* p2);
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" CWnd* MS_ABI impl__GetActiveWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__XZ(
    const CMFCBaseTabCtrl* pThis);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSettingChange_CWnd__IEAAXIPEB_W_Z(
    CWnd* pThis, unsigned int uFlags, const wchar_t* lpszSection);
extern "C" void MS_ABI impl__OnSysColorChange_CMFCBaseTabCtrl__IEAAXXZ(
    CMFCBaseTabCtrl* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCBaseTabCtrl__UEAAHPEAUtagMSG___Z(
    CMFCBaseTabCtrl* pThis, MSG* pMsg);
extern "C" int MS_ABI impl__SetImageList_CMFCBaseTabCtrl__UEAAHIHK_Z(
    CMFCBaseTabCtrl* pThis, unsigned int nBitmapID, int cx, unsigned long clrTransparent);
extern "C" int MS_ABI impl__SetImageList_CMFCBaseTabCtrl__UEAAHPEAU_IMAGELIST___Z(
    CMFCBaseTabCtrl* pThis, HIMAGELIST hImageList);

//=============================================================================
// CMFCTabCtrl::DrawResizeDragRect
//=============================================================================

//=============================================================================
// CMFCTabCtrl::EnableActiveTabCloseButton
//=============================================================================

//=============================================================================
// CMFCTabCtrl::EnableInPlaceEdit
//=============================================================================

//=============================================================================
// CMFCTabCtrl::EnableTabDocumentsMenu
//=============================================================================

//=============================================================================
// CMFCTabCtrl::EnsureVisible
//=============================================================================

//=============================================================================
// CMFCTabCtrl::FindTargetWnd
//=============================================================================

//=============================================================================
// CMFCTabCtrl::GetDocumentIcon (static)
//=============================================================================

//=============================================================================
// CMFCTabCtrl::GetTabFromPoint
//=============================================================================

//=============================================================================
// CMFCTabCtrl::HideActiveWindowHorzScrollBar
//=============================================================================

//=============================================================================
// CMFCTabCtrl::HideNoTabs
//=============================================================================

//=============================================================================
// CMFCTabCtrl::HideSingleTab
//=============================================================================

//=============================================================================
// CMFCTabCtrl::IsMDITabGroup
//=============================================================================

//=============================================================================
// CMFCTabCtrl::IsPtInTabArea
//=============================================================================

//=============================================================================
// CMFCTabCtrl::ModifyTabStyle
//=============================================================================

//=============================================================================
// CMFCTabCtrl::MoveTab
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnCancelMode
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnCommand
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnCreate
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnDestroy
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnDragEnter
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnDragOver
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnDraw
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnEraseBkgnd
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnHScroll
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnLButtonDblClk
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnLButtonDown
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnLButtonUp
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnMouseMove
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnNotify
//=============================================================================

//=============================================================================
// CMFCTabCtrl::OnPaint
//=============================================================================
// Retail (0x18013cd90) wraps a stack CRectTracker (ctor 0x1802a3c20 /
// dtor 0x1802a3ce0), computes a drag-grid step from the header rects
// (0x484-0x47c or 0x488-0x480, halved and incremented) and calls
// CRectTracker::DrawDragRect (0x1802a5720) for the two rectangles. Requires
// the header-rect geometry members and the CRectTracker type, neither of
// which is modeled.
// Symbol: ?DrawResizeDragRect@CMFCTabCtrl@@MEAAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__DrawResizeDragRect_CMFCTabCtrl__MEAAXAEAVCRect__0_Z(
    CMFCTabCtrl* pThis, CRect& rectNew, CRect& rectLast)
{
    (void)rectNew; (void)rectLast;
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- retail draws through a stack
    // CRectTracker whose step size comes from the header-rect members at
    // 0x47c/0x484/0x480/0x488; neither the tracker nor the rects are modeled.
}
// Retail (0x18013c8e0) stores the flag at +0x3fc, then triggers a relayout/
// redraw (vslot 0x318) unconditionally and a second redraw (vslot 0x458) when
// the active tab index (+0x154) is valid. The flag member is not declared in
// OpenMFC; the redraw side effect is kept.
// Symbol: ?EnableActiveTabCloseButton@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableActiveTabCloseButton_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bEnable)
{
    (void)bEnable;
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the m_bActiveTabCloseButton
    // flag (retail +0x3fc) has no declared member, so it is not stored.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x18013c140) is a pure flag write: `if (style flag +0x3c0 != 0)
// { flag +0x230 = bEnable; }`. Neither offset is a declared member.
// Symbol: ?EnableInPlaceEdit@CMFCTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableInPlaceEdit_CMFCTabCtrl__UEAAXH_Z(
    CMFCTabCtrl* pThis, int bEnable)
{
    (void)bEnable;
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- retail stores bEnable at
    // +0x230 when the +0x3c0 style flag is set; neither member is declared.
}
// Retail (0x18013c7b0) stores the flag at +0x400, walks a CWinAppEx menu
// singleton (0x1803b25e8), loads menu resource 0x3ea3/0x3f43, updates the
// CMFCToolBarMenuButton at +0x10a8, calls the internal relayout helper
// 0x18013c400 and resets the scroll offsets (+0x414/+0x420). None of these
// objects are modeled.
// Symbol: ?EnableTabDocumentsMenu@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTabDocumentsMenu_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bEnable)
{
    (void)bEnable;
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the menu-manager singleton,
    // the documents-menu button at +0x10a8 and the +0x400 flag member.
}
// Retail (0x18013b730): bounds-check `0 <= iTab < tabs count (+0x150)`; when
// out of bounds return FALSE (0x18013b944: xor eax,eax). Otherwise scroll the
// tab into view and return TRUE. The scroll geometry (header rect +0x44c,
// tab array +0x108/+0x110, scroll offset +0x414) is unmodeled, so only the
// bounds gate and the terminal value are kept.
// Symbol: ?EnsureVisible@CMFCTabCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__EnsureVisible_CMFCTabCtrl__UEAAHH_Z(
    CMFCTabCtrl* pThis, int iTab)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- retail scrolls the tab strip
    // (header rect +0x44c, tab rect array +0x108, scroll offset +0x414) so
    // that iTab becomes visible; that geometry is not modeled.
    return TRUE;
}
// Retail (0x18013a270): if point.y < tab-header bottom (+0x1d8) or the point
// lies over a non-detachable tab (per-tab flag at +0x48) it returns NULL;
// otherwise it returns CWnd::FromHandlePermanent(m_hWnd) i.e. `this`. The
// header bound and per-tab geometry are unmodeled; for a live control the
// FromHandlePermanent answer is the object itself.
// Symbol: ?FindTargetWnd@CMFCTabCtrl@@MEAAPEAVCWnd@@AEBVCPoint@@@Z
extern "C" CWnd* MS_ABI impl__FindTargetWnd_CMFCTabCtrl__MEAAPEAVCWnd__AEBVCPoint___Z(
    CMFCTabCtrl* pThis, const CPoint& point)
{
    (void)point;
    if (!pThis) return nullptr;
    // TODO(clean-room): partially transcribed -- retail returns NULL when the
    // point is above the tab-header bottom (+0x1d8) or over a tab whose
    // +0x48 flag is set; the header bound and tab rects are not modeled.
    return pThis;
}
// Retail RVA 0x13cc80 (mfc140u) looks up the command ID in the static
// document-icon map and returns the HICON at +0x8 of a matching entry, or NULL.
// The map's CMap storage is not represented in OpenMFC.
// Symbol: ?GetDocumentIcon@CMFCTabCtrl@@SAPEAUHICON__@@I@Z
extern "C" HICON MS_ABI impl__GetDocumentIcon_CMFCTabCtrl__SAPEAUHICON____I_Z(
    unsigned int /*nCmdID*/)
{
    // STUB: the retail map backing this lookup is not implemented.
    return nullptr;
}
// Retail (0x18013c510): returns -1 when the point is outside the tab-area
// rect (+0x44c), otherwise it scans the tab rect array (+0x108/+0x110, active
// tab +0x154, style flags +0x3c4/+0x3c8/+0x3cc) and returns the index of the
// first tab whose rect contains the point, or -1. The geometry is unmodeled,
// so the conservative "no tab found" value is returned.
// Symbol: ?GetTabFromPoint@CMFCTabCtrl@@UEBAHAEAVCPoint@@@Z
extern "C" int MS_ABI impl__GetTabFromPoint_CMFCTabCtrl__UEBAHAEAVCPoint___Z(
    const CMFCTabCtrl* pThis, CPoint& point)
{
    (void)point;
    if (!pThis) return -1;
    // TODO(clean-room): not transcribed -- needs the tab-area rect (+0x44c)
    // and the per-tab rect array (+0x108/+0x110); not modeled.
    return -1;
}
// Retail (0x18013b5f0): resolves the active tab window (vslot 0x430) and, when
// the flag at +0x3d0 is set, calls ShowScrollBar(hwnd, SB_HORZ, FALSE) and
// resets the scroll range/page via SetScrollInfo. The +0x3d0 flag is
// unmodeled; the scroll-bar hiding for the active tab is kept.
// Symbol: ?HideActiveWindowHorzScrollBar@CMFCTabCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__HideActiveWindowHorzScrollBar_CMFCTabCtrl__QEAAXXZ(
    CMFCTabCtrl* pThis)
{
    if (!pThis) return;
    CWnd* pActive = pThis->GetTabWnd(pThis->GetActiveTab());
    if (!pActive) return;
    HWND hWnd = pActive->GetSafeHwnd();
    if (!hWnd) return;
    // TODO(clean-room): partially transcribed -- retail gates on the flag at
    // +0x3d0 (not declared) and afterwards also resets the scroll info
    // (0x1802a96f0) so the bar stays hidden; only ShowScrollBar is kept.
    ::ShowScrollBar(hWnd, SB_HORZ, FALSE);
}
// Retail (0x18013ba30): early-returns when the flag at +0x3f4 already equals
// bHide; otherwise stores it and, when m_hWnd is valid, triggers a relayout/
// redraw (vslot 0x318) plus the internal helper 0x18013b4d0(this, 0). The
// flag member is unmodeled; the redraw side effect is kept.
// Symbol: ?HideNoTabs@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__HideNoTabs_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bHide)
{
    (void)bHide;
    if (!pThis) return;
    if (!pThis->GetSafeHwnd()) return;
    // TODO(clean-room): partially transcribed -- the m_bHideNoTabs flag
    // (retail +0x3f4) has no declared member, so the change-detection and
    // storage are skipped.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x18013c190): early-returns when the flag at +0x1f4 already equals
// bHide; otherwise stores it and, when m_hWnd is valid, triggers a relayout/
// redraw (vslot 0x318) plus the internal helper 0x18013b4d0(this, 0). The
// flag member is unmodeled; the redraw side effect is kept.
// Symbol: ?HideSingleTab@CMFCTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__HideSingleTab_CMFCTabCtrl__UEAAXH_Z(
    CMFCTabCtrl* pThis, int bHide)
{
    (void)bHide;
    if (!pThis) return;
    if (!pThis->GetSafeHwnd()) return;
    // TODO(clean-room): partially transcribed -- the m_bHideSingleTab flag
    // (retail +0x1f4) has no declared member, so the change-detection and
    // storage are skipped.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x18013ce50) resolves the parent window (FromHandlePermanent of
// GetParent(m_hWnd)) and returns whether it IsKindOf(CMDIClientAreaWnd)
// (runtime class at 0x1802ed340, name string "CMDIClientAreaWnd" at
// 0x18033fad0). CMDIClientAreaWnd is not declared in the OpenMFC headers.
// Symbol: ?IsMDITabGroup@CMFCTabCtrl@@UEBAHXZ
extern "C" int MS_ABI impl__IsMDITabGroup_CMFCTabCtrl__UEBAHXZ(
    const CMFCTabCtrl* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- retail returns TRUE when the
    // parent window is a CMDIClientAreaWnd; that class (and its runtime
    // class descriptor) is not declared in OpenMFC, so this always returns
    // FALSE.
    return FALSE;
}
// Retail (0x18013c130) is a single tail-call: `CRect* (+0x44c).PtInRect(point)`.
// The tab-area rect member (+0x44c) is not declared.
// Symbol: ?IsPtInTabArea@CMFCTabCtrl@@UEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__IsPtInTabArea_CMFCTabCtrl__UEBAHVCPoint___Z(
    const CMFCTabCtrl* pThis, CPoint point)
{
    (void)point;
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- retail hit-tests the tab-area
    // rect at +0x44c; that member is not declared, so no point can match.
    return FALSE;
}
// Retail (0x18013c380) derives the four style flags from `style`:
//   +0x3c0 = (style == 1); +0x3c4 = +0xf0 = (style == 4);
//   +0x3c8 = (style == 5); +0x3cc = (6 <= style <= 7);
// then relayouts (internal 0x18013c400 + vslot 0x300) and always returns 1.
// The flags are unmodeled; the constant return value is kept.
// Symbol: ?ModifyTabStyle@CMFCTabCtrl@@QEAAHW4Style@1@@Z
extern "C" int MS_ABI impl__ModifyTabStyle_CMFCTabCtrl__QEAAHW4Style_1__Z(
    CMFCTabCtrl* pThis, int /*style*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- the four style flags at
    // +0x3c0/+0x3c4/+0xf0/+0x3c8/+0x3cc are not declared, so the derived
    // style state is not stored and no relayout is issued.
    return TRUE;
}
// Retail (0x18013c750): scrolls the strip (0x180018ec0), and when any of the
// style flags (+0x3c4/+0x3c8/+0x3cc) is set it moves the tab in the internal
// array (+0x108/+0x110), resets the scroll offsets (+0x414/+0x420) and
// triggers a redraw (vslot 0x458). The tab array and flags are unmodeled.
// Symbol: ?MoveTab@CMFCTabCtrl@@UEAAXHH@Z
extern "C" void MS_ABI impl__MoveTab_CMFCTabCtrl__UEAAXHH_Z(
    CMFCTabCtrl* pThis, int /*i1*/, int /*i2*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the internal tab array
    // (+0x108/+0x110) and the style flags; not modeled.
}
// Retail (0x18013b390): when this control is the capture owner and its
// vslot-0x5f0 drag query is active it posts a registered message to the
// parent window (0x1803c24c0), then runs internal resets and, if the
// +0x3dc flag is set, releases the mouse capture. The conservative version
// keeps the capture release.
// Symbol: ?OnCancelMode@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the registered cancel
    // message to the parent and the internal drag-state resets (+0x3dc/+0x40c)
    // are not modeled.
    if (::GetCapture() == pThis->GetSafeHwnd()) {
        ::ReleaseCapture();
    }
}
// Retail (0x18013ba70) first handles commands from its internal scrollbar
// (+0x588) and documents-menu button (+0x10a8) by recomputing the scroll
// offset (+0x414/+0x420) from the tab geometry, and otherwise returns the
// result of an internal helper (0x18028e040). Those internals are unmodeled;
// the base CWnd::OnCommand routing is the closest declared equivalent and is
// non-destructive for unrelated commands.
// Symbol: ?OnCommand@CMFCTabCtrl@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCTabCtrl__MEAAH_K_J_Z(
    CMFCTabCtrl* pThis, unsigned long long wParam, long long lParam)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- retail intercepts commands
    // from the internal scroll bar (+0x588) / documents button (+0x10a8) and
    // recomputes the strip scroll offset; those windows are not modeled, so
    // everything is routed to the base handler.
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(pThis,
        static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
}
// Retail (0x180139790) calls the base OnCreate first (-1 aborts), then creates
// the internal scroll bar (class string at 0x18033d19c) and the documents
// button, wires up the +0x3d30 handler list and returns 0. The child window
// objects (scroll bar +0x588, button +0x1bc8) are not modeled.
// Symbol: ?OnCreate@CMFCTabCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCTabCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCTabCtrl* pThis, CREATESTRUCTW* /*lpCreateStruct*/)
{
    if (!pThis) return -1;
    // TODO(clean-room): partially transcribed -- the internal scroll bar and
    // documents-menu child windows are not modeled, so no children are
    // created; 0 keeps the window alive (retail returns 0 on success).
    return 0;
}
// Retail (0x180137230) destroys the tooltip helper (+0x210), the handler-list
// objects (+0x3d30, +0x1b8, +0x1c0) and tail-calls CWnd::OnDestroy
// (0x1800182e0). The helper objects are unmodeled; the base call is kept.
// Symbol: ?OnDestroy@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the tooltip and handler-list
    // sub-objects at +0x210/+0x3d30/+0x1b8/+0x1c0 are not modeled.
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}
// Retail (0x18009da20) is a single tail-call to the owner drop-target virtual
// (vslot 0x580) which forwards to the drag logic shared with OnDragOver. The
// drop machinery is unmodeled; DROPEFFECT_NONE is the safe default.
// Symbol: ?OnDragEnter@CMFCTabCtrl@@UEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDragEnter_CMFCTabCtrl__UEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCTabCtrl* pThis, COleDataObject* /*pDataObject*/, unsigned long /*dwKeyState*/, CPoint /*point*/)
{
    if (!pThis) return 0;
    // TODO(clean-room): partially transcribed -- retail forwards to the
    // vslot-0x580 drop-target virtual (the shared drag-enter/over logic in
    // CMFCBaseTabCtrl); that virtual is not modeled.
    return 0;   // DROPEFFECT_NONE
}
// Retail (0x1800071e0) is literally `xor eax,eax; ret` -- it always returns
// DROPEFFECT_NONE. Transcribed exactly.
// Symbol: ?OnDragOver@CMFCTabCtrl@@UEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDragOver_CMFCTabCtrl__UEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCTabCtrl* pThis, COleDataObject* /*pDataObject*/, unsigned long /*dwKeyState*/, CPoint /*point*/)
{
    if (!pThis) return 0;
    return 0;   // DROPEFFECT_NONE (retail: xor eax,eax; ret)
}
// Retail (0x180137360) is the full tab-strip painter: it queries the global
// visual data (0x180009774), computes the tab rectangles through the drawing
// manager, draws the header/close buttons/scrollbar per the style flags and
// finishes with the active tab. Requires AFX_GLOBAL_DATA, CDrawingManager and
// the whole header geometry -- none of which is modeled.
// Symbol: ?OnDraw@CMFCTabCtrl@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCTabCtrl__UEAAXPEAVCDC___Z(
    CMFCTabCtrl* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- the tab-strip painter depends on
    // AFX_GLOBAL_DATA, CDrawingManager and the header-rect members.
}
// Retail (0x180139df0) returns TRUE immediately when either the +0x3e8 flag or
// the vslot-0x358 printing query is set; otherwise it fills the client rect
// with the global background brush (0x1803c16d0) and returns TRUE. The brush
// and flag are unmodeled; the terminal TRUE is kept.
// Symbol: ?OnEraseBkgnd@CMFCTabCtrl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCTabCtrl__IEAAHPEAVCDC___Z(
    CMFCTabCtrl* pThis, CDC* /*pDC*/)
{
    if (!pThis) return TRUE;
    // TODO(clean-room): partially transcribed -- the background fill uses the
    // global brush at 0x1803c16d0 and the +0x3e8 flag; neither is modeled.
    return TRUE;
}
// Retail (0x18013a0f0): when the scrollbar style flag (+0x3c0) is clear it
// forwards to the base CWnd::OnHScroll (0x18028ac80); when set it recomputes
// the strip scroll offset from the active tab geometry. The style flag is
// unmodeled, so the base handler is the faithful conservative path.
// Symbol: ?OnHScroll@CMFCTabCtrl@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CMFCTabCtrl__IEAAXIIPEAVCScrollBar___Z(
    CMFCTabCtrl* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- retail's custom branch (style
    // flag +0x3c0 set) scrolls the tab strip via the active-tab geometry;
    // only the base-handler branch is kept.
    impl__OnHScroll_CWnd__IEAAXIIPEAVCScrollBar___Z(pThis, nSBCode, nPos, pScrollBar);
}
// Retail (0x180139650) walks the documents-menu list (+0x3d38), hit-tests the
// close-button rects and the tabs, sends WM_LBUTTONDBLCLK/WM_LBUTTONDOWN to
// the child tab window and, for a double click on a tab, activates it. Needs
// the tab geometry and the internal lists; not modeled.
// Symbol: ?OnLButtonDblClk@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the tab-rect geometry, the
    // documents-menu list (+0x3d38) and the vslot-0x4a8 hit test.
}
// Retail (0x180139430): when the point is inside the strip rect (+0x42c) it
// sets the +0x3dc capture flag and calls SetCapture, then handles the
// resize-drag path (rects +0x46c/+0x48c, style +0x3d28) and finally activates
// the clicked tab. The rect members and capture flag are unmodeled.
// Symbol: ?OnLButtonDown@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the strip-rect (+0x42c) and
    // drag-rect (+0x46c/+0x48c) members and the +0x3dc capture flag.
}
// Retail (0x18013ad40) resets the capture flags (+0x3dc/+0x40c) and releases
// the mouse capture when a capture or drag is active, then runs the
// drag-cancel / drop logic. The flags are unmodeled; the capture release is
// the visible side effect that is kept.
// Symbol: ?OnLButtonUp@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the drag-state flags
    // (+0x3dc/+0x40c) and the internal drop/cancel logic (0x18013aea0) are
    // not modeled.
    if (::GetCapture() == pThis->GetSafeHwnd()) {
        ::ReleaseCapture();
    }
}
// Retail (0x18013afd0) only acts while a drag is active (+0x40c): it clamps
// the drag rectangle (+0x47c..+0x494) against the strip bounds and redraws
// the resize drag rect according to the +0x3d28 style. All those members are
// unmodeled.
// Symbol: ?OnMouseMove@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x40c drag flag, the
    // drag rects (+0x47c..+0x494) and the +0x3d28 style.
}
// Retail (0x18013b970) calls an internal helper (0x18028e150) first, then
// special-cases the tooltip notifications (code 0xfffffdf7, windows +0x1b8/
// +0x1c0) and the in-place edit close (code 0xfffffebf, internal relayout
// 0x18013b4d0) and returns the helper's result. Those helpers and windows are
// unmodeled; the base CWnd::OnNotify routing is the closest declared
// equivalent and non-destructive.
// Symbol: ?OnNotify@CMFCTabCtrl@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CMFCTabCtrl__MEAAH_K_JPEA_J_Z(
    CMFCTabCtrl* pThis, unsigned long long wParam, long long lParam, long long* pResult)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- the tooltip (+0x1b8/+0x1c0)
    // and in-place-edit notifications are not modeled, so everything is
    // routed to the base handler.
    return impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(pThis,
        static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam),
        reinterpret_cast<LRESULT*>(pResult));
}
// Retail (0x180137280) constructs a CPaintDC, draws the strip via OnDraw
// (vslot 0x5c0) and lets the paint DC end the paint. The OnDraw virtual is
// unmodeled, so the conservative version keeps only the BeginPaint/EndPaint
// bracket that validates the update region (preventing repaint loops).
// Symbol: ?OnPaint@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    if (!pThis) return;
    HWND hWnd = pThis->GetSafeHwnd();
    if (!hWnd) return;
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(hWnd, &ps);
    (void)hdc;
    // TODO(clean-room): partially transcribed -- retail calls the OnDraw
    // virtual (vslot 0x5c0) to paint the tabs; not modeled.
    ::EndPaint(hWnd, &ps);
}
CMFCTabCtrl::CMFCTabCtrl() {
    memset(_mfctabctrl_padding, 0, sizeof(_mfctabctrl_padding));
}
CMFCTabCtrl::~CMFCTabCtrl() {}
void CMFCTabCtrl::EnableTabSwap(BOOL bEnable) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabSwapEnabled = bEnable != FALSE;
}
void CMFCTabCtrl::SetTabBorderSize(int nTabBorderSize) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabBorderSize = nTabBorderSize;
}
void CMFCTabCtrl::SetTabsHeight(int nTabHeight) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabsHeight = nTabHeight;
}
// Stub placeholder for the static CMap<UINT, UINT, HICON, HICON> export. No
// retail-compatible map storage is available in this implementation.
// Symbol: ?m_mapDocIcons@CMFCTabCtrl@@1V?$CMap@IIPEAUHICON__@@PEAU1@@@A
extern "C" void* MS_ABI impl__m_mapDocIcons_CMFCTabCtrl__1V__CMap_IIPEAUHICON____PEAU1___A = nullptr;

// STUB: Retail RVA 0x13db10 (mfc140 ANSI twin) resolves the selected tab's
// MDI/window state and dispatches activation; the required state is unmodeled.
// Symbol: ?ActivateMDITab@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__ActivateMDITab_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int iTab)
{
    (void)pThis; (void)iTab;
}

// STUB: Retail RVA 0x1394a0 (mfc140 ANSI twin) lays out per-tab rectangles,
// buttons, and the strip using state not represented by the opaque class padding.
// Symbol: ?AdjustTabs@CMFCTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustTabs_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// STUB: Retail RVA 0x13af90 (mfc140 ANSI twin) updates the strip scroll
// offsets and scrollbar state using unmodeled geometry and flags.
// Symbol: ?AdjustTabsScroll@CMFCTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustTabsScroll_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// STUB: Retail RVA 0x13b750 (mfc140 ANSI twin) adjusts the active child
// window's scroll/placement from unmodeled tab-control geometry.
// Symbol: ?AdjustWndScroll@CMFCTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustWndScroll_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// STUB: Retail RVA 0x137d40 (mfc140 ANSI twin) writes style/location state
// and creates the internal scroll bar; those fields and child objects are absent.
// Symbol: ?Create@CMFCTabCtrl@@QEAAHW4Style@1@AEBUtagRECT@@PEAVCWnd@@IW4Location@CMFCBaseTabCtrl@@H@Z
extern "C" int MS_ABI impl__Create_CMFCTabCtrl__QEAAHW4Style_1_AEBUtagRECT__PEAVCWnd__IW4Location_CMFCBaseTabCtrl__H_Z(
    CMFCTabCtrl* pThis, int style, const RECT* pRect, CWnd* pParentWnd,
    unsigned int nID, int location, int bCloseBtn)
{
    (void)pThis; (void)style; (void)pRect; (void)pParentWnd;
    (void)nID; (void)location; (void)bCloseBtn;
    return FALSE;
}

// STUB: Retail RVA 0x139f70 (mfc140 ANSI twin) checks style/rectangle
// conditions and invokes global drawing services; the geometry and renderer are unmodeled.
// Symbol: ?Draw3DTab@CMFCTabCtrl@@MEAAXPEAVCDC@@PEAVCMFCTabInfo@@H@Z
extern "C" void MS_ABI impl__Draw3DTab_CMFCTabCtrl__MEAAXPEAVCDC__PEAVCMFCTabInfo__H_Z(
    CMFCTabCtrl* pThis, CDC* pDC, void* pTabInfo, int nTab)
{
    (void)pThis; (void)pDC; (void)pTabInfo; (void)nTab;
}

// STUB: Retail RVA 0x13a050 (mfc140 ANSI twin) draws through global visual
// data and tab geometry that are not represented in OpenMFC.
// Symbol: ?DrawFlatTab@CMFCTabCtrl@@MEAAXPEAVCDC@@PEAVCMFCTabInfo@@H@Z
extern "C" void MS_ABI impl__DrawFlatTab_CMFCTabCtrl__MEAAXPEAVCDC__PEAVCMFCTabInfo__H_Z(
    CMFCTabCtrl* pThis, CDC* pDC, void* pTabInfo, int nTab)
{
    (void)pThis; (void)pDC; (void)pTabInfo; (void)nTab;
}

// Retail RVA 0x13b890 (mfc140 ANSI twin) hit-tests the strip and resize
// rectangles and selects cursors from global cursor data. Those rectangles
// are unmodeled, so use the CWnd default path as a conservative fallback.
// Symbol: ?OnSetCursor@CMFCTabCtrl@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCTabCtrl__IEAAHPEAVCWnd__II_Z(
    CMFCTabCtrl* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message)
{
    (void)pWnd; (void)nHitTest; (void)message;
    if (!pThis) return FALSE;
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(
        static_cast<CWnd*>(pThis)));
}

// Retail RVA 0x13cbe0 (mfc140 ANSI twin) calls CWnd::OnSetFocus (entry RVA
// 0x28d720), obtains the active tab window through vtable byte offset 0x430
// (slot 0x86), then calls CWnd::SetFocus (entry RVA 0x2a7a70). The active
// window is obtained with the existing base thunk rather than a virtual-slot
// dispatch; NULL this is defensively ignored.
// Symbol: ?OnSetFocus@CMFCTabCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCTabCtrl__IEAAXPEAVCWnd___Z(
    CMFCTabCtrl* pThis, CWnd* pOldWnd)
{
    if (!pThis) return;
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(static_cast<CWnd*>(pThis), pOldWnd);
    CWnd* pActive = impl__GetActiveWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__XZ(
        static_cast<const CMFCBaseTabCtrl*>(pThis));
    if (pActive) {
        (void)impl__SetFocus_CWnd__QEAAPEAV1_XZ(pActive);
    }
}

// Retail mfc140 ANSI-twin entry RVA 0x13c360 unconditionally calls
// CWnd::OnSettingChange before testing the tab style flag at +0x3c0 and doing
// tab-layout/scroll work. The exact wide export has no mapped RVA; the base
// handler call is established from the ANSI twin, but +0x3c0 is unmodeled.
// Symbol: ?OnSettingChange@CMFCTabCtrl@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCTabCtrl__IEAAXIPEB_W_Z(
    CMFCTabCtrl* pThis, unsigned int uFlags, const wchar_t* lpszSection)
{
    if (!pThis) return;
    impl__OnSettingChange_CWnd__IEAAXIPEB_W_Z(
        static_cast<CWnd*>(pThis), uFlags, lpszSection);
    // TODO(clean-room): the ANSI-twin body then gates layout and scrollbar
    // updates on +0x3c0; that member and the related geometry are unmodeled.
}

// STUB: Retail RVA 0x13d5b0 (mfc140 ANSI twin) builds and displays a document
// menu from global menu-manager and per-tab/list state that is not modeled.
// Symbol: ?OnShowTabDocumentsMenu@CMFCTabCtrl@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnShowTabDocumentsMenu_CMFCTabCtrl__UEAAXVCPoint___Z(
    CMFCTabCtrl* pThis, long long point)
{
    (void)pThis; (void)point;
}

// Retail RVA 0x138d60 (mfc140 ANSI twin) first calls CWnd::Default, then
// computes button/tab sizing, conditionally clears scroll offsets, and
// synchronizes scrollbars using unmodeled fields. Keep the known default call.
// Symbol: ?OnSize@CMFCTabCtrl@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCTabCtrl__IEAAXIHH_Z(
    CMFCTabCtrl* pThis, unsigned int nType, int cx, int cy)
{
    (void)nType; (void)cx; (void)cy;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    // TODO(clean-room): the subsequent sizing, field updates and scrollbar
    // synchronization require the retail geometry members, which are absent.
}

// Retail RVA 0x13c0c0 (mfc140 ANSI twin) calls the base tab handler and then
// conditionally rebuilds tab color/image state. Keep the known base behavior.
// Symbol: ?OnSysColorChange@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    if (!pThis) return;
    impl__OnSysColorChange_CMFCBaseTabCtrl__IEAAXXZ(
        static_cast<CMFCBaseTabCtrl*>(pThis));
    // TODO(clean-room): the remaining branch uses tab-style and image-list
    // state, which are not declared.
}

// STUB: Retail RVA 0x13dba0 (mfc140 ANSI twin) rebuilds tooltip entries only
// when wParam has bit 0x4 set, using tooltip helpers and per-tab labels/IDs;
// those structures are not modeled. It returns zero on both paths.
// Symbol: ?OnUpdateToolTips@CMFCTabCtrl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCTabCtrl__IEAA_J_K_J_Z(
    CMFCTabCtrl* pThis, unsigned __int64 wParam, __int64 lParam)
{
    (void)pThis; (void)wParam; (void)lParam;
    return 0;
}

// Retail RVA 0x13d980 (mfc140 ANSI twin) calls CWnd::Default, sets a layout
// flag, then may set WINDOWPOS flags after a drag-query virtual. Keep the
// unconditional default call; the layout/virtual state is unmodeled.
// Symbol: ?OnWindowPosChanged@CMFCTabCtrl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCTabCtrl__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCTabCtrl* pThis, WINDOWPOS* pWndPos)
{
    (void)pWndPos;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    // TODO(clean-room): the dirty flag and drag-query virtual branch cannot
    // be represented by the current class layout.
}

// Retail RVA 0x13d9d0 (mfc140 ANSI twin) calls CWnd::Default and may set
// WINDOWPOS flags based on a drag-query virtual. Keep the known default call.
// Symbol: ?OnWindowPosChanging@CMFCTabCtrl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CMFCTabCtrl__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCTabCtrl* pThis, WINDOWPOS* pWndPos)
{
    (void)pWndPos;
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    // TODO(clean-room): the drag-query virtual and WINDOWPOS update are not modeled.
}

// Retail RVA 0x13ab20 (mfc140 ANSI twin) handles keyboard tab scrolling and
// tooltip messages before falling back to the base-tab handler. The custom
// state is unavailable, so preserve the known base message processing.
// Symbol: ?PreTranslateMessage@CMFCTabCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCTabCtrl__UEAAHPEAUtagMSG___Z(
    CMFCTabCtrl* pThis, MSG* pMsg)
{
    if (!pThis) return FALSE;
    return impl__PreTranslateMessage_CMFCBaseTabCtrl__UEAAHPEAUtagMSG___Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), pMsg);
}

// STUB: Retail RVA 0x13b080 (mfc140 ANSI twin) performs a substantial tab,
// button, and scroll layout using state not represented by the class declaration.
// Symbol: ?RecalcLayout@CMFCTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCTabCtrl__UEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// STUB: Retail RVA 0x13cc20 (mfc140 ANSI twin) repositions internal buttons
// from two by-value geometry aggregates and internal control members.
// Symbol: ?ReposButtons@CMFCTabCtrl@@IEAAXVCPoint@@VCSize@@HH@Z
extern "C" void MS_ABI impl__ReposButtons_CMFCTabCtrl__IEAAXVCPoint__VCSize__HH_Z(
    CMFCTabCtrl* pThis, long long point, long long size, int bHide, int nButtonMargin)
{
    (void)pThis; (void)point; (void)size; (void)bHide; (void)nButtonMargin;
}

// STUB: Retail RVA 0x138eb0 (mfc140 ANSI twin) validates and activates tabs,
// sends notifications, and updates child-window/scroll state. The tab state is
// kept in a side table, but no callable impl__ setter is available in this file.
// Symbol: ?SetActiveTab@CMFCTabCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__SetActiveTab_CMFCTabCtrl__UEAAHH_Z(
    CMFCTabCtrl* pThis, int iTab)
{
    (void)pThis; (void)iTab;
    return FALSE;
}

// STUB: Retail RVA 0x13cdd0 (mfc140 ANSI twin) stores the bold-font flag at
// +0x3f8 and, when the HWND exists, invokes relayout (vslot 0x318) followed by
// scroll synchronization. Those effects are not modeled.
// Symbol: ?SetActiveTabBoldFont@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetActiveTabBoldFont_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bEnable)
{
    (void)pThis; (void)bEnable;
}

// STUB: Retail RVA 0x13d180 (mfc140 ANSI twin) only stores its argument at
// +0x3f0, a field absent from the declared CMFCTabCtrl layout.
// Symbol: ?SetDrawFrame@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDrawFrame_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bDrawFrame)
{
    (void)pThis; (void)bDrawFrame;
}

// Retail RVA 0x13aa50 (mfc140 ANSI twin) tail-jumps to the base resource
// overload at RVA 0x19b10 (mfc140 ANSI twin). Forward through its existing
// thunk; that base implementation still lacks embedded CImageList state. The
// NULL-this guard is defensive and differs from retail's unconditional tail jump.
// Symbol: ?SetImageList@CMFCTabCtrl@@UEAAHIHK@Z
extern "C" int MS_ABI impl__SetImageList_CMFCTabCtrl__UEAAHIHK_Z(
    CMFCTabCtrl* pThis, unsigned int nBitmapID, int cx, unsigned long clrTransparent)
{
    if (!pThis) return FALSE;
    return impl__SetImageList_CMFCBaseTabCtrl__UEAAHIHK_Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), nBitmapID, cx, clrTransparent);
}

// Retail RVA 0x13aa60 (mfc140 ANSI twin) tail-jumps to the base handle
// overload at RVA 0x19c80 (mfc140 ANSI twin). Forward through its existing
// thunk; the base implementation does not retain the handle in its current model.
// The NULL-this guard is defensive and differs from retail's unconditional tail jump.
// Symbol: ?SetImageList@CMFCTabCtrl@@UEAAHPEAU_IMAGELIST@@@Z
extern "C" int MS_ABI impl__SetImageList_CMFCTabCtrl__UEAAHPEAU_IMAGELIST___Z(
    CMFCTabCtrl* pThis, HIMAGELIST hImageList)
{
    if (!pThis) return FALSE;
    return impl__SetImageList_CMFCBaseTabCtrl__UEAAHPEAU_IMAGELIST___Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), hImageList);
}

// STUB: Retail RVA 0x13d960 (mfc140 ANSI twin) stores resize mode at +0x3d28
// and relayouts through vtable byte offset 0x318 (slot 99); neither is modeled.
// Symbol: ?SetResizeMode@CMFCTabCtrl@@QEAAXW4ResizeMode@1@@Z
extern "C" void MS_ABI impl__SetResizeMode_CMFCTabCtrl__QEAAXW4ResizeMode_1__Z(
    CMFCTabCtrl* pThis, int resizeMode)
{
    (void)pThis; (void)resizeMode;
}

// STUB: Retail RVA 0x13d080 (mfc140 ANSI twin) configures the internal scroll
// and documents buttons from style flags and child-control objects not modeled.
// Symbol: ?SetScrollButtons@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__SetScrollButtons_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// STUB: Retail RVA 0x13d940 (mfc140 ANSI twin) stores the maximum width at
// +0x428 and invokes relayout; neither the field nor layout is modeled.
// Symbol: ?SetTabMaxWidth@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetTabMaxWidth_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int nMaxWidth)
{
    (void)pThis; (void)nMaxWidth;
}

// STUB: Retail RVA 0x13bb20 (mfc140 ANSI twin) exits resize-drag state,
// redraws rectangles, and may send a message using unmodeled drag/window state.
// Symbol: ?StopResize@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__StopResize_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bCancel)
{
    (void)pThis; (void)bCancel;
}

// STUB: Retail RVA 0x13d390 (mfc140 ANSI twin) calls the base tab swap and
// conditionally resets scroll offsets at +0x414/+0x420, which are unmodeled.
// Symbol: ?SwapTabs@CMFCTabCtrl@@UEAAXHH@Z
extern "C" void MS_ABI impl__SwapTabs_CMFCTabCtrl__UEAAXHH_Z(
    CMFCTabCtrl* pThis, int iTab1, int iTab2)
{
    (void)pThis; (void)iTab1; (void)iTab2;
}

// STUB: Retail RVA 0x13c150 (mfc140 ANSI twin) uses MDI scroll state, the
// active window, and a scrollbar handle to synchronize position; those fields are absent.
// Symbol: ?SynchronizeScrollBar@CMFCTabCtrl@@QEAAHPEAUtagSCROLLINFO@@@Z
extern "C" int MS_ABI impl__SynchronizeScrollBar_CMFCTabCtrl__QEAAHPEAUtagSCROLLINFO___Z(
    CMFCTabCtrl* pThis, SCROLLINFO* pScrollInfo)
{
    (void)pThis; (void)pScrollInfo;
    return FALSE;
}

// STUB: Retail RVA 0x13ce50 (mfc140 ANSI twin) updates the internal scroll
// and documents buttons from tab/style state and child controls not modeled.
// Symbol: ?UpdateScrollButtonsState@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateScrollButtonsState_CMFCTabCtrl__IEAAXXZ(
    CMFCTabCtrl* pThis)
{
    (void)pThis;
}
