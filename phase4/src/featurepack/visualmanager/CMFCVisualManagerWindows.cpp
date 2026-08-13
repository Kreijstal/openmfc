// CMFCVisualManagerWindows — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CMFCVisualManagerWindows@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerWindows__SAPEAVCObject__XZ() { return new CMFCVisualManagerWindows(); }
// Symbol: ?GetThisClass@CMFCVisualManagerWindows@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows* pThis) { return CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}
// Symbol: ?OnDrawStatusBarProgress@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@HHKKKH@Z
extern "C" void MS_ABI impl__OnDrawStatusBarProgress_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__HHKKKH_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCStatusBar* pBar,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText, int bProgressText) {
    pThis->OnDrawStatusBarProgress(pDC, pBar, rectProgress, nProgressTotal, nProgressCurr,
                                   clrBar, clrProgressBarDest, clrProgressText, bProgressText);
}
CMFCVisualManagerWindows::CMFCVisualManagerWindows() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_b3DTabsXPTheme_CMFCVisualManagerWindows__2HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerWindows::~CMFCVisualManagerWindows() {}
CObject* CMFCVisualManagerWindows::CreateObject() { return new CMFCVisualManagerWindows(); }
// Symbol: ?GetButtonExtraBorder@CMFCVisualManagerWindows@@UEBA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetButtonExtraBorder_CMFCVisualManagerWindows__UEBA_AVCSize__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetCaptionButtonExtraBorder@CMFCVisualManagerWindows@@UEBA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetCaptionButtonExtraBorder_CMFCVisualManagerWindows__UEBA_AVCSize__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManagerWindows@@UEAAKPEAVCMFCToolBarMenuButton@@@Z
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerWindows__UEAAKPEAVCMFCToolBarMenuButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManagerWindows@@UEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerWindows__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, void* p4, void* /*struct*/ p5) {
    return 0;
}

// Symbol: ?IsDefaultWinXPPopupButton@CMFCVisualManagerWindows@@UEBAHPEAVCMFCDesktopAlertWndButton@@@Z
extern "C" int MS_ABI impl__IsDefaultWinXPPopupButton_CMFCVisualManagerWindows__UEBAHPEAVCMFCDesktopAlertWndButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsWinXPThemeAvailable@CMFCVisualManagerWindows@@SAHXZ
extern "C" int MS_ABI impl__IsWinXPThemeAvailable_CMFCVisualManagerWindows__SAHXZ() {
    return 0;
}

// Symbol: ?OnDrawBarGripper@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, void* /*class*/* p3) {}

// Symbol: ?OnDrawBrowseButton@CMFCVisualManagerWindows@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@CMFCVisualManager@@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerWindows__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, unsigned long* p8) {
    return 0;
}

// Symbol: ?OnDrawButtonSeparator@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, int p8) {}

// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4, int p5, int p6) {}

// Symbol: ?OnDrawCaptionButtonIcon@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@W4IMAGES_IDS@CMenuImages@@HHVCPoint@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionButtonIcon_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(void* /*class*/* p0, void* /*class*/* p1, int /*enum*/ p2, unsigned int p3, float p4, unsigned short* p5, unsigned char p6, void* p7, int p8, unsigned int p9, char p10, void* p11, void* p12, float p13, void* p14, void* p15, void* p16, unsigned int p17, void* p18, void* p19, void* p20, void* p21, void* p22, int p23, int p24, void* /*class*/ p25) {}

// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawComboBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, void* /*class*/* p5) {}

// Symbol: ?OnDrawControlBorder@CMFCVisualManagerWindows@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnDrawControlBorder_CMFCVisualManagerWindows__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnDrawEditBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawExpandingBox@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HK@Z
extern "C" void MS_ABI impl__OnDrawExpandingBox_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HK_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, unsigned long p3) {}

// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawHeaderCtrlBorder@CMFCVisualManagerWindows@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerWindows__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {}

// Symbol: ?OnDrawHeaderCtrlSortArrow@CMFCVisualManagerWindows@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawHeaderCtrlSortArrow_CMFCVisualManagerWindows__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3) {}

// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@IIH@Z
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__IIH_Z(void* /*class*/* p0, void* /*class*/ p1, unsigned int p2, unsigned int p3, int p4) {}

// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawOutlookPageButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEAVCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3) {}

// Symbol: ?OnDrawPaneBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCBasePane@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerWindows@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerWindows__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, void* /*class*/ p3, void* /*class*/ p4) {
    return 0;
}

// Symbol: ?OnDrawPopupWindowButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {}

// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEBVCRect__HHH_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawSeparator@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3) {}

// Symbol: ?OnDrawSpinButtons@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned int p3, unsigned int p4) {}

// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawTab@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, const void* /*class*/* p4) {}

// Symbol: ?OnDrawTabCloseButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTabCloseButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawTask@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {}

// Symbol: ?OnDrawTasksGroupAreaBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3) {}

// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {}

// Symbol: ?OnEraseTabsArea@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {}

// Symbol: ?OnEraseTabsButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerWindows@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerWindows__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnFillBarBackground@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3, int p4) {}

// Symbol: ?OnFillButtonInterior@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerWindows@@UEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerWindows__UEAAKPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {
    return 0;
}

// Symbol: ?OnFillMenuImageRect@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillMenuImageRect_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerWindows@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerWindows__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?OnFillOutlookPageButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2, int p3, unsigned long* p4) {}

// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnFillTasksPaneBackground@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillTasksPaneBackground_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned long* p3) {}

// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerWindows@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerWindows__UEAAXXZ() {}

// Symbol: ?SetOfficeStyleMenus@CMFCVisualManagerWindows@@QEAAXH@Z
extern "C" void MS_ABI impl__SetOfficeStyleMenus_CMFCVisualManagerWindows__QEAAXH_Z(int p0) {}
