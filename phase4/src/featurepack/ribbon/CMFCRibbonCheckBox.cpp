// CMFCRibbonCheckBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// Retail entry RVA 0xf0bb0 (mfc140u) writes the CSize through its hidden result
// buffer (RDX) and calculates it from checkbox fields that are not modeled here.
// Keep the existing fallback dimensions, but honor the result-buffer ABI.
// Symbol: ?GetIntermediateSize@CMFCRibbonCheckBox@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetIntermediateSize_CMFCRibbonCheckBox__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pResult, void* pDC) {
    (void)pThis;
    (void)pDC;
    if (pResult == nullptr) {
        return nullptr;
    }
    *static_cast<CSize*>(pResult) = CSize(16, 16);
    return pResult;
}
// Symbol: ?OnDraw@CMFCRibbonCheckBox@@UEAAXPEAVCDC@@@Z
// Stub: retail entry RVA 0xf0cc0 (mfc140u) uses a RECT at +0xc8; reads fields
// at +0xcc, +0xd4, +0x138, +0x148, +0x14c and +0x190; passes the address +0xa0
// to a virtual call; and makes multiple virtual calls through this object's
// vtable. The clean-room checkbox does not model the retail CMFCRibbonButton
// base/layout or the required virtual behavior, so drawing cannot be safely
// transcribed here.
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCheckBox__UEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}
// Symbol: ?OnDrawOnList@CMFCRibbonCheckBox@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
// Approximation: retail delegates checkbox-list rendering to the visual manager.
// This keeps the local GDI fallback, but uses the actual by-value argument layout.
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonCheckBox__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    void* pThis, CDC* pDC, const CString* text, int textOffset, const CRect* rect,
    int isSelected, int isHighlighted) {
    (void)pThis;
    (void)text;
    (void)textOffset;
    if (pDC == nullptr || pDC->GetSafeHdc() == nullptr || rect == nullptr) {
        return;
    }
    const RECT* itemRect = reinterpret_cast<const RECT*>(rect);
    COLORREF bg = isHighlighted ? COLOR_BTNFACE : COLOR_WINDOW;
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(bg));
    if (brush != nullptr) {
        ::FillRect(pDC->GetSafeHdc(), itemRect, brush);
        ::DeleteObject(brush);
    }
    if (isSelected) {
        ::FrameRect(pDC->GetSafeHdc(), itemRect, (HBRUSH)(intptr_t)(COLOR_3DLIGHT + 1));
    }
}
CMFCRibbonCheckBox::CMFCRibbonCheckBox() { memset(_pad, 0, sizeof(_pad)); }
CMFCRibbonCheckBox::~CMFCRibbonCheckBox() {}
