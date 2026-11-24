// CPaneFrameWnd — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-6_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MultiPaneFrameSupport.h"
#include "detail/PaneContainerLayout.h"
#include "detail/CbarcoreSupport.h"
#include "detail/CPaneFrameWndSupport.h"
#include "detail/DragFrameSupport.h"
#include "detail/MfccoreSupport.h"
#include "detail/Pane16MsgmapSupport.h"

#include <cstdlib>
#include <new>
#include <windowsx.h>

static_assert(sizeof(CPaneFrameWnd) == 0x258);

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__AddMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(
    CDockingManager*, void*);
extern "C" int MS_ABI impl__RemoveMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(
    CDockingManager*, void*);
extern "C" void MS_ABI impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString*, void*, const wchar_t*);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[];
extern "C" unsigned int impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA;
extern "C" int MS_ABI impl__GetTotalRefCount_CPaneContainerManager__QEBAHXZ(const void*);
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(const CBasePane*, int);
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    CDockingManager*, CWnd*, CPoint, CRect*, BOOL*, CDockablePane**);
extern "C" void MS_ABI impl__EnterDragMode_CPane__MEAAXH_Z(CPane*, int);
extern "C" void MS_ABI impl__SetDragMode_CPane__MEAAXH_Z(CPane*, int);
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager*, CWnd*);
extern "C" int MS_ABI impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(const CBasePane*);
extern "C" void* MS_ABI impl__DockPaneStandard_CMultiPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(void*, int*);
extern "C" void MS_ABI impl__OnDockToRecentPos_CPaneFrameWnd__UEAAXXZ(void*);
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(void*, int);

// ---------------------------------------------------------------------------
// Retail decode notes for the handlers added in this file.
//
// Older unqualified retail addresses in this file refer to mfc140.dll (ANSI
// twin). Image-qualified citations updated here distinguish mfc140.dll from
// mfc140u.dll; Unicode RVAs are used only where disas.py --u resolved them.
// Function bodies are byte-identical across the images, but export RVAs differ.
// Function-entry and instruction-level addresses are identified in context.
// A USER32 import resolving to SendMessageA in mfc140.dll corresponds to
// SendMessageW in mfc140u; code writes ::SendMessage and lets UNICODE pick. An
// indirect `call *0x1802c5bd0` in an ANSI retail listing is the CFG guard
// dispatch (__guard_dispatch_icall_fptr), i.e. an ordinary indirect call
// through RAX -- it is not a USER32 import.
//
// CPaneFrameWnd reserves the retail member region. Its handlers use the
// state owned by CPaneFrameWndSupport for fields in that region.

// Sibling exports defined later in this translation unit or in other phase4
// translation units.  Declared here so the handlers below can call the thunk
// instead of the C++ method (which has no internal definition to link to).
extern "C" void* MS_ABI impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
    CBasePane* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int flags, void* pParams);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__SaveState_CDockablePaneAdapter__UEAAHPEB_WHI_Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" int MS_ABI impl__IsDocked_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" int MS_ABI impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(
    const CDockablePane* pThis);
extern "C" int MS_ABI impl__CanAcceptPane_CMFCOutlookBar__UEBAHPEBVCBasePane___Z(
    const void* pThis, const void* pPane);
extern "C" int MS_ABI impl__SaveState_CBasePane__UEAAHPEB_WHI_Z(
    CBasePane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" int MS_ABI impl__SaveState_CPane__UEAAHPEB_WHI_Z(
    CPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" int MS_ABI impl__SaveState_CMFCToolBar__UEAAHPEB_WHI_Z(
    CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID);
extern "C" std::int32_t impl__m_bHandleMinSize_CPane__2HA;
extern "C" void MS_ABI impl__SizeToContent_CPaneFrameWnd__UEAAXXZ(void* pThis);
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
extern "C" void  MS_ABI impl__OnPressButtons_CPaneFrameWnd__MEAAXI_Z(void*, UINT);
extern "C" void  MS_ABI impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ(void*);
extern "C" void  MS_ABI impl__RedrawCaptionButton_CPaneFrameWnd__IEAAXPEAVCMFCCaptionButton___Z(
    void* pThis, void* pButton);

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
// The global CGlobalUtils instance (retail mfc140.dll: object at .data VA 0x1803aacf8).
// CGlobalUtils::AdjustRectToWorkArea reads no members, so passing the exported
// storage is safe even though OpenMFC never runs a constructor over it.
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[];

namespace {
constexpr UINT_PTR dockingTimer = 0xec04;
constexpr UINT_PTR rollupTimer = 0xec05;
constexpr DWORD autoRollupStyle = 0x10; // AFX_CBRS_AUTO_ROLLUP

BOOL CursorInPaneFrame(HWND window, POINT& cursor) {
    RECT bounds{};
    return window && ::GetCursorPos(&cursor) && ::GetWindowRect(window, &bounds) &&
        ::PtInRect(&bounds, cursor);
}

// Registry behind CPaneFrameWnd::m_mapFloatingBars. Retail mfc140.dll keeps a
// CMap<UINT,UINT,HWND__*,HWND__*> in static storage at .data VA 0x1803aad90 and
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

CMFCCaptionButton* FindCaptionButton(const PaneFrameState& state, UINT id) {
    for (const auto& button : state.captionButtons)
        if (button->m_nHit == id) return button.get();
    return nullptr;
}

CMFCCaptionButton* HitCaptionButton(const void* frame, CPoint screen) {
    const HWND window = HwndOf(const_cast<void*>(frame));
    RECT bounds{};
    if ((::GetWindowLongPtrW(window, GWL_STYLE) & WS_CAPTION) != WS_CAPTION ||
        !::GetWindowRect(window, &bounds)) return nullptr;
    CPoint point(screen.x - bounds.left, screen.y - bounds.top);
    if (::GetWindowLongPtrW(window, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
        point.x = bounds.right - bounds.left - 1 - point.x;
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    if (const auto* state = FindStatePtr(frame)) {
        for (const auto& button : state->captionButtons)
            if (button->GetRect().PtInRect(point)) return button.get();
    }
    return nullptr;
}

CPoint CaptionMessagePoint(LPARAM position) {
    return CPoint(GET_X_LPARAM(position), GET_Y_LPARAM(position));
}

void TrackCaptionButton(void* frame, UINT id) {
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto* state = FindStatePtr(frame);
        if (!state || state->dragging || state->hotButtonID == id) return;
        if (auto* previous = FindCaptionButton(*state, state->hotButtonID))
            previous->m_bFocused = FALSE;
        state->hotButtonID = id;
        if (auto* current = FindCaptionButton(*state, id)) current->m_bFocused = TRUE;
        if (auto* pressed = FindCaptionButton(*state, state->hitButtonID))
            pressed->m_bPushed = pressed->m_nHit == id;
    }
    ::SendMessageW(HwndOf(frame), WM_NCPAINT, 0, 0);
}
}  // namespace


// Symbol: ?Create@CPaneFrameWnd@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CPaneFrameWnd__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
    void* pThis, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, CCreateContext* pContext) {
    return pThis && static_cast<CPaneFrameWnd*>(pThis)->CreatePopup(
        0, lpszClassName, dwStyle, rect, pParentWnd, pContext);
}
// Symbol: ?AddPane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
// Retail mfc140.dll (ANSI twin) entry RVA 0xaf330 stores pPane->m_hWnd at
// CPaneFrameWnd +0x148 before adding the pane to its list. Keep the equivalent
// side-table handle in sync with the pane-pointer list used by GetPane().
extern "C" void MS_ABI impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    if (!pThis || !pPane) return;
    auto* pane = static_cast<CBasePane*>(pPane);
    const HWND paneHwnd = pane->GetSafeHwnd();
    bool first = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto& state = GetState(pThis);
        first = !state.paneHwnd && !state.captionButtonsConfigured;
        state.paneHwnd = paneHwnd;
    }
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        PaneState& state = g_framePanes[pThis];
        AddUniquePane(state, pPane);
        state.rollUp = FALSE;
    }
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pPane, TRUE);
    if (first && paneHwnd && pane->CanBeClosed() &&
        (::GetWindowLongPtrW(HwndOf(pThis), GWL_STYLE) & WS_SYSMENU))
        impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(pThis, 2);
    if (first) impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(pThis);
}
// Symbol: ?RemovePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
// Retail mfc140.dll (ANSI twin) entry RVA 0xaf680 clears +0x148 only when it
// still matches the pane being removed.
extern "C" void MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(
    void* pThis, void* pPane, int destroy, int delay) {
    if (!pThis || !pPane) return;
    bool empty = false;
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        auto it = g_framePanes.find(pThis);
        if (it == g_framePanes.end() ||
            std::find(it->second.panes.begin(), it->second.panes.end(), pPane) == it->second.panes.end()) return;
        RemovePane(it->second, pPane);
        empty = it->second.panes.empty();
        if (empty) it->second.rollUp = FALSE;
    }
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(pPane, FALSE);
    HWND paneHwnd = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(pThis)) {
            state->noDelayedDestroy = delay != 0;
            if (empty) paneHwnd = std::exchange(state->paneHwnd, nullptr);
        }
    }
    if (paneHwnd) {
        std::lock_guard<std::mutex> lock(FloatingPaneMutex());
        auto& panes = FloatingPaneMap();
        for (auto it = panes.begin(); it != panes.end();) {
            if (it->second == paneHwnd) it = panes.erase(it);
            else ++it;
        }
    }
    const HWND hwnd = HwndOf(pThis);
    if (empty && hwnd) {
        if (destroy) ::DestroyWindow(hwnd);
        else ::PostMessageW(hwnd, impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, 0, 0);
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
extern "C" void MS_ABI impl__GetCaptionRect_CPaneFrameWnd__UEBAXAEAVCRect___Z(void* pThis, CRect* rect) {
    if (!rect) return;
    WINDOWINFO info{sizeof(info)};
    const HWND window = pThis ? HwndOf(pThis) : nullptr;
    if (!::GetWindowInfo(window, &info) || (info.dwStyle & WS_CAPTION) != WS_CAPTION) {
        rect->SetRectEmpty();
        return;
    }
    rect->SetRect(info.rcClient.left - info.rcWindow.left, info.cyWindowBorders,
                  info.rcClient.right - info.rcWindow.left, info.rcClient.top - info.rcWindow.top);
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
    const auto entry = g_framePanes.find(pThis);
    if (entry != g_framePanes.end()) {
        for (auto* pane : entry->second.panes) {
            auto* window = static_cast<CWnd*>(pane);
            if (window && ::IsWindowVisible(window->m_hWnd)) return window;
        }
    }
    return nullptr;
}
// Symbol: ?GetPane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const auto it = g_framePanes.find(pThis);
    return it != g_framePanes.end() ? FirstPane(it->second) : nullptr;
}
// Symbol: ?IsRollDown@CPaneFrameWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsRollDown_CPaneFrameWnd__UEBAHXZ(void* pThis) {
    const HWND window = HwndOf(pThis);
    POINT cursor{};
    if (!CursorInPaneFrame(window, cursor)) return FALSE;
    const HWND hit = ::WindowFromPoint(cursor);
    if (hit == window) return TRUE;
    if (hit != ::GetDesktopWindow() || !::IsWindowVisible(window) || !::IsWindowEnabled(window)) return FALSE;

    // A visible owned popup can be omitted by the desktop's point lookup.
    // Check its sibling order before treating the frame as exposed at the cursor.
    for (HWND above = ::GetWindow(window, GW_HWNDPREV); above; above = ::GetWindow(above, GW_HWNDPREV)) {
        RECT bounds{};
        if (::IsWindowVisible(above) && ::GetWindowRect(above, &bounds) && ::PtInRect(&bounds, cursor))
            return FALSE;
    }
    return TRUE;
}
// Symbol: ?IsRollUp@CPaneFrameWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsRollUp_CPaneFrameWnd__UEBAHXZ(void* pThis) {
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto* state = FindStatePtr(pThis);
        if (!state || state->pinned) return FALSE;
    }
    POINT cursor{};
    const HWND window = HwndOf(pThis);
    RECT bounds{};
    return window && ::GetCursorPos(&cursor) && ::GetWindowRect(window, &bounds) &&
        !::PtInRect(&bounds, cursor);
}
// Symbol: ?StartTearOff@CPaneFrameWnd@@QEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__StartTearOff_CPaneFrameWnd__QEAAHPEAVCMFCPopupMenu___Z(void* pThis, void* pMenu) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_framePanes[pThis], pMenu);
    return pThis != nullptr;
}
// Symbol: ?StopCaptionButtonsTracking@CPaneFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (!pThis) return;
    const HWND window = HwndOf(pThis);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto* state = FindStatePtr(pThis);
        if (!state || (!state->hitButtonID && !state->hotButtonID)) return;
        if (auto* button = FindCaptionButton(*state, std::exchange(state->hitButtonID, 0u)))
            button->m_bPushed = FALSE;
        if (auto* button = FindCaptionButton(*state, std::exchange(state->hotButtonID, 0u)))
            button->m_bFocused = FALSE;
    }
    if (::GetCapture() == window) ::ReleaseCapture();
    ::SendMessageW(window, WM_NCPAINT, 0, 0);
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
// Retail mfc140.dll entry RVA 0xb1870 returns if m_pToolTip (+0x1b8) or its
// HWND is null; otherwise it gets the frame's client rect, walks the caption
// buttons, queries TTM_GETTOOLCOUNT, and updates tool rectangles. The SendMessageA
// import slot 0x1802c5378 is USER32!SendMessageA (UNICODE twin: SendMessageW).
// STUB: side slots now exist for a tooltip pointer, semantic button rectangles,
// and tooltip records, but this code has no initialized native tooltip control
// or tool-registration lifecycle to update. The body intentionally touches
// nothing, matching the retail null-tooltip early return when no control exists.
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
// CDockablePane resize handles). Semantic button records now exist, but this
// partial hit test still lacks their native geometry and the pane virtuals; only
// its window/caption-region approximation is currently transcribed.
// TODO(clean-room): transcribed partially.
extern "C" __int64 MS_ABI impl__HitTest_CPaneFrameWnd__UEAA_JVCPoint__H_Z(
    void* pThis, CPoint point, int nIndex) {
    if (pThis == nullptr) return 1;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return 1;
    if (const auto* button = HitCaptionButton(pThis, point)) return button->m_nHit;

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
//          !pane->IsKindOf(RUNTIME_CLASS(CMFCToolBar))). The pane HWND at
// retail +0x148 is represented by PaneFrameState::paneHwnd; it is absent until
// a frame's AddPane path populates that side-table slot.
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
// .rdata VA 0x1802f5ad8 (mfc140.dll) shows 0x328 is ?SetCaptionButtons@CPaneFrameWnd@@UEAAXK@Z
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
namespace {
CMFCDragFrameImpl* FrameDrag(CPaneFrameWnd& frame) {
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    auto* state = FindStatePtr(&frame);
    return state ? &state->dragFrame : nullptr;
}
}

// Symbol: ?MoveDragFrame@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__MoveDragFrame_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis) {
        if (auto* drag = FrameDrag(*static_cast<CPaneFrameWnd*>(pThis)))
            openmfc::detail::UpdateDockDragPreview(*drag, true, true);
    }
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
extern "C" int MS_ABI impl__OnBeforeDock_CPaneFrameWnd__UEAAHXZ(void* pThis) {
    if (!pThis || ::GetKeyState(VK_CONTROL) < 0) return FALSE;
    auto* frame = static_cast<CPaneFrameWnd*>(pThis);
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(frame);
    if (!manager) return TRUE;
    POINT point{};
    if (!::GetCursorPos(&point)) return FALSE;
    CRect preview;
    BOOL drawTab = FALSE;
    impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
        manager, frame, CPoint(point), &preview, &drawTab, nullptr);
    return drawTab || !preview.IsRectEmpty();
}
// Symbol: ?OnCancelMode@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis) static_cast<CPaneFrameWnd*>(pThis)->OnCancelPaneDrag(0, 0);
}
// Symbol: ?OnCapture@CPaneFrameWnd@@MEAAXH@Z
// mfc140u 0xb20e0 forwards capture to CPane::SetDragMode and closes deferred windows on release.
extern "C" void MS_ABI impl__OnCapture_CPaneFrameWnd__MEAAXH_Z(
    void* pThis, int capture) {
    if (!pThis) return;
    HWND paneWindow = nullptr;
    HWND frameHelper = nullptr;
    const HWND frameWindow = HwndOf(pThis);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto* state = FindStatePtr(pThis);
        if (!state) return;
        state->paneCaptureActive = capture != FALSE;
        paneWindow = state->paneHwnd;
        if (!capture) frameHelper = std::exchange(state->destroyOnReleaseHwnd, nullptr);
    }
    HWND paneHelper = nullptr;
    auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(paneWindow));
    if (window && window->IsKindOf(RUNTIME_CLASS(CPane))) {
        auto* pane = static_cast<CPane*>(window);
        impl__SetDragMode_CPane__MEAAXH_Z(pane, capture);
        if (!capture) paneHelper = std::exchange(pane->m_hwndMiniFrameToBeClosed, nullptr);
    }
    if (::IsWindow(frameHelper)) ::DestroyWindow(frameHelper);
    if (paneHelper != frameWindow && paneHelper != frameHelper && ::IsWindow(paneHelper))
        ::DestroyWindow(paneHelper);
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
extern "C" __int64 MS_ABI impl__OnCheckEmptyState_CPaneFrameWnd__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    return pThis ? static_cast<CPaneFrameWnd*>(pThis)->OnCheckEmptyState(wParam, lParam) : 0;
}
// Symbol: ?OnCheckRollState@CPaneFrameWnd@@MEAAXXZ
// mfc140u 0xaeae0 saves the expanded height and removes only the client height.
extern "C" void MS_ABI impl__OnCheckRollState_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    auto* frame = static_cast<CPaneFrameWnd*>(pThis);
    if (!frame || !frame->m_hWnd) return;
    CGlobalUtils utils;
    const auto* docking = FindDockingState(utils.GetDockingManager(frame));
    if (docking && docking->lockUpdate) return;
    bool rolledUp = false;
    UINT expandedHeight = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto* state = FindStatePtr(frame);
        if (!state) return;
        rolledUp = state->rolledUp;
        expandedHeight = state->heightBeforeRollUp;
    }
    RECT bounds{}, client{};
    if (!::GetWindowRect(frame->m_hWnd, &bounds) || !::GetClientRect(frame->m_hWnd, &client)) return;
    const bool expand = rolledUp && frame->IsRollDown();
    if (!expand && (rolledUp || !frame->IsRollUp())) return;
    const LONG height = bounds.bottom - bounds.top;
    const LONG target = expand ? static_cast<LONG>(expandedHeight) : height - (client.bottom - client.top);
    if (target <= 0) return;
    const HWND window = frame->m_hWnd;
    if (::SetWindowPos(window, nullptr, 0, 0, bounds.right - bounds.left, target,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE)) {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(frame); state && state->frameWindow == window) {
            if (!expand) state->heightBeforeRollUp = height;
            state->rolledUp = !expand;
        }
    }
}
// Symbol: ?OnClose@CPaneFrameWnd@@IEAAXXZ
// Retail (0xb3d30) is three instructions: a tail jump through this object's
// vtable slot 0x458.  An earlier note here said that slot's target was not
// modelled; the mfc140.dll vtable at .rdata VA 0x1802f5ad8 gives 0x458 == entry RVA 0xb1b10 ==
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
extern "C" void MS_ABI impl__OnDestroy_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis) static_cast<CPaneFrameWnd*>(pThis)->OnDestroy();
}
// Symbol: ?OnDockToRecentPos@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnDockToRecentPos_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    auto* frame = static_cast<CPaneFrameWnd*>(pThis);
    auto* pane = frame ? frame->GetPane() : nullptr;
    if (pane && pane->IsKindOf(RUNTIME_CLASS(CPane)))
        static_cast<CPane*>(pane)->DockToRecentPosition();
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
// Draw the caption list in native order. The pin glyph follows the pin state.
extern "C" void MS_ABI impl__OnDrawCaptionButtons_CPaneFrameWnd__MEAAXPEAVCDC___Z(
    void* pThis, CDC* dc) {
    if (!pThis || !dc) return;
    std::vector<CMFCCaptionButton*> buttons;
    const BOOL active = ::GetActiveWindow() == HwndOf(pThis);
    BOOL pinned = FALSE;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(pThis)) {
            for (const auto& button : state->captionButtons) buttons.push_back(button.get());
            pinned = state->pinned;
        }
    }
    for (auto* button : buttons) button->OnDraw(dc, active, TRUE, pinned, FALSE);
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
// Retail hit-tests caption buttons (FindButton + vtable 0x340) and always ends
// in Default(). Semantic button records exist, but native hit-test/action routing
// is not modeled -> Default() only.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnLButtonDblClk_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
// Symbol: ?OnLButtonDown@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
// Retail closes any captured popup, routes the click to a caption button
// (list at +0x188) or starts a drag, and ends in Default(). Semantic records
// exist, but native hit-test/action routing is not modeled; only Default() runs.
// TODO(clean-room): transcribed partially.
extern "C" void MS_ABI impl__OnLButtonDown_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}
// Symbol: ?OnLButtonUp@CPaneFrameWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int flags, CPoint point) {
    if (pThis) static_cast<CPaneFrameWnd*>(pThis)->OnLButtonUp(flags, point);
}
CPaneFrameWnd::CPaneFrameWnd() = default;

void CPaneFrameWnd::DestroyPaneFrameWindow() {
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(this)) state->destroyWithWindow = false;
    }
    if (m_hWnd) DestroyWindow();
}

CPaneFrameWnd::~CPaneFrameWnd() {
    DestroyPaneFrameWindow();
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto it = g_other6State.find(this);
        if (it != g_other6State.end()) {
            g_other6State.erase(it);
        }
    }
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_framePanes.erase(this);
}

const AFX_MSGMAP* AFXAPI CPaneFrameWnd::GetThisMessageMap() {
    return &openmfc::detail::pane16msgmap::classCPaneFrameWnd_msgmap;
}

const AFX_MSGMAP* CPaneFrameWnd::GetMessageMap() const {
    return GetThisMessageMap();
}

BOOL CPaneFrameWnd::CreatePopup(DWORD extendedStyle, const wchar_t* title, DWORD style,
                               const RECT& rect, CWnd* owner, CCreateContext* context) {
    if (m_hWnd || (owner && (!owner->m_hWnd || !owner->IsKindOf(RUNTIME_CLASS(CFrameWnd))))) return FALSE;
    CDockingManager* manager = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        manager = GetState(this).dockManager;
    }
    if (owner && !manager) {
        CGlobalUtils utils;
        manager = utils.GetDockingManager(owner);
        if (!manager) return FALSE;
    }
    if (owner) extendedStyle |= ::GetWindowLongPtrW(owner->m_hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL;

    alignas(CString) unsigned char storage[sizeof(CString)];
    auto* className = reinterpret_cast<CString*>(storage);
    impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
        className, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, L"Afx:PaneFrame");
    const CRect bounds = NormalizeRect(&rect, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    style = (style & ~WS_CHILD) | WS_POPUP | WS_CLIPCHILDREN;
    const BOOL created = CWnd::CreateEx(extendedStyle, className->GetString(), title, style, *static_cast<const RECT*>(bounds), owner, 0, context);
    className->~CString();
    if (!created) return FALSE;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto& state = GetState(this);
        state.frameWindow = m_hWnd;
        state.ownerHwnd = owner ? owner->m_hWnd : nullptr;
        state.dockManager = manager;
        state.recentFloatingRect = *static_cast<const RECT*>(bounds);
    }
    POINT anchor{-1, -1};
    ::GetCursorPos(&anchor);
    openmfc::detail::BeginDockDrag(*FrameDrag(*this), *this, manager, anchor);
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        auto& state = g_framePanes[this];
        state.parent = owner;
        state.recentRect = bounds;
    }
    if (manager) impl__AddMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(manager, this);
    return TRUE;
}

void CPaneFrameWnd::SetDockingManager(CDockingManager* manager) {
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    GetState(this).dockManager = manager;
}

void CPaneFrameWnd::AutoDestroyWithWindow() {
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    GetState(this).destroyWithWindow = true;
}

void CPaneFrameWnd::PostNcDestroy() {
    bool destroy = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto it = g_other6State.find(this);
        if (it != g_other6State.end()) {
            destroy = it->second.destroyWithWindow;
            g_other6State.erase(it);
        }
    }
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        g_framePanes.erase(this);
    }
    CWnd::PostNcDestroy();
    if (destroy) delete this;
}

void CPaneFrameWnd::OnClose() {
    impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(this);
}

void CPaneFrameWnd::OnDestroy() {
    openmfc::detail::ClearPaneFrameDockPreview(*this);
    CDockingManager* manager = nullptr;
    bool dragging = false;
    HWND pane = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(this)) {
            state->frameWindow = nullptr;
            manager = state->dockManager;
            state->dockManager = nullptr;
            dragging = state->dragging;
            state->dragging = false;
            pane = state->paneHwnd;
            if (state->rollupTimerID) ::KillTimer(m_hWnd, state->rollupTimerID);
        }
    }
    if (dragging && ::GetCapture() == m_hWnd) ::ReleaseCapture();
    impl__KillDockingTimer_CPaneFrameWnd__QEAAXXZ(this);
    if (manager && FindDockingState(manager))
        impl__RemoveMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(manager, this);
    if (auto* window = static_cast<CWnd*>(FromHandlePermanent(pane)); window && window->IsKindOf(RUNTIME_CLASS(CBasePane)))
        impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(window, FALSE);
    CWnd::OnDestroy();
}

LRESULT CPaneFrameWnd::OnCheckEmptyState(WPARAM, LPARAM) {
    if (GetPane()) return 0;
    if (IsKindOf(CMultiPaneFrameWnd::GetThisClass()) &&
        impl__GetTotalRefCount_CPaneContainerManager__QEBAHXZ(
            static_cast<CMultiPaneFrameWnd*>(this)->GetContainerManager())) {
        ::ShowWindow(m_hWnd, SW_HIDE);
        return 0;
    }
    bool hide = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(this)) hide = state->noDelayedDestroy;
    }
    if (hide) ::ShowWindow(m_hWnd, SW_HIDE);
    else DestroyWindow();
    return 0;
}

LRESULT CPaneFrameWnd::OnCaptionDoubleClick(WPARAM hit, LPARAM) {
    if (hit != HTCAPTION) return Default();
    impl__OnDockToRecentPos_CPaneFrameWnd__UEAAXXZ(this);
    return 0;
}
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
    return Create(nullptr, dwStyle, createRect, pParentWnd, nullptr);
}
void CPaneFrameWnd::AddPane(CBasePane* pBar) {
    impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(this, pBar);
}
void CPaneFrameWnd::RemovePane(CBasePane* pBar, BOOL bDestroy) {
    impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(this, pBar, bDestroy, FALSE);
}
// Symbol: ?m_mapFloatingBars@CPaneFrameWnd@@1V?$CMap@IIPEAUHWND__@@PEAU1@@@A
// Static data member. Retail places a constructed CMap<UINT,UINT,HWND__*,HWND__*>
// at data RVA 0x3aad90 (mfc140.dll) / 0x3b1d90 (mfc140u.dll): the qword at +0
// is a vtable pointer, and GetPaneList / FrameFromPoint read the element count
// at +0x18. That matches MFC's CMap layout -- CObject vptr, m_pHashTable,
// m_nHashTableSize, m_nCount, m_pFreeList, m_pBlocks, m_nBlockSize (0x38 bytes).
// OpenMFC does not
// construct a CMap here, so this stays zeroed storage; it is sized to 0x38 so
// that a client writing through the export cannot run off the end (the previous
// 8-byte definition could).  The live registry is the private table above.
extern "C" alignas(8) unsigned char impl__m_mapFloatingBars_CPaneFrameWnd__1V__CMap_IIPEAUHWND____PEAU1___A[0x38] = {};

// Symbol: ?AddButton@CPaneFrameWnd@@MEAAXI@Z
// mfc140u 0xb0f50 adds each new hit code at the head of the caption list.
extern "C" void MS_ABI impl__AddButton_CPaneFrameWnd__MEAAXI_Z(void* pThis, UINT id) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    auto& state = GetState(pThis);
    if (FindCaptionButton(state, id)) return;
    state.captionButtonsConfigured = true;
    auto button = std::make_unique<CMFCCaptionButton>(id);
    button->SetMiniFrameButton();
    state.captionButtons.insert(state.captionButtons.begin(), std::move(button));
}

// Symbol: ?AddRemovePaneFromGlobalList@CPaneFrameWnd@@SAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(
    void* pPane, int bAdd) {
    if (pPane == nullptr) return FALSE;
    auto* window = static_cast<CWnd*>(pPane);
    const int id = window->m_hWnd ? ::GetDlgCtrlID(window->m_hWnd) : -1;
    if (id != -1) {
        std::lock_guard<std::mutex> lock(FloatingPaneMutex());
        if (bAdd) FloatingPaneMap()[id] = window->m_hWnd;
        else if (const auto found = FloatingPaneMap().find(id);
                 found != FloatingPaneMap().end() && found->second == window->m_hWnd)
            FloatingPaneMap().erase(found);
        return TRUE;
    }
    if (!window->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) return FALSE;
    auto* tabs = static_cast<CBaseTabbedPane*>(window)->GetTabs();
    if (tabs) {
        for (int i = 0; i < tabs->GetTabsCount(); ++i) {
            auto* page = tabs->GetTabWnd(i);
            if (page && page->IsKindOf(RUNTIME_CLASS(CBasePane)))
                impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(page, bAdd);
        }
    }
    return TRUE;
}

// Symbol: ?ArrangeCaptionButtons@CPaneFrameWnd@@IEAAXXZ
// mfc140u 0xb13d0 lays out the list from tail to head with four pixels between buttons.
extern "C" void MS_ABI impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto* state = FindStatePtr(pThis);
        if (!state || state->captionButtons.empty()) return;
    }
    CRect caption;
    static_cast<CPaneFrameWnd*>(pThis)->GetCaptionRect(caption);
    const CSize size = CMFCCaptionButton::GetSize();
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(pThis)) {
            const LONG y = caption.top + (caption.Height() - size.cy) / 2;
            LONG left = caption.left;
            LONG right = caption.right;
            for (auto it = state->captionButtons.rbegin(); it != state->captionButtons.rend(); ++it) {
                auto& button = **it;
                if (button.m_bHidden) continue;
                if (button.m_bLeftAlign) {
                    button.Move(CPoint(left, y));
                    left += size.cx + 4;
                } else {
                    right = (std::max<LONG>)(caption.left, right - size.cx);
                    button.Move(CPoint(right, y));
                    right -= 4;
                }
            }
        }
    }
    impl__UpdateTooltips_CPaneFrameWnd__IEAAXXZ(pThis);
}

// Symbol: ?CanBeDockedToPane@CPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z
// Retail mfc140.dll entry RVA 0xb3770 resolves the pane HWND at +0x148, keeps it
// only when it is a CPane, then tail-dispatches the passed CDockablePane's
// vtable +0x318 CanAcceptPane. The base CDockablePane body is RVA 0x12d90
// (mfc140.dll): accept only a non-null CDockablePane when the target is docked
// or in a floating multi-pane frame. Those checks have existing typed thunks.
// Deviation: the known CMFCOutlookBar override is dispatched through its thunk,
// which remains a stub; for the base body, IsDocked uses the CBasePane thunk and
// therefore does not reach further-derived overrides.
extern "C" int MS_ABI impl__CanBeDockedToPane_CPaneFrameWnd__UEBAHPEBVCDockablePane___Z(
    const void* pThis, const void* pDockablePane) {
    if (pThis == nullptr || pDockablePane == nullptr) return FALSE;
    HWND paneHwnd = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const PaneFrameState* state = FindStatePtr(pThis);
        if (state != nullptr) paneHwnd = state->paneHwnd;
    }
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(paneHwnd);
    CPane* pPane = nullptr;
    CRuntimeClass* pPaneClass = impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
    if (pWnd != nullptr && pPaneClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, pPaneClass)) {
        pPane = static_cast<CPane*>(pWnd);
    }
    CRuntimeClass* pOutlookClass = impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ();
    if (pOutlookClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(static_cast<const CWnd*>(pDockablePane)), pOutlookClass)) {
        return impl__CanAcceptPane_CMFCOutlookBar__UEBAHPEBVCBasePane___Z(pDockablePane, pPane);
    }
    CRuntimeClass* pDockableClass = impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
    if (pPane == nullptr || pDockableClass == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pDockableClass)) {
        return FALSE;
    }
    const CDockablePane* pTarget = static_cast<const CDockablePane*>(pDockablePane);
    if (impl__IsDocked_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(pTarget))) return TRUE;
    return impl__IsInFloatingMultiPaneFrameWnd_CDockablePane__UEBAHXZ(pTarget) ? TRUE : FALSE;
}

// Symbol: ?CloseMiniFrame@CPaneFrameWnd@@MEAAXXZ
// Retail entry RVA 0xb1550 (mfc140u): call vtable slot +0x450 and return if it
// is false; otherwise hide this window, read m_hwndPane at +0x148, resolve it
// with CWnd::FromHandlePermanent, and hide that window if it is mapped.
// Deviation: OpenMFC calls the OnCloseMiniFrame thunk and GetPane thunk
// directly, rather than the retail virtual dispatch and permanent lookup. The
// latter uses this file's pane side table, not a retail-layout field. A null
// pThis guard is also added; retail dereferences this unconditionally.
extern "C" void MS_ABI impl__CloseMiniFrame_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (!impl__OnCloseMiniFrame_CPaneFrameWnd__MEAAHXZ(pThis)) return;
    auto* frame = static_cast<CPaneFrameWnd*>(pThis);
    auto* pane = frame->GetPane();
    CDockingManager* manager = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(pThis)) manager = state->dockManager;
    }
    if (manager && pane && pane->IsKindOf(RUNTIME_CLASS(CBasePane)))
        manager->HidePane(static_cast<CBasePane*>(pane));
    if (pane && pane->m_hWnd) ::ShowWindow(pane->m_hWnd, SW_HIDE);
    if (frame->m_hWnd) ::ShowWindow(frame->m_hWnd, SW_HIDE);
}

// Symbol: ?ConvertToTabbedDocument@CPaneFrameWnd@@UEAAXXZ
// Retail (0xae120):
//     CWnd* p = GetFirstVisiblePane();          // vtable 0x368
//     if (p && p->IsKindOf(RUNTIME_CLASS(CDockablePane))) {
//         p->vtable[0x5d0](FALSE);              // unexported CDockablePane virtual
//         ::PostMessage(m_hWnd, AFX_WM_CHECKEMPTYMINIFRAME, 0, 0);
//     }
// The .data slot at VA 0x1803bae78 (mfc140.dll) is ?AFX_WM_CHECKEMPTYMINIFRAME@@3IA (zero in
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
extern "C" int MS_ABI impl__CreateEx_CPaneFrameWnd__UEAAHKPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
    void* pThis, unsigned long extendedStyle, const wchar_t* title, unsigned long style,
    const RECT& rect, CWnd* owner, CCreateContext* context) {
    return pThis && static_cast<CPaneFrameWnd*>(pThis)->CreatePopup(extendedStyle, title, style, rect, owner, context);
}

namespace {
CDockablePane* DropFramePane(CPaneFrameWnd* frame, BOOL& docked, int method = 1) {
    docked = FALSE;
    auto* pane = frame ? frame->GetPane() : nullptr;
    if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CDockablePane)) || ::GetKeyState(VK_CONTROL) < 0)
        return nullptr;
    POINT point{};
    if (!::GetCursorPos(&point)) return nullptr;
    openmfc::detail::ClearPaneFrameDockPreview(*frame);
    if (frame->IsKindOf(CMultiPaneFrameWnd::GetThisClass())) {
        auto* multi = static_cast<CMultiPaneFrameWnd*>(frame);
        if (openmfc::detail::ContainerPanes(multi->GetContainerManager()).size() > 1)
            return openmfc::detail::DockMultiPaneFrameAtPoint(*multi, CPoint(point), docked, method);
    }
    return DockPaneAtPoint(*static_cast<CDockablePane*>(pane), point, docked, method);
}
}

// Symbol: ?DockPane@CPaneFrameWnd@@UEAAPEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPane_CPaneFrameWnd__UEAAPEAVCDockablePane__AEAH_Z(void* frame, int* docked) {
    return docked ? DropFramePane(static_cast<CPaneFrameWnd*>(frame), *docked) : nullptr;
}

// Symbol: ?DockPaneStandard@CPaneFrameWnd@@MEAAPEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPaneStandard_CPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(void* frame, int* docked) {
    if (!docked) return nullptr;
    *docked = FALSE;
    return impl__OnBeforeDock_CPaneFrameWnd__UEAAHXZ(frame)
        ? DropFramePane(static_cast<CPaneFrameWnd*>(frame), *docked, 5 /* DM_STANDARD */) : nullptr;
}

bool openmfc::detail::EndPaneFrameDockDrag(CPaneFrameWnd& frame) {
    bool dragging = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(&frame)) {
            dragging = std::exchange(state->dragging, false);
            state->systemMoveStarted = state->movedBySystem = false;
        }
    }
    ClearPaneFrameDockPreview(frame);
    if (dragging && ::GetCapture() == frame.m_hWnd) ::ReleaseCapture();
    return dragging;
}

void openmfc::detail::ClearPaneFrameDockPreview(CPaneFrameWnd& frame) {
    if (auto* drag = FrameDrag(frame)) impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(drag, TRUE);
}

namespace {
bool UsesStandardDocking(CPaneFrameWnd& frame) {
    auto* window = frame.GetFirstVisiblePane();
    return window && window->IsKindOf(RUNTIME_CLASS(CBasePane)) &&
        (impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(static_cast<CBasePane*>(window)) & 2 /* DT_STANDARD */);
}

bool FinishPaneFrameMove(CPaneFrameWnd& frame) {
    BOOL docked = FALSE;
    if (UsesStandardDocking(frame)) {
        const HWND popup = frame.m_hWnd;
        const auto* first = frame.GetFirstVisiblePane();
        if (frame.IsKindOf(CMultiPaneFrameWnd::GetThisClass()))
            impl__DockPaneStandard_CMultiPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(&frame, &docked);
        else
            impl__DockPaneStandard_CPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(&frame, &docked);
        return docked || CWnd::FromHandlePermanent(popup) != &frame || frame.GetFirstVisiblePane() != first;
    } else {
        DropFramePane(&frame, docked);
        return docked != FALSE;
    }
}
}

// Symbol: ?EnterDragMode@CPaneFrameWnd@@MEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__EnterDragMode_CPaneFrameWnd__MEAAXPEAUHWND_____Z(void* frame, HWND) {
    if (frame) static_cast<CPaneFrameWnd*>(frame)->BeginPaneDrag();
}

void CPaneFrameWnd::BeginPaneDrag() {
    RECT bounds{};
    POINT point{};
    if (!::GetWindowRect(m_hWnd, &bounds) || !::GetCursorPos(&point)) return;
    const bool outlineOnly = UsesStandardDocking(*this);
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(this);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto& state = GetState(this);
        if (state.dragging) return;
        state.dragging = true;
        state.dragOutlineOnly = outlineOnly;
        state.dragStartBounds = bounds;
        state.dragStartPoint = point;
    }
    openmfc::detail::BeginDockDrag(*FrameDrag(*this), *this, manager, point);
    const HWND window = m_hWnd;
    ::SetCapture(window);
    if (CWnd::FromHandlePermanent(window) != this) return;
    if (::GetCapture() != window) {
        openmfc::detail::EndPaneFrameDockDrag(*this);
        return;
    }
    impl__OnCapture_CPaneFrameWnd__MEAAXH_Z(this, TRUE);
    ::SetCursor(::LoadCursorW(nullptr, IDC_SIZEALL));
}

void CPaneFrameWnd::OnMouseMove(UINT, CPoint clientPoint) {
    const HWND frame = m_hWnd;
    if (::GetCapture() != frame) {
        Default();
        return;
    }
    RECT original{};
    POINT start{};
    bool outlineOnly = false;
    bool dragging = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto* state = FindStatePtr(this);
        if (!state) return;
        dragging = state->dragging;
        original = state->dragStartBounds;
        start = state->dragStartPoint;
        outlineOnly = state->dragOutlineOnly;
    }
    if (!dragging) {
        POINT screen{clientPoint.x, clientPoint.y};
        ::ClientToScreen(frame, &screen);
        impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(this, CPoint(screen));
        return;
    }
    POINT point{};
    if (!::GetCursorPos(&point)) return;
    if (!outlineOnly)
        ::SetWindowPos(frame, nullptr, original.left + point.x - start.x, original.top + point.y - start.y,
            0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    if (auto* drag = FrameDrag(*this)) openmfc::detail::UpdateDockDragPreview(*drag, outlineOnly, false);
}

void CPaneFrameWnd::OnLButtonUp(UINT, CPoint clientPoint) {
    bool dragging = false;
    bool outlineOnly = false;
    UINT pressed = 0;
    RECT startingBounds{};
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(this)) {
            dragging = std::exchange(state->dragging, false);
            pressed = state->hitButtonID;
            startingBounds = state->dragStartBounds;
            outlineOnly = state->dragOutlineOnly;
        }
    }
    if (!dragging) {
        if (pressed) {
            const HWND window = m_hWnd;
            POINT point{clientPoint.x, clientPoint.y};
            ::ClientToScreen(window, &point);
            const auto* button = HitCaptionButton(this, CPoint(point));
            const bool activate = button && button->m_bEnabled && button->m_nHit == pressed;
            impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(this);
            if (!activate || CWnd::FromHandlePermanent(window) != this) return;
            if (pressed == HTOBJECT || pressed == HTCLOSE) {
                ::SendMessageW(window, WM_CLOSE, 0, 0);
            } else if (pressed == 9) {
                {
                    std::lock_guard<std::mutex> lock(g_other6Mutex);
                    auto& state = GetState(this);
                    state.pinned = !state.pinned;
                }
                ::SendMessageW(window, WM_NCPAINT, 0, 0);
            } else {
                impl__OnPressButtons_CPaneFrameWnd__MEAAXI_Z(this, pressed);
            }
            return;
        }
        Default();
        return;
    }
    const HWND popup = m_hWnd;
    RECT outline{};
    auto* drag = FrameDrag(*this);
    const bool hasOutline = outlineOnly && drag && openmfc::detail::DockDragBounds(*drag, outline);
    openmfc::detail::ClearPaneFrameDockPreview(*this);
    if (::GetCapture() == popup) ::ReleaseCapture();
    if (CWnd::FromHandlePermanent(popup) != this) return;
    if (outlineOnly) {
        if (hasOutline && !FinishPaneFrameMove(*this) && CWnd::FromHandlePermanent(popup) == this)
            ::SetWindowPos(popup, nullptr, outline.left, outline.top, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
        return;
    }
    RECT current{};
    if (::GetWindowRect(m_hWnd, &current) && !::EqualRect(&current, &startingBounds))
        FinishPaneFrameMove(*this);
}

LRESULT CPaneFrameWnd::OnCaptionButtonDown(WPARAM hit, LPARAM position) {
    if (auto* button = HitCaptionButton(this, CaptionMessagePoint(position))) {
        if (button->m_nHit == hit) {
            if (!button->m_bEnabled) return 0;
            const UINT id = button->m_nHit;
            {
                std::lock_guard<std::mutex> lock(g_other6Mutex);
                auto& state = GetState(this);
                state.hitButtonID = id;
                button->m_bPushed = TRUE;
            }
            ::SetCapture(m_hWnd);
            TrackCaptionButton(this, id);
            ::SendMessageW(m_hWnd, WM_NCPAINT, 0, 0);
            return 0;
        }
    }
    if (hit != HTCAPTION || !GetPane()) return Default();
    ::SetActiveWindow(m_hWnd);
    BeginPaneDrag();
    return 0;
}

LRESULT CPaneFrameWnd::OnCaptionHitTest(WPARAM, LPARAM position) {
    if (const auto* button = HitCaptionButton(this, CaptionMessagePoint(position))) return button->m_nHit;
    const LRESULT result = Default();
    if (result == HTCLOSE) {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(this); state && state->captionButtonsConfigured) return HTCAPTION;
    }
    return result;
}

LRESULT CPaneFrameWnd::OnCaptionMouseMove(WPARAM, LPARAM position) {
    const auto* button = HitCaptionButton(this, CaptionMessagePoint(position));
    TrackCaptionButton(this, button && button->m_bEnabled ? button->m_nHit : 0);
    TRACKMOUSEEVENT tracking{sizeof(tracking), TME_NONCLIENT | TME_LEAVE, m_hWnd, 0};
    ::TrackMouseEvent(&tracking);
    return button ? 0 : Default();
}

LRESULT CPaneFrameWnd::OnCaptionMouseLeave(WPARAM, LPARAM) {
    TrackCaptionButton(this, 0);
    return Default();
}

LRESULT CPaneFrameWnd::OnCaptionPaint(WPARAM, LPARAM paint) {
    bool partial = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(this)) partial = !::IsRectEmpty(&state->redrawRect);
    }
    const LRESULT result = partial ? 0 : Default();
    if (paint != -1) impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ(this);
    return result;
}

LRESULT CPaneFrameWnd::OnPaneDragKey(WPARAM key, LPARAM) {
    if (key == VK_ESCAPE && ::GetCapture() == m_hWnd) return OnCancelPaneDrag(0, 0);
    if (key == VK_CONTROL && ::GetCapture() == m_hWnd) {
        bool outlineOnly = false;
        {
            std::lock_guard<std::mutex> lock(g_other6Mutex);
            if (const auto* state = FindStatePtr(this)) outlineOnly = state->dragOutlineOnly;
        }
        if (auto* drag = FrameDrag(*this)) openmfc::detail::UpdateDockDragPreview(*drag, outlineOnly, true);
    }
    return Default();
}

LRESULT CPaneFrameWnd::OnEnterSizeMove(WPARAM, LPARAM) {
    RECT bounds{};
    ::GetWindowRect(m_hWnd, &bounds);
    POINT anchor{-1, -1};
    ::GetCursorPos(&anchor);
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(this);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto& state = GetState(this);
        state.systemMoveStarted = true;
        state.movedBySystem = false;
        state.dragStartBounds = bounds;
    }
    openmfc::detail::BeginDockDrag(*FrameDrag(*this), *this, manager, anchor);
    return Default();
}

void CPaneFrameWnd::OnMoving(UINT, RECT*) {
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto* state = FindStatePtr(this);
        if (!state || !state->systemMoveStarted) return;
        state->movedBySystem = true;
    }
    if (auto* drag = FrameDrag(*this)) openmfc::detail::UpdateDockDragPreview(*drag, false, true);
}

LRESULT CPaneFrameWnd::OnExitSizeMove(WPARAM, LPARAM) {
    const LRESULT result = Default();
    RECT original{};
    bool moved = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(this)) {
            original = state->dragStartBounds;
            state->systemMoveStarted = false;
            moved = std::exchange(state->movedBySystem, false);
        }
    }
    openmfc::detail::ClearPaneFrameDockPreview(*this);
    RECT bounds{};
    // Windows restores the starting rectangle when Escape cancels a move.
    if (moved && ::GetWindowRect(m_hWnd, &bounds) && !::EqualRect(&bounds, &original))
        FinishPaneFrameMove(*this);
    return result;
}

LRESULT CPaneFrameWnd::OnCaptureChanged(WPARAM, LPARAM) {
    bool lostDrag = false;
    bool paneCapture = false;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(this)) {
            lostDrag = std::exchange(state->dragging, false);
            paneCapture = state->paneCaptureActive;
        }
    }
    if (lostDrag) openmfc::detail::ClearPaneFrameDockPreview(*this);
    impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(this);
    const LRESULT result = Default();
    if (paneCapture) impl__OnCapture_CPaneFrameWnd__MEAAXH_Z(this, FALSE);
    return result;
}

LRESULT CPaneFrameWnd::OnCancelPaneDrag(WPARAM, LPARAM) {
    const HWND window = m_hWnd;
    impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(this);
    bool dragging = false;
    RECT bounds{};
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(this)) {
            dragging = std::exchange(state->dragging, false);
            bounds = state->dragStartBounds;
            state->systemMoveStarted = state->movedBySystem = false;
        }
    }
    openmfc::detail::ClearPaneFrameDockPreview(*this);
    if (dragging) {
        if (::GetCapture() == window) ::ReleaseCapture();
        if (CWnd::FromHandlePermanent(window) != this) return 0;
        ::SetWindowPos(window, nullptr, bounds.left, bounds.top, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    return Default();
}

// Symbol: ?FindButton@CPaneFrameWnd@@IEBAPEAVCMFCCaptionButton@@I@Z
extern "C" CMFCCaptionButton* MS_ABI impl__FindButton_CPaneFrameWnd__IEBAPEAVCMFCCaptionButton__I_Z(
    const void* pThis, UINT id) {
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    const auto* state = FindStatePtr(pThis);
    return state ? FindCaptionButton(*state, id) : nullptr;
}

// Symbol: ?FindButton@CPaneFrameWnd@@IEBAPEAVCMFCCaptionButton@@VCPoint@@@Z
extern "C" CMFCCaptionButton* MS_ABI impl__FindButton_CPaneFrameWnd__IEBAPEAVCMFCCaptionButton__VCPoint___Z(
    const void* pThis, CPoint point) {
    return HitCaptionButton(pThis, point);
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
// mfc140u 0xb2380 tests visible panes. A positive sensitivity limits the hit to their edge band.
extern "C" void* MS_ABI impl__FrameFromPoint_CPaneFrameWnd__SAPEAV1_VCPoint__HPEAV1_H_Z(
    CPoint point, int sensitivity, const CPaneFrameWnd* excluded, int multiOnly) {
    if (sensitivity < 0) return nullptr;
    std::vector<HWND> windows;
    {
        std::lock_guard<std::mutex> lock(FloatingPaneMutex());
        for (const auto& entry : FloatingPaneMap()) windows.push_back(entry.second);
    }
    for (HWND window : windows) {
        if (!::IsWindowVisible(window)) continue;
        auto* object = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
        if (!object || !object->IsKindOf(RUNTIME_CLASS(CBasePane))) continue;
        auto* pane = static_cast<CBasePane*>(object);
        auto* frame = static_cast<CPaneFrameWnd*>(impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pane, FALSE));
        if (!frame || frame == excluded || !::IsWindowVisible(frame->m_hWnd)) continue;
        if (multiOnly && !(pane->m_dwStyle & CBRS_FLOAT_MULTI) &&
            !(pane->IsKindOf(RUNTIME_CLASS(CDockablePane)) && frame->IsKindOf(CMultiPaneFrameWnd::GetThisClass()))) continue;
        RECT bounds{};
        if (!::GetWindowRect(window, &bounds) || !::PtInRect(&bounds, point)) continue;
        if (!sensitivity) return frame;
        ::InflateRect(&bounds, -sensitivity, -sensitivity);
        if (!::PtInRect(&bounds, point)) return frame;
    }
    return nullptr;
}

// Symbol: ?GetCaptionButtonTotalWidth@CPaneFrameWnd@@IEBAHXZ
extern "C" int MS_ABI impl__GetCaptionButtonTotalWidth_CPaneFrameWnd__IEBAHXZ(const void* pThis) {
    int width = 0;
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    if (const auto* state = FindStatePtr(pThis)) {
        for (const auto& button : state->captionButtons) width += button->GetRect().Width();
    }
    return width;
}

// Symbol: ?GetDockingMode@CPaneFrameWnd@@UEBA?AW4AFX_DOCK_TYPE@@XZ
// The pane supplies the mode. CBasePane's implementation supplies the global default.
extern "C" int MS_ABI impl__GetDockingMode_CPaneFrameWnd__UEBA_AW4AFX_DOCK_TYPE__XZ(
    const void* pThis) {
    const auto* frame = static_cast<const CPaneFrameWnd*>(pThis);
    const auto* pane = frame ? frame->GetPane() : nullptr;
    return impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(
        pane && pane->IsKindOf(RUNTIME_CLASS(CBasePane)) ? static_cast<const CBasePane*>(pane) : nullptr);
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
extern "C" void MS_ABI impl__OnMouseMove_CPaneFrameWnd__IEAAXIVCPoint___Z(
    void* frame, unsigned int flags, CPoint point) {
    if (frame) static_cast<CPaneFrameWnd*>(frame)->OnMouseMove(flags, point);
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
extern "C" void MS_ABI impl__OnMoving_CPaneFrameWnd__IEAAXIPEAUtagRECT___Z(
    void* frame, unsigned int side, RECT* bounds) {
    if (frame) static_cast<CPaneFrameWnd*>(frame)->OnMoving(side, bounds);
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
// Deviation: m_nCaptionHeight has a PaneFrameState side slot, but RecalcCaptionHeight
// cannot populate it without the unexported AFX_GLOBAL_DATA virtual at +0x298;
// this handler therefore continues to use kPaneFrameCaptionHeight.
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
extern "C" void MS_ABI impl__OnNcDestroy_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (pThis) impl__OnNcDestroy_CWnd__IEAAXXZ(AsCwnd(pThis));
}

// Symbol: ?OnNcHitTest@CPaneFrameWnd@@IEAA_JVCPoint@@@Z
// Retail (0xb01c0), transcribed in full -- four instructions that tail-jump to
// vtable +0x3e8 with the third argument zeroed, i.e. `return HitTest(point, 0)`
// (slot 0x3e8 is CPaneFrameWnd::HitTest, entry RVA 0xb01e0, in the mfc140.dll vtable at VA 0x1802f5ad8).
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
// .rdata VA 0x1802f5620, entries at VA 0x1802f5630 (mfc140.dll) maps message 0xa3
// (WM_NCLBUTTONDBLCLK) with signature 0x36 -- void(UINT, CPoint), the same
// signature its OnLButtonDown/OnMouseMove entries use -- to RVA 0xdab0.  That
// address is a single `jmp` to VA 0x180289090 (?Default@CWnd@@IEAA_JXZ, mfc140.dll); the RVA map
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
// The system paints the resize border. Paint the managed strip in the same coordinates as hit testing.
extern "C" void MS_ABI impl__OnNcPaint_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    auto* frame = static_cast<CPaneFrameWnd*>(pThis);
    if (!frame || !frame->m_hWnd) return;
    CRect caption;
    frame->GetCaptionRect(caption);
    if (caption.IsRectEmpty()) return;
    CRect title = caption;
    title.DeflateRect(3, 0);
    RECT redraw{};
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto* state = FindStatePtr(pThis);
        if (!state || !state->captionButtonsConfigured) return;
        redraw = state->redrawRect;
        for (const auto& button : state->captionButtons) {
            if (button->m_bHidden) continue;
            const CRect bounds = button->GetRect();
            if (button->m_bLeftAlign) title.left = (std::max)(title.left, bounds.right + 3);
            else title.right = (std::min)(title.right, bounds.left - 3);
        }
    }
    const HWND window = frame->m_hWnd;
    const HDC hdc = ::GetWindowDC(window);
    if (!hdc) return;
    const int saved = ::SaveDC(hdc);
    if (!saved) {
        ::ReleaseDC(window, hdc);
        return;
    }
    ::IntersectClipRect(hdc, caption.left, caption.top, caption.right, caption.bottom);
    if (!::IsRectEmpty(&redraw))
        ::IntersectClipRect(hdc, redraw.left, redraw.top, redraw.right, redraw.bottom);
    const bool active = ::GetActiveWindow() == window;
    ::FillRect(hdc, caption, ::GetSysColorBrush(active ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION));
    ::SetBkMode(hdc, TRANSPARENT);
    ::SetTextColor(hdc, ::GetSysColor(active ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT));
    NONCLIENTMETRICSW metrics{sizeof(metrics)};
    HFONT font = nullptr;
    if (::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(metrics), &metrics, 0)) {
        const bool toolWindow = (::GetWindowLongPtrW(window, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != 0;
        font = ::CreateFontIndirectW(toolWindow ? &metrics.lfSmCaptionFont : &metrics.lfCaptionFont);
    }
    if (font) ::SelectObject(hdc, font);
    std::wstring text(::GetWindowTextLengthW(window) + 1, L'\0');
    const int length = ::GetWindowTextW(window, text.data(), static_cast<int>(text.size()));
    if (!title.IsRectEmpty())
        ::DrawTextW(hdc, text.data(), length, title, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

    CDC dc;
    dc.SetOutputDC(hdc);
    impl__OnDrawCaptionButtons_CPaneFrameWnd__MEAAXPEAVCDC___Z(frame, &dc);
    dc.ReleaseOutputDC();
    ::RestoreDC(hdc, saved);
    if (font) ::DeleteObject(font);
    ::ReleaseDC(window, hdc);
}

// Symbol: ?OnNeedTipText@CPaneFrameWnd@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
// Retail mfc140.dll (ANSI twin) entry RVA 0xb4120 throws on null pNMHDR. It
// returns FALSE for a mismatched tooltip HWND, an active popup, or an out-of-range
// idFrom; otherwise IDs 8/9, 0x14, and 0x19 select resources 0x3ea1, 0x3ee9,
// and 0x4279 respectively; loaded text is stored in NMTTDISPINFO +0x18. The UINT id argument
// is not used. STUB: tooltip/button side slots now exist, but no initialized
// native tooltip tool-ID mapping or resource-string ownership path is available;
// this body also does not reproduce retail's exception paths.
extern "C" int MS_ABI impl__OnNeedTipText_CPaneFrameWnd__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    void* pThis, unsigned int id, NMHDR* pNMHDR, __int64* pResult) {
    (void)pThis;
    (void)id;
    (void)pNMHDR;
    (void)pResult;
    return FALSE;
}

// Symbol: ?OnPaneRecalcLayout@CPaneFrameWnd@@UEAAXXZ
// Retail mfc140.dll (ANSI twin) entry RVA 0xb0c30 gets the frame client rect,
// resolves the hosted pane HWND (+0x148) permanently, and requires a CPane.
// When CPane::m_bHandleMinSize is nonzero it gets the pane minimum size via the
// unexported vtable slot +0x4f0, expands the client rect as needed, and records
// whether it changed. It then calls the pane's SetWindowPos virtual (+0x480)
// with flags 0x34, redraws the pane (RedrawWindow flags 0x105), and calls the
// frame's SizeToContent virtual (+0x378) only if the minimum-size expansion
// occurred. The mfc140u symbol map does not resolve this export, but the ANSI
// twin body is disassembled and the function bodies are byte-identical.
// OpenMFC uses PaneFrameState::paneHwnd, now synchronized by AddPane/RemovePane;
// it reads the verified CPane::m_sizeMin field because the +0x4f0 inline has no
// thunk, and calls the exported SetWindowPos thunk (which virtual-dispatches
// through CBasePane). The final SizeToContent thunk remains a no-op in this
// build; its direct thunk call also bypasses further-derived overrides. A null
// frame HWND is guarded here.
static_assert(offsetof(CPane, m_sizeMin) == 0x200,
              "OnPaneRecalcLayout reads the verified CPane minimum-size field");
extern "C" void MS_ABI impl__OnPaneRecalcLayout_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;

    HWND frameHwnd = HwndOf(pThis);
    if (frameHwnd == nullptr) return;
    RECT clientRect = {};
    ::GetClientRect(frameHwnd, &clientRect);

    HWND paneHwnd = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const PaneFrameState* state = FindStatePtr(pThis);
        if (state != nullptr) paneHwnd = state->paneHwnd;
    }
    CWnd* paneWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(paneHwnd);
    if (paneWnd == nullptr) return;
    CRuntimeClass* paneClass = impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
    if (paneClass == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(paneWnd, paneClass)) {
        return;
    }
    CPane* pane = static_cast<CPane*>(paneWnd);

    bool expanded = false;
    if (impl__m_bHandleMinSize_CPane__2HA != 0) {
        if (clientRect.right - clientRect.left < pane->m_sizeMin.cx) {
            clientRect.right = clientRect.left + pane->m_sizeMin.cx;
            expanded = true;
        }
        if (clientRect.bottom - clientRect.top < pane->m_sizeMin.cy) {
            clientRect.bottom = clientRect.top + pane->m_sizeMin.cy;
            expanded = true;
        }
    }

    impl__SetWindowPos_CBasePane__UEAAPEAXPEBVCWnd__HHHHIPEAX_Z(
        static_cast<CBasePane*>(pane), nullptr, clientRect.left, clientRect.top,
        clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
        0x34, nullptr);
    ::RedrawWindow(paneHwnd, nullptr, nullptr, 0x105);
    if (expanded) impl__SizeToContent_CPaneFrameWnd__UEAAXXZ(pThis);
}

// Symbol: ?OnPressButtons@CPaneFrameWnd@@MEAAXI@Z
// Retail mfc140.dll entry RVA 0xb4430 returns immediately in customize mode;
// otherwise only ID 0x19 is handled, by finding its menu button and opening the
// pane menu. The customize flag and semantic button records are available here.
// STUB: the menu-button object and popup-menu construction/command-routing path
// are not modeled, so handling ID 0x19 cannot be reproduced safely.
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
// Retail mfc140.dll entry RVA 0xaf4f0 gets the pane, verifies CBasePane RTTI,
// then calls its vtable slot +0x398 and tests bit 0x10 before starting the timer.
// That base virtual's body is RVA 0x8850 (mfc140.dll): `return m_dwControlBarStyle`
// from +0x108. Read the verified field directly; this matches CBasePane's base
// implementation but, unlike retail virtual dispatch, skips derived overrides.
static_assert(offsetof(CBasePane, m_dwControlBarStyle) == 0x108,
              "OnSetRollUpTimer reads CBasePane::m_dwControlBarStyle");
extern "C" void MS_ABI impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CWnd* pPane = reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pThis));
    if (pPane == nullptr) return;
    CRuntimeClass* pBasePane = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    if (pBasePane == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pBasePane)) {
        return;
    }
    const CBasePane* pBase = static_cast<const CBasePane*>(pPane);
    if ((pBase->m_dwControlBarStyle & autoRollupStyle) != 0) {
        impl__SetRollUpTimer_CPaneFrameWnd__IEAAXXZ(pThis);
    }
}

// Symbol: ?OnSettingChange@CPaneFrameWnd@@IEAAXIPEB_W@Z
// Retail (0xb4090 -- the ANSI twin's ?OnSettingChange@CPaneFrameWnd@@IEAAXIPEBD@Z),
// transcribed in full: CWnd::OnSettingChange(uFlags, lpszSection), then a tail
// call to vtable +0x498, which the mfc140.dll CPaneFrameWnd vtable at VA 0x1802f5ad8 shows is
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
extern "C" void MS_ABI impl__OnSize_CPaneFrameWnd__IEAAXIHH_Z(
    void* pThis, unsigned int type, int width, int height) {
    if (pThis) static_cast<CPaneFrameWnd*>(pThis)->OnSize(type, width, height);
}

void CPaneFrameWnd::OnSize(UINT type, int width, int height) {
    Default();
    if (type == SIZE_MINIMIZED) return;
    impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(this);
    if (auto* pane = GetPane(); pane && ::GetParent(pane->m_hWnd) == m_hWnd) {
        ::SetWindowPos(pane->m_hWnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

// Symbol: ?OnSizing@CPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
// Retail mfc140.dll (ANSI twin) entry RVA 0xb0790 first calls CWnd::Default(),
// then returns early when the rolled-up state flag (+0xf8) is nonzero. On the
// remaining resize paths it checks whether the proposed edge/size changed,
// resolves the hosted pane (+0x148), and returns unless it is a CMFCToolBar.
// For an applicable toolbar resize it subtracts the border (vtable +0x388) and
// caption height (+0x114), asks toolbar virtuals +0x558 and +0x420 to reflow,
// and adjusts lpRect to the reported size; later layout/window-position work
// also follows. The mfc140u symbol map did not resolve this export.
// Partial implementation: the Default() call is reproduced; the +0xf8 gate
// and resize constraint are omitted because the hosted-pane, caption-height,
// and toolbar virtual state are not modelled. OpenMFC also returns for null
// pThis instead of retail's unconditional dereference.
extern "C" void MS_ABI impl__OnSizing_CPaneFrameWnd__IEAAXIPEAUtagRECT___Z(
    void* pThis, unsigned int /*nSide*/, RECT* /*lpRect*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(AsCwnd(pThis));
}

// Symbol: ?OnTimer@CPaneFrameWnd@@IEAAX_K@Z
// mfc140u 0xb0c80 sends docking ticks to the manager and checks roll-up on the other timer.
extern "C" void MS_ABI impl__OnTimer_CPaneFrameWnd__IEAAX_K_Z(
    void* pThis, UINT_PTR timer) {
    if (pThis) static_cast<CPaneFrameWnd*>(pThis)->OnTimer(timer);
}

void CPaneFrameWnd::OnTimer(UINT_PTR timer) {
    const HWND window = m_hWnd;
    if (timer == dockingTimer) {
        CGlobalUtils utils;
        if (auto* manager = utils.GetDockingManager(this))
            impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(manager, this);
        return;
    }
    if (timer == rollupTimer) impl__OnCheckRollState_CPaneFrameWnd__MEAAXXZ(this);
    if (CWnd::FromHandlePermanent(window) == this) Default();
}

// Symbol: ?OnTrackCaptionButtons@CPaneFrameWnd@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CPaneFrameWnd__MEAAXVCPoint___Z(
    void* pThis, CPoint point) {
    if (!pThis) return;
    const auto* button = HitCaptionButton(pThis, point);
    TrackCaptionButton(pThis, button && button->m_bEnabled ? button->m_nHit : 0);
}

// Symbol: ?OnUpdateToolTips@CPaneFrameWnd@@IEAA_J_K_J@Z
// Retail mfc140.dll (ANSI twin) entry RVA 0xb4a00, transcribed:
//     if (wParam & 8) {
//         CTooltipManager::CreateToolTip(m_pToolTip /*0x1b8*/, this, 8);
//         SetCaptionButtonsToolTips();     // direct call to 0xb4300
//         UpdateTooltips();                // direct call to 0xb1870
//     }
//     return 0;
// TODO(clean-room): the tooltip pointer slot exists, but no initialized native
// tooltip control is available, so CTooltipManager::CreateToolTip is skipped;
// the two helper calls are reproduced and remain no-ops. OpenMFC also returns
// 0 for null pThis, while retail dereferences the object.
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
// Deviation: the caption-height side slot exists, but the unexported global-data
// sizing virtual is not available, so this handler uses kPaneFrameCaptionHeight.
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
// the CPaneFrameWnd vtable at .rdata VA 0x1802f5ad8 (mfc140.dll) points at entry RVA 0xb1c80.)
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
// TODO(clean-room): the tooltip pointer slot exists, but no initialized native
// tooltip control/tool-registration lifecycle is available; the relay is skipped
// and only the CWnd::PreTranslateMessage tail call runs. This matches retail's
// null-m_pToolTip path.
extern "C" int MS_ABI impl__PreTranslateMessage_CPaneFrameWnd__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg) {
    if (pThis == nullptr) return FALSE;
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(AsCwnd(pThis), pMsg);
}

// Symbol: ?RecalcCaptionHeight@CPaneFrameWnd@@MEAAXXZ
// Retail mfc140.dll entry RVA 0xb40c0, transcribed:
//     CSize sz = afxGlobalData.<unexported virtual, vtable +0x298>();
//     m_nCaptionHeight /*0x114*/ = sz.cy + ::GetSystemMetrics(SM_CYSMCAPTION);
//     m_?? /*0x160*/ = m_?? /*0x164*/ = m_nCaptionHeight + 15;
// (afxGlobalData is the lazily-constructed singleton returned by the getter at
// RVA 0x97f4 (mfc140.dll); its virtual at +0x298 returns CSize by value, and
// only .cy -- the second int, read at instruction 0xb40ef inside RecalcCaptionHeight -- is used.)
// STUB: captionHeight and minSize fields now exist in PaneFrameState, but the
// required AFX_GLOBAL_DATA virtual at vtable +0x298 is unexported and the local
// global-data blob has no verified callable slot for it. Substituting another
// metric would invent behavior; GetCaptionRect/OnNcCalcSize still use a constant.
extern "C" void MS_ABI impl__RecalcCaptionHeight_CPaneFrameWnd__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?RedrawAll@CPaneFrameWnd@@SAXXZ
// mfc140u 0xb3060 repaints every live mini-frame and its children.
extern "C" void MS_ABI impl__RedrawAll_CPaneFrameWnd__SAXXZ() {
    std::vector<HWND> frames;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        for (const auto& item : g_other6State) {
            if (item.second.frameWindow) frames.push_back(item.second.frameWindow);
        }
    }
    constexpr UINT flags = RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME;
    for (HWND frame : frames) {
        if (CWnd::FromHandlePermanent(frame)) ::RedrawWindow(frame, nullptr, nullptr, flags);
    }
}

// Symbol: ?RedrawCaptionButton@CPaneFrameWnd@@IEAAXPEAVCMFCCaptionButton@@@Z
// Retail (0xb1a80), transcribed in full:
//     if (pButton == NULL) return;
//     m_rectRedraw /*0x168, 16 bytes*/ = pButton-><vtable +0x28>();  // its rect
//     ::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
//     ::SetRectEmpty(&m_rectRedraw);
//     ::UpdateWindow(m_hWnd);
// The redraw rectangle limits non-client painting to the requested button.
extern "C" void MS_ABI impl__RedrawCaptionButton_CPaneFrameWnd__IEAAXPEAVCMFCCaptionButton___Z(
    void* pThis, void* pButton) {
    if (pThis == nullptr || pButton == nullptr) return;
    HWND hwnd = HwndOf(pThis);
    if (hwnd == nullptr) return;
    const CRect bounds = static_cast<CMFCCaptionButton*>(pButton)->GetRect();
    if (bounds.IsRectEmpty()) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).redrawRect = *static_cast<const RECT*>(bounds);
    }
    ::SendMessageW(hwnd, WM_NCPAINT, 0, 0);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(pThis)) ::SetRectEmpty(&state->redrawRect);
    }
    ::UpdateWindow(hwnd);
}

// Symbol: ?RemoveAllCaptionButtons@CPaneFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveAllCaptionButtons_CPaneFrameWnd__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(pThis);
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    auto& state = GetState(pThis);
    state.captionButtonsConfigured = true;
    state.rollupFlags = 0;
    state.captionButtons.clear();
}

// Symbol: ?RemoveButton@CPaneFrameWnd@@IEAAXI@Z
extern "C" void MS_ABI impl__RemoveButton_CPaneFrameWnd__IEAAXI_Z(void* pThis, UINT id) {
    if (!pThis) return;
    impl__StopCaptionButtonsTracking_CPaneFrameWnd__MEAAXXZ(pThis);
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (auto* state = FindStatePtr(pThis)) {
            auto& buttons = state->captionButtons;
            const auto item = std::find_if(buttons.begin(), buttons.end(),
                [id](const auto& button) { return button->m_nHit == id; });
            if (item != buttons.end()) buttons.erase(item);
        }
    }
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
// Retail mfc140.dll (ANSI twin) entry RVA 0xb1690, transcribed in full:
//     for (POSITION p = m_lstCaptionButtons.GetHeadPosition() /*0x188*/; p;
//          p = <node pNext at +0>) {
//         CMFCCaptionButton* b = <node data at +0x10>;
//         if (b->m_nHit /*+0x20*/ == uiOldID) { b->m_nHit = uiNewID; return; }
//     }
extern "C" void MS_ABI impl__ReplaceButton_CPaneFrameWnd__IEAAXII_Z(
    void* pThis, unsigned int uiOldID, unsigned int uiNewID) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    PaneFrameState* state = FindStatePtr(pThis);
    if (state == nullptr) return;
    if (auto* button = FindCaptionButton(*state, uiOldID)) {
        button->m_nHit = uiNewID;
        if (state->hitButtonID == uiOldID) state->hitButtonID = uiNewID;
        if (state->hotButtonID == uiOldID) state->hotButtonID = uiNewID;
    }
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
    if (!pThis || !pOldBar || !pNewBar || pOldBar == pNewBar) return;
    openmfc::detail::ReplaceFloatingPaneRegistration(*static_cast<CPaneFrameWnd*>(pThis),
        *static_cast<CBasePane*>(pOldBar), *static_cast<CBasePane*>(pNewBar));
    impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(pThis);
}

void openmfc::detail::ReplaceFloatingPaneRegistration(
    CPaneFrameWnd& frame, CBasePane& oldPane, CBasePane& newPane) {
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(&oldPane, FALSE);
    const HWND hOld = oldPane.GetSafeHwnd();
    const HWND hNew = newPane.GetSafeHwnd();
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        PaneFrameState& st = GetState(&frame);
        if (st.paneHwnd == hOld) st.paneHwnd = hNew;
    }
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        PaneState& state = g_framePanes[&frame];
        RemovePane(state, &oldPane);
        AddUniquePane(state, &newPane);
    }
    impl__AddRemovePaneFromGlobalList_CPaneFrameWnd__SAHPEAVCBasePane__H_Z(&newPane, TRUE);
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
// Retail mfc140.dll ANSI-twin entry RVA 0xb2c40; the Unicode spelling has no
// resolved RVA in the symbol map. It resolves the hosted pane at +0x148, returns
// TRUE when no CBasePane is found, and otherwise dispatches its vtable +0x470
// SaveState(profile, uiID, UINT(-1)). Use the real C ABI thunks for the modeled
// CMFCToolBar, CDockablePaneAdapter, and CPane paths (CPane covers
// CDockablePane's forwarding body); other CBasePane instances use their base
// thunk. Deviation: this explicit type routing does not cover every derived
// override. CBaseTabbedPane has a source thunk, but that thunk is itself a
// documented stub, so the CPane path is used as the available fallback for
// CPane-derived types.
extern "C" int MS_ABI impl__SaveState_CPaneFrameWnd__UEAAHPEB_WI_Z(
    void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    if (pThis == nullptr) return FALSE;
    HWND paneHwnd = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const PaneFrameState* state = FindStatePtr(pThis);
        if (state != nullptr) paneHwnd = state->paneHwnd;
    }
    CWnd* pPane = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(paneHwnd);
    if (pPane == nullptr) return TRUE;
    CRuntimeClass* pBaseClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    if (pBaseClass == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pBaseClass)) {
        return TRUE;
    }

    const int nIndex = static_cast<int>(uiID);
    const unsigned int paneUIID = static_cast<unsigned int>(-1);
    CRuntimeClass* pToolbarClass = impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
    if (pToolbarClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pToolbarClass)) {
        return impl__SaveState_CMFCToolBar__UEAAHPEB_WHI_Z(
            static_cast<CMFCToolBar*>(pPane), lpszProfileName, nIndex, paneUIID);
    }
    CRuntimeClass* pAdapterClass = impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ();
    if (pAdapterClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pAdapterClass)) {
        return impl__SaveState_CDockablePaneAdapter__UEAAHPEB_WHI_Z(
            pPane, lpszProfileName, nIndex, paneUIID);
    }
    CRuntimeClass* pPaneClass = impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();
    if (pPaneClass != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pPane, pPaneClass)) {
        return impl__SaveState_CPane__UEAAHPEB_WHI_Z(
            static_cast<CPane*>(pPane), lpszProfileName, nIndex, paneUIID);
    }
    return impl__SaveState_CBasePane__UEAAHPEB_WHI_Z(
        static_cast<CBasePane*>(pPane), lpszProfileName, nIndex, paneUIID);
}

extern "C" void* impl__m_pParentWndForSerialize_CPaneFrameWnd__1PEAVCFrameWnd__EA;

// Symbol: ?Serialize@CPaneFrameWnd@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CPaneFrameWnd__UEAAXAEAVCArchive___Z(
    CPaneFrameWnd* frame, CArchive& archive) {
    if (frame) frame->CPaneFrameWnd::Serialize(archive);
}

void CPaneFrameWnd::Serialize(CArchive& archive) {
    DWORD style = 0;
    RECT bounds{};
    BOOL visible = FALSE;
    UINT paneId = 0;
    DWORD buttons = 0;
    BOOL pinned = FALSE;
    if (archive.IsStoring()) {
        style = static_cast<DWORD>(::GetWindowLongPtrW(m_hWnd, GWL_STYLE));
        ::GetWindowRect(m_hWnd, &bounds);
        visible = ::IsWindowVisible(m_hWnd);
        if (auto* pane = GetPane()) paneId = static_cast<UINT>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(pane));
        {
            std::lock_guard<std::mutex> lock(g_other6Mutex);
            if (const auto* state = FindStatePtr(this)) {
                if (state->rolledUp && state->heightBeforeRollUp)
                    bounds.bottom = bounds.top + static_cast<LONG>(state->heightBeforeRollUp);
                buttons = static_cast<DWORD>(state->rollupFlags);
                pinned = state->pinned;
            }
        }
        archive << style;
        archive.Write(&bounds, sizeof(bounds));
        archive << visible << paneId << buttons << pinned;
        return;
    }

    archive >> style;
    archive.EnsureRead(&bounds, sizeof(bounds));
    archive >> visible >> paneId >> buttons >> pinned;
    auto* owner = static_cast<CFrameWnd*>(impl__m_pParentWndForSerialize_CPaneFrameWnd__1PEAVCFrameWnd__EA);
    if (!owner || !owner->m_hWnd || !Create(L"", style & ~static_cast<DWORD>(WS_VISIBLE), bounds, owner, nullptr))
        AfxThrowArchiveException(CArchiveException::generic, archive.m_strFileName.GetString());
    std::lock_guard<std::mutex> lock(g_other6Mutex);
    auto& state = GetState(this);
    state.restoredEmbeddedBarID = paneId;
    state.captionButtonsConfigured = true;
    state.rollupFlags = static_cast<int>(buttons);
    state.pinned = pinned != FALSE;
    state.restoredFrameVisible = visible != FALSE;
}

// Symbol: ?SetCaptionButtons@CPaneFrameWnd@@UEAAXK@Z
// Retail mfc140.dll ANSI twin entry RVA 0xb13f0, transcribed in full:
//     RemoveAllCaptionButtons();                        // 0xb1810
//     if (dwButtons & 2) {
//         CWnd* p = GetPane();                          // vtable 0x360
//         if (p && p->IsKindOf(RUNTIME_CLASS(CBasePane) /*descriptor VA 0x1802da490, mfc140.dll*/) &&
//             p-><vtable +0x3a0>())
//             AddButton(0x14);                          // vtable 0x470
//     }
//     if (dwButtons & 1)    AddButton(9);
//     if (dwButtons & 4)    AddButton(8);
//     if (dwButtons & 0x10) AddButton(0x19);            // the menu button
//     m_dwCaptionButtons /*0x12c*/ = dwButtons;
//     SetCaptionButtonsToolTips();                      // entry RVA 0xb4300 (mfc140.dll)
//     ArrangeCaptionButtons();                          // entry RVA 0xb13d0 (mfc140u.dll)
//     ::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);          // mfc140.dll USER32 import slot VA 0x1802c5378, tail jump
extern "C" void MS_ABI impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(
    void* pThis, unsigned long dwButtons) {
    if (pThis == nullptr) return;
    impl__RemoveAllCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
    if ((dwButtons & 0x2) != 0) {
        auto* pane = static_cast<CPaneFrameWnd*>(pThis)->GetPane();
        if (pane && pane->IsKindOf(RUNTIME_CLASS(CBasePane)) && static_cast<CBasePane*>(pane)->CanBeClosed())
            impl__AddButton_CPaneFrameWnd__MEAAXI_Z(pThis, HTCLOSE);
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
// Retail mfc140.dll (ANSI twin) entry RVA 0xb4300 drives its tooltip control
// at +0x1b8: it queries TTM_GETTOOLCOUNT (SendMessageA import slot 0x1802c5378),
// compares with the caption-button count, then creates/deletes tools through
// CTooltipManager. STUB: semantic button and tooltip records now exist, but no
// initialized native tooltip control or actual tool-registration lifecycle does.
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
    const UINT_PTR nNew = hwnd ? ::SetTimer(hwnd, dockingTimer, nTimeOut, nullptr) : 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).dockTimerID = static_cast<int>(nNew);
    }
}

extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager*, UINT, int);

void openmfc::detail::FinishFloatingFrameRestore(CPaneFrameWnd& frame, bool show) {
    DWORD buttons = 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto& state = GetState(&frame);
        buttons = static_cast<DWORD>(state.rollupFlags);
        show = show && state.restoredFrameVisible;
        state.delayShow = show;
    }
    impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(&frame, buttons);
    if (buttons & 1) impl__OnSetRollUpTimer_CPaneFrameWnd__UEAAXXZ(&frame);
    if (auto* pane = frame.GetPane()) {
        std::wstring caption(::GetWindowTextLengthW(pane->m_hWnd) + 1, L'\0');
        ::GetWindowTextW(pane->m_hWnd, caption.data(), static_cast<int>(caption.size()));
        frame.SetWindowTextW(caption.c_str());
        for (int size : {ICON_SMALL, ICON_BIG}) {
            const auto icon = ::SendMessageW(pane->m_hWnd, WM_GETICON, size, 0);
            ::SendMessageW(frame.m_hWnd, WM_SETICON, size, icon);
        }
    }
    frame.AdjustLayout();
    ::ShowWindow(frame.m_hWnd, show ? SW_SHOWNOACTIVATE : SW_HIDE);
}

void openmfc::detail::ApplyDelayedMiniFrameVisibility(CPaneFrameWnd& frame, BOOL show) {
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        auto* state = FindStatePtr(&frame);
        if (!state || !std::exchange(state->delayShow, false)) return;
    }
    if (::IsWindow(frame.m_hWnd))
        ::ShowWindow(frame.m_hWnd, show ? SW_SHOWNOACTIVATE : SW_HIDE);
}

// Symbol: ?SetDockState@CPaneFrameWnd@@UEAAXPEAVCDockingManager@@@Z
extern "C" void MS_ABI impl__SetDockState_CPaneFrameWnd__UEAAXPEAVCDockingManager___Z(
    void* self, void* dockManager) {
    auto* frame = static_cast<CPaneFrameWnd*>(self);
    auto* manager = static_cast<CDockingManager*>(dockManager);
    if (!frame || !frame->m_hWnd || !manager) return;
    UINT id = 0;
    BOOL visible = FALSE;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        if (const auto* state = FindStatePtr(frame)) {
            id = state->restoredEmbeddedBarID;
            visible = state->restoredFrameVisible;
        }
    }
    auto* pane = id && id != UINT(-1)
        ? impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(manager, id, TRUE) : nullptr;
    if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CPane)) ||
        !openmfc::detail::RestorePaneInFloatingFrame(*static_cast<CPane*>(pane), *frame, *manager, visible)) {
        if (!frame->GetPane()) frame->DestroyWindow();
        return;
    }
    RECT client{};
    if (::GetClientRect(frame->m_hWnd, &client))
        ::SetWindowPos(pane->m_hWnd, nullptr, 0, 0, client.right, client.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
    openmfc::detail::FinishFloatingFrameRestore(*frame, visible != FALSE);
}

// Symbol: ?SetPreDockState@CPaneFrameWnd@@UEAAHW4AFX_PREDOCK_STATE@@PEAVCBasePane@@W4AFX_DOCK_METHOD@@@Z
// The single-pane frame returns FALSE once it dispatches an accepted request.
extern "C" int MS_ABI impl__SetPreDockState_CPaneFrameWnd__UEAAHW4AFX_PREDOCK_STATE__PEAVCBasePane__W4AFX_DOCK_METHOD___Z(
    void* pThis, int request, void* bar, int method) {
    auto* frame = static_cast<CPaneFrameWnd*>(pThis);
    if (!frame || !frame->m_hWnd || (request != PreDockRegular && request != PreDockToTab)) return TRUE;
    auto* window = frame->GetPane();
    if (!window || !window->IsKindOf(RUNTIME_CLASS(CDockablePane))) return TRUE;
    auto* pane = static_cast<CDockablePane*>(window);
    auto* candidate = static_cast<CBasePane*>(bar);
    if (candidate && (candidate == pane || !candidate->IsKindOf(RUNTIME_CLASS(CDockablePane)) ||
            !::IsWindow(candidate->m_hWnd) ||
            impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(candidate, FALSE) ||
            (request == PreDockToTab && !candidate->CanBeAttached()))) return TRUE;
    CGlobalUtils utils;
    auto* manager = utils.GetDockingManager(frame);
    POINT point{};
    if (!manager || !::GetCursorPos(&point)) return TRUE;
    const auto drop = PreDockTarget(manager, CPoint(point), pane->m_dwEnabledAlignment,
        static_cast<CDockablePane*>(candidate), request);

    const bool wasDragging = openmfc::detail::EndPaneFrameDockDrag(*frame);
    if (auto* docked = DockPaneToDrop(*pane, drop, method)) {
        if (wasDragging && ::GetKeyState(VK_LBUTTON) < 0)
            impl__EnterDragMode_CPane__MEAAXH_Z(docked, FALSE);
    }
    return FALSE;
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
        hwnd ? ::SetTimer(hwnd, rollupTimer, impl__m_nRollTimeOut_CPaneFrameWnd__2IA, nullptr) : 0;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        GetState(pThis).rollupTimerID = static_cast<int>(nNew);
    }
    impl__SetCaptionButtons_CPaneFrameWnd__UEAAXK_Z(pThis, dwButtons | 1UL);
}

// Symbol: ?ShowButton@CPaneFrameWnd@@IEAAXIH@Z
// The Boolean is copied into the hidden field by retail.
extern "C" void MS_ABI impl__ShowButton_CPaneFrameWnd__IEAAXIH_Z(void* pThis, UINT id, BOOL hidden) {
    if (!pThis) return;
    {
        std::lock_guard<std::mutex> lock(g_other6Mutex);
        const auto* state = FindStatePtr(pThis);
        auto* button = state ? FindCaptionButton(*state, id) : nullptr;
        if (!button) return;
        button->m_bHidden = hidden;
    }
    impl__ArrangeCaptionButtons_CPaneFrameWnd__IEAAXXZ(pThis);
    if (HWND window = HwndOf(pThis)) ::SendMessageW(window, WM_NCPAINT, 0, 0);
}

// Symbol: ?SizeToContent@CPaneFrameWnd@@UEAAXXZ
// Retail mfc140.dll ANSI twin entry RVA 0xb0b30, transcribed in full:
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndPane /*0x148*/);
//     if (!p || !p->IsKindOf(RUNTIME_CLASS(CPane))) return;
//     CSize size = p-><vtable +0x4d0>(0, TRUE);       // the pane's content size
//     CRect rectWindow; ::GetWindowRect(m_hWnd, &rectWindow); // USER32 slot 0x1802c5370
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient); // USER32 slot 0x1802c5358
//     SetWindowPos(NULL, 0, 0,
//                  size.cx + rectWindow.Width()  - rectClient.Width(),
//                  size.cy + rectWindow.Height() - rectClient.Height(),
//                  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE /*0x16*/);
// STUB: the pane HWND is represented in side state, but its ideal content size
// still comes entirely from the unexported CPane virtual at +0x4d0. Substituting
// the current window size would defeat the operation.
extern "C" void MS_ABI impl__SizeToContent_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    (void)pThis;
}
