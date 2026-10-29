// CMFCRibbonCaptionButton — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

extern "C" void MS_ABI impl__OnDrawRibbonCaptionButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonCaptionButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonCaptionButton* pButton);

CMFCRibbonCaptionButton::CMFCRibbonCaptionButton() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonCaptionButton::~CMFCRibbonCaptionButton() {}
// Retail RVA 0xd7be0 (mfc140u) allocates 0x278 bytes and calls the (UINT, HWND)
// constructor at RVA 0xd7c30. OpenMFC declares a different base class and only a
// default constructor, so faithful construction needs the header layout corrected.
// Symbol: ?CreateObject@CMFCRibbonCaptionButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCRibbonCaptionButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0xd7db0 (mfc140u) calls USER32!GetSystemMetrics with 0x37 then
// 0x36, stores the latter in cx and the former in cy in the CSize return buffer.
// mfc140u IAT slot VA 0x1802c6bd8 resolves to USER32!GetSystemMetrics.
// Symbol: ?GetRegularSize@CMFCRibbonCaptionButton@@MEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetRegularSize_CMFCRibbonCaptionButton__MEAA_AVCSize__PEAVCDC___Z(
    CMFCRibbonCaptionButton* /*pThis*/, CSize* pRet, CDC* /*pDC*/) {
    const int cy = ::GetSystemMetrics(0x37);
    const int cx = ::GetSystemMetrics(0x36);
    pRet->cx = cx;
    pRet->cy = cy;
    return pRet;
}

// Retail RVA 0xd7ce0 (mfc140u) obtains the visual manager and dispatches its
// vtable slot +0x4d0 with (pDC, this). The project's C++ GetInstance method has no
// impl__ thunk, so use the published pointer; unlike retail's internal accessor
// (RVA 0x9774, mfc140u), this does not create a manager when that pointer is null.
// Symbol: ?OnDraw@CMFCRibbonCaptionButton@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCaptionButton__MEAAXPEAVCDC___Z(
    CMFCRibbonCaptionButton* pThis, CDC* pDC) {
    CMFCVisualManager* manager = static_cast<CMFCVisualManager*>(
        impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (!manager) return;
    impl__OnDrawRibbonCaptionButton_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCRibbonCaptionButton___Z(
        manager, pDC, pThis);
}

// The name lookup misses this export, but ordinal 10395 maps it to RVA 0xd7d20
// (mfc140u). Retail reads fields including this+0x270 and this+0x118, and writes
// this+0x13c; these belong to the retail button layout, not the OpenMFC
// CMFCRibbonBaseElement-derived class with only a 32-byte tail. Keep this as a
// stub rather than applying accesses for the incompatible retail layout.
// Symbol: ?OnLButtonUp@CMFCRibbonCaptionButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonCaptionButton__MEAAXVCPoint___Z(
    CMFCRibbonCaptionButton* pThis, long long point) {
    (void)pThis;
    (void)point;
}

