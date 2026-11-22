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
    if (bar->IsKindOf(impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ()))
        return IsFloatingBar(bar);
    return IsFloatingBar(bar->m_pDockBar);
}

// Symbol: ?IsVisible@CControlBar@@UEBAHXZ
extern "C" BOOL impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* bar) {
    if (bar->m_nStateFlags & delayHide) return FALSE;
    if (bar->m_nStateFlags & delayShow) return TRUE;
    return WindowStyleIsVisible(bar);
}

// Symbol: ?DelayShow@CControlBar@@UEAAXH@Z
extern "C" void impl__DelayShow_CControlBar__UEAAXH_Z(CControlBar* bar, BOOL show) {
    bar->m_nStateFlags &= ~(delayHide | delayShow);
    if ((show != FALSE) != WindowStyleIsVisible(bar))
        bar->m_nStateFlags |= show ? delayShow : delayHide;
}
