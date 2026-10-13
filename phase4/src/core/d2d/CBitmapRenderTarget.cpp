// CBitmapRenderTarget — native MSVC layout and Direct2D ownership transfers.
#define OPENMFC_APPCORE_IMPL
#include "detail/GdicoreSupport.h"

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBitmapRenderTarget__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(void*, void*);

CRuntimeClass* CBitmapRenderTarget::GetRuntimeClass() const {
    return impl__GetThisClass_CBitmapRenderTarget__SAPEAUCRuntimeClass__XZ();
}
void CBitmapRenderTarget::Attach(ID2D1BitmapRenderTarget* target) {
    if (m_pRenderTarget) return;
    m_pBitmapRenderTarget = target;
    CRenderTarget::Attach(target);
}
ID2D1BitmapRenderTarget* CBitmapRenderTarget::Detach() {
    auto* target = m_pBitmapRenderTarget;
    m_pBitmapRenderTarget = nullptr;
    CRenderTarget::Detach();
    return target;
}
int CBitmapRenderTarget::GetBitmap(CD2DBitmap& bitmap) {
    if (!m_pBitmapRenderTarget) return FALSE;
    // Retail mfc140u RVA 0xd6680 writes the returned COM interface at +0x18,
    // after the CD2DResource prefix. It never overwrites the wrapper's vfptr.
    auto** output = reinterpret_cast<ID2D1Bitmap**>(reinterpret_cast<unsigned char*>(&bitmap) + 0x18);
    HRESULT hr = m_pBitmapRenderTarget->GetBitmap(output);
    // Keep the legacy bitmap implementation's side state in sync as well.
    impl__Attach_CD2DBitmap__QEAAXPEAUID2D1Bitmap___Z(&bitmap, *output);
    return SUCCEEDED(hr);
}

// Symbol: ?GetBitmap@CBitmapRenderTarget@@QEAAHAEAVCD2DBitmap@@@Z
extern "C" int MS_ABI impl__GetBitmap_CBitmapRenderTarget__QEAAHAEAVCD2DBitmap___Z(void* self, void* bitmap) {
    return self && bitmap && static_cast<CBitmapRenderTarget*>(self)->GetBitmap(*static_cast<CD2DBitmap*>(bitmap));
}
// Symbol: ?Attach@CBitmapRenderTarget@@QEAAXPEAUID2D1BitmapRenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(void* self, void* target) {
    if (self) static_cast<CBitmapRenderTarget*>(self)->Attach(static_cast<ID2D1BitmapRenderTarget*>(target));
}
// Symbol: ?Detach@CBitmapRenderTarget@@QEAAPEAUID2D1BitmapRenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CBitmapRenderTarget__QEAAPEAUID2D1BitmapRenderTarget__XZ(void* self) {
    return self ? static_cast<CBitmapRenderTarget*>(self)->Detach() : nullptr;
}
