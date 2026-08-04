// CMFCToolBarColorButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?CreateObject@CMFCToolBarColorButton@@SAPEAVCObject@@XZ
// Symbol: ?OnDraw@CMFCToolBarColorButton@@MEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarColorButton__MEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* pRect, void* pImages,
    int nColor, int nStyle, int, int, int) {
    (void)pThis;
    (void)nColor;
    (void)nStyle;
    (void)pImages;

    auto* pDCObj = static_cast<CDC*>(pDC);
    if (pDCObj == nullptr || pDCObj->m_hDC == nullptr || pRect == nullptr) {
        return;
    }

    const RECT* rect = static_cast<const RECT*>(pRect);
    HBRUSH brush = ::GetSysColorBrush(COLOR_BTNFACE);
    ::FillRect(pDCObj->m_hDC, rect, brush);
}
