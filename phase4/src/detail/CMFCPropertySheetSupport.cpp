#include "CMFCPropertySheetSupport.h"

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
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
} } }  // namespace openmfc::detail::cmfcpropertysheet
