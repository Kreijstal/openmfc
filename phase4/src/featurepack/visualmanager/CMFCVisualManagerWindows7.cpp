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
