// CMFCToolBar Feature Pack exports, wave 3 ("mfc-feature-3").
//
// Each function is transcribed from the retail mfc140u export
// (ordinal -> RVA -> objdump) against the member layout documented in
// include/openmfc/afxmfc.h (CMFCToolBar own members run 0x3F8..0x1350, i.e.
// m_ImagesLocked .. m_AccelKeys). Where the retail body needs internal helpers
// or virtual slots that OpenMFC does not model, the implementation keeps the
// visible side effects (member writes, window invalidation) and returns the
// safe default, with a `// TODO(clean-room)` marker.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Statics with storage in feature_static_data.cpp.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void*        impl__m_pLastHookedToolbar_CMFCToolBar__1PEAV1_EA;

// Forward declaration: NextMenu, OnCancelMode and OnContextMenu call
// OnChangeHot, which is defined later in this file.
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot);

// CMFCToolBarInfo is not declared in OpenMFC headers. The layout-faithful mirror
// below (8 x UINT, size 32) is identical to the one used by the exported
// CMFCToolBarInfo ctor in global_cmfctoolbarinfo.cpp, so the by-reference
// parameter matches the MSVC-mangled AEAVCMFCToolBarInfo@@ type exactly.
struct CMFCToolBarInfo {
    UINT m_uiColdResID;          // +0  Toolbar regular images
    UINT m_uiHotResID;           // +4  Toolbar "hot" images
    UINT m_uiDisabledResID;      // +8  Toolbar disabled images
    UINT m_uiLargeColdResID;     // +12 Toolbar large regular images
    UINT m_uiLargeHotResID;      // +16 Toolbar large "hot" images
    UINT m_uiLargeDisabledResID; // +20 Toolbar large disabled images
    UINT m_uiMenuResID;          // +24 Menu images
    UINT m_uiMenuDisabledResID;  // +28 Menu disabled images
};
static_assert(sizeof(CMFCToolBarInfo) == 32, "CMFCToolBarInfo size must be 32");

// CSettingsStore is not declared in OpenMFC headers either. Only a forward
// declaration is used so the by-reference parameter matches AEAVCSettingsStore@@;
// the two functions that take it never dereference the object.
class CSettingsStore;

//=============================================================================
// CMFCToolBar::InvalidateButton
//=============================================================================
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

//=============================================================================
// CMFCToolBar::IsCommandRarelyUsed (static)
//=============================================================================
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

//=============================================================================
// CMFCToolBar::IsLastCommandFromButton (static)
//=============================================================================
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

//=============================================================================
// CMFCToolBar::IsUserDefined
//=============================================================================
// Retail (0x1801596a0): consults a global pane-collection singleton
// (0x1803be1b8), two RTTI checks and an internal map lookup. Not modeled.
// Symbol: ?IsUserDefined@CMFCToolBar@@QEBAHXZ
extern "C" int MS_ABI impl__IsUserDefined_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed.
    return FALSE;
}

//=============================================================================
// CMFCToolBar::LoadBitmapEx
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadBitmapW
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadLargeIconsState (static)
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadLastOriginalState
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadParameters (static)
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadResetOriginalState
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadState
//=============================================================================
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

//=============================================================================
// CMFCToolBar::LoadToolBarEx
//=============================================================================
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

//=============================================================================
// CMFCToolBar::NextMenu
//=============================================================================
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

//=============================================================================
// CMFCToolBar::NotifyControlCommand
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnAfterButtonDelete
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnAfterDock
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnAfterFloat
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnBeforeChangeParent
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnCalcSeparatorRect
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnCancelMode
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnChangeHot
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnChangeVisualManager
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnCommand
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnContextMenu
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnCopyImage
//=============================================================================
// Retail (0x180157230): copies the image of the selected button (m_iSelected)
// into the user-images store via several internal image-list helpers and global
// static CMFCToolBarImages objects. Not modeled.
// Symbol: ?OnCopyImage@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnCopyImage_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed -- internal image-copy helpers skipped.
}

//=============================================================================
// CMFCToolBar::OnCreate
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnCtlColor
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnCustomizeMode
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnDestroy
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnDragEnter
//=============================================================================
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
