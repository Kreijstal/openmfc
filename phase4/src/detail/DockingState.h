#pragma once

#include "openmfc/afxmfc.h"

#include <cstddef>
#include <mutex>
#include <string>
#include <unordered_map>

namespace openmfc::docking_state {

struct CaptionButtonState {
    UINT id = 0;
    RECT rect{};
    BOOL visible = FALSE;
    BOOL enabled = FALSE;
    BOOL hot = FALSE;
    BOOL pressed = FALSE;
};

struct DockingTooltipState {
    HWND target = nullptr;
    UINT id = 0;
    RECT rect{};
    std::wstring text;
};

// pThis-keyed fallback state for members that the compact public pane classes
// cannot hold. Retail offsets are recorded beside each value; these fields are
// not overlays and must be accessed through this map, never by adding the
// retail offset to a local CBasePane/CDockablePane allocation.
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
    void* autoHideBar = nullptr;                  // retail CMFCAutoHideBar* m_pAutoHideBar +0x480
    CMFCAutoHideButton* autoHideButton = nullptr; // m_pAutoHideButton +0x478

    CaptionButtonState* captionButtons = nullptr;  // semantic CObArray payload; retail m_arrButtons starts +0x488, m_pData +0x490
    INT_PTR captionButtonCount = 0;                // CArray m_nSize +0x498
    INT_PTR captionButtonCapacity = 0;              // CArray m_nMaxSize +0x4a0
    INT_PTR captionButtonGrowBy = 0;                // CArray m_nGrowBy +0x4a8
    CRuntimeClass* tabbedControlBarRTC = nullptr;   // CDockablePane m_pTabbedControlBarRTC +0x4b0
    CToolTipCtrl* toolTip = nullptr;                // CDockablePane m_pToolTip +0x4b8
    HWND defaultSliderHwnd = nullptr;               // CDockablePane m_hDefaultSlider +0x4c0
    HWND restoredDefaultSliderHwnd = nullptr;       // CDockablePane m_hRestoredDefaultSlider +0x4c8
    UINT hotCaptionButtonID = 0xffffffffu;          // CDockablePane m_nHot +0x4d0
    UINT hitCaptionButtonID = 0xffffffffu;          // CDockablePane m_nHit +0x4d4
    BOOL captionButtonsCaptured = FALSE;            // CDockablePane m_bCaptionButtonsCaptured +0x4d8
    DockingTooltipState* toolTips = nullptr;         // semantic tooltip registrations, side storage
    std::size_t toolTipCount = 0;
    std::size_t toolTipCapacity = 0;

    // CBaseTabbedPane is 0x4d8 locally. Its CArray member starts at +0x4f0;
    // these side-table values model the CArray payload, not retail in-object
    // fields. Offsets are established by FillDefaultTabsOrderArray (entry RVA
    // 0x14b10, mfc140u) and the CBaseTabbedPane shipping header.
    CMFCTabCtrl* tabWnd = nullptr;                  // m_pTabWnd +0x4e8
    UINT* defaultTabOrder = nullptr;                // m_arDefaultTabsOrder.m_pData +0x4f8
    INT_PTR defaultTabOrderCount = 0;               // m_nSize +0x500
    INT_PTR defaultTabOrderCapacity = 0;             // m_nMaxSize +0x508
    INT_PTR defaultTabOrderGrowBy = 0;               // m_nGrowBy +0x510
    UINT* siblingPaneIDs = nullptr;
    INT_PTR siblingPaneIDCount = 0;
};

inline std::mutex& PaneCoreStateMutex() {
    static std::mutex mutex;
    return mutex;
}

inline std::unordered_map<const void*, PaneCoreState>& PaneCoreStates() {
    static std::unordered_map<const void*, PaneCoreState> states;
    return states;
}

// CBaseTabbedPane uses the scalar/pointer fields in PaneCoreState above; no
// second map or in-object extension is introduced.
// CDockingManager::SetDockingMode has process-global smart-docking theme state.
// This accessor lets other TUs share the value without changing the ABI class.
inline int& DockingSmartTheme() {
    static int theme = 0;
    return theme;
}

} // namespace openmfc::docking_state
