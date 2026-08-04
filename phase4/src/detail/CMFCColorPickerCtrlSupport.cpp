#include "CMFCColorPickerCtrlSupport.h"

static_assert(offsetof(CColorPicker, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(CColorPicker, m_nType) == 0xe8, "m_nType @0xe8");
static_assert(offsetof(CColorPicker, m_dL) == 0xf0, "m_dL @0xf0");
static_assert(offsetof(CColorPicker, m_dS) == 0xf8, "m_dS @0xf8");
static_assert(offsetof(CColorPicker, m_dH) == 0x100, "m_dH @0x100");
static_assert(offsetof(CColorPicker, m_nColor) == 0x108, "m_nColor @0x108");
static_assert(offsetof(CColorPicker, m_nOriginalColor) == 0x10c, "orig @0x10c");
static_assert(offsetof(CColorPicker, m_nLumBarWidth) == 0x110, "bar width @0x110");
static_assert(offsetof(CColorPicker, m_pPalette) == 0x118, "palette @0x118");
static_assert(offsetof(CColorPicker, m_hObject) == 0x128, "gdi handle @0x128");
static_assert(offsetof(CColorPicker, m_pCells) == 0x138, "cells @0x138");
static_assert(offsetof(CColorPicker, m_nCells) == 0x140, "cell count @0x140");
static_assert(offsetof(ColorCell, pts) == 0x8, "pts @0x08");
static_assert(offsetof(ColorCell, color) == 0x38, "color @0x38");
static_assert(offsetof(ColorCell, cx) == 0x40, "cx @0x40");
static_assert(offsetof(ColorCell, cy) == 0x44, "cy @0x44");
static_assert(offsetof(ColorCell, size) == 0x48, "size @0x48");
namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
COLORREF HlsToRgb_Cmfccolorpickerctrl(double H, double L, double S)
{
    if (S == 0.0) {
        const int v = static_cast<int>(L * 255.0);
        return RGB(v, v, v);
    }
    double M2;
    if (L <= 0.5) {
        M2 = L * (1.0 + S);
    } else if (L == 1.0) {
        M2 = L;
    } else {
        M2 = L + S - L * S;
    }
    const double M1 = 2.0 * L - M2;
    const int r = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H + 1.0 / 3.0) * 255.0);
    const int g = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H) * 255.0);
    const int b = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H - 1.0 / 3.0) * 255.0);
    return RGB(r, g, b);
}
POINT GetColorDotPoint(CColorPicker* s)
{
    POINT pt = { 0, 0 };
    if (s == nullptr || s->m_hWnd == nullptr) return pt;
    RECT rc;
    ::GetClientRect(s->m_hWnd, &rc);
    switch (s->m_nType) {
    case 1:  // luminance bar: dot sits on the bar (left + 6 + width)
        pt.x = rc.left + 6 + s->m_nLumBarWidth;
        pt.y = impl__PointFromLum_CMFCColorPickerCtrl__IEAAHN_Z(s, s->m_dL);
        break;
    case 2:  // HLS plane: x = H * width, y = (1-S) * height
        pt.x = static_cast<int>(s->m_dH * static_cast<double>(rc.right - rc.left));
        pt.y = static_cast<int>((1.0 - s->m_dS) *
                                static_cast<double>(rc.bottom - rc.top));
        break;
    default:
        break;
    }
    return pt;
}
RECT GetLumBarRect(CColorPicker* s)
{
    RECT rc = { 0, 0, 0, 0 };
    if (s == nullptr) return rc;
    switch (s->m_nType) {
    case 2: {   // 9x9 box around the colour dot
        const POINT pt = GetColorDotPoint(s);
        rc.left = pt.x;
        rc.top = pt.y;
        rc.right = pt.x + 9;
        rc.bottom = pt.y + 9;
        break;
    }
    case 3: {   // 19x19 box around the colour dot
        const POINT pt = GetColorDotPoint(s);
        rc.left = pt.x;
        rc.top = pt.y;
        rc.right = pt.x + 19;
        rc.bottom = pt.y + 19;
        break;
    }
    default:
        ::SetRectEmpty(&rc);
        break;
    }
    ::OffsetRect(&rc, -(rc.right - rc.left) / 2, -(rc.bottom - rc.top) / 2);
    return rc;
}
int SelectCellHexagonImpl(CColorPicker* s, int x, int y)
{
    if (s == nullptr) return 0;
    const int n = s->m_nCells;
    if (s->m_pCells == nullptr) return 0;
    for (int i = 0; i < n; i++) {
        ColorCell* cell = reinterpret_cast<ColorCell*>(s->m_pCells[i]);
        if (cell == nullptr) continue;
        // Retail: CreatePolygonRgn(cell->pts, 6, 1) into a local CRgn
        // (0x1802c6218), PtInRegion (0x1802c6210) with (x, y), then the CRgn
        // destructor.  The raw region is equivalent here.
        HRGN rgn = ::CreatePolygonRgn(cell->pts, 6, 1);
        BOOL bHit = FALSE;
        if (rgn != nullptr) {
            bHit = ::PtInRegion(rgn, x, y);
            ::DeleteObject(rgn);
        }
        if (bHit) {
            s->m_nColor = cell->color;                        // 0x108
            impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(     // 0x18005b8c0
                cell->color, &s->m_dH, &s->m_dS, &s->m_dL);
            return 1;
        }
    }
    return 0;
}
} } }  // namespace openmfc::detail::cmfccolorpickerctrl
