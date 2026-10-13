// Native bitmap resources, WIC decoding and MSVC ABI accessors.
#include "detail/D2DResourceSupport.h"
#include <wincodec.h>
#include <cstring>

extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*, const wchar_t*);
void OpenMfcForgetD2DBitmapSource(CD2DBitmap*);
namespace {
template<class T> struct ComScope {
    T* value = nullptr;
    ~ComScope() { if (value) value->Release(); }
    T** Out() { return &value; }
    T* operator->() const { return value; }
};
D2D1_SIZE_U SizeFromBits(unsigned long long bits) {
    D2D1_SIZE_U result; std::memcpy(&result, &bits, sizeof(result)); return result;
}
struct CopiedBitmapCleanup {
    CD2DBitmap& bitmap;
    ~CopiedBitmapCleanup() {
        if (bitmap.m_bAutoDestroyHBMP && bitmap.m_hBmpSrc) {
            DeleteObject(bitmap.m_hBmpSrc); bitmap.m_hBmpSrc = nullptr;
        }
    }
};
}
CD2DBitmap::CD2DBitmap(CRenderTarget* parent, BOOL autoDestroy)
    : CD2DResource(parent, autoDestroy), m_pBitmap(nullptr), m_uiResID(0), m_lpszType(nullptr),
      m_sizeDest{}, m_hBmpSrc(nullptr), m_bAutoDestroyHBMP(FALSE) {}
CD2DBitmap::CD2DBitmap(CRenderTarget* parent, UINT id, const wchar_t* type, D2D1_SIZE_U size, BOOL autoDestroy)
    : CD2DBitmap(parent, autoDestroy) { m_uiResID = id; m_lpszType = type; m_sizeDest = size; }
CD2DBitmap::CD2DBitmap(CRenderTarget* parent, const wchar_t* path, D2D1_SIZE_U size, BOOL autoDestroy)
    : CD2DBitmap(parent, autoDestroy) { m_strPath = path; m_sizeDest = size; }
CD2DBitmap::CD2DBitmap(CRenderTarget* parent, HBITMAP source, D2D1_SIZE_U size, BOOL autoDestroy)
    : CD2DBitmap(parent, autoDestroy) {
    // Retail copies a handle for deferred use, or immediately uploads it when
    // the parent is already valid. The destination size is stored afterwards.
    if (parent && parent->IsValid()) { m_hBmpSrc = source; Create(parent); }
    else {
        m_hBmpSrc = static_cast<HBITMAP>(CopyImage(source, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
        m_bAutoDestroyHBMP = TRUE;
    }
    m_sizeDest = size;
}
CD2DBitmap::~CD2DBitmap() {
    OpenMfcForgetD2DBitmapSource(this);
    CD2DBitmap::Destroy();
    // Also release a deferred copy when the bitmap was never created.
    CopiedBitmapCleanup cleanup{*this};
}
void CD2DBitmap::CommonInit() { m_pBitmap = nullptr; m_uiResID = 0; m_bAutoDestroyHBMP = FALSE; }
void CD2DBitmap::Destroy() {
    auto* bitmap = m_pBitmap; m_pBitmap = nullptr;
    if (bitmap) bitmap->Release();
}
HRESULT CD2DBitmap::Create(CRenderTarget* parent) {
    if (!parent || !parent->IsValid() || m_pBitmap) return E_FAIL;
    CopiedBitmapCleanup cleanup{*this};
    auto* factory = OpenMfcD2DState().GetWICFactory();
    if (!factory) return E_FAIL;
    ComScope<IWICBitmapDecoder> decoder;
    ComScope<IWICBitmapFrameDecode> frame;
    ComScope<IWICStream> stream;
    ComScope<IWICBitmapScaler> scaler;
    ComScope<IWICFormatConverter> converter;
    ComScope<IWICBitmap> fromHandle;
    HRESULT hr;
    bool handleSource = false;
    if (!m_strPath.IsEmpty()) {
        hr = factory->CreateDecoderFromFilename(m_strPath.GetString(), nullptr, GENERIC_READ,
            WICDecodeMetadataCacheOnLoad, decoder.Out());
        if (FAILED(hr)) return hr;
    } else if (m_uiResID) {
        const wchar_t* name = MAKEINTRESOURCEW(static_cast<WORD>(m_uiResID));
        const wchar_t* type = m_lpszType ? m_lpszType : L"image";
        HINSTANCE module = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, type);
        HRSRC resource = module ? FindResourceW(module, name, type) : nullptr;
        HGLOBAL loaded = resource ? LoadResource(module, resource) : nullptr;
        BYTE* data = loaded ? static_cast<BYTE*>(LockResource(loaded)) : nullptr;
        DWORD length = resource ? SizeofResource(module, resource) : 0;
        if (!data || !length) return E_FAIL;
        hr = factory->CreateStream(stream.Out());
        if (SUCCEEDED(hr)) hr = stream->InitializeFromMemory(data, length);
        if (SUCCEEDED(hr)) hr = factory->CreateDecoderFromStream(stream.value, nullptr,
            WICDecodeMetadataCacheOnLoad, decoder.Out());
        if (FAILED(hr)) return hr;
    } else if (m_hBmpSrc) {
        hr = factory->CreateBitmapFromHBITMAP(m_hBmpSrc, nullptr, WICBitmapUseAlpha, fromHandle.Out());
        if (FAILED(hr)) return hr;
        handleSource = true;
    } else return E_FAIL;
    IWICBitmapSource* source = fromHandle.value;
    if (!handleSource) {
        hr = decoder->GetFrame(0, frame.Out());
        if (FAILED(hr)) return hr;
        source = frame.value;
    }
    UINT width = m_sizeDest.width, height = m_sizeDest.height;
    if (!handleSource && (width || height)) {
        UINT originalWidth = 0, originalHeight = 0;
        hr = source->GetSize(&originalWidth, &originalHeight);
        if (FAILED(hr)) return hr;
        if (!originalWidth || !originalHeight) return E_FAIL;
        if (!width) width = static_cast<UINT>(static_cast<float>(height) / originalHeight * originalWidth);
        if (!height) height = static_cast<UINT>(static_cast<float>(width) / originalWidth * originalHeight);
        hr = factory->CreateBitmapScaler(scaler.Out());
        if (SUCCEEDED(hr)) hr = scaler->Initialize(source, width, height, WICBitmapInterpolationModeCubic);
        if (FAILED(hr)) return hr;
        source = scaler.value;
    }
    // HBITMAP sources can expose straight-alpha BGRA, which Direct2D cannot
    // upload directly. Normalize all source kinds to premultiplied BGRA.
    hr = factory->CreateFormatConverter(converter.Out());
    if (SUCCEEDED(hr)) hr = converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) return hr;
    return parent->GetRenderTarget()->CreateBitmapFromWicBitmap(converter.value, nullptr, &m_pBitmap);
}


// Symbol: ??0CD2DBitmap@@IEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__IEAA_PEAVCRenderTarget__H_Z(void* self, void* parent, int autoDestroy) {
    return self ? new(self) CD2DBitmap(static_cast<CRenderTarget*>(parent), autoDestroy) : nullptr;
}

// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@IPEB_WVCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__H_Z(void* self, void* parent, unsigned int id, const wchar_t* type, unsigned long long size, int autoDestroy) {
    return self ? new(self) CD2DBitmap(static_cast<CRenderTarget*>(parent), id, type, SizeFromBits(size), autoDestroy) : nullptr;
}

// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@PEAUHBITMAP__@@VCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__PEAUHBITMAP____VCD2DSizeU__H_Z(void* self, void* parent, HBITMAP source, unsigned long long size, int autoDestroy) {
    return self ? new(self) CD2DBitmap(static_cast<CRenderTarget*>(parent), source, SizeFromBits(size), autoDestroy) : nullptr;
}

// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@PEB_WVCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__PEB_WVCD2DSizeU__H_Z(void* self, void* parent, const wchar_t* path, unsigned long long size, int autoDestroy) {
    return self ? new(self) CD2DBitmap(static_cast<CRenderTarget*>(parent), path, SizeFromBits(size), autoDestroy) : nullptr;
}

// Symbol: ?CommonInit@CD2DBitmap@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CD2DBitmap__IEAAXXZ(void* self) {
    if (self) static_cast<CD2DBitmap*>(self)->CD2DBitmap::CommonInit();
}

// Symbol: ?GetSize@CD2DBitmap@@QEBA?AVCD2DSizeF@@XZ
extern "C" void* MS_ABI impl__GetSize_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void* self, void* result) {
    if (!result) return nullptr;
    auto* bitmap = self ? static_cast<const CD2DBitmap*>(self)->m_pBitmap : nullptr;
    D2D1_SIZE_F value{};
    if (bitmap) { value = bitmap->GetSize(); }
    std::memcpy(result, &value, sizeof(value)); return result;
}

// Symbol: ?Destroy@CD2DBitmap@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBitmap__UEAAXXZ(void* self) {
    if (self) static_cast<CD2DBitmap*>(self)->CD2DBitmap::Destroy();
}

// Symbol: ?Detach@CD2DBitmap@@QEAAPEAUID2D1Bitmap@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(void* self) {
    if (!self) return nullptr;
    auto* object = static_cast<CD2DBitmap*>(self);
    auto* bitmap = object->m_pBitmap; object->m_pBitmap = nullptr; return bitmap;
}

// Symbol: ?Attach@CD2DBitmap@@QEAAXPEAUID2D1Bitmap@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(void* self, void* native) {
    if (self) static_cast<CD2DBitmap*>(self)->m_pBitmap = static_cast<ID2D1Bitmap*>(native);
}

// Symbol: ??1CD2DBitmap@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DBitmap__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DBitmap*>(self)->CD2DBitmap::~CD2DBitmap();
}

// Symbol: ?CopyFromBitmap@CD2DBitmap@@QEAAJPEBV1@PEBVCD2DPointU@@PEBVCD2DRectU@@@Z
extern "C" long MS_ABI impl__CopyFromBitmap_CD2DBitmap__QEAAJPEBV1_PEBVCD2DPointU__PEBVCD2DRectU___Z(void* self, const void* source, const D2D1_POINT_2U* point, const D2D1_RECT_U* rect) {
    auto* bitmap = self ? static_cast<CD2DBitmap*>(self)->m_pBitmap : nullptr;
    auto* input = source ? static_cast<const CD2DBitmap*>(source)->m_pBitmap : nullptr;
    return bitmap && input ? bitmap->CopyFromBitmap(point, input, rect) : E_FAIL;
}

// Symbol: ?CopyFromMemory@CD2DBitmap@@QEAAJPEBXIPEBVCD2DRectU@@@Z
extern "C" long MS_ABI impl__CopyFromMemory_CD2DBitmap__QEAAJPEBXIPEBVCD2DRectU___Z(void* self, const void* data, unsigned int pitch, const D2D1_RECT_U* rect) {
    auto* bitmap = self ? static_cast<CD2DBitmap*>(self)->m_pBitmap : nullptr;
    return bitmap ? bitmap->CopyFromMemory(rect, data, pitch) : E_FAIL;
}

// Symbol: ?CopyFromRenderTarget@CD2DBitmap@@QEAAJPEBVCRenderTarget@@PEBVCD2DPointU@@PEBVCD2DRectU@@@Z
extern "C" long MS_ABI impl__CopyFromRenderTarget_CD2DBitmap__QEAAJPEBVCRenderTarget__PEBVCD2DPointU__PEBVCD2DRectU___Z(void* self, const void* source, const D2D1_POINT_2U* point, const D2D1_RECT_U* rect) {
    auto* bitmap = self ? static_cast<CD2DBitmap*>(self)->m_pBitmap : nullptr;
    auto* input = source ? static_cast<const CRenderTarget*>(source)->m_pRenderTarget : nullptr;
    return bitmap && input ? bitmap->CopyFromRenderTarget(point, input, rect) : E_FAIL;
}

// Symbol: ?Create@CD2DBitmap@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DBitmap__UEAAJPEAVCRenderTarget___Z(void* self, void* parent) {
    return self ? static_cast<CD2DBitmap*>(self)->CD2DBitmap::Create(static_cast<CRenderTarget*>(parent)) : E_POINTER;
}

// Symbol: ?GetDPI@CD2DBitmap@@QEBA?AVCD2DSizeF@@XZ
extern "C" void* MS_ABI impl__GetDPI_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void* self, void* result) {
    if (!result) return nullptr;
    auto* bitmap = self ? static_cast<const CD2DBitmap*>(self)->m_pBitmap : nullptr;
    D2D1_SIZE_F value{};
    if (bitmap) { bitmap->GetDpi(&value.width, &value.height); }
    std::memcpy(result, &value, sizeof(value)); return result;
}

// Symbol: ?GetPixelFormat@CD2DBitmap@@QEBA?AUD2D1_PIXEL_FORMAT@@XZ
extern "C" void* MS_ABI impl__GetPixelFormat_CD2DBitmap__QEBA_AUD2D1_PIXEL_FORMAT__XZ(const void* self, void* result) {
    if (!result) return nullptr;
    auto* bitmap = self ? static_cast<const CD2DBitmap*>(self)->m_pBitmap : nullptr;
    D2D1_PIXEL_FORMAT value{};
    if (bitmap) { value = bitmap->GetPixelFormat(); }
    std::memcpy(result, &value, sizeof(value)); return result;
}

// Symbol: ?GetPixelSize@CD2DBitmap@@QEBA?AVCD2DSizeU@@XZ
extern "C" void* MS_ABI impl__GetPixelSize_CD2DBitmap__QEBA_AVCD2DSizeU__XZ(const void* self, void* result) {
    if (!result) return nullptr;
    auto* bitmap = self ? static_cast<const CD2DBitmap*>(self)->m_pBitmap : nullptr;
    D2D1_SIZE_U value{};
    if (bitmap) { value = bitmap->GetPixelSize(); }
    std::memcpy(result, &value, sizeof(value)); return result;
}
