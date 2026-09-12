// CMFCVisualManagerOffice2003 — OpenMFC implementation.
// Sources: global_mfc-feature-58_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCVisualManagerOffice2003 drawing overrides
// (Wave 2, mfc-feature-58).
//
// Every symbol here is an MSVC-x64 virtual override of CMFCVisualManager
// exported by the retail mfc140u.dll under the CMFCVisualManagerOffice2003
// class. The retail bodies paint full Office2003-theme chrome (theme colors,
// cached bitmaps, and ribbon/tab/status-bar internals at offsets the OpenMFC
// headers do not model), so they cannot be transcribed faithfully without
// inventing member layout.
//
// House pattern (matches global_mfc-feature-36_impl.cpp for the equivalent
// CMFCVisualManagerOffice2007 overrides): delegate to the corresponding
// CMFCVisualManager base-class implementation, which draws a generic but
// functional representation. The handful of overrides with no base-class
// virtual declared in the OpenMFC headers are emitted as type-correct
// conservative no-ops.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxmfc.h"

// ::GetThemeSysColor, used by GetThemeColor below exactly as retail does
// (the import slot at 0x1802c5408 resolves to UxTheme.dll!GetThemeSysColor).
// The phase4 link line already carries -luxtheme.
#include <uxtheme.h>

#include <cstdint>
#include <cstring>
#include <cmath>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// Retail decoding notes for the Wave-2 implementations at the bottom of this
// file.
//
// Everything cited below was read out of the retail mfc140.dll shipped with
// MSVC 14.51.36231 (x64) -- the ANSI twin of the mfc140u.dll OpenMFC
// reimplements.  Function BODIES are byte-identical between the two images, so
// control flow, member offsets and constants are valid for either; ADDRESSES
// are not, so every RVA quoted here is tagged "(mfc140)" and is NOT an
// mfc140u address.
//
//  * afxGlobalData is the retail AFX_GLOBAL_DATA singleton at image address
//    0x1803ba380 (mfc140).  Its m_bInitialized guard is the first word, so the
//    recurring "cmpl $0,0x1803ba380 / call 0x18006a5c0" preamble in these
//    bodies is just afxGlobalData.Initialize().  AFX_GLOBAL_DATA::UpdateSysColors
//    (mfc140 RVA 0x6afd0) fills the COLORREF fields used here straight from
//    ::GetSysColor, which pins the mapping:
//        +0x28 == +0x60  clrBtnFace   / clrBarFace    COLOR_BTNFACE      (15)
//        +0x2c == +0x64  clrBtnShadow / clrBarShadow  COLOR_BTNSHADOW    (16)
//        +0x30 == +0x68  clrBtnHilite / clrBarHilite  COLOR_BTNHIGHLIGHT (20)
//        +0x34 == +0x74  clrBtnText   / clrBarText    COLOR_BTNTEXT      (18)
//    (stores at 0x6b066/0x6b069, 0x6b077/0x6b07a, 0x6b0aa/0x6b0ad,
//    0x6b0bb/0x6b0be).  OpenMFC exports afxGlobalData as a zero-filled blob
//    whose Initialize() is a no-op, so reading it would yield black; the GD_*
//    helpers below call ::GetSysColor instead.  This is the same substitution
//    CMFCVisualManager.cpp and CMFCVisualManagerOfficeXP.cpp in this directory
//    already make.
//  * UpdateSysColors also derives, from ::GetSysColor and ::GetDeviceCaps alone:
//        +0x260 = (GetSysColor(COLOR_3DLIGHT)    == 0xffffff &&
//                  GetSysColor(COLOR_BTNFACE)    == 0x000000)   // white HC
//        +0x264 = (GetSysColor(COLOR_3DDKSHADOW) == 0x000000 &&
//                  GetSysColor(COLOR_BTNFACE)    == 0xffffff)   // black HC
//        +0x288 = ::GetDeviceCaps(<screen DC>, BITSPIXEL)
//    (the sequences ending at 0x6b00d, 0x6b035 and 0x6b058).  Unlike the colour
//    members these ARE reproducible exactly, and the helpers below do so.  Most
//    Office2003 bodies open with the same three-way test on them -- written
//    here as Office2003_RichColorDesktop().
//  * The Office2003 object's own theme state lives at +0x108..+0x2e8 and is
//    built by CMFCVisualManagerOffice2003::OnUpdateSystemColors (mfc140 RVA
//    0x190b30).  OpenMFC declares the class as CMFCVisualManager plus
//    char _pad[64] (include/openmfc/afxmfc.h), so it has no storage at those
//    offsets.  The entry points that need it recompute the palette on demand
//    through O2003_Theme() -- see the "theme block, recomputed" section below
//    for what that reproduces and what it cannot.
//  * In retail, CMFCVisualManagerOffice2003 derives from
//    CMFCVisualManagerOfficeXP, which derives from CMFCVisualManagerWindows.
//    Five of this class's exported ordinals are not distinct functions at all:
//    their export-address-table entries point at the INHERITED implementation.
//    Verified two independent ways -- the mfc140u export address table (via
//    mfc_complete_ordinal_mapping.json) resolves them to the base class's RVA,
//    and the retail CMFCVisualManagerOffice2003 vftable (image address
//    0x18031a5c0, mfc140, installed by ??0CMFCVisualManagerOffice2003@@QEAA@XZ
//    at 0x18ed30) holds the base class's function in the matching slot:
//        OnDrawHeaderCtrlBorder        slot 88  -> CMFCVisualManagerWindows
//        OnDrawExpandingBox            slot 100 -> CMFCVisualManagerWindows
//        OnDrawCheckBoxEx              slot 107 -> CMFCVisualManagerWindows
//        OnDrawControlBorder           slot 108 -> CMFCVisualManagerWindows
//        OnDrawPopupWindowButtonBorder slot 125 -> CMFCVisualManagerOfficeXP
//    Those five are therefore implemented as calls to the corresponding sibling
//    impl__ thunk (all five have real bodies in CMFCVisualManagerWindows.cpp /
//    CMFCVisualManagerOfficeXP.cpp as of 2026-09-12).
//=============================================================================

// Implementations in sibling translation units that this unit calls.
// (C++ methods of these classes exist only as impl__ thunks inside this DLL.)
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* pGrid);
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupTextColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* pGrid);
extern "C" unsigned long MS_ABI impl__GetSmartDockingHighlightToneColor_CMFCVisualManager__UEAAKXZ(
    CMFCVisualManager* pThis);
extern "C" int MS_ABI impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(
    void* pThis);
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManager__UEAAXXZ();
extern "C" void MS_ABI impl__RedrawAll_CMFCVisualManager__SAXXZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ();
// The four CMFCVisualManagerWindows entries and the OfficeXP one that this
// class's exports alias (see the file header).  These declarations are derived
// from the mangled export names and match the definitions in
// CMFCVisualManagerWindows.cpp / CMFCVisualManagerOfficeXP.cpp (verified
// 2026-09-12; both files carry real bodies for them).
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHH_Z(
    void* pThis, CDC* pDC, CRect rect, int nState, int bHighlighted, int bPressed, int bEnabled);
extern "C" void MS_ABI impl__OnDrawControlBorder_CMFCVisualManagerWindows__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWndCtrl);
extern "C" void MS_ABI impl__OnDrawExpandingBox_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HK_Z(
    void* pThis, CDC* pDC, CRect rect, int bIsOpened, unsigned long colorBox);
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerWindows__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
    void* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, int bIsPressed, int bIsHighlighted);
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    void* pThis, CDC* pDC, CRect rect, void* pButton /* CMFCDesktopAlertWndButton* */);

namespace {

// AFX_GLOBAL_DATA COLORREF fields, named by their retail offset and mapped to
// the ::GetSysColor index AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0) stores
// into each -- see the file header.
inline COLORREF GD_clrBarFace()   { return ::GetSysColor(COLOR_BTNFACE); }      // +0x60
inline COLORREF GD_clrBarShadow() { return ::GetSysColor(COLOR_BTNSHADOW); }    // +0x64
inline COLORREF GD_clrBarHilite() { return ::GetSysColor(COLOR_BTNHIGHLIGHT); } // +0x68
inline COLORREF GD_clrBarText()   { return ::GetSysColor(COLOR_BTNTEXT); }      // +0x74
inline COLORREF GD_clrWindow()    { return ::GetSysColor(COLOR_WINDOW); }       // +0x78

// afxGlobalData +0x260 / +0x264 / +0x288, reproduced exactly from
// UpdateSysColors (see the file header).
inline bool GD_IsWhiteHighContrast()
{
    return ::GetSysColor(COLOR_3DLIGHT) == 0x00ffffffu &&
           ::GetSysColor(COLOR_BTNFACE) == 0x00000000u;
}
inline bool GD_IsBlackHighContrast()
{
    return ::GetSysColor(COLOR_3DDKSHADOW) == 0x00000000u &&
           ::GetSysColor(COLOR_BTNFACE)    == 0x00ffffffu;
}
inline int GD_BitsPerPixel()
{
    HDC hdcScreen = ::GetDC(nullptr);
    if (hdcScreen == nullptr) return 32;
    int bpp = ::GetDeviceCaps(hdcScreen, BITSPIXEL);
    ::ReleaseDC(nullptr, hdcScreen);
    return bpp;
}

// The guard almost every Office2003 body opens with:
//     afxGlobalData.Initialize();
//     if (afxGlobalData.m_nBitsPerPixel (+0x288) > 8 &&
//         afxGlobalData[+0x264] == 0 && afxGlobalData[+0x260] == 0)  ... themed
//     else ... plain fallback
// (e.g. GetToolTipInfo 0x1970c8..0x1970fb, GetSmartDockingBaseGuideColors
// 0x195791..0x1957c5).
inline bool Office2003_RichColorDesktop()
{
    return GD_BitsPerPixel() > 8 && !GD_IsBlackHighContrast() && !GD_IsWhiteHighContrast();
}

// CMFCToolTipInfo field offsets.  OpenMFC declares CMFCToolTipInfo as an
// opaque 96-byte blob with no base class and no virtuals
// (include/openmfc/afxmfc.h), so the object address is the address of its first
// field and the retail offsets can be written directly.  The offsets are the
// public MFC CMFCToolTipInfo member order, which the stores in the retail body
// (mfc140 RVA 0x197090) match one for one; the whole 0x00..0x33 byte range
// fits inside the 96-byte blob.
const std::ptrdiff_t kTTI_bBoldLabel        = 0x00;
const std::ptrdiff_t kTTI_bDrawDescription  = 0x04;
const std::ptrdiff_t kTTI_bDrawIcon         = 0x08;
const std::ptrdiff_t kTTI_bRoundedCorners   = 0x0c;
const std::ptrdiff_t kTTI_bBalloonTooltip   = 0x10;
const std::ptrdiff_t kTTI_bDrawSeparator    = 0x14;
const std::ptrdiff_t kTTI_bVislManagerTheme = 0x18;
const std::ptrdiff_t kTTI_nMaxDescrWidth    = 0x1c;
const std::ptrdiff_t kTTI_nGradientAngle    = 0x20;
const std::ptrdiff_t kTTI_clrFill           = 0x24;
const std::ptrdiff_t kTTI_clrFillGradient   = 0x28;
const std::ptrdiff_t kTTI_clrText           = 0x2c;
const std::ptrdiff_t kTTI_clrBorder         = 0x30;

inline void TTI_SetInt(void* pInfo, std::ptrdiff_t off, int value)
{
    *reinterpret_cast<int*>(static_cast<char*>(pInfo) + off) = value;
}

} // namespace

//=============================================================================
// Wave-3 additions: the CMFCVisualManagerOffice2003 theme block, recomputed.
//
// Retail keeps this class's palette in the object at +0x108..+0x2e8 and builds
// it in CMFCVisualManagerOffice2003::OnUpdateSystemColors (mfc140 RVA
// 0x190b30).  OpenMFC's object has no storage there (see the file header), so
// -- exactly as CMFCVisualManagerOfficeXP.cpp does with XP_Colors() -- the
// palette is recomputed on demand by O2003_Theme() below, from the same inputs
// OnUpdateSystemColors reads.  Everything in this block was read out of the
// retail mfc140.dll; function-entry RVAs are cited per helper and are mfc140
// addresses, not mfc140u ones.
//
// What is and is not reproduced, stated once here:
//  * The "standard Windows XP theme" path.  OnUpdateSystemColors stores
//    GetStandardWindowsTheme() (vftable +0x60, slot 12) in +0x210 and sets
//    +0x2e8 = (unsigned)(style - 1) <= 2 (0x190bcc..0x190be0).  When +0x2e8 is
//    set, the palette is read through UxTheme (GetThemeColor on the HTHEMEs the
//    object keeps at +0x20 and +0x50, GetThemeSysColor on +0x08; the run at
//    0x190fbb..0x19135f and 0x19166c..0x19178a).  OpenMFC has no storage for
//    those handles, CMFCBaseVisualManager::UpdateSystemColors never opens them,
//    and its GetStandardWindowsTheme thunk returns 0, so +0x2e8 is FALSE here
//    and O2003_Theme() reproduces the +0x2e8 == 0 path only.  The style value
//    is still taken from the thunk, so the four-way switch at 0x190c90..
//    0x190f86 is transcribed in full.
//  * The m_bUseGlobalTheme == FALSE probe (0x190b62..0x190b9c: style is forced
//    to -1 when ::GetWindowTheme(AfxGetMainWnd()->m_hWnd) is NULL) is not
//    reproduced: AfxGetMainWnd is a C++ symbol this unit may not reference.
//    With the thunk returning 0 the difference is invisible -- both 0 and -1
//    take the `default` arm of the switch and clear +0x2e8.
//  * On the low-colour / high-contrast path (0x19185d..0x191a5e) retail writes
//    only +0x218/+0x220/+0x224 and the ribbon colours +0x26c..+0x294; every
//    other member keeps whatever it held before.  The constructor (0x18ed30)
//    does not initialise them, so in retail they are undefined on such a
//    desktop.  O2003_Theme() zero-fills them instead and says so.
//  * ModifyGlobalColors (vftable +0x658, called at 0x190bf0) is a no-op unless
//    +0x2e8 is set (see its own comment), so nothing is lost by not calling it.
//  * One store outside the +0x108..+0x2e8 block: on the rich-colour path retail
//    also writes afxGlobalData.clrBarFace into this->[+0xf8], the inherited
//    CMFCVisualManager::m_clrMenuShadowBase (0x19160d..0x19162a).  OpenMFC's
//    CMFCVisualManager keeps that member in opaque padding, so the store is not
//    reproduced; nothing in this file reads it.
//=============================================================================

// CDrawingManager statics that ARE implemented in phase4/src/core/gdi/CDrawingManager.cpp
// (their comments record exhaustive sweeps against the retail DLL).
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKH_Z(unsigned long srcPixel, int nPercent);
extern "C" void MS_ABI impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(unsigned long rgb, double* pH, double* pS, double* pL);
extern "C" double MS_ABI impl__HuetoRGB_CDrawingManager__SANNNN_Z(double m1, double m2, double h);
// CDrawingManager members that are still stubs in this tree; only reached on the
// CMFCToolBarImages::m_bIsDrawOnGlass path, where retail uses them instead of GDI.
// The definitions in CDrawingManager.cpp carry auto-generated parameter lists
// that do not match these (derived from the mangled names); see headerRequests.
extern "C" void MS_ABI impl__DrawLine_CDrawingManager__QEAAXHHHHK_Z(void* pThis, int x1, int y1, int x2, int y2, unsigned long clr);
extern "C" void MS_ABI impl__DrawRect_CDrawingManager__QEAAXAEBVCRect__KK_Z(void* pThis, const CRect* pRect, unsigned long clrFill, unsigned long clrLine);
// CDC entry points (phase4/src/core/gdi/CDC.cpp, all real bodies).
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXHHHHK_Z(CDC* pThis, int x, int y, int cx, int cy, unsigned long clr);
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y);
extern "C" int MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y);
// CMenuImages::Draw, both overloads.  Defined -- as empty stubs with
// auto-generated parameter lists that do NOT match the mangled names -- in
// phase4/src/featurepack/controls/CMFCReBar.cpp; these declarations are the
// ones derived from the names (see headerRequests).
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int nImageID, const CRect& rect, int nImageState, const CSize& sizeImage);
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int nImageID, const CPoint& pt, int nImageState, const CSize& sizeImage);
// CMFCToolBar statics (phase4/src/featurepack/toolbar/StaticData.cpp and CMFCToolBar.cpp).
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;
extern "C" std::int32_t impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA;
extern "C" CSize impl__m_sizeButton_CMFCToolBar__1VCSize__A;
extern "C" CSize impl__m_sizeCurButton_CMFCToolBar__1VCSize__A;
extern "C" CSize impl__m_sizeImage_CMFCToolBar__1VCSize__A;
extern "C" CSize impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A;
extern "C" CSize impl__m_sizeCurImage_CMFCToolBar__1VCSize__A;
// CMFCToolBar::GetRowHeight -- a `return 0` stub in CMFCToolBar.cpp today, whose
// definition also omits the `this` parameter; declared here from the mangled name.
extern "C" int MS_ABI impl__GetRowHeight_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
// Base-class / OfficeXP bodies this class falls back to.  All of these
// declarations are derived from the mangled names and match the definitions in
// CMFCVisualManager.cpp / CMFCVisualManagerOfficeXP.cpp (checked 2026-09-12;
// the OfficeXP ones are real transcribed bodies, no longer placeholders).
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManager__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    CMFCVisualManager* pThis, const CMFCBaseTabCtrl* pTabWnd, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack);
extern "C" void MS_ABI impl__OnDrawAutoHideButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectBounds, CRect rectBorderSize, void* pButton);
extern "C" void MS_ABI impl__OnFillTasksPaneBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect);
extern "C" void MS_ABI impl__OnDrawOutlookBarSplitter_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectSplitter);
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect* pRect, int bIsHighlighted, int bIsPressed);
// Both defined in phase4/src/featurepack/visualmanager/Thunks.cpp (they forward
// to the C++ virtuals CMFCVisualManager.cpp implements).
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryCaption_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonContextCaption___Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonContextCaption* pCaption);
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryTab_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonTab__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCRibbonTab* pTab, int bIsActive);
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    void* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar);
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    void* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state);
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    void* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, void* pButton);
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    void* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted, void* pButton);
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    void* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect);
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    void* pThis, CDC* pDC, CBasePane* pBar, CRect* pRect);
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    void* pThis, CDC* pDC, void* pBar, int bActive, CRect rectCaption, CRect rectButtons);
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
    void* pThis, CDC* pDC, CRect rectCaption, void* pPopupWnd);
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    void* pThis, CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton);
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* pDC, CRect rect);
// The two OnUpdateSystemColors bodies retail calls around its own stores; both
// are empty in this tree (CMFCBaseVisualManager.cpp cannot open the UxTheme
// handles it has no storage for; CMFCVisualManagerOfficeXP.cpp recomputes its
// palette on demand through XP_Colors() instead of caching it here).
extern "C" void MS_ABI impl__UpdateSystemColors_CMFCBaseVisualManager__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOfficeXP__MEAAXXZ(void* pThis);
// This file's own GetBaseThemeColor (defined below) -- vftable +0x640, slot 200.
extern "C" unsigned long MS_ABI impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(CMFCVisualManagerOffice2003* pThis);

namespace {

// More AFX_GLOBAL_DATA fields, mapped by the same UpdateSysColors (0x6afd0)
// stores as the GD_* helpers above (0x6b06c..0x6b0c6 and 0x6b0cf..).
inline COLORREF GD_clrBtnFace()     { return ::GetSysColor(COLOR_BTNFACE); }      // +0x28
inline COLORREF GD_clrBtnShadow()   { return ::GetSysColor(COLOR_BTNSHADOW); }    // +0x2c
inline COLORREF GD_clrBtnHilite()   { return ::GetSysColor(COLOR_BTNHIGHLIGHT); } // +0x30
inline COLORREF GD_clrBtnDkShadow() { return ::GetSysColor(COLOR_3DDKSHADOW); }   // +0x3c
inline COLORREF GD_clrHilite()      { return ::GetSysColor(COLOR_HIGHLIGHT); }    // +0x48
inline COLORREF GD_clrBarDkShadow() { return ::GetSysColor(COLOR_3DDKSHADOW); }   // +0x6c
inline COLORREF GD_clrBarLight()    { return ::GetSysColor(COLOR_3DLIGHT); }      // +0x70
// afxGlobalData's CBrush members at +0xa8 (brBtnFace) and +0x118 (brBarFace):
// UpdateSysColors creates them with ::CreateSolidBrush(+0x28) at 0x6b23b and
// ::CreateSolidBrush(+0x60) at 0x6b25e.  OpenMFC's afxGlobalData is a zero
// blob with no brush handles, so callers create an equivalent brush locally.
inline COLORREF GD_brBtnFaceColor() { return ::GetSysColor(COLOR_BTNFACE); }
inline COLORREF GD_brBarFaceColor() { return ::GetSysColor(COLOR_BTNFACE); }

//--------------------------------------------------------------------------
// Colour arithmetic transcribed from CDrawingManager.  The tree's own
// SmartMixColors / HLStoRGB_ONE / HSVtoRGB / RGBtoHSV thunks are `return 0`
// stubs (phase4/src/core/gdi/CDrawingManager.cpp), so the bodies are carried
// here.  Only RGBtoHSL and HuetoRGB, which are real there, are called.
//--------------------------------------------------------------------------

// Per channel: (wa*a + wb*b) / 255, the `mul $0x80808081; shr $7` idiom
// (exact unsigned /255 over this range) that OfficeXP::OnUpdateSystemColors
// uses at 0x1ac217..0x1ac286 and 0x1ac334..0x1ac39e.
inline COLORREF DM_Blend255(COLORREF a, unsigned wa, COLORREF b, unsigned wb)
{
    const unsigned r  = (wa * (unsigned)GetRValue(a) + wb * (unsigned)GetRValue(b)) / 255u;
    const unsigned g  = (wa * (unsigned)GetGValue(a) + wb * (unsigned)GetGValue(b)) / 255u;
    const unsigned bl = (wa * (unsigned)GetBValue(a) + wb * (unsigned)GetBValue(b)) / 255u;
    return RGB(r & 0xffu, g & 0xffu, bl & 0xffu);
}

// CDrawingManager::PixelAlpha(COLORREF, double, double, double) -- mfc140 RVA
// 0x5b160, complete body.  NOTE: nothing in this file calls it -- retail's
// only use inside OnUpdateSystemColors is on the +0x2e8 (standard XP theme)
// path at 0x19132e, which is not transcribed; it is kept for when that path
// is.  Per channel: v = (int)(ch * k + 0.5); when the
// channel is 0 and k > 1.0, v = (int)((k - 1.0) * 255 + 0.5) instead
// (0x5b18b..0x5b1a5); the result byte is v when v < 255, else 255.
inline COLORREF DM_PixelAlphaF(COLORREF c, double kR, double kG, double kB)
{
    auto chan = [](unsigned ch, double k) -> unsigned {
        int v = (int)((double)ch * k + 0.5);
        if (ch == 0 && k > 1.0) v = (int)((k - 1.0) * 255.0 + 0.5);
        return (v < 255) ? (unsigned)(v & 0xff) : 255u;
    };
    return RGB(chan(GetRValue(c), kR), chan(GetGValue(c), kG), chan(GetBValue(c), kB));
}

// CDrawingManager::RGBtoHSV(COLORREF, double* H, double* S, double* V) --
// mfc140 RVA 0x5b870, complete body.  Channels are scaled to [0,1]; V is the
// max; if V == 0 then S = 0 and H = -1 (0x5b8e8..0x5b8f9); otherwise
// S = delta / V, and H is 60 * (the usual sector formula) -- except that a
// zero delta yields H = 60 (the `movsd 1.0` at 0x5b91c feeding the `mulsd 60`
// at 0x5b968), and a negative H has 360 added (0x5b97a).
inline void DM_RGBtoHSV(COLORREF rgb, double* pH, double* pS, double* pV)
{
    const double r = GetRValue(rgb) / 255.0;
    const double g = GetGValue(rgb) / 255.0;
    const double b = GetBValue(rgb) / 255.0;
    double mx = g > b ? g : b; if (r > mx) mx = r;
    double mn = g < b ? g : b; if (r < mn) mn = r;
    *pV = mx;
    if (mx == 0.0) { *pS = 0.0; *pH = -1.0; return; }
    const double delta = mx - mn;
    *pS = delta / mx;
    double h;
    if (delta == 0.0)   h = 1.0;
    else if (r == mx)   h = (g - b) / delta;
    else if (g == mx)   h = (b - r) / delta + 2.0;
    else                h = (r - g) / delta + 4.0;
    h *= 60.0;
    if (h < 0.0) h += 360.0;
    *pH = h;
}

// CDrawingManager::HSVtoRGB(double H, double S, double V) -- mfc140 RVA
// 0x5b9a0, complete body.  S == 0 gives the grey (V,V,V); otherwise the
// sector i = (int)floor(H / 60) (the api-ms-win-crt-math floor import at
// 0x1802c5588) selects the standard (V, t, p) permutation, with the
// `default` arm (0x5ba4a) covering i == 5 and anything out of range.  Each
// channel is (int)(c * 255 + 0.5) truncated to a byte.
inline COLORREF DM_HSVtoRGB(double H, double S, double V)
{
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
    return RGB(r, g, b);
}

// CDrawingManager::HLStoRGB_ONE(double H, double L, double S) -- mfc140 RVA
// 0x5b4a0, complete body.  S == 0 gives (L,L,L).  Otherwise
//     m2 = (L <= 0.5) ? (S + 1) * L : (L == 1.0 ? L : L + S - L*S)   (0x5b4e3..0x5b524)
//     m1 = 2*L - m2
//     R = HuetoRGB(m1, m2, H + 1/3), G = HuetoRGB(m1, m2, H),
//     B = HuetoRGB(m1, m2, H - 1/3)                                  (0x5b545/0x5b559/0x5b576)
// and each channel is (int)(c * 255) truncated to a byte (0x5b5a0..0x5b5d0).
inline COLORREF DM_HLStoRGB_ONE(double H, double L, double S)
{
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
    return RGB(r, g, b);
}

// CDrawingManager::SmartMixColors(COLORREF color1, COLORREF color2,
//         double dblLumRatio, int k1, int k2) -- mfc140 RVA 0x5bae0, complete body:
//     if (k1 < 0 || k2 < 0) AfxThrowInvalidArgException();    // 0x5bb07 / 0x5bb12 -> 0x225b80
//     if (k1 + k2 == 0) return 0;                             // 0x5bb1e
//     mix = RGB((R1*k1 + R2*k2)/(k1+k2), ...);               // 0x5bb37..0x5bb9c
//     RGBtoHSV(mix, &h1, &s1, &v);  RGBtoHSV(color2, &h2, &s2, &v);   // 0x5bba2 / 0x5bbb6
//         (the second call overwrites v -- both pass the same &v slot)
//     rgb = HSVtoRGB(h1, (k1*s1 + k2*s2)/(k1+k2), v);         // 0x5bbef
//     if (dblLumRatio == 1.0) return rgb;                     // 0x5bbfc
//     RGBtoHSL(rgb, &h, &s, &l);                              // 0x5bc16
//     return HLStoRGB_ONE(h, min(1.0, l * dblLumRatio), s);   // 0x5bc35
COLORREF DM_SmartMixColors(COLORREF color1, COLORREF color2, double dblLumRatio, int k1, int k2)
{
    if (k1 < 0 || k2 < 0) return 0;          // DEVIATION: retail throws CInvalidArgException (0x225b80); no caller here passes a negative weight
    const int k = k1 + k2;
    if (k == 0) return 0;
    const COLORREF mix = RGB(((int)GetRValue(color1) * k1 + (int)GetRValue(color2) * k2) / k,
                             ((int)GetGValue(color1) * k1 + (int)GetGValue(color2) * k2) / k,
                             ((int)GetBValue(color1) * k1 + (int)GetBValue(color2) * k2) / k);
    double h1 = 0.0, s1 = 0.0, h2 = 0.0, s2 = 0.0, v = 0.0;
    DM_RGBtoHSV(mix, &h1, &s1, &v);
    DM_RGBtoHSV(color2, &h2, &s2, &v);
    const COLORREF rgb = DM_HSVtoRGB(h1, ((double)k1 * s1 + (double)k2 * s2) / (double)k, v);
    if (dblLumRatio == 1.0) return rgb;
    double h = 0.0, s = 0.0, l = 0.0;
    impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(rgb, &h, &s, &l);
    l *= dblLumRatio;
    if (l > 1.0) l = 1.0;
    return DM_HLStoRGB_ONE(h, l, s);
}

inline COLORREF DM_PixelAlpha(COLORREF c, int nPercent)
{
    return impl__PixelAlpha_CDrawingManager__SAKKH_Z(c, nPercent);
}

//--------------------------------------------------------------------------
// The CMFCVisualManagerOfficeXP members this class inherits and reads
// (+0x108, +0x10c, +0x110, +0x118, +0x11c, +0x134, +0x138), as
// CMFCVisualManagerOfficeXP::OnUpdateSystemColors (mfc140 RVA 0x1ac0e0)
// derives them.  This duplicates the corresponding fields of XP_Colors() in
// CMFCVisualManagerOfficeXP.cpp (that helper is file-local there); the stores
// were re-read from retail for this file:
//   gradient path (bpp > 8, no high contrast; 0x1ac1c4..0x1ac682):
//     +0x110 = blend(clrWindow 219, clrBarFace 36)/255          store 0x1ac289
//     +0x108 = blend(clrBarFace 215, clrWindow 40)/255          store 0x1ac3ac
//     +0x10c = PixelAlpha(+0x108, 94)                           store 0x1ac3bd
//     +0x114 = PixelAlpha(clrBarFace, 55)                       store 0x1ac3cd
//     mix    = blend(+0x110 178, clrHilite 77)/255              0x1ac42e..0x1ac499
//     L      = RGBtoHSL(clrHilite).L (0x1ac414), compared with 0.8 (0x1ac49b)
//     L > 0.8:  +0x118 = PixelAlpha(mix, 91); +0x11c = PixelAlpha(mix, 98);
//               +0x138 = PixelAlpha(clrHilite, 84)              0x1ac4ac..0x1ac4ef
//     else:     +0x118 = PixelAlpha(mix, 102); +0x11c = PixelAlpha(+0x118, 87);
//               +0x138 = clrHilite                              0x1ac4fb..0x1ac533
//     +0x134 = +0x114                                           store 0x1ac615
//   low-colour / high-contrast path (0x1ac687..0x1ac8e7):
//     +0x110 = clrWindow; +0x108 = clrBtnFace;
//     white HC (+0x260): +0x118 = +0x11c = clrHilite, +0x10c = clrBtnFace
//     else:              +0x118 = +0x11c = clrBtnFace, +0x10c = clrBarLight
//     +0x134 = clrBtnDkShadow (+0x3c, 0x1ac867)
//     +0x138 = (no HC flag set) ? clrHilite : clrBtnDkShadow   0x1ac8a3..0x1ac8e7
// clrWindow is what retail's vftable +0x630 (GetWindowColor) returns; this
// class's override (0x195450) is GetSysColor(COLOR_WINDOW) without an HTHEME.
//--------------------------------------------------------------------------
struct O2003_XPColors {
    COLORREF c108, c10c, c110, c118, c11c, c134, c138;
};

O2003_XPColors O2003_XP()
{
    O2003_XPColors x;
    const COLORREF clrBarFace = GD_clrBarFace();
    const COLORREF clrWindow  = GD_clrWindow();
    const COLORREF clrHilite  = GD_clrHilite();
    const bool bWhiteHC = GD_IsWhiteHighContrast();
    const bool bBlackHC = GD_IsBlackHighContrast();

    if (GD_BitsPerPixel() > 8 && !bWhiteHC && !bBlackHC) {
        x.c110 = DM_Blend255(clrWindow, 0xdb, clrBarFace, 0x24);
        x.c108 = DM_Blend255(clrBarFace, 0xd7, clrWindow, 0x28);
        x.c10c = DM_PixelAlpha(x.c108, 0x5e);
        const COLORREF c114 = DM_PixelAlpha(clrBarFace, 0x37);
        const COLORREF clrMix = DM_Blend255(x.c110, 0xb2, clrHilite, 0x4d);
        double dH = 0.0, dS = 0.0, dL = 0.0;
        impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(clrHilite, &dH, &dS, &dL);
        if (dL > 0.8) {
            x.c118 = DM_PixelAlpha(clrMix, 0x5b);
            x.c11c = DM_PixelAlpha(clrMix, 0x62);
            x.c138 = DM_PixelAlpha(clrHilite, 0x54);
        } else {
            x.c118 = DM_PixelAlpha(clrMix, 0x66);
            x.c11c = DM_PixelAlpha(x.c118, 0x57);
            x.c138 = clrHilite;
        }
        x.c134 = c114;
    } else {
        x.c110 = clrWindow;
        x.c108 = clrBarFace;
        if (bWhiteHC) { x.c118 = x.c11c = clrHilite;  x.c10c = clrBarFace; }
        else          { x.c118 = x.c11c = clrBarFace; x.c10c = GD_clrBarLight(); }
        x.c134 = GD_clrBtnDkShadow();
        x.c138 = (!bBlackHC && !bWhiteHC) ? clrHilite : GD_clrBtnDkShadow();
    }
    return x;
}

//--------------------------------------------------------------------------
// The Office2003 theme block, named by retail offset.  Derivation: see
// O2003_Theme() and the block comment at the top of this section.
//--------------------------------------------------------------------------
struct O2003Theme {
    int      nStyle;                 // +0x210  GetStandardWindowsTheme() result
    bool     bIsStandardWinXPTheme;  // +0x2e8  (unsigned)(nStyle - 1) <= 2  (always false here)
    bool     bRich;                  // bpp > 8 && no high-contrast flag
    O2003_XPColors xp;               // +0x108..+0x138 (untouched on the +0x2e8 == 0 path)
    COLORREF c214, c218, c21c, c220, c224, c228, c22c;
    COLORREF c230, c234, c238, c23c, c240, c244, c248, c24c;
    COLORREF c250, c254, c258, c25c, c260, c264, c268;
    COLORREF c26c, c270, c274, c278, c27c, c280, c284, c288, c28c, c290, c294;
    COLORREF pen1e0;                 // CPen at +0x1e0, made at 0x1915e0
    COLORREF pen298;                 // CPen at +0x298, made at 0x19165b (= c238)
    COLORREF pen2a8;                 // CPen at +0x2a8, made at 0x191550 (clrBarHilite)
    COLORREF pen2b8;                 // CPen at +0x2b8, made at 0x191847/0x191a29 (= c294)
    COLORREF brush190;               // CBrush at +0x190, made at 0x191508 (= c220)
    COLORREF brush2c8;               // CBrush at +0x2c8, made at 0x191592 (clrBarFace)
    COLORREF brush2d8;               // CBrush at +0x2d8, made at 0x1915b8 (= c220)
};

// GetThemeColor(this->[+0x08], id) as OnUpdateSystemColors inlines it
// (e.g. 0x190cac..0x190ccc): GetThemeSysColor when the HTHEME is non-NULL,
// else ::GetSysColor.  OpenMFC never has the HTHEME, so this is the latter.
inline COLORREF O2003_ThemeSysColor(int nColorId) { return ::GetSysColor(nColorId); }

// CMFCVisualManagerOffice2003::OnUpdateSystemColors (mfc140 RVA 0x190b30),
// +0x2e8 == 0 path, recomputed.  Reads in order:
//   0x190b51  CMFCBaseVisualManager::UpdateSystemColors (0x18da80) -- opens the
//             UxTheme handles; a stub in this tree, nothing to reproduce.
//   0x190b59  m_bUseGlobalTheme == 0 -> ::GetWindowTheme probe (not reproduced, see above)
//   0x190ba4  style = vftable[12]() = GetStandardWindowsTheme()
//   0x190bb4  m_bDefaultWinXPColors == 0 && style != -1  ->  style = 0
//   0x190bcc  +0x2e8 = (unsigned)(style - 1) <= 2
//   0x190be9  vftable[203]() = ModifyGlobalColors() (no-op while +0x2e8 == 0)
//   0x190bf9  CMFCVisualManagerOfficeXP::OnUpdateSystemColors (0x1ac0e0) -> O2003_XP()
//   0x190c24  bpp > 8 && !blackHC && !whiteHC  -> rich path, else 0x19185d
O2003Theme O2003_Theme(CMFCVisualManagerOffice2003* pThis)
{
    O2003Theme t;
    std::memset(&t, 0, sizeof(t));

    int nStyle = impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(pThis);
    if (impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA == 0 && nStyle != -1) nStyle = 0;
    t.nStyle = nStyle;
    t.bIsStandardWinXPTheme = (unsigned)(nStyle - 1) <= 2u;
    // DEVIATION: if a future GetStandardWindowsTheme ever returned 1..3 here,
    // retail would take the UxTheme path this tree cannot run (no HTHEMEs);
    // the +0x2e8 == 0 derivation below is used regardless.

    t.xp = O2003_XP();
    t.bRich = Office2003_RichColorDesktop();

    if (t.bRich) {
        // 0x190c60: base = vftable[200]() = GetBaseThemeColor()
        const COLORREF base = impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(pThis);
        const COLORREF clrFace   = O2003_ThemeSysColor(COLOR_BTNFACE);       // 0xf
        const COLORREF clrWindow = O2003_ThemeSysColor(COLOR_WINDOW);        // 5
        switch (nStyle) {
        case 2:   // "homestead" (Olive), 0x190c99..0x190d29 then 0x190dcd
            t.c21c = DM_PixelAlpha(base, 0x78);
            t.c214 = DM_SmartMixColors(base, clrFace, 0.87, 1, 3);
            t.c220 = DM_SmartMixColors(base, clrWindow, 1.0, 2, 1);
            t.c218 = DM_SmartMixColors(base, clrWindow, 1.03, 1, 1);
            break;
        case 3:   // "metallic" (Silver), 0x190d2e..0x190dc7 then 0x190dcd
            t.c21c = DM_SmartMixColors(base, clrFace, 0.75, 2, 1);
            t.c214 = DM_PixelAlpha(base, 0x78);
            t.c220 = DM_SmartMixColors(base, O2003_ThemeSysColor(COLOR_BTNHIGHLIGHT), 0.98, 1, 1);
            t.c218 = DM_SmartMixColors(base, clrWindow, 1.03, 1, 1);
            break;
        case 1:   // "normalcolor" (Luna Blue), 0x190e0a..0x190ebf then 0x190f73
            t.c21c = DM_SmartMixColors(base, clrFace, 0.93, 2, 1);
            t.c214 = DM_SmartMixColors(base, O2003_ThemeSysColor(COLOR_3DLIGHT), 0.99, 2, 1);
            t.c220 = t.c218 = DM_SmartMixColors(base, clrWindow, 1.03, 1, 1);
            break;
        default:  // no standard scheme (0 / -1), 0x190ec4..0x190f80
            t.c21c = DM_SmartMixColors(base, clrFace, 0.93, 2, 1);
            t.c214 = DM_SmartMixColors(base, O2003_ThemeSysColor(COLOR_3DLIGHT), 0.99, 2, 1);
            t.c220 = t.c218 = DM_SmartMixColors(base, clrWindow, 1.0, 1, 4);
            break;
        }
        t.c228 = t.c220;                                   // 0x190f9d
        t.c22c = DM_PixelAlpha(t.c21c, 0x62);              // 0x190fa3
        // +0x2e8 == 0: 0x191364..0x1914c5
        t.c248 = t.xp.c118;
        t.c24c = t.xp.c11c;
        t.c244 = (COLORREF)-1;
        // 0x191364 loads +0x118 into %ecx and 0x19136f loads +0x11c into %eax;
        // the PixelAlpha call at 0x19138b takes %ecx, so +0x250 derives from
        // +0x118 (the +0x248 colour), not from +0x11c.
        t.c250 = DM_PixelAlpha(t.xp.c118, 0x78);
        t.c230 = GD_clrBarShadow();
        t.c234 = DM_SmartMixColors(t.c230, GD_clrBarFace(), 1.0, 1, 1);
        t.c238 = DM_PixelAlpha(t.c21c, 0x4b);
        t.c23c = GD_clrBarLight();
        t.c240 = t.c21c;
        t.c260 = GD_clrBarShadow();
        t.c264 = GD_clrBarShadow();
        t.c268 = GD_clrBarDkShadow();
        // 0x1914cb..0x191667
        t.c254 = t.c24c;
        t.c258 = t.c24c;
        t.c25c = DM_PixelAlpha(t.c250, 0x78);
        t.brush190 = t.c220;
        t.pen2a8   = GD_clrBarHilite();
        t.brush2c8 = GD_clrBarFace();
        t.brush2d8 = t.c220;
        t.pen1e0   = GD_clrBarShadow();                    // edi from 0x191486 on this path
        t.c224 = DM_SmartMixColors(t.c21c, t.c220, 0.92, 1, 1);
        t.pen298   = t.c238;
        // ribbon colours, +0x2e8 == 0 (or no +0x50 HTHEME): 0x19178f..0x19182e
        t.c26c = t.c214;
        t.c270 = t.c220;
        t.c274 = t.c214;
        t.c278 = t.c220;
        t.c27c = DM_SmartMixColors(t.c230, DM_PixelAlpha(GD_clrBarShadow(), 0x7d), 1.0, 1, 1);
        t.c280 = t.c234;
        t.c284 = t.c288 = t.c28c = t.c290 = t.c294 = t.c220;
        t.pen2b8 = t.c294;
    } else {
        // Low-colour / high-contrast path, 0x19185d..0x191a58.  Only these
        // members are written by retail; the rest are left as they were
        // (undefined after the constructor) and are zero here.
        t.c26c = t.c270 = GD_clrWindow();
        t.c274 = t.c278 = t.c27c = t.c280 = GD_clrBarFace();
        t.c284 = t.c288 = t.c28c = t.c290 = GD_clrWindow();
        t.c294 = GD_clrBtnShadow();
        t.c220 = t.c218 = GD_clrBarLight();
        t.pen2b8 = t.c294;
        t.c224 = GD_clrBtnHilite();
    }
    return t;
}

//--------------------------------------------------------------------------
// GDI helpers.  CPen/CBrush/CRgn C++ objects are not constructed here: their
// destructors call CGdiObject::DeleteObject, which exists only as an impl__
// thunk (see the file header rule), so raw handles are used and released.
//--------------------------------------------------------------------------
inline RECT O2003_ToRECT(const CRect& r) { return RECT{ r.left, r.top, r.right, r.bottom }; }

// `CBrush br(clr); ::FillRect(pDC->m_hDC, rect, br);` -- the pattern retail
// inlines (??0CBrush@@QEAA@K@Z at 0x2a1fa0 then the USER32 FillRect import).
void O2003_FillRect(CDC* pDC, const CRect& rect, COLORREF clr)
{
    if (pDC == nullptr || pDC->m_hDC == nullptr) return;
    HBRUSH hbr = ::CreateSolidBrush(clr);
    if (hbr == nullptr) return;
    RECT r = O2003_ToRECT(rect);
    ::FillRect(pDC->m_hDC, &r, hbr);
    ::DeleteObject(hbr);
}

inline void O2003_Draw3dRect(CDC* pDC, const CRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
    RECT r = O2003_ToRECT(rect);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &r, clrTopLeft, clrBottomRight);
}

inline void O2003_FillSolidRect(CDC* pDC, int x, int y, int cx, int cy, COLORREF clr)
{
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, x, y, cx, cy, clr);
}

// CDrawingManager::FillGradient(CRect rect, COLORREF colorStart, COLORREF
// colorFinish, BOOL bHorz, int nStartFlatPercentage, int nEndFlatPercentage)
// -- mfc140 RVA 0x57960.  When CMFCToolBarImages::m_bIsDrawOnGlass is clear it
// tail-calls the worker at 0x5760c directly (0x57986..0x579b5); when set it
// renders through a 32-bit DIB and ORs the alpha channel in (0x579ba..0x57bff).
// The worker is transcribed below; the DIB detour is NOT (it draws the same
// pixels, minus the alpha fix-up that only matters on DWM glass).
//   0x5763a  colorStart == colorFinish -> one FillRect, done
//   0x57679  nStartFlat > 0: bHorz ? top band (colorFinish) : left band (colorStart)
//   0x57739  nEndFlat   > 0: bHorz ? bottom band (colorStart) : right band (colorFinish)
//   0x577f0  nStartFlat + nEndFlat > 100 -> done
//   0x57838  64 bands i = 0..63, colour = ((64-i)*start + i*finish) >> 6 per channel;
//            bHorz: band i spans [bottom - h*(i+1)/64, bottom - h*i/64) -- start at the BOTTOM
//            else : band i spans [left + w*i/64, left + w*(i+1)/64)
// Note the worker mutates its rect argument (the flat bands shrink it).
void O2003_FillGradient(CDC* pDC, CRect rect, COLORREF colorStart, COLORREF colorFinish,
                        int bHorz, int nStartFlat, int nEndFlat)
{
    if (pDC == nullptr || pDC->m_hDC == nullptr) return;
    if (colorStart == colorFinish) { O2003_FillRect(pDC, rect, colorStart); return; }

    if (nStartFlat > 0) {
        CRect r = rect;
        if (bHorz) {
            const int y = rect.top + (rect.bottom - rect.top) * nStartFlat / 100;
            r.bottom = y; rect.top = y;
            O2003_FillRect(pDC, r, colorFinish);
        } else {
            const int x = rect.left + (rect.right - rect.left) * nStartFlat / 100;
            r.right = x; rect.left = x;
            O2003_FillRect(pDC, r, colorStart);
        }
    }
    if (nEndFlat > 0) {
        CRect r = rect;
        if (bHorz) {
            const int y = rect.bottom - (rect.bottom - rect.top) * nEndFlat / 100;
            r.top = y; rect.bottom = y;
            O2003_FillRect(pDC, r, colorStart);
        } else {
            const int x = rect.right - (rect.right - rect.left) * nEndFlat / 100;
            r.left = x; rect.right = x;
            O2003_FillRect(pDC, r, colorFinish);
        }
    }
    if (nStartFlat + nEndFlat > 100) return;

    const int sR = GetRValue(colorStart), sG = GetGValue(colorStart), sB = GetBValue(colorStart);
    const int fR = GetRValue(colorFinish), fG = GetGValue(colorFinish), fB = GetBValue(colorFinish);
    for (int i = 0; i < 64; ++i) {
        const int w = 64 - i;
        const COLORREF clr = RGB(((w * sR + i * fR) >> 6) & 0xff,
                                 ((w * sG + i * fG) >> 6) & 0xff,
                                 ((w * sB + i * fB) >> 6) & 0xff);
        CRect r = rect;
        if (bHorz) {
            const int h = rect.bottom - rect.top;
            r.bottom = rect.bottom - ((h * i) >> 6);
            r.top    = rect.bottom - ((h * (i + 1)) >> 6);
            if (r.bottom - r.top > 0) O2003_FillRect(pDC, r, clr);
        } else {
            const int cx = rect.right - rect.left;
            r.right = rect.left + ((cx * (i + 1)) >> 6);
            r.left  = rect.left + ((cx * i) >> 6);
            if (r.right - r.left > 0) O2003_FillRect(pDC, r, clr);
        }
    }
}

// One pen-drawn line: `CPen* pOld = pDC->SelectObject(&pen); pDC->MoveTo(x1,y1);
// pDC->LineTo(x2,y2); pDC->SelectObject(pOld);` with the CPen replaced by a raw
// ::CreatePen handle (see above).  CDC::SelectObject(CPen*) is the thunk in
// CDC.cpp, which needs a CPen object; ::SelectObject on m_hDC is used instead,
// which is exactly what that thunk does (it selects into m_hDC only and
// never touches m_hAttribDC).
void O2003_PenLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF clr)
{
    if (pDC == nullptr || pDC->m_hDC == nullptr) return;
    HPEN pen = ::CreatePen(PS_SOLID, 1, clr);
    if (pen == nullptr) return;
    HGDIOBJ old = ::SelectObject(pDC->m_hDC, pen);
    impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x1, y1);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, x2, y2);
    ::SelectObject(pDC->m_hDC, old);
    ::DeleteObject(pen);
}

inline int O2003_IsKindOf(const void* pObj, CRuntimeClass* pClass)
{
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pObj), pClass);
}

//--------------------------------------------------------------------------
// CMFCToolBar size helpers used by OnDrawBarGripper.  Both are non-exported
// inlines in retail, read here out of the gripper body and the helper it
// calls; member offsets are the harvested CMFCToolBar layout in
// include/openmfc/afxmfc.h (m_bLocked 0x10b8, m_bLargeIconsAreEnbaled 0x10bc,
// m_sizeButtonLocked 0x1158, m_sizeImageLocked 0x1160, m_sizeCurButtonLocked
// 0x1168, m_sizeCurImageLocked 0x1170).
//--------------------------------------------------------------------------
// GetImageSize(): 0x18febc..0x18feff --
//   m_bLocked ? (m_bLargeIconsAreEnbaled ? m_sizeCurImageLocked : m_sizeImageLocked)
//             : (m_bLargeIconsAreEnbaled ? CMFCToolBar::m_sizeCurImage : CMFCToolBar::m_sizeImage)
inline CSize ToolBar_GetImageSize(const CMFCToolBar* pBar)
{
    if (pBar->m_bLocked)
        return pBar->m_bLargeIconsAreEnbaled ? pBar->m_sizeCurImageLocked : pBar->m_sizeImageLocked;
    return pBar->m_bLargeIconsAreEnbaled ? impl__m_sizeCurImage_CMFCToolBar__1VCSize__A
                                         : impl__m_sizeImage_CMFCToolBar__1VCSize__A;
}
// GetButtonSize(): the helper at mfc140 0x23a74 --
//   sz = m_bLocked ? (large ? m_sizeCurButtonLocked : m_sizeButtonLocked)
//                  : (large ? CMFCToolBar::m_sizeCurButton : CMFCToolBar::m_sizeButton)
//   if (vftable[217]())                       // CMFCToolBar vtable +0x6c8 -> 0x3ae0, `return 1`
//       sz += CMFCVisualManager::GetInstance()->vftable[82]();   // GetButtonExtraBorder
// Slot 82 of the retail Office2003 vftable (0x18031a5c0, mfc140) is 0x8960,
// which returns CSize(0,0), so for this manager the extra border is zero and
// only the base size survives.  (A derived manager overriding
// GetButtonExtraBorder would differ; not modelled.)
inline CSize ToolBar_GetButtonSize(const CMFCToolBar* pBar)
{
    if (pBar->m_bLocked)
        return pBar->m_bLargeIconsAreEnbaled ? pBar->m_sizeCurButtonLocked : pBar->m_sizeButtonLocked;
    return pBar->m_bLargeIconsAreEnbaled ? impl__m_sizeCurButton_CMFCToolBar__1VCSize__A
                                         : impl__m_sizeButton_CMFCToolBar__1VCSize__A;
}

//--------------------------------------------------------------------------
// Bodies of two of this class's own virtuals that other bodies below reach
// through the retail vftable.  OpenMFC's object has a mingw (Itanium) vtable
// with nothing at MSVC slot indices, so -- like CMFCVisualManagerOfficeXP.cpp
// does -- the same body is called directly.  That deviates from retail only
// for a class derived from CMFCVisualManagerOffice2003 that overrides them.
//--------------------------------------------------------------------------

// CMFCVisualManagerOffice2003::OnFillHighlightedArea(CDC*, CRect, CBrush*,
// CMFCToolBarButton*) -- vftable +0x638 (slot 199), mfc140 RVA 0x191a80.
// Low-colour / high-contrast desktops tail-call the OfficeXP body (0x1b1400,
// via 0x191cee).  Otherwise:
//     clr1 = clr2 = -1;  bHorz = 1;  bInPopupBar = 1;
//     if (pButton) {
//         bHorz = pButton->m_bHorz (+0x60);                         0x191b18
//         if (!IsKindOf(CMFCToolBarMenuButton))       bInPopupBar = 0, no vcall   0x191b1e
//         else if (m_pWndParent (+0x80) is a CMFCPopupMenuBar) {                  0x191b41
//             if (pBrush == &this->br1a0 && +0x244 != -1)
//                 { CBrush br(+0x244); ::FillRect; return; }                     0x191b51..0x191b97
//         } else {
//             bInPopupBar = 0;
//             if (pButton->vftable[30]() /* m_pPopupMenu (+0xc0) != NULL */)     0x191baf..0x191bef
//                 clr1 = PixelAlpha(+0x21c, +0x2e8 ? 101 : 120), clr2 = PixelAlpha(+0x220, 110);
//         }
//     } else bInPopupBar = 0;                                                    0x191c1a
//     if (+0x2e8) { three brush-identity cases +0x1a0/+0x1b0/+0x1c0 -> +0x248..+0x25c }   0x191bf5..0x191c8e
//     if (clr1 != -1 && clr2 != -1) FillGradient(rect, clr1, clr2, bHorz, 0, 0);          0x191c94..0x191cdd
//     else CMFCVisualManagerOfficeXP::OnFillHighlightedArea(...)                          0x191d09
// Retail's pBrush identity tests compare against the object's own CBrush
// members; the caller passes those addresses only from inside this class, and
// the two callers in this file (OnDrawBrowseButton, OnDrawComboDropButton) are
// transcribed to pass the SAME identity through the `nBrushId` argument
// (0 = a caller brush, 0x1a0 / 0x1b0 / 0x1c0 = the retail member).  +0x244 is
// -1 on the +0x2e8 == 0 path and every +0x2e8 branch is dead here, so those
// arms reduce to the fall-through.
// The vftable[30] read: CMFCToolBarMenuButton's slot 30 (its vftable at
// 0x180316338, mfc140) is 0x2a580, `return this->[+0xc0] != NULL`.  OpenMFC's
// CMFCToolBarMenuButton (sizeof 296, MSVC layout) keeps only padding there; the
// word is read by offset and is 0 unless something in this tree ever sets it.
void O2003_FillHighlightedArea(CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect,
                               CBrush* pBrush, unsigned nBrushId, CMFCToolBarButton* pButton)
{
    if (!Office2003_RichColorDesktop()) {
        impl__OnFillHighlightedArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
            pThis, pDC, rect, pBrush, pButton);
        return;
    }
    const O2003Theme t = O2003_Theme(pThis);
    COLORREF clr1 = (COLORREF)-1, clr2 = (COLORREF)-1;
    int bHorz = 1;
    if (pButton != nullptr) {
        bHorz = pButton->m_bHorz;
        if (O2003_IsKindOf(pButton, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ())) {
            CWnd* pParent = pButton->m_pWndParent;
            if (pParent != nullptr &&
                O2003_IsKindOf(pParent, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ())) {
                if (nBrushId == 0x1a0 && t.c244 != (COLORREF)-1) {
                    O2003_FillRect(pDC, rect, t.c244);
                    return;
                }
            } else {
                const void* pPopup = *reinterpret_cast<void* const*>(
                    reinterpret_cast<const char*>(pButton) + 0xc0);
                if (pPopup != nullptr) {
                    clr1 = DM_PixelAlpha(t.c21c, t.bIsStandardWinXPTheme ? 0x65 : 0x78);
                    clr2 = DM_PixelAlpha(t.c220, 0x6e);
                }
            }
        }
    }
    // The +0x2e8 brush-identity arms (0x191bf5..0x191c8e) are unreachable here.
    if (clr1 != (COLORREF)-1 && clr2 != (COLORREF)-1) {
        O2003_FillGradient(pDC, rect, clr1, clr2, bHorz, 0, 0);
        return;
    }
    impl__OnFillHighlightedArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
        pThis, pDC, rect, pBrush, pButton);
}

// CMFCVisualManagerOffice2003::OnDrawBarGripper -- vftable +0x098 (slot 19),
// mfc140 RVA 0x18fdc0; the exported thunk below forwards here.
void O2003_DrawBarGripper(CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectGripper,
                          int bHorz, CBasePane* pBar);

} // namespace




//=============================================================================
// CMFCVisualManagerOffice2003 - Ribbon drawing overrides
//=============================================================================
















//=============================================================================
// CMFCVisualManagerOffice2003 - Erase/fill overrides with no base-class
// virtual declared in the OpenMFC headers (conservative no-ops)
//=============================================================================















// Symbol: ?OnDrawRibbonSliderChannel@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderChannel_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // slider channel through the ribbon theme globals; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderChannel(pDC, pSlider, rect);
}
// Symbol: ?OnDrawRibbonSliderThumb@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderThumb_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect,
    int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // slider thumb glyph; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderThumb(pDC, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled);
}
// Symbol: ?OnDrawRibbonSliderZoomButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSlider@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonSliderZoomButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSlider__VCRect__HHHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSlider* pSlider, CRect rect,
    int bIsZoomOut, int bIsHighlighted, int bIsPressed, int bIsDisabled)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the themed
    // slider zoom-button glyph; delegate to base.
    pThis->CMFCVisualManager::OnDrawRibbonSliderZoomButton(pDC, pSlider, rect, bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
}
// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonStatusBar* pBar, CMFCRibbonStatusBarPane* pPane)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail fills the pane with
    // the Office2003 status-bar theme and returns the text color; delegate to base.
    return (unsigned long)pThis->CMFCVisualManager::OnDrawRibbonStatusBarPane(pDC, pBar, pPane);
}
// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEBVCRect__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, const CRect& rect,
    int nBorderSize, int iImage, int bHilited)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the Office2003
    // scroll-button theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawScrollButtons(pDC, rect, nBorderSize, iImage, bHilited);
}
// Symbol: ?OnDrawSeparator@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // separator groove; delegate to base.
    pThis->CMFCVisualManager::OnDrawSeparator(pDC, pBar, rect, bHorz);
}
// Symbol: ?OnDrawShowAllMenuItems@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawShowAllMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail fills the "show all
    // menu items" bar with the Office2003 theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawShowAllMenuItems(pDC, rect, state);
}
// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectPane,
    unsigned int uiID, unsigned int nStyle)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the Office2003
    // status-bar pane bevels; delegate to base.
    pThis->CMFCVisualManager::OnDrawStatusBarPaneBorder(pDC, pBar, rectPane, uiID, nStyle);
}
// Symbol: ?OnDrawStatusBarSizeBox@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawStatusBarSizeBox_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCStatusBar* pBar, CRect rectSizeBox)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // status-bar sizing grip; delegate to base.
    pThis->CMFCVisualManager::OnDrawStatusBarSizeBox(pDC, pBar, rectSizeBox);
}
// Symbol: ?OnDrawTab@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectTab, int iTab, int bIsActive,
    const CMFCBaseTabCtrl* pTabWnd)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // tab button theme; delegate to base.
    pThis->CMFCVisualManager::OnDrawTab(pDC, rectTab, iTab, bIsActive, pTabWnd);
}
// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect& rect, CMFCButton* pButton,
    unsigned int uiState, CMFCBaseTabCtrl* pWndTab)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail draws the Office2003
    // tab-button border; delegate to base.
    pThis->CMFCVisualManager::OnDrawTabsButtonBorder(pDC, rect, pButton, uiState, pWndTab);
}
// Symbol: ?OnDrawTask@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons,
    int bIsHighlighted, int bIsSelected)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the task row
    // (icon + hot-tracking background) for Office2003; delegate to base.
    pThis->CMFCVisualManager::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
}
// Symbol: ?OnDrawTasksGroupAreaBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bSpecial, int bNoTitle)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail frames the tasks-pane
    // group area with the Office2003 border; delegate to base.
    pThis->CMFCVisualManager::OnDrawTasksGroupAreaBorder(pDC, rect, bSpecial, bNoTitle);
}
// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCTasksPaneTaskGroup* pGroup,
    int bIsHighlighted, int bIsSelected, int bCanCollapse)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail paints the Office2003
    // group caption bar; delegate to base.
    pThis->CMFCVisualManager::OnDrawTasksGroupCaption(pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
}
// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectCaption, int bIsActive)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail fills the tear-off
    // caption with the Office2003 active/inactive color; delegate to base.
    pThis->CMFCVisualManager::OnDrawTearOffCaption(pDC, rectCaption, bIsActive);
}
// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, CMFCDesktopAlertWndButton* /*pButton*/)
{
    // Retail erases the popup-window button area with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rectArea*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    // Retail erases the tab area with the Office2003 theme color. No base-class
    // virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, CMFCButton* /*pButton*/, CMFCBaseTabCtrl* /*pWndTab*/)
{
    // Retail erases a single tab button before redrawing it with the
    // Office2003 theme. No base-class virtual is declared in the headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnEraseTabsFrame@CMFCVisualManagerOffice2003@@UEAAHPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI impl__OnEraseTabsFrame_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    // Retail fills the frame around the tabs and reports whether it painted.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially
    return 0;
}
// Symbol: ?OnFillAutoHideButtonBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnFillAutoHideButtonBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, void* /*pButton*/ /* CMFCAutoHideButton* -- not declared in headers */)
{
    // Retail fills the auto-hide tab button background with the Office2003
    // theme. No base-class virtual and no CMFCAutoHideButton declaration
    // exist in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillBarBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail fills the bar with the
    // Office2003 gradient background; delegate to base.
    pThis->CMFCVisualManager::OnFillBarBackground(pDC, pBar, rectClient, rectClip, bNCArea);
}
// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CMFCToolBarButton* /*pButton*/, CRect /*rect*/, CMFCVisualManager::AFX_BUTTON_STATE /*state*/)
{
    // Retail fills the toolbar-button interior with the Office2003 state
    // gradient. No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bIsSelected*/)
{
    // Retail fills the customize commands list with the Office2003 theme and
    // returns the text color. No base-class virtual is declared.
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially
    return 0;
}
// Symbol: ?OnFillHeaderCtrlBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillHeaderCtrlBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CMFCHeaderCtrl* /*pHeaderCtrl*/, CDC* /*pDC*/, CRect /*rect*/)
{
    // Retail fills the header-control background with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// CMFCVisualManagerOffice2003::OnFillHighlightedArea -- retail mfc140 RVA
// 0x191a80, transcribed in O2003_FillHighlightedArea() above (the same body
// the in-file callers reach through retail's vftable slot 199).  An external
// caller can never pass one of the object's own +0x1a0/+0x1b0/+0x1c0 CBrush
// members (OpenMFC has no storage for them), so the brush identity is 0 here.
// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton)
{
    if (pThis == nullptr || pDC == nullptr) return;
    O2003_FillHighlightedArea(pThis, pDC, rect, pBrush, 0, pButton);
}
// Symbol: ?OnFillOutlookBarCaption@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookBarCaption_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__AEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rectCaption*/, unsigned long& /*clrText*/)
{
    // Retail fills the Outlook bar caption with the Office2003 gradient and
    // returns the caption text color through the reference. No base-class
    // virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillOutlookPageButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEBVCRect@@HHAEAK@Z
extern "C" void MS_ABI impl__OnFillOutlookPageButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEBVCRect__HHAEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, const CRect& /*rect*/, int /*bIsHighlighted*/, int /*bIsPressed*/, unsigned long& /*clrText*/)
{
    // Retail fills an Outlook page navigation button with the Office2003
    // state gradient and reports its text color. No base-class virtual is
    // declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    // Retail fills the popup-window background with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillTab@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rectFill*/, CBrush* /*pBrush*/, int /*iTab*/, int /*bIsActive*/, const CMFCBaseTabCtrl* /*pTabWnd*/)
{
    // Retail fills the active/inactive tab body with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* /*pDC*/, CRect /*rect*/, int /*bSpecial*/)
{
    // Retail fills the tasks-group interior with the Office2003 theme.
    // No base-class virtual is declared in the OpenMFC headers.
    if (!pThis) return;
    // TODO(clean-room): transcribed partially
}
// Symbol: ?CreateObject@CMFCVisualManagerOffice2003@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOffice2003__SAPEAVCObject__XZ() { return new CMFCVisualManagerOffice2003(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOffice2003@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOffice2003@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOffice2003__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOffice2003* pThis) { return CMFCVisualManagerOffice2003::GetThisClass(); }
// Symbol: ?OnDrawRibbonProgressBar@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonProgressBar@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnDrawRibbonProgressBar_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonProgressBar__VCRect__2H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonProgressBar* pProgress,
    CRect rectProgress, CRect rectChunk, int bInfiniteMode) {
    pThis->OnDrawRibbonProgressBar(pDC, pProgress, rectProgress, rectChunk, bInfiniteMode);
}
// Symbol: ?OnDrawStatusBarProgress@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@HHKKKH@Z
extern "C" void MS_ABI impl__OnDrawStatusBarProgress_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__HHKKKH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCStatusBar* pBar,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText, int bProgressText) {
    pThis->OnDrawStatusBarProgress(pDC, pBar, rectProgress, nProgressTotal, nProgressCurr,
                                   clrBar, clrProgressBarDest, clrProgressText, bProgressText);
}
CMFCVisualManagerOffice2003::CMFCVisualManagerOffice2003() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA = TRUE;
    impl__m_bStatusBarOfficeXPLook_CMFCVisualManagerOffice2003__1HA = TRUE;
    impl__m_bUseGlobalTheme_CMFCVisualManagerOffice2003__1HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOffice2003::~CMFCVisualManagerOffice2003() {}
CObject* CMFCVisualManagerOffice2003::CreateObject() { return new CMFCVisualManagerOffice2003(); }
// CMFCVisualManagerOffice2003::DrawCustomizeButton(CDC* pDC, CRect rect,
//         BOOL bIsHorz, AFX_BUTTON_STATE state, BOOL bIsCustomize, BOOL bIsMoreButtons)
// -- retail mfc140 RVA 0x18ef10, complete body:
//     clrStart  = state ? +0x24c : +0x230;   clrFinish = state ? +0x248 : +0x234;   // 0x18ef43..0x18ef5f
//     POINT pts[6] = bIsHorz                                                        // 0x18ef8b..0x18efd6
//         ? { (l,t), (l+2,t+1), (l+3,b-3), (l,b), (r,b), (r,t) }
//         : { (l,t), (l+3,t+2), (r-3,t+3), (r,t), (r,b), (l,b) };
//     CRgn rgn; rgn.Attach(::CreatePolygonRgn(pts, 6, WINDING));                    // 0x18eff5 (GDI32 import)
//     pDC->SelectClipRgn(&rgn);                                                     // 0x18f00e (CDC::SelectClipRgn(CRgn*), 0x2a0d60)
//     CDrawingManager(*pDC).FillGradient(rect, clrStart, clrFinish, bIsHorz, 0, 0); // 0x18f047
//     if (bIsHorz) { ::InflateRect(rect, 0, -2); rect.left += 2; }                  // 0x18f05a/0x18f060
//     else         { ::InflateRect(rect, -2, 0); rect.top  += 2; }                  // 0x18f06c/0x18f072
//     nMargin = this->vftable[188]();                                               // 0x18f07c: +0x5e0 -> 0x3ae0, `return 1`
//     CSize szImage = CMenuImages::Size();                                          // 0x18f091 (0x8fbb0)
//     if (CMFCToolBar::m_bLargeIcons) szImage *= 2;                                 // 0x18f096..0x18f0b1
//     if (bIsCustomize) {                                                           // 0x18f0bf
//         CRect ri = rect;
//         if (bIsHorz) ri.top  = rect.bottom - 2*nMargin - szImage.cy;              // 0x18f0e5..0x18f0f1
//         else       { ri.top  = rect.top + 1; ri.left = rect.right - 2*nMargin - szImage.cx; }  // 0x18f0f6..0x18f102
//         ri.InflateRect(-(ri.Width() - szImage.cx)/2, -(ri.Height() - szImage.cy)/2);       // 0x18f108..0x18f135
//         ri.OffsetRect(1, 1);  CMenuImages::Draw(pDC, bIsHorz ? 29 : 30, ri, 3 /*ImageWhite*/, szImage);   // 0x18f147/0x18f16e
//         ri.OffsetRect(-1,-1); CMenuImages::Draw(pDC, bIsHorz ? 29 : 30, ri, 0 /*ImageBlack*/, szImage);   // 0x18f17f/0x18f19c
//     }
//     if (bIsMoreButtons) {                                                         // 0x18f1a1
//         CRect ri = rect;
//         if (bIsHorz) ri.bottom = rect.top + 2*nMargin + szImage.cy;               // 0x18f1c4..0x18f1d1
//         else       { ri.right  = rect.left + 2*nMargin + szImage.cx; ri.top = rect.top + 1; }  // 0x18f1dd..0x18f1f2
//         (same inflate / two Draw calls, ids bIsHorz ? 31 : 32)                    // 0x18f1f8..0x18f282
//     }
//     pDC->SelectClipRgn(NULL);                                                     // 0x18f28c
// The CMenuImages::Draw overload used (0x8fc50) takes a CPoint& -- the rect's
// top-left.  Ids 29..32 are the retail IMAGES_IDS values passed; OpenMFC's
// CMenuImages enum only declares IdMin/IdMax, so they are written numerically.
// The (int)/2 divisions are signed (`cltd; sub; sar 1`).
// DEVIATIONS: CMenuImages::Size (0x8fbb0) is a stub in this tree that never
// writes its CSize return buffer, so the 9x9 edge it yields at 100% scaling
// (see GetShowAllMenuItemsHeight above) is used directly; CMenuImages::Draw is
// likewise a no-op stub today, so the glyphs do not appear until it is
// implemented.  The clip region and gradient are drawn with raw GDI handles
// (CRgn's destructor would call a CGdiObject method that is thunk-only).
// Symbol: ?DrawCustomizeButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HW4AFX_BUTTON_STATE@CMFCVisualManager@@HH@Z
extern "C" void MS_ABI impl__DrawCustomizeButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HW4AFX_BUTTON_STATE_CMFCVisualManager__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bIsHorz,
    CMFCVisualManager::AFX_BUTTON_STATE state, int bIsCustomize, int bIsMoreButtons)
{
    if (pThis == nullptr || pDC == nullptr || pDC->m_hDC == nullptr) return;
    const O2003Theme t = O2003_Theme(pThis);
    const COLORREF clrStart  = (state != 0) ? t.c24c : t.c230;
    const COLORREF clrFinish = (state != 0) ? t.c248 : t.c234;

    POINT pts[6];
    if (bIsHorz) {
        pts[0] = { rect.left,     rect.top };
        pts[1] = { rect.left + 2, rect.top + 1 };
        pts[2] = { rect.left + 3, rect.bottom - 3 };
        pts[3] = { rect.left,     rect.bottom };
        pts[4] = { rect.right,    rect.bottom };
        pts[5] = { rect.right,    rect.top };
    } else {
        pts[0] = { rect.left,      rect.top };
        pts[1] = { rect.left + 3,  rect.top + 2 };
        pts[2] = { rect.right - 3, rect.top + 3 };
        pts[3] = { rect.right,     rect.top };
        pts[4] = { rect.right,     rect.bottom };
        pts[5] = { rect.left,      rect.bottom };
    }
    HRGN hrgn = ::CreatePolygonRgn(pts, 6, WINDING);
    ::SelectClipRgn(pDC->m_hDC, hrgn);

    O2003_FillGradient(pDC, rect, clrStart, clrFinish, bIsHorz, 0, 0);

    if (bIsHorz) { rect.InflateRect(0, -2); rect.left += 2; }
    else         { rect.InflateRect(-2, 0); rect.top  += 2; }

    const int nMargin = 1;                          // vftable[188] -> `return 1`
    CSize szImage(9, 9);                            // CMenuImages::Size() at 100% scaling
    if (impl__m_bLargeIcons_CMFCToolBar__1HA) { szImage.cx *= 2; szImage.cy *= 2; }

    if (bIsCustomize) {
        CRect ri = rect;
        if (bIsHorz) ri.top = rect.bottom - 2 * nMargin - szImage.cy;
        else       { ri.top = rect.top + 1; ri.left = rect.right - 2 * nMargin - szImage.cx; }
        ri.InflateRect(-((ri.right - ri.left - szImage.cx) / 2), -((ri.bottom - ri.top - szImage.cy) / 2));
        const int id = bIsHorz ? 29 : 30;
        ri.OffsetRect(1, 1);
        impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(pDC, id, ri.TopLeft(), 3, szImage);
        ri.OffsetRect(-1, -1);
        impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(pDC, id, ri.TopLeft(), 0, szImage);
    }
    if (bIsMoreButtons) {
        CRect ri = rect;
        if (bIsHorz) ri.bottom = rect.top + 2 * nMargin + szImage.cy;
        else       { ri.right = rect.left + 2 * nMargin + szImage.cx; ri.top = rect.top + 1; }
        ri.InflateRect(-((ri.right - ri.left - szImage.cx) / 2), -((ri.bottom - ri.top - szImage.cy) / 2));
        const int id = bIsHorz ? 31 : 32;
        ri.OffsetRect(1, 1);
        impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(pDC, id, ri.TopLeft(), 3, szImage);
        ri.OffsetRect(-1, -1);
        impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(pDC, id, ri.TopLeft(), 0, szImage);
    }

    ::SelectClipRgn(pDC->m_hDC, nullptr);
    if (hrgn != nullptr) ::DeleteObject(hrgn);
}

// CMFCVisualManagerOffice2003::GetBaseThemeColor() -- retail mfc140 RVA
// 0x195940:
//     if (this->[+0x2e8] != 0 && this->[+0x08] != NULL)          // 0x195944/0x19594d
//         return ::GetThemeSysColor(this->[+0x08], 0x1d);         // 0x19595f
//     afxGlobalData.Initialize();
//     return afxGlobalData.clrBarFace (+0x60);                    // 0x195985
// 0x1d is 29 == COLOR_MENUHILIGHT, and the tail-jump slot at 0x1802c5408
// resolves to UxTheme.dll!GetThemeSysColor.  this->[+0x08] is the class's
// HTHEME -- it is the same member GetWindowColor (0x195450) hands to
// GetThemeSysColor.  [+0x2e8] is set by OnUpdateSystemColors at 0x190be0 to
// (unsigned)(GetStandardWindowsTheme() - 1) <= 2 (the `dec/cmp/setbe` sequence
// at 0x190bcc..0x190be0), i.e. "one of the three standard XP colour schemes is
// active".  OpenMFC models neither member (see the file header), so only the
// non-themed terminal is reproducible; that is the branch retail takes whenever
// visual styles are off.
// Symbol: ?GetBaseThemeColor@CMFCVisualManagerOffice2003@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetBaseThemeColor_CMFCVisualManagerOffice2003__UEAAKXZ(
    CMFCVisualManagerOffice2003* pThis)
{
    if (pThis == nullptr) return 0;
    return GD_clrBarFace();
}

// CMFCVisualManagerOffice2003::GetPropertyGridGroupColor(CMFCPropertyGridCtrl*)
// -- retail mfc140 RVA 0x195ff0 is a single `jmp 0x180189d10`, i.e. an
// unconditional tail-call to CMFCVisualManager::GetPropertyGridGroupColor.
// Symbol: ?GetPropertyGridGroupColor@CMFCVisualManagerOffice2003@@UEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManagerOffice2003__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CMFCPropertyGridCtrl* pGrid)
{
    return impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(pThis, pGrid);
}

// CMFCVisualManagerOffice2003::GetPropertyGridGroupTextColor(CMFCPropertyGridCtrl*)
// -- retail mfc140 RVA 0x196000 is a single `jmp 0x180189d70`, i.e. an
// unconditional tail-call to CMFCVisualManager::GetPropertyGridGroupTextColor.
// Symbol: ?GetPropertyGridGroupTextColor@CMFCVisualManagerOffice2003@@UEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupTextColor_CMFCVisualManagerOffice2003__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManagerOffice2003* pThis, CMFCPropertyGridCtrl* pGrid)
{
    return impl__GetPropertyGridGroupTextColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(pThis, pGrid);
}

// CMFCVisualManagerOffice2003::GetShowAllMenuItemsHeight(CDC*, const CSize&)
// -- retail mfc140 RVA 0x191e80, complete body:
//     CSize sz = CMenuImages::Size();   // call at 0x191e89
//     return sz.cy + 10;                // 0x191e8e / 0x191e91
// Neither pDC nor sizeDefault is read.  CMenuImages::Size (mfc140 RVA 0x8fbb0)
// returns a square CSize whose edge is 9 scaled by afxGlobalData's DPI ratio:
// it starts from 9 (0x8fbcd), takes 1.0 unless afxGlobalData[+0x2c0] is set, in
// which case the ratio at afxGlobalData[+0x2b8], and only when that ratio is
// not 1.0 recomputes the edge as (int)(ratio * 9.0 + 0.5) (0x8fc10..0x8fc20).
// At the default 100% scaling the edge is therefore 9 and this returns 19.
// OpenMFC cannot call its own CMenuImages::Size thunk here: that thunk
// (phase4/src/featurepack/controls/CMFCReBar.cpp) is a stub that returns
// nullptr and never writes the caller's CSize return buffer, so the result
// would be uninitialised stack.
// Symbol: ?GetShowAllMenuItemsHeight@CMFCVisualManagerOffice2003@@UEAAHPEAVCDC@@AEBVCSize@@@Z
extern "C" int MS_ABI impl__GetShowAllMenuItemsHeight_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__AEBVCSize___Z(
    CMFCVisualManagerOffice2003* /*pThis*/, CDC* /*pDC*/, const CSize& /*sizeDefault*/)
{
    const int nMenuImageEdge = 9;   // CMenuImages::Size().cy at 100% scaling
    return nMenuImageEdge + 10;
}

// CMFCVisualManagerOffice2003::GetSmartDockingBaseGuideColors(COLORREF&, COLORREF&)
// -- retail mfc140 RVA 0x195760, complete body:
//     afxGlobalData.Initialize();
//     if (bpp > 8 && !blackHC && !whiteHC) {          // 0x195791..0x1957c5
//         clrBaseGroupBackground = 0x00e4e4e4;        // 0x1957c7
//         clrBaseGroupBorder     = 0x00b5b5b5;        // 0x1957cd, stored at 0x19581b
//     } else {
//         clrBaseGroupBackground = afxGlobalData.clrBarFace   (+0x60);  // 0x1957ee
//         clrBaseGroupBorder     = afxGlobalData.clrBarShadow (+0x64);  // 0x195815
//     }
// `this` is not read.  Both branches are reproduced exactly here.
// Symbol: ?GetSmartDockingBaseGuideColors@CMFCVisualManagerOffice2003@@UEAAXAEAK0@Z
extern "C" void MS_ABI impl__GetSmartDockingBaseGuideColors_CMFCVisualManagerOffice2003__UEAAXAEAK0_Z(
    CMFCVisualManagerOffice2003* /*pThis*/, unsigned long& clrBaseGroupBackground,
    unsigned long& clrBaseGroupBorder)
{
    if (Office2003_RichColorDesktop()) {
        clrBaseGroupBackground = 0x00e4e4e4u;   // RGB(228, 228, 228)
        clrBaseGroupBorder     = 0x00b5b5b5u;   // RGB(181, 181, 181)
        return;
    }
    clrBaseGroupBackground = GD_clrBarFace();
    clrBaseGroupBorder     = GD_clrBarShadow();
}

// CMFCVisualManagerOffice2003::GetSmartDockingHighlightToneColor() -- retail
// mfc140 RVA 0x195830, complete body:
//     afxGlobalData.Initialize();
//     if (bpp > 8 && !blackHC && !whiteHC) {                 // 0x19585a..0x19588d
//         switch (this->vftable[12]()) {                     // 0x195895/0x195899
//         case 1: return 0x00f17b3d;                         // 0x1958c4
//         case 2: return 0x006d92be;                         // 0x1958b9
//         case 3: return 0x00a98286;                         // 0x1958ae
//         }
//     }
//     return CMFCVisualManager::GetSmartDockingHighlightToneColor();  // 0x1958d7
// (the three cases are the `sub $1/je`, `sub $1/je`, `cmp $1/jne` chain at
// 0x19589f..0x1958ac; anything else falls through to the tail-call.)
// Vftable slot 12 is byte offset 0x60 of the retail CMFCVisualManagerOffice2003
// vftable at image address 0x18031a5c0 (mfc140), which holds
// CMFCBaseVisualManager::GetStandardWindowsTheme (0x18e790) -- the class does
// not override it.  That function returns 1 for the "normalcolor" (Luna Blue)
// scheme, 2 for "homestead" (Olive) and 3 for "metallic" (Silver) -- the string
// compares at 0x18e91c / 0x18e936 / 0x18e954 against the literals at
// 0x33f100 / 0x33f110 / 0x33f120.  It returns 0 when a visual style is active
// but is none of those (0x18e9da) and -1 when the theme name could not be read
// at all (0x18e81f); the "metallic" branch also collapses to 0 when the theme
// path contains "royale" (the literal at 0x33f12c).
// OpenMFC has no vtable to dispatch
// through here, so the sibling impl__ thunk is called directly; that is
// equivalent for this class because slot 12 is not overridden anywhere in the
// Office2003 chain.  The OpenMFC thunk currently returns 0, so today this
// always reaches the base-class tail-call -- which is the retail behaviour for
// a desktop with no standard XP colour scheme.
// Symbol: ?GetSmartDockingHighlightToneColor@CMFCVisualManagerOffice2003@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetSmartDockingHighlightToneColor_CMFCVisualManagerOffice2003__UEAAKXZ(
    CMFCVisualManagerOffice2003* pThis)
{
    if (pThis != nullptr && Office2003_RichColorDesktop()) {
        switch (impl__GetStandardWindowsTheme_CMFCBaseVisualManager__UEAA_AW4WinXpTheme_1_XZ(pThis)) {
        case 1: return 0x00f17b3du;   // "normalcolor" (Luna Blue)
        case 2: return 0x006d92beu;   // "homestead"   (Olive)
        case 3: return 0x00a98286u;   // "metallic"    (Silver)
        default: break;
        }
    }
    return impl__GetSmartDockingHighlightToneColor_CMFCVisualManager__UEAAKXZ(pThis);
}

// CMFCVisualManagerOffice2003::GetTabFrameColors(const CMFCBaseTabCtrl*,
//         COLORREF& clrDark, COLORREF& clrBlack, COLORREF& clrHighlight,
//         COLORREF& clrFace, COLORREF& clrDarkShadow, COLORREF& clrLight,
//         CBrush*& pbrFace, CBrush*& pbrBlack) -- retail mfc140 RVA 0x193a90:
//     CMFCVisualManager::GetTabFrameColors(pTabWnd, ...all eight...);   // 0x193af7 (0x188fe0)
//     if (pTabWnd->vftable[164]()) return;                              // 0x193b02 (+0x520)
//     if (bpp <= 8 || blackHC || whiteHC) return;                       // 0x193b38..0x193b73
//     if (pTabWnd->[+0x1fc]) return;                                    // 0x193b79
//     if (!this->[+0x2e8]) return;                                      // 0x193b86
//     ... standard-XP-theme overrides of the six colours from afxGlobalData,
//         pbrFace from pTabWnd->vftable[133]() (0x193b9b..0x193cd2) ...
// Every path after the base call either returns or requires +0x2e8, which is
// FALSE in this tree (see the theme-block comment), so the reachable body is
// the base-class call.  The +0x2e8 tail is not transcribed.
// Symbol: ?GetTabFrameColors@CMFCVisualManagerOffice2003@@UEAAXPEBVCMFCBaseTabCtrl@@AEAK11111AEAPEAVCBrush@@2@Z
extern "C" void MS_ABI impl__GetTabFrameColors_CMFCVisualManagerOffice2003__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
    CMFCVisualManagerOffice2003* pThis, const CMFCBaseTabCtrl* pTabWnd, unsigned long& clrDark,
    unsigned long& clrBlack, unsigned long& clrHighlight, unsigned long& clrFace,
    unsigned long& clrDarkShadow, unsigned long& clrLight, CBrush*& pbrFace, CBrush*& pbrBlack)
{
    impl__GetTabFrameColors_CMFCVisualManager__UEAAXPEBVCMFCBaseTabCtrl__AEAK11111AEAPEAVCBrush__2_Z(
        pThis, pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight, pbrFace, pbrBlack);
    // +0x2e8 (standard XP theme) is never set in this tree: nothing more to do.
}

// CMFCVisualManagerOffice2003::GetThemeColor(void* hTheme, int nColorId) const
// -- retail mfc140 RVA 0x192470, complete body:
//     if (hTheme != NULL) return ::GetThemeSysColor(hTheme, nColorId);  // 0x19247e
//     return ::GetSysColor(nColorId);                                   // 0x192488
// Both tail-calls were resolved through the import table: the slot at
// 0x1802c5408 is UxTheme.dll!GetThemeSysColor and 0x1802c4dc0 is
// USER32.dll!GetSysColor.  `this` is not read.  This is an exact transcription.
// Symbol: ?GetThemeColor@CMFCVisualManagerOffice2003@@IEBAKPEAXH@Z
extern "C" unsigned long MS_ABI impl__GetThemeColor_CMFCVisualManagerOffice2003__IEBAKPEAXH_Z(
    const CMFCVisualManagerOffice2003* /*pThis*/, void* hTheme, int nColorId)
{
    if (hTheme != nullptr) {
        return ::GetThemeSysColor(static_cast<HTHEME>(hTheme), nColorId);
    }
    return ::GetSysColor(nColorId);
}

// CMFCVisualManagerOffice2003::GetToolTipInfo(CMFCToolTipInfo& params, UINT nType)
// -- retail mfc140 RVA 0x197090, complete body.  `this` and nType are never
// read; the function only fills the struct and returns TRUE on both paths
// (`mov %edi,%eax` at 0x1971cc with edi == 1 since 0x1970af):
//
//   afxGlobalData.Initialize();
//   if (bpp > 8 && !blackHC && !whiteHC) {                 // 0x1970c8..0x1970fb
//       m_bBalloonTooltip  (+0x10) = TRUE;                 // 0x197101
//       m_bDrawIcon        (+0x08) = TRUE;                 // 0x197104
//       m_bDrawDescription (+0x04) = TRUE;                 // 0x197107
//       m_bRoundedCorners  (+0x0c) = TRUE;                 // 0x19710a
//       m_bDrawSeparator   (+0x14) = FALSE;                // 0x19710d
//       m_clrFill          (+0x24) = afxGlobalData.clrBarHilite (+0x68);  // 0x197131
//       m_clrFillGradient  (+0x28) = afxGlobalData.clrBarFace   (+0x60);  // 0x197151
//       m_clrText          (+0x2c) = afxGlobalData.clrBarText   (+0x74);  // 0x197171
//       m_clrBorder        (+0x30) = afxGlobalData.clrBarShadow (+0x64);  // 0x197191
//       // m_bBoldLabel, m_bVislManagerTheme, m_nMaxDescrWidth and
//       // m_nGradientAngle are deliberately left untouched on this path.
//   } else {                                               // 0x197196
//       m_bBoldLabel       (+0x00) = FALSE;
//       m_clrFill/m_clrFillGradient/m_clrText/m_clrBorder = (COLORREF)-1;
//       m_bDrawDescription (+0x04) = TRUE;
//       // one 64-bit store of 1 at +0x08 sets m_bDrawIcon TRUE and
//       // m_bRoundedCorners FALSE (0x1971ae);
//       m_bBalloonTooltip  (+0x10) = TRUE;
//       // one 64-bit store of 1 at +0x14 sets m_bDrawSeparator TRUE and
//       // m_bVislManagerTheme FALSE (0x1971b5);
//       m_nMaxDescrWidth   (+0x1c) = 200;
//       m_nGradientAngle   (+0x20) = -1;
//   }
//   return TRUE;
//
// OpenMFC declares CMFCToolTipInfo as an opaque 96-byte blob, so the fields are
// written by offset -- see kTTI_* above.
// Symbol: ?GetToolTipInfo@CMFCVisualManagerOffice2003@@UEAAHAEAVCMFCToolTipInfo@@I@Z
extern "C" int MS_ABI impl__GetToolTipInfo_CMFCVisualManagerOffice2003__UEAAHAEAVCMFCToolTipInfo__I_Z(
    CMFCVisualManagerOffice2003* /*pThis*/, CMFCToolTipInfo& params, unsigned int /*nType*/)
{
    void* p = &params;

    if (Office2003_RichColorDesktop()) {
        TTI_SetInt(p, kTTI_bBalloonTooltip,  TRUE);
        TTI_SetInt(p, kTTI_bDrawIcon,        TRUE);
        TTI_SetInt(p, kTTI_bDrawDescription, TRUE);
        TTI_SetInt(p, kTTI_bRoundedCorners,  TRUE);
        TTI_SetInt(p, kTTI_bDrawSeparator,   FALSE);
        TTI_SetInt(p, kTTI_clrFill,          static_cast<int>(GD_clrBarHilite()));
        TTI_SetInt(p, kTTI_clrFillGradient,  static_cast<int>(GD_clrBarFace()));
        TTI_SetInt(p, kTTI_clrText,          static_cast<int>(GD_clrBarText()));
        TTI_SetInt(p, kTTI_clrBorder,        static_cast<int>(GD_clrBarShadow()));
        return TRUE;
    }

    TTI_SetInt(p, kTTI_bBoldLabel,        FALSE);
    TTI_SetInt(p, kTTI_clrFill,           -1);
    TTI_SetInt(p, kTTI_clrFillGradient,   -1);
    TTI_SetInt(p, kTTI_clrText,           -1);
    TTI_SetInt(p, kTTI_clrBorder,         -1);
    TTI_SetInt(p, kTTI_bDrawDescription,  TRUE);
    TTI_SetInt(p, kTTI_bDrawIcon,         TRUE);
    TTI_SetInt(p, kTTI_bRoundedCorners,   FALSE);
    TTI_SetInt(p, kTTI_bBalloonTooltip,   TRUE);
    TTI_SetInt(p, kTTI_bDrawSeparator,    TRUE);
    TTI_SetInt(p, kTTI_bVislManagerTheme, FALSE);
    TTI_SetInt(p, kTTI_nMaxDescrWidth,    200);
    TTI_SetInt(p, kTTI_nGradientAngle,    -1);
    return TRUE;
}

// CMFCVisualManagerOffice2003::GetWindowColor() const -- retail mfc140 RVA
// 0x195450, complete body:
//     HTHEME h = this->[+0x08];                                 // 0x195450
//     if (h != NULL) return ::GetThemeSysColor(h, COLOR_WINDOW); // 0x19545e
//     return ::GetSysColor(COLOR_WINDOW);                        // 0x19546a
// (both tail-call slots resolved through the import table: 0x1802c5408 is
// UxTheme!GetThemeSysColor, 0x1802c4dc0 is USER32!GetSysColor; the immediate is
// 5 == COLOR_WINDOW on both.)  The HTHEME member at +0x08 has no storage in
// OpenMFC (see the file header), so only the NULL-theme terminal is
// reproducible -- the branch retail takes with visual styles off.
// Symbol: ?GetWindowColor@CMFCVisualManagerOffice2003@@MEBAKXZ
extern "C" unsigned long MS_ABI impl__GetWindowColor_CMFCVisualManagerOffice2003__MEBAKXZ(
    const CMFCVisualManagerOffice2003* /*pThis*/)
{
    return GD_clrWindow();
}

// CMFCVisualManagerOffice2003::IsToolbarRoundShape(CMFCToolBar* pToolBar)
// -- retail mfc140 RVA 0x18f2d0, complete body:
//     return pToolBar->IsKindOf(RUNTIME_CLASS(CMFCMenuBar)) == 0;
// The call at 0x18f2de is CObject::IsKindOf (0x233310) and the descriptor
// passed in %rdx is the static at image address 0x1803aa268 (mfc140), whose
// m_lpszClassName points at the string "CMFCMenuBar".  The `sete` at 0x18f2e7
// inverts the result: everything that is NOT a menu bar gets the round shape.
// `this` is not read.  Retail dereferences pToolBar unconditionally (a NULL one
// makes CObject::IsKindOf take its __report_rangecheckfailure path); the NULL
// guard below is OpenMFC's, and it returns the "not a menu bar" answer.
// Symbol: ?IsToolbarRoundShape@CMFCVisualManagerOffice2003@@UEAAHPEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__IsToolbarRoundShape_CMFCVisualManagerOffice2003__UEAAHPEAVCMFCToolBar___Z(
    CMFCVisualManagerOffice2003* /*pThis*/, CMFCToolBar* pToolBar)
{
    if (pToolBar == nullptr) return TRUE;
    const int bIsMenuBar = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        reinterpret_cast<const CObject*>(pToolBar),
        impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ());
    return bIsMenuBar == 0 ? TRUE : FALSE;
}

// CMFCVisualManagerOffice2003::ModifyGlobalColors() -- retail mfc140 RVA
// 0x193550.  The gate is transcribed:
//     if (bpp <= 8 || !this->[+0x2e8] || blackHC || whiteHC) return;   // 0x193587..0x1935c8
// Past it (0x1935ce..0x19385e) retail recolours a long run of afxGlobalData
// fields from GetBaseThemeColor() (vftable +0x640), +0x210 and the HTHEME at
// +0x08, through PixelAlpha / SmartMixColors.  That tail is NOT transcribed:
// +0x2e8 is never set in this tree (see the theme-block comment), OpenMFC
// exports afxGlobalData as a zero blob the rest of the tree reads through
// ::GetSysColor instead, and writing an approximation there would corrupt it.
// Symbol: ?ModifyGlobalColors@CMFCVisualManagerOffice2003@@MEAAXXZ
extern "C" void MS_ABI impl__ModifyGlobalColors_CMFCVisualManagerOffice2003__MEAAXXZ(
    CMFCVisualManagerOffice2003* pThis)
{
    if (pThis == nullptr || !Office2003_RichColorDesktop()) return;
    if (!O2003_Theme(pThis).bIsStandardWinXPTheme) return;
    // Standard-XP-theme recolouring of afxGlobalData: unreachable here, not transcribed.
}

// CMFCVisualManagerOffice2003::OnDrawAutoHideButtonBorder(CDC* pDC, CRect rectBounds,
//         CRect rectBorderSize, CMFCAutoHideButton* pButton) -- retail mfc140 RVA
// 0x194e90, complete body.  Low-colour / high-contrast desktops tail-call
// CMFCVisualManager::OnDrawAutoHideButtonBorder (0x189310, at 0x19500a).  Otherwise:
//     clrShadow = afxGlobalData.clrBarShadow (+0x64);                        // 0x194f23
//     clrText   = ::GetTextColor(pDC->m_hAttribDC (+0x10));                  // 0x194f29 (GDI32 import 0x1802c4228)
//     if (rectBorderSize.left   > 0) pDC->FillSolidRect(bounds.left, bounds.top, bounds.left + border.left, bounds.bottom, clrShadow);   // 0x194f50
//     if (rectBorderSize.top    > 0) pDC->FillSolidRect(bounds.left, bounds.top, bounds.right, bounds.top + border.top, clrShadow);      // 0x194f74
//     if (rectBorderSize.right  > 0) pDC->FillSolidRect(bounds.right - border.right, bounds.top, bounds.right, bounds.bottom, clrShadow); // 0x194f9b
//     if (rectBorderSize.bottom > 0) pDC->FillSolidRect(bounds.left, bounds.bottom - border.bottom, bounds.right, bounds.bottom, clrShadow); // 0x194fc1
//     pDC->vftable[14](clrText);                                             // 0x194fd3: CDC +0x70 = SetTextColor
// The FillSolidRect overload is (x, y, cx, cy, clr) (0x2a3a60) and retail
// really does pass bounds.right / bounds.bottom / bounds.left + border.left as
// the cx/cy arguments; that is reproduced literally.  pButton is not read.
// Symbol: ?OnDrawAutoHideButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@1PEAVCMFCAutoHideButton@@@Z
extern "C" void MS_ABI impl__OnDrawAutoHideButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectBounds, CRect rectBorderSize,
    void* pButton /* CMFCAutoHideButton* -- not declared in the OpenMFC headers */)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (!Office2003_RichColorDesktop()) {
        impl__OnDrawAutoHideButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__1PEAVCMFCAutoHideButton___Z(
            pThis, pDC, rectBounds, rectBorderSize, pButton);
        return;
    }
    const COLORREF clrShadow = GD_clrBarShadow();
    const COLORREF clrText = ::GetTextColor(pDC->m_hAttribDC);
    if (rectBorderSize.left > 0)
        O2003_FillSolidRect(pDC, rectBounds.left, rectBounds.top, rectBounds.left + rectBorderSize.left, rectBounds.bottom, clrShadow);
    if (rectBorderSize.top > 0)
        O2003_FillSolidRect(pDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.top + rectBorderSize.top, clrShadow);
    if (rectBorderSize.right > 0)
        O2003_FillSolidRect(pDC, rectBounds.right - rectBorderSize.right, rectBounds.top, rectBounds.right, rectBounds.bottom, clrShadow);
    if (rectBorderSize.bottom > 0)
        O2003_FillSolidRect(pDC, rectBounds.left, rectBounds.bottom - rectBorderSize.bottom, rectBounds.right, rectBounds.bottom, clrShadow);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);
}

// CMFCVisualManagerOffice2003::OnDrawBarGripper(CDC* pDC, CRect rectGripper,
//         BOOL bHorz, CBasePane* pBar) -- retail mfc140 RVA 0x18fdc0, complete body:
//     if ((pBar && pBar->m_bIsDlgControl (+0xf4)) || bpp <= 8)              // 0x18fdf2 / 0x18fe1f (no HC test here)
//         return CMFCVisualManagerOfficeXP::OnDrawBarGripper(...);           // 0x18fe44 (0x1aca30)
//     if (bHorz) rect.left = rect.right - 4; else rect.top = rect.bottom - 4; // 0x18fe53..0x18fe63
//     if (pBar && pBar->IsKindOf(CMFCToolBar)) {                            // 0x18fe79 (descriptor 0x1803aa5f8 -> "CMFCToolBar")
//         if (bHorz) {
//             n = CMFCToolBar::m_bLargeIcons ? pBar->vftable[215]()          // 0x18fe9e: +0x6b8 = CMFCToolBar::GetRowHeight (0x157370)
//                                            : GetButtonSize().cy;           // 0x18feb4 (helper 0x23a74)
//             d = (n - GetImageSize().cy) / 2;                               // 0x18febc..0x18ff0b
//             ::InflateRect(rect, 0, d < 0 ? 0 : -d);                        // 0x18ff49 (d<0: 0x18ff0f; else 0x18ff14..0x18ff3f)
//         } else {
//             n = m_bLargeIcons ? pBar->vftable[216]()                       // 0x18ff60: +0x6c0 = 0x23a40 (non-exported GetColumnWidth)
//                               : GetButtonSize().cx;                        // 0x18ff77
//             d = (n - GetImageSize().cx) / 2;  ::InflateRect(rect, d < 0 ? 0 : -d, 0);   // 0x18ff7f..0x190005
//         }
//     }
//     if (bHorz) { nDots = (rect.Height() - 4) / 4; off = (rect.Height() - 4*nDots) / 2; }   // 0x1900e9..0x19002c
//     else       { nDots = (rect.Width()  - 4) / 4; off = (rect.Width()  - 4*nDots) / 2; }   // 0x19000b..0x190031
//     for (i = 0; i < nDots; ++i, off += 4) {                                // 0x19003c..0x1900c9
//         x = bHorz ? rect.left : rect.left + off;  y = bHorz ? rect.top + off : rect.top;
//         pDC->FillSolidRect(x + 1, y + 1, 2, 2, afxGlobalData.clrBtnHilite (+0x30));   // 0x190093
//         pDC->FillSolidRect(x,     y,     2, 2, this->[+0x260]);                       // 0x1900ba
//     }
// The divisions are signed (`cltd; and $3; add; sar $2` and `cltd; sub; sar 1`).
// GetButtonSize / GetImageSize are the ToolBar_* helpers above.  The retail
// 0x23a40 (column width) is not exported and not transcribed: with
// m_bLargeIcons set on a vertical toolbar it is replaced by GetButtonSize().cx,
// and the GetRowHeight thunk this calls is still a `return 0` stub -- both
// only affect the inflation of a large-icon toolbar's gripper.
// Symbol: ?OnDrawBarGripper@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar)
{
    O2003_DrawBarGripper(pThis, pDC, rectGripper, bHorz, pBar);
}

namespace {
void O2003_DrawBarGripper(CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bHorz, CBasePane* pBar)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if ((pBar != nullptr && pBar->m_bIsDlgControl) || GD_BitsPerPixel() <= 8) {
        impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
            pThis, pDC, rect, bHorz, pBar);
        return;
    }
    if (bHorz) rect.left = rect.right - 4;
    else       rect.top  = rect.bottom - 4;

    if (pBar != nullptr && O2003_IsKindOf(pBar, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        const CMFCToolBar* pToolBar = static_cast<const CMFCToolBar*>(pBar);
        const CSize szImage = ToolBar_GetImageSize(pToolBar);
        if (bHorz) {
            const int n = impl__m_bLargeIcons_CMFCToolBar__1HA
                ? impl__GetRowHeight_CMFCToolBar__UEBAHXZ(pToolBar)
                : ToolBar_GetButtonSize(pToolBar).cy;
            const int d = (n - szImage.cy) / 2;
            rect.InflateRect(0, d < 0 ? 0 : -d);
        } else {
            // DEVIATION: vftable[216] (retail 0x23a40, column width) is not
            // exported; GetButtonSize().cx is used for the large-icon case too.
            const int n = ToolBar_GetButtonSize(pToolBar).cx;
            const int d = (n - szImage.cx) / 2;
            rect.InflateRect(d < 0 ? 0 : -d, 0);
        }
    }

    int nDots, off;
    if (bHorz) { nDots = (rect.bottom - rect.top - 4) / 4;  off = (rect.bottom - rect.top - 4 * nDots) / 2; }
    else       { nDots = (rect.right - rect.left - 4) / 4;  off = (rect.right - rect.left - 4 * nDots) / 2; }
    const COLORREF clrHilite = GD_clrBtnHilite();
    const COLORREF clrDot = O2003_Theme(pThis).c260;
    for (int i = 0; i < nDots; ++i, off += 4) {
        const int x = bHorz ? rect.left : rect.left + off;
        const int y = bHorz ? rect.top + off : rect.top;
        O2003_FillSolidRect(pDC, x + 1, y + 1, 2, 2, clrHilite);
        O2003_FillSolidRect(pDC, x, y, 2, 2, clrDot);
    }
}
} // namespace

// CMFCVisualManagerOffice2003::OnDrawBrowseButton(CDC* pDC, CRect rect,
//         CMFCEditBrowseCtrl* pEdit, AFX_BUTTON_STATE state, COLORREF& clrText)
// -- retail mfc140 RVA 0x1952c0, complete body:
//     CRect rectFrame = rect; rectFrame.top -= 1; rectFrame.right += 1; rectFrame.bottom += 1;   // 0x1952d7..0x1952f8
//     if (state == 1)       pBrush = &this->br1b0;   // 0x195302 (`sub $1; je`)  -> 0x1953f1
//     else if (state == 2)  pBrush = &this->br1a0;   // 0x19530b (`cmp $1; je`)  -> 0x1953e8
//     else {                                                                   // 0x195311
//         CDrawingManager(*pDC).FillGradient(rect, afxGlobalData.clrBtnFace (+0x28),
//                                            afxGlobalData.clrBtnHilite (+0x30), TRUE, 0, 0);   // 0x19538b
//         pDC->Draw3dRect(rect, afxGlobalData.clrBarHilite (+0x68), clrBarHilite);            // 0x1953e0
//         return TRUE;
//     }
//     this->vftable[199](pDC, rect, pBrush, NULL);          // 0x195417: +0x638 = OnFillHighlightedArea
//     pDC->Draw3dRect(rectFrame, this->[+0x21c], this->[+0x21c]);   // 0x19542e
//     return TRUE;                                                   // 0x195433
// Neither pEdit nor clrText is touched on any path (the out-param is left as
// the caller set it).  The +0x1a0 / +0x1b0 CBrush members are
// ::CreateSolidBrush(+0x118) and (+0x11c) (OfficeXP::OnUpdateSystemColors
// 0x1ac944 / 0x1ac95f); the brush identity is forwarded to
// O2003_FillHighlightedArea, which is the slot-199 body.
// The state values are compared NUMERICALLY on purpose.  In retail's
// AFX_BUTTON_STATE, 1 is ButtonsIsPressed and 2 is ButtonsIsHighlighted --
// CMFCVisualManager::OnDrawButtonBorder (0x183600) draws state 1 sunken
// (dark top-left, 0x1836a2) and state 2 raised (0x18367c) -- so retail pairs
// Pressed with the +0x1b0 "highlight-down" brush and Highlighted with +0x1a0.
// include/openmfc/afxmfc.h now spells the enum in retail order; the literals
// are kept so this body stays ABI-correct independently of that header.
// Symbol: ?OnDrawBrowseButton@CMFCVisualManagerOffice2003@@UEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@CMFCVisualManager@@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect,
    void* /*pEdit*/ /* CMFCEditBrowseCtrl* -- not declared in the OpenMFC headers */,
    CMFCVisualManager::AFX_BUTTON_STATE state, unsigned long& /*clrText*/)
{
    if (pThis == nullptr || pDC == nullptr) return TRUE;
    CRect rectFrame = rect;
    rectFrame.top -= 1; rectFrame.right += 1; rectFrame.bottom += 1;

    const O2003Theme t = O2003_Theme(pThis);
    unsigned nBrushId;
    COLORREF clrBrush;
    if (static_cast<int>(state) == 1)       { nBrushId = 0x1b0; clrBrush = t.xp.c11c; }   // retail ButtonsIsPressed
    else if (static_cast<int>(state) == 2)  { nBrushId = 0x1a0; clrBrush = t.xp.c118; }   // retail ButtonsIsHighlighted
    else {
        O2003_FillGradient(pDC, rect, GD_clrBtnFace(), GD_clrBtnHilite(), TRUE, 0, 0);
        O2003_Draw3dRect(pDC, rect, GD_clrBarHilite(), GD_clrBarHilite());
        return TRUE;
    }
    // The OfficeXP fallback inside O2003_FillHighlightedArea takes a CBrush*;
    // a raw ::CreateSolidBrush handle is passed through a CBrush-shaped local
    // (vptr + m_hObject, the layout include/openmfc/afxwin.h declares) so that
    // no CGdiObject method is invoked from this unit.
    struct { const void* vptr; HGDIOBJ h; } brushShape = { nullptr, ::CreateSolidBrush(clrBrush) };
    O2003_FillHighlightedArea(pThis, pDC, rect, reinterpret_cast<CBrush*>(&brushShape), nBrushId, nullptr);
    if (brushShape.h != nullptr) ::DeleteObject(brushShape.h);
    O2003_Draw3dRect(pDC, rectFrame, t.c21c, t.c21c);
    return TRUE;
}

// CMFCVisualManagerOffice2003::OnDrawButtonBorder(CDC* pDC, CMFCToolBarButton* pButton,
//         CRect rect, AFX_BUTTON_STATE state) -- retail mfc140 RVA 0x1906a0, complete body:
//     if (pButton && pButton->IsKindOf(CMFCCustomizeButton)          // 0x1906c4 (descriptor 0x1803aa148 -> "CMFCCustomizeButton")
//         && pButton->[+0x150] != 0 && pButton->[+0x154] == 0         // 0x1906cd / 0x1906d6
//         && bpp > 8 && !blackHC && !whiteHC)                         // 0x190700..0x190733
//         return;                                                     // -> 0x19075c, draws nothing
//     CMFCVisualManagerOfficeXP::OnDrawButtonBorder(pDC, pButton, rect, state);   // 0x190757 (0x1adc30)
// +0x150 / +0x154 are CZButton::m_n150 / m_b154 in
// phase4/src/detail/CMFCCustomizeButtonSupport.h (m_n150 is set to 1 by
// CMFCCustomizeButton::CommonInit).  The OfficeXP fallback is a real body in
// CMFCVisualManagerOfficeXP.cpp.
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect,
    CMFCVisualManager::AFX_BUTTON_STATE state)
{
    if (pThis == nullptr) return;
    if (pButton != nullptr &&
        O2003_IsKindOf(pButton, impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ())) {
        const char* p = reinterpret_cast<const char*>(pButton);
        const int n150 = *reinterpret_cast<const int*>(p + 0x150);   // CZButton::m_n150
        const int b154 = *reinterpret_cast<const int*>(p + 0x154);   // CZButton::m_b154
        if (n150 != 0 && b154 == 0 && Office2003_RichColorDesktop()) return;
    }
    impl__OnDrawButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
        pThis, pDC, pButton, rect, static_cast<int>(state));
}

// CMFCVisualManagerOffice2003::OnDrawCaptionBarBorder(CDC* pDC, CMFCCaptionBar* pBar,
//         CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder) -- retail mfc140 RVA
// 0x191ea0, complete body:
//     if (clrBarBorder == (COLORREF)-1) {                                    // 0x191ebc
//         hbr = (pBar && pBar->[+0xf4]) ? afxGlobalData.brBtnFace (+0xa8)    // 0x191ec6 / 0x191eef
//                                       : afxGlobalData.brBarFace (+0x118);  // 0x191f17
//         ::FillRect(pDC->m_hDC, rect, hbr);                                 // 0x191f25
//     } else { CBrush br(clrBarBorder); ::FillRect(pDC->m_hDC, rect, br); }  // 0x191f32..0x191f5a
//     if (!bFlatBorder) pDC->Draw3dRect(rect, this->[+0x218], this->[+0x238]);   // 0x191f5f / 0x191f7a
// pBar->[+0xf4] is CBasePane::m_bIsDlgControl (CMFCCaptionBar derives from
// CPane -- its descriptor in phase4/src/featurepack/controls/RuntimeClasses.cpp
// names CPane as the base -- and the harvested CBasePane layout puts
// m_bIsDlgControl at 0xf4).  The two afxGlobalData brushes are
// ::CreateSolidBrush(clrBtnFace) and ::CreateSolidBrush(clrBarFace)
// (AFX_GLOBAL_DATA::UpdateSysColors 0x6b23b / 0x6b25e); OpenMFC has no brush
// handles in afxGlobalData, so a solid brush of the same colour is made here.
// Symbol: ?OnDrawCaptionBarBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@KH@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__KH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    unsigned long clrBarBorder, int bFlatBorder)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (clrBarBorder == (COLORREF)-1) {
        const bool bDlgControl = pBar != nullptr &&
            reinterpret_cast<const CBasePane*>(pBar)->m_bIsDlgControl != 0;
        O2003_FillRect(pDC, rect, bDlgControl ? GD_brBtnFaceColor() : GD_brBarFaceColor());
    } else {
        O2003_FillRect(pDC, rect, clrBarBorder);
    }
    if (!bFlatBorder) {
        const O2003Theme t = O2003_Theme(pThis);
        O2003_Draw3dRect(pDC, rect, t.c218, t.c238);
    }
}

// CMFCVisualManagerOffice2003::OnDrawCheckBoxEx -- this export is the INHERITED
// CMFCVisualManagerWindows::OnDrawCheckBoxEx (see the file header: mfc140u
// export table, and slot 107 of the retail Office2003 vftable at 0x18031a5c0).
// Symbol: ?OnDrawCheckBoxEx@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawCheckBoxEx_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect,
    int nState, int bHighlighted, int bPressed, int bEnabled)
{
    impl__OnDrawCheckBoxEx_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HHHH_Z(
        pThis, pDC, rect, nState, bHighlighted, bPressed, bEnabled);
}

// CMFCVisualManagerOffice2003::OnDrawComboBorder(CDC* pDC, CRect rect, BOOL bDisabled,
//         BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton* pButton)
// -- retail mfc140 RVA 0x190120, complete body.  Low-colour / high-contrast
// desktops call CMFCVisualManagerOfficeXP::OnDrawComboBorder (0x1afc80, at
// 0x19023a) with all six arguments.  Otherwise:
//     if (!bIsHighlighted && !bIsDropped && !bDisabled) return;             // 0x190192..0x1901a8
//     ::InflateRect(rect, -1, -1);                                          // 0x1901b7 (USER32 import 0x1802c5310)
//     clr = bDisabled ? afxGlobalData.clrBtnShadow (+0x2c) : this->[+0x138]; // 0x1901bd..0x1901e9
//     pDC->Draw3dRect(rect, clr, clr);                                      // 0x1901f9
// +0x138 is the OfficeXP border colour (O2003_XP().c138; retail's +0x2e8 path
// would have overwritten it at 0x1912cd, which never runs here).
// Symbol: ?OnDrawComboBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped,
    int bIsHighlighted, CMFCToolBarComboBoxButton* pButton)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (!Office2003_RichColorDesktop()) {
        impl__OnDrawComboBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
            pThis, pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
        return;
    }
    if (!bIsHighlighted && !bIsDropped && !bDisabled) return;
    rect.InflateRect(-1, -1);
    const COLORREF clr = bDisabled ? GD_clrBtnShadow() : O2003_XP().c138;
    O2003_Draw3dRect(pDC, rect, clr, clr);
}

// CMFCVisualManagerOffice2003::OnDrawComboDropButton(CDC* pDC, CRect rect, BOOL bDisabled,
//         BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton* pButton)
// -- retail mfc140 RVA 0x191f90, complete body.  Low-colour / high-contrast
// desktops call CMFCVisualManagerOfficeXP::OnDrawComboDropButton (0x1afa30, at
// 0x192253).  Otherwise:
//     if (!bDisabled) {                                                          // 0x192012
//         if (!bIsDropped && !bIsHighlighted) {                                  // 0x19201b / 0x192024
//             CDrawingManager(*pDC).FillGradient(rect, +0x21c, +0x220, TRUE, 0, 0);          // 0x192069
//             if (CMFCToolBarImages::m_bIsDrawOnGlass) dm.DrawRect(rect, -1, afxGlobalData.clrWindow (+0x78));   // 0x1920a4 (0x5abf0)
//             else pDC->Draw3dRect(rect, clrWindow, clrWindow);                              // 0x1920f9
//         } else {
//             this->vftable[199](pDC, rect, bIsDropped ? &br1b0 : &br1a0, NULL);            // 0x19213f: +0x638 OnFillHighlightedArea
//             clrBorder = this->[+0x138];                                                    // 0x192145
//             if (m_bIsDrawOnGlass) dm.DrawLine(rect.left, rect.top, rect.left, rect.bottom, clrBorder);   // 0x19217e (0x59350)
//             else { CPen pen(PS_SOLID, 1, clrBorder); pOld = pDC->SelectObject(&pen);     // 0x19218f / 0x19219c
//                    pDC->MoveTo(rect.left, rect.top); pDC->LineTo(rect.left, rect.bottom); // 0x1921bb / 0x1921c9
//                    pDC->SelectObject(pOld); }                                             // 0x1921d4
//         }
//     }
//     nState = bDisabled ? 1 /*ImageGray*/ : (bIsDropped && bIsHighlighted) ? 3 /*ImageWhite*/ : 0 /*ImageBlack*/;   // 0x1921f6..0x19220e
//     CMenuImages::Draw(pDC, 0 /*IdArrowDown*/, rect, nState, CSize(0,0));       // 0x192222 (0x8fd40, the CRect& overload)
// The DrawRect / DrawLine members reached on the m_bIsDrawOnGlass path are
// still stubs in this tree (phase4/src/core/gdi/CDrawingManager.cpp); they
// are called as retail does and draw nothing until implemented.  So is
// CMenuImages::Draw, hence no arrow appears yet.
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped,
    int bIsHighlighted, CMFCToolBarComboBoxButton* pButton)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (!Office2003_RichColorDesktop()) {
        impl__OnDrawComboDropButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
            pThis, pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
        return;
    }
    if (!bDisabled) {
        const O2003Theme t = O2003_Theme(pThis);
        if (!bIsDropped && !bIsHighlighted) {
            O2003_FillGradient(pDC, rect, t.c21c, t.c220, TRUE, 0, 0);
            const COLORREF clrWindow = GD_clrWindow();
            if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA) {
                struct { const void* vptr; CDC* pDC; } dm = { nullptr, pDC };   // CDrawingManager { vptr, CDC& }
                impl__DrawRect_CDrawingManager__QEAAXAEBVCRect__KK_Z(&dm, &rect, (COLORREF)-1, clrWindow);
            } else {
                O2003_Draw3dRect(pDC, rect, clrWindow, clrWindow);
            }
        } else {
            const unsigned nBrushId = bIsDropped ? 0x1b0u : 0x1a0u;
            struct { const void* vptr; HGDIOBJ h; } brushShape =
                { nullptr, ::CreateSolidBrush(bIsDropped ? t.xp.c11c : t.xp.c118) };
            O2003_FillHighlightedArea(pThis, pDC, rect, reinterpret_cast<CBrush*>(&brushShape), nBrushId, nullptr);
            if (brushShape.h != nullptr) ::DeleteObject(brushShape.h);
            const COLORREF clrBorder = t.xp.c138;
            if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA) {
                struct { const void* vptr; CDC* pDC; } dm = { nullptr, pDC };
                impl__DrawLine_CDrawingManager__QEAAXHHHHK_Z(&dm, rect.left, rect.top, rect.left, rect.bottom, clrBorder);
            } else {
                O2003_PenLine(pDC, rect.left, rect.top, rect.left, rect.bottom, clrBorder);
            }
        }
    }
    const int nState = bDisabled ? 1 : ((bIsDropped && bIsHighlighted) ? 3 : 0);
    const CSize sizeNull(0, 0);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(pDC, 0, rect, nState, sizeNull);
}

// CMFCVisualManagerOffice2003::OnDrawControlBorder -- this export is the
// INHERITED CMFCVisualManagerWindows::OnDrawControlBorder (see the file header:
// mfc140u export table, and slot 108 of the retail Office2003 vftable).
// Symbol: ?OnDrawControlBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnDrawControlBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCWnd___Z(
    CMFCVisualManagerOffice2003* pThis, CWnd* pWndCtrl)
{
    impl__OnDrawControlBorder_CMFCVisualManagerWindows__UEAAXPEAVCWnd___Z(pThis, pWndCtrl);
}

// CMFCVisualManagerOffice2003::OnDrawExpandingBox -- this export is the
// INHERITED CMFCVisualManagerWindows::OnDrawExpandingBox (see the file header:
// mfc140u export table, and slot 100 of the retail Office2003 vftable).
// Symbol: ?OnDrawExpandingBox@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@HK@Z
extern "C" void MS_ABI impl__OnDrawExpandingBox_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__HK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, int bIsOpened, unsigned long colorBox)
{
    impl__OnDrawExpandingBox_CMFCVisualManagerWindows__UEAAXPEAVCDC__VCRect__HK_Z(
        pThis, pDC, rect, bIsOpened, colorBox);
}

// CMFCVisualManagerOffice2003::OnDrawHeaderCtrlBorder -- this export is the
// INHERITED CMFCVisualManagerWindows::OnDrawHeaderCtrlBorder (see the file
// header: mfc140u export table, and slot 88 of the retail Office2003 vftable).
// Symbol: ?OnDrawHeaderCtrlBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCMFCHeaderCtrl@@PEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CMFCHeaderCtrl* pCtrl, CDC* pDC, CRect& rect,
    int bIsPressed, int bIsHighlighted)
{
    impl__OnDrawHeaderCtrlBorder_CMFCVisualManagerWindows__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
        pThis, pCtrl, pDC, rect, bIsPressed, bIsHighlighted);
}

// CMFCVisualManagerOffice2003::OnDrawMenuBorder(CDC* pDC, CMFCPopupMenu* pMenu, CRect rect)
// -- retail mfc140 RVA 0x1923f0, complete body:
//     saved = this->[+0x200];                                                 // 0x1923fb
//     if (pMenu->[+0x228] && pMenu->[+0x228]->IsKindOf(CMFCCustomizeButton))  // 0x192405..0x192428 (descriptor 0x1803aa148)
//         this->[+0x200] = 0;                                                 // 0x19242a
//     CMFCVisualManagerOfficeXP::OnDrawMenuBorder(pDC, pMenu, rect);          // 0x19244b (0x1ace60)
//     this->[+0x200] = saved;                                                 // 0x192450
// +0x200 is the first member past CMFCVisualManagerOfficeXP's +0x1f0 CPen and
// is only read by the OfficeXP body being called (its m_bDrawMenuBorder-style
// gate).  DEVIATION: the save/clear/restore is not reproduced because OpenMFC's
// object has no storage at +0x200 (CMFCVisualManager plus 64 bytes of
// padding), so there is nothing to clear and the OfficeXP body cannot observe
// it.  pMenu->[+0x228] itself IS readable -- it is PopupMenu::m_pParentButton
// in phase4/src/detail/CMFCPopupMenuSupport.h, and CMFCPopupMenu.cpp reads it
// the same way -- but with no +0x200 to clear, reading it here would change
// nothing, so the IsKindOf probe is omitted too.  Only the OfficeXP call is
// made, with the same arguments retail passes.
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCPopupMenu* pMenu, CRect rect)
{
    if (pThis == nullptr) return;
    impl__OnDrawMenuBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(pThis, pDC, pMenu, rect);
}

// CMFCVisualManagerOffice2003::OnDrawOutlookBarSplitter(CDC* pDC, CRect rectSplitter)
// -- retail mfc140 RVA 0x195020, complete body.  Low-colour / high-contrast
// desktops call CMFCVisualManager::OnDrawOutlookBarSplitter (0x189440, at
// 0x195192).  Otherwise:
//     CDrawingManager(*pDC).FillGradient(rect, this->[+0x268], this->[+0x264], TRUE, 0, 0);   // 0x1950dc
//     nGrip = rect.Height() - 3;                                            // 0x1950e1..0x1950ec
//     x = (rect.left + rect.right) / 2 - 5 * nGrip;  y = rect.top + 2;      // 0x1950f0..0x195102
//     nHalf = nGrip / 2;                                                    // 0x195106..0x19510e (signed)
//     for (10 times; x += nGrip) {                                          // 0x195116..0x19517a
//         pDC->FillSolidRect(x + 1, y + 1, nHalf, nHalf, afxGlobalData.clrBtnHilite (+0x30));   // 0x195150
//         pDC->FillSolidRect(x,     y,     nHalf, nHalf, this->[+0x260]);                      // 0x19516e
//     }
// Symbol: ?OnDrawOutlookBarSplitter@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawOutlookBarSplitter_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectSplitter)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (!Office2003_RichColorDesktop()) {
        impl__OnDrawOutlookBarSplitter_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(pThis, pDC, rectSplitter);
        return;
    }
    const O2003Theme t = O2003_Theme(pThis);
    O2003_FillGradient(pDC, rectSplitter, t.c268, t.c264, TRUE, 0, 0);
    const int nGrip = rectSplitter.bottom - rectSplitter.top - 3;
    int x = (rectSplitter.left + rectSplitter.right) / 2 - 5 * nGrip;
    const int y = rectSplitter.top + 2;
    const int nHalf = nGrip / 2;
    const COLORREF clrHilite = GD_clrBtnHilite();
    for (int i = 0; i < 10; ++i, x += nGrip) {
        O2003_FillSolidRect(pDC, x + 1, y + 1, nHalf, nHalf, clrHilite);
        O2003_FillSolidRect(pDC, x, y, nHalf, nHalf, t.c260);
    }
}

// CMFCVisualManagerOffice2003::OnDrawOutlookPageButtonBorder(CDC* pDC, CRect& rect,
//         BOOL bIsHighlighted, BOOL bIsPressed) -- retail mfc140 RVA 0x1903c0, complete body:
//     if (bpp > 8 && !blackHC && !whiteHC)                                    // 0x1903f6..0x19042a
//         pDC->Draw3dRect(rect, afxGlobalData.clrBtnHilite (+0x30), this->[+0x260]);   // 0x19045c
//     else CMFCVisualManager::OnDrawOutlookPageButtonBorder(pDC, rect, bIsHighlighted, bIsPressed);   // 0x19047a (0x186a10)
// Symbol: ?OnDrawOutlookPageButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@AEAVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOutlookPageButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__AEAVCRect__HH_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect& rect, int bIsHighlighted, int bIsPressed)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (Office2003_RichColorDesktop()) {
        O2003_Draw3dRect(pDC, rect, GD_clrBtnHilite(), O2003_Theme(pThis).c260);
        return;
    }
    impl__OnDrawOutlookPageButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__AEAVCRect__HH_Z(pThis, pDC, &rect, bIsHighlighted, bIsPressed);
}

// CMFCVisualManagerOffice2003::OnDrawPaneBorder(CDC* pDC, CBasePane* pBar, CRect& rect)
// -- retail mfc140 RVA 0x18fd30, complete body:
//     if (pBar->m_bIsDlgControl (+0xf4) || bpp <= 8 || blackHC || whiteHC)   // 0x18fd39 / 0x18fd70..0x18fda3
//         CMFCVisualManagerOfficeXP::OnDrawPaneBorder(pDC, pBar, rect);       // 0x18fdb1 (0x1ad500)
//     // otherwise nothing is drawn (the `je 0x18018fdb6` at 0x18fda3)
// Retail dereferences pBar unconditionally; the NULL guard is OpenMFC's.
// Symbol: ?OnDrawPaneBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCBasePane@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CBasePane* pBar, CRect& rect)
{
    if (pThis == nullptr || pBar == nullptr) return;
    if (pBar->m_bIsDlgControl || !Office2003_RichColorDesktop()) {
        impl__OnDrawPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(pThis, pDC, pBar, &rect);
    }
}

// CMFCVisualManagerOffice2003::OnDrawPaneCaption(CDC* pDC, CDockablePane* pBar,
//         BOOL bActive, CRect rectCaption, CRect rectButtons) -- retail mfc140 RVA
// 0x194c20, complete body.  Low-colour / high-contrast desktops return
// CMFCVisualManagerOfficeXP::OnDrawPaneCaption(...) (0x1ae8e0, at 0x194d58).  Otherwise:
//     CDrawingManager dm(*pDC);
//     if (!bActive) dm.FillGradient(rectCaption, this->[+0x21c], this->[+0x220], TRUE, 0, 0);   // 0x194cdb..0x194cf9
//     else          dm.FillGradient(rectCaption, this->[+0x24c], this->[+0x248], TRUE, 0, 0);   // 0x194ceb..0x194cf9
//     return afxGlobalData.clrBarText (+0x74);                                                 // 0x194d1a
// pBar and rectButtons are not read on this path.
// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CDockablePane* pBar, int bActive,
    CRect rectCaption, CRect rectButtons)
{
    if (pThis == nullptr || pDC == nullptr) return 0;
    if (!Office2003_RichColorDesktop()) {
        return impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
            pThis, pDC, pBar, bActive, rectCaption, rectButtons);
    }
    const O2003Theme t = O2003_Theme(pThis);
    if (!bActive) O2003_FillGradient(pDC, rectCaption, t.c21c, t.c220, TRUE, 0, 0);
    else          O2003_FillGradient(pDC, rectCaption, t.c24c, t.c248, TRUE, 0, 0);
    return GD_clrBarText();
}

// CMFCVisualManagerOffice2003::OnDrawPopupWindowBorder(CDC* pDC, CRect rect)
// -- retail mfc140 RVA 0x195ba0, complete body:
//     pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);   // 0x195bba (0x2a3b00)
// +0x138 is the OfficeXP border colour (O2003_XP().c138); retail's +0x2e8
// path would overwrite it at 0x1912cd, which never runs in this tree.
// Symbol: ?OnDrawPopupWindowBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect)
{
    if (pThis == nullptr || pDC == nullptr) return;
    const COLORREF clr = O2003_XP().c138;
    O2003_Draw3dRect(pDC, rect, clr, clr);
}

// CMFCVisualManagerOffice2003::OnDrawPopupWindowButtonBorder -- this export is
// the INHERITED CMFCVisualManagerOfficeXP::OnDrawPopupWindowButtonBorder (see
// the file header: mfc140u export table, and slot 125 of the retail Office2003
// vftable).  Unlike the four CMFCVisualManagerWindows aliases, the OfficeXP
// thunk is a real implementation in this tree.
// Symbol: ?OnDrawPopupWindowButtonBorder@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, CMFCDesktopAlertWndButton* pButton)
{
    impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
        pThis, pDC, rect, pButton);
}

// CMFCVisualManagerOffice2003::OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption,
//         CMFCDesktopAlertWnd* pPopupWnd) -- retail mfc140 RVA 0x195bd0, complete body.
// Low-colour / high-contrast desktops return
// CMFCVisualManagerOfficeXP::OnDrawPopupWindowCaption(...) (0x1b1930, at 0x195d34).  Otherwise:
//     CDrawingManager(*pDC).FillGradient(rectCaption, this->[+0x268], this->[+0x264], TRUE, 0, 0);   // 0x195c91
//     if (pPopupWnd->[+0x10c]) {                                                                 // 0x195c96
//         cx = (rect.left + rect.right) / 2;  cy = (rect.top + rect.bottom) / 2;                  // 0x195ca0..0x195cb6
//         this->vftable[19](pDC, CRect(cx - 20, cy - 4, cx + 20, cy + 2), FALSE, NULL);           // 0x195cf2: +0x098 = OnDrawBarGripper
//     }
//     return afxGlobalData.clrBarHilite (+0x68);                                                 // 0x195d17
// DEVIATION: pPopupWnd->[+0x10c] lies outside OpenMFC's CMFCDesktopAlertWnd
// (CWnd plus 16 bytes, sizeof 0xf8) and cannot be read; the small gripper is
// therefore never drawn here.  The slot-19 dispatch is replaced by a direct
// call to this class's own gripper body (see O2003_DrawBarGripper).
// Symbol: ?OnDrawPopupWindowCaption@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWnd@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rectCaption, CMFCDesktopAlertWnd* pPopupWnd)
{
    if (pThis == nullptr || pDC == nullptr) return 0;
    if (!Office2003_RichColorDesktop()) {
        return impl__OnDrawPopupWindowCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
            pThis, pDC, rectCaption, pPopupWnd);
    }
    const O2003Theme t = O2003_Theme(pThis);
    O2003_FillGradient(pDC, rectCaption, t.c268, t.c264, TRUE, 0, 0);
    // pPopupWnd->[+0x10c] gripper flag: unreadable here (see above).
    (void)pPopupWnd;
    return GD_clrBarHilite();
}

// CMFCVisualManagerOffice2003::OnDrawRibbonButtonsGroup(CDC* pDC,
//         CMFCRibbonButtonsGroup* pGroup, CRect rectGroup) -- retail mfc140 RVA 0x1965c0:
//     if (pGroup && pGroup->IsKindOf(CMFCRibbonQuickAccessToolBar)) return -1;   // 0x1965e5 (descriptor 0x180309930)
//     if (pGroup->[+0x190] == 0) return -1;                                       // 0x1965f2 (element count)
//     if (pGroup->[+0x188][0]->[+0x158] == 0) return -1;                          // 0x19660d..0x19661e (first element)
//     CPen pen(PS_SOLID, 1, this->[+0x21c]); select it and NULL_BRUSH;            // 0x196636..0x196664
//     ::InflateRect(rectGroup, -1, -1); ::RoundRect(pDC->m_hDC, l, t, r, b, 2, 2); // 0x19667b / 0x1966a5
//     restore; return -1;
// Left stubbed: OpenMFC's CMFCRibbonButtonsGroup is CMFCRibbonBaseElement plus
// 32 bytes of padding, so the element array at +0x188/+0x190 and the first
// element's +0x158 flag cannot be read.  Returns -1 as every retail path does.
// Symbol: ?OnDrawRibbonButtonsGroup@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCMFCRibbonButtonsGroup@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonButtonsGroup_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCMFCRibbonButtonsGroup__VCRect___Z(
    CMFCVisualManagerOffice2003* /*pThis*/, CDC* /*pDC*/, CMFCRibbonButtonsGroup* /*pGroup*/, CRect /*rectGroup*/)
{
    return (unsigned long)-1;
}

// CMFCVisualManagerOffice2003::OnDrawRibbonCategoryCaption(CDC* pDC,
//         CMFCRibbonContextCaption* pCaption) -- retail mfc140 RVA 0x196700.
// Low-colour / high-contrast desktops return CMFCVisualManager::
// OnDrawRibbonCategoryCaption (0x18c390, at 0x196831).  Otherwise:
//     clr = this->vftable[163](pCaption->[+0x270]);        // 0x19678e: +0x518 = RibbonCategoryColorToRGB
//     if (clr != -1) CDrawingManager(*pDC).FillGradient(pCaption->[+0xc8] /*CRect*/, clr,
//                                                       afxGlobalData.clrBarFace (+0x60), TRUE, 0, 0);   // 0x1967fe
//     return afxGlobalData.clrBarText (+0x74);              // 0x196820
// The gradient is not painted: OpenMFC's CMFCRibbonContextCaption is CObject
// plus 32 bytes of padding, so neither the +0x270 category colour nor the
// +0xc8 rect exists.  The control flow around it is retail's -- the low-colour
// path is the base call, and the rich-colour path returns clrBarText whether
// or not the gradient was drawn (0x196804..0x196826 is reached from both the
// clr == -1 skip at 0x196799 and the FillGradient at 0x1967fe).
// Symbol: ?OnDrawRibbonCategoryCaption@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCMFCRibbonContextCaption@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryCaption_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCMFCRibbonContextCaption___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonContextCaption* pCaption)
{
    if (pThis == nullptr) return 0;
    if (!Office2003_RichColorDesktop()) {
        return impl__OnDrawRibbonCategoryCaption_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonContextCaption___Z(
            pThis, pDC, pCaption);
    }
    // TODO(clean-room): FillGradient(pCaption->[+0xc8], RibbonCategoryColorToRGB(pCaption->[+0x270]),
    // clrBarFace, TRUE, 0, 0) needs the unmodelled CMFCRibbonContextCaption layout.
    return GD_clrBarText();
}

// CMFCVisualManagerOffice2003::OnDrawRibbonCategoryTab(CDC* pDC, CMFCRibbonTab* pTab,
//         BOOL bIsActive) -- retail mfc140 RVA 0x196010 (0x5b0 bytes).  Not
// transcribed: it reads the tab's rect (+0xc8..+0xd4), its +0x140 flag, its
// owner (+0xe0), the owner's +0x258 colour and the owner's +0x760 pointer
// (whose +0x40c / +0x478 words gate the shape, 0x1960b4..0x1960c7 / 0x196171),
// dispatches the tab's vftable slots +0x1b0 / +0x1b8 / +0x1d8 / +0x398 and
// this->[+0x518] (RibbonCategoryColorToRGB), and paints several
// CDrawingManager gradients keyed on this->[+0x248]/[+0x24c].  OpenMFC's
// CMFCRibbonTab is CMFCRibbonBaseElement plus 32 bytes of padding, so none of
// the tab members can be read and the drawing is not reproducible.  What IS
// reproducible is the control flow around it: low-colour / high-contrast
// desktops return CMFCVisualManager::OnDrawRibbonCategoryTab (0x18ab00, at
// 0x19658c), and every rich-colour exit returns afxGlobalData.clrBarText
// (+0x74: 0x196255 and 0x196554/0x19657c).  That shape is kept so callers
// get retail's text colour; the tab itself is not painted here.
// Symbol: ?OnDrawRibbonCategoryTab@CMFCVisualManagerOffice2003@@UEAAKPEAVCDC@@PEAVCMFCRibbonTab@@H@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonCategoryTab_CMFCVisualManagerOffice2003__UEAAKPEAVCDC__PEAVCMFCRibbonTab__H_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonTab* pTab, int bIsActive)
{
    if (pThis == nullptr) return 0;
    if (!Office2003_RichColorDesktop()) {
        return impl__OnDrawRibbonCategoryTab_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCRibbonTab__H_Z(
            pThis, pDC, pTab, bIsActive);
    }
    // TODO(clean-room): the tab painting (0x1960a9..0x196554) needs the
    // CMFCRibbonTab / owner layout OpenMFC does not model; only the return
    // value is retail's.
    return GD_clrBarText();
}

// CMFCVisualManagerOffice2003::OnDrawRibbonQuickAccessToolBarSeparator(CDC* pDC,
//         CMFCRibbonSeparator* pSeparator, CRect rect) -- retail mfc140 RVA
// 0x196f30, complete body (pSeparator is never read):
//     x = (rect.left + rect.right) / 2;                                              // 0x196f4b..0x196f57
//     if (CMFCToolBarImages::m_bIsDrawOnGlass) {                                     // 0x196f59
//         CDrawingManager dm(*pDC);
//         dm.DrawLine(x,     rect.top,     x,     rect.bottom - 1, afxGlobalData.clrBarDkShadow (+0x6c));   // 0x196fb5 (0x59350)
//         dm.DrawLine(x + 1, rect.top + 1, x + 1, rect.bottom,     afxGlobalData.clrBarLight    (+0x70));   // 0x196ff2
//     } else {
//         pOld = pDC->SelectObject(&this->pen1e0);                                   // 0x197004
//         pDC->MoveTo(x, rect.top); pDC->LineTo(x, rect.bottom - 1);                 // 0x197022 / 0x197030
//         pDC->SelectObject(&this->pen2a8);                                          // 0x19703f
//         pDC->MoveTo(x + 1, rect.top + 1); pDC->LineTo(x + 1, rect.bottom);         // 0x197057 / 0x197065
//         pDC->SelectObject(pOld);                                                   // 0x197070
//     }
// The two CPen members are ::CreatePen(PS_SOLID, 1, clrBarShadow) at
// 0x1915e0 and ::CreatePen(PS_SOLID, 1, clrBarHilite) at 0x191550 of
// OnUpdateSystemColors (O2003Theme::pen1e0 / pen2a8).  CDrawingManager::
// DrawLine is a stub in this tree, so the glass path draws nothing yet.
// Symbol: ?OnDrawRibbonQuickAccessToolBarSeparator@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCRibbonSeparator@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonQuickAccessToolBarSeparator_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCRibbonSeparator__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCRibbonSeparator* /*pSeparator*/, CRect rect)
{
    if (pThis == nullptr || pDC == nullptr) return;
    const int x = (rect.left + rect.right) / 2;
    if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA) {
        struct { const void* vptr; CDC* pDC; } dm = { nullptr, pDC };   // CDrawingManager { vptr, CDC& }
        impl__DrawLine_CDrawingManager__QEAAXHHHHK_Z(&dm, x, rect.top, x, rect.bottom - 1, GD_clrBarDkShadow());
        impl__DrawLine_CDrawingManager__QEAAXHHHHK_Z(&dm, x + 1, rect.top + 1, x + 1, rect.bottom, GD_clrBarLight());
        return;
    }
    const O2003Theme t = O2003_Theme(pThis);
    O2003_PenLine(pDC, x, rect.top, x, rect.bottom - 1, t.pen1e0);
    O2003_PenLine(pDC, x + 1, rect.top + 1, x + 1, rect.bottom, t.pen2a8);
}

// CMFCVisualManagerOffice2003::OnFillTasksPaneBackground(CDC* pDC, CRect rect)
// -- retail mfc140 RVA 0x193cf0, complete body:
//     if (bpp > 8 && !blackHC && !whiteHC)                                       // 0x193d29..0x193d5c
//         CDrawingManager(*pDC).FillGradient(rect, this->[+0x26c], this->[+0x270], TRUE, 0, 0);   // 0x193da8
//     else CMFCVisualManager::OnFillTasksPaneBackground(pDC, rect);              // 0x193dc4 (0x187630)
// Symbol: ?OnFillTasksPaneBackground@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillTasksPaneBackground_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect)
{
    if (pThis == nullptr || pDC == nullptr) return;
    if (Office2003_RichColorDesktop()) {
        const O2003Theme t = O2003_Theme(pThis);
        O2003_FillGradient(pDC, rect, t.c26c, t.c270, TRUE, 0, 0);
        return;
    }
    impl__OnFillTasksPaneBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(pThis, pDC, rect);
}

// CMFCVisualManagerOffice2003::OnHighlightQuickCustomizeMenuButton(CDC* pDC,
//         CMFCToolBarMenuButton* pButton, CRect rect) -- retail mfc140 RVA 0x195990,
// complete body (pButton is never read; there is no bits-per-pixel test here):
//     if (!blackHC && !whiteHC) { CBrush br(this->[+0x220]); ::FillRect(pDC->m_hDC, rect, br); }   // 0x1959c0..0x195a05
//     else ::FillRect(pDC->m_hDC, rect, this->br160.m_hObject);                                  // 0x195a0c..0x195a28
//     pDC->Draw3dRect(rect, this->[+0x134], this->[+0x134]);                                     // 0x195a3e
// The +0x160 CBrush is ::CreateSolidBrush(+0x108) (OfficeXP::OnUpdateSystemColors
// 0x1ac8f3), i.e. O2003_XP().c108; +0x134 is O2003_XP().c134 (retail's +0x2e8
// path would overwrite it at 0x1912f8, which never runs here).
// Symbol: ?OnHighlightQuickCustomizeMenuButton@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightQuickCustomizeMenuButton_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CMFCToolBarMenuButton* /*pButton*/, CRect rect)
{
    if (pThis == nullptr || pDC == nullptr) return;
    const O2003_XPColors xp = O2003_XP();
    if (!GD_IsBlackHighContrast() && !GD_IsWhiteHighContrast()) {
        O2003_FillRect(pDC, rect, O2003_Theme(pThis).c220);
    } else {
        O2003_FillRect(pDC, rect, xp.c108);
    }
    O2003_Draw3dRect(pDC, rect, xp.c134, xp.c134);
}

// CMFCVisualManagerOffice2003::OnHighlightRarelyUsedMenuItems(CDC* pDC, CRect rect)
// -- retail mfc140 RVA 0x195480, complete body:
//     rect.left--;                                                          // 0x19549b
//     nMargin = this->vftable[185]();                                       // 0x1954ab: +0x5c8 -> 0xdbf0, `return 3`
//     rect.right = rect.left + 2*nMargin + CMFCToolBar::GetMenuImageSize().cx + 2;   // 0x1954bc (0x155a60) .. 0x1954cb
//     if (bpp > 8 && !blackHC && !whiteHC)                                  // 0x1954f4..0x195521
//         CDrawingManager(*pDC).FillGradient(rect, this->[+0x10c], this->[+0x21c], FALSE, 0, 0);   // 0x195562
//     else CMFCVisualManagerOfficeXP::OnHighlightRarelyUsedMenuItems(pDC, rect);                  // 0x19557f (0x1ae1f0)
// (the adjusted rect is what both branches receive.)
// DEVIATION: CMFCToolBar::GetMenuImageSize is a stub in this tree that never
// writes its CSize return buffer (phase4/src/featurepack/toolbar/CMFCToolBar.cpp),
// so its retail body is inlined here instead: it returns m_sizeMenuImage, or
// m_sizeImage when m_sizeMenuImage.cx == -1 (the `cmove` at 0x155a77), then
// scales both components by afxGlobalData's DPI ratio (+0x2b8) only when the
// flag at +0x2c0 is set (0x155aa3..0x155b74).  Both statics are exported by
// this tree (phase4/src/featurepack/toolbar/CMFCToolBar.cpp: m_sizeMenuImage
// {-1,-1}, m_sizeImage {16,15}) and are read here; OpenMFC's afxGlobalData is
// a zero blob, so the DPI flag is 0 and the scaling step is omitted, not
// approximated.  The OfficeXP fallback is a real body in
// CMFCVisualManagerOfficeXP.cpp.
// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOffice2003@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOffice2003__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect)
{
    if (pThis == nullptr || pDC == nullptr) return;
    rect.left--;
    const int nMargin = 3;                                           // vftable[185] -> `return 3`
    const int cxMenuImage = (impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cx == -1)   // GetMenuImageSize().cx, see above
        ? impl__m_sizeImage_CMFCToolBar__1VCSize__A.cx
        : impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cx;
    rect.right = rect.left + 2 * nMargin + cxMenuImage + 2;
    if (Office2003_RichColorDesktop()) {
        const O2003Theme t = O2003_Theme(pThis);
        O2003_FillGradient(pDC, rect, t.xp.c10c, t.c21c, FALSE, 0, 0);
        return;
    }
    impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(pThis, pDC, rect);
}

// CMFCVisualManagerOffice2003::OnUpdateSystemColors() -- retail mfc140 RVA
// 0x190b30 (0xf50 bytes).  Its control flow is:
//     CMFCBaseVisualManager::UpdateSystemColors();                 // 0x190b51 (0x18da80)
//     style = GetStandardWindowsTheme() (vftable +0x60), forced to -1 by the
//       m_bUseGlobalTheme == 0 probe and to 0 by m_bDefaultWinXPColors == 0;   // 0x190b59..0x190bc9
//     this->[+0x210] = style; this->[+0x2e8] = (unsigned)(style - 1) <= 2;    // 0x190bae / 0x190be0
//     ModifyGlobalColors() (vftable +0x658);                       // 0x190bf0
//     CMFCVisualManagerOfficeXP::OnUpdateSystemColors();           // 0x190bf9 (0x1ac0e0)
//     ... the palette stores at +0x214..+0x294 and the CBrush/CPen members,
//         which O2003_Theme() above transcribes ...                // 0x190bfe..0x191a58
// OpenMFC's object has no storage for any of those members, so the palette is
// recomputed by O2003_Theme() on every read instead of being cached here; this
// body performs the three calls retail makes around the stores.  None of the
// three does anything observable in this tree today (CMFCBaseVisualManager::
// UpdateSystemColors has no HTHEME storage to fill, the OfficeXP body is empty
// because that class recomputes its palette on demand too, and
// ModifyGlobalColors is gated off), and ModifyGlobalColors is called directly
// rather than through a vftable that OpenMFC's object does not have (see the
// O2003_FillHighlightedArea comment).
// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerOffice2003@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOffice2003__UEAAXXZ(
    CMFCVisualManagerOffice2003* pThis)
{
    if (pThis == nullptr) return;
    impl__UpdateSystemColors_CMFCBaseVisualManager__IEAAXXZ(pThis);
    impl__ModifyGlobalColors_CMFCVisualManagerOffice2003__MEAAXXZ(pThis);
    impl__OnUpdateSystemColors_CMFCVisualManagerOfficeXP__MEAAXXZ(pThis);
    // Palette: recomputed on demand by O2003_Theme(); nothing to store.
}

// CMFCVisualManagerOffice2003::SetDefaultWinXPColors(BOOL b) -- retail mfc140
// RVA 0x193a60, complete body:
//     m_bDefaultWinXPColors = b;                              // 0x193a64
//     CMFCVisualManager::GetInstance()->OnUpdateSystemColors();// 0x193a6a..0x193a7c
//     CMFCVisualManager::GetInstance();                        // 0x193a82
//     CMFCVisualManager::RedrawAll();                          // tail jmp 0x193a8b
// The static it writes is at image address 0x1803aab84 (mfc140), exported as
// ?m_bDefaultWinXPColors@CMFCVisualManagerOffice2003@@1HA.  The call at
// 0x193a6a is the internal CMFCVisualManager::GetInstance helper at 0x97f4
// (it allocates a 0x108-byte CMFCVisualManager, sets its +0x104 to 1 and
// dispatches vftable slot 14); the `mov 0x70(%rcx),%rax` at 0x193a75 is that
// same slot 14, which the Office2003 vftable at 0x18031a5c0 shows is
// OnUpdateSystemColors.  The redundant second GetInstance call is retail's.
//
// DEVIATIONS, both forced by what OpenMFC exports:
//  * GetInstance is not an entry point of this DLL (retail's is the internal
//    helper at 0x97f4, not an export), so there is no impl__ thunk to reach it
//    through from here.  The exported CMFCVisualManager::m_pVisManager static
//    is read instead, which means the manager singleton is NOT created here if
//    it does not exist yet -- retail's GetInstance would create it.
//  * OnUpdateSystemColors is dispatched virtually in retail; OpenMFC has no
//    vtable to dispatch through, so the CMFCVisualManager thunk is called
//    directly and a derived manager's override is not reached.
// Symbol: ?SetDefaultWinXPColors@CMFCVisualManagerOffice2003@@SAXH@Z
extern "C" void MS_ABI impl__SetDefaultWinXPColors_CMFCVisualManagerOffice2003__SAXH_Z(int bDefaultWinXPColors)
{
    impl__m_bDefaultWinXPColors_CMFCVisualManagerOffice2003__1HA = bDefaultWinXPColors;
    if (impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA != nullptr) {
        impl__OnUpdateSystemColors_CMFCVisualManager__UEAAXXZ();
    }
    impl__RedrawAll_CMFCVisualManager__SAXXZ();
}

// CMFCVisualManagerOffice2003::SetStatusBarOfficeXPLook(BOOL b) -- retail
// mfc140 RVA 0x193a40, complete body:
//     m_bStatusBarOfficeXPLook = b;                 // 0x193a44 -> 0x1803aab80
//     CMFCVisualManager::GetInstance();             // 0x193a4a
//     CMFCVisualManager::RedrawAll();               // tail jmp 0x193a53
// Note this one does NOT call OnUpdateSystemColors, unlike its two siblings.
// The GetInstance call is dropped here for the reason given on
// SetDefaultWinXPColors above; its only effect in retail is to create the
// manager singleton on first use.
// Symbol: ?SetStatusBarOfficeXPLook@CMFCVisualManagerOffice2003@@SAXH@Z
extern "C" void MS_ABI impl__SetStatusBarOfficeXPLook_CMFCVisualManagerOffice2003__SAXH_Z(int bStatusBarOfficeXPLook)
{
    impl__m_bStatusBarOfficeXPLook_CMFCVisualManagerOffice2003__1HA = bStatusBarOfficeXPLook;
    impl__RedrawAll_CMFCVisualManager__SAXXZ();
}

// CMFCVisualManagerOffice2003::SetUseGlobalTheme(BOOL b) -- retail mfc140 RVA
// 0x193a10; same shape as SetDefaultWinXPColors above, writing the static at
// image address 0x1803aab88 (?m_bUseGlobalTheme@CMFCVisualManagerOffice2003@@1HA):
//     m_bUseGlobalTheme = b;                                    // 0x193a14
//     CMFCVisualManager::GetInstance()->OnUpdateSystemColors();  // 0x193a1a..0x193a2c
//     CMFCVisualManager::GetInstance();                          // 0x193a32
//     CMFCVisualManager::RedrawAll();                            // tail jmp 0x193a3b
// Same two deviations as SetDefaultWinXPColors above.
// Symbol: ?SetUseGlobalTheme@CMFCVisualManagerOffice2003@@SAXH@Z
extern "C" void MS_ABI impl__SetUseGlobalTheme_CMFCVisualManagerOffice2003__SAXH_Z(int bUseGlobalTheme)
{
    impl__m_bUseGlobalTheme_CMFCVisualManagerOffice2003__1HA = bUseGlobalTheme;
    if (impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA != nullptr) {
        impl__OnUpdateSystemColors_CMFCVisualManager__UEAAXXZ();
    }
    impl__RedrawAll_CMFCVisualManager__SAXXZ();
}
