// CPaneFrameWnd — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-6_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CPaneFrameWndSupport.h"
#include "detail/MfccoreSupport.h"

// ---------------------------------------------------------------------------
// Retail decode notes for the handlers added in this file.
//
// Every RVA cited below is a FUNCTION ENTRY in retail mfc140.dll (image base
// 0x180000000).  Where an individual instruction is meant that is said so.
// The disassembled DLL is the ANSI twin (mfc140.dll), so a USER32 import that
// resolves to e.g. SendMessageA corresponds to SendMessageW in mfc140u; the
// code below writes ::SendMessage and lets UNICODE pick.  An indirect
// `call *0x1802c5bd0` in the retail listings is the CFG guard dispatch
// (__guard_dispatch_icall_fptr), i.e. an ordinary indirect call through RAX --
// it is not a USER32 import.
//
// OpenMFC does NOT reproduce the retail CPaneFrameWnd object layout (the
// public class in include/openmfc/afxmfc.h is a thin CMiniFrameWnd subclass
// with a padding array), so these retail members have no counterpart here:
//   0x0e8 mouse-move re-entry byte      0x0ec last "active" flag
//   0x0f8 in-move flag                  0x10d dragging byte (side table)
//   0x114 caption height                0x118/0x11c tracked/hot button id
//   0x130 owner HWND                    0x138 drag-helper HWND
//   0x140 focus HWND (side table)       0x148 hosted pane HWND
//   0x178/0x1d0 drag anchor points      0x180/0x188/0x190 caption-button lists
//   0x1b8 tooltip control               0x1c8 CMFCDragFrameImpl
//   0x248 CDockingManager*
// Bodies that need one of those carry a TODO(clean-room) note naming exactly
// which part of the retail body is not reproduced.
//
// Retail CPaneFrameWnd vtable (read out of .rdata at 0x1802f5ad8, the address
// the retail constructor ??0CPaneFrameWnd@@QEAA@XZ (0xad6d0) stores at +0):
//   0x2e0 IsRollDown            0x2e8 IsRollUp
//   0x2f0 <unexported> `return m_nCaptionHeight;` (0x92350)
//   0x358 <unexported> visible-pane count (0xad610)
//   0x360 GetPane              0x368 GetFirstVisiblePane
//   0x370 GetDockingMode       0x388 CalcBorderSize
//   0x3a0 GetCaptionRect       0x3e8 HitTest
//   0x428 OnCapture            0x450 OnCloseMiniFrame
//   0x458 CloseMiniFrame       0x478 OnTrackCaptionButtons
//   0x490 OnCheckRollState     0x498 RecalcCaptionHeight
// ---------------------------------------------------------------------------

// Sibling exports defined later in this translation unit or in other phase4
// translation units.  Declared here so the handlers below can call the thunk
// instead of the C++ method (which has no internal definition to link to).
extern "C" void  MS_ABI impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(void* pThis, CPoint point);
extern "C" void  MS_ABI impl__RecalcCaptionHeight_CPaneFrameWnd__MEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetCaptionButtonsToolTips_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__GetDockingMode_CPaneFrameWnd__UEBA_AW4AFX_DOCK_TYPE__XZ(const void* pThis);
extern "C" void  MS_ABI impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(void* pThis, unsigned long dwButtons);
extern "C" void  MS_ABI impl__RemoveAllCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(void* pThis);

extern "C" int   MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(CWnd* pThis, CREATESTRUCTW& cs);
extern "C" int   MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
// Retail static (?m_nRollTimeOut@CPaneFrameWnd@@2IA, .data 0x3aab24) -- the
// WM_TIMER interval SetRollUpTimer passes to ::SetTimer.  Defined in
// phase4/src/featurepack/docking/StaticData.cpp.
extern "C" unsigned int impl__m_nRollTimeOut_CPaneFrameWnd__2IA;

extern "C" void  MS_ABI impl__OnSettingChange_CWnd__IEAAXIPEB_W_Z(CWnd* pThis, unsigned int uFlags, const wchar_t* lpszSection);
extern "C" void  MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, CWnd* pOldWnd);
extern "C" void  MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" int   MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void* MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(void* pList, void* pObject);
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void  MS_ABI impl__AdjustRectToWorkArea_CGlobalUtils__QEAAXAEAVCRect__PEAV2__Z(
                     void* pThis, CRect* rect, CRect* pRectDelta);
// The global CGlobalUtils instance (retail: the object at .data 0x1803aacf8).
// CGlobalUtils::AdjustRectToWorkArea reads no members, so passing the exported
// storage is safe even though OpenMFC never runs a constructor over it.
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[];

namespace {
// Registry behind CPaneFrameWnd::m_mapFloatingBars.  Retail keeps a
// CMap<UINT,UINT,HWND__*,HWND__*> in static storage at .data 0x1803aad90 and
// AddRemovePaneFromGlobalList / FindFloatingPaneByID / GetPaneList are its only
// readers and writers.  The exported data symbol below is only storage (no
// constructed CMap), so the three functions share this private table instead.
// NOTE on who fills it: ReplacePane below calls AddRemovePaneFromGlobalList
// (remove-then-add, as retail does), and CDockablePane.cpp calls it too.  What
// is still missing is retail's registration from the frame's own AddPane
// (0xaf330 -- the call at 0xaf41a, with bAdd=TRUE) and RemovePane (0xaf680 --
// the call at 0xaf6a4, with bAdd=FALSE); this file's AddPane/RemovePane do not
// touch the registry, so a pane that is only ever added never appears here.
std::mutex& FloatingPaneMutex() { static std::mutex m; return m; }
std::unordered_map<int, HWND>& FloatingPaneMap() {
    static std::unordered_map<int, HWND> m;
    return m;
}
}  // namespace


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
// Retail (0xb2280): for each of the two tracked caption-button ids -- the
// pressed button at 0x118 and the hot button at 0x11c -- it looks the button up
// in the caption-button CObList at 0x188, zeroes the id member, calls
// ::ReleaseCapture(), clears the button's pressed (+0x8) / hot (+0xc) flag and
// redraws it via RedrawCaptionButton (0xb1a80).
// STUB: no caption-button list exists in this object layout, so there is
// nothing tracked to stop -- and calling ::ReleaseCapture() unconditionally
// would steal the capture from a drag that this class did not start.
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    (void)pThis;
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
// Retail (0xb1870):
//     if (m_pToolTip /*0x1b8*/ == NULL || m_pToolTip->m_hWnd == NULL) return;
//     CRect rc; ::GetWindowRect(m_hWnd, &rc); ScreenToClient(rc);  // 0x2a11f0
//     for each caption button in the list at 0x188:
//         ask the tooltip control for its tool count (TTM_GETTOOLCOUNT, 0x40d)
//         and reposition that button's tool rect.
// STUB: neither the tooltip control nor the caption-button list exists in this
// object layout, so this reproduces retail's m_pToolTip == NULL path, which
// returns having touched nothing.  (An earlier version of this body called
// ::InvalidateRect on the whole frame; retail invalidates nothing here, and
// ArrangeCaptionButtons -- which SetCaptionButtons and RemoveButton now call --
// runs this on every caption-button change.)
extern "C" void MS_ABI impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
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
// Retail (0xaf630), transcribed in full:
//     if (m_nRollUpTimerID /*0x124*/ == 0) return;
//     ::KillTimer(m_hWnd, m_nRollUpTimerID);
//     m_nRollUpTimerID = 0;
//     SetCaptionButtons(m_dwCaptionButtons /*0x12c*/ & ~1);   // vtable 0x328
// (an earlier note here called slot 0x328 "an internal virtual ... to refresh
// roll-up visuals" and left the call out.  Dumping the CPaneFrameWnd vtable at
// .rdata 0x1802f5ad8 shows 0x328 is ?SetCaptionButtons@CPaneFrameWnd@@UEAAXK@Z
// (0xb13f0), which IS implemented below, so the call is made.  Note retail does
// not store the masked value itself -- SetCaptionButtons stores it at its end.)
// Deviation: SetCaptionButtons is called directly rather than through the
// vtable, so a further-derived override is not reached.
extern "C" void MS_ABI impl__KillRollupTimer_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    int id = 0;
    unsigned long dwButtons = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        id = st.rollupTimerID;
        if (id == 0) return;
        st.rollupTimerID = 0;
        dwButtons = static_cast<unsigned long>(st.rollupFlags);
    }
    HWND hwnd = HwndOf(pThis);
    if (hwnd) ::KillTimer(hwnd, static_cast<UINT_PTR>(id));
    impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(pThis, dwButtons & ~1UL);
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
// Retail (0xaf060) is three instructions:
//     rcx += 0x1c8;  edx = 1;  jmp CMFCDragFrameImpl::MoveDragFrame (0x55450)
// i.e. CMFCDragFrameImpl::MoveDragFrame(&m_dragFrameImpl /*retail 0x1c8*/, TRUE).
// STUB: 0x1c8 is a RETAIL offset and, as the header note at the top of this file
// says, OpenMFC does not reproduce the retail CPaneFrameWnd layout -- the public
// class (include/openmfc/afxmfc.h) is a thin CMiniFrameWnd subclass whose whole
// size is 632 bytes as this build's compiler lays it out, with the trailing
// `_paneframewnd_padding` starting at 504 -- so byte 0x1c8 (456) falls inside
// the CWnd/CFrameWnd subobject and holds no drag-frame impl.
// An earlier version of this body passed `(char*)this + 0x1c8` to the
// CMFCDragFrameImpl thunk; that is only harmless today because that thunk is
// still a generated no-op -- retail's MoveDragFrame writes through the pointer
// immediately (`mov %r13,0x40(%rcx)` at 0x180055480) and would scribble over
// this object's own members the moment it is implemented.  Left as a no-op
// until a drag-frame impl is actually modelled.
extern "C" void MS_ABI impl__MoveDragFrame_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
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
// Retail (0xb26a0):
//     CPane* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     if (p is a CPane) p-><vtable +0x628>(bCapture); else p = NULL;
//     if (::IsWindow(m_hwndDragHelper /*0x138*/) && !bCapture) {
//         ::DestroyWindow(m_hwndDragHelper); m_hwndDragHelper = NULL;
//     }
//     if (p && !bCapture) {
//         if (p->0x228 != m_hWnd && ::IsWindow(p->0x228)) ::DestroyWindow(p->0x228);
//         p->0x228 = NULL;
//     }
// STUB: the two helper HWNDs it destroys are retail 0x138 (this frame's) and
// pane+0x228; NEITHER is modelled here, and the pane virtual at +0x628 is
// unexported.  An earlier version of this body destroyed the side table's
// `exitSizeMoveHwnd` -- but that field is retail 0x140, the window OnSetFocus
// (0xb3e50, the store at 0xb3eb2) saves so that OnExitSizeMove can put the
// focus BACK on it.  Destroying it meant ::DestroyWindow on whatever window
// last had the focus, which is not something this class ever does.
extern "C" void MS_ABI impl__OnCapture_CPaneFrameWnd__MEAAXH_Z(
    void* pThis, int nCap) {
    (void)pThis;
    (void)nCap;
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
// Retail (0xb3d30) is three instructions: a tail jump through this object's
// vtable slot 0x458.  An earlier note here said that slot's target was not
// modelled; the vtable at .rdata 0x1802f5ad8 gives 0x458 == 0xb1b10 ==
// ?CloseMiniFrame@CPaneFrameWnd@@MEAAXXZ, which is implemented below, so the
// whole retail body is `CloseMiniFrame();`.  Note WM_CLOSE therefore hides a
// mini frame (and its pane) rather than destroying it.
// Deviation: called directly rather than through the vtable, so a
// further-derived override is not reached.
extern "C" void MS_ABI impl__OnClose_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(pThis);
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
// Retail (0xb37d0), transcribed: recompute whether the frame (or a child) owns
// the focus, store it at 0xec, and when it CHANGED repaint the non-client area
// with ::SendMessage(m_hWnd, WM_NCPAINT, 0, 0) -- the immediate at 0xb383c is
// 0x85, not WM_NCACTIVATE, which is what an earlier version of this body sent.
// Then always fan out WM_IDLEUPDATECMDUI (0x363) to the descendants.
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
        ::SendMessageW(hwnd, WM_NCPAINT, 0, 0);
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
// Static data member.  Retail places a constructed
// CMap<UINT,UINT,HWND__*,HWND__*> here (.data 0x1803aad90): the qword at +0 is
// a vtable pointer in the retail image and CPaneFrameWnd::GetPaneList (0xb3d50)
// and FrameFromPoint (0xb2940) read the element count at +0x18.  That matches
// MFC's CMap layout -- CObject vptr, m_pHashTable, m_nHashTableSize, m_nCount,
// m_pFreeList, m_pBlocks, m_nBlockSize -- i.e. 0x38 bytes.  OpenMFC does not
// construct a CMap here, so this stays zeroed storage; it is sized to 0x38 so
// that a client writing through the export cannot run off the end (the previous
// 8-byte definition could).  The live registry is the private table above.
extern "C" alignas(8) unsigned char impl__m_mapFloatingBars_CPaneFrameWnd__1V__CMap_IIPEAUHWND____PEAU1___A[0x38] = {};

// Symbol: ?AddButton@CPaneFrameWnd@@MEAAXI@Z
// Retail (0xb1510) searches the caption-button CObList embedded at 0x180 for a
// button whose id (CMFCCaptionButton +0x20) equals uiID; if found it does
// nothing.  Otherwise it allocates a CMFCCaptionButton (0x38 bytes) -- or a
// CMFCCaptionMenuButton (0x48 bytes) when uiID == 0x19 -- AddHead()s it and
// sets +0x30 (visible) to 1.
// STUB: the caption-button list has no counterpart in this object layout and
// CMFCCaptionButton is not modelled, so there is nothing to add it to.
extern "C" void MS_ABI impl__AddButton_CPaneFrameWnd__MEAAXI_Z(void* pThis, unsigned int uiID) {
    (void)pThis;
    (void)uiID;
}

// Symbol: ?AddRemovePaneFromGlobalList@CPaneFrameWnd@@SAHPEAVCBasePane@@H@Z
// Retail (0xaf740):
//     int nID = pPane->GetDlgCtrlID();
//     if (nID != -1) {
//         if (bAdd) m_mapFloatingBars.SetAt(nID, pPane ? pPane->m_hWnd : NULL);
//         else      m_mapFloatingBars.RemoveKey(nID);
//         return TRUE;
//     }
//     if (!pPane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) return FALSE;
//     // ... else walk the tabbed pane's children (its vtable +0x758 count /
//     //     +0x7a8 accessor) and add/remove each child by its own ctrl id
//     return TRUE;
// Implemented here against the private registry.  Deviations, both documented:
//   * retail calls GetDlgCtrlID on pPane before its NULL test, so a NULL pPane
//     faults there; this returns FALSE instead.
//   * the CBaseTabbedPane branch needs the pane's unexported child-enumeration
//     virtuals, so a tabbed pane with ctrl id -1 returns FALSE here where
//     retail enumerates its children and returns TRUE.
// TODO(clean-room): transcribed partially (tabbed-pane branch).
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
    void* pPane, int bAdd) {
    if (pPane == nullptr) return FALSE;
    CWnd* pWnd = reinterpret_cast<CWnd*>(pPane);
    const int nID = impl__GetDlgCtrlID_CWnd__QEBAHXZ(pWnd);
    if (nID != -1) {
        std::lock_guard<std::mutex> lock(FloatingPaneMutex());
        if (bAdd) {
            FloatingPaneMap()[nID] = pWnd->GetSafeHwnd();
        } else {
            FloatingPaneMap().erase(nID);
        }
        return TRUE;
    }
    CRuntimeClass* pTabbed = impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
    if (pTabbed == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            reinterpret_cast<const CObject*>(pPane), pTabbed)) {
        return FALSE;
    }
    return TRUE;
}

// Symbol: ?ArrangeCaptionButtons@CPaneFrameWnd@@IEAAXXZ
// Retail (0xb1990): GetCaptionRect() (vtable 0x3a0), then walks the visible
// caption-button list at 0x190 laying the buttons out right-to-left inside the
// caption rect, and finishes with an unconditional UpdateTooltips() (the call
// at 0xb1a5a to 0xb1870).  No caption buttons exist in this object layout, so
// the layout loop has nothing to iterate and only the unconditional tail call
// is reproduced.
// TODO(clean-room): transcribed partially (button layout loop).
extern "C" void MS_ABI impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(pThis);
}

// Symbol: ?CanBeDockedToPane@CPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z
// Retail (0xb3770):
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     CPane* pPane = (p && p->IsKindOf(RUNTIME_CLASS(CPane))) ? (CPane*)p : NULL;
//     return pDockablePane->vtable[0x318](pPane);   // tail call on the ARGUMENT
// STUB: the answer comes entirely from an unexported virtual on the passed-in
// CDockablePane, which OpenMFC does not model; guessing it would silently
// allow or forbid docking.
extern "C" int MS_ABI impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(
    const void* pThis, const void* pDockablePane) {
    (void)pThis;
    (void)pDockablePane;
    return FALSE;
}

// Symbol: ?CloseMiniFrame@CPaneFrameWnd@@MEAAXXZ
// Retail (0xb1b10):
//     if (OnCloseMiniFrame() /*vtable 0x450*/) {
//         ShowWindow(SW_HIDE);
//         CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//         if (p) p->ShowWindow(SW_HIDE);
//     }
// GetPane() is retail's own `CWnd::FromHandlePermanent(m_hwndPane)` (0xaf900),
// so it stands in for the 0x148 lookup here.
extern "C" void MS_ABI impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (!impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(pThis)) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd) ::ShowWindow(hwnd, SW_HIDE);
    CWnd* pPane = reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    if (pPane && pPane->GetSafeHwnd()) ::ShowWindow(pPane->GetSafeHwnd(), SW_HIDE);
}

// Symbol: ?ConvertToTabbedDocument@CPaneFrameWnd@@UEAAXXZ
// Retail (0xae120):
//     CWnd* p = GetFirstVisiblePane();          // vtable 0x368
//     if (p && p->IsKindOf(RUNTIME_CLASS(CDockablePane))) {
//         p->vtable[0x5d0](FALSE);              // unexported CDockablePane virtual
//         ::PostMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);
//     }
// The .data slot at 0x1803bae78 is ?AFX_WM_CHECKEMPTYMINIFRAME@@3IA (zero in
// the image only because RegisterWindowMessage fills it in at run time).
// Correction to an earlier note here: OpenMFC DOES have that id -- see
// impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA in phase4/src/core/runtime/Globals.cpp
// -- so the message is not what blocks this function.
// STUB: the conversion itself is the unexported CDockablePane virtual at
// +0x5d0.  Posting the notification without having converted the pane would
// ask the owner frame to re-check an empty state that never changed.
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CreateEx@CPaneFrameWnd@@UEAAHKPEB_WKAEBUtagRECT@@PEAVCWnd@@PEAUCCreateContext@@@Z
// Retail (0xad920) is the creation primitive -- CPaneFrameWnd::Create (0xad8f0)
// is a two-line forwarder that calls it with dwStyleEx = 0:
//     dwStyle |= WS_POPUP;
//     if (pParentWnd) {
//         if (pParentWnd->GetExStyle() & WS_EX_LAYOUTRTL) dwStyleEx |= WS_EX_LAYOUTRTL;
//         m_hwndOwner /*0x130*/ = pParentWnd->m_hWnd;
//     } else m_hwndOwner = NULL;
//     CString cls = afxGlobalData.RegisterWindowClass("<private class name>");
//     if (!CWnd::CreateEx(dwStyleEx, cls, lpszClassName /*as the WINDOW name*/,
//                         dwStyle, rect, pParentWnd, 0, pContext)) return FALSE;
//     if (pParentWnd) {
//         if (!pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))) return FALSE;
//         CDockingManager* p = m_pDockManager /*0x248*/;
//         if (!p) { p = afxGlobalUtils.GetDockingManager(pParentWnd); if (!p) return FALSE; }
//         p->vtable[0x48](this);                       // add this mini frame
//     }
//     CMFCDragFrameImpl::Init(&m_dragFrameImpl /*0x1c8*/, this);
//     return TRUE;
// Here the private window-class registration and the docking-manager
// registration are unavailable, so window creation is delegated to this file's
// own Create path (which owns the side-table bookkeeping) with WS_POPUP added,
// after applying retail's CFrameWnd-parent rejection.  Deviations: dwStyleEx is
// dropped (the Create path takes no extended style), and the docking manager is
// not told about the new frame.
// TODO(clean-room): transcribed partially.
extern "C" int MS_ABI impl__CreateEx_CPaneFrameWnd__UEAAHKPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
    void* pThis, unsigned long /*dwStyleEx*/, const wchar_t* lpszClassName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    if (pParentWnd != nullptr) {
        CRuntimeClass* pFrameClass = impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
        if (pFrameClass == nullptr ||
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParentWnd, pFrameClass)) {
            return FALSE;
        }
    }
    return impl__Create_CPaneFrameWnd__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
        pThis, lpszClassName, dwStyle | WS_POPUP, rect, pParentWnd, pContext);
}

// Symbol: ?DockPane@CPaneFrameWnd@@UEAAPEAVCDockablePane@@AEAH@Z
// Retail (0xae190) takes GetPane(), requires it to be a CDockablePane, clears
// *pbWasDocked, reads GetDockingMode() and then drives the CDockingManager
// (0x248, or afxGlobalUtils.GetDockingManager(this)) through several of its
// unexported virtuals to find a dock site and re-dock the pane.
// STUB: the docking manager is not modelled; returning a fabricated pane
// pointer would corrupt the caller's docking state.
extern "C" void* MS_ABI impl__DockPane_CPaneFrameWnd__UEAAPEAVCDockablePane__AEAH_Z(
    void* pThis, int* pbWasDocked) {
    (void)pThis;
    if (pbWasDocked) *pbWasDocked = FALSE;   // retail clears it at 0xae1e8
    return nullptr;
}

// Symbol: ?DockPaneStandard@CPaneFrameWnd@@MEAAPEAVCDockablePane@@AEAH@Z
// Retail (0xae4c0):
//     CWnd* p = GetPane();                                  // vtable 0x360
//     if (!p || !p->IsKindOf(RUNTIME_CLASS(CDockablePane))) return NULL;
//     if (!this->vtable[0x348]() /*OnBeforeDock*/) return NULL;
//     if (!::IsWindowVisible(p->m_hWnd) && (p->GetDockingMode() & 2))
//         p->ShowWindow(SW_SHOW);
//     CDockablePane* r = p->vtable[0x570](pbWasDocked);     // pane's own dock step
//     return (r && r->IsKindOf(RUNTIME_CLASS(CDockablePane))) ? r : NULL;
// STUB: the docking step is an unexported CDockablePane virtual.
extern "C" void* MS_ABI impl__DockPaneStandard_CPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(
    void* pThis, int* pbWasDocked) {
    (void)pThis;
    (void)pbWasDocked;
    return nullptr;
}

// Symbol: ?EnterDragMode@CPaneFrameWnd@@MEAAXPEAUHWND__@@@Z
// Retail (0xadd70):
//     if (m_bDragging /*0x10d*/) return;
//     CWnd::FromHandle(::SetCapture(m_hWnd));               // result discarded
//     if (m_hwndDragHelper /*0x138*/ == NULL) m_hwndDragHelper = hwndCaptured;
//     m_bDragging = TRUE;
//     OnCapture(TRUE);                                      // vtable 0x428
//     ::GetCursorPos(&m_ptDrag /*0x1d0*/);
//     if ((GetDockingMode() & 1) && !(GetDockingMode() & 0x80))
//         ::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
//     ::GetCursorPos(&m_ptDragStart /*0x178*/);
//     CDockingManager* p = m_pDockManager /*0x248*/;
//     if (!p) p = afxGlobalUtils.GetDockingManager(this);
//     p-><unexported> (0x4d910);
// Reproduced: the re-entry guard, the mouse capture, the dragging flag (the
// side table's `dragging`, which OnCancelMode/OnChar already consume), the
// OnCapture(TRUE) notification and the drag cursor.
// TODO(clean-room): transcribed partially -- the 0x138 drag-helper HWND, the
// two drag anchor points and the final docking-manager call are not modelled.
extern "C" void MS_ABI impl__EnterDragMode_CPaneFrameWnd__MEAAXPEAUHWND_____Z(
    void* pThis, HWND /*hwndCaptured*/) {
    if (pThis == nullptr) return;
    HWND hwnd = HwndOf(pThis);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        if (st.dragging) return;
        st.dragging = true;
    }
    if (hwnd) ::SetCapture(hwnd);
    impl__OnCapture_CPaneFrameWnd__MEAAXH_Z(pThis, 1);
    const int nDockMode = impl__GetDockingMode_CPaneFrameWnd__UEBA_AW4AFX_DOCK_TYPE__XZ(pThis);
    if ((nDockMode & 1) != 0 && (nDockMode & 0x80) == 0) {
        ::SetCursor(::LoadCursor(nullptr, IDC_SIZEALL));
    }
}

// Symbol: ?FindButton@CPaneFrameWnd@@IEBAPEAVCMFCCaptionButton@@I@Z
// Retail (0xb1700) walks the caption-button CObList head at 0x188 (node layout
// pNext/pPrev/data at +0/+8/+0x10) and returns the first button whose id
// (CMFCCaptionButton +0x20) equals uiID, else NULL.
// STUB: no caption-button list exists in this object layout.
extern "C" void* MS_ABI impl__FindButton_CPaneFrameWnd__IEBAPEAVCMFCCaptionButton__I_Z(
    const void* pThis, unsigned int uiID) {
    (void)pThis;
    (void)uiID;
    return nullptr;
}

// Symbol: ?FindButton@CPaneFrameWnd@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
// Retail (0xb1720) gets the window rect, mirrors the hit point when the window
// has WS_EX_LAYOUTRTL, then walks the caption-button list at 0x188 asking each
// button for its rect (its vtable +0x28), offsetting that rect by the window
// origin and returning the first button whose rect contains the point.
// STUB: no caption-button list exists in this object layout.
extern "C" void* MS_ABI impl__FindButton_CPaneFrameWnd__IEBAPEAVCMFCCaptionButton__VCPoint___Z(
    const void* pThis, CPoint point) {
    (void)pThis;
    (void)point;
    return nullptr;
}

// Symbol: ?FindFloatingPaneByID@CPaneFrameWnd@@SAPEAVCBasePane@@I@Z
// Retail (0xb28e0):
//     HWND h;
//     if (!m_mapFloatingBars.Lookup(nID, h)) return NULL;
//     CWnd* p = CWnd::FromHandlePermanent(h);
//     return (p && p->IsKindOf(RUNTIME_CLASS(CBasePane))) ? (CBasePane*)p : NULL;
// Transcribed against the private registry described above.
extern "C" void* MS_ABI impl__FindFloatingPaneByID_CPaneFrameWnd__SAPEAVCBasePane__I_Z(
    unsigned int nID) {
    HWND hwnd = nullptr;
    {
        std::lock_guard<std::mutex> lock(FloatingPaneMutex());
        std::unordered_map<int, HWND>& map = FloatingPaneMap();
        std::unordered_map<int, HWND>::const_iterator it = map.find(static_cast<int>(nID));
        if (it == map.end()) return nullptr;
        hwnd = it->second;
    }
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwnd);
    if (pWnd == nullptr) return nullptr;
    CRuntimeClass* pBasePane = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    if (pBasePane == nullptr) return nullptr;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, pBasePane) ? pWnd : nullptr;
}

// Symbol: ?FrameFromPoint@CPaneFrameWnd@@SAPEAV1@VCPoint@@HPEAV1@H@Z
// Retail (0xb2940) walks m_mapFloatingBars; for each visible CBasePane it asks
// the pane for its parent mini frame (pane vtable +0x460), skips
// pFrameToExclude, hit-tests the pane's window rect (also inflated by
// -nSensitivity) and, when bFloatMultiOnly is set, filters on the pane's
// alignment/pane-count virtuals (+0x390, +0x360).
// STUB: three unexported CBasePane virtuals decide the result; without them any
// answer would be a guess about which floating frame the cursor is over.
extern "C" void* MS_ABI impl__FrameFromPoint_CPaneFrameWnd__SAPEAV1_VCPoint__HPEAV1_H_Z(
    CPoint point, int nSensitivity, void* pFrameToExclude, int bFloatMultiOnly) {
    (void)point;
    (void)nSensitivity;
    (void)pFrameToExclude;
    (void)bFloatMultiOnly;
    return nullptr;
}

// Symbol: ?GetCaptionButtonTotalWidth@CPaneFrameWnd@@IEBAHXZ
// Retail (0xb2380) sums (rect.right - rect.left) over the caption-button list
// at 0x188, asking each button for its rect through its vtable +0x28.
// STUB: no caption-button list exists in this object layout.  Zero happens to
// be what retail returns for an empty list, but nothing here can produce a
// non-zero width, so this is not an implementation of the function.
extern "C" int MS_ABI impl__GetCaptionButtonTotalWidth_CPaneFrameWnd__IEBAHXZ(const void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetDockingMode@CPaneFrameWnd@@UEBA?AW4AFX_DOCK_TYPE@@XZ
// Retail (0xb3c20):
//     CWnd* p = GetPane();                               // vtable 0x360
//     if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane)))
//         return p->GetDockingMode();                    // pane vtable 0x380, tail call
//     return <global default at .data 0x1803aaac8>;
// The enum is returned in EAX (no hidden return pointer).  OpenMFC's
// CBasePane::GetDockingMode export is itself an unimplemented stub, so the
// delegation cannot be reproduced and only retail's fallback is returned; the
// image value of that .data slot is 2.
// TODO(clean-room): transcribed partially (pane delegation missing).
extern "C" int MS_ABI impl__GetDockingMode_CPaneFrameWnd__UEBA_AW4AFX_DOCK_TYPE__XZ(
    const void* pThis) {
    (void)pThis;
    return 2;
}

// Symbol: ?GetPaneList@CPaneFrameWnd@@KAXAEAVCObList@@PEAUCRuntimeClass@@H@Z
// Static (protected).  Retail (0xb3d50) walks m_mapFloatingBars and, for each
// value HWND:
//     CWnd* p = CWnd::FromHandle(h);
//     if (bIncludeTabbed && p && p->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)))
//         p->vtable[0x7e0](lstBars, pRTCFilter);   // let the tabbed pane add its own
//     else if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane)) &&
//              (pRTCFilter == NULL || p->GetRuntimeClass() == pRTCFilter))
//         lstBars.AddTail(p);
// The tabbed-pane branch needs an unexported CBaseTabbedPane virtual, so a
// tabbed pane is passed to the plain branch here instead of expanding it.
// TODO(clean-room): transcribed partially (tabbed-pane expansion).
extern "C" void MS_ABI impl__GetPaneList_CPaneFrameWnd__KAXAEAVCObList__PEAUCRuntimeClass__H_Z(
    void* pLstBars, void* pRTCFilter, int /*bIncludeTabbed*/) {
    if (pLstBars == nullptr) return;
    CRuntimeClass* pBasePane = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    if (pBasePane == nullptr) return;
    std::unordered_map<int, HWND> snapshot;
    {
        std::lock_guard<std::mutex> lock(FloatingPaneMutex());
        snapshot = FloatingPaneMap();
    }
    for (std::unordered_map<int, HWND>::const_iterator it = snapshot.begin();
         it != snapshot.end(); ++it) {
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(it->second);
        if (pWnd == nullptr) continue;
        if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, pBasePane)) continue;
        if (pRTCFilter != nullptr &&
            impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(pWnd) != pRTCFilter) {
            continue;
        }
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(pLstBars, pWnd);
    }
}

// Symbol: ?OnMouseActivate@CPaneFrameWnd@@IEAAHPEAVCWnd@@II@Z
// Retail (0xadc80):
//     HWND h = m_hwndPane /*0x148*/;
//     if (h && ::IsWindow(h)) {
//         CWnd* pPane = CWnd::FromHandle(h);
//         CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//         LRESULT ht = HitTest(pt, TRUE);                  // vtable 0x3e8
//         if (pPane->IsKindOf(RUNTIME_CLASS(CMFCToolBar)) && ht != 1) {
//             SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
//                          SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
//             return MA_NOACTIVATE;                        // retail: mov $0x3,%eax
//         }
//     }
//     return CWnd::Default();
// GetPane() is retail's own FromHandlePermanent(m_hwndPane) (0xaf900) and
// stands in for the 0x148 lookup; retail uses FromHandle there, so a pane
// window owned by another MFC module resolves to a temporary CWnd in retail and
// to NULL here.
extern "C" int MS_ABI impl__OnMouseActivate_CPaneFrameWnd__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* /*pDesktopWnd*/, unsigned int /*nHitTest*/, unsigned int /*message*/) {
    if (pThis == nullptr) return 0;
    HWND hwnd = HwndOf(pThis);
    CWnd* pPane = reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    if (hwnd && pPane && pPane->GetSafeHwnd() && ::IsWindow(pPane->GetSafeHwnd())) {
        POINT pt = {};
        ::GetCursorPos(&pt);
        ::ScreenToClient(hwnd, &pt);
        const __int64 ht =
            impl__HitTest_CPaneFrameWnd__UEAA_JVCPoint__H_Z(pThis, CPoint(pt), TRUE);
        CRuntimeClass* pToolBarClass = impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
        if (pToolBarClass != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pToolBarClass) && ht != 1) {
            ::SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0,
                           SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
            return MA_NOACTIVATE;
        }
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis)));
}

// Symbol: ?OnMouseMove@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Retail (0xae5a0) is the drag loop: it uses the re-entry byte at 0xe8, the
// dragging byte at 0x10d, the "moving" flag at 0xf0, the drag anchor at
// 0x178/0x17c and the drag-frame implementation at 0x1c8, and drives the
// docking manager's hit testing while the frame is being dragged.
// STUB: none of that state is modelled, and a partial drag loop that moved the
// window without the docking manager's agreement would be worse than no-op.
extern "C" void MS_ABI impl__OnMouseMove_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, CPoint point) {
    (void)pThis;
    (void)nFlags;
    (void)point;
}

// Symbol: ?OnMovePane@CPaneFrameWnd@@UEAAXPEAVCPane@@VCPoint@@@Z
// Retail (0xb0ee0), transcribed in full.  The CPane* argument is never read:
//     CRect rect; ::GetWindowRect(m_hWnd, &rect);
//     ::OffsetRect(&rect, ptOffset.x, ptOffset.y);
//     MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
extern "C" void MS_ABI impl__OnMovePane_CPaneFrameWnd__UEAAXPEAVCPane__VCPoint___Z(
    void* pThis, void* /*pBar*/, CPoint ptOffset) {
    if (pThis == nullptr) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return;
    RECT rect = {};
    ::GetWindowRect(hwnd, &rect);
    ::OffsetRect(&rect, ptOffset.x, ptOffset.y);
    ::MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

// Symbol: ?OnMoving@CPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
// Retail (0xaf080), transcribed in full -- it is two calls and neither argument
// is read:  CWnd::Default(); then a direct (non-virtual) tail jump to
// CPaneFrameWnd::MoveMiniFrame (0xb1080), whose BOOL result is discarded.
extern "C" void MS_ABI impl__OnMoving_CPaneFrameWnd__IEAAXIPEAUtagRECT___Z(
    void* pThis, unsigned int /*nSide*/, RECT* /*lpRect*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
    impl__MoveMiniFrame_CPaneFrameWnd__IEAAHXZ(pThis);
}

// Symbol: ?OnNcActivate@CPaneFrameWnd@@IEAAHH@Z
// Retail (0xb1350):
//     if (GetStyle() & MFS_SYNCACTIVE /*0x100, afxwin.h*/)
//         return !((m_flags /*0xa8*/ >> 9) & 1);
//     BOOL bActive = (CWnd::FromHandle(::GetFocus()) == this);
//     if (m_bActive /*0xec*/ != bActive) {
//         m_bActive = bActive;
//         if (m_pDockManager /*0x248*/ ||
//             afxGlobalUtils.GetDockingManager(CWnd::FromHandlePermanent(m_hwndOwner /*0x130*/)))
//             ::SendMessage(m_hWnd, WM_NCPAINT /*0x85*/, 0, 0);
//     }
//     return TRUE;
// Reproduced: the focus comparison and the active-flag bookkeeping, kept in the
// side table's `idleActive`.
// TODO(clean-room): transcribed partially -- the MFS_SYNCACTIVE early return
// reads an unmodelled CWnd/CMiniFrameWnd bitfield (0xa8) so TRUE is returned on
// that path too, and the WM_NCPAINT repaint is skipped because whether retail
// sends it depends on a docking manager this repo does not model.  (The
// immediate at 0xb13bc is 0x85 = WM_NCPAINT, not WM_NCACTIVATE -- an earlier
// note here read it as WM_NCACTIVATE.  The only 0x86 this class ever sends is
// in OnFloatStatus, 0xb1dfe.)
extern "C" int MS_ABI impl__OnNcActivate_CPaneFrameWnd__IEAAHH_Z(void* pThis, int /*bActive*/) {
    if (pThis == nullptr) return TRUE;
    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    const int bActiveNow = (pFocus == reinterpret_cast<CWnd*>(pThis)) ? 1 : 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).idleActive = bActiveNow;
    }
    return TRUE;
}

// Symbol: ?OnNcCalcSize@CPaneFrameWnd@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
// Retail (0xaf950), transcribed in full:
//     CRect rectBorder(0,0,0,0);
//     CalcBorderSize(rectBorder);                 // vtable 0x388
//     lpncsp->rgrc[0].top    += m_nCaptionHeight /*0x114*/ + rectBorder.top;
//     lpncsp->rgrc[0].bottom -= rectBorder.bottom;
//     lpncsp->rgrc[0].left   += rectBorder.left;
//     lpncsp->rgrc[0].right  -= rectBorder.right;
//     CWnd::Default();
// Deviation: retail's m_nCaptionHeight (0x114, computed by RecalcCaptionHeight)
// has no counterpart here, so the file's own kPaneFrameCaptionHeight constant
// is used -- the same constant GetCaptionRect above already uses.
extern "C" void MS_ABI impl__OnNcCalcSize_CPaneFrameWnd__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp) {
    if (pThis == nullptr) return;
    if (lpncsp != nullptr) {
        CRect rectBorder(0, 0, 0, 0);
        impl__CalcBorderSize_CPaneFrameWnd__UEBAXAEAVCRect___Z(pThis, &rectBorder);
        lpncsp->rgrc[0].top += kPaneFrameCaptionHeight + rectBorder.top;
        lpncsp->rgrc[0].bottom -= rectBorder.bottom;
        lpncsp->rgrc[0].left += rectBorder.left;
        lpncsp->rgrc[0].right -= rectBorder.right;
    }
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}

// Symbol: ?OnNcCreate@CPaneFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Retail (0xb1c90 -- the ANSI twin's ?OnNcCreate@CPaneFrameWnd@@IEAAHPEAUtag
// CREATESTRUCTA@@@Z), transcribed in full:
//     if (!CWnd::Default()) return FALSE;
//     if (GetStyle() & MFS_SYNCACTIVE /*0x100, afxwin.h*/) {
//         CWnd* pTop = GetTopLevelParent();       // AfxThrowInvalidArgException if NULL
//         CWnd* pFore = CWnd::FromHandle(::GetForegroundWindow());
//         BOOL bActive = (pTop == pFore) ||
//             (CWnd::FromHandle(::GetLastActivePopup(pTop->m_hWnd)) == pFore &&
//              ::SendMessage(pFore->m_hWnd, WM_FLOATSTATUS, FS_SYNCACTIVE, 0) != 0);
//         ::SendMessage(m_hWnd, WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE, 0);
//     }
//     return TRUE;
// WM_FLOATSTATUS is 0x036D and FS_ACTIVATE/FS_DEACTIVATE/FS_SYNCACTIVE are
// 4/8/0x40 (afxpriv.h); they are spelled as literals below because OpenMFC's
// headers do not define them.
// Deviation: retail throws CInvalidArgException when GetTopLevelParent()
// returns NULL; this returns TRUE (leaving the window created) instead.
extern "C" int MS_ABI impl__OnNcCreate_CPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* /*lpcs*/) {
    if (pThis == nullptr) return FALSE;
    CWnd* pWnd = AsCwnd(pThis);
    if (impl__Default_CWnd__IEAA_JXZ(pWnd) == 0) return FALSE;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return TRUE;
    if ((::GetWindowLong(hwnd, GWL_STYLE) & 0x100 /*MFS_SYNCACTIVE*/) == 0) return TRUE;
    CWnd* pTop = reinterpret_cast<CWnd*>(
        impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(pWnd));
    if (pTop == nullptr) return TRUE;
    CWnd* pFore = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetForegroundWindow());
    BOOL bActive = (pTop == pFore);
    if (!bActive && pFore != nullptr && pTop->GetSafeHwnd() != nullptr) {
        CWnd* pPopup = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetLastActivePopup(pTop->GetSafeHwnd()));
        bActive = (pPopup == pFore) &&
                  ::SendMessage(pFore->GetSafeHwnd(), 0x36d /*WM_FLOATSTATUS*/,
                                0x40 /*FS_SYNCACTIVE*/, 0) != 0;
    }
    ::SendMessage(hwnd, 0x36d /*WM_FLOATSTATUS*/,
                  bActive ? 4 /*FS_ACTIVATE*/ : 8 /*FS_DEACTIVATE*/, 0);
    return TRUE;
}

// Symbol: ?OnNcDestroy@CPaneFrameWnd@@IEAAXXZ
// Retail (0xb1e40), transcribed in full:
//     OnCancelMode();          // direct call to 0xb1e80
//     CWnd::OnNcDestroy();     // direct call to 0x289d80
//     delete this;             // vtable slot 1 (scalar deleting dtor, flag 1)
extern "C" void MS_ABI impl__OnNcDestroy_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(pThis);
    impl__OnNcDestroy_CWnd__IEAAXXZ(AsCwnd(pThis));
    delete reinterpret_cast<CPaneFrameWnd*>(pThis);
}

// Symbol: ?OnNcHitTest@CPaneFrameWnd@@IEAA_JVCPoint@@@Z
// Retail (0xb01c0), transcribed in full -- four instructions that tail-jump to
// vtable +0x3e8 with the third argument zeroed, i.e. `return HitTest(point, 0)`
// (slot 0x3e8 is CPaneFrameWnd::HitTest, 0xb01e0, in the vtable at 0x1802f5ad8).
// Deviation: the call is made directly to this class's HitTest instead of
// through the vtable, so a further-derived override is not reached.
extern "C" __int64 MS_ABI impl__OnNcHitTest_CPaneFrameWnd__IEAA_JVCPoint___Z(
    void* pThis, CPoint point) {
    if (pThis == nullptr) return 0;
    return impl__HitTest_CPaneFrameWnd__UEAA_JVCPoint__H_Z(pThis, point, 0);
}

// Symbol: ?OnNcLButtonDblClk@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// This export has no RVA of its own in mfc140_rva_symbols.json, but the retail
// body is still identifiable: CPaneFrameWnd's message map (the AFX_MSGMAP at
// .rdata 0x1802f5620, entries at 0x1802f5630) maps message 0xa3
// (WM_NCLBUTTONDBLCLK) with signature 0x36 -- void(UINT, CPoint), the same
// signature its OnLButtonDown/OnMouseMove entries use -- to RVA 0xdab0.  That
// address is a single `jmp 0x180289090` (?Default@CWnd@@IEAA_JXZ); the RVA map
// labels it ?VKeyToItem@CListBox@@UEAAHII@Z because the linker folded the two
// identical one-instruction bodies onto the same address.
// So the whole retail body is:  CWnd::Default();   -- neither argument is read.
extern "C" void MS_ABI impl__OnNcLButtonDblClk_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nHitTest*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}

// Symbol: ?OnNcMouseMove@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Retail (0xb2130), transcribed in full:
//     if (m_bInNcMouseMove /*0xe8*/ == 0 && m_bDragging /*0x10d*/ == 0)
//         OnTrackCaptionButtons(point);            // vtable 0x478
//     CWnd::Default();                             // tail jump
// TODO(clean-room): transcribed partially -- the 0xe8 re-entry byte has no
// counterpart here, so only the dragging test (the side table's `dragging`)
// guards the call.  The call is made directly rather than through the vtable.
extern "C" void MS_ABI impl__OnNcMouseMove_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nHitTest*/, CPoint point) {
    if (pThis == nullptr) return;
    bool bDragging = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        bDragging = GetState(pThis).dragging;
    }
    if (!bDragging) {
        impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(pThis, point);
    }
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}

// Symbol: ?OnNcPaint@CPaneFrameWnd@@IEAAXXZ
// Retail (0xaf9d0) paints the whole non-client frame: it resolves the docking
// manager (0x248 or afxGlobalUtils.GetDockingManager on the owner at 0x130),
// gets a window DC, and drives the active CMFCVisualManager to draw the border
// (vtable 0x438 OnDrawBorder), the caption (using the caption height at 0x114)
// and the caption buttons (vtable 0x440 OnDrawCaptionButtons).
// STUB: the visual manager drawing entry points and the caption-button list are
// not modelled; painting a hand-made approximation would look nothing like the
// rest of the frame.
extern "C" void MS_ABI impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnNeedTipText@CPaneFrameWnd@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
// Retail (0xb4120) checks that the notification came from its own tooltip
// control (0x1b8), converts the tooltip id into an index into the caption-button
// list (count at 0x198, head at 0x188), maps the button's id (+0x20) onto one of
// three string resource ids (0x3ea1, 0x3ee9, 0x4279), loads that string and
// stores it in the NMTTDISPINFO text pointer.
// STUB: both the tooltip control and the caption-button list are unmodelled, and
// the string resources are not present in this build.
extern "C" int MS_ABI impl__OnNeedTipText_CPaneFrameWnd__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    void* pThis, unsigned int id, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)id;
    (void)pNMHDR;
    (void)pResult;
    return FALSE;
}

// Symbol: ?OnPaneRecalcLayout@CPaneFrameWnd@@UEAAXXZ
// Retail (0xb0c30) gets the client rect, requires the hosted pane (0x148) to be
// a CPane, asks it for its minimum size (pane vtable 0x4f0) when a global
// resize-honouring flag is set, grows the client rect to fit, then calls the
// pane's own layout entry (pane vtable 0x480) and RedrawWindow.
// STUB: the two CPane virtuals are unexported and the hosted pane HWND is not
// modelled, so there is no size to lay out to.
extern "C" void MS_ABI impl__OnPaneRecalcLayout_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnPressButtons@CPaneFrameWnd@@MEAAXI@Z
// Retail (0xb4430) first gates the WHOLE body on a global: the compare at
// 0xb4463 against .data 0x1803b70bc (?m_bCustomizeMode@CMFCToolBar@@1HA)
// returns immediately when customize mode is on.  Otherwise, for the menu
// button (id 0x19) only, it looks the button up in the list at 0x188 and pops
// up the pane menu; every other id falls through and does nothing.
// STUB: no caption-button list exists in this object layout.  (The customize
// flag itself IS available here -- impl__m_bCustomizeMode_CMFCToolBar__1HA,
// declared in detail/CPaneFrameWndSupport.h -- but with no button to press
// there is nothing for the gate to guard.)
extern "C" void MS_ABI impl__OnPressButtons_CPaneFrameWnd__MEAAXI_Z(
    void* pThis, unsigned int uiID) {
    (void)pThis;
    (void)uiID;
}

// Symbol: ?OnSetCursor@CPaneFrameWnd@@IEAAHPEAVCWnd@@II@Z
// Retail (0xb0700), transcribed in full.  It switches on nHitTest only:
//     HTLEFT(10)/HTRIGHT(11)            -> IDC_SIZEWE   (0x7f84)
//     HTTOP(12)/HTBOTTOM(15)            -> IDC_SIZENS   (0x7f85)
//     HTTOPLEFT(13)/HTBOTTOMRIGHT(17)   -> IDC_SIZENWSE (0x7f82)
//     HTTOPRIGHT(14)/HTBOTTOMLEFT(16)   -> IDC_SIZENESW (0x7f83)
//     anything else                     -> tail jump to CWnd::Default()
// Each sizing branch does ::SetCursor(::LoadCursor(NULL, <id>)) and returns
// TRUE.  (Retail also calls AfxGetModuleState() on each branch and discards the
// result -- it loads the cursor with a NULL hInstance -- so it is not modelled.)
extern "C" int MS_ABI impl__OnSetCursor_CPaneFrameWnd__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* /*pWnd*/, unsigned int nHitTest, unsigned int /*message*/) {
    if (pThis == nullptr) return FALSE;
    const wchar_t* pCursor = nullptr;
    switch (nHitTest) {
    case HTLEFT:
    case HTRIGHT:
        pCursor = IDC_SIZEWE;
        break;
    case HTTOP:
    case HTBOTTOM:
        pCursor = IDC_SIZENS;
        break;
    case HTTOPLEFT:
    case HTBOTTOMRIGHT:
        pCursor = IDC_SIZENWSE;
        break;
    case HTTOPRIGHT:
    case HTBOTTOMLEFT:
        pCursor = IDC_SIZENESW;
        break;
    default:
        return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis)));
    }
    ::SetCursor(::LoadCursor(nullptr, pCursor));
    return TRUE;
}

// Symbol: ?OnSetFocus@CPaneFrameWnd@@IEAAXPEAVCWnd@@@Z
// Retail (0xb3e50):
//     CWnd::OnSetFocus(pOldWnd);
//     m_hwndLastFocus /*0x140*/ = NULL;
//     if (m_hwndPane /*0x148*/ && ::IsWindow(m_hwndPane) &&
//         CWnd::FromHandle(m_hwndPane)->IsKindOf(RUNTIME_CLASS(CMFCToolBar)))
//         m_hwndLastFocus = pOldWnd ? pOldWnd->m_hWnd : NULL;
//     CWnd* p = GetFirstVisiblePane();                 // vtable 0x368
//     if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane)) && p->vtable[0x3c0]())
//         p->SetFocus();
//     if (GetParentFrame())
//         GetParentFrame()->SetWindowPos(&wndTop, 0, 0, 0, 0,
//                                        SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
// The side table's `exitSizeMoveHwnd` is retail's 0x140, so that bookkeeping is
// reproduced.
// The `&wndTop` insert-after resolves to CWnd::wndTop.m_hWnd == HWND_TOP, which
// is what is passed to ::SetWindowPos below.
// TODO(clean-room): transcribed partially -- the CBasePane "can this pane take
// the focus" virtual (+0x3c0) is unexported, so the focus is given to the first
// visible pane whenever it is a CBasePane.
extern "C" void MS_ABI impl__OnSetFocus_CPaneFrameWnd__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pOldWnd) {
    if (pThis == nullptr) return;
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(AsCwnd(pThis), pOldWnd);
    HWND hLastFocus = nullptr;
    CWnd* pPane = reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    CRuntimeClass* pToolBarClass = impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
    if (pPane && pPane->GetSafeHwnd() && ::IsWindow(pPane->GetSafeHwnd()) &&
        pToolBarClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pToolBarClass)) {
        hLastFocus = pOldWnd ? pOldWnd->GetSafeHwnd() : nullptr;
    }
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).exitSizeMoveHwnd = hLastFocus;
    }
    CWnd* pVisible = reinterpret_cast<CWnd*>(
        impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    CRuntimeClass* pBasePane = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    if (pVisible && pVisible->GetSafeHwnd() && pBasePane != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pVisible, pBasePane)) {
        ::SetFocus(pVisible->GetSafeHwnd());
    }
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsCwnd(pThis));
    if (pFrame != nullptr && pFrame->GetSafeHwnd() != nullptr) {
        ::SetWindowPos(pFrame->GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0,
                       SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
}

// Symbol: ?OnSetRollUpTimer@CPaneFrameWnd@@UEAAXXZ
// Retail (0xaf4f0):
//     CWnd* p = GetPane();                             // vtable 0x360
//     if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane)) && (p->vtable[0x398]() & 0x10))
//         SetRollUpTimer();                            // direct call to 0xaf5d0
// STUB: whether the timer is started is decided entirely by an unexported
// CBasePane virtual (+0x398); starting the timer unconditionally would roll the
// frame up for panes that never asked for it.
extern "C" void MS_ABI impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnSettingChange@CPaneFrameWnd@@IEAAXIPEB_W@Z
// Retail (0xb4090 -- the ANSI twin's ?OnSettingChange@CPaneFrameWnd@@IEAAXIPEBD@Z),
// transcribed in full: CWnd::OnSettingChange(uFlags, lpszSection), then a tail
// call to vtable +0x498, which the CPaneFrameWnd vtable at 0x1802f5ad8 shows is
// CPaneFrameWnd::RecalcCaptionHeight (0xb40c0).
// Deviation: RecalcCaptionHeight is called directly rather than through the
// vtable, so a further-derived override is not reached.
extern "C" void MS_ABI impl__OnSettingChange_CPaneFrameWnd__IEAAXIPEB_W_Z(
    void* pThis, unsigned int uFlags, const wchar_t* lpszSection) {
    if (pThis == nullptr) return;
    impl__OnSettingChange_CWnd__IEAAXIPEB_W_Z(AsCwnd(pThis), uFlags, lpszSection);
    impl__RecalcCaptionHeight_CPaneFrameWnd__MEAAXXZ(pThis);
}

// Symbol: ?OnSize@CPaneFrameWnd@@IEAAXIHH@Z
// Retail (0xb0a40):
//     CWnd::Default();
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     if (p && p->IsKindOf(RUNTIME_CLASS(CPane))) p->vtable[0x430]();
//     CRect rect; ::GetWindowRect(m_hWnd, &rect);
//     afxGlobalUtils.AdjustRectToWorkArea(rect, NULL);
//     SetWindowPos(NULL, rect.left, rect.top, 0, 0,
//                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//     ArrangeCaptionButtons();                     // direct call to 0xb1990
//     ::SendMessage(m_hWnd, WM_NCPAINT /*0x85, the immediate at 0xb0b05*/, 0, 0);
// TODO(clean-room): transcribed partially -- the hosted pane's unexported
// layout virtual (+0x430) is skipped; everything else is reproduced.
extern "C" void MS_ABI impl__OnSize_CPaneFrameWnd__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return;
    RECT wr = {};
    ::GetWindowRect(hwnd, &wr);
    CRect rect(wr);
    impl__AdjustRectToWorkArea_CGlobalUtils__QEAAXAEAVCRect__PEAV2__Z(
        impl__afxGlobalUtils__3VCGlobalUtils__A, &rect, nullptr);
    ::SetWindowPos(hwnd, nullptr, rect.left, rect.top, 0, 0,
                   SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
    ::SendMessage(hwnd, WM_NCPAINT, 0, 0);
}

// Symbol: ?OnSizing@CPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
// Retail (0xb0790) constrains an interactive resize: after CWnd::Default() it
// requires the hosted pane (0x148) to be a CMFCToolBar, subtracts the border
// (vtable 0x388) and caption height (0x114) to get the client size the toolbar
// would receive, asks the toolbar to reflow at that size (pane vtable 0x558 and
// 0x420) and snaps lpRect to the size the toolbar reports back.
// STUB: the toolbar reflow virtuals are unexported and the caption height is
// not modelled, so there is no size to snap to.
extern "C" void MS_ABI impl__OnSizing_CPaneFrameWnd__IEAAXIPEAUtagRECT___Z(
    void* pThis, unsigned int nSide, RECT* lpRect) {
    (void)pThis;
    (void)nSide;
    (void)lpRect;
}

// Symbol: ?OnTimer@CPaneFrameWnd@@IEAAX_K@Z
// Retail (0xb1240):
//     if (nIDEvent == 0xec04) {
//         CDockingManager* p = m_pDockManager /*0x248*/;
//         if (!p) p = afxGlobalUtils.GetDockingManager(
//                         CWnd::FromHandlePermanent(m_hwndOwner /*0x130*/));
//         p->vtable[0x88](this);       // tail call -- CWnd::Default is NOT reached
//         return;
//     }
//     if (nIDEvent == 0xec05) OnCheckRollState();   // vtable 0x490
//     CWnd::Default();
// TODO(clean-room): transcribed partially -- the 0xec04 docking tick needs the
// docking manager, so that branch returns without doing anything (matching
// retail in that it does not fall through to CWnd::Default()).
extern "C" void MS_ABI impl__OnTimer_CPaneFrameWnd__IEAAX_K_Z(
    void* pThis, unsigned __int64 nIDEvent) {
    if (pThis == nullptr) return;
    if (nIDEvent == 0xec04) return;
    if (nIDEvent == 0xec05) {
        impl__OnCheckRollState_CPaneFrameWnd__MEAAXXZ(pThis);
    }
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}

// Symbol: ?OnTrackCaptionButtons@CPaneFrameWnd@@MEAAXVCPoint@@@Z
// Retail (0xb2170) returns immediately when the global at .data 0x1803b6fe8
// (?m_pActivePopupMenu@CMFCPopupMenu@@1PEAV1@EA) is non-NULL -- the test at
// 0xb217f.  Otherwise it hit-tests the caption-button list at 0x188 through
// FindButton(point) (0xb1720), moves the "hot" button id at 0x11c (in
// customize mode -- the same 0x1803b70bc flag OnPressButtons reads -- only ids
// below 0x14 may go hot), redraws the button that gained and the button that
// lost the highlight (RedrawCaptionButton, 0xb1a80) and takes or releases the
// mouse capture so that the highlight can be cleared when the cursor leaves.
// STUB: no caption-button list exists in this object layout, so there is
// nothing to track -- and taking the capture here without any button to
// release it on would strand the mouse.
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(
    void* pThis, CPoint point) {
    (void)pThis;
    (void)point;
}

// Symbol: ?OnUpdateToolTips@CPaneFrameWnd@@IEAA_J_K_J@Z
// Retail (0xb4a00), transcribed:
//     if (wParam & 8) {
//         CTooltipManager::CreateToolTip(m_pToolTip /*0x1b8*/, this, 8);
//         SetCaptionButtonsToolTips();     // direct call to 0xb4300
//         UpdateTooltips();                // direct call to 0xb1870
//     }
//     return 0;
// TODO(clean-room): transcribed partially -- the tooltip control at 0x1b8 is
// not modelled, so CTooltipManager::CreateToolTip is skipped; the two direct
// calls that follow it are reproduced.
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CPaneFrameWnd__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 /*lParam*/) {
    if (pThis == nullptr) return 0;
    if ((wParam & 8) != 0) {
        impl__SetCaptionButtonsToolTips_CPaneFrameWnd__IEAAXXZ(pThis);
        impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(pThis);
    }
    return 0;
}

// Symbol: ?OnWindowPosChanging@CPaneFrameWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
// Retail (0xb0f80), transcribed in full:
//     lpwndpos->flags |= SWP_NOACTIVATE;
//     if (!(lpwndpos->flags & SWP_NOMOVE)) {
//         CRect rectWindow; ::GetWindowRect(m_hWnd, &rectWindow);
//         CRect rect(0,0,0,0);
//         if (lpwndpos->flags & SWP_NOSIZE)
//             ::SetRect(&rect, lpwndpos->x, lpwndpos->y,
//                       lpwndpos->x + rectWindow.Width(),
//                       lpwndpos->y + rectWindow.Height());
//         else
//             ::SetRect(&rect, lpwndpos->x, lpwndpos->y,
//                       lpwndpos->x + lpwndpos->cx, lpwndpos->y + lpwndpos->cy);
//         int n = GetCaptionHeight();      // vtable 0x2f0 -> 0x92350, `return m_nCaptionHeight;`
//         CRect rectDelta(n, n, n, n);
//         afxGlobalUtils.AdjustRectToWorkArea(rect, &rectDelta);
//         lpwndpos->x = rect.left; lpwndpos->y = rect.top;
//     }
//     CWnd::Default();
// Deviation: retail's caption height (0x114) has no counterpart here, so the
// file's kPaneFrameCaptionHeight constant is used for the delta rect.
extern "C" void MS_ABI impl__OnWindowPosChanging_CPaneFrameWnd__IEAAXPEAUtagWINDOWPOS___Z(
    void* pThis, WINDOWPOS* lpwndpos) {
    if (pThis == nullptr) return;
    if (lpwndpos != nullptr) {
        lpwndpos->flags |= SWP_NOACTIVATE;
        if ((lpwndpos->flags & SWP_NOMOVE) == 0) {
            HWND hwnd = HwndOf(pThis);
            RECT wr = {};
            if (hwnd) ::GetWindowRect(hwnd, &wr);
            CRect rect(0, 0, 0, 0);
            if ((lpwndpos->flags & SWP_NOSIZE) != 0) {
                rect.SetRect(lpwndpos->x, lpwndpos->y,
                             lpwndpos->x + (wr.right - wr.left),
                             lpwndpos->y + (wr.bottom - wr.top));
            } else {
                rect.SetRect(lpwndpos->x, lpwndpos->y,
                             lpwndpos->x + lpwndpos->cx, lpwndpos->y + lpwndpos->cy);
            }
            CRect rectDelta(kPaneFrameCaptionHeight, kPaneFrameCaptionHeight,
                            kPaneFrameCaptionHeight, kPaneFrameCaptionHeight);
            impl__AdjustRectToWorkArea_CGlobalUtils__QEAAXAEAVCRect__PEAV2__Z(
                impl__afxGlobalUtils__3VCGlobalUtils__A, &rect, &rectDelta);
            lpwndpos->x = rect.left;
            lpwndpos->y = rect.top;
        }
    }
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}

// Symbol: ?PaneFromPoint@CPaneFrameWnd@@UEAAPEAVCBasePane@@VCPoint@@HH@Z
// Retail (0xb36c0), transcribed in full.  nSensitivity is never read:
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     if (!p || !p->IsKindOf(RUNTIME_CLASS(CPane))) return NULL;
//     CRect rect; ::GetWindowRect(p->m_hWnd, &rect);
//     if (!::PtInRect(&rect, point)) return NULL;
//     if (::IsWindowVisible(p->m_hWnd)) return p;
//     return bCheckVisibility ? NULL : p;
// GetPane() is retail's own FromHandlePermanent(m_hwndPane) (0xaf900) and
// stands in for the 0x148 lookup.
// Deviation: the RUNTIME_CLASS(CPane) filter is relaxed to CBasePane, because
// OpenMFC has a CBasePane RTTI thunk but no CPane one.
extern "C" void* MS_ABI impl__PaneFromPoint_CPaneFrameWnd__UEAAPEAVCBasePane__VCPoint__HH_Z(
    void* pThis, CPoint point, int /*nSensitivity*/, int bCheckVisibility) {
    if (pThis == nullptr) return nullptr;
    CWnd* pPane = reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    if (pPane == nullptr) return nullptr;
    HWND hPane = pPane->GetSafeHwnd();
    if (hPane == nullptr) return nullptr;
    CRuntimeClass* pBasePane = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    if (pBasePane == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pBasePane)) {
        return nullptr;
    }
    RECT rect = {};
    ::GetWindowRect(hPane, &rect);
    POINT pt = {point.x, point.y};
    if (!::PtInRect(&rect, pt)) return nullptr;
    if (::IsWindowVisible(hPane)) return pPane;
    return bCheckVisibility ? nullptr : pPane;
}

// Symbol: ?PreCreateWindow@CPaneFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z
// Retail 0xb1c80 -- the ANSI twin's
// ?PreCreateWindow@CPaneFrameWnd@@MEAAHAEAUtagCREATESTRUCTA@@@Z.  (The mangled
// name above has no entry of its own in mfc140_rva_symbols.json; slot 0xd8 of
// the CPaneFrameWnd vtable at .rdata 0x1802f5ad8 is what points at 0xb1c80.)
// Two instructions, transcribed in full:
//     btsl $8, cs.style      // cs.style |= MFS_SYNCACTIVE (0x100)
// (MFS_SYNCACTIVE is 0x100 in MFC's own afxwin.h but is not defined by any
// OpenMFC header, hence the literal below.  Retail's OnNcCreate, 0xb1c90, reads
// the same bit back out of the created window's style at 0xb1cb7.)
//     jmp  CWnd::PreCreateWindow(cs)          // 0x289ac0, tail jump
extern "C" int MS_ABI impl__PreCreateWindow_CPaneFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW& cs) {
    if (pThis == nullptr) return FALSE;
    cs.style |= 0x100 /*MFS_SYNCACTIVE*/;
    return impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(AsCwnd(pThis), cs);
}

// Symbol: ?PreTranslateMessage@CPaneFrameWnd@@UEAAHPEAUtagMSG@@@Z
// Retail (0xb4250), transcribed.  The switch at 0xb425a..0xb42b8 selects on
// pMsg->message and every arm lands on the same block:
//     case WM_NCLBUTTONDOWN(0xa1): case WM_NCLBUTTONUP(0xa2):
//     case WM_NCRBUTTONDOWN(0xa4): case WM_NCRBUTTONUP(0xa5):
//     case WM_NCMBUTTONDOWN(0xa7): case WM_NCMBUTTONUP(0xa8):
//     case WM_KEYDOWN(0x100):      case WM_SYSKEYDOWN(0x104):
//     case WM_MOUSEMOVE(0x200):    case WM_LBUTTONDOWN(0x201):
//     case WM_LBUTTONUP(0x202):    case WM_RBUTTONDOWN(0x204):
//     case WM_RBUTTONUP(0x205):    case WM_MBUTTONDOWN(0x207):
//     case WM_MBUTTONUP(0x208):
//         if (m_pToolTip /*0x1b8*/ != NULL && m_pToolTip->m_hWnd != NULL)
//             ::SendMessage(m_pToolTip->m_hWnd, TTM_RELAYEVENT /*0x407*/,
//                           0, (LPARAM)pMsg);
//         break;
//     return CWnd::PreTranslateMessage(pMsg);     // 0x28a080, tail jump
// TODO(clean-room): transcribed partially -- the tooltip control at 0x1b8 has
// no counterpart in this object layout, so the relay is skipped and only the
// unconditional CWnd::PreTranslateMessage tail call is reproduced.  That is
// exactly what retail does on every path whenever m_pToolTip is NULL.
extern "C" int MS_ABI impl__PreTranslateMessage_CPaneFrameWnd__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg) {
    if (pThis == nullptr) return FALSE;
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(AsCwnd(pThis), pMsg);
}

// Symbol: ?RecalcCaptionHeight@CPaneFrameWnd@@MEAAXXZ
// Retail (0xb40c0), transcribed:
//     CSize sz = afxGlobalData.<unexported virtual, vtable +0x298>();
//     m_nCaptionHeight /*0x114*/ = sz.cy + ::GetSystemMetrics(SM_CYSMCAPTION);
//     m_?? /*0x160*/ = m_?? /*0x164*/ = m_nCaptionHeight + 15;
// (afxGlobalData is the lazily-constructed singleton returned by 0x97f4; the
// virtual at +0x298 returns a CSize by value and only its .cy -- the second
// int of the returned pair, read at 0xb40ef -- is used.)
// STUB: neither the afxGlobalData virtual nor a caption-height member exists
// here.  PaneFrameState (phase4/src/detail/CPaneFrameWndSupport.h) has no field
// to hold the result, and this file's GetCaptionRect / OnNcCalcSize use the
// fixed kPaneFrameCaptionHeight constant, so a computed height would have
// nowhere to go and no reader.  See headerRequests.
extern "C" void MS_ABI impl__RecalcCaptionHeight_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?RedrawAll@CPaneFrameWnd@@SAXXZ
// Retail (0xb3620) walks a private list of live mini-frame HWNDs whose head
// node pointer is the qword at .data 0x1803aadd0 (node layout: pNext at +0,
// the HWND at +0x10) and, for every entry that CWnd::FromHandlePermanent
// (0x2891d0) still resolves, calls
//     ::RedrawWindow(hwnd, NULL, NULL, 0x585)
// i.e. RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_UPDATENOW|RDW_FRAME.
// STUB: OpenMFC keeps no list of live CPaneFrameWnd windows.  Neither side
// table in this file stores a frame's own HWND (g_framePanes is keyed by object
// pointer and is never pruned when a frame dies; g_other6State only holds the
// hosted-pane and exit-size-move handles), so recovering an HWND would mean
// dereferencing an object that may already be freed.
extern "C" void MS_ABI impl__RedrawAll_CPaneFrameWnd__SAXXZ() {}

// Symbol: ?RedrawCaptionButton@CPaneFrameWnd@@IEAAXPEAVCMFCCaptionButton@@@Z
// Retail (0xb1a80), transcribed in full:
//     if (pButton == NULL) return;
//     m_rectRedraw /*0x168, 16 bytes*/ = pButton-><vtable +0x28>();  // its rect
//     ::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
//     ::SetRectEmpty(&m_rectRedraw);
//     ::UpdateWindow(m_hWnd);
// (OnNcPaint consults m_rectRedraw so that the repaint is clipped to the one
// button; clearing it afterwards restores full-frame painting.)
// TODO(clean-room): transcribed partially -- CMFCCaptionButton is not modelled
// and there is no m_rectRedraw member here, so the clip rect is not set and the
// whole non-client area is repainted instead of just the button.  The NULL
// guard and the WM_NCPAINT + UpdateWindow pair are reproduced exactly.
extern "C" void MS_ABI impl__RedrawCaptionButton_CPaneFrameWnd__IEAAXPEAVCMFCCaptionButton___Z(
    void* pThis, void* pButton) {
    if (pThis == nullptr || pButton == nullptr) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return;
    ::SendMessage(hwnd, WM_NCPAINT, 0, 0);
    ::UpdateWindow(hwnd);
}

// Symbol: ?RemoveAllCaptionButtons@CPaneFrameWnd@@IEAAXXZ
// Retail (0xb1810), transcribed in full:
//     m_dwCaptionButtons /*0x12c*/ = 0;             // unconditional, first
//     while (m_lstCaptionButtons.GetCount() /*0x198*/ != 0) {
//         CObject* p = m_lstCaptionButtons.RemoveHead();   // 0x230560
//         if (p) p-><vtable slot 1>(1);             // scalar deleting dtor
//     }
// (m_lstCaptionButtons is the CObList embedded at 0x180: head 0x188, tail
// 0x190, count 0x198.)
// TODO(clean-room): transcribed partially -- there is no caption-button list in
// this object layout, so only the mask store is reproduced.  It is kept in the
// side table's `rollupFlags`, which is retail's 0x12c (the field's name in
// CPaneFrameWndSupport.h predates the decode of SetCaptionButtons; the word
// really is the caption-button mask SetCaptionButtons stores, and bit 0 of it
// is the button SetRollUpTimer turns on -- AddButton id 9).
extern "C" void MS_ABI impl__RemoveAllCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    GetState(pThis).rollupFlags = 0;
}

// Symbol: ?RemoveButton@CPaneFrameWnd@@IEAAXI@Z
// Retail (0xb1630), transcribed in full:
//     for (POSITION p = m_lstCaptionButtons.GetHeadPosition() /*0x188*/; p; ) {
//         CMFCCaptionButton* b = <node data at +0x10>;
//         if (b->m_nHit /*+0x20*/ == uiID) {
//             m_lstCaptionButtons.RemoveAt(p);      // 0x2306f0
//             b-><vtable slot 1>(1);                // scalar deleting dtor
//             break;
//         }
//         p = <node pNext at +0>;
//     }
//     ArrangeCaptionButtons();                      // 0xb1990, UNCONDITIONAL tail jump
// TODO(clean-room): transcribed partially -- no caption-button list exists in
// this object layout, so nothing is removed; only retail's unconditional
// ArrangeCaptionButtons tail call is reproduced.
extern "C" void MS_ABI impl__RemoveButton_CPaneFrameWnd__IEAAXI_Z(
    void* pThis, unsigned int uiID) {
    if (pThis == nullptr) return;
    (void)uiID;
    impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
}

// Symbol: ?RemoveNonValidPanes@CPaneFrameWnd@@UEAAXXZ
// Retail (0xb3c80), transcribed in full.  It decides whether the hosted pane is
// still usable and, if not, forgets it:
//     BOOL bValid = FALSE;
//     CWnd* p = GetPane();                              // vtable 0x360
//     if (p && p->IsKindOf(RUNTIME_CLASS(CWnd)) && ::IsWindow(p->m_hWnd)) {
//         if (p->GetDlgCtrlID() != -1) bValid = TRUE;   // 0x2a78b0
//         else {
//             CWnd* p2 = GetPane();
//             bValid = p2 && p2->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)) &&
//                      p2-><unexported CBaseTabbedPane virtual, +0x758>();
//         }
//     }
//     if (!bValid) m_hwndPane /*0x148*/ = NULL;
// Here "forget the pane" means both halves of this file's split model of retail
// 0x148: the side table's `paneHwnd` and the entry in g_framePanes that
// GetPane() actually reads.  Retail has a single pane handle, so its
// post-condition is GetPane() == NULL; this file's g_framePanes entry is a
// list, so only the pane that failed the test is dropped and GetPane() falls
// through to the next one when several were added.
// Deviations:
//   * the CBaseTabbedPane branch's unexported virtual (+0x758) cannot be
//     called, so a tabbed pane whose control id is -1 is KEPT rather than
//     dropped.  Retail drops it when that virtual returns 0; keeping it is the
//     conservative direction (dropping a still-live pane loses it).
//   * retail's IsKindOf(RUNTIME_CLASS(CWnd)) test on the pane is not emitted --
//     GetPane() already yields a CWnd* here -- so only the ::IsWindow test
//     guards the handle.
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CWnd* pPane = reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    BOOL bValid = FALSE;
    if (pPane != nullptr && pPane->GetSafeHwnd() != nullptr &&
        ::IsWindow(pPane->GetSafeHwnd())) {
        if (impl__GetDlgCtrlID_CWnd__QEBAHXZ(pPane) != -1) {
            bValid = TRUE;
        } else {
            CRuntimeClass* pTabbed = impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
            bValid = (pTabbed != nullptr &&
                      impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pTabbed))
                         ? TRUE : FALSE;
        }
    }
    if (bValid) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).paneHwnd = nullptr;
    }
    if (pPane != nullptr) {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        RemovePane(g_framePanes[pThis], pPane);
    }
}

// Symbol: ?ReplaceButton@CPaneFrameWnd@@IEAAXII@Z
// Retail (0xb1690), transcribed in full:
//     for (POSITION p = m_lstCaptionButtons.GetHeadPosition() /*0x188*/; p;
//          p = <node pNext at +0>) {
//         CMFCCaptionButton* b = <node data at +0x10>;
//         if (b->m_nHit /*+0x20*/ == uiOldID) { b->m_nHit = uiNewID; return; }
//     }
// STUB: no caption-button list exists in this object layout, so there is no
// button whose id could be rewritten.
extern "C" void MS_ABI impl__ReplaceButton_CPaneFrameWnd__IEAAXII_Z(
    void* pThis, unsigned int uiOldID, unsigned int uiNewID) {
    (void)pThis;
    (void)uiOldID;
    (void)uiNewID;
}

// Symbol: ?ReplacePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@0@Z
// Retail (0xaf880), transcribed in full:
//     if (!pOldBar || !pNewBar || pOldBar == pNewBar) AfxThrowInvalidArgException();
//     AddRemovePaneFromGlobalList(pOldBar, FALSE);       // 0xaf740
//     if (m_hwndPane /*0x148*/ == pOldBar->m_hWnd) m_hwndPane = pNewBar->m_hWnd;
//     AddRemovePaneFromGlobalList(pNewBar, TRUE);
//     OnSetRollUpTimer();                                // vtable 0x3f8, tail jump
// Deviations, both documented:
//   * OpenMFC has no AfxThrowInvalidArgException thunk, so the bad-argument
//     case returns instead of throwing.
//   * this file models retail's single 0x148 pane handle two ways -- the side
//     table's `paneHwnd` and the g_framePanes list GetPane() reads -- so both
//     are updated: the handle is swapped when it matches, and pOldBar is
//     replaced by pNewBar in the list (which moves it to the list's tail).
//   * OnSetRollUpTimer is called directly rather than through the vtable, so a
//     further-derived override is not reached.
extern "C" void MS_ABI impl__ReplacePane_CPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(
    void* pThis, void* pOldBar, void* pNewBar) {
    if (pThis == nullptr) return;
    if (pOldBar == nullptr || pNewBar == nullptr || pOldBar == pNewBar) return;
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pOldBar, FALSE);
    HWND hOld = reinterpret_cast<CWnd*>(pOldBar)->GetSafeHwnd();
    HWND hNew = reinterpret_cast<CWnd*>(pNewBar)->GetSafeHwnd();
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        if (st.paneHwnd == hOld) st.paneHwnd = hNew;
    }
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        PaneState& state = g_framePanes[pThis];
        RemovePane(state, pOldBar);
        AddUniquePane(state, pNewBar);
    }
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pNewBar, TRUE);
    impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(pThis);
}

// Symbol: ?SaveRecentFloatingState@CPaneFrameWnd@@MEAAXXZ
// Retail (0xada90), transcribed in full:
//     ::GetWindowRect(m_hWnd, &m_rectRecentFloatingRect /*0x150*/);
//     if (m_hwndPane /*0x148*/ != NULL) {
//         CWnd* p = CWnd::FromHandlePermanent(m_hwndPane);
//         if (p && p->IsKindOf(RUNTIME_CLASS(CPane))) {
//             p->m_rectRecentFloating /*pane +0x2c0*/ = m_rectRecentFloatingRect;
//             CPoint pt(0, 0);
//             ::GetCursorPos(&pt);
//             ::ScreenToClient(p->m_hWnd, &pt);
//             p->m_ptRecentFloating /*pane +0x208*/ = pt;
//         }
//     }
// TODO(clean-room): transcribed partially -- the two CPane members (0x2c0,
// 0x208) are not modelled, so only the frame's own half is reproduced.  Its
// counterpart here is g_framePanes[this].recentRect, the rectangle this file's
// Create / AdjustLayout / IsRollUp bookkeeping already treats as retail 0x150.
extern "C" void MS_ABI impl__SaveRecentFloatingState_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return;
    RECT wr = {};
    ::GetWindowRect(hwnd, &wr);
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_framePanes[pThis].recentRect = CRect(wr);
}

// Symbol: ?SaveState@CPaneFrameWnd@@UEAAHPEB_WI@Z
// Retail 0xb2c40 -- the ANSI twin's ?SaveState@CPaneFrameWnd@@UEAAHPEBDI@Z.
// (The mangled name above has no entry of its own in mfc140_rva_symbols.json;
// slot 0x3b8 of the CPaneFrameWnd vtable at .rdata 0x1802f5ad8 points there.)
// Transcribed in full:
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     if (!p || !p->IsKindOf(RUNTIME_CLASS(CBasePane) /*0x1802da490*/)) return TRUE;
//     return p-><vtable +0x470>(lpszProfileName, uiID, (UINT)-1);
// (that CBasePane virtual is SaveState(LPCTSTR, int nIndex, UINT uiID) -- note
// retail passes this frame's uiID as the pane's nIndex and -1 as its uiID.)
// STUB: OpenMFC's CBasePane::SaveState is itself a null stub and its thunk
// (phase4/src/featurepack/docking/CBasePane.cpp:397) is declared without a
// `this` parameter, so it cannot be called correctly from here.  TRUE is what
// retail returns whenever there is no CBasePane to delegate to, but this body does
// not implement the delegation and must not be read as if it did.
extern "C" int MS_ABI impl__SaveState_CPaneFrameWnd__UEAAHPEB_WI_Z(
    void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)uiID;
    return TRUE;
}

// Symbol: ?Serialize@CPaneFrameWnd@@UEAAXAEAVCArchive@@@Z
// Retail (0xb2d20) is a hand-inlined CArchive body (the store/load switch is
// `test $1, ar.m_nMode` -- bit 0 is CArchive::load, so the fall-through path at
// 0xb2d5e is the LOAD path).  Six items are serialized, in this order:
//   store (0xb2ef4): the DWORD CWnd::GetStyle (0x2a75a0) returns, then the
//     16-byte ::GetWindowRect -- whose bottom is first replaced by
//     rect.top + <int at 0x110> when the flag at 0xf8 is non-zero, the same
//     pair OnCheckRollState uses to restore a frame's un-rolled height -- then
//     ::IsWindowVisible(m_hWnd) as a DWORD, then the hosted pane's
//     GetDlgCtrlID (0 when 0x148 resolves to nothing), then
//     m_dwCaptionButtons (0x12c), then the int at 0xfc.
//   load: style, rect, one skipped DWORD (the visible flag), then 0x128, 0x12c
//     and 0xfc; it clears bit 28 of the style word and calls its own vtable
//     +0x418 (CPaneFrameWnd::Create) with the loaded rect and, as the parent,
//     the global at .data 0x1803b6fe0
//     (?m_pParentWndForSerialize@CPaneFrameWnd@@1PEAVCFrameWnd@@EA), finally
//     storing that parent's m_hWnd into 0x130.
// STUB: four of the six items (0xf8/0x110, 0xfc, 0x128, 0x130) have no
// counterpart in this object layout.  OpenMFC does define the parent global
// (phase4/src/core/runtime/StaticData.cpp) -- correcting an earlier note here
// that said it did not -- but nothing ever sets it, so the load path would
// re-create the frame with a null parent.  A partial round-trip would silently
// produce a frame with the wrong geometry and no pane, which is worse than not
// restoring it at all.
extern "C" void MS_ABI impl__Serialize_CPaneFrameWnd__UEAAXAEAVCArchive___Z(
    void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// Symbol: ?SetCaptionButtons@CPaneFrameWnd@@UEAAXK@Z
// Retail (0xb13f0), transcribed in full:
//     RemoveAllCaptionButtons();                        // 0xb1810
//     if (dwButtons & 2) {
//         CWnd* p = GetPane();                          // vtable 0x360
//         if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane) /*0x1802da490*/) &&
//             p-><vtable +0x3a0>())
//             AddButton(0x14);                          // vtable 0x470
//     }
//     if (dwButtons & 1)    AddButton(9);
//     if (dwButtons & 4)    AddButton(8);
//     if (dwButtons & 0x10) AddButton(0x19);            // the menu button
//     m_dwCaptionButtons /*0x12c*/ = dwButtons;
//     SetCaptionButtonsToolTips();                      // 0xb4300
//     ArrangeCaptionButtons();                          // 0xb1990
//     ::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);          // tail jump
// The four AddButton calls and the RemoveAllCaptionButtons call are reproduced
// as calls (AddButton has no list to add to yet, so today they do nothing, but
// the control flow is retail's).  The mask store is real: it goes to the side
// table's `rollupFlags`, which is retail's 0x12c -- SetRollUpTimer and
// RemoveAllCaptionButtons below read and clear the same word.
// TODO(clean-room): transcribed partially -- the extra CBasePane virtual
// (+0x3a0) that retail requires before adding button 0x14 cannot be called, so
// the bit-1 branch is taken on the CBasePane test alone.  That is observable
// only once AddButton itself is implemented.
extern "C" void MS_ABI impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(
    void* pThis, unsigned long dwButtons) {
    if (pThis == nullptr) return;
    impl__RemoveAllCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
    if ((dwButtons & 0x2) != 0) {
        CWnd* pPane = reinterpret_cast<CWnd*>(
            impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
        CRuntimeClass* pPaneClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
        if (pPane != nullptr && pPaneClass != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pPaneClass)) {
            impl__AddButton_CPaneFrameWnd__MEAAXI_Z(pThis, 0x14);
        }
    }
    if ((dwButtons & 0x1) != 0)  impl__AddButton_CPaneFrameWnd__MEAAXI_Z(pThis, 9);
    if ((dwButtons & 0x4) != 0)  impl__AddButton_CPaneFrameWnd__MEAAXI_Z(pThis, 8);
    if ((dwButtons & 0x10) != 0) impl__AddButton_CPaneFrameWnd__MEAAXI_Z(pThis, 0x19);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).rollupFlags = static_cast<int>(dwButtons);
    }
    impl__SetCaptionButtonsToolTips_CPaneFrameWnd__IEAAXXZ(pThis);
    impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
    HWND hwnd = HwndOf(pThis);
    if (hwnd) ::SendMessage(hwnd, WM_NCPAINT, 0, 0);
}

// Symbol: ?SetCaptionButtonsToolTips@CPaneFrameWnd@@IEAAXXZ
// Retail (0xb4300) drives its own tooltip control (m_pToolTip, 0x1b8): it asks
// the control for its tool count (TTM_GETTOOLCOUNT, 0x40d), compares it with
// the caption-button count (0x198), adds tools with CTooltipManager (0x273d40)
// until the counts match, then deletes surplus tools (0x273c00) with an empty
// rect.  Every step needs either the tooltip control or the caption-button
// list.
// STUB: neither exists in this object layout.
extern "C" void MS_ABI impl__SetCaptionButtonsToolTips_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?SetDockingTimer@CPaneFrameWnd@@QEAAXI@Z
// Retail (0xb2860), transcribed in full:
//     if (m_nDockTimerID /*0x120*/ != 0) KillDockingTimer();   // 0xb28b0
//     m_nDockTimerID = ::SetTimer(m_hWnd, 0xec04, nTimeOut, NULL);
// (0xec04 is the docking tick OnTimer above already dispatches on.)
extern "C" void MS_ABI impl__SetDockingTimer_CPaneFrameWnd__QEAAXI_Z(
    void* pThis, unsigned int nTimeOut) {
    if (pThis == nullptr) return;
    int id = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        id = GetState(pThis).dockTimerID;
    }
    if (id != 0) impl__KillDockingTimer_CPaneFrameWnd__QEAAXXZ(pThis);
    HWND hwnd = HwndOf(pThis);
    const UINT_PTR nNew = hwnd ? ::SetTimer(hwnd, 0xec04, nTimeOut, nullptr) : 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).dockTimerID = static_cast<int>(nNew);
    }
}

// Symbol: ?SetDockState@CPaneFrameWnd@@UEAAXPEAVCDockingManager@@@Z
// Retail (0xb31c0) restores a floating frame from a saved dock state.  It reads
// the saved pane id at 0x128, asks the CDockingManager argument for the pane
// with that id (manager vtable +0x58), and then re-parents and re-shows it:
// ::GetParent / ::SetParent on the frame's owner (0x130), a pane virtual at
// +0x790, a CDockablePane branch (+0x3f0, +0x3c8, +0x6e0), rect fix-ups through
// +0x460 / +0x408, CWnd::SetWindowPos and finally SetCaptionButtons with the
// mask at 0x12c.  When the saved id is 0, or the manager cannot produce a pane,
// it calls its own DestroyWindow (vtable 0xd0) instead.
// STUB: the CDockingManager and eight unexported pane virtuals decide
// everything here, and the saved-id member (0x128) is not modelled.  Guessing
// would leave a frame either destroyed or holding a pane it never owned.
extern "C" void MS_ABI impl__SetDockState_CPaneFrameWnd__UEAAXPEAVCDockingManager___Z(
    void* pThis, void* pDockManager) {
    (void)pThis;
    (void)pDockManager;
}

// Symbol: ?SetPreDockState@CPaneFrameWnd@@UEAAHW4AFX_PREDOCK_STATE@@PEAVCBasePane@@W4AFX_DOCK_METHOD@@@Z
// Retail (0xb0d80), transcribed in full.  dockMethod (the 4th argument) is
// never read:
//     if (preDockState == PDS_NOTHING /*0*/) return TRUE;
//     if (preDockState == 2 && pBar && !pBar-><vtable +0x328>()) return TRUE;
//     if (pBar && pBar-><vtable +0x460>(0) != NULL) return TRUE;
//     BOOL bWasDragging = m_bDragging /*0x10d*/;
//     if (bWasDragging) {
//         ::ReleaseCapture();
//         m_bDragging = FALSE;
//         OnCapture(FALSE);                            // vtable 0x428
//     }
//     CPoint pt(0, 0); ::GetCursorPos(&pt);
//     ::ScreenToClient(GetPane()->m_hWnd, &pt);        // result discarded
//     m_preDockState /*0x250*/ = preDockState;
//     m_pDockBar /*0x1c0*/ = (pBar && pBar->IsKindOf(RUNTIME_CLASS(CDockablePane)
//                                                    /*0x1803aa178*/)) ? pBar : NULL;
//     BOOL bWasDocked = 0;
//     CDockablePane* p = DockPane(bWasDocked);         // vtable 0x318
//     if (p) {
//         p-><vtable +0x4e0>(0);
//         if (bWasDragging) p-><vtable +0x638>(0);
//     }
//     return FALSE;
// STUB: the two CBasePane virtuals that gate every early return (+0x328,
// +0x460) are unexported, so this body cannot tell the "nothing to do" case
// from the real one -- and taking the wrong branch would either release a drag
// capture that a live drag still owns or silently skip the dock.  The 0x250 and
// 0x1c0 members are not modelled either.  TRUE (retail's "nothing to do")
// is returned; this is not an implementation of the docking path.
extern "C" int MS_ABI impl__SetPreDockState_CPaneFrameWnd__UEAAHW4AFX_PREDOCK_STATE__PEAVCBasePane__W4AFX_DOCK_METHOD___Z(
    void* pThis, int preDockState, void* pBar, int dockMethod) {
    (void)pThis;
    (void)preDockState;
    (void)pBar;
    (void)dockMethod;
    return TRUE;
}

// Symbol: ?SetRollUpTimer@CPaneFrameWnd@@IEAAXXZ
// Retail (0xaf5d0), transcribed in full:
//     if (m_nRollUpTimerID /*0x124*/ != 0) return;
//     m_nRollUpTimerID = ::SetTimer(m_hWnd, 0xec05, m_nRollTimeOut, NULL);
//     SetCaptionButtons(m_dwCaptionButtons /*0x12c*/ | 1);   // vtable 0x328
// (0xec05 is the roll-state tick OnTimer above dispatches on; the OR with 1 is
// what puts the roll-up pin into the caption-button mask.  Note retail does NOT
// store the OR-ed value itself -- SetCaptionButtons does that at its end.)
// Deviation: SetCaptionButtons is called directly rather than through the
// vtable, so a further-derived override is not reached.
extern "C" void MS_ABI impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    unsigned long dwButtons = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(pThis);
        if (st.rollupTimerID != 0) return;
        dwButtons = static_cast<unsigned long>(st.rollupFlags);
    }
    HWND hwnd = HwndOf(pThis);
    const UINT_PTR nNew =
        hwnd ? ::SetTimer(hwnd, 0xec05, impl__m_nRollTimeOut_CPaneFrameWnd__2IA, nullptr) : 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).rollupTimerID = static_cast<int>(nNew);
    }
    impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(pThis, dwButtons | 1UL);
}

// Symbol: ?ShowButton@CPaneFrameWnd@@IEAAXIH@Z
// Retail (0xb16c0), transcribed in full:
//     for (POSITION p = m_lstCaptionButtons.GetHeadPosition() /*0x188*/; p;
//          p = <node pNext at +0>) {
//         CMFCCaptionButton* b = <node data at +0x10>;
//         if (b->m_nHit /*+0x20*/ == uiID) {
//             b->m_bHidden? /*+0x10*/ = bShow;
//             ArrangeCaptionButtons();               // 0xb1990
//             return;
//         }
//     }
// STUB: no caption-button list exists in this object layout.  Note the
// ArrangeCaptionButtons call here is INSIDE the match, unlike RemoveButton's,
// so there is nothing to reproduce unconditionally.
extern "C" void MS_ABI impl__ShowButton_CPaneFrameWnd__IEAAXIH_Z(
    void* pThis, unsigned int uiID, int bShow) {
    (void)pThis;
    (void)uiID;
    (void)bShow;
}

// Symbol: ?SizeToContent@CPaneFrameWnd@@UEAAXXZ
// Retail (0xb0b30), transcribed in full:
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     if (!p || !p->IsKindOf(RUNTIME_CLASS(CPane))) return;
//     CSize size = p-><vtable +0x4d0>(0, TRUE);       // the pane's content size
//     CRect rectWindow; ::GetWindowRect(m_hWnd, &rectWindow);
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     SetWindowPos(NULL, 0, 0,
//                  size.cx + rectWindow.Width()  - rectClient.Width(),
//                  size.cy + rectWindow.Height() - rectClient.Height(),
//                  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE /*0x16*/);
// STUB: the content size comes entirely from an unexported CPane virtual
// (+0x4d0).  Only the non-client margins can be computed here; substituting the
// pane's current window size for its ideal size would resize the frame to
// whatever it already is and defeat the call.
extern "C" void MS_ABI impl__SizeToContent_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    (void)pThis;
}
