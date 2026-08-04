// CMFCPropertySheet — OpenMFC implementation.
// Sources: global_cmfcpropertysheet.cpp

#include "detail/CMFCPropertySheetSupport.h"

// Symbol: ??0CMFCPropertySheet@@QEAA@IPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCPropertySheet__QEAA_IPEAVCWnd__I_Z(
    void* pThis, unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    (void)nIDCaption;
    InitBase_Cmfcpropertysheet(pThis, pParentWnd, iSelectPage);
    static_cast<CPropertySheet*>(pThis)->m_pszCaption = nullptr;
    return pThis;
}
// Symbol: ??0CMFCPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z
extern "C" void* MS_ABI impl___0CMFCPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(
    void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    InitBase_Cmfcpropertysheet(pThis, pParentWnd, iSelectPage);
    static_cast<CPropertySheet*>(pThis)->m_pszCaption = pszCaption;
    return pThis;
}
// Symbol: ??0CMFCPropertySheet@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertySheet__QEAA_XZ(void* pThis) {
    InitBase_Cmfcpropertysheet(pThis, nullptr, 0);
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
