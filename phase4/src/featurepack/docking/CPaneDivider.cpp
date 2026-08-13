// CPaneDivider — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ??0CPaneDivider@@QEAA@HPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CPaneDivider__QEAA_HPEAVCWnd___Z(void* pThis, int, void*) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_dividerPanes[pThis] = {};
    return pThis;
}
// Symbol: ??0CPaneDivider@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneDivider__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_dividerPanes[pThis] = {};
    return pThis;
}
// Symbol: ??1CPaneDivider@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneDivider__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_dividerPanes.erase(pThis);
    return pThis;
}
// Symbol: ?AddPane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_dividerPanes[pThis];
    AddUniquePane(state, pPane);
    const int paneCount = static_cast<int>(state.panes.size());
    state.recentRect = CRect(0, 0, std::max(80, paneCount * kPaneDividerUnitWidth), kPaneDividerThickness);
}
// Symbol: ?RemovePane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_dividerPanes[pThis];
    RemovePane(state, pPane);
    if (state.panes.empty()) {
        state.recentRect.SetRectEmpty();
    } else {
        const int paneCount = static_cast<int>(state.panes.size());
        state.recentRect = CRect(0, 0, std::max(80, paneCount * kPaneDividerUnitWidth), kPaneDividerThickness);
    }
}
// Symbol: ?CheckVisibility@CPaneDivider@@UEAAHXZ
extern "C" int MS_ABI impl__CheckVisibility_CPaneDivider__UEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_dividerPanes[pThis].panes.empty() ? FALSE : TRUE;
}
// Symbol: ?FindPaneContainer@CPaneDivider@@QEAAPEAVCPaneContainer@@PEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneDivider__QEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pIndex) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)pThis;
    if (pIndex) *pIndex = 0;
    auto it = g_paneToContainer.find(pPane);
    return it == g_paneToContainer.end() ? nullptr : it->second;
}
// Symbol: ?GetFirstPane@CPaneDivider@@QEBAPEBVCBasePane@@XZ
extern "C" void* MS_ABI impl__GetFirstPane_CPaneDivider__QEBAPEBVCBasePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_dividerPanes[pThis]);
}
// Symbol: ?GetPaneDividers@CPaneDivider@@QEAAXAEAVCObList@@@Z
extern "C" void MS_ABI impl__GetPaneDividers_CPaneDivider__QEAAXAEAVCObList___Z(void* pThis, CObList* pList) {
    if (!pList) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pList->RemoveAll();
    const auto it = g_dividerPanes.find(pThis);
    if (it != g_dividerPanes.end() && pThis != nullptr) {
        pList->AddTail(reinterpret_cast<CObject*>(pThis));
    }
}
// Symbol: ?GetPanes@CPaneDivider@@QEAAXAEAVCObList@@@Z
extern "C" void MS_ABI impl__GetPanes_CPaneDivider__QEAAXAEAVCObList___Z(void* pThis, CObList* pList) {
    if (!pList) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pList->RemoveAll();
    const auto it = g_dividerPanes.find(pThis);
    if (it == g_dividerPanes.end()) return;
    for (void* pane : it->second.panes) {
        if (pane != nullptr) {
            pList->AddTail(reinterpret_cast<CObject*>(pane));
        }
    }
}
// Symbol: ?GetRootContainerRect@CPaneDivider@@QEAA?AVCRect@@XZ
extern "C" void MS_ABI impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(CRect* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_dividerPanes[pThis];
    if (state.recentRect.Width() > 0 && state.recentRect.Height() > 0) {
        *pRet = state.recentRect;
    } else {
        pRet->SetRect(0, 0, state.panes.empty() ? 0 : 240, state.panes.empty() ? 0 : 140);
    }
}
// Symbol: ?CalcFixedLayout@CPaneDivider@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CPaneDivider__UEAA_AVCSize__HH_Z(CSize* pRet, void* pThis, int, int) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_dividerPanes[pThis];
    if (state.recentRect.Width() > 0 && state.recentRect.Height() > 0) {
        pRet->cx = state.recentRect.Width();
        pRet->cy = state.recentRect.Height();
        return;
    }
    const int paneCount = static_cast<int>(state.panes.size());
    pRet->cx = paneCount > 0 ? std::max(80, 80 * paneCount) : 0;
    pRet->cy = paneCount > 0 ? kPaneDividerThickness : 0;
}
void CPaneDivider::AddPane(CDockablePane* pPane) {
    impl__AddPane_CPaneDivider__UEAAXPEAVCDockablePane___Z(this, pPane);
}
void CPaneDivider::RemovePane(CDockablePane* pPane) {
    impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(this, pPane);
}
BOOL CPaneDivider::CheckVisibility() {
    return impl__CheckVisibility_CPaneDivider__UEAAHXZ(this);
}
CPaneContainer* CPaneDivider::FindPaneContainer(CDockablePane* pBar, int& nIndex) {
    return reinterpret_cast<CPaneContainer*>(
        impl__FindPaneContainer_CPaneDivider__QEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(this, pBar, &nIndex));
}
const CBasePane* CPaneDivider::GetFirstPane() const {
    return reinterpret_cast<const CBasePane*>(
        impl__GetFirstPane_CPaneDivider__QEBAPEBVCBasePane__XZ(const_cast<CPaneDivider*>(this)));
}
void CPaneDivider::GetPaneDividers(CObList& lst) {
    impl__GetPaneDividers_CPaneDivider__QEAAXAEAVCObList___Z(this, &lst);
}
void CPaneDivider::GetPanes(CObList& lst) {
    impl__GetPanes_CPaneDivider__QEAAXAEAVCObList___Z(this, &lst);
}
CRect CPaneDivider::GetRootContainerRect() {
    CRect rect;
    impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(&rect, this);
    return rect;
}
CSize CPaneDivider::CalcFixedLayout(BOOL bStretch, BOOL bHorz) {
    CSize size;
    impl__CalcFixedLayout_CPaneDivider__UEAA_AVCSize__HH_Z(&size, this, bStretch, bHorz);
    return size;
}
// Symbol: ?AddPaneContainer@CPaneDivider@@UEAAHAEAVCPaneContainerManager@@H@Z
extern "C" int MS_ABI impl__AddPaneContainer_CPaneDivider__UEAAHAEAVCPaneContainerManager__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?AddPaneContainer@CPaneDivider@@UEAAHPEAVCDockablePane@@AEAVCPaneContainerManager@@K@Z
extern "C" int MS_ABI impl__AddPaneContainer_CPaneDivider__UEAAHPEAVCDockablePane__AEAVCPaneContainerManager__K_Z(void* /*class*/* p0, void* /*class*/* p1, unsigned long p2) {
    return 0;
}

// Symbol: ?AddRecentPane@CPaneDivider@@UEAAPEAVCDockablePane@@PEAV2@@Z
extern "C" void* MS_ABI impl__AddRecentPane_CPaneDivider__UEAAPEAVCDockablePane__PEAV2__Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?CalcExpectedDockedRect@CPaneDivider@@UEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CPaneDivider__UEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int* p3, void* /*class*/** p4) {}

// Symbol: ?Create@CPaneDivider@@UEAAHKAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CPaneDivider__UEAAHKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(unsigned long p0, const void* /*struct*/* p1, void* /*class*/* p2, unsigned int p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?CreateEx@CPaneDivider@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CPaneDivider__UEAAHKKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(unsigned long p0, unsigned long p1, const void* /*struct*/* p2, void* /*class*/* p3, unsigned int p4, void* /*struct*/* p5) {
    return 0;
}

// Symbol: ?DoesAllowDynInsertBefore@CPaneDivider@@UEBAHXZ
extern "C" int MS_ABI impl__DoesAllowDynInsertBefore_CPaneDivider__UEBAHXZ() {
    return 0;
}

// Symbol: ?DoesContainFloatingPane@CPaneDivider@@UEAAHXZ
extern "C" int MS_ABI impl__DoesContainFloatingPane_CPaneDivider__UEAAHXZ() {
    return 0;
}

// Symbol: ?FindTabbedPane@CPaneDivider@@QEAAPEAVCDockablePane@@I@Z
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneDivider__QEAAPEAVCDockablePane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?Init@CPaneDivider@@QEAAXHPEAVCWnd@@@Z
extern "C" void MS_ABI impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(int p0, void* /*class*/* p1) {}

// Symbol: ?InsertPane@CPaneDivider@@UEAAHPEAVCDockablePane@@0KPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__InsertPane_CPaneDivider__UEAAHPEAVCDockablePane__0KPEBUtagRECT___Z(void* /*class*/* p0, void* /*class*/* p1, unsigned long p2, const void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?Move@CPaneDivider@@UEAAXAEAVCPoint@@H@Z
extern "C" void MS_ABI impl__Move_CPaneDivider__UEAAXAEAVCPoint__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?NotifyAboutRelease@CPaneDivider@@UEAAXXZ
extern "C" void MS_ABI impl__NotifyAboutRelease_CPaneDivider__UEAAXXZ() {}

// Symbol: ?OnCancelMode@CPaneDivider@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CPaneDivider__IEAAXXZ() {}

// Symbol: ?OnCreate@CPaneDivider@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CPaneDivider__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDestroy@CPaneDivider@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CPaneDivider__IEAAXXZ() {}

// Symbol: ?OnEraseBkgnd@CPaneDivider@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPaneDivider__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnLButtonDown@CPaneDivider@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CPaneDivider__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnLButtonUp@CPaneDivider@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CPaneDivider__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnMouseMove@CPaneDivider@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CPaneDivider__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnNcDestroy@CPaneDivider@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CPaneDivider__IEAAXXZ() {}

// Symbol: ?OnPaint@CPaneDivider@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CPaneDivider__IEAAXXZ() {}

// Symbol: ?OnSetCursor@CPaneDivider@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CPaneDivider__IEAAHPEAVCWnd__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?OnShowPane@CPaneDivider@@UEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__OnShowPane_CPaneDivider__UEAAXPEAVCDockablePane__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnSize@CPaneDivider@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPaneDivider__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?ReleaseEmptyPaneContainers@CPaneDivider@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CPaneDivider__QEAAXXZ() {}

// Symbol: ?ReplacePane@CPaneDivider@@UEAAHPEAVCDockablePane@@0@Z
extern "C" int MS_ABI impl__ReplacePane_CPaneDivider__UEAAHPEAVCDockablePane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?RepositionPanes@CPaneDivider@@UEAAXAEAVCRect@@AEAPEAX@Z
extern "C" void MS_ABI impl__RepositionPanes_CPaneDivider__UEAAXAEAVCRect__AEAPEAX_Z(void* /*class*/* p0, void** p1) {}

// Symbol: ?Serialize@CPaneDivider@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CPaneDivider__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?ShowWindow@CPaneDivider@@QEAAXH@Z
extern "C" void MS_ABI impl__ShowWindow_CPaneDivider__QEAAXH_Z(int p0) {}

// Symbol: ?StopTracking@CPaneDivider@@MEAAXH@Z
extern "C" void MS_ABI impl__StopTracking_CPaneDivider__MEAAXH_Z(int p0) {}

// Symbol: ?StoreRecentDockSiteInfo@CPaneDivider@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneDivider__QEAAXPEAVCDockablePane___Z(void* /*class*/* p0) {}

// Symbol: ?StoreRecentTabRelatedInfo@CPaneDivider@@QEAAXPEAVCDockablePane@@0@Z
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CPaneDivider__QEAAXPEAVCDockablePane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}
