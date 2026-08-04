#pragma once

#include "openmfc/afxmfc.h"

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace openmfc::ribbon_state {

struct RibbonPanelState {
    std::vector<CMFCRibbonBaseElement*> elements;
    std::unordered_set<CMFCRibbonBaseElement*> ownedElements;
};

struct RibbonCategoryState {
    std::vector<CMFCRibbonPanel*> panels;
    std::unordered_set<CMFCRibbonPanel*> ownedPanels;
};

struct RibbonBarState {
    CWnd* parent = nullptr;
    std::vector<CMFCRibbonCategory*> categories;
    std::unordered_set<CMFCRibbonCategory*> ownedCategories;
    std::vector<CMFCRibbonBaseElement*> tabs;
    CMFCRibbonButtonsGroup* quickAccessToolbar = nullptr;
    CMFCRibbonCategory* activeCategory = nullptr;
    bool quickAccessToolbarOnTop = true;
    bool minimized = false;
};

inline std::mutex& RibbonMutex() {
    static std::mutex mutex;
    return mutex;
}

inline std::unordered_map<const CMFCRibbonPanel*, RibbonPanelState>& RibbonPanelStates() {
    static std::unordered_map<const CMFCRibbonPanel*, RibbonPanelState> states;
    return states;
}

inline std::unordered_map<const CMFCRibbonCategory*, RibbonCategoryState>& RibbonCategoryStates() {
    static std::unordered_map<const CMFCRibbonCategory*, RibbonCategoryState> states;
    return states;
}

inline std::unordered_map<const CMFCRibbonBar*, RibbonBarState>& RibbonBarStates() {
    static std::unordered_map<const CMFCRibbonBar*, RibbonBarState> states;
    return states;
}

} // namespace openmfc::ribbon_state
