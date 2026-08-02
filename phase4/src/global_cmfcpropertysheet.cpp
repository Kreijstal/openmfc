// OpenMFC — real implementations for CMFCPropertySheet stub exports.
//
// CMFCPropertySheet (size 29232, base CPropertySheet) adds navigation pane
// (tree/list), page headers, and visual-manager integration. This file provides
// minimal-but-real implementations: ctors/dtors delegate to the base
// CPropertySheet, virtual overrides forward where possible, and CMFC-specific
// helpers return sensible defaults.
//
// Every impl_ name matches the .def alias exactly. // Symbol: markers exclude
// these from auto-generated weak stubs so the linker picks the strong symbol.

#include <windows.h>
#include <cstring>
#include <algorithm>
#include <new>
#include <mutex>
#include <unordered_map>
#include <cwchar>
#include <vector>
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Forward declarations for symbols we delegate to (defined in dlgcore.cpp / thunks.cpp).
extern "C" void MS_ABI impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(CPropertySheet*, CPropertyPage*);
extern "C" void MS_ABI impl__RemovePage_CPropertySheet__QEAAXH_Z(CPropertySheet*, int);
extern "C" void MS_ABI impl__RemovePage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(CPropertySheet*, CPropertyPage*);
extern "C" int MS_ABI impl__OnInitDialog_CPropertySheet__UEAAHXZ(CPropertySheet*);
extern "C" int MS_ABI impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(CPropertySheet*, MSG*);
extern "C" void MS_ABI impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(CPropertySheet*, CWnd*, unsigned int);
extern "C" void MS_ABI impl___1CMFCPropertySheetCategoryInfo__UEAA_XZ(void*);
extern "C" void* MS_ABI impl___0CMFCPropertySheetCategoryInfo__QEAA_PEB_WHHPEBV0_AEAVCMFCPropertySheet___Z(
    void*, const wchar_t*, int, int, const void*, void*);

namespace {

// Size harvested from mfc140u.dll (tools/harvest).
static const size_t kSizeof_CMFCPropertySheet = 29232;
static const size_t kSizeof_CMFCPropertySheetCategoryInfo = 168;
static const int kDefaultNavBarWidth = 180;

struct CMFCPropertySheetState {
    bool pageHeaderEnabled = true;
    bool leftNavigationPane = true;
    bool dynamicLayout = false;
    bool tabCreated = false;
    bool navCreated = false;
    int navBarWidth = kDefaultNavBarWidth;
    int look = 0;
    int iconCx = 0;
    unsigned int nImageList = 0;
    void* imagelist = nullptr;
    void* tabCtrl = nullptr;
    void* navWnd = nullptr;
    std::vector<void*> navPages;
    std::vector<void*> pages;
    std::vector<void*> categories;
    std::vector<void*> rootCategories;
    std::unordered_map<void*, std::vector<void*>> categoryPages;
    std::unordered_map<void*, std::vector<void*>> categoryChildren;
    std::unordered_map<void*, void*> categoryParent;
    std::unordered_map<void*, int> pageToNavIndex;
    std::unordered_map<void*, void*> pageToCategory;
};

std::mutex g_stateMutex;
std::unordered_map<const void*, CMFCPropertySheetState> g_sheetStates;
std::unordered_map<void*, void*> g_categoryMap;

CMFCPropertySheetState* GetSheetState(const void* pThis, bool createIfMissing) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_stateMutex);
    auto it = g_sheetStates.find(pThis);
    if (it == g_sheetStates.end()) {
        if (!createIfMissing) {
            return nullptr;
        }
        it = g_sheetStates.emplace(pThis, CMFCPropertySheetState{}).first;
    }
    return &it->second;
}

namespace {

template <typename T>
void EraseFromVector(std::vector<T>& values, const T& value) {
    values.erase(std::remove(values.begin(), values.end(), value), values.end());
}

void ReindexNavPages(CMFCPropertySheetState& state) {
    state.pageToNavIndex.clear();
    for (size_t i = 0; i < state.navPages.size(); ++i) {
        state.pageToNavIndex[state.navPages[i]] = static_cast<int>(i);
    }
}

void* CategoryGetParentPtr(const void* pCategory) {
    if (pCategory == nullptr) {
        return nullptr;
    }
    return *reinterpret_cast<void* const*>(
        static_cast<const char*>(pCategory) + 24);
}

void* CategoryGetOwnerPtr(const void* pCategory) {
    if (pCategory == nullptr) {
        return nullptr;
    }
    return *reinterpret_cast<void* const*>(
        static_cast<const char*>(pCategory) + 32);
}

int RemovePageFromState(CMFCPropertySheetState& state, void* pPage) {
    auto it = state.pageToNavIndex.find(pPage);
    if (it == state.pageToNavIndex.end()) {
        return -1;
    }
    const int index = it->second;
    state.pageToNavIndex.erase(it);

    if (index >= 0 && index < static_cast<int>(state.navPages.size())) {
        if (state.navWnd) {
            auto* nav = static_cast<CWnd*>(state.navWnd);
            if (nav && nav->GetSafeHwnd()) {
                ::SendMessageW(nav->m_hWnd, LB_DELETESTRING, index, 0);
            }
        }
        if (state.navPages[index] == pPage) {
            state.navPages.erase(state.navPages.begin() + index);
            ReindexNavPages(state);
        }
    }

    if (const auto* pCategory = state.pageToCategory[pPage]) {
        auto catIt = state.categoryPages.find(const_cast<void*>(pCategory));
        if (catIt != state.categoryPages.end()) {
            EraseFromVector(catIt->second, pPage);
        }
    }
    state.pageToCategory.erase(pPage);
    EraseFromVector(state.pages, pPage);
    return index;
}

int AddPageLabelIndex(CMFCPropertySheetState& state, CPropertyPage* pPage, const wchar_t* fallbackLabel) {
    if (!state.navWnd) {
        return -1;
    }
    auto* navWnd = static_cast<CWnd*>(state.navWnd);
    if (!navWnd || !navWnd->GetSafeHwnd()) {
        return -1;
    }
    if (fallbackLabel == nullptr) {
        fallbackLabel = L"";
    }
    const int idx = static_cast<int>(::SendMessageW(
        navWnd->m_hWnd, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(fallbackLabel)));
    if (idx >= 0) {
        if (idx < static_cast<int>(state.navPages.size())) {
            state.navPages[idx] = pPage;
        } else if (idx == static_cast<int>(state.navPages.size())) {
            state.navPages.push_back(pPage);
        } else {
            state.navPages.resize(idx + 1, nullptr);
            state.navPages[idx] = pPage;
        }
        state.pageToNavIndex[pPage] = idx;
    }
    return idx;
}

void BuildPageListText(void* pPage, wchar_t* out, size_t cch, int fallbackIndex) {
    if (out == nullptr || cch == 0) {
        return;
    }
    out[0] = L'\0';
    const auto* page = static_cast<CPropertyPage*>(pPage);
    if (page && page->m_hWnd) {
        const int len = page->GetWindowTextLengthW();
        if (len > 0) {
            page->GetWindowTextW(out, static_cast<int>(cch));
            if (out[0] != L'\0') {
                return;
            }
        }
    }
    ::swprintf(out, cch, L"Page %d", fallbackIndex + 1);
}

}  // namespace

// Helper: zero-init the CMFCPropertySheet object and placement-new the base
// CPropertySheet with the given ctor args. The most-derived vtable is installed
// at runtime through the CRuntimeClass / CreateObject path.
inline void* InitBase(void* pThis, CWnd* pParentWnd, unsigned int iSelectPage) {
    std::memset(pThis, 0, kSizeof_CMFCPropertySheet);
    auto* base = static_cast<CPropertySheet*>(pThis);
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(base, pParentWnd, iSelectPage);
    if (auto* state = GetSheetState(pThis, true)) {
        state->navBarWidth = kDefaultNavBarWidth;
    }
    return pThis;
}

} // namespace

// =============================================================================
// Constructors / Destructor (ords 646, 647, 648, 1236)
// =============================================================================

// Symbol: ??0CMFCPropertySheet@@QEAA@IPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCPropertySheet__QEAA_IPEAVCWnd__I_Z(
    void* pThis, unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    (void)nIDCaption;
    InitBase(pThis, pParentWnd, iSelectPage);
    static_cast<CPropertySheet*>(pThis)->m_pszCaption = nullptr;
    return pThis;
}

// Symbol: ??0CMFCPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(
    void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    InitBase(pThis, pParentWnd, iSelectPage);
    static_cast<CPropertySheet*>(pThis)->m_pszCaption = pszCaption;
    return pThis;
}

// Symbol: ??0CMFCPropertySheet@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertySheet__QEAA_XZ(void* pThis) {
    InitBase(pThis, nullptr, 0);
    return pThis;
}

// Symbol: ??1CMFCPropertySheet@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCPropertySheet__UEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<CPropertySheet*>(pThis)->~CPropertySheet();
        std::lock_guard<std::mutex> lock(g_stateMutex);
        for (auto it = g_categoryMap.begin(); it != g_categoryMap.end();) {
            if (CategoryGetOwnerPtr(it->first) == pThis) {
                impl___1CMFCPropertySheetCategoryInfo__UEAA_XZ(it->second);
                ::operator delete[](it->second);
                it = g_categoryMap.erase(it);
            } else {
                ++it;
            }
        }
        auto it = g_sheetStates.find(pThis);
        if (it != g_sheetStates.end()) {
            if (it->second.tabCtrl) {
                auto* tab = static_cast<CMFCTabCtrl*>(it->second.tabCtrl);
                if (tab) {
                    tab->DestroyWindow();
                    delete tab;
                }
            }
            if (it->second.navWnd) {
                auto* navWnd = static_cast<CWnd*>(it->second.navWnd);
                if (navWnd->GetSafeHwnd()) {
                    navWnd->DestroyWindow();
                }
                delete navWnd;
            }
            g_sheetStates.erase(it);
        }
    }
}

// =============================================================================
// Page management — forward to base CPropertySheet (ords 1903, 12388, 12389)
// =============================================================================

// Symbol: ?AddPage@CMFCPropertySheet@@QEAAXPEAVCPropertyPage@@@Z
extern "C" void MS_ABI impl__AddPage_CMFCPropertySheet__QEAAXPEAVCPropertyPage___Z(
    void* pThis, CPropertyPage* pPage) {
    impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(static_cast<CPropertySheet*>(pThis), pPage);
}

// Symbol: ?RemovePage@CMFCPropertySheet@@QEAAXH@Z
extern "C" void MS_ABI impl__RemovePage_CMFCPropertySheet__QEAAXH_Z(void* pThis, int nPage) {
    impl__RemovePage_CPropertySheet__QEAAXH_Z(static_cast<CPropertySheet*>(pThis), nPage);
}

// Symbol: ?RemovePage@CMFCPropertySheet@@QEAAXPEAVCPropertyPage@@@Z
extern "C" void MS_ABI impl__RemovePage_CMFCPropertySheet__QEAAXPEAVCPropertyPage___Z(
    void* pThis, CPropertyPage* pPage) {
    impl__RemovePage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(static_cast<CPropertySheet*>(pThis), pPage);
}

// =============================================================================
// Overrides — forward to base or return default (ords 10172, 11884, 7727, 14336, 14350)
// =============================================================================

// Symbol: ?OnInitDialog@CMFCPropertySheet@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCPropertySheet__UEAAHXZ(void* pThis) {
    return impl__OnInitDialog_CPropertySheet__UEAAHXZ(static_cast<CPropertySheet*>(pThis));
}

// Symbol: ?PreTranslateMessage@CMFCPropertySheet@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCPropertySheet__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg) {
    return impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(static_cast<CPropertySheet*>(pThis), pMsg);
}

// Symbol: ?InitNavigationControl@CMFCPropertySheet@@UEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__InitNavigationControl_CMFCPropertySheet__UEAAPEAVCWnd__XZ(void* pThis) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (!pWnd || !pWnd->m_hWnd) {
        return nullptr;
    }
    auto* state = GetSheetState(pThis, true);
    if (!state) {
        return nullptr;
    }
    if (state->navWnd) {
        return static_cast<CWnd*>(state->navWnd);
    }

    RECT rc{};
    ::GetClientRect(pWnd->m_hWnd, &rc);
    auto* navWnd = new CWnd();
    const int width = state->navBarWidth > 0 ? state->navBarWidth : kDefaultNavBarWidth;
    HWND hWnd = ::CreateWindowExW(
        0, L"LISTBOX", nullptr, WS_CHILD | WS_VISIBLE,
        rc.left, rc.top, width, rc.bottom - rc.top,
        pWnd->m_hWnd, nullptr, AfxGetInstanceHandle(), nullptr);
    if (!hWnd) {
        delete navWnd;
        return nullptr;
    }
    navWnd->Attach(hWnd);
    state->navWnd = navWnd;
    state->navCreated = true;
    return navWnd;
}

// Symbol: ?CanAddPageToDynamicLayout@CMFCPropertySheet@@MEBAHXZ
extern "C" int MS_ABI impl__CanAddPageToDynamicLayout_CMFCPropertySheet__MEBAHXZ(const void* pThis) {
    auto* state = GetSheetState(pThis, false);
    return state && state->dynamicLayout ? TRUE : FALSE;
}

// Symbol: ?IsLeftNavigationPane@CMFCPropertySheet@@MEBAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__IsLeftNavigationPane_CMFCPropertySheet__MEBAHPEAUHWND_____Z(
    const void* pThis, HWND hWnd) {
    auto* state = GetSheetState(pThis, false);
    (void)hWnd;
    return state ? (state->leftNavigationPane ? TRUE : FALSE) : TRUE;
}

// =============================================================================
// CMFC-specific helpers — no-op or sensible defaults (ords 4292, 2887, 4587, 7807, 6738, 14346)
// =============================================================================

// Symbol: ?EnablePageHeader@CMFCPropertySheet@@QEAAXH@Z
extern "C" void MS_ABI impl__EnablePageHeader_CMFCPropertySheet__QEAAXH_Z(void* pThis, int bEnable) {
    auto* state = GetSheetState(pThis, true);
    if (state) {
        state->pageHeaderEnabled = (bEnable != 0);
    }
}

// Symbol: ?CommonInit@CMFCPropertySheet@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCPropertySheet__IEAAXXZ(void* pThis) {
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        if (!pWnd->m_hWnd) {
            return;
        }
    }
    auto* state = GetSheetState(pThis, true);
    if (!state) {
        return;
    }
    if (!state->tabCtrl) {
        state->tabCtrl = new CMFCTabCtrl();
        state->tabCreated = true;
    }
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        pWnd->Invalidate();
    }
}

// Symbol: ?FindPageIndexInList@CMFCPropertySheet@@IEAAHPEAVCPropertyPage@@@Z
extern "C" int MS_ABI impl__FindPageIndexInList_CMFCPropertySheet__IEAAHPEAVCPropertyPage___Z(
    void* pThis, CPropertyPage* pPage) {
    auto* sheet = static_cast<CPropertySheet*>(pThis);
    if (!sheet || !pPage) {
        return -1;
    }
    return sheet->GetPageIndex(pPage);
}

// Symbol: ?InternalAddPage@CMFCPropertySheet@@IEAAXH@Z
extern "C" void MS_ABI impl__InternalAddPage_CMFCPropertySheet__IEAAXH_Z(void* pThis, int nIndex) {
    auto* sheet = static_cast<CPropertySheet*>(pThis);
    if (!sheet) {
        return;
    }
    if (sheet->GetPage(nIndex) != nullptr) {
        sheet->SetActivePage(nIndex);
    }
}

// Symbol: ?GetTab@CMFCPropertySheet@@QEBAAEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__GetTab_CMFCPropertySheet__QEBAAEAVCMFCTabCtrl__XZ(const void* pThis) {
    // The CMFCTabCtrl member is embedded at a specific offset within
    // CMFCPropertySheet. Return a pointer to the base object's tab area.
    // In the absence of the full layout, return (this + small_offset).
    // The real offset is derived from the harvested layout; for now we return
    // a pointer into the padded object that callers treat as CMFCTabCtrl*.
    auto* state = GetSheetState(pThis, true);
    return state ? state->tabCtrl : nullptr;
}

// Symbol: ?GetNavBarWidth@CMFCPropertySheet@@QEBAHXZ
extern "C" int MS_ABI impl__GetNavBarWidth_CMFCPropertySheet__QEBAHXZ(const void* pThis) {
    auto* state = GetSheetState(pThis, false);
    return state ? state->navBarWidth : kDefaultNavBarWidth;
}

// =============================================================================
// Page-header / activation notifications — no-op (ords 9459, 8606, 8626)
// =============================================================================

// Symbol: ?OnDrawPageHeader@CMFCPropertySheet@@UEAAXPEAVCDC@@HVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPageHeader_CMFCPropertySheet__UEAAXPEAVCDC__HVCRect___Z(
    void* pThis, CDC* pDC, int nPage, const CRect& rect) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (!pWnd || !pDC || !pDC->m_hDC) {
        return;
    }
    auto* state = GetSheetState(pThis, false);
    if (!state || !state->pageHeaderEnabled) {
        return;
    }

    HBRUSH brush = ::GetSysColorBrush(COLOR_3DFACE);
    RECT rcPage{rect.left, rect.top, rect.right, rect.bottom};
    ::FillRect(pDC->m_hDC, &rcPage, brush);
    RECT drawRect{rect.left, rect.top, rect.right, rect.bottom};
    std::wstring label = L"";
    int pageIndex = nPage;
    auto* sheet = static_cast<CPropertySheet*>(pThis);
    if (sheet && sheet->m_nActivePage >= 0) {
        pageIndex = sheet->m_nActivePage;
    }
    wchar_t tmp[48]{};
    std::swprintf(tmp, 48, L"Page %d", pageIndex + 1);
    label.assign(tmp);
    SetTextColor(pDC->m_hDC, ::GetSysColor(COLOR_WINDOWTEXT));
    SetBkMode(pDC->m_hDC, TRANSPARENT);
    InflateRect(&drawRect, -4, -2);
    ::DrawTextW(pDC->m_hDC, label.c_str(), static_cast<int>(label.size()), &drawRect,
                DT_SINGLELINE | DT_VCENTER | DT_LEFT);
}

// Symbol: ?OnActivatePage@CMFCPropertySheet@@UEAAXPEAVCPropertyPage@@@Z
extern "C" void MS_ABI impl__OnActivatePage_CMFCPropertySheet__UEAAXPEAVCPropertyPage___Z(
    void* pThis, CPropertyPage* pPage) {
    if (auto* sheet = static_cast<CPropertySheet*>(pThis)) {
        if (pPage) {
            sheet->SetActivePage(pPage);
        }
    }
}

// Symbol: ?OnAfterActivatePage@CMFCPropertySheet@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnAfterActivatePage_CMFCPropertySheet__IEAA_J_K_J_Z(
    void* pThis, WPARAM wParam, LPARAM lParam) {
    (void)lParam;
    auto* state = GetSheetState(pThis, true);
    if (state && wParam > 0) {
        state->dynamicLayout = true;
    }
    return static_cast<LRESULT>(wParam);
}

// =============================================================================
// Tree / navigation-list callbacks — no-op or default (ords 1816, 1906, 2027,
// 12339, 9990, 10895, 10996, 10997, 11145, 11314)
// =============================================================================

// Symbol: ?AddCategoryToTree@CMFCPropertySheet@@IEAAXPEAVCMFCPropertySheetCategoryInfo@@@Z
extern "C" void MS_ABI impl__AddCategoryToTree_CMFCPropertySheet__IEAAXPEAVCMFCPropertySheetCategoryInfo___Z(
    void* pThis, void* pCategory) {
    auto* state = GetSheetState(pThis, true);
    if (!state || !pCategory) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_stateMutex);
    if (std::find(state->categories.begin(), state->categories.end(), pCategory) !=
        state->categories.end()) {
        return;
    }

    const void* parent = CategoryGetParentPtr(pCategory);
    state->categories.push_back(pCategory);
    state->categoryParent[pCategory] = const_cast<void*>(parent);
    state->categoryPages.emplace(pCategory, std::vector<void*>{});
    if (parent != nullptr) {
        state->categoryChildren[const_cast<void*>(parent)].push_back(pCategory);
    } else {
        state->rootCategories.push_back(pCategory);
    }

}

// Symbol: ?AddPageToTree@CMFCPropertySheet@@QEAAXPEAVCMFCPropertySheetCategoryInfo@@PEAVCMFCPropertyPage@@HH@Z
extern "C" void MS_ABI impl__AddPageToTree_CMFCPropertySheet__QEAAXPEAVCMFCPropertySheetCategoryInfo__PEAVCMFCPropertyPage__HH_Z(
    void* pThis, void* pCategory, void* pPage, int nIcon, int nSelIcon) {
    (void)nIcon;
    (void)nSelIcon;
    auto* state = GetSheetState(pThis, true);
    if (!state || !pCategory || !pPage) {
        return;
    }

    auto* page = static_cast<CPropertyPage*>(pPage);
    std::lock_guard<std::mutex> lock(g_stateMutex);
    if (state->pageToNavIndex.find(pPage) != state->pageToNavIndex.end()) {
        return;
    }

    state->pageToCategory[pPage] = pCategory;

    wchar_t label[128]{};
    BuildPageListText(page, label, 128, static_cast<int>(state->pages.size()));
    const int index = AddPageLabelIndex(*state, page, label);

    if (index < 0) {
        state->pageToNavIndex[pPage] = static_cast<int>(state->navPages.size());
        state->navPages.push_back(pPage);
    }
    state->pages.push_back(pPage);
    state->categoryPages[pCategory].push_back(pPage);
}

// Symbol: ?AddTreeCategory@CMFCPropertySheet@@QEAAPEAVCMFCPropertySheetCategoryInfo@@PEB_WHHPEBV2@@Z
extern "C" void* MS_ABI impl__AddTreeCategory_CMFCPropertySheet__QEAAPEAVCMFCPropertySheetCategoryInfo__PEB_WHHPEBV2__Z(
    void* pThis, const wchar_t* lpszName, int nIcon, int nSelIcon, const void* pParentCategory) {
    auto* storage = static_cast<unsigned char*>(::operator new[](kSizeof_CMFCPropertySheetCategoryInfo));
    auto* category = impl___0CMFCPropertySheetCategoryInfo__QEAA_PEB_WHHPEBV0_AEAVCMFCPropertySheet___Z(
        storage, lpszName, nIcon, nSelIcon, pParentCategory, pThis);
    if (!category) {
        ::operator delete[](storage);
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_stateMutex);
    g_categoryMap[category] = storage;
    return category;
}

// Symbol: ?RemoveCategory@CMFCPropertySheet@@QEAAXPEAVCMFCPropertySheetCategoryInfo@@@Z
extern "C" void MS_ABI impl__RemoveCategory_CMFCPropertySheet__QEAAXPEAVCMFCPropertySheetCategoryInfo___Z(
    void* pThis, void* pCategory) {
    if (!pCategory) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_stateMutex);

    const auto* owner = const_cast<const void*>(CategoryGetOwnerPtr(pCategory));
    if (owner != nullptr) {
        auto stateIt = g_sheetStates.find(owner);
        if (stateIt != g_sheetStates.end()) {
            CMFCPropertySheetState& state = stateIt->second;
            EraseFromVector(state.categories, pCategory);
            EraseFromVector(state.rootCategories, pCategory);

            if (auto parentIt = state.categoryParent.find(pCategory);
                parentIt != state.categoryParent.end()) {
                auto parent = parentIt->second;
                auto childIt = state.categoryChildren.find(parent);
                if (childIt != state.categoryChildren.end()) {
                    EraseFromVector(childIt->second, pCategory);
                }
            }

            state.categoryParent.erase(pCategory);
            state.categoryChildren.erase(pCategory);
            if (auto catIt = state.categoryPages.find(pCategory);
                catIt != state.categoryPages.end()) {
                for (void* page : catIt->second) {
                    state.pageToCategory.erase(page);
                }
                state.categoryPages.erase(catIt);
            }
        }
    }

    if (pThis != nullptr) {
        // keep compatibility if RemoveCategory called explicitly on a sheet-managed node
        // before we resolve the owner from embedded metadata
        auto extraIt = g_sheetStates.find(pThis);
        if (extraIt != g_sheetStates.end()) {
            CMFCPropertySheetState& extraState = extraIt->second;
            EraseFromVector(extraState.categories, pCategory);
            EraseFromVector(extraState.rootCategories, pCategory);
            extraState.categoryParent.erase(pCategory);
            extraState.categoryChildren.erase(pCategory);
            if (auto catIt = extraState.categoryPages.find(pCategory);
                catIt != extraState.categoryPages.end()) {
                for (void* page : catIt->second) {
                    extraState.pageToCategory.erase(page);
                }
                extraState.categoryPages.erase(catIt);
            }
        }
    }

    auto it = g_categoryMap.find(pCategory);
    if (it == g_categoryMap.end()) {
        return;
    }
    impl___1CMFCPropertySheetCategoryInfo__UEAA_XZ(it->second);
    ::operator delete[](it->second);
    g_categoryMap.erase(it);
}

// Symbol: ?OnGetDispInfo@CMFCPropertySheet@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnGetDispInfo_CMFCPropertySheet__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    (void)pThis;
    (void)pNMHDR;
    if (pResult) {
        *pResult = 0;
    }
}

// Symbol: ?OnRemoveTreePage@CMFCPropertySheet@@UEAAHPEAVCPropertyPage@@@Z
extern "C" int MS_ABI impl__OnRemoveTreePage_CMFCPropertySheet__UEAAHPEAVCPropertyPage___Z(
    void* pThis, CPropertyPage* pPage) {
    auto* sheet = static_cast<CPropertySheet*>(pThis);
    if (!sheet || !pPage) {
        return FALSE;
    }
    {
        std::lock_guard<std::mutex> lock(g_stateMutex);
        if (auto* state = GetSheetState(pThis, false)) {
            RemovePageFromState(*state, pPage);
        }
    }
    const int page = sheet->GetPageIndex(pPage);
    if (page < 0) {
        return FALSE;
    }
    impl__RemovePage_CPropertySheet__QEAAXH_Z(sheet, page);
    return TRUE;
}

// Symbol: ?OnSelectList@CMFCPropertySheet@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelectList_CMFCPropertySheet__IEAAXXZ(void* pThis) {
    if (auto* sheet = static_cast<CPropertySheet*>(pThis)) {
        if (auto* state = GetSheetState(pThis, false)) {
            if (state->navWnd) {
                auto* navWnd = static_cast<CWnd*>(state->navWnd);
                if (navWnd && navWnd->GetSafeHwnd()) {
                    const int sel = static_cast<int>(::SendMessageW(
                        navWnd->m_hWnd, LB_GETCURSEL, 0, 0));
                    if (sel >= 0 && sel < static_cast<int>(state->navPages.size())) {
                        if (auto* selectedPage = static_cast<CPropertyPage*>(state->navPages[sel])) {
                            sheet->SetActivePage(selectedPage);
                            return;
                        }
                    }
                }
            }
            if (sheet->GetPageCount() > 0) {
                sheet->SetActivePage(0);
            }
            return;
        }
    }
}

// Symbol: ?OnSelectTree@CMFCPropertySheet@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnSelectTree_CMFCPropertySheet__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    (void)pNMHDR;
    if (pResult) {
        *pResult = 0;
    }
    impl__OnSelectList_CMFCPropertySheet__IEAAXXZ(pThis);
}

// Symbol: ?OnSettingChange@CMFCPropertySheet@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCPropertySheet__IEAAXIPEB_W_Z(
    void* pThis, unsigned int uFlags, const wchar_t* lpszSection) {
    if (auto* wnd = static_cast<CWnd*>(pThis)) {
        wnd->OnSettingChange(uFlags, lpszSection);
    }
}

// Symbol: ?OnSysColorChange@CMFCPropertySheet@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCPropertySheet__IEAAXXZ(void* pThis) {
    if (auto* wnd = static_cast<CWnd*>(pThis)) {
        wnd->OnSysColorChange();
    }
}

// =============================================================================
// Visual / icon / look (ords 13253, 13254, 13338)
// =============================================================================

// Symbol: ?SetIconsList@CMFCPropertySheet@@QEAAHIHK@Z
extern "C" int MS_ABI impl__SetIconsList_CMFCPropertySheet__QEAAHIHK_Z(
    void* pThis, unsigned int nImageList, int cx, unsigned long crMask) {
    auto* state = GetSheetState(pThis, true);
    if (state) {
        state->nImageList = nImageList;
        state->iconCx = cx;
    }
    (void)crMask;
    return state && state->nImageList != 0 ? 1 : -1;
}

// Symbol: ?SetIconsList@CMFCPropertySheet@@QEAAXPEAU_IMAGELIST@@@Z
extern "C" void MS_ABI impl__SetIconsList_CMFCPropertySheet__QEAAXPEAU_IMAGELIST___Z(
    void* pThis, void* pImageList) {
    if (auto* state = GetSheetState(pThis, true)) {
        state->imagelist = pImageList;
    }
}

// Symbol: ?SetLook@CMFCPropertySheet@@QEAAXW4PropSheetLook@1@H@Z
extern "C" void MS_ABI impl__SetLook_CMFCPropertySheet__QEAAXW4PropSheetLook_1_H_Z(
    void* pThis, int look, int nNavWidth) {
    if (auto* state = GetSheetState(pThis, true)) {
        state->look = look;
        state->navBarWidth = (nNavWidth > 0 ? nNavWidth : kDefaultNavBarWidth);
    }
    if (auto* sheet = static_cast<CWnd*>(pThis)) {
        sheet->Invalidate();
        auto* state = GetSheetState(pThis, false);
        if (state && state->navWnd) {
            auto* nav = static_cast<CWnd*>(state->navWnd);
            if (nav->GetSafeHwnd()) {
                RECT area{};
                ::GetClientRect(nav->m_hWnd, &area);
                nav->SetWindowPos(
                    nullptr, 0, 0, state->navBarWidth, area.bottom - area.top,
                    SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSENDCHANGING);
            }
        }
    }
}
