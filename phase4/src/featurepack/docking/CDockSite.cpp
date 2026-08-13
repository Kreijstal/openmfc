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
// Symbol: ?AddRow@CDockSite@@QEAAPEAVCDockingPanesRow@@PEAU__POSITION@@H@Z
extern "C" void* MS_ABI impl__AddRow_CDockSite__QEAAPEAVCDockingPanesRow__PEAU__POSITION__H_Z(void* /*struct*/* p0, int p1) {
    return nullptr;
}

// Symbol: ?AlignDockSite@CDockSite@@QEAAXAEBVCRect@@AEAV2@H@Z
extern "C" void MS_ABI impl__AlignDockSite_CDockSite__QEAAXAEBVCRect__AEAV2_H_Z(const void* /*class*/* p0, void* /*class*/* p1, int p2) {}

// Symbol: ?CalcFixedLayout@CDockSite@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CDockSite__UEAA_AVCSize__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?CalcWindowRect@CDockSite@@MEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CDockSite__MEAAXPEAUtagRECT__I_Z(void* /*struct*/* p0, unsigned int p1) {}

// Symbol: ?CanAcceptPane@CDockSite@@UEBAHPEBVCBasePane@@@Z
extern "C" int MS_ABI impl__CanAcceptPane_CDockSite__UEBAHPEBVCBasePane___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?CreateRow@CDockSite@@UEAAPEAVCDockingPanesRow@@PEAV1@HH@Z
extern "C" void* MS_ABI impl__CreateRow_CDockSite__UEAAPEAVCDockingPanesRow__PEAV1_HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?FindNextVisibleRow@CDockSite@@IEAAPEAVCDockingPanesRow@@AEAPEAU__POSITION@@H@Z
extern "C" void* MS_ABI impl__FindNextVisibleRow_CDockSite__IEAAPEAVCDockingPanesRow__AEAPEAU__POSITION__H_Z(void* /*struct*/** p0, int p1) {
    return nullptr;
}

// Symbol: ?FindPaneByID@CDockSite@@QEAAPEAVCPane@@I@Z
extern "C" void* MS_ABI impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(unsigned int p0) {
    return nullptr;
}

// Symbol: ?FindRowByRect@CDockSite@@IEAAPEAVCDockingPanesRow@@VCRect@@@Z
extern "C" void* MS_ABI impl__FindRowByRect_CDockSite__IEAAPEAVCDockingPanesRow__VCRect___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?FindRowIndex@CDockSite@@QEAAHPEAVCDockingPanesRow@@@Z
extern "C" int MS_ABI impl__FindRowIndex_CDockSite__QEAAHPEAVCDockingPanesRow___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?IsDragMode@CDockSite@@UEBAHXZ
extern "C" int MS_ABI impl__IsDragMode_CDockSite__UEBAHXZ() {
    return 0;
}

// Symbol: ?IsLastRow@CDockSite@@QEBA_NPEAVCDockingPanesRow@@@Z
extern "C" int MS_ABI impl__IsLastRow_CDockSite__QEBA_NPEAVCDockingPanesRow___Z(void* /*class*/* p0) {
    return 0;  // false
}

// Symbol: ?IsRectWithinDockSite@CDockSite@@QEAAHVCRect@@AEAVCPoint@@@Z
extern "C" int MS_ABI impl__IsRectWithinDockSite_CDockSite__QEAAHVCRect__AEAVCPoint___Z(void* /*class*/ p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?MovePane@CDockSite@@UEAAHPEAVCPane@@IVCPoint@@@Z
extern "C" int MS_ABI impl__MovePane_CDockSite__UEAAHPEAVCPane__IVCPoint___Z(void* /*class*/* p0, unsigned int p1, void* /*class*/ p2) {
    return 0;
}

// Symbol: ?OnContextMenu@CDockSite@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CDockSite__IEAAXPEAVCWnd__VCPoint___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnDestroy@CDockSite@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CDockSite__IEAAXXZ() {}

// Symbol: ?OnEraseBkgnd@CDockSite@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CDockSite__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnNcDestroy@CDockSite@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CDockSite__IEAAXXZ() {}

// Symbol: ?OnPaint@CDockSite@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDockSite__IEAAXXZ() {}

// Symbol: ?OnResizeRow@CDockSite@@UEAAHPEAVCDockingPanesRow@@H@Z
extern "C" int MS_ABI impl__OnResizeRow_CDockSite__UEAAHPEAVCDockingPanesRow__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnShowRow@CDockSite@@UEAAXPEAU__POSITION@@H@Z
extern "C" void MS_ABI impl__OnShowRow_CDockSite__UEAAXPEAU__POSITION__H_Z(void* /*struct*/* p0, int p1) {}

// Symbol: ?OnSize@CDockSite@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CDockSite__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?PaneFromPoint@CDockSite@@UEAAPEAVCPane@@VCPoint@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CDockSite__UEAAPEAVCPane__VCPoint___Z(void* /*class*/ p0) {
    return nullptr;
}

// Symbol: ?RectSideFromPoint@CDockSite@@SAHAEBVCRect@@AEBVCPoint@@@Z
extern "C" int MS_ABI impl__RectSideFromPoint_CDockSite__SAHAEBVCRect__AEBVCPoint___Z(const void* /*class*/* p0, const void* /*class*/* p1) {
    return 0;
}

// Symbol: ?RemovePane@CDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@@Z
extern "C" void MS_ABI impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14) {}

// Symbol: ?RemoveRow@CDockSite@@QEAAXPEAVCDockingPanesRow@@@Z
extern "C" void MS_ABI impl__RemoveRow_CDockSite__QEAAXPEAVCDockingPanesRow___Z(void* /*class*/* p0) {}

// Symbol: ?ReplacePane@CDockSite@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__ReplacePane_CDockSite__QEAAHPEAVCPane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?RepositionPanes@CDockSite@@UEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__RepositionPanes_CDockSite__UEAAXAEAVCRect___Z(void* /*class*/* p0) {}

// Symbol: ?RowFromPane@CDockSite@@IEBAPEAVCDockingPanesRow@@PEAVCBasePane@@@Z
extern "C" void* MS_ABI impl__RowFromPane_CDockSite__IEBAPEAVCDockingPanesRow__PEAVCBasePane___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?RowFromPoint@CDockSite@@IEBAPEAVCDockingPanesRow@@VCPoint@@AEA_N@Z
extern "C" void* MS_ABI impl__RowFromPoint_CDockSite__IEBAPEAVCDockingPanesRow__VCPoint__AEA_N_Z(void* /*class*/ p0, bool* p1) {
    return nullptr;
}

// Symbol: ?ShowRow@CDockSite@@QEAAXPEAVCDockingPanesRow@@HH@Z
extern "C" void MS_ABI impl__ShowRow_CDockSite__QEAAXPEAVCDockingPanesRow__HH_Z(void* /*class*/* p0, int p1, int p2) {}

// Symbol: ?SwapRows@CDockSite@@QEAAXPEAVCDockingPanesRow@@0@Z
extern "C" void MS_ABI impl__SwapRows_CDockSite__QEAAXPEAVCDockingPanesRow__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}
