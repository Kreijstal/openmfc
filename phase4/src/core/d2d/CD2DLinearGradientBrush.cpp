// CD2DLinearGradientBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Attach@CD2DLinearGradientBrush@@QEAAXPEAUID2D1LinearGradientBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DLinearGradientBrush__QEAAXPEAUID2D1LinearGradientBrush___Z(
    void* pThis, void* pBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_resource = pBrush;
}
// Symbol: ?Create@CD2DLinearGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DLinearGradientBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}
// Symbol: ?Destroy@CD2DLinearGradientBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DLinearGradientBrush__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    g_cd2dLinearGradientBrushStates.erase(pThis);
}
// Symbol: ?Detach@CD2DLinearGradientBrush@@QEAAPEAUID2D1LinearGradientBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DLinearGradientBrush__QEAAPEAUID2D1LinearGradientBrush__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    auto it = g_cd2dLinearGradientBrushStates.find(pThis);
    if (it == g_cd2dLinearGradientBrushStates.end()) {
        return nullptr;
    }

    void* brush = it->second.m_resource;
    it->second.m_resource = nullptr;
    return brush;
}
// Symbol: ?GetEndPoint@CD2DLinearGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" ::CD2DPointF MS_ABI impl__GetEndPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DPointF(0.0f, 0.0f);
    }

    CD2DLinearGradientBrushState state{};
    if (!TryLoadCD2DLinearGradientBrushState(const_cast<void*>(pThis), state)) {
        return ::CD2DPointF(0.0f, 0.0f);
    }
    return ::CD2DPointF(state.m_endPoint.x, state.m_endPoint.y);
}
// Symbol: ?GetStartPoint@CD2DLinearGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" ::CD2DPointF MS_ABI impl__GetStartPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DPointF(0.0f, 0.0f);
    }

    CD2DLinearGradientBrushState state{};
    if (!TryLoadCD2DLinearGradientBrushState(const_cast<void*>(pThis), state)) {
        return ::CD2DPointF(0.0f, 0.0f);
    }
    return ::CD2DPointF(state.m_startPoint.x, state.m_startPoint.y);
}
// Symbol: ?SetEndPoint@CD2DLinearGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetEndPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(
    void* pThis, ::CD2DPointF endPoint) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_endPoint = {endPoint.x, endPoint.y};
}
// Symbol: ?SetStartPoint@CD2DLinearGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetStartPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(
    void* pThis, ::CD2DPointF startPoint) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_startPoint = {startPoint.x, startPoint.y};
}
// Symbol: ??0CD2DLinearGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DLinearGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, int stopCount, void* properties,
    int gammaMode, int extendMode, void* pBrushProps, int unusedFlags) {
    (void)pStops;
    (void)stopCount;
    (void)gammaMode;
    (void)extendMode;
    (void)pBrushProps;
    (void)unusedFlags;
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    ApplyLinearGradientBrushProperties(state, properties);
    if (state.m_startPoint.x == 0.0f && state.m_startPoint.y == 0.0f &&
        state.m_endPoint.x == 0.0f && state.m_endPoint.y == 0.0f) {
        state.m_endPoint = {1.0f, 0.0f};
    }
    return pThis;
}
// Symbol: ??1CD2DLinearGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DLinearGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DLinearGradientBrush__UEAAXXZ(pThis);
    }
}
