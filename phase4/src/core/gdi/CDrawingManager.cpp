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
// Symbol: ?CreateBitmap_32@CDrawingManager@@SAPEAUHBITMAP__@@AEBVCSize@@PEAPEAX@Z
extern "C" void* MS_ABI impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(const void* /*class*/* p0, void** p1) {
    return nullptr;
}

// Symbol: ?CreateBitmap_32@CDrawingManager@@SAPEAUHBITMAP__@@PEAU2@K@Z
extern "C" void* MS_ABI impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____PEAU2_K_Z(void* /*struct*/* p0, unsigned long p1) {
    return nullptr;
}

// Symbol: ?DrawAlpha@CDrawingManager@@QEAAXPEAVCDC@@AEBVCRect@@01@Z
extern "C" void MS_ABI impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(void* /*class*/* p0, const void* /*class*/* p1, void* /*class*/* p2, const void* /*class*/* p3) {}

// Symbol: ?DrawEllipse@CDrawingManager@@QEAAXAEBVCRect@@KK@Z
extern "C" void MS_ABI impl__DrawEllipse_CDrawingManager__QEAAXAEBVCRect__KK_Z(const void* /*class*/* p0, unsigned long p1, unsigned long p2) {}

// Symbol: ?DrawGradientRing@CDrawingManager@@QEAAHVCRect@@KKKHHK@Z
extern "C" int MS_ABI impl__DrawGradientRing_CDrawingManager__QEAAHVCRect__KKKHHK_Z(void* /*class*/ p0, unsigned long p1, unsigned long p2, unsigned long p3, int p4, int p5, unsigned long p6) {
    return 0;
}

// Symbol: ?DrawLine@CDrawingManager@@QEAAXHHHHK@Z
extern "C" void MS_ABI impl__DrawLine_CDrawingManager__QEAAXHHHHK_Z(int p0, int p1, int p2, int p3, unsigned long p4) {}

// Symbol: ?DrawLineA@CDrawingManager@@QEAAXNNNNK@Z
extern "C" void MS_ABI impl__DrawLineA_CDrawingManager__QEAAXNNNNK_Z(double p0, double p1, double p2, double p3, unsigned long p4) {}

// Symbol: ?DrawRect@CDrawingManager@@QEAAXAEBVCRect@@KK@Z
extern "C" void MS_ABI impl__DrawRect_CDrawingManager__QEAAXAEBVCRect__KK_Z(const void* /*class*/* p0, unsigned long p1, unsigned long p2) {}

// Symbol: ?DrawRotated@CDrawingManager@@QEAAXVCRect@@AEAVCDC@@H@Z
extern "C" void MS_ABI impl__DrawRotated_CDrawingManager__QEAAXVCRect__AEAVCDC__H_Z(void* /*class*/ p0, void* /*class*/* p1, int p2) {}

// Symbol: ?DrawShadow@CDrawingManager@@QEAAHVCRect@@HHHPEAVCBitmap@@1KH@Z
extern "C" int MS_ABI impl__DrawShadow_CDrawingManager__QEAAHVCRect__HHHPEAVCBitmap__1KH_Z(void* /*class*/ p0, int p1, int p2, int p3, void* /*class*/* p4, int p5, unsigned long p6, int p7) {
    return 0;
}

// Symbol: ?Fill4ColorsGradient@CDrawingManager@@QEAAXVCRect@@KKKKHH@Z
extern "C" void MS_ABI impl__Fill4ColorsGradient_CDrawingManager__QEAAXVCRect__KKKKHH_Z(void* /*class*/ p0, unsigned long p1, unsigned long p2, unsigned long p3, unsigned long p4, int p5, int p6) {}

// Symbol: ?FillAlpha@CDrawingManager@@QEAAXAEBVCRect@@E@Z
extern "C" void MS_ABI impl__FillAlpha_CDrawingManager__QEAAXAEBVCRect__E_Z(const void* /*class*/* p0, unsigned char p1) {}

// Symbol: ?FillGradient@CDrawingManager@@QEAAXVCRect@@KKHHH@Z
extern "C" void MS_ABI impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(void* /*class*/ p0, unsigned long p1, unsigned long p2, int p3, int p4, int p5) {}

// Symbol: ?FillGradient2@CDrawingManager@@QEAAXVCRect@@KKH@Z
extern "C" void MS_ABI impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(void* /*class*/ p0, unsigned long p1, unsigned long p2, int p3) {}

// Symbol: ?GrayRect@CDrawingManager@@QEAAHVCRect@@HKK@Z
extern "C" int MS_ABI impl__GrayRect_CDrawingManager__QEAAHVCRect__HKK_Z(void* /*class*/ p0, int p1, unsigned long p2, unsigned long p3) {
    return 0;
}

// Symbol: ?HighlightRect@CDrawingManager@@QEAAHVCRect@@HKHK@Z
extern "C" int MS_ABI impl__HighlightRect_CDrawingManager__QEAAHVCRect__HKHK_Z(void* /*class*/ p0, int p1, unsigned long p2, int p3, unsigned long p4) {
    return 0;
}

// Symbol: ?HLStoRGB_ONE@CDrawingManager@@SAKNNN@Z
extern "C" unsigned long MS_ABI impl__HLStoRGB_ONE_CDrawingManager__SAKNNN_Z(double p0, double p1, double p2) {
    return 0;
}

// Symbol: ?HLStoRGB_TWO@CDrawingManager@@SAKNNN@Z
extern "C" unsigned long MS_ABI impl__HLStoRGB_TWO_CDrawingManager__SAKNNN_Z(double p0, double p1, double p2) {
    return 0;
}

// Symbol: ?HSVtoRGB@CDrawingManager@@SAKNNN@Z
extern "C" unsigned long MS_ABI impl__HSVtoRGB_CDrawingManager__SAKNNN_Z(double p0, double p1, double p2) {
    return 0;
}

// Symbol: ?MirrorRect@CDrawingManager@@QEAAXVCRect@@H@Z
extern "C" void MS_ABI impl__MirrorRect_CDrawingManager__QEAAXVCRect__H_Z(void* /*class*/ p0, int p1) {}

// Symbol: ?PixelAlpha@CDrawingManager@@SAKKNNN@Z
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(unsigned long p0, double p1, double p2, double p3) {
    return 0;
}

// Symbol: ?PrepareShadowMask@CDrawingManager@@SAPEAUHBITMAP__@@HKHH@Z
extern "C" void* MS_ABI impl__PrepareShadowMask_CDrawingManager__SAPEAUHBITMAP____HKHH_Z(int p0, unsigned long p1, int p2, int p3) {
    return nullptr;
}

// Symbol: ?RGBtoHSV@CDrawingManager@@SAXKPEAN00@Z
extern "C" void MS_ABI impl__RGBtoHSV_CDrawingManager__SAXKPEAN00_Z(unsigned long p0, double* p1, unsigned long p2, unsigned long p3) {}

// Symbol: ?SetPixel@CDrawingManager@@SAXPEAKHHHHK@Z
extern "C" void MS_ABI impl__SetPixel_CDrawingManager__SAXPEAKHHHHK_Z(unsigned long* p0, int p1, int p2, int p3, int p4, unsigned long p5) {}

// Symbol: ?SmartMixColors@CDrawingManager@@SAKKKNHH@Z
extern "C" unsigned long MS_ABI impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(unsigned long p0, unsigned long p1, double p2, int p3, int p4) {
    return 0;
}
