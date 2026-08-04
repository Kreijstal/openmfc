// CD2DSizeF — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CD2DSizeF@@QEAA@MM@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_MM_Z(CD2DSizeF* pThis, float widthValue, float heightValue) {
    return InitCD2DSizeF(pThis, widthValue, heightValue);
}
// Symbol: ??0CD2DSizeF@@QEAA@AEBVCSize@@@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_AEBVCSize___Z(CD2DSizeF* pThis, const CSize* pSize) {
    if (!pSize) return InitCD2DSizeF(pThis, 0.0f, 0.0f);
    return InitCD2DSizeF(pThis, (float)pSize->cx, (float)pSize->cy);
}
// Symbol: ??0CD2DSizeF@@QEAA@AEBUD2D_SIZE_F@@@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_AEBUD2D_SIZE_F___Z(CD2DSizeF* pThis, const void* pSize) {
    const auto* size = static_cast<const D2D_SIZE_F_L*>(pSize);
    return size ? InitCD2DSizeF(pThis, size->width, size->height) : InitCD2DSizeF(pThis, 0.0f, 0.0f);
}
// Symbol: ??0CD2DSizeF@@QEAA@PEBUD2D_SIZE_F@@@Z
extern "C" CD2DSizeF* MS_ABI impl___0CD2DSizeF__QEAA_PEBUD2D_SIZE_F___Z(CD2DSizeF* pThis, const void* pSize) {
    return impl___0CD2DSizeF__QEAA_AEBUD2D_SIZE_F___Z(pThis, pSize);
}
CD2DSizeF::CD2DSizeF() : width(0.0f), height(0.0f) {}
CD2DSizeF::CD2DSizeF(float widthValue, float heightValue) { impl___0CD2DSizeF__QEAA_MM_Z(this, widthValue, heightValue); }
CD2DSizeF::CD2DSizeF(const CSize& size) { impl___0CD2DSizeF__QEAA_AEBVCSize___Z(this, &size); }
CD2DSizeF::CD2DSizeF(const D2D_SIZE_F& size) { impl___0CD2DSizeF__QEAA_AEBUD2D_SIZE_F___Z(this, &size); }
CD2DSizeF::CD2DSizeF(const D2D_SIZE_F* size) { impl___0CD2DSizeF__QEAA_PEBUD2D_SIZE_F___Z(this, size); }
