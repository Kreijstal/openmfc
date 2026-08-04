#pragma once
// Shared internals of the former mfccore.cpp translation unit.
// Definitions live in detail/MfccoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfccore {} } }
using namespace openmfc::detail::mfccore;
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
#include "detail/DockingState.h"
#include "detail/RibbonState.h"
#include <algorithm>
#include <cwctype>
#include <cstring>
#include <cstdlib>
#include <climits>
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

// CCmdUI export helpers (defined in olecore.cpp).
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);
extern "C" void MS_ABI impl__SetCheck_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck);

// CMFCPropertyGridProperty export helpers (defined in thunks.cpp).
extern "C" void MS_ABI impl__SetOriginalValue_CMFCPropertyGridProperty__UEAAXAEBVCOleVariant___Z(
    CMFCPropertyGridProperty* pThis, const COleVariant* varValue);

//=============================================================================
// CMFCVisualManager - Base visual manager
//=============================================================================

namespace openmfc { namespace detail { namespace mfccore {
extern CMFCVisualManager* g_pVisualManager;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern CRuntimeClass* g_pDefaultVisualManagerClass;
} } }

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


namespace openmfc { namespace detail { namespace mfccore {
using openmfc::ribbon_state::RibbonBarState;
} } }
namespace openmfc { namespace detail { namespace mfccore {
using openmfc::ribbon_state::RibbonCategoryState;
} } }
namespace openmfc { namespace detail { namespace mfccore {
using openmfc::ribbon_state::RibbonPanelState;
} } }
namespace openmfc { namespace detail { namespace mfccore {
using openmfc::docking_state::PaneCoreState;
} } }

namespace openmfc { namespace detail { namespace mfccore {
extern decltype(openmfc::docking_state::PaneCoreStateMutex()) g_paneCoreStateMutex_Mfccore;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern decltype(openmfc::docking_state::PaneCoreStates()) g_paneCoreState_Mfccore;
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct DockingManagerSnapshot {
    std::vector<UINT> ids;
    std::vector<CRect> rects;
    std::vector<BOOL> hidden;
    std::vector<BOOL> floating;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CDockingManager*, DockingManagerState> g_dockingStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern std::mutex g_dockingProfileMutex;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern std::unordered_map<const CDockingManager*,
                          std::unordered_map<std::wstring, DockingManagerSnapshot>>
    g_dockingProfileStates;
} } }

namespace openmfc { namespace detail { namespace mfccore {
DockingManagerState& EnsureDockingState(const CDockingManager* pManager);
} } }

namespace openmfc { namespace detail { namespace mfccore {
const DockingManagerState* FindDockingState(const CDockingManager* pManager);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void RemoveDockingState(const CDockingManager* pManager);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring DockingProfileName(const wchar_t* section);
} } }

namespace openmfc { namespace detail { namespace mfccore {
DockingManagerSnapshot* FindDockingProfileSnapshot(const CDockingManager* pManager, const wchar_t* section);
} } }

namespace openmfc { namespace detail { namespace mfccore {
template <typename T>
bool ContainsPtr(const std::vector<T*>& values, T* value) {
    return std::find(values.begin(), values.end(), value) != values.end();
}
} } }

namespace openmfc { namespace detail { namespace mfccore {
void AddDockingPane(CDockingManager* pManager, CBasePane* pPane, CBasePane* pInsertAfter = nullptr);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void RemoveDockingPane(CDockingManager* pManager, CBasePane* pPane);
} } }

namespace openmfc { namespace detail { namespace mfccore {
bool IsPaneVisibleForDocking(const DockingManagerState& state, CBasePane* pPane);
} } }

namespace openmfc { namespace detail { namespace mfccore {
int MiniFrameZRank(HWND hWnd);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CRect DefaultMiniFrameRect(int index);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CBasePane* FirstDockingPane(const CDockingManager* pManager, bool visibleOnly = false);
} } }

namespace openmfc { namespace detail { namespace mfccore {
UINT PaneCommandID(const CBasePane* pPane);
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct MenuBarState {
    CWnd* parent = nullptr;
    HMENU menu = nullptr;
    std::vector<CMFCToolBarButton*> items;
    std::unordered_set<CMFCToolBarButton*> ownedItems;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct ToolBarState {
    CWnd* parent = nullptr;
    DWORD dockStyle = 0;
    CSize buttonSize = CSize(23, 22);
    CSize imageSize = CSize(16, 15);
    std::vector<CMFCToolBarButton*> buttons;
    std::unordered_set<CMFCToolBarButton*> ownedButtons;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct PopupMenuBarState {
    HMENU menu = nullptr;
    std::vector<CMFCToolBarMenuButton*> items;
    std::unordered_set<CMFCToolBarMenuButton*> ownedItems;
    int selected = -1;
    bool delayedSubMenuOpen = false;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct MenuButtonState {
    bool hasDropDownArrow = false;
    bool userButton = false;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct ContextMenuState {
    std::unordered_map<UINT, HMENU> menusById;
    std::unordered_map<UINT, HMENU> ownedMenusById;
    std::unordered_map<std::wstring, UINT> idsByName;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct TooltipManagerState {
    UINT types = 0;
    CRuntimeClass* runtimeClass = nullptr;
    CMFCToolTipInfo* params = nullptr;
    std::vector<CToolTipCtrl*> tooltips;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct TabEntry {
    CWnd* window = nullptr;
    std::wstring label;
    UINT id = static_cast<UINT>(-1);
    BOOL detachable = TRUE;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct TabCtrlState {
    std::vector<TabEntry> tabs;
    int activeTab = -1;
    BOOL tabSwapEnabled = TRUE;
    int tabBorderSize = 0;
    int tabsHeight = 0;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct MenuHashState {
    std::unordered_map<HMENU, CMFCToolBar*> barsByMenu;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCPopupMenu*, PopupMenuState> g_popupMenuStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCMenuBar*, MenuBarState> g_menuBarStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCToolBar*, ToolBarState> g_toolBarStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern decltype(openmfc::ribbon_state::RibbonPanelStates()) g_ribbonPanelStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern decltype(openmfc::ribbon_state::RibbonCategoryStates()) g_ribbonCategoryStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern decltype(openmfc::ribbon_state::RibbonBarStates()) g_ribbonBarStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCPopupMenuBar*, PopupMenuBarState> g_popupMenuBarStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCToolBarMenuButton*, MenuButtonState> g_menuButtonStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CContextMenuManager*, ContextMenuState> g_contextMenuStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CTooltipManager*, TooltipManagerState> g_tooltipManagerStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCButton*, MFCButtonState> g_mfcButtonStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMenuHash*, MenuHashState> g_menuHashStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCBaseTabCtrl*, TabCtrlState> g_tabCtrlStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern std::unordered_map<const CWinAppEx*, WinAppExState> g_winAppExStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern int g_mouseManagerToken;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern int g_shellManagerToken;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern CUserToolsManager g_userToolsManager;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern BOOL g_forceMenuFocus;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern BOOL g_showAllAccelerators;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern CUserToolsManager* afxUserToolsManager;
} } }
extern unsigned char g_menuFontStorage[sizeof(CFont)];
namespace openmfc { namespace detail { namespace mfccore {
extern CFont* g_menuFontPtr;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern COLORREF g_visualAccentColor;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern COLORREF g_visualBackgroundColor;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern COLORREF g_visualBorderColor;
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearPopupMenuState(const CMFCPopupMenu* pMenu);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearMenuBarState(const CMFCMenuBar* pMenuBar);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearToolBarButtons(ToolBarState& state);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearToolBarState(const CMFCToolBar* pToolBar);
} } }

namespace openmfc { namespace detail { namespace mfccore {
MFCButtonState& EnsureMFCButtonState(const CMFCButton* pButton);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void RemoveMFCButtonState(const CMFCButton* pButton);
} } }

namespace openmfc { namespace detail { namespace mfccore {
TabCtrlState& EnsureTabCtrlState(const CMFCBaseTabCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
const TabCtrlState* FindTabCtrlState(const CMFCBaseTabCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void RemoveTabCtrlState(const CMFCBaseTabCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearRibbonPanelState(const CMFCRibbonPanel* pPanel);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearRibbonCategoryState(const CMFCRibbonCategory* pCategory);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearRibbonBarState(const CMFCRibbonBar* pBar);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void CopyToolBarButtonBaseFields(CMFCToolBarButton* dst, const CMFCToolBarButton& src);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void CopyMenuButtonState(CMFCToolBarMenuButton* dst, const CMFCToolBarMenuButton* src);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CMFCToolBarButton* CloneToolBarButton(const CMFCToolBarButton& button);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ClearPopupMenuBarState(const CMFCPopupMenuBar* pMenuBar);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring CurrentAppExSection(CWinAppEx* app);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CString GetMenuItemText(HMENU hMenu, int index);
} } }

namespace openmfc { namespace detail { namespace mfccore {
UINT GetMenuItemCommand(HMENU hMenu, int index);
} } }

namespace openmfc { namespace detail { namespace mfccore {
HMENU GetMenuItemSubMenu(HMENU hMenu, int index);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CMFCToolBarMenuButton* CreateMenuButtonFromMenu(HMENU hMenu, int index);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ImportMenuIntoPopupItems(HMENU hMenu, std::vector<CMFCToolBarMenuButton*>& items,
                              std::unordered_set<CMFCToolBarMenuButton*>& ownedItems);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CFont& EnsureMenuFont_Mfccore();
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring WideKey(const wchar_t* value);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring AppEntryKey(const wchar_t* section, const wchar_t* entry);
} } }

namespace openmfc { namespace detail { namespace mfccore {
HMENU LoadMenuResource(UINT uiMenuResId, HMENU* pOwnedMenu);
} } }

namespace openmfc { namespace detail { namespace mfccore {
HDC SafeHdc(CDC* pDC);
} } }

namespace openmfc { namespace detail { namespace mfccore {
RECT ToRECT(const CRect& rect);
} } }

namespace openmfc { namespace detail { namespace mfccore {
bool IsDrawableRect(const CRect& rect);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void FillSolid(CDC* pDC, const CRect& rect, COLORREF color);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void FrameSolid(CDC* pDC, const CRect& rect, COLORREF color);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void Draw3dFrame(CDC* pDC, const CRect& rect, COLORREF light, COLORREF dark);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void FillAndFrame(CDC* pDC, const CRect& rect, COLORREF fill, COLORREF border);
} } }

namespace openmfc { namespace detail { namespace mfccore {
COLORREF ButtonFillForState(CMFCVisualManager::AFX_BUTTON_STATE state);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void DrawArrowGlyph(CDC* pDC, CRect rect, bool down, COLORREF color);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void DrawPlusMinus(CDC* pDC, CRect rect, bool minusOnly, COLORREF color);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void DrawRectText(CDC* pDC, CRect rect, const wchar_t* text, COLORREF color);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void DestroyContextMenus(ContextMenuState& state);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void SyncVisualManagerExports();
} } }

namespace openmfc { namespace detail { namespace mfccore {
void SetVisualPalette(COLORREF accent, COLORREF background, COLORREF border);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ApplyVisualPaletteForClass(const CRuntimeClass* pClass);
} } }


namespace openmfc { namespace detail { namespace mfccore {
CMFCVisualManager* CreateVisualManagerFromRuntimeClass(CRuntimeClass* pRTI);
} } }

























// Office variant visual managers




















//=============================================================================
// CBasePane
//=============================================================================



//=============================================================================
// CPane
//=============================================================================






//=============================================================================
// CDockablePane
//=============================================================================


// Old convenience overloads (non-MSDN API)

//=============================================================================
// CBaseTabbedPane
//=============================================================================




//=============================================================================
// CTabbedPane
//=============================================================================



//=============================================================================
// CUserTool
//=============================================================================




// Draws the tool's icon centred inside rectImage. Retail (mfc140u
// ?DrawToolIcon@CUserTool@@) computes
//     x = left + (width  - iconSize) / 2
//     y = top  + (height - iconSize) / 2
// clamping each offset at 0 (cmovns), then calls DrawIconEx(..., DI_NORMAL).
// Retail reads the icon extent from its cached afxGlobalData small-icon size;
// we query the same value from the system directly.



//=============================================================================
// CUserToolsManager
//=============================================================================






//=============================================================================
// CMFCToolBarButton
//=============================================================================

// Field defaults match real mfc140u's CMFCToolBarButton::Initialize() exactly
// (harvested via tools/harvest/family). NOTE the non-obvious ones:
// m_iImage = m_iUserImage = -1, and m_bImage / m_bWholeText / m_bHorz / m_bVisible = TRUE.
namespace openmfc { namespace detail { namespace mfccore {
void OpenMFC_InitToolBarButton(CMFCToolBarButton* b);
} } }




//=============================================================================
// CMFCToolBar
//=============================================================================
// Retail base chain is CMFCToolBar : CMFCBaseToolBar : CPane : CBasePane.














//=============================================================================
// CMFCMenuBar
//=============================================================================







namespace openmfc { namespace detail { namespace mfccore {
CSize CalcMenuBarFixedLayout(const CMFCMenuBar* pMenuBar);
} } }




















//=============================================================================
// CMFCRibbonBaseElement
//=============================================================================



//=============================================================================
// CMFCRibbonButton
//=============================================================================


//=============================================================================
// CMFCRibbonPanel
//=============================================================================



//=============================================================================
// CMFCRibbonCategory
//=============================================================================


// Old convenience overloads (non-MSDN API)

//=============================================================================
// CMFCRibbonBar
//=============================================================================

// Retail layout, pinned. Transcribed from /d1reportSingleClassLayout and then
// confirmed against the shipping constructor's own stores: it writes the ctor
// argument to 0x468 (m_bReplaceFrameCaption), constructs a subobject at 0x498
// (m_TabElements), and initializes CArray vtables at 0xb28/0xb50/0xb78/0xba0
// (the four arrays, 40 bytes apart) -- all exactly where this layout puts them.


// Old convenience overloads (non-MSDN API)

//=============================================================================
// CMiniFrameWnd
//=============================================================================


//=============================================================================
// CMFCButton
//=============================================================================



//=============================================================================
// CMFCPopupMenu
//=============================================================================

namespace openmfc { namespace detail { namespace mfccore {
extern CMFCPopupMenu* g_pActivePopupMenu;
} } }


























//=============================================================================
// CMFCPopupMenuBar
//=============================================================================


















//=============================================================================
// CMFCBaseTabCtrl
//=============================================================================









//=============================================================================
// CMFCTabCtrl
//=============================================================================






namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

#ifndef CHILDID_SELF
#define CHILDID_SELF 0
#endif

#ifndef ROLE_SYSTEM_OUTLINE
#define ROLE_SYSTEM_OUTLINE 0x23
#endif

#ifndef ROLE_SYSTEM_OUTLINEITEM
#define ROLE_SYSTEM_OUTLINEITEM 0x24
#endif

#ifndef ROLE_SYSTEM_PROGRESSBAR
#define ROLE_SYSTEM_PROGRESSBAR 0x30
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

namespace openmfc { namespace detail { namespace mfccore {
struct TasksPaneTaskState {
    UINT id = 0;
    std::wstring label;
    int icon = -1;
    BOOL isSeparator = FALSE;
    UINT commandID = 0;
    unsigned __int64 userData = 0;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
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
} } }

namespace openmfc { namespace detail { namespace mfccore {
struct TasksPaneState {
    std::wstring caption;
    std::vector<TasksPaneGroupState> groups;
};
} } }

namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCPropertyGridProperty*, PropertyGridPropertyState> g_propertyGridPropertyStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCPropertyGridCtrl*, PropertyGridCtrlState> g_propertyGridCtrlStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCTasksPaneTask*, TasksPaneTaskState> g_tasksPaneTaskStates;
} } }
namespace openmfc { namespace detail { namespace mfccore {
extern thread_local std::unordered_map<const CMFCTasksPane*, TasksPaneState> g_tasksPaneStates;
} } }

namespace openmfc { namespace detail { namespace mfccore {
PropertyGridPropertyState& EnsurePropertyGridPropertyState(const CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
const PropertyGridPropertyState* FindPropertyGridPropertyState(const CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
PropertyGridPropertyState* FindMutablePropertyGridPropertyState(const CMFCPropertyGridProperty* pProp);
} } }

void SetPropertyGridOwnerRecursive(CMFCPropertyGridProperty* pProp, CMFCPropertyGridCtrl* pOwner);

namespace openmfc { namespace detail { namespace mfccore {
void DeletePropertyGridChildren(CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void RemovePropertyGridPropertyReferences(const CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
PropertyGridCtrlState& EnsurePropertyGridCtrlState(const CMFCPropertyGridCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
const PropertyGridCtrlState* FindPropertyGridCtrlState(const CMFCPropertyGridCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
PropertyGridCtrlState* FindMutablePropertyGridCtrlState(const CMFCPropertyGridCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void TouchPropertyGridCtrl(CMFCPropertyGridCtrl* pCtrl, BOOL bAdjustLayout);
} } }

void SetPropertyGridOwnerRecursive(CMFCPropertyGridProperty* pProp, CMFCPropertyGridCtrl* pOwner);

namespace openmfc { namespace detail { namespace mfccore {
void DetachPropertyFromParent(CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void DetachPropertyFromGrid(CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
BOOL IsPropertyGridAncestorOf(const CMFCPropertyGridProperty* pAncestor,
                              const CMFCPropertyGridProperty* pDescendant);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring JoinPropertyGridArrayParts(const std::vector<std::wstring>& parts, const PropertyGridCtrlState* ctrlState);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring PropertyGridArrayToString(const VARIANT& value, const PropertyGridCtrlState* ctrlState);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::wstring PropertyGridVariantToString(const VARIANT& value, const PropertyGridCtrlState* ctrlState = nullptr);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void AssignPropertyGridVariant(COleVariant& target, const VARIANT& source);
} } }

namespace openmfc { namespace detail { namespace mfccore {
BOOL PropertyGridValuesEqual(const VARIANT& lhs, const VARIANT& rhs);
} } }

namespace openmfc { namespace detail { namespace mfccore {
int CountExpandedPropertyGridSubItems(const CMFCPropertyGridProperty* pProp, BOOL bIncludeHidden);
} } }

namespace openmfc { namespace detail { namespace mfccore {
int CountPropertyGridItems(const CMFCPropertyGridProperty* pProp, BOOL bIncludeHidden);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void AppendVisiblePropertyGridItems(CMFCPropertyGridProperty* pProp,
                                    std::vector<CMFCPropertyGridProperty*>& items);
} } }

namespace openmfc { namespace detail { namespace mfccore {
std::vector<CMFCPropertyGridProperty*> VisiblePropertyGridItems(const CMFCPropertyGridCtrl* pCtrl);
} } }

namespace openmfc { namespace detail { namespace mfccore {
int PropertyGridChildIDFromVariant(const VARIANT& varChild);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CMFCPropertyGridProperty* PropertyGridPropertyFromChildID(CMFCPropertyGridCtrl* pCtrl, int childID);
} } }

namespace openmfc { namespace detail { namespace mfccore {
int PropertyGridChildIDFromProperty(CMFCPropertyGridCtrl* pCtrl, CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void SetPropertyGridVariantChild(VARIANT* pvarChild, int childID);
} } }

namespace openmfc { namespace detail { namespace mfccore {
HRESULT AllocPropertyGridString(const std::wstring& text, BSTR* ppszValue);
} } }

namespace openmfc { namespace detail { namespace mfccore {
RECT NativeRect(const CRect& rect);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CRect PropertyGridItemRect(CMFCPropertyGridCtrl* pCtrl, int childID);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CMFCPropertyGridProperty* PropertyGridHitTestByPoint(CMFCPropertyGridCtrl* pCtrl, CPoint point);
} } }

namespace openmfc { namespace detail { namespace mfccore {
CMFCPropertyGridProperty* FindPropertyGridSubItemByData(CMFCPropertyGridProperty* pProp, DWORD_PTR dwData);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ResetPropertyGridOriginalValueRecursive(CMFCPropertyGridProperty* pProp);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void MarkPropertyGridModifiedRecursive(CMFCPropertyGridProperty* pProp, BOOL bModified);
} } }

namespace openmfc { namespace detail { namespace mfccore {
template <typename Compare>
void SortPropertyGridSubItemsRecursive(CMFCPropertyGridProperty* pProp, Compare compare) {
    PropertyGridPropertyState* state = FindMutablePropertyGridPropertyState(pProp);
    if (!state) return;
    std::stable_sort(state->subItems.begin(), state->subItems.end(), compare);
    for (CMFCPropertyGridProperty* child : state->subItems) {
        SortPropertyGridSubItemsRecursive(child, compare);
    }
}
} } }

namespace openmfc { namespace detail { namespace mfccore {
TasksPaneTaskState& EnsureTasksPaneTaskState(const CMFCTasksPaneTask* pTask);
} } }

namespace openmfc { namespace detail { namespace mfccore {
TasksPaneState& EnsureTasksPaneState(const CMFCTasksPane* pPane);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void RemoveTaskFromAllGroups(const CMFCTasksPaneTask* pTask);
} } }

namespace openmfc { namespace detail { namespace mfccore {
TasksPaneGroupState& EnsureTasksPaneGroup(TasksPaneState& state, int nGroup);
} } }

namespace openmfc { namespace detail { namespace mfccore {
void ExpandPropertyRecursive(CMFCPropertyGridProperty* pProp, BOOL bExpand);
} } }


//=============================================================================
// CMFCPropertyGridProperty
//=============================================================================




//=============================================================================
// CMFCPropertyGridCtrl
//=============================================================================
























































//=============================================================================
// CMFCTasksPaneTask
//=============================================================================


//=============================================================================
// CMFCTasksPane
//=============================================================================


// Old convenience overloads (non-MSDN API)

//=============================================================================
// CPaneFrameWnd
//=============================================================================


extern "C" void MS_ABI impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane);
extern "C" void MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int, int);

//=============================================================================
// CDockingManager
//=============================================================================


















extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
    const CDockingManager* pThis, CPoint point, int bIncludeHidden, int bCheckForPoint,
    CRuntimeClass* pRTCFilter, int, const CDockingManager* pDockManager);
extern "C" void MS_ABI impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(CDockingManager* pThis);































//=============================================================================
// CFrameWndEx
//=============================================================================



//=============================================================================
// CMDIFrameWndEx
//=============================================================================



//=============================================================================
// CMDIChildWndEx
//=============================================================================


//=============================================================================
// Forward type stubs (DECLARE_DYNAMIC classes defined inline in afxmfc.h)
//=============================================================================




















//=============================================================================
// Feature Pack managers/settings helpers
//=============================================================================


















//=============================================================================
// CShellManager
//=============================================================================


namespace openmfc { namespace detail { namespace mfccore {
template <typename Fn>
Fn LoadShell32Function(const char* name) {
    HMODULE shell32 = ::GetModuleHandleW(L"shell32.dll");
    if (!shell32) shell32 = ::LoadLibraryW(L"shell32.dll");
    return shell32 ? reinterpret_cast<Fn>(::GetProcAddress(shell32, name)) : nullptr;
}
} } }

namespace openmfc { namespace detail { namespace mfccore {
LPITEMIDLIST AllocatePidlBytes(UINT bytes);
} } }









































































// Virtuals required by the vtable (MFC-default no-ops; the export impls in
// this file carry the same behavior).






// =============================================================================
// CMFCVisualManager::OnDrawPropertySheetListItem virtuals (ords 9490-9492)
// These are stubbed because CMFCPropertySheet is not declared in our headers.
// MFC default returns FALSE (did not handle — let the property sheet do default
// drawing).
// =============================================================================


namespace openmfc { namespace detail { namespace mfccore {
unsigned long MS_ABI DrawPropertySheetListItemFallback(
    void* pThis, CDC* pDC, const CRect& rect, int nItem, int nState);
} } }





//=============================================================================
// CMFCTasksPane OnUpdate* command-UI handlers
//=============================================================================




//=============================================================================
// Frame Window Ex OnUpdate* handlers
//=============================================================================



//=============================================================================
// CMFCOutlookBarTabCtrl OnUpdate* handler (no header class yet)
//=============================================================================


//=============================================================================
// CMFCImageEditorDialog OnUpdate* handlers (no header class yet)
//=============================================================================








//=== CMFCBaseVisualManager / CMFCVisualManager derived progress exports =======







//=== CAnimationController export ==============================================


//=== CMFCPropertyGridProperty ctor exports (ord 640, 641) ====================



//=== CMFCPropertyGridColorProperty::SetOriginalValue ==========================

