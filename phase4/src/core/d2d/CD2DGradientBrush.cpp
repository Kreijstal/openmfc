// CD2DGradientBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CD2DGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IW4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IW4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, int stopCount,
    int gammaMode, int extendMode, void* pProps, int unusedFlags) {
    if (pThis == nullptr) {
        return nullptr;
    }
    (void)pRenderTarget;
    (void)pStops;
    (void)stopCount;
    (void)gammaMode;
    (void)extendMode;
    (void)pProps;
    (void)unusedFlags;
    auto& state = EnsureCD2DGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    state.m_opacity = 1.0f;
    return pThis;
}
// Symbol: ??1CD2DGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DGradientBrush__MEAAXXZ(pThis);
    }
}
// Symbol: ?Destroy@CD2DGradientBrush@@MEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DGradientBrush__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dGradientBrushStateMutex);
    g_cd2dGradientBrushStates.erase(pThis);
}
