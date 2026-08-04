// CD2DBitmapBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?CommonInit@CD2DBitmapBrush@@IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES@@@Z
extern "C" void MS_ABI impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(
    void* pThis, void* pBrushProps) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    ApplyBitmapBrushDefaults(state);
    state.m_bitmapObject = pBrushProps;
}
// Symbol: ?Attach@CD2DBitmapBrush@@QEAAXPEAUID2D1BitmapBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBitmapBrush__QEAAXPEAUID2D1BitmapBrush___Z(void* pThis, void* pBitmapBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_resource = pBitmapBrush;
}
// Symbol: ?Create@CD2DBitmapBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DBitmapBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}
// Symbol: ?Destroy@CD2DBitmapBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBitmapBrush__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    g_cd2dbitmapBrushStates.erase(pThis);
}
// Symbol: ?Detach@CD2DBitmapBrush@@QEAAPEAUID2D1BitmapBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBitmapBrush__QEAAPEAUID2D1BitmapBrush__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    const auto it = g_cd2dbitmapBrushStates.find(pThis);
    if (it == g_cd2dbitmapBrushStates.end()) {
        return nullptr;
    }

    void* brush = it->second.m_resource;
    it->second.m_resource = nullptr;
    return brush;
}
// Symbol: ?GetExtendModeX@CD2DBitmapBrush@@QEBA?AW4D2D1_EXTEND_MODE@@XZ
extern "C" D2D1_EXTEND_MODE MS_ABI impl__GetExtendModeX_CD2DBitmapBrush__QEBA_AW4D2D1_EXTEND_MODE__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }

    CD2DBitmapBrushState state{};
    if (!TryLoadCD2DBitmapBrushState(const_cast<void*>(pThis), state)) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }
    return state.m_extendModeX;
}
// Symbol: ?GetExtendModeY@CD2DBitmapBrush@@QEBA?AW4D2D1_EXTEND_MODE@@XZ
extern "C" D2D1_EXTEND_MODE MS_ABI impl__GetExtendModeY_CD2DBitmapBrush__QEBA_AW4D2D1_EXTEND_MODE__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }

    CD2DBitmapBrushState state{};
    if (!TryLoadCD2DBitmapBrushState(const_cast<void*>(pThis), state)) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }
    return state.m_extendModeY;
}
// Symbol: ?GetInterpolationMode@CD2DBitmapBrush@@QEBA?AW4D2D1_BITMAP_INTERPOLATION_MODE@@XZ
extern "C" D2D1_BITMAP_INTERPOLATION_MODE MS_ABI impl__GetInterpolationMode_CD2DBitmapBrush__QEBA_AW4D2D1_BITMAP_INTERPOLATION_MODE__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(0);
    }

    CD2DBitmapBrushState state{};
    if (!TryLoadCD2DBitmapBrushState(const_cast<void*>(pThis), state)) {
        return static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(0);
    }
    return state.m_interpolationMode;
}
// Symbol: ?SetBitmap@CD2DBitmapBrush@@QEAAXPEAVCD2DBitmap@@@Z
extern "C" void MS_ABI impl__SetBitmap_CD2DBitmapBrush__QEAAXPEAVCD2DBitmap___Z(
    void* pThis, void* pBitmap) {
    if (pThis == nullptr) {
        return;
    }
    if (pBitmap == nullptr) {
        return;
    }

    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_bitmap = pBitmap;
    if (state.m_bitmapObject == nullptr && state.m_resource == nullptr) {
        state.m_resource = pBitmap;
    }
}
// Symbol: ?SetExtendModeX@CD2DBitmapBrush@@QEAAXW4D2D1_EXTEND_MODE@@@Z
extern "C" void MS_ABI impl__SetExtendModeX_CD2DBitmapBrush__QEAAXW4D2D1_EXTEND_MODE___Z(
    void* pThis, D2D1_EXTEND_MODE extendMode) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_extendModeX = extendMode;
}
// Symbol: ?SetExtendModeY@CD2DBitmapBrush@@QEAAXW4D2D1_EXTEND_MODE@@@Z
extern "C" void MS_ABI impl__SetExtendModeY_CD2DBitmapBrush__QEAAXW4D2D1_EXTEND_MODE___Z(
    void* pThis, D2D1_EXTEND_MODE extendMode) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_extendModeY = extendMode;
}
// Symbol: ?SetInterpolationMode@CD2DBitmapBrush@@QEAAXW4D2D1_BITMAP_INTERPOLATION_MODE@@@Z
extern "C" void MS_ABI impl__SetInterpolationMode_CD2DBitmapBrush__QEAAXW4D2D1_BITMAP_INTERPOLATION_MODE___Z(
    void* pThis, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_interpolationMode = interpolationMode;
}
// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@IPEB_WVCD2DSizeU@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pBitmap, const wchar_t* pText, void* pBrushProps, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)pBitmap;
    (void)pText;
    (void)pBrushProps;
    (void)pBrushProperties;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(pThis, pBrushProps);
    }
    return pThis;
}
// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pProps, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)pProps;
    (void)pBrushProperties;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(pThis, pProps);
    }
    return pThis;
}
// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@PEB_WVCD2DSizeU@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__PEB_WVCD2DSizeU__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, const wchar_t* pText, void* pProps, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)pText;
    (void)pProps;
    (void)pBrushProperties;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(pThis, pProps);
    }
    return pThis;
}
// Symbol: ??1CD2DBitmapBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DBitmapBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DBitmapBrush__UEAAXXZ(pThis);
    }
}
