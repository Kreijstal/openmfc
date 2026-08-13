// CMFCRibbonSlider — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?GetRegularSize@CMFCRibbonSlider@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetRegularSize_CMFCRibbonSlider__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonSlider* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 96 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?OnDraw@CMFCRibbonSlider@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonSlider__UEAAXPEAVCDC___Z(CMFCRibbonSlider* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}
// Symbol: ?SetRange@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRange_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nMin, int nMax) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_sliderStates[pThis->GetID()];
    NormalizeRange(nMin, nMax, state.nMin, state.nMax);
    state.nPos = std::clamp(state.nPos, state.nMin, state.nMax);
}
// Symbol: ?SetPos@CMFCRibbonSlider@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPos_CMFCRibbonSlider__QEAAXHH_Z(CMFCRibbonSlider* pThis, int nPos, int /*bRedraw*/) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto& state = g_sliderStates[pThis->GetID()];
    state.nPos = std::clamp(nPos, state.nMin, state.nMax);
}
// Symbol: ?SetZoomButtons@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomButtons_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int bSet) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_sliderStates[pThis->GetID()].bShowZoomButtons = (bSet != FALSE);
}
// Symbol: ?SetZoomIncrement@CMFCRibbonSlider@@QEAAXH@Z
extern "C" void MS_ABI impl__SetZoomIncrement_CMFCRibbonSlider__QEAAXH_Z(CMFCRibbonSlider* pThis, int nDelta) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_sliderStates[pThis->GetID()].nZoomIncrement = std::max(1, nDelta);
}
CMFCRibbonSlider::CMFCRibbonSlider() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonSlider::~CMFCRibbonSlider() {}
// ?CommonInit@CMFCRibbonSlider@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonSlider__IEAAXXZ() {}

// ?CopyFrom@CMFCRibbonSlider@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonSlider__MEAAXAEBVCMFCRibbonBaseElement___Z(const void* /*class*/* p0) {}

// ?GetHitTest@CMFCRibbonSlider@@IEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__GetHitTest_CMFCRibbonSlider__IEBAHVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// ?GetPosFromPoint@CMFCRibbonSlider@@IEAAHH@Z
extern "C" int MS_ABI impl__GetPosFromPoint_CMFCRibbonSlider__IEAAHH_Z(int p0) {
    return 0;
}

// ?IsAutoRepeatMode@CMFCRibbonSlider@@MEBAHAEAH@Z
extern "C" int MS_ABI impl__IsAutoRepeatMode_CMFCRibbonSlider__MEBAHAEAH_Z(int* p0) {
    return 0;
}

// ?NotifyCommand@CMFCRibbonSlider@@IEAAXXZ
extern "C" void MS_ABI impl__NotifyCommand_CMFCRibbonSlider__IEAAXXZ() {}

// ?OnAfterChangeRect@CMFCRibbonSlider@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonSlider__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// ?OnAutoRepeat@CMFCRibbonSlider@@MEAAHXZ
extern "C" int MS_ABI impl__OnAutoRepeat_CMFCRibbonSlider__MEAAHXZ() {
    return 0;
}

// ?OnDrawOnList@CMFCRibbonSlider@@MEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonSlider__MEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(void* /*class*/* p0, void* /*class*/ p1, void** p2, void* p3, int p4, void* /*class*/ p5, int p6, int p7) {}

// ?OnLButtonDown@CMFCRibbonSlider@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonSlider__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// ?OnLButtonUp@CMFCRibbonSlider@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonSlider__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// ?OnMouseMove@CMFCRibbonSlider@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonSlider__MEAAXVCPoint___Z(void* /*class*/ p0) {}

// ?SetACCData@CMFCRibbonSlider@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonSlider__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// ?SetThumbRect@CMFCRibbonSlider@@IEAAXXZ
extern "C" void MS_ABI impl__SetThumbRect_CMFCRibbonSlider__IEAAXXZ() {}

