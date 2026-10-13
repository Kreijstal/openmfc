// Native gradient stop storage and lifecycle.
#include "detail/D2DResourceSupport.h"
#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <memory>
extern "C" void* MS_ABI impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(void*, void*, void*, int);
namespace {
std::atomic<void*> gradientConstructorVtable{nullptr};
struct GradientVtableProbe : CD2DGradientBrush {
    GradientVtableProbe() : CD2DGradientBrush(nullptr, nullptr, 0, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, nullptr, FALSE) {}
    HRESULT Create(CRenderTarget*) override { std::abort(); }
};
}
D2DGradientStopArray::~D2DGradientStopArray() { delete[] data; }
void D2DGradientStopArray::Assign(const D2D1_GRADIENT_STOP* stops, UINT count) {
    if (!stops || !count) return;
    std::unique_ptr<D2D1_GRADIENT_STOP[]> copy(new D2D1_GRADIENT_STOP[count]);
    std::copy_n(stops, count, copy.get());
    delete[] data; data = copy.release(); size = capacity = count;
}
CD2DGradientBrush::CD2DGradientBrush(CRenderTarget* parent, const D2D1_GRADIENT_STOP* stops, UINT count,
    D2D1_GAMMA gamma, D2D1_EXTEND_MODE extend, const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DBrush(parent, props, autoDestroy), m_colorInterpolationGamma(gamma), m_extendMode(extend), m_pGradientStops(nullptr) {
    gradientConstructorVtable.store(*reinterpret_cast<void**>(this), std::memory_order_relaxed);
    m_arGradientStops.Assign(stops, count);
}
CD2DGradientBrush::~CD2DGradientBrush() { CD2DGradientBrush::Destroy(); }
void CD2DGradientBrush::Destroy() {
    CD2DBrush::Destroy();
    auto* stops = m_pGradientStops; m_pGradientStops = nullptr;
    if (stops) stops->Release();
}
void* CD2DGradientBrush::ConstructorVtable() {
    static void* const table = [] {
        GradientVtableProbe probe;
        return gradientConstructorVtable.load(std::memory_order_relaxed);
    }();
    return table;
}


// Symbol: ??0CD2DGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IW4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IW4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, const D2D1_GRADIENT_STOP* stops, unsigned int count, int gamma, int extend, void* props, int autoDestroy) {
    if (!self) return nullptr;
    void* table = CD2DGradientBrush::ConstructorVtable();
    impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(self, parent, props, autoDestroy);
    auto* gradient = static_cast<CD2DGradientBrush*>(self);
    new(&gradient->m_arGradientStops) D2DGradientStopArray;
    gradient->m_colorInterpolationGamma = static_cast<D2D1_GAMMA>(gamma);
    gradient->m_extendMode = static_cast<D2D1_EXTEND_MODE>(extend);
    gradient->m_pGradientStops = nullptr;
    try { gradient->m_arGradientStops.Assign(stops, count); }
    catch (...) {
        gradient->m_arGradientStops.~D2DGradientStopArray();
        gradient->CD2DBrush::~CD2DBrush(); throw;
    }
    std::memcpy(self, &table, sizeof(table)); return self;
}

// Symbol: ??1CD2DGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DGradientBrush__UEAA_XZ(void* self) {
    if (self) static_cast<CD2DGradientBrush*>(self)->CD2DGradientBrush::~CD2DGradientBrush();
}

// Symbol: ?Destroy@CD2DGradientBrush@@MEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DGradientBrush__MEAAXXZ(void* self) {
    if (self) static_cast<CD2DGradientBrush*>(self)->CD2DGradientBrush::Destroy();
}
