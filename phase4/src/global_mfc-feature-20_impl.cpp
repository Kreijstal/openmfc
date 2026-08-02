// OpenMFC: real implementation of the CMFCTasksPane handlers exported as the
// wave1_mfc-feature-20 batch.
//
// The retail CMFCTasksPane (afxtaskspane.h) is a CDockablePane-derived
// control whose own members start at offset 0x4E0 (the CWnd + CBasePane +
// CPane + CDockablePane subobjects fill 0..0x4DF). OpenMFC's CMFCTasksPane
// carries only opaque padding, so every member access below goes through a
// layout-faithful mirror struct whose offsets were harvested from the
// shipping MSVC atlmfc headers (/d1reportSingleClassLayoutCMFCTasksPane)
// and cross-checked byte-for-byte against the retail mfc140u disassembly
// (the RVA column in each comment refers to the retail export).
//
// The small navigation/font/accessibility handlers (OnEraseBkgnd, OnGetFont,
// OnSetFont, OnPressBackButton/Forward/Home, OnPressButtons, OnSetAccData,
// OnSize, OnSettingChange) are transcribed faithfully. Anything that walks
// the task-group/task object lists (CObList at 0x608 / 0x640), the vertical
// scroll-bar sub-window (0x680), the icon list or the history stack deep
// enough to need object layout OpenMFC does not model gets a type-correct
// conservative body with a TODO(clean-room) note, exactly like the earlier
// feature batches. Internal MFC globals (the last-navigation index at
// 0x1803b1b3c, the animation state at 0x1803be288, the visual-manager
// initialized flag at 0x1803c1620) are never touched.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view of CMFCTasksPane. The base subobjects are opaque
// (CWnd m_hWnd at +0x40 is still readable through the typed CWnd base of
// CMFCTasksPane, and CBasePane::m_AccData at +0x150 is a declared member);
// only the pane's own members are mirrored here.
struct S {
    char  _base[0x4E0];                // 0x000  CWnd + CBasePane + CPane + CDockablePane

    BOOL  m_bCanCollapse;              // 0x4E0
    BOOL  m_bUseNavigationToolbar;     // 0x4E4
    BOOL  m_bHistoryMenuButtons;       // 0x4E8
    BOOL  m_bUseScrollButtons;         // 0x4EC
    BOOL  m_bAnimationEnabled;         // 0x4F0
    BOOL  m_bOffsetCustomControls;     // 0x4F4
    BOOL  m_bMenuBtnPressed;           // 0x4F8
    BOOL  m_bWrapTasks;                // 0x4FC
    BOOL  m_bWrapLabels;               // 0x500
    int   m_iActivePage;               // 0x504
    int   m_iScrollMode;               // 0x508
    int   m_iScrollBtnHeight;          // 0x50C
    int   m_nVertScrollOffset;         // 0x510
    int   m_nVertScrollTotal;          // 0x514
    int   m_nVertScrollPage;           // 0x518
    int   m_nRowHeight;                // 0x51C
    int   m_nAnimGroupExtraHeight;     // 0x520
    int   m_nVertMargin;               // 0x524
    int   m_nHorzMargin;               // 0x528
    int   m_nGroupVertOffset;          // 0x52C
    int   m_nGroupCaptionHeight;       // 0x530
    int   m_nGroupCaptionHorzOffset;   // 0x534
    int   m_nGroupCaptionVertOffset;   // 0x538
    int   m_nTasksHorzOffset;          // 0x53C
    int   m_nTasksIconHorzOffset;      // 0x540
    int   m_nTasksIconVertOffset;      // 0x544
    int   m_nMaxHistory;               // 0x548  (const int)
    UINT  m_uiToolbarBmpRes;           // 0x54C
    HFONT m_hFont;                     // 0x550
    char  m_fonts[0x30];               // 0x558  m_fontBold / m_fontBoldUnderline / m_fontUnderline (CFont, 16 each; HFONT m_hObject at +8)
    SIZE  m_sizeIcon;                  // 0x588
    SIZE  m_sizeToolbarImage;          // 0x590
    SIZE  m_sizeToolbarButton;         // 0x598
    SIZE  m_sizeAnim;                  // 0x5A0
    RECT  m_rectToolbar;               // 0x5A8
    RECT  m_rectScrollUp;              // 0x5B8
    RECT  m_rectScrollDn;              // 0x5C8
    RECT  m_rectTasks;                 // 0x5D8
    char  m_menuOther[0x10];           // 0x5E8  CMenu (HMENU m_hMenu at +8 = 0x5F0)
    void* m_pHotTask;                  // 0x5F8
    void* m_pClickedTask;              // 0x600
    char  m_lstTasksPanes[0x38];       // 0x608  CObList (head at +8 = 0x610, count at +0x18 = 0x620)
    char  m_lstTaskGroups[0x38];       // 0x640  CObList (head at +8 = 0x648, count at +0x18 = 0x658)
    wchar_t* m_strCaption;             // 0x678  CString -> single LPWSTR data pointer
    char  m_wndScrollVert[0xE8];       // 0x680  vertical scroll sub-window blob (232 bytes)
    char  m_lstIcons[0x10];            // 0x768
    void* m_arrHistoryVfptr;           // 0x778  CArray<int,int> : CObject (vtable ptr)
    const int* m_pHistoryData;         // 0x780  m_pData
    __int64    m_nHistorySize;         // 0x788  m_nSize (INT_PTR)
    __int64    m_nHistoryMaxSize;      // 0x790  m_nMaxSize
    __int64    m_nHistoryGrowBy;       // 0x798  m_nGrowBy
    void* m_pHotGroupCaption;          // 0x7A0
    void* m_pClickedGroupCaption;      // 0x7A8
    void* m_pAnimatedGroup;            // 0x7B0
    char  m_wndToolBar[0x48];          // 0x7B8  toolbar sub-window blob
};

static_assert(offsetof(S, m_bCanCollapse)          == 0x4E0, "m_bCanCollapse @0x4E0");
static_assert(offsetof(S, m_bHistoryMenuButtons)   == 0x4E8, "m_bHistoryMenuButtons @0x4E8");
static_assert(offsetof(S, m_bMenuBtnPressed)       == 0x4F8, "m_bMenuBtnPressed @0x4F8");
static_assert(offsetof(S, m_iActivePage)           == 0x504, "m_iActivePage @0x504");
static_assert(offsetof(S, m_nVertScrollTotal)      == 0x514, "m_nVertScrollTotal @0x514");
static_assert(offsetof(S, m_hFont)                 == 0x550, "m_hFont @0x550");
static_assert(offsetof(S, m_rectScrollUp)          == 0x5B8, "m_rectScrollUp @0x5B8");
static_assert(offsetof(S, m_rectScrollDn)          == 0x5C8, "m_rectScrollDn @0x5C8");
static_assert(offsetof(S, m_rectTasks)             == 0x5D8, "m_rectTasks @0x5D8");
static_assert(offsetof(S, m_pHotTask)              == 0x5F8, "m_pHotTask @0x5F8");
static_assert(offsetof(S, m_pClickedTask)          == 0x600, "m_pClickedTask @0x600");
static_assert(offsetof(S, m_strCaption)            == 0x678, "m_strCaption @0x678");
static_assert(offsetof(S, m_pHistoryData)          == 0x780, "m_pHistoryData @0x780");
static_assert(offsetof(S, m_nHistorySize)          == 0x788, "m_nHistorySize @0x788");
static_assert(offsetof(S, m_pHotGroupCaption)      == 0x7A0, "m_pHotGroupCaption @0x7A0");
static_assert(offsetof(S, m_pClickedGroupCaption)  == 0x7A8, "m_pClickedGroupCaption @0x7A8");

// Refresh idiom used by the faithful bodies below: the retail handlers call
// internal layout helpers (0x148990 / 0x148200) that are not exported, then
// either the virtual RecalcLayout (vtable slot 0x7d0) or a redraw with
// RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW (0x105). RedrawWindow on the whole
// pane is the safe equivalent partial behaviour (see feature-6 house style).
void RefreshPane(CMFCTasksPane* pThis)
{
    if (!pThis) return;
    pThis->RecalcLayout();
    pThis->RedrawWindow(nullptr, nullptr, 0x105);
}

} // namespace

// CMFCTasksPane::OnPressHomeButton -- forward-declared so OnHome (which
// tail-calls the virtual slot 0x760 that points at it) can dispatch to it.
extern "C" void MS_ABI impl__OnPressHomeButton_CMFCTasksPane__UEAAXXZ(CMFCTasksPane* pThis);

//=============================================================================
// Navigation buttons
//=============================================================================

// CMFCTasksPane::OnPressBackButton -- retail (RVA 0x148d60) decrements
// m_iActivePage when it is positive and refreshes via the internal helper at
// 0x148990. The refresh is approximated with the pane-wide redraw.
// Symbol: ?OnPressBackButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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
        S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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

//=============================================================================
// Font / text / accessibility
//=============================================================================

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
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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

//=============================================================================
// Mouse / cursor
//=============================================================================

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

//=============================================================================
// Drawing
//=============================================================================

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

//=============================================================================
// Window lifecycle / misc messages
//=============================================================================

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
