// CMFCTasksPane — OpenMFC implementation.
// Sources: ctrlcore.cpp, global_mfc-feature-20_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/CMFCTasksPaneSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?AddGroup@CMFCTasksPane@@QEAAHHPEB_WHHPEAUHICON__@@@Z
extern "C" int MS_ABI impl__AddGroup_CMFCTasksPane__QEAAHHPEB_WHHPEAUHICON_____Z(
    CMFCTasksPane* pThis, int nGroup, const wchar_t* lpszName, int bBottomHasGripper, int bSpecial, void* hIcon) {
    return pThis ? pThis->AddGroup(nGroup, lpszName, bBottomHasGripper ? TRUE : FALSE, bSpecial ? TRUE : FALSE,
                                     reinterpret_cast<HICON>(hIcon))
                 : -1;
}
// Symbol: ?RemoveAllTasks@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RemoveAllTasks_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nGroup) {
    if (pThis) pThis->RemoveAllTasks(nGroup);
}
// Symbol: ?SetCaption@CMFCTasksPane@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetCaption_CMFCTasksPane__QEAAXPEB_W_Z(CMFCTasksPane* pThis, const wchar_t* lpszCaption) {
    if (pThis) pThis->SetCaption(lpszCaption);
}
// CMFCTasksPane::OnPressBackButton -- retail (RVA 0x148d60) decrements
// m_iActivePage when it is positive and refreshes via the internal helper at
// 0x148990. The refresh is approximated with the pane-wide redraw.
// Symbol: ?OnPressBackButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    if (self->m_iActivePage > 0) {
        self->m_iActivePage--;
        RefreshPane(pThis);
    }
}
// CMFCTasksPane::OnPressForwardButton -- retail (RVA 0x148d90) increments
// m_iActivePage while it is below the history-stack count minus one
// (m_arrHistoryStack.m_nSize @0x788) and refreshes.
// Symbol: ?OnPressForwardButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    if (static_cast<__int64>(self->m_iActivePage) < self->m_nHistorySize - 1) {
        self->m_iActivePage++;
        RefreshPane(pThis);
    }
}
// CMFCTasksPane::OnPressHomeButton -- retail (RVA 0x148dc0) validates the
// current page index against the history stack (data @0x780, size @0x788),
// and when the history entry for the current page is non-zero navigates to
// page 0 (internal helper 0x148b00). The navigation is reproduced against
// the mirrored history fields.
// Symbol: ?OnPressHomeButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressHomeButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    __int64 n = self->m_iActivePage;
    if (n < 0 || n >= self->m_nHistorySize || self->m_pHistoryData == nullptr) {
        return;
    }
    if (self->m_pHistoryData[n] != 0) {
        self->m_iActivePage = 0;
        RefreshPane(pThis);
    }
}
// CMFCTasksPane::OnPressButtons(unsigned int) -- retail (RVA 0x148ce0)
// dispatches on nHit: 0x17 -> the "back" virtual (vtable slot 0x750),
// 0x18 -> the "forward" virtual (slot 0x758), 0x19 -> the "other" menu
// button, which sets m_bMenuBtnPressed (0x4F8), looks up the module resource
// handle (AfxFindResourceHandle at 0x1800453c0) and invokes the popup-menu
// virtual (slot 0x768). The 0x17/0x18 branches are the exported
// OnPressBackButton/OnPressForwardButton methods and are reproduced exactly;
// the 0x19 popup-menu path needs the menu/icon state that is not modeled.
// TODO(clean-room): partially transcribed -- the 0x19 "other menu" popup is
// not reproduced.
// Symbol: ?OnPressButtons@CMFCTasksPane@@MEAAXI@Z
extern "C" void MS_ABI impl__OnPressButtons_CMFCTasksPane__MEAAXI_Z(
    CMFCTasksPane* pThis, unsigned int nHit)
{
    if (!pThis) return;
    switch (nHit) {
    case 0x17:
        impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(pThis);
        break;
    case 0x18:
        impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(pThis);
        break;
    case 0x19: {
        // Retail: m_bMenuBtnPressed = TRUE; popup-menu virtual call;
        // m_bMenuBtnPressed = FALSE.
        S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
        self->m_bMenuBtnPressed = TRUE;
        self->m_bMenuBtnPressed = FALSE;
        break;
    }
    default:
        break;
    }
}
// CMFCTasksPane::OnForward -- retail (RVA 0x14b1e0): when m_bHistoryMenuButtons
// (0x4E8) is set it computes the new active page from the internal last
// navigation index (global 0x1803b1b3c) and the history-stack geometry, then
// refreshes via 0x148990; otherwise it tail-calls the forward virtual (vtable
// slot 0x758). The global navigation index is internal MFC state, so the
// history branch cannot be reproduced here.
// TODO(clean-room): partially transcribed -- depends on the internal global
// last-navigation index.
// Symbol: ?OnForward@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnForward_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    if (self->m_bHistoryMenuButtons) {
        // Retail: g_nLastNavigationIndex (0x1803b1b3c) must be >= 0 and less
        // than (historySize - m_iActivePage - 1) before the jump happens.
        // The global is not modeled -> conservative no-op.
    }
    // Retail else-branch tail-calls vtable slot 0x758 (OnPressForwardButton);
    // not reproduced to avoid double-navigation.
}
// CMFCTasksPane::OnHome -- retail (RVA 0x14b230) is a pure tail call to
// vtable slot 0x760, which points at OnPressHomeButton; reproduced directly.
// Symbol: ?OnHome@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnHome_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    impl__OnPressHomeButton_CMFCTasksPane__UEAAXXZ(pThis);
}
// CMFCTasksPane::OnOther -- retail (RVA 0x14b270) tail-calls the internal
// NavigateToPage helper (0x148b00) with the internal last-navigation index
// (global 0x1803b1b3c). That index is internal MFC state.
// TODO(clean-room): partially transcribed -- depends on the internal global
// last-navigation index.
// Symbol: ?OnOther@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnOther_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    // Conservative: no navigation performed.
}
// CMFCTasksPane::OnClose -- retail (RVA 0x14b250) is a pure tail call to
// vtable slot 0x5c8 (a close/visibility virtual not exported by OpenMFC).
// TODO(clean-room): partially transcribed -- vtable slot 0x5c8 is not
// modeled.
// Symbol: ?OnClose@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnEraseBkgnd(CDC*) -- retail (RVA 0x3a60) is literally
// `mov $0x1,%eax; ret` (always erase).
// Symbol: ?OnEraseBkgnd@CMFCTasksPane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCTasksPane__IEAAHPEAVCDC___Z(
    CMFCTasksPane* pThis, CDC* /*pDC*/)
{
    if (!pThis) return FALSE;
    return TRUE;
}
// CMFCTasksPane::OnGetFont() -- retail (RVA 0x146ea0) returns m_hFont (0x550)
// when set, otherwise GetStockObject(DEFAULT_GUI_FONT) (0x11 via IAT
// 0x1802c6250).
// Symbol: ?OnGetFont@CMFCTasksPane@@IEAAPEAUHFONT__@@XZ
extern "C" HFONT MS_ABI impl__OnGetFont_CMFCTasksPane__IEAAPEAUHFONT____XZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return nullptr;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    if (self->m_hFont != nullptr) {
        return self->m_hFont;
    }
    return static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
}
// CMFCTasksPane::OnSetFont(CFont*, int) -- retail (RVA 0x146e50) stores
// pFont->m_hObject (CFont +8, or null for a null font) into m_hFont (0x550),
// then rebuilds the three derived fonts (internal helper 0x146ec0), refreshes
// the layout (0x148200) and tail-calls the virtual RecalcLayout (vtable slot
// 0x7d0). The derived-font rebuild is not reproduced; the stored handle and
// the layout refresh are.
// TODO(clean-room): partially transcribed -- m_fontBold / m_fontBoldUnderline
// / m_fontUnderline are not rebuilt.
// Symbol: ?OnSetFont@CMFCTasksPane@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCTasksPane__IEAAXPEAVCFont__H_Z(
    CMFCTasksPane* pThis, CFont* pFont, int /*bRedraw*/)
{
    if (!pThis) return;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    self->m_hFont = pFont
        ? *reinterpret_cast<HFONT*>(reinterpret_cast<char*>(pFont) + 8)
        : nullptr;
    RefreshPane(pThis);
}
// CMFCTasksPane::OnSetText(const wchar_t*) -- retail (RVA 0x14bb00) first
// calls the base CWnd::OnSetText (0x18028ac80) and propagates its return
// value, then copies the text into m_strCaption (0x678, CString). The base
// handler is not exported, so it is approximated with SetWindowTextW on
// m_hWnd. m_strCaption is left untouched: writing an alien heap pointer into
// the retail CString slot would be freed by the retail CString destructor
// with the wrong allocator.
// TODO(clean-room): partially transcribed -- m_strCaption is not updated.
// Symbol: ?OnSetText@CMFCTasksPane@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMFCTasksPane__IEAAHPEB_W_Z(
    CMFCTasksPane* pThis, const wchar_t* lpszText)
{
    if (!pThis) return FALSE;
    if (pThis->m_hWnd != nullptr) {
        return ::SetWindowTextW(pThis->m_hWnd, lpszText ? lpszText : L"");
    }
    return FALSE;
}
// CMFCTasksPane::OnSetAccData(int) -- retail (RVA 0x14bb50) screen-to-client
// converts the packed hit point, hit-tests a task or group caption and calls
// the target object's SetACCData virtual (vtable slot 0x28) to fill
// pThis->m_AccData (0x150). Before that it always resets the accessibility
// block: the six CStrings are emptied and the role/state/hit/rect/point
// numeric fields (0x150+0x30..+0x53) are zeroed. The task/group list walking
// needs object layout OpenMFC does not model; the accessibility reset and the
// TRUE result are reproduced faithfully.
// TODO(clean-room): partially transcribed -- the task/group SetACCData
// population is not reproduced.
// Symbol: ?OnSetAccData@CMFCTasksPane@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCTasksPane__UEAAHJ_Z(
    CMFCTasksPane* pThis, int /*nAccData*/)
{
    if (!pThis) return FALSE;

    CAccessibilityData* acc = &pThis->m_AccData;
    acc->m_strAccName.Empty();
    acc->m_strAccValue.Empty();
    acc->m_strDescription.Empty();
    acc->m_strAccKeys.Empty();
    acc->m_strAccHelp.Empty();
    acc->m_strAccDefAction.Empty();
    acc->m_nAccRole = 0;
    acc->m_bAccState = 0;
    acc->m_nAccHit = 0;
    acc->m_rectAccLocation.SetRectEmpty();
    acc->m_ptAccHit = CPoint(0, 0);

    return TRUE;
}
// CMFCTasksPane::OnLButtonDown(unsigned int, CPoint) -- retail (RVA 0x147af0)
// tracks caption buttons, starts the scroll-button timers and performs
// hit-testing against the task/group geometry. The hit-test internals
// (0x147070) and the unmodeled group objects are required.
// TODO(clean-room): partially transcribed -- hit-testing and button tracking
// are not modeled.
// Symbol: ?OnLButtonDown@CMFCTasksPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCTasksPane__IEAAXIVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnLButtonUp(unsigned int, CPoint) -- retail (RVA 0x1476f0)
// releases mouse capture, dispatches clicks on group captions / tasks and
// sends WM_COMMAND / starts the animation timer (0xec0b). Requires the
// task/group objects and the command routing OpenMFC does not model.
// TODO(clean-room): partially transcribed -- click dispatch and animation
// timer are not modeled.
// Symbol: ?OnLButtonUp@CMFCTasksPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCTasksPane__IEAAXIVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnMouseMove(unsigned int, CPoint) -- retail (RVA 0x1472f0)
// updates the hot task / group caption highlight and changes the cursor.
// Requires the hit-test helper (0x147070) and the task/group geometry.
// TODO(clean-room): partially transcribed -- hot-tracking is not modeled.
// Symbol: ?OnMouseMove@CMFCTasksPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCTasksPane__IEAAXIVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnMouseWheel(unsigned int, short, CPoint) -- retail (RVA
// 0x149b90) returns 1 when the internal animation state (0x1803be288) is
// active, otherwise scrolls the vertical scroll-bar through the DoScroll
// helper (0x147f20) using m_nVertScrollTotal (0x514) and the scroll sub-window
// (0x680), then redraws and falls back to the base handler (0x18028ac80).
// Both the animation global and the scroll sub-window are unmodeled.
// TODO(clean-room): partially transcribed -- vertical scrolling is not
// modeled.
// Symbol: ?OnMouseWheel@CMFCTasksPane@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CMFCTasksPane__IEAAHIFVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, short /*zDelta*/, CPoint /*point*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCTasksPane::OnSetCursor(CWnd*, unsigned int, unsigned int) -- retail
// (RVA 0x147220) GetCursorPos/ScreenToClient hit-tests the task under the
// pointer, and when a clickable task or a collapsible group caption is hit
// sets the visual-manager cursor (visual-manager virtuals + global 0x1803c1620),
// otherwise defers to the base handler (0x18028ac80).
// TODO(clean-room): partially transcribed -- visual-manager cursor lookup is
// not modeled.
// Symbol: ?OnSetCursor@CMFCTasksPane@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCTasksPane__IEAAHPEAVCWnd__II_Z(
    CMFCTasksPane* pThis, CWnd* /*pWnd*/, unsigned int /*nHitTest*/,
    unsigned int /*message*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCTasksPane::OnTrackCaptionButtons(CPoint) -- retail (RVA 0x1498d0)
// hit-tests the caption buttons and toggles their pressed state.
// TODO(clean-room): partially transcribed -- caption-button hit-testing is
// not modeled.
// Symbol: ?OnTrackCaptionButtons@CMFCTasksPane@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CMFCTasksPane__MEAAXVCPoint___Z(
    CMFCTasksPane* pThis, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnDrawTasks(CDC*, CRect) -- retail (RVA 0x145360) renders
// the task groups through the CMFCVisualManager virtuals (slots 0x2d8/0x2e0/
// 0x2f0/0x2f8/0x300/0x308, visual-manager accessor at 0x180009774) walking
// the group/task lists at 0x640/0x608.
// TODO(clean-room): partially transcribed -- task rendering is not modeled.
// Symbol: ?OnDrawTasks@CMFCTasksPane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawTasks_CMFCTasksPane__MEAAXPEAVCDC__VCRect___Z(
    CMFCTasksPane* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnFillBackground(CDC*, CRect) -- retail (RVA 0x1457d0)
// paints the pane background through the visual manager.
// TODO(clean-room): partially transcribed -- background rendering is not
// modeled.
// Symbol: ?OnFillBackground@CMFCTasksPane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCTasksPane__MEAAXPEAVCDC__VCRect___Z(
    CMFCTasksPane* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnPaint() -- retail (RVA 0x148620) constructs a CPaintDC,
// calls the draw virtual (vtable slot 0x4d8) and destroys it. No-op here:
// triggering a redraw from inside a paint cycle would repaint forever.
// TODO(clean-room): partially transcribed -- painting is not modeled.
// Symbol: ?OnPaint@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnCreate(CREATESTRUCTW*) -- retail (RVA 0x144530) registers
// the navigation messages (resource strings 0x427b/0x427c/0x427d/0x4280,
// RegisterWindowMessage via 0x18000db70), creates the scroll-bar sub-window
// (0x680), the toolbar (0x7B8), the history stack and the font objects, and
// returns 0 on success after the base OnCreate (0x1800403a0).
// TODO(clean-room): partially transcribed -- window/control creation is not
// modeled.
// Symbol: ?OnCreate@CMFCTasksPane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCTasksPane__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCTasksPane* pThis, void* /*lpCreateStruct*/)
{
    if (!pThis) return -1;
    return 0;
}
// CMFCTasksPane::OnDestroy() -- retail (RVA 0x1487b0) deletes every group in
// m_lstTaskGroups (0x640, count 0x658) and then tail-calls the base
// CWnd::OnDestroy (0x180044fb0). The groups are CObject-derived objects
// OpenMFC never allocates, so they must not be deleted here.
// TODO(clean-room): partially transcribed -- group deletion is not modeled.
// Symbol: ?OnDestroy@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnNeedTipText(unsigned int, NMHDR*, __int64*) -- retail
// (RVA 0x149910) verifies the tooltip window, loads the tooltip strings
// (0x427c/0x427d/0x4280), registers a navigation message and writes the text
// pointer into pNMHDR->pszText (NMHDR+0x18). Requires the tooltip sub-window
// (m_pToolTip at 0x4B8) and the string-loader (0x1802aee00).
// TODO(clean-room): partially transcribed -- tooltip wiring is not modeled.
// Symbol: ?OnNeedTipText@CMFCTasksPane@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCTasksPane__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMFCTasksPane* pThis, unsigned int /*id*/, NMHDR* /*pNMHDR*/,
    __int64* /*pResult*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCTasksPane::OnOK() -- retail (RVA 0x1499f0) walks the accessibility
// window hierarchy and posts WM_COMMAND to activate the focused task.
// TODO(clean-room): partially transcribed -- accessibility window traversal is
// not modeled.
// Symbol: ?OnOK@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnOK_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnPressOtherButton(CMFCCaptionMenuButton*, CWnd*) -- retail
// (RVA 0x148e00) attaches m_menuOther (0x5E8) to the caption button, pops the
// menu, and navigates to the last history index when the button's flag
// (pbtn+0x38) is set. Needs the CMFCCaptionMenuButton layout and the internal
// navigation global.
// TODO(clean-room): partially transcribed -- menu popup is not modeled.
// Symbol: ?OnPressOtherButton@CMFCTasksPane@@UEAAXPEAVCMFCCaptionMenuButton@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnPressOtherButton_CMFCTasksPane__UEAAXPEAVCMFCCaptionMenuButton__PEAVCWnd___Z(
    CMFCTasksPane* pThis, void* /*pbtn*/, CWnd* /*pWndOwner*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnSettingChange(unsigned int, const wchar_t*) -- retail (RVA
// 0x146ff0) calls the base CWnd::OnSettingChange (0x18028f380), refreshes the
// internal layout (0x148200) and tail-calls the virtual RecalcLayout (vtable
// slot 0x7d0). The base handler is not exported; the layout refresh is.
// Symbol: ?OnSettingChange@CMFCTasksPane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCTasksPane__IEAAXIPEB_W_Z(
    CMFCTasksPane* pThis, unsigned int /*uFlags*/, const wchar_t* /*lpszSection*/)
{
    if (!pThis) return;
    pThis->RecalcLayout();
}
// CMFCTasksPane::OnSize(unsigned int, int, int) -- retail (RVA 0x1447f0)
// calls the base CWnd::OnSize (0x18000c2a0), refreshes the internal layout
// (0x148200), tail-calls the virtual RecalcLayout (vtable slot 0x7d0) and
// ends with RedrawWindow(nullptr, nullptr, RDW_INVALIDATE|RDW_ERASE|
// RDW_UPDATENOW). The base OnSize is not exported; the recalc/redraw are
// reproduced.
// Symbol: ?OnSize@CMFCTasksPane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCTasksPane__IEAAXIHH_Z(
    CMFCTasksPane* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (!pThis) return;
    pThis->RecalcLayout();
    pThis->RedrawWindow(nullptr, nullptr, 0x105);
}
// CMFCTasksPane::OnTimer(unsigned __int64) -- retail (RVA 0x14a1a0) handles
// the scroll-button timers (0xec0b/0xec0c): GetCursorPos/ScreenToClient/
// PtInRect against m_rectScrollUp (0x5B8) and m_rectScrollDn (0x5C8) update
// m_nVertScrollOffset (0x510), plus a scroll-animation path. Requires the
// scroll sub-window and the animation state.
// TODO(clean-room): partially transcribed -- scroll-button timers are not
// modeled.
// Symbol: ?OnTimer@CMFCTasksPane@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCTasksPane__IEAAX_K_Z(
    CMFCTasksPane* pThis, unsigned __int64 /*nIDEvent*/)
{
    if (!pThis) return;
}
// Symbol: ?OnUpdateBack@CMFCTasksPane@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateBack_CMFCTasksPane__IEAAXPEAVCCmdUI___Z(CMFCTasksPane*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateClose@CMFCTasksPane@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateClose_CMFCTasksPane__IEAAXPEAVCCmdUI___Z(CMFCTasksPane*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateForward@CMFCTasksPane@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateForward_CMFCTasksPane__IEAAXPEAVCCmdUI___Z(CMFCTasksPane*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
CMFCTasksPane::CMFCTasksPane() {
    memset(_taskspane_padding, 0, sizeof(_taskspane_padding));
    EnsureTasksPaneState(this);
}
CMFCTasksPane::~CMFCTasksPane() { g_tasksPaneStates.erase(this); }
BOOL CMFCTasksPane::Create(DWORD, const RECT&, CWnd*, UINT) {
    EnsureTasksPaneState(this);
    return TRUE;
}
int CMFCTasksPane::AddTask(int nGroup, const wchar_t* lpszName, int nIcon, unsigned int uiCmdID, unsigned __int64 dwUserData) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState& group = EnsureTasksPaneGroup(state, nGroup);

    TasksPaneGroupState::TaskEntry task;
    task.label = lpszName ? lpszName : L"";
    task.icon = nIcon;
    task.commandID = uiCmdID;
    task.userData = dwUserData;
    group.tasks.push_back(std::move(task));
    return static_cast<int>(group.tasks.size() - 1);
}
void CMFCTasksPane::AddTask(int nGroup, CMFCTasksPaneTask* pTask) {
    if (!pTask) return;
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState& group = EnsureTasksPaneGroup(state, nGroup);
    auto it = std::find_if(group.tasks.begin(), group.tasks.end(),
        [pTask](const TasksPaneGroupState::TaskEntry& task) { return task.task == pTask; });
    if (it == group.tasks.end()) {
        TasksPaneGroupState::TaskEntry task;
        task.task = pTask;
        group.tasks.push_back(std::move(task));
    }
}
void CMFCTasksPane::RemoveAllTasks() {
    TasksPaneState& state = EnsureTasksPaneState(this);
    for (auto& group : state.groups) {
        group.tasks.clear();
    }
}
void CMFCTasksPane::RemoveAllTasks(int nGroup) {
    if (nGroup < 0) {
        RemoveAllTasks();
        return;
    }

    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup >= static_cast<int>(state.groups.size())) return;
    state.groups[static_cast<size_t>(nGroup)].tasks.clear();
}
void CMFCTasksPane::SetCaption(int nGroup, const wchar_t* lpszCaption) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup < 0) {
        state.caption = lpszCaption ? lpszCaption : L"";
        return;
    }
    if (nGroup >= static_cast<int>(state.groups.size())) return;
    state.groups[static_cast<size_t>(nGroup)].name = lpszCaption ? lpszCaption : L"";
}
void CMFCTasksPane::SetCaption(const wchar_t* lpszCaption) { SetCaption(-1, lpszCaption); }
BOOL CMFCTasksPane::SetGroupName(int nGroup, const wchar_t* lpszName) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup < 0 || nGroup >= static_cast<int>(state.groups.size())) return FALSE;
    state.groups[static_cast<size_t>(nGroup)].name = lpszName ? lpszName : L"";
    return TRUE;
}
int CMFCTasksPane::AddGroup(const wchar_t* lpszName, BOOL bBottomHasGripper, BOOL bSpecial, int nIcon) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState group;
    group.name = lpszName ? lpszName : L"";
    group.hasGripper = bBottomHasGripper ? TRUE : FALSE;
    group.isSpecial = bSpecial ? TRUE : FALSE;
    group.icon = nIcon;
    state.groups.push_back(std::move(group));
    return static_cast<int>(state.groups.size() - 1);
}
int CMFCTasksPane::AddGroup(int nGroup, const wchar_t* lpszName, BOOL bBottomHasGripper, BOOL bSpecial, HICON hIcon) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState group;
    group.name = lpszName ? lpszName : L"";
    group.hasGripper = bBottomHasGripper ? TRUE : FALSE;
    group.isSpecial = bSpecial ? TRUE : FALSE;
    group.hIcon = hIcon;

    if (nGroup >= 0 && nGroup <= static_cast<int>(state.groups.size())) {
        state.groups.insert(state.groups.begin() + nGroup, std::move(group));
        return nGroup;
    }

    state.groups.push_back(std::move(group));
    return static_cast<int>(state.groups.size() - 1);
}
