// CD2DBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Attach@CD2DBrush@@QEAAXPEAUID2D1Brush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void* pThis, void* pBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBrushState(pThis);
    state.m_resource = pBrush;
}
// Symbol: ?Destroy@CD2DBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBrush__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    g_cd2dBrushStates.erase(pThis);
}
// Symbol: ?Detach@CD2DBrush@@QEAAPEAUID2D1Brush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    auto it = g_cd2dBrushStates.find(pThis);
    if (it == g_cd2dBrushStates.end()) {
        return nullptr;
    }
    void* brush = it->second.m_resource;
    it->second.m_resource = nullptr;
    return brush;
}
// Symbol: ?GetOpacity@CD2DBrush@@QEBAMXZ
extern "C" float MS_ABI impl__GetOpacity_CD2DBrush__QEBAMXZ(const void* pThis) {
    if (pThis == nullptr) {
        return 1.0f;
    }
    CD2DBrushState state{};
    if (!TryLoadCD2DBrushState(const_cast<void*>(pThis), state)) {
        return 1.0f;
    }
    return state.m_opacity;
}
// Symbol: ?GetTransform@CD2DBrush@@QEBAXPEAUD2D_MATRIX_3X2_F@@@Z
extern "C" void MS_ABI impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(
    const void* pThis, D2D_MATRIX_3X2_F* pMatrix) {
    if (pThis == nullptr || pMatrix == nullptr) {
        return;
    }

    CD2DBrushState state{};
    if (!TryLoadCD2DBrushState(const_cast<void*>(pThis), state)) {
        pMatrix->_11 = 1.0f;
        pMatrix->_12 = 0.0f;
        pMatrix->_21 = 0.0f;
        pMatrix->_22 = 1.0f;
        pMatrix->_31 = 0.0f;
        pMatrix->_32 = 0.0f;
        return;
    }
    *pMatrix = state.m_transform;
}
// Symbol: ?SetOpacity@CD2DBrush@@QEAAXM@Z
extern "C" void MS_ABI impl__SetOpacity_CD2DBrush__QEAAXM_Z(void* pThis, float opacity) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBrushState(pThis);
    state.m_opacity = opacity;
}
// Symbol: ?SetTransform@CD2DBrush@@QEAAXPEBUD2D_MATRIX_3X2_F@@@Z
extern "C" void MS_ABI impl__SetTransform_CD2DBrush__QEAAXPEBUD2D_MATRIX_3X2_F___Z(
    void* pThis, const D2D_MATRIX_3X2_F* pMatrix) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBrushState(pThis);
    if (pMatrix == nullptr) {
        state.m_transform = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
        return;
    }
    state.m_transform = *pMatrix;
}
// Symbol: ??0CD2DBrush@@IEAA@PEAVCRenderTarget@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)unusedFlags;
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCD2DBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    ApplyCD2DBrushDefaults(state);
    ApplyTransformFromBrushProperties(state, static_cast<const float*>(pBrushProperties));
    return pThis;
}
// Symbol: ??1CD2DBrush@@MEAA@XZ
extern "C" void MS_ABI impl___1CD2DBrush__MEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DBrush__UEAAXXZ(pThis);
    }
}
