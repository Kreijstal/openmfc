#include "MfccoreSupport.h"

IMPLEMENT_DYNAMIC(CMFCVisualManager, CObject)
alignas(CFont) unsigned char g_menuFontStorage[sizeof(CFont)] = {};
IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2003, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2007, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CMFCVisualManagerOfficeXP, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CMFCVisualManagerVS2005, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows7, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CMFCVisualManagerAero, CMFCVisualManager)
IMPLEMENT_DYNAMIC(CBasePane, CWnd)
IMPLEMENT_DYNAMIC(CPane, CBasePane)
IMPLEMENT_DYNAMIC(CMFCAutoHideBar, CPane)
IMPLEMENT_DYNAMIC(CDockablePane, CPane)
IMPLEMENT_DYNAMIC(CBaseTabbedPane, CDockablePane)
IMPLEMENT_SERIAL(CTabbedPane, CBaseTabbedPane, 0x80000002)
IMPLEMENT_SERIAL(CUserTool, CObject, 0x80000001)
IMPLEMENT_DYNAMIC(CUserToolsManager, CObject)
IMPLEMENT_DYNAMIC(CMFCToolBarButton, CObject)
IMPLEMENT_DYNAMIC(CMFCBaseToolBar, CPane)
IMPLEMENT_DYNAMIC(CMFCToolBar, CMFCBaseToolBar)
IMPLEMENT_DYNAMIC(CMFCMenuBar, CMFCToolBar)
IMPLEMENT_DYNAMIC(CMFCRibbonBaseElement, CObject)
IMPLEMENT_DYNAMIC(CMFCRibbonButton, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonPanel, CObject)
IMPLEMENT_DYNAMIC(CMFCRibbonCategory, CObject)
IMPLEMENT_DYNAMIC(CMFCRibbonBar, CPane)
static_assert(sizeof(CMFCRibbonBar) == 8400, "CMFCRibbonBar sizeof 8400");
static_assert(offsetof(CMFCRibbonBar, m_nTabsHeight) == 1016, "m_nTabsHeight @1016");
static_assert(offsetof(CMFCRibbonBar, m_nHighlightedTab) == 1028, "m_nHighlightedTab @1028");
static_assert(offsetof(CMFCRibbonBar, m_nKeyboardNavLevel) == 1044, "m_nKeyboardNavLevel @1044");
static_assert(offsetof(CMFCRibbonBar, m_nTooltipWidthRegular) == 1052, "m_nTooltipWidthRegular @1052");
static_assert(offsetof(CMFCRibbonBar, m_nTooltipWidthLargeImage) == 1056, "m_nTooltipWidthLargeImage @1056");
static_assert(offsetof(CMFCRibbonBar, m_bRecalcCategoryHeight) == 1060, "m_bRecalcCategoryHeight @1060");
static_assert(offsetof(CMFCRibbonBar, m_bQuickAccessToolbarOnTop) == 1076, "m_bQuickAccessToolbarOnTop @1076");
static_assert(offsetof(CMFCRibbonBar, m_bMaximizeMode) == 1084, "m_bMaximizeMode @1084");
static_assert(offsetof(CMFCRibbonBar, m_bReplaceFrameCaption) == 1128, "m_bReplaceFrameCaption @1128");
static_assert(offsetof(CMFCRibbonBar, m_hFont) == 1136, "m_hFont @1136");
static_assert(offsetof(CMFCRibbonBar, m_dwHideFlags) == 1144, "m_dwHideFlags @1144");
static_assert(offsetof(CMFCRibbonBar, m_pMainButton) == 1152, "m_pMainButton @1152");
static_assert(offsetof(CMFCRibbonBar, m_TabElements) == 1176, "m_TabElements @1176");
static_assert(offsetof(CMFCRibbonBar, m_pActiveCategory) == 2824, "m_pActiveCategory @2824");
static_assert(offsetof(CMFCRibbonBar, m_arContextCaptions) == 2856, "m_arContextCaptions @2856");
static_assert(offsetof(CMFCRibbonBar, m_arCategories) == 2896, "m_arCategories @2896");
static_assert(offsetof(CMFCRibbonBar, m_rectCaption) == 3016, "m_rectCaption @3016");
static_assert(offsetof(CMFCRibbonBar, m_sizeMainButton) == 3064, "m_sizeMainButton @3064");
static_assert(offsetof(CMFCRibbonBar, m_CaptionButtons) == 3096, "m_CaptionButtons @3096");
static_assert(offsetof(CMFCRibbonBar, m_QAToolbar) == 4992, "m_QAToolbar @4992");
static_assert(offsetof(CMFCRibbonBar, m_Tabs) == 6744, "m_Tabs @6744");
static_assert(offsetof(CMFCRibbonBar, m_bWindows7Look) == 8392, "m_bWindows7Look @8392");
IMPLEMENT_DYNAMIC(CMiniFrameWnd, CFrameWnd)
IMPLEMENT_DYNAMIC(CMFCButton, CButton)
IMPLEMENT_DYNAMIC(CMFCPopupMenu, CMiniFrameWnd)
IMPLEMENT_DYNAMIC(CMFCPopupMenuBar, CMFCToolBar)
IMPLEMENT_DYNAMIC(CMFCBaseTabCtrl, CWnd)
IMPLEMENT_DYNAMIC(CMFCTabCtrl, CMFCBaseTabCtrl)
void SetPropertyGridOwnerRecursive(CMFCPropertyGridProperty* pProp, CMFCPropertyGridCtrl* pOwner) {
    if (!pProp) return;
    auto& state = EnsurePropertyGridPropertyState(pProp);
    state.owner = pOwner;
    for (CMFCPropertyGridProperty* child : state.subItems) {
        SetPropertyGridOwnerRecursive(child, pOwner);
    }
}
IMPLEMENT_DYNAMIC(CMFCPropertyGridProperty, CObject)
IMPLEMENT_DYNAMIC(CMFCPropertyGridCtrl, CWnd)
IMPLEMENT_DYNAMIC(CMFCTasksPaneTask, CObject)
IMPLEMENT_DYNAMIC(CMFCTasksPane, CBasePane)
IMPLEMENT_DYNAMIC(CPaneFrameWnd, CMiniFrameWnd)
IMPLEMENT_DYNAMIC(CDockingManager, CObject)
IMPLEMENT_DYNAMIC(CFrameWndEx, CFrameWnd)
IMPLEMENT_DYNAMIC(CMDIFrameWndEx, CMDIFrameWnd)
IMPLEMENT_DYNAMIC(CMDIChildWndEx, CMDIChildWnd)
IMPLEMENT_DYNAMIC(CMFCToolBarComboBoxButton, CMFCToolBarButton)
IMPLEMENT_DYNAMIC(CMFCToolBarEditBoxButton, CMFCToolBarButton)
IMPLEMENT_DYNAMIC(CMFCToolBarMenuButton, CMFCToolBarButton)
IMPLEMENT_DYNAMIC(CMFCToolBarMenuButtonsButton, CMFCToolBarMenuButton)
IMPLEMENT_DYNAMIC(CContextMenuManager, CObject)
IMPLEMENT_DYNAMIC(CKeyboardManager, CObject)
IMPLEMENT_DYNAMIC(CTooltipManager, CObject)
IMPLEMENT_DYNAMIC(CMenuHash, CObject)
IMPLEMENT_DYNAMIC(CWinAppEx, CWinApp)
IMPLEMENT_DYNAMIC(CMFCStatusBar, CStatusBar)
IMPLEMENT_DYNAMIC(CMFCRibbonButtonsGroup, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonCaptionButton, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonCategoryScroll, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonTab, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonContextCaption, CObject)
IMPLEMENT_DYNAMIC(CMFCRibbonCheckBox, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonEdit, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonGallery, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonLabel, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonMainPanel, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonProgressBar, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonSeparator, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonSlider, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCRibbonStatusBar, CBasePane)
IMPLEMENT_DYNAMIC(CMFCRibbonStatusBarPane, CMFCRibbonBaseElement)
IMPLEMENT_DYNAMIC(CMFCTasksPaneTaskGroup, CObject)
namespace openmfc { namespace detail { namespace mfccore {
CMFCVisualManager* g_pVisualManager = nullptr;
CRuntimeClass* g_pDefaultVisualManagerClass = RUNTIME_CLASS(CMFCVisualManager);
decltype(openmfc::docking_state::PaneCoreStateMutex()) g_paneCoreStateMutex_Mfccore = openmfc::docking_state::PaneCoreStateMutex();
decltype(openmfc::docking_state::PaneCoreStates()) g_paneCoreState_Mfccore = openmfc::docking_state::PaneCoreStates();
thread_local std::unordered_map<const CDockingManager*, DockingManagerState> g_dockingStates;
std::mutex g_dockingProfileMutex;
std::unordered_map<const CDockingManager*,
                          std::unordered_map<std::wstring, DockingManagerSnapshot>>
    g_dockingProfileStates;
DockingManagerState& EnsureDockingState(const CDockingManager* pManager) {
    return g_dockingStates[pManager];
}
const DockingManagerState* FindDockingState(const CDockingManager* pManager) {
    auto it = g_dockingStates.find(pManager);
    return it == g_dockingStates.end() ? nullptr : &it->second;
}
void RemoveDockingState(const CDockingManager* pManager) {
    g_dockingStates.erase(pManager);
}
std::wstring DockingProfileName(const wchar_t* section) {
    return section && *section ? std::wstring(section) : std::wstring(L"DockingManager");
}
DockingManagerSnapshot* FindDockingProfileSnapshot(const CDockingManager* pManager, const wchar_t* section) {
    auto it = g_dockingProfileStates.find(pManager);
    if (it == g_dockingProfileStates.end()) return nullptr;

    const std::wstring profile = DockingProfileName(section);
    auto profileIt = it->second.find(profile);
    if (profileIt == it->second.end()) return nullptr;
    return &profileIt->second;
}
void AddDockingPane(CDockingManager* pManager, CBasePane* pPane, CBasePane* pInsertAfter) {
    if (!pManager || !pPane) return;

    DockingManagerState& state = EnsureDockingState(pManager);
    if (ContainsPtr(state.panes, pPane)) return;

    if (pInsertAfter) {
        auto it = std::find(state.panes.begin(), state.panes.end(), pInsertAfter);
        if (it != state.panes.end()) {
            state.panes.insert(std::next(it), pPane);
            return;
        }
    }

    state.panes.push_back(pPane);
}
void RemoveDockingPane(CDockingManager* pManager, CBasePane* pPane) {
    if (!pManager || !pPane) return;

    DockingManagerState& state = EnsureDockingState(pManager);
    state.panes.erase(std::remove(state.panes.begin(), state.panes.end(), pPane), state.panes.end());
    state.hiddenPanes.erase(pPane);
    state.floatingPanes.erase(pPane);
}
bool IsPaneVisibleForDocking(const DockingManagerState& state, CBasePane* pPane) {
    return pPane != nullptr && state.hiddenPanes.find(pPane) == state.hiddenPanes.end();
}
int MiniFrameZRank(HWND hWnd) {
    if (!hWnd) return INT_MAX;
    int rank = 0;
    for (HWND current = ::GetTopWindow(nullptr); current; current = ::GetWindow(current, GW_HWNDNEXT), ++rank) {
        if (current == hWnd) return rank;
    }
    return INT_MAX;
}
CRect DefaultMiniFrameRect(int index) {
    constexpr int kDockingMiniFrameWidth = 240;
    constexpr int kDockingMiniFrameHeight = 180;
    constexpr int kDockingMiniFrameSpacing = 18;
    int offset = index * kDockingMiniFrameSpacing;
    return CRect(offset, offset, offset + kDockingMiniFrameWidth, offset + kDockingMiniFrameHeight);
}
CBasePane* FirstDockingPane(const CDockingManager* pManager, bool visibleOnly) {
    const DockingManagerState* state = FindDockingState(pManager);
    if (!state) return nullptr;

    for (CBasePane* pane : state->panes) {
        if (!visibleOnly || IsPaneVisibleForDocking(*state, pane)) {
            return pane;
        }
    }

    return nullptr;
}
UINT PaneCommandID(const CBasePane* pPane) {
    if (!pPane) return 0;
    HWND hwnd = pPane->GetSafeHwnd();
    if (!hwnd) return 0;
    return static_cast<UINT>(::GetDlgCtrlID(hwnd));
}
thread_local std::unordered_map<const CMFCPopupMenu*, PopupMenuState> g_popupMenuStates;
thread_local std::unordered_map<const CMFCMenuBar*, MenuBarState> g_menuBarStates;
thread_local std::unordered_map<const CMFCToolBar*, ToolBarState> g_toolBarStates;
decltype(openmfc::ribbon_state::RibbonPanelStates()) g_ribbonPanelStates = openmfc::ribbon_state::RibbonPanelStates();
decltype(openmfc::ribbon_state::RibbonCategoryStates()) g_ribbonCategoryStates = openmfc::ribbon_state::RibbonCategoryStates();
decltype(openmfc::ribbon_state::RibbonBarStates()) g_ribbonBarStates = openmfc::ribbon_state::RibbonBarStates();
thread_local std::unordered_map<const CMFCPopupMenuBar*, PopupMenuBarState> g_popupMenuBarStates;
thread_local std::unordered_map<const CMFCToolBarMenuButton*, MenuButtonState> g_menuButtonStates;
thread_local std::unordered_map<const CContextMenuManager*, ContextMenuState> g_contextMenuStates;
thread_local std::unordered_map<const CTooltipManager*, TooltipManagerState> g_tooltipManagerStates;
thread_local std::unordered_map<const CMFCButton*, MFCButtonState> g_mfcButtonStates;
thread_local std::unordered_map<const CMenuHash*, MenuHashState> g_menuHashStates;
thread_local std::unordered_map<const CMFCBaseTabCtrl*, TabCtrlState> g_tabCtrlStates;
std::unordered_map<const CWinAppEx*, WinAppExState> g_winAppExStates;
int g_mouseManagerToken = 0;
int g_shellManagerToken = 0;
CUserToolsManager g_userToolsManager;
BOOL g_forceMenuFocus = FALSE;
BOOL g_showAllAccelerators = FALSE;
CUserToolsManager* afxUserToolsManager = &g_userToolsManager;
CFont* g_menuFontPtr = nullptr;
COLORREF g_visualAccentColor = RGB(0, 120, 215);
COLORREF g_visualBackgroundColor = RGB(240, 240, 240);
COLORREF g_visualBorderColor = RGB(160, 160, 160);
void ClearPopupMenuState(const CMFCPopupMenu* pMenu) {
    auto it = g_popupMenuStates.find(pMenu);
    if (it == g_popupMenuStates.end()) return;
    for (CMFCToolBarMenuButton* item : it->second.ownedItems) {
        delete item;
    }
    g_popupMenuStates.erase(it);
}
void ClearMenuBarState(const CMFCMenuBar* pMenuBar) {
    auto it = g_menuBarStates.find(pMenuBar);
    if (it == g_menuBarStates.end()) return;
    for (CMFCToolBarButton* item : it->second.ownedItems) {
        delete item;
    }
    g_menuBarStates.erase(it);
}
void ClearToolBarButtons(ToolBarState& state) {
    for (CMFCToolBarButton* button : state.ownedButtons) {
        if (button && button->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
            g_menuButtonStates.erase(static_cast<CMFCToolBarMenuButton*>(button));
        }
        delete button;
    }
    state.buttons.clear();
    state.ownedButtons.clear();
}
void ClearToolBarState(const CMFCToolBar* pToolBar) {
    auto it = g_toolBarStates.find(pToolBar);
    if (it == g_toolBarStates.end()) return;
    ClearToolBarButtons(it->second);
    g_toolBarStates.erase(it);
}
MFCButtonState& EnsureMFCButtonState(const CMFCButton* pButton) {
    return g_mfcButtonStates[pButton];
}
void RemoveMFCButtonState(const CMFCButton* pButton) {
    auto it = g_mfcButtonStates.find(pButton);
    if (it == g_mfcButtonStates.end()) return;
    if (it->second.icon && it->second.ownsIcon) {
        ::DestroyIcon(it->second.icon);
    }
    if (it->second.iconHot && it->second.ownsHotIcon) {
        ::DestroyIcon(it->second.iconHot);
    }
    g_mfcButtonStates.erase(it);
}
TabCtrlState& EnsureTabCtrlState(const CMFCBaseTabCtrl* pCtrl) {
    return g_tabCtrlStates[pCtrl];
}
const TabCtrlState* FindTabCtrlState(const CMFCBaseTabCtrl* pCtrl) {
    auto it = g_tabCtrlStates.find(pCtrl);
    return it == g_tabCtrlStates.end() ? nullptr : &it->second;
}
void RemoveTabCtrlState(const CMFCBaseTabCtrl* pCtrl) {
    g_tabCtrlStates.erase(pCtrl);
}
void ClearRibbonPanelState(const CMFCRibbonPanel* pPanel) {
    if (!pPanel) return;
    auto it = g_ribbonPanelStates.find(pPanel);
    if (it != g_ribbonPanelStates.end()) {
        std::vector<CMFCRibbonBaseElement*> ownedElements(it->second.ownedElements.begin(), it->second.ownedElements.end());
        g_ribbonPanelStates.erase(it);
        for (CMFCRibbonBaseElement* element : ownedElements) {
            delete element;
        }
    }

    for (auto& entry : g_ribbonCategoryStates) {
        entry.second.ownedPanels.erase(const_cast<CMFCRibbonPanel*>(pPanel));
        auto& panels = entry.second.panels;
        panels.erase(std::remove(panels.begin(), panels.end(), pPanel), panels.end());
    }
}
void ClearRibbonCategoryState(const CMFCRibbonCategory* pCategory) {
    if (!pCategory) return;

    std::vector<CMFCRibbonPanel*> ownedPanels;
    auto it = g_ribbonCategoryStates.find(pCategory);
    if (it != g_ribbonCategoryStates.end()) {
        ownedPanels.assign(it->second.ownedPanels.begin(), it->second.ownedPanels.end());
        g_ribbonCategoryStates.erase(it);
    }

    for (auto& entry : g_ribbonBarStates) {
        RibbonBarState& state = entry.second;
        if (state.activeCategory == pCategory) {
            state.activeCategory = nullptr;
        }
        state.ownedCategories.erase(const_cast<CMFCRibbonCategory*>(pCategory));
        auto& categories = state.categories;
        categories.erase(std::remove(categories.begin(), categories.end(), pCategory), categories.end());
    }

    for (CMFCRibbonPanel* panel : ownedPanels) {
        delete panel;
    }
}
void ClearRibbonBarState(const CMFCRibbonBar* pBar) {
    if (!pBar) return;

    std::vector<CMFCRibbonCategory*> ownedCategories;
    auto it = g_ribbonBarStates.find(pBar);
    if (it != g_ribbonBarStates.end()) {
        ownedCategories.assign(it->second.ownedCategories.begin(), it->second.ownedCategories.end());
        g_ribbonBarStates.erase(it);
    }

    for (CMFCRibbonCategory* category : ownedCategories) {
        delete category;
    }
}
void CopyToolBarButtonBaseFields(CMFCToolBarButton* dst, const CMFCToolBarButton& src) {
    if (!dst) return;
    dst->m_bUserButton = src.m_bUserButton;
    dst->m_bText = src.m_bText;
    dst->m_bImage = src.m_bImage;
    dst->m_bWrap = src.m_bWrap;
    dst->m_bWholeText = src.m_bWholeText;
    dst->m_bTextBelow = src.m_bTextBelow;
    dst->m_bDragFromCollection = src.m_bDragFromCollection;
    dst->m_nID = src.m_nID;
    dst->m_nStyle = src.m_nStyle;
    dst->m_dwdItemData = src.m_dwdItemData;
    dst->m_strText = src.m_strText;
    dst->m_strTextCustom = src.m_strTextCustom;
    dst->m_iImage = src.m_iImage;
    dst->m_iUserImage = src.m_iUserImage;
    dst->m_bLocked = src.m_bLocked;
    dst->m_bIsHidden = src.m_bIsHidden;
    dst->m_bDisableFill = src.m_bDisableFill;
    dst->m_bExtraSize = src.m_bExtraSize;
    dst->m_bHorz = src.m_bHorz;
    dst->m_bVisible = src.m_bVisible;
    dst->m_rect = src.m_rect;
    dst->m_sizeText = src.m_sizeText;
    dst->m_pWndParent = src.m_pWndParent;
}
void CopyMenuButtonState(CMFCToolBarMenuButton* dst, const CMFCToolBarMenuButton* src) {
    if (!dst) return;
    if (!src) {
        g_menuButtonStates.erase(dst);
        return;
    }

    auto stateIt = g_menuButtonStates.find(src);
    if (stateIt == g_menuButtonStates.end()) {
        g_menuButtonStates.erase(dst);
        return;
    }

    g_menuButtonStates[dst] = stateIt->second;
}
CMFCToolBarButton* CloneToolBarButton(const CMFCToolBarButton& button) {
    if (button.IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton))) {
        auto* cloned = new (std::nothrow) CMFCToolBarMenuButtonsButton();
        if (cloned) {
            CopyToolBarButtonBaseFields(cloned, button);
            if (button.IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
                const auto& menuButton = static_cast<const CMFCToolBarMenuButton&>(button);
                cloned->m_hMenu = menuButton.m_hMenu;
                CopyMenuButtonState(cloned, &menuButton);
            }
            cloned->m_uiSystemCommand = static_cast<const CMFCToolBarMenuButtonsButton&>(button).m_uiSystemCommand;
        }
        return cloned;
    }
    if (button.IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
        const auto& menuButton = static_cast<const CMFCToolBarMenuButton&>(button);
        auto* cloned = new (std::nothrow) CMFCToolBarMenuButton(menuButton);
        CopyMenuButtonState(cloned, &menuButton);
        return cloned;
    }
    if (button.IsKindOf(RUNTIME_CLASS(CMFCToolBarComboBoxButton))) {
        auto* cloned = new (std::nothrow) CMFCToolBarComboBoxButton();
        if (cloned) {
            CopyToolBarButtonBaseFields(cloned, button);
            const auto& combo = static_cast<const CMFCToolBarComboBoxButton&>(button);
            cloned->m_bFlat = combo.m_bFlat;
            cloned->m_bCenterVert = combo.m_bCenterVert;
            OpenMFC_CopyComboButtonState(cloned, &combo);
        }
        return cloned;
    }
    if (button.IsKindOf(RUNTIME_CLASS(CMFCToolBarEditBoxButton))) {
        auto* cloned = new (std::nothrow) CMFCToolBarEditBoxButton();
        if (cloned) {
            CopyToolBarButtonBaseFields(cloned, button);
            const auto& edit = static_cast<const CMFCToolBarEditBoxButton&>(button);
            cloned->m_bFlat = edit.m_bFlat;
            OpenMFC_CopyEditButtonState(cloned, &edit);
        }
        return cloned;
    }
    return new (std::nothrow) CMFCToolBarButton(button);
}
void ClearPopupMenuBarState(const CMFCPopupMenuBar* pMenuBar) {
    auto it = g_popupMenuBarStates.find(pMenuBar);
    if (it == g_popupMenuBarStates.end()) return;
    for (CMFCToolBarMenuButton* item : it->second.ownedItems) {
        delete item;
    }
    g_popupMenuBarStates.erase(it);
}
std::wstring CurrentAppExSection(CWinAppEx* app) {
    if (!app) return L"";
    CString section = app->GetRegSectionPath(nullptr);
    const wchar_t* value = static_cast<const wchar_t*>(section);
    return value ? std::wstring(value) : std::wstring();
}
CString GetMenuItemText(HMENU hMenu, int index) {
    wchar_t buffer[256] = {};
    if (hMenu) {
        ::GetMenuStringW(hMenu, static_cast<UINT>(index), buffer, 256, MF_BYPOSITION);
    }
    return CString(buffer);
}
UINT GetMenuItemCommand(HMENU hMenu, int index) {
    if (!hMenu) return 0;
    UINT id = ::GetMenuItemID(hMenu, index);
    return id == static_cast<UINT>(-1) ? 0 : id;
}
HMENU GetMenuItemSubMenu(HMENU hMenu, int index) {
    return hMenu ? ::GetSubMenu(hMenu, index) : nullptr;
}
CMFCToolBarMenuButton* CreateMenuButtonFromMenu(HMENU hMenu, int index) {
    const UINT id = GetMenuItemCommand(hMenu, index);
    HMENU subMenu = GetMenuItemSubMenu(hMenu, index);
    CString text = GetMenuItemText(hMenu, index);
    return new CMFCToolBarMenuButton(id, subMenu, subMenu != nullptr, static_cast<const wchar_t*>(text), FALSE);
}
void ImportMenuIntoPopupItems(HMENU hMenu, std::vector<CMFCToolBarMenuButton*>& items,
                              std::unordered_set<CMFCToolBarMenuButton*>& ownedItems) {
    for (CMFCToolBarMenuButton* item : ownedItems) {
        delete item;
    }
    items.clear();
    ownedItems.clear();

    if (!hMenu) return;
    int count = ::GetMenuItemCount(hMenu);
    for (int i = 0; i < count; ++i) {
        CMFCToolBarMenuButton* button = CreateMenuButtonFromMenu(hMenu, i);
        items.push_back(button);
        ownedItems.insert(button);
    }
}
CFont& EnsureMenuFont_Mfccore() {
    if (!g_menuFontPtr) {
        g_menuFontPtr = reinterpret_cast<CFont*>(g_menuFontStorage);
        g_menuFontPtr->m_hObject = nullptr;
    }
    return *g_menuFontPtr;
}
std::wstring WideKey(const wchar_t* value) {
    return value ? std::wstring(value) : std::wstring();
}
std::wstring AppEntryKey(const wchar_t* section, const wchar_t* entry) {
    std::wstring key = WideKey(section);
    key.push_back(L'/');
    key += WideKey(entry);
    return key;
}
HMENU LoadMenuResource(UINT uiMenuResId, HMENU* pOwnedMenu) {
    HMENU hMenu = ::LoadMenuW(AfxGetResourceHandle(), MAKEINTRESOURCEW(uiMenuResId));
    if (hMenu) {
        if (pOwnedMenu) *pOwnedMenu = hMenu;
        HMENU hSubMenu = ::GetSubMenu(hMenu, 0);
        return hSubMenu ? hSubMenu : hMenu;
    }
    hMenu = ::CreatePopupMenu();
    if (pOwnedMenu) *pOwnedMenu = hMenu;
    return hMenu;
}
HDC SafeHdc(CDC* pDC) {
    return pDC ? pDC->GetSafeHdc() : nullptr;
}
RECT ToRECT(const CRect& rect) {
    return RECT{rect.left, rect.top, rect.right, rect.bottom};
}
bool IsDrawableRect(const CRect& rect) {
    return rect.right > rect.left && rect.bottom > rect.top;
}
void FillSolid(CDC* pDC, const CRect& rect, COLORREF color) {
    HDC hdc = SafeHdc(pDC);
    if (!hdc || !IsDrawableRect(rect)) return;
    RECT nativeRect = ToRECT(rect);
    HBRUSH brush = ::CreateSolidBrush(color);
    if (brush) {
        ::FillRect(hdc, &nativeRect, brush);
        ::DeleteObject(brush);
    }
}
void FrameSolid(CDC* pDC, const CRect& rect, COLORREF color) {
    HDC hdc = SafeHdc(pDC);
    if (!hdc || !IsDrawableRect(rect)) return;
    RECT nativeRect = ToRECT(rect);
    HBRUSH brush = ::CreateSolidBrush(color);
    if (brush) {
        ::FrameRect(hdc, &nativeRect, brush);
        ::DeleteObject(brush);
    }
}
void DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color) {
    HDC hdc = SafeHdc(pDC);
    if (!hdc) return;
    HPEN pen = ::CreatePen(PS_SOLID, 1, color);
    HGDIOBJ oldPen = pen ? ::SelectObject(hdc, pen) : nullptr;
    ::MoveToEx(hdc, x1, y1, nullptr);
    ::LineTo(hdc, x2, y2);
    if (oldPen) ::SelectObject(hdc, oldPen);
    if (pen) ::DeleteObject(pen);
}
void Draw3dFrame(CDC* pDC, const CRect& rect, COLORREF light, COLORREF dark) {
    if (!IsDrawableRect(rect)) return;
    DrawLine(pDC, rect.left, rect.top, rect.right - 1, rect.top, light);
    DrawLine(pDC, rect.left, rect.top, rect.left, rect.bottom - 1, light);
    DrawLine(pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, dark);
    DrawLine(pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, dark);
}
void FillAndFrame(CDC* pDC, const CRect& rect, COLORREF fill, COLORREF border) {
    FillSolid(pDC, rect, fill);
    FrameSolid(pDC, rect, border);
}
COLORREF ButtonFillForState(CMFCVisualManager::AFX_BUTTON_STATE state) {
    switch (state) {
    case CMFCVisualManager::ButtonsIsPressed:
        return CMFCVisualManager::GetThemeColor(g_visualAccentColor, -35);
    case CMFCVisualManager::ButtonsIsHighlighted:
        return CMFCVisualManager::GetThemeColor(g_visualAccentColor, 45);
    case CMFCVisualManager::ButtonsIsDisabled:
        return ::GetSysColor(COLOR_BTNFACE);
    case CMFCVisualManager::ButtonsIsRegular:
    default:
        return g_visualBackgroundColor;
    }
}
void DrawArrowGlyph(CDC* pDC, CRect rect, bool down, COLORREF color) {
    HDC hdc = SafeHdc(pDC);
    if (!hdc || !IsDrawableRect(rect)) return;
    const int cx = (rect.left + rect.right) / 2;
    const int cy = (rect.top + rect.bottom) / 2;
    POINT points[3]{};
    if (down) {
        points[0] = POINT{cx - 4, cy - 2};
        points[1] = POINT{cx + 4, cy - 2};
        points[2] = POINT{cx, cy + 3};
    } else {
        points[0] = POINT{cx - 4, cy + 2};
        points[1] = POINT{cx + 4, cy + 2};
        points[2] = POINT{cx, cy - 3};
    }
    HBRUSH brush = ::CreateSolidBrush(color);
    HPEN pen = ::CreatePen(PS_SOLID, 1, color);
    HGDIOBJ oldBrush = brush ? ::SelectObject(hdc, brush) : nullptr;
    HGDIOBJ oldPen = pen ? ::SelectObject(hdc, pen) : nullptr;
    ::Polygon(hdc, points, 3);
    if (oldPen) ::SelectObject(hdc, oldPen);
    if (oldBrush) ::SelectObject(hdc, oldBrush);
    if (pen) ::DeleteObject(pen);
    if (brush) ::DeleteObject(brush);
}
void DrawPlusMinus(CDC* pDC, CRect rect, bool minusOnly, COLORREF color) {
    if (!IsDrawableRect(rect)) return;
    FrameSolid(pDC, rect, color);
    const int cx = (rect.left + rect.right) / 2;
    const int cy = (rect.top + rect.bottom) / 2;
    DrawLine(pDC, rect.left + 3, cy, rect.right - 3, cy, color);
    if (!minusOnly) {
        DrawLine(pDC, cx, rect.top + 3, cx, rect.bottom - 3, color);
    }
}
void DrawRectText(CDC* pDC, CRect rect, const wchar_t* text, COLORREF color) {
    HDC hdc = SafeHdc(pDC);
    if (!hdc || !text) return;
    RECT nativeRect = ToRECT(rect);
    COLORREF oldText = ::SetTextColor(hdc, color);
    int oldMode = ::SetBkMode(hdc, TRANSPARENT);
    ::DrawTextW(hdc, text, -1, &nativeRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    ::SetBkMode(hdc, oldMode);
    ::SetTextColor(hdc, oldText);
}
void DestroyContextMenus(ContextMenuState& state) {
    for (auto& entry : state.ownedMenusById) {
        if (entry.second) ::DestroyMenu(entry.second);
    }
    state.menusById.clear();
    state.ownedMenusById.clear();
    state.idsByName.clear();
}
void SyncVisualManagerExports() {
    impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA = g_pDefaultVisualManagerClass;
    impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA = g_pVisualManager;
}
void SetVisualPalette(COLORREF accent, COLORREF background, COLORREF border) {
    g_visualAccentColor = accent;
    g_visualBackgroundColor = background;
    g_visualBorderColor = border;
}
void ApplyVisualPaletteForClass(const CRuntimeClass* pClass) {
    if (pClass == CMFCVisualManagerOffice2007::GetThisClass()) {
        switch (CMFCVisualManagerOffice2007::GetStyle()) {
        case CMFCVisualManagerOffice2007::Office2007_ObsidianBlack:
            SetVisualPalette(RGB(65, 75, 86), RGB(214, 218, 223), RGB(93, 103, 115));
            break;
        case CMFCVisualManagerOffice2007::Office2007_Silver:
            SetVisualPalette(RGB(126, 142, 167), RGB(232, 234, 239), RGB(145, 153, 166));
            break;
        case CMFCVisualManagerOffice2007::Office2007_Aqua:
            SetVisualPalette(RGB(49, 155, 177), RGB(224, 241, 244), RGB(103, 166, 181));
            break;
        case CMFCVisualManagerOffice2007::Office2007_LunaBlue:
        default:
            SetVisualPalette(RGB(59, 112, 185), RGB(221, 232, 246), RGB(117, 150, 191));
            break;
        }
        return;
    }
    if (pClass == CMFCVisualManagerOffice2003::GetThisClass() ||
        pClass == CMFCVisualManagerOfficeXP::GetThisClass()) {
        SetVisualPalette(RGB(49, 106, 197), RGB(236, 239, 243), RGB(127, 157, 185));
        return;
    }
    if (pClass == CMFCVisualManagerVS2005::GetThisClass()) {
        SetVisualPalette(RGB(0, 122, 204), RGB(238, 238, 242), RGB(104, 104, 104));
        return;
    }
    if (pClass == CMFCVisualManagerWindows7::GetThisClass() ||
        pClass == CMFCVisualManagerWindows::GetThisClass() ||
        pClass == CMFCVisualManagerAero::GetThisClass()) {
        SetVisualPalette(RGB(0, 120, 215), ::GetSysColor(COLOR_3DFACE), RGB(160, 160, 160));
        return;
    }
    SetVisualPalette(RGB(0, 120, 215), ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DSHADOW));
}
CMFCVisualManager* CreateVisualManagerFromRuntimeClass(CRuntimeClass* pRTI) {
    CRuntimeClass* pClass = pRTI;
    if (pClass == nullptr || !pClass->IsDerivedFrom(RUNTIME_CLASS(CMFCVisualManager))) {
        pClass = RUNTIME_CLASS(CMFCVisualManager);
    }
    ApplyVisualPaletteForClass(pClass);

    CObject* pObject = pClass->CreateObject();
    if (pObject != nullptr && pObject->IsKindOf(RUNTIME_CLASS(CMFCVisualManager))) {
        CMFCVisualManager* manager = static_cast<CMFCVisualManager*>(pObject);
        g_pVisualManager = manager;
        SyncVisualManagerExports();
        return manager;
    }

    delete pObject;
    CMFCVisualManager* manager = new CMFCVisualManager();
    g_pVisualManager = manager;
    SyncVisualManagerExports();
    return manager;
}
void OpenMFC_InitToolBarButton(CMFCToolBarButton* b) {
    b->m_bUserButton = FALSE;
    b->m_bText = FALSE;
    b->m_bImage = TRUE;
    b->m_bWrap = FALSE;
    b->m_bWholeText = TRUE;
    b->m_bTextBelow = FALSE;
    b->m_bDragFromCollection = FALSE;
    b->m_nID = 0;
    b->m_nStyle = 0;
    b->m_dwdItemData = 0;
    b->m_iImage = -1;
    b->m_iUserImage = -1;
    b->m_bLocked = FALSE;
    b->m_bIsHidden = FALSE;
    b->m_bDisableFill = FALSE;
    b->m_bExtraSize = FALSE;
    b->m_bHorz = TRUE;
    b->m_bVisible = TRUE;
    b->m_rect.SetRectEmpty();
    b->m_sizeText.cx = 0;
    b->m_sizeText.cy = 0;
    b->m_pWndParent = nullptr;
}
CSize CalcMenuBarFixedLayout(const CMFCMenuBar* pMenuBar) {
    if (!pMenuBar) return CSize();
    const auto it = g_menuBarStates.find(pMenuBar);
    const int count = it == g_menuBarStates.end() ? 0 : static_cast<int>(it->second.items.size());
    return CSize(std::max(23, count * 80), 22);
}
CMFCPopupMenu* g_pActivePopupMenu = nullptr;
thread_local std::unordered_map<const CMFCPropertyGridProperty*, PropertyGridPropertyState> g_propertyGridPropertyStates;
thread_local std::unordered_map<const CMFCPropertyGridCtrl*, PropertyGridCtrlState> g_propertyGridCtrlStates;
thread_local std::unordered_map<const CMFCTasksPaneTask*, TasksPaneTaskState> g_tasksPaneTaskStates;
thread_local std::unordered_map<const CMFCTasksPane*, TasksPaneState> g_tasksPaneStates;
PropertyGridPropertyState& EnsurePropertyGridPropertyState(const CMFCPropertyGridProperty* pProp) {
    return g_propertyGridPropertyStates[pProp];
}
const PropertyGridPropertyState* FindPropertyGridPropertyState(const CMFCPropertyGridProperty* pProp) {
    auto it = g_propertyGridPropertyStates.find(pProp);
    return it == g_propertyGridPropertyStates.end() ? nullptr : &it->second;
}
PropertyGridPropertyState* FindMutablePropertyGridPropertyState(const CMFCPropertyGridProperty* pProp) {
    auto it = g_propertyGridPropertyStates.find(pProp);
    return it == g_propertyGridPropertyStates.end() ? nullptr : &it->second;
}
void DeletePropertyGridChildren(CMFCPropertyGridProperty* pProp) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
    if (!state) return;

    std::vector<CMFCPropertyGridProperty*> children = state->subItems;
    state->subItems.clear();
    for (CMFCPropertyGridProperty* child : children) {
        if (!child) continue;
        PropertyGridPropertyState* childState = FindMutablePropertyGridPropertyState(child);
        if (childState && childState->parent == pProp) {
            childState->parent = nullptr;
            SetPropertyGridOwnerRecursive(child, nullptr);
        }
        delete child;
    }
}
void RemovePropertyGridPropertyReferences(const CMFCPropertyGridProperty* pProp) {
    g_propertyGridPropertyStates.erase(pProp);
    for (auto& [unusedPropertyKey, propertyState] : g_propertyGridPropertyStates) {
        (void)unusedPropertyKey;
        auto& subItems = propertyState.subItems;
        subItems.erase(std::remove(subItems.begin(), subItems.end(), pProp), subItems.end());
        if (propertyState.parent == pProp) {
            propertyState.parent = nullptr;
        }
    }
    for (auto& [unusedCtrlKey, ctrlState] : g_propertyGridCtrlStates) {
        (void)unusedCtrlKey;
        auto& properties = ctrlState.properties;
        properties.erase(std::remove(properties.begin(), properties.end(), pProp), properties.end());
        if (ctrlState.current == pProp) {
            ctrlState.current = nullptr;
        }
    }
}
PropertyGridCtrlState& EnsurePropertyGridCtrlState(const CMFCPropertyGridCtrl* pCtrl) {
    return g_propertyGridCtrlStates[pCtrl];
}
const PropertyGridCtrlState* FindPropertyGridCtrlState(const CMFCPropertyGridCtrl* pCtrl) {
    auto it = g_propertyGridCtrlStates.find(pCtrl);
    return it == g_propertyGridCtrlStates.end() ? nullptr : &it->second;
}
PropertyGridCtrlState* FindMutablePropertyGridCtrlState(const CMFCPropertyGridCtrl* pCtrl) {
    auto it = g_propertyGridCtrlStates.find(pCtrl);
    return it == g_propertyGridCtrlStates.end() ? nullptr : &it->second;
}
void TouchPropertyGridCtrl(CMFCPropertyGridCtrl* pCtrl, BOOL bAdjustLayout) {
    if (!pCtrl) return;
    if (bAdjustLayout) {
        pCtrl->AdjustLayout();
    } else {
        pCtrl->Invalidate(FALSE);
    }
}
void DetachPropertyFromParent(CMFCPropertyGridProperty* pProp) {
    if (!pProp) return;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
    if (!state) return;

    if (state->parent) {
        PropertyGridPropertyState* parentState = FindMutablePropertyGridPropertyState(state->parent);
        if (parentState) {
            auto& subItems = parentState->subItems;
            subItems.erase(std::remove(subItems.begin(), subItems.end(), pProp), subItems.end());
        }
        state->parent = nullptr;
    }
}
void DetachPropertyFromGrid(CMFCPropertyGridProperty* pProp) {
    if (!pProp) return;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
    if (!state || !state->owner) return;

    PropertyGridCtrlState* ctrlState = FindMutablePropertyGridCtrlState(state->owner);
    if (ctrlState) {
        auto& properties = ctrlState->properties;
        properties.erase(std::remove(properties.begin(), properties.end(), pProp), properties.end());
    }
    SetPropertyGridOwnerRecursive(pProp, nullptr);
}
BOOL IsPropertyGridAncestorOf(const CMFCPropertyGridProperty* pAncestor,
                              const CMFCPropertyGridProperty* pDescendant) {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pDescendant);
    while (state && state->parent) {
        if (state->parent == pAncestor) {
            return TRUE;
        }
        state = FindPropertyGridPropertyState(state->parent);
    }
    return FALSE;
}
std::wstring JoinPropertyGridArrayParts(const std::vector<std::wstring>& parts, const PropertyGridCtrlState* ctrlState) {
    std::wstring result;
    const wchar_t delimiter = ctrlState ? ctrlState->listDelimiter : L',';
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) {
            result.push_back(delimiter);
            result.push_back(L' ');
        }
        result += parts[i];
    }
    return result;
}
std::wstring PropertyGridArrayToString(const VARIANT& value, const PropertyGridCtrlState* ctrlState) {
    if (!(value.vt & VT_ARRAY) || !value.parray || SafeArrayGetDim(value.parray) != 1) {
        return L"";
    }

    long lower = 0;
    long upper = -1;
    if (FAILED(SafeArrayGetLBound(value.parray, 1, &lower)) ||
        FAILED(SafeArrayGetUBound(value.parray, 1, &upper)) ||
        upper < lower) {
        return L"";
    }

    VARTYPE elementType = value.vt & VT_TYPEMASK;
    if (elementType == VT_EMPTY) {
        SafeArrayGetVartype(value.parray, &elementType);
    }

    std::vector<std::wstring> parts;
    parts.reserve(static_cast<size_t>(upper - lower + 1));
    for (long index = lower; index <= upper; ++index) {
        VARIANT element;
        VariantInit(&element);
        HRESULT hr = E_FAIL;

        switch (elementType) {
        case VT_VARIANT:
            hr = SafeArrayGetElement(value.parray, &index, &element);
            break;
        case VT_BSTR: {
            BSTR text = nullptr;
            hr = SafeArrayGetElement(value.parray, &index, &text);
            if (SUCCEEDED(hr)) {
                element.vt = VT_BSTR;
                element.bstrVal = text;
            }
            break;
        }
        case VT_BOOL: {
            VARIANT_BOOL item = VARIANT_FALSE;
            hr = SafeArrayGetElement(value.parray, &index, &item);
            if (SUCCEEDED(hr)) {
                element.vt = VT_BOOL;
                element.boolVal = item;
            }
            break;
        }
        case VT_I2: {
            SHORT item = 0;
            hr = SafeArrayGetElement(value.parray, &index, &item);
            if (SUCCEEDED(hr)) {
                element.vt = VT_I2;
                element.iVal = item;
            }
            break;
        }
        case VT_I4:
        case VT_INT: {
            LONG item = 0;
            hr = SafeArrayGetElement(value.parray, &index, &item);
            if (SUCCEEDED(hr)) {
                element.vt = elementType;
                element.lVal = item;
            }
            break;
        }
        case VT_R8: {
            DOUBLE item = 0;
            hr = SafeArrayGetElement(value.parray, &index, &item);
            if (SUCCEEDED(hr)) {
                element.vt = VT_R8;
                element.dblVal = item;
            }
            break;
        }
        default:
            break;
        }

        if (SUCCEEDED(hr)) {
            parts.push_back(PropertyGridVariantToString(element, ctrlState));
        }
        VariantClear(&element);
    }

    return JoinPropertyGridArrayParts(parts, ctrlState);
}
std::wstring PropertyGridVariantToString(const VARIANT& value, const PropertyGridCtrlState* ctrlState) {
    if (value.vt & VT_ARRAY) {
        return PropertyGridArrayToString(value, ctrlState);
    }

    switch (value.vt) {
    case VT_EMPTY:
    case VT_NULL:
        return L"";
    case VT_BSTR:
        return value.bstrVal ? std::wstring(value.bstrVal) : std::wstring();
    case VT_BOOL:
        if (ctrlState) {
            return value.boolVal == VARIANT_FALSE ? ctrlState->boolFalse : ctrlState->boolTrue;
        }
        return value.boolVal == VARIANT_FALSE ? L"False" : L"True";
    case VT_I1:
        return std::to_wstring(value.cVal);
    case VT_UI1:
        return std::to_wstring(value.bVal);
    case VT_I2:
        return std::to_wstring(value.iVal);
    case VT_UI2:
        return std::to_wstring(value.uiVal);
    case VT_I4:
    case VT_INT:
        return std::to_wstring(value.lVal);
    case VT_UI4:
    case VT_UINT:
        return std::to_wstring(value.ulVal);
    case VT_I8:
        return std::to_wstring(value.llVal);
    case VT_UI8:
        return std::to_wstring(value.ullVal);
    case VT_R4:
        return std::to_wstring(value.fltVal);
    case VT_R8:
        return std::to_wstring(value.dblVal);
    default:
        break;
    }

    VARIANT converted;
    VariantInit(&converted);
    if (SUCCEEDED(VariantChangeType(&converted, const_cast<VARIANT*>(&value), 0, VT_BSTR))) {
        std::wstring result = converted.bstrVal ? converted.bstrVal : L"";
        VariantClear(&converted);
        return result;
    }
    return L"";
}
void AssignPropertyGridVariant(COleVariant& target, const VARIANT& source) {
    target.Clear();
    VariantCopy(&target, const_cast<VARIANT*>(&source));
}
BOOL PropertyGridValuesEqual(const VARIANT& lhs, const VARIANT& rhs) {
    if (lhs.vt == rhs.vt) {
        switch (lhs.vt) {
        case VT_EMPTY:
        case VT_NULL:
            return TRUE;
        case VT_BSTR:
            return wcscmp(lhs.bstrVal ? lhs.bstrVal : L"", rhs.bstrVal ? rhs.bstrVal : L"") == 0;
        case VT_BOOL:
            return (lhs.boolVal == VARIANT_FALSE) == (rhs.boolVal == VARIANT_FALSE);
        case VT_I1:
            return lhs.cVal == rhs.cVal;
        case VT_UI1:
            return lhs.bVal == rhs.bVal;
        case VT_I2:
            return lhs.iVal == rhs.iVal;
        case VT_UI2:
            return lhs.uiVal == rhs.uiVal;
        case VT_I4:
        case VT_INT:
            return lhs.lVal == rhs.lVal;
        case VT_UI4:
        case VT_UINT:
            return lhs.ulVal == rhs.ulVal;
        case VT_I8:
            return lhs.llVal == rhs.llVal;
        case VT_UI8:
            return lhs.ullVal == rhs.ullVal;
        case VT_R4:
            return lhs.fltVal == rhs.fltVal;
        case VT_R8:
            return lhs.dblVal == rhs.dblVal;
        default:
            break;
        }
    }

    VARIANT converted;
    VariantInit(&converted);
    if (SUCCEEDED(VariantChangeType(&converted, const_cast<VARIANT*>(&rhs), 0, lhs.vt))) {
        BOOL equal = PropertyGridValuesEqual(lhs, converted);
        VariantClear(&converted);
        return equal;
    }

    return PropertyGridVariantToString(lhs) == PropertyGridVariantToString(rhs);
}
int CountExpandedPropertyGridSubItems(const CMFCPropertyGridProperty* pProp, BOOL bIncludeHidden) {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state) return 0;

    int count = 0;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        if (!child) continue;
        if (!bIncludeHidden && !child->IsVisible()) continue;
        ++count;
        if (child->IsExpanded()) {
            count += CountExpandedPropertyGridSubItems(child, bIncludeHidden);
        }
    }
    return count;
}
int CountPropertyGridItems(const CMFCPropertyGridProperty* pProp, BOOL bIncludeHidden) {
    if (!pProp) return 0;
    if (!bIncludeHidden && !pProp->IsVisible()) return 0;
    int count = 1;
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state) return count;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        count += CountPropertyGridItems(child, bIncludeHidden);
    }
    return count;
}
void AppendVisiblePropertyGridItems(CMFCPropertyGridProperty* pProp,
                                    std::vector<CMFCPropertyGridProperty*>& items) {
    if (!pProp || !pProp->IsVisible() || !pProp->IsParentExpanded()) return;
    items.push_back(pProp);

    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state || !pProp->IsExpanded()) return;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        AppendVisiblePropertyGridItems(child, items);
    }
}
std::vector<CMFCPropertyGridProperty*> VisiblePropertyGridItems(const CMFCPropertyGridCtrl* pCtrl) {
    std::vector<CMFCPropertyGridProperty*> items;
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pCtrl);
    if (!state) return items;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        AppendVisiblePropertyGridItems(prop, items);
    }
    return items;
}
int PropertyGridChildIDFromVariant(const VARIANT& varChild) {
    if (varChild.vt == VT_EMPTY) return CHILDID_SELF;
    if (varChild.vt == VT_I4 || varChild.vt == VT_INT) return static_cast<int>(varChild.lVal);
    if (varChild.vt == VT_UI4 || varChild.vt == VT_UINT) return static_cast<int>(varChild.ulVal);
    if (varChild.vt == VT_I2) return static_cast<int>(varChild.iVal);
    if (varChild.vt == VT_UI2) return static_cast<int>(varChild.uiVal);
    return -1;
}
CMFCPropertyGridProperty* PropertyGridPropertyFromChildID(CMFCPropertyGridCtrl* pCtrl, int childID) {
    if (!pCtrl || childID <= CHILDID_SELF) return nullptr;
    std::vector<CMFCPropertyGridProperty*> items = VisiblePropertyGridItems(pCtrl);
    const size_t index = static_cast<size_t>(childID - 1);
    return index < items.size() ? items[index] : nullptr;
}
int PropertyGridChildIDFromProperty(CMFCPropertyGridCtrl* pCtrl, CMFCPropertyGridProperty* pProp) {
    if (!pCtrl || !pProp) return CHILDID_SELF;
    std::vector<CMFCPropertyGridProperty*> items = VisiblePropertyGridItems(pCtrl);
    auto it = std::find(items.begin(), items.end(), pProp);
    return it == items.end() ? CHILDID_SELF : static_cast<int>(it - items.begin()) + 1;
}
void SetPropertyGridVariantChild(VARIANT* pvarChild, int childID) {
    if (!pvarChild) return;
    VariantInit(pvarChild);
    pvarChild->vt = VT_I4;
    pvarChild->lVal = childID;
}
HRESULT AllocPropertyGridString(const std::wstring& text, BSTR* ppszValue) {
    if (!ppszValue) return E_POINTER;
    *ppszValue = ::SysAllocString(text.c_str());
    return *ppszValue ? S_OK : E_OUTOFMEMORY;
}
RECT NativeRect(const CRect& rect) {
    RECT nativeRect{rect.left, rect.top, rect.right, rect.bottom};
    return nativeRect;
}
CRect PropertyGridItemRect(CMFCPropertyGridCtrl* pCtrl, int childID) {
    RECT rcClient{0, 0, 240, 18};
    if (pCtrl && pCtrl->GetSafeHwnd()) {
        pCtrl->GetClientRect(&rcClient);
    }

    const int rowHeight = 18;
    int top = 0;
    if (childID > CHILDID_SELF) {
        top = (childID - 1) * rowHeight;
    }
    return CRect(rcClient.left, top, rcClient.right, top + rowHeight);
}
CMFCPropertyGridProperty* PropertyGridHitTestByPoint(CMFCPropertyGridCtrl* pCtrl, CPoint point) {
    if (!pCtrl) return nullptr;
    const int rowHeight = 18;
    if (point.y < 0) return nullptr;
    int childID = point.y / rowHeight + 1;
    return PropertyGridPropertyFromChildID(pCtrl, childID);
}
CMFCPropertyGridProperty* FindPropertyGridSubItemByData(CMFCPropertyGridProperty* pProp, DWORD_PTR dwData) {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state) return nullptr;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        if (!child) continue;
        if (child->GetData() == dwData) return child;
        if (CMFCPropertyGridProperty* found = FindPropertyGridSubItemByData(child, dwData)) {
            return found;
        }
    }
    return nullptr;
}
void ResetPropertyGridOriginalValueRecursive(CMFCPropertyGridProperty* pProp) {
    if (!pProp) return;
    pProp->ResetOriginalValue();
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state) return;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        ResetPropertyGridOriginalValueRecursive(child);
    }
}
void MarkPropertyGridModifiedRecursive(CMFCPropertyGridProperty* pProp, BOOL bModified) {
    if (!pProp) return;
    pProp->SetModified(bModified);
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state) return;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        MarkPropertyGridModifiedRecursive(child, bModified);
    }
}
TasksPaneTaskState& EnsureTasksPaneTaskState(const CMFCTasksPaneTask* pTask) {
    return g_tasksPaneTaskStates[pTask];
}
TasksPaneState& EnsureTasksPaneState(const CMFCTasksPane* pPane) {
    return g_tasksPaneStates[pPane];
}
void RemoveTaskFromAllGroups(const CMFCTasksPaneTask* pTask) {
    g_tasksPaneTaskStates.erase(pTask);
    for (auto& [unusedPaneKey, paneState] : g_tasksPaneStates) {
        (void)unusedPaneKey;
        for (auto& group : paneState.groups) {
            auto& tasks = group.tasks;
            tasks.erase(
                std::remove_if(tasks.begin(), tasks.end(),
                    [pTask](const TasksPaneGroupState::TaskEntry& task) { return task.task == pTask; }),
                tasks.end());
        }
    }
}
TasksPaneGroupState& EnsureTasksPaneGroup(TasksPaneState& state, int nGroup) {
    if (nGroup < 0) {
        state.groups.push_back({});
        return state.groups.back();
    }

    const size_t groupIndex = static_cast<size_t>(nGroup);
    if (groupIndex >= state.groups.size()) {
        state.groups.resize(groupIndex + 1);
    }
    return state.groups[groupIndex];
}
void ExpandPropertyRecursive(CMFCPropertyGridProperty* pProp, BOOL bExpand) {
    if (!pProp) return;
    pProp->Expand(bExpand);
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pProp);
    if (!state) return;
    for (CMFCPropertyGridProperty* child : state->subItems) {
        ExpandPropertyRecursive(child, bExpand);
    }
}
LPITEMIDLIST AllocatePidlBytes(UINT bytes) {
    if (bytes < sizeof(USHORT)) bytes = sizeof(USHORT);
    void* memory = ::CoTaskMemAlloc(bytes);
    if (!memory) return nullptr;
    std::memset(memory, 0, bytes);
    return static_cast<LPITEMIDLIST>(memory);
}
unsigned long MS_ABI DrawPropertySheetListItemFallback(
    void* pThis, CDC* pDC, const CRect& rect, int nItem, int nState) {
    (void)pThis;
    if (!pDC || !pDC->m_hDC) {
        return FALSE;
    }

    HDC hdc = pDC->m_hDC;
    const bool selected = (nState & 1) != 0;
    const bool disabled = (nState & 2) != 0;

    RECT r = { rect.left, rect.top, rect.right, rect.bottom };
    const COLORREF bg = selected ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_WINDOW);
    const COLORREF text = disabled ? GetSysColor(COLOR_GRAYTEXT)
                                  : (selected ? GetSysColor(COLOR_HIGHLIGHTTEXT)
                                             : GetSysColor(COLOR_WINDOWTEXT));

    HBRUSH brush = ::CreateSolidBrush(bg);
    if (brush) {
        ::FillRect(hdc, &r, brush);
        ::DeleteObject(brush);
    }

    RECT rcText = r;
    ::InflateRect(&rcText, -4, 0);
    wchar_t label[64];
    ::swprintf(label, 64, L"Item %d", nItem + 1);

    ::SetBkMode(hdc, TRANSPARENT);
    ::SetTextColor(hdc, text);
    ::DrawTextW(hdc, label, -1, &rcText,
                DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
    ::DrawFocusRect(hdc, &r);

    return TRUE;
}
} } }  // namespace openmfc::detail::mfccore
