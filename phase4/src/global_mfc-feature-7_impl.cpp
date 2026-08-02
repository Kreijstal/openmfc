// CMFCRibbonBar exports implemented for the wave2_mfc-feature-7 batch.
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump) rather than inferred, and member offsets referenced below are the
// ones declared in include/openmfc/afxmfc.h (pinned by static_asserts in
// phase4/src/mfccore.cpp):
//
//   m_dwHideFlags                  1144 (0x478)
//   m_pMainButton                  1152 (0x480)
//   m_pHighlighted                 1160 (0x488)
//   m_pPressed                     1168 (0x490)
//   m_TabElements                  1176 (0x498)  opaque CMFCRibbonButtonsGroup
//   m_pActiveCategory              2824 (0xb08)
//   m_arCategories                 2896 (0xb50)  opaque CArray blob
//   m_arKeyElements                2936 (0xb80)  opaque CArray blob
//   m_sizeMainButton               3064 (0xbf8)
//   m_pKeyboardNavLevelParent      3080 (0xc08)
//   m_pKeyboardNavLevelCurrent     3088 (0xc10)
//   m_CaptionButtons               3096 (0xc18)  opaque group blob
//   m_QAToolbar                    4992 (0x1380) opaque group blob
//   m_bSingleLevelAccessibilityMode 6736
//   m_Tabs                         6744 (0x1a58) opaque CMFCRibbonTabsGroup
//
// Most of these handlers walk the four opaque embedded group blobs
// (m_TabElements / m_CaptionButtons / m_QAToolbar / m_Tabs), the undeclared
// tab-group counter at +0x410 and the tooltip/size machinery behind the
// private messages 0x407/0x418/0x41c. Those functions get type-correct
// conservative bodies with a TODO(clean-room) note, exactly like the earlier
// ribbon batches. The ones that only need the ribbon_state.h side tables or
// declared members (category management, PreCreateWindow, OnSize's hide-flag
// logic, SetApplicationButton, SetMaximizeMode tail) are implemented against
// that state so they behave instead of reading uninitialized retail-shaped
// blobs.
//
// Retail addresses for the folded CWnd default handlers: 0x18028ac80 is the
// COMDAT-folded "CWnd::OnXxx() { Default(); }" used as the base of
// OnShowWindow/OnSysCommand/OnTimer/OnSizing; ::DefWindowProcW is its exact
// semantic equivalent and is used below instead of those internal addresses.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "ribbon_state.h"

#include <mutex>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

using openmfc::ribbon_state::RibbonMutex;
using openmfc::ribbon_state::RibbonBarStates;
using openmfc::ribbon_state::RibbonBarState;

//---------------------------------------------------------------------------
// Cross-translation-unit helpers.
//---------------------------------------------------------------------------

// CMFCRibbonBar::ForceRecalcLayout -- defined in global_mfc-feature-6_impl.cpp
// (wave-1 batch); used by OnSettingChange below.
extern "C" void MS_ABI impl__ForceRecalcLayout_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis);

// CWnd::PreCreateWindow -- defined in wincore.cpp; base of PreCreateWindow.
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs);

// CMFCRibbonCategory::~CMFCRibbonCategory -- defined in cbarcore.cpp; removes
// the category from the ribbon side tables and runs the real destructor.
extern "C" void MS_ABI impl___1CMFCRibbonCategory__UEAA_XZ(void* pThis);

// CMFCRibbonBar::RecalcLayout (this file) -- the vtable+0x430 dispatch used by
// OnSize / SetMaximizeMode resolves to this override in retail.
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(
    CMFCRibbonBar* pThis);

// CMFCRibbonBar::SetKeyboardNavigationLevel (this file) -- timer 0xec1a fires
// this as a delayed key-tip navigation reset.
extern "C" void MS_ABI impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(
    CMFCRibbonBar* pThis, CObject* pObj, int nLevel);

//=============================================================================
// Window messages
//=============================================================================

// CMFCRibbonBar::OnSettingChange(UINT, const wchar_t*) -- retail (RVA 0xdfe50)
// calls the CWnd base handler first, then for nFlags in {0x22 SPI_SETDESKWALLPAPER,
// 0x2a SPI_SETNONCLIENTMETRICS, 0x2f SPI_SETWORKAREA} (bitmask 0x840400000000
// tested with `bt`) calls its own ForceRecalcLayout. The flags match the
// system-wide settings that change font/metrics and force a full ribbon relayout.
// Symbol: ?OnSettingChange@CMFCRibbonBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCRibbonBar__IEAAXIPEB_W_Z(
    CMFCRibbonBar* pThis, unsigned int nFlags, const wchar_t* lpszNewValue)
{
    if (!pThis) return;

    pThis->CWnd::OnSettingChange(nFlags, lpszNewValue);

    switch (nFlags) {
        case 0x22: // SPI_SETDESKWALLPAPER
        case 0x2a: // SPI_SETNONCLIENTMETRICS
        case 0x2f: // SPI_SETWORKAREA
            impl__ForceRecalcLayout_CMFCRibbonBar__QEAAXXZ(pThis);
            break;
        default:
            break;
    }
}

// CMFCRibbonBar::OnShowRibbonContextMenu(CWnd*, int, int,
// CMFCRibbonBaseElement*) -- retail (RVA 0xdeef0) starts by setting the
// keyboard navigation level, kills the auto-command timer when m_bAutoCommandTimer
// is set, then walks the element/context-menu machinery to build and track the
// popup (GetActiveMenu / frame forwarding). The popup-menu tracking that would
// make the return value meaningful is not modeled.
// TODO(clean-room): partially transcribed -- the popup tracking chain is not
// modeled.
// Symbol: ?OnShowRibbonContextMenu@CMFCRibbonBar@@UEAAHPEAVCWnd@@HHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__OnShowRibbonContextMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CWnd* /*pWnd*/, int /*x*/, int /*y*/,
    CMFCRibbonBaseElement* /*pElem*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonBar::OnShowRibbonQATMenu(CWnd*, int, int,
// CMFCRibbonBaseElement*) -- retail (RVA 0xdf8e0) saves m_bIsCustomizeMenu,
// sets it TRUE, dispatches the bar's virtual at vtable+0x6d8 (the QAT menu
// builder), then restores the flag. OpenMFC does not model the vtable layout,
// so the dispatch cannot be reproduced; the flag save/restore is kept.
// TODO(clean-room): partially transcribed -- the vtable+0x6d8 dispatch is not
// modeled.
// Symbol: ?OnShowRibbonQATMenu@CMFCRibbonBar@@UEAAHPEAVCWnd@@HHPEAVCMFCRibbonBaseElement@@@Z
extern "C" int MS_ABI impl__OnShowRibbonQATMenu_CMFCRibbonBar__UEAAHPEAVCWnd__HHPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CWnd* /*pWnd*/, int /*x*/, int /*y*/,
    CMFCRibbonBaseElement* /*pElem*/)
{
    if (!pThis) return 0;

    int nSaved = pThis->m_bIsCustomizeMenu;
    pThis->m_bIsCustomizeMenu = TRUE;
    // Retail: call of the vtable+0x6d8 virtual would go here.
    pThis->m_bIsCustomizeMenu = nSaved;
    return 0;
}

// CMFCRibbonBar::OnShowWindow(int, UINT) -- retail (RVA 0xe37b0) calls the
// folded CWnd default, then, when hiding a bar with a transparent caption
// (bShow == 0 && m_bIsTransparentCaption), repaints the parent frame caption
// through GetParent() -> FromHandlePermanent() -> an invalidate helper
// (0x1801cad40). The base behaviour is reproduced with DefWindowProcW; the
// frame-caption repaint is unmodeled.
// TODO(clean-room): partially transcribed -- the transparent-caption parent
// repaint flow is not modeled.
// Symbol: ?OnShowWindow@CMFCRibbonBar@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCRibbonBar__IEAAXHI_Z(
    CMFCRibbonBar* pThis, int bShow, unsigned int nStatus)
{
    if (!pThis) return;

    if (pThis->m_hWnd) {
        ::DefWindowProcW(pThis->m_hWnd, WM_SHOWWINDOW,
                         static_cast<WPARAM>(bShow), static_cast<LPARAM>(nStatus));
    }
    if (bShow == 0 && pThis->m_bIsTransparentCaption) {
        // Retail repaints the parent's caption area here.
    }
}

// CMFCRibbonBar::OnSize(UINT, int, int) -- retail (RVA 0xda1d0) calls the
// CWnd base handler, updates m_bIsMaximized from IsZoomed(GetParent()), then
// toggles bit 1 of m_dwHideFlags whenever the "currently hidden" state stops
// matching "client width < 300" (hiding the category strip on very narrow
// bars). When no toggle is needed it dispatches the RecalcLayout virtual
// (vtable+0x430) and sets m_bForceRedraw if the maximized state changed; when
// a toggle happens it clears the main button rect (+0xc8, unmodeled). Both
// paths tail-call an internal tooltip-update helper (0x1800e15b0, unmodeled).
// TODO(clean-room): partially transcribed -- main-button rect clear and the
// tooltip-update tail are not modeled.
// Symbol: ?OnSize@CMFCRibbonBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCRibbonBar__IEAAXIHH_Z(
    CMFCRibbonBar* pThis, unsigned int /*nType*/, int cx, int /*cy*/)
{
    if (!pThis) return;

    BOOL bOldMaximized = pThis->m_bIsMaximized;
    HWND hParent = pThis->m_hWnd ? ::GetParent(pThis->m_hWnd) : nullptr;
    BOOL bParentZoomed = (hParent && ::IsZoomed(hParent)) ? TRUE : FALSE;
    pThis->m_bIsMaximized = bParentZoomed;

    const bool bHideFlagSet = (pThis->m_dwHideFlags & 0x2) != 0;
    const bool bNarrow = (cx < 300);
    if (bHideFlagSet != bNarrow) {
        // Retail: set bit 1 of m_dwHideFlags when the bar becomes narrow,
        // clear it when it becomes wide again.
        if (bNarrow) {
            pThis->m_dwHideFlags |= 0x2;
        } else {
            pThis->m_dwHideFlags &= ~0x2u;
        }
        return;
    }

    if (bParentZoomed != bOldMaximized) {
        pThis->m_bForceRedraw = TRUE;
    }
    // Retail dispatches the RecalcLayout virtual here.
    impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(pThis);
}

// CMFCRibbonBar::OnSizing(UINT, RECT*) -- retail (RVA 0xdf930) forwards a
// message to a global singleton layout window (0x1803be288) and tail-calls the
// folded CWnd default. Neither the singleton nor its constraint logic is
// modeled; leaving the size unconstrained is the safe equivalent.
// TODO(clean-room): transcribed partially -- the singleton constraint forward
// is not modeled.
// Symbol: ?OnSizing@CMFCRibbonBar@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CMFCRibbonBar__IEAAXIPEAUtagRECT___Z(
    CMFCRibbonBar* pThis, unsigned int /*nSide*/, RECT* /*lpRect*/)
{
    if (!pThis) return;
}

// CMFCRibbonBar::OnSysColorChange() -- retail (RVA 0xe1120) calls the
// CPane/CWnd base handler (InvalidateRect), notifies the application object
// through a vtable dispatch, and repaints the whole bar with
// RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_FRAME (0x505). The app-object
// notification is unmodeled; the repaints are reproduced.
// TODO(clean-room): partially transcribed -- the CWinApp virtual notification
// is not modeled.
// Symbol: ?OnSysColorChange@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCRibbonBar__IEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    // Base chain (CPane::OnSysColorChange -> CWnd::OnSysColorChange).
    pThis->CWnd::OnSysColorChange();

    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}

// CMFCRibbonBar::OnSysCommand(UINT, __int64) -- retail (RVA 0xe12f0): when
// m_bIsTransparentCaption is clear it tail-calls the folded CWnd default.
// Otherwise it forwards WM_SYSCOMMAND to the parent window, converting
// SC_MAXIMIZE (0xf030) to SC_RESTORE (0xf120) when the parent is already
// zoomed so the caption button toggles. The FromHandlePermanent round trip
// only serves to obtain the parent HWND and is collapsed into GetParent().
// Symbol: ?OnSysCommand@CMFCRibbonBar@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMFCRibbonBar__IEAAXI_J_Z(
    CMFCRibbonBar* pThis, unsigned int nID, __int64 lParam)
{
    if (!pThis) return;

    if (!pThis->m_bIsTransparentCaption) {
        if (pThis->m_hWnd) {
            ::DefWindowProcW(pThis->m_hWnd, WM_SYSCOMMAND,
                             static_cast<WPARAM>(nID), static_cast<LPARAM>(lParam));
        }
        return;
    }

    unsigned int nCmd = nID;
    if (nCmd == 0xf030) { // SC_MAXIMIZE
        HWND hParent = pThis->m_hWnd ? ::GetParent(pThis->m_hWnd) : nullptr;
        nCmd = (hParent && ::IsZoomed(hParent)) ? 0xf120 /*SC_RESTORE*/ : 0xf030;
    }
    HWND hParent = pThis->m_hWnd ? ::GetParent(pThis->m_hWnd) : nullptr;
    if (hParent) {
        ::SendMessageW(hParent, WM_SYSCOMMAND,
                       static_cast<WPARAM>(nCmd), static_cast<LPARAM>(lParam));
    }
}

// CMFCRibbonBar::OnSysKeyDown(CFrameWnd*, WPARAM, LPARAM) -- retail
// (RVA 0xe3580): VK_SPACE (0x20) computes a point at (client.left+4,
// client.bottom+23), feeds it to an internal mouse helper (0x1800deb10),
// removes all key tips and kills timer 0xec1a, returning 0. When key tips are
// disabled it consumes VK_F10 (0x79) and VK_MENU (0x12). With key tips
// enabled it reads GetKeyState(VK_CONTROL)/GetKeyState(VK_SHIFT) and drives
// the unmodeled key-tip navigation.
// TODO(clean-room): partially transcribed -- key-tip navigation and the
// internal mouse helper are not modeled.
// Symbol: ?OnSysKeyDown@CMFCRibbonBar@@QEAAHPEAVCFrameWnd@@_K_J@Z
extern "C" int MS_ABI impl__OnSysKeyDown_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(
    CMFCRibbonBar* pThis, CFrameWnd* /*pFrame*/, unsigned __int64 nChar,
    __int64 /*lParam*/)
{
    if (!pThis) return 0;

    if (nChar == 0x20) { // VK_SPACE
        // Retail: internal mouse-move helper at (left+4, bottom+23), then
        // RemoveAllKeys and KillTimer(0xec1a).
        pThis->KillTimer(0xec1a);
        return 0;
    }
    if (!pThis->m_bKeyTips) {
        return (nChar == 0x79 || nChar == 0x12) ? 1 : 0;
    }
    return 0;
}

// CMFCRibbonBar::OnSysKeyUp(CFrameWnd*, WPARAM, LPARAM) -- retail
// (RVA 0xe3710): when key tips are disabled it consumes VK_F10 and VK_MENU,
// returning 1. With key tips enabled it kills timer 0xec1a and, on VK_MENU,
// restores keyboard focus (GetFocus/FromHandlePermanent chain, unmodeled) and
// repaints the bar (0x105).
// TODO(clean-room): partially transcribed -- the focus-restore chain is not
// modeled.
// Symbol: ?OnSysKeyUp@CMFCRibbonBar@@QEAAHPEAVCFrameWnd@@_K_J@Z
extern "C" int MS_ABI impl__OnSysKeyUp_CMFCRibbonBar__QEAAHPEAVCFrameWnd___K_J_Z(
    CMFCRibbonBar* pThis, CFrameWnd* /*pFrame*/, unsigned __int64 nChar,
    __int64 /*lParam*/)
{
    if (!pThis) return 0;

    if (!pThis->m_bKeyTips) {
        return (nChar == 0x79 || nChar == 0x12) ? 1 : 0;
    }
    pThis->KillTimer(0xec1a);
    if (nChar == 0x12) { // VK_MENU
        // Retail: restore focus to the bar or frame, then repaint.
        pThis->RedrawWindow(nullptr, nullptr, 0x105);
        return 1;
    }
    return 0;
}

// CMFCRibbonBar::OnTimer(WPARAM) -- retail (RVA 0xe02d0): timer 0xec19 is the
// auto-command timer -- when m_pPressed is set it hit-tests the cursor against
// the pressed element's rect (+0xc8) and an element virtual at vtable+0x2b0,
// killing the timer when the repeat finishes. Timer 0xec1a resets the
// keyboard-navigation level (SetKeyboardNavigationLevel) and kills itself.
// The pressed-element rect/vtable are not modeled, so only the 0xec1a path is
// reproduced; the base CWnd default is DefWindowProcW.
// TODO(clean-room): partially transcribed -- the 0xec19 auto-repeat path
// needs the element rect at +0xc8.
// Symbol: ?OnTimer@CMFCRibbonBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCRibbonBar__IEAAX_K_Z(
    CMFCRibbonBar* pThis, unsigned __int64 nIDEvent)
{
    if (!pThis) return;

    if (nIDEvent == 0xec19) {
        if (pThis->m_pPressed) {
            // Retail: GetCursorPos + ScreenToClient + PtInRect against the
            // pressed element rect (+0xc8); unmodeled.
        }
    } else if (nIDEvent == 0xec1a) {
        impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(
            pThis, nullptr, 0);
        pThis->KillTimer(0xec1a);
    }

    // Base CWnd::OnTimer -> folded default -> DefWindowProcW.
    if (pThis->m_hWnd) {
        ::DefWindowProcW(pThis->m_hWnd, WM_TIMER,
                         static_cast<WPARAM>(nIDEvent), 0);
    }
}

// CMFCRibbonBar::OnUpdateCmdUI(CFrameWnd*, BOOL) -- retail (RVA 0xddc30)
// dispatches the active category's OnUpdateCmdUI virtual, then the QAT group
// (m_QAToolbar, +0x1380) and the tab group (m_TabElements, +0x498) update
// virtuals. All four objects are unmodeled opaque blobs.
// TODO(clean-room): transcribed partially -- the group/category update
// virtuals are not modeled.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonBar__MEAAXPEAVCFrameWnd__H_Z(
    CMFCRibbonBar* pThis, CFrameWnd* /*pFrame*/, int /*bDisableIfNoHndler*/)
{
    if (!pThis) return;
}

// CMFCRibbonBar::OnUpdateToolTips(WPARAM, LPARAM) -- retail (RVA 0xde7f0):
// when (wParam & 0x200) it walks the tooltip group behind +0xc00 and sends the
// private message 0x418 to its window, then continues the tooltip layout
// machinery. The +0xc00 tooltip group is not modeled.
// TODO(clean-room): transcribed partially -- the tooltip group is not modeled.
// Symbol: ?OnUpdateToolTips@CMFCRibbonBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCRibbonBar__IEAA_J_K_J_Z(
    CMFCRibbonBar* pThis, unsigned __int64 /*wParam*/, __int64 /*lParam*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonBar::PreTranslateMessage(MSG*) -- retail (RVA 0xde560) forwards
// key/mouse messages (WM_KEYDOWN, WM_SYSKEYDOWN, WM_MOUSEMOVE, WM_LBUTTONDOWN/
// UP, WM_RBUTTONDOWN/UP, WM_MBUTTONDOWN/UP) to a hidden tooltip window behind
// +0xc00 via the private message 0x407, and on WM_LBUTTONDOWN runs a
// focus-management chain. The +0xc00 tooltip state is not modeled, so the
// function reports "not handled", which is also the retail fall-through value.
// TODO(clean-room): partially transcribed -- the +0xc00 tooltip window and the
// focus chain are not modeled.
// Symbol: ?PreTranslateMessage@CMFCRibbonBar@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCRibbonBar__UEAAHPEAUtagMSG___Z(
    CMFCRibbonBar* pThis, MSG* /*pMsg*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonBar::PopTooltip() -- retail (RVA 0xde8b0) reads the tooltip
// object behind +0xc00 and, when present, sends the private message 0x41c
// (WM_POPUPTOOLTIP) to its window (+0x40). The tooltip object is not modeled.
// TODO(clean-room): transcribed partially -- the tooltip object behind +0xc00
// is not modeled.
// Symbol: ?PopTooltip@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__PopTooltip_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}

//=============================================================================
// Window creation
//=============================================================================

// CMFCRibbonBar::PreCreateWindow(CREATESTRUCTW&) -- retail (RVA 0xd9fd0)
// clears style bits 0x80..0x800 (the pane alignment/visibility bits, mask
// 0xfffff07f) from m_dwStyle (0x104) and tail-calls CWnd::PreCreateWindow.
// Symbol: ?PreCreateWindow@CMFCRibbonBar@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCRibbonBar__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMFCRibbonBar* pThis, CREATESTRUCTW& cs)
{
    if (!pThis) return FALSE;

    pThis->m_dwStyle &= 0xfffff07f;
    return impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}

//=============================================================================
// Keyboard navigation
//=============================================================================

// CMFCRibbonBar::ProcessKey(int) -- retail (RVA 0xe2f30) iterates the key-tip
// elements in m_arKeyElements (CArray at +0xb80) and invokes each key tip's
// matching virtual. OpenMFC has no key-tip state, so nothing can ever match.
// TODO(clean-room): partially transcribed -- key tips are not modeled.
// Symbol: ?ProcessKey@CMFCRibbonBar@@IEAAHH@Z
extern "C" int MS_ABI impl__ProcessKey_CMFCRibbonBar__IEAAHH_Z(
    CMFCRibbonBar* pThis, int /*nKey*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonBar::SetKeyboardNavigationLevel(CObject*, int) -- retail
// (RVA 0xe1db0) returns immediately when key tips are disabled or the bar is
// hidden (m_dwHideFlags bit 1); otherwise it removes all key tips, resets the
// navigation cursor (m_nCurrKeyChar, m_pKeyboardNavLevelParent,
// m_pKeyboardNavLevelCurrent, m_nKeyboardNavLevel) and drives the key-tip
// show/hide machinery through GetFocus/IsChild checks and the category groups.
// The declared-member cursor reset is reproduced; the key-tip machinery is
// not.
// TODO(clean-room): partially transcribed -- the key-tip show/hide and focus
// checks are not modeled.
// Symbol: ?SetKeyboardNavigationLevel@CMFCRibbonBar@@QEAAXPEAVCObject@@H@Z
extern "C" void MS_ABI impl__SetKeyboardNavigationLevel_CMFCRibbonBar__QEAAXPEAVCObject__H_Z(
    CMFCRibbonBar* pThis, CObject* pObj, int /*nLevel*/)
{
    if (!pThis) return;
    if (!pThis->m_bKeyTips) return;
    if ((pThis->m_dwHideFlags & 0x2) != 0) return;

    pThis->m_nCurrKeyChar = 0;
    pThis->m_pKeyboardNavLevelParent = nullptr;
    pThis->m_pKeyboardNavLevelCurrent = pObj;
    pThis->m_nKeyboardNavLevel = 0;
}

//=============================================================================
// Layout
//=============================================================================

// CMFCRibbonBar::RecalcLayout() -- retail (RVA 0xdc480) is a large geometry
// pass over the main category, the caption buttons, the QAT and the tab group
// (including a hidden-element sweep via 0x410 and CleanUpSizes). None of that
// geometry is modeled; a no-op is the safe behaviour because nothing has been
// laid out in the first place.
// TODO(clean-room): transcribed partially -- ribbon geometry is not modeled.
// Symbol: ?RecalcLayout@CMFCRibbonBar@@UEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}

//=============================================================================
// Category management
//=============================================================================

// CMFCRibbonBar::RemoveAllCategories() -- retail (RVA 0xdb150) clears
// m_pPrintPreviewCategory when it is one of the removed categories, deletes
// every category in m_arCategories (virtual dtor + operator delete) and every
// context caption, then RemoveAll on both arrays. Implemented over the ribbon
// side table, which is where OpenMFC keeps the category list; context captions
// do not exist here.
// Symbol: ?RemoveAllCategories@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllCategories_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    std::vector<CMFCRibbonCategory*> categories;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonBarStates().find(pThis);
        if (it == RibbonBarStates().end()) return;
        RibbonBarState& state = it->second;
        categories = state.categories;
        state.categories.clear();
        state.ownedCategories.clear();
        state.activeCategory = nullptr;
    }

    for (CMFCRibbonCategory* pCategory : categories) {
        if (!pCategory) continue;
        impl___1CMFCRibbonCategory__UEAA_XZ(pCategory);
        ::operator delete(pCategory);
    }
}

// CMFCRibbonBar::RemoveCategory(int) -- retail (RVA 0xdaf90) bounds-checks the
// index, deletes the category (virtual dtor + operator delete), shifts the
// array (memmove) and, when the active category was removed, reselects the
// category at min(nIndex, count-1) if it is visible (+0xa8) or the first
// visible category, clearing the active pointer if none is visible. Implemented
// over the ribbon side table; every OpenMFC category is visible (no +0xa8
// member), so the first candidate always wins.
// Symbol: ?RemoveCategory@CMFCRibbonBar@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveCategory_CMFCRibbonBar__QEAAHH_Z(
    CMFCRibbonBar* pThis, int nIndex)
{
    if (!pThis || nIndex < 0) return 0;

    CMFCRibbonCategory* pRemoved = nullptr;
    {
        std::lock_guard<std::mutex> lock(RibbonMutex());
        auto it = RibbonBarStates().find(pThis);
        if (it == RibbonBarStates().end()) return 0;
        RibbonBarState& state = it->second;
        if (nIndex >= static_cast<int>(state.categories.size())) return 0;

        pRemoved = state.categories[static_cast<size_t>(nIndex)];
        state.categories.erase(state.categories.begin() + nIndex);
        state.ownedCategories.erase(pRemoved);

        if (state.activeCategory == pRemoved) {
            if (state.categories.empty()) {
                state.activeCategory = nullptr;
            } else {
                size_t pick = static_cast<size_t>(nIndex);
                if (pick >= state.categories.size()) {
                    pick = state.categories.size() - 1;
                }
                state.activeCategory = state.categories[pick];
            }
        }
    }

    if (pRemoved) {
        impl___1CMFCRibbonCategory__UEAA_XZ(pRemoved);
        ::operator delete(pRemoved);
    }
    return 1;
}

//=============================================================================
// Tabs
//=============================================================================

// CMFCRibbonBar::RemoveAllFromTabs() -- retail (RVA 0xde250) clears the
// m_TabElements group (CArray RemoveAll at +0x498). OpenMFC tracks the tab
// elements in the ribbon side table, so the equivalent is clearing that list.
// Symbol: ?RemoveAllFromTabs@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllFromTabs_CMFCRibbonBar__QEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;

    std::lock_guard<std::mutex> lock(RibbonMutex());
    auto it = RibbonBarStates().find(pThis);
    if (it == RibbonBarStates().end()) return;
    it->second.tabs.clear();
}

// CMFCRibbonBar::SetActiveMDIChild(CWnd*) -- retail (RVA 0xe0250) iterates the
// tab elements (m_TabElements +0x620/+0x628) and writes pWnd's m_hWnd into
// each tab that is kind-of the MDI tab group (element +0x270). The tab group
// and the element slot are not modeled.
// TODO(clean-room): transcribed partially -- the tab element group and its
// +0x270 MDI-child slot are not modeled.
// Symbol: ?SetActiveMDIChild@CMFCRibbonBar@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetActiveMDIChild_CMFCRibbonBar__QEAAXPEAVCWnd___Z(
    CMFCRibbonBar* pThis, CWnd* /*pWnd*/)
{
    if (!pThis) return;
}

//=============================================================================
// Key tips
//=============================================================================

// CMFCRibbonBar::RemoveAllKeys() -- retail (RVA 0xe3160) removes and destroys
// every key tip in m_arKeyElements (CArray at +0xb80). OpenMFC has no key-tip
// state; the array is an opaque blob nothing populates.
// TODO(clean-room): partially transcribed -- key tips are not modeled.
// Symbol: ?RemoveAllKeys@CMFCRibbonBar@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveAllKeys_CMFCRibbonBar__IEAAXXZ(
    CMFCRibbonBar* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonBar::SetElementKeys(UINT, const wchar_t*, const wchar_t*) --
// retail (RVA 0xddfa0) collects all elements with the given command ID and
// assigns each its key tip (element virtual at vtable+0x180); it returns
// FALSE when no element matched, otherwise TRUE. The key-tip assignment
// virtual is not modeled and no element key tips exist in OpenMFC.
// TODO(clean-room): transcribed partially -- key-tip assignment is not
// modeled.
// Symbol: ?SetElementKeys@CMFCRibbonBar@@QEAAHIPEB_W0@Z
extern "C" int MS_ABI impl__SetElementKeys_CMFCRibbonBar__QEAAHIPEB_W0_Z(
    CMFCRibbonBar* pThis, unsigned int /*uiCmd*/, const wchar_t* /*lpszKeys*/,
    const wchar_t* /*lpszDescr*/)
{
    if (!pThis) return 0;
    return 0;
}

//=============================================================================
// State persistence
//=============================================================================

// CMFCRibbonBar::SaveState(const wchar_t*, int, UINT) -- retail (RVA 0xdf970)
// writes the bar's minimized state, active category index and QAT commands to
// the registry through CWinAppEx. Registry-backed ribbon state is not modeled.
// TODO(clean-room): transcribed partially -- registry state is not modeled.
// Symbol: ?SaveState@CMFCRibbonBar@@MEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCRibbonBar__MEAAHPEB_WHI_Z(
    CMFCRibbonBar* pThis, const wchar_t* /*lpszProfileName*/, int /*nIndex*/,
    unsigned int /*uiID*/)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonBar::SaveToXMLBuffer(unsigned char**) -- retail (RVA 0xe4640)
// serializes the whole ribbon into an XML writer, detaches the buffer into
// *ppBuffer and returns its length. OpenMFC has no ribbon XML writer.
// TODO(clean-room): transcribed partially -- the XML serializer is not
// modeled.
// Symbol: ?SaveToXMLBuffer@CMFCRibbonBar@@QEBAIPEAPEAE@Z
extern "C" unsigned int MS_ABI impl__SaveToXMLBuffer_CMFCRibbonBar__QEBAIPEAPEAE_Z(
    const CMFCRibbonBar* pThis, unsigned char** ppBuffer)
{
    if (!pThis) return 0;
    if (ppBuffer) *ppBuffer = nullptr;
    return 0;
}

// CMFCRibbonBar::SaveToXMLFile(const wchar_t*) -- retail (RVA 0xe45a0)
// calls SaveToXMLBuffer and writes the returned buffer to the file. With no
// XML serializer the save always fails.
// TODO(clean-room): transcribed partially -- the XML serializer is not
// modeled.
// Symbol: ?SaveToXMLFile@CMFCRibbonBar@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__SaveToXMLFile_CMFCRibbonBar__QEBAHPEB_W_Z(
    const CMFCRibbonBar* pThis, const wchar_t* /*lpszFileName*/)
{
    if (!pThis) return 0;
    return 0;
}

//=============================================================================
// Application button / maximize mode
//=============================================================================

// CMFCRibbonBar::SetApplicationButton(CMFCRibbonApplicationButton*, CSize) --
// retail (RVA 0xda2e0): stores the button into m_pMainButton (+0x480), sets
// the button's back-pointer to the bar (+0xd8, undeclared, skipped) and
// stores the size into m_sizeMainButton (+0xbf8); a null button zeroes the
// size. CMFCRibbonApplicationButton is not declared in OpenMFC, so the
// parameter is taken as void*.
// TODO(clean-room): partially transcribed -- the button back-pointer at +0xd8
// is not modeled.
// Symbol: ?SetApplicationButton@CMFCRibbonBar@@QEAAXPEAVCMFCRibbonApplicationButton@@VCSize@@@Z
extern "C" void MS_ABI impl__SetApplicationButton_CMFCRibbonBar__QEAAXPEAVCMFCRibbonApplicationButton__VCSize___Z(
    CMFCRibbonBar* pThis, void* pButton, CSize size)
{
    if (!pThis) return;

    pThis->m_pMainButton = static_cast<CMFCRibbonButton*>(pButton);
    if (pButton != nullptr) {
        pThis->m_sizeMainButton = size;
    } else {
        pThis->m_sizeMainButton = CSize(0, 0);
    }
}

// CMFCRibbonBar::SetMaximizeMode(BOOL, CWnd*) -- retail (RVA 0xdffa0) returns
// immediately when the mode is unchanged, otherwise it rebuilds the caption
// buttons and re-links the system menu (unmodeled), then stores the mode into
// m_bMaximizeMode, clears m_pHighlighted/m_pPressed, dispatches the
// RecalcLayout virtual and repaints the bar (0x105). The declared-member tail
// is reproduced.
// TODO(clean-room): partially transcribed -- the caption-button rebuild and
// system-menu re-linking are not modeled.
// Symbol: ?SetMaximizeMode@CMFCRibbonBar@@QEAAXHPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetMaximizeMode_CMFCRibbonBar__QEAAXHPEAVCWnd___Z(
    CMFCRibbonBar* pThis, int bMax, CWnd* /*pWnd*/)
{
    if (!pThis) return;
    if (pThis->m_bMaximizeMode == bMax) return;

    pThis->m_bMaximizeMode = bMax;
    pThis->m_pHighlighted = nullptr;
    pThis->m_pPressed = nullptr;
    impl__RecalcLayout_CMFCRibbonBar__UEAAXXZ(pThis);
    pThis->RedrawWindow(nullptr, nullptr, 0x105);
}
