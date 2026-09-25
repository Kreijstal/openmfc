// CMFCVisualManagerVS2005 — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

//=============================================================================
// Retail decoding notes.
//
// Everything below was read out of the retail mfc140.dll shipped with MSVC
// 14.51.36231 (x64), the ANSI twin of the mfc140u.dll OpenMFC reimplements.
// Function BODIES are byte-identical between the two images, so control flow,
// member offsets and constants hold for either; ADDRESSES do not.  Every
// instruction or data address quoted in this file is an mfc140 address and is
// tagged "(mfc140)"; each export's entry is also given for mfc140u, resolved
// through the export table (ordinal -> EAT, urva.py) and tagged "(mfc140u)".
//
// Class shape.  Retail CMFCVisualManagerVS2005 derives from
// CMFCVisualManagerOffice2003 -> OfficeXP -> CMFCVisualManager
// (afxvisualmanagervs2005.h:27) and adds m_colorActiveTabBorder (+0x2f0),
// m_penActiveTabBorder (+0x2f8), m_brMenuButtonDroppedDown (+0x308),
// m_brMenuItemCheckedHighlight (+0x318) and m_CurrAppTheme (+0x328); the
// offsets are the ones the constructor (0x1b3260, mfc140) and
// OnUpdateSystemColors (0x1b33c0, mfc140) store to.  OpenMFC declares the class
// as CMFCVisualManager plus char _pad[64] (include/openmfc/afxmfc.h), 0x148
// bytes (asserted below), so of the retail state only the OfficeXP colour
// words +0x108..+0x147 have storage.  The consequences, stated once:
//
//  * m_CurrAppTheme (+0x328) is, in retail, the value OnUpdateSystemColors
//    stores from this->GetStandardWindowsTheme() (vftable +0x60, slot 12,
//    which the retail VS2005 vftable at 0x18031ba20 (mfc140) holds as
//    CMFCBaseVisualManager::GetStandardWindowsTheme).  The bodies here call
//    that thunk instead of reading the member.  OpenMFC's thunk returns 0
//    (WinXpTheme_NonStandard) because no UxTheme state is modelled
//    (CMFCBaseVisualManager.cpp), so every `m_CurrAppTheme in {1,2}` /
//    `in {1,2,3}` arm below is unreachable in this build.
//  * m_colorActiveTabBorder (+0x2f0) is set to -1 by the constructor (store
//    at 0x1b32c9); the only other writer among this class's bodies is
//    OnUpdateSystemColors, which stores -1 unless ::GetThemeColor on the
//    UxTheme handle at this->[+0x30] succeeds (0x1b3417..0x1b3452).  That
//    handle is never opened in OpenMFC, and OpenMFC has no storage for the
//    member anyway, so the bodies below take it as -1.  (Only the VS2005
//    bodies were checked for writers; a client class writing the protected
//    member itself is not honoured.)
//  * The Office2003 palette at +0x214..+0x238 has no storage either.  As
//    CMFCVisualManagerOffice2003.cpp and CMFCVisualManagerVS2008.cpp already
//    do, the two members these bodies read are recomputed on demand by
//    VS05_Palette() below; its comment states exactly what it reproduces.
//    When a body here delegates to an Office2003 / OfficeXP thunk, that thunk
//    recomputes its OWN palette, so the VS2005-specific adjustments that
//    retail's OnUpdateSystemColors applies to the shared members (+0x224,
//    +0x234, +0x238, +0x124, +0x114, the pen at +0x1e0 -- see that function
//    below) are not seen by the delegated-to body.  That is a limitation of
//    the recompute-on-demand model, not something this file can fix.
//
// afxGlobalData.  0x1803ba380 (mfc140) is ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A.
// OpenMFC exports it as a 720-byte blob that AFX_GLOBAL_DATA::Initialize /
// UpdateSysColors (core/runtime/AFX_GLOBAL_DATA.cpp, real bodies) populate, so
// -- exactly as CMFCVisualManagerVS2008.cpp does -- this file reads the blob
// behind the same init gate every retail body inlines, at the offsets that
// file's AfxGlobalData transcription pins: +0x3c clrBtnDkShadow, +0x40
// clrBtnLight, +0x48 clrHilite, +0x4c clrTextHilite, +0x60 clrBarFace, +0x64
// clrBarShadow, +0x6c clrBarDkShadow, +0x70 clrBarLight, +0x80
// clrCaptionText, +0xf0 brActiveCaption.m_hObject (the CBrush at +0xe8),
// +0x260 / +0x264 the two high-contrast flags, +0x288 m_nBitsPerPixel.
//
// Virtual calls.  OpenMFC's objects carry a mingw (Itanium) vtable with nothing
// at the MSVC slot indices, so -- as in every sibling visual-manager file --
// each virtual call on `this` or on an argument is replaced by the function the
// retail vftable holds in that slot, called through its impl__ thunk.  A client
// class overriding one of those virtuals is therefore not honoured.  Where the
// slot belongs to a class whose OpenMFC counterpart cannot answer it (the
// CMFCBaseTabCtrl query virtuals at +0x510..+0x530, CMFCCaptionButton's), the
// body is left a stub and says so.
//=============================================================================

// ---- afxGlobalData (featurepack/CMFC_misc_stubs.cpp, core/runtime/AFX_GLOBAL_DATA.cpp) ----
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);

// ---- RTTI (core/runtime/CObject.cpp, featurepack/toolbar/RuntimeClasses.cpp) ----
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ();

// ---- ancestor bodies.  Declared from the mangled names; they match the
// definitions in CMFCVisualManager.cpp / CMFCVisualManagerOffice2003.cpp /
// CMFCVisualManagerOfficeXP.cpp / CMFCBaseVisualManager.cpp (checked
// 2026-09-25; `this` is spelled void* here because OpenMFC's VS2005 is not
// derived from those classes). ----
extern "C" int MS_ABI impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(void* pThis);
extern "C" unsigned long MS_ABI impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(void* pThis);
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOffice2003__UEAAXXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* pGrid);
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerOffice2003__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    void* pThis, const CMFCBaseTabCtrl* pTabWnd, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack);
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    void* pThis, CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton);
extern "C" void MS_ABI impl__OnFillAutoHideButtonBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
    void* pThis, CDC* pDC, CRect rect, void* pButton /* CMFCAutoHideButton* */);
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    void* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz);
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
    void* pThis, CDC* pDC, CRect rectCaption, CPaneFrameWnd* pFrameWnd, int bActive);
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    void* pThis, CDC* pDC, void* pBar /* CDockablePane* */, int bActive, CRect rectCaption, CRect rectButtons);

// ---- CPaneFrameWnd::GetPane (featurepack/docking/CPaneFrameWnd.cpp) ----
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);

// ---- CDrawingManager (core/gdi/CDrawingManager.cpp, real bodies) ----
extern "C" void* MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC);
extern "C" void MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(
    void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish,
    int bHorz, int nStartFlatPercentage, int nEndFlatPercentage);
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKH_Z(unsigned long srcPixel, int nPercent);
extern "C" unsigned long MS_ABI impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(
    unsigned long color1, unsigned long color2, double dblLumRatio, int k1, int k2);

// ---- CDC (core/gdi/CDC.cpp, real bodies) ----
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y);
extern "C" int MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y);

// ---- MFC's exported operator new, ??2@YAPEAX_K@Z (detail/MemcoreSupport.cpp) ----
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

// ---- statics ----
// CTabbedPane::m_StyleTabWnd (retail static at 0x1803b7034 (mfc140));
// defined in featurepack/docking/CTabbedPane.cpp.
extern "C" int impl__m_StyleTabWnd_CTabbedPane__2W4Style_CMFCTabCtrl__A;
// CMFCToolBar::m_sizeButton / m_sizeCurButton / m_sizeImage / m_sizeCurImage
// (retail statics at 0x1803aaf88 / 0x1803aafc8 / 0x1803aae38 / 0x1803aad08
// (mfc140), names read with name.py); defined in
// featurepack/toolbar/CMFCToolBar.cpp as { long cx; long cy; } pairs.
struct VS05_StaticCSize { long cx; long cy; };
extern "C" VS05_StaticCSize impl__m_sizeButton_CMFCToolBar__1VCSize__A;
extern "C" VS05_StaticCSize impl__m_sizeCurButton_CMFCToolBar__1VCSize__A;
extern "C" VS05_StaticCSize impl__m_sizeImage_CMFCToolBar__1VCSize__A;
extern "C" VS05_StaticCSize impl__m_sizeCurImage_CMFCToolBar__1VCSize__A;

// This file's own export, called by the two auto-hide bodies (defined below).
extern "C" int MS_ABI impl__CreateAutoHideButtonRegion_CMFCVisualManagerVS2005__IEAAHVCRect__KAEAPEAUtagPOINT___Z(
    void* pThis, CRect rect, unsigned long dwAlignment, POINT*& points);

static_assert(sizeof(CMFCVisualManager) == 0x108, "CMFCVisualManager ends where the OfficeXP colour words begin");
static_assert(sizeof(CMFCVisualManagerVS2005) == 0x148, "OpenMFC's VS2005 object: CMFCVisualManager + 64 bytes (+0x108..+0x147)");

namespace {

//--------------------------------------------------------------------------
// afxGlobalData access (the pattern of CMFCVisualManagerVS2008.cpp).
//--------------------------------------------------------------------------

// The init gate every retail reader inlines:
//     if (afxGlobalData.m_bInitialized == 0) { afxGlobalData.Initialize(); m_bInitialized = 1; }
inline void GD_Init()
{
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
}

inline unsigned GD_U32(std::size_t off)
{
    GD_Init();
    unsigned v = 0;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}

inline void* GD_Ptr(std::size_t off)
{
    GD_Init();
    void* p = nullptr;
    std::memcpy(&p, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof p);
    return p;
}

inline COLORREF GD_clrBtnDkShadow() { return GD_U32(0x3c); }
inline COLORREF GD_clrBtnLight()    { return GD_U32(0x40); }
inline COLORREF GD_clrHilite()      { return GD_U32(0x48); }
inline COLORREF GD_clrTextHilite()  { return GD_U32(0x4c); }
inline COLORREF GD_clrBarShadow()   { return GD_U32(0x64); }
inline COLORREF GD_clrBarDkShadow() { return GD_U32(0x6c); }
inline COLORREF GD_clrBarLight()    { return GD_U32(0x70); }
inline COLORREF GD_clrCaptionText() { return GD_U32(0x80); }
inline HBRUSH   GD_brActiveCaption(){ return static_cast<HBRUSH>(GD_Ptr(0xf0)); }

// bpp (+0x288) > 8 && [+0x264] == 0 && [+0x260] == 0, tested in that order --
// the gate every rich-colour arm below opens with.
inline bool VS05_RichColorDesktop()
{
    return static_cast<int>(GD_U32(0x288)) > 8 && GD_U32(0x264) == 0 && GD_U32(0x260) == 0;
}

// CBRS_ALIGN_* (afxres.h), the values the `and $0xf000` / `cmp $0x1000..` runs test.
constexpr unsigned long kAlignLeft   = 0x1000;
constexpr unsigned long kAlignTop    = 0x2000;
constexpr unsigned long kAlignRight  = 0x4000;
constexpr unsigned long kAlignBottom = 0x8000;
constexpr unsigned long kAlignAny    = 0xF000;

// m_CurrAppTheme (+0x328): see the file header.
inline int VS05_CurrAppTheme(void* pThis)
{
    return impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(pThis);
}

inline int VS05_IsKindOf(const void* pObj, const CRuntimeClass* pClass)
{
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pObj), pClass);
}

// CMFCAutoHideButton::m_dwAlignment -- retail afxautohidebutton.h member order
// after the CObject vptr (m_bTop +0x08, m_bVisible +0x0c, m_bHighlighted +0x10,
// m_dwAlignment +0x14); both auto-hide bodies read `0x14(pButton)`.  The class
// is opaque in OpenMFC (detail/CMFCAutoHideButtonSupport.h names the same word
// m_dwStyle), so it is read by offset.
inline unsigned long VS05_AutoHideAlignment(const void* pButton)
{
    unsigned long v = 0;
    std::memcpy(&v, static_cast<const char*>(pButton) + 0x14, sizeof v);
    return v;
}

inline COLORREF VS05_ReadColor(const void* pObject, std::ptrdiff_t off)
{
    COLORREF v = 0;
    std::memcpy(&v, static_cast<const char*>(pObject) + off, sizeof v);
    return v;
}

inline void VS05_WriteColor(void* pObject, std::ptrdiff_t off, COLORREF clr)
{
    std::memcpy(static_cast<char*>(pObject) + off, &clr, sizeof clr);
}

//--------------------------------------------------------------------------
// The two Office2003 palette members the bodies below read (+0x214, +0x218),
// recomputed.  Both are only read on the rich-colour path.
//
// For a VS2005 object retail builds them in this order (all mfc140):
//  1. VS2005::OnUpdateSystemColors (0x1b33c0) latches the static
//     m_bDefaultWinXPColors (0x1b33d6) and, unless m_CurrAppTheme is 3, CLEARS
//     it (0x1b33fc) before calling Office2003::OnUpdateSystemColors (0x1b3409).
//  2. Office2003 therefore forces its style to 0 unless it is -1
//     (0x190bb4..0x190bc9), so +0x2e8 is 0 and the `default` arm of its
//     style switch runs (0x190ec4..0x190f80):
//        base   = GetBaseThemeColor()          (vftable +0x640; with +0x2e8 == 0
//                                               retail 0x195940 returns
//                                               afxGlobalData.clrBarFace)
//        +0x214 = SmartMixColors(base, sys(COLOR_3DLIGHT), 0.99, 2, 1)  0x190f3a
//        +0x220 = +0x218 = SmartMixColors(base, sys(COLOR_WINDOW), 1.0, 1, 4)  0x190f7b/0x190f86
//     where sys(i) is ::GetThemeSysColor on the HTHEME at this->[+0x08] or,
//     when that is NULL (always, in OpenMFC), ::GetSysColor(i).  The two
//     constants were read from the image (0x348ba8 = 0.99, 0x348bb0 = 1.0).
//  3. Back in VS2005, if the latched static was non-zero, the rich path and
//     m_CurrAppTheme in {1,2} it rewrites (0x1b3506..0x1b3574, 0x1b373b..0x1b375f):
//        +0x220 = SmartMixColors(base, sys(COLOR_WINDOW), 1.0, 3, 2)
//        +0x218 = PixelAlpha(+0x220, 0x5f)
//        +0x214 = PixelAlpha(+0x214, 0x61)
//     (It also restores the static at 0x1b384b, so the net effect of an update
//     on m_bDefaultWinXPColors is nil.)
// APPROXIMATIONS: the static's CURRENT value stands in for its value when the
// last update latched it (OpenMFC keeps no update history); and the one case
// that needs UxTheme -- m_CurrAppTheme == 3 with the static set, where
// Office2003 keeps style 3 and reads its palette through GetThemeColor -- is
// given the default-arm values.  Neither matters in this build: m_CurrAppTheme
// is always 0 here (see the file header), so step 3 never applies either.
//--------------------------------------------------------------------------
struct VS05Palette {
    COLORREF c214;
    COLORREF c218;
};

VS05Palette VS05_Palette(void* pThis)
{
    const COLORREF base = impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(pThis);
    VS05Palette p;
    p.c214 = impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(base, ::GetSysColor(COLOR_3DLIGHT), 0.99, 2, 1);
    p.c218 = impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(base, ::GetSysColor(COLOR_WINDOW), 1.0, 1, 4);

    const int nTheme = VS05_CurrAppTheme(pThis);
    if (impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA != 0 &&
        static_cast<unsigned>(nTheme - 1) <= 1u) {
        const COLORREF c220 = impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(
            base, ::GetSysColor(COLOR_WINDOW), 1.0, 3, 2);
        p.c218 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(c220, 0x5f);
        p.c214 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(p.c214, 0x61);
    }
    return p;
}

//--------------------------------------------------------------------------
// GDI helpers.  Retail builds CBrush / CPen / CRgn objects and selects them
// through CDC::SelectObject / SelectClipRgn; those C++ methods exist only as
// impl__ thunks inside this DLL, so raw handles are used and released here
// (the same substitution the sibling visual-manager files make).
//--------------------------------------------------------------------------
inline RECT VS05_ToRECT(const CRect& r) { return RECT{ r.left, r.top, r.right, r.bottom }; }

// `CDrawingManager dm(*pDC); dm.FillGradient(rect, colorStart, colorFinish, bHorz, 0, 0);`
void VS05_FillGradient(CDC* pDC, CRect rect, COLORREF colorStart, COLORREF colorFinish, int bHorz)
{
    struct { const void* vptr; CDC* pDC; } dm = { nullptr, nullptr };
    impl___0CDrawingManager__QEAA_AEAVCDC___Z(&dm, pDC);
    impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(&dm, &rect, colorStart, colorFinish, bHorz, 0, 0);
    impl___1CDrawingManager__UEAA_XZ(&dm);
}

inline void VS05_Draw3dRect(CDC* pDC, const CRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
    RECT r = VS05_ToRECT(rect);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &r, clrTopLeft, clrBottomRight);
}

//--------------------------------------------------------------------------
// CMFCToolBar size helpers, as OnDrawSeparator (0x1b4b70, mfc140) inlines them.
// Member offsets are the harvested CMFCToolBar layout in include/openmfc/afxmfc.h
// (m_bLocked 0x10b8, m_bLargeIconsAreEnbaled 0x10bc, m_sizeButtonLocked 0x1158,
// m_sizeImageLocked 0x1160, m_sizeCurButtonLocked 0x1168, m_sizeCurImageLocked
// 0x1170), which are the offsets the retail bodies read.
//--------------------------------------------------------------------------
// GetImageSize(), inlined at 0x1b4bba..0x1b4bf2 (and three more times):
//   m_bLocked ? (large ? m_sizeCurImageLocked : m_sizeImageLocked)
//             : (large ? CMFCToolBar::m_sizeCurImage : CMFCToolBar::m_sizeImage)
inline CSize VS05_ToolBarImageSize(const CMFCToolBar* pBar)
{
    if (pBar->m_bLocked)
        return pBar->m_bLargeIconsAreEnbaled ? pBar->m_sizeCurImageLocked : pBar->m_sizeImageLocked;
    const VS05_StaticCSize& s = pBar->m_bLargeIconsAreEnbaled ? impl__m_sizeCurImage_CMFCToolBar__1VCSize__A
                                                              : impl__m_sizeImage_CMFCToolBar__1VCSize__A;
    return CSize(static_cast<int>(s.cx), static_cast<int>(s.cy));
}
// GetButtonSize(), the non-exported helper at 0x23a74 (mfc140):
//   sz = m_bLocked ? (large ? m_sizeCurButtonLocked : m_sizeButtonLocked)
//                  : (large ? CMFCToolBar::m_sizeCurButton : CMFCToolBar::m_sizeButton);
//   if (this->vftable[217]())                                  // +0x6c8, call at 0x23abc
//       sz += CMFCVisualManager::GetInstance()->vftable[82](); // +0x290, GetButtonExtraBorder
// Slot 82 of the retail VS2005 vftable (0x18031ba20, mfc140) is 0x8960, the
// folded `return CSize(0,0)` body CMFCVisualManagerOffice2003.cpp identifies
// for the Office2003 vftable, so while a VS2005 manager is the instance the
// addition is zero whatever slot 217 answers, and neither virtual is
// dispatched here.  DEVIATION: a different current manager (or a client class
// overriding GetButtonExtraBorder) would add its extra border in retail.
inline CSize VS05_ToolBarButtonSize(const CMFCToolBar* pBar)
{
    if (pBar->m_bLocked)
        return pBar->m_bLargeIconsAreEnbaled ? pBar->m_sizeCurButtonLocked : pBar->m_sizeButtonLocked;
    const VS05_StaticCSize& s = pBar->m_bLargeIconsAreEnbaled ? impl__m_sizeCurButton_CMFCToolBar__1VCSize__A
                                                              : impl__m_sizeButton_CMFCToolBar__1VCSize__A;
    return CSize(static_cast<int>(s.cx), static_cast<int>(s.cy));
}

// A growable POINT array on malloc/realloc, standing in for the CList the
// retail CreateAutoHideButtonRegion builds (std::vector would pull a libstdc++
// throw helper into this DLL).  An allocation failure leaves the array short;
// see that function's DEVIATION note.
struct VS05PointBuf {
    POINT* p = nullptr;
    std::size_t n = 0;
    std::size_t cap = 0;
    bool failed = false;
    ~VS05PointBuf() { std::free(p); }
    void push(int x, int y)
    {
        if (n == cap) {
            const std::size_t newCap = cap ? cap * 2 : 32;
            void* q = std::realloc(p, newCap * sizeof(POINT));
            if (q == nullptr) { failed = true; return; }
            p = static_cast<POINT*>(q);
            cap = newCap;
        }
        p[n].x = x;
        p[n].y = y;
        ++n;
    }
};

} // namespace

// Symbol: ?CreateObject@CMFCVisualManagerVS2005@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerVS2005__SAPEAVCObject__XZ() { return new CMFCVisualManagerVS2005(); }
// Symbol: ?GetThisClass@CMFCVisualManagerVS2005@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerVS2005::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2005@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerVS2005__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerVS2005* pThis) { return CMFCVisualManagerVS2005::GetThisClass(); }
// The retail constructor (??0CMFCVisualManagerVS2005@@QEAA@XZ, entry 0x1b3260
// (mfc140)) does NOT touch CMFCVisualManagerVS2005::m_bRoundedAutohideButtons:
// that static lives in .bss (0x1803b7134 (mfc140), so it starts FALSE) and a
// scan of the whole mfc140 image finds only readers of it (0x1b3210, 0x1b431a,
// 0x1b4781, 0x1b4990) -- it is a public knob for the application to set.  An
// earlier version of this constructor stored TRUE into it, which silently
// switched both auto-hide bodies below onto their rounded-outline paths; that
// store has been removed.  The static stores retail's constructor DOES make
// (CDockingManager::m_bDockBarMenu = 1 at 0x1b32d3, m_dockModeGlobal = 0x81 at
// 0x1b32dc, CMFCAutoHideButton::m_bOverlappingTabs = 0 at 0x1b32ec, and an
// unnamed static at 0x1803b6f6c = 0) and its member stores are not reproduced.
CMFCVisualManagerVS2005::CMFCVisualManagerVS2005() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005() {}
CObject* CMFCVisualManagerVS2005::CreateObject() { return new CMFCVisualManagerVS2005(); }

// CMFCVisualManagerVS2005::CreateAutoHideButtonRegion(CRect rect, DWORD
// dwAlignment, LPPOINT& points) -- entry 0x1b4250 (mfc140), 0x1b5c50 (mfc140u).
// Builds the outline of an auto-hide tab as a CList<POINT,POINT> and copies it
// into a new[]'d array.  Transcribed in full:
//     align = dwAlignment & CBRS_ALIGN_ANY;                        // 0x1b427e
//     if (align == LEFT) rect.right--; else if (align == TOP) rect.bottom--;   // 0x1b42a6 / 0x1b42a1
//     rectOrign = rect;                          // left/top/right kept at -0x1/-0x9/-0x5(%rbp)
//     if (align == LEFT || align == RIGHT)
//         rect = CRect(0, 0, rectOrign.Height(), rectOrign.Width());   // 0x1b42d6..0x1b42ec
//     if (!m_bRoundedAutohideButtons) {                            // static 0x1803b7134, 0x1b431a
//         rect.right--;                                            // 0x1b432a
//         AddHead x6: (l,t) (l,b-2) (l+2,b) (r-2,b) (r,b-2) (r,t)  // 0x18ea50 calls
//     } else {
//         posLeft  = InsertAfter(AddHead((l,t)), (l,t+2));         // 0x1b43c5 / 0x1b43e5
//         posRight = InsertBefore(AddTail((r,t)), (r,t+2));        // inline AddTail, 0x1b442c
//         xLeft = l+1; xRight = r-1; bIsHorz = (align == LEFT || align == RIGHT);
//         for (y = t+2; y < b-4; y += 2) {                         // 0x1b445c..0x1b44e1
//             posLeft  = InsertAfter (posLeft,  (xLeft, y));  posLeft  = InsertAfter (posLeft,  (xLeft, y+2));
//             posRight = InsertBefore(posRight, (xRight, y)); posRight = InsertBefore(posRight, (xRight, y+2));
//             xLeft++; xRight--; }
//         if (align == BOTTOM && !bIsHorz) { xLeft--; xRight++; }  // 0x1b44f2
//         if (bIsHorz) xRight++;                                   // the cmove at 0x1b450c
//         for (; y < b-1; y++) {                                   // 0x1b4510..0x1b4617
//             posLeft  = InsertAfter (posLeft,  (xLeft, y));  posLeft  = InsertAfter (posLeft,  (xLeft+1, y+1));
//             posRight = InsertBefore(posRight, (xRight, y)); posRight = InsertBefore(posRight, (xRight-1, y+1));
//             if (y == b-2) {
//                 posLeft  = InsertAfter (posLeft,  (xLeft+1, y+1)); posLeft  = InsertAfter (posLeft,  (xLeft+3, y+1));
//                 posRight = InsertBefore(posRight, (xRight, y+1));  posRight = InsertBefore(posRight, (xRight-2, y+1)); }
//             xLeft++; xRight--; }
//         InsertAfter(posLeft, (xLeft+2, b)); InsertBefore(posRight, (xRight-2, b));   // 0x1b463c / 0x1b4657
//     }
//     points = new POINT[count];                                   // ??2 at 0x1b4681
//     for each node i (head to tail) {                             // 0x1b46a5..0x1b4726
//         points[i] = node;
//         BOTTOM: points[i].y = rect.top - points[i].y + rect.bottom;
//         RIGHT:  (x, y) = (rectOrign.right - y, rectOrign.top + x);
//         LEFT:   (x, y) = (rectOrign.left  + y, rectOrign.top + x);
//     }
//     return count;
// The list helpers were identified from their bodies: 0x18ea50 links a new
// node in front of m_pNodeHead (AddHead), 0x18eaf4 links it after `pos`
// (InsertAfter), 0x18ea94 before `pos` (InsertBefore), and the sequence at
// 0x1b43ee..0x1b4417 is an inlined AddTail.  Since every left-side insert goes
// after the previous one and every right-side insert before the previous one,
// the finished list is the left-side points in insertion order followed by
// the right-side points in reverse insertion order; that is how it is built
// here (six AddHead calls are likewise the reverse of their insertion order).
// Allocation: retail calls MFC's own operator new (??2@YAPEAX_K@Z, 0x2840 --
// a malloc loop) with count*8, saturated to SIZE_MAX on overflow (the cmovb at
// 0x1b467a); the export is called here too, and its callers free the array
// with the CRT free() import exactly as retail's (0x1802c5558, mfc140).
// DEVIATION: when an allocation fails this body returns 0 with points == NULL.
// Retail has no such path: its operator new retries through the new handler
// in AFX_MODULE_THREAD_STATE (+0x50; AfxGetModuleThreadState is called at
// 0x285e inside ??2 (0x2840))
// and returns NULL only when no handler is installed or the handler gives up,
// after which the caller writes through the NULL; the CList node allocator
// (0x12218) has its own failure behaviour.  Here the scratch buffers can fail
// as well.  `this` is not read.
// Symbol: ?CreateAutoHideButtonRegion@CMFCVisualManagerVS2005@@IEAAHVCRect@@KAEAPEAUtagPOINT@@@Z
extern "C" int MS_ABI impl__CreateAutoHideButtonRegion_CMFCVisualManagerVS2005__IEAAHVCRect__KAEAPEAUtagPOINT___Z(
    void* /*pThis*/, CRect rect, unsigned long dwAlignment, POINT*& points)
{
    const unsigned long align = dwAlignment & kAlignAny;
    if (align == kAlignLeft)      rect.right--;
    else if (align == kAlignTop)  rect.bottom--;

    const CRect rectOrign = rect;
    const bool bIsHorz = (align == kAlignLeft || align == kAlignRight);
    if (bIsHorz) {
        rect = CRect(0, 0, rectOrign.bottom - rectOrign.top, rectOrign.right - rectOrign.left);
    }

    VS05PointBuf ptsLeft;    // head side, in list order
    VS05PointBuf ptsRight;   // tail side, in reverse list order

    if (!impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA) {
        rect.right--;
        ptsRight.push(rect.left, rect.top);
        ptsRight.push(rect.left, rect.bottom - 2);
        ptsRight.push(rect.left + 2, rect.bottom);
        ptsRight.push(rect.right - 2, rect.bottom);
        ptsRight.push(rect.right, rect.bottom - 2);
        ptsRight.push(rect.right, rect.top);
    } else {
        ptsLeft.push(rect.left, rect.top);
        ptsLeft.push(rect.left, rect.top + 2);
        ptsRight.push(rect.right, rect.top);
        ptsRight.push(rect.right, rect.top + 2);

        int xLeft = rect.left + 1;
        int xRight = rect.right - 1;
        int y = rect.top + 2;
        for (; y < rect.bottom - 4; y += 2) {
            ptsLeft.push(xLeft, y);
            ptsLeft.push(xLeft, y + 2);
            ptsRight.push(xRight, y);
            ptsRight.push(xRight, y + 2);
            xLeft++;
            xRight--;
        }
        if (align == kAlignBottom && !bIsHorz) {
            xLeft--;
            xRight++;
        }
        if (bIsHorz) xRight++;
        for (; y < rect.bottom - 1; y++) {
            ptsLeft.push(xLeft, y);
            ptsLeft.push(xLeft + 1, y + 1);
            ptsRight.push(xRight, y);
            ptsRight.push(xRight - 1, y + 1);
            if (y == rect.bottom - 2) {
                ptsLeft.push(xLeft + 1, y + 1);
                ptsLeft.push(xLeft + 3, y + 1);
                ptsRight.push(xRight, y + 1);
                ptsRight.push(xRight - 2, y + 1);
            }
            xLeft++;
            xRight--;
        }
        ptsLeft.push(xLeft + 2, rect.bottom);
        ptsRight.push(xRight - 2, rect.bottom);
    }

    points = nullptr;
    if (ptsLeft.failed || ptsRight.failed) return 0;
    const std::size_t count = ptsLeft.n + ptsRight.n;
    const std::size_t cb = (count > SIZE_MAX / sizeof(POINT)) ? SIZE_MAX : count * sizeof(POINT);
    points = static_cast<POINT*>(impl___2_YAPEAX_K_Z(cb));
    if (points == nullptr) return 0;

    std::size_t i = 0;
    auto emit = [&](POINT p) {
        switch (align) {
        case kAlignBottom:
            p.y = rect.top - p.y + rect.bottom;
            break;
        case kAlignRight: {
            const int x = rectOrign.right - p.y;
            const int yy = rectOrign.top + p.x;
            p.x = x; p.y = yy;
            break;
        }
        case kAlignLeft: {
            const int x = rectOrign.left + p.y;
            const int yy = rectOrign.top + p.x;
            p.x = x; p.y = yy;
            break;
        }
        default:
            break;
        }
        points[i++] = p;
    };
    for (std::size_t k = 0; k < ptsLeft.n; ++k) emit(ptsLeft.p[k]);
    for (std::size_t k = ptsRight.n; k-- > 0;) emit(ptsRight.p[k]);
    return static_cast<int>(count);
}

// CMFCVisualManagerVS2005::GetDockingTabsBordersSize() -- entry 0x1b4e40
// (mfc140), 0x1b6840 (mfc140u), complete body:
//     return CTabbedPane::m_StyleTabWnd == CMFCTabCtrl::STYLE_3D_ROUNDED ? 0 : 3;
// (`mov m_StyleTabWnd,%eax; sub $6; neg; sbb; and $3` -- STYLE_3D_ROUNDED is 6
// in afxtabctrl.h).  `this` is not read.
// Symbol: ?GetDockingTabsBordersSize@CMFCVisualManagerVS2005@@UEAAHXZ
extern "C" int MS_ABI impl__GetDockingTabsBordersSize_CMFCVisualManagerVS2005__UEAAHXZ(void* /*pThis*/) {
    return impl__m_StyleTabWnd_CTabbedPane__2W4Style_CMFCTabCtrl__A == 6 /*STYLE_3D_ROUNDED*/ ? 0 : 3;
}

// CMFCVisualManagerVS2005::GetPropertyGridGroupColor(CMFCPropertyGridCtrl*) --
// entry 0x1b4e60 (mfc140), 0x1b6860 (mfc140u), complete body:
//     if (CMFCVisualManagerOffice2003::m_bDefaultWinXPColors)          // 0x1b4e66
//         return CMFCVisualManager::GetPropertyGridGroupColor(pPropList);  // tail jmp 0x189d10
//     return pPropList->m_bControlBarColors (+0x5fc)                   // 0x1b4e77
//         ? afxGlobalData.clrBarLight (+0x70) : afxGlobalData.clrBtnLight (+0x40);
// DEVIATION: m_bControlBarColors is not read from the grid.  OpenMFC's
// CMFCPropertyGridCtrl is a CWnd plus 128 bytes (include/openmfc/afxmfc.h), so
// +0x5fc lies past the end of an OpenMFC-built grid; that class keeps the flag
// in a file-local side table (PropertyGridWndState::bControlBarColors in
// featurepack/propertygrid/CMFCPropertyGridCtrl.cpp) that nothing in the tree
// ever sets, i.e. it is FALSE for every grid.  The FALSE arm is therefore
// taken.  pPropList is not dereferenced, so no NULL test is needed.
// Symbol: ?GetPropertyGridGroupColor@CMFCVisualManagerVS2005@@UEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManagerVS2005__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManagerVS2005* pThis, CMFCPropertyGridCtrl* pPropList)
{
    if (impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA != 0) {
        return impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(pThis, pPropList);
    }
    return GD_clrBtnLight();
}

// CMFCVisualManagerVS2005::GetTabFrameColors(...) -- entry 0x1b4ad0 (mfc140),
// 0x1b64d0 (mfc140u), complete body:
//     CMFCVisualManagerOffice2003::GetTabFrameColors(pTabWnd, clrDark, clrBlack,
//         clrHighlight, clrFace, clrDarkShadow, clrLight, pbrFace, pbrBlack);  // 0x193a90, call at 0x1b4b2e
//     if (pTabWnd->vftable[+0x528]() /* IsVS2005Style */ &&                   // call at 0x1b4b40
//         m_colorActiveTabBorder (+0x2f0) != (COLORREF)-1)
//         clrHighlight = m_colorActiveTabBorder;
//     clrBlack = clrDarkShadow;                                               // 0x1b4b58
// m_colorActiveTabBorder is -1 for every object in this build (see the file
// header), so the conditional store can never happen and the side-effect-free
// IsVS2005Style query (the +0x528 slot name is the one
// CMFCVisualManagerOfficeXP.cpp derives; OpenMFC's tab controls cannot answer
// it anyway) is not made.  Everything else is exact.
// Symbol: ?GetTabFrameColors@CMFCVisualManagerVS2005@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerVS2005__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    void* pThis, const CMFCBaseTabCtrl* pTabWnd, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack)
{
    impl__GetTabFrameColors_CMFCVisualManagerOffice2003__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
        pThis, pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight, pbrFace, pbrBlack);
    clrBlack = clrDarkShadow;
}

// CMFCVisualManagerVS2005::OnDrawAutoHideButtonBorder(CDC*, CRect rectBounds,
// CRect rectBorderSize, CMFCAutoHideButton* pButton) -- entry 0x1b48f0
// (mfc140), 0x1b62f0 (mfc140u), complete body:
//     CPen pen(PS_SOLID, 1, afxGlobalData.clrBarShadow (+0x64));        // ??0CPen 0x2a1ed0, 0x1b493f
//     CPen* pOldPen = pDC->SelectObject(&pen);                          // 0x1b494d
//     ENSURE(pOldPen != NULL);                                          // -> 0x225b80 at 0x1b4ac1
//     LPPOINT points;
//     int nPoints = CreateAutoHideButtonRegion(rectBounds, pButton->m_dwAlignment (+0x14), points);  // 0x1b4983
//     if (!m_bRoundedAutohideButtons)                                   // 0x1b4990
//         ::Polyline(pDC->m_hDC, points, nPoints);                      // GDI32 import 0x1802c4090
//     else {
//         BOOL bIsHorz = (align == CBRS_ALIGN_LEFT || align == CBRS_ALIGN_RIGHT);
//         for (int i = 0; i < nPoints; i++) {
//             if ((i % 2) == 0) continue;                               // test $1 at 0x1b49dc
//             CPoint pt1 = points[i - 1], pt2 = points[i];
//             if (bIsHorz) { int yMid = (rectBounds.bottom + rectBounds.top) / 2;
//                            if (pt1.y > yMid && pt2.y > yMid) { pt1.y--; pt2.y--; } }
//             else         { int xMid = (rectBounds.right + rectBounds.left) / 2;
//                            if (pt1.x > xMid && pt2.x > xMid) { pt1.x--; pt2.x--; } }
//             if (pt2.y < pt1.y) { pDC->MoveTo(pt2); pDC->LineTo(pt1); }
//             else               { pDC->MoveTo(pt1); pDC->LineTo(pt2); }   // 0x2a1000 / 0x2a1060
//         }
//     }
//     pDC->SelectObject(pOldPen);                                       // 0x1b4a8b
//     delete[] points;                                                  // CRT free import 0x1802c5558
// rectBorderSize is never read.  Import slots resolved with iat.py.
// Substitutions: the pen is a raw ::CreatePen handle selected with
// ::SelectObject on m_hDC (CDC::SelectObject exists only as a thunk taking a
// CPen object); MoveTo / LineTo go through the CDC thunks.  DEVIATIONS: retail
// throws CInvalidArgException when the selection fails -- this body just stops
// there (after releasing the pen); and the NULL tests on pDC / pButton are
// OpenMFC's (retail dereferences both).
// Symbol: ?OnDrawAutoHideButtonBorder@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@1PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnDrawAutoHideButtonBorder_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(
    void* pThis, CDC* pDC, CRect rectBounds, CRect /*rectBorderSize*/, void* pButton /* CMFCAutoHideButton* */)
{
    if (pDC == nullptr || pButton == nullptr) return;

    HPEN hPen = ::CreatePen(PS_SOLID, 1, GD_clrBarShadow());
    HGDIOBJ hOldPen = (hPen != nullptr) ? ::SelectObject(pDC->m_hDC, hPen) : nullptr;
    if (hOldPen == nullptr) {
        if (hPen != nullptr) ::DeleteObject(hPen);
        return;
    }

    const unsigned long dwAlignment = VS05_AutoHideAlignment(pButton);
    POINT* points = nullptr;
    const int nPoints = impl__CreateAutoHideButtonRegion_CMFCVisualManagerVS2005__IEAAHVCRect__KAEAPEAUtagPOINT___Z(
        pThis, rectBounds, dwAlignment, points);

    if (!impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA) {
        if (points != nullptr) ::Polyline(pDC->m_hDC, points, nPoints);
    } else {
        const unsigned long align = dwAlignment & kAlignAny;
        const bool bIsHorz = (align == kAlignLeft || align == kAlignRight);
        for (int i = 0; i < nPoints; i++) {
            if ((i % 2) == 0) continue;
            int x1 = points[i - 1].x, y1 = points[i - 1].y;
            int x2 = points[i].x,     y2 = points[i].y;
            if (bIsHorz) {
                const int yMid = (rectBounds.bottom + rectBounds.top) / 2;
                if (y1 > yMid && y2 > yMid) { y1--; y2--; }
            } else {
                const int xMid = (rectBounds.right + rectBounds.left) / 2;
                if (x1 > xMid && x2 > xMid) { x1--; x2--; }
            }
            if (y2 < y1) {
                impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x2, y2);
                impl__LineTo_CDC__QEAAHHH_Z(pDC, x1, y1);
            } else {
                impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x1, y1);
                impl__LineTo_CDC__QEAAHHH_Z(pDC, x2, y2);
            }
        }
    }

    ::SelectObject(pDC->m_hDC, hOldPen);
    std::free(points);
    ::DeleteObject(hPen);
}

// CMFCVisualManagerVS2005::OnDrawCaptionButton(CDC*, CMFCCaptionButton*, BOOL
// bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled, int nImageID) -- entry
// 0x1b3c00 (mfc140), 0x1b5600 (mfc140u).  Retail delegates to
// CMFCVisualManagerOfficeXP::OnDrawCaptionButton (0x1aea90, call at 0x1b3db0)
// when bActive or pButton->[+0x30] is set; otherwise it asks the button for its
// rect (pButton vftable +0x28, call at 0x1b3c4a), fills it with
// afxGlobalData.brBarFace (the CBrush at +0x118; its handle is the +0x120 word
// read at 0x1b3c8e) when pButton->[+0x8]/[+0xc]/[+0x18] is set and bDisabled
// is clear, resolves the icon through pButton vftable
// +0x38 when nImageID == -1, centres CMenuImages::Size() in the rect, draws it
// through this->vftable[+0x618] and finally frames it with
// afxGlobalData.clrBarDkShadow (+0x6c).
// STUB: OpenMFC's CMFCCaptionButton is a CObject plus 16 bytes
// (include/openmfc/afxmfc.h) -- +0x18 and +0x30 lie past its end -- and its
// virtuals at +0x28 / +0x38 cannot be dispatched on OpenMFC's objects (the
// GetIconID export in featurepack/controls/CMFCCaptionButton.cpp still has an
// auto-generated parameter list), and the OfficeXP fallback is itself an empty
// body.  Parameter list corrected from the mangled name (it had no `this`).
// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerVS2005__UEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
    void* /*pThis*/, CDC* /*pDC*/, CMFCCaptionButton* /*pButton*/, int /*bActive*/, int /*bHorz*/,
    int /*bMaximized*/, int /*bDisabled*/, int /*nImageID*/) {}

// CMFCVisualManagerVS2005::OnDrawPaneCaption(CDC*, CDockablePane*, BOOL bActive,
// CRect rectCaption, CRect rectButtons) -- entry 0x1b3930 (mfc140), 0x1b5330
// (mfc140u), complete body:
//     if (!<rich-colour desktop>)                                       // 0x1b397a..0x1b39b6
//         return CMFCVisualManagerOfficeXP::OnDrawPaneCaption(pDC, pBar, bActive,
//                    rectCaption, rectButtons);                         // 0x1ae8e0, call at 0x1b3bd4
//     rectCaption.bottom++;                                             // 0x1b39c2
//     if (!bActive) {
//         COLORREF clr = CDrawingManager::PixelAlpha(this->[+0x214], 0x57);   // 0x1b39da
//         CBrush br(clr);  ::FillRect(pDC->m_hDC, rectCaption, br);          // 0x1b39e7 / 0x1b39f8
//         pDC->Draw3dRect(rectCaption, afxGlobalData.clrBarShadow,
//                                      afxGlobalData.clrBarShadow);          // 0x1b3a4e
//         return (GetRValue(clr) <= 0xc0 && GetGValue(clr) <= 0xc0 &&
//                 GetBValue(clr) <= 0xc0) ? RGB(255,255,255) : RGB(0,0,0);  // 0x1b3a68..0x1b3a8f
//     }
//     if ((unsigned)(m_CurrAppTheme - 1) > 2) {                         // 0x1b3a94..0x1b3aa0
//         ::FillRect(pDC->m_hDC, rectCaption, afxGlobalData.brActiveCaption);  // 0x1b3ac7
//         return afxGlobalData.clrCaptionText;                          // 0x1b3ae9
//     }
//     CDrawingManager dm(*pDC);
//     dm.FillGradient(rectCaption, afxGlobalData.clrHilite,
//         CDrawingManager::PixelAlpha(afxGlobalData.clrHilite, 0x82), TRUE, 0, 0);  // 0x1b3b79
//     return afxGlobalData.clrTextHilite;                               // 0x1b3b9a
// +0x214 comes from VS05_Palette(), m_CurrAppTheme from the thunk (file
// header).  The CBrush is a raw ::CreateSolidBrush handle.  pBar and
// rectButtons are read only by the fallback.  Retail dereferences pDC without
// a test; so does this body, on the same paths.
// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerVS2005@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerVS2005__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    void* pThis, CDC* pDC, void* pBar /* CDockablePane* */, int bActive, CRect rectCaption, CRect rectButtons)
{
    if (!VS05_RichColorDesktop()) {
        return impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
            pThis, pDC, pBar, bActive, rectCaption, rectButtons);
    }

    rectCaption.bottom++;

    if (!bActive) {
        const COLORREF clr = impl__PixelAlpha_CDrawingManager__SAKKH_Z(VS05_Palette(pThis).c214, 0x57);
        HBRUSH hbr = ::CreateSolidBrush(clr);
        RECT r = VS05_ToRECT(rectCaption);
        ::FillRect(pDC->m_hDC, &r, hbr);
        const COLORREF clrShadow = GD_clrBarShadow();
        VS05_Draw3dRect(pDC, rectCaption, clrShadow, clrShadow);
        if (hbr != nullptr) ::DeleteObject(hbr);
        if (GetRValue(clr) > 0xc0 || GetGValue(clr) > 0xc0 || GetBValue(clr) > 0xc0) return RGB(0, 0, 0);
        return RGB(255, 255, 255);
    }

    if (static_cast<unsigned>(VS05_CurrAppTheme(pThis) - 1) > 2u) {
        RECT r = VS05_ToRECT(rectCaption);
        ::FillRect(pDC->m_hDC, &r, GD_brActiveCaption());
        return GD_clrCaptionText();
    }

    const COLORREF clrHilite = GD_clrHilite();
    VS05_FillGradient(pDC, rectCaption, clrHilite, impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrHilite, 0x82), TRUE);
    return GD_clrTextHilite();
}

// CMFCVisualManagerVS2005::OnDrawSeparator(CDC*, CBasePane* pBar, CRect rect,
// BOOL bIsHoriz) -- entry 0x1b4b70 (mfc140), 0x1b6570 (mfc140u), complete body:
//     if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) {  // descriptor 0x1803aa5f8, 0x1b4bad
//         if (bIsHoriz) rect.top  += max(0, (GetButtonSize().cy - GetImageSize().cy) / 2);   // 0x1b4c6d
//         else          rect.left += max(0, (GetButtonSize().cx - GetImageSize().cx) / 2);   // 0x1b4d04
//     }
//     CMFCVisualManagerOfficeXP::OnDrawSeparator(pDC, pBar, rect, bIsHoriz);  // 0x1ada00, call at 0x1b4d22
// Only ONE edge moves (the single `add %eax,0x4(%rsi)` / `add %eax,(%rsi)`),
// and the fallback is OfficeXP's body, not Office2003's -- both as retail.  The
// delta is computed twice in retail (a max() macro); once here.  The two size
// helpers are described above VS05_ToolBarButtonSize.  The descriptor name was
// read with rtc.py.
// Symbol: ?OnDrawSeparator@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerVS2005__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    void* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bIsHoriz)
{
    if (pBar != nullptr && VS05_IsKindOf(pBar, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        const CMFCToolBar* pToolBar = static_cast<const CMFCToolBar*>(pBar);
        const CSize sizeImage = VS05_ToolBarImageSize(pToolBar);
        const CSize sizeButton = VS05_ToolBarButtonSize(pToolBar);
        if (bIsHoriz) {
            int nDelta = (sizeButton.cy - sizeImage.cy) / 2;
            if (nDelta < 0) nDelta = 0;
            rect.top += nDelta;
        } else {
            int nDelta = (sizeButton.cx - sizeImage.cx) / 2;
            if (nDelta < 0) nDelta = 0;
            rect.left += nDelta;
        }
    }
    impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
        pThis, pDC, pBar, rect, bIsHoriz);
}

// CMFCVisualManagerVS2005::OnDrawTab(CDC*, CRect rectTab, int iTab, BOOL
// bIsActive, const CMFCBaseTabCtrl* pTabWnd) -- entry 0x1b4030 (mfc140),
// 0x1b5a30 (mfc140u).  Retail opens by calling three CMFCBaseTabCtrl virtuals
// on pTabWnd -- +0x510 (IsFlatTab), +0x520 (IsOneNoteStyle), +0x528
// (IsVS2005Style), calls at 0x1b4066 / 0x1b4081 / 0x1b409c, slot names as
// CMFCVisualManagerOfficeXP.cpp derives them -- and branches on each, then
// reads the tab colours through +0x3c8 / +0x3d0.
// STUB: those virtuals cannot be dispatched on OpenMFC's CMFCBaseTabCtrl
// (a CWnd plus padding with none of them), so not even the first branch can
// be chosen -- the same limitation CMFCVisualManagerOfficeXP.cpp and
// CMFCVisualManagerVS2008.cpp record for their OnDrawTab.  Parameter list
// corrected from the mangled name (it had no `this`).
// Symbol: ?OnDrawTab@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rectTab*/, int /*iTab*/, int /*bIsActive*/,
    const CMFCBaseTabCtrl* /*pTabWnd*/) {}

// CMFCVisualManagerVS2005::OnDrawToolBoxFrame(CDC*, const CRect&) -- entry
// 0x1b4f90 (mfc140), 0x1b6990 (mfc140u), complete body:
//     pDC->Draw3dRect(rect, afxGlobalData.clrBarShadow (+0x64),
//                           afxGlobalData.clrBarShadow);       // tail jmp 0x2a3b00
// Symbol: ?OnDrawToolBoxFrame@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawToolBoxFrame_CMFCVisualManagerVS2005__UEAAXPEAVCDC__AEBVCRect___Z(
    void* /*pThis*/, CDC* pDC, const CRect& rect)
{
    const COLORREF clr = GD_clrBarShadow();
    VS05_Draw3dRect(pDC, rect, clr, clr);
}

// CMFCVisualManagerVS2005::OnEraseTabsArea(CDC*, CRect rect, const
// CMFCBaseTabCtrl* pTabWnd) -- entry 0x1b3dd0 (mfc140), 0x1b57d0 (mfc140u).
// The first thing retail does is call pTabWnd's vftable +0x510 (IsFlatTab,
// call at 0x1b3e0c); TRUE delegates to CMFCVisualManagerOfficeXP::OnEraseTabsArea
// (0x1ae830, call at 0x1b4008).  Otherwise, on a rich-colour desktop, it asks
// +0x520 and +0x528, tests pTabWnd->[+0x1fc], and fills rect with
// afxGlobalData.brBarFace / brBtnFace or a gradient over the parent frame.
// STUB: the +0x510 / +0x520 / +0x528 virtuals cannot be dispatched on
// OpenMFC's CMFCBaseTabCtrl (see OnDrawTab), and the OfficeXP fallback is an
// empty body for the same reason.  Parameter list corrected from the mangled
// name (it had no `this`).
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/) {}

// CMFCVisualManagerVS2005::OnFillAutoHideButtonBackground(CDC*, CRect rect,
// CMFCAutoHideButton* pButton) -- entry 0x1b4760 (mfc140), 0x1b6160 (mfc140u),
// complete body:
//     if (!m_bRoundedAutohideButtons) return;                           // 0x1b4781
//     LPPOINT points;
//     int nPoints = CreateAutoHideButtonRegion(rect, pButton->m_dwAlignment (+0x14), points);  // 0x1b47a3
//     CRgn rgn;
//     rgn.Attach(::CreatePolygonRgn(points, nPoints, WINDING));         // GDI32 import 0x1802c41d8
//     pDC->SelectClipRgn(&rgn);                                         // 0x2a0d60, 0x1b47e0
//     if (<rich-colour desktop>) {                                      // 0x1b4808..0x1b483b
//         BOOL bHorz = !(align == CBRS_ALIGN_LEFT || align == CBRS_ALIGN_RIGHT);   // 0x1b483d..0x1b4854
//         CDrawingManager dm(*pDC);
//         dm.FillGradient(rect, this->[+0x214], this->[+0x218], bHorz, 0, 0);    // 0x57960, 0x1b4898
//     } else
//         CMFCVisualManagerOffice2003::OnFillAutoHideButtonBackground(pDC, rect, pButton);  // 0x194d70, 0x1b48b6
//     pDC->SelectClipRgn(NULL);                                         // 0x1b48c0
//     delete[] points;                                                  // CRT free import 0x1802c5558
// Import slots resolved with iat.py.  +0x214 / +0x218 come from
// VS05_Palette().  The CRgn is a raw HRGN; OpenMFC's CDC::SelectClipRgn thunk
// is `::SelectClipRgn(m_hDC, hRgn)`, which is called directly here.  The
// Office2003 fallback is still an empty body in this tree, so on a low-colour
// desktop nothing is painted inside the clip.  DEVIATION: the NULL tests on
// pDC / pButton are OpenMFC's (retail dereferences both).
// Symbol: ?OnFillAutoHideButtonBackground@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnFillAutoHideButtonBackground_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
    void* pThis, CDC* pDC, CRect rect, void* pButton /* CMFCAutoHideButton* */)
{
    if (!impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA) return;
    if (pDC == nullptr || pButton == nullptr) return;

    const unsigned long dwAlignment = VS05_AutoHideAlignment(pButton);
    POINT* points = nullptr;
    const int nPoints = impl__CreateAutoHideButtonRegion_CMFCVisualManagerVS2005__IEAAHVCRect__KAEAPEAUtagPOINT___Z(
        pThis, rect, dwAlignment, points);

    HRGN hRgn = (points != nullptr) ? ::CreatePolygonRgn(points, nPoints, WINDING) : nullptr;
    ::SelectClipRgn(pDC->m_hDC, hRgn);

    if (VS05_RichColorDesktop()) {
        const unsigned long align = dwAlignment & kAlignAny;
        const int bHorz = (align == kAlignLeft || align == kAlignRight) ? FALSE : TRUE;
        const VS05Palette p = VS05_Palette(pThis);
        VS05_FillGradient(pDC, rect, p.c214, p.c218, bHorz);
    } else {
        impl__OnFillAutoHideButtonBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
            pThis, pDC, rect, pButton);
    }

    ::SelectClipRgn(pDC->m_hDC, nullptr);
    std::free(points);
    if (hRgn != nullptr) ::DeleteObject(hRgn);
}

// CMFCVisualManagerVS2005::OnFillHighlightedArea(CDC*, CRect, CBrush* pBrush,
// CMFCToolBarButton* pButton) -- entry 0x1b4d40 (mfc140), 0x1b6740 (mfc140u):
//     if (pButton != NULL && (unsigned)(m_CurrAppTheme - 1) <= 1) {       // 0x1b4d5f..0x1b4d73
//         CMFCToolBarMenuButton* pMenuButton = DYNAMIC_DOWNCAST(CMFCToolBarMenuButton, pButton);  // 0x1803aa748
//         BOOL bIsPopupMenu = pMenuButton && pMenuButton->m_pWndParent (+0x80)
//                             && m_pWndParent->IsKindOf(CMFCPopupMenuBar);                        // 0x1803aa478
//         if (bIsPopupMenu && (pButton->m_nStyle (+0x28) & TBBS_CHECKED (0x10000))
//             && pBrush == &m_brHighlightDn (+0x1b0))
//             { ::FillRect(pDC->m_hDC, rect, m_brMenuItemCheckedHighlight (+0x318)); return; }   // 0x1b4dda
//         if (pMenuButton && !bIsPopupMenu && pMenuButton->IsDroppedDown() /* vftable +0xf0 */)
//             { ::FillRect(pDC->m_hDC, rect, m_brMenuButtonDroppedDown (+0x308)); return; }
//     }
//     CMFCVisualManagerOffice2003::OnFillHighlightedArea(pDC, rect, pBrush, pButton);  // 0x191a80, 0x1b4e24
// The guarded block is NOT transcribed: it is keyed on m_CurrAppTheme being 1
// or 2, which cannot happen in this build (see the file header), and its two
// brushes (+0x308 / +0x318, built at 0x1b37d2..0x1b3811 from colours that
// depend on the update history) and the &m_brHighlightDn identity test have no
// storage in OpenMFC's object.  If GetStandardWindowsTheme ever returns 1 or 2
// here, that arm falls through to Office2003 instead -- a DEVIATION.  The
// reachable path, the Office2003 call, is exact.
// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerVS2005@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerVS2005__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    void* pThis, CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton)
{
    impl__OnFillHighlightedArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
        pThis, pDC, rect, pBrush, pButton);
}

// CMFCVisualManagerVS2005::OnFillMiniFrameCaption(CDC*, CRect rectCaption,
// CPaneFrameWnd* pFrameWnd, BOOL bActive) -- entry 0x1b4ed0 (mfc140), 0x1b68d0
// (mfc140u), complete body:
//     CWnd* pPane = pFrameWnd->GetPane();                               // vftable +0x360, 0x1b4ef2
//     if (pPane != NULL && pPane->IsKindOf(RUNTIME_CLASS(CMFCBaseToolBar))) {  // 0x1802dcad0, 0x1b4f07
//         ::FillRect(pDC->GetSafeHdc(), rectCaption,
//                    ::GetSysColorBrush(COLOR_BTNSHADOW));              // USER32 imports 0x1802c52f0 / 0x1802c5230
//         return afxGlobalData.clrCaptionText (+0x80);                  // 0x1b4f53
//     }
//     return CMFCVisualManagerOfficeXP::OnFillMiniFrameCaption(pDC, rectCaption,
//                pFrameWnd, bActive);                                  // 0x1affc0, call at 0x1b4f7d
// Slot +0x360 of the retail CPaneFrameWnd vftable is
// ?GetPane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ (vt_paneframewnd.txt, as
// CMFCVisualManagerOfficeXP.cpp also records); its export is called directly,
// so a derived frame's override is not reached.  The descriptor name was read
// with rtc.py; the GetSysColorBrush argument is 0x10 (COLOR_BTNSHADOW).
// DEVIATION: retail dereferences pFrameWnd unconditionally; a NULL frame is
// passed straight to the OfficeXP body here (which tests it).
// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerVS2005@@UEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerVS2005__UEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
    void* pThis, CDC* pDC, CRect rectCaption, CPaneFrameWnd* pFrameWnd, int bActive)
{
    void* pPane = (pFrameWnd != nullptr) ? impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pFrameWnd) : nullptr;
    if (pPane != nullptr &&
        VS05_IsKindOf(pPane, impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ())) {
        HBRUSH hbr = ::GetSysColorBrush(COLOR_BTNSHADOW);
        RECT r = VS05_ToRECT(rectCaption);
        ::FillRect(pDC != nullptr ? pDC->m_hDC : nullptr, &r, hbr);
        return GD_clrCaptionText();
    }
    return impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
        pThis, pDC, rectCaption, pFrameWnd, bActive);
}

// CMFCVisualManagerVS2005::OnUpdateSystemColors() -- entry 0x1b33c0 (mfc140),
// 0x1b4dc0 (mfc140u).  Retail:
//     BOOL bDefaultWinXPColors = m_bDefaultWinXPColors;                // latched, 0x1b33d6
//     m_clrPressedButtonBorder (+0x124) = (COLORREF)-1;                // 0x1b33e0
//     m_CurrAppTheme (+0x328) = GetStandardWindowsTheme();             // vftable +0x60, 0x1b33f1
//     if (m_CurrAppTheme != 3) m_bDefaultWinXPColors = FALSE;          // 0x1b33fc
//     CMFCVisualManagerOffice2003::OnUpdateSystemColors();             // 0x190b30, call at 0x1b3409
//     if (!bDefaultWinXPColors) return;                                // 0x1b340e
//     m_colorActiveTabBorder (+0x2f0): -1 unless ::GetThemeColor(this->[+0x30],
//         5, 0, 0xed9, &m_colorActiveTabBorder) succeeds               // 0x1b3417..0x1b3452
//     if (<rich-colour desktop>) {                                     // 0x1b347a..0x1b34b4
//         +0x234 = SmartMixColors(+0x230, afxGlobalData.clrBarFace, 1.5, 1, 1);    // 0x1b34ed
//         m_CurrAppTheme in {1,2}: +0x21c = PixelAlpha(+0x21c, 0x53),
//             +0x220 = SmartMixColors(GetBaseThemeColor(), sys(COLOR_WINDOW), 1.0, 3, 2);  // 0x1b3506..0x1b356e
//         m_CurrAppTheme == 1: +0x230/+0x234/+0x238 re-derived by PixelAlpha;   // 0x1b3585..0x1b35bf
//         m_CurrAppTheme == 2: +0x230..+0x238, +0x118/+0x11c/+0x120 and the
//             brushes +0x1a0/+0x1b0/+0x1c0 re-derived;                   // 0x1b35cd..0x1b36e0
//         m_CurrAppTheme not 1..3: +0x238 = +0x21c;                      // 0x1b36e7
//         ebp = PixelAlpha(+0x108, 0x6b);  edi = sys(COLOR_HIGHLIGHT);  // 0x1b36fe / 0x1b3712
//         m_CurrAppTheme in {1,2}: +0x218 / +0x214 (see VS05_Palette);   // 0x1b373b..0x1b375f
//         +0x224 = SmartMixColors(+0x21c, +0x220, 0.92, 1, 2);            // 0x1b3785
//         +0x124 = SmartMixColors(+0x138, afxGlobalData.clrBarDkShadow, 0.8, 1, 2);  // 0x1b37c7
//     }   (else ebp = +0x108, edi = +0x11c, loaded at 0x1b3422 / 0x1b342d)
//     m_brMenuButtonDroppedDown (+0x308) = CreateSolidBrush(ebp);       // 0x1b37e3
//     m_brMenuItemCheckedHighlight (+0x318) = CreateSolidBrush(edi);    // 0x1b3805
//     m_penActiveTabBorder (+0x2f8) = CreatePen(PS_SOLID, 1, +0x2f0) if +0x2f0 != -1;  // 0x1b3833
//     m_bDefaultWinXPColors = bDefaultWinXPColors;                      // restored, 0x1b384b
//     m_clrInactiveTabText (+0x114) = afxGlobalData.clrBtnDkShadow;     // 0x1b386d
//     if (<rich-colour desktop>)
//         m_penSeparator (+0x1e0) = CreatePen(PS_SOLID, 1,
//             PixelAlpha(afxGlobalData.clrBarFace, 0x54));              // 0x1b38c4..0x1b3914
// (Constants read from the image: 0x348be8 = 1.5, 0x348bb0 = 1.0, 0x348b80 =
// 0.92, 0x348b58 = 0.8.  GetThemeColor / GetSysColor / CreateSolidBrush /
// CreatePen import slots resolved with iat.py.)
// What this body does: every call and every store that has storage in
// OpenMFC's object (+0x108..+0x147, see the static_asserts) -- the latch, the
// +0x124 = -1 store, the clear-and-restore of the static around the Office2003
// thunk (which runs OfficeXP's, which writes +0x108..+0x138), and, when the
// latched value is set, the rich-path +0x124 mix (reading back the +0x138 the
// OfficeXP body just stored) and the +0x114 store.  Not reproduced, for want
// of storage: +0x214..+0x238, +0x2f0 and the GDI objects at +0x1a0..+0x1c0,
// +0x1e0, +0x2f8, +0x308, +0x318 (the palette members the bodies in this file
// need are recomputed by VS05_Palette(); +0x2f0 is -1 by construction, see
// the file header); and the m_CurrAppTheme 1/2 arms, which are unreachable in
// this build.  Nothing in the tree reads +0x114 / +0x124 back (the OfficeXP
// and Office2003 bodies recompute their palettes), so those two stores are
// made for fidelity only.  GetStandardWindowsTheme is called through its
// thunk, not through the object's vftable.
// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerVS2005@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerVS2005__UEAAXXZ(void* pThis)
{
    if (pThis == nullptr) return;

    const std::int32_t bDefaultWinXPColors = impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA;
    VS05_WriteColor(pThis, 0x124, static_cast<COLORREF>(-1));
    const int nCurrAppTheme = VS05_CurrAppTheme(pThis);
    if (nCurrAppTheme != 3) impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA = FALSE;

    impl__OnUpdateSystemColors_CMFCVisualManagerOffice2003__UEAAXXZ(pThis);

    if (!bDefaultWinXPColors) return;

    if (VS05_RichColorDesktop()) {
        VS05_WriteColor(pThis, 0x124, impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(
            VS05_ReadColor(pThis, 0x138), GD_clrBarDkShadow(), 0.8, 1, 2));
    }

    impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA = bDefaultWinXPColors;
    VS05_WriteColor(pThis, 0x114, GD_clrBtnDkShadow());
}
