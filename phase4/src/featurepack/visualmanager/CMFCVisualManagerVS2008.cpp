// CMFCVisualManagerVS2008 — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstdint>
#include <cstring>

//=============================================================================
// Retail decoding notes.
//
// Everything below was read out of the retail mfc140.dll shipped with MSVC
// 14.51.36231 (x64), the ANSI twin of the mfc140u.dll OpenMFC reimplements.
// Function BODIES are byte-identical between the two images, so control flow,
// member offsets and constants hold for either; ADDRESSES do not.  Every
// instruction address quoted in this file is an mfc140 address and is tagged
// "(mfc140)"; each export's entry is given for both images, the mfc140u one
// resolved through the export table (ordinal -> EAT) and tagged "(mfc140u)".
//
// Class shape.  Retail CMFCVisualManagerVS2008 derives from
// CMFCVisualManagerVS2005 -> Office2003 -> OfficeXP -> CMFCVisualManager, is
// 0x338 bytes (CreateObject allocates 0x338; the descriptor in
// featurepack/visualmanager/RuntimeClasses.cpp carries 824) and adds exactly
// one member, BOOL m_bOSColors at +0x330 (afxvisualmanagervs2008.h:69; the
// constructor stores it at 0x1b50a6 (mfc140)).  OpenMFC declares no C++ class
// for it: the object is OpenMFC's CMFCVisualManagerVS2005 (328 bytes, see
// include/openmfc/afxmfc.h) placement-constructed at the front of the 0x338
// block, so +0x330 is free storage this file owns.  Everything else the
// retail bodies read out of `this` is the Office2003/OfficeXP colour palette
// at +0x108..+0x2e8, which OpenMFC has no storage for; VS08_Palette() below
// recomputes the members these bodies need, exactly as
// CMFCVisualManagerOffice2003.cpp / CMFCVisualManagerOfficeXP.cpp already do
// for their own classes (their helpers are file-local there, so the few
// formulas needed here are carried again, each re-read from retail).
//
// afxGlobalData.  0x1803ba380 (mfc140) is ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A.
// OpenMFC exports it as a 720-byte blob (featurepack/CMFC_misc_stubs.cpp) that
// AFX_GLOBAL_DATA::Initialize / UpdateSysColors (core/runtime/AFX_GLOBAL_DATA.cpp,
// real transcribed bodies) populate, so this file reads the blob directly
// behind the same init gate every retail body inlines, at the offsets of the
// AfxGlobalData transcription in that file: +0x28 clrBtnFace, +0x3c
// clrBtnDkShadow, +0x48 clrHilite, +0x60 clrBarFace, +0x64 clrBarShadow, +0x74
// clrBarText, +0x78 clrWindow, +0x260 / +0x264 the two high-contrast flags,
// +0x288 m_nBitsPerPixel.  Of these, +0x28/+0x48/+0x60/+0x64/+0x78/+0x288 carry
// their own static_assert there; +0x3c/+0x74/+0x260/+0x264 follow from the
// member order (with the struct's total size asserted), and every one of them
// is the offset the retail bodies below actually read.  (Same approach as
// featurepack/controls/CMFCHeaderCtrl.cpp.)
//
// The rich-colour gate.  Nearly every body opens with
//     if (afxGlobalData.m_nBitsPerPixel (+0x288) > 8 &&
//         afxGlobalData[+0x264] == 0 && afxGlobalData[+0x260] == 0) ...
// written here as VS08_RichColorDesktop().
//
// Virtual calls on `this`.  OpenMFC cannot dispatch through the retail MSVC
// vftable of this object (it has none: the vptr left by the VS2005 constructor
// is OpenMFC's own), so, as in the sibling visual-manager files, each virtual
// call is replaced by the function the retail CMFCVisualManagerVS2008 vftable
// (0x18031c0b8, mfc140, installed by the constructor at 0x1b5093) holds in that
// slot.  Read off that vftable:
//     +0x060 slot  12  CMFCBaseVisualManager::GetStandardWindowsTheme
//     +0x138 slot  39  CMFCVisualManagerOfficeXP::GetHighlightedMenuItemTextColor
//     +0x5c8 slot 185  0xdbf0 (mfc140), `mov $3,%eax; ret` (a folded COMDAT the
//                      RVA map labels with an unrelated name).  Taken to be
//                      OfficeXP's inline GetMenuImageMargin() { return 3; }
//                      (afxvisualmanagerofficexp.h:93): the constant matches and
//                      both call sites below use the result as a margin
//                      (OnHighlightRarelyUsedMenuItems doubles it at 0x1b53e5).
//     +0x630 slot 198  CMFCVisualManagerOffice2003::GetWindowColor
//     +0x638 slot 199  CMFCVisualManagerVS2005::OnFillHighlightedArea
//     +0x640 slot 200  CMFCVisualManagerOffice2003::GetBaseThemeColor
// A client class that overrides one of those virtuals is therefore not
// honoured here.  GetStandardWindowsTheme's OpenMFC body returns 0
// (WinXpTheme_NonStandard) because the UxTheme state is unmodelled
// (CMFCBaseVisualManager.cpp), so the `== 1` (WinXpTheme_Blue) branches below
// are unreachable in this build; they are transcribed anyway, with the one
// caveat stated where it applies.
//
// m_bOSColors (+0x330).  CMFCVisualManagerVS2008::OnUpdateSystemColors sets it
// to (this->[+0x50] != NULL && rich-colour desktop), +0x50 being a UxTheme
// HTHEME of CMFCBaseVisualManager.  No HTHEME is ever opened in OpenMFC, so the
// value is 0 unless a derived client class writes the (protected) member
// itself; the bodies below read the member rather than assume it.
//=============================================================================

// ---- afxGlobalData (featurepack/CMFC_misc_stubs.cpp, core/runtime/AFX_GLOBAL_DATA.cpp) ----
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);

// ---- RTTI (core/runtime/CObject.cpp and the subsystems' RuntimeClasses.cpp) ----
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ();

// ---- base-class constructor / destructor (featurepack/visualmanager/Thunks.cpp) ----
extern "C" void* MS_ABI impl___0CMFCVisualManagerVS2005__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCVisualManagerVS2005__UEAA_XZ(void* pThis);

// ---- ancestor bodies this class falls back to.  Declared from the mangled
// names; they match the definitions in CMFCVisualManagerOffice2003.cpp /
// CMFCVisualManagerOfficeXP.cpp / CMFCBaseVisualManager.cpp /
// CMFCVisualManagerOffice2007.cpp (checked 2026-09-22).  Signatures only:
// Office2003's OnFillButtonInterior / OnFillCommandsListBackground are still
// empty bodies there and its OnFillBarBackground delegates to
// CMFCVisualManager, so those fallbacks paint what those bodies paint. ----
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state);
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    void* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea);
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__H_Z(
    void* pThis, CDC* pDC, CRect rect, int bIsSelected);
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* pDC, CRect rect);
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, unsigned long* pclrText);
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
    void* pThis, CMFCToolBarButton* pButton);
extern "C" unsigned long MS_ABI impl__GetWindowColor_CMFCVisualManagerOffice2003__MEBAKXZ(const void* pThis);
extern "C" unsigned long MS_ABI impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(void* pThis);
extern "C" int MS_ABI impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(void* pThis);
extern "C" int MS_ABI impl__GetSmartDockingTheme_CMFCVisualManagerOffice2007__UEAA_AW4AFX_SMARTDOCK_THEME__XZ(void* pThis);
extern "C" std::int32_t impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA;

// ---- CDrawingManager (core/gdi/CDrawingManager.cpp, real bodies) ----
extern "C" void* MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC);
extern "C" void MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(
    void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish,
    int bHorz, int nStartFlatPercentage, int nEndFlatPercentage);
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKH_Z(unsigned long srcPixel, int nPercent);
extern "C" void MS_ABI impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(
    unsigned long rgb, double* pH, double* pS, double* pL);
extern "C" unsigned long MS_ABI impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(
    unsigned long color1, unsigned long color2, double dblLumRatio, int k1, int k2);

// ---- CDC (core/gdi/CDC.cpp, real bodies) ----
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y);
extern "C" int MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y);

// ---- MFC's exported operator new, ??2@YAPEAX_K@Z (detail/MemcoreSupport.cpp) ----
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

// ---- toolbar / menu helpers ----
extern "C" CSize* MS_ABI impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(CSize* pRet);
extern "C" int MS_ABI impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(CMFCPopupMenuBar* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);

// This file's own OnUpdateSystemColors (defined at the bottom; the constructor calls it).
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerVS2008__UEAAXXZ(void* pThis);

namespace {

//--------------------------------------------------------------------------
// afxGlobalData access.
//--------------------------------------------------------------------------

// The init gate every retail reader inlines (e.g. 0x1b534c..0x1b5372 (mfc140)):
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

inline COLORREF GD_clrBtnFace()     { return GD_U32(0x28); }
inline COLORREF GD_clrBtnDkShadow() { return GD_U32(0x3c); }
inline COLORREF GD_clrHilite()      { return GD_U32(0x48); }
inline COLORREF GD_clrBarFace()     { return GD_U32(0x60); }
inline COLORREF GD_clrBarShadow()   { return GD_U32(0x64); }
inline COLORREF GD_clrBarText()     { return GD_U32(0x74); }
inline COLORREF GD_clrWindow()      { return GD_U32(0x78); }
inline int      GD_Flag260()        { return static_cast<int>(GD_U32(0x260)); }
inline int      GD_Flag264()        { return static_cast<int>(GD_U32(0x264)); }
inline int      GD_BitsPerPixel()   { return static_cast<int>(GD_U32(0x288)); }

// bpp > 8 && [+0x264] == 0 && [+0x260] == 0, tested in that order.
inline bool VS08_RichColorDesktop()
{
    return GD_BitsPerPixel() > 8 && GD_Flag264() == 0 && GD_Flag260() == 0;
}

//--------------------------------------------------------------------------
// Object members.
//--------------------------------------------------------------------------

constexpr std::ptrdiff_t kOffOSColors = 0x330;   // BOOL m_bOSColors (see the file header)

inline int VS08_OSColors(const void* pThis)
{
    int v = 0;
    std::memcpy(&v, static_cast<const char*>(pThis) + kOffOSColors, sizeof v);
    return v;
}

inline int VS08_IsKindOf(const void* pObj, const CRuntimeClass* pClass)
{
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pObj), pClass);
}

// The guard OnDrawButtonBorder (0x1b5475..0x1b54a7) and OnFillButtonInterior
// (0x1b5525..0x1b5563) share (mfc140):
//     pButton != NULL && pButton->IsKindOf(CMFCToolBarMenuButton)   // descriptor 0x1803aa748
//     && pButton->m_pWndParent (+0x80) != NULL
//     && m_pWndParent->IsKindOf(CMFCMenuBar)                         // descriptor 0x1803aa268
// (descriptor names read with rtc.py; m_pWndParent is pinned at 0x80 by the
// static_asserts in featurepack/menu/CMFCPopupMenu.cpp).
inline bool VS08_IsMenuBarMenuButton(CMFCToolBarButton* pButton)
{
    if (pButton == nullptr) return false;
    if (!VS08_IsKindOf(pButton, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ())) return false;
    CWnd* pParent = pButton->m_pWndParent;
    if (pParent == nullptr) return false;
    return VS08_IsKindOf(pParent, impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ()) != 0;
}

// pButton->IsDroppedDown(): vftable +0xf0 (slot 30) of CMFCToolBarMenuButton.
// The retail CMFCToolBarMenuButton vftable (0x180316338, mfc140, installed by
// ??0CMFCToolBarMenuButton@@QEAA@XZ at 0x170e0e) holds 0x2a580 (mfc140) there,
// whose whole body is `return this->[+0xc0] (m_pPopupMenu) != NULL`.  Read
// by offset, as CMFCVisualManagerOffice2003.cpp does for the same slot; only
// called after the IsKindOf(CMFCToolBarMenuButton) test above.
inline bool VS08_IsDroppedDown(const CMFCToolBarButton* pButton)
{
    const void* pPopup = nullptr;
    std::memcpy(&pPopup, reinterpret_cast<const char*>(pButton) + 0xc0, sizeof pPopup);
    return pPopup != nullptr;
}

// The vptr OpenMFC's CMFCVisualManagerVS2005 constructor installs, captured by
// this class's constructor (see the destructor below for why).  Every
// OpenMFC-built VS2005 subobject gets the same value.
const void* g_pVS2005Vptr = nullptr;

// vftable +0x5c8 (slot 185): see the file header.
constexpr int kMenuImageMargin = 3;

// WinXpTheme_Blue (afxvisualmanager.h:120: None = -1, NonStandard, Blue, ...),
// the value the `cmp $1` after each vftable +0x60 call tests.
constexpr int kWinXpThemeBlue = 1;

inline int VS08_StandardWindowsTheme(void* pThis)
{
    return impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(pThis);
}

//--------------------------------------------------------------------------
// The palette members these bodies read, recomputed.
//
// In retail the chain VS2008 -> VS2005 -> Office2003 -> OfficeXP
// ::OnUpdateSystemColors stores them; for a VS2008 object with
// GetStandardWindowsTheme() == 0 (always, in this build) the stores are:
//
//  CMFCVisualManagerOfficeXP::OnUpdateSystemColors (entry 0x1ac0e0, mfc140)
//   rich-colour path (0x1ac1c4..0x1ac682):
//     window = this->GetWindowColor()          vftable +0x630, call at 0x1ac1d1
//     +0x110 = blend(window 0xdb, clrBarFace 0x24) / 255      store 0x1ac289
//     +0x108 = blend(clrBarFace 0xd7, window 0x28) / 255      store 0x1ac3ac
//     mix    = blend(+0x110 0xb2, clrHilite 0x4d) / 255       0x1ac42e..0x1ac499
//     L      = RGBtoHSL(clrHilite).L (0x1ac414), compared with 0.8 (0x1ac49b;
//              the constant at 0x180348b58 reads 0.8)
//     L > 0.8: +0x118 = PixelAlpha(mix, 0x5b), +0x11c = PixelAlpha(mix, 0x62),
//              +0x138 = PixelAlpha(clrHilite, 0x54)           0x1ac4a7..0x1ac4ef
//     else:    +0x118 = PixelAlpha(mix, 0x66), +0x11c = PixelAlpha(+0x118, 0x57),
//              +0x138 = clrHilite                             0x1ac4f6..0x1ac533
//   low-colour / high-contrast path (0x1ac687..0x1ac8e7):
//     +0x110 = clrWindow (+0x78); +0x108 = clrBtnFace (+0x28);
//     [+0x260] set: +0x118 = +0x11c = clrHilite (+0x48)
//     else:         +0x118 = +0x11c = clrBtnFace (+0x28)
//     +0x138 = ([+0x264] == 0 && [+0x260] == 0) ? clrHilite : clrBtnDkShadow (+0x3c)
//   then brush +0x160 = CreateSolidBrush(+0x108) and brush +0x180 =
//   CreateSolidBrush(+0x110) (0x1ac8ed..0x1ac939).  No later store in the
//   rich path touches +0x138 (0x1ac53f..0x1ac682 writes +0x120..+0x134 only).
//
//  CMFCVisualManagerOffice2003::OnUpdateSystemColors (entry 0x190b30, mfc140),
//   rich path with +0x2e8 == 0 (+0x2e8 is `(unsigned)(style - 1) <= 2`,
//   0x190bcc..0x190be0, so it is 0 for style 0 / -1):
//     base = this->GetBaseThemeColor()          vftable +0x640, call at 0x190c6d
//     `default` arm of the style switch (0x190ec4..0x190f80):
//     +0x21c = SmartMixColors(base, sys(COLOR_BTNFACE), 0.93, 2, 1)
//     +0x220 = SmartMixColors(base, sys(COLOR_WINDOW),  1.0,  1, 4)
//       where sys(i) is GetThemeSysColor on the HTHEME at +0x08 or, when that
//       is NULL (always here), ::GetSysColor(i)
//     +0x248 = +0x118, +0x24c = +0x11c                         0x191364..0x19137b
//     pen +0x1e0 = CreatePen(PS_SOLID, 1, clrBarShadow (+0x64)) (0x191480 loads
//       it into edi, 0x1915d8 passes edi)
//   Neither +0x180 nor +0x21c/+0x220/+0x248/+0x24c is written again on this
//   path, and the +0x2e8 != 0 arm (0x190fbb..0x19135f), which would, is not
//   reachable for style 0.
//
//  CMFCVisualManagerVS2005::OnUpdateSystemColors (entry 0x1b33c0, mfc140):
//   latches the static m_bDefaultWinXPColors into r12 (0x1b33d6), stores
//   +0x124 = -1 and +0x328 = GetStandardWindowsTheme(), and -- unless that
//   theme is 3 -- CLEARS the static (0x1b33fc) before calling Office2003's
//   body (0x1b3409).  Office2003 therefore sees the static as 0 and forces
//   its own style +0x210 to 0 unless it is -1 (0x190bb4..0x190bc9), so for a
//   VS2005/VS2008
//   object +0x2e8 is 0 and the default arm above runs whatever the theme
//   (other than 3) is.  VS2005 then returns straight away when the latched
//   value was 0 (0x1b340e).  Otherwise it also rewrites +0x114, +0x124,
//   +0x224, +0x234, +0x238, +0x2f0 and the GDI objects at +0x2f8/+0x308/
//   +0x318 -- none of which this file reads -- and on a rich-colour desktop
//   re-creates pen +0x1e0 with PixelAlpha(clrBarFace, 0x54) (0x1b38c4..
//   0x1b3914).  Its +0x118/+0x11c/+0x21c/+0x220 stores are all inside arms
//   keyed on +0x328 being 1 or 2.
//
//  CMFCVisualManagerVS2008::OnUpdateSystemColors (entry 0x1b6700, mfc140)
//   writes the palette only when m_bOSColors is set, and then only
//   +0x214..+0x240 and three GDI objects this file does not read.
//
// APPROXIMATION (pen +0x1e0): because VS2005 clears m_bDefaultWinXPColors on
// the very pass that consults it, retail's pen is PixelAlpha(clrBarFace, 0x54)
// only after the first colour update of the process (normally this class's
// constructor, see below) and clrBarShadow after any later one (e.g. a
// WM_SYSCOLORCHANGE).  OpenMFC's VS2005::OnUpdateSystemColors is an empty
// stub that never clears the static, and this file keeps no update history,
// so VS08_Palette() simply reads the static's current value -- 0 as
// featurepack/visualmanager/StaticData.cpp initialises it, i.e. retail's
// post-first-update clrBarShadow.
//
// The `== WinXpTheme_Blue` branches below read only +0x110 (through brush
// +0x180) and pen +0x1e0 from this palette.  Per the VS2005 note above,
// retail derives those the same way for theme 1 (VS2005's extra `+0x328 == 1`
// stores land in +0x214..+0x238, and no VS2005 store touches +0x110 or brush
// +0x180 on any path), so the palette applies to
// those branches too -- which are unreachable anyway while OpenMFC's
// GetStandardWindowsTheme returns 0.
//--------------------------------------------------------------------------

// Per channel: (wa*a + wb*b) / 255 -- the `mul $0x80808081; shr $7` idiom.
inline COLORREF VS08_Blend255(COLORREF a, unsigned wa, COLORREF b, unsigned wb)
{
    const unsigned r  = (wa * GetRValue(a) + wb * GetRValue(b)) / 255u;
    const unsigned g  = (wa * GetGValue(a) + wb * GetGValue(b)) / 255u;
    const unsigned bl = (wa * GetBValue(a) + wb * GetBValue(b)) / 255u;
    return RGB(r & 0xffu, g & 0xffu, bl & 0xffu);
}

inline COLORREF VS08_PixelAlpha(COLORREF c, int nPercent)
{
    return impl__PixelAlpha_CDrawingManager__SAKKH_Z(c, nPercent);
}

struct VS08Palette {
    COLORREF c108;    // OfficeXP m_clrBarBkgnd       (brush +0x160)
    COLORREF c110;    // OfficeXP menu background     (brush +0x180)
    COLORREF c118;    // OfficeXP highlight
    COLORREF c11c;    // OfficeXP pressed highlight
    COLORREF c138;    // OfficeXP m_clrMenuItemBorder
    // Rich-colour path only (zero otherwise; no body below reads them there):
    COLORREF c21c;
    COLORREF c220;
    COLORREF c248;
    COLORREF c24c;
    COLORREF pen1e0;  // colour of the CPen at +0x1e0
};

VS08Palette VS08_Palette(void* pThis)
{
    VS08Palette p;
    std::memset(&p, 0, sizeof p);

    if (VS08_RichColorDesktop()) {
        const COLORREF window     = impl__GetWindowColor_CMFCVisualManagerOffice2003__MEBAKXZ(pThis);
        const COLORREF clrBarFace = GD_clrBarFace();
        const COLORREF clrHilite  = GD_clrHilite();
        p.c110 = VS08_Blend255(window, 0xdb, clrBarFace, 0x24);
        p.c108 = VS08_Blend255(clrBarFace, 0xd7, window, 0x28);
        const COLORREF mix = VS08_Blend255(p.c110, 0xb2, clrHilite, 0x4d);
        double h = 0.0, s = 0.0, l = 0.0;
        impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(clrHilite, &h, &s, &l);
        if (l > 0.8) {
            p.c118 = VS08_PixelAlpha(mix, 0x5b);
            p.c11c = VS08_PixelAlpha(mix, 0x62);
            p.c138 = VS08_PixelAlpha(clrHilite, 0x54);
        } else {
            p.c118 = VS08_PixelAlpha(mix, 0x66);
            p.c11c = VS08_PixelAlpha(p.c118, 0x57);
            p.c138 = clrHilite;
        }

        const COLORREF base = impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(pThis);
        p.c21c = impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(base, ::GetSysColor(COLOR_BTNFACE), 0.93, 2, 1);
        p.c220 = impl__SmartMixColors_CDrawingManager__SAKKKNHH_Z(base, ::GetSysColor(COLOR_WINDOW), 1.0, 1, 4);
        p.c248 = p.c118;
        p.c24c = p.c11c;
        // See APPROXIMATION (pen +0x1e0) above: the current value of the static
        // stands in for its value on entry to the last VS2005 colour update.
        p.pen1e0 = (impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA != 0)
                       ? VS08_PixelAlpha(clrBarFace, 0x54)
                       : GD_clrBarShadow();
    } else {
        p.c110 = GD_clrWindow();
        p.c108 = GD_clrBtnFace();
        p.c118 = p.c11c = (GD_Flag260() != 0) ? GD_clrHilite() : GD_clrBtnFace();
        p.c138 = (GD_Flag264() == 0 && GD_Flag260() == 0) ? GD_clrHilite() : GD_clrBtnDkShadow();
    }
    return p;
}

//--------------------------------------------------------------------------
// GDI helpers.  Retail builds CBrush / CPen / CRgn objects and selects them
// through CDC::SelectObject / SelectClipRgn; those C++ methods exist only as
// impl__ thunks inside this DLL, so the raw handles are used and released
// here (the same substitution CMFCVisualManagerOffice2003.cpp makes).
//--------------------------------------------------------------------------

inline RECT VS08_ToRECT(const CRect& r) { return RECT{ r.left, r.top, r.right, r.bottom }; }

// `CBrush br(clr); ::FillRect(pDC->m_hDC, &rect, br);` -- also used where retail
// fills with one of the object's own solid member brushes (+0x180), by that
// brush's colour.
void VS08_FillSolid(CDC* pDC, const CRect& rect, COLORREF clr)
{
    if (pDC == nullptr) return;
    HBRUSH hbr = ::CreateSolidBrush(clr);
    if (hbr == nullptr) return;
    RECT r = VS08_ToRECT(rect);
    ::FillRect(pDC->m_hDC, &r, hbr);
    ::DeleteObject(hbr);
}

// `CDrawingManager dm(*pDC); dm.FillGradient(rect, colorStart, colorFinish, bHorz, 0, 0);`
void VS08_FillGradient(CDC* pDC, CRect rect, COLORREF colorStart, COLORREF colorFinish, int bHorz)
{
    struct { const void* vptr; CDC* pDC; } dm = { nullptr, nullptr };
    impl___0CDrawingManager__QEAA_AEAVCDC___Z(&dm, pDC);
    impl__FillGradient_CDrawingManager__QEAAXVCRect__KKHHH_Z(&dm, &rect, colorStart, colorFinish, bHorz, 0, 0);
    impl___1CDrawingManager__UEAA_XZ(&dm);
}

inline void VS08_Draw3dRect(CDC* pDC, const CRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
    RECT r = VS08_ToRECT(rect);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &r, clrTopLeft, clrBottomRight);
}

// `CPen* pOld = pDC->SelectObject(&pen); pDC->MoveTo(x, y1); pDC->LineTo(x, y2);
//  pDC->SelectObject(pOld);` with a solid 1-pixel pen of colour clr.
void VS08_VertLine(CDC* pDC, int x, int y1, int y2, COLORREF clr)
{
    if (pDC == nullptr) return;
    HPEN hpen = ::CreatePen(PS_SOLID, 1, clr);
    HGDIOBJ hOld = (hpen != nullptr) ? ::SelectObject(pDC->m_hDC, hpen) : nullptr;
    impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x, y1);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, x, y2);
    if (hOld != nullptr) ::SelectObject(pDC->m_hDC, hOld);
    if (hpen != nullptr) ::DeleteObject(hpen);
}

inline int VS08_MenuImageWidth()
{
    CSize size(0, 0);
    impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(&size);
    return size.cx;
}

} // namespace

// ??0CMFCVisualManagerVS2008@@QEAA@XZ -- entry 0x1b5080 (mfc140), 0x1b6a80 (mfc140u):
//     CMFCVisualManagerVS2005::CMFCVisualManagerVS2005();   // call 0x1b3260 at 0x1b508d
//     vptr = &CMFCVisualManagerVS2008::`vftable';            // 0x18031c0b8 (mfc140), 0x1b5093
//     *(void**)(this + 0x200) = NULL;                        // qword store, 0x1b509f
//     m_bOSColors (+0x330) = FALSE;                          // 0x1b50a6
//     CMFCVisualManagerVS2008::OnUpdateSystemColors();       // direct call 0x1b6700, 0x1b50af
//     return this;
// DEVIATION: OpenMFC has no VS2008 vftable, so the vptr left by OpenMFC's
// CMFCVisualManagerVS2005 constructor stays in place -- an object built here
// dispatches (and reports GetRuntimeClass) as CMFCVisualManagerVS2005 inside
// this DLL.  +0x200 and +0x330 lie beyond OpenMFC's 328-byte VS2005 object and
// inside the 0x338 bytes a VS2008 object occupies, so the two stores are made
// exactly as retail makes them.
// (Not on this file's stub list -- it was a bare `return pThis;` that left the
// object unconstructed -- but CreateObject below depends on it.)
// Symbol: ??0CMFCVisualManagerVS2008@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCVisualManagerVS2008__QEAA_XZ(void* pThis) {
    impl___0CMFCVisualManagerVS2005__QEAA_XZ(pThis);
    std::memcpy(&g_pVS2005Vptr, pThis, sizeof g_pVS2005Vptr);   // for the destructor
    char* p = static_cast<char*>(pThis);
    void* const pNull = nullptr;
    std::memcpy(p + 0x200, &pNull, sizeof pNull);
    const int bOSColors = FALSE;
    std::memcpy(p + kOffOSColors, &bOSColors, sizeof bOSColors);
    impl__OnUpdateSystemColors_CMFCVisualManagerVS2008__UEAAXXZ(pThis);
    return pThis;
}

// ??1CMFCVisualManagerVS2008@@UEAA@XZ -- entry 0x1b5110 (mfc140), 0x1b6b10 (mfc140u),
// complete body:
//     vptr = &CMFCVisualManagerVS2008::`vftable';   // 0x1b5110..0x1b5117
//     jmp CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005   // 0x1b3350, tail call
// DEVIATION, forced by the base thunk: OpenMFC's ??1CMFCVisualManagerVS2005
// thunk (featurepack/visualmanager/Thunks.cpp) is `p->~CMFCVisualManagerVS2005()`,
// which g++ compiles as a VIRTUAL call through slot 1 of whatever vptr the
// object holds (`mov (%rcx),%rax; jmp *0x8(%rax)` in
// build-phase4/obj/featurepack/visualmanager/Thunks.o).  This export's callers
// are the destructors of MSVC-compiled classes derived from
// CMFCVisualManagerVS2008, which leave their own MSVC vptr in the object, so
// slot 1 there would be their scalar deleting destructor.  Standing in for
// retail's "install this class's vftable" step, the vptr the VS2005
// constructor installed (captured by the constructor above) is put back
// first; if no object was ever built through that constructor there is no
// such vptr and the base destructor is not run.
// Symbol: ??1CMFCVisualManagerVS2008@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCVisualManagerVS2008__UEAA_XZ(void* pThis) {
    if (pThis == nullptr || g_pVS2005Vptr == nullptr) return;
    std::memcpy(pThis, &g_pVS2005Vptr, sizeof g_pVS2005Vptr);
    impl___1CMFCVisualManagerVS2005__UEAA_XZ(pThis);
}

// ?CreateObject@CMFCVisualManagerVS2008@@SAPEAVCObject@@XZ -- entry 0x1b5040
// (mfc140), 0x1b6a40 (mfc140u), complete body:
//     void* p = operator new(0x338);                 // ??2@YAPEAX_K@Z, 0x1b5049
//     return p ? new(p) CMFCVisualManagerVS2008 : NULL;   // ctor 0x1b5080, 0x1b505b
// The allocation goes through the same exported MFC operator new.
// Symbol: ?CreateObject@CMFCVisualManagerVS2008@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerVS2008__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(0x338);
    if (p == nullptr) return nullptr;
    return static_cast<CObject*>(impl___0CMFCVisualManagerVS2008__QEAA_XZ(p));
}

// ?GetSmartDockingTheme@CMFCVisualManagerVS2008@@UEAA?AW4AFX_SMARTDOCK_THEME@@XZ
// -- entry 0x1ab6d0 (mfc140).  In mfc140u this export and
// ?GetSmartDockingTheme@CMFCVisualManagerOffice2007@@... resolve through the
// export table to the SAME RVA, 0x1ad0d0 (urva.py, both names): the linker
// folded the two identical bodies.  Complete body (0x1ab6d0..0x1ab738, mfc140):
//     <afxGlobalData init gate>
//     if (bpp (+0x288) > 8 && [+0x264] == 0 && [+0x260] == 0) return 2;  // AFX_SDT_VS2008
//     return 1;                                                           // AFX_SDT_VS2005
// Forwarded to the Office2007 thunk so the two exports keep sharing one
// implementation as they do in retail.  That thunk returns the same 2 / 1 on
// the same three conditions, but it recomputes them from Win32 on each call
// (GetDC(NULL)/GetDeviceCaps and four GetSysColor tests, see
// CMFCVisualManagerOffice2007.cpp) instead of reading the cached afxGlobalData
// fields as retail does, so after a colour/depth change it can answer before
// afxGlobalData has been refreshed.  The
// generated stub's 17-parameter list and pointer return were demangler noise;
// the export takes only `this` and returns a 4-byte enum.
// Symbol: ?GetSmartDockingTheme@CMFCVisualManagerVS2008@@UEAA?AW4AFX_SMARTDOCK_THEME@@XZ
extern "C" int MS_ABI impl__GetSmartDockingTheme_CMFCVisualManagerVS2008__UEAA_AW4AFX_SMARTDOCK_THEME__XZ(void* pThis) {
    return impl__GetSmartDockingTheme_CMFCVisualManagerOffice2007__UEAA_AW4AFX_SMARTDOCK_THEME__XZ(pThis);
}

// ?GetTabFrameColors@CMFCVisualManagerVS2008@@... -- entry 0x1b6c20 (mfc140),
// 0x1b8620 (mfc140u):
//     CMFCVisualManagerVS2005::GetTabFrameColors(pTabWnd, clrDark, clrBlack,
//         clrHighlight, clrFace, clrDarkShadow, clrLight, pbrFace, pbrBlack);   // 0x1b4ad0, 0x1b6c7a
//     if (bpp <= 8 || [+0x264] || [+0x260]) return;
//     if (pTabWnd->vftable[+0x510]()) return;                                   // call at 0x1b6ce2
//     clrBlack = afxGlobalData.clrBarShadow (+0x64);                            // store at 0x1b6d10
// STUB: the VS2005 body it starts from is defined in CMFCVisualManagerVS2005.cpp
// with an auto-generated parameter list (no `this`) -- see headerRequests --
// and the CMFCBaseTabCtrl virtual at +0x510 (IsFlatTab in the slot map
// CMFCVisualManagerOfficeXP.cpp derives; in the retail CMFCTabCtrl vftable
// 0x180310520 (mfc140) that slot returns this->[+0x3c0]) cannot be dispatched
// on OpenMFC's tab controls (the same limitation
// CMFCVisualManagerOfficeXP.cpp records for OnDrawTab).  Parameter list
// corrected from the mangled name.
// Symbol: ?GetTabFrameColors@CMFCVisualManagerVS2008@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerVS2008__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    void* /*pThis*/, const CMFCBaseTabCtrl* /*pTabWnd*/, unsigned long* /*clrDark*/,
    unsigned long* /*clrBlack*/, unsigned long* /*clrHighlight*/, unsigned long* /*clrFace*/,
    unsigned long* /*clrDarkShadow*/, unsigned long* /*clrLight*/, CBrush** /*pbrFace*/,
    CBrush** /*pbrBlack*/) {}

// ?OnDrawAutoHideButtonBorder@CMFCVisualManagerVS2008@@... -- entry 0x1b6270
// (mfc140), 0x1b7c70 (mfc140u):
//     if (rich-colour desktop && this->[+0x80] /* HTHEME */ != NULL) {
//         ... ::DrawThemeBackground on that theme, chosen by the
//             0x1000/0x2000/0x4000/0x8000 bits of pButton->[+0x14] ...
//         return;
//     }
//     CMFCVisualManagerVS2005::OnDrawAutoHideButtonBorder(pDC, rectBounds,
//         rectBorderSize, pButton);                                   // 0x1b48f0, 0x1b653a
// No HTHEME is ever opened in OpenMFC, so the reachable behaviour is the
// VS2005 fallback alone.
// STUB: that VS2005 body is defined in CMFCVisualManagerVS2005.cpp with an
// auto-generated parameter list (no `this`); see headerRequests.  Parameter
// list corrected from the mangled name.
// Symbol: ?OnDrawAutoHideButtonBorder@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@VCRect@@1PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnDrawAutoHideButtonBorder_CMFCVisualManagerVS2008__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rectBounds*/, CRect /*rectBorderSize*/,
    void* /*pButton*/ /* CMFCAutoHideButton* */) {}

// ?OnDrawButtonBorder@CMFCVisualManagerVS2008@@... -- entry 0x1b5460 (mfc140),
// 0x1b6e60 (mfc140u), complete body:
//     if (<menu-bar menu button, see VS08_IsMenuBarMenuButton>) {  // 0x1b5475..0x1b54a7
//         rect.bottom -= 2;                                       // 0x1b54af
//         if (pButton->IsDroppedDown()) {                         // vftable +0xf0, call at 0x1b54ba
//             pDC->Draw3dRect(&rect, m_clrMenuItemBorder (+0x138),
//                                    m_clrMenuItemBorder);       // 0x2a3b00, 0x1b54d4
//             return;
//         }
//     }
//     CMFCVisualManagerOffice2003::OnDrawButtonBorder(pDC, pButton, rect, state);  // 0x1906a0, 0x1b54fd
// The fallback receives the already-shortened rect whenever the guard passed
// (it copies the same local at 0x1b54db).  +0x138 comes from VS08_Palette().
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerVS2008__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (VS08_IsMenuBarMenuButton(pButton)) {
        rect.bottom -= 2;
        if (VS08_IsDroppedDown(pButton)) {
            const COLORREF clr = VS08_Palette(pThis).c138;
            VS08_Draw3dRect(pDC, rect, clr, clr);
            return;
        }
    }
    impl__OnDrawButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        pThis, pDC, pButton, rect, state);
}

// ?OnDrawPaneCaption@CMFCVisualManagerVS2008@@... -- entry 0x1b6570 (mfc140),
// 0x1b7f70 (mfc140u), complete body:
//     if (m_bOSColors (+0x330) == 0)                                   // 0x1b657f
//         return CMFCVisualManagerVS2005::OnDrawPaneCaption(pDC, pBar,
//                    bActive, rectCaption, rectButtons);             // 0x1b3930, 0x1b65ba
//     rectCaption.bottom++;                                            // 0x1b65d2
//     CDrawingManager dm(*pDC);
//     dm.FillGradient(rectCaption,
//         bActive ? afxGlobalData[+0x94] : [+0x98],    // colorStart (r8)
//         bActive ? afxGlobalData[+0x88] : [+0x8c],    // colorFinish (r9)
//         TRUE, 0, 0);                                                 // 0x57960, 0x1b6698
//     return bActive ? afxGlobalData[+0x80] : [+0x90];                 // 0x1b66bd / 0x1b66e1
// (+0x80/+0x88/+0x8c/+0x90/+0x94/+0x98 are clrCaptionText, clrActiveCaption,
// clrInactiveCaption, clrInactiveCaptionText, clrActiveCaptionGradient,
// clrInactiveCaptionGradient in core/runtime/AFX_GLOBAL_DATA.cpp's shadow.)
// STUB: m_bOSColors is 0 unless a derived class sets it (see the file
// header), so the ordinary path is the VS2005 fallback alone, and that body is
// defined in
// CMFCVisualManagerVS2005.cpp with an auto-generated parameter list (no
// `this`); see headerRequests.  Parameter list corrected from the mangled name.
// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerVS2008@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerVS2008__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    void* /*pThis*/, CDC* /*pDC*/, void* /*pBar*/ /* CDockablePane* */, int /*bActive*/,
    CRect /*rectCaption*/, CRect /*rectButtons*/) {
    return 0;
}

// ?OnDrawTab@CMFCVisualManagerVS2008@@... -- entry 0x1b5e10 (mfc140), 0x1b7810 (mfc140u).
// Opens with pTabWnd->GetTabBkColor(iTab) (vftable +0x3c8, call at 0x1b5e57)
// and a read of pTabWnd->[+0x1dc].  Unless ALL of (rich-colour desktop,
// pTabWnd->[+0x1fc] == 0, the tab control's vftable +0x510 query FALSE, that
// colour == -1) hold, it calls CMFCVisualManagerVS2005::OnDrawTab (0x1b4030,
// the call at 0x1b624a) and returns; otherwise it goes on to query the tab
// control's vftable +0x520 / +0x528 / +0x530 and this->[+0x80] (an HTHEME) and
// paints or delegates along several paths (0x1b5f08..0x1b6227).
// STUB: the CMFCBaseTabCtrl virtuals at +0x510 / +0x528 (IsFlatTab /
// IsVS2005Style in the slot map CMFCVisualManagerOfficeXP.cpp derives) cannot
// be dispatched on OpenMFC's tab controls, and the VS2005 fallback is defined
// with an auto-generated parameter list; see headerRequests.
// Symbol: ?OnDrawTab@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerVS2008__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rectTab*/, int /*iTab*/, int /*bIsActive*/,
    const CMFCBaseTabCtrl* /*pTabWnd*/) {}

// ?OnDrawTabResizeBar@CMFCVisualManagerVS2008@@... -- entry 0x1b6d20 (mfc140),
// 0x1b8720 (mfc140u).  On a rich-colour desktop whose tab control answers
// FALSE to its vftable +0x510 query (call at 0x1b6dcf) it paints the bar
// itself -- clips to rect, widens it against a frame window's client area and
// fills it with FillGradient(rect, this->[+0x214], this->[+0x218], FALSE, 0, 0)
// (0x1b6ddd..0x1b6ef3); otherwise it calls CMFCVisualManager::
// OnDrawTabResizeBar (0x185890, the call at 0x1b6f21).
// STUB: the +0x510 virtual (IsFlatTab, see OnDrawTab) cannot be dispatched on
// OpenMFC's tab controls, so the branch cannot be chosen.
// Symbol: ?OnDrawTabResizeBar@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@PEAVCMFCBaseTabCtrl@@HVCRect@@PEAVCBrush@@PEAVCPen@@@Z
extern "C" void MS_ABI impl__OnDrawTabResizeBar_CMFCVisualManagerVS2008__UEAAXPEAVCDC__PEAVCMFCBaseTabCtrl__HVCRect__PEAVCBrush__PEAVCPen___Z(
    void* /*pThis*/, CDC* /*pDC*/, CMFCBaseTabCtrl* /*pWndTab*/, int /*bIsVert*/, CRect /*rect*/,
    CBrush* /*pbrFace*/, CPen* /*pPen*/) {}

// ?OnEraseTabsArea@CMFCVisualManagerVS2008@@... -- entry 0x1b58e0 (mfc140),
// 0x1b72e0 (mfc140u).  On a rich-colour desktop, when pTabWnd's vftable +0x528
// query (call at 0x1b5977) is TRUE and pTabWnd->[+0x1fc] == 0, it widens rect
// against a frame window's client area (the frame is read from 0x1803b6f18
// (mfc140), else pTabWnd->GetTopLevelFrame()) and fills it with
// FillGradient(rect, this->[+0x214], this->[+0x218], FALSE, 0, 0); otherwise
// CMFCVisualManagerVS2005::OnEraseTabsArea (0x1b3dd0, the call at 0x1b5a81).
// STUB: the +0x528 virtual (IsVS2005Style, see OnDrawTab) cannot be dispatched
// on OpenMFC's tab controls, and the VS2005 fallback is defined with an
// auto-generated parameter list; see headerRequests.
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerVS2008__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/) {}

// ?OnEraseTabsButton@CMFCVisualManagerVS2008@@... -- entry 0x1b5aa0 (mfc140),
// 0x1b74a0 (mfc140u).  Same shape as OnEraseTabsArea: on a rich-colour desktop
// it asks pWndTab's vftable +0x528 (call at 0x1b5b42) and tests pWndTab->[+0x1fc]
// and the button's +0x114 / +0x11c words before painting a clipped gradient
// from this->[+0x214] / [+0x218] (0x1b5b80..0x1b5ca5); otherwise it calls
// CMFCVisualManagerOffice2003::OnEraseTabsButton (0x193260, the call at 0x1b5cc6).
// STUB: the +0x528 virtual (IsVS2005Style, see OnDrawTab) cannot be dispatched
// on OpenMFC's tab controls, so the branch cannot be chosen.
// Symbol: ?OnEraseTabsButton@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerVS2008__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, CMFCButton* /*pButton*/, CMFCBaseTabCtrl* /*pWndTab*/) {}

// ?OnEraseTabsFrame@CMFCVisualManagerVS2008@@... -- entry 0x1b5cf0 (mfc140),
// 0x1b76f0 (mfc140u).  The first thing it does is call pTabWnd's vftable
// +0x528 (call at 0x1b5d14); when that is TRUE on a rich-colour desktop it fills rect
// with pTabWnd->GetTabBkColor(<vftable +0x428 result>) (this->[+0x118] when
// that is -1) and returns TRUE; otherwise it returns
// CMFCVisualManagerOffice2003::OnEraseTabsFrame (0x1930a0, the call at 0x1b5dff).
// STUB: the +0x528 virtual (IsVS2005Style, see OnDrawTab) cannot be dispatched
// on OpenMFC's tab controls.  FALSE tells the caller nothing was painted.
// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerVS2008@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerVS2008__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    void* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/) {
    return 0;
}

// ?OnFillBarBackground@CMFCVisualManagerVS2008@@... -- entry 0x1b5120 (mfc140),
// 0x1b6b20 (mfc140u), complete body:
//     if (rich-colour desktop                                      // 0x1b514e..0x1b51aa
//         && pBar->IsKindOf(CMFCPopupMenuBar)                      // descriptor 0x1803aa478, 0x1b51ba
//         && !pBar->IsKindOf(CMFCColorBar)                         // descriptor 0x1803aa0e8, 0x1b51d1
//         && this->GetStandardWindowsTheme() == WinXpTheme_Blue) { // vftable +0x60, 0x1b51e8
//         if (::IsRectEmpty(&rectClip)) rectClip = rectClient;     // 0x1b51fa
//         ::FillRect(pDC->m_hDC, &rectClip, m_brMenuLight (+0x180)); // 0x1b5228
//         CMFCPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(CMFCPopupMenuBar, pBar);
//         if (!pMenuBar->m_bDisableSideBarInXPMode (+0x1350)) {    // 0x1b5248
//             CRect rectImages = rectClient;
//             rectImages.right = rectImages.left + pMenuBar->GetGutterWidth();  // 0x1b5261
//             ::InflateRect(&rectImages, 0, -1);                   // 0x1b5276
//             CBrush br(RGB(0xf1, 0xf1, 0xf1));
//             ::FillRect(pDC->m_hDC, &rectImages, br);             // 0x1b5297
//             CPen* pOld = pDC->SelectObject(&m_penSeparator (+0x1e0));
//             pDC->MoveTo(rectImages.right, rectImages.top);
//             pDC->LineTo(rectImages.right, rectImages.bottom);
//             pDC->SelectObject(pOld);
//         }
//         return;
//     }
//     CMFCVisualManagerOffice2003::OnFillBarBackground(pDC, pBar, rectClient,
//         rectClip, bNCArea);                                      // 0x18f300, 0x1b5322
// The class names come from rtc.py on the two descriptors; +0x1350 is
// kOffDisableSideBarInXPMode in featurepack/menu/CMFCPopupMenuBar.cpp.
// Deviations: retail calls IsKindOf on pBar without a NULL test; OpenMFC
// treats a NULL pBar as "not a popup menu bar".  The WinXpTheme_Blue branch is
// unreachable in this build (file header); its +0x180 / +0x1e0 colours come
// from VS08_Palette(), with the pen caveat the header's APPROXIMATION note
// states.
// Symbol: ?OnFillBarBackground@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerVS2008__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    void* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea)
{
    if (VS08_RichColorDesktop() && pBar != nullptr &&
        VS08_IsKindOf(pBar, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()) &&
        !VS08_IsKindOf(pBar, impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ()) &&
        VS08_StandardWindowsTheme(pThis) == kWinXpThemeBlue) {
        RECT rClip = VS08_ToRECT(rectClip);
        if (::IsRectEmpty(&rClip)) rectClip = rectClient;
        const VS08Palette pal = VS08_Palette(pThis);
        VS08_FillSolid(pDC, rectClip, pal.c110);

        CMFCPopupMenuBar* pMenuBar = reinterpret_cast<CMFCPopupMenuBar*>(pBar);
        int bDisableSideBar = 0;
        std::memcpy(&bDisableSideBar, reinterpret_cast<const char*>(pMenuBar) + 0x1350, sizeof bDisableSideBar);
        if (bDisableSideBar == 0) {
            CRect rectImages = rectClient;
            rectImages.right = rectImages.left + impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(pMenuBar);
            RECT r = VS08_ToRECT(rectImages);
            ::InflateRect(&r, 0, -1);
            rectImages = CRect(r.left, r.top, r.right, r.bottom);
            VS08_FillSolid(pDC, rectImages, RGB(0xf1, 0xf1, 0xf1));
            VS08_VertLine(pDC, rectImages.right, rectImages.top, rectImages.bottom, pal.pen1e0);
        }
        return;
    }
    impl__OnFillBarBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
        pThis, pDC, pBar, rectClient, rectClip, bNCArea);
}

// ?OnFillButtonInterior@CMFCVisualManagerVS2008@@... -- entry 0x1b5510 (mfc140),
// 0x1b6f10 (mfc140u), complete body:
//     if (<menu-bar menu button, see VS08_IsMenuBarMenuButton>) {  // 0x1b5525..0x1b5563
//         rect.bottom -= 2;                                       // 0x1b5569
//         if (pButton->IsDroppedDown()) {                         // vftable +0xf0, call at 0x1b557a
//             if (m_bOSColors (+0x330) == 0)                      // 0x1b5588
//                 this->OnFillHighlightedArea(pDC, rect,
//                     &m_brBarBkgnd (+0x160), pButton);           // vftable +0x638, call at 0x1b55bb
//             else {
//                 CDrawingManager dm(*pDC);
//                 dm.FillGradient(rect, CDrawingManager::PixelAlpha(this->[+0x118], 0x55),
//                                 RGB(255, 255, 255), TRUE, 0, 0);   // 0x1b55c6..0x1b561b
//             }
//             return;
//         }
//     }
//     CMFCVisualManagerOffice2003::OnFillButtonInterior(pDC, pButton, rect, state);  // 0x190490, 0x1b5645
// DEVIATION (dropped-down, m_bOSColors == 0 arm): vftable slot 199 of this
// class is CMFCVisualManagerVS2005::OnFillHighlightedArea, which
// CMFCVisualManagerVS2005.cpp defines with an auto-generated parameter list
// (no `this`) and an empty body; it is not called here and nothing is painted
// on that arm -- see headerRequests.  (The current stub would paint nothing
// either.)  The m_bOSColors arm reads +0x118 through VS08_Palette().
// Symbol: ?OnFillButtonInterior@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerVS2008__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (VS08_IsMenuBarMenuButton(pButton)) {
        rect.bottom -= 2;
        if (VS08_IsDroppedDown(pButton)) {
            if (VS08_OSColors(pThis) != 0) {
                const COLORREF clrStart = VS08_PixelAlpha(VS08_Palette(pThis).c118, 0x55);
                VS08_FillGradient(pDC, rect, clrStart, RGB(255, 255, 255), TRUE);
            }
            // else: this->OnFillHighlightedArea(...) -- see DEVIATION above.
            return;
        }
    }
    impl__OnFillButtonInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        pThis, pDC, pButton, rect, state);
}

// ?OnFillCommandsListBackground@CMFCVisualManagerVS2008@@... -- entry 0x1b68e0
// (mfc140), 0x1b82e0 (mfc140u), complete body:
//     if (!rich-colour desktop)                                        // 0x1b691c..0x1b6971
//         return CMFCVisualManagerOffice2003::OnFillCommandsListBackground(
//                    pDC, rect, bIsSelected);                         // 0x194860, 0x1b6bf3
//     rect.left = 0;                                                   // 0x1b6977
//     BOOL bVS = GetStandardWindowsTheme() == WinXpTheme_Blue           // vftable +0x60, 0x1b6989
//                || m_bOSColors;                                       // 0x1b6993
//     if (bIsSelected) {
//         COLORREF c1, c2, clrBorder;
//         if (!bVS) { c1 = clrBorder = this->[+0x24c]; c2 = this->[+0x248]; }   // 0x1b69b1
//         else      { c1 = 0xeed2c1; c2 = 0xfaf3ef; clrBorder = 0xe2b598; }    // 0x1b69c4
//         if (m_bOSColors) { c1 = this->[+0x118]; c2 = RGB(255,255,255);
//                            clrBorder = this->[+0x11c]; }             // 0x1b69de
//         CDrawingManager dm(*pDC);
//         dm.FillGradient(rect, c1, c2, TRUE, 0, 0);                   // 0x1b6a2b
//         pDC->Draw3dRect(&rect, clrBorder, clrBorder);                // 0x1b6a3c
//         CMFCToolBarMenuButton dummy;                                 // 0x170e00, 0x1b6a46
//         return this->GetHighlightedMenuItemTextColor(&dummy);        // vftable +0x138, 0x1b6a5e
//     }
//     ::FillRect(pDC->m_hDC, &rect, m_brMenuLight (+0x180));           // 0x1b6a94
//     CRect rectImages = rect;
//     rectImages.right = rect.left + CMFCToolBar::GetMenuImageSize().cx
//                        + GetMenuImageMargin() + 2;                   // vftable +0x5c8, 0x1b6aab..0x1b6ad2
//     if (bVS) {
//         CBrush br(RGB(0xf1, 0xf1, 0xf1)); ::FillRect(pDC->m_hDC, &rectImages, br);   // 0x1b6afd
//         CPen* pOld = pDC->SelectObject(&m_penSeparator (+0x1e0));
//         pDC->MoveTo(rectImages.right, rectImages.top);
//         pDC->LineTo(rectImages.right, rectImages.bottom);
//         pDC->SelectObject(pOld);
//     } else {
//         CDrawingManager dm(*pDC);
//         dm.FillGradient(rectImages, this->[+0x220], this->[+0x21c], FALSE, 0, 0);  // 0x1b6bad
//     }
//     return afxGlobalData.clrBarText (+0x74);                         // 0x1b6bd2
// Palette members come from VS08_Palette(); the GetHighlightedMenuItemTextColor
// slot is devirtualized to the OfficeXP body the retail vftable holds, called
// on a default-constructed CMFCToolBarMenuButton exactly as retail builds one.
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerVS2008@@UEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerVS2008__UEAAKPEAVCDC__VCRect__H_Z(
    void* pThis, CDC* pDC, CRect rect, int bIsSelected)
{
    if (!VS08_RichColorDesktop()) {
        return impl__OnFillCommandsListBackground_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__H_Z(
            pThis, pDC, rect, bIsSelected);
    }

    rect.left = 0;
    const int bOSColors = VS08_OSColors(pThis);
    const bool bVS = VS08_StandardWindowsTheme(pThis) == kWinXpThemeBlue || bOSColors != 0;
    const VS08Palette pal = VS08_Palette(pThis);

    if (bIsSelected) {
        COLORREF c1, c2, clrBorder;
        if (!bVS) {
            c1 = clrBorder = pal.c24c;
            c2 = pal.c248;
        } else {
            c1 = RGB(0xc1, 0xd2, 0xee);          // 0xeed2c1
            c2 = RGB(0xef, 0xf3, 0xfa);          // 0xfaf3ef
            clrBorder = RGB(0x98, 0xb5, 0xe2);   // 0xe2b598
        }
        if (bOSColors != 0) {
            c1 = pal.c118;
            c2 = RGB(255, 255, 255);
            clrBorder = pal.c11c;
        }
        VS08_FillGradient(pDC, rect, c1, c2, TRUE);
        VS08_Draw3dRect(pDC, rect, clrBorder, clrBorder);

        alignas(16) unsigned char dummy[0x128];   // retail sizeof(CMFCToolBarMenuButton)
        static_assert(sizeof(CMFCToolBarMenuButton) <= sizeof(dummy),
                      "the stack buffer must hold OpenMFC's CMFCToolBarMenuButton");
        impl___0CMFCToolBarMenuButton__QEAA_XZ(dummy);
        const unsigned long clrText =
            impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
                pThis, reinterpret_cast<CMFCToolBarButton*>(dummy));
        impl___1CMFCToolBarMenuButton__UEAA_XZ(dummy);
        return clrText;
    }

    VS08_FillSolid(pDC, rect, pal.c110);

    CRect rectImages = rect;
    rectImages.right = rect.left + VS08_MenuImageWidth() + kMenuImageMargin + 2;
    if (bVS) {
        VS08_FillSolid(pDC, rectImages, RGB(0xf1, 0xf1, 0xf1));
        VS08_VertLine(pDC, rectImages.right, rectImages.top, rectImages.bottom, pal.pen1e0);
    } else {
        VS08_FillGradient(pDC, rectImages, pal.c220, pal.c21c, FALSE);
    }
    return GD_clrBarText();
}

// ?OnHighlightMenuItem@CMFCVisualManagerVS2008@@... -- entry 0x1b5660 (mfc140),
// 0x1b7060 (mfc140u), complete body:
//     if (!rich-colour desktop) {                                      // 0x1b5692..0x1b56e7
//         CMFCVisualManagerOfficeXP::OnHighlightMenuItem(pDC, pButton, rect, clrText);  // 0x1ae070, 0x1b58c6
//         return;
//     }
//     COLORREF c1, c2, clrPen;
//     if (GetStandardWindowsTheme() == WinXpTheme_Blue)                 // vftable +0x60, 0x1b56f7
//          { c1 = 0xeed2c1; c2 = 0xfaf3ef; clrPen = 0xe2b598; }        // 0x1b5701
//     else { c1 = clrPen = this->[+0x24c]; c2 = this->[+0x248]; }      // 0x1b5713
//     if (m_bOSColors) { c1 = this->[+0x118]; c2 = RGB(255,255,255);
//                        clrPen = this->[+0x11c]; }                    // 0x1b572b
//     ::InflateRect(&rect, -2, 0);                                     // 0x1b5747
//     CRgn rgn; rgn.Attach(::CreateRoundRectRgn(rect.left, rect.top,
//         rect.right, rect.bottom, 3, 3));                             // 0x1b577c
//     pDC->SelectClipRgn(&rgn);                                        // 0x1b5795
//     CDrawingManager dm(*pDC);
//     dm.FillGradient(rect, c1, c2, TRUE, 0, 0);                       // 0x1b57d8
//     pDC->SelectClipRgn(NULL);                                        // 0x1b57e2
//     CPen pen(PS_SOLID, 1, clrPen);                                   // 0x1b57f4
//     CPen* pOldPen = pDC->SelectObject(&pen);
//     CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);      // CDC vftable +0x58, 0x1b581d
//     ::RoundRect(pDC->m_hDC, rect.left, rect.top, rect.right, rect.bottom, 5, 5);   // 0x1b5848
//     pDC->SelectObject(pOldPen); pDC->SelectObject(pOldBrush);
//     clrText = this->GetHighlightedMenuItemTextColor(pButton);        // vftable +0x138, 0x1b5874
// Palette members come from VS08_Palette(); the text-colour slot is
// devirtualized to the OfficeXP body the retail vftable holds.
// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerVS2008__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, unsigned long* pclrText)
{
    if (!VS08_RichColorDesktop()) {
        impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
            pThis, pDC, pButton, rect, pclrText);
        return;
    }

    const VS08Palette pal = VS08_Palette(pThis);
    COLORREF c1, c2, clrPen;
    if (VS08_StandardWindowsTheme(pThis) == kWinXpThemeBlue) {
        c1 = RGB(0xc1, 0xd2, 0xee);        // 0xeed2c1
        c2 = RGB(0xef, 0xf3, 0xfa);        // 0xfaf3ef
        clrPen = RGB(0x98, 0xb5, 0xe2);    // 0xe2b598
    } else {
        c1 = clrPen = pal.c24c;
        c2 = pal.c248;
    }
    if (VS08_OSColors(pThis) != 0) {
        c1 = pal.c118;
        c2 = RGB(255, 255, 255);
        clrPen = pal.c11c;
    }

    RECT r = VS08_ToRECT(rect);
    ::InflateRect(&r, -2, 0);
    rect = CRect(r.left, r.top, r.right, r.bottom);

    HDC hdc = (pDC != nullptr) ? pDC->m_hDC : nullptr;
    HRGN hrgn = ::CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, 3, 3);
    if (hdc != nullptr) ::SelectClipRgn(hdc, hrgn);
    VS08_FillGradient(pDC, rect, c1, c2, TRUE);
    if (hdc != nullptr) ::SelectClipRgn(hdc, nullptr);

    HPEN hpen = ::CreatePen(PS_SOLID, 1, clrPen);
    if (hdc != nullptr) {
        HGDIOBJ hOldPen = (hpen != nullptr) ? ::SelectObject(hdc, hpen) : nullptr;
        HGDIOBJ hOldBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
        ::RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 5, 5);
        if (hOldPen != nullptr) ::SelectObject(hdc, hOldPen);
        if (hOldBrush != nullptr) ::SelectObject(hdc, hOldBrush);
    }

    const unsigned long clrText =
        impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
            pThis, pButton);
    if (pclrText != nullptr) *pclrText = clrText;

    if (hpen != nullptr) ::DeleteObject(hpen);
    if (hrgn != nullptr) ::DeleteObject(hrgn);
}

// ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerVS2008@@... -- entry 0x1b5340
// (mfc140), 0x1b6d40 (mfc140u), complete body:
//     if (rich-colour desktop                                          // 0x1b534c..0x1b53af
//         && this->GetStandardWindowsTheme() == WinXpTheme_Blue) {     // vftable +0x60, 0x1b53bf
//         rect.left--;                                                 // 0x1b53cf
//         rect.right = rect.left + CMFCToolBar::GetMenuImageSize().cx
//                      + 2 * GetMenuImageMargin() + 2;                 // vftable +0x5c8, 0x1b53da..0x1b5403
//         CBrush br(RGB(0xe1, 0xe1, 0xe1));
//         ::FillRect(pDC->m_hDC, &rect, br);                           // 0x1b5417
//         return;
//     }
//     CMFCVisualManagerOffice2003::OnHighlightRarelyUsedMenuItems(pDC, rect);  // 0x195480, 0x1b5449
// The WinXpTheme_Blue branch is unreachable in this build (file header); it
// uses no palette member.
// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerVS2008@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerVS2008__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* pDC, CRect rect)
{
    if (VS08_RichColorDesktop() && VS08_StandardWindowsTheme(pThis) == kWinXpThemeBlue) {
        rect.left--;
        rect.right = rect.left + VS08_MenuImageWidth() + 2 * kMenuImageMargin + 2;
        VS08_FillSolid(pDC, rect, RGB(0xe1, 0xe1, 0xe1));
        return;
    }
    impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(pThis, pDC, rect);
}

// ?OnUpdateSystemColors@CMFCVisualManagerVS2008@@UEAAXXZ -- entry 0x1b6700
// (mfc140), 0x1b8100 (mfc140u):
//     m_bOSColors (+0x330) = this->[+0x50] /* HTHEME */ != NULL
//                            && rich-colour desktop;              // 0x1b6716..0x1b6773
//     CMFCVisualManagerVS2005::OnUpdateSystemColors();             // 0x1b33c0, 0x1b6779
//     if (!m_bOSColors) return;                                    // 0x1b677e
//     ... ::GetThemeColor on +0x50 into +0x220 / +0x21c, then +0x214..+0x240,
//         brushes +0x190 / +0x2d8 and pen +0x298 ...              // 0x1b678a..0x1b68c5
// STUB: in this build the HTHEME is never opened, so the reachable behaviour
// is the +0x330 store (always FALSE) and the VS2005 call, and that VS2005
// body is defined in CMFCVisualManagerVS2005.cpp with an auto-generated
// parameter list (no `this`); see headerRequests.  The constructor above
// clears +0x330 itself, as retail's does, before calling this.  Parameter
// list corrected from the mangled name (`this` was missing).
// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerVS2008@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerVS2008__UEAAXXZ(void* /*pThis*/) {}
