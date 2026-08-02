// CPaneFrameWnd message handlers and helpers, wave "other-6".
//
// Every function here was decoded from the retail mfc140u export (ordinal ->
// RVA -> objdump, see /tmp/openmfc_remaining/dis_out/*.txt) rather than
// inferred. The retail member offsets used below (0x140 exit-size-move focus
// hwnd, 0x148 pane hwnd, 0x120/0x124 timer ids, 0x12c roll-up flags, 0x10d
// dragging flag, 0xec idle-active flag) are NOT named in the OpenMFC headers
// (CPaneFrameWnd is modeled as a thin CMiniFrameWnd subclass), so object state
// is kept in a private side table - the same pattern cbarcore.cpp uses for the
// sibling CPaneFrameWnd exports. Only CWnd members declared in afxwin.h are
// accessed directly (m_hWnd @ 64 and the inline wrapper methods).
//
// Where a retail body depends on internal virtual slots (drag-frame impl,
// docking-manager / caption-button internals) or unexported globals, a
// type-correct conservative version is emitted (safe defaults, no state
// corruption) with a `TODO(clean-room)` note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Side-table state for CPaneFrameWnd instances (retail members not modeled in
// include/openmfc/afxmfc.h). This mirrors the g_framePanes pattern from
// cbarcore.cpp; the two tables are independent, so entries default to the safe
// "nothing active" values below.
// ---------------------------------------------------------------------------
namespace {

struct PaneFrameState {
    HWND exitSizeMoveHwnd = nullptr;  // retail 0x140: hwnd to re-focus after sizing
    HWND paneHwnd = nullptr;          // retail 0x148: hwnd of the hosted pane
    int  dockTimerID = 0;             // retail 0x120
    int  rollupTimerID = 0;           // retail 0x124
    int  rollupFlags = 0;             // retail 0x12c
    int  idleActive = 0;              // retail 0xec (bool-ish)
    bool dragging = false;            // retail 0x10d (byte)
};

static std::mutex g_other6Mutex;
static std::unordered_map<void*, PaneFrameState> g_other6State;

static CWnd* AsCwnd(void* pThis) {
    return reinterpret_cast<CWnd*>(pThis);
}

static HWND HwndOf(void* pThis) {
    CWnd* w = AsCwnd(pThis);
    return w ? w->GetSafeHwnd() : nullptr;
}

static PaneFrameState& GetState(void* pThis) {
    return g_other6State[pThis];
}

static PaneFrameState* FindStatePtr(const void* pThis) {
    auto it = g_other6State.find(const_cast<void*>(pThis));
    return (it != g_other6State.end()) ? &it->second : nullptr;
}

// WM_NCACTIVATE / WM_IDLEUPDATECMDUI
#ifndef WM_NCACTIVATE
#define WM_NCACTIVATE 0x0086
#endif
#ifndef WM_IDLEUPDATECMDUI
#define WM_IDLEUPDATECMDUI 0x0363
#endif

} // namespace

// ---------------------------------------------------------------------------
// Sibling impl_ exports (defined in other phase4/src/*.cpp)
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void   MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" CWnd*  MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd*  MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int    MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                        const CObject* pThis, const CRuntimeClass* pClass);
extern "C" int    MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
                        CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd,
                        unsigned int nFlags);
extern "C" void   MS_ABI impl__MoveDragFrame_CMFCDragFrameImpl__QEAAXH_Z(
                        void* pThis, int bMoveToFront);

// Global customize-mode flag (CMFCToolBar::m_bCustomizeMode) + RTTI getter.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();

// ---------------------------------------------------------------------------
// Exports
// ---------------------------------------------------------------------------

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
