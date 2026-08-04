// CDCRenderTarget — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(CRenderTarget* pThis, void* pRenderTarget);
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(CRenderTarget* pThis);
extern "C" CRenderTarget* MS_ABI impl___0CRenderTarget__QEAA_XZ(CRenderTarget* pThis);

// Symbol: ?GetRuntimeClass@CDCRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDCRenderTarget__UEBAPEAUCRuntimeClass__XZ(const CDCRenderTarget* pThis) {
    (void)pThis;
    return CDCRenderTarget::GetThisClass();
}
// Symbol: ?GetThisClass@CDCRenderTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDCRenderTarget__SAPEAUCRuntimeClass__XZ() {
    return CDCRenderTarget::GetThisClass();
}
// Symbol: ??0CDCRenderTarget@@QEAA@XZ
extern "C" CDCRenderTarget* MS_ABI impl___0CDCRenderTarget__QEAA_XZ(CDCRenderTarget* pThis) {
    if (!pThis) return nullptr;
    impl___0CRenderTarget__QEAA_XZ(pThis);
    return pThis;
}
// Symbol: ?Attach@CDCRenderTarget@@QEAAXPEAUID2D1DCRenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CDCRenderTarget__QEAAXPEAUID2D1DCRenderTarget___Z(CDCRenderTarget* pThis, void* pRenderTarget) {
    impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(pThis, pRenderTarget);
}
// Symbol: ?Detach@CDCRenderTarget@@QEAAPEAUID2D1DCRenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CDCRenderTarget__QEAAPEAUID2D1DCRenderTarget__XZ(CDCRenderTarget* pThis) {
    return impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(pThis);
}
// Symbol: ?Create@CDCRenderTarget@@QEAAHAEBUD2D1_RENDER_TARGET_PROPERTIES@@@Z
extern "C" int MS_ABI impl__Create_CDCRenderTarget__QEAAHAEBUD2D1_RENDER_TARGET_PROPERTIES___Z(
    CDCRenderTarget* pThis, const void* pRenderTargetProperties) {
    if (!pThis || !pRenderTargetProperties) return FALSE;
    using CreateFactoryFn = HRESULT (WINAPI*)(D2D1_FACTORY_TYPE, REFIID, const D2D1_FACTORY_OPTIONS*, void**);
    static HMODULE module = ::LoadLibraryW(L"d2d1.dll");
    static CreateFactoryFn createFactory = module
        ? reinterpret_cast<CreateFactoryFn>(::GetProcAddress(module, "D2D1CreateFactory")) : nullptr;
    if (!createFactory) return FALSE;

    ID2D1Factory* factory = nullptr;
    HRESULT hr = createFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), nullptr,
                               reinterpret_cast<void**>(&factory));
    if (FAILED(hr) || !factory) return FALSE;
    ID2D1DCRenderTarget* target = nullptr;
    hr = factory->CreateDCRenderTarget(
        static_cast<const D2D1_RENDER_TARGET_PROPERTIES*>(pRenderTargetProperties), &target);
    factory->Release();
    if (FAILED(hr) || !target) return FALSE;
    impl__Attach_CDCRenderTarget__QEAAXPEAUID2D1DCRenderTarget___Z(pThis, target);
    return TRUE;
}
// Symbol: ?BindDC@CDCRenderTarget@@QEAAHAEBVCDC@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__BindDC_CDCRenderTarget__QEAAHAEBVCDC__AEBVCRect___Z(
    CDCRenderTarget* pThis, const CDC* pDC, const CRect* pRect) {
    if (!pThis || !pDC || !pDC->GetSafeHdc() || !pRect) return FALSE;
    ID2D1DCRenderTarget* target = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_wave2StateMutex);
        auto& state = g_renderTargetState[pThis];
        target = static_cast<ID2D1DCRenderTarget*>(state.resource);
        state.size = CD2DSizeF((float)(pRect->right - pRect->left), (float)(pRect->bottom - pRect->top));
    }
    if (!target) return FALSE;
    RECT rect = {pRect->left, pRect->top, pRect->right, pRect->bottom};
    return SUCCEEDED(target->BindDC(pDC->GetSafeHdc(), &rect));
}
CDCRenderTarget::CDCRenderTarget() {
    impl___0CDCRenderTarget__QEAA_XZ(this);
}
void CDCRenderTarget::Attach(void* pRenderTarget) {
    impl__Attach_CDCRenderTarget__QEAAXPEAUID2D1DCRenderTarget___Z(this, pRenderTarget);
}
void* CDCRenderTarget::Detach() {
    return impl__Detach_CDCRenderTarget__QEAAPEAUID2D1DCRenderTarget__XZ(this);
}
int CDCRenderTarget::Create(const void* pRenderTargetProperties) {
    return impl__Create_CDCRenderTarget__QEAAHAEBUD2D1_RENDER_TARGET_PROPERTIES___Z(this, pRenderTargetProperties);
}
int CDCRenderTarget::BindDC(const CDC& dc, const CRect& rect) {
    return impl__BindDC_CDCRenderTarget__QEAAHAEBVCDC__AEBVCRect___Z(this, &dc, &rect);
}
