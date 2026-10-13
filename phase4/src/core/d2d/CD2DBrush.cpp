// Native brush storage and COM properties, matching retail client-inline access.
#include "detail/D2DResourceSupport.h"
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <memory>

extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void*, void*, int);
namespace {
std::atomic<void*> brushConstructorVtable{nullptr};
struct BrushVtableProbe : CD2DBrush {
    BrushVtableProbe() : CD2DBrush(nullptr, nullptr, FALSE) {}
    HRESULT Create(CRenderTarget*) override { std::abort(); }
};
}
CD2DBrush::CD2DBrush(CRenderTarget* parent, const D2D1_BRUSH_PROPERTIES* props, BOOL autoDestroy)
    : CD2DResource(parent, autoDestroy), m_pBrush(nullptr),
      m_pBrushProperties(props ? new D2D1_BRUSH_PROPERTIES(*props) : nullptr) {
    brushConstructorVtable.store(*reinterpret_cast<void**>(this), std::memory_order_relaxed);
}
CD2DBrush::~CD2DBrush() { CD2DBrush::Destroy(); }
void CD2DBrush::Destroy() {
    auto* brush = m_pBrush;
    m_pBrush = nullptr;
    if (brush) brush->Release();
    delete m_pBrushProperties;
    m_pBrushProperties = nullptr;
}
void* CD2DBrush::ConstructorVtable() {
    static void* const table = [] {
        BrushVtableProbe probe;
        return brushConstructorVtable.load(std::memory_order_relaxed);
    }();
    return table;
}

// Symbol: ?Attach@CD2DBrush@@QEAAXPEAUID2D1Brush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void* self, void* brush) {
    if (self) static_cast<CD2DBrush*>(self)->m_pBrush = static_cast<ID2D1Brush*>(brush);
}
// Symbol: ?Destroy@CD2DBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBrush__UEAAXXZ(void* self) {
    if (self) static_cast<CD2DBrush*>(self)->CD2DBrush::Destroy();
}
// Symbol: ?Detach@CD2DBrush@@QEAAPEAUID2D1Brush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void* self) {
    if (!self) return nullptr;
    auto* object = static_cast<CD2DBrush*>(self);
    auto* brush = object->m_pBrush;
    object->m_pBrush = nullptr;
    return brush;
}
// Symbol: ?GetOpacity@CD2DBrush@@QEBAMXZ
extern "C" float MS_ABI impl__GetOpacity_CD2DBrush__QEBAMXZ(const void* self) {
    auto* brush = self ? static_cast<const CD2DBrush*>(self)->m_pBrush : nullptr;
    return brush ? brush->GetOpacity() : 0.0f;
}
// Symbol: ?GetTransform@CD2DBrush@@QEBAXPEAUD2D_MATRIX_3X2_F@@@Z
extern "C" void MS_ABI impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(const void* self, D2D_MATRIX_3X2_F* matrix) {
    if (!matrix) return;
    auto* brush = self ? static_cast<const CD2DBrush*>(self)->m_pBrush : nullptr;
    if (brush) brush->GetTransform(matrix);
}
// Symbol: ?SetOpacity@CD2DBrush@@QEAAXM@Z
extern "C" void MS_ABI impl__SetOpacity_CD2DBrush__QEAAXM_Z(void* self, float opacity) {
    auto* brush = self ? static_cast<CD2DBrush*>(self)->m_pBrush : nullptr;
    if (brush) brush->SetOpacity(opacity);
}
// Symbol: ?SetTransform@CD2DBrush@@QEAAXPEBUD2D_MATRIX_3X2_F@@@Z
extern "C" void MS_ABI impl__SetTransform_CD2DBrush__QEAAXPEBUD2D_MATRIX_3X2_F___Z(void* self, const D2D_MATRIX_3X2_F* matrix) {
    auto* brush = self ? static_cast<CD2DBrush*>(self)->m_pBrush : nullptr;
    if (brush && matrix) brush->SetTransform(matrix);
}
// Symbol: ??0CD2DBrush@@IEAA@PEAVCRenderTarget@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(void* self, void* parent, void* properties, int autoDestroy) {
    if (!self) return nullptr;
    auto* props = static_cast<const D2D1_BRUSH_PROPERTIES*>(properties);
    std::unique_ptr<D2D1_BRUSH_PROPERTIES> copy(props ? new D2D1_BRUSH_PROPERTIES(*props) : nullptr);
    void* table = CD2DBrush::ConstructorVtable();
    impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(self, parent, autoDestroy);
    std::memcpy(self, &table, sizeof(table));
    auto* brush = static_cast<CD2DBrush*>(self);
    brush->m_pBrush = nullptr;
    brush->m_pBrushProperties = copy.release();
    return self;
}
// Symbol: ??1CD2DBrush@@MEAA@XZ
extern "C" void MS_ABI impl___1CD2DBrush__MEAA_XZ(void* self) {
    if (self) static_cast<CD2DBrush*>(self)->CD2DBrush::~CD2DBrush();
}
