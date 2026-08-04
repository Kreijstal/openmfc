// CMFCDesktopAlertWndButton — OpenMFC implementation.
// Sources: global_cmfcdesktopalertwndbutton.cpp

#include "detail/CMFCDesktopAlertWndButtonSupport.h"

// CMFCDesktopAlertWndButton keeps its background transparent: the owning alert window
// paints the caption gradient behind the button, so this override intentionally draws
// nothing (it exists only to suppress the CMFCButton face fill).
// Symbol: ?OnFillBackground@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI
impl__OnFillBackground_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* /*pThis*/, void* /*pDC*/, const RECT* /*rectClient*/)
{
    // Faithful empty override: the alert window owns the caption background.
}
// CMFCDesktopAlertWndButton is borderless chrome on the alert caption bar: this override
// suppresses the CMFCButton border draw and intentionally paints nothing in the resting
// state.
// Symbol: ?OnDrawBorder@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDrawBorder_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* /*pThis*/, void* /*pDC*/, RECT* /*rectClient*/, unsigned int /*uiState*/)
{
    // Faithful empty override: no border is drawn by the alert caption/close button.
}
// CMFCDesktopAlertWndButton::OnDraw paints the button glyph centred inside rect using the
// button-text system colour: an "X" for the close button, a down-arrow for the caption
// (menu) button. Real mfc140u draws this via CMenuImages; the private glyph sheet is not
// available here, so equivalent GDI primitives reproduce the same observable shape.
// Symbol: ?OnDraw@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDraw_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int /*uiState*/)
{
    if (pThis == nullptr || rect == nullptr) return;
    HDC hdc = hdc_of_Cmfcdesktopalertwndbutton(pDC);
    if (hdc == nullptr) return;

    const S_Cmfcdesktopalertwndbutton* p = reinterpret_cast<const S_Cmfcdesktopalertwndbutton*>(pThis);

    const LONG w  = rect->right - rect->left;
    const LONG h  = rect->bottom - rect->top;
    if (w <= 0 || h <= 0) return;

    const LONG cx = rect->left + w / 2;
    const LONG cy = rect->top  + h / 2;
    LONG ext = ((w < h) ? w : h) / 4;   // half-extent of the glyph box
    if (ext < 2) ext = 2;

    const COLORREF clr = GetSysColor(COLOR_BTNTEXT);

    if (p->m_bIsCloseButton) {
        // Draw an "X": two diagonals across the centred glyph box.
        HPEN   pen = CreatePen(PS_SOLID, 1, clr);
        HGDIOBJ old = SelectObject(hdc, pen);

        MoveToEx(hdc, cx - ext, cy - ext, nullptr);
        LineTo  (hdc, cx + ext + 1, cy + ext + 1);
        MoveToEx(hdc, cx + ext, cy - ext, nullptr);
        LineTo  (hdc, cx - ext - 1, cy + ext + 1);

        SelectObject(hdc, old);
        DeleteObject(pen);
    } else if (p->m_bIsCaptionButton) {
        // Draw a filled down-pointing arrow (the caption/menu button glyph).
        HBRUSH  br     = CreateSolidBrush(clr);
        HGDIOBJ oldBr  = SelectObject(hdc, br);
        HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(NULL_PEN));

        POINT pts[3] = {
            { cx - ext, cy - ext / 2 },
            { cx + ext, cy - ext / 2 },
            { cx,       cy + ext      }
        };
        Polygon(hdc, pts, 3);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBr);
        DeleteObject(br);
    }
    // Neither flag set: nothing to draw (matches an unconfigured button).
}
