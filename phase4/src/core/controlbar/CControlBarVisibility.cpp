#define OPENMFC_APPCORE_IMPL
#include "detail/ClassicDockingSupport.h"
#include "openmfc/afxole.h"

using namespace openmfc::detail::dock;

namespace {
bool WindowStyleIsVisible(const CControlBar* bar) {
    return (::GetWindowLongW(bar->m_hWnd, GWL_STYLE) & WS_VISIBLE) != 0;
}
}

// Symbol: ?IsFloating@CControlBar@@QEBAHXZ
extern "C" BOOL impl__IsFloating_CControlBar__QEBAHXZ(const CControlBar* bar) {
    if (bar->IsDockBar())
        return IsFloatingBar(bar);
    return IsFloatingBar(bar->m_pDockBar);
}

// Symbol: ?IsVisible@CControlBar@@UEBAHXZ
extern "C" BOOL impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* bar) {
    return bar->CControlBar::IsVisible();
}

BOOL CControlBar::IsVisible() const {
    if (m_nStateFlags & delayHide) return FALSE;
    if (m_nStateFlags & delayShow) return TRUE;
    return WindowStyleIsVisible(this);
}

// Symbol: ?DelayShow@CControlBar@@UEAAXH@Z
extern "C" void impl__DelayShow_CControlBar__UEAAXH_Z(CControlBar* bar, BOOL show) {
    bar->CControlBar::DelayShow(show);
}

void CControlBar::DelayShow(BOOL show) {
    m_nStateFlags &= ~(delayHide | delayShow);
    if ((show != FALSE) != WindowStyleIsVisible(this))
        m_nStateFlags |= show ? delayShow : delayHide;
}
