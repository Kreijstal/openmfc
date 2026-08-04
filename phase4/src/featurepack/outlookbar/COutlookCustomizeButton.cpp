// COutlookCustomizeButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?CreateObject@COutlookCustomizeButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COutlookCustomizeButton__SAPEAVCObject__XZ() {
    return new (std::nothrow) COutlookCustomizeButtonObject();
}
// Symbol: ?CreatePopupMenu@COutlookCustomizeButton@@UEAAPEAVCMFCPopupMenu@@XZ
extern "C" void* MS_ABI impl__CreatePopupMenu_COutlookCustomizeButton__UEAAPEAVCMFCPopupMenu__XZ(
    void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCOutlookCustomizeButtonState(pThis);
    if (state.m_popupMenu == nullptr) {
        state.m_popupMenu = new (std::nothrow) CMFCPopupMenu();
    }
    return state.m_popupMenu;
}
// Symbol: ?OnDraw@COutlookCustomizeButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_COutlookCustomizeButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* rect, void* pImages,
    int x, int y, int cx, int cy, int flags) {
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    if (pThis != nullptr) {
        auto& state = EnsureCOutlookCustomizeButtonState(pThis);
        state.m_didDraw = true;
    }

    const RECT* drawRect = static_cast<const RECT*>(rect);
    RECT localRect{};
    if (drawRect != nullptr) {
        localRect = *drawRect;
    } else {
        localRect = {x, y, x + cx, y + cy};
    }

    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    if (brush != nullptr) {
        ::FillRect(pDeviceContext->GetSafeHdc(), &localRect, brush);
        ::DeleteObject(brush);
    }

    if (!::IsRectEmpty(&localRect)) {
        ::DrawEdge(pDeviceContext->GetSafeHdc(), &localRect, BDR_RAISEDINNER, BF_RECT);
    }
    (void)pImages;
    (void)flags;
}
// Symbol: ?GetRuntimeClass@COutlookCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_COutlookCustomizeButton__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@COutlookCustomizeButton@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_COutlookCustomizeButton__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
