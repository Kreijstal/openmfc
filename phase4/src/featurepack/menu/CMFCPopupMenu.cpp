// CMFCPopupMenu — OpenMFC implementation.
// Sources: global_cmfcpopupmenu.cpp, mfccore.cpp, ole_olectors_exports.cpp
//
// Object model note (read before adding a body that touches +0x230..+0x1650,
// +0x16e0 or +0x1858).  Retail CMFCPopupMenu is 0x19c8 bytes and EMBEDS a
// CMFCPopupMenuBar m_wndMenuBar at +0x230 (GetMenuBar(), the non-exported
// vslot-0x3a0 body at RVA 0x34cf0 (mfc140u), is `lea 0x230(%rcx),%rax`), a
// CScrollBar m_wndScrollBarVert at +0x16e0 and a CToolTipCtrl m_wndToolTip at
// +0x1858.  The clean-room class in include/openmfc/afxmfc.h is 0x238 bytes,
// and the exported ctor thunk (featurepack/menu/Thunks.cpp,
// ??0CMFCPopupMenu@@QEAA@XZ) placement-news only that; it neither constructs
// the three embedded objects nor calls Initialize().  The impl__ bodies in
// this file therefore follow the file's standing convention -- they address
// the retail layout on the assumption that the caller's object is
// retail-sized -- but they never dereference the embedded sub-objects (their
// vfptrs are not set); every retail path that needs the bar is left a
// documented stub or marked PARTIAL at the function.

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCPopupMenuSupport.h"
#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <cstdlib>
#include <commctrl.h>
#include <oleacc.h>

// ---------------------------------------------------------------------------
// Retail CMFCPopupMenu member offsets used by the bodies below.
//
// Names follow the on-host MSVC 14.51 afxpopupmenu.h declaration order; every
// offset was read from the retail body that uses it (mfc140u.dll, cited at the
// function).  They agree with the offsets pinned by detail/CMFCPopupMenuSupport.h
// (static_asserts below), but several of that header's field NAMES are
// best-effort guesses that do not match afxpopupmenu.h -- e.g. its
// "m_pMenuBar @0x19b8" is really m_pParentRibbonElement (SetParentRibbonElement,
// RVA 0xbae80 stores its argument there and PostNcDestroy calls vslot 0x340 =
// CMFCRibbonBaseElement::ClosePopupMenu on it), and its "m_pFadeBmp @0x1984" is
// BOOL m_bIsResizable (EnableResize, RVA 0xbb560, stores the result of
// `sizeMinResize != (0,0)` there).  The embedded CMFCPopupMenuBar m_wndMenuBar
// lives at +0x230 (the non-exported GetMenuBar body at RVA 0x34cf0 is
// `lea 0x230(%rcx),%rax; ret`), which is why the 0x230..0x1650 span is opaque.
// ---------------------------------------------------------------------------
namespace {
constexpr int kOffHWnd                    = 0x40;    // CWnd::m_hWnd
constexpr int kOffMessageWnd              = 0x1f8;   // CWnd*  m_pMessageWnd
constexpr int kOffPtLocation              = 0x200;   // CPoint m_ptLocation
constexpr int kOffPtLocationInitial       = 0x208;   // CPoint m_ptLocationInitial (afxpopupmenu.h order; ShowAllCommands copies it into m_ptLocation)
constexpr int kOffFinalSize               = 0x210;   // CSize  m_FinalSize
constexpr int kOffParentBtn               = 0x228;   // CMFCToolBarMenuButton* m_pParentBtn
constexpr int kOffMenuBar                 = 0x230;   // CMFCPopupMenuBar m_wndMenuBar (embedded, 0x1420 bytes)
constexpr int kOffShown                   = 0x1670;  // BOOL m_bShown
constexpr int kOffMaxWidth                = 0x167c;  // int  m_iMaxWidth
constexpr int kOffLogoWidth               = 0x1680;  // int  m_iLogoWidth
constexpr int kOffLogoLocation            = 0x1688;  // LOGO_LOCATION m_nLogoLocation
constexpr int kOffAnimationIsDone         = 0x169c;  // BOOL m_bAnimationIsDone
constexpr int kOffScrollable              = 0x16a4;  // BOOL m_bScrollable
constexpr int kOffRectScrollUp            = 0x16ac;  // CRect m_rectScrollUp
constexpr int kOffRectScrollDn            = 0x16bc;  // CRect m_rectScrollDn
constexpr int kOffScrollMode              = 0x16cc;  // int  m_iScrollMode (-1 up, 0 none, 1 down)
constexpr int kOffScrollBarVert           = 0x16e0;  // CScrollBar m_wndScrollBarVert (embedded)
constexpr int kOffShadowSize              = 0x1810;  // int  m_iShadowSize
constexpr int kOffWndShadow               = 0x1838;  // CMFCShadowWnd* m_pWndShadow
constexpr int kOffTearOffTracking         = 0x1840;  // BOOL m_bTearOffTracking
constexpr int kOffTearOffCaptionActive    = 0x1844;  // BOOL m_bIsTearOffCaptionActive
constexpr int kOffRectTearOffCaption      = 0x1848;  // CRect m_rectTearOffCaption
constexpr int kOffToolTip                 = 0x1858;  // CToolTipCtrl m_wndToolTip (embedded, 0x120 bytes)
constexpr int kOffResizeTracking          = 0x1978;  // BOOL m_bResizeTracking
constexpr int kOffResizeBarOnTop          = 0x1980;  // BOOL m_bIsResizeBarOnTop
constexpr int kOffSizeMinResize           = 0x1988;  // CSize m_sizeMinResize
constexpr int kOffRectResize              = 0x1998;  // CRect m_rectResize
constexpr int kOffQuickCustomize          = 0x19a8;  // BOOL m_bQuickCusomize (retail spelling)
constexpr int kOffParentRibbonElement     = 0x19b8;  // CMFCRibbonBaseElement* m_pParentRibbonElement
constexpr int kOffConnectedFloaty         = 0x19c0;  // HWND m_hwndConnectedFloaty

static_assert(offsetof(PopupMenu, m_hWnd)          == kOffHWnd,                "CWnd::m_hWnd");
static_assert(offsetof(PopupMenu, m_pOwnerWnd)     == kOffMessageWnd,          "m_pMessageWnd");
static_assert(offsetof(PopupMenu, m_pParentButton) == kOffParentBtn,           "m_pParentBtn");
static_assert(offsetof(PopupMenu, m_nFadeX)        == kOffShown,               "m_bShown");
static_assert(offsetof(PopupMenu, m_nFadeStep)     == kOffMaxWidth,            "m_iMaxWidth");
static_assert(offsetof(PopupMenu, m_bShowLogo)     == kOffLogoWidth,           "m_iLogoWidth");
static_assert(offsetof(PopupMenu, m_nLogoLocation) == kOffLogoLocation,        "m_nLogoLocation");
static_assert(offsetof(PopupMenu, m_bMenuSound)    == kOffAnimationIsDone,     "m_bAnimationIsDone");
static_assert(offsetof(PopupMenu, m_rcParent)      == kOffRectScrollUp,        "m_rectScrollUp");
static_assert(offsetof(PopupMenu, m_rcScreen)      == kOffRectScrollDn,        "m_rectScrollDn");
static_assert(offsetof(PopupMenu, m_nLogoHeight)   == kOffScrollMode,          "m_iScrollMode");
static_assert(offsetof(PopupMenu, m_nMenuBorder)   == kOffShadowSize,          "m_iShadowSize");
static_assert(offsetof(PopupMenu, m_pMenuBarFrame) == kOffWndShadow,           "m_pWndShadow");
static_assert(offsetof(PopupMenu, m_pToolTip)      == kOffTearOffTracking,     "m_bTearOffTracking");
static_assert(offsetof(PopupMenu, m_rcLastMenuBar) == kOffRectTearOffCaption,  "m_rectTearOffCaption");
static_assert(offsetof(PopupMenu, m_pDelayedPopup) == kOffResizeTracking,      "m_bResizeTracking");
static_assert(offsetof(PopupMenu, m_bCustomizing)  == kOffResizeBarOnTop,      "m_bIsResizeBarOnTop");
static_assert(offsetof(PopupMenu, m_rcScroll)      == kOffRectResize,          "m_rectResize");
static_assert(offsetof(PopupMenu, m_pMenuBarList)  == kOffQuickCustomize,      "m_bQuickCusomize");
static_assert(offsetof(PopupMenu, m_pMenuBar)      == kOffParentRibbonElement, "m_pParentRibbonElement");
static_assert(offsetof(PopupMenu, m_pParentFloaty) == kOffConnectedFloaty,     "m_hwndConnectedFloaty");
static_assert(sizeof(PopupMenu) == 0x19c8, "retail CMFCPopupMenu is 0x19c8 bytes");

template <class T> inline T& At(void* pThis, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(pThis) + off);
}
template <class T> inline const T& At(const void* pThis, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(pThis) + off);
}
inline HWND HWndOf(const void* pThis) { return At<HWND>(pThis, kOffHWnd); }
inline CWnd* AsWnd(void* pThis) { return static_cast<CWnd*>(static_cast<CMFCPopupMenu*>(pThis)); }
inline CFrameWnd* AsFrame(void* pThis) { return static_cast<CFrameWnd*>(static_cast<CMFCPopupMenu*>(pThis)); }

// CPoint passed BY VALUE is an 8-byte aggregate in a register under the MS x64
// ABI: x in the low dword, y in the high dword.
inline POINT UnpackPoint(long long v) {
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<std::uint32_t>(v & 0xffffffffu));
    pt.y = static_cast<LONG>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(v) >> 32));
    return pt;
}
inline long long PackPoint(POINT pt) {
    return static_cast<long long>((static_cast<std::uint64_t>(static_cast<std::uint32_t>(pt.y)) << 32) |
                                  static_cast<std::uint32_t>(pt.x));
}

// afxGlobalData, as CPaneDivider.cpp reads it: +0x000 is the one-time init gate
// every retail reader tests, +0x170 is HCURSOR m_hcurSizeAll (afxglobals.h
// order: m_hcurStretch +0x158, m_hcurStretchVert +0x160, m_hcurHand +0x168,
// m_hcurSizeAll +0x170).
constexpr int kGlobalDataInitGate  = 0x000;
constexpr int kGlobalDataCurSizeAll = 0x170;
} // namespace

// Class statics with storage in featurepack/menu/StaticData.cpp,
// featurepack/toolbar/StaticData.cpp and core/runtime/StaticData.cpp.  The
// retail addresses are the mfc140u ones read by the bodies below.
extern "C" std::uint32_t impl__m_AnimationSpeed_CMFCPopupMenu__1IA;                 // 0x1803b1b30
extern "C" std::int32_t  impl__m_bForceShadow_CMFCPopupMenu__1HA;                   // 0x1803b1b34
extern "C" void*         impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;         // 0x1803be288
extern "C" void*         impl__m_pCurrent_CMFCRibbonMiniToolBar__1PEAV1_EA;         // 0x1803be2a8
extern "C" std::int32_t  impl__m_bCustomizeMode_CMFCToolBar__1HA;                   // 0x1803be35c
extern "C" std::int32_t  impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;               // 0x1803b1b08
extern "C" std::int32_t  impl__m_bShowAllCommands_CMFCMenuBar__1HA;                // 0x1803be254
extern "C" std::int32_t  impl__m_bRecentlyUsedOld_CMFCCustomizeMenuButton__2HA;     // 0x1803be1d0
extern "C" void*         impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;          // 0x1803be3c0
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];            // 0x1803c1620

// Cross-file thunks.  Each was grepped to its definition; parameter lists are
// derived from the mangled names.
//   core/runtime/CObject.cpp        : IsKindOf
//   core/window/CWnd.cpp            : FromHandle, FromHandlePermanent, ShowWindow, SetWindowPos, OnNotify
//   core/window/Thunks.cpp          : Default, GetExStyle, ClientToScreen(RECT*), GetTopLevelParent
//   core/frame/Thunks.cpp           : CFrameWnd::OnSetCursor / OnSize
//   core/frame/CFrameWnd.cpp        : CFrameWnd::PreTranslateMessage
//   core/frame/CMiniFrameWnd.cpp    : CMiniFrameWnd::OnNcHitTest (still a generated stub returning 0)
//   core/gdi/CPaintDC.cpp           : CPaintDC ctor/dtor
//   core/runtime/AFX_GLOBAL_DATA.cpp: AFX_GLOBAL_DATA::Initialize (empty stub)
//   detail/MfcExceptionsSupport.cpp : AfxThrowInvalidArgException
//   featurepack/CMFC_misc_stubs.cpp : AFXGetParentFrame
//   featurepack/menu/RuntimeClasses.cpp / toolbar/RuntimeClasses.cpp : GetThisClass getters
//   featurepack/ribbon/CMFCRibbonBaseElement.cpp : GetParentWnd (stub, returns NULL)
//   featurepack/visualmanager/CMFCShadowWnd.cpp  : Repos (empty stub)
//   featurepack/CMFC_misc_stubs.cpp : AFXPlaySystemSound (::MessageBeep)
//   featurepack/toolbar/CMFCToolBarMenuButton.cpp : OnCancelMode (empty stub; its
//       definition still carries the auto-generated `()` parameter list -- the
//       declaration below is the one derived from the mangled name)
//   featurepack/ribbon/CMFCRibbonBaseElement.cpp : ClosePopupMenu (documented stub)
//   featurepack/toolbar/RuntimeClasses.cpp : CMFCToolBarMenuButton::GetThisClass
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__OnSetCursor_CFrameWnd__IEAAHPEAVCWnd__II_Z(CFrameWnd* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message);
extern "C" void MS_ABI impl__OnSize_CFrameWnd__IEAAXIHH_Z(CFrameWnd* pThis, unsigned int nType, int cx, int cy);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(CFrameWnd* pThis, MSG* pMsg);
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMiniFrameWnd__QEAA_JVCPoint___Z(void* pThis, long long point);
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetParentWnd_CMFCRibbonBaseElement__UEBAPEAVCWnd__XZ(const void* pElem);
extern "C" void MS_ABI impl__Repos_CMFCShadowWnd__AEAAXXZ(void* pShadow);
extern "C" void MS_ABI impl__AFXPlaySystemSound__YAXH_Z(int nSound);
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(void* pButton);
extern "C" void MS_ABI impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(CMFCRibbonBaseElement* pElem);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();

// Exports of this class defined elsewhere in THIS file and called by the bodies
// below (retail reaches most of them through vtable slots; see each call site).
extern "C" int MS_ABI impl__IsScrollUpAvailable_CMFCPopupMenu__MEAAHXZ(void* pThis);
extern "C" int MS_ABI impl__IsScrollDnAvailable_CMFCPopupMenu__MEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__DoPaint_CMFCPopupMenu__MEAAXPEAVCDC___Z(void* pThis, CDC* pDC);
extern "C" void MS_ABI impl__DrawFade_CMFCPopupMenu__MEAAXPEAVCDC___Z(void* pThis, CDC* pDC);
extern "C" int MS_ABI impl__GetBorderSize_CMFCPopupMenu__MEBAHXZ(const void* pThis);
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(const void* pThis);
extern "C" int MS_ABI impl__TearOff_CMFCPopupMenu__MEAAHVCPoint___Z(void* pThis, long long point);
extern "C" int MS_ABI impl__StartResize_CMFCPopupMenu__IEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__OnVScroll_CMFCPopupMenu__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
extern "C" void MS_ABI impl__OnMouseMove_CMFCPopupMenu__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point);
extern "C" void MS_ABI impl__UpdateShadow_CMFCPopupMenu__QEAAXPEAUtagRECT___Z(void* pThis, RECT* lprectScreen);
extern "C" int MS_ABI impl__InitMenuBar_CMFCPopupMenu__MEAAHXZ(void* pThis);
extern "C" int MS_ABI impl__AdjustScroll_CMFCPopupMenu__IEAAHH_Z(void* pThis, int bForceMenuBarResize);
extern "C" void MS_ABI impl__SetScrollBar_CMFCPopupMenu__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateBottomWindows_CMFCPopupMenu__IEAAXH_Z(void* pThis, int bCheckOnly);

namespace {
// The afxGlobalData init gate, exactly as every retail reader inlines it:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); afxGlobalData.<+0> = 1; }
inline void EnsureGlobalDataInitialized() {
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}
inline HCURSOR GlobalDataCursor(int off) {
    HCURSOR h = nullptr;
    std::memcpy(&h, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof h);
    return h;
}
inline void SetGlobalDataCursor(int off, HCURSOR h) {
    std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, &h, sizeof h);
}

// oleacc!AccessibleObjectFromWindow.  Retail reaches it through a delay-load
// import slot (0x1803e9148); OpenMFC does not link oleacc, so it is resolved
// on first use the same lazy way.
typedef HRESULT (WINAPI *AccessibleObjectFromWindowFn)(HWND, DWORD, REFIID, void**);
inline AccessibleObjectFromWindowFn GetAccessibleObjectFromWindow() {
    static AccessibleObjectFromWindowFn s_fn = nullptr;
    if (!s_fn) {
        HMODULE h = ::LoadLibraryW(L"oleacc.dll");
        if (h) s_fn = reinterpret_cast<AccessibleObjectFromWindowFn>(::GetProcAddress(h, "AccessibleObjectFromWindow"));
    }
    return s_fn;
}
} // namespace

























// Retail (RVA 0xb9820, mfc140u -- an earlier comment here cited "0x9b820",
// a transposition): recomputes the two scroll-arrow rects, not a scroll.
// Takes the client rect, shrinks it by m_iShadowSize (+0x1810) outside
// CMFCToolBar::m_bCustomizeMode, by GetBorderSize() (vslot 0x428) and by the
// logo strip / tear-off caption / resize bar (same geometry as SetScrollBar),
// SetRectEmpty(m_rectScrollUp/Dn) (+0x16ac/+0x16bc), then if m_bScrollable
// (+0x16a4): with m_bShowScrollBar (+0x16a8) reserves SM_CXVSCROLL on the
// right, otherwise carves an arrow strip off the top/bottom for each of
// IsScrollUpAvailable (vslot 0x3d8) / IsScrollDnAvailable (vslot 0x3e0);
// if not scrollable and m_bAnimationIsDone (+0x169c) is FALSE, KillTimer
// 0xec16 and m_iScrollMode = 0.  Unless bForceMenuBarResize is FALSE and
// neither rect changed (EqualRect), the embedded bar (vslot 0x3a0) gets
// CBasePane::SetWindowPos (bar vslot 0x480) with the remaining rect and
// m_nMenuBarHeight (+0x1678) is stored; otherwise only the bar's
// AdjustLayout (bar vslot 0x428).  Returns TRUE (after InvalidateRect of the
// old and new rect + UpdateWindow) when either arrow rect changed.  Needs the
// embedded bar; documented stub returning FALSE.
// TODO(clean-room): not transcribed -- embedded menu bar is not modeled.
// Symbol: ?AdjustScroll@CMFCPopupMenu@@IEAAHH@Z
extern "C" int MS_ABI impl__AdjustScroll_CMFCPopupMenu__IEAAHH_Z(
    void* pThis, int bForceMenuBarResize)
{
    (void)pThis; (void)bForceMenuBarResize;
    return FALSE;
}
// Retail (RVA 0xb9b80, mfc140u): computes the client rect in screen coordinates and
// classifies the point as MENUAREA_OUTSIDE / logo / menu areas using
// m_bShowLogo@0x1680, m_nLogoLocation@0x1688 and PtInRect tests.  The logo
// geometry is not modeled, so 0 is returned conservatively.
// Symbol: ?CheckArea@CMFCPopupMenu@@QEBA?AW4MENUAREA_TYPE@1@AEBVCPoint@@@Z
extern "C" int MS_ABI impl__CheckArea_CMFCPopupMenu__QEBA_AW4MENUAREA_TYPE_1_AEBVCPoint___Z(
    const void* pThis, const CPoint& ptScreen)
{
    (void)ptScreen;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- client-rect classification and
    // the logo-area geometry are not modeled.
    return 0;
}
// Retail (RVA 0xb80f0, mfc140u): walks the menu bar's button list and closes every
// delayed sub-menu through the per-button vslot-0xc0.  The list/vslot are not
// modeled.
// Symbol: ?CollapseSubmenus@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__CollapseSubmenus_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- button-list walk and the
    // vslot-0xc0 close are not modeled.
}
// Retail (RVA 0xba230): news a CMFCToolBar, creates it as a 0x50402808-style
// child and turns it into a tear-off bar.  The toolbar creation chain is not
// modeled, so nullptr is returned.
// Symbol: ?CreateTearOffBar@CMFCPopupMenu@@MEAAPEAVCPane@@PEAVCFrameWnd@@IPEB_W@Z
extern "C" CPane* MS_ABI impl__CreateTearOffBar_CMFCPopupMenu__MEAAPEAVCPane__PEAVCFrameWnd__IPEB_W_Z(
    void* pThis, CFrameWnd* pFrame, unsigned int uiID, const wchar_t* lpszWindowName)
{
    (void)pFrame; (void)uiID; (void)lpszWindowName;
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially -- tear-off bar construction is
    // not modeled.
    return nullptr;
}
// Retail (RVA 0xb8b20): draws the fade animation frames / menu shadow through
// the CDC and CMenuImages machinery.  Not modeled.
// Symbol: ?DoPaint@CMFCPopupMenu@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCPopupMenu__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    (void)pDC;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- drawing is not modeled.
}
// Retail (RVA 0xb91a0): draws the fade-in animation frame list.  Not modeled.
// Symbol: ?DrawFade@CMFCPopupMenu@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawFade_CMFCPopupMenu__MEAAXPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    (void)pDC;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- fade animation is not modeled.
}
// Retail (RVA 0xb97f0):
//     +0x1680 = bShow;
//     +0x1688 = location;
//     return vslot0x300(this, TRUE);   // relayout tail call
// The two stores are reproduced; the vslot-0x300 relayout is not modeled.
// Symbol: ?EnableMenuLogo@CMFCPopupMenu@@QEAAXHW4LOGO_LOCATION@1@@Z
extern "C" void MS_ABI impl__EnableMenuLogo_CMFCPopupMenu__QEAAXHW4LOGO_LOCATION_1__Z(
    void* pThis, int bShow, int location)
{
    if (!pThis) return;
    PopupMenu* p = static_cast<PopupMenu*>(pThis);
    p->m_bShowLogo = bShow;
    p->m_nLogoLocation = location;
    // TODO(clean-room): transcribed partially -- the vslot-0x300 relayout call
    // is not modeled.
}
// Retail (RVA 0xbb990, mfc140u), fully transcribed:
//     if (CMFCRibbonMiniToolBar::m_pCurrent == NULL) return NULL;          // 0x3be2a8
//     for (CMFCPopupMenu* p = m_pActivePopupMenu; p != NULL;              // 0x3be288
//          p = p->GetParentPopupMenu()) {                                  // 0xb7550
//         if (CWnd::FromHandlePermanent(p->m_hWnd) != NULL                 // 0x28adc0
//             && p->m_hwndConnectedFloaty != NULL)                         // +0x19c0
//             return p;
//     }
//     return NULL;
// (Both globals resolve through the mfc140u export table: 0x3be2a8 is
// ?m_pCurrent@CMFCRibbonMiniToolBar@@ and 0x3be288 is
// ?m_pActivePopupMenu@CMFCPopupMenu@@, not a "torn-off menu" pointer.)
// Symbol: ?FindMenuWithConnectedFloaty@CMFCPopupMenu@@KAPEAV1@XZ
extern "C" void* MS_ABI impl__FindMenuWithConnectedFloaty_CMFCPopupMenu__KAPEAV1_XZ()
{
    if (impl__m_pCurrent_CMFCRibbonMiniToolBar__1PEAV1_EA == nullptr) return nullptr;
    for (void* p = impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA; p != nullptr;
         p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(p)) {
        if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWndOf(p)) != nullptr &&
            At<HWND>(p, kOffConnectedFloaty) != nullptr) {
            return p;
        }
    }
    return nullptr;
}
// Retail (RVA 0xbb420, mfc140u), fully decoded:
//     return CMFCVisualManager::GetInstance()->GetPopupMenuBorderSize();
// 0x9774 is the inlined, non-exported GetInstance() (lazily creates the default
// manager -- see toolbar/CMFCToolBar.cpp's DrawSeparator note); vslot 0x5f8 of
// the mfc140u CMFCVisualManager vftable (0x18031c128) is RVA 0x183fb0,
// `mov 0xf4(%rcx),%eax; ret`, i.e. the non-exported inline
// `GetPopupMenuBorderSize() const { return m_nMenuBorderSize; }`.
// OpenMFC: the manager is read from the exported static m_pVisManager (no lazy
// creation, as in DrawSeparator) and m_nMenuBorderSize is read at +0xf4 of it
// directly (the virtual has no export to dispatch to).  Note the retail ctor
// chain stores 2 (base) / 1 (OfficeXP-derived) there while the clean-room
// CMFCVisualManager ctor zero-fills its padding, so the value is 0 until the
// visual-manager owner models that member -- DEVIATION in the data, not here.
// Symbol: ?GetBorderSize@CMFCPopupMenu@@MEBAHXZ
extern "C" int MS_ABI impl__GetBorderSize_CMFCPopupMenu__MEBAHXZ(const void* pThis)
{
    (void)pThis;   // retail never reads `this` either
    const void* pVM = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (!pVM) return 0;   // TODO(clean-room): retail would create the default manager here
    static_assert(sizeof(CMFCVisualManager) > 0xf4, "m_nMenuBorderSize lies inside the clean-room manager");
    return At<int>(pVM, 0xf4);
}
// Retail (RVA 0xbb3a0): when a parent button exists (+0x228), copies the
// button's rect (button+0x68) into rectArea and returns the button's parent
// window (button+0x80 = m_pWndParent); otherwise it falls back to the internal
// menu bar (+0x19b8) through the vslot-0x158 getter and copies its +0xc8 rect.
// The first branch is transcribed; the menu-bar branch needs an unmodeled
// vtable slot.
// Symbol: ?GetParentArea@CMFCPopupMenu@@UEAAPEAVCWnd@@AEAVCRect@@@Z
extern "C" CWnd* MS_ABI impl__GetParentArea_CMFCPopupMenu__UEAAPEAVCWnd__AEAVCRect___Z(
    void* pThis, CRect& rectArea)
{
    if (!pThis) return nullptr;
    PopupMenu* p = static_cast<PopupMenu*>(pThis);
    if (p->m_pParentButton) {
        CWnd* pParentWnd = *reinterpret_cast<CWnd**>(
            static_cast<char*>(p->m_pParentButton) + 0x80);
        if (pParentWnd) {
            rectArea = *reinterpret_cast<CRect*>(
                static_cast<char*>(p->m_pParentButton) + 0x68);
            return pParentWnd;
        }
        return nullptr;
    }
    // TODO(clean-room): transcribed partially -- retail falls back to the
    // internal menu bar (+0x19b8) via the vslot-0x158 getter and copies its
    // +0xc8 rect; not modeled.
    return nullptr;
}
// Retail (RVA 0xb7550, mfc140u), fully transcribed:
//     CWnd* pWnd;
//     if (m_pParentBtn != NULL) {                                          // +0x228
//         pWnd = m_pParentBtn->m_pWndParent;                               // +0x80
//     } else {
//         if (m_pParentRibbonElement == NULL) return NULL;                 // +0x19b8
//         pWnd = m_pParentRibbonElement->GetParentWnd();                   // vslot 0x158 (vtable 0x1802dc5c0 slot 43)
//     }
//     if (pWnd == NULL || !pWnd->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar))) return NULL;   // 0x234cf0, RTC 0x1803b1478 "CMFCPopupMenuBar"
//     CFrameWnd* pFrame = AFXGetParentFrame(pWnd);                         // 0x6bd00
//     if (pFrame == NULL || !pFrame->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))) return NULL;  // RTC 0x1803b1448 "CMFCPopupMenu"
//     return pFrame;
// The ribbon element's GetParentWnd is called through its base thunk
// (non-virtual, DEVIATION for derived overrides; that thunk is itself still a
// stub returning NULL).
// Symbol: ?GetParentPopupMenu@CMFCPopupMenu@@QEBAPEAV1@XZ
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(
    const void* pThis)
{
    if (!pThis) return nullptr;
    CWnd* pWnd = nullptr;
    const CMFCToolBarButton* pBtn = At<const CMFCToolBarButton*>(pThis, kOffParentBtn);
    if (pBtn) {
        pWnd = pBtn->m_pWndParent;
    } else {
        const void* pElem = At<const void*>(pThis, kOffParentRibbonElement);
        if (!pElem) return nullptr;
        pWnd = static_cast<CWnd*>(impl__GetParentWnd_CMFCRibbonBaseElement__UEBAPEAVCWnd__XZ(pElem));
    }
    if (!pWnd) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ()))
        return nullptr;
    CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pWnd);
    if (!pFrame) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pFrame, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ()))
        return nullptr;
    return pFrame;
}
// Retail (RVA 0xb75f0, mfc140u), fully transcribed:
//     if (m_pParentBtn == NULL) return NULL;                               // +0x228
//     CWnd* pWnd = m_pParentBtn->m_pWndParent;                             // +0x80
//     if (pWnd == NULL || !pWnd->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) return NULL;   // 0x234cf0, RTC 0x1803b15f8 "CMFCToolBar"
//     return (CMFCToolBar*)pWnd;
// Symbol: ?GetParentToolBar@CMFCPopupMenu@@QEBAPEAVCMFCToolBar@@XZ
extern "C" CMFCToolBar* MS_ABI impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(
    const void* pThis)
{
    if (!pThis) return nullptr;
    const CMFCToolBarButton* pBtn = At<const CMFCToolBarButton*>(pThis, kOffParentBtn);
    if (!pBtn) return nullptr;
    CWnd* pWnd = pBtn->m_pWndParent;
    if (!pWnd) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ()))
        return nullptr;
    return static_cast<CMFCToolBar*>(pWnd);
}
// Retail (RVA 0xb8320): imports the menu from m_pMenu@+0x1658 into the
// internal CMFCPopupMenuBar, installs the button list and finishes with a
// vslot-0x300 relayout; returns TRUE on success and FALSE on failure.  The
// menu-bar machinery is not modeled, so the retail success value (TRUE) is
// returned.
// Symbol: ?InitMenuBar@CMFCPopupMenu@@MEAAHXZ
extern "C" int MS_ABI impl__InitMenuBar_CMFCPopupMenu__MEAAHXZ(void* pThis)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- menu import into the
    // embedded CMFCPopupMenuBar m_wndMenuBar (+0x230, vslot 0x3a0; +0x19b8 is
    // m_pParentRibbonElement, see the file header) is not modeled; the retail
    // success value is returned.
    return TRUE;
}
// Retail (RVA 0xb4fa0): applies the class-wide defaults.  Every constant store
// below is transcribed 1:1.  The +0x48 flag, the +0x169c/+0x16d0 fields and
// the 0x1803be290 global depend on unmodeled helpers (0x18002e600,
// 0x180009774 + vtable 0x5f8, CMenuImages::Size 0x18008f600) and globals
// (0x1803c1898, 0x1803c187c, 0x1803b1b00, 0x1803c18a8 OS version), so they
// keep their neutral defaults.
// Symbol: ?Initialize@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    char* const b = static_cast<char*>(pThis);

    *reinterpret_cast<std::uint64_t*>(b + 0x1658) = 0;  // menu handle
    *reinterpret_cast<std::uint64_t*>(b + 0x200)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x208)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x228)  = 0;  // parent button
    *reinterpret_cast<std::int32_t*>(b + 0x1f0)   = 1;
    *reinterpret_cast<std::int32_t*>(b + 0x1660)  = 1;
    *reinterpret_cast<std::uint64_t*>(b + 0x210)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x168c) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1678)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x16a0) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x16a8)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1670) = 0;  // fade x/y pair
    *reinterpret_cast<std::int32_t*>(b + 0x16d4)  = -1;
    *reinterpret_cast<std::int32_t*>(b + 0x167c)  = -1;
    *reinterpret_cast<std::int32_t*>(b + 0x1680)  = 0;  // logo show
    *reinterpret_cast<std::int32_t*>(b + 0x1688)  = 0;  // logo location
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x16ac));
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x16bc));
    *reinterpret_cast<std::int32_t*>(b + 0x16cc)  = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1694)  = 1;
    *reinterpret_cast<std::int32_t*>(b + 0x1698)  = 1;
    *reinterpret_cast<std::int32_t*>(b + 0x1810)  = 0;  // menu border size
    *reinterpret_cast<std::int32_t*>(b + 0x16d8)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1840) = 0;
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x1848));
    *reinterpret_cast<std::int32_t*>(b + 0x1650)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1f8)  = 0;  // owner window
    *reinterpret_cast<std::int32_t*>(b + 0x1664)  = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x166c)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x19a8) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x19b0)  = 2;
    *reinterpret_cast<std::uint64_t*>(b + 0x19b8) = 0;  // menu bar
    *reinterpret_cast<std::uint64_t*>(b + 0x19c0) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1684)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1984) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x198c)  = 0;
    ::SetRectEmpty(reinterpret_cast<RECT*>(b + 0x1998));
    *reinterpret_cast<std::uint64_t*>(b + 0x1978) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1980)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1990) = 0;
    *reinterpret_cast<std::int32_t*>(b + 0x1668)  = 0;
    *reinterpret_cast<std::uint64_t*>(b + 0x1838) = 0;

    // TODO(clean-room): transcribed partially -- the +0x48 flag, the
    // +0x169c/+0x16d0 fields and the 0x1803be290 global depend on unmodeled
    // helpers/globals and keep their neutral defaults.
}
// Retail (RVA 0xb8040): asks the menu bar (vslot 0x3a0) for its client rect
// and compares the last item's bottom against the bar height + the fade offset.
// The vslot getter is not modeled, so FALSE is returned conservatively.
// Symbol: ?IsScrollDnAvailable@CMFCPopupMenu@@MEAAHXZ
extern "C" int MS_ABI impl__IsScrollDnAvailable_CMFCPopupMenu__MEAAHXZ(void* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially -- vslot-0x3a0 menu-bar getter
    // and the item-rect comparison are not modeled.
    return FALSE;
}
// Retail (RVA 0xb8010): asks the menu bar (vslot 0x3a0) and returns whether its
// +0x1364 scroll offset is positive.  The vslot getter is not modeled, so FALSE
// is returned conservatively.
// Symbol: ?IsScrollUpAvailable@CMFCPopupMenu@@MEAAHXZ
extern "C" int MS_ABI impl__IsScrollUpAvailable_CMFCPopupMenu__MEAAHXZ(void* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially -- vslot-0x3a0 menu-bar getter
    // and the +0x1364 scroll-offset read are not modeled.
    return FALSE;
}
// Retail (RVA 0xba7f0): when the owner (+0x1f8) is a CDialog/CFrameWnd and the
// global torn-off-menu pointer (0x1803be288) matches `this`, sends the
// notification to the dialog's +0x158 member.  The RTTI classes and the global
// are not modeled, so 0 is returned.
// Symbol: ?NotifyParentDlg@CMFCPopupMenu@@IEAAHH@Z
extern "C" int MS_ABI impl__NotifyParentDlg_CMFCPopupMenu__IEAAHH_Z(
    void* pThis, int bMsgWhat)
{
    (void)bMsgWhat;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the CDialog/CFrameWnd RTTI
    // checks and the 0x1803be288 global compare are not modeled.
    return 0;
}
// Retail (RVA 0xb7b10, mfc140u):
//     if (bActive) return;
//     if (CMFCToolBar::m_bCustomizeMode) return;          // 0x3be35c (NOT m_bForceMenuFocus, which is 0x3b1b28)
//     if (InCommand()) return;                            // vslot 0x3a8: GetMenuBar()->m_bInCommand (bar +0x1384)
//     if (m_bTrackMode) m_bTobeDstroyed = TRUE;           // +0x1664, +0x1674
//     ::PostMessageW(m_hWnd, WM_CLOSE, 0, 0);             // import slot 0x1802c72b0 = PostMessageW
// (Two corrections to the earlier transcription: the global is
// CMFCToolBar::m_bCustomizeMode, and the close is POSTED, not sent.)
// TODO(clean-room): transcribed partially -- the InCommand() test reads the
// embedded menu bar, which is not modeled.
// Symbol: ?OnActivateApp@CMFCPopupMenu@@IEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CMFCPopupMenu__IEAAXHK_Z(
    void* pThis, int bActive, unsigned long dwThreadID)
{
    (void)dwThreadID;
    if (!pThis) return;
    if (bActive != 0) return;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;

    PopupMenu* p = static_cast<PopupMenu*>(pThis);
    if (p->m_bDialogParent != 0)   // +0x1664 = BOOL m_bTrackMode
        p->m_nFadeY = 1;           // +0x1674 = BOOL m_bTobeDstroyed
    ::PostMessageW(p->m_hWnd, WM_CLOSE, 0, 0);
}
// Retail (RVA 0xb9740):
//     if (CFrameWnd::OnCmdMsg(this, nID, nCode, pExtra, pHandlerInfo))
//         return TRUE;
//     pParent = this->+0x1f8;
//     if (pParent) return pParent->vtable[0x28](pParent, nID, nCode, pExtra,
//                                               pHandlerInfo);  // OnCmdMsg slot 5
//     return FALSE;
// Symbol: ?OnCmdMsg@CMFCPopupMenu@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CMFCPopupMenu__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo)
{
    if (!pThis) return 0;

    if (impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            pThis, nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }

    void* pParent = *reinterpret_cast<void**>(static_cast<char*>(pThis) + 0x1f8);
    if (pParent) {
        void** vtbl = *reinterpret_cast<void***>(pParent);
        OnCmdMsgFn fn = reinterpret_cast<OnCmdMsgFn>(vtbl[VT_ONCMDMSG_SLOT]);
        return fn(pParent, nID, nCode, pExtra, pHandlerInfo);
    }
    return FALSE;
}
// Retail (RVA 0xb5790): asserts a non-null create struct, runs
// CFrameWnd::OnCreateHelper and returns -1 when that fails; otherwise it builds
// the embedded CMFCPopupMenuBar m_wndMenuBar (+0x230), transfers the button list and the
// menu style/geometry, and returns 0.  The helper call is modeled; the menu-bar
// construction is not, so 0 (the retail success value) is returned.
// Symbol: ?OnCreate@CMFCPopupMenu@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCPopupMenu__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    if (!pThis) return -1;
    if (!lpCreateStruct) return -1;   // retail asserts here; fail safely

    int nRet = impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
        reinterpret_cast<CFrameWnd*>(pThis), lpCreateStruct, nullptr);
    if (nRet == -1) return -1;

    // TODO(clean-room): transcribed partially -- the internal CMFCPopupMenuBar
    // construction, button-list transfer and geometry setup are not modeled.
    return 0;
}
// Retail (RVA 0xb6b60): collapses open sub-menus, walks the window's frame
// chain to detach the owner-dialog hook, releases the menu-bar drag button and
// clears the internal bar references.  The frame/list machinery is not modeled.
// Symbol: ?OnDestroy@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- sub-menu collapse, the
    // owner-dialog detach and the menu-bar cleanup are not modeled.
}
// Symbol: ?ActivatePopupMenu@CMFCPopupMenu@@SAHPEAVCFrameWnd@@PEAV1@@Z
extern "C" int MS_ABI impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(CFrameWnd* pTopFrame, CMFCPopupMenu* pPopupMenu) {
    return CMFCPopupMenu::ActivatePopupMenu(pTopFrame, pPopupMenu);
}
// Symbol: ?GetSafeActivePopupMenu@CMFCPopupMenu@@SAPEAV1@XZ
extern "C" CMFCPopupMenu* MS_ABI impl__GetSafeActivePopupMenu_CMFCPopupMenu__SAPEAV1_XZ() {
    return CMFCPopupMenu::GetSafeActivePopupMenu();
}
// Symbol: ?GetMenuItemCount@CMFCPopupMenu@@QEBAHXZ
extern "C" int MS_ABI impl__GetMenuItemCount_CMFCPopupMenu__QEBAHXZ(const CMFCPopupMenu* pThis) {
    return pThis ? pThis->GetMenuItemCount() : 0;
}
// Symbol: ?GetMenuItem@CMFCPopupMenu@@QEBAPEAVCMFCToolBarMenuButton@@H@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetMenuItem_CMFCPopupMenu__QEBAPEAVCMFCToolBarMenuButton__H_Z(const CMFCPopupMenu* pThis, int nIndex) {
    return pThis ? pThis->GetMenuItem(nIndex) : nullptr;
}
// Symbol: ?GetSelItem@CMFCPopupMenu@@QEAAPEAVCMFCToolBarMenuButton@@XZ
extern "C" CMFCToolBarMenuButton* MS_ABI impl__GetSelItem_CMFCPopupMenu__QEAAPEAVCMFCToolBarMenuButton__XZ(CMFCPopupMenu* pThis) {
    return pThis ? pThis->GetSelItem() : nullptr;
}
// Symbol: ?FindSubItemByCommand@CMFCPopupMenu@@QEBAPEAVCMFCToolBarMenuButton@@I@Z
extern "C" CMFCToolBarMenuButton* MS_ABI impl__FindSubItemByCommand_CMFCPopupMenu__QEBAPEAVCMFCToolBarMenuButton__I_Z(const CMFCPopupMenu* pThis, unsigned int uiCmd) {
    return pThis ? pThis->FindSubItemByCommand(uiCmd) : nullptr;
}
// Symbol: ?InsertItem@CMFCPopupMenu@@QEAAHAEBVCMFCToolBarMenuButton@@H@Z
extern "C" int MS_ABI impl__InsertItem_CMFCPopupMenu__QEAAHAEBVCMFCToolBarMenuButton__H_Z(CMFCPopupMenu* pThis, const CMFCToolBarMenuButton* pButton, int iInsertAt) {
    return (pThis && pButton) ? pThis->InsertItem(*pButton, iInsertAt) : FALSE;
}
// Symbol: ?InsertSeparator@CMFCPopupMenu@@QEAAHH@Z
extern "C" int MS_ABI impl__InsertSeparator_CMFCPopupMenu__QEAAHH_Z(CMFCPopupMenu* pThis, int iInsertAt) {
    return pThis ? pThis->InsertSeparator(iInsertAt) : FALSE;
}
// Symbol: ?CloseMenu@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__CloseMenu_CMFCPopupMenu__QEAAXH_Z(CMFCPopupMenu* pThis, int bSetFocusToBar) {
    if (pThis) pThis->CloseMenu(bSetFocusToBar);
}
// Symbol: ?EnableResize@CMFCPopupMenu@@QEAAXVCSize@@@Z
extern "C" void MS_ABI impl__EnableResize_CMFCPopupMenu__QEAAXVCSize___Z(CMFCPopupMenu* pThis, CSize sizeMinResize) {
    if (pThis) pThis->EnableResize(sizeMinResize);
}
// Symbol: ?EnableVertResize@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableVertResize_CMFCPopupMenu__QEAAXH_Z(CMFCPopupMenu* pThis, int bEnable) {
    if (pThis) pThis->EnableVertResize(bEnable);
}
// Symbol: ?HideRarelyUsedCommands@CMFCPopupMenu@@QEBAHXZ
extern "C" int MS_ABI impl__HideRarelyUsedCommands_CMFCPopupMenu__QEBAHXZ(const CMFCPopupMenu* pThis) {
    return pThis ? pThis->HideRarelyUsedCommands() : FALSE;
}
CMFCPopupMenu::CMFCPopupMenu() {
    memset(_mfcpopupmenu_padding, 0, sizeof(_mfcpopupmenu_padding));
}
CMFCPopupMenu::~CMFCPopupMenu() {
    if (g_pActivePopupMenu == this) {
        g_pActivePopupMenu = nullptr;
    }
    ClearPopupMenuState(this);
}
CMFCPopupMenu* CMFCPopupMenu::GetActiveMenu() { return g_pActivePopupMenu; }
CMFCPopupMenu* CMFCPopupMenu::GetSafeActivePopupMenu() { return g_pActivePopupMenu; }
void CMFCPopupMenu::SetForceMenuFocus(BOOL bForceFocus) { g_forceMenuFocus = bForceFocus; }
BOOL CMFCPopupMenu::ActivatePopupMenu(CFrameWnd*, CMFCPopupMenu* pPopupMenu) {
    g_pActivePopupMenu = pPopupMenu;
    return pPopupMenu != nullptr;
}
BOOL CMFCPopupMenu::Create(CWnd* pParentWnd, int x, int y, HMENU hMenu, BOOL bLocked, BOOL bIsMainMenu) {
    PopupMenuState& state = g_popupMenuStates[this];
    state.parent = pParentWnd;
    state.x = x;
    state.y = y;
    state.menu = hMenu;
    state.locked = bLocked != FALSE;
    state.mainMenu = bIsMainMenu != FALSE;
    state.selected = -1;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    g_pActivePopupMenu = this;
    return TRUE;
}
void CMFCPopupMenu::CloseMenu(BOOL) {
    if (g_pActivePopupMenu == this) {
        g_pActivePopupMenu = nullptr;
    }
}
int CMFCPopupMenu::GetMenuItemCount() const {
    auto it = g_popupMenuStates.find(this);
    return it == g_popupMenuStates.end() ? 0 : static_cast<int>(it->second.items.size());
}
CMFCToolBarMenuButton* CMFCPopupMenu::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
CMFCToolBarMenuButton* CMFCPopupMenu::GetSelItem() {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    return GetMenuItem(it->second.selected);
}
CMFCToolBarMenuButton* CMFCPopupMenu::FindSubItemByCommand(UINT uiCmd) const {
    auto it = g_popupMenuStates.find(this);
    if (it == g_popupMenuStates.end()) return nullptr;
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (item && item->m_nID == uiCmd) return item;
    }
    return nullptr;
}
BOOL CMFCPopupMenu::InsertItem(const CMFCToolBarMenuButton& button, int iInsertAt) {
    PopupMenuState& state = g_popupMenuStates[this];
    CMFCToolBarMenuButton* copy = new CMFCToolBarMenuButton(button);
    if (iInsertAt < 0 || iInsertAt >= static_cast<int>(state.items.size())) {
        state.items.push_back(copy);
    } else {
        state.items.insert(state.items.begin() + iInsertAt, copy);
    }
    state.ownedItems.insert(copy);
    return TRUE;
}
BOOL CMFCPopupMenu::InsertSeparator(int iInsertAt) {
    CMFCToolBarMenuButton separator(0, nullptr, FALSE, nullptr, FALSE);
    return InsertItem(separator, iInsertAt);
}
void CMFCPopupMenu::EnableResize(CSize sizeMinResize) {
    g_popupMenuStates[this].minResize = sizeMinResize;
}
void CMFCPopupMenu::EnableVertResize(BOOL bEnable) {
    g_popupMenuStates[this].vertResize = bEnable != FALSE;
}
BOOL CMFCPopupMenu::HideRarelyUsedCommands() const {
    return FALSE;
}
// Symbol: ??0CMFCPopupMenu@@IEAA@PEAVCMFCToolBarsMenuPropertyPage@@PEB_W@Z
// CMFCPopupMenu::CMFCPopupMenu(CMFCToolBarsMenuPropertyPage*, const wchar_t*)
extern "C" void* MS_ABI impl___0CMFCPopupMenu__IEAA_PEAVCMFCToolBarsMenuPropertyPage__PEB_W_Z(
    void* pPage, const wchar_t* pName) {
    (void)pPage;
    (void)pName;
    return new CMFCPopupMenu();
}
// The six IAccessible getters below share one retail shape: a NULL out-param is
// E_INVALIDARG; the CHILDID_SELF case (varChild.vt == VT_I4 && lVal == 0) is
// answered by the popup itself; everything else is forwarded to the embedded
// CMFCPopupMenuBar (GetMenuBar(), vslot 0x3a0, retail body `lea 0x230(%rcx)`)
// through the bar's own IAccessible vslot.  OpenMFC never constructs that
// embedded bar (see the ctor note at the top of this file), so the forwarded
// cases take retail's "no bar" edge instead: they return S_FALSE.  That edge
// is dead in retail (the base GetMenuBar never returns NULL) -- it is a
// DEVIATION, not a transcription of live behaviour.

// Retail (RVA 0xbbd00, mfc140u):
//     if (!ppdispChild) return E_INVALIDARG;
//     if (varChild.vt == VT_I4 && varChild.lVal != 0) {
//         if (CMFCPopupMenuBar* pBar = GetMenuBar())               // vslot 0x3a0
//             return pBar->get_accChild(varChild, ppdispChild);    // vslot 0x150 = CMFCToolBar::get_accChild
//     }
//     *ppdispChild = NULL;
//     return S_FALSE;
// Symbol: ?get_accChild@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCPopupMenu__MEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    void* pThis, VARIANT varChild, IDispatch** ppdispChild)
{
    (void)pThis;
    if (!ppdispChild) return E_INVALIDARG;
    // DEVIATION: the child case is forwarded to the embedded bar in retail;
    // the bar is not modeled, so it falls through to the S_FALSE edge.
    (void)varChild;
    *ppdispChild = nullptr;
    return S_FALSE;
}

// Retail (RVA 0xbbca0, mfc140u):
//     if (!pcountChildren) return E_INVALIDARG;
//     if (CMFCPopupMenuBar* pBar = GetMenuBar())                   // vslot 0x3a0
//         return pBar->get_accChildCount(pcountChildren);          // vslot 0x148 = CMFCToolBar::get_accChildCount
//     *pcountChildren = 0;
//     return S_FALSE;
// CMFCToolBar::get_accChildCount (RVA 0x15a320) is `*p = AccGetButtonsCount();
// return S_OK`.  OpenMFC keeps this popup's item list in the mfccore side table
// that InsertItem/GetMenuItemCount in this file maintain, so the count is taken
// from there (DEVIATION from the retail dispatch, same quantity).
// Symbol: ?get_accChildCount@CMFCPopupMenu@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCPopupMenu__MEAAJPEAJ_Z(void* pThis, long* pcountChildren)
{
    if (!pcountChildren) return E_INVALIDARG;
    if (!pThis) { *pcountChildren = 0; return S_FALSE; }
    *pcountChildren = static_cast<const CMFCPopupMenu*>(pThis)->GetMenuItemCount();
    return S_OK;
}

// Retail (RVA 0xbbb00, mfc140u):
//     if (!pszName) return E_INVALIDARG;
//     if (varChild.vt == VT_I4 && varChild.lVal == CHILDID_SELF && m_pParentBtn != NULL) {
//         CString strText = m_pParentBtn->m_strText;      // +0x38, CSimpleStringT::CloneData (0xdd40)
//         strText.Remove(_T('&'));                         // 0x12370
//         if (strText.GetLength() == 0) return S_FALSE;    // nDataLength at pszData-0x10
//         *pszName = ::SysAllocString(strText);            // OLEAUT32 #4
//         if (*pszName == NULL) CSimpleStringT::ThrowMemoryException();   // 0x3160
//         return S_OK;
//     }
//     if (CMFCPopupMenuBar* pBar = GetMenuBar())           // vslot 0x3a0
//         return pBar->get_accName(varChild, pszName);     // vslot 0x158 = CBasePane::get_accName
//     return S_FALSE;
// The '&' stripping is done on a local copy of the button's text instead of
// through CString::Remove so no reference count is touched on a string that a
// client-side CStringT may own; the result is the same.
// Symbol: ?get_accName@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CMFCPopupMenu__MEAAJUtagVARIANT__PEAPEA_W_Z(
    void* pThis, VARIANT varChild, wchar_t** pszName)
{
    if (!pszName) return E_INVALIDARG;
    if (!pThis) return S_FALSE;
    if (varChild.vt == VT_I4 && varChild.lVal == 0 /*CHILDID_SELF*/) {
        const CMFCToolBarButton* pBtn = At<const CMFCToolBarButton*>(pThis, kOffParentBtn);
        if (pBtn) {
            static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "m_strText @0x38");
            const wchar_t* pszText = pBtn->m_strText.GetString();
            const size_t nLen = pszText ? std::wcslen(pszText) : 0;
            wchar_t* pszCopy = static_cast<wchar_t*>(std::malloc((nLen + 1) * sizeof(wchar_t)));
            if (!pszCopy) return E_OUTOFMEMORY;   // DEVIATION: retail throws CMemoryException
            size_t j = 0;
            for (size_t i = 0; i < nLen; ++i) {
                if (pszText[i] != L'&') pszCopy[j++] = pszText[i];
            }
            pszCopy[j] = L'\0';
            long hr;
            if (j == 0) {
                hr = S_FALSE;
            } else {
                *pszName = ::SysAllocString(pszCopy);
                hr = *pszName ? S_OK : E_OUTOFMEMORY;   // DEVIATION: retail throws CMemoryException
            }
            std::free(pszCopy);
            return hr;
        }
    }
    // DEVIATION: retail forwards to the embedded bar's get_accName here.
    return S_FALSE;
}

// Retail (RVA 0xbbc20, mfc140u), fully transcribed:
//     if (!ppdispParent) return E_INVALIDARG;
//     *ppdispParent = NULL;
//     if (m_pParentBtn != NULL && m_pParentBtn->m_pWndParent != NULL         // +0x228, +0x80
//         && m_pParentBtn->m_pWndParent->IsKindOf(RUNTIME_CLASS(CMFCToolBar))  // 0x234cf0 / RTC 0x1803b15f8 "CMFCToolBar"
//         && m_pParentBtn->m_pWndParent->m_hWnd != NULL)
//         return ::AccessibleObjectFromWindow(hWnd, OBJID_CLIENT /*0xfffffffc*/,
//                    IID_IAccessible /*0x1802d9828*/, (void**)ppdispParent);  // delay-load slot 0x1803e9148
//     return S_FALSE;
// Symbol: ?get_accParent@CMFCPopupMenu@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCPopupMenu__MEAAJPEAPEAUIDispatch___Z(void* pThis, IDispatch** ppdispParent)
{
    if (!ppdispParent) return E_INVALIDARG;
    *ppdispParent = nullptr;
    if (!pThis) return S_FALSE;
    const CMFCToolBarButton* pBtn = At<const CMFCToolBarButton*>(pThis, kOffParentBtn);
    if (!pBtn) return S_FALSE;
    static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "m_pWndParent @0x80");
    CWnd* pParent = pBtn->m_pWndParent;
    if (!pParent) return S_FALSE;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ()))
        return S_FALSE;
    HWND hWnd = pParent->m_hWnd;
    if (!hWnd) return S_FALSE;
    // IID_IAccessible = {618736E0-3C3D-11CF-810C-00AA00389B71}, read from the
    // 16 bytes at 0x1802d9828 in mfc140u.
    static const GUID kIidIAccessible = { 0x618736E0, 0x3C3D, 0x11CF, { 0x81, 0x0C, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
    AccessibleObjectFromWindowFn fn = GetAccessibleObjectFromWindow();
    if (!fn) return S_FALSE;   // DEVIATION: a missing oleacc.dll would fault retail's delay-load
    return fn(hWnd, static_cast<DWORD>(OBJID_CLIENT), kIidIAccessible, reinterpret_cast<void**>(ppdispParent));
}

// Retail (RVA 0xbbd90, mfc140u):
//     if (!pvarRole) return E_INVALIDARG;
//     if (varChild.vt == VT_I4 && varChild.lVal == CHILDID_SELF) {
//         pvarRole->vt = VT_I4; pvarRole->lVal = ROLE_SYSTEM_MENUPOPUP /*0xb*/; return S_OK;
//     }
//     if (CMFCPopupMenuBar* pBar = GetMenuBar()) return pBar->get_accRole(varChild, pvarRole);  // vslot 0x170
//     return S_FALSE;
// Symbol: ?get_accRole@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCPopupMenu__MEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, VARIANT varChild, VARIANT* pvarRole)
{
    (void)pThis;
    if (!pvarRole) return E_INVALIDARG;
    if (varChild.vt == VT_I4 && varChild.lVal == 0 /*CHILDID_SELF*/) {
        pvarRole->vt = VT_I4;
        pvarRole->lVal = 0xb;   // ROLE_SYSTEM_MENUPOPUP
        return S_OK;
    }
    // DEVIATION: retail forwards to the embedded bar's get_accRole here.
    return S_FALSE;
}

// Retail (RVA 0xbbe30, mfc140u): same shape as get_accRole with
//     pvarState->lVal = STATE_SYSTEM_FOCUSED | STATE_SYSTEM_FOCUSABLE /*0x100004*/
// and the bar forward through vslot 0x178 (CMFCPopupMenuBar::get_accState).
// Symbol: ?get_accState@CMFCPopupMenu@@MEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCPopupMenu__MEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, VARIANT varChild, VARIANT* pvarState)
{
    (void)pThis;
    if (!pvarState) return E_INVALIDARG;
    if (varChild.vt == VT_I4 && varChild.lVal == 0 /*CHILDID_SELF*/) {
        pvarState->vt = VT_I4;
        pvarState->lVal = 0x100004;   // STATE_SYSTEM_FOCUSED | STATE_SYSTEM_FOCUSABLE
        return S_OK;
    }
    // DEVIATION: retail forwards to the embedded bar's get_accState here.
    return S_FALSE;
}

// Retail: the WM_ERASEBKGND entry of the CMFCPopupMenu message map
// (0x1802f8028, entry msg=0x14 sig=1) is RVA 0x3a60 (mfc140u), a two-instruction
// body `mov $1,%eax; ret` shared by identical-code folding with several other
// `return 1` exports.  That is the whole handler: erase nothing, report TRUE.
// Symbol: ?OnEraseBkgnd@CMFCPopupMenu@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCPopupMenu__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC)
{
    (void)pThis; (void)pDC;
    return TRUE;
}

// Retail (RVA 0xb70b0..0xb7550, mfc140u, 293 instructions): the keyboard
// navigation handler.  Verified call/data set: CMFCToolBar::m_bCustomizeMode
// (0x3be35c), CMFCPopupMenu::m_AnimationType (0x3be290), the embedded bar
// (vslot 0x3a0) and its GetRowHeight (vslot 0x6b8) / AdjustLocations (vslot
// 0x7d8), GetSelItem (0xb7630), CMFCToolBar::GetButton (0x14fe00), IsKindOf
// (0x234cf0) against RTC 0x1803b1478 (CMFCPopupMenuBar), AdjustScroll
// (0xb9820) plus SetScrollPos on m_wndScrollBarVert, GetParentToolBar
// (0xb75f0), GetParentPopupMenu (0xb7550), GetExStyle (0x2a96c0), CloseMenu
// (0xb7680), ShowAllCommands (0xb8150), a recursive OnKeyDown, GetAsyncKeyState,
// SendMessageW, GetClientRect and CWnd::Default (0x28ac80) for unhandled keys.
// Every branch selects or scrolls buttons of the embedded CMFCPopupMenuBar,
// which OpenMFC does not construct, so this stays a documented stub.
// TODO(clean-room): not transcribed -- embedded menu bar is not modeled.
// Symbol: ?OnKeyDown@CMFCPopupMenu@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCPopupMenu__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags)
{
    (void)pThis; (void)nChar; (void)nRepCnt; (void)nFlags;
}

// Retail (RVA 0xb9d00, mfc140u), fully transcribed:
//     if (CMFCToolBar::m_bCustomizeMode == 0                       // 0x3be35c
//         && ::PtInRect(&m_rectTearOffCaption, point)) {           // +0x1848
//         m_bIsTearOffCaptionActive = TRUE;                        // +0x1844
//         ::InvalidateRect(m_hWnd, &m_rectTearOffCaption, TRUE);
//         m_bTearOffTracking = TRUE;                               // +0x1840
//         CWnd::FromHandle(::SetCapture(m_hWnd));                  // CWnd::SetCapture(), result dropped
//         return;
//     }
//     CWnd::Default();                                             // 0x28ac80
// Symbol: ?OnLButtonDown@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCPopupMenu__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point)
{
    (void)nFlags;
    if (!pThis) return;
    const POINT pt = UnpackPoint(point);
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 &&
        ::PtInRect(&At<RECT>(pThis, kOffRectTearOffCaption), pt)) {
        At<int>(pThis, kOffTearOffCaptionActive) = TRUE;
        ::InvalidateRect(HWndOf(pThis), &At<RECT>(pThis, kOffRectTearOffCaption), TRUE);
        At<int>(pThis, kOffTearOffTracking) = TRUE;
        impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(HWndOf(pThis)));
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail (RVA 0xb9d80, mfc140u), fully transcribed:
//     if (m_bTearOffTracking) {                                    // +0x1840
//         m_bTearOffTracking = FALSE;
//         ::ReleaseCapture();
//         m_bIsTearOffCaptionActive = FALSE;                       // +0x1844
//         ::InvalidateRect(m_hWnd, &m_rectTearOffCaption, TRUE);   // +0x1848
//     }
//     CWnd::Default();                                             // tail-jump 0x28ac80
// Symbol: ?OnLButtonUp@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCPopupMenu__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point)
{
    (void)nFlags; (void)point;
    if (!pThis) return;
    if (At<int>(pThis, kOffTearOffTracking) != 0) {
        At<int>(pThis, kOffTearOffTracking) = FALSE;
        ::ReleaseCapture();
        At<int>(pThis, kOffTearOffCaptionActive) = FALSE;
        ::InvalidateRect(HWndOf(pThis), &At<RECT>(pThis, kOffRectTearOffCaption), TRUE);
    }
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail: the WM_MOUSEACTIVATE entry of the message map (msg=0x21 sig=12) is
// RVA 0x5da20 (mfc140u), `mov $3,%eax; ret` (folded with other `return 3`
// exports): the popup never activates -- MA_NOACTIVATE.
// Symbol: ?OnMouseActivate@CMFCPopupMenu@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMFCPopupMenu__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message)
{
    (void)pThis; (void)pDesktopWnd; (void)nHitTest; (void)message;
    return MA_NOACTIVATE;
}

// Retail (RVA 0xb7ec0, mfc140u), fully transcribed:
//     if (m_bTearOffTracking) {                                    // +0x1840
//         if (!::PtInRect(&m_rectTearOffCaption, point)) {         // +0x1848
//             ::ReleaseCapture();
//             m_bTearOffTracking = FALSE;
//             TearOff(point);                                      // vslot 0x400
//         }
//         return;
//     }
//     CWnd::Default();                                             // 0x28ac80
//     if (!m_bScrollable) return;                                  // +0x16a4
//     if (m_iScrollMode != 0) return;                              // +0x16cc
//     if (::PtInRect(&m_rectScrollUp, point) && IsScrollUpAvailable()) {      // +0x16ac, vslot 0x3d8
//         m_iScrollMode = -1; ::InvalidateRect(m_hWnd, &m_rectScrollUp, TRUE);
//     } else if (::PtInRect(&m_rectScrollDn, point) && IsScrollDnAvailable()) { // +0x16bc, vslot 0x3e0
//         m_iScrollMode = 1;  ::InvalidateRect(m_hWnd, &m_rectScrollDn, TRUE);
//     } else {
//         m_iScrollMode = 0;
//     }
//     if (m_iScrollMode != 0) ::SetTimer(m_hWnd, 0xec16, 80, NULL);
// The three virtuals are reached through this file's own thunks (non-virtual);
// a derived class override would not be honoured -- DEVIATION.
// Symbol: ?OnMouseMove@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCPopupMenu__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point)
{
    (void)nFlags;
    if (!pThis) return;
    const POINT pt = UnpackPoint(point);
    if (At<int>(pThis, kOffTearOffTracking) != 0) {
        if (!::PtInRect(&At<RECT>(pThis, kOffRectTearOffCaption), pt)) {
            ::ReleaseCapture();
            At<int>(pThis, kOffTearOffTracking) = FALSE;
            impl__TearOff_CMFCPopupMenu__MEAAHVCPoint___Z(pThis, point);
        }
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    if (At<int>(pThis, kOffScrollable) == 0) return;
    if (At<int>(pThis, kOffScrollMode) != 0) return;

    if (::PtInRect(&At<RECT>(pThis, kOffRectScrollUp), pt) &&
        impl__IsScrollUpAvailable_CMFCPopupMenu__MEAAHXZ(pThis)) {
        At<int>(pThis, kOffScrollMode) = -1;
        ::InvalidateRect(HWndOf(pThis), &At<RECT>(pThis, kOffRectScrollUp), TRUE);
    } else if (::PtInRect(&At<RECT>(pThis, kOffRectScrollDn), pt) &&
               impl__IsScrollDnAvailable_CMFCPopupMenu__MEAAHXZ(pThis)) {
        At<int>(pThis, kOffScrollMode) = 1;
        ::InvalidateRect(HWndOf(pThis), &At<RECT>(pThis, kOffRectScrollDn), TRUE);
    } else {
        At<int>(pThis, kOffScrollMode) = 0;
    }
    if (At<int>(pThis, kOffScrollMode) != 0) {
        ::SetTimer(HWndOf(pThis), 0xec16, 80, nullptr);
    }
}

// Retail (RVA 0xbb320, mfc140u), fully transcribed:
//     if (m_pActivePopupMenu == this && m_bScrollable) {           // 0x3be288, +0x16a4
//         int nSteps = abs(zDelta) / 120;                          // WHEEL_DELTA (magic 0x88888889 >> 6)
//         for (int i = 0; i < nSteps; i++)
//             OnVScroll(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, &m_wndScrollBarVert);   // 0xbb160, +0x16e0
//     }
//     return TRUE;
// (nFlags and pt are never read.)  OnVScroll is this file's thunk, which is
// still a documented stub, so the loop currently has no effect.
// Symbol: ?OnMouseWheel@CMFCPopupMenu@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CMFCPopupMenu__IEAAHIFVCPoint___Z(
    void* pThis, unsigned int nFlags, short zDelta, long long pt)
{
    (void)nFlags; (void)pt;
    if (!pThis) return TRUE;
    if (impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == pThis &&
        At<int>(pThis, kOffScrollable) != 0) {
        const int nSteps = (zDelta < 0 ? -static_cast<int>(zDelta) : static_cast<int>(zDelta)) / 120;
        CScrollBar* pBar = reinterpret_cast<CScrollBar*>(static_cast<char*>(pThis) + kOffScrollBarVert);
        for (int i = 0; i < nSteps; ++i) {
            impl__OnVScroll_CMFCPopupMenu__IEAAXIIPEAVCScrollBar___Z(
                pThis, zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, pBar);
        }
    }
    return TRUE;
}

// Retail (RVA 0xbb450, mfc140u), fully transcribed:
//     DWORD dwExStyle = GetExStyle();                              // 0x2a96c0
//     if (!::IsRectEmpty(&m_rectResize)) {                         // +0x1998
//         CRect rect = m_rectResize;
//         if (m_sizeMinResize.cx > 0) {                            // +0x1988
//             rect.left = rect.right - rect.Height();              // the square grip at the right end
//             ClientToScreen(&rect);                               // 0x2a3310
//             if (::PtInRect(&rect, point)) {
//                 BOOL bRTL = dwExStyle & WS_EX_LAYOUTRTL;
//                 if (m_bIsResizeBarOnTop) return bRTL ? HTTOPLEFT : HTTOPRIGHT;        // +0x1980
//                 return bRTL ? HTBOTTOMLEFT : HTBOTTOMRIGHT;
//             }
//         } else {
//             ClientToScreen(&rect);
//             if (::PtInRect(&rect, point)) return m_bIsResizeBarOnTop ? HTTOP : HTBOTTOM;
//         }
//     }
//     return CMiniFrameWnd::OnNcHitTest(point);                    // 0x2a9030
// The CMiniFrameWnd::OnNcHitTest thunk (core/frame/CMiniFrameWnd.cpp) is still
// a generated stub returning 0 (HTNOWHERE), so the fallback edge is degraded.
// Symbol: ?OnNcHitTest@CMFCPopupMenu@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCPopupMenu__IEAA_JVCPoint___Z(void* pThis, long long point)
{
    if (!pThis) return HTNOWHERE;
    const POINT pt = UnpackPoint(point);
    const unsigned long dwExStyle = impl__GetExStyle_CWnd__QEBAKXZ(AsWnd(pThis));
    if (!::IsRectEmpty(&At<RECT>(pThis, kOffRectResize))) {
        RECT rect = At<RECT>(pThis, kOffRectResize);
        if (At<SIZE>(pThis, kOffSizeMinResize).cx > 0) {
            rect.left = rect.right - (rect.bottom - rect.top);
            impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rect);
            if (::PtInRect(&rect, pt)) {
                const bool bRTL = (dwExStyle & WS_EX_LAYOUTRTL) != 0;
                if (At<int>(pThis, kOffResizeBarOnTop) != 0) return bRTL ? HTTOPLEFT : HTTOPRIGHT;
                return bRTL ? HTBOTTOMLEFT : HTBOTTOMRIGHT;
            }
        } else {
            impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rect);
            if (::PtInRect(&rect, pt)) {
                return At<int>(pThis, kOffResizeBarOnTop) != 0 ? HTTOP : HTBOTTOM;
            }
        }
    }
    return impl__OnNcHitTest_CMiniFrameWnd__QEAA_JVCPoint___Z(pThis, point);
}

// Retail (RVA 0xbb540, mfc140u), fully transcribed:
//     UINT base = m_bIsResizeBarOnTop ? HTTOP /*12*/ : HTBOTTOM /*15*/;   // +0x1980
//     if ((UINT)(nHitTest - base) <= 2)      // HTTOP..HTTOPRIGHT or HTBOTTOM..HTBOTTOMRIGHT
//         StartResize();                     // tail-jump 0xbb5d0 (BOOL result dropped)
//     else
//         CWnd::Default();                   // tail-jump 0x28ac80
// (point is never read.)
// Symbol: ?OnNcLButtonDown@CMFCPopupMenu@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDown_CMFCPopupMenu__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nHitTest, long long point)
{
    (void)point;
    if (!pThis) return;
    const unsigned int base = At<int>(pThis, kOffResizeBarOnTop) != 0 ? HTTOP : HTBOTTOM;
    if (nHitTest - base <= 2u) {
        impl__StartResize_CMFCPopupMenu__IEAAHXZ(pThis);
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Retail (RVA 0xba740, mfc140u), fully transcribed:
//     if (lParam == 0) AfxThrowInvalidArgException();              // 0x227720 (ENSURE)
//     switch (((NMHDR*)lParam)->code) {                            // +0x10
//     case TTN_SHOW /*-521*/: m_bIsTearOffCaptionActive = TRUE;  goto redraw;   // +0x1844
//     case TTN_POP  /*-522*/: m_bIsTearOffCaptionActive = FALSE;
//     redraw: ::InvalidateRect(m_hWnd, &m_rectTearOffCaption, TRUE); ::UpdateWindow(m_hWnd);   // +0x1848
//     }
//     return CWnd::OnNotify(wParam, lParam, pResult);              // tail-jump 0x28e150
// Symbol: ?OnNotify@CMFCPopupMenu@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CMFCPopupMenu__MEAAH_K_JPEA_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam, __int64* pResult)
{
    if (!pThis) return FALSE;
    if (lParam == 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // not reached: the thunk throws (detail/MfcExceptionsSupport.cpp)
    }
    const NMHDR* pNMHDR = reinterpret_cast<const NMHDR*>(lParam);
    const unsigned int code = pNMHDR->code;
    if (code == static_cast<unsigned int>(TTN_SHOW) || code == static_cast<unsigned int>(TTN_POP)) {
        At<int>(pThis, kOffTearOffCaptionActive) = (code == static_cast<unsigned int>(TTN_SHOW)) ? TRUE : FALSE;
        ::InvalidateRect(HWndOf(pThis), &At<RECT>(pThis, kOffRectTearOffCaption), TRUE);
        ::UpdateWindow(HWndOf(pThis));
    }
    return impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(AsWnd(pThis), static_cast<WPARAM>(wParam),
                                                static_cast<LPARAM>(lParam),
                                                reinterpret_cast<LRESULT*>(pResult));
}

// Retail (RVA 0xb5ac0, mfc140u), fully transcribed:
//     CPaintDC dc(this);                                           // 0x2a3d20 / 0x2a3dd0
//     if (!m_bAnimationIsDone && !CMFCToolBar::m_bCustomizeMode)  // +0x169c, 0x3be35c
//         DrawFade(&dc);                                           // vslot 0x3f0
//     else
//         DoPaint(&dc);                                            // vslot 0x3e8
// CPaintDC exists in this DLL only as its impl__ thunks, so the object is built
// in raw storage and torn down through the matching thunk (as CDockablePane's
// OnPaint does).  DrawFade/DoPaint are reached through this file's thunks,
// non-virtually (DEVIATION for derived overrides); both are still stubs.
// Symbol: ?OnPaint@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, AsWnd(pThis));
    if (At<int>(pThis, kOffAnimationIsDone) == 0 && impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        impl__DrawFade_CMFCPopupMenu__MEAAXPEAVCDC___Z(pThis, pDC);
    } else {
        impl__DoPaint_CMFCPopupMenu__MEAAXPEAVCDC___Z(pThis, pDC);
    }
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail (RVA 0xbad80, mfc140u), fully transcribed:
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     if (::PtInRect(&m_rectTearOffCaption, pt)) {                 // +0x1848
//         <afxGlobalData init gate>                                // 0x3c1620
//         if (afxGlobalData.m_hcurSizeAll == NULL) {               // +0x170
//             AfxGetModuleState();                                 // inlined AfxGetApp(), result unused
//             afxGlobalData.m_hcurSizeAll = ::LoadCursorW(NULL, IDC_SIZEALL /*0x7f86*/);   // CWinApp::LoadStandardCursor
//         }
//         ::SetCursor(afxGlobalData.m_hcurSizeAll);
//         return TRUE;
//     }
//     return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);      // 0x29d1f0
// Symbol: ?OnSetCursor@CMFCPopupMenu@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCPopupMenu__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message)
{
    if (!pThis) return FALSE;
    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    ::ScreenToClient(HWndOf(pThis), &pt);
    if (::PtInRect(&At<RECT>(pThis, kOffRectTearOffCaption), pt)) {
        EnsureGlobalDataInitialized();
        if (GlobalDataCursor(kGlobalDataCurSizeAll) == nullptr) {
            HCURSOR h = ::LoadCursorW(nullptr, IDC_SIZEALL);
            EnsureGlobalDataInitialized();
            SetGlobalDataCursor(kGlobalDataCurSizeAll, h);
        }
        EnsureGlobalDataInitialized();
        ::SetCursor(GlobalDataCursor(kGlobalDataCurSizeAll));
        return TRUE;
    }
    return impl__OnSetCursor_CFrameWnd__IEAAHPEAVCWnd__II_Z(AsFrame(pThis), pWnd, nHitTest, message);
}

// Retail (RVA 0xbad50, mfc140u), fully transcribed:
//     CWnd::Default();                                             // 0x28ac80
//     if (!bShow) m_bShown = FALSE;                                // +0x1670
// (nStatus is never read.)
// Symbol: ?OnShowWindow@CMFCPopupMenu@@IEAAXHI@Z
extern "C" void MS_ABI impl__OnShowWindow_CMFCPopupMenu__IEAAXHI_Z(void* pThis, int bShow, unsigned int nStatus)
{
    (void)nStatus;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    if (bShow == 0) At<int>(pThis, kOffShown) = FALSE;
}

// Retail (RVA 0xb5990, mfc140u):
//     CFrameWnd::OnSize(nType, cx, cy);                            // 0x2a0450
//     CMFCPopupMenuBar* pBar = GetMenuBar();                       // vslot 0x3a0
//     if (pBar->m_bInUpdateShadow) return;                         // bar +0x111c
//     if (pBar->m_hWnd != NULL) { AdjustScroll(TRUE); SetScrollBar(); }   // 0xb9820, 0xbae90
//     int nBorder = GetBorderSize();                               // vslot 0x428
//     if (m_iLogoWidth > 0) {                                      // +0x1680
//         CRect rect; ::GetClientRect(m_hWnd, &rect);
//         switch (m_nLogoLocation) {                               // +0x1688
//         case MENU_LOGO_LEFT:   rect.right  = rect.left + m_iLogoWidth + nBorder; break;
//         case MENU_LOGO_RIGHT:  rect.left   = rect.right - m_iLogoWidth - nBorder; break;
//         case MENU_LOGO_TOP:    rect.bottom = rect.top + m_iLogoWidth + nBorder; break;
//         case MENU_LOGO_BOTTOM: rect.top    = rect.bottom - m_iLogoWidth - nBorder; break;
//         }
//         ::InvalidateRect(m_hWnd, &rect, TRUE); ::UpdateWindow(m_hWnd);
//     }
// PARTIAL: the two bar-dependent steps (the m_bInUpdateShadow early return and
// the AdjustScroll/SetScrollBar pair behind the bar's HWND) are skipped because
// the embedded bar is not constructed by OpenMFC; the base call and the logo
// invalidation are transcribed.  GetBorderSize is called non-virtually.
// Symbol: ?OnSize@CMFCPopupMenu@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCPopupMenu__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy)
{
    if (!pThis) return;
    impl__OnSize_CFrameWnd__IEAAXIHH_Z(AsFrame(pThis), nType, cx, cy);
    // TODO(clean-room): retail returns here when m_wndMenuBar.m_bInUpdateShadow
    // is set and otherwise runs AdjustScroll(TRUE) + SetScrollBar() when the
    // bar has a window -- the embedded bar is not modeled.
    const int nBorder = impl__GetBorderSize_CMFCPopupMenu__MEBAHXZ(pThis);
    const int nLogo = At<int>(pThis, kOffLogoWidth);
    if (nLogo > 0) {
        RECT rect = { 0, 0, 0, 0 };
        ::GetClientRect(HWndOf(pThis), &rect);
        switch (At<int>(pThis, kOffLogoLocation)) {
        case 0: rect.right  = rect.left + nLogo + nBorder;   break;   // MENU_LOGO_LEFT
        case 1: rect.left   = rect.right - nLogo - nBorder;  break;   // MENU_LOGO_RIGHT
        case 2: rect.bottom = rect.top + nLogo + nBorder;    break;   // MENU_LOGO_TOP
        case 3: rect.top    = rect.bottom - nLogo - nBorder; break;   // MENU_LOGO_BOTTOM
        default: break;
        }
        ::InvalidateRect(HWndOf(pThis), &rect, TRUE);
        ::UpdateWindow(HWndOf(pThis));
    }
}

// Retail (RVA 0xb7b70..0xb7ec0, mfc140u, 199 instructions).  It first takes
// the embedded bar (vslot 0x3a0) and switches on nIDEvent - 0xec15:
//   0xec16 (scroll timer): GetCursorPos/ScreenToClient; if GetSelItem()
//     (0xb7630) is non-NULL calls its vslot 0xc0 (CMFCToolBarMenuButton::
//     OnCancelMode); then if the cursor is in m_rectScrollUp (+0x16ac) with
//     m_iScrollMode < 0, or in m_rectScrollDn (+0x16bc) with m_iScrollMode > 0,
//     moves the bar's m_iOffset (+0x1364) by -1/+1, calls the bar's vslot 0x7d8
//     (CMFCPopupMenuBar::AdjustLocations) and AdjustScroll(FALSE) (0xb9820);
//     otherwise KillTimer(0xec16), m_iScrollMode = 0 and InvalidateRect on
//     both arrow rects.
//   0xec15 (animation timer): clock()-based stepping of m_iFadePercent
//     (+0x16d8, += 10, clamped at 101 and tested against 100) through the
//     bar's vslot 0x6c0, the non-exported helper 0x2e600 and
//     m_AnimationSpeed (0x3b1b30); when finished KillTimer, a vslot-0x480
//     call, ValidateRect, m_bAnimationIsDone = TRUE (+0x169c), a
//     m_DropDirection (+0x1650) test, UpdateShadow (0xba8a0) and RedrawWindow.
//   anything else: CWnd::Default (0x28ac80).
// Both branches drive the embedded bar, which OpenMFC does not construct, so
// this stays a documented stub.
// TODO(clean-room): not transcribed -- embedded menu bar is not modeled.
// Symbol: ?OnTimer@CMFCPopupMenu@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCPopupMenu__IEAAX_K_Z(void* pThis, unsigned __int64 nIDEvent)
{
    (void)pThis; (void)nIDEvent;
}

// Retail (RVA 0xbb160, mfc140u):
//     if (pScrollBar->GetSafeHwnd() != m_wndScrollBarVert.GetSafeHwnd()) {      // +0x16e0
//         CFrameWnd::OnVScroll(nSBCode, nPos, pScrollBar); return;             // 0x29e4b0
//     }
//     CMFCPopupMenuBar* pBar = GetMenuBar();                                    // vslot 0x3a0
//     SCROLLINFO si = {sizeof si, SIF_ALL/*0x17*/}; ::GetScrollInfo(m_wndScrollBarVert.m_hWnd, SB_CTL, &si);
//     int nMaxOffset = si.nMax;
//     if (nMaxOffset <= 0) {   // derive from the bar: rows = (buttons * rowHeight - m_nMenuBarHeight) / rowHeight + 1
//         int nRowHeight = pBar->GetRowHeight();                                // vslot 0x6b8
//         if (pBar->m_Buttons.GetCount() > 0 && nRowHeight > 0)                 // bar +0x11a0
//             nMaxOffset = max(0, (count * nRowHeight - m_nMenuBarHeight) / nRowHeight + 1);   // +0x1678
//     }
//     int nOffset = pBar->m_iOffset;                                            // bar +0x1364
//     switch (nSBCode) { SB_LINEUP: --; SB_LINEDOWN: ++; SB_PAGEUP: -= si.nPage; SB_PAGEDOWN: += si.nPage;
//                        SB_THUMBPOSITION/SB_THUMBTRACK: = nPos; SB_TOP: = 0; SB_BOTTOM: = nMaxOffset; default: return; }
//     nOffset = min(max(nOffset, 0), nMaxOffset);
//     if (nOffset == pBar->m_iOffset) return;
//     pBar->m_iOffset = nOffset; pBar->AdjustLocations();                      // vslot 0x7d8
//     if (m_wndScrollBarVert.m_hWnd) ::SetScrollPos(m_wndScrollBarVert.m_hWnd, SB_CTL, nOffset, TRUE);
//     AdjustScroll(FALSE);                                                      // 0xb9820
// Both the embedded bar and the embedded scroll bar are unconstructed in
// OpenMFC, so this stays a documented stub.
// TODO(clean-room): not transcribed -- embedded menu bar / scroll bar are not modeled.
// Symbol: ?OnVScroll@CMFCPopupMenu@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CMFCPopupMenu__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar)
{
    (void)pThis; (void)nSBCode; (void)nPos; (void)pScrollBar;
}

// Retail (RVA 0xbbaa0, mfc140u), fully transcribed:
//     CWnd::Default();                                             // 0x28ac80
//     if (m_pWndShadow != NULL && m_pWndShadow->m_hWnd != NULL     // +0x1838
//         && !m_bResizeTracking) {                                 // +0x1978 (a NULL-lpwndpos guard is added below; retail has none)
//         if (lpwndpos->flags & SWP_HIDEWINDOW)                    // +0x20
//             m_pWndShadow->ShowWindow(SW_HIDE);                   // 0x2a9ad0
//         else if ((lpwndpos->flags & (SWP_NOSIZE|SWP_NOMOVE)) == 0 || (lpwndpos->flags & SWP_SHOWWINDOW))
//             m_pWndShadow->Repos();                               // 0xb4af0
//     }
// m_pWndShadow stays NULL in OpenMFC (no CMFCShadowWnd is ever created for a
// popup), so only the base call is live today; Repos is still an empty stub.
// Symbol: ?OnWindowPosChanged@CMFCPopupMenu@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCPopupMenu__IEAAXPEAUtagWINDOWPOS___Z(void* pThis, WINDOWPOS* lpwndpos)
{
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    void* pShadow = At<void*>(pThis, kOffWndShadow);
    if (pShadow != nullptr && At<HWND>(pShadow, kOffHWnd) != nullptr &&
        At<int>(pThis, kOffResizeTracking) == 0 && lpwndpos != nullptr) {
        if (lpwndpos->flags & SWP_HIDEWINDOW) {
            impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(pShadow), SW_HIDE);
        } else if ((lpwndpos->flags & (SWP_NOSIZE | SWP_NOMOVE)) == 0 || (lpwndpos->flags & SWP_SHOWWINDOW)) {
            impl__Repos_CMFCShadowWnd__AEAAXXZ(pShadow);
        }
    }
}

// Retail (RVA 0xb97b0, mfc140u), fully transcribed:
//     if (m_pMessageWnd == NULL) return FALSE;                     // +0x1f8
//     UINT uiMsg = (uiCommandID - 0xf000 <= 0x1ef) ? WM_SYSCOMMAND : WM_COMMAND;   // SC_* range
//     return ::PostMessageW(m_pMessageWnd->m_hWnd, uiMsg, uiCommandID, 0);        // tail-jump
// Symbol: ?PostCommand@CMFCPopupMenu@@QEAAHI@Z
extern "C" int MS_ABI impl__PostCommand_CMFCPopupMenu__QEAAHI_Z(void* pThis, unsigned int uiCommandID)
{
    if (!pThis) return FALSE;
    CWnd* pMsgWnd = At<CWnd*>(pThis, kOffMessageWnd);
    if (!pMsgWnd) return FALSE;
    const unsigned int uiMsg = (uiCommandID - 0xf000u <= 0x1efu) ? WM_SYSCOMMAND : WM_COMMAND;
    return ::PostMessageW(pMsgWnd->m_hWnd, uiMsg, uiCommandID, 0) ? TRUE : FALSE;
}

// Retail (RVA 0xb6f90, mfc140u), fully decoded.  The entry is resolved from
// the mfc140u export table by ordinal (urva.py) -- the mfc140u_rva_symbols.json
// map does not list it -- and the body matches the ANSI export instruction for
// instruction (?PostNcDestroy@CMFCPopupMenu@@MEAAXXZ is mfc140.dll RVA 0xb7550,
// an mfc140.dll address, NOT an mfc140u one; in mfc140u 0xb7550 is
// GetParentPopupMenu):
//     if (m_pParentBtn) m_pParentBtn->OnCancelMode();              // +0x228, vslot 0xc0 = CMFCToolBarMenuButton vftable 0x1803184e8 slot 24 -> 0x175820 (?OnCancelMode@CMFCToolBarMenuButton@@UEAAXXZ)
//     if (m_pParentRibbonElement) m_pParentRibbonElement->ClosePopupMenu();   // +0x19b8, vslot 0x340 = CMFCRibbonBaseElement vftable 0x1802dc5c0 slot 104 -> 0x110a0 (?ClosePopupMenu@CMFCRibbonBaseElement@@UEAAXXZ)
//     (this->*vslot 1)(1);                                         // tail-jump: scalar deleting destructor with the delete flag = `delete this` (CMFCPopupMenu vftable 0x1802f85b0 slot 1 -> 0xb4e10)
// There is no base-class call.  Both indirect calls go through the CFG
// dispatch thunk (0x1802c7b30 = GuardCFDispatchFunctionPointer of the
// mfc140u load-config directory).
// PARTIAL: the two notifications are transcribed, reached through the two
// classes' exported thunks instead of their vtables (DEVIATION for a
// client-derived button/element override; both thunks are still documented
// no-op stubs in their own files today, so the live effect is nil until they
// are filled in).  The `delete this` is deliberately OMITTED: the storage was
// allocated by the client and the object's vptr may be the clean-room mingw
// one (see the ctor note at the top of this file), so neither the deleting
// destructor slot nor the matching allocator can be reached safely.  This
// matches the repo's CFrameWnd::PostNcDestroy (core/frame/CFrameWnd.cpp,
// RemoveFrameWnd() only) and CView::PostNcDestroy (core/view/CView.cpp,
// RemoveView only), neither of which deletes the object either.
// Symbol: ?PostNcDestroy@CMFCPopupMenu@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CMFCPopupMenu__MEAAXXZ(void* pThis)
{
    if (!pThis) return;
    void* pBtn = At<void*>(pThis, kOffParentBtn);
    if (pBtn) {
        impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(pBtn);
    }
    CMFCRibbonBaseElement* pElem = At<CMFCRibbonBaseElement*>(pThis, kOffParentRibbonElement);
    if (pElem) {
        impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(pElem);
    }
    // DEVIATION: retail ends with `delete this` here -- see the note above.
}

// Retail (RVA 0xba650, mfc140u):
//     if (m_wndToolTip.m_hWnd != NULL)                             // +0x1858 embedded CToolTipCtrl, hwnd at +0x1898
//         ::SendMessageW(m_wndToolTip.m_hWnd, TTM_RELAYEVENT /*0x407*/, 0, (LPARAM)pMsg);
//     if (pMsg->message == WM_MOUSEMOVE
//         && (!::IsRectEmpty(&m_rectScrollUp) || !::IsRectEmpty(&m_rectScrollDn))) {   // +0x16ac, +0x16bc
//         CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//         if (::PtInRect(&m_rectScrollUp, pt) || ::PtInRect(&m_rectScrollDn, pt)) {
//             OnMouseMove((UINT)pMsg->wParam, pt);                 // 0xb7ec0
//             return TRUE;
//         }
//     }
//     return CFrameWnd::PreTranslateMessage(pMsg);                 // 0x29ceb0
// PARTIAL: the tooltip relay is skipped -- the embedded CToolTipCtrl is not
// constructed by OpenMFC, so its m_hWnd slot is not meaningful; the
// scroll-arrow hover redirect and the base call are transcribed.
// Symbol: ?PreTranslateMessage@CMFCPopupMenu@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCPopupMenu__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg)
{
    if (!pThis || !pMsg) return FALSE;
    // TODO(clean-room): retail relays pMsg to m_wndToolTip (TTM_RELAYEVENT)
    // here; the embedded tooltip control is not modeled.
    if (pMsg->message == WM_MOUSEMOVE &&
        (!::IsRectEmpty(&At<RECT>(pThis, kOffRectScrollUp)) || !::IsRectEmpty(&At<RECT>(pThis, kOffRectScrollDn)))) {
        POINT pt = { 0, 0 };
        ::GetCursorPos(&pt);
        ::ScreenToClient(HWndOf(pThis), &pt);
        if (::PtInRect(&At<RECT>(pThis, kOffRectScrollUp), pt) || ::PtInRect(&At<RECT>(pThis, kOffRectScrollDn), pt)) {
            impl__OnMouseMove_CMFCPopupMenu__IEAAXIVCPoint___Z(pThis, static_cast<unsigned int>(pMsg->wParam), PackPoint(pt));
            return TRUE;
        }
    }
    return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(AsFrame(pThis), pMsg);
}

// Retail (RVA 0xb5b50..0xb6b60, mfc140u, 989 instructions): the layout engine.
// Verified call/data set: the embedded bar (vslot 0x3a0) with its GetRowHeight
// (vslot 0x6b8) and AdjustLocations (vslot 0x7d8) plus five vslot-0x2d8 calls
// (CWnd::Create-family slot), GetParentArea (vslot 0x3b8), GetBorderSize
// (vslot 0x428), GetParentPopupMenu (0xb7550, five times), IsKindOf (0x234cf0)
// against RTC 0x1803b15f8 (CMFCToolBar), CMFCMenuBar::GetFloatPopupDirection
// (0x8e1e0), CMFCRibbonBaseElement::IsMenuMode (0x10520), the inlined
// CMFCVisualManager::GetInstance (0x9774) with a vslot-0x558 call on it,
// CToolTipCtrl::AddTool (0x2750f0), CWnd::ClientToScreen/ScreenToClient(RECT*),
// GetExStyle, SetWindowPos (0x2a9a60), CMFCToolBar::m_bCustomizeMode
// (0x3be35c), afxGlobalData (0x3c1620, +0x2a0), and USER32 GetSystemMetrics
// (six times), MonitorFromPoint/GetMonitorInfoW/SystemParametersInfoW,
// IntersectRect/CopyRect/SetRectEmpty/IsRectEmpty, GetWindowRect,
// InvalidateRect/RedrawWindow/UpdateWindow and SendMessageW.  Nothing of it
// can run without the embedded bar, so this stays a documented stub.
// TODO(clean-room): not transcribed -- embedded menu bar is not modeled.
// Symbol: ?RecalcLayout@CMFCPopupMenu@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMFCPopupMenu__UEAAXH_Z(void* pThis, int bNotify)
{
    (void)pThis; (void)bNotify;
}

// Retail (RVA 0xb78f0, mfc140u): `GetMenuBar()->RemoveAllButtons();` -- vslot
// 0x3a0 then vslot 0x6b0 (CMFCToolBar::RemoveAllButtons, 0x14ec50) on the
// embedded bar.  OpenMFC keeps this popup's item list in the mfccore side table
// that InsertItem/GetMenuItem in this file maintain, so the removal is applied
// there (DEVIATION from the retail dispatch, same visible result through this
// class' item API).
// Symbol: ?RemoveAllItems@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllItems_CMFCPopupMenu__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    auto it = g_popupMenuStates.find(static_cast<const CMFCPopupMenu*>(pThis));
    if (it == g_popupMenuStates.end()) return;
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (it->second.ownedItems.erase(item) != 0) delete item;
    }
    it->second.items.clear();
    it->second.selected = -1;
}

// Retail (RVA 0xb78b0, mfc140u): `return GetMenuBar()->RemoveButton(iIndex);`
// -- vslot 0x3a0 then vslot 0x6a8 (CMFCToolBar::RemoveButton, 0x14ecf0) on the
// embedded bar.  Applied to the side-table item list as RemoveAllItems above
// (DEVIATION from the retail dispatch); out-of-range indices return FALSE as
// CMFCToolBar::RemoveButton does.
// Symbol: ?RemoveItem@CMFCPopupMenu@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveItem_CMFCPopupMenu__QEAAHH_Z(void* pThis, int iIndex)
{
    if (!pThis) return FALSE;
    auto it = g_popupMenuStates.find(static_cast<const CMFCPopupMenu*>(pThis));
    if (it == g_popupMenuStates.end()) return FALSE;
    if (iIndex < 0 || iIndex >= static_cast<int>(it->second.items.size())) return FALSE;
    CMFCToolBarMenuButton* item = it->second.items[static_cast<size_t>(iIndex)];
    it->second.items.erase(it->second.items.begin() + iIndex);
    if (it->second.ownedItems.erase(item) != 0) delete item;
    if (it->second.selected == iIndex) it->second.selected = -1;
    else if (it->second.selected > iIndex) --it->second.selected;
    return TRUE;
}

// Retail (RVA 0xb6ff0, mfc140u), fully decoded but NOT implemented:
//     if (!CMFCToolBar::m_bCustomizeMode) return;                  // 0x3be35c
//     if (m_pParentBtn == NULL) return;                            // +0x228
//     if (m_pParentBtn->+0xe8 != 0) return;                        // m_bMenuPaletteMode by afxtoolbarmenubutton.h order (m_pPopupMenu is at +0xc0)
//     HMENU hMenu = GetMenuBar()->ExportToMenu();                  // vslot 0x3a0, vslot 0x888 (0xbd7b0)
//     if (hMenu == NULL) AfxThrowInvalidArgException();            // 0x227720
//     m_pParentBtn->CreateFromMenu(hMenu);                         // vslot 0x1b0 (0x173d20)
//     ::DestroyMenu(hMenu);
//     if (CMFCPopupMenu* pParent = GetParentPopupMenu()) pParent->SaveState();   // 0xb7550, vslot 0x3d0
// Left a stub: the exported menu comes from the embedded bar, which OpenMFC
// does not construct.
// TODO(clean-room): not transcribed -- embedded menu bar is not modeled.
// Symbol: ?SaveState@CMFCPopupMenu@@UEAAXXZ
extern "C" void MS_ABI impl__SaveState_CMFCPopupMenu__UEAAXXZ(void* pThis)
{
    (void)pThis;
}

// Retail (RVA 0xba7d0, mfc140u), fully transcribed:
//     if (nElapse - 1 <= 0xc7)               // 1..200 ms
//         m_AnimationSpeed = nElapse;        // 0x3b1b30
// Symbol: ?SetAnimationSpeed@CMFCPopupMenu@@SAXI@Z
extern "C" void MS_ABI impl__SetAnimationSpeed_CMFCPopupMenu__SAXI_Z(unsigned int nElapse)
{
    if (nElapse - 1u <= 0xc7u) impl__m_AnimationSpeed_CMFCPopupMenu__1IA = nElapse;
}

// Retail (RVA 0xb82a0, mfc140u):
//     if (iMaxWidth == m_iMaxWidth) return;                        // +0x167c
//     m_iMaxWidth = iMaxWidth;
//     if (m_hWnd == NULL) return;
//     CMFCPopupMenuBar* pBar = GetMenuBar();                       // vslot 0x3a0
//     if (!::IsWindow(m_hWnd) || !::IsWindow(pBar->m_hWnd)) return;
//     pBar->m_iMaxWidth = m_iMaxWidth;                             // bar +0x1370
//     RecalcLayout(TRUE);                                          // vslot 0x300
// PARTIAL: the store and the two early returns are transcribed; the push into
// the embedded bar and the relayout are skipped (bar not modeled).
// Symbol: ?SetMaxWidth@CMFCPopupMenu@@QEAAXH@Z
extern "C" void MS_ABI impl__SetMaxWidth_CMFCPopupMenu__QEAAXH_Z(void* pThis, int iMaxWidth)
{
    if (!pThis) return;
    if (iMaxWidth == At<int>(pThis, kOffMaxWidth)) return;
    At<int>(pThis, kOffMaxWidth) = iMaxWidth;
    if (HWndOf(pThis) == nullptr) return;
    // TODO(clean-room): retail copies m_iMaxWidth into m_wndMenuBar.m_iMaxWidth
    // and calls RecalcLayout(TRUE) when both windows exist -- not modeled.
}

// Retail (RVA 0xbae80, mfc140u), three instructions:
//     m_pParentRibbonElement = pElem;                              // +0x19b8
//     pElem->m_pPopupMenu = this;                                  // element +0x108, no NULL check
// PARTIAL: the first store is transcribed.  The back-link store is omitted:
// include/openmfc/afxmfc.h declares CMFCRibbonBaseElement with a 32-byte pad,
// so +0x108 lies outside an OpenMFC-constructed element (ribbon/
// CMFCRibbonBaseElement.cpp treats that slot as unmodeled for the same reason).
// Symbol: ?SetParentRibbonElement@CMFCPopupMenu@@QEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetParentRibbonElement_CMFCPopupMenu__QEAAXPEAVCMFCRibbonBaseElement___Z(
    void* pThis, CMFCRibbonBaseElement* pElem)
{
    if (!pThis) return;
    At<CMFCRibbonBaseElement*>(pThis, kOffParentRibbonElement) = pElem;
    // TODO(clean-room): retail also stores `this` into pElem->+0x108
    // (m_pPopupMenu); that member is outside the clean-room element.
}

// Retail (RVA 0xbad20, mfc140u), fully transcribed:
//     CMFCCustomizeMenuButton::m_bRecentlyUsedOld = CMFCMenuBar::m_bRecentlyUsedMenus;   // 0x3be1d0 <- 0x3b1b08
//     CMFCMenuBar::m_bRecentlyUsedMenus = FALSE;
//     m_bQuickCusomize = TRUE;                                     // +0x19a8
// Symbol: ?SetQuickMode@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__SetQuickMode_CMFCPopupMenu__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    impl__m_bRecentlyUsedOld_CMFCCustomizeMenuButton__2HA = impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;
    impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA = FALSE;
    At<int>(pThis, kOffQuickCustomize) = TRUE;
}

// Retail (RVA 0xbae90..0xbb160, mfc140u): returns unless m_bShowScrollBar
// (+0x16a8) and m_bScrollable (+0x16a4) are both set.  Otherwise takes the
// client rect, shrinks right/bottom by m_iShadowSize (+0x1810) outside
// CMFCToolBar::m_bCustomizeMode, InflateRect(-GetBorderSize()) (vslot 0x428),
// cuts off the logo strip (+0x1680/+0x1688), the tear-off caption height
// (+0x1848) and the resize bar height (+0x1998, top or bottom by +0x1980), and
// keeps the right SM_CXVSCROLL columns as the scroll bar rect.  Then either
// creates m_wndScrollBarVert (+0x16e0; vslot 0x2d8 = CScrollBar::Create with
// style 0x50000001 = WS_CHILD|WS_VISIBLE|SBS_VERT, parent this, id 1) or
// SetWindowPos (0x2a9a60) + GetScrollInfo on it; fills a SCROLLINFO whose
// nMax/nPage derive from the embedded bar's GetRowHeight (vslot 0x6b8) and
// m_Buttons count, stores the page size into the bar's m_nDropDownPageSize
// (+0x139c), SetScrollInfo, EnableScrollBar(ESB_ENABLE_BOTH / ESB_DISABLE_BOTH)
// and EnableWindow(TRUE) (0x2a9b30).  Needs both embedded objects; documented
// stub.
// TODO(clean-room): not transcribed -- embedded menu bar / scroll bar are not modeled.
// Symbol: ?SetScrollBar@CMFCPopupMenu@@IEAAXXZ
extern "C" void MS_ABI impl__SetScrollBar_CMFCPopupMenu__IEAAXXZ(void* pThis)
{
    (void)pThis;
}

// Retail (RVA 0xb8150..0xb829c, mfc140u), fully decoded.  The entry is
// resolved from the mfc140u export table by ordinal (urva.py; the
// mfc140u_rva_symbols.json map does not list it) and the body matches the
// ANSI export instruction for instruction (mfc140.dll RVA 0xb8710 -- an
// mfc140.dll address, NOT an mfc140u one):
//     if (m_pParentBtn == NULL || !m_pParentBtn->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) return;  // +0x228, 0x234cf0, RTC 0x1803b1748
//     GetMenuBar()->SetHot(NULL);                                  // vslot 0x3a0, 0x157d80 = ?SetHot@CMFCToolBar@@QEAAHPEAVCMFCToolBarButton@@@Z (mfc140u export table)
//     CMFCMenuBar::m_bShowAllCommands = TRUE;                      // 0x3be254
//     AFXPlaySystemSound(2);                                       // 0x132460
//     ShowWindow(SW_HIDE); m_bShown = FALSE;                       // 0x2a9ad0, +0x1670
//     if (m_bmpShadowRight.m_hObject)  m_bmpShadowRight.DeleteObject();    // +0x1818 (handle at +0x1820), 0x2a3f60
//     if (m_bmpShadowBottom.m_hObject) m_bmpShadowBottom.DeleteObject();   // +0x1828 (handle at +0x1830)
//     m_ptLocation = m_ptLocationInitial;                          // +0x200 <- +0x208 (one 8-byte move)
//     InitMenuBar();                                               // vslot 0x3f8 (vftable 0x1802f85b0 slot 127 -> 0xb8320), result dropped
//     if (m_bScrollable) { AdjustScroll(FALSE); SetScrollBar(); }  // +0x16a4, 0xb9820, 0xbae90
//     UpdateBottomWindows(FALSE);                                  // 0xb89d0
//     ShowWindow(SW_SHOWNOACTIVATE /*4*/);
//     CWnd* pParent = m_pParentBtn->m_pWndParent;                  // +0x80
//     if (pParent != NULL && ::IsWindow(pParent->m_hWnd)) {        // import 0x1802c7138
//         ::InvalidateRect(pParent->m_hWnd, &m_pParentBtn->m_rect /*+0x68*/, TRUE);   // 0x1802c7128
//         ::UpdateWindow(pParent->m_hWnd);                         // 0x1802c7300
//     }
// PARTIAL: everything above is transcribed except the two steps that touch
// objects the clean-room ctor never constructs -- the embedded bar's
// SetHot(NULL) and the two shadow-bitmap DeleteObject calls (their m_hObject
// slots at +0x1820/+0x1830 are uninitialised storage in an OpenMFC-built
// popup and must not be read).  InitMenuBar is reached through this file's
// thunk, non-virtually (DEVIATION for a client override); InitMenuBar,
// AdjustScroll and SetScrollBar are themselves still documented stubs here,
// so the re-import that is the point of the call does not happen yet.  Note
// the gate: the clean-room ctor zero-fills +0x228, and nothing in OpenMFC
// stores a parent button there, so on an OpenMFC-created popup this returns
// at the first test exactly as retail would for a popup without a parent
// button.
// Symbol: ?ShowAllCommands@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__ShowAllCommands_CMFCPopupMenu__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    CMFCToolBarButton* pBtn = At<CMFCToolBarButton*>(pThis, kOffParentBtn);
    if (!pBtn) return;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pBtn, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ()))
        return;
    // TODO(clean-room): retail calls m_wndMenuBar.SetHot(NULL) here -- the
    // embedded bar is not modeled.
    impl__m_bShowAllCommands_CMFCMenuBar__1HA = TRUE;
    impl__AFXPlaySystemSound__YAXH_Z(2);
    impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_HIDE);
    At<int>(pThis, kOffShown) = FALSE;
    // TODO(clean-room): retail deletes m_bmpShadowRight / m_bmpShadowBottom
    // here -- the embedded CBitmaps are not constructed, see above.
    At<POINT>(pThis, kOffPtLocation) = At<POINT>(pThis, kOffPtLocationInitial);
    impl__InitMenuBar_CMFCPopupMenu__MEAAHXZ(pThis);
    if (At<int>(pThis, kOffScrollable) != 0) {
        impl__AdjustScroll_CMFCPopupMenu__IEAAHH_Z(pThis, FALSE);
        impl__SetScrollBar_CMFCPopupMenu__IEAAXXZ(pThis);
    }
    impl__UpdateBottomWindows_CMFCPopupMenu__IEAAXH_Z(pThis, FALSE);
    impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_SHOWNOACTIVATE);
    static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
    static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "m_pWndParent @0x80");
    CWnd* pParent = pBtn->m_pWndParent;
    if (pParent != nullptr && ::IsWindow(pParent->m_hWnd)) {
        ::InvalidateRect(pParent->m_hWnd, &At<RECT>(pBtn, 0x68), TRUE);   // &m_pParentBtn->m_rect
        ::UpdateWindow(pParent->m_hWnd);
    }
}

// Retail (RVA 0xbb5d0..0xbb990, mfc140u, 249 instructions): the modal resize
// loop.  SetCapture, then GetMessageW/PeekMessageW/DispatchMessageW until the
// button goes up, sizing the frame from GetWindowRect / GetExStyle (RTL),
// pushing the new height into the embedded bar (vslot 0x3a0) and the scroll
// bar (GetScrollPos, OnVScroll 0xbb160), deleting the shadow bitmaps
// (0x2a3f60), reposting through UpdateBottomWindows (0xb89d0) and the shadow
// window (CMFCShadowWnd::Repos 0xb4af0 / ShowWindow), and re-posting WM_QUIT
// through AfxPostQuitMessage (0x1cc850) if one was swallowed.  Depends on the
// embedded bar and scroll bar; documented stub returning FALSE.
// TODO(clean-room): not transcribed -- embedded menu bar / scroll bar are not modeled.
// Symbol: ?StartResize@CMFCPopupMenu@@IEAAHXZ
extern "C" int MS_ABI impl__StartResize_CMFCPopupMenu__IEAAHXZ(void* pThis)
{
    (void)pThis;
    return FALSE;
}

// Retail (RVA 0xb9dd0..0xba230, mfc140u, 281 instructions): tears the menu off
// into a floating toolbar.  Walks the parent chain with IsKindOf (0x234cf0,
// seven RTTI tests), resolves the top-level frame (0x28e490 / g_pTopLevelFrame
// 0x3be1b8), builds the bar title from the parent button text (CloneData
// 0xdd40 + Remove('&') 0x12370), registers the new bar (CObList::AddTail
// 0x231e70) and hands the drag to CPaneFrameWnd::StartTearOff (0xb32c0) with a
// rect from GetWindowRect/ClientToScreen.  The tear-off bar is created through
// CreateTearOffBar (vslot 0x408), which is still a stub in this file, so
// nothing downstream can work; documented stub returning FALSE.
// TODO(clean-room): not transcribed -- tear-off bar construction is not modeled.
// Symbol: ?TearOff@CMFCPopupMenu@@MEAAHVCPoint@@@Z
extern "C" int MS_ABI impl__TearOff_CMFCPopupMenu__MEAAHVCPoint___Z(void* pThis, long long point)
{
    (void)pThis; (void)point;
    return FALSE;
}

// Retail (RVA 0xbb9f0..0xbba91, mfc140u), fully decoded.  The entry is
// resolved from the mfc140u export table by ordinal (urva.py; the
// mfc140u_rva_symbols.json map does not list it) and the body matches the
// ANSI export instruction for instruction (mfc140.dll RVA 0xbbfb0 -- an
// mfc140.dll address, NOT an mfc140u one):
//     m_bResizeTracking = TRUE;                                    // +0x1978
//     GetMenuBar()->m_bResizeTracking = TRUE;                      // vslot 0x3a0, bar +0x13a4
//     GetMenuBar()->AdjustLocations();                             // vslot 0x7d8 of the bar
//     ::RedrawWindow(m_hWnd, NULL, NULL, 0x581);                   // import 0x1802c7130; RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_UPDATENOW|RDW_FRAME
//     UpdateBottomWindows(FALSE);                                  // 0xb89d0
//     m_bResizeTracking = FALSE;
//     GetMenuBar()->m_bResizeTracking = FALSE;
// PARTIAL: the flag bracket, the repaint and UpdateBottomWindows are
// transcribed; the three bar accesses (its flag, twice, and its
// AdjustLocations relayout) are skipped because the embedded bar is not
// constructed by OpenMFC.  DEVIATION: retail hands m_hWnd to ::RedrawWindow
// unguarded; a NULL HWND makes USER32 repaint the desktop window, and an
// OpenMFC-created popup never owns a window, so the call is skipped when
// m_hWnd is NULL.
// Symbol: ?TriggerResize@CMFCPopupMenu@@QEAAXXZ
extern "C" void MS_ABI impl__TriggerResize_CMFCPopupMenu__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    At<int>(pThis, kOffResizeTracking) = TRUE;
    // TODO(clean-room): retail sets m_wndMenuBar.m_bResizeTracking and calls
    // m_wndMenuBar.AdjustLocations() here -- the embedded bar is not modeled.
    if (HWND hWnd = HWndOf(pThis)) {
        ::RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME);
    }
    impl__UpdateBottomWindows_CMFCPopupMenu__IEAAXH_Z(pThis, FALSE);
    At<int>(pThis, kOffResizeTracking) = FALSE;
    // TODO(clean-room): retail clears m_wndMenuBar.m_bResizeTracking here.
}

// Retail (RVA 0xbacd0, mfc140u), fully transcribed:
//     for (CMFCPopupMenu* p = m_pActivePopupMenu; p != NULL; p = p->GetParentPopupMenu()) {   // 0x3be288, 0xb7550
//         if (CWnd::FromHandlePermanent(p->m_hWnd) != NULL)        // 0x28adc0
//             p->UpdateShadow(lprectScreen);                       // 0xba8a0
//     }
// UpdateShadow is this file's thunk, still a generated stub.
// Symbol: ?UpdateAllShadows@CMFCPopupMenu@@SAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(RECT* lprectScreen)
{
    for (void* p = impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA; p != nullptr;
         p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(p)) {
        if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWndOf(p)) != nullptr) {
            impl__UpdateShadow_CMFCPopupMenu__QEAAXPEAUtagRECT___Z(p, lprectScreen);
        }
    }
}

// Retail (RVA 0xb89d0, mfc140u), fully transcribed:
//     CWnd* pWndTop = GetTopLevelParent();                         // 0x28e2e0
//     if (pWndTop == NULL) return;
//     if (m_iShadowSize != 0 && !m_bForceShadow) {                 // +0x1810, 0x3b1b34
//         RECT rcTop = {}; ::GetWindowRect(pWndTop->m_hWnd, &rcTop);
//         RECT rcMenu = { m_ptLocation.x, m_ptLocation.y,                          // +0x200
//                         m_ptLocation.x + m_iShadowSize + m_FinalSize.cx,         // +0x210
//                         m_ptLocation.y + m_iShadowSize + m_FinalSize.cy };
//         if (GetExStyle() & WS_EX_LAYOUTRTL)                                      // 0x2a96c0, bit 22
//             ::OffsetRect(&rcMenu, -(m_iShadowSize + m_FinalSize.cx), 0);
//         RECT rcUnion = {}; ::UnionRect(&rcUnion, &rcMenu, &rcTop);
//         if (!::EqualRect(&rcUnion, &rcMenu)) {
//             m_iShadowSize = 0;
//             if (!bCheckOnly)
//                 SetWindowPos(NULL, -1, -1, m_FinalSize.cx, m_FinalSize.cy,
//                              SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE /*0x16*/);   // 0x2a9a60; x/y are -1 in the bytes
//             return;
//         }
//     }
//     ::UpdateWindow(pWndTop->m_hWnd);
// (Yes: the rect test is UnionRect/EqualRect against rcMenu, i.e. the shadow
// is dropped unless the top-level window lies inside the menu rect; that is
// what the retail bytes do.)
// Symbol: ?UpdateBottomWindows@CMFCPopupMenu@@IEAAXH@Z
extern "C" void MS_ABI impl__UpdateBottomWindows_CMFCPopupMenu__IEAAXH_Z(void* pThis, int bCheckOnly)
{
    if (!pThis) return;
    CWnd* pWndTop = static_cast<CWnd*>(impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(AsWnd(pThis)));
    if (!pWndTop) return;
    if (At<int>(pThis, kOffShadowSize) != 0 && impl__m_bForceShadow_CMFCPopupMenu__1HA == 0) {
        RECT rcTop = { 0, 0, 0, 0 };
        ::GetWindowRect(pWndTop->m_hWnd, &rcTop);
        const POINT ptLocation = At<POINT>(pThis, kOffPtLocation);
        const SIZE  sizeFinal  = At<SIZE>(pThis, kOffFinalSize);
        const int   nShadow    = At<int>(pThis, kOffShadowSize);
        RECT rcMenu = { ptLocation.x, ptLocation.y,
                        ptLocation.x + nShadow + sizeFinal.cx,
                        ptLocation.y + nShadow + sizeFinal.cy };
        if (impl__GetExStyle_CWnd__QEBAKXZ(AsWnd(pThis)) & WS_EX_LAYOUTRTL) {
            ::OffsetRect(&rcMenu, -(nShadow + sizeFinal.cx), 0);
        }
        RECT rcUnion = { 0, 0, 0, 0 };
        ::UnionRect(&rcUnion, &rcMenu, &rcTop);
        if (!::EqualRect(&rcUnion, &rcMenu)) {
            At<int>(pThis, kOffShadowSize) = 0;
            if (bCheckOnly == 0) {
                impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pThis), nullptr, -1, -1, sizeFinal.cx, sizeFinal.cy,
                                                            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
            return;
        }
    }
    ::UpdateWindow(pWndTop->m_hWnd);
}

// Retail (RVA 0xba8a0, mfc140u): repaints the layered shadow window / the
// saved shadow bitmaps for this popup.  Not on this batch's list; only the
// parameter list was corrected (this, LPRECT) so UpdateAllShadows can call it.
// TODO(clean-room): not transcribed.
// Symbol: ?UpdateShadow@CMFCPopupMenu@@QEAAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__UpdateShadow_CMFCPopupMenu__QEAAXPEAUtagRECT___Z(void* pThis, RECT* lprectScreen)
{
    (void)pThis; (void)lprectScreen;
}
