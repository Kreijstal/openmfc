// CMFCRibbonCheckBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?GetIntermediateSize@CMFCRibbonCheckBox@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetIntermediateSize_CMFCRibbonCheckBox__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
    static CSize size(16, 16);
    return static_cast<void*>(&size);
}
// Symbol: ?OnDraw@CMFCRibbonCheckBox@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCheckBox__UEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}
// Symbol: ?OnDrawOnList@CMFCRibbonCheckBox@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonCheckBox__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    void* pThis, void* pDC, const void* text, const void* rect, int index, int isEnabled) {
    (void)pThis;
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    const RECT* itemRect = static_cast<const RECT*>(rect);
    if (itemRect == nullptr) {
        return;
    }
    COLORREF bg = (isEnabled == 0) ? COLOR_BTNFACE : COLOR_WINDOW;
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(bg));
    if (brush != nullptr) {
        ::FillRect(pDeviceContext->GetSafeHdc(), itemRect, brush);
        ::DeleteObject(brush);
    }
    if (index >= 0 && (index & 1)) {
        ::FrameRect(pDeviceContext->GetSafeHdc(), itemRect, (HBRUSH)(intptr_t)(COLOR_3DLIGHT + 1));
    }
    (void)text;
}
CMFCRibbonCheckBox::CMFCRibbonCheckBox() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonCheckBox::~CMFCRibbonCheckBox() {}
