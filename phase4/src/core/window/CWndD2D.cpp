// CWnd Direct2D selection, registered paint messages and target ownership.
#include "detail/D2DResourceSupport.h"
#include <memory>
#include <mutex>
#include <unordered_map>

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned int impl__AFX_WM_DRAW2D__3IA;
extern "C" unsigned int impl__AFX_WM_RECREATED2DRESOURCES__3IA;

static_assert(sizeof(CWnd) == 232, "retail CWnd size");
static_assert(offsetof(CWnd, m_pRenderTarget) == 224, "retail CWnd render-target offset");
namespace {
struct TargetOwners {
    std::mutex mutex;
    std::unordered_map<CWnd*, std::shared_ptr<CRenderTarget>> targets;
};
TargetOwners& Owners() { static TargetOwners owners; return owners; }
std::shared_ptr<CRenderTarget> PaintTarget(CWnd* window) {
    auto& owners = Owners();
    std::shared_ptr<CRenderTarget> retired, retained;
    {
        std::lock_guard<std::mutex> lock(owners.mutex);
        auto& target = owners.targets[window];
        if (target.get() != window->m_pRenderTarget) {
            // Client subclasses may install a heap target in the protected
            // MFC member. Adopt that ownership before invoking callbacks.
            std::shared_ptr<CRenderTarget> replacement(window->m_pRenderTarget);
            retired = std::move(target);
            target = std::move(replacement);
        }
        retained = target;
    }
    return retained;
}
struct PaintDC {
    HWND window;
    PAINTSTRUCT paint{};
    HDC dc;
    explicit PaintDC(HWND hwnd) : window(hwnd), dc(BeginPaint(hwnd, &paint)) {}
    ~PaintDC() { if (dc) EndPaint(window, &paint); }
};
}
void CWnd::EnableD2DSupport(int enable, int useDC) {
    if (!enable) {
        std::shared_ptr<CRenderTarget> owned;
        auto* raw = m_pRenderTarget;
        m_pRenderTarget = nullptr;
        auto& owners = Owners();
        {
            std::lock_guard<std::mutex> lock(owners.mutex);
            auto it = owners.targets.find(this);
            if (it != owners.targets.end()) { owned = std::move(it->second); owners.targets.erase(it); }
        }
        // Release resources outside the registry lock. An active paint retains
        // its target until EndDraw even if a callback disables support.
        if (!owned || owned.get() != raw) delete raw;
        return;
    }
    if (m_pRenderTarget || !OpenMfcD2DState().InitD2D()) return;
    std::shared_ptr<CRenderTarget> target = useDC
        ? std::shared_ptr<CRenderTarget>(new CDCRenderTarget())
        : std::shared_ptr<CRenderTarget>(new CHwndRenderTarget(m_hWnd));
    auto& owners = Owners();
    {
        std::lock_guard<std::mutex> lock(owners.mutex);
        owners.targets[this] = target;
        m_pRenderTarget = target.get();
    }
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
    auto retained = PaintTarget(this);
    if (auto* target = GetRenderTarget()) target->Resize(D2D1_SIZE_U{width, height});
}
int CWnd::DoD2DPaint() {
    const HWND hwnd = m_hWnd;
    if (!hwnd || !IsWindow(hwnd) || !m_pRenderTarget) return FALSE;
    auto retained = PaintTarget(this);
    auto* target = retained.get();
    if (target->IsKindOf(impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ())) {
        auto* windowTarget = static_cast<CHwndRenderTarget*>(target);
        if (!windowTarget->IsValid() && !windowTarget->Create(hwnd)) return FALSE;
        windowTarget->BeginDraw();
        LRESULT handled = ::SendMessageW(hwnd, impl__AFX_WM_DRAW2D__3IA, 0, reinterpret_cast<LPARAM>(windowTarget));
        HRESULT result = windowTarget->EndDraw();
        if (result == D2DERR_RECREATE_TARGET) {
            windowTarget->ReCreate(hwnd);
            ::SendMessageW(hwnd, impl__AFX_WM_RECREATED2DRESOURCES__3IA, 0, reinterpret_cast<LPARAM>(windowTarget));
        }
        if (handled) ::ValidateRect(hwnd, nullptr);
        return handled != 0;
    }
    if (!target->IsKindOf(CDCRenderTarget::GetThisClass())) return FALSE;
    auto* dcTarget = static_cast<CDCRenderTarget*>(target);
    if (!dcTarget->IsValid()) {
        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
        if (!dcTarget->Create(&props)) return FALSE;
    }
    RECT client{};
    if (!::GetClientRect(hwnd, &client)) return FALSE;
    PaintDC paint(hwnd);
    if (!paint.dc || !dcTarget->GetDCRenderTarget() || FAILED(dcTarget->GetDCRenderTarget()->BindDC(paint.dc, &client))) return FALSE;
    dcTarget->BeginDraw();
    LRESULT handled = ::SendMessageW(hwnd, impl__AFX_WM_DRAW2D__3IA, 0, reinterpret_cast<LPARAM>(dcTarget));
    HRESULT result = dcTarget->EndDraw();
    if (result == D2DERR_RECREATE_TARGET)
        ::SendMessageW(hwnd, impl__AFX_WM_RECREATED2DRESOURCES__3IA, 0, reinterpret_cast<LPARAM>(dcTarget));
    return static_cast<int>(handled);
}
void CWnd::OnPaint() {
    const HWND hwnd = m_hWnd;
    if (m_pRenderTarget && DoD2DPaint()) return;
    if (!hwnd || !IsWindow(hwnd)) return;
    PaintDC paint(hwnd);
    if (paint.dc) ::FillRect(paint.dc, &paint.paint.rcPaint, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
}

// Symbol: ?EnableD2DSupport@CWnd@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableD2DSupport_CWnd__QEAAXHH_Z(CWnd* self, int enable, int useDC) { if (self) self->EnableD2DSupport(enable, useDC); }
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
