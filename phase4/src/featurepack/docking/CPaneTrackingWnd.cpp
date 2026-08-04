// CPaneTrackingWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?OnEraseBkgnd@CPaneTrackingWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPaneTrackingWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    auto* pCDC = static_cast<CDC*>(pDC);
    if (pWnd == nullptr || pCDC == nullptr || pCDC->m_hDC == nullptr) {
        return FALSE;
    }

    if (!pWnd->m_hWnd) {
        return FALSE;
    }

    RECT clientRect = {};
    ::GetClientRect(pWnd->m_hWnd, &clientRect);
    HBRUSH brush = ::GetSysColorBrush(COLOR_3DFACE);
    ::FillRect(pCDC->m_hDC, &clientRect, brush);
    return TRUE;
}
// Symbol: ?OnPaint@CPaneTrackingWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CPaneTrackingWnd__IEAAXXZ(void* pThis) {
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        pWnd->OnPaint();
    }
}
