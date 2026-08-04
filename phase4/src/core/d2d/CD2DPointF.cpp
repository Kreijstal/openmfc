// CD2DPointF — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CD2DPointF@@QEAA@MM@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_MM_Z(CD2DPointF* pThis, float xValue, float yValue) {
    return InitCD2DPointF(pThis, xValue, yValue);
}
// Symbol: ??0CD2DPointF@@QEAA@AEBVCPoint@@@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_AEBVCPoint___Z(CD2DPointF* pThis, const CPoint* pPoint) {
    if (!pPoint) return InitCD2DPointF(pThis, 0.0f, 0.0f);
    return InitCD2DPointF(pThis, (float)pPoint->x, (float)pPoint->y);
}
// Symbol: ??0CD2DPointF@@QEAA@AEBUD2D_POINT_2F@@@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_AEBUD2D_POINT_2F___Z(CD2DPointF* pThis, const void* pPoint) {
    const auto* point = static_cast<const D2D_POINT_2F_L*>(pPoint);
    return point ? InitCD2DPointF(pThis, point->x, point->y) : InitCD2DPointF(pThis, 0.0f, 0.0f);
}
// Symbol: ??0CD2DPointF@@QEAA@PEBUD2D_POINT_2F@@@Z
extern "C" CD2DPointF* MS_ABI impl___0CD2DPointF__QEAA_PEBUD2D_POINT_2F___Z(CD2DPointF* pThis, const void* pPoint) {
    return impl___0CD2DPointF__QEAA_AEBUD2D_POINT_2F___Z(pThis, pPoint);
}
CD2DPointF::CD2DPointF() : x(0.0f), y(0.0f) {}
CD2DPointF::CD2DPointF(float xValue, float yValue) { impl___0CD2DPointF__QEAA_MM_Z(this, xValue, yValue); }
CD2DPointF::CD2DPointF(const CPoint& point) { impl___0CD2DPointF__QEAA_AEBVCPoint___Z(this, &point); }
CD2DPointF::CD2DPointF(const D2D_POINT_2F& point) { impl___0CD2DPointF__QEAA_AEBUD2D_POINT_2F___Z(this, &point); }
CD2DPointF::CD2DPointF(const D2D_POINT_2F* point) { impl___0CD2DPointF__QEAA_PEBUD2D_POINT_2F___Z(this, point); }
