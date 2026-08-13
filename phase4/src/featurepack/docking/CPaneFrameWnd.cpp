// CPaneFrameWnd — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-6_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CPaneFrameWndSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?Create@CPaneFrameWnd@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CPaneFrameWnd__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
    void* pThis, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        PaneState& state = g_framePanes[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        if (state.minSize.cx <= 0) state.minSize.cx = kPaneFrameDefaultMinWidth;
        if (state.minSize.cy <= 0) state.minSize.cy = kPaneFrameDefaultMinHeight;
        state.rollUp = FALSE;
    }
    return TryCreatePaneWindow(reinterpret_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, 0, pContext);
}
// Symbol: ?AddPane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    AddUniquePane(state, pPane);
    state.rollUp = FALSE;
}
// Symbol: ?RemovePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
extern "C" void MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int, int) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    RemovePane(state, pPane);
    if (state.panes.empty()) {
        state.rollUp = FALSE;
    }
}
// Symbol: ?AdjustLayout@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    }
    if (state.minSize.cx <= 0) state.minSize.cx = kPaneFrameDefaultMinWidth;
    if (state.minSize.cy <= 0) state.minSize.cy = kPaneFrameDefaultMinHeight;
    if (state.rollUp) {
        state.recentRect.bottom = state.recentRect.top + kPaneFrameCaptionHeight + kPaneFrameBorder;
    } else {
        if (state.recentRect.Width() < state.minSize.cx) {
            state.recentRect.right = state.recentRect.left + state.minSize.cx;
        }
        if (state.recentRect.Height() < state.minSize.cy + kPaneFrameCaptionHeight) {
            state.recentRect.bottom = state.recentRect.top + state.minSize.cy + kPaneFrameCaptionHeight;
        }
    }
    state.rollUp = state.recentRect.Height() <= (kPaneFrameCaptionHeight + kPaneFrameBorder);
}
// Symbol: ?CalcBorderSize@CPaneFrameWnd@@UEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__CalcBorderSize_CPaneFrameWnd__UEBAXAEAVCRect___Z(void* pThis, CRect* pRect) {
    if (!pRect) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    const int border = state.panes.empty() ? kPaneFrameThinBorder : kPaneFrameBorder;
    pRect->SetRect(border, border, border, border);
}
// Symbol: ?CalcMinSize@CPaneFrameWnd@@MEAAXAEAVCSize@@PEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(void* pThis, CSize* pSize, MINMAXINFO*) {
    if (!pSize) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    const int border = state.panes.empty() ? kPaneFrameBorder : kPaneFrameWideBorder;
    pSize->cx = std::max(80, state.minSize.cx) + border;
    pSize->cy = std::max(40, state.minSize.cy) + kPaneFrameCaptionHeight + border;
}
// Symbol: ?GetCaptionRect@CPaneFrameWnd@@UEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetCaptionRect_CPaneFrameWnd__UEBAXAEAVCRect___Z(void* pThis, CRect* pRect) {
    if (!pRect) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    const CRect frameRect = (state.recentRect.Width() > 0 && state.recentRect.Height() > 0)
                                ? state.recentRect
                                : CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    pRect->left = frameRect.left;
    pRect->top = frameRect.top;
    pRect->right = frameRect.right;
    pRect->bottom = frameRect.top + std::min(kPaneFrameCaptionHeight, frameRect.Height());
}
// Symbol: ?GetCaptionText@CPaneFrameWnd@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetCaptionText_CPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pRet->Format(L"Pane frame (%d)", static_cast<int>(g_framePanes[pThis].panes.size()));
}
// Symbol: ?GetFirstVisiblePane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_framePanes[pThis]);
}
// Symbol: ?GetPane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_framePanes[pThis]);
}
// Symbol: ?IsRollDown@CPaneFrameWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsRollDown_CPaneFrameWnd__UEBAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    return state.rollUp ? FALSE : TRUE;
}
// Symbol: ?IsRollUp@CPaneFrameWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsRollUp_CPaneFrameWnd__UEBAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_framePanes[pThis].rollUp;
}
// Symbol: ?StartTearOff@CPaneFrameWnd@@QEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__StartTearOff_CPaneFrameWnd__QEAAHPEAVCMFCPopupMenu___Z(void* pThis, void* pMenu) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_framePanes[pThis], pMenu);
    return pThis != nullptr;
}
// Symbol: ?StopCaptionButtonsTracking@CPaneFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(void*) {
}
// Symbol: ?StoreRecentDockSiteInfo@CPaneFrameWnd@@UEAAXPEAVCPane@@@Z
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneFrameWnd__UEAAXPEAVCPane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    AddUniquePane(state, pPane);
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    }
}
// Symbol: ?StoreRecentTabRelatedInfo@CPaneFrameWnd@@UEAAXPEAVCDockablePane@@0@Z
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CPaneFrameWnd__UEAAXPEAVCDockablePane__0_Z(
    void* pThis, void* pPane, void* pTabSibling) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    AddUniquePane(state, pPane);
    AddUniquePane(state, pTabSibling);
}
// Symbol: ?UpdateTooltips@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    CWnd* wnd = reinterpret_cast<CWnd*>(pThis);
    if (wnd->GetSafeHwnd()) {
        ::InvalidateRect(wnd->GetSafeHwnd(), nullptr, FALSE);
    }
}
BOOL CPaneFrameWnd::Create(const wchar_t* lpszClassName, DWORD dwStyle, const RECT& rect,
                           CWnd* pParentWnd, CCreateContext* pContext) {
    return impl__Create_CPaneFrameWnd__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
        this, lpszClassName, dwStyle, rect, pParentWnd, pContext);
}
void CPaneFrameWnd::AdjustLayout() {
    impl__AdjustLayout_CPaneFrameWnd__UEAAXXZ(this);
}
void CPaneFrameWnd::CalcBorderSize(CRect& rect) const {
    impl__CalcBorderSize_CPaneFrameWnd__UEBAXAEAVCRect___Z(const_cast<CPaneFrameWnd*>(this), &rect);
}
void CPaneFrameWnd::CalcMinSize(CSize& size, MINMAXINFO* pMMI) {
    impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(this, &size, pMMI);
}
void CPaneFrameWnd::GetCaptionRect(CRect& rect) const {
    impl__GetCaptionRect_CPaneFrameWnd__UEBAXAEAVCRect___Z(const_cast<CPaneFrameWnd*>(this), &rect);
}
CString CPaneFrameWnd::GetCaptionText() {
    CString text;
    impl__GetCaptionText_CPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(&text, this);
    return text;
}
CWnd* CPaneFrameWnd::GetFirstVisiblePane() const {
    return reinterpret_cast<CWnd*>(
        impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(const_cast<CPaneFrameWnd*>(this)));
}
CWnd* CPaneFrameWnd::GetPane() const {
    return reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(const_cast<CPaneFrameWnd*>(this)));
}
BOOL CPaneFrameWnd::IsRollDown() const {
    return impl__IsRollDown_CPaneFrameWnd__UEBAHXZ(const_cast<CPaneFrameWnd*>(this));
}
BOOL CPaneFrameWnd::IsRollUp() const {
    return impl__IsRollUp_CPaneFrameWnd__UEBAHXZ(const_cast<CPaneFrameWnd*>(this));
}
// Symbol: ?HitTest@CPaneFrameWnd@@UEAA_JVCPoint@@H@Z
// Retail walks caption-button and pane hit paths (border/caption/button ids,
// CDockablePane resize handles); those depend on the caption-button list at
// retail 0x188 and internal virtuals. Only the window/caption-region part is
// transcribed here - anything we cannot model falls back to HTCLIENT-ish 1.
// TODO(clean-room): transcribed partially.
extern "C" __int64 MS_ABI impl__HitTest_CPaneFrameWnd__UEAA_JVCPoint__H_Z(
    void* pThis, CPoint point, int nIndex) {
    if (pThis == nullptr) return 1;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return 1;

    RECT wr;
    ::GetWindowRect(hwnd, &wr);
    CRect winRect(wr);
    if (!winRect.PtInRect(point)) return 1;   // outside window -> HTNOWHERE-ish

    RECT cr;
    ::GetClientRect(hwnd, &cr);
    CRect clientRect(cr);
    ::ClientToScreen(hwnd, reinterpret_cast<LPPOINT>(&clientRect.left));   // (left, top)
    ::ClientToScreen(hwnd, reinterpret_cast<LPPOINT>(&clientRect.right));  // (right, bottom)
    if (clientRect.PtInRect(point)) return 1;    // inside client area

    // Non-client strip: retail computes a caption rect from the border-size
    // virtual (vtable 0x388) plus caption height (retail 0x114). Use the same
    // shape with conservative constants.
    const int borderX = 2;
    const int borderY = 2;
    const int captionHeight = 22;
    CRect captionRect(winRect.left + borderX, winRect.top + borderY,
                      winRect.right - borderX, winRect.top + borderY + captionHeight);
    if (captionRect.PtInRect(point) && nIndex != 0) return 2;  // HTCAPTION-ish
    return 1;
}
// Symbol: ?IsCustModeAndNotFloatingToolbar@CPaneFrameWnd@@IEBAHXZ
// Retail: (CMFCToolBar::IsCustomizeMode() && pane != NULL &&
//          !pane->IsKindOf(RUNTIME_CLASS(CMFCToolBar))). The pane lookup uses
// retail 0x148 which is not modeled -> side table (empty for now), so this
// conservatively reports FALSE unless we have a tracked pane.
extern "C" int MS_ABI impl__IsCustModeAndNotFloatingToolbar_CPaneFrameWnd__IEBAHXZ(
    const void* pThis) {
    if (pThis == nullptr) return FALSE;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) return FALSE;
    CWnd* pPane = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const PaneFrameState* st = FindStatePtr(pThis);
        if (st && st->paneHwnd) {
            pPane = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(st->paneHwnd);
        }
    }
    if (pPane == nullptr) return FALSE;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               pPane, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())
               ? FALSE : TRUE;
}
// Symbol: ?KillDockingTimer@CPaneFrameWnd@@QEAAXXZ
extern "C" void MS_ABI impl__KillDockingTimer_CPaneFrameWnd__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    int id = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        id = st.dockTimerID;
    }
    if (id == 0) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd) ::KillTimer(hwnd, static_cast<UINT_PTR>(id));
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).dockTimerID = 0;
    }
}
// Symbol: ?KillRollupTimer@CPaneFrameWnd@@IEAAXXZ
// Retail also clears bit 0 of the roll-up flags (0x12c) and then invokes an
// internal virtual (vtable 0x328) to refresh roll-up visuals; that virtual is
// not reproducible, so only the timer + flag work is transcribed.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__KillRollupTimer_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    int id = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        id = st.rollupTimerID;
        if (id != 0) {
            st.rollupFlags &= ~1;
            st.rollupTimerID = 0;
        }
    }
    if (id == 0) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd) ::KillTimer(hwnd, static_cast<UINT_PTR>(id));
}
// Symbol: ?LoadState@CPaneFrameWnd@@UEAAHPEB_WI@Z
// Retail: if the hosted pane (0x148) is a CMFCToolBar it delegates to
// CDockablePane::LoadState (vtable 0x468); otherwise returns TRUE. The pane
// lookup + virtual dispatch are not modeled, so the non-toolbar default
// (TRUE) is returned.
// TODO(clean-room): transcribed partially.
extern "C" int MS_ABI impl__LoadState_CPaneFrameWnd__UEAAHPEB_WI_Z(
    void* pThis, const wchar_t* /*lpszProfileName*/, unsigned int /*uiID*/) {
    if (pThis == nullptr) return FALSE;
    return TRUE;
}
// Symbol: ?MoveDragFrame@CPaneFrameWnd@@IEAAXXZ
// Retail is a tail call: CMFCDragFrameImpl::MoveDragFrame(&m_dragFrameImpl, 1)
// where the embedded drag-frame impl lives at retail offset 0x1c8.
extern "C" void MS_ABI impl__MoveDragFrame_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__MoveDragFrame_CMFCDragFrameImpl__QEAAXH_Z(
        reinterpret_cast<char*>(pThis) + 0x1c8, 1);
}
// Symbol: ?MoveMiniFrame@CPaneFrameWnd@@IEAAHXZ
// Retail dispatches to a pane-type-specific MoveWindow-style virtual on the
// hosted frame (0x130), returning FALSE when no class matches. The virtual
// dispatch is not modeled -> return the "no match" default.
// TODO(clean-room): transcribed partially.
extern "C" int MS_ABI impl__MoveMiniFrame_CPaneFrameWnd__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    return FALSE;
}
// Symbol: ?OnBeforeDock@CPaneFrameWnd@@UEAAHXZ
// Retail returns FALSE while Ctrl is held (docking pre-check is skipped), then
// performs a docking-manager hit test; the manager internals are not modeled,
// so TRUE (proceed) is the conservative result otherwise.
// TODO(clean-room): transcribed partially.
extern "C" int MS_ABI impl__OnBeforeDock_CPaneFrameWnd__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    if (::GetKeyState(VK_CONTROL) < 0) return FALSE;
    return TRUE;
}
// Symbol: ?OnCancelMode@CPaneFrameWnd@@IEAAXXZ
// Retail ends an in-progress drag (drag-frame virtuals + capture). Only the
// capture release is reproducible here.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        if (st.dragging) {
            st.dragging = false;
            ::ReleaseCapture();
        }
    }
}
// Symbol: ?OnCapture@CPaneFrameWnd@@MEAAXH@Z
// Retail forwards capture-start/end to the hosted CDockablePane (vtable 0x628)
// and destroys the helper windows at 0x138 / pane->0x228 on release. Only the
// 0x138 cleanup is reproducible via the side table.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnCapture_CPaneFrameWnd__MEAAXH_Z(
    void* pThis, int nCap) {
    if (pThis == nullptr) return;
    if (nCap != 0) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        if (st.exitSizeMoveHwnd && ::IsWindow(st.exitSizeMoveHwnd)) {
            ::DestroyWindow(st.exitSizeMoveHwnd);
        }
        st.exitSizeMoveHwnd = nullptr;
    }
}
// Symbol: ?OnChar@CPaneFrameWnd@@IEAAXIII@Z
// Retail ESC path (while dragging) either calls OnCancelMode or releases the
// capture and docks back to the recent position; both paths end in Default().
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnChar_CPaneFrameWnd__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int /*nRepCnt*/, unsigned int /*nFlags*/) {
    if (pThis == nullptr) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        if (st.dragging && nChar == 0x1b) {
            st.dragging = false;
            ::ReleaseCapture();
        }
    }
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
// Symbol: ?OnCheckEmptyState@CPaneFrameWnd@@IEAA_J_K_J@Z
// Retail hides an empty floating frame when its panes are gone (internal
// vtable 0x350 + window-visible bookkeeping). Not reproducible.
// TODO(clean-room): transcribed partially.
extern "C" __int64 MS_ABI impl__OnCheckEmptyState_CPaneFrameWnd__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 /*wParam*/, __int64 /*lParam*/) {
    if (pThis == nullptr) return 0;
    return 0;
}
// Symbol: ?OnCheckRollState@CPaneFrameWnd@@MEAAXXZ
// Retail resizes the frame to the caption strip when the pane requests a
// roll-up; it needs the docking manager (0x248/GetDockingManager chain) and
// IsRollDown/IsRollUp virtuals, none of which are modeled. Safe no-op.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnCheckRollState_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
}
// Symbol: ?OnClose@CPaneFrameWnd@@IEAAXXZ
// Retail tail-dispatches to an internal virtual (vtable 0x458); the virtual
// layout is not modeled. Safe no-op.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnClose_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
}
// Symbol: ?OnCloseMiniFrame@CPaneFrameWnd@@MEAAHXZ
// Retail dispatches CloseMiniFrame to the top-level frame (global + virtuals);
// not reproducible. TRUE is the retail default when no frame is found.
// TODO(clean-room): transcribed partially.
extern "C" int MS_ABI impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    return TRUE;
}
// Symbol: ?OnContextMenu@CPaneFrameWnd@@IEAAXPEAVCWnd@@VCPoint@@@Z
// Retail shows a docking context menu via the docking manager's delayed menu
// (needs global captured-pane + manager internals). Safe no-op.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnContextMenu_CPaneFrameWnd__IEAAXPEAVCWnd__VCPoint___Z(
    void* pThis, CWnd* /*pWnd*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
}
// Symbol: ?OnCreate@CPaneFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Retail: Default(); if that returns -1 propagate it; otherwise register the
// frame and create the caption tooltip (CTooltipManager::CreateToolTip,
// internal) and return 0.
// TODO(clean-room): transcribed partially (tooltip creation skipped).
extern "C" int MS_ABI impl__OnCreate_CPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* /*lpCreateStruct*/) {
    if (pThis == nullptr) return -1;
    __int64 result = impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
    if (static_cast<int>(result) == -1) return -1;
    return 0;
}
// Symbol: ?OnDestroy@CPaneFrameWnd@@IEAAXXZ
// Retail: if dragging, release capture + internal virtual; kill the docking
// and roll-up timers; remove from docking manager and the global miniframe
// list; destroy the tooltip; then tail-call CWnd::OnDestroy.
// TODO(clean-room): transcribed partially (manager/global-list/tooltip steps
// need internals we do not model).
extern "C" void MS_ABI impl__OnDestroy_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    HWND hwnd = HwndOf(pThis);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState* st = FindStatePtr(pThis);
        if (st && st->dragging) {
            st->dragging = false;
            ::ReleaseCapture();
        }
    }
    impl__KillDockingTimer_CPaneFrameWnd__QEAAXXZ(pThis);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState* st = FindStatePtr(pThis);
        if (st && st->rollupTimerID != 0) {
            if (hwnd) ::KillTimer(hwnd, static_cast<UINT_PTR>(st->rollupTimerID));
            st->rollupTimerID = 0;
        }
        g_other6State.erase(pThis);   // frame is going away; drop side state
    }
    impl__OnDestroy_CWnd__IEAAXXZ(AsCwnd(pThis));
}
// Symbol: ?OnDockToRecentPos@CPaneFrameWnd@@UEAAXXZ
// Retail docks the hosted CDockablePane to its recent position (vtable 0x400)
// and ForceAdjustLayout()s the manager; requires CDockablePane virtuals +
// manager state that are not modeled. Safe no-op.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnDockToRecentPos_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
}
// Symbol: ?OnDrawBorder@CPaneFrameWnd@@MEAAXPEAVCDC@@@Z
// Retail paints the frame border via the visual manager using docking-manager
// state; not modeled. Safe no-op.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnDrawBorder_CPaneFrameWnd__MEAAXPEAVCDC___Z(
    void* pThis, void* /*pDC*/) {
    if (pThis == nullptr) return;
}
// Symbol: ?OnDrawCaptionButtons@CPaneFrameWnd@@MEAAXPEAVCDC@@@Z
// Retail walks the caption-button list (retail 0x188) and draws each button
// through its vtable; CMFCCaptionButton is not modeled. Safe no-op.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnDrawCaptionButtons_CPaneFrameWnd__MEAAXPEAVCDC___Z(
    void* pThis, void* /*pDC*/) {
    if (pThis == nullptr) return;
}
// Symbol: ?OnEraseBkgnd@CPaneFrameWnd@@IEAAHPEAVCDC@@@Z
// Retail body is literally `mov $1,%eax; ret` - always TRUE.
extern "C" int MS_ABI impl__OnEraseBkgnd_CPaneFrameWnd__IEAAHPEAVCDC___Z(
    void* pThis, void* /*pDC*/) {
    if (pThis == nullptr) return TRUE;
    return TRUE;
}
// Symbol: ?OnExitSizeMove@CPaneFrameWnd@@IEAAXXZ
// Retail: if a saved focus hwnd (0x140) exists and is a window, SetFocus() it
// and clear the slot.
extern "C" void MS_ABI impl__OnExitSizeMove_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    HWND focusHwnd = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        focusHwnd = st.exitSizeMoveHwnd;
    }
    if (focusHwnd == nullptr) return;
    if (::IsWindow(focusHwnd)) {
        ::SetFocus(focusHwnd);
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).exitSizeMoveHwnd = nullptr;
    }
}
// Symbol: ?OnFloatStatus@CPaneFrameWnd@@IEAA_J_K_J@Z
// Transcribed from retail:
//   wParam bits 0-1: SetWindowPos with SWP_SHOWWINDOW/SWP_HIDEWINDOW
//   wParam bits 4-5: EnableWindow((wParam>>4)&1)
//   wParam bits 2-3: if style has 0x100: ModifyStyle(0x100,0),
//                    SendMessage(WM_NCACTIVATE, (wParam>>2)&1), ModifyStyle(0,0x100)
//   return (wParam>>6)&1 when style has 0x100, else 0.
extern "C" __int64 MS_ABI impl__OnFloatStatus_CPaneFrameWnd__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 /*lParam*/) {
    if (pThis == nullptr) return 0;
    CWnd* w = AsCwnd(pThis);

    __int64 result = 0;
    if (w->GetStyle() & 0x100) {
        result = static_cast<__int64>((wParam >> 6) & 1ULL);
    }

    if (wParam & 0x3) {
        // bit0 set -> SWP_SHOWWINDOW, cleared -> SWP_HIDEWINDOW
        unsigned int nFlags = (wParam & 1) ? 0x57u : 0x97u;
        w->SetWindowPos(nullptr, 0, 0, 0, 0, nFlags);
    }

    if (wParam & 0x30) {
        w->EnableWindow(static_cast<int>((wParam >> 4) & 1ULL));
    }

    if ((wParam & 0xc) && (w->GetStyle() & 0x100)) {
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(w, 0x100, 0, 0);
        ::SendMessageW(w->GetSafeHwnd(), WM_NCACTIVATE,
                       (wParam >> 2) & 1ULL, 0);
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(w, 0, 0x100, 0);
    }

    return result;
}
// Symbol: ?OnGetMinMaxInfo@CPaneFrameWnd@@IEAAXPEAUtagMINMAXINFO@@@Z
// Retail folds the hosted pane's min size into lpMMI via CDockablePane virtuals
// (0x4f0/0x488) then tail-calls CWnd::OnGetMinMaxInfo -> Default(). Only the
// Default() tail is reproducible here.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CPaneFrameWnd__IEAAXPEAUtagMINMAXINFO___Z(
    void* pThis, MINMAXINFO* /*lpMMI*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
// Symbol: ?OnIdleUpdateCmdUI@CPaneFrameWnd@@IEAA_J_K_J@Z
// Transcribed from retail: recompute whether the frame (or a child) owns the
// focus; on change notify WM_NCACTIVATE; always fan out WM_IDLEUPDATECMDUI.
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CPaneFrameWnd__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 /*wParam*/, __int64 /*lParam*/) {
    if (pThis == nullptr) return 0;
    HWND hwnd = HwndOf(pThis);

    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    int active = 0;
    if (pFocus != nullptr) {
        HWND focusHwnd = pFocus->GetSafeHwnd();
        if (focusHwnd != nullptr &&
            (::IsChild(hwnd, focusHwnd) || hwnd == focusHwnd)) {
            active = 1;
        }
    }

    int prev = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState* st = FindStatePtr(pThis);
        if (st != nullptr) {
            prev = st->idleActive;
            st->idleActive = active;
        } else {
            g_other6State[pThis].idleActive = active;
        }
    }

    if (active != prev && hwnd) {
        ::SendMessageW(hwnd, WM_NCACTIVATE, 0, 0);
    }
    if (hwnd) {
        CWnd::SendMessageToDescendants(hwnd, WM_IDLEUPDATECMDUI, 1, 0, 1, 1);
    }
    return 0;
}
// Symbol: ?OnKillRollUpTimer@CPaneFrameWnd@@UEAAXXZ
// Retail only kills the roll-up timer when the hosted pane is not a
// CMFCToolBar with the "keep rolled-up" state bit set (vtable 0x398). Because
// KillRollupTimer is a no-op unless the timer is actually running, calling it
// unconditionally is safe and conservative.
extern "C" void MS_ABI impl__OnKillRollUpTimer_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__KillRollupTimer_CPaneFrameWnd__IEAAXXZ(pThis);
}
// Symbol: ?OnLButtonDblClk@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Retail hit-tests the caption buttons (FindButton + vtable 0x340) and always
// ends in Default(). Caption buttons are not modeled -> Default() only.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnLButtonDblClk_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
// Symbol: ?OnLButtonDown@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Retail closes any captured popup, routes the click to a caption button
// (list at 0x188) or starts a drag, and ends in Default(). Only the Default()
// tail is reproducible.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnLButtonDown_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
// Symbol: ?OnLButtonUp@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Retail completes a caption-button click (toggle roll-up, close) or ends a
// drag; both paths end in Default(). Caption-button ids + CDockablePane
// virtuals are not modeled -> Default() only.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnLButtonUp_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
CPaneFrameWnd::CPaneFrameWnd() {
    memset(_paneframewnd_padding, 0, sizeof(_paneframewnd_padding));
}
CPaneFrameWnd::~CPaneFrameWnd() {}
BOOL CPaneFrameWnd::Create(CWnd* pParentWnd, DWORD dwStyle, UINT) {
    CRect rect(0, 0, 320, 220);
    if (pParentWnd && pParentWnd->GetSafeHwnd()) {
        RECT parentRect{};
        pParentWnd->GetClientRect(&parentRect);
        rect = CRect(parentRect);
        if (rect.Width() <= 0 || rect.Height() <= 0) {
            rect = CRect(0, 0, 320, 220);
        }
    }
    RECT createRect{rect.left, rect.top, rect.right, rect.bottom};
    return Create(L"STATIC", dwStyle, createRect, pParentWnd, nullptr);
}
void CPaneFrameWnd::AddPane(CBasePane* pBar) {
    impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(this, pBar);
}
void CPaneFrameWnd::RemovePane(CBasePane* pBar, BOOL bDestroy) {
    impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(this, pBar, bDestroy, FALSE);
}
// Symbol: ?m_mapFloatingBars@CPaneFrameWnd@@1V?$CMap@IIPEAUHWND__@@PEAU1@@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_mapFloatingBars_CPaneFrameWnd__1V__CMap_IIPEAUHWND____PEAU1___A = {};

// Symbol: ?AddButton@CPaneFrameWnd@@MEAAXI@Z
extern "C" void MS_ABI impl__AddButton_CPaneFrameWnd__MEAAXI_Z(unsigned int p0) {}

// Symbol: ?AddRemovePaneFromGlobalList@CPaneFrameWnd@@SAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?ArrangeCaptionButtons@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ() {}

// Symbol: ?CanBeDockedToPane@CPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z
extern "C" int MS_ABI impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?CloseMiniFrame@CPaneFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ() {}

// Symbol: ?ConvertToTabbedDocument@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?CreateEx@CPaneFrameWnd@@UEAAHKPEB_WKAEBUtagRECT@@PEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CPaneFrameWnd__UEAAHKPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(unsigned long p0, const wchar_t* p1, unsigned long p2, const void* /*struct*/* p3, void* /*class*/* p4, void* /*struct*/* p5) {
    return 0;
}

// Symbol: ?DockPane@CPaneFrameWnd@@UEAAPEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPane_CPaneFrameWnd__UEAAPEAVCDockablePane__AEAH_Z(int* p0) {
    return nullptr;
}

// Symbol: ?DockPaneStandard@CPaneFrameWnd@@MEAAPEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPaneStandard_CPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(int* p0) {
    return nullptr;
}

// Symbol: ?EnterDragMode@CPaneFrameWnd@@MEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__EnterDragMode_CPaneFrameWnd__MEAAXPEAUHWND_____Z(void* /*struct*/* p0) {}

// Symbol: ?FindButton@CPaneFrameWnd@@IEBAPEAVCMFCCaptionButton@@I@Z
extern "C" void* MS_ABI impl__FindButton_CPaneFrameWnd__IEBAPEAVCMFCCaptionButton__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FindButton@CPaneFrameWnd@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__FindButton_CPaneFrameWnd__IEBAPEAVCMFCCaptionButton__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?FindFloatingPaneByID@CPaneFrameWnd@@SAPEAVCBasePane@@I@Z
extern "C" void* MS_ABI impl__FindFloatingPaneByID_CPaneFrameWnd__SAPEAVCBasePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FrameFromPoint@CPaneFrameWnd@@SAPEAV1@VCPoint@@HPEAV1@H@Z
extern "C" void* MS_ABI impl__FrameFromPoint_CPaneFrameWnd__SAPEAV1_VCPoint__HPEAV1_H_Z(int p0, void* /*class*/* p1, int p2) {
    return nullptr;
}

// Symbol: ?GetCaptionButtonTotalWidth@CPaneFrameWnd@@IEBAHXZ
extern "C" int MS_ABI impl__GetCaptionButtonTotalWidth_CPaneFrameWnd__IEBAHXZ() {
    return 0;
}

// Symbol: ?GetDockingMode@CPaneFrameWnd@@UEBA?AW4AFX_DOCK_TYPE@@XZ
extern "C" void* MS_ABI impl__GetDockingMode_CPaneFrameWnd__UEBA_AW4AFX_DOCK_TYPE__XZ(int /*enum*/* p0, short* p1, int p2, char p3, void* p4, void* p5, unsigned long p6, int p7, void* p8, void* p9, unsigned char* p10) {
    return nullptr;
}

// Symbol: ?GetPaneList@CPaneFrameWnd@@KAXAEAVCObList@@PEAUCRuntimeClass@@H@Z
extern "C" void MS_ABI impl__GetPaneList_CPaneFrameWnd__KAXAEAVCObList__PEAUCRuntimeClass__H_Z(void* /*class*/* p0, void* /*struct*/* p1, int p2) {}

// Symbol: ?OnMouseActivate@CPaneFrameWnd@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CPaneFrameWnd__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnMouseMove@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CPaneFrameWnd__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMovePane@CPaneFrameWnd@@UEAAXPEAVCPane@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnMovePane_CPaneFrameWnd__UEAAXPEAVCPane__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnMoving@CPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnMoving_CPaneFrameWnd__IEAAXIPEAUtagRECT___Z(unsigned int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcActivate@CPaneFrameWnd@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CPaneFrameWnd__IEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnNcCalcSize@CPaneFrameWnd@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CPaneFrameWnd__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {}

// Symbol: ?OnNcCreate@CPaneFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnNcDestroy@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CPaneFrameWnd__IEAAXXZ() {}

// Symbol: ?OnNcHitTest@CPaneFrameWnd@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CPaneFrameWnd__IEAA_JVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?OnNcLButtonDblClk@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDblClk_CPaneFrameWnd__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcMouseMove@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CPaneFrameWnd__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcPaint@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ() {}

// Symbol: ?OnNeedTipText@CPaneFrameWnd@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CPaneFrameWnd__IEAAHIPEAUtagNMHDR__PEA_J_Z(unsigned int p0, void* /*struct*/* p1, __int64* p2) {
    return 0;
}

// Symbol: ?OnPaneRecalcLayout@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnPaneRecalcLayout_CPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?OnPressButtons@CPaneFrameWnd@@MEAAXI@Z
extern "C" void MS_ABI impl__OnPressButtons_CPaneFrameWnd__MEAAXI_Z(unsigned int p0) {}

// Symbol: ?OnSetCursor@CPaneFrameWnd@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CPaneFrameWnd__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnSetFocus@CPaneFrameWnd@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CPaneFrameWnd__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSetRollUpTimer@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?OnSettingChange@CPaneFrameWnd@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CPaneFrameWnd__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnSize@CPaneFrameWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPaneFrameWnd__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnSizing@CPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CPaneFrameWnd__IEAAXIPEAUtagRECT___Z(unsigned int p0, void* /*struct*/* p1) {}

// Symbol: ?OnTimer@CPaneFrameWnd@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CPaneFrameWnd__IEAAX_K_Z(unsigned __int64 p0) {}

// Symbol: ?OnTrackCaptionButtons@CPaneFrameWnd@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnUpdateToolTips@CPaneFrameWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CPaneFrameWnd__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnWindowPosChanging@CPaneFrameWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CPaneFrameWnd__IEAAXPEAUtagWINDOWPOS___Z(void* /*struct*/* p0) {}

// Symbol: ?PaneFromPoint@CPaneFrameWnd@@UEAAPEAVCBasePane@@VCPoint@@HH@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CPaneFrameWnd__UEAAPEAVCBasePane__VCPoint__HH_Z(void* /*class*/ p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?PreCreateWindow@CPaneFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CPaneFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?PreTranslateMessage@CPaneFrameWnd@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CPaneFrameWnd__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RecalcCaptionHeight@CPaneFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__RecalcCaptionHeight_CPaneFrameWnd__MEAAXXZ() {}

// Symbol: ?RedrawAll@CPaneFrameWnd@@SAXXZ
extern "C" void MS_ABI impl__RedrawAll_CPaneFrameWnd__SAXXZ() {}

// Symbol: ?RedrawCaptionButton@CPaneFrameWnd@@IEAAXPEAVCMFCCaptionButton@@@Z
extern "C" void MS_ABI impl__RedrawCaptionButton_CPaneFrameWnd__IEAAXPEAVCMFCCaptionButton___Z(void* /*class*/* p0) {}

// Symbol: ?RemoveAllCaptionButtons@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveAllCaptionButtons_CPaneFrameWnd__IEAAXXZ() {}

// Symbol: ?RemoveButton@CPaneFrameWnd@@IEAAXI@Z
extern "C" void MS_ABI impl__RemoveButton_CPaneFrameWnd__IEAAXI_Z(unsigned int p0) {}

// Symbol: ?RemoveNonValidPanes@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?ReplaceButton@CPaneFrameWnd@@IEAAXII@Z
extern "C" void MS_ABI impl__ReplaceButton_CPaneFrameWnd__IEAAXII_Z(unsigned int p0, unsigned int p1) {}

// Symbol: ?ReplacePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@0@Z
extern "C" void MS_ABI impl__ReplacePane_CPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?SaveRecentFloatingState@CPaneFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__SaveRecentFloatingState_CPaneFrameWnd__MEAAXXZ() {}

// Symbol: ?SaveState@CPaneFrameWnd@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CPaneFrameWnd__UEAAHPEB_WI_Z(const wchar_t* p0, unsigned int p1) {
    return 0;
}

// Symbol: ?Serialize@CPaneFrameWnd@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CPaneFrameWnd__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetCaptionButtons@CPaneFrameWnd@@UEAAXK@Z
extern "C" void MS_ABI impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(unsigned long p0) {}

// Symbol: ?SetCaptionButtonsToolTips@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__SetCaptionButtonsToolTips_CPaneFrameWnd__IEAAXXZ() {}

// Symbol: ?SetDockingTimer@CPaneFrameWnd@@QEAAXI@Z
extern "C" void MS_ABI impl__SetDockingTimer_CPaneFrameWnd__QEAAXI_Z(unsigned int p0) {}

// Symbol: ?SetDockState@CPaneFrameWnd@@UEAAXPEAVCDockingManager@@@Z
extern "C" void MS_ABI impl__SetDockState_CPaneFrameWnd__UEAAXPEAVCDockingManager___Z(void* /*class*/* p0) {}

// Symbol: ?SetPreDockState@CPaneFrameWnd@@UEAAHW4AFX_PREDOCK_STATE@@PEAVCBasePane@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__SetPreDockState_CPaneFrameWnd__UEAAHW4AFX_PREDOCK_STATE__PEAVCBasePane__W4AFX_DOCK_METHOD___Z(int /*enum*/ p0, short* p1, int p2, void** p3, unsigned char p4, char p5, void* p6, void* p7, unsigned long p8, int p9, void* p10, void* p11, void** p12, unsigned char p13, void* /*class*/* p14, int /*enum*/ p15, short* p16, int p17, char p18, void* p19, void* p20, unsigned long p21, int p22, float p23, unsigned char p24, void* p25, int p26, void* p27, char p28) {
    return 0;
}

// Symbol: ?SetRollUpTimer@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ() {}

// Symbol: ?ShowButton@CPaneFrameWnd@@IEAAXIH@Z
extern "C" void MS_ABI impl__ShowButton_CPaneFrameWnd__IEAAXIH_Z(unsigned int p0, int p1) {}

// Symbol: ?SizeToContent@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__SizeToContent_CPaneFrameWnd__UEAAXXZ() {}
