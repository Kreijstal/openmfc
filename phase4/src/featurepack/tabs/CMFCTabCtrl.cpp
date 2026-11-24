// CMFCTabCtrl — OpenMFC implementation.
// Sources: global_mfc-feature-8_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"
#include <windowsx.h>
#include "detail/Mfc07MsgmapSupport.h"

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
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include <windows.h>

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstdint>
#include <limits>

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
extern "C" int MS_ABI impl__GetTabFromPoint_CMFCBaseTabCtrl__UEBAHAEAVCPoint___Z(const CMFCBaseTabCtrl*, CPoint&);
extern "C" UINT impl__AFX_WM_CHANGING_ACTIVE_TAB__3IA;
extern "C" UINT impl__AFX_WM_CHANGE_ACTIVE_TAB__3IA;
extern "C" UINT impl__AFX_WM_ON_HSCROLL__3IA;
extern "C" UINT impl__AFX_WM_UPDATETOOLTIPS__3IA;
extern "C" UINT impl__AFX_WM_GETDRAGBOUNDS__3IA;
extern "C" UINT impl__AFX_WM_ON_DRAGCOMPLETE__3IA;
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();


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
// Symbol: ?DrawResizeDragRect@CMFCTabCtrl@@MEAAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__DrawResizeDragRect_CMFCTabCtrl__MEAAXAEAVCRect__0_Z(
    CMFCTabCtrl* pThis, CRect& next, CRect& previous)
{
    if (pThis) pThis->DrawResizeDragRect(next, previous);
}
// Symbol: ?EnableActiveTabCloseButton@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableActiveTabCloseButton_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, BOOL enable)
{
    if (pThis) pThis->EnableActiveTabCloseButton(enable);
}
// Symbol: ?EnableInPlaceEdit@CMFCTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableInPlaceEdit_CMFCTabCtrl__UEAAXH_Z(CMFCTabCtrl* pThis, BOOL enable)
{
    if (pThis) pThis->CMFCTabCtrl::EnableInPlaceEdit(enable);
}
// Symbol: ?EnableTabDocumentsMenu@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTabDocumentsMenu_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, BOOL enable)
{
    if (pThis) pThis->EnableTabDocumentsMenu(enable);
}
// Symbol: ?EnsureVisible@CMFCTabCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__EnsureVisible_CMFCTabCtrl__UEAAHH_Z(CMFCTabCtrl* pThis, int index)
{
    return pThis && pThis->EnsureVisible(index);
}
// Symbol: ?FindTargetWnd@CMFCTabCtrl@@MEAAPEAVCWnd@@AEBVCPoint@@@Z
extern "C" CWnd* MS_ABI impl__FindTargetWnd_CMFCTabCtrl__MEAAPEAVCWnd__AEBVCPoint___Z(
    CMFCTabCtrl* pThis, const CPoint& point)
{
    const auto* state = pThis ? FindTabCtrlState(pThis) : nullptr;
    if (!state || !pThis->m_hWnd || point.y < state->strip.Height()) return nullptr;
    if (std::any_of(state->tabs.begin(), state->tabs.end(),
            [&point](const TabEntry& tab) { return tab.visible && ::PtInRect(tab.rect, point); }))
        return nullptr;
    const HWND parent = ::GetParent(pThis->m_hWnd);
    return parent ? CWnd::FromHandle(parent) : nullptr;
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
    return impl__GetTabFromPoint_CMFCBaseTabCtrl__UEBAHAEAVCPoint___Z(pThis, point);
}
// Symbol: ?HideActiveWindowHorzScrollBar@CMFCTabCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__HideActiveWindowHorzScrollBar_CMFCTabCtrl__QEAAXXZ(CMFCTabCtrl* pThis)
{
    if (pThis) pThis->HideActiveWindowHorzScrollBar();
}
// Retail (0x18013ba30): early-returns when the flag at +0x3f4 already equals
// bHide; otherwise stores it and, when m_hWnd is valid, triggers a relayout/
// redraw (vslot 0x318) plus the internal helper 0x18013b4d0(this, 0). The
// flag member is unmodeled; the redraw side effect is kept.
// Symbol: ?HideNoTabs@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__HideNoTabs_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, int bHide)
{
    if (!pThis) return;
    EnsureTabCtrlState(pThis).hideNoTabs = bHide;
    pThis->RecalcLayout();
}
// Retail (0x18013c190): early-returns when the flag at +0x1f4 already equals
// bHide; otherwise stores it and, when m_hWnd is valid, triggers a relayout/
// redraw (vslot 0x318) plus the internal helper 0x18013b4d0(this, 0). The
// flag member is unmodeled; the redraw side effect is kept.
// Symbol: ?HideSingleTab@CMFCTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__HideSingleTab_CMFCTabCtrl__UEAAXH_Z(
    CMFCTabCtrl* pThis, int bHide)
{
    if (pThis) pThis->HideSingleTab(bHide);
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
// Symbol: ?IsPtInTabArea@CMFCTabCtrl@@UEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__IsPtInTabArea_CMFCTabCtrl__UEBAHVCPoint___Z(
    const CMFCTabCtrl* pThis, CPoint point)
{
    const auto* state = pThis ? FindTabCtrlState(pThis) : nullptr;
    return state && ::PtInRect(state->strip, point);
}
namespace {
constexpr UINT tabTooltipType = 0x0004;
constexpr size_t flatTabOffset = 0x3c0;
constexpr size_t sharedScrollbarOffset = 0x3d0;
constexpr size_t sharedScrollbarWidthOffset = 0x424;
constexpr int sharedScrollbarSplitterWidth = 5;
constexpr size_t tabScrollOffset = 0x3d4;
constexpr size_t documentsMenuOffset = 0x400;
constexpr size_t hiddenDocumentsOffset = 0x404;
constexpr size_t tabScrollPositionOffset = 0x414;
constexpr size_t firstVisibleTabOffset = 0x420;
constexpr size_t activeTabBoldOffset = 0x3f8;
constexpr size_t tabMaxWidthOffset = 0x428;

BOOL& TabControlFlag(CMFCTabCtrl& tabs, size_t offset) {
    return *reinterpret_cast<BOOL*>(reinterpret_cast<BYTE*>(&tabs) + offset);
}

bool HasRoundedTabStyle(CMFCTabCtrl& tabs) {
    return TabControlFlag(tabs, 0x3c4) || TabControlFlag(tabs, 0x3c8) || TabControlFlag(tabs, 0x3cc);
}

void ResetRoundedTabScroll(CMFCTabCtrl& tabs) {
    if (!HasRoundedTabStyle(tabs)) return;
    TabControlFlag(tabs, tabScrollPositionOffset) = 0;
    TabControlFlag(tabs, firstVisibleTabOffset) = 0;
}

void SetTabStyleFlags(CMFCTabCtrl& tabs, int style, bool creating) {
    const bool sharedScroll = style == CMFCTabCtrl::STYLE_FLAT_SHARED_HORZ_SCROLL;
    const bool flat = style == CMFCTabCtrl::STYLE_FLAT || (creating && sharedScroll);
    const bool oneNote = style == CMFCTabCtrl::STYLE_3D_ONENOTE;
    TabControlFlag(tabs, flatTabOffset) = flat;
    TabControlFlag(tabs, 0x3c4) = oneNote;
    TabControlFlag(tabs, 0x3c8) = style == CMFCTabCtrl::STYLE_3D_VS2005;
    TabControlFlag(tabs, 0x3cc) = style == CMFCTabCtrl::STYLE_3D_ROUNDED || style == CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL;
    TabControlFlag(tabs, 0xf0) = oneNote;
    if (creating) {
        TabControlFlag(tabs, sharedScrollbarOffset) = sharedScroll;
        TabControlFlag(tabs, tabScrollOffset) = flat || oneNote || style == CMFCTabCtrl::STYLE_3D_VS2005 ||
            style == CMFCTabCtrl::STYLE_3D_SCROLLED || style == CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL;
    }
}
}

// Symbol: ?ModifyTabStyle@CMFCTabCtrl@@QEAAHW4Style@1@@Z
extern "C" int MS_ABI impl__ModifyTabStyle_CMFCTabCtrl__QEAAHW4Style_1__Z(
    CMFCTabCtrl* pThis, int style)
{
    if (!pThis) return FALSE;
    SetTabStyleFlags(*pThis, style, false);
    pThis->RefreshTabs();
    return TRUE;
}
// Symbol: ?MoveTab@CMFCTabCtrl@@UEAAXHH@Z
extern "C" void MS_ABI impl__MoveTab_CMFCTabCtrl__UEAAXHH_Z(CMFCTabCtrl* pThis, int from, int to)
{
    if (pThis) pThis->MoveTab(from, to);
}
// Symbol: ?OnCancelMode@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    if (pThis) pThis->OnCancelMode();
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
    CMFCTabCtrl* pThis, CDC* pDC)
{
    if (pThis) pThis->DrawTabs(pDC);
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
// Symbol: ?OnHScroll@CMFCTabCtrl@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CMFCTabCtrl__IEAAXIIPEAVCScrollBar___Z(
    CMFCTabCtrl* pThis, UINT code, UINT position, CScrollBar* source)
{
    if (pThis) pThis->OnHScroll(code, position, source);
}
// Symbol: ?OnLButtonDblClk@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, UINT flags, CPoint point)
{
    if (pThis) pThis->OnLButtonDblClk(flags, point);
}
// Retail (0x180139430): when the point is inside the strip rect (+0x42c) it
// sets the +0x3dc capture flag and calls SetCapture, then handles the
// resize-drag path (rects +0x46c/+0x48c, style +0x3d28) and finally activates
// the clicked tab. The rect members and capture flag are unmodeled.
// Symbol: ?OnLButtonDown@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, UINT flags, CPoint point)
{
    if (pThis) pThis->OnLButtonDown(flags, point);
}
// Symbol: ?OnLButtonUp@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, UINT flags, CPoint point)
{
    if (pThis) pThis->OnLButtonUp(flags, point);
}
// Symbol: ?OnMouseMove@CMFCTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCTabCtrl__IEAAXIVCPoint___Z(
    CMFCTabCtrl* pThis, UINT flags, CPoint point)
{
    if (pThis) pThis->OnMouseMove(flags, point);
}
// Symbol: ?OnNotify@CMFCTabCtrl@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CMFCTabCtrl__MEAAH_K_JPEA_J_Z(
    CMFCTabCtrl* pThis, unsigned long long wParam, long long lParam, long long* result)
{
    return pThis && pThis->CMFCTabCtrl::OnNotify(wParam, lParam, result);
}
// Symbol: ?OnPaint@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    if (pThis) pThis->OnPaint();
}
static_assert(sizeof(CMFCTabCtrl) == 0x3d68);

CMFCTabCtrl::CMFCTabCtrl() {
    static_assert(offsetof(CMFCTabCtrl, m_tabCloseButton) == 0x3d8);
    static_assert(offsetof(CMFCTabCtrl, m_flatFrame) == 0x3e0);
    static_assert(offsetof(CMFCTabCtrl, m_drawFrame) == 0x3f0);
    static_assert(offsetof(CMFCTabCtrl, m_activeTabCloseButton) == 0x3fc);
    static_assert(offsetof(CMFCTabCtrl, m_resizing) == 0x40c);
    static_assert(offsetof(CMFCTabCtrl, m_resizeGrip) == 0x46c);
    static_assert(offsetof(CMFCTabCtrl, m_resizeDrag) == 0x47c);
    static_assert(offsetof(CMFCTabCtrl, m_resizeBounds) == 0x48c);
    static_assert(offsetof(CMFCTabCtrl, m_wndScrollWnd) == 0x4a0);
    static_assert(offsetof(CMFCTabCtrl, m_resizeMode) == 0x3d28);
}

CMFCTabCtrl::~CMFCTabCtrl() {
    if (m_hWnd) DestroyWindow();
}

const AFX_MSGMAP* AFXAPI CMFCTabCtrl::GetThisMessageMap() {
    return &openmfc::detail::mfc07msgmap::classCMFCTabCtrl_msgmap;
}

const AFX_MSGMAP* CMFCTabCtrl::GetMessageMap() const {
    return GetThisMessageMap();
}

BOOL CMFCTabCtrl::CreateTabs(int style, const RECT& rect, CWnd* parent, UINT id, int location, BOOL closeButton) {
    if (m_hWnd || !parent || !parent->GetSafeHwnd()) return FALSE;
    SetLocation(location);
    if (!CMFCBaseTabCtrl::Create(WS_VISIBLE | WS_CLIPSIBLINGS, rect, parent, id)) return FALSE;
    SetTabStyleFlags(*this, style, true);
    m_tabCloseButton = closeButton;
    if (auto* scrollbar = GetScrollBar()) {
        const RECT empty{};
        if (!scrollbar->Create(WS_CHILD | SBS_HORZ, empty, this, 0)) {
            DestroyWindow();
            return FALSE;
        }
    }
    EnsureTabCtrlState(this).liveLayout = true;
    RecalcLayout();
    return TRUE;
}

CScrollBar* CMFCTabCtrl::GetScrollBar() {
    return TabControlFlag(*this, sharedScrollbarOffset) ? &m_wndScrollWnd : nullptr;
}

void CMFCTabCtrl::HideActiveWindowHorzScrollBar() {
    if (!GetScrollBar()) return;
    const auto* active = GetTabWnd(GetActiveTab());
    const HWND window = active ? active->GetSafeHwnd() : nullptr;
    if (!window) return;
    ::ShowScrollBar(window, SB_HORZ, FALSE);
    if (::IsWindow(window)) CWnd::ModifyStyle(window, WS_HSCROLL, 0, SWP_FRAMECHANGED);
}

BOOL CMFCTabCtrl::SynchronizeScrollBar(SCROLLINFO* info) {
    auto* scrollbar = GetScrollBar();
    if (!scrollbar) return FALSE;
    const HWND bar = scrollbar->GetSafeHwnd();
    if (!bar) return TRUE;
    const HWND control = m_hWnd;
    auto* active = GetTabWnd(GetActiveTab());
    SCROLLINFO scroll{sizeof(scroll), SIF_ALL};
    if (info) {
        scroll = *info;
        scroll.cbSize = sizeof(scroll);
    } else if (!active || !active->GetSafeHwnd() ||
        !active->GetScrollInfo(SB_HORZ, &scroll, SIF_ALL) ||
        LONGLONG(scroll.nMin) + scroll.nPage >= scroll.nMax) {
        if (CWnd::FromHandlePermanent(control) == this)
            ::EnableScrollBar(bar, SB_CTL, ESB_DISABLE_BOTH);
        return TRUE;
    }
    if (CWnd::FromHandlePermanent(control) != this) return TRUE;
    ::EnableScrollBar(bar, SB_CTL, ESB_ENABLE_BOTH);
    ::SetScrollInfo(bar, SB_CTL, &scroll, TRUE);
    if (CWnd::FromHandlePermanent(control) == this) HideActiveWindowHorzScrollBar();
    return TRUE;
}

void CMFCTabCtrl::AdjustWndScroll() {
    const auto* scrollbar = GetScrollBar();
    const auto* state = FindTabCtrlState(this);
    if (!scrollbar || !scrollbar->GetSafeHwnd() || !state) return;
    const CRect bounds = state->sharedScrollbarBounds;
    ::SetWindowPos(scrollbar->GetSafeHwnd(), nullptr, bounds.left, bounds.top, bounds.Width(), bounds.Height(),
        SWP_NOACTIVATE | SWP_NOZORDER | (bounds.IsRectEmpty() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW));
}

void CMFCTabCtrl::OnHScroll(UINT code, UINT position, CScrollBar* source) {
    const auto* scrollbar = GetScrollBar();
    if (!scrollbar || !source || !source->GetSafeHwnd() ||
        source->GetSafeHwnd() != scrollbar->GetSafeHwnd()) {
        if (!source || !source->SendChildNotifyLastMsg(nullptr)) Default();
        return;
    }
    const HWND control = m_hWnd;
    const HWND bar = scrollbar->GetSafeHwnd();
    auto* active = GetTabWnd(GetActiveTab());
    const HWND window = active ? active->GetSafeHwnd() : nullptr;
    if (!window) return;
    auto& state = EnsureTabCtrlState(this);
    if (state.forwardingSharedScroll) return;
    state.forwardingSharedScroll = true;
    if (code == SB_THUMBPOSITION || code == SB_THUMBTRACK) {
        wchar_t className[32]{};
        if (::GetClassNameW(window, className, ARRAYSIZE(className)) && _wcsicmp(className, WC_LISTVIEWW) == 0) {
            const int current = active->GetScrollPos(SB_HORZ);
            if (CWnd::FromHandlePermanent(control) != this) return;
            ::SendMessageW(window, LVM_SCROLL, static_cast<int>(position) - current, 0);
        }
    }
    const WPARAM params = MAKEWPARAM(code, position);
    if (CWnd::FromHandlePermanent(control) != this) return;
    if (::IsWindow(window)) ::SendMessageW(window, WM_HSCROLL, params, 0);
    if (CWnd::FromHandlePermanent(control) != this) return;
    state.forwardingSharedScroll = false;
    if (active == GetTabWnd(GetActiveTab()) && ::IsWindow(window)) {
        const int current = active->GetScrollPos(SB_HORZ);
        if (CWnd::FromHandlePermanent(control) != this) return;
        ::SetScrollPos(bar, SB_CTL, current, TRUE);
        HideActiveWindowHorzScrollBar();
    }
    if (CWnd::FromHandlePermanent(control) == this) {
        const HWND parent = ::GetParent(control);
        if (parent) ::SendMessageW(parent, impl__AFX_WM_ON_HSCROLL__3IA, params, 0);
    }
}

void CMFCTabCtrl::SetCloseButton(BOOL enable) {
    if (m_tabCloseButton == enable) return;
    m_tabCloseButton = enable;
    RefreshTabs();
}

void CMFCTabCtrl::EnableActiveTabCloseButton(BOOL enable) {
    if (m_activeTabCloseButton == enable) return;
    m_activeTabCloseButton = enable;
    RefreshTabs();
}

void CMFCTabCtrl::SetFlatFrame(BOOL flat, BOOL repaint) {
    if (m_flatFrame == flat) return;
    m_flatFrame = flat;
    if (repaint && m_hWnd) {
        ::InvalidateRect(m_hWnd, nullptr, FALSE);
        ::UpdateWindow(m_hWnd);
    }
}

bool CMFCTabCtrl::PressTabCloseButton(CPoint point) {
    const auto* current = FindTabCtrlState(this);
    if (!current) return false;
    const RECT bounds = TabCloseButtonBounds(*current);
    if (!::PtInRect(&bounds, point)) return false;
    const HWND control = m_hWnd;
    const int id = current->tabs[current->activeTab].id;
    ::SetCapture(control);
    if (CWnd::FromHandlePermanent(control) != this || ::GetCapture() != control) return true;
    auto& state = EnsureTabCtrlState(this);
    state.closePressedTabId = id;
    state.closeButtonHot = true;
    m_closeButtonPressed = m_closeButtonHighlighted = m_activeTabCloseButton;
    ::InvalidateRect(control, &bounds, FALSE);
    return true;
}

void CMFCTabCtrl::TrackTabCloseButton(CPoint point) {
    const auto* current = FindTabCtrlState(this);
    if (!current) return;
    const RECT bounds = TabCloseButtonBounds(*current);
    const bool hot = ::PtInRect(&bounds, point) != FALSE;
    if (current->closeButtonHot == hot) return;
    auto& state = EnsureTabCtrlState(this);
    state.closeButtonHot = hot;
    m_closeButtonHighlighted = hot && m_activeTabCloseButton;
    if (hot) {
        TRACKMOUSEEVENT track{sizeof(track), TME_LEAVE, m_hWnd};
        ::TrackMouseEvent(&track);
    }
    ::InvalidateRect(m_hWnd, &bounds, FALSE);
}

void CMFCTabCtrl::CancelTabCloseButton() {
    const auto* current = FindTabCtrlState(this);
    if (!current) return;
    const bool pressed = current->closePressedTabId != -1;
    if (!pressed && !current->closeButtonHot) return;
    const RECT bounds = TabCloseButtonBounds(*current);
    auto& state = EnsureTabCtrlState(this);
    state.closePressedTabId = -1;
    state.closeButtonHot = false;
    m_closeButtonHighlighted = m_closeButtonPressed = FALSE;
    const HWND control = m_hWnd;
    ::InvalidateRect(control, &bounds, FALSE);
    if (pressed && ::GetCapture() == control) ::ReleaseCapture();
}

void CMFCTabCtrl::OnCancelMode() {
    const HWND control = m_hWnd;
    CancelTabCloseButton();
    if (CWnd::FromHandlePermanent(control) != this) return;
    StopResize(TRUE);
    if (CWnd::FromHandlePermanent(control) == this) CMFCBaseTabCtrl::OnCancelMode();
}

void CMFCTabCtrl::SetResizeMode(ResizeMode mode) {
    const HWND control = m_hWnd;
    if (m_resizeMode != mode && m_resizing) {
        StopResize(TRUE);
        if (CWnd::FromHandlePermanent(control) != this) return;
    }
    m_resizeMode = mode;
    RefreshTabs();
}

void CMFCTabCtrl::DrawResizeDragRect(const CRect& next, const CRect& previous) {
    if (::EqualRect(next, previous)) return;
    const HDC screen = ::GetDC(nullptr);
    if (!screen) return;
    // Drawing the same focus rectangle twice erases it without repainting the pane.
    if (!previous.IsRectEmpty()) ::DrawFocusRect(screen, previous);
    if (!next.IsRectEmpty()) ::DrawFocusRect(screen, next);
    ::ReleaseDC(nullptr, screen);
}

bool CMFCTabCtrl::StartResize(CPoint point) {
    if (!::PtInRect(m_resizeGrip, point)) return false;
    const HWND control = m_hWnd;
    const HWND parent = ::GetParent(control);
    const ResizeMode mode = m_resizeMode;
    CRect bounds{};
    const auto accepted = parent && ::SendMessageW(parent, impl__AFX_WM_GETDRAGBOUNDS__3IA,
        reinterpret_cast<WPARAM>(this), reinterpret_cast<LPARAM>(static_cast<RECT*>(bounds)));
    if (CWnd::FromHandlePermanent(control) != this) return true;
    if (!accepted || bounds.IsRectEmpty() || m_resizeMode != mode ||
        !::PtInRect(m_resizeGrip, point)) return false;

    CRect drag = m_resizeGrip;
    ::MapWindowPoints(control, nullptr, reinterpret_cast<POINT*>(static_cast<RECT*>(drag)), 2);
    if (mode == RESIZE_VERT ? bounds.Width() < drag.Width() : bounds.Height() < drag.Height())
        return false;
    ::SetCapture(control);
    if (CWnd::FromHandlePermanent(control) != this || ::GetCapture() != control) return true;
    m_resizeBounds = bounds;
    m_resizeDrag = drag;
    m_resizing = TRUE;
    DrawResizeDragRect(drag, CRect{});
    return true;
}

void CMFCTabCtrl::OnMouseMove(UINT, CPoint point) {
    POINT screen = point;
    if (!m_resizing) {
        TrackTabCloseButton(point);
        return;
    }
    if (!::ClientToScreen(m_hWnd, &screen)) return;
    CRect next = m_resizeDrag;
    if (m_resizeMode == RESIZE_VERT) {
        const int width = next.Width();
        next.left = (std::clamp)(static_cast<int>(screen.x) - width / 2, m_resizeBounds.left, m_resizeBounds.right - width);
        next.right = next.left + width;
    } else {
        const int height = next.Height();
        next.top = (std::clamp)(static_cast<int>(screen.y) - height / 2, m_resizeBounds.top, m_resizeBounds.bottom - height);
        next.bottom = next.top + height;
    }
    DrawResizeDragRect(next, m_resizeDrag);
    m_resizeDrag = next;
}

void CMFCTabCtrl::OnLButtonUp(UINT, CPoint point) {
    if (m_resizing) {
        StopResize(FALSE);
        return;
    }
    const auto* state = FindTabCtrlState(this);
    if (!state || state->closePressedTabId == -1) return;
    const RECT bounds = TabCloseButtonBounds(*state);
    HWND target = nullptr;
    if (::PtInRect(&bounds, point) && state->tabs[state->activeTab].id == state->closePressedTabId) {
        if (const auto* page = GetTabWnd(state->activeTab)) target = page->GetSafeHwnd();
    }
    const HWND control = m_hWnd;
    CancelTabCloseButton();
    if (target && CWnd::FromHandlePermanent(control) == this && ::IsWindow(target))
        ::SendMessageW(target, WM_CLOSE, 0, 0);
}

void CMFCTabCtrl::StopResize(BOOL cancel) {
    if (!m_resizing) return;
    const HWND control = m_hWnd;
    RECT proposed{};
    const BOOL notify = !cancel && ::GetWindowRect(control, &proposed);
    if (m_resizeMode == RESIZE_VERT) proposed.right = m_resizeDrag.right;
    else proposed.bottom = m_resizeDrag.bottom;
    DrawResizeDragRect(CRect{}, m_resizeDrag);
    m_resizing = FALSE;
    m_resizeDrag.SetRectEmpty();
    m_resizeBounds.SetRectEmpty();
    if (::GetCapture() == control) ::ReleaseCapture();

    if (notify && CWnd::FromHandlePermanent(control) == this) {
        if (const HWND parent = ::GetParent(control))
            ::SendMessageW(parent, impl__AFX_WM_ON_DRAGCOMPLETE__3IA,
                reinterpret_cast<WPARAM>(this), reinterpret_cast<LPARAM>(&proposed));
    }
}

BOOL CMFCTabCtrl::SetResizeCursor() {
    if (!m_resizing) {
        POINT cursor{};
        if (!::GetCursorPos(&cursor) || !::ScreenToClient(m_hWnd, &cursor) ||
            !::PtInRect(m_resizeGrip, cursor)) return FALSE;
    }
    ::SetCursor(::LoadCursorW(nullptr, m_resizeMode == RESIZE_VERT ? IDC_SIZEWE : IDC_SIZENS));
    return TRUE;
}

LRESULT CMFCTabCtrl::OnUpdateToolTips(WPARAM types, LPARAM) {
    if (types & tabTooltipType) {
        const HWND control = m_hWnd;
        DestroyTabTooltips();
        if (CWnd::FromHandlePermanent(control) == this) RefreshTabs();
    }
    return 0;
}

LRESULT CMFCTabCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    if (message && message == impl__AFX_WM_UPDATETOOLTIPS__3IA)
        return OnUpdateToolTips(wParam, lParam);

    if (message == WM_NCHITTEST) {
        POINT point{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        if (::ScreenToClient(m_hWnd, &point) && ::PtInRect(m_resizeGrip, point)) return HTCLIENT;
    } else if (message == WM_SETCURSOR && SetResizeCursor()) {
        return TRUE;
    }
    if (m_resizing) {
        if (message == WM_MOUSEMOVE || message == WM_LBUTTONUP) {
            const CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            if (message == WM_MOUSEMOVE) OnMouseMove(static_cast<UINT>(wParam), point);
            else OnLButtonUp(static_cast<UINT>(wParam), point);
            return 0;
        }
        const bool escape = message == WM_KEYDOWN && wParam == VK_ESCAPE;
        if (escape || message == WM_CANCELMODE || message == WM_CAPTURECHANGED || message == WM_NCDESTROY) {
            const HWND control = m_hWnd;
            StopResize(TRUE);
            if (escape || CWnd::FromHandlePermanent(control) != this) return 0;
        }
    }
    if (auto* scrollbar = GetScrollBar()) {
        if (message == WM_HSCROLL && lParam && reinterpret_cast<HWND>(lParam) == scrollbar->GetSafeHwnd()) {
            OnHScroll(LOWORD(wParam), HIWORD(wParam), scrollbar);
            return 0;
        }
        if (FindTabCtrlState(this)) {
            auto& state = EnsureTabCtrlState(this);
            if (message == WM_LBUTTONDOWN && !m_pInPlaceEdit) {
                const POINT point{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                if (::PtInRect(state.sharedScrollSplitter, point)) {
                    state.sharedScrollDragOffset = state.sharedScrollSplitter.right - point.x;
                    state.resizingSharedScrollbar = true;
                    ::SetCapture(m_hWnd);
                    return 0;
                }
            } else if (message == WM_MOUSEMOVE && state.resizingSharedScrollbar) {
                if (!state.sharedScrollSplitter.IsRectEmpty()) {
                    const int width = state.sharedScrollbarBounds.right - GET_X_LPARAM(lParam) -
                        state.sharedScrollDragOffset;
                    TabControlFlag(*this, sharedScrollbarWidthOffset) = (std::max)(1, width);
                    RecalcLayout();
                }
                return 0;
            } else if (message == WM_NCHITTEST || message == WM_SETCURSOR) {
                POINT cursor{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                if (message == WM_SETCURSOR) ::GetCursorPos(&cursor);
                if (::ScreenToClient(m_hWnd, &cursor) && ::PtInRect(state.sharedScrollSplitter, cursor)) {
                    if (message == WM_NCHITTEST) return HTCLIENT;
                    ::SetCursor(::LoadCursorW(nullptr, IDC_SIZEWE));
                    return TRUE;
                }
            }
            if (message == WM_NCDESTROY) state.forwardingSharedScroll = false;
            if (state.resizingSharedScrollbar && (message == WM_LBUTTONUP || message == WM_CANCELMODE ||
                    message == WM_CAPTURECHANGED || message == WM_NCDESTROY)) {
                state.resizingSharedScrollbar = false;
                const HWND control = m_hWnd;
                if (message != WM_CAPTURECHANGED && ::GetCapture() == control) ::ReleaseCapture();
                if (message == WM_LBUTTONUP || CWnd::FromHandlePermanent(control) != this) return 0;
            }
        }
    }
    if (message == WM_MOUSEMOVE) {
        OnMouseMove(static_cast<UINT>(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
    } else if (const auto* state = FindTabCtrlState(this)) {
        if (message == WM_LBUTTONUP && state->closePressedTabId != -1) {
            OnLButtonUp(static_cast<UINT>(wParam), CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
            return 0;
        }
        if (message == WM_MOUSELEAVE && state->closePressedTabId == -1) {
            CancelTabCloseButton();
        } else if (message == WM_CAPTURECHANGED || message == WM_CANCELMODE || message == WM_NCDESTROY) {
            const HWND control = m_hWnd;
            CancelTabCloseButton();
            if (CWnd::FromHandlePermanent(control) != this) return 0;
        }
    }
    return CMFCBaseTabCtrl::WindowProc(message, wParam, lParam);
}

namespace {
class TabImages {
public:
    explicit TabImages(const CMFCBaseTabCtrl& tabs)
        : list_(tabs.GetImageListHandle()), size_(tabs.GetImageSize()),
          count_(list_ ? ::ImageList_GetImageCount(list_) : 0) {
        if (list_ && size_.cx && size_.cy)
            ::ImageList_GetIconSize(list_, &listSize_.cx, &listSize_.cy);
    }

    CSize Size(const TabEntry& tab) const {
        if (tab.customIcon)
            return CSize((std::max)(size_.cx, ::GetSystemMetrics(SM_CXSMICON)),
                         (std::max)(size_.cy, ::GetSystemMetrics(SM_CYSMICON)));
        if (HasListImage(tab)) return listSize_;
        if (tab.windowIcon) return CSize(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
        return CSize(0, 0);
    }

    void Draw(HDC dc, const TabEntry& tab, int x, int y, CSize size) const {
        if (tab.customIcon || !HasListImage(tab)) {
            const HICON icon = tab.customIcon ? tab.customIcon.get() : tab.windowIcon;
            if (icon) ::DrawIconEx(dc, x, y, icon, size.cx, size.cy, 0, nullptr, DI_NORMAL);
        } else {
            ::ImageList_Draw(list_, static_cast<int>(tab.image), dc, x, y, ILD_TRANSPARENT);
        }
    }

private:
    bool HasListImage(const TabEntry& tab) const {
        return count_ > 0 && tab.image < static_cast<UINT>(count_) && listSize_.cx > 0 && listSize_.cy > 0;
    }

    HIMAGELIST list_;
    CSize size_;
    CSize listSize_{0, 0};
    int count_;
};

using TabFont = std::unique_ptr<std::remove_pointer_t<HFONT>, decltype(&::DeleteObject)>;

TabFont CreateBoldTabFont(CMFCTabCtrl& tabs, HGDIOBJ regular) {
    LOGFONTW font{};
    if (!TabControlFlag(tabs, activeTabBoldOffset) ||
        !::GetObjectW(regular, sizeof(font), &font)) return TabFont(nullptr, &::DeleteObject);
    font.lfWeight = FW_BOLD;
    return TabFont(::CreateFontIndirectW(&font), &::DeleteObject);
}

UINT_PTR TabTooltipId(const TabEntry& tab) {
    return static_cast<UINT_PTR>(tab.id) + 1;
}

bool CreateTabTooltip(CMFCTabCtrl& tabs, CToolTipCtrl*& target) {
    const HWND owner = tabs.m_hWnd;
    CToolTipCtrl* created = nullptr;
    if (!CTooltipManager::CreateToolTip(created, &tabs, tabTooltipType)) return false;
    if (CWnd::FromHandlePermanent(owner) != &tabs) {
        CTooltipManager::DeleteToolTip(created);
        return false;
    }
    target = created;
    return true;
}

void UpdateLabelTooltips(CMFCTabCtrl& tabs, TabCtrlState& state, const std::vector<int>& fullWidths,
                         CToolTipCtrl*& labelTooltip) {
    const HWND owner = tabs.m_hWnd;
    std::vector<UINT_PTR> updated;
    for (size_t i = 0; i < state.tabs.size(); ++i) {
        const auto& tab = state.tabs[i];
        if (!tab.visible || tab.rect.IsRectEmpty()) continue;
        if (!tabs.UsesCustomToolTips() && (tab.label.empty() ||
            (!tab.iconOnly && !tab.tooltipAlways && tab.rect.Width() >= fullWidths[i]))) continue;
        CRect bounds = tab.rect;
        if (static_cast<int>(i) == state.activeTab) {
            const auto close = tabs.GetTabCloseButton();
            if (!close.IsRectEmpty()) bounds.right = close.left;
        }
        if (bounds.IsRectEmpty()) continue;
        if (!labelTooltip) {
            if (!CreateTabTooltip(tabs, labelTooltip)) return;
            state.labelTooltips = labelTooltip->GetSafeHwnd();
            if (::SendMessageW(state.labelTooltips, TTM_GETMAXTIPWIDTH, 0, 0) < 0)
                labelTooltip->SetMaxTipWidth(600);
        }
        TOOLINFOW tool{sizeof(TOOLINFOW)};
        tool.hwnd = owner;
        tool.uId = TabTooltipId(tab);
        tool.uFlags = TTF_SUBCLASS;
        tool.lpszText = LPSTR_TEXTCALLBACKW;
        tool.rect = *static_cast<const RECT*>(bounds);
        const bool existing = std::find(state.labelTooltipIds.begin(), state.labelTooltipIds.end(),
            tool.uId) != state.labelTooltipIds.end();
        const UINT message = existing ? TTM_NEWTOOLRECTW : TTM_ADDTOOLW;
        const auto result = ::SendMessageW(state.labelTooltips, message, 0, reinterpret_cast<LPARAM>(&tool));
        if (existing || result) updated.push_back(tool.uId);
    }

    for (UINT_PTR id : state.labelTooltipIds) {
        if (std::find(updated.begin(), updated.end(), id) != updated.end()) continue;
        TOOLINFOW tool{sizeof(TOOLINFOW)};
        tool.hwnd = owner;
        tool.uId = id;
        ::SendMessageW(state.labelTooltips, TTM_DELTOOLW, 0, reinterpret_cast<LPARAM>(&tool));
    }
    state.labelTooltipIds = std::move(updated);
}

void UpdateCloseTooltip(CMFCTabCtrl& tabs, CToolTipCtrl*& closeTooltip) {
    const CRect bounds = tabs.GetTabCloseButton();
    if (!closeTooltip && (bounds.IsRectEmpty() || !CreateTabTooltip(tabs, closeTooltip))) return;
    TOOLINFOW tool{sizeof(TOOLINFOW)};
    tool.hwnd = tabs.m_hWnd;
    tool.uId = 1;
    tool.rect = *static_cast<const RECT*>(bounds);
    tool.uFlags = TTF_SUBCLASS;
    tool.lpszText = LPSTR_TEXTCALLBACKW;
    const HWND tooltip = closeTooltip->GetSafeHwnd();
    const bool exists = ::SendMessageW(tooltip, TTM_GETTOOLCOUNT, 0, 0) != 0;
    if (bounds.IsRectEmpty() && !exists) return;
    const UINT message = bounds.IsRectEmpty() ? TTM_DELTOOLW : exists ? TTM_NEWTOOLRECTW : TTM_ADDTOOLW;
    ::SendMessageW(tooltip, message, 0, reinterpret_cast<LPARAM>(&tool));
}

RECT TabCloseBounds(const CRect& strip, const CRect& area) {
    const int size = (std::min)(15, strip.Height() - 4);
    if (size <= 0 || area.Width() < size + 4 || strip.IsRectEmpty()) return RECT{};
    const int top = strip.top + (strip.Height() - size) / 2;
    return RECT{area.right - size - 2, top, area.right - 2, top + size};
}

bool HasOverflowedDocuments(const TabCtrlState& state) {
    return std::any_of(state.tabs.begin(), state.tabs.end(),
        [](const auto& tab) { return tab.visible && !tab.temporary && tab.rect.IsRectEmpty(); });
}

int TabStripEnd(const TabCtrlState& state) {
    return state.closeButton && !state.activeCloseButton
        ? state.strip.right - (std::min)(19, state.strip.Width()) : state.strip.right;
}

RECT DocumentsButtonBounds(const TabCtrlState& state) {
    if (!state.documentsMenu || state.strip.IsRectEmpty() ||
        std::none_of(state.tabs.begin(), state.tabs.end(),
            [](const TabEntry& tab) { return tab.visible && !tab.temporary; }))
        return RECT{};
    CRect area = state.strip;
    area.right = TabStripEnd(state);
    return TabCloseBounds(state.strip, area);
}


}

RECT openmfc::detail::mfccore::TabCloseButtonBounds(const TabCtrlState& state) {
    if (state.activeTab < 0 || state.activeTab >= static_cast<int>(state.tabs.size())) return RECT{};
    if (state.activeCloseButton) return TabCloseBounds(state.strip, state.tabs[state.activeTab].rect);
    if (state.closeButton) return TabCloseBounds(state.strip, state.strip);
    return RECT{};
}

void CMFCTabCtrl::RecalcLayout() {
    if (!m_hWnd) return;
    auto& state = EnsureTabCtrlState(this);
    RECT client{};
    if (!::GetClientRect(m_hWnd, &client)) return;
    state.closeButton = m_tabCloseButton;
    state.activeCloseButton = m_activeTabCloseButton;

    m_resizeGrip.SetRectEmpty();
    constexpr LONG resizeSpace = 6;
    if (m_resizeMode == RESIZE_VERT) {
        const LONG edge = client.right;
        client.right = (std::max)(client.left, edge - resizeSpace);
        m_resizeGrip.SetRect(client.right + 1, client.top, edge, client.bottom);
    } else if (m_resizeMode == RESIZE_HORIZ) {
        const LONG edge = client.bottom;
        client.bottom = (std::max)(client.top, edge - resizeSpace);
        m_resizeGrip.SetRect(client.left, client.bottom + 1, client.right, edge);
    }

    const int inset = (std::clamp)(state.tabBorderSize, 0,
        static_cast<int>((std::min)(client.right, client.bottom)) / 2);
    ::InflateRect(&client, -inset, -inset);
    const int count = static_cast<int>(std::count_if(state.tabs.begin(), state.tabs.end(),
        [](const TabEntry& tab) { return tab.visible != FALSE; }));
    const TabImages images(*this);
    int height = state.tabsHeight > 0 ? state.tabsHeight : ::GetSystemMetrics(SM_CYSMCAPTION) + 7;
    if (state.tabsHeight <= 0) {
        for (const auto& tab : state.tabs) {
            if (tab.visible) height = (std::max)(height, static_cast<int>(images.Size(tab).cy) + 6);
        }
    }
    height = (std::min)(height, static_cast<int>(client.bottom - client.top));
    const bool hideLabels = (state.hideNoTabs && count == 0) || (state.hideSingleTab && count <= 1);
    if (hideLabels && !GetScrollBar()) height = 0;

    state.page = CRect(client);
    state.strip = CRect(client);
    if (state.location == 1) {
        state.page.top += height;
        state.strip.bottom = state.page.top;
    } else {
        state.page.bottom -= height;
        state.strip.top = state.page.bottom;
    }
    state.sharedScrollbarBounds.SetRectEmpty();
    state.sharedScrollSplitter.SetRectEmpty();
    if (GetScrollBar() && height > 3) {
        const int minWidth = 2 * ::GetSystemMetrics(SM_CXHSCROLL);
        const int maxWidth = state.strip.Width() - sharedScrollbarSplitterWidth - 32;
        auto& width = TabControlFlag(*this, sharedScrollbarWidthOffset);
        if (hideLabels || count == 0) {
            state.sharedScrollbarBounds = state.strip;
            state.strip.right = state.strip.left;
        } else if (maxWidth >= minWidth) {
            if (width == 0) width = (std::max)(minWidth, state.strip.Width() / 2);
            state.sharedScrollbarBounds = state.strip;
            state.sharedScrollbarBounds.left = state.strip.right - (std::clamp)(width, minWidth, maxWidth);
            state.sharedScrollSplitter = state.sharedScrollbarBounds;
            state.sharedScrollSplitter.right = state.sharedScrollbarBounds.left;
            state.sharedScrollSplitter.left = state.sharedScrollSplitter.right - sharedScrollbarSplitterWidth;
            state.strip.right = state.sharedScrollSplitter.left;
        }
        if (!state.sharedScrollbarBounds.IsRectEmpty()) {
            ++state.sharedScrollbarBounds.top;
            state.sharedScrollbarBounds.bottom -= 2;
        }
    }
    const CRect page = state.page;
    const HWND control = m_hWnd;

    const HDC dc = ::GetDC(control);
    const auto regularFont = ::GetStockObject(DEFAULT_GUI_FONT);
    const auto oldFont = dc ? ::SelectObject(dc, regularFont) : nullptr;
    const auto boldFont = CreateBoldTabFont(*this, regularFont);
    const RECT documentsButton = DocumentsButtonBounds(state);
    const bool documentsMenu = !::IsRectEmpty(&documentsButton);
    int lastX = documentsMenu ? documentsButton.left - 2 : TabStripEnd(state);
    auto& widths = state.tabNaturalWidths;
    widths.assign(state.tabs.size(), 0);
    std::vector<int> fullWidths(state.tabs.size());
    const int maxWidth = TabControlFlag(*this, tabMaxWidthOffset);
    LONGLONG totalWidth = 0;
    for (int i = 0; i < static_cast<int>(state.tabs.size()); ++i) {
        const auto& tab = state.tabs[i];
        if (!tab.visible || hideLabels || height <= 0) continue;
        SIZE text{};
        if (dc && !tab.iconOnly) {
            if (boldFont) ::SelectObject(dc, i == state.activeTab ? boldFont.get() : regularFont);
            ::GetTextExtentPoint32W(dc, tab.label.c_str(), static_cast<int>(tab.label.size()), &text);
        }
        const int closeWidth = state.activeCloseButton && i == state.activeTab ? 19 : 0;
        const int imageWidth = images.Size(tab).cx;
        const int padding = tab.iconOnly ? 12 : 20 + (imageWidth ? 4 : 0);
        fullWidths[i] = static_cast<int>((std::clamp)(
            LONGLONG(text.cx) + padding + imageWidth + closeWidth,
            tab.iconOnly ? 16LL : 32LL, LONGLONG((std::numeric_limits<int>::max)())));
        widths[i] = maxWidth > 0 ? (std::min)(maxWidth, fullWidths[i]) : fullWidths[i];
        totalWidth += widths[i];
    }
    if (dc) {
        if (oldFont) ::SelectObject(dc, oldFont);
        ::ReleaseDC(control, dc);
    }

    const auto displayed = DisplayedTabIndices(state);
    int anchor = -1;
    int active = -1;
    int first = 0;
    const bool scroll = TabControlFlag(*this, tabScrollOffset) && !documentsMenu;
    state.scrollLeft.SetRectEmpty();
    state.scrollRight.SetRectEmpty();
    if (scroll && totalWidth > lastX - state.strip.left) {
        CRect buttons = state.strip;
        buttons.right = lastX;
        const RECT right = TabCloseBounds(state.strip, buttons);
        buttons.right = right.left;
        const RECT left = TabCloseBounds(state.strip, buttons);
        if (!::IsRectEmpty(&left) && left.left > state.strip.left + 3) {
            state.scrollLeft = CRect(left);
            state.scrollRight = CRect(right);
            lastX = left.left - 2;
        }
    }
    const int available = (std::max)(0, lastX - state.strip.left);
    state.maxTabScroll = scroll ? static_cast<int>((std::clamp)(
        totalWidth - available, 0LL, LONGLONG((std::numeric_limits<int>::max)()))) : 0;
    for (int position = 0; position < static_cast<int>(displayed.size()); ++position) {
        const int index = displayed[position];
        if (index == state.activeTab) active = position;
        if (state.tabs[index].id == state.revealTabId && widths[index]) anchor = position;
        state.tabs[index].rect.SetRectEmpty();
    }
    if (anchor == -1 && documentsMenu) anchor = active;

    auto& offset = TabControlFlag(*this, tabScrollPositionOffset);
    if (documentsMenu && anchor >= 0) {
        LONGLONG space = available;
        for (int position = anchor; position >= 0; --position) {
            space -= widths[displayed[position]];
            if (space < 0) {
                first = (std::min)(position + 1, anchor);
                break;
            }
        }
        offset = 0;
    } else if (scroll) {
        LONGLONG targetOffset = offset;
        if (anchor >= 0) {
            LONGLONG anchorLeft = 0;
            for (int position = 0; position < anchor; ++position) anchorLeft += widths[displayed[position]];
            const auto anchorRight = anchorLeft + widths[displayed[anchor]];
            if (anchorLeft < offset || widths[displayed[anchor]] > available) targetOffset = anchorLeft;
            else if (anchorRight > LONGLONG(offset) + available) targetOffset = anchorRight - available;
        }
        offset = static_cast<int>((std::clamp)(targetOffset, 0LL, LONGLONG(state.maxTabScroll)));
    } else {
        offset = 0;
    }

    LONGLONG x = LONGLONG(state.strip.left) - offset;
    int remaining = count;
    int firstDrawn = -1;
    std::vector<std::pair<HWND, BOOL>> windows;
    for (int position = 0; position < static_cast<int>(displayed.size()); ++position) {
        const int index = displayed[position];
        auto& tab = state.tabs[index];
        if (widths[index] && position >= first) {
            const int space = scroll ? 0 : static_cast<int>((std::max)(0LL, lastX - x));
            int width = widths[index];
            if (documentsMenu) {
                const bool display = space >= 32 || position == anchor || (anchor < 0 && x == state.strip.left);
                width = display ? (std::min)(width, space) : 0;
            } else if (!scroll) {
                width = (std::min)(width, space - (std::min)(space / 2, remaining - 1));
            }
            const auto left = (std::max)(x, LONGLONG(state.strip.left));
            const auto right = (std::min)(x + width, LONGLONG(lastX));
            if (right > left) {
                tab.rect.SetRect(static_cast<int>(left), state.strip.top, static_cast<int>(right), state.strip.bottom);
                if (firstDrawn < 0) firstDrawn = position;
            }
            x += width;
            --remaining;
        }
        if (tab.window && tab.window->GetSafeHwnd())
            windows.emplace_back(tab.window->GetSafeHwnd(), tab.visible && state.activeTab == index);
    }
    const CRect close = m_activeTabCloseButton ? CRect(TabCloseButtonBounds(state)) : CRect{};
    if (!::EqualRect(close, m_closeButtonRect)) state.closeButtonHot = false;
    m_closeButtonRect = close;
    m_closeButtonHighlighted = !close.IsRectEmpty() && state.closeButtonHot;
    m_closeButtonPressed = !close.IsRectEmpty() && state.tabs[state.activeTab].id == state.closePressedTabId;

    TabControlFlag(*this, firstVisibleTabOffset) = (std::max)(0, firstDrawn);
    TabControlFlag(*this, hiddenDocumentsOffset) = documentsMenu && HasOverflowedDocuments(state);
    UpdateLabelTooltips(*this, state, fullWidths, m_labelTooltip);
    if (CWnd::FromHandlePermanent(control) != this) return;
    UpdateCloseTooltip(*this, m_closeTooltip);
    for (const auto& [window, show] : windows) {
        if (CWnd::FromHandlePermanent(control) != this) return;
        if (::GetParent(window) != control) continue;
        ::SetWindowPos(window, nullptr, page.left, page.top, page.Width(), page.Height(),
            SWP_NOACTIVATE | SWP_NOZORDER | (show ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
    }
    if (CWnd::FromHandlePermanent(control) != this) return;
    AdjustWndScroll();
    if (CWnd::FromHandlePermanent(control) != this) return;
    SynchronizeScrollBar();
    if (CWnd::FromHandlePermanent(control) == this) {
        ::InvalidateRect(control, nullptr, FALSE);
        PositionTabEditor();
    }
}

BOOL CMFCTabCtrl::ActivateTab(int index) {
    const auto* state = FindTabCtrlState(this);
    if (!state || index < 0 || index >= static_cast<int>(state->tabs.size()) || !state->tabs[index].visible)
        return FALSE;
    if (state->activeTab == index) return TRUE;
    const HWND control = m_hWnd;
    const HWND parent = control ? ::GetParent(control) : nullptr;
    const int id = state->tabs[index].id;
    if (parent && ::SendMessageW(parent, impl__AFX_WM_CHANGING_ACTIVE_TAB__3IA,
            index, reinterpret_cast<LPARAM>(this))) return FALSE;
    if (control && CWnd::FromHandlePermanent(control) != this) return FALSE;
    state = FindTabCtrlState(this);
    if (!state || index >= static_cast<int>(state->tabs.size()) || state->tabs[index].id != id ||
        !state->tabs[index].visible) return FALSE;
    SetActiveTab(index);
    if (parent && CWnd::FromHandlePermanent(control) == this)
        ::SendMessageW(parent, impl__AFX_WM_CHANGE_ACTIVE_TAB__3IA, index, reinterpret_cast<LPARAM>(this));
    return TRUE;
}

void CMFCTabCtrl::ActivateMDITab(int index) {
    if (index == -1) index = GetActiveTab();
    const auto* page = GetTabWnd(index);
    const HWND child = page ? page->GetSafeHwnd() : nullptr;
    const HWND client = ::GetParent(child);
    auto* frame = client ? static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(client))) : nullptr;
    if (!m_hWnd || !child || !frame || !frame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) ||
        static_cast<CMDIFrameWnd*>(frame)->m_hWndMDIClient != client) return;
    const HWND control = m_hWnd;
    if (!ActivateTab(index) || CWnd::FromHandlePermanent(control) != this ||
        ::GetParent(child) != client || GetTabWnd(GetActiveTab()) != CWnd::FromHandlePermanent(child)) return;
    if (reinterpret_cast<HWND>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0)) != child)
        ::SendMessageW(client, WM_MDIACTIVATE, reinterpret_cast<WPARAM>(child), 0);
    if (::GetParent(child) == client) ::SetFocus(child);
}

BOOL CMFCTabCtrl::EnsureVisible(int index) {
    auto& state = EnsureTabCtrlState(this);
    if (index < 0 || index >= static_cast<int>(state.tabs.size())) return FALSE;
    if (!m_hWnd || !state.tabs[index].visible || !TabControlFlag(*this, tabScrollOffset)) return TRUE;
    const auto& tab = state.tabs[index];
    if (!tab.rect.IsRectEmpty() && index < static_cast<int>(state.tabNaturalWidths.size()) &&
        tab.rect.Width() >= state.tabNaturalWidths[index]) return TRUE;

    state.revealTabId = tab.id;
    if (state.documentsMenu) CMFCBaseTabCtrl::MoveTab(index, 0);
    else RecalcLayout();
    return TRUE;
}

void CMFCTabCtrl::MoveTab(int from, int to) {
    ResetRoundedTabScroll(*this);
    CMFCBaseTabCtrl::MoveTab(from, to);
}

void CMFCTabCtrl::SwapTabs(int first, int second) {
    ResetRoundedTabScroll(*this);
    CMFCBaseTabCtrl::SwapTabs(first, second);
}

extern "C" COLORREF MS_ABI impl__GetTabBkColor_CMFCBaseTabCtrl__UEBAKH_Z(const CMFCBaseTabCtrl*, int);

void CMFCTabCtrl::DrawTabs(CDC* dc) {
    const auto* state = FindTabCtrlState(this);
    const HDC handle = dc ? dc->GetSafeHdc() : nullptr;
    if (!handle || !state) return;
    RECT client{};
    ::GetClientRect(m_hWnd, &client);
    ::FillRect(handle, &client, ::GetSysColorBrush(COLOR_BTNFACE));
    if (m_drawFrame && !state->page.IsRectEmpty()) {
        CRect pageFrame = state->page;
        pageFrame.InflateRect(1, 1);
        RECT frame{};
        if (::IntersectRect(&frame, pageFrame, &client)) {
            if (m_flatFrame) ::FrameRect(handle, &frame, ::GetSysColorBrush(COLOR_BTNSHADOW));
            else ::DrawEdge(handle, &frame, EDGE_SUNKEN, BF_RECT);
        }
    }
    if (!m_resizeGrip.IsRectEmpty()) {
        RECT edge = *static_cast<const RECT*>(m_resizeGrip);
        if (m_resizeMode == RESIZE_VERT) edge.right = edge.left + 1;
        else edge.bottom = edge.top + 1;
        ::FillRect(handle, &edge, ::GetSysColorBrush(COLOR_BTNSHADOW));
    }
    auto* visualManager = CMFCVisualManager::GetInstance();
    const int saved = ::SaveDC(handle);
    if (!saved) return;
    const auto regularFont = ::GetStockObject(DEFAULT_GUI_FONT);
    const auto boldFont = CreateBoldTabFont(*this, regularFont);
    ::SelectObject(handle, regularFont);
    ::SetBkMode(handle, TRANSPARENT);
    const auto defaultText = ::GetSysColor(COLOR_BTNTEXT);
    const TabImages images(*this);
    for (int i = 0; i < static_cast<int>(state->tabs.size()); ++i) {
        const auto& tab = state->tabs[i];
        if (!tab.visible || tab.rect.IsRectEmpty()) continue;
        const bool active = state->activeTab == i;
        visualManager->OnDrawTab(dc, tab.rect, i, active, this);
        CRect text = tab.rect;
        auto backgroundColor = impl__GetTabBkColor_CMFCBaseTabCtrl__UEBAKH_Z(this, i);
        if (backgroundColor == COLORREF(-1) && active) backgroundColor = m_activeTabColor;
        if (backgroundColor != COLORREF(-1)) {
            CRect interior = tab.rect;
            interior.DeflateRect(1, 1);
            HBRUSH background = ::CreateSolidBrush(backgroundColor);
            if (background) {
                ::FillRect(handle, interior, background);
                ::DeleteObject(background);
            }
        }
        if (boldFont) ::SelectObject(handle, active ? boldFont.get() : regularFont);
        ::SetTextColor(handle, tab.textColor != COLORREF(-1) ? tab.textColor
            : active ? GetActiveTabTextColor() : defaultText);
        text.DeflateRect(6, 0);
        if (state->activeCloseButton && active) {
            const RECT close = TabCloseBounds(state->strip, tab.rect);
            if (!::IsRectEmpty(&close)) text.right = (std::min)(text.right, static_cast<int>(close.left - 1));
        }
        const CSize icon = images.Size(tab);
        if (icon.cx > 0 && icon.cy > 0 && icon.cx <= text.Width()) {
            const int x = tab.iconOnly ? text.left + (text.Width() - icon.cx) / 2 : text.left;
            const int y = text.top + (text.Height() - icon.cy) / 2;
            const int clip = ::SaveDC(handle);
            if (clip) {
                ::IntersectClipRect(handle, text.left, text.top, text.right, text.bottom);
                images.Draw(handle, tab, x, y, icon);
                ::RestoreDC(handle, clip);
            }
            text.left += icon.cx + 4;
        }
        if (!tab.iconOnly)
            ::DrawTextW(handle, tab.label.c_str(), static_cast<int>(tab.label.size()), text,
                DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
    }
    RECT close = TabCloseButtonBounds(*state);
    if (!::IsRectEmpty(&close)) {
        UINT flags = DFCS_CAPTIONCLOSE;
        if (!state->closeButtonHot) flags |= DFCS_FLAT;
        else if (state->tabs[state->activeTab].id == state->closePressedTabId) flags |= DFCS_PUSHED;
        ::DrawFrameControl(handle, &close, DFC_CAPTION, flags);
    }
    RECT left = *static_cast<const RECT*>(state->scrollLeft);
    RECT right = *static_cast<const RECT*>(state->scrollRight);
    if (!::IsRectEmpty(&left)) {
        const int offset = TabControlFlag(*this, tabScrollPositionOffset);
        ::DrawFrameControl(handle, &left, DFC_SCROLL,
            DFCS_SCROLLLEFT | DFCS_FLAT | (offset == 0 ? DFCS_INACTIVE : 0));
        ::DrawFrameControl(handle, &right, DFC_SCROLL,
            DFCS_SCROLLRIGHT | DFCS_FLAT | (offset == state->maxTabScroll ? DFCS_INACTIVE : 0));
    }
    RECT documents = DocumentsButtonBounds(*state);
    if (!::IsRectEmpty(&documents)) {
        ::DrawFrameControl(handle, &documents, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_FLAT);
        if (HasOverflowedDocuments(*state)) {
            RECT overflow{documents.left + 4, documents.top + 4, documents.right - 4, documents.top + 5};
            ::FillRect(handle, &overflow, ::GetSysColorBrush(COLOR_BTNTEXT));
        }
    }
    if (!state->sharedScrollSplitter.IsRectEmpty()) {
        RECT splitter = *static_cast<const RECT*>(state->sharedScrollSplitter);
        ::DrawEdge(handle, &splitter, EDGE_ETCHED, BF_RIGHT);
    }
    if (saved) ::RestoreDC(handle, saved);
}

void CMFCTabCtrl::OnPaint() {
    CPaintDC dc(this);
    DrawTabs(&dc);
}

namespace {
bool ScrollTabStrip(CMFCTabCtrl& tabs, const TabCtrlState& state, CPoint point) {
    const bool left = ::PtInRect(state.scrollLeft, point);
    if (!left && !::PtInRect(state.scrollRight, point)) return false;
    auto& offset = TabControlFlag(tabs, tabScrollPositionOffset);
    if (left ? offset == 0 : offset == state.maxTabScroll) return true;

    LONGLONG target = LONGLONG(offset) + (left ? -20 : 20);
    if (HasRoundedTabStyle(tabs)) {
        const auto displayed = DisplayedTabIndices(state);
        LONGLONG boundary = 0;
        for (int index : displayed) {
            if (!state.tabNaturalWidths[index]) continue;
            const auto next = boundary + state.tabNaturalWidths[index];
            if (left ? next >= offset : next > offset) {
                target = left ? boundary : next;
                break;
            }
            boundary = next;
        }
    }
    offset = static_cast<int>((std::clamp)(target, 0LL, LONGLONG(state.maxTabScroll)));
    EnsureTabCtrlState(&tabs).revealTabId = -1;
    tabs.RecalcLayout();
    return true;
}

void SelectTabAndFocus(CMFCTabCtrl& tabs, int index) {
    const HWND control = tabs.m_hWnd;
    if (!tabs.ActivateTab(index) || CWnd::FromHandlePermanent(control) != &tabs) return;
    auto* page = tabs.GetTabWnd(tabs.GetActiveTab());
    if (page && ::IsWindowVisible(page->GetSafeHwnd())) ::SetFocus(page->GetSafeHwnd());
}
}

void CMFCTabCtrl::OnLButtonDown(UINT, CPoint point) {
    if (StartResize(point) || PressTabCloseButton(point)) return;
    if (const auto* state = FindTabCtrlState(this)) {
        if (ScrollTabStrip(*this, *state, point)) return;
        const RECT documents = DocumentsButtonBounds(*state);
        if (::PtInRect(&documents, point)) {
            POINT anchor{documents.left, documents.bottom};
            ::ClientToScreen(m_hWnd, &anchor);
            OnShowTabDocumentsMenu(CPoint(anchor.x, anchor.y));
            return;
        }
    }
    const int index = impl__GetTabFromPoint_CMFCBaseTabCtrl__UEBAHAEAVCPoint___Z(this, point);
    if (index >= 0) SelectTabAndFocus(*this, index);
    else Default();
}

void CMFCTabCtrl::SetTabMaxWidth(int width) {
    TabControlFlag(*this, tabMaxWidthOffset) = width;
    RefreshTabs();
}

void CMFCTabCtrl::SetActiveTabBoldFont(BOOL bold) {
    if (TabControlFlag(*this, activeTabBoldOffset) == bold) return;
    TabControlFlag(*this, activeTabBoldOffset) = bold;
    RefreshTabs();
}

BOOL CMFCTabCtrl::OnNotify(WPARAM id, LPARAM notification, LRESULT* result) {
    if (OnNeedTipText(static_cast<UINT>(id), reinterpret_cast<NMHDR*>(notification), result))
        return TRUE;
    return CWnd::OnNotify(id, notification, result);
}

void CMFCTabCtrl::EnableInPlaceEdit(BOOL enable) {
    if (TabControlFlag(*this, flatTabOffset)) CMFCBaseTabCtrl::EnableInPlaceEdit(enable);
}

void CMFCTabCtrl::OnLButtonDblClk(UINT flags, CPoint point) {
    if (::PtInRect(m_resizeGrip, point)) return;
    if (const auto* state = FindTabCtrlState(this)) {
        const RECT documents = DocumentsButtonBounds(*state);
        const RECT close = TabCloseButtonBounds(*state);
        if (::PtInRect(&documents, point) || ::PtInRect(&close, point) ||
            ::PtInRect(state->scrollLeft, point) || ::PtInRect(state->scrollRight, point)) return;
    }
    if (auto* target = impl__FindTargetWnd_CMFCTabCtrl__MEAAPEAVCWnd__AEBVCPoint___Z(this, point)) {
        POINT mapped{point.x, point.y};
        ::MapWindowPoints(m_hWnd, target->m_hWnd, &mapped, 1);
        ::SendMessageW(target->m_hWnd, WM_LBUTTONDBLCLK, flags, MAKELPARAM(mapped.x, mapped.y));
        return;
    }
    CMFCBaseTabCtrl::OnLButtonDblClk(flags, point);
}

void CMFCTabCtrl::OnSize(UINT, int, int) {
    Default();
    if (const auto* state = FindTabCtrlState(this); state && state->liveLayout) RecalcLayout();
}
void CMFCTabCtrl::EnableTabSwap(BOOL bEnable) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabSwapEnabled = bEnable != FALSE;
}
void CMFCTabCtrl::SetTabBorderSize(int nTabBorderSize) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabBorderSize = nTabBorderSize;
    RefreshTabs();
}
void CMFCTabCtrl::SetTabsHeight(int nTabHeight) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabsHeight = nTabHeight;
    RefreshTabs();
}

// Symbol: ?ActivateMDITab@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__ActivateMDITab_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, int index)
{
    if (pThis) pThis->ActivateMDITab(index);
}

// STUB: Retail RVA 0x1394a0 (mfc140 ANSI twin) lays out per-tab rectangles,
// buttons, and the strip using state not represented by the opaque class padding.
// Symbol: ?AdjustTabs@CMFCTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustTabs_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis)
{
    if (pThis) pThis->RecalcLayout();
}

// STUB: Retail RVA 0x13af90 (mfc140 ANSI twin) updates the strip scroll
// offsets and scrollbar state using unmodeled geometry and flags.
// Symbol: ?AdjustTabsScroll@CMFCTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustTabsScroll_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// Symbol: ?AdjustWndScroll@CMFCTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustWndScroll_CMFCTabCtrl__MEAAXXZ(CMFCTabCtrl* pThis)
{
    if (pThis) pThis->AdjustWndScroll();
}
// Symbol: ?Create@CMFCTabCtrl@@QEAAHW4Style@1@AEBUtagRECT@@PEAVCWnd@@IW4Location@CMFCBaseTabCtrl@@H@Z
extern "C" int MS_ABI impl__Create_CMFCTabCtrl__QEAAHW4Style_1_AEBUtagRECT__PEAVCWnd__IW4Location_CMFCBaseTabCtrl__H_Z(
    CMFCTabCtrl* pThis, int style, const RECT* pRect, CWnd* pParentWnd,
    unsigned int nID, int location, int bCloseBtn)
{
    return pThis && pRect && pThis->CreateTabs(style, *pRect, pParentWnd, nID, location, bCloseBtn);
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

// Symbol: ?OnSetCursor@CMFCTabCtrl@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCTabCtrl__IEAAHPEAVCWnd__II_Z(
    CMFCTabCtrl* pThis, CWnd*, UINT, UINT)
{
    return pThis && (pThis->SetResizeCursor() || impl__Default_CWnd__IEAA_JXZ(pThis));
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

namespace {
struct DocumentIconAssociation {
    UINT command;
    HICON icon;
    DocumentIconAssociation* next;
    UINT hash;
};

struct DocumentIconMap {
    void* vtable = nullptr;
    DocumentIconAssociation** buckets = nullptr;
    UINT bucketCount = 17;
    INT_PTR count = 0;
    DocumentIconAssociation* freeList = nullptr;
    void* blocks = nullptr;
    INT_PTR blockSize = 10;
};
static_assert(sizeof(DocumentIconAssociation) == 0x20);
static_assert(sizeof(DocumentIconMap) == 0x38 && offsetof(DocumentIconMap, count) == 0x18);

UINT DocumentIconHash(UINT command) {
    const auto key = static_cast<LONG>(command);
    const auto hash = 16807L * (key % 127773L) - 2836L * (key / 127773L);
    return static_cast<UINT>(hash < 0 ? hash + 2147483647L : hash);
}

struct DocumentMenuEntry {
    UINT command;
    int tabId;
    std::wstring label;
    HWND window;
    HICON icon = nullptr;
};

HBITMAP DocumentMenuBitmap(HICON icon) {
    const int width = ::GetSystemMetrics(SM_CXSMICON);
    const int height = ::GetSystemMetrics(SM_CYSMICON);
    if (width <= 0 || height <= 0) return nullptr;
    std::vector<DWORD> onBlack(static_cast<size_t>(width) * height);
    BITMAPINFO format{};
    format.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    format.bmiHeader.biWidth = width;
    format.bmiHeader.biHeight = -height;
    format.bmiHeader.biPlanes = 1;
    format.bmiHeader.biBitCount = 32;
    void* bits = nullptr;
    const HBITMAP bitmap = ::CreateDIBSection(nullptr, &format, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!bitmap) return nullptr;
    const HDC dc = ::CreateCompatibleDC(nullptr);
    if (!dc) {
        ::DeleteObject(bitmap);
        return nullptr;
    }
    const HGDIOBJ old = ::SelectObject(dc, bitmap);
    auto* pixels = static_cast<DWORD*>(bits);
    std::fill_n(pixels, onBlack.size(), 0);
    const BOOL drewBlack = ::DrawIconEx(dc, 0, 0, icon, width, height, 0, nullptr, DI_NORMAL);
    ::GdiFlush();
    std::copy_n(pixels, onBlack.size(), onBlack.begin());
    std::fill_n(pixels, onBlack.size(), 0x00ffffff);
    const BOOL drewWhite = ::DrawIconEx(dc, 0, 0, icon, width, height, 0, nullptr, DI_NORMAL);
    ::GdiFlush();
    // The difference between the two backgrounds gives the transparency for
    // both alpha-channel icons and older icons with a separate AND mask.
    for (size_t i = 0; i < onBlack.size(); ++i) {
        const int background = static_cast<int>(pixels[i] & 0xff) - static_cast<int>(onBlack[i] & 0xff);
        const DWORD alpha = static_cast<DWORD>(255 - (std::clamp)(background, 0, 255));
        pixels[i] = (alpha << 24) | (onBlack[i] & 0x00ffffff);
    }
    ::SelectObject(dc, old);
    ::DeleteDC(dc);
    if (drewBlack && drewWhite) return bitmap;
    ::DeleteObject(bitmap);
    return nullptr;
}

CContextMenuManager* DocumentContextMenuManager() {
    struct AppAccess : CWinAppEx { using CWinAppEx::m_pContextMenuManager; };
    auto* app = AfxGetApp();
    return app && app->IsKindOf(impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())
        ? static_cast<AppAccess*>(static_cast<CWinAppEx*>(app))->m_pContextMenuManager : nullptr;
}

std::wstring DocumentMenuLabel(const std::wstring& label) {
    std::wstring text;
    for (std::size_t i = 0; i < label.size(); ++i) {
        text += label[i];
        if (label[i] != L'&') continue;
        text += L'&';
        if (i + 1 < label.size() && label[i + 1] == L'&') ++i;
    }
    return text;
}
}

// Symbol: ?m_mapDocIcons@CMFCTabCtrl@@1V?$CMap@IIPEAUHICON__@@PEAU1@@@A
extern "C" DocumentIconMap impl__m_mapDocIcons_CMFCTabCtrl__1V__CMap_IIPEAUHICON____PEAU1___A{};

namespace {
class DocumentMenuIcons {
public:
    explicit DocumentMenuIcons(const std::vector<DocumentMenuEntry>& entries)
        : m_associations(entries.size()) {
        for (size_t i = 0; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            if (!entry.icon) continue;
            const UINT hash = DocumentIconHash(entry.command);
            auto*& bucket = m_buckets[hash % ARRAYSIZE(m_buckets)];
            m_associations[i] = {entry.command, entry.icon, bucket, hash};
            bucket = &m_associations[i];
            ++m_map.count;
        }
        m_map.buckets = m_buckets;
        m_previous = s_active;
        s_active = &m_map;
        ::AcquireSRWLockExclusive(&s_exportLock);
        m_previousExport = s_lastExport;
        s_lastExport = this;
        impl__m_mapDocIcons_CMFCTabCtrl__1V__CMap_IIPEAUHICON____PEAU1___A = m_map;
        ::ReleaseSRWLockExclusive(&s_exportLock);
    }

    ~DocumentMenuIcons() {
        s_active = m_previous;
        ::AcquireSRWLockExclusive(&s_exportLock);
        auto** entry = &s_lastExport;
        while (*entry != this) entry = &(*entry)->m_previousExport;
        *entry = m_previousExport;
        impl__m_mapDocIcons_CMFCTabCtrl__1V__CMap_IIPEAUHICON____PEAU1___A =
            s_lastExport ? s_lastExport->m_map : DocumentIconMap{};
        ::ReleaseSRWLockExclusive(&s_exportLock);
    }

    static const DocumentIconMap* Active() { return s_active; }

private:
    static inline thread_local const DocumentIconMap* s_active = nullptr;
    static inline SRWLOCK s_exportLock = SRWLOCK_INIT;
    static inline DocumentMenuIcons* s_lastExport = nullptr;
    const DocumentIconMap* m_previous = nullptr;
    DocumentMenuIcons* m_previousExport = nullptr;
    DocumentIconMap m_map;
    DocumentIconAssociation* m_buckets[17]{};
    std::vector<DocumentIconAssociation> m_associations;
};
}

void CMFCTabCtrl::EnableTabDocumentsMenu(BOOL enable) {
    auto& state = EnsureTabCtrlState(this);
    if (state.documentsMenu == enable || (TabControlFlag(*this, flatTabOffset) && !TabControlFlag(*this, tabScrollOffset)))
        return;
    state.documentsMenu = enable;
    TabControlFlag(*this, documentsMenuOffset) = enable;
    RefreshTabs();
}

HICON CMFCTabCtrl::GetDocumentIcon(UINT command) {
    const auto* map = DocumentMenuIcons::Active();
    if (!map) return nullptr;
    const UINT hash = DocumentIconHash(command);
    for (auto* entry = map->buckets[hash % map->bucketCount]; entry; entry = entry->next) {
        if (entry->command == command) return entry->icon;
    }
    return nullptr;
}

// Symbol: ?GetDocumentIcon@CMFCTabCtrl@@SAPEAUHICON__@@I@Z
extern "C" HICON MS_ABI impl__GetDocumentIcon_CMFCTabCtrl__SAPEAUHICON____I_Z(UINT command) {
    return CMFCTabCtrl::GetDocumentIcon(command);
}

void CMFCTabCtrl::OnShowTabDocumentsMenu(CPoint point) {
    const HWND control = m_hWnd;
    auto* manager = DocumentContextMenuManager();
    const auto* state = FindTabCtrlState(this);
    if (!control || !manager || !state) return;

    std::vector<DocumentMenuEntry> entries;
    for (size_t i = 0; i < state->tabs.size(); ++i) {
        const auto& tab = state->tabs[i];
        if (!tab.visible || tab.temporary || !tab.window || !tab.window->m_hWnd) continue;
        entries.push_back({UINT(-100) - static_cast<UINT>(i), tab.id, DocumentMenuLabel(tab.label),
            tab.window->m_hWnd, tab.windowIcon});
    }
    if (entries.empty()) return;
    std::stable_sort(entries.begin(), entries.end(), [](const auto& left, const auto& right) {
        return _wcsicmp(left.label.c_str(), right.label.c_str()) < 0;
    });

    using MenuBitmap = std::unique_ptr<std::remove_pointer_t<HBITMAP>, decltype(&::DeleteObject)>;
    std::vector<MenuBitmap> bitmaps;
    bitmaps.reserve(entries.size());
    CMenu menu;
    if (!menu.CreatePopupMenu()) return;
    for (auto& item : entries) {
        if (!item.icon && ::IsWindow(item.window)) {
            item.icon = reinterpret_cast<HICON>(::SendMessageW(item.window, WM_GETICON, ICON_SMALL, 0));
            if (!item.icon) item.icon = reinterpret_cast<HICON>(::GetClassLongPtrW(item.window, GCLP_HICONSM));
        }
        if (!menu.AppendMenu(MF_STRING, item.command, item.label.c_str()) || !item.icon) continue;
        MenuBitmap bitmap(DocumentMenuBitmap(item.icon), &::DeleteObject);
        if (!bitmap) continue;
        MENUITEMINFOW information{sizeof(MENUITEMINFOW)};
        information.fMask = MIIM_BITMAP;
        information.hbmpItem = bitmap.get();
        if (::SetMenuItemInfoW(menu.m_hMenu, item.command, FALSE, &information))
            bitmaps.push_back(std::move(bitmap));
    }
    if (CWnd::FromHandlePermanent(control) != this) return;

    manager = DocumentContextMenuManager();
    if (!manager) return;
    UINT command;
    {
        const DocumentMenuIcons icons(entries);
        command = manager->TrackPopupMenu(menu.m_hMenu, point.x, point.y, this, FALSE);
    }
    if (CWnd::FromHandlePermanent(control) != this) return;
    const auto selected = std::find_if(entries.begin(), entries.end(), [command](const auto& entry) {
        return entry.command == command;
    });
    state = FindTabCtrlState(this);
    if (selected == entries.end() || !state) return;
    const auto tab = std::find_if(state->tabs.begin(), state->tabs.end(), [&selected](const auto& candidate) {
        return candidate.id == selected->tabId && candidate.window && candidate.window->m_hWnd == selected->window;
    });
    if (tab != state->tabs.end()) SelectTabAndFocus(*this, static_cast<int>(tab - state->tabs.begin()));
}

// Symbol: ?OnShowTabDocumentsMenu@CMFCTabCtrl@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnShowTabDocumentsMenu_CMFCTabCtrl__UEAAXVCPoint___Z(
    CMFCTabCtrl* pThis, CPoint point) {
    if (pThis) pThis->CMFCTabCtrl::OnShowTabDocumentsMenu(point);
}

// Retail RVA 0x138d60 (mfc140 ANSI twin) first calls CWnd::Default, then
// computes button/tab sizing, conditionally clears scroll offsets, and
// synchronizes scrollbars using unmodeled fields. Keep the known default call.
// Symbol: ?OnSize@CMFCTabCtrl@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCTabCtrl__IEAAXIHH_Z(
    CMFCTabCtrl* pThis, unsigned int nType, int cx, int cy)
{
    if (pThis) pThis->OnSize(nType, cx, cy);
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

// Symbol: ?OnUpdateToolTips@CMFCTabCtrl@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnUpdateToolTips_CMFCTabCtrl__IEAA_J_K_J_Z(
    CMFCTabCtrl* pThis, WPARAM types, LPARAM param)
{
    return pThis ? pThis->OnUpdateToolTips(types, param) : 0;
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
    if (pThis) pThis->RecalcLayout();
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
    return pThis && pThis->ActivateTab(iTab);
}

// Symbol: ?SetActiveTabBoldFont@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetActiveTabBoldFont_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, BOOL enable)
{
    if (pThis) pThis->SetActiveTabBoldFont(enable);
}

// Symbol: ?SetDrawFrame@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDrawFrame_CMFCTabCtrl__QEAAXH_Z(
    CMFCTabCtrl* pThis, BOOL draw)
{
    if (pThis) pThis->SetDrawFrame(draw);
}

// Symbol: ?SetImageList@CMFCTabCtrl@@UEAAHIHK@Z
extern "C" int MS_ABI impl__SetImageList_CMFCTabCtrl__UEAAHIHK_Z(
    CMFCTabCtrl* pThis, unsigned int nBitmapID, int cx, unsigned long clrTransparent)
{
    if (!pThis) return FALSE;
    return impl__SetImageList_CMFCBaseTabCtrl__UEAAHIHK_Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), nBitmapID, cx, clrTransparent);
}

// Symbol: ?SetImageList@CMFCTabCtrl@@UEAAHPEAU_IMAGELIST@@@Z
extern "C" int MS_ABI impl__SetImageList_CMFCTabCtrl__UEAAHPEAU_IMAGELIST___Z(
    CMFCTabCtrl* pThis, HIMAGELIST hImageList)
{
    if (!pThis) return FALSE;
    return impl__SetImageList_CMFCBaseTabCtrl__UEAAHPEAU_IMAGELIST___Z(
        static_cast<CMFCBaseTabCtrl*>(pThis), hImageList);
}

// Symbol: ?SetResizeMode@CMFCTabCtrl@@QEAAXW4ResizeMode@1@@Z
extern "C" void MS_ABI impl__SetResizeMode_CMFCTabCtrl__QEAAXW4ResizeMode_1__Z(
    CMFCTabCtrl* pThis, int mode)
{
    if (pThis) pThis->SetResizeMode(static_cast<CMFCTabCtrl::ResizeMode>(mode));
}

// STUB: Retail RVA 0x13d080 (mfc140 ANSI twin) configures the internal scroll
// and documents buttons from style flags and child-control objects not modeled.
// Symbol: ?SetScrollButtons@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__SetScrollButtons_CMFCTabCtrl__IEAAXXZ(CMFCTabCtrl* pThis)
{
    (void)pThis;
}

// Symbol: ?SetTabMaxWidth@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetTabMaxWidth_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, int width)
{
    if (pThis) pThis->SetTabMaxWidth(width);
}

// Symbol: ?StopResize@CMFCTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__StopResize_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, BOOL cancel)
{
    if (pThis) pThis->StopResize(cancel);
}

// Symbol: ?SwapTabs@CMFCTabCtrl@@UEAAXHH@Z
extern "C" void MS_ABI impl__SwapTabs_CMFCTabCtrl__UEAAXHH_Z(CMFCTabCtrl* pThis, int first, int second)
{
    if (pThis) pThis->SwapTabs(first, second);
}

// Symbol: ?SynchronizeScrollBar@CMFCTabCtrl@@QEAAHPEAUtagSCROLLINFO@@@Z
extern "C" int MS_ABI impl__SynchronizeScrollBar_CMFCTabCtrl__QEAAHPEAUtagSCROLLINFO___Z(
    CMFCTabCtrl* pThis, SCROLLINFO* info)
{
    return pThis && pThis->SynchronizeScrollBar(info);
}
// STUB: Retail RVA 0x13ce50 (mfc140 ANSI twin) updates the internal scroll
// and documents buttons from tab/style state and child controls not modeled.
// Symbol: ?UpdateScrollButtonsState@CMFCTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateScrollButtonsState_CMFCTabCtrl__IEAAXXZ(
    CMFCTabCtrl* pThis)
{
    (void)pThis;
}
