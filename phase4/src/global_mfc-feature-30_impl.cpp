// CMFCBaseTabCtrl exports, wave "mfc-feature-30".
//
// Each function was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump, see /tmp/openmfc_remaining/wave2_mfc-feature-30.txt). The class in
// include/openmfc/afxmfc.h is modeled as a thin CWnd subclass with 128 bytes
// of opaque padding, so none of the retail member offsets the disassembly
// touches (the internal tab array +0x108/+0x110, tab count +0x150, active tab
// +0x154, per-tab records whose +0x40..+0x54 flags live behind that array,
// the drag state +0x1dc/+0x1e0/+0x344/+0x348, the in-place edit window
// +0x228/+0x220/+0x234, the embedded CImageList +0x160 and image members
// +0x170/+0x178/+0x17c, the style/color members +0x204/+0x208/+0x1f8/+0xf8,
// the accessibility block +0x360, ...) are named in the headers. Following
// the wave-1/2 house rules, bodies that would have to read or write such
// unmodeled state are emitted as type-correct conservative versions that keep
// the safe visible side effects (bounds checks through the declared
// GetTabsCount()/GetTabWnd(), base handler delegation, capture release, tab
// removal via the public RemoveTab path, WM_PAINT posting, relayout redraws)
// and return the retail terminal values, with a `// TODO(clean-room)` marker.
// No member offsets are invented and no header files are touched.
//
// The side-table state backing the declared CMFCBaseTabCtrl methods
// (GetTabsCount/GetActiveTab/GetTabWnd/RemoveTab, established in mfccore.cpp)
// is the only tab storage OpenMFC has; the helpers that reach it
// (EnsureTabCtrlState/FindTabCtrlState) are not declared in any header, so
// this translation unit relies exclusively on the public methods.
//
// Cross-file impl helpers referenced here (defined in thunks.cpp/wincore.cpp):
//   impl__Default_CWnd__IEAA_JXZ
//   impl__OnDestroy_CWnd__IEAAXXZ
//   impl__OnSysColorChange_CWnd__IEAAXXZ
//   impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include <windows.h>

#include "openmfc/afx.h"
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
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
    CWnd* pThis, MSG* pMsg);

//=============================================================================
// CMFCBaseTabCtrl::IsTabIconOnly
//=============================================================================
// Retail (0x180016d30): bounds-checks `0 <= iTab < tabs count (+0x150)` (and
// against the array size +0x110) and returns the "icon only" flag at +0x50 of
// the tab record at (+0x108 + iTab*8); 0 when out of bounds. The per-tab
// records are unmodeled, so the default (flag clear) is returned.
// Symbol: ?IsTabIconOnly@CMFCBaseTabCtrl@@UEBAHH@Z
extern "C" int MS_ABI impl__IsTabIconOnly_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- retail reads the per-tab
    // "icon only" flag at +0x50 of the tab record (tab array +0x108/+0x110);
    // the per-tab records are not modeled, so no tab can report TRUE.
    return FALSE;
}

//=============================================================================
// CMFCBaseTabCtrl::IsTabVisible
//=============================================================================
// Retail (0x180017000): identical shape to IsTabIconOnly but returns the
// per-tab "visible" flag at +0x48.
// Symbol: ?IsTabVisible@CMFCBaseTabCtrl@@UEBAHH@Z
extern "C" int MS_ABI impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- retail reads the per-tab
    // "visible" flag at +0x48 of the tab record (tab array +0x108/+0x110);
    // the per-tab records are not modeled, so no tab can report TRUE.
    return FALSE;
}

//=============================================================================
// CMFCBaseTabCtrl::MoveTab
//=============================================================================
// Retail (0x180018ec0): validates both indices against the tab array
// (+0x100/+0x110) and the active tab (+0x154), then reorders the per-tab
// records (a CMFCBaseTabInfo array at +0x108) honoring the +0x200 swap flag.
// The array and flags are unmodeled.
// Symbol: ?MoveTab@CMFCBaseTabCtrl@@UEAAXHH@Z
extern "C" void MS_ABI impl__MoveTab_CMFCBaseTabCtrl__UEAAXHH_Z(
    CMFCBaseTabCtrl* pThis, int /*i1*/, int /*i2*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the internal tab array
    // (+0x108/+0x110), the active tab (+0x154) and the +0x200 swap flag;
    // not modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::OnCancelMode
//=============================================================================
// Retail (0x180018190): calls CWnd::Default, then when the drag flags
// (+0x348/+0x344) are set resets them and posts WM_PAINT, then releases the
// capture when a tab drag is active (+0x1dc >= 0) and tears the in-place edit
// window down (+0x228/+0x220/+0x234). The conservative version keeps the
// base delegation and the capture release.
// Symbol: ?OnCancelMode@CMFCBaseTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCBaseTabCtrl__IEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): partially transcribed -- the drag-cancel resets
    // (+0x344/+0x348, WM_PAINT post) and the edit-window teardown
    // (+0x220/+0x228/+0x234) are not modeled.
    if (::GetCapture() == pThis->GetSafeHwnd()) {
        ::ReleaseCapture();
    }
}

//=============================================================================
// CMFCBaseTabCtrl::OnCreate
//=============================================================================
// Retail (0x180019ff0): calls CWnd::Default first (-1 aborts), resolves the
// parent frame (AFXGetParentFrame, stored at +0xe8) and, when the thread
// state allows OLE drop targets, registers the COleDropTarget embedded at
// +0x290. Returns 0 on success.
// Symbol: ?OnCreate@CMFCBaseTabCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCBaseTabCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCBaseTabCtrl* pThis, CREATESTRUCTW* /*lpCreateStruct*/)
{
    if (!pThis) return -1;
    const __int64 nResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    if (nResult == -1) return -1;
    // TODO(clean-room): partially transcribed -- the COleDropTarget at +0x290
    // and the parent-frame member at +0xe8 are not modeled.
    return 0;
}

//=============================================================================
// CMFCBaseTabCtrl::OnDestroy
//=============================================================================
// Retail (0x1800182e0): calls a virtual at vslot 0x588 (internal handler-list
// teardown) and tail-jumps to CWnd::OnDestroy (0x18028b840). The virtual is
// unmodeled; the base call is kept.
// Symbol: ?OnDestroy@CMFCBaseTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCBaseTabCtrl__IEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- retail first calls the
    // vslot-0x588 virtual (internal handler-list cleanup); not modeled.
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}

//=============================================================================
// CMFCBaseTabCtrl::OnLButtonDblClk
//=============================================================================
// Retail (0x180017a30): calls CWnd::Default, then, when the in-place edit
// flag (+0x230) is set, hit-tests the tab (vslot 0x440/0x428) and routes
// WM_LBUTTONDBLCLK to the child tab window (vslot 0x400). The edit flag and
// tab geometry are unmodeled; the base handler call is kept.
// Symbol: ?OnLButtonDblClk@CMFCBaseTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): not transcribed -- needs the +0x230 in-place-edit
    // flag and the tab hit-test/routing virtuals (vslot 0x440/0x428/0x400).
}

//=============================================================================
// CMFCBaseTabCtrl::OnLButtonDown
//=============================================================================
// Retail (0x180017660): calls CWnd::Default after clearing +0x340, then
// either starts a capture-drag when the point hits the +0x34c rect (posts
// WM_PAINT, sets +0x348) or records the clicked tab (+0x1dc) and walks the
// vslot-0x5a8 detach path. The rect/flag members are unmodeled; the base
// handler call is kept.
// Symbol: ?OnLButtonDown@CMFCBaseTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): not transcribed -- needs the +0x340/+0x348/+0x34c
    // drag members and the tab hit-test virtuals (vslot 0x440/0x5a8).
}

//=============================================================================
// CMFCBaseTabCtrl::OnLButtonUp
//=============================================================================
// Retail (0x180017ae0): when the drag flag (+0x348) is set resets
// +0x344/+0x348 and posts WM_PAINT, then resolves the drag end (0x1802c7258,
// release capture, +0x1dc = -1, InvalidateTab) and finally activates the tab
// under the cursor via the vslot-0x2d8/0x4d0 virtuals. The drag state is
// unmodeled; the capture release is the visible side effect that is kept.
// Symbol: ?OnLButtonUp@CMFCBaseTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the drag-state flags
    // (+0x344/+0x348/+0x1dc/+0x1e0), the WM_PAINT post and the tab-activation
    // virtuals (vslot 0x2d8/0x4d0/0x438) are not modeled.
    if (::GetCapture() == pThis->GetSafeHwnd()) {
        ::ReleaseCapture();
    }
}

//=============================================================================
// CMFCBaseTabCtrl::OnMouseMove
//=============================================================================
// Retail (0x180017e50): calls CWnd::Default, recomputes the +0x344 hit value
// from the +0x34c rect (posting WM_PAINT when it changed) and, while a drag is
// active (+0x1dc >= 0 and < +0x1e0 with +0x234 clear), runs the drag logic.
// The rect and drag members are unmodeled; the base handler call is kept.
// Symbol: ?OnMouseMove@CMFCBaseTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): not transcribed -- needs the +0x344/+0x34c rect state
    // and the +0x1dc/+0x1e0/+0x234 drag flags; not modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::OnNeedTipText
//=============================================================================
// Retail (0x18001ab10): when the notify hwnd matches one of the tooltip
// helpers (+0x1c0/+0x1b8) it loads the "Tab" string resource 0x3ea0 into the
// shared tooltip buffer (0x1803c1448) and copies it into the NMHDR-derived
// text buffer (third param); returns 0 for unmatched windows. The tooltip
// helpers and shared buffer are unmodeled.
// Symbol: ?OnNeedTipText@CMFCBaseTabCtrl@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCBaseTabCtrl__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*id*/, NMHDR* /*pNMHDR*/,
    __int64* /*pText*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the tooltip helpers
    // (+0x1c0/+0x1b8), the shared tooltip text buffer (0x1803c1448) and the
    // "Tab" string resource; none are modeled.
    return FALSE;
}

//=============================================================================
// CMFCBaseTabCtrl::OnRButtonDown
//=============================================================================
// Retail (0x180017880): calls CWnd::Default, then, when the +0xf4 flag is
// set, hit-tests the tab (vslot 0x440), updates +0x338/+0x33c and pops the
// context menu through the tab virtuals. The flag and menu state are
// unmodeled; the base handler call is kept.
// Symbol: ?OnRButtonDown@CMFCBaseTabCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CMFCBaseTabCtrl__IEAAXIVCPoint___Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): not transcribed -- needs the +0xf4 flag, the tab
    // hit-test virtual (vslot 0x440) and the +0x338/+0x33c menu state.
}

//=============================================================================
// CMFCBaseTabCtrl::OnSetAccData
//=============================================================================
// Retail (0x18001acc0): reinitializes the embedded CAccessibilityData block at
// +0x360, finds the tab for the given index through the tab array
// (+0x108/+0x110), feeds it to the vslot-0x5b8 SetACCData virtual and updates
// the active-tab ACC state. The +0x360 block and tab array are unmodeled.
// Symbol: ?OnSetAccData@CMFCBaseTabCtrl@@MEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCBaseTabCtrl__MEAAHJ_Z(
    CMFCBaseTabCtrl* pThis, long /*lVal*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the embedded accessibility
    // block at +0x360 and the vslot-0x5b8 per-tab SetACCData virtual; not
    // modeled.
    return FALSE;
}

//=============================================================================
// CMFCBaseTabCtrl::OnSysColorChange
//=============================================================================
// Retail (0x18001a560): calls CWnd::OnSysColorChange (0x18028f320) and then
// tail-calls a virtual at vslot 0x5a0 (repaint). The virtual is unmodeled;
// the base call is kept.
// Symbol: ?OnSysColorChange@CMFCBaseTabCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCBaseTabCtrl__IEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    impl__OnSysColorChange_CWnd__IEAAXXZ(pThis);
    // TODO(clean-room): partially transcribed -- retail then tail-calls the
    // vslot-0x5a0 virtual (strip repaint); not modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::OnWindowPosChanged
//=============================================================================
// Retail (0x18001aca0): calls CWnd::Default and stores 1 at +0x340 (layout
// dirty flag). The +0x340 member is not declared; the base call is kept.
// Symbol: ?OnWindowPosChanged@CMFCBaseTabCtrl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCBaseTabCtrl__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCBaseTabCtrl* pThis, WINDOWPOS* /*pWndPos*/)
{
    if (!pThis) return;
    (void)impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): partially transcribed -- retail also stores 1 at +0x340
    // (layout-dirty flag); that member is not declared.
}

//=============================================================================
// CMFCBaseTabCtrl::PreTranslateMessage
//=============================================================================
// Retail (0x1800196d0): while the in-place edit window (+0x228) is open it
// filters its keys (VK_RETURN/VK_ESCAPE destroy the editor), otherwise every
// message falls through to CWnd::PreTranslateMessage (0x18028bc00). The edit
// window is unmodeled, so the base handler is the faithful conservative path.
// Symbol: ?PreTranslateMessage@CMFCBaseTabCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCBaseTabCtrl__UEAAHPEAUtagMSG___Z(
    CMFCBaseTabCtrl* pThis, MSG* pMsg)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- retail intercepts
    // VK_RETURN/VK_ESCAPE while the in-place edit window (+0x228) exists and
    // routes other keys through it (vslot 0x408); that window is not modeled.
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

//=============================================================================
// CMFCBaseTabCtrl::RemoveAllTabs
//=============================================================================
// Retail (0x180016450): sets the active tab (+0x154) to -1, walks the tab
// array from the end deleting every CMFCBaseTabInfo record (DelTool on the
// +0x1b8 tooltip, destroying child windows per the +0x1e4 flag), clears the
// array objects (+0x100/+0x128) and finishes with the vslot-0x4e8,
// vslot-0x318 and vslot-0x4f0(-1) relayout/redraw calls. The internal arrays
// are unmodeled; the side-table is cleared through the declared public
// RemoveTab path and the relayout redraw (vslot 0x318) is kept.
// Symbol: ?RemoveAllTabs@CMFCBaseTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__RemoveAllTabs_CMFCBaseTabCtrl__UEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    while (pThis->GetTabsCount() > 0) {
        pThis->RemoveTab(0);
    }
    // TODO(clean-room): partially transcribed -- the tooltip DelTool calls,
    // the tab-record destruction and the internal array teardown
    // (+0x100/+0x108/+0x110/+0x128) are not modeled; the side-table is
    // emptied through the public RemoveTab path instead.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}

//=============================================================================
// CMFCBaseTabCtrl::RenameTab
//=============================================================================
// Retail (0x180019870): early-returns 0 when no in-place edit window (+0x228)
// exists; otherwise it reads the editor text (CWnd::GetWindowTextW,
// 0x18028be00) into a CString and hands it to the vslot-0x410 virtual. Since
// the edit window is never present in OpenMFC, 0 is the faithful terminal
// value for every call.
// Symbol: ?RenameTab@CMFCBaseTabCtrl@@UEAAHXZ
extern "C" int MS_ABI impl__RenameTab_CMFCBaseTabCtrl__UEAAHXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- retail returns 0 when the
    // in-place edit window (+0x228) is absent (always the case here) and
    // otherwise renames the tab via the vslot-0x410 virtual.
    return FALSE;
}

//=============================================================================
// CMFCBaseTabCtrl::ResetImageList
//=============================================================================
// Retail (0x180019de0): calls the vslot-0x2f8 virtual, nulls the stored image
// list handle (+0x170) and tail-calls CMapPtrToPtr::RemoveAll (0x18001ba40)
// on the map at +0x180. The image members and map are unmodeled.
// Symbol: ?ResetImageList@CMFCBaseTabCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__ResetImageList_CMFCBaseTabCtrl__QEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the vslot-0x2f8 virtual, the
    // +0x170 image handle and the +0x180 image map; not modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::Serialize
//=============================================================================
// Retail (0x180018860): serializes the internal options block (+0x248, via
// 0x18001bcb0) and, per CArchive direction, reads or writes the tab array
// (+0x108/+0x110) as a CArray<CMFCBaseTabInfo>::Serialize stream plus the
// count and active-tab integers. Neither the block nor the array is modeled,
// so no archive traffic is emitted (matching a control with no tabs).
// Symbol: ?Serialize@CMFCBaseTabCtrl@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCBaseTabCtrl__UEAAXAEAVCArchive___Z(
    CMFCBaseTabCtrl* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- retail archives the +0x248 options
    // block and the CArray<CMFCBaseTabInfo> tab array (+0x108/+0x110);
    // neither is modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::SetACCData
//=============================================================================
// Retail (0x18001b100): zeroes the numeric CAccessibilityData fields, copies
// the tab-name string (CMFCTabInfo m_strName at +0x8) into m_strAccName,
// loads a string resource into m_strAccDefAction, sets m_nAccRole = 0x25,
// m_nAccHit = 1, ORs STATE_SYSTEM_FOCUSED (0x2) into m_bAccState when
// bSelected, copies the tab rect (CMFCTabInfo +0x20) into
// m_rectAccLocation, converts it to screen coordinates (CWnd::ClientToScreen)
// and returns 1. CMFCTabInfo is not declared in OpenMFC, so the pointer is
// taken as void*; the numeric fields are filled, the string/rect reads are
// skipped.
// Symbol: ?SetACCData@CMFCBaseTabCtrl@@MEAAHPEAVCMFCTabInfo@@AEAVCAccessibilityData@@H@Z
extern "C" int MS_ABI impl__SetACCData_CMFCBaseTabCtrl__MEAAHPEAVCMFCTabInfo__AEAVCAccessibilityData__H_Z(
    CMFCBaseTabCtrl* pThis, void* /*pTabInfo*/, CAccessibilityData& accData,
    int bSelected)
{
    if (!pThis) return FALSE;

    // Retail zeroes the whole numeric tail of the block first (movq/xorps to
    // +0x30/+0x34/+0x38/+0x3c/+0x4c), then fills the role/hit/state fields.
    accData.m_nAccRole = 0;
    accData.m_bAccState = 0;
    accData.m_nAccHit = 0;
    accData.m_rectAccLocation = CRect(0, 0, 0, 0);
    accData.m_ptAccHit = CPoint(0, 0);

    accData.m_nAccRole = 0x25;          // ROLE_SYSTEM_PAGETAB
    accData.m_nAccHit = 1;
    if (bSelected) {
        accData.m_bAccState |= 0x2;     // STATE_SYSTEM_FOCUSED
    }
    // TODO(clean-room): partially transcribed -- the name/action strings come
    // from the unmodeled CMFCTabInfo record (m_strName at +0x8) and a string
    // resource, and the tab rect (CMFCTabInfo +0x20) is converted to screen
    // coordinates; only the numeric fields are filled.
    return TRUE;
}

//=============================================================================
// CMFCBaseTabCtrl::SetActiveTabColor
//=============================================================================
// Retail (0x180019980): stores the color at +0x204, deletes the previous
// solid brush (CGdiObject at +0x210, DeleteObject 0x1802a3f60) and creates/
// attaches a new one from the vslot-0x470 color virtual. The +0x204 member
// and the +0x210 brush object are unmodeled.
// Symbol: ?SetActiveTabColor@CMFCBaseTabCtrl@@UEAAXK@Z
extern "C" void MS_ABI impl__SetActiveTabColor_CMFCBaseTabCtrl__UEAAXK_Z(
    CMFCBaseTabCtrl* pThis, unsigned long /*color*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x204 color member and
    // the +0x210 CGdiObject brush; not modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::SetActiveTabTextColor
//=============================================================================
// Retail (0x180019970) is a single store: `mov %edx,0x208(%rcx); ret`. The
// +0x208 member is not declared, so the store is skipped.
// Symbol: ?SetActiveTabTextColor@CMFCBaseTabCtrl@@UEAAXK@Z
extern "C" void MS_ABI impl__SetActiveTabTextColor_CMFCBaseTabCtrl__UEAAXK_Z(
    CMFCBaseTabCtrl* pThis, unsigned long /*color*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- retail stores the color at
    // +0x208 (m_activeTabTextColor); that member is not declared.
}

//=============================================================================
// CMFCBaseTabCtrl::SetAutoColors
//=============================================================================
// Retail (0x18001a590): resets every tab's background color (+0x44) to -1,
// clears the internal auto-color array (+0x300 via 0x180015034), then copies
// the caller's CArray<unsigned long> (m_pData +0x8 / m_nSize +0x10) into it
// and marks the color state (+0x32c = 1) before the vslot-0x5a0 repaint and
// WM_PAINT. The per-tab records and the +0x300 array are unmodeled.
// Symbol: ?SetAutoColors@CMFCBaseTabCtrl@@QEAAXAEBV?$CArray@KK@@@Z
extern "C" void MS_ABI impl__SetAutoColors_CMFCBaseTabCtrl__QEAAXAEBV__CArray_KK___Z(
    CMFCBaseTabCtrl* pThis, const CArray<unsigned long, unsigned long>& /*colors*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the per-tab color records
    // (+0x44) and the internal auto-color array (+0x300); not modeled.
}

//=============================================================================
// CMFCBaseTabCtrl::SetDrawNoPrefix
//=============================================================================
// Retail (0x180019fc0): stores the no-prefix flag at +0x1f8 and, when the
// second argument is set, posts WM_PAINT to the window. The +0x1f8 member is
// not declared; the WM_PAINT posting is kept.
// Symbol: ?SetDrawNoPrefix@CMFCBaseTabCtrl@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetDrawNoPrefix_CMFCBaseTabCtrl__QEAAXHH_Z(
    CMFCBaseTabCtrl* pThis, int /*bDrawNoPrefix*/, int bPostPaint)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x1f8 no-prefix flag
    // (m_bDrawNoPrefix) has no declared member; the WM_PAINT post is kept.
    if (bPostPaint) {
        HWND hWnd = pThis->GetSafeHwnd();
        if (hWnd) {
            ::PostMessageW(hWnd, WM_PAINT, 0, 0);
        }
    }
}

//=============================================================================
// CMFCBaseTabCtrl::SetImageList (resource-id overload)
//=============================================================================
// Retail (0x180019b50): loads the bitmap/icon resource, destroys the existing
// CImageList embedded at +0x160 (DeleteImageList 0x180296c80), creates a new
// one sized from the bitmap and stores it in that object; returns 0 when the
// resource cannot be loaded. The embedded CImageList object is unmodeled, so
// no image list can be created.
// Symbol: ?SetImageList@CMFCBaseTabCtrl@@UEAAHIHK@Z
extern "C" int MS_ABI impl__SetImageList_CMFCBaseTabCtrl__UEAAHIHK_Z(
    CMFCBaseTabCtrl* pThis, unsigned int /*nBitmapID*/, int /*cx*/,
    unsigned long /*clrTransparent*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the embedded CImageList at
    // +0x160 (LoadBitmap/ImageList_Create from the resource); not modeled.
    return FALSE;
}

//=============================================================================
// CMFCBaseTabCtrl::SetImageList (HIMAGELIST overload)
//=============================================================================
// Retail (0x180019cc0): destroys the existing CImageList at +0x160, resolves
// the handle through CImageList::FromHandle (0x180296cb0), stores the size
// (+0x178/+0x17c) and handle (+0x170), triggers the vslot-0x300 relayout and
// returns 1. The image members are unmodeled; the terminal TRUE is kept.
// Symbol: ?SetImageList@CMFCBaseTabCtrl@@UEAAHPEAU_IMAGELIST@@@Z
extern "C" int MS_ABI impl__SetImageList_CMFCBaseTabCtrl__UEAAHPEAU_IMAGELIST___Z(
    CMFCBaseTabCtrl* pThis, HIMAGELIST hImageList)
{
    if (!pThis) return FALSE;
    if (!hImageList) return FALSE;   // retail fails/asserts on NULL here
    // TODO(clean-room): partially transcribed -- the stored image-list
    // members (+0x160 embedded CImageList, +0x170 handle, +0x178/+0x17c size)
    // are not modeled, so the handle is not retained.
    return TRUE;
}

//=============================================================================
// CMFCBaseTabCtrl::SetLocation
//=============================================================================
// Retail (0x18001a6a0): stores the location enum at +0xf8, runs the vslot-0x318
// relayout and posts a registered message (0x185) to the window. The +0xf8
// member is not declared; the relayout redraw (vslot 0x318) is kept.
// Symbol: ?SetLocation@CMFCBaseTabCtrl@@UEAAXW4Location@1@@Z
extern "C" void MS_ABI impl__SetLocation_CMFCBaseTabCtrl__UEAAXW4Location_1__Z(
    CMFCBaseTabCtrl* pThis, int /*location*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0xf8 location member
    // (m_location) is not declared, and the registered-message post (0x185)
    // is skipped.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}

//=============================================================================
// CMFCBaseTabCtrl::SetTabBkColor
//=============================================================================
// Retail (0x180019a50): bounds-checks `0 <= iTab < tabs count (+0x150)` and
// stores the color at +0x44 of the tab record; returns 1 when applied, 0 when
// out of bounds. The per-tab records are unmodeled; the bounds gate and the
// terminal values are kept.
// Symbol: ?SetTabBkColor@CMFCBaseTabCtrl@@UEAAHHK@Z
extern "C" int MS_ABI impl__SetTabBkColor_CMFCBaseTabCtrl__UEAAHHK_Z(
    CMFCBaseTabCtrl* pThis, int iTab, unsigned long /*color*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab background color
    // (+0x44 of the tab record, tab array +0x108/+0x110) is not modeled.
    return TRUE;
}

//=============================================================================
// CMFCBaseTabCtrl::SetTabHicon
//=============================================================================
// Retail (0x180016bf0): bounds-checks, destroys the previous icon (+0x18 of
// the tab record) and copies the new one (0x1802c7260), stores it at +0x18
// and resets +0x10 to -1, then reconciles the image-size members
// (+0x178/+0x17c) with AFX_GLOBAL_DATA and runs the vslot-0x300 relayout;
// returns 1 when applied, 0 when out of bounds. The per-tab records are
// unmodeled; the bounds gate and the terminal values are kept.
// Symbol: ?SetTabHicon@CMFCBaseTabCtrl@@UEAAHHPEAUHICON__@@@Z
extern "C" int MS_ABI impl__SetTabHicon_CMFCBaseTabCtrl__UEAAHHPEAUHICON_____Z(
    CMFCBaseTabCtrl* pThis, int iTab, HICON /*hIcon*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab icon record
    // (+0x18/+0x10 of the tab struct) and the image-size members
    // (+0x178/+0x17c) are not modeled.
    return TRUE;
}
