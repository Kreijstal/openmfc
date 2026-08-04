// CPaneContainerManager — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ??0CPaneContainerManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneContainerManager__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerManagerPanes[pThis] = {};
    return pThis;
}
// Symbol: ??1CPaneContainerManager@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneContainerManager__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerManagerPanes.erase(pThis);
    return pThis;
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
