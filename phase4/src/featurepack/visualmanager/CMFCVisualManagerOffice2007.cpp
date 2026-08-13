// CMFCVisualManagerOffice2007 — OpenMFC implementation.
// Sources: global_mfc-feature-36_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCVisualManagerOffice2007 ribbon/status-bar drawing overrides
// (Wave 2, feature-36).
//
// Every symbol here is an MSVC-x64 virtual override of CMFCVisualManager
// exported by the retail mfc140u.dll under the CMFCVisualManagerOffice2007
// class. The retail bodies paint full Office2007-theme chrome (bitmap caches,
// theme colors, ribbon layout internals at offsets the OpenMFC headers do not
// model), so they cannot be transcribed faithfully.
//
// House pattern (matches mfccore.cpp for the other Office2007 overrides such as
// OnDrawButtonBorder/OnDrawMenuBorder): delegate to the corresponding
// CMFCVisualManager base-class implementation, which draws a generic but
// functional representation. The handful of overrides with no base-class
// virtual declared in the headers are emitted as type-correct conservative
// no-ops.

#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CMFCVisualManagerOffice2007 - Ribbon drawing overrides
//=============================================================================





























//=============================================================================
// CMFCVisualManagerOffice2007 - Status-bar drawing overrides
//=============================================================================



// Implementations this unit calls that are defined with their own class.
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void*, const CRect& rect, int nItem, int nState);

// Symbol: ?OnDrawRibbonButtonsGroup@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonButtonsGroup@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonButtonsGroup_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButtonsGroup__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButtonsGroup* pGroup, CRect rect)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail paints the Office2007
    // group theme (bitmap cache at 0x2c7b30 etc.); delegate to base drawing.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonButtonsGroup(pDC, pGroup, rect);
}
// Symbol: ?OnDrawRibbonCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawRibbonCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonBar__VCRect__2_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonBar* pBar, CRect rectCaption, CRect rectText)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // caption/title text via the ribbon bar internals; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonCaption(pDC, pBar, rectCaption, rectText);
}
// Symbol: ?OnDrawRibbonCaptionButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonCaptionButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCaptionButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonCaptionButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonCaptionButton* pButton)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the caption
    // button glyphs themed for Office2007; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonCaptionButton(pDC, pButton);
}
// Symbol: ?OnDrawRibbonCategory@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonCategory@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategory_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonCategory__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonCategory* pCategory, CRect rectCategory)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail renders the category
    // area (panels list at 0x5cf0 etc.); delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonCategory(pDC, pCategory, rectCategory);
}
// Symbol: ?OnDrawRibbonCategoryCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonContextCaption@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonContextCaption___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonContextCaption* pCaption)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // context-caption bar and returns its text color; delegate to base.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonCategoryCaption(pDC, pCaption);
}
// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonCategoryScroll* pScroll)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // scroll-arrow bitmap (cache at 0x2c7b30); delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonCategoryScroll(pDC, pScroll);
}
// Symbol: ?OnDrawRibbonCategoryTab@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonTab@@H@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryTab_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonTab__H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonTab* pTab, int bIsActive)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail paints the selected/
    // hover tab theme and returns the text color; delegate to base.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonCategoryTab(pDC, pTab, bIsActive);
}
// Symbol: ?OnDrawRibbonCheckBoxOnList@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonCheckBox@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawRibbonCheckBoxOnList_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonCheckBox__VCRect__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonCheckBox* pCheckBox, CRect rect, int bIsSelected, int bHighlighted)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail renders the themed
    // check box glyph on the customize list; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonCheckBoxOnList(pDC, pCheckBox, rect, bIsSelected, bHighlighted);
}
// Symbol: ?OnDrawRibbonDefaultPaneButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonDefaultPaneButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // default-pane button; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonDefaultPaneButton(pDC, pButton);
}
// Symbol: ?OnDrawRibbonDefaultPaneButtonIndicator@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawRibbonDefaultPaneButtonIndicator_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* /*pDC*/, CMFCRibbonButton* /*pButton*/, CRect /*rect*/, int /*bIsSelected*/, int /*bIsHighlighted*/)
{
    // No base-class virtual declared in the OpenMFC headers and the retail
    // body paints an Office2007-themed indicator bitmap.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnDrawRibbonGalleryBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonGallery@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonGalleryBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonGallery__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonGallery* pGallery, CRect rect)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // gallery border; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonGalleryBorder(pDC, pGallery, rect);
}
// Symbol: ?OnDrawRibbonGalleryButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonGalleryIcon@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonGalleryButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonGalleryIcon___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* /*pDC*/, void* /*pButton*/ /* CMFCRibbonGalleryIcon* -- not declared in headers */)
{
    // Retail paints the themed gallery-icon hover/pressed background. No
    // base-class virtual and no CMFCRibbonGalleryIcon declaration exist here.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnDrawRibbonKeyTip@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonBaseElement@@VCRect@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonKeyTip_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonBaseElement__VCRect__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* /*pDC*/, CMFCRibbonBaseElement* /*pElement*/, CRect /*rect*/, void** /*pstrKeyTip*/ /* CStringT (wchar_t) by value -> pointer to caller copy */)
{
    // Retail renders the KeyTip bubble using the ribbon's theme and the
    // key-tip string. No base-class virtual is declared in the headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnDrawRibbonLaunchButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonLaunchButton@@PEAVCMFCRibbonPanel@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonLaunchButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonLaunchButton__PEAVCMFCRibbonPanel___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* /*pDC*/, void* /*pButton*/ /* CMFCRibbonLaunchButton* -- not declared in headers */, CMFCRibbonPanel* /*pPanel*/)
{
    // Retail paints the themed panel-launch (chevron) button. No base-class
    // virtual and no CMFCRibbonLaunchButton declaration exist here.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnDrawRibbonMainPanelButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMainPanelButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed main
    // panel button border; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonMainPanelButtonBorder(pDC, pButton);
}
// Symbol: ?OnDrawRibbonMainPanelFrame@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMainPanelFrame_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonMainPanel* pPanel, CRect rect)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail frames the main panel
    // with the Office2007 theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonMainPanelFrame(pDC, pPanel, rect);
}
// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* /*pDC*/, CMFCRibbonButton* /*pButton*/, CRect /*rect*/)
{
    // Retail frames the check area of a ribbon menu item. No base-class
    // virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnDrawRibbonPanel@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonPanel@@VCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonPanel_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonPanel__VCRect__2_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonPanel* pPanel, CRect rectPanel, CRect rectCaption)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail paints the panel
    // background/caption chrome and returns the caption text color.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonPanel(pDC, pPanel, rectPanel, rectCaption);
}
// Symbol: ?OnDrawRibbonPanelCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonPanelCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonPanel__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonPanel* pPanel, CRect rectCaption)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail renders the themed
    // panel caption; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonPanelCaption(pDC, pPanel, rectCaption);
}
// Symbol: ?OnDrawRibbonRecentFilesFrame@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* /*pDC*/, CMFCRibbonMainPanel* /*pPanel*/, CRect /*rect*/)
{
    // Retail frames the "Recent Documents" list inside the main panel. No
    // base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnDrawRibbonSliderChannel@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderChannel_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // slider channel; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderChannel(pDC, pSlider, rect);
}
// Symbol: ?OnDrawRibbonSliderThumb@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderThumb_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect, int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // slider thumb for the hover/pressed/disabled states; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderThumb(pDC, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled);
}
// Symbol: ?OnDrawRibbonSliderZoomButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderZoomButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect, int bIsZoomOut, int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed zoom
    // (+/-) button for the hover/pressed/disabled states; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderZoomButton(pDC, pSlider, rect, bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
}
// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonStatusBar* pBar, CMFCRibbonStatusBarPane* pPane)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // status-bar pane and returns its text color; delegate to base.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonStatusBarPane(pDC, pBar, pPane);
}
// Symbol: ?OnDrawRibbonTabsFrame@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonBar@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonTabsFrame_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonBar__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonBar* pBar, CRect rectTab)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // tabs background and returns the tab text color; delegate to base.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonTabsFrame(pDC, pBar, rectTab);
}
// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEBVCRect__HHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, const CRect* rect, int nBorderSize, int iImage, int bHilited)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // scroll arrow buttons; delegate to base.
    pThis->CMFCVisualManager::OnDrawScrollButtons(pDC, *rect, nBorderSize, iImage, bHilited);
}
// Symbol: ?OnDrawSeparator@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // separator line; delegate to base.
    pThis->CMFCVisualManager::OnDrawSeparator(pDC, pBar, rect, bHorz);
}
// Symbol: ?OnDrawShowAllMenuItems@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawShowAllMenuItems_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // "show all menu items" bar; delegate to base.
    pThis->CMFCVisualManager::OnDrawShowAllMenuItems(pDC, rect, state);
}
// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectPane, unsigned int uiID, unsigned int nStyle)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // status-bar pane border; delegate to base.
    pThis->CMFCVisualManager::OnDrawStatusBarPaneBorder(pDC, pBar, rectPane, uiID, nStyle);
}
// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the themed
    // status-bar sizing grip; delegate to base.
    pThis->CMFCVisualManager::OnDrawStatusBarSizeBox(pDC, pBar, rectSizeBox);
}
// Symbol: ?CreateObject@CMFCVisualManagerOffice2007@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOffice2007__SAPEAVCObject__XZ() { return new CMFCVisualManagerOffice2007(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOffice2007@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2007__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOffice2007::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2007@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2007__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2007* pThis) { return CMFCVisualManagerOffice2007::GetThisClass(); }
// Symbol: ?SetStyle@CMFCVisualManagerOffice2007@@SAHW4Style@1@PEB_W@Z
extern "C" int MS_ABI impl__SetStyle_CMFCVisualManagerOffice2007__SAHW4Style_1_PEB_W_Z(int style, const wchar_t* lpszPath) {
    return CMFCVisualManagerOffice2007::SetStyle((CMFCVisualManagerOffice2007::Style)style, lpszPath);
}
// Symbol: ?GetStyle@CMFCVisualManagerOffice2007@@SA?AW4Style@1@XZ
extern "C" int MS_ABI impl__GetStyle_CMFCVisualManagerOffice2007__SA_AW4Style_1_XZ() {
    return (int)CMFCVisualManagerOffice2007::GetStyle();
}
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}
// Symbol: ?OnDrawPropertySheetListItem@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCPropertySheet@@VCRect@@HH@Z
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void* pPropSheet, const CRect& rect, int nItem, int nState) {
    (void)pPropSheet;
    return impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
        pThis, pDC, pPropSheet, rect, nItem, nState);
}
// Symbol: ?OnDrawRibbonProgressBar@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonProgressBar@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnDrawRibbonProgressBar_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonProgressBar__VCRect__2H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonProgressBar* pProgress,
    CRect rectProgress, CRect rectChunk, int bInfiniteMode) {
    pThis->OnDrawRibbonProgressBar(pDC, pProgress, rectProgress, rectChunk, bInfiniteMode);
}
static CMFCVisualManagerOffice2007::Style g_office2007Style = CMFCVisualManagerOffice2007::Office2007_LunaBlue;
CMFCVisualManagerOffice2007::CMFCVisualManagerOffice2007() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bAutoFreeRes_CMFCVisualManagerOffice2007__1HA = TRUE;
    impl__m_Style_CMFCVisualManagerOffice2007__1W4Style_1_A = static_cast<std::uint32_t>(g_office2007Style);
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOffice2007::~CMFCVisualManagerOffice2007() {}
CObject* CMFCVisualManagerOffice2007::CreateObject() { return new CMFCVisualManagerOffice2007(); }
int CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Style style, const wchar_t*) {
    if (style < Office2007_LunaBlue || style > Office2007_Aqua) {
        style = Office2007_LunaBlue;
    }
    g_office2007Style = style;
    impl__m_Style_CMFCVisualManagerOffice2007__1W4Style_1_A = static_cast<std::uint32_t>(style);
    ApplyVisualPaletteForClass(GetThisClass());
    if (g_pVisualManager && g_pVisualManager->IsKindOf(GetThisClass())) {
        SyncVisualManagerExports();
    }
    return TRUE;
}
CMFCVisualManagerOffice2007::Style CMFCVisualManagerOffice2007::GetStyle() { return g_office2007Style; }
// Symbol: ?m_strStylePrefix@CMFCVisualManagerOffice2007@@1V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_strStylePrefix_CMFCVisualManagerOffice2007__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = {};

// Symbol: ?CleanStyle@CMFCVisualManagerOffice2007@@SAXXZ
extern "C" void MS_ABI impl__CleanStyle_CMFCVisualManagerOffice2007__SAXXZ() {}

// Symbol: ?CleanUp@CMFCVisualManagerOffice2007@@MEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCVisualManagerOffice2007__MEAAXXZ() {}

// Symbol: ?DrawNcBtn@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEBVCRect@@IW4AFX_BUTTON_STATE@CMFCVisualManager@@HHH@Z
extern "C" void MS_ABI impl__DrawNcBtn_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEBVCRect__IW4AFX_BUTTON_STATE_CMFCVisualManager__HHH_Z(void* /*class*/* p0, const void* /*class*/* p1, unsigned int p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, int p8, int p9, int p10) {}

// Symbol: ?DrawNcCaption@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@VCRect@@KKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2PEAUHICON__@@HHHAEBVCObList@@@Z
extern "C" void MS_ABI impl__DrawNcCaption_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__VCRect__KKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2PEAUHICON____HHHAEBVCObList___Z(void* /*class*/* p0, void* /*class*/ p1, unsigned long p2, unsigned long p3, const void* /*class*/* p4, void** p5, void* p6, unsigned long p7, void* /*struct*/* p8, int p9, int p10, int p11, const void* /*class*/* p12) {}

// Symbol: ?DrawNcText@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEAVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2HHHHHHK@Z
extern "C" void MS_ABI impl__DrawNcText_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEAVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2HHHHHHK_Z(void* /*class*/* p0, void* /*class*/* p1, const void* /*class*/* p2, void** p3, void* p4, const void* /*class*/* p5, int p6, int p7, int p8, int p9, int p10, int p11, unsigned long p12) {}

// Symbol: ?DrawSeparator@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEBVCRect@@AEAVCPen@@2H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEBVCRect__AEAVCPen__2H_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, void* /*class*/* p3, int p4) {}

// Symbol: ?DrawSeparator@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEBVCRect__H_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2) {}

// Symbol: ?GetCaptionBarTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCCaptionBar@@@Z
extern "C" unsigned long MS_ABI impl__GetCaptionBarTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCCaptionBar___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCToolBarMenuButton@@@Z
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCToolBarMenuButton___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetMenuItemTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCToolBarMenuButton@@HH@Z
extern "C" unsigned long MS_ABI impl__GetMenuItemTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCToolBarMenuButton__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return 0;
}

// Symbol: ?GetNcBtnSize@CMFCVisualManagerOffice2007@@UEBA?AVCSize@@H@Z
extern "C" void* MS_ABI impl__GetNcBtnSize_CMFCVisualManagerOffice2007__UEBA_AVCSize__H_Z(void* /*class*/* p0, int p1) {
    return nullptr;
}

// Symbol: ?GetRibbonBar@CMFCVisualManagerOffice2007@@QEBAPEAVCMFCRibbonBar@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetRibbonBar_CMFCVisualManagerOffice2007__QEBAPEAVCMFCRibbonBar__PEAVCWnd___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetRibbonEditBackgroundColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCRibbonRichEditCtrl@@HHH@Z
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(void* /*class*/* p0, int p1, int p2, int p3) {
    return 0;
}

// Symbol: ?GetRibbonHyperlinkTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCRibbonLinkCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetRibbonHyperlinkTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCRibbonLinkCtrl___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetRibbonPopupBorderSize@CMFCVisualManagerOffice2007@@UEBAHPEBVCMFCRibbonPanelMenu@@@Z
extern "C" int MS_ABI impl__GetRibbonPopupBorderSize_CMFCVisualManagerOffice2007__UEBAHPEBVCMFCRibbonPanelMenu___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetRibbonQuickAccessToolBarRightMargin@CMFCVisualManagerOffice2007@@UEAAHXZ
extern "C" int MS_ABI impl__GetRibbonQuickAccessToolBarRightMargin_CMFCVisualManagerOffice2007__UEAAHXZ() {
    return 0;
}

// Symbol: ?GetRibbonStatusBarTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCRibbonStatusBar@@@Z
extern "C" unsigned long MS_ABI impl__GetRibbonStatusBarTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCRibbonStatusBar___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetShowAllMenuItemsHeight@CMFCVisualManagerOffice2007@@UEAAHPEAVCDC@@AEBVCSize@@@Z
extern "C" int MS_ABI impl__GetShowAllMenuItemsHeight_CMFCVisualManagerOffice2007__UEAAHPEAVCDC__AEBVCSize___Z(void* /*class*/* p0, const void* /*class*/* p1) {
    return 0;
}

// Symbol: ?GetSmartDockingTheme@CMFCVisualManagerOffice2007@@UEAA?AW4AFX_SMARTDOCK_THEME@@XZ
extern "C" void* MS_ABI impl__GetSmartDockingTheme_CMFCVisualManagerOffice2007__UEAA_AW4AFX_SMARTDOCK_THEME__XZ(int /*enum*/* p0, short* p1, int p2, void* p3, float p4, void** p5, void* p6, char p7, void* p8, void* p9, unsigned long p10, int p11, void* p12, int p13, unsigned char p14, float p15, unsigned char p16) {
    return nullptr;
}

// Symbol: ?GetStatusBarPaneTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCStatusBar@@PEAVCMFCStatusBarPaneInfo@@@Z
extern "C" unsigned long MS_ABI impl__GetStatusBarPaneTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?GetStyleResourceID@CMFCVisualManagerOffice2007@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@W4Style@1@@Z
extern "C" void* MS_ABI impl__GetStyleResourceID_CMFCVisualManagerOffice2007__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4Style_1__Z(void* /*class*/* p0, void** p1, void* p2, int /*enum*/ p3, void* p4, void* p5, void* p6, void* p7, void* p8, void** p9) {
    return nullptr;
}

// Symbol: ?GetSystemBorders@CMFCVisualManagerOffice2007@@IEBA?AVCSize@@H@Z
extern "C" void* MS_ABI impl__GetSystemBorders_CMFCVisualManagerOffice2007__IEBA_AVCSize__H_Z(void* /*class*/* p0, int p1) {
    return nullptr;
}

// Symbol: ?GetTabFrameColors@CMFCVisualManagerOffice2007@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerOffice2007__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(const void* /*class*/* p0, unsigned long* p1, unsigned long* p2, unsigned long* p3, unsigned long* p4, unsigned long* p5, unsigned long* p6, void* /*class*/** p7, void* /*class*/** p8) {}

// Symbol: ?GetTabHorzMargin@CMFCVisualManagerOffice2007@@UEAAHPEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__GetTabHorzMargin_CMFCVisualManagerOffice2007__UEAAHPEBVCMFCBaseTabCtrl___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?GetTabTextColor@CMFCVisualManagerOffice2007@@UEAAKPEBVCMFCBaseTabCtrl@@HH@Z
extern "C" unsigned long MS_ABI impl__GetTabTextColor_CMFCVisualManagerOffice2007__UEAAKPEBVCMFCBaseTabCtrl__HH_Z(const void* /*class*/* p0, int p1, int p2) {
    return 0;
}

// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, void* p4, void* /*struct*/ p5) {
    return 0;
}

// Symbol: ?GetToolTipInfo@CMFCVisualManagerOffice2007@@UEAAHAEAVCMFCToolTipInfo@@I@Z
extern "C" int MS_ABI impl__GetToolTipInfo_CMFCVisualManagerOffice2007__UEAAHAEAVCMFCToolTipInfo__I_Z(void* /*class*/* p0, unsigned int p1) {
    return 0;
}

// Symbol: ?IsHighlightWholeMenuItem@CMFCVisualManagerOffice2007@@UEAAHXZ
extern "C" int MS_ABI impl__IsHighlightWholeMenuItem_CMFCVisualManagerOffice2007__UEAAHXZ() {
    return 0;
}

// Symbol: ?IsOwnerDrawMenuCheck@CMFCVisualManagerOffice2007@@UEAAHXZ
extern "C" int MS_ABI impl__IsOwnerDrawMenuCheck_CMFCVisualManagerOffice2007__UEAAHXZ() {
    return 0;
}

// Symbol: ?IsRibbonPresent@CMFCVisualManagerOffice2007@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__IsRibbonPresent_CMFCVisualManagerOffice2007__QEBAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsWindowActive@CMFCVisualManagerOffice2007@@IEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__IsWindowActive_CMFCVisualManagerOffice2007__IEBAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?MakeResourceID@CMFCVisualManagerOffice2007@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void* MS_ABI impl__MakeResourceID_CMFCVisualManagerOffice2007__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(void* /*class*/* p0, void** p1, void* p2, const wchar_t* p3) {
    return nullptr;
}

// Symbol: ?OnDrawBarGripper@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, void* /*class*/* p3) {}

// Symbol: ?OnDrawButtonSeparator@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, int p8) {}

// Symbol: ?OnDrawCaptionBarInfoArea@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarInfoArea_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawComboBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, void* /*class*/* p5) {}

// Symbol: ?OnDrawDefaultRibbonImage@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawEditBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawHeaderCtrlBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {}

// Symbol: ?OnDrawMenuCheck@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawMenuCheck_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4) {}

// Symbol: ?OnDrawMenuItemButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawMenuItemButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4) {}

// Symbol: ?OnDrawMenuLabel@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return 0;
}

// Symbol: ?OnDrawMenuResizeBar@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuResizeBar_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnDrawMenuScrollButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawMenuScrollButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, int p5) {}

// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@IIH@Z
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__IIH_Z(void* /*class*/* p0, void* /*class*/ p1, unsigned int p2, unsigned int p3, int p4) {}

// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Symbol: ?OnDrawOutlookBarSplitter@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawOutlookBarSplitter_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnDrawOutlookPageButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEAVCRect__HH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3) {}

// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, void* /*class*/ p3, void* /*class*/ p4) {
    return 0;
}

// Symbol: ?OnDrawPopupWindowCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWnd@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnDrawRibbonApplicationButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonApplicationButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?OnDrawTab@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, const void* /*class*/* p4) {}

// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned int p3, void* /*class*/* p4) {}

// Symbol: ?OnDrawTask@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {}

// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4) {}

// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnEraseMDIClientArea@CMFCVisualManagerOffice2007@@UEAAHPEAVCDC@@VCRect@@@Z
extern "C" int MS_ABI impl__OnEraseMDIClientArea_CMFCVisualManagerOffice2007__UEAAHPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return 0;
}

// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {}

// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerOffice2007@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerOffice2007__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {
    return 0;
}

// Symbol: ?OnFillBarBackground@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3, int p4) {}

// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Symbol: ?OnFillCaptionBarButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, int p6, int p7) {
    return 0;
}

// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?OnFillOutlookBarCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookBarCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/ p1, unsigned long* p2) {}

// Symbol: ?OnFillOutlookPageButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(void* /*class*/* p0, const void* /*class*/* p1, int p2, int p3, unsigned long* p4) {}

// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnFillRibbonButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnFillRibbonEdit@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonRichEditCtrl@@VCRect@@HHHAEAK33@Z
extern "C" void MS_ABI impl__OnFillRibbonEdit_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonRichEditCtrl__VCRect__HHHAEAK33_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, unsigned long* p6, unsigned long* p7, unsigned long* p8) {}

// Symbol: ?OnFillRibbonMainPanelButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonMainPanelButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnFillRibbonMenuFrame@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnFillTab@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3, int p4, const void* /*class*/* p5) {}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned long* p3) {}

// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnNcActivate@CMFCVisualManagerOffice2007@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnNcActivate_CMFCVisualManagerOffice2007__UEAAHPEAVCWnd__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnNcPaint@CMFCVisualManagerOffice2007@@UEAAHPEAVCWnd@@AEBVCObList@@VCRect@@@Z
extern "C" int MS_ABI impl__OnNcPaint_CMFCVisualManagerOffice2007__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnSetWindowRegion@CMFCVisualManagerOffice2007@@UEAAHPEAVCWnd@@VCSize@@@Z
extern "C" int MS_ABI impl__OnSetWindowRegion_CMFCVisualManagerOffice2007__UEAAHPEAVCWnd__VCSize___Z(void* /*class*/* p0, void* /*class*/ p1) {
    return 0;
}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerOffice2007@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOffice2007__UEAAXXZ() {}

// Symbol: ?SetResourceHandle@CMFCVisualManagerOffice2007@@SAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__SetResourceHandle_CMFCVisualManagerOffice2007__SAXPEAUHINSTANCE_____Z(void* /*struct*/* p0) {}
