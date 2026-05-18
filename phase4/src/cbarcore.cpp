// Toolbar / StatusBar / DialogBar Implementation
// Provides MFC wrappers around Windows common controls
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <commctrl.h>
#include <algorithm>
#include <cstring>
#include <string>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
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
struct RibbonPanelState {
    std::vector<CMFCRibbonBaseElement*> elements;
};

struct RibbonCategoryState {
    std::vector<CMFCRibbonPanel*> panels;
    std::unordered_set<CMFCRibbonPanel*> ownedPanels;
};

struct RibbonBarState {
    std::vector<CMFCRibbonCategory*> categories;
    std::unordered_set<CMFCRibbonCategory*> ownedCategories;
    CMFCRibbonCategory* activeCategory = nullptr;
};

std::mutex g_ribbonMutex;
std::unordered_map<CMFCRibbonPanel*, RibbonPanelState> g_ribbonPanels;
std::unordered_map<CMFCRibbonCategory*, RibbonCategoryState> g_ribbonCategories;
std::unordered_map<CMFCRibbonBar*, RibbonBarState> g_ribbonBars;
constexpr int kApproxRibbonCharPx = 6;

std::unordered_map<UINT, std::wstring> g_ribbonToolTips;
std::unordered_map<UINT, std::wstring> g_ribbonDescriptions;
std::unordered_map<UINT, int> g_galleryLastSelectedByID;

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

} // namespace

extern "C" void* MS_ABI impl___0CMFCRibbonBar__QEAA_H_Z(void* pThis, int bReplaceFrameCaption) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    CMFCRibbonBar* bar = new(pThis) CMFCRibbonBar(bReplaceFrameCaption);
    g_ribbonBars[bar] = RibbonBarState{};
    return bar;
}

extern "C" void* MS_ABI impl___0CMFCRibbonCategory__IEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    CMFCRibbonCategory* category = new(pThis) CMFCRibbonCategory();
    g_ribbonCategories[category] = RibbonCategoryState{};
    return category;
}

extern "C" void MS_ABI impl___1CMFCRibbonPanel__UEAA_XZ(void* pThis) {
    auto* panel = reinterpret_cast<CMFCRibbonPanel*>(pThis);
    if (!panel) return;

    {
        std::lock_guard<std::mutex> lock(g_ribbonMutex);
        g_ribbonPanels.erase(panel);
        for (auto& [_, categoryState] : g_ribbonCategories) {
            categoryState.ownedPanels.erase(panel);
            auto& panels = categoryState.panels;
            panels.erase(std::remove(panels.begin(), panels.end(), panel), panels.end());
        }
    }

    panel->~CMFCRibbonPanel();
}

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

extern "C" void MS_ABI impl__Add_CMFCRibbonPanel__UEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonPanel* pThis, CMFCRibbonBaseElement* pElement) {
    if (!pThis || !pElement) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonPanels[pThis].elements.push_back(pElement);
}

extern "C" void MS_ABI impl__AddSeparator_CMFCRibbonPanel__UEAAXXZ(CMFCRibbonPanel* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonPanels[pThis].elements.push_back(nullptr);
}

extern "C" int MS_ABI impl__GetCount_CMFCRibbonPanel__QEBAHXZ(const CMFCRibbonPanel* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonPanels.find(const_cast<CMFCRibbonPanel*>(pThis));
    return (it == g_ribbonPanels.end()) ? 0 : static_cast<int>(it->second.elements.size());
}

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

extern "C" int MS_ABI impl__GetPanelCount_CMFCRibbonCategory__QEBAHXZ(const CMFCRibbonCategory* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonCategories.find(const_cast<CMFCRibbonCategory*>(pThis));
    return (it == g_ribbonCategories.end()) ? 0 : static_cast<int>(it->second.panels.size());
}

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

extern "C" int MS_ABI impl__GetCategoryCount_CMFCRibbonBar__QEBAHXZ(const CMFCRibbonBar* pThis) {
    if (!pThis) return 0;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonBars.find(const_cast<CMFCRibbonBar*>(pThis));
    return (it == g_ribbonBars.end()) ? 0 : static_cast<int>(it->second.categories.size());
}

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

extern "C" void MS_ABI impl__AddToTabs_CMFCRibbonBar__QEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonBar*, CMFCRibbonBaseElement*) {}

extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonCategory__SAPEAVCObject__XZ() {
    return CMFCRibbonCategory::GetThisClass()->CreateObject();
}

extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonPanel__SAPEAVCObject__XZ() {
    return CMFCRibbonPanel::GetThisClass()->CreateObject();
}

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonBar__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonBar* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CMFCRibbonBar::GetThisClass();
}

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonCategory__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonCategory* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CMFCRibbonCategory::GetThisClass();
}

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonPanel__UEBAPEAUCRuntimeClass__XZ(
    const CMFCRibbonPanel* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CMFCRibbonPanel::GetThisClass();
}

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonBar__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonBar::GetThisClass();
}

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonCategory__SAPEAUCRuntimeClass__XZ() {
    return CMFCRibbonCategory::GetThisClass();
}

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
    : m_nCount(0), m_hbmImageWell(nullptr), m_pToolTip(nullptr),
      m_pDockBar(nullptr), m_pDockSite(nullptr), m_pDockContext(nullptr),
      m_bInRecalcLayout(FALSE), m_bDelayedButtonLayout(FALSE) {
    m_sizeButton.cx = 23;
    m_sizeButton.cy = 22;
    m_sizeImage.cx = 16;
    m_sizeImage.cy = 15;
    memset(_toolbar_padding, 0, sizeof(_toolbar_padding));
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

BOOL CToolBar::LoadToolBar(const wchar_t* lpszResourceName) {
    if (!lpszResourceName) return FALSE;
    // Try loading by name
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(), lpszResourceName, RT_TOOLBAR);
    if (!hRsrc) return FALSE;
    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;
    WORD* pData = (WORD*)::LockResource(hGlobal);
    WORD wCount = pData[1];
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
    m_pToolTip = pToolTip;
    if (m_hWnd && pToolTip && pToolTip->GetSafeHwnd()) {
        ::SendMessageW(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pToolTip->GetSafeHwnd(), 0);
    }
}

CToolTipCtrl* CToolBar::GetToolTips() const {
    return m_pToolTip;
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

CStatusBar::CStatusBar()
    : m_nCount(0), m_pData(nullptr), m_cxLeftBorder(0), m_cxRightBorder(0),
      m_pToolTip(nullptr) {
    memset(_statusbar_padding, 0, sizeof(_statusbar_padding));
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
    if (!m_hWnd || nIndex < 0 || nIndex >= m_nCount) return FALSE;
    m_pData[nIndex * 4] = nID;
    m_pData[nIndex * 4 + 1] = nStyle;
    m_pData[nIndex * 4 + 2] = cxWidth;
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
    if (bUpdate) {
        ::SendMessageW(m_hWnd, SB_SETTEXTW, nIndex | SBT_OWNERDRAW, (LPARAM)lpszNewText);
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
        sz.cx = (int)::SendMessageW(m_hWnd, SB_GETBORDERS, 0, 0);
        sz.cy = 0;
    }
    return sz;
}

void CStatusBar::SetToolTips(CToolTipCtrl* pToolTip) {
    m_pToolTip = pToolTip;
}

CToolTipCtrl* CStatusBar::GetToolTips() const {
    return m_pToolTip;
}

void CStatusBar::EnableDocking(DWORD dwDockStyle) {
    (void)dwDockStyle;
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

    HRESULT hr = pTaskDialogIndirect
        ? pTaskDialogIndirect(&tc, &nButton, &nRadio, &bVerification)
        : E_NOTIMPL;
    if (SUCCEEDED(hr)) {
        m_bVerificationChecked = bVerification;
        m_nSelectedRadioButtonID = nRadio;
        return nButton;
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
    g_ribbonToolTips[RibbonElementID(pThis)] = (lpszText != nullptr) ? lpszText : L"";
}

// Symbol: ?GetToolTipText@CMFCRibbonBaseElement@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonBaseElement* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
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
    g_ribbonDescriptions[RibbonElementID(pThis)] = (lpszText != nullptr) ? lpszText : L"";
}

// Symbol: ?GetDescription@CMFCRibbonBaseElement@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDescription_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonBaseElement* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
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
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnDrawBorder@CMFCRibbonButton@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCRibbonButton__UEAAXPEAVCDC___Z(CMFCRibbonButton* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnFillBackground@CMFCRibbonButton@@UEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonButton__UEAAKPEAVCDC___Z(CMFCRibbonButton* /*pThis*/, CDC* /*pDC*/) {
    return 0;
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
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonGallery__UEAAXPEAVCDC___Z(CMFCRibbonGallery* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnEnable@CMFCRibbonGallery@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonGallery__UEAAXH_Z(CMFCRibbonGallery* pThis, int bEnable) {
    if (!pThis) return;
    (void)bEnable;
}

// Symbol: ?SelectItem@CMFCRibbonGallery@@QEAAXH@Z
extern "C" void MS_ABI impl__SelectItem_CMFCRibbonGallery__QEAAXH_Z(CMFCRibbonGallery* pThis, int nItem) {
    if (!pThis) return;
    g_galleryLastSelectedByID[pThis->GetID()] = nItem;
}

// Symbol: ?GetLastSelectedItem@CMFCRibbonGallery@@SAHI@Z
extern "C" int MS_ABI impl__GetLastSelectedItem_CMFCRibbonGallery__SAHI_Z(UINT nGalleryID) {
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
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonEdit__UEAAXPEAVCDC___Z(CMFCRibbonEdit* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnEnable@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonEdit__UEAAXH_Z(CMFCRibbonEdit* pThis, int bEnable) {
    if (!pThis) return;
    (void)bEnable;
}

// Symbol: ?GetRegularSize@CMFCRibbonSlider@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonSlider__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonSlider* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?OnDraw@CMFCRibbonSlider@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonSlider__UEAAXPEAVCDC___Z(CMFCRibbonSlider* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?SetRange@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nMin, int nMax) {
    if (!pThis) return;
    auto& state = g_sliderStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}

// Symbol: ?SetPos@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    auto& state = g_sliderStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}

// Symbol: ?SetZoomButtons@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomButtons_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int bSet) {
    if (!pThis) return;
    g_sliderStates[pThis->GetID()].bShowZoomButtons = (bSet != FALSE);
}

// Symbol: ?SetZoomIncrement@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomIncrement_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int nDelta) {
    if (!pThis) return;
    g_sliderStates[pThis->GetID()].nZoomIncrement = std::max(1, nDelta);
}

// Symbol: ?GetRegularSize@CMFCRibbonProgressBar@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonProgressBar__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonProgressBar* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 16);
}

// Symbol: ?OnDraw@CMFCRibbonProgressBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonProgressBar__UEAAXPEAVCDC___Z(CMFCRibbonProgressBar* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?SetRange@CMFCRibbonProgressBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonProgressBar__QEAAXHH_Z(CMFCRibbonProgressBar* pThis, int nMin, int nMax) {
    if (!pThis) return;
    auto& state = g_progressStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}

// Symbol: ?SetPos@CMFCRibbonProgressBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonProgressBar__QEAAXHH_Z(CMFCRibbonProgressBar* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    auto& state = g_progressStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}

// Symbol: ?SetInfiniteMode@CMFCRibbonProgressBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetInfiniteMode_CMFCRibbonProgressBar__QEAAXH_Z(CMFCRibbonProgressBar* pThis, int bSet) {
    if (!pThis) return;
    g_progressStates[pThis->GetID()].bInfinite = (bSet != FALSE);
}

// Symbol: ?GetIntermediateSize@CMFCRibbonStatusBarPane@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonStatusBarPane__MEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonStatusBarPane* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}

// Symbol: ?OnCalcTextSize@CMFCRibbonStatusBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnCalcTextSize_CMFCRibbonStatusBarPane__MEAAXPEAVCDC___Z(CMFCRibbonStatusBarPane* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnDraw@CMFCRibbonStatusBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonStatusBarPane__MEAAXPEAVCDC___Z(CMFCRibbonStatusBarPane* /*pThis*/, CDC* /*pDC*/) {}

// Symbol: ?OnFillBackground@CMFCRibbonStatusBarPane@@UEAAKPEAVCDC@@@Z
extern "C" unsigned long MS_ABI impl__OnFillBackground_CMFCRibbonStatusBarPane__UEAAKPEAVCDC___Z(CMFCRibbonStatusBarPane* /*pThis*/, CDC* /*pDC*/) {
    return 0;
}

// Symbol: ?GetToolTipText@CMFCRibbonStatusBarPane@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonStatusBarPane__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonStatusBarPane* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
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
    auto& state = g_statusPaneStates[pThis->GetID()];
    state.bAnimating = true;
    state.nAnimationFrame = 0;
    state.nAnimationCount = nAnimationDuration;
    if (nAnimationDelay != 0) state.dwAnimationDelay = nAnimationDelay;
}

// Symbol: ?StopAnimation@CMFCRibbonStatusBarPane@@QEAAXXZ
extern "C" void MS_ABI impl__StopAnimation_CMFCRibbonStatusBarPane__QEAAXXZ(CMFCRibbonStatusBarPane* pThis) {
    if (!pThis) return;
    g_statusPaneStates[pThis->GetID()].bAnimating = false;
}


//=============================================================================
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

// Symbol: ?GetPaneText@CStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CStatusBar* pThis, int nIndex, CString* pStr) {
    pThis->GetPaneText(nIndex, *pStr);
}
