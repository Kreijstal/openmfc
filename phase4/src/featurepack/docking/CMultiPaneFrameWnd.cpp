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
