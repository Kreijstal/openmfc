// CMultiPaneFrameWnd — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

// Symbol: ??0CMultiPaneFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMultiPaneFrameWnd__QEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_multiFramePanes[pThis] = {};
    return pThis;
}
// Symbol: ??1CMultiPaneFrameWnd@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMultiPaneFrameWnd__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_multiFramePanes.erase(pThis);
    return pThis;
}
// Symbol: ?AddPane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddPane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_multiFramePanes[pThis], pPane);
}
// Symbol: ?RemovePane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@HH@Z
extern "C" void MS_ABI impl__RemovePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__HH_Z(void* pThis, void* pPane, int, int) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    RemovePane(g_multiFramePanes[pThis], pPane);
}
// Symbol: ?GetPane@CMultiPaneFrameWnd@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetPane_CMultiPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_multiFramePanes[pThis]);
}
// Symbol: ?GetCaptionText@CMultiPaneFrameWnd@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetCaptionText_CMultiPaneFrameWnd__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pRet->Format(L"Pane group (%d)", static_cast<int>(g_multiFramePanes[pThis].panes.size()));
}
// Symbol: ?CalcExpectedDockedRect@CMultiPaneFrameWnd@@UEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CMultiPaneFrameWnd__UEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void*, void*, CPoint, CRect* pRect, int* pnAlign, void** ppBar) {
    if (pRect) pRect->SetRectEmpty();
    if (pnAlign) *pnAlign = 0;
    if (ppBar) *ppBar = nullptr;
}
// Symbol: ?AddRecentPane@CMultiPaneFrameWnd@@UEAAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__AddRecentPane_CMultiPaneFrameWnd__UEAAHPEAVCDockablePane___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?AdjustPaneFrames@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustPaneFrames_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?CanBeDockedToPane@CMultiPaneFrameWnd@@UEBAHPEBVCDockablePane@@@Z
extern "C" int MS_ABI impl__CanBeDockedToPane_CMultiPaneFrameWnd__UEBAHPEBVCDockablePane___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?CheckGripperVisibility@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__CheckGripperVisibility_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?CloseMiniFrame@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__CloseMiniFrame_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?ConvertToTabbedDocument@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__ConvertToTabbedDocument_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?CreateObject@CMultiPaneFrameWnd@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMultiPaneFrameWnd__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?DockFrame@CMultiPaneFrameWnd@@UEAAHPEAVCPaneFrameWnd@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__DockFrame_CMultiPaneFrameWnd__UEAAHPEAVCPaneFrameWnd__W4AFX_DOCK_METHOD___Z(void* /*class*/* p0, int /*enum*/ p1, short* p2, int p3, char p4, void* p5, void* p6, unsigned long p7, int p8, float p9, unsigned char p10, void* p11, int p12, void* p13, char p14) {
    return 0;
}

// Symbol: ?DockPane@CMultiPaneFrameWnd@@UEAAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__DockPane_CMultiPaneFrameWnd__UEAAHPEAVCDockablePane___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?DockPaneStandard@CMultiPaneFrameWnd@@MEAAPEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__DockPaneStandard_CMultiPaneFrameWnd__MEAAPEAVCDockablePane__AEAH_Z(int* p0) {
    return nullptr;
}

// Symbol: ?DockRecentPaneToMainFrame@CMultiPaneFrameWnd@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__DockRecentPaneToMainFrame_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane___Z(void* /*class*/* p0) {}

// Symbol: ?InsertPane@CMultiPaneFrameWnd@@UEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CMultiPaneFrameWnd__UEAAHPEAVCBasePane__0H_Z(void* /*class*/* p0, void* /*class*/* p1, int p2) {
    return 0;
}

// Symbol: ?LoadState@CMultiPaneFrameWnd@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CMultiPaneFrameWnd__UEAAHPEB_WI_Z(const wchar_t* p0, unsigned int p1) {
    return 0;
}

// Symbol: ?OnCheckEmptyState@CMultiPaneFrameWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnCheckEmptyState_CMultiPaneFrameWnd__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnCreate@CMultiPaneFrameWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMultiPaneFrameWnd__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnDockToRecentPos@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnDockToRecentPos_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?OnGetMinMaxInfo@CMultiPaneFrameWnd@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMultiPaneFrameWnd__IEAAXPEAUtagMINMAXINFO___Z(void* /*struct*/* p0) {}

// Symbol: ?OnIdleUpdateCmdUI@CMultiPaneFrameWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMultiPaneFrameWnd__IEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?OnKillRollUpTimer@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnKillRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?OnLButtonDblClk@CMultiPaneFrameWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMultiPaneFrameWnd__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Symbol: ?OnPaneRecalcLayout@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnPaneRecalcLayout_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?OnSetFocus@CMultiPaneFrameWnd@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMultiPaneFrameWnd__IEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnSetRollUpTimer@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__OnSetRollUpTimer_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?OnShowPane@CMultiPaneFrameWnd@@UEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__OnShowPane_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?OnSize@CMultiPaneFrameWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMultiPaneFrameWnd__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?OnSizing@CMultiPaneFrameWnd@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CMultiPaneFrameWnd__IEAAXIPEAUtagRECT___Z(unsigned int p0, void* /*struct*/* p1) {}

// Symbol: ?PaneFromPoint@CMultiPaneFrameWnd@@UEAAPEAVCBasePane@@VCPoint@@HH@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CMultiPaneFrameWnd__UEAAPEAVCBasePane__VCPoint__HH_Z(void* /*class*/ p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?RemoveNonValidPanes@CMultiPaneFrameWnd@@UEAAXXZ
extern "C" void MS_ABI impl__RemoveNonValidPanes_CMultiPaneFrameWnd__UEAAXXZ() {}

// Symbol: ?ReplacePane@CMultiPaneFrameWnd@@UEAAXPEAVCBasePane@@0@Z
extern "C" void MS_ABI impl__ReplacePane_CMultiPaneFrameWnd__UEAAXPEAVCBasePane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Symbol: ?SaveRecentFloatingState@CMultiPaneFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__SaveRecentFloatingState_CMultiPaneFrameWnd__MEAAXXZ() {}

// Symbol: ?SaveState@CMultiPaneFrameWnd@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CMultiPaneFrameWnd__UEAAHPEB_WI_Z(const wchar_t* p0, unsigned int p1) {
    return 0;
}

// Symbol: ?Serialize@CMultiPaneFrameWnd@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMultiPaneFrameWnd__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetDockState@CMultiPaneFrameWnd@@UEAAXPEAVCDockingManager@@@Z
extern "C" void MS_ABI impl__SetDockState_CMultiPaneFrameWnd__UEAAXPEAVCDockingManager___Z(void* /*class*/* p0) {}

// Symbol: ?SetPreDockState@CMultiPaneFrameWnd@@UEAAHW4AFX_PREDOCK_STATE@@PEAVCBasePane@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__SetPreDockState_CMultiPaneFrameWnd__UEAAHW4AFX_PREDOCK_STATE__PEAVCBasePane__W4AFX_DOCK_METHOD___Z(int /*enum*/ p0, short* p1, int p2, void** p3, unsigned char p4, char p5, void* p6, void* p7, unsigned long p8, int p9, void* p10, void* p11, void** p12, unsigned char p13, void* /*class*/* p14, int /*enum*/ p15, short* p16, int p17, char p18, void* p19, void* p20, unsigned long p21, int p22, float p23, unsigned char p24, void* p25, int p26, void* p27, char p28) {
    return 0;
}

// Symbol: ?StoreRecentDockSiteInfo@CMultiPaneFrameWnd@@UEAAXPEAVCPane@@@Z
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CMultiPaneFrameWnd__UEAAXPEAVCPane___Z(void* /*class*/* p0) {}

// Symbol: ?StoreRecentTabRelatedInfo@CMultiPaneFrameWnd@@UEAAXPEAVCDockablePane@@0@Z
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CMultiPaneFrameWnd__UEAAXPEAVCDockablePane__0_Z(void* /*class*/* p0, void* /*class*/* p1) {}
