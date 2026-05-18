// OpenMFC MFC Feature Pack Classes Implementation
// Minimal stub implementations for ABI compatibility.
// Full Feature Pack functionality requires complex rendering engines.
//
// These are C++ class implementations compiled to GCC-mangled names.
// The .def file maps MSVC-mangled exports to weak stubs; these C++ methods
// provide internal implementations that can be called from within the DLL
// and serve as a foundation for future real implementations.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CMFCVisualManager - Base visual manager
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCVisualManager, CObject)

static CMFCVisualManager* g_pVisualManager = nullptr;
static CRuntimeClass* g_pDefaultVisualManagerClass = RUNTIME_CLASS(CMFCVisualManager);

static CMFCVisualManager* CreateVisualManagerFromRuntimeClass(CRuntimeClass* pRTI) {
    CRuntimeClass* pClass = pRTI;
    if (pClass == nullptr || !pClass->IsDerivedFrom(RUNTIME_CLASS(CMFCVisualManager))) {
        pClass = RUNTIME_CLASS(CMFCVisualManager);
    }

    CObject* pObject = pClass->CreateObject();
    if (pObject != nullptr && pObject->IsKindOf(RUNTIME_CLASS(CMFCVisualManager))) {
        return static_cast<CMFCVisualManager*>(pObject);
    }

    delete pObject;
    return new CMFCVisualManager();
}

CMFCVisualManager::CMFCVisualManager() {
    memset(_visualmanager_padding, 0, sizeof(_visualmanager_padding));
    if (g_pVisualManager == nullptr) {
        g_pVisualManager = this;
    }
}

CMFCVisualManager::~CMFCVisualManager() {
    if (g_pVisualManager == this) {
        g_pVisualManager = nullptr;
    }
}

CMFCVisualManager* CMFCVisualManager::GetInstance() {
    if (!g_pVisualManager) {
        g_pVisualManager = CreateVisualManagerFromRuntimeClass(g_pDefaultVisualManagerClass);
    }
    return g_pVisualManager;
}

void CMFCVisualManager::SetDefaultManager(CRuntimeClass* pRTI) {
    g_pDefaultVisualManagerClass = pRTI;
    if (g_pDefaultVisualManagerClass == nullptr ||
        !g_pDefaultVisualManagerClass->IsDerivedFrom(RUNTIME_CLASS(CMFCVisualManager))) {
        g_pDefaultVisualManagerClass = RUNTIME_CLASS(CMFCVisualManager);
    }
}

void CMFCVisualManager::OnDrawMenuBorder(CDC*, CMFCPopupMenu*, CRect) {}
void CMFCVisualManager::OnDrawMenuImage(CDC*, const CRect&, const CRect&) {}
void CMFCVisualManager::OnFillBarBackground(CDC*, CBasePane*, CRect, CRect, BOOL) {}
void CMFCVisualManager::OnDrawBarGripper(CDC*, CRect, BOOL, CBasePane*) {}
void CMFCVisualManager::OnDrawButtonBorder(CDC*, CMFCToolBarButton*, CRect, CMFCVisualManager::AFX_BUTTON_STATE) {}
void CMFCVisualManager::OnDrawButtonSeparator(CDC*, CMFCToolBarButton*, CRect, BOOL) {}
void CMFCVisualManager::OnDrawCaptionButton(CDC*, CMFCCaptionButton*, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawCaptionButtonIcon(CDC*, CMFCCaptionButton*, CMenuImages::IMAGES_IDS, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawCheckBoxEx(CDC*, CRect, int, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawComboBorder(CDC*, CRect, BOOL, BOOL, BOOL, CMFCToolBarComboBoxButton*) {}
void CMFCVisualManager::OnDrawComboDropButton(CDC*, CRect, BOOL, BOOL, BOOL, CMFCToolBarComboBoxButton*) {}
void CMFCVisualManager::OnDrawControlBorder(CWnd*) {}
void CMFCVisualManager::OnDrawDockingBarScrollButton(CDC*, CMFCToolBarButton*, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawEditBorder(CDC*, CRect, BOOL, BOOL, CMFCToolBarEditBoxButton*) {}
void CMFCVisualManager::OnDrawExpandingBox(CDC*, CRect, BOOL, COLORREF) {}
void CMFCVisualManager::OnDrawFloatingToolbarBorder(CDC*, CMFCToolBar*, CRect, CRect) {}
void CMFCVisualManager::OnDrawHeaderCtrlBorder(CMFCHeaderCtrl*, CDC*, CRect&, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawHeaderCtrlSortArrow(CMFCHeaderCtrl*, CDC*, CRect&, BOOL) {}
void CMFCVisualManager::OnDrawMenuArrowOnCustomizeList(CDC*, CRect, BOOL) {}
void CMFCVisualManager::OnDrawMenuCheck(CDC*, CMFCToolBarMenuButton*, CRect, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawMenuItemButton(CDC*, CMFCToolBarMenuButton*, CRect, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawMenuResizeBar(CDC*, CRect, int) {}
void CMFCVisualManager::OnDrawMenuScrollButton(CDC*, CRect, BOOL, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawMenuShadow(CDC*, const CRect&, const CRect&, int, int, int, CBitmap*, CBitmap*, COLORREF) {}
void CMFCVisualManager::OnDrawMenuSystemButton(CDC*, CRect, UINT, UINT, BOOL) {}
void CMFCVisualManager::OnDrawMiniFrameBorder(CDC*, CPaneFrameWnd*, CRect, CRect) {}
void CMFCVisualManager::OnDrawOutlookBarSplitter(CDC*, CRect) {}
void CMFCVisualManager::OnDrawOutlookPageButtonBorder(CDC*, CRect&, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawPaneBorder(CDC*, CBasePane*, CRect&) {}
unsigned long CMFCVisualManager::OnDrawPaneCaption(CDC*, CDockablePane*, int, CRect, int) { return 0; }
void CMFCVisualManager::OnDrawPaneDivider(CDC*, CPaneDivider*, CRect, BOOL) {}
void CMFCVisualManager::OnDrawPopupWindowBorder(CDC*, CRect) {}
void CMFCVisualManager::OnDrawPopupWindowButtonBorder(CDC*, CRect, CMFCDesktopAlertWndButton*) {}
void CMFCVisualManager::OnDrawPopupWindowCaption(CDC*, CRect, CMFCDesktopAlertWnd*) {}
void CMFCVisualManager::OnDrawRibbonApplicationButton(CDC*, CMFCRibbonButton*) {}
void CMFCVisualManager::OnDrawRibbonButtonBorder(CDC*, CMFCRibbonButton*) {}
unsigned long CMFCVisualManager::OnDrawRibbonButtonsGroup(CDC*, CMFCRibbonButtonsGroup*, CRect) { return 0; }
void CMFCVisualManager::OnDrawRibbonCaption(CDC*, CMFCRibbonBar*, CRect, CRect) {}
void CMFCVisualManager::OnDrawRibbonCaptionButton(CDC*, CMFCRibbonCaptionButton*) {}
void CMFCVisualManager::OnDrawRibbonCategory(CDC*, CMFCRibbonCategory*, CRect) {}
unsigned long CMFCVisualManager::OnDrawRibbonCategoryCaption(CDC*, CMFCRibbonContextCaption*) { return 0; }
void CMFCVisualManager::OnDrawRibbonCategoryScroll(CDC*, CMFCRibbonCategoryScroll*) {}
void CMFCVisualManager::OnDrawRibbonCategoryTab(CDC*, CMFCRibbonTab*, BOOL) {}
void CMFCVisualManager::OnDrawRibbonCheckBoxOnList(CDC*, CMFCRibbonCheckBox*, CRect, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawRibbonDefaultPaneButton(CDC*, CMFCRibbonButton*) {}
void CMFCVisualManager::OnDrawRibbonDefaultPaneButtonContext(CDC*, CMFCRibbonButton*) {}
void CMFCVisualManager::OnDrawRibbonDesign(CDC*, CMFCRibbonBar*, CRect) {}
void CMFCVisualManager::OnDrawRibbonEdit(CDC*, CMFCRibbonEdit*, CRect, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawRibbonGalleryBorder(CDC*, CMFCRibbonGallery*, CRect) {}
void CMFCVisualManager::OnDrawRibbonLabel(CDC*, CMFCRibbonLabel*, CRect) {}
void CMFCVisualManager::OnDrawRibbonMainPanelButtonBorder(CDC*, CMFCRibbonButton*) {}
void CMFCVisualManager::OnDrawRibbonMainPanelFrame(CDC*, CMFCRibbonMainPanel*, CRect) {}
void CMFCVisualManager::OnDrawRibbonPanel(CDC*, CMFCRibbonPanel*, CRect, CRect) {}
void CMFCVisualManager::OnDrawRibbonPanelCaption(CDC*, CMFCRibbonPanel*, CRect) {}
void CMFCVisualManager::OnDrawRibbonProgressBar(CDC*, CMFCRibbonProgressBar*, CRect, CRect, BOOL) {}
void CMFCVisualManager::OnDrawRibbonQATSeparator(CDC*, CMFCRibbonSeparator*, CRect) {}
void CMFCVisualManager::OnDrawRibbonQuickAccessToolBarSeparator(CDC*, CMFCRibbonSeparator*, CRect) {}
void CMFCVisualManager::OnDrawRibbonSliderChannel(CDC*, CMFCRibbonSlider*, CRect) {}
void CMFCVisualManager::OnDrawRibbonSliderThumb(CDC*, CMFCRibbonSlider*, CRect, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawRibbonSliderZoomButton(CDC*, CMFCRibbonSlider*, CRect, BOOL, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawRibbonStatusBarPane(CDC*, CMFCRibbonStatusBar*, CMFCRibbonStatusBarPane*) {}
void CMFCVisualManager::OnDrawRibbonTabsFrame(CDC*, CMFCRibbonBar*, CRect) {}
void CMFCVisualManager::OnDrawScrollButtons(CDC*, const CRect&, const int, int, BOOL) {}
void CMFCVisualManager::OnDrawSeparator(CDC*, CBasePane*, CRect, BOOL) {}
void CMFCVisualManager::OnDrawShowAllMenuItems(CDC*, CRect, CMFCVisualManager::AFX_BUTTON_STATE) {}
void CMFCVisualManager::OnDrawSpinButtons(CDC*, CRect, int, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawStatusBarPaneBorder(CDC*, CMFCStatusBar*, CRect, UINT, UINT) {}
void CMFCVisualManager::OnDrawStatusBarProgress(CDC*, CMFCStatusBar*, CRect, int, int, COLORREF, COLORREF, COLORREF, BOOL) {}
void CMFCVisualManager::OnDrawStatusBarSizeBox(CDC*, CMFCStatusBar*, CRect) {}
void CMFCVisualManager::OnDrawTab(CDC*, CRect, int, BOOL, const CMFCBaseTabCtrl*) {}
void CMFCVisualManager::OnDrawTabButton(CDC*, CRect, const CMFCBaseTabCtrl*, int, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawTabCloseButton(CDC*, CRect, const CMFCBaseTabCtrl*, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawTabContent(CDC*, CRect, int, BOOL, const CMFCBaseTabCtrl*, COLORREF) {}
void CMFCVisualManager::OnDrawTabsButtonBorder(CDC*, CRect&, CMFCButton*, UINT, CMFCBaseTabCtrl*) {}
void CMFCVisualManager::OnDrawTask(CDC*, CMFCTasksPaneTask*, CImageList*, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawTasksGroupCaption(CDC*, CMFCTasksPaneTaskGroup*, BOOL, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawTasksGroupAreaBorder(CDC*, CRect, BOOL, BOOL) {}
void CMFCVisualManager::OnDrawTearOffCaption(CDC*, CRect, BOOL) {}
void CMFCVisualManager::OnDrawToolBoxFrame(CDC*, const CRect&) {}

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
}

// Symbol: ?CreateObject@CMFCVisualManager@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManager__SAPEAVCObject__XZ() { return new CMFCVisualManager(); }
// Symbol: ?CreateObject@CMFCVisualManagerOffice2003@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOffice2003__SAPEAVCObject__XZ() { return new CMFCVisualManagerOffice2003(); }
// Symbol: ?CreateObject@CMFCVisualManagerOffice2007@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOffice2007__SAPEAVCObject__XZ() { return new CMFCVisualManagerOffice2007(); }
// Symbol: ?CreateObject@CMFCVisualManagerOfficeXP@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOfficeXP__SAPEAVCObject__XZ() { return new CMFCVisualManagerOfficeXP(); }
// Symbol: ?CreateObject@CMFCVisualManagerVS2005@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerVS2005__SAPEAVCObject__XZ() { return new CMFCVisualManagerVS2005(); }
// Symbol: ?CreateObject@CMFCVisualManagerWindows@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerWindows__SAPEAVCObject__XZ() { return new CMFCVisualManagerWindows(); }
// Symbol: ?CreateObject@CMFCVisualManagerWindows7@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerWindows7__SAPEAVCObject__XZ() { return new CMFCVisualManagerWindows7(); }

// Symbol: ?GetThisClass@CMFCVisualManager@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManager__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManager::GetThisClass(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOffice2003@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOffice2007@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2007__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOffice2007::GetThisClass(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOfficeXP@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ?GetThisClass@CMFCVisualManagerVS2005@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerVS2005::GetThisClass(); }
// Symbol: ?GetThisClass@CMFCVisualManagerWindows@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?GetThisClass@CMFCVisualManagerWindows7@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerWindows7::GetThisClass(); }

// Symbol: ?GetRuntimeClass@CMFCVisualManager@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManager__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManager* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManager::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2003@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2003__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2003* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2007@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2007__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2007* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManagerOffice2007::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOfficeXP@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOfficeXP__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOfficeXP* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2005@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerVS2005__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerVS2005* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManagerVS2005::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows7@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows7__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows7* pThis) { return pThis ? pThis->GetRuntimeClass() : CMFCVisualManagerWindows7::GetThisClass(); }

// Symbol: ?GetToolbarDisabledTextColor@CMFCVisualManager@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetToolbarDisabledTextColor_CMFCVisualManager__UEAAKXZ(CMFCVisualManager* pThis) {
    return pThis ? pThis->GetHighlightedColor(COLOR_GRAYTEXT) : ::GetSysColor(COLOR_GRAYTEXT);
}

// Symbol: ?GetToolbarHighlightColor@CMFCVisualManager@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetToolbarHighlightColor_CMFCVisualManager__UEAAKXZ(CMFCVisualManager* pThis) {
    return pThis ? pThis->GetHighlightedColor(COLOR_HIGHLIGHT) : ::GetSysColor(COLOR_HIGHLIGHT);
}

// Symbol: ?GetShowAllMenuItemsHeight@CMFCVisualManager@@UEAAHPEAVCDC@@AEBVCSize@@@Z
extern "C" int MS_ABI impl__GetShowAllMenuItemsHeight_CMFCVisualManager__UEAAHPEAVCDC__AEBVCSize___Z(
    CMFCVisualManager* pThis, CDC* pDC, const CSize* pSize) {
    (void)pThis;
    (void)pDC;
    return (pSize != nullptr && pSize->cy > 0) ? pSize->cy : 16;
}

// Symbol: ?OnDrawButtonBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@1@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1__Z(
    void* /*class*/*, void* /*class*/*, void* /*class*/, int /*enum*/, short*, int, void*, void* /*struct*/) {}

// Symbol: ?OnDrawButtonSeparator@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@1@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1_H_Z(
    void* /*class*/*, void* /*class*/*, void* /*class*/, int /*enum*/, short*, int, void*, void* /*struct*/,
    void*, void*, double, int, void*, void*, void**, unsigned char, void* /*class*/, int) {}

// Symbol: ?OnDrawCaptionButton@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
    void* /*class*/*, void* /*class*/*, int, int, int, int, int) {}

// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    void* /*class*/*, void* /*class*/*, void* /*class*/, void*) {}

// Symbol: ?OnDrawMenuShadow@CMFCVisualManager@@UEAAXPEAVCDC@@AEBVCRect@@1HHHPEAVCBitmap@@2H@Z
extern "C" void MS_ABI impl__OnDrawMenuShadow_CMFCVisualManager__UEAAXPEAVCDC__AEBVCRect__1HHHPEAVCBitmap__2H_Z(
    void* /*class*/*, const void* /*class*/*, const void* /*class*/*, int, int, int, void* /*class*/*, int, int) {}

// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManager__UEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(
    void* /*class*/*, void* /*class*/*) {}

// Symbol: ?OnDrawShowAllMenuItems@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@W4AFX_BUTTON_STATE@1@@Z
extern "C" void MS_ABI impl__OnDrawShowAllMenuItems_CMFCVisualManager__UEAAXPEAVCDC__VCRect__W4AFX_BUTTON_STATE_1__Z(
    void* /*class*/*, void* /*class*/, int /*enum*/, short*, int, void*, void* /*struct*/) {}

// Symbol: ?OnDrawSpinButtons@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
    void* /*class*/*, void* /*class*/, int, int, void* /*class*/*) {}

// Office variant visual managers
IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2003, CMFCVisualManager)
CMFCVisualManagerOffice2003::CMFCVisualManagerOffice2003() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerOffice2003::~CMFCVisualManagerOffice2003() {}
CObject* CMFCVisualManagerOffice2003::CreateObject() { return new CMFCVisualManagerOffice2003(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2007, CMFCVisualManager)
static CMFCVisualManagerOffice2007::Style g_office2007Style = CMFCVisualManagerOffice2007::Office2007_LunaBlue;
CMFCVisualManagerOffice2007::CMFCVisualManagerOffice2007() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerOffice2007::~CMFCVisualManagerOffice2007() {}
CObject* CMFCVisualManagerOffice2007::CreateObject() { return new CMFCVisualManagerOffice2007(); }
int CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Style style, const wchar_t*) {
    g_office2007Style = style;
    return TRUE;
}
CMFCVisualManagerOffice2007::Style CMFCVisualManagerOffice2007::GetStyle() { return g_office2007Style; }

IMPLEMENT_DYNAMIC(CMFCVisualManagerOfficeXP, CMFCVisualManager)
CMFCVisualManagerOfficeXP::CMFCVisualManagerOfficeXP() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerOfficeXP::~CMFCVisualManagerOfficeXP() {}
CObject* CMFCVisualManagerOfficeXP::CreateObject() { return new CMFCVisualManagerOfficeXP(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerVS2005, CMFCVisualManager)
CMFCVisualManagerVS2005::CMFCVisualManagerVS2005() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005() {}
CObject* CMFCVisualManagerVS2005::CreateObject() { return new CMFCVisualManagerVS2005(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows, CMFCVisualManager)
CMFCVisualManagerWindows::CMFCVisualManagerWindows() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerWindows::~CMFCVisualManagerWindows() {}
CObject* CMFCVisualManagerWindows::CreateObject() { return new CMFCVisualManagerWindows(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows7, CMFCVisualManager)
CMFCVisualManagerWindows7::CMFCVisualManagerWindows7() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerWindows7::~CMFCVisualManagerWindows7() {}
CObject* CMFCVisualManagerWindows7::CreateObject() { return new CMFCVisualManagerWindows7(); }
int CMFCVisualManagerWindows7::SetStyle(const wchar_t*) { return TRUE; }

IMPLEMENT_DYNAMIC(CMFCVisualManagerAero, CMFCVisualManager)
CMFCVisualManagerAero::CMFCVisualManagerAero() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerAero::~CMFCVisualManagerAero() {}

// Symbol: ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCVisualManagerOfficeXP__IEAA_H_Z(void* pThis, int) {
    return new(pThis) CMFCVisualManagerOfficeXP();
}

// Symbol: ?SetStyle@CMFCVisualManagerOffice2007@@SAHW4Style@1@PEB_W@Z
extern "C" int MS_ABI impl__SetStyle_CMFCVisualManagerOffice2007__SAHW4Style_1_PEB_W_Z(int style, const wchar_t* lpszPath) {
    return CMFCVisualManagerOffice2007::SetStyle((CMFCVisualManagerOffice2007::Style)style, lpszPath);
}

// Symbol: ?GetStyle@CMFCVisualManagerOffice2007@@SA?AW4Style@1@XZ
extern "C" int MS_ABI impl__GetStyle_CMFCVisualManagerOffice2007__SA_AW4Style_1_XZ() {
    return (int)CMFCVisualManagerOffice2007::GetStyle();
}

// Symbol: ?SetStyle@CMFCVisualManagerWindows7@@SAHPEB_W@Z
extern "C" int MS_ABI impl__SetStyle_CMFCVisualManagerWindows7__SAHPEB_W_Z(const wchar_t* lpszPath) {
    return CMFCVisualManagerWindows7::SetStyle(lpszPath);
}

// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}

// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}

// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) {
    pThis->CMFCVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
}

// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}

// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}

// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerWindows7__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, CMFCToolBarComboBoxButton* pButton) {
    pThis->CMFCVisualManager::OnDrawComboDropButton(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
}

// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerWindows@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerWindows__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}

// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerOffice2007@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOffice2007__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerOffice2007* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}

// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerWindows7@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerWindows7__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerWindows7* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect) {
    pThis->CMFCVisualManager::OnDrawMenuBorder(pDC, pMenu, rect);
}

//=============================================================================
// CBasePane
//=============================================================================
IMPLEMENT_DYNAMIC(CBasePane, CWnd)

CBasePane::CBasePane() {
    memset(_basepane_padding, 0, sizeof(_basepane_padding));
}
CBasePane::~CBasePane() {}

BOOL CBasePane::CanAcceptPane(const CBasePane*) const { return TRUE; }
BOOL CBasePane::CanAutoHide() const { return FALSE; }
BOOL CBasePane::CanBeAttached() const { return TRUE; }
BOOL CBasePane::CanBeClosed() const { return TRUE; }
BOOL CBasePane::CanBeDocked() const { return TRUE; }
BOOL CBasePane::CanBeFloating() const { return FALSE; }
BOOL CBasePane::CanBeResized() const { return TRUE; }
BOOL CBasePane::CanFloat() const { return FALSE; }
BOOL CBasePane::DoesAllowSiblingBars() const { return TRUE; }
BOOL CBasePane::HasGripper() const { return FALSE; }
BOOL CBasePane::IsAccessibilityCompatible() { return TRUE; }
void CBasePane::SetPaneAlignment(DWORD) {}
DWORD CBasePane::GetPaneAlignment() const { return 0; }
void* CBasePane::SetWindowPos(const CWnd*, int, int, int, int, unsigned int, void*) { return nullptr; }
void CBasePane::CalcFixedLayout(BOOL, BOOL) {}
void CBasePane::RecalcLayout() {}

//=============================================================================
// CPane
//=============================================================================
IMPLEMENT_DYNAMIC(CPane, CBasePane)

CPane::CPane() {
    memset(_pane_padding, 0, sizeof(_pane_padding));
}
CPane::~CPane() {}

BOOL CPane::CanBeDocked() const { return TRUE; }
BOOL CPane::CanFloat() const { return TRUE; }
void CPane::RecalcLayout() {}

//=============================================================================
// CDockablePane
//=============================================================================
IMPLEMENT_DYNAMIC(CDockablePane, CPane)

CDockablePane::CDockablePane() {
    memset(_dockablepane_padding, 0, sizeof(_dockablepane_padding));
}
CDockablePane::~CDockablePane() {}

BOOL CDockablePane::CanBeAttached() const { return TRUE; }
BOOL CDockablePane::CanAutoHide() const { return TRUE; }
void CDockablePane::EnableAutohideAll() {}
CMFCAutoHideBar* CDockablePane::SetAutoHideMode(int, unsigned long, void*, int) { return nullptr; }
// Old convenience overloads (non-MSDN API)
BOOL CDockablePane::IsAutoHideMode() const { return FALSE; }
BOOL CDockablePane::IsTabbed() const { return FALSE; }

//=============================================================================
// CMFCToolBarButton
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCToolBarButton, CObject)

CMFCToolBarButton::CMFCToolBarButton()
    : m_nID(0), m_iImage(-1), m_bUserButton(FALSE), m_bLocked(FALSE) {
    memset(_mfctoolbarbutton_padding, 0, sizeof(_mfctoolbarbutton_padding));
}

CMFCToolBarButton::CMFCToolBarButton(UINT uiCmdID, int iImage, const wchar_t* lpszText, BOOL bUserButton, BOOL bLocked)
    : m_nID(uiCmdID), m_iImage(iImage), m_bUserButton(bUserButton), m_bLocked(bLocked) {
    if (lpszText) m_strText = lpszText;
    memset(_mfctoolbarbutton_padding, 0, sizeof(_mfctoolbarbutton_padding));
}

CMFCToolBarButton::~CMFCToolBarButton() {}

//=============================================================================
// CMFCToolBar
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCToolBar, CBasePane)

CMFCToolBar::CMFCToolBar() {
    memset(_mfctoolbar_padding, 0, sizeof(_mfctoolbar_padding));
}
CMFCToolBar::~CMFCToolBar() {}

BOOL CMFCToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    m_hWnd = ::CreateWindowExW(0, L"ToolbarWindow32", nullptr,
                                dwStyle, 0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

BOOL CMFCToolBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    return Create(pParentWnd, dwStyle, nID);
}

BOOL CMFCToolBar::LoadToolBar(UINT, UINT, UINT, BOOL, UINT, UINT, UINT) { return TRUE; }
BOOL CMFCToolBar::LoadBitmap(UINT) { return TRUE; }
BOOL CMFCToolBar::SetButtons(const UINT*, int, BOOL) { return TRUE; }
BOOL CMFCToolBar::ReplaceButton(UINT, const CMFCToolBarButton&, BOOL) { return FALSE; }
int CMFCToolBar::GetCount() const { return 0; }
CMFCToolBarButton* CMFCToolBar::GetButton(int) const { return nullptr; }
void CMFCToolBar::SetSizes(SIZE, SIZE) {}
CSize CMFCToolBar::GetButtonSize() const { return CSize(23, 22); }
void CMFCToolBar::EnableDocking(DWORD) {}
void CMFCToolBar::AdjustLayout() {}
void CMFCToolBar::AdjustSize() {}
CString CMFCToolBar::GetButtonText(int) const { return CString(); }
void CMFCToolBar::GetButtonText(int, CString& rString) const { rString = CString(); }

// Symbol: ?LoadToolBar@CMFCToolBar@@UEAAHIIIHIII@Z
extern "C" int MS_ABI impl__LoadToolBar_CMFCToolBar__UEAAHIIIHIII_Z(
    CMFCToolBar* pThis, unsigned int p0, unsigned int p1, unsigned int p2,
    int p3, unsigned int p4, unsigned int p5, unsigned int p6) {
    return (int)pThis->LoadToolBar(p0, p1, p2, p3, p4, p5, p6);
}

// Symbol: ?SetButtons@CMFCToolBar@@UEAAHPEBIHH@Z
extern "C" int MS_ABI impl__SetButtons_CMFCToolBar__UEAAHPEBIHH_Z(
    CMFCToolBar* pThis, const unsigned int* p0, int p1, int p2) {
    return (int)pThis->SetButtons(p0, p1, p2);
}

// Symbol: ?SetSizes@CMFCToolBar@@SAXUtagSIZE@@0@Z
extern "C" void MS_ABI impl__SetSizes_CMFCToolBar__SAXUtagSIZE__0_Z(SIZE p0, SIZE p1) {
    CMFCToolBar::SetSizes(p0, p1);
}

// Symbol: ?EnableDocking@CMFCToolBar@@UEAAXK@Z
extern "C" void MS_ABI impl__EnableDocking_CMFCToolBar__UEAAXK_Z(CMFCToolBar* pThis, unsigned long p0) {
    pThis->EnableDocking(p0);
}

// Symbol: ?AdjustLayout@CMFCToolBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis) {
    pThis->AdjustLayout();
}

// Symbol: ?AdjustSize@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__AdjustSize_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis) {
    pThis->AdjustSize();
}

// Symbol: ?GetButtonText@CMFCToolBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetButtonText_CMFCToolBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CMFCToolBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetButtonText(nIndex));
}

// Symbol: ?GetButtonText@CMFCToolBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetButtonText_CMFCToolBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCToolBar* pThis, int nIndex, CString* pStr) {
    pThis->GetButtonText(nIndex, *pStr);
}

//=============================================================================
// CMFCRibbonBaseElement
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonBaseElement, CObject)

CMFCRibbonBaseElement::CMFCRibbonBaseElement() : m_nID(0) {
    memset(_ribbonbaseelement_padding, 0, sizeof(_ribbonbaseelement_padding));
}
CMFCRibbonBaseElement::~CMFCRibbonBaseElement() {}

UINT CMFCRibbonBaseElement::GetID() const { return m_nID; }
void CMFCRibbonBaseElement::SetID(UINT nID) { m_nID = nID; }
CString CMFCRibbonBaseElement::GetText() const { return m_strText; }
void CMFCRibbonBaseElement::SetText(const wchar_t* lpszText) { if(lpszText) m_strText = lpszText; }

//=============================================================================
// CMFCRibbonButton
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonButton, CMFCRibbonBaseElement)

CMFCRibbonButton::CMFCRibbonButton() {
    memset(_ribbonbutton_padding, 0, sizeof(_ribbonbutton_padding));
}
CMFCRibbonButton::CMFCRibbonButton(UINT nID, const wchar_t* lpszText, int, int, BOOL)
    : CMFCRibbonBaseElement() {
    SetID(nID);
    if (lpszText) SetText(lpszText);
    memset(_ribbonbutton_padding, 0, sizeof(_ribbonbutton_padding));
}
CMFCRibbonButton::CMFCRibbonButton(UINT nID, const wchar_t* lpszText, HICON, BOOL, HICON, BOOL, BOOL)
    : CMFCRibbonBaseElement() {
    SetID(nID);
    if (lpszText) SetText(lpszText);
    memset(_ribbonbutton_padding, 0, sizeof(_ribbonbutton_padding));
}
CMFCRibbonButton::~CMFCRibbonButton() {}

//=============================================================================
// CMFCRibbonPanel
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonPanel, CObject)

CMFCRibbonPanel::CMFCRibbonPanel(const wchar_t* lpszName, HICON hIcon)
    : m_hIcon(hIcon) {
    if (lpszName) m_strName = lpszName;
    memset(_ribbonpanel_padding, 0, sizeof(_ribbonpanel_padding));
}
CMFCRibbonPanel::~CMFCRibbonPanel() {}

void CMFCRibbonPanel::Add(CMFCRibbonBaseElement*) {}
void CMFCRibbonPanel::AddSeparator() {}
int CMFCRibbonPanel::GetCount() const { return 0; }
CMFCRibbonBaseElement* CMFCRibbonPanel::GetElement(int) const { return nullptr; }
CString CMFCRibbonPanel::GetName() const { return m_strName; }

//=============================================================================
// CMFCRibbonCategory
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonCategory, CObject)

CMFCRibbonCategory::CMFCRibbonCategory(const wchar_t* lpszName, UINT, UINT) {
    if (lpszName) m_strName = lpszName;
    memset(_ribboncategory_padding, 0, sizeof(_ribboncategory_padding));
}
CMFCRibbonCategory::~CMFCRibbonCategory() {}

CMFCRibbonPanel* CMFCRibbonCategory::AddPanel(const wchar_t*, HICON, CRuntimeClass*) { return nullptr; }
// Old convenience overloads (non-MSDN API)
void CMFCRibbonCategory::AddPanel(CMFCRibbonPanel*) {}
int CMFCRibbonCategory::GetPanelCount() const { return 0; }
CMFCRibbonPanel* CMFCRibbonCategory::GetPanel(int) const { return nullptr; }
CString CMFCRibbonCategory::GetName() const { return m_strName; }

//=============================================================================
// CMFCRibbonBar
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonBar, CPane)

CMFCRibbonBar::CMFCRibbonBar(BOOL) {
    memset(_ribbonbar_padding, 0, sizeof(_ribbonbar_padding));
}
CMFCRibbonBar::~CMFCRibbonBar() {}

BOOL CMFCRibbonBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    (void)pParentWnd; (void)dwStyle; (void)nID; return TRUE;
}
CMFCRibbonCategory* CMFCRibbonBar::AddCategory(const wchar_t*, unsigned int, unsigned int, CSize, unsigned int, int, CRuntimeClass*) { return nullptr; }
// Old convenience overloads (non-MSDN API)
BOOL CMFCRibbonBar::AddCategory(CMFCRibbonCategory*) { return TRUE; }
int CMFCRibbonBar::GetCategoryCount() const { return 0; }
CMFCRibbonCategory* CMFCRibbonBar::GetCategory(int) const { return nullptr; }
BOOL CMFCRibbonBar::SetActiveCategory(CMFCRibbonCategory*, BOOL) { return TRUE; }
void CMFCRibbonBar::AddToTabs(CMFCRibbonBaseElement*) {}
void CMFCRibbonBar::SetQuickAccessToolbar(CMFCRibbonButtonsGroup*) {}
void CMFCRibbonBar::ToggleMimimizeState() {}
BOOL CMFCRibbonBar::IsMinimized() const { return FALSE; }

//=============================================================================
// CMiniFrameWnd
//=============================================================================
IMPLEMENT_DYNAMIC(CMiniFrameWnd, CFrameWnd)

CMiniFrameWnd::CMiniFrameWnd() { memset(_pad, 0, sizeof(_pad)); }
CMiniFrameWnd::~CMiniFrameWnd() {}

//=============================================================================
// CMFCButton
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCButton, CButton)

CMFCButton::CMFCButton() {
    memset(_mfcbutton_padding, 0, sizeof(_mfcbutton_padding));
}
CMFCButton::~CMFCButton() {}

void CMFCButton::SetImage(HICON, BOOL, HICON, BOOL) {}
void CMFCButton::SetFaceColor(COLORREF, BOOL) {}
void CMFCButton::SetTextColor(COLORREF) {}
void CMFCButton::SetTextHotColor(COLORREF) {}
BOOL CMFCButton::IsOwnerDraw() const { return FALSE; }
void CMFCButton::EnableFullTextTooltip(BOOL) {}
void CMFCButton::SetTooltip(const wchar_t*) {}

//=============================================================================
// CMFCPopupMenu
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPopupMenu, CMiniFrameWnd)

static CMFCPopupMenu* g_pActivePopupMenu = nullptr;

CMFCPopupMenu::CMFCPopupMenu() {
    memset(_mfcpopupmenu_padding, 0, sizeof(_mfcpopupmenu_padding));
}
CMFCPopupMenu::~CMFCPopupMenu() {}

CMFCPopupMenu* CMFCPopupMenu::GetActiveMenu() { return g_pActivePopupMenu; }
void CMFCPopupMenu::SetForceMenuFocus(BOOL) {}
BOOL CMFCPopupMenu::Create(CWnd*, int, int, HMENU, BOOL, BOOL) { return TRUE; }

//=============================================================================
// CMFCBaseTabCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCBaseTabCtrl, CWnd)

CMFCBaseTabCtrl::CMFCBaseTabCtrl() {
    memset(_mfcbasetabctrl_padding, 0, sizeof(_mfcbasetabctrl_padding));
}
CMFCBaseTabCtrl::~CMFCBaseTabCtrl() {}

BOOL CMFCBaseTabCtrl::Create(DWORD, const RECT&, CWnd*, UINT) { return TRUE; }
int CMFCBaseTabCtrl::GetTabsCount() const { return 0; }
void CMFCBaseTabCtrl::AddTab(CWnd*, const wchar_t*, UINT) {}
int CMFCBaseTabCtrl::RemoveTab(int, int) { return 0; }
void CMFCBaseTabCtrl::SetActiveTab(int) {}
int CMFCBaseTabCtrl::GetActiveTab() const { return -1; }
CWnd* CMFCBaseTabCtrl::GetTabWnd(int) const { return nullptr; }

//=============================================================================
// CMFCTabCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCTabCtrl, CMFCBaseTabCtrl)

CMFCTabCtrl::CMFCTabCtrl() {
    memset(_mfctabctrl_padding, 0, sizeof(_mfctabctrl_padding));
}
CMFCTabCtrl::~CMFCTabCtrl() {}

void CMFCTabCtrl::EnableTabSwap(BOOL) {}
void CMFCTabCtrl::SetTabBorderSize(int) {}
void CMFCTabCtrl::SetTabsHeight(int) {}

//=============================================================================
// CMFCPropertyGridProperty
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPropertyGridProperty, CObject)

CMFCPropertyGridProperty::CMFCPropertyGridProperty(const wchar_t* lpszName, const COleVariant& varValue,
                                                     const wchar_t* lpszDescr, DWORD_PTR dwData)
    : m_varValue(varValue), m_dwData(dwData),
      m_bModified(FALSE), m_bEnabled(TRUE), m_bVisible(TRUE), m_bExpanded(FALSE) {
    if (lpszName) m_strName = lpszName;
    if (lpszDescr) m_strDescr = lpszDescr;
    memset(_propgridproperty_padding, 0, sizeof(_propgridproperty_padding));
}
CMFCPropertyGridProperty::~CMFCPropertyGridProperty() {}

const CString& CMFCPropertyGridProperty::GetName() const { return m_strName; }
const COleVariant& CMFCPropertyGridProperty::GetValue() const { return m_varValue; }
void CMFCPropertyGridProperty::SetValue(const COleVariant& varValue) { m_varValue = varValue; }
BOOL CMFCPropertyGridProperty::IsModified() const { return m_bModified; }
void CMFCPropertyGridProperty::SetModified(BOOL bModified) { m_bModified = bModified; }
BOOL CMFCPropertyGridProperty::IsEnabled() const { return m_bEnabled; }
void CMFCPropertyGridProperty::SetEnabled(BOOL bEnable) { m_bEnabled = bEnable; }
BOOL CMFCPropertyGridProperty::IsVisible() const { return m_bVisible; }
void CMFCPropertyGridProperty::Show(BOOL bShow) { m_bVisible = bShow; }
int CMFCPropertyGridProperty::AddSubItem(CMFCPropertyGridProperty*) { return 0; }
int CMFCPropertyGridProperty::GetSubItemsCount() const { return 0; }
CMFCPropertyGridProperty* CMFCPropertyGridProperty::GetSubItem(int) const { return nullptr; }
void CMFCPropertyGridProperty::RemoveAllSubItems() {}
void CMFCPropertyGridProperty::Expand(BOOL bExpand) { m_bExpanded = bExpand; }
BOOL CMFCPropertyGridProperty::IsExpanded() const { return m_bExpanded; }
void CMFCPropertyGridProperty::SetData(DWORD_PTR dwData) { m_dwData = dwData; }
DWORD_PTR CMFCPropertyGridProperty::GetData() const { return m_dwData; }

//=============================================================================
// CMFCPropertyGridCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPropertyGridCtrl, CWnd)

CMFCPropertyGridCtrl::CMFCPropertyGridCtrl() {
    memset(_propgridctrl_padding, 0, sizeof(_propgridctrl_padding));
}
CMFCPropertyGridCtrl::~CMFCPropertyGridCtrl() {}

BOOL CMFCPropertyGridCtrl::Create(DWORD, const RECT&, CWnd*, UINT) { return TRUE; }
int CMFCPropertyGridCtrl::AddProperty(CMFCPropertyGridProperty*, int, int) { return 0; }
int CMFCPropertyGridCtrl::GetPropertyCount() const { return 0; }
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::GetProperty(int) const { return nullptr; }
void CMFCPropertyGridCtrl::RemoveAll() {}
void CMFCPropertyGridCtrl::ExpandAll(BOOL) {}
void CMFCPropertyGridCtrl::AdjustLayout() {}
void CMFCPropertyGridCtrl::SetDescriptionRows(int) {}

//=============================================================================
// CMFCTasksPaneTask
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCTasksPaneTask, CObject)

CMFCTasksPaneTask::CMFCTasksPaneTask(UINT nID, const wchar_t*, int, BOOL) {
    (void)nID;
    memset(_taskspanetask_padding, 0, sizeof(_taskspanetask_padding));
}
CMFCTasksPaneTask::~CMFCTasksPaneTask() {}

//=============================================================================
// CMFCTasksPane
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCTasksPane, CBasePane)

CMFCTasksPane::CMFCTasksPane() {
    memset(_taskspane_padding, 0, sizeof(_taskspane_padding));
}
CMFCTasksPane::~CMFCTasksPane() {}

BOOL CMFCTasksPane::Create(DWORD, const RECT&, CWnd*, UINT) { return TRUE; }
int CMFCTasksPane::AddTask(int, const wchar_t*, int, unsigned int, unsigned __int64) { return 0; }
// Old convenience overloads (non-MSDN API)
void CMFCTasksPane::AddTask(int, CMFCTasksPaneTask*) {}
void CMFCTasksPane::RemoveAllTasks() {}
void CMFCTasksPane::SetCaption(int, const wchar_t*) {}
BOOL CMFCTasksPane::SetGroupName(int, const wchar_t*) { return TRUE; }
int CMFCTasksPane::AddGroup(const wchar_t*, BOOL, BOOL, int) { return 0; }

//=============================================================================
// CPaneFrameWnd
//=============================================================================
IMPLEMENT_DYNAMIC(CPaneFrameWnd, CMiniFrameWnd)

CPaneFrameWnd::CPaneFrameWnd() {
    memset(_paneframewnd_padding, 0, sizeof(_paneframewnd_padding));
}
CPaneFrameWnd::~CPaneFrameWnd() {}

BOOL CPaneFrameWnd::Create(CWnd*, DWORD, UINT) { return TRUE; }
void CPaneFrameWnd::AddPane(CBasePane*) {}
void CPaneFrameWnd::RemovePane(CBasePane*, BOOL) {}

//=============================================================================
// CDockingManager
//=============================================================================
IMPLEMENT_DYNAMIC(CDockingManager, CObject)

CDockingManager::CDockingManager()
    : m_pParentWnd(nullptr) {
    memset(_dockingmanager_padding, 0, sizeof(_dockingmanager_padding));
}

CDockingManager::CDockingManager(CFrameWnd* pParentFrameWnd)
    : m_pParentWnd(pParentFrameWnd) {
    memset(_dockingmanager_padding, 0, sizeof(_dockingmanager_padding));
}
CDockingManager::~CDockingManager() {}

void CDockingManager::DockPane(CBasePane*, UINT, LPCRECT) {}
void CDockingManager::DockPaneLeftOf(CBasePane*, CBasePane*) {}
void CDockingManager::EnableDocking(DWORD) {}
void CDockingManager::FloatPane(CBasePane*, CPoint, DWORD) {}
void CDockingManager::HidePane(CBasePane*) {}
void CDockingManager::ShowPane(CBasePane*, BOOL) {}
void CDockingManager::RecalcLayout() {}
void CDockingManager::SetDockState() {}

//=============================================================================
// CFrameWndEx
//=============================================================================
IMPLEMENT_DYNAMIC(CFrameWndEx, CFrameWnd)

CFrameWndEx::CFrameWndEx()
    : m_pVisualManager(nullptr), m_pDockingManager(nullptr) {
    memset(_framewndex_padding, 0, sizeof(_framewndex_padding));
}
CFrameWndEx::~CFrameWndEx() {
    if (m_pDockingManager) delete m_pDockingManager;
}

BOOL CFrameWndEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
                          const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName,
                          DWORD dwExStyle, CCreateContext* pContext) {
    return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect,
                             pParentWnd, lpszMenuName, dwExStyle, pContext);
}
BOOL CFrameWndEx::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}
CMFCVisualManager* CFrameWndEx::GetVisualManager() const { return m_pVisualManager; }
CDockingManager* CFrameWndEx::GetDockingManager() {
    if (!m_pDockingManager) m_pDockingManager = new CDockingManager(this);
    return m_pDockingManager;
}
void CFrameWndEx::EnableDocking(DWORD dwDockStyle) {
    if (!m_pDockingManager) m_pDockingManager = new CDockingManager(this);
    m_pDockingManager->EnableDocking(dwDockStyle);
}

//=============================================================================
// CMDIFrameWndEx
//=============================================================================
IMPLEMENT_DYNAMIC(CMDIFrameWndEx, CMDIFrameWnd)

CMDIFrameWndEx::CMDIFrameWndEx()
    : m_pVisualManager(nullptr) {
    memset(_mdiframewndex_padding, 0, sizeof(_mdiframewndex_padding));
}
CMDIFrameWndEx::~CMDIFrameWndEx() {}

CMFCVisualManager* CMDIFrameWndEx::GetVisualManager() const { return m_pVisualManager; }

//=============================================================================
// CMDIChildWndEx
//=============================================================================
IMPLEMENT_DYNAMIC(CMDIChildWndEx, CMDIChildWnd)

CMDIChildWndEx::CMDIChildWndEx() {
    memset(_mdichildwndex_padding, 0, sizeof(_mdichildwndex_padding));
}
CMDIChildWndEx::~CMDIChildWndEx() {}

//=============================================================================
// Forward type stubs (DECLARE_DYNAMIC classes defined inline in afxmfc.h)
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCToolBarComboBoxButton, CMFCToolBarButton)
CMFCToolBarComboBoxButton::CMFCToolBarComboBoxButton() { memset(_pad, 0, sizeof(_pad)); }
CMFCToolBarComboBoxButton::~CMFCToolBarComboBoxButton() {}

IMPLEMENT_DYNAMIC(CMFCToolBarEditBoxButton, CMFCToolBarButton)
CMFCToolBarEditBoxButton::CMFCToolBarEditBoxButton() { memset(_pad, 0, sizeof(_pad)); }
CMFCToolBarEditBoxButton::~CMFCToolBarEditBoxButton() {}

IMPLEMENT_DYNAMIC(CMFCToolBarMenuButton, CMFCToolBarButton)
CMFCToolBarMenuButton::CMFCToolBarMenuButton() : m_hMenu(nullptr) { memset(_pad, 0, sizeof(_pad)); }
CMFCToolBarMenuButton::~CMFCToolBarMenuButton() {}

IMPLEMENT_DYNAMIC(CMFCStatusBar, CStatusBar)
CMFCStatusBar::CMFCStatusBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCStatusBar::~CMFCStatusBar() {}

IMPLEMENT_DYNAMIC(CMFCRibbonButtonsGroup, CMFCRibbonBaseElement)
CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonButtonsGroup::~CMFCRibbonButtonsGroup() {}

IMPLEMENT_DYNAMIC(CMFCRibbonCaptionButton, CMFCRibbonBaseElement)
CMFCRibbonCaptionButton::CMFCRibbonCaptionButton() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonCaptionButton::~CMFCRibbonCaptionButton() {}

IMPLEMENT_DYNAMIC(CMFCRibbonCategoryScroll, CMFCRibbonBaseElement)
CMFCRibbonCategoryScroll::CMFCRibbonCategoryScroll() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonCategoryScroll::~CMFCRibbonCategoryScroll() {}

IMPLEMENT_DYNAMIC(CMFCRibbonTab, CMFCRibbonBaseElement)
CMFCRibbonTab::CMFCRibbonTab() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonTab::~CMFCRibbonTab() {}

IMPLEMENT_DYNAMIC(CMFCRibbonCheckBox, CMFCRibbonBaseElement)
CMFCRibbonCheckBox::CMFCRibbonCheckBox() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonCheckBox::~CMFCRibbonCheckBox() {}

IMPLEMENT_DYNAMIC(CMFCRibbonEdit, CMFCRibbonBaseElement)
CMFCRibbonEdit::CMFCRibbonEdit() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonEdit::~CMFCRibbonEdit() {}

IMPLEMENT_DYNAMIC(CMFCRibbonGallery, CMFCRibbonBaseElement)
CMFCRibbonGallery::CMFCRibbonGallery() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonGallery::~CMFCRibbonGallery() {}

IMPLEMENT_DYNAMIC(CMFCRibbonLabel, CMFCRibbonBaseElement)
CMFCRibbonLabel::CMFCRibbonLabel() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonLabel::~CMFCRibbonLabel() {}

IMPLEMENT_DYNAMIC(CMFCRibbonMainPanel, CMFCRibbonBaseElement)
CMFCRibbonMainPanel::CMFCRibbonMainPanel() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonMainPanel::~CMFCRibbonMainPanel() {}

IMPLEMENT_DYNAMIC(CMFCRibbonProgressBar, CMFCRibbonBaseElement)
CMFCRibbonProgressBar::CMFCRibbonProgressBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonProgressBar::~CMFCRibbonProgressBar() {}

IMPLEMENT_DYNAMIC(CMFCRibbonSeparator, CMFCRibbonBaseElement)
CMFCRibbonSeparator::CMFCRibbonSeparator() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonSeparator::~CMFCRibbonSeparator() {}

IMPLEMENT_DYNAMIC(CMFCRibbonSlider, CMFCRibbonBaseElement)
CMFCRibbonSlider::CMFCRibbonSlider() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonSlider::~CMFCRibbonSlider() {}

IMPLEMENT_DYNAMIC(CMFCRibbonStatusBar, CBasePane)
CMFCRibbonStatusBar::CMFCRibbonStatusBar() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonStatusBar::~CMFCRibbonStatusBar() {}

IMPLEMENT_DYNAMIC(CMFCRibbonStatusBarPane, CMFCRibbonBaseElement)
CMFCRibbonStatusBarPane::CMFCRibbonStatusBarPane() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonStatusBarPane::~CMFCRibbonStatusBarPane() {}

IMPLEMENT_DYNAMIC(CMFCTasksPaneTaskGroup, CObject)
CMFCTasksPaneTaskGroup::CMFCTasksPaneTaskGroup() { memset(_pad, 0, sizeof(_pad)); }
CMFCTasksPaneTaskGroup::~CMFCTasksPaneTaskGroup() {}
