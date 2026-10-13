// CHwndRenderTarget: retail 88-byte layout with two independent interface slots.
#include "detail/GdicoreSupport.h"
#include "detail/D2DFactorySupport.h"

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ();

CHwndRenderTarget::CHwndRenderTarget(HWND hwnd) { if (hwnd) Create(hwnd); }
CRuntimeClass* CHwndRenderTarget::GetRuntimeClass() const { return impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ(); }
void CHwndRenderTarget::Attach(ID2D1HwndRenderTarget* target) {
    if (m_pRenderTarget) return;
    m_pHwndRenderTarget = target;
    CRenderTarget::Attach(target);
}
ID2D1HwndRenderTarget* CHwndRenderTarget::Detach() {
    auto* result = m_pHwndRenderTarget;
    m_pHwndRenderTarget = nullptr;
    CRenderTarget::Detach();
    return result;
}
int CHwndRenderTarget::Create(HWND hwnd) {
    if (!hwnd || m_pRenderTarget) return FALSE;
    ID2D1Factory* factory = OpenMfcD2DState().GetDirect2dFactory();
    if (!factory) return FALSE;
    RECT rect{};
    GetClientRect(hwnd, &rect);
    D2D1_RENDER_TARGET_PROPERTIES properties{};
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProperties = {
        hwnd, {static_cast<UINT32>(rect.right - rect.left), static_cast<UINT32>(rect.bottom - rect.top)},
        D2D1_PRESENT_OPTIONS_NONE};
    HRESULT hr = factory->CreateHwndRenderTarget(&properties, &hwndProperties, &m_pHwndRenderTarget);
    if (FAILED(hr)) return FALSE;
    CRenderTarget::Attach(m_pHwndRenderTarget);
    return TRUE;
}
int CHwndRenderTarget::ReCreate(HWND hwnd) {
    if (!m_pHwndRenderTarget) return FALSE;
    m_pHwndRenderTarget->Release();
    m_pHwndRenderTarget = nullptr;
    CRenderTarget::Detach();
    if (!Create(hwnd)) return FALSE;
    ReCreateResources();
    return TRUE;
}
int CHwndRenderTarget::Resize(const D2D_SIZE_U& size) {
    return m_pHwndRenderTarget && SUCCEEDED(m_pHwndRenderTarget->Resize(&size));
}
HWND CHwndRenderTarget::GetHwnd() const { return m_pHwndRenderTarget ? m_pHwndRenderTarget->GetHwnd() : nullptr; }

// Symbol: ??0CHwndRenderTarget@@QEAA@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl___0CHwndRenderTarget__QEAA_PEAUHWND_____Z(void* self, void* hwnd) {
    return self ? new(self) CHwndRenderTarget(static_cast<HWND>(hwnd)) : nullptr;
}
// Symbol: ?Attach@CHwndRenderTarget@@QEAAXPEAUID2D1HwndRenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CHwndRenderTarget__QEAAXPEAUID2D1HwndRenderTarget___Z(void* self, ID2D1HwndRenderTarget* target) {
    if (self) static_cast<CHwndRenderTarget*>(self)->Attach(target);
}
// Symbol: ?Create@CHwndRenderTarget@@QEAAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__Create_CHwndRenderTarget__QEAAHPEAUHWND_____Z(void* self, HWND hwnd) {
    return self && static_cast<CHwndRenderTarget*>(self)->Create(hwnd);
}
// Symbol: ?Detach@CHwndRenderTarget@@QEAAPEAUID2D1HwndRenderTarget@@XZ
extern "C" ID2D1HwndRenderTarget* MS_ABI impl__Detach_CHwndRenderTarget__QEAAPEAUID2D1HwndRenderTarget__XZ(void* self) {
    return self ? static_cast<CHwndRenderTarget*>(self)->Detach() : nullptr;
}
// Symbol: ?ReCreate@CHwndRenderTarget@@QEAAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__ReCreate_CHwndRenderTarget__QEAAHPEAUHWND_____Z(void* self, HWND hwnd) {
    return self && static_cast<CHwndRenderTarget*>(self)->ReCreate(hwnd);
}
// Symbol: ?Resize@CHwndRenderTarget@@QEAAHAEBVCD2DSizeU@@@Z
extern "C" int MS_ABI impl__Resize_CHwndRenderTarget__QEAAHAEBVCD2DSizeU___Z(void* self, const D2D1_SIZE_U* size) {
    return self && size && static_cast<CHwndRenderTarget*>(self)->Resize(*size);
}
