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
