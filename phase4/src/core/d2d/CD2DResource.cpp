// Direct2D resource ownership and virtual lifecycle, matching the MSVC ABI.
#include "detail/D2DResourceSupport.h"
#include <atomic>
#include <cstdlib>
#include <cstring>

namespace {
std::atomic<void*> resourceConstructorVtable{nullptr};
// Captures the compiler's abstract-base vtable during base construction.
// These overrides exist only to instantiate the probe and must never be called.
struct ResourceVtableProbe : CD2DResource {
    ResourceVtableProbe() : CD2DResource(nullptr, FALSE) {}
    HRESULT Create(CRenderTarget*) override { std::abort(); }
    void Destroy() override { std::abort(); }
    BOOL IsValid() const override { std::abort(); }
};
}

CD2DResource::CD2DResource(CRenderTarget* parent, BOOL autoDestroy)
    : m_bIsAutoDestroy(autoDestroy), m_pParentTarget(parent) {
    resourceConstructorVtable.store(*reinterpret_cast<void**>(this), std::memory_order_relaxed);
    if (parent) OpenMfcRegisterD2DResource(parent, this);
}
CD2DResource::~CD2DResource() {
    if (m_pParentTarget) OpenMfcUnregisterD2DResource(m_pParentTarget, this);
}
HRESULT CD2DResource::ReCreate(CRenderTarget* target) {
    // Retail RVA 0xd28f0: release the old resource before Create, and return
    // Create's HRESULT unchanged. Ownership stays with the original parent.
    Destroy();
    return Create(target);
}
void* CD2DResource::ConstructorVtable() {
    static void* const table = [] {
        ResourceVtableProbe probe;
        return resourceConstructorVtable.load(std::memory_order_relaxed);
    }();
    return table;
}

// Symbol: ??0CD2DResource@@IEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void* self, void* parent, int autoDestroy) {
    if (!self) return nullptr;
    // The class is abstract, so initialize caller-provided base storage using
    // the compiler-emitted abstract vtable rather than constructing a proxy.
    void* table = CD2DResource::ConstructorVtable();
    std::memcpy(self, &table, sizeof(table));
    auto* resource = static_cast<CD2DResource*>(self);
    resource->m_bIsAutoDestroy = autoDestroy;
    resource->m_pParentTarget = static_cast<CRenderTarget*>(parent);
    if (parent) OpenMfcRegisterD2DResource(static_cast<CRenderTarget*>(parent), resource);
    return self;
}
// Symbol: ??1CD2DResource@@MEAA@XZ
extern "C" void* MS_ABI impl___1CD2DResource__MEAA_XZ(void* self) {
    if (self) static_cast<CD2DResource*>(self)->CD2DResource::~CD2DResource();
    return self;
}
// Symbol: ?ReCreate@CD2DResource@@MEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(void* self, void* target) {
    return self ? static_cast<CD2DResource*>(self)->CD2DResource::ReCreate(static_cast<CRenderTarget*>(target)) : E_POINTER;
}
// Symbol: ?VerifyResource@CRenderTarget@@IEAAHPEAVCD2DResource@@@Z
extern "C" int MS_ABI impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(void* self, void* wrapper) {
    if (!wrapper) return FALSE;
    auto* resource = static_cast<CD2DResource*>(wrapper);
    // Retail RVA 0xd5540: IsValid, then Create only when invalid. Verification
    // never destroys or reparents an already valid resource.
    return resource->IsValid() || SUCCEEDED(resource->Create(static_cast<CRenderTarget*>(self)));
}
