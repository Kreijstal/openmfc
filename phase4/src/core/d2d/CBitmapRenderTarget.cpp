// CBitmapRenderTarget — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?GetBitmap@CBitmapRenderTarget@@QEAAHAEAVCD2DBitmap@@@Z
extern "C" int MS_ABI impl__GetBitmap_CBitmapRenderTarget__QEAAHAEAVCD2DBitmap___Z(
    void* pThis, void* pBitmap) {
    if (pThis == nullptr || pBitmap == nullptr) {
        return 0;
    }

    void* bitmap = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_bitmapRenderTargetMutex);
        auto it = g_bitmapRenderTargetBitmaps.find(pThis);
        if (it == g_bitmapRenderTargetBitmaps.end()) {
            return 0;
        }
        bitmap = it->second;
    }

    *static_cast<void**>(pBitmap) = bitmap;
    return 1;
}
// Symbol: ?Attach@CBitmapRenderTarget@@QEAAXPEAUID2D1BitmapRenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(
    void* pThis, void* pBitmapTarget) {
    if (pThis == nullptr || pBitmapTarget == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_bitmapRenderTargetMutex);
    g_bitmapRenderTargetBitmaps[pThis] = pBitmapTarget;
}
// Symbol: ?Detach@CBitmapRenderTarget@@QEAAPEAUID2D1BitmapRenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CBitmapRenderTarget__QEAAPEAUID2D1BitmapRenderTarget__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_bitmapRenderTargetMutex);
    auto it = g_bitmapRenderTargetBitmaps.find(pThis);
    if (it == g_bitmapRenderTargetBitmaps.end()) {
        return nullptr;
    }
    void* bitmap = it->second;
    g_bitmapRenderTargetBitmaps.erase(it);
    return bitmap;
}
