// CSplitterWndEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CSplitterWndEx@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSplitterWndEx__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CSplitterWndEx@@UEAA@XZ
extern "C" void MS_ABI impl___1CSplitterWndEx__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
}
// Symbol: ?OnDrawSplitter@CSplitterWndEx@@UEAAXPEAVCDC@@W4ESplitType@CSplitterWnd@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitter_CSplitterWndEx__UEAAXPEAVCDC__W4ESplitType_CSplitterWnd__AEBVCRect___Z(
    void* pThis, void* pDC, int splitType, unsigned char draw,
    const void* pRect) {
    (void)pThis;
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    if (pRect == nullptr) {
        return;
    }
    const RECT& splitRect = *static_cast<const RECT*>(pRect);
    if (IsRectEmpty(&splitRect)) {
        return;
    }

    const COLORREF color = (draw != 0) ? COLOR_3DSHADOW : COLOR_3DFACE;
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(color));
    if (brush == nullptr) {
        return;
    }
    RECT rc = splitRect;
    if (splitType == 0) {
        rc.right = rc.left + 2;
    } else {
        rc.bottom = rc.top + 2;
    }
    ::FillRect(pDeviceContext->GetSafeHdc(), &rc, brush);
    ::DeleteObject(brush);
}
