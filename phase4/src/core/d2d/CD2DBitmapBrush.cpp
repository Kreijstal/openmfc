// Native bitmap brushes and their source-wrapper lifetime.
#include "detail/D2DResourceSupport.h"
#include <atomic>
#include <cstring>
#include <memory>
#include <mutex>
#include <unordered_map>
extern "C" int MS_ABI impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(void*, void*);
namespace {
struct SourceRecord { CD2DBitmap* source; std::unique_ptr<CD2DBitmap> standaloneOwner; };
struct SourceRegistry;
std::atomic<SourceRegistry*> liveRegistry{nullptr};
struct SourceRegistry {
    std::mutex mutex;
    std::unordered_map<CD2DBitmapBrush*, SourceRecord> records;
    SourceRegistry() { liveRegistry.store(this); }
    ~SourceRegistry() { liveRegistry.store(nullptr); }
};
SourceRegistry& Sources() { static SourceRegistry registry; return registry; }
void InitializeSource(CD2DBitmapBrush* brush, CRenderTarget* parent,
    std::unique_ptr<CD2DBitmap> source, const D2D1_BITMAP_BRUSH_PROPERTIES* properties) {
    std::unique_ptr<D2D1_BITMAP_BRUSH_PROPERTIES> copy(properties ? new D2D1_BITMAP_BRUSH_PROPERTIES(*properties) : nullptr);
    auto& registry = Sources();
    auto* bitmap = source.get();
    std::lock_guard<std::mutex> lock(registry.mutex);
    auto inserted = registry.records.emplace(brush, SourceRecord{bitmap, nullptr});
    if (parent) source.release(); // Registered with the parent as an auto-owned bitmap.
    else inserted.first->second.standaloneOwner = std::move(source);
    brush->m_pBitmap = bitmap;
    brush->m_pBitmapBrushProperties = copy.release();
}
D2D1_SIZE_U SizeFromBits(unsigned long long bits) {
    D2D1_SIZE_U size; std::memcpy(&size, &bits, sizeof(size)); return size;
}
}
// A parent can delete a bitmap wrapper while a borrowed brush still survives.
// The COM brush keeps its own bitmap reference; clear the wrapper pointer so
// inline GetBitmap() and later recreation cannot use freed wrapper storage.
void OpenMfcForgetD2DBitmapSource(CD2DBitmap* bitmap) {
    auto* registry = liveRegistry.load();
    if (!registry) return;
    std::lock_guard<std::mutex> lock(registry->mutex);
    for (auto& entry : registry->records) {
        if (entry.second.standaloneOwner.get() == bitmap) entry.second.standaloneOwner.release();
        if (entry.second.source == bitmap) { entry.first->m_pBitmap = nullptr; entry.second.source = nullptr; }
    }
}
CD2DBitmapBrush::CD2DBitmapBrush(CRenderTarget* parent, const D2D1_BITMAP_BRUSH_PROPERTIES* bitmapProps,
    const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DBrush(parent, props, autoDestroy), m_pBitmap(nullptr), m_pBitmapBrush(nullptr), m_pBitmapBrushProperties(nullptr) {
    InitializeSource(this, parent, std::unique_ptr<CD2DBitmap>(new CD2DBitmap(parent, TRUE)), bitmapProps);
}
CD2DBitmapBrush::CD2DBitmapBrush(CRenderTarget* parent, UINT id, const wchar_t* type, D2D1_SIZE_U size,
    const D2D1_BITMAP_BRUSH_PROPERTIES* bitmapProps, const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DBrush(parent, props, autoDestroy), m_pBitmap(nullptr), m_pBitmapBrush(nullptr), m_pBitmapBrushProperties(nullptr) {
    InitializeSource(this, parent, std::unique_ptr<CD2DBitmap>(new CD2DBitmap(parent, id, type, size, TRUE)), bitmapProps);
}
CD2DBitmapBrush::CD2DBitmapBrush(CRenderTarget* parent, const wchar_t* path, D2D1_SIZE_U size,
    const D2D1_BITMAP_BRUSH_PROPERTIES* bitmapProps, const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DBrush(parent, props, autoDestroy), m_pBitmap(nullptr), m_pBitmapBrush(nullptr), m_pBitmapBrushProperties(nullptr) {
    InitializeSource(this, parent, std::unique_ptr<CD2DBitmap>(new CD2DBitmap(parent, path, size, TRUE)), bitmapProps);
}
CD2DBitmapBrush::~CD2DBitmapBrush() {
    CD2DBitmapBrush::Destroy();
    std::unique_ptr<CD2DBitmap> owned;
    auto& registry = Sources();
    {
        std::lock_guard<std::mutex> lock(registry.mutex);
        auto it = registry.records.find(this);
        if (it != registry.records.end()) { owned = std::move(it->second.standaloneOwner); registry.records.erase(it); }
    }
}
void CD2DBitmapBrush::CommonInit(const D2D1_BITMAP_BRUSH_PROPERTIES* props) {
    m_pBitmapBrush = nullptr;
    m_pBitmapBrushProperties = props ? new D2D1_BITMAP_BRUSH_PROPERTIES(*props) : nullptr;
}
void CD2DBitmapBrush::SetBitmap(CD2DBitmap* bitmap) {
    if (!bitmap) return;
    m_pBitmap = bitmap;
    auto& registry = Sources();
    {
        std::lock_guard<std::mutex> lock(registry.mutex);
        auto it = registry.records.find(this);
        if (it != registry.records.end()) it->second.source = bitmap;
    }
    if (m_pBitmapBrush) m_pBitmapBrush->SetBitmap(bitmap->m_pBitmap);
}
HRESULT CD2DBitmapBrush::Create(CRenderTarget* parent) {
    if (!parent || !parent->IsValid() || m_pBrush ||
        !impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(parent, m_pBitmap)) return E_FAIL;
    ID2D1BitmapBrush* brush = nullptr;
    HRESULT hr = parent->GetRenderTarget()->CreateBitmapBrush(m_pBitmap->m_pBitmap,
        m_pBitmapBrushProperties, m_pBrushProperties, &brush);
    if (SUCCEEDED(hr)) { m_pBitmapBrush = brush; m_pBrush = brush; }
    return hr;
}
void CD2DBitmapBrush::Destroy() {
    CD2DBrush::Destroy(); m_pBitmapBrush = nullptr;
    delete m_pBitmapBrushProperties; m_pBitmapBrushProperties = nullptr;
}


// Symbol: ?CommonInit@CD2DBitmapBrush@@IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES@@@Z
extern "C" void MS_ABI impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(void* self, const D2D1_BITMAP_BRUSH_PROPERTIES* props) {
    if (self) static_cast<CD2DBitmapBrush*>(self)->CommonInit(props);
}

// Symbol: ?Attach@CD2DBitmapBrush@@QEAAXPEAUID2D1BitmapBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBitmapBrush__QEAAXPEAUID2D1BitmapBrush___Z(void* self, void* native) {
    if (!self) return;
    auto* brush = static_cast<CD2DBitmapBrush*>(self);
    brush->m_pBitmapBrush = static_cast<ID2D1BitmapBrush*>(native); brush->m_pBrush = brush->m_pBitmapBrush;
}

// Symbol: ?Create@CD2DBitmapBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DBitmapBrush__UEAAJPEAVCRenderTarget___Z(void* self, void* parent) {
    return self ? static_cast<CD2DBitmapBrush*>(self)->CD2DBitmapBrush::Create(static_cast<CRenderTarget*>(parent)) : E_POINTER;
}

// Symbol: ?Destroy@CD2DBitmapBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBitmapBrush__UEAAXXZ(void* self) {
    if (self) static_cast<CD2DBitmapBrush*>(self)->CD2DBitmapBrush::Destroy();
}

// Symbol: ?Detach@CD2DBitmapBrush@@QEAAPEAUID2D1BitmapBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBitmapBrush__QEAAPEAUID2D1BitmapBrush__XZ(void* self) {
    if (!self) return nullptr;
    auto* brush = static_cast<CD2DBitmapBrush*>(self); auto* result = brush->m_pBitmapBrush;
    brush->m_pBitmapBrush = nullptr; brush->m_pBrush = nullptr; return result;
}

// Symbol: ?GetExtendModeX@CD2DBitmapBrush@@QEBA?AW4D2D1_EXTEND_MODE@@XZ
extern "C" D2D1_EXTEND_MODE MS_ABI impl__GetExtendModeX_CD2DBitmapBrush__QEBA_AW4D2D1_EXTEND_MODE__XZ(const void* self) {
    if (!self) return static_cast<D2D1_EXTEND_MODE>(-1);
    auto* brush = static_cast<const CD2DBitmapBrush*>(self);
    if (brush->m_pBitmapBrush) return brush->m_pBitmapBrush->GetExtendModeX();
    return brush->m_pBitmapBrushProperties ? brush->m_pBitmapBrushProperties->extendModeX : static_cast<D2D1_EXTEND_MODE>(-1);
}

// Symbol: ?GetExtendModeY@CD2DBitmapBrush@@QEBA?AW4D2D1_EXTEND_MODE@@XZ
extern "C" D2D1_EXTEND_MODE MS_ABI impl__GetExtendModeY_CD2DBitmapBrush__QEBA_AW4D2D1_EXTEND_MODE__XZ(const void* self) {
    if (!self) return static_cast<D2D1_EXTEND_MODE>(-1);
    auto* brush = static_cast<const CD2DBitmapBrush*>(self);
    if (brush->m_pBitmapBrush) return brush->m_pBitmapBrush->GetExtendModeY();
    return brush->m_pBitmapBrushProperties ? brush->m_pBitmapBrushProperties->extendModeY : static_cast<D2D1_EXTEND_MODE>(-1);
}

// Symbol: ?GetInterpolationMode@CD2DBitmapBrush@@QEBA?AW4D2D1_BITMAP_INTERPOLATION_MODE@@XZ
extern "C" D2D1_BITMAP_INTERPOLATION_MODE MS_ABI impl__GetInterpolationMode_CD2DBitmapBrush__QEBA_AW4D2D1_BITMAP_INTERPOLATION_MODE__XZ(const void* self) {
    if (!self) return static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(-1);
    auto* brush = static_cast<const CD2DBitmapBrush*>(self);
    if (brush->m_pBitmapBrush) return brush->m_pBitmapBrush->GetInterpolationMode();
    return brush->m_pBitmapBrushProperties ? brush->m_pBitmapBrushProperties->interpolationMode : static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(-1);
}

// Symbol: ?SetBitmap@CD2DBitmapBrush@@QEAAXPEAVCD2DBitmap@@@Z
extern "C" void MS_ABI impl__SetBitmap_CD2DBitmapBrush__QEAAXPEAVCD2DBitmap___Z(void* self, void* bitmap) {
    if (self) static_cast<CD2DBitmapBrush*>(self)->SetBitmap(static_cast<CD2DBitmap*>(bitmap));
}

// Symbol: ?SetExtendModeX@CD2DBitmapBrush@@QEAAXW4D2D1_EXTEND_MODE@@@Z
extern "C" void MS_ABI impl__SetExtendModeX_CD2DBitmapBrush__QEAAXW4D2D1_EXTEND_MODE___Z(void* self, D2D1_EXTEND_MODE value) {
    if (!self) return;
    auto* brush = static_cast<CD2DBitmapBrush*>(self);
    if (brush->m_pBitmapBrushProperties) brush->m_pBitmapBrushProperties->extendModeX = value;
    if (brush->m_pBitmapBrush) brush->m_pBitmapBrush->SetExtendModeX(value);
}

// Symbol: ?SetExtendModeY@CD2DBitmapBrush@@QEAAXW4D2D1_EXTEND_MODE@@@Z
extern "C" void MS_ABI impl__SetExtendModeY_CD2DBitmapBrush__QEAAXW4D2D1_EXTEND_MODE___Z(void* self, D2D1_EXTEND_MODE value) {
    if (!self) return;
    auto* brush = static_cast<CD2DBitmapBrush*>(self);
    if (brush->m_pBitmapBrushProperties) brush->m_pBitmapBrushProperties->extendModeY = value;
    if (brush->m_pBitmapBrush) brush->m_pBitmapBrush->SetExtendModeY(value);
}

// Symbol: ?SetInterpolationMode@CD2DBitmapBrush@@QEAAXW4D2D1_BITMAP_INTERPOLATION_MODE@@@Z
extern "C" void MS_ABI impl__SetInterpolationMode_CD2DBitmapBrush__QEAAXW4D2D1_BITMAP_INTERPOLATION_MODE___Z(void* self, D2D1_BITMAP_INTERPOLATION_MODE value) {
    if (!self) return;
    auto* brush = static_cast<CD2DBitmapBrush*>(self);
    if (brush->m_pBitmapBrushProperties) brush->m_pBitmapBrushProperties->interpolationMode = value;
    if (brush->m_pBitmapBrush) brush->m_pBitmapBrush->SetInterpolationMode(value);
}

// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@IPEB_WVCD2DSizeU@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, unsigned int id, const wchar_t* type, unsigned long long size, const D2D1_BITMAP_BRUSH_PROPERTIES* bitmapProps, const D2D1_BRUSH_PROPERTIES* props, int autoDestroy) {
    return self ? new(self) CD2DBitmapBrush(static_cast<CRenderTarget*>(parent), id, type, SizeFromBits(size), bitmapProps, props, autoDestroy) : nullptr;
}

// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, const D2D1_BITMAP_BRUSH_PROPERTIES* bitmapProps, const D2D1_BRUSH_PROPERTIES* props, int autoDestroy) {
    return self ? new(self) CD2DBitmapBrush(static_cast<CRenderTarget*>(parent), bitmapProps, props, autoDestroy) : nullptr;
}

// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@PEB_WVCD2DSizeU@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__PEB_WVCD2DSizeU__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, const wchar_t* path, unsigned long long size, const D2D1_BITMAP_BRUSH_PROPERTIES* bitmapProps, const D2D1_BRUSH_PROPERTIES* props, int autoDestroy) {
    return self ? new(self) CD2DBitmapBrush(static_cast<CRenderTarget*>(parent), path, SizeFromBits(size), bitmapProps, props, autoDestroy) : nullptr;
}

// Symbol: ??1CD2DBitmapBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DBitmapBrush__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DBitmapBrush*>(self)->CD2DBitmapBrush::~CD2DBitmapBrush();
}
