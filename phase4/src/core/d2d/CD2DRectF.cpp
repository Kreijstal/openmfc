// CD2DRectF — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CD2DRectF@@QEAA@MMMM@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_MMMM_Z(
    CD2DRectF* pThis, float leftValue, float topValue, float rightValue, float bottomValue) {
    return InitCD2DRectF(pThis, leftValue, topValue, rightValue, bottomValue);
}
// Symbol: ??0CD2DRectF@@QEAA@AEBVCRect@@@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_AEBVCRect___Z(CD2DRectF* pThis, const CRect* pRect) {
    if (!pRect) return InitCD2DRectF(pThis, 0.0f, 0.0f, 0.0f, 0.0f);
    return InitCD2DRectF(pThis, (float)pRect->left, (float)pRect->top, (float)pRect->right, (float)pRect->bottom);
}
// Symbol: ??0CD2DRectF@@QEAA@AEBUD2D_RECT_F@@@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_AEBUD2D_RECT_F___Z(CD2DRectF* pThis, const void* pRect) {
    const auto* rect = static_cast<const D2D_RECT_F_L*>(pRect);
    if (!rect) return InitCD2DRectF(pThis, 0.0f, 0.0f, 0.0f, 0.0f);
    return InitCD2DRectF(pThis, rect->left, rect->top, rect->right, rect->bottom);
}
// Symbol: ??0CD2DRectF@@QEAA@PEBUD2D_RECT_F@@@Z
extern "C" CD2DRectF* MS_ABI impl___0CD2DRectF__QEAA_PEBUD2D_RECT_F___Z(CD2DRectF* pThis, const void* pRect) {
    return impl___0CD2DRectF__QEAA_AEBUD2D_RECT_F___Z(pThis, pRect);
}
CD2DRectF::CD2DRectF() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
CD2DRectF::CD2DRectF(float leftValue, float topValue, float rightValue, float bottomValue) {
    impl___0CD2DRectF__QEAA_MMMM_Z(this, leftValue, topValue, rightValue, bottomValue);
}
CD2DRectF::CD2DRectF(const CRect& rect) { impl___0CD2DRectF__QEAA_AEBVCRect___Z(this, &rect); }
CD2DRectF::CD2DRectF(const D2D_RECT_F& rect) { impl___0CD2DRectF__QEAA_AEBUD2D_RECT_F___Z(this, &rect); }
CD2DRectF::CD2DRectF(const D2D_RECT_F* rect) { impl___0CD2DRectF__QEAA_PEBUD2D_RECT_F___Z(this, rect); }
