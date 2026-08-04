// CMFCVisualManager — OpenMFC implementation.
// Sources: global_mfc-feature-56_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// CMFCVisualManager exports implemented for the wave2_mfc-feature-56 batch
// (30 symbols). All are virtuals of the base visual manager.
//
// Retail reference: real mfc140u.dll (14.51.36231, x64, MSVC ABI). Every
// color-returning getter reads a COLORREF field of the global AFX_GLOBAL_DATA
// object (base 0x1803c1620 in the retail image). The member offsets were
// cross-verified two ways:
//   * decoding the retail disassembly of each function, and
//   * running a wine probe (tools/harvest/family/vm56_probe.cpp) against the
//     real DLL that records each getter's return values.
// The probed values match ::GetSysColor(...) under Wine, so the impls below
// return ::GetSysColor(...) constants rather than replicating the retail
// "Initialize once, then read m_bInitialized/color field" dance. The verified
// mapping used here:
//
//   0x1648 f5f5f5 clrBtnFace        = COLOR_BTNFACE
//   0x164c a6a6a6 clrBtnShadow      = COLOR_BTNSHADOW
//   0x1650 6a6a6a clrBtnDkShadow    = COLOR_3DDKSHADOW
//   0x1654 000000 clrMenuText       = COLOR_MENUTEXT
//   0x165c 6a6a6a (btn-dkshadow variant) = COLOR_GRAYTEXT
//   0x1660 ffffff clrBtnHilite      = COLOR_BTNHIGHLIGHT
//   0x1664 6a6a6a clrGrayedText     = COLOR_GRAYTEXT
//   0x1670 fa9630 clrHotLinkNormal  = COLOR_HOTLIGHT
//   0x1674 fa9630 clrHotLinkHovered = COLOR_HOTLIGHT
//   0x1680 f5f5f5 clrBarFace        = COLOR_BTNFACE
//   0x1684 a6a6a6 clrBarShadow      = COLOR_BTNSHADOW
//   0x1688 ffffff clrBarHilite      = COLOR_BTNHIGHLIGHT
//   0x168c 6a6a6a clrBarDkShadow    = COLOR_3DDKSHADOW
//   0x1690 e3e3e3 clrBarLight       = COLOR_3DLIGHT
//   0x1694 000000 clrBarText        = COLOR_BTNTEXT
//   0x1698 ffffff clrWindow         = COLOR_WINDOW
//   0x16a8 fa9632 clrActiveCaptionGradient = COLOR_ACTIVECAPTION
//
// The many OnDraw* entries are large painting routines that rasterize ribbon /
// caption-bar / splitter chrome through CDC + internal helpers (FillSolidRect,
// FillGradient, CPen) that the clean-room classes do not model. Per the
// implementation guide they get conservative, type-correct no-ops (NULL
// checks only, nothing drawn, no state touched) marked TODO(clean-room).
// GetTabFrameColors likewise cannot return the retail static CBrush objects
// (they live in afxGlobalData, which OpenMFC does not model), so the brush
// out-params are set to nullptr.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// Color getters (all read AFX_GLOBAL_DATA color fields in retail)
//=============================================================================













//=============================================================================
// On* notifications
//=============================================================================


//=============================================================================
// OnDraw* painting routines (conservative no-ops, see header comment)
//=============================================================================

















// CMFCVisualManager::GetPropertyGridGroupColor(CMFCPropertyGridCtrl*) -- retail
// returns clrBtnFace (0x1648) when the grid's +0x5fc flag is zero, else
// clrBarFace (0x1680); both are COLOR_BTNFACE. The +0x5fc member is not
// modeled, so both paths collapse to the same constant.
// Symbol: ?GetPropertyGridGroupColor@CMFCVisualManager@@UEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* /*pGrid*/)
{
    return pThis ? ::GetSysColor(COLOR_BTNFACE) : 0;
}
// CMFCVisualManager::GetPropertyGridGroupTextColor(CMFCPropertyGridCtrl*) --
// retail returns clrBtnDkShadow (0x168c) when the grid's +0x5fc flag is
// nonzero, else the 0x165c button variant; Wine probe shows 0x6a6a6a on both
// paths (the +0x5fc member is not modeled).
// Symbol: ?GetPropertyGridGroupTextColor@CMFCVisualManager@@UEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupTextColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* /*pGrid*/)
{
    return pThis ? ::GetSysColor(COLOR_GRAYTEXT) : 0;
}
// CMFCVisualManager::GetRibbonEditBackgroundColor(...) -- retail returns
// clrWindow (0x1698) when bIsHighlighted && !bIsDisabled, else clrBarFace
// (0x1680). CMFCRibbonRichEditCtrl is not declared in OpenMFC (opaque).
// Symbol: ?GetRibbonEditBackgroundColor@CMFCVisualManager@@UEAAKPEAVCMFCRibbonRichEditCtrl@@HHH@Z
extern "C" unsigned long MS_ABI impl__GetRibbonEditBackgroundColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonRichEditCtrl__HHH_Z(
    CMFCVisualManager* pThis, void* /*pEdit*/, int bIsHighlighted, int /*bIsPaneHighlighted*/, int bIsDisabled)
{
    if (!pThis) return 0;
    return (bIsHighlighted && !bIsDisabled) ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE);
}
// CMFCVisualManager::GetRibbonHyperlinkTextColor(CMFCRibbonLinkCtrl*) -- retail
// asks the link for IsVisited() (vptr+0x1c8) / IsHighlighted() (vptr+0x1b0)
// and returns clrHotLinkNormalText (0x1670) / clrHotLinkHoveredText (0x1674);
// both are COLOR_HOTLIGHT and the default-constructed link probes as fa9630.
// CMFCRibbonLinkCtrl is not declared in OpenMFC (opaque).
// Symbol: ?GetRibbonHyperlinkTextColor@CMFCVisualManager@@UEAAKPEAVCMFCRibbonLinkCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetRibbonHyperlinkTextColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonLinkCtrl___Z(
    CMFCVisualManager* pThis, void* /*pLink*/)
{
    return pThis ? ::GetSysColor(COLOR_HOTLIGHT) : 0;
}
// CMFCVisualManager::GetRibbonSliderColors(...) -- faithful transcription of
// the retail body (RVA 0x18dfa0). clrLine is clrBarShadow when disabled,
// else clrBarDkShadow. clrFill is clrBarShadow when hovered-and-pressed,
// clrBarHilite when just hovered, clrBarFace otherwise. Probe-verified.
// Symbol: ?GetRibbonSliderColors@CMFCVisualManager@@UEAAXPEAVCMFCRibbonSlider@@HHHAEAK1@Z
extern "C" void MS_ABI impl__GetRibbonSliderColors_CMFCVisualManager__UEAAXPEAVCMFCRibbonSlider__HHHAEAK1_Z(
    CMFCVisualManager* pThis, CMFCRibbonSlider* /*pSlider*/, int bIsHighlighted, int bIsPressed,
    int bIsDisabled, unsigned long& clrLine, unsigned long& clrFill)
{
    if (!pThis) return;
    clrLine = bIsDisabled ? ::GetSysColor(COLOR_BTNSHADOW) : ::GetSysColor(COLOR_3DDKSHADOW);
    if (bIsHighlighted && bIsPressed) {
        clrFill = ::GetSysColor(COLOR_BTNSHADOW);
    } else if (bIsHighlighted) {
        clrFill = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    } else {
        clrFill = ::GetSysColor(COLOR_BTNFACE);
    }
}
// CMFCVisualManager::GetRibbonStatusBarTextColor(...) -- retail returns
// clrBarText (0x1694) unconditionally (Wine probe: 0x00000000).
// Symbol: ?GetRibbonStatusBarTextColor@CMFCVisualManager@@UEAAKPEAVCMFCRibbonStatusBar@@@Z
extern "C" unsigned long MS_ABI impl__GetRibbonStatusBarTextColor_CMFCVisualManager__UEAAKPEAVCMFCRibbonStatusBar___Z(
    CMFCVisualManager* pThis, CMFCRibbonStatusBar* /*pStatusBar*/)
{
    return pThis ? ::GetSysColor(COLOR_BTNTEXT) : 0;
}
// CMFCVisualManager::GetSmartDockingBaseGuideColors(...) -- retail writes
// clrBarFace (0x1680) then clrBarShadow (0x1684). Probe: (f5f5f5, a6a6a6).
// Symbol: ?GetSmartDockingBaseGuideColors@CMFCVisualManager@@UEAAXAEAK0@Z
extern "C" void MS_ABI impl__GetSmartDockingBaseGuideColors_CMFCVisualManager__UEAAXAEAK0_Z(
    CMFCVisualManager* pThis, unsigned long& clrBaseGroupBackground, unsigned long& clrBaseGroupBorder)
{
    if (!pThis) return;
    clrBaseGroupBackground = ::GetSysColor(COLOR_BTNFACE);
    clrBaseGroupBorder = ::GetSysColor(COLOR_BTNSHADOW);
}
// CMFCVisualManager::GetSmartDockingHighlightToneColor() -- retail returns
// clrActiveCaptionGradient (0x16a8). Probe: 0x00fa9632.
// Symbol: ?GetSmartDockingHighlightToneColor@CMFCVisualManager@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetSmartDockingHighlightToneColor_CMFCVisualManager__UEAAKXZ(
    CMFCVisualManager* pThis)
{
    return pThis ? ::GetSysColor(COLOR_ACTIVECAPTION) : 0;
}
// CMFCVisualManager::GetStatusBarPaneTextColor(...) -- faithful transcription
// of the retail body (RVA 0x18b870). CMFCStatusBarPaneInfo is not declared in
// OpenMFC, so it is treated as an opaque buffer with the harvested offsets:
//   +0x14 dwStyle-ish flags (bit 0x04000000 => grayed text)
//   +0x24 clrText (-1 => default text color, else returned verbatim)
// Retail calls __report_rangecheckfailure on a NULL pane; we return a safe
// default instead.
// Symbol: ?GetStatusBarPaneTextColor@CMFCVisualManager@@UEAAKPEAVCMFCStatusBar@@PEAVCMFCStatusBarPaneInfo@@@Z
extern "C" unsigned long MS_ABI impl__GetStatusBarPaneTextColor_CMFCVisualManager__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(
    CMFCVisualManager* pThis, CMFCStatusBar* /*pStatusBar*/, void* pPane)
{
    if (!pThis) return 0;
    if (pPane == nullptr) return ::GetSysColor(COLOR_WINDOWTEXT); // retail would fault here
    const unsigned char* pane = static_cast<const unsigned char*>(pPane);
    DWORD dwStyle = 0;
    std::memcpy(&dwStyle, pane + 0x14, sizeof(dwStyle));
    if (dwStyle & 0x04000000) { // SBPS_DISABLED-ish flag -> grayed text (0x1664)
        return ::GetSysColor(COLOR_GRAYTEXT);
    }
    DWORD clrText = 0;
    std::memcpy(&clrText, pane + 0x24, sizeof(clrText));
    if (clrText == 0xFFFFFFFF) { // -1 -> default menu text (0x1654, probe: 0x00000000)
        return ::GetSysColor(COLOR_MENUTEXT);
    }
    return clrText; // pane carries its own explicit color
}
// CMFCVisualManager::GetTabFrameColors(...) -- the retail body (RVA 0x18a9d0)
// branches on the tab's m_nHighlightedTab member (pTabWnd+0x1fc) and three
// vtable calls. The default branch (m_nHighlightedTab == 0, probe-verified
// with a plain CMFCTabCtrl) yields the six colors below. The brushes are
// pointers to static afxGlobalData CBrush objects in retail; OpenMFC has no
// such globals, so the out-params are set to nullptr (callers must tolerate
// it until afxGlobalData is modeled).
// TODO(clean-room): transcribed partially -- highlighted-tab branch and static
// afxGlobalData brushes are not modeled.
// Symbol: ?GetTabFrameColors@CMFCVisualManager@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManager__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    CMFCVisualManager* pThis, const CMFCBaseTabCtrl* /*pTabWnd*/, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack)
{
    if (!pThis) return;
    clrDark = ::GetSysColor(COLOR_BTNSHADOW);        // 0x1684
    clrBlack = ::GetSysColor(COLOR_BTNTEXT);         // 0x1694
    clrHighlight = ::GetSysColor(COLOR_BTNHIGHLIGHT); // 0x1688
    clrFace = ::GetSysColor(COLOR_BTNFACE);          // 0x1680
    clrDarkShadow = ::GetSysColor(COLOR_3DDKSHADOW); // 0x168c
    clrLight = ::GetSysColor(COLOR_3DLIGHT);         // 0x1690
    pbrFace = nullptr;
    pbrBlack = nullptr;
}
// CMFCVisualManager::GetToolTipInfo(...) -- full faithful transcription of the
// retail body (RVA 0x18f0c0); nType is unused. CMFCToolTipInfo is an opaque
// 96-byte blob in OpenMFC, so the fields are written by raw offset (verified
// byte-for-byte against the wine probe of the real CMFCToolTipInfo).
// Symbol: ?GetToolTipInfo@CMFCVisualManager@@UEAAHAEAVCMFCToolTipInfo@@I@Z
extern "C" int MS_ABI impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
    CMFCVisualManager* pThis, CMFCToolTipInfo& params, unsigned int /*nType*/)
{
    if (!pThis) return 0;
    int* raw = reinterpret_cast<int*>(&params);
    raw[0x00 / 4] = 0;      // m_bBalloonTooltip = FALSE
    raw[0x04 / 4] = 1;      // m_bDrawIcon = TRUE
    raw[0x08 / 4] = 1;      // m_bDrawDescription = TRUE
    raw[0x0c / 4] = 0;      // m_bRoundedCorners = FALSE
    raw[0x10 / 4] = 1;      // m_bBoldLabel = TRUE
    raw[0x14 / 4] = 1;      // m_bDrawSeparator = TRUE
    raw[0x18 / 4] = 0;      // m_bVislManagerTheme = FALSE
    raw[0x1c / 4] = 200;    // m_nMaxDescrWidth = 200
    raw[0x20 / 4] = -1;     // m_nGradientAngle = -1
    raw[0x24 / 4] = -1;     // m_clrFill = -1
    raw[0x28 / 4] = -1;     // m_clrFillGradient = -1
    raw[0x2c / 4] = -1;     // m_clrText = -1
    raw[0x30 / 4] = -1;     // m_clrBorder = -1
    return TRUE;
}
// CMFCVisualManager::GetToolbarButtonTextColor(...) -- retail body (RVA
// 0x188220) branches on a VM flag, pButton->m_nStyle bit 0x40000 (Feature Pack
// disabled bit, not in the public headers), IsKindOf(CMFCToolBarMenuButton)
// and nState. For a plain button the probe returns 0 for all four states.
// Conservative transcription:
//   * disabled bit set  -> clrGrayedText (0x1664)
//   * state == 2 (retail AFX_BUTTON_STATE::ButtonsIsChecked)
//                        -> CMFCToolBar::GetHotTextColor() (0 on a default VM)
//   * otherwise          -> clrBarText (0x1694)
// Note the retail AFX_BUTTON_STATE enum is {Pressed,Highlighted,Checked,
// Disabled}; OpenMFC's header declares a different value set, so nState is
// compared against the raw retail value 2.
// TODO(clean-room): transcribed partially -- menu-button/theme branches not
// modeled.
// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManager@@UEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@1@@Z
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CMFCToolBarButton* pButton, int nState)
{
    if (!pThis) return 0;
    if (pButton != nullptr && (pButton->m_nStyle & 0x40000)) {
        return ::GetSysColor(COLOR_GRAYTEXT);
    }
    if (nState == 2) { // retail ButtonsIsChecked -> hot text color (0 on default theme)
        return ::GetSysColor(COLOR_WINDOWTEXT);
    }
    return ::GetSysColor(COLOR_BTNTEXT);
}
// CMFCVisualManager::OnActivateApp(...) -- retail body is `ret $0` (no-op).
// Symbol: ?OnActivateApp@CMFCVisualManager@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivateApp_CMFCVisualManager__UEAAXPEAVCWnd__H_Z(
    CMFCVisualManager* /*pThis*/, CWnd* /*pWnd*/, int /*bActive*/)
{
    // Retail is a pure no-op (verified: `ret $0`).
}
// CMFCVisualManager::OnDrawAutoHideButtonBorder(...) -- retail draws up to
// four 1px border segments of the auto-hide button using clrBarShadow (0x1684)
// plus a rounded-corner arc. CMFCAutoHideButton is not declared in OpenMFC.
// TODO(clean-room): transcribed partially -- border rasterization not modeled.
// Symbol: ?OnDrawAutoHideButtonBorder@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@1PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnDrawAutoHideButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect /*rectBounds*/, CRect /*rectBorderSize*/, void* /*pButton*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawBrowseButton(...) -- retail fills the button with a
// pattern brush, runs a gradient and frames it, then returns TRUE. The wine
// probe confirms the return value is TRUE on the default path; clrText is not
// written by the base implementation. CMFCEditBrowseCtrl is not declared.
// TODO(clean-room): transcribed partially -- browse-button rasterization not
// modeled.
// Symbol: ?OnDrawBrowseButton@CMFCVisualManager@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@1@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManager__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_1_AEAK_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect /*rect*/, void* /*pEdit*/, int /*nState*/, COLORREF* /*pClrText*/)
{
    if (!pThis || !pDC) return 0;
    return TRUE;
}
// CMFCVisualManager::OnDrawCaptionBarBorder(...) -- retail fills the border
// area with clrBarBorder (or the global bar brush when clrBarBorder == -1) and
// paints a gradient when bFlatBorder is false.
// TODO(clean-room): transcribed partially -- caption-bar border rasterization
// not modeled.
// Symbol: ?OnDrawCaptionBarBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@KH@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__KH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* /*pBar*/, CRect /*rect*/,
    unsigned long /*clrBarBorder*/, int /*bFlatBorder*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawCaptionBarButtonBorder(...) -- retail fills the
// button rect with a clrBarHilite/clrBarShadow (0x1688/0x168c) gradient,
// choosing the pair from the disabled / pressed / caption-bar state.
// TODO(clean-room): transcribed partially -- caption-bar button rasterization
// not modeled.
// Symbol: ?OnDrawCaptionBarButtonBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* /*pBar*/, CRect /*rect*/,
    int /*bIsPressed*/, int /*bIsHighlighted*/, int /*bIsDisabled*/, int /*bHasDropDownArrow*/, int /*bIsSysButton*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawCaptionBarInfoArea(...) -- retail fills the info
// area with a clrBarHilite/clrBarShadow gradient (0x1688/0x1684) then a
// clrBarShadow/clrBarHilite gradient.
// TODO(clean-room): transcribed partially -- caption-bar info-area
// rasterization not modeled.
// Symbol: ?OnDrawCaptionBarInfoArea@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarInfoArea_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* /*pBar*/, CRect /*rect*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawCheckBox(...) -- retail delegates straight to
// OnDrawCheckBoxEx (vtable+0x358): nState = (bChecked != 0), bPressed = FALSE.
// The clean-room OnDrawCheckBoxEx is declared and implemented, so this wrapper
// is a faithful virtual delegation.
// Symbol: ?OnDrawCheckBox@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawCheckBox_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bHighlighted, int bChecked, int bEnabled)
{
    if (!pThis || !pDC) return;
    pThis->OnDrawCheckBoxEx(pDC, rect, bChecked ? 1 : 0, bHighlighted, FALSE, bEnabled);
}
// CMFCVisualManager::OnDrawDefaultRibbonImage(...) -- retail draws a default
// ribbon glyph (image placeholder) centered in the rect with a frame.
// TODO(clean-room): transcribed partially -- ribbon image rasterization not
// modeled.
// Symbol: ?OnDrawDefaultRibbonImage@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawDefaultRibbonImage_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect /*rectImage*/, int /*bIsDisabled*/, int /*bIsPressed*/, int /*bIsHighlighted*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawMenuLabel(...) -- retail fills the label with a
// pattern brush, paints a 2px gradient strip at the bottom (clrBtnShadow ->
// clrBtnDkShadow, 0x164c -> 0x1650) and returns clrMenuText (0x1654, probe: 0).
// TODO(clean-room): transcribed partially -- label rasterization not modeled.
// Symbol: ?OnDrawMenuLabel@CMFCVisualManager@@UEAAKPEAVCDC@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManager__UEAAKPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect /*rect*/)
{
    if (!pThis || !pDC) return 0;
    return ::GetSysColor(COLOR_MENUTEXT); // 0x1654, probe: 0x00000000
}
// CMFCVisualManager::OnDrawRibbonColorPaletteBox(...) -- retail paints the
// color swatch with the provided COLORREF (or a checker pattern) plus edge
// highlights. CMFCRibbonColorButton / CMFCRibbonGalleryIcon are not declared.
// TODO(clean-room): transcribed partially -- color-palette rasterization not
// modeled.
// Symbol: ?OnDrawRibbonColorPaletteBox@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonColorButton@@PEAVCMFCRibbonGalleryIcon@@KVCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonColorPaletteBox_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonColorButton__PEAVCMFCRibbonGalleryIcon__KVCRect__HHHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pColorButton*/, void* /*pIcon*/, unsigned long /*color*/,
    CRect /*rect*/, int /*bDrawTopEdge*/, int /*bDrawBottomEdge*/, int /*bIsHighlighted*/, int /*bIsChecked*/, int /*bIsDisabled*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawRibbonDefaultPaneButtonIndicator(...) -- retail
// paints a small triangle indicator on the ribbon default-pane button.
// TODO(clean-room): transcribed partially -- indicator rasterization not
// modeled.
// Symbol: ?OnDrawRibbonDefaultPaneButtonIndicator@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawRibbonDefaultPaneButtonIndicator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect__HH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonButton* /*pButton*/, CRect /*rect*/, int /*bIsSelected*/, int /*bHighlighted*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawRibbonGalleryButton(...) -- retail delegates to two
// gallery virtuals (vptr+0x480, vptr+0x490). CMFCRibbonGalleryIcon is not
// declared in OpenMFC.
// TODO(clean-room): transcribed partially -- gallery-button rasterization not
// modeled.
// Symbol: ?OnDrawRibbonGalleryButton@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonGalleryIcon@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonGalleryButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonGalleryIcon___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pButton*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawRibbonKeyTip(...) -- retail draws a small key-tip
// box (with the shortcut text) at the given rect. The by-value CString is
// received as a pointer in the 5th argument slot (MSVC x64 passes non-trivial
// classes by hidden pointer).
// TODO(clean-room): transcribed partially -- key-tip rasterization not modeled.
// Symbol: ?OnDrawRibbonKeyTip@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonBaseElement@@VCRect@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonKeyTip_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonBaseElement__VCRect__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonBaseElement* /*pElement*/, CRect /*rect*/, const CString& /*str*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawRibbonLaunchButton(...) -- retail reads the launch
// button's pressed/hovered state and paints the small launch glyph (image at
// index 0x23). CMFCRibbonLaunchButton is not declared in OpenMFC.
// TODO(clean-room): transcribed partially -- launch-button rasterization not
// modeled.
// Symbol: ?OnDrawRibbonLaunchButton@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonLaunchButton@@PEAVCMFCRibbonPanel@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonLaunchButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonLaunchButton__PEAVCMFCRibbonPanel___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pButton*/, CMFCRibbonPanel* /*pPanel*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawRibbonMenuCheckFrame(...) -- retail fills the check
// rect with a pattern brush and paints a clrBtnShadow/clrBtnDkShadow gradient
// (0x164c -> 0x1650).
// TODO(clean-room): transcribed partially -- menu-check rasterization not
// modeled.
// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonButton* /*pButton*/, CRect /*rect*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawRibbonRecentFilesFrame(...) -- retail fills the
// recent-files list frame and paints a clrBtnShadow/clrBtnDkShadow gradient
// (0x164c -> 0x1650).
// TODO(clean-room): transcribed partially -- recent-files frame rasterization
// not modeled.
// Symbol: ?OnDrawRibbonRecentFilesFrame@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonMainPanel* /*pPanel*/, CRect /*rect*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawSplitterBorder(...) -- retail paints a gradient
// (clrBarHilite 0x1688 -> clrBarShadow 0x1684) then frames the splitter with
// clrBarFace (0x1680). CSplitterWndEx is not declared in OpenMFC.
// TODO(clean-room): transcribed partially -- splitter border rasterization not
// modeled.
// Symbol: ?OnDrawSplitterBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCSplitterWndEx@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitterBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCSplitterWndEx__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pSplitterWnd*/, CRect /*rect*/)
{
    if (!pThis || !pDC) return;
}
// CMFCVisualManager::OnDrawSplitterBox(...) -- retail paints a gradient
// (clrBarShadow 0x1684 -> clrBarFace 0x1680) in the splitter box rect.
// CSplitterWndEx is not declared in OpenMFC.
// TODO(clean-room): transcribed partially -- splitter box rasterization not
// modeled.
// Symbol: ?OnDrawSplitterBox@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCSplitterWndEx@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitterBox_CMFCVisualManager__UEAAXPEAVCDC__PEAVCSplitterWndEx__AEAVCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pSplitterWnd*/, CRect& /*rect*/)
{
    if (!pThis || !pDC) return;
}
// Symbol: ?AdjustFrames@CMFCVisualManager@@SAXXZ
extern "C" void MS_ABI impl__AdjustFrames_CMFCVisualManager__SAXXZ() {
}
// Symbol: ?AdjustToolbars@CMFCVisualManager@@SAXXZ
extern "C" void MS_ABI impl__AdjustToolbars_CMFCVisualManager__SAXXZ() {
}
// Symbol: ?CreateVisualManager@CMFCVisualManager@@KAPEAV1@PEAUCRuntimeClass@@@Z
extern "C" CMFCVisualManager* MS_ABI impl__CreateVisualManager_CMFCVisualManager__KAPEAV1_PEAUCRuntimeClass___Z(CRuntimeClass* pRTI) {
    return CreateVisualManagerFromRuntimeClass(pRTI);
}
// Symbol: ?DestroyInstance@CMFCVisualManager@@SAXH@Z
extern "C" void MS_ABI impl__DestroyInstance_CMFCVisualManager__SAXH_Z(int) {
    delete g_pVisualManager;
    g_pVisualManager = nullptr;
    SyncVisualManagerExports();
}
// Symbol: ?CreateObject@CMFCVisualManager@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManager__SAPEAVCObject__XZ() { return new CMFCVisualManager(); }
// Symbol: ?GetThisClass@CMFCVisualManager@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManager__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManager::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManager@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManager__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManager* pThis) { return CMFCVisualManager::GetThisClass(); }
// Symbol: ?GetToolbarDisabledTextColor@CMFCVisualManager@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetToolbarDisabledTextColor_CMFCVisualManager__UEAAKXZ(CMFCVisualManager* pThis) {
    return pThis ? pThis->GetToolbarDisabledTextColor() : ::GetSysColor(COLOR_GRAYTEXT);
}
// Symbol: ?GetToolbarHighlightColor@CMFCVisualManager@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetToolbarHighlightColor_CMFCVisualManager__UEAAKXZ(CMFCVisualManager* pThis) {
    return pThis ? pThis->GetToolbarHighlightColor() : ::GetSysColor(COLOR_HIGHLIGHT);
}
// Symbol: ?GetShowAllMenuItemsHeight@CMFCVisualManager@@UEAAHPEAVCDC@@AEBVCSize@@@Z
extern "C" int MS_ABI impl__GetShowAllMenuItemsHeight_CMFCVisualManager__UEAAHPEAVCDC__AEBVCSize___Z(
    CMFCVisualManager* pThis, CDC* pDC, const CSize* pSize) {
    static const CSize defaultSize(0, 16);
    const CSize& size = pSize != nullptr ? *pSize : defaultSize;
    return pThis ? pThis->GetShowAllMenuItemsHeight(pDC, size) : (size.cy > 0 ? size.cy : 16);
}
// Symbol: ?OnDrawButtonBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@1@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}
// Symbol: ?OnDrawButtonSeparator@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@1@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1_H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE /*state*/, int bHorz) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawButtonSeparator(pDC, pButton, rect, bHorz);
}
// Symbol: ?OnDrawCaptionButton@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionButton* pButton, int a, int b, BOOL bDisabled, int, int) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawCaptionButton(pDC, pButton, a, b, bDisabled);
}
// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCBaseToolBar* pBar, CRect rectBorder, CRect rect) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawFloatingToolbarBorder(pDC, reinterpret_cast<CMFCToolBar*>(pBar), rectBorder, rect);
}
// Symbol: ?OnDrawMenuShadow@CMFCVisualManager@@UEAAXPEAVCDC@@AEBVCRect@@1HHHPEAVCBitmap@@2H@Z
extern "C" void MS_ABI impl__OnDrawMenuShadow_CMFCVisualManager__UEAAXPEAVCDC__AEBVCRect__1HHHPEAVCBitmap__2H_Z(
    CMFCVisualManager* pThis, CDC* pDC, const CRect& rectClient, const CRect& rectExclude, int nDepth, int, int,
    CBitmap* pBmp, CBitmap* pBmp2, COLORREF color) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawMenuShadow(pDC, rectClient, rectExclude, nDepth, 0, 0, pBmp, pBmp2, color);
}
// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManager__UEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonCategoryScroll* pScroll) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawRibbonCategoryScroll(pDC, pScroll);
}
// Symbol: ?OnDrawShowAllMenuItems@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@W4AFX_BUTTON_STATE@1@@Z
extern "C" void MS_ABI impl__OnDrawShowAllMenuItems_CMFCVisualManager__UEAAXPEAVCDC__VCRect__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawShowAllMenuItems(pDC, rect, state);
}
// Symbol: ?OnDrawSpinButtons@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int nState, BOOL bIsHovered, CMFCSpinButtonCtrl* pSpinBtn) {
    if (!pThis || !pDC) return;
    pThis->CMFCVisualManager::OnDrawSpinButtons(pDC, rect, nState, TRUE, bIsHovered);
}
// Symbol: ?OnDrawPropertySheetListItem@CMFCVisualManager@@UEAAKPEAVCDC@@PEAVCMFCPropertySheet@@VCRect@@HH@Z
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void*, const CRect& rect, int nItem, int nState) {
    (void)pThis;
    return DrawPropertySheetListItemFallback(pThis, pDC, rect, nItem, nState);
}
CMFCVisualManager::CMFCVisualManager() {
    memset(_visualmanager_padding, 0, sizeof(_visualmanager_padding));
    if (g_pVisualManager == nullptr) {
        g_pVisualManager = this;
        SyncVisualManagerExports();
    }
}
CMFCVisualManager::~CMFCVisualManager() {
    if (g_pVisualManager == this) {
        g_pVisualManager = nullptr;
        SyncVisualManagerExports();
    }
}
CMFCVisualManager* CMFCVisualManager::GetInstance() {
    if (!g_pVisualManager) {
        g_pVisualManager = CreateVisualManagerFromRuntimeClass(g_pDefaultVisualManagerClass);
    }
    SyncVisualManagerExports();
    return g_pVisualManager;
}
void CMFCVisualManager::SetDefaultManager(CRuntimeClass* pRTI) {
    g_pDefaultVisualManagerClass = pRTI;
    if (g_pDefaultVisualManagerClass == nullptr ||
        !g_pDefaultVisualManagerClass->IsDerivedFrom(RUNTIME_CLASS(CMFCVisualManager))) {
        g_pDefaultVisualManagerClass = RUNTIME_CLASS(CMFCVisualManager);
    }
    if (g_pVisualManager != nullptr && !g_pVisualManager->IsKindOf(g_pDefaultVisualManagerClass)) {
        delete g_pVisualManager;
        g_pVisualManager = nullptr;
    }
    ApplyVisualPaletteForClass(g_pDefaultVisualManagerClass);
    SyncVisualManagerExports();
}
void CMFCVisualManager::OnDrawMenuBorder(CDC* pDC, CMFCPopupMenu*, CRect rect) { FrameSolid(pDC, rect, g_visualBorderColor); }
void CMFCVisualManager::OnDrawMenuImage(CDC* pDC, const CRect& rect, const CRect& rectImage) {
    FillSolid(pDC, rect, ::GetSysColor(COLOR_MENU));
    FrameSolid(pDC, rectImage, ::GetSysColor(COLOR_3DSHADOW));
}
void CMFCVisualManager::OnFillBarBackground(CDC* pDC, CBasePane*, CRect rectClient, CRect rectClip, BOOL) {
    FillSolid(pDC, IsDrawableRect(rectClip) ? rectClip : rectClient, g_visualBackgroundColor);
}
void CMFCVisualManager::OnDrawBarGripper(CDC* pDC, CRect rectGripper, BOOL bHorz, CBasePane*) {
    COLORREF color = ::GetSysColor(COLOR_3DSHADOW);
    if (bHorz) {
        for (int y = rectGripper.top + 3; y + 1 < rectGripper.bottom; y += 4) {
            FillSolid(pDC, CRect(rectGripper.left + 3, y, rectGripper.left + 5, y + 2), color);
        }
    } else {
        for (int x = rectGripper.left + 3; x + 1 < rectGripper.right; x += 4) {
            FillSolid(pDC, CRect(x, rectGripper.top + 3, x + 2, rectGripper.top + 5), color);
        }
    }
}
void CMFCVisualManager::OnDrawButtonBorder(CDC* pDC, CMFCToolBarButton*, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    FillAndFrame(pDC, rect, ButtonFillForState(state), state == ButtonsIsPressed ? ::GetSysColor(COLOR_3DDKSHADOW) : g_visualBorderColor);
}
void CMFCVisualManager::OnDrawButtonSeparator(CDC* pDC, CMFCToolBarButton*, CRect rect, BOOL bHorz) {
    if (bHorz) {
        int x = (rect.left + rect.right) / 2;
        DrawLine(pDC, x, rect.top + 2, x, rect.bottom - 2, ::GetSysColor(COLOR_3DSHADOW));
    } else {
        int y = (rect.top + rect.bottom) / 2;
        DrawLine(pDC, rect.left + 2, y, rect.right - 2, y, ::GetSysColor(COLOR_3DSHADOW));
    }
}
void CMFCVisualManager::OnDrawCaptionButton(CDC* pDC, CMFCCaptionButton*, BOOL, BOOL, BOOL bDisabled) {
    CRect rect(0, 0, 16, 16);
    FillAndFrame(pDC, rect, ::GetSysColor(COLOR_BTNFACE), bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : g_visualBorderColor);
}
void CMFCVisualManager::OnDrawCaptionButtonIcon(CDC* pDC, CMFCCaptionButton*, CMenuImages::IMAGES_IDS, BOOL, BOOL) {
    DrawRectText(pDC, CRect(0, 0, 16, 16), L"x", ::GetSysColor(COLOR_BTNTEXT));
}
void CMFCVisualManager::OnDrawCheckBoxEx(CDC* pDC, CRect rect, int nState, BOOL bHighlighted, BOOL bPressed, BOOL bEnabled) {
    FillAndFrame(pDC, rect, bEnabled ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE),
                 (bHighlighted || bPressed) ? g_visualAccentColor : g_visualBorderColor);
    if (nState != 0) {
        COLORREF markColor = ::GetSysColor(bEnabled ? COLOR_BTNTEXT : COLOR_GRAYTEXT);
        DrawLine(pDC, rect.left + 3, (rect.top + rect.bottom) / 2, rect.left + rect.Width() / 2, rect.bottom - 4, markColor);
        DrawLine(pDC, rect.left + rect.Width() / 2, rect.bottom - 4, rect.right - 3, rect.top + 3, markColor);
    }
}
void CMFCVisualManager::OnDrawComboBorder(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton*) {
    FrameSolid(pDC, rect, bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : (bIsDropped || bIsHighlighted ? g_visualAccentColor : g_visualBorderColor));
}
void CMFCVisualManager::OnDrawComboDropButton(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton*) {
    FillAndFrame(pDC, rect, (bIsDropped || bIsHighlighted) ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE),
                 bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : g_visualBorderColor);
    DrawArrowGlyph(pDC, rect, true, bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_BTNTEXT));
}
void CMFCVisualManager::OnDrawControlBorder(CWnd* pWndCtrl) {
    if (!pWndCtrl || !pWndCtrl->GetSafeHwnd()) return;
    HDC hdc = ::GetWindowDC(pWndCtrl->GetSafeHwnd());
    if (!hdc) return;
    RECT rect{};
    ::GetWindowRect(pWndCtrl->GetSafeHwnd(), &rect);
    ::OffsetRect(&rect, -rect.left, -rect.top);
    HBRUSH brush = ::CreateSolidBrush(g_visualBorderColor);
    if (brush) {
        ::FrameRect(hdc, &rect, brush);
        ::DeleteObject(brush);
    }
    ::ReleaseDC(pWndCtrl->GetSafeHwnd(), hdc);
}
void CMFCVisualManager::OnDrawDockingBarScrollButton(CDC* pDC, CMFCToolBarButton*, BOOL bUp, BOOL bHorz) {
    CRect rect(0, 0, 16, 16);
    FillAndFrame(pDC, rect, ::GetSysColor(COLOR_BTNFACE), g_visualBorderColor);
    DrawArrowGlyph(pDC, rect, bHorz ? false : !bUp, ::GetSysColor(COLOR_BTNTEXT));
}
void CMFCVisualManager::OnDrawEditBorder(CDC* pDC, CRect rect, BOOL bDisabled, BOOL bIsHighlighted, CMFCToolBarEditBoxButton*) {
    FrameSolid(pDC, rect, bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : (bIsHighlighted ? g_visualAccentColor : g_visualBorderColor));
}
void CMFCVisualManager::OnDrawExpandingBox(CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox) { DrawPlusMinus(pDC, rect, bIsOpened, colorBox); }
void CMFCVisualManager::OnDrawFloatingToolbarBorder(CDC* pDC, CMFCToolBar*, CRect rectBorder, CRect) { Draw3dFrame(pDC, rectBorder, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW)); }
void CMFCVisualManager::OnDrawHeaderCtrlBorder(CMFCHeaderCtrl*, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted) {
    FillAndFrame(pDC, rect, bIsPressed ? ::GetSysColor(COLOR_3DLIGHT) : (bIsHighlighted ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE)), g_visualBorderColor);
}
void CMFCVisualManager::OnDrawHeaderCtrlSortArrow(CMFCHeaderCtrl*, CDC* pDC, CRect& rect, BOOL bIsAscending) { DrawArrowGlyph(pDC, rect, !bIsAscending, ::GetSysColor(COLOR_BTNTEXT)); }
void CMFCVisualManager::OnDrawMenuArrowOnCustomizeList(CDC* pDC, CRect rect, BOOL bSelected) { DrawArrowGlyph(pDC, rect, true, bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_MENUTEXT)); }
void CMFCVisualManager::OnDrawMenuCheck(CDC* pDC, CMFCToolBarMenuButton*, CRect rect, BOOL bHighlight, BOOL bIsRadio) {
    FillAndFrame(pDC, rect, bHighlight ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_MENU), g_visualBorderColor);
    if (bIsRadio) {
        HDC hdc = SafeHdc(pDC);
        if (hdc) {
            COLORREF markColor = ::GetSysColor(COLOR_MENUTEXT);
            HBRUSH brush = ::CreateSolidBrush(markColor);
            HPEN pen = ::CreatePen(PS_SOLID, 1, markColor);
            HGDIOBJ oldBrush = brush ? ::SelectObject(hdc, brush) : nullptr;
            HGDIOBJ oldPen = pen ? ::SelectObject(hdc, pen) : nullptr;
            ::Ellipse(hdc, rect.left + 4, rect.top + 4, rect.right - 4, rect.bottom - 4);
            if (oldPen) ::SelectObject(hdc, oldPen);
            if (oldBrush) ::SelectObject(hdc, oldBrush);
            if (pen) ::DeleteObject(pen);
            if (brush) ::DeleteObject(brush);
        }
    } else {
        OnDrawCheckBoxEx(pDC, rect, 1, bHighlight, FALSE, TRUE);
    }
}
void CMFCVisualManager::OnDrawMenuItemButton(CDC* pDC, CMFCToolBarMenuButton*, CRect rectButton, BOOL bHighlight, BOOL bDisabled) {
    FillAndFrame(pDC, rectButton, bHighlight ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_MENU),
                 bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : g_visualBorderColor);
}
void CMFCVisualManager::OnDrawMenuResizeBar(CDC* pDC, CRect rect, int) {
    FillSolid(pDC, rect, ::GetSysColor(COLOR_MENU));
    for (int i = 0; i < 3; ++i) DrawLine(pDC, rect.right - 4 - i * 4, rect.bottom - 2, rect.right - 2, rect.bottom - 4 - i * 4, ::GetSysColor(COLOR_3DSHADOW));
}
void CMFCVisualManager::OnDrawMenuScrollButton(CDC* pDC, CRect rect, BOOL bIsScrollDown, BOOL bIsHighlited, BOOL bIsPressed, BOOL bIsDisabled) {
    FillAndFrame(pDC, rect, bIsPressed ? ButtonFillForState(ButtonsIsPressed) : (bIsHighlited ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_MENU)), g_visualBorderColor);
    DrawArrowGlyph(pDC, rect, bIsScrollDown, bIsDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_MENUTEXT));
}
void CMFCVisualManager::OnDrawMenuShadow(CDC* pDC, const CRect& rectClient, const CRect& rectExclude, int nDepth, int, int, CBitmap*, CBitmap*, COLORREF color) {
    CRect right(rectClient.right, rectClient.top + nDepth, rectClient.right + nDepth, rectClient.bottom + nDepth);
    CRect bottom(rectClient.left + nDepth, rectClient.bottom, rectClient.right + nDepth, rectClient.bottom + nDepth);
    FillSolid(pDC, right, color);
    FillSolid(pDC, bottom, color);
    if (IsDrawableRect(rectExclude)) FillSolid(pDC, rectExclude, ::GetSysColor(COLOR_MENU));
}
void CMFCVisualManager::OnDrawMenuSystemButton(CDC* pDC, CRect rect, UINT, UINT, BOOL bHighlight) {
    FillAndFrame(pDC, rect, bHighlight ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_MENU), g_visualBorderColor);
}
void CMFCVisualManager::OnDrawMiniFrameBorder(CDC* pDC, CPaneFrameWnd*, CRect rectBorder, CRect) { FrameSolid(pDC, rectBorder, g_visualBorderColor); }
void CMFCVisualManager::OnDrawOutlookBarSplitter(CDC* pDC, CRect rect) { FillSolid(pDC, rect, ::GetSysColor(COLOR_3DSHADOW)); }
void CMFCVisualManager::OnDrawOutlookPageButtonBorder(CDC* pDC, CRect& rect, BOOL bIsHighlighted, BOOL bIsPressed) {
    FillAndFrame(pDC, rect, bIsPressed ? ButtonFillForState(ButtonsIsPressed) : (bIsHighlighted ? ButtonFillForState(ButtonsIsHighlighted) : g_visualBackgroundColor), g_visualBorderColor);
}
void CMFCVisualManager::OnDrawPaneBorder(CDC* pDC, CBasePane*, CRect& rect) { FrameSolid(pDC, rect, g_visualBorderColor); }
unsigned long CMFCVisualManager::OnDrawPaneCaption(CDC* pDC, CDockablePane*, int active, CRect rect, int) {
    COLORREF fill = active ? g_visualAccentColor : ::GetSysColor(COLOR_INACTIVECAPTION);
    FillAndFrame(pDC, rect, fill, g_visualBorderColor);
    return active ? ::GetSysColor(COLOR_CAPTIONTEXT) : ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
}
void CMFCVisualManager::OnDrawPaneDivider(CDC* pDC, CPaneDivider*, CRect rect, BOOL) { FillSolid(pDC, rect, ::GetSysColor(COLOR_3DFACE)); FrameSolid(pDC, rect, ::GetSysColor(COLOR_3DSHADOW)); }
void CMFCVisualManager::OnDrawPopupWindowBorder(CDC* pDC, CRect rect) { FrameSolid(pDC, rect, g_visualBorderColor); }
void CMFCVisualManager::OnDrawPopupWindowButtonBorder(CDC* pDC, CRect rectClient, CMFCDesktopAlertWndButton*) { FillAndFrame(pDC, rectClient, ButtonFillForState(ButtonsIsHighlighted), g_visualBorderColor); }
COLORREF CMFCVisualManager::OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd*) { FillSolid(pDC, rectCaption, g_visualAccentColor); return ::GetSysColor(COLOR_CAPTIONTEXT); }
void CMFCVisualManager::OnDrawRibbonApplicationButton(CDC* pDC, CMFCRibbonButton*) { FillAndFrame(pDC, CRect(0, 0, 28, 28), g_visualAccentColor, ::GetSysColor(COLOR_3DDKSHADOW)); }
void CMFCVisualManager::OnDrawRibbonButtonBorder(CDC* pDC, CMFCRibbonButton*) { FillAndFrame(pDC, CRect(0, 0, 24, 22), ButtonFillForState(ButtonsIsHighlighted), g_visualBorderColor); }
unsigned long CMFCVisualManager::OnDrawRibbonButtonsGroup(CDC* pDC, CMFCRibbonButtonsGroup*, CRect rect) { FillAndFrame(pDC, rect, ::GetSysColor(COLOR_3DFACE), g_visualBorderColor); return ::GetSysColor(COLOR_BTNTEXT); }
void CMFCVisualManager::OnDrawRibbonCaption(CDC* pDC, CMFCRibbonBar*, CRect rectCaption, CRect rectText) {
    FillSolid(pDC, rectCaption, g_visualAccentColor);
    DrawRectText(pDC, rectText, L"", ::GetSysColor(COLOR_CAPTIONTEXT));
}
void CMFCVisualManager::OnDrawRibbonCaptionButton(CDC* pDC, CMFCRibbonCaptionButton*) { OnDrawCaptionButton(pDC, nullptr, TRUE, FALSE, FALSE); }
void CMFCVisualManager::OnDrawRibbonCategory(CDC* pDC, CMFCRibbonCategory*, CRect rect) { FillSolid(pDC, rect, g_visualBackgroundColor); }
unsigned long CMFCVisualManager::OnDrawRibbonCategoryCaption(CDC* pDC, CMFCRibbonContextCaption*) { FillSolid(pDC, CRect(0, 0, 120, 18), g_visualAccentColor); return ::GetSysColor(COLOR_CAPTIONTEXT); }
void CMFCVisualManager::OnDrawRibbonCategoryScroll(CDC* pDC, CMFCRibbonCategoryScroll*) { FillAndFrame(pDC, CRect(0, 0, 18, 18), ::GetSysColor(COLOR_BTNFACE), g_visualBorderColor); }
COLORREF CMFCVisualManager::OnDrawRibbonCategoryTab(CDC* pDC, CMFCRibbonTab*, BOOL bIsActive) { FillAndFrame(pDC, CRect(0, 0, 80, 24), bIsActive ? ::GetSysColor(COLOR_WINDOW) : g_visualBackgroundColor, g_visualBorderColor); return ::GetSysColor(COLOR_BTNTEXT); }
void CMFCVisualManager::OnDrawRibbonCheckBoxOnList(CDC* pDC, CMFCRibbonCheckBox*, CRect rect, BOOL bIsSelected, BOOL bHighlighted) { OnDrawCheckBoxEx(pDC, rect, bIsSelected, bHighlighted, FALSE, TRUE); }
void CMFCVisualManager::OnDrawRibbonDefaultPaneButton(CDC* pDC, CMFCRibbonButton*) { FillAndFrame(pDC, CRect(0, 0, 80, 22), ::GetSysColor(COLOR_BTNFACE), g_visualBorderColor); }
void CMFCVisualManager::OnDrawRibbonDefaultPaneButtonContext(CDC* pDC, CMFCRibbonButton*) { OnDrawRibbonDefaultPaneButton(pDC, nullptr); }
void CMFCVisualManager::OnDrawRibbonDesign(CDC* pDC, CMFCRibbonBar*, CRect rect) { FillSolid(pDC, rect, g_visualBackgroundColor); }
void CMFCVisualManager::OnDrawRibbonEdit(CDC* pDC, CMFCRibbonEdit*, CRect rect, BOOL bIsHighlighted, BOOL, BOOL bIsDisabled) {
    FillAndFrame(pDC, rect, bIsDisabled ? ::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_WINDOW), bIsHighlighted ? g_visualAccentColor : g_visualBorderColor);
}
void CMFCVisualManager::OnDrawRibbonGalleryBorder(CDC* pDC, CMFCRibbonGallery*, CRect rect) { FrameSolid(pDC, rect, g_visualBorderColor); }
void CMFCVisualManager::OnDrawRibbonLabel(CDC* pDC, CMFCRibbonLabel*, CRect rect) {
    FillSolid(pDC, rect, g_visualBackgroundColor);
    DrawLine(pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, ::GetSysColor(COLOR_3DLIGHT));
}
void CMFCVisualManager::OnDrawRibbonMainPanelButtonBorder(CDC* pDC, CMFCRibbonButton*) { FillAndFrame(pDC, CRect(0, 0, 120, 24), ButtonFillForState(ButtonsIsHighlighted), g_visualBorderColor); }
void CMFCVisualManager::OnDrawRibbonMainPanelFrame(CDC* pDC, CMFCRibbonMainPanel*, CRect rect) { FillAndFrame(pDC, rect, ::GetSysColor(COLOR_MENU), g_visualBorderColor); }
COLORREF CMFCVisualManager::OnDrawRibbonPanel(CDC* pDC, CMFCRibbonPanel*, CRect rectPanel, CRect rectCaption) { FillAndFrame(pDC, rectPanel, ::GetSysColor(COLOR_3DFACE), g_visualBorderColor); if (IsDrawableRect(rectCaption)) FillSolid(pDC, rectCaption, ::GetSysColor(COLOR_3DLIGHT)); return ::GetSysColor(COLOR_BTNTEXT); }
void CMFCVisualManager::OnDrawRibbonPanelCaption(CDC* pDC, CMFCRibbonPanel*, CRect rectCaption) { FillSolid(pDC, rectCaption, ::GetSysColor(COLOR_3DLIGHT)); }
void CMFCVisualManager::OnDrawRibbonProgressBar(CDC* pDC, CMFCRibbonProgressBar*, CRect rectProgress, CRect rectChunk, BOOL bInfiniteMode) {
    FillAndFrame(pDC, rectProgress, ::GetSysColor(COLOR_WINDOW), g_visualBorderColor);
    FillSolid(pDC, bInfiniteMode ? rectProgress : rectChunk, g_visualAccentColor);
}
void CMFCVisualManager::OnDrawRibbonQATSeparator(CDC* pDC, CMFCRibbonSeparator*, CRect rect) { OnDrawSeparator(pDC, nullptr, rect, TRUE); }
void CMFCVisualManager::OnDrawRibbonQuickAccessToolBarSeparator(CDC* pDC, CMFCRibbonSeparator*, CRect rect) { OnDrawSeparator(pDC, nullptr, rect, TRUE); }
void CMFCVisualManager::OnDrawRibbonSliderChannel(CDC* pDC, CMFCRibbonSlider*, CRect rect) { FillAndFrame(pDC, rect, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_3DSHADOW)); }
void CMFCVisualManager::OnDrawRibbonSliderThumb(CDC* pDC, CMFCRibbonSlider*, CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled) {
    FillAndFrame(pDC, rect, bIsPressed ? ButtonFillForState(ButtonsIsPressed) : (bIsHighlighted ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE)), bIsDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : g_visualBorderColor);
}
void CMFCVisualManager::OnDrawRibbonSliderZoomButton(CDC* pDC, CMFCRibbonSlider*, CRect rect, BOOL bIsZoomOut, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled) {
    OnDrawRibbonSliderThumb(pDC, nullptr, rect, bIsHighlighted, bIsPressed, bIsDisabled);
    DrawPlusMinus(pDC, CRect(rect.left + 4, rect.top + 4, rect.right - 4, rect.bottom - 4), bIsZoomOut, ::GetSysColor(COLOR_BTNTEXT));
}
COLORREF CMFCVisualManager::OnDrawRibbonStatusBarPane(CDC* pDC, CMFCRibbonStatusBar*, CMFCRibbonStatusBarPane*) { FillAndFrame(pDC, CRect(0, 0, 80, 22), ::GetSysColor(COLOR_3DFACE), g_visualBorderColor); return ::GetSysColor(COLOR_BTNTEXT); }
COLORREF CMFCVisualManager::OnDrawRibbonTabsFrame(CDC* pDC, CMFCRibbonBar*, CRect rectTab) { FillAndFrame(pDC, rectTab, g_visualBackgroundColor, g_visualBorderColor); return ::GetSysColor(COLOR_BTNTEXT); }
void CMFCVisualManager::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int, int iImage, BOOL bHilited) { FillAndFrame(pDC, rect, bHilited ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE), g_visualBorderColor); DrawArrowGlyph(pDC, rect, iImage != 0, ::GetSysColor(COLOR_BTNTEXT)); }
void CMFCVisualManager::OnDrawSeparator(CDC* pDC, CBasePane*, CRect rect, BOOL bHorz) { OnDrawButtonSeparator(pDC, nullptr, rect, bHorz); }
void CMFCVisualManager::OnDrawShowAllMenuItems(CDC* pDC, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) { FillAndFrame(pDC, rect, ButtonFillForState(state), g_visualBorderColor); DrawArrowGlyph(pDC, rect, true, ::GetSysColor(COLOR_BTNTEXT)); }
void CMFCVisualManager::OnDrawSpinButtons(CDC* pDC, CRect rect, int nState, BOOL, BOOL bIsHovered) { FillAndFrame(pDC, rect, bIsHovered ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE), g_visualBorderColor); DrawArrowGlyph(pDC, rect, nState != 0, ::GetSysColor(COLOR_BTNTEXT)); }
void CMFCVisualManager::OnDrawStatusBarPaneBorder(CDC* pDC, CMFCStatusBar*, CRect rectPane, UINT, UINT) { Draw3dFrame(pDC, rectPane, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT)); }
void CMFCVisualManager::OnDrawStatusBarProgress(CDC* pDC, CMFCStatusBar*, CRect rectProgress, int nProgressTotal, int nProgressCurr, COLORREF clrBar, COLORREF, COLORREF, BOOL) {
    FillAndFrame(pDC, rectProgress, ::GetSysColor(COLOR_WINDOW), g_visualBorderColor);
    if (nProgressTotal > 0) {
        int innerWidth = std::max(0, rectProgress.Width() - 2);
        int width = static_cast<int>(std::max(0LL, std::min<long long>(innerWidth,
            (static_cast<long long>(innerWidth) * nProgressCurr) / nProgressTotal)));
        COLORREF bar = (clrBar != static_cast<COLORREF>(-1)) ? clrBar : g_visualAccentColor;
        FillSolid(pDC, CRect(rectProgress.left + 1, rectProgress.top + 1, rectProgress.left + 1 + width, rectProgress.bottom - 1), bar);
    }
}
void CMFCVisualManager::OnDrawStatusBarSizeBox(CDC* pDC, CMFCStatusBar*, CRect rect) { OnDrawMenuResizeBar(pDC, rect, 0); }
void CMFCVisualManager::OnDrawTab(CDC* pDC, CRect rect, int, BOOL bIsActive, const CMFCBaseTabCtrl*) { FillAndFrame(pDC, rect, bIsActive ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_3DFACE), g_visualBorderColor); }
void CMFCVisualManager::OnDrawTabButton(CDC* pDC, CRect rect, const CMFCBaseTabCtrl*, int, BOOL bIsHilited, BOOL bIsPressed) { FillAndFrame(pDC, rect, bIsPressed ? ButtonFillForState(ButtonsIsPressed) : (bIsHilited ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE)), g_visualBorderColor); }
void CMFCVisualManager::OnDrawTabCloseButton(CDC* pDC, CRect rect, const CMFCBaseTabCtrl*, BOOL bIsHilited, BOOL bIsPressed, BOOL bIsDisabled) { OnDrawTabButton(pDC, rect, nullptr, 0, bIsHilited, bIsPressed); DrawRectText(pDC, rect, L"x", bIsDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_BTNTEXT)); }
void CMFCVisualManager::OnDrawTabContent(CDC* pDC, CRect rect, int, BOOL, const CMFCBaseTabCtrl*, COLORREF) { FillSolid(pDC, rect, ::GetSysColor(COLOR_WINDOW)); }
void CMFCVisualManager::OnDrawTabsButtonBorder(CDC* pDC, CRect& rect, CMFCButton*, UINT uiState, CMFCBaseTabCtrl*) { FillAndFrame(pDC, rect, uiState ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_BTNFACE), g_visualBorderColor); }
void CMFCVisualManager::OnDrawTask(CDC* pDC, CMFCTasksPaneTask*, CImageList*, BOOL bIsHighlighted, BOOL bIsSelected) { FillAndFrame(pDC, CRect(0, 0, 120, 22), bIsSelected ? ButtonFillForState(ButtonsIsPressed) : (bIsHighlighted ? ButtonFillForState(ButtonsIsHighlighted) : ::GetSysColor(COLOR_WINDOW)), g_visualBorderColor); }
void CMFCVisualManager::OnDrawTasksGroupCaption(CDC* pDC, CMFCTasksPaneTaskGroup*, BOOL bIsHighlighted, BOOL bIsSelected, BOOL) { FillAndFrame(pDC, CRect(0, 0, 160, 24), bIsSelected ? ButtonFillForState(ButtonsIsPressed) : (bIsHighlighted ? ButtonFillForState(ButtonsIsHighlighted) : g_visualAccentColor), g_visualBorderColor); }
void CMFCVisualManager::OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL, BOOL) { FrameSolid(pDC, rect, g_visualBorderColor); }
void CMFCVisualManager::OnDrawTearOffCaption(CDC* pDC, CRect rect, BOOL bIsActive) { FillSolid(pDC, rect, bIsActive ? g_visualAccentColor : ::GetSysColor(COLOR_INACTIVEBORDER)); }
void CMFCVisualManager::OnDrawToolBoxFrame(CDC* pDC, const CRect& rect) { FrameSolid(pDC, rect, g_visualBorderColor); }
COLORREF CMFCVisualManager::GetToolbarDisabledTextColor() {
    return GetHighlightedColor(COLOR_GRAYTEXT);
}
COLORREF CMFCVisualManager::GetToolbarHighlightColor() {
    return GetHighlightedColor(COLOR_HIGHLIGHT);
}
int CMFCVisualManager::GetShowAllMenuItemsHeight(CDC*, const CSize& sizeDefault) {
    return sizeDefault.cy > 0 ? sizeDefault.cy : 16;
}
COLORREF CMFCVisualManager::GetHighlightedColor(UINT nColorIndex) const {
    COLORREF clr = ::GetSysColor(static_cast<int>(nColorIndex));
    if (clr == 0) {
        clr = RGB(0, 120, 215);
    }
    return clr;
}
COLORREF CMFCVisualManager::GetThemeColor(COLORREF clrBase, int nIntensity) {
    auto clampByte = [](int v) -> BYTE {
        if (v < 0) return 0;
        if (v > 255) return 255;
        return static_cast<BYTE>(v);
    };
    const int delta = (nIntensity * 255) / 100;
    return RGB(
        clampByte(static_cast<int>(GetRValue(clrBase)) + delta),
        clampByte(static_cast<int>(GetGValue(clrBase)) + delta),
        clampByte(static_cast<int>(GetBValue(clrBase)) + delta));
}
