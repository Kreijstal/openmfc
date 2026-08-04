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
