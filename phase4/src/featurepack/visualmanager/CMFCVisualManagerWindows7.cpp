// CMFCVisualManagerWindows7 — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CMFCVisualManagerWindows7@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerWindows7__SAPEAVCObject__XZ() { return new CMFCVisualManagerWindows7(); }
// Symbol: ?GetThisClass@CMFCVisualManagerWindows7@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerWindows7::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows7@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows7__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows7* pThis) { return CMFCVisualManagerWindows7::GetThisClass(); }
// Symbol: ?SetStyle@CMFCVisualManagerWindows7@@SAHPEB_W@Z
extern "C" int MS_ABI impl__SetStyle_CMFCVisualManagerWindows7__SAHPEB_W_Z(const wchar_t* lpszPath) {
    return CMFCVisualManagerWindows7::SetStyle(lpszPath);
}
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}
// Symbol: ?OnDrawRibbonProgressBar@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonProgressBar@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnDrawRibbonProgressBar_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonProgressBar__VCRect__2H_Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCRibbonProgressBar* pProgress,
    CRect rectProgress, CRect rectChunk, int bInfiniteMode) {
    pThis->OnDrawRibbonProgressBar(pDC, pProgress, rectProgress, rectChunk, bInfiniteMode);
}
CMFCVisualManagerWindows7::CMFCVisualManagerWindows7() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerWindows7::~CMFCVisualManagerWindows7() {}
CObject* CMFCVisualManagerWindows7::CreateObject() { return new CMFCVisualManagerWindows7(); }
int CMFCVisualManagerWindows7::SetStyle(const wchar_t*) {
    ApplyVisualPaletteForClass(GetThisClass());
    return TRUE;
}
// Symbol: ?CleanStyle@CMFCVisualManagerWindows7@@SAXXZ
extern "C" void MS_ABI impl__CleanStyle_CMFCVisualManagerWindows7__SAXXZ() {}

// Symbol: ?CleanUp@CMFCVisualManagerWindows7@@UEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCVisualManagerWindows7__UEAAXXZ() {}

// Symbol: ?DrawNcBtn@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEBVCRect@@IW4AFX_BUTTON_STATE@CMFCVisualManager@@HHH@Z
extern "C" void MS_ABI impl__DrawNcBtn_CMFCVisualManagerWindows7__UEAAXPEAVCDC__AEBVCRect__IW4AFX_BUTTON_STATE_CMFCVisualManager__HHH_Z(void* /*class*/* p0, const void* /*class*/* p1, unsigned int p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, int p8, int p9, int p10) {}

// Symbol: ?DrawNcText@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEAVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HHHHHK@Z
extern "C" void MS_ABI impl__DrawNcText_CMFCVisualManagerWindows7__UEAAXPEAVCDC__AEAVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHHHHK_Z(void* /*class*/* p0, void* /*class*/* p1, const void* /*class*/* p2, void** p3, void* p4, int p5, int p6, int p7, int p8, int p9, unsigned long p10) {}

// Symbol: ?DrawSeparator@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEBVCRect@@AEAVCPen@@2H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCVisualManagerWindows7__UEAAXPEAVCDC__AEBVCRect__AEAVCPen__2H_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, void* /*class*/* p3, int p4) {}

// Symbol: ?GetRibbonBar@CMFCVisualManagerWindows7@@QEBAPEAVCMFCRibbonBar@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetRibbonBar_CMFCVisualManagerWindows7__QEBAPEAVCMFCRibbonBar__PEAVCWnd___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetRibbonEditBackgroundColor@CMFCVisualManagerWindows7@@UEAAKPEAVCMFCRibbonRichEditCtrl@@HHH@Z
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManagerWindows7__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {
    return 0;
}

// Symbol: ?GetRibbonPopupBorderSize@CMFCVisualManagerWindows7@@UEBAHPEBVCMFCRibbonPanelMenu@@@Z
extern "C" int MS_ABI impl__GetRibbonPopupBorderSize_CMFCVisualManagerWindows7__UEBAHPEBVCMFCRibbonPanelMenu___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetStyleResourceID@CMFCVisualManagerWindows7@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI impl__GetStyleResourceID_CMFCVisualManagerWindows7__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(void* /*class*/* p0, void** p1, void* p2) {
    return nullptr;
}

// Symbol: ?GetSystemBorders@CMFCVisualManagerWindows7@@IEBA?AVCSize@@H@Z
extern "C" void* MS_ABI impl__GetSystemBorders_CMFCVisualManagerWindows7__IEBA_AVCSize__H_Z(void* /*class*/* p0, int p1) {
    return nullptr;
}

// Symbol: ?IsHighlightWholeMenuItem@CMFCVisualManagerWindows7@@UEAAHXZ
extern "C" int MS_ABI impl__IsHighlightWholeMenuItem_CMFCVisualManagerWindows7__UEAAHXZ() {
    return 0;
}

// Symbol: ?IsOwnerDrawMenuCheck@CMFCVisualManagerWindows7@@UEAAHXZ
extern "C" int MS_ABI impl__IsOwnerDrawMenuCheck_CMFCVisualManagerWindows7__UEAAHXZ() {
    return 0;
}

// Symbol: ?IsRibbonPresent@CMFCVisualManagerWindows7@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__IsRibbonPresent_CMFCVisualManagerWindows7__QEBAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsWindowActive@CMFCVisualManagerWindows7@@IEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__IsWindowActive_CMFCVisualManagerWindows7__IEBAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?MakeResourceID@CMFCVisualManagerWindows7@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void* MS_ABI impl__MakeResourceID_CMFCVisualManagerWindows7__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(void* /*class*/* p0, void** p1, void* p2, const wchar_t* p3) {
    return nullptr;
}

// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawDefaultRibbonImage@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawMenuCheck@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawMenuCheck_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4) {}

// Symbol: ?OnDrawMenuLabel@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManagerWindows7__UEAAKPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return 0;
}

// Symbol: ?OnDrawRadioButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawRadioButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawRibbonApplicationButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonApplicationButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonCaption@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawRibbonCaption_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonBar__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawRibbonCaptionButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonCaptionButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCaptionButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonCaptionButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonCategory@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonCategory@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategory_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonCategory__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonCategoryCaption@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonContextCaption@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryCaption_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonContextCaption___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonCategoryTab@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonTab@@H@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryTab_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonTab__H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?OnDrawRibbonDefaultPaneButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonDefaultPaneButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonGalleryButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonGalleryIcon@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonGalleryButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonGalleryIcon___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonLaunchButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonLaunchButton@@PEAVCMFCRibbonPanel@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonLaunchButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonLaunchButton__PEAVCMFCRibbonPanel___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {}

// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonPanel@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonPanel@@VCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonPanel_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonPanel__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {
    return 0;
}

// Symbol: ?OnDrawRibbonPanelCaption@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonPanelCaption_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonPanel__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonRecentFilesFrame@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonSliderChannel@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderChannel_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawRibbonSliderThumb@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderThumb_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawRibbonSliderZoomButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderZoomButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, int p6) {}

// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnDrawRibbonTabsFrame@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonBar@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonTabsFrame_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnFillBarBackground@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3, int p4) {}

// Symbol: ?OnFillButtonInterior@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnFillMenuImageRect@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillMenuImageRect_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnFillRibbonButton@CMFCVisualManagerWindows7@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerWindows7__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned long* p3) {}

// Symbol: ?OnNcActivate@CMFCVisualManagerWindows7@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnNcActivate_CMFCVisualManagerWindows7__UEAAHPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnNcPaint@CMFCVisualManagerWindows7@@UEAAHPEAVCWnd@@AEBVCObList@@VCRect@@@Z
extern "C" int MS_ABI impl__OnNcPaint_CMFCVisualManagerWindows7__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerWindows7@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerWindows7__UEAAXXZ() {}

// Symbol: ?SetResourceHandle@CMFCVisualManagerWindows7@@SAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__SetResourceHandle_CMFCVisualManagerWindows7__SAXPEAUHINSTANCE_____Z(void* /*struct*/* p0) {}
