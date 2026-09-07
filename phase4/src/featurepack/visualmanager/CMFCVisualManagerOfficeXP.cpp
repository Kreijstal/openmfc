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
//    storage at those offsets, so they cannot be read.  Where one is needed the
//    comment names it and says what OnUpdateSystemColors derives it from;
//    entries that cannot be reproduced honestly are left as documented stubs.
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
//    +0x628 (slot 197)  CMFCVisualManagerOfficeXP::ExtendMenuButton            (0x1af6d0)
//    +0x638 (slot 199)  CMFCVisualManagerOfficeXP::OnFillHighlightedArea       (0x1b1400)
// All three are themselves stubs in OpenMFC, which is part of why the callers
// that need them stay stubbed.
// ---------------------------------------------------------------------------

// Implementations in sibling translation units that this unit calls.
extern "C" unsigned long MS_ABI impl__PixelAlpha_CDrawingManager__SAKKH_Z(unsigned long srcPixel, int nPercent);
extern "C" unsigned long MS_ABI impl__GetPropertyGridGroupColor_CMFCVisualManager__UEAAKPEAVCMFCPropertyGridCtrl___Z(
    CMFCVisualManager* pThis, CMFCPropertyGridCtrl* pList);
extern "C" void MS_ABI impl__OnDrawPaneBorder_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__AEAVCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect* pRect);
// CMFCToolBarEditBoxButton::m_bFlat (retail static at image address 0x1803aab78).
extern "C" std::int32_t impl__m_bFlat_CMFCToolBarEditBoxButton__1HA;
// CMFCToolBar::m_bCustomizeMode (retail static at image address 0x1803b70bc);
// OpenMFC defines it in phase4/src/featurepack/toolbar/StaticData.cpp.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetToolbarButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCToolBarButton__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager* pThis, CMFCToolBarButton* pButton, int nState);

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

// CMFCVisualManagerOfficeXP's border colour, retail member +0x138.  It is also
// the colour of the 1px separator pen at +0x1f0, which OnUpdateSystemColors
// creates from it with the ::CreatePen call at 0x1ac9f7.  OnUpdateSystemColors
// sets +0x138 to
// afxGlobalData.clrHilite (+0x48 == ::GetSysColor(COLOR_HIGHLIGHT)) on its
// low-colour / high-contrast path (store at 0x1ac8e7; the only exception there
// is a high-contrast desktop, where it uses clrBtnDkShadow instead), and on its
// gradient path (store at 0x1ac539) to either clrHilite unchanged or
// CDrawingManager::PixelAlpha(clrHilite, 0x54).  OpenMFC models neither the
// member nor afxGlobalData, so this returns clrHilite: exact on the branches
// that use it unchanged, and an undarkened approximation on the PixelAlpha one.
inline COLORREF XPBorderColor() { return ::GetSysColor(COLOR_HIGHLIGHT); }

// CMFCVisualManagerOfficeXP's highlight fill colour, retail member +0x118 (the
// brush at +0x1a0 is ::CreateSolidBrush of it, made by the call at 0x1ac93e).
// This is an APPROXIMATION on every retail path -- none of them simply stores
// clrHilite:
//   * gradient path: a 0xb2/0x4d weighted blend of member +0x110 with
//     afxGlobalData.clrHilite, passed through CDrawingManager::PixelAlpha with
//     0x5b or 0x66 (stores at 0x1ac4b6 / 0x1ac505) -- a pale tint.
//   * low-colour path: afxGlobalData.clrBtnFace (+0x28) on the ordinary
//     sub-branch (store at 0x1ac74f), and afxGlobalData.clrHilite (+0x48) only
//     on the white-high-contrast sub-branch, i.e. when afxGlobalData+0x260 is
//     set (store at 0x1ac705; the branch is decided by the test at 0x1ac6e4).
// Returning clrHilite is therefore exact only on that last, rare sub-branch and
// is a saturated stand-in everywhere else.  Reproducing it properly needs the
// unmodelled member +0x110 (see the file header).
inline COLORREF XPHighlightColor() { return ::GetSysColor(COLOR_HIGHLIGHT); }

// CMFCVisualManagerOfficeXP's bar background, retail member +0x108 (the brush
// at +0x160 is CreateSolidBrush of it, made at 0x1ac8ed).  OnUpdateSystemColors
// stores afxGlobalData.clrBtnFace there on the low-colour path (0x1ac6c3); on
// the gradient path it is an 86%/14% blend of that colour with a virtual-call
// result, which OpenMFC cannot reproduce.
inline COLORREF XPBarBkgndColor() { return ::GetSysColor(COLOR_BTNFACE); }

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
// Symbol: ?CreateGripperBrush@CMFCVisualManagerOfficeXP@@MEAAXXZ
extern "C" void MS_ABI impl__CreateGripperBrush_CMFCVisualManagerOfficeXP__MEAAXXZ() {}

// Retail (RVA 0x1af6d0) walks pButton->m_pPopupMenu (+0xc0) to its parent
// CMFCPopupMenuBar (+0x40), screen-maps the rects and grows the caller's rect
// by 1..4 px along the drop direction held at +0x1650 of the popup menu.  None
// of CMFCToolBarMenuButton/CMFCPopupMenu is laid out to those offsets in
// OpenMFC, so the member reads cannot be reproduced.
// Symbol: ?ExtendMenuButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCMFCToolBarMenuButton@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__ExtendMenuButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCMFCToolBarMenuButton__AEAVCRect___Z(void* /*class*/* p0, void* /*class*/* p1) {}

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
// -- retail RVA 0x1ae130:
//     if (pButton->m_nStyle (+0x28) & TBBS_DISABLED /*0x40000*/)
//         return afxGlobalData.clrGrayedText (+0x44);
//     // otherwise a luminance test on the three bytes of the OfficeXP
//     // highlight colour, member +0x118:
//     return (R > 0x80 && G > 0x80 && B > 0x80) ? RGB(0,0,0) : RGB(255,255,255);
// The disabled branch is reproduced exactly.  Member +0x118 has no storage in
// OpenMFC (see the file header), so the second branch is fixed at black -- the
// value retail computes whenever that colour is a light tint, which is what
// OnUpdateSystemColors derives on every non-high-contrast desktop.  Retail
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
    return RGB(0, 0, 0);
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

// Retail (RVA 0x1b2960) selects the two out-params from the OfficeXP colour
// members at +0x108..+0x138 and blends them with CDrawingManager::PixelAlpha.
// Those members have no storage in OpenMFC (see the file header).
// Symbol: ?GetRibbonSliderColors@CMFCVisualManagerOfficeXP@@MEAAXPEAVCMFCRibbonSlider@@HHHAEAK1@Z
extern "C" void MS_ABI impl__GetRibbonSliderColors_CMFCVisualManagerOfficeXP__MEAAXPEAVCMFCRibbonSlider__HHHAEAK1_Z(void* /*class*/* p0, int p1, int p2, int p3, unsigned long* p4, int p5) {}

// Retail (RVA 0x1b1800) is exactly `*p0 = this->[+0x108]; *p1 = this->[+0x134];`.
// Both are OfficeXP colour members with no storage in OpenMFC, and unlike the
// cases handled above they are not reducible to a single ::GetSysColor value
// (+0x134 is clrBtnDkShadow on the low-colour path but a PixelAlpha blend on
// the gradient path), so no honest substitute exists.
// Symbol: ?GetSmartDockingBaseGuideColors@CMFCVisualManagerOfficeXP@@MEAAXAEAK0@Z
extern "C" void MS_ABI impl__GetSmartDockingBaseGuideColors_CMFCVisualManagerOfficeXP__MEAAXAEAK0_Z(unsigned long* p0, unsigned long* p1) {}

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

// Retail (RVA 0x1aca30) paints the gripper with the pattern brushes
// CreateGripperBrush() installs at +0x140/+0x150 and with the colour members
// at +0x108/+0x128/+0x150; none of them exist in OpenMFC.
// Symbol: ?OnDrawBarGripper@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__OnDrawBarGripper_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HPEAVCBasePane___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, void* /*class*/* p3) {}

// Retail (RVA 0x1b1610) fills with the OfficeXP brushes at +0x1a0/+0x1b0 and
// frames with the colour member at +0x138, then returns the text colour
// through the CDC virtual at +0x70.  The brush members have no storage here.
// Symbol: ?OnDrawBrowseButton@CMFCVisualManagerOfficeXP@@MEAAHPEAVCDC@@VCRect@@PEAVCMFCEditBrowseCtrl@@W4AFX_BUTTON_STATE@CMFCVisualManager@@AEAK@Z
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7, unsigned long* p8) {
    return 0;
}

// Retail (RVA 0x1adc30) picks between the colour members at +0x138, +0x134 and
// +0x124, calls the visual-manager virtual at vtable +0x628 and, for a menu
// button on a drop-down bar, CDrawingManager::DrawShadow (0x58c80) with the
// members at +0xcc/+0xf8.  Too much unmodelled state to transcribe honestly.
// Symbol: ?OnDrawButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnDrawButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

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

// Retail (RVA 0x1b3040) delegates to CMFCVisualManager::OnDrawCaptionBarButtonBorder
// (0x186ee0) when pBar->[+0x404] is zero, and otherwise frames with the colour
// members at +0x124/+0x138.  OpenMFC's CMFCCaptionBar is not laid out to
// +0x404 and the OfficeXP members do not exist, so the branch cannot be taken
// faithfully.
// Symbol: ?OnDrawCaptionBarButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionBarButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, int p6, int p7) {}

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

// Retail (RVA 0x1aea90) paints from the OfficeXP brush/colour members and the
// CMFCCaptionButton rect/state fields, none of which OpenMFC lays out.
// Symbol: ?OnDrawCaptionButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionButton@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawCaptionButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4, int p5, int p6) {}

// Retail (RVA 0x1aec70) routes the glyph through CMenuImages::Draw with a
// state chosen from the OfficeXP colour members and the button's own fields.
// OpenMFC's CMenuImages::Draw export is itself an empty stub, so nothing would
// be painted even with the state decoded.
// Symbol: ?OnDrawCaptionButtonIcon@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCCaptionButton@@W4IMAGES_IDS@CMenuImages@@HHVCPoint@@@Z
extern "C" void MS_ABI impl__OnDrawCaptionButtonIcon_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCCaptionButton__W4IMAGES_IDS_CMenuImages__HHVCPoint___Z(void* /*class*/* p0, void* /*class*/* p1, int /*enum*/ p2, unsigned int p3, float p4, unsigned short* p5, unsigned char p6, void* p7, int p8, unsigned int p9, char p10, void* p11, void* p12, float p13, void* p14, void* p15, void* p16, unsigned int p17, void* p18, void* p19, void* p20, void* p21, void* p22, int p23, int p24, void* /*class*/ p25) {}

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

// Retail (RVA 0x1afa30) fills the drop button from the OfficeXP brushes at
// +0x1a0/+0x1b0/+0x1c0 before drawing the arrow; those members do not exist here.
// Symbol: ?OnDrawComboDropButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHHPEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, int p4, void* /*class*/* p5) {}

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

// Retail (RVA 0x1b0360) frames the floating bar with ::PatBlt runs driven by
// the OfficeXP brush members and the border-size rect; unmodelled state.
// Symbol: ?OnDrawFloatingToolbarBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCBaseToolBar@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawFloatingToolbarBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCBaseToolBar__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

// Retail (RVA 0x1aeec0) sets rectArrow.left = rect.right - CMenuImages::Size().cx
// (0x8fbb0), then picks black + CMenuImages::ImageBlack, or white +
// CMenuImages::ImageWhite, from a luminance test on the three bytes of the
// OfficeXP highlight colour member +0x118 (compares at 0x1aeefa/0x1aef02/
// 0x1aef0a); it draws glyph id 0xe with CMenuImages::Draw (0x8fd40) and then a
// 1px vertical rule at x = rectArrow.left - 1 with a LOCAL CPen built from that
// same colour.  Member +0x118 has no storage in OpenMFC (see the file header)
// and both CMenuImages entry points are no-op stubs here, so neither the glyph
// nor the black/white choice for the rule can be reproduced.
// Symbol: ?OnDrawMenuArrowOnCustomizeList@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuArrowOnCustomizeList_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Retail (RVA 0x1ace60) frames the popup using the OfficeXP colour members and
// the popup menu's own layout fields; neither is modelled here.
// Symbol: ?OnDrawMenuBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCPopupMenu@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMenuBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCPopupMenu__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

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

// Retail (RVA 0x1af250) paints the resize gripper from the OfficeXP brush
// members at +0x160/+0x1d0; unmodelled state.
// Symbol: ?OnDrawMenuResizeBar@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawMenuResizeBar_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

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

// Retail (RVA 0x1ad1f0) is a CDrawingManager::DrawShadow driver that caches
// into the caller's CBitmap objects.  CDrawingManager::DrawShadow is not
// implemented in OpenMFC, so there is nothing to drive.
// Symbol: ?OnDrawMenuShadow@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEBVCRect@@1HHHPEAVCBitmap@@2H@Z
extern "C" void MS_ABI impl__OnDrawMenuShadow_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEBVCRect__1HHHPEAVCBitmap__2H_Z(void* /*class*/* p0, const void* /*class*/* p1, const void* /*class*/* p2, int p3, int p4, int p5, void* /*class*/* p6, int p7, int p8) {}

// Retail (RVA 0x1af880) maps the SC_* command to a CMenuImages id, optionally
// calls the visual-manager virtual at vtable +0x638 (OnFillHighlightedArea)
// with the +0x1a0 brush and frames with +0x138, then draws the glyph through
// CMenuImages::Draw.  Both the brush member and CMenuImages::Draw are missing.
// Symbol: ?OnDrawMenuSystemButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@IIH@Z
extern "C" void MS_ABI impl__OnDrawMenuSystemButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__IIH_Z(void* /*class*/* p0, void* /*class*/ p1, unsigned int p2, unsigned int p3, int p4) {}

// Retail (RVA 0x1b0120) selects the OfficeXP brush at +0x1d0 and paints eight
// ::PatBlt runs with PATCOPY (0xf00021); the brush member does not exist here.
// Symbol: ?OnDrawMiniFrameBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCPaneFrameWnd@@VCRect@@2@Z
extern "C" void MS_ABI impl__OnDrawMiniFrameBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCPaneFrameWnd__VCRect__2_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3) {}

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

// Retail (RVA 0x1b18c0) is
//     pDC->Draw3dRect(rect, this->[+0x134], this->[+0x134]);
//     rect.InflateRect(-1, -1);
//     pDC->Draw3dRect(rect, this->[+0x110], this->[+0x110]);
// Both members are OfficeXP colours with no OpenMFC storage, and neither
// reduces to one ::GetSysColor value across OnUpdateSystemColors' two paths.
// Symbol: ?OnDrawPopupWindowBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawPopupWindowBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

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

// Retail (RVA 0x1b21d0) reads the ribbon button's rect/state fields and the
// OfficeXP brush members; the ribbon element layout is not modelled here.
// Symbol: ?OnDrawRibbonButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Retail (RVA 0x1b24d0) fills through the visual-manager virtual at vtable
// +0x638 with either the +0x1a0 brush or afxGlobalData.brBarFace, then draws
// the arrow with CMenuImages::Draw and frames with clrBarShadow.  The scroll
// object's rect at +0xc8 and pressed flag at +0x270 are not modelled in
// OpenMFC's CMFCRibbonCategoryScroll, so the geometry cannot be read.
// Symbol: ?OnDrawRibbonCategoryScroll@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCRibbonCategoryScroll@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonCategoryScroll_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCRibbonCategoryScroll___Z(void* /*class*/* p0, void* /*class*/* p1) {}

// Retail (RVA 0x1b2b10) paints the swatch from the OfficeXP highlight members
// and the gallery icon's state; unmodelled state.
// Symbol: ?OnDrawRibbonColorPaletteBox@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonColorButton@@PEAVCMFCRibbonGalleryIcon@@KVCRect@@HHHHH@Z
extern "C" void MS_ABI impl__OnDrawRibbonColorPaletteBox_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonColorButton__PEAVCMFCRibbonGalleryIcon__KVCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned long p3, void* /*class*/ p4, int p5, int p6, int p7, int p8, int p9) {}

// CMFCVisualManagerOfficeXP::OnDrawRibbonMenuCheckFrame(CDC*, CMFCRibbonButton*,
//         CRect rect) -- retail RVA 0x1b2650:
//     this->OnFillHighlightedArea(pDC, rect, &this->[+0x1a0], NULL);  // vtable +0x638
//     pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);          // border colour
// The button pointer is not read.  Deviation: OpenMFC's
// CMFCVisualManagerOfficeXP::OnFillHighlightedArea export is still an empty
// stub, so the solid fill it would perform with the +0x1a0 highlight brush is
// done inline here with XPHighlightColor(); +0x138 is XPBorderColor().
// Symbol: ?OnDrawRibbonMenuCheckFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonMenuCheckFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonButton__VCRect___Z(
    CMFCVisualManagerOfficeXP* /*pThis*/, CDC* pDC, void* /*pButton*/ /* CMFCRibbonButton* */,
    CRect rect)
{
    XP_FillSolid(pDC, rect, XPHighlightColor());
    DC_Draw3dRect(pDC, rect, XPBorderColor(), XPBorderColor());
}

// Retail (RVA 0x1b2a40) reads the separator element's orientation fields and
// the OfficeXP pen members; neither is modelled here.
// Symbol: ?OnDrawRibbonQuickAccessToolBarSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonSeparator@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawRibbonQuickAccessToolBarSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonSeparator__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

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

// Retail (RVA 0x1b2790) chooses the pane background from the status bar's
// extended-mode flag and the OfficeXP colour members before returning the text
// colour; the ribbon status bar layout is not modelled in OpenMFC.
// Symbol: ?OnDrawRibbonStatusBarPane@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonStatusBar@@PEAVCMFCRibbonStatusBarPane@@@Z
extern "C" unsigned long MS_ABI impl__OnDrawRibbonStatusBarPane_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonStatusBar__PEAVCMFCRibbonStatusBarPane___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2) {
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

// Retail (RVA 0x1ada00) delegates to CMFCVisualManager::OnDrawSeparator
// (0x183750) for dialog-control panes, and otherwise selects the OfficeXP pen
// at +0x1e0 and shortens the rule using CMFCToolBar fields at +0x1350/+0x142c
// and CMFCToolBar::GetMenuImageSize.  Those toolbar offsets are not modelled
// in OpenMFC, so the geometry cannot be reproduced.
// Symbol: ?OnDrawSeparator@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3) {}

// Retail (RVA 0x1b10a0) fills the two halves from the OfficeXP brush members
// and draws the arrows with CMenuImages::Draw; both are unavailable here.
// Symbol: ?OnDrawSpinButtons@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEAVCMFCSpinButtonCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, void* /*class*/* p4) {}

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

// Retail (RVA 0x1ae260) first calls five CMFCBaseTabCtrl virtuals in a row --
// vtable +0x510, +0x520, +0x560, +0x528 and +0x530 -- and returns without
// painting anything if ANY of them returns non-zero (the five tests at
// 0x1ae29e..0x1ae30c all jump to the epilogue at 0x1ae6ad); only then does it
// paint, from the tab control's own layout fields and the OfficeXP brush
// members.  OpenMFC models neither that vtable shape nor those members, so the
// guard cannot even be evaluated.
// Symbol: ?OnDrawTab@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3, const void* /*class*/* p4) {}

// Retail (RVA 0x1afce0) is
//     if (bIsHilited)
//         this->OnFillHighlightedArea(pDC, rect,
//                 bIsPressed ? &this->[+0x1b0] : &this->[+0x1a0], NULL); // vtable +0x638
//     CSize sizeImage(0, 0);
//     CMenuImages::Draw(pDC, (IMAGES_IDS)5, rect, CMenuImages::ImageBlack, sizeImage);
//     if (bIsHilited)
//         pDC->Draw3dRect(rect, this->[+0x138], this->[+0x138]);
// (the brush is selected by the `sbb/and $0x10/add $0x1a0` sequence at
// 0x1afd1c..0x1afd31; the tab-control pointer and bIsDisabled are not read.)
// +0x138 is XPBorderColor(), but the two highlight brushes are built from
// members +0x118 and +0x11c, which OnUpdateSystemColors (0x1ac0e0) gives two
// DIFFERENT PixelAlpha-derived values on its gradient path (0x1ac4b6/0x1ac4c3
// and 0x1ac505/0x1ac512), so there is no single honest stand-in for the fill;
// and OpenMFC's CMenuImages::Draw is a no-op stub, so the glyph is unavailable.
// Symbol: ?OnDrawTabCloseButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTabCloseButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl__HHH_Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2, int p3, int p4, int p5) {}

// Retail (RVA 0x1afed0) branches on a CMFCBaseTabCtrl virtual (vtable +0x510,
// call at 0x1afef4) and on CMFCButton::m_bHover (+0x114) / m_bClickStarted
// (+0x11c), then draws one Draw3dRect whose two colours are
// afxGlobalData.clrBarDkShadow (+0x6c) and, on two of the three painting paths,
// the OfficeXP member +0x128.  +0x128 is not reducible to one ::GetSysColor
// value -- OnUpdateSystemColors stores clrBtnShadow there on its low-colour
// path (0x1ac821) but CDrawingManager::PixelAlpha(clrBarShadow, 0x6e) on its
// gradient path (0x1ac673/0x1ac67c) -- and OpenMFC has no storage for it, nor a
// modelled +0x510 slot on CMFCBaseTabCtrl.
// Symbol: ?OnDrawTabsButtonBorder@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@AEAVCRect@@PEAVCMFCButton@@IPEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnDrawTabsButtonBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__AEAVCRect__PEAVCMFCButton__IPEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, unsigned int p3, void* /*class*/* p4) {}

// Retail (RVA 0x1b0b80) returns immediately when pTask or pIcons is NULL, then
// reads the task's rect at CMFCTasksPaneTask+0x18 and its separator flag at
// +0x54 to drive CDC::MoveTo/LineTo and the icon blit.  OpenMFC does not lay
// CMFCTasksPaneTask out to those offsets, so the geometry cannot be read.
// Symbol: ?OnDrawTask@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCTasksPaneTask@@PEAVCImageList@@HH@Z
extern "C" void MS_ABI impl__OnDrawTask_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTask__PEAVCImageList__HH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/* p2, int p3, int p4) {}

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
extern "C" void MS_ABI impl__OnDrawTasksGroupAreaBorder_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__HH_Z(void* /*class*/* p0, void* /*class*/ p1, int p2, int p3) {}

// Retail (RVA 0x1b0730) drives everything off CMFCTasksPaneTaskGroup fields --
// +0x08, +0x88, +0x5c, +0x60, +0x64, +0x68 and +0x7c (loads at 0x1b0771..
// 0x1b07ab) -- to decide whether to delegate to the visual-manager virtual at
// vtable +0x2e8, and then paints the caption from the group's own rects.
// OpenMFC does not lay CMFCTasksPaneTaskGroup out to those offsets.
// Symbol: ?OnDrawTasksGroupCaption@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCTasksPaneTaskGroup@@HHH@Z
extern "C" void MS_ABI impl__OnDrawTasksGroupCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCTasksPaneTaskGroup__HHH_Z(void* /*class*/* p0, void* /*class*/* p1, int p2, int p3, int p4) {}

// Retail (RVA 0x1aefd0) fills with the OfficeXP brush +0x180, calls the
// visual-manager virtual at vtable +0x638 with the +0x160 or +0x1a0 brush,
// centres the caption text using CMFCToolBar::GetMenuImageSize (0x155a60),
// selects the font at +0x140, and finally reads the colour members +0x108 /
// +0x118 (the `and $0x10,%edx; mov 0x108(%rdx,%rsi,1),%edx` at 0x1af15c) before
// framing with afxGlobalData.clrBarDkShadow.  None of those members exists in
// OpenMFC (see the file header).
// Symbol: ?OnDrawTearOffCaption@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawTearOffCaption_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Retail (RVA 0x1b1990) picks the fill colour from the button state:
//     m_bHover(+0x114) && m_bClickStarted(+0x11c)  -> this->[+0x11c]
//     exactly one of them set                      -> this->[+0x118]
//     neither                                      -> map the parent window's
//         client rect (GetParent / CWnd::FromHandle / GetClientRect /
//         MapWindowPoints, 0x1b19e3..0x1b1a2f) and delegate to the
//         visual-manager virtual at vtable +0x3c8, painting nothing here.
// The two button flags are pinned by phase4/src/detail/CMFCButtonSupport.h, but
// +0x118 and +0x11c are OfficeXP colour members with no OpenMFC storage that
// OnUpdateSystemColors gives two different PixelAlpha-derived values on its
// gradient path (0x1ac4b6/0x1ac4c3), so neither fill can be reproduced.
// Symbol: ?OnErasePopupWindowButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWndButton@@@Z
extern "C" void MS_ABI impl__OnErasePopupWindowButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCDesktopAlertWndButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {}

// Retail (RVA 0x1ae830) is
//     if (pTabWnd->vtbl[+0x510]())                       // call at 0x1ae852
//         { CMFCVisualManager::OnFillPopupWindowBackground(pDC, rect); return; }  // 0x183c90
//     ::FillRect(pDC->m_hDC, rect,
//                pTabWnd->[+0x1fc] ? afxGlobalData.brBtnFace (+0xa8).m_hObject
//                                  : this->[+0x190].m_hObject);
// OpenMFC models neither the +0x510 vtable slot nor CMFCBaseTabCtrl+0x1fc, and
// the +0x190 brush is CreateSolidBrush of a value OnUpdateSystemColors computes
// differently on each of its two paths (clrBtnFace at 0x1ac7bc on the
// low-colour path, an HLS-converted blend at 0x1ac678 on the gradient path).
// Symbol: ?OnEraseTabsArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, const void* /*class*/* p2) {}

// Retail (RVA 0x1afd90) branches on the same CMFCBaseTabCtrl virtual (+0x510,
// call at 0x1afdbc) as OnEraseTabsArea; on the taken side it fills with the
// OfficeXP brush +0x1a0 or +0x1b0 (chosen from CMFCButton::m_bHover /
// m_bClickStarted) or afxGlobalData.brBarFace and then calls the
// visual-manager virtual at vtable +0x638, and on the other side it fills with
// afxGlobalData.brBtnFace or this->[+0x190].  The +0x510 slot, +0x1fc and the
// OfficeXP brush members are all unmodelled here.
// Symbol: ?OnEraseTabsButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCMFCButton@@PEAVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnEraseTabsButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCMFCButton__PEAVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Retail (RVA 0x1ad730) dispatches on the pane's runtime class: it delegates to
// the visual-manager virtual at vtable +0x48 for a CReBar (or a CReBar parent),
// and otherwise selects one of the OfficeXP brushes +0x160 / +0x180 / +0x190
// after IsDerivedFrom tests against CMFCMenuBar, CMFCOutlookBarPane,
// CMFCColorBar, CMFCToolBar, CAutoHideDockSite and CMFCPopupMenuBar (the
// CRuntimeClass pointers at 0x3aa268 / 0x3aa358 / 0x3aa0e8 / 0x3aa5f8 /
// 0x2d9df8 / 0x3aa478), reading CMFCToolBar+0x1350, +0x1368 and +0x1448 along
// the way, before falling back to CMFCVisualManager::OnFillBarBackground
// (0x182bd0).  Neither those toolbar offsets nor the brush members exist here.
// Symbol: ?OnFillBarBackground@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@2H@Z
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, void* p3, int p4) {}

// Retail (RVA 0x1aded0) returns without painting unless state is 1 or 2, then
// selects one of the OfficeXP brushes +0x160 / +0x180 / +0x1a0 / +0x1b0 /
// +0x1c0 from CMFCToolBar::m_bCustomizeMode (0x3b70bc), a second toolbar global
// (0x3b70cc), the button's m_nStyle bits 0x10000/0x20000/0x40000 and an
// IsKindOf test against CMFCToolBarMenuButton (0x3aa748), and hands the chosen
// brush to the visual-manager virtual at vtable +0x638.  None of those brush
// members exists in OpenMFC.
// Symbol: ?OnFillButtonInterior@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarButton@@VCRect@@W4AFX_BUTTON_STATE@CMFCVisualManager@@@Z
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_CMFCVisualManager___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int /*enum*/ p3, short* p4, int p5, void* p6, void* /*struct*/ p7) {}

// Retail (RVA 0x1b2ef0) delegates to CMFCVisualManager::OnFillCaptionBarButton
// (0x186e20) when pBar->[+0x404] is zero; otherwise it either returns
// (COLORREF)-1, or fills through the visual-manager virtual at vtable +0x638
// with the OfficeXP brush +0x1a0 and returns black/white from a luminance test
// on member +0x118, or fills with the OfficeXP brush +0x180 and returns
// afxGlobalData.clrBarText.  CMFCCaptionBar is not laid out to +0x404 in
// OpenMFC and none of those OfficeXP members exists, so no branch is reachable
// faithfully.
// Symbol: ?OnFillCaptionBarButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCCaptionBar@@VCRect@@HHHHH@Z
extern "C" unsigned long MS_ABI impl__OnFillCaptionBarButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCCaptionBar__VCRect__HHHHH_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, int p3, int p4, int p5, int p6, int p7) {
    return 0;
}

// Retail (RVA 0x1aed30) calls the visual-manager virtual at vtable +0x5c8 and
// CMFCToolBar::GetMenuImageSize (0x155a60) up front.  For bIsSelected it fills
// through the vtable +0x638 virtual with the OfficeXP brush +0x1a0, frames with
// member +0x138 and returns black or white from a luminance test on member
// +0x118; otherwise it fills with the brushes +0x180 and +0x160 and returns
// afxGlobalData.clrBarText.  The +0x160/+0x180 brushes come from members +0x108
// and +0x110, which OnUpdateSystemColors computes as blends on its gradient
// path (0x1ac3ac / 0x1ac289), so they have no single ::GetSysColor stand-in,
// and OpenMFC has no storage for any of them.
// Symbol: ?OnFillCommandsListBackground@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillCommandsListBackground_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {
    return 0;
}

// Symbol: ?OnFillHighlightedArea@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCBrush@@PEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnFillHighlightedArea_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__PEAVCMFCToolBarButton___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, void* /*class*/* p3) {}

// Symbol: ?OnFillMiniFrameCaption@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@VCRect@@PEAVCPaneFrameWnd@@H@Z
extern "C" unsigned long MS_ABI impl__OnFillMiniFrameCaption_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__VCRect__PEAVCPaneFrameWnd__H_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?OnFillPopupWindowBackground@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillPopupWindowBackground_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnFillRibbonButton@CMFCVisualManagerOfficeXP@@MEAAKPEAVCDC@@PEAVCMFCRibbonButton@@@Z
extern "C" unsigned long MS_ABI impl__OnFillRibbonButton_CMFCVisualManagerOfficeXP__MEAAKPEAVCDC__PEAVCMFCRibbonButton___Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnFillRibbonMenuFrame@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCRibbonMainPanel@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillRibbonMenuFrame_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCRibbonMainPanel__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnFillTab@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@PEAVCBrush@@HHPEBVCMFCBaseTabCtrl@@@Z
extern "C" void MS_ABI impl__OnFillTab_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__PEAVCBrush__HHPEBVCMFCBaseTabCtrl___Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2, int p3, int p4, const void* /*class*/* p5) {}

// Symbol: ?OnFillTasksGroupInterior@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@H@Z
extern "C" void MS_ABI impl__OnFillTasksGroupInterior_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect__H_Z(void* /*class*/* p0, void* /*class*/ p1, int p2) {}

// Symbol: ?OnHighlightMenuItem@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@AEAK@Z
extern "C" void MS_ABI impl__OnHighlightMenuItem_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect__AEAK_Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2, unsigned long* p3) {}

// Symbol: ?OnHighlightQuickCustomizeMenuButton@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@PEAVCMFCToolBarMenuButton@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightQuickCustomizeMenuButton_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__PEAVCMFCToolBarMenuButton__VCRect___Z(void* /*class*/* p0, void* /*class*/* p1, void* /*class*/ p2) {}

// Symbol: ?OnHighlightRarelyUsedMenuItems@CMFCVisualManagerOfficeXP@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManagerOfficeXP__MEAAXPEAVCDC__VCRect___Z(void* /*class*/* p0, void* /*class*/ p1) {}

// Symbol: ?OnUpdateSystemColors@CMFCVisualManagerOfficeXP@@MEAAXXZ
extern "C" void MS_ABI impl__OnUpdateSystemColors_CMFCVisualManagerOfficeXP__MEAAXXZ() {}
