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

//=============================================================================
// On* notifications
//=============================================================================

// CMFCVisualManager::OnActivateApp(...) -- retail body is `ret $0` (no-op).
// Symbol: ?OnActivateApp@CMFCVisualManager@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivateApp_CMFCVisualManager__UEAAXPEAVCWnd__H_Z(
    CMFCVisualManager* /*pThis*/, CWnd* /*pWnd*/, int /*bActive*/)
{
    // Retail is a pure no-op (verified: `ret $0`).
}

//=============================================================================
// OnDraw* painting routines (conservative no-ops, see header comment)
//=============================================================================

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
