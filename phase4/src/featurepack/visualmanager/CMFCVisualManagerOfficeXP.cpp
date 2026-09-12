// CMFCVisualManagerOfficeXP — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void*, const CRect& rect, int nItem, int nState);

// ---------------------------------------------------------------------------
// CMFCVisualManagerOfficeXP -- Office XP ("flat") chrome.
//
// The bodies below were transcribed from the retail mfc140.dll shipped with
// MSVC 14.51.36231 (x64, MSVC ABI); every implemented entry cites the RVA of
// the function entry it came from.  Two pieces of retail state recur:
//
//  * afxGlobalData (retail `?afxGlobalData@@3UAFX_GLOBAL_DATA@@A`, image
//    address 0x1803ba380).  Retail reads its COLORREF fields after a guarded
//    `afxGlobalData.Initialize()` (the guard word is m_bInitialized at +0x00).
//    AFX_GLOBAL_DATA::UpdateSysColors (RVA 0x6afd0) fills every one of those
//    fields directly from ::GetSysColor, and disassembling it pins both the
//    field->COLOR_* mapping used by the GD_* helpers below and the struct
//    layout (its GetDeviceCaps(BITSPIXEL) result is stored at +0x288, which is
//    AFX_GLOBAL_DATA::m_nBitsPerPixel).  OpenMFC exports afxGlobalData as a
//    zero-filled blob (phase4/src/featurepack/CMFC_misc_stubs.cpp) whose
//    Initialize() is a no-op, so reading it would yield black; the helpers
//    call ::GetSysColor instead.  This is the same substitution the already
//    implemented CMFCVisualManager colour getters make -- see the header
//    comment of CMFCVisualManager.cpp in this directory.
//
//  * CMFCVisualManagerOfficeXP's own colour/brush/pen members at +0x108..+0x1f0,
//    computed by CMFCVisualManagerOfficeXP::OnUpdateSystemColors (RVA 0x1ac0e0).
//    OpenMFC's class is `CMFCVisualManager` plus `char _pad[64]` and has no
//    storage at those offsets, so nothing can be read back from the object.
//    Instead XP_Colors() below recomputes the colour members the way
//    OnUpdateSystemColors derives them; that function's own comment states
//    exactly which retail inputs are reproduced and which one (the +0x190 brush
//    colour) is not.  Bodies that need one of the GDI objects built from those
//    colours create an equivalent object locally and say so.
//
// Recurring retail helpers referenced below, resolved from the RVA map:
//    0x2a3b00  CDC::Draw3dRect(LPCRECT, COLORREF clrTopLeft, COLORREF clrBottomRight)
//    0x2a3a60  CDC::FillSolidRect(int x, int y, int cx, int cy, COLORREF)
//    0x8fd40   CMenuImages::Draw(CDC*, IMAGES_IDS, const CRect&, IMAGE_STATE, const CSize&)
//    0x5b0c0   CDrawingManager::PixelAlpha(COLORREF, int nPercent)
//    0x2c5bd0  __guard_dispatch_icall_fptr (CFG); `mov <slot>,%rax; call *0x2c5bd0`
//              is an ordinary indirect/virtual call to the value in %rax.
//              (Confirmed against the PE load-config directory's
//              GuardCFDispatchFunctionPointer field, which holds 0x1802c5bd0.)
//
// Several bodies below call back into the visual manager through `this`'s own
// vtable.  Those slots were read out of the retail vtables -- CMFCVisualManager's
// at image address 0x180319f78 (installed by ??0CMFCVisualManager@@QEAA@H@Z,
// 0x182640) and CMFCVisualManagerOfficeXP's at 0x18031b3d8 (installed by
// ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z, 0x1abe10) -- and are, for the OfficeXP
// vtable:
//    +0x3c8 (slot 121)  CMFCVisualManagerOfficeXP::OnFillPopupWindowBackground (0x1b1ab0)
//    +0x620 (slot 196)  CMFCVisualManagerOfficeXP::CreateGripperBrush          (0x1af590)
//    +0x628 (slot 197)  CMFCVisualManagerOfficeXP::ExtendMenuButton            (0x1af6d0)
//    +0x630 (slot 198)  CMFCVisualManagerOfficeXP::GetWindowColor              (0x1b17a0)
//    +0x638 (slot 199)  CMFCVisualManagerOfficeXP::OnFillHighlightedArea       (0x1b1400)
// Bodies below that need one of these do NOT dispatch through the object's
// vptr: OpenMFC's CMFCVisualManagerOfficeXP is a mingw-built class whose
// Itanium-ABI vtable comes from the virtual list in include/openmfc/afxmfc.h
// and has nothing at those MSVC slot indices.  They call the file-local helper
// that carries the same body instead (XP_FillHighlighted,
// XP_FillPopupWindowBackground), which deviates from retail only for a class
// derived from CMFCVisualManagerOfficeXP that overrides the virtual.
// ExtendMenuButton is still a stub (it needs CMFCToolBarMenuButton::m_pPopupMenu,
// see its comment); its two callers, OnFillButtonInterior and
// OnDrawButtonBorder, are implemented with that dropped-down arm documented as
// not taken.
//
// Member names used in the comments below come from the MSVC
// afxvisualmanagerofficexp.h shipped with this toolset, laid over the retail
// offsets: the OfficeXP members start at +0x108 (== the retail sizeof of
// CMFCVisualManager) in declaration order -- m_clrBarBkgnd +0x108,
// m_clrMenuRarelyUsed +0x10c, m_clrMenuLight +0x110, m_clrInactiveTabText +0x114,
// m_clrHighlight +0x118, m_clrHighlightDn +0x11c, m_clrHighlightChecked +0x120,
// m_clrPressedButtonBorder +0x124, m_clrGripper +0x128, m_clrSeparator +0x12c,
// m_clrPaneBorder +0x130, m_clrMenuBorder +0x134, m_clrMenuItemBorder +0x138,
// then the 16-byte CBrush objects m_brGripperHorz +0x140, m_brGripperVert +0x150,
// m_brBarBkgnd +0x160, m_brMenuRarelyUsed +0x170, m_brMenuLight +0x180,
// m_brTabBack +0x190, m_brHighlight +0x1a0, m_brHighlightDn +0x1b0,
// m_brHighlightChecked +0x1c0, m_brFloatToolBarBorder +0x1d0, the CPens
// m_penSeparator +0x1e0 and m_penMenuItemBorder +0x1f0, and the BOOLs
// m_bConnectMenuToParent +0x200, m_bShdowDroppedDownMenuButton +0x204,
// m_bDrawLastTabLine +0x208 (all three set to 1 by the retail ctor at
// 0x1abeed / 0x1abf4f / 0x1abf5b).  Every brush except m_brTabBack and
// m_brFloatToolBarBorder is ::CreateSolidBrush of the colour member with the
// same suffix (the run at 0x1ac8ed..0x1ac98a inside OnUpdateSystemColors);
// those two take the HLS-derived colour XP_Colors() leaves unmodelled and the
// value it returns as c1d0 (0x1ac991 / 0x1ac9c9).  That is why XP_Colors() can
// stand in for the brushes.
// ---------------------------------------------------------------------------

// Implementations in sibling translation units that this unit calls.
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKH_Z(unsigned long srcPixel, int nPercent);
extern "C" void MS_ABI impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(unsigned long rgb, double* pH, double* pS, double* pL);
// CMenuImages::Draw(CDC*, IMAGES_IDS, const CRect&, IMAGE_STATE, const CSize&).
// Defined -- as an empty stub with an auto-generated parameter list that does
// NOT match the mangled name -- in phase4/src/featurepack/controls/CMFCReBar.cpp.
// This declaration is the one derived from the mangled name; see the
// headerRequest that goes with this file.
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int nImageID, const CRect& rect, int nImageState, const CSize& sizeImage);
// CMenuImages::Draw(CDC*, IMAGES_IDS, const CPoint&, IMAGE_STATE, const CSize&)
// -- the other overload, same situation (empty stub with a garbage parameter
// list in CMFCReBar.cpp; CMFCVisualManagerOffice2003.cpp declares it this way too).
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int nImageID, const CPoint& pt, int nImageState, const CSize& sizeImage);
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* pList);
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect* pRect);
// CMFCToolBarEditBoxButton::m_bFlat (retail static at image address 0x1803aab78).
extern "C" std::int32_t impl__m_bFlat_CMFCToolBarEditBoxButton__1HA;
// CMFCToolBar::m_bCustomizeMode (retail static at image address 0x1803b70bc);
// OpenMFC defines it in phase4/src/featurepack/toolbar/StaticData.cpp.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
// CMFCToolBarImages::m_bIsDrawOnGlass (retail static at image address
// 0x1803b70f4); OpenMFC defines it in phase4/src/featurepack/toolbar/StaticData.cpp.
extern "C" std::int32_t impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA;
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CMFCToolBarButton* pButton, int nState);

// Base-class bodies the OfficeXP overrides below fall back to (all defined in
// CMFCVisualManager.cpp / Thunks.cpp in this directory).
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int bIsPressed, int bIsHighlighted, int bIsDisabled, int bHasDropDownArrow, int bIsSysButton);
extern "C" void MS_ABI impl__OnDrawCaptionBarButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManager* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int bIsPressed, int bIsHighlighted, int bIsDisabled, int bHasDropDownArrow, int bIsSysButton);
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
    CMFCVisualManager* pThis, CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize);
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar);
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz);
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int bNCArea);
// Sibling classes.
extern "C" void* MS_ABI impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(CMFCPopupMenuBar* pThis);
extern "C" CFont* MS_ABI impl__GetMenuFont_CMFCMenuBar__SAAEBVCFont__H_Z(int bHorz);
// CRuntimeClass descriptors the IsKindOf / IsDerivedFrom tests below use.  Each
// is a real definition in the RuntimeClasses.cpp of its own subsystem (or in the
// class's own .cpp), located with grep before being declared here.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAutoHideDockSite__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ();
// CMFCToolBar::m_bAltCustomizeMode (retail static at image address 0x1803b70cc);
// defined in phase4/src/featurepack/toolbar/StaticData.cpp.
extern "C" std::int32_t impl__m_bAltCustomizeMode_CMFCToolBar__1HA;
// CMFCToolBar::m_sizeMenuImage / m_sizeImage (retail statics at image addresses
// 0x1803aacb8 / 0x1803aae38), the two inputs of CMFCToolBar::GetMenuImageSize
// (0x155a60).  Defined in phase4/src/featurepack/toolbar/CMFCToolBar.cpp as a
// { long cx; long cy; } pair; the same shape is used here.
struct XP_StaticCSize { long cx; long cy; };
extern "C" XP_StaticCSize impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A;
extern "C" XP_StaticCSize impl__m_sizeImage_CMFCToolBar__1VCSize__A;

namespace {

// AFX_GLOBAL_DATA COLORREF fields, named by their retail offsets and mapped to
// the ::GetSysColor index that AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0)
// stores into each of them.
inline COLORREF GD_clrBtnFace()     { return ::GetSysColor(COLOR_BTNFACE); }      // +0x28, and clrBarFace +0x60
inline COLORREF GD_clrBtnShadow()   { return ::GetSysColor(COLOR_BTNSHADOW); }    // +0x2c, and clrBarShadow +0x64
inline COLORREF GD_clrBtnText()     { return ::GetSysColor(COLOR_BTNTEXT); }      // +0x34, and clrBarText +0x74
inline COLORREF GD_clrBtnDkShadow() { return ::GetSysColor(COLOR_3DDKSHADOW); }   // +0x3c, and clrBarDkShadow +0x6c
inline COLORREF GD_clrGrayedText()  { return ::GetSysColor(COLOR_GRAYTEXT); }     // +0x44
inline COLORREF GD_clrWindow()      { return ::GetSysColor(COLOR_WINDOW); }       // +0x78, and clrBarWindow +0x5c
inline COLORREF GD_clrCaptionText() { return ::GetSysColor(COLOR_CAPTIONTEXT); }  // +0x80
inline COLORREF GD_clrBarHilite()   { return ::GetSysColor(COLOR_BTNHIGHLIGHT); } // +0x68, and clrBtnHilite +0x30
inline COLORREF GD_clrBarLight()    { return ::GetSysColor(COLOR_3DLIGHT); }      // +0x70, and clrBtnLight +0x40

// afxGlobalData.m_nBitsPerPixel (+0x288).  AFX_GLOBAL_DATA::UpdateSysColors
// (0x6afd0) fills it from a ::GetDeviceCaps(<screen DC>, BITSPIXEL) call at
// 0x6b052, stored at 0x6b058.
inline int GD_BitsPerPixel()
{
    HDC hdcScreen = ::GetDC(nullptr);
    if (hdcScreen == nullptr) return 32;
    int bpp = ::GetDeviceCaps(hdcScreen, BITSPIXEL);
    ::ReleaseDC(nullptr, hdcScreen);
    return bpp;
}

inline COLORREF GD_clrWindowText()  { return ::GetSysColor(COLOR_WINDOWTEXT); }  // +0x7c
inline COLORREF GD_clrTextHilite()  { return ::GetSysColor(COLOR_HIGHLIGHTTEXT); }// +0x4c
inline COLORREF GD_clrHilite()      { return ::GetSysColor(COLOR_HIGHLIGHT); }   // +0x48

// afxGlobalData's two high-contrast flags, +0x260 and +0x264.
// AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0) computes both from ::GetSysColor
// alone -- the sequences at 0x6afdd..0x6b00d and 0x6b013..0x6b035 -- so unlike
// the OfficeXP colour members they can be reproduced exactly here.
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

// ---------------------------------------------------------------------------
// The CMFCVisualManagerOfficeXP colour members at +0x108..+0x138 (and the
// +0x1d0 brush colour), as CMFCVisualManagerOfficeXP::OnUpdateSystemColors
// (retail RVA 0x1ac0e0) derives them.
//
// Retail caches them in the object and builds one GDI object per colour at
// +0x160..+0x1f0 (the ::CreateSolidBrush / ::CreatePen run at 0x1ac8ed..0x1aca1f).
// OpenMFC's CMFCVisualManagerOfficeXP is `CMFCVisualManager` plus
// `char _pad[64]` and has no storage there, so XP_Colors() recomputes the whole
// set on demand instead.  Every input OnUpdateSystemColors reads is available:
//   * the afxGlobalData colour fields it reads are each assigned from exactly
//     one ::GetSysColor call by AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0), so
//     the GD_* helpers above return the same values;
//   * the one virtual it calls -- vtable byte offset 0x630, the call at
//     0x1ac1d1 -- is CMFCVisualManagerOfficeXP::GetWindowColor, read out of the
//     retail OfficeXP vtable at image address 0x18031b3d8, and that returns
//     afxGlobalData.clrWindow (+0x78), i.e. GD_clrWindow();
//   * CDrawingManager::PixelAlpha(COLORREF,int) (0x5b0c0) and
//     CDrawingManager::RGBtoHSL (0x5b710) are really implemented in
//     phase4/src/core/gdi/CDrawingManager.cpp; the comments there record
//     0/65416 and 0/140608 mismatches against the retail DLL over the sweeps
//     they ran.
//
// Two deliberate differences from retail, neither of which changes a value on a
// desktop whose system colours are not being changed underneath it:
//   * the set is recomputed per call rather than cached at OnUpdateSystemColors
//     time, so a colour-scheme change takes effect immediately here and only at
//     the next OnUpdateSystemColors in retail;
//   * retail dispatches GetWindowColor virtually, so a class derived from
//     CMFCVisualManagerOfficeXP that overrode it would shift the whole palette;
//     XP_Colors() always uses the OfficeXP value.
//
// NOT modelled, and therefore absent from XPColors: the colour behind the brush
// at +0x190.  On the low-colour path it is just afxGlobalData.clrBtnFace
// (0x1ac7bc), but on the gradient path it is a hue/saturation-adjusted
// clrBarFace computed with CDrawingManager::RGBtoHSL plus HLStoRGB_ONE
// (0x5b4a0) over 0x1ac298..0x1ac330, and HLStoRGB_ONE is still a `return 0`
// stub in this tree.  Nothing in this file needs it.
// ---------------------------------------------------------------------------
struct XPColors
{
    COLORREF c108;   // bar background        -> brush +0x160
    COLORREF c10c;   //                       -> brush +0x170
    COLORREF c110;   // menu/popup background -> brush +0x180
    COLORREF c114;
    COLORREF c118;   // highlight fill        -> brush +0x1a0
    COLORREF c11c;   // pressed fill          -> brush +0x1b0
    COLORREF c120;   //                       -> brush +0x1c0
    COLORREF c128;
    COLORREF c12c;   // separator colour      -> pen   +0x1e0
    COLORREF c130;
    COLORREF c134;
    COLORREF c138;   // border colour         -> pen   +0x1f0
    COLORREF c1d0;   //                       -> brush +0x1d0
};

// Per channel: (wa*a + wb*b) / 255.  Retail does the division with
// `mul $0x80808081; shr $7` on each channel, which is an exact unsigned /255.
inline COLORREF XP_Blend255(COLORREF a, unsigned wa, COLORREF b, unsigned wb)
{
    const unsigned r = (wa * (unsigned)GetRValue(a) + wb * (unsigned)GetRValue(b)) / 255u;
    const unsigned g = (wa * (unsigned)GetGValue(a) + wb * (unsigned)GetGValue(b)) / 255u;
    const unsigned bl= (wa * (unsigned)GetBValue(a) + wb * (unsigned)GetBValue(b)) / 255u;
    return RGB(r & 0xffu, g & 0xffu, bl & 0xffu);
}

// Per channel: (5*a + b) / 6.  Retail uses `mul $0xaaaaaaab; shr $2`, an exact
// unsigned /6 over this range (the sequence at 0x1ac545..0x1ac59f).
inline COLORREF XP_Mix5to1(COLORREF a, COLORREF b)
{
    const unsigned r = (5u * (unsigned)GetRValue(a) + (unsigned)GetRValue(b)) / 6u;
    const unsigned g = (5u * (unsigned)GetGValue(a) + (unsigned)GetGValue(b)) / 6u;
    const unsigned bl= (5u * (unsigned)GetBValue(a) + (unsigned)GetBValue(b)) / 6u;
    return RGB(r, g, bl);
}

// CDrawingManager::PixelAlpha(COLORREF, double, double, double) (retail 0x5b160)
// with the same factor on all three channels: v = (int)(channel * k + 0.5),
// clamped high at 255.  Its "channel == 0 && k > 1.0" special case (0x5b18f)
// cannot trigger for the k < 1 factors used below.  Transcribed here rather than
// called because OpenMFC's ?PixelAlpha@CDrawingManager@@SAKKNNN@Z is a
// `return 0` stub (phase4/src/core/gdi/CDrawingManager.cpp).
inline COLORREF XP_PixelAlphaF(COLORREF c, double k)
{
    int r = (int)((double)GetRValue(c) * k + 0.5); if (r > 255) r = 255;
    int g = (int)((double)GetGValue(c) * k + 0.5); if (g > 255) g = 255;
    int b = (int)((double)GetBValue(c) * k + 0.5); if (b > 255) b = 255;
    return RGB(r, g, b);
}

XPColors XP_Colors()
{
    XPColors x;

    const COLORREF clrBarFace   = GD_clrBtnFace();     // +0x28 == +0x60
    const COLORREF clrWindow    = GD_clrWindow();      // GetWindowColor(), vtable +0x630
    const COLORREF clrHilite    = GD_clrHilite();      // +0x48
    const COLORREF clrBarShadow = GD_clrBtnShadow();   // +0x2c == +0x64
    const bool bWhiteHC = GD_IsWhiteHighContrast();    // afxGlobalData +0x260
    const bool bBlackHC = GD_IsBlackHighContrast();    // afxGlobalData +0x264

    if (GD_BitsPerPixel() > 8 && !bWhiteHC && !bBlackHC)
    {
        // Gradient path, 0x1ac1c4..0x1ac682.  Taken when m_nBitsPerPixel > 8 and
        // neither high-contrast flag is set (the three tests at 0x1ac18a /
        // 0x1ac1ac / 0x1ac1b8).
        x.c110 = XP_Blend255(clrWindow, 0xdb, clrBarFace, 0x24);   // store 0x1ac289
        x.c108 = XP_Blend255(clrBarFace, 0xd7, clrWindow, 0x28);   // store 0x1ac3ac
        x.c10c = impl__PixelAlpha_CDrawingManager__SAKKH_Z(x.c108, 0x5e);      // 0x1ac3bd
        x.c114 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrBarFace, 0x37);  // 0x1ac3cd

        // 0x1ac42e..0x1ac499: a 0xb2/0x4d blend of +0x110 with clrHilite.
        const COLORREF clrMix = XP_Blend255(x.c110, 0xb2, clrHilite, 0x4d);

        // 0x1ac414 + 0x1ac49b: RGBtoHSL(clrHilite) and a comparison of L
        // against the double 0.8 held at image address 0x180348b58.
        double dH = 0.0, dS = 0.0, dL = 0.0;
        impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(clrHilite, &dH, &dS, &dL);

        if (dL > 0.8)
        {
            x.c118 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrMix, 0x5b);     // 0x1ac4b6
            x.c11c = impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrMix, 0x62);     // 0x1ac4c3
            x.c138 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrHilite, 0x54);  // 0x1ac4ef/0x1ac539
        }
        else
        {
            x.c118 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrMix, 0x66);     // 0x1ac505
            x.c11c = impl__PixelAlpha_CDrawingManager__SAKKH_Z(x.c118, 0x57);     // 0x1ac512
            x.c138 = clrHilite;                                                   // 0x1ac533/0x1ac539
        }

        // 0x1ac545..0x1ac5a6.  The trailing PixelAlpha takes 0x64 == 100, which
        // is the identity, so only the (5*[0x110] + clrHilite)/6 mix survives.
        x.c120 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(XP_Mix5to1(x.c110, clrHilite), 0x64);

        x.c12c = XP_PixelAlphaF(clrBarFace, 0.86);                                // 0x1ac5e6
        x.c130 = clrBarShadow;                                                    // 0x1ac60f
        x.c134 = x.c114;                                                          // 0x1ac615, ebp still holding c114
        x.c128 = impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrBarShadow, 0x6e);   // 0x1ac673/0x1ac67c
        // 0x1ac64c reassigns ebp -- the register the +0x1d0 brush is later made
        // from at 0x1ac9c9 -- to PixelAlpha(clrBarShadow, 0.85, 0.85, 0.85).
        x.c1d0 = XP_PixelAlphaF(clrBarShadow, 0.85);
    }
    else
    {
        // Low-colour / high-contrast path, 0x1ac687..0x1ac8e7.
        x.c110 = clrWindow;      // 0x1ac6a0
        x.c108 = clrBarFace;     // 0x1ac6c3
        if (bWhiteHC)
        {
            // afxGlobalData +0x260 set; branch decided at 0x1ac6e4.
            x.c118 = x.c11c = x.c120 = clrHilite;   // 0x1ac705/0x1ac70b/0x1ac711
            x.c10c = clrBarFace;                    // 0x1ac72e -> 0x1ac79b
        }
        else
        {
            x.c118 = x.c11c = clrBarFace;   // 0x1ac74f/0x1ac755
            x.c120 = clrWindow;             // 0x1ac778
            x.c10c = GD_clrBarLight();      // +0x70, 0x1ac795 -> 0x1ac79b
        }
        x.c114 = GD_clrBtnDkShadow();       // +0x3c, 0x1ac7db
        x.c12c = clrBarShadow;              // 0x1ac7fe
        x.c128 = clrBarShadow;              // 0x1ac821
        x.c130 = clrBarShadow;              // 0x1ac844
        x.c134 = GD_clrBtnDkShadow();       // 0x1ac867
        x.c1d0 = clrBarShadow;              // ebp at 0x1ac888, brush at 0x1ac9c9
        // 0x1ac8a3..0x1ac8e7: clrHilite unless a high-contrast flag is set.
        x.c138 = (!bBlackHC && !bWhiteHC) ? clrHilite : GD_clrBtnDkShadow();
    }

    return x;
}

// CMFCVisualManagerOfficeXP's border colour, retail member +0x138.  It is also
// the colour of the 1px separator pen at +0x1f0, which OnUpdateSystemColors
// creates from it with the ::CreatePen call at 0x1ac9f7.
inline COLORREF XPBorderColor() { return XP_Colors().c138; }

// CMFCVisualManagerOfficeXP's highlight fill colour, retail member +0x118; the
// brush at +0x1a0 is ::CreateSolidBrush of it, made by the call at 0x1ac93e.
inline COLORREF XPHighlightColor() { return XP_Colors().c118; }

// CMFCVisualManagerOfficeXP's bar background, retail member +0x108; the brush
// at +0x160 is ::CreateSolidBrush of it, made at 0x1ac8ed.
inline COLORREF XPBarBkgndColor() { return XP_Colors().c108; }

// CMFCToolBarButton::IsEditable() -- the virtual retail dispatches at 0x1b05b4
// (vtable byte offset 0xd0, slot 26).  It is not exported, so it is transcribed
// here from the retail body itself rather than dispatched to.
//
// Do NOT dispatch this through the object's vptr.  The MSVC-shaped 53-entry
// vtable in phase4/src/detail/CMFCToolBarButtonSupport.cpp is never installed on
// anything: OpenMFC_PatchToolBarButtonVtable has no caller anywhere in the tree
// (its own header comment claiming "called from the exported ctors" is stale),
// and the exported constructor -- impl___0CMFCToolBarButton__QEAA_XZ in
// phase4/src/featurepack/toolbar/Thunks.cpp -- placement-news a mingw-built
// CMFCToolBarButton, whose Itanium-ABI vtable has six entries
// (GetRuntimeClass, the two dtor slots, Serialize, AssertValid, Dump).  Reading
// index 26 of that would be 208 bytes past the end of the vtable and calling
// the result would be an indirect call into unrelated data.
//
// Retail's slot-26 body is at 0x23820 (reached from the CMFCToolBarButton vtable
// at image address 0x180313e98, which ??0CMFCToolBarButton@@QEAA@XZ installs at
// 0x15a530).  It reads only m_nID (+0x24) and returns FALSE for the reserved
// command-ID ranges, TRUE otherwise:
//     UINT id = m_nID;                                  // 0x23824
//     if (id - 0xe110 <= 0x0f)  return FALSE;           // 0x23827..0x23830
//     if (id - 0xf000 <= 0x1ef) return FALSE;           // 0x23832..0x2383d
//     if ((int)id >= 0xff00)    return FALSE;           // 0x2383f..0x23845
//     if (id - 0xe210 <= 0x0f)  return FALSE;           // 0x23847..0x23850
//     ... two further "reserved command" lookups (a global object at 0x3b7110
//     whose +0x40 field is compared against id, and a list walk over the global
//     at 0x3ab0c8 via the helper at 0x11f3c) ...
//     return TRUE;                                      // 0x23874
// The two global lookups are omitted: OpenMFC has neither table, and both can
// only turn a TRUE into a FALSE, so omitting them cannot make a button that
// retail treats as editable be treated as non-editable here.
// (m_nID is at +0x24 == 36 in OpenMFC's CMFCToolBarButton too -- see the
// harvested layout in include/openmfc/afxmfc.h.)
inline int XP_Button_IsEditable(const CMFCToolBarButton* pButton)
{
    if (pButton == nullptr) return 0;
    const unsigned int id = pButton->m_nID;
    if (static_cast<unsigned int>(id - 0xe110u) <= 0x0fu)  return 0;
    if (static_cast<unsigned int>(id - 0xf000u) <= 0x1efu) return 0;
    if (static_cast<int>(id) >= 0xff00)                    return 0;
    if (static_cast<unsigned int>(id - 0xe210u) <= 0x0fu)  return 0;
    return 1;
}

inline HDC XP_Hdc(CDC* pDC) { return pDC ? pDC->GetSafeHdc() : nullptr; }

inline RECT XP_ToRECT(const CRect& r) { return RECT{ r.left, r.top, r.right, r.bottom }; }

// One-pixel line, used for the pen-based rules retail draws with CDC::MoveTo /
// CDC::LineTo (0x2a1000 / 0x2a1060).
void XP_DrawLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF clr)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;
    HPEN pen = ::CreatePen(PS_SOLID, 1, clr);
    HGDIOBJ oldPen = (pen != nullptr) ? ::SelectObject(hdc, pen) : nullptr;
    ::MoveToEx(hdc, x1, y1, nullptr);
    ::LineTo(hdc, x2, y2);
    if (oldPen != nullptr) ::SelectObject(hdc, oldPen);
    if (pen != nullptr) ::DeleteObject(pen);
}

void XP_FillSolid(CDC* pDC, const CRect& rect, COLORREF clr)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr || rect.right <= rect.left || rect.bottom <= rect.top) return;
    RECT r = XP_ToRECT(rect);
    HBRUSH brush = ::CreateSolidBrush(clr);
    if (brush != nullptr) {
        ::FillRect(hdc, &r, brush);
        ::DeleteObject(brush);
    }
}

// CDC::Draw3dRect(LPCRECT, COLORREF clrTopLeft, COLORREF clrBottomRight)
// (retail 0x2a3b00, which tail-calls the x/y/cx/cy overload at 0x2a3b40; that
// one issues four CDC::FillSolidRect runs -- (x,y,cx-1,1) and (x,y,1,cy-1) in
// clrTopLeft, then (x+cx,y,-1,cy) and (x,y+cy,cx,-1) in clrBottomRight).  The
// four XP_DrawLine calls below cover exactly the same pixels.
void DC_Draw3dRect(CDC* pDC, const CRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
    if (rect.right <= rect.left || rect.bottom <= rect.top) return;
    XP_DrawLine(pDC, rect.left, rect.top, rect.right - 1, rect.top, clrTopLeft);
    XP_DrawLine(pDC, rect.left, rect.top, rect.left, rect.bottom - 1, clrTopLeft);
    XP_DrawLine(pDC, rect.right - 1, rect.top, rect.right - 1, rect.bottom, clrBottomRight);
    XP_DrawLine(pDC, rect.left, rect.bottom - 1, rect.right, rect.bottom - 1, clrBottomRight);
}

// Filled triangle standing in for the CMenuImages::Draw arrow glyphs, see the
// call sites for exactly which retail call it replaces.
void XP_DrawArrow(CDC* pDC, const CRect& rect, bool bDown, COLORREF clr)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr || rect.right <= rect.left || rect.bottom <= rect.top) return;
    const int cx = (rect.left + rect.right) / 2;
    const int cy = (rect.top + rect.bottom) / 2;
    POINT pts[3];
    if (bDown) {
        pts[0] = POINT{ cx - 4, cy - 2 };
        pts[1] = POINT{ cx + 4, cy - 2 };
        pts[2] = POINT{ cx, cy + 3 };
    } else {
        pts[0] = POINT{ cx - 4, cy + 2 };
        pts[1] = POINT{ cx + 4, cy + 2 };
        pts[2] = POINT{ cx, cy - 3 };
    }
    HBRUSH brush = ::CreateSolidBrush(clr);
    HPEN pen = ::CreatePen(PS_SOLID, 1, clr);
    HGDIOBJ oldBrush = (brush != nullptr) ? ::SelectObject(hdc, brush) : nullptr;
    HGDIOBJ oldPen = (pen != nullptr) ? ::SelectObject(hdc, pen) : nullptr;
    ::Polygon(hdc, pts, 3);
    if (oldPen != nullptr) ::SelectObject(hdc, oldPen);
    if (oldBrush != nullptr) ::SelectObject(hdc, oldBrush);
    if (pen != nullptr) ::DeleteObject(pen);
    if (brush != nullptr) ::DeleteObject(brush);
}

// CDC::FillSolidRect(int x, int y, int cx, int cy, COLORREF) (retail 0x2a3a60).
// Negative cx/cy are used by the callers below to mean "one pixel back from
// this edge", exactly as CDC::FillSolidRect handles them.
void DC_FillSolidRect(CDC* pDC, int x, int y, int cx, int cy, COLORREF clr)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;
    RECT r;
    r.left   = (cx >= 0) ? x : x + cx;
    r.right  = (cx >= 0) ? x + cx : x;
    r.top    = (cy >= 0) ? y : y + cy;
    r.bottom = (cy >= 0) ? y + cy : y;
    HBRUSH brush = ::CreateSolidBrush(clr);
    if (brush != nullptr) {
        ::FillRect(hdc, &r, brush);
        ::DeleteObject(brush);
    }
}

// CBasePane pane-style border bits (retail afxres.h values; the mask 0x0f00 is
// CBRS_BORDER_ANY, which the retail body below tests literally).
const DWORD kXPBorderLeft   = 0x0100;
const DWORD kXPBorderTop    = 0x0200;
const DWORD kXPBorderRight  = 0x0400;
const DWORD kXPBorderBottom = 0x0800;
const DWORD kXPBorderAny    = 0x0f00;

// SBPS_NOBORDERS (afxres.h); OnDrawStatusBarPaneBorder tests it literally.
const unsigned int kXPStatusNoBorders = 0x0100;

// Byte offsets into a CMFCDesktopAlertWndButton, taken from the shadow structs
// this repo already pins: m_bHover / m_bClickStarted from CMFCButton
// (phase4/src/detail/CMFCButtonSupport.h, S_Cmfcbutton) and m_bIsCaptionButton
// from phase4/src/detail/CMFCDesktopAlertWndButtonSupport.h, where it is the
// first member appended after the 2848-byte CMFCButton base.
const std::ptrdiff_t kXPBtnHover           = 0x114;
const std::ptrdiff_t kXPBtnClickStarted    = 0x11c;
const std::ptrdiff_t kXPAlertBtnIsCaption  = 2848;   // 0xb20

// CWnd::m_hWnd.  Retail reads it at 0x40 -- e.g. the `mov 0x40(%r9),%rcx`
// feeding ::GetParent at 0x1b19e3 inside OnErasePopupWindowButton -- and
// include/openmfc/afxwin.h pins CWnd::m_hWnd to the same offset 64.
const std::ptrdiff_t kXPWndHwnd = 0x40;

// The body of CMFCVisualManagerOfficeXP::OnFillHighlightedArea (retail RVA
// 0x1b1400), taking the brush handle directly.  The exported thunk and the
// three bodies below that retail routes through the vtable at +0x638 all use
// this.  Retail:
//     if (CMFCToolBarImages::m_bIsDrawOnGlass) {          // test at 0x1b1422
//         LOGBRUSH lb;
//         ::GetObject(pBrush->m_hObject, sizeof(LOGBRUSH), &lb);
//         CDrawingManager dm(*pDC);
//         dm.DrawRect(rect, lb.lbColor, (COLORREF)-1);    // 0x5abf0
//     } else {
//         ::FillRect(pDC->m_hDC, rect, pBrush ? pBrush->m_hObject : NULL);
//     }
// The CMFCToolBarButton* argument is not read on either path.
// Deviation: OpenMFC's ?DrawRect@CDrawingManager@@QEAAXAEBVCRect@@KK@Z is an
// empty stub (phase4/src/core/gdi/CDrawingManager.cpp), so the on-glass branch
// paints nothing there; this helper takes the same no-op rather than
// substituting the opaque fill, which is the very thing m_bIsDrawOnGlass exists
// to suppress.
void XP_FillHighlighted(CDC* pDC, const CRect& rect, HBRUSH hbr)
{
    if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA != 0) return;
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;
    RECT r = XP_ToRECT(rect);
    ::FillRect(hdc, &r, hbr);
}

void XP_FillHighlightedSolid(CDC* pDC, const CRect& rect, COLORREF clr)
{
    if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA != 0) return;
    HBRUSH hbr = ::CreateSolidBrush(clr);
    if (hbr == nullptr) return;
    XP_FillHighlighted(pDC, rect, hbr);
    ::DeleteObject(hbr);
}

// The body of CMFCVisualManagerOfficeXP::OnFillPopupWindowBackground (retail
// RVA 0x1b1ab0): ::FillRect(pDC->m_hDC, rect, this->[+0x180].m_hObject), where
// the +0x180 brush is ::CreateSolidBrush(this->[+0x110]) (the call at 0x1ac929).
void XP_FillPopupWindowBackground(CDC* pDC, const CRect& rect)
{
    XP_FillSolid(pDC, rect, XP_Colors().c110);
}

// The gripper brush that CMFCVisualManagerOfficeXP::CreateGripperBrush (retail
// RVA 0x1af590) attaches to the CBrush member at +0x140: ::CreatePatternBrush
// (0x1af612) of an 8x8 1bpp ::CreateBitmap (0x1af5fc, nWidth=8 nHeight=8
// nPlanes=1 nBitCount=1) whose 16 bytes of bits are four DWORDs of 0x000000ff
// (the stores at 0x1af5b6..0x1af5cb).  A 1bpp DDB scanline is WORD-padded, so
// those bytes are the rows ff 00 / 00 00 / ff 00 / ... -- alternating solid and
// empty rows.  (CreateGripperBrush also builds a second brush into +0x150 from
// four DWORDs of 0x00aa00aa, i.e. every row 10101010; nothing in this file uses
// it.)  OpenMFC has no storage at +0x140, so the caller builds and destroys the
// brush around its one use.
HBRUSH XP_CreateGripperBrush()
{
    static const BYTE bits[16] = { 0xff, 0x00, 0x00, 0x00,
                                   0xff, 0x00, 0x00, 0x00,
                                   0xff, 0x00, 0x00, 0x00,
                                   0xff, 0x00, 0x00, 0x00 };
    HBITMAP hbm = ::CreateBitmap(8, 8, 1, 1, bits);
    if (hbm == nullptr) return nullptr;
    HBRUSH hbr = ::CreatePatternBrush(hbm);
    ::DeleteObject(hbm);
    return hbr;
}


// ---------------------------------------------------------------------------
// Helpers added with the second batch of bodies.
// ---------------------------------------------------------------------------

// CMFCVisualManagerOfficeXP::GetMenuImageMargin() -- the virtual retail calls
// through vtable byte offset 0x5c8 (slot 185).  Slot 185 of the retail OfficeXP
// vtable (image address 0x18031b3d8) is the two-instruction body at 0xdbf0,
// `mov $3,%eax; ret`, and slot 185 of the base CMFCVisualManager vtable
// (0x180319f78) is 0xf4360, `mov $2,%eax; ret` -- matching the inline
// `GetMenuImageMargin() const { return 3; }` / `{ return 2; }` in the MSVC
// afxvisualmanagerofficexp.h / afxvisualmanager.h headers.  OpenMFC's
// CMFCVisualManager declares no such virtual, so the OfficeXP value is used
// directly; a class derived from CMFCVisualManagerOfficeXP that overrode it in
// retail would get its own value there and 3 here.
inline int XP_MenuImageMargin() { return 3; }

// CMFCToolBar::GetMenuImageSize() (retail 0x155a60): m_sizeMenuImage, or
// m_sizeImage when m_sizeMenuImage.cx == -1 (the cmove at 0x155a77), then both
// components scaled by afxGlobalData's ribbon image ratio (+0x2b8) when the
// flag at +0x2c0 is set, else by 1.0 (0x155aa3..0x155b74; the ucomisd against
// 1.0 at 0x155ac2 skips the scaling entirely for a 1.0 ratio).  OpenMFC's
// afxGlobalData is a zero-filled blob, so that flag reads 0 and the ratio is
// 1.0 -- the scaling step is therefore omitted here, not approximated.  Called
// locally because the exported thunk (phase4/src/featurepack/toolbar/
// CMFCToolBar.cpp) is a `return nullptr` stub that never writes the CSize.
inline SIZE XP_MenuImageSize()
{
    const XP_StaticCSize& src =
        (impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A.cx == -1)
            ? impl__m_sizeImage_CMFCToolBar__1VCSize__A
            : impl__m_sizeMenuImage_CMFCToolBar__1VCSize__A;
    return SIZE{ src.cx, src.cy };
}

// CMenuImages::Size() (retail 0x8fbb0): once CMenuImages is initialised the
// edge is 9 (0x8fbcd) scaled by the same afxGlobalData ratio as above (a 1.0
// ratio skips the multiply at 0x8fc0e); before initialisation it runs
// CMenuImages::Initialize (0x8f970), which stores 9 into the static size at
// 0x1803ba6c8 (0x8f9fc/0x8fa02) and returns that.  Either way 9 at 100%.  The
// exported ?Size@CMenuImages@@SA?AVCSize@@XZ is a `return nullptr` stub in
// phase4/src/featurepack/controls/CMFCReBar.cpp, so the constant is used.
inline int XP_MenuImageEdge() { return 9; }

// The solid colour each CMenuImages::IMAGE_STATE glyph set is re-tinted to by
// CMenuImages::Initialize (0x8f970): the CreateCopy calls at 0x8faa9 (Gray,
// 0x808080), 0x8faba (DkGray, 0x484848), 0x8facb (LtGray, 0xc0c0c0), 0x8fadc
// (White, 0xffffff) and 0x8faea (Black2, 0).  ImageBlack is the untinted
// source set, painted as black here exactly as OnDrawMenuScrollButton in this
// file already does.  Enum values from afxmenuimages.h: ImageBlack 0,
// ImageGray 1, ImageLtGray 2, ImageWhite 3, ImageDkGray 4, ImageBlack2 5.
inline COLORREF XP_MenuImageColor(int nState)
{
    switch (nState) {
    case 1:  return 0x00808080u;
    case 2:  return 0x00c0c0c0u;
    case 3:  return 0x00ffffffu;
    case 4:  return 0x00484848u;
    default: return 0x00000000u;
    }
}

// Stand-in for CMenuImages::Draw(pDC, id, rect, state, CSize(0,0)) (retail
// 0x8fd40) for the four arrow glyphs used by the bodies below --
// afxmenuimages.h IdArrowDown 0, IdArrowRight 1, IdArrowUp 7, IdArrowLeft 9 and
// IdArrowRightLarge 14 -- because the exported CMenuImages::Draw is an empty
// stub in this tree (see the declaration at the top of the file).  Any other id
// is handed to that stub so the call structure stays visible.
void XP_DrawMenuImage(CDC* pDC, int nImageID, const CRect& rect, int nState)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr || rect.right <= rect.left || rect.bottom <= rect.top) return;

    const int cx = (rect.left + rect.right) / 2;
    const int cy = (rect.top + rect.bottom) / 2;
    POINT pts[3];
    switch (nImageID) {
    case 0:   pts[0] = POINT{ cx - 4, cy - 2 }; pts[1] = POINT{ cx + 4, cy - 2 }; pts[2] = POINT{ cx, cy + 3 }; break; // IdArrowDown
    case 7:   pts[0] = POINT{ cx - 4, cy + 2 }; pts[1] = POINT{ cx + 4, cy + 2 }; pts[2] = POINT{ cx, cy - 3 }; break; // IdArrowUp
    case 1:
    case 14:  pts[0] = POINT{ cx - 2, cy - 4 }; pts[1] = POINT{ cx - 2, cy + 4 }; pts[2] = POINT{ cx + 3, cy }; break; // IdArrowRight / IdArrowRightLarge
    case 9:   pts[0] = POINT{ cx + 2, cy - 4 }; pts[1] = POINT{ cx + 2, cy + 4 }; pts[2] = POINT{ cx - 3, cy }; break; // IdArrowLeft
    default: {
        const CSize sizeImage(0, 0);
        impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
            pDC, nImageID, rect, nState, sizeImage);
        return;
    }
    }

    const COLORREF clr = XP_MenuImageColor(nState);
    HBRUSH brush = ::CreateSolidBrush(clr);
    HPEN pen = ::CreatePen(PS_SOLID, 1, clr);
    HGDIOBJ oldBrush = (brush != nullptr) ? ::SelectObject(hdc, brush) : nullptr;
    HGDIOBJ oldPen = (pen != nullptr) ? ::SelectObject(hdc, pen) : nullptr;
    ::Polygon(hdc, pts, 3);
    if (oldPen != nullptr) ::SelectObject(hdc, oldPen);
    if (oldBrush != nullptr) ::SelectObject(hdc, oldBrush);
    if (pen != nullptr) ::DeleteObject(pen);
    if (brush != nullptr) ::DeleteObject(brush);
}

// afxGlobalData.brLight (+0xc8).  AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0)
// builds it at 0x6b3c3..0x6b424: when m_nBitsPerPixel (+0x288) > 8 it is
// ::CreateSolidBrush of the per-channel midpoint
//     clrBtnFace + (clrBtnHilite - clrBtnFace) / 2     (signed, truncating)
// of the fields at +0x28 / +0x30 (COLOR_BTNFACE / COLOR_BTNHIGHLIGHT); at 8 bpp
// or less it is ::CreatePatternBrush of AFX_GLOBAL_DATA::CreateDitherBitmap
// (0x6b6c0), a dither of the same two colours.  Deviation: the low-colour
// dither is not reproduced -- the solid midpoint is used on both paths.
inline COLORREF GD_clrLightMidpoint()
{
    const COLORREF face = GD_clrBtnFace();
    const COLORREF hil  = GD_clrBarHilite();
    const int r = (int)GetRValue(face) + ((int)GetRValue(hil) - (int)GetRValue(face)) / 2;
    const int g = (int)GetGValue(face) + ((int)GetGValue(hil) - (int)GetGValue(face)) / 2;
    const int b = (int)GetBValue(face) + ((int)GetBValue(hil) - (int)GetBValue(face)) / 2;
    return RGB(r & 0xff, g & 0xff, b & 0xff);
}

// The luminance test retail applies to m_clrHighlight (+0x118) when it has to
// pick a text colour to draw over the highlight brush: black when every one of
// the three channel bytes is above 0x80, white otherwise.  Byte compares
// against 0x80 with `jbe` -> white at 0x1aedf7/0x1aedff/0x1aee07
// (OnFillCommandsListBackground) and 0x1b2fd1/0x1b2fd9/0x1b2fe1
// (OnFillCaptionBarButton).
inline COLORREF XP_TextOverHighlight(COLORREF clrHighlight)
{
    return (GetRValue(clrHighlight) > 0x80 && GetGValue(clrHighlight) > 0x80 &&
            GetBValue(clrHighlight) > 0x80) ? RGB(0, 0, 0) : RGB(255, 255, 255);
}

// The 1..8 ::PatBlt(PATCOPY) frame retail paints with a selected brush in
// OnDrawMiniFrameBorder (0x1b0120) and OnDrawFloatingToolbarBorder (0x1b0360):
// the four outer runs, then, after both rects are deflated by 2, the four
// inner runs.  x/y/cx/cy of every run are transcribed at the call sites.
void XP_PatBltFrame(HDC hdc, const CRect& rb, const CRect& rs, bool bInner)
{
    if (!bInner) {
        ::PatBlt(hdc, rb.left, rb.top, rs.left, rb.bottom - rb.top, PATCOPY);
        ::PatBlt(hdc, rb.left, rb.top, rb.right - rb.left, rs.top, PATCOPY);
        ::PatBlt(hdc, rb.right - rs.right, rb.top, rs.right, rb.bottom - rb.top, PATCOPY);
        ::PatBlt(hdc, rb.left, rb.bottom - rs.bottom, rb.right - rb.left, rs.bottom, PATCOPY);
    } else {
        ::PatBlt(hdc, rb.left, rb.top + 1, rs.left, rb.bottom - rb.top - 2, PATCOPY);
        ::PatBlt(hdc, rb.left + 1, rb.top, rb.right - rb.left - 2, rs.top, PATCOPY);
        ::PatBlt(hdc, rb.right - rs.right, rb.top + 1, rs.right, rb.bottom - rb.top - 2, PATCOPY);
        ::PatBlt(hdc, rb.left + 1, rb.bottom - rs.bottom, rb.right - rb.left - 2, rs.bottom, PATCOPY);
    }
}

// CMFCCaptionBar::m_bIsMessageBarMode.  Retail reads it at +0x404 (the
// `cmpl $0x0,0x404(%r8)` at 0x1b304f and 0x1b2efc); the offset is pinned by
// the CB shadow struct in phase4/src/detail/CMFCCaptionBarSupport.h (whose
// field there carries a guessed name), and the member name comes from the
// afxcaptionbar.h declaration order: m_clrBarText / m_clrBarBackground /
// m_clrBarBorder / m_bIsMessageBarMode / m_pToolTip, with the shadow struct
// pinning the tooltip pointer at +0x408.
const std::ptrdiff_t kXPCaptionBarIsMessageBarMode = 0x404;

// CMFCVisualManager::m_bEnableToolbarButtonFill.  ??0CMFCVisualManager@@QEAA@H@Z
// (0x182640) stores 1 into +0xb0 at 0x1826af (the MSVC header's default TRUE),
// and OnFillCommandsListBackground / OnFillButtonInterior test it there.  The
// base sub-object ends at +0xa0 in retail (its members begin with the two BOOLs
// the 8-byte zero store at 0x182692 covers), which puts m_bEnableToolbarButtonFill,
// the fifth BOOL of afxvisualmanager.h's member list, at +0xb0.  OpenMFC's
// CMFCVisualManager keeps only zeroed padding there, so the flag reads 0 unless
// an MSVC client's inline EnableToolbarButtonFill() wrote it -- see the
// headerRequest filed with this file.
const std::ptrdiff_t kXPVmEnableToolbarButtonFill = 0xb0;

inline int XP_ReadInt(const void* pObject, std::ptrdiff_t off)
{
    return *reinterpret_cast<const int*>(static_cast<const char*>(pObject) + off);
}

// The AFX_BUTTON_STATE compares in the bodies below use the MSVC header's
// values: afxvisualmanager.h declares ButtonsIsRegular 0, ButtonsIsPressed 1,
// ButtonsIsHighlighted 2 (so every `state - 1 <= 1` range check retail makes
// means exactly "pressed or highlighted").  include/openmfc/afxmfc.h has the
// last two swapped -- a headerRequest is filed -- so raw values are compared.
const int kXPStatePressed     = 1;
const int kXPStateHighlighted = 2;

} // namespace

// Symbol: ?CreateObject@CMFCVisualManagerOfficeXP@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerOfficeXP__SAPEAVCObject__XZ() { return new CMFCVisualManagerOfficeXP(); }
// Symbol: ?GetThisClass@CMFCVisualManagerOfficeXP@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerOfficeXP@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerOfficeXP__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerOfficeXP* pThis) { return CMFCVisualManagerOfficeXP::GetThisClass(); }
// Symbol: ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCVisualManagerOfficeXP__IEAA_H_Z(void* pThis, int) {
    return new(pThis) CMFCVisualManagerOfficeXP();
}
// Symbol: ?OnDrawPropertySheetListItem@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCPropertySheet@@VCRect@@HH@Z
extern "C" unsigned long MS_ABI impl__OnDrawPropertySheetListItem_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
    void* pThis, CDC* pDC, void* pPropSheet, const CRect& rect, int nItem, int nState) {
    (void)pPropSheet;
    return impl__OnDrawPropertySheetListItem_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCPropertySheet__VCRect__HH_Z(
        pThis, pDC, pPropSheet, rect, nItem, nState);
}
CMFCVisualManagerOfficeXP::CMFCVisualManagerOfficeXP() {
    memset(_pad, 0, sizeof(_pad));
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerOfficeXP::~CMFCVisualManagerOfficeXP() {}
CObject* CMFCVisualManagerOfficeXP::CreateObject() { return new CMFCVisualManagerOfficeXP(); }
// Retail (RVA 0x1af590) builds two 8x8 monochrome ::CreateBitmap patterns and
// attaches ::CreatePatternBrush results to the CBrush members at +0x140 and
// +0x150.  OpenMFC's CMFCVisualManagerOfficeXP has no storage at those offsets
// (see the file header), so there is nowhere to keep the brushes.
// XP_CreateGripperBrush (above) carries the first pattern and OnDrawBarGripper
// the second, each built around its one use.
// Symbol: ?CreateGripperBrush@CMFCVisualManagerOfficeXP@@MEAAXXZ
extern "C" void MS_ABI impl__CreateGripperBrush_CMFCVisualManagerOfficeXP__MEAAXXZ(CMFCVisualManagerOfficeXP* /*pThis*/) {}

// CMFCVisualManagerOfficeXP::ExtendMenuButton(CMFCToolBarMenuButton*, CRect& rect)
// -- retail RVA 0x1af6d0 (mfc140.dll):
//     CMFCPopupMenu* pPopup = pMenuButton->m_pPopupMenu;             // +0xc0, 0x1af6f3
//     if (!pPopup || !pPopup->m_hWnd) return;                          // +0x40, 0x1af700/0x1af709
//     CRect rectPopup; ::GetWindowRect(pPopup->m_hWnd, &rectPopup);    // 0x1af724
//     if (pMenuButton->IsKindOf(RUNTIME_CLASS(CMFCCustomizeButton)) /*0x1803aa148*/
//         && pMenuButton->m_pWndParent /*+0x80*/
//         && parent->IsKindOf(RUNTIME_CLASS(CBasePane))) {             // 0x1802da490, 0x1af734..0x1af76a
//         CRect r = rect; parent->ClientToScreen(&r);                  // 0x2a1250
//         if (parent->vtbl[+0x2d8]()) { r.right = rectPopup.right; r.bottom = rectPopup.bottom; }
//         else { r.left = rectPopup.left; r.right = rectPopup.right; r.bottom++; }
//         CRect rInter; if (!::IntersectRect(&rInter, &r, &rectPopup)) return;   // 0x1af7c9
//     }
//     switch (pPopup->[+0x1650]) {                                      // drop direction, 0x1af7d3
//         case 1: rect.bottom += (rectPopup.Width()  >= rect.Width())  ? 4 : 1; break;   // 0x1af838
//         case 2: rect.top    -= (rectPopup.Width()  >= rect.Width())  ? 4 : 1; break;   // 0x1af81e
//         case 3: rect.right  += (rectPopup.Height() >= rect.Height()) ? 4 : 1; break;   // 0x1af803
//         case 4: rect.left   -= (rectPopup.Height() >= rect.Height()) ? 4 : 1; break;   // 0x1af7e9
//     }
// Left a stub: the body is reached only through m_pPopupMenu (+0xc0), which is
// inside OpenMFC's CMFCToolBarMenuButton padding and is never set anywhere in
// this tree (phase4/src/featurepack/toolbar/CMFCToolBar.cpp leaves
// DropDownMenu / GetDroppedDownMenu stubbed for the same reason), so every
// call would take the early return at 0x1af703.  (The two descriptors read
// back as "CMFCCustomizeButton" and "CBasePane".)
// Symbol: ?ExtendMenuButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCMFCToolBarMenuButton@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__ExtendMenuButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCMFCToolBarMenuButton__AEAVCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CMFCToolBarMenuButton* /*pMenuButton*/, CRect* /*pRect*/) {}

// CMFCVisualManagerOfficeXP::GetAutoHideButtonTextColor(CMFCAutoHideButton*)
// -- retail RVA 0x1b17d0.  Guarded afxGlobalData.Initialize(), then returns
// afxGlobalData.clrBtnDkShadow (+0x3c).  Neither `this` nor the button
// argument is read.
// Symbol: ?GetAutoHideButtonTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCAutoHideButton@@@Z
extern "C" unsigned long MS_ABI impl__GetAutoHideButtonTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCAutoHideButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, void* /*pButton*/ /* CMFCAutoHideButton* */)
{
    return GD_clrBtnDkShadow();
}

// CMFCVisualManagerOfficeXP::GetHighlightedMenuItemTextColor(CMFCToolBarMenuButton*)
// -- retail RVA 0x1ae130 (mfc140.dll):
//     if (pButton->m_nStyle (+0x28) & TBBS_DISABLED /*0x40000*/)      // 0x1ae134
//         return afxGlobalData.clrGrayedText (+0x44);                  // 0x1ae160
//     // otherwise a luminance test on the three bytes of the OfficeXP
//     // highlight colour, member +0x118 (the `jbe` chain at 0x1ae170 /
//     // 0x1ae178 / 0x1ae188):
//     return (R > 0x80 && G > 0x80 && B > 0x80) ? RGB(0,0,0) : RGB(255,255,255);
// Both branches are reproduced: the highlight colour is XP_Colors().c118 (the
// same value OnUpdateSystemColors stores at +0x118 -- see the file header) and
// the byte test is XP_TextOverHighlight, the helper the identical compares in
// OnFillCommandsListBackground / OnFillCaptionBarButton use.  Retail
// dereferences pButton unconditionally; the null guard here is OpenMFC's.
// CMFCToolBarMenuButton derives from CMFCToolBarButton without adding a base,
// so m_nStyle is read through the base type.
// Symbol: ?GetHighlightedMenuItemTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCToolBarMenuButton@@@Z
extern "C" unsigned long MS_ABI impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CMFCToolBarButton* pButton)
{
    if (pButton != nullptr && (pButton->m_nStyle & 0x00040000u) != 0) {
        return GD_clrGrayedText();
    }
    return XP_TextOverHighlight(XP_Colors().c118);
}

// CMFCVisualManagerOfficeXP::GetPropertyGridGroupColor(CMFCPropertyGridCtrl*)
// -- retail RVA 0x1b1ae0:
//     afxGlobalData.Initialize();
//     if (afxGlobalData.m_nBitsPerPixel (+0x288) <= 8)
//         return CMFCVisualManager::GetPropertyGridGroupColor(pList); // 0x189d10
//     return CDrawingManager::PixelAlpha(
//                pList->[+0x5fc] ? afxGlobalData.clrBarFace (+0x60)
//                                : afxGlobalData.clrBtnFace (+0x28), 0x5e);
// UpdateSysColors (0x6afd0) stores GetSysColor(COLOR_BTNFACE) into both
// clrBtnFace and clrBarFace (stores at 0x6b066 and 0x6b069), so the +0x5fc branch
// selects the
// same colour either way and the unmodelled member does not matter here.
// Symbol: ?GetPropertyGridGroupColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManagerOfficeXP* pThis, CMFCPropertyGridCtrl* pList)
{
    if (GD_BitsPerPixel() <= 8) {
        return impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
            pThis, pList);
    }
    return impl__PixelAlpha_CDrawingManager__SAKKH_Z(GD_clrBtnFace(), 0x5e);
}

// CMFCVisualManagerOfficeXP::GetPropertyGridGroupTextColor(CMFCPropertyGridCtrl*)
// -- retail RVA 0x1b1b90:
//     return pList->[+0x5fc] ? afxGlobalData.clrBarShadow (+0x64)
//                            : afxGlobalData.clrBtnShadow (+0x2c);
// UpdateSysColors (0x6afd0) stores GetSysColor(COLOR_BTNSHADOW) into both of
// those fields (0x6b077/0x6b07a), so both branches yield the same colour and
// the unmodelled +0x5fc member does not change the result.
// Symbol: ?GetPropertyGridGroupTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCPropertyGridCtrl@@@Z
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CMFCPropertyGridCtrl* /*pList*/)
{
    return GD_clrBtnShadow();
}


// CMFCVisualManagerOfficeXP::GetRibbonSliderColors(CMFCRibbonSlider*,
//         BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled,
//         COLORREF& clrLine, COLORREF& clrFill) -- retail RVA 0x1b2960:
//     if (bIsDisabled)                       clrLine = afxGlobalData.clrBarShadow   (+0x64);
//     else if (bIsPressed || bIsHighlighted) clrLine = this->[+0x138];
//     else                                   clrLine = afxGlobalData.clrBarDkShadow (+0x6c);
//     if (!bIsHighlighted) clrFill = afxGlobalData.clrBarFace (+0x60);
//     else if (bIsPressed) clrFill = this->[+0x11c];
//     else                 clrFill = this->[+0x118];
// (The clrLine branch is the test chain at 0x1b296f / 0x1b29a6 / 0x1b29aa, all
// three arms merging at the store at 0x1b2a1b; the clrFill branch is the
// 0x1b2a22 / 0x1b2a26 pair merging at the store at 0x1b29fe.)  The slider
// pointer is never read.  UpdateSysColors (0x6afd0) assigns clrBarShadow,
// clrBarDkShadow and clrBarFace from ::GetSysColor(COLOR_BTNSHADOW),
// (COLOR_3DDKSHADOW) and (COLOR_BTNFACE) at 0x6b07a, 0x6b08b and 0x6b066.
// Symbol: ?GetRibbonSliderColors@CMFCVisualManagerOfficeXP@@MEAAXPEAVCMFCRibbonSlider@@HHHAEAK1@Z
extern "C" void MS_ABI impl__GetRibbonSliderColors_CMFCVisualManagerOfficeXP__MEAAXPEAVCMFCRibbonSlider__HHHAEAK1_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, void* /*pSlider*/ /* CMFCRibbonSlider* */,
    int bIsHighlighted, int bIsPressed, int bIsDisabled,
    unsigned long* pclrLine, unsigned long* pclrFill)
{
    const XPColors x = XP_Colors();

    if (bIsDisabled)                            *pclrLine = GD_clrBtnShadow();
    else if (bIsPressed || bIsHighlighted)      *pclrLine = x.c138;
    else                                        *pclrLine = GD_clrBtnDkShadow();

    if (!bIsHighlighted)  *pclrFill = GD_clrBtnFace();
    else if (bIsPressed)  *pclrFill = x.c11c;
    else                  *pclrFill = x.c118;
}


// CMFCVisualManagerOfficeXP::GetSmartDockingBaseGuideColors(COLORREF&, COLORREF&)
// -- retail RVA 0x1b1800, and the whole body is four instructions:
//     clrBaseGroupBackground = this->[+0x108];
//     clrBaseGroupBorder     = this->[+0x134];
// Both come from XP_Colors(); see its comment for how faithful that is.
// Symbol: ?GetSmartDockingBaseGuideColors@CMFCVisualManagerOfficeXP@@MEAAXAEAK0@Z
extern "C" void MS_ABI impl__GetSmartDockingBaseGuideColors_CMFCVisualManagerOfficeXP__MEAAXAEAK0_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, unsigned long* pclrBaseGroupBackground,
    unsigned long* pclrBaseGroupBorder)
{
    const XPColors x = XP_Colors();
    *pclrBaseGroupBackground = x.c108;
    *pclrBaseGroupBorder     = x.c134;
}

// CMFCVisualManagerOfficeXP::GetToolbarButtonTextColor(CMFCToolBarButton*,
//         AFX_BUTTON_STATE state) -- retail RVA 0x1b0550:
//     afxGlobalData.Initialize();
//     if (afxGlobalData.[+0x264] || afxGlobalData.[+0x260])          // high contrast
//         return CMFCVisualManager::GetToolbarButtonTextColor(pButton, state); // 0x186830
//     BOOL bDisabled = CMFCToolBar::m_bCustomizeMode
//                          ? !pButton->IsEditable()                   // vtable +0xd0
//                          : (pButton->m_nStyle & TBBS_DISABLED /*0x40000*/) != 0;
//     if (pButton->IsKindOf(RUNTIME_CLASS(CMFCOutlookBarPaneButton))) {  // 0x1803aa3b8
//         if (bDisabled) return afxGlobalData.clrGrayedText (+0x44);
//         afxGlobalData.Initialize();
//         return (!afxGlobalData.[+0x264] && !afxGlobalData.[+0x260])
//                    ? afxGlobalData.clrBarText    (+0x74)
//                    : afxGlobalData.clrWindowText (+0x7c);
//     }
//     if (state == 2 && (pButton->m_nStyle & 0x30000))
//         return afxGlobalData.clrTextHilite (+0x4c);
//     return CMFCVisualManager::GetToolbarButtonTextColor(pButton, state);
// `this` is only forwarded to the base implementation; no OfficeXP colour member
// is read anywhere in this body, which is why it can be reproduced in full.
// The class the IsKindOf at 0x1b05e8 tests against is CMFCOutlookBarPaneButton
// (the CRuntimeClass at image address 0x1803aa3b8, whose m_lpszClassName reads
// "CMFCOutlookBarPaneButton").  The re-test of the two high-contrast flags
// inside the IsKindOf branch can only take the clrBarText side, because the
// entry test already returned when either flag was set and Initialize() cannot
// change them once m_bInitialized is 1 -- both arms are kept below anyway so the
// transcription stays faithful.  state is compared against the raw retail
// AFX_BUTTON_STATE value 2, exactly as CMFCVisualManager.cpp does in this repo.
// Deviation: retail dereferences pButton unconditionally; the null guards here
// are OpenMFC's.
// Deviation: pButton->IsEditable() is not dispatched through the object's vptr
// -- see XP_Button_IsEditable at the top of this file for why that would be an
// out-of-bounds indirect call here.  It is transcribed from the retail body at
// 0x23820 instead, minus that body's two reserved-command global lookups, which
// can only widen the "not editable" set.
// Symbol: ?GetToolbarButtonTextColor@CMFCVisualManagerOfficeXP@@MEAAKPEAVCMFCToolBarButton@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOfficeXP* pThis, CMFCToolBarButton* pButton, int state)
{
    const bool bHighContrast = GD_IsBlackHighContrast() || GD_IsWhiteHighContrast();
    if (bHighContrast || pButton == nullptr) {
        return impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
            pThis, pButton, state);
    }

    const bool bDisabled =
        (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0)
            ? (XP_Button_IsEditable(pButton) == 0)
            : ((pButton->m_nStyle & 0x00040000u) != 0);

    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pButton),
            impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ()) != 0) {
        if (bDisabled) return GD_clrGrayedText();
        return (!GD_IsBlackHighContrast() && !GD_IsWhiteHighContrast())
                   ? GD_clrBtnText()
                   : GD_clrWindowText();
    }

    if (state == 2 && (pButton->m_nStyle & 0x00030000u) != 0) {
        return GD_clrTextHilite();
    }

    return impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
        pThis, pButton, state);
}

// CMFCVisualManagerOfficeXP::GetWindowColor() const -- retail RVA 0x1b17a0.
// Calls afxGlobalData.Initialize() through the m_bInitialized guard and returns
// afxGlobalData.clrWindow (+0x78).  `this` is not otherwise touched.
// Symbol: ?GetWindowColor@CMFCVisualManagerOfficeXP@@MEBAKXZ
extern "C" unsigned long MS_ABI impl__GetWindowColor_CMFCVisualManagerOfficeXP__MEBAKXZ(
    const CMFCVisualManagerOfficeXP* /*pThis*/)
{
    return GD_clrWindow();
}

// CMFCVisualManagerOfficeXP::OnDrawBarGripper(CDC*, CRect rectGripper, BOOL bHorz,
//         CBasePane* pBar) -- retail RVA 0x1aca30 (mfc140.dll):
//     if (pBar && pBar->m_bIsDlgControl /*+0xf4*/) {                          // 0x1aca67/0x1aca6c
//         CMFCVisualManager::OnDrawBarGripper(pDC, rectGripper, bHorz, pBar); return; }   // 0x182a80
//     if (m_brGripperHorz.m_hObject == NULL) this->CreateGripperBrush();      // +0x148 / vtable +0x620
//     BOOL bIsDockable = pBar && pBar->IsKindOf(RUNTIME_CLASS(CDockablePane)); // 0x3aa178, 0x1acac9
//     BOOL bIsMenuBar  = pBar && pBar->IsKindOf(RUNTIME_CLASS(CMFCMenuBar));   // 0x3aa268, 0x1acae9
//     CRect rectFill = rectGripper;
//     if (!bIsDockable) {
//         if (bHorz) { int xc = (l + r) / 2; rectFill.left = xc - 1; rectFill.right = xc + 2;
//                      ::InflateRect(&rectFill, 0, -5); }                       // 0x1acb08..0x1acb4a
//         else       { int yc = (t + b) / 2; rectFill.top = yc - 1; rectFill.bottom = yc + 2;
//                      ::InflateRect(&rectFill, -5, 0); }                       // 0x1acb28..0x1acb4a
//     } else { ::InflateRect(&rectFill, -4, 0); bHorz = !bHorz; }             // 0x1acb5d..0x1acb6b
//     COLORREF clrOldText = pDC->SetTextColor(m_clrGripper);                   // +0x128, vtable +0x70, 0x1acb7e
//     COLORREF clrOldBk   = pDC->SetBkColor((!bIsDockable && !bIsMenuBar)
//                               ? m_clrBarBkgnd /*+0x108*/ : afxGlobalData.clrBarFace /*+0x60*/); // 0x1acbd0
//     ::FillRect(pDC->m_hDC, rectFill, bHorz ? m_brGripperHorz : m_brGripperVert);   // +0x140/+0x150, 0x1acbfd
//     if (bIsDockable) {                                                       // 0x1acc03
//         int nOldBkMode = pDC->SetBkMode(TRANSPARENT);                        // 0x2a07a0
//         pDC->SetTextColor(afxGlobalData.clrBarText);                         // +0x74
//         CGdiObject* pOldFont = pDC->SelectObject(&CMFCMenuBar::GetMenuFont(bHorz));   // 0x8ddb0, vtable +0x60
//         CString strCaption; pBar->GetWindowText(strCaption);                // 0x28a280
//         strCaption = L" " + strCaption + L" ";                               // 0x1accaf..0x1accd0, literal at 0x33af14
//         TEXTMETRIC tm; ::GetTextMetrics(pDC->m_hAttribDC, &tm);              // 0x1acd35
//         CRect rectText = rectGripper;  UINT nFormat;
//         if (bHorz) { rectText.top += (rectGripper.Height() - tm.tmHeight - 1) / 2; nFormat = 0; }   // 0x1acd40..0x1acd86
//         else { int dx = (rectGripper.Width() - tm.tmHeight + 1) / 2;
//                rectText.left = rectText.right - dx; rectText.bottom = rectText.top; nFormat = DT_NOCLIP; }  // 0x1acd59..0x1acdae
//         pDC->DrawText(strCaption, strCaption.GetLength(), rectText, nFormat);   // vtable +0xe0, 0x1acdc4
//         pDC->SelectObject(pOldFont); pDC->SetBkMode(nOldBkMode);
//     }
//     pDC->SetTextColor(clrOldText); pDC->SetBkColor(clrOldBk);               // 0x1ace1a / 0x1ace2d
// The two runtime-class descriptors read back as "CDockablePane" and
// "CMFCMenuBar".  In the vertical caption case retail really does collapse
// rectText to zero height and rely on DT_NOCLIP with the rotated menu font.
// The gripper brushes are built by XP_CreateGripperBrush (horizontal pattern;
// see its comment) -- the vertical pattern, CreateGripperBrush's second bitmap
// of four DWORDs 0x00aa00aa (0x1af627..0x1af63c, i.e. every row 10101010 in a
// WORD-padded 1-bpp scanline), is built inline below.  Deviations forced by the
// missing member storage: both brushes are created and destroyed per call;
// the CDC virtuals SetTextColor/SetBkColor/SetBkMode/SelectObject/DrawText go
// straight to GDI; the caption is built in a plain wchar_t buffer, not a CString.
// The CFont comes from OpenMFC's exported CMFCMenuBar::GetMenuFont.
// Symbol: ?OnDrawBarGripper@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rectGripper, int bHorz, CBasePane* pBar)
{
    if (pBar != nullptr && pBar->m_bIsDlgControl) {
        impl__OnDrawBarGripper_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(
            pThis, pDC, rectGripper, bHorz, pBar);
        return;
    }

    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;

    const bool bIsDockable =
        pBar != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pBar), impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ()) != 0;
    const bool bIsMenuBar =
        pBar != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pBar), impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ()) != 0;

    CRect rectFill(rectGripper);
    if (!bIsDockable) {
        if (bHorz) {
            const int xc = (rectFill.left + rectFill.right) / 2;
            rectFill.left  = xc - 1;
            rectFill.right = xc + 2;
            rectFill.InflateRect(0, -5);
        } else {
            const int yc = (rectFill.top + rectFill.bottom) / 2;
            rectFill.top    = yc - 1;
            rectFill.bottom = yc + 2;
            rectFill.InflateRect(-5, 0);
        }
    } else {
        rectFill.InflateRect(-4, 0);
        bHorz = !bHorz;
    }

    const XPColors x = XP_Colors();
    const COLORREF clrOldText = ::SetTextColor(hdc, x.c128);
    const COLORREF clrOldBk   = ::SetBkColor(hdc, (!bIsDockable && !bIsMenuBar) ? x.c108 : GD_clrBtnFace());

    HBRUSH hbrGripper = nullptr;
    if (bHorz) {
        hbrGripper = XP_CreateGripperBrush();
    } else {
        static const BYTE bitsVert[16] = { 0xaa, 0x00, 0xaa, 0x00,
                                           0xaa, 0x00, 0xaa, 0x00,
                                           0xaa, 0x00, 0xaa, 0x00,
                                           0xaa, 0x00, 0xaa, 0x00 };
        HBITMAP hbm = ::CreateBitmap(8, 8, 1, 1, bitsVert);
        if (hbm != nullptr) {
            hbrGripper = ::CreatePatternBrush(hbm);
            ::DeleteObject(hbm);
        }
    }
    if (hbrGripper != nullptr) {
        RECT r = XP_ToRECT(rectFill);
        ::FillRect(hdc, &r, hbrGripper);
        ::DeleteObject(hbrGripper);
    }

    if (bIsDockable) {
        const int nOldBkMode = ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, GD_clrBtnText());

        CFont* pFont = impl__GetMenuFont_CMFCMenuBar__SAAEBVCFont__H_Z(bHorz);
        HGDIOBJ hOldFont = nullptr;
        if (pFont != nullptr && pFont->GetSafeHandle() != nullptr) {
            hOldFont = ::SelectObject(hdc, pFont->GetSafeHandle());
        }

        // strCaption = L" " + <window text> + L" "
        const int nLen = ::GetWindowTextLengthW(pBar->m_hWnd);
        wchar_t* strCaption = static_cast<wchar_t*>(
            ::HeapAlloc(::GetProcessHeap(), 0, (static_cast<size_t>(nLen > 0 ? nLen : 0) + 3) * sizeof(wchar_t)));
        if (strCaption == nullptr) {
            if (hOldFont != nullptr) ::SelectObject(hdc, hOldFont);
            ::SetBkMode(hdc, nOldBkMode);
            ::SetTextColor(hdc, clrOldText);
            ::SetBkColor(hdc, clrOldBk);
            return;
        }
        int nCaption = 0;
        strCaption[nCaption++] = L' ';
        if (nLen > 0) {
            const int nGot = ::GetWindowTextW(pBar->m_hWnd, strCaption + 1, nLen + 1);
            if (nGot > 0) nCaption += nGot;
        }
        strCaption[nCaption++] = L' ';
        strCaption[nCaption] = L'\0';

        TEXTMETRICW tm;
        std::memset(&tm, 0, sizeof(tm));
        ::GetTextMetricsW(pDC->m_hAttribDC != nullptr ? pDC->m_hAttribDC : hdc, &tm);

        RECT rectText = XP_ToRECT(rectGripper);
        UINT nFormat;
        if (bHorz) {
            rectText.top += ((rectGripper.bottom - rectGripper.top) - tm.tmHeight - 1) / 2;
            nFormat = 0;
        } else {
            const int dx = ((rectGripper.right - rectGripper.left) - tm.tmHeight + 1) / 2;
            rectText.left   = rectText.right - dx;
            rectText.bottom = rectText.top;
            nFormat = DT_NOCLIP;
        }
        ::DrawTextW(hdc, strCaption, nCaption, &rectText, nFormat);
        ::HeapFree(::GetProcessHeap(), 0, strCaption);

        if (hOldFont != nullptr) ::SelectObject(hdc, hOldFont);
        ::SetBkMode(hdc, nOldBkMode);
    }

    ::SetTextColor(hdc, clrOldText);
    ::SetBkColor(hdc, clrOldBk);
}

// CMFCVisualManagerOfficeXP::OnDrawBrowseButton(CDC*, CRect rect, CMFCEditBrowseCtrl*,
//         AFX_BUTTON_STATE state, COLORREF& clrText) -- retail RVA 0x1b1610 (mfc140.dll):
//     CRect rectFrame = rect;
//     rectFrame.top--; rectFrame.right++; rectFrame.bottom++;          // 0x1b1637..0x1b1650
//     if (state == ButtonsIsPressed /*1*/) {                          // 0x1b1654
//         ::FillRect(pDC->m_hDC, rect, m_brHighlightDn.m_hObject);     // +0x1b0, 0x1b1737
//         pDC->Draw3dRect(rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);  // 0x1b174f
//         pDC->SetTextColor(afxGlobalData.clrWindow);                  // vtable +0x70 with +0x78, 0x1b1782
//     } else if (state == ButtonsIsHighlighted /*2*/) {               // 0x1b165f
//         ::FillRect(pDC->m_hDC, rect, m_brHighlight.m_hObject);       // +0x1a0, 0x1b16fc
//         pDC->Draw3dRect(rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);  // 0x1b1714
//     } else {
//         ::FillRect(pDC->m_hDC, rect, afxGlobalData.brBtnFace.m_hObject);       // +0xb0, 0x1b168d
//         pDC->Draw3dRect(rect, afxGlobalData.clrBarHilite, afxGlobalData.clrBarHilite);  // +0x68, 0x1b1714
//     }
//     return TRUE;
// The regular arm frames `rect` itself, the other two frame rectFrame.  Neither
// the edit control nor clrText is read or written -- the 6th argument slot
// [rsp+0x88] is never touched, so the caller's colour is left as it was.  Slot
// +0x70 of the retail CDC vtable (0x180338fb0) is ?SetTextColor@CDC@@UEAAKK@Z;
// ::SetTextColor on the HDC is used here.  brBtnFace is ::CreateSolidBrush of
// COLOR_BTNFACE (UpdateSysColors 0x6b238/0x6b23b) and clrBarHilite is
// COLOR_BTNHIGHLIGHT; the OfficeXP brushes come from XP_Colors() (c11c / c118)
// and the border from c138.  State values per kXPStatePressed / kXPStateHighlighted.
// Symbol: ?OnDrawBrowseButton@CMFCVisualManagerOfficeXP@@MEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@CMFCVisualManager@@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    void* /*pEdit*/ /* CMFCEditBrowseCtrl* */, int state, unsigned long* /*pclrText*/)
{
    CRect rectFrame(rect);
    rectFrame.top--;
    rectFrame.right++;
    rectFrame.bottom++;

    if (state == kXPStatePressed) {
        const XPColors x = XP_Colors();
        XP_FillSolid(pDC, rect, x.c11c);
        DC_Draw3dRect(pDC, rectFrame, x.c138, x.c138);
        HDC hdc = XP_Hdc(pDC);
        if (hdc != nullptr) ::SetTextColor(hdc, GD_clrWindow());
    } else if (state == kXPStateHighlighted) {
        const XPColors x = XP_Colors();
        XP_FillSolid(pDC, rect, x.c118);
        DC_Draw3dRect(pDC, rectFrame, x.c138, x.c138);
    } else {
        XP_FillSolid(pDC, rect, GD_clrBtnFace());
        DC_Draw3dRect(pDC, rect, GD_clrBarHilite(), GD_clrBarHilite());
    }
    return TRUE;
}

// CMFCVisualManagerOfficeXP::OnDrawButtonBorder(CDC*, CMFCToolBarButton*, CRect rect,
//         AFX_BUTTON_STATE state) -- retail RVA 0x1adc30 (mfc140.dll):
//     if (state != ButtonsIsPressed && state != ButtonsIsHighlighted) return;   // 0x1adc5a..0x1adc61
//     COLORREF clr = m_clrMenuItemBorder;                                        // +0x138, 0x1adc67
//     BOOL bIsPopupMenuItem = FALSE, bNotDroppedDown = FALSE;
//     CMFCToolBarMenuButton* pMenuButton = NULL;
//     if (pButton && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {  // 0x1adc7d
//         CWnd* pParent = pButton->m_pWndParent;                                 // +0x80
//         if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar))) bIsPopupMenuItem = TRUE;  // -> TAIL
//         else pMenuButton = pButton;
//     }
//     if (!bIsPopupMenuItem) {
//         if (!pMenuButton || !pMenuButton->IsDroppedDown()) bNotDroppedDown = TRUE;   // vtable +0xf0
//         else {
//             CMFCPopupMenu* pPopup = pMenuButton->m_pPopupMenu;                  // +0xc0, 0x1adcdd
//             if (pPopup && (::IsWindowVisible(pPopup->m_hWnd) || pPopup->[+0x1670] != 0)) {
//                 clr = m_clrMenuBorder;                                         // +0x134
//                 this->ExtendMenuButton(pMenuButton, rect);                     // vtable +0x628
//                 if (m_bShdowDroppedDownMenuButton /*+0x204*/ && !(pPopup->GetExStyle() & WS_EX_LAYOUTRTL)
//                     && CMFCMenuBar::m_bMenuShadows /*0x3aab00*/ && !CMFCToolBar::m_bCustomizeMode
//                     && bpp > 8 && !high-contrast && !pPopup->m_bIsSubMenu /*+0x166c*/) {
//                     CDrawingManager dm(*pDC);
//                     dm.DrawShadow(rect, m_nMenuShadowDepth /*+0xcc*/, 100, 75, NULL, NULL,
//                                   m_clrMenuShadowBase /*+0xf8*/, TRUE);        // 0x58c80, 0x1ade30
//                 }
//             }
//         }
//     }
//   TAIL (0x1ade45):
//     BOOL bChecked = pButton->m_nStyle & TBBS_CHECKED;
//     if (state == ButtonsIsPressed) {
//         if (bNotDroppedDown && m_clrPressedButtonBorder /*+0x124*/ != -1 && !bChecked
//             && rect.Width() > 5 && rect.Height() > 5) clr = m_clrPressedButtonBorder;   // 0x1ade5c..0x1ade86
//     }
//     if (bIsPopupMenuItem && bChecked) {                                        // 0x1ade8a/0x1ade8e
//         if (pButton->m_nStyle & TBBS_MARKED /*0x800000*/) clr = m_clrPressedButtonBorder;  // 0x1ade92/0x1ade9b
//         rect.bottom++;                                                         // 0x1adea3
//     }
//     pDC->Draw3dRect(rect, clr, clr);                                           // 0x1adeb2
// DEVIATION: as in OnFillButtonInterior, the IsDroppedDown() arm (m_pPopupMenu,
// +0xc0, is unmodelled padding in this tree and CDrawingManager::DrawShadow is
// a stub) is treated as not taken, so bNotDroppedDown is TRUE for every
// non-popup button.  m_clrPressedButtonBorder is the constant -1 for this class
// (OnUpdateSystemColors stores it at 0x1ac9e5), which makes both of the
// +0x124 arms inert; they are kept as explicit constants.  The guards on
// pButton are OpenMFC's (retail only null-tests it before the IsKindOf).
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state)
{
    if (state != kXPStatePressed && state != kXPStateHighlighted) return;
    if (pButton == nullptr) return;

    const XPColors x = XP_Colors();
    const COLORREF clrPressedButtonBorder = (COLORREF)-1;   // m_clrPressedButtonBorder, see above
    COLORREF clr = x.c138;

    bool bIsPopupMenuItem = false;
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pButton),
            impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ()) != 0) {
        CWnd* pParent = pButton->m_pWndParent;
        bIsPopupMenuItem =
            pParent != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent),
                impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()) != 0;
    }
    // Dropped-down menu-button arm: see DEVIATION above -- not evaluated.
    const bool bNotDroppedDown = !bIsPopupMenuItem;

    const unsigned int nStyle = pButton->m_nStyle;
    const bool bChecked = (nStyle & 0x00010000u) != 0;

    if (state == kXPStatePressed) {
        if (bNotDroppedDown && clrPressedButtonBorder != (COLORREF)-1 && !bChecked &&
            rect.right - rect.left > 5 && rect.bottom - rect.top > 5) {
            clr = clrPressedButtonBorder;
        }
    }
    if (bIsPopupMenuItem && bChecked) {
        if ((nStyle & 0x00800000u) != 0) clr = clrPressedButtonBorder;
        rect.bottom++;
    }
    DC_Draw3dRect(pDC, rect, clr, clr);
}

// CMFCVisualManagerOfficeXP::OnDrawButtonSeparator(CDC*, CMFCToolBarButton*,
//         CRect rect, AFX_BUTTON_STATE state, BOOL bHorz)
// -- retail RVA 0x1b1820:
//     CGdiObject* pOldPen = pDC->SelectObject(&this->[+0x1f0]);  // separator pen
//     if (bHorz) { pDC->MoveTo(rect.left, rect.top);  pDC->LineTo(rect.left, rect.bottom); }
//     else       { pDC->MoveTo(rect.left, rect.top);  pDC->LineTo(rect.right, rect.top);   }
//     pDC->SelectObject(pOldPen);
// So bHorz picks a vertical rule between horizontally laid-out buttons and a
// horizontal rule otherwise.  The button pointer and `state` are not read.
// The pen at +0x1f0 is created by OnUpdateSystemColors at 0x1ac9f7 as
// ::CreatePen(PS_SOLID, 1, this->[+0x138]), so it draws in XPBorderColor().
// Symbol: ?OnDrawButtonSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@H@Z
extern "C" void MS_ABI impl__OnDrawButtonSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager__H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CMFCToolBarButton* /*pButton*/,
    CRect rect, int /*state*/, int bHorz)
{
    const COLORREF clrSeparator = XPBorderColor();
    if (bHorz) {
        XP_DrawLine(pDC, rect.left, rect.top, rect.left, rect.bottom, clrSeparator);
    } else {
        XP_DrawLine(pDC, rect.left, rect.top, rect.right, rect.top, clrSeparator);
    }
}

// CMFCVisualManagerOfficeXP::OnDrawCaptionBarButtonBorder(CDC*, CMFCCaptionBar*,
//         CRect rect, BOOL bIsPressed, BOOL bIsHighlighted, BOOL bIsDisabled,
//         BOOL bHasDropDownArrow, BOOL bIsSysButton) -- retail RVA 0x1b3040 (mfc140.dll):
//     if (!pBar->m_bIsMessageBarMode /*+0x404*/) {                    // 0x1b304f
//         CMFCVisualManager::OnDrawCaptionBarButtonBorder(pDC, pBar, rect, bIsPressed,
//             bIsHighlighted, bIsDisabled, bHasDropDownArrow, bIsSysButton);  // 0x186ee0
//         return;
//     }
//     COLORREF clr;
//     if (bIsHighlighted) {                                            // [rsp+0x98], 0x1b30af
//         clr = (bIsSysButton && bIsPressed && m_clrPressedButtonBorder /*+0x124*/ != -1)
//                   ? m_clrPressedButtonBorder : m_clrMenuItemBorder;  // 0x1b30b9..0x1b30e1
//     } else {
//         if (bIsSysButton) return;                                    // 0x1b30e6
//         clr = afxGlobalData.clrBarDkShadow;                          // +0x6c, 0x1b3111
//     }
//     pDC->Draw3dRect(rect, clr, clr);                                 // 0x1b3145
// bIsDisabled and bHasDropDownArrow are never read.  m_clrPressedButtonBorder is
// set to -1 by CMFCVisualManagerOfficeXP::OnUpdateSystemColors on both of its
// paths (the store at 0x1ac9e5 sits in the common tail), so for this class the
// pressed-border arm always resolves to m_clrMenuItemBorder; it is kept below
// as an explicit constant so the shape of the test survives.  The +0x404 read
// goes through kXPCaptionBarIsMessageBarMode (see its comment).  Retail
// dereferences pBar unconditionally; the null guard is OpenMFC's.
// Symbol: ?OnDrawCaptionBarButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int bIsPressed, int bIsHighlighted, int bIsDisabled, int bHasDropDownArrow, int bIsSysButton)
{
    if (pBar == nullptr) return;

    if (XP_ReadInt(pBar, kXPCaptionBarIsMessageBarMode) == 0) {
        impl__OnDrawCaptionBarButtonBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
            pThis, pDC, pBar, rect, bIsPressed, bIsHighlighted, bIsDisabled, bHasDropDownArrow, bIsSysButton);
        return;
    }

    const COLORREF clrPressedButtonBorder = (COLORREF)-1;   // m_clrPressedButtonBorder, see above
    COLORREF clr;
    if (bIsHighlighted) {
        clr = (bIsSysButton && bIsPressed && clrPressedButtonBorder != (COLORREF)-1)
                  ? clrPressedButtonBorder
                  : XPBorderColor();
    } else {
        if (bIsSysButton) return;
        clr = GD_clrBtnDkShadow();
    }
    DC_Draw3dRect(pDC, rect, clr, clr);
}

// CMFCVisualManagerOfficeXP::OnDrawCaptionBarInfoArea(CDC*, CMFCCaptionBar*, CRect)
// -- retail RVA 0x1b3160:
//     ::FillRect(pDC ? pDC->m_hDC : NULL, rect, ::GetSysColorBrush(COLOR_INFOBK));
//     afxGlobalData.Initialize();
//     pDC->Draw3dRect(rect, afxGlobalData.clrBarShadow (+0x64),
//                           afxGlobalData.clrBarShadow);
// The CMFCCaptionBar argument is not read.  Retail really does pass a NULL HDC
// to ::FillRect when pDC is NULL; this version keeps the same guard shape by
// letting XP_Hdc() yield NULL and skipping the paint.
// Symbol: ?OnDrawCaptionBarInfoArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarInfoArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, void* /*pBar*/ /* CMFCCaptionBar* */, CRect rect)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc != nullptr) {
        RECT r = XP_ToRECT(rect);
        ::FillRect(hdc, &r, ::GetSysColorBrush(COLOR_INFOBK));
    }
    DC_Draw3dRect(pDC, rect, GD_clrBtnShadow(), GD_clrBtnShadow());
}

// CMFCVisualManagerOfficeXP::OnDrawCaptionButton(CDC*, CMFCCaptionButton*, BOOL bActive,
//         BOOL bHorz, BOOL bMaximized, BOOL bDisabled, int nImageID) -- retail RVA
// 0x1aea90 (mfc140.dll).  It starts with pButton->GetRect() (vtable +0x28, call at
// 0x1aead6), reads the button's m_bPushed (+0x8) / m_bFocused (+0xc) /
// m_bDroppedDown (+0x18), fills the rect through the visual-manager virtual at
// +0x638 with m_brHighlightDn or m_brHighlight, asks
// pButton->GetIconID(bHorz, bMaximized) (vtable +0x38, 0x1aeb8c) when
// nImageID == -1, centres a CMenuImages::Size() glyph and hands it to
// this->OnDrawCaptionButtonIcon (vtable +0x618, 0x1aebf3), then frames with
// afxGlobalData.clrBarDkShadow.  The three flag offsets ARE pinned in this
// repo (S_Cmfccaptionmenubutton in phase4/src/detail/CMFCCaptionMenuButtonSupport.h:
// m_bPushed 8, m_bFocused 12, m_bDroppedDown 24) even though
// include/openmfc/afxmfc.h declares CMFCCaptionButton as a 24-byte placeholder.
// Left a stub because the geometry is not obtainable: GetRect and GetIconID are
// CMFCCaptionButton virtuals (the base class has no m_rect member -- the same
// shadow header records that the rectangle is computed from m_ptOrg -- and the
// derived caption buttons override both), and OpenMFC's objects carry a
// mingw vtable with nothing at those slots.
// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CMFCCaptionButton* /*pButton*/,
    int /*bActive*/, int /*bHorz*/, int /*bMaximized*/, int /*bDisabled*/, int /*nImageID*/) {}

// CMFCVisualManagerOfficeXP::OnDrawCaptionButtonIcon(CDC*, CMFCCaptionButton*,
//         CMenuImages::IMAGES_IDS id, BOOL bActive, BOOL bDisabled, CPoint ptImage)
// -- retail RVA 0x1aec70 (mfc140.dll):
//     CMenuImages::IMAGE_STATE state;
//     if (bDisabled) state = ImageGray;                                          // [rsp+0x78], 0x1aec74
//     else if (!pButton->m_bFocused /*+0xc*/ && !pButton->m_bPushed /*+0x8*/) {   // 0x1aec8c/0x1aec93
//         COLORREF clr = pButton->m_clrForeground;                                // +0x24, 0x1aec9a
//         if (clr == -1) state = bActive ? ImageWhite : ImageBlack;               // 0x1aeca3..0x1aecaa
//         else state = (every byte of clr > 0xc0) ? ImageWhite : ImageBlack;       // 0x1aecb0..0x1aecc5
//     } else {
//         COLORREF clr = pButton->m_bPushed ? m_clrHighlightDn : m_clrHighlight;   // +0x11c / +0x118, 0x1aecff
//         state = (every byte of clr <= 0xc0) ? ImageWhite : ImageBlack;           // 0x1aed08..0x1aed1e
//     }
//     CMenuImages::Draw(pDC, id, ptImage, state, CSize(0, 0));                    // 0x8fc50 (CPoint overload)
// The byte tests are `cmp $0xc0,<byte>; jbe/ja` chains: in the no-focus arm every
// byte must be ABOVE 0xc0 for ImageWhite (jbe -> ImageBlack at 0x1aecc5); in the
// pushed/focused arm any byte above 0xc0 gives ImageBlack (ja -> 0x1aecc5) and
// only all-<= 0xc0 reaches ImageWhite at 0x1aed1e.  CPoint arrives by value in
// the 7th argument slot and its address is what the Draw overload receives.
// The three button members are read at the offsets pinned by
// S_Cmfccaptionmenubutton (phase4/src/detail/CMFCCaptionMenuButtonSupport.h:
// m_bPushed 8, m_bFocused 12, m_clrForeground 36); m_clrHighlightDn /
// m_clrHighlight are XP_Colors().c11c / c118.  The glyph itself goes to the
// exported CMenuImages::Draw, which is an empty stub in this tree (see the
// declaration at the top of the file) -- the same situation as
// OnDrawMenuSystemButton and OnDrawTabCloseButton, which also hand their
// non-arrow ids to that stub.  Retail dereferences pButton unconditionally;
// the null guard is OpenMFC's.
// Symbol: ?OnDrawCaptionButtonIcon@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionButton@@W4IMAGES_IDS@CMenuImages@@HHVCPoint@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionButtonIcon_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CMFCCaptionButton* pButton,
    int id, int bActive, int bDisabled, long long ptImage /* CPoint by value */)
{
    if (pButton == nullptr) return;

    int nState;
    if (bDisabled) {
        nState = 1;   // ImageGray
    } else {
        const int bFocused = XP_ReadInt(pButton, 0xc);
        const int bPushed  = XP_ReadInt(pButton, 0x8);
        if (bFocused == 0 && bPushed == 0) {
            const COLORREF clr = static_cast<COLORREF>(XP_ReadInt(pButton, 0x24));
            if (clr == (COLORREF)-1) {
                nState = bActive ? 3 /*ImageWhite*/ : 0 /*ImageBlack*/;
            } else {
                nState = (GetRValue(clr) > 0xc0 && GetGValue(clr) > 0xc0 && GetBValue(clr) > 0xc0)
                             ? 3 /*ImageWhite*/ : 0 /*ImageBlack*/;
            }
        } else {
            const XPColors x = XP_Colors();
            const COLORREF clr = bPushed ? x.c11c : x.c118;
            nState = (GetRValue(clr) <= 0xc0 && GetGValue(clr) <= 0xc0 && GetBValue(clr) <= 0xc0)
                         ? 3 /*ImageWhite*/ : 0 /*ImageBlack*/;
        }
    }

    const CPoint pt(static_cast<int>(static_cast<unsigned long long>(ptImage) & 0xffffffffu),
                    static_cast<int>(static_cast<unsigned long long>(ptImage) >> 32));
    const CSize sizeImage(0, 0);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCPoint__W4IMAGE_STATE_1_AEBVCSize___Z(
        pDC, id, pt, nState, sizeImage);
}

// CMFCVisualManagerOfficeXP::OnDrawComboBorder(CDC*, CRect rect, BOOL bDisabled,
//         BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton*)
// -- retail RVA 0x1afc80:
//     if (!bIsHighlighted && !bIsDropped) return;
//     rect.InflateRect(-1, -1);
//     pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);   // border colour
// bDisabled and the button pointer are not read.  +0x138 comes from
// XPBorderColor() -- see the file header.
// Symbol: ?OnDrawComboBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    int /*bDisabled*/, int bIsDropped, int bIsHighlighted,
    void* /*pButton*/ /* CMFCToolBarComboBoxButton* */)
{
    if (!bIsHighlighted && !bIsDropped) return;
    rect.InflateRect(-1, -1);
    DC_Draw3dRect(pDC, rect, XPBorderColor(), XPBorderColor());
}

// CMFCVisualManagerOfficeXP::OnDrawComboDropButton(CDC*, CRect rect, BOOL bDisabled,
//         BOOL bIsDropped, BOOL bIsHighlighted, CMFCToolBarComboBoxButton*)
// -- retail RVA 0x1afa30 (mfc140.dll):
//     COLORREF clrOldText = ::GetTextColor(pDC->m_hAttribDC);          // +0x10, 0x1afa56
//     if (!bIsDropped && !bIsHighlighted) {                            // 0x1afa64/0x1afa6d
//         ::FillRect(pDC->m_hDC, rect, afxGlobalData.brBarFace.m_hObject);       // +0x120, 0x1afaa3
//         if (CMFCToolBarImages::m_bIsDrawOnGlass) { CDrawingManager dm(*pDC);
//             dm.DrawRect(rect, afxGlobalData.clrWindow /*+0x78*/, (COLORREF)-1); }  // 0x5abf0, 0x1afaea
//         else pDC->Draw3dRect(rect, afxGlobalData.clrBarWindow, afxGlobalData.clrBarWindow); // +0x5c, 0x1afb3f
//     } else {
//         this->OnFillHighlightedArea(pDC, rect,                       // 0x1afb84
//             bIsDropped ? &m_brHighlightDn /*+0x1b0*/ : &m_brHighlight /*+0x1a0*/, NULL);
//         if (m_bIsDrawOnGlass) { dm.DrawLine(rect.left, rect.top, rect.left, rect.bottom, m_clrMenuItemBorder); }  // 0x59350
//         else { pOld = pDC->SelectObject(&m_penMenuItemBorder /*+0x1f0*/); if (!pOld) throw;
//                pDC->MoveTo(rect.left, rect.top); pDC->LineTo(rect.left, rect.bottom); pDC->SelectObject(pOld); }
//     }
//     int nState = bDisabled ? ImageGray /*1*/
//                : (bIsDropped && bIsHighlighted) ? ImageWhite /*3*/ : ImageBlack /*0*/;  // 0x1afc19..0x1afc2f
//     CMenuImages::Draw(pDC, IdArrowDown /*0*/, rect, nState, CSize(0,0));   // 0x1afc45
//     pDC->SetTextColor(clrOldText);                                   // vtable +0x70, 0x1afc57
// The combo button pointer is never read.  clrBarWindow (+0x5c) and clrWindow
// (+0x78) are both ::GetSysColor(COLOR_WINDOW) per UpdateSysColors, brBarFace is
// COLOR_BTNFACE.  Deviations: the on-glass DrawRect / DrawLine arms are not
// taken -- OpenMFC's CDrawingManager exports are stubs -- and the 1px rule is
// drawn with a local pen on both paths; the arrow is painted by XP_DrawMenuImage
// (exported CMenuImages::Draw is a stub).  Brushes/border from XP_Colors().
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    int bDisabled, int bIsDropped, int bIsHighlighted,
    void* /*pButton*/ /* CMFCToolBarComboBoxButton* */)
{
    HDC hdc = XP_Hdc(pDC);
    const COLORREF clrOldText = (pDC != nullptr && pDC->m_hAttribDC != nullptr)
                                    ? ::GetTextColor(pDC->m_hAttribDC) : 0;

    if (!bIsDropped && !bIsHighlighted) {
        XP_FillSolid(pDC, rect, GD_clrBtnFace());
        DC_Draw3dRect(pDC, rect, GD_clrWindow(), GD_clrWindow());
    } else {
        const XPColors x = XP_Colors();
        XP_FillHighlightedSolid(pDC, rect, bIsDropped ? x.c11c : x.c118);
        XP_DrawLine(pDC, rect.left, rect.top, rect.left, rect.bottom, x.c138);
    }

    const int nState = bDisabled ? 1 : ((bIsDropped && bIsHighlighted) ? 3 : 0);
    XP_DrawMenuImage(pDC, 0 /*IdArrowDown*/, rect, nState);

    if (hdc != nullptr) ::SetTextColor(hdc, clrOldText);
}

// CMFCVisualManagerOfficeXP::OnDrawEditBorder(CDC*, CRect rect, BOOL bDisabled,
//         BOOL bIsHighlighted, CMFCToolBarEditBoxButton*)
// -- retail RVA 0x1b06d0:
//     if (!CMFCToolBarEditBoxButton::m_bFlat) {
//         if (!bIsHighlighted) return;
//         ::DrawEdge(pDC->m_hDC, CRect(rect), EDGE_SUNKEN /*0x0a*/, BF_RECT /*0x0f*/);
//     } else {
//         if (!bIsHighlighted) return;
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);   // border colour
//     }
// bDisabled and the button pointer are not read; nothing is painted when the
// control is not highlighted on either path.  +0x138 comes from XPBorderColor().
// Symbol: ?OnDrawEditBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawEditBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCToolBarEditBoxButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    int /*bDisabled*/, int bIsHighlighted,
    void* /*pButton*/ /* CMFCToolBarEditBoxButton* */)
{
    if (!bIsHighlighted) return;

    if (impl__m_bFlat_CMFCToolBarEditBoxButton__1HA == 0) {
        HDC hdc = XP_Hdc(pDC);
        if (hdc == nullptr) return;
        RECT r = XP_ToRECT(rect);
        ::DrawEdge(hdc, &r, EDGE_SUNKEN, BF_RECT);
        return;
    }
    DC_Draw3dRect(pDC, rect, XPBorderColor(), XPBorderColor());
}

// CMFCVisualManagerOfficeXP::OnDrawFloatingToolbarBorder(CDC*, CMFCBaseToolBar*,
//         CRect rectBorder, CRect rectBorderSize) -- retail RVA 0x1b0360 (mfc140.dll):
//     CGdiObject* pOld = pDC->SelectObject(&m_brFloatToolBarBorder);   // +0x1d0, 0x1b0383; NULL -> throw
//     <four outer ::PatBlt(PATCOPY) runs, 0x1b03b6..0x1b0426>
//     rectBorderSize.InflateRect(-2, -2); rectBorder.InflateRect(-2, -2);   // 0x1b0437/0x1b0448
//     afxGlobalData.Initialize();
//     pDC->SelectObject(&afxGlobalData.brBarFace);                      // +0x118, 0x1b0477
//     <four inner ::PatBlt(PATCOPY) runs, 0x1b049e..0x1b0520>
//     pDC->SelectObject(pOld);
// Same eight runs as OnDrawMiniFrameBorder (XP_PatBltFrame), no runtime-class
// test, and the inner brush is brBarFace (COLOR_BTNFACE, UpdateSysColors
// 0x6b25b/0x6b25e) rather than brLight.  The toolbar pointer is never read.
// m_brFloatToolBarBorder is made locally from XP_Colors().c1d0.
// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CMFCBaseToolBar* /*pToolBar*/,
    CRect rectBorder, CRect rectBorderSize)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;

    HBRUSH hbrBorder = ::CreateSolidBrush(XP_Colors().c1d0);
    HBRUSH hbrFace   = ::CreateSolidBrush(GD_clrBtnFace());
    if (hbrBorder == nullptr || hbrFace == nullptr) {
        if (hbrBorder != nullptr) ::DeleteObject(hbrBorder);
        if (hbrFace != nullptr) ::DeleteObject(hbrFace);
        return;
    }

    HGDIOBJ hOld = ::SelectObject(hdc, hbrBorder);
    XP_PatBltFrame(hdc, rectBorder, rectBorderSize, false);

    rectBorderSize.InflateRect(-2, -2);
    rectBorder.InflateRect(-2, -2);

    ::SelectObject(hdc, hbrFace);
    XP_PatBltFrame(hdc, rectBorder, rectBorderSize, true);

    if (hOld != nullptr) ::SelectObject(hdc, hOld);
    ::DeleteObject(hbrFace);
    ::DeleteObject(hbrBorder);
}

// CMFCVisualManagerOfficeXP::OnDrawMenuArrowOnCustomizeList(CDC*, CRect rectCommand,
//         BOOL bSelected) -- retail RVA 0x1aeec0 (mfc140.dll):
//     CRect rectArrow = rectCommand;
//     rectArrow.left = rectCommand.right - CMenuImages::Size().cx;    // 0x8fbb0, 0x1aeeef..0x1aeef5
//     COLORREF clrLine; CMenuImages::IMAGE_STATE nState;
//     if (every byte of m_clrHighlight /*+0x118*/ >= 0x80) { clrLine = RGB(0,0,0);       nState = ImageBlack; }  // 0x1aeefa..0x1aef17
//     else                                                { clrLine = RGB(255,255,255); nState = ImageWhite; }  // 0x1aef19
//     CMenuImages::Draw(pDC, IdArrowRightLarge /*0xe*/, rectArrow, nState, CSize(0,0)); // 0x8fd40, 0x1aef41
//     CPen pen(PS_SOLID, 1, clrLine);                                  // 0x1aef53
//     CGdiObject* pOld = pDC->SelectObject(&pen); if (!pOld) AfxThrowInvalidArgException();
//     pDC->MoveTo(rectArrow.left - 1, rectCommand.top + 2);           // 0x1aef80
//     pDC->LineTo(rectArrow.left - 1, rectCommand.bottom - 2);        // 0x1aef93
//     pDC->SelectObject(pOld);
// bSelected (r9) is overwritten at 0x1aef12 before it is read.  Note the test
// here is `jb` (0x1aef00/0x1aef08/0x1aef10): black for >= 0x80, unlike the
// strict > 0x80 of XP_TextOverHighlight.  Deviations: CMenuImages::Size() is
// the local 9 (XP_MenuImageEdge) and the glyph is painted by XP_DrawMenuImage
// because the exported CMenuImages entry points are stubs; m_clrHighlight is
// XP_Colors().c118.
// Symbol: ?OnDrawMenuArrowOnCustomizeList@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuArrowOnCustomizeList_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rectCommand, int /*bSelected*/)
{
    CRect rectArrow(rectCommand);
    rectArrow.left = rectCommand.right - XP_MenuImageEdge();

    const COLORREF clrHighlight = XPHighlightColor();
    const bool bDark = GetRValue(clrHighlight) >= 0x80 && GetGValue(clrHighlight) >= 0x80 &&
                       GetBValue(clrHighlight) >= 0x80;
    const COLORREF clrLine = bDark ? RGB(0, 0, 0) : RGB(255, 255, 255);
    const int nState = bDark ? 0 /*ImageBlack*/ : 3 /*ImageWhite*/;

    XP_DrawMenuImage(pDC, 14 /*IdArrowRightLarge*/, rectArrow, nState);
    XP_DrawLine(pDC, rectArrow.left - 1, rectCommand.top + 2,
                rectArrow.left - 1, rectCommand.bottom - 2, clrLine);
}

// CMFCVisualManagerOfficeXP::OnDrawMenuBorder(CDC*, CMFCPopupMenu* pMenu, CRect rect)
// -- retail RVA 0x1ace60 (mfc140.dll):
//     DWORD dwExStyle = pMenu->GetExStyle();                                  // 0x2a75d0, 0x1ace96
//     pDC->Draw3dRect(rect, m_clrMenuBorder, m_clrMenuBorder);                // +0x134, 0x1aceae
//     ::InflateRect(&rect, -1, -1);                                           // 0x1acebc
//     pDC->Draw3dRect(rect, m_clrMenuLight, m_clrMenuLight);                  // +0x110, 0x1aced3
//     CRect rectLeft(1, 1, 2, rect.bottom - 1);                               // 0x1acee9..0x1acefb
//     ::FillRect(pDC->m_hDC, rectLeft, m_brBarBkgnd.m_hObject);               // +0x160, 0x1acf14
//     // (A) 0x1acf1a..0x1ad032: if pMenu->GetParentPopupMenu() (0xb7b10) is non-NULL and
//     //     that parent's m_bQuickCusomize (+0x19a8) is set, the ex-style lacks
//     //     WS_EX_LAYOUTRTL, and pMenu->m_pParentButton (+0x228) has its BOOL at +0xec set:
//     //     fill the 1px seam between the two menus (rects from ::GetWindowRect of both
//     //     windows and pMenu->GetMenuBar() [vtable +0x3a0] item geometry) with m_brBarBkgnd.
//     // (B) 0x1ad038..0x1ad1c9: if !CMFCToolBar::m_bCustomizeMode && m_bConnectMenuToParent
//     //     (+0x200) && pMenu->m_pParentButton && GetParentPopupMenu() == NULL && the parent
//     //     button's virtual at vtable +0x1d8 returns non-zero: map the parent button's
//     //     m_rect (+0x68) into pMenu's client space and, switching on the drop direction
//     //     at pMenu->+0x1650, paint a 1px strip of m_brBarBkgnd across the shared edge
//     //     (with a width test against rect.Height()+2 for the horizontal directions).
// Only the unconditional prefix -- the two frames and the left strip -- is
// reproduced.  DEVIATION: (A) and (B) are omitted: they read CMFCToolBarMenuButton
// +0xec (inside OpenMFC's padding), dispatch two CMFCPopupMenu / menu-button
// virtuals this build cannot reach, and CMFCPopupMenu::GetParentPopupMenu has
// no usable OpenMFC counterpart; each one is a cosmetic seam fill between a
// popup and its parent.  Colours from XP_Colors() (c134 / c110 / c108).  The
// menu pointer is only needed by (A)/(B) and is therefore unused here.
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CMFCPopupMenu* /*pMenu*/, CRect rect)
{
    const XPColors x = XP_Colors();
    DC_Draw3dRect(pDC, rect, x.c134, x.c134);
    rect.InflateRect(-1, -1);
    DC_Draw3dRect(pDC, rect, x.c110, x.c110);

    const CRect rectLeft(1, 1, 2, rect.bottom - 1);
    XP_FillSolid(pDC, rectLeft, x.c108);
}

// CMFCVisualManagerOfficeXP::OnDrawMenuLabel(CDC*, CRect rect) -- retail RVA 0x1b2e20:
//     ::FillRect(pDC->m_hDC, rect, this->[+0x160].m_hObject);   // bar-background brush
//     CRect rectEdge(rect); rectEdge.top = rect.bottom - 2;
//     pDC->Draw3dRect(rectEdge, afxGlobalData.clrBarShadow (+0x64),
//                               afxGlobalData.clrBarHilite (+0x68));
//     afxGlobalData.Initialize();
//     return afxGlobalData.clrBarText (+0x74);
// The brush at +0x160 is ::CreateSolidBrush(this->[+0x108]), built by the call
// at 0x1ac8ed inside OnUpdateSystemColors (0x1ac0e0); XPBarBkgndColor() stands
// in for it -- see the file header.
// Symbol: ?OnDrawMenuLabel@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawMenuLabel_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect)
{
    XP_FillSolid(pDC, rect, XPBarBkgndColor());

    CRect rectEdge(rect);
    rectEdge.top = rect.bottom - 2;
    DC_Draw3dRect(pDC, rectEdge, GD_clrBtnShadow(), GD_clrBarHilite());

    return GD_clrBtnText();
}


// CMFCVisualManagerOfficeXP::OnDrawMenuResizeBar(CDC*, CRect rect,
//         int nResizeBarHeight) -- retail RVA 0x1af250:
//     ::FillRect(pDC->m_hDC, rect, this->[+0x180].m_hObject);         // +0x110 brush
//     ::InflateRect(rect, -1, -1);                                    // 0x1af2a8
//     this->OnFillHighlightedArea(pDC, rect, &this->[+0x160], NULL);  // vtable +0x638
//     CRect rectGripper(rect);
//     if (nResizeBarHeight == 1 || nResizeBarHeight == 3) {           // test at 0x1af2ec
//         rectGripper.left = rectGripper.right + rectGripper.top - rectGripper.bottom;
//     } else {
//         int cy = rect.bottom - rect.top;
//         rectGripper.left  = (rect.left + rect.right) / 2 - cy / 2;
//         rectGripper.right = rectGripper.left + cy;
//     }
//     ::InflateRect(rectGripper, -2, -2);                             // 0x1af344
//     if (this->[+0x140].m_hObject == NULL) this->CreateGripperBrush();  // vtable +0x620
//     COLORREF clrOldText = pDC->SetTextColor(afxGlobalData.clrBarShadow /*+0x64*/); // vtable +0x70
//     COLORREF clrOldBk   = pDC->SetBkColor(this->[+0x108]);                         // vtable +0x68
//     ::FillRect(pDC->m_hDC, rectGripper, this->[+0x140].m_hObject);
//     pDC->SetTextColor(clrOldText);
//     pDC->SetBkColor(clrOldBk);
// The `test $0xfffffffd,%eax` on nResizeBarHeight - 1 at 0x1af2ec is zero only
// for nResizeBarHeight 1 and 3.  CDC's vtable, at image address 0x180338fb0
// (installed by ??0CDC@@QEAA@XZ at 0x2a02e0), has SetBkColor at +0x68 and
// SetTextColor at +0x70, so the dark clrBarShadow is the pattern's text colour
// and the +0x108 bar background is its background colour.
// Deviations, all forced by the missing member storage: the +0x160 fill brush
// and the +0x140 gripper pattern brush are built locally (see
// XP_CreateGripperBrush), and the two colour setters go straight to
// ::SetTextColor / ::SetBkColor on the HDC instead of through CDC's vtable.
// Symbol: ?OnDrawMenuResizeBar@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuResizeBar_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect, int nResizeBarHeight)
{
    const XPColors x = XP_Colors();

    XP_FillSolid(pDC, rect, x.c110);
    rect.InflateRect(-1, -1);
    XP_FillHighlightedSolid(pDC, rect, x.c108);

    CRect rectGripper(rect);
    if (nResizeBarHeight == 1 || nResizeBarHeight == 3) {
        rectGripper.left = rectGripper.right + rectGripper.top - rectGripper.bottom;
    } else {
        const int cy = rect.bottom - rect.top;
        rectGripper.left  = (rect.left + rect.right) / 2 - cy / 2;
        rectGripper.right = rectGripper.left + cy;
    }
    rectGripper.InflateRect(-2, -2);

    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;

    HBRUSH hbrGripper = XP_CreateGripperBrush();
    if (hbrGripper == nullptr) return;

    const COLORREF clrOldText = ::SetTextColor(hdc, GD_clrBtnShadow());
    const COLORREF clrOldBk   = ::SetBkColor(hdc, x.c108);

    RECT r = XP_ToRECT(rectGripper);
    ::FillRect(hdc, &r, hbrGripper);

    ::SetTextColor(hdc, clrOldText);
    ::SetBkColor(hdc, clrOldBk);
    ::DeleteObject(hbrGripper);
}

// CMFCVisualManagerOfficeXP::OnDrawMenuScrollButton(CDC*, CRect rect,
//         BOOL bIsScrollDown, BOOL bIsHighlited, BOOL bIsPressed, BOOL bIsDisabled)
// -- retail RVA 0x1af430:
//     rect.top--;
//     ::FillRect(pDC->m_hDC, rect, afxGlobalData.brBarFace (+0x118).m_hObject);
//     CSize sizeImage(0, 0);
//     CMenuImages::Draw(pDC, bIsScrollDown ? CMenuImages::IdArrowDown
//                                          : CMenuImages::IdArrowUp,
//                       rect, CMenuImages::ImageBlack, sizeImage);
//     if (!bIsHighlited) return;                       // 4th BOOL, at [rsp+0x90]
//     CPen pen(PS_SOLID, 1, afxGlobalData.clrBarShadow (+0x64));
//     CGdiObject* pOldPen   = pDC->SelectObject(&pen);
//     CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
//     rect.InflateRect(-1, -1);
//     ::RoundRect(pDC->m_hDC, rect.left, rect.top, rect.right, rect.bottom, 2, 2);
//     pDC->SelectObject(pOldBrush); pDC->SelectObject(pOldPen);
// bIsPressed and bIsDisabled are not read.  Deviation: OpenMFC's exported
// CMenuImages::Draw is a no-op stub, so the arrow is painted with the local
// XP_DrawArrow() helper instead (same up/down choice, drawn in black, which is
// what CMenuImages::ImageBlack means).
// Symbol: ?OnDrawMenuScrollButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHH@Z
extern "C" void MS_ABI impl__OnDrawMenuScrollButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    int bIsScrollDown, int bIsHighlited, int /*bIsPressed*/, int /*bIsDisabled*/)
{
    rect.top--;

    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;

    RECT r = XP_ToRECT(rect);
    ::FillRect(hdc, &r, ::GetSysColorBrush(COLOR_BTNFACE));

    XP_DrawArrow(pDC, rect, bIsScrollDown != 0, RGB(0, 0, 0));

    if (!bIsHighlited) return;

    HPEN pen = ::CreatePen(PS_SOLID, 1, GD_clrBtnShadow());
    HGDIOBJ oldPen = (pen != nullptr) ? ::SelectObject(hdc, pen) : nullptr;
    HGDIOBJ oldBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
    ::InflateRect(&r, -1, -1);
    ::RoundRect(hdc, r.left, r.top, r.right, r.bottom, 2, 2);
    if (oldBrush != nullptr) ::SelectObject(hdc, oldBrush);
    if (oldPen != nullptr) ::SelectObject(hdc, oldPen);
    if (pen != nullptr) ::DeleteObject(pen);
}

// Retail (RVA 0x1ad1f0, mfc140.dll) is a CDrawingManager::DrawShadow driver:
// both of its painting arms end in a call to 0x58c80
// (?DrawShadow@CDrawingManager@@QEAAHVCRect@@HHHPEAVCBitmap@@1KH@Z, at 0x1ad291
// and 0x1ad410) that caches into the caller's CBitmap objects.  OpenMFC's
// exported DrawShadow is a `return 0` stub (phase4/src/core/gdi/
// CDrawingManager.cpp), so there is nothing to drive; left a stub.
// Symbol: ?OnDrawMenuShadow@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEBVCRect@@1HHHPEAVCBitmap@@2H@Z
extern "C" void MS_ABI impl__OnDrawMenuShadow_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEBVCRect__1HHHPEAVCBitmap__2H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, const CRect* /*rectClient*/, const CRect* /*rectExclude*/,
    int /*nDepth*/, int /*iMinBrightness*/, int /*iMaxBrightness*/,
    CBitmap* /*pBmpSaveBottom*/, CBitmap* /*pBmpSaveRight*/, int /*bRTL*/) {}


// CMFCVisualManagerOfficeXP::OnDrawMenuSystemButton(CDC*, CRect rect,
//         UINT uiSystemCommand, UINT nStyle, BOOL bHighlight)
// -- retail RVA 0x1af880:
//     int nImage;
//     switch (uiSystemCommand) {                      // 0x1af899..0x1af8c3
//         case 0xf020 /*SC_MINIMIZE*/: nImage = 3; break;
//         case 0xf060 /*SC_CLOSE*/:    nImage = 5; break;
//         case 0xf120 /*SC_RESTORE*/:  nImage = 4; break;
//         default: return;                            // jne 0x1af970 == the epilogue
//     }
//     BOOL bDisabled = nStyle & TBBS_DISABLED /*0x40000*/;
//     if (bHighlight && !bDisabled) {
//         this->OnFillHighlightedArea(pDC, rect,      // vtable +0x638
//             (nStyle & TBBS_PRESSED /*0x20000*/) ? &this->[+0x1b0] : &this->[+0x1a0], NULL);
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);
//     }
//     int nState = bDisabled ? 1 : (bHighlight ? 3 : 0);
//     CMenuImages::Draw(pDC, (IMAGES_IDS)nImage, rect, (IMAGE_STATE)nState, CSize(0, 0));
// The brush index is the `(nStyle & 0x20000 | 0x340000) >> 13` trick at
// 0x1af8f2..0x1af918, which yields 0x1a0 or 0x1b0; the state is the
// `neg/sbb/and $3` at 0x1af93c plus the `cmovne` on bDisabled at 0x1af95d.
// The CMenuImages::Draw call is reached on every non-default arm, including
// the one where nothing was filled.
// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@IIH@Z
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    unsigned int uiSystemCommand, unsigned int nStyle, int bHighlight)
{
    int nImage;
    switch (uiSystemCommand) {
    case 0xf020u: nImage = 3; break;    // SC_MINIMIZE
    case 0xf060u: nImage = 5; break;    // SC_CLOSE
    case 0xf120u: nImage = 4; break;    // SC_RESTORE
    default:      return;
    }

    const bool bDisabled = (nStyle & 0x00040000u) != 0;

    if (bHighlight && !bDisabled) {
        const XPColors x = XP_Colors();
        XP_FillHighlightedSolid(pDC, rect, (nStyle & 0x00020000u) != 0 ? x.c11c : x.c118);
        DC_Draw3dRect(pDC, rect, x.c138, x.c138);
    }

    const int nState = bDisabled ? 1 : (bHighlight ? 3 : 0);
    const CSize sizeImage(0, 0);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
        pDC, nImage, rect, nState, sizeImage);
}

// CMFCVisualManagerOfficeXP::OnDrawMiniFrameBorder(CDC*, CPaneFrameWnd*,
//         CRect rectBorder, CRect rectBorderSize) -- retail RVA 0x1b0120 (mfc140.dll):
//     if (!pFrameWnd->IsKindOf(RUNTIME_CLASS(CMFCTasksPaneFrameWnd))) {   // 0x1803aa5c8, 0x1b014b
//         CMFCVisualManager::OnDrawMiniFrameBorder(pDC, pFrameWnd, rectBorder, rectBorderSize);  // 0x1864c0
//         return;
//     }
//     CGdiObject* pOld = pDC->SelectObject(&m_brFloatToolBarBorder);   // +0x1d0, 0x1b0162; NULL -> throw
//     <four outer ::PatBlt(PATCOPY) runs, 0x1b0195..0x1b0207>
//     rectBorderSize.InflateRect(-2, -2); rectBorder.InflateRect(-2, -2);   // 0x1b021a/0x1b0228
//     pDC->SelectObject(&afxGlobalData.brLight);                        // +0xc8, 0x1b0257
//     <four inner ::PatBlt(PATCOPY) runs, 0x1b027e..0x1b0302>
//     pDC->SelectObject(pOld);
// The runs are, outer: (l, t, size.l, H) (l, t, W, size.t) (r - size.r, t, size.r, H)
// (l, b - size.b, W, size.b); inner, on the deflated rects: (l, t+1, size.l, H-2)
// (l+1, t, W-2, size.t) (r - size.r, t+1, size.r, H-2) (l+1, b - size.b, W-2, size.b)
// -- transcribed in XP_PatBltFrame.  m_brFloatToolBarBorder is ::CreateSolidBrush
// of XP_Colors().c1d0 (0x1ac9c9) and brLight is GD_clrLightMidpoint() (see
// its comment for the 8-bpp deviation); both are made locally.  Retail
// dereferences pFrameWnd unconditionally; the null guard is OpenMFC's.
// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CPaneFrameWnd* pFrameWnd,
    CRect rectBorder, CRect rectBorderSize)
{
    if (pFrameWnd == nullptr ||
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pFrameWnd),
            impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ()) == 0) {
        impl__OnDrawMiniFrameBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(
            pThis, pDC, pFrameWnd, rectBorder, rectBorderSize);
        return;
    }

    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;

    HBRUSH hbrBorder = ::CreateSolidBrush(XP_Colors().c1d0);
    HBRUSH hbrLight  = ::CreateSolidBrush(GD_clrLightMidpoint());
    if (hbrBorder == nullptr || hbrLight == nullptr) {
        if (hbrBorder != nullptr) ::DeleteObject(hbrBorder);
        if (hbrLight != nullptr) ::DeleteObject(hbrLight);
        return;
    }

    HGDIOBJ hOld = ::SelectObject(hdc, hbrBorder);
    XP_PatBltFrame(hdc, rectBorder, rectBorderSize, false);

    rectBorderSize.InflateRect(-2, -2);
    rectBorder.InflateRect(-2, -2);

    ::SelectObject(hdc, hbrLight);
    XP_PatBltFrame(hdc, rectBorder, rectBorderSize, true);

    if (hOld != nullptr) ::SelectObject(hdc, hOld);
    ::DeleteObject(hbrLight);
    ::DeleteObject(hbrBorder);
}

// CMFCVisualManagerOfficeXP::OnDrawPaneBorder(CDC*, CBasePane*, CRect&)
// -- retail RVA 0x1ad500:
//     if (pBar->m_bIsDlgControl (+0xf4)) {
//         CMFCVisualManager::OnDrawPaneBorder(pDC, pBar, rect);   // 0x182df0
//         return;
//     }
//     DWORD dwStyle = pBar->vtbl[+0x390]();          // see below
//     if ((dwStyle & CBRS_BORDER_ANY /*0x0f00*/) == 0) return;
//     COLORREF clrOldBk = ::GetBkColor(pDC->m_hAttribDC);
//     if (dwStyle & CBRS_BORDER_LEFT)   pDC->FillSolidRect(0, 0, 1, rect.Height()-1, clrBarFace);
//     if (dwStyle & CBRS_BORDER_TOP)    pDC->FillSolidRect(0, 0, rect.Width()-1, 1, clrBarFace);
//     if (dwStyle & CBRS_BORDER_RIGHT)  pDC->FillSolidRect(rect.right, 0, -1, rect.Height(), clrBarFace);
//     if (dwStyle & CBRS_BORDER_BOTTOM) pDC->FillSolidRect(0, rect.bottom, rect.Width()-1, -1, clrBarFace);
//     if (dwStyle & CBRS_BORDER_LEFT)   rect.left++;
//     if (dwStyle & CBRS_BORDER_TOP)    rect.top++;
//     if (dwStyle & CBRS_BORDER_RIGHT)  rect.right--;
//     if (dwStyle & CBRS_BORDER_BOTTOM) rect.bottom--;
//     pDC->SetBkColor(clrOldBk);                     // virtual, vtable +0x68
// The x/y origins really are 0 rather than rect.left/rect.top -- retail passes
// literal zeros to CDC::FillSolidRect there.
// Deviations: the style word comes from pBar->m_dwStyle (+0x104) instead of the
// virtual at CBasePane vtable +0x390, because that virtual's retail body is
// exactly `{ return this->m_dwStyle; }` (RVA 0x8840) and OpenMFC's CBasePane
// vtable has a different shape; and the GetBkColor/SetBkColor pair is dropped
// because the fills here use ::FillRect with a temporary brush rather than the
// ExtTextOut(ETO_OPAQUE) that makes retail's CDC::FillSolidRect clobber the
// DC background colour in the first place.
// Symbol: ?OnDrawPaneBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CBasePane* pBar, CRect* pRect)
{
    if (pBar == nullptr || pRect == nullptr) return;

    if (pBar->m_bIsDlgControl) {
        impl__OnDrawPaneBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
            pThis, pDC, pBar, pRect);
        return;
    }

    const DWORD dwStyle = pBar->m_dwStyle;
    if ((dwStyle & kXPBorderAny) == 0) return;

    const COLORREF clrBarFace = GD_clrBtnFace();
    CRect& rect = *pRect;

    if (dwStyle & kXPBorderLeft) {
        DC_FillSolidRect(pDC, 0, 0, 1, rect.bottom - rect.top - 1, clrBarFace);
    }
    if (dwStyle & kXPBorderTop) {
        DC_FillSolidRect(pDC, 0, 0, rect.right - rect.left - 1, 1, clrBarFace);
    }
    if (dwStyle & kXPBorderRight) {
        DC_FillSolidRect(pDC, rect.right, 0, -1, rect.bottom - rect.top, clrBarFace);
    }
    if (dwStyle & kXPBorderBottom) {
        DC_FillSolidRect(pDC, 0, rect.bottom, rect.right - rect.left - 1, -1, clrBarFace);
    }

    if (dwStyle & kXPBorderLeft)   rect.left++;
    if (dwStyle & kXPBorderTop)    rect.top++;
    if (dwStyle & kXPBorderRight)  rect.right--;
    if (dwStyle & kXPBorderBottom) rect.bottom--;
}

// CMFCVisualManagerOfficeXP::OnDrawPaneCaption(CDC*, CDockablePane*, BOOL bActive,
//                                              CRect rectCaption, CRect rectButtons)
// -- retail RVA 0x1ae8e0:
//     CPen pen(PS_SOLID, 1, bActive ? afxGlobalData.clrBarLight  (+0x70)
//                                   : afxGlobalData.clrBarShadow (+0x64));
//     CGdiObject* pOldPen   = pDC->SelectObject(&pen);
//     CGdiObject* pOldBrush = pDC->SelectObject(bActive ? &afxGlobalData.brActiveCaption (+0xe8)
//                                                       : &afxGlobalData.brBarFace       (+0x118));
//     if (bActive) rectCaption.InflateRect(1, 1);
//     ::RoundRect(pDC->m_hDC, rectCaption.left, rectCaption.top,
//                 rectCaption.right, rectCaption.bottom, 2, 2);
//     pDC->SelectObject(pOldBrush);
//     pDC->SelectObject(pOldPen);
//     return bActive ? afxGlobalData.clrCaptionText (+0x80)
//                    : afxGlobalData.clrBarText     (+0x74);
// The rectangle painted is the 4th argument (rectCaption); rectButtons and the
// pane pointer are not read.  brActiveCaption/brBarFace are the solid brushes
// AFX_GLOBAL_DATA::UpdateSysColors creates from clrActiveCaption (0x6b27e) and
// clrBarFace (0x6b25b), i.e. ::GetSysColorBrush(COLOR_ACTIVECAPTION) and
// ::GetSysColorBrush(COLOR_BTNFACE); GDI is driven directly here because the
// CDC/CPen methods have no linkable definition inside this DLL.
// Symbol: ?OnDrawPaneCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCDockablePane@@HVCRect@@2@Z
extern "C" unsigned long MS_ABI impl__OnDrawPaneCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCDockablePane__HVCRect__2_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, void* /*pBar*/ /* CDockablePane* */,
    int bActive, CRect rectCaption, CRect /*rectButtons*/)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc != nullptr) {
        HPEN pen = ::CreatePen(PS_SOLID, 1, bActive ? GD_clrBarLight() : GD_clrBtnShadow());
        HGDIOBJ oldPen = (pen != nullptr) ? ::SelectObject(hdc, pen) : nullptr;
        HGDIOBJ oldBrush = ::SelectObject(
            hdc, ::GetSysColorBrush(bActive ? COLOR_ACTIVECAPTION : COLOR_BTNFACE));

        RECT r = XP_ToRECT(rectCaption);
        if (bActive) ::InflateRect(&r, 1, 1);
        ::RoundRect(hdc, r.left, r.top, r.right, r.bottom, 2, 2);

        if (oldBrush != nullptr) ::SelectObject(hdc, oldBrush);
        if (oldPen != nullptr) ::SelectObject(hdc, oldPen);
        if (pen != nullptr) ::DeleteObject(pen);
    }
    return bActive ? GD_clrCaptionText() : GD_clrBtnText();
}


// CMFCVisualManagerOfficeXP::OnDrawPopupWindowBorder(CDC*, CRect rect)
// -- retail RVA 0x1b18c0:
//     pDC->Draw3dRect(rect, this->[+0x134], this->[+0x134]);   // 0x1b18e8
//     ::InflateRect(rect, -1, -1);                             // 0x1b18f6
//     pDC->Draw3dRect(rect, this->[+0x110], this->[+0x110]);   // 0x1b190c
// Both colours come from XP_Colors().
// Symbol: ?OnDrawPopupWindowBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect)
{
    const XPColors x = XP_Colors();
    DC_Draw3dRect(pDC, rect, x.c134, x.c134);
    rect.InflateRect(-1, -1);
    DC_Draw3dRect(pDC, rect, x.c110, x.c110);
}

// CMFCVisualManagerOfficeXP::OnDrawPopupWindowButtonBorder(CDC*, CRect rect,
//         CMFCDesktopAlertWndButton* pButton) -- retail RVA 0x195f00:
//     if (pButton->[+0x11c] != 0 || pButton->[+0x114] != 0 || pButton->[+0xb20] != 0)
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);
// (the three compares are at 0x195f0c / 0x195f15 / 0x195f1e and every one of
// them jumps to the same Draw3dRect at 0x195f37, so it is a plain OR.)
// All three offsets are already pinned by shadow structs this repo carries:
// +0x114 is CMFCButton::m_bHover and +0x11c CMFCButton::m_bClickStarted
// (phase4/src/detail/CMFCButtonSupport.h), and +0xb20 == 2848 is
// CMFCDesktopAlertWndButton::m_bIsCaptionButton, the first member the derived
// class appends after the 2848-byte CMFCButton base
// (phase4/src/detail/CMFCDesktopAlertWndButtonSupport.h).  Member +0x138 is the
// OfficeXP border colour, XPBorderColor() -- see the file header.
// Retail dereferences pButton unconditionally; the null guard here is OpenMFC's.
// Symbol: ?OnDrawPopupWindowButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    void* pButton /* CMFCDesktopAlertWndButton* */)
{
    if (pButton == nullptr) return;

    const char* const p = static_cast<const char*>(pButton);
    const int bHover        = *reinterpret_cast<const int*>(p + kXPBtnHover);
    const int bClickStarted = *reinterpret_cast<const int*>(p + kXPBtnClickStarted);
    const int bIsCaption    = *reinterpret_cast<const int*>(p + kXPAlertBtnIsCaption);

    if (bClickStarted == 0 && bHover == 0 && bIsCaption == 0) return;

    DC_Draw3dRect(pDC, rect, XPBorderColor(), XPBorderColor());
}

// CMFCVisualManagerOfficeXP::OnDrawPopupWindowCaption(CDC*, CRect rectCaption,
//         CMFCDesktopAlertWnd*) -- retail RVA 0x1b1930:
//     ::FillRect(pDC->m_hDC, rectCaption, this->[+0x1a0].m_hObject);  // highlight brush
//     afxGlobalData.Initialize();
//     return afxGlobalData.clrBarText (+0x74);
// The alert-window pointer is not read.  The +0x1a0 brush is
// ::CreateSolidBrush(this->[+0x118]), built by the call at 0x1ac93e inside
// OnUpdateSystemColors (0x1ac0e0); XPHighlightColor() stands in for it -- see
// the file header.
// Symbol: ?OnDrawPopupWindowCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWnd@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawPopupWindowCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCMFCDesktopAlertWnd___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rectCaption,
    void* /*pPopupWnd*/ /* CMFCDesktopAlertWnd* */)
{
    XP_FillSolid(pDC, rectCaption, XPHighlightColor());
    return GD_clrBtnText();
}

// Retail (RVA 0x1b21d0, mfc140.dll) drives everything off CMFCRibbonButton
// virtuals -- vtable +0x1d0 (three calls) and +0x1c8 (two) -- and the element's
// +0x100 member, before framing with m_clrMenuItemBorder (+0x138, read three
// times) through CDC::Draw3dRect / MoveTo / LineTo.  OpenMFC's CMFCRibbonButton
// is a 64-byte placeholder over a CMFCRibbonBaseElement that models only m_nID
// and m_strText (include/openmfc/afxmfc.h), with an unrelated vtable, so
// neither the state virtuals nor the member can be reached; left a stub.
// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CMFCRibbonButton* /*pButton*/) {}

// CMFCVisualManagerOfficeXP::OnDrawRibbonCategoryScroll(CDC*, CRibbonCategoryScroll*)
// -- retail RVA 0x1b24d0 (mfc140.dll):
//     CRect rect = pScroll->m_rect; rect.bottom--;                       // +0xc8, 0x1b24e6..0x1b24ff
//     this->OnFillHighlightedArea(pDC, rect,                            // vtable +0x638, 0x1b2575
//         pScroll->IsHighlighted() /*vtable +0x1b0*/ ? &m_brHighlight : &afxGlobalData.brBarFace, NULL);
//     BOOL bLeft = pScroll->m_bIsLeft;                                  // +0x270, 0x1b2582
//     if (afxGlobalData.m_bIsRTL /*+0x274*/) bLeft = !bLeft;            // 0x1b25a0..0x1b25b0
//     CMenuImages::Draw(pDC, bLeft ? IdArrowLeftLarge /*17*/ : IdArrowRightLarge /*14*/,
//                       rect, ImageBlack, CSize(0,0));                  // 0x1b25da
//     pDC->Draw3dRect(rect, afxGlobalData.clrBarShadow, afxGlobalData.clrBarShadow);  // +0x64, 0x1b2636
// The +0x270 flag IS pinned (S_Cribboncategoryscroll::m_bIsLeft in
// phase4/src/detail/CRibbonCategoryScrollSupport.h), but the element rect at
// +0xc8 lies in that struct's unnamed padding and IsHighlighted is a
// CMFCRibbonBaseElement virtual OpenMFC's vtable does not carry, so the
// geometry and the fill choice cannot be obtained; left a stub.
// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, void* /*pScroll*/ /* CRibbonCategoryScroll* */) {}

// Retail (RVA 0x1b2b10, mfc140.dll) fills the swatch through the visual-manager
// virtual at +0x638 (two calls) with m_brHighlight (+0x1a0), then builds local
// CPen / CBrush objects (0x2a1ed0 / 0x2a1fa0) and draws the box edges with four
// CDC::MoveTo / LineTo pairs and three Draw3dRect calls, choosing between them
// on bDrawTopEdge / bDrawBottomEdge / bIsHighlighted / bIsChecked / bIsDisabled
// and the caller's colour.  Not yet transcribed: the 205-instruction body
// interleaves the five flags with the CMFCRibbonGalleryIcon pointer, whose
// class OpenMFC does not declare at all; left a stub rather than a partial
// reading.
// Symbol: ?OnDrawRibbonColorPaletteBox@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonColorButton@@PEAVCMFCRibbonGalleryIcon@@KVCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonColorPaletteBox_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonColorButton__PEAVCMFCRibbonGalleryIcon__KVCRect__HHHHH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, void* /*pColorButton*/, void* /*pIcon*/,
    unsigned long /*color*/, CRect /*rect*/, int /*bDrawTopEdge*/, int /*bDrawBottomEdge*/,
    int /*bIsHighlighted*/, int /*bIsChecked*/, int /*bIsDisabled*/) {}

// CMFCVisualManagerOfficeXP::OnDrawRibbonMenuCheckFrame(CDC*, CMFCRibbonButton*,
//         CRect rect) -- retail RVA 0x1b2650 (mfc140.dll):
//     this->OnFillHighlightedArea(pDC, rect, &this->[+0x1a0], NULL);  // vtable +0x638
//     pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);          // border colour
// The button pointer is not read.  The +0x638 virtual is
// CMFCVisualManagerOfficeXP::OnFillHighlightedArea, whose body is
// XP_FillHighlighted / XP_FillHighlightedSolid in this file (it honours
// CMFCToolBarImages::m_bIsDrawOnGlass); the +0x1a0 brush is
// ::CreateSolidBrush(+0x118) = XPHighlightColor(), and +0x138 is
// XPBorderColor().
// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, void* /*pButton*/ /* CMFCRibbonButton* */,
    CRect rect)
{
    XP_FillHighlightedSolid(pDC, rect, XPHighlightColor());
    DC_Draw3dRect(pDC, rect, XPBorderColor(), XPBorderColor());
}


// CMFCVisualManagerOfficeXP::OnDrawRibbonQuickAccessToolBarSeparator(CDC*,
//         CMFCRibbonSeparator* pSeparator, CRect rect) -- retail RVA 0x1b2a40:
//     int x  = (rect.left + rect.right) / 2;
//     int y1 = rect.top;
//     int y2 = rect.bottom - 1;
//     if (CMFCToolBarImages::m_bIsDrawOnGlass) {                  // test at 0x1b2a6f
//         CDrawingManager dm(*pDC);
//         dm.DrawLine(x, y1, x, y2, this->[+0x12c]);              // 0x59350
//     } else {
//         CGdiObject* pOld = pDC->SelectObject(&this->[+0x1e0]);  // the +0x12c pen
//         if (pOld == NULL) AfxThrowInvalidArgException();
//         pDC->MoveTo(x, y1); pDC->LineTo(x, y2);
//         pDC->SelectObject(pOld);
//     }
// The separator element pointer is never read -- the geometry all comes from
// the CRect argument (0x1b2a56..0x1b2a6c).  The pen at +0x1e0 is
// ::CreatePen(PS_SOLID, 1, this->[+0x12c]), made by the call at 0x1ac9b2.
// Both arms put the same 1px line at the same place in the same colour, so this
// build draws it once with a locally created pen; the difference is only that
// CDrawingManager::DrawLine would blend it for a glass background.
// Symbol: ?OnDrawRibbonQuickAccessToolBarSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonSeparator@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonQuickAccessToolBarSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonSeparator__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC,
    void* /*pSeparator*/ /* CMFCRibbonSeparator* */, CRect rect)
{
    const int x = (rect.left + rect.right) / 2;
    XP_DrawLine(pDC, x, rect.top, x, rect.bottom - 1, XP_Colors().c12c);
}

// CMFCVisualManagerOfficeXP::OnDrawRibbonRecentFilesFrame(CDC*,
//         CMFCRibbonMainPanel*, CRect rect) -- retail RVA 0x1b26e0:
//     ::FillRect(pDC->m_hDC, rect, this->[+0x160].m_hObject);   // bar-background brush
//     CRect rectEdge(rect); rectEdge.right = rect.left + 2;
//     pDC->Draw3dRect(rectEdge, afxGlobalData.clrBarShadow (+0x64),
//                               afxGlobalData.clrBarHilite (+0x68));
// The panel pointer is not read.  Same +0x160 substitution as OnDrawMenuLabel.
// Symbol: ?OnDrawRibbonRecentFilesFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonRecentFilesFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, void* /*pPanel*/ /* CMFCRibbonMainPanel* */,
    CRect rect)
{
    XP_FillSolid(pDC, rect, XPBarBkgndColor());

    CRect rectEdge(rect);
    rectEdge.right = rect.left + 2;
    DC_Draw3dRect(pDC, rectEdge, GD_clrBtnShadow(), GD_clrBarHilite());
}

// Retail (RVA 0x1b2790, mfc140.dll) dispatches two CMFCRibbonStatusBarPane
// virtuals (vtable +0x1b0 and +0x1c0), fills with m_brHighlight (+0x1a0) or
// leaves the background, frames with m_clrMenuItemBorder (+0x138) or
// afxGlobalData colours through two Draw3dRect calls, and returns a text
// colour.  OpenMFC's CMFCRibbonStatusBarPane is a placeholder (CMFCRibbonBaseElement
// plus 32 bytes of padding) with an unrelated vtable, so the two state
// virtuals cannot be dispatched; left a stub.  The `return 0` is the
// generated stub's value, not a retail one.
// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CMFCRibbonStatusBar* /*pBar*/, CMFCRibbonStatusBarPane* /*pPane*/) {
    return 0;
}

// CMFCVisualManagerOfficeXP::OnDrawScrollButtons(CDC*, const CRect& rect,
//         const int nBorderSize, int iImage, BOOL bHilited)
// -- retail RVA 0x1b0fb0:
//     CRect rectFill(rect); rectFill.top = rect.top - nBorderSize;
//     ::FillRect(pDC->m_hDC, rectFill, afxGlobalData.brWindow (+0x108).m_hObject);
//     if (bHilited) {
//         ::FillRect(pDC->m_hDC, rect, this->[+0x1a0].m_hObject);   // highlight brush
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);    // border colour
//     }
//     CSize sizeImage(0, 0);
//     CMenuImages::Draw(pDC, (CMenuImages::IMAGES_IDS)iImage, rect,
//                       CMenuImages::ImageBlack, sizeImage);
// afxGlobalData.brWindow is the solid brush AFX_GLOBAL_DATA::UpdateSysColors
// (0x6afd0) builds from clrWindow at 0x6b310, i.e. the same colour as
// ::GetSysColorBrush(COLOR_WINDOW).  The two OfficeXP
// members come from XPHighlightColor()/XPBorderColor() (see the file header).
// Deviation: the glyph is not drawn -- iImage is a caller-supplied
// CMenuImages::IMAGES_IDS and OpenMFC's exported CMenuImages::Draw is a no-op
// stub, so there is nothing faithful to substitute for an arbitrary id.
// Symbol: ?OnDrawScrollButtons@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEBVCRect@@HHH@Z
extern "C" void MS_ABI impl__OnDrawScrollButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEBVCRect__HHH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, const CRect* pRect,
    int nBorderSize, int /*iImage*/, int bHilited)
{
    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr || pRect == nullptr) return;

    RECT rectFill = XP_ToRECT(*pRect);
    rectFill.top = pRect->top - nBorderSize;
    ::FillRect(hdc, &rectFill, ::GetSysColorBrush(COLOR_WINDOW));

    if (bHilited) {
        XP_FillSolid(pDC, *pRect, XPHighlightColor());
        DC_Draw3dRect(pDC, *pRect, XPBorderColor(), XPBorderColor());
    }
}

// CMFCVisualManagerOfficeXP::OnDrawSeparator(CDC*, CBasePane* pBar, CRect rect, BOOL bHorz)
// -- retail RVA 0x1ada00 (mfc140.dll):
//     if (pBar->m_bIsDlgControl /*+0xf4*/) {                                   // 0x1ada2e
//         CMFCVisualManager::OnDrawSeparator(pDC, pBar, rect, bHorz); return; }  // 0x183750
//     CGdiObject* pOld = pDC->SelectObject(&m_penSeparator);                   // +0x1e0, 0x1ada65; NULL -> throw
//     int x1, y1, x2, y2;
//     if (bHorz) { x1 = x2 = (rect.left + rect.right) / 2; y1 = rect.top; y2 = rect.bottom - 1; }  // 0x1ada81..0x1ada98
//     else {
//         y1 = y2 = (rect.top + rect.bottom) / 2; x1 = rect.left; x2 = rect.right;              // 0x1adaa0..0x1adabb
//         BOOL bRibbonMenu = pBar->IsKindOf(RUNTIME_CLASS(CMFCRibbonPanelMenuBar))             // 0x307f90, 0x1adabe
//                            && !((CMFCRibbonPanelMenuBar*)pBar)->m_bIsDefaultMenuLook;       // +0x142c, 0x1adac9
//         if (pBar->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar)) && !bRibbonMenu               // 0x3aa478, 0x1adadc
//             && !pBar->IsKindOf(RUNTIME_CLASS(CMFCColorBar))) {                            // 0x3aa0e8, 0x1adafb
//             int nMargin = this->GetMenuImageMargin();                                       // vtable +0x5c8
//             int xImages = rect.left + 1 + nMargin + CMFCToolBar::GetMenuImageSize().cx;    // 0x1adb33..0x1adb3a
//             CRect rectClient; ::GetClientRect(pBar->m_hWnd, &rectClient);                  // 0x1adb42
//             if (rectClient.right - x2 < 50) x2 = rectClient.right;                         // 0x1adb59/0x1adb5c
//             x1 = ((CMFCPopupMenuBar*)pBar)->m_bDisableSideBarInXPMode /*+0x1350*/ ? 0 : xImages;  // 0x1adb64/0x1adb6a
//             CWnd* pParent = CWnd::FromHandle(::GetParent(pBar->m_hWnd));                   // 0x1adb7a/0x1adb83
//             if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))                // 0x3aa448, 0x1adb9a
//                 && ((CMFCPopupMenu*)pParent)->m_QuickType /*+0x19b0*/ == QUICK_CUSTOMIZE_PANE /*0*/)  // 0x1adba3
//                 x1 = rect.left + 2 * GetMenuImageSize().cx + 2 + 3 * nMargin;              // 0x1adbb9..0x1adbd4
//         }
//     }
//     pDC->MoveTo(x1, y1); pDC->LineTo(x2, y2); pDC->SelectObject(pOld);        // 0x1adbe8 / 0x1adbf6
// Offsets not pinned by a shadow struct, derived from the retail CRuntimeClass
// object sizes plus the MSVC headers' member order: CMFCPopupMenuBar's own
// members start at +0x1350 (== sizeof CMFCToolBar) with m_bDisableSideBarInXPMode
// (afxpopupmenubar.h), and CMFCRibbonPanelMenuBar's at +0x1420 (== the
// CMFCPopupMenuBar object size in its descriptor) as m_bTracked, m_bSimpleMode,
// m_bIsMenuMode, m_bIsDefaultMenuLook (afxribbonpanelmenu.h) -> +0x142c.
// CMFCPopupMenu +0x19b0 is m_QuickType: ??0CMFCPopupMenu's Initialize (0xb5560)
// stores QUICK_CUSTOMIZE_NONE (2) there at 0xb5769, and OpenMFC's own
// CMFCPopupMenu::Initialize (phase4/src/featurepack/menu/CMFCPopupMenu.cpp)
// writes the same 2 at +0x19b0.  The pen at +0x1e0 is ::CreatePen(PS_SOLID, 1,
// m_clrSeparator) (0x1ac9b2), XP_Colors().c12c.
// Deviations: CWnd::FromHandle is replaced by the exported FromHandlePermanent
// -- a temporary CWnd wrapper would fail the IsKindOf(CMFCPopupMenu) anyway, so
// the outcome is the same; m_bDisableSideBarInXPMode (+0x1350) and
// m_bIsDefaultMenuLook (+0x142c) are read from the object at those offsets
// (both sit in OpenMFC padding that the ctors zero, i.e. the retail defaults
// FALSE unless an MSVC client's inline setter wrote them).
// Symbol: ?OnDrawSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz)
{
    if (pBar == nullptr) return;

    if (pBar->m_bIsDlgControl) {
        impl__OnDrawSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
            pThis, pDC, pBar, rect, bHorz);
        return;
    }

    int x1, y1, x2, y2;
    if (bHorz) {
        x1 = x2 = (rect.left + rect.right) / 2;
        y1 = rect.top;
        y2 = rect.bottom - 1;
    } else {
        y1 = y2 = (rect.top + rect.bottom) / 2;
        x1 = rect.left;
        x2 = rect.right;

        const CObject* pObj = static_cast<const CObject*>(pBar);
        const bool bRibbonMenu =
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObj, impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ()) != 0 &&
            XP_ReadInt(pBar, 0x142c) == 0;

        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObj, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()) != 0 &&
            !bRibbonMenu &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pObj, impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ()) == 0) {
            const int nMargin = XP_MenuImageMargin();
            const SIZE sz = XP_MenuImageSize();
            const int xImages = rect.left + 1 + nMargin + sz.cx;

            RECT rectClient = { 0, 0, 0, 0 };
            ::GetClientRect(pBar->m_hWnd, &rectClient);
            if (rectClient.right - x2 < 50) x2 = rectClient.right;

            x1 = (XP_ReadInt(pBar, 0x1350) != 0) ? 0 : xImages;

            CWnd* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pBar->m_hWnd));
            if (pParent != nullptr &&
                impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                    static_cast<const CObject*>(pParent),
                    impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ()) != 0 &&
                XP_ReadInt(pParent, 0x19b0) == 0) {
                x1 = rect.left + 2 * sz.cx + 2 + 3 * nMargin;
            }
        }
    }

    XP_DrawLine(pDC, x1, y1, x2, y2, XP_Colors().c12c);
}

// CMFCVisualManagerOfficeXP::OnDrawSpinButtons(CDC*, CRect rectSpin, int nState,
//         BOOL bOrientation, CMFCSpinButtonCtrl*) -- retail RVA 0x1b10a0 (mfc140.dll):
//     CRect rect[2] = { rectSpin, rectSpin };
//     if (!bOrientation) { rect[0].bottom = rectSpin.bottom - rectSpin.Height() / 2;
//                          rect[1].top = rect[0].bottom + 1; }                       // 0x1b1128..0x1b113e
//     else               { rect[1].right = rectSpin.right - rectSpin.Width() / 2;
//                          rect[0].left = rect[1].right; }                           // 0x1b1143..0x1b1151
//     static const int ids[4] = { IdArrowUp, IdArrowDown, IdArrowRight, IdArrowLeft };  // 0x180348dd0 = {7,0,1,9}
//     const int* pIds = bOrientation ? ids + 2 : ids;                                // 0x1b118f..0x1b119d
//     int nPressed     = (nState & 3) - 1;                                           // 0x1b1161..0x1b1168
//     int nHighlighted = (nState & 4) ? 0 : (nState & 8) ? 1 : -1;                   // 0x1b116b..0x1b1181
//     int nImageState  = (nState & 0x10) ? ImageGray : ImageBlack;                   // 0x1b1186..0x1b1189
//     for (int i = 0; i < 2; i++) {
//         if (i == nPressed || i == nHighlighted)
//             this->OnFillHighlightedArea(pDC, rect[i], i == nPressed ? &m_brHighlightDn : &m_brHighlight, NULL);  // 0x1b133a
//         else if (CMFCToolBarImages::m_bIsDrawOnGlass) { CDrawingManager dm(*pDC);
//             dm.DrawRect(rect[i], afxGlobalData.clrBarFace, afxGlobalData.clrBarHilite); }   // 0x1b123a
//         else { ::FillRect(pDC->m_hDC, rect[i], afxGlobalData.brBarFace);              // 0x1b128b
//                pDC->Draw3dRect(rect[i], afxGlobalData.clrBarHilite, afxGlobalData.clrBarHilite); }  // 0x1b12ec
//         CMenuImages::Draw(pDC, pIds[i], rect[i], nImageState, CSize(0,0));           // 0x1b1362
//     }
//     if (nHighlighted >= 0) {                                                       // 0x1b137d
//         if (m_bIsDrawOnGlass) dm.DrawRect(rect[nHighlighted], (COLORREF)-1, m_clrMenuItemBorder);  // 0x1b13bd
//         else pDC->Draw3dRect(rect[nHighlighted], m_clrMenuItemBorder, m_clrMenuItemBorder);        // 0x1b13cf
//     }
// The spin control pointer is never read.  Deviations: the two on-glass
// DrawRect arms are not taken (CDrawingManager exports are stubs here); the
// glyphs are painted by XP_DrawMenuImage; brushes/border from XP_Colors().
// Symbol: ?OnDrawSpinButtons@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rectSpin, int nState, int bOrientation,
    void* /*pSpinCtrl*/ /* CMFCSpinButtonCtrl* */)
{
    CRect rect[2] = { rectSpin, rectSpin };
    if (!bOrientation) {
        rect[0].bottom = rectSpin.bottom - (rectSpin.bottom - rectSpin.top) / 2;
        rect[1].top    = rect[0].bottom + 1;
    } else {
        rect[1].right = rectSpin.right - (rectSpin.right - rectSpin.left) / 2;
        rect[0].left  = rect[1].right;
    }

    static const int ids[4] = { 7 /*IdArrowUp*/, 0 /*IdArrowDown*/, 1 /*IdArrowRight*/, 9 /*IdArrowLeft*/ };
    const int* pIds = bOrientation ? ids + 2 : ids;

    const int nPressed     = (nState & 3) - 1;
    const int nHighlighted = (nState & 4) ? 0 : ((nState & 8) ? 1 : -1);
    const int nImageState  = (nState & 0x10) ? 1 /*ImageGray*/ : 0 /*ImageBlack*/;

    const XPColors x = XP_Colors();
    for (int i = 0; i < 2; i++) {
        if (i == nPressed || i == nHighlighted) {
            XP_FillHighlightedSolid(pDC, rect[i], (i == nPressed) ? x.c11c : x.c118);
        } else {
            XP_FillSolid(pDC, rect[i], GD_clrBtnFace());
            DC_Draw3dRect(pDC, rect[i], GD_clrBarHilite(), GD_clrBarHilite());
        }
        XP_DrawMenuImage(pDC, pIds[i], rect[i], nImageState);
    }

    if (nHighlighted >= 0) {
        DC_Draw3dRect(pDC, rect[nHighlighted], x.c138, x.c138);
    }
}

// CMFCVisualManagerOfficeXP::OnDrawSplitterBorder(CDC*, CSplitterWndEx*, CRect rect)
// -- retail RVA 0x1b14a0:
//     afxGlobalData.Initialize();
//     pDC->Draw3dRect(rect, afxGlobalData.clrBarShadow (+0x64),
//                           afxGlobalData.clrBarShadow);   // call at 0x1b150a
//     ::InflateRect(rect, -1, -1);                         // call at 0x1b1518
//     afxGlobalData.Initialize();
//     pDC->Draw3dRect(rect, afxGlobalData.clrBarFace (+0x60),
//                           afxGlobalData.clrBarFace);     // call at 0x1b1573
// Neither `this` nor the splitter pointer is read, and no OfficeXP member is
// touched, so this is reproduced exactly: AFX_GLOBAL_DATA::UpdateSysColors
// (0x6afd0) fills clrBarShadow from ::GetSysColor(COLOR_BTNSHADOW) (store at
// 0x6b07a) and clrBarFace from ::GetSysColor(COLOR_BTNFACE) (store at 0x6b069).
// Symbol: ?OnDrawSplitterBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCSplitterWndEx@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitterBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCSplitterWndEx__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC,
    void* /*pSplitter*/ /* CSplitterWndEx* */, CRect rect)
{
    DC_Draw3dRect(pDC, rect, GD_clrBtnShadow(), GD_clrBtnShadow());
    rect.InflateRect(-1, -1);
    DC_Draw3dRect(pDC, rect, GD_clrBtnFace(), GD_clrBtnFace());
}

// CMFCVisualManagerOfficeXP::OnDrawSplitterBox(CDC*, CSplitterWndEx*, CRect& rect)
// -- retail RVA 0x1b1590:
//     afxGlobalData.Initialize();
//     pDC->Draw3dRect(rect, afxGlobalData.clrBarFace (+0x60),
//                           afxGlobalData.clrBarFace);
// That Draw3dRect is the whole body -- 0x1b1609 tail-jumps into CDC::Draw3dRect
// (0x2a3b00).  `this` and the splitter pointer are not read and the rect is not
// modified.  clrBarFace is ::GetSysColor(COLOR_BTNFACE) (UpdateSysColors store
// at 0x6b069), so this is exact.
// Symbol: ?OnDrawSplitterBox@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCSplitterWndEx@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitterBox_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCSplitterWndEx__AEAVCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC,
    void* /*pSplitter*/ /* CSplitterWndEx* */, CRect* pRect)
{
    if (pRect == nullptr) return;
    DC_Draw3dRect(pDC, *pRect, GD_clrBtnFace(), GD_clrBtnFace());
}

// CMFCVisualManagerOfficeXP::OnDrawStatusBarPaneBorder(CDC*, CMFCStatusBar*,
//         CRect rectPane, UINT uiID, UINT nStyle) -- retail RVA 0x1af990:
//     if (nStyle & SBPS_NOBORDERS /*0x0100*/) return;         // test at 0x1af9a9
//     if (nStyle & SBPS_POPOUT /*0x0200*/) {                  // test at 0x1af9b6
//         CDrawingManager dm(*pDC);                           // local, vtable 0x1802e50d8
//         dm.HighlightRect(rectPane, -1, (COLORREF)-1, 0, (COLORREF)-1);  // 0x56750
//     }
//     pDC->Draw3dRect(rectPane, this->[+0x130], this->[+0x130]);  // call at 0x1afa0e
// The status-bar pointer and uiID are never read.  Member +0x130 is the one
// OfficeXP colour that CMFCVisualManagerOfficeXP::OnUpdateSystemColors
// (0x1ac0e0) gives the SAME value on both of its paths: the gradient path
// stores afxGlobalData.clrBarShadow (+0x64) at 0x1ac60f and the low-colour /
// high-contrast path stores clrBtnShadow (+0x2c) at 0x1ac844, and
// AFX_GLOBAL_DATA::UpdateSysColors (0x6afd0) fills both of those fields from
// ::GetSysColor(COLOR_BTNSHADOW) (the two stores at 0x6b077 and 0x6b07a).  So
// GD_clrBtnShadow() reproduces +0x130 exactly and the fact that OpenMFC has no
// storage for the member does not matter here.
// Deviation: the SBPS_POPOUT highlight is not drawn.  OpenMFC's exported
// CDrawingManager::HighlightRect (phase4/src/core/gdi/CDrawingManager.cpp:156)
// is an empty stub, and its generated signature also omits the implicit `this`,
// so calling it would paint nothing and pass arguments in the wrong registers.
// Symbol: ?OnDrawStatusBarPaneBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCStatusBar@@VCRect@@II@Z
extern "C" void MS_ABI impl__OnDrawStatusBarPaneBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCStatusBar__VCRect__II_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC,
    void* /*pBar*/ /* CMFCStatusBar* */, CRect rectPane,
    unsigned int /*uiID*/, unsigned int nStyle)
{
    if ((nStyle & kXPStatusNoBorders) != 0) return;
    DC_Draw3dRect(pDC, rectPane, GD_clrBtnShadow(), GD_clrBtnShadow());
}

// Retail (RVA 0x1ae260, mfc140.dll) first calls five CMFCBaseTabCtrl virtuals
// in a row -- vtable +0x510 IsFlatTab, +0x520 IsOneNoteStyle, +0x560 IsColored,
// +0x528 IsVS2005Style, +0x530 IsLeftRightRounded -- and if ANY of them returns
// non-zero (the five tests at 0x1ae29e..0x1ae30c all jump to 0x1ae6ad) it
// delegates to CMFCVisualManager::OnDrawTab (0x183ce0, the call at 0x1ae6d0)
// and returns.  Only when all five are zero does it paint the OfficeXP tab
// itself: three local CPens, this->GetTabFrameColors (vtable +0x228, 0x1ae368),
// m_bDrawLastTabLine (+0x208) and the tab control's layout fields.  The slot
// names were read off the retail CMFCBaseTabCtrl vtable at 0x1802dbd88
// (installed by ??0CMFCBaseTabCtrl@@QEAA@XZ at 0x156dd): +0x508 and +0x558/
// +0x560 resolve to the exported IsTabVisible / HideSingleTab / IsColored,
// which pins the run of inline `return FALSE` virtuals between them to
// afxbasetabctrl.h's declaration order.  OpenMFC's CMFCBaseTabCtrl (a CWnd
// plus 128 bytes of padding) has none of those virtuals, so the guard cannot
// be evaluated; left a stub.  (An earlier comment here said the non-zero case
// "returns without painting anything"; it delegates to the base class.)
// Symbol: ?OnDrawTab@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CRect /*rectTab*/, int /*iTab*/, int /*bIsActive*/,
    const CMFCBaseTabCtrl* /*pTabWnd*/) {}


// CMFCVisualManagerOfficeXP::OnDrawTabCloseButton(CDC*, CRect rect,
//         const CMFCBaseTabCtrl* pTabWnd, BOOL bIsHighlighted, BOOL bIsPressed,
//         BOOL bIsDisabled) -- retail RVA 0x1afce0:
//     if (bIsHighlighted)
//         this->OnFillHighlightedArea(pDC, rect,                 // vtable +0x638
//             bIsPressed ? &this->[+0x1b0] : &this->[+0x1a0], NULL);
//     CMenuImages::Draw(pDC, (IMAGES_IDS)5, rect, (IMAGE_STATE)0, CSize(0, 0)); // 0x8fd40
//     if (bIsHighlighted)
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);
// Both `if`s test the same argument (the two `cmpl $0x0,0x80(%rsp)` at 0x1afce8
// and 0x1afd65); the CMenuImages::Draw call in between is unconditional.  The
// brush choice is the `neg %ecx; sbb %r9,%r9; and $0x10,%r9d` at 0x1afd0e.
// pTabWnd and bIsDisabled are not read.
// The +0x1a0 / +0x1b0 brushes are ::CreateSolidBrush(this->[+0x118]) and
// (this->[+0x11c]); this build makes an equivalent solid brush from XP_Colors().
// Symbol: ?OnDrawTabCloseButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTabCloseButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    const void* /*pTabWnd*/ /* const CMFCBaseTabCtrl* */,
    int bIsHighlighted, int bIsPressed, int /*bIsDisabled*/)
{
    const XPColors x = XP_Colors();

    if (bIsHighlighted) {
        XP_FillHighlightedSolid(pDC, rect, bIsPressed ? x.c11c : x.c118);
    }

    const CSize sizeImage(0, 0);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
        pDC, 5, rect, 0, sizeImage);

    if (bIsHighlighted) {
        DC_Draw3dRect(pDC, rect, x.c138, x.c138);
    }
}

// Retail (RVA 0x1afed0, mfc140.dll) branches on pWndTab->IsFlatTab()
// (CMFCBaseTabCtrl vtable +0x510, call at 0x1afef4; slot identified as in
// OnDrawTab above) and on CMFCButton::m_bHover (+0x114) / m_bClickStarted
// (+0x11c), then draws one Draw3dRect whose two colours are
// afxGlobalData.clrBarDkShadow (+0x6c) and, on two of the three painting paths,
// m_clrGripper (+0x128, XP_Colors().c128).  IsFlatTab is not dispatchable on
// OpenMFC's CMFCBaseTabCtrl (see OnDrawTab), so the branch cannot be chosen;
// left a stub.
// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CRect* /*pRect*/, CMFCButton* /*pButton*/,
    unsigned int /*uiState*/, CMFCBaseTabCtrl* /*pWndTab*/) {}

// CMFCVisualManagerOfficeXP::OnDrawTask(CDC*, CMFCTasksPaneTask* pTask, CImageList* pIcons,
//         BOOL bIsHighlighted, BOOL bIsSelected) -- retail RVA 0x1b0b80 (mfc140.dll).
// Retail throws (0x225b80) when pTask or pIcons is NULL, then:
//     CRect rectText = pTask->m_rect;                                    // +0x18
//     if (pTask->m_bIsSeparator /*+0x54*/) { select afxGlobalData.penBarShadow (+0x148);
//         MoveTo(left, ymid); LineTo(right, ymid); restore; return; }    // 0x1b0c01..0x1b0c46
//     ImageList_GetIconSize(pIcons->m_hImageList, &cx, &cy);            // 0x6f1e0
//     if (pTask->m_nIcon /*+0x28*/ >= 0 && cx > 0)
//         ImageList_Draw(pIcons->m_hImageList, m_nIcon, pDC->m_hDC, left, top, ILD_TRANSPARENT);  // 0x60b94
//     CMFCTasksPane* pPane = pTask->m_pGroup->m_pPage->m_pTaskPane;      // +0x8 -> +0x8 -> +0x10, 0x1b0cb8..0x1b0cc0
//     int nOffset = pPane->m_nTasksIconHorzOffset;                       // +0x540, 0x1b0cc4
//     if (nOffset == -1) nOffset = m_nTasksIconHorzOffset;               // CMFCVisualManager +0xec, 0x1b0ccf
//     rectText.left += cx + nOffset;
//     <select afxGlobalData.fontBold/fontRegular/fontUnderline (+0x1c8/+0x1a8/+0x1e8) and a
//      text colour from m_clrText/m_clrTextHot (+0x5c/+0x60) or afxGlobalData, on m_uiCommandID
//      (+0x30), m_bIsBold (+0x58), m_bEnabled (+0x50) and bIsSelected; DrawText the task name
//      (+0x10) with DT_WORDBREAK when (m_uiCommandID ? pPane->m_bWrapTasks /*+0x4fc*/
//      : pPane->m_bWrapLabels /*+0x500*/) is set (the `neg/sbb/and $-4` at 0x1b0eb3..0x1b0ec9
//      selecting the offset), else with CR/LF stripped and DT_SINGLELINE|DT_VCENTER|
//      DT_END_ELLIPSIS (0x8024, 0x1b0f23); restore>
// Every object offset above IS pinned by a shadow struct in this repo:
// S_Cmfctaskspanetask (phase4/src/detail/CMFCTasksPaneTaskSupport.h: m_pGroup
// 8, m_strName 16, m_rect 24, m_nIcon 40, m_uiCommandID 48, m_bEnabled 80,
// m_bIsSeparator 84, m_bIsBold 88, m_clrText 92, m_clrTextHot 96),
// S_Cmfctaskspanetaskgroup (m_pPage 8), S_Cmfctaskspanepropertypage
// (m_pTaskPane 16) and S_MfcFeature20Impl in CMFCTasksPaneSupport.h
// (m_bWrapTasks 0x4fc, m_bWrapLabels 0x500, m_nTasksIconHorzOffset 0x540).
// Left a stub for one reason only: the three fonts the text is drawn with
// live in afxGlobalData (+0x1a8 / +0x1c8 / +0x1e8), which OpenMFC exports as a
// zero-filled blob (phase4/src/featurepack/CMFC_misc_stubs.cpp), so the
// bold / underline / regular distinction that is the visible point of this
// body cannot be reproduced; a transcription would draw every task in
// whatever font the caller left selected.  (An earlier comment here claimed
// the CMFCTasksPane members were unmodelled and cited +0x504; both were wrong.)
// Symbol: ?OnDrawTask@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CMFCTasksPaneTask* /*pTask*/, CImageList* /*pIcons*/,
    int /*bIsHighlighted*/, int /*bIsSelected*/) {}

// CMFCVisualManagerOfficeXP::OnDrawTasksGroupAreaBorder(CDC*, CRect, BOOL, BOOL)
// -- the retail body is EMPTY: the OfficeXP look draws no border around the
// tasks-pane group area.
//
// This one export has no entry of its own in mfc140_rva_symbols.json, so it was
// resolved through the vtables instead.  CMFCVisualManager's version is at
// 0x187c00 and sits in vtable slot 95 (byte offset 0x2f8) of the base vtable at
// image address 0x180319f78 (installed by ??0CMFCVisualManager@@QEAA@H@Z at
// 0x182640, store at 0x182666).  Slot 95 of CMFCVisualManagerOffice2003's
// vtable (0x18031a5c0) holds 0x1944a0 and slot 95 of CMFCVisualManagerWindows'
// (0x18031c758) holds 0x1b9780 -- i.e. exactly the two derived overrides the
// symbol map does resolve -- which pins the slot.  Slot 95 of
// CMFCVisualManagerOfficeXP's vtable (0x18031b3d8, installed by
// ??0CMFCVisualManagerOfficeXP@@IEAA@H@Z at 0x1abe10, store at 0x1abe2f) holds
// 0x180002820, which disassembles to a single `ret` (it is the ICF-folded empty
// body the linker also shares with ?UpdateModifiedFlag@CRichEditDoc@@UEAAXXZ).
// The address differs from the base's, so the class does override the virtual
// -- with a do-nothing body.  The empty body below is therefore exact, not a
// stub.
// Symbol: ?OnDrawTasksGroupAreaBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, int /*bSpecial*/, int /*bNoTitle*/) {}

// Retail (RVA 0x1b0730, mfc140.dll) throws when pGroup or pGroup->m_pPage (+0x8)
// is NULL, reads the group's m_rect (+0x5c..) / m_sizeIcon (+0x7c) / m_hIcon
// (+0x88) (loads at 0x1b0771..0x1b07ab; all pinned by S_Cmfctaskspanetaskgroup
// in phase4/src/detail/CMFCTasksPaneTaskGroupSupport.h), and when an icon fits
// hands it to this->OnDrawTasksGroupIcon (vtable +0x2e8, 0x1b07dc).  It then
// selects afxGlobalData.fontBold (+0x1c8, 0x1b0811), sets colours from
// m_clrText / m_clrTextHot (+0x90/+0x94) or afxGlobalData, draws the caption
// text and a ::Rectangle frame (CDC::SelectObject 0x2a0670 and the GDI imports
// GetBkColor 0x2c4178 / Rectangle 0x2c4218) and, for a collapsible group, a
// CMenuImages::Draw glyph (0x8fc50,
// 0x1b0ab8) sized by CMenuImages::Size (0x8fbb0).  Left a stub: the bold font
// lives in afxGlobalData, which OpenMFC exports as a zero-filled blob, and the
// icon / glyph calls go to virtuals and exports that are empty stubs here.
// (An earlier comment here said the group was not laid out; it is.)
// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CMFCTasksPaneTaskGroup* /*pGroup*/,
    int /*bIsHighlighted*/, int /*bIsSelected*/, int /*bCanCollapse*/) {}

// CMFCVisualManagerOfficeXP::OnDrawTearOffCaption(CDC*, CRect rect, BOOL bIsActive)
// -- retail RVA 0x1aefd0 (mfc140.dll):
//     ::FillRect(pDC->m_hDC, rect, m_brMenuLight.m_hObject);                    // +0x180, 0x1af015
//     ::InflateRect(&rect, -1, -1);                                              // 0x1af025
//     this->OnFillHighlightedArea(pDC, rect,                                    // vtable +0x638, 0x1af069
//         bIsActive ? &m_brHighlight /*+0x1a0*/ : &m_brBarBkgnd /*+0x160*/, NULL);
//     int cxGripper = 20;                                                       // 0x1af079
//     if (2 * CMFCToolBar::GetMenuImageSize().cx >= 20) cxGripper = 2 * GetMenuImageSize().cx;  // 0x1af074..0x1af092
//     CRect rectGripper = rect;
//     ::InflateRect(&rectGripper, -((rect.Width() - cxGripper) / 2), -1);       // 0x1af0a9..0x1af0bb
//     if (m_brGripperHorz.m_hObject == NULL) this->CreateGripperBrush();        // vtable +0x620, 0x1af0e1
//     COLORREF clrOldText = pDC->SetTextColor(bIsActive ? afxGlobalData.clrBarDkShadow /*+0x6c*/
//                                                       : afxGlobalData.clrBarShadow /*+0x64*/);  // 0x1af145
//     COLORREF clrOldBk   = pDC->SetBkColor(bIsActive ? m_clrHighlight /*+0x118*/ : m_clrBarBkgnd /*+0x108*/);  // 0x1af16a
//     if (bIsActive) ::InflateRect(&rectGripper, 0, -1);                        // 0x1af182
//     ::FillRect(pDC->m_hDC, rectGripper, m_brGripperHorz.m_hObject);           // +0x140, 0x1af19f
//     pDC->SetTextColor(clrOldText); pDC->SetBkColor(clrOldBk);                 // 0x1af1b2 / 0x1af1c4
//     if (bIsActive) pDC->Draw3dRect(rect, afxGlobalData.clrBarDkShadow, afxGlobalData.clrBarDkShadow);  // 0x1af224
// The SetBkColor colour is the `and $0x10,%edx; mov 0x108(%rdx,%rsi,1)` at
// 0x1af15c..0x1af163.  Deviations, all forced by the missing member storage:
// the brushes are XP_Colors() solids and XP_CreateGripperBrush, the CDC colour
// setters go to GDI, and GetMenuImageSize is the local helper.
// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect, int bIsActive)
{
    const XPColors x = XP_Colors();

    XP_FillSolid(pDC, rect, x.c110);
    rect.InflateRect(-1, -1);
    XP_FillHighlightedSolid(pDC, rect, bIsActive ? x.c118 : x.c108);

    int cxGripper = 20;
    const SIZE sz = XP_MenuImageSize();
    if (2 * sz.cx >= 20) cxGripper = 2 * static_cast<int>(sz.cx);

    CRect rectGripper(rect);
    rectGripper.InflateRect(-(((rect.right - rect.left) - cxGripper) / 2), -1);

    HDC hdc = XP_Hdc(pDC);
    if (hdc == nullptr) return;

    HBRUSH hbrGripper = XP_CreateGripperBrush();
    if (hbrGripper == nullptr) return;

    const COLORREF clrOldText = ::SetTextColor(hdc, bIsActive ? GD_clrBtnDkShadow() : GD_clrBtnShadow());
    const COLORREF clrOldBk   = ::SetBkColor(hdc, bIsActive ? x.c118 : x.c108);

    if (bIsActive) rectGripper.InflateRect(0, -1);
    RECT r = XP_ToRECT(rectGripper);
    ::FillRect(hdc, &r, hbrGripper);

    ::SetTextColor(hdc, clrOldText);
    ::SetBkColor(hdc, clrOldBk);
    ::DeleteObject(hbrGripper);

    if (bIsActive) {
        DC_Draw3dRect(pDC, rect, GD_clrBtnDkShadow(), GD_clrBtnDkShadow());
    }
}


// CMFCVisualManagerOfficeXP::OnErasePopupWindowButton(CDC*, CRect rectClient,
//         CMFCDesktopAlertWndButton* pButton) -- retail RVA 0x1b1990:
//     BOOL bHover        = pButton->[+0x114];
//     BOOL bClickStarted = pButton->[+0x11c];
//     if (!bHover && !bClickStarted) {
//         CRect rc(0, 0, 0, 0);
//         HWND hWndParent = ::GetParent(pButton->m_hWnd);
//         ::GetClientRect(CWnd::FromHandle(hWndParent)->m_hWnd, &rc);
//         ::MapWindowPoints(CWnd::FromHandle(hWndParent)->m_hWnd,
//                           pButton->m_hWnd, (LPPOINT)&rc, 2);
//         this->OnFillPopupWindowBackground(pDC, rc);            // vtable +0x3c8
//         return;
//     }
//     CBrush br((bHover && bClickStarted) ? this->[+0x11c] : this->[+0x118]);
//     ::FillRect(pDC->m_hDC, rectClient, br.m_hObject);
// The four-way test is the chain at 0x1b19c6..0x1b19e1: only the
// !bHover && !bClickStarted corner reaches the parent-rect path at 0x1b19e3;
// the pressed colour +0x11c is used only when BOTH flags are set (the store at
// 0x1b19cf), every other combination taking +0x118 at 0x1b1a5a.  Note that on
// the fill path the rect painted is the argument, not the mapped parent rect.
// The two CWnd::FromHandle(hWndParent)->m_hWnd round trips (0x1b19f8 and
// 0x1b1a18) just recover hWndParent, so they are folded away here.
// +0x114 is CMFCButton::m_bHover and +0x11c CMFCButton::m_bClickStarted
// (phase4/src/detail/CMFCButtonSupport.h); CWnd::m_hWnd is at +0x40.
// Retail dereferences pButton unconditionally; the null guards here are
// OpenMFC's, and so is the ::GetParent == NULL guard (retail would fault in
// CWnd::FromHandle's caller).
// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect,
    void* pButton /* CMFCDesktopAlertWndButton* */)
{
    if (pButton == nullptr) return;

    const char* const p = static_cast<const char*>(pButton);
    const int bHover        = *reinterpret_cast<const int*>(p + kXPBtnHover);
    const int bClickStarted = *reinterpret_cast<const int*>(p + kXPBtnClickStarted);

    if (bHover == 0 && bClickStarted == 0) {
        HWND hWndButton = *reinterpret_cast<HWND const*>(p + kXPWndHwnd);
        HWND hWndParent = ::GetParent(hWndButton);
        if (hWndParent == nullptr) return;

        RECT rc = { 0, 0, 0, 0 };
        ::GetClientRect(hWndParent, &rc);
        ::MapWindowPoints(hWndParent, hWndButton, reinterpret_cast<LPPOINT>(&rc), 2);

        XP_FillPopupWindowBackground(pDC, CRect(rc.left, rc.top, rc.right, rc.bottom));
        return;
    }

    const XPColors x = XP_Colors();
    XP_FillSolid(pDC, rect, (bHover != 0 && bClickStarted != 0) ? x.c11c : x.c118);
}

// Retail (RVA 0x1ae830, mfc140.dll) is
//     if (pTabWnd->IsFlatTab())                          // vtable +0x510, call at 0x1ae852
//         { CMFCVisualManager::OnFillPopupWindowBackground(pDC, rect); return; }  // 0x183c90 (ICF-shared
//                                                        //   with the base OnEraseTabsArea body)
//     ::FillRect(pDC->m_hDC, rect,
//                pTabWnd->[+0x1fc] ? afxGlobalData.brBtnFace (+0xa8).m_hObject
//                                  : m_brTabBack (+0x190).m_hObject);
// Left a stub: IsFlatTab is not dispatchable on OpenMFC's CMFCBaseTabCtrl (see
// OnDrawTab), +0x1fc is in that class's padding, and m_brTabBack is
// ::CreateSolidBrush of a value OnUpdateSystemColors computes differently on
// each of its two paths (clrBtnFace at 0x1ac7bc on the low-colour path, an
// HLS-converted blend through HLStoRGB_ONE at 0x1ac32b on the gradient path)
// that this file does not model (see the file header).
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, const CMFCBaseTabCtrl* /*pTabWnd*/) {}

// Retail (RVA 0x1afd90, mfc140.dll) branches on pWndTab->IsFlatTab() (vtable
// +0x510, call at 0x1afdbc) exactly as OnEraseTabsArea; on the flat side it
// fills with m_brHighlightDn (+0x1b0) when m_bHover && m_bClickStarted, with
// m_brHighlight (+0x1a0) when only one of them is set, else with
// afxGlobalData.brBarFace, and then calls the +0x638 virtual with the same
// brush; on the other side it fills with afxGlobalData.brBtnFace or
// m_brTabBack (+0x190) on pWndTab->[+0x1fc].  Left a stub for the reasons given
// at OnEraseTabsArea (IsFlatTab, +0x1fc and m_brTabBack).
// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CRect /*rect*/, CMFCButton* /*pButton*/,
    CMFCBaseTabCtrl* /*pWndTab*/) {}

// CMFCVisualManagerOfficeXP::OnFillBarBackground(CDC*, CBasePane* pBar, CRect rectClient,
//         CRect rectClip, BOOL bNCArea) -- retail RVA 0x1ad730 (mfc140.dll):
//     if ((pBar && pBar->IsKindOf(RUNTIME_CLASS(CReBar)))                                  // 0x3210a0, 0x1ad76f
//         || (CWnd::FromHandle(::GetParent(pBar->m_hWnd)) &&
//             CWnd::FromHandle(::GetParent(pBar->m_hWnd))->IsKindOf(RUNTIME_CLASS(CReBar)))) {  // 0x1ad77c..0x1ad799
//         this->FillReBarPane(pDC, pBar, rectClient); return; }                             // vtable +0x48, 0x1ad7c0
//     if (::IsRectEmpty(&rectClip)) rectClip = rectClient;                                  // 0x1ad7ce
//     CRuntimeClass* pRTC = pBar->GetRuntimeClass();                                       // vtable slot 0, 0x1ad7ea
//     if (pRTC == NULL || pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCMenuBar))) goto BASE;       // 0x3aa268
//     if (pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCOutlookBarPane))                            // 0x3aa358
//         && pBar->IsKindOf(RUNTIME_CLASS(CMFCOutlookBarPane)) && pBar->[+0x1368]) goto BASE;
//     if (pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCColorBar))) {                              // 0x3aa0e8
//         if (pBar->m_bIsDlgControl) goto BASE;
//         ::FillRect(pDC->m_hDC, rectClip, ((CMFCColorBar*)pBar)->m_bIsTearOff /*+0x1448*/
//                                              ? m_brBarBkgnd /*+0x160*/ : m_brMenuLight /*+0x180*/);  // 0x1ad8b0
//         return; }
//     if (pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCPopupMenuBar))) {                          // 0x3aa478
//         ::FillRect(pDC->m_hDC, rectClip, m_brMenuLight);                                 // 0x1ad8ee
//         if (((CMFCPopupMenuBar*)pBar)->m_bDisableSideBarInXPMode /*+0x1350*/) return;   // 0x1ad90b
//         CRect rectImages = rectClient;
//         rectImages.right = rectClient.left + ((CMFCPopupMenuBar*)pBar)->GetGutterWidth();  // 0xc0250, 0x1ad922
//         ::InflateRect(&rectImages, 0, -1);                                               // 0x1ad937
//         ::FillRect(pDC->m_hDC, rectImages, m_brBarBkgnd); return; }
//     if (pRTC->IsDerivedFrom(RUNTIME_CLASS(CMFCToolBar))) {                               // 0x3aa5f8
//         if (pBar->m_bIsDlgControl) goto BASE;
//         ::FillRect(pDC->m_hDC, rectClip, m_brBarBkgnd); return; }
//     if (pRTC->IsDerivedFrom(RUNTIME_CLASS(CAutoHideDockSite))) {                         // 0x2d9df8
//         ::FillRect(pDC->m_hDC, rectClip, m_brTabBack /*+0x190*/); return; }
//   BASE:
//     CMFCVisualManager::OnFillBarBackground(pDC, pBar, rectClient, rectClip, FALSE);      // 0x182bd0
// bNCArea is never read (the base call passes a literal 0 at 0x1ad9c3).  The
// descriptors read back as CReBar / CMFCMenuBar / CMFCOutlookBarPane /
// CMFCColorBar / CMFCPopupMenuBar / CMFCToolBar / CAutoHideDockSite.  The
// pRTC->IsDerivedFrom tests are expressed as IsKindOf on the object, which is
// the same predicate.  CMFCColorBar +0x1448 is m_bIsTearOff (afxcolorbar.h:
// the eleventh member after the 0x1420-byte CMFCPopupMenuBar base) and
// CMFCPopupMenuBar +0x1350 is m_bDisableSideBarInXPMode -- both read from the
// object as in OnDrawSeparator above.
// DEVIATIONS: (1) the CMFCOutlookBarPane arm tests +0x1368, which OpenMFC's
// CMFCOutlookBarPane does not lay out; it is taken as 0, i.e. that arm falls
// through to the CMFCColorBar/.../CMFCToolBar chain as retail does when the flag
// is clear.  (2) the CAutoHideDockSite arm needs m_brTabBack, whose colour
// this file does not model (see the file header: an HLS-adjusted clrBarFace on
// the gradient path); it is delegated to the base implementation instead of
// guessing a colour.  (3) FillReBarPane (CMFCBaseVisualManager, vtable +0x48)
// has no dispatchable slot here; its arm delegates to the base body as well.
// (4) CWnd::FromHandle is replaced by the exported FromHandlePermanent -- a
// temporary wrapper would fail the IsKindOf(CReBar) anyway.
// Retail dereferences pBar->m_hWnd even for a NULL pBar; the guard is OpenMFC's.
// Symbol: ?OnFillBarBackground@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, int /*bNCArea*/)
{
    if (pBar == nullptr) return;
    const CObject* pObj = static_cast<const CObject*>(pBar);

    const bool bIsReBar =
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ()) != 0;
    bool bParentIsReBar = false;
    if (!bIsReBar) {
        CWnd* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pBar->m_hWnd));
        bParentIsReBar =
            pParent != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent), impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ()) != 0;
    }
    if (bIsReBar || bParentIsReBar) {
        // FillReBarPane arm -- see DEVIATION (3).
        impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
            pThis, pDC, pBar, rectClient, rectClip, FALSE);
        return;
    }

    if (rectClip.right <= rectClip.left || rectClip.bottom <= rectClip.top) {
        rectClip = rectClient;
    }

    const XPColors x = XP_Colors();
    const bool bIsMenuBar =
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ()) != 0;

    if (!bIsMenuBar) {
        // CMFCOutlookBarPane arm -- see DEVIATION (1): +0x1368 taken as 0, falls through.

        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ()) != 0) {
            if (!pBar->m_bIsDlgControl) {
                XP_FillSolid(pDC, rectClip, XP_ReadInt(pBar, 0x1448) != 0 ? x.c108 : x.c110);
                return;
            }
        } else if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()) != 0) {
            XP_FillSolid(pDC, rectClip, x.c110);
            if (XP_ReadInt(pBar, 0x1350) != 0) return;
            CRect rectImages(rectClient);
            rectImages.right = rectClient.left +
                impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(static_cast<CMFCPopupMenuBar*>(pBar));
            rectImages.InflateRect(0, -1);
            XP_FillSolid(pDC, rectImages, x.c108);
            return;
        } else if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ()) != 0) {
            if (!pBar->m_bIsDlgControl) {
                XP_FillSolid(pDC, rectClip, x.c108);
                return;
            }
        } else if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, impl__GetThisClass_CAutoHideDockSite__SAPEAUCRuntimeClass__XZ()) != 0) {
            // m_brTabBack arm -- see DEVIATION (2): delegated to the base body below.
        }
    }

    impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
        pThis, pDC, pBar, rectClient, rectClip, FALSE);
}

// CMFCVisualManagerOfficeXP::OnFillButtonInterior(CDC*, CMFCToolBarButton*, CRect rect,
//         AFX_BUTTON_STATE state) -- retail RVA 0x1aded0 (mfc140.dll):
//     if (state != ButtonsIsPressed && state != ButtonsIsHighlighted) return;   // 0x1adefc..0x1adf02
//     if (CMFCToolBar::m_bCustomizeMode && !CMFCToolBar::m_bAltCustomizeMode      // 0x3b70bc / 0x3b70cc
//         && !pButton->m_bLocked /*+0x50*/) return;                              // 0x1adf08..0x1adf1f
//     CMFCToolBarMenuButton* pMenuButton = NULL; BOOL bIsPopupMenuItem = FALSE;
//     if (pButton && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {   // 0x3aa748, 0x1adf34
//         pMenuButton = pButton;
//         CWnd* pParent = pButton->m_pWndParent;                                  // +0x80
//         bIsPopupMenuItem = pParent && pParent->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar)); // 0x3aa478
//     }
//     if (!bIsPopupMenuItem && !m_bEnableToolbarButtonFill /*+0xb0*/) return;    // 0x1adf65..0x1adf6f
//     CBrush* pBrush = ((pButton->m_nStyle & TBBS_PRESSED) && bIsPopupMenuItem)   // 0x1adf75..0x1adf8d
//                          ? &m_brHighlightDn /*+0x1b0*/ : &m_brHighlight /*+0x1a0*/;
//     if (pMenuButton && !bIsPopupMenuItem && pMenuButton->IsDroppedDown()) {     // vtable +0xf0, 0x1adfab
//         this->ExtendMenuButton(pMenuButton, rect);                              // vtable +0x628, 0x1adfc8
//         pBrush = &m_brBarBkgnd;                                                 // +0x160
//     }
//     if (pButton->m_nStyle & TBBS_CHECKED)                                       // 0x1adfd5
//         pBrush = (state == ButtonsIsHighlighted) ? &m_brHighlightDn : &m_brHighlightChecked; // +0x1c0, cmove at 0x1adff1
//     if (pMenuButton && (pButton->m_nStyle & TBBS_DISABLED)) pBrush = &m_brMenuLight;  // +0x180, 0x1ae003
//     if (!(pButton->m_nStyle & TBBS_CHECKED)) ::InflateRect(&rect, -1, -1);       // 0x1ae021
//     this->OnFillHighlightedArea(pDC, rect, pBrush, pButton);                    // vtable +0x638, 0x1ae051
// TBBS_CHECKED / TBBS_PRESSED / TBBS_DISABLED are MAKELONG(0, TBSTATE_*) = 0x10000
// / 0x20000 / 0x40000 (afxext.h).  Slot +0xf0 of the retail CMFCToolBarMenuButton
// vtable (0x180316338) is the body at 0x2a580, `return this->[0xc0] != NULL`,
// i.e. m_pPopupMenu.
// DEVIATION: the IsDroppedDown() arm is not evaluated.  CMFCToolBarMenuButton::
// m_pPopupMenu (+0xc0) lies inside OpenMFC's anonymous padding and nothing in
// this tree ever sets it (phase4/src/featurepack/toolbar/CMFCToolBar.cpp leaves
// DropDownMenu / GetDroppedDownMenu stubbed for the same reason), so the
// dropped-down branch -- ExtendMenuButton plus the m_brBarBkgnd fill -- is
// treated as not taken.  A menu-bar button whose menu is open therefore keeps
// the highlight fill here.  m_bEnableToolbarButtonFill is read through
// kXPVmEnableToolbarButtonFill (see its comment).  Retail dereferences pButton
// unconditionally (it only null-tests it before the IsKindOf); the guard is
// OpenMFC's.
// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect, int state)
{
    if (state != kXPStatePressed && state != kXPStateHighlighted) return;
    if (pButton == nullptr || pThis == nullptr) return;

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 &&
        impl__m_bAltCustomizeMode_CMFCToolBar__1HA == 0 &&
        pButton->m_bLocked == 0) {
        return;
    }

    bool bIsMenuButton = false;
    bool bIsPopupMenuItem = false;
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pButton),
            impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ()) != 0) {
        bIsMenuButton = true;
        CWnd* pParent = pButton->m_pWndParent;
        bIsPopupMenuItem =
            pParent != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent),
                impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()) != 0;
    }

    if (!bIsPopupMenuItem && XP_ReadInt(pThis, kXPVmEnableToolbarButtonFill) == 0) return;

    const XPColors x = XP_Colors();
    const unsigned int nStyle = pButton->m_nStyle;
    COLORREF clrFill = ((nStyle & 0x00020000u) != 0 && bIsPopupMenuItem) ? x.c11c : x.c118;

    // pMenuButton->IsDroppedDown() arm: see DEVIATION above -- not evaluated.

    if ((nStyle & 0x00010000u) != 0) {
        clrFill = (state == kXPStateHighlighted) ? x.c11c : x.c120;
    }
    if (bIsMenuButton && (nStyle & 0x00040000u) != 0) {
        clrFill = x.c110;
    }
    if ((nStyle & 0x00010000u) == 0) {
        rect.InflateRect(-1, -1);
    }
    XP_FillHighlightedSolid(pDC, rect, clrFill);
}

// CMFCVisualManagerOfficeXP::OnFillCaptionBarButton(CDC*, CMFCCaptionBar*, CRect rect,
//         BOOL bIsPressed, BOOL bIsHighlighted, BOOL bIsDisabled,
//         BOOL bHasDropDownArrow, BOOL bIsSysButton) -- retail RVA 0x1b2ef0 (mfc140.dll):
//     if (!pBar->m_bIsMessageBarMode /*+0x404*/)                        // 0x1b2efc
//         return CMFCVisualManager::OnFillCaptionBarButton(...same args...);  // 0x186e20
//     if (bIsDisabled) return (COLORREF)-1;                            // [rsp+0xb0], 0x1b2f5f
//     afxGlobalData.Initialize();
//     if (bIsHighlighted) {                                            // [rsp+0xa8], 0x1b2f90
//         this->OnFillHighlightedArea(pDC, rect, &m_brHighlight /*+0x1a0*/, NULL);  // 0x1b2fc9
//         return <black if every byte of m_clrHighlight > 0x80, else white>;        // 0x1b2fcf..0x1b2ff2
//     }
//     COLORREF clrText = afxGlobalData.clrBarText;                     // +0x74, 0x1b2ffc
//     if (!bIsSysButton)                                               // [rsp+0xc0], 0x1b2ff4
//         ::FillRect(pDC->m_hDC, rect, m_brMenuLight.m_hObject);       // +0x180, 0x1b301d
//     return clrText;
// bIsPressed and bHasDropDownArrow are never read.  The +0x404 read goes through
// kXPCaptionBarIsMessageBarMode; the brushes are XP_Colors() c118 / c110.
// Retail dereferences pBar unconditionally; the null guard is OpenMFC's.
// Symbol: ?OnFillCaptionBarButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCCaptionBar* pBar, CRect rect,
    int bIsPressed, int bIsHighlighted, int bIsDisabled, int bHasDropDownArrow, int bIsSysButton)
{
    if (pBar == nullptr) return static_cast<unsigned long>(-1);

    if (XP_ReadInt(pBar, kXPCaptionBarIsMessageBarMode) == 0) {
        return impl__OnFillCaptionBarButton_CMFCVisualManager__UEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(
            pThis, pDC, pBar, rect, bIsPressed, bIsHighlighted, bIsDisabled, bHasDropDownArrow, bIsSysButton);
    }

    if (bIsDisabled) return static_cast<unsigned long>(-1);

    const XPColors x = XP_Colors();
    if (bIsHighlighted) {
        XP_FillHighlightedSolid(pDC, rect, x.c118);
        return XP_TextOverHighlight(x.c118);
    }

    const COLORREF clrText = GD_clrBtnText();
    if (!bIsSysButton) {
        XP_FillSolid(pDC, rect, x.c110);
    }
    return clrText;
}

// CMFCVisualManagerOfficeXP::OnFillCommandsListBackground(CDC*, CRect rect,
//         BOOL bIsSelected) -- retail RVA 0x1aed30 (mfc140.dll):
//     afxGlobalData.Initialize();
//     int nMargin = this->GetMenuImageMargin();                       // vtable +0x5c8, 0x1aed82
//     CSize sz = CMFCToolBar::GetMenuImageSize();                     // 0x1aed8f
//     if (bIsSelected) {
//         if (m_bEnableToolbarButtonFill /*+0xb0*/) rect.left = 0;    // 0x1aed9d/0x1aeda6
//         this->OnFillHighlightedArea(pDC, rect, &m_brHighlight /*+0x1a0*/, NULL); // 0x1aedda
//         pDC->Draw3dRect(rect, m_clrMenuItemBorder, m_clrMenuItemBorder);       // 0x1aedf0
//         return <black if every byte of m_clrHighlight > 0x80, else white>;     // 0x1aedf5..0x1aee1b
//     }
//     ::FillRect(pDC->m_hDC, rect, m_brMenuLight.m_hObject);          // +0x180, 0x1aee3b
//     CRect rectImages = rect;
//     rectImages.right = rect.left + 2 + nMargin + sz.cx;             // 0x1aee1d/0x1aee54..0x1aee59
//     ::FillRect(pDC->m_hDC, rectImages, m_brBarBkgnd.m_hObject);     // +0x160, 0x1aee6f
//     return afxGlobalData.clrBarText;                                // +0x74, 0x1aee94
// The +0xb0 flag is read through kXPVmEnableToolbarButtonFill (see its
// comment: OpenMFC's ctor leaves it 0 where retail's sets 1).  Brushes come
// from XP_Colors() (c118 / c110 / c108) and the margin and image size from the
// local helpers, with the deviations noted there.
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, int bIsSelected)
{
    const XPColors x = XP_Colors();
    const int nMargin = XP_MenuImageMargin();
    const SIZE sz = XP_MenuImageSize();

    if (bIsSelected) {
        if (pThis != nullptr && XP_ReadInt(pThis, kXPVmEnableToolbarButtonFill) != 0) {
            rect.left = 0;
        }
        XP_FillHighlightedSolid(pDC, rect, x.c118);
        DC_Draw3dRect(pDC, rect, x.c138, x.c138);
        return XP_TextOverHighlight(x.c118);
    }

    XP_FillSolid(pDC, rect, x.c110);

    CRect rectImages(rect);
    rectImages.right = rect.left + 2 + nMargin + sz.cx;
    XP_FillSolid(pDC, rectImages, x.c108);

    return GD_clrBtnText();
}


// CMFCVisualManagerOfficeXP::OnFillHighlightedArea(CDC*, CRect rect,
//         CBrush* pBrush, CMFCToolBarButton* pButton) -- retail RVA 0x1b1400.
// The body is transcribed in XP_FillHighlighted() above, which also documents
// the one deviation (the CMFCToolBarImages::m_bIsDrawOnGlass branch).  Retail
// passes NULL to ::FillRect when pBrush is NULL rather than skipping the call
// (the `test %r9,%r9` at 0x1b1469); that is reproduced.  pButton is not read.
// CBrush::m_hObject is at +8 -- retail reads 0x8(%r9), and OpenMFC's
// CGdiObject (include/openmfc/afxwin.h) puts m_hObject directly after the
// CObject vptr -- so CGdiObject::GetSafeHandle(), which is inline in the
// header, reads exactly what retail reads.
// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect, CBrush* pBrush,
    CMFCToolBarButton* /*pButton*/)
{
    XP_FillHighlighted(pDC, rect,
                       pBrush != nullptr ? static_cast<HBRUSH>(pBrush->GetSafeHandle()) : nullptr);
}

// CMFCVisualManagerOfficeXP::OnFillMiniFrameCaption(CDC*, CRect rectCaption,
//         CPaneFrameWnd* pFrameWnd, BOOL bActive) -- retail RVA 0x1affc0 (mfc140.dll):
//     BOOL bIsTasksPane = pFrameWnd->IsKindOf(RUNTIME_CLASS(CMFCTasksPaneFrameWnd));  // 0x1803aa5c8, 0x1affe1
//     CWnd* pPane = pFrameWnd->GetPane();                              // vtable +0x360, 0x1afff5
//     if (pPane != NULL && pPane->IsKindOf(RUNTIME_CLASS(CMFCBaseToolBar))) {  // 0x1802dcad0, 0x1b000c
//         ::FillRect(pDC->m_hDC, rectCaption, m_brFloatToolBarBorder.m_hObject);  // +0x1d0, 0x1b00e9
//         return afxGlobalData.clrCaptionText;                         // +0x80, 0x1b010d
//     }
//     if (bIsTasksPane) {
//         ::FillRect(pDC->m_hDC, rectCaption, afxGlobalData.brBarFace.m_hObject);  // +0x120, 0x1b0061
//         return afxGlobalData.clrBarText;                             // +0x74, 0x1b0085
//     }
//     ::FillRect(pDC->m_hDC, rectCaption, bActive ? afxGlobalData.brActiveCaption.m_hObject    // +0xf0
//                                                 : afxGlobalData.brInactiveCaption.m_hObject); // +0x100
//     return afxGlobalData.clrCaptionText;                             // +0x80
// The two runtime-class descriptors read back as "CMFCTasksPaneFrameWnd" and
// "CMFCBaseToolBar".  Slot +0x360 of the retail CPaneFrameWnd vtable
// (0x1802f5ad8, installed by ??0CPaneFrameWnd@@QEAA@XZ at 0xad6e6) is
// ?GetPane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ (0xaf900); OpenMFC's export of it is
// called directly since the MSVC slot cannot be dispatched here.  brActiveCaption /
// brInactiveCaption / brBarFace are the solid brushes UpdateSysColors (0x6afd0)
// builds from ::GetSysColor(COLOR_ACTIVECAPTION) (field +0x88, brush at 0x6b284),
// ::GetSysColor(COLOR_INACTIVECAPTION) (+0x8c, 0x6b2aa) and COLOR_BTNFACE (+0x60,
// 0x6b25e).  m_brFloatToolBarBorder is ::CreateSolidBrush of the value XP_Colors()
// returns as c1d0.  Retail dereferences pFrameWnd unconditionally; the null guard
// is OpenMFC's.
// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rectCaption, CPaneFrameWnd* pFrameWnd, int bActive)
{
    const bool bIsTasksPane =
        pFrameWnd != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pFrameWnd),
            impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ()) != 0;

    const CObject* pPane = (pFrameWnd != nullptr)
        ? static_cast<const CObject*>(static_cast<CWnd*>(impl__GetPane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pFrameWnd)))
        : nullptr;
    if (pPane != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pPane, impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ()) != 0) {
        XP_FillSolid(pDC, rectCaption, XP_Colors().c1d0);
        return GD_clrCaptionText();
    }

    if (bIsTasksPane) {
        XP_FillSolid(pDC, rectCaption, GD_clrBtnFace());
        return GD_clrBtnText();
    }

    XP_FillSolid(pDC, rectCaption, ::GetSysColor(bActive ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION));
    return GD_clrCaptionText();
}


// CMFCVisualManagerOfficeXP::OnFillPopupWindowBackground(CDC*, CRect rect)
// -- retail RVA 0x1b1ab0, one call:
//     ::FillRect(pDC->m_hDC, rect, this->[+0x180].m_hObject);
// The +0x180 brush is ::CreateSolidBrush(this->[+0x110]), built by the call at
// 0x1ac929 inside OnUpdateSystemColors.
// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect)
{
    XP_FillPopupWindowBackground(pDC, rect);
}

// Retail (RVA 0x1b1bf0, mfc140.dll) is a 0x5b0-byte body that starts with the
// button's virtual at vtable +0x4a8 (0x1b1c35) and its +0x138 member, then
// dispatches +0x418, +0x1b0, +0x1d8, +0x1c8 (IsHighlighted / IsPressed /
// IsChecked / IsDisabled and relatives), tests an IsKindOf against the
// descriptor at 0x180302a10, and fills through CDrawingManager::DrawRect /
// HighlightRect (0x5abf0 / 0x56750) or the +0x638 virtual with the OfficeXP
// brushes, returning a text colour.  OpenMFC's CMFCRibbonButton placeholder
// carries none of those virtuals or members and both CDrawingManager exports
// are stubs; left a stub.  The `return 0` is the generated stub's value.
// Symbol: ?OnFillRibbonButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CMFCRibbonButton* /*pButton*/) {
    return 0;
}


// CMFCVisualManagerOfficeXP::OnFillRibbonMenuFrame(CDC*, CMFCRibbonMainPanel*,
//         CRect rect) -- retail RVA 0x1b26b0, one call:
//     ::FillRect(pDC->m_hDC, rect, this->[+0x180].m_hObject);
// The panel pointer is not read.  Same +0x110 brush as
// OnFillPopupWindowBackground above.
// Symbol: ?OnFillRibbonMenuFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, void* /*pPanel*/ /* CMFCRibbonMainPanel* */,
    CRect rect)
{
    XP_FillSolid(pDC, rect, XP_Colors().c110);
}

// CMFCVisualManagerOfficeXP::OnFillTab(CDC*, CRect rectFill, CBrush* pbrFill, int iTab,
//         BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd) -- retail RVA 0x1ae6f0 (mfc140.dll):
//     if (pTabWnd->GetTabBkColor(iTab) != -1 && !bIsActive) {           // vtable +0x3c8, 0x1ae72b
//         CBrush br(pTabWnd->GetTabBkColor(iTab));                        // 0x2a1fa0
//         ::FillRect(pDC->m_hDC, rectFill, br); return; }                 // 0x1ae76f
//     if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style() || pTabWnd->IsLeftRightRounded())  // +0x520/+0x528/+0x530
//         { CMFCVisualManager::OnFillTab(pDC, rectFill, pbrFill, iTab, bIsActive, pTabWnd); return; }  // 0x185110
//     if (bIsActive) ::FillRect(pDC->m_hDC, rectFill, pbrFill ? pbrFill->m_hObject : NULL);  // 0x1ae7ee
// Left a stub: GetTabBkColor and the three style virtuals are CMFCBaseTabCtrl
// vtable slots (+0x3c8 resolves to the exported ?GetTabBkColor@CMFCBaseTabCtrl@@
// on the retail vtable at 0x1802dbd88) that OpenMFC's CMFCBaseTabCtrl does not
// carry, so neither the early-out nor the delegation can be decided.
// Symbol: ?OnFillTab@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* /*pDC*/, CRect /*rectFill*/, CBrush* /*pbrFill*/,
    int /*iTab*/, int /*bIsActive*/, const CMFCBaseTabCtrl* /*pTabWnd*/) {}

// CMFCVisualManagerOfficeXP::OnFillTasksGroupInterior(CDC*, CRect rect, BOOL bSpecial)
// -- retail RVA 0x1b0af0 (mfc140.dll):
//     afxGlobalData.Initialize();
//     CGdiObject* pOld = pDC->SelectObject(&afxGlobalData.penBarShadow);   // +0x148, 0x1b0b2e
//     pDC->MoveTo(rect.left, rect.top);                                   // 0x1b0b45
//     pDC->LineTo(rect.right, rect.top);                                  // 0x1b0b54
//     pDC->SelectObject(pOld);                                            // tail-jump at 0x1b0b6e
// bSpecial is never read.  The pen at afxGlobalData +0x148 is created by
// UpdateSysColors (0x6afd0) at 0x6b397 as ::CreatePen(PS_SOLID, 1, <field +0x64>),
// i.e. clrBarShadow == ::GetSysColor(COLOR_BTNSHADOW).
// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rect, int /*bSpecial*/)
{
    XP_DrawLine(pDC, rect.left, rect.top, rect.right, rect.top, GD_clrBtnShadow());
}

// CMFCVisualManagerOfficeXP::OnHighlightMenuItem(CDC*, CMFCToolBarMenuButton*,
//         CRect rect, COLORREF& clrText) -- retail RVA 0x1ae070 (mfc140.dll):
//     BOOL bDisabled = pButton->m_nStyle & TBBS_DISABLED;              // +0x28 & 0x40000, 0x1ae07d/0x1ae09b
//     ::InflateRect(&rect, -1, 0);                                     // 0x1ae0a4
//     this->OnFillHighlightedArea(pDC, rect,                           // vtable +0x638, 0x1ae0e0
//         bDisabled ? &m_brMenuLight /*+0x180*/ : &m_brHighlight /*+0x1a0*/, pButton);
//     pDC->Draw3dRect(rect, m_clrMenuItemBorder, m_clrMenuItemBorder); // +0x138, 0x1ae0f6
//     clrText = this->GetHighlightedMenuItemTextColor(pButton);        // vtable +0x138, 0x1ae10b
// The brush choice is the `neg %ebx; sbb %r9,%r9; and $-32; add $0x1a0` at
// 0x1ae0b5..0x1ae0d0.  The rect copy handed to OnFillHighlightedArea is taken
// at 0x1ae0b2, i.e. after the InflateRect.  Slot +0x138 of the retail OfficeXP
// vtable (0x18031b3d8) is ?GetHighlightedMenuItemTextColor@CMFCVisualManagerOfficeXP@@
// (0x1ae130), whose OpenMFC body is defined earlier in this file and is called
// directly (this build cannot dispatch through the MSVC vtable slot -- see the
// file header).  The two brushes are ::CreateSolidBrush of m_clrMenuLight
// (+0x110) and m_clrHighlight (+0x118); XP_Colors() supplies both.  Retail
// dereferences pButton unconditionally; the null guard is OpenMFC's.
// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CMFCToolBarButton* pButton, CRect rect,
    unsigned long* pclrText)
{
    const bool bDisabled = (pButton != nullptr) && (pButton->m_nStyle & 0x00040000u) != 0;
    rect.InflateRect(-1, 0);

    const XPColors x = XP_Colors();
    XP_FillHighlightedSolid(pDC, rect, bDisabled ? x.c110 : x.c118);
    DC_Draw3dRect(pDC, rect, x.c138, x.c138);

    if (pclrText != nullptr) {
        *pclrText = impl__GetHighlightedMenuItemTextColor_CMFCVisualManagerOfficeXP__MEAAKPEAVCMFCToolBarMenuButton___Z(
            pThis, pButton);
    }
}

// CMFCVisualManagerOfficeXP::OnHighlightQuickCustomizeMenuButton(CDC*,
//         CMFCToolBarMenuButton*, CRect rect) -- retail RVA 0x1ae1a0 (mfc140.dll):
//     ::FillRect(pDC->m_hDC, rect, m_brBarBkgnd.m_hObject);       // +0x160, call at 0x1ae1c8
//     pDC->Draw3dRect(rect, m_clrMenuBorder, m_clrMenuBorder);    // +0x134, call at 0x1ae1de
// The button pointer arrives in r8 and is overwritten at 0x1ae1a8 before it is
// ever read.  Member names follow afxvisualmanagerofficexp.h (the OfficeXP
// members start at +0x108 with m_clrBarBkgnd; the CBrush at +0x160 is
// m_brBarBkgnd, ::CreateSolidBrush(m_clrBarBkgnd) at 0x1ac8f3 inside
// OnUpdateSystemColors), so XP_Colors().c108 stands in for the brush and c134
// for m_clrMenuBorder -- see the file header for how faithful those are.
// Symbol: ?OnHighlightQuickCustomizeMenuButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightQuickCustomizeMenuButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CMFCToolBarMenuButton* /*pButton*/, CRect rect)
{
    const XPColors x = XP_Colors();
    XP_FillSolid(pDC, rect, x.c108);
    DC_Draw3dRect(pDC, rect, x.c134, x.c134);
}

// CMFCVisualManagerOfficeXP::OnHighlightRarelyUsedMenuItems(CDC*, CRect rectRarelyUsed)
// -- retail RVA 0x1ae1f0 (mfc140.dll):
//     rectRarelyUsed.left--;                                            // 0x1ae201
//     int nMargin = this->GetMenuImageMargin();                         // vtable +0x5c8, 0x1ae214
//     CSize sz = CMFCToolBar::GetMenuImageSize();                       // 0x155a60
//     rectRarelyUsed.right = rectRarelyUsed.left + 2 + 2 * nMargin + sz.cx;  // 0x1ae227..0x1ae235
//     ::FillRect(pDC->m_hDC, rectRarelyUsed, m_brMenuRarelyUsed.m_hObject); // +0x170, 0x1ae249
// (rectRarelyUsed.left is read AFTER the decrement, at 0x1ae207.)  The +0x170
// brush is ::CreateSolidBrush(m_clrMenuRarelyUsed) (0x1ac90e), member +0x10c,
// XP_Colors().c10c.  GetMenuImageMargin / GetMenuImageSize come from the local
// XP_MenuImageMargin / XP_MenuImageSize helpers -- see their comments for the
// deviations (a derived class's override of the margin virtual; the omitted
// 1.0x DPI scaling step).
// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, CRect rectRarelyUsed)
{
    rectRarelyUsed.left--;
    const SIZE sz = XP_MenuImageSize();
    rectRarelyUsed.right = rectRarelyUsed.left + 2 + 2 * XP_MenuImageMargin() + sz.cx;
    XP_FillSolid(pDC, rectRarelyUsed, XP_Colors().c10c);
}

// CMFCVisualManagerOfficeXP::OnUpdateSystemColors() -- retail RVA 0x1ac0e0
// (mfc140.dll).  It does NOT call the base class: it deletes the nine GDI
// objects at +0x160..+0x1e0 and +0x190 (CGdiObject::DeleteObject, 0x2a1ea0, the
// run at 0x1ac0fb..0x1ac15b), recomputes the thirteen colour members exactly
// as XP_Colors() at the top of this file transcribes (its comment cites every
// store), sets m_clrPressedButtonBorder (+0x124) to -1 (0x1ac9e5), and
// re-creates the brushes and pens with ::CreateSolidBrush / ::CreatePen +
// CGdiObject::Attach (0x1ac8ed..0x1aca0f).  Left a stub: OpenMFC's
// CMFCVisualManagerOfficeXP has no storage for any of those members (see the
// file header), so there is nothing to write; every body in this file that
// needs one of the values recomputes it through XP_Colors() instead, which is
// the deviation the header describes.
// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerOfficeXP@@MEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOfficeXP__MEAAXXZ(CMFCVisualManagerOfficeXP* /*pThis*/) {}
