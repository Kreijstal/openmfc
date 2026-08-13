// CD2DBitmap — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CD2DBitmap@@IEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__IEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int unusedType) {
    (void)pRenderTarget;
    (void)unusedType;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}
// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@IPEB_WVCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__H_Z(
    void* pThis, void* pRenderTarget, void* pBitmap, void* pSize, int unusedFlags) {
    (void)pRenderTarget;
    (void)pBitmap;
    (void)pSize;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}
// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@PEAUHBITMAP__@@VCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__PEAUHBITMAP____VCD2DSizeU__H_Z(
    void* pThis, void* pRenderTarget, void* hBitmap, void* size, int unusedFlags) {
    (void)pRenderTarget;
    (void)hBitmap;
    (void)size;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}
// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@PEB_WVCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__PEB_WVCD2DSizeU__H_Z(
    void* pThis, void* pRenderTarget, const wchar_t* pText, void* pSize, int unusedFlags) {
    (void)pRenderTarget;
    (void)pText;
    (void)pSize;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}
// Symbol: ?CommonInit@CD2DBitmap@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CD2DBitmap__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapState(pThis);
    ApplyBitmapDefaults(state);
}
// Symbol: ?GetSize@CD2DBitmap@@QEBA?AVCD2DSizeF@@XZ
extern "C" ::CD2DSizeF MS_ABI impl__GetSize_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DSizeF(0.0f, 0.0f);
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return ::CD2DSizeF(0.0f, 0.0f);
    }
    return ::CD2DSizeF(static_cast<float>(state.m_pixelSize.width), static_cast<float>(state.m_pixelSize.height));
}
// Symbol: ?Destroy@CD2DBitmap@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBitmap__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    g_cd2dbitmapStates.erase(pThis);
}
// Symbol: ?Detach@CD2DBitmap@@QEAAPEAUID2D1Bitmap@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    const auto it = g_cd2dbitmapStates.find(pThis);
    if (it == g_cd2dbitmapStates.end()) {
        return nullptr;
    }

    void* bitmap = it->second.m_resource;
    it->second.m_resource = nullptr;
    return bitmap;
}
// Symbol: ?Attach@CD2DBitmap@@QEAAXPEAUID2D1Bitmap@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(
    void* pThis, void* pBitmap)
{
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_resource = pBitmap;
}
// Symbol: ??1CD2DBitmap@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DBitmap__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DBitmap__UEAAXXZ(pThis);
    }
}
