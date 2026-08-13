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
// Symbol: ?AddNode@CPaneContainer@@IEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddNode_CPaneContainer__IEAAXPEAV1__Z(void* /*class*/* p0) {}

// Symbol: ?AddRef@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__AddRef_CPaneContainer__QEAAXXZ() {}

// Symbol: ?AddSubPaneContainer@CPaneContainer@@QEAAHPEAV1@H@Z
extern "C" int MS_ABI impl__AddSubPaneContainer_CPaneContainer__QEAAHPEAV1_H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?CalcAvailablePaneSpace@CPaneContainer@@UEAAHHPEAVCPane@@PEAV1@H@Z
extern "C" int MS_ABI impl__CalcAvailablePaneSpace_CPaneContainer__UEAAHHPEAVCPane__PEAV1_H_Z(int p0, void* /*class*/* p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?CalcAvailableSpace@CPaneContainer@@UEAA?AVCSize@@V2@H@Z
extern "C" void* MS_ABI impl__CalcAvailableSpace_CPaneContainer__UEAA_AVCSize__V2_H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {
    return nullptr;
}

// Symbol: ?CalculateRecentSize@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__CalculateRecentSize_CPaneContainer__QEAAXXZ() {}

// Symbol: ?CheckPaneDividerVisibility@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ() {}

// Symbol: ?CleanUp@CPaneContainer@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CPaneContainer__IEAAXXZ() {}

// Symbol: ?Copy@CPaneContainer@@UEAAPEAV1@PEAV1@@Z
extern "C" void* MS_ABI impl__Copy_CPaneContainer__UEAAPEAV1_PEAV1__Z() {
    return nullptr;
}

// Symbol: ?DeletePane@CPaneContainer@@UEAAXPEAVCDockablePane@@W4BC_FIND_CRITERIA@1@@Z
extern "C" void MS_ABI impl__DeletePane_CPaneContainer__UEAAXPEAVCDockablePane__W4BC_FIND_CRITERIA_1__Z(void* /*class*/* p0, int /*enum*/ p1, void* p2, void* p3, int p4, short p5, unsigned int p6, double p7, char p8, int p9, void* p10, void* p11, unsigned int p12, void* p13, unsigned char p14, void* p15, unsigned int p16, void** p17, int /*enum*/ p18) {}

// Symbol: ?FindSubPaneContainer@CPaneContainer@@QEAAPEAV1@PEBVCObject@@W4BC_FIND_CRITERIA@1@@Z
extern "C" void* MS_ABI impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(int /*enum*/ p0, void* p1, void* p2, int p3, short p4, unsigned int p5, double p6, char p7, int p8, void* p9, void* p10, unsigned int p11, void* p12, unsigned char p13, void* p14, unsigned int p15, void** p16, void* p17) {
    return nullptr;
}

// Symbol: ?FindTabbedPane@CPaneContainer@@QEAAPEAVCDockablePane@@I@Z
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FreeReleasedPaneContainer@CPaneContainer@@IEAAXXZ
extern "C" void MS_ABI impl__FreeReleasedPaneContainer_CPaneContainer__IEAAXXZ() {}

// Symbol: ?GetAssociatedSiblingPaneIDs@CPaneContainer@@QEAAPEAV?$CList@II@@PEAVCDockablePane@@@Z
extern "C" void* MS_ABI impl__GetAssociatedSiblingPaneIDs_CPaneContainer__QEAAPEAV__CList_II__PEAVCDockablePane___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?GetMinSize@CPaneContainer@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(void* /*class*/* p0) {}

// Symbol: ?GetMinSizeLeft@CPaneContainer@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSizeLeft_CPaneContainer__UEBAXAEAVCSize___Z(void* /*class*/* p0) {}

// Symbol: ?GetMinSizeRight@CPaneContainer@@UEBAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetMinSizeRight_CPaneContainer__UEBAXAEAVCSize___Z(void* /*class*/* p0) {}

// Symbol: ?GetNodeCount@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__GetNodeCount_CPaneContainer__QEBAHXZ() {
    return 0;
}

// Symbol: ?GetResizeStep@CPaneContainer@@UEBAHXZ
extern "C" int MS_ABI impl__GetResizeStep_CPaneContainer__UEBAHXZ() {
    return 0;
}

// Symbol: ?GetTotalReferenceCount@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ() {
    return 0;
}

// Symbol: ?GetWindowRect@CPaneContainer@@UEBAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?IsEmpty@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__IsEmpty_CPaneContainer__QEBAHXZ() {
    return 0;
}

// Symbol: ?IsLeftPane@CPaneContainer@@QEBAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__IsLeftPane_CPaneContainer__QEBAHPEAVCDockablePane___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsLeftPaneContainer@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__IsLeftPaneContainer_CPaneContainer__QEBAHXZ() {
    return 0;
}

// Symbol: ?IsLeftPartEmpty@CPaneContainer@@QEBAHH@Z
extern "C" int MS_ABI impl__IsLeftPartEmpty_CPaneContainer__QEBAHH_Z(int p0) {
    return 0;
}

// Symbol: ?IsPaneDividerHorz@CPaneContainer@@IEBAHXZ
extern "C" int MS_ABI impl__IsPaneDividerHorz_CPaneContainer__IEBAHXZ() {
    return 0;
}

// Symbol: ?IsRightPartEmpty@CPaneContainer@@QEBAHH@Z
extern "C" int MS_ABI impl__IsRightPartEmpty_CPaneContainer__QEBAHH_Z(int p0) {
    return 0;
}

// Symbol: ?IsVisible@CPaneContainer@@QEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CPaneContainer__QEBAHXZ() {
    return 0;
}

// Symbol: ?LoadTabbedPane@CPaneContainer@@IEAAPEAVCDockablePane@@AEAVCArchive@@AEAV?$CList@II@@@Z
extern "C" void* MS_ABI impl__LoadTabbedPane_CPaneContainer__IEAAPEAVCDockablePane__AEAVCArchive__AEAV__CList_II___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return nullptr;
}

// Symbol: ?Move@CPaneContainer@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__Move_CPaneContainer__UEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?MoveWnd@CPaneContainer@@IEAAXPEAVCWnd@@HH@Z
extern "C" void MS_ABI impl__MoveWnd_CPaneContainer__IEAAXPEAVCWnd__HH_Z(void* /*class*/* p0, int p1, int p2) {}

// Symbol: ?OnMoveInternalPaneDivider@CPaneContainer@@UEAAHHAEAPEAX@Z
extern "C" int MS_ABI impl__OnMoveInternalPaneDivider_CPaneContainer__UEAAHHAEAPEAX_Z(int p0, void** p1) {
    return 0;
}

// Symbol: ?OnShowPane@CPaneContainer@@UEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__OnShowPane_CPaneContainer__UEAAXPEAVCDockablePane__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?Release@CPaneContainer@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CPaneContainer__QEAAKXZ() {
    return 0;
}

// Symbol: ?ReleaseEmptyPaneContainer@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ() {}

// Symbol: ?RemoveNonValidPanes@CPaneContainer@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ() {}

// Symbol: ?Resize@CPaneContainer@@UEAAXVCRect@@AEAPEAXH@Z
extern "C" void MS_ABI impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(void* /*class*/ p0, void** p1, int p2) {}

// Symbol: ?ResizePane@CPaneContainer@@UEAAXHPEAVCPane@@PEAV1@HHAEAPEAX@Z
extern "C" void MS_ABI impl__ResizePane_CPaneContainer__UEAAXHPEAVCPane__PEAV1_HHAEAPEAX_Z(int p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4, void** p5) {}

// Symbol: ?ResizePartOfPaneContainer@CPaneContainer@@UEAAXHHAEAPEAX@Z
extern "C" void MS_ABI impl__ResizePartOfPaneContainer_CPaneContainer__UEAAXHHAEAPEAX_Z(int p0, int p1, void** p2) {}

// Symbol: ?SaveTabbedPane@CPaneContainer@@IEAAXAEAVCArchive@@PEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__SaveTabbedPane_CPaneContainer__IEAAXAEAVCArchive__PEAVCDockablePane___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?Serialize@CPaneContainer@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CPaneContainer__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetPane@CPaneContainer@@QEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__SetPane_CPaneContainer__QEAAXPEAVCDockablePane__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?SetPaneContainer@CPaneContainer@@QEAAXPEAV1@H@Z
extern "C" void MS_ABI impl__SetPaneContainer_CPaneContainer__QEAAXPEAV1_H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?SetPaneContainerManager@CPaneContainer@@IEAAXPEAVCPaneContainerManager@@H@Z
extern "C" void MS_ABI impl__SetPaneContainerManager_CPaneContainer__IEAAXPEAVCPaneContainerManager__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?SetUpByID@CPaneContainer@@QEAAHIPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__SetUpByID_CPaneContainer__QEAAHIPEAVCDockablePane___Z(unsigned int p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?StoreRecentDockSiteInfo@CPaneContainer@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneContainer__UEAAXPEAVCDockablePane___Z(void* /*class*/* p0) {}

// Symbol: ?StretchPaneContainer@CPaneContainer@@UEAAHHHHHAEAPEAX@Z
extern "C" int MS_ABI impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(int p0, int p1, int p2, int p3, void** p4) {
    return 0;
}
