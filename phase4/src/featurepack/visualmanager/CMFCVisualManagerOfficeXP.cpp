// CMFCVisualManagerOfficeXP — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void*, const CRect& rect, int nItem, int nState);

// Symbol: ?CreateObject@CMFCVisualManagerOfficeXP@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOfficeXP__SAPEAVCObject__XZ() { return new CMFCVisualManagerOfficeXP(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOfficeXP@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOfficeXP@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOfficeXP__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOfficeXP* pThis) { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCVisualManagerOfficeXP__IEAA_H_Z(void* pThis, int) {
    return new(pThis) CMFCVisualManagerOfficeXP();
}
// Symbol: ?OnDrawPropertySheetListItem@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCPropertySheet@@VCRect@@HH@Z
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void* pPropSheet, const CRect& rect, int nItem, int nState) {
    (void)pPropSheet;
    return impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
        pThis, pDC, pPropSheet, rect, nItem, nState);
}
CMFCVisualManagerOfficeXP::CMFCVisualManagerOfficeXP() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOfficeXP::~CMFCVisualManagerOfficeXP() {}
CObject* CMFCVisualManagerOfficeXP::CreateObject() { return new CMFCVisualManagerOfficeXP(); }
// Symbol: ?CreateGripperBrush@CMFCVisualManagerOfficeXP@@MEAAXXZ
extern "C" void MS_ABI impl__CreateGripperBrush_CMFCVisualManagerOfficeXP__MEAAXXZ() {}

// Symbol: ?ExtendMenuButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCMFCToolBarMenuButton@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__ExtendMenuButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCMFCToolBarMenuButton__AEAVCRect___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?GetAutoHideButtonTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCAutoHideButton@@@Z
extern "C" unsigned long MS_ABI impl__GetAutoHideButtonTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCAutoHideButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCToolBarMenuButton@@@Z
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetPropertyGridGroupColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCPropertyGridCtrl___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetPropertyGridGroupTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCPropertyGridCtrl___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetRibbonSliderColors@CMFCVisualManagerOfficeXP@@MEAAXPEAVCMFCRibbonSlider@@HHHAEAK1@Z
extern "C" void MS_ABI impl__GetRibbonSliderColors_CMFCVisualManagerOfficeXP__MEAAXPEAVCMFCRibbonSlider__HHHAEAK1_Z(void* /*class*/* p0, int p1, int p2, int p3, unsigned long* p4, int p5) {}

// Symbol: ?GetSmartDockingBaseGuideColors@CMFCVisualManagerOfficeXP@@MEAAXAEAK0@Z
extern "C" void MS_ABI impl__GetSmartDockingBaseGuideColors_CMFCVisualManagerOfficeXP__MEAAXAEAK0_Z(unsigned long* p0, unsigned long* p1) {}

// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, void* p4, void* /*struct*/ p5) {
    return 0;
}

// Symbol: ?GetWindowColor@CMFCVisualManagerOfficeXP@@MEBAKXZ
extern "C" unsigned long MS_ABI impl__GetWindowColor_CMFCVisualManagerOfficeXP__MEBAKXZ() {
    return 0;
}

// Symbol: ?OnDrawBarGripper@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, void* /*class*/* p3) {}

// Symbol: ?OnDrawBrowseButton@CMFCVisualManagerOfficeXP@@MEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@CMFCVisualManager@@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, unsigned long* p8) {
    return 0;
}

// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnDrawButtonSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, int p8) {}

// Symbol: ?OnDrawCaptionBarButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, int p6, int p7) {}

// Symbol: ?OnDrawCaptionBarInfoArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarInfoArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4, int p5, int p6) {}

// Symbol: ?OnDrawCaptionButtonIcon@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionButton@@W4IMAGES_IDS@CMenuImages@@HHVCPoint@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionButtonIcon_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(void* /*class*/* p0, void* /*class*/* p1, int /*enum*/ p2, unsigned int p3, float p4, unsigned short* p5, unsigned char p6, void* p7, int p8, unsigned int p9, char p10, void* p11, void* p12, float p13, void* p14, void* p15, void* p16, unsigned int p17, void* p18, void* p19, void* p20, void* p21, void* p22, int p23, int p24, void* /*class*/ p25) {}

// Symbol: ?OnDrawComboBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, void* /*class*/* p5) {}

// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, void* /*class*/* p5) {}

// Symbol: ?OnDrawEditBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawMenuArrowOnCustomizeList@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuArrowOnCustomizeList_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawMenuLabel@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return 0;
}

// Symbol: ?OnDrawMenuResizeBar@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuResizeBar_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnDrawMenuScrollButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawMenuScrollButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawMenuShadow@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEBVCRect@@1HHHPEAVCBitmap@@2H@Z
extern "C" void MS_ABI impl__OnDrawMenuShadow_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEBVCRect__1HHHPEAVCBitmap__2H_Z(void* /*class*/* p0, const void* /*class*/* p1, const void* /*class*/* p2, int p3, int p4, int p5, void* /*class*/* p6, int p7, int p8) {}

// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@IIH@Z
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(void* /*class*/* p0, void* /*class*/ p1, unsigned int p2, unsigned int p3, int p4) {}

// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawPaneBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, void* /*class*/ p3, void* /*class*/ p4) {
    return 0;
}

// Symbol: ?OnDrawPopupWindowBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnDrawPopupWindowButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {}

// Symbol: ?OnDrawPopupWindowCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWnd@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonColorPaletteBox@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonColorButton@@PEAVCMFCRibbonGalleryIcon@@KVCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonColorPaletteBox_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonColorButton__PEAVCMFCRibbonGalleryIcon__KVCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned long p3, void* /*class*/ p4, int p5, int p6, int p7, int p8, int p9) {}

// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonQuickAccessToolBarSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonSeparator@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonQuickAccessToolBarSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonSeparator__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonRecentFilesFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEBVCRect__HHH_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3) {}

// Symbol: ?OnDrawSpinButtons@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawSplitterBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCSplitterWndEx@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitterBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCSplitterWndEx__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawSplitterBox@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCSplitterWndEx@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitterBox_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCSplitterWndEx__AEAVCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned int p3, unsigned int p4) {}

// Symbol: ?OnDrawTab@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, const void* /*class*/* p4) {}

// Symbol: ?OnDrawTabCloseButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTabCloseButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawTask@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {}

// Symbol: ?OnDrawTasksGroupAreaBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3) {}

// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {}

// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {}

// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnFillBarBackground@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3, int p4) {}

// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnFillCaptionBarButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, int p6, int p7) {
    return 0;
}

// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {
    return 0;
}

// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnFillRibbonButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnFillRibbonMenuFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnFillTab@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3, int p4, const void* /*class*/* p5) {}

// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned long* p3) {}

// Symbol: ?OnHighlightQuickCustomizeMenuButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightQuickCustomizeMenuButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerOfficeXP@@MEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOfficeXP__MEAAXXZ() {}
