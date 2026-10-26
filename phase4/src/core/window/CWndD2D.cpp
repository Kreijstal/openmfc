// CWnd Direct2D selection, registered paint messages and target ownership.
#include "detail/D2DResourceSupport.h"
#include <cstdlib>

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned int impl__AFX_WM_DRAW2D__3IA;
extern "C" unsigned int impl__AFX_WM_RECREATED2DRESOURCES__3IA;

static_assert(sizeof(CHwndRenderTarget)==88 && sizeof(CDCRenderTarget)==88,"native D2D wrapper storage");
static_assert(sizeof(CWnd) == 232, "retail CWnd size");
static_assert(offsetof(CWnd, m_pRenderTarget) == 224, "retail CWnd render-target offset");
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
void CWnd::EnableD2DSupport(int enable, int useDC) {
    if (!enable) {
        if (m_pRenderTarget) {
            using Delete = void* (MS_ABI*)(void*,unsigned);
            auto callback = reinterpret_cast<Delete>((*reinterpret_cast<void***>(m_pRenderTarget))[1]);
            callback(m_pRenderTarget,1);
            m_pRenderTarget = nullptr;
        }
        return;
    }
    if (!OpenMfcD2DState().InitD2D() || m_pRenderTarget) return;
    auto* storage = impl___2_YAPEAX_K_Z(sizeof(CHwndRenderTarget));
    CRenderTarget* target = nullptr;
    if (storage) {
        try {
            target = useDC ? static_cast<CRenderTarget*>(::new(storage) CDCRenderTarget)
                           : static_cast<CRenderTarget*>(::new(storage) CHwndRenderTarget(m_hWnd));
        } catch (...) { std::free(storage);throw; }
    }
    m_pRenderTarget = target;
}
int CWnd::IsD2DSupportEnabled() { return m_pRenderTarget != nullptr; }
CHwndRenderTarget* CWnd::GetRenderTarget() {
    return m_pRenderTarget && m_pRenderTarget->IsKindOf(impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ())
        ? static_cast<CHwndRenderTarget*>(m_pRenderTarget) : nullptr;
}
CDCRenderTarget* CWnd::GetDCRenderTarget() {
    return m_pRenderTarget && m_pRenderTarget->IsKindOf(CDCRenderTarget::GetThisClass())
        ? static_cast<CDCRenderTarget*>(m_pRenderTarget) : nullptr;
}
void CWnd::ResizeD2DRenderTarget(UINT width, UINT height) {
    if (auto* target = GetRenderTarget()) target->Resize(D2D1_SIZE_U{width, height});
}
int CWnd::DoD2DPaint() {
    if (!m_pRenderTarget) return FALSE;
    auto* target = m_pRenderTarget;
    if (target->IsKindOf(impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ())) {
        auto* windowTarget = static_cast<CHwndRenderTarget*>(target);
        if (!windowTarget->IsValid()) windowTarget->Create(m_hWnd);
        if (!windowTarget->IsValid()) return FALSE;
        windowTarget->BeginDraw();
        LRESULT handled = ::SendMessageW(m_hWnd, impl__AFX_WM_DRAW2D__3IA, 0, reinterpret_cast<LPARAM>(windowTarget));
        HRESULT result = windowTarget->EndDraw();
        if (result == D2DERR_RECREATE_TARGET) {
            windowTarget->ReCreate(m_hWnd);
            ::SendMessageW(m_hWnd, impl__AFX_WM_RECREATED2DRESOURCES__3IA, 0, reinterpret_cast<LPARAM>(windowTarget));
        }
        const int resultCode = static_cast<int>(handled);
        if (resultCode) ::ValidateRect(m_hWnd, nullptr);
        return resultCode != 0;
    }
    if (!target->IsKindOf(CDCRenderTarget::GetThisClass())) return FALSE;
    auto* dcTarget = static_cast<CDCRenderTarget*>(target);
    if (!dcTarget->IsValid()) {
        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
        dcTarget->Create(&props);
    }
    if (!dcTarget->IsValid()) return FALSE;
    CDC dc;
    RECT client{};
    ::GetClientRect(m_hWnd, &client);
    PAINTSTRUCT paint;
    dc.Attach(::BeginPaint(m_hWnd, &paint));
    if (dcTarget->GetDCRenderTarget() && dc.GetSafeHdc())
        dcTarget->GetDCRenderTarget()->BindDC(dc.GetSafeHdc(), &client);
    dcTarget->BeginDraw();
    int handled = static_cast<int>(::SendMessageW(m_hWnd, impl__AFX_WM_DRAW2D__3IA, 0, reinterpret_cast<LPARAM>(dcTarget)));
    HRESULT result = dcTarget->EndDraw();
    if (result == D2DERR_RECREATE_TARGET)
        ::SendMessageW(m_hWnd, impl__AFX_WM_RECREATED2DRESOURCES__3IA, 0, reinterpret_cast<LPARAM>(dcTarget));
    dc.Detach();
    ::EndPaint(m_hWnd, &paint);
    return handled;
}
void CWnd::OnPaint() {
    if (m_pCtrlCont) {
        CPaintDC dc(this);
        using Paint = void (MS_ABI*)(void*,CDC*);
        auto callback = reinterpret_cast<Paint>((*reinterpret_cast<void***>(m_pCtrlCont))[40]);
        callback(m_pCtrlCont,&dc);
    }
    Default();
}

// Symbol: ?EnableD2DSupport@CWnd@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableD2DSupport_CWnd__QEAAXHH_Z(CWnd* self, int enable, int useDC) { self->EnableD2DSupport(enable, useDC); }
// Symbol: ?IsD2DSupportEnabled@CWnd@@QEAAHXZ
extern "C" int MS_ABI impl__IsD2DSupportEnabled_CWnd__QEAAHXZ(CWnd* self) { return self && self->IsD2DSupportEnabled(); }
// Symbol: ?GetRenderTarget@CWnd@@QEAAPEAVCHwndRenderTarget@@XZ
extern "C" CHwndRenderTarget* MS_ABI impl__GetRenderTarget_CWnd__QEAAPEAVCHwndRenderTarget__XZ(CWnd* self) { return self ? self->GetRenderTarget() : nullptr; }
// Symbol: ?GetDCRenderTarget@CWnd@@QEAAPEAVCDCRenderTarget@@XZ
extern "C" CDCRenderTarget* MS_ABI impl__GetDCRenderTarget_CWnd__QEAAPEAVCDCRenderTarget__XZ(CWnd* self) { return self ? self->GetDCRenderTarget() : nullptr; }
// Symbol: ?DoD2DPaint@CWnd@@IEAAHXZ
extern "C" int MS_ABI impl__DoD2DPaint_CWnd__IEAAHXZ(CWnd* self) { return self && self->DoD2DPaint(); }
// Symbol: ?OnPaint@CWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CWnd__IEAAXXZ(CWnd* self) { if (self) self->OnPaint(); }
