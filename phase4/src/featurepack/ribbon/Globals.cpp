// Globals — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?CopyFromBitmap@CD2DBitmap@@QEAAJPEBV1@PEBVCD2DPointU@@PEBVCD2DRectU@@@Z
extern "C" long MS_ABI impl__CopyFromBitmap_CD2DBitmap__QEAAJPEBV1_PEBVCD2DPointU__PEBVCD2DRectU___Z(
    void* pThis, const void* pBitmap, const CD2DPointU* /*pPoint*/, const CD2DRectU* pRect) {
    if (pThis == nullptr || pBitmap == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    CD2DBitmapState sourceState{};
    const bool haveSourceState = TryLoadCD2DBitmapState(const_cast<void*>(pBitmap), sourceState);

    auto& state = EnsureCD2DBitmapState(pThis);
    if (haveSourceState) {
        state.m_pixelFormat = sourceState.m_pixelFormat;
        state.m_pixelSize = sourceState.m_pixelSize;
        if (sourceState.m_dpi.width != 0.0f || sourceState.m_dpi.height != 0.0f) {
            state.m_dpi = sourceState.m_dpi;
        }
        state.m_resource = sourceState.m_resource;
    }
    if (pRect != nullptr) {
        state.m_pixelSize = CalcSizeFromRect(pRect);
    }
    if (state.m_resource == nullptr) {
        state.m_resource = const_cast<void*>(pBitmap);
    }
    return S_OK;
}
// Symbol: ?CopyFromMemory@CD2DBitmap@@QEAAJPEBXIPEBVCD2DRectU@@@Z
extern "C" long MS_ABI impl__CopyFromMemory_CD2DBitmap__QEAAJPEBXIPEBVCD2DRectU___Z(
    void* pThis, const void* pData, unsigned int dataSize, const CD2DRectU* pRect) {
    if (pThis == nullptr || (pData == nullptr && dataSize != 0)) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_resource = (pData == nullptr) ? nullptr : pThis;
    if (pRect != nullptr) {
        state.m_pixelSize = CalcSizeFromRect(pRect);
    } else if (state.m_pixelSize.width == 0 && state.m_pixelSize.height == 0 && dataSize != 0) {
        state.m_pixelSize = CD2DSizeU{1, 1};
    }
    return S_OK;
}
// Symbol: ?CopyFromRenderTarget@CD2DBitmap@@QEAAJPEBVCRenderTarget@@PEBVCD2DPointU@@PEBVCD2DRectU@@@Z
extern "C" long MS_ABI impl__CopyFromRenderTarget_CD2DBitmap__QEAAJPEBVCRenderTarget__PEBVCD2DPointU__PEBVCD2DRectU___Z(
    void* pThis, const void* pRenderTarget, const CD2DPointU* /*pPoint*/, const CD2DRectU* pRect) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_parentRenderTarget = const_cast<void*>(pRenderTarget);
    state.m_resource = const_cast<void*>(pRenderTarget);
    if (pRect != nullptr) {
        state.m_pixelSize = CalcSizeFromRect(pRect);
    }
    return S_OK;
}
// Symbol: ?Create@CD2DBitmap@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DBitmap__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}
// Symbol: ?GetDPI@CD2DBitmap@@QEBA?AVCD2DSizeF@@XZ
extern "C" ::CD2DSizeF MS_ABI impl__GetDPI_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DSizeF(96.0f, 96.0f);
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return ::CD2DSizeF(96.0f, 96.0f);
    }
    return ::CD2DSizeF(state.m_dpi.width, state.m_dpi.height);
}
// Symbol: ?GetPixelFormat@CD2DBitmap@@QEBA?AUD2D1_PIXEL_FORMAT@@XZ
extern "C" D2D1_PIXEL_FORMAT MS_ABI impl__GetPixelFormat_CD2DBitmap__QEBA_AUD2D1_PIXEL_FORMAT__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return D2D1_PIXEL_FORMAT{0, 0};
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return D2D1_PIXEL_FORMAT{0, 0};
    }
    return state.m_pixelFormat;
}
// Symbol: ?GetPixelSize@CD2DBitmap@@QEBA?AVCD2DSizeU@@XZ
extern "C" CD2DSizeU MS_ABI impl__GetPixelSize_CD2DBitmap__QEBA_AVCD2DSizeU__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return CD2DSizeU{0, 0};
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return CD2DSizeU{0, 0};
    }
    return state.m_pixelSize;
}
// Symbol: ??$HashKey@AEBUtagVARIANT@@@@YAIAEBUtagVARIANT@@@Z
extern "C" long long MS_ABI impl____HashKey_AEBUtagVARIANT____YAIAEBUtagVARIANT___Z(
    void* pThis, const void* value) {
    (void)pThis;
    if (value == nullptr) {
        return 0;
    }
    const void* firstField = value;
    return static_cast<long long>(
        HashRawBytes(firstField, sizeof(unsigned long long) * 4));
}
// Symbol: ??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z
extern "C" long long MS_ABI impl____HashKey_VCComBSTR_ATL____YAIVCComBSTR_ATL___Z(
    void* pThis, const void* value) {
    (void)pThis;
    if (value == nullptr) {
        return 0;
    }
    const void* firstField = value;
    unsigned long long hash = HashRawBytes(firstField, sizeof(unsigned long long) * 2);
    if (hash == 0) {
        hash = 1;
    }
    return static_cast<long long>(hash);
}
// Symbol: ??$SerializeElements@VCComBSTR@ATL@@@@YAXAEAVCArchive@@PEAVCComBSTR@ATL@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_VCComBSTR_ATL____YAXAEAVCArchive__PEAVCComBSTR_ATL___J_Z(
    void* pThis, void* pArchive, void* pList, long long* pHash) {
    (void)pThis;
    (void)pArchive;
    (void)pList;
    (void)pHash;
}
