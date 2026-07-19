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
#include "docking_state.h"
#include "ribbon_state.h"
#include <algorithm>
#include <cwctype>
#include <cstring>
#include <cstdlib>
#include <new>
#include <shlobj.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

extern "C" void* impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA;
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
extern "C" std::int32_t impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA;
extern "C" std::int32_t impl__m_bStatusBarOfficeXPLook_CMFCVisualManagerOffice2003__1HA;
extern "C" std::int32_t impl__m_bUseGlobalTheme_CMFCVisualManagerOffice2003__1HA;
extern "C" std::int32_t impl__m_bAutoFreeRes_CMFCVisualManagerOffice2007__1HA;
extern "C" void* impl__m_hinstRes_CMFCVisualManagerOffice2007__1PEAUHINSTANCE____EA;
extern "C" std::uint32_t impl__m_Style_CMFCVisualManagerOffice2007__1W4Style_1_A;
extern "C" std::int32_t impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA;
extern "C" std::int32_t impl__m_b3DTabsXPTheme_CMFCVisualManagerWindows__2HA;

void OpenMFC_CopyComboButtonState(CMFCToolBarComboBoxButton* dst, const CMFCToolBarComboBoxButton* src);
void OpenMFC_CopyEditButtonState(CMFCToolBarEditBoxButton* dst, const CMFCToolBarEditBoxButton* src);

namespace {

using openmfc::ribbon_state::RibbonBarState;
using openmfc::ribbon_state::RibbonCategoryState;
using openmfc::ribbon_state::RibbonPanelState;
using openmfc::docking_state::PaneCoreState;

auto& g_paneCoreStateMutex = openmfc::docking_state::PaneCoreStateMutex();
auto& g_paneCoreState = openmfc::docking_state::PaneCoreStates();

struct DockingManagerState {
    std::vector<CBasePane*> panes;
    std::vector<void*> miniFrames;
    std::unordered_set<CBasePane*> hiddenPanes;
    std::unordered_set<CBasePane*> floatingPanes;
    DWORD enabledAlignment = 0;
    bool lockUpdate = false;
    bool paneContextMenu = false;
    UINT paneContextMenuID = 0;
    CString paneContextMenuName;
};

thread_local std::unordered_map<const CDockingManager*, DockingManagerState> g_dockingStates;

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

template <typename T>
bool ContainsPtr(const std::vector<T*>& values, T* value) {
    return std::find(values.begin(), values.end(), value) != values.end();
}

void AddDockingPane(CDockingManager* pManager, CBasePane* pPane, CBasePane* pInsertAfter = nullptr) {
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

CBasePane* FirstDockingPane(const CDockingManager* pManager, bool visibleOnly = false) {
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

struct PopupMenuState {
    CWnd* parent = nullptr;
    int x = 0;
    int y = 0;
    HMENU menu = nullptr;
    bool locked = false;
    bool mainMenu = false;
    bool vertResize = false;
    CSize minResize = CSize(0, 0);
    int selected = -1;
    std::vector<CMFCToolBarMenuButton*> items;
    std::unordered_set<CMFCToolBarMenuButton*> ownedItems;
};

struct MenuBarState {
    CWnd* parent = nullptr;
    HMENU menu = nullptr;
    std::vector<CMFCToolBarButton*> items;
    std::unordered_set<CMFCToolBarButton*> ownedItems;
};

struct ToolBarState {
    CWnd* parent = nullptr;
    DWORD dockStyle = 0;
    CSize buttonSize = CSize(23, 22);
    CSize imageSize = CSize(16, 15);
    std::vector<CMFCToolBarButton*> buttons;
    std::unordered_set<CMFCToolBarButton*> ownedButtons;
};

struct PopupMenuBarState {
    HMENU menu = nullptr;
    std::vector<CMFCToolBarMenuButton*> items;
    std::unordered_set<CMFCToolBarMenuButton*> ownedItems;
    int selected = -1;
    bool delayedSubMenuOpen = false;
};

struct MenuButtonState {
    bool hasDropDownArrow = false;
    bool userButton = false;
};

struct ContextMenuState {
    std::unordered_map<UINT, HMENU> menusById;
    std::unordered_map<UINT, HMENU> ownedMenusById;
    std::unordered_map<std::wstring, UINT> idsByName;
};

struct TooltipManagerState {
    UINT types = 0;
    CRuntimeClass* runtimeClass = nullptr;
    CMFCToolTipInfo* params = nullptr;
    std::vector<CToolTipCtrl*> tooltips;
};

struct MFCButtonState {
    HICON icon = nullptr;
    HICON iconHot = nullptr;
    COLORREF faceColor = RGB(240, 240, 240);
    COLORREF textColor = RGB(0, 0, 0);
    COLORREF textHotColor = RGB(0, 0, 0);
    bool ownerDraw = false;
    bool fullTextTooltip = false;
    CString tooltip;
    bool ownsIcon = false;
    bool ownsHotIcon = false;
};

struct TabEntry {
    CWnd* window = nullptr;
    std::wstring label;
    UINT id = static_cast<UINT>(-1);
    BOOL detachable = TRUE;
};

struct TabCtrlState {
    std::vector<TabEntry> tabs;
    int activeTab = -1;
    BOOL tabSwapEnabled = TRUE;
    int tabBorderSize = 0;
    int tabsHeight = 0;
};

struct MenuHashState {
    std::unordered_map<HMENU, CMFCToolBar*> barsByMenu;
};

struct WinAppExState {
    std::unordered_map<std::wstring, int> ints;
    std::unordered_map<std::wstring, CString> strings;
    std::unordered_map<std::wstring, std::vector<unsigned char>> binaries;
    std::unordered_set<std::wstring> objects;
    std::unordered_set<std::wstring> states;
    bool tearOffMenus = false;
    bool userTools = false;
    bool mouseManager = false;
    bool shellManager = false;
    CRect windowRect = CRect(0, 0, 0, 0);
    int windowShowCmd = SW_SHOWNORMAL;
    int windowFlags = 0;
};

thread_local std::unordered_map<const CMFCPopupMenu*, PopupMenuState> g_popupMenuStates;
thread_local std::unordered_map<const CMFCMenuBar*, MenuBarState> g_menuBarStates;
thread_local std::unordered_map<const CMFCToolBar*, ToolBarState> g_toolBarStates;
auto& g_ribbonPanelStates = openmfc::ribbon_state::RibbonPanelStates();
auto& g_ribbonCategoryStates = openmfc::ribbon_state::RibbonCategoryStates();
auto& g_ribbonBarStates = openmfc::ribbon_state::RibbonBarStates();
thread_local std::unordered_map<const CMFCPopupMenuBar*, PopupMenuBarState> g_popupMenuBarStates;
thread_local std::unordered_map<const CMFCToolBarMenuButton*, MenuButtonState> g_menuButtonStates;
thread_local std::unordered_map<const CContextMenuManager*, ContextMenuState> g_contextMenuStates;
thread_local std::unordered_map<const CTooltipManager*, TooltipManagerState> g_tooltipManagerStates;
thread_local std::unordered_map<const CMFCButton*, MFCButtonState> g_mfcButtonStates;
thread_local std::unordered_map<const CMenuHash*, MenuHashState> g_menuHashStates;
thread_local std::unordered_map<const CMFCBaseTabCtrl*, TabCtrlState> g_tabCtrlStates;
std::unordered_map<const CWinAppEx*, WinAppExState> g_winAppExStates;
static int g_mouseManagerToken = 0;
static int g_shellManagerToken = 0;
static CUserToolsManager g_userToolsManager;
static BOOL g_forceMenuFocus = FALSE;
static BOOL g_showAllAccelerators = FALSE;
CUserToolsManager* afxUserToolsManager = &g_userToolsManager;
alignas(CFont) static unsigned char g_menuFontStorage[sizeof(CFont)] = {};
static CFont* g_menuFontPtr = nullptr;
static COLORREF g_visualAccentColor = RGB(0, 120, 215);
static COLORREF g_visualBackgroundColor = RGB(240, 240, 240);
static COLORREF g_visualBorderColor = RGB(160, 160, 160);

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

CFont& EnsureMenuFont() {
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

} // namespace

static CMFCVisualManager* CreateVisualManagerFromRuntimeClass(CRuntimeClass* pRTI) {
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
void CMFCVisualManager::OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd*) { FillSolid(pDC, rectCaption, g_visualAccentColor); }
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
void CMFCVisualManager::OnDrawRibbonCategoryTab(CDC* pDC, CMFCRibbonTab*, BOOL bIsActive) { FillAndFrame(pDC, CRect(0, 0, 80, 24), bIsActive ? ::GetSysColor(COLOR_WINDOW) : g_visualBackgroundColor, g_visualBorderColor); }
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
void CMFCVisualManager::OnDrawRibbonPanel(CDC* pDC, CMFCRibbonPanel*, CRect rectPanel, CRect rectCaption) { FillAndFrame(pDC, rectPanel, ::GetSysColor(COLOR_3DFACE), g_visualBorderColor); if (IsDrawableRect(rectCaption)) FillSolid(pDC, rectCaption, ::GetSysColor(COLOR_3DLIGHT)); }
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
void CMFCVisualManager::OnDrawRibbonStatusBarPane(CDC* pDC, CMFCRibbonStatusBar*, CMFCRibbonStatusBarPane*) { FillAndFrame(pDC, CRect(0, 0, 80, 22), ::GetSysColor(COLOR_3DFACE), g_visualBorderColor); }
void CMFCVisualManager::OnDrawRibbonTabsFrame(CDC* pDC, CMFCRibbonBar*, CRect rectTab) { FillAndFrame(pDC, rectTab, g_visualBackgroundColor, g_visualBorderColor); }
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
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManager__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManager* pThis) { return CMFCVisualManager::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2003@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2003__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2003* pThis) { return CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2007@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2007__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2007* pThis) { return CMFCVisualManagerOffice2007::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOfficeXP@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOfficeXP__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOfficeXP* pThis) { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2005@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerVS2005__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerVS2005* pThis) { return CMFCVisualManagerVS2005::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows* pThis) { return CMFCVisualManagerWindows::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerWindows7@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerWindows7__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerWindows7* pThis) { return CMFCVisualManagerWindows7::GetThisClass(); }

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
CMFCVisualManagerOffice2003::CMFCVisualManagerOffice2003() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA = TRUE;
    impl__m_bStatusBarOfficeXPLook_CMFCVisualManagerOffice2003__1HA = TRUE;
    impl__m_bUseGlobalTheme_CMFCVisualManagerOffice2003__1HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOffice2003::~CMFCVisualManagerOffice2003() {}
CObject* CMFCVisualManagerOffice2003::CreateObject() { return new CMFCVisualManagerOffice2003(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerOffice2007, CMFCVisualManager)
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

IMPLEMENT_DYNAMIC(CMFCVisualManagerOfficeXP, CMFCVisualManager)
CMFCVisualManagerOfficeXP::CMFCVisualManagerOfficeXP() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOfficeXP::~CMFCVisualManagerOfficeXP() {}
CObject* CMFCVisualManagerOfficeXP::CreateObject() { return new CMFCVisualManagerOfficeXP(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerVS2005, CMFCVisualManager)
CMFCVisualManagerVS2005::CMFCVisualManagerVS2005() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005() {}
CObject* CMFCVisualManagerVS2005::CreateObject() { return new CMFCVisualManagerVS2005(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows, CMFCVisualManager)
CMFCVisualManagerWindows::CMFCVisualManagerWindows() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_b3DTabsXPTheme_CMFCVisualManagerWindows__2HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerWindows::~CMFCVisualManagerWindows() {}
CObject* CMFCVisualManagerWindows::CreateObject() { return new CMFCVisualManagerWindows(); }

IMPLEMENT_DYNAMIC(CMFCVisualManagerWindows7, CMFCVisualManager)
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

IMPLEMENT_DYNAMIC(CMFCVisualManagerAero, CMFCVisualManager)
CMFCVisualManagerAero::CMFCVisualManagerAero() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
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
    // Zero the retail member block (CWnd subobject is constructed already),
    // then apply the non-zero retail defaults.
    std::memset(reinterpret_cast<char*>(this) + sizeof(CWnd), 0,
                sizeof(CBasePane) - sizeof(CWnd));
    new (&m_AccData) CAccessibilityData();
    m_bEnableIDChecking = TRUE;
    m_dwEnabledAlignment = CBRS_ALIGN_ANY;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[this];
    state.canFloat = FALSE;
    state.canAutoHide = FALSE;
    state.hasGripper = FALSE;
}
CBasePane::~CBasePane() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState.erase(this);
}

BOOL CBasePane::CanAcceptPane(const CBasePane*) const { return TRUE; }
BOOL CBasePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.canAutoHide : FALSE;
}
BOOL CBasePane::CanBeAttached() const { return TRUE; }
BOOL CBasePane::CanBeClosed() const { return TRUE; }
BOOL CBasePane::CanBeDocked() const { return TRUE; }
BOOL CBasePane::CanBeFloating() const { return CanFloat(); }
BOOL CBasePane::CanBeResized() const { return TRUE; }
BOOL CBasePane::CanFloat() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.canFloat : FALSE;
}
BOOL CBasePane::DoesAllowSiblingBars() const { return TRUE; }
BOOL CBasePane::HasGripper() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.hasGripper : FALSE;
}
BOOL CBasePane::IsAccessibilityCompatible() { return TRUE; }
void CBasePane::SetPaneAlignment(DWORD dwAlignment) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[this].alignment = dwAlignment;
}
DWORD CBasePane::GetPaneAlignment() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.alignment : 0;
}
void* CBasePane::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* pExtra) {
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        PaneCoreState& state = g_paneCoreState[this];
        CRect rect = state.recentRect;
        if ((nFlags & SWP_NOMOVE) == 0) {
            rect.left = x;
            rect.top = y;
        }
        if ((nFlags & SWP_NOSIZE) == 0) {
            rect.right = rect.left + std::max(0, cx);
            rect.bottom = rect.top + std::max(0, cy);
        }
        state.recentRect = rect;
    }
    HWND hwnd = GetSafeHwnd();
    if (hwnd != nullptr) {
        HWND hwndInsertAfter = pWndInsertAfter != nullptr ? pWndInsertAfter->GetSafeHwnd() : nullptr;
        ::SetWindowPos(hwnd, hwndInsertAfter, x, y, cx, cy, nFlags);
    }
    return pExtra;
}
void CBasePane::CalcFixedLayout(BOOL, BOOL) {}
void CBasePane::RecalcLayout() {
    HWND hwnd = GetSafeHwnd();
    if (hwnd) {
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
    }
}

//=============================================================================
// CPane
//=============================================================================
IMPLEMENT_DYNAMIC(CPane, CBasePane)

CPane::CPane() {
    std::memset(reinterpret_cast<char*>(this) + sizeof(CBasePane), 0,
                sizeof(CPane) - sizeof(CBasePane));
    m_cxLeftBorder = m_cxRightBorder = 6;
    m_cyTopBorder = m_cyBottomBorder = 6;
    m_cxDefaultGap = 1;
    m_nMRUWidth = 32767;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[this].canFloat = TRUE;
}
CPane::~CPane() {}

BOOL CPane::CanBeDocked() const { return TRUE; }
BOOL CPane::CanFloat() const { return TRUE; }
void CPane::RecalcLayout() {
    CBasePane::RecalcLayout();
}

//=============================================================================
// CDockablePane
//=============================================================================
IMPLEMENT_DYNAMIC(CDockablePane, CPane)

CDockablePane::CDockablePane() {
    memset(_dockablepane_padding, 0, sizeof(_dockablepane_padding));
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[this];
    state.canFloat = TRUE;
    state.canAutoHide = TRUE;
    state.hasGripper = TRUE;
}
CDockablePane::~CDockablePane() {}

BOOL CDockablePane::CanBeAttached() const { return TRUE; }
BOOL CDockablePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.canAutoHide : TRUE;
}
void CDockablePane::EnableAutohideAll() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[this].canAutoHide = TRUE;
}
CMFCAutoHideBar* CDockablePane::SetAutoHideMode(int bAutoHideMode, unsigned long dwAlignment, void* pAutoHideBar, int) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[this];
    state.autoHideMode = bAutoHideMode ? TRUE : FALSE;
    state.autoHideAlignment = bAutoHideMode ? dwAlignment : 0;
    state.autoHideBar = bAutoHideMode ? pAutoHideBar : nullptr;
    state.canAutoHide = TRUE;
    return reinterpret_cast<CMFCAutoHideBar*>(state.autoHideBar);
}
// Old convenience overloads (non-MSDN API)
BOOL CDockablePane::IsAutoHideMode() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.autoHideMode : FALSE;
}
BOOL CDockablePane::IsTabbed() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    auto it = g_paneCoreState.find(this);
    return it != g_paneCoreState.end() ? it->second.tabbed : FALSE;
}

//=============================================================================
// CBaseTabbedPane
//=============================================================================
IMPLEMENT_DYNAMIC(CBaseTabbedPane, CDockablePane)

CBaseTabbedPane::CBaseTabbedPane(int iStyle) {
    memset(_basetabbedpane_padding, 0, sizeof(_basetabbedpane_padding));
    (void)iStyle;
}

CBaseTabbedPane::~CBaseTabbedPane() {}

// Symbol: ??0CBaseTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CBaseTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CBaseTabbedPane(iStyle); }
// Symbol: ??1CBaseTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CBaseTabbedPane__UEAA_XZ(CBaseTabbedPane* pThis) { if (pThis) pThis->~CBaseTabbedPane(); }

//=============================================================================
// CTabbedPane
//=============================================================================
IMPLEMENT_SERIAL(CTabbedPane, CBaseTabbedPane, 0x80000002)

CTabbedPane::CTabbedPane(int iStyle) : CBaseTabbedPane(iStyle) { memset(_tabbedpane_padding, 0, sizeof(_tabbedpane_padding)); }
CTabbedPane::~CTabbedPane() {}

// Symbol: ??0CTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CTabbedPane(iStyle); }
// Symbol: ??1CTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CTabbedPane__UEAA_XZ(CTabbedPane* pThis) { if (pThis) pThis->~CTabbedPane(); }

//=============================================================================
// CUserTool
//=============================================================================
IMPLEMENT_SERIAL(CUserTool, CObject, 0x80000001)

CUserTool::CUserTool() : m_uiCmdId(0), m_hIcon(nullptr) {}

CUserTool::~CUserTool() {}

int CUserTool::Invoke() { return 0; }
void CUserTool::Serialize(CArchive&) {}

// Draws the tool's icon centred inside rectImage. Retail (mfc140u
// ?DrawToolIcon@CUserTool@@) computes
//     x = left + (width  - iconSize) / 2
//     y = top  + (height - iconSize) / 2
// clamping each offset at 0 (cmovns), then calls DrawIconEx(..., DI_NORMAL).
// Retail reads the icon extent from its cached afxGlobalData small-icon size;
// we query the same value from the system directly.
void CUserTool::DrawToolIcon(CDC* pDC, const CRect& rectImage) {
    if (pDC == nullptr || m_hIcon == nullptr) return;

    const int cx = ::GetSystemMetrics(SM_CXSMICON);
    const int cy = ::GetSystemMetrics(SM_CYSMICON);

    int dx = (rectImage.right - rectImage.left - cx) / 2;
    int dy = (rectImage.bottom - rectImage.top - cy) / 2;
    if (dx < 0) dx = 0;
    if (dy < 0) dy = 0;

    ::DrawIconEx(pDC->m_hDC, rectImage.left + dx, rectImage.top + dy,
                 m_hIcon, 0, 0, 0, nullptr, DI_NORMAL);
}

// Symbol: ?DrawToolIcon@CUserTool@@QEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(
    CUserTool* pThis, CDC* pDC, const CRect* pRect) {
    if (pThis && pRect) pThis->DrawToolIcon(pDC, *pRect);
}

// Symbol: ??0CUserTool@@QEAA@XZ
extern "C" void* MS_ABI impl___0CUserTool__QEAA_XZ(void* pThis) { return new (pThis) CUserTool(); }
// Symbol: ??1CUserTool@@UEAA@XZ
extern "C" void MS_ABI impl___1CUserTool__UEAA_XZ(CUserTool* pThis) { if (pThis) pThis->~CUserTool(); }

//=============================================================================
// CUserToolsManager
//=============================================================================
IMPLEMENT_DYNAMIC(CUserToolsManager, CObject)

CUserToolsManager::CUserToolsManager() {
    memset(_usertoolsmanager_padding, 0, sizeof(_usertoolsmanager_padding));
}

CUserToolsManager::CUserToolsManager(int, int, int, CRuntimeClass*, int, int) {
    memset(_usertoolsmanager_padding, 0, sizeof(_usertoolsmanager_padding));
}

CUserToolsManager::~CUserToolsManager() {}

// Symbol: ??0CUserToolsManager@@QEAA@IIIPEAUCRuntimeClass@@II@Z
extern "C" void* MS_ABI impl___0CUserToolsManager__QEAA_IIIPEAUCRuntimeClass__II_Z(
    void* pThis, int p0, int p1, int p2, CRuntimeClass* p3,
    int p4, int p5) {
    return new (pThis) CUserToolsManager(p0, p1, p2, p3, p4, p5);
}

// Symbol: ??0CUserToolsManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CUserToolsManager__QEAA_XZ(void* pThis) { return new (pThis) CUserToolsManager(); }
// Symbol: ??1CUserToolsManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CUserToolsManager__UEAA_XZ(CUserToolsManager* pThis) { if (pThis) pThis->~CUserToolsManager(); }

//=============================================================================
// CMFCToolBarButton
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCToolBarButton, CObject)

// Field defaults match real mfc140u's CMFCToolBarButton::Initialize() exactly
// (harvested via tools/harvest/family). NOTE the non-obvious ones:
// m_iImage = m_iUserImage = -1, and m_bImage / m_bWholeText / m_bHorz / m_bVisible = TRUE.
static void OpenMFC_InitToolBarButton(CMFCToolBarButton* b) {
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

CMFCToolBarButton::CMFCToolBarButton() {
    OpenMFC_InitToolBarButton(this);
}

CMFCToolBarButton::CMFCToolBarButton(UINT uiCmdID, int iImage, const wchar_t* lpszText, BOOL bUserButton, BOOL bLocked) {
    OpenMFC_InitToolBarButton(this);
    m_nID = uiCmdID;
    if (lpszText) m_strText = lpszText;
    m_bUserButton = bUserButton;
    m_bLocked = bLocked;
    if (bUserButton) m_iUserImage = iImage; else m_iImage = iImage;
}

CMFCToolBarButton::~CMFCToolBarButton() {}

//=============================================================================
// CMFCToolBar
//=============================================================================
// Retail base chain is CMFCToolBar : CMFCBaseToolBar : CPane : CBasePane.
IMPLEMENT_DYNAMIC(CMFCBaseToolBar, CPane)

CMFCBaseToolBar::CMFCBaseToolBar() {}
CMFCBaseToolBar::~CMFCBaseToolBar() {}

IMPLEMENT_DYNAMIC(CMFCToolBar, CMFCBaseToolBar)

CMFCToolBar::CMFCToolBar() {
    // The eight CMFCToolBarImages members precede m_bLocked and keep their
    // constructed state. The span zeroed here is NOT purely POD, though: the
    // three CObLists (4488/4544/4600) and m_penDrag (4800) all sit after
    // m_bLocked, so they are re-constructed below. Zeroing already gives an
    // empty list and a null pen handle; what the memset destroys and placement
    // new restores is their vfptrs.
    char* const base = reinterpret_cast<char*>(this);
    std::memset(base + offsetof(CMFCToolBar, m_bLocked), 0,
                sizeof(CMFCToolBar) - offsetof(CMFCToolBar, m_bLocked));
    new (&m_Buttons) CObList();
    new (&m_OrigButtons) CObList();
    new (&m_OrigResetButtons) CObList();
    new (&m_penDrag) CPen();
    m_iButtonCapture = -1;
    m_iHighlighted   = -1;
    m_iSelected      = -1;
    m_iHot           = -1;
    m_iDragIndex     = -1;
    m_iAccHotItem    = -1;
    m_bGrayDisabledButtons = TRUE;
    m_bRouteCommandsViaFrame = TRUE;
    m_bShowHotBorder = TRUE;
}
CMFCToolBar::~CMFCToolBar() {
    ClearToolBarState(this);
}

BOOL CMFCToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    g_toolBarStates[this].parent = pParentWnd;
    m_hWnd = ::CreateWindowExW(0, L"ToolbarWindow32", nullptr,
                                dwStyle, 0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    } else {
        ClearToolBarState(this);
    }
    return m_hWnd != nullptr;
}

BOOL CMFCToolBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    return Create(pParentWnd, dwStyle, nID);
}

BOOL CMFCToolBar::LoadToolBar(UINT, UINT, UINT, BOOL, UINT, UINT, UINT) { return TRUE; }
BOOL CMFCToolBar::LoadBitmap(UINT) { return TRUE; }
BOOL CMFCToolBar::SetButtons(const UINT* lpIDArray, int nIDCount, BOOL bImages) {
    if (nIDCount < 0 || (nIDCount > 0 && lpIDArray == nullptr)) return FALSE;

    ToolBarState& state = g_toolBarStates[this];
    ClearToolBarButtons(state);

    for (int i = 0; i < nIDCount; ++i) {
        UINT id = lpIDArray[i];
        auto* button = new (std::nothrow) CMFCToolBarButton(id, bImages ? i : -1);
        if (!button) {
            ClearToolBarButtons(state);
            return FALSE;
        }
        if (id == 0) {
            button->m_nStyle = TBSTYLE_SEP;
            button->m_iImage = -1;
            button->m_bImage = FALSE;
        }
        state.buttons.push_back(button);
        state.ownedButtons.insert(button);
    }

    if (m_hWnd) {
        int existing = static_cast<int>(::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0));
        while (existing-- > 0) {
            ::SendMessageW(m_hWnd, TB_DELETEBUTTON, 0, 0);
        }

        for (CMFCToolBarButton* button : state.buttons) {
            TBBUTTON tb = {};
            tb.iBitmap = button->m_iImage;
            tb.idCommand = static_cast<int>(button->m_nID);
            tb.fsState = TBSTATE_ENABLED;
            tb.fsStyle = button->m_nID == 0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
            ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, reinterpret_cast<LPARAM>(&tb));
        }
    }

    return TRUE;
}

BOOL CMFCToolBar::ReplaceButton(UINT nID, const CMFCToolBarButton& button, BOOL) {
    ToolBarState& state = g_toolBarStates[this];
    for (size_t i = 0; i < state.buttons.size(); ++i) {
        CMFCToolBarButton* current = state.buttons[i];
        if (!current || current->m_nID != nID) continue;

        CMFCToolBarButton* replacement = CloneToolBarButton(button);
        if (!replacement) return FALSE;

        if (state.ownedButtons.erase(current) != 0) {
            delete current;
        }
        state.buttons[i] = replacement;
        state.ownedButtons.insert(replacement);

        if (m_hWnd) {
            TBBUTTONINFO tbi = {};
            tbi.cbSize = sizeof(TBBUTTONINFO);
            tbi.dwMask = TBIF_COMMAND | TBIF_IMAGE | TBIF_STATE | TBIF_STYLE;
            tbi.idCommand = static_cast<int>(replacement->m_nID);
            tbi.iImage = replacement->m_iImage;
            tbi.fsState = TBSTATE_ENABLED;
            tbi.fsStyle = replacement->m_nID == 0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
            ::SendMessageW(m_hWnd, TB_SETBUTTONINFO, nID, reinterpret_cast<LPARAM>(&tbi));
        }
        return TRUE;
    }
    return FALSE;
}

int CMFCToolBar::GetCount() const {
    auto it = g_toolBarStates.find(this);
    return it == g_toolBarStates.end() ? 0 : static_cast<int>(it->second.buttons.size());
}

CMFCToolBarButton* CMFCToolBar::GetButton(int nIndex) const {
    auto it = g_toolBarStates.find(this);
    if (it == g_toolBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.buttons.size())) return nullptr;
    return it->second.buttons[nIndex];
}

void CMFCToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage) {
    for (auto& entry : g_toolBarStates) {
        entry.second.buttonSize = CSize(sizeButton);
        entry.second.imageSize = CSize(sizeImage);
        if (entry.first && entry.first->GetSafeHwnd()) {
            ::SendMessageW(entry.first->GetSafeHwnd(), TB_SETBUTTONSIZE, 0, MAKELPARAM(sizeButton.cx, sizeButton.cy));
            ::SendMessageW(entry.first->GetSafeHwnd(), TB_SETBITMAPSIZE, 0, MAKELPARAM(sizeImage.cx, sizeImage.cy));
        }
    }
}

CSize CMFCToolBar::GetButtonSize() const {
    auto it = g_toolBarStates.find(this);
    return it == g_toolBarStates.end() ? CSize(23, 22) : it->second.buttonSize;
}

void CMFCToolBar::EnableDocking(DWORD dwDockStyle) {
    g_toolBarStates[this].dockStyle = dwDockStyle;
    // Retail's override is exactly two stores:
    //     mov DWORD PTR [rcx+0x108],0x1   ; m_dwControlBarStyle = 1
    //     mov DWORD PTR [rcx+0x100],edx   ; m_dwEnabledAlignment = dwDockStyle
    // The side table above is what OpenMFC's own docking code reads, but these
    // members are what a real client sees, and previously they were left at
    // zero -- so an MSVC caller could not tell a dockable toolbar from a fixed
    // one. Note the literal is assigned, not OR'd: retail discards any style
    // bits already present, and that is reproduced rather than "corrected".
    m_dwControlBarStyle  = 1;
    m_dwEnabledAlignment = dwDockStyle;
}
void CMFCToolBar::AdjustLayout() {
    ToolBarState& state = g_toolBarStates[this];
    const int count = static_cast<int>(state.buttons.size());
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(state.buttonSize.cx, state.buttonSize.cy));
        ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(state.imageSize.cx, state.imageSize.cy));
        ::SendMessageW(m_hWnd, TB_AUTOSIZE, 0, 0);
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
    }

    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& paneState = g_paneCoreState[this];
    if (paneState.recentRect.IsRectEmpty()) {
        paneState.recentRect = CRect(0, 0, std::max(1, count) * state.buttonSize.cx, state.buttonSize.cy);
    } else if (count > 0) {
        paneState.recentRect.right = paneState.recentRect.left + count * state.buttonSize.cx;
        paneState.recentRect.bottom = paneState.recentRect.top + state.buttonSize.cy;
    }
}
void CMFCToolBar::AdjustSize() {
    AdjustLayout();
}
CString CMFCToolBar::GetButtonText(int nIndex) const {
    CMFCToolBarButton* button = GetButton(nIndex);
    return button ? button->m_strText : CString();
}

void CMFCToolBar::GetButtonText(int nIndex, CString& rString) const {
    rString = GetButtonText(nIndex);
}

//=============================================================================
// CMFCMenuBar
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCMenuBar, CMFCToolBar)

CMFCMenuBar::CMFCMenuBar() {
    memset(_mfcmenubar_padding, 0, sizeof(_mfcmenubar_padding));
}

CMFCMenuBar::~CMFCMenuBar() {
    ClearMenuBarState(this);
}

BOOL CMFCMenuBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    g_menuBarStates[this].parent = pParentWnd;
    return CMFCToolBar::Create(pParentWnd, dwStyle, nID);
}

BOOL CMFCMenuBar::CreateEx(CWnd* pParentWnd, DWORD, DWORD dwStyle, CRect, UINT nID) {
    return Create(pParentWnd, dwStyle, nID);
}

void CMFCMenuBar::CreateFromMenu(HMENU hMenu, BOOL, BOOL) {
    MenuBarState& state = g_menuBarStates[this];
    for (CMFCToolBarButton* item : state.ownedItems) {
        delete item;
    }
    state.items.clear();
    state.ownedItems.clear();
    state.menu = hMenu;

    if (!hMenu) return;
    int count = ::GetMenuItemCount(hMenu);
    for (int i = 0; i < count; ++i) {
        CMFCToolBarMenuButton* button = CreateMenuButtonFromMenu(hMenu, i);
        state.items.push_back(button);
        state.ownedItems.insert(button);
    }
}

CMFCToolBarButton* CMFCMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_menuBarStates.find(this);
    if (it == g_menuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}

static CSize CalcMenuBarFixedLayout(const CMFCMenuBar* pMenuBar) {
    if (!pMenuBar) return CSize();
    const auto it = g_menuBarStates.find(pMenuBar);
    const int count = it == g_menuBarStates.end() ? 0 : static_cast<int>(it->second.items.size());
    return CSize(std::max(23, count * 80), 22);
}

CSize CMFCMenuBar::CalcLayout(DWORD, int nLength) {
    CSize size = CalcMenuBarFixedLayout(this);
    if (nLength > 0) size.cx = nLength;
    return size;
}

void CMFCMenuBar::AdjustLocations() {
    MenuBarState& state = g_menuBarStates[this];
    const int count = static_cast<int>(state.items.size());
    const int height = 22;
    const int width = count > 0 ? std::max(23, (count * 80)) : 23;
    if (GetSafeHwnd()) {
        ::SendMessageW(GetSafeHwnd(), TB_AUTOSIZE, 0, 0);
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[this].recentRect = CRect(0, 0, width, height);
}

CFont& CMFCMenuBar::GetMenuFont(BOOL) {
    return EnsureMenuFont();
}

// Symbol: ?Create@CMFCMenuBar@@UEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CMFCMenuBar__UEAAHPEAVCWnd__KI_Z(CMFCMenuBar* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID) {
    return pThis ? pThis->Create(pParentWnd, dwStyle, nID) : FALSE;
}

// Symbol: ?CreateEx@CMFCMenuBar@@UEAAHPEAVCWnd@@KKVCRect@@I@Z
extern "C" int MS_ABI impl__CreateEx_CMFCMenuBar__UEAAHPEAVCWnd__KKVCRect__I_Z(
    CMFCMenuBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, CRect rcBorders, unsigned int nID) {
    return pThis ? pThis->CreateEx(pParentWnd, dwCtrlStyle, dwStyle, rcBorders, nID) : FALSE;
}

// Symbol: ?CreateFromMenu@CMFCMenuBar@@UEAAXPEAUHMENU__@@HH@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(CMFCMenuBar* pThis, HMENU hMenu, int bDefaultMenu, int bForceUpdate) {
    if (pThis) pThis->CreateFromMenu(hMenu, bDefaultMenu, bForceUpdate);
}

// Symbol: ?GetMenuItem@CMFCMenuBar@@QEBAPEAVCMFCToolBarButton@@H@Z
extern "C" CMFCToolBarButton* MS_ABI impl__GetMenuItem_CMFCMenuBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCMenuBar* pThis, int nIndex) {
    return pThis ? pThis->GetMenuItem(nIndex) : nullptr;
}

// Symbol: ?CalcFixedLayout@CMFCMenuBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CMFCMenuBar__UEAA_AVCSize__HH_Z(CSize* pRet, CMFCMenuBar* pThis, int bStretch, int bHorz) {
    (void)bStretch;
    (void)bHorz;
    new (pRet) CSize(CalcMenuBarFixedLayout(pThis));
}

// Symbol: ?CalcLayout@CMFCMenuBar@@UEAA?AVCSize@@KH@Z
extern "C" void MS_ABI impl__CalcLayout_CMFCMenuBar__UEAA_AVCSize__KH_Z(CSize* pRet, CMFCMenuBar* pThis, unsigned long dwMode, int nLength) {
    new (pRet) CSize(pThis ? pThis->CalcLayout(dwMode, nLength) : CSize());
}

// Symbol: ?AdjustLocations@CMFCMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCMenuBar__UEAAXXZ(CMFCMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}

// Symbol: ?GetMenuFont@CMFCMenuBar@@SAAEBVCFont@@H@Z
extern "C" CFont* MS_ABI impl__GetMenuFont_CMFCMenuBar__SAAEBVCFont__H_Z(int bHorz) {
    return &CMFCMenuBar::GetMenuFont(bHorz);
}

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
    g_ribbonPanelStates[this];
}
CMFCRibbonPanel::~CMFCRibbonPanel() {
    ClearRibbonPanelState(this);
}

void CMFCRibbonPanel::Add(CMFCRibbonBaseElement* pElem) {
    if (!pElem) return;
    RibbonPanelState& state = g_ribbonPanelStates[this];
    state.elements.push_back(pElem);
    state.ownedElements.insert(pElem);
}
void CMFCRibbonPanel::AddSeparator() {
    g_ribbonPanelStates[this].elements.push_back(nullptr);
}
int CMFCRibbonPanel::GetCount() const {
    auto it = g_ribbonPanelStates.find(this);
    return it == g_ribbonPanelStates.end() ? 0 : static_cast<int>(it->second.elements.size());
}
CMFCRibbonBaseElement* CMFCRibbonPanel::GetElement(int nIndex) const {
    auto it = g_ribbonPanelStates.find(this);
    if (it == g_ribbonPanelStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.elements.size())) return nullptr;
    return it->second.elements[static_cast<size_t>(nIndex)];
}
CString CMFCRibbonPanel::GetName() const { return m_strName; }

//=============================================================================
// CMFCRibbonCategory
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonCategory, CObject)

CMFCRibbonCategory::CMFCRibbonCategory(const wchar_t* lpszName, UINT, UINT) {
    if (lpszName) m_strName = lpszName;
    memset(_ribboncategory_padding, 0, sizeof(_ribboncategory_padding));
    g_ribbonCategoryStates[this];
}
CMFCRibbonCategory::~CMFCRibbonCategory() {
    ClearRibbonCategoryState(this);
}

CMFCRibbonPanel* CMFCRibbonCategory::AddPanel(const wchar_t* lpszLabel, HICON hIcon, CRuntimeClass*) {
    auto* panel = new (std::nothrow) CMFCRibbonPanel(lpszLabel, hIcon);
    if (!panel) return nullptr;

    RibbonCategoryState& state = g_ribbonCategoryStates[this];
    state.panels.push_back(panel);
    state.ownedPanels.insert(panel);
    return panel;
}
// Old convenience overloads (non-MSDN API)
void CMFCRibbonCategory::AddPanel(CMFCRibbonPanel* pPanel) {
    if (!pPanel) return;
    RibbonCategoryState& state = g_ribbonCategoryStates[this];
    if (std::find(state.panels.begin(), state.panels.end(), pPanel) == state.panels.end()) {
        state.panels.push_back(pPanel);
    }
    state.ownedPanels.insert(pPanel);
    g_ribbonPanelStates[pPanel];
}
int CMFCRibbonCategory::GetPanelCount() const {
    auto it = g_ribbonCategoryStates.find(this);
    return it == g_ribbonCategoryStates.end() ? 0 : static_cast<int>(it->second.panels.size());
}
CMFCRibbonPanel* CMFCRibbonCategory::GetPanel(int nIndex) const {
    auto it = g_ribbonCategoryStates.find(this);
    if (it == g_ribbonCategoryStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.panels.size())) return nullptr;
    return it->second.panels[static_cast<size_t>(nIndex)];
}
CString CMFCRibbonCategory::GetName() const { return m_strName; }

//=============================================================================
// CMFCRibbonBar
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCRibbonBar, CPane)

// Retail layout, pinned. Transcribed from /d1reportSingleClassLayout and then
// confirmed against the shipping constructor's own stores: it writes the ctor
// argument to 0x468 (m_bReplaceFrameCaption), constructs a subobject at 0x498
// (m_TabElements), and initializes CArray vtables at 0xb28/0xb50/0xb78/0xba0
// (the four arrays, 40 bytes apart) -- all exactly where this layout puts them.
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

CMFCRibbonBar::CMFCRibbonBar(BOOL bReplaceFrameCaption) {
    // Every member in this span is POD -- the four large embedded ribbon
    // objects are deliberately opaque byte blocks (see afxmfc.h), and CRect /
    // CSize are plain structs -- so unlike the CMFCToolBar and
    // CMFCToolBarImages constructors there is nothing here whose vfptr a
    // blanket memset could destroy. If any of those blocks is ever replaced by
    // a real class, this memset has to be split around it.
    char* const base = reinterpret_cast<char*>(this);
    std::memset(base + sizeof(CPane), 0, sizeof(CMFCRibbonBar) - sizeof(CPane));

    // Non-zero defaults, read off the retail constructor rather than guessed.
    // It zeroes via r15 and writes ebp==1 / eax==-1 to these slots:
    m_nHighlightedTab         = -1;    // 0x404, eax
    m_nKeyboardNavLevel       = -1;    // 0x414, eax
    m_nTooltipWidthRegular    = 210;   // 0x41c, literal 0xd2
    m_nTooltipWidthLargeImage = 318;   // 0x420, literal 0x13e
    m_bRecalcCategoryHeight   = TRUE;  // 0x424, ebp
    m_bRecalcCategoryWidth    = TRUE;  // 0x428, rbp (low half; 0x42c stays 0)
    m_bIsPrintPreview         = TRUE;  // 0x430, ebp
    m_bToolTip                = TRUE;  // 0x450, ebp
    m_bToolTipDescr           = TRUE;  // 0x454, ebp
    m_bKeyTips                = TRUE;  // 0x458, rbp (low half; 0x45c stays 0)
    m_bReplaceFrameCaption    = bReplaceFrameCaption;  // 0x468, the ctor arg

    g_ribbonBarStates[this];
}
CMFCRibbonBar::~CMFCRibbonBar() {
    ClearRibbonBarState(this);
}

BOOL CMFCRibbonBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    g_ribbonBarStates[this].parent = pParentWnd;
    m_hWnd = ::CreateWindowExW(0, L"STATIC", nullptr,
                                dwStyle, 0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    if (!m_hWnd) {
        ClearRibbonBarState(this);
        return FALSE;
    }
    return TRUE;
}
CMFCRibbonCategory* CMFCRibbonBar::AddCategory(const wchar_t* lpszName, unsigned int uiSmallImage, unsigned int uiLargeImage, CSize, unsigned int, int nInsertAt, CRuntimeClass*) {
    auto* category = new (std::nothrow) CMFCRibbonCategory(lpszName, uiSmallImage, uiLargeImage);
    if (!category) return nullptr;

    RibbonBarState& state = g_ribbonBarStates[this];
    auto insertPos = state.categories.end();
    if (nInsertAt >= 0 && nInsertAt < static_cast<int>(state.categories.size())) {
        insertPos = state.categories.begin() + nInsertAt;
    }
    state.categories.insert(insertPos, category);
    state.ownedCategories.insert(category);
    if (!state.activeCategory) {
        state.activeCategory = category;
    }
    return category;
}
// Old convenience overloads (non-MSDN API)
BOOL CMFCRibbonBar::AddCategory(CMFCRibbonCategory* pCategory) {
    if (!pCategory) return FALSE;
    RibbonBarState& state = g_ribbonBarStates[this];
    if (std::find(state.categories.begin(), state.categories.end(), pCategory) == state.categories.end()) {
        state.categories.push_back(pCategory);
    }
    state.ownedCategories.insert(pCategory);
    g_ribbonCategoryStates[pCategory];
    if (!state.activeCategory) {
        state.activeCategory = pCategory;
    }
    return TRUE;
}
int CMFCRibbonBar::GetCategoryCount() const {
    auto it = g_ribbonBarStates.find(this);
    return it == g_ribbonBarStates.end() ? 0 : static_cast<int>(it->second.categories.size());
}
CMFCRibbonCategory* CMFCRibbonBar::GetCategory(int nIndex) const {
    auto it = g_ribbonBarStates.find(this);
    if (it == g_ribbonBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.categories.size())) return nullptr;
    return it->second.categories[static_cast<size_t>(nIndex)];
}
BOOL CMFCRibbonBar::SetActiveCategory(CMFCRibbonCategory* pCategory, BOOL) {
    RibbonBarState& state = g_ribbonBarStates[this];
    if (!pCategory) {
        state.activeCategory = nullptr;
        return TRUE;
    }
    if (std::find(state.categories.begin(), state.categories.end(), pCategory) == state.categories.end()) {
        return FALSE;
    }
    state.activeCategory = pCategory;
    return TRUE;
}
void CMFCRibbonBar::AddToTabs(CMFCRibbonBaseElement* pElement) {
    if (!pElement) return;
    auto& tabs = g_ribbonBarStates[this].tabs;
    if (std::find(tabs.begin(), tabs.end(), pElement) == tabs.end()) {
        tabs.push_back(pElement);
    }
}
void CMFCRibbonBar::SetQuickAccessToolbar(CMFCRibbonButtonsGroup* pQAT) {
    g_ribbonBarStates[this].quickAccessToolbar = pQAT;
}
void CMFCRibbonBar::SetQuickAccessToolbarOnTop(BOOL bOnTop) {
    g_ribbonBarStates[this].quickAccessToolbarOnTop = bOnTop != FALSE;
}
void CMFCRibbonBar::ToggleMimimizeState() {
    RibbonBarState& state = g_ribbonBarStates[this];
    state.minimized = !state.minimized;
}
BOOL CMFCRibbonBar::IsMinimized() const {
    auto it = g_ribbonBarStates.find(this);
    return it != g_ribbonBarStates.end() && it->second.minimized ? TRUE : FALSE;
}

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
CMFCButton::~CMFCButton() {
    RemoveMFCButtonState(this);
}

void CMFCButton::SetImage(HICON hIcon, BOOL bAutoDestroy, HICON hIconHot, BOOL) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    if (state.ownsIcon && state.icon && state.icon != hIcon) ::DestroyIcon(state.icon);
    if (state.ownsHotIcon && state.iconHot && state.iconHot != hIconHot) ::DestroyIcon(state.iconHot);
    state.icon = hIcon;
    state.iconHot = hIconHot;
    state.ownsIcon = bAutoDestroy != FALSE;
    state.ownsHotIcon = bAutoDestroy != FALSE && hIconHot != nullptr;
    state.ownerDraw = (hIcon != nullptr || hIconHot != nullptr);
    if (GetSafeHwnd()) Invalidate(FALSE);
}
void CMFCButton::SetFaceColor(COLORREF crFace, BOOL bRedraw) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    state.faceColor = crFace;
    state.ownerDraw = true;
    if (bRedraw && GetSafeHwnd()) Invalidate(FALSE);
}
void CMFCButton::SetTextColor(COLORREF crText) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    state.textColor = crText;
    state.ownerDraw = true;
    if (GetSafeHwnd()) Invalidate(FALSE);
}
void CMFCButton::SetTextHotColor(COLORREF crTextHot) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    state.textHotColor = crTextHot;
    state.ownerDraw = true;
    if (GetSafeHwnd()) Invalidate(FALSE);
}
BOOL CMFCButton::IsOwnerDraw() const {
    auto it = g_mfcButtonStates.find(this);
    return it != g_mfcButtonStates.end() && it->second.ownerDraw ? TRUE : FALSE;
}
void CMFCButton::EnableFullTextTooltip(BOOL bEnable) {
    EnsureMFCButtonState(this).fullTextTooltip = bEnable != FALSE;
}
void CMFCButton::SetTooltip(const wchar_t* lpszToolTipText) {
    EnsureMFCButtonState(this).tooltip = lpszToolTipText ? lpszToolTipText : L"";
}

//=============================================================================
// CMFCPopupMenu
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPopupMenu, CMiniFrameWnd)

static CMFCPopupMenu* g_pActivePopupMenu = nullptr;

CMFCPopupMenu::CMFCPopupMenu() {
    memset(_mfcpopupmenu_padding, 0, sizeof(_mfcpopupmenu_padding));
}
CMFCPopupMenu::~CMFCPopupMenu() {
    if (g_pActivePopupMenu == this) {
        g_pActivePopupMenu = nullptr;
    }
    ClearPopupMenuState(this);
}

CMFCPopupMenu* CMFCPopupMenu::GetActiveMenu() { return g_pActivePopupMenu; }
CMFCPopupMenu* CMFCPopupMenu::GetSafeActivePopupMenu() { return g_pActivePopupMenu; }
void CMFCPopupMenu::SetForceMenuFocus(BOOL bForceFocus) { g_forceMenuFocus = bForceFocus; }
BOOL CMFCPopupMenu::ActivatePopupMenu(CFrameWnd*, CMFCPopupMenu* pPopupMenu) {
    g_pActivePopupMenu = pPopupMenu;
    return pPopupMenu != nullptr;
}

BOOL CMFCPopupMenu::Create(CWnd* pParentWnd, int x, int y, HMENU hMenu, BOOL bLocked, BOOL bIsMainMenu) {
    PopupMenuState& state = g_popupMenuStates[this];
    state.parent = pParentWnd;
    state.x = x;
    state.y = y;
    state.menu = hMenu;
    state.locked = bLocked != FALSE;
    state.mainMenu = bIsMainMenu != FALSE;
    state.selected = -1;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    g_pActivePopupMenu = this;
    return TRUE;
}

void CMFCPopupMenu::CloseMenu(BOOL) {
    if (g_pActivePopupMenu == this) {
        g_pActivePopupMenu = nullptr;
    }
}

int CMFCPopupMenu::GetMenuItemCount() const {
    auto it = g_popupMenuStates.find(this);
    return it == g_popupMenuStates.end() ? 0 : static_cast<int>(it->second.items.size());
}

CMFCToolBarMenuButton* CMFCPopupMenu::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}

CMFCToolBarMenuButton* CMFCPopupMenu::GetSelItem() {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    return GetMenuItem(it->second.selected);
}

CMFCToolBarMenuButton* CMFCPopupMenu::FindSubItemByCommand(UINT uiCmd) const {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (item && item->m_nID == uiCmd) return item;
    }
    return nullptr;
}

BOOL CMFCPopupMenu::InsertItem(const CMFCToolBarMenuButton& button, int iInsertAt) {
    PopupMenuState& state = g_popupMenuStates[this];
    CMFCToolBarMenuButton* copy = new CMFCToolBarMenuButton(button);
    if (iInsertAt < 0 || iInsertAt >= static_cast<int>(state.items.size())) {
        state.items.push_back(copy);
    } else {
        state.items.insert(state.items.begin() + iInsertAt, copy);
    }
    state.ownedItems.insert(copy);
    return TRUE;
}

BOOL CMFCPopupMenu::InsertSeparator(int iInsertAt) {
    CMFCToolBarMenuButton separator(0, nullptr, FALSE, nullptr, FALSE);
    return InsertItem(separator, iInsertAt);
}

void CMFCPopupMenu::EnableResize(CSize sizeMinResize) {
    g_popupMenuStates[this].minResize = sizeMinResize;
}

void CMFCPopupMenu::EnableVertResize(BOOL bEnable) {
    g_popupMenuStates[this].vertResize = bEnable != FALSE;
}

BOOL CMFCPopupMenu::HideRarelyUsedCommands() const {
    return FALSE;
}

// Symbol: ?ActivatePopupMenu@CMFCPopupMenu@@SAHPEAVCFrameWnd@@PEAV1@@Z
extern "C" int MS_ABI impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(CFrameWnd* pTopFrame, CMFCPopupMenu* pPopupMenu) {
    return CMFCPopupMenu::ActivatePopupMenu(pTopFrame, pPopupMenu);
}

// Symbol: ?GetSafeActivePopupMenu@CMFCPopupMenu@@SAPEAV1@XZ
extern "C" CMFCPopupMenu* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() {
    return CMFCPopupMenu::GetSafeActivePopupMenu();
}

// Symbol: ?GetMenuItemCount@CMFCPopupMenu@@QEBAHXZ
extern "C" int MS_ABI impl__GetMenuItemCount_CMFCPopupMenu__QEBAHXZ(const CMFCPopupMenu* pThis) {
    return pThis ? pThis->GetMenuItemCount() : 0;
}

// Symbol: ?GetMenuItem@CMFCPopupMenu@@QEBAPEAVCMFCToolBarMenuButton@@H@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetMenuItem_CMFCPopupMenu__QEBAPEAVCMFCToolBarMenuButton__H_Z(const CMFCPopupMenu* pThis, int nIndex) {
    return pThis ? pThis->GetMenuItem(nIndex) : nullptr;
}

// Symbol: ?GetSelItem@CMFCPopupMenu@@QEAAPEAVCMFCToolBarMenuButton@@XZ
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetSelItem_CMFCPopupMenu__QEAAPEAVCMFCToolBarMenuButton__XZ(CMFCPopupMenu* pThis) {
    return pThis ? pThis->GetSelItem() : nullptr;
}

// Symbol: ?FindSubItemByCommand@CMFCPopupMenu@@QEBAPEAVCMFCToolBarMenuButton@@I@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__FindSubItemByCommand_CMFCPopupMenu__QEBAPEAVCMFCToolBarMenuButton__I_Z(const CMFCPopupMenu* pThis, unsigned int uiCmd) {
    return pThis ? pThis->FindSubItemByCommand(uiCmd) : nullptr;
}

// Symbol: ?InsertItem@CMFCPopupMenu@@QEAAHAEBVCMFCToolBarMenuButton@@H@Z
extern "C" int MS_ABI impl__InsertItem_CMFCPopupMenu__QEAAHAEBVCMFCToolBarMenuButton__H_Z(CMFCPopupMenu* pThis, const CMFCToolBarMenuButton* pButton, int iInsertAt) {
    return (pThis && pButton) ? pThis->InsertItem(*pButton, iInsertAt) : FALSE;
}

// Symbol: ?InsertSeparator@CMFCPopupMenu@@QEAAHH@Z
extern "C" int MS_ABI impl__InsertSeparator_CMFCPopupMenu__QEAAHH_Z(CMFCPopupMenu* pThis, int iInsertAt) {
    return pThis ? pThis->InsertSeparator(iInsertAt) : FALSE;
}

// Symbol: ?CloseMenu@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__CloseMenu_CMFCPopupMenu__QEAAXH_Z(CMFCPopupMenu* pThis, int bSetFocusToBar) {
    if (pThis) pThis->CloseMenu(bSetFocusToBar);
}

// Symbol: ?EnableResize@CMFCPopupMenu@@QEAAXVCSize@@@Z
extern "C" void MS_ABI impl__EnableResize_CMFCPopupMenu__QEAAXVCSize___Z(CMFCPopupMenu* pThis, CSize sizeMinResize) {
    if (pThis) pThis->EnableResize(sizeMinResize);
}

// Symbol: ?EnableVertResize@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableVertResize_CMFCPopupMenu__QEAAXH_Z(CMFCPopupMenu* pThis, int bEnable) {
    if (pThis) pThis->EnableVertResize(bEnable);
}

// Symbol: ?HideRarelyUsedCommands@CMFCPopupMenu@@QEBAHXZ
extern "C" int MS_ABI impl__HideRarelyUsedCommands_CMFCPopupMenu__QEBAHXZ(const CMFCPopupMenu* pThis) {
    return pThis ? pThis->HideRarelyUsedCommands() : FALSE;
}

//=============================================================================
// CMFCPopupMenuBar
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPopupMenuBar, CMFCToolBar)

CMFCPopupMenuBar::CMFCPopupMenuBar() {
    memset(_mfcpopupmenubar_padding, 0, sizeof(_mfcpopupmenubar_padding));
}

CMFCPopupMenuBar::~CMFCPopupMenuBar() {
    ClearPopupMenuBarState(this);
}

BOOL CMFCPopupMenuBar::ImportFromMenu(HMENU hMenu, BOOL) {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.menu = hMenu;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    return TRUE;
}

HMENU CMFCPopupMenuBar::ExportToMenu() const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (it->second.menu) return it->second.menu;

    HMENU hMenu = ::CreatePopupMenu();
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (!item) continue;
        if (item->m_hMenu) {
            ::AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(item->m_hMenu), static_cast<const wchar_t*>(item->m_strText));
        } else {
            ::AppendMenuW(hMenu, MF_STRING, static_cast<UINT_PTR>(item->m_nID), static_cast<const wchar_t*>(item->m_strText));
        }
    }
    return hMenu;
}

BOOL CMFCPopupMenuBar::BuildOrigItems(UINT) { return TRUE; }

CMFCToolBarMenuButton* CMFCPopupMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}

int CMFCPopupMenuBar::GetGutterWidth() const { return 22; }

CSize CMFCPopupMenuBar::CalcSize(BOOL) {
    auto it = g_popupMenuBarStates.find(this);
    const int count = it == g_popupMenuBarStates.end() ? 0 : static_cast<int>(it->second.items.size());
    return CSize(160, std::max(22, count * 22));
}

void CMFCPopupMenuBar::AdjustLayout() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    if (GetSafeHwnd()) {
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
        ::UpdateWindow(GetSafeHwnd());
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[this].recentRect = CRect(0, 0, 160, std::max(22, static_cast<int>(state.items.size()) * 22));
}
void CMFCPopupMenuBar::AdjustLocations() {
    AdjustLayout();
}
void CMFCPopupMenuBar::CloseDelayedSubMenu() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.delayedSubMenuOpen = false;
    state.selected = -1;
}

// Symbol: ?ImportFromMenu@CMFCPopupMenuBar@@UEAAHPEAUHMENU__@@H@Z
extern "C" int MS_ABI impl__ImportFromMenu_CMFCPopupMenuBar__UEAAHPEAUHMENU____H_Z(CMFCPopupMenuBar* pThis, HMENU hMenu, int bShowAllCommands) {
    return pThis ? pThis->ImportFromMenu(hMenu, bShowAllCommands) : FALSE;
}

// Symbol: ?ExportToMenu@CMFCPopupMenuBar@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__ExportToMenu_CMFCPopupMenuBar__UEBAPEAUHMENU____XZ(const CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->ExportToMenu() : nullptr;
}

// Symbol: ?BuildOrigItems@CMFCPopupMenuBar@@QEAAHI@Z
extern "C" int MS_ABI impl__BuildOrigItems_CMFCPopupMenuBar__QEAAHI_Z(CMFCPopupMenuBar* pThis, unsigned int uiMenuResID) {
    return pThis ? pThis->BuildOrigItems(uiMenuResID) : FALSE;
}

// Symbol: ?GetGutterWidth@CMFCPopupMenuBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->GetGutterWidth() : 0;
}

// Symbol: ?CalcSize@CMFCPopupMenuBar@@MEAA?AVCSize@@H@Z
extern "C" void MS_ABI impl__CalcSize_CMFCPopupMenuBar__MEAA_AVCSize__H_Z(CSize* pRet, CMFCPopupMenuBar* pThis, int bVertDock) {
    new (pRet) CSize(pThis ? pThis->CalcSize(bVertDock) : CSize());
}

// Symbol: ?AdjustLayout@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLayout();
}

// Symbol: ?AdjustLocations@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}

// Symbol: ?CloseDelayedSubMenu@CMFCPopupMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->CloseDelayedSubMenu();
}

//=============================================================================
// CMFCBaseTabCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCBaseTabCtrl, CWnd)

CMFCBaseTabCtrl::CMFCBaseTabCtrl() {
    memset(_mfcbasetabctrl_padding, 0, sizeof(_mfcbasetabctrl_padding));
}
CMFCBaseTabCtrl::~CMFCBaseTabCtrl() {
    RemoveTabCtrlState(this);
}

BOOL CMFCBaseTabCtrl::Create(DWORD, const RECT&, CWnd*, UINT) { return TRUE; }
int CMFCBaseTabCtrl::GetTabsCount() const {
    const TabCtrlState* state = FindTabCtrlState(this);
    return state ? static_cast<int>(state->tabs.size()) : 0;
}

void CMFCBaseTabCtrl::AddTab(CWnd* pWnd, const wchar_t* lpszLabel, UINT uiId, BOOL bDetachable) {
    if (!pWnd) {
        return;
    }

    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabs.push_back({
        pWnd,
        lpszLabel ? lpszLabel : L"",
        uiId,
        bDetachable,
    });
    if (state.activeTab < 0) {
        state.activeTab = 0;
    }
}

int CMFCBaseTabCtrl::RemoveTab(int nIndex, int) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.tabs.size())) {
        return 0;
    }

    state.tabs.erase(state.tabs.begin() + nIndex);

    if (state.tabs.empty()) {
        state.activeTab = -1;
    } else if (state.activeTab == nIndex) {
        state.activeTab = (nIndex == 0) ? 0 : nIndex - 1;
    } else if (state.activeTab > nIndex) {
        --state.activeTab;
    }

    return 1;
}

void CMFCBaseTabCtrl::SetActiveTab(int nIndex) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.tabs.size())) {
        state.activeTab = -1;
        return;
    }
    state.activeTab = nIndex;
}

int CMFCBaseTabCtrl::GetActiveTab() const {
    const TabCtrlState* state = FindTabCtrlState(this);
    return state ? state->activeTab : -1;
}

CWnd* CMFCBaseTabCtrl::GetTabWnd(int nIndex) const {
    const TabCtrlState* state = FindTabCtrlState(this);
    if (!state) {
        return nullptr;
    }
    if (nIndex < 0 || nIndex >= static_cast<int>(state->tabs.size())) {
        return nullptr;
    }
    return state->tabs[nIndex].window;
}

BOOL CMFCBaseTabCtrl::IsTabDetachable(int nIndex) const {
    const TabCtrlState* state = FindTabCtrlState(this);
    if (!state) {
        return FALSE;
    }
    if (nIndex < 0 || nIndex >= static_cast<int>(state->tabs.size())) {
        return FALSE;
    }
    return state->tabs[nIndex].detachable;
}

//=============================================================================
// CMFCTabCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCTabCtrl, CMFCBaseTabCtrl)

CMFCTabCtrl::CMFCTabCtrl() {
    memset(_mfctabctrl_padding, 0, sizeof(_mfctabctrl_padding));
}
CMFCTabCtrl::~CMFCTabCtrl() {}

void CMFCTabCtrl::EnableTabSwap(BOOL bEnable) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabSwapEnabled = bEnable != FALSE;
}

void CMFCTabCtrl::SetTabBorderSize(int nTabBorderSize) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabBorderSize = nTabBorderSize;
}

void CMFCTabCtrl::SetTabsHeight(int nTabHeight) {
    TabCtrlState& state = EnsureTabCtrlState(this);
    state.tabsHeight = nTabHeight;
}

namespace {

struct PropertyGridVariantValue {
    VARIANT value;

    PropertyGridVariantValue() {
        VariantInit(&value);
    }

    PropertyGridVariantValue(const PropertyGridVariantValue& other) {
        VariantInit(&value);
        VariantCopy(&value, const_cast<VARIANT*>(&other.value));
    }

    PropertyGridVariantValue(PropertyGridVariantValue&& other) noexcept {
        value = other.value;
        VariantInit(&other.value);
    }

    ~PropertyGridVariantValue() {
        VariantClear(&value);
    }

    PropertyGridVariantValue& operator=(const PropertyGridVariantValue& other) {
        if (this != &other) {
            VariantClear(&value);
            VariantInit(&value);
            VariantCopy(&value, const_cast<VARIANT*>(&other.value));
        }
        return *this;
    }

    PropertyGridVariantValue& operator=(PropertyGridVariantValue&& other) noexcept {
        if (this != &other) {
            VariantClear(&value);
            value = other.value;
            VariantInit(&other.value);
        }
        return *this;
    }

    void Assign(const VARIANT& source) {
        VariantClear(&value);
        VariantInit(&value);
        VariantCopy(&value, const_cast<VARIANT*>(&source));
    }
};

struct PropertyGridPropertyState {
    std::vector<CMFCPropertyGridProperty*> subItems;
    std::vector<std::wstring> options;
    CMFCPropertyGridProperty* parent = nullptr;
    CMFCPropertyGridCtrl* owner = nullptr;
    PropertyGridVariantValue originalValue;
    BOOL hasOriginalValue = FALSE;
    int selectedOption = -1;
    BOOL spinEnabled = FALSE;
    int spinMin = 0;
    int spinMax = 0;
};

struct PropertyGridCtrlState {
    std::vector<CMFCPropertyGridProperty*> properties;
    CMFCPropertyGridProperty* current = nullptr;
    int descriptionRows = 0;
    int layoutRevision = 0;
    std::wstring boolTrue = L"True";
    std::wstring boolFalse = L"False";
    wchar_t listDelimiter = L',';
    BOOL alphabeticMode = FALSE;
    BOOL groupNameFullWidth = FALSE;
    COLORREF customColors[7] = {
        RGB(0, 0, 0),
        RGB(128, 128, 128),
        RGB(128, 0, 0),
        RGB(0, 128, 0),
        RGB(0, 0, 128),
        RGB(255, 255, 255),
        RGB(255, 255, 0)
    };
};

#ifndef CHILDID_SELF
#define CHILDID_SELF 0
#endif

#ifndef ROLE_SYSTEM_OUTLINE
#define ROLE_SYSTEM_OUTLINE 0x23
#endif

#ifndef ROLE_SYSTEM_OUTLINEITEM
#define ROLE_SYSTEM_OUTLINEITEM 0x24
#endif

#ifndef STATE_SYSTEM_UNAVAILABLE
#define STATE_SYSTEM_UNAVAILABLE 0x00000001
#endif

#ifndef STATE_SYSTEM_SELECTED
#define STATE_SYSTEM_SELECTED 0x00000002
#endif

#ifndef STATE_SYSTEM_FOCUSED
#define STATE_SYSTEM_FOCUSED 0x00000004
#endif

#ifndef STATE_SYSTEM_INVISIBLE
#define STATE_SYSTEM_INVISIBLE 0x00008000
#endif

#ifndef STATE_SYSTEM_EXPANDED
#define STATE_SYSTEM_EXPANDED 0x00000200
#endif

#ifndef STATE_SYSTEM_COLLAPSED
#define STATE_SYSTEM_COLLAPSED 0x00000400
#endif

struct TasksPaneTaskState {
    UINT id = 0;
    std::wstring label;
    int icon = -1;
    BOOL isSeparator = FALSE;
    UINT commandID = 0;
    unsigned __int64 userData = 0;
};

struct TasksPaneGroupState {
    struct TaskEntry {
        CMFCTasksPaneTask* task = nullptr;
        std::wstring label;
        int icon = -1;
        UINT commandID = 0;
        unsigned __int64 userData = 0;
    };

    std::wstring name;
    BOOL hasGripper = FALSE;
    BOOL isSpecial = FALSE;
    int icon = -1;
    HICON hIcon = nullptr;
    std::vector<TaskEntry> tasks;
};

struct TasksPaneState {
    std::wstring caption;
    std::vector<TasksPaneGroupState> groups;
};

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

void SetPropertyGridOwnerRecursive(CMFCPropertyGridProperty* pProp, CMFCPropertyGridCtrl* pOwner);

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

void SetPropertyGridOwnerRecursive(CMFCPropertyGridProperty* pProp, CMFCPropertyGridCtrl* pOwner) {
    if (!pProp) return;
    auto& state = EnsurePropertyGridPropertyState(pProp);
    state.owner = pOwner;
    for (CMFCPropertyGridProperty* child : state.subItems) {
        SetPropertyGridOwnerRecursive(child, pOwner);
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

std::wstring PropertyGridVariantToString(const VARIANT& value, const PropertyGridCtrlState* ctrlState);

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

std::wstring PropertyGridVariantToString(const VARIANT& value, const PropertyGridCtrlState* ctrlState = nullptr) {
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

template <typename Compare>
void SortPropertyGridSubItemsRecursive(CMFCPropertyGridProperty* pProp, Compare compare) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
    if (!state) return;
    std::stable_sort(state->subItems.begin(), state->subItems.end(), compare);
    for (CMFCPropertyGridProperty* child : state->subItems) {
        SortPropertyGridSubItemsRecursive(child, compare);
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

} // namespace

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
    auto& state = EnsurePropertyGridPropertyState(this);
    state.originalValue.Assign(m_varValue);
    state.hasOriginalValue = TRUE;
}
CMFCPropertyGridProperty::~CMFCPropertyGridProperty() {
    DeletePropertyGridChildren(this);
    RemovePropertyGridPropertyReferences(this);
}

const CString& CMFCPropertyGridProperty::GetName() const { return m_strName; }
const COleVariant& CMFCPropertyGridProperty::GetValue() const { return m_varValue; }
void CMFCPropertyGridProperty::SetValue(const COleVariant& varValue) {
    AssignPropertyGridVariant(m_varValue, varValue);
    m_bModified = IsValueChanged();
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::IsModified() const { return m_bModified; }
void CMFCPropertyGridProperty::SetModified(BOOL bModified) {
    m_bModified = bModified;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::IsEnabled() const { return m_bEnabled; }
void CMFCPropertyGridProperty::SetEnabled(BOOL bEnable) {
    m_bEnabled = bEnable;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::IsVisible() const { return m_bVisible; }
void CMFCPropertyGridProperty::Show(BOOL bShow) { m_bVisible = bShow; }
void CMFCPropertyGridProperty::Show(BOOL bShow, BOOL bAdjustLayout) {
    Show(bShow);
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, bAdjustLayout);
    }
}
int CMFCPropertyGridProperty::AddSubItem(CMFCPropertyGridProperty* pProp) {
    if (!pProp || pProp == this) return -1;
    if (IsPropertyGridAncestorOf(pProp, this)) return -1;

    auto& parentState = EnsurePropertyGridPropertyState(this);
    auto& subItems = parentState.subItems;
    auto it = std::find(subItems.begin(), subItems.end(), pProp);
    if (it != subItems.end()) return static_cast<int>(it - subItems.begin());

    DetachPropertyFromParent(pProp);
    DetachPropertyFromGrid(pProp);
    EnsurePropertyGridPropertyState(pProp).parent = this;
    SetPropertyGridOwnerRecursive(pProp, parentState.owner);

    subItems.push_back(pProp);
    TouchPropertyGridCtrl(parentState.owner, TRUE);
    return static_cast<int>(subItems.size() - 1);
}
int CMFCPropertyGridProperty::GetSubItemsCount() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    return state ? static_cast<int>(state->subItems.size()) : 0;
}
CMFCPropertyGridProperty* CMFCPropertyGridProperty::GetSubItem(int nIndex) const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->subItems.size())) return nullptr;
    return state->subItems[static_cast<size_t>(nIndex)];
}
void CMFCPropertyGridProperty::RemoveAllSubItems() {
    auto& state = EnsurePropertyGridPropertyState(this);
    CMFCPropertyGridCtrl* owner = state.owner;
    if (owner) {
        PropertyGridCtrlState* ctrlState = FindMutablePropertyGridCtrlState(owner);
        if (ctrlState) {
            for (CMFCPropertyGridProperty* child : state.subItems) {
                if (ctrlState->current == child || IsPropertyGridAncestorOf(child, ctrlState->current)) {
                    ctrlState->current = nullptr;
                    break;
                }
            }
        }
    }

    std::vector<CMFCPropertyGridProperty*> children = state.subItems;
    state.subItems.clear();
    for (CMFCPropertyGridProperty* child : children) {
        PropertyGridPropertyState* childState = FindMutablePropertyGridPropertyState(child);
        if (childState && childState->parent == this) {
            childState->parent = nullptr;
            SetPropertyGridOwnerRecursive(child, nullptr);
        }
        delete child;
    }
    TouchPropertyGridCtrl(owner, TRUE);
}
void CMFCPropertyGridProperty::Expand(BOOL bExpand) {
    m_bExpanded = bExpand;
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, TRUE);
    }
}
BOOL CMFCPropertyGridProperty::IsExpanded() const { return m_bExpanded; }
void CMFCPropertyGridProperty::SetData(DWORD_PTR dwData) {
    m_dwData = dwData;
}
DWORD_PTR CMFCPropertyGridProperty::GetData() const { return m_dwData; }
BOOL CMFCPropertyGridProperty::AddOption(const wchar_t* lpszOption, BOOL bInsertUnique) {
    if (!lpszOption) return FALSE;
    auto& state = EnsurePropertyGridPropertyState(this);
    std::wstring option(lpszOption);
    if (bInsertUnique) {
        auto it = std::find(state.options.begin(), state.options.end(), option);
        if (it != state.options.end()) {
            return TRUE;
        }
    }
    state.options.push_back(std::move(option));
    if (state.selectedOption < 0) {
        state.selectedOption = 0;
    }
    TouchPropertyGridCtrl(state.owner, FALSE);
    return TRUE;
}
const wchar_t* CMFCPropertyGridProperty::GetOption(int nIndex) const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->options.size())) return nullptr;
    return state->options[static_cast<size_t>(nIndex)].c_str();
}
int CMFCPropertyGridProperty::GetOptionCount() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    return state ? static_cast<int>(state->options.size()) : 0;
}
void CMFCPropertyGridProperty::RemoveAllOptions() {
    auto& state = EnsurePropertyGridPropertyState(this);
    state.options.clear();
    state.selectedOption = -1;
    TouchPropertyGridCtrl(state.owner, FALSE);
}
void CMFCPropertyGridProperty::Enable(BOOL bEnable) {
    SetEnabled(bEnable);
}
int CMFCPropertyGridProperty::GetExpandedSubItems(BOOL bIncludeHidden) const {
    return CountExpandedPropertyGridSubItems(this, bIncludeHidden);
}
int CMFCPropertyGridProperty::GetHierarchyLevel() const {
    int level = 0;
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    while (state && state->parent) {
        ++level;
        state = FindPropertyGridPropertyState(state->parent);
    }
    return level;
}
BOOL CMFCPropertyGridProperty::IsParentExpanded() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    while (state && state->parent) {
        if (!state->parent->IsExpanded()) return FALSE;
        state = FindPropertyGridPropertyState(state->parent);
    }
    return TRUE;
}
BOOL CMFCPropertyGridProperty::IsSubItem(CMFCPropertyGridProperty* pProp) const {
    return IsPropertyGridAncestorOf(this, pProp);
}
BOOL CMFCPropertyGridProperty::IsSubItem(void* pProp) const {
    return IsSubItem(static_cast<CMFCPropertyGridProperty*>(pProp));
}
CMFCPropertyGridProperty* CMFCPropertyGridProperty::FindSubItemByData(DWORD_PTR dwData) const {
    return FindPropertyGridSubItemByData(const_cast<CMFCPropertyGridProperty*>(this), dwData);
}
CString CMFCPropertyGridProperty::FormatProperty() {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    const PropertyGridCtrlState* ctrlState = (state && state->owner) ? FindPropertyGridCtrlState(state->owner) : nullptr;
    return CString(PropertyGridVariantToString(m_varValue, ctrlState).c_str());
}
CString CMFCPropertyGridProperty::GetNameTooltip() {
    return m_strName;
}
CString CMFCPropertyGridProperty::GetValueTooltip() {
    return FormatProperty();
}
BOOL CMFCPropertyGridProperty::IsValueChanged() const {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(this);
    if (!state || !state->hasOriginalValue) return m_bModified;
    return !PropertyGridValuesEqual(m_varValue, state->originalValue.value);
}
void CMFCPropertyGridProperty::ResetOriginalValue() {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->hasOriginalValue) {
        AssignPropertyGridVariant(m_varValue, state->originalValue.value);
        PropertyGridPropertyState* updatedState = FindMutablePropertyGridPropertyState(this);
        if (updatedState && updatedState->owner) {
            TouchPropertyGridCtrl(updatedState->owner, FALSE);
        }
    }
    SetModified(FALSE);
}
void CMFCPropertyGridProperty::SetOriginalValue(const COleVariant& varValue) {
    auto& state = EnsurePropertyGridPropertyState(this);
    state.originalValue.Assign(varValue);
    state.hasOriginalValue = TRUE;
    m_bModified = IsValueChanged();
    TouchPropertyGridCtrl(state.owner, FALSE);
}
void CMFCPropertyGridProperty::SetName(const wchar_t* lpszName, BOOL bRedraw) {
    m_strName = lpszName ? lpszName : L"";
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner && bRedraw) {
        TouchPropertyGridCtrl(state->owner, TRUE);
    }
}
void CMFCPropertyGridProperty::Redraw() {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (state && state->owner) {
        TouchPropertyGridCtrl(state->owner, FALSE);
    }
}
BOOL CMFCPropertyGridProperty::RemoveSubItem(CMFCPropertyGridProperty*& pProp, BOOL bDelete) {
    if (!pProp) return FALSE;
    auto& state = EnsurePropertyGridPropertyState(this);
    auto it = std::find(state.subItems.begin(), state.subItems.end(), pProp);
    if (it == state.subItems.end()) return FALSE;

    CMFCPropertyGridProperty* removed = *it;
    state.subItems.erase(it);
    PropertyGridPropertyState* childState = FindMutablePropertyGridPropertyState(removed);
    if (childState && childState->parent == this) {
        childState->parent = nullptr;
        SetPropertyGridOwnerRecursive(removed, nullptr);
    }
    if (bDelete) {
        delete removed;
    }
    pProp = nullptr;
    TouchPropertyGridCtrl(state.owner, TRUE);
    return TRUE;
}
BOOL CMFCPropertyGridProperty::RemoveSubItem(void*& pProp, BOOL bDelete) {
    CMFCPropertyGridProperty* typed = static_cast<CMFCPropertyGridProperty*>(pProp);
    BOOL result = RemoveSubItem(typed, bDelete);
    pProp = typed;
    return result;
}
void CMFCPropertyGridProperty::ExpandDeep(BOOL bExpand) {
    ExpandPropertyRecursive(this, bExpand);
}
void CMFCPropertyGridProperty::SetOwnerList(CMFCPropertyGridCtrl* pWndList) {
    SetPropertyGridOwnerRecursive(this, pWndList);
}
void CMFCPropertyGridProperty::SetModifiedFlag() {
    SetModified(IsValueChanged());
}
BOOL CMFCPropertyGridProperty::OnUpdateValue() {
    SetModified(TRUE);
    return TRUE;
}
BOOL CMFCPropertyGridProperty::OnEndEdit() {
    SetModified(IsValueChanged());
    return TRUE;
}
void CMFCPropertyGridProperty::OnSelectCombo() {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (!state || state->options.empty()) return;
    if (state->selectedOption < 0 || state->selectedOption >= static_cast<int>(state->options.size())) {
        state->selectedOption = 0;
    }
    COleVariant value(state->options[static_cast<size_t>(state->selectedOption)].c_str());
    SetValue(value);
}
void CMFCPropertyGridProperty::OnCloseCombo() {
    SetModified(IsValueChanged());
}
BOOL CMFCPropertyGridProperty::OnRotateListValue(BOOL bForward) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(this);
    if (!state || state->options.empty()) return FALSE;

    const PropertyGridCtrlState* ctrlState = state->owner ? FindPropertyGridCtrlState(state->owner) : nullptr;
    const std::wstring current = PropertyGridVariantToString(m_varValue, ctrlState);
    auto it = std::find(state->options.begin(), state->options.end(), current);
    int index = it == state->options.end()
        ? (state->selectedOption < 0 ? 0 : state->selectedOption)
        : static_cast<int>(it - state->options.begin());
    if (bForward) {
        index = (index + 1) % static_cast<int>(state->options.size());
    } else {
        index = (index + static_cast<int>(state->options.size()) - 1) % static_cast<int>(state->options.size());
    }
    state->selectedOption = index;
    COleVariant value(state->options[static_cast<size_t>(index)].c_str());
    SetValue(value);
    return TRUE;
}

//=============================================================================
// CMFCPropertyGridCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPropertyGridCtrl, CWnd)

CMFCPropertyGridCtrl::CMFCPropertyGridCtrl() {
    memset(_propgridctrl_padding, 0, sizeof(_propgridctrl_padding));
    EnsurePropertyGridCtrlState(this);
}
CMFCPropertyGridCtrl::~CMFCPropertyGridCtrl() {
    PropertyGridCtrlState* state = FindMutablePropertyGridCtrlState(this);
    if (state) {
        std::vector<CMFCPropertyGridProperty*> properties = state->properties;
        state->properties.clear();
        state->current = nullptr;
        for (CMFCPropertyGridProperty* prop : properties) {
            SetPropertyGridOwnerRecursive(prop, nullptr);
            delete prop;
        }
    }
    g_propertyGridCtrlStates.erase(this);
}

BOOL CMFCPropertyGridCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    EnsurePropertyGridCtrlState(this);
    return CWnd::CreateEx(0, L"SysListView32", L"", dwStyle, rect.left, rect.top,
                          rect.right - rect.left, rect.bottom - rect.top,
                          pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr,
                          reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), this);
}
int CMFCPropertyGridCtrl::AddProperty(CMFCPropertyGridProperty* pProp, int nPos, int bRedraw) {
    if (!pProp) return -1;

    auto& properties = EnsurePropertyGridCtrlState(this).properties;
    properties.erase(std::remove(properties.begin(), properties.end(), pProp), properties.end());
    DetachPropertyFromParent(pProp);
    DetachPropertyFromGrid(pProp);
    PropertyGridPropertyState& propState = EnsurePropertyGridPropertyState(pProp);
    propState.parent = nullptr;
    SetPropertyGridOwnerRecursive(pProp, this);

    if (nPos < 0 || nPos > static_cast<int>(properties.size())) {
        properties.push_back(pProp);
        if (bRedraw) {
            TouchPropertyGridCtrl(this, TRUE);
        }
        return static_cast<int>(properties.size() - 1);
    }

    auto it = properties.begin() + nPos;
    properties.insert(it, pProp);
    if (bRedraw) {
        TouchPropertyGridCtrl(this, TRUE);
    }
    return nPos;
}
int CMFCPropertyGridCtrl::GetPropertyCount() const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    return state ? static_cast<int>(state->properties.size()) : 0;
}
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::GetProperty(int nIndex) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state || nIndex < 0 || nIndex >= static_cast<int>(state->properties.size())) return nullptr;
    return state->properties[static_cast<size_t>(nIndex)];
}
BOOL CMFCPropertyGridCtrl::DeleteProperty(CMFCPropertyGridProperty*& pProp, BOOL bRedraw, BOOL bAdjustLayout) {
    if (!pProp) return FALSE;
    PropertyGridCtrlState& state = EnsurePropertyGridCtrlState(this);
    auto top = std::find(state.properties.begin(), state.properties.end(), pProp);
    if (top != state.properties.end()) {
        CMFCPropertyGridProperty* removed = *top;
        state.properties.erase(top);
        if (state.current == removed || IsPropertyGridAncestorOf(removed, state.current)) {
            state.current = nullptr;
        }
        SetPropertyGridOwnerRecursive(removed, nullptr);
        delete removed;
        pProp = nullptr;
        if (bRedraw) {
            TouchPropertyGridCtrl(this, bAdjustLayout);
        }
        return TRUE;
    }

    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pProp);
    if (propState && propState->parent) {
        if (propState->owner != this) return FALSE;
        CMFCPropertyGridProperty* parent = propState->parent;
        CMFCPropertyGridProperty* removed = pProp;
        if (state.current == removed || IsPropertyGridAncestorOf(removed, state.current)) {
            state.current = nullptr;
        }
        if (parent->RemoveSubItem(pProp, TRUE)) {
            if (bRedraw) {
                TouchPropertyGridCtrl(this, bAdjustLayout);
            }
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CMFCPropertyGridCtrl::DeleteProperty(void*& pProp, BOOL bRedraw, BOOL bAdjustLayout) {
    CMFCPropertyGridProperty* typed = static_cast<CMFCPropertyGridProperty*>(pProp);
    BOOL result = DeleteProperty(typed, bRedraw, bAdjustLayout);
    pProp = typed;
    return result;
}
CMFCPropertyGridProperty* CMFCPropertyGridCtrl::FindItemByData(DWORD_PTR dwData, BOOL bSearchSubItems) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return nullptr;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        if (!prop) continue;
        if (prop->GetData() == dwData) return prop;
        if (bSearchSubItems) {
            if (CMFCPropertyGridProperty* found = prop->FindSubItemByData(dwData)) {
                return found;
            }
        }
    }
    return nullptr;
}
void CMFCPropertyGridCtrl::RemoveAll() {
    auto& state = EnsurePropertyGridCtrlState(this);
    std::vector<CMFCPropertyGridProperty*> properties = state.properties;
    state.properties.clear();
    state.current = nullptr;
    for (CMFCPropertyGridProperty* pProp : properties) {
        SetPropertyGridOwnerRecursive(pProp, nullptr);
        delete pProp;
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::ExpandAll(BOOL bExpand) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* pProp : state->properties) {
        ExpandPropertyRecursive(pProp, bExpand);
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::AdjustLayout() {
    ++EnsurePropertyGridCtrlState(this).layoutRevision;
    Invalidate(FALSE);
}
void CMFCPropertyGridCtrl::SetDescriptionRows(int nRows) {
    EnsurePropertyGridCtrlState(this).descriptionRows = (nRows < 0) ? 0 : nRows;
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::SetCurSel(CMFCPropertyGridProperty* pProp, BOOL bRedraw) {
    auto& state = EnsurePropertyGridCtrlState(this);
    if (pProp) {
        const PropertyGridPropertyState* propState = FindPropertyGridPropertyState(pProp);
        if (!propState || propState->owner != this) return;
    }
    state.current = pProp;
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::ResetOriginalValues(BOOL bRedraw) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        ResetPropertyGridOriginalValueRecursive(prop);
    }
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::MarkModifiedProperties(BOOL bModified, BOOL bRedraw) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        MarkPropertyGridModifiedRecursive(prop, bModified);
    }
    if (bRedraw) {
        TouchPropertyGridCtrl(this, FALSE);
    }
}
void CMFCPropertyGridCtrl::SetBoolLabels(const wchar_t* lpszTrue, const wchar_t* lpszFalse) {
    auto& state = EnsurePropertyGridCtrlState(this);
    state.boolTrue = lpszTrue ? lpszTrue : L"";
    state.boolFalse = lpszFalse ? lpszFalse : L"";
    TouchPropertyGridCtrl(this, FALSE);
}
void CMFCPropertyGridCtrl::SetListDelimiter(wchar_t c) {
    EnsurePropertyGridCtrlState(this).listDelimiter = c;
    TouchPropertyGridCtrl(this, FALSE);
}
void CMFCPropertyGridCtrl::SetAlphabeticMode(BOOL bSet) {
    auto& state = EnsurePropertyGridCtrlState(this);
    state.alphabeticMode = bSet ? TRUE : FALSE;
    if (state.alphabeticMode) {
        auto compareProps = [this](const CMFCPropertyGridProperty* lhs, const CMFCPropertyGridProperty* rhs) {
            return CompareProps(lhs, rhs) < 0;
        };
        std::stable_sort(state.properties.begin(), state.properties.end(),
            compareProps);
        for (CMFCPropertyGridProperty* prop : state.properties) {
            SortPropertyGridSubItemsRecursive(prop, compareProps);
        }
    }
    TouchPropertyGridCtrl(this, TRUE);
}
void CMFCPropertyGridCtrl::SetGroupNameFullWidth(BOOL bSet, BOOL bRedraw) {
    EnsurePropertyGridCtrlState(this).groupNameFullWidth = bSet ? TRUE : FALSE;
    if (bRedraw) {
        TouchPropertyGridCtrl(this, TRUE);
    }
}
int CMFCPropertyGridCtrl::GetTotalItems(BOOL bIncludeHidden) const {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(this);
    if (!state) return 0;
    int count = 0;
    for (CMFCPropertyGridProperty* prop : state->properties) {
        count += CountPropertyGridItems(prop, bIncludeHidden);
    }
    return count;
}
int CMFCPropertyGridCtrl::CompareProps(const CMFCPropertyGridProperty* pProp1, const CMFCPropertyGridProperty* pProp2) const {
    if (pProp1 == pProp2) return 0;
    if (!pProp1) return -1;
    if (!pProp2) return 1;
    return pProp1->GetName().Compare(pProp2->GetName());
}

// Symbol: ?FormatProperty@CMFCPropertyGridProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__FormatProperty_CMFCPropertyGridProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, CMFCPropertyGridProperty* pThis) {
    new(pRet) CString(pThis ? pThis->FormatProperty() : CString());
}

// Symbol: ?GetNameTooltip@CMFCPropertyGridProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetNameTooltip_CMFCPropertyGridProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, CMFCPropertyGridProperty* pThis) {
    new(pRet) CString(pThis ? pThis->GetNameTooltip() : CString());
}

// Symbol: ?GetValueTooltip@CMFCPropertyGridProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetValueTooltip_CMFCPropertyGridProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, CMFCPropertyGridProperty* pThis) {
    new(pRet) CString(pThis ? pThis->GetValueTooltip() : CString());
}

// Symbol: ?HasButton@CMFCPropertyGridProperty@@MEBAHXZ
extern "C" int MS_ABI impl__HasButton_CMFCPropertyGridProperty__MEBAHXZ(CMFCPropertyGridProperty* pThis) {
    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pThis);
    return (state && !state->options.empty()) ? TRUE : FALSE;
}

// Symbol: ?IsSelected@CMFCPropertyGridProperty@@UEBAHXZ
extern "C" int MS_ABI impl__IsSelected_CMFCPropertyGridProperty__UEBAHXZ(CMFCPropertyGridProperty* pThis) {
    const PropertyGridPropertyState* propState = FindPropertyGridPropertyState(pThis);
    const PropertyGridCtrlState* ctrlState = propState && propState->owner ? FindPropertyGridCtrlState(propState->owner) : nullptr;
    return (ctrlState && ctrlState->current == pThis) ? TRUE : FALSE;
}

// Symbol: ?OnActivateByTab@CMFCPropertyGridProperty@@MEAAHXZ
extern "C" int MS_ABI impl__OnActivateByTab_CMFCPropertyGridProperty__MEAAHXZ(CMFCPropertyGridProperty* pThis) {
    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pThis);
    if (!pThis || !propState || !propState->owner || !pThis->IsEnabled() || !pThis->IsVisible()) return FALSE;
    propState->owner->SetCurSel(pThis, TRUE);
    return TRUE;
}

// Symbol: ?OnClickButton@CMFCPropertyGridProperty@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridProperty__UEAAXVCPoint___Z(
    CMFCPropertyGridProperty* pThis, CPoint) {
    if (pThis) impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(pThis, TRUE);
}

// Symbol: ?OnClickValue@CMFCPropertyGridProperty@@UEAAHIVCPoint@@@Z
extern "C" int MS_ABI impl__OnClickValue_CMFCPropertyGridProperty__UEAAHIVCPoint___Z(
    CMFCPropertyGridProperty* pThis, unsigned int, CPoint) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pThis);
    if (propState && propState->owner) propState->owner->SetCurSel(pThis, TRUE);
    return TRUE;
}

// Symbol: ?OnDblClk@CMFCPropertyGridProperty@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__OnDblClk_CMFCPropertyGridProperty__UEAAHVCPoint___Z(
    CMFCPropertyGridProperty* pThis, CPoint) {
    if (!pThis) return FALSE;
    if (pThis->GetSubItemsCount() > 0) {
        pThis->Expand(!pThis->IsExpanded());
        return TRUE;
    }
    return impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(pThis, TRUE);
}

// Symbol: ?OnEdit@CMFCPropertyGridProperty@@UEAAHPEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__OnEdit_CMFCPropertyGridProperty__UEAAHPEAUtagPOINT___Z(
    CMFCPropertyGridProperty* pThis, POINT*) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    PropertyGridPropertyState* propState = FindMutablePropertyGridPropertyState(pThis);
    if (propState && propState->owner) propState->owner->SetCurSel(pThis, TRUE);
    return TRUE;
}

// Symbol: ?PushChar@CMFCPropertyGridProperty@@UEAAHI@Z
extern "C" int MS_ABI impl__PushChar_CMFCPropertyGridProperty__UEAAHI_Z(
    CMFCPropertyGridProperty* pThis, unsigned int nChar) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    if (nChar == VK_SPACE || nChar == VK_RETURN) {
        return impl__OnRotateListValue_CMFCPropertyGridProperty__MEAAHH_Z(pThis, TRUE);
    }
    return FALSE;
}

// Symbol: ?EnableSpinControl@CMFCPropertyGridProperty@@QEAAXHHH@Z
extern "C" void MS_ABI impl__EnableSpinControl_CMFCPropertyGridProperty__QEAAXHHH_Z(
    CMFCPropertyGridProperty* pThis, int bEnable, int nMin, int nMax) {
    if (!pThis) return;
    PropertyGridPropertyState& state = EnsurePropertyGridPropertyState(pThis);
    state.spinEnabled = bEnable ? TRUE : FALSE;
    state.spinMin = nMin;
    state.spinMax = nMax;
    TouchPropertyGridCtrl(state.owner, FALSE);
}

// Symbol: ?AdjustInPlaceEditRect@CMFCPropertyGridProperty@@UEAAXAEAVCRect@@0@Z
extern "C" void MS_ABI impl__AdjustInPlaceEditRect_CMFCPropertyGridProperty__UEAAXAEAVCRect__0_Z(
    CMFCPropertyGridProperty*, CRect* rectEdit, CRect* rectSpin) {
    if (rectEdit) rectEdit->DeflateRect(2, 1);
    if (rectSpin && rectEdit) {
        rectSpin->SetRect(rectEdit->right - 16, rectEdit->top, rectEdit->right, rectEdit->bottom);
        rectEdit->right = rectSpin->left;
    }
}

// Symbol: ?AdjustButtonRect@CMFCPropertyGridProperty@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustButtonRect_CMFCPropertyGridProperty__UEAAXXZ(CMFCPropertyGridProperty* pThis) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pThis);
    TouchPropertyGridCtrl(state ? state->owner : nullptr, FALSE);
}

// Symbol: ?OnDrawName@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::DrawTextW(pDC->GetSafeHdc(), pThis->GetName(), -1, &nativeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
}

// Symbol: ?OnDrawValue@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    CString value = pThis->FormatProperty();
    RECT nativeRect = NativeRect(rect);
    ::DrawTextW(pDC->GetSafeHdc(), value, -1, &nativeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
}

// Symbol: ?OnDrawDescription@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawDescription_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    CString text = pThis->GetNameTooltip();
    RECT nativeRect = NativeRect(rect);
    ::DrawTextW(pDC->GetSafeHdc(), text, -1, &nativeRect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
}

// Symbol: ?OnDrawButton@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawButton_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty*, CDC* pDC, CRect rect) {
    if (!pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::DrawFrameControl(pDC->GetSafeHdc(), &nativeRect, DFC_BUTTON, DFCS_BUTTONPUSH);
}

// Symbol: ?OnDrawExpandBox@CMFCPropertyGridProperty@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawExpandBox_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc() || pThis->GetSubItemsCount() <= 0) return;
    RECT nativeRect = NativeRect(rect);
    ::DrawFrameControl(pDC->GetSafeHdc(), &nativeRect, DFC_BUTTON,
                       pThis->IsExpanded() ? DFCS_BUTTONPUSH : (DFCS_BUTTONPUSH | DFCS_PUSHED));
    const int midX = (rect.left + rect.right) / 2;
    const int midY = (rect.top + rect.bottom) / 2;
    ::MoveToEx(pDC->GetSafeHdc(), rect.left + 3, midY, nullptr);
    ::LineTo(pDC->GetSafeHdc(), rect.right - 3, midY);
    if (!pThis->IsExpanded()) {
        ::MoveToEx(pDC->GetSafeHdc(), midX, rect.top + 3, nullptr);
        ::LineTo(pDC->GetSafeHdc(), midX, rect.bottom - 3);
    }
}

// Symbol: ?OnSetCursor@CMFCPropertyGridProperty@@UEBAHXZ
extern "C" int MS_ABI impl__OnSetCursor_CMFCPropertyGridProperty__UEBAHXZ(CMFCPropertyGridProperty* pThis) {
    if (!pThis || !pThis->IsEnabled()) return FALSE;
    ::SetCursor(::LoadCursorW(nullptr, IDC_ARROW));
    return TRUE;
}

// Symbol: ?HitTest@CMFCPropertyGridProperty@@QEAAPEAV1@VCPoint@@PEAW4ClickArea@1@@Z
extern "C" CMFCPropertyGridProperty* MS_ABI impl__HitTest_CMFCPropertyGridProperty__QEAAPEAV1_VCPoint__PEAW4ClickArea_1__Z(
    CMFCPropertyGridProperty* pThis, CPoint, int* pClickArea) {
    if (pClickArea) *pClickArea = 0;
    return (pThis && pThis->IsVisible()) ? pThis : nullptr;
}

// Symbol: ?Init@CMFCPropertyGridProperty@@IEAAXXZ
extern "C" void MS_ABI impl__Init_CMFCPropertyGridProperty__IEAAXXZ(CMFCPropertyGridProperty* pThis) {
    if (!pThis) return;
    EnsurePropertyGridPropertyState(pThis);
    pThis->SetModified(FALSE);
}

// Symbol: ?AddTerminalProp@CMFCPropertyGridProperty@@IEAAXAEAV?$CList@PEAVCMFCPropertyGridProperty@@PEAV1@@@@Z
extern "C" void MS_ABI impl__AddTerminalProp_CMFCPropertyGridProperty__IEAAXAEAV__CList_PEAVCMFCPropertyGridProperty__PEAV1____Z(
    CMFCPropertyGridProperty* pThis, CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* pList) {
    if (!pThis || !pList) return;

    const PropertyGridPropertyState* state = FindPropertyGridPropertyState(pThis);
    if (!state || state->subItems.empty()) {
        pList->AddTail(pThis);
        return;
    }

    for (CMFCPropertyGridProperty* child : state->subItems) {
        impl__AddTerminalProp_CMFCPropertyGridProperty__IEAAXAEAV__CList_PEAVCMFCPropertyGridProperty__PEAV1____Z(child, pList);
    }
}

// Symbol: ?get_accChildCount@CMFCPropertyGridCtrl@@UEAAJPEAJ@Z
extern "C" HRESULT MS_ABI impl__get_accChildCount_CMFCPropertyGridCtrl__UEAAJPEAJ_Z(
    CMFCPropertyGridCtrl* pThis, long* pCountChildren) {
    if (!pCountChildren) return E_POINTER;
    *pCountChildren = pThis ? static_cast<long>(VisiblePropertyGridItems(pThis).size()) : 0;
    return S_OK;
}

// Symbol: ?get_accChild@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" HRESULT MS_ABI impl__get_accChild_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCPropertyGridCtrl*, VARIANT varChild, IDispatch** ppdispChild) {
    if (!ppdispChild) return E_POINTER;
    *ppdispChild = nullptr;
    return PropertyGridChildIDFromVariant(varChild) >= CHILDID_SELF ? S_FALSE : E_INVALIDARG;
}

// Symbol: ?get_accName@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accName_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszName) {
    if (!pszName) return E_POINTER;
    *pszName = nullptr;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID == CHILDID_SELF) return AllocPropertyGridString(L"Property Grid", pszName);
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    return AllocPropertyGridString(static_cast<const wchar_t*>(prop->GetName()), pszName);
}

// Symbol: ?get_accValue@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accValue_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszValue) {
    if (!pszValue) return E_POINTER;
    *pszValue = nullptr;
    int childID = PropertyGridChildIDFromVariant(varChild);
    CMFCPropertyGridProperty* prop = childID == CHILDID_SELF ? (FindPropertyGridCtrlState(pThis) ? FindPropertyGridCtrlState(pThis)->current : nullptr)
                                                             : PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return S_FALSE;
    CString value = prop->FormatProperty();
    return AllocPropertyGridString(static_cast<const wchar_t*>(value), pszValue);
}

// Symbol: ?get_accDescription@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDescription_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszDescription) {
    return impl__get_accName_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(pThis, varChild, pszDescription);
}

// Symbol: ?get_accDefaultAction@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accDefaultAction_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, BSTR* pszDefaultAction) {
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID == CHILDID_SELF) return S_FALSE;
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    return AllocPropertyGridString(prop->GetSubItemsCount() > 0 ? L"Expand or collapse" : L"Edit", pszDefaultAction);
}

// Symbol: ?get_accRole@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accRole_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, VARIANT* pvarRole) {
    (void)pThis;
    if (!pvarRole) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID < CHILDID_SELF) return E_INVALIDARG;
    VariantInit(pvarRole);
    pvarRole->vt = VT_I4;
    pvarRole->lVal = childID == CHILDID_SELF ? ROLE_SYSTEM_OUTLINE : ROLE_SYSTEM_OUTLINEITEM;
    return S_OK;
}

// Symbol: ?get_accState@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" HRESULT MS_ABI impl__get_accState_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPropertyGridCtrl* pThis, VARIANT varChild, VARIANT* pvarState) {
    if (!pvarState) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    if (childID < CHILDID_SELF) return E_INVALIDARG;
    VariantInit(pvarState);
    pvarState->vt = VT_I4;
    pvarState->lVal = 0;
    if (childID == CHILDID_SELF) return S_OK;

    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    const PropertyGridCtrlState* ctrlState = FindPropertyGridCtrlState(pThis);
    if (!prop->IsEnabled()) pvarState->lVal |= STATE_SYSTEM_UNAVAILABLE;
    if (!prop->IsVisible()) pvarState->lVal |= STATE_SYSTEM_INVISIBLE;
    if (ctrlState && ctrlState->current == prop) pvarState->lVal |= STATE_SYSTEM_SELECTED | STATE_SYSTEM_FOCUSED;
    if (prop->GetSubItemsCount() > 0) pvarState->lVal |= prop->IsExpanded() ? STATE_SYSTEM_EXPANDED : STATE_SYSTEM_COLLAPSED;
    return S_OK;
}

// Symbol: ?get_accFocus@CMFCPropertyGridCtrl@@UEAAJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__get_accFocus_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, VARIANT* pvarChild) {
    if (!pvarChild) return E_POINTER;
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    SetPropertyGridVariantChild(pvarChild, state ? PropertyGridChildIDFromProperty(pThis, state->current) : CHILDID_SELF);
    return S_OK;
}

// Symbol: ?get_accSelection@CMFCPropertyGridCtrl@@UEAAJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__get_accSelection_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, VARIANT* pvarChildren) {
    return impl__get_accFocus_CMFCPropertyGridCtrl__UEAAJPEAUtagVARIANT___Z(pThis, pvarChildren);
}

// Symbol: ?accSelect@CMFCPropertyGridCtrl@@UEAAJJUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accSelect_CMFCPropertyGridCtrl__UEAAJJUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long, VARIANT varChild) {
    int childID = PropertyGridChildIDFromVariant(varChild);
    CMFCPropertyGridProperty* prop = PropertyGridPropertyFromChildID(pThis, childID);
    if (!prop) return E_INVALIDARG;
    pThis->SetCurSel(prop, TRUE);
    return S_OK;
}

// Symbol: ?accHitTest@CMFCPropertyGridCtrl@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accHitTest_CMFCPropertyGridCtrl__UEAAJJJPEAUtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long xLeft, long yTop, VARIANT* pvarChild) {
    if (!pvarChild) return E_POINTER;
    POINT pt{xLeft, yTop};
    if (pThis && pThis->GetSafeHwnd()) ::ScreenToClient(pThis->GetSafeHwnd(), &pt);
    CMFCPropertyGridProperty* prop = PropertyGridHitTestByPoint(pThis, CPoint(pt.x, pt.y));
    SetPropertyGridVariantChild(pvarChild, PropertyGridChildIDFromProperty(pThis, prop));
    return S_OK;
}

// Symbol: ?accLocation@CMFCPropertyGridCtrl@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__accLocation_CMFCPropertyGridCtrl__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCPropertyGridCtrl* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) {
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_POINTER;
    int childID = PropertyGridChildIDFromVariant(varChild);
    CRect itemRect = PropertyGridItemRect(pThis, childID);
    POINT pt{itemRect.left, itemRect.top};
    if (pThis && pThis->GetSafeHwnd()) ::ClientToScreen(pThis->GetSafeHwnd(), &pt);
    *pxLeft = pt.x;
    *pyTop = pt.y;
    *pcxWidth = itemRect.Width();
    *pcyHeight = itemRect.Height();
    return S_OK;
}

// Symbol: ?get_accHelp@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accHelp_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl*, VARIANT, BSTR* pszHelp) {
    if (!pszHelp) return E_POINTER;
    *pszHelp = nullptr;
    return S_FALSE;
}

// Symbol: ?get_accHelpTopic@CMFCPropertyGridCtrl@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" HRESULT MS_ABI impl__get_accHelpTopic_CMFCPropertyGridCtrl__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    CMFCPropertyGridCtrl*, BSTR* pszHelpFile, VARIANT, long* pidTopic) {
    if (pszHelpFile) *pszHelpFile = nullptr;
    if (pidTopic) *pidTopic = 0;
    return S_FALSE;
}

// Symbol: ?get_accKeyboardShortcut@CMFCPropertyGridCtrl@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" HRESULT MS_ABI impl__get_accKeyboardShortcut_CMFCPropertyGridCtrl__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CMFCPropertyGridCtrl*, VARIANT, BSTR* pszKeyboardShortcut) {
    if (!pszKeyboardShortcut) return E_POINTER;
    *pszKeyboardShortcut = nullptr;
    return S_FALSE;
}

// Symbol: ?EditItem@CMFCPropertyGridCtrl@@UEAAHPEAVCMFCPropertyGridProperty@@PEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__EditItem_CMFCPropertyGridCtrl__UEAAHPEAVCMFCPropertyGridProperty__PEAUtagPOINT___Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, POINT* point) {
    (void)point;
    if (!pThis || !pProp || !pProp->IsEnabled()) return FALSE;
    pThis->SetCurSel(pProp, TRUE);
    return TRUE;
}

// Symbol: ?EndEditItem@CMFCPropertyGridCtrl@@UEAAHH@Z
extern "C" int MS_ABI impl__EndEditItem_CMFCPropertyGridCtrl__UEAAHH_Z(CMFCPropertyGridCtrl* pThis, int bUpdateData) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    if (!state || !state->current) return FALSE;
    return bUpdateData ? state->current->OnUpdateValue() : state->current->OnEndEdit();
}

// Symbol: ?EnsureVisible@CMFCPropertyGridCtrl@@QEAAXPEAVCMFCPropertyGridProperty@@H@Z
extern "C" void MS_ABI impl__EnsureVisible_CMFCPropertyGridCtrl__QEAAXPEAVCMFCPropertyGridProperty__H_Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp, int bExpandParents) {
    if (!pThis || !pProp) return;
    if (bExpandParents) {
        PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
        while (state && state->parent) {
            state->parent->Expand(TRUE);
            state = FindMutablePropertyGridPropertyState(state->parent);
        }
    }
    pThis->SetCurSel(pProp, TRUE);
}

// Symbol: ?HitTest@CMFCPropertyGridCtrl@@QEBAPEAVCMFCPropertyGridProperty@@VCPoint@@PEAW4ClickArea@2@H@Z
extern "C" CMFCPropertyGridProperty* MS_ABI impl__HitTest_CMFCPropertyGridCtrl__QEBAPEAVCMFCPropertyGridProperty__VCPoint__PEAW4ClickArea_2_H_Z(
    CMFCPropertyGridCtrl* pThis, CPoint point, int* pClickArea, int) {
    if (pClickArea) *pClickArea = point.x < 120 ? 1 : 2;
    return PropertyGridHitTestByPoint(pThis, point);
}

// Symbol: ?OnDrawProperty@CMFCPropertyGridCtrl@@UEBAHPEAVCDC@@PEAVCMFCPropertyGridProperty@@@Z
extern "C" int MS_ABI impl__OnDrawProperty_CMFCPropertyGridCtrl__UEBAHPEAVCDC__PEAVCMFCPropertyGridProperty___Z(
    CMFCPropertyGridCtrl* pThis, CDC* pDC, CMFCPropertyGridProperty* pProp) {
    if (!pDC || !pDC->GetSafeHdc() || !pProp) return FALSE;
    int childID = PropertyGridChildIDFromProperty(pThis, pProp);
    CRect rect = PropertyGridItemRect(pThis, childID);
    COLORREF back = pProp->IsEnabled() ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE);
    ::SetBkColor(pDC->GetSafeHdc(), back);
    RECT nativeRect = NativeRect(rect);
    ::ExtTextOutW(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, &nativeRect, nullptr, 0, nullptr);
    CRect nameRect(rect.left + 4 + pProp->GetHierarchyLevel() * 12, rect.top, rect.left + rect.Width() / 2, rect.bottom);
    CRect valueRect(rect.left + rect.Width() / 2 + 4, rect.top, rect.right - 2, rect.bottom);
    impl__OnDrawName_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pProp, pDC, nameRect);
    impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(pProp, pDC, valueRect);
    return TRUE;
}

// Symbol: ?OnDraw@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    if (!pThis || !pDC || !pDC->GetSafeHdc()) return;
    for (CMFCPropertyGridProperty* prop : VisiblePropertyGridItems(pThis)) {
        impl__OnDrawProperty_CMFCPropertyGridCtrl__UEBAHPEAVCDC__PEAVCMFCPropertyGridProperty___Z(pThis, pDC, prop);
    }
}

// Symbol: ?OnFillBackground@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCPropertyGridCtrl__MEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridCtrl*, CDC* pDC, CRect rect) {
    if (!pDC || !pDC->GetSafeHdc()) return;
    RECT nativeRect = NativeRect(rect);
    ::ExtTextOutW(pDC->GetSafeHdc(), 0, 0, ETO_OPAQUE, &nativeRect, nullptr, 0, nullptr);
}

// Symbol: ?OnDrawList@CMFCPropertyGridCtrl@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawList_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(CMFCPropertyGridCtrl* pThis, CDC* pDC) {
    impl__OnDraw_CMFCPropertyGridCtrl__MEAAXPEAVCDC___Z(pThis, pDC);
}

// Symbol: ?SetCustomColors@CMFCPropertyGridCtrl@@QEAAXKKKKKKK@Z
extern "C" void MS_ABI impl__SetCustomColors_CMFCPropertyGridCtrl__QEAAXKKKKKKK_Z(
    CMFCPropertyGridCtrl* pThis, COLORREF c1, COLORREF c2, COLORREF c3, COLORREF c4, COLORREF c5, COLORREF c6, COLORREF c7) {
    if (!pThis) return;
    PropertyGridCtrlState& state = EnsurePropertyGridCtrlState(pThis);
    COLORREF values[7] = {c1, c2, c3, c4, c5, c6, c7};
    std::copy(std::begin(values), std::end(values), std::begin(state.customColors));
}

// Symbol: ?GetCustomColors@CMFCPropertyGridCtrl@@QEAAXAEAK000000@Z
extern "C" void MS_ABI impl__GetCustomColors_CMFCPropertyGridCtrl__QEAAXAEAK000000_Z(
    CMFCPropertyGridCtrl* pThis, COLORREF* c1, COLORREF* c2, COLORREF* c3, COLORREF* c4, COLORREF* c5, COLORREF* c6, COLORREF* c7) {
    const PropertyGridCtrlState* state = FindPropertyGridCtrlState(pThis);
    const COLORREF defaults[7] = {RGB(0, 0, 0), RGB(128, 128, 128), RGB(128, 0, 0), RGB(0, 128, 0), RGB(0, 0, 128), RGB(255, 255, 255), RGB(255, 255, 0)};
    const COLORREF* colors = state ? state->customColors : defaults;
    if (c1) *c1 = colors[0];
    if (c2) *c2 = colors[1];
    if (c3) *c3 = colors[2];
    if (c4) *c4 = colors[3];
    if (c5) *c5 = colors[4];
    if (c6) *c6 = colors[5];
    if (c7) *c7 = colors[6];
}

// Symbol: ?EnableDescriptionArea@CMFCPropertyGridCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableDescriptionArea_CMFCPropertyGridCtrl__QEAAXH_Z(CMFCPropertyGridCtrl* pThis, int bEnable) {
    if (pThis) pThis->SetDescriptionRows(bEnable ? 2 : 0);
}

// Symbol: ?Init@CMFCPropertyGridCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__Init_CMFCPropertyGridCtrl__MEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    if (pThis) EnsurePropertyGridCtrlState(pThis);
}

// Symbol: ?NotifyAccessibility@CMFCPropertyGridCtrl@@MEAAXPEAVCMFCPropertyGridProperty@@@Z
extern "C" void MS_ABI impl__NotifyAccessibility_CMFCPropertyGridCtrl__MEAAXPEAVCMFCPropertyGridProperty___Z(
    CMFCPropertyGridCtrl* pThis, CMFCPropertyGridProperty* pProp) {
    (void)pProp;
    TouchPropertyGridCtrl(pThis, FALSE);
}

// Symbol: ?CalcEditMargin@CMFCPropertyGridCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__CalcEditMargin_CMFCPropertyGridCtrl__IEAAXXZ(CMFCPropertyGridCtrl* pThis) {
    TouchPropertyGridCtrl(pThis, FALSE);
}

//=============================================================================
// CMFCTasksPaneTask
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCTasksPaneTask, CObject)

CMFCTasksPaneTask::CMFCTasksPaneTask(UINT nID, const wchar_t* lpszLabel, int nIcon, BOOL bIsSeparator) {
    memset(_taskspanetask_padding, 0, sizeof(_taskspanetask_padding));
    auto& state = EnsureTasksPaneTaskState(this);
    state.id = nID;
    state.label = lpszLabel ? lpszLabel : L"";
    state.icon = nIcon;
    state.isSeparator = bIsSeparator ? TRUE : FALSE;
}
CMFCTasksPaneTask::~CMFCTasksPaneTask() { RemoveTaskFromAllGroups(this); }

//=============================================================================
// CMFCTasksPane
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCTasksPane, CBasePane)

CMFCTasksPane::CMFCTasksPane() {
    memset(_taskspane_padding, 0, sizeof(_taskspane_padding));
    EnsureTasksPaneState(this);
}
CMFCTasksPane::~CMFCTasksPane() { g_tasksPaneStates.erase(this); }

BOOL CMFCTasksPane::Create(DWORD, const RECT&, CWnd*, UINT) {
    EnsureTasksPaneState(this);
    return TRUE;
}
int CMFCTasksPane::AddTask(int nGroup, const wchar_t* lpszName, int nIcon, unsigned int uiCmdID, unsigned __int64 dwUserData) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState& group = EnsureTasksPaneGroup(state, nGroup);

    TasksPaneGroupState::TaskEntry task;
    task.label = lpszName ? lpszName : L"";
    task.icon = nIcon;
    task.commandID = uiCmdID;
    task.userData = dwUserData;
    group.tasks.push_back(std::move(task));
    return static_cast<int>(group.tasks.size() - 1);
}
// Old convenience overloads (non-MSDN API)
void CMFCTasksPane::AddTask(int nGroup, CMFCTasksPaneTask* pTask) {
    if (!pTask) return;
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState& group = EnsureTasksPaneGroup(state, nGroup);
    auto it = std::find_if(group.tasks.begin(), group.tasks.end(),
        [pTask](const TasksPaneGroupState::TaskEntry& task) { return task.task == pTask; });
    if (it == group.tasks.end()) {
        TasksPaneGroupState::TaskEntry task;
        task.task = pTask;
        group.tasks.push_back(std::move(task));
    }
}
void CMFCTasksPane::RemoveAllTasks() {
    TasksPaneState& state = EnsureTasksPaneState(this);
    for (auto& group : state.groups) {
        group.tasks.clear();
    }
}
void CMFCTasksPane::RemoveAllTasks(int nGroup) {
    if (nGroup < 0) {
        RemoveAllTasks();
        return;
    }

    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup >= static_cast<int>(state.groups.size())) return;
    state.groups[static_cast<size_t>(nGroup)].tasks.clear();
}
void CMFCTasksPane::SetCaption(int nGroup, const wchar_t* lpszCaption) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup < 0) {
        state.caption = lpszCaption ? lpszCaption : L"";
        return;
    }
    if (nGroup >= static_cast<int>(state.groups.size())) return;
    state.groups[static_cast<size_t>(nGroup)].name = lpszCaption ? lpszCaption : L"";
}
void CMFCTasksPane::SetCaption(const wchar_t* lpszCaption) { SetCaption(-1, lpszCaption); }
BOOL CMFCTasksPane::SetGroupName(int nGroup, const wchar_t* lpszName) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup < 0 || nGroup >= static_cast<int>(state.groups.size())) return FALSE;
    state.groups[static_cast<size_t>(nGroup)].name = lpszName ? lpszName : L"";
    return TRUE;
}
int CMFCTasksPane::AddGroup(const wchar_t* lpszName, BOOL bBottomHasGripper, BOOL bSpecial, int nIcon) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState group;
    group.name = lpszName ? lpszName : L"";
    group.hasGripper = bBottomHasGripper ? TRUE : FALSE;
    group.isSpecial = bSpecial ? TRUE : FALSE;
    group.icon = nIcon;
    state.groups.push_back(std::move(group));
    return static_cast<int>(state.groups.size() - 1);
}
int CMFCTasksPane::AddGroup(int nGroup, const wchar_t* lpszName, BOOL bBottomHasGripper, BOOL bSpecial, HICON hIcon) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState group;
    group.name = lpszName ? lpszName : L"";
    group.hasGripper = bBottomHasGripper ? TRUE : FALSE;
    group.isSpecial = bSpecial ? TRUE : FALSE;
    group.hIcon = hIcon;

    if (nGroup >= 0 && nGroup <= static_cast<int>(state.groups.size())) {
        state.groups.insert(state.groups.begin() + nGroup, std::move(group));
        return nGroup;
    }

    state.groups.push_back(std::move(group));
    return static_cast<int>(state.groups.size() - 1);
}

//=============================================================================
// CPaneFrameWnd
//=============================================================================
IMPLEMENT_DYNAMIC(CPaneFrameWnd, CMiniFrameWnd)

CPaneFrameWnd::CPaneFrameWnd() {
    memset(_paneframewnd_padding, 0, sizeof(_paneframewnd_padding));
}
CPaneFrameWnd::~CPaneFrameWnd() {}

BOOL CPaneFrameWnd::Create(CWnd* pParentWnd, DWORD dwStyle, UINT) {
    CRect rect(0, 0, 320, 220);
    if (pParentWnd && pParentWnd->GetSafeHwnd()) {
        RECT parentRect{};
        pParentWnd->GetClientRect(&parentRect);
        rect = CRect(parentRect);
        if (rect.Width() <= 0 || rect.Height() <= 0) {
            rect = CRect(0, 0, 320, 220);
        }
    }
    RECT createRect{rect.left, rect.top, rect.right, rect.bottom};
    return Create(L"STATIC", dwStyle, createRect, pParentWnd, nullptr);
}
extern "C" void MS_ABI impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane);
extern "C" void MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int, int);
void CPaneFrameWnd::AddPane(CBasePane* pBar) {
    impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(this, pBar);
}
void CPaneFrameWnd::RemovePane(CBasePane* pBar, BOOL bDestroy) {
    impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(this, pBar, bDestroy, FALSE);
}

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
CDockingManager::~CDockingManager() {
    RemoveDockingState(this);
}

void CDockingManager::DockPane(CBasePane* pBar, UINT, LPCRECT) {
    AddDockingPane(this, pBar);
    DockingManagerState& state = EnsureDockingState(this);
    state.hiddenPanes.erase(pBar);
    state.floatingPanes.erase(pBar);
}

BOOL CDockingManager::DockPaneLeftOf(CBasePane* pBarToDock, CBasePane* pBar) {
    if (!pBarToDock) return FALSE;

    DockingManagerState& state = EnsureDockingState(this);
    state.panes.erase(std::remove(state.panes.begin(), state.panes.end(), pBarToDock), state.panes.end());

    auto it = std::find(state.panes.begin(), state.panes.end(), pBar);
    if (it != state.panes.end()) {
        state.panes.insert(it, pBarToDock);
    } else {
        state.panes.push_back(pBarToDock);
    }

    state.hiddenPanes.erase(pBarToDock);
    state.floatingPanes.erase(pBarToDock);
    return TRUE;
}

BOOL CDockingManager::EnableDocking(DWORD dwDockStyle) {
    EnsureDockingState(this).enabledAlignment = dwDockStyle;
    return TRUE;
}

void CDockingManager::FloatPane(CBasePane* pBar, CPoint, DWORD) {
    AddDockingPane(this, pBar);
    DockingManagerState& state = EnsureDockingState(this);
    state.hiddenPanes.erase(pBar);
    state.floatingPanes.insert(pBar);
    if (pBar) {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        PaneCoreState& paneState = g_paneCoreState[pBar];
        paneState.canFloat = TRUE;
        paneState.visible = TRUE;
    }
}

void CDockingManager::HidePane(CBasePane* pBar) {
    if (!pBar) return;
    AddDockingPane(this, pBar);
    EnsureDockingState(this).hiddenPanes.insert(pBar);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[pBar].visible = FALSE;
}

void CDockingManager::ShowPane(CBasePane* pBar, BOOL) {
    if (!pBar) return;
    AddDockingPane(this, pBar);
    EnsureDockingState(this).hiddenPanes.erase(pBar);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[pBar].visible = TRUE;
}
void CDockingManager::RecalcLayout() {
    DockingManagerState& state = EnsureDockingState(this);
    if (state.lockUpdate) return;

    RECT client{0, 0, 0, 0};
    HWND parent = m_pParentWnd ? m_pParentWnd->GetSafeHwnd() : nullptr;
    if (parent) {
        ::GetClientRect(parent, &client);
    }
    int visibleCount = 0;
    for (CBasePane* pane : state.panes) {
        if (IsPaneVisibleForDocking(state, pane)) ++visibleCount;
    }
    if (visibleCount <= 0) return;

    int index = 0;
    int width = (client.right > client.left) ? (client.right - client.left) / visibleCount : 0;
    for (CBasePane* pane : state.panes) {
        if (!IsPaneVisibleForDocking(state, pane)) continue;
        CRect rect(client.left + index * width, client.top,
                   index == visibleCount - 1 ? client.right : client.left + (index + 1) * width,
                   client.bottom);
        {
            std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
            g_paneCoreState[pane].recentRect = rect;
            g_paneCoreState[pane].visible = TRUE;
        }
        if (pane->GetSafeHwnd() && rect.Width() > 0 && rect.Height() > 0) {
            pane->SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        } else {
            pane->RecalcLayout();
        }
        ++index;
    }
}
void CDockingManager::SetDockState() {
    DockingManagerState& state = EnsureDockingState(this);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    for (CBasePane* pane : state.panes) {
        if (!pane) continue;
        PaneCoreState& paneState = g_paneCoreState[pane];
        paneState.visible = state.hiddenPanes.find(pane) == state.hiddenPanes.end();
        paneState.canFloat = paneState.canFloat || state.floatingPanes.find(pane) != state.floatingPanes.end();
        if (state.enabledAlignment != 0 && paneState.alignment == 0) {
            paneState.alignment = state.enabledAlignment;
        }
    }
}

// Symbol: ?AddMiniFrame@CDockingManager@@UEAAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__AddMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(CDockingManager* pThis, void* pFrame) {
    if (!pThis || !pFrame) return FALSE;
    auto& frames = EnsureDockingState(pThis).miniFrames;
    if (std::find(frames.begin(), frames.end(), pFrame) == frames.end()) {
        frames.push_back(pFrame);
    }
    return TRUE;
}

// Symbol: ?AddPane@CDockingManager@@QEAAHPEAVCBasePane@@HHH@Z
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int, int, int) {
    if (!pThis || !pPane) return FALSE;
    AddDockingPane(pThis, pPane);
    return TRUE;
}

// Symbol: ?AdjustDockingLayout@CDockingManager@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(CDockingManager* pThis, void*) {
    if (pThis) pThis->RecalcLayout();
}

// Symbol: ?AdjustPaneFrames@CDockingManager@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustPaneFrames_CDockingManager__UEAAXXZ(CDockingManager*) {}

// Symbol: ?AdjustRectToClientArea@CDockingManager@@UEAAHAEAVCRect@@K@Z
extern "C" int MS_ABI impl__AdjustRectToClientArea_CDockingManager__UEAAHAEAVCRect__K_Z(CDockingManager*, CRect*, unsigned long) {
    return TRUE;
}

// Symbol: ?AlignAutoHidePane@CDockingManager@@QEAAXPEAVCPaneDivider@@H@Z
extern "C" void MS_ABI impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(CDockingManager*, void*, int) {}

// Symbol: ?AutoHidePane@CDockingManager@@QEAAPEAVCMFCAutoHideBar@@PEAVCDockablePane@@PEAV2@@Z
extern "C" CMFCAutoHideBar* MS_ABI impl__AutoHidePane_CDockingManager__QEAAPEAVCMFCAutoHideBar__PEAVCDockablePane__PEAV2__Z(
    CDockingManager* pThis, CDockablePane* pPane, CDockablePane*) {
    AddDockingPane(pThis, pPane);
    return nullptr;
}

// Symbol: ?BringBarsToTop@CDockingManager@@QEAAXKH@Z
extern "C" void MS_ABI impl__BringBarsToTop_CDockingManager__QEAAXKH_Z(CDockingManager*, unsigned long, int) {}

// Symbol: ?BuildPanesMenu@CDockingManager@@QEAAXAEAVCMenu@@H@Z
extern "C" void MS_ABI impl__BuildPanesMenu_CDockingManager__QEAAXAEAVCMenu__H_Z(CDockingManager*, CMenu*, int) {}

// Symbol: ?CalcExpectedDockedRect@CDockingManager@@QEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    CDockingManager*, CWnd*, CPoint ptMouse, CRect* rectResult, int* pnAlignment, CDockablePane** ppTargetBar) {
    if (rectResult) *rectResult = CRect(ptMouse.x, ptMouse.y, ptMouse.x, ptMouse.y);
    if (pnAlignment) *pnAlignment = 0;
    if (ppTargetBar) *ppTargetBar = nullptr;
}

// Symbol: ?DeterminePaneAndStatus@CDockingManager@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@HKPEAPEAVCBasePane@@PEBV4@2@Z
extern "C" int MS_ABI impl__DeterminePaneAndStatus_CDockingManager__UEAA_AW4AFX_CS_STATUS__VCPoint__HKPEAPEAVCBasePane__PEBV4_2_Z(
    CDockingManager* pThis, CPoint, int, unsigned long, CBasePane** ppTargetBar, const CDockingManager*, const CDockingManager*) {
    if (ppTargetBar) *ppTargetBar = FirstDockingPane(pThis);
    return 0;
}

// Symbol: ?EnableAutoHidePanes@CDockingManager@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(CDockingManager* pThis, unsigned long dwStyle) {
    if (pThis) EnsureDockingState(pThis).enabledAlignment |= dwStyle;
    return TRUE;
}

// Symbol: ?EnablePaneContextMenu@CDockingManager@@QEAAXHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__EnablePaneContextMenu_CDockingManager__QEAAXHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CDockingManager* pThis, int bEnable, unsigned int uiCustomizeCmd, const CString* strCustomizeText, int) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    state.paneContextMenu = bEnable != FALSE;
    state.paneContextMenuID = uiCustomizeCmd;
    state.paneContextMenuName = strCustomizeText ? *strCustomizeText : CString();
}

// Symbol: ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    for (CBasePane* pane : state->panes) {
        if (PaneCommandID(pane) == nID) return pane;
    }

    (void)bSearchMiniFrames;
    return nullptr;
}

// Symbol: ?FixupVirtualRects@CDockingManager@@UEAAXXZ
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingManager__UEAAXXZ(CDockingManager*) {}

// Symbol: ?FloatPane@CDockingManager@@QEAAXPEAVCBasePane@@VCPoint@@K@Z
extern "C" void MS_ABI impl__FloatPane_CDockingManager__QEAAXPEAVCBasePane__VCPoint__K_Z(
    CDockingManager* pThis, CBasePane* pPane, CPoint ptOffset, unsigned long dwAlignment) {
    if (pThis) pThis->FloatPane(pPane, ptOffset, dwAlignment);
}

// Symbol: ?FrameFromPoint@CDockingManager@@UEBAPEAVCPaneFrameWnd@@VCPoint@@PEAV2@H@Z
extern "C" void* MS_ABI impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(
    const CDockingManager* pThis, CPoint, void*, int) {
    const DockingManagerState* state = FindDockingState(pThis);
    return (state && !state->miniFrames.empty()) ? state->miniFrames.front() : nullptr;
}

// Symbol: ?GetPaneList@CDockingManager@@QEAAXAEAVCObList@@HPEAUCRuntimeClass@@H@Z
extern "C" void MS_ABI impl__GetPaneList_CDockingManager__QEAAXAEAVCObList__HPEAUCRuntimeClass__H_Z(
    CDockingManager* pThis, CObList* pList, int bIncludeHidden, CRuntimeClass* pRTCFilter, int) {
    if (!pList) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        if (!bIncludeHidden && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) continue;
        if (pRTCFilter && !pane->IsKindOf(pRTCFilter)) continue;
        pList->AddTail(static_cast<CObject*>(pane));
    }
}

// Symbol: ?HideAutoHidePanes@CDockingManager@@QEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__HideAutoHidePanes_CDockingManager__QEAAXPEAVCDockablePane__H_Z(
    CDockingManager* pThis, CDockablePane* pPane, int) {
    if (pThis && pPane) pThis->HidePane(pPane);
}

// Symbol: ?InsertPane@CDockingManager@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pPane, CBasePane* pTarget, int) {
    if (!pThis || !pPane) return FALSE;
    AddDockingPane(pThis, pPane, pTarget);
    return TRUE;
}

// Symbol: ?LoadState@CDockingManager@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t*, unsigned int) {
    if (pThis) EnsureDockingState(pThis);
    return TRUE;
}

// Symbol: ?LockUpdate@CDockingManager@@QEAAXH@Z
extern "C" void MS_ABI impl__LockUpdate_CDockingManager__QEAAXH_Z(CDockingManager* pThis, int bLock) {
    if (pThis) EnsureDockingState(pThis).lockUpdate = bLock != FALSE;
}

// Symbol: ?OnActivateFrame@CDockingManager@@UEAAXH@Z
extern "C" void MS_ABI impl__OnActivateFrame_CDockingManager__UEAAXH_Z(CDockingManager*, int) {}

// Symbol: ?OnClosePopupMenu@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__OnClosePopupMenu_CDockingManager__QEAAXXZ(CDockingManager*) {}

// Symbol: ?OnMoveMiniFrame@CDockingManager@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager*, CWnd*) {
    return TRUE;
}

// Symbol: ?OnPaneContextMenu@CDockingManager@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CDockingManager__QEAAXVCPoint___Z(CDockingManager*, CPoint) {}

// Symbol: ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@HPEBV2@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
    const CDockingManager* pThis, CPoint, int bIncludeHidden, int, CRuntimeClass* pRTCFilter, int, const CDockingManager*) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        if (!bIncludeHidden && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) continue;
        if (pRTCFilter && !pane->IsKindOf(pRTCFilter)) continue;
        return pane;
    }

    return nullptr;
}

// Symbol: ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@PEBV2@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* pThis, CPoint point, int bIncludeHidden, unsigned long* dwAlignment, CRuntimeClass* pRTCFilter, const CDockingManager* pDockManager) {
    if (dwAlignment) *dwAlignment = 0;
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        pThis, point, bIncludeHidden, FALSE, pRTCFilter, FALSE, pDockManager);
}

// Symbol: ?ProcessPaneContextMenuCommand@CDockingManager@@QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__ProcessPaneContextMenuCommand_CDockingManager__QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CDockingManager*, unsigned int, int, void*, void*) {
    return FALSE;
}

// Symbol: ?RecalcLayout@CDockingManager@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int) {
    if (pThis) pThis->RecalcLayout();
}

// Symbol: ?RedrawAllMiniFrames@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(CDockingManager*) {}

// Symbol: ?ReleaseEmptyPaneContainers@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CDockingManager__QEAAXXZ(CDockingManager*) {}

// Symbol: ?RemoveHiddenMDITabbedBar@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemoveHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(CDockingManager* pThis, CDockablePane* pPane) {
    RemoveDockingPane(pThis, pPane);
}

// Symbol: ?RemoveMiniFrame@CDockingManager@@UEAAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__RemoveMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(CDockingManager* pThis, void* pFrame) {
    if (!pThis || !pFrame) return FALSE;
    auto& frames = EnsureDockingState(pThis).miniFrames;
    auto oldSize = frames.size();
    frames.erase(std::remove(frames.begin(), frames.end(), pFrame), frames.end());
    return frames.size() != oldSize;
}

// Symbol: ?RemovePaneFromDockManager@CDockingManager@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int, int, int, CBasePane*) {
    RemoveDockingPane(pThis, pPane);
}

// Symbol: ?ReplacePane@CDockingManager@@QEAAHPEAVCDockablePane@@0@Z
extern "C" int MS_ABI impl__ReplacePane_CDockingManager__QEAAHPEAVCDockablePane__0_Z(
    CDockingManager* pThis, CDockablePane* pOldPane, CDockablePane* pNewPane) {
    if (!pThis || !pOldPane || !pNewPane) return FALSE;
    DockingManagerState& state = EnsureDockingState(pThis);
    auto it = std::find(state.panes.begin(), state.panes.end(), static_cast<CBasePane*>(pOldPane));
    if (it == state.panes.end()) return FALSE;
    *it = pNewPane;
    state.hiddenPanes.erase(pOldPane);
    state.floatingPanes.erase(pOldPane);
    return TRUE;
}

// Symbol: ?ResortMiniFramesForZOrder@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__ResortMiniFramesForZOrder_CDockingManager__QEAAXXZ(CDockingManager*) {}

// Symbol: ?SaveState@CDockingManager@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t*, unsigned int) {
    if (pThis) EnsureDockingState(pThis);
    return TRUE;
}

// Symbol: ?SendMessageToMiniFrames@CDockingManager@@QEAAHI_K_J@Z
extern "C" int MS_ABI impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(CDockingManager*, unsigned int, unsigned __int64, __int64) {
    return 0;
}

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

IMPLEMENT_DYNAMIC(CMFCToolBarEditBoxButton, CMFCToolBarButton)
CMFCToolBarEditBoxButton::CMFCToolBarEditBoxButton() { memset(_pad, 0, sizeof(_pad)); }

IMPLEMENT_DYNAMIC(CMFCToolBarMenuButton, CMFCToolBarButton)
CMFCToolBarMenuButton::CMFCToolBarMenuButton() : m_hMenu(nullptr) { memset(_pad, 0, sizeof(_pad)); }
CMFCToolBarMenuButton::CMFCToolBarMenuButton(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton)
    : CMFCToolBarMenuButton() {
    Initialize(uiID, hMenu, bHasDropDownArrow, lpszText, bUserButton);
}
CMFCToolBarMenuButton::CMFCToolBarMenuButton(const CMFCToolBarMenuButton& src)
    : CMFCToolBarButton(src.m_nID, src.m_iImage, static_cast<const wchar_t*>(src.m_strText), src.m_bUserButton, src.m_bLocked),
      m_hMenu(src.m_hMenu) {
    memset(_pad, 0, sizeof(_pad));
    CopyMenuButtonState(this, &src);
}
CMFCToolBarMenuButton::~CMFCToolBarMenuButton() {
    g_menuButtonStates.erase(this);
}

void CMFCToolBarMenuButton::Initialize(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton) {
    m_nID = uiID;
    m_hMenu = hMenu;
    m_bUserButton = bUserButton;
    if (lpszText) m_strText = lpszText;
    MenuButtonState& state = g_menuButtonStates[this];
    state.hasDropDownArrow = bHasDropDownArrow != FALSE;
    state.userButton = bUserButton != FALSE;
}

void CMFCToolBarMenuButton::CreateFromMenu(HMENU hMenu) {
    m_hMenu = hMenu;
}

HMENU CMFCToolBarMenuButton::CreateMenu() const {
    return m_hMenu ? m_hMenu : ::CreatePopupMenu();
}

void CMFCToolBarMenuButton::CopyFrom(const CMFCToolBarButton& src) {
    m_nID = src.m_nID;
    m_iImage = src.m_iImage;
    m_strText = src.m_strText;
    m_bUserButton = src.m_bUserButton;
    m_bLocked = src.m_bLocked;
    if (src.IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
        const CMFCToolBarMenuButton& menuSrc = static_cast<const CMFCToolBarMenuButton&>(src);
        m_hMenu = menuSrc.m_hMenu;
        CopyMenuButtonState(this, &menuSrc);
    }
}

int CMFCToolBarMenuButton::CompareWith(const CMFCToolBarButton& other) const {
    if (m_nID != other.m_nID) {
        return m_nID < other.m_nID ? -1 : 1;
    }
    return m_strText.Compare(other.m_strText);
}

IMPLEMENT_DYNAMIC(CMFCToolBarMenuButtonsButton, CMFCToolBarMenuButton)
CMFCToolBarMenuButtonsButton::CMFCToolBarMenuButtonsButton()
    : m_uiSystemCommand(0) {
    memset(_padButtons, 0, sizeof(_padButtons));
}
CMFCToolBarMenuButtonsButton::CMFCToolBarMenuButtonsButton(UINT uiSystemCommand)
    : CMFCToolBarMenuButtonsButton() {
    m_uiSystemCommand = uiSystemCommand;
    m_nID = uiSystemCommand;
}
CMFCToolBarMenuButtonsButton::~CMFCToolBarMenuButtonsButton() {}

// Symbol: ??0CMFCToolBarMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, unsigned int uiID, HMENU hMenu, int bHasDropDownArrow, const wchar_t* lpszText, int bUserButton) {
    return new (pThis) CMFCToolBarMenuButton(uiID, hMenu, bHasDropDownArrow, lpszText, bUserButton);
}

// Symbol: ??0CMFCToolBarMenuButton@@QEAA@AEBV0@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_AEBV0__Z(void* pThis, const CMFCToolBarMenuButton* pSrc) {
    return new (pThis) CMFCToolBarMenuButton(*pSrc);
}

// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXIPEAUHMENU____HPEB_WH_Z(
    CMFCToolBarMenuButton* pThis, unsigned int uiID, HMENU hMenu, int bHasDropDownArrow, const wchar_t* lpszText, int bUserButton) {
    if (pThis) pThis->Initialize(uiID, hMenu, bHasDropDownArrow, lpszText, bUserButton);
}

// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXXZ(CMFCToolBarMenuButton* pThis) {
    if (pThis) pThis->Initialize(0, nullptr, FALSE, nullptr, FALSE);
}

// Symbol: ?CreateFromMenu@CMFCToolBarMenuButton@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(CMFCToolBarMenuButton* pThis, HMENU hMenu) {
    if (pThis) pThis->CreateFromMenu(hMenu);
}

// Symbol: ?CreateMenu@CMFCToolBarMenuButton@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCToolBarMenuButton__UEBAPEAUHMENU____XZ(const CMFCToolBarMenuButton* pThis) {
    return pThis ? pThis->CreateMenu() : nullptr;
}

// Symbol: ?CopyFrom@CMFCToolBarMenuButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc) {
    if (pThis && pSrc) pThis->CopyFrom(*pSrc);
}

// Symbol: ?CompareWith@CMFCToolBarMenuButton@@UEBAHAEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarMenuButton__UEBAHAEBVCMFCToolBarButton___Z(const CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pOther) {
    return (pThis && pOther) ? pThis->CompareWith(*pOther) : 0;
}

// Symbol: ??0CMFCToolBarMenuButtonsButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButtonsButton__QEAA_XZ(void* pThis) {
    return new (pThis) CMFCToolBarMenuButtonsButton();
}

// Symbol: ??0CMFCToolBarMenuButtonsButton@@QEAA@I@Z
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButtonsButton__QEAA_I_Z(void* pThis, unsigned int uiSystemCommand) {
    return new (pThis) CMFCToolBarMenuButtonsButton(uiSystemCommand);
}

// Symbol: ??1CMFCToolBarMenuButtonsButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarMenuButtonsButton__UEAA_XZ(CMFCToolBarMenuButtonsButton* pThis) {
    if (pThis) pThis->~CMFCToolBarMenuButtonsButton();
}

//=============================================================================
// Feature Pack managers/settings helpers
//=============================================================================
IMPLEMENT_DYNAMIC(CContextMenuManager, CObject)

CContextMenuManager::CContextMenuManager() { memset(_contextmenumanager_padding, 0, sizeof(_contextmenumanager_padding)); }
CContextMenuManager::~CContextMenuManager() {
    auto it = g_contextMenuStates.find(this);
    if (it != g_contextMenuStates.end()) {
        DestroyContextMenus(it->second);
        g_contextMenuStates.erase(it);
    }
}

BOOL CContextMenuManager::AddMenu(UINT uiMenuNameResId, UINT uiMenuResId) {
    wchar_t buffer[32] = {};
    _snwprintf(buffer, 32, L"%u", uiMenuNameResId);
    return AddMenu(buffer, uiMenuResId);
}

BOOL CContextMenuManager::AddMenu(const wchar_t* lpszName, UINT uiMenuResId) {
    ContextMenuState& state = g_contextMenuStates[this];
    HMENU hOwnedMenu = nullptr;
    HMENU hMenu = LoadMenuResource(uiMenuResId, &hOwnedMenu);
    auto itOld = state.ownedMenusById.find(uiMenuResId);
    if (itOld != state.ownedMenusById.end() && itOld->second && itOld->second != hOwnedMenu) {
        ::DestroyMenu(itOld->second);
    }
    state.menusById[uiMenuResId] = hMenu;
    state.ownedMenusById[uiMenuResId] = hOwnedMenu;
    if (lpszName) state.idsByName[lpszName] = uiMenuResId;
    return hMenu != nullptr;
}

HMENU CContextMenuManager::GetMenuById(UINT uiMenuResId) const {
    auto itState = g_contextMenuStates.find(this);
    if (itState == g_contextMenuStates.end()) return nullptr;
    auto it = itState->second.menusById.find(uiMenuResId);
    return it == itState->second.menusById.end() ? nullptr : it->second;
}

HMENU CContextMenuManager::GetMenuByName(const wchar_t* lpszName, UINT* puiMenuResId) const {
    auto itState = g_contextMenuStates.find(this);
    if (itState == g_contextMenuStates.end()) return nullptr;
    auto it = itState->second.idsByName.find(WideKey(lpszName));
    if (it == itState->second.idsByName.end()) return nullptr;
    if (puiMenuResId) *puiMenuResId = it->second;
    return GetMenuById(it->second);
}

void CContextMenuManager::GetMenuNames(CStringList& listOfNames) const {
    auto itState = g_contextMenuStates.find(this);
    if (itState == g_contextMenuStates.end()) return;
    for (const auto& entry : itState->second.idsByName) {
        listOfNames.AddTail(CString(entry.first.c_str()));
    }
}

BOOL CContextMenuManager::ShowPopupMenu(UINT uiMenuResId, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage, BOOL bRightAlign) {
    HMENU hMenu = GetMenuById(uiMenuResId);
    if (!hMenu) return FALSE;
    return ShowPopupMenu(hMenu, x, y, pWndOwner, bOwnMessage, TRUE, bRightAlign) != nullptr;
}

CMFCPopupMenu* CContextMenuManager::ShowPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL, BOOL bAutoDestroy, BOOL) {
    (void)bAutoDestroy;
    if (!hmenuPopup) return nullptr;
    CMFCPopupMenu* pPopup = new CMFCPopupMenu();
    if (!pPopup->Create(pWndOwner, x, y, hmenuPopup, FALSE, FALSE)) {
        delete pPopup;
        return nullptr;
    }
    CMFCPopupMenu::ActivatePopupMenu(nullptr, pPopup);
    return pPopup;
}

UINT CContextMenuManager::TrackPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL) {
    if (!hmenuPopup) return 0;
    return static_cast<UINT>(::TrackPopupMenu(hmenuPopup, TPM_RETURNCMD, x, y, 0, pWndOwner ? pWndOwner->GetSafeHwnd() : nullptr, nullptr));
}

BOOL CContextMenuManager::LoadState(const wchar_t*) { return TRUE; }
BOOL CContextMenuManager::SaveState(const wchar_t*) { return TRUE; }
BOOL CContextMenuManager::ResetState() {
    auto& state = g_contextMenuStates[this];
    DestroyContextMenus(state);
    return TRUE;
}

// Symbol: ??0CContextMenuManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CContextMenuManager__QEAA_XZ(void* pThis) { return new (pThis) CContextMenuManager(); }
// Symbol: ??1CContextMenuManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CContextMenuManager__UEAA_XZ(CContextMenuManager* pThis) { if (pThis) pThis->~CContextMenuManager(); }
// Symbol: ?AddMenu@CContextMenuManager@@QEAAHII@Z
extern "C" int MS_ABI impl__AddMenu_CContextMenuManager__QEAAHII_Z(CContextMenuManager* pThis, unsigned int nameId, unsigned int menuId) { return pThis ? pThis->AddMenu(nameId, menuId) : FALSE; }
// Symbol: ?AddMenu@CContextMenuManager@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__AddMenu_CContextMenuManager__QEAAHPEB_WI_Z(CContextMenuManager* pThis, const wchar_t* name, unsigned int menuId) { return pThis ? pThis->AddMenu(name, menuId) : FALSE; }
// Symbol: ?GetMenuById@CContextMenuManager@@QEBAPEAUHMENU__@@I@Z
extern "C" HMENU MS_ABI impl__GetMenuById_CContextMenuManager__QEBAPEAUHMENU____I_Z(const CContextMenuManager* pThis, unsigned int id) { return pThis ? pThis->GetMenuById(id) : nullptr; }
// Symbol: ?GetMenuByName@CContextMenuManager@@QEBAPEAUHMENU__@@PEB_WPEAI@Z
extern "C" HMENU MS_ABI impl__GetMenuByName_CContextMenuManager__QEBAPEAUHMENU____PEB_WPEAI_Z(const CContextMenuManager* pThis, const wchar_t* name, unsigned int* id) { return pThis ? pThis->GetMenuByName(name, id) : nullptr; }
// Symbol: ?GetMenuNames@CContextMenuManager@@QEBAXAEAVCStringList@@@Z
extern "C" void MS_ABI impl__GetMenuNames_CContextMenuManager__QEBAXAEAVCStringList___Z(const CContextMenuManager* pThis, CStringList* list) { if (pThis && list) pThis->GetMenuNames(*list); }
// Symbol: ?ShowPopupMenu@CContextMenuManager@@UEAAHIHHPEAVCWnd@@HH@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CContextMenuManager__UEAAHIHHPEAVCWnd__HH_Z(CContextMenuManager* pThis, unsigned int id, int x, int y, CWnd* owner, int own, int right) { return pThis ? pThis->ShowPopupMenu(id, x, y, owner, own, right) : FALSE; }
// Symbol: ?ShowPopupMenu@CContextMenuManager@@UEAAPEAVCMFCPopupMenu@@PEAUHMENU__@@HHPEAVCWnd@@HHH@Z
extern "C" CMFCPopupMenu* MS_ABI impl__ShowPopupMenu_CContextMenuManager__UEAAPEAVCMFCPopupMenu__PEAUHMENU____HHPEAVCWnd__HHH_Z(CContextMenuManager* pThis, HMENU menu, int x, int y, CWnd* owner, int own, int destroy, int right) { return pThis ? pThis->ShowPopupMenu(menu, x, y, owner, own, destroy, right) : nullptr; }
// Symbol: ?TrackPopupMenu@CContextMenuManager@@UEAAIPEAUHMENU__@@HHPEAVCWnd@@H@Z
extern "C" unsigned int MS_ABI impl__TrackPopupMenu_CContextMenuManager__UEAAIPEAUHMENU____HHPEAVCWnd__H_Z(CContextMenuManager* pThis, HMENU menu, int x, int y, CWnd* owner, int right) { return pThis ? pThis->TrackPopupMenu(menu, x, y, owner, right) : 0; }
// Symbol: ?ResetState@CContextMenuManager@@UEAAHXZ
extern "C" int MS_ABI impl__ResetState_CContextMenuManager__UEAAHXZ(CContextMenuManager* pThis) { return pThis ? pThis->ResetState() : FALSE; }
// Symbol: ?LoadState@CContextMenuManager@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadState_CContextMenuManager__UEAAHPEB_W_Z(CContextMenuManager* pThis, const wchar_t* profile) { return pThis ? pThis->LoadState(profile) : FALSE; }
// Symbol: ?SaveState@CContextMenuManager@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveState_CContextMenuManager__UEAAHPEB_W_Z(CContextMenuManager* pThis, const wchar_t* profile) { return pThis ? pThis->SaveState(profile) : FALSE; }

IMPLEMENT_DYNAMIC(CKeyboardManager, CObject)
CKeyboardManager::CKeyboardManager() { memset(_keyboardmanager_padding, 0, sizeof(_keyboardmanager_padding)); }
CKeyboardManager::~CKeyboardManager() {}
BOOL CKeyboardManager::IsKeyPrintable(UINT nChar) { return nChar >= 0x20 && nChar < 0x7f; }
UINT CKeyboardManager::TranslateCharToUpper(UINT nChar) { return static_cast<UINT>(std::towupper(static_cast<wint_t>(nChar))); }
void CKeyboardManager::ShowAllAccelerators(BOOL bShowAll) { g_showAllAccelerators = bShowAll; }
void CKeyboardManager::CleanUp() { g_showAllAccelerators = FALSE; }
void CKeyboardManager::ResetAll() { g_showAllAccelerators = FALSE; }
BOOL CKeyboardManager::LoadState(const wchar_t*, CFrameWnd*) { return TRUE; }
BOOL CKeyboardManager::SaveState(const wchar_t*, CFrameWnd*) { return TRUE; }
// Symbol: ??0CKeyboardManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CKeyboardManager__QEAA_XZ(void* pThis) { return new (pThis) CKeyboardManager(); }
// Symbol: ??1CKeyboardManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CKeyboardManager__UEAA_XZ(CKeyboardManager* pThis) { if (pThis) pThis->~CKeyboardManager(); }
// Symbol: ?IsKeyPrintable@CKeyboardManager@@SAHI@Z
extern "C" int MS_ABI impl__IsKeyPrintable_CKeyboardManager__SAHI_Z(unsigned int ch) { return CKeyboardManager::IsKeyPrintable(ch); }
// Symbol: ?TranslateCharToUpper@CKeyboardManager@@SAII@Z
extern "C" unsigned int MS_ABI impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(unsigned int ch) { return CKeyboardManager::TranslateCharToUpper(ch); }
// Symbol: ?ShowAllAccelerators@CKeyboardManager@@SAXH@Z
extern "C" void MS_ABI impl__ShowAllAccelerators_CKeyboardManager__SAXH_Z(int show) { CKeyboardManager::ShowAllAccelerators(show); }
// Symbol: ?CleanUp@CKeyboardManager@@SAXXZ
extern "C" void MS_ABI impl__CleanUp_CKeyboardManager__SAXXZ() { CKeyboardManager::CleanUp(); }
// Symbol: ?ResetAll@CKeyboardManager@@QEAAXXZ
extern "C" void MS_ABI impl__ResetAll_CKeyboardManager__QEAAXXZ(CKeyboardManager* pThis) { if (pThis) pThis->ResetAll(); }
// Symbol: ?LoadState@CKeyboardManager@@QEAAHPEB_WPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__LoadState_CKeyboardManager__QEAAHPEB_WPEAVCFrameWnd___Z(CKeyboardManager* pThis, const wchar_t* profile, CFrameWnd* frame) { return pThis ? pThis->LoadState(profile, frame) : FALSE; }
// Symbol: ?SaveState@CKeyboardManager@@QEAAHPEB_WPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__SaveState_CKeyboardManager__QEAAHPEB_WPEAVCFrameWnd___Z(CKeyboardManager* pThis, const wchar_t* profile, CFrameWnd* frame) { return pThis ? pThis->SaveState(profile, frame) : FALSE; }

IMPLEMENT_DYNAMIC(CTooltipManager, CObject)
CTooltipManager::CTooltipManager() { memset(_tooltipmanager_padding, 0, sizeof(_tooltipmanager_padding)); }
CTooltipManager::~CTooltipManager() { g_tooltipManagerStates.erase(this); }
BOOL CTooltipManager::CreateToolTip(CToolTipCtrl*& pToolTip, CWnd* pWndParent, UINT nType) {
    const bool allocated = pToolTip == nullptr;
    if (allocated) pToolTip = new CToolTipCtrl();
    BOOL created = pToolTip->Create(pWndParent, nType);
    if (!created && allocated) {
        delete pToolTip;
        pToolTip = nullptr;
    }
    return created;
}

// Symbol: ?CreateToolTipObject@CTooltipManager@@IEAAHAEAPEAVCToolTipCtrl@@I@Z
extern "C" int MS_ABI impl__CreateToolTipObject_CTooltipManager__IEAAHAEAPEAVCToolTipCtrl__I_Z(
    CTooltipManager* pThis, CToolTipCtrl** ppToolTip, unsigned int nType) {
    (void)pThis;
    return ppToolTip ? CTooltipManager::CreateToolTip(*ppToolTip, nullptr, nType) : FALSE;
}

void CTooltipManager::DeleteToolTip(CToolTipCtrl*& pToolTip) {
    if (pToolTip) {
        pToolTip->DestroyWindow();
        delete pToolTip;
        pToolTip = nullptr;
    }
}
void CTooltipManager::SetTooltipParams(UINT nTypes, CRuntimeClass* pRTC, CMFCToolTipInfo* pParams) {
    TooltipManagerState& state = g_tooltipManagerStates[this];
    state.types = nTypes; state.runtimeClass = pRTC; state.params = pParams;
}
void CTooltipManager::UpdateTooltips() {}
// Symbol: ??0CTooltipManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CTooltipManager__QEAA_XZ(void* pThis) { return new (pThis) CTooltipManager(); }
// Symbol: ??1CTooltipManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CTooltipManager__UEAA_XZ(CTooltipManager* pThis) { if (pThis) pThis->~CTooltipManager(); }
// Symbol: ?CreateToolTip@CTooltipManager@@SAHAEAPEAVCToolTipCtrl@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateToolTip_CTooltipManager__SAHAEAPEAVCToolTipCtrl__PEAVCWnd__I_Z(CToolTipCtrl** ppToolTip, CWnd* parent, unsigned int type) { return ppToolTip ? CTooltipManager::CreateToolTip(*ppToolTip, parent, type) : FALSE; }
// Symbol: ?DeleteToolTip@CTooltipManager@@SAXAEAPEAVCToolTipCtrl@@@Z
extern "C" void MS_ABI impl__DeleteToolTip_CTooltipManager__SAXAEAPEAVCToolTipCtrl___Z(CToolTipCtrl** ppToolTip) { if (ppToolTip) CTooltipManager::DeleteToolTip(*ppToolTip); }
// Symbol: ?UpdateTooltips@CTooltipManager@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CTooltipManager__QEAAXXZ(CTooltipManager* pThis) { if (pThis) pThis->UpdateTooltips(); }
// Symbol: ?SetTooltipParams@CTooltipManager@@QEAAXIPEAUCRuntimeClass@@PEAVCMFCToolTipInfo@@@Z
extern "C" void MS_ABI impl__SetTooltipParams_CTooltipManager__QEAAXIPEAUCRuntimeClass__PEAVCMFCToolTipInfo___Z(CTooltipManager* pThis, unsigned int types, CRuntimeClass* rtc, CMFCToolTipInfo* params) { if (pThis) pThis->SetTooltipParams(types, rtc, params); }
// Symbol: ?SetTooltipText@CTooltipManager@@SAXPEAUtagTOOLINFOW@@PEAVCToolTipCtrl@@IV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__SetTooltipText_CTooltipManager__SAXPEAUtagTOOLINFOW__PEAVCToolTipCtrl__IV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    TOOLINFOW* toolInfo, CToolTipCtrl* toolTip, unsigned int id, const CString* text, const wchar_t* description) {
    const wchar_t* resolved = text ? text->GetString() : nullptr;
    if (!resolved || !*resolved) resolved = description ? description : L"";

    TOOLINFOW localInfo = toolInfo ? *toolInfo : TOOLINFOW{};
    localInfo.cbSize = sizeof(TOOLINFOW);
    localInfo.uId = id ? id : localInfo.uId;
    localInfo.lpszText = const_cast<wchar_t*>(resolved);

    if (toolInfo) {
        toolInfo->cbSize = localInfo.cbSize;
        toolInfo->uId = localInfo.uId;
        toolInfo->lpszText = localInfo.lpszText;
    }
    if (toolTip && toolTip->GetSafeHwnd()) {
        ::SendMessageW(toolTip->GetSafeHwnd(), TTM_UPDATETIPTEXTW, 0, reinterpret_cast<LPARAM>(&localInfo));
    }
}

IMPLEMENT_DYNAMIC(CMenuHash, CObject)
CMenuHash::CMenuHash() { memset(_menuhash_padding, 0, sizeof(_menuhash_padding)); }
CMenuHash::~CMenuHash() { g_menuHashStates.erase(this); }
BOOL CMenuHash::LoadMenuBar(HMENU hMenu, CMFCToolBar* pBar) { g_menuHashStates[this].barsByMenu[hMenu] = pBar; return TRUE; }
BOOL CMenuHash::SaveMenuBar(HMENU hMenu, CMFCToolBar* pBar) { g_menuHashStates[this].barsByMenu[hMenu] = pBar; return TRUE; }
BOOL CMenuHash::RemoveMenu(HMENU hMenu) { return g_menuHashStates[this].barsByMenu.erase(hMenu) != 0; }
void CMenuHash::CleanUp() { g_menuHashStates[this].barsByMenu.clear(); }
// Symbol: ??0CMenuHash@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMenuHash__QEAA_XZ(void* pThis) { return new (pThis) CMenuHash(); }
// Symbol: ??1CMenuHash@@UEAA@XZ
extern "C" void MS_ABI impl___1CMenuHash__UEAA_XZ(CMenuHash* pThis) { if (pThis) pThis->~CMenuHash(); }
// Symbol: ?LoadMenuBar@CMenuHash@@QEAAHPEAUHMENU__@@PEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU menu, CMFCToolBar* bar) { return pThis ? pThis->LoadMenuBar(menu, bar) : FALSE; }
// Symbol: ?SaveMenuBar@CMenuHash@@QEAAHPEAUHMENU__@@PEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU menu, CMFCToolBar* bar) { return pThis ? pThis->SaveMenuBar(menu, bar) : FALSE; }
// Symbol: ?RemoveMenu@CMenuHash@@QEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__RemoveMenu_CMenuHash__QEAAHPEAUHMENU_____Z(CMenuHash* pThis, HMENU menu) { return pThis ? pThis->RemoveMenu(menu) : FALSE; }
// Symbol: ?CleanUp@CMenuHash@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMenuHash__QEAAXXZ(CMenuHash* pThis) { if (pThis) pThis->CleanUp(); }

CGlobalUtils::CGlobalUtils() { memset(_globalutils_padding, 0, sizeof(_globalutils_padding)); }
CGlobalUtils::~CGlobalUtils() {}
void CGlobalUtils::AdjustRectToWorkArea(CRect& rect, CRect* pRectDelta) {
    CRect old = rect;
    rect.NormalizeRect();
    RECT workArea = {};
    if (!::SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0)) {
        workArea.left = 0;
        workArea.top = 0;
        workArea.right = ::GetSystemMetrics(SM_CXSCREEN);
        workArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);
    }
    if (rect.right > workArea.right) rect.OffsetRect(workArea.right - rect.right, 0);
    if (rect.bottom > workArea.bottom) rect.OffsetRect(0, workArea.bottom - rect.bottom);
    if (rect.left < workArea.left) rect.OffsetRect(workArea.left - rect.left, 0);
    if (rect.top < workArea.top) rect.OffsetRect(0, workArea.top - rect.top);
    if (pRectDelta) *pRectDelta = CRect(rect.left - old.left, rect.top - old.top, rect.right - old.right, rect.bottom - old.bottom);
}
void CGlobalUtils::FlipRect(CRect& rect, BOOL bHorz) { if (bHorz) std::swap(rect.left, rect.right); else std::swap(rect.top, rect.bottom); rect.NormalizeRect(); }
DWORD CGlobalUtils::GetOppositeAlignment(DWORD dwAlign) {
    if (dwAlign & CBRS_LEFT) return CBRS_RIGHT;
    if (dwAlign & CBRS_RIGHT) return CBRS_LEFT;
    if (dwAlign & CBRS_TOP) return CBRS_BOTTOM;
    if (dwAlign & CBRS_BOTTOM) return CBRS_TOP;
    return dwAlign;
}
CSize CGlobalUtils::GetSystemBorders(DWORD dwStyle) { (void)dwStyle; return CSize(::GetSystemMetrics(SM_CXFRAME), ::GetSystemMetrics(SM_CYFRAME)); }
CSize CGlobalUtils::GetSystemBorders(CWnd* pWnd) { return GetSystemBorders(pWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pWnd->GetSafeHwnd(), GWL_STYLE)) : 0); }
HICON CGlobalUtils::GetWndIcon(CWnd* pWnd) { return pWnd ? reinterpret_cast<HICON>(::SendMessageW(pWnd->GetSafeHwnd(), WM_GETICON, ICON_SMALL, 0)) : nullptr; }
BOOL CGlobalUtils::CanBeAttached(CWnd* pWnd) const { return pWnd != nullptr; }
BOOL CGlobalUtils::CanPaneBeInFloatingMultiPaneFrameWnd(CWnd* pWnd) const { return pWnd != nullptr; }
CDockingManager* CGlobalUtils::GetDockingManager(CWnd* pWnd) { return pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CFrameWndEx)) ? static_cast<CFrameWndEx*>(pWnd)->GetDockingManager() : nullptr; }

//=============================================================================
// CShellManager
//=============================================================================

namespace {

template <typename Fn>
Fn LoadShell32Function(const char* name) {
    HMODULE shell32 = ::GetModuleHandleW(L"shell32.dll");
    if (!shell32) shell32 = ::LoadLibraryW(L"shell32.dll");
    return shell32 ? reinterpret_cast<Fn>(::GetProcAddress(shell32, name)) : nullptr;
}

LPITEMIDLIST AllocatePidlBytes(UINT bytes) {
    if (bytes < sizeof(USHORT)) bytes = sizeof(USHORT);
    void* memory = ::CoTaskMemAlloc(bytes);
    if (!memory) return nullptr;
    std::memset(memory, 0, bytes);
    return static_cast<LPITEMIDLIST>(memory);
}

} // namespace

int CALLBACK CShellManager::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM, LPARAM lpData) {
    if (uMsg == BFFM_INITIALIZED && lpData != 0) {
        ::SendMessageW(hwnd, BFFM_SETSELECTIONW, TRUE, lpData);
    }
    return 0;
}

BOOL CShellManager::BrowseForFolder(CString& strOutFolder, CWnd* pWndParent, const wchar_t* lpszTitle,
                                    const wchar_t* lpszInitialFolder, UINT ulFlags, int* piFolderImage) {
    using SHBrowseForFolderWFn = LPITEMIDLIST (WINAPI*)(LPBROWSEINFOW);
    using SHGetPathFromIDListWFn = BOOL (WINAPI*)(LPCITEMIDLIST, LPWSTR);

    auto browseForFolder = LoadShell32Function<SHBrowseForFolderWFn>("SHBrowseForFolderW");
    auto getPathFromIDList = LoadShell32Function<SHGetPathFromIDListWFn>("SHGetPathFromIDListW");
    if (!browseForFolder || !getPathFromIDList) return FALSE;

    wchar_t displayName[MAX_PATH] = {};
    BROWSEINFOW browseInfo = {};
    browseInfo.hwndOwner = pWndParent ? pWndParent->GetSafeHwnd() : nullptr;
    browseInfo.pszDisplayName = displayName;
    browseInfo.lpszTitle = lpszTitle;
    browseInfo.ulFlags = ulFlags;
    browseInfo.lpfn = &CShellManager::BrowseCallbackProc;
    browseInfo.lParam = reinterpret_cast<LPARAM>(lpszInitialFolder);
    browseInfo.iImage = 0;

    LPITEMIDLIST selected = browseForFolder(&browseInfo);
    if (!selected) return FALSE;

    wchar_t path[MAX_PATH] = {};
    BOOL ok = getPathFromIDList(selected, path);
    if (ok) strOutFolder = path;
    if (piFolderImage) *piFolderImage = browseInfo.iImage;
    FreeItem(selected);
    return ok;
}

LPITEMIDLIST CShellManager::CreateItem(UINT cbSize) {
    if (cbSize < sizeof(USHORT)) cbSize = sizeof(USHORT);
    LPITEMIDLIST pidl = AllocatePidlBytes(cbSize + sizeof(USHORT));
    if (pidl) pidl->mkid.cb = static_cast<USHORT>(cbSize);
    return pidl;
}

void CShellManager::FreeItem(LPITEMIDLIST pidl) {
    ::CoTaskMemFree(pidl);
}

UINT CShellManager::GetItemSize(LPCITEMIDLIST pidl) {
    if (!pidl) return 0;
    UINT total = 0;
    const BYTE* cursor = reinterpret_cast<const BYTE*>(pidl);
    for (;;) {
        const auto* item = reinterpret_cast<LPCITEMIDLIST>(cursor);
        total += sizeof(USHORT);
        if (item->mkid.cb == 0) break;
        total += item->mkid.cb - sizeof(USHORT);
        cursor += item->mkid.cb;
    }
    return total;
}

UINT CShellManager::GetItemCount(LPCITEMIDLIST pidl) {
    if (!pidl) return 0;
    UINT count = 0;
    const BYTE* cursor = reinterpret_cast<const BYTE*>(pidl);
    for (;;) {
        const auto* item = reinterpret_cast<LPCITEMIDLIST>(cursor);
        if (item->mkid.cb == 0) break;
        ++count;
        cursor += item->mkid.cb;
    }
    return count;
}

LPITEMIDLIST CShellManager::GetNextItem(LPCITEMIDLIST pidl) {
    if (!pidl || pidl->mkid.cb == 0) return nullptr;
    return reinterpret_cast<LPITEMIDLIST>(const_cast<BYTE*>(reinterpret_cast<const BYTE*>(pidl)) + pidl->mkid.cb);
}

LPITEMIDLIST CShellManager::CopyItem(LPCITEMIDLIST pidl) {
    UINT bytes = GetItemSize(pidl);
    if (bytes == 0) return nullptr;
    LPITEMIDLIST copy = AllocatePidlBytes(bytes);
    if (copy) std::memcpy(copy, pidl, bytes);
    return copy;
}

LPITEMIDLIST CShellManager::ConcatenateItem(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2) {
    if (!pidl1) return CopyItem(pidl2);
    if (!pidl2) return CopyItem(pidl1);

    UINT size1 = GetItemSize(pidl1);
    UINT size2 = GetItemSize(pidl2);
    if (size1 == 0 || size2 == 0) return nullptr;

    UINT payload1 = size1 - sizeof(USHORT);
    LPITEMIDLIST combined = AllocatePidlBytes(payload1 + size2);
    if (!combined) return nullptr;

    std::memcpy(combined, pidl1, payload1);
    std::memcpy(reinterpret_cast<BYTE*>(combined) + payload1, pidl2, size2);
    return combined;
}

BOOL CShellManager::GetParentItem(LPCITEMIDLIST pidl, LPITEMIDLIST& pidlParent) {
    pidlParent = nullptr;
    if (!pidl || pidl->mkid.cb == 0) return FALSE;

    const BYTE* base = reinterpret_cast<const BYTE*>(pidl);
    const BYTE* cursor = base;
    const BYTE* last = base;
    while (reinterpret_cast<LPCITEMIDLIST>(cursor)->mkid.cb != 0) {
        last = cursor;
        cursor += reinterpret_cast<LPCITEMIDLIST>(cursor)->mkid.cb;
    }

    UINT parentBytes = static_cast<UINT>(last - base) + sizeof(USHORT);
    pidlParent = AllocatePidlBytes(parentBytes);
    if (!pidlParent) return FALSE;
    if (parentBytes > sizeof(USHORT)) {
        std::memcpy(pidlParent, pidl, parentBytes - sizeof(USHORT));
    }
    return TRUE;
}

HRESULT CShellManager::ItemFromPath(const wchar_t* lpszPath, LPITEMIDLIST& pidl) {
    pidl = nullptr;
    if (!lpszPath || *lpszPath == L'\0') return E_INVALIDARG;

    using SHParseDisplayNameFn = HRESULT (WINAPI*)(PCWSTR, IBindCtx*, PIDLIST_ABSOLUTE*, SFGAOF, SFGAOF*);
    auto parseDisplayName = LoadShell32Function<SHParseDisplayNameFn>("SHParseDisplayName");
    if (parseDisplayName) {
        PIDLIST_ABSOLUTE absolute = nullptr;
        HRESULT hr = parseDisplayName(lpszPath, nullptr, &absolute, 0, nullptr);
        if (SUCCEEDED(hr)) pidl = absolute;
        return hr;
    }

    PIDLIST_ABSOLUTE absolute = nullptr;
    IShellFolder* desktop = nullptr;
    HRESULT hr = ::SHGetDesktopFolder(&desktop);
    if (FAILED(hr)) return hr;

    SFGAOF attributes = 0;
    ULONG chEaten = 0;
    hr = desktop->ParseDisplayName(nullptr, nullptr, const_cast<LPWSTR>(lpszPath), &chEaten, &absolute, &attributes);
    desktop->Release();
    if (SUCCEEDED(hr)) pidl = absolute;
    return hr;
}

// Symbol: ??0CShellManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CShellManager__QEAA_XZ(CShellManager* pThis) {
    return pThis ? new(pThis) CShellManager() : nullptr;
}

// Symbol: ??1CShellManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CShellManager__UEAA_XZ(CShellManager* pThis) {
    if (pThis) pThis->~CShellManager();
}

// Symbol: ?BrowseCallbackProc@CShellManager@@KAHPEAUHWND__@@I_J1@Z
extern "C" int MS_ABI impl__BrowseCallbackProc_CShellManager__KAHPEAUHWND____I_J1_Z(
    HWND hwnd, unsigned int uMsg, __int64 lParam, __int64 lpData) {
    return CShellManager::BrowseCallbackProc(hwnd, uMsg, static_cast<LPARAM>(lParam), static_cast<LPARAM>(lpData));
}

// Symbol: ?BrowseForFolder@CShellManager@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCWnd@@PEB_W2IPEAH@Z
extern "C" int MS_ABI impl__BrowseForFolder_CShellManager__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCWnd__PEB_W2IPEAH_Z(
    CShellManager* pThis, CString* strOutFolder, CWnd* pWndParent, const wchar_t* lpszTitle,
    const wchar_t* lpszInitialFolder, unsigned int ulFlags, int* piFolderImage) {
    if (!pThis || !strOutFolder) return FALSE;
    return pThis->BrowseForFolder(*strOutFolder, pWndParent, lpszTitle, lpszInitialFolder, ulFlags, piFolderImage);
}

// Symbol: ?ConcatenateItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@0@Z
extern "C" LPITEMIDLIST MS_ABI impl__ConcatenateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2_0_Z(
    CShellManager* pThis, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2) {
    return pThis ? pThis->ConcatenateItem(pidl1, pidl2) : nullptr;
}

// Symbol: ?CopyItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@@Z
extern "C" LPITEMIDLIST MS_ABI impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(
    CShellManager* pThis, LPCITEMIDLIST pidl) {
    return pThis ? pThis->CopyItem(pidl) : nullptr;
}

// Symbol: ?CreateItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@I@Z
extern "C" LPITEMIDLIST MS_ABI impl__CreateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__I_Z(
    CShellManager* pThis, unsigned int cbSize) {
    return pThis ? pThis->CreateItem(cbSize) : nullptr;
}

// Symbol: ?FreeItem@CShellManager@@QEAAXPEFAU_ITEMIDLIST@@@Z
extern "C" void MS_ABI impl__FreeItem_CShellManager__QEAAXPEFAU_ITEMIDLIST___Z(
    CShellManager* pThis, LPITEMIDLIST pidl) {
    if (pThis) pThis->FreeItem(pidl);
}

// Symbol: ?GetItemCount@CShellManager@@QEAAIPEFBU_ITEMIDLIST@@@Z
extern "C" unsigned int MS_ABI impl__GetItemCount_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(
    CShellManager* pThis, LPCITEMIDLIST pidl) {
    return pThis ? pThis->GetItemCount(pidl) : 0;
}

// Symbol: ?GetItemSize@CShellManager@@QEAAIPEFBU_ITEMIDLIST@@@Z
extern "C" unsigned int MS_ABI impl__GetItemSize_CShellManager__QEAAIPEFBU_ITEMIDLIST___Z(
    CShellManager* pThis, LPCITEMIDLIST pidl) {
    return pThis ? pThis->GetItemSize(pidl) : 0;
}

// Symbol: ?GetNextItem@CShellManager@@QEAAPEFAU_ITEMIDLIST@@PEFBU2@@Z
extern "C" LPITEMIDLIST MS_ABI impl__GetNextItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(
    CShellManager* pThis, LPCITEMIDLIST pidl) {
    return pThis ? pThis->GetNextItem(pidl) : nullptr;
}

// Symbol: ?GetParentItem@CShellManager@@QEAAHPEFBU_ITEMIDLIST@@AEAPEFAU2@@Z
extern "C" int MS_ABI impl__GetParentItem_CShellManager__QEAAHPEFBU_ITEMIDLIST__AEAPEFAU2__Z(
    CShellManager* pThis, LPCITEMIDLIST pidl, LPITEMIDLIST* pidlParent) {
    if (!pThis || !pidlParent) return FALSE;
    LPITEMIDLIST parent = nullptr;
    BOOL ok = pThis->GetParentItem(pidl, parent);
    *pidlParent = parent;
    return ok;
}

// Symbol: ?ItemFromPath@CShellManager@@QEAAJPEB_WAEAPEFAU_ITEMIDLIST@@@Z
extern "C" long MS_ABI impl__ItemFromPath_CShellManager__QEAAJPEB_WAEAPEFAU_ITEMIDLIST___Z(
    CShellManager* pThis, const wchar_t* lpszPath, LPITEMIDLIST* pidl) {
    if (!pThis || !pidl) return E_POINTER;
    LPITEMIDLIST item = nullptr;
    HRESULT hr = pThis->ItemFromPath(lpszPath, item);
    *pidl = item;
    return hr;
}

// Symbol: ??0CGlobalUtils@@QEAA@XZ
extern "C" void* MS_ABI impl___0CGlobalUtils__QEAA_XZ(void* pThis) { return new (pThis) CGlobalUtils(); }
// Symbol: ??1CGlobalUtils@@UEAA@XZ
extern "C" void MS_ABI impl___1CGlobalUtils__UEAA_XZ(CGlobalUtils* pThis) { if (pThis) pThis->~CGlobalUtils(); }
// Symbol: ?AdjustRectToWorkArea@CGlobalUtils@@QEAAXAEAVCRect@@PEAV2@@Z
extern "C" void MS_ABI impl__AdjustRectToWorkArea_CGlobalUtils__QEAAXAEAVCRect__PEAV2__Z(CGlobalUtils* pThis, CRect* rect, CRect* delta) { if (pThis && rect) pThis->AdjustRectToWorkArea(*rect, delta); }
// Symbol: ?FlipRect@CGlobalUtils@@QEAAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__FlipRect_CGlobalUtils__QEAAXAEAVCRect__H_Z(CGlobalUtils* pThis, CRect* rect, int horz) { if (pThis && rect) pThis->FlipRect(*rect, horz); }
// Symbol: ?GetOppositeAlignment@CGlobalUtils@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__GetOppositeAlignment_CGlobalUtils__QEAAKK_Z(CGlobalUtils* pThis, unsigned long align) { return pThis ? pThis->GetOppositeAlignment(align) : align; }
// Symbol: ?GetSystemBorders@CGlobalUtils@@QEAA?AVCSize@@K@Z
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__K_Z(CSize* ret, CGlobalUtils* pThis, unsigned long style) { new (ret) CSize(pThis ? pThis->GetSystemBorders(style) : CSize()); }
// Symbol: ?GetSystemBorders@CGlobalUtils@@QEAA?AVCSize@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__GetSystemBorders_CGlobalUtils__QEAA_AVCSize__PEAVCWnd___Z(CSize* ret, CGlobalUtils* pThis, CWnd* wnd) { new (ret) CSize(pThis ? pThis->GetSystemBorders(wnd) : CSize()); }
// Symbol: ?GetWndIcon@CGlobalUtils@@QEAAPEAUHICON__@@PEAVCWnd@@@Z
extern "C" HICON MS_ABI impl__GetWndIcon_CGlobalUtils__QEAAPEAUHICON____PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->GetWndIcon(wnd) : nullptr; }
// Symbol: ?CanBeAttached@CGlobalUtils@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CanBeAttached_CGlobalUtils__QEBAHPEAVCWnd___Z(const CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->CanBeAttached(wnd) : FALSE; }
// Symbol: ?CanPaneBeInFloatingMultiPaneFrameWnd@CGlobalUtils@@QEBAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CanPaneBeInFloatingMultiPaneFrameWnd_CGlobalUtils__QEBAHPEAVCWnd___Z(const CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->CanPaneBeInFloatingMultiPaneFrameWnd(wnd) : FALSE; }
// Symbol: ?GetDockingManager@CGlobalUtils@@QEAAPEAVCDockingManager@@PEAVCWnd@@@Z
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(CGlobalUtils* pThis, CWnd* wnd) { return pThis ? pThis->GetDockingManager(wnd) : nullptr; }

IMPLEMENT_DYNAMIC(CWinAppEx, CWinApp)
CWinAppEx::CWinAppEx(BOOL) : CWinApp(nullptr), m_pContextMenuManager(nullptr), m_pKeyboardManager(nullptr), m_pTooltipManager(nullptr), m_nDataVersion(1) { memset(_winappex_padding, 0, sizeof(_winappex_padding)); }
CWinAppEx::~CWinAppEx() { delete m_pContextMenuManager; delete m_pKeyboardManager; delete m_pTooltipManager; g_winAppExStates.erase(this); }
BOOL CWinAppEx::InitContextMenuManager() { if (!m_pContextMenuManager) m_pContextMenuManager = new CContextMenuManager(); return TRUE; }
BOOL CWinAppEx::InitKeyboardManager() { if (!m_pKeyboardManager) m_pKeyboardManager = new CKeyboardManager(); return TRUE; }
BOOL CWinAppEx::InitTooltipManager() { if (!m_pTooltipManager) m_pTooltipManager = new CTooltipManager(); return TRUE; }
CContextMenuManager* CWinAppEx::GetContextMenuManager() { InitContextMenuManager(); return m_pContextMenuManager; }
CKeyboardManager* CWinAppEx::GetKeyboardManager() { InitKeyboardManager(); return m_pKeyboardManager; }
CTooltipManager* CWinAppEx::GetTooltipManager() { InitTooltipManager(); return m_pTooltipManager; }
int CWinAppEx::GetDataVersion() const { return m_nDataVersion; }
const wchar_t* CWinAppEx::SetRegistryBase(const wchar_t* lpszSectionName) { m_strRegSection = lpszSectionName ? lpszSectionName : L""; return static_cast<const wchar_t*>(m_strRegSection); }
CString CWinAppEx::GetRegSectionPath(const wchar_t* lpszSectionAdd) { CString path = m_strRegSection; if (lpszSectionAdd && *lpszSectionAdd) { if (static_cast<const wchar_t*>(path)[0] != 0) path += L"\\"; path += lpszSectionAdd; } return path; }
int CWinAppEx::GetInt(const wchar_t* lpszEntry, int nDefault) { auto& state = g_winAppExStates[this]; auto it = state.ints.find(AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)); return it == state.ints.end() ? nDefault : it->second; }
BOOL CWinAppEx::WriteInt(const wchar_t* lpszEntry, int nValue) { g_winAppExStates[this].ints[AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)] = nValue; return TRUE; }
CString CWinAppEx::GetString(const wchar_t* lpszEntry, const wchar_t* lpszDefault) { auto& state = g_winAppExStates[this]; auto it = state.strings.find(AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)); return it == state.strings.end() ? CString(lpszDefault ? lpszDefault : L"") : it->second; }
BOOL CWinAppEx::WriteString(const wchar_t* lpszEntry, const wchar_t* lpszValue) { g_winAppExStates[this].strings[AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)] = CString(lpszValue ? lpszValue : L""); return TRUE; }
BOOL CWinAppEx::LoadState(const wchar_t* lpszSectionName, void*) { g_winAppExStates[this].states.insert(WideKey(lpszSectionName)); return TRUE; }
BOOL CWinAppEx::SaveState(const wchar_t* lpszSectionName, void*) { g_winAppExStates[this].states.insert(WideKey(lpszSectionName)); return TRUE; }
BOOL CWinAppEx::CleanState(const wchar_t* lpszSectionName) { g_winAppExStates[this].states.erase(WideKey(lpszSectionName)); return TRUE; }
BOOL CWinAppEx::IsStateExists(const wchar_t* lpszSectionName) { return g_winAppExStates[this].states.count(WideKey(lpszSectionName)) != 0; }
int CWinAppEx::ExitInstance() { delete m_pContextMenuManager; m_pContextMenuManager = nullptr; delete m_pKeyboardManager; m_pKeyboardManager = nullptr; delete m_pTooltipManager; m_pTooltipManager = nullptr; return CWinApp::ExitInstance(); }
// Symbol: ??0CWinAppEx@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CWinAppEx__QEAA_H_Z(void* pThis, int smart) { return new (pThis) CWinAppEx(smart); }
// Symbol: ??1CWinAppEx@@UEAA@XZ
extern "C" void MS_ABI impl___1CWinAppEx__UEAA_XZ(CWinAppEx* pThis) { if (pThis) pThis->~CWinAppEx(); }
// Symbol: ?InitContextMenuManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitContextMenuManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->InitContextMenuManager() : FALSE; }
// Symbol: ?InitKeyboardManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitKeyboardManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->InitKeyboardManager() : FALSE; }
// Symbol: ?InitTooltipManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitTooltipManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->InitTooltipManager() : FALSE; }
// Symbol: ?GetContextMenuManager@CWinAppEx@@QEAAPEAVCContextMenuManager@@XZ
extern "C" CContextMenuManager* MS_ABI impl__GetContextMenuManager_CWinAppEx__QEAAPEAVCContextMenuManager__XZ(CWinAppEx* pThis) { return pThis ? pThis->GetContextMenuManager() : nullptr; }
// Symbol: ?GetKeyboardManager@CWinAppEx@@QEAAPEAVCKeyboardManager@@XZ
extern "C" CKeyboardManager* MS_ABI impl__GetKeyboardManager_CWinAppEx__QEAAPEAVCKeyboardManager__XZ(CWinAppEx* pThis) { return pThis ? pThis->GetKeyboardManager() : nullptr; }
// Symbol: ?GetTooltipManager@CWinAppEx@@QEAAPEAVCTooltipManager@@XZ
extern "C" CTooltipManager* MS_ABI impl__GetTooltipManager_CWinAppEx__QEAAPEAVCTooltipManager__XZ(CWinAppEx* pThis) { return pThis ? pThis->GetTooltipManager() : nullptr; }
// Symbol: ?GetDataVersion@CWinAppEx@@QEBAHXZ
extern "C" int MS_ABI impl__GetDataVersion_CWinAppEx__QEBAHXZ(const CWinAppEx* pThis) { return pThis ? pThis->GetDataVersion() : 0; }
// Symbol: ?SetRegistryBase@CWinAppEx@@QEAAPEB_WPEB_W@Z
extern "C" const wchar_t* MS_ABI impl__SetRegistryBase_CWinAppEx__QEAAPEB_WPEB_W_Z(CWinAppEx* pThis, const wchar_t* section) { return pThis ? pThis->SetRegistryBase(section) : nullptr; }
// Symbol: ?GetRegSectionPath@CWinAppEx@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CString* ret, CWinAppEx* pThis, const wchar_t* add) { new (ret) CString(pThis ? pThis->GetRegSectionPath(add) : CString()); }
// Symbol: ?GetInt@CWinAppEx@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__GetInt_CWinAppEx__QEAAHPEB_WH_Z(CWinAppEx* pThis, const wchar_t* entry, int defVal) { return pThis ? pThis->GetInt(entry, defVal) : defVal; }
// Symbol: ?WriteInt@CWinAppEx@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__WriteInt_CWinAppEx__QEAAHPEB_WH_Z(CWinAppEx* pThis, const wchar_t* entry, int value) { return pThis ? pThis->WriteInt(entry, value) : FALSE; }
// Symbol: ?GetString@CWinAppEx@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0@Z
extern "C" void MS_ABI impl__GetString_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(CString* ret, CWinAppEx* pThis, const wchar_t* entry, const wchar_t* defVal) { new (ret) CString(pThis ? pThis->GetString(entry, defVal) : CString(defVal ? defVal : L"")); }
// Symbol: ?GetSectionString@CWinAppEx@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W00@Z
extern "C" void MS_ABI impl__GetSectionString_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W00_Z(
    CString* ret, CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* defVal) {
    if (!pThis) {
        new (ret) CString(defVal ? defVal : L"");
        return;
    }
    auto& state = g_winAppExStates[pThis];
    auto it = state.strings.find(AppEntryKey(section, entry));
    new (ret) CString(it == state.strings.end() ? CString(defVal ? defVal : L"") : it->second);
}
// Symbol: ?WriteString@CWinAppEx@@QEAAHPEB_W0@Z
extern "C" int MS_ABI impl__WriteString_CWinAppEx__QEAAHPEB_W0_Z(CWinAppEx* pThis, const wchar_t* entry, const wchar_t* value) { return pThis ? pThis->WriteString(entry, value) : FALSE; }
// Symbol: ?LoadState@CWinAppEx@@UEAAHPEB_WPEAVCFrameImpl@@@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__UEAAHPEB_WPEAVCFrameImpl___Z(CWinAppEx* pThis, const wchar_t* section, void* frame) { return pThis ? pThis->LoadState(section, frame) : FALSE; }
// Symbol: ?SaveState@CWinAppEx@@UEAAHPEB_WPEAVCFrameImpl@@@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__UEAAHPEB_WPEAVCFrameImpl___Z(CWinAppEx* pThis, const wchar_t* section, void* frame) { return pThis ? pThis->SaveState(section, frame) : FALSE; }
// Symbol: ?CleanState@CWinAppEx@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__CleanState_CWinAppEx__UEAAHPEB_W_Z(CWinAppEx* pThis, const wchar_t* section) { return pThis ? pThis->CleanState(section) : FALSE; }
// Symbol: ?IsStateExists@CWinAppEx@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__IsStateExists_CWinAppEx__QEAAHPEB_W_Z(CWinAppEx* pThis, const wchar_t* section) { return pThis ? pThis->IsStateExists(section) : FALSE; }
// Symbol: ?ExitInstance@CWinAppEx@@UEAAHXZ
extern "C" int MS_ABI impl__ExitInstance_CWinAppEx__UEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->ExitInstance() : 0; }

// Symbol: ?GetRuntimeClass@CWinAppEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWinAppEx__UEBAPEAUCRuntimeClass__XZ(const CWinAppEx*) {
    return &CWinAppEx::classCWinAppEx;
}

// Symbol: ?GetThisClass@CWinAppEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ() {
    return &CWinAppEx::classCWinAppEx;
}

// Symbol: ?GetBinary@CWinAppEx@@QEAAHPEB_WPEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetBinary_CWinAppEx__QEAAHPEB_WPEAPEAEPEAI_Z(
    CWinAppEx* pThis, const wchar_t* entry, unsigned char** data, unsigned int* bytes) {
    if (data) *data = nullptr;
    if (bytes) *bytes = 0;
    if (!pThis || !entry || !data || !bytes) return FALSE;
    auto& binaries = g_winAppExStates[pThis].binaries;
    const std::wstring section = CurrentAppExSection(pThis);
    auto it = binaries.find(AppEntryKey(section.c_str(), entry));
    if (it == binaries.end()) return FALSE;
    const std::vector<unsigned char>& blob = it->second;
    unsigned char* copy = new (std::nothrow) unsigned char[blob.size()];
    if (!copy && !blob.empty()) return FALSE;
    if (!blob.empty()) std::memcpy(copy, blob.data(), blob.size());
    *data = copy;
    *bytes = static_cast<unsigned int>(blob.size());
    return TRUE;
}

// Symbol: ?WriteBinary@CWinAppEx@@QEAAHPEB_WPEAEI@Z
extern "C" int MS_ABI impl__WriteBinary_CWinAppEx__QEAAHPEB_WPEAEI_Z(
    CWinAppEx* pThis, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    const std::wstring section = CurrentAppExSection(pThis);
    g_winAppExStates[pThis].binaries[AppEntryKey(section.c_str(), entry)] = std::move(blob);
    return TRUE;
}

// Symbol: ?GetObjectW@CWinAppEx@@QEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__GetObjectW_CWinAppEx__QEAAHPEB_WAEAVCObject___Z(CWinAppEx* pThis, const wchar_t* entry, CObject*) {
    if (!pThis || !entry) return FALSE;
    const std::wstring section = CurrentAppExSection(pThis);
    return g_winAppExStates[pThis].objects.count(AppEntryKey(section.c_str(), entry)) ? TRUE : FALSE;
}

// Symbol: ?WriteObject@CWinAppEx@@QEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__WriteObject_CWinAppEx__QEAAHPEB_WAEAVCObject___Z(CWinAppEx* pThis, const wchar_t* entry, CObject* object) {
    if (!pThis || !entry || !object) return FALSE;
    const std::wstring section = CurrentAppExSection(pThis);
    g_winAppExStates[pThis].objects.insert(AppEntryKey(section.c_str(), entry));
    return TRUE;
}

// Symbol: ?GetSectionBinary@CWinAppEx@@QEAAHPEB_W0PEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetSectionBinary_CWinAppEx__QEAAHPEB_W0PEAPEAEPEAI_Z(
    CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, unsigned char** data, unsigned int* bytes) {
    if (data) *data = nullptr;
    if (bytes) *bytes = 0;
    if (!pThis || !section || !entry || !data || !bytes) return FALSE;
    auto& binaries = g_winAppExStates[pThis].binaries;
    auto it = binaries.find(AppEntryKey(section, entry));
    if (it == binaries.end()) return FALSE;
    const std::vector<unsigned char>& blob = it->second;
    unsigned char* copy = new (std::nothrow) unsigned char[blob.size()];
    if (!copy && !blob.empty()) return FALSE;
    if (!blob.empty()) std::memcpy(copy, blob.data(), blob.size());
    *data = copy;
    *bytes = static_cast<unsigned int>(blob.size());
    return TRUE;
}

// Symbol: ?WriteSectionBinary@CWinAppEx@@QEAAHPEB_W0PEAEI@Z
extern "C" int MS_ABI impl__WriteSectionBinary_CWinAppEx__QEAAHPEB_W0PEAEI_Z(
    CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !section || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    g_winAppExStates[pThis].binaries[AppEntryKey(section, entry)] = std::move(blob);
    return TRUE;
}

// Symbol: ?GetSectionInt@CWinAppEx@@QEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__GetSectionInt_CWinAppEx__QEAAHPEB_W0H_Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, int defVal) {
    if (!pThis || !section || !entry) return defVal;
    auto& ints = g_winAppExStates[pThis].ints;
    auto it = ints.find(AppEntryKey(section, entry));
    return it == ints.end() ? defVal : it->second;
}

// Symbol: ?WriteSectionInt@CWinAppEx@@QEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__WriteSectionInt_CWinAppEx__QEAAHPEB_W0H_Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, int value) {
    if (!pThis || !section || !entry) return FALSE;
    g_winAppExStates[pThis].ints[AppEntryKey(section, entry)] = value;
    return TRUE;
}

// Symbol: ?WriteSectionString@CWinAppEx@@QEAAHPEB_W00@Z
extern "C" int MS_ABI impl__WriteSectionString_CWinAppEx__QEAAHPEB_W00_Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* value) {
    if (!pThis || !section || !entry) return FALSE;
    g_winAppExStates[pThis].strings[AppEntryKey(section, entry)] = CString(value ? value : L"");
    return TRUE;
}

// Symbol: ?GetSectionObject@CWinAppEx@@QEAAHPEB_W0AEAVCObject@@@Z
extern "C" int MS_ABI impl__GetSectionObject_CWinAppEx__QEAAHPEB_W0AEAVCObject___Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, CObject*) {
    if (!pThis || !section || !entry) return FALSE;
    return g_winAppExStates[pThis].objects.count(AppEntryKey(section, entry)) ? TRUE : FALSE;
}

// Symbol: ?WriteSectionObject@CWinAppEx@@QEAAHPEB_W0AEAVCObject@@@Z
extern "C" int MS_ABI impl__WriteSectionObject_CWinAppEx__QEAAHPEB_W0AEAVCObject___Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, CObject* object) {
    if (!pThis || !section || !entry || !object) return FALSE;
    g_winAppExStates[pThis].objects.insert(AppEntryKey(section, entry));
    return TRUE;
}

// Symbol: ?EnableTearOffMenus@CWinAppEx@@QEAAHPEB_WII@Z
extern "C" int MS_ABI impl__EnableTearOffMenus_CWinAppEx__QEAAHPEB_WII_Z(CWinAppEx* pThis, const wchar_t*, unsigned int, unsigned int) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].tearOffMenus = true;
    return TRUE;
}

// Symbol: ?EnableUserTools@CWinAppEx@@QEAAHIIIPEAUCRuntimeClass@@II@Z
extern "C" int MS_ABI impl__EnableUserTools_CWinAppEx__QEAAHIIIPEAUCRuntimeClass__II_Z(
    CWinAppEx* pThis, unsigned int, unsigned int, unsigned int, CRuntimeClass*, unsigned int, unsigned int) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].userTools = true;
    return TRUE;
}

// Symbol: ?InitMouseManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitMouseManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].mouseManager = true;
    return TRUE;
}

// Symbol: ?GetMouseManager@CWinAppEx@@QEAAPEAVCMouseManager@@XZ
extern "C" CMouseManager* MS_ABI impl__GetMouseManager_CWinAppEx__QEAAPEAVCMouseManager__XZ(CWinAppEx* pThis) {
    if (!pThis) return nullptr;
    impl__InitMouseManager_CWinAppEx__QEAAHXZ(pThis);
    return reinterpret_cast<CMouseManager*>(&g_mouseManagerToken);
}

// Symbol: ?InitShellManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitShellManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].shellManager = true;
    return TRUE;
}

// Symbol: ?GetShellManager@CWinAppEx@@QEAAPEAVCShellManager@@XZ
extern "C" CShellManager* MS_ABI impl__GetShellManager_CWinAppEx__QEAAPEAVCShellManager__XZ(CWinAppEx* pThis) {
    if (!pThis) return nullptr;
    impl__InitShellManager_CWinAppEx__QEAAHXZ(pThis);
    return reinterpret_cast<CShellManager*>(&g_shellManagerToken);
}

// Symbol: ?GetUserToolsManager@CWinAppEx@@QEAAPEAVCUserToolsManager@@XZ
extern "C" CUserToolsManager* MS_ABI impl__GetUserToolsManager_CWinAppEx__QEAAPEAVCUserToolsManager__XZ(CWinAppEx* pThis) {
    if (!pThis) return nullptr;
    g_winAppExStates[pThis].userTools = true;
    return afxUserToolsManager;
}

// Symbol: ?LoadWindowPlacement@CWinAppEx@@MEAAHAEAVCRect@@AEAH1@Z
extern "C" int MS_ABI impl__LoadWindowPlacement_CWinAppEx__MEAAHAEAVCRect__AEAH1_Z(CWinAppEx* pThis, CRect* rect, int* showCmd, int* flags) {
    if (!pThis || !rect || !showCmd || !flags) return FALSE;
    WinAppExState& state = g_winAppExStates[pThis];
    *rect = state.windowRect;
    *showCmd = state.windowShowCmd;
    *flags = state.windowFlags;
    return TRUE;
}

// Symbol: ?StoreWindowPlacement@CWinAppEx@@MEAAHAEBVCRect@@HH@Z
extern "C" int MS_ABI impl__StoreWindowPlacement_CWinAppEx__MEAAHAEBVCRect__HH_Z(CWinAppEx* pThis, const CRect* rect, int showCmd, int flags) {
    if (!pThis || !rect) return FALSE;
    WinAppExState& state = g_winAppExStates[pThis];
    state.windowRect = *rect;
    state.windowShowCmd = showCmd;
    state.windowFlags = flags;
    return TRUE;
}

// Symbol: ?LoadState@CWinAppEx@@QEAAHPEAVCFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__QEAAHPEAVCFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->LoadState(section, nullptr) : FALSE;
}

// Symbol: ?SaveState@CWinAppEx@@QEAAHPEAVCFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__QEAAHPEAVCFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->SaveState(section, nullptr) : FALSE;
}

// Symbol: ?LoadState@CWinAppEx@@QEAAHPEAVCMDIFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__QEAAHPEAVCMDIFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CMDIFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->LoadState(section, nullptr) : FALSE;
}

// Symbol: ?SaveState@CWinAppEx@@QEAAHPEAVCMDIFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__QEAAHPEAVCMDIFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CMDIFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->SaveState(section, nullptr) : FALSE;
}

// Symbol: ?ReloadWindowPlacement@CWinAppEx@@MEAAHPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__ReloadWindowPlacement_CWinAppEx__MEAAHPEAVCFrameWnd___Z(CWinAppEx* pThis, CFrameWnd* frame) {
    if (!pThis || !frame) return FALSE;
    WinAppExState& state = g_winAppExStates[pThis];
    HWND hwnd = frame->GetSafeHwnd();
    if (!hwnd) return TRUE;
    if (!state.windowRect.IsRectNull()) {
        ::MoveWindow(hwnd, state.windowRect.left, state.windowRect.top, state.windowRect.Width(), state.windowRect.Height(), TRUE);
    }
    ::ShowWindow(hwnd, state.windowShowCmd);
    return TRUE;
}

// Symbol: ?OnAppContextHelp@CWinAppEx@@UEAAXPEAVCWnd@@QEBK@Z
extern "C" void MS_ABI impl__OnAppContextHelp_CWinAppEx__UEAAXPEAVCWnd__QEBK_Z(CWinAppEx*, CWnd*, const unsigned long*) {
}

// Symbol: ?OnClosingMainFrame@CWinAppEx@@MEAAXPEAVCFrameImpl@@@Z
extern "C" void MS_ABI impl__OnClosingMainFrame_CWinAppEx__MEAAXPEAVCFrameImpl___Z(CWinAppEx*, void*) {
}

// Symbol: ?OnViewDoubleClick@CWinAppEx@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnViewDoubleClick_CWinAppEx__UEAAHPEAVCWnd__H_Z(CWinAppEx*, CWnd*, int) {
    return FALSE;
}

// Symbol: ?ShowPopupMenu@CWinAppEx@@UEAAHIAEBVCPoint@@PEAVCWnd@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CWinAppEx__UEAAHIAEBVCPoint__PEAVCWnd___Z(CWinAppEx*, unsigned int menuId, const CPoint* point, CWnd* owner) {
    HMENU menu = ::LoadMenuW(AfxGetResourceHandle(), MAKEINTRESOURCEW(menuId));
    if (!menu) return FALSE;
    HMENU popup = ::GetSubMenu(menu, 0);
    BOOL shown = FALSE;
    if (popup && point) {
        shown = ::TrackPopupMenu(popup, TPM_LEFTALIGN | TPM_TOPALIGN, point->x, point->y, 0, owner ? owner->GetSafeHwnd() : nullptr, nullptr);
    }
    ::DestroyMenu(menu);
    return shown ? TRUE : FALSE;
}

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
