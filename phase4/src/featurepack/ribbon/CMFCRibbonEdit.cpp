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
// Symbol: ?CommonInit@CMFCRibbonEdit@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonEdit__IEAAXXZ() {}

// Symbol: ?CopyFrom@CMFCRibbonEdit@@UEAAXAEBVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonEdit__UEAAXAEBVCMFCRibbonBaseElement___Z(const void* /*class*/* p0) {}

// Symbol: ?CreateEdit@CMFCRibbonEdit@@UEAAPEAVCMFCRibbonRichEditCtrl@@PEAVCWnd@@K@Z
extern "C" void* MS_ABI impl__CreateEdit_CMFCRibbonEdit__UEAAPEAVCMFCRibbonRichEditCtrl__PEAVCWnd__K_Z(void* /*class*/* p0, unsigned long p1) {
    return nullptr;
}

// Symbol: ?CreateSpinButton@CMFCRibbonEdit@@IEAAHPEAVCMFCRibbonRichEditCtrl@@PEAVCWnd@@@Z
extern "C" int MS_ABI impl__CreateSpinButton_CMFCRibbonEdit__IEAAHPEAVCMFCRibbonRichEditCtrl__PEAVCWnd___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?DestroyCtrl@CMFCRibbonEdit@@UEAAXXZ
extern "C" void MS_ABI impl__DestroyCtrl_CMFCRibbonEdit__UEAAXXZ() {}

// Symbol: ?EnableSpinButtons@CMFCRibbonEdit@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableSpinButtons_CMFCRibbonEdit__QEAAXHH_Z(int p0, int p1) {}

// Symbol: ?OnAfterChangeRect@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonEdit__UEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnDrawLabelAndImage@CMFCRibbonEdit@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDrawLabelAndImage_CMFCRibbonEdit__UEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// Symbol: ?OnDrawOnList@CMFCRibbonEdit@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonEdit__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(void* /*class*/* p0, void* /*class*/ p1, void** p2, void* p3, int p4, void* /*class*/ p5, int p6, int p7) {}

// Symbol: ?OnHighlight@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnHighlight_CMFCRibbonEdit__UEAAXH_Z(int p0) {}

// Symbol: ?OnKey@CMFCRibbonEdit@@UEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonEdit__UEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?OnLButtonDown@CMFCRibbonEdit@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonEdit__UEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnLButtonUp@CMFCRibbonEdit@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonEdit__UEAAXVCPoint___Z(void* /*class*/ p0) {}

// Symbol: ?OnRTLChanged@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnRTLChanged_CMFCRibbonEdit__UEAAXH_Z(int p0) {}

// Symbol: ?OnSetFocus@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCRibbonEdit__UEAAXH_Z(int p0) {}

// Symbol: ?OnShow@CMFCRibbonEdit@@UEAAXH@Z
extern "C" void MS_ABI impl__OnShow_CMFCRibbonEdit__UEAAXH_Z(int p0) {}

// Symbol: ?PreLMouseDown@CMFCRibbonEdit@@UEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__PreLMouseDown_CMFCRibbonEdit__UEAAHVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Symbol: ?Redraw@CMFCRibbonEdit@@UEAAXXZ
extern "C" void MS_ABI impl__Redraw_CMFCRibbonEdit__UEAAXXZ() {}

// Symbol: ?RepositionRibbonEditCtrl@CMFCRibbonEdit@@IEAAXXZ
extern "C" void MS_ABI impl__RepositionRibbonEditCtrl_CMFCRibbonEdit__IEAAXXZ() {}

// Symbol: ?SetACCData@CMFCRibbonEdit@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonEdit__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?SetEditText@CMFCRibbonEdit@@QEAAXV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetEditText_CMFCRibbonEdit__QEAAXV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/ p0, void** p1, void* p2) {}

// Symbol: ?SetTextAlign@CMFCRibbonEdit@@QEAAXH@Z
extern "C" void MS_ABI impl__SetTextAlign_CMFCRibbonEdit__QEAAXH_Z(int p0) {}

// Symbol: ?SetWidth@CMFCRibbonEdit@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetWidth_CMFCRibbonEdit__QEAAXHH_Z(int p0, int p1) {}
