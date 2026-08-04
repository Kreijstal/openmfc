// CD2DRadialGradientBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Attach@CD2DRadialGradientBrush@@QEAAXPEAUID2D1RadialGradientBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DRadialGradientBrush__QEAAXPEAUID2D1RadialGradientBrush___Z(
    void* pThis, void* pBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_resource = pBrush;
}
// Symbol: ?Create@CD2DRadialGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DRadialGradientBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}
// Symbol: ??0CD2DRadialGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DRadialGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, int stopCount, void* properties,
    int gammaMode, int extendMode, void* pBrushProps, int unusedFlags) {
    (void)pStops;
    (void)stopCount;
    (void)properties;
    (void)gammaMode;
    (void)extendMode;
    (void)pBrushProps;
    (void)unusedFlags;
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return pThis;
}
// Symbol: ??1CD2DRadialGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DRadialGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
        g_cd2dRadialGradientBrushStates.erase(pThis);
    }
}
