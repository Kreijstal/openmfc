// CMFCOutlookBarScrollButton — OpenMFC implementation.
// Sources: global_cmfcoutlookbarscrollbutton.cpp

#include "detail/CMFCOutlookBarScrollButtonSupport.h"

// Symbol: ?OnFillBackground@CMFCOutlookBarScrollButton@@EEAAXPEAVCDC@@AEBVCRect@@@Z
// virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);
// Fills the scroll-button interior. The framework's default fill for an Outlook
// scroll button paints the button face colour across the client rectangle.
extern "C" void MS_ABI
impl__OnFillBackground_CMFCOutlookBarScrollButton__EEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient)
{
    (void)pThis;
    HDC hdc = DcHandle_Cmfcoutlookbarscrollbutton(pDC);
    if (hdc == nullptr || rectClient == nullptr) {
        return;
    }
    // Paint the client area with the standard 3D button-face colour.
    HBRUSH hbr = ::GetSysColorBrush(COLOR_BTNFACE);
    if (hbr != nullptr) {
        ::FillRect(hdc, rectClient, hbr);
    }
}
// Symbol: ?OnDrawBorder@CMFCOutlookBarScrollButton@@EEAAXPEAVCDC@@AEAVCRect@@I@Z
// virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
// Draws the scroll button's raised (or, when pressed, sunken) border and then
// deflates rectClient by the consumed border so the caller draws content inside.
extern "C" void MS_ABI
impl__OnDrawBorder_CMFCOutlookBarScrollButton__EEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, UINT uiState)
{
    (void)pThis;
    HDC hdc = DcHandle_Cmfcoutlookbarscrollbutton(pDC);
    if (hdc == nullptr || rectClient == nullptr) {
        return;
    }
    // A pressed button gets a sunken edge; otherwise a raised edge. DrawEdge with
    // BF_ADJUST shrinks rectClient in place by the space the border consumed.
    UINT edge = (uiState & SB_STATE_PRESSED) ? BDR_SUNKENOUTER : BDR_RAISEDINNER;
    ::DrawEdge(hdc, rectClient, edge, BF_RECT | BF_ADJUST);
}
