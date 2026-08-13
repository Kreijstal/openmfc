// CMDIChildWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-3_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

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



















//=============================================================================
// CMDIFrameWndEx — docking / MDI-tab operations
//=============================================================================












// Symbol: ?CreateObject@CMDIChildWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWndEx__SAPEAVCObject__XZ() {
    return new CMDIChildWndEx();
}
// Symbol: ?GetThisClass@CMDIChildWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CMDIChildWndEx::classCMDIChildWndEx;
}
// Symbol: ?GetRuntimeClass@CMDIChildWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIChildWndEx__UEBAPEAUCRuntimeClass__XZ(CMDIChildWndEx* pThis) {
    return pThis ? &CMDIChildWndEx::classCMDIChildWndEx : nullptr;
}
// Symbol: ?EnableDocking@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_CMDIChildWndEx__QEAAHK_Z(CMDIChildWndEx* pThis, DWORD dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    pThis->CFrameWnd::EnableDocking(dwDockStyle);
    return TRUE;
}
// Symbol: ?RegisterTaskbarTab@CMDIChildWndEx@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pTabWnd) {
    (void)pThis;
    (void)pTabWnd;
}
// Symbol: ?UnregisterTaskbarTab@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(CMDIChildWndEx* pThis, int bCheckRegisteredTab) {
    (void)pThis;
    (void)bCheckRegisteredTab;
}
// Symbol: ?SetTaskbarTabActive@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabActive_CMDIChildWndEx__QEAAXXZ(CMDIChildWndEx* pThis) {
    (void)pThis;
}
// Symbol: ?SetTaskbarTabOrder@CMDIChildWndEx@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pWndInsertBefore) {
    (void)pThis;
    (void)pWndInsertBefore;
}
// Symbol: ?SetTaskbarTabProperties@CMDIChildWndEx@@QEAAXK@Z
extern "C" void MS_ABI impl__SetTaskbarTabProperties_CMDIChildWndEx__QEAAXK_Z(CMDIChildWndEx* pThis, DWORD dwFlags) {
    (void)pThis;
    (void)dwFlags;
}
// Symbol: ?SetTaskbarTabText@CMDIChildWndEx@@MEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTaskbarTabText_CMDIChildWndEx__MEAAXPEB_W_Z(CMDIChildWndEx* pThis, const wchar_t* lpszTabText) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_taskbarTabText[pThis] = lpszTabText ? lpszTabText : L"";
}
// Symbol: ?SetTaskbarThumbnailClipRect@CMDIChildWndEx@@MEAAHVCRect@@@Z
extern "C" int MS_ABI impl__SetTaskbarThumbnailClipRect_CMDIChildWndEx__MEAAHVCRect___Z(CMDIChildWndEx* pThis, const CRect& rectClip) {
    (void)pThis;
    (void)rectClip;
    return TRUE;
}
// Symbol: ?CanShowOnMDITabs@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(CMDIChildWndEx* pThis) {
    (void)pThis;
    return TRUE;
}
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
CMDIChildWndEx::CMDIChildWndEx() {
    memset(_mdichildwndex_padding, 0, sizeof(_mdichildwndex_padding));
}
CMDIChildWndEx::~CMDIChildWndEx() {}
// Symbol: ?ActivateFrame@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(int p0) {}

// Symbol: ?ActivateTopLevelFrame@CMDIChildWndEx@@MEAAXXZ
extern "C" void MS_ABI impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ() {}

// Symbol: ?AddDockSite@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CMDIChildWndEx__QEAAXXZ() {}

// Symbol: ?AddPane@CMDIChildWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CMDIChildWndEx__QEAAHPEAVCBasePane__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?AddTabbedPane@CMDIChildWndEx@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddTabbedPane_CMDIChildWndEx__QEAAXPEAVCDockablePane___Z(void* /*class*/* p0) {}

// Symbol: ?AdjustClientArea@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ() {}

// Symbol: ?AdjustDockingLayout@CMDIChildWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(void* p0) {}

// Symbol: ?DockPane@CMDIChildWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CMDIChildWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(void* /*class*/* p0, unsigned int p1, const void* /*struct*/* p2) {}

// Symbol: ?DockPaneLeftOf@CMDIChildWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CMDIChildWndEx__QEAAHPEAVCPane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?EnableAutoHidePanes@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CMDIChildWndEx__QEAAHK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?EnableTaskbarThumbnailClipRect@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTaskbarThumbnailClipRect_CMDIChildWndEx__QEAAXH_Z(int p0) {}

// Symbol: ?GetDocumentName@CMDIChildWndEx@@UEAAPEB_WPEAPEAVCObject@@@Z
extern "C" void* MS_ABI impl__GetDocumentName_CMDIChildWndEx__UEAAPEB_WPEAPEAVCObject___Z(void* /*class*/** p0) {
    return nullptr;
}

// Symbol: ?GetFrameIcon@CMDIChildWndEx@@UEBAPEAUHICON__@@XZ
extern "C" void* MS_ABI impl__GetFrameIcon_CMDIChildWndEx__UEBAPEAUHICON____XZ() {
    return nullptr;
}

// Symbol: ?GetFrameText@CMDIChildWndEx@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI impl__GetFrameText_CMDIChildWndEx__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* /*class*/* p0, void** p1, void* p2) {
    return nullptr;
}

// Symbol: ?GetPane@CMDIChildWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__GetPane_CMDIChildWndEx__QEAAPEAVCBasePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?GetTaskbarPreviewWnd@CMDIChildWndEx@@UEAAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetTaskbarPreviewWnd_CMDIChildWndEx__UEAAPEAVCWnd__XZ() {
    return nullptr;
}

// Symbol: ?GetTaskbarThumbnailClipRect@CMDIChildWndEx@@UEBA?AVCRect@@XZ
extern "C" void* MS_ABI impl__GetTaskbarThumbnailClipRect_CMDIChildWndEx__UEBA_AVCRect__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?InsertPane@CMDIChildWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CMDIChildWndEx__QEAAHPEAVCBasePane__0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?InvalidateIconicBitmaps@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ() {
    return 0;
}

// Symbol: ?IsPointNearDockSite@CMDIChildWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CMDIChildWndEx__QEBAHVCPoint__AEAKAEAH_Z(void* /*class*/ p0, unsigned long* p1, int* p2) {
    return 0;
}

// Symbol: ?IsRegisteredWithTaskbarTabs@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__IsRegisteredWithTaskbarTabs_CMDIChildWndEx__QEAAHXZ() {
    return 0;
}

// Symbol: ?IsTabbedMDIChild@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__IsTabbedMDIChild_CMDIChildWndEx__UEAAHXZ() {
    return 0;
}

// Symbol: ?IsTaskbarTabsSupportEnabled@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ() {
    return 0;
}

// Symbol: ?OnChangeVisualManager@CMDIChildWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CMDIChildWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnCreate@CMDIChildWndEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMDIChildWndEx__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIChildWndEx__IEAAXXZ() {}

// Symbol: ?OnEraseBkgnd@CMDIChildWndEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMDIChildWndEx__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnGetMinMaxInfo@CMDIChildWndEx@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMDIChildWndEx__IEAAXPEAUtagMINMAXINFO___Z(void* /*struct*/* p0) {}

// Symbol: ?OnIdleUpdateCmdUI@CMDIChildWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMDIChildWndEx__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnLButtonDown@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMDIActivate@CMDIChildWndEx@@IEAAXHPEAVCWnd@@0@Z
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWndEx__IEAAXHPEAVCWnd__0_Z(int p0, void* /*class*/* p1, int p2) {}

// Symbol: ?OnMouseMove@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMoveMiniFrame@CMDIChildWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CMDIChildWndEx__UEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnNcActivate@CMDIChildWndEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMDIChildWndEx__IEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnNcCalcSize@CMDIChildWndEx@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMDIChildWndEx__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcHitTest@CMDIChildWndEx@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMDIChildWndEx__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcMouseMove@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcPaint@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMDIChildWndEx__IEAAXXZ() {}

// Symbol: ?OnNcRButtonUp@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcRButtonUp_CMDIChildWndEx__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnPressTaskbarThmbnailCloseButton@CMDIChildWndEx@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressTaskbarThmbnailCloseButton_CMDIChildWndEx__UEAAXXZ() {}

// Symbol: ?OnSendIconicLivePreviewBitmap@CMDIChildWndEx@@IEAAX_K_J@Z
extern "C" void MS_ABI impl__OnSendIconicLivePreviewBitmap_CMDIChildWndEx__IEAAX_K_J_Z(unsigned __int64 p0, __int64 p1) {}

// Symbol: ?OnSendIconicThumbnail@CMDIChildWndEx@@IEAAX_K_J@Z
extern "C" void MS_ABI impl__OnSendIconicThumbnail_CMDIChildWndEx__IEAAX_K_J_Z(unsigned __int64 p0, __int64 p1) {}

// Symbol: ?OnSetIcon@CMDIChildWndEx@@IEAAPEAUHICON__@@HPEAU2@@Z
extern "C" void* MS_ABI impl__OnSetIcon_CMDIChildWndEx__IEAAPEAUHICON____HPEAU2__Z(int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?OnSetPreviewMode@CMDIChildWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_CMDIChildWndEx__UEAAXHPEAUCPrintPreviewState___Z(int p0, void* /*struct*/* p1) {}
