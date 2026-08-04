#pragma once
// Shared internals of the former global_cmfccustomcolorspropertypage.cpp translation unit.
// Definitions live in detail/CMFCCustomColorsPropertyPageSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccustomcolorspropertypage {} } }
using namespace openmfc::detail::cmfccustomcolorspropertypage;
// OpenMFC: CMFCCustomColorsPropertyPage exports.
//
// All 9 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the members are reached through raw offsets.  It is CDialog-derived and
// 0x430 bytes: hwnd at +0x40, the CMFCColorDialog owner at +0x158, the embedded
// CMFCColorPickerCtrl sub-objects at +0x160/+0x2b8, the luminance slider value
// at +0x250, the H/S mirrors at +0x258/+0x260, the colour-control HWND at
// +0x1a0, the HLS doubles at +0x3a8 (L) / +0x3b0 (S) / +0x3b8 (H), the COLORREF
// at +0x3c0, the RGB ints at +0x410/+0x414/+0x418, the HLS ints at +0x41c (L) /
// +0x420 (H) / +0x424 (S), the init flag at +0x428 and a re-entrancy guard at
// +0x42c.
//
// Faithfully transcribed: the DDX wiring in DoDataExchange and the colour
// arithmetic in Setup / OnColour / OnLuminance / OnRGBChanged / OnHLSChanged /
// OnInitDialog.
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

extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int p0);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" void MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult);
extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pv);
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(
    void* pDX, int nIDC, int* pv);
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAI_Z(
    void* pDX, int nIDC, unsigned int* pv);
extern "C" void MS_ABI impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
    void* pThis, double dH, double dL, double dS, int bRedraw);
extern "C" void MS_ABI impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette);
extern "C" void MS_ABI impl__SetLuminanceBarWidth_CMFCColorPickerCtrl__QEAAXH_Z(
    void* pThis, int nWidth);
extern "C" void MS_ABI impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(
    COLORREF rgb, double* pH, double* pS, double* pL);
extern "C" double MS_ABI impl__HuetoRGB_CDrawingManager__SANNNN_Z(
    double m1, double m2, double h);
extern "C" void MS_ABI impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(
    void* pThis, unsigned long clr);
extern "C" void MS_ABI impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue);
extern "C" void MS_ABI impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue);


namespace openmfc { namespace detail { namespace cmfccustomcolorspropertypage {
struct CColorsPage {
    char      _pad000[0x40];
    HWND      m_hWnd;              // 0x040: CWnd::m_hWnd
    char      _pad048[0x158 - 0x48];
    void*     m_pOwner;            // 0x158: CMFCColorDialog*
    char      _pad160[0x250 - 0x160];  // 0x160: picker sub-objects (opaque)
    double    m_dLumSlider;        // 0x250: luminance slider value (OnLuminance)
    double    m_dS255;             // 0x258: S mirror (OnColour)
    double    m_dH255;             // 0x260: H mirror (OnColour)
    char      _pad268[0x3a0 - 0x268];
    int       m_nPageKind;         // 0x3a0: page kind (OnInitDialog sets 2)
    double    m_dL;                // 0x3a8: Luminance
    double    m_dS;                // 0x3b0: Saturation
    double    m_dH;                // 0x3b8: Hue (turns)
    COLORREF  m_nColor;            // 0x3c0: current color
    char      _pad3c4[0x410 - 0x3c4];
    int       m_nR;                // 0x410: Red edit box value
    int       m_nB;                // 0x414: Blue edit box value
    int       m_nG;                // 0x418: Green edit box value
    int       m_nL;                // 0x41c: Luminance edit box value
    int       m_nH;                // 0x420: Hue edit box value
    int       m_nS;                // 0x424: Saturation edit box value
    int       m_bInitialized;      // 0x428: controls-initialized flag
    int       m_bUpdating;         // 0x42c: re-entrancy guard
};
} } }


// The channel-to-8-bit helper used all over the colour page: (double*255)+0.5
// truncated, clamped to 255 (retail constant 0x180350098 = 255.0,
// 0x18034fef0 = 0.5, 0x18034ff68 = 1.0).
namespace openmfc { namespace detail { namespace cmfccustomcolorspropertypage {
int ChannelToInt(double d);
} } }

// HLS -> COLORREF.  Matches CDrawingManager::HLStoRGB (retail 0x18005b650):
// hue is a turn fraction in [0,1], the conversion follows the standard
// hue-to-rgb formula and each channel is truncated after scaling by 255.
// (Identical to the helper used in global_cmfccolorpickerctrl.cpp.)
namespace openmfc { namespace detail { namespace cmfccustomcolorspropertypage {
COLORREF HlsToRgb_Cmfccustomcolorspropertypage(double H, double L, double S);
} } }










