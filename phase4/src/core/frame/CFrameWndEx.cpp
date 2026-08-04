// CFrameWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_core-frames-1_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// CFrameWndEx methods, wave 1 (core-frames-1). Every function here was decoded
// from the retail mfc140u export (ordinal -> RVA -> objdump, see
// /tmp/openmfc_remaining/disasm_out) rather than inferred.
//
// Retail CFrameWndEx embeds a CDockingManager sub-object at this+0x380 and
// additional frame state at this+0x1f0 / this+0x370 / this+0x530 / this+0x548.
// OpenMFC's simplified CFrameWndEx (afxmfc.h) instead holds a lazily-allocated
// CDockingManager* (m_pDockingManager), so every retail `lea 0x380(%rcx)`
// delegation is routed through GetDockingManager() and every access to an
// unmodeled byte offset is conservatively skipped with a `TODO(clean-room)`
// note, per IMPLEMENTATION_GUIDE.md.
//
// Where a retail body depends on virtual slots or internal unexported helpers
// that the OpenMFC headers do not model, a type-correct conservative version is
// emitted (safe defaults, no state corruption) with a `TODO(clean-room)` note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
// CDockingManager (mfccore.cpp / thunks.cpp)
extern "C" void MS_ABI impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CDockingManager* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect);
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
    CDockingManager* pThis, CPane* pBarToDock, CPane* pBar);
extern "C" int MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(
    CDockingManager* pThis, unsigned long dwAlignment);
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pPane, CBasePane* pTarget, int nInsertAfter);
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int bCanFloat, int bSelect, int bSkipLayout);
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames);
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(
    CDockingManager* pThis, void* pParam);
// CFrameWnd (frame_font_exports.cpp / thunks.cpp)
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(
    CFrameWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" void MS_ABI impl__OnContextHelp_CFrameWnd__QEAAXXZ(CFrameWnd* pThis);
extern "C" int MS_ABI impl__OnCreate_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pCreateStruct);
extern "C" void MS_ABI impl__OnDestroy_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnClose_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
extern "C" void MS_ABI impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(
    CFrameWnd* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized);
// CWnd (global_core-windows_impl.cpp)
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned long long data, unsigned int command);

// CPoint arrives by value as one 8-byte integer register under MS x64; the
// handlers below take it as `unsigned long long` and do not unpack it.

//=============================================================================
// Simple / no-op methods
//=============================================================================




//=============================================================================
// Docking delegation (retail forwards to the embedded CDockingManager at
// this+0x380; OpenMFC routes through the lazily-allocated m_pDockingManager)
//=============================================================================










//=============================================================================
// Frame/menu lifecycle delegation
//=============================================================================













//=============================================================================
// Message-handler stubs (unmodeled state)
//=============================================================================






// Symbol: ?CreateObject@CFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWndEx__SAPEAVCObject__XZ() {
    return new CFrameWndEx();
}
// Symbol: ?GetThisClass@CFrameWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CFrameWndEx::classCFrameWndEx;
}
// Symbol: ?GetRuntimeClass@CFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWndEx__UEBAPEAUCRuntimeClass__XZ(CFrameWndEx* pThis) {
    return pThis ? &CFrameWndEx::classCFrameWndEx : nullptr;
}
// Symbol: ?OnShowPopupMenu@CFrameWndEx@@UEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(CFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
    return FALSE;
}
// Symbol: ?OnClosePopupMenu@CFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(CFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
}
// Symbol: ?OnShowPanes@CFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_CFrameWndEx__UEAAHH_Z(CFrameWndEx* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return TRUE;
}
// Symbol: ?OnToolbarCreateNew@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_CFrameWndEx__QEAA_J_K_J_Z(CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// CFrameWndEx::ActiveItemRecalcLayout() — retail (RVA 0x68370): asks the frame
// for its in-place active item (vtable 0x3a8), notifies the item's OLE map
// (unmodeled item+0xa8 / vtable 0xb0) and ends at AdjustClientArea (vtable
// 0x3a0). The active-item map is not modeled, so the layout recalc is kept.
// Symbol: ?ActiveItemRecalcLayout@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ActiveItemRecalcLayout_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout();
    // TODO(clean-room): transcribed partially — retail consults the OLE
    // in-place item map (unmodeled) before the layout recalc.
}
// CFrameWndEx::AddDockSite() — retail (RVA 0x27d0) is a bare `ret $0` (no-op).
// Symbol: ?AddDockSite@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    (void)pThis;
}
// CFrameWndEx::AdjustClientArea() — retail (RVA 0x67ed0) performs layout math on
// the frame rect (this+0x110..0x120) and the border rect (this+0x548) after
// querying an unmodeled helper window. Skipped conservatively.
// Symbol: ?AdjustClientArea@CFrameWndEx@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CFrameWndEx__MEAAXXZ(CFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): transcribed partially — layout rects at this+0x110/0x548
    // and the helper-window RTTI path are not modeled in the simplified headers.
}
// CFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail (RVA 0x67dc0):
//     add $0x380,%rcx; jmp CDockingManager::DockPane
// Symbol: ?DockPane@CFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CFrameWndEx* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    if (!pThis) return;
    impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
        pThis->GetDockingManager(), pBar, nDockBarID, lpRect);
}
// CFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail (RVA 0x67970):
//     add $0x380,%rcx; jmp CDockingManager::DockPaneLeftOf
// Symbol: ?DockPaneLeftOf@CFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CFrameWndEx__QEAAHPEAVCPane__0_Z(
    CFrameWndEx* pThis, CPane* pBarToDock, CPane* pBar) {
    if (!pThis) return FALSE;
    return impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
        pThis->GetDockingManager(), pBarToDock, pBar);
}
// CFrameWndEx::EnableAutoHidePanes(DWORD) — retail (RVA 0x68020):
//     add $0x380,%rcx; jmp CDockingManager::EnableAutoHidePanes
// Symbol: ?EnableAutoHidePanes@CFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CFrameWndEx__QEAAHK_Z(
    CFrameWndEx* pThis, unsigned long dwAlignment) {
    if (!pThis) return FALSE;
    return impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(
        pThis->GetDockingManager(), dwAlignment);
}
// CFrameWndEx::InsertPane(CBasePane*, CBasePane*, int) — retail (RVA 0x67da0):
//     add $0x380,%rcx; jmp CDockingManager::InsertPane
// Symbol: ?InsertPane@CFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    CFrameWndEx* pThis, CBasePane* pPane, CBasePane* pTarget, int nInsertAfter) {
    if (!pThis) return FALSE;
    return impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
        pThis->GetDockingManager(), pPane, pTarget, nInsertAfter);
}
// CFrameWndEx::AddPane(CBasePane*, BOOL bSelect) — retail (RVA 0x67d00)
// performs RTTI class checks on pBar, stashes it into this+0x370/0x378 and then
// forwards to CDockingManager::AddPane(pBar, bSelect, FALSE, FALSE). The RTTI
// checks and the unmodeled 0x370/0x378 slots are skipped; the docking-manager
// registration is forwarded.
// Symbol: ?AddPane@CFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CFrameWndEx__QEAAHPEAVCBasePane__H_Z(
    CFrameWndEx* pThis, CBasePane* pBar, int bSelect) {
    if (!pThis || !pBar) return FALSE;
    // TODO(clean-room): transcribed partially — retail first checks IsKindOf
    // (RuntimeClass 0x2fea10 / 0x30d170) and caches pBar at this+0x370/0x378.
    return impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
        pThis->GetDockingManager(), pBar, bSelect, bSelect, 0);
}
// CFrameWndEx::GetPane(UINT nID) — retail (RVA 0x68030):
//     add $0x380,%rcx; r8d=1; CDockingManager::GetPane(nID, TRUE)
// CDockingManager::GetPane(nID, TRUE) is FindPaneByID(nID, TRUE) in MFC.
// Symbol: ?GetPane@CFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" CBasePane* MS_ABI impl__GetPane_CFrameWndEx__QEAAPEAVCBasePane__I_Z(
    CFrameWndEx* pThis, unsigned int nID) {
    if (!pThis) return nullptr;
    return impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
        pThis->GetDockingManager(), nID, TRUE);
}
// CFrameWndEx::IsPointNearDockSite(CPoint, DWORD&, int&) — retail (RVA 0x67e40)
// forwards to an internal CDockingManager helper (0x180048310) that has no
// public export to delegate to. Conservative FALSE.
// Symbol: ?IsPointNearDockSite@CFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const CFrameWndEx* pThis, unsigned long long pt, unsigned long& dwDockBarMask, int& nDistance) {
    (void)pThis;
    (void)pt;
    (void)dwDockBarMask;
    (void)nDistance;
    // TODO(clean-room): transcribed partially — target CDockingManager method
    // is not exported.
    return FALSE;
}
// CFrameWndEx::AdjustDockingLayout(void*) — retail (RVA 0x67e50) checks a
// CDockingManager flag, calls its AdjustDockingLayout, then the frame's own
// AdjustClientArea / RecalcLayout chain. The frame-level extras are dropped;
// the docking-manager relayout is forwarded.
// Symbol: ?AdjustDockingLayout@CFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CFrameWndEx__UEAAXPEAX_Z(
    CFrameWndEx* pThis, void* pParam) {
    if (!pThis) return;
    impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(
        pThis->GetDockingManager(), pParam);
    // TODO(clean-room): transcribed partially — retail also runs the frame's
    // AdjustClientArea and a conditional RecalcLayout(1) on this+0x530 state.
}
// CFrameWndEx::OnExitSizeMove() — retail (RVA 0x68730) calls the frame
// RecalcLayout(1) then a CDockingManager virtual. Replicate both pieces.
// Symbol: ?OnExitSizeMove@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnExitSizeMove_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout();
    CDockingManager* pDock = pThis->GetDockingManager();
    if (pDock) pDock->RecalcLayout();
}
// CFrameWndEx::DelayUpdateFrameMenu(HMENU) — retail (RVA 0x67a10) calls the
// frame OnSize virtual, sets a flag at this+0x1d0 and stores hMenuAlt at
// this+0x190 (unmodeled). The base CFrameWnd::DelayUpdateFrameMenu carries the
// documented contract.
// Symbol: ?DelayUpdateFrameMenu@CFrameWndEx@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__DelayUpdateFrameMenu_CFrameWndEx__UEAAXPEAUHMENU_____Z(
    CFrameWndEx* pThis, HMENU hMenuAlt) {
    if (!pThis) return;
    pThis->CFrameWnd::DelayUpdateFrameMenu(hMenuAlt);
    // TODO(clean-room): transcribed partially — retail toggles unmodeled
    // this+0x1d0 flag and this+0x190 menu-alt slot.
}
// CFrameWndEx::OnActivate(UINT, CWnd*, BOOL) — retail (RVA 0x67980) starts by
// calling the base CFrameWnd::OnActivate then handles state 2 (UpdateWindow)
// and state 0 (tooltip setup on this+0x1f0). The base dispatch is kept.
// Symbol: ?OnActivate@CFrameWndEx@@QEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CFrameWndEx__QEAAXIPEAVCWnd__H_Z(
    CFrameWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    if (!pThis) return;
    impl__OnActivate_CFrameWnd__IEAAXIPEAVCWnd__H_Z(pThis, nState, pWndOther, bMinimized);
    // TODO(clean-room): transcribed partially — state==2 UpdateWindow and the
    // state==0 tooltip setup touch unmodeled state (this+0x1f0).
}
// CFrameWndEx::OnActivateApp(BOOL, DWORD) — retail (RVA 0x679d0) forwards to a
// CDockingManager virtual (vtable 0xc0) and an internal 0x1f0 helper. Neither is
// reachable from the simplified headers. Conservative no-op.
// Symbol: ?OnActivateApp@CFrameWndEx@@QEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CFrameWndEx__QEAAXHK_Z(
    CFrameWndEx* pThis, int bActive, unsigned long dwThreadID) {
    (void)pThis;
    (void)bActive;
    (void)dwThreadID;
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::OnClose() — retail (RVA 0x677a0) has an extra owner-frame
// WM_COMMAND(0xe300) fast path (this+0x6e8, unmodeled) before delegating to
// CFrameWnd::OnClose. The base close is kept.
// Symbol: ?OnClose@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnClose_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnClose_CFrameWnd__IEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially — retail's this+0x6e8 owner
    // fast path and in-place item teardown are not modeled.
}
// CFrameWndEx::OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) — retail
// (RVA 0x68830): if the base CFrameWnd::OnCmdMsg handles the command return 1,
// else forward to the embedded CDockingManager's OnCmdMsg. The docking manager
// has no exported OnCmdMsg, so only the base dispatch is performed.
// Symbol: ?OnCmdMsg@CFrameWndEx@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWndEx__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWndEx* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    int handled = impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis, nID, nCode, pExtra, pHandlerInfo);
    return handled ? TRUE : FALSE;
    // TODO(clean-room): transcribed partially — retail additionally forwards
    // unhandled commands to the embedded CDockingManager::OnCmdMsg.
}
// CFrameWndEx::OnCommand(WPARAM, LPARAM) — retail (RVA 0x67690) special-cases
// menu commands before falling back to the base CFrameWnd::OnCommand. The base
// dispatch is kept.
// Symbol: ?OnCommand@CFrameWndEx@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CFrameWndEx__MEAAH_K_J_Z(
    CFrameWndEx* pThis, WPARAM wParam, LPARAM lParam) {
    if (!pThis) return FALSE;
    return impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(pThis, wParam, lParam);
    // TODO(clean-room): transcribed partially — retail menu-command fast path
    // (wParam hi-word==1, this+0x1f0 sub-object) is not modeled.
}
// CFrameWndEx::OnCompositionChanged() — retail (RVA 0x68b80) tail-jumps into
// an internal helper on the unmodeled this+0x1f0 sub-object. Conservative no-op.
// Symbol: ?OnCompositionChanged@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::OnContextHelp() — retail (RVA 0x678b0) toggles an unmodeled
// this+0x1dc flag around a help-state loop (CWinApp helpers + context help
// toggles). The base CFrameWnd::OnContextHelp carries the documented contract.
// Symbol: ?OnContextHelp@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnContextHelp_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnContextHelp_CFrameWnd__QEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::OnCreate(CREATESTRUCTW*) — retail (RVA 0x68970): run the base
// CFrameWnd::OnCreate; if it fails return -1, otherwise stash a WS-style bit
// into this+0x218 and set up the unmodeled 0x1f0 sub-object before returning 0.
// Symbol: ?OnCreate@CFrameWndEx@@QEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CFrameWndEx__QEAAHPEAUtagCREATESTRUCTW___Z(
    CFrameWndEx* pThis, CREATESTRUCTW* pCreateStruct) {
    if (!pThis) return -1;
    int result = impl__OnCreate_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, pCreateStruct);
    if (result == -1) return -1;
    // TODO(clean-room): transcribed partially — retail records
    // (cs->dwStyle >> 23) & 1 at this+0x218 and initializes the this+0x1f0
    // sub-object; both unmodeled in the simplified headers.
    return 0;
}
// CFrameWndEx::OnDestroy() — retail (RVA 0x67ba0) is a large teardown
// (docking/tooltip state on unmodeled offsets) ending at CFrameWnd::OnDestroy.
// The base teardown is kept.
// Symbol: ?OnDestroy@CFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CFrameWndEx__QEAAXXZ(CFrameWndEx* pThis) {
    if (!pThis) return;
    impl__OnDestroy_CFrameWnd__IEAAXXZ(pThis);
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::OnDrawMenuImage(CDC*, const CMFCToolBarMenuButton*, const CRect&)
// — retail (RVA 0x67670): if this+0x370 (unmodeled custom-image owner) is
// non-null it draws via an internal helper, otherwise returns 0. The unmodeled
// owner cannot be consulted, so the empty result is returned.
// Symbol: ?OnDrawMenuImage@CFrameWndEx@@UEAAHPEAVCDC@@PEBVCMFCToolBarMenuButton@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__OnDrawMenuImage_CFrameWndEx__UEAAHPEAVCDC__PEBVCMFCToolBarMenuButton__AEBVCRect___Z(
    CFrameWndEx* pThis, CDC* pDC, const CMFCToolBarMenuButton* pButton, const CRect& rectImage) {
    (void)pThis;
    (void)pDC;
    (void)pButton;
    (void)rectImage;
    // TODO(clean-room): transcribed partially — retail consults the unmodeled
    // this+0x370 custom-menu-image owner.
    return 0;
}
// CFrameWndEx::OnGetMinMaxInfo(MINMAXINFO*) — retail (RVA 0x68770): if an
// unmodeled flag at this+0x338 is set, derives ptMaxSize/ptMaxTrackSize from
// unmodeled rect fields (this+0x320..0x32c); otherwise falls through to the
// default frame handler. Skipped conservatively.
// Symbol: ?OnGetMinMaxInfo@CFrameWndEx@@QEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFrameWndEx__QEAAXPEAUtagMINMAXINFO___Z(
    CFrameWndEx* pThis, MINMAXINFO* lpMMI) {
    (void)pThis;
    (void)lpMMI;
    // TODO(clean-room): transcribed partially — flag at this+0x338 and the
    // border-rect source fields are not modeled in the simplified headers.
}
// CFrameWndEx::OnChangeVisualManager() — retail (RVA 0x689e0): call an internal
// helper on the unmodeled this+0x1f0 sub-object, return 0. Only the constant
// return value is kept.
// Symbol: ?OnChangeVisualManager@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    // TODO(clean-room): transcribed partially.
    return 0;
}
// CFrameWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) — retail (RVA 0x68560): call
// an internal CDockingManager helper (0x18004d2a0), return 0. The helper is not
// exported; the constant return value is kept.
// Symbol: ?OnIdleUpdateCmdUI@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CFrameWndEx__QEAA_J_K_J_Z(
    CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    // TODO(clean-room): transcribed partially — retail forwards to the embedded
    // CDockingManager idle-update helper before returning 0.
    return 0;
}
// CFrameWndEx::OnLButtonDown(UINT, CPoint) — retail (RVA 0x68b50) forwards to
// an internal helper on the unmodeled this+0x1f0 sub-object then to the base
// CWnd handler. Conservative no-op.
// Symbol: ?OnLButtonDown@CFrameWndEx@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CFrameWndEx__QEAAXIVCPoint___Z(
    CFrameWndEx* pThis, unsigned int nFlags, unsigned long long pt) {
    (void)pThis;
    (void)nFlags;
    (void)pt;
    // TODO(clean-room): transcribed partially.
}
// CFrameWndEx::GetInPlaceActiveItem() — retail (RVA 0x67a50): walks the active
// view (vtable 0x2f0), reads unmodeled view state (view+0x170), RTTI-checks
// against COleClientItem and returns the item from an unmodeled map slot
// (vtable 0x230 on view+0xe8). The OpenMFC view layout does not carry that
// state, so nullptr is returned (safe "no active item").
// Symbol: ?GetInPlaceActiveItem@CFrameWndEx@@MEAAPEAVCOleClientItem@@XZ
extern "C" COleClientItem* MS_ABI impl__GetInPlaceActiveItem_CFrameWndEx__MEAAPEAVCOleClientItem__XZ(
    CFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): transcribed partially — active-view OLE item map
    // (view+0x170 / view+0xe8) not modeled in the simplified headers.
    return nullptr;
}
// CFrameWndEx::NegotiateBorderSpace(UINT, RECT*) — retail (RVA 0x683d0): for
// nBorderCmd 1 and 3 calls an internal border-space helper (0x1802a03a0), for
// cmd 1 additionally copies the internal border rect (this+0x548, unmodeled)
// into lpRectBorder, and always returns 1. The constant result is kept.
// Symbol: ?NegotiateBorderSpace@CFrameWndEx@@UEAAHIPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__NegotiateBorderSpace_CFrameWndEx__UEAAHIPEAUtagRECT___Z(
    CFrameWndEx* pThis, unsigned int nBorderCmd, RECT* lpRectBorder) {
    (void)pThis;
    (void)nBorderCmd;
    (void)lpRectBorder;
    // TODO(clean-room): transcribed partially — border-space helper not exported;
    // source rect at this+0x548 not modeled in the simplified headers.
    return 1;
}
// CFrameWndEx::HtmlHelpW(ULONG_PTR, UINT) — retail (RVA 0x67870): when
// (dwData != 0) or the unmodeled this+0x1dc context-help flag is clear it
// forwards to the generic CWnd::HtmlHelpW; otherwise it runs the on-window
// context-help path. The unmodeled flag defaults to inactive, so the generic
// dispatch is used.
// Symbol: ?HtmlHelpW@CFrameWndEx@@MEAAX_KI@Z
extern "C" void MS_ABI impl__HtmlHelpW_CFrameWndEx__MEAAX_KI_Z(
    CFrameWndEx* pThis, unsigned __int64 data, unsigned int command) {
    if (!pThis) return;
    impl__HtmlHelpW_CWnd__UEAAX_KI_Z(pThis, data, command);
    // TODO(clean-room): transcribed partially — retail's this+0x1dc
    // context-help branch is not modeled.
}
// Symbol: ?OnUpdatePaneMenu@CFrameWndEx@@QEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePaneMenu_CFrameWndEx__QEAAXPEAVCCmdUI___Z(CFrameWndEx*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
CFrameWndEx::CFrameWndEx()
    : m_pVisualManager(nullptr), m_pDockingManager(nullptr) {
    memset(_framewndex_padding, 0, sizeof(_framewndex_padding));
}
CFrameWndEx::~CFrameWndEx() {
    if (m_pDockingManager) delete m_pDockingManager;
}
BOOL CFrameWndEx::Create(const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
                          const RECT& rect, CWnd* pParentWnd, const wchar_t* lpszMenuName,
                          DWORD dwExStyle, CCreateContext* pContext) {
    return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect,
                             pParentWnd, lpszMenuName, dwExStyle, pContext);
}
BOOL CFrameWndEx::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
CMFCVisualManager* CFrameWndEx::GetVisualManager() const { return m_pVisualManager; }
CDockingManager* CFrameWndEx::GetDockingManager() {
    if (!m_pDockingManager) m_pDockingManager = new CDockingManager(this);
    return m_pDockingManager;
}
BOOL CFrameWndEx::EnableDocking(DWORD dwDockStyle) {
    if (!m_pDockingManager) m_pDockingManager = new CDockingManager(this);
    return m_pDockingManager && m_pDockingManager->EnableDocking(dwDockStyle);
}
