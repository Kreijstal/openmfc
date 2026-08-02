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
extern "C" void MS_ABI   impl__OnCancelMode_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI   impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(CMFCToolBar* pThis, int nHot);
extern "C" void MS_ABI   impl__OnAfterButtonDelete_CMFCToolBar__MEAAXXZ(CMFCToolBar* pThis);

// Functions implemented in this file that are referenced by earlier functions.
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBar__UEAAHH_Z(CMFCToolBar* pThis, int iIndex);
extern "C" void MS_ABI impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);

//=============================================================================
// CMFCToolBar::OnSetFocus
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnSettingChange
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnShowWindow
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnSize
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnSysColorChange
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolHitTest
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarAppearance
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarDelete
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarImage
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarImageAndText
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarNewMenu
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarReset
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarStartGroup
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnToolbarText
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnUpdateCmdUI
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnUpdateToolTips
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnUserToolTip
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnWindowPosChanged
//=============================================================================
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

//=============================================================================
// CMFCToolBar::OnWindowPosChanging
//=============================================================================
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

//=============================================================================
// CMFCToolBar::PreTranslateMessage
//=============================================================================
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

//=============================================================================
// CMFCToolBar::PrevMenu
//=============================================================================
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

//=============================================================================
// CMFCToolBar::ProcessCommand
//=============================================================================
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

//=============================================================================
// CMFCToolBar::RebuildAccelerationKeys
//=============================================================================
// Retail (0x1801563b0) clears the m_AccelKeys CMap (0x1318) and rebuilds it
// from every button's accelerator (&x) text. The CMap internals are not
// modeled.
// Symbol: ?RebuildAccelerationKeys@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildAccelerationKeys_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    // TODO(clean-room): not transcribed -- needs the m_AccelKeys CMap internals.
    (void)pThis;
}

//=============================================================================
// CMFCToolBar::RedrawCustomizeButton
//=============================================================================
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

//=============================================================================
// CMFCToolBar::RedrawUnderlines (static)
//=============================================================================
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

//=============================================================================
// CMFCToolBar::RemoveAllButtons
//=============================================================================
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

//=============================================================================
// CMFCToolBar::RemoveButton
//=============================================================================
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

//=============================================================================
// CMFCToolBar::RemoveResetStateButton
//=============================================================================
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

//=============================================================================
// CMFCToolBar::RemoveStateFromRegistry
//=============================================================================
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

//=============================================================================
// CMFCToolBar::ResetAll (static)
//=============================================================================
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
