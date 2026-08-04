// CMFCPrintPreviewToolBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?OnContextMenu@CMFCPrintPreviewToolBar@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCPrintPreviewToolBar__IEAAXPEAVCWnd__VCPoint___Z(
    void* pThis, void* pWnd, const void* point) {
    if (pWnd == nullptr) {
        return;
    }
    const POINT* screenPoint = static_cast<const POINT*>(point);
    if (pThis != nullptr) {
        auto& state = EnsureCMFCPrintPreviewToolBarState(pThis);
        state.m_destroyed = false;
        if (screenPoint != nullptr) {
            state.m_lastX = screenPoint->x;
            state.m_lastY = screenPoint->y;
        }
    }
    if (screenPoint != nullptr) {
        CWnd* owner = static_cast<CWnd*>(pWnd);
        if (owner != nullptr) {
            POINT menuPoint = *screenPoint;
            if (owner->GetSafeHwnd()) {
                ::ClientToScreen(owner->GetSafeHwnd(), &menuPoint);
            }
            owner->SendMessageW(WM_CONTEXTMENU, (WPARAM)0, MAKELPARAM(menuPoint.x, menuPoint.y));
        }
    }
}
// Symbol: ?OnDestroy@CMFCPrintPreviewToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCPrintPreviewToolBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    ClearCMFCPrintPreviewToolBarState(pThis);
}
// Symbol: ?OnToolHitTest@CMFCPrintPreviewToolBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" long long MS_ABI impl__OnToolHitTest_CMFCPrintPreviewToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const void* pThis, const void* point, void* pInfo) {
    if (pThis == nullptr || point == nullptr || pInfo == nullptr) {
        return -1;
    }

    const POINT* cursor = static_cast<const POINT*>(point);
    auto& state = EnsureCMFCPrintPreviewToolBarState(const_cast<void*>(pThis));
    if (state.m_destroyed) {
        return -1;
    }
    state.m_lastToolHit = 0;
    state.m_lastX = cursor->x;
    state.m_lastY = cursor->y;

    tagTOOLINFOW* info = static_cast<tagTOOLINFOW*>(pInfo);
    info->cbSize = sizeof(tagTOOLINFOW);
    info->rect.left = cursor->x;
    info->rect.top = cursor->y;
    info->rect.right = cursor->x;
    info->rect.bottom = cursor->y;
    return 0;
}
