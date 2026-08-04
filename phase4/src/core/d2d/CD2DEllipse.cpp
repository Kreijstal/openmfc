// CD2DEllipse — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CD2DEllipse@@QEAA@AEBVCD2DPointF@@AEBVCD2DSizeF@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(
    CD2DEllipse* pThis, const CD2DPointF* pPoint, const CD2DSizeF* pRadius) {
    if (!pThis) return nullptr;
    pThis->point = pPoint ? *pPoint : CD2DPointF();
    pThis->radius = pRadius ? *pRadius : CD2DSizeF();
    return pThis;
}
// Symbol: ??0CD2DEllipse@@QEAA@AEBVCD2DRectF@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_AEBVCD2DRectF___Z(CD2DEllipse* pThis, const CD2DRectF* pRect) {
    if (!pThis) return nullptr;
    const CD2DRectF rect = pRect ? *pRect : CD2DRectF();
    const float radiusX = std::fabs((rect.right - rect.left) * 0.5f);
    const float radiusY = std::fabs((rect.bottom - rect.top) * 0.5f);
    const CD2DPointF point((rect.left + rect.right) * 0.5f, (rect.top + rect.bottom) * 0.5f);
    const CD2DSizeF radius(radiusX, radiusY);
    return impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(pThis, &point, &radius);
}
// Symbol: ??0CD2DEllipse@@QEAA@AEBUD2D1_ELLIPSE@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_AEBUD2D1_ELLIPSE___Z(CD2DEllipse* pThis, const void* pEllipse) {
    const auto* ellipse = static_cast<const D2D1_ELLIPSE_L*>(pEllipse);
    if (!ellipse) return impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(pThis, nullptr, nullptr);
    const CD2DPointF point(ellipse->point.x, ellipse->point.y);
    const CD2DSizeF radius(ellipse->radiusX, ellipse->radiusY);
    return impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(pThis, &point, &radius);
}
// Symbol: ??0CD2DEllipse@@QEAA@PEBUD2D1_ELLIPSE@@@Z
extern "C" CD2DEllipse* MS_ABI impl___0CD2DEllipse__QEAA_PEBUD2D1_ELLIPSE___Z(CD2DEllipse* pThis, const void* pEllipse) {
    return impl___0CD2DEllipse__QEAA_AEBUD2D1_ELLIPSE___Z(pThis, pEllipse);
}
CD2DEllipse::CD2DEllipse() : point(), radius() {}
CD2DEllipse::CD2DEllipse(const CD2DPointF& pointValue, const CD2DSizeF& radiusValue) {
    impl___0CD2DEllipse__QEAA_AEBVCD2DPointF__AEBVCD2DSizeF___Z(this, &pointValue, &radiusValue);
}
CD2DEllipse::CD2DEllipse(const CD2DRectF& rect) { impl___0CD2DEllipse__QEAA_AEBVCD2DRectF___Z(this, &rect); }
CD2DEllipse::CD2DEllipse(const D2D1_ELLIPSE& ellipse) { impl___0CD2DEllipse__QEAA_AEBUD2D1_ELLIPSE___Z(this, &ellipse); }
CD2DEllipse::CD2DEllipse(const D2D1_ELLIPSE* ellipse) { impl___0CD2DEllipse__QEAA_PEBUD2D1_ELLIPSE___Z(this, ellipse); }
