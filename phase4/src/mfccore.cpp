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

CMFCVisualManager::CMFCVisualManager() {
    memset(_visualmanager_padding, 0, sizeof(_visualmanager_padding));
}

CMFCVisualManager::~CMFCVisualManager() {}

CMFCVisualManager* CMFCVisualManager::GetInstance() {
    if (!g_pVisualManager) g_pVisualManager = new CMFCVisualManager();
    return g_pVisualManager;
}

void CMFCVisualManager::SetDefaultManager(CRuntimeClass* pRTI) { (void)pRTI; }

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
void CMFCVisualManager::OnDrawPaneCaption(CDC*, CDockablePane*, CRect) {}
void CMFCVisualManager::OnDrawPaneDivider(CDC*, CPaneDivider*, CRect, BOOL) {}
void CMFCVisualManager::OnDrawPopupWindowBorder(CDC*, CRect) {}
void CMFCVisualManager::OnDrawPopupWindowButtonBorder(CDC*, CRect, CMFCDesktopAlertWndButton*) {}
void CMFCVisualManager::OnDrawPopupWindowCaption(CDC*, CRect, CMFCDesktopAlertWnd*) {}
void CMFCVisualManager::OnDrawRibbonApplicationButton(CDC*, CMFCRibbonButton*) {}
void CMFCVisualManager::OnDrawRibbonButtonBorder(CDC*, CMFCRibbonButton*) {}
void CMFCVisualManager::OnDrawRibbonButtonsGroup(CDC*, CMFCRibbonButtonsGroup*) {}
void CMFCVisualManager::OnDrawRibbonCaption(CDC*, CMFCRibbonBar*, CRect, CRect) {}
void CMFCVisualManager::OnDrawRibbonCaptionButton(CDC*, CMFCRibbonCaptionButton*) {}
void CMFCVisualManager::OnDrawRibbonCategory(CDC*, CMFCRibbonCategory*, CRect) {}
void CMFCVisualManager::OnDrawRibbonCategoryCaption(CDC*, CMFCRibbonCategory*) {}
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

COLORREF CMFCVisualManager::GetHighlightedColor(UINT) const { return RGB(0, 120, 215); }
COLORREF CMFCVisualManager::GetThemeColor(COLORREF, int) { return RGB(0, 0, 0); }

// Office variant visual managers
IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2003, CMFCVisualManager)
CMFCVisualManagerOffice2003::CMFCVisualManagerOffice2003() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerOffice2003::~CMFCVisualManagerOffice2003() {}

IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2007, CMFCVisualManager)
CMFCVisualManagerOffice2007::CMFCVisualManagerOffice2007() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerOffice2007::~CMFCVisualManagerOffice2007() {}

IMPLEMENT_DYNAMIC(CMFCVisualManagerOfficeXP, CMFCVisualManager)
CMFCVisualManagerOfficeXP::CMFCVisualManagerOfficeXP() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerOfficeXP::~CMFCVisualManagerOfficeXP() {}

IMPLEMENT_DYNAMIC(CMFCVisualManagerVS2005, CMFCVisualManager)
CMFCVisualManagerVS2005::CMFCVisualManagerVS2005() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005() {}

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows, CMFCVisualManager)
CMFCVisualManagerWindows::CMFCVisualManagerWindows() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerWindows::~CMFCVisualManagerWindows() {}

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows7, CMFCVisualManager)
CMFCVisualManagerWindows7::CMFCVisualManagerWindows7() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerWindows7::~CMFCVisualManagerWindows7() {}

IMPLEMENT_DYNAMIC(CMFCVisualManagerAero, CMFCVisualManager)
CMFCVisualManagerAero::CMFCVisualManagerAero() { memset(_pad, 0, sizeof(_pad)); }
CMFCVisualManagerAero::~CMFCVisualManagerAero() {}

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
void CBasePane::SetWindowPos(CWnd*, int, int, int, int, UINT) {}
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
void CDockablePane::SetAutoHideMode(BOOL, DWORD) {}
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

BOOL CMFCToolBar::LoadToolBar(UINT) { return TRUE; }
BOOL CMFCToolBar::LoadBitmap(UINT) { return TRUE; }
BOOL CMFCToolBar::SetButtons(const UINT*, int) { return TRUE; }
BOOL CMFCToolBar::ReplaceButton(UINT, const CMFCToolBarButton&, BOOL) { return FALSE; }
int CMFCToolBar::GetCount() const { return 0; }
CMFCToolBarButton* CMFCToolBar::GetButton(int) const { return nullptr; }
void CMFCToolBar::SetSizes(SIZE, SIZE) {}
CSize CMFCToolBar::GetButtonSize() const { return CSize(23, 22); }

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
void CMFCBaseTabCtrl::RemoveTab(int) {}
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
void CMFCPropertyGridProperty::AddSubItem(CMFCPropertyGridProperty*) {}
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
void CMFCPropertyGridCtrl::AddProperty(CMFCPropertyGridProperty*) {}
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
