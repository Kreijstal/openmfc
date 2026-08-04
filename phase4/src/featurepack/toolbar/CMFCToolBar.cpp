// CMFCToolBar — OpenMFC implementation.
// Sources: global_mfc-feature-3_impl.cpp, global_mfc-feature-4_impl.cpp, global_mfc-feature-5_impl.cpp, global_mfctoolbar_batch1.cpp, global_mfctoolbar_batch2.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCToolBarSupport.h"
#include "detail/MfcFeature5ImplSupport.h"
#include "detail/MfctoolbarBatch2Support.h"
#include "detail/MfccoreSupport.h"

// CMFCToolBar Feature Pack exports, wave 4 ("mfc-feature-4").
//
// Each function is transcribed from the retail mfc140u export
// (ordinal -> RVA -> objdump) against the member layout documented in
// include/openmfc/afxmfc.h (CMFCToolBar own members run 0x3F8..0x1350).
// Where the retail body needs internal helpers, unmodeled globals or virtual
// slots that OpenMFC does not model, the implementation keeps the visible side
// effects (member writes, window invalidation, message posting) and returns
// the safe default, with a `// TODO(clean-room)` marker.
//
// Cross-file impl helpers referenced here:
//   thunks.cpp   : CWnd::Default / CWnd::OnSetFocus / CWnd::UpdateDialogControls
//   wincore.cpp  : CWnd::PreTranslateMessage
//   mfccore.cpp  : CTooltipManager::CreateToolTip
//   feature-3    : CMFCToolBar::OnCancelMode / OnChangeHot / OnAfterButtonDelete
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Statics with storage in feature_static_data.cpp.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;
extern "C" void*        impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA;
extern "C" void*        impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA;

// Forward declarations of exported helpers defined in sibling shards.
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI   impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, CWnd* pOldWnd);
extern "C" void MS_ABI   impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
                             CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHandler);
extern "C" int MS_ABI    impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
extern "C" int MS_ABI    impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
                             CToolTipCtrl** ppToolTip, CWnd* pParentWnd, unsigned int nType);
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot);
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis);

// Functions implemented in this file that are referenced by earlier functions.
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBar__UEAAHH_Z(CMFCToolBar* pThis, int iIndex);
extern "C" void MS_ABI impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);

//=============================================================================
// CMFCToolBar::OnSetFocus
//=============================================================================

//=============================================================================
// CMFCToolBar::OnSettingChange
//=============================================================================

//=============================================================================
// CMFCToolBar::OnShowWindow
//=============================================================================

//=============================================================================
// CMFCToolBar::OnSize
//=============================================================================

//=============================================================================
// CMFCToolBar::OnSysColorChange
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolHitTest
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarAppearance
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarDelete
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarImage
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarImageAndText
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarNewMenu
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarReset
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarStartGroup
//=============================================================================

//=============================================================================
// CMFCToolBar::OnToolbarText
//=============================================================================

//=============================================================================
// CMFCToolBar::OnUpdateCmdUI
//=============================================================================

//=============================================================================
// CMFCToolBar::OnUpdateToolTips
//=============================================================================

//=============================================================================
// CMFCToolBar::OnUserToolTip
//=============================================================================

//=============================================================================
// CMFCToolBar::OnWindowPosChanged
//=============================================================================

//=============================================================================
// CMFCToolBar::OnWindowPosChanging
//=============================================================================

//=============================================================================
// CMFCToolBar::PreTranslateMessage
//=============================================================================

//=============================================================================
// CMFCToolBar::PrevMenu
//=============================================================================

//=============================================================================
// CMFCToolBar::ProcessCommand
//=============================================================================

//=============================================================================
// CMFCToolBar::RebuildAccelerationKeys
//=============================================================================

//=============================================================================
// CMFCToolBar::RedrawCustomizeButton
//=============================================================================

//=============================================================================
// CMFCToolBar::RedrawUnderlines (static)
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveAllButtons
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveButton
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveResetStateButton
//=============================================================================

//=============================================================================
// CMFCToolBar::RemoveStateFromRegistry
//=============================================================================

//=============================================================================
// CMFCToolBar::ResetAll (static)
//=============================================================================
// CMFCToolBar methods enabled by the corrected retail layout (see the layout
// chain rebuild: CBasePane 424 / CPane 1016 / CMFCToolBar 4944). Each function
// below was decoded from the retail mfc140u export (ordinal -> RVA -> objdump),
// and the member offsets the shipping code touches are exactly the ones
// transcribed into include/openmfc/afxmfc.h:
//
//   [rcx+0x10b8] 4280 m_bLocked          [rcx+0x1134] 4404 m_iButtonCapture
//   [rcx+0x10bc] 4284 m_bLargeIconsAreEnbaled
//   [rcx+0x10c8] 4296 m_bTextLabels      [rcx+0x1138] 4408 m_iHighlighted
//   [rcx+0x10ec] 4332 m_bMenuMode        [rcx+0x12f8] 4856 m_pBrotherToolBar
//   [rcx+0x0130]  304 m_pDockBarRow (inherited from CBasePane)
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CMFCToolBar::m_bCustomizeMode is a protected static; it already has storage
// in feature_static_data.cpp under its exported name.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;






// Implementations this unit calls that are defined with their own class.
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;
extern "C" void* impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA;

// Retail (0x18014fe50):
//     if (iButton < 0 || iButton >= [0x11a0] /* m_Buttons.m_nCount */) return 0;
//     rect = 0;
//     vcall slot 0x6f0 (this, &rect, iButton);        // GetButtonRect(iButton,&rect)
//     pButton = GetButton(iButton);
//     if (pButton == m_pCustomizeBtn) { rect.right += 10; rect.bottom += 10; }
//     InvalidateRect(m_hWnd, &rect, TRUE);
//     if (pButton == m_pCustomizeBtn) { extra text-size invalidate from the
//       customize button's extended members at 0x12c/0x150 -- not modeled }
//     return pButton;
// Slot 0x6f0 and the extended CMFCToolBarCustomizeButton members are not modeled
// in the OpenMFC headers, so the button's own m_rect stands in for the rect.
// Symbol: ?InvalidateButton@CMFCToolBar@@QEAAPEAVCMFCToolBarButton@@H@Z
extern "C" CMFCToolBarButton* MS_ABI
impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(
    CMFCToolBar* pThis, int iButton)
{
    if (!pThis) return nullptr;
    if (iButton < 0 || iButton >= pThis->GetCount()) return nullptr;

    CMFCToolBarButton* pButton = pThis->GetButton(iButton);

    CRect rect(0, 0, 0, 0);
    if (pButton) {
        rect = pButton->m_rect;
        if (pButton == pThis->m_pCustomizeBtn) {
            rect.right += 10;
            rect.bottom += 10;
        }
    }
    pThis->InvalidateRect((const struct tagRECT*)&rect, TRUE);
    return pButton;
}
// Retail (0x180158c60): returns TRUE only for commands inside three hard-coded
// ID windows AND not present in two internal command lists (0x1803b2020 /
// 0x1803b1fd0). The lists are internal globals OpenMFC does not model; with them
// empty, FALSE is the honest answer for every command, so only the
// customize-mode gate is kept.
// Symbol: ?IsCommandRarelyUsed@CMFCToolBar@@SAHI@Z
extern "C" int MS_ABI impl__IsCommandRarelyUsed_CMFCToolBar__SAHI_Z(unsigned int nCmdID)
{
    (void)nCmdID;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return FALSE;
    // TODO(clean-room): partially transcribed -- internal command lists skipped.
    return FALSE;
}
// Retail (0x180156960): pButton->vslot0x80 -> IsWindow() check, then compares
// the button against the global last-command info (0x18028ac10). Not modeled.
// Symbol: ?IsLastCommandFromButton@CMFCToolBar@@SAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__IsLastCommandFromButton_CMFCToolBar__SAHPEAVCMFCToolBarButton___Z(
    CMFCToolBarButton* pButton)
{
    if (!pButton) return FALSE;
    // TODO(clean-room): not transcribed -- needs the global last-command state.
    return FALSE;
}
// Retail (0x1801596a0): consults a global pane-collection singleton
// (0x1803be1b8), two RTTI checks and an internal map lookup. Not modeled.
// Symbol: ?IsUserDefined@CMFCToolBar@@QEBAHXZ
extern "C" int MS_ABI impl__IsUserDefined_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed.
    return FALSE;
}
// Retail (0x18014e360):
//     m_bLocked = bLocked;
//     locked path (bLocked != 0) maps the eight CMFCToolBarInfo resource ids
//     onto the toolbar's own member image collections:
//         m_ImagesLocked            <- info.m_uiHotResID          (always loaded)
//         m_ColdImagesLocked        <- info.m_uiColdResID         (or copied from
//                                       m_ImagesLocked when 0 and the global
//                                       image-count flag is set)
//         m_DisabledImagesLocked    <- info.m_uiDisabledResID
//         m_LargeImagesLocked       <- info.m_uiLargeColdResID
//         m_LargeColdImagesLocked   <- info.m_uiLargeHotResID
//         m_LargeDisabledImagesLocked <- info.m_uiLargeDisabledResID
//         m_MenuImagesLocked        <- info.m_uiMenuResID
//         m_MenuImagesLocked        <- info.m_uiMenuResID again when the menu
//                                       disabled id is set (retail quirk)
//     any failed Load returns 0. The unlocked path (bLocked == 0) targets the
//     static image objects 0x1803c25a0..0x1803c3100 which are not modeled, so it
//     reports success without loading.
// Symbol: ?LoadBitmapEx@CMFCToolBar@@UEAAHAEAVCMFCToolBarInfo@@H@Z
extern "C" int MS_ABI impl__LoadBitmapEx_CMFCToolBar__UEAAHAEAVCMFCToolBarInfo__H_Z(
    CMFCToolBar* pThis, CMFCToolBarInfo& info, int bLocked)
{
    if (!pThis) return 0;
    pThis->m_bLocked = bLocked;

    if (!bLocked) {
        // TODO(clean-room): partially transcribed -- the unlocked path uses the
        // static image objects 0x1803c25a0.. which are not modeled.
        return TRUE;
    }

    if (!pThis->m_ImagesLocked.Load(info.m_uiHotResID, nullptr, TRUE)) return 0;

    if (info.m_uiColdResID != 0) {
        if (!pThis->m_ColdImagesLocked.Load(info.m_uiColdResID, nullptr, TRUE)) return 0;
    } else {
        // TODO(clean-room): retail copies m_ImagesLocked -> m_ColdImagesLocked
        // and sets its image size when the global default count is nonzero;
        // the globals 0x1803be358/0x1803be34c are not modeled.
    }

    if (info.m_uiDisabledResID != 0 &&
        !pThis->m_DisabledImagesLocked.Load(info.m_uiDisabledResID, nullptr, TRUE)) return 0;

    if (info.m_uiLargeColdResID != 0 &&
        !pThis->m_LargeImagesLocked.Load(info.m_uiLargeColdResID, nullptr, TRUE)) return 0;

    if (info.m_uiLargeHotResID != 0 &&
        !pThis->m_LargeColdImagesLocked.Load(info.m_uiLargeHotResID, nullptr, TRUE)) return 0;

    if (info.m_uiLargeDisabledResID != 0 &&
        !pThis->m_LargeDisabledImagesLocked.Load(info.m_uiLargeDisabledResID, nullptr, TRUE)) return 0;

    if (info.m_uiMenuResID != 0 &&
        !pThis->m_MenuImagesLocked.Load(info.m_uiMenuResID, nullptr, TRUE)) return 0;

    if (info.m_uiMenuDisabledResID != 0 &&
        !pThis->m_MenuImagesLocked.Load(info.m_uiMenuResID, nullptr, TRUE)) return 0;

    return TRUE;
}
// Retail (0x18014e2b0): packs the six arguments into a stack CMFCToolBarInfo
//     cold = p1  hot = p0  disabled = p4  large* = 0  menu = p2  menuDisabled = p5
// and tail-calls virtual LoadBitmapEx(this, &info, bLocked = p3).
// Symbol: ?LoadBitmapW@CMFCToolBar@@UEAAHIIIHII@Z
extern "C" int MS_ABI impl__LoadBitmapW_CMFCToolBar__UEAAHIIIHII_Z(
    CMFCToolBar* pThis, unsigned int p0, unsigned int p1, unsigned int p2,
    int p3, unsigned int p4, unsigned int p5)
{
    if (!pThis) return 0;
    CMFCToolBarInfo info = {};
    info.m_uiColdResID          = p1;
    info.m_uiHotResID           = p0;
    info.m_uiDisabledResID      = p4;
    info.m_uiMenuResID          = p2;
    info.m_uiMenuDisabledResID  = p5;
    return impl__LoadBitmapEx_CMFCToolBar__UEAAHAEAVCMFCToolBarInfo__H_Z(pThis, info, p3);
}
// Retail (0x180158520): opens registry key 0x180342a60, reads the large-icons
// value into the class static m_bLargeIcons (0x1803be368) via a CRegKey /
// CSettingsStore chain and returns it. Registry access is not modeled.
// Symbol: ?LoadLargeIconsState@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(
    const wchar_t* lpszProfileName)
{
    (void)lpszProfileName;
    // TODO(clean-room): not transcribed -- registry read skipped.
    return 0;
}
// Retail (0x18015a660): asks the CSettingsStore for the "LastOriginalState"
// profile string, loads a serialized button list and calls virtual slot 0x870.
// CSettingsStore is not declared in OpenMFC headers and the serialization format
// is not modeled.
// Symbol: ?LoadLastOriginalState@CMFCToolBar@@MEAAHAEAVCSettingsStore@@@Z
extern "C" int MS_ABI impl__LoadLastOriginalState_CMFCToolBar__MEAAHAEAVCSettingsStore___Z(
    CMFCToolBar* pThis, CSettingsStore& store)
{
    (void)pThis; (void)store;
    // TODO(clean-room): not transcribed -- CSettingsStore + serialized buttons.
    return 0;
}
// Retail (0x1801582b0): registry read of the toolbar parameter values into three
// class statics (m_bDontScaleImages @0x1803b1b64, @0x1803b1b60, m_bLargeIcons
// @0x1803be368). Registry access is not modeled.
// Symbol: ?LoadParameters@CMFCToolBar@@SAHPEB_W@Z
extern "C" int MS_ABI impl__LoadParameters_CMFCToolBar__SAHPEB_W_Z(
    const wchar_t* lpszProfileName)
{
    (void)lpszProfileName;
    // TODO(clean-room): not transcribed -- registry read skipped.
    return 0;
}
// Retail (0x18015af10): iterates the m_OrigResetButtons list (0x11f8) and
// removes the buttons whose runtime class matches CMFCToolBarButton (RTTI at
// 0x1803b1628). The CObList contents are side-table backed in OpenMFC and the
// removal path also needs the CSettingsStore; not modeled.
// Symbol: ?LoadResetOriginalState@CMFCToolBar@@IEAAHAEAVCSettingsStore@@@Z
extern "C" int MS_ABI impl__LoadResetOriginalState_CMFCToolBar__IEAAHAEAVCSettingsStore___Z(
    CMFCToolBar* pThis, CSettingsStore& store)
{
    (void)pThis; (void)store;
    // TODO(clean-room): not transcribed.
    return 0;
}
// Retail (0x1801539c0): registry state restoration -- builds the profile key
// name, opens a CSettingsStore and reads the toolbar layout. Not modeled.
// Symbol: ?LoadState@CMFCToolBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCToolBar__UEAAHPEB_WHI_Z(
    CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID)
{
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    // TODO(clean-room): not transcribed -- registry state restoration.
    return 0;
}
// Retail (0x18014e650) locates the RT_TOOLBAR resource, parses the button array
// (FindResourceW/LoadResource/LockResource + the 0xf1/RT_TOOLBAR layout struct)
// and forwards to the image loader. OpenMFC's LoadToolBar stub accepts resources
// without parsing; the resource layout is not modeled here, but the original
// resource id is remembered (m_uiOriginalResID) so CanBeRestored/LoadState see
// the toolbar as loaded, and success is reported like the LoadToolBar stub does.
// Symbol: ?LoadToolBarEx@CMFCToolBar@@UEAAHIAEAVCMFCToolBarInfo@@H@Z
extern "C" int MS_ABI impl__LoadToolBarEx_CMFCToolBar__UEAAHIAEAVCMFCToolBarInfo__H_Z(
    CMFCToolBar* pThis, unsigned int uiResID, CMFCToolBarInfo& info, int bLocked)
{
    if (!pThis) return 0;
    if (uiResID == 0) return 0;
    (void)info; (void)bLocked;
    pThis->m_uiOriginalResID = uiResID;
    // TODO(clean-room): partially transcribed -- RT_TOOLBAR resource parsing and
    // button/bitmap setup not modeled.
    return TRUE;
}
// Retail (0x180157c30): after internal checks, scans forward from
// m_iSelected + 1 (wrapping at the m_Buttons count @0x11a0) for a menu button
// that IsKindOf(CMFCToolBarMenuButton) and whose m_nStyle does not carry the
// 0x40000 bit; then calls virtual slot 0x770 (OnChangeHot) with the found index
// and, when the new hot button is a dropped menu, posts WM_KEYDOWN(VK_MENU) to
// its parent. Returns 1 unless the internal highlight helper fails.
// Symbol: ?NextMenu@CMFCToolBar@@UEAAHXZ
extern "C" int MS_ABI impl__NextMenu_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return 0;

    const int iSelected = pThis->m_iSelected;
    if (iSelected < 0) return 0;

    const int nCount = pThis->GetCount();
    if (nCount == 0) return 1;

    int iFound = -1;
    int i = iSelected + 1;
    if (i >= nCount) i = 0;

    for (int scanned = 0; scanned < nCount; ++scanned) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) &&
            (pButton->m_nStyle & 0x40000) == 0) {
            iFound = i;
            break;
        }
        if (++i >= nCount) i = 0;
    }

    if (iFound == -1) return 1;

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, iFound);

    // TODO(clean-room): retail also posts WM_KEYDOWN(VK_MENU) to the hot menu
    // button's parent via extended CMFCToolBarMenuButton members; not modeled.
    return 1;
}
// Retail (0x180154b80):
//     if (nNotifyCmd == 0 && pButton->vslot0x90(nCommand)) return FALSE;
//     hwndParent = [this+0xa0] ? [this+0xa0]->m_hWnd : GetParent(m_hWnd);
//     PostMessageW(hwndParent, WM_COMMAND,
//                  LOWORD(pButton->m_nID) | (LOWORD(nCommand) << 16), lParam);
//     return TRUE;
// Slot 0x90 and the owner member at [this+0xa0] are not modeled; the button
// early-out is dropped and the parent is resolved with CWnd::GetParent().
// Symbol: ?NotifyControlCommand@CMFCToolBar@@MEAAHPEAVCMFCToolBarButton@@HH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__HH_K_J_Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, int nNotifyCmd, int nCommand,
    unsigned long long wParam, long long lParam)
{
    (void)wParam; (void)nNotifyCmd;
    if (!pThis || !pButton) return 0;

    CWnd* pParent = pThis->GetParent();
    if (!pParent) return 0;

    ::PostMessageW(pParent->GetSafeHwnd(), WM_COMMAND,
                   MAKEWPARAM(LOWORD(pButton->m_nID), LOWORD(nCommand)),
                   static_cast<LPARAM>(lParam));
    return TRUE;
}
// Retail (0x180154260):
//     if (GetButtonCount() > 0) AdjustLayout();
//     else SetDrawTextLabels(TRUE);
// (GetButtonCount = vtable slot 0x2f0, AdjustLayout = slot 0x428,
//  SetDrawTextLabels = slot 0x5b8 with edx=1.)
// Symbol: ?OnAfterButtonDelete@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (pThis->GetCount() > 0) {
        pThis->AdjustLayout();
    } else {
        pThis->m_bDrawTextLabels = TRUE;
    }
}
// Retail (0x18015a0a0):
//     m_bFloating = FALSE;
//     hwndParent = GetParent(m_hWnd);
//     pParentWnd = CWnd::FromHandlePermanent(hwndParent);
//     if (pParentWnd) {
//         GetWindowRect(m_hWnd, &rc);
//         pParentWnd->ScreenToClient(&rc);
//         RedrawWindow(pParentWnd->m_hWnd, &rc, NULL, RDW_INVALIDATE|RDW_UPDATENOW);
//     }
// Symbol: ?OnAfterDock@CMFCToolBar@@MEAAXPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__OnAfterDock_CMFCToolBar__MEAAXPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CMFCToolBar* pThis, CBasePane* /*pDockBar*/, const RECT* /*lpRect*/, int /*dockMethod*/)
{
    if (!pThis) return;
    pThis->m_bFloating = FALSE;

    CWnd* pParent = pThis->GetParent();
    if (!pParent) return;

    RECT rc;
    ::GetWindowRect(pThis->GetSafeHwnd(), &rc);
    pParent->ScreenToClient(&rc);
    pParent->RedrawWindow(&rc, nullptr, 0x105);
}
// Retail (0x18015a030): calls base CBasePane::OnAfterFloat, runs two window-pos
// virtuals (slots 0x420/0x460, then 0x378 on the result) and finally stores
// m_bFloating = TRUE. The base call and the virtual chain are not modeled; the
// observable member write is reproduced.
// Symbol: ?OnAfterFloat@CMFCToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterFloat_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    pThis->m_bFloating = TRUE;
    // TODO(clean-room): partially transcribed -- base CBasePane::OnAfterFloat and
    // the window-position virtual chain are not modeled.
}
// Retail (0x18015a140):
//     CBasePane::OnBeforeChangeParent(pWndNewParent, bDelay);
//     m_bFloating = (pWndNewParent != NULL &&
//                    pWndNewParent->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)));
// (RTTI string "CPaneFrameWnd" is at 0x1803b1418 in the retail image.)
// Symbol: ?OnBeforeChangeParent@CMFCToolBar@@MEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnBeforeChangeParent_CMFCToolBar__MEAAXPEAVCWnd__H_Z(
    CMFCToolBar* pThis, CWnd* pWndNewParent, int /*bDelay*/)
{
    if (!pThis) return;
    // TODO(clean-room): base CBasePane::OnBeforeChangeParent not modeled.
    pThis->m_bFloating =
        (pWndNewParent != nullptr && pWndNewParent->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
            ? TRUE : FALSE;
}
// Retail (0x18015ac70):
//     GetClientRect(m_hWnd, &rcClient);
//     *pRect = button->m_rect;
//     if (button->m_bWrap && bHorz) {
//         pRect->left   = rcClient.left;
//         pRect->right  = rcClient.right;
//         pRect->top    = button->m_rect.bottom;
//         pRect->bottom = button->m_rect.bottom + 5;
//     }
// (The member reads are m_bWrap@0x14 and m_rect@0x68 in CMFCToolBarButton.)
// Symbol: ?OnCalcSeparatorRect@CMFCToolBar@@MEAAXPEAVCMFCToolBarButton@@AEAVCRect@@H@Z
extern "C" void MS_ABI impl__OnCalcSeparatorRect_CMFCToolBar__MEAAXPEAVCMFCToolBarButton__AEAVCRect__H_Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, CRect& rect, int bHorz)
{
    if (!pThis || !pButton) return;

    CRect rcClient;
    pThis->GetClientRect((struct tagRECT*)&rcClient);

    rect = pButton->m_rect;

    if (pButton->m_bWrap && bHorz) {
        rect.left   = rcClient.left;
        rect.right  = rcClient.right;
        rect.top    = pButton->m_rect.bottom;
        rect.bottom = pButton->m_rect.bottom + 5;
    }
}
// Retail (0x1801516b0):
//     CBasePane::OnCancelMode();
//     if (m_bStretchButton) {
//         m_pDragButton = NULL; m_bStretchButton = FALSE;
//         SetRectEmpty(&m_rectTrack); ReleaseCapture();
//         if (m_pWndLastCapture) { SetCapture(m_pWndLastCapture->m_hWnd);
//                                  m_pWndLastCapture = NULL; }
//     }
//     if (m_iButtonCapture >= 0) {
//         pButton = GetButton(m_iButtonCapture);
//         if (pButton) {
//             style = pButton->m_nStyle & ~(1<<17);   // btr $0x11
//             if (GetCapture() == m_hWnd) {
//                 ReleaseCapture();
//                 if (m_pWndLastCapture) { SetCapture(...); m_pWndLastCapture = NULL; }
//             }
//             vslot0x6f8 (this, m_iButtonCapture, style);  // SetButtonStyle
//         }
//     }
//     m_iButtonCapture = -1; m_iHighlighted = -1;
//     vslot0x770 (this);                 // OnChangeHot(-1)
//     for (pButton : m_Buttons) pButton->vslot0xc0();      // OnCancelMode()
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_FRAME); // 0x505
// Base call, SetButtonStyle and per-button virtuals are not modeled.
// Symbol: ?OnCancelMode@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    if (pThis->m_bStretchButton) {
        pThis->m_bStretchButton = FALSE;
        pThis->m_pDragButton = nullptr;
        pThis->m_rectTrack.SetRectEmpty();
        if (pThis->GetSafeHwnd()) ::ReleaseCapture();
        pThis->m_pWndLastCapture = nullptr;
    }

    if (pThis->m_iButtonCapture >= 0) {
        CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iButtonCapture);
        if (pButton) {
            pButton->m_nStyle &= ~(1 << 17);
            if (::GetCapture() == pThis->GetSafeHwnd()) {
                ::ReleaseCapture();
                pThis->m_pWndLastCapture = nullptr;
            }
            // TODO(clean-room): vslot0x6f8 (SetButtonStyle) not modeled.
        }
    }

    pThis->m_iButtonCapture = -1;
    pThis->m_iHighlighted = -1;
    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);

    // TODO(clean-room): per-button vslot0xc0 (OnCancelMode) not modeled.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x1801576f0), first part -- the m_iHot toggle:
//     if (m_iHot != nHot) m_iHot = nHot;
//     else if (m_iHot >= 0) m_iHot = -1;      // toggling back clears the hot
//     else m_iHot = nHot;
// The menu-mode / NotifyWinEvent(0x8005, hwnd, OBJID_CLIENT, ...) tail depends
// on CMFCMenuBar RTTI plus focus state and is not modeled.
// Symbol: ?OnChangeHot@CMFCToolBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot)
{
    if (!pThis) return;
    const int iOld = pThis->m_iHot;
    if (iOld == nHot) {
        pThis->m_iHot = (iOld < 0) ? nHot : -1;
    } else {
        pThis->m_iHot = nHot;
    }
    // TODO(clean-room): menu-mode NotifyWinEvent path not modeled.
}
// Retail (0x18015b6d0):
//     m_bRoundShape = pVisManager->GetToolBarButtonRoundShape(this);  // slot 0x188
//     if (m_bRoundShape) { round-shape handling } else SetWindowRgn(m_hWnd, 0, 0);
//     if (!m_bLocked && !vslot0x2e8()) vslot0x5b8 (this, 1);
//     adjust internal image lists + redraw (tail 0x180151890).
// GetToolBarButtonRoundShape and the round-shape helper are not modeled; the
// non-round branch (SetWindowRgn + layout) and the final redraw are kept.
// Symbol: ?OnChangeVisualManager@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__OnChangeVisualManager_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    // TODO(clean-room): retail asks CMFCVisualManager for the round-shape flag
    // (vslot0x188 GetToolBarButtonRoundShape); not modeled, keep FALSE.
    pThis->m_bRoundShape = FALSE;

    if (pThis->m_bRoundShape) {
        // TODO(clean-room): round-shape handling (0x18015b760) not modeled.
    } else if (pThis->GetSafeHwnd()) {
        ::SetWindowRgn(pThis->GetSafeHwnd(), nullptr, FALSE);
    }

    if (!pThis->m_bLocked) {
        pThis->AdjustLayout();
    }

    // retail tail (0x180151890): per-image adjustments + RedrawWindow.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x180154a60):
//     if (m_bCustomizeMode && !m_bLocked) return call 0x18028e040();
//     if (lParam == 0) {
//         if (wParam == 2) { vslot0x6d8 (this); return 1; }   // OnCustomize
//         if (wParam == 1) { focus-based button handling }
//     } else { iterate m_Buttons doing IsChild checks }
//     return 0;
// Only the customize-mode gate and the IDCANCEL-style early return are kept.
// Symbol: ?OnCommand@CMFCToolBar@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCToolBar__MEAAH_K_J_Z(
    CMFCToolBar* pThis, unsigned long long wParam, long long lParam)
{
    if (!pThis) return 0;

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !pThis->m_bLocked) {
        // TODO(clean-room): retail dispatches to an internal customizer helper.
        return 0;
    }

    if (lParam == 0 && wParam == 2) {
        // retail calls vslot0x6d8 (OnCustomize) then returns TRUE.
        return 1;
    }

    // TODO(clean-room): partially transcribed -- focus/IsChild button handling
    // not modeled.
    return 0;
}
// Retail (0x180153db0):
//     if (m_bLocked && m_bCustomizeMode) { MessageBeep(-1); return; }
//     if (vslot0x4e8 (this)) return;
//     if (m_bStretchButton) return;
//     vslot0x770 (this, -1);                          // OnChangeHot(-1)
//     if (!m_bCustomizeMode) { base CBasePane::OnContextMenu(...); return; }
//     ::ScreenToClient(m_hWnd, &pt);
//     iHit = vslot0x730 (this, pt);                   // HitTest
//     old = m_iSelected; m_iSelected = iHit;
//     if (old != -1) InvalidateButton(old);
//     ...
// The base call and the post-hit-test menu setup are not modeled; the hit test
// is reimplemented as a button-rect scan.
// Symbol: ?OnContextMenu@CMFCToolBar@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCToolBar__IEAAXPEAVCWnd__VCPoint___Z(
    CMFCToolBar* pThis, CWnd* pWnd, CPoint point)
{
    if (!pThis) return;

    if (pThis->m_bLocked && impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }
    if (pThis->m_bStretchButton) return;

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        // TODO(clean-room): base CBasePane::OnContextMenu not modeled.
        (void)pWnd;
        return;
    }

    CPoint pt = point;
    ::ScreenToClient(pThis->GetSafeHwnd(), (LPPOINT)&pt);

    int iHit = -1;
    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->m_rect.PtInRect(pt)) {
            iHit = i;
            break;
        }
    }

    const int iOld = pThis->m_iSelected;
    pThis->m_iSelected = iHit;
    if (iOld != -1) {
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iOld);
    }

    // TODO(clean-room): partially transcribed -- post-hit-test menu handling not
    // modeled.
}
// Retail (0x180157230): copies the image of the selected button (m_iSelected)
// into the user-images store via several internal image-list helpers and global
// static CMFCToolBarImages objects. Not modeled.
// Symbol: ?OnCopyImage@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCopyImage_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed -- internal image-copy helpers skipped.
}
// Retail (0x180151ff0): calls base CWnd::OnCreate, one-time static init, loads
// the 0x7904/0x7905 cursors and registers the tooltip control class. Base
// OnCreate and the static registration are not modeled.
// Symbol: ?OnCreate@CMFCToolBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCToolBar* pThis, LPCREATESTRUCTW lpCreateStruct)
{
    (void)pThis; (void)lpCreateStruct;
    // TODO(clean-room): not transcribed -- base OnCreate + cursor/tooltip init.
    return 0;
}
// Retail (0x1801546e0): brush = base CWnd::OnCtlColor(...); when
// m_bCustomizeMode && !m_bLocked it then walks m_Buttons asking each button for
// its own brush via virtuals (slots 0x150/0xa0). Those button virtuals are not
// modeled; the base brush is returned.
// Symbol: ?OnCtlColor@CMFCToolBar@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCToolBar__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CMFCToolBar* pThis, CDC* pDC, CWnd* pWnd, unsigned int nCtlColor)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): partially transcribed -- per-button brush virtuals skipped.
    return pThis->CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}
// Retail (0x180156490):
//     m_iButtonCapture = m_iHighlighted = m_iSelected = -1;
//     for (pButton : m_Buttons) {
//         pButton->vslot0xc0 (pButton);                // OnCancelMode()
//         if ((pButton->m_nStyle & 0x40000) == 0)
//             pButton->vslot0x140 (pButton, bCustomize == 0);
//     }
// The per-button virtuals are not modeled; the state reset is kept.
// Symbol: ?OnCustomizeMode@CMFCToolBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(CMFCToolBar* pThis, int bCustomize)
{
    if (!pThis) return;

    pThis->m_iButtonCapture = -1;
    pThis->m_iHighlighted = -1;
    pThis->m_iSelected = -1;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        if ((pButton->m_nStyle & 0x40000) == 0) {
            // TODO(clean-room): vslot0xc0 / vslot0x140 per-button customize-mode
            // calls not modeled; bCustomize is deliberately unused.
            (void)bCustomize;
        }
    }
}
// Retail (0x180152fd0): destroys m_penDrag (0x12c0), deletes m_pToolTip
// (0x12e8), calls base CBasePane::OnDestroy, clears the global
// m_pLastHookedToolbar when it points at this toolbar, and unlinks itself from
// the global toolbar list (0x1803b2098). Only the m_pLastHookedToolbar side
// effect and the tooltip pointer reset are reproduced; m_penDrag is a CPen and
// m_pToolTip a CMFCToolTipCtrl* whose full type is not available here.
// Symbol: ?OnDestroy@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- CPen dtor, tooltip delete, base
    // OnDestroy and the global toolbar-list unlink are not modeled.
    pThis->m_pToolTip = nullptr;
    if (impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA == pThis) {
        impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA = nullptr;
    }
}
// Retail (0x180152250):
//     if (m_bDisableCustomize) return 0;
//     m_iDragIndex = -1;
//     global drag flag (0x1803c2578) = 0;
//     return vslot0x7c0 (this, pDataObject, dwKeyState, point);   // OnDragOver
// Symbol: ?OnDragEnter@CMFCToolBar@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragEnter_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point)
{
    (void)pDataObject; (void)dwKeyState; (void)point;
    if (!pThis) return 0;

    if (pThis->m_bDisableCustomize) return 0;

    pThis->m_iDragIndex = -1;
    // TODO(clean-room): the global drag flag and the OnDragOver tail call
    // (vslot0x7c0) are not modeled; DROPEFFECT_NONE is the safe default.
    return DROPEFFECT_NONE;
}
// Retail (0x180158660):
//     CWnd::OnSetFocus(pOldWnd);
//     if (m_bLeaveFocus && pOldWnd && IsWindow(pOldWnd->m_hWnd) &&
//         !pOldWnd->IsKindOf(CMFCToolBar) &&
//         pOldWnd's parent is not a CMFCToolBar &&
//         this's parent is not a CMFCBaseTabCtrl)
//         m_hwndLastFocus = pOldWnd->m_hWnd;
// Symbol: ?OnSetFocus@CMFCToolBar@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCToolBar__IEAAXPEAVCWnd___Z(
    CMFCToolBar* pThis, CWnd* pOldWnd)
{
    if (!pThis) return;

    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(pThis, pOldWnd);

    if (!pThis->m_bLeaveFocus) return;
    if (!pOldWnd || !::IsWindow(pOldWnd->GetSafeHwnd())) return;
    if (pOldWnd->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) return;

    CWnd* pParentOld = CWnd::FromHandle(::GetParent(pOldWnd->GetSafeHwnd()));
    if (pParentOld && pParentOld->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) return;

    CWnd* pParentThis = CWnd::FromHandle(::GetParent(pThis->GetSafeHwnd()));
    if (pParentThis && pParentThis->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl))) return;

    pThis->m_hwndLastFocus = pOldWnd->GetSafeHwnd();
}
// Retail (0x180159620):
//     CBasePane::OnSettingChange(wParam, lpszSection);
//     if (wParam <= 0x2f && bit wParam of 0x840400000000 is set) {
//         gdata init + AFX_GLOBAL_DATA::UpdateFonts();
//         AdjustLayout();
//     }
// The base call and the AFX_GLOBAL_DATA object are not modeled.
// Symbol: ?OnSettingChange@CMFCToolBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCToolBar__IEAAXIPEB_W_Z(
    CMFCToolBar* pThis, unsigned int wParam, const wchar_t* lpszSection)
{
    if (!pThis) return;
    (void)lpszSection;
    // TODO(clean-room): retail starts with CBasePane::OnSettingChange (0x18000ce90).
    const unsigned long long mask = 0x840400000000ULL;   // SPI_SETNONCLIENTMETRICS etc.
    if (wParam <= 0x2f && ((mask >> wParam) & 1ULL)) {
        // TODO(clean-room): retail inits AFX_GLOBAL_DATA and calls UpdateFonts
        // (0x18006a9e0); the global data object is not modeled.
        pThis->AdjustLayout();
    }
}
// Retail (0x18015ad00):
//     CWnd::Default();
//     if (m_bCustomizeMode && customize-manager singleton && !m_bLocked) {
//         manager->ShowToolBar(this, bShow);           // 0x18017c600
//         if (this == selected-toolbar global) {
//             selected-toolbar global = NULL;
//             m_iSelected = -1;
//         }
//     }
// The manager singleton (0x1803be3a0), its ShowToolBar helper and the
// selected-toolbar global are unmodeled; the m_pSelToolbar static stands in
// for the latter.
// Symbol: ?OnShowWindow@CMFCToolBar@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCToolBar__IEAAXHI_Z(
    CMFCToolBar* pThis, int bShow, unsigned int nStatus)
{
    (void)bShow; (void)nStatus;
    if (!pThis) return;

    impl__Default_CWnd__IEAA_JXZ(pThis);

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !pThis->m_bLocked) {
        // TODO(clean-room): retail gates on the customize-manager singleton
        // (0x1803be3a0) and calls its ShowToolBar helper (0x18017c600) with
        // (this, bShow); neither is modeled.
        if (impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA == pThis) {
            impl__m_pSelToolbar_CMFCToolBar__1PEAV1_EA = nullptr;
            pThis->m_iSelected = -1;
        }
    }
}
// Retail (0x180155860):
//     SetRoundedRgn();
//     CBasePane::OnSize(nType, cx, cy);
//     if (m_bCustomizeMode && !m_bLocked) OnCancelMode();
//     if (!m_bInUpdateShadow) vslot0x7d8();
//     if (GetButtonCount() > 0) {
//         parent frame (AFXGetParentFrame) IsKindOf(CMiniFrameWnd):
//             adjust the frame's system menu (GetSystemMenu/DeleteMenu/
//             EnableMenuItem on ids 0xf120/0xf020/0xf030/0xf060).
//     }
// SetRoundedRgn, the base OnSize and the frame-menu walk are not modeled.
// Symbol: ?OnSize@CMFCToolBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCToolBar__IEAAXIHH_Z(
    CMFCToolBar* pThis, unsigned int nType, int cx, int cy)
{
    (void)nType; (void)cx; (void)cy;
    if (!pThis) return;

    // TODO(clean-room): retail first calls SetRoundedRgn (0x18015b760) and then
    // CBasePane::OnSize (0x18000c2a0); neither is modeled.

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && !pThis->m_bLocked) {
        impl__OnCancelMode_CMFCToolBar__IEAAXXZ(pThis);
    }

    if (!pThis->m_bInUpdateShadow) {
        // TODO(clean-room): retail calls vslot 0x7d8; not modeled.
    }

    if (pThis->GetCount() > 0) {
        // TODO(clean-room): retail walks the parent CMiniFrameWnd and edits its
        // system menu via AFXGetParentFrame/GetSystemMenu/DeleteMenu/
        // EnableMenuItem; the frame chain is not modeled.
    }
}
// Retail (0x180151820):
//     gdata init + AFX_GLOBAL_DATA::UpdateSysColors();
//     global-data helper (0x180009774) + its vslot 0x70;
//     UpdateImagesColor();       // per-image CMFCToolBarImages::OnSysColorChange
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
// Only the final redraw survives (the AFX_GLOBAL_DATA object and the image
// collection helpers are not modeled).
// Symbol: ?OnSysColorChange@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): gdata init/UpdateSysColors and the per-image
    // CMFCToolBarImages::OnSysColorChange loop (0x180151890) not modeled.
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x18014ff80) is complex: it gates on m_bShowTooltips /
// GetCapture() == this, hit-tests via vslot 0x730, and fills pTI->lpszText
// through CTooltipManager::SetTooltipText (0x180182560). The conservative
// version below keeps the button-rect hit test and the minimal TOOLINFOW
// fields, which is the observable part without the tooltip text plumbing.
// Symbol: ?OnToolHitTest@CMFCToolBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CMFCToolBar* pThis, CPoint point, TOOLINFOW* pTI)
{
    if (!pThis) return -1;
    // TODO(clean-room): partially transcribed -- the m_bShowTooltips/GetCapture
    // gate and CTooltipManager::SetTooltipText are not modeled.
    if (!pTI) return -1;

    const int nCount = pThis->GetCount();
    int iHit = -1;
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->m_rect.PtInRect(point)) {
            iHit = i;
            break;
        }
    }
    if (iHit == -1) return -1;

    CMFCToolBarButton* pButton = pThis->GetButton(iHit);
    if (!pButton) return -1;

    std::memset(pTI, 0, sizeof(*pTI));
    pTI->cbSize = sizeof(*pTI);
    pTI->hwnd = pThis->GetSafeHwnd();
    pTI->uId = pButton->m_nID;
    pTI->rect.left   = pButton->m_rect.left;
    pTI->rect.top    = pButton->m_rect.top;
    pTI->rect.right  = pButton->m_rect.right;
    pTI->rect.bottom = pButton->m_rect.bottom;
    return (__int64)pButton->m_nID;
}
// Retail (0x180154100):
//     pButton = GetButton(m_iSelected); if NULL return;
//     if (button text empty) vslot0x750 (OnSetDefaultButtonText);
//     run CMFCToolBarButtonCustomizeDialog(pButton, ..., this);
//     if (IDOK) { AdjustLayout(); SetDrawTextLabels(TRUE); RedrawWindow(0x505); }
// The dialog and the text virtual are not modeled.
// Symbol: ?OnToolbarAppearance@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarAppearance_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;
    // TODO(clean-room): retail runs the CMFCToolBarButtonCustomizeDialog
    // (ctor 0x18015eae0 / DoModal 0x1802088b0 / dtor 0x18015ee10) and, on IDOK,
    // calls AdjustLayout, SetDrawTextLabels(TRUE) and RedrawWindow(0x505); the
    // dialog is not modeled.
    (void)pButton;
}
// Retail (0x180154210):
//     vslot0x6a8 (RemoveButton) with m_iSelected;
//     m_iSelected = -1;
//     tail-call vslot0x858 (OnAfterButtonDelete).
// Symbol: ?OnToolbarDelete@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarDelete_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    impl__RemoveButton_CMFCToolBar__UEAAHH_Z(pThis, pThis->m_iSelected);
    pThis->m_iSelected = -1;
    impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(pThis);
}
// Retail (0x1801542b0):
//     pButton = GetButton(m_iSelected); if NULL return;
//     save m_bText/m_bImage; set m_bText = FALSE, m_bImage = TRUE;
//     if image index < 0 run the customize dialog and restore the flags on
//     cancel; on OK / valid image:
//         AdjustLayout(); SetDrawTextLabels(TRUE);
// The dialog is not modeled; the success path is kept.
// Symbol: ?OnToolbarImage@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarImage_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    pButton->m_bText = FALSE;
    pButton->m_bImage = TRUE;

    // TODO(clean-room): retail shows the CMFCToolBarButtonCustomizeDialog for
    // image-less buttons and restores m_bText/m_bImage on cancel; not modeled.
    pThis->AdjustLayout();
    pThis->m_bDrawTextLabels = TRUE;    // retail vslot 0x5b8 SetDrawTextLabels(TRUE)
}
// Retail (0x1801543c0):
//     pButton = GetButton(m_iSelected); if NULL return;
//     set m_bText = m_bImage = TRUE;
//     dialog path (skipped); then:
//     if (button text empty) vslot0x750 (OnSetDefaultButtonText);
//     if (text still empty) { MessageBeep(-1); m_bText = FALSE; m_bImage = TRUE; }
//     AdjustLayout();
// Symbol: ?OnToolbarImageAndText@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarImageAndText_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    pButton->m_bText = TRUE;
    pButton->m_bImage = TRUE;

    // TODO(clean-room): retail runs the CMFCToolBarButtonCustomizeDialog first
    // for image-less buttons; not modeled.
    if (pButton->m_strText.IsEmpty()) {
        // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
        // not modeled.
        ::MessageBeep(static_cast<UINT>(-1));
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
    }

    pThis->AdjustLayout();
}
// Retail (0x1801587e0) allocates a CMFCToolBarMenuButton (0x128 bytes), sets
// text flags, runs the customize dialog and on IDOK inserts the button via
// vslot 0x690 (InsertButton) at m_iSelected, then AdjustLayout, RedrawWindow
// and the button's vslot 0x50 (OnChangeParentWnd). None of the dialog, the
// insertion virtual or the delete path can be reproduced faithfully.
// Symbol: ?OnToolbarNewMenu@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarNewMenu_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    // TODO(clean-room): not transcribed -- needs the customize dialog and the
    // InsertButton virtual (vslot 0x690); neither is modeled.
    (void)pThis;
}
// Retail (0x180156ab0) resets the selected button to its CCommandManager
// default (image lookup in 0x1803b1f90 + SetImage), forces text on/off from
// m_bMenuMode/m_bImage, then SetButtonText, AdjustLayout, usage count update
// and RedrawWindow(0x505). The CCommandManager is not modeled.
// Symbol: ?OnToolbarReset@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarReset_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;
    // TODO(clean-room): the CCommandManager image lookup (0x1803b1f90) and the
    // button-state reset are not modeled; only the layout/redraw is kept.
    (void)pButton;
    pThis->AdjustLayout();
    pThis->RedrawWindow(nullptr, nullptr, 0x505);
}
// Retail (0x180154500) walks the real m_Buttons CObList for the button at
// m_iSelected and toggles a group separator (InsertSeparator via vslot 0x6a0
// or RemoveButton of the previous button), then AdjustLayout. The walk uses
// the real CObList which OpenMFC keeps empty (live buttons are side-table
// backed in mfccore.cpp), so only the relayout survives.
// Symbol: ?OnToolbarStartGroup@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarStartGroup_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the separator toggle walks the
    // real m_Buttons list and uses the InsertSeparator virtual; not modeled.
    pThis->AdjustLayout();
}
// Retail (0x1801545a0):
//     pButton = GetButton(m_iSelected); if NULL return;
//     m_bText = TRUE; m_bImage = FALSE;
//     if (text empty) vslot0x750 (OnSetDefaultButtonText);
//     if (text still empty) { MessageBeep(-1); m_bText = FALSE; m_bImage = TRUE; }
//     AdjustLayout(); SetDrawTextLabels(TRUE);
// Symbol: ?OnToolbarText@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarText_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pButton = pThis->GetButton(pThis->m_iSelected);
    if (!pButton) return;

    pButton->m_bText = TRUE;
    pButton->m_bImage = FALSE;

    if (pButton->m_strText.IsEmpty()) {
        // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
        // not modeled.
        ::MessageBeep(static_cast<UINT>(-1));
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
    }

    pThis->AdjustLayout();
    pThis->m_bDrawTextLabels = TRUE;    // retail vslot 0x5b8 SetDrawTextLabels(TRUE)
}
// Retail (0x180151c80) builds a stack CMFCToolBarCmdUI and routes every
// non-separator, non-AFX-id button through pTarget->OnCmdMsg (frame vslot
// 0x28); it ends with CWnd::UpdateDialogControls(this, pTarget,
// bDisableIfNoHandler && m_bDisableControlsIfNoHandler). The CmdUI object and
// the per-button routing are not modeled; the skip conditions and the final
// UpdateDialogControls call are kept.
// Symbol: ?OnUpdateCmdUI@CMFCToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(
    CMFCToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHandler)
{
    if (!pThis) return;

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        if (pButton->m_nStyle & 0x1) continue;          // separator/checkbox style
        const UINT nID = pButton->m_nID;
        if (nID == 0) continue;
        if ((nID - 0xf000) <= 0x1ef) continue;          // AFX control-bar ids
        if (nID >= 0xff00) continue;                    // AFX prompt ids
        if (nID == 0xffff) continue;
        // TODO(clean-room): retail routes the button through a stack
        // CMFCToolBarCmdUI + pTarget->OnCmdMsg; not modeled.
    }

    if (!pTarget) return;

    const int bDisable = (bDisableIfNoHandler != 0 &&
                          pThis->m_bDisableControlsIfNoHandler != 0) ? 1 : 0;
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(pThis, pTarget, bDisable);
}
// Retail (0x18015b930):
//     if (wParam & 2) {
//         CTooltipManager::CreateToolTip(&m_pToolTip, this, 2);
//         UpdateTooltips();
//     }
//     return 0;
// UpdateTooltips (0x180159ce0) is not modeled.
// Symbol: ?OnUpdateToolTips@CMFCToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateToolTips_CMFCToolBar__IEAA_J_K_J_Z(
    CMFCToolBar* pThis, unsigned long long wParam, long long lParam)
{
    (void)lParam;
    if (!pThis) return 0;

    if (wParam & 2) {
        impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(
            reinterpret_cast<CToolTipCtrl**>(&pThis->m_pToolTip), pThis, 2);
        // TODO(clean-room): retail then calls UpdateTooltips (0x180159ce0);
        // not modeled.
    }
    return 0;
}
// Retail (0x180159370) walks the AFXGetParentFrame() chain (skipping a
// CMDIChildWndEx frame) and delegates to the frame's OnGetToolTipText virtual
// (vslot 0x400 on CMDIFrameWndEx, 0x3d8 on CFrameWndEx, 0x3b8 on
// CMDIChildWndEx). Those frame virtuals are not declared in OpenMFC.
// Symbol: ?OnUserToolTip@CMFCToolBar@@UEBAHPEAVCMFCToolBarButton@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUserToolTip_CMFCToolBar__UEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCToolBar* pThis, CMFCToolBarButton* pButton, CString& strText)
{
    (void)pThis; (void)pButton; (void)strText;
    // TODO(clean-room): not transcribed -- the frame OnGetToolTipText virtuals
    // (0x400/0x3d8/0x3b8) are not modeled.
    return FALSE;
}
// Retail (0x180154640):
//     bResized = (m_sizeLast.cx < lpwndpos->cx) || (m_sizeLast.cy < lpwndpos->cy);
//     m_sizeLast = { cx, cy };
//     CWnd::Default();
//     if (bResized || parent IsKindOf(CMFCReBar)) RedrawWindow(NULL,NULL,0x585);
//     tail-call RedrawCustomizeButton();
// CMFCReBar is not declared in OpenMFC headers, so only the bResized gate is
// kept.
// Symbol: ?OnWindowPosChanged@CMFCToolBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCToolBar__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCToolBar* pThis, WINDOWPOS* lpwndpos)
{
    if (!pThis || !lpwndpos) return;

    const int cx = lpwndpos->cx;
    const int cy = lpwndpos->cy;
    const BOOL bResized = (pThis->m_sizeLast.cx < cx) ||
                          (pThis->m_sizeLast.cy < cy);

    pThis->m_sizeLast.cx = cx;
    pThis->m_sizeLast.cy = cy;

    impl__Default_CWnd__IEAA_JXZ(pThis);

    if (bResized) {
        // TODO(clean-room): retail also redraws when the parent
        // IsKindOf(CMFCReBar) (RTTI 0x1802fd8b8); CMFCReBar is not declared in
        // OpenMFC headers.
        ::RedrawWindow(pThis->GetSafeHwnd(), nullptr, nullptr, 0x585);
    }

    impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(pThis);
}
// Retail (0x180159020):
//     CWnd::Default();
//     if (parent IsKindOf(CMFCReBar)) AdjustLayout();
// Symbol: ?OnWindowPosChanging@CMFCToolBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CMFCToolBar__IEAAXPEAUtagWINDOWPOS___Z(
    CMFCToolBar* pThis, WINDOWPOS* lpwndpos)
{
    (void)lpwndpos;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): retail relayouts when the parent IsKindOf(CMFCReBar)
    // (RTTI 0x1802fd8b8); CMFCReBar is not declared in OpenMFC headers.
}
// Retail (0x180154940):
//     if (WM_KEYDOWN && wParam == VK_ESCAPE) {
//         if (m_bStretchButton) { OnCancelMode(); return TRUE; }
//         if (!GetDroppedDownMenu(this, NULL)) { vslot0x6d0(); vslot0x6d8(); }
//         return TRUE;
//     }
//     if (message == registered customize message) { OnPromptReset(...); return TRUE; }
//     if (message is a keyboard/mouse navigation message)
//         SendMessageW(m_pToolTip->m_hWnd, 0x407, 0, (LPARAM)pMsg);
//     return CBasePane::PreTranslateMessage(pMsg);
// GetDroppedDownMenu, vslots 0x6d0/0x6d8, the registered message and the
// CBasePane base are not modeled; the ESC/stretch path, the tooltip forward
// and the CWnd base are kept.
// Symbol: ?PreTranslateMessage@CMFCToolBar@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(
    CMFCToolBar* pThis, MSG* pMsg)
{
    if (!pThis || !pMsg) return 0;

    const UINT nMsg = pMsg->message;

    if (nMsg == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
        if (pThis->m_bStretchButton) {
            impl__OnCancelMode_CMFCToolBar__IEAAXXZ(pThis);
            return 1;
        }
        // TODO(clean-room): retail checks GetDroppedDownMenu(); when none is
        // open it calls vslots 0x6d0/0x6d8 and returns TRUE. Not modeled.
        return 1;
    }

    // TODO(clean-room): retail handles a registered customize message
    // (global 0x1803c2500) via OnPromptReset; not modeled.

    // Forward mouse/keyboard activity to the tooltip so it can dismiss itself.
    static const UINT s_tooltipFwd[] = {
        0xa1, 0xa2, 0xa4, 0xa5, 0xa7, 0xa8,   // non-client mouse buttons
        0x104,                                 // WM_SYSKEYDOWN
        0x200, 0x201, 0x202, 0x204, 0x205, 0x207,  // client mouse buttons
    };
    bool bForward = false;
    for (UINT m : s_tooltipFwd) {
        if (nMsg == m) { bForward = true; break; }
    }
    if (bForward && pThis->m_pToolTip) {
        // CMFCToolTipCtrl is only forward-declared; its CWnd base carries
        // m_hWnd at offset 0x40.
        HWND hwndTip = *reinterpret_cast<HWND*>(
            reinterpret_cast<char*>(pThis->m_pToolTip) + 0x40);
        if (hwndTip) {
            ::SendMessageW(hwndTip, 0x407, 0, reinterpret_cast<LPARAM>(pMsg));
        }
    }

    // Retail tail-calls CBasePane::PreTranslateMessage (0x18000cb30); the CWnd
    // implementation is the closest modeled base.
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// Retail (0x180157ae0) first asks GetDroppedDownMenu(); when the dropped menu
// is closed it scans backward from the dropped index (wrapping) for a
// CMFCToolBarMenuButton whose m_nStyle does not carry 0x40000 and calls
// OnChangeHot on it. GetDroppedDownMenu is not modeled, so the scan anchors on
// m_iSelected instead.
// Symbol: ?PrevMenu@CMFCToolBar@@UEAAHXZ
extern "C" int MS_ABI impl__PrevMenu_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return 0;

    const int nCount = pThis->GetCount();
    if (nCount == 0) return 0;

    const int iSelected = pThis->m_iSelected;

    int iFound = -1;
    int i = iSelected - 1;
    if (i < 0) i = nCount - 1;

    for (int scanned = 0; scanned < nCount; ++scanned) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (pButton && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) &&
            (pButton->m_nStyle & 0x40000) == 0) {
            iFound = i;
            break;
        }
        if (--i < 0) i = nCount - 1;
    }

    if (iFound == -1) return 1;

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, iFound);

    // TODO(clean-room): retail posts WM_KEYDOWN(VK_MENU) to the hot button's
    // parent when the newly selected button is a dropped-down menu; not modeled.
    return 1;
}
// Retail (0x180157eb0):
//     if (m_nID == 0 || m_nID == 0xffffffff) return 0;
//     AFXPlaySystemSound(1);
//     CMFCCmdUsageCount::AddCmd(m_nID);       // global map 0x1803b1fd0
//     hwndParent = owner handle at [this+0xa0] or GetParent(m_hWnd);
//     pParent = CWnd::FromHandle(hwndParent);
//     PostMessageW(pParent->m_hWnd, WM_COMMAND, m_nID, 0);
//     return 1;
// The sound, the usage-count map and the owner-handle member are not modeled.
// Symbol: ?ProcessCommand@CMFCToolBar@@QEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__ProcessCommand_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton)
{
    if (!pThis || !pButton) return 0;

    const UINT nID = pButton->m_nID;
    if (nID == 0 || nID == 0xffffffff) return 0;

    // TODO(clean-room): retail plays AFXPlaySystemSound(1) (0x180132460) and
    // records the command in CMFCCmdUsageCount::AddCmd; not modeled.

    CWnd* pParent = CWnd::FromHandle(::GetParent(pThis->GetSafeHwnd()));
    if (!pParent) return 0;

    ::PostMessageW(pParent->GetSafeHwnd(), WM_COMMAND, nID, 0);
    return 1;
}
// Retail (0x1801563b0) clears the m_AccelKeys CMap (0x1318) and rebuilds it
// from every button's accelerator (&x) text. The CMap internals are not
// modeled.
// Symbol: ?RebuildAccelerationKeys@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    // TODO(clean-room): not transcribed -- needs the m_AccelKeys CMap internals.
    (void)pThis;
}
// Retail (0x18015b870):
//     if (!m_hWnd || !m_pCustomizeBtn) return;
//     rect = m_pCustomizeBtn->GetInvalidateRect();   // vslot 0x120
//     InflateRect(&rect, dx, dy);                    // extended members 0x12c/0x150
//     rect.right += 10; rect.bottom += 10;
//     RedrawWindow(m_hWnd, &rect, NULL, 0x505);
// The GetInvalidateRect virtual and the extended customize-button members are
// not modeled; m_rect stands in for the invalidate rect.
// Symbol: ?RedrawCustomizeButton@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    if (!pThis->GetSafeHwnd()) return;
    CMFCToolBarButton* pBtn = pThis->m_pCustomizeBtn;
    if (!pBtn) return;

    // TODO(clean-room): retail uses vslot 0x120 (GetInvalidateRect) and the
    // extended customize-button members at 0x12c/0x150; m_rect stands in.
    CRect rect = pBtn->m_rect;
    rect.right += 10;
    rect.bottom += 10;
    ::RedrawWindow(pThis->GetSafeHwnd(), (const struct tagRECT*)&rect, nullptr, 0x505);
}
// Retail (0x18015b970) iterates the global toolbar list (0x1803b2098), and for
// every CMFCToolBar whose buttons have text with a '&' accelerator it
// invalidates each button rect and finally calls UpdateWindow. The global
// toolbar list is not modeled.
// Symbol: ?RedrawUnderlines@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__RedrawUnderlines_CMFCToolBar__SAXXZ()
{
    // TODO(clean-room): not transcribed -- the global toolbar list
    // (0x1803b2098) is not modeled.
}
// Retail (0x18014ec50):
//     m_iButtonCapture = m_iHighlighted = m_iSelected = -1;
//     while (m_Buttons not empty) {
//         pButton = m_Buttons.RemoveHead();
//         pButton->vslot0xc0();            // OnCancelMode
//         delete pButton;                  // vslot 0x8
//     }
//     m_pCustomizeBtn = NULL;
// OpenMFC keeps the live buttons in a per-toolbar side table (mfccore.cpp
// g_toolBarStates) that is not reachable from this file; the real m_Buttons
// CObList is a compatibility member and is normally empty, so the loop below
// is a no-op and the side-table buttons are left untouched (documented
// divergence).
// Symbol: ?RemoveAllButtons@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__RemoveAllButtons_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    pThis->m_iButtonCapture = -1;
    pThis->m_iHighlighted = -1;
    pThis->m_iSelected = -1;

    // TODO(clean-room): OpenMFC's live buttons are side-table backed and are
    // not cleared here (mfccore.cpp g_toolBarStates is not reachable).
    while (pThis->m_Buttons.GetCount() > 0) {
        CMFCToolBarButton* pButton =
            static_cast<CMFCToolBarButton*>(pThis->m_Buttons.RemoveHead());
        if (pButton) {
            // TODO(clean-room): retail calls vslot 0xc0 (OnCancelMode) first.
            delete pButton;
        }
    }

    pThis->m_pCustomizeBtn = nullptr;
}
// Retail (0x18014ecf0):
//     if (iIndex < 0 || iIndex >= m_Buttons.GetCount()) return FALSE;
//     if (iIndex == count-1 && m_pCustomizeBtn) return FALSE;
//     pButton = m_Buttons[iIndex]; m_Buttons.RemoveAt(pos);
//     pButton->vslot0xc0(); delete pButton;
//     adjust m_iSelected / m_iButtonCapture / m_iHighlighted;
//     OnChangeHot(-1);
//     remove trailing separator buttons;
//     RebuildAccelerationKeys();
//     return TRUE;
// As with RemoveAllButtons, OpenMFC's live buttons are side-table backed, so
// this operates on the (normally empty) compatibility CObList.
// Symbol: ?RemoveButton@CMFCToolBar@@UEAAHH@Z
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBar__UEAAHH_Z(CMFCToolBar* pThis, int iIndex)
{
    if (!pThis) return FALSE;

    const INT_PTR nCount = pThis->m_Buttons.GetCount();
    if (iIndex < 0 || iIndex >= nCount) return FALSE;

    // The last button cannot be removed while a customize button is present.
    if (iIndex == nCount - 1 && pThis->m_pCustomizeBtn != nullptr) return FALSE;

    CObList::POSITION pos = pThis->m_Buttons.FindIndex(iIndex);
    if (pos == CObList::POSITION(nullptr)) return FALSE;

    CMFCToolBarButton* pButton =
        static_cast<CMFCToolBarButton*>(pThis->m_Buttons.GetAt(pos));
    if (!pButton) return FALSE;

    pThis->m_Buttons.RemoveAt(pos);
    // TODO(clean-room): retail calls pButton->vslot 0xc0 (OnCancelMode) first.
    delete pButton;

    if (pThis->m_iSelected == iIndex) {
        pThis->m_iSelected = -1;
    } else if (pThis->m_iSelected > iIndex && pThis->m_iSelected >= 0) {
        pThis->m_iSelected--;
    }
    if (pThis->m_iButtonCapture == iIndex) {
        pThis->m_iButtonCapture = -1;
    } else if (pThis->m_iButtonCapture > iIndex && pThis->m_iButtonCapture >= 0) {
        pThis->m_iButtonCapture--;
    }
    if (pThis->m_iHighlighted == iIndex) {
        pThis->m_iHighlighted = -1;
    } else if (pThis->m_iHighlighted > iIndex && pThis->m_iHighlighted >= 0) {
        pThis->m_iHighlighted--;
    }

    impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, -1);

    // Remove trailing separator buttons.
    while (pThis->m_Buttons.GetCount() > 0) {
        CMFCToolBarButton* pTail =
            static_cast<CMFCToolBarButton*>(pThis->m_Buttons.GetTail());
        if (pTail && (pTail->m_nStyle & 0x1) != 0) {
            pThis->m_Buttons.RemoveTail();
            delete pTail;
        } else {
            break;
        }
    }

    impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(pThis);

    // TODO(clean-room): side-table divergence (see RemoveAllButtons) -- when
    // m_Buttons is empty this returns FALSE without touching the side table.
    return TRUE;
}
// Retail (0x18015b480): scans m_OrigResetButtons (0x11f8) for the button whose
// m_nID == uiID and removes it; returns FALSE when not found.
// Symbol: ?RemoveResetStateButton@CMFCToolBar@@IEAAHI@Z
extern "C" int MS_ABI impl__RemoveResetStateButton_CMFCToolBar__IEAAHI_Z(
    CMFCToolBar* pThis, unsigned int uiID)
{
    if (!pThis) return FALSE;

    INT_PTR nIndex = 0;
    CObList::POSITION pos = pThis->m_OrigResetButtons.GetHeadPosition();
    while (pos != CObList::POSITION(nullptr)) {
        CMFCToolBarButton* pButton =
            static_cast<CMFCToolBarButton*>(pThis->m_OrigResetButtons.GetNext(pos));
        if (pButton && pButton->m_nID == uiID) {
            CObList::POSITION removePos = pThis->m_OrigResetButtons.FindIndex(nIndex);
            if (removePos == CObList::POSITION(nullptr)) return FALSE;
            pThis->m_OrigResetButtons.RemoveAt(removePos);
            return TRUE;
        }
        ++nIndex;
    }
    return FALSE;
}
// Retail (0x180153880) deletes the toolbar's layout from the registry through
// a CSettingsStore chain (0x1800d2070 / 0x18000da00 / 0x18012a550).
// CSettingsStore is not modeled.
// Symbol: ?RemoveStateFromRegistry@CMFCToolBar@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__RemoveStateFromRegistry_CMFCToolBar__UEAAHPEB_WHI_Z(
    CMFCToolBar* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID)
{
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    // TODO(clean-room): not transcribed -- registry access via CSettingsStore.
    return FALSE;
}
// Retail (0x1801594b0) resets the CCommandManager and then every registered
// toolbar through the global toolbar list (0x1803b2098), calling the
// CanBeRestored / RestoreOriginalState virtuals. Neither the manager nor the
// global list is modeled.
// Symbol: ?ResetAll@CMFCToolBar@@SAXXZ
extern "C" void MS_ABI impl__ResetAll_CMFCToolBar__SAXXZ()
{
    // TODO(clean-room): not transcribed -- needs the CCommandManager and the
    // global toolbar list (0x1803b2098).
}
// Symbol: ?m_DefaultImages@CMFCToolBar@@1V?$CMap@IIHH@@A
extern "C" CMap_IIHH_56Bytes impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A = {};
// Symbol: ?m_DropSource@CMFCToolBar@@1VCMFCToolBarDropSource@@A
extern "C" CToolBarDropSource_144Bytes impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A = {};
// The eight CMFCToolBarImages statics below are shared with other CMFCToolBar
// code through their exported symbols, so they keep plain (non-static)
// linkage; `extern "C"` plus an initializer in one declaration matches the
// house pattern in feature_static_data.cpp and keeps the object at the
// modeled 408-byte retail size.
// Symbol: ?m_Images@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_ColdImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_DisabledImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_DisabledImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_DisabledMenuImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_LargeImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_LargeColdImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_LargeDisabledImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Symbol: ?m_MenuImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();
// Retail (0x18015b760): if m_hWnd:
//     if !m_bRoundShape (or GetVisualManager()==NULL): SetWindowRgn(hwnd,NULL,FALSE);
//     else: GetWindowRect -> CreateRoundRectRgn(0,0,w+1,h+1,4,4)
//           -> SetWindowRgn(hwnd, rgn, FALSE).
// The visual-manager gate (vslot 0x350) is not modeled, so the create path is
// taken whenever m_bRoundShape is set.
// Symbol: ?SetRoundedRgn@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__SetRoundedRgn_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    const HWND hwnd = pThis->GetSafeHwnd();
    if (!hwnd) return;

    // TODO(clean-room): retail gates the create path on GetVisualManager()
    // (vslot 0x350) being non-NULL; not modeled, so this assumes the manager
    // exists whenever m_bRoundShape is set.
    if (!pThis->m_bRoundShape) {
        // Retail clears the region when the shape flag is off.
        ::SetWindowRgn(hwnd, nullptr, FALSE);
        return;
    }

    RECT rc;
    ::GetWindowRect(hwnd, &rc);
    const int cx = rc.right - rc.left + 1;
    const int cy = rc.bottom - rc.top + 1;
    HRGN hRgn = ::CreateRoundRectRgn(0, 0, cx, cy, 4, 4);
    // Retail passes the (possibly NULL) region straight to SetWindowRgn;
    // SetWindowRgn with NULL clears the region, matching retail on failure.
    ::SetWindowRgn(hwnd, hRgn, FALSE);
}
// Retail (0x18015b010) is exactly six stores; fully transcribed.
// Symbol: ?SetSiblingToolBar@CMFCToolBar@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetSiblingToolBar_CMFCToolBar__QEAAXPEAV1__Z(
    CMFCToolBar* pThis, CMFCToolBar* pSibling)
{
    if (!pThis || !pSibling) return;

    pThis->m_pBrotherToolBar = pSibling;   // 0x12F8
    pThis->m_bHasBrother = 1;              // 0x110C
    pSibling->m_bHasBrother = 1;
    pSibling->m_pBrotherToolBar = pThis;
    pThis->m_bElderBrother = 1;            // 0x1110
    pSibling->m_bElderBrother = 0;
}
// Retail (0x180158920):
//     pButton = GetButton(nIndex); if NULL return;
//     if (bShowText) {
//         if (szText == NULL) vslot0x750(OnSetDefaultButtonText, pButton);
//         else                SetButtonText(nIndex, szText);   // sets button text
//     }
//     pButton->m_bImage = bShowImage;   // 0x10
//     pButton->m_bText  = bShowText;    // 0x0C
// The two text virtuals are not modeled; the string write is done directly.
// Symbol: ?SetToolBarBtnText@CMFCToolBar@@QEAAXIPEB_WHH@Z
extern "C" void MS_ABI impl__SetToolBarBtnText_CMFCToolBar__QEAAXIPEB_WHH_Z(
    CMFCToolBar* pThis, unsigned int nIndex, const wchar_t* szText, int bShowText, int bShowImage)
{
    if (!pThis) return;

    CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(nIndex));
    if (!pButton) return;

    if (bShowText) {
        if (szText) {
            // Retail delegates to CMFCToolBar::SetButtonText (0x18014f1c0),
            // which assigns the string to the button; reproduced inline.
            pButton->m_strText = szText;
        } else {
            // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
            // not modeled.
        }
    }
    pButton->m_bImage = bShowImage;   // @16 (retail stores the raw int)
    pButton->m_bText  = bShowText;    // @12
}
// Retail (0x18015b110) re-lays both toolbars into two rows using CMFCToolBar
// virtual layout slots (0x350, 0x4d0, 0x58) and CMFCControlBarImpl helpers,
// all unmodeled.
// Symbol: ?SetTwoRowsWithSibling@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__SetTwoRowsWithSibling_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the sibling two-row layout
    // (0x18015b110) walks virtual layout slots and CObList internals that
    // OpenMFC does not model.
}
// Retail (0x18014de40): if pImages && m_hbmImageWell && size matches the
// default button size globals -> store pImages to static m_pUserImages, return
// TRUE.  The size globals (0x1803b1cb8/0x1803b1cbc) and the m_pUserImages
// static (ordinal 8387, a different wave) are not modeled here.
// Symbol: ?SetUserImages@CMFCToolBar@@SAHPEAVCMFCToolBarImages@@@Z
extern "C" int MS_ABI impl__SetUserImages_CMFCToolBar__SAHPEAVCMFCToolBarImages___Z(
    CMFCToolBarImages* pImages)
{
    if (!pImages) return 0;
    // m_hbmImageWell (0x0A0) is protected in the header; read via the
    // documented offset.
    const void* pHbm = *reinterpret_cast<void* const*>(
        reinterpret_cast<const char*>(pImages) + 0xA0);
    if (!pHbm) return 0;
    // TODO(clean-room): retail compares m_sizeImage (0x68) against the default
    // button-size globals and stores pImages into the CMFCToolBar::m_pUserImages
    // static; neither is modeled in this wave.
    return 1;
}
// Retail (0x1801566f0) maps the command ID to status-bar text via the
// status-bar manager singleton (0x1803be360) and AFX_GLOBAL_DATA, then posts
// WM_SETMESSAGESTRING to the frame; all unmodeled.
// Symbol: ?ShowCommandMessageString@CMFCToolBar@@MEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(
    CMFCToolBar* pThis, unsigned int nID)
{
    (void)nID;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the command -> string table
    // lookup and WM_SETMESSAGESTRING post depend on AFX_GLOBAL_DATA and the
    // status-bar manager singleton; not modeled.
}
// Retail (0x1801555b0) computes the toolbar size through the visual manager
// (vslot 0x338/0x3b20), CalcLayout and CMFCToolBarImages locking; unmodeled.
// Symbol: ?SizeToolBar@CMFCToolBar@@IEAAXHH@Z
extern "C" void MS_ABI impl__SizeToolBar_CMFCToolBar__IEAAXHH_Z(
    CMFCToolBar* pThis, int cx, int cy)
{
    (void)cx; (void)cy;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- layout math needs the visual
    // manager and CalcLayout; not modeled.
}
// Retail (0x180159950) walks the CObList comparing against m_OrigButtons and
// refreshes m_bResourceWasChanged / button styles; CObList contents live in a
// side table and the comparison helpers are unmodeled.
// Symbol: ?SmartUpdate@CMFCToolBar@@MEAAHAEBVCObList@@@Z
extern "C" int MS_ABI impl__SmartUpdate_CMFCToolBar__MEAAHAEBVCObList___Z(
    CMFCToolBar* pThis, const CObList& lstButtons)
{
    (void)lstButtons;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the retail list comparison
    // (0x180159950) is not modeled.
    return 0;
}
// Retail (0x180154c40) computes the size via SizeToolBar and CanBeResized.
// CSize is returned through the hidden return pointer used by MSVC for the
// exported ?AVCSize@@ methods (see cbarcore.cpp).
// Symbol: ?StretchPane@CMFCToolBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__StretchPane_CMFCToolBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CMFCToolBar* pThis, int cx, int cy)
{
    if (!pRet) return;
    new(pRet) CSize(0, 0);
    if (!pThis) return;
    (void)cx; (void)cy;
    // TODO(clean-room): transcribed partially -- retail sizes the pane through
    // SizeToolBar and CanBeResized (vslot 0x2d8); not modeled.
}
// Retail (0x180156cb0) is a WH_MOUSE hook proc that forwards WM_MOUSEMOVE to
// the currently hooked toolbar (global 0x1803be360) and installs/removes the
// hook; the hook-global and the mouse handling are unmodeled.
// Symbol: ?ToolBarMouseHookProc@CMFCToolBar@@KA_JH_K_J@Z
extern "C" __int64 MS_ABI impl__ToolBarMouseHookProc_CMFCToolBar__KA_JH_K_J_Z(
    int nCode, unsigned __int64 wParam, __int64 lParam)
{
    (void)wParam; (void)lParam;
    if (nCode < 0) return 0;
    // TODO(clean-room): transcribed partially -- the global hook handle and
    // toolbar pointer (0x1803be360) are not modeled.
    return 0;
}
// Retail (0x180159570) looks the character up in the m_AccelKeys map and runs
// the matching command; the CMap internals (0x1318) are not modeled.
// Symbol: ?TranslateChar@CMFCToolBar@@UEAAHI@Z
extern "C" int MS_ABI impl__TranslateChar_CMFCToolBar__UEAAHI_Z(
    CMFCToolBar* pThis, unsigned int nChar)
{
    (void)nChar;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- m_AccelKeys map lookup not
    // modeled.
    return 0;
}
// Retail (0x180151e40) validates the button style range and refreshes the
// button window; internal button-state helpers are unmodeled.
// Symbol: ?UpdateButton@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateButton_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int iIndex)
{
    (void)iIndex;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180151e40) redraws
    // the button through unmodeled state helpers.
}
// Retail (0x180151890) calls CMFCToolBarImages::OnSysColorChange for seven
// static image lists (m_DisabledImages is skipped) and the eight embedded
// *_Locked lists, in this exact order.  Transcribed 1:1 against the static
// data exports defined at the top of this file.
// Symbol: ?UpdateImagesColor@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateImagesColor_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    // Static image lists (retail .data 0x3c25a0..0x3c3100).
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);

    // Embedded locked lists.
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_ImagesLocked);              // 0x3F8
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_ColdImagesLocked);          // 0x590

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A);

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_MenuImagesLocked);          // 0xD88
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledImagesLocked);      // 0x728
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledMenuImagesLocked);  // 0xF20

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeImagesLocked);         // 0x8C0
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeColdImagesLocked);     // 0xA58
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeDisabledImagesLocked); // 0xBF0
    // NB: retail does NOT touch m_DisabledImages here -- 7 static lists + 8
    // locked lists = 15 calls at 0x180151890.
}
// Retail (0x180159ce0) posts tooltip-add messages to m_pToolTip for every
// button with text; the tooltip manager and button walk are unmodeled.
// Symbol: ?UpdateTooltips@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180159ce0) walks
    // m_Buttons and drives m_pToolTip via TTM_ADDTOOL; not modeled.
}
// Retail (0x180155270) wraps buttons across rows using CalcLayout and a
// "wrap" font; the layout machinery is unmodeled.
// Symbol: ?WrapToolBar@CMFCToolBar@@QEAAHHHPEAVCDC@@HH@Z
extern "C" int MS_ABI impl__WrapToolBar_CMFCToolBar__QEAAHHHPEAVCDC__HH_Z(
    CMFCToolBar* pThis, int cx, int cy, CDC* pDC, int nMaxRows, int nMaxCols)
{
    (void)cx; (void)cy; (void)pDC; (void)nMaxRows; (void)nMaxCols;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail wrap layout (0x180155270)
    // is not modeled.
    return 0;
}
// Retail (0x18015a4a0): validate vt==VT_I4, resolve the button by child id,
// then for m_nID outside {0, 0xFFFFFFFF} run the button's OnClick virtual and
// post WM_COMMAND to the frame when it returns 0; for the two special ids call
// vslot 0x50.
// The child-id resolution (AccGetButtonByChildId, 0x18015ba90) numbers
// buttons 1-based among those passing the button vslot 0x188 gate; with that
// gate unmodeled the child id maps to GetButton(nChild - 1).
// Symbol: ?accDoDefaultAction@CMFCToolBar@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCToolBar__MEAAJUtagVARIANT___Z(
    CMFCToolBar* pThis, VARIANT varChild)
{
    if (!pThis) return S_OK;
    if (varChild.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const LONG nChild = varChild.lVal;
    if (nChild == 0) return S_OK;

    // Retail resolves the button through AccGetButtonByChildId (0x18015ba90);
    // child ids are 1-based, so index = nChild - 1.
    CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(nChild - 1));
    if (!pButton) return S_OK;

    // Retail (0x18015a4db..): `dec m_nID; cmp $0xFFFFFFFD; ja` — the vslot-0x50
    // path is taken only for m_nID == 0 or m_nID == 0xFFFFFFFF; every other id
    // runs the OnClick + WM_COMMAND path.
    if (pButton->m_nID != 0 && pButton->m_nID != 0xFFFFFFFFu) {
        // TODO(clean-room): retail invokes the button OnClick virtual (vslot
        // 0x58) and posts WM_COMMAND (0x111) only when it returns 0; the
        // virtual is not modeled.  Retail resolves the owner as
        // m_pWndParent@0xA0, falling back to GetParent(m_hWnd); m_pWndParent
        // is not modeled either, so the fallback is used here.
        HWND hOwner = ::GetParent(pThis->GetSafeHwnd());
        if (hOwner) {
            ::SendMessageW(hOwner, WM_COMMAND, pButton->m_nID, 0);
        }
    } else {
        // TODO(clean-room): retail calls button vslot 0x50; not modeled.
    }
    return S_OK;
}
// Retail (0x18015a250): null pVarOut -> E_INVALIDARG; ScreenToClient the point,
// walk the buttons, and on the first m_rect hit store the 1-based child id.
// Symbol: ?accHitTest@CMFCToolBar@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCToolBar__MEAAJJJPEAUtagVARIANT___Z(
    CMFCToolBar* pThis, long x, long y, VARIANT* pVarOut)
{
    if (!pThis) return S_OK;
    if (!pVarOut) return E_INVALIDARG;

    pVarOut->vt = 3;   // VT_I4
    pVarOut->lVal = 0;

    POINT pt;
    pt.x = x;
    pt.y = y;
    if (pThis->GetSafeHwnd()) {
        ::ScreenToClient(pThis->GetSafeHwnd(), &pt);
    }

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        // TODO(clean-room): retail first calls the button's vslot 0x188
        // (rect/visibility check) and only tests m_rect when it returns > 0.
        if (pButton->m_rect.PtInRect(CPoint(pt))) {
            pVarOut->lVal = i + 1;   // 1-based child id
            // TODO(clean-room): retail then calls the button's vslot 0x180 to
            // fill accessibility data; not modeled.
            return S_OK;
        }
    }
    return S_OK;
}
// Retail (0x18015a3e0), fully transcribed.  NAVDIR ids: LEFT=3, RIGHT=4,
// NEXT=5, PREVIOUS=6, FIRSTCHILD=7, LASTCHILD=8 (oleacc.h).  LEFT shares a
// body with PREVIOUS, RIGHT with NEXT; out-of-range navigation clears vt to
// VT_EMPTY and returns S_FALSE; FIRSTCHILD/LASTCHILD reject a non-zero start
// child with S_FALSE.
// Symbol: ?accNavigate@CMFCToolBar@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCToolBar__MEAAJJUtagVARIANT__PEAU2__Z(
    CMFCToolBar* pThis, long navDir, VARIANT varStart, VARIANT* pvarEnd)
{
    if (!pvarEnd) return E_INVALIDARG;

    pvarEnd->vt = 0;   // VT_EMPTY
    if (varStart.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const long nCount = pThis ? static_cast<long>(pThis->GetCount()) : 0;

    switch (navDir) {
    case 3:  // NAVDIR_LEFT
    case 6:  // NAVDIR_PREVIOUS
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n - 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next > 0) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 4:  // NAVDIR_RIGHT
    case 5:  // NAVDIR_NEXT
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n + 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next <= nCount) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 7:  // NAVDIR_FIRSTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = 1;
        return S_OK;

    case 8:  // NAVDIR_LASTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = nCount;
        return S_OK;

    default:
        // Retail (0x18015a488) returns S_FALSE for any other navDir, leaving
        // pvarEnd->vt at VT_EMPTY (set at entry).
        return S_FALSE;
    }
}
// Retail (0x18015a350): null ppDispChild -> E_INVALIDARG; otherwise zero the
// out-param and return S_FALSE unless the child resolves through the RTTI and
// IAccessibleFromWindow plumbing, which is unmodeled.
// Symbol: ?get_accChild@CMFCToolBar@@MEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCToolBar__MEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCToolBar* pThis, VARIANT varChild, IDispatch** ppDispChild)
{
    if (!pThis) return S_FALSE;
    if (!ppDispChild) return E_INVALIDARG;
    *ppDispChild = nullptr;

    if (varChild.vt != 3 /*VT_I4*/) return S_FALSE;
    if (varChild.lVal == 0) return S_FALSE;

    // TODO(clean-room): retail resolves the button, checks IsKindOf(
    // CMFCToolBarButton) and calls IAccessibleFromWindow on its host window
    // (0x1803e9148); not modeled, so children are reported as not accessible.
    return S_FALSE;
}
// Retail (0x18015a320), fully transcribed: *pCountChildren = GetCount().
// Symbol: ?get_accChildCount@CMFCToolBar@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCToolBar__MEAAJPEAJ_Z(
    CMFCToolBar* pThis, long* pCountChildren)
{
    if (!pThis) return S_OK;
    if (!pCountChildren) return E_INVALIDARG;
    *pCountChildren = static_cast<long>(pThis->GetCount());
    return S_OK;
}
// CMFCToolBar::EnableLargeIcons(BOOL) — retail is a single store:
//     mov [rcx+0x10bc],edx ; ret
// It deliberately does not relayout; callers follow up with AdjustLayout.
// Symbol: ?EnableLargeIcons@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableLargeIcons_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int bEnable)
{
    if (!pThis) return;
    pThis->m_bLargeIconsAreEnbaled = bEnable;
}
// CMFCToolBar::EnableTextLabels(BOOL) — retail:
//     if (m_bMenuMode) return;        // cmp [rcx+0x10ec],0 ; jne done
//     m_bTextLabels = bEnable;        // mov [rcx+0x10c8],edx
//     AdjustLayout();                 // virtual, vtable slot +0x428
// The early-out matters: a menu bar silently ignores the request rather than
// relaying out.
// Symbol: ?EnableTextLabels@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTextLabels_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int bEnable)
{
    if (!pThis) return;
    if (pThis->m_bMenuMode) return;
    pThis->m_bTextLabels = bEnable;
    pThis->AdjustLayout();
}
// CMFCToolBar::GetHighlightedButton() — retail:
//     mov edx,[rcx+0x1138]      ; m_iHighlighted
//     test edx,edx ; jns take   ; negative -> no highlight
//     xor eax,eax ; ret
//     jmp GetButton             ; tail call with the index still in edx
// Symbol: ?GetHighlightedButton@CMFCToolBar@@QEBAPEAVCMFCToolBarButton@@XZ
extern "C" CMFCToolBarButton* MS_ABI
impl__GetHighlightedButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__XZ(const CMFCToolBar* pThis)
{
    if (!pThis) return nullptr;
    const int iHighlighted = pThis->m_iHighlighted;
    if (iHighlighted < 0) return nullptr;
    return pThis->GetButton(iHighlighted);
}
// CMFCToolBar::IsButtonHighlighted(int) — retail, in order:
//     if (m_bCustomizeMode && !m_bLocked) return FALSE;
//     if (m_bMenuMode) return iButton == m_iHighlighted;
//     if (iButton != m_iHighlighted && iButton != m_iButtonCapture) return FALSE;
//     if (m_iButtonCapture == -1) return TRUE;
//     return iButton == m_iButtonCapture;
// The customize-mode gate reads the class static, not an instance member.
// Symbol: ?IsButtonHighlighted@CMFCToolBar@@QEBAHH@Z
extern "C" int MS_ABI impl__IsButtonHighlighted_CMFCToolBar__QEBAHH_Z(
    const CMFCToolBar* pThis, int iButton)
{
    if (!pThis) return FALSE;

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && pThis->m_bLocked == 0)
        return FALSE;

    const int iHighlighted   = pThis->m_iHighlighted;
    const int iButtonCapture = pThis->m_iButtonCapture;

    if (pThis->m_bMenuMode != 0)
        return iButton == iHighlighted ? TRUE : FALSE;

    if (iButton != iHighlighted && iButton != iButtonCapture)
        return FALSE;
    if (iButtonCapture == -1)
        return TRUE;
    return iButton == iButtonCapture ? TRUE : FALSE;
}
// CMFCToolBar::IsOneRowWithSibling() — retail:
//     mov rdx,[rcx+0x12f8]      ; m_pBrotherToolBar
//     mov rdx,[rdx+0x130]       ; brother->m_pDockBarRow
//     cmp [rcx+0x130],rdx       ; this->m_pDockBarRow
//     sete al
// Retail dereferences m_pBrotherToolBar without a null check (the method is
// only reached when a sibling exists). OpenMFC guards instead of faulting:
// a null-deref is not behaviour worth reproducing bit-for-bit, and FALSE is
// the answer consistent with "not sharing a row with a sibling".
// Symbol: ?IsOneRowWithSibling@CMFCToolBar@@QEAAHXZ
extern "C" int MS_ABI impl__IsOneRowWithSibling_CMFCToolBar__QEAAHXZ(CMFCToolBar* pThis)
{
    if (!pThis) return FALSE;
    const CMFCToolBar* pBrother = pThis->m_pBrotherToolBar;
    if (!pBrother) return FALSE;
    return pThis->m_pDockBarRow == pBrother->m_pDockBarRow ? TRUE : FALSE;
}
// CMFCToolBar::CanBeRestored() const — retail:
//     xor eax,eax ; cmp DWORD PTR [rcx+0x1308],eax ; setne al ; ret
// A toolbar can be restored precisely when it remembers the resource id it was
// originally loaded from. Note the result is normalized to 0/1 by setne, so it
// is a genuine BOOL rather than the raw id.
// Symbol: ?CanBeRestored@CMFCToolBar@@UEBAHXZ
extern "C" int MS_ABI impl__CanBeRestored_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis)
{
    if (!pThis) return FALSE;
    return pThis->m_uiOriginalResID != 0 ? TRUE : FALSE;
}
// Symbol: ?LoadToolBar@CMFCToolBar@@UEAAHIIIHIII@Z
extern "C" int MS_ABI impl__LoadToolBar_CMFCToolBar__UEAAHIIIHIII_Z(
    CMFCToolBar* pThis, unsigned int p0, unsigned int p1, unsigned int p2,
    int p3, unsigned int p4, unsigned int p5, unsigned int p6) {
    return (int)pThis->LoadToolBar(p0, p1, p2, p3, p4, p5, p6);
}
// Symbol: ?SetButtons@CMFCToolBar@@UEAAHPEBIHH@Z
extern "C" int MS_ABI impl__SetButtons_CMFCToolBar__UEAAHPEBIHH_Z(
    CMFCToolBar* pThis, const unsigned int* p0, int p1, int p2) {
    return (int)pThis->SetButtons(p0, p1, p2);
}
// Symbol: ?SetSizes@CMFCToolBar@@SAXUtagSIZE@@0@Z
extern "C" void MS_ABI impl__SetSizes_CMFCToolBar__SAXUtagSIZE__0_Z(SIZE p0, SIZE p1) {
    CMFCToolBar::SetSizes(p0, p1);
}
// Symbol: ?EnableDocking@CMFCToolBar@@UEAAXK@Z
extern "C" void MS_ABI impl__EnableDocking_CMFCToolBar__UEAAXK_Z(CMFCToolBar* pThis, unsigned long p0) {
    pThis->EnableDocking(p0);
}
// Symbol: ?AdjustLayout@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis) {
    pThis->AdjustLayout();
}
// Symbol: ?AdjustSize@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__AdjustSize_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis) {
    pThis->AdjustSize();
}
// Symbol: ?GetButtonText@CMFCToolBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetButtonText_CMFCToolBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CMFCToolBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetButtonText(nIndex));
}
// Symbol: ?GetButtonText@CMFCToolBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetButtonText_CMFCToolBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCToolBar* pThis, int nIndex, CString* pStr) {
    pThis->GetButtonText(nIndex, *pStr);
}
CMFCToolBar::CMFCToolBar() {
    // The eight CMFCToolBarImages members precede m_bLocked and keep their
    // constructed state. The span zeroed here is NOT purely POD, though: the
    // three CObLists (4488/4544/4600) and m_penDrag (4800) all sit after
    // m_bLocked, so they are re-constructed below. Zeroing already gives an
    // empty list and a null pen handle; what the memset destroys and placement
    // new restores is their vfptrs.
    char* const base = reinterpret_cast<char*>(this);
    std::memset(base + offsetof(CMFCToolBar, m_bLocked), 0,
                sizeof(CMFCToolBar) - offsetof(CMFCToolBar, m_bLocked));
    new (&m_Buttons) CObList();
    new (&m_OrigButtons) CObList();
    new (&m_OrigResetButtons) CObList();
    new (&m_penDrag) CPen();
    m_iButtonCapture = -1;
    m_iHighlighted   = -1;
    m_iSelected      = -1;
    m_iHot           = -1;
    m_iDragIndex     = -1;
    m_iAccHotItem    = -1;
    m_bGrayDisabledButtons = TRUE;
    m_bRouteCommandsViaFrame = TRUE;
    m_bShowHotBorder = TRUE;
}
CMFCToolBar::~CMFCToolBar() {
    ClearToolBarState(this);
}
BOOL CMFCToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    g_toolBarStates[this].parent = pParentWnd;
    m_hWnd = ::CreateWindowExW(0, L"ToolbarWindow32", nullptr,
                                dwStyle, 0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    } else {
        ClearToolBarState(this);
    }
    return m_hWnd != nullptr;
}
BOOL CMFCToolBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    return Create(pParentWnd, dwStyle, nID);
}
BOOL CMFCToolBar::LoadToolBar(UINT, UINT, UINT, BOOL, UINT, UINT, UINT) { return TRUE; }
BOOL CMFCToolBar::LoadBitmap(UINT) { return TRUE; }
BOOL CMFCToolBar::SetButtons(const UINT* lpIDArray, int nIDCount, BOOL bImages) {
    if (nIDCount < 0 || (nIDCount > 0 && lpIDArray == nullptr)) return FALSE;

    ToolBarState& state = g_toolBarStates[this];
    ClearToolBarButtons(state);

    for (int i = 0; i < nIDCount; ++i) {
        UINT id = lpIDArray[i];
        auto* button = new (std::nothrow) CMFCToolBarButton(id, bImages ? i : -1);
        if (!button) {
            ClearToolBarButtons(state);
            return FALSE;
        }
        if (id == 0) {
            button->m_nStyle = TBSTYLE_SEP;
            button->m_iImage = -1;
            button->m_bImage = FALSE;
        }
        state.buttons.push_back(button);
        state.ownedButtons.insert(button);
    }

    if (m_hWnd) {
        int existing = static_cast<int>(::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0));
        while (existing-- > 0) {
            ::SendMessageW(m_hWnd, TB_DELETEBUTTON, 0, 0);
        }

        for (CMFCToolBarButton* button : state.buttons) {
            TBBUTTON tb = {};
            tb.iBitmap = button->m_iImage;
            tb.idCommand = static_cast<int>(button->m_nID);
            tb.fsState = TBSTATE_ENABLED;
            tb.fsStyle = button->m_nID == 0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
            ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, reinterpret_cast<LPARAM>(&tb));
        }
    }

    return TRUE;
}
BOOL CMFCToolBar::ReplaceButton(UINT nID, const CMFCToolBarButton& button, BOOL) {
    ToolBarState& state = g_toolBarStates[this];
    for (size_t i = 0; i < state.buttons.size(); ++i) {
        CMFCToolBarButton* current = state.buttons[i];
        if (!current || current->m_nID != nID) continue;

        CMFCToolBarButton* replacement = CloneToolBarButton(button);
        if (!replacement) return FALSE;

        if (state.ownedButtons.erase(current) != 0) {
            delete current;
        }
        state.buttons[i] = replacement;
        state.ownedButtons.insert(replacement);

        if (m_hWnd) {
            TBBUTTONINFO tbi = {};
            tbi.cbSize = sizeof(TBBUTTONINFO);
            tbi.dwMask = TBIF_COMMAND | TBIF_IMAGE | TBIF_STATE | TBIF_STYLE;
            tbi.idCommand = static_cast<int>(replacement->m_nID);
            tbi.iImage = replacement->m_iImage;
            tbi.fsState = TBSTATE_ENABLED;
            tbi.fsStyle = replacement->m_nID == 0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
            ::SendMessageW(m_hWnd, TB_SETBUTTONINFO, nID, reinterpret_cast<LPARAM>(&tbi));
        }
        return TRUE;
    }
    return FALSE;
}
int CMFCToolBar::GetCount() const {
    auto it = g_toolBarStates.find(this);
    return it == g_toolBarStates.end() ? 0 : static_cast<int>(it->second.buttons.size());
}
CMFCToolBarButton* CMFCToolBar::GetButton(int nIndex) const {
    auto it = g_toolBarStates.find(this);
    if (it == g_toolBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.buttons.size())) return nullptr;
    return it->second.buttons[nIndex];
}
void CMFCToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage) {
    for (auto& entry : g_toolBarStates) {
        entry.second.buttonSize = CSize(sizeButton);
        entry.second.imageSize = CSize(sizeImage);
        if (entry.first && entry.first->GetSafeHwnd()) {
            ::SendMessageW(entry.first->GetSafeHwnd(), TB_SETBUTTONSIZE, 0, MAKELPARAM(sizeButton.cx, sizeButton.cy));
            ::SendMessageW(entry.first->GetSafeHwnd(), TB_SETBITMAPSIZE, 0, MAKELPARAM(sizeImage.cx, sizeImage.cy));
        }
    }
}
CSize CMFCToolBar::GetButtonSize() const {
    auto it = g_toolBarStates.find(this);
    return it == g_toolBarStates.end() ? CSize(23, 22) : it->second.buttonSize;
}
void CMFCToolBar::EnableDocking(DWORD dwDockStyle) {
    g_toolBarStates[this].dockStyle = dwDockStyle;
    // Retail's override is exactly two stores:
    //     mov DWORD PTR [rcx+0x108],0x1   ; m_dwControlBarStyle = 1
    //     mov DWORD PTR [rcx+0x100],edx   ; m_dwEnabledAlignment = dwDockStyle
    // The side table above is what OpenMFC's own docking code reads, but these
    // members are what a real client sees, and previously they were left at
    // zero -- so an MSVC caller could not tell a dockable toolbar from a fixed
    // one. Note the literal is assigned, not OR'd: retail discards any style
    // bits already present, and that is reproduced rather than "corrected".
    m_dwControlBarStyle  = 1;
    m_dwEnabledAlignment = dwDockStyle;
}
void CMFCToolBar::AdjustLayout() {
    ToolBarState& state = g_toolBarStates[this];
    const int count = static_cast<int>(state.buttons.size());
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(state.buttonSize.cx, state.buttonSize.cy));
        ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(state.imageSize.cx, state.imageSize.cy));
        ::SendMessageW(m_hWnd, TB_AUTOSIZE, 0, 0);
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
    }

    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& paneState = g_paneCoreState_Mfccore[this];
    if (paneState.recentRect.IsRectEmpty()) {
        paneState.recentRect = CRect(0, 0, std::max(1, count) * state.buttonSize.cx, state.buttonSize.cy);
    } else if (count > 0) {
        paneState.recentRect.right = paneState.recentRect.left + count * state.buttonSize.cx;
        paneState.recentRect.bottom = paneState.recentRect.top + state.buttonSize.cy;
    }
}
void CMFCToolBar::AdjustSize() {
    AdjustLayout();
}
CString CMFCToolBar::GetButtonText(int nIndex) const {
    CMFCToolBarButton* button = GetButton(nIndex);
    return button ? button->m_strText : CString();
}
void CMFCToolBar::GetButtonText(int nIndex, CString& rString) const {
    rString = GetButtonText(nIndex);
}
