#pragma once
// Shared internals of the former global_cmfccolorpickerctrl.cpp translation unit.
// Definitions live in detail/CMFCColorPickerCtrlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {} } }
using namespace openmfc::detail::cmfccolorpickerctrl;
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


namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
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
} } }


// A hexagon cell (CreateHexagon's records): 6 vertices at +0x08, the colour at
// +0x38 and the geometry used by the arrow-key moves at +0x40..+0x48.
namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
struct ColorCell {
    char     _pad0[0x8];
    POINT    pts[6];        // 0x08: hexagon vertices (ends at 0x38)
    COLORREF color;         // 0x38
    char     _pad3c[0x40 - 0x3c];
    int      cx;            // 0x40: centre x
    int      cy;            // 0x44: centre y
    int      size;          // 0x48: half height/width used by OnKeyDown
};
} } }


// The HLS<->RGB recomposition used by SetColor / OnMouseMove / OnKeyDown.
// Matches CDrawingManager::HLStoRGB_ONE (retail RVA 0x18005b650): hue is a
// turn fraction in [0,1], the conversion follows the standard hue-to-rgb
// formula, and each channel is truncated (cvttsd2si) after scaling by 255.
// (Retail SetHLS routes through HLStoRGB_TWO with the L/S arguments swapped -
// reproduced in SetHLS below.)
namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
COLORREF HlsToRgb_Cmfccolorpickerctrl(double H, double L, double S);
} } }

// The internal luminance-bar / colour-dot geometry helpers (retail 0x18002e440
// and 0x18002e360).  GetLumBarRect returns the small bar highlight rect
// (centered via OffsetRect); GetColorDotPoint returns the current colour dot.
namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
POINT GetColorDotPoint(CColorPicker* s);
} } }

namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
RECT GetLumBarRect(CColorPicker* s);
} } }

// The exported SelectCellHexagon(int, int) body -- shared by OnMouseMove /
// OnKeyDown / OnLButtonDblClk.  Iterates the hexagon cells, tests the mouse
// point against each cell's polygon region and selects the first hit.
namespace openmfc { namespace detail { namespace cmfccolorpickerctrl {
int SelectCellHexagonImpl(CColorPicker* s, int x, int y);
} } }


//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::CreateHexGreyScaleBar
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::CreateHexagon
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawCursor
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawHex
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawItem
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawLuminanceBar
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::DrawPicker
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetAngleFromPoint
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetCursorPos
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetCursorRect
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorPickerCtrl::GetHLS
//-----------------------------------------------------------------------------


























