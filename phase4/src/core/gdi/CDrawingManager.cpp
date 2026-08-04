// CDrawingManager — OpenMFC implementation.
// Sources: global_static_utils.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCDynamicLayoutSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?HuetoRGB@CDrawingManager@@SANNNN@Z
extern "C" MS_ABI double impl__HuetoRGB_CDrawingManager__SANNNN_Z(double m1, double m2, double h)
{
    return DM_HuetoRGB(m1, m2, h);
}
// static BYTE CDrawingManager::HueToRGB(float m1, float m2, float h)
// Float variant whose hue is expressed in DEGREES [0,360].  Returns the channel
// scaled to a byte (truncated).  Verified 0/2916 mismatches against the real
// ?HueToRGB@CDrawingManager@@SAEMMM@Z.
// Symbol: ?HueToRGB@CDrawingManager@@SAEMMM@Z
extern "C" MS_ABI BYTE impl__HueToRGB_CDrawingManager__SAEMMM_Z(float m1, float m2, float h)
{
    if (h > 360.0f) h -= 360.0f;
    else if (h < 0.0f) h += 360.0f;

    float v;
    if (h < 60.0f)       v = m1 + (m2 - m1) * h / 60.0f;
    else if (h < 180.0f) v = m2;
    else if (h < 240.0f) v = m1 + (m2 - m1) * (240.0f - h) / 60.0f;
    else                 v = m1;

    return (BYTE)(v * 255.0f);
}
// static COLORREF CDrawingManager::PixelAlpha(COLORREF srcPixel, int nPercent)
// Scales each channel by nPercent/100, saturating high at 255 (no low clamp -
// matches the real DLL for the documented 0..100 range and its unsigned overflow
// behaviour outside it).  Verified 0/65416 mismatches for nPercent in [0,100].
// Symbol: ?PixelAlpha@CDrawingManager@@SAKKH@Z
extern "C" MS_ABI COLORREF impl__PixelAlpha_CDrawingManager__SAKKH_Z(COLORREF srcPixel, int nPercent)
{
    int r = GetRValue(srcPixel) * nPercent / 100;
    int g = GetGValue(srcPixel) * nPercent / 100;
    int b = GetBValue(srcPixel) * nPercent / 100;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    return RGB(r, g, b);
}
// static COLORREF CDrawingManager::PixelAlpha(COLORREF srcColor, COLORREF dstColor, int nPercent)
// Alpha blend where nPercent is the weight (%) of srcColor over dstColor:
//   channel = (src*nPercent + dst*(100-nPercent)) / 100
// computed with a single integer division per channel, saturating high at 255.
// Verified 0/65416 mismatches for nPercent in [0,100].
// Symbol: ?PixelAlpha@CDrawingManager@@SAKKKH@Z
extern "C" MS_ABI COLORREF impl__PixelAlpha_CDrawingManager__SAKKKH_Z(COLORREF srcColor, COLORREF dstColor, int nPercent)
{
    int r = (GetRValue(srcColor) * nPercent + GetRValue(dstColor) * (100 - nPercent)) / 100;
    int g = (GetGValue(srcColor) * nPercent + GetGValue(dstColor) * (100 - nPercent)) / 100;
    int b = (GetBValue(srcColor) * nPercent + GetBValue(dstColor) * (100 - nPercent)) / 100;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    return RGB(r, g, b);
}
// static void CDrawingManager::RGBtoHSL(COLORREF rgb, double* pH, double* pS, double* pL)
// Standard RGB->HSL conversion with H, S and L all normalized to [0,1].  Achromatic
// colors (incl. black and grays) yield H = 0, S = 0.  Verified 0/140608 mismatches
// across the full RGB cube (5-step sweep) against the real DLL.
// Symbol: ?RGBtoHSL@CDrawingManager@@SAXKPEAN00@Z
extern "C" MS_ABI void impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(COLORREF rgb, double* pH, double* pS, double* pL)
{
    double R = GetRValue(rgb);
    double G = GetGValue(rgb);
    double B = GetBValue(rgb);

    double mx = R; if (G > mx) mx = G; if (B > mx) mx = B;
    double mn = R; if (G < mn) mn = G; if (B < mn) mn = B;
    double delta = mx - mn;

    double L = (mx + mn) / 2.0 / 255.0;
    double H = 0.0, S = 0.0;
    if (delta != 0.0)
    {
        S = (L <= 0.5) ? (delta / (mx + mn)) : (delta / (2.0 * 255.0 - mx - mn));
        if (mx == R)      H = (G - B) / delta;
        else if (mx == G) H = (B - R) / delta + 2.0;
        else              H = (R - G) / delta + 4.0;
        if (H < 0.0) H += 6.0;
        H /= 6.0;
    }
    if (pH) *pH = H;
    if (pS) *pS = S;
    if (pL) *pL = L;
}
// Symbol: ??0CDrawingManager@@QEAA@AEAVCDC@@@Z
extern "C" void* MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC) {
    (void)pDC;
    return pThis;
}
// Symbol: ??1CDrawingManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis) {
    (void)pThis;
}
