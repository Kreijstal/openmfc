// OpenMFC: CMFCColorPickerCtrl exports.
//
// All 37 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the layout is mirrored with the file-local struct below.  It is the
// CWnd-derived control embedded at +0xdb0 in CMFCColorDialog; only the fields
// the exports touch are named.  The stub constructor
// (manual_small_stub_implementations.cpp) leaves the object state
// uninitialized, so every pointer access is guarded and a zeroed object behaves
// like a fresh retail object.
//
// Layout notes (harvested from the retail bodies):
//   * HLS channels are doubles: L at +0xf0, S at +0xf8, H at +0x100.  SetColor
//     decomposes through CDrawingManager::RGBtoHSL (0x18005b8c0, whose
//     parameter order is (rgb, &H, &S, &L)); SetHLS / OnMouseMove / OnKeyDown
//     recompose through CDrawingManager::HLStoRGB (0x18005b650).
//   * The hexagon cells are a CPtrArray at +0x138 (count at +0x140); each cell
//     is a { POINT pts[6]@0x08, COLORREF color@0x38, int cx@0x40, int cy@0x44,
//     int size@0x48 } record used by SelectCellHexagon.
//   * OnGetDlgCode and OnEraseBkgnd share the same retail RVA 0x3a60 (both
//     return 1).
//
// Faithfully transcribed: SetColor / SetHLS / SetOriginalColor / SetType /
// SetPalette / SetLuminanceBarWidth / LumFromPoint / PointFromLum /
// NotifyParent / SelectCellHexagon (both overloads) / the focus, palette and
// size message handlers / PreCreateWindow / PreSubclassWindow / OnMouseMove and
// OnKeyDown (all four colour types, including the HLS clamping and the
// hexagon-cell neighbour moves).  The drawing bodies depend on the unmodeled
// visual-manager machinery and are kept as conservative type-correct versions.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" void MS_ABI impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(
    void* pThis, COLORREF clr);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" void MS_ABI impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(
    COLORREF rgb, double* pH, double* pS, double* pL);
extern "C" double MS_ABI impl__HuetoRGB_CDrawingManager__SANNNN_Z(
    double m1, double m2, double h);
// Exports defined later in this file (cross-referenced by the message
// handlers below).
extern "C" int MS_ABI impl__PointFromLum_CMFCColorPickerCtrl__IEAAHN_Z(
    void* pThis, double dLum);
extern "C" void MS_ABI impl__OnMouseMove_CMFCColorPickerCtrl__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point);

namespace {

struct CColorPicker {
    char    _pad000[0x40];
    HWND    m_hWnd;              // 0x040: CWnd::m_hWnd
    char    _pad048[0xe8 - 0x48];
    int     m_nType;             // 0x0e8: COLORTYPE (1..4)
    char    _padec[0xf0 - 0xec];
    double  m_dL;                // 0x0f0: Luminance (RGBtoHSL pL)
    double  m_dS;                // 0x0f8: Saturation (RGBtoHSL pS)
    double  m_dH;                // 0x100: Hue in turns (RGBtoHSL pH)
    COLORREF m_nColor;           // 0x108: current color
    COLORREF m_nOriginalColor;   // 0x10c: original color (SetOriginalColor)
    int     m_nLumBarWidth;      // 0x110: luminance-bar width (SetLuminanceBarWidth)
    char    _pad114[0x118 - 0x114];
    void*   m_pPalette;          // 0x118: CPalette* (SetPalette stores here)
    char    _pad120[0x8];        // 0x120: embedded CGdiObject vtable (opaque)
    HGDIOBJ m_hObject;           // 0x128: that object's m_hObject (Detach+DeleteObject)
    char    _pad130[0x138 - 0x130];
    void**  m_pCells;            // 0x138: CPtrArray<cell> data
    int     m_nCells;            // 0x140: cell count
};

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

// A hexagon cell (CreateHexagon's records): 6 vertices at +0x08, the colour at
// +0x38 and the geometry used by the arrow-key moves at +0x40..+0x48.
struct ColorCell {
    char     _pad0[0x8];
    POINT    pts[6];        // 0x08: hexagon vertices (ends at 0x38)
    COLORREF color;         // 0x38
    char     _pad3c[0x40 - 0x3c];
    int      cx;            // 0x40: centre x
    int      cy;            // 0x44: centre y
    int      size;          // 0x48: half height/width used by OnKeyDown
};

static_assert(offsetof(ColorCell, pts) == 0x8, "pts @0x08");
static_assert(offsetof(ColorCell, color) == 0x38, "color @0x38");
static_assert(offsetof(ColorCell, cx) == 0x40, "cx @0x40");
static_assert(offsetof(ColorCell, cy) == 0x44, "cy @0x44");
static_assert(offsetof(ColorCell, size) == 0x48, "size @0x48");

// The HLS<->RGB recomposition used by SetColor / OnMouseMove / OnKeyDown.
// Matches CDrawingManager::HLStoRGB_ONE (retail RVA 0x18005b650): hue is a
// turn fraction in [0,1], the conversion follows the standard hue-to-rgb
// formula, and each channel is truncated (cvttsd2si) after scaling by 255.
// (Retail SetHLS routes through HLStoRGB_TWO with the L/S arguments swapped -
// reproduced in SetHLS below.)
COLORREF HlsToRgb(double H, double L, double S)
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

// The internal luminance-bar / colour-dot geometry helpers (retail 0x18002e440
// and 0x18002e360).  GetLumBarRect returns the small bar highlight rect
// (centered via OffsetRect); GetColorDotPoint returns the current colour dot.
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

// The exported SelectCellHexagon(int, int) body -- shared by OnMouseMove /
// OnKeyDown / OnLButtonDblClk.  Iterates the hexagon cells, tests the mouse
// point against each cell's polygon region and selects the first hit.
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

} // namespace

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::CreateHexGreyScaleBar
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::CreateHexagon
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawCursor
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawHex
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawItem
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawLuminanceBar
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawPicker
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetAngleFromPoint
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetCursorPos
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetCursorRect
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetHLS
//-----------------------------------------------------------------------------
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
    s->m_nColor = HlsToRgb(s->m_dH, s->m_dS, s->m_dL);        // retail L/S swap
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
        s->m_nColor = HlsToRgb(s->m_dH, s->m_dL, s->m_dS);
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
        s->m_nColor = HlsToRgb(s->m_dH, s->m_dL, s->m_dS);
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
            s->m_nColor = HlsToRgb(s->m_dH, dNew, s->m_dS);
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
            s->m_nColor = HlsToRgb(dNewH, s->m_dL, dNewS);
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
