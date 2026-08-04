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
