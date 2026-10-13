// Radial gradients backed by native Direct2D stop collections and brushes.
#include "detail/D2DResourceSupport.h"
#include <cstring>
namespace {
D2D1_POINT_2F PointFromBits(unsigned long long bits) {
    D2D1_POINT_2F point; std::memcpy(&point, &bits, sizeof(point)); return point;
}
}
CD2DRadialGradientBrush::CD2DRadialGradientBrush(CRenderTarget* parent, const D2D1_GRADIENT_STOP* stops, UINT count,
    D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES properties, D2D1_GAMMA gamma, D2D1_EXTEND_MODE extend,
    const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DGradientBrush(parent, stops, count, gamma, extend, props, autoDestroy),
      m_RadialGradientBrushProperties(properties), m_pRadialGradientBrush(nullptr) {}
CD2DRadialGradientBrush::~CD2DRadialGradientBrush() { CD2DRadialGradientBrush::Destroy(); }
HRESULT CD2DRadialGradientBrush::Create(CRenderTarget* parent) {
    if (!parent || !parent->IsValid() || m_pBrush || !m_arGradientStops.size) return E_FAIL;
    ID2D1GradientStopCollection* stops = nullptr;
    HRESULT hr = parent->GetRenderTarget()->CreateGradientStopCollection(m_arGradientStops.data,
        static_cast<UINT>(m_arGradientStops.size), m_colorInterpolationGamma, m_extendMode, &stops);
    if (FAILED(hr)) return hr;
    ID2D1RadialGradientBrush* brush = nullptr;
    hr = parent->GetRenderTarget()->CreateRadialGradientBrush(&m_RadialGradientBrushProperties, m_pBrushProperties, stops, &brush);
    if (FAILED(hr)) { stops->Release(); return hr; }
    if (m_pGradientStops) m_pGradientStops->Release();
    m_pGradientStops = stops; m_pRadialGradientBrush = brush; m_pBrush = brush; return hr;
}
void CD2DRadialGradientBrush::Destroy() { CD2DGradientBrush::Destroy(); m_pRadialGradientBrush = nullptr; }


// Symbol: ?Attach@CD2DRadialGradientBrush@@QEAAXPEAUID2D1RadialGradientBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DRadialGradientBrush__QEAAXPEAUID2D1RadialGradientBrush___Z(void* self, void* brush) {
    if (!self) return;
    auto* object = static_cast<CD2DRadialGradientBrush*>(self);
    object->m_pRadialGradientBrush = static_cast<ID2D1RadialGradientBrush*>(brush); object->m_pBrush = object->m_pRadialGradientBrush;
}

// Symbol: ?Create@CD2DRadialGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DRadialGradientBrush__UEAAJPEAVCRenderTarget___Z(void* self, void* parent) {
    return self ? static_cast<CD2DRadialGradientBrush*>(self)->CD2DRadialGradientBrush::Create(static_cast<CRenderTarget*>(parent)) : E_POINTER;
}

// Symbol: ??0CD2DRadialGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DRadialGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, const D2D1_GRADIENT_STOP* stops, unsigned int count, const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES* properties, int gamma, int extend, void* props, int autoDestroy) {
    return self && properties ? new(self) CD2DRadialGradientBrush(static_cast<CRenderTarget*>(parent), stops, count,
        *properties, static_cast<D2D1_GAMMA>(gamma), static_cast<D2D1_EXTEND_MODE>(extend),
        static_cast<const D2D1_BRUSH_PROPERTIES*>(props), autoDestroy) : nullptr;
}

// Symbol: ??1CD2DRadialGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DRadialGradientBrush__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DRadialGradientBrush*>(self)->CD2DRadialGradientBrush::~CD2DRadialGradientBrush();
}

// Symbol: ?Destroy@CD2DRadialGradientBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DRadialGradientBrush__UEAAXXZ(void* self) {
    if (self) static_cast<CD2DRadialGradientBrush*>(self)->CD2DRadialGradientBrush::Destroy();
}

// Symbol: ?Detach@CD2DRadialGradientBrush@@QEAAPEAUID2D1RadialGradientBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DRadialGradientBrush__QEAAPEAUID2D1RadialGradientBrush__XZ(void* self) {
    if (!self) return nullptr;
    auto* object = static_cast<CD2DRadialGradientBrush*>(self);
    auto* brush = object->m_pRadialGradientBrush; object->m_pRadialGradientBrush = nullptr; object->m_pBrush = nullptr; return brush;
}

// Symbol: ?GetCenter@CD2DRadialGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" void* MS_ABI impl__GetCenter_CD2DRadialGradientBrush__QEBA_AVCD2DPointF__XZ(const void* self, void* result) {
    if (!self || !result) return result;
    auto* object = static_cast<const CD2DRadialGradientBrush*>(self);
    auto point = object->m_pRadialGradientBrush ? object->m_pRadialGradientBrush->GetCenter() : object->m_RadialGradientBrushProperties.center;
    std::memcpy(result, &point, sizeof(point)); return result;
}

// Symbol: ?GetGradientOriginOffset@CD2DRadialGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" void* MS_ABI impl__GetGradientOriginOffset_CD2DRadialGradientBrush__QEBA_AVCD2DPointF__XZ(const void* self, void* result) {
    if (!self || !result) return result;
    auto* object = static_cast<const CD2DRadialGradientBrush*>(self);
    auto point = object->m_pRadialGradientBrush ? object->m_pRadialGradientBrush->GetGradientOriginOffset() : object->m_RadialGradientBrushProperties.gradientOriginOffset;
    std::memcpy(result, &point, sizeof(point)); return result;
}

// Symbol: ?GetRadiusX@CD2DRadialGradientBrush@@QEBAMXZ
extern "C" float MS_ABI impl__GetRadiusX_CD2DRadialGradientBrush__QEBAMXZ(const void* self) {
    if (!self) return 0;
    auto* object = static_cast<const CD2DRadialGradientBrush*>(self);
    return object->m_pRadialGradientBrush ? object->m_pRadialGradientBrush->GetRadiusX() : object->m_RadialGradientBrushProperties.radiusX;
}

// Symbol: ?GetRadiusY@CD2DRadialGradientBrush@@QEBAMXZ
extern "C" float MS_ABI impl__GetRadiusY_CD2DRadialGradientBrush__QEBAMXZ(const void* self) {
    if (!self) return 0;
    auto* object = static_cast<const CD2DRadialGradientBrush*>(self);
    return object->m_pRadialGradientBrush ? object->m_pRadialGradientBrush->GetRadiusY() : object->m_RadialGradientBrushProperties.radiusY;
}

// Symbol: ?SetCenter@CD2DRadialGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetCenter_CD2DRadialGradientBrush__QEAAXVCD2DPointF___Z(void* self, unsigned long long bits) {
    if (!self) return;
    auto* object = static_cast<CD2DRadialGradientBrush*>(self); auto point = PointFromBits(bits);
    object->m_RadialGradientBrushProperties.center = point;
    if (object->m_pRadialGradientBrush) object->m_pRadialGradientBrush->SetCenter(point);
}

// Symbol: ?SetGradientOriginOffset@CD2DRadialGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetGradientOriginOffset_CD2DRadialGradientBrush__QEAAXVCD2DPointF___Z(void* self, unsigned long long bits) {
    if (!self) return;
    auto* object = static_cast<CD2DRadialGradientBrush*>(self); auto point = PointFromBits(bits);
    object->m_RadialGradientBrushProperties.gradientOriginOffset = point;
    if (object->m_pRadialGradientBrush) object->m_pRadialGradientBrush->SetGradientOriginOffset(point);
}

// Symbol: ?SetRadiusX@CD2DRadialGradientBrush@@QEAAXM@Z
extern "C" void MS_ABI impl__SetRadiusX_CD2DRadialGradientBrush__QEAAXM_Z(void* self, float value) {
    if (!self) return;
    auto* object = static_cast<CD2DRadialGradientBrush*>(self); object->m_RadialGradientBrushProperties.radiusX = value;
    if (object->m_pRadialGradientBrush) object->m_pRadialGradientBrush->SetRadiusX(value);
}

// Symbol: ?SetRadiusY@CD2DRadialGradientBrush@@QEAAXM@Z
extern "C" void MS_ABI impl__SetRadiusY_CD2DRadialGradientBrush__QEAAXM_Z(void* self, float value) {
    if (!self) return;
    auto* object = static_cast<CD2DRadialGradientBrush*>(self); object->m_RadialGradientBrushProperties.radiusY = value;
    if (object->m_pRadialGradientBrush) object->m_pRadialGradientBrush->SetRadiusY(value);
}
