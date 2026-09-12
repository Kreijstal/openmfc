// CMFCVisualManager — OpenMFC implementation.
// Sources: global_mfc-feature-56_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"
// CMFCCaptionBar is only forward-declared in afxmfc.h; its retail layout is
// mirrored by openmfc::detail::cmfccaptionbar::CB (m_bDrawBackground at 0x404
// is the flag retail's caption-bar bodies and the two caption-bar visual-manager
// bodies below test -- retail MFC calls it m_bIsMessageBarMode).
#include "detail/CMFCCaptionBarSupport.h"

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

// Sibling impl__ thunks used by the bodies below (each definition was located
// in the tree before being declared here; see the comment at each call site).
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                 // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ(); // toolbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();     // toolbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();           // docking/CPane.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ();      // controls/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ(); // outlookbar/RuntimeClasses.cpp
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);              // docking/CPaneFrameWnd.cpp
extern "C" const CObList* MS_ABI impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ();       // toolbar/CMFCToolBar.cpp
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(
    const CObList* pThis, long long nIndex);                                                    // core/collections/CObList.cpp
// (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z comes from detail/CMFCCaptionBarSupport.h)
extern "C" void MS_ABI impl__OnChangeVisualManager_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);    // toolbar/CMFCToolBar.cpp
extern "C" void MS_ABI impl__RedrawAll_CPaneFrameWnd__SAXXZ();                                  // docking/CPaneFrameWnd.cpp
extern "C" CWnd* MS_ABI impl__AfxGetMainWnd__YAPEAVCWnd__XZ();                                  // detail/CWinAppSupport.cpp
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;                                // toolbar/StaticData.cpp

namespace {

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h), the node a
// CObList::POSITION points at.  CObList::GetHeadPosition / GetNext are inline in
// retail MFC, so they have no export and no impl__ thunk; the exported FindIndex
// thunk plus this read-only view is how a CObList is walked from another
// translation unit (the same pattern docking/CDockingPanesRow.cpp uses).
struct VmObNode {
    VmObNode* pNext;
    VmObNode* pPrev;
    CObject*  data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

inline VmObNode* VmNodeFromPos(CObList::POSITION pos)
{
    VmObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}

// Walks OpenMFC's GetAllToolbars list and hands every toolbar that
// IsKindOf(pClass) and whose HWND still maps to a permanent CWnd -- the filter
// both AdjustToolbars and RedrawAll apply to retail's gAllToolbars list -- to
// `fn`.  The matching bars are snapshotted first, because OpenMFC's
// GetAllToolbars rebuilds its CObList on every call (see
// toolbar/CMFCToolBar.cpp), so a node pointer must not be held across the
// per-toolbar callbacks.  (malloc rather than std::vector: the latter drags in
// libstdc++ throw helpers this DLL does not link.)
void VmForEachLiveToolBar(CRuntimeClass* pClass, void (*fn)(CMFCToolBar*))
{
    const CObList* pList = impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ();
    if (pList == nullptr) return;
    size_t n = 0;
    for (VmObNode* p = VmNodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0));
         p != nullptr; p = p->pNext) {
        ++n;
    }
    if (n == 0) return;
    CMFCToolBar** bars = static_cast<CMFCToolBar**>(std::malloc(n * sizeof(CMFCToolBar*)));
    if (bars == nullptr) return;
    size_t count = 0;
    for (VmObNode* p = VmNodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0));
         p != nullptr && count < n; p = p->pNext) {
        CObject* pObj = p->data;
        if (pObj == nullptr) continue;
        if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, pClass)) continue;
        CMFCToolBar* pBar = static_cast<CMFCToolBar*>(pObj);
        if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pBar->m_hWnd) == nullptr) continue;
        bars[count++] = pBar;
    }
    for (size_t i = 0; i < count; ++i) fn(bars[i]);
    std::free(bars);
}

} // namespace


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
// CMFCVisualManager::OnActivateApp(CWnd*, BOOL) -- faithful transcription: the
// retail body is a bare `ret` at RVA 0x27d0 (mfc140u), an ICF-folded empty
// virtual shared with OnFillTasksGroupInterior and OnUpdateSystemColors (the
// name that survives at that RVA in the symbol map is OnDrawRibbonLabel).
// Symbol: ?OnActivateApp@CMFCVisualManager@@UEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivateApp_CMFCVisualManager__UEAAXPEAVCWnd__H_Z(
    CMFCVisualManager* /*pThis*/, CWnd* /*pWnd*/, int /*bActive*/)
{
    // Retail is a pure no-op (RVA 0x27d0 is a bare `ret`).
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
// CMFCVisualManager::AdjustFrames() -- retail body (RVA 0x184420, mfc140u)
// walks CFrameImpl::m_lstFrames (the CList at 0x3b1cc0; its head node pointer
// is the qword at 0x3b1cc8, nodes are +0x00 next / +0x10 element) and, for
// every frame whose HWND still maps to a permanent CWnd
// (?FromHandlePermanent@CWnd@@SAPEAV1@PEAUHWND__@@@Z, RVA 0x28adc0), issues
// ::SendMessage(pFrame->m_hWnd, AFX_WM_CHANGEVISUALMANAGER, 0, 0) -- the
// message id is the exported UINT at 0x3c2508, ?AFX_WM_CHANGEVISUALMANAGER@@3IA.
// OpenMFC does register that message (impl__AFX_WM_CHANGEVISUALMANAGER__3IA in
// core/runtime/Globals.cpp) and does keep the frame list (AddFrame / RemoveFrame
// in core/frame/CFrameImpl.cpp), but the live list is the file-local
// g_pFrameListHead inside that translation unit's anonymous namespace and the
// exported ?m_lstFrames@CFrameImpl@@ storage is a zero blob, so there is no way
// to enumerate the frames from here.
// TODO(clean-room): not transcribed -- CFrameImpl.cpp exposes no accessor for
// its frame list; once it does, the walk above is a five-line body.
// Symbol: ?AdjustFrames@CMFCVisualManager@@SAXXZ
extern "C" void MS_ABI impl__AdjustFrames_CMFCVisualManager__SAXXZ() {
}
// CMFCVisualManager::AdjustToolbars() -- transcription of the retail body
// (RVA 0x1843b0, mfc140u):
//     for (node = gAllToolbars.head /* 0x3b2098 */; node; node = node->next) {
//         CObject* p = node->data;                              // +0x10
//         if (p == NULL) continue;
//         if (!p->IsKindOf(RUNTIME_CLASS(CMFCToolBar)))          // 0x234cf0 vs 0x3b15f8
//             continue;
//         if (CWnd::FromHandlePermanent(p->m_hWnd) == NULL)      // 0x28adc0, m_hWnd @ +0x40
//             continue;
//         p->vftable[+0x778]();   // CMFCToolBar::OnChangeVisualManager (0x15b6d0)
//     }
// 0x3b2098 is the head pointer of the CObList at 0x3b2090 that
// ?GetAllToolbars@CMFCToolBar@@SAAEBVCObList@@XZ (RVA 0x159610) returns, 0x3b15f8
// is the CRuntimeClass ?GetThisClass@CMFCToolBar@@ hands out, and slot +0x778 of
// the CMFCToolBar vftable (0x3157c8) holds the OnChangeVisualManager export.
// OpenMFC's list comes from the impl__GetAllToolbars thunk (rebuilt from the
// toolbar side table on each call, see toolbar/CMFCToolBar.cpp).
// Deviation: retail dispatches OnChangeVisualManager virtually; OpenMFC's
// CMFCToolBar has no matching vtable slot, so the base thunk is called directly
// and a derived toolbar's override is not reached.
// Symbol: ?AdjustToolbars@CMFCVisualManager@@SAXXZ
extern "C" void MS_ABI impl__AdjustToolbars_CMFCVisualManager__SAXXZ() {
    VmForEachLiveToolBar(impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ(),
                         impl__OnChangeVisualManager_CMFCToolBar__UEAAXXZ);
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
//=============================================================================
// wave: base-class defaults decoded from retail mfc140u.dll (14.51.36231, x64)
//
// Every body below was transcribed from the retail disassembly.  The retail
// bodies read their colors out of the global `?afxGlobalData@@3UAFX_GLOBAL_DATA@@A`
// object, which lives at RVA 0x3c1620 (mfc140u); each read is preceded by the
// inlined "if (!afxGlobalData.<+0x00>) { afxGlobalData.Initialize(); ... }"
// guard, which is pure lazy-init bookkeeping and is not modeled here.
//
// The color fields themselves are nothing but cached ::GetSysColor() results:
// AFX_GLOBAL_DATA's color initializer (RVA 0x6b1c0, called from
// ?Initialize@AFX_GLOBAL_DATA@@QEAAXXZ at 0x6a790) fills them like this --
// this table was read straight off that function and is the authority used by
// the implementations below:
//
//   +0x28 = GetSysColor(COLOR_BTNFACE)        +0x60 = GetSysColor(COLOR_BTNFACE)
//   +0x2c = GetSysColor(COLOR_BTNSHADOW)      +0x64 = GetSysColor(COLOR_BTNSHADOW)
//   +0x30 = GetSysColor(COLOR_BTNHIGHLIGHT)   +0x68 = GetSysColor(COLOR_BTNHIGHLIGHT)
//   +0x34 = GetSysColor(COLOR_BTNTEXT)        +0x6c = GetSysColor(COLOR_3DDKSHADOW)
//   +0x38 = GetSysColor(COLOR_WINDOWFRAME)    +0x70 = GetSysColor(COLOR_3DLIGHT)
//   +0x3c = GetSysColor(COLOR_3DDKSHADOW)     +0x74 = GetSysColor(COLOR_BTNTEXT)
//   +0x40 = GetSysColor(COLOR_3DLIGHT)        +0x78 = GetSysColor(COLOR_WINDOW)
//   +0x44 = GetSysColor(COLOR_GRAYTEXT)       +0x7c = GetSysColor(COLOR_WINDOWTEXT)
//   +0x48 = GetSysColor(COLOR_HIGHLIGHT)      +0x80 = GetSysColor(COLOR_CAPTIONTEXT)
//   +0x4c = GetSysColor(COLOR_HIGHLIGHTTEXT)  +0x84 = GetSysColor(COLOR_MENUTEXT)
//   +0x50 = GetSysColor(COLOR_HOTLIGHT)       +0x88 = GetSysColor(COLOR_ACTIVECAPTION)
//   +0x54 = 0x00ff0000 (blue)                 +0x8c = GetSysColor(COLOR_INACTIVECAPTION)
//   +0x58 = 0x00800080 (purple)               +0x90 = GetSysColor(COLOR_INACTIVECAPTIONTEXT)
//   +0x5c = GetSysColor(COLOR_WINDOW)         +0x94 = GetSysColor(COLOR_GRADIENTACTIVECAPTION)
//                                             +0x98 = GetSysColor(COLOR_GRADIENTINACTIVECAPTION)
//   (+0x50/+0x54/+0x58 all collapse to GetSysColor(COLOR_WINDOWTEXT) when the
//    black-high-contrast flag at +0x260 is set)
//   +0x260 = (GetSysColor(COLOR_3DLIGHT)==0xffffff && GetSysColor(COLOR_BTNFACE)==0)
//   +0x264 = (GetSysColor(COLOR_3DDKSHADOW)==0 && GetSysColor(COLOR_BTNFACE)==0xffffff)
//   +0x288 = GetDeviceCaps(screen DC, BITSPIXEL)
//
// Cached solid brushes (CBrush objects; the HBRUSH sits at +8 inside each):
//   brush at +0xa8 (handle +0xb0) = CreateSolidBrush(field +0x28) -> COLOR_BTNFACE
//   brush at +0xb8 (handle +0xc0) = CreateSolidBrush(field +0x48) -> COLOR_HIGHLIGHT
//   brush at +0x108 (handle +0x110) = CreateSolidBrush(field +0x78) -> COLOR_WINDOW
//   brush at +0x118 (handle +0x120) = CreateSolidBrush(field +0x60) -> COLOR_BTNFACE
//
// NOTE on the older offset table at the top of this file: those "0x16xx"
// numbers are the low 16 bits of the ABSOLUTE addresses (afxGlobalData base
// 0x1803c1620 + the offsets above), not member offsets -- 0x1648 is +0x28,
// 0x1694 is +0x74, and so on.  Two of its color names disagree with the
// initializer decoded above: 0x1654 (+0x34) is GetSysColor(COLOR_BTNTEXT), not
// COLOR_MENUTEXT, and 0x1674 (+0x54) is the constant 0x00ff0000, not
// COLOR_HOTLIGHT.  Bodies written before this batch were not touched.
//=============================================================================

namespace {

// Retail draws through ::FillRect with the cached afxGlobalData brushes and
// through CDC/CPen; OpenMFC models neither afxGlobalData nor the in-DLL CDC
// method symbols, so these file-local helpers go straight to GDI with the
// equivalent handles.
inline HDC VmHdc(CDC* pDC) { return pDC != nullptr ? pDC->GetSafeHdc() : nullptr; }

inline RECT VmRect(const CRect& rect)
{
    return RECT{ rect.left, rect.top, rect.right, rect.bottom };
}

// ::FillRect(pDC->m_hDC, rect, <cached solid brush of ::GetSysColor(nSysColor)>)
void VmFillSysColor(CDC* pDC, const CRect& rect, int nSysColor)
{
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return;
    RECT r = VmRect(rect);
    HBRUSH hbr = ::CreateSolidBrush(::GetSysColor(nSysColor));
    if (hbr != nullptr) {
        ::FillRect(hdc, &r, hbr);
        ::DeleteObject(hbr);
    }
}

// CDC::MoveTo/LineTo with a 1px PS_SOLID pen of `clr` (LineTo excludes the
// end point, exactly as the retail sequence does).
void VmLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF clr)
{
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return;
    HPEN hpen = ::CreatePen(PS_SOLID, 1, clr);
    if (hpen == nullptr) return;
    HGDIOBJ hOld = ::SelectObject(hdc, hpen);
    ::MoveToEx(hdc, x1, y1, nullptr);
    ::LineTo(hdc, x2, y2);
    ::SelectObject(hdc, hOld);
    ::DeleteObject(hpen);
}

// afxGlobalData +0x260 / +0x264, recomputed live (retail caches them).
bool VmIsBlackHighContrast()
{
    return ::GetSysColor(COLOR_3DLIGHT) == 0x00ffffff && ::GetSysColor(COLOR_BTNFACE) == 0;
}
bool VmIsWhiteHighContrast()
{
    return ::GetSysColor(COLOR_3DDKSHADOW) == 0 && ::GetSysColor(COLOR_BTNFACE) == 0x00ffffff;
}

// afxGlobalData +0x288: GetDeviceCaps(BITSPIXEL) of a screen DC.
int VmScreenBitsPerPixel()
{
    HDC hdc = ::GetDC(nullptr);
    if (hdc == nullptr) return 32;
    int bpp = ::GetDeviceCaps(hdc, BITSPIXEL);
    ::ReleaseDC(nullptr, hdc);
    return bpp;
}

} // namespace

// CMFCVisualManager::DoDrawHeaderSortArrow(CDC*, CRect, int, int) -- faithful
// transcription of the retail body at RVA 0x18b530 (mfc140u).  Retail builds
// two 1px PS_SOLID CPens and walks a three-segment triangle:
//   light pen = afxGlobalData +0x30 (p3 != 0) or +0x68 (p3 == 0); both are
//               ::GetSysColor(COLOR_BTNHIGHLIGHT)
//   dark pen  = afxGlobalData +0x3c (p3 != 0) or +0x6c (p3 == 0); both are
//               ::GetSysColor(COLOR_3DDKSHADOW)
// p2 != 0 (up):   MoveTo(left,bottom) -light-> (right,bottom) -light-> (mid,top)
//                 -dark-> (left,bottom)
// p2 == 0 (down): MoveTo(right,top) -light-> (mid,bottom) -dark-> (left,top)
//                 -dark-> (right,top)
// where mid == (left + right) / 2 truncated toward zero.  The p3 palette pair
// only picks between two fields that hold the same system color, so the two
// palettes are indistinguishable here.
// Symbol: ?DoDrawHeaderSortArrow@CMFCVisualManager@@QEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__DoDrawHeaderSortArrow_CMFCVisualManager__QEAAXPEAVCDC__VCRect__HH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bIsUp, int /*bUseBtnPalette*/)
{
    if (!pThis || !pDC) return;
    const COLORREF clrLight = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    const COLORREF clrDark = ::GetSysColor(COLOR_3DDKSHADOW);
    const int mid = (rect.left + rect.right) / 2;
    if (bIsUp) {
        VmLine(pDC, rect.left, rect.bottom, rect.right, rect.bottom, clrLight);
        VmLine(pDC, rect.right, rect.bottom, mid, rect.top, clrLight);
        VmLine(pDC, mid, rect.top, rect.left, rect.bottom, clrDark);
    } else {
        VmLine(pDC, rect.right, rect.top, mid, rect.bottom, clrLight);
        VmLine(pDC, mid, rect.bottom, rect.left, rect.top, clrDark);
        VmLine(pDC, rect.left, rect.top, rect.right, rect.top, clrDark);
    }
}

// CMFCVisualManager::DrawTextOnGlass(CDC*, CString, CRect, DWORD, int, COLORREF)
// -- retail (RVA 0x18f1d0, mfc140u) saves ::GetTextColor(pDC->m_hAttribDC),
// calls the virtual CDC::SetTextColor(0), forwards to
// ?DrawTextOnGlass@AFX_GLOBAL_DATA@@QEAAH... (RVA 0x6c500) passing the theme
// handle cached at CMFCVisualManager+0x20 with iPartId/iStateId = 0, then
// restores the saved text color and returns that call's result.
// CMFCVisualManager+0x20 is not modeled here (OpenMFC's CMFCVisualManager
// carries only _visualmanager_padding), so the NULL-theme terminal of
// AFX_GLOBAL_DATA::DrawTextOnGlass (its entry test at 0x6c536 / the
// IsDwmCompositionEnabled test at 0x6c53f, both landing on 0x6c711) is what is
// transcribed: CDC::DrawText(str, str.GetLength(), rect, dwFlags) followed by
// `return FALSE`.  On that path nGlowSize and clrText are unused by retail too.
// Symbol: ?DrawTextOnGlass@CMFCVisualManager@@UEAAHPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@KHK@Z
extern "C" int MS_ABI impl__DrawTextOnGlass_CMFCVisualManager__UEAAHPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__KHK_Z(
    CMFCVisualManager* pThis, CDC* pDC, const CString& strText, CRect rect,
    unsigned long dwFlags, int /*nGlowSize*/, unsigned long /*clrText*/)
{
    if (!pThis) return FALSE;
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return FALSE;
    const COLORREF clrOld = ::SetTextColor(hdc, RGB(0, 0, 0));
    RECT r = VmRect(rect);
    ::DrawTextW(hdc, strText.GetString(), strText.GetLength(), &r, static_cast<UINT>(dwFlags));
    ::SetTextColor(hdc, clrOld);
    return FALSE; // retail's NULL-theme terminal returns FALSE (0x6c760)
}

// CMFCVisualManager::GetAutoHideButtonTextColor(CMFCAutoHideButton*) -- retail
// body (RVA 0x18ae00, mfc140u; ICF-folded with GetRibbonStatusBarTextColor)
// returns afxGlobalData +0x74 unconditionally; pButton is unused.
// Symbol: ?GetAutoHideButtonTextColor@CMFCVisualManager@@UEAAKPEAVCMFCAutoHideButton@@@Z
extern "C" unsigned long MS_ABI impl__GetAutoHideButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCAutoHideButton___Z(
    CMFCVisualManager* pThis, void* /*pButton*/)
{
    return pThis ? ::GetSysColor(COLOR_BTNTEXT) : 0;
}

// CMFCVisualManager::GetCaptionBarTextColor(CMFCCaptionBar*) -- faithful
// transcription of the retail body (RVA 0x1885a0, mfc140u):
//     if (pBar->[+0x404] != 0) return ::GetSysColor(COLOR_INFOTEXT);  // tail jmp, ecx = 0x17
//     return afxGlobalData +0x78;                                    // COLOR_WINDOW
// +0x404 is the flag the CMFCCaptionBar shadow struct in
// detail/CMFCCaptionBarSupport.h names m_bDrawBackground (retail MFC's
// m_bIsMessageBarMode; the caption bar's own OnDrawBackground at RVA 0x208a0
// gates on the same slot).  Retail dereferences pBar unconditionally; the NULL
// guard is OpenMFC's and yields the default-state (flag clear) answer.
// Symbol: ?GetCaptionBarTextColor@CMFCVisualManager@@UEAAKPEAVCMFCCaptionBar@@@Z
extern "C" unsigned long MS_ABI impl__GetCaptionBarTextColor_CMFCVisualManager__UEAAKPEAVCMFCCaptionBar___Z(
    CMFCVisualManager* pThis, CMFCCaptionBar* pBar)
{
    if (!pThis) return 0;
    const CB* pShadow = reinterpret_cast<const CB*>(pBar);
    if (pShadow != nullptr && pShadow->m_bDrawBackground != 0) {
        return ::GetSysColor(COLOR_INFOTEXT);
    }
    return ::GetSysColor(COLOR_WINDOW);
}

// CMFCVisualManager::GetHighlightedMenuItemTextColor(CMFCToolBarMenuButton*)
// -- faithful transcription of the retail body (RVA 0x184db0, mfc140u):
//     if (pButton->[+0x28] & 0x40000) return afxGlobalData +0x44;  // COLOR_GRAYTEXT
//     return afxGlobalData +0x4c;                                  // COLOR_HIGHLIGHTTEXT
// CMFCToolBarButton::m_nStyle is the member at retail +0x28 (the same test the
// already-implemented GetToolbarButtonTextColor in this file uses).
// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManager@@UEAAKPEAVCMFCToolBarMenuButton@@@Z
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarMenuButton___Z(
    CMFCVisualManager* pThis, CMFCToolBarMenuButton* pButton)
{
    if (!pThis) return 0;
    if (pButton != nullptr && (pButton->m_nStyle & 0x40000)) {
        return ::GetSysColor(COLOR_GRAYTEXT);
    }
    return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
}

// CMFCVisualManager::GetMenuItemTextColor(CMFCToolBarMenuButton*, BOOL, BOOL)
// -- faithful transcription of the retail body (RVA 0x18b7c0, mfc140u).  The
// button pointer is never dereferenced; the result is a pure 2x2 table:
//     bHighlighted && bDisabled  -> afxGlobalData +0x28  (COLOR_BTNFACE)
//     bHighlighted && !bDisabled -> afxGlobalData +0x4c  (COLOR_HIGHLIGHTTEXT)
//     !bHighlighted && bDisabled -> afxGlobalData +0x44  (COLOR_GRAYTEXT)
//     neither                    -> afxGlobalData +0x7c  (COLOR_WINDOWTEXT)
// Symbol: ?GetMenuItemTextColor@CMFCVisualManager@@UEAAKPEAVCMFCToolBarMenuButton@@HH@Z
extern "C" unsigned long MS_ABI impl__GetMenuItemTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarMenuButton__HH_Z(
    CMFCVisualManager* pThis, CMFCToolBarMenuButton* /*pButton*/, int bHighlighted, int bDisabled)
{
    if (!pThis) return 0;
    if (bHighlighted) {
        return bDisabled ? ::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_HIGHLIGHTTEXT);
    }
    return bDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_WINDOWTEXT);
}

// CMFCVisualManager::GetNcBtnSize(BOOL) const -- faithful transcription of the
// retail body (RVA 0x88e0, mfc140u, ICF-folded with CMFCRibbonTab::GetRegularSize):
//     movq $0,(%rdx) ; mov %rdx,%rax ; ret
// i.e. it writes CSize(0, 0) into the hidden return slot (rdx, because rcx
// carries `this`) and returns that slot.  The BOOL argument is ignored.
// Symbol: ?GetNcBtnSize@CMFCVisualManager@@UEBA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__GetNcBtnSize_CMFCVisualManager__UEBA_AVCSize__H_Z(
    const CMFCVisualManager* /*pThis*/, CSize* pRet, int /*bHorz*/)
{
    if (pRet != nullptr) {
        pRet->cx = 0;
        pRet->cy = 0;
    }
    return pRet;
}

// CMFCVisualManager::OnDrawTabResizeBar(CDC*, CMFCBaseTabCtrl*, BOOL, CRect,
// CBrush*, CPen*) -- faithful transcription of the retail body (RVA 0x187280,
// mfc140u):
//     ::FillRect(pDC->m_hDC, &rect, pbrFace ? pbrFace->m_hObject : NULL);
//     pOld = pDC->SelectObject(pPen);
//     pDC->MoveTo(rect.left, rect.top);
//     pDC->LineTo(bIsHorz ? rect.left  : rect.right,
//                 bIsHorz ? rect.bottom : rect.top);
//     pDC->SelectObject(pOld);
// The tab control is not touched at all.  Retail passes the brush handle to
// ::FillRect even when it is NULL, so a NULL pbrFace simply paints nothing.
// Symbol: ?OnDrawTabResizeBar@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCBaseTabCtrl@@HVCRect@@PEAVCBrush@@PEAVCPen@@@Z
extern "C" void MS_ABI impl__OnDrawTabResizeBar_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseTabCtrl__HVCRect__PEAVCBrush__PEAVCPen___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCBaseTabCtrl* /*pTabWnd*/, int bIsHorz,
    CRect rect, CBrush* pbrFace, CPen* pPen)
{
    if (!pThis) return;
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return;
    RECT r = VmRect(rect);
    ::FillRect(hdc, &r, pbrFace != nullptr ? static_cast<HBRUSH>(pbrFace->GetSafeHandle()) : nullptr);
    HGDIOBJ hOld = nullptr;
    if (pPen != nullptr && pPen->GetSafeHandle() != nullptr) {
        hOld = ::SelectObject(hdc, pPen->GetSafeHandle());
    }
    ::MoveToEx(hdc, rect.left, rect.top, nullptr);
    if (bIsHorz) {
        ::LineTo(hdc, rect.left, rect.bottom);
    } else {
        ::LineTo(hdc, rect.right, rect.top);
    }
    if (hOld != nullptr) ::SelectObject(hdc, hOld);
}

// CMFCVisualManager::OnDrawTasksGroupIcon(...) -- retail body (RVA 0x189520,
// mfc140u) reads six CMFCTasksPaneTaskGroup members (the HICON at +0x88, the
// origin at +0x5c/+0x60, +0x68, and the icon extent at +0x7c/+0x80), plus the
// int at +0x538 of the object two hops away (group+0x08 -> that object's +0x10
// -> +0x538; falling back to CMFCVisualManager+0xe4 when it is -1), centres the
// icon, and calls ::DrawIconEx(pDC->m_hDC, x, y, hIcon, cx, cy, 0, NULL,
// DI_NORMAL /* 3 */) through the import at 0x1802c72e0.  Returns immediately
// when the +0x88 icon handle is NULL.
// None of those members are modeled by OpenMFC: afxmfc.h declares
// CMFCTasksPaneTaskGroup as an opaque CObject with a 32-byte pad (no shadow
// struct pins the retail offsets) and CMFCVisualManager carries only padding,
// so nothing can be drawn without inventing a layout.
// TODO(clean-room): not transcribed -- CMFCTasksPaneTaskGroup layout unmodeled.
// Symbol: ?OnDrawTasksGroupIcon@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupIcon_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* /*pGroup*/,
    int /*nIconHOffset*/, int /*bIsHighlighted*/, int /*bIsSelected*/, int /*bCanCollapse*/)
{
    if (!pThis || !pDC) return;
}

// CMFCVisualManager::OnEraseMDIClientArea(CDC*, CRect) -- faithful
// transcription: the retail body is `xor %eax,%eax ; ret` at RVA 0x71e0
// (mfc140u), i.e. it draws nothing and returns FALSE so the caller falls back
// to its own erase.  (That two-instruction body is ICF-folded and shared with
// several other "return FALSE" exports, e.g. CPageSetupDialog::OnDrawPage.)
// Symbol: ?OnEraseMDIClientArea@CMFCVisualManager@@UEAAHPEAVCDC@@VCRect@@@Z
extern "C" int MS_ABI impl__OnEraseMDIClientArea_CMFCVisualManager__UEAAHPEAVCDC__VCRect___Z(
    CMFCVisualManager* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/)
{
    return FALSE; // retail: xor eax,eax; ret
}

// CMFCVisualManager::OnErasePopupWindowButton(CDC*, CRect, CMFCDesktopAlertWndButton*)
// -- retail body (RVA 0x18b2a0, mfc140u):
//     if (pButton->[+0xb20] != 0)
//         ::FillRect(pDC->m_hDC, &rectClient, <brush at afxGlobalData+0xb0>);  // COLOR_BTNFACE
//     else {
//         // map the parent's client rect into the button's coordinates and
//         // let the virtual at CMFCVisualManager vftable +0x3c8
//         // (OnFillPopupWindowBackground) paint it:
//         ::GetClientRect(::GetParent(pButton->m_hWnd), &rc);
//         ::MapWindowPoints(hwndParent, pButton->m_hWnd, &rc, 2);
//         this->OnFillPopupWindowBackground(pDC, rc);
//     }
// (0x1802c72d8 = USER32!GetParent, 0x1802c7330 = GetClientRect, 0x1802c7228 =
// MapWindowPoints; each HWND goes through CWnd::FromHandle at 0x28ad70 first.)
// afxmfc.h declares CMFCDesktopAlertWndButton as an opaque 16-byte-padded
// CObject -- not the retail CMFCButton-derived window -- so neither the +0xb20
// selector nor an m_hWnd exists on the OpenMFC object, and there is no virtual
// slot to dispatch through.
// TODO(clean-room): not transcribed -- CMFCDesktopAlertWndButton unmodeled.
// Symbol: ?OnErasePopupWindowButton@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect /*rectClient*/, CMFCDesktopAlertWndButton* /*pButton*/)
{
    if (!pThis || !pDC) return;
}

// CMFCVisualManager::OnEraseTabsArea(CDC*, CRect, const CMFCBaseTabCtrl*) --
// faithful transcription of the retail body at RVA 0x185680 (mfc140u), a body
// ICF-shared by OnEraseTabsArea, OnEraseTabsButton, OnFillAutoHideButtonBackground
// and OnFillPopupWindowBackground (it is also CMFCVisualManager vftable +0x3c8):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x120>);
// where that brush is CreateSolidBrush(afxGlobalData+0x60) == COLOR_BTNFACE.
// The tab control is never touched.
// Symbol: ?OnEraseTabsArea@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnEraseTabsButton(...) -- same retail body as
// OnEraseTabsArea (RVA 0x185680, mfc140u; the two exports are ICF-folded onto
// it): fill the rect with the cached COLOR_BTNFACE brush at afxGlobalData+0x120.
// Neither the button nor the tab control is touched.
// Symbol: ?OnEraseTabsButton@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, CMFCButton* /*pButton*/,
    CMFCBaseTabCtrl* /*pTabWnd*/)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnEraseTabsFrame(CDC*, CRect, const CMFCBaseTabCtrl*) --
// retail body (RVA 0x186c10, mfc140u):
//     int n = pTabWnd->vftable[+0x428]();          // active-tab getter
//     COLORREF clr = pTabWnd->vftable[+0x3c8](n);  // per-tab background color
//     if (clr == (COLORREF)-1) return FALSE;
//     pDC->FillSolidRect(&rect, clr);
//     return TRUE;
// Both calls are virtuals of CMFCBaseTabCtrl.  OpenMFC's CMFCBaseTabCtrl has a
// different vtable, its GetActiveTab is a non-virtual C++ method with no impl__
// thunk, and while an impl__GetTabBkColor_CMFCBaseTabCtrl thunk does exist
// (tabs/CMFCBaseTabCtrl.cpp) it returns -1 for every tab because the per-tab
// record is unmodeled -- so the only value this body could ever compute is the
// -1 that makes retail return FALSE without painting.
// TODO(clean-room): not transcribed -- CMFCBaseTabCtrl vtable slots +0x428 /
// +0x3c8 are not modeled and the per-tab color is not available.
// Symbol: ?OnEraseTabsFrame@CMFCVisualManager@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManager__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManager* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    return FALSE;
}

// CMFCVisualManager::OnFillAutoHideButtonBackground(CDC*, CRect, CMFCAutoHideButton*)
// -- same retail body as OnEraseTabsArea (RVA 0x185680, mfc140u, ICF-folded):
// fill the rect with the cached COLOR_BTNFACE brush at afxGlobalData+0x120.
// The button is not touched.
// Symbol: ?OnFillAutoHideButtonBackground@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnFillAutoHideButtonBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, void* /*pButton*/)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnFillButtonInterior(CDC*, CMFCToolBarButton*, CRect,
// AFX_BUTTON_STATE) -- retail body (RVA 0x184bb0, mfc140u), decoded in full:
//     if (pButton->IsKindOf(<0x30f268 = CMFCShowAllButton>)) {
//         if (state == 2) CDrawingManager(*pDC).HighlightRect(rect, -1, -1, 0, -1);
//         return;
//     }
//     if (this->[+0xb0] == 0) {                       // m_bEnableToolbarButtonFill
//         if (!pButton || !pButton->IsKindOf(<0x3b1748 = CMFCToolBarMenuButton>)
//             || !pButton->m_pWndParent /* +0x80 */
//             || !pButton->m_pWndParent->IsKindOf(<0x3b1478 = CMFCPopupMenuBar>))
//             return;
//     }
//     if (pButton->IsKindOf(<0x3b13b8 = CMFCOutlookBarPaneButton>)) return;
//     if (CMFCToolBar::m_bCustomizeMode /* 0x3be35c */) return;
//     if (state == 2) return;
//     if (!(pButton->m_nStyle /* +0x28 */ & 0x110000)) return;   // TBBS_CHECKED | TBBS_INDETERMINATE
//     CRect r = rect; ::InflateRect(&r, -afxData.cxBorder2, -afxData.cyBorder2);  // 0x3c32d0 / 0x3c32d4
//     CMFCToolBarImages::FillDitheredRect(pDC, r);                             // 0x16d4b0
// The four CRuntimeClass addresses were read off the matching ?GetThisClass@
// getters; 0x3c32c0 is ?afxData@@3UAUX_DATA@@A, so the two deflate amounts are
// AUX_DATA::cxBorder2 / cyBorder2 (both 2 in retail -- CX_BORDER*2; OpenMFC's
// exported afxData is a zero blob, so the constant is used).  FillDitheredRect
// is `::FillRect(pDC->m_hDC, &rect, afxGlobalData+0xd0)`, the handle of the
// CBrush at +0xc8 ("brLight"), which the color initializer at 0x6b1c0 builds as
// a solid brush of the per-channel midpoint between field +0x28 (COLOR_BTNFACE)
// and +0x30 (COLOR_BTNHIGHLIGHT) -- face + trunc((hilite - face) / 2) -- when
// the screen is deeper than 8bpp (0x6b5bc..0x6b608), and as a CreatePatternBrush
// of a dither bitmap (helper 0x6b8b0) otherwise.
// What is transcribed: the checked / indeterminate fill on the default path,
// with this+0xb0 taken as TRUE -- the value the retail constructor stores at
// 0x18409f (`mov %rsi,0xb0(%rbx)`, rsi == 1) and that OpenMFC's padding-only
// CMFCVisualManager cannot represent -- and the solid midpoint brush on every
// color depth.  NOT transcribed: the CDrawingManager::HighlightRect highlight of
// a hovered CMFCShowAllButton (returns without painting instead), the
// m_bEnableToolbarButtonFill == FALSE menu-button exception, and the <= 8bpp
// dither pattern.  The NULL guards are OpenMFC's; retail's IsKindOf takes its
// range-check-failure path on a NULL button.
// TODO(clean-room): transcribed partially -- see the list above.
// Symbol: ?OnFillButtonInterior@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@1@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int nState)
{
    if (!pThis || !pDC || !pButton) return;
    const CObject* pObj = pButton;
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pObj, impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ())) {
        // retail: state == 2 -> CDrawingManager::HighlightRect (not modeled)
        return;
    }
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pObj, impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    if (nState == 2) return;
    if ((pButton->m_nStyle & 0x110000) == 0) return;
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return;
    RECT r = VmRect(rect);
    ::InflateRect(&r, -2, -2);
    const COLORREF clrFace = ::GetSysColor(COLOR_BTNFACE);
    const COLORREF clrHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    auto mid = [](int face, int hilite) -> BYTE {
        return static_cast<BYTE>(face + (hilite - face) / 2);
    };
    HBRUSH hbr = ::CreateSolidBrush(RGB(mid(GetRValue(clrFace), GetRValue(clrHilite)),
                                        mid(GetGValue(clrFace), GetGValue(clrHilite)),
                                        mid(GetBValue(clrFace), GetBValue(clrHilite))));
    if (hbr != nullptr) {
        ::FillRect(hdc, &r, hbr);
        ::DeleteObject(hbr);
    }
}

// CMFCVisualManager::OnFillCaptionBarButton(CDC*, CMFCCaptionBar*, CRect,
// BOOL bIsPressed, BOOL bIsHighlighted, BOOL bIsDisabled, BOOL bHasDropDownArrow,
// BOOL bIsSysButton) -- faithful transcription of the retail body (RVA 0x188810,
// mfc140u):
//     if (pBar->[+0x404] == 0) return (COLORREF)-1;   // nothing painted
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x120>);  // COLOR_BTNFACE
//     return bIsDisabled ? afxGlobalData+0x44   // COLOR_GRAYTEXT
//                        : afxGlobalData+0x74;  // COLOR_BTNTEXT
// bIsDisabled is the 7th argument (stack slot 0x60 after the 0x20 prologue
// shift); bIsPressed / bIsHighlighted / bHasDropDownArrow / bIsSysButton are not
// read.  +0x404 is the CMFCCaptionBar flag the shadow struct in
// detail/CMFCCaptionBarSupport.h calls m_bDrawBackground (retail's
// m_bIsMessageBarMode), the same slot GetCaptionBarTextColor tests.  Retail
// dereferences pBar unconditionally; the NULL guard is OpenMFC's and takes the
// flag-clear arm.
// Symbol: ?OnFillCaptionBarButton@CMFCVisualManager@@UEAAKPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int /*bIsPressed*/, int /*bIsHighlighted*/, int bIsDisabled,
    int /*bHasDropDownArrow*/, int /*bIsSysButton*/)
{
    if (!pThis) return static_cast<unsigned long>(-1);
    const CB* pShadow = reinterpret_cast<const CB*>(pBar);
    if (pShadow == nullptr || pShadow->m_bDrawBackground == 0) {
        return static_cast<unsigned long>(-1);
    }
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
    return bIsDisabled ? ::GetSysColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_BTNTEXT);
}

// CMFCVisualManager::OnFillCommandsListBackground(CDC*, CRect, BOOL bIsSelected)
// -- faithful transcription of the retail body (RVA 0x187320, mfc140u):
//   bIsSelected:
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0xc0>);  // COLOR_HIGHLIGHT
//     ::InflateRect(&rect, -1, -1);  rect.bottom--;  rect.right--;
//     four ::PatBlt(hdc, ..., PATINVERT) calls trace a 1px frame:
//       (left,        top+1, 1,            bottom-top)
//       (left,        top,   right-left,   1)
//       (right,       top,   1,            bottom-top)
//       (left+1,      bottom, right-left,  1)
//     return afxGlobalData+0x4c;   // COLOR_HIGHLIGHTTEXT
//   otherwise:
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x120>);  // COLOR_BTNFACE
//     return afxGlobalData+0x74;   // COLOR_BTNTEXT
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManager@@UEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManager__UEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bIsSelected)
{
    if (!pThis) return 0;
    if (!bIsSelected) {
        VmFillSysColor(pDC, rect, COLOR_BTNFACE);
        return ::GetSysColor(COLOR_BTNTEXT);
    }
    VmFillSysColor(pDC, rect, COLOR_HIGHLIGHT);
    HDC hdc = VmHdc(pDC);
    if (hdc != nullptr) {
        RECT r = VmRect(rect);
        ::InflateRect(&r, -1, -1);
        r.bottom -= 1;
        r.right -= 1;
        const int cy = r.bottom - r.top;
        const int cx = r.right - r.left;
        ::PatBlt(hdc, r.left, r.top + 1, 1, cy, PATINVERT);
        ::PatBlt(hdc, r.left, r.top, cx, 1, PATINVERT);
        ::PatBlt(hdc, r.right, r.top, 1, cy, PATINVERT);
        ::PatBlt(hdc, r.left + 1, r.bottom, cx, 1, PATINVERT);
    }
    return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
}

// CMFCVisualManager::OnFillHeaderCtrlBackground(CMFCHeaderCtrl*, CDC*, CRect)
// -- faithful transcription of the retail body (RVA 0x188c40, mfc140u):
//     HBRUSH hbr = pCtrl->[+0x130] != 0 ? <brush at afxGlobalData+0xb0>
//                                       : <brush at afxGlobalData+0x120>;
//     ::FillRect(pDC->m_hDC, &rect, hbr);
// Both cached brushes are CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)) --
// the first from field +0x28, the second from field +0x60 -- so the two arms
// paint the same color and the unmodeled CMFCHeaderCtrl +0x130 selector makes
// no visible difference.
// Symbol: ?OnFillHeaderCtrlBackground@CMFCVisualManager@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillHeaderCtrlBackground_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CMFCHeaderCtrl* /*pCtrl*/, CDC* pDC, CRect rect)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnFillMiniFrameCaption(CDC*, CRect, CPaneFrameWnd*, BOOL)
// -- transcription of the retail body (RVA 0x187d80, mfc140u):
//     CObject* p = pFrameWnd->vftable[+0x360]();          // CPaneFrameWnd::GetPane (0xaf340)
//     if (p != NULL && !p->IsKindOf(<CRuntimeClass at 0x2deb80>)) p = NULL;
//     BOOL bAct = (p != NULL) ? TRUE : bActive;            // cmove 0x80(%rsp),%ebx
//     CBrush br(bAct ? afxGlobalData+0x88 : afxGlobalData+0x8c);
//     ::FillRect(pDC->m_hDC, &rectCaption, br);
//     return bAct ? afxGlobalData+0x80 : afxGlobalData+0x90;
// i.e. COLOR_ACTIVECAPTION / COLOR_INACTIVECAPTION for the fill and
// COLOR_CAPTIONTEXT / COLOR_INACTIVECAPTIONTEXT for the returned text color.
// Slot +0x360 of the CPaneFrameWnd vftable (0x2f7b88) holds the GetPane export,
// and 0x2deb80 is the CRuntimeClass ?GetThisClass@CMFCBaseToolBar@@ returns
// (read from that getter's `lea`), so a floating toolbar's caption always
// paints active.  OpenMFC has both pieces: the impl__GetPane_CPaneFrameWnd thunk
// (docking/CPaneFrameWnd.cpp, side-table backed) and the CMFCBaseToolBar RTTI
// descriptor (toolbar/RuntimeClasses.cpp).
// Deviation: retail dispatches GetPane virtually; OpenMFC's CPaneFrameWnd has
// no matching vtable slot, so the base thunk is called directly and a derived
// frame's override is not reached.  The NULL guards are OpenMFC's.
// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManager@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManager__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectCaption, CPaneFrameWnd* pFrameWnd, int bActive)
{
    if (!pThis) return 0;
    if (pFrameWnd != nullptr) {
        void* pPane = impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pFrameWnd);
        if (pPane != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                reinterpret_cast<const CObject*>(pPane),
                impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ())) {
            bActive = TRUE;
        }
    }
    VmFillSysColor(pDC, rectCaption, bActive ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION);
    return bActive ? ::GetSysColor(COLOR_CAPTIONTEXT) : ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
}

// CMFCVisualManager::OnFillOutlookBarCaption(CDC*, CRect, COLORREF&) --
// faithful transcription of the retail body (RVA 0x18aee0, mfc140u):
//     pDC->FillSolidRect(&rectCaption, afxGlobalData+0x64);  // COLOR_BTNSHADOW
//     clrText = afxGlobalData+0x68;                          // COLOR_BTNHIGHLIGHT
// Symbol: ?OnFillOutlookBarCaption@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookBarCaption_CMFCVisualManager__UEAAXPEAVCDC__VCRect__AEAK_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectCaption, unsigned long& clrText)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rectCaption, COLOR_BTNSHADOW);
    clrText = ::GetSysColor(COLOR_BTNHIGHLIGHT);
}

// CMFCVisualManager::OnFillOutlookPageButton(CDC*, const CRect&, BOOL, BOOL,
// COLORREF&) -- faithful transcription of the retail body (RVA 0x188380,
// mfc140u):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x120>);  // COLOR_BTNFACE
//     clrText = afxGlobalData+0x74;                                   // COLOR_BTNTEXT
// The two BOOLs are read into registers but never used by the base body.
// Symbol: ?OnFillOutlookPageButton@CMFCVisualManager@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManager__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(
    CMFCVisualManager* pThis, CDC* pDC, const CRect& rect, int /*bIsHighlighted*/,
    int /*bIsPressed*/, unsigned long& clrText)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
    clrText = ::GetSysColor(COLOR_BTNTEXT);
}

// CMFCVisualManager::OnFillPopupWindowBackground(CDC*, CRect) -- same retail
// body as OnEraseTabsArea (RVA 0x185680, mfc140u, ICF-folded; it is also the
// virtual sitting at CMFCVisualManager vftable +0x3c8):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x120>);  // COLOR_BTNFACE
// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnFillRibbonButton(CDC*, CMFCRibbonButton*) -- retail body
// (RVA 0x18d090, mfc140u) branches three ways on the element type:
//   * pButton->IsKindOf(<CRuntimeClass at 0x180304ac0>): paints the rect built
//     from pButton+0x1a8 / pButton+0xc8 either through
//     CDrawingManager::DrawRect(rect, afxGlobalData+0x78, afxGlobalData+0x64)
//     or through ::FillRect + CDrawingManager::HighlightRect + CDC::Draw3dRect,
//     gated on the two virtuals at CMFCRibbonBaseElement vftable +0x1d8/+0x1b0
//     and on the global BOOL at 0x1803be394;
//   * a second arm keyed on ?IsMenuMode@CMFCRibbonBaseElement@@QEBAHXZ (RVA
//     0x10520) and the virtuals at
//     +0x418/+0x1b0 fills pButton+0xc8 with the COLOR_HIGHLIGHT brush at
//     afxGlobalData+0xc0 and returns afxGlobalData+0x4c (COLOR_HIGHLIGHTTEXT);
//   * a third arm (+0x1d0/+0x1b0) fills pButton+0xc8 with afxGlobalData+0x78
//     (COLOR_WINDOW) or dithers it via CMFCToolBarImages::FillDitheredRect.
// Every other path returns (COLORREF)-1.  CMFCRibbonButton's member layout, the
// CMFCRibbonBaseElement vtable and CDrawingManager are all unmodeled in
// OpenMFC, so only the "-1" terminal can be produced.
// TODO(clean-room): not transcribed -- CMFCRibbonButton layout / ribbon element
// vtable / CDrawingManager unmodeled.
// Symbol: ?OnFillRibbonButton@CMFCVisualManager@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManager* /*pThis*/, CDC* /*pDC*/, CMFCRibbonButton* /*pButton*/)
{
    return static_cast<unsigned long>(-1);
}

// CMFCVisualManager::OnFillRibbonEdit(CDC*, CMFCRibbonRichEditCtrl*, CRect,
// BOOL bIsHighlighted, BOOL bIsPaneHighlighted, BOOL bIsDisabled, COLORREF&,
// COLORREF&, COLORREF&) -- retail body (RVA 0x18d3e0, mfc140u):
//     if (bIsHighlighted && !bIsDisabled)
//         fill rect with afxGlobalData+0x78  (COLOR_WINDOW)
//     else {
//         fill rect with afxGlobalData+0x60  (COLOR_BTNFACE)
//         if the global BOOL at 0x1803be394 is clear, additionally run
//         CDrawingManager::HighlightRect(rect, -1, -1, 0, -1) over it
//     }
// The fill is done either by CDrawingManager::DrawRect(rect, clr, -1) (border
// suppressed) or by ::FillRect with the matching cached brush, depending on
// that same global; both paint the identical color, which is why the fill below
// is unconditional.  None of the three COLORREF& out-parameters is written by
// the base body.  The rich-edit pointer is never dereferenced.
// TODO(clean-room): transcribed partially -- the CDrawingManager::HighlightRect
// overlay on the non-highlighted path is not modeled.
// Symbol: ?OnFillRibbonEdit@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonRichEditCtrl@@VCRect@@HHHAEAK33@Z
extern "C" void MS_ABI impl__OnFillRibbonEdit_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonRichEditCtrl__VCRect__HHHAEAK33_Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pEdit*/, CRect rect,
    int bIsHighlighted, int /*bIsPaneHighlighted*/, int bIsDisabled,
    unsigned long& /*clrText*/, unsigned long& /*clrSelBackground*/, unsigned long& /*clrSelText*/)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, (bIsHighlighted && !bIsDisabled) ? COLOR_WINDOW : COLOR_BTNFACE);
}

// CMFCVisualManager::OnFillRibbonMainPanelButton(CDC*, CMFCRibbonButton*) --
// faithful transcription of the retail body (RVA 0x18d3a0, mfc140u), which is
// three instructions:
//     mov (%rcx),%rax ; mov 0x480(%rax),%rax ; jmp <cfg dispatch>
// i.e. an unconditional tail-call through CMFCVisualManager vftable +0x480.
// That slot holds OnFillRibbonButton (verified: the vftable at 0x31c128 stores
// 0x18d090 there, which is the OnFillRibbonButton export).  OpenMFC's
// CMFCVisualManager declares no such virtual, so the base thunk is called
// directly -- a derived visual manager's override is therefore NOT picked up,
// which is a deviation from retail's virtual dispatch.
// Symbol: ?OnFillRibbonMainPanelButton@CMFCVisualManager@@UEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonMainPanelButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonButton* pButton)
{
    return impl__OnFillRibbonButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(pThis, pDC, pButton);
}

// CMFCVisualManager::OnFillRibbonMenuFrame(CDC*, CMFCRibbonMainPanel*, CRect)
// -- faithful transcription of the retail body (RVA 0x18da70, mfc140u):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x110>);
// where that brush is CreateSolidBrush(afxGlobalData+0x78) == COLOR_WINDOW.
// The panel pointer is not touched.
// Symbol: ?OnFillRibbonMenuFrame@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonMainPanel* /*pPanel*/, CRect rect)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_WINDOW);
}

// CMFCVisualManager::OnFillRibbonQuickAccessToolBarPopup(CDC*,
// CMFCRibbonPanelMenuBar*, CRect) -- faithful transcription of the retail body
// (RVA 0x18e670, mfc140u):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x120>);  // COLOR_BTNFACE
// The menu bar pointer is not touched.
// Symbol: ?OnFillRibbonQuickAccessToolBarPopup@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCRibbonPanelMenuBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonQuickAccessToolBarPopup_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonPanelMenuBar__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pMenuBar*/, CRect rect)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnFillSplitterBackground(CDC*, CSplitterWndEx*, CRect) --
// faithful transcription of the retail body (RVA 0x189fe0, mfc140u):
//     pDC->FillSolidRect(&rect, afxGlobalData+0x60);   // COLOR_BTNFACE
// The splitter pointer is not touched.
// Symbol: ?OnFillSplitterBackground@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCSplitterWndEx@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillSplitterBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCSplitterWndEx__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pSplitterWnd*/, CRect rect)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_BTNFACE);
}

// CMFCVisualManager::OnFillTab(CDC*, CRect, CBrush*, int iTab, BOOL bIsActive,
// const CMFCBaseTabCtrl*) -- retail body (RVA 0x186b00, mfc140u):
//     HBRUSH hbr = pbrFill ? pbrFill->m_hObject : NULL;
//     if (bIsActive && !afxGlobalData[+0x264] && !afxGlobalData[+0x260]
//         && (pTabWnd->vftable[+0x520]() || pTabWnd->vftable[+0x528]()
//             || pTabWnd->vftable[+0x530]())
//         && pTabWnd->vftable[+0x3c8](iTab) == (COLORREF)-1)
//         hbr = <brush at afxGlobalData+0x110>;          // COLOR_WINDOW
//     ::FillRect(pDC->m_hDC, &rectFill, hbr);
// The three style predicates and the per-tab background-color getter are
// CMFCBaseTabCtrl virtuals whose slots OpenMFC does not model, so only the
// general arm -- fill with the caller's brush -- is transcribed.
// TODO(clean-room): transcribed partially -- the active-tab special case that
// substitutes the COLOR_WINDOW brush needs CMFCBaseTabCtrl vftable slots
// +0x520/+0x528/+0x530/+0x3c8.
// Symbol: ?OnFillTab@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectFill, CBrush* pbrFill,
    int /*iTab*/, int /*bIsActive*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    if (!pThis) return;
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return;
    RECT r = VmRect(rectFill);
    ::FillRect(hdc, &r, pbrFill != nullptr ? static_cast<HBRUSH>(pbrFill->GetSafeHandle()) : nullptr);
}

// CMFCVisualManager::OnFillTasksGroupInterior(CDC*, CRect, BOOL) -- faithful
// transcription: the retail body is `ret` at RVA 0x27d0 (mfc140u), a pure
// no-op that the linker folded together with several other empty virtuals
// (the symbol that survives at that RVA in the map is OnDrawRibbonLabel).
// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManager__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManager* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, int /*bIsSpecial*/)
{
    // Retail draws nothing here (RVA 0x27d0 is a bare `ret`).
}

// CMFCVisualManager::OnFillTasksPaneBackground(CDC*, CRect) -- faithful
// transcription of the retail body (RVA 0x189020, mfc140u):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0x110>);  // COLOR_WINDOW
// Symbol: ?OnFillTasksPaneBackground@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillTasksPaneBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_WINDOW);
}

// CMFCVisualManager::OnFillToolTip(CDC*, CMFCToolTipCtrl*, CRect, COLORREF&
// clrText, COLORREF& clrLine) -- retail body (RVA 0x18f100, mfc140u):
//     HTHEME h = this->[+0x88];
//     if (h != NULL) {
//         ::DrawThemeBackground(h, pDC->m_hDC, 1, 0, &rect, NULL);
//         ::GetThemeColor(h, 1, 0, 0xedb, &clrText);
//         ::GetThemeColor(h, 1, 0, 0xedf, &clrLine);
//     } else {
//         ::FillRect(pDC->m_hDC, &rect, ::GetSysColorBrush(COLOR_INFOBK));
//     }
// CMFCVisualManager+0x88 (the tooltip theme handle) is not modeled by OpenMFC,
// so the NULL-theme arm is the terminal: fill with the COLOR_INFOBK system
// brush and leave both COLORREF& out-parameters untouched, exactly as retail
// does on that path.  The tooltip control pointer is never dereferenced.
// Symbol: ?OnFillToolTip@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolTipCtrl@@VCRect@@AEAK3@Z
extern "C" void MS_ABI impl__OnFillToolTip_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolTipCtrl__VCRect__AEAK3_Z(
    CMFCVisualManager* pThis, CDC* pDC, void* /*pToolTip*/, CRect rect,
    unsigned long& /*clrText*/, unsigned long& /*clrLine*/)
{
    if (!pThis) return;
    HDC hdc = VmHdc(pDC);
    if (hdc == nullptr) return;
    RECT r = VmRect(rect);
    ::FillRect(hdc, &r, ::GetSysColorBrush(COLOR_INFOBK));
}

// CMFCVisualManager::OnHighlightMenuItem(CDC*, CMFCToolBarMenuButton*, CRect,
// COLORREF& clrText) -- faithful transcription of the retail body (RVA
// 0x184d60, mfc140u):
//     ::FillRect(pDC->m_hDC, &rect, <brush at afxGlobalData+0xc0>);  // COLOR_HIGHLIGHT
// The button is never dereferenced and clrText is NOT written by the base
// implementation (derived managers override that).
// Symbol: ?OnHighlightMenuItem@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCToolBarMenuButton* /*pButton*/, CRect rect,
    unsigned long& /*clrText*/)
{
    if (!pThis) return;
    VmFillSysColor(pDC, rect, COLOR_HIGHLIGHT);
}

// CMFCVisualManager::OnHighlightRarelyUsedMenuItems(CDC*, CRect) -- retail body
// (RVA 0x184e10, mfc140u):
//     CDrawingManager dm(*pDC);
//     dm.HighlightRect(rect, -1, (COLORREF)-1, 0);
//     pDC->Draw3dRect(&rect, afxGlobalData+0x64, afxGlobalData+0x68);
// i.e. an alpha highlight pass followed by a 3D frame whose top/left edges use
// ::GetSysColor(COLOR_BTNSHADOW) and whose bottom/right edges use
// ::GetSysColor(COLOR_BTNHIGHLIGHT).  CDrawingManager is not modeled by
// OpenMFC, so only the Draw3dRect is reproduced.
// The four segments below reproduce CDC::Draw3dRect's edge extents exactly
// (top cx-1, left cy-1, bottom cx, right cy) for a non-empty rect.  The
// empty-rect early-out is OpenMFC's: retail's Draw3dRect passes negative
// extents to FillSolidRect and would still touch the pixels just outside a
// degenerate rect; no caller hands this function such a rect.
// TODO(clean-room): transcribed partially -- the CDrawingManager::HighlightRect
// pass that precedes the frame is not modeled.
// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect)
{
    if (!pThis || !pDC) return;
    if (rect.right <= rect.left || rect.bottom <= rect.top) return;
    const COLORREF clrTopLeft = ::GetSysColor(COLOR_BTNSHADOW);
    const COLORREF clrBottomRight = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    VmLine(pDC, rect.left, rect.top, rect.right - 1, rect.top, clrTopLeft);
    VmLine(pDC, rect.left, rect.top, rect.left, rect.bottom - 1, clrTopLeft);
    VmLine(pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clrBottomRight);
    VmLine(pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clrBottomRight);
}

// CMFCVisualManager::OnNcActivate(CWnd*, BOOL) -- faithful transcription: the
// retail body is `xor %eax,%eax ; ret` at RVA 0x71e0 (mfc140u), i.e. the base
// manager declines to handle WM_NCACTIVATE and returns FALSE.
// Symbol: ?OnNcActivate@CMFCVisualManager@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnNcActivate_CMFCVisualManager__UEAAHPEAVCWnd__H_Z(
    CMFCVisualManager* /*pThis*/, CWnd* /*pWnd*/, int /*bActive*/)
{
    return FALSE; // retail: xor eax,eax; ret
}

// CMFCVisualManager::OnNcPaint(CWnd*, const CObList&, CRect) -- faithful
// transcription: the retail body is `xor %eax,%eax ; ret` at RVA 0x71e0
// (mfc140u), i.e. the base manager declines to paint the non-client area and
// returns FALSE so the caller falls back to the default frame drawing.
// Symbol: ?OnNcPaint@CMFCVisualManager@@UEAAHPEAVCWnd@@AEBVCObList@@VCRect@@@Z
extern "C" int MS_ABI impl__OnNcPaint_CMFCVisualManager__UEAAHPEAVCWnd__AEBVCObList__VCRect___Z(
    CMFCVisualManager* /*pThis*/, CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, CRect /*rect*/)
{
    return FALSE; // retail: xor eax,eax; ret
}

// CMFCVisualManager::OnSetWindowRegion(CWnd*, CSize) -- retail body (RVA
// 0x18ede0, mfc140u):
//     if (afxGlobalData.IsDwmCompositionEnabled()) return FALSE;
//     if      (pWnd->IsKindOf(<CRuntimeClass at 0x1802e9210>)) p = pWnd->[+0x370];
//     else if (pWnd->IsKindOf(<CRuntimeClass at 0x1802ee478>)) p = pWnd->[+0x710];
//     else return FALSE;
//     if (!p || !::IsWindowVisible(p->m_hWnd) || p->[+0x468] == 0) return FALSE;
//     ... builds a rounded region out of ::CreateRectRgn / ::CreateEllipticRgn
//     and four ::CombineRgn passes, then ::SetWindowRgn(pWnd->m_hWnd, hrgn, TRUE)
//     and returns TRUE.
// 0x1802e9210 is the CRuntimeClass ?GetThisClass@CFrameWndEx@@ returns and
// 0x1802ee478 the one ?GetThisClass@CMDIFrameWndEx@@ returns (both read from
// those getters' `lea`).  +0x370 / +0x710 are the frames' ribbon-bar pointers:
// CFrameWndEx::AddPane (0x67d00) stores the pane there after
// IsKindOf(<CMFCRibbonBar CRuntimeClass at 0x2fea10>), and CMDIFrameWndEx::AddPane
// (0x890e0) does the same into +0x710.  +0x468 is a BOOL of the ribbon bar that
// CMFCRibbonBar::CreateEx (0xd9880) also tests (the public MFC source calls it
// m_bReplaceFrameCaption); so the region is the rounded top of a ribbon frame.
// OpenMFC has no GetRibbonBar thunk for either frame class and models none of
// those offsets, so the region cannot be built; every retail path that is
// reachable here ends in FALSE.
// TODO(clean-room): not transcribed -- the CFrameWndEx / CMDIFrameWndEx
// ribbon-bar members the region depends on are unmodeled.
// Symbol: ?OnSetWindowRegion@CMFCVisualManager@@UEAAHPEAVCWnd@@VCSize@@@Z
extern "C" int MS_ABI impl__OnSetWindowRegion_CMFCVisualManager__UEAAHPEAVCWnd__VCSize___Z(
    CMFCVisualManager* /*pThis*/, CWnd* /*pWnd*/, long long /*sizeWindow*/)
{
    return FALSE;
}

// CMFCVisualManager::OnUpdateSystemColors() -- faithful transcription: the
// retail body is `ret` at RVA 0x27d0 (mfc140u), the same folded no-op body
// OnFillTasksGroupInterior and OnActivateApp use.  The base manager caches no
// colors of its own (afxGlobalData does the caching), so there is nothing to
// refresh; every derived manager overrides this.
// Symbol: ?OnUpdateSystemColors@CMFCVisualManager@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManager__UEAAXXZ(
    CMFCVisualManager* /*pThis*/)
{
    // Retail is a pure no-op (RVA 0x27d0 is a bare `ret`).
}

// CMFCVisualManager::RedrawAll() -- retail body (RVA 0x1842b0, mfc140u):
//     CWinThread* pThread = AfxGetModuleThreadState()->[+0x08];   // 0x133a20
//     CWnd* pMain = pThread ? pThread->vftable[+0xf8]() : NULL;    // GetMainWnd
//     for each node of CFrameImpl::m_lstFrames (head qword at 0x3b1cc8):
//         if (CWnd::FromHandlePermanent(pFrame->m_hWnd))           // 0x28adc0
//             ::RedrawWindow(pFrame->m_hWnd, NULL, NULL, 0x585);   // 0x1802c7130
//     if (pMain && pMain->m_hWnd && CWnd::FromHandlePermanent(pMain->m_hWnd))
//         ::RedrawWindow(pMain->m_hWnd, NULL, NULL, 0x585);
//     for each node of gAllToolbars (head qword at 0x3b2098):
//         if (p && p->IsKindOf(<CRuntimeClass at 0x2f5918>)        // CPane
//               && CWnd::FromHandlePermanent(p->m_hWnd))
//             ::RedrawWindow(p->m_hWnd, NULL, NULL, 0x185);
//     tail-jump ?RedrawAll@CPaneFrameWnd@@SAXXZ (0xb3060).
// (0x585 = RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_UPDATENOW|RDW_FRAME,
//  0x185 = the same without RDW_ALLCHILDREN.  0x2f5918 is the CRuntimeClass
//  ?GetThisClass@CPane@@ returns -- note it is CPane here, not the CMFCToolBar
//  class AdjustToolbars filters on.)
// Transcribed: the main-window redraw (through the impl__AfxGetMainWnd thunk,
// since the module-thread-state / CWinThread vtable path is not modeled), the
// toolbar walk (through impl__GetAllToolbars, IsKindOf CPane) and the
// CPaneFrameWnd::RedrawAll tail call (its OpenMFC thunk is currently an empty
// stub).  NOT transcribed: the CFrameImpl::m_lstFrames walk -- OpenMFC keeps
// that list file-local inside core/frame/CFrameImpl.cpp with no accessor, so
// secondary frames are not repainted from here (see AdjustFrames).
// TODO(clean-room): transcribed partially -- the frame-list walk needs an
// accessor from CFrameImpl.cpp.
// Symbol: ?RedrawAll@CMFCVisualManager@@SAXXZ
extern "C" void MS_ABI impl__RedrawAll_CMFCVisualManager__SAXXZ() {
    CWnd* pMain = impl__AfxGetMainWnd__YAPEAVCWnd__XZ();
    if (pMain != nullptr && pMain->m_hWnd != nullptr &&
        impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pMain->m_hWnd) != nullptr) {
        ::RedrawWindow(pMain->m_hWnd, nullptr, nullptr, 0x585);
    }
    VmForEachLiveToolBar(impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ(), [](CMFCToolBar* pBar) {
        ::RedrawWindow(pBar->m_hWnd, nullptr, nullptr, 0x185);
    });
    impl__RedrawAll_CPaneFrameWnd__SAXXZ();
}

// CMFCVisualManager::RibbonCategoryColorToRGB(AFX_RibbonCategoryColor) --
// faithful transcription of the retail body (RVA 0x18dc60, mfc140u).  Retail
// picks between two palettes: the pastel one when the display is deeper than
// 8bpp (afxGlobalData+0x288) and neither high-contrast flag (+0x264, +0x260) is
// set, the saturated one otherwise.  Both flags and the bit depth are just
// cached ::GetSysColor / ::GetDeviceCaps results (see the table at the top of
// this batch), so they are recomputed live here.  Any value outside 1..7 --
// including AFX_CategoryColor_None (0) -- returns (COLORREF)-1 on both paths.
// Symbol: ?RibbonCategoryColorToRGB@CMFCVisualManager@@UEAAKW4AFX_RibbonCategoryColor@@@Z
extern "C" unsigned long MS_ABI impl__RibbonCategoryColorToRGB_CMFCVisualManager__UEAAKW4AFX_RibbonCategoryColor___Z(
    CMFCVisualManager* pThis, int nColor)
{
    if (!pThis) return static_cast<unsigned long>(-1);
    const bool bRichColors = VmScreenBitsPerPixel() > 8 &&
                             !VmIsWhiteHighContrast() && !VmIsBlackHighContrast();
    if (bRichColors) {
        switch (nColor) {
        case 1: return 0x00a0a0ff; // Red
        case 2: return 0x0037bdef; // Orange
        case 3: return 0x001be5fd; // Yellow
        case 4: return 0x0059be71; // Green
        case 5: return 0x00c4b580; // Blue
        case 6: return 0x00e0a372; // Indigo
        case 7: return 0x00d1b2d6; // Violet
        default: return static_cast<unsigned long>(-1);
        }
    }
    switch (nColor) {
    case 1: return 0x000000ff; // Red
    case 2: return 0x000080ff; // Orange
    case 3: return 0x0000ffff; // Yellow
    case 4: return 0x0000ff00; // Green
    case 5: return 0x00ff0000; // Blue
    case 6: return 0x00800000; // Indigo
    case 7: return 0x00ff00ff; // Violet
    default: return static_cast<unsigned long>(-1);
    }
}
