// Linear gradients backed by native Direct2D stop collections and brushes.
#include "detail/D2DResourceSupport.h"
#include <cstring>
namespace {
D2D1_POINT_2F PointFromBits(unsigned long long bits) {
    D2D1_POINT_2F point; std::memcpy(&point, &bits, sizeof(point)); return point;
}
}
CD2DLinearGradientBrush::CD2DLinearGradientBrush(CRenderTarget* parent, const D2D1_GRADIENT_STOP* stops, UINT count,
    D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES properties, D2D1_GAMMA gamma, D2D1_EXTEND_MODE extend,
    const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DGradientBrush(parent, stops, count, gamma, extend, props, autoDestroy),
      m_LinearGradientBrushProperties(properties), m_pLinearGradientBrush(nullptr) {}
CD2DLinearGradientBrush::~CD2DLinearGradientBrush() { CD2DLinearGradientBrush::Destroy(); }
HRESULT CD2DLinearGradientBrush::Create(CRenderTarget* parent) {
    if (!parent || !parent->IsValid() || m_pBrush || !m_arGradientStops.size) return E_FAIL;
    ID2D1GradientStopCollection* stops = nullptr;
    HRESULT hr = parent->GetRenderTarget()->CreateGradientStopCollection(m_arGradientStops.data,
        static_cast<UINT>(m_arGradientStops.size), m_colorInterpolationGamma, m_extendMode, &stops);
    if (FAILED(hr)) return hr;
    ID2D1LinearGradientBrush* brush = nullptr;
    hr = parent->GetRenderTarget()->CreateLinearGradientBrush(&m_LinearGradientBrushProperties, m_pBrushProperties, stops, &brush);
    if (FAILED(hr)) { stops->Release(); return hr; }
    if (m_pGradientStops) m_pGradientStops->Release();
    m_pGradientStops = stops; m_pLinearGradientBrush = brush; m_pBrush = brush; return hr;
}
void CD2DLinearGradientBrush::Destroy() { CD2DGradientBrush::Destroy(); m_pLinearGradientBrush = nullptr; }


// Symbol: ?Attach@CD2DLinearGradientBrush@@QEAAXPEAUID2D1LinearGradientBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DLinearGradientBrush__QEAAXPEAUID2D1LinearGradientBrush___Z(void* self, void* brush) {
    if (!self) return;
    auto* object = static_cast<CD2DLinearGradientBrush*>(self);
    object->m_pLinearGradientBrush = static_cast<ID2D1LinearGradientBrush*>(brush); object->m_pBrush = object->m_pLinearGradientBrush;
}

// Symbol: ?Create@CD2DLinearGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DLinearGradientBrush__UEAAJPEAVCRenderTarget___Z(void* self, void* parent) {
    return self ? static_cast<CD2DLinearGradientBrush*>(self)->CD2DLinearGradientBrush::Create(static_cast<CRenderTarget*>(parent)) : E_POINTER;
}

// Symbol: ?Destroy@CD2DLinearGradientBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DLinearGradientBrush__UEAAXXZ(void* self) {
    if (self) static_cast<CD2DLinearGradientBrush*>(self)->CD2DLinearGradientBrush::Destroy();
}

// Symbol: ?Detach@CD2DLinearGradientBrush@@QEAAPEAUID2D1LinearGradientBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DLinearGradientBrush__QEAAPEAUID2D1LinearGradientBrush__XZ(void* self) {
    if (!self) return nullptr;
    auto* object = static_cast<CD2DLinearGradientBrush*>(self);
    auto* brush = object->m_pLinearGradientBrush; object->m_pLinearGradientBrush = nullptr; object->m_pBrush = nullptr; return brush;
}

// Symbol: ?GetEndPoint@CD2DLinearGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" void* MS_ABI impl__GetEndPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void* self, void* result) {
    if (!self || !result) return result;
    auto* object = static_cast<const CD2DLinearGradientBrush*>(self);
    auto point = object->m_pLinearGradientBrush ? object->m_pLinearGradientBrush->GetEndPoint() : object->m_LinearGradientBrushProperties.endPoint;
    std::memcpy(result, &point, sizeof(point)); return result;
}

// Symbol: ?GetStartPoint@CD2DLinearGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" void* MS_ABI impl__GetStartPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void* self, void* result) {
    if (!self || !result) return result;
    auto* object = static_cast<const CD2DLinearGradientBrush*>(self);
    auto point = object->m_pLinearGradientBrush ? object->m_pLinearGradientBrush->GetStartPoint() : object->m_LinearGradientBrushProperties.startPoint;
    std::memcpy(result, &point, sizeof(point)); return result;
}

// Symbol: ?SetEndPoint@CD2DLinearGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetEndPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(void* self, unsigned long long bits) {
    if (!self) return;
    auto* object = static_cast<CD2DLinearGradientBrush*>(self); auto point = PointFromBits(bits);
    object->m_LinearGradientBrushProperties.endPoint = point;
    if (object->m_pLinearGradientBrush) object->m_pLinearGradientBrush->SetEndPoint(point);
}

// Symbol: ?SetStartPoint@CD2DLinearGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetStartPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(void* self, unsigned long long bits) {
    if (!self) return;
    auto* object = static_cast<CD2DLinearGradientBrush*>(self); auto point = PointFromBits(bits);
    object->m_LinearGradientBrushProperties.startPoint = point;
    if (object->m_pLinearGradientBrush) object->m_pLinearGradientBrush->SetStartPoint(point);
}

// Symbol: ??0CD2DLinearGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DLinearGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, const D2D1_GRADIENT_STOP* stops, unsigned int count, const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES* properties, int gamma, int extend, void* props, int autoDestroy) {
    return self && properties ? new(self) CD2DLinearGradientBrush(static_cast<CRenderTarget*>(parent), stops, count,
        *properties, static_cast<D2D1_GAMMA>(gamma), static_cast<D2D1_EXTEND_MODE>(extend),
        static_cast<const D2D1_BRUSH_PROPERTIES*>(props), autoDestroy) : nullptr;
}

// Symbol: ??1CD2DLinearGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DLinearGradientBrush__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DLinearGradientBrush*>(self)->CD2DLinearGradientBrush::~CD2DLinearGradientBrush();
}
