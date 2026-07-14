// Toolbar / StatusBar / DialogBar Implementation
// Provides MFC wrappers around Windows common controls
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "docking_state.h"
#include "ribbon_state.h"
#include <commctrl.h>
#include <algorithm>
#include <cstring>
#include <memory>
#include <string>
#include <mutex>
#include <new>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// MinGW compat
#ifndef RT_TOOLBAR
#define RT_TOOLBAR MAKEINTRESOURCE(241)
#endif
#ifndef ID_SEPARATOR
#define ID_SEPARATOR 0
#endif

namespace {
using openmfc::ribbon_state::RibbonBarState;
using openmfc::ribbon_state::RibbonCategoryState;
using openmfc::ribbon_state::RibbonPanelState;
using openmfc::docking_state::PaneCoreState;

auto& g_ribbonMutex = openmfc::ribbon_state::RibbonMutex();
auto& g_ribbonPanels = openmfc::ribbon_state::RibbonPanelStates();
auto& g_ribbonCategories = openmfc::ribbon_state::RibbonCategoryStates();
auto& g_ribbonBars = openmfc::ribbon_state::RibbonBarStates();
constexpr int kApproxRibbonCharPx = 6;

std::unordered_map<UINT, std::wstring> g_ribbonToolTips;
std::unordered_map<UINT, std::wstring> g_ribbonDescriptions;
std::unordered_map<UINT, int> g_galleryLastSelectedByID;
std::unordered_map<const CMFCRibbonBaseElement*, bool> g_ribbonElementsEnabled;

struct RibbonSliderState {
    int nMin = 0;
    int nMax = 100;
    int nPos = 0;
    bool bShowZoomButtons = false;
    int nZoomIncrement = 1;
};
std::unordered_map<UINT, RibbonSliderState> g_sliderStates;

struct RibbonProgressState {
    int nMin = 0;
    int nMax = 100;
    int nPos = 0;
    bool bInfinite = false;
};
std::unordered_map<UINT, RibbonProgressState> g_progressStates;

struct RibbonStatusPaneState {
    bool bAnimating = false;
    UINT nAnimationFrame = 0;
    UINT nAnimationCount = 0;
    DWORD dwAnimationDelay = 0;
    HBITMAP hAnimationBitmap = nullptr;
    int cxAnimation = 0;
};
std::unordered_map<UINT, RibbonStatusPaneState> g_statusPaneStates;

inline UINT RibbonElementID(const CMFCRibbonBaseElement* pElem) {
    return (pElem != nullptr) ? pElem->GetID() : 0;
}

inline void NormalizeRange(int nMin, int nMax, int& outMin, int& outMax) {
    outMin = std::min(nMin, nMax);
    outMax = std::max(nMin, nMax);
}

inline int RibbonTextPixels(const CMFCRibbonBaseElement* pElem) {
    if (!pElem) return 0;
    return std::max(0, pElem->GetText().GetLength()) * kApproxRibbonCharPx;
}

inline void BuildCStringResult(void* pRet, const std::wstring& value) {
    if (!pRet) return;
    new(pRet) CString(value.c_str());
}

inline void BuildCSizeResult(void* pRet, int cx, int cy) {
    if (!pRet) return;
    new(pRet) CSize(cx, cy);
}

inline COLORREF SystemColor(int index) {
    return ::GetSysColor(index);
}

inline COLORREF FeaturePackBackgroundColor() {
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    if (manager != nullptr) {
        return manager->GetHighlightedColor(COLOR_3DFACE);
    }
    return SystemColor(COLOR_3DFACE);
}

} // namespace

// Symbol: ??0CMFCRibbonBar@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonBar__QEAA_H_Z(void* pThis, int bReplaceFrameCaption) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    CMFCRibbonBar* bar = new(pThis) CMFCRibbonBar(bReplaceFrameCaption);
    g_ribbonBars[bar] = RibbonBarState{};
    return bar;
}

// Symbol: ??0CMFCRibbonCategory@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonCategory__IEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    CMFCRibbonCategory* category = new(pThis) CMFCRibbonCategory();
    g_ribbonCategories[category] = RibbonCategoryState{};
    return category;
}

// Symbol: ??1CMFCRibbonPanel@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonPanel__UEAA_XZ(void* pThis) {
    auto* panel = reinterpret_cast<CMFCRibbonPanel*>(pThis);
    if (!panel) return;

    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        auto panelIt = g_ribbonPanels.find(panel);
        if (panelIt != g_ribbonPanels.end()) {
            std::vector<CMFCRibbonBaseElement*> ownedElements(panelIt->second.ownedElements.begin(), panelIt->second.ownedElements.end());
            g_ribbonPanels.erase(panelIt);
            for (CMFCRibbonBaseElement* element : ownedElements) {
                delete element;
            }
        }
        for (auto& [_, categoryState] : g_ribbonCategories) {
            categoryState.ownedPanels.erase(panel);
            auto& panels = categoryState.panels;
            panels.erase(std::remove(panels.begin(), panels.end(), panel), panels.end());
        }
    }

    panel->~CMFCRibbonPanel();
}

// Symbol: ??1CMFCRibbonCategory@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonCategory__UEAA_XZ(void* pThis) {
    auto* category = reinterpret_cast<CMFCRibbonCategory*>(pThis);
    if (!category) return;

    std::vector<CMFCRibbonPanel*> ownedPanels;
    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        if (auto it = g_ribbonCategories.find(category); it != g_ribbonCategories.end()) {
            ownedPanels.assign(it->second.ownedPanels.begin(), it->second.ownedPanels.end());
            g_ribbonCategories.erase(it);
        }
        for (auto& [_, barState] : g_ribbonBars) {
            if (barState.activeCategory == category) {
                barState.activeCategory = nullptr;
            }
            barState.ownedCategories.erase(category);
            auto& categories = barState.categories;
            categories.erase(std::remove(categories.begin(), categories.end(), category), categories.end());
        }
    }

    for (CMFCRibbonPanel* panel : ownedPanels) {
        if (panel) {
            impl___1CMFCRibbonPanel__UEAA_XZ(panel);
            ::operator delete(panel);
        }
    }

    category->~CMFCRibbonCategory();
}

// Symbol: ??1CMFCRibbonBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonBar__UEAA_XZ(void* pThis) {
    auto* bar = reinterpret_cast<CMFCRibbonBar*>(pThis);
    if (!bar) return;

    std::vector<CMFCRibbonCategory*> ownedCategories;
    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        if (auto it = g_ribbonBars.find(bar); it != g_ribbonBars.end()) {
            ownedCategories.assign(it->second.ownedCategories.begin(), it->second.ownedCategories.end());
            g_ribbonBars.erase(it);
        }
    }

    for (CMFCRibbonCategory* category : ownedCategories) {
        if (category) {
            impl___1CMFCRibbonCategory__UEAA_XZ(category);
            ::operator delete(category);
        }
    }

    bar->~CMFCRibbonBar();
}

// Symbol: ?Add@CMFCRibbonPanel@@UEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__Add_CMFCRibbonPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* pThis, CMFCRibbonBaseElement* pElement) {
    if (!pThis || !pElement) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonPanels[pThis];
    state.elements.push_back(pElement);
    state.ownedElements.insert(pElement);
}

// Symbol: ?AddSeparator@CMFCRibbonPanel@@UEAAXXZ
extern "C" void MS_ABI impl__AddSeparator_CMFCRibbonPanel__UEAAXXZ(CMFCRibbonPanel* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonPanels[pThis].elements.push_back(nullptr);
}

// Symbol: ?GetCount@CMFCRibbonPanel@@QEBAHXZ
extern "C" int MS_ABI impl__GetCount_CMFCRibbonPanel__QEBAHXZ(const CMFCRibbonPanel* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonPanels.find(const_cast<CMFCRibbonPanel*>(pThis));
    return (it == g_ribbonPanels.end()) ? 0 : static_cast<int>(it->second.elements.size());
}

// Symbol: ?GetElement@CMFCRibbonPanel@@QEBAPEAVCMFCRibbonBaseElement@@H@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetElement_CMFCRibbonPanel__QEBAPEAVCMFCRibbonBaseElement__H_Z(
    const CMFCRibbonPanel* pThis, int nIndex) {
    if (!pThis || nIndex < 0) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonPanels.find(const_cast<CMFCRibbonPanel*>(pThis));
    if (it == g_ribbonPanels.end()) return nullptr;
    const auto& elements = it->second.elements;
    if (nIndex >= static_cast<int>(elements.size())) return nullptr;
    return elements[static_cast<size_t>(nIndex)];
}

// Symbol: ?AddPanel@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@PEB_WPEAUHICON__@@PEAUCRuntimeClass@@@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__AddPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__PEB_WPEAUHICON____PEAUCRuntimeClass___Z(
    CMFCRibbonCategory* pThis, const wchar_t* lpszLabel, HICON__* hIcon, CRuntimeClass*) {
    if (!pThis) return nullptr;

    auto* panel = new CMFCRibbonPanel(lpszLabel, hIcon);
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonCategories[pThis];
    state.panels.push_back(panel);
    state.ownedPanels.insert(panel);
    g_ribbonPanels.try_emplace(panel);
    return panel;
}

// Symbol: ?GetPanel@CMFCRibbonCategory@@QEAAPEAVCMFCRibbonPanel@@H@Z
extern "C" CMFCRibbonPanel* MS_ABI impl__GetPanel_CMFCRibbonCategory__QEAAPEAVCMFCRibbonPanel__H_Z(
    CMFCRibbonCategory* pThis, int nIndex) {
    if (!pThis || nIndex < 0) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonCategories.find(pThis);
    if (it == g_ribbonCategories.end()) return nullptr;
    const auto& panels = it->second.panels;
    if (nIndex >= static_cast<int>(panels.size())) return nullptr;
    return panels[static_cast<size_t>(nIndex)];
}

// Symbol: ?GetPanelCount@CMFCRibbonCategory@@QEBAHXZ
extern "C" int MS_ABI impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(const CMFCRibbonCategory* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonCategories.find(const_cast<CMFCRibbonCategory*>(pThis));
    return (it == g_ribbonCategories.end()) ? 0 : static_cast<int>(it->second.panels.size());
}

// Symbol: ?AddCategory@CMFCRibbonBar@@QEAAPEAVCMFCRibbonCategory@@PEB_WIIVCSize@@1HPEAUCRuntimeClass@@@Z
extern "C" CMFCRibbonCategory* MS_ABI impl__AddCategory_CMFCRibbonBar__QEAAPEAVCMFCRibbonCategory__PEB_WIIVCSize__1HPEAUCRuntimeClass___Z(
    CMFCRibbonBar* pThis, const wchar_t* lpszName, unsigned int uiSmallImage, unsigned int uiLargeImage,
    CSize sizeSmall, unsigned int uiAnimLargeImage, int nInsertAt, CRuntimeClass*) {
    if (!pThis) return nullptr;

    auto* category = new CMFCRibbonCategory(lpszName, uiSmallImage, uiLargeImage);
    (void)sizeSmall;
    (void)uiAnimLargeImage;

    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonBars[pThis];
    auto insertPos = state.categories.end();
    if (nInsertAt >= 0 && nInsertAt < static_cast<int>(state.categories.size())) {
        insertPos = state.categories.begin() + nInsertAt;
    }
    state.categories.insert(insertPos, category);
    state.ownedCategories.insert(category);
    if (state.activeCategory == nullptr) {
        state.activeCategory = category;
    }
    g_ribbonCategories.try_emplace(category);
    return category;
}

// Symbol: ?GetCategory@CMFCRibbonBar@@QEBAPEAVCMFCRibbonCategory@@H@Z
extern "C" CMFCRibbonCategory* MS_ABI impl__GetCategory_CMFCRibbonBar__QEBAPEAVCMFCRibbonCategory__H_Z(
    const CMFCRibbonBar* pThis, int nIndex) {
    if (!pThis || nIndex < 0) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonBars.find(const_cast<CMFCRibbonBar*>(pThis));
    if (it == g_ribbonBars.end()) return nullptr;
    const auto& categories = it->second.categories;
    if (nIndex >= static_cast<int>(categories.size())) return nullptr;
    return categories[static_cast<size_t>(nIndex)];
}

// Symbol: ?GetCategoryCount@CMFCRibbonBar@@QEBAHXZ
extern "C" int MS_ABI impl__GetCategoryCount_CMFCRibbonBar__QEBAHXZ(const CMFCRibbonBar* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonBars.find(const_cast<CMFCRibbonBar*>(pThis));
    return (it == g_ribbonBars.end()) ? 0 : static_cast<int>(it->second.categories.size());
}

// Symbol: ?SetActiveCategory@CMFCRibbonBar@@UEAAHPEAVCMFCRibbonCategory@@H@Z
extern "C" int MS_ABI impl__SetActiveCategory_CMFCRibbonBar__UEAAHPEAVCMFCRibbonCategory__H_Z(
    CMFCRibbonBar* pThis, CMFCRibbonCategory* pCategory, int) {
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonBars[pThis];
    if (pCategory == nullptr) {
        state.activeCategory = nullptr;
        return TRUE;
    }
    auto it = std::find(state.categories.begin(), state.categories.end(), pCategory);
    if (it == state.categories.end()) return FALSE;
    state.activeCategory = pCategory;
    return TRUE;
}

// Symbol: ?AddToTabs@CMFCRibbonBar@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__AddToTabs_CMFCRibbonBar__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar* pThis, CMFCRibbonBaseElement* pElement) {
    if (!pThis || !pElement) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& tabs = g_ribbonBars[pThis].tabs;
    if (std::find(tabs.begin(), tabs.end(), pElement) == tabs.end()) {
        tabs.push_back(pElement);
    }
}

// Symbol: ?SetQuickAccessToolbarOnTop@CMFCRibbonBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetQuickAccessToolbarOnTop_CMFCRibbonBar__QEAAXH_Z(CMFCRibbonBar* pThis, int bOnTop) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonBars[pThis].quickAccessToolbarOnTop = (bOnTop != FALSE);
}

// Symbol: ?ToggleMimimizeState@CMFCRibbonBar@@QEAAXXZ
extern "C" void MS_ABI impl__ToggleMimimizeState_CMFCRibbonBar__QEAAXXZ(CMFCRibbonBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_ribbonBars[pThis];
    state.minimized = !state.minimized;
}

// Symbol: ?CreateObject@CMFCRibbonCategory@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonCategory__SAPEAVCObject__XZ() {
    return CMFCRibbonCategory::GetThisClass()->CreateObject();
}

// Symbol: ?CreateObject@CMFCRibbonPanel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonPanel__SAPEAVCObject__XZ() {
    return CMFCRibbonPanel::GetThisClass()->CreateObject();
}

// Symbol: ?GetRuntimeClass@CMFCRibbonBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonBar__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonBar* pThis) {
    return CMFCRibbonBar::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMFCRibbonCategory@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonCategory__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonCategory* pThis) {
    return CMFCRibbonCategory::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMFCRibbonPanel@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonPanel__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonPanel* pThis) {
    return CMFCRibbonPanel::GetThisClass();
}

// Symbol: ?GetThisClass@CMFCRibbonBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonBar__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonBar::GetThisClass();
}

// Symbol: ?GetThisClass@CMFCRibbonCategory@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonCategory__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonCategory::GetThisClass();
}

// Symbol: ?GetThisClass@CMFCRibbonPanel@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanel__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonPanel::GetThisClass();
}

//=============================================================================
// CToolTipCtrl
//=============================================================================
BOOL CToolTipCtrl::Create(CWnd* pParentWnd, DWORD dwStyle) {
    if (!pParentWnd) return FALSE;
    m_hWnd = ::CreateWindowExW(0, TOOLTIPS_CLASSW, nullptr,
                                dwStyle | WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                pParentWnd->GetSafeHwnd(), nullptr,
                                AfxGetInstanceHandle(), nullptr);
    if (m_hWnd) {
        ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
                       SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    return m_hWnd != nullptr;
}

BOOL CToolTipCtrl::AddTool(CWnd* pWnd, const wchar_t* pszText, LPCRECT lpRectTool, UINT_PTR nIDTool) {
    if (!m_hWnd || !pWnd) return FALSE;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd->GetSafeHwnd();
    ti.uId = nIDTool ? nIDTool : (UINT_PTR)pWnd->GetSafeHwnd();
    ti.lpszText = (LPWSTR)pszText;
    ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
    if (lpRectTool) ti.rect = *lpRectTool;
    return ::SendMessageW(m_hWnd, TTM_ADDTOOLW, 0, (LPARAM)&ti) ? TRUE : FALSE;
}

void CToolTipCtrl::UpdateTipText(const wchar_t* pszText, CWnd* pWnd, UINT_PTR nIDTool) {
    if (!m_hWnd) return;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    ti.uId = nIDTool;
    ti.lpszText = (LPWSTR)pszText;
    ::SendMessageW(m_hWnd, TTM_UPDATETIPTEXTW, 0, (LPARAM)&ti);
}

void CToolTipCtrl::Activate(BOOL bActivate) {
    if (m_hWnd) ::SendMessageW(m_hWnd, TTM_ACTIVATE, (WPARAM)bActivate, 0);
}

void CToolTipCtrl::SetMaxTipWidth(int iWidth) {
    if (m_hWnd) ::SendMessageW(m_hWnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)iWidth);
}

int CToolTipCtrl::GetText(CWnd* pWnd, UINT_PTR nIDTool, wchar_t* pszText, int cchMax) const {
    if (!m_hWnd) return 0;
    TOOLINFOW ti = {};
    ti.cbSize = sizeof(TOOLINFOW);
    ti.hwnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;
    ti.uId = nIDTool;
    ti.lpszText = pszText;
    return (int)::SendMessageW(m_hWnd, TTM_GETTEXTW, 0, (LPARAM)&ti);
}

//=============================================================================
// CToolBar
//=============================================================================
IMPLEMENT_DYNAMIC(CToolBar, CControlBar)

CToolBar::CToolBar()
    : m_hRsrcImageWell(nullptr), m_hInstImageWell(nullptr), m_hbmImageWell(nullptr),
      m_bDelayedButtonLayout(FALSE), m_pStringMap(nullptr) {
    // m_nCount and the docking members are inherited from CControlBar (ctor).
    m_sizeButton.cx = 23;
    m_sizeButton.cy = 22;
    m_sizeImage.cx = 16;
    m_sizeImage.cy = 15;
}

CToolBar::~CToolBar() {
    if (m_hbmImageWell) {
        ::DeleteObject(m_hbmImageWell);
        m_hbmImageWell = nullptr;
    }
}

BOOL CToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    return CreateEx(pParentWnd, TBSTYLE_FLAT, dwStyle, CRect(0,0,0,0), nID);
}

BOOL CToolBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle,
                         CRect rcBorders, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;

    DWORD dwWinStyle = dwStyle & 0xFFFF;
    dwWinStyle |= WS_CHILD | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER;

    m_hWnd = ::CreateWindowExW(0, TOOLBARCLASSNAMEW, nullptr, dwWinStyle,
                                rcBorders.left, rcBorders.top,
                                rcBorders.right - rcBorders.left,
                                rcBorders.bottom - rcBorders.top,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);

    if (!m_hWnd) return FALSE;

    ::SendMessageW(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwCtrlStyle);
    ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(m_sizeImage.cx, m_sizeImage.cy));
    ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(m_sizeButton.cx, m_sizeButton.cy));

    return TRUE;
}

BOOL CToolBar::LoadToolBar(UINT nIDResource) {
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCEW(nIDResource), RT_TOOLBAR);
    if (!hRsrc) return FALSE;

    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;

    WORD* pData = (WORD*)::LockResource(hGlobal);
    if (!pData) return FALSE;

    WORD wVersion = pData[0];
    WORD wCount = pData[1];
    WORD wWidth = pData[2];
    WORD wHeight = pData[3];
    (void)wVersion; (void)wWidth; (void)wHeight;

    WORD* pButtonIDs = pData + 4;

    for (WORD i = 0; i < wCount; ++i) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = pButtonIDs[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (pButtonIDs[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, (LPARAM)&tb);
    }

    m_nCount = wCount;
    return TRUE;
}

BOOL CToolBar::LoadToolBar(const wchar_t* lpszResourceName) {
    if (!lpszResourceName) return FALSE;
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(), lpszResourceName, RT_TOOLBAR);
    if (!hRsrc) return FALSE;
    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;
    WORD* pData = (WORD*)::LockResource(hGlobal);
    if (!pData) return FALSE;

    WORD wCount = pData[1];
    WORD* pButtonIDs = pData + 4;

    for (int i = 0; i < wCount; i++) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = pButtonIDs[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (pButtonIDs[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, (LPARAM)&tb);
    }

    m_nCount = wCount;
    return TRUE;
}

BOOL CToolBar::LoadBitmap(UINT nIDResource) {
    HBITMAP hbm = ::LoadBitmapW(AfxGetInstanceHandle(), MAKEINTRESOURCEW(nIDResource));
    if (!hbm) return FALSE;
    return SetBitmap(hbm);
}

BOOL CToolBar::LoadBitmap(const wchar_t* lpszResourceName) {
    HBITMAP hbm = ::LoadBitmapW(AfxGetInstanceHandle(), lpszResourceName);
    if (!hbm) return FALSE;
    return SetBitmap(hbm);
}

BOOL CToolBar::SetButtons(const UINT* lpIDArray, int nIDCount) {
    if (!m_hWnd || !lpIDArray || nIDCount <= 0) return FALSE;

    while ((int)::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0) > 0) {
        ::SendMessageW(m_hWnd, TB_DELETEBUTTON, 0, 0);
    }

    for (int i = 0; i < nIDCount; i++) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = lpIDArray[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (lpIDArray[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_INSERTBUTTONW, i, (LPARAM)&tb);
    }
    m_nCount = nIDCount;
    return TRUE;
}

BOOL CToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage) {
    if (!m_hWnd) return FALSE;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_COMMAND | TBIF_STYLE | TBIF_IMAGE;
    tbi.idCommand = nID;
    tbi.fsStyle = (BYTE)nStyle;
    tbi.iImage = iImage;
    return ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi) ? TRUE : FALSE;
}

void CToolBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const {
    nID = 0; nStyle = 0; iImage = -1;
    if (!m_hWnd) return;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb)) {
        nID = tb.idCommand;
        nStyle = tb.fsStyle;
        iImage = tb.iBitmap;
    }
}

int CToolBar::CommandToIndex(UINT nIDFind) const {
    if (!m_hWnd) return -1;
    return (int)::SendMessageW(m_hWnd, TB_COMMANDTOINDEX, nIDFind, 0);
}

UINT CToolBar::GetItemID(int nIndex) const {
    if (!m_hWnd) return 0;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb))
        return tb.idCommand;
    return 0;
}

void CToolBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (m_hWnd && lpRect)
        ::SendMessageW(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)lpRect);
}

void CToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage) {
    m_sizeButton = sizeButton;
    m_sizeImage = sizeImage;
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(sizeImage.cx, sizeImage.cy));
        ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(sizeButton.cx, sizeButton.cy));
    }
}

CSize CToolBar::GetButtonSize() const {
    return CSize(m_sizeButton.cx, m_sizeButton.cy);
}

SIZE CToolBar::GetToolBarCtrlSize() const {
    SIZE sz = {0, 0};
    if (m_hWnd) ::SendMessageW(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)&sz);
    return sz;
}

void CToolBar::SetHeight(int cyHeight) {
    if (!m_hWnd) return;
    m_sizeButton.cy = cyHeight;
    ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(m_sizeButton.cx, m_sizeButton.cy));
}

int CToolBar::GetHeight() const {
    return m_sizeButton.cy;
}

void CToolBar::SetButtonStyle(int nIndex, UINT nStyle) {
    if (!m_hWnd) return;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_STYLE;
    tbi.fsStyle = (BYTE)nStyle;
    ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi);
}

UINT CToolBar::GetButtonStyle(int nIndex) const {
    if (!m_hWnd) return 0;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb))
        return tb.fsStyle;
    return 0;
}

void CToolBar::SetButtonText(int nIndex, const wchar_t* lpszText) {
    if (!m_hWnd) return;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = (LPWSTR)lpszText;
    ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi);
}

CString CToolBar::GetButtonText(int nIndex) const {
    CString str;
    if (!m_hWnd) return str;
    wchar_t buf[256] = {};
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = buf;
    tbi.cchText = 256;
    if (::SendMessageW(m_hWnd, TB_GETBUTTONINFOW, nIndex, (LPARAM)&tbi))
        str = buf;
    return str;
}

void CToolBar::GetButtonText(int nIndex, CString& rString) const {
    rString = GetButtonText(nIndex);
}

BOOL CToolBar::SetBitmap(HBITMAP hbmImageWell) {
    if (!m_hWnd || !hbmImageWell) return FALSE;
    if (m_hbmImageWell) ::DeleteObject(m_hbmImageWell);
    m_hbmImageWell = hbmImageWell;

    TBADDBITMAP tbAddBmp = {};
    tbAddBmp.hInst = nullptr;
    tbAddBmp.nID = (UINT_PTR)hbmImageWell;
    ::SendMessageW(m_hWnd, TB_ADDBITMAP, 1, (LPARAM)&tbAddBmp);

    int nCount = (int)::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0);
    for (int i = 0; i < nCount; i++) {
        TBBUTTONINFO tbi = {};
        tbi.cbSize = sizeof(TBBUTTONINFO);
        tbi.dwMask = TBIF_IMAGE;
        tbi.iImage = i;
        ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, i, (LPARAM)&tbi);
    }
    return TRUE;
}

void CToolBar::SetToolTips(CToolTipCtrl* pToolTip) {
    // Real MFC keeps no tooltip member; the toolbar control owns it.
    if (m_hWnd && pToolTip && pToolTip->GetSafeHwnd()) {
        ::SendMessageW(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pToolTip->GetSafeHwnd(), 0);
    }
}

CToolTipCtrl* CToolBar::GetToolTips() const {
    if (!m_hWnd) return nullptr;
    HWND h = (HWND)::SendMessageW(m_hWnd, TB_GETTOOLTIPS, 0, 0);
    if (!h) return nullptr;
    return reinterpret_cast<CToolTipCtrl*>(CWnd::FromHandle(h));
}

void CToolBar::EnableDocking(DWORD dwDockStyle) {
    m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | (dwDockStyle & CBRS_ALIGN_ANY);
}

BOOL CToolBar::IsVisible() const {
    return m_hWnd && (::GetWindowLongW(m_hWnd, GWL_STYLE) & WS_VISIBLE);
}

BOOL CToolBar::IsFloating() const {
    return (m_dwStyle & CBRS_FLOATING) != 0;
}

//=============================================================================
// CStatusBar
//=============================================================================
IMPLEMENT_DYNAMIC(CStatusBar, CControlBar)

CStatusBar::CStatusBar() : m_nMinHeight(0) {
    // m_nCount/m_pData/m_cxLeftBorder/m_cxRightBorder are inherited from
    // CControlBar (initialized by its ctor).
}

CStatusBar::~CStatusBar() {
    if (m_pData) {
        free(m_pData);
        m_pData = nullptr;
    }
}

BOOL CStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;

    m_hWnd = ::CreateWindowExW(0, STATUSCLASSNAMEW, nullptr,
                                WS_CHILD | WS_VISIBLE | dwStyle,
                                0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

BOOL CStatusBar::SetIndicators(const UINT* lpIDArray, int nIDCount) {
    if (!m_hWnd || !lpIDArray || nIDCount <= 0) return FALSE;

    if (m_pData) free(m_pData);
    m_pData = (UINT*)malloc(nIDCount * sizeof(UINT) * 4);
    if (!m_pData) return FALSE;
    m_nCount = nIDCount;

    for (int i = 0; i < nIDCount; i++) {
        m_pData[i * 4] = lpIDArray[i];
        m_pData[i * 4 + 1] = 0;
        m_pData[i * 4 + 2] = 0;
        m_pData[i * 4 + 3] = 0;
    }

    // Simple even distribution
    int nParts = nIDCount;
    int* pWidths = (int*)malloc(nParts * sizeof(int));
    if (pWidths) {
        RECT rc;
        ::GetClientRect(m_hWnd, &rc);
        int cxTotal = rc.right - rc.left;
        int cxWidth = cxTotal / nParts;
        for (int i = 0; i < nParts; i++) {
            pWidths[nParts - 1 - i] = cxTotal - (i * cxWidth);
        }
        ::SendMessageW(m_hWnd, SB_SETPARTS, nParts, (LPARAM)pWidths);
        free(pWidths);
    }

    // Set text for string-based indicators
    for (int i = 0; i < nIDCount; i++) {
        if (lpIDArray[i] != 0 && lpIDArray[i] != ID_SEPARATOR) {
            wchar_t buf[256];
            if (::LoadStringW(AfxGetInstanceHandle(), lpIDArray[i], buf, 256) > 0) {
                ::SendMessageW(m_hWnd, SB_SETTEXTW, i, (LPARAM)buf);
            }
        }
    }

    return TRUE;
}

BOOL CStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth) {
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount || !m_pData) return FALSE;

    CString paneText = GetPaneText(nIndex);
    m_pData[nIndex * 4] = nID;
    m_pData[nIndex * 4 + 1] = nStyle;
    m_pData[nIndex * 4 + 2] = cxWidth;

    RECT clientRect = {};
    ::GetClientRect(m_hWnd, &clientRect);
    const int totalWidth = std::max(0, static_cast<int>(clientRect.right - clientRect.left));

    int fixedWidth = 0;
    int autoPaneCount = 0;
    for (int i = 0; i < m_nCount; ++i) {
        int paneWidth = static_cast<int>(m_pData[i * 4 + 2]);
        if (paneWidth > 0) {
            fixedWidth += paneWidth;
        } else {
            ++autoPaneCount;
        }
    }

    int remainingWidth = std::max(0, totalWidth - fixedWidth);
    int autoWidth = (autoPaneCount > 0) ? (remainingWidth / autoPaneCount) : 0;
    int autoRemainder = (autoPaneCount > 0) ? (remainingWidth % autoPaneCount) : 0;

    std::vector<int> parts(static_cast<size_t>(m_nCount));
    int right = 0;
    for (int i = 0; i < m_nCount; ++i) {
        int paneWidth = static_cast<int>(m_pData[i * 4 + 2]);
        if (paneWidth <= 0) {
            paneWidth = autoWidth;
            if (autoRemainder > 0) {
                ++paneWidth;
                --autoRemainder;
            }
        }
        right += std::max(0, paneWidth);
        parts[static_cast<size_t>(i)] = right;
    }

    if (!parts.empty()) {
        parts.back() = -1;
    }

    ::SendMessageW(m_hWnd, SB_SETPARTS, m_nCount, reinterpret_cast<LPARAM>(parts.data()));
    ::SendMessageW(m_hWnd, SB_SETTEXTW, nIndex | (nStyle & 0xFF00),
                   reinterpret_cast<LPARAM>(paneText.GetString()));
    return TRUE;
}

void CStatusBar::GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const {
    if (nIndex >= 0 && nIndex < m_nCount && m_pData) {
        nID = m_pData[nIndex * 4];
        nStyle = m_pData[nIndex * 4 + 1];
        cxWidth = m_pData[nIndex * 4 + 2];
    } else {
        nID = 0; nStyle = 0; cxWidth = 0;
    }
}

int CStatusBar::CommandToIndex(UINT nIDFind) const {
    if (!m_pData) return -1;
    for (int i = 0; i < m_nCount; i++) {
        if (m_pData[i * 4] == nIDFind) return i;
    }
    return -1;
}

UINT CStatusBar::GetItemID(int nIndex) const {
    if (!m_pData || nIndex < 0 || nIndex >= m_nCount) return 0;
    return m_pData[nIndex * 4];
}

void CStatusBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (m_hWnd && lpRect)
        ::SendMessageW(m_hWnd, SB_GETRECT, nIndex, (LPARAM)lpRect);
}

BOOL CStatusBar::SetPaneText(int nIndex, const wchar_t* lpszNewText, BOOL bUpdate) {
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount) return FALSE;
    ::SendMessageW(m_hWnd, SB_SETTEXTW, nIndex, (LPARAM)(lpszNewText ? lpszNewText : L""));
    if (bUpdate) {
        ::InvalidateRect(m_hWnd, nullptr, TRUE);
        ::UpdateWindow(m_hWnd);
    }
    return TRUE;
}

CString CStatusBar::GetPaneText(int nIndex) const {
    CString str;
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount) return str;
    int len = LOWORD(::SendMessageW(m_hWnd, SB_GETTEXTLENGTHW, nIndex, 0));
    if (len > 0) {
        wchar_t* buf = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
        if (buf) {
            ::SendMessageW(m_hWnd, SB_GETTEXTW, nIndex, (LPARAM)buf);
            str = buf;
            free(buf);
        }
    }
    return str;
}

void CStatusBar::GetPaneText(int nIndex, CString& rString) const {
    rString = GetPaneText(nIndex);
}

void CStatusBar::SetPaneStyle(int nIndex, UINT nStyle) {
    if (nIndex >= 0 && nIndex < m_nCount && m_pData) {
        m_pData[nIndex * 4 + 1] = nStyle;
    }
}

UINT CStatusBar::GetPaneStyle(int nIndex) const {
    if (nIndex >= 0 && nIndex < m_nCount && m_pData) {
        return m_pData[nIndex * 4 + 1];
    }
    return 0;
}

CSize CStatusBar::GetPaneSize(int nIndex) const {
    CSize sz(0, 0);
    if (m_hWnd) {
        RECT rc = {};
        ::SendMessageW(m_hWnd, SB_GETRECT, nIndex, (LPARAM)&rc);
        sz.cx = rc.right - rc.left;
        sz.cy = rc.bottom - rc.top;
    }
    return sz;
}

CSize CStatusBar::GetBorders() const {
    CSize sz(0, 0);
    if (m_hWnd) {
        int borders[3] = {0, 0, 0};
        if (::SendMessageW(m_hWnd, SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(borders))) {
            sz.cx = borders[0];
            sz.cy = borders[1];
        }
    }
    return sz;
}

void CStatusBar::SetToolTips(CToolTipCtrl* pToolTip) {
    // Real MFC CStatusBar has no tooltip member (status panes manage their own).
    (void)pToolTip;
}

CToolTipCtrl* CStatusBar::GetToolTips() const {
    return nullptr;
}

void CStatusBar::EnableDocking(DWORD dwDockStyle) {
    m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | (dwDockStyle & CBRS_ALIGN_ANY);
}

BOOL CStatusBar::IsSimple() const {
    if (!m_hWnd) return FALSE;
    return (BOOL)::SendMessageW(m_hWnd, SB_ISSIMPLE, 0, 0);
}

//=============================================================================
// CDialogBar
//=============================================================================
IMPLEMENT_DYNAMIC(CDialogBar, CControlBar)

CDialogBar::CDialogBar()
    : m_bAutoDelete(FALSE), m_pOccDialogInfo(nullptr) {
    memset(_dialogbar_padding, 0, sizeof(_dialogbar_padding));
}

CDialogBar::~CDialogBar() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}

BOOL CDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = nStyle;

    m_hWnd = ::CreateDialogParamW(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCEW(nIDTemplate),
                                   pParentWnd->GetSafeHwnd(),
                                   nullptr, 0);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}

BOOL CDialogBar::Create(CWnd* pParentWnd, const wchar_t* lpszTemplateName,
                         UINT nStyle, UINT nID) {
    if (!pParentWnd || !lpszTemplateName) return FALSE;

    m_dwStyle = nStyle;

    m_hWnd = ::CreateDialogParamW(AfxGetInstanceHandle(),
                                   lpszTemplateName,
                                   pParentWnd->GetSafeHwnd(),
                                   nullptr, 0);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);

    if (nStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }

    return TRUE;
}

void CDialogBar::UpdateData(BOOL bSaveAndValidate) {
    (void)bSaveAndValidate;
}

BOOL CDialogBar::IsVisible() const {
    return m_hWnd && ::IsWindowVisible(m_hWnd);
}

void CDialogBar::SetOccDialogInfo(void* pDialogInfo) {
    m_pOccDialogInfo = pDialogInfo;
}

//=============================================================================
// CSplitterWnd
//=============================================================================
IMPLEMENT_DYNAMIC(CSplitterWnd, CWnd)

CSplitterWnd::CSplitterWnd()
    : m_nRows(0), m_nCols(0), m_cxSplitter(4), m_cySplitter(4),
      m_cxBorderShare(0), m_cyBorderShare(0),
      m_cxSplitterGap(4), m_cySplitterGap(4),
      m_nMaxRows(0), m_nMaxCols(0),
      m_nId(0), m_bHasHScroll(FALSE), m_bHasVScroll(FALSE),
      m_pActivePane(nullptr), m_nActiveRow(0), m_nActiveCol(0) {
    m_sizeMin.cx = 0; m_sizeMin.cy = 0;
    memset(_splitterwnd_padding, 0, sizeof(_splitterwnd_padding));
}

CSplitterWnd::~CSplitterWnd() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}

BOOL CSplitterWnd::Create(CWnd* pParentWnd, int nMaxRows, int nMaxCols,
                           SIZE sizeMin, CCreateContext* pContext, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;
    m_nMaxRows = nMaxRows;
    m_nMaxCols = nMaxCols;
    m_sizeMin = sizeMin;

    m_hWnd = ::CreateWindowExW(0, L"AfxSplitterWnd", nullptr,
                                dwStyle | WS_CLIPCHILDREN,
                                0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), pContext);
    if (!m_hWnd) return FALSE;
    m_nId = nID;
    return TRUE;
}

BOOL CSplitterWnd::CreateStatic(CWnd* pParentWnd, int nRows, int nCols,
                                 DWORD dwStyle, UINT nID) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nMaxRows = nRows;
    m_nMaxCols = nCols;

    m_hWnd = ::CreateWindowExW(0, L"AfxSplitterWnd", nullptr,
                                dwStyle | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                                0, 0, 0, 0,
                                pParentWnd->GetSafeHwnd(),
                                (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

BOOL CSplitterWnd::CreateView(int row, int col, CRuntimeClass* pViewClass,
                               SIZE sizeInit, CCreateContext* pContext) {
    (void)row; (void)col; (void)pViewClass; (void)sizeInit; (void)pContext;
    return FALSE;
}

CWnd* CSplitterWnd::GetPane(int row, int col) const {
    (void)row; (void)col;
    return nullptr;
}

void CSplitterWnd::GetRowInfo(int row, int& cyCur, int& cyMin) const {
    cyCur = 0; cyMin = 0;
    if (!m_hWnd) return;
    (void)row;
}

void CSplitterWnd::SetRowInfo(int row, int cyIdeal, int cyMin) {
    (void)row; (void)cyIdeal; (void)cyMin;
}

void CSplitterWnd::GetColumnInfo(int col, int& cxCur, int& cxMin) const {
    cxCur = 0; cxMin = 0;
    (void)col;
}

void CSplitterWnd::SetColumnInfo(int col, int cxIdeal, int cxMin) {
    (void)col; (void)cxIdeal; (void)cxMin;
}

void CSplitterWnd::RecalcLayout() {
}

void CSplitterWnd::SetSplitCursor(int ht) {
    (void)ht;
}

int CSplitterWnd::GetActivePane(int* pRow, int* pCol) const {
    if (pRow) *pRow = m_nActiveRow;
    if (pCol) *pCol = m_nActiveCol;
    return 0;
}

void CSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd) {
    m_nActiveRow = row;
    m_nActiveCol = col;
    m_pActivePane = pWnd;
}

CWnd* CSplitterWnd::GetActivePane() {
    return m_pActivePane;
}

BOOL CSplitterWnd::CanActivateNext(BOOL bPrev) {
    return FALSE;
}

void CSplitterWnd::ActivateNext(BOOL bPrev) {
}

BOOL CSplitterWnd::DoKeyboardSplit() {
    return FALSE;
}

void CSplitterWnd::OnDrawSplitter(CDC* pDC, int nType, const CRect& rect) {
    if (pDC && pDC->m_hDC) {
        HBRUSH hBrush = ::GetSysColorBrush(COLOR_3DFACE);
        ::FillRect(pDC->m_hDC, (const RECT*)rect, hBrush);
    }
}

void CSplitterWnd::OnInvertTracker(const CRect& rect) {
}

BOOL CSplitterWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) {
    return FALSE;
}

//=============================================================================
// CTaskDialog
//=============================================================================
CTaskDialog::CTaskDialog(const wchar_t* pszContent, const wchar_t* pszMainInstruction,
                          const wchar_t* pszWindowTitle, int nCommonButtons, int nTaskDialogOptions)
    : m_nCommonButtons(nCommonButtons), m_nTaskDialogOptions(nTaskDialogOptions),
      m_nDialogWidth(0), m_hMainIcon(nullptr), m_hFooterIcon(nullptr),
      m_nProgressMin(0), m_nProgressMax(100), m_nProgressPos(0),
      m_bProgressMarquee(FALSE), m_bVerificationChecked(FALSE),
      m_nSelectedCommandID(0), m_nSelectedRadioButtonID(0), m_bExpanded(FALSE) {
    if (pszContent) m_strContent = pszContent;
    if (pszMainInstruction) m_strMainInstruction = pszMainInstruction;
    if (pszWindowTitle) m_strWindowTitle = pszWindowTitle;
    memset(_taskdialog_padding, 0, sizeof(_taskdialog_padding));
}

CTaskDialog::~CTaskDialog() {
}

void CTaskDialog::SetDialogWidth(int nWidth) {
    m_nDialogWidth = nWidth;
}

void CTaskDialog::SetMainIcon(HICON hMainIcon) {
    m_hMainIcon = hMainIcon;
}

void CTaskDialog::SetMainIcon(UINT nMainIconID) {
    m_hMainIcon = ::LoadIconW(nullptr, MAKEINTRESOURCEW(nMainIconID));
}

void CTaskDialog::SetFooterIcon(HICON hFooterIcon) {
    m_hFooterIcon = hFooterIcon;
}

void CTaskDialog::SetFooterText(const wchar_t* pszFooterText) {
    if (pszFooterText) m_strFooterText = pszFooterText;
}

void CTaskDialog::SetVerificationCheckboxText(const wchar_t* pszText) {
    if (pszText) m_strVerificationText = pszText;
}

void CTaskDialog::SetExpandedInformation(const wchar_t* pszText) {
    if (pszText) m_strExpandedInfo = pszText;
}

void CTaskDialog::SetExpandedControlText(const wchar_t* pszText) {
    if (pszText) m_strExpandedControlText = pszText;
}

void CTaskDialog::SetCollapsedControlText(const wchar_t* pszText) {
    if (pszText) m_strCollapsedControlText = pszText;
}

void CTaskDialog::SetProgressBarRange(int nMin, int nMax) {
    m_nProgressMin = nMin;
    m_nProgressMax = nMax;
}

void CTaskDialog::SetProgressBarPosition(int nPos) {
    m_nProgressPos = nPos;
}

void CTaskDialog::SetProgressBarMarquee(BOOL bMarquee, int nSpeed) {
    m_bProgressMarquee = bMarquee;
    (void)nSpeed;
}

HRESULT CTaskDialog::AddCommandControl(int nCommandID, const wchar_t* pszLabel) {
    (void)nCommandID; (void)pszLabel;
    return S_OK;
}

HRESULT CTaskDialog::AddRadioButton(int nRadioButtonID, const wchar_t* pszLabel) {
    (void)nRadioButtonID; (void)pszLabel;
    return S_OK;
}

HRESULT CTaskDialog::AddPushButton(int nButtonID, const wchar_t* pszLabel) {
    (void)nButtonID; (void)pszLabel;
    return S_OK;
}

int CTaskDialog::DoModal(HWND hWndParent) {
    // Use TaskDialogIndirect if available (Vista+), otherwise fallback to MessageBox
    TASKDIALOGCONFIG tc = {};
    tc.cbSize = sizeof(TASKDIALOGCONFIG);
    tc.hwndParent = hWndParent;
    tc.dwFlags = m_nTaskDialogOptions;
    tc.dwCommonButtons = m_nCommonButtons;
    tc.pszWindowTitle = m_strWindowTitle;
    tc.pszMainInstruction = m_strMainInstruction;
    tc.pszContent = m_strContent;
    tc.hMainIcon = m_hMainIcon;
    tc.hFooterIcon = m_hFooterIcon;
    tc.pszFooter = m_strFooterText.IsEmpty() ? nullptr : (const wchar_t*)m_strFooterText;
    tc.pszVerificationText = m_strVerificationText.IsEmpty() ? nullptr : (const wchar_t*)m_strVerificationText;
    tc.pszExpandedInformation = m_strExpandedInfo.IsEmpty() ? nullptr : (const wchar_t*)m_strExpandedInfo;
    tc.pszExpandedControlText = m_strExpandedControlText.IsEmpty() ? nullptr : (const wchar_t*)m_strExpandedControlText;
    tc.pszCollapsedControlText = m_strCollapsedControlText.IsEmpty() ? nullptr : (const wchar_t*)m_strCollapsedControlText;

    if (m_nDialogWidth > 0) tc.cxWidth = m_nDialogWidth;

    int nButton = 0;
    int nRadio = 0;
    BOOL bVerification = FALSE;

    using TaskDialogIndirectFn = HRESULT (WINAPI *)(const TASKDIALOGCONFIG*, int*, int*, BOOL*);
    HMODULE hComctl = ::GetModuleHandleW(L"comctl32.dll");
    if (!hComctl) {
        hComctl = ::LoadLibraryW(L"comctl32.dll");
    }
    TaskDialogIndirectFn pTaskDialogIndirect = hComctl
        ? reinterpret_cast<TaskDialogIndirectFn>(::GetProcAddress(hComctl, "TaskDialogIndirect"))
        : nullptr;

    if (pTaskDialogIndirect) {
        HRESULT hr = pTaskDialogIndirect(&tc, &nButton, &nRadio, &bVerification);
        if (SUCCEEDED(hr)) {
            m_bVerificationChecked = bVerification;
            m_nSelectedRadioButtonID = nRadio;
            return nButton;
        }
    }

    // Fallback to simple MessageBox
    UINT uType = MB_OK;
    if (m_nCommonButtons & CTaskDialog::TDCBF_OK_BUTTON) uType = MB_OK;
    else if (m_nCommonButtons & CTaskDialog::TDCBF_YES_BUTTON) uType = MB_YESNO;
    else if (m_nCommonButtons & CTaskDialog::TDCBF_RETRY_BUTTON) uType = MB_RETRYCANCEL;

    int result = ::MessageBoxW(hWndParent, m_strContent, m_strWindowTitle, uType);
    switch (result) {
        case IDOK: return IDOK;
        case IDYES: return IDYES;
        case IDNO: return IDNO;
        case IDCANCEL: return IDCANCEL;
        case IDRETRY: return IDRETRY;
        default: return IDOK;
    }

}

// Ribbon element wrappers (Wave 2 retry)
//=============================================================================

// Symbol: ??0CMFCRibbonButton@@QEAA@IPEB_WPEAUHICON__@@H1HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonButton__QEAA_IPEB_WPEAUHICON____H1HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, HICON hIcon, int bAlwaysShowDescription,
    HICON hIconSmall, int bAutoDestroyIcon, int bAlphaBlendIcon) {
    if (!pThis) return nullptr;
    return new(pThis) CMFCRibbonButton(nID, lpszText, hIcon, bAlwaysShowDescription, hIconSmall, bAutoDestroyIcon, bAlphaBlendIcon);
}

// Symbol: ??0CMFCRibbonGallery@@QEAA@IPEB_WHHAEAVCMFCToolBarImages@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGallery__QEAA_IPEB_WHHAEAVCMFCToolBarImages___Z(
    void* pThis, UINT nID, const wchar_t* lpszText, int nSmallImageIndex, int nLargeImageIndex, CMFCToolBarImages* /*pImages*/) {
    if (!pThis) return nullptr;
    auto* pGallery = new(pThis) CMFCRibbonGallery();
    pGallery->SetID(nID);
    pGallery->SetText(lpszText);
    (void)nSmallImageIndex;
    (void)nLargeImageIndex;
    return pGallery;
}

// Symbol: ??0CMFCRibbonGallery@@QEAA@IPEB_WHHVCSize@@HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGallery__QEAA_IPEB_WHHVCSize__HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, int nSmallImageIndex, int nLargeImageIndex,
    CSize /*sizeIcon*/, int /*nIconsInRow*/, int /*nRows*/) {
    if (!pThis) return nullptr;
    auto* pGallery = new(pThis) CMFCRibbonGallery();
    pGallery->SetID(nID);
    pGallery->SetText(lpszText);
    (void)nSmallImageIndex;
    (void)nLargeImageIndex;
    return pGallery;
}

// Symbol: ??0CMFCRibbonStatusBarPane@@QEAA@IPEB_WHPEAUHICON__@@0H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonStatusBarPane__QEAA_IPEB_WHPEAUHICON____0H_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, int /*bIsStatic*/, HICON /*hIcon*/, HICON /*hIconDisabled*/, int /*nTextAlign*/) {
    if (!pThis) return nullptr;
    auto* pPane = new(pThis) CMFCRibbonStatusBarPane();
    pPane->SetID(nID);
    pPane->SetText(lpszText);
    return pPane;
}

// Symbol: ??0CMFCRibbonStatusBarPane@@QEAA@IPEB_WIHKPEAUHICON__@@HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonStatusBarPane__QEAA_IPEB_WIHKPEAUHICON____HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, UINT /*cxText*/, int /*bAlmostLargeText*/, DWORD /*dwStyle*/,
    HICON /*hIcon*/, int /*nTextAlign*/, int /*bIsExtended*/) {
    if (!pThis) return nullptr;
    auto* pPane = new(pThis) CMFCRibbonStatusBarPane();
    pPane->SetID(nID);
    pPane->SetText(lpszText);
    return pPane;
}

// Symbol: ??0CMFCRibbonStatusBarPane@@QEAA@IPEB_WPEAUHBITMAP__@@HKPEAUHICON__@@HH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonStatusBarPane__QEAA_IPEB_WPEAUHBITMAP____HKPEAUHICON____HH_Z(
    void* pThis, UINT nID, const wchar_t* lpszText, HBITMAP hBmpAnimation, int cxAnimation, DWORD dwAnimationSpeed,
    HICON /*hIcon*/, int /*nTextAlign*/, int /*bIsExtended*/) {
    if (!pThis) return nullptr;
    auto* pPane = new(pThis) CMFCRibbonStatusBarPane();
    pPane->SetID(nID);
    pPane->SetText(lpszText);
    auto& state = g_statusPaneStates[pPane->GetID()];
    state.hAnimationBitmap = hBmpAnimation;
    state.cxAnimation = cxAnimation;
    state.dwAnimationDelay = dwAnimationSpeed;
    return pPane;
}

// Symbol: ?SetToolTipText@CMFCRibbonBaseElement@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetToolTipText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(CMFCRibbonBaseElement* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonToolTips[RibbonElementID(pThis)] = (lpszText != nullptr) ? lpszText : L"";
}

// Symbol: ?GetToolTipText@CMFCRibbonBaseElement@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonBaseElement* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonToolTips.find(RibbonElementID(pThis));
    if (it != g_ribbonToolTips.end()) {
        BuildCStringResult(pRet, it->second);
        return;
    }
    CString text = pThis->GetText();
    BuildCStringResult(pRet, (const wchar_t*)text);
}

// Symbol: ?SetDescription@CMFCRibbonBaseElement@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(CMFCRibbonBaseElement* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonDescriptions[RibbonElementID(pThis)] = (lpszText != nullptr) ? lpszText : L"";
}

// Symbol: ?GetDescription@CMFCRibbonBaseElement@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDescription_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonBaseElement* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonDescriptions.find(RibbonElementID(pThis));
    if (it != g_ribbonDescriptions.end()) {
        BuildCStringResult(pRet, it->second);
        return;
    }
    CString text = pThis->GetText();
    BuildCStringResult(pRet, (const wchar_t*)text);
}

// Symbol: ?SetText@CMFCRibbonButton@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCRibbonButton__UEAAXPEB_W_Z(CMFCRibbonButton* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    pThis->CMFCRibbonBaseElement::SetText(lpszText);
}

// Symbol: ?CanBeStretched@CMFCRibbonButton@@UEAAHXZ
extern "C" int MS_ABI impl__CanBeStretched_CMFCRibbonButton__UEAAHXZ(CMFCRibbonButton* /*pThis*/) {
    return FALSE;
}

// Symbol: ?GetRegularSize@CMFCRibbonButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonButton* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 32 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?GetCompactSize@CMFCRibbonButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonButton* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 22 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?GetIntermediateSize@CMFCRibbonButton@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonButton__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonButton* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 26 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?OnCalcTextSize@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnCalcTextSize_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnDraw@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    if (CMFCVisualManager* manager = CMFCVisualManager::GetInstance(); manager != nullptr) {
        manager->OnDrawRibbonDefaultPaneButton(pDC, pThis);
    }
}

// Symbol: ?OnDrawBorder@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    if (CMFCVisualManager* manager = CMFCVisualManager::GetInstance(); manager != nullptr) {
        manager->OnDrawRibbonButtonBorder(pDC, pThis);
    }
}

// Symbol: ?OnFillBackground@CMFCRibbonButton@@UEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonButton__UEAAKPEAVCDC___Z(CMFCRibbonButton* /*pThis*/, CDC* /*pDC*/) {
    const COLORREF color = FeaturePackBackgroundColor();
    return color;
}

// Symbol: ?GetRegularSize@CMFCRibbonGallery@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonGallery__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonGallery* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 42);
}

// Symbol: ?GetCompactSize@CMFCRibbonGallery@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonGallery__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonGallery* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 64 + RibbonTextPixels(pThis), 32);
}

// Symbol: ?OnDraw@CMFCRibbonGallery@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonGallery__UEAAXPEAVCDC___Z(CMFCRibbonGallery* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}

// Symbol: ?OnEnable@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonGallery__UEAAXH_Z(CMFCRibbonGallery* pThis, int bEnable) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonElementsEnabled[pThis] = (bEnable != FALSE);
}

// Symbol: ?SelectItem@CMFCRibbonGallery@@QEAAXH@Z
extern "C" void MS_ABI impl__SelectItem_CMFCRibbonGallery__QEAAXH_Z(CMFCRibbonGallery* pThis, int nItem) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_galleryLastSelectedByID[pThis->GetID()] = nItem;
}

// Symbol: ?GetLastSelectedItem@CMFCRibbonGallery@@SAHI@Z
extern "C" int MS_ABI impl__GetLastSelectedItem_CMFCRibbonGallery__SAHI_Z(UINT nGalleryID) {
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_galleryLastSelectedByID.find(nGalleryID);
    return (it != g_galleryLastSelectedByID.end()) ? it->second : -1;
}

// Symbol: ?GetCompactSize@CMFCRibbonEdit@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonEdit__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonEdit* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 80 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?GetIntermediateSize@CMFCRibbonEdit@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonEdit__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonEdit* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 120 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?OnDraw@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonEdit__UEAAXPEAVCDC___Z(CMFCRibbonEdit* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}

// Symbol: ?OnEnable@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonEdit__UEAAXH_Z(CMFCRibbonEdit* pThis, int bEnable) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonElementsEnabled[pThis] = (bEnable != FALSE);
}

// Symbol: ?GetRegularSize@CMFCRibbonSlider@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonSlider__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonSlider* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?OnDraw@CMFCRibbonSlider@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonSlider__UEAAXPEAVCDC___Z(CMFCRibbonSlider* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}

// Symbol: ?SetRange@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nMin, int nMax) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_sliderStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}

// Symbol: ?SetPos@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_sliderStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}

// Symbol: ?SetZoomButtons@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomButtons_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int bSet) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_sliderStates[pThis->GetID()].bShowZoomButtons = (bSet != FALSE);
}

// Symbol: ?SetZoomIncrement@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomIncrement_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int nDelta) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_sliderStates[pThis->GetID()].nZoomIncrement = std::max(1, nDelta);
}

// Symbol: ?GetRegularSize@CMFCRibbonProgressBar@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonProgressBar__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonProgressBar* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 16);
}

// Symbol: ?OnDraw@CMFCRibbonProgressBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonProgressBar__UEAAXPEAVCDC___Z(CMFCRibbonProgressBar* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}

// Symbol: ?SetRange@CMFCRibbonProgressBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonProgressBar__QEAAXHH_Z(CMFCRibbonProgressBar* pThis, int nMin, int nMax) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_progressStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}

// Symbol: ?SetPos@CMFCRibbonProgressBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonProgressBar__QEAAXHH_Z(CMFCRibbonProgressBar* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_progressStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}

// Symbol: ?SetInfiniteMode@CMFCRibbonProgressBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetInfiniteMode_CMFCRibbonProgressBar__QEAAXH_Z(CMFCRibbonProgressBar* pThis, int bSet) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_progressStates[pThis->GetID()].bInfinite = (bSet != FALSE);
}

// Symbol: ?GetIntermediateSize@CMFCRibbonStatusBarPane@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonStatusBarPane__MEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonStatusBarPane* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?OnCalcTextSize@CMFCRibbonStatusBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnCalcTextSize_CMFCRibbonStatusBarPane__MEAAXPEAVCDC___Z(CMFCRibbonStatusBarPane* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnDraw@CMFCRibbonStatusBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonStatusBarPane__MEAAXPEAVCDC___Z(CMFCRibbonStatusBarPane* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    if (CMFCVisualManager* manager = CMFCVisualManager::GetInstance(); manager != nullptr) {
        manager->OnDrawRibbonStatusBarPane(pDC, nullptr, pThis);
    }
}

// Symbol: ?OnFillBackground@CMFCRibbonStatusBarPane@@UEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonStatusBarPane__UEAAKPEAVCDC___Z(CMFCRibbonStatusBarPane* /*pThis*/, CDC* /*pDC*/) {
    const COLORREF color = SystemColor(COLOR_BTNFACE);
    return color;
}

// Symbol: ?GetToolTipText@CMFCRibbonStatusBarPane@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonStatusBarPane__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonStatusBarPane* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonToolTips.find(RibbonElementID(pThis));
    if (it != g_ribbonToolTips.end()) {
        BuildCStringResult(pRet, it->second);
        return;
    }
    CString text = pThis->GetText();
    BuildCStringResult(pRet, (const wchar_t*)text);
}

// Symbol: ?SetAnimationList@CMFCRibbonStatusBarPane@@QEAAHIHK@Z
extern "C" int MS_ABI impl__SetAnimationList_CMFCRibbonStatusBarPane__QEAAHIHK_Z(
    CMFCRibbonStatusBarPane* pThis, UINT nAnimationListResID, int cxAnimation, DWORD dwAnimationSpeed) {
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.hAnimationBitmap = nullptr;
    state.cxAnimation = cxAnimation;
    state.dwAnimationDelay = dwAnimationSpeed;
    state.nAnimationCount = 0;
    return (nAnimationListResID != 0);
}

// Symbol: ?SetAnimationList@CMFCRibbonStatusBarPane@@QEAAXPEAUHBITMAP__@@HK@Z
extern "C" void MS_ABI impl__SetAnimationList_CMFCRibbonStatusBarPane__QEAAXPEAUHBITMAP____HK_Z(
    CMFCRibbonStatusBarPane* pThis, HBITMAP hAnimationList, int cxAnimation, DWORD dwAnimationSpeed) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.hAnimationBitmap = hAnimationList;
    state.cxAnimation = cxAnimation;
    state.dwAnimationDelay = dwAnimationSpeed;
    state.nAnimationCount = 0;
}

// Symbol: ?StartAnimation@CMFCRibbonStatusBarPane@@QEAAXII@Z
extern "C" void MS_ABI impl__StartAnimation_CMFCRibbonStatusBarPane__QEAAXII_Z(
    CMFCRibbonStatusBarPane* pThis, UINT nAnimationDuration, UINT nAnimationDelay) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.bAnimating = true;
    state.nAnimationFrame = 0;
    state.nAnimationCount = nAnimationDuration;
    if (nAnimationDelay != 0) state.dwAnimationDelay = nAnimationDelay;
}

// Symbol: ?StopAnimation@CMFCRibbonStatusBarPane@@QEAAXXZ
extern "C" void MS_ABI impl__StopAnimation_CMFCRibbonStatusBarPane__QEAAXXZ(CMFCRibbonStatusBarPane* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_statusPaneStates[pThis->GetID()].bAnimating = false;
}

//=============================================================================
// CToolBar / CStatusBar ABI wrappers
// These provide proper // Symbol: exclusions for CString-returning methods.
//=============================================================================

// Symbol: ?GetButtonText@CToolBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetButtonText_CToolBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CToolBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetButtonText(nIndex));
}

// Symbol: ?GetButtonText@CToolBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetButtonText_CToolBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CToolBar* pThis, int nIndex, CString* pStr) {
    pThis->GetButtonText(nIndex, *pStr);
}

// Symbol: ?GetPaneText@CStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetPaneText_CStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CStatusBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetPaneText(nIndex));
}

// Symbol: ?GetPaneInfo@CStatusBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetPaneInfo_CStatusBar__QEBAXHAEAI0AEAH_Z(
    const CStatusBar* pThis, int nIndex, UINT* pnID, UINT* pnStyle, int* pcxWidth) {
    UINT id = 0;
    UINT style = 0;
    int width = 0;
    if (pThis) {
        pThis->GetPaneInfo(nIndex, id, style, width);
    }
    if (pnID) *pnID = id;
    if (pnStyle) *pnStyle = style;
    if (pcxWidth) *pcxWidth = width;
}

// Symbol: ?GetPaneText@CStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CStatusBar* pThis, int nIndex, CString* pStr) {
    pThis->GetPaneText(nIndex, *pStr);
}

// Symbol: ?GetText@CStatusBarCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HPEAH@Z
extern "C" void MS_ABI impl__GetText_CStatusBarCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEAH_Z(
    CString* pRet, const CWnd* pThis, int nPane, int* pType) {
    CString text;
    int type = 0;
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (hwnd) {
        LRESULT lenAndType = ::SendMessageW(hwnd, SB_GETTEXTLENGTHW, nPane, 0);
        int len = LOWORD(lenAndType);
        type = HIWORD(lenAndType);
        if (len > 0) {
            std::vector<wchar_t> buffer(static_cast<size_t>(len) + 1);
            ::SendMessageW(hwnd, SB_GETTEXTW, nPane, reinterpret_cast<LPARAM>(buffer.data()));
            text = buffer.data();
        }
    }
    if (pType) *pType = type;
    new (pRet) CString(text);
}

// Symbol: ?GetTipText@CStatusBarCtrl@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetTipText_CStatusBarCtrl__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CWnd* pThis, int nPane) {
    CString text;
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (hwnd) {
        constexpr int kMaxTipText = 1024;
        std::vector<wchar_t> buffer(kMaxTipText);
        ::SendMessageW(hwnd, SB_GETTIPTEXTW, MAKEWPARAM(nPane, kMaxTipText), reinterpret_cast<LPARAM>(buffer.data()));
        buffer.back() = L'\0';
        text = buffer.data();
    }
    new (pRet) CString(text);
}

// Symbol: ?GetString@CToolBarCtrl@@QEBAHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetString_CToolBarCtrl__QEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, int nString, CString* pString) {
    if (!pString) return 0;
    *pString = L"";
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (!hwnd) return 0;

    constexpr int kMaxToolBarString = 1024;
    std::vector<wchar_t> buffer(kMaxToolBarString);
    LRESULT copied = ::SendMessageW(
        hwnd, TB_GETSTRINGW, MAKEWPARAM(kMaxToolBarString, nString), reinterpret_cast<LPARAM>(buffer.data()));
    if (copied < 0) return static_cast<int>(copied);
    buffer.back() = L'\0';
    *pString = buffer.data();
    return static_cast<int>(copied);
}

// Symbol: ?Create@CStatusBarCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CStatusBarCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
    ::InitCommonControlsEx(&icc);
    return pThis->CreateEx(0, STATUSCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}

// Symbol: ?CreateEx@CStatusBarCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CStatusBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
    ::InitCommonControlsEx(&icc);
    return pThis->CreateEx(dwExStyle, STATUSCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}

// Symbol: ?GetBorders@CStatusBarCtrl@@QEBAHAEAH00@Z
extern "C" int MS_ABI impl__GetBorders_CStatusBarCtrl__QEBAHAEAH00_Z(
    const CWnd* pThis, int* pHorz, int* pVert, int* pSpacing) {
    int borders[3] = {};
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    BOOL ok = hwnd ? static_cast<BOOL>(::SendMessageW(hwnd, SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(borders))) : FALSE;
    if (pHorz) *pHorz = borders[0];
    if (pVert) *pVert = borders[1];
    if (pSpacing) *pSpacing = borders[2];
    return ok;
}

// Symbol: ?GetText@CStatusBarCtrl@@QEBAHPEA_WHPEAH@Z
extern "C" int MS_ABI impl__GetText_CStatusBarCtrl__QEBAHPEA_WHPEAH_Z(
    const CWnd* pThis, wchar_t* pBuffer, int nPane, int* pType) {
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    if (!hwnd) {
        if (pType) *pType = 0;
        if (pBuffer) *pBuffer = L'\0';
        return 0;
    }
    LRESULT lenAndType = ::SendMessageW(hwnd, SB_GETTEXTLENGTHW, nPane, 0);
    if (pType) *pType = HIWORD(lenAndType);
    if (!pBuffer) return LOWORD(lenAndType);
    LRESULT copied = ::SendMessageW(hwnd, SB_GETTEXTW, nPane, reinterpret_cast<LPARAM>(pBuffer));
    return LOWORD(copied);
}

// Symbol: ?GetTextLength@CStatusBarCtrl@@QEBAHHPEAH@Z
extern "C" int MS_ABI impl__GetTextLength_CStatusBarCtrl__QEBAHHPEAH_Z(
    const CWnd* pThis, int nPane, int* pType) {
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    LRESULT lenAndType = hwnd ? ::SendMessageW(hwnd, SB_GETTEXTLENGTHW, nPane, 0) : 0;
    if (pType) *pType = HIWORD(lenAndType);
    return LOWORD(lenAndType);
}

// Symbol: ?AddString@CToolBarCtrl@@QEAAHI@Z
extern "C" int MS_ABI impl__AddString_CToolBarCtrl__QEAAHI_Z(CWnd* pThis, UINT nStringID) {
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    return hwnd ? static_cast<int>(::SendMessageW(hwnd, TB_ADDSTRINGW, reinterpret_cast<WPARAM>(AfxGetResourceHandle()), nStringID)) : -1;
}

// Symbol: ?Create@CToolBarCtrl@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CToolBarCtrl__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
    ::InitCommonControlsEx(&icc);
    return pThis->CreateEx(0, TOOLBARCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}

// Symbol: ?CreateEx@CToolBarCtrl@@QEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CToolBarCtrl__QEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwExStyle, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_BAR_CLASSES };
    ::InitCommonControlsEx(&icc);
    return pThis->CreateEx(dwExStyle, TOOLBARCLASSNAMEW, L"", dwStyle,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}

// Symbol: ?GetDropTarget@CToolBarCtrl@@QEBAJPEAPEAUIDropTarget@@@Z
extern "C" long MS_ABI impl__GetDropTarget_CToolBarCtrl__QEBAJPEAPEAUIDropTarget___Z(
    const CWnd* pThis, IDropTarget** ppDropTarget) {
    if (ppDropTarget) *ppDropTarget = nullptr;
    HWND hwnd = pThis ? pThis->GetSafeHwnd() : nullptr;
    return hwnd ? static_cast<long>(::SendMessageW(hwnd, TB_GETOBJECT, reinterpret_cast<WPARAM>(&IID_IDropTarget), reinterpret_cast<LPARAM>(ppDropTarget))) : E_FAIL;
}

namespace {
struct DockSiteCoreState {
    CWnd* parent = nullptr;
    CRect recentRect = CRect(0, 0, 0, 0);
    int rowCount = 0;
    unsigned int layoutRevision = 0;
};

auto& g_paneCoreStateMutex = openmfc::docking_state::PaneCoreStateMutex();
auto& g_paneCoreState = openmfc::docking_state::PaneCoreStates();
std::unordered_map<const void*, DockSiteCoreState> g_dockSiteCoreState;

static CRect NormalizeRect(const RECT* rect, int fallbackWidth = 200, int fallbackHeight = 120) {
    if (rect != nullptr) {
        CRect out(*rect);
        if (out.Width() <= 0) out.right = out.left + fallbackWidth;
        if (out.Height() <= 0) out.bottom = out.top + fallbackHeight;
        return out;
    }
    return CRect(0, 0, fallbackWidth, fallbackHeight);
}

static CRect NormalizeRect(const CRect* rect, int fallbackWidth = 200, int fallbackHeight = 120) {
    if (rect != nullptr) {
        CRect out(*rect);
        if (out.Width() <= 0) out.right = out.left + fallbackWidth;
        if (out.Height() <= 0) out.bottom = out.top + fallbackHeight;
        return out;
    }
    return CRect(0, 0, fallbackWidth, fallbackHeight);
}

static BOOL TryCreatePaneWindow(CWnd* paneWnd, const wchar_t* className, DWORD style, const CRect& rect, CWnd* parentWnd, UINT id, CCreateContext* context) {
    if (paneWnd == nullptr || parentWnd == nullptr) return FALSE;
    if (paneWnd->GetSafeHwnd() != nullptr) return TRUE;
    const wchar_t* useClass = className != nullptr ? className : L"STATIC";
    const DWORD useStyle = style != 0 ? style : (WS_CHILD | WS_VISIBLE);
    return paneWnd->Create(useClass, L"", useStyle, *static_cast<const RECT*>(rect), parentWnd, id, context);
}
} // namespace

// Symbol: ?CreateEx@CBasePane@@UEAAHKPEB_W0KAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CBasePane__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CBasePane* pThis, unsigned long dwStyleEx, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, unsigned int nID, unsigned long, CCreateContext* pContext) {
    (void)dwStyleEx;
    (void)lpszWindowName;
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        PaneCoreState& state = g_paneCoreState[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 32;
        if (state.minSize.cy <= 0) state.minSize.cy = 32;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, nID, pContext);
}

// Symbol: ?ShowPane@CBasePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int) {
    if (pThis == nullptr) return;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        g_paneCoreState[pThis].visible = bShow ? TRUE : FALSE;
    }
    if (pThis->GetSafeHwnd() != nullptr) {
        pThis->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
    }
}

// Symbol: ?GetRuntimeClass@CBasePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBasePane__UEBAPEAUCRuntimeClass__XZ(const CBasePane*) {
    return CBasePane::GetThisClass();
}

// Symbol: ?GetThisClass@CBasePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ() {
    return CBasePane::GetThisClass();
}

// Symbol: ?CreateObject@CPane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPane__SAPEAVCObject__XZ() {
    return new CPane();
}

// Symbol: ?GetRuntimeClass@CPane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPane__UEBAPEAUCRuntimeClass__XZ(const CPane*) {
    return CPane::GetThisClass();
}

// Symbol: ?GetThisClass@CPane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ() {
    return CPane::GetThisClass();
}

// Symbol: ?Create@CPane@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CPane* pThis, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect,
    CWnd* pParentWnd, unsigned int nID, unsigned long, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        PaneCoreState& state = g_paneCoreState[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        state.canFloat = TRUE;
        if (state.minSize.cx <= 0) state.minSize.cx = 64;
        if (state.minSize.cy <= 0) state.minSize.cy = 64;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, nID, pContext);
}

// Symbol: ?CreateEx@CPane@@UEAAHKPEB_WKAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CPane__UEAAHKPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CPane* pThis, unsigned long, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect,
    CWnd* pParentWnd, unsigned int nID, unsigned long cbStyle, CCreateContext* pContext) {
    return impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
        pThis, lpszClassName, dwStyle, rect, pParentWnd, nID, cbStyle, pContext);
}

// Symbol: ?Dock@CPane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int) {
    if (pThis == nullptr || pTargetBar == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[pThis];
    state.parent = static_cast<CWnd*>(pTargetBar);
    state.recentRect = NormalizeRect(lpRect);
    state.tabbed = FALSE;
    return TRUE;
}

// Symbol: ?DockPane@CPane@@UEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockPane_CPane__UEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CPane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod) {
    return impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        pThis, pTargetBar, lpRect, dockMethod);
}

// Symbol: ?OnBeforeFloat@CPane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(CPane* pThis, CRect& rect, int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[pThis].recentRect = rect;
    return TRUE;
}

// Symbol: ?OnAfterFloat@CPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnAfterFloat_CPane__UEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_paneCoreState[pThis].visible = TRUE;
}

// Symbol: ?CalcRecentDockedRect@CPane@@QEAAXXZ
extern "C" void MS_ABI impl__CalcRecentDockedRect_CPane__QEAAXXZ(CPane* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[pThis];
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, 200, 120);
    }
}

// Symbol: ?AdjustSizeImmediate@CPane@@UEAAXH@Z
extern "C" void MS_ABI impl__AdjustSizeImmediate_CPane__UEAAXH_Z(CPane* pThis, int nLength) {
    if (pThis == nullptr || nLength <= 0) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[pThis];
    if (state.minSize.cx < nLength) state.minSize.cx = nLength;
    if (state.minSize.cy < nLength) state.minSize.cy = nLength;
}

// Symbol: ?CreateObject@CDockablePane@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDockablePane__SAPEAVCObject__XZ() {
    return new CDockablePane();
}

// Symbol: ?GetRuntimeClass@CDockablePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockablePane__UEBAPEAUCRuntimeClass__XZ(const CDockablePane*) {
    return CDockablePane::GetThisClass();
}

// Symbol: ?GetThisClass@CDockablePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ() {
    return CDockablePane::GetThisClass();
}

// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect, 240, 140);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        PaneCoreState& state = g_paneCoreState[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        state.canFloat = TRUE;
        state.canAutoHide = TRUE;
        state.hasGripper = bHasGripper ? TRUE : FALSE;
        state.tabbed = (dwTabbedStyle != 0) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 120;
        if (state.minSize.cy <= 0) state.minSize.cy = 80;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), L"STATIC", dwStyle, useRect, pParentWnd, nID, pContext);
}

// Symbol: ?Create@CDockablePane@@UEAAHPEB_WPEAVCWnd@@VCSize@@HIKKK@Z
extern "C" int MS_ABI impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__VCSize__HIKKK_Z(
    CDockablePane* pThis, const wchar_t* lpszCaption, CWnd* pParentWnd, CSize sizeDefault,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle, unsigned long dwControlBarStyle) {
    CRect rect(0, 0, sizeDefault.cx > 0 ? sizeDefault.cx : 240, sizeDefault.cy > 0 ? sizeDefault.cy : 140);
    int created = impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, *static_cast<const RECT*>(rect), bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, nullptr);
    if (created && pThis != nullptr) {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
        g_paneCoreState[pThis].minSize = sizeDefault;
    }
    return created;
}

// Symbol: ?CreateEx@CDockablePane@@UEAAHKPEB_WPEAVCWnd@@AEBUtagRECT@@HIKKKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockablePane__UEAAHKPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
    CDockablePane* pThis, unsigned long, const wchar_t* lpszCaption, CWnd* pParentWnd, const RECT& rect,
    int bHasGripper, unsigned int nID, unsigned long dwStyle, unsigned long dwTabbedStyle,
    unsigned long dwControlBarStyle, CCreateContext* pContext) {
    return impl__Create_CDockablePane__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__HIKKKPEAUCCreateContext___Z(
        pThis, lpszCaption, pParentWnd, rect, bHasGripper, nID, dwStyle, dwTabbedStyle, dwControlBarStyle, pContext);
}

// Symbol: ?Dock@CDockablePane@@MEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CDockablePane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod) {
    return impl__Dock_CPane__MEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), pTargetBar, lpRect, dockMethod);
}

// Symbol: ?OnBeforeFloat@CDockablePane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CDockablePane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
    CDockablePane* pThis, CRect& rect, int dockMethod) {
    return impl__OnBeforeFloat_CPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
        reinterpret_cast<CPane*>(pThis), rect, dockMethod);
}

// Symbol: ?DockToFrameWindow@CDockablePane@@UEAAHKPEBUtagRECT@@KPEAVCBasePane@@HH@Z
extern "C" int MS_ABI impl__DockToFrameWindow_CDockablePane__UEAAHKPEBUtagRECT__KPEAVCBasePane__HH_Z(
    CDockablePane* pThis, unsigned long, const RECT* lpRect, unsigned long, CBasePane* pTargetBar, int, int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[pThis];
    state.parent = pTargetBar != nullptr ? static_cast<CWnd*>(pTargetBar) : state.parent;
    state.recentRect = NormalizeRect(lpRect, 240, 140);
    state.tabbed = FALSE;
    return TRUE;
}

// Symbol: ?ShowPane@CDockablePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane* pThis, int bShow, int bDelay, int bActivate) {
    impl__ShowPane_CBasePane__UEAAXHHH_Z(reinterpret_cast<CBasePane*>(pThis), bShow, bDelay, bActivate);
}

static constexpr unsigned int kNoSerializationSchema = 0xFFFF;

// CDockSite: DECLARE_DYNAMIC, real MSVC sizeof=544, base CBasePane (harvested from
// mfc140u.dll). Chain via m_pfnGetBaseClass to CBasePane's exported getter so the
// IsKindOf chain reaches CObject correctly (was sizeof 0 / base CObject — wrong).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
static CRuntimeClass* AFXAPI gb_CDockSite() {
    return impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
}

static CRuntimeClass g_runtimeClassDockSite = {
    "CDockSite",
    544,
    kNoSerializationSchema,
    nullptr,
    gb_CDockSite,
    nullptr,
    nullptr
};

// Symbol: ?CreateObject@CDockSite@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDockSite__SAPEAVCObject__XZ() {
    // CDockSite has no concrete class declaration in this phase; keep factory disabled.
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CDockSite@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockSite__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_runtimeClassDockSite;
}

// Symbol: ?GetThisClass@CDockSite@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassDockSite;
}

// Symbol: ?Create@CDockSite@@UEAAHKAEBUtagRECT@@PEAVCWnd@@KPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void* pThis, unsigned long, const RECT& rect, CWnd* pParentWnd, unsigned long, CCreateContext*) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.parent = pParentWnd;
    state.recentRect = NormalizeRect(&rect, 320, 220);
    return TRUE;
}

// Symbol: ?CreateEx@CDockSite@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@KPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockSite__UEAAHKKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void* pThis, unsigned long, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned long nID, CCreateContext* pContext) {
    return impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
        pThis, dwStyle, rect, pParentWnd, nID, pContext);
}

// Symbol: ?DockPane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@PEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT___Z(
    void* pThis, CPane* pPane, int, const RECT* lpRect) {
    if (pThis == nullptr || pPane == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    g_dockSiteCoreState[pThis].recentRect = NormalizeRect(lpRect, 320, 220);
    g_paneCoreState[pPane].parent = reinterpret_cast<CWnd*>(pThis);
}

// Symbol: ?DockPaneLeftOf@CDockSite@@UEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockSite__UEAAHPEAVCPane__0_Z(void* pThis, CPane* pPaneToDock, CPane* pLeftOf) {
    if (pThis == nullptr || pPaneToDock == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    PaneCoreState& state = g_paneCoreState[pPaneToDock];
    if (pLeftOf != nullptr) {
        auto it = g_paneCoreState.find(pLeftOf);
        state.parent = (it != g_paneCoreState.end()) ? it->second.parent : reinterpret_cast<CWnd*>(pThis);
    } else {
        state.parent = reinterpret_cast<CWnd*>(pThis);
    }
    return TRUE;
}

// Symbol: ?OnSetWindowPos@CDockSite@@UEAAHPEBVCWnd@@AEBVCRect@@I@Z
extern "C" int MS_ABI impl__OnSetWindowPos_CDockSite__UEAAHPEBVCWnd__AEBVCRect__I_Z(void* pThis, const CWnd*, const CRect& rect, unsigned int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&rect, 320, 220);
    ++state.layoutRevision;
    return TRUE;
}

// Symbol: ?ResizeDockSite@CDockSite@@QEAAXHH@Z
extern "C" void MS_ABI impl__ResizeDockSite_CDockSite__QEAAXHH_Z(void* pThis, int cx, int cy) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    CRect& rect = state.recentRect;
    rect.right = rect.left + (cx > 0 ? cx : rect.Width());
    rect.bottom = rect.top + (cy > 0 ? cy : rect.Height());
    ++state.layoutRevision;
}

// Symbol: ?ResizeDockSiteByOffset@CDockSite@@IEAAXHH@Z
extern "C" void MS_ABI impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(void* pThis, int cx, int cy) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    CRect& rect = state.recentRect;
    rect.right += cx;
    rect.bottom += cy;
    if (rect.Width() <= 0) rect.right = rect.left + 1;
    if (rect.Height() <= 0) rect.bottom = rect.top + 1;
    ++state.layoutRevision;
}

// Symbol: ?ResizeRow@CDockSite@@QEAAHPEAVCDockingPanesRow@@HH@Z
extern "C" int MS_ABI impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(void*, void*, int, int) {
    return TRUE;
}

// Symbol: ?ShowPane@CDockSite@@UEAAHPEAVCBasePane@@HHH@Z
extern "C" int MS_ABI impl__ShowPane_CDockSite__UEAAHPEAVCBasePane__HHH_Z(void* pThis, CBasePane* pPane, int bShow, int bDelay, int bActivate) {
    if (pThis == nullptr || pPane == nullptr) return FALSE;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pPane, bShow, bDelay, bActivate);
    return TRUE;
}

// Symbol: ?AdjustDockingLayout@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockSite__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&state.recentRect, 320, 220);
    ++state.layoutRevision;
}

// Symbol: ?AdjustLayout@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CDockSite__UEAAXXZ(void* pThis) {
    impl__AdjustDockingLayout_CDockSite__UEAAXXZ(pThis);
}

// Symbol: ?FixupVirtualRects@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__FixupVirtualRects_CDockSite__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&state.recentRect, 320, 220);
    ++state.layoutRevision;
}

// Symbol: ?OnInsertRow@CDockSite@@UEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__OnInsertRow_CDockSite__UEAAXPEAU__POSITION___Z(void* pThis, void*) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    ++state.rowCount;
    ++state.layoutRevision;
}

// Symbol: ?OnRemoveRow@CDockSite@@UEAAXPEAU__POSITION@@H@Z
extern "C" void MS_ABI impl__OnRemoveRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* pThis, void*, int) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    if (state.rowCount > 0) --state.rowCount;
    ++state.layoutRevision;
}

namespace {
class CFrameImpl;
struct FrameImplState {
    CFrameWnd* ownerFrame = nullptr;
    void* menuBar = nullptr;
    CRuntimeClass* visualManagerClass = nullptr;
};

std::mutex g_featurePackStateMutex;
std::unordered_map<CFrameImpl*, FrameImplState> g_frameImplStates;
std::unordered_map<CMDIChildWndEx*, std::wstring> g_taskbarTabText;

CRuntimeClass g_cmdiClientAreaRuntimeClass = {
    "CMDIClientAreaWnd",
    sizeof(void*),
    0xFFFF,
    nullptr,
    nullptr,
    &CWnd::classCWnd,
    nullptr
};
} // namespace

// Symbol: ??0CFrameImpl@@QEAA@PEAVCFrameWnd@@@Z
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    g_frameImplStates.insert_or_assign(frameImpl, FrameImplState{pFrame, nullptr, manager != nullptr ? manager->GetRuntimeClass() : nullptr});
    return pThis;
}

// Symbol: ??1CFrameImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis) {
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    (void)g_frameImplStates.erase(frameImpl);
}

// Symbol: ??0CMDIClientAreaWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDIClientAreaWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1CMDIClientAreaWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CMDIClientAreaWnd__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CreateObject@CFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CFrameWndEx__SAPEAVCObject__XZ() {
    return new CFrameWndEx();
}

// Symbol: ?GetThisClass@CFrameWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CFrameWndEx::classCFrameWndEx;
}

// Symbol: ?GetRuntimeClass@CFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFrameWndEx__UEBAPEAUCRuntimeClass__XZ(CFrameWndEx* pThis) {
    return pThis ? &CFrameWndEx::classCFrameWndEx : nullptr;
}

// Symbol: ?CreateObject@CMDIFrameWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIFrameWndEx__SAPEAVCObject__XZ() {
    return new CMDIFrameWndEx();
}

// Symbol: ?GetThisClass@CMDIFrameWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CMDIFrameWndEx::classCMDIFrameWndEx;
}

// Symbol: ?GetRuntimeClass@CMDIFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIFrameWndEx__UEBAPEAUCRuntimeClass__XZ(CMDIFrameWndEx* pThis) {
    return pThis ? &CMDIFrameWndEx::classCMDIFrameWndEx : nullptr;
}

// Symbol: ?CreateObject@CMDIChildWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWndEx__SAPEAVCObject__XZ() {
    return new CMDIChildWndEx();
}

// Symbol: ?GetThisClass@CMDIChildWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CMDIChildWndEx::classCMDIChildWndEx;
}

// Symbol: ?GetRuntimeClass@CMDIChildWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIChildWndEx__UEBAPEAUCRuntimeClass__XZ(CMDIChildWndEx* pThis) {
    return pThis ? &CMDIChildWndEx::classCMDIChildWndEx : nullptr;
}

// Symbol: ?GetThisClass@CMDIClientAreaWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIClientAreaWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_cmdiClientAreaRuntimeClass;
}

// Symbol: ?GetRuntimeClass@CMDIClientAreaWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIClientAreaWnd__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    return pThis ? &g_cmdiClientAreaRuntimeClass : nullptr;
}

// Symbol: ?LoadFrame@CMDIFrameWndEx@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__LoadFrame_CMDIFrameWndEx__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CMDIFrameWndEx* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return pThis ? pThis->CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext) : FALSE;
}

// Symbol: ?EnableDocking@CMDIFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_CMDIFrameWndEx__QEAAHK_Z(CMDIFrameWndEx* pThis, DWORD dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    pThis->CFrameWnd::EnableDocking(dwDockStyle);
    return TRUE;
}

// Symbol: ?EnableDocking@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_CMDIChildWndEx__QEAAHK_Z(CMDIChildWndEx* pThis, DWORD dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    pThis->CFrameWnd::EnableDocking(dwDockStyle);
    return TRUE;
}

// Symbol: ?OnShowPopupMenu@CFrameWndEx@@UEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(CFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
    return FALSE;
}

// Symbol: ?OnClosePopupMenu@CFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(CFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
}

// Symbol: ?OnShowPanes@CFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_CFrameWndEx__UEAAHH_Z(CFrameWndEx* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return TRUE;
}

// Symbol: ?OnToolbarCreateNew@CFrameWndEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_CFrameWndEx__QEAA_J_K_J_Z(CFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?OnShowPopupMenu@CMDIFrameWndEx@@UEAAHPEAVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CMDIFrameWndEx__UEAAHPEAVCMFCPopupMenu___Z(CMDIFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
    return FALSE;
}

// Symbol: ?OnClosePopupMenu@CMDIFrameWndEx@@UEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__OnClosePopupMenu_CMDIFrameWndEx__UEAAXPEAVCMFCPopupMenu___Z(CMDIFrameWndEx* pThis, void* pPopup) {
    (void)pThis;
    (void)pPopup;
}

// Symbol: ?OnShowPanes@CMDIFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_CMDIFrameWndEx__UEAAHH_Z(CMDIFrameWndEx* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return TRUE;
}

// Symbol: ?OnToolbarCreateNew@CMDIFrameWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnToolbarCreateNew_CMDIFrameWndEx__IEAA_J_K_J_Z(CMDIFrameWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?OnShowMDITabContextMenu@CMDIFrameWndEx@@UEAAHVCPoint@@KH@Z
extern "C" int MS_ABI impl__OnShowMDITabContextMenu_CMDIFrameWndEx__UEAAHVCPoint__KH_Z(
    CMDIFrameWndEx* pThis, const CPoint& point, DWORD dwAllowedItems, int bDrop) {
    (void)pThis;
    (void)point;
    (void)dwAllowedItems;
    (void)bDrop;
    return FALSE;
}

// Symbol: ?RegisterTaskbarTab@CMDIChildWndEx@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pTabWnd) {
    (void)pThis;
    (void)pTabWnd;
}

// Symbol: ?UnregisterTaskbarTab@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(CMDIChildWndEx* pThis, int bCheckRegisteredTab) {
    (void)pThis;
    (void)bCheckRegisteredTab;
}

// Symbol: ?SetTaskbarTabActive@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabActive_CMDIChildWndEx__QEAAXXZ(CMDIChildWndEx* pThis) {
    (void)pThis;
}

// Symbol: ?SetTaskbarTabOrder@CMDIChildWndEx@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pWndInsertBefore) {
    (void)pThis;
    (void)pWndInsertBefore;
}

// Symbol: ?SetTaskbarTabProperties@CMDIChildWndEx@@QEAAXK@Z
extern "C" void MS_ABI impl__SetTaskbarTabProperties_CMDIChildWndEx__QEAAXK_Z(CMDIChildWndEx* pThis, DWORD dwFlags) {
    (void)pThis;
    (void)dwFlags;
}

// Symbol: ?SetTaskbarTabText@CMDIChildWndEx@@MEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTaskbarTabText_CMDIChildWndEx__MEAAXPEB_W_Z(CMDIChildWndEx* pThis, const wchar_t* lpszTabText) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_taskbarTabText[pThis] = lpszTabText ? lpszTabText : L"";
}

// Symbol: ?SetTaskbarThumbnailClipRect@CMDIChildWndEx@@MEAAHVCRect@@@Z
extern "C" int MS_ABI impl__SetTaskbarThumbnailClipRect_CMDIChildWndEx__MEAAHVCRect___Z(CMDIChildWndEx* pThis, const CRect& rectClip) {
    (void)pThis;
    (void)rectClip;
    return TRUE;
}

// Symbol: ?CanShowOnMDITabs@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(CMDIChildWndEx* pThis) {
    (void)pThis;
    return TRUE;
}

// Symbol: ?OnShowPopupMenu@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@PEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
    void* pThis, void* pPopupMenu, CFrameWnd* pWnd) {
    (void)pThis;
    (void)pPopupMenu;
    (void)pWnd;
    return FALSE;
}

// Symbol: ?OnShowCustomizePane@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@I@Z
extern "C" int MS_ABI impl__OnShowCustomizePane_CFrameImpl__IEAAHPEAVCMFCPopupMenu__I_Z(
    void* pThis, void* pMenuPopup, UINT uiToolbarID) {
    (void)pThis;
    (void)pMenuPopup;
    (void)uiToolbarID;
    return FALSE;
}

// Symbol: ?OnCloseFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCloseFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates.erase(frameImpl);
}

// Symbol: ?OnLoadFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnLoadFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    FrameImplState& state = g_frameImplStates[frameImpl];
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
}

// Symbol: ?OnChangeVisualManager@CFrameImpl@@QEAAXXZ
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }

    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    CFrameWnd* ownerFrame = nullptr;
    CWnd* menuBarWnd = nullptr;

    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        FrameImplState& state = g_frameImplStates[frameImpl];
        ownerFrame = state.ownerFrame;
        menuBarWnd = reinterpret_cast<CWnd*>(state.menuBar);
        CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
        state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
    }

    if (ownerFrame != nullptr && ownerFrame->GetSafeHwnd() != nullptr) {
        ownerFrame->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
    if (menuBarWnd != nullptr && menuBarWnd->GetSafeHwnd() != nullptr) {
        menuBarWnd->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
}

// Symbol: ?SetMenuBar@CFrameImpl@@IEAAXPEAVCMFCMenuBar@@@Z
extern "C" void MS_ABI impl__SetMenuBar_CFrameImpl__IEAAXPEAVCMFCMenuBar___Z(void* pThis, void* pMenuBar) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates[frameImpl].menuBar = pMenuBar;
}

// Symbol: ?OnUpdateTabs@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateTabs_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}

// Symbol: ?UpdateTabs@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible) {
    (void)pThis;
    (void)bSetActiveTabVisible;
}

// Symbol: ?SetTaskbarTabOrder@CMDIClientAreaWnd@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIClientAreaWnd__QEAAXXZ(void* pThis) {
    (void)pThis;
}

namespace {
std::mutex g_statusBarTextMutex;
std::unordered_map<const CMFCStatusBar*, std::unordered_map<int, std::wstring>> g_statusBarPaneText;

std::mutex g_captionBarTextMutex;
struct CaptionBarState {
    std::wstring text;
    int alignment = 0;
    int layoutVersion = 0;
};
std::unordered_map<const CMFCCaptionBar*, CaptionBarState> g_captionBarText;

static CString GetStoredPaneText(const CMFCStatusBar* pThis, int nIndex) {
    std::lock_guard<std::mutex> lock(g_statusBarTextMutex);
    auto itBar = g_statusBarPaneText.find(pThis);
    if (itBar == g_statusBarPaneText.end()) {
        return CString();
    }

    auto itText = itBar->second.find(nIndex);
    if (itText == itBar->second.end()) {
        return CString();
    }

    return CString(itText->second.c_str());
}
} // namespace

// Symbol: ?SetPaneText@CMFCStatusBar@@UEAAHHPEB_WH@Z
extern "C" int MS_ABI impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(
    CMFCStatusBar* pThis, int nIndex, const wchar_t* lpszNewText, int bUpdate) {
    if (!pThis || nIndex < 0) {
        return FALSE;
    }

    {
        std::lock_guard<std::mutex> lock(g_statusBarTextMutex);
        g_statusBarPaneText[pThis][nIndex] = lpszNewText ? lpszNewText : L"";
    }

    return (int)static_cast<CStatusBar*>(pThis)->SetPaneText(nIndex, lpszNewText, bUpdate);
}

// Symbol: ?GetPaneText@CMFCStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetPaneText_CMFCStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* __ret, const CMFCStatusBar* pThis, int nIndex) {
    if (!__ret) {
        return;
    }

    CString text;
    if (pThis) {
        text = static_cast<const CStatusBar*>(pThis)->GetPaneText(nIndex);
    }

    if (text.IsEmpty() && pThis) {
        text = GetStoredPaneText(pThis, nIndex);
    }

    new(__ret) CString(text);
}

// Symbol: ?GetPaneText@CMFCStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CMFCStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCStatusBar* pThis, int nIndex, CString* rString) {
    if (!rString) {
        return;
    }

    if (pThis) {
        static_cast<const CStatusBar*>(pThis)->GetPaneText(nIndex, *rString);
    } else {
        *rString = CString();
    }

    if (rString->IsEmpty() && pThis) {
        *rString = GetStoredPaneText(pThis, nIndex);
    }
}

// Symbol: ?SetText@CMFCCaptionBar@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@W4BarElementAlignment@1@@Z
extern "C" void MS_ABI impl__SetText_CMFCCaptionBar__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4BarElementAlignment_1__Z(
    CMFCCaptionBar* pThis, const CString* pText, int nAlignment) {
    if (!pThis) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_captionBarTextMutex);
    auto& state = g_captionBarText[pThis];
    state.text = pText ? (const wchar_t*)(*pText) : L"";
    state.alignment = nAlignment;
}

// Symbol: ?AdjustLayout@CMFCCaptionBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCCaptionBar__MEAAXXZ(CMFCCaptionBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_captionBarTextMutex);
    ++g_captionBarText[pThis].layoutVersion;
}

// Symbol: ?RecalcLayout@CMFCCaptionBar@@MEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCCaptionBar__MEAAXXZ(CMFCCaptionBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_captionBarTextMutex);
    ++g_captionBarText[pThis].layoutVersion;
}

namespace {
struct PaneState {
    std::vector<void*> panes;
    CWnd* parent = nullptr;
    CRect recentRect = CRect(0, 0, 0, 0);
    CSize minSize = CSize(0, 0);
    BOOL rollUp = FALSE;
};

struct AutoHideButtonState {
    BOOL highlighted;
    BOOL horizontal;
};

static std::mutex g_wave2Mutex;
static constexpr int kPaneFrameDefaultWidth = 240;
static constexpr int kPaneFrameDefaultHeight = 140;
static constexpr int kPaneFrameDefaultMinWidth = 120;
static constexpr int kPaneFrameDefaultMinHeight = 80;
static constexpr int kPaneFrameCaptionHeight = 22;
static constexpr int kPaneFrameThinBorder = 1;
static constexpr int kPaneFrameBorder = 2;
static constexpr int kPaneFrameWideBorder = 4;
static constexpr int kPaneDividerThickness = 8;
static constexpr int kPaneDividerUnitWidth = 120;
static std::unordered_map<void*, PaneState> g_framePanes;
static std::unordered_map<void*, PaneState> g_multiFramePanes;
static std::unordered_map<void*, PaneState> g_containerPanes;
static std::unordered_map<void*, PaneState> g_containerManagerPanes;
static std::unordered_map<void*, PaneState> g_dividerPanes;
static std::unordered_map<void*, PaneState> g_autoHideBarWindows;
static std::unordered_map<void*, void*> g_paneToContainer;
static std::unordered_map<void*, void*> g_paneToAutoHideButton;
static std::unordered_map<void*, std::unique_ptr<int>> g_autoHideButtonStorage;
static std::unordered_map<void*, AutoHideButtonState> g_autoHideButtons;

static void AddUniquePane(PaneState& state, void* pane) {
    if (!pane) return;
    if (std::find(state.panes.begin(), state.panes.end(), pane) == state.panes.end()) {
        state.panes.push_back(pane);
    }
    if (state.minSize.cx <= 0) state.minSize.cx = kPaneFrameDefaultMinWidth;
    if (state.minSize.cy <= 0) state.minSize.cy = kPaneFrameDefaultMinHeight;
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    }
}

static void RemovePane(PaneState& state, void* pane) {
    if (!pane) return;
    state.panes.erase(std::remove(state.panes.begin(), state.panes.end(), pane), state.panes.end());
}

static void* FirstPane(const PaneState& state) {
    return state.panes.empty() ? nullptr : state.panes.front();
}
} // namespace

// Symbol: ??0CMFCAutoHideBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAutoHideBar__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideBarWindows[pThis] = {};
    return pThis;
}

// Symbol: ??0CMFCAutoHideButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAutoHideButton__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons[pThis] = { FALSE, FALSE };
    return pThis;
}

// Symbol: ??0CMultiPaneFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMultiPaneFrameWnd__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_multiFramePanes[pThis] = {};
    return pThis;
}

// Symbol: ??0CPaneContainer@@QEAA@PEAVCPaneContainerManager@@PEAVCDockablePane@@1PEAVCPaneDivider@@@Z
extern "C" void* MS_ABI impl___0CPaneContainer__QEAA_PEAVCPaneContainerManager__PEAVCDockablePane__1PEAVCPaneDivider___Z(
    void* pThis, void* pManager, void* pFirstPane, void* pSecondPane, void* pDivider) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_containerPanes[pThis];
    AddUniquePane(state, pFirstPane);
    AddUniquePane(state, pSecondPane);
    if (pManager) {
        AddUniquePane(g_containerManagerPanes[pManager], pFirstPane);
        AddUniquePane(g_containerManagerPanes[pManager], pSecondPane);
    }
    if (pDivider) {
        AddUniquePane(g_dividerPanes[pDivider], pFirstPane);
        AddUniquePane(g_dividerPanes[pDivider], pSecondPane);
    }
    if (pFirstPane) g_paneToContainer[pFirstPane] = pThis;
    if (pSecondPane) g_paneToContainer[pSecondPane] = pThis;
    return pThis;
}

// Symbol: ??0CPaneContainerManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneContainerManager__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerManagerPanes[pThis] = {};
    return pThis;
}

// Symbol: ??0CPaneDivider@@QEAA@HPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CPaneDivider__QEAA_HPEAVCWnd___Z(void* pThis, int, void*) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_dividerPanes[pThis] = {};
    return pThis;
}

// Symbol: ??0CPaneDivider@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneDivider__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_dividerPanes[pThis] = {};
    return pThis;
}

// Symbol: ??1CMFCAutoHideBar@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCAutoHideBar__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    auto it = g_autoHideBarWindows.find(pThis);
    if (it != g_autoHideBarWindows.end()) {
        for (void* pane : it->second.panes) {
            auto buttonIt = g_paneToAutoHideButton.find(pane);
            if (buttonIt != g_paneToAutoHideButton.end()) {
                g_autoHideButtons.erase(buttonIt->second);
                g_paneToAutoHideButton.erase(buttonIt);
            }
            g_autoHideButtonStorage.erase(pane);
        }
        g_autoHideBarWindows.erase(it);
    }
    return pThis;
}

// Symbol: ??1CMFCAutoHideButton@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCAutoHideButton__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons.erase(pThis);
    return pThis;
}

// Symbol: ??1CMultiPaneFrameWnd@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMultiPaneFrameWnd__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_multiFramePanes.erase(pThis);
    return pThis;
}

// Symbol: ??1CPaneContainer@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneContainer__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerPanes.erase(pThis);
    return pThis;
}

// Symbol: ??1CPaneContainerManager@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneContainerManager__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerManagerPanes.erase(pThis);
    return pThis;
}

// Symbol: ??1CPaneDivider@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneDivider__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_dividerPanes.erase(pThis);
    return pThis;
}

// Symbol: ?AddAutoHideWindow@CMFCAutoHideBar@@QEAAPEAVCMFCAutoHideButton@@PEAVCDockablePane@@K@Z
extern "C" void* MS_ABI impl__AddAutoHideWindow_CMFCAutoHideBar__QEAAPEAVCMFCAutoHideButton__PEAVCDockablePane__K_Z(
    void* pThis, void* pPane, unsigned long dwAlignment) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)dwAlignment;
    AddUniquePane(g_autoHideBarWindows[pThis], pPane);
    if (pPane && g_paneToAutoHideButton.find(pPane) == g_paneToAutoHideButton.end()) {
        auto storage = std::make_unique<int>(0);
        void* buttonPtr = storage.get();
        g_autoHideButtonStorage[pPane] = std::move(storage);
        g_paneToAutoHideButton[pPane] = buttonPtr;
        g_autoHideButtons[buttonPtr] = { FALSE, FALSE };
    }
    return pPane ? g_paneToAutoHideButton[pPane] : nullptr;
}

// Symbol: ?ButtonFromAutoHideWindow@CMFCAutoHideBar@@IEAAPEAVCMFCAutoHideButton@@PEAVCDockablePane@@@Z
extern "C" void* MS_ABI impl__ButtonFromAutoHideWindow_CMFCAutoHideBar__IEAAPEAVCMFCAutoHideButton__PEAVCDockablePane___Z(
    void*, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    auto it = g_paneToAutoHideButton.find(pPane);
    return it == g_paneToAutoHideButton.end() ? nullptr : it->second;
}

// Symbol: ?ButtonFromPoint@CMFCAutoHideBar@@IEAAPEAVCMFCAutoHideButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__ButtonFromPoint_CMFCAutoHideBar__IEAAPEAVCMFCAutoHideButton__VCPoint___Z(void* pThis, CPoint) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    void* pane = FirstPane(g_autoHideBarWindows[pThis]);
    auto it = g_paneToAutoHideButton.find(pane);
    return it == g_paneToAutoHideButton.end() ? nullptr : it->second;
}

// Symbol: ?GetFirstAHWindow@CMFCAutoHideBar@@QEAAPEAVCDockablePane@@XZ
extern "C" void* MS_ABI impl__GetFirstAHWindow_CMFCAutoHideBar__QEAAPEAVCDockablePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_autoHideBarWindows[pThis]);
}

// Symbol: ?GetVisibleCount@CMFCAutoHideBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetVisibleCount_CMFCAutoHideBar__QEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return static_cast<int>(g_autoHideBarWindows[pThis].panes.size());
}

// Symbol: ?HighlightButton@CMFCAutoHideButton@@UEAAXH@Z
extern "C" void MS_ABI impl__HighlightButton_CMFCAutoHideButton__UEAAXH_Z(void* pThis, int bHighlight) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideButtons[pThis].highlighted = bHighlight ? TRUE : FALSE;
}

// Symbol: ?IsHorizontal@CMFCAutoHideButton@@QEBAHXZ
extern "C" int MS_ABI impl__IsHorizontal_CMFCAutoHideButton__QEBAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_autoHideButtons[pThis].horizontal;
}

// Symbol: ?GetSize@CMFCAutoHideButton@@QEBA?AVCSize@@XZ
extern "C" void MS_ABI impl__GetSize_CMFCAutoHideButton__QEBA_AVCSize__XZ(CSize* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const AutoHideButtonState& s = g_autoHideButtons[pThis];
    pRet->cx = s.highlighted ? 96 : 80;
    pRet->cy = 22;
}

// Symbol: ?GetTextSize@CMFCAutoHideButton@@UEBA?AVCSize@@XZ
extern "C" void MS_ABI impl__GetTextSize_CMFCAutoHideButton__UEBA_AVCSize__XZ(CSize* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const AutoHideButtonState& s = g_autoHideButtons[pThis];
    pRet->cx = s.highlighted ? 88 : 72;
    pRet->cy = 16;
}

// Symbol: ?AddPane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddPane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_multiFramePanes[pThis], pPane);
}

// Symbol: ?RemovePane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
extern "C" void MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int, int) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    RemovePane(g_multiFramePanes[pThis], pPane);
}

// Symbol: ?GetPane@CMultiPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetPane_CMultiPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_multiFramePanes[pThis]);
}

// Symbol: ?GetCaptionText@CMultiPaneFrameWnd@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetCaptionText_CMultiPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pRet->Format(L"Pane group (%d)", static_cast<int>(g_multiFramePanes[pThis].panes.size()));
}

// Symbol: ?CalcExpectedDockedRect@CMultiPaneFrameWnd@@UEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CMultiPaneFrameWnd__UEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void*, void*, CPoint, CRect* pRect, int* pnAlign, void** ppBar) {
    if (pRect) pRect->SetRectEmpty();
    if (pnAlign) *pnAlign = 0;
    if (ppBar) *ppBar = nullptr;
}

// Symbol: ?Create@CPaneFrameWnd@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@PEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CPaneFrameWnd__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
    void* pThis, const wchar_t* lpszClassName, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        PaneState& state = g_framePanes[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        if (state.minSize.cx <= 0) state.minSize.cx = kPaneFrameDefaultMinWidth;
        if (state.minSize.cy <= 0) state.minSize.cy = kPaneFrameDefaultMinHeight;
        state.rollUp = FALSE;
    }
    return TryCreatePaneWindow(reinterpret_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, 0, pContext);
}

// Symbol: ?AddPane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    AddUniquePane(state, pPane);
    state.rollUp = FALSE;
}

// Symbol: ?RemovePane@CPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
extern "C" void MS_ABI impl__RemovePane_CPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int, int) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    RemovePane(state, pPane);
    if (state.panes.empty()) {
        state.rollUp = FALSE;
    }
}

// Symbol: ?AdjustLayout@CPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CPaneFrameWnd__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_framePanes[pThis];
    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    }
    if (state.minSize.cx <= 0) state.minSize.cx = kPaneFrameDefaultMinWidth;
    if (state.minSize.cy <= 0) state.minSize.cy = kPaneFrameDefaultMinHeight;
    if (state.rollUp) {
        state.recentRect.bottom = state.recentRect.top + kPaneFrameCaptionHeight + kPaneFrameBorder;
    } else {
        if (state.recentRect.Width() < state.minSize.cx) {
            state.recentRect.right = state.recentRect.left + state.minSize.cx;
        }
        if (state.recentRect.Height() < state.minSize.cy + kPaneFrameCaptionHeight) {
            state.recentRect.bottom = state.recentRect.top + state.minSize.cy + kPaneFrameCaptionHeight;
        }
    }
    state.rollUp = state.recentRect.Height() <= (kPaneFrameCaptionHeight + kPaneFrameBorder);
}

// Symbol: ?CalcBorderSize@CPaneFrameWnd@@UEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__CalcBorderSize_CPaneFrameWnd__UEBAXAEAVCRect___Z(void* pThis, CRect* pRect) {
    if (!pRect) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    const int border = state.panes.empty() ? kPaneFrameThinBorder : kPaneFrameBorder;
    pRect->SetRect(border, border, border, border);
}

// Symbol: ?CalcMinSize@CPaneFrameWnd@@MEAAXAEAVCSize@@PEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(void* pThis, CSize* pSize, MINMAXINFO*) {
    if (!pSize) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    const int border = state.panes.empty() ? kPaneFrameBorder : kPaneFrameWideBorder;
    pSize->cx = std::max(80, state.minSize.cx) + border;
    pSize->cy = std::max(40, state.minSize.cy) + kPaneFrameCaptionHeight + border;
}

// Symbol: ?GetCaptionRect@CPaneFrameWnd@@UEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetCaptionRect_CPaneFrameWnd__UEBAXAEAVCRect___Z(void* pThis, CRect* pRect) {
    if (!pRect) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    const CRect frameRect = (state.recentRect.Width() > 0 && state.recentRect.Height() > 0)
                                ? state.recentRect
                                : CRect(0, 0, kPaneFrameDefaultWidth, kPaneFrameDefaultHeight);
    pRect->left = frameRect.left;
    pRect->top = frameRect.top;
    pRect->right = frameRect.right;
    pRect->bottom = frameRect.top + std::min(kPaneFrameCaptionHeight, frameRect.Height());
}

// Symbol: ?GetCaptionText@CPaneFrameWnd@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetCaptionText_CPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pRet->Format(L"Pane frame (%d)", static_cast<int>(g_framePanes[pThis].panes.size()));
}

// Symbol: ?GetFirstVisiblePane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_framePanes[pThis]);
}

// Symbol: ?GetPane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_framePanes[pThis]);
}

// Symbol: ?IsRollDown@CPaneFrameWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsRollDown_CPaneFrameWnd__UEBAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_framePanes[pThis];
    return state.rollUp ? FALSE : TRUE;
}

// Symbol: ?IsRollUp@CPaneFrameWnd@@UEBAHXZ
extern "C" int MS_ABI impl__IsRollUp_CPaneFrameWnd__UEBAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_framePanes[pThis].rollUp;
}

BOOL CPaneFrameWnd::Create(const wchar_t* lpszClassName, DWORD dwStyle, const RECT& rect,
                           CWnd* pParentWnd, CCreateContext* pContext) {
    return impl__Create_CPaneFrameWnd__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__PEAUCCreateContext___Z(
        this, lpszClassName, dwStyle, rect, pParentWnd, pContext);
}

void CPaneFrameWnd::AdjustLayout() {
    impl__AdjustLayout_CPaneFrameWnd__UEAAXXZ(this);
}

void CPaneFrameWnd::CalcBorderSize(CRect& rect) const {
    impl__CalcBorderSize_CPaneFrameWnd__UEBAXAEAVCRect___Z(const_cast<CPaneFrameWnd*>(this), &rect);
}

void CPaneFrameWnd::CalcMinSize(CSize& size, MINMAXINFO* pMMI) {
    impl__CalcMinSize_CPaneFrameWnd__MEAAXAEAVCSize__PEAUtagMINMAXINFO___Z(this, &size, pMMI);
}

void CPaneFrameWnd::GetCaptionRect(CRect& rect) const {
    impl__GetCaptionRect_CPaneFrameWnd__UEBAXAEAVCRect___Z(const_cast<CPaneFrameWnd*>(this), &rect);
}

CString CPaneFrameWnd::GetCaptionText() {
    CString text;
    impl__GetCaptionText_CPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(&text, this);
    return text;
}

CWnd* CPaneFrameWnd::GetFirstVisiblePane() const {
    return reinterpret_cast<CWnd*>(
        impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(const_cast<CPaneFrameWnd*>(this)));
}

CWnd* CPaneFrameWnd::GetPane() const {
    return reinterpret_cast<CWnd*>(
        impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(const_cast<CPaneFrameWnd*>(this)));
}

BOOL CPaneFrameWnd::IsRollDown() const {
    return impl__IsRollDown_CPaneFrameWnd__UEBAHXZ(const_cast<CPaneFrameWnd*>(this));
}

BOOL CPaneFrameWnd::IsRollUp() const {
    return impl__IsRollUp_CPaneFrameWnd__UEBAHXZ(const_cast<CPaneFrameWnd*>(this));
}

// Symbol: ?AddPane@CPaneContainer@@QEAAPEAVCDockablePane@@PEAV2@@Z
extern "C" void* MS_ABI impl__AddPane_CPaneContainer__QEAAPEAVCDockablePane__PEAV2__Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_containerPanes[pThis], pPane);
    if (pPane) g_paneToContainer[pPane] = pThis;
    return pPane;
}

// Symbol: ?RemovePane@CPaneContainer@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemovePane_CPaneContainer__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    RemovePane(g_containerPanes[pThis], pPane);
    if (pPane) g_paneToContainer.erase(pPane);
}

// Symbol: ?AddPane@CPaneContainerManager@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_containerManagerPanes[pThis], pPane);
}

// Symbol: ?FindPane@CPaneContainerManager@@IEAAIVCPoint@@PEAPEAVCPane@@AEAPEAU__POSITION@@@Z
extern "C" unsigned int MS_ABI impl__FindPane_CPaneContainerManager__IEAAIVCPoint__PEAPEAVCPane__AEAPEAU__POSITION___Z(
    void* pThis, CPoint, void** ppPane, void** ppPos) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    void* foundPane = FirstPane(g_containerManagerPanes[pThis]);
    if (ppPane) *ppPane = foundPane;
    if (ppPos) *ppPos = nullptr;
    return foundPane != nullptr ? 1U : 0U;
}

// Symbol: ?FindPane@CPaneContainerManager@@IEAAIVCRect@@PEAPEAVCPane@@AEAPEAU__POSITION@@@Z
extern "C" unsigned int MS_ABI impl__FindPane_CPaneContainerManager__IEAAIVCRect__PEAPEAVCPane__AEAPEAU__POSITION___Z(
    void* pThis, const CRect&, void** ppPane, void** ppPos) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    void* foundPane = FirstPane(g_containerManagerPanes[pThis]);
    if (ppPane) *ppPane = foundPane;
    if (ppPos) *ppPos = nullptr;
    return foundPane != nullptr ? 1U : 0U;
}

// Symbol: ?FindPaneContainer@CPaneContainerManager@@UEAAPEAVCPaneContainer@@PEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pIndex) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)pThis;
    if (pIndex) *pIndex = 0;
    auto it = g_paneToContainer.find(pPane);
    return it == g_paneToContainer.end() ? nullptr : it->second;
}

// Symbol: ?GetFirstPane@CPaneContainerManager@@UEBAPEAVCBasePane@@XZ
extern "C" void* MS_ABI impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_containerManagerPanes[pThis]);
}

// Symbol: ?GetFirstVisiblePane@CPaneContainerManager@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_containerManagerPanes[pThis]);
}

// Symbol: ?AddPane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_dividerPanes[pThis];
    AddUniquePane(state, pPane);
    const int paneCount = static_cast<int>(state.panes.size());
    state.recentRect = CRect(0, 0, std::max(80, paneCount * kPaneDividerUnitWidth), kPaneDividerThickness);
}

// Symbol: ?RemovePane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_dividerPanes[pThis];
    RemovePane(state, pPane);
    if (state.panes.empty()) {
        state.recentRect.SetRectEmpty();
    } else {
        const int paneCount = static_cast<int>(state.panes.size());
        state.recentRect = CRect(0, 0, std::max(80, paneCount * kPaneDividerUnitWidth), kPaneDividerThickness);
    }
}

// Symbol: ?CheckVisibility@CPaneDivider@@UEAAHXZ
extern "C" int MS_ABI impl__CheckVisibility_CPaneDivider__UEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_dividerPanes[pThis].panes.empty() ? FALSE : TRUE;
}

// Symbol: ?FindPaneContainer@CPaneDivider@@QEAAPEAVCPaneContainer@@PEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneDivider__QEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pIndex) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)pThis;
    if (pIndex) *pIndex = 0;
    auto it = g_paneToContainer.find(pPane);
    return it == g_paneToContainer.end() ? nullptr : it->second;
}

// Symbol: ?GetFirstPane@CPaneDivider@@QEBAPEBVCBasePane@@XZ
extern "C" void* MS_ABI impl__GetFirstPane_CPaneDivider__QEBAPEBVCBasePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_dividerPanes[pThis]);
}

// Symbol: ?GetPaneDividers@CPaneDivider@@QEAAXAEAVCObList@@@Z
extern "C" void MS_ABI impl__GetPaneDividers_CPaneDivider__QEAAXAEAVCObList___Z(void* pThis, CObList* pList) {
    if (!pList) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pList->RemoveAll();
    const auto it = g_dividerPanes.find(pThis);
    if (it != g_dividerPanes.end() && pThis != nullptr) {
        pList->AddTail(reinterpret_cast<CObject*>(pThis));
    }
}

// Symbol: ?GetPanes@CPaneDivider@@QEAAXAEAVCObList@@@Z
extern "C" void MS_ABI impl__GetPanes_CPaneDivider__QEAAXAEAVCObList___Z(void* pThis, CObList* pList) {
    if (!pList) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pList->RemoveAll();
    const auto it = g_dividerPanes.find(pThis);
    if (it == g_dividerPanes.end()) return;
    for (void* pane : it->second.panes) {
        if (pane != nullptr) {
            pList->AddTail(reinterpret_cast<CObject*>(pane));
        }
    }
}

// Symbol: ?GetRootContainerRect@CPaneDivider@@QEAA?AVCRect@@XZ
extern "C" void MS_ABI impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(CRect* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_dividerPanes[pThis];
    if (state.recentRect.Width() > 0 && state.recentRect.Height() > 0) {
        *pRet = state.recentRect;
    } else {
        pRet->SetRect(0, 0, state.panes.empty() ? 0 : 240, state.panes.empty() ? 0 : 140);
    }
}

// Symbol: ?CalcFixedLayout@CPaneDivider@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CPaneDivider__UEAA_AVCSize__HH_Z(CSize* pRet, void* pThis, int, int) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_dividerPanes[pThis];
    if (state.recentRect.Width() > 0 && state.recentRect.Height() > 0) {
        pRet->cx = state.recentRect.Width();
        pRet->cy = state.recentRect.Height();
        return;
    }
    const int paneCount = static_cast<int>(state.panes.size());
    pRet->cx = paneCount > 0 ? std::max(80, 80 * paneCount) : 0;
    pRet->cy = paneCount > 0 ? kPaneDividerThickness : 0;
}

void CPaneDivider::AddPane(CDockablePane* pPane) {
    impl__AddPane_CPaneDivider__UEAAXPEAVCDockablePane___Z(this, pPane);
}

void CPaneDivider::RemovePane(CDockablePane* pPane) {
    impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(this, pPane);
}

BOOL CPaneDivider::CheckVisibility() {
    return impl__CheckVisibility_CPaneDivider__UEAAHXZ(this);
}

CPaneContainer* CPaneDivider::FindPaneContainer(CDockablePane* pBar, int& nIndex) {
    return reinterpret_cast<CPaneContainer*>(
        impl__FindPaneContainer_CPaneDivider__QEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(this, pBar, &nIndex));
}

const CBasePane* CPaneDivider::GetFirstPane() const {
    return reinterpret_cast<const CBasePane*>(
        impl__GetFirstPane_CPaneDivider__QEBAPEBVCBasePane__XZ(const_cast<CPaneDivider*>(this)));
}

void CPaneDivider::GetPaneDividers(CObList& lst) {
    impl__GetPaneDividers_CPaneDivider__QEAAXAEAVCObList___Z(this, &lst);
}

void CPaneDivider::GetPanes(CObList& lst) {
    impl__GetPanes_CPaneDivider__QEAAXAEAVCObList___Z(this, &lst);
}

CRect CPaneDivider::GetRootContainerRect() {
    CRect rect;
    impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(&rect, this);
    return rect;
}

CSize CPaneDivider::CalcFixedLayout(BOOL bStretch, BOOL bHorz) {
    CSize size;
    impl__CalcFixedLayout_CPaneDivider__UEAA_AVCSize__HH_Z(&size, this, bStretch, bHorz);
    return size;
}

//=============================================================================
// CMFCToolBarButton - static members and additional methods
//=============================================================================

namespace {
struct ComboButtonState {
    std::vector<std::pair<CString, DWORD_PTR>> items;
    int curSel = -1;
};

thread_local std::unordered_map<const CMFCToolBarComboBoxButton*, ComboButtonState> g_comboStates;
thread_local std::unordered_map<UINT, std::vector<CMFCToolBarComboBoxButton*>> g_comboByCmd;

thread_local std::unordered_map<UINT, CString> g_editContentsByCmd;
thread_local std::unordered_map<UINT, std::vector<CMFCToolBarEditBoxButton*>> g_editByCmd;

ComboButtonState& EnsureComboState(const CMFCToolBarComboBoxButton* pButton) {
    return g_comboStates[pButton];
}

const ComboButtonState* FindComboState(const CMFCToolBarComboBoxButton* pButton) {
    auto it = g_comboStates.find(pButton);
    return (it != g_comboStates.end()) ? &it->second : nullptr;
}

void RegisterComboButton(CMFCToolBarComboBoxButton* pButton) {
    if (!pButton || pButton->m_nID == 0) return;
    auto& buttons = g_comboByCmd[pButton->m_nID];
    for (CMFCToolBarComboBoxButton* existing : buttons) {
        if (existing == pButton) return;
    }
    buttons.push_back(pButton);
}

void UnregisterComboButton(CMFCToolBarComboBoxButton* pButton) {
    if (!pButton) return;
    g_comboStates.erase(pButton);
    for (auto it = g_comboByCmd.begin(); it != g_comboByCmd.end(); ) {
        auto& buttons = it->second;
        buttons.erase(std::remove(buttons.begin(), buttons.end(), pButton), buttons.end());
        if (buttons.empty()) {
            it = g_comboByCmd.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<CMFCToolBarComboBoxButton*> GetComboButtonsByCmd(UINT uiCmd) {
    auto it = g_comboByCmd.find(uiCmd);
    if (it == g_comboByCmd.end()) return {};
    return it->second;
}

int CompareComboText(const wchar_t* s1, const wchar_t* s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    return ::CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE, s1, -1, s2, -1) - 2;
}

void RegisterEditButton(CMFCToolBarEditBoxButton* pButton) {
    if (!pButton || pButton->m_nID == 0) return;
    auto& buttons = g_editByCmd[pButton->m_nID];
    for (CMFCToolBarEditBoxButton* existing : buttons) {
        if (existing == pButton) return;
    }
    buttons.push_back(pButton);
}

void UnregisterEditButton(CMFCToolBarEditBoxButton* pButton) {
    if (!pButton) return;
    for (auto it = g_editByCmd.begin(); it != g_editByCmd.end(); ) {
        auto& buttons = it->second;
        buttons.erase(std::remove(buttons.begin(), buttons.end(), pButton), buttons.end());
        if (buttons.empty()) {
            g_editContentsByCmd.erase(it->first);
            it = g_editByCmd.erase(it);
        } else {
            ++it;
        }
    }
}
} // namespace

void OpenMFC_CopyComboButtonState(CMFCToolBarComboBoxButton* dst, const CMFCToolBarComboBoxButton* src) {
    if (!dst || !src) return;
    if (const ComboButtonState* state = FindComboState(src)) {
        g_comboStates[dst] = *state;
    }
    RegisterComboButton(dst);
}

void OpenMFC_CopyEditButtonState(CMFCToolBarEditBoxButton* dst, const CMFCToolBarEditBoxButton* src) {
    if (!dst || !src) return;
    RegisterEditButton(dst);
    auto it = g_editContentsByCmd.find(src->m_nID);
    if (it != g_editContentsByCmd.end()) {
        g_editContentsByCmd[dst->m_nID] = it->second;
    }
}

// Static members - provided without // Symbol: comments so typed_stubs handles
// the MSVC-named exports. Definitions needed for C++ ODR completeness.
CString CMFCToolBarButton::m_strClipboardFormatName;
BOOL CMFCToolBarButton::m_bWrapText = TRUE;
BOOL CMFCToolBarButton::m_bUpdateImages = FALSE;

// Symbol: ?SetClipboardFormatName@CMFCToolBarButton@@SAXPEB_W@Z
void CMFCToolBarButton::SetClipboardFormatName(const wchar_t* lpszName) {
    if (lpszName && *lpszName) {
        m_strClipboardFormatName = lpszName;
    } else {
        m_strClipboardFormatName.Empty();
    }
}

extern "C" void MS_ABI impl__SetClipboardFormatName_CMFCToolBarButton__SAXPEB_W_Z(const wchar_t* p0) {
    CMFCToolBarButton::SetClipboardFormatName(p0);
}

//=============================================================================
// CMFCToolBarComboBoxButton
//=============================================================================

CMFCToolBarComboBoxButton::~CMFCToolBarComboBoxButton() {
    UnregisterComboButton(this);
}

// Symbol: ?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
INT_PTR CMFCToolBarComboBoxButton::AddItem(const wchar_t* lpszItem, DWORD_PTR dwData) {
    if (!lpszItem) return CB_ERR;
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    state.items.emplace_back(CString(lpszItem), dwData);
    return static_cast<INT_PTR>(state.items.size() - 1);
}

// Symbol: ?AddSortedItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
INT_PTR CMFCToolBarComboBoxButton::AddSortedItem(const wchar_t* lpszItem, DWORD_PTR dwData) {
    if (!lpszItem) return CB_ERR;
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    auto insertPos = state.items.begin();
    while (insertPos != state.items.end() &&
           CompareComboText(static_cast<const wchar_t*>(insertPos->first), lpszItem) <= 0) {
        ++insertPos;
    }
    const int index = static_cast<int>(insertPos - state.items.begin());
    state.items.insert(insertPos, std::make_pair(CString(lpszItem), dwData));
    if (state.curSel >= index) ++state.curSel;
    return index;
}

// Symbol: ?Compare@CMFCToolBarComboBoxButton@@UEAAHPEB_W0@Z
int CMFCToolBarComboBoxButton::Compare(const wchar_t* s1, const wchar_t* s2) {
    return CompareComboText(s1, s2);
}

// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHH@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(int nIndex) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.items.size())) return FALSE;
    state.items.erase(state.items.begin() + nIndex);
    if (state.curSel == nIndex) {
        state.curSel = -1;
        m_strText.Empty();
    } else if (state.curSel > nIndex) {
        --state.curSel;
    }
    return TRUE;
}

// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(DWORD_PTR dwData) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    for (int i = 0; i < static_cast<int>(state.items.size()); ++i) {
        if (state.items[i].second == dwData) {
            return DeleteItem(i);
        }
    }
    return FALSE;
}

// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
BOOL CMFCToolBarComboBoxButton::DeleteItem(const wchar_t* lpszText) {
    const int index = FindItem(lpszText);
    return (index != CB_ERR) ? DeleteItem(index) : FALSE;
}

// Symbol: ?FindItem@CMFCToolBarComboBoxButton@@QEBAHPEB_W@Z
int CMFCToolBarComboBoxButton::FindItem(const wchar_t* lpszText) const {
    if (!lpszText) return CB_ERR;
    const ComboButtonState* state = FindComboState(this);
    if (!state) return CB_ERR;
    for (int i = 0; i < static_cast<int>(state->items.size()); ++i) {
        if (CompareComboText(static_cast<const wchar_t*>(state->items[i].first), lpszText) == 0) {
            return i;
        }
    }
    return CB_ERR;
}

// Symbol: ?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z
const wchar_t* CMFCToolBarComboBoxButton::GetItem(int nIndex) const {
    const ComboButtonState* state = FindComboState(this);
    if (!state) return nullptr;
    if (nIndex < 0) nIndex = state->curSel;
    if (nIndex < 0 || nIndex >= static_cast<int>(state->items.size())) return nullptr;
    return state->items[nIndex].first;
}

// Symbol: ?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ
INT_PTR CMFCToolBarComboBoxButton::GetCount() const {
    const ComboButtonState* state = FindComboState(this);
    return state ? static_cast<INT_PTR>(state->items.size()) : 0;
}

// Symbol: ?GetItemData@CMFCToolBarComboBoxButton@@QEBA_KH@Z
DWORD_PTR CMFCToolBarComboBoxButton::GetItemData(int nIndex) const {
    const ComboButtonState* state = FindComboState(this);
    if (!state) return 0;
    if (nIndex < 0) nIndex = state->curSel;
    if (nIndex < 0 || nIndex >= static_cast<int>(state->items.size())) return 0;
    return state->items[nIndex].second;
}

// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(const wchar_t* lpszItem) {
    const int index = FindItem(lpszItem);
    return (index != CB_ERR) ? SelectItem(index, TRUE) : FALSE;
}

// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHHH@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(int nIndex, BOOL) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    if (nIndex < 0 || nIndex >= static_cast<int>(state.items.size())) return FALSE;
    state.curSel = nIndex;
    m_strText = state.items[nIndex].first;
    return TRUE;
}

// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
BOOL CMFCToolBarComboBoxButton::SelectItem(DWORD_PTR dwData) {
    RegisterComboButton(this);
    ComboButtonState& state = EnsureComboState(this);
    for (int i = 0; i < static_cast<int>(state.items.size()); ++i) {
        if (state.items[i].second == dwData) {
            return SelectItem(i, TRUE);
        }
    }
    return FALSE;
}

// Symbol: ?SetText@CMFCToolBarComboBoxButton@@QEAAXPEB_W@Z
void CMFCToolBarComboBoxButton::SetText(const wchar_t* lpszText) {
    RegisterComboButton(this);
    m_strText = lpszText ? lpszText : L"";
}

// Static methods
// Symbol: ?GetByCmd@CMFCToolBarComboBoxButton@@SAPEAV1@IH@Z
CMFCToolBarComboBoxButton* CMFCToolBarComboBoxButton::GetByCmd(UINT uiCmd, BOOL) {
    const auto buttons = GetComboButtonsByCmd(uiCmd);
    return buttons.empty() ? nullptr : buttons.front();
}

// Symbol: ?GetCountAll@CMFCToolBarComboBoxButton@@SAHI@Z
int CMFCToolBarComboBoxButton::GetCountAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? static_cast<int>(pButton->GetCount()) : 0;
}

// Symbol: ?GetCurSelAll@CMFCToolBarComboBoxButton@@SAHI@Z
int CMFCToolBarComboBoxButton::GetCurSelAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    if (!pButton) return CB_ERR;
    const ComboButtonState* state = FindComboState(pButton);
    return state ? state->curSel : CB_ERR;
}

// Symbol: ?GetItemAll@CMFCToolBarComboBoxButton@@SAPEB_WIH@Z
const wchar_t* CMFCToolBarComboBoxButton::GetItemAll(UINT uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? pButton->GetItem(iIndex) : nullptr;
}

// Symbol: ?GetItemDataAll@CMFCToolBarComboBoxButton@@SA_KIH@Z
DWORD_PTR CMFCToolBarComboBoxButton::GetItemDataAll(UINT uiCmd, int iIndex) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? pButton->GetItemData(iIndex) : 0;
}

// Symbol: ?GetTextAll@CMFCToolBarComboBoxButton@@SAPEB_WI@Z
const wchar_t* CMFCToolBarComboBoxButton::GetTextAll(UINT uiCmd) {
    CMFCToolBarComboBoxButton* pButton = GetByCmd(uiCmd, FALSE);
    return pButton ? static_cast<const wchar_t*>(pButton->m_strText) : nullptr;
}

// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIPEB_W@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, const wchar_t* lpszText) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(lpszText)) result = TRUE;
    }
    return result;
}

// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIH@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, int nIndex) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(nIndex, TRUE)) result = TRUE;
    }
    return result;
}

// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHI_K@Z
BOOL CMFCToolBarComboBoxButton::SelectItemAll(UINT uiCmd, DWORD_PTR dwData) {
    BOOL result = FALSE;
    for (CMFCToolBarComboBoxButton* button : GetComboButtonsByCmd(uiCmd)) {
        if (button && button->SelectItem(dwData)) result = TRUE;
    }
    return result;
}

// --- extern "C" MS_ABI thunks for CMFCToolBarComboBoxButton ---

// Symbol: ?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, DWORD_PTR p1) {
    return pThis->AddItem(p0, p1);
}

// Symbol: ?AddSortedItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z
extern "C" INT_PTR MS_ABI impl__AddSortedItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, DWORD_PTR p1) {
    return pThis->AddSortedItem(p0, p1);
}

// Symbol: ?Compare@CMFCToolBarComboBoxButton@@UEAAHPEB_W0@Z
extern "C" int MS_ABI impl__Compare_CMFCToolBarComboBoxButton__UEAAHPEB_W0_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0, const wchar_t* p1) {
    return pThis->Compare(p0, p1);
}

// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHH@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAHH_Z(CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->DeleteItem(p0);
}

// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR p0) {
    return pThis->DeleteItem(p0);
}

// Symbol: ?DeleteItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__DeleteItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->DeleteItem(p0);
}

// Symbol: ?FindItem@CMFCToolBarComboBoxButton@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__FindItem_CMFCToolBarComboBoxButton__QEBAHPEB_W_Z(const CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->FindItem(p0);
}

// Symbol: ?GetByCmd@CMFCToolBarComboBoxButton@@SAPEAV1@IH@Z
extern "C" CMFCToolBarComboBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarComboBoxButton__SAPEAV1_IH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetByCmd(p0, (BOOL)p1);
}

// Symbol: ?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ
extern "C" INT_PTR MS_ABI impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(const CMFCToolBarComboBoxButton* pThis) {
    return pThis->GetCount();
}

// Symbol: ?GetCountAll@CMFCToolBarComboBoxButton@@SAHI@Z
extern "C" int MS_ABI impl__GetCountAll_CMFCToolBarComboBoxButton__SAHI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetCountAll(p0);
}

// Symbol: ?GetCurSelAll@CMFCToolBarComboBoxButton@@SAHI@Z
extern "C" int MS_ABI impl__GetCurSelAll_CMFCToolBarComboBoxButton__SAHI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetCurSelAll(p0);
}

// Symbol: ?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z
extern "C" const wchar_t* MS_ABI impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(const CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->GetItem(p0);
}

// Symbol: ?GetItemAll@CMFCToolBarComboBoxButton@@SAPEB_WIH@Z
extern "C" const wchar_t* MS_ABI impl__GetItemAll_CMFCToolBarComboBoxButton__SAPEB_WIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetItemAll(p0, p1);
}

// Symbol: ?GetItemData@CMFCToolBarComboBoxButton@@QEBA_KH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(const CMFCToolBarComboBoxButton* pThis, int p0) {
    return pThis->GetItemData(p0);
}

// Symbol: ?GetItemDataAll@CMFCToolBarComboBoxButton@@SA_KIH@Z
extern "C" DWORD_PTR MS_ABI impl__GetItemDataAll_CMFCToolBarComboBoxButton__SA_KIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::GetItemDataAll(p0, p1);
}

// Symbol: ?GetTextAll@CMFCToolBarComboBoxButton@@SAPEB_WI@Z
extern "C" const wchar_t* MS_ABI impl__GetTextAll_CMFCToolBarComboBoxButton__SAPEB_WI_Z(unsigned int p0) {
    return CMFCToolBarComboBoxButton::GetTextAll(p0);
}

// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    return pThis->SelectItem(p0);
}

// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAHHH@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHHH_Z(CMFCToolBarComboBoxButton* pThis, int p0, int p1) {
    return pThis->SelectItem(p0, (BOOL)p1);
}

// Symbol: ?SelectItem@CMFCToolBarComboBoxButton@@QEAAH_K@Z
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAH_K_Z(CMFCToolBarComboBoxButton* pThis, DWORD_PTR p0) {
    return pThis->SelectItem(p0);
}

// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIPEB_W@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHIPEB_W_Z(unsigned int p0, const wchar_t* p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}

// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHIH@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHIH_Z(unsigned int p0, int p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}

// Symbol: ?SelectItemAll@CMFCToolBarComboBoxButton@@SAHI_K@Z
extern "C" int MS_ABI impl__SelectItemAll_CMFCToolBarComboBoxButton__SAHI_K_Z(unsigned int p0, DWORD_PTR p1) {
    return CMFCToolBarComboBoxButton::SelectItemAll(p0, p1);
}

// Symbol: ?SetText@CMFCToolBarComboBoxButton@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCToolBarComboBoxButton__QEAAXPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* p0) {
    pThis->SetText(p0);
}

//=============================================================================
// CMFCToolBarEditBoxButton
//=============================================================================

CMFCToolBarEditBoxButton::~CMFCToolBarEditBoxButton() {
    UnregisterEditButton(this);
}

// Symbol: ?SetContents@CMFCToolBarEditBoxButton@@UEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
void CMFCToolBarEditBoxButton::SetContents(const CString& sContents) {
    RegisterEditButton(this);
    m_strText = sContents;
    if (m_nID != 0) {
        g_editContentsByCmd[m_nID] = sContents;
    }
}

// Symbol: ?GetByCmd@CMFCToolBarEditBoxButton@@SAPEAV1@I@Z
CMFCToolBarEditBoxButton* CMFCToolBarEditBoxButton::GetByCmd(UINT uiCmd) {
    auto it = g_editByCmd.find(uiCmd);
    if (it == g_editByCmd.end() || it->second.empty()) return nullptr;
    return it->second.front();
}

// Symbol: ?GetContentsAll@CMFCToolBarEditBoxButton@@SA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
CString CMFCToolBarEditBoxButton::GetContentsAll(UINT uiCmd) {
    if (CMFCToolBarEditBoxButton* pButton = GetByCmd(uiCmd)) {
        return pButton->m_strText;
    }
    auto it = g_editContentsByCmd.find(uiCmd);
    return (it != g_editContentsByCmd.end()) ? it->second : CString();
}

// Symbol: ?SetContentsAll@CMFCToolBarEditBoxButton@@SAHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
BOOL CMFCToolBarEditBoxButton::SetContentsAll(UINT uiCmd, const CString& sContents) {
    BOOL updated = FALSE;
    g_editContentsByCmd[uiCmd] = sContents;
    auto it = g_editByCmd.find(uiCmd);
    if (it != g_editByCmd.end()) {
        for (CMFCToolBarEditBoxButton* button : it->second) {
            if (!button) continue;
            button->m_strText = sContents;
            updated = TRUE;
        }
    }
    return updated;
}

// --- extern "C" MS_ABI thunks for CMFCToolBarEditBoxButton ---

// Symbol: ?SetContents@CMFCToolBarEditBoxButton@@UEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetContents_CMFCToolBarEditBoxButton__UEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CMFCToolBarEditBoxButton* pThis, const CString* p0) {
    pThis->SetContents(*p0);
}

// Symbol: ?GetByCmd@CMFCToolBarEditBoxButton@@SAPEAV1@I@Z
extern "C" CMFCToolBarEditBoxButton* MS_ABI impl__GetByCmd_CMFCToolBarEditBoxButton__SAPEAV1_I_Z(unsigned int p0) {
    return CMFCToolBarEditBoxButton::GetByCmd(p0);
}

// Symbol: ?GetContentsAll@CMFCToolBarEditBoxButton@@SA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
extern "C" void MS_ABI impl__GetContentsAll_CMFCToolBarEditBoxButton__SA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(CString* pRet, unsigned int p0) {
    *pRet = CMFCToolBarEditBoxButton::GetContentsAll(p0);
}

// Symbol: ?SetContentsAll@CMFCToolBarEditBoxButton@@SAHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__SetContentsAll_CMFCToolBarEditBoxButton__SAHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(unsigned int p0, const CString* p1) {
    return CMFCToolBarEditBoxButton::SetContentsAll(p0, *p1);
}

//=============================================================================
// CMFCToolBarMenuButton - parameterized constructor + Initialize
//=============================================================================

// Symbol: ??0CMFCToolBarMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
CMFCToolBarMenuButton::CMFCToolBarMenuButton(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow,
                                              const wchar_t* lpszText, BOOL bUserButton)
    : CMFCToolBarButton(), m_hMenu(nullptr) {
    memset(_pad, 0, sizeof(_pad));
    Initialize(uiID, hMenu, bHasDropDownArrow, lpszText, bUserButton);
}

// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
void CMFCToolBarMenuButton::Initialize(UINT uiID, HMENU hMenu, BOOL, const wchar_t* lpszText, BOOL bUserButton) {
    m_nID = uiID;
    m_hMenu = hMenu;
    m_bUserButton = bUserButton;
    if (lpszText) m_strText = lpszText;
}

extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(void* pThis, unsigned int p0, HMENU p1, int p2, const wchar_t* p3, int p4) {
    return new(pThis) CMFCToolBarMenuButton(p0, p1, (BOOL)p2, p3, (BOOL)p4);
}

// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXIPEAUHMENU____HPEB_WH_Z(CMFCToolBarMenuButton* pThis, unsigned int p0, HMENU p1, int p2, const wchar_t* p3, int p4) {
    pThis->Initialize(p0, p1, (BOOL)p2, p3, (BOOL)p4);
}
