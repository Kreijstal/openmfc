// Release x64 menu tracking and base menu-visibility notifications.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"

int CFrameWnd::IsTracking() {
    const UINT id = GetTrackingID();
    return (id - 1u <= 0xdfffu || id >= 0xe003u) ? TRUE : FALSE;
}

// Symbol: ?IsTracking@CFrameWnd@@QEAAHXZ
extern "C" BOOL MS_ABI impl__IsTracking_CFrameWnd__QEAAHXZ(CFrameWnd* self) {
    return self->IsTracking();
}

static_assert(offsetof(CFrameWnd, m_nIDTracking) == 0x168 && sizeof(CFrameWnd) == 0x1d8,
              "native frame tracking storage");

// Retail base visibility notifications are empty overridable callbacks.
void CFrameWnd::OnShowMenuBar() {}
void CFrameWnd::OnHideMenuBar() {}
// Symbol: ?OnShowMenuBar@CFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowMenuBar_CFrameWnd__MEAAXXZ(CFrameWnd* self) {
    self->CFrameWnd::OnShowMenuBar();
}
// Symbol: ?OnHideMenuBar@CFrameWnd@@MEAAXXZ
extern "C" void MS_ABI impl__OnHideMenuBar_CFrameWnd__MEAAXXZ(CFrameWnd* self) {
    self->CFrameWnd::OnHideMenuBar();
}
