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
