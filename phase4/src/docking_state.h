#pragma once

#include "openmfc/afxmfc.h"

#include <mutex>
#include <unordered_map>

namespace openmfc::docking_state {

struct PaneCoreState {
    CWnd* parent = nullptr;
    CSize minSize = CSize(0, 0);
    CRect recentRect = CRect(0, 0, 0, 0);
    BOOL visible = FALSE;
    DWORD alignment = 0;
    DWORD enabledDocking = 0;
    DWORD autoHideAlignment = 0;
    BOOL canFloat = TRUE;
    BOOL canAutoHide = FALSE;
    BOOL hasGripper = FALSE;
    BOOL autoHideMode = FALSE;
    BOOL tabbed = FALSE;
    void* autoHideBar = nullptr;
};

inline std::mutex& PaneCoreStateMutex() {
    static std::mutex mutex;
    return mutex;
}

inline std::unordered_map<const void*, PaneCoreState>& PaneCoreStates() {
    static std::unordered_map<const void*, PaneCoreState> states;
    return states;
}

inline PaneCoreState& EnsurePaneCoreState(const void* pane) {
    return PaneCoreStates()[pane];
}

inline const PaneCoreState* FindPaneCoreState(const void* pane) {
    auto& states = PaneCoreStates();
    auto it = states.find(pane);
    return it == states.end() ? nullptr : &it->second;
}

inline void RemovePaneCoreState(const void* pane) {
    PaneCoreStates().erase(pane);
}

} // namespace openmfc::docking_state
