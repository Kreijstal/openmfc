// CMFCBaseTabCtrl — OpenMFC implementation.
// Sources: ctrl_mfcbasetabctrl_addtab.cpp, global_cmfcbasetabctrl.cpp, global_mfc-feature-30_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCBaseTabCtrlSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCBaseTabCtrl exports.
//
// All 57 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class in include/openmfc/afxmfc.h is a thin CWnd subclass with 128 bytes
// of opaque padding, so none of the retail member offsets the disassembly
// touches are named in the headers: the internal tab array +0x108/+0x110, the
// tab count +0x150, the active tab +0x154, the per-tab CMFCBaseTabInfo records
// (label CString +0x8, icon index +0x10, hicon +0x18, tab rect +0x20, ID +0x3c,
// text color +0x40, bk color +0x44, visible flag +0x48, icon-only flag +0x50,
// detach flag +0x58, full width +0x38), the image-list state +0x160..+0x180,
// the tooltip helper +0x1b8/+0x1c8, the flags +0x1f4/+0x1f8/+0x230/+0x234, the
// drag state +0x238/+0x240, the in-place edit window +0x228/+0x230, the
// auto-color array +0x300..+0x32c, the accessibility block +0x360..+0x3a8 and
// the registered "change/changing active tab" message ids.
//
// Bodies that only read such unmodeled state are emitted as type-correct
// conservative versions that keep the safe visible side effects available
// through the declared public API (GetTabsCount / GetActiveTab / GetTabWnd /
// GetTabNumberToDetach / RemoveTab), the bounds checks, the base IAccessible
// validation and the redraws, and return the retail terminal values, with a
// `// TODO(clean-room)` marker.  No member offsets are invented and no header
// files are touched.
//
// The side table backing the declared methods (GetTabsCount / GetActiveTab /
// GetTabWnd, established in mfccore.cpp) is the only tab storage OpenMFC has.
// GetActiveWnd and GetTabNumberToDetach are fully transcribed through it;
// GetFirstVisibleTab / GetLastVisibleTab / GetVisibleTabsNum scan it through
// the sibling IsTabVisible (which reports every tab invisible because the
// per-tab +0x48 flag is unmodeled), matching the retail structure exactly.
//
// ABI note for GetMaxWindowSize: the retail MSVC x64 convention for a member
// function returning CSize passes `this` in %rcx and the hidden CSize* return
// slot in %rdx (verified in the retail body, which writes the accumulated size
// through %rdx and returns that pointer in %rax).  That impl is therefore
// declared as (pThis, pRet) -- pThis first -- unlike the pRet-first convention
// used for free-standing helper chunks, so real MSVC callers get the correct
// register assignment.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleacc.h>

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" int MS_ABI impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab);
extern "C" int MS_ABI impl__GetTabID_CMFCBaseTabCtrl__QEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab);
extern "C" CWnd* MS_ABI impl__GetFirstVisibleTab_CMFCBaseTabCtrl__UEAAPEAVCWnd__HAEAH_Z(
    const CMFCBaseTabCtrl* pThis, int iStartTab, int& iTab);
extern "C" int MS_ABI impl__GetTabRect_CMFCBaseTabCtrl__UEBAHHAEAVCRect___Z(
    const CMFCBaseTabCtrl* pThis, int iTab, CRect& rect);
extern "C" unsigned long MS_ABI impl__GetTabBkColor_CMFCBaseTabCtrl__UEBAKH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab);

//=============================================================================
// CMFCBaseTabCtrl - thin CWnd subclass; the internal tab storage, image-list
// object, drag/dock state and the relayout/repaint virtuals are unmodeled
// (see the file header). The declared public API (GetTabsCount/RemoveTab) and
// CWnd::RedrawWindow are the only touch points.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::ApplyRestoredTabInfo
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::CleanUp
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::ClearImageList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::CreateWrapper
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::DetachTab
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::EnableAutoColor
//-----------------------------------------------------------------------------

//=============================================================================
// CMFCBaseTabCtrl::EnableCustomToolTips
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::EnableTabDetach
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::EnterDragMode
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::FindTabInfo
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::FireChangeActiveTab
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::FireChangingActiveTab
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetActiveWnd
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetFirstVisibleTab (index-only overload)
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetFirstVisibleTab (with start index)
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetLastVisibleTab
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetMaxWindowSize
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabBkColor
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabByID
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabFromHwnd
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabFromPoint
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabFullWidth
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabHicon
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabID
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabIcon
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabLabel
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabNumberToDetach
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabRect
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabTextColor
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetTabWndNoWrapper
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::GetVisibleTabsNum
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::HasImage
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::HideSingleTab
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::InitAutoColors
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::InsertTab (image-id overload)
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::InsertTab (label overload)
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl - thin CWnd subclass (128 bytes of opaque padding in
// include/openmfc/afxmfc.h). All per-tab records / internal state unmodeled.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::InvalidateTab
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::IsColored
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::SetTabIcon
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::SetTabIconOnly
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::SetTabLabel
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::SetTabTextColor
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::SetTabsOrder
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::ShowTab
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::StartRenameTab
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::SwapTabs
//-----------------------------------------------------------------------------

//=============================================================================
// CMFCBaseTabCtrl IAccessible methods (IAccessible).  The retail bodies
// validate the pure parameters exactly as below and then resolve per-tab
// accessibility data from the unmodeled record state (+0x20 rects, +0x48
// visible flags) or the +0x360 CAccessibilityData block; the failure
// terminals for the same argument values are kept.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::accDoDefaultAction
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::accHitTest
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::accLocation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::accNavigate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accChild
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accChildCount
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accDefaultAction
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accName
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accRole
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accState
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCBaseTabCtrl::get_accValue
//-----------------------------------------------------------------------------
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

//=============================================================================
// CMFCBaseTabCtrl::IsTabVisible
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::MoveTab
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnCancelMode
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnCreate
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnDestroy
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnLButtonDblClk
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnLButtonDown
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnLButtonUp
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnMouseMove
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnNeedTipText
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnRButtonDown
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnSetAccData
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnSysColorChange
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::OnWindowPosChanged
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::PreTranslateMessage
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::RemoveAllTabs
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::RenameTab
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::ResetImageList
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::Serialize
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetACCData
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetActiveTabColor
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetActiveTabTextColor
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetAutoColors
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetDrawNoPrefix
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetImageList (resource-id overload)
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetImageList (HIMAGELIST overload)
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetLocation
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetTabBkColor
//=============================================================================

//=============================================================================
// CMFCBaseTabCtrl::SetTabHicon
//=============================================================================
// Symbol: ?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@IIH@Z
extern "C" void MS_ABI impl__AddTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__IIH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* pTabWnd, unsigned int uiResTabLabel,
    unsigned int uiImageId, int bDetachable)
{
    if (!pThis) return;

    wchar_t label[kMaxTabLabel];
    const bool haveLabel = LoadTabLabel(uiResTabLabel, label, kMaxTabLabel);
    pThis->AddTab(pTabWnd, haveLabel ? label : nullptr, uiImageId,
                  bDetachable ? TRUE : FALSE);
}
// Symbol: ?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@PEB_WIH@Z
extern "C" void MS_ABI impl__AddTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__PEB_WIH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* pTabWnd, const wchar_t* lpszTabLabel,
    unsigned int uiImageId, int bDetachable)
{
    if (!pThis) return;
    pThis->AddTab(pTabWnd, lpszTabLabel, uiImageId, bDetachable ? TRUE : FALSE);
}
// Retail (0x180018390): walks the restore array at +0x250, re-hands each
// serialized tab info to internal vslot/virtual calls (0x180018310,
// vslot 0x58, ...) and re-runs the +0x2e8/frame checks. The +0x250 array and
// the frame members are unmodeled.
// Symbol: ?ApplyRestoredTabInfo@CMFCBaseTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__ApplyRestoredTabInfo_CMFCBaseTabCtrl__UEAAXH_Z(
    CMFCBaseTabCtrl* pThis, int /*nTabNumber*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the restore-info array at
    // +0x250 and the internal tab/frame virtuals; not modeled.
}
// Retail (0x180015ad0): walks the tab array (+0x108/+0x110/+0x150), destroys
// every tab window when the +0x1e4 flag is set and deletes the per-tab
// records, then tears down the tooltips (+0x1b8/+0x1c0) and the arrays
// (+0x100/+0x128), and finally sets the tab count (+0x150) to 0 and the active
// tab (+0x154) to -1. The internal arrays are unmodeled, so the side-table is
// emptied through the declared public RemoveTab path instead.
// Symbol: ?CleanUp@CMFCBaseTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCBaseTabCtrl__UEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    while (pThis->GetTabsCount() > 0) {
        pThis->RemoveTab(0);
    }
    // TODO(clean-room): partially transcribed -- the per-tab window
    // destruction (+0x1e4 flag), the tab-record deletion, the tooltip teardown
    // (+0x1b8/+0x1c0) and the internal array resets (+0x100/+0x128/+0x150/
    // +0x154) are not modeled; the side-table is cleared via the public
    // RemoveTab path instead.
}
// Retail (0x180019d90): destroys the embedded CImageList object at +0x160
// (DeleteImageList 0x180296c80 when a handle is present), zeroes the size
// member +0x178 and tail-calls the vslot-0x300 relayout. The image members
// and the relayout virtual are unmodeled; the relayout is proxied by a redraw.
// Symbol: ?ClearImageList@CMFCBaseTabCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__ClearImageList_CMFCBaseTabCtrl__UEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the embedded CImageList
    // (+0x160) and the size member (+0x178) and the vslot-0x300 relayout are
    // not modeled; only the relayout redraw is kept.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x1800192b0): resolves the parent frame (+0x288) and, when the tab
// control class or state requires it, allocates the wrapper window (0x500),
// combines its rect/style members (+0x4e0/+0x4b0) and drives the vslot-0x658
// create call. The members and virtuals are unmodeled, so no wrapper can be
// created.
// Symbol: ?CreateWrapper@CMFCBaseTabCtrl@@MEAAPEAVCWnd@@PEAV2@PEB_WH@Z
extern "C" CWnd* MS_ABI impl__CreateWrapper_CMFCBaseTabCtrl__MEAAPEAVCWnd__PEAV2_PEB_WH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* /*pParentWnd*/, const wchar_t* /*lpszText*/,
    int /*bIsInternal*/)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): not transcribed -- needs the +0x288 frame member, the
    // 0x500-byte wrapper allocation and the vslot-0x658 create virtual; not
    // modeled.
    return nullptr;
}
// Retail (0x180017050): resolves the tab (vslot 0x590/0x3f0/0x370), validates
// its type against the built-in tab class (0x180234cf0 / 0x1802f5918), drives
// the drag/float docking machinery (vslot 0x328/0x3a8 and the +0x238/+0x23c
// rect members) and returns the detachment result (0 on failure). The dock
// state and virtuals are unmodeled, so the retail failure terminal 0 is
// returned.
// Symbol: ?DetachTab@CMFCBaseTabCtrl@@UEAAHW4AFX_DOCK_METHOD@@HH@Z
extern "C" int MS_ABI impl__DetachTab_CMFCBaseTabCtrl__UEAAHW4AFX_DOCK_METHOD__HH_Z(
    CMFCBaseTabCtrl* pThis, int /*dockMethod*/, int /*nTabNum*/, int /*nTabWidth*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the tab-resolution virtuals
    // (vslot 0x590/0x3f0/0x370/0x328/0x3a8), the rect members +0x238/+0x23c
    // and the dock state; not modeled.
    return FALSE;
}
// Retail (0x18001a190): when auto-color was previously enabled and bEnable is
// cleared, resets every tab's background color (+0x44) to -1; then stores
// +0x328 = bEnable, runs the vslot-0x5a0 repaint virtual and finally calls
// RedrawWindow(hwnd, NULL, NULL, 0x105). The flag, the per-tab records and the
// repaint virtual are unmodeled; the explicit RedrawWindow is kept.
// Symbol: ?EnableAutoColor@CMFCBaseTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableAutoColor_CMFCBaseTabCtrl__QEAAXH_Z(
    CMFCBaseTabCtrl* pThis, int /*bEnable*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the +0x328 enable flag, the
    // per-tab background-color resets (+0x44 of each tab record) and the
    // vslot-0x5a0 repaint are not modeled; the final RedrawWindow is kept.
    HWND hWnd = pThis->GetSafeHwnd();
    if (hWnd) {
        ::RedrawWindow(hWnd, nullptr, nullptr, 0x105);
    }
}
// Retail (0x18001a9d0): when bEnable differs from the +0x1c8 flag it stores
// it and, if the tooltip helper (+0x1b8) and its tooltip window (+0x40) exist,
// reinstalls the per-tab tooltips (a range of CToolTipCtrl calls through the
// tab array) and triggers the vslot-0x318 relayout. Returns 1 always. The
// flag and helper are unmodeled; the constant return value is kept.
// Symbol: ?EnableCustomToolTips@CMFCBaseTabCtrl@@QEAAHH@Z
extern "C" int MS_ABI impl__EnableCustomToolTips_CMFCBaseTabCtrl__QEAAHH_Z(
    CMFCBaseTabCtrl* pThis, int /*bEnable*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): partially transcribed -- the m_bCustomToolTips flag
    // (+0x1c8) and the tooltip helper (+0x1b8) are not modeled, so the change
    // detection, the per-tab tooltip (re)install and the vslot-0x318 relayout
    // are skipped.
    return TRUE;
}
// Retail (0x180016e10): bounds-checks `0 <= iTab < tabs count (+0x150)` and
// stores bDetach at +0x58 of the tab record; returns 1 when applied, 0 when
// out of bounds. The per-tab record is unmodeled; the bounds gate and the
// terminal values are kept.
// Symbol: ?EnableTabDetach@CMFCBaseTabCtrl@@UEAAHHH@Z
extern "C" int MS_ABI impl__EnableTabDetach_CMFCBaseTabCtrl__UEAAHHH_Z(
    CMFCBaseTabCtrl* pThis, int iTab, int /*bDetach*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab "detachable"
    // flag at +0x58 of the tab record (tab array +0x108/+0x110) is not
    // modeled.
    return TRUE;
}
// Retail (0x180017980): sets the +0x234 drag flag, resolves the parent frame
// (CWnd::FromHandlePermanent of GetParent), reads the in-place drag geometry
// (a vslot-0x380 virtual filling a stack rect from the active tab +0x154,
// storing it at +0x238, and a +0x240 horizontal offset) and copies the active
// tab to +0x158. The drag members and the vslot-0x380 virtual are unmodeled.
// Symbol: ?EnterDragMode@CMFCBaseTabCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__EnterDragMode_CMFCBaseTabCtrl__QEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x234 drag flag, the
    // in-place drag state (+0x238/+0x240/+0x158) and the vslot-0x380 virtual.
}
// Retail (0x180018310): nulls *ppTabInfo, scans the tab array (+0x108/+0x110)
// calling CWnd::GetDlgCtrlID (0x1802a99a0) on each tab's window (+0x30) and,
// on a match with nID, stores the CMFCBaseTabInfo record pointer in
// *ppTabInfo and returns the 0-based index; returns 0 otherwise. The record
// pointer cannot be produced (the records are unmodeled), so the failure
// terminal (0, NULL) is returned for every call.
// Symbol: ?FindTabInfo@CMFCBaseTabCtrl@@IEAAHHPEAPEAVCMFCTabInfo@@@Z
extern "C" int MS_ABI impl__FindTabInfo_CMFCBaseTabCtrl__IEAAHHPEAPEAVCMFCTabInfo___Z(
    CMFCBaseTabCtrl* pThis, int /*nID*/, void** ppTabInfo)
{
    if (!pThis) return FALSE;
    if (ppTabInfo) *ppTabInfo = nullptr;
    // TODO(clean-room): partially transcribed -- retail writes the matched
    // CMFCBaseTabInfo record pointer; the records are not modeled, so no tab
    // can be reported (the control-ID scan itself is matchable only through
    // the unmodeled records).
    return FALSE;
}
// Retail (0x18001a750): sends the registered "change active tab" message
// (0x1803c1434) to the parent frame window(s), sets the +0x338 accessibility
// flag and, when the tab index is valid, updates the embedded accessibility
// block (+0x360) through the vslot-0x5b8 virtual. The message id, flag and
// block are unmodeled.
// Symbol: ?FireChangeActiveTab@CMFCBaseTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__FireChangeActiveTab_CMFCBaseTabCtrl__UEAAXH_Z(
    CMFCBaseTabCtrl* pThis, int /*iNewTab*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the registered message id
    // (0x1803c1434), the +0x338 flag and the +0x360 accessibility block; not
    // modeled.
}
// Retail (0x18001a870): sends the registered "changing active tab" message
// (0x1803c143c) to the parent frame; returns 1 when the frame handled it,
// otherwise sends it to a second candidate frame and returns that result
// (0 when both are absent). The message ids and frames are unmodeled, so
// FALSE (the "not handled" terminal) is returned.
// Symbol: ?FireChangingActiveTab@CMFCBaseTabCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__FireChangingActiveTab_CMFCBaseTabCtrl__UEAAHH_Z(
    CMFCBaseTabCtrl* pThis, int /*iNewTab*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the registered message id
    // (0x1803c143c) and the parent-frame resolution; not modeled.
    return FALSE;
}
// Retail (0x180016e50): reads the active tab (+0x154) and returns NULL when
// it is -1, otherwise bounds-checks it and returns the CWnd* stored at +0x30
// of the tab record. Fully transcribed through the declared API: the retail
// CWnd* at record+0x30 is exactly GetTabWnd()'s side-table value.
// Symbol: ?GetActiveWnd@CMFCBaseTabCtrl@@UEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetActiveWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__XZ(
    const CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return nullptr;
    const int nActive = pThis->GetActiveTab();
    if (nActive < 0) return nullptr;
    return pThis->GetTabWnd(nActive);
}
// Retail (0x180019ed0) is a forwarder: `return GetFirstVisibleTab(0, iTab)`
// via vslot 0x4b8. Transcribed through the two-argument impl.
// Symbol: ?GetFirstVisibleTab@CMFCBaseTabCtrl@@UEAAPEAVCWnd@@AEAH@Z
extern "C" CWnd* MS_ABI impl__GetFirstVisibleTab_CMFCBaseTabCtrl__UEAAPEAVCWnd__AEAH_Z(
    const CMFCBaseTabCtrl* pThis, int& iTab)
{
    if (!pThis) {
        iTab = -1;
        return nullptr;
    }
    return impl__GetFirstVisibleTab_CMFCBaseTabCtrl__UEAAPEAVCWnd__HAEAH_Z(
        pThis, 0, iTab);
}
// Retail (0x180019ef0): scans the tab array from iStartTab upward for the
// first tab whose +0x48 (visible) flag is set; on a hit stores the index and
// returns the CWnd* at record+0x30. When nothing is found and iStartTab > 0
// it restarts the scan from 0 (vslot 0x4b8); otherwise it stores -1 and
// returns NULL. The per-tab flag is unmodeled, so (like IsTabVisible) no tab
// can ever be reported visible; the scan shape is kept through the declared
// IsTabVisible helper.
// Symbol: ?GetFirstVisibleTab@CMFCBaseTabCtrl@@UEAAPEAVCWnd@@HAEAH@Z
extern "C" CWnd* MS_ABI impl__GetFirstVisibleTab_CMFCBaseTabCtrl__UEAAPEAVCWnd__HAEAH_Z(
    const CMFCBaseTabCtrl* pThis, int iStartTab, int& iTab)
{
    if (!pThis) {
        iTab = -1;
        return nullptr;
    }
    const int nCount = pThis->GetTabsCount();
    const int nStart = iStartTab < 0 ? 0 : iStartTab;
    for (int i = nStart; i < nCount; ++i) {
        if (impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(pThis, i)) {
            iTab = i;
            return pThis->GetTabWnd(i);
        }
    }
    if (iStartTab > 0) {
        // Retail restarts the scan from the first tab (tail-jump to vslot
        // 0x4b8 with iStartTab = 0).
        return impl__GetFirstVisibleTab_CMFCBaseTabCtrl__UEAAPEAVCWnd__HAEAH_Z(
            pThis, 0, iTab);
    }
    iTab = -1;
    return nullptr;
}
// Retail (0x180019f70): scans the tab array from the end downward for the
// first tab whose +0x48 (visible) flag is set; on a hit stores the index and
// returns the CWnd* at record+0x30, otherwise stores -1 and returns NULL. The
// per-tab flag is unmodeled (IsTabVisible reports no visible tab), so the
// failure terminal is reached for every call.
// Symbol: ?GetLastVisibleTab@CMFCBaseTabCtrl@@UEAAPEAVCWnd@@AEAH@Z
extern "C" CWnd* MS_ABI impl__GetLastVisibleTab_CMFCBaseTabCtrl__UEAAPEAVCWnd__AEAH_Z(
    const CMFCBaseTabCtrl* pThis, int& iTab)
{
    if (!pThis) {
        iTab = -1;
        return nullptr;
    }
    for (int i = pThis->GetTabsCount() - 1; i >= 0; --i) {
        if (impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(pThis, i)) {
            iTab = i;
            return pThis->GetTabWnd(i);
        }
    }
    iTab = -1;
    return nullptr;
}
// Retail (0x18001a050): starts with CSize(0,0) and accumulates the maximum
// over every tab window: for each tab CWnd (record+0x30) that IsKindOf the
// class at 0x1802dc540 it calls the vslot-0x4d0 max-size virtual, otherwise
// it uses GetClientRect (right-left / bottom-top), keeping max(cx)/max(cy).
// The class and the virtual are unmodeled, so the retail CSize(0,0) "no
// constraint" result (the value for a control with no tabs) is returned.
//
// ABI: MSVC x64 passes `this` in %rcx and the hidden CSize* return slot in
// %rdx and returns that pointer in %rax; the declaration below mirrors that
// (pThis first) so real MSVC callers see the correct registers.
// Symbol: ?GetMaxWindowSize@CMFCBaseTabCtrl@@UEBA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetMaxWindowSize_CMFCBaseTabCtrl__UEBA_AVCSize__XZ(
    const CMFCBaseTabCtrl* pThis, CSize* pRet)
{
    if (!pThis || !pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    // TODO(clean-room): partially transcribed -- the per-tab window scan uses
    // the IsKindOf class check (0x1802dc540) and the vslot-0x4d0 max-size
    // virtual; neither is modeled, so the tab geometry is not accumulated.
    return pRet;
}
// Retail (0x1800199e0): bounds-checks and returns the background color at
// +0x44 of the tab record; when that slot holds -1 and the auto-color flag
// (+0x328) is set it computes the color from the auto-color array
// (+0x308/+0x310) and caches it in the record. Out of bounds returns
// 0xffffffff. The record and array are unmodeled, so the retail "unset"
// color 0xffffffff is returned for every in-bounds tab too.
// Symbol: ?GetTabBkColor@CMFCBaseTabCtrl@@UEBAKH@Z
extern "C" unsigned long MS_ABI impl__GetTabBkColor_CMFCBaseTabCtrl__UEBAKH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return (unsigned long)-1;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return (unsigned long)-1;
    // TODO(clean-room): partially transcribed -- the per-tab background color
    // (+0x44) and the auto-color array (+0x308/+0x310/+0x328) are not modeled.
    return (unsigned long)-1;
}
// Retail (0x180016fb0): scans the tab array and returns the index of the
// first tab whose ID (record +0x3c) equals nID, or -1. The scan is kept
// through the declared GetTabID helper (which also reads the unmodeled
// +0x3c record field and therefore always reports -1), so the terminal -1 is
// returned for every call.
// Symbol: ?GetTabByID@CMFCBaseTabCtrl@@UEBAHH@Z
extern "C" int MS_ABI impl__GetTabByID_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int nID)
{
    if (!pThis) return -1;
    for (int i = 0; i < pThis->GetTabsCount(); ++i) {
        if (impl__GetTabID_CMFCBaseTabCtrl__QEBAHH_Z(pThis, i) == nID) {
            return i;
        }
    }
    // TODO(clean-room): partially transcribed -- the per-tab ID (record +0x3c)
    // is not modeled, so no tab can match.
    return -1;
}
// Retail (0x180016f10): scans the tab array; for each tab window (+0x30) it
// first compares its m_hWnd (+0x40) directly and, when that fails and the
// window IsKindOf the class at 0x1803b11a8, compares the m_hWnd of the
// vslot-0x758 wrapper result. Returns the matching index or -1. The direct
// branch is transcribed through GetTabWnd()/GetSafeHwnd(); the wrapper
// virtual is unmodeled.
// Symbol: ?GetTabFromHwnd@CMFCBaseTabCtrl@@UEBAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(
    const CMFCBaseTabCtrl* pThis, HWND hWnd)
{
    if (!pThis) return -1;
    for (int i = 0; i < pThis->GetTabsCount(); ++i) {
        CWnd* pTab = pThis->GetTabWnd(i);
        if (!pTab) continue;
        if (pTab->GetSafeHwnd() == hWnd) {
            return i;
        }
    }
    // TODO(clean-room): partially transcribed -- retail also matches through
    // the vslot-0x758 wrapper window when the tab window IsKindOf the class
    // at 0x1803b11a8; the class and the virtual are not modeled.
    return -1;
}
// Retail (0x180016e90): scans the tab array for a tab whose +0x48 (visible)
// flag is set and whose rect (+0x20) contains the point (CRect::PtInRect);
// returns the matching index or -1. The per-tab rects and flags are
// unmodeled, so the "no tab found" terminal is returned.
// Symbol: ?GetTabFromPoint@CMFCBaseTabCtrl@@UEBAHAEAVCPoint@@@Z
extern "C" int MS_ABI impl__GetTabFromPoint_CMFCBaseTabCtrl__UEBAHAEAVCPoint___Z(
    const CMFCBaseTabCtrl* pThis, CPoint& /*point*/)
{
    if (!pThis) return -1;
    // TODO(clean-room): not transcribed -- needs the per-tab rects (+0x20) and
    // the visible flag (+0x48) of the tab records; not modeled.
    return -1;
}
// Retail (0x180019b10): bounds-checks and returns the full width stored at
// +0x38 of the tab record; out of bounds returns 0. The per-tab record is
// unmodeled; the bounds gate and the terminal value are kept.
// Symbol: ?GetTabFullWidth@CMFCBaseTabCtrl@@UEBAHH@Z
extern "C" int MS_ABI impl__GetTabFullWidth_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return 0;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return 0;
    // TODO(clean-room): partially transcribed -- the per-tab full width
    // (record +0x38) is not modeled.
    return 0;
}
// Retail (0x180016bb0): bounds-checks and returns the HICON stored at +0x18
// of the tab record; out of bounds returns NULL. The per-tab record is
// unmodeled; the bounds gate and the terminal value are kept.
// Symbol: ?GetTabHicon@CMFCBaseTabCtrl@@UEBAPEAUHICON__@@H@Z
extern "C" HICON MS_ABI impl__GetTabHicon_CMFCBaseTabCtrl__UEBAPEAUHICON____H_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return nullptr;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return nullptr;
    // TODO(clean-room): partially transcribed -- the per-tab icon handle
    // (record +0x18) is not modeled.
    return nullptr;
}
// Retail (0x180016860): bounds-checks and returns the ID stored at +0x3c of
// the tab record; out of bounds returns -1. The per-tab record is unmodeled;
// the bounds gate and the terminal value are kept.
// Symbol: ?GetTabID@CMFCBaseTabCtrl@@QEBAHH@Z
extern "C" int MS_ABI impl__GetTabID_CMFCBaseTabCtrl__QEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return -1;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return -1;
    // TODO(clean-room): partially transcribed -- the per-tab ID (record +0x3c,
    // set by AddTab's uiId) is not exposed by the declared API.
    return -1;
}
// Retail (0x180016b00): bounds-checks and returns the icon index stored at
// +0x10 of the tab record; out of bounds returns 0xffffffff. The per-tab
// record is unmodeled; the bounds gate and the terminal value are kept.
// Symbol: ?GetTabIcon@CMFCBaseTabCtrl@@UEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetTabIcon_CMFCBaseTabCtrl__UEBAIH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return (unsigned int)-1;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return (unsigned int)-1;
    // TODO(clean-room): partially transcribed -- the per-tab icon index
    // (record +0x10) is not modeled.
    return (unsigned int)-1;
}
// Retail (0x1800168a0): bounds-checks, then copies the tab label CString
// (record +0x8) into strLabel -- or loads the default "Tab" label when the
// icon-only flag (+0x50) is set -- and returns 1; out of bounds returns 0.
// The per-tab records are unmodeled, so the output is emptied to a
// well-defined value and the in-bounds terminal TRUE is kept.
// Symbol: ?GetTabLabel@CMFCBaseTabCtrl@@UEBAHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI
impl__GetTabLabel_CMFCBaseTabCtrl__UEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCBaseTabCtrl* pThis, int iTab, CString& strLabel)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    strLabel.Empty();
    // TODO(clean-room): partially transcribed -- the per-tab label (record
    // +0x8) and the icon-only flag (+0x50) are not modeled, so the label
    // cannot be populated.
    return TRUE;
}
// Retail (0x180017040): when iTab is -1 it substitutes the active tab
// (+0x154) and returns the result. Fully transcribed through the declared
// GetActiveTab.
// Symbol: ?GetTabNumberToDetach@CMFCBaseTabCtrl@@MEBAHH@Z
extern "C" int MS_ABI impl__GetTabNumberToDetach_CMFCBaseTabCtrl__MEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return -1;
    if (iTab == -1) {
        return pThis->GetActiveTab();
    }
    return iTab;
}
// Retail (0x180016800): bounds-checks; when the tab's +0x48 (visible) flag is
// set it copies the tab rect (record +0x20) into rect and returns 1,
// otherwise it empties rect (CRect::SetRectEmpty) and returns 0. Out of
// bounds returns 0 without touching rect. The flag and rect are unmodeled
// (flag defaults to clear), so the emptied-rect/0 terminal is kept for every
// in-bounds tab.
// Symbol: ?GetTabRect@CMFCBaseTabCtrl@@UEBAHHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetTabRect_CMFCBaseTabCtrl__UEBAHHAEAVCRect___Z(
    const CMFCBaseTabCtrl* pThis, int iTab, CRect& rect)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    rect.SetRectEmpty();
    // TODO(clean-room): partially transcribed -- the per-tab visible flag
    // (+0x48) and tab rect (+0x20) are not modeled, so no in-bounds tab can
    // report a rect (retail returns 0/empty while the flag is clear).
    return FALSE;
}
// Retail (0x180019a90): bounds-checks and returns the text color stored at
// +0x40 of the tab record; out of bounds returns 0xffffffff. The per-tab
// record is unmodeled; the bounds gate and the terminal value are kept.
// Symbol: ?GetTabTextColor@CMFCBaseTabCtrl@@UEBAKH@Z
extern "C" unsigned long MS_ABI impl__GetTabTextColor_CMFCBaseTabCtrl__UEBAKH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return (unsigned long)-1;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return (unsigned long)-1;
    // TODO(clean-room): partially transcribed -- the per-tab text color
    // (record +0x40) is not modeled.
    return (unsigned long)-1;
}
// Retail (0x180016780): bounds-checks, loads the tab's CWnd* (record +0x30)
// and, when it IsKindOf the class at 0x1803b11a8, returns the vslot-0x758
// wrapper result; otherwise it returns the CWnd* directly. Out of bounds
// returns NULL. The direct branch is transcribed through GetTabWnd(); the
// wrapper virtual is unmodeled.
// Symbol: ?GetTabWndNoWrapper@CMFCBaseTabCtrl@@UEBAPEAVCWnd@@H@Z
extern "C" CWnd* MS_ABI impl__GetTabWndNoWrapper_CMFCBaseTabCtrl__UEBAPEAVCWnd__H_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return nullptr;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return nullptr;
    // TODO(clean-room): partially transcribed -- retail returns the vslot-0x758
    // wrapper window when the tab window IsKindOf the class at 0x1803b11a8;
    // the class and the virtual are not modeled.
    return pThis->GetTabWnd(iTab);
}
// Retail (0x180016580): counts the tabs whose +0x48 (visible) flag is set.
// The flag is unmodeled (IsTabVisible reports no visible tab), so the count
// is always 0; the scan shape is kept through the declared IsTabVisible.
// Symbol: ?GetVisibleTabsNum@CMFCBaseTabCtrl@@UEBAHXZ
extern "C" int MS_ABI impl__GetVisibleTabsNum_CMFCBaseTabCtrl__UEBAHXZ(
    const CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return 0;
    int nVisible = 0;
    for (int i = 0; i < pThis->GetTabsCount(); ++i) {
        if (impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(pThis, i)) {
            ++nVisible;
        }
    }
    // TODO(clean-room): partially transcribed -- the per-tab visible flag
    // (+0x48) is not modeled, so no tab can be counted.
    return nVisible;
}
// Retail (0x18001a900): bounds-checks and returns 1 when the tab's hicon
// (record +0x18) is non-NULL, or when the vslot-0x488 image list is present
// and the tab's icon index (record +0x10) is not -1; otherwise 0. The
// per-tab records and the image list are unmodeled, so FALSE is returned.
// Symbol: ?HasImage@CMFCBaseTabCtrl@@UEBAHH@Z
extern "C" int MS_ABI impl__HasImage_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab hicon (+0x18) and
    // icon index (+0x10) and the vslot-0x488 image list are not modeled.
    return FALSE;
}
// Retail (0x180019ea0): early-returns when the +0x1f4 flag already equals
// bHide; otherwise stores it and, when m_hWnd is valid, triggers the
// vslot-0x318 relayout. The flag member is unmodeled; the relayout redraw is
// kept (mirroring the sibling CMFCTabCtrl::HideSingleTab at +0x1f4).
// Symbol: ?HideSingleTab@CMFCBaseTabCtrl@@UEAAXH@Z
extern "C" void MS_ABI impl__HideSingleTab_CMFCBaseTabCtrl__UEAAXH_Z(
    CMFCBaseTabCtrl* pThis, int /*bHide*/)
{
    if (!pThis) return;
    if (!pThis->GetSafeHwnd()) return;
    // TODO(clean-room): partially transcribed -- the m_bHideSingleTab flag
    // (retail +0x1f4) has no declared member, so the change detection and
    // storage are skipped.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x18001a220): when the +0x32c auto-color flag is set, clears the
// internal auto-color array (+0x300) and refills it with the eight default
// colors (starting 0xf2d4c5), then clears +0x32c. The flag and array are
// unmodeled.
// Symbol: ?InitAutoColors@CMFCBaseTabCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__InitAutoColors_CMFCBaseTabCtrl__MEAAXXZ(
    CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x32c flag and the
    // auto-color array at +0x300 (CArray<COLORREF> of 8 defaults); not
    // modeled.
}
// Retail (0x180016120): resolves the tab window's control ID and the image
// list (CMapPtrToPtr at +0x180), allocates a CMFCBaseTabInfo record, invokes
// the vslot-0x598 insertion virtual (with the image index, ID and the +0x338
// callback state) and releases the record reference. The record, image map
// and virtuals are unmodeled, so no tab is inserted (matching a control with
// no image support).
// Symbol: ?InsertTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@IHIH@Z
extern "C" void MS_ABI impl__InsertTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__IHIH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* /*pWnd*/, unsigned int /*uiImageID*/,
    int /*nIndex*/, unsigned int /*uiID*/, int /*bDetachable*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the CMFCBaseTabInfo records,
    // the +0x180 image map and the vslot-0x598 insertion virtual.
}
// Retail (0x180015d60): validates/normalizes the insertion index, resolves
// the insert-position state (vslot 0x430), allocates a CMFCBaseTabInfo record
// with the label, and invokes the vslot-0x598/vslot-0x338 insertion virtuals.
// The records and virtuals are unmodeled; the declared AddTab (which appends
// to the side table) cannot honor the nIndex position, so no tab is inserted.
// Symbol: ?InsertTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@PEB_WHIH@Z
extern "C" void MS_ABI impl__InsertTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__PEB_WHIH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* /*pWnd*/, const wchar_t* /*lpszLabel*/,
    int /*nIndex*/, unsigned int /*uiID*/, int /*bDetachable*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the CMFCBaseTabInfo records
    // and the vslot-0x598/vslot-0x338 insertion virtuals; the declared AddTab
    // path cannot honor the insertion index.
}
// Retail (0x180018270): returns immediately for iTab < 0, otherwise runs the
// vslot-0x380 GetTabRect(iTab, &rect) virtual and, on a hit, CWnd::
// InvalidateRect(&rect, TRUE) + CWnd::UpdateWindow(). Transcribed through the
// declared GetTabRect helper (which reports no visible tab, so the
// invalidation is skipped exactly like retail for an invisible tab).
// Symbol: ?InvalidateTab@CMFCBaseTabCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__InvalidateTab_CMFCBaseTabCtrl__QEAAXH_Z(
    CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return;
    if (iTab < 0) return;
    CRect rect;
    if (impl__GetTabRect_CMFCBaseTabCtrl__UEBAHHAEAVCRect___Z(pThis, iTab, rect)) {
        pThis->InvalidateRect(rect, TRUE);
        pThis->UpdateWindow();
    }
}
// Retail (0x18001a700): returns TRUE as soon as any tab's background color
// (record +0x44) differs from 0xffffffff, scanning through the vslot-0x3c8
// GetTabBkColor virtual; FALSE when no tab is colored. Transcribed through
// the declared GetTabBkColor helper (which always reports the unset color),
// so the scan shape matches retail and the terminal FALSE is reached.
// Symbol: ?IsColored@CMFCBaseTabCtrl@@UEBAHXZ
extern "C" int MS_ABI impl__IsColored_CMFCBaseTabCtrl__UEBAHXZ(
    const CMFCBaseTabCtrl* pThis)
{
    if (!pThis) return FALSE;
    for (int i = 0; i < pThis->GetTabsCount(); ++i) {
        if (impl__GetTabBkColor_CMFCBaseTabCtrl__UEBAKH_Z(pThis, i) !=
            (unsigned long)-1) {
            return TRUE;
        }
    }
    // TODO(clean-room): partially transcribed -- the per-tab background color
    // (+0x44 of the tab record) is not modeled, so no tab can be colored.
    return FALSE;
}
// Retail (0x180016b40): bounds-checks `0 <= iTab < count (+0x150)`, destroys
// the previous per-tab hicon (record +0x18, DestroyIcon at 0x1802c7278),
// stores uiIcon at +0x10 and nulls +0x18; returns 1 when applied, 0 when out
// of bounds. The per-tab records are unmodeled; the bounds gate and the
// terminal values are kept.
// Symbol: ?SetTabIcon@CMFCBaseTabCtrl@@UEAAHHI@Z
extern "C" int MS_ABI impl__SetTabIcon_CMFCBaseTabCtrl__UEAAHHI_Z(
    CMFCBaseTabCtrl* pThis, int iTab, unsigned int /*uiIcon*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab icon index
    // (+0x10) and hicon (+0x18) of the tab record are not modeled.
    return TRUE;
}
// Retail (0x180016d70): bounds-checks, stores bIconOnly at +0x50 and the tab
// width at +0x54 of the tab record, then runs the vslot-0x318 relayout and
// returns 1; out of bounds returns 0. The per-tab record is unmodeled; the
// bounds gate, the relayout redraw and the terminal values are kept.
// Symbol: ?SetTabIconOnly@CMFCBaseTabCtrl@@UEAAHHHH@Z
extern "C" int MS_ABI impl__SetTabIconOnly_CMFCBaseTabCtrl__UEAAHHHH_Z(
    CMFCBaseTabCtrl* pThis, int iTab, int /*bIconOnly*/, int /*nTabWidth*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab "icon only" flag
    // (+0x50) and width (+0x54) of the tab record are not modeled; the
    // vslot-0x318 relayout is kept as a redraw.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
    return TRUE;
}
// Retail (0x180016920): bounds-checks (failing on an uninitialized label
// CString too), stores the label into the tab record (+0x8), refreshes the
// tooltip helper (+0x1b8/+0x1c8), mirrors the text into the tab window
// (CWnd::SetWindowTextW on record+0x30) and runs the vslot-0x318 relayout,
// then updates the frame when iTab is the active tab; returns 1 on success,
// 0 on failure. The record/tooltip are unmodeled; the tab-window text update,
// the bounds gate and the relayout redraw are kept.
// Symbol: ?SetTabLabel@CMFCBaseTabCtrl@@UEAAHHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI
impl__SetTabLabel_CMFCBaseTabCtrl__UEAAHHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCBaseTabCtrl* pThis, int iTab, const CString& strLabel)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // Retail (0x18001695e) also fails when the label CString is uninitialized
    // (refcount gate on the label data); not modeled here.
    CWnd* pTabWnd = pThis->GetTabWnd(iTab);
    if (pTabWnd && pTabWnd->GetSafeHwnd()) {
        // Retail mirrors the new label into the tab window text
        // (CWnd::SetWindowTextW at 0x1802a9790).
        pTabWnd->SetWindowTextW(strLabel.GetString());
    }
    // TODO(clean-room): partially transcribed -- the per-tab label store (+0x8
    // of the tab record), the tooltip refresh (+0x1b8/+0x1c8) and the
    // active-tab frame update are not modeled; the vslot-0x318 relayout is
    // kept as a redraw.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
    return TRUE;
}
// Retail (0x180019ad0): bounds-checks and stores the color at +0x40 of the
// tab record; returns 1 when applied, 0 when out of bounds. The per-tab
// record is unmodeled; the bounds gate and the terminal values are kept.
// Symbol: ?SetTabTextColor@CMFCBaseTabCtrl@@UEAAHHK@Z
extern "C" int MS_ABI impl__SetTabTextColor_CMFCBaseTabCtrl__UEAAHHK_Z(
    CMFCBaseTabCtrl* pThis, int iTab, unsigned long /*color*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab text color
    // (record +0x40) is not modeled.
    return TRUE;
}
// Retail (0x180019170): returns 0 when the tab count (+0x150) differs from
// tabsOrder.GetSize() (CArray m_nSize at +0x10); otherwise it copies the
// tab-record pointers into a temporary CArray, clears the internal tab array
// (+0x100/+0x108/+0x110) and re-appends the records in the caller's order,
// tracking the active tab (+0x154); returns 1. The records/array are
// unmodeled; the size gate and the terminal values are kept.
// Symbol: ?SetTabsOrder@CMFCBaseTabCtrl@@QEAAHAEBV?$CArray@HH@@@Z
extern "C" int MS_ABI impl__SetTabsOrder_CMFCBaseTabCtrl__QEAAHAEBV__CArray_HH___Z(
    CMFCBaseTabCtrl* pThis, const CArray<int, int>& tabsOrder)
{
    if (!pThis) return FALSE;
    if (pThis->GetTabsCount() != tabsOrder.GetSize()) return FALSE;
    // TODO(clean-room): partially transcribed -- the internal tab array
    // (+0x100/+0x108/+0x110) and the active tab (+0x154) are not modeled, so
    // the reordering cannot be applied.
    return TRUE;
}
// Retail (0x1800165d0): bounds-checks and early-returns TRUE when the per-tab
// visible flag (+0x48) already equals bShow; otherwise it stores bShow,
// recomputes the active tab (+0x154, scanning the visible flags), optionally
// runs the vslot-0x318 relayout and the vslot-0x438/0x4f0 activation virtuals
// and returns TRUE. The per-tab flags/active-tab state are unmodeled; the
// bounds gate and the terminal values are kept.
// Symbol: ?ShowTab@CMFCBaseTabCtrl@@UEAAHHHHH@Z
extern "C" int MS_ABI impl__ShowTab_CMFCBaseTabCtrl__UEAAHHHHH_Z(
    CMFCBaseTabCtrl* pThis, int iTab, int /*bShow*/, int /*bDontShowActive*/,
    int /*bChangeActiveTab*/)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- the per-tab visible flag
    // (+0x48), the active tab (+0x154) and the relayout/activation virtuals
    // (vslot 0x318/0x438/0x4f0) are not modeled.
    return TRUE;
}
// Retail (0x1800194a0): returns 0 when the +0x230 in-place-edit flag is clear,
// when iTab is out of bounds or when the tab rect (+0x20) is empty; otherwise
// it creates the in-place CEdit editor at +0x228 (style 0x50800080, parent
// this, with WM_* setup) and returns 1. The flag/rect/editor are unmodeled
// (the flag defaults clear), so 0 -- the retail terminal for the flag-clear
// state -- is returned for every call.
// Symbol: ?StartRenameTab@CMFCBaseTabCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__StartRenameTab_CMFCBaseTabCtrl__UEAAHH_Z(
    CMFCBaseTabCtrl* pThis, int iTab)
{
    if (!pThis) return FALSE;
    if (iTab < 0 || iTab >= pThis->GetTabsCount()) return FALSE;
    // TODO(clean-room): partially transcribed -- retail also gates on the
    // +0x230 in-place-edit flag and the tab rect (+0x20, IsRectEmpty) and
    // creates the CEdit editor at +0x228; none are modeled.
    return FALSE;
}
// Retail (0x180018e60): validates both indices against the tab array
// (+0x110) and swaps the two CMFCBaseTabInfo record pointers. The array is
// unmodeled; the bounds gate is kept and the swap itself cannot be applied.
// Symbol: ?SwapTabs@CMFCBaseTabCtrl@@UEAAXHH@Z
extern "C" void MS_ABI impl__SwapTabs_CMFCBaseTabCtrl__UEAAXHH_Z(
    CMFCBaseTabCtrl* pThis, int iTab1, int iTab2)
{
    if (!pThis) return;
    const int nCount = pThis->GetTabsCount();
    if (iTab1 < 0 || iTab2 < 0) return;
    if (iTab1 >= nCount || iTab2 >= nCount) return;
    // TODO(clean-room): not transcribed -- retail swaps the tab-record
    // pointers in the internal array (+0x108/+0x110); the records are not
    // modeled.
}
// Retail (0x18001b050): rejects a non-VT_I4 child id with E_INVALIDARG,
// otherwise walks the visible tabs (record +0x48 flag set, +0x20 rect
// non-empty) and activates the one whose 1-based visible index equals
// varChildID.lVal (vslot 0x438/0x4f0), returning S_OK in all cases. The
// per-tab state is unmodeled, so no activation can happen (retail also
// returns S_OK when no tab matches).
// Symbol: ?accDoDefaultAction@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCBaseTabCtrl__MEAAJUtagVARIANT___Z(
    CMFCBaseTabCtrl* pThis, VARIANT varChildID)
{
    if (!pThis) return S_OK;
    if (varChildID.vt != 3 /*VT_I4*/) return E_INVALIDARG;
    // TODO(clean-room): partially transcribed -- the visible-tab walk uses the
    // per-tab +0x48 flag and +0x20 rect; not modeled.
    return S_OK;
}
// Retail (0x18001add0): null pVarID -> E_INVALIDARG; otherwise it initializes
// *pVarID to {VT_I4, 0}, ScreenToClients the point and stores the 1-based
// child id of the first tab whose rect (+0x20) contains it. The rects are
// unmodeled, so the initialized no-hit result is left in place and S_OK is
// returned.
// Symbol: ?accHitTest@CMFCBaseTabCtrl@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCBaseTabCtrl__MEAAJJJPEAUtagVARIANT___Z(
    CMFCBaseTabCtrl* pThis, long /*xLeft*/, long /*yTop*/, VARIANT* pVarID)
{
    if (!pThis) return S_OK;
    if (!pVarID) return E_INVALIDARG;
    pVarID->vt = 3;   // VT_I4
    pVarID->lVal = 0;
    // TODO(clean-room): partially transcribed -- the per-tab rects (+0x20,
    // PtInRect at 0x1802c72f8) are not modeled.
    return S_OK;
}
// Retail (0x18001b470): NULL out-pointers -> E_INVALIDARG. For varID.lVal == 0
// it reports the control window rect (CWnd::GetWindowRect); for lVal > 0 it
// resolves the tab rect through the vslot-0x5b0 virtual into the +0x39c ACC
// block; lVal < 0 and non-VT_I4 write nothing. The lVal==0 branch is
// transcribed; the +0x39c block is unmodeled, so the lVal>0 branch leaves the
// out-pointers untouched (the retail lVal<0 behavior).
// Symbol: ?accLocation@CMFCBaseTabCtrl@@MEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCBaseTabCtrl__MEAAJPEAJ000UtagVARIANT___Z(
    CMFCBaseTabCtrl* pThis, long* pxLeft, long* pyTop, long* pcxWidth,
    long* pcyHeight, VARIANT varID)
{
    if (!pThis) return S_OK;
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_INVALIDARG;
    if (varID.vt != 3 /*VT_I4*/) return S_OK;
    if (varID.lVal == 0) {
        CRect rect;
        rect.SetRectEmpty();
        if (pThis->GetSafeHwnd()) {
            pThis->GetWindowRect(rect);
        }
        *pxLeft = rect.left;
        *pyTop = rect.top;
        *pcxWidth = rect.Width();
        *pcyHeight = rect.Height();
        return S_OK;
    }
    // TODO(clean-room): partially transcribed -- for varID.lVal > 0 retail
    // resolves the tab rect via the vslot-0x5b0 virtual into the +0x39c ACC
    // block; that block is not modeled, so the out-pointers are left untouched
    // (matching the retail lVal<0 path which also writes nothing).
    return S_OK;
}
// Retail (0x18001af40), fully transcribed. NAVDIR ids (oleacc.h): LEFT=3,
// RIGHT=4, NEXT=5, PREVIOUS=6, FIRSTCHILD=7, LASTCHILD=8. LEFT shares a body
// with PREVIOUS, RIGHT with NEXT; FIRSTCHILD and LASTCHILD both reject a
// non-zero start child with S_FALSE (FIRSTCHILD answers 1, LASTCHILD answers
// the visible-child count); any other navDir falls through to S_FALSE with
// pvarEnd->vt left at VT_EMPTY. The navigation range is the number of visible
// tabs (per-tab +0x48 flag set and +0x20 rect non-empty); with that state
// unmodeled the range is 0.
// Symbol: ?accNavigate@CMFCBaseTabCtrl@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCBaseTabCtrl__MEAAJJUtagVARIANT__PEAU2__Z(
    CMFCBaseTabCtrl* pThis, long navDir, VARIANT varStart, VARIANT* pvarEnd)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarEnd) return E_INVALIDARG;

    pvarEnd->vt = 0;   // VT_EMPTY
    if (varStart.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    // Retail counts the visible tabs (record +0x48 flag set, +0x20 rect
    // non-empty); the per-tab state is unmodeled, so the range is always 0.
    const long nChildren = 0;

    switch (navDir) {
    case 3:  // NAVDIR_LEFT
    case 6:  // NAVDIR_PREVIOUS
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n - 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next > 0) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 4:  // NAVDIR_RIGHT
    case 5:  // NAVDIR_NEXT
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n + 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next <= nChildren) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 7:  // NAVDIR_FIRSTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = 1;
        return S_OK;

    case 8:  // NAVDIR_LASTCHILD
        // Retail (0x18001afe4): non-zero start child -> S_FALSE; otherwise the
        // last child is the visible-child count itself (1-based ids).
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        // TODO(clean-room): partially transcribed -- the visible-children range
        // is 0 because the per-tab +0x48/+0x20 state is not modeled.
        pvarEnd->lVal = nChildren;
        return S_OK;

    default:   // retail returns S_FALSE for every other navDir (incl. up/down)
        // TODO(clean-room): partially transcribed -- the visible-children range
        // is 0 because the per-tab +0x48/+0x20 state is not modeled.
        return S_FALSE;
    }
}
// Retail (0x18001af30) is a straight guard: NULL ppdispChild -> E_INVALIDARG,
// otherwise S_FALSE. No child is ever resolved and *ppdispChild is never
// written. Fully transcribed.
// Symbol: ?get_accChild@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCBaseTabCtrl__MEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCBaseTabCtrl* pThis, VARIANT /*varChildID*/, IDispatch** ppDispChild)
{
    if (!pThis) return S_FALSE;
    if (!ppDispChild) return E_INVALIDARG;
    return S_FALSE;
}
// Retail (0x18001aeb0): NULL pCountChildren -> E_INVALIDARG; otherwise it
// counts the visible tabs (record +0x48 flag set, +0x20 rect non-empty),
// stores the count and returns S_OK. The per-tab state is unmodeled, so the
// count is 0.
// Symbol: ?get_accChildCount@CMFCBaseTabCtrl@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCBaseTabCtrl__MEAAJPEAJ_Z(
    CMFCBaseTabCtrl* pThis, long* pCountChildren)
{
    if (!pThis) return S_OK;
    if (!pCountChildren) return E_INVALIDARG;
    // TODO(clean-room): partially transcribed -- the visible-tab count uses the
    // per-tab +0x48 flag and +0x20 rect; not modeled.
    *pCountChildren = 0;
    return S_OK;
}
// Retail (0x18001b570): a non-VT_I4 child id with a non-zero lVal is rejected
// with E_INVALIDARG; otherwise it resolves the tab's ACC "default action"
// string (accData m_strAccDefAction at +0x388 via the vslot-0x5b0 virtual)
// and returns it as a BSTR, with S_FALSE for the empty string. The string is
// unmodeled (empty), so S_FALSE is returned.
// Symbol: ?get_accDefaultAction@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CMFCBaseTabCtrl__MEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCBaseTabCtrl* pThis, VARIANT varChildID, wchar_t** /*pszDefaultAction*/)
{
    if (!pThis) return S_FALSE;
    if (varChildID.vt != 3 /*VT_I4*/ && varChildID.lVal != 0) {
        return E_INVALIDARG;
    }
    // TODO(clean-room): partially transcribed -- the ACC default-action string
    // (accData +0x388, via the vslot-0x5b0 virtual) is not modeled, so the
    // empty-string S_FALSE terminal is returned.
    return S_FALSE;
}
// Retail (0x18001b390): a non-VT_I4 child id returns S_OK; for lVal == 0 it
// returns the control window text (CWnd::GetWindowTextW into the shared
// buffer) as a BSTR with S_OK; for lVal > 0 it resolves the tab's ACC name
// (accData m_strAccName at +0x360 via the vslot-0x5b0 virtual), returning
// S_FALSE for the empty string; lVal < 0 returns S_OK. The strings are
// unmodeled (empty), so the empty-string terminals are returned.
// Symbol: ?get_accName@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CMFCBaseTabCtrl__MEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCBaseTabCtrl* pThis, VARIANT varChildID, wchar_t** /*pszName*/)
{
    if (!pThis) return S_OK;
    if (varChildID.vt != 3 /*VT_I4*/) return S_OK;
    // TODO(clean-room): partially transcribed -- the control window text
    // (lVal == 0) and the per-tab ACC name string (accData +0x360 via the
    // vslot-0x5b0 virtual) are not modeled, so no name is produced.
    return (varChildID.lVal > 0) ? S_FALSE : S_OK;
}
// Retail (0x18001b1f0): for the control itself (VT_I4, lVal == 0) it reports
// ROLE_SYSTEM_TABCONTROL (0x3c); for lVal > 0 it resolves the tab's ACC role
// (accData m_nAccRole at +0x390 via the vslot-0x5b0 virtual); any other
// combination returns E_INVALIDARG. The +0x390 block is unmodeled, so the
// per-tab role falls back to 0 with the retail S_OK terminal.
// Symbol: ?get_accRole@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCBaseTabCtrl__MEAAJUtagVARIANT__PEAU2__Z(
    CMFCBaseTabCtrl* pThis, VARIANT varChildID, VARIANT* pvarRole)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarRole) return E_INVALIDARG;
    if (varChildID.vt == 3 /*VT_I4*/ && varChildID.lVal == 0) {
        pvarRole->vt = 3;
        pvarRole->lVal = 0x3c;   // ROLE_SYSTEM_TABCONTROL
        return S_OK;
    }
    if (varChildID.vt == 3 /*VT_I4*/ && varChildID.lVal > 0) {
        pvarRole->vt = 3;
        // TODO(clean-room): partially transcribed -- retail fills the per-tab
        // role from accData m_nAccRole (+0x390) via the vslot-0x5b0 virtual;
        // that block is not modeled (role 0).
        pvarRole->lVal = 0;
        return S_OK;
    }
    return E_INVALIDARG;
}
// Retail (0x18001b310): for the control itself (VT_I4, lVal == 0) it reports
// {VT_I4, 0}; for lVal > 0 it resolves the tab's ACC state (accData
// m_bAccState at +0x394 via the vslot-0x5b0 virtual); any other combination
// returns E_INVALIDARG. The +0x394 block is unmodeled, so the per-tab state
// falls back to 0 with the retail S_OK terminal.
// Symbol: ?get_accState@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCBaseTabCtrl__MEAAJUtagVARIANT__PEAU2__Z(
    CMFCBaseTabCtrl* pThis, VARIANT varChildID, VARIANT* pvarState)
{
    if (!pThis) return E_INVALIDARG;
    if (!pvarState) return E_INVALIDARG;
    if (varChildID.vt == 3 /*VT_I4*/ && varChildID.lVal == 0) {
        pvarState->vt = 3;
        pvarState->lVal = 0;
        return S_OK;
    }
    if (varChildID.vt == 3 /*VT_I4*/ && varChildID.lVal > 0) {
        pvarState->vt = 3;
        // TODO(clean-room): partially transcribed -- retail fills the per-tab
        // state from accData m_bAccState (+0x394) via the vslot-0x5b0 virtual;
        // that block is not modeled (state 0).
        pvarState->lVal = 0;
        return S_OK;
    }
    return E_INVALIDARG;
}
// Retail (0x18001b270): a non-VT_I4 child id returns S_FALSE. For lVal == 0 it
// returns the active tab's label (record +0x8) as a BSTR; for lVal > 0 it
// resolves the tab's ACC value (accData m_strAccValue at +0x368 via the
// vslot-0x5b0 virtual). Both strings are unmodeled (empty), so the
// empty-string S_FALSE terminal is returned for every valid input.
// Symbol: ?get_accValue@CMFCBaseTabCtrl@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_CMFCBaseTabCtrl__MEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCBaseTabCtrl* pThis, VARIANT varChildID, wchar_t** /*pszValue*/)
{
    if (!pThis) return S_FALSE;
    if (varChildID.vt != 3 /*VT_I4*/) return S_FALSE;
    // TODO(clean-room): partially transcribed -- the active-tab label (record
    // +0x8) and the ACC value string (accData +0x368) are not modeled, so the
    // empty-string S_FALSE terminal is returned.
    return S_FALSE;
}
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
CMFCBaseTabCtrl::CMFCBaseTabCtrl() {
    memset(_mfcbasetabctrl_padding, 0, sizeof(_mfcbasetabctrl_padding));
}
CMFCBaseTabCtrl::~CMFCBaseTabCtrl() {
    RemoveTabCtrlState(this);
}
BOOL CMFCBaseTabCtrl::Create(DWORD, const RECT&, CWnd*, UINT) { return TRUE; }
int CMFCBaseTabCtrl::GetTabsCount() const {
    const TabCtrlState* state = FindTabCtrlState(this);
    return state ? static_cast<int>(state->tabs.size()) : 0;
}
void CMFCBaseTabCtrl::AddTab(CWnd* pWnd, const wchar_t* lpszLabel, UINT uiId, BOOL bDetachable) {
    if (!pWnd) {
        return;
    }

    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabs.push_back({
        pWnd,
        lpszLabel ? lpszLabel : L"",
        uiId,
        bDetachable,
    });
    if (state.activeTab < 0) {
        state.activeTab = 0;
    }
}
int CMFCBaseTabCtrl::RemoveTab(int nIndex, int) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.tabs.size())) {
        return 0;
    }

    state.tabs.erase(state.tabs.begin() + nIndex);

    if (state.tabs.empty()) {
        state.activeTab = -1;
    } else if (state.activeTab == nIndex) {
        state.activeTab = (nIndex == 0) ? 0 : nIndex - 1;
    } else if (state.activeTab > nIndex) {
        --state.activeTab;
    }

    return 1;
}
void CMFCBaseTabCtrl::SetActiveTab(int nIndex) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.tabs.size())) {
        state.activeTab = -1;
        return;
    }
    state.activeTab = nIndex;
}
int CMFCBaseTabCtrl::GetActiveTab() const {
    const TabCtrlState* state = FindTabCtrlState(this);
    return state ? state->activeTab : -1;
}
CWnd* CMFCBaseTabCtrl::GetTabWnd(int nIndex) const {
    const TabCtrlState* state = FindTabCtrlState(this);
    if (!state) {
        return nullptr;
    }
    if (nIndex < 0 || nIndex >= static_cast<int>(state->tabs.size())) {
        return nullptr;
    }
    return state->tabs[nIndex].window;
}
BOOL CMFCBaseTabCtrl::IsTabDetachable(int nIndex) const {
    const TabCtrlState* state = FindTabCtrlState(this);
    if (!state) {
        return FALSE;
    }
    if (nIndex < 0 || nIndex >= static_cast<int>(state->tabs.size())) {
        return FALSE;
    }
    return state->tabs[nIndex].detachable;
}
