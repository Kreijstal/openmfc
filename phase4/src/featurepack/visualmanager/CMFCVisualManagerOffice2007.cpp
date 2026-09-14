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
//
// That paragraph describes the FIRST batch in this file only. The second
// batch (from the "style/NC/menu/tab/toolbar overrides" banner below) was
// decoded from the retail disassembly rather than assumed: not every symbol
// there is a virtual override (CleanStyle, GetStyleResourceID and
// MakeResourceID are statics), several are transcribed in full, and each
// delegation states which retail callee it stands in for. Read the banner
// comment there before adding to it.

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
//=============================================================================
// CMFCVisualManagerOffice2007 - style/NC/menu/tab/toolbar overrides
//=============================================================================
//
// How this batch was decoded (retail mfc140.dll 14.51.36231, x64; RVAs below
// are that image's unless the text says mfc140u, and every one of them is the
// ENTRY of the named function, never an instruction inside it).
//
// A note on names that "have no RVA": mfc140_rva_symbols.json stores one
// symbol per address, so every export whose body was COMDAT-folded onto
// another export's loses its name there. That is a limitation of the file, not
// of the evidence. Where it mattered below, the fold was PROVED instead of
// guessed, by rebuilding name -> RVA from the sibling image: mfc140u.dll's
// export address table (it exports by ordinal only) crossed with the
// mfc140u ordinal list in mfc_complete_ordinal_mapping.json. That map is
// self-checking -- e.g. it puts the three-argument
// ?DrawSeparator@CMFCVisualManagerOffice2007@@...@H@Z at mfc140u 0x1a3d40,
// whose body forwards to 0x1a3d70 exactly as the ANSI 0x1a2340 forwards to
// 0x1a2370. Each "folded with" claim below cites the mfc140u RVA the two
// names share.
//
// Retail CMFCVisualManagerOffice2007 is a 0x10aa8-byte object -- the size is
// visible in the operator-new argument of ?CreateObject@CMFCVisualManagerOffice2007@@SAPEAVCObject@@XZ
// (0x197bd0: `mov $0x10aa8,%ecx; call operator new`). Everything past the
// CMFCVisualManager base is Office2007 theme state: CMFCToolBarImages
// sub-objects and arrays (the constructor at 0x197c10 runs
// ??0CMFCToolBarImages@@QEAA@XZ (0x169720) over +0x310, +0x4a8, +0x640, +0x7d8
// and the vector-ctor helper 0x2b5210 over the 2-element arrays at +0x970,
// +0xca0, +0xfd0, +0x1300, element stride 0x198), a CMapWordToPtr at +0x10a70,
// and a large block of COLORREF fields around +0xc5b0..+0xc7a0. OpenMFC
// declares the class as CMFCVisualManager plus `char _pad[64]`, so NONE of
// those offsets exist
// here and they must not be invented.
//
// Almost every override in this batch has the same retail shape:
//
//     if (!<predicate>) { return <Base>::Method(...); }   // non-themed path
//     ... paint/return from the Office2007 members ...    // themed path
//
// <predicate> is a non-exported helper compiled at RVA 0x1972e8. 0x1803ba380
// is ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A (a named export, not a guess), so
// the three .data addresses below are fields of that struct. Transcribed, the
// helper forces AFX_GLOBAL_DATA::Initialize() (0x6a5c0) on afxGlobalData when
// its leading dword is still 0, then returns TRUE only when
//     *(int*)0x1803ba608 > 8      (afxGlobalData + 0x288, colour depth)
//  && *(int*)0x1803ba5e4 == 0     (afxGlobalData + 0x264)
//  && *(int*)0x1803ba5e0 == 0     (afxGlobalData + 0x260)
//  && this->[+0x2f0]    != 0.
// The Office2007 constructor ??0CMFCVisualManagerOffice2007@@IEAA@XZ (0x197c10)
// stores 0 into +0x2f0 (`mov %r15,0x2f0(%rbx)` with r15 = 0); only loading the
// style resources sets it. OpenMFC never loads them, so in this build the
// predicate is constantly FALSE and the reachable retail path is always the
// non-themed one. That is the path implemented below, and it is why these
// bodies delegate rather than paint Office2007 chrome.
//
// One deviation to be explicit about: retail's non-themed branch lands in
// whichever ancestor declares the method, and the resolved callees in this
// batch live in four different ancestor classes -- CMFCVisualManagerOffice2003,
// CMFCVisualManagerWindows, CMFCVisualManagerOfficeXP and CMFCVisualManager
// (MFC's hierarchy nests them in that order; what the disassembly proves is
// only that all four appear as callees). Each function below names the callee
// its own fallback edge resolves to. OpenMFC's header derives
// CMFCVisualManagerOffice2007 directly from CMFCVisualManager, so the
// delegation target here is always CMFCVisualManager's implementation, which
// is the nearest available approximation of that callee, not the same function.

// Base-class entry points this unit calls through their extern "C" impl__
// thunks (all defined in CMFCVisualManager.cpp) rather than as C++ methods --
// either because OpenMFC's headers declare no matching CMFCVisualManager
// virtual, or because the thunk's parameter list is the one this override
// receives (OnDrawFloatingToolbarBorder takes CMFCBaseToolBar*, the header
// method takes CMFCToolBar*).
//
// NOTE on the first three: CMFCVisualManager.cpp currently defines them as
// generated stubs that take fewer parameters and ignore them (they return 0).
// They are declared here with the real retail parameter list so the call sites
// stay correct once those stubs are implemented; passing the extra arguments is
// harmless under the MS x64 ABI in the meantime.
extern "C" unsigned long MS_ABI impl__GetCaptionBarTextColor_CMFCVisualManager__UEAAKPEAVCMFCCaptionBar___Z(
    CMFCVisualManager* pThis, CMFCCaptionBar* pBar);
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarMenuButton___Z(
    CMFCVisualManager* pThis, CMFCToolBarMenuButton* pButton);
extern "C" unsigned long MS_ABI impl__GetMenuItemTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarMenuButton__HH_Z(
    CMFCVisualManager* pThis, CMFCToolBarMenuButton* pButton, int bHighlight, int bDisabled);
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
    CMFCVisualManager* pThis, void* pEdit, int bIsHighlighted, int bIsPaneHighlighted, int bIsDisabled);
extern "C" unsigned long MS_ABI impl__GetRibbonHyperlinkTextColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonLinkCtrl___Z(
    CMFCVisualManager* pThis, void* pLink);
extern "C" unsigned long MS_ABI impl__GetRibbonStatusBarTextColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonStatusBar___Z(
    CMFCVisualManager* pThis, CMFCRibbonStatusBar* pStatusBar);
extern "C" unsigned long MS_ABI impl__GetStatusBarPaneTextColor_CMFCVisualManager__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(
    CMFCVisualManager* pThis, CMFCStatusBar* pStatusBar, void* pPane);
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManager__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    CMFCVisualManager* pThis, const CMFCBaseTabCtrl* pTabWnd, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack);
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CMFCToolBarButton* pButton, int nState);
extern "C" int MS_ABI impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
    CMFCVisualManager* pThis, CMFCToolTipInfo& params, unsigned int nType);
extern "C" void MS_ABI impl__OnDrawCaptionBarInfoArea_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect);
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectImage, int bIsDisabled, int bIsPressed, int bIsHighlighted);
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCBaseToolBar* pBar, CRect rectBorder, CRect rectBorderSize);
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManager__UEAAKPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect);

// Symbol: ?m_strStylePrefix@CMFCVisualManagerOffice2007@@1V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
// Data export (a private static CString), not a function, so it has no RVA to
// disassemble. Retail fills it in when a style resource module is loaded and
// ?MakeResourceID@CMFCVisualManagerOffice2007@@KA... (ANSI twin at 0x198eb0)
// prepends it to every resource name. OpenMFC never loads a style module, so
// the prefix stays empty; the storage is left as a null CStringT pointer
// exactly as before, because handing out a CStringData that this build's
// ATL::CStringT does not own would be worse than an untouched slot.
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_strStylePrefix_CMFCVisualManagerOffice2007__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = {};

// Symbol: ?CleanStyle@CMFCVisualManagerOffice2007@@SAXXZ
// Retail (0x199190) releases the loaded style resource module. The three
// .data addresses it touches are themselves named exports, so they are not
// guesses: 0x1803b70fc is ?m_bAutoFreeRes@CMFCVisualManagerOffice2007@@1HA,
// 0x1803b7128 is ?m_hinstRes@CMFCVisualManagerOffice2007@@1PEAUHINSTANCE__@@EA
// and 0x1803b7130 is ?m_Style@CMFCVisualManagerOffice2007@@1W4Style@1@A.
// Transcribed:
//     if (m_bAutoFreeRes && (UINT_PTR)m_hinstRes > 0x20)
//         ::FreeLibrary(m_hinstRes);              // IAT slot 0x1802c4698
//     m_hinstRes = NULL; m_Style = 0;
//     m_strStylePrefix.Empty();                   // tail-jmp 0x180003430 =
//                                                 // ?Empty@?$CSimpleStringT@D$00@ATL@@QEAAXXZ
// OpenMFC has no style resource module and no populated m_strStylePrefix, so
// there is nothing to release. Left an explicit no-op rather than inventing
// state to clear.
extern "C" void MS_ABI impl__CleanStyle_CMFCVisualManagerOffice2007__SAXXZ() {}

// Symbol: ?CleanUp@CMFCVisualManagerOffice2007@@MEAAXXZ
// Retail (0x1991e0) resets the whole Office2007 theme block: it stores -1 into
// the 21 COLORREF fields at +0xc5e4..+0xc634 inclusive, then calls
// ?Clear@CMFCToolBarImages@@QEAAXXZ (0x16dcb0) on the CMFCToolBarImages
// members at +0x310, +0x4a8, +0x640, +0x7d8 ... and a virtual at vtable +0x60
// on the sub-objects at +0x2cf0, +0x22f0, +0x24f0, +0x26f0, +0x28f0 ...
// (whose class was not identified -- only the vtable slot is visible).
// None of those members exist in OpenMFC's 64-byte derived block, so there is
// no state to reset. Deliberately left a no-op.
extern "C" void MS_ABI impl__CleanUp_CMFCVisualManagerOffice2007__MEAAXXZ(void* /*pThis*/) {}

// Symbol: ?DrawNcBtn@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEBVCRect@@IW4AFX_BUTTON_STATE@CMFCVisualManager@@HHH@Z
// Retail (0x1a0a40) has NO non-themed fallback: it selects one of the embedded
// CMFCToolBarImages objects by the SC_ command id -- 0xf060 SC_CLOSE -> +0x970,
// 0xf020 SC_MINIMIZE -> +0x1300, 0xf030 SC_MAXIMIZE -> +0xfd0, 0xf120
// SC_RESTORE -> +0xca0, each +0x198 further when the bSmall argument is
// non-zero (they are 2-element arrays of stride 0x198) -- and draws it into
// the DC. Any other command id falls straight through to the epilogue and
// draws nothing. Those image objects and their bitmaps are Office2007 state
// OpenMFC does not model, so no faithful body is possible; left a stub rather
// than painting something invented.
extern "C" void MS_ABI impl__DrawNcBtn_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEBVCRect__IW4AFX_BUTTON_STATE_CMFCVisualManager__HHH_Z(
    void* /*pThis*/, CDC* /*pDC*/, const CRect* /*pRect*/, unsigned int /*uiSysCommand*/,
    int /*state*/, int /*bSmall*/, int /*bHorz*/, int /*bMaximized*/) {}

// Symbol: ?DrawNcCaption@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@VCRect@@KKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2PEAUHICON__@@HHHAEBVCObList@@@Z
// The Unicode symbol has no RVA in mfc140_rva_symbols.json (that file is built
// from mfc140.dll, the ANSI twin, so only the @D form appears); the body is at
// mfc140 0x1a10e0, and the Unicode export is at mfc140u 0x1a2ae0.
// It paints the whole themed non-client caption (gradient,
// icon, caption text via DrawNcText, and the button list) out of the manager's
// Office2007 renderers and colour block. Not modellable here; left a stub.
extern "C" void MS_ABI impl__DrawNcCaption_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__VCRect__KKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2PEAUHICON____HHHAEBVCObList___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rectCaption*/, unsigned long /*dwStyle*/,
    unsigned long /*dwStyleEx*/, const void* /*strTitle*/, const void* /*strDocument*/,
    HICON /*hIcon*/, int /*bActive*/, int /*bTextCenter*/, int /*bMaximized*/,
    const void* /*lstButtons*/) {}

// Symbol: ?DrawNcText@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEAVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2HHHHHHK@Z
// As above: the ANSI twin is at mfc140 0x1a0cc0, the Unicode export at
// mfc140u 0x1a26c0. Re-read 2026-09-14 (mfc140u): after the early-outs
// (both strings empty, or rect.right <= rect.left) it does
// SetBkMode(TRANSPARENT), saves ::GetTextColor(m_hAttribDC), builds
// DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER (0x8824, plus
// DT_RTLREADING when the 8th argument is set), and takes its text colours
// from the Office2007 block: the pairs this->[+0xc590 / +0xc594] and
// [+0xc598 / +0xc59c], each indexed by the 7th argument, the 6th argument
// choosing which string gets which pair (a lone title uses the second
// pair). It then measures with ::GetTextExtentPoint32W, centres when the
// 9th argument is set, and draws either through vftable +0x5a8
// (DrawTextOnGlass, when the 10th is set, passing the 11th and 12th
// through) or SetTextColor + DrawText. The parameter names on the stub
// below are the earlier generation's and were NOT re-derived here. Those
// four colours are the palette the style module loads; nothing in this
// build holds them, so the body is left a stub rather than painted in a
// colour retail never uses.
extern "C" void MS_ABI impl__DrawNcText_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEAVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2HHHHHHK_Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect* /*pRect*/, const void* /*strTitle*/,
    const void* /*strDocument*/, int /*bPrefix*/, int /*bActive*/, int /*bTextCenter*/,
    int /*bIsRTL*/, int /*nTextAlign*/, int /*bMaximized*/, unsigned long /*dwStyle*/) {}

namespace {
// The plain-GDI branch of retail's DrawSeparator (see the comment above),
// driven through the HDC: CDC::SelectObject / MoveTo / LineTo exist in this
// DLL only as export thunks, so they cannot be called as C++ methods here.
void Office2007DrawSeparatorLines(CDC* pDC, const CRect& rect, HPEN hPen1, HPEN hPen2, int bHorz)
{
    HDC hdc = pDC ? pDC->GetSafeHdc() : nullptr;
    if (!hdc) return;

    CRect rc(rect);
    CRect rc2;
    if (bHorz) {
        const int y = rc.top - 1 + (rc.bottom - rc.top) / 2;
        rc.top = y;
        rc.bottom = y;
        rc2 = CRect(rc.left, y, rc.right, y);
        rc2.OffsetRect(0, 1);
    } else {
        const int x = rc.left - 1 + (rc.right - rc.left) / 2;
        rc.left = x;
        rc.right = x;
        rc2 = CRect(x, rc.top, x, rc.bottom);
        rc2.OffsetRect(1, 0);
    }

    HGDIOBJ hOld = nullptr;
    if (hPen1 != nullptr) {
        hOld = ::SelectObject(hdc, hPen1);
        ::MoveToEx(hdc, rc.left, rc.top, nullptr);
        ::LineTo(hdc, rc.right, rc.bottom);
    }
    if (hPen2 != nullptr) {
        HGDIOBJ hPrev = ::SelectObject(hdc, hPen2);
        if (hOld == nullptr) hOld = hPrev;
        ::MoveToEx(hdc, rc2.left, rc2.top, nullptr);
        ::LineTo(hdc, rc2.right, rc2.bottom);
    }
    if (hOld != nullptr) ::SelectObject(hdc, hOld);
}
} // namespace

// Symbol: ?DrawSeparator@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEBVCRect@@AEAVCPen@@2H@Z
// Transcription of the retail body at 0x1a2370. That address carries the name
// ?DrawSeparator@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@AEBVCRect@@AEAVCPen@@2H@Z
// in the RVA map because the Office2007 five-argument export folded onto it.
// The fold is PROVED, twice: in mfc140u.dll both
// ?DrawSeparator@CMFCVisualManagerOffice2007@@IEAAX...AEAVCPen@@2H@Z and the
// CMFCVisualManagerWindows7 name resolve to the single RVA 0x1a3d70, and in
// mfc140.dll the Office2007 three-argument overload at 0x1a2340 -- a name that
// did keep its RVA -- reaches this body with a direct `call 0x1801a2370`.
//
// Retail, instruction for instruction:
//   rc  = rect; rc2 = rect;
//   bHorz : y = rc.top  - 1 + (rc.bottom - rc.top ) / 2;  rc.top = rc.bottom = y;
//           rc2 = (rc.left, y, rc.right, y); ::OffsetRect(&rc2, 0, 1);
//   else  : x = rc.left - 1 + (rc.right  - rc.left) / 2;  rc.left = rc.right = x;
//           rc2 = (x, rc.top, x, rc.bottom); ::OffsetRect(&rc2, 1, 0);
//   then, when the module global at .data 0x1803b70f4 is zero (the branch
//   taken below):
//           old = pDC->SelectObject(&pen1);        // 0x2a0670, the folded
//                                                  // ?SelectObject@CDC@@UEAAPEAVCFont@@PEAV2@@Z
//           pDC->MoveTo(rc.left,  rc.top);         // 0x2a1000
//           pDC->LineTo(rc.right, rc.bottom);      // 0x2a1060
//           pDC->SelectObject(&pen2);
//           pDC->MoveTo(rc2.left,  rc2.top);
//           pDC->LineTo(rc2.right, rc2.bottom);
//           pDC->SelectObject(old);
// DEVIATION: the .data global tested at 0x1a2427 is the named export
// ?m_bIsDrawOnGlass@CMFCToolBarImages@@2HA (0x1803b70f4). When it is non-zero
// retail instead reads each pen's LOGPEN with ::GetObject (GDI32 import slot
// 0x1802c4250) and rules both lines through
// ?DrawLine@CDrawingManager@@QEAAXHHHHK@Z (0x180059350) using the LOGPEN
// colour. OpenMFC has no draw-on-glass path, so only the plain GDI branch is
// implemented; that is the branch retail takes whenever m_bIsDrawOnGlass is
// FALSE, and the slot is zero-initialised in the image, so it stays FALSE
// until a caller turns it on.
// Retail also does not null-check the pens;
// the helper skips the corresponding line instead of drawing it with whatever
// pen the DC currently holds.

extern "C" void MS_ABI impl__DrawSeparator_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEBVCRect__AEAVCPen__2H_Z(
    void* pThis, CDC* pDC, const CRect* pRect, CPen* pPen1, CPen* pPen2, int bHorz)
{
    if (!pThis || !pRect) return;
    Office2007DrawSeparatorLines(pDC, *pRect,
                                 pPen1 ? (HPEN)pPen1->GetSafeHandle() : nullptr,
                                 pPen2 ? (HPEN)pPen2->GetSafeHandle() : nullptr,
                                 bHorz);
}

// Symbol: ?DrawSeparator@CMFCVisualManagerOffice2007@@IEAAXPEAVCDC@@AEBVCRect@@H@Z
// Retail (0x1a2340) is a pure forwarder:
//     DrawSeparator(pDC, rect, this->[+0x1e0], this->[+0xc530], bHorz);
// i.e. it hands the five-argument overload two CPen members of the manager.
// DEVIATION: OpenMFC models neither pen, so this creates the two pens locally.
// The colours are NOT from retail (they come from unloaded Office2007 style
// data); COLOR_3DSHADOW/COLOR_3DHILIGHT are chosen to match the separator
// colour CMFCVisualManager::OnDrawButtonSeparator already uses in this tree.
// The geometry is retail's, via the transcribed helper above.
extern "C" void MS_ABI impl__DrawSeparator_CMFCVisualManagerOffice2007__IEAAXPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* pDC, const CRect* pRect, int bHorz)
{
    if (!pThis || !pRect) return;
    HPEN hDark = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
    HPEN hLight = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
    Office2007DrawSeparatorLines(pDC, *pRect, hDark, hLight, bHorz);
    if (hDark) ::DeleteObject(hDark);
    if (hLight) ::DeleteObject(hLight);
}

// Symbol: ?GetCaptionBarTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCCaptionBar@@@Z
// Retail 0x1a2550: `if (!predicate) return CMFCVisualManager::GetCaptionBarTextColor(pBar);
// else return this->[+0xc670];`. The non-themed callee is resolved: the direct
// call at 0x1a256f targets 0x186bb0 = ?GetCaptionBarTextColor@CMFCVisualManager@@UEAAKPEAVCMFCCaptionBar@@@Z.
// Only that branch is reachable here (see the batch note above).
extern "C" unsigned long MS_ABI impl__GetCaptionBarTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCCaptionBar___Z(
    CMFCVisualManagerOffice2007* pThis, CMFCCaptionBar* pBar)
{
    if (!pThis) return 0;
    return impl__GetCaptionBarTextColor_CMFCVisualManager__UEAAKPEAVCMFCCaptionBar___Z(pThis, pBar);
}

// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCToolBarMenuButton@@@Z
// Retail 0x1a59e0: non-themed branch calls 0x1ae130 =
// ?GetHighlightedMenuItemTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCToolBarMenuButton@@@Z;
// the themed branch returns this->[+0xc5d0]. OpenMFC has no OfficeXP override
// in the hierarchy, so this delegates to CMFCVisualManager's implementation.
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCToolBarMenuButton___Z(
    CMFCVisualManagerOffice2007* pThis, CMFCToolBarMenuButton* pButton)
{
    if (!pThis) return 0;
    return impl__GetHighlightedMenuItemTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarMenuButton___Z(pThis, pButton);
}

// Symbol: ?GetMenuItemTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCToolBarMenuButton@@HH@Z
// Retail 0x1a5990: non-themed branch calls 0x189dd0 =
// ?GetMenuItemTextColor@CMFCVisualManager@@UEAAKPEAVCMFCToolBarMenuButton@@HH@Z;
// the themed branch returns this->[+0xc5cc] or [+0xc5d4] depending on bDisabled.
extern "C" unsigned long MS_ABI impl__GetMenuItemTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCToolBarMenuButton__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CMFCToolBarMenuButton* pButton, int bHighlight, int bDisabled)
{
    if (!pThis) return 0;
    return impl__GetMenuItemTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarMenuButton__HH_Z(
        pThis, pButton, bHighlight, bDisabled);
}

// Symbol: ?GetNcBtnSize@CMFCVisualManagerOffice2007@@UEBA?AVCSize@@H@Z
// Retail 0x1a2320, transcribed completely:
//     neg %r8d; sbb %rax,%rax; and $8,%eax; mov 0x300(%rax,%rcx,1),%rax
//     mov %rax,(%rdx); mov %rdx,%rax; ret
// i.e. it copies the 8-byte CSize at this->[+0x300 + (bSmall ? 8 : 0)] into the
// caller's return buffer (RDX holds the hidden return pointer -- CSize is
// returned in memory, and the buffer pointer comes back in RAX).
// The constructor at 0x197c10 zero-fills exactly those two CSize slots
// (`lea 0x300(%rbx),%rdi` then a 2-iteration loop storing r15 = 0), and only
// loading the Office2007 style bitmaps replaces them. OpenMFC never loads them,
// so both entries are CSize(0, 0) here, which is what a freshly constructed
// retail manager also returns. This also repairs the ABI: the generated stub
// returned nullptr instead of the caller's buffer.
extern "C" CSize* MS_ABI impl__GetNcBtnSize_CMFCVisualManagerOffice2007__UEBA_AVCSize__H_Z(
    const void* /*pThis*/, CSize* pRet, int /*bSmall*/)
{
    if (pRet) *pRet = CSize(0, 0);
    return pRet;
}

// Symbol: ?GetRibbonBar@CMFCVisualManagerOffice2007@@QEBAPEAVCMFCRibbonBar@@PEAVCWnd@@@Z
// The mangled name lost its RVA to a COMDAT fold with
// ?GetRibbonBar@CMFCVisualManagerWindows7@@QEBAPEAVCMFCRibbonBar@@PEAVCWnd@@@Z
// (0x1a16b0). PROVED, twice: in mfc140u.dll both names resolve to the single
// RVA 0x1a30b0, and in mfc140.dll ?OnNcPaint@CMFCVisualManagerOffice2007@@
// (entry 0x1a1770) reaches this body with a direct `call 0x1801a16b0` at
// 0x1a1828, i.e. Office2007's own code runs it.
// It does: when pWnd is NULL it takes the object returned by
// ?AfxGetModuleThreadState@@YAPEAVAFX_MODULE_THREAD_STATE@@XZ (0x1346a0),
// reads its +0x8, and calls that object's vtable +0xf8 to obtain a
// window; then, if the window's m_hWnd (+0x40) is non-NULL, it returns
// pWnd->[+0x370] when ?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z (0x233310)
// matches the CRuntimeClass at 0x1802e7160, or pWnd->[+0x710] when it matches
// the one at 0x1802ec3c8, else NULL. Neither descriptor is named in the RVA
// map, and +0x370 / +0x710 are frame members OpenMFC does not model, so the
// body is resolved but NOT reproducible; left a stub returning NULL. What is
// missing here is the frame layout, not the disassembly.
extern "C" void* MS_ABI impl__GetRibbonBar_CMFCVisualManagerOffice2007__QEBAPEAVCMFCRibbonBar__PEAVCWnd___Z(
    const void* /*pThis*/, CWnd* /*pWnd*/)
{
    return nullptr;
}

// Symbol: ?GetRibbonEditBackgroundColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCRibbonRichEditCtrl@@HHH@Z
// Retail 0x1ab1b0: non-themed branch calls 0x18d0e0 =
// ?GetRibbonEditBackgroundColor@CMFCVisualManager@@UEAAKPEAVCMFCRibbonRichEditCtrl@@HHH@Z
// with the same four arguments; the themed branch returns [+0xc6dc] when
// bIsDisabled, [+0xc6e0] when bIsHighlighted, else [+0xc6d8].
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
    CMFCVisualManagerOffice2007* pThis, void* pEdit, int bIsHighlighted, int bIsPaneHighlighted, int bIsDisabled)
{
    if (!pThis) return 0;
    return impl__GetRibbonEditBackgroundColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
        pThis, pEdit, bIsHighlighted, bIsPaneHighlighted, bIsDisabled);
}

// Symbol: ?GetRibbonHyperlinkTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCRibbonLinkCtrl@@@Z
// Retail 0x1ab0d0: every path that does not produce an Office2007 colour ends
// at the call at 0x1ab19a to 0x18d030 =
// ?GetRibbonHyperlinkTextColor@CMFCVisualManager@@UEAAKPEAVCMFCRibbonLinkCtrl@@@Z,
// including the !predicate path -- which is the one taken here.
// The themed side, described only as far as the instructions show it: it needs
// the predicate TRUE and the link's vtable +0x1c8 query to return 0; it then
// loads this->[+0xc764 + (pLink vtable +0x1b0 query ? 4 : 0)], and if BOTH
// this->[+0xc770] and this->[+0xc76c] are other than -1 and the object at
// pLink->[+0xd8] passes CObject::IsKindOf (descriptor 0x18030b0c0) and
// ?IsExtendedElement@CMFCRibbonStatusBar@@QEBAH... (0x128100) returns 0, it
// reloads this->[+0xc76c + (same +0x1b0 adjustment)] instead. Whatever colour
// it ends up with, a value of -1 still falls through to the base call. What
// the two vtable queries mean is not established -- only their slots are.
extern "C" unsigned long MS_ABI impl__GetRibbonHyperlinkTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCRibbonLinkCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, void* pLink)
{
    if (!pThis) return 0;
    return impl__GetRibbonHyperlinkTextColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonLinkCtrl___Z(pThis, pLink);
}

// Symbol: ?GetRibbonPopupBorderSize@CMFCVisualManagerOffice2007@@UEBAHPEBVCMFCRibbonPanelMenu@@@Z
// Retail 0x1aad20 inverts the usual shape: when the predicate is FALSE it
// returns this->[+0xf4] directly (`mov 0xf4(%rbx),%eax` at 0x1aad39), and only
// the themed path inspects the menu (CMFCRibbonPanelMenu vtable +0x3a0, then
// ?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z (0x233310) against the
// CRuntimeClass at 0x180307f90, whose name is not in the RVA map).
// +0xf4 is NOT an Office2007 member: it lives in the CMFCVisualManager
// base block and its value IS established by the construction chain, which is
// why this returns a constant rather than staying a stub.
//   ??0CMFCVisualManager@@QEAA@H@Z          (0x182640) `movl $0x2,0xf4(%rbx)`
//   ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z  (0x1abe10) `mov  %edi,0xf4(%rbx)`
//                                                      with edi = 1 (0x1abeea)
//   ??0CMFCVisualManagerOffice2003@@QEAA@XZ (0x18ed30) does not touch +0xf4
//   ??0CMFCVisualManagerOffice2007@@IEAA@XZ (0x197c10) does not touch +0xf4
// and the Office2007 ctor chain is 0x197c10 -> 0x18ed30 -> 0x1abe10 ->
// 0x182640, so the base ctor's 2 is overwritten by OfficeXP's 1 and +0xf4 is 1
// for every constructed Office2007 manager. Scanning the whole Office2007 /
// Office2003 / OfficeXP text range 0x190000..0x1b0000 finds exactly one other
// store to +0xf4 -- the themed style-load path at 0x199d47
// (`movl $0x1,0x0(%r13)` with r13 = this+0xf4 from 0x199d40) -- and it stores
// 1 as well, so 1 is the answer on both sides of the predicate here.
// CMFCVisualManagerWindows7's copy of the same override (0x1c2550) reads the
// same member on its own !predicate edge, which corroborates the reading.
extern "C" int MS_ABI impl__GetRibbonPopupBorderSize_CMFCVisualManagerOffice2007__UEBAHPEBVCMFCRibbonPanelMenu___Z(
    const void* /*pThis*/, const void* /*pMenu*/)
{
    return 1;
}

// Symbol: ?GetRibbonQuickAccessToolBarRightMargin@CMFCVisualManagerOffice2007@@UEAAHXZ
// Retail 0x1a7ad0, complete: `if (predicate() && this->[+0x5998] != NULL)
// return this->[+0x5ac8]; return 0;` -- both guarded paths need Office2007
// members, and the only reachable edge in this build returns 0.
extern "C" int MS_ABI impl__GetRibbonQuickAccessToolBarRightMargin_CMFCVisualManagerOffice2007__UEAAHXZ(
    CMFCVisualManagerOffice2007* /*pThis*/)
{
    return 0;
}

// Symbol: ?GetRibbonStatusBarTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCRibbonStatusBar@@@Z
// Retail 0x1ab220: non-themed branch calls 0x189410 =
// ?GetRibbonStatusBarTextColor@CMFCVisualManager@@UEAAKPEAVCMFCRibbonStatusBar@@@Z;
// the themed branch returns this->[+0xc5d8].
extern "C" unsigned long MS_ABI impl__GetRibbonStatusBarTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCRibbonStatusBar___Z(
    CMFCVisualManagerOffice2007* pThis, CMFCRibbonStatusBar* pStatusBar)
{
    if (!pThis) return 0;
    return impl__GetRibbonStatusBarTextColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonStatusBar___Z(pThis, pStatusBar);
}

// Symbol: ?GetShowAllMenuItemsHeight@CMFCVisualManagerOffice2007@@UEAAHPEAVCDC@@AEBVCSize@@@Z
// Retail 0x1a51b0, complete. The themed path (predicate TRUE and
// this->[+0x3d98] != NULL) returns this->[+0x3eac] - this->[+0x3ea4] + 6.
// Every other path -- including the !predicate one, which is the reachable one
// here -- computes `CMenuImages::Size().cy + 10` (call 0x8fbb0 =
// ?Size@CMenuImages@@SA?AVCSize@@XZ, then `mov 0x4(%rax),%eax; add $0xa,%eax`).
// Note that retail reads NEITHER argument on either path: pDC and sizeDefault
// are dead.
// DEVIATION, and it is a real one: OpenMFC cannot compute the retail
// expression, because impl__Size_CMenuImages__SA_AVCSize__XZ
// (phase4/src/featurepack/controls/CMFCReBar.cpp) is still a generated stub
// that returns nullptr, and the CMenuImages declared in include/openmfc/afxmfc.h
// is an opaque `char _pad[16]`. So this delegates to
// CMFCVisualManager::GetShowAllMenuItemsHeight, which returns
// `sizeDefault.cy > 0 ? sizeDefault.cy : 16` -- a DIFFERENT quantity from
// retail's, and one that depends on the sizeDefault retail ignores. The two
// substituted values below (CSize(0,16) for a NULL sizeDefault pointer, 16 for
// a NULL this) are likewise this tree's convention, not retail's.
extern "C" int MS_ABI impl__GetShowAllMenuItemsHeight_CMFCVisualManagerOffice2007__UEAAHPEAVCDC__AEBVCSize___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, const CSize* pSizeDefault)
{
    static const CSize kFallback(0, 16);
    if (!pThis) return 16;
    return pThis->CMFCVisualManager::GetShowAllMenuItemsHeight(pDC, pSizeDefault ? *pSizeDefault : kFallback);
}

// Symbol: ?GetSmartDockingTheme@CMFCVisualManagerOffice2007@@UEAA?AW4AFX_SMARTDOCK_THEME@@XZ
// The Office2007 mangled name has no RVA in the map, but the body it runs is
// established here from the VTABLE, not inferred from a sibling class. The
// constructor at 0x197c10 installs the vtable at 0x18031ace8; its slot 120
// (byte offset 0x3c0) holds 0x1801ab6d0. Slot 120 is GetSmartDockingTheme:
// slots 118 and 119 of the same vtable are
// ?GetSmartDockingBaseGuideColors@CMFCVisualManagerOffice2003@@ (0x195760) and
// ?GetSmartDockingHighlightToneColor@CMFCVisualManagerOffice2003@@ (0x195830),
// and CMFCVisualManager (vtable 0x180319f78), CMFCVisualManagerOfficeXP
// (0x18031b3d8) and CMFCVisualManagerOffice2003 (0x18031a5c0) all carry 0x3ae0
// in that slot -- a folded `mov $1,%eax; ret`, i.e. the base answer 1. The RVA
// map labels 0x1ab6d0 with the CMFCVisualManagerVS2008 name only because the
// two classes' bodies are byte-identical and folded onto one COMDAT -- which
// the mfc140u.dll export table confirms independently: both mangled names
// resolve there to the single RVA 0x1ad0d0, and the body at mfc140u 0x1ad0d0
// is instruction-for-instruction the same function.
//
// 0x1ab6d0, transcribed completely (0x1ab6d0..0x1ab738, ret to int3):
//     afxGlobalData.Initialize()  (0x6a5c0) if its leading dword is still 0;
//     if (*(int*)0x1803ba608 > 8        // afxGlobalData + 0x288, bits/pixel
//      && *(int*)0x1803ba5e4 == 0       // afxGlobalData + 0x264
//      && *(int*)0x1803ba5e0 == 0)      // afxGlobalData + 0x260
//         return 2;
//     return 1;
// This is NOT the 0x1972e8 predicate used by the rest of this batch: the
// this->[+0x2f0] "style resources loaded" term is absent, so BOTH answers stay
// reachable in this build and there is no fallback edge to take.
//
// The three afxGlobalData fields are recomputed rather than modelled, because
// ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ (0x6afd0) sets them from plain
// Win32 calls and is short enough to transcribe:
//   +0x288 = ::GetDeviceCaps(dc, BITSPIXEL)         // 0x6b052, GDI32 import
//            slot 0x1802c4168, on a CWindowDC(NULL) built at 0x6b042
//            (0x2a1b60 = ??0CWindowDC@@QEAA@PEAVCWnd@@@Z)
//   +0x260 = (::GetSysColor(COLOR_3DLIGHT)    == 0xFFFFFF &&
//             ::GetSysColor(COLOR_BTNFACE)    == 0x000000) ? 1 : 0   // 0x6b00d
//   +0x264 = (::GetSysColor(COLOR_3DDKSHADOW) == 0x000000 &&
//             ::GetSysColor(COLOR_BTNFACE)    == 0xFFFFFF) ? 1 : 0   // 0x6b035
// (colour indices read from the ecx literals 0x16 / 0xf / 0x15 and the
// GetSysColor import slot 0x1802c4dc0. Calling the two flags black/white
// high-contrast detectors is an interpretation of what those colour tests mean;
// the tests themselves are transcribed verbatim and that is what is coded.)
// A NULL screen DC yields ::GetDeviceCaps(NULL, ...) == 0, which
// falls to the same 1 retail would return, so no value is invented for it.
// The literal results 2 and 1 are AFX_SMARTDOCK_THEME enumerators; OpenMFC
// declares no such enum, so they are returned as plain ints, exactly as the
// export's 4-byte enum return does.
// This also repairs the generated stub, whose 17-parameter signature was
// demangler noise and whose nullptr return was wrong for a 4-byte enum.
extern "C" int MS_ABI impl__GetSmartDockingTheme_CMFCVisualManagerOffice2007__UEAA_AW4AFX_SMARTDOCK_THEME__XZ(
    void* /*pThis*/)
{
    HDC hdcScreen = ::GetDC(nullptr);
    const int nBitsPerPixel = ::GetDeviceCaps(hdcScreen, BITSPIXEL);
    if (hdcScreen != nullptr) {
        ::ReleaseDC(nullptr, hdcScreen);
    }

    const COLORREF clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
    const int bBlackHighContrast =
        (::GetSysColor(COLOR_3DLIGHT) == RGB(255, 255, 255) && clrBtnFace == RGB(0, 0, 0)) ? 1 : 0;
    const int bWhiteHighContrast =
        (::GetSysColor(COLOR_3DDKSHADOW) == RGB(0, 0, 0) && clrBtnFace == RGB(255, 255, 255)) ? 1 : 0;

    if (nBitsPerPixel > 8 && bWhiteHighContrast == 0 && bBlackHighContrast == 0) {
        return 2;
    }
    return 1;
}

// Symbol: ?GetStatusBarPaneTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCStatusBar@@PEAVCMFCStatusBarPaneInfo@@@Z
// Retail 0x1a5820: non-themed branch calls 0x189e80 =
// ?GetStatusBarPaneTextColor@CMFCVisualManager@@UEAAKPEAVCMFCStatusBar@@PEAVCMFCStatusBarPaneInfo@@@Z
// with the same arguments. (The themed branch tests the pane's +0x14 flag bit
// 0x04000000 and its +0x24 colour against -1, returning [+0xc5dc]/[+0xc5d8];
// with a NULL pane it branches to ?AfxThrowInvalidArgException@@YAXXZ
// (0x225b80) -- but only on that themed path, so the delegation below is not
// missing an argument check retail performs here.)
extern "C" unsigned long MS_ABI impl__GetStatusBarPaneTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(
    CMFCVisualManagerOffice2007* pThis, CMFCStatusBar* pStatusBar, void* pPane)
{
    if (!pThis) return 0;
    return impl__GetStatusBarPaneTextColor_CMFCVisualManager__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(
        pThis, pStatusBar, pPane);
}

// Symbol: ?GetStyleResourceID@CMFCVisualManagerOffice2007@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@W4Style@1@@Z
// Transcribed from the ANSI twin at mfc140 0x198f30 and then re-read in the
// Unicode image, ?GetStyleResourceID@CMFCVisualManagerOffice2007@@KA?AV?$CStringT@_W...
// at mfc140u 0x19a930, so the wide-character literals below are the real ones
// and not a transliteration of the char ones.
// Retail builds `CString str(_T("IDX_OFFICE2007_STYLE"))`, an empty `strStyle`,
// then selects a literal by style value and returns `strStyle + str`:
//     0 -> "BLUE_"   (mfc140 0x18033f190 / mfc140u 0x1803432a0)
//     1 -> "BLACK_"  (mfc140 0x18033f198 / mfc140u 0x1803432b0)
//     2 -> "AQUA_"   (mfc140 0x18033f1a0 / mfc140u 0x1803432c0)
//     3 -> "SILVER_" (mfc140 0x18033f1a8 / mfc140u 0x1803432d0)
//     anything else -> strStyle stays empty
// The 2/3 mapping is what the jump chains at mfc140 0x198f7e..0x198f8f and
// mfc140u 0x19a97e..0x19a98f both do -- 2 selects AQUA_ and 3 selects SILVER_.
// It does NOT line up with the enumerator order in include/openmfc/afxmfc.h
// (Office2007_Silver = 2, Office2007_Aqua = 3). The export takes an integer, so
// the disassembly is followed here and the integer mapping is reproduced
// exactly; the header's two enumerators look swapped relative to retail, which
// is a header question and is not papered over in this body.
// The result is returned in the caller's buffer (RCX) and echoed in RAX, which
// also repairs the generated stub's ABI (it returned nullptr).
extern "C" CString* MS_ABI impl__GetStyleResourceID_CMFCVisualManagerOffice2007__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4Style_1__Z(
    CString* pRet, int style)
{
    if (!pRet) return pRet;
    new (pRet) CString();
    const wchar_t* pszStyle = L"";
    switch (style) {
    case 0: pszStyle = L"BLUE_";   break;
    case 1: pszStyle = L"BLACK_";  break;
    case 2: pszStyle = L"AQUA_";   break;
    case 3: pszStyle = L"SILVER_"; break;
    default: break;
    }
    *pRet = pszStyle;
    *pRet += L"IDX_OFFICE2007_STYLE";
    return pRet;
}

// Symbol: ?GetSystemBorders@CMFCVisualManagerOffice2007@@IEBA?AVCSize@@H@Z
// The Office2007 mangled name lost its RVA in mfc140_rva_symbols.json to a
// COMDAT fold with
// ?GetSystemBorders@CMFCVisualManagerWindows7@@IEBA?AVCSize@@H@Z (0x1ab260),
// which is the body transcribed here. The fold is PROVED, not inferred: in
// mfc140u.dll both mangled names resolve to the one RVA 0x1acc60.
// The body itself is self-contained and complete:
//     size.cy = ::GetSystemMetrics(SM_CXSIZEFRAME);   // 0x20
//     size.cx = ::GetSystemMetrics(SM_CYSIZEFRAME);   // 0x21
//     if (bFlag) { size.cx -= 1; size.cy -= 1; }
// The cx/cy cross-assignment is what the instructions do (`mov %esi,0x4(%rdi)`
// stores the SM_CXSIZEFRAME result into .cy), and it is reproduced verbatim.
extern "C" CSize* MS_ABI impl__GetSystemBorders_CMFCVisualManagerOffice2007__IEBA_AVCSize__H_Z(
    const void* /*pThis*/, CSize* pRet, int bAdjust)
{
    if (!pRet) return pRet;
    const int cxFrame = ::GetSystemMetrics(SM_CXSIZEFRAME);
    const int cyFrame = ::GetSystemMetrics(SM_CYSIZEFRAME);
    pRet->cy = cxFrame;
    pRet->cx = cyFrame;
    if (bAdjust) {
        pRet->cx -= 1;
        pRet->cy -= 1;
    }
    return pRet;
}

// Symbol: ?GetTabFrameColors@CMFCVisualManagerOffice2007@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
// Retail 0x1a5a20 calls 0x193a90 =
// ?GetTabFrameColors@CMFCVisualManagerOffice2003@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
// with the same nine arguments on BOTH branches; only when the predicate holds
// does it afterwards overwrite clrBlack with [+0xc694] and clrHighlight with
// [+0xc698], and then only if the tab control's vtable +0x510 query returns
// non-zero and its +0x1fc is zero, and only for values other than 0xff000000.
// OpenMFC has no Office2003 override, so the shared call is routed to
// CMFCVisualManager's implementation.
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerOffice2007__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    CMFCVisualManagerOffice2007* pThis, const CMFCBaseTabCtrl* pTabWnd, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack)
{
    if (!pThis) return;
    impl__GetTabFrameColors_CMFCVisualManager__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
        pThis, pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight,
        pbrFace, pbrBlack);
}

// Symbol: ?GetTabHorzMargin@CMFCVisualManagerOffice2007@@UEAAHPEBVCMFCBaseTabCtrl@@@Z
// Retail 0x1a62f0, complete. Every guard failure -- predicate FALSE, or any of
// the tab control's four vtable queries at +0x520/+0x560/+0x528/+0x530
// returning non-zero, or the manager's pointer at [+0x4d98 (+0x400 when the
// tab control's vtable +0x510 query returned non-zero)] being NULL -- lands on
// `xor %eax,%eax`. Only the fully themed path returns this->[+0x4ec8, with the
// same +0x400 adjustment] / 2. In this build the predicate is FALSE, so the
// reachable result is 0.
extern "C" int MS_ABI impl__GetTabHorzMargin_CMFCVisualManagerOffice2007__UEAAHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* /*pThis*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    return 0;
}

// Symbol: ?GetTabTextColor@CMFCVisualManagerOffice2007@@UEAAKPEBVCMFCBaseTabCtrl@@HH@Z
// Retail 0x1a6270, complete. The default edge is `or $0xffffffff,%eax`, i.e.
// (COLORREF)-1 meaning "no override, use the caller's default"; it is taken
// when the predicate is FALSE, when the tab's +0x1fc is non-zero, when the
// vtable +0x520 query is TRUE, or when the vtable +0x3c8 query already returned
// a colour other than -1. Only the fully themed path returns
// this->[+0xc6a0 (-4 when bIsActive)]. The predicate is FALSE here, so -1.
extern "C" unsigned long MS_ABI impl__GetTabTextColor_CMFCVisualManagerOffice2007__UEAAKPEBVCMFCBaseTabCtrl__HH_Z(
    CMFCVisualManagerOffice2007* /*pThis*/, const CMFCBaseTabCtrl* /*pTabWnd*/, int /*iTab*/, int /*bIsActive*/)
{
    return (unsigned long)-1;
}

// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManagerOffice2007@@UEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
// Retail 0x1a5890: non-themed branch calls 0x1b0550 =
// ?GetToolbarButtonTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
// with the same arguments; the themed branch picks among [+0xc5b4..+0xc5c8]
// from the button's style bit 0x40000 and runtime class. Routed to
// CMFCVisualManager's implementation (no OfficeXP class in this hierarchy).
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerOffice2007__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2007* pThis, CMFCToolBarButton* pButton, int nState)
{
    if (!pThis) return 0;
    return impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
        pThis, pButton, nState);
}

// Symbol: ?GetToolTipInfo@CMFCVisualManagerOffice2007@@UEAAHAEAVCMFCToolTipInfo@@I@Z
// Retail 0x1ab310. The themed branch (predicate TRUE and this->[+0x2fc] != 0)
// copies the 13 dwords at this->[+0x10a3c..+0x10a6c] into the caller's
// CMFCToolTipInfo and returns 1. The non-themed branch tail-calls 0x197090 =
// ?GetToolTipInfo@CMFCVisualManagerOffice2003@@UEAAHAEAVCMFCToolTipInfo@@I@Z
// and -- worth noting -- it does NOT forward the caller's nType: it loads
// `or $0xffffffff,%r8d`, i.e. it always passes (UINT)-1. That substitution is
// reproduced below. OpenMFC has no Office2003 override, so the call goes to
// CMFCVisualManager's implementation.
extern "C" int MS_ABI impl__GetToolTipInfo_CMFCVisualManagerOffice2007__UEAAHAEAVCMFCToolTipInfo__I_Z(
    CMFCVisualManagerOffice2007* pThis, CMFCToolTipInfo& params, unsigned int /*nType*/)
{
    if (!pThis) return 0;
    return impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
        pThis, params, (unsigned int)-1);
}

// Symbol: ?IsHighlightWholeMenuItem@CMFCVisualManagerOffice2007@@UEAAHXZ
// Retail 0x1a5800 in full: `sub $0x28,%rsp; call <predicate>; mov $1,%eax;
// add $0x28,%rsp; ret`. That is the entire function -- five instructions, no
// branch. The predicate is still called (it is not inlinable and has the
// AFX_GLOBAL_DATA::Initialize side effect) but its result is never tested, so
// TRUE is the complete, unconditional retail answer for every input and every
// object state. Why the source looks that way is not visible in the object
// code and is not asserted here.
extern "C" int MS_ABI impl__IsHighlightWholeMenuItem_CMFCVisualManagerOffice2007__UEAAHXZ(
    CMFCVisualManagerOffice2007* /*pThis*/)
{
    return TRUE;
}

// Symbol: ?IsOwnerDrawMenuCheck@CMFCVisualManagerOffice2007@@UEAAHXZ
// Retail 0x1a57f0, the same five-instruction shape as IsHighlightWholeMenuItem
// but with `xor %eax,%eax`: the predicate's result is never tested, so FALSE
// is unconditional.
extern "C" int MS_ABI impl__IsOwnerDrawMenuCheck_CMFCVisualManagerOffice2007__UEAAHXZ(
    CMFCVisualManagerOffice2007* /*pThis*/)
{
    return FALSE;
}

// Symbol: ?IsRibbonPresent@CMFCVisualManagerOffice2007@@QEBAHPEAVCWnd@@@Z
// The mangled name lost its RVA to a COMDAT fold with
// ?IsRibbonPresent@CMFCVisualManagerWindows7@@QEBAHPEAVCWnd@@@Z (0x1a1740) --
// PROVED: in mfc140u.dll both names resolve to the single RVA 0x1a3140.
// That body is
// `pBar = GetRibbonBar(pWnd); return pBar != NULL && ::IsWindowVisible(pBar->m_hWnd);`
// (the call at 0x1a1744 goes to GetRibbonBar at 0x1a16b0 and the import slot at
// 0x1802c5350 resolves to USER32!IsWindowVisible). Since GetRibbonBar cannot be
// implemented here (see above), the result would be FALSE unconditionally,
// which is indistinguishable from a stub -- so this is left a stub rather than
// dressed up as an implementation.
extern "C" int MS_ABI impl__IsRibbonPresent_CMFCVisualManagerOffice2007__QEBAHPEAVCWnd___Z(
    const void* /*pThis*/, CWnd* /*pWnd*/)
{
    return 0;
}

// Symbol: ?IsWindowActive@CMFCVisualManagerOffice2007@@IEBAHPEAVCWnd@@@Z
// Retail 0x1a08d0, transcribed completely:
//     if (pWnd == NULL) return FALSE;
//     if (pWnd->m_hWnd (+0x40) == NULL) return FALSE;
//     entry = <CMap assoc lookup, 0x1c07c>(this + 0x10a70, hWnd, &tmp, &tmp);
//     return entry ? entry->[+8] : TRUE;
// +0x10a70 is a CMapWordToPtr (the destructor path calls
// ?RemoveAll@CMapWordToPtr@@QEAAXXZ (0x1bad0) on it at 0x199c52), keyed by
// HWND. Retail populates it from ?OnNcActivate@CMFCVisualManagerOffice2007@@
// (0x1a0910), which calls this function and then stores the result through the
// map's operator[] at 0x1a09ce. OpenMFC models neither the map nor a working
// OnNcActivate override (this file's OnNcActivate is still a generated stub),
// so nothing can ever populate it and the lookup always misses. The retail
// answer therefore reduces here to the two NULL guards followed by TRUE, which
// is what is implemented -- the lookup branch is a deliberate omission, not an
// oversight.
extern "C" int MS_ABI impl__IsWindowActive_CMFCVisualManagerOffice2007__IEBAHPEAVCWnd___Z(
    const void* /*pThis*/, CWnd* pWnd)
{
    if (pWnd == nullptr) return FALSE;
    if (pWnd->m_hWnd == nullptr) return FALSE;
    return TRUE;
}

// Symbol: ?MakeResourceID@CMFCVisualManagerOffice2007@@KA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
// Transcribed from the ANSI twin at mfc140 0x198eb0; the Unicode export is at
// mfc140u 0x19a8b0 and matches:
//     CString str(lpszBase);                       // ctor at 0x18000dd70
//     if (m_strStylePrefix.GetLength() != 0)       // nDataLength at prefix-0x10
//         str = m_strStylePrefix + str;            // operator+ at 0x1800793e0
//     return str;                                  // buffer echoed in RAX
// OpenMFC never populates the static m_strStylePrefix (see its comment above),
// so only the empty-prefix path is reachable and the result is lpszBase itself.
// This also repairs the ABI: the generated stub returned nullptr rather than
// the caller's return buffer.
extern "C" CString* MS_ABI impl__MakeResourceID_CMFCVisualManagerOffice2007__KA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* pRet, const wchar_t* lpszBase)
{
    if (!pRet) return pRet;
    new (pRet) CString();
    if (lpszBase != nullptr) *pRet = lpszBase;
    return pRet;
}

// Symbol: ?OnDrawBarGripper@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
// Retail 0x1a3310. Three guards fall through to the call at 0x1a366a =
// ?OnDrawBarGripper@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z:
// predicate FALSE, pBar->[+0xf4] non-zero, or this->[+0x318] zero. Only past
// those does retail tile the gripper image (this->[+0x310], drawn with
// CMFCToolBarImages::DrawEx at 0x16b6d0) using the CSize at this->[+0x378].
// The predicate is FALSE here, so the base call is the reachable path; OpenMFC
// has no Office2003 override, so it goes to CMFCVisualManager's.
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawBarGripper(pDC, rectGripper, bHorz, pBar);
}

// Symbol: ?OnDrawButtonSeparator@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
// Retail 0x1a4c50: the !predicate branch calls 0x1b1820 =
// ?OnDrawButtonSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
// with the same arguments; the themed branch draws with the manager's pens at
// this->[+0x1f0]. Routed to CMFCVisualManager's implementation.
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect,
    CMFCVisualManager::AFX_BUTTON_STATE /*state*/, int bHorz)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawButtonSeparator(pDC, pButton, rect, bHorz);
}

// Symbol: ?OnDrawCaptionBarInfoArea@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@@Z
// Retail 0x1a2590: the !predicate branch calls 0x1b3160 =
// ?OnDrawCaptionBarInfoArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@@Z;
// the themed branch fills the area through CDrawingManager::FillGradient
// (0x57960) with colours read out of afxGlobalData and the Office2007 block.
// Routed to CMFCVisualManager's thunk (which is itself still a no-op in this
// tree, so nothing is painted today -- the control flow, not the pixels, is
// what is transcribed here).
extern "C" void MS_ABI impl__OnDrawCaptionBarInfoArea_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect)
{
    if (!pThis) return;
    impl__OnDrawCaptionBarInfoArea_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(
        pThis, pDC, pBar, rect);
}

// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
// Retail 0x1a70b0: the !predicate branch calls 0x195f50 =
// ?OnDrawCheckBoxEx@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHH@Z with
// the same six arguments; the themed branch maps (nState, bHighlighted,
// bPressed, bEnabled) onto an index 4*nState + {0..3} into the Office2007
// check-box image strip. Routed to CMFCVisualManager's implementation.
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, int nState,
    int bHighlighted, int bPressed, int bEnabled)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawCheckBoxEx(pDC, rect, nState, bHighlighted, bPressed, bEnabled);
}

// Symbol: ?OnDrawComboBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
// Retail 0x1a40e0: the !predicate branch calls 0x190120 =
// ?OnDrawComboBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
// with the same six arguments; the themed branch frames the combo through
// CDrawingManager::DrawRect (0x5abf0) / CDC::Draw3dRect (0x2a3b00) with
// Office2007 colours. Routed to CMFCVisualManager's implementation.
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, int bDisabled,
    int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawComboBorder(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}

// Symbol: ?OnDrawDefaultRibbonImage@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHH@Z
// Retail 0x1a7cd0: the themed path needs both the predicate AND a non-NULL
// pointer at this->[+0x2158 + 0xa0]; it then draws that image strip with
// CMFCToolBarImages::DrawEx (0x16b6d0). Everything else falls to 0x18a340 =
// ?OnDrawDefaultRibbonImage@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@HHH@Z,
// which is the reachable path here (its OpenMFC thunk is still a no-op, so
// nothing is painted yet).
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectImage, int bIsDisabled,
    int bIsPressed, int bIsHighlighted)
{
    if (!pThis) return;
    impl__OnDrawDefaultRibbonImage_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHH_Z(
        pThis, pDC, rectImage, bIsDisabled, bIsPressed, bIsHighlighted);
}

// Symbol: ?OnDrawEditBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
// Retail 0x1a41e0: the !predicate branch calls 0x1b06d0 =
// ?OnDrawEditBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
// with the same five arguments; the themed branch frames the edit box with
// CDrawingManager::DrawRect (0x5abf0) / CDC::Draw3dRect (0x2a3b00).
// Routed to CMFCVisualManager's implementation.
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, int bDisabled,
    int bIsHighlighted, CMFCToolBarEditBoxButton* pButton)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawEditBorder(pDC, rect, bDisabled, bIsHighlighted, pButton);
}

// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
// Retail 0x1a55b0: the !predicate branch calls 0x1b0360 =
// ?OnDrawFloatingToolbarBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
// with the same five arguments; the themed branch selects the brush at
// this->[+0x1d0] into the DC (CDC::SelectObject 0x2a0670) and blits the border
// strips with ROP 0x00f00021 (PATCOPY). Routed through CMFCVisualManager's export thunk, which takes
// the CMFCBaseToolBar* form of the parameter.
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCBaseToolBar* pToolBar,
    CRect rectBorder, CRect rectBorderSize)
{
    if (!pThis) return;
    impl__OnDrawFloatingToolbarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
        pThis, pDC, pToolBar, rectBorder, rectBorderSize);
}

// Symbol: ?OnDrawHeaderCtrlBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@HH@Z
// Retail 0x1a6ee0: the !predicate branch calls 0x195a50 =
// ?OnDrawHeaderCtrlBorder@CMFCVisualManagerWindows@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@HH@Z
// with the same five arguments; the themed branch gradient-fills the header
// (CDrawingManager::FillGradient 0x57960) with [+0xc650]/[+0xc654] and rules a
// line with a locally constructed CPen (0x2a1ed0).
// Routed to CMFCVisualManager's implementation.
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect* pRect,
    int bIsPressed, int bIsHighlighted)
{
    if (!pThis || !pRect) return;
    pThis->CMFCVisualManager::OnDrawHeaderCtrlBorder(pCtrl, pDC, *pRect, bIsPressed, bIsHighlighted);
}

// Symbol: ?OnDrawMenuCheck@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z
// Retail 0x1a4e50: the themed path needs the predicate AND a non-NULL image at
// a non-zero dword at this->[+0x1af8 (+0x198 when bIsRadio)] + 8, and then
// draws the corresponding image with
// CMFCToolBarImages (Mirror 0x16ee30 / DrawEx 0x16b6d0). Both guard failures
// jump to the block at 0x1a4f78, whose call at 0x1a4f9a targets 0x1834e0 =
// ?OnDrawMenuCheck@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z,
// which is the reachable path here.
extern "C" void MS_ABI impl__OnDrawMenuCheck_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarMenuButton* pButton,
    CRect rect, int bHighlight, int bIsRadio)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawMenuCheck(pDC, pButton, rect, bHighlight, bIsRadio);
}

// Symbol: ?OnDrawMenuItemButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z
// Retail 0x1a4fc0: the themed path needs the predicate AND a non-NULL pointer
// at this->[+0x42f0 + 0xa8]; it adjusts the rect and calls ::InflateRect on it
// (import slot 0x1802c5310), then paints the highlight with a locally
// constructed CBrush (0x2a1fa0). Both guard failures jump to the block at
// 0x1a50d2, whose call at 0x1a50f6 targets 0x183530 =
// ?OnDrawMenuItemButton@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@HH@Z,
// which is the reachable path here.
extern "C" void MS_ABI impl__OnDrawMenuItemButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarMenuButton* pButton,
    CRect rectButton, int bHighlight, int bDisabled)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawMenuItemButton(pDC, pButton, rectButton, bHighlight, bDisabled);
}

// Symbol: ?OnDrawMenuLabel@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@VCRect@@@Z
// Retail 0x1ab590 is the one override in this batch with NO predicate call and
// NO non-themed branch: it unconditionally ::FillRect's the label with the
// brush at this->[+0x160] or [+0xc570] (whichever the pointer test selects),
// then calls DrawSeparator(pDC, {rect.left, rect.top, rect.right,
// rect.bottom - 2}, this->[+0x1e0], this->[+0xc530], TRUE), and returns
// this->[+0xc66c], or this->[+0xc5cc] when that is -1.
// DEVIATION: every one of those members is unmodelled Office2007 state, so
// there is no retail path to take. This delegates to CMFCVisualManager's
// OnDrawMenuLabel thunk, which paints nothing and returns COLOR_MENUTEXT --
// an approximation, not a transcription.
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect)
{
    if (!pThis) return 0;
    return impl__OnDrawMenuLabel_CMFCVisualManager__UEAAKPEAVCDC__VCRect___Z(pThis, pDC, rect);
}

//=============================================================================
// CMFCVisualManagerOffice2007 - third batch: menu/tab/pane/outlook/ribbon
// fills, NC handling, OnUpdateSystemColors, SetResourceHandle
//=============================================================================
//
// Decoded from the retail disassembly. RVAs in this batch are mfc140u.dll
// entry points (resolved through the mfc140u export table). Review re-diffed
// all 44 bodies against the mfc140.dll twin: 42 are identical apart from
// RIP-relative displacements; OnNcPaint differs by one wchar_t pointer-
// difference shift and OnUpdateSystemColors by its (wider) frame size --
// both were read in mfc140u anyway. Every address quoted is the ENTRY of
// the named function unless the text says "the call at".
//
// The predicate is the same non-exported helper the second batch describes
// (mfc140 0x1972e8; its body is `afxGlobalData.Initialize() if needed;
// return bpp > 8 && !whiteHC && !blackHC && this->[+0x2f0] != 0`). In this
// build this->[+0x2f0] -- the "style resources loaded" flag -- has no
// representation and nothing ever sets it, so the predicate is FALSE and
// only its FALSE edge is reachable. Exactly as CMFCVisualManagerWindows7.cpp
// does, each body below implements that reachable edge and documents the
// themed edge (offsets, callees, colours) without inventing the members it
// needs. Where the FALSE edge is itself a bare `return 0`, the symbol is
// left an honest stub and says so.
//
// Retail's FALSE edge calls the base-class method: CMFCVisualManagerOffice2007
// derives from CMFCVisualManagerOffice2003 (which derives from OfficeXP, then
// Windows), and the callee named by each `call` below is the one the object
// code resolves to. OpenMFC declares the three classes as siblings under
// CMFCVisualManager with the same layout (CMFCVisualManager + char _pad[64];
// the static_asserts in the helper block below pin that), and the Office2003 /
// OfficeXP thunks reach `this` only through CMFCVisualManager-level thunks
// and their own on-demand palette helpers (O2003_Theme / XP_Colors), never
// through the derived block, so those thunks are called directly here with
// the Office2007 object as `this`. That is the retail control flow; how
// faithful the pixels are is a property of the callee's own file. Several of
// those callees are still delegating stubs (noted per function).

// Sibling-class entry points this batch calls, declared with the parameter
// lists of their definitions (CMFCVisualManagerOffice2003.cpp,
// CMFCVisualManagerOfficeXP.cpp, CMFCVisualManager.cpp, core/runtime/CObject.cpp).
extern "C" void MS_ABI impl__OnDrawMenuResizeBar_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, int nResizeBarHeight);
extern "C" void MS_ABI impl__OnDrawMenuScrollButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect,
    int bIsScrollDown, int bIsHighlited, int bIsPressed, int bIsDisabled);
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, unsigned int uiSystemCommand, unsigned int nStyle, int bHighlight);
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize);
extern "C" void MS_ABI impl__OnDrawOutlookBarSplitter_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectSplitter);
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect& rect, int bIsHighlighted, int bIsPressed);
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CDockablePane* pBar, int bActive, CRect rectCaption, CRect rectButtons);
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd* pPopupWnd);
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCRibbonButton* pButton);
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectTab, int iTab, int bIsActive, const CMFCBaseTabCtrl* pTabWnd);
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect& rect, CMFCButton* pButton, unsigned int uiState, CMFCBaseTabCtrl* pWndTab);
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons, int bIsHighlighted, int bIsSelected);
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* pGroup, int bIsHighlighted, int bIsSelected, int bCanCollapse);
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectCaption, int bIsActive);
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectArea, const CMFCBaseTabCtrl* pTabWnd);
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, CMFCButton* pButton, CMFCBaseTabCtrl* pWndTab);
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd);
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea);
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int bIsPressed, int bIsHighlighted, int bIsDisabled, int bHasDropDownArrow, int bIsSysButton);
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton);
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectCaption, CPaneFrameWnd* pFrameWnd, int bActive);
extern "C" void MS_ABI impl__OnFillOutlookBarCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__AEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectCaption, unsigned long& clrText);
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, const CRect& rect, int bIsHighlighted, int bIsPressed, unsigned long& clrText);
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect);
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCRibbonButton* pButton);
extern "C" void MS_ABI impl__OnFillRibbonEdit_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonRichEditCtrl__VCRect__HHHAEAK33_Z(
    CMFCVisualManager* pThis, CDC* pDC, void* pEdit, CRect rect,
    int bIsHighlighted, int bIsPaneHighlighted, int bIsDisabled,
    unsigned long& clrText, unsigned long& clrSelBackground, unsigned long& clrSelText);
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, void* pPanel, CRect rect);
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* pMenuBar, CRect rect);
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectFill, CBrush* pBrush, int iTab, int bIsActive, const CMFCBaseTabCtrl* pTabWnd);
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, unsigned long* pclrText);
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect);
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectRarelyUsed);
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOffice2003__UEAAXXZ(
    CMFCVisualManagerOffice2003* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
// This file's own entry points that retail reaches through the vftable.
extern "C" void MS_ABI impl__CleanUp_CMFCVisualManagerOffice2007__MEAAXXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton);
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOffice2007__UEAAXXZ(CMFCVisualManagerOffice2007* pThis);

namespace {
// See the batch banner: the retail base classes are OpenMFC siblings with an
// identical layout, and the callees never touch the derived block.
static_assert(sizeof(CMFCVisualManagerOffice2003) == sizeof(CMFCVisualManagerOffice2007),
              "Office2003/Office2007 layouts must match for the sibling-thunk calls below");
static_assert(sizeof(CMFCVisualManagerOfficeXP) == sizeof(CMFCVisualManagerOffice2007),
              "OfficeXP/Office2007 layouts must match for the sibling-thunk calls below");
inline CMFCVisualManagerOffice2003* AsOffice2003(CMFCVisualManagerOffice2007* p)
{
    return reinterpret_cast<CMFCVisualManagerOffice2003*>(p);
}
inline CMFCVisualManagerOfficeXP* AsOfficeXP(CMFCVisualManagerOffice2007* p)
{
    return reinterpret_cast<CMFCVisualManagerOfficeXP*>(p);
}
} // namespace

// Symbol: ?OnDrawMenuResizeBar@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@H@Z
// Retail 0x1a5dc0 (mfc140u). The !predicate edge calls
// ?OnDrawMenuResizeBar@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
// with the same three arguments. The themed edge picks an object by
// nResizeFlags (1 -> this+0x1630, 3 -> this+0x17c8, else this+0x1960; it is
// drawn with CMFCToolBarImages::DrawEx, so a CMFCToolBarImages), requires
// its +0xa0 and the +0xa8 of the object at this+0x36f0 to be non-NULL,
// fills through the this+0x36f0 object's vftable +0x30 and then draws the
// grip image -- all unmodelled Office2007 state.
extern "C" void MS_ABI impl__OnDrawMenuResizeBar_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, int nResizeFlags)
{
    if (!pThis) return;
    impl__OnDrawMenuResizeBar_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
        AsOfficeXP(pThis), pDC, rect, nResizeFlags);
}

// Symbol: ?OnDrawMenuScrollButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
// Retail 0x1a5f00 (mfc140u). The !predicate edge calls
// ?OnDrawMenuScrollButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHH@Z
// with the same six arguments. The themed edge does `rect.top--`, fills via
// the renderer at this+0x44f0 (this+0x46f0 when bIsScrollDown and the
// pointer at this+0x4798 is non-NULL) with bIsHighlited selecting the image
// index, then draws the arrow with
// ?Draw@CMenuImages@@SAXPEAVCDC@@W4IMAGES_IDS@1@AEBVCRect@@W4IMAGE_STATE@1@AEBVCSize@@@Z
// (image id 0 when bIsScrollDown, else 7). Unmodelled.
extern "C" void MS_ABI impl__OnDrawMenuScrollButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect,
    int bIsScrollDown, int bIsHighlited, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    impl__OnDrawMenuScrollButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHH_Z(
        AsOfficeXP(pThis), pDC, rect, bIsScrollDown, bIsHighlited, bIsPressed, bIsDisabled);
}

// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@IIH@Z
// Retail 0x1a5ff0 (mfc140u). The !predicate edge calls
// ?OnDrawMenuSystemButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@IIH@Z
// with the same five arguments. The themed edge selects one of the
// CMFCToolBarImages arrays by uiSystemCommand (SC_MINIMIZE 0xf020 -> +0x1300,
// SC_CLOSE 0xf060 -> +0x970, SC_RESTORE 0xf120 -> +0xca0; anything else
// draws nothing), picks image 3 when nStyle & 0x40000 else 0, fills the
// highlight through the renderer at this+0x86f0 depending on bHighlight and
// nStyle bit 0x20000, and draws with CMFCToolBarImages::DrawEx. Unmodelled.
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__IIH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect,
    unsigned int uiSystemCommand, unsigned int nStyle, int bHighlight)
{
    if (!pThis) return;
    impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(
        AsOfficeXP(pThis), pDC, rect, uiSystemCommand, nStyle, bHighlight);
}

// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
// Retail 0x1a6d40 (mfc140u). The !predicate edge calls
// ?OnDrawMiniFrameBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
// with the same five arguments (rectBorder and rectBorderSize in the same
// order). The themed edge: when pFrameWnd->IsKindOf(CMFCTasksPaneFrameWnd)
// it selects the brush at this+0x1d0 (NULL result -> AfxThrowInvalidArgException),
// PatBlt's (ROP 0x00f00021, PATCOPY) the four outer border strips, shrinks
// both rects with ::InflateRect(-2, -2), selects the afxGlobalData brush at
// +0x118 and PatBlt's four more strips inset by one pixel, then restores the
// old brush; otherwise it calls the same OfficeXP method. Only the
// tasks-pane strip painter needs the unmodelled +0x1d0 brush; the reachable
// edge is the OfficeXP call either way.
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CPaneFrameWnd* pFrameWnd,
    CRect rectBorder, CRect rectBorderSize)
{
    if (!pThis) return;
    impl__OnDrawMiniFrameBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
        AsOfficeXP(pThis), pDC, pFrameWnd, rectBorder, rectBorderSize);
}

// Symbol: ?OnDrawOutlookBarSplitter@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@@Z
// Retail 0x1a42b0 (mfc140u). The !predicate edge calls
// ?OnDrawOutlookBarSplitter@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
// with the same arguments. The themed edge gradient-fills the splitter
// (CDrawingManager::FillGradient(rect, this->[+0x268], this->[+0x264], TRUE, 0, 0)),
// draws the image at this+0x4a8 (CMFCToolBarImages::DrawEx, alignment
// arguments 1, 1) and rules the top and bottom-1 lines with the pen at
// this+0x298. Those
// three members are unmodelled.
extern "C" void MS_ABI impl__OnDrawOutlookBarSplitter_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectSplitter)
{
    if (!pThis) return;
    impl__OnDrawOutlookBarSplitter_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
        AsOffice2003(pThis), pDC, rectSplitter);
}

// Symbol: ?OnDrawOutlookPageButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEAVCRect@@HH@Z
// Retail 0x1a4230 (mfc140u). The !predicate edge calls
// ?OnDrawOutlookPageButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEAVCRect@@HH@Z
// with the same four arguments. The themed edge is
// `pDC->Draw3dRect(rect, afxGlobalData[+0x30], this->[+0x238])` (+0x30 is
// the COLOR_BTNHIGHLIGHT slot AFX_GLOBAL_DATA::UpdateSysColors fills;
// +0x238 is an unmodelled Office2007 colour).
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect* pRect, int bIsHighlighted, int bIsPressed)
{
    if (!pThis || !pRect) return;
    impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__HH_Z(
        AsOffice2003(pThis), pDC, *pRect, bIsHighlighted, bIsPressed);
}

// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
// Retail 0x1a53e0 (mfc140u). Three guards route to
// ?OnDrawPaneCaption@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
// with the same five arguments: predicate FALSE, pBar == NULL, or
// pBar->[+0xf4] != 0. (Note the NULL pBar goes to the base call, it is not
// an error.) The themed edge needs no Office2007 member -- it is, in full:
//     CPen pen(PS_SOLID, 1, afxGlobalData[+0x60]);      // COLOR_BTNFACE slot
//     old = pDC->SelectObject(&pen);
//     rectCaption.bottom += 2;
//     MoveTo(left, bottom); LineTo(left, top);
//     MoveTo(left+1, top);  LineTo(right-1, top);
//     MoveTo(right-1, top+1); LineTo(right-1, bottom);
//     pDC->SelectObject(old);
//     rectCaption.left++; rectCaption.top++; rectCaption.right--;
//     ::FillRect(pDC->m_hDC, &rectCaption,
//                bActive ? afxGlobalData.brActiveCaption   /* CBrush at +0xe8, HBRUSH at +0xf0 */
//                        : afxGlobalData.brInactiveCaption /* CBrush at +0xf8, HBRUSH at +0x100 */);
//     return bActive ? afxGlobalData[+0x80] : afxGlobalData[+0x90];
//                      // COLOR_CAPTIONTEXT / COLOR_INACTIVECAPTIONTEXT slots
// (afxGlobalData field roles read from ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ,
// mfc140u 0x6b1c0: the two CBrushes are CreateSolidBrush of +0x88 =
// COLOR_ACTIVECAPTION and +0x8c = COLOR_INACTIVECAPTION.) It is still gated
// on the predicate, so only the base call is reachable here.
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CDockablePane* pBar, int bActive,
    CRect rectCaption, CRect rectButtons)
{
    if (!pThis) return 0;
    return impl__OnDrawPaneCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
        AsOffice2003(pThis), pDC, pBar, bActive, rectCaption, rectButtons);
}

// Symbol: ?OnDrawPopupWindowCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWnd@@@Z
// Retail 0x1ace70 (mfc140u), complete:
//     clr = CMFCVisualManagerOffice2003::OnDrawPopupWindowCaption(pDC, rectCaption, pPopupWnd);
//     if (predicate()) clr = this->[+0xc6b0];
//     return clr;
// The base call is unconditional; only the colour override is themed.
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd* pPopupWnd)
{
    if (!pThis) return 0;
    return impl__OnDrawPopupWindowCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
        AsOffice2003(pThis), pDC, rectCaption, pPopupWnd);
}

// Symbol: ?OnDrawRibbonApplicationButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1a9780 (mfc140u). The !predicate edge calls
// ?OnDrawRibbonApplicationButton@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// with the same arguments (the base is two levels up; OfficeXP/Office2003 do
// not override it). The themed edge queries the button's vftable +0x1b0,
// +0x1b8, +0x1c0, +0x1d8 (pressed/highlighted-style states), offsets the
// button rect by (1, -1), and draws through the renderer at this+0x94f0
// (vftable +0x40) with an image index derived from those states and the
// renderer's size at this+0x96a0. Unmodelled.
extern "C" void MS_ABI impl__OnDrawRibbonApplicationButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    pThis->CMFCVisualManager::OnDrawRibbonApplicationButton(pDC, pButton);
}

// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1ab3f0 (mfc140u). This one does NOT return after the base call:
//     if (!predicate())
//         CMFCVisualManagerOfficeXP::OnDrawRibbonButtonBorder(pDC, pButton);   // falls through
//     if (pButton->IsKindOf(RUNTIME_CLASS(CMFCRibbonEdit))) {                  // descriptor at mfc140 0x180302a10
//         rect = pButton->[+0xc8]; rect.left = pButton->[+0x1a8];
//         clr  = this->[+0xc6e8], replaced by [+0xc6ec] when the button's
//                vftable +0x1c8 query is TRUE, else by [+0xc6f0 / +0xc6f4]
//                (chosen by +0x1d8) when any of +0x1b0 / +0x1d8 / +0x1b8 is TRUE;
//         if (CMFCToolBarImages::m_bIsDrawOnGlass) CDrawingManager(*pDC).DrawRect(rect, -1, clr);
//         else pDC->Draw3dRect(rect, clr, clr);
//     }
// The second half runs on both edges. DEVIATION: it is omitted here -- it
// needs the CMFCRibbonEdit descriptor, four CMFCRibbonButton virtuals and
// the +0xc6e8.. colour block, none of which OpenMFC models -- so ribbon edit
// borders are not framed. The OfficeXP thunk this reaches is itself still a
// no-op stub in CMFCVisualManagerOfficeXP.cpp.
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return;
    impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
        AsOfficeXP(pThis), pDC, pButton);
}

// Symbol: ?OnDrawTab@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
// Retail 0x1a7680 (mfc140u). Predicate FALSE, or any of the tab control's
// vftable +0x520 / +0x560 / +0x528 / +0x530 queries returning non-zero,
// routes to
// ?OnDrawTab@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
// with the same five arguments. The themed edge builds a polygon clip
// region from the tab rect (reading pTabWnd->[+0xf8], [+0x1dc], the
// +0x3d8(iTab) and +0x510 queries), draws the tab body through the renderer
// at this+0x52f0 (0x200 lower when [+0xf8] is set) and the text with the
// font at this+0xc510 (+0xc520 when bIsActive). Unmodelled.
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectTab, int iTab, int bIsActive,
    const CMFCBaseTabCtrl* pTabWnd)
{
    if (!pThis) return;
    impl__OnDrawTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
        AsOffice2003(pThis), pDC, rectTab, iTab, bIsActive, pTabWnd);
}

// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
// Retail 0x1a8220 (mfc140u), complete:
//     if (predicate()) return;                       // themed: draws nothing
//     if (pButton->[+0x114] || pButton->[+0x11c])    // CMFCButton m_bHover / m_bClickStarted
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);
// The !predicate half is instruction-for-instruction the body of
// ?OnDrawTabsButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
// (mfc140u 0x194f10) inlined (only the register allocation differs), so the
// reachable edge is that base call. (The
// Office2003 thunk in this tree currently delegates to CMFCVisualManager's
// implementation rather than transcribing the +0x138 3D rect.)
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect* pRect, CMFCButton* pButton,
    unsigned int uiState, CMFCBaseTabCtrl* pWndTab)
{
    if (!pThis || !pRect) return;
    impl__OnDrawTabsButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
        AsOffice2003(pThis), pDC, *pRect, pButton, uiState, pWndTab);
}

// Symbol: ?OnDrawTask@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
// Retail 0x1a87a0 (mfc140u). The themed edge needs the predicate AND
// pTask->[+0x54] != 0, and then draws only
// DrawSeparator(pDC, *(CRect*)(pTask + 0x18), this->[+0x1e0], this->[+0xc530], TRUE)
// with the two pens the second batch's DrawSeparator note describes.
// Everything else -- including the reachable !predicate edge -- calls
// ?OnDrawTask@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
// with the same five arguments.
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons,
    int bIsHighlighted, int bIsSelected)
{
    if (!pThis) return;
    impl__OnDrawTask_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
        AsOffice2003(pThis), pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
}

// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
// Retail 0x1a8270 (mfc140u). The !predicate edge calls
// ?OnDrawTasksGroupCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
// with the same five arguments. The themed edge (0x1a82d3..0x1a8772 in
// mfc140u) is a long painter: gradient (FillGradient / Fill4ColorsGradient) from the
// group's +0x54 flag and this->[+0x10a38] (the style version, compared with
// 20), text with SetBkMode/SetTextColor through the group's own virtuals,
// and the collapse box via CMenuImages::Size / Draw. Unmodelled.
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* pGroup,
    int bIsHighlighted, int bIsSelected, int bCanCollapse)
{
    if (!pThis) return;
    impl__OnDrawTasksGroupCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
        AsOffice2003(pThis), pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
}

// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@H@Z
// Retail 0x1a5cc0 (mfc140u). The themed edge needs the predicate AND the
// CMFCToolBarImages at this+0x4a8 to have a non-zero +0x8; it then FillRects
// with the brush at this+0x160 (m_hObject at +0x168), fills the highlight
// through the renderer at this+0x3ef0 when bIsActive, and draws the
// this+0x4a8 image with CMFCToolBarImages::DrawEx (alignment arguments 1, 1). Every other path -- the
// reachable one -- calls
// ?OnDrawTearOffCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@H@Z
// with the same three arguments.
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectCaption, int bIsActive)
{
    if (!pThis) return;
    impl__OnDrawTearOffCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__H_Z(
        AsOffice2003(pThis), pDC, rectCaption, bIsActive);
}

// Symbol: ?OnEraseMDIClientArea@CMFCVisualManagerOffice2007@@UEAAHPEAVCDC@@VCRect@@@Z
// Retail 0x1accc0 (mfc140u), complete:
//     if (predicate() && this->[+0xc568] /* HBRUSH of the CBrush at +0xc560 */ != NULL) {
//         ::FillRect(pDC->m_hDC, &rectClient, that brush); return TRUE; }
//     return FALSE;
// Only the FALSE edge is reachable here, and it is a bare `return FALSE`, so
// this stays an honest stub: there is no base call to make.
extern "C" int MS_ABI impl__OnEraseMDIClientArea_CMFCVisualManagerOffice2007__UEAAHPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2007* /*pThis*/, CDC* /*pDC*/, CRect /*rectClient*/)
{
    return FALSE;
}

// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
// Retail 0x1a7500 (mfc140u). Predicate FALSE, pTabWnd->[+0x1fc] != 0, or any
// of the tab control's vftable +0x520 / +0x560 / +0x528 / +0x530 queries
// returning non-zero routes to
// ?OnEraseTabsArea@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
// with the same three arguments. The themed edge fills through the renderer
// at this+0x52f0 (0x200 lower when pTabWnd->[+0xf8] is set) when the +0x510
// query is TRUE, else FillGradient(rect, this->[+0x214], this->[+0x218],
// TRUE, 0, 0) -- the two colours swapped when pTabWnd->[+0xf8] is set.
// Unmodelled.
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectArea, const CMFCBaseTabCtrl* pTabWnd)
{
    if (!pThis) return;
    impl__OnEraseTabsArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
        AsOffice2003(pThis), pDC, rectArea, pTabWnd);
}

// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
// Retail 0x1a7fa0 (mfc140u). It first narrows pWndTab to a CMFCTabCtrl
// (IsKindOf against the descriptor at mfc140 0x180310b30, 'CMFCTabCtrl';
// NULL when it is not one), then: predicate FALSE, no CMFCTabCtrl,
// pWndTab->[+0x1fc] != 0, any of its vftable +0x510 / +0x520 / +0x560 /
// +0x528 / +0x530 queries non-zero, or pButton->[+0x11c] == 0 (the
// CMFCButton flag at +0x114 is tested first but never decides on its own:
// +0x114 set with +0x11c clear still takes this edge) routes to
// ?OnEraseTabsButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
// with the same four arguments (the original pWndTab, not the narrowed
// pointer). The themed edge (0x1a80bc..0x1a81df in mfc140u) starts by
// clipping to a CRgn of the rect and paints from the Office2007 block; it
// was not read further than its guards. Unmodelled.
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, CMFCButton* pButton, CMFCBaseTabCtrl* pWndTab)
{
    if (!pThis) return;
    impl__OnEraseTabsButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
        AsOffice2003(pThis), pDC, rect, pButton, pWndTab);
}

// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerOffice2007@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
// Retail 0x1a7dd0 (mfc140u). Same guard set as OnEraseTabsArea (predicate,
// pTabWnd->[+0x1fc], the four vftable queries); every failure returns
// ?OnEraseTabsFrame@CMFCVisualManagerOffice2003@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
// (same three arguments). The themed edge builds a CPen, draws from the
// Office2007 block and returns TRUE (one inner guard returns FALSE); it was
// not read further than its guards. Unmodelled.
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerOffice2007__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd)
{
    if (!pThis) return FALSE;
    return impl__OnEraseTabsFrame_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
        AsOffice2003(pThis), pDC, rect, pTabWnd);
}

// Symbol: ?OnFillBarBackground@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
// Retail 0x1a4470 (mfc140u). It calls pBar->GetRuntimeClass() (vftable slot
// 0, no NULL test on pBar) BEFORE the predicate; that result is consumed
// only on the themed edge. Then predicate FALSE, pBar->[+0xf4] != 0, or the
// runtime class deriving from CMFCColorBar (descriptor mfc140 0x1803aa0e8)
// routes to
// ?OnFillBarBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
// with the same six arguments. The themed edge (from 0x1a44fa in mfc140u)
// branches on IsKindOf(CMFCMenuBar) and the bar's vftable +0x390 style
// bits, then gradient-fills from this->[+0xc5ac..] and [+0x214..+0x21c].
// Unmodelled. DEVIATION: the unconditional GetRuntimeClass call is not
// reproduced (its value is dead on the reachable edge, and retail would
// fault on a NULL pBar where this does not).
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea)
{
    if (!pThis) return;
    impl__OnFillBarBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
        AsOffice2003(pThis), pDC, pBar, rectClient, rectClip, bNCArea);
}

// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
// Retail 0x1a6180 (mfc140u). The !predicate edge jumps straight to the call
// of
// ?OnFillButtonInterior@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
// with the same four arguments; so does the themed edge when the button
// IsKindOf CMFCCustomizeButton (descriptor mfc140 0x1803aa148). When
// ?m_bCustomizeMode@CMFCToolBar@@1HA (0x1803b70bc) is set with
// ?m_bAltCustomizeMode@CMFCToolBar@@1HA (0x1803b70cc) clear and
// pButton->[+0x50] == 0, the themed edge instead RETURNS without drawing
// (the `je` at 0x1a61fe, mfc140u, lands on the epilogue, not on the base
// call). Otherwise the themed edge maps the button style (+0x28) and state
// onto an Office2007 renderer index and fills. Unmodelled.
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect,
    CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (!pThis) return;
    impl__OnFillButtonInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        AsOffice2003(pThis), pDC, pButton, rect, state);
}

// Symbol: ?OnFillCaptionBarButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
// Retail 0x1ad040 (mfc140u), complete:
//     clr = CMFCVisualManagerOfficeXP::OnFillCaptionBarButton(all eight arguments, unchanged);
//     if (predicate() && pBar->[+0x404] != 0 && bIsSysButton && !bIsHighlighted)
//         clr = this->[+0xc5b4];
//     return clr;
// The OfficeXP call is unconditional and comes first; only the colour
// override is themed.
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int bIsPressed, int bIsHighlighted, int bIsDisabled, int bHasDropDownArrow, int bIsSysButton)
{
    if (!pThis) return 0;
    return impl__OnFillCaptionBarButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
        AsOfficeXP(pThis), pDC, pBar, rect, bIsPressed, bIsHighlighted, bIsDisabled,
        bHasDropDownArrow, bIsSysButton);
}

// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
// Retail 0x1a4990 (mfc140u). The !predicate edge calls
// ?OnFillHighlightedArea@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
// with the same four arguments. The themed edge reads pButton->[+0x60] and
// tests IsKindOf(CMFCCustomizeButton) (descriptor mfc140 0x1803aa148), and
// two of its own inner guards (the `je`s at 0x1a4a78 / 0x1a4a7d, mfc140u)
// land on the same base call; otherwise it is
// CDrawingManager(*pDC).FillGradient(rect, clr1, clr2, pButton->[+0x60], 0, 0)
// with the colour pair chosen by which of the manager's own brushes pBrush
// is (this+0x1a0 -> [+0x24c]/[+0x248], +0x1b0 -> [+0x254]/[+0x250],
// +0x1c0 -> [+0x25c]/[+0x258]; a CMFCCustomizeButton whose vftable +0xf0
// query is TRUE presets [+0x254]/[+0x250]), and either colour still -1
// is one of those two guards. Unmodelled.
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton)
{
    if (!pThis) return;
    impl__OnFillHighlightedArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
        AsOffice2003(pThis), pDC, rect, pBrush, pButton);
}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
// Retail 0x1a6c00 (mfc140u). The !predicate edge returns
// ?OnFillMiniFrameCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
// with the same four arguments. The themed edge needs no Office2007 member:
// it asks pFrameWnd's vftable +0x360 for the hosted pane, treats bActive as
// FALSE when that pane IsKindOf CMFCBaseToolBar (descriptor mfc140
// 0x1802dcad0), FillRects with afxGlobalData.brActiveCaption /
// brInactiveCaption (HBRUSH at +0xf0 / +0x100) and returns afxGlobalData
// +0x80 / +0x90 (COLOR_CAPTIONTEXT / COLOR_INACTIVECAPTIONTEXT slots). It is
// still behind the predicate, so only the OfficeXP call is reachable.
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectCaption, CPaneFrameWnd* pFrameWnd, int bActive)
{
    if (!pThis) return 0;
    return impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
        AsOfficeXP(pThis), pDC, rectCaption, pFrameWnd, bActive);
}

// Symbol: ?OnFillOutlookBarCaption@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@AEAK@Z
// Retail 0x1a4420 (mfc140u), complete:
//     CMFCVisualManagerOffice2003::OnFillOutlookBarCaption(pDC, rectCaption, clrText);
//     if (predicate()) clrText = this->[+0xc6b0];
// The base call is unconditional; only the text-colour override is themed.
extern "C" void MS_ABI impl__OnFillOutlookBarCaption_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__AEAK_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectCaption, unsigned long* pclrText)
{
    if (!pThis || !pclrText) return;
    impl__OnFillOutlookBarCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__AEAK_Z(
        AsOffice2003(pThis), pDC, rectCaption, *pclrText);
}

// Symbol: ?OnFillOutlookPageButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
// Retail 0x1a40a0 (mfc140u). The !predicate edge calls
// ?OnFillOutlookPageButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
// with the same five arguments. The themed edge needs the renderer at
// this+0x56f0 (+0xa8 non-NULL), sets clrText from this->[+0xc6ac] (pressed)
// or the neighbouring slots, and fills through that renderer. Unmodelled.
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, const CRect* pRect, int bIsHighlighted, int bIsPressed,
    unsigned long* pclrText)
{
    if (!pThis || !pRect || !pclrText) return;
    impl__OnFillOutlookPageButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(
        AsOffice2003(pThis), pDC, *pRect, bIsHighlighted, bIsPressed, *pclrText);
}

// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@@Z
// Retail 0x1acdd0 (mfc140u). The !predicate edge calls
// ?OnFillPopupWindowBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
// with the same arguments; the themed edge is
// `CDrawingManager(*pDC).FillGradient(rect, this->[+0xc760], this->[+0xc75c], TRUE, 0, 0)`.
// Unmodelled colours.
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect)
{
    if (!pThis) return;
    impl__OnFillPopupWindowBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
        AsOffice2003(pThis), pDC, rect);
}

// Symbol: ?OnFillRibbonButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1aa640 (mfc140u). The !predicate edge returns
// ?OnFillRibbonButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// with the same arguments (the value survives the epilogue's cookie check).
// The themed edge is a 0xd6b-byte dispatcher over the button's runtime
// class, IsMenuMode and a dozen of its virtuals, drawing through the
// Office2007 renderers and CMFCVisualManagerBitmapCache and returning a
// text colour from the +0xc5cc.. block. Unmodelled. The OfficeXP thunk this
// reaches is itself still a `return 0` stub in CMFCVisualManagerOfficeXP.cpp.
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return 0;
    return impl__OnFillRibbonButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
        AsOfficeXP(pThis), pDC, pButton);
}

// Symbol: ?OnFillRibbonEdit@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonRichEditCtrl@@VCRect@@HHHAEAK33@Z
// Retail 0x1ab900 (mfc140u). The !predicate edge calls
// ?OnFillRibbonEdit@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonRichEditCtrl@@VCRect@@HHHAEAK33@Z
// with the same ten arguments in the same order. The themed edge
// gradient-fills with this->[+0xc6dc] (disabled) / [+0xc6e0] (highlighted)
// / [+0xc6d8], then sets clrText = afxGlobalData[+0x44] when disabled, else
// clrText = clrSelText = this->[+0xc5cc] and clrSelBackground = this->[+0xc6f8].
// Unmodelled colours.
extern "C" void MS_ABI impl__OnFillRibbonEdit_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonRichEditCtrl__VCRect__HHHAEAK33_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, void* pEdit, CRect rect,
    int bIsHighlighted, int bIsPaneHighlighted, int bIsDisabled,
    unsigned long* pclrText, unsigned long* pclrSelBackground, unsigned long* pclrSelText)
{
    if (!pThis || !pclrText || !pclrSelBackground || !pclrSelText) return;
    impl__OnFillRibbonEdit_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonRichEditCtrl__VCRect__HHHAEAK33_Z(
        pThis, pDC, pEdit, rect, bIsHighlighted, bIsPaneHighlighted, bIsDisabled,
        *pclrText, *pclrSelBackground, *pclrSelText);
}

// Symbol: ?OnFillRibbonMainPanelButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
// Retail 0x1abc00 (mfc140u). The !predicate edge is a VIRTUAL call,
// `return this->vftable[144](pDC, pButton)` (byte offset 0x480); slot 144 of
// the Office2007 vftable is
// ?OnFillRibbonButton@CMFCVisualManagerOffice2007@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z.
// The themed edge fills through the renderer at this+0x70f0 with the
// button's +0x1b0 / +0x1c8 states and returns this->[+0xc5d0] / [+0xc5cc] /
// [+0xc5d4]. Unmodelled.
// DEVIATION: the virtual dispatch is replaced by a direct call to this
// file's OnFillRibbonButton thunk, so a derived class's override of
// OnFillRibbonButton is not reached (retail's own CMFCVisualManagerVS2008
// does not override it).
extern "C" unsigned long MS_ABI impl__OnFillRibbonMainPanelButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    if (!pThis) return 0;
    return impl__OnFillRibbonButton_CMFCVisualManagerOffice2007__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
        pThis, pDC, pButton);
}

// Symbol: ?OnFillRibbonMenuFrame@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
// Retail 0x1abaf0 (mfc140u), complete. BOTH edges execute
//     ::FillRect(pDC->m_hDC, &rect, ((CBrush*)(this + 0x180))->m_hObject);
// (the !predicate edge merely copies rect to a local first -- the inlined
// ?OnFillRibbonMenuFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z,
// mfc140u 0x1b40b0, whose body is that one FillRect). pPanel is never read.
// The +0x180 brush is OfficeXP's m_brMenuLight, so the OfficeXP thunk is the
// exact body on either edge.
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, void* pPanel, CRect rect)
{
    if (!pThis) return;
    impl__OnFillRibbonMenuFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
        AsOfficeXP(pThis), pDC, pPanel, rect);
}

// Symbol: ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@VCRect@@@Z
// Retail 0x1ac650 (mfc140u). The !predicate edge calls
// ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@VCRect@@@Z
// with the same arguments. The themed edge fills through the renderer at
// this+0xb6f0 when its +0xa8 is non-NULL, else FillGradient(rect,
// this->[+0x214], this->[+0x218], TRUE, 0, 0). Unmodelled.
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, void* pMenuBar, CRect rect)
{
    if (!pThis) return;
    impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
        pThis, pDC, pMenuBar, rect);
}

// Symbol: ?OnFillTab@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
// Retail 0x1a7ad0 (mfc140u). Predicate FALSE, pTabWnd->[+0x1fc] != 0, or any
// of the tab control's vftable +0x510 / +0x520 / +0x560 / +0x528 / +0x530
// queries returning non-zero routes to
// ?OnFillTab@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
// with the same six arguments. The themed edge fills through the renderer
// at this+0x4ef0 (0x200 lower when pTabWnd->[+0xf8] is set) with an index from
// bIsActive, iTab == pTabWnd->[+0x1dc] (the hot tab) and the style version
// this->[+0x10a38] >= 20, skipping the fill for an inactive, non-hot tab on
// the older style. Unmodelled.
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rectFill, CBrush* pBrush, int iTab, int bIsActive,
    const CMFCBaseTabCtrl* pTabWnd)
{
    if (!pThis) return;
    impl__OnFillTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
        AsOffice2003(pThis), pDC, rectFill, pBrush, iTab, bIsActive, pTabWnd);
}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
// Retail 0x1a6730 (mfc140u). The !predicate edge calls
// ?OnHighlightMenuItem@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
// with the same four arguments. The themed edge fills through the renderer
// at this+0x3ef0 (+0x200 when the button's style word +0x28 has 0x40000
// set) and never touches clrText. Unmodelled.
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect, unsigned long* pclrText)
{
    if (!pThis) return;
    impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
        AsOfficeXP(pThis), pDC, pButton, rect, pclrText);
}

// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@@Z
// Retail 0x1a67c0 (mfc140u), complete -- and it does NOT return after the
// base call:
//     if (!predicate())
//         CMFCVisualManagerOffice2003::OnHighlightRarelyUsedMenuItems(pDC, rect);  // a copy of rect
//     rect.left--;                                             // the caller's copy, in place
//     nMargin = this->vftable[185]();                          // byte offset 0x5c8: `return 3`
//     rect.right = rect.left + CMFCToolBar::GetMenuImageSize().cx + 2 * nMargin + 2;
//     ::FillRect(pDC->m_hDC, &rect, ((CBrush*)(this + 0x170))->m_hObject);   // m_brMenuRarelyUsed
// The tail after the conditional call performs exactly the operations of
// ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
// (mfc140u 0x1afbf0; compared side by side: same slot-185 virtual, same
// GetMenuImageSize call, same right-edge arithmetic, same +0x170 FillRect,
// only the register allocation differs), i.e. it is that method inlined, so
// it is expressed here as that thunk; both halves run on the reachable edge
// exactly as retail runs them.
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect)
{
    if (!pThis) return;
    impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
        AsOffice2003(pThis), pDC, rect);
    impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(
        AsOfficeXP(pThis), pDC, rect);
}

// Symbol: ?OnNcActivate@CMFCVisualManagerOffice2007@@UEAAHPEAVCWnd@@H@Z
// Retail 0x1a2310 (mfc140u), no predicate. Transcribed:
//     if (pWnd == NULL || pWnd->m_hWnd == NULL) return FALSE;
//     if (afxGlobalData.IsDwmCompositionEnabled()) return FALSE;
//     if (pWnd->[+0xa8] & 0x20) bActive = TRUE;
//     if (!pWnd->IsWindowEnabled()) bActive = FALSE;
//     if (this->vftable[127]() /* byte 0x3f8, non-exported helper mfc140 0x1971f0 */) {
//         bIsMDIFrame = pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd));   // descriptor mfc140 0x180339c20
//         bWasActive  = IsWindowActive(pWnd);                     // this file's thunk
//     } else bIsMDIFrame = bWasActive = FALSE;
//     m_ActivateFlag[pWnd->m_hWnd] = bActive;                     // CMap at this+0x10a70, operator[] mfc140 0x1ab868
//     ::SendMessage(pWnd->m_hWnd, WM_NCPAINT, 0, 0);
//     if (this->vftable[127]() && bIsMDIFrame && bWasActive != bActive)
//         ::RedrawWindow(pWnd->[+0x1d8], NULL, NULL, 0x81 /* RDW_INVALIDATE | RDW_ALLCHILDREN */);
//     return TRUE;
// (The slot-127 helper, mfc140u 0x198bf0, is `predicate() && !DwmComposition`,
// so in this build it is FALSE and bIsMDIFrame / bWasActive are FALSE.)
// Left a stub: the HWND -> BOOL map at +0x10a70 is the state this function
// exists to maintain (IsWindowActive reads it), and OpenMFC's object has no
// such member; the +0xa8 / +0x1d8 window members are likewise unmodelled.
// More importantly, the reachable retail edge returns TRUE, which tells the
// frame that the visual manager owns non-client activation -- but this
// file's OnNcPaint never paints the themed caption (its reachable edge is
// `return FALSE`), so returning TRUE here would suppress the frame's default
// WM_NCACTIVATE handling and leave captions never changing state. Returning
// FALSE keeps the default handling; it is the safer deviation.
extern "C" int MS_ABI impl__OnNcActivate_CMFCVisualManagerOffice2007__UEAAHPEAVCWnd__H_Z(
    CMFCVisualManagerOffice2007* /*pThis*/, CWnd* /*pWnd*/, int /*bActive*/)
{
    return FALSE;
}

// Symbol: ?OnNcPaint@CMFCVisualManagerOffice2007@@UEAAHPEAVCWnd@@AEBVCObList@@VCRect@@@Z
// Retail 0x1a3170 (mfc140u): `if (!predicate() || pWnd == NULL ||
// pWnd->m_hWnd == NULL) return FALSE;` -- the reachable edge -- and
// otherwise a 0x9a8-byte themed painter (CWindowDC on pWnd, clip to
// rectRedraw when non-empty, GetRibbonBar / IsWindowVisible for the ribbon
// case, caption and borders via DrawNcCaption / DrawNcBtn and the
// Office2007 renderers) returning TRUE. Only `return FALSE` is reachable, so
// this stays an honest stub.
extern "C" int MS_ABI impl__OnNcPaint_CMFCVisualManagerOffice2007__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(
    CMFCVisualManagerOffice2007* /*pThis*/, CWnd* /*pWnd*/, const CObList* /*lstSysButtons*/, CRect /*rectRedraw*/)
{
    return FALSE;
}

// Symbol: ?OnSetWindowRegion@CMFCVisualManagerOffice2007@@UEAAHPEAVCWnd@@VCSize@@@Z
// Retail 0x1a3b20 (mfc140u): `if (pWnd == NULL || pWnd->m_hWnd == NULL ||
// !predicate() || afxGlobalData.IsDwmCompositionEnabled()) return FALSE;`
// is the reachable edge. The themed edge (documented, not implemented):
// pWnd IsKindOf CMFCPopupMenu (descriptor mfc140 0x1803aa448) -> corner
// radius 3; else IsKindOf CMFCRibbonBar (0x1802fe3c8) -> FALSE; else if
// (pWnd->GetStyle() & 0x01000000 /* WS_MAXIMIZE */) -> SetWindowRgn(pWnd->m_hWnd,
// NULL, TRUE) and return TRUE; else radius 9. It then CreateRoundRectRgn(0, 0, cx+1, cy+1, r, r),
// ORs in a CreateRectRgn(0, r, cx, cy) bottom block for windows IsKindOf
// CMDIChildWnd (0x180339bf0) (CombineRgn RGN_OR), drops the HRGN from the module thread state's handle map
// (CMapPtrToWord::RemoveKey on AFX_MODULE_THREAD_STATE+0x40+0x28) and
// SetWindowRgn(pWnd->m_hWnd, hRgn, TRUE), returning TRUE. Only `return
// FALSE` is reachable here, so this stays an honest stub.
extern "C" int MS_ABI impl__OnSetWindowRegion_CMFCVisualManagerOffice2007__UEAAHPEAVCWnd__VCSize___Z(
    CMFCVisualManagerOffice2007* /*pThis*/, CWnd* /*pWnd*/, long long /*sizeWindow -- CSize by value, 8-byte aggregate in a register*/)
{
    return FALSE;
}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerOffice2007@@UEAAXXZ
// Retail 0x19b690 (mfc140u), a 0x6c38-byte function. Its prefix, which is
// what this build can run:
//     this->vftable[204]();                                    // byte 0x660 = CleanUp (this file)
//     CMFCVisualManagerOffice2003::OnUpdateSystemColors();     // mfc140u 0x192530
//     afxGlobalData.Initialize() if needed;
//     if (afxGlobalData[+0x264] != 0 || [+0x260] != 0 || [+0x288] <= 8) return;   // no rich colour desktop
//     this->[+0xf4] = 1;
//     if (m_hinstRes == NULL) SetStyle(0, NULL);               // mfc140u 0x19aa70 (lpszPath is not read there)
//     if (m_hinstRes != NULL) swap it into AfxGetModuleState()->m_hCurrentResourceHandle (+0x18);
//     CTagManager tm; strID = GetStyleResourceID(m_Style);
//     if (!tm.LoadFromResource(strID, "STYLE_XML")) { restore the handle; return; }
//     ... parse the style XML into this->[+0x10a38] (version) and the
//         whole +0x300..+0x10a70 renderer/colour block ...
// OpenMFC runs the first two calls and stops there. The desktop test could
// be recomputed (Office2003_RichColorDesktop does exactly that), but
// everything behind it is the style-module load: OpenMFC's SetStyle is a
// palette switch that never sets m_hinstRes, no STYLE_XML resource exists
// in this DLL, and the parsed block has no storage. DEVIATIONS: the
// vftable dispatch to CleanUp is a direct call (retail's own derived
// CMFCVisualManagerVS2008 does not override CleanUp); this->[+0xf4] is not
// written (no such member here); SetStyle is NOT invoked, because calling
// this tree's SetStyle(0) from here would reset the user's chosen style to
// Office2007_LunaBlue on every colour update, which retail's -- gated on
// m_hinstRes == NULL, i.e. first use only -- does not do.
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOffice2007__UEAAXXZ(CMFCVisualManagerOffice2007* pThis)
{
    if (!pThis) return;
    impl__CleanUp_CMFCVisualManagerOffice2007__MEAAXXZ(pThis);
    impl__OnUpdateSystemColors_CMFCVisualManagerOffice2003__UEAAXXZ(AsOffice2003(pThis));
}

// Symbol: ?SetResourceHandle@CMFCVisualManagerOffice2007@@SAXPEAUHINSTANCE__@@@Z
// Retail 0x19ab30 (mfc140u), complete:
//     m_bAutoFreeRes = FALSE;                                  // unconditional, before the test
//     if (m_hinstRes != hinstRes) {
//         m_hinstRes = hinstRes;
//         if (CMFCVisualManager::GetInstance()->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerOffice2007)))
//             CMFCVisualManager::GetInstance()->OnUpdateSystemColors();   // vftable slot 14, via the CFG dispatcher
//     }
// (The two statics are the named exports ?m_bAutoFreeRes@...@1HA and
// ?m_hinstRes@...@1PEAUHINSTANCE__@@EA; the descriptor is
// ?GetThisClass@CMFCVisualManagerOffice2007@@ 's.) DEVIATIONS, the same two
// CMFCVisualManagerOffice2003.cpp makes in SetDefaultWinXPColors: retail's
// GetInstance is an internal helper (mfc140 0x97f4) that creates the
// manager singleton on first use, so the exported m_pVisManager is read
// instead and a missing manager is not created; and OnUpdateSystemColors is
// called through this file's thunk rather than dispatched, so an override in
// a class derived from Office2007 (retail's CMFCVisualManagerVS2008 has one)
// is not reached.
extern "C" void MS_ABI impl__SetResourceHandle_CMFCVisualManagerOffice2007__SAXPEAUHINSTANCE_____Z(HINSTANCE hinstRes)
{
    impl__m_bAutoFreeRes_CMFCVisualManagerOffice2007__1HA = FALSE;
    if (impl__m_hinstRes_CMFCVisualManagerOffice2007__1PEAUHINSTANCE____EA == hinstRes) return;
    impl__m_hinstRes_CMFCVisualManagerOffice2007__1PEAUHINSTANCE____EA = hinstRes;
    CMFCVisualManager* pManager =
        static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pManager != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pManager, CMFCVisualManagerOffice2007::GetThisClass())) {
        impl__OnUpdateSystemColors_CMFCVisualManagerOffice2007__UEAAXXZ(
            static_cast<CMFCVisualManagerOffice2007*>(pManager));
    }
}
