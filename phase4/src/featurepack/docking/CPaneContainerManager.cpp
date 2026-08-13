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
// Symbol: ?AddPaneAndPaneContainer@CPaneContainerManager@@IEAAHPEAVCDockablePane@@PEAVCPaneContainer@@K@Z
extern "C" int MS_ABI impl__AddPaneAndPaneContainer_CPaneContainerManager__IEAAHPEAVCDockablePane__PEAVCPaneContainer__K_Z(void* /*class*/* p0, void* /*class*/* p1, unsigned long p2) {
    return 0;
}

// Symbol: ?AddPaneAndPaneDivider@CPaneContainerManager@@IEAAHPEAVCDockablePane@@0PEAU__POSITION@@K@Z
extern "C" int MS_ABI impl__AddPaneAndPaneDivider_CPaneContainerManager__IEAAHPEAVCDockablePane__0PEAU__POSITION__K_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*struct*/* p2, unsigned long p3) {
    return 0;
}

// Symbol: ?AddPaneContainerManager@CPaneContainerManager@@UEAAHAEAV1@H@Z
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHAEAV1_H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?AddPaneContainerManager@CPaneContainerManager@@UEAAHPEAVCDockablePane@@KAEAV1@H@Z
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHPEAVCDockablePane__KAEAV1_H_Z(void* /*class*/* p0, unsigned long p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?AddPaneContainerManagerToDockablePane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@AEAV1@@Z
extern "C" int MS_ABI impl__AddPaneContainerManagerToDockablePane_CPaneContainerManager__UEAAHPEAVCDockablePane__AEAV1__Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?AddPanesToList@CPaneContainerManager@@QEAAXPEAVCObList@@0@Z
extern "C" void MS_ABI impl__AddPanesToList_CPaneContainerManager__QEAAXPEAVCObList__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?AddPaneToList@CPaneContainerManager@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddPaneToList_CPaneContainerManager__QEAAXPEAVCDockablePane___Z(void* /*class*/* p0) {}

// Symbol: ?AddPaneToRecentPaneContainer@CPaneContainerManager@@UEAAPEAVCDockablePane@@PEAV2@PEAVCPaneContainer@@@Z
extern "C" void* MS_ABI impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?CalcRects@CPaneContainerManager@@QEAAXAEAVCRect@@00AEAKKVCSize@@2@Z
extern "C" void MS_ABI impl__CalcRects_CPaneContainerManager__QEAAXAEAVCRect__00AEAKKVCSize__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned long* p3, unsigned long p4, void* /*class*/ p5, unsigned long p6) {}

// Symbol: ?CanBeAttached@CPaneContainerManager@@UEBAHXZ
extern "C" int MS_ABI impl__CanBeAttached_CPaneContainerManager__UEBAHXZ() {
    return 0;
}

// Symbol: ?CheckAndRemoveNonValidPane@CPaneContainerManager@@QEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CheckAndRemoveNonValidPane_CPaneContainerManager__QEAAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?CheckForMiniFrameAndCaption@CPaneContainerManager@@UEAAHVCPoint@@PEAPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__CheckForMiniFrameAndCaption_CPaneContainerManager__UEAAHVCPoint__PEAPEAVCDockablePane___Z(void* /*class*/ p0, void* /*class*/** p1) {
    return 0;
}

// Symbol: ?Create@CPaneContainerManager@@UEAAHPEAVCWnd@@PEAVCPaneDivider@@PEAUCRuntimeClass@@@Z
extern "C" int MS_ABI impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?CreateObject@CPaneContainerManager@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CPaneContainerManager__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?CreatePaneDivider@CPaneContainerManager@@IEAAPEAVCPaneDivider@@VCRect@@KH@Z
extern "C" void* MS_ABI impl__CreatePaneDivider_CPaneContainerManager__IEAAPEAVCPaneDivider__VCRect__KH_Z(void* /*class*/ p0, unsigned long p1, int p2) {
    return nullptr;
}

// Symbol: ?DoesAllowDynInsertBefore@CPaneContainerManager@@UEBAHXZ
extern "C" int MS_ABI impl__DoesAllowDynInsertBefore_CPaneContainerManager__UEBAHXZ() {
    return 0;
}

// Symbol: ?DoesContainFloatingPane@CPaneContainerManager@@UEAAHXZ
extern "C" int MS_ABI impl__DoesContainFloatingPane_CPaneContainerManager__UEAAHXZ() {
    return 0;
}

// Symbol: ?EnableGrippers@CPaneContainerManager@@UEAAXH@Z
extern "C" void MS_ABI impl__EnableGrippers_CPaneContainerManager__UEAAXH_Z(int p0) {}

// Symbol: ?FindTabbedPane@CPaneContainerManager@@QEAAPEAVCDockablePane@@I@Z
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneContainerManager__QEAAPEAVCDockablePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?GetAvailableSpace@CPaneContainerManager@@UEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?GetMinMaxOffset@CPaneContainerManager@@UEAAXPEAVCPaneDivider@@AEAH11@Z
extern "C" void MS_ABI impl__GetMinMaxOffset_CPaneContainerManager__UEAAXPEAVCPaneDivider__AEAH11_Z(void* /*class*/* p0, int* p1, int* p2, int* p3) {}

// Symbol: ?GetMinSize@CPaneContainerManager@@UEAAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSize_CPaneContainerManager__UEAAXAEAVCSize___Z(void* /*class*/* p0) {}

// Symbol: ?GetNodeCount@CPaneContainerManager@@QEBAHXZ
extern "C" int MS_ABI impl__GetNodeCount_CPaneContainerManager__QEBAHXZ() {
    return 0;
}

// Symbol: ?GetTotalRefCount@CPaneContainerManager@@QEBAHXZ
extern "C" int MS_ABI impl__GetTotalRefCount_CPaneContainerManager__QEBAHXZ() {
    return 0;
}

// Symbol: ?GetVisiblePaneCount@CPaneContainerManager@@UEBAHXZ
extern "C" int MS_ABI impl__GetVisiblePaneCount_CPaneContainerManager__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetWindowRect@CPaneContainerManager@@UEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetWindowRect_CPaneContainerManager__UEBAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?HideAll@CPaneContainerManager@@UEAAXXZ
extern "C" void MS_ABI impl__HideAll_CPaneContainerManager__UEAAXXZ() {}

// Symbol: ?InsertPane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@0KPEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__InsertPane_CPaneContainerManager__UEAAHPEAVCDockablePane__0KPEBUtagRECT__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, void* /*class*/* p1, unsigned long p2, const void* /*struct*/* p3, int /*enum*/ p4, short* p5, int p6, char p7, void* p8, void* p9, unsigned long p10, int p11, float p12, unsigned char p13, void* p14, int p15, void* p16, char p17) {
    return 0;
}

// Symbol: ?IsEmpty@CPaneContainerManager@@QEBAHXZ
extern "C" int MS_ABI impl__IsEmpty_CPaneContainerManager__QEBAHXZ() {
    return 0;
}

// Symbol: ?IsRootPaneContainerVisible@CPaneContainerManager@@UEBAHXZ
extern "C" int MS_ABI impl__IsRootPaneContainerVisible_CPaneContainerManager__UEBAHXZ() {
    return 0;
}

// Symbol: ?NotifyPaneDivider@CPaneContainerManager@@QEAAXXZ
extern "C" void MS_ABI impl__NotifyPaneDivider_CPaneContainerManager__QEAAXXZ() {}

// Symbol: ?OnPaneDividerMove@CPaneContainerManager@@UEAAHPEAVCPaneDivider@@IHAEAPEAX@Z
extern "C" int MS_ABI impl__OnPaneDividerMove_CPaneContainerManager__UEAAHPEAVCPaneDivider__IHAEAPEAX_Z(void* /*class*/* p0, unsigned int p1, int p2, void** p3) {
    return 0;
}

// Symbol: ?OnShowPane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@H@Z
extern "C" int MS_ABI impl__OnShowPane_CPaneContainerManager__UEAAHPEAVCDockablePane__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?PaneFromPoint@CPaneContainerManager@@UEAAPEAVCDockablePane@@VCPoint@@HHAEAH1@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(void* /*class*/ p0, int p1, int p2, int* p3, int p4) {
    return nullptr;
}

// Symbol: ?ReleaseEmptyPaneContainers@CPaneContainerManager@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CPaneContainerManager__QEAAXXZ() {}

// Symbol: ?RemoveAllPanesAndPaneDividers@CPaneContainerManager@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllPanesAndPaneDividers_CPaneContainerManager__QEAAXXZ() {}

// Symbol: ?RemoveNonValidPanes@CPaneContainerManager@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneContainerManager__QEAAXXZ() {}

// Symbol: ?RemovePaneDivider@CPaneContainerManager@@UEAAXPEAVCPaneDivider@@@Z
extern "C" void MS_ABI impl__RemovePaneDivider_CPaneContainerManager__UEAAXPEAVCPaneDivider___Z(void* /*class*/* p0) {}

// Symbol: ?RemovePaneFromPaneContainer@CPaneContainerManager@@UEAAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__RemovePaneFromPaneContainer_CPaneContainerManager__UEAAHPEAVCDockablePane___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ReplacePane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@0@Z
extern "C" int MS_ABI impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?ResizePaneContainers@CPaneContainerManager@@UEAAXIHHAEAPEAX@Z
extern "C" void MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXIHHAEAPEAX_Z(unsigned int p0, int p1, int p2, void** p3) {}

// Symbol: ?ResizePaneContainers@CPaneContainerManager@@UEAAXVCRect@@AEAPEAX@Z
extern "C" void MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(void* /*class*/ p0, void** p1) {}

// Symbol: ?Serialize@CPaneContainerManager@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetDefaultPaneDividerForPanes@CPaneContainerManager@@QEAAXPEAVCPaneDivider@@@Z
extern "C" void MS_ABI impl__SetDefaultPaneDividerForPanes_CPaneContainerManager__QEAAXPEAVCPaneDivider___Z(void* /*class*/* p0) {}

// Symbol: ?SetResizeMode@CPaneContainerManager@@UEAAXH@Z
extern "C" void MS_ABI impl__SetResizeMode_CPaneContainerManager__UEAAXH_Z(int p0) {}

// Symbol: ?StoreRecentDockSiteInfo@CPaneContainerManager@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(void* /*class*/* p0) {}
