// CDockSite — OpenMFC implementation.
// Sources: cbarcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int);

// Symbol: ?CreateObject@CDockSite@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDockSite__SAPEAVCObject__XZ() {
    // CDockSite has no concrete class declaration in this phase; keep factory disabled.
    return nullptr;
}
// Symbol: ?GetRuntimeClass@CDockSite@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockSite__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_runtimeClassDockSite;
}
// Symbol: ?GetThisClass@CDockSite@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassDockSite;
}
// Symbol: ?Create@CDockSite@@UEAAHKAEBUtagRECT@@PEAVCWnd@@KPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void* pThis, unsigned long, const RECT& rect, CWnd* pParentWnd, unsigned long, CCreateContext*) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.parent = pParentWnd;
    state.recentRect = NormalizeRect(&rect, 320, 220);
    return TRUE;
}
// Symbol: ?CreateEx@CDockSite@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@KPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CDockSite__UEAAHKKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
    void* pThis, unsigned long, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned long nID, CCreateContext* pContext) {
    return impl__Create_CDockSite__UEAAHKAEBUtagRECT__PEAVCWnd__KPEAUCCreateContext___Z(
        pThis, dwStyle, rect, pParentWnd, nID, pContext);
}
// Symbol: ?DockPane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@PEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT___Z(
    void* pThis, CPane* pPane, int, const RECT* lpRect) {
    if (pThis == nullptr || pPane == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    g_dockSiteCoreState[pThis].recentRect = NormalizeRect(lpRect, 320, 220);
    g_paneCoreState_Cbarcore[pPane].parent = reinterpret_cast<CWnd*>(pThis);
}
// Symbol: ?DockPaneLeftOf@CDockSite@@UEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockSite__UEAAHPEAVCPane__0_Z(void* pThis, CPane* pPaneToDock, CPane* pLeftOf) {
    if (pThis == nullptr || pPaneToDock == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    PaneCoreState& state = g_paneCoreState_Cbarcore[pPaneToDock];
    if (pLeftOf != nullptr) {
        auto it = g_paneCoreState_Cbarcore.find(pLeftOf);
        state.parent = (it != g_paneCoreState_Cbarcore.end()) ? it->second.parent : reinterpret_cast<CWnd*>(pThis);
    } else {
        state.parent = reinterpret_cast<CWnd*>(pThis);
    }
    return TRUE;
}
// Symbol: ?OnSetWindowPos@CDockSite@@UEAAHPEBVCWnd@@AEBVCRect@@I@Z
extern "C" int MS_ABI impl__OnSetWindowPos_CDockSite__UEAAHPEBVCWnd__AEBVCRect__I_Z(void* pThis, const CWnd*, const CRect& rect, unsigned int) {
    if (pThis == nullptr) return FALSE;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&rect, 320, 220);
    ++state.layoutRevision;
    return TRUE;
}
// Symbol: ?ResizeDockSite@CDockSite@@QEAAXHH@Z
extern "C" void MS_ABI impl__ResizeDockSite_CDockSite__QEAAXHH_Z(void* pThis, int cx, int cy) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    CRect& rect = state.recentRect;
    rect.right = rect.left + (cx > 0 ? cx : rect.Width());
    rect.bottom = rect.top + (cy > 0 ? cy : rect.Height());
    ++state.layoutRevision;
}
// Symbol: ?ResizeDockSiteByOffset@CDockSite@@IEAAXHH@Z
extern "C" void MS_ABI impl__ResizeDockSiteByOffset_CDockSite__IEAAXHH_Z(void* pThis, int cx, int cy) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    CRect& rect = state.recentRect;
    rect.right += cx;
    rect.bottom += cy;
    if (rect.Width() <= 0) rect.right = rect.left + 1;
    if (rect.Height() <= 0) rect.bottom = rect.top + 1;
    ++state.layoutRevision;
}
// Symbol: ?ResizeRow@CDockSite@@QEAAHPEAVCDockingPanesRow@@HH@Z
extern "C" int MS_ABI impl__ResizeRow_CDockSite__QEAAHPEAVCDockingPanesRow__HH_Z(void*, void*, int, int) {
    return TRUE;
}
// Symbol: ?ShowPane@CDockSite@@UEAAHPEAVCBasePane@@HHH@Z
extern "C" int MS_ABI impl__ShowPane_CDockSite__UEAAHPEAVCBasePane__HHH_Z(void* pThis, CBasePane* pPane, int bShow, int bDelay, int bActivate) {
    if (pThis == nullptr || pPane == nullptr) return FALSE;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pPane, bShow, bDelay, bActivate);
    return TRUE;
}
// Symbol: ?AdjustDockingLayout@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockSite__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&state.recentRect, 320, 220);
    ++state.layoutRevision;
}
// Symbol: ?AdjustLayout@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CDockSite__UEAAXXZ(void* pThis) {
    impl__AdjustDockingLayout_CDockSite__UEAAXXZ(pThis);
}
// Symbol: ?FixupVirtualRects@CDockSite@@UEAAXXZ
extern "C" void MS_ABI impl__FixupVirtualRects_CDockSite__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    state.recentRect = NormalizeRect(&state.recentRect, 320, 220);
    ++state.layoutRevision;
}
// Symbol: ?OnInsertRow@CDockSite@@UEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__OnInsertRow_CDockSite__UEAAXPEAU__POSITION___Z(void* pThis, void*) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    ++state.rowCount;
    ++state.layoutRevision;
}
// Symbol: ?OnRemoveRow@CDockSite@@UEAAXPEAU__POSITION@@H@Z
extern "C" void MS_ABI impl__OnRemoveRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* pThis, void*, int) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
    DockSiteCoreState& state = g_dockSiteCoreState[pThis];
    if (state.rowCount > 0) --state.rowCount;
    ++state.layoutRevision;
}
// Symbol: ??0CDockSite@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDockSite__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CDockSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockSite__UEAA_XZ(void* pThis) {
    (void)pThis;
}
