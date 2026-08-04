// CMFCColorPickerCtrl — OpenMFC implementation.
// Sources: global_cmfccolorpickerctrl.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCColorPickerCtrlSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Retail (0x18002c1d0): guards the +0x140 cell count, measures the client
// rect and fills the +0x138 CPtrArray with 256 grey-scale hexagon cells.  The
// cell array is unmodeled.
// Symbol: ?CreateHexGreyScaleBar@CMFCColorPickerCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__CreateHexGreyScaleBar_CMFCColorPickerCtrl__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x138/+0x140 cell array
    // and the geometry; not modeled.
}
// Retail (0x18002c630): guards the +0x140 cell count, measures the client
// rect and fills the +0x138 CPtrArray with the colour hexagon cells.  The
// cell array is unmodeled.
// Symbol: ?CreateHexagon@CMFCColorPickerCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__CreateHexagon_CMFCColorPickerCtrl__IEAAXXZ(
    void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x138/+0x140 cell array
    // and the geometry; not modeled.
}
// Retail (0x18002d0f0): draws the cursor on the picker DC (type-dependent
// shape).  The drawing state is unmodeled.
// Symbol: ?DrawCursor@CMFCColorPickerCtrl@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawCursor_CMFCColorPickerCtrl__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, CDC* /*pDC*/, const CRect& /*rect*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0xe8 type state and the
    // geometry; not modeled.
}
// Retail (0x18002c8c0): draws the colour hexagon from the +0x138 cell array.
// The cell array is unmodeled.
// Symbol: ?DrawHex@CMFCColorPickerCtrl@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawHex_CMFCColorPickerCtrl__IEAAXPEAVCDC___Z(
    void* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x138 cell array; not
    // modeled.
}
// Retail (0x18002dce0): draws the whole picker (luminance bar, hexagon,
// cursor) into the DRAWITEMSTRUCT's DC.  The drawing state is unmodeled.
// Symbol: ?DrawItem@CMFCColorPickerCtrl@@MEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CMFCColorPickerCtrl__MEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* /*lpDrawItemStruct*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the picker drawing state; not
    // modeled.
}
// Retail (0x18002d000): draws the luminance bar from the +0xf0/+0xf8/+0x100
// HLS state.  The drawing helpers are unmodeled.
// Symbol: ?DrawLuminanceBar@CMFCColorPickerCtrl@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawLuminanceBar_CMFCColorPickerCtrl__IEAAXPEAVCDC___Z(
    void* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the drawing helpers and the
    // HLS state; not modeled.
}
// Retail (0x18002cd20): draws the color plane for the +0xe8 type.  The
// drawing state is unmodeled.
// Symbol: ?DrawPicker@CMFCColorPickerCtrl@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawPicker_CMFCColorPickerCtrl__IEAAXPEAVCDC___Z(
    void* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0xe8 type state; not
    // modeled.
}
// Retail (0x18002c190): returns the hexagon angle (0..6) for a client point.
// The geometry is unmodeled.
// Symbol: ?GetAngleFromPoint@CMFCColorPickerCtrl@@IEAAHHH@Z
extern "C" int MS_ABI impl__GetAngleFromPoint_CMFCColorPickerCtrl__IEAAHHH_Z(
    void* pThis, int /*x*/, int /*y*/)
{
    if (!pThis) return 0;
    // TODO(clean-room): not transcribed -- needs the hexagon geometry; not
    // modeled.
    return 0;
}
// Retail (0x18002e360): measures the client rect and computes the cursor point
// from the +0xe8 type and the HLS doubles.  The geometry is unmodeled, so the
// retail empty-point terminal is returned.
// Symbol: ?GetCursorPos@CMFCColorPickerCtrl@@IEAA?AVCPoint@@XZ
extern "C" void MS_ABI impl__GetCursorPos_CMFCColorPickerCtrl__IEAA_AVCPoint__XZ(
    CPoint* pRet, void* pThis)
{
    if (!pRet) return;
    if (pThis) {
        // TODO(clean-room): partially transcribed -- the +0xe8 type switch and
        // the HLS geometry are not modeled; the retail failure terminal (0,0)
        // is returned.
    }
    new (pRet) CPoint(0, 0);
}
// Retail (0x18002e440): builds the cursor rect from GetCursorPos (types 2/4)
// or the client rect (default).  The geometry is unmodeled, so the retail
// empty-rect terminal is returned.
// Symbol: ?GetCursorRect@CMFCColorPickerCtrl@@IEAA?AVCRect@@XZ
extern "C" void MS_ABI impl__GetCursorRect_CMFCColorPickerCtrl__IEAA_AVCRect__XZ(
    CRect* pRet, void* pThis)
{
    if (!pRet) return;
    if (pThis) {
        // TODO(clean-room): partially transcribed -- the +0xe8 type switch and
        // the GetCursorPos geometry are not modeled; the retail failure
        // terminal (0,0,0,0) is returned.
    }
    new (pRet) CRect(0, 0, 0, 0);
}
// Retail (0x18002d400): writes the three HLS doubles: *pH = +0x100 (H),
// *pL = +0xf0 (L), *pS = +0xf8 (S).  Transcribed verbatim.
// Symbol: ?GetHLS@CMFCColorPickerCtrl@@QEAAXPEAN00@Z
extern "C" void MS_ABI impl__GetHLS_CMFCColorPickerCtrl__QEAAXPEAN00_Z(
    void* pThis, double* pH, double* pL, double* pS)
{
    if (!pThis) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (pH) *pH = s->m_dH;
    if (pL) *pL = s->m_dL;
    if (pS) *pS = s->m_dS;
}
// SetColor(COLORREF clr): stores the color at 0x108, decomposes it into the
// HLS channels (RGBtoHSL, 0x18005b8c0) and repaints when a window exists.
// Transcribed from retail RVA 0x2c4e0.
// Symbol: ?SetColor@CMFCColorPickerCtrl@@QEAAXK@Z
extern "C" void MS_ABI impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    s->m_nColor = static_cast<COLORREF>(clr);                 // 0x108
    impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(static_cast<COLORREF>(clr),
                                                 &s->m_dH, &s->m_dS, &s->m_dL);
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);           // 0x1802c7128
        ::UpdateWindow(s->m_hWnd);                            // 0x1802c7300
    }
}
// SetHLS(double dH, double dL, double dS, BOOL bRedraw): stores each channel
// (a -1.0 sentinel keeps the existing value), recomposes the RGB and repaints
// when bRedraw and a window exist.  Transcribed from retail RVA 0x2d360.
// NOTE: the retail recomposes through HLStoRGB_TWO passing the arguments as
// (H, S, L) -- i.e. with L and S swapped relative to the HLStoRGB (H,L,S)
// signature; that swap is reproduced faithfully here (the picker therefore
// resolves the displayed color with the caller's L and S interchanged).
// Symbol: ?SetHLS@CMFCColorPickerCtrl@@QEAAXNNNH@Z
extern "C" void MS_ABI impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
    void* pThis, double dH, double dL, double dS, int bRedraw)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (dH != -1.0) s->m_dH = dH;                             // 0x100
    if (dS != -1.0) s->m_dS = dS;                             // 0xf8
    if (dL != -1.0) s->m_dL = dL;                             // 0xf0
    s->m_nColor = HlsToRgb_Cmfccolorpickerctrl(s->m_dH, s->m_dS, s->m_dL);        // retail L/S swap
    if (bRedraw != 0 && s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// SetOriginalColor(COLORREF clr): retail is a single store at 0x10c
// (RVA 0x2d350).  Transcribed verbatim.
// Symbol: ?SetOriginalColor@CMFCColorPickerCtrl@@QEAAXK@Z
extern "C" void MS_ABI impl__SetOriginalColor_CMFCColorPickerCtrl__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    s->m_nOriginalColor = static_cast<COLORREF>(clr);         // 0x10c
}
// SetType(COLORTYPE type): retail is a single store at 0xe8 (RVA 0x2d790).
// Transcribed verbatim.
// Symbol: ?SetType@CMFCColorPickerCtrl@@QEAAXW4COLORTYPE@1@@Z
extern "C" void MS_ABI impl__SetType_CMFCColorPickerCtrl__QEAAXW4COLORTYPE_1__Z(
    void* pThis, int type)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    s->m_nType = type;                                        // 0xe8
}
// SetPalette(CPalette* pPalette): stores the palette pointer at 0x118,
// releases the previous embedded GDI object at 0x120 (CGdiObject::Detach +
// DeleteObject) and repaints.  Transcribed from retail RVA 0x2e1e0.
// Symbol: ?SetPalette@CMFCColorPickerCtrl@@QEAAXPEAVCPalette@@@Z
extern "C" void MS_ABI impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    s->m_pPalette = pPalette;                                 // 0x118
    if (s->m_hObject != nullptr) {                            // 0x128
        // Retail: CGdiObject::Detach (0x1802a3f10) + DeleteObject.
        HGDIOBJ h = s->m_hObject;
        s->m_hObject = nullptr;
        ::DeleteObject(h);
    }
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// SetLuminanceBarWidth(int nWidth): clamps the width to 3/4 of the client
// width and stores it at 0x110, then invalidates.  Transcribed from retail
// RVA 0x2d6c0 (the clamp is floor(3*(right-left)/4), non-negative in practice).
// Symbol: ?SetLuminanceBarWidth@CMFCColorPickerCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__SetLuminanceBarWidth_CMFCColorPickerCtrl__QEAAXH_Z(
    void* pThis, int nWidth)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    int nBar = nWidth;
    if (s->m_hWnd != nullptr) {
        RECT rc;
        ::GetClientRect(s->m_hWnd, &rc);                      // 0x1802c7330
        const int nMax = (3 * (rc.right - rc.left)) / 4;
        if (nBar >= nMax) nBar = nMax;
    }
    s->m_nLumBarWidth = nBar;                                 // 0x110
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    }
}
// LumFromPoint(int nY): maps a client y-coordinate onto the luminance range,
// returning 1.0 at the top and 0.0 at the bottom.  Transcribed from retail
// RVA 0x2e240 (GetClientRect, InflateRect(&rc, 0, -5), clamp to [top,bottom]).
// Symbol: ?LumFromPoint@CMFCColorPickerCtrl@@IEAANH@Z
extern "C" double MS_ABI impl__LumFromPoint_CMFCColorPickerCtrl__IEAANH_Z(
    void* pThis, int nY)
{
    if (pThis == nullptr) return 0.0;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (s->m_hWnd == nullptr) return 0.0;
    RECT rc;
    ::GetClientRect(s->m_hWnd, &rc);
    ::InflateRect(&rc, 0, -5);                                // 0x1802c72e8
    int y = nY;
    if (rc.top > y) y = rc.top;
    if (y > rc.bottom) y = rc.bottom;
    return static_cast<double>(rc.bottom - y) /
           static_cast<double>(rc.bottom - rc.top);
}
// PointFromLum(double dLum): inverse of LumFromPoint -- the y-coordinate of
// the given luminance.  Transcribed from retail RVA 0x2e2d0.
// Symbol: ?PointFromLum@CMFCColorPickerCtrl@@IEAAHN@Z
extern "C" int MS_ABI impl__PointFromLum_CMFCColorPickerCtrl__IEAAHN_Z(
    void* pThis, double dLum)
{
    if (pThis == nullptr) return 0;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (s->m_hWnd == nullptr) return 0;
    RECT rc;
    ::GetClientRect(s->m_hWnd, &rc);
    ::InflateRect(&rc, 0, -5);
    return rc.top +
           static_cast<int>((1.0 - dLum) *
                            static_cast<double>(rc.bottom - rc.top));
}
// NotifyParent(): sends WM_COMMAND (BN_CLICKED, wParam = control id) to the
// permanent parent CWnd.  Transcribed from retail RVA 0x2e4f0.
// Symbol: ?NotifyParent@CMFCColorPickerCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (s->m_hWnd == nullptr) return;
    HWND hwndParent = ::GetParent(s->m_hWnd);                 // 0x1802c72d8
    if (hwndParent == nullptr) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwndParent);
    if (pParent == nullptr || pParent->m_hWnd == nullptr) return;
    const WORD wId = static_cast<WORD>(::GetDlgCtrlID(s->m_hWnd));
    ::SendMessageW(pParent->m_hWnd, WM_COMMAND,               // 0x1802c7120
                   static_cast<WPARAM>(wId),                  // BN_CLICKED == 0
                   reinterpret_cast<LPARAM>(s->m_hWnd));
}
// SelectCellHexagon(int nX, int nY): BOOL result -- selects the hexagon cell
// under (nX, nY).  Transcribed from retail RVA 0x2c530.
// Symbol: ?SelectCellHexagon@CMFCColorPickerCtrl@@IEAAHHH@Z
extern "C" int MS_ABI impl__SelectCellHexagon_CMFCColorPickerCtrl__IEAAHHH_Z(
    void* pThis, int nX, int nY)
{
    return SelectCellHexagonImpl(reinterpret_cast<CColorPicker*>(pThis), nX, nY);
}
// SelectCellHexagon(BYTE bR, BYTE bG, BYTE bB): packs the RGB bytes and calls
// SetColor.  Transcribed from retail RVA 0x2c4c0 (tail-calls SetColor 0x2c4e0).
// Symbol: ?SelectCellHexagon@CMFCColorPickerCtrl@@QEAAXEEE@Z
extern "C" void MS_ABI impl__SelectCellHexagon_CMFCColorPickerCtrl__QEAAXEEE_Z(
    void* pThis, unsigned char bR, unsigned char bG, unsigned char bB)
{
    if (pThis == nullptr) return;
    const unsigned long clr = static_cast<unsigned long>(bR) |
                              (static_cast<unsigned long>(bG) << 8) |
                              (static_cast<unsigned long>(bB) << 16);
    impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(pThis, clr);
}
// OnMouseMove(UINT nFlags, CPoint point): while this control owns the mouse
// capture, clamps the point into the client rect and updates the colour
// according to the picker type (hexagon types 3/4 select a cell; type 1 moves
// the luminance; type 2 moves the hue/saturation point, holding the current
// hue with MK_CONTROL and the current luminance with MK_SHIFT), then notifies
// the parent and repaints.  Transcribed from retail RVA 0x2d470.
// Symbol: ?OnMouseMove@CMFCColorPickerCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCColorPickerCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture()) !=
            reinterpret_cast<CWnd*>(pThis)) {
        return;   // not the capture owner
    }
    if (s->m_hWnd == nullptr) return;

    RECT rc;
    ::GetClientRect(s->m_hWnd, &rc);                          // 0x1802c7330

    // Retail clamps the y coordinate to [rc.left, rc.right] and the x
    // coordinate to [rc.top, rc.bottom] (the axes are transposed relative to
    // the drawing space).
    int xClamped = point.x, yClamped = point.y;
    if (rc.top > xClamped) xClamped = rc.top;
    if (xClamped > rc.bottom) xClamped = rc.bottom;
    if (rc.left > yClamped) yClamped = rc.left;
    if (yClamped > rc.right) yClamped = rc.right;

    switch (s->m_nType) {
    case 3:
    case 4:
        // Hexagon cell selection.
        if (SelectCellHexagonImpl(s, yClamped, xClamped) != 0) {
            ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
            impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
            ::UpdateWindow(s->m_hWnd);
        }
        return;
    case 1:
        // Luminance: the vertical position maps to L via LumFromPoint.
        s->m_dL = impl__LumFromPoint_CMFCColorPickerCtrl__IEAANH_Z(pThis, xClamped);
        s->m_nColor = HlsToRgb_Cmfccolorpickerctrl(s->m_dH, s->m_dL, s->m_dS);
        {
            RECT rcBar = GetLumBarRect(s);
            ::InflateRect(&rcBar, 1, 1);                      // 0x1802c72e8
            ::InvalidateRect(s->m_hWnd, &rcBar, TRUE);
            POINT pt = GetColorDotPoint(s);
            RECT rPt = { pt.x, pt.y, 0, 0 };   // retail passes the raw POINT
            ::InvalidateRect(s->m_hWnd, &rPt, TRUE);
        }
        impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
        ::UpdateWindow(s->m_hWnd);
        return;
    case 2:
        // Hue/saturation plane: x sets S, y sets H; MK_CONTROL/MK_SHIFT keep
        // the current hue/luminance-position values respectively.
        if ((nFlags & MK_CONTROL) != 0) {
            POINT pt = GetColorDotPoint(s);
            yClamped = pt.x;
        }
        if ((nFlags & MK_SHIFT) != 0) {
            POINT pt = GetColorDotPoint(s);
            xClamped = pt.y;
        }
        s->m_dH = static_cast<double>(yClamped) /
                  static_cast<double>(rc.right - rc.left);
        s->m_dS = 1.0 - static_cast<double>(xClamped) /
                        static_cast<double>(rc.bottom - rc.top);
        s->m_nColor = HlsToRgb_Cmfccolorpickerctrl(s->m_dH, s->m_dL, s->m_dS);
        {
            RECT rcBar = GetLumBarRect(s);
            ::InflateRect(&rcBar, 1, 1);
            ::InvalidateRect(s->m_hWnd, &rcBar, TRUE);
            POINT pt = GetColorDotPoint(s);
            RECT rPt = { pt.x, pt.y, 0, 0 };
            ::InvalidateRect(s->m_hWnd, &rPt, TRUE);
        }
        impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
        ::UpdateWindow(s->m_hWnd);
        return;
    default:
        // Unhandled types still notify the parent and repaint (retail RVA
        // 0x2d658).
        impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
        ::UpdateWindow(s->m_hWnd);
        return;
    }
}
// OnLButtonDown(UINT, CPoint): captures the mouse, focuses the control and
// forwards to OnMouseMove.  Transcribed from retail RVA 0x2d420 (the
// FromHandle(SetCapture()) result is discarded in retail).
// Symbol: ?OnLButtonDown@CMFCColorPickerCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCColorPickerCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    ::SetCapture(s->m_hWnd);                                  // 0x1802c7250
    (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture());
    ::SetFocus(s->m_hWnd);   // retail: CWnd::SetFocus
    impl__OnMouseMove_CMFCColorPickerCtrl__IEAAXIVCPoint___Z(pThis, nFlags, point);
}
// OnLButtonUp(UINT, CPoint): releases the capture when this control still owns
// it, then runs the default handler.  Transcribed from retail RVA 0x2d690.
// Symbol: ?OnLButtonUp@CMFCColorPickerCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCColorPickerCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture()) ==
            reinterpret_cast<CWnd*>(pThis)) {
        ::ReleaseCapture();                                   // 0x1802c7258
    }
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// OnLButtonDblClk(UINT, CPoint): for types 2/3/4 selects the hexagon cell at
// the point and, on a hit, sends the parent a BN_DBLCLK command; other types
// fall back to the default handler.  Transcribed from retail RVA 0x2e550.
// Symbol: ?OnLButtonDblClk@CMFCColorPickerCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCColorPickerCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT point)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (s->m_nType != 2 && s->m_nType != 3 && s->m_nType != 4) {
        (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
        return;
    }
    if (SelectCellHexagonImpl(s, point.x, point.y) != 0) {
        if (s->m_hWnd != nullptr) {
            HWND hwndParent = ::GetParent(s->m_hWnd);
            CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwndParent);
            if (pParent != nullptr && pParent->m_hWnd != nullptr) {
                const WORD wId = static_cast<WORD>(::GetDlgCtrlID(s->m_hWnd));
                ::SendMessageW(pParent->m_hWnd, WM_COMMAND,
                               static_cast<WPARAM>(wId) | 0x50000u,   // BN_DBLCLK
                               reinterpret_cast<LPARAM>(s->m_hWnd));
            }
        }
    }
}
// OnKeyDown(UINT nChar, UINT, UINT): adjusts the colour with the arrow keys.
// Type 1 (luminance) moves L with Up/Down; type 2 moves H with Left/Right and
// S with Up/Down; types 3/4 (hexagon) move the selected cell with the arrows,
// then recompose through HLStoRGB and repaint the bar/dot regions.  The retail
// "move into the parent picker" branches (type 3 VK_DOWN at the edge, type 4
// Left/Up/Down on the first cell) depend on the unmodeled cross-control
// geometry and are skipped.  Transcribed from retail RVA 0x2d7a0.
// Symbol: ?OnKeyDown@CMFCColorPickerCtrl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCColorPickerCtrl__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int /*nRepCnt*/, unsigned int /*nFlags*/)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);

    if (s->m_nType == 1) {
        RECT rcBar = GetLumBarRect(s);
        ::InflateRect(&rcBar, 1, 1);
        const double dOld = s->m_dL;
        double dNew = dOld;
        if (nChar == VK_UP) dNew += 0.05;
        else if (nChar == VK_DOWN) dNew -= 0.05;
        if (dNew < 0.0) dNew = 0.0;
        if (dNew > 1.0) dNew = 1.0;
        if (dNew != dOld) {
            s->m_dL = dNew;
            s->m_nColor = HlsToRgb_Cmfccolorpickerctrl(s->m_dH, dNew, s->m_dS);
            if (s->m_hWnd != nullptr) ::InvalidateRect(s->m_hWnd, &rcBar, TRUE);
            RECT rcBar2 = GetLumBarRect(s);
            if (s->m_hWnd != nullptr) ::InvalidateRect(s->m_hWnd, &rcBar2, TRUE);
            impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
        }
        (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
        return;
    }

    if (s->m_nType == 2) {
        RECT rcBar = GetLumBarRect(s);
        ::InflateRect(&rcBar, 1, 1);
        const double dOldH = s->m_dH, dOldS = s->m_dS;
        double dNewH = dOldH, dNewS = dOldS;
        switch (nChar) {
        case VK_LEFT:  dNewH -= 0.05; break;
        case VK_RIGHT: dNewH += 0.05; break;
        case VK_UP:    dNewS += 0.05; break;
        case VK_DOWN:  dNewS -= 0.05; break;
        default: break;
        }
        if (dNewS < 0.0) dNewS = 0.0;
        if (dNewS > 1.0) dNewS = 1.0;
        if (dNewH < 0.0) dNewH = 0.0;
        if (dNewH > 1.0) dNewH = 1.0;
        s->m_dS = dNewS;
        s->m_dH = dNewH;
        if (dNewH != dOldH || dNewS != dOldS) {
            s->m_nColor = HlsToRgb_Cmfccolorpickerctrl(dNewH, s->m_dL, dNewS);
            if (s->m_hWnd != nullptr) ::InvalidateRect(s->m_hWnd, &rcBar, TRUE);
            RECT rcBar2 = GetLumBarRect(s);
            if (s->m_hWnd != nullptr) ::InvalidateRect(s->m_hWnd, &rcBar2, TRUE);
            impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
        }
        (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
        return;
    }

    if (s->m_nType == 3 || s->m_nType == 4) {
        // Locate the currently selected cell (the one whose colour is m_nColor).
        const int n = s->m_nCells;
        int idx = -1;
        ColorCell* cell = nullptr;
        if (s->m_pCells != nullptr) {
            for (int i = 0; i < n; i++) {
                ColorCell* c = reinterpret_cast<ColorCell*>(s->m_pCells[i]);
                if (c != nullptr && c->color == s->m_nColor) {
                    idx = i;
                    cell = c;
                    break;
                }
            }
        }
        if (cell == nullptr) {
            (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
            return;
        }

        if (s->m_nType == 3) {
            // Move to the neighbouring hexagon cell (2-D grid geometry).
            int nx = cell->cx, ny = cell->cy;
            switch (nChar) {
            case VK_LEFT:
                nx = cell->cx - cell->size;
                break;
            case VK_UP:
                nx = cell->cx - cell->size / 2;
                ny = cell->cy - cell->size;
                break;
            case VK_RIGHT:
                nx = cell->cx + cell->size;
                break;
            case VK_DOWN:
                nx = cell->cx + cell->size / 2;
                ny = cell->cy + cell->size;
                break;
            default:
                (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
                return;
            }
            if (nx == cell->cx && ny == cell->cy) {
                (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
                return;
            }
            if (SelectCellHexagonImpl(s, nx, ny) != 0) {
                if (s->m_hWnd != nullptr) {
                    ::RedrawWindow(s->m_hWnd, nullptr, nullptr, 0x105);  // 0x1802c7130
                }
                impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
            } else if (nChar == VK_DOWN) {
                // TODO(clean-room): transcribed partially -- the retail moves
                // the selection into the parent picker (FromHandle/IsKindOf of
                // the parent CMFCColorPickerCtrl, SetFocus, colour copy from
                // the parent's cell array at +0x3f0/+0x3f8); not modeled.
            }
            (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
            return;
        }

        // Type 4: the cells are a linear array; arrows walk the neighbours.
        int ni = -1;
        switch (nChar) {
        case VK_RIGHT:
            if (idx >= n - 1) {
                (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
                return;
            }
            ni = idx + 1;
            break;
        case VK_LEFT:
        case VK_UP:
        case VK_DOWN:
            if (idx <= 0) {
                // TODO(clean-room): transcribed partially -- the retail jumps
                // into the parent picker at the first cell (parent +0x298/
                // +0x2a0 array); not modeled.
                (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
                return;
            }
            ni = idx - 1;
            break;
        default:
            (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
            return;
        }
        ColorCell* cell2 = reinterpret_cast<ColorCell*>(s->m_pCells[ni]);
        if (cell2 != nullptr) {
            impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(pThis, cell2->color);
            if (s->m_hWnd != nullptr) {
                ::RedrawWindow(s->m_hWnd, nullptr, nullptr, 0x105);
            }
            impl__NotifyParent_CMFCColorPickerCtrl__IEAAXXZ(pThis);
        }
        (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
        return;
    }

    // Unhandled type: just run the default handler.
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// OnKillFocus(CWnd*): runs the default handler, then invalidates the control.
// Transcribed from retail RVA 0x2dc00.
// Symbol: ?OnKillFocus@CMFCColorPickerCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCColorPickerCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* /*pNewWnd*/)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    }
}
// OnSetFocus(CWnd*): runs the CWnd base handler, then invalidates the control.
// Transcribed from retail RVA 0x2dbd0 (base at 0x18028f2a0).
// Symbol: ?OnSetFocus@CMFCColorPickerCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCColorPickerCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pOldWnd)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(reinterpret_cast<CWnd*>(pThis), pOldWnd);
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    }
}
// OnQueryNewPalette(): invalidates the control and returns the default
// handler's result.  Transcribed from retail RVA 0x2dc30.
// Symbol: ?OnQueryNewPalette@CMFCColorPickerCtrl@@IEAAHXZ
extern "C" int MS_ABI impl__OnQueryNewPalette_CMFCColorPickerCtrl__IEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    }
    return static_cast<int>(
        impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis)));
}
// OnPaletteChanged(CWnd*): runs the default handler and invalidates when the
// palette-focus window differs from this control.  Transcribed from retail
// RVA 0x2dc60.
// Symbol: ?OnPaletteChanged@CMFCColorPickerCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnPaletteChanged_CMFCColorPickerCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pFocusWnd)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
    const HWND hwndFocus = (pFocusWnd != nullptr) ? pFocusWnd->m_hWnd : nullptr;
    if (hwndFocus != s->m_hWnd && s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    }
}
// OnSize(UINT, int, int): runs the default handler, releases the embedded GDI
// object at 0x120 (Detach + DeleteObject) when it holds a handle, then
// repaints.  Transcribed from retail RVA 0x2d740.
// Symbol: ?OnSize@CMFCColorPickerCtrl@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCColorPickerCtrl__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (pThis == nullptr) return;
    CColorPicker* s = reinterpret_cast<CColorPicker*>(pThis);
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
    if (s->m_hObject != nullptr) {
        HGDIOBJ h = s->m_hObject;
        s->m_hObject = nullptr;
        ::DeleteObject(h);                                    // 0x1802c6278
    }
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// OnCancelMode(): retail is a tail-jump to the default handler (RVA 0xda30).
// Transcribed verbatim.
// Symbol: ?OnCancelMode@CMFCColorPickerCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCColorPickerCtrl__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}
// OnGetDlgCode(): retail is `mov $1,%eax; ret` (RVA 0x3a60).  Transcribed
// verbatim.
// Symbol: ?OnGetDlgCode@CMFCColorPickerCtrl@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CMFCColorPickerCtrl__IEAAIXZ(
    void* /*pThis*/)
{
    return 1;
}
// OnEraseBkgnd(CDC*): shares retail RVA 0x3a60 with OnGetDlgCode -- both
// return 1.  Transcribed verbatim.
// Symbol: ?OnEraseBkgnd@CMFCColorPickerCtrl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCColorPickerCtrl__IEAAHPEAVCDC___Z(
    void* /*pThis*/, void* /*pDC*/)
{
    return 1;
}
// PreCreateWindow(CREATESTRUCTW& cs): adjusts the window style (clear bit 0,
// set bits 1 and 3 -- WS_CHILD | WS_TABSTOP) and runs the CWnd base.
// Transcribed from retail RVA 0x2dcb0.
// Symbol: ?PreCreateWindow@CMFCColorPickerCtrl@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCColorPickerCtrl__MEAAHAEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW& cs)
{
    if (pThis == nullptr) return 0;
    cs.style = (cs.style & ~1u) | 0xau;
    return impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
        reinterpret_cast<CWnd*>(pThis), cs);
}
// PreSubclassWindow(): applies CWnd::ModifyStyle(1, 0xb, 0).  Transcribed from
// retail RVA 0x2dcd0 (tail-call to ?ModifyStyle@CWnd, 0x1802a96f0).
// Symbol: ?PreSubclassWindow@CMFCColorPickerCtrl@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCColorPickerCtrl__MEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    (void)impl__ModifyStyle_CWnd__QEAAHKKI_Z(
        reinterpret_cast<CWnd*>(pThis), 1, 0xb, 0);
}
// Symbol: ??0CMFCColorPickerCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorPickerCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}
