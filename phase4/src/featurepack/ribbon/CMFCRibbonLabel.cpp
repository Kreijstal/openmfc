// CMFCRibbonLabel — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

extern "C" int MS_ABI impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonBaseElement* pThis, CWnd* pParentWnd, CAccessibilityData& data);

CMFCRibbonLabel::CMFCRibbonLabel() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonLabel::~CMFCRibbonLabel() {}
// Retail mfc140u RVA 0x10eb30 reads/writes label/button members at +0x180,
// +0x184, +0x190, and +0x194, then dispatches a virtual at vtable+0x310.
// CMFCRibbonLabel is currently modeled as a direct CMFCRibbonBaseElement with
// only 32 bytes of derived padding, not the retail CMFCRibbonButton layout, so
// those offsets are not safe to access here.
// Symbol: ?GetIntermediateSize@CMFCRibbonLabel@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void MS_ABI impl__GetIntermediateSize_CMFCRibbonLabel__MEAA_AVCSize__PEAVCDC___Z(
    CSize* pRet, CMFCRibbonLabel* pThis, CDC* pDC) {
    (void)pRet;
    (void)pThis;
    (void)pDC;
}

// This export is present in the mfc140u ordinal map, but --u has no resolved
// RVA for it. Its ANSI-twin body at RVA 0x10ac0 (mfc140) reads +0xb8, tests the
// pointee field at -0x10, and conditionally dispatches through vtable+0x368.
// Function bodies are byte-identical across the two images; +0xb8 is outside
// the current label layout, so that behavior cannot safely be accessed here.
// Symbol: ?OnAfterChangeRect@CMFCRibbonLabel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonLabel__MEAAXPEAVCDC___Z(
    CMFCRibbonLabel* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// Retail mfc140u RVA 0x10e990 reads unmodeled base/button state (including
// +0x164, +0xe0, and +0x180/+0x184) and performs CDC text measurement using a
// temporary string buffer. The clean-room label/button layout is unavailable.
// Symbol: ?OnCalcTextSize@CMFCRibbonLabel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnCalcTextSize_CMFCRibbonLabel__MEAAXPEAVCDC___Z(
    CMFCRibbonLabel* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// Retail mfc140u RVA 0x10e770 reads the label rectangle at +0xc8 and other
// unmodeled button fields, then performs a multi-path CDC drawing operation.
// The class declaration does not provide the retail layout needed to do this
// without out-of-bounds accesses.
// Symbol: ?OnDraw@CMFCRibbonLabel@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonLabel__MEAAXPEAVCDC___Z(
    CMFCRibbonLabel* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// The mfc140u ordinal map contains this export, although --u does not resolve
// its RVA. The ANSI-twin body at RVA 0x10f4f0 (mfc140) calls the base method,
// writes role 0x29 and state 0, empties the default-action string, and returns
// TRUE. Bodies are byte-identical across the two images. The base thunk here is
// still a stub, so only the label-specific post-base updates are reproduced.
// Symbol: ?SetACCData@CMFCRibbonLabel@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonLabel__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonLabel* pThis, CWnd* pParentWnd, CAccessibilityData& data) {
    impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
        pThis, pParentWnd, data);
    data.m_nAccRole = 0x29;
    data.m_bAccState = 0;
    data.m_strAccDefAction.Empty();
    return TRUE;
}

