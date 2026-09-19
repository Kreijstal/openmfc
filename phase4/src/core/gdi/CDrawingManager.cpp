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

//=============================================================================
// CDrawingManager -- the feature-pack GDI helper.
//
// Every body below was transcribed from the retail mfc140u.dll export
// (disassembled with the campaign's disas.py --u; RVAs quoted are mfc140u
// function ENTRY points).  Retail's object is 0x10 bytes: { vptr, CDC& m_dc }
// (constructor 0x56890: `mov %rdx,0x8(%rcx); lea vftable,%rax; mov %rax,(%rcx)`).
//
// Conventions shared by the GDI bodies:
//  * Retail wraps every scratch DC / bitmap in stack CDC / CBitmap objects
//    (CDC::Attach + ~CDC == DeleteDC(Detach()) at 0x2a2560; CGdiObject::~ ==
//    DeleteObject at 0x1c6f0).  Inside this DLL those C++ members exist only as
//    export thunks, so the same handles are held as raw HDC / HBITMAP and the
//    same ::DeleteDC / ::DeleteObject calls are made at the same points.
//  * Retail's `CDC::SelectObject(CGdiObject*)` / `SelectStockObject` on the scratch
//    DC (non-exported 0x2a2730 / 0x2a26d0, mfc140u) call ::SelectObject on m_hDC
//    only when m_hDC != m_hAttribDC, then on m_hAttribDC whenever it is non-NULL,
//    and return FromHandle() of the last result; an Attach()ed DC has both equal,
//    so they reduce to one ::SelectObject on that HDC, which is what is written here.
//  * `m_dc.GetSafeHdc()` is inlined by retail as `this ? m_hDC : NULL`; the
//    SafeHdc() helper reproduces it.  Where retail reads m_dc.m_hDC without the
//    null test the code here does the same.
//  * Retail's ENSURE(pBmpOld != NULL) after selecting the scratch bitmap is a
//    call to AfxThrowInvalidArgException (0x227720); retail's SEH unwind then
//    runs the scratch CDC/CBitmap destructors.  Here the handles are released
//    explicitly before the throw (same GDI calls, no unwinder needed).
//  * afxGlobalData: OpenMFC exports it as a zero-filled 720-byte blob whose
//    Initialize() is a no-op (core/runtime/AFX_GLOBAL_DATA.cpp), so the two
//    fields retail reads here -- m_nBitsPerPixel (+0x288) and clrBtnHilite
//    (+0x30) -- are recomputed from ::GetDeviceCaps / ::GetSysColor, the same
//    substitution the visual-manager files make.  Retail's lazy-init gate
//    (`if (afxGlobalData.<+0> == 0) { Initialize(); <+0> = 1; }`) is kept.
//=============================================================================

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>

// Retail's `?PixelAlpha@CDrawingManager@@SAKKH@Z` is defined above; the sibling
// thunks below are defined later in this file or in other translation units.
// All parameter lists are derived from the mangled names.
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject* pThis);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];             // 0x1803c1620 (mfc140u)
extern "C" std::int32_t impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA;              // 0x1803be394 (mfc140u)
extern "C" void MS_ABI impl__FillDitheredRect_CMFCToolBarImages__SAXPEAVCDC__AEBVCRect___Z(CDC* pDC, const CRect* pRect);
extern "C" int MS_ABI impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(const CDC* pThis, RECT* lpRect);
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject);
extern "C" HBITMAP MS_ABI impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(const CSize* pSize, void** pBits);
extern "C" unsigned long MS_ABI impl__HLStoRGB_ONE_CDrawingManager__SAKNNN_Z(double H, double L, double S);
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(unsigned long srcPixel, double dR, double dG, double dB);
extern "C" void MS_ABI impl__RGBtoHSV_CDrawingManager__SAXKPEAN00_Z(unsigned long rgb, double* pH, double* pS, double* pV);
extern "C" unsigned long MS_ABI impl__HSVtoRGB_CDrawingManager__SAKNNN_Z(double H, double S, double V);
extern "C" void MS_ABI impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(void* pThis, CDC* pDstDC, const CRect* pRectDst, CDC* pSrcDC, const CRect* pRectSrc);
extern "C" void MS_ABI impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish, int bHorz, int nStartFlatPercentage, int nEndFlatPercentage);

namespace {

// Shadow of the retail object.  There is no detail/CDrawingManagerSupport.h and
// no C++ declaration of the class in include/openmfc, so the layout is pinned
// here from the constructor body (0x56890, mfc140u).
struct S_DrawingManager {
    const void* vptr;   // +0x00  CDrawingManager vftable (0x1802e7188 in mfc140u)
    CDC*        pDC;    // +0x08  CDC& m_dc
};
static_assert(sizeof(S_DrawingManager) == 0x10, "CDrawingManager is 0x10 bytes in retail");
static_assert(offsetof(S_DrawingManager, pDC) == 0x8, "CDrawingManager::m_dc at +0x8");

// Head of a CDC as this DLL lays it out ({ vptr, m_hDC, m_hAttribDC }); used
// for the temporary CDC objects retail builds on its stack around a scratch HDC
// (CDC::CDC() exists only as an export thunk inside the DLL).
struct S_CDCHead {
    const void* vptr;
    HDC m_hDC;
    HDC m_hAttribDC;
};
static_assert(offsetof(CDC, m_hDC) == 0x8, "CDC::m_hDC at +0x8");
static_assert(offsetof(CDC, m_hAttribDC) == 0x10, "CDC::m_hAttribDC at +0x10");
static_assert(offsetof(S_CDCHead, m_hDC) == 0x8 && offsetof(S_CDCHead, m_hAttribDC) == 0x10, "S_CDCHead mirrors CDC");
inline CDC* AsCDC(S_CDCHead* p) { return reinterpret_cast<CDC*>(p); }

// OpenMFC's CRect is four ints and not derived from RECT; the Win32 calls take it as a RECT.
static_assert(sizeof(CRect) == sizeof(RECT) && offsetof(CRect, bottom) == offsetof(RECT, bottom), "CRect mirrors RECT");
inline const RECT* AsRECT(const CRect* p) { return reinterpret_cast<const RECT*>(p); }
inline RECT* AsRECT(CRect* p) { return reinterpret_cast<RECT*>(p); }

inline S_DrawingManager* DM(void* pThis) { return static_cast<S_DrawingManager*>(pThis); }
inline CDC* DmDC(void* pThis) { return DM(pThis)->pDC; }
// CDC::GetSafeHdc() as retail inlines it: `this == NULL ? NULL : m_hDC`.
inline HDC SafeHdc(CDC* pDC) { return pDC ? pDC->m_hDC : nullptr; }
// m_dc.m_hDC read without the null test (retail does this at several sites).
inline HDC DcHdc(void* pThis) { return DmDC(pThis)->m_hDC; }

// COLORREF (0x00BBGGRR) -> 32-bpp DIB pixel (0x00RRGGBB), the byte swap retail
// writes as `(c & 0xff00) | ((c >> 16) & 0xff) | ((c & 0xff) << 16)`.
inline DWORD DibSwap(COLORREF c)
{
    return (c & 0xff00u) | ((c >> 16) & 0xffu) | ((c & 0xffu) << 16);
}
// Same with an opaque alpha byte: retail computes `((c & 0xff) | 0xffffff00) << 16`
// which is 0xFF000000 | (R << 16) once truncated to 32 bits.
inline DWORD DibOpaque(COLORREF c)
{
    return 0xff000000u | DibSwap(c);
}

// The afxGlobalData lazy-init gate as every retail reader inlines it
// (HighlightRect 0x56958..0x5696d, GrayRect 0x57386..0x5739b).
inline void EnsureGlobalDataInitialized()
{
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
}
// afxGlobalData.m_nBitsPerPixel (+0x288) -- recomputed, see the file header.
inline int GD_BitsPerPixel()
{
    HDC hdcScreen = ::GetDC(nullptr);
    if (hdcScreen == nullptr) return 32;
    int bpp = ::GetDeviceCaps(hdcScreen, BITSPIXEL);
    ::ReleaseDC(nullptr, hdcScreen);
    return bpp;
}
// afxGlobalData.clrBtnHilite (+0x30, bytes 0x1803c1650..52 in mfc140u) --
// UpdateSysColors stores ::GetSysColor(COLOR_BTNHIGHLIGHT) there.
inline COLORREF GD_clrBtnHilite() { return ::GetSysColor(COLOR_BTNHIGHLIGHT); }

// msimg32!AlphaBlend.  Retail reaches it through a delay-load import slot
// (0x1803e9020 in mfc140u, MSIMG32.dll / AlphaBlend per the delay-import
// directory); OpenMFC does not link msimg32, so it is resolved lazily the same way.
typedef BOOL (WINAPI* PFN_AlphaBlend)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
PFN_AlphaBlend GetAlphaBlend()
{
    static PFN_AlphaBlend s_pfn = nullptr;
    static bool s_bTried = false;
    if (!s_bTried) {
        s_bTried = true;
        HMODULE h = ::LoadLibraryW(L"msimg32.dll");
        if (h != nullptr)
            s_pfn = reinterpret_cast<PFN_AlphaBlend>(reinterpret_cast<void*>(::GetProcAddress(h, "AlphaBlend")));
    }
    return s_pfn;
}

// CDrawingManager's MSVC-layout vtable.  Retail (0x1802e7188, mfc140u) holds:
//   [0] CObject::GetRuntimeClass (0x37a0)        [1] vector deleting destructor (0x568b0)
//   [2] [3] [4] one shared `ret` (0x27d0) -- Serialize / AssertValid / Dump are
//   empty in the retail build.
void* MS_ABI slot_DrawingManager_GetRuntimeClass(const void* pThis)
{
    return impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(static_cast<const CObject*>(pThis));
}
void MS_ABI slot_DrawingManager_Empty(void*) {}
void* MS_ABI slot_DrawingManager_VecDelDtor(void* pThis, unsigned flags);

void* const g_DrawingManager_vtbl[5] = {
    (void*)&slot_DrawingManager_GetRuntimeClass,
    (void*)&slot_DrawingManager_VecDelDtor,
    (void*)&slot_DrawingManager_Empty,
    (void*)&slot_DrawingManager_Empty,
    (void*)&slot_DrawingManager_Empty,
};

// Retail 0x568b0: reset the vptr, then, when flags & 1, release the storage:
// flags & 4 calls 0x27d0 (which is a bare `ret` in this build) and otherwise
// the CRT free() import (0x1802c74e8).  Transcribed as such.
void* MS_ABI slot_DrawingManager_VecDelDtor(void* pThis, unsigned flags)
{
    DM(pThis)->vptr = g_DrawingManager_vtbl;
    if ((flags & 1) && !(flags & 4))
        std::free(pThis);
    return pThis;
}

} // namespace

// CDrawingManager::CDrawingManager(CDC& dc) -- RVA 0x56890 (mfc140u):
//     m_dc = dc; vptr = vftable; return this.
// Symbol: ??0CDrawingManager@@QEAA@AEAVCDC@@@Z
extern "C" void* MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC) {
    DM(pThis)->pDC = static_cast<CDC*>(pDC);
    DM(pThis)->vptr = g_DrawingManager_vtbl;
    return pThis;
}
// CDrawingManager::~CDrawingManager() -- RVA 0x568f0 (mfc140u): only re-installs
// the vptr; nothing is owned.
// Symbol: ??1CDrawingManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis) {
    DM(pThis)->vptr = g_DrawingManager_vtbl;
}

//-----------------------------------------------------------------------------
// Colour-space statics
//-----------------------------------------------------------------------------

// static COLORREF CDrawingManager::HLStoRGB_ONE(double H, double L, double S)
// RVA 0x5b650 (mfc140u).  Hue is a turn fraction in [0,1].
//     S == 0                     -> (L, L, L)                          (0x5b686)
//     L <= 0.5                   -> m2 = (S + 1) * L                   (0x5b6a1)
//     L == 1.0                   -> m2 = L                             (0x5b6bd)
//     else                       -> m2 = L + S - L*S                   (0x5b6c3)
//     m1 = 2L - m2
//     R = HuetoRGB(m1, m2, H + 1/3), G = HuetoRGB(m1, m2, H), B = HuetoRGB(m1, m2, H - 1/3)
//                                                              (calls at 0x5b6f5 / 0x5b709 / 0x5b726)
// each channel (int)(c * 255) truncated to a byte, packed B<<16 | G<<8 | R.
// Symbol: ?HLStoRGB_ONE@CDrawingManager@@SAKNNN@Z
extern "C" unsigned long MS_ABI impl__HLStoRGB_ONE_CDrawingManager__SAKNNN_Z(double H, double L, double S) {
    double R, G, B;
    if (S == 0.0) {
        R = G = B = L;
    } else {
        double m2;
        if (L <= 0.5)       m2 = (S + 1.0) * L;
        else if (L == 1.0)  m2 = L;
        else                m2 = L + S - L * S;
        const double m1 = 2.0 * L - m2;
        R = impl__HuetoRGB_CDrawingManager__SANNNN_Z(m1, m2, H + 1.0 / 3.0);
        G = impl__HuetoRGB_CDrawingManager__SANNNN_Z(m1, m2, H);
        B = impl__HuetoRGB_CDrawingManager__SANNNN_Z(m1, m2, H - 1.0 / 3.0);
    }
    const unsigned r = (unsigned)(int)(R * 255.0) & 0xffu;
    const unsigned g = (unsigned)(int)(G * 255.0) & 0xffu;
    const unsigned b = (unsigned)(int)(B * 255.0) & 0xffu;
    return (b << 16) | (g << 8) | r;
}

// static COLORREF CDrawingManager::HLStoRGB_TWO(double H, double L, double S)
// RVA 0x5b790 (mfc140u).  Hue in DEGREES; the float HueToRGB (0x5b5d0) is used.
//     S == 0   -> v = (BYTE)(int)(L * 255); (v, v, v)                    (0x5b7bd)
//     m2 (float) = L <= 0.5 ? (float)(L*S + L) : L == 1.0 ? (float)L : (float)(L + S - L*S)
//     m1 (float) = (float)(2L - (double)m2)
//     B = HueToRGB(m1, m2, (float)(H - 120)), G = HueToRGB(m1, m2, (float)H),
//     R = HueToRGB(m1, m2, (float)(H + 120))     (calls at 0x5b850 / 0x5b866 / 0x5b886)
// packed B<<16 | G<<8 | R.
// Symbol: ?HLStoRGB_TWO@CDrawingManager@@SAKNNN@Z
extern "C" unsigned long MS_ABI impl__HLStoRGB_TWO_CDrawingManager__SAKNNN_Z(double H, double L, double S) {
    unsigned r, g, b;
    if (S == 0.0) {
        const unsigned v = (unsigned)(int)(L * 255.0) & 0xffu;
        r = g = b = v;
    } else {
        float m2;
        if (L <= 0.5)       m2 = (float)(L * S + L);
        else if (L == 1.0)  m2 = (float)L;
        else                m2 = (float)(L + S - L * S);
        const float m1 = (float)(2.0 * L - (double)m2);
        b = impl__HueToRGB_CDrawingManager__SAEMMM_Z(m1, m2, (float)(H - 120.0));
        g = impl__HueToRGB_CDrawingManager__SAEMMM_Z(m1, m2, (float)H);
        r = impl__HueToRGB_CDrawingManager__SAEMMM_Z(m1, m2, (float)(H + 120.0));
    }
    return ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu);
}

// static COLORREF CDrawingManager::HSVtoRGB(double H, double S, double V)
// RVA 0x5bb50 (mfc140u).
//     S == 0 -> (V, V, V)                                             (0x5bb7a)
//     h = H / 60; i = (int)floor(h) (crt floor via IAT 0x1802c7538); f = h - i
//     p = (1 - S) * V; q = (1 - f*S) * V; t = (1 - (1-f)*S) * V
//     switch (i) { 0:(V,t,p) 1:(q,V,p) 2:(p,V,t) 3:(p,q,V) 4:(t,p,V) default:(V,p,q) }
// each channel (int)(c * 255 + 0.5) truncated to a byte (0x5bc46..0x5bc80).
// Symbol: ?HSVtoRGB@CDrawingManager@@SAKNNN@Z
extern "C" unsigned long MS_ABI impl__HSVtoRGB_CDrawingManager__SAKNNN_Z(double H, double S, double V) {
    double R, G, B;
    if (S == 0.0) {
        R = G = B = V;
    } else {
        const double h = H / 60.0;
        const int i = (int)std::floor(h);
        const double f = h - (double)i;
        const double p = (1.0 - S) * V;
        const double q = (1.0 - f * S) * V;
        const double t = (1.0 - (1.0 - f) * S) * V;
        switch (i) {
        case 0:  R = V; G = t; B = p; break;
        case 1:  R = q; G = V; B = p; break;
        case 2:  R = p; G = V; B = t; break;
        case 3:  R = p; G = q; B = V; break;
        case 4:  R = t; G = p; B = V; break;
        default: R = V; G = p; B = q; break;
        }
    }
    const unsigned r = (unsigned)(int)(R * 255.0 + 0.5) & 0xffu;
    const unsigned g = (unsigned)(int)(G * 255.0 + 0.5) & 0xffu;
    const unsigned b = (unsigned)(int)(B * 255.0 + 0.5) & 0xffu;
    return (b << 16) | (g << 8) | r;
}

// static void CDrawingManager::RGBtoHSV(COLORREF rgb, double* H, double* S, double* V)
// RVA 0x5ba20 (mfc140u; not in the RVA map -- resolved through the export
// directory, ordinal 12598).  Channels scaled to [0,1]; *V = max is stored
// before any test (0x5ba8f).
//     max == 0        -> *S = 0, *H = -1, return                      (0x5ba98..0x5baa9)
//     *S = delta / max                                                 (0x5bac3)
//     delta == 0      -> h = 1.0                                       (0x5bacc)
//     r == max        -> h = (g - b) / delta
//     g == max        -> h = (b - r) / delta + 2
//     else            -> h = (r - g) / delta + 4
//     h *= 60; *H = h; if (h < 0) *H = h + 360                          (0x5bb18..0x5bb32)
// No null checks on the out-pointers.
// Symbol: ?RGBtoHSV@CDrawingManager@@SAXKPEAN00@Z
extern "C" void MS_ABI impl__RGBtoHSV_CDrawingManager__SAXKPEAN00_Z(unsigned long rgb, double* pH, double* pS, double* pV) {
    const double r = (double)(rgb & 0xffu) / 255.0;
    const double g = (double)((rgb >> 8) & 0xffu) / 255.0;
    const double b = (double)((rgb >> 16) & 0xffu) / 255.0;
    double mx = g > b ? g : b; if (r > mx) mx = r;
    double mn = g < b ? g : b; if (r < mn) mn = r;
    *pV = mx;
    if (mx == 0.0) {
        *pS = 0.0;
        *pH = -1.0;
        return;
    }
    const double delta = mx - mn;
    *pS = delta / mx;
    double h;
    if (delta == 0.0)   h = 1.0;
    else if (r == mx)   h = (g - b) / delta;
    else if (g == mx)   h = (b - r) / delta + 2.0;
    else                h = (r - g) / delta + 4.0;
    h *= 60.0;
    *pH = h;
    if (h < 0.0) *pH = h + 360.0;
}

// static COLORREF CDrawingManager::PixelAlpha(COLORREF src, double dR, double dG, double dB)
// RVA 0x5b310 (mfc140u; export directory, ordinal 11737).  Per channel:
//     v = (int)(ch * k + 0.5)
//     if (ch == 0 && k > 1.0) v = (int)((k - 1.0) * 255 + 0.5)         (0x5b33b..0x5b355)
//     byte = v < 255 ? (BYTE)v : 255                                    (cmp/cmovl 0x5b363)
// Symbol: ?PixelAlpha@CDrawingManager@@SAKKNNN@Z
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(unsigned long srcPixel, double dR, double dG, double dB) {
    auto chan = [](unsigned ch, double k) -> unsigned {
        int v = (int)((double)ch * k + 0.5);
        if (ch == 0 && k > 1.0) v = (int)((k - 1.0) * 255.0 + 0.5);
        return (v < 255) ? ((unsigned)v & 0xffu) : 255u;
    };
    const unsigned r = chan(srcPixel & 0xffu, dR);
    const unsigned g = chan((srcPixel >> 8) & 0xffu, dG);
    const unsigned b = chan((srcPixel >> 16) & 0xffu, dB);
    return (b << 16) | (g << 8) | r;
}

// static COLORREF CDrawingManager::SmartMixColors(COLORREF color1, COLORREF color2,
//                                               double dblLumRatio, int k1, int k2)
// RVA 0x5bc90 (mfc140u; export directory, ordinal 13882).
//     k1 < 0 || k2 < 0  -> AfxThrowInvalidArgException (0x5bdef -> 0x227720)
//     k1 + k2 == 0      -> return 0                                     (0x5bccc)
//     mix = per channel (c1*k1 + c2*k2) / (k1+k2), signed idiv         (0x5bce7..0x5bd4c)
//     RGBtoHSV(mix, &h1, &s1, &v); RGBtoHSV(color2, &h2, &s2, &v)     (0x5bd52 / 0x5bd66;
//                                   both calls pass the same &v slot, so v is color2's)
//     rgb = HSVtoRGB(h1, (k2*s2 + k1*s1) / (k1+k2), v)                 (0x5bd9f)
//     dblLumRatio == 1.0 -> return rgb                                 (0x5bdb2)
//     RGBtoHSL(rgb, &h, &s, &l); return HLStoRGB_ONE(h, min(1.0, l*ratio), s)  (0x5bdc6 / 0x5bde5)
// Symbol: ?SmartMixColors@CDrawingManager@@SAKKKNHH@Z
extern "C" unsigned long MS_ABI impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(unsigned long color1, unsigned long color2, double dblLumRatio, int k1, int k2) {
    if (k1 < 0 || k2 < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    const int k = k1 + k2;
    if (k == 0) return 0;
    const int r = ((int)(color1 & 0xffu) * k1 + (int)(color2 & 0xffu) * k2) / k;
    const int g = ((int)((color1 >> 8) & 0xffu) * k1 + (int)((color2 >> 8) & 0xffu) * k2) / k;
    const int b = ((int)((color1 >> 16) & 0xffu) * k1 + (int)((color2 >> 16) & 0xffu) * k2) / k;
    const unsigned long mix = ((unsigned)b & 0xffu) << 16 | ((unsigned)g & 0xffu) << 8 | ((unsigned)r & 0xffu);
    double h1 = 0.0, s1 = 0.0, h2 = 0.0, s2 = 0.0, v = 0.0;
    impl__RGBtoHSV_CDrawingManager__SAXKPEAN00_Z(mix, &h1, &s1, &v);
    impl__RGBtoHSV_CDrawingManager__SAXKPEAN00_Z(color2, &h2, &s2, &v);
    const double s = ((double)k2 * s2 + (double)k1 * s1) / (double)k;
    const unsigned long rgb = impl__HSVtoRGB_CDrawingManager__SAKNNN_Z(h1, s, v);
    if (dblLumRatio == 1.0) return rgb;
    double h = 0.0, sat = 0.0, l = 0.0;
    impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(rgb, &h, &sat, &l);
    l *= dblLumRatio;
    if (l > 1.0) l = 1.0;   // minsd(1.0, l)
    return impl__HLStoRGB_ONE_CDrawingManager__SAKNNN_Z(h, l, sat);
}

// static void CDrawingManager::SetPixel(COLORREF* pBits, int cx, int cy, int x, int y, COLORREF color)
// RVA 0x5b4d0 (mfc140u; export directory, ordinal 13472):
//     pBits[(cy - y) * cx + x] = (R << 16) | (G << 8) | B    (alpha byte 0; 64-bit index)
// Symbol: ?SetPixel@CDrawingManager@@SAXPEAKHHHHK@Z
extern "C" void MS_ABI impl__SetPixel_CDrawingManager__SAXPEAKHHHHK_Z(unsigned long* pBits, int cx, int cy, int x, int y, unsigned long color) {
    const long long idx = (long long)((cy - y) * cx) + (long long)x;
    pBits[idx] = DibSwap(color);
}

//-----------------------------------------------------------------------------
// DIB helpers
//-----------------------------------------------------------------------------

// static HBITMAP CDrawingManager::CreateBitmap_32(const CSize& size, LPVOID* pBits)
// RVA 0x56580 (mfc140u):
//     if (pBits) *pBits = NULL;
//     if (size.cx <= 0 || size.cy == 0) return NULL;                    (0x565b4..0x565bd)
//     BITMAPINFO bi = {}; biSize = sizeof(BITMAPINFOHEADER); biWidth = cx; biHeight = cy;
//     biPlanes = 1; biBitCount = 32; biCompression = BI_RGB; biSizeImage = cx * labs(cy);
//     hbmp = ::CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, &pData, NULL, 0);   (0x56617)
//     if (pData && hbmp && pBits) *pBits = pData;                        (0x56621..0x56630)
//     return hbmp;
// Symbol: ?CreateBitmap_32@CDrawingManager@@SAPEAUHBITMAP__@@AEBVCSize@@PEAPEAX@Z
extern "C" HBITMAP MS_ABI impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(const CSize* pSize, void** pBits) {
    if (pBits != nullptr) *pBits = nullptr;
    if (pSize->cx <= 0 || pSize->cy == 0) return nullptr;

    BITMAPINFO bi;
    std::memset(&bi, 0, sizeof bi);
    bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth       = pSize->cx;
    bi.bmiHeader.biHeight      = pSize->cy;
    bi.bmiHeader.biPlanes      = 1;
    bi.bmiHeader.biBitCount    = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage   = (DWORD)(pSize->cx * std::labs((long)pSize->cy));

    void* pData = nullptr;
    HBITMAP hbmp = ::CreateDIBSection(nullptr, &bi, DIB_RGB_COLORS, &pData, nullptr, 0);
    if (pData != nullptr && hbmp != nullptr && pBits != nullptr)
        *pBits = pData;
    return hbmp;
}

// static HBITMAP CDrawingManager::CreateBitmap_32(HBITMAP hBitmap, COLORREF clrTransparent)
// RVA 0x56660 (mfc140u):
//     if (!hBitmap) return NULL;
//     BITMAP bmp; if (!::GetObject(hBitmap, sizeof bmp, &bmp) || bmp.bmBits == NULL) return NULL;
//     hNew = CreateBitmap_32(CSize(bmWidth, bmHeight), &pBits); nHeight = abs(bmHeight);
//     if (!hNew) return NULL;
//     nCount = nHeight * bmWidth (unsigned loops)
//     bmBitsPixel == 32 -> memcpy(pBits, bmBits, nCount*4); return hNew   (0x566f0..0x5670d)
//     dcSrc = CreateCompatibleDC(NULL); hOldSrc = SelectObject(dcSrc, hBitmap);
//     if (!hOldSrc) { DeleteDC(dcSrc); return hNew; }                     (0x56753 -> 0x56858)
//     dcDst = CreateCompatibleDC(NULL); hOldDst = SelectObject(dcDst, hNew);
//     BitBlt(dcDst, 0, 0, bmWidth, nHeight, dcSrc, 0, 0, SRCCOPY);
//     if (hOldDst) SelectObject(dcDst, hOldDst); SelectObject(dcSrc, hOldSrc);
//     clrTransparent == -1 -> every pixel |= 0xFF000000                    (0x567f7)
//     else key = DibSwap(clrTransparent); pixel == key ? 0 : pixel | 0xFF000000   (0x56830..0x56840)
//     DeleteDC(dcDst); DeleteDC(dcSrc); return hNew;
// Symbol: ?CreateBitmap_32@CDrawingManager@@SAPEAUHBITMAP__@@PEAU2@K@Z
extern "C" HBITMAP MS_ABI impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____PEAU2_K_Z(HBITMAP hBitmap, unsigned long clrTransparent) {
    if (hBitmap == nullptr) return nullptr;

    BITMAP bmp;
    if (!::GetObject(hBitmap, sizeof(BITMAP), &bmp)) return nullptr;
    if (bmp.bmBits == nullptr) return nullptr;

    void* pBits = nullptr;
    CSize size(bmp.bmWidth, bmp.bmHeight);
    HBITMAP hNew = impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(&size, &pBits);
    const int nHeight = std::abs((int)bmp.bmHeight);
    if (hNew == nullptr) return nullptr;

    const unsigned nCount = (unsigned)(nHeight * bmp.bmWidth);
    if (bmp.bmBitsPixel == 32) {
        std::memcpy(pBits, bmp.bmBits, (size_t)nCount * 4);
        return hNew;
    }

    HDC dcSrc = ::CreateCompatibleDC(nullptr);
    HGDIOBJ hOldSrc = ::SelectObject(dcSrc, hBitmap);
    if (hOldSrc == nullptr) {
        if (dcSrc) ::DeleteDC(dcSrc);
        return hNew;
    }
    HDC dcDst = ::CreateCompatibleDC(nullptr);
    HGDIOBJ hOldDst = ::SelectObject(dcDst, hNew);
    ::BitBlt(dcDst, 0, 0, bmp.bmWidth, nHeight, dcSrc, 0, 0, SRCCOPY);
    if (hOldDst != nullptr) ::SelectObject(dcDst, hOldDst);
    ::SelectObject(dcSrc, hOldSrc);

    DWORD* p = static_cast<DWORD*>(pBits);
    if (clrTransparent == (unsigned long)-1) {
        for (unsigned i = 0; i < nCount; ++i)
            p[i] |= 0xff000000u;
    } else {
        const DWORD key = DibSwap(clrTransparent);
        for (unsigned i = 0; i < nCount; ++i) {
            DWORD v = p[i] | 0xff000000u;
            if (p[i] == key) v = 0;
            p[i] = v;
        }
    }
    if (dcDst) ::DeleteDC(dcDst);
    if (dcSrc) ::DeleteDC(dcSrc);
    return hNew;
}

// static HBITMAP CDrawingManager::PrepareShadowMask(int nDepth, COLORREF clrBase,
//                                                  int iMinBrightness, int iMaxBrightness)
// RVA 0x5beb0 (mfc140u; export directory, ordinal 11829).
//     nDepth == 0 -> NULL
//     nSize = 2 * max(nDepth, 3) + 1; hDib = CreateBitmap_32(CSize(nSize, nSize), &pBits)
//     if (!hDib || !pBits) return NULL
//     pAlpha = new double[nSize*nSize] (operator new 0x27f0, freed with free() at 0x5c142)
//     max = iMaxBrightness/100.0, min = iMinBrightness/100.0, range = max - min,
//     limit = nCount * 0.25, nHalf = nSize / 2
//     for y in [-nHalf, nHalf] for x in [-nHalf, nHalf]:                         (0x5bfd6..0x5c09a)
//        d = x*x + y*y (as doubles); if (d > limit) a = 0                     (0x5bffa)
//        else a = exp(-2 * d / limit) * range + min, then a = max(a, min), a = min(a, max)
//             (retail recomputes the same exp() expression on each compare arm)
//     bytes r,g,b = channel >> 2 of clrBase                                     (0x5c0bf..0x5c0c5)
//     per pixel: [2] = (BYTE)(int)(r * a), [1] = (BYTE)(int)(g * a), [0] = (BYTE)(int)(b * a),
//                [3] = (BYTE)(int)(a * 255)                                    (0x5c0ee..0x5c132)
// Symbol: ?PrepareShadowMask@CDrawingManager@@SAPEAUHBITMAP__@@HKHH@Z
extern "C" HBITMAP MS_ABI impl__PrepareShadowMask_CDrawingManager__SAPEAUHBITMAP____HKHH_Z(int nDepth, unsigned long clrBase, int iMinBrightness, int iMaxBrightness) {
    if (nDepth == 0) return nullptr;
    const int nDepth2 = nDepth < 3 ? 3 : nDepth;
    const int nSize = nDepth2 * 2 + 1;

    void* pBits = nullptr;
    CSize size(nSize, nSize);
    HBITMAP hDib = impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(&size, &pBits);
    if (hDib == nullptr || pBits == nullptr) return nullptr;

    const int nCount = nSize * nSize;
    // retail: operator new (0x27f0) of nCount*8 bytes, released with free() at 0x5c142
    double* pAlpha = static_cast<double*>(std::malloc((size_t)nCount * sizeof(double)));
    if (pAlpha == nullptr) {   // DEVIATION: retail's operator new throws CMemoryException here
        ::DeleteObject(hDib);
        return nullptr;
    }

    const double dMax   = (double)iMaxBrightness / 100.0;
    const double dMin   = (double)iMinBrightness / 100.0;
    const double dRange = dMax - dMin;
    const double dLimit = (double)nCount * 0.25;
    const int nHalf = nSize / 2;

    double* p = pAlpha;
    for (int y = -nHalf; y <= nHalf; ++y) {
        const double y2 = (double)(y * y);
        for (int x = -nHalf; x <= nHalf; ++x) {
            const double d = (double)(x * x) + y2;
            double a;
            if (d > dLimit) {
                a = 0.0;
            } else {
                const double t = -(d / dLimit) * 2.0;
                a = std::exp(t) * dRange + dMin;
                if (!(a > dMin)) a = dMin;
                if (a > dMax)    a = dMax;
            }
            *p++ = a;
        }
    }

    const BYTE r = (BYTE)((clrBase & 0xffu) >> 2);
    const BYTE g = (BYTE)(((clrBase >> 8) & 0xffu) >> 2);
    const BYTE b = (BYTE)(((clrBase >> 16) & 0xffu) >> 2);
    BYTE* pPix = static_cast<BYTE*>(pBits);
    for (int i = 0; i < nCount; ++i, pPix += 4) {
        const double a = pAlpha[i];
        pPix[2] = (BYTE)(int)((double)r * a);
        pPix[1] = (BYTE)(int)((double)g * a);
        pPix[0] = (BYTE)(int)((double)b * a);
        pPix[3] = (BYTE)(int)(a * 255.0);
    }
    std::free(pAlpha);
    return hDib;
}

//-----------------------------------------------------------------------------
// Blitting members
//-----------------------------------------------------------------------------

// void CDrawingManager::DrawAlpha(CDC* pDstDC, const CRect& rectDst, CDC* pSrcDC, const CRect& rectSrc)
// RVA 0x5be00 (mfc140u).  `this` is unused.
//     BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }   (the DWORD 0x01ff0000 at 0x5be46)
//     AlphaBlend(pDstDC->m_hDC, rectDst.left, rectDst.top, rectDst.Width(), rectDst.Height(),
//                pSrcDC ? pSrcDC->m_hDC : NULL, rectSrc.left, rectSrc.top,
//                rectSrc.Width(), rectSrc.Height(), bf)          (delay-load slot 0x1803e9020)
// pDstDC is dereferenced without a null test.
// Symbol: ?DrawAlpha@CDrawingManager@@QEAAXPEAVCDC@@AEBVCRect@@01@Z
extern "C" void MS_ABI impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(void* pThis, CDC* pDstDC, const CRect* pRectDst, CDC* pSrcDC, const CRect* pRectSrc) {
    (void)pThis;
    BLENDFUNCTION bf;
    bf.BlendOp             = AC_SRC_OVER;
    bf.BlendFlags          = 0;
    bf.SourceConstantAlpha = 0xff;
    bf.AlphaFormat         = AC_SRC_ALPHA;
    PFN_AlphaBlend pfn = GetAlphaBlend();
    if (pfn == nullptr) return;   // DEVIATION: retail's delay-load helper would raise; there is nothing to draw with
    pfn(pDstDC->m_hDC, pRectDst->left, pRectDst->top,
        pRectDst->right - pRectDst->left, pRectDst->bottom - pRectDst->top,
        SafeHdc(pSrcDC), pRectSrc->left, pRectSrc->top,
        pRectSrc->right - pRectSrc->left, pRectSrc->bottom - pRectSrc->top, bf);
}

// void CDrawingManager::FillAlpha(const CRect& rect, BYTE bValue)
// RVA 0x58340 (mfc140u):
//     cx = Width, cy = Height; hDib = CreateBitmap_32(CSize(cx, cy), &pBits)
//     if (!hDib || !pBits) return                                          (0x58391 / 0x5839e)
//     dcMem.Attach(CreateCompatibleDC(m_dc.GetSafeHdc())); hOld = SelectObject(dcMem, hDib)
//     BitBlt(dcMem, 0, 0, cx, cy, m_dc.GetSafeHdc(), rect.left, rect.top, SRCCOPY)   (0x58429)
//     for i in [0, cx*cy): pBits[i] |= bValue << 24                        (0x58441)
//     BitBlt(m_dc.m_hDC, rect.left, rect.top, cx, cy, dcMem, 0, 0, SRCCOPY)  (0x58484; no null test)
//     if (hOld) SelectObject(dcMem, hOld); DeleteObject(hDib); ~dcMem
// Symbol: ?FillAlpha@CDrawingManager@@QEAAXAEBVCRect@@E@Z
extern "C" void MS_ABI impl__FillAlpha_CDrawingManager__QEAAXAEBVCRect__E_Z(void* pThis, const CRect* pRect, unsigned char bValue) {
    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    void* pBits = nullptr;
    CSize size(cx, cy);
    HBITMAP hDib = impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(&size, &pBits);
    if (hDib == nullptr || pBits == nullptr) return;

    HDC dcMem = ::CreateCompatibleDC(SafeHdc(DmDC(pThis)));
    HGDIOBJ hOld = ::SelectObject(dcMem, hDib);
    ::BitBlt(dcMem, 0, 0, cx, cy, SafeHdc(DmDC(pThis)), pRect->left, pRect->top, SRCCOPY);

    const DWORD alpha = (DWORD)bValue << 24;
    DWORD* p = static_cast<DWORD*>(pBits);
    const int n = cy * cx;
    for (int i = 0; i < n; ++i)
        p[i] |= alpha;

    ::BitBlt(DcHdc(pThis), pRect->left, pRect->top, cx, cy, dcMem, 0, 0, SRCCOPY);
    if (hOld != nullptr) ::SelectObject(dcMem, hOld);
    ::DeleteObject(hDib);
    if (dcMem != nullptr) ::DeleteDC(dcMem);
}

// void CDrawingManager::DrawRotated(CRect rectDest, CDC& dcSrc, BOOL bClockWise)
// RVA 0x5c190 (mfc140u):
//     w = Width; if (w <= 0) return; h = Height; if (h <= 0) return
//     for y in [0, h] (inclusive) for x in [0, w] (inclusive):               (0x5c1df / 0x5c1f5)
//        srcY = bClockWise ? (w - x - 1) : x                                (cmovne 0x5c20b)
//        c = ::GetPixel(dcSrc.m_hDC, y, srcY)                               (0x5c20f)
//        ::SetPixel(m_dc.m_hDC, rect.left + x, rect.top + y, c)             (0x5c223)
// Symbol: ?DrawRotated@CDrawingManager@@QEAAXVCRect@@AEAVCDC@@H@Z
extern "C" void MS_ABI impl__DrawRotated_CDrawingManager__QEAAXVCRect__AEAVCDC__H_Z(void* pThis, CRect* pRect, CDC* pSrcDC, int bClockWise) {
    const int w = pRect->right - pRect->left;
    if (w <= 0) return;
    const int h = pRect->bottom - pRect->top;
    if (h <= 0) return;
    for (int y = 0; y <= h; ++y) {
        for (int x = 0; x <= w; ++x) {
            const int srcY = bClockWise ? (w - x - 1) : x;
            const COLORREF c = ::GetPixel(pSrcDC->m_hDC, y, srcY);
            ::SetPixel(DcHdc(pThis), pRect->left + x, pRect->top + y, c);
        }
    }
}

// void CDrawingManager::Fill4ColorsGradient(CRect rect, COLORREF c1, COLORREF c2,
//                                          COLORREF c3, COLORREF c4, BOOL bHorz, int nPercentage)
// RVA 0x58240 (mfc140u).  Splits the rect at nPercentage (signed /100) and
// makes two FillGradient calls with flat percentages 0:
//     !bHorz: x = left + Width*pct/100;  r1 = {left,top,x,bottom}, r2 = {x,top,right,bottom}  (0x58273)
//      bHorz: y = top + Height*pct/100;  r1 = {left,top,right,y},  r2 = {left,y,right,bottom} (0x5829a)
//     FillGradient(r1, c1, c2, bHorz, 0, 0); FillGradient(r2, c3, c4, bHorz, 0, 0)  (0x582ea / 0x5832a)
// Symbol: ?Fill4ColorsGradient@CDrawingManager@@QEAAXVCRect@@KKKKHH@Z
extern "C" void MS_ABI impl__Fill4ColorsGradient_CDrawingManager__QEAAXVCRect__KKKKHH_Z(void* pThis, CRect* pRect, unsigned long c1, unsigned long c2, unsigned long c3, unsigned long c4, int bHorz, int nPercentage) {
    CRect r1(*pRect), r2(*pRect);
    if (!bHorz) {
        const int x = pRect->left + (pRect->right - pRect->left) * nPercentage / 100;
        r1.right = x;
        r2.left  = x;
    } else {
        const int y = pRect->top + (pRect->bottom - pRect->top) * nPercentage / 100;
        r1.bottom = y;
        r2.top    = y;
    }
    impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(pThis, &r1, c1, c2, bHorz, 0, 0);
    impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(pThis, &r2, c3, c4, bHorz, 0, 0);
}

namespace {

// The non-exported FillGradient worker at RVA 0x577bc (mfc140u), called by
// FillGradient on both of its paths.  Takes the rect by pointer and MUTATES it
// (the flat bands shrink it).  CBrush(COLORREF) / ~CBrush are CreateSolidBrush /
// DeleteObject here.  DEVIATION: retail's CBrush ctor (0x2a4060) throws
// CResourceException when CreateSolidBrush fails; here the NULL brush is passed
// to ::FillRect, which then fails, and the band is skipped.  Same in the
// colorStart == colorFinish fast path of FillGradient2 below.
//     colorStart == colorFinish -> one FillRect, done                       (0x577ea)
//     nStartFlat > 0: bHorz ? top band [top, top + h*pct/100) in colorFinish, rect.top = y
//                           : left band [left, left + w*pct/100) in colorStart, rect.left = x   (0x57829..0x578e4)
//     nEndFlat > 0:   bHorz ? bottom band [bottom - h*pct/100, bottom) in colorStart, rect.bottom = y
//                           : right band [right - w*pct/100, right) in colorFinish, rect.right = x (0x578e9..0x5799b)
//     nStartFlat + nEndFlat > 100 -> done                                   (0x579a4)
//     64 bands i = 0..63, channel = ((64-i)*start + i*finish) >> 6, byte-truncated (0x579e8..0x57a3b)
//        bHorz: band [bottom - (h*(i+1) >> 6), bottom - (h*i >> 6))   -- starts at the BOTTOM (0x57a53)
//        else : band [left + (w*i >> 6), left + (w*(i+1) >> 6))                             (0x57a83)
//        drawn only when non-empty (0x57aaa)
void FillGradientWorker(void* pThis, CRect* pRect, COLORREF colorStart, COLORREF colorFinish,
                        int bHorz, int nStartFlat, int nEndFlat)
{
    HDC hdc = DcHdc(pThis);
    if (colorStart == colorFinish) {
        HBRUSH br = ::CreateSolidBrush(colorStart);
        ::FillRect(hdc, AsRECT(pRect), br);
        if (br) ::DeleteObject(br);
        return;
    }
    if (nStartFlat > 0) {
        CRect r(*pRect);
        COLORREF clr;
        if (bHorz) {
            const int y = pRect->top + (pRect->bottom - pRect->top) * nStartFlat / 100;
            r.bottom = y; pRect->top = y; clr = colorFinish;
        } else {
            const int x = pRect->left + (pRect->right - pRect->left) * nStartFlat / 100;
            r.right = x; pRect->left = x; clr = colorStart;
        }
        HBRUSH br = ::CreateSolidBrush(clr);
        ::FillRect(hdc, AsRECT(&r), br);
        if (br) ::DeleteObject(br);
    }
    if (nEndFlat > 0) {
        CRect r(*pRect);
        COLORREF clr;
        if (bHorz) {
            const int y = pRect->bottom - (pRect->bottom - pRect->top) * nEndFlat / 100;
            r.top = y; pRect->bottom = y; clr = colorStart;
        } else {
            const int x = pRect->right - (pRect->right - pRect->left) * nEndFlat / 100;
            r.left = x; pRect->right = x; clr = colorFinish;
        }
        HBRUSH br = ::CreateSolidBrush(clr);
        ::FillRect(hdc, AsRECT(&r), br);
        if (br) ::DeleteObject(br);
    }
    if (nStartFlat + nEndFlat > 100) return;

    const int sR = (int)(colorStart & 0xffu), sG = (int)((colorStart >> 8) & 0xffu), sB = (int)((colorStart >> 16) & 0xffu);
    const int fR = (int)(colorFinish & 0xffu), fG = (int)((colorFinish >> 8) & 0xffu), fB = (int)((colorFinish >> 16) & 0xffu);
    for (int i = 0; i < 64; ++i) {
        const int w = 64 - i;
        const unsigned g = (unsigned)((w * sG + i * fG) >> 6) & 0xffu;
        const unsigned b = (unsigned)((w * sB + i * fB) >> 6) & 0xffu;
        const unsigned r = (unsigned)((w * sR + i * fR) >> 6) & 0xffu;
        const COLORREF clr = (b << 16) | (g << 8) | r;
        HBRUSH br = ::CreateSolidBrush(clr);
        CRect rc(*pRect);
        if (bHorz) {
            const int h = pRect->bottom - pRect->top;
            rc.bottom = pRect->bottom - ((h * i) >> 6);
            rc.top    = pRect->bottom - ((h * (i + 1)) >> 6);
            if (rc.bottom - rc.top > 0) ::FillRect(hdc, AsRECT(&rc), br);
        } else {
            const int cx = pRect->right - pRect->left;
            rc.right = pRect->left + ((cx * (i + 1)) >> 6);
            rc.left  = pRect->left + ((cx * i) >> 6);
            if (rc.right - rc.left > 0) ::FillRect(hdc, AsRECT(&rc), br);
        }
        if (br) ::DeleteObject(br);
    }
}

// Shared prologue of the DIB-backed members (HighlightRect, GrayRect, MirrorRect,
// FillGradient's glass path, FillGradient2, DrawLine, DrawLineA, DrawRect,
// DrawEllipse, DrawGradientRing, DrawShadow), which all open identically:
//     CDC dcMem;  if (!dcMem.Attach(::CreateCompatibleDC(m_dc.GetSafeHdc()))) -> fail
//     CBitmap bmpMem; if (!bmpMem.Attach(::CreateCompatibleBitmap(m_dc.m_hDC, cx, cy))) -> fail
//     CBitmap* pBmpOld = dcMem.SelectObject(&bmpMem); ENSURE(pBmpOld != NULL);
//     hDib = CreateBitmap_32(CSize(cx, cy), &pBits); if (!hDib || !pBits) -> fail
//     dcMem.SelectObject(hDib);
// and close with
//     dcMem.SelectObject(pBmpOld); ::DeleteObject(hDib); ~bmpMem (DeleteObject); ~dcMem (DeleteDC).
// CDC::Attach fails only for a NULL handle, CGdiObject::Attach likewise.
struct DibScratch {
    HDC     dcMem   = nullptr;
    HBITMAP bmpMem  = nullptr;
    HGDIOBJ hBmpOld = nullptr;
    HBITMAP hDib    = nullptr;
    DWORD*  pBits   = nullptr;

    // Returns false on the retail failure paths (the caller then returns FALSE / void).
    bool Open(void* pThis, int cx, int cy)
    {
        dcMem = ::CreateCompatibleDC(SafeHdc(DmDC(pThis)));
        if (dcMem == nullptr) return false;
        bmpMem = ::CreateCompatibleBitmap(DcHdc(pThis), cx, cy);
        if (bmpMem == nullptr) { Close(); return false; }
        hBmpOld = ::SelectObject(dcMem, bmpMem);
        if (hBmpOld == nullptr) {
            // retail: ENSURE(pBmpOld != NULL) -> AfxThrowInvalidArgException, unwinding dcMem/bmpMem
            Close();
            impl__AfxThrowInvalidArgException__YAXXZ();
            return false;
        }
        void* p = nullptr;
        CSize size(cx, cy);
        hDib = impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(&size, &p);
        if (hDib == nullptr || p == nullptr) { Close(); return false; }
        pBits = static_cast<DWORD*>(p);
        ::SelectObject(dcMem, hDib);
        return true;
    }
    void Close()
    {
        if (dcMem != nullptr && hBmpOld != nullptr) ::SelectObject(dcMem, hBmpOld);
        if (hDib != nullptr)   { ::DeleteObject(hDib);   hDib = nullptr; }
        if (bmpMem != nullptr) { ::DeleteObject(bmpMem); bmpMem = nullptr; }
        if (dcMem != nullptr)  { ::DeleteDC(dcMem);      dcMem = nullptr; }
        hBmpOld = nullptr;
        pBits = nullptr;
    }
};

} // namespace

// void CDrawingManager::FillGradient(CRect rect, COLORREF colorStart, COLORREF colorFinish,
//                                   BOOL bHorz, int nStartFlatPercentage, int nEndFlatPercentage)
// RVA 0x57b10 (mfc140u).
//     CMFCToolBarImages::m_bIsDrawOnGlass == 0 -> worker(this, copy of rect, ...)   (0x57b60)
//     else (glass): left/top = min corner, cx/cy = |extent|; if (cx == 0 || cy == 0) return
//        DibScratch as above (0x57bd0..0x57c87); CDrawingManager dmTmp(dcMem) (vptr 0x1802e7188 stored at 0x57c8d)
//        worker(&dmTmp, CRect(0,0,cx,cy), colorStart, colorFinish, bHorz, nStart, nEnd)   (0x57cd8)
//        every pixel |= 0xFF000000                                          (0x57cf4)
//        BitBlt(m_dc.m_hDC, left, top, cx, cy, dcMem, 0, 0, SRCCOPY)        (0x57d34)
//        close scratch
// Symbol: ?FillGradient@CDrawingManager@@QEAAXVCRect@@KKHHH@Z
extern "C" void MS_ABI impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish, int bHorz, int nStartFlatPercentage, int nEndFlatPercentage) {
    if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA == 0) {
        CRect r(*pRect);
        FillGradientWorker(pThis, &r, colorStart, colorFinish, bHorz, nStartFlatPercentage, nEndFlatPercentage);
        return;
    }
    const int left = pRect->left <= pRect->right ? pRect->left : pRect->right;
    const int top  = pRect->top <= pRect->bottom ? pRect->top : pRect->bottom;
    const int cx   = (pRect->left <= pRect->right ? pRect->right : pRect->left) - left;
    const int cy   = (pRect->top <= pRect->bottom ? pRect->bottom : pRect->top) - top;
    if (cx == 0 || cy == 0) return;

    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return;

    // Temporary CDrawingManager over the scratch DC, exactly as retail builds it on its stack.
    S_CDCHead dcTmp = { nullptr, s.dcMem, s.dcMem };   // only m_hDC is read through it
    S_DrawingManager dmTmp = { g_DrawingManager_vtbl, AsCDC(&dcTmp) };
    CRect rectTmp(0, 0, cx, cy);
    FillGradientWorker(&dmTmp, &rectTmp, colorStart, colorFinish, bHorz, nStartFlatPercentage, nEndFlatPercentage);

    const int n = cx * cy;
    for (int i = 0; i < n; ++i)
        s.pBits[i] |= 0xff000000u;

    ::BitBlt(DcHdc(pThis), left, top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    s.Close();
}

// void CDrawingManager::FillGradient2(CRect rect, COLORREF colorStart, COLORREF colorFinish, int nAngle)
// RVA 0x57dd0 (mfc140u).
//     colorStart == colorFinish -> one FillRect on m_dc.m_hDC                   (0x57e0c..0x57e40)
//     nAngle ==   0 / 360 -> FillGradient(rect, colorStart, colorFinish, FALSE, 0, 0)  (0x581de)
//     nAngle ==  90       -> FillGradient(rect, colorStart, colorFinish, TRUE,  0, 0)  (0x58220)
//     nAngle == 180       -> FillGradient(rect, colorFinish, colorStart, FALSE, 0, 0)  (0x581d8)
//     nAngle == 270       -> FillGradient(rect, colorFinish, colorStart, TRUE,  0, 0)  (0x581c0)
//     otherwise: DibScratch (Width x Height); hOldPen = dcMem.SelectStockObject(NULL_PEN)  (0x57f5a)
//        h = Height; nOffset = (int)(cos((nAngle + 180) * PI / 180) * h)               (0x57f69..0x57f9a)
//        w = Width + abs(nOffset)                                                        (0x57fad)
//        64 bands i: clr as in the worker; x1 = (i*w) >> 6, x2 = ((i+1)*w) >> 6, both -= nOffset when nOffset > 0
//           x1 != x2 -> Polygon(dcMem, {x1,0},{x2,0},{x2+nOffset,h},{x1+nOffset,h})  (0x58087..0x580d4)
//        dcMem.SelectObject(hOldPen)                                                     (0x58117)
//        BitBlt(m_dc.m_hDC, rect.left, rect.top, Width, Height, dcMem, 0, 0, SRCCOPY)   (0x58162)
// Symbol: ?FillGradient2@CDrawingManager@@QEAAXVCRect@@KKH@Z
extern "C" void MS_ABI impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish, int nAngle) {
    if (colorStart == colorFinish) {
        HBRUSH br = ::CreateSolidBrush(colorStart);
        ::FillRect(DcHdc(pThis), AsRECT(pRect), br);
        if (br) ::DeleteObject(br);
        return;
    }
    switch (nAngle) {
    case 0:
    case 360: { CRect r(*pRect); impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(pThis, &r, colorStart, colorFinish, FALSE, 0, 0); return; }
    case 90:  { CRect r(*pRect); impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(pThis, &r, colorStart, colorFinish, TRUE,  0, 0); return; }
    case 180: { CRect r(*pRect); impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(pThis, &r, colorFinish, colorStart, FALSE, 0, 0); return; }
    case 270: { CRect r(*pRect); impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(pThis, &r, colorFinish, colorStart, TRUE,  0, 0); return; }
    default: break;
    }

    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return;

    HGDIOBJ hOldPen = ::SelectObject(s.dcMem, ::GetStockObject(NULL_PEN));
    const int h = cy;
    const int nOffset = (int)(std::cos((double)(nAngle + 180) * 3.141592653589793 / 180.0) * (double)h);
    const int w = cx + std::abs(nOffset);

    const int sR = (int)(colorStart & 0xffu), sG = (int)((colorStart >> 8) & 0xffu), sB = (int)((colorStart >> 16) & 0xffu);
    const int fR = (int)(colorFinish & 0xffu), fG = (int)((colorFinish >> 8) & 0xffu), fB = (int)((colorFinish >> 16) & 0xffu);
    for (int i = 0; i < 64; ++i) {
        const int k = 64 - i;
        const unsigned g = (unsigned)((k * sG + i * fG) >> 6) & 0xffu;
        const unsigned b = (unsigned)((k * sB + i * fB) >> 6) & 0xffu;
        const unsigned r = (unsigned)((k * sR + i * fR) >> 6) & 0xffu;
        HBRUSH br = ::CreateSolidBrush((b << 16) | (g << 8) | r);

        int x1 = (i * w) >> 6;
        if (nOffset > 0) x1 -= nOffset;
        int x2 = ((i + 1) * w) >> 6;
        if (nOffset > 0) x2 -= nOffset;
        if (x1 != x2) {
            POINT pts[4] = { { x1, 0 }, { x2, 0 }, { x2 + nOffset, h }, { x1 + nOffset, h } };
            HGDIOBJ hOldBr = ::SelectObject(s.dcMem, br);
            ::Polygon(s.dcMem, pts, 4);
            ::SelectObject(s.dcMem, hOldBr);
        }
        if (br) ::DeleteObject(br);
    }
    ::SelectObject(s.dcMem, hOldPen);
    ::BitBlt(DcHdc(pThis), pRect->left, pRect->top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    s.Close();
}

// void CDrawingManager::MirrorRect(CRect rect, BOOL bHorz)
// RVA 0x56f90 (mfc140u):
//     Height <= 0 || Width <= 0 -> return
//     m_dc.GetClipBox(&rectClip)   (virtual, vtable slot 22 via the CFG dispatcher at 0x56ffb)
//     if (!EqualRect(UnionRect(rectClip, rect), rectClip)) return      -- rect must lie inside the clip box (0x57013..0x57029)
//     DibScratch (cx x cy); BitBlt(dcMem, 0, 0, cx, cy, m_dc.GetSafeHdc(), rect.left, rect.top, SRCCOPY)
//     bHorz:  for y in [0, cy], x in [0, cx/2]: base = (cy - y) * cx;              (0x57172..0x571ec)
//                 if (base + x < cx*cy && base + cx - x - 1 < cx*cy) swap(p[base + x], p[base + cx - x - 1])
//     !bHorz: for y in [0, cy/2], x in [0, cx): swap(p[y*cx + x], p[(cy - y - 1)*cx + x])   (0x571f0..0x57245)
//     BitBlt(m_dc.m_hDC, rect.left, rect.top, cx, cy, dcMem, 0, 0, SRCCOPY); close
// Symbol: ?MirrorRect@CDrawingManager@@QEAAXVCRect@@H@Z
extern "C" void MS_ABI impl__MirrorRect_CDrawingManager__QEAAXVCRect__H_Z(void* pThis, CRect* pRect, int bHorz) {
    if (pRect->bottom - pRect->top <= 0) return;
    if (pRect->right - pRect->left <= 0) return;

    RECT rectClip = { 0, 0, 0, 0 };
    impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(DmDC(pThis), &rectClip);
    RECT rectUnion = { 0, 0, 0, 0 };
    ::UnionRect(&rectUnion, &rectClip, AsRECT(pRect));
    if (!::EqualRect(&rectUnion, &rectClip)) return;

    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return;

    ::BitBlt(s.dcMem, 0, 0, cx, cy, SafeHdc(DmDC(pThis)), pRect->left, pRect->top, SRCCOPY);

    DWORD* p = s.pBits;
    if (bHorz) {
        if (cy >= 0) {
            const int half = cx / 2;
            const int total = cx * cy;
            for (int y = 0; y <= cy; ++y) {
                const int base = (cy - y) * cx;
                for (int x = 0; x <= half; ++x) {
                    if (base + x >= total) continue;
                    if (base - x - 1 + cx >= total) continue;
                    const long long i1 = (long long)base + x;
                    const long long i2 = (long long)base + (cx - x) - 1;
                    const DWORD t = p[i1];
                    p[i1] = p[i2];
                    p[i2] = t;
                }
            }
        }
    } else {
        const int half = cy / 2;
        if (half >= 0) {
            for (int y = 0; y <= half; ++y) {
                const long long r1 = (long long)(cx * y);
                const long long r2 = (long long)((cy - y - 1) * cx);
                for (int x = 0; x < cx; ++x) {
                    const DWORD t = p[r1 + x];
                    p[r1 + x] = p[r2 + x];
                    p[r2 + x] = t;
                }
            }
        }
    }

    ::BitBlt(DcHdc(pThis), pRect->left, pRect->top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    s.Close();
}

// BOOL CDrawingManager::GrayRect(CRect rect, int nPercentage, COLORREF clrTransparent, COLORREF clrDisabledColor)
// RVA 0x57330 (mfc140u):
//     Height <= 0 || Width <= 0 -> TRUE
//     afxGlobalData gate; m_nBitsPerPixel <= 8 -> CMFCToolBarImages::FillDitheredRect(&m_dc, rect); TRUE  (0x573b4)
//     DibScratch (cx x cy) -> FALSE on failure
//     BitBlt(dcMem, 0, 0, cx, cy, m_dc.GetSafeHdc(), rect.left, rect.top, SRCCOPY)   (0x57510)
//     clrTransparent != -1 -> byte-swapped to DIB order                              (0x5751f)
//     clrDisabled == -1 -> afxGlobalData.clrBtnHilite (0x1803c1650)                   (0x57565)
//     per pixel (skipping pix == clrTransparent):
//       nPercentage == -1: RGBtoHSL(pix, &h, &s, &l); gray = HLStoRGB_ONE(h, l, 0.0);
//          each byte = gray_b + (dis_b - gray_b) / 2 (truncating), clamped 255; alpha 0xFF   (0x57591..0x57632)
//          (low byte pairs with clrDisabled's blue, high byte with its red)
//       else: RGBtoHSL(pix, &h, &s, &l); k = nPercentage * 0.01;
//          pix = PixelAlpha(HLStoRGB_ONE(h, l, 0.0), k, k, k) | 0xFF000000                   (0x57665..0x576a1)
//     BitBlt(m_dc.m_hDC, rect.left, rect.top, cx, cy, dcMem, 0, 0, SRCCOPY); close; TRUE
// Symbol: ?GrayRect@CDrawingManager@@QEAAHVCRect@@HKK@Z
extern "C" int MS_ABI impl__GrayRect_CDrawingManager__QEAAHVCRect__HKK_Z(void* pThis, CRect* pRect, int nPercentage, unsigned long clrTransparent, unsigned long clrDisabledColor) {
    if (pRect->bottom - pRect->top <= 0) return TRUE;
    if (pRect->right - pRect->left <= 0) return TRUE;

    EnsureGlobalDataInitialized();
    if (GD_BitsPerPixel() <= 8) {
        impl__FillDitheredRect_CMFCToolBarImages__SAXPEAVCDC__AEBVCRect___Z(DmDC(pThis), pRect);
        return TRUE;
    }

    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return FALSE;

    ::BitBlt(s.dcMem, 0, 0, cx, cy, SafeHdc(DmDC(pThis)), pRect->left, pRect->top, SRCCOPY);

    DWORD clrT = clrTransparent;
    if (clrT != (DWORD)-1) clrT = DibSwap(clrT);
    COLORREF clrDis = clrDisabledColor;
    if (clrDis == (COLORREF)-1) {
        EnsureGlobalDataInitialized();
        clrDis = GD_clrBtnHilite();
    }

    const int n = cy * cx;
    DWORD* p = s.pBits;
    if (n > 0) {
        if (nPercentage == -1) {
            const int disB = (int)((clrDis >> 16) & 0xffu);
            const int disG = (int)((clrDis >> 8) & 0xffu);
            const int disR = (int)(clrDis & 0xffu);
            for (int i = 0; i < n; ++i, ++p) {
                if (*p == clrT) continue;
                double h = 0.0, sat = 0.0, l = 0.0;
                impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(*p, &h, &sat, &l);
                const unsigned long gray = impl__HLStoRGB_ONE_CDrawingManager__SAKNNN_Z(h, l, 0.0);
                const int g0 = (int)(gray & 0xffu), g1 = (int)((gray >> 8) & 0xffu), g2 = (int)((gray >> 16) & 0xffu);
                int v0 = g0 + (disB - g0) / 2; if (v0 > 255) v0 = 255;
                int v1 = g1 + (disG - g1) / 2; if (v1 > 255) v1 = 255;
                int v2 = g2 + (disR - g2) / 2; if (v2 > 255) v2 = 255;
                *p = 0xff000000u | (((unsigned)v2 & 0xffu) << 16) | (((unsigned)v1 & 0xffu) << 8) | ((unsigned)v0 & 0xffu);
            }
        } else {
            const double k = (double)nPercentage * 0.01;
            for (int i = 0; i < n; ++i, ++p) {
                if (*p == clrT) continue;
                double h = 0.0, sat = 0.0, l = 0.0;
                impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(*p, &h, &sat, &l);
                const unsigned long gray = impl__HLStoRGB_ONE_CDrawingManager__SAKNNN_Z(h, l, 0.0);
                *p = (DWORD)impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(gray, k, k, k) | 0xff000000u;
            }
        }
    }

    ::BitBlt(DcHdc(pThis), pRect->left, pRect->top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    s.Close();
    return TRUE;
}

// BOOL CDrawingManager::HighlightRect(CRect rect, int nPercentage, COLORREF clrTransparent,
//                                    int nTolerance, COLORREF clrBlend)
// RVA 0x56900 (mfc140u):
//     nPercentage == 100 -> TRUE; Height <= 0 || Width <= 0 -> TRUE
//     afxGlobalData gate; m_nBitsPerPixel <= 8 -> FillDitheredRect(&m_dc, rect); TRUE   (0x56987)
//     clrBlend != -1 && nPercentage > 100 -> FALSE                                      (0x56993..0x5699f)
//     DibScratch (cx x cy) -> FALSE on failure
//     BitBlt(dcMem, 0, 0, cx, cy, m_dc.GetSafeHdc(), rect.left, rect.top, SRCCOPY)     (0x56afb)
//     clrTransparent != -1 -> byte-swapped to DIB order                                 (0x56b07)
//     per pixel: skip when
//        nTolerance > 0: all three |pix_b - clrT_b| < nTolerance                       (0x56b51..0x56b9d)
//        else          : pix == clrT                                                    (0x56bac)
//       nPercentage == -1: byte = (2*pix_b + clrBtnHilite_b) / 3 (unsigned, clamp 255), alpha 0xFF  (0x56bc4..0x56d39)
//       clrBlend == -1   : PixelAlpha(pix, k, k, k) | 0xFF000000 with k = nPercentage * 0.01   (0x56d44..0x56d68)
//       else             : byte = pix_b + MulDiv(clrBlend_b - pix_b, nPercentage, 100), clamped 255, alpha 0xFF
//                          (blue of clrBlend pairs with the DIB's low byte)             (0x56d6d..0x56e60)
//     BitBlt(m_dc.m_hDC, rect.left, rect.top, cx, cy, dcMem, 0, 0, SRCCOPY); close; TRUE
// Symbol: ?HighlightRect@CDrawingManager@@QEAAHVCRect@@HKHK@Z
extern "C" int MS_ABI impl__HighlightRect_CDrawingManager__QEAAHVCRect__HKHK_Z(void* pThis, CRect* pRect, int nPercentage, unsigned long clrTransparent, int nTolerance, unsigned long clrBlend) {
    if (nPercentage == 100) return TRUE;
    if (pRect->bottom - pRect->top <= 0) return TRUE;
    if (pRect->right - pRect->left <= 0) return TRUE;

    EnsureGlobalDataInitialized();
    if (GD_BitsPerPixel() <= 8) {
        impl__FillDitheredRect_CMFCToolBarImages__SAXPEAVCDC__AEBVCRect___Z(DmDC(pThis), pRect);
        return TRUE;
    }
    if (clrBlend != (unsigned long)-1 && nPercentage > 100) return FALSE;

    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return FALSE;

    ::BitBlt(s.dcMem, 0, 0, cx, cy, SafeHdc(DmDC(pThis)), pRect->left, pRect->top, SRCCOPY);

    DWORD clrT = clrTransparent;
    if (clrT != (DWORD)-1) clrT = DibSwap(clrT);

    const int n = cy * cx;
    DWORD* p = s.pBits;
    for (int i = 0; i < n; ++i, ++p) {
        const DWORD pix = *p;
        const int p0 = (int)(pix & 0xffu), p1 = (int)((pix >> 8) & 0xffu), p2 = (int)((pix >> 16) & 0xffu);
        bool bSkip;
        if (nTolerance > 0) {
            bSkip = std::abs(p0 - (int)(clrT & 0xffu)) < nTolerance &&
                    std::abs(p1 - (int)((clrT >> 8) & 0xffu)) < nTolerance &&
                    std::abs(p2 - (int)((clrT >> 16) & 0xffu)) < nTolerance;
        } else {
            bSkip = (pix == clrT);
        }
        if (bSkip) continue;

        DWORD out;
        if (nPercentage == -1) {
            EnsureGlobalDataInitialized();
            const COLORREF hil = GD_clrBtnHilite();
            unsigned v0 = (2u * (unsigned)p0 + ((hil >> 16) & 0xffu)) / 3u; if (v0 > 255u) v0 = 255u;
            unsigned v1 = (2u * (unsigned)p1 + ((hil >> 8) & 0xffu)) / 3u;  if (v1 > 255u) v1 = 255u;
            unsigned v2 = (2u * (unsigned)p2 + (hil & 0xffu)) / 3u;         if (v2 > 255u) v2 = 255u;
            out = 0xff000000u | ((v2 & 0xffu) << 16) | ((v1 & 0xffu) << 8) | (v0 & 0xffu);
        } else if (clrBlend == (unsigned long)-1) {
            const double k = (double)nPercentage * 0.01;
            out = (DWORD)impl__PixelAlpha_CDrawingManager__SAKKNNN_Z(pix, k, k, k) | 0xff000000u;
        } else {
            int v0 = p0 + ::MulDiv((int)((clrBlend >> 16) & 0xffu) - p0, nPercentage, 100); if (v0 > 255) v0 = 255;
            int v1 = p1 + ::MulDiv((int)((clrBlend >> 8) & 0xffu) - p1, nPercentage, 100);  if (v1 > 255) v1 = 255;
            int v2 = p2 + ::MulDiv((int)(clrBlend & 0xffu) - p2, nPercentage, 100);         if (v2 > 255) v2 = 255;
            out = 0xff000000u | (((unsigned)v2 & 0xffu) << 16) | (((unsigned)v1 & 0xffu) << 8) | ((unsigned)v0 & 0xffu);
        }
        *p = out;
    }

    ::BitBlt(DcHdc(pThis), pRect->left, pRect->top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    s.Close();
    return TRUE;
}

// void CDrawingManager::DrawRect(const CRect& rect, COLORREF clrFill, COLORREF clrLine)
// RVA 0x5ada0 (mfc140u):
//     clrFill == -1 && clrLine == -1 -> return
//     normalise rect (swap left/right, top/bottom); cx/cy; either 0 -> return
//     DibScratch (cx x cy)
//     clrLine != -1: row 0 (the DIB's bottom row) = line colour;                        (0x5af63)
//        cy > 1: memcpy row (cy-1) from row 0;                                          (0x5af9e)
//        cy > 2: row 1 gets the line colour at column 0 and (cx >= 2) column cx-1       (0x5afaa..0x5afb5)
//     fill = clrFill == -1 ? 0 : opaque(clrFill)                                        (0x5b00c)
//     inner rows: first inner row filled from column (clrLine != -1) to cx - (clrLine != -1),
//        then memcpy'd down through the remaining inner rows                            (0x5b01a..0x5b05c)
//     clrFill != -1 -> BitBlt(m_dc.m_hDC, left, top, cx, cy, dcMem, 0, 0, SRCCOPY)      (0x5b0a9)
//     else          -> DrawAlpha(&m_dc, rect, &dcMem, CRect(0,0,cx,cy))                 (0x5b0da)
//     close
// Symbol: ?DrawRect@CDrawingManager@@QEAAXAEBVCRect@@KK@Z
extern "C" void MS_ABI impl__DrawRect_CDrawingManager__QEAAXAEBVCRect__KK_Z(void* pThis, const CRect* pRect, unsigned long clrFill, unsigned long clrLine) {
    if (clrFill == (unsigned long)-1 && clrLine == (unsigned long)-1) return;

    CRect rect(*pRect);
    if (rect.left > rect.right) { const int t = rect.left; rect.left = rect.right; rect.right = t; }
    if (rect.top > rect.bottom) { const int t = rect.top; rect.top = rect.bottom; rect.bottom = t; }
    const int cx = rect.right - rect.left;
    const int cy = rect.bottom - rect.top;
    if (cx == 0 || cy == 0) return;

    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return;

    DWORD* p = s.pBits;
    const size_t rowBytes = (size_t)cx * 4;
    int xStart, xEnd, yStart, yEnd;
    if (clrLine != (unsigned long)-1) {
        const DWORD line = DibOpaque(clrLine);
        for (int x = 0; x < cx; ++x)
            *p++ = line;                                   // row 0
        if (cy > 1) {
            std::memcpy(p + (size_t)(cy - 2) * cx, p - cx, rowBytes);   // row cy-1 = row 0
            if (cy > 2) {
                *p = line;                                 // row 1, column 0
                if (cx >= 2) p[cx - 1] = line;             // row 1, column cx-1
                ++p;
            }
        }
        xStart = 1; xEnd = cx - 1; yStart = 2; yEnd = cy - 1;
    } else {
        xStart = 0; xEnd = cx; yStart = 1; yEnd = cy;
    }

    const DWORD fill = (clrFill == (unsigned long)-1) ? 0u : DibOpaque(clrFill);
    if (yStart <= yEnd) {
        if (xStart < xEnd) {                               // 0x5b015: the border skip below only runs with the fill loop
            for (int x = xStart; x < xEnd; ++x)
                *p++ = fill;
            if (clrLine != (unsigned long)-1) ++p;         // skip the right border pixel
        }
        for (int y = yStart; y < yEnd; ++y) {
            std::memcpy(p, p - cx, rowBytes);
            p += cx;
        }
    }

    if (clrFill != (unsigned long)-1) {
        ::BitBlt(DcHdc(pThis), rect.left, rect.top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    } else {
        S_CDCHead dcTmp = { nullptr, s.dcMem, s.dcMem };
        CRect rectSrc(0, 0, cx, cy);
        impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(pThis, DmDC(pThis), &rect, AsCDC(&dcTmp), &rectSrc);
    }
    s.Close();
}

// void CDrawingManager::DrawLine(int x1, int y1, int x2, int y2, COLORREF clrLine)
// RVA 0x59500 (mfc140u).  Bresenham into a 32-bpp DIB that is then alpha-blended.
//     clrLine == -1 -> return
//     adx = |x2-x1|, ady = |y2-y1|, sx/sy = sign (0 when equal); both 0 -> return     (0x59536..0x59590)
//     rect = { min x, min y, max x + 1, max y + 1 }; width/height 0 -> return         (0x59596..0x595e3)
//     DibScratch (width x height)
//     major = max(adx, ady), minor = min; err = 2*minor - major; pixel = opaque(clrLine)
//     for i = 1..major:  plot(x, y);                                                  (0x5972c..0x59795)
//        while (err >= 0) { if (ady <= adx) y += sy; else x += sx; err -= 2*major; }
//        if (ady > adx) y += sy; else x += sx; err += 2*minor;
//     plot(x, y) once more                                                            (0x59797..0x597b9)
//        plot: pBits[(height - 1 - (y - top)) * width + (x - left)]   (bottom-up rows)
//     DrawAlpha(&m_dc, rect, &dcMem, CRect(0,0,width,height)); close                  (0x597e6)
// Symbol: ?DrawLine@CDrawingManager@@QEAAXHHHHK@Z
extern "C" void MS_ABI impl__DrawLine_CDrawingManager__QEAAXHHHHK_Z(void* pThis, int x1, int y1, int x2, int y2, unsigned long clrLine) {
    if (clrLine == (unsigned long)-1) return;

    const int dx = x2 - x1, dy = y2 - y1;
    const int adx = std::abs(dx), ady = std::abs(dy);
    const int sx = dx == 0 ? 0 : (dx > 0 ? 1 : -1);
    const int sy = dy == 0 ? 0 : (dy > 0 ? 1 : -1);
    if (adx == 0 && ady == 0) return;

    CRect rect;
    rect.left = x1; rect.top = y1;
    int xmax = x2, ymax = y2;
    if (x1 > x2) { xmax = x1; rect.left = x2; }
    if (y1 > y2) { ymax = y1; rect.top = y2; }
    rect.right = xmax + 1;
    rect.bottom = ymax + 1;
    const int width = rect.right - rect.left;
    const int height = rect.bottom - rect.top;
    if (width == 0 || height == 0) return;

    DibScratch s;
    if (!s.Open(pThis, width, height)) return;

    int major, minor;
    if (ady > adx) { major = ady; minor = adx; } else { major = adx; minor = ady; }
    const int minor2 = 2 * minor;
    const int major2 = 2 * major;
    int err = minor2 - major;
    const DWORD pixel = DibOpaque(clrLine);
    DWORD* p = s.pBits;
    int x = x1, y = y1;
    auto plot = [&](int px, int py) {
        const long long idx = (long long)((height - 1 + (rect.top - py)) * width) + (long long)(px - rect.left);
        p[idx] = pixel;
    };
    for (int i = 1; i <= major; ++i) {
        plot(x, y);
        while (err >= 0) {
            if (ady <= adx) y += sy; else x += sx;
            err -= major2;
        }
        if (ady > adx) y += sy; else x += sx;
        err += minor2;
    }
    plot(x, y);

    S_CDCHead dcTmp = { nullptr, s.dcMem, s.dcMem };
    CRect rectSrc(0, 0, width, height);
    impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(pThis, DmDC(pThis), &rect, AsCDC(&dcTmp), &rectSrc);
    s.Close();
}

namespace {

// The non-exported shadow-pixel helper at RVA 0x5b180 (mfc140u), called by
// DrawShadow for every pixel of the shadow bands:
//     stride = rect.Width() + nDepth;  row = rect.Height() - y + nDepth
//     idx = bFlip ? (row + 1) * stride - x : row * stride + x                        (0x5b1a3..0x5b1c5)
//     pBits[idx] = PixelAlpha(pBits[idx], nPercentage)     (the int overload, 0x5b270 in mfc140u)
//     clrBase != -1: byte = (3 * pix_b + clrBase_b) >> 2, clamped 255 (unsigned cmova);
//        clrBase's blue pairs with the DIB's low byte; the alpha byte is left 0      (0x5b1e5..0x5b253)
void SetShadowPixel(DWORD* pBits, const CRect* pRect, int x, int y, int nPercentage, int nDepth,
                    COLORREF clrBase, int bFlip)
{
    const int stride = (pRect->right - pRect->left) + nDepth;
    const int row = (pRect->bottom - pRect->top) - y + nDepth;
    long long idx;
    if (bFlip) idx = (long long)((row + 1) * stride) - (long long)x;
    else       idx = (long long)(row * stride) + (long long)x;
    const DWORD pix = (DWORD)impl__PixelAlpha_CDrawingManager__SAKKH_Z(pBits[idx], nPercentage);
    pBits[idx] = pix;
    if (clrBase == (COLORREF)-1) return;
    unsigned v0 = (3u * (pix & 0xffu) + ((clrBase >> 16) & 0xffu)) >> 2;         if (v0 > 255u) v0 = 255u;
    unsigned v1 = (3u * ((pix >> 8) & 0xffu) + ((clrBase >> 8) & 0xffu)) >> 2;   if (v1 > 255u) v1 = 255u;
    unsigned v2 = (3u * ((pix >> 16) & 0xffu) + (clrBase & 0xffu)) >> 2;         if (v2 > 255u) v2 = 255u;
    pBits[idx] = ((v2 & 0xffu) << 16) | ((v1 & 0xffu) << 8) | (v0 & 0xffu);
}

} // namespace

// BOOL CDrawingManager::DrawShadow(CRect rect, int nDepth, int iMinBrightness, int iMaxBrightness,
//                                 CBitmap* pBmpSaveBottom, CBitmap* pBmpSaveRight,
//                                 COLORREF clrBase, BOOL bRightShadow)
// RVA 0x58e30 (mfc140u):
//     nDepth == 0 || IsRectEmpty(rect) -> TRUE
//     pBmpSaveRight && pBmpSaveRight->m_hObject: ENSURE(pBmpSaveBottom && its m_hObject) (else 0x594eb throw)
//        DrawState(m_dc.m_hDC, 0, 0, pBmpSaveRight->m_hObject, 0, bRight ? rect.right : rect.left - nDepth,
//                  rect.top, nDepth, cy + nDepth, DST_BITMAP)                                (0x58f20)
//        DrawState(m_dc.m_hDC, 0, 0, pBmpSaveBottom->m_hObject, 0, bRight ? rect.left : rect.left - nDepth,
//                  rect.bottom, cx + nDepth, nDepth, DST_BITMAP); TRUE                        (0x58f66)
//     else ENSURE(pBmpSaveBottom == NULL || pBmpSaveBottom->m_hObject == NULL)              (0x58f8c..0x58f95)
//     DibScratch ((cx + nDepth) x (cy + nDepth)) -> FALSE on failure
//     BitBlt(dcMem, 0, 0, cx+nDepth, cy+nDepth, m_dc.GetSafeHdc(), bRight ? rect.left : rect.left - nDepth, rect.top, SRCCOPY)
//     nStep = (iMaxBrightness - iMinBrightness) / nDepth                                     (0x590ef)
//     for i in [0, nDepth):  (helper = SetShadowPixel(pBits, rect, x, y, pct, nDepth, clrBase, !bRight))
//        pct = iMax - nStep*(nDepth - i): for y in [cy, cy + nDepth - i)      helper(nDepth + i, y)     (0x59105)
//        pct = iMax - nStep*i:           for x in [2*nDepth - i, cx + i)     helper(x, cy + i)         (0x5916d)
//        pct = iMax - nStep*i:           for y in [2*nDepth - i, cy + i + 1) helper(cx + i, y)         (0x591eb)
//        pct = iMax - nStep*(nDepth - i): for x in [cx, cx + nDepth - i)      helper(x, nDepth + i)     (0x5924d)
//     BitBlt(m_dc.m_hDC, bRight ? rect.left : rect.left - nDepth, rect.top, cx+nDepth, cy+nDepth, dcMem, 0, 0, SRCCOPY)
//     pBmpSaveRight:  Attach(CreateCompatibleBitmap(m_dc.m_hDC, nDepth + 1, cy + nDepth)); select into dcMem;
//        BitBlt(dcMem, 0, 0, nDepth, cy + nDepth, m_dc.GetSafeHdc(), bRight ? rect.right : 0, rect.top, SRCCOPY)  (0x5934f..0x593b1)
//     pBmpSaveBottom: Attach(CreateCompatibleBitmap(m_dc.m_hDC, cx + nDepth, nDepth + 1)); select into dcMem;
//        BitBlt(dcMem, 0, 0, cx + nDepth, nDepth, m_dc.GetSafeHdc(), bRight ? rect.left : rect.left - nDepth, rect.bottom, SRCCOPY)
//     close; TRUE
// Symbol: ?DrawShadow@CDrawingManager@@QEAAHVCRect@@HHHPEAVCBitmap@@1KH@Z
extern "C" int MS_ABI impl__DrawShadow_CDrawingManager__QEAAHVCRect__HHHPEAVCBitmap__1KH_Z(void* pThis, CRect* pRect, int nDepth, int iMinBrightness, int iMaxBrightness, CBitmap* pBmpSaveBottom, CBitmap* pBmpSaveRight, unsigned long clrBase, int bRightShadow) {
    if (nDepth == 0) return TRUE;
    if (::IsRectEmpty(AsRECT(pRect))) return TRUE;

    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    const int bFlip = (bRightShadow == 0);
    const int xLeft = bRightShadow ? pRect->left : pRect->left - nDepth;

    if (pBmpSaveRight != nullptr && pBmpSaveRight->m_hObject != nullptr) {
        if (pBmpSaveBottom == nullptr || pBmpSaveBottom->m_hObject == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;
        }
        ::DrawState(DcHdc(pThis), nullptr, nullptr, (LPARAM)pBmpSaveRight->m_hObject, 0,
                    bRightShadow ? pRect->right : pRect->left - nDepth, pRect->top,
                    nDepth, cy + nDepth, DST_BITMAP);
        ::DrawState(DcHdc(pThis), nullptr, nullptr, (LPARAM)pBmpSaveBottom->m_hObject, 0,
                    xLeft, pRect->bottom, cx + nDepth, nDepth, DST_BITMAP);
        return TRUE;
    }
    if (pBmpSaveBottom != nullptr && pBmpSaveBottom->m_hObject != nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }

    DibScratch s;
    if (!s.Open(pThis, cx + nDepth, cy + nDepth)) return FALSE;

    ::BitBlt(s.dcMem, 0, 0, cx + nDepth, cy + nDepth, SafeHdc(DmDC(pThis)), xLeft, pRect->top, SRCCOPY);

    const int nStep = (iMaxBrightness - iMinBrightness) / nDepth;
    for (int i = 0; i < nDepth; ++i) {
        int pct = iMaxBrightness - nStep * (nDepth - i);
        for (int y = cy; y < cy + nDepth - i; ++y)
            SetShadowPixel(s.pBits, pRect, nDepth + i, y, pct, nDepth, clrBase, bFlip);

        pct = iMaxBrightness - nStep * i;
        for (int x = 2 * nDepth - i; x < cx + i; ++x)
            SetShadowPixel(s.pBits, pRect, x, cy + i, pct, nDepth, clrBase, bFlip);

        for (int y = 2 * nDepth - i; y < cy + i + 1; ++y)
            SetShadowPixel(s.pBits, pRect, cx + i, y, pct, nDepth, clrBase, bFlip);

        pct = iMaxBrightness - nStep * (nDepth - i);
        for (int x = cx; x < cx + nDepth - i; ++x)
            SetShadowPixel(s.pBits, pRect, x, nDepth + i, pct, nDepth, clrBase, bFlip);
    }

    ::BitBlt(DcHdc(pThis), xLeft, pRect->top, cx + nDepth, cy + nDepth, s.dcMem, 0, 0, SRCCOPY);

    if (pBmpSaveRight != nullptr) {
        impl__Attach_CGdiObject__QEAAHPEAX_Z(pBmpSaveRight, ::CreateCompatibleBitmap(DcHdc(pThis), nDepth + 1, cy + nDepth));
        ::SelectObject(s.dcMem, pBmpSaveRight->m_hObject);
        ::BitBlt(s.dcMem, 0, 0, nDepth, cy + nDepth, SafeHdc(DmDC(pThis)),
                 bRightShadow ? pRect->right : 0, pRect->top, SRCCOPY);
    }
    if (pBmpSaveBottom != nullptr) {
        impl__Attach_CGdiObject__QEAAHPEAX_Z(pBmpSaveBottom, ::CreateCompatibleBitmap(DcHdc(pThis), cx + nDepth, nDepth + 1));
        ::SelectObject(s.dcMem, pBmpSaveBottom->m_hObject);
        ::BitBlt(s.dcMem, 0, 0, cx + nDepth, nDepth, SafeHdc(DmDC(pThis)), xLeft, pRect->bottom, SRCCOPY);
    }
    s.Close();
    return TRUE;
}

namespace {

// Wu-style edge-pixel pair shared by the four anti-aliased ellipse edges
// (DrawEllipse 0x5a120, mfc140u; the four copies at 0x5a5b7..0x5a712,
// 0x5a755..0x5a881, 0x5a938..0x5aaca and 0x5ab11..0x5ac47 are identical
// modulo register allocation).  f is the distance from the outer pixel row
// (or column) to the ideal edge, in [0,1):
//     Rf = (int)(lineR * f), Gf = (int)(lineG * f), Bf = (int)(lineB * f), A = (int)(f * 255)
//     inner (fill != -1) = opaque, byte = (int)((1 - f) * fill_c + line_cf)      (0x5a64b..0x5a6b2)
//     inner (fill == -1) = (A << 24) | (byte)Rf << 16 | (byte)Gf << 8 | (byte)Bf  (0x5a6bc..0x5a6db)
//     outer              = (0xFF000000 - (A << 24)) | (byte)(lineR - Rf) << 16
//                          | (byte)(lineG - Gf) << 8 | (byte)(lineB - Bf)       (0x5a6de..0x5a710)
inline void EllipseEdgePair(double f, int lineR, int lineG, int lineB,
                            unsigned long clrFill, double fillR, double fillG, double fillB,
                            DWORD& inner, DWORD& outer)
{
    const int Bf = (int)((double)lineB * f);
    const int Gf = (int)((double)lineG * f);
    const int Rf = (int)((double)lineR * f);
    const int A  = (int)(f * 255.0);
    if (clrFill != (unsigned long)-1) {
        const double g = 1.0 - f;
        inner = 0xff000000u
              | ((unsigned)(unsigned char)(int)(g * fillR + (double)Rf) << 16)
              | ((unsigned)(unsigned char)(int)(g * fillG + (double)Gf) << 8)
              |  (unsigned)(unsigned char)(int)(g * fillB + (double)Bf);
    } else {
        inner = ((unsigned)A << 24)
              | ((unsigned)(unsigned char)Rf << 16)
              | ((unsigned)(unsigned char)Gf << 8)
              |  (unsigned)(unsigned char)Bf;
    }
    outer = (0xff000000u - ((unsigned)A << 24))
          | ((unsigned)(unsigned char)(lineR - (unsigned char)Rf) << 16)
          | ((unsigned)(unsigned char)(lineG - (unsigned char)Gf) << 8)
          |  (unsigned)(unsigned char)(lineB - (unsigned char)Bf);
}

} // namespace

// void CDrawingManager::DrawEllipse(const CRect& rect, COLORREF clrFill, COLORREF clrLine)
// RVA 0x5a120 (mfc140u).  Anti-aliased ellipse into a 32-bpp DIB that is then
// alpha-blended (crt ceil / floor / sqrt via IAT 0x1802c7508 / 0x1802c7538 / 0x1802c7510).
//     clrFill == -1 && clrLine == -1 -> return
//     normalise rect; cx/cy; either 0 -> return                                      (0x5a1a0..0x5a1f1)
//     DibScratch (cx x cy)
//     line colour = clrLine != -1 ? clrLine : clrFill                                 (0x5a2ec..0x5a2f5)
//     x1 = min(0, cx-1), x2 = max(0, cx-1); y1 = min(0, cy-1), y2 = max(0, cy-1)      (0x5a34f..0x5a396)
//     bSwap = (x2 - x1) >= (y2 - y1): swap the x and y pairs (axes exchanged)         (0x5a3a5..0x5a3c3)
//     a = (x2-x1)/2, b = (y2-y1)/2, xc = (x2+x1)/2, yc = (y2+y1)/2                    (0x5a3c8..0x5a3fa)
//     clrFill != -1: for x in [ceil(xc-a), floor(xc+a)]: q = 1 - ((x-xc)/a)^2; q < 0 -> skip
//        h = sqrt(q)*b; rows [ceil(yc-h), floor(yc+h)] of column x = opaque(clrFill)
//        (bSwap: pixels [x*cx + y0, x*cx + y1] instead)                                (0x5a407..0x5a4f5)
//     phase 1: for x in [floor(xc - a^2/sqrt(a^2+b^2)), ceil(xc + a^2/sqrt(a^2+b^2))]:  (0x5a504..0x5a8ca)
//        q as above; h = sqrt(q)*b
//        yT = ceil(yc+h), f = yT - (yc+h): pixel(x,yT) = outer, pixel(x,yT-1) = inner
//        yB = floor(yc-h), f = (yc-h) - yB:  pixel(x,yB) = outer, pixel(x,yB+1) = inner
//     phase 2: for y in [ceil(yc - b^2/sqrt(a^2+b^2)), floor(yc + b^2/sqrt(a^2+b^2))]:  (0x5a8d0..0x5ac90)
//        q = 1 - ((y-yc)/b)^2; w = sqrt(q)*a
//        xL = floor(xc-w), f = (xc-w) - xL: pixel(xL,y) = outer, pixel(xL+1,y) = inner
//        xR = ceil(xc+w),  f = xR - (xc+w): pixel(xR,y) = outer, pixel(xR-1,y) = inner
//        (pixel(x,y) = pBits[y*cx + x]; bSwap exchanges the roles of x and y in the index)
//     DrawAlpha(&m_dc, rect, &dcMem, CRect(0,0,cx,cy)); close                         (0x5acc6)
// No bounds checks in retail; a 1-pixel extent (a or b == 0) divides by zero, and the
// resulting NaN goes through cvttsd2si as INT_MIN, so retail faults on it as well.
// Symbol: ?DrawEllipse@CDrawingManager@@QEAAXAEBVCRect@@KK@Z
extern "C" void MS_ABI impl__DrawEllipse_CDrawingManager__QEAAXAEBVCRect__KK_Z(void* pThis, const CRect* pRect, unsigned long clrFill, unsigned long clrLine) {
    if (clrFill == (unsigned long)-1 && clrLine == (unsigned long)-1) return;

    CRect rect(*pRect);
    if (rect.left > rect.right) { const int t = rect.left; rect.left = rect.right; rect.right = t; }
    if (rect.top > rect.bottom) { const int t = rect.top; rect.top = rect.bottom; rect.bottom = t; }
    const int cx = rect.right - rect.left;
    const int cy = rect.bottom - rect.top;
    if (cx == 0 || cy == 0) return;

    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return;
    DWORD* const p = s.pBits;

    const unsigned long clr = (clrLine != (unsigned long)-1) ? clrLine : clrFill;
    const int lineR = (int)(clr & 0xffu), lineG = (int)((clr >> 8) & 0xffu), lineB = (int)((clr >> 16) & 0xffu);
    const double fillR = (double)(int)(clrFill & 0xffu);
    const double fillG = (double)(int)((clrFill >> 8) & 0xffu);
    const double fillB = (double)(int)((clrFill >> 16) & 0xffu);

    double x1 = 0.0, x2 = (double)(cx - 1);
    if (0.0 <= x2) { /* keep */ } else { x1 = x2; x2 = 0.0; }
    double y1 = 0.0, y2 = (double)(cy - 1);
    if (0.0 <= y2) { /* keep */ } else { y1 = y2; y2 = 0.0; }
    const bool bSwap = (x2 - x1) >= (y2 - y1);
    if (bSwap) {
        double t = x1; x1 = y1; y1 = t;
        t = x2; x2 = y2; y2 = t;
    }
    const double a  = (x2 - x1) * 0.5;
    const double b  = (y2 - y1) * 0.5;
    const double xc = (x2 + x1) * 0.5;
    const double yc = (y2 + y1) * 0.5;

    // pixel(x, y) in the ellipse's own (possibly swapped) axes
    auto at = [&](int x, int y) -> DWORD& {
        return bSwap ? p[(long long)(x * cx) + (long long)y] : p[(long long)(y * cx) + (long long)x];
    };

    if (clrFill != (unsigned long)-1) {
        const DWORD fill = DibOpaque(clrFill);
        const int xs = (int)std::ceil(xc - a);
        const int xe = (int)std::floor(xc + a);
        for (int x = xs; x <= xe; ++x) {
            const double t = ((double)x - xc) / a;
            const double q = 1.0 - t * t;
            if (0.0 > q) continue;
            const double h = std::sqrt(q) * b;
            const int y0 = (int)std::ceil(yc - h);
            const int y1e = (int)std::floor(yc + h);
            for (int y = y0; y <= y1e; ++y)
                at(x, y) = fill;
        }
    }

    const double a2 = a * a, b2 = b * b;
    const double hyp = std::sqrt(a2 + b2);
    DWORD inner, outer;

    // phase 1: the two arcs where |slope| <= 1 in the (x, y) frame
    {
        const double ext = a2 / hyp;
        const int xs = (int)std::floor(xc - ext);
        const int xe = (int)std::ceil(xc + ext);
        for (int x = xs; x <= xe; ++x) {
            const double t = ((double)x - xc) / a;
            const double q = 1.0 - t * t;
            if (0.0 > q) continue;
            const double h = std::sqrt(q) * b;

            const int yT = (int)std::ceil(h + yc);
            EllipseEdgePair((double)yT - yc - h, lineR, lineG, lineB, clrFill, fillR, fillG, fillB, inner, outer);
            at(x, yT) = outer;
            at(x, yT - 1) = inner;

            const double yb = yc - h;
            const int yB = (int)std::floor(yb);
            EllipseEdgePair(yb - (double)yB, lineR, lineG, lineB, clrFill, fillR, fillG, fillB, inner, outer);
            at(x, yB) = outer;
            at(x, yB + 1) = inner;
        }
    }
    // phase 2: the two arcs where |slope| > 1
    {
        const double ext = b2 / hyp;
        const int ys = (int)std::ceil(yc - ext);
        const int ye = (int)std::floor(yc + ext);
        for (int y = ys; y <= ye; ++y) {
            const double t = ((double)y - yc) / b;
            const double q = 1.0 - t * t;
            if (0.0 > q) continue;
            const double w = std::sqrt(q) * a;

            const double xl = xc - w;
            const int xL = (int)std::floor(xl);
            EllipseEdgePair(xl - (double)xL, lineR, lineG, lineB, clrFill, fillR, fillG, fillB, inner, outer);
            at(xL, y) = outer;
            at(xL + 1, y) = inner;

            const int xR = (int)std::ceil(w + xc);
            EllipseEdgePair((double)xR - xc - w, lineR, lineG, lineB, clrFill, fillR, fillG, fillB, inner, outer);
            at(xR, y) = outer;
            at(xR - 1, y) = inner;
        }
    }

    S_CDCHead dcTmp = { nullptr, s.dcMem, s.dcMem };
    CRect rectSrc(0, 0, cx, cy);
    impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(pThis, DmDC(pThis), &rect, AsCDC(&dcTmp), &rectSrc);
    s.Close();
}

// BOOL CDrawingManager::DrawGradientRing(CRect rect, COLORREF colorStart, COLORREF colorFinish,
//                                       COLORREF colorBorder, int nAngle, int nSize, COLORREF clrFace)
// RVA 0x584f0 (mfc140u).
//     Width <= 4 || Height <= 4 -> FALSE                                              (0x5856b / 0x58584)
//     DibScratch (cx x cy) -> FALSE on failure
//     BitBlt(dcMem, 0, 0, cx, cy, m_dc.GetSafeHdc(), rect.left, rect.top, SRCCOPY)    (0x586c4)
//     ::OffsetRect(&rect, -left, -top)                                                (0x586d8)
//     xCenter = (right + left) / 2, yCenter = (bottom + top) / 2                      (signed; 0x586de..0x586f6)
//     dAngle = nAngle * PI / 180; dAngleEnd = dAngle + 2*PI                          (0x586f9..0x5872b)
//     per channel: start_c, d_c = (finish_c + 0.5 - start_c) / 360 * 2               (0x58730..0x587e8)
//     for j in [0, nSize):                                                            (0x58817..0x58cea)
//        radius = min(Width, Height) / 2; delta = Width - Height
//        j == 0 && clrFace != -1: CBrush(clrFace) + NULL_PEN into dcMem:              (0x58858..0x5899b)
//           delta == 0: Ellipse(rect)
//           delta >  0: Ellipse(l,t,l+(b-t),b); Ellipse(r-(b-t),t,r,b); Rectangle(l+(b-t)/2,t,r-(b-t)/2,b)
//           delta <  0: Ellipse(l,t,r,t+(r-l)); Ellipse(l,b-(r-l),r,b); Rectangle(l,t+(r-l)/2,r,b-(r-l)/2)
//        xPrev = yPrev = -1; for (theta = dAngle, i = 0; dAngleEnd + PI/180 > theta; theta += PI/180, ++i):
//           k = (dAngle + dAngleEnd) / 2 >= theta ? i : 360 - i                       (0x589f0..0x589fd)
//           c = clamp(k * d_c + 0.5 + start_c, 0, 255) per channel                    (0x58a01..0x58a87)
//           clr = (j == 0 && colorBorder != -1) ? colorBorder : RGB((BYTE)R,(BYTE)G,(BYTE)B)
//           x = (int)(cos(theta) * radius) + xCenter; y = (int)(sin(theta) * radius) + yCenter  (IAT 0x1802c7520 / 0x1802c7528)
//           delta > 0: s = (int)(delta * -0.5); x > xCenter ? x -= s : x += s;        (0x58b06..0x58b28)
//                      xPrev != -1 && side(x) != side(xPrev): run [min, max) on row y = clr   (0x58b2a..0x58ba7)
//           delta < 0: s = (int)(delta * 0.5);  y > yCenter ? y -= s : y += s;        (0x58bb4..0x58bd0)
//                      yPrev != -1 && side(y) != side(yPrev): run [min, max) on column x = clr (0x58bd3..0x58c3a)
//           pBits[(cy - y) * cx + x] = DibSwap(clr); xPrev = x; yPrev = y            (0x58c3c..0x58c77)
//        ::InflateRect(&rect, -1, -1)                                                 (0x58cbf)
//     BitBlt(m_dc.m_hDC, left, top, cx, cy, dcMem, 0, 0, SRCCOPY); close; TRUE        (0x58d30)
// Retail indexes rows as (cy - y), not (cy - 1 - y): the topmost ring row (y == 0)
// lands one row past the DIB.  DEVIATION: those out-of-buffer stores are dropped
// here (PutRing); every in-range store is identical.
// DEVIATION: retail's CBrush(clrFace) throws CResourceException when
// CreateSolidBrush fails (0x2a409f -> 0x2a42e0); the scratch objects are released
// first here (retail leaks hDib on that unwind), then the same exception is thrown.
// Symbol: ?DrawGradientRing@CDrawingManager@@QEAAHVCRect@@KKKHHK@Z
extern "C" int MS_ABI impl__DrawGradientRing_CDrawingManager__QEAAHVCRect__KKKHHK_Z(void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish, unsigned long colorBorder, int nAngle, int nSize, unsigned long clrFace) {
    const int left = pRect->left;
    const int cx = pRect->right - left;
    if (cx <= 4) return FALSE;
    const int top = pRect->top;
    const int cy = pRect->bottom - top;
    if (cy <= 4) return FALSE;

    DibScratch s;
    if (!s.Open(pThis, cx, cy)) return FALSE;
    DWORD* const p = s.pBits;
    const long long n = (long long)cx * cy;

    ::BitBlt(s.dcMem, 0, 0, cx, cy, SafeHdc(DmDC(pThis)), left, top, SRCCOPY);
    ::OffsetRect(AsRECT(pRect), -left, -top);

    const int xCenter = (pRect->right + pRect->left) / 2;
    const int yCenter = (pRect->bottom + pRect->top) / 2;

    const double PI = 3.141592653589793;
    const double dAngle    = (double)nAngle * PI / 180.0;
    const double dAngleEnd = dAngle + 2.0 * PI;
    const double sR = (double)(int)(colorStart & 0xffu);
    const double dR = ((double)(int)(colorFinish & 0xffu) + 0.5 - sR) / 360.0 * 2.0;
    const double sG = (double)(int)((colorStart >> 8) & 0xffu);
    const double dG = ((double)(int)((colorFinish >> 8) & 0xffu) + 0.5 - sG) / 360.0 * 2.0;
    const double sB = (double)(int)((colorStart >> 16) & 0xffu);
    const double dB = ((double)(int)((colorFinish >> 16) & 0xffu) + 0.5 - sB) / 360.0 * 2.0;

    auto clamp255 = [](double v) -> double {
        if (v > 255.0) return 255.0;
        if (0.0 > v)   return 0.0;
        return v;
    };
    auto PutRing = [&](int x, int y, DWORD pix) {
        const long long idx = (long long)((cy - y) * cx) + (long long)x;
        if (idx >= 0 && idx < n) p[idx] = pix;
    };

    for (int j = 0; j < nSize; ++j) {
        const double dAngleStop = dAngleEnd + PI / 180.0;
        const int w = pRect->right - pRect->left;
        const int h = pRect->bottom - pRect->top;
        const int radius = (w >= h ? h : w) / 2;
        const int delta = w - h;

        if (clrFace != (unsigned long)-1 && j == 0) {
            HBRUSH br = ::CreateSolidBrush(clrFace);
            if (br == nullptr) {
                s.Close();
                impl__AfxThrowResourceException__YAXXZ();
                return FALSE;
            }
            HGDIOBJ hOldBrush = ::SelectObject(s.dcMem, br);
            HGDIOBJ hOldPen   = ::SelectObject(s.dcMem, ::GetStockObject(NULL_PEN));
            const int l = pRect->left, t = pRect->top, r = pRect->right, b = pRect->bottom;
            if (delta == 0) {
                ::Ellipse(s.dcMem, l, t, r, b);
            } else if (delta > 0) {
                ::Ellipse(s.dcMem, l, t, l + (b - t), b);
                ::Ellipse(s.dcMem, r - (b - t), t, r, b);
                const int k = (b - t) / 2;
                ::Rectangle(s.dcMem, l + k, t, r - k, b);
            } else {
                ::Ellipse(s.dcMem, l, t, r, t + (r - l));
                ::Ellipse(s.dcMem, l, b - (r - l), r, b);
                const int k = (r - l) / 2;
                ::Rectangle(s.dcMem, l, t + k, r, b - k);
            }
            ::SelectObject(s.dcMem, hOldBrush);
            ::SelectObject(s.dcMem, hOldPen);
            ::DeleteObject(br);
        }

        int xPrev = -1, yPrev = -1;
        double theta = dAngle;
        if (dAngleStop > theta) {
            const double dMid = (dAngle + dAngleEnd) * 0.5;
            const double dRadius = (double)radius;
            int i = 0;
            do {
                const int k = (dMid >= theta) ? i : 360 - i;
                const double dk = (double)k;
                const double R = clamp255(dk * dR + 0.5 + sR);
                const double G = clamp255(dk * dG + 0.5 + sG);
                const double B = clamp255(dk * dB + 0.5 + sB);
                unsigned long clr;
                if (j == 0 && colorBorder != (unsigned long)-1)
                    clr = colorBorder;
                else
                    clr = ((unsigned)(unsigned char)(int)B << 16) | ((unsigned)(unsigned char)(int)G << 8) | (unsigned)(unsigned char)(int)R;
                const DWORD pix = DibSwap(clr);

                int x = (int)(std::cos(theta) * dRadius) + xCenter;
                int y = (int)(std::sin(theta) * dRadius) + yCenter;
                if (delta > 0) {
                    const int sh = (int)((double)delta * -0.5);
                    if (x > xCenter) x -= sh; else x += sh;
                    if (xPrev != -1 && (x > xCenter) != (xPrev > xCenter)) {
                        const int lo = (x < xPrev) ? x : xPrev;
                        const int hi = (x > xPrev) ? x : xPrev;
                        for (int xx = lo; xx < hi; ++xx)
                            PutRing(xx, y, pix);
                    }
                } else if (delta < 0) {
                    const int sh = (int)((double)delta * 0.5);
                    if (y > yCenter) y -= sh; else y += sh;
                    if (yPrev != -1 && (y > yCenter) != (yPrev > yCenter)) {
                        const int lo = (y < yPrev) ? y : yPrev;
                        const int hi = (y > yPrev) ? y : yPrev;
                        for (int yy = lo; yy < hi; ++yy)
                            PutRing(x, yy, pix);
                    }
                }
                PutRing(x, y, pix);
                xPrev = x;
                yPrev = y;
                theta += PI / 180.0;
                ++i;
            } while (dAngleStop > theta);
        }
        ::InflateRect(AsRECT(pRect), -1, -1);
    }

    ::BitBlt(DcHdc(pThis), left, top, cx, cy, s.dcMem, 0, 0, SRCCOPY);
    s.Close();
    return TRUE;
}

// void CDrawingManager::DrawLineA(double x1, double y1, double x2, double y2, COLORREF clrLine)
// RVA 0x59880 (mfc140u).  Xiaolin Wu anti-aliased line into a 32-bpp DIB that is
// then alpha-blended (crt fabs / floor via IAT 0x1802c7540 / 0x1802c7538).
//     clrLine == -1 -> return; dx = x2-x1, dy = y2-y1; both 0 -> return              (0x598f0..0x59926)
//     steep = fabs(dy) >= fabs(dx): exchange x and y of both points (and dx, dy)      (0x59942..0x5996e)
//     x1 > x2 -> exchange the points, recompute dx, dy                                (0x59972..0x59999)
//     gradient = dy / dx
//     xend = floor(x1+0.5); yend1 = (xend-x1)*gradient + y1; xgap1 = 1 - fpart(x1+0.5)
//     xpxl1 = (int)xend; ypxl1 = (int)floor(yend1); fy1 = fpart(yend1)                (0x599a3..0x59a23)
//     xend = floor(x2+0.5); yend2 = (xend-x2)*gradient + y2; xgap2 = 1 - fpart(x2-0.5)  [sic: x2 - 0.5]
//     xpxl2 = (int)xend; ypxl2 = (int)floor(yend2); fy2 = fpart(yend2)                (0x59a28..0x59aa2)
//     rect = bounding box of the two endpoint pixels in screen axes;
//        right = maxx + 1 (+1 more when steep), bottom = maxy + 1 (+1 more when !steep)
//        width == 0 || height == 0 -> return                                          (0x59aa7..0x59b44)
//     DibScratch (width x height)
//     pixel(c) = ((int)(c*255) << 24) | (byte)(R*c) << 16 | (byte)(G*c) << 8 | (byte)(B*c)
//     endpoint 1: pixel((1-fy1)*xgap1) at (xpxl1, ypxl1), pixel(fy1*xgap1) at (xpxl1, ypxl1+1)
//     endpoint 2: pixel((1-fy2)*xgap2) at (xpxl2, ypxl2), pixel(fy2*xgap2) at (xpxl2, ypxl2+1)  (0x59c3c..0x59ed4)
//     intery = yend1 + gradient; for x in [xpxl1+1, xpxl2-1]:                         (0x59ed8..0x5a009)
//        f = fpart(intery); A = (int)(f*255); Rf = (int)(f*R) ...
//        (x, floor(intery))   = (0xFF000000 - (A << 24)) | (byte)(R-Rf) << 16 | (byte)(G-Gf) << 8 | (byte)(B-Bf)
//        (x, floor(intery)+1) = (A << 24) | (byte)Rf << 16 | (byte)Gf << 8 | (byte)Bf
//        intery += gradient
//     (steep: (x, y) above are (screen y, screen x); DIB rows are bottom-up:
//      idx = (height - 1 + (top - sy)) * width + (sx - left))
//     DrawAlpha(&m_dc, rect, &dcMem, CRect(0,0,width,height)); close                  (0x5a03d)
// Symbol: ?DrawLineA@CDrawingManager@@QEAAXNNNNK@Z
extern "C" void MS_ABI impl__DrawLineA_CDrawingManager__QEAAXNNNNK_Z(void* pThis, double x1, double y1, double x2, double y2, unsigned long clrLine) {
    if (clrLine == (unsigned long)-1) return;
    double dx = x2 - x1;
    double dy = y2 - y1;
    if (dx == 0.0 && dy == 0.0) return;

    const bool bSteep = std::fabs(dy) >= std::fabs(dx);
    if (bSteep) {
        double t = x1; x1 = y1; y1 = t;
        t = x2; x2 = y2; y2 = t;
        t = dx; dx = dy; dy = t;
    }
    if (x1 > x2) {
        double t = x1; x1 = x2; x2 = t;
        t = y1; y1 = y2; y2 = t;
        dx = x2 - x1;
        dy = y2 - y1;
    }
    const double gradient = dy / dx;

    const double xe1   = x1 + 0.5;
    const double yend1 = (std::floor(xe1) - x1) * gradient + y1;
    const double xgap1 = 1.0 - (xe1 - std::floor(xe1));
    const int xpxl1 = (int)std::floor(xe1);
    const int ypxl1 = (int)std::floor(yend1);
    const double fy1 = yend1 - std::floor(yend1);

    const double xe2   = x2 + 0.5;
    const double yend2 = (std::floor(xe2) - x2) * gradient + y2;
    const double xm2   = x2 - 0.5;
    const double xgap2 = 1.0 - (xm2 - std::floor(xm2));
    const int xpxl2 = (int)std::floor(xe2);
    const int ypxl2 = (int)std::floor(yend2);
    const double fy2 = yend2 - std::floor(yend2);

    // endpoint pixels in screen axes
    const int rx1 = bSteep ? ypxl1 : xpxl1, ry1 = bSteep ? xpxl1 : ypxl1;
    const int rx2 = bSteep ? ypxl2 : xpxl2, ry2 = bSteep ? xpxl2 : ypxl2;
    CRect rect;
    rect.left = (rx1 > rx2) ? rx2 : rx1;
    rect.top  = (ry1 <= ry2) ? ry1 : ry2;
    const int maxx = (rx1 <= rx2) ? rx2 : rx1;
    const int maxy = (ry1 <= ry2) ? ry2 : ry1;
    rect.right  = maxx + 1 + (bSteep ? 1 : 0);
    rect.bottom = maxy + 1 + (bSteep ? 0 : 1);
    const int width  = rect.right - rect.left;
    const int height = rect.bottom - rect.top;
    if (width == 0 || height == 0) return;

    DibScratch s;
    if (!s.Open(pThis, width, height)) return;
    DWORD* const p = s.pBits;

    const double c1a = (1.0 - fy1) * xgap1;
    const double c1b = fy1 * xgap1;
    double intery = yend1 + gradient;
    const double c2a = (1.0 - fy2) * xgap2;
    const double c2b = fy2 * xgap2;

    const int R = (int)(clrLine & 0xffu), G = (int)((clrLine >> 8) & 0xffu), B = (int)((clrLine >> 16) & 0xffu);
    const double dR = (double)R, dG = (double)G, dB = (double)B;
    auto pixel = [&](double c) -> DWORD {
        return ((unsigned)(int)(c * 255.0) << 24)
             | ((unsigned)(unsigned char)(int)(dR * c) << 16)
             | ((unsigned)(unsigned char)(int)(dG * c) << 8)
             |  (unsigned)(unsigned char)(int)(dB * c);
    };
    // (x, y) in the line's own axes -> DIB index (bottom-up rows)
    auto idx = [&](int x, int y) -> long long {
        const int sx = bSteep ? y : x;
        const int sy = bSteep ? x : y;
        return (long long)((height - 1 + (rect.top - sy)) * width) + (long long)(sx - rect.left);
    };
    // the second pixel of each pair is one step along the minor axis: +1 column
    // when steep, one row further down (idx - width) otherwise
    const long long minorStep = bSteep ? 1 : -(long long)width;

    long long i1 = idx(xpxl1, ypxl1);
    p[i1] = pixel(c1a);
    p[i1 + minorStep] = pixel(c1b);
    long long i2 = idx(xpxl2, ypxl2);
    p[i2] = pixel(c2a);
    p[i2 + minorStep] = pixel(c2b);

    for (int x = xpxl1 + 1; x <= xpxl2 - 1; ++x) {
        const double fl = std::floor(intery);
        const double f = intery - fl;
        const int ypx = (int)fl;
        const int A  = (int)(f * 255.0);
        const int Bf = (int)(f * dB);
        const int Rf = (int)(f * dR);
        const int Gf = (int)(f * dG);
        const DWORD inner = ((unsigned)A << 24)
                          | ((unsigned)(unsigned char)Rf << 16)
                          | ((unsigned)(unsigned char)Gf << 8)
                          |  (unsigned)(unsigned char)Bf;
        const DWORD outer = (0xff000000u - ((unsigned)A << 24))
                          | ((unsigned)(unsigned char)(R - (unsigned char)Rf) << 16)
                          | ((unsigned)(unsigned char)(G - (unsigned char)Gf) << 8)
                          |  (unsigned)(unsigned char)(B - (unsigned char)Bf);
        const long long i = idx(x, ypx);
        p[i] = outer;
        p[i + minorStep] = inner;
        intery += gradient;
    }

    S_CDCHead dcTmp = { nullptr, s.dcMem, s.dcMem };
    CRect rectSrc(0, 0, width, height);
    impl__DrawAlpha_CDrawingManager__QEAAXPEAVCDC__AEBVCRect__01_Z(pThis, DmDC(pThis), &rect, AsCDC(&dcTmp), &rectSrc);
    s.Close();
}
