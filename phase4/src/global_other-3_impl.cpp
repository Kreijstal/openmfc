// OpenMFC — CMDIFrameWndEx / CMDIChildWndEx method stubs (wave1_other-3).
//
// Every function here was decoded from the retail mfc140u export (ordinal ->
// RVA -> objdump, see /tmp/openmfc_remaining/disasm.py) rather than inferred.
//
// Retail CMDIFrameWndEx embeds a CDockingManager sub-object at this+0x228 and a
// CMDIClientAreaWnd at this+0x720; retail CMDIChildWndEx embeds a CFrameImpl at
// this+0x218 and a CDockingManager at this+0x3a8. OpenMFC's simplified headers
// (afxmfc.h) model neither class beyond a padding blob, so every retail access
// to those offsets is conservatively skipped with a `TODO(clean-room)` note per
// IMPLEMENTATION_GUIDE.md; no offsets are invented and no header is edited.
//
// Where a retail body is a bare `ret` / `xor eax,eax; ret`, the function is
// transcribed faithfully. Message handlers that end in a tail call to a base
// export are routed through the corresponding sibling impl_ function. The rest
// are type-correct conservative versions (safe defaults, no state corruption).

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
// CWnd::Default (thunks.cpp)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
// CFrameWnd::OnSysCommand (thunks.cpp)
extern "C" void MS_ABI impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(
    CFrameWnd* pThis, unsigned int nID, __int64 lParam);
// CMDIChildWnd::OnUpdateFrameTitle / PreCreateWindow / PreTranslateMessage
// (frame_font_exports.cpp)
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(
    CMDIChildWnd* pThis, int bAddToTitle);
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(
    CMDIChildWnd* pThis, MSG* pMsg);

//=============================================================================
// CMDIChildWndEx — message handlers
//=============================================================================

// CMDIChildWndEx::OnSetText(const wchar_t*) — retail (RVA 0x84720) calls
// CWnd::Default() and propagates its result, then forwards the text to the
// parent MDI frame's client area (this+0x718/+0x720 UpdateTabs), runs
// CFrameImpl::OnSetText (this+0x218) and a vtable-0x458 virtual. The base
// result is preserved; the unmodeled sub-object updates are skipped.
// Symbol: ?OnSetText@CMDIChildWndEx@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMDIChildWndEx__IEAAHPEB_W_Z(
    CMDIChildWndEx* pThis, const wchar_t* lpszText) {
    if (!pThis) return 0;
    int nResult = static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
    // TODO(clean-room): transcribed partially — retail also updates the MDI
    // frame client area (this+0x718 +0x720), CFrameImpl (this+0x218) and a
    // vtable-0x458 virtual with lpszText; unmodeled.
    return nResult;
}

// CMDIChildWndEx::OnSize(UINT, int, int) — retail (RVA 0x84c60) drives the
// MDI-tab client area, the CFrameImpl (this+0x218) and the embedded docking
// manager (this+0x3a8) with unmodeled flags at this+0x1f0/0x1f4/0x1f8/0x23c/
// 0x3b8. No base dispatch is involved. Conservative no-op.
// Symbol: ?OnSize@CMDIChildWndEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIChildWndEx__IEAAXIHH_Z(
    CMDIChildWndEx* pThis, unsigned int nType, int cx, int cy) {
    (void)pThis;
    (void)nType;
    (void)cx;
    (void)cy;
    // TODO(clean-room): transcribed partially — retail touches unmodeled frame
    // state (this+0x1f0/0x1f4/0x1f8/0x23c/0x3b8, CFrameImpl at 0x218, dock
    // manager at 0x3a8) before relayout.
}

// CMDIChildWndEx::OnSizing(UINT, LPRECT) — retail (RVA 0x85520) calls
// CWnd::Default(), compares the current window rect against the proposed rect
// and calls RecalcLayout(FALSE) when the sizes differ. The comparison is
// faithful (m_hWnd at 0x40 is modeled); the virtual RecalcLayout dispatch is
// routed to the base layout instead of the derived vtable slot.
// Symbol: ?OnSizing@CMDIChildWndEx@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CMDIChildWndEx__IEAAXIPEAUtagRECT___Z(
    CMDIChildWndEx* pThis, unsigned int fwSide, RECT* pRect) {
    (void)fwSide;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (pThis->GetSafeHwnd() && pRect) {
        RECT rectWnd;
        ::GetWindowRect(pThis->GetSafeHwnd(), &rectWnd);
        const int nWndW = rectWnd.right - rectWnd.left;
        const int nWndH = rectWnd.bottom - rectWnd.top;
        const int nRectW = pRect->right - pRect->left;
        const int nRectH = pRect->bottom - pRect->top;
        if (nWndW != nRectW || nWndH != nRectH) {
            // TODO(clean-room): retail dispatches RecalcLayout(FALSE) through
            // the derived vtable; routed to the base layout conservatively.
            pThis->CFrameWnd::RecalcLayout(FALSE);
        }
    }
}

// CMDIChildWndEx::OnStyleChanged(int, LPSTYLESTRUCT) — retail (RVA 0x859c0)
// toggles MDI-tab visibility from the style bits (0x80000) using unmodeled
// CFrameImpl (this+0x218) / taskbar state (this+0x718) and screen-formatting
// helpers. Conservative no-op.
// Symbol: ?OnStyleChanged@CMDIChildWndEx@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanged_CMDIChildWndEx__IEAAXHPEAUtagSTYLESTRUCT___Z(
    CMDIChildWndEx* pThis, int nStyleType, void* lpStyleStruct) {
    (void)pThis;
    (void)nStyleType;
    (void)lpStyleStruct;
    // TODO(clean-room): transcribed partially — retail reacts to the WS_* /
    // 0x80000 style-bit change with unmodeled MDI-tab updates.
}

// CMDIChildWndEx::OnSysCommand(UINT, LPARAM) — retail (RVA 0x85e20): for
// SC_CLOSE (0xf100) it suppresses the base close when a tabbed control bar is
// attached (this+0x718) and non-auto-hidden; otherwise it tail-calls
// CFrameWnd::OnSysCommand. The suppression test is unmodeled, so the base
// dispatch is always performed.
// Symbol: ?OnSysCommand@CMDIChildWndEx@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMDIChildWndEx__IEAAXI_J_Z(
    CMDIChildWndEx* pThis, unsigned int nID, __int64 lParam) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail skips the close for
    // SC_CLOSE when the unmodeled this+0x718 tabbed-bar slot is active.
    impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(pThis, nID, lParam);
}

// CMDIChildWndEx::OnTaskbarTabThumbnailActivate(UINT, CWnd*, BOOL) — retail
// (RVA 0x866f0): when nState == 1 it activates the top-level frame through
// vtable slot 0x450 (ActivateTopLevelFrame). The vtable slot is not modeled in
// the simplified headers. Conservative no-op.
// Symbol: ?OnTaskbarTabThumbnailActivate@CMDIChildWndEx@@UEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnTaskbarTabThumbnailActivate_CMDIChildWndEx__UEAAXIPEAVCWnd__H_Z(
    CMDIChildWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    (void)pWndOther;
    (void)bMinimized;
    if (!pThis) return;
    if (nState == 1) {
        // TODO(clean-room): retail calls ActivateTopLevelFrame (vtable 0x450);
        // not modeled in the simplified headers.
    }
}

// CMDIChildWndEx::OnTaskbarTabThumbnailMouseActivate(CWnd*, UINT, UINT) —
// retail (RVA 0x86710) returns TRUE unconditionally; on WM_LBUTTONUP (0x202)
// it also activates the top-level frame (vtable 0x450). The constant result is
// kept; the vtable dispatch is unmodeled.
// Symbol: ?OnTaskbarTabThumbnailMouseActivate@CMDIChildWndEx@@UEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnTaskbarTabThumbnailMouseActivate_CMDIChildWndEx__UEAAHPEAVCWnd__II_Z(
    CMDIChildWndEx* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — retail also activates the
    // top-level frame (vtable 0x450) on WM_LBUTTONUP.
    return TRUE;
}

// CMDIChildWndEx::OnTaskbarTabThumbnailStretch(HBITMAP, const CRect&, HBITMAP,
// const CRect&) — retail (RVA 0x86270) performs a full GDI/D2D stretch/blend
// pipeline (bitmap create, color management, alpha blend) with a complex helper
// chain that is not modeled. Returns FALSE on null bitmaps; conservative FALSE.
// Symbol: ?OnTaskbarTabThumbnailStretch@CMDIChildWndEx@@UEAAHPEAUHBITMAP__@@AEBVCRect@@01@Z
extern "C" int MS_ABI impl__OnTaskbarTabThumbnailStretch_CMDIChildWndEx__UEAAHPEAUHBITMAP____AEBVCRect__01_Z(
    CMDIChildWndEx* pThis, HBITMAP hBmpDst, const CRect& rectDst, HBITMAP hBmpSrc, const CRect& rectSrc) {
    (void)pThis;
    (void)hBmpDst;
    (void)rectDst;
    (void)hBmpSrc;
    (void)rectSrc;
    // TODO(clean-room): transcribed partially — retail GDI/D2D stretch pipeline
    // (CreateBitmap / ColorMatrix / AlphaBlend) is not modeled.
    return FALSE;
}

// CMDIChildWndEx::OnUpdateFrameTitle(BOOL) — retail (RVA 0x84ad0) consults the
// CWinApp frame state, this+0x248 flag, GetStyle() bit 0x10000 and the
// taskbar-tab text before WM_NCACTIVATE / MDI-client UpdateTabs. The base
// CMDIChildWnd::OnUpdateFrameTitle carries the documented contract; extras are
// skipped.
// Symbol: ?OnUpdateFrameTitle@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bAddToTitle) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail also handles
    // WM_NCACTIVATE, MDI-client UpdateTabs and taskbar-tab text via unmodeled
    // state.
    impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(pThis, bAddToTitle);
}

// CMDIChildWndEx::OnWindowPosChanged(WINDOWPOS*) — retail (RVA 0x85ec0): when
// lpWndPos->flags has SWP_FRAMECHANGED (0x20) it updates the tabbed-bar layout
// (this+0x218 / this+0x718) and clears the autohide flag at this+0x23c, then
// unconditionally tail-calls CWnd::Default(). The base dispatch is kept.
// Symbol: ?OnWindowPosChanged@CMDIChildWndEx@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMDIChildWndEx__IEAAXPEAUtagWINDOWPOS___Z(
    CMDIChildWndEx* pThis, WINDOWPOS* lpWndPos) {
    if (!pThis) return;
    if (lpWndPos && (lpWndPos->flags & SWP_FRAMECHANGED)) {
        // TODO(clean-room): retail updates the tabbed-bar layout (this+0x218 /
        // this+0x718) and clears the autohide flag at this+0x23c here.
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const —
// retail (RVA 0x850c0) delegates to the embedded CDockingManager at this+0x3a8.
// The dock manager is not modeled in the simplified headers. Conservative NULL.
// Symbol: ?PaneFromPoint@CMDIChildWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIChildWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const CMDIChildWndEx* pThis, unsigned long long pt, int nSensitivity,
    unsigned long* dwAlignment, CRuntimeClass* pRTCBarType) {
    (void)pt;
    (void)nSensitivity;
    (void)pRTCBarType;
    if (!pThis) return nullptr;
    // TODO(clean-room): retail forwards to the embedded CDockingManager
    // (this+0x3a8) PaneFromPoint; not modeled.
    if (dwAlignment) *dwAlignment = 0;
    return nullptr;
}

// CMDIChildWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const —
// retail (RVA 0x85080) delegates to the embedded CDockingManager at this+0x3a8.
// The dock manager is not modeled. Conservative NULL.
// Symbol: ?PaneFromPoint@CMDIChildWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIChildWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const CMDIChildWndEx* pThis, unsigned long long pt, int nSensitivity,
    bool bExactBar, CRuntimeClass* pRTCBarType) {
    (void)pt;
    (void)nSensitivity;
    (void)bExactBar;
    (void)pRTCBarType;
    if (!pThis) return nullptr;
    // TODO(clean-room): retail forwards to the embedded CDockingManager
    // (this+0x3a8) PaneFromPoint; not modeled.
    return nullptr;
}

// CMDIChildWndEx::PreCreateWindow(CREATESTRUCTW&) — retail (RVA 0x84e80) caches
// `this` at this+0x558 and tail-calls CMDIChildWnd::PreCreateWindow. The
// self-pointer slot is unmodeled; the base precreate is performed.
// Symbol: ?PreCreateWindow@CMDIChildWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWndEx* pThis, CREATESTRUCTW& cs) {
    if (!pThis) return FALSE;
    // TODO(clean-room): retail stores `this` at this+0x558 (unmodeled) before
    // the base call.
    return impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}

// CMDIChildWndEx::PreTranslateMessage(MSG*) — retail (RVA 0x83990) has keyboard
// fast paths (0x100..0x109 suppression when a tabbed bar is present, and a
// WM_KEYDOWN+VK_ESCAPE close path through unmodeled this+0x718/0x1e8 and a
// global flag) before falling back to CMDIChildWnd::PreTranslateMessage. The
// base dispatch is kept.
// Symbol: ?PreTranslateMessage@CMDIChildWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWndEx__UEAAHPEAUtagMSG___Z(
    CMDIChildWndEx* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — retail keyboard fast paths
    // rely on unmodeled this+0x718 / this+0x1e8 state and a global flag.
    return impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// CMDIChildWndEx::RecalcLayout(BOOL) — retail (RVA 0x852b0) drives the embedded
// CDockingManager (this+0x3a8), the MDI client (this+0x558) and the internal
// AdjustClientArea (0x180085140) behind reentrancy flags (this+0x1a0/0x1f8).
// The base frame relayout is performed; the unmodeled dock/client state is
// skipped.
// Symbol: ?RecalcLayout@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bNotify) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout(bNotify);
    // TODO(clean-room): transcribed partially — retail also runs the embedded
    // CDockingManager (this+0x3a8) layout and AdjustClientArea behind the
    // reentrancy flags at this+0x1a0/0x1f8.
}

// CMDIChildWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL,
// CBasePane*) — retail (RVA 0x84ec0) tail-jumps to the embedded CDockingManager
// (this+0x3a8) RemovePaneFromDockManager. Not modeled. Conservative no-op.
// Symbol: ?RemovePaneFromDockManager@CMDIChildWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CMDIChildWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, int bDestroy,
    int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement) {
    (void)pControlBar;
    (void)bDestroy;
    (void)bAdjustLayout;
    (void)bAutoHide;
    (void)pBarReplacement;
    if (!pThis) return;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager
    // (this+0x3a8) RemovePaneFromDockManager; not modeled.
}

// CMDIChildWndEx::ShowPane(CBasePane*, BOOL, BOOL, BOOL) — retail (RVA 0x68050)
// dispatches a CBasePane virtual (vtable 0x458) to show/hide the pane. The
// retail pane vtable is not modeled. Conservative no-op.
// Symbol: ?ShowPane@CMDIChildWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_CMDIChildWndEx__QEAAXPEAVCBasePane__HHH_Z(
    CMDIChildWndEx* pThis, CBasePane* pBar, int bShow, int bDelay, int bActivate) {
    (void)pBar;
    (void)bShow;
    (void)bDelay;
    (void)bActivate;
    if (!pThis) return;
    // TODO(clean-room): retail dispatches a CBasePane virtual (vtable 0x458);
    // the retail pane vtable is not modeled.
}

// CMDIChildWndEx::UpdateTaskbarTabIcon(HICON) — retail (RVA 0x841e0) posts
// WM_SETICON (0x80) to the taskbar-tab proxy window (this+0x720/+0x760). The
// proxy window is not modeled. Conservative no-op.
// Symbol: ?UpdateTaskbarTabIcon@CMDIChildWndEx@@UEAAXPEAUHICON__@@@Z
extern "C" void MS_ABI impl__UpdateTaskbarTabIcon_CMDIChildWndEx__UEAAXPEAUHICON_____Z(
    CMDIChildWndEx* pThis, HICON hIcon) {
    (void)hIcon;
    if (!pThis) return;
    // TODO(clean-room): retail sends WM_SETICON to the tab-proxy window
    // (this+0x720/+0x760); not modeled in the simplified headers.
}

//=============================================================================
// CMDIFrameWndEx — docking / MDI-tab operations
//=============================================================================

// CMDIFrameWndEx::ActiveItemRecalcLayout() — retail (RVA 0x895b0) asks the
// frame for its OLE in-place active item (vtable 0x468), notifies the item's
// OLE map (item+0xa8) and ends at a layout recalc. The active-item map is not
// modeled, so the base frame relayout is kept.
// Symbol: ?ActiveItemRecalcLayout@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__ActiveItemRecalcLayout_CMDIFrameWndEx__QEAAXXZ(
    CMDIFrameWndEx* pThis) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout();
    // TODO(clean-room): transcribed partially — retail consults the OLE
    // in-place item map (vtable 0x468) before the layout recalc.
}

// CMDIFrameWndEx::AddDockSite() — retail (RVA 0x27d0) is a bare `ret` (no-op).
// Symbol: ?AddDockSite@CMDIFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CMDIFrameWndEx__QEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
}

// CMDIFrameWndEx::AddPane(CBasePane*, BOOL) — retail (RVA 0x890e0) class-checks
// pBar (IsKindOf CMFCToolBar / CDockablePane), caches it at this+0x710/0x718
// and forwards to the embedded CDockingManager (this+0x228) AddPane. The dock
// manager is not modeled. Conservative FALSE.
// Symbol: ?AddPane@CMDIFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CMDIFrameWndEx__QEAAHPEAVCBasePane__H_Z(
    CMDIFrameWndEx* pThis, CBasePane* pBar, int bSelect) {
    (void)bSelect;
    if (!pThis || !pBar) return FALSE;
    // TODO(clean-room): transcribed partially — retail class-checks pBar
    // (IsKindOf CDockablePane), caches it at this+0x710/0x718 and forwards to
    // the embedded CDockingManager at this+0x228; not modeled.
    return FALSE;
}

// CMDIFrameWndEx::AdjustClientArea() — retail (RVA 0x892d0) computes the client
// rect from unmodeled frame rects (this+0x110/0x3f0) and resizes the MDI
// client-area window (this+0x720). Conservative no-op.
// Symbol: ?AdjustClientArea@CMDIFrameWndEx@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CMDIFrameWndEx__UEAAXXZ(CMDIFrameWndEx* pThis) {
    (void)pThis;
    // TODO(clean-room): transcribed partially — retail derives the client rect
    // from this+0x110/0x3f0 and resizes the MDI client area (this+0x720).
}

// CMDIFrameWndEx::AdjustDockingLayout(HDWP) — retail (RVA 0x89250) forwards to
// the embedded CDockingManager (this+0x228) AdjustDockingLayout, then runs the
// frame's AdjustClientArea and a conditional RecalcLayout. The dock manager is
// not modeled. Conservative no-op.
// Symbol: ?AdjustDockingLayout@CMDIFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIFrameWndEx__UEAAXPEAX_Z(
    CMDIFrameWndEx* pThis, void* pParam) {
    (void)pParam;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail forwards to the embedded
    // CDockingManager (this+0x228) and then runs AdjustClientArea / RecalcLayout.
}

// CMDIFrameWndEx::ControlBarToTabbedDocument(CDockablePane*) — retail (RVA
// 0x8a140) allocates a CMDIChildWndEx (0x818 bytes), converts the dockable pane
// into a tabbed document and reparents it into the MDI client area. The
// conversion pipeline is not modeled. Conservative NULL.
// Symbol: ?ControlBarToTabbedDocument@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEAVCDockablePane@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__ControlBarToTabbedDocument_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEAVCDockablePane___Z(
    CMDIFrameWndEx* pThis, CDockablePane* pBar) {
    (void)pBar;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially — retail allocates a
    // CMDIChildWndEx, converts the dockable pane and reparents it into the MDI
    // client area; complex pipeline not modeled.
    return nullptr;
}

// CMDIFrameWndEx::CreateDocumentWindow(const wchar_t*, CObject*) — retail (RVA
// 0x71e0) is `xor eax,eax; ret` — returns NULL.
// Symbol: ?CreateDocumentWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEB_WPEAVCObject@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__CreateDocumentWindow_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEB_WPEAVCObject___Z(
    CMDIFrameWndEx* pThis, const wchar_t* lpcszDocName, CObject* pObj) {
    (void)pThis;
    (void)lpcszDocName;
    (void)pObj;
    return nullptr;
}

// CMDIFrameWndEx::CreateNewWindow(const wchar_t*, CObject*) — retail (RVA
// 0x89b70) allocates a CMDIChildWndEx and registers it via the MDI client area
// behind tabbed-group state (this+0x4574/0x4668). Not modeled. Conservative NULL.
// Symbol: ?CreateNewWindow@CMDIFrameWndEx@@UEAAPEAVCMDIChildWndEx@@PEB_WPEAVCObject@@@Z
extern "C" CMDIChildWndEx* MS_ABI impl__CreateNewWindow_CMDIFrameWndEx__UEAAPEAVCMDIChildWndEx__PEB_WPEAVCObject___Z(
    CMDIFrameWndEx* pThis, const wchar_t* lpcszDocName, CObject* pObj) {
    (void)lpcszDocName;
    (void)pObj;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially — retail allocates a
    // CMDIChildWndEx and registers it via the MDI client area; requires
    // unmodeled frame/tab state (this+0x4574/0x4668).
    return nullptr;
}

// CMDIFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) — retail (RVA 0x891a0)
// tail-jumps to the embedded CDockingManager at this+0x228. Not modeled.
// Conservative no-op.
// Symbol: ?DockPane@CMDIFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CMDIFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CMDIFrameWndEx* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    (void)pBar;
    (void)nDockBarID;
    (void)lpRect;
    if (!pThis) return;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (DockPane); not modeled.
}

// CMDIFrameWndEx::DockPaneLeftOf(CPane*, CPane*) — retail (RVA 0x88ab0)
// tail-jumps to the embedded CDockingManager at this+0x228. Not modeled.
// Conservative FALSE.
// Symbol: ?DockPaneLeftOf@CMDIFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CMDIFrameWndEx__QEAAHPEAVCPane__0_Z(
    CMDIFrameWndEx* pThis, CPane* pBarToDock, CPane* pBar) {
    (void)pBarToDock;
    (void)pBar;
    if (!pThis) return FALSE;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (DockPaneLeftOf); not modeled.
    return FALSE;
}

// CMDIFrameWndEx::EnableAutoHidePanes(DWORD) — retail (RVA 0x89390) tail-jumps
// to the embedded CDockingManager at this+0x228. Not modeled. Conservative FALSE.
// Symbol: ?EnableAutoHidePanes@CMDIFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CMDIFrameWndEx__QEAAHK_Z(
    CMDIFrameWndEx* pThis, unsigned long dwStyle) {
    (void)dwStyle;
    if (!pThis) return FALSE;
    // TODO(clean-room): retail tail-jumps to the embedded CDockingManager at
    // this+0x228 (EnableAutoHidePanes); not modeled.
    return FALSE;
}

// CMDIFrameWndEx::EnableMDITabbedGroups(BOOL, const CMDITabInfo&) — retail (RVA
// 0x88f40) tail-jumps to the embedded CMDIClientAreaWnd at this+0x720
// (EnableMDITabbedGroups). The client-area window is not modeled; the CMDITabInfo
// value struct is also not declared in the headers, so the reference is taken
// opaquely. Conservative no-op.
// Symbol: ?EnableMDITabbedGroups@CMDIFrameWndEx@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabbedGroups_CMDIFrameWndEx__QEAAXHAEBVCMDITabInfo___Z(
    CMDIFrameWndEx* pThis, int bEnable, const void* pParams) {
    (void)bEnable;
    (void)pParams;
    if (!pThis) return;
    // TODO(clean-room): retail tail-jumps to the embedded CMDIClientAreaWnd at
    // this+0x720 (EnableMDITabbedGroups); not modeled.
}
