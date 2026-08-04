// CMFCOutlookBarToolBar — OpenMFC implementation.
// Sources: global_cmfcoutlookbartoolbar.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// OpenMFC: CMFCOutlookBarToolBar exports.
//
// All 8 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the public headers, so `this` is taken as void*
// and the touched member offsets are mirrored with raw offsets.
// CMFCOutlookBarToolBar derives from CMFCToolBar and is 5008 bytes (0x1390);
// the CMFCToolBar base is declared in afxmfc.h (0..4944 = 0x1350) and the
// class's own region starts at 0x1350.  OnCustomizeMode, OnNcCalcSize,
// OnNcPaint and OnUserToolTip are transcribed faithfully; the bodies that need
// the unmodeled vtable slots or the parent outlook bar are type-correct safe
// defaults with the retail return value.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CMFCToolBar::OnCustomizeMode (global_mfc-feature-3_impl.cpp).
extern "C" void MS_ABI impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(CMFCToolBar* pThis, int bCustomize);
// CWnd::EnableWindow (wincore.cpp).
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
// CMFCToolBar::UpdateTooltips (global_mfc-feature-5_impl.cpp).
extern "C" void MS_ABI impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);

//=============================================================================
// CMFCOutlookBarToolBar
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::AdjustLocations()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnCustomizeMode(BOOL bCustomize)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnNcCalcSize(BOOL bCalcValidRects,
//                                          NCCALCSIZE_PARAMS* lpncsp)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnNcPaint()
//=============================================================================

//=============================================================================
// int CMFCOutlookBarToolBar::OnSendCommand(const CMFCToolBarButton* pButton)
//=============================================================================

//=============================================================================
// int CMFCOutlookBarToolBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisable)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarToolBar::OnUserToolTip(CMFCToolBarButton* pButton,
//                                           CString& rString)
//=============================================================================
// Retail (RVA 0x9b750): scales m_sizeImageLocked@0x1160 by the global layout
// factor, lays out the page buttons and the customize button, then rebuilds the
// tooltips.  The layout math and the button iteration are not modeled; only the
// tooltip rebuild terminal is reproduced.
// Symbol: ?AdjustLocations@CMFCOutlookBarToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCOutlookBarToolBar__UEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- image scaling, button layout
    // and the m_pCustomizeBtn special case are not modeled.
    impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(reinterpret_cast<CMFCToolBar*>(pThis));
}
// Retail (RVA 0x9b9c0):
//     CMFCToolBar::OnCustomizeMode(this, bCustomize);
//     return CWnd::EnableWindow(this, bCustomize == 0);   // tail call
// Symbol: ?OnCustomizeMode@CMFCOutlookBarToolBar@@UEAAXH@Z
extern "C" void MS_ABI impl__OnCustomizeMode_CMFCOutlookBarToolBar__UEAAXH_Z(
    void* pThis, int bCustomize)
{
    if (!pThis) return;
    impl__OnCustomizeMode_CMFCToolBar__MEAAXH_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), bCustomize);
    impl__EnableWindow_CWnd__QEAAHH_Z(
        reinterpret_cast<CWnd*>(pThis), bCustomize == 0);
}
// Retail (RVA 0x27d0) is a bare `ret $0` -- genuine no-op.
// Symbol: ?OnNcCalcSize@CMFCOutlookBarToolBar@@QEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCOutlookBarToolBar__QEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    (void)pThis; (void)bCalcValidRects; (void)lpncsp;
}
// Retail (RVA 0x27d0) is a bare `ret $0` -- genuine no-op.
// Symbol: ?OnNcPaint@CMFCOutlookBarToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCOutlookBarToolBar__QEAAXXZ(void* pThis)
{
    (void)pThis;
}
// Retail (RVA 0x9b510): resolves the button's index through the internal
// +0x1350 per-button info array, compares it against the hot button of the
// internal toolbar (+0x1388) and dispatches WM_COMMAND to the parent frame.
// The internal arrays are not modeled, so the base terminal (0) is returned.
// Symbol: ?OnSendCommand@CMFCOutlookBarToolBar@@UEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCOutlookBarToolBar__UEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton* pButton)
{
    (void)pButton;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- +0x1350 info array, hot-button
    // compare and the WM_COMMAND dispatch are not modeled.
    return 0;
}
// Retail (RVA 0x9b680): GetCursorPos/ScreenToClient then hit-tests the tab area
// through the vslot-0x730 virtual; when over a page button it sets the hand
// cursor (AFX_GLOBAL_DATA::GetHandCursor) and returns 1, otherwise it forwards
// to CMFCToolBar::OnSetCursor (0x1801547b0).  Neither the vslot nor the base
// impl is modeled, so 0 (not handled) is returned.
// Symbol: ?OnSetCursor@CMFCOutlookBarToolBar@@QEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCOutlookBarToolBar__QEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message)
{
    (void)pWnd; (void)nHitTest; (void)message;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- vslot-0x730 hit-test, hand
    // cursor and the base OnSetCursor fallback are not modeled.
    return 0;
}
// Retail (RVA 0x9b5d0): walks m_Buttons (CObList @0x1188, count @0x11a0), reads
// the per-button info from the internal +0x1350 array, compares the hot button
// of the internal toolbar (+0x1388) and re-asserts button styles through the
// vslot-0x6f8 SetButtonStyle.  The internal arrays/vslots are not modeled.
// Symbol: ?OnUpdateCmdUI@CMFCOutlookBarToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCOutlookBarToolBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler)
{
    (void)pTarget; (void)bDisableIfNoHndler;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- +0x1350 info array, hot-button
    // compare and the vslot-0x6f8 style update are not modeled.
}
// Retail (RVA 0x9b730):
//     rString = pButton->m_strText;      // CStringT::operator= (button + 0x38)
//     return TRUE;
// Symbol: ?OnUserToolTip@CMFCOutlookBarToolBar@@UEBAHPEAVCMFCToolBarButton@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUserToolTip_CMFCOutlookBarToolBar__UEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, CMFCToolBarButton* pButton, CString* pString)
{
    (void)pThis;
    if (!pButton || !pString) return FALSE;
    *pString = pButton->m_strText;
    return TRUE;
}
// Symbol: ??0CMFCOutlookBarToolBar@@QEAA@PEAVCMFCOutlookBarTabCtrl@@@Z
extern "C" void* MS_ABI impl___0CMFCOutlookBarToolBar__QEAA_PEAVCMFCOutlookBarTabCtrl___Z(void* pThis, void* pTabCtrl) {
    (void)pTabCtrl;
    return pThis;
}
