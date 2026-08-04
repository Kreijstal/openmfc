// CMFCRibbonEdit — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?GetCompactSize@CMFCRibbonEdit@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetCompactSize_CMFCRibbonEdit__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonEdit* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 80 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?GetIntermediateSize@CMFCRibbonEdit@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonEdit__UEAA_AVCSize__PEAVCDC___Z(void* pRet, const CMFCRibbonEdit* pThis, CDC* /*pDC*/) {
    BuildCSizeResult(pRet, 120 + RibbonTextPixels(pThis), 22);
}
// Symbol: ?OnDraw@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonEdit__UEAAXPEAVCDC___Z(CMFCRibbonEdit* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) {
        return;
    }
    (void)CMFCVisualManager::GetInstance();
}
// Symbol: ?OnEnable@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCRibbonEdit__UEAAXH_Z(CMFCRibbonEdit* pThis, int bEnable) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonElementsEnabled[pThis] = (bEnable != FALSE);
}
CMFCRibbonEdit::CMFCRibbonEdit() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonEdit::~CMFCRibbonEdit() {}
