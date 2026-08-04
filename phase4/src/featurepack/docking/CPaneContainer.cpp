// CPaneContainer — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

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
// Symbol: ??1CPaneContainer@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneContainer__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerPanes.erase(pThis);
    return pThis;
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
