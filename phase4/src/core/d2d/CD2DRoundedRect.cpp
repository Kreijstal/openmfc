// CD2DRoundedRect — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CD2DRoundedRect@@QEAA@AEBVCD2DRectF@@AEBVCD2DSizeF@@@Z
extern "C" CD2DRoundedRect* MS_ABI impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(
    CD2DRoundedRect* pThis, const CD2DRectF* pRect, const CD2DSizeF* pRadius) {
    if (!pThis) return nullptr;
    pThis->rect = pRect ? *pRect : CD2DRectF();
    pThis->radius = pRadius ? *pRadius : CD2DSizeF();
    return pThis;
}
// Symbol: ??0CD2DRoundedRect@@QEAA@AEBUD2D1_ROUNDED_RECT@@@Z
extern "C" CD2DRoundedRect* MS_ABI impl___0CD2DRoundedRect__QEAA_AEBUD2D1_ROUNDED_RECT___Z(
    CD2DRoundedRect* pThis, const void* pRoundedRect) {
    const auto* roundedRect = static_cast<const D2D1_ROUNDED_RECT_L*>(pRoundedRect);
    if (!roundedRect) return impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(pThis, nullptr, nullptr);
    const CD2DRectF rect(roundedRect->rect.left, roundedRect->rect.top, roundedRect->rect.right, roundedRect->rect.bottom);
    const CD2DSizeF radius(roundedRect->radiusX, roundedRect->radiusY);
    return impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(pThis, &rect, &radius);
}
// Symbol: ??0CD2DRoundedRect@@QEAA@PEBUD2D1_ROUNDED_RECT@@@Z
extern "C" CD2DRoundedRect* MS_ABI impl___0CD2DRoundedRect__QEAA_PEBUD2D1_ROUNDED_RECT___Z(
    CD2DRoundedRect* pThis, const void* pRoundedRect) {
    return impl___0CD2DRoundedRect__QEAA_AEBUD2D1_ROUNDED_RECT___Z(pThis, pRoundedRect);
}
CD2DRoundedRect::CD2DRoundedRect() : rect(), radius() {}
CD2DRoundedRect::CD2DRoundedRect(const CD2DRectF& rectValue, const CD2DSizeF& radiusValue) {
    impl___0CD2DRoundedRect__QEAA_AEBVCD2DRectF__AEBVCD2DSizeF___Z(this, &rectValue, &radiusValue);
}
CD2DRoundedRect::CD2DRoundedRect(const D2D1_ROUNDED_RECT& roundedRect) {
    impl___0CD2DRoundedRect__QEAA_AEBUD2D1_ROUNDED_RECT___Z(this, &roundedRect);
}
CD2DRoundedRect::CD2DRoundedRect(const D2D1_ROUNDED_RECT* roundedRect) {
    impl___0CD2DRoundedRect__QEAA_PEBUD2D1_ROUNDED_RECT___Z(this, roundedRect);
}
