// CMFCVisualManagerOffice2003 — OpenMFC implementation.
// Sources: global_mfc-feature-58_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCVisualManagerOffice2003 drawing overrides
// (Wave 2, mfc-feature-58).
//
// Every symbol here is an MSVC-x64 virtual override of CMFCVisualManager
// exported by the retail mfc140u.dll under the CMFCVisualManagerOffice2003
// class. The retail bodies paint full Office2003-theme chrome (theme colors,
// cached bitmaps, and ribbon/tab/status-bar internals at offsets the OpenMFC
// headers do not model), so they cannot be transcribed faithfully without
// inventing member layout.
//
// House pattern (matches global_mfc-feature-36_impl.cpp for the equivalent
// CMFCVisualManagerOffice2007 overrides): delegate to the corresponding
// CMFCVisualManager base-class implementation, which draws a generic but
// functional representation. The handful of overrides with no base-class
// virtual declared in the OpenMFC headers are emitted as type-correct
// conservative no-ops.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CMFCVisualManagerOffice2003 - Ribbon drawing overrides
//=============================================================================
















//=============================================================================
// CMFCVisualManagerOffice2003 - Erase/fill overrides with no base-class
// virtual declared in the OpenMFC headers (conservative no-ops)
//=============================================================================















// Symbol: ?OnDrawRibbonSliderChannel@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderChannel_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // slider channel through the ribbon theme globals; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderChannel(pDC, pSlider, rect);
}
// Symbol: ?OnDrawRibbonSliderThumb@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderThumb_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect,
    int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // slider thumb glyph; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderThumb(pDC, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled);
}
// Symbol: ?OnDrawRibbonSliderZoomButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderZoomButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect,
    int bIsZoomOut, int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // slider zoom-button glyph; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderZoomButton(pDC, pSlider, rect, bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
}
// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonStatusBar* pBar, CMFCRibbonStatusBarPane* pPane)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail fills the pane with
    // the Office2003 status-bar theme and returns the text color; delegate to base.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonStatusBarPane(pDC, pBar, pPane);
}
// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEBVCRect__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, const CRect& rect,
    int nBorderSize, int iImage, int bHilited)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the Office2003
    // scroll-button theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawScrollButtons(pDC, rect, nBorderSize, iImage, bHilited);
}
// Symbol: ?OnDrawSeparator@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // separator groove; delegate to base.
    pThis->CMFCVisualManager::OnDrawSeparator(pDC, pBar, rect, bHorz);
}
// Symbol: ?OnDrawShowAllMenuItems@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawShowAllMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail fills the "show all
    // menu items" bar with the Office2003 theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawShowAllMenuItems(pDC, rect, state);
}
// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectPane,
    unsigned int uiID, unsigned int nStyle)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the Office2003
    // status-bar pane bevels; delegate to base.
    pThis->CMFCVisualManager::OnDrawStatusBarPaneBorder(pDC, pBar, rectPane, uiID, nStyle);
}
// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // status-bar sizing grip; delegate to base.
    pThis->CMFCVisualManager::OnDrawStatusBarSizeBox(pDC, pBar, rectSizeBox);
}
// Symbol: ?OnDrawTab@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectTab, int iTab, int bIsActive,
    const CMFCBaseTabCtrl* pTabWnd)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // tab button theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawTab(pDC, rectTab, iTab, bIsActive, pTabWnd);
}
// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect& rect, CMFCButton* pButton,
    unsigned int uiState, CMFCBaseTabCtrl* pWndTab)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the Office2003
    // tab-button border; delegate to base.
    pThis->CMFCVisualManager::OnDrawTabsButtonBorder(pDC, rect, pButton, uiState, pWndTab);
}
// Symbol: ?OnDrawTask@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons,
    int bIsHighlighted, int bIsSelected)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the task row
    // (icon + hot-tracking background) for Office2003; delegate to base.
    pThis->CMFCVisualManager::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
}
// Symbol: ?OnDrawTasksGroupAreaBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bSpecial, int bNoTitle)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail frames the tasks-pane
    // group area with the Office2003 border; delegate to base.
    pThis->CMFCVisualManager::OnDrawTasksGroupAreaBorder(pDC, rect, bSpecial, bNoTitle);
}
// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* pGroup,
    int bIsHighlighted, int bIsSelected, int bCanCollapse)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // group caption bar; delegate to base.
    pThis->CMFCVisualManager::OnDrawTasksGroupCaption(pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
}
// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectCaption, int bIsActive)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail fills the tear-off
    // caption with the Office2003 active/inactive color; delegate to base.
    pThis->CMFCVisualManager::OnDrawTearOffCaption(pDC, rectCaption, bIsActive);
}
// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, CMFCDesktopAlertWndButton* /*pButton*/)
{
    // Retail erases the popup-window button area with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rectArea*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    // Retail erases the tab area with the Office2003 theme color. No base-class
    // virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, CMFCButton* /*pButton*/, CMFCBaseTabCtrl* /*pWndTab*/)
{
    // Retail erases a single tab button before redrawing it with the
    // Office2003 theme. No base-class virtual is declared in the headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerOffice2003@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    // Retail fills the frame around the tabs and reports whether it painted.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially
    return 0;
}
// Symbol: ?OnFillAutoHideButtonBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnFillAutoHideButtonBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, void* /*pButton*/ /* CMFCAutoHideButton* -- not declared in headers */)
{
    // Retail fills the auto-hide tab button background with the Office2003
    // theme. No base-class virtual and no CMFCAutoHideButton declaration
    // exist in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillBarBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail fills the bar with the
    // Office2003 gradient background; delegate to base.
    pThis->CMFCVisualManager::OnFillBarBackground(pDC, pBar, rectClient, rectClip, bNCArea);
}
// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CMFCToolBarButton* /*pButton*/, CRect /*rect*/, CMFCVisualManager::AFX_BUTTON_STATE /*state*/)
{
    // Retail fills the toolbar-button interior with the Office2003 state
    // gradient. No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bIsSelected*/)
{
    // Retail fills the customize commands list with the Office2003 theme and
    // returns the text color. No base-class virtual is declared.
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially
    return 0;
}
// Symbol: ?OnFillHeaderCtrlBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillHeaderCtrlBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CMFCHeaderCtrl* /*pHeaderCtrl*/, CDC* /*pDC*/, CRect /*rect*/)
{
    // Retail fills the header-control background with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, CBrush* /*pBrush*/, CMFCToolBarButton* /*pButton*/)
{
    // Retail fills the highlighted (hot-tracked) toolbar area with the
    // Office2003 highlight brush. No base-class virtual is declared.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillOutlookBarCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookBarCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__AEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rectCaption*/, unsigned long& /*clrText*/)
{
    // Retail fills the Outlook bar caption with the Office2003 gradient and
    // returns the caption text color through the reference. No base-class
    // virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillOutlookPageButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, const CRect& /*rect*/, int /*bIsHighlighted*/, int /*bIsPressed*/, unsigned long& /*clrText*/)
{
    // Retail fills an Outlook page navigation button with the Office2003
    // state gradient and reports its text color. No base-class virtual is
    // declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    // Retail fills the popup-window background with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillTab@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rectFill*/, CBrush* /*pBrush*/, int /*iTab*/, int /*bIsActive*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    // Retail fills the active/inactive tab body with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bSpecial*/)
{
    // Retail fills the tasks-group interior with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?CreateObject@CMFCVisualManagerOffice2003@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOffice2003__SAPEAVCObject__XZ() { return new CMFCVisualManagerOffice2003(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOffice2003@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2003@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2003__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2003* pThis) { return CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?OnDrawRibbonProgressBar@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonProgressBar@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnDrawRibbonProgressBar_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonProgressBar__VCRect__2H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonProgressBar* pProgress,
    CRect rectProgress, CRect rectChunk, int bInfiniteMode) {
    pThis->OnDrawRibbonProgressBar(pDC, pProgress, rectProgress, rectChunk, bInfiniteMode);
}
// Symbol: ?OnDrawStatusBarProgress@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@HHKKKH@Z
extern "C" void MS_ABI impl__OnDrawStatusBarProgress_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__HHKKKH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCStatusBar* pBar,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText, int bProgressText) {
    pThis->OnDrawStatusBarProgress(pDC, pBar, rectProgress, nProgressTotal, nProgressCurr,
                                   clrBar, clrProgressBarDest, clrProgressText, bProgressText);
}
CMFCVisualManagerOffice2003::CMFCVisualManagerOffice2003() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA = TRUE;
    impl__m_bStatusBarOfficeXPLook_CMFCVisualManagerOffice2003__1HA = TRUE;
    impl__m_bUseGlobalTheme_CMFCVisualManagerOffice2003__1HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOffice2003::~CMFCVisualManagerOffice2003() {}
CObject* CMFCVisualManagerOffice2003::CreateObject() { return new CMFCVisualManagerOffice2003(); }
