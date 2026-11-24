// CMFCToolTipCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <new>
#include <uxtheme.h>

// ===========================================================================
// CMFCToolTipCtrl -- the feature-pack tooltip (label + optional icon,
// separator and description, drawn through the visual manager).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the RVA of the function ENTRY it was read from, and every Win32 import
// named here was resolved from its IAT slot with iatu.py (GetCursorPos,
// ScreenToClient, GetClientRect, GetWindowRect, SendMessageW, InflateRect,
// GetSysColor, FillRect, GetTextExtentPoint32W, MonitorFromPoint,
// GetMonitorInfoW, SystemParametersInfoW, GetSystemMetrics, GetWindowTheme,
// wcschr).  Ten of the twenty exports are not in the mfc140u RVA map used by
// disas.py --u (SetParams, SetLocation, SetHotRibbonButton, SetDescription,
// OnPop, OnEraseBkgnd, OnFillBackground, OnDrawSeparator, OnShow, OnPaint);
// their entries were recovered from the export table by ordinal (ordrva.py)
// and cross-checked against the class message map at 0x31bb98 (mfc140u):
// WM_PAINT -> 0x180db0, WM_CREATE -> 0x181e80, WM_ERASEBKGND -> 0x181f30,
// reflected TTN_SHOW -> 0x181130, reflected TTN_POP -> 0x181570.
//
// The class uses the 0x1a8-byte layout pinned by the retail constructor. The tooltip HWND routes paint and
// the owner's TTN_SHOW/TTN_POP notifications to the concrete object.
//
// Remaining notes on the drawing helpers:
//
////  (2) the visual manager.  Retail's inlined CMFCVisualManager::GetInstance
//      (the helper at 0x9774, mfc140u) reads ?m_pVisManager@CMFCVisualManager@@
//      (0x3be3c0) and CREATES the default manager when it is NULL; that
//      creating half exists here only as the C++ static
//      CMFCVisualManager::GetInstance() (visualmanager/CMFCVisualManager.cpp:715),
//      which the link audit forbids calling, so -- exactly as
//      docking/CPaneDivider.cpp and ribbon/CMFCRibbonPanelMenuBar.cpp do --
//      the exported static is read and the manager call is skipped while it
//      is NULL.  Retail then calls GetToolTipInfo / OnFillToolTip through the
//      manager's vtable (+0x5b8 / +0x5c0); OpenMFC's C++ CMFCVisualManager
//      declares neither virtual, so the base CMFCVisualManager export thunks
//      are called and a derived manager's override (e.g. the Office2003 /
//      Office2007 GetToolTipInfo thunks) is not reached.
//
//  (3) the ribbon-button arms.  GetIconSize (0x1816d0) and OnDrawIcon
//      (0x181980) have a first arm for m_pRibbonButton != NULL that calls
//      CMFCRibbonButton virtuals (+0x4e0, +0x238 GetImageSize, +0x250
//      DrawImage) and reads ribbon-button members at +0x148/+0x1b8/+0x1bc/
//      +0x1d8/+0x214.  OpenMFC's CMFCRibbonButton adds nothing but 64 bytes
//      of opaque padding over CMFCRibbonBaseElement (afxmfc.h:844), and its
//      ?DrawImage / ?GetImageSize thunks are still auto-generated placeholders
//      -- 39- and 38-parameter garbage argument lists with an empty / `return
//      nullptr` body (ribbon/CMFCRibbonButton.cpp:266/:303) -- so that arm is
//      NOT transcribed: GetIconSize returns
//      CSize(0,0) and OnDrawIcon returns FALSE when a ribbon button is set.
//      Each site documents the retail control flow it skips.
//
//  (4) afxGlobalData.  Retail reads ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A
//      (0x3c1620) after the inlined one-time "if (!<+0>) Initialize()" gate:
//      m_bIsRibbonImageScale (+0x2c0) / m_dblRibbonImageScale (+0x2b8) in
//      GetFixedWidth, and the CFont objects fontTooltip (+0x1b8) / fontBold
//      (+0x1c8) that OnDrawLabel / OnDrawDescription select.  OpenMFC exports
//      that object as a zero-filled 720-byte blob whose Initialize() is an
//      empty stub (core/runtime/AFX_GLOBAL_DATA.cpp:98), so the gate is
//      reproduced (controls/CMFCReBar.cpp's EnsureGlobalDataInitialized), the
//      scale reads whatever the blob holds, and a NULL font handle falls back
//      to the stock DEFAULT_GUI_FONT as controls/CMFCStatusBar.cpp:369 does.
//
// CString members use this DLL's own CString (a single m_pszData pointer,
// include/openmfc/afxstr.h), whose inline constructor / destructor / operator=
// / Replace / Empty perform the same nil-string / refcount operations the
// retail bodies inline or call (0xde30 CSimpleStringT::operator=, 0x12440
// CStringT::Replace(PCWSTR,PCWSTR), 0x33b0 CSimpleStringT::Empty).
// ===========================================================================

// Thunks defined elsewhere in the tree (every one checked to exist as a
// definition, not just a declaration):
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                           // core/window/Thunks.cpp:1182
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp:868
extern "C" void  MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString); // core/window/CWnd.cpp:898
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd); // core/gdi/CClientDC.cpp:11
extern "C" void  MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);                          // core/gdi/CClientDC.cpp:26
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject);    // core/gdi/CGdiObject.cpp:16
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);    // core/gdi/CDC.cpp:444
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y);      // core/gdi/CDC.cpp:61
extern "C" int   MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y);                 // core/gdi/CDC.cpp:74
extern "C" void  MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight); // core/gdi/CDC.cpp:583
extern "C" int   MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat); // detail/GdicoreSupport.cpp:140 (CDC::DrawText is inline in retail; this is the tree's helper, not an export)
extern "C" void* MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC);     // core/gdi/CDrawingManager.cpp:278
extern "C" void  MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis);                         // core/gdi/CDrawingManager.cpp:286
extern "C" void  MS_ABI impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish, int nAngle); // core/gdi/CDrawingManager.cpp:958
extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);                // core/runtime/AFX_GLOBAL_DATA.cpp:98 (empty stub)
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                       // featurepack/CMFC_misc_stubs.cpp (zero blob, retail offsets); 0x3c1620 (mfc140u)
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                            // core/runtime/StaticData.cpp; 0x3be3c0 (mfc140u)
extern "C" const unsigned char impl__wndTop_CWnd__2V1_B[];                                     // core/window/CWnd.cpp:594 (?wndTop@CWnd@@2V1@B, 0x3c3370 mfc140u); a CWnd-shaped static whose m_hWnd is HWND_TOP
extern "C" int   MS_ABI impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(CMFCVisualManager* pThis, CMFCToolTipInfo& params, unsigned int nType); // visualmanager/CMFCVisualManager.cpp:327
extern "C" void  MS_ABI impl__OnFillToolTip_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolTipCtrl__VCRect__AEAK3_Z(CMFCVisualManager* pThis, CDC* pDC, void* pToolTip, CRect rect, unsigned long& clrText, unsigned long& clrLine); // visualmanager/CMFCVisualManager.cpp:1797
extern "C" int   MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(CMFCToolBar* pThis, CPoint point);                       // toolbar/CMFCToolBar.cpp:5087
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCToolBar* pThis, int nIndex); // toolbar/Thunks.cpp:1193
extern "C" void* impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA;                 // toolbar/CMFCToolBar.cpp:3672; 0x3be370 (mfc140u)
extern "C" CMFCToolBarImages impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A;           // toolbar/CMFCToolBar.cpp:2874; 0x3c28e0 (mfc140u)
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;               // toolbar/CMFCToolBar.cpp:2860; 0x3c25a0 (mfc140u)
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;                                  // toolbar/StaticData.cpp:42; 0x3be368 (mfc140u)
// CMFCToolBarImages::PrepareDrawImage is still a placeholder that returns 0
// (toolbar/CMFCToolBarImages.cpp:930; retail ignores its result here anyway),
// EndDrawImage is real (toolbar/CMFCToolBarImages.cpp:447).  The declaration
// is the one controls/CMFCReBar.cpp:205 uses (CSize by value = one 8-byte
// register).
extern "C" int   MS_ABI impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(CMFCToolBarImages* pThis, void* pDS, long long sizeImageDest, int bFadeInactive);
extern "C" void  MS_ABI impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(CMFCToolBarImages* pThis, void* pDS);
// CMFCToolBarButton::OnDraw(CDC*, const CRect&, CMFCToolBarImages*, BOOL bHorz,
// BOOL bCustomizeMode, BOOL bHighlight, BOOL bDrawBorder, BOOL bGrayDisabledButtons)
// is still an EMPTY placeholder (toolbar/CMFCToolBarButton.cpp:105), so the
// toolbar arm of OnDrawIcon below paints nothing until it is implemented.
extern "C" void  MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(CMFCToolBarButton* pThis, CDC* pDC, const CRect* pRect, CMFCToolBarImages* pImages, int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons);
// CMFCRibbonBaseElement::GetTopLevelRibbonBar is still an argument-less
// placeholder returning nullptr (ribbon/CMFCRibbonBaseElement.cpp:483); OnShow
// only uses its result behind a NULL check, exactly as retail does.
extern "C" void* MS_ABI impl__GetTopLevelRibbonBar_CMFCRibbonBaseElement__QEBAPEAVCMFCRibbonBar__XZ(const void* pThis);

namespace {

static_assert(sizeof(CString) == 8, "CString must be a single m_pszData pointer");
static_assert(sizeof(CWnd) == 0xe8, "OpenMFC's CWnd is 0xe8 bytes, the retail CWnd size");
static_assert(sizeof(CToolTipCtrl) == 0x120, "CToolTipCtrl = CWnd + the 0x38-byte CMapStringToPtr retail constructs at +0xe8");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");
static_assert(sizeof(CSize) == 8 && sizeof(CPoint) == 8 && sizeof(CRect) == 16, "CSize/CPoint/CRect mirror SIZE/POINT/RECT");
static_assert(offsetof(CDC, m_hDC) == 0x8 && offsetof(CDC, m_hAttribDC) == 0x10, "CDC::m_hDC at +0x8, m_hAttribDC at +0x10");
static_assert(sizeof(CMFCToolBar) == 0x1350, "CMFCToolBar retail size (afxmfc.h harvested layout)");
static_assert(sizeof(CMFCToolBarImages) == 0x198, "CMFCToolBarImages retail size (afxmfc.h harvested layout)");
static_assert(sizeof(CMFCToolBarButton) >= 0x50, "the CMFCToolBarButton members read below sit inside the afxmfc.h layout");

inline CWnd* AsWnd(void* p) { return static_cast<CWnd*>(p); }
inline HWND HwndOf(void* p) { return AsWnd(p)->m_hWnd; }   // CWnd::m_hWnd, +0x40

// CMFCToolBar / CMFCToolBarButton / CMFCToolBarImages members the retail
// bodies read directly.  afxmfc.h declares the three classes with the
// harvested retail layout; the CMFCToolBar and CMFCToolBarButton members are
// accessible there and are pinned below, the two CMFCToolBarImages members are
// protected and are reached by offset (the offsets afxmfc.h's layout comment
// carries, the same ones controls/CMFCReBar.cpp uses):
constexpr std::size_t kTbOffImagesLocked     = 0x3f8;    // CMFCToolBar::m_ImagesLocked      (CMFCToolBarImages)
constexpr std::size_t kTbOffMenuImagesLocked = 0xd88;    // CMFCToolBar::m_MenuImagesLocked  (CMFCToolBarImages)
constexpr std::size_t kTbOffLocked           = 0x10b8;   // CMFCToolBar::m_bLocked           (BOOL)
constexpr std::size_t kTbbOffUserButton      = 0x08;     // CMFCToolBarButton::m_bUserButton
constexpr std::size_t kTbbOffText            = 0x0c;     // CMFCToolBarButton::m_bText
constexpr std::size_t kTbbOffImage           = 0x10;     // CMFCToolBarButton::m_bImage
constexpr std::size_t kTbbOffStyle           = 0x28;     // CMFCToolBarButton::m_nStyle
constexpr std::size_t kTbbOffImageIndex      = 0x48;     // CMFCToolBarButton::m_iImage
constexpr std::size_t kTbbOffUserImageIndex  = 0x4c;     // CMFCToolBarButton::m_iUserImage
constexpr std::size_t kTiOffCount            = 0x08;     // CMFCToolBarImages::m_iCount
constexpr std::size_t kTiOffSizeImage        = 0x68;     // CMFCToolBarImages::m_sizeImage (CSize)
static_assert(offsetof(CMFCToolBar, m_ImagesLocked) == kTbOffImagesLocked, "CMFCToolBar::m_ImagesLocked @0x3f8");
static_assert(offsetof(CMFCToolBar, m_MenuImagesLocked) == kTbOffMenuImagesLocked, "CMFCToolBar::m_MenuImagesLocked @0xd88");
static_assert(offsetof(CMFCToolBar, m_bLocked) == kTbOffLocked, "CMFCToolBar::m_bLocked @0x10b8");
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == kTbbOffUserButton, "CMFCToolBarButton::m_bUserButton @0x8");
static_assert(offsetof(CMFCToolBarButton, m_bText) == kTbbOffText, "CMFCToolBarButton::m_bText @0xc");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == kTbbOffImage, "CMFCToolBarButton::m_bImage @0x10");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == kTbbOffStyle, "CMFCToolBarButton::m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == kTbbOffImageIndex, "CMFCToolBarButton::m_iImage @0x48");
static_assert(offsetof(CMFCToolBarButton, m_iUserImage) == kTbbOffUserImageIndex, "CMFCToolBarButton::m_iUserImage @0x4c");

inline int  RdInt(const void* p, std::size_t off) { int v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof v); return v; }
inline void WrInt(void* p, std::size_t off, int v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof v); }
inline CSize RdSize(const void* p, std::size_t off) { CSize s; std::memcpy(&s, static_cast<const unsigned char*>(p) + off, sizeof s); return s; }
inline CMFCToolBarImages* TbImagesAt(CMFCToolBar* pBar, std::size_t off) { return reinterpret_cast<CMFCToolBarImages*>(reinterpret_cast<unsigned char*>(pBar) + off); }
// CMFCToolBarButton::GetImage() as retail inlines it (the neg/sbb/and $4 at
// 0x18180c in GetIconSize): m_bUserButton ? m_iUserImage : m_iImage.
inline int TbbImage(const CMFCToolBarButton* pBtn) { return RdInt(pBtn, RdInt(pBtn, kTbbOffUserButton) != 0 ? kTbbOffUserImageIndex : kTbbOffImageIndex); }

// ---- afxGlobalData (deviation (4)) ----------------------------------------
constexpr std::size_t kGdOffInitGate           = 0x000;
constexpr std::size_t kGdOffFontTooltipHandle  = 0x1c0;   // CFont fontTooltip @+0x1b8, its m_hObject @+0x1c0
constexpr std::size_t kGdOffFontBoldHandle     = 0x1d0;   // CFont fontBold    @+0x1c8, its m_hObject @+0x1d0
constexpr std::size_t kGdOffRibbonImageScale   = 0x2b8;   // double m_dblRibbonImageScale
constexpr std::size_t kGdOffIsRibbonImageScale = 0x2c0;   // BOOL   m_bIsRibbonImageScale

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline double GlobalDataDouble(std::size_t off) { double v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
// The retail one-time gate: if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }   // 0x3c1620, 0x6a790
inline void EnsureGlobalDataInitialized() {
    if (RdInt(GlobalDataBytes(), kGdOffInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        WrInt(GlobalDataBytes(), kGdOffInitGate, 1);
    }
}
// AFX_GLOBAL_DATA::GetRibbonImageScale() (afxglobals.h:205, inline):
//   m_bIsRibbonImageScale ? m_dblRibbonImageScale : 1.0
inline double GlobalDataRibbonImageScale() {
    return RdInt(GlobalDataBytes(), kGdOffIsRibbonImageScale) != 0 ? GlobalDataDouble(kGdOffRibbonImageScale) : 1.0;
}
inline HFONT GlobalDataFont(std::size_t hOff) {
    HFONT h = nullptr;
    std::memcpy(&h, GlobalDataBytes() + hOff, sizeof h);
    if (h == nullptr) h = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));   // deviation (4)
    return h;
}
// pDC->SelectObject(&afxGlobalData.<font>) as retail does it (CDC vslot +0x60,
// the CFont* overload): the HFONT is wrapped in a temporary CGdiObject so the
// exported CFont* SelectObject thunk is what runs, as
// ribbon/CMFCRibbonPanelMenuBar.cpp's SelectRibbonFont does.
inline CFont* SelectGlobalFont(CDC* pDC, std::size_t hOff) {
    CGdiObject* pFont = impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(GlobalDataFont(hOff));
    return impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, static_cast<CFont*>(pFont));
}

// The visual manager singleton (deviation (2)).
inline CMFCVisualManager* VisualManager() {
    return static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
}

constexpr unsigned int kTTM_GETMARGIN = 0x041b;   // WM_USER + 27
constexpr unsigned long kTTS_BALLOON  = 0x40;

inline RECT ToRECT(const CRect& r) { RECT rc = { r.left, r.top, r.right, r.bottom }; return rc; }

} // namespace

IMPLEMENT_DYNCREATE(CMFCToolTipCtrl, CToolTipCtrl)

static_assert(sizeof(CMFCToolTipCtrl) == 0x1a8);

CMFCToolTipCtrl::CMFCToolTipCtrl(CMFCToolTipInfo* params)
    : m_Params(params ? *params : CMFCToolTipInfo{}) {
    static_assert(offsetof(CMFCToolTipCtrl, m_pToolBar) == 0x120);
    static_assert(offsetof(CMFCToolTipCtrl, m_Params) == 0x140);
    static_assert(offsetof(CMFCToolTipCtrl, m_strDescription) == 0x178);
    static_assert(offsetof(CMFCToolTipCtrl, m_ptLocation) == 0x198);
}

CMFCToolTipCtrl::~CMFCToolTipCtrl() {
    if (m_hWnd) ::DestroyWindow(m_hWnd);
}

BOOL CMFCToolTipCtrl::Create(CWnd* parent, DWORD style) {
    if (m_hWnd || !parent || !parent->GetSafeHwnd()) return FALSE;
    const HWND owner = parent->GetSafeHwnd();
    if ((style & TTS_BALLOON) != 0) m_Params.m_bBalloonTooltip = TRUE;
    if (m_Params.m_bBalloonTooltip) style |= TTS_BALLOON;
    if (!CToolTipCtrl::Create(parent, style)) return FALSE;
    const auto id = reinterpret_cast<UINT_PTR>(this);
    if (!::SetWindowSubclass(m_hWnd, TooltipWindowSubclass, id, reinterpret_cast<DWORD_PTR>(owner)) ||
        !::SetWindowSubclass(owner, TooltipOwnerSubclass, id, 0)) {
        const HWND window = m_hWnd;
        m_hWnd = nullptr;
        ::DestroyWindow(window);
        return FALSE;
    }
    SetOwner(parent);
    CWnd::ModifyStyle(m_hWnd, WS_BORDER, 0, SWP_FRAMECHANGED);
    return TRUE;
}

void CMFCToolTipCtrl::SetOwner(CWnd* owner) {
    m_pToolBar = nullptr;
    if (!owner || !owner->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) return;
    for (auto* type = owner->GetRuntimeClass(); type; type = type->BaseClass()) {
        if (type->m_lpszClassName && std::strcmp(type->m_lpszClassName, "CMFCOutlookBarPane") == 0)
            return;
    }
    m_pToolBar = static_cast<CMFCToolBar*>(owner);
}

LRESULT CALLBACK CMFCToolTipCtrl::TooltipWindowSubclass(HWND window, UINT msg, WPARAM wp, LPARAM lp,
                                                       UINT_PTR id, DWORD_PTR owner) {
    auto* tooltip = reinterpret_cast<CMFCToolTipCtrl*>(id);
    if (msg == WM_NCDESTROY) {
        ::RemoveWindowSubclass(window, TooltipWindowSubclass, id);
        ::RemoveWindowSubclass(reinterpret_cast<HWND>(owner), TooltipOwnerSubclass, id);
        tooltip->m_hWnd = nullptr;
    } else if (!tooltip->m_Params.m_bBalloonTooltip) {
        if (msg == WM_PAINT) {
            tooltip->OnPaint();
            return 0;
        }
        if ((msg == WM_ERASEBKGND || msg == WM_PRINTCLIENT) && wp) {
            CDC dc;
            dc.m_hDC = dc.m_hAttribDC = reinterpret_cast<HDC>(wp);
            if (msg == WM_ERASEBKGND) tooltip->OnEraseBkgnd(&dc);
            else tooltip->DrawTooltip(dc);
            dc.m_hDC = dc.m_hAttribDC = nullptr;
            return msg == WM_ERASEBKGND;
        }
    }
    return ::DefSubclassProc(window, msg, wp, lp);
}

LRESULT CALLBACK CMFCToolTipCtrl::TooltipOwnerSubclass(HWND owner, UINT msg, WPARAM wp, LPARAM lp,
                                                      UINT_PTR id, DWORD_PTR) {
    auto* tooltip = reinterpret_cast<CMFCToolTipCtrl*>(id);
    if (msg == WM_NOTIFY && lp) {
        auto* notification = reinterpret_cast<NMHDR*>(lp);
        if (notification->hwndFrom == tooltip->m_hWnd) {
            LRESULT result = 0;
            if (notification->code == static_cast<UINT>(TTN_SHOW)) {
                tooltip->OnShow(notification, &result);
                return result;
            }
            if (notification->code == static_cast<UINT>(TTN_POP)) tooltip->OnPop(notification, &result);
        }
    } else if (msg == WM_NCDESTROY) {
        ::RemoveWindowSubclass(owner, TooltipOwnerSubclass, id);
        if (tooltip->m_hWnd) ::DestroyWindow(tooltip->m_hWnd);
    }
    return ::DefSubclassProc(owner, msg, wp, lp);
}

// Symbol: ??0CMFCToolTipCtrl@@QEAA@PEAVCMFCToolTipInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCToolTipCtrl__QEAA_PEAVCMFCToolTipInfo___Z(void* storage, CMFCToolTipInfo* params) {
    return storage ? new (storage) CMFCToolTipCtrl(params) : nullptr;
}

// Symbol: ??1CMFCToolTipCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolTipCtrl__UEAA_XZ(CMFCToolTipCtrl* tooltip) {
    if (tooltip) tooltip->~CMFCToolTipCtrl();
}

// Symbol: ?CreateObject@CMFCToolTipCtrl@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolTipCtrl__SAPEAVCObject__XZ() {
    return CMFCToolTipCtrl::CreateObject();
}

// Symbol: ?GetThisClass@CMFCToolTipCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolTipCtrl__SAPEAUCRuntimeClass__XZ() {
    return CMFCToolTipCtrl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMFCToolTipCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCToolTipCtrl__UEBAPEAUCRuntimeClass__XZ(
        const CMFCToolTipCtrl*) {
    return CMFCToolTipCtrl::GetThisClass();
}

void CMFCToolTipCtrl::SetParams(CMFCToolTipInfo* params) {
    m_Params = params ? *params : CMFCToolTipInfo{};
    if (m_hWnd)
        CWnd::ModifyStyle(m_hWnd, TTS_BALLOON, m_Params.m_bBalloonTooltip ? TTS_BALLOON : 0, SWP_FRAMECHANGED);
}

// Symbol: ?SetParams@CMFCToolTipCtrl@@QEAAXPEAVCMFCToolTipInfo@@@Z
extern "C" void MS_ABI impl__SetParams_CMFCToolTipCtrl__QEAAXPEAVCMFCToolTipInfo___Z(
        CMFCToolTipCtrl* tooltip, CMFCToolTipInfo* params) {
    if (tooltip) tooltip->SetParams(params);
}

// CMFCToolTipCtrl::SetLocation(CPoint pt) -- entry RVA 0x182230 (mfc140u;
// ordinal 13334): `mov %rdx,0x198(%rcx)` -- m_ptLocation = pt.  CPoint by
// value is the packed 8-byte register (x low, y high).
// Symbol: ?SetLocation@CMFCToolTipCtrl@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__SetLocation_CMFCToolTipCtrl__QEAAXVCPoint___Z(
        CMFCToolTipCtrl* tooltip, CPoint location) {
    if (tooltip) tooltip->SetLocation(location);
}

// CMFCToolTipCtrl::SetHotRibbonButton(CMFCRibbonButton* p) -- entry RVA
// 0x182240 (mfc140u; ordinal 13248): `mov %rdx,0x138(%rcx)` -- m_pRibbonButton = p.
// Symbol: ?SetHotRibbonButton@CMFCToolTipCtrl@@QEAAXPEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__SetHotRibbonButton_CMFCToolTipCtrl__QEAAXPEAVCMFCRibbonButton___Z(
        CMFCToolTipCtrl* tooltip, CMFCRibbonButton* button) {
    if (tooltip) tooltip->SetHotRibbonButton(button);
}

void CMFCToolTipCtrl::SetDescription(const CString description) {
    GetHotButton();
    m_strDescription = description;
    m_strDescription.Replace(L"\t", L"    ");
}

// Symbol: ?SetDescription@CMFCToolTipCtrl@@UEAAXV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetDescription_CMFCToolTipCtrl__UEAAXV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        CMFCToolTipCtrl* tooltip, CString* description) {
    if (!description) return;
    const CString value = *description;
    description->~CString();
    if (tooltip) tooltip->CMFCToolTipCtrl::SetDescription(value);
}

// CMFCToolTipCtrl::GetHotButton() -- RVA 0x182110 (mfc140u), transcribed:
//     m_pHotButton = NULL; m_pToolBarImages = NULL;
//     if (m_pRibbonButton != NULL) return;
//     if (m_pToolBar == NULL) return;
//     CPoint pt(0,0); ::GetCursorPos(&pt); ::ScreenToClient(m_pToolBar->m_hWnd, &pt);
//     m_pHotButton = m_pToolBar->GetButton(m_pToolBar->HitTest(pt));   // HitTest: CMFCToolBar vslot +0x730 (deviation (1): thunk called directly); GetButton 0x14fe00
//     if (m_pHotButton == NULL) return;
//     if (m_pToolBar->m_bLocked) {                                        // +0x10b8
//         if (m_pToolBar->m_MenuImagesLocked.GetCount() > 0)  m_pToolBarImages = &m_pToolBar->m_MenuImagesLocked;   // +0xd88
//         else if (m_pToolBar->m_ImagesLocked.GetCount() > 0) m_pToolBarImages = &m_pToolBar->m_ImagesLocked;       // +0x3f8
//         else                                                m_pToolBarImages = NULL;
//     } else if (m_pHotButton->m_bUserButton) {                          // +0x8
//         m_pToolBarImages = CMFCToolBar::m_pUserImages;                  // 0x3be370
//     } else {
//         m_pToolBarImages = &CMFCToolBar::m_MenuImages;                  // 0x3c28e0
//         if (CMFCToolBar::m_MenuImages.GetCount() <= 0) m_pToolBarImages = &CMFCToolBar::m_Images;   // 0x3c25a0
//     }
void CMFCToolTipCtrl::GetHotButton() {
    m_pHotButton = nullptr;
    m_pToolBarImages = nullptr;
    if (m_pRibbonButton != nullptr) return;
    CMFCToolBar* pBar = m_pToolBar;
    if (pBar == nullptr) return;

    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    ::ScreenToClient(AsWnd(pBar)->m_hWnd, &pt);
    const int nHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, CPoint(pt.x, pt.y));
    m_pHotButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, nHit);
    if (m_pHotButton == nullptr) return;

    if (RdInt(pBar, kTbOffLocked) != 0) {
        CMFCToolBarImages* pMenuLocked = TbImagesAt(pBar, kTbOffMenuImagesLocked);
        if (RdInt(pMenuLocked, kTiOffCount) > 0) {
            m_pToolBarImages = pMenuLocked;
        } else {
            CMFCToolBarImages* pLocked = TbImagesAt(pBar, kTbOffImagesLocked);
            m_pToolBarImages = RdInt(pLocked, kTiOffCount) > 0 ? pLocked : nullptr;
        }
    } else if (RdInt(m_pHotButton, kTbbOffUserButton) != 0) {
        m_pToolBarImages = static_cast<CMFCToolBarImages*>(impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA);
    } else {
        m_pToolBarImages = &impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A;
        if (RdInt(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A, kTiOffCount) <= 0) {
            m_pToolBarImages = &impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;
        }
    }
}

// Symbol: ?GetHotButton@CMFCToolTipCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__GetHotButton_CMFCToolTipCtrl__IEAAXXZ(CMFCToolTipCtrl* tooltip) {
    if (tooltip) tooltip->GetHotButton();
}

// CMFCToolTipCtrl::GetFixedWidth() -- RVA 0x182250 (mfc140u), transcribed:
//     <afxGlobalData init gate>                                                  // 0x3c1620, 0x6a790
//     double dblScale = afxGlobalData.m_bIsRibbonImageScale ? afxGlobalData.m_dblRibbonImageScale : 1.0;   // +0x2c0 / +0x2b8 / 0x34ff68
//     int nLargeImageWidth = (int)(dblScale * 32.0);                            // 0x350060, cvttsd2si
//     return m_sizeImage.cx > nLargeImageWidth ? m_nFixedWidthWithImage : m_nFixedWidthRegular;   // +0x180, cmovg +0x1a4 / +0x1a0
int CMFCToolTipCtrl::GetFixedWidth() {
    EnsureGlobalDataInitialized();
    const int nLargeImageWidth = static_cast<int>(GlobalDataRibbonImageScale() * 32.0);
    return m_sizeImage.cx > nLargeImageWidth ? m_nFixedWidthWithImage : m_nFixedWidthRegular;
}

// Symbol: ?GetFixedWidth@CMFCToolTipCtrl@@IEAAHXZ
extern "C" int MS_ABI impl__GetFixedWidth_CMFCToolTipCtrl__IEAAHXZ(CMFCToolTipCtrl* tooltip) {
    return tooltip ? tooltip->GetFixedWidth() : 0;
}

// CMFCToolTipCtrl::GetIconSize() -- RVA 0x1816d0 (mfc140u).  The toolbar arm is
// transcribed:
//     if (m_pRibbonButton != NULL) { ... see below ... }
//     if (m_pHotButton == NULL || m_pToolBarImages == NULL) return CSize(0,0);   // +0x130 / +0x128
//     if (m_pToolBarImages->m_iCount == 0) return CSize(0,0);                    // +0x8
//     if (m_pHotButton->GetImage() < 0) return CSize(0,0);                       // inline: +0x48 or +0x4c by m_bUserButton
//     return m_pToolBarImages->m_sizeImage;                                       // +0x68
// The ribbon arm (deviation (3), NOT transcribed -- OpenMFC's ribbon button is
// opaque and its GetImageSize thunk is a placeholder) reads, in retail:
//     if (!m_pRibbonButton->vslot(+0x4e0)()) return CSize(0,0);
//     BOOL bLarge = m_pRibbonButton->[+0x214];
//     if (m_pRibbonButton->[+0x1d8] != NULL) {                                   // an icon handle
//         m_nRibbonImageType = bLarge ? 0 : 1;
//         return m_pRibbonButton->GetImageSize(m_nRibbonImageType);              // vslot +0x238
//     }
//     CSize s(0,0);
//     if (bLarge && m_pRibbonButton->[+0x1bc] >= 0) s = m_pRibbonButton->GetImageSize(0);
//     if (s != CSize(0,0)) { m_nRibbonImageType = 0; return s; }
//     if (m_pRibbonButton->[+0x1b8] >= 0) s = m_pRibbonButton->GetImageSize(1);
//     m_nRibbonImageType = 1; return s;
// Here that arm returns CSize(0,0) and leaves m_nRibbonImageType untouched.
CSize CMFCToolTipCtrl::GetIconSize() {
    CSize size(0, 0);
    if (m_pRibbonButton != nullptr) {
        return size;   // deviation (3)
    }
    if (m_pHotButton == nullptr || m_pToolBarImages == nullptr) return size;
    if (RdInt(m_pToolBarImages, kTiOffCount) == 0) return size;
    if (TbbImage(m_pHotButton) < 0) return size;
    size = RdSize(m_pToolBarImages, kTiOffSizeImage);
    return size;
}

// Symbol: ?GetIconSize@CMFCToolTipCtrl@@UEAA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetIconSize_CMFCToolTipCtrl__UEAA_AVCSize__XZ(CMFCToolTipCtrl* tooltip, CSize* result) {
    if (result) *result = tooltip ? tooltip->CMFCToolTipCtrl::GetIconSize() : CSize(0, 0);
    return result;
}

// CMFCToolTipCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) -- RVA 0x181e80
// (mfc140u), transcribed:
//     if (CWnd::Default() == -1) return -1;                                       // 0x28ac80 (CToolTipCtrl::OnCreate is CWnd's)
//     CWnd* pParent = CWnd::FromHandlePermanent(lpCreateStruct->hwndParent);     // 0x28adc0, +0x18
//     m_pToolBar = (pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) ? (CMFCToolBar*)pParent : NULL;   // 0x234cf0, 0x3b15f8
//     if (m_pToolBar != NULL && m_pToolBar->IsKindOf(RUNTIME_CLASS(CMFCOutlookBarPane))) m_pToolBar = NULL;             // 0x3b1358
//     ModifyStyle(WS_BORDER, 0, 0);                                               // 0x2a96f0
//     if (m_Params.m_bBalloonTooltip) ModifyStyle(0, TTS_BALLOON, 0);              // +0x140
//     return 0;
int CMFCToolTipCtrl::OnCreate(CREATESTRUCTW* create) {
    if (!create || Default() == -1) return -1;
    auto* parent = static_cast<CWnd*>(CWnd::FromHandlePermanent(create->hwndParent));
    SetOwner(parent);
    CWnd::ModifyStyle(m_hWnd, WS_BORDER, m_Params.m_bBalloonTooltip ? TTS_BALLOON : 0, 0);
    return 0;
}

// Symbol: ?OnCreate@CMFCToolTipCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCToolTipCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCToolTipCtrl* tooltip, CREATESTRUCTW* create) {
    return tooltip ? tooltip->OnCreate(create) : -1;
}

// CMFCToolTipCtrl::OnEraseBkgnd(CDC* pDC) -- entry RVA 0x181f30 (mfc140u;
// ordinal 9780), transcribed:
//     CRect rect(0,0,0,0); ::GetClientRect(m_hWnd, &rect);
//     COLORREF clrText, clrLine;                       // never read afterwards (MSVC folded both into one slot)
//     OnFillBackground(pDC, rect, clrText, clrLine);   // vslot +0x2f8 (deviation (1))
//     return TRUE;
BOOL CMFCToolTipCtrl::OnEraseBkgnd(CDC* pDC) {
    RECT rc = { 0, 0, 0, 0 };
    ::GetClientRect(m_hWnd, &rc);
    unsigned long clrText = 0, clrLine = 0;
    OnFillBackground(pDC, CRect(rc), clrText, clrLine);
    return TRUE;
}

// Symbol: ?OnEraseBkgnd@CMFCToolTipCtrl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCToolTipCtrl__IEAAHPEAVCDC___Z(CMFCToolTipCtrl* tooltip, CDC* dc) {
    return tooltip ? tooltip->OnEraseBkgnd(dc) : TRUE;
}

// CMFCToolTipCtrl::OnFillBackground(CDC* pDC, CRect rect, COLORREF& clrText,
// COLORREF& clrLine) -- entry RVA 0x1815e0 (mfc140u; ordinal 9866), transcribed:
//     if (m_Params.m_clrFill == (COLORREF)-1) {                                       // +0x164
//         CMFCVisualManager::GetInstance()->OnFillToolTip(pDC, this, rect, clrText, clrLine);   // 0x9774, vslot +0x5c0 (deviation (2))
//         return;
//     }
//     if (m_Params.m_clrFillGradient == (COLORREF)-1) {                               // +0x168
//         CBrush br(m_Params.m_clrFill);                                              // 0x2a4060
//         ::FillRect(pDC->m_hDC, &rect, (HBRUSH)br.m_hObject);                         // CDC::FillRect inlined
//         br.~CBrush();                                                               // 0x1c6f0 -> DeleteObject (0x2a3f60)
//         return;
//     }
//     CDrawingManager dm(*pDC);                                                       // inline: vftable 0x2e7188, m_dc
//     dm.FillGradient2(rect, m_Params.m_clrFillGradient, m_Params.m_clrFill,
//                      m_Params.m_nGradientAngle == -1 ? 90 : m_Params.m_nGradientAngle);   // 0x57dd0; +0x160
// (retail does not run ~CDrawingManager on that path -- it is trivial; the
// OpenMFC thunk only re-stores the vptr, so calling it changes nothing.)
void CMFCToolTipCtrl::OnFillBackground(CDC* pDC, CRect rect, COLORREF& text, COLORREF& line) {
    if (!pDC) return;
    const auto& P = m_Params;
    if (P.m_clrFill == static_cast<unsigned long>(-1)) {
        CMFCVisualManager* pVM = VisualManager();
        if (pVM) {
            impl__OnFillToolTip_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolTipCtrl__VCRect__AEAK3_Z(pVM, pDC, this, rect, text, line);
        } else {
            ::FillRect(pDC->m_hDC, rect, ::GetSysColorBrush(COLOR_INFOBK));
        }
        return;
    }
    if (P.m_clrFillGradient == static_cast<unsigned long>(-1)) {
        if (const HBRUSH brush = ::CreateSolidBrush(P.m_clrFill)) {
            ::FillRect(pDC->m_hDC, rect, brush);
            ::DeleteObject(brush);
        }
        return;
    }
    alignas(void*) unsigned char dmStorage[16] = {};   // CDrawingManager is 0x10 bytes (core/gdi/CDrawingManager.cpp:160)
    impl___0CDrawingManager__QEAA_AEAVCDC___Z(dmStorage, pDC);
    CRect r(rect);
    impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(dmStorage, &r, P.m_clrFillGradient, P.m_clrFill,
                                                            P.m_nGradientAngle == -1 ? 90 : P.m_nGradientAngle);
    impl___1CDrawingManager__UEAA_XZ(dmStorage);
}

// Symbol: ?OnFillBackground@CMFCToolTipCtrl@@UEAAXPEAVCDC@@VCRect@@AEAK2@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__AEAK2_Z(
        CMFCToolTipCtrl* tooltip, CDC* dc, CRect rect, COLORREF* text, COLORREF* line) {
    if (tooltip && text && line) tooltip->CMFCToolTipCtrl::OnFillBackground(dc, rect, *text, *line);
}

// CMFCToolTipCtrl::OnDrawBorder(CDC* pDC, CRect rect, COLORREF clrLine) -- RVA
// 0x181850 (mfc140u), transcribed:
//     int cx = m_sizeCornerRadius.cx / 2, cy = m_sizeCornerRadius.cy / 2;   // +0x188 / +0x18c, signed halves
//     if (cx == 0 || cy == 0) { pDC->Draw3dRect(&rect, clrLine, clrLine); return; }   // 0x2a5bc0
//     pDC->MoveTo(rect.left + cx, rect.top);                 // 0x2a30c0 -- the rounded outline uses the pen OnPaint selected
//     pDC->LineTo(rect.right - cx - 1, rect.top);            // 0x2a3120 x8 in this order
//     pDC->LineTo(rect.right - 1, rect.top + cy);
//     pDC->LineTo(rect.right - 1, rect.bottom - cy - 1);
//     pDC->LineTo(rect.right - cx - 1, rect.bottom - 1);
//     pDC->LineTo(rect.left + cx, rect.bottom - 1);
//     pDC->LineTo(rect.left, rect.bottom - cy - 1);
//     pDC->LineTo(rect.left, rect.top + cy);
//     pDC->LineTo(rect.left + cx, rect.top);
void CMFCToolTipCtrl::OnDrawBorder(CDC* pDC, CRect rect, COLORREF clrLine) {
    if (pDC == nullptr) return;   // deviation: retail has no NULL checks
    const int cx = m_sizeCornerRadius.cx / 2;
    const int cy = m_sizeCornerRadius.cy / 2;
    if (cx == 0 || cy == 0) {
        RECT rc = ToRECT(rect);
        impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rc, clrLine, clrLine);
        return;
    }
    impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, rect.left + cx, rect.top);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.right - cx - 1, rect.top);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.right - 1, rect.top + cy);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.right - 1, rect.bottom - cy - 1);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.right - cx - 1, rect.bottom - 1);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.left + cx, rect.bottom - 1);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.left, rect.bottom - cy - 1);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.left, rect.top + cy);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, rect.left + cx, rect.top);
}

// Symbol: ?OnDrawBorder@CMFCToolTipCtrl@@UEAAXPEAVCDC@@VCRect@@K@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__K_Z(CMFCToolTipCtrl* tooltip, CDC* dc, CRect rect, COLORREF line) {
    if (tooltip) tooltip->CMFCToolTipCtrl::OnDrawBorder(dc, rect, line);
}

// CMFCToolTipCtrl::OnDrawIcon(CDC* pDC, CRect rectImage) -- RVA 0x181980
// (mfc140u).  The toolbar arm is transcribed:
//     if (m_pRibbonButton != NULL) { ... see below ... }
//     if (m_pHotButton == NULL || m_pToolBarImages == NULL) return FALSE;      // +0x130 / +0x128
//     CAfxDrawState ds; m_pToolBarImages->PrepareDrawImage(ds, CSize(0,0), FALSE);   // 0x16c060 (result ignored)
//     BOOL  bLargeIcons = CMFCToolBar::m_bLargeIcons;   UINT nStyle = m_pHotButton->m_nStyle;   // 0x3be368, +0x28
//     BOOL  bText = m_pHotButton->m_bText;              BOOL bImage = m_pHotButton->m_bImage;   // +0xc, +0x10
//     CMFCToolBar::m_bLargeIcons = FALSE; m_pHotButton->m_bText = FALSE; m_pHotButton->m_bImage = TRUE; m_pHotButton->m_nStyle = 0;
//     m_pHotButton->CMFCToolBarButton::OnDraw(pDC, rectImage, m_pToolBarImages, TRUE, FALSE, FALSE, TRUE, TRUE);   // DIRECT call 0x15c910 (not virtual)
//     m_pHotButton->m_nStyle = nStyle; m_pHotButton->m_bText = bText; m_pHotButton->m_bImage = bImage; CMFCToolBar::m_bLargeIcons = bLargeIcons;
//     m_pToolBarImages->EndDrawImage(ds);                                       // 0x16c270
//     return TRUE;
// The ribbon arm (deviation (3), NOT transcribed -- OpenMFC's ribbon button is
// opaque and its DrawImage thunk is a placeholder) reads, in retail:
//     BOOL bDisabled = m_pRibbonButton->[+0x148]; m_pRibbonButton->[+0x148] = FALSE;
//     m_pRibbonButton->DrawImage(pDC, m_nRibbonImageType, rectImage);           // vslot +0x250
//     m_pRibbonButton->[+0x148] = bDisabled; return TRUE;
// Here that arm returns FALSE without drawing.
BOOL CMFCToolTipCtrl::OnDrawIcon(CDC* pDC, CRect rectImage) {
    if (m_pRibbonButton != nullptr) {
        return FALSE;   // deviation (3)
    }
    if (m_pHotButton == nullptr || m_pToolBarImages == nullptr) return FALSE;

    HBITMAP ds[3] = { nullptr, nullptr, nullptr };   // CAfxDrawState { hbmMono, hbmMonoOld, hbmOldGlyphs } (as controls/CMFCReBar.cpp models it)
    impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(m_pToolBarImages, ds, 0, FALSE);

    CMFCToolBarButton* pBtn = m_pHotButton;
    const int bLargeIcons = impl__m_bLargeIcons_CMFCToolBar__1HA;
    const int nStyle = RdInt(pBtn, kTbbOffStyle);
    const int bText  = RdInt(pBtn, kTbbOffText);
    const int bImage = RdInt(pBtn, kTbbOffImage);
    impl__m_bLargeIcons_CMFCToolBar__1HA = 0;
    WrInt(pBtn, kTbbOffText, 0);
    WrInt(pBtn, kTbbOffImage, 1);
    WrInt(pBtn, kTbbOffStyle, 0);

    impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
        pBtn, pDC, &rectImage, m_pToolBarImages, TRUE, FALSE, FALSE, TRUE, TRUE);

    WrInt(pBtn, kTbbOffStyle, nStyle);
    WrInt(pBtn, kTbbOffText, bText);
    WrInt(pBtn, kTbbOffImage, bImage);
    impl__m_bLargeIcons_CMFCToolBar__1HA = bLargeIcons;
    impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(m_pToolBarImages, ds);
    return TRUE;
}

// Symbol: ?OnDrawIcon@CMFCToolTipCtrl@@UEAAHPEAVCDC@@VCRect@@@Z
extern "C" int MS_ABI impl__OnDrawIcon_CMFCToolTipCtrl__UEAAHPEAVCDC__VCRect___Z(CMFCToolTipCtrl* tooltip, CDC* dc, CRect rect) {
    return tooltip && tooltip->CMFCToolTipCtrl::OnDrawIcon(dc, rect);
}

// CMFCToolTipCtrl::OnDrawLabel(CDC* pDC, CRect rect, BOOL bCalcOnly) -- entry
// RVA 0x181b00 (mfc140u; ordinal 9394), transcribed:
//     CSize sizeText(0,0);
//     CString strText; GetWindowText(strText);                                   // 0x28be00
//     strText.Replace(L"\t", L"    ");                                           // 0x12440
//     BOOL bDrawDescr = m_Params.m_bDrawDescription && !m_strDescription.IsEmpty();   // +0x148, +0x178
//     <afxGlobalData init gate>
//     CFont* pOldFont = pDC->SelectObject((m_Params.m_bBoldLabel && bDrawDescr) ? &afxGlobalData.fontBold : &afxGlobalData.fontTooltip);   // +0x150; CDC vslot +0x60; 0x3c17e8 / 0x3c17d8
//     if (strText.GetLength() > 0 && wcschr(strText, L'\n') != NULL) {          // multi-line (the `Find('\n') >= 0` idiom)
//         UINT nFlags = DT_NOPREFIX | (bCalcOnly ? DT_CALCRECT : 0);
//         sizeText.cy = pDC->DrawText(strText, strText.GetLength(), &rect, nFlags);   // CDC vslot +0xe0
//         sizeText.cx = rect.Width();                                            // rect as DT_CALCRECT left it
//     } else if (bCalcOnly) {
//         ::GetTextExtentPoint32W(pDC->m_hAttribDC, strText, strText.GetLength(), &sizeText);   // CDC::GetTextExtent inlined, +0x10
//     } else {
//         UINT nFlags = DT_SINGLELINE | DT_NOCLIP | (bDrawDescr ? 0 : DT_VCENTER) | (m_pRibbonButton != NULL ? DT_NOPREFIX : 0);   // 0x120 (+4) (|0x800)
//         sizeText.cy = pDC->DrawText(strText, strText.GetLength(), &rect, nFlags);
//         sizeText.cx = rect.Width();
//     }
//     pDC->SelectObject(pOldFont);                                               // CDC vslot +0x60
//     return sizeText;
CSize CMFCToolTipCtrl::OnDrawLabel(CDC* pDC, CRect rect, BOOL bCalcOnly) {
    CSize size(0, 0);
    if (!pDC) return size;

    CString strText;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(this, &strText);
    strText.Replace(L"\t", L"    ");
    const bool bDrawDescr = m_Params.m_bDrawDescription != 0 && !m_strDescription.IsEmpty();

    EnsureGlobalDataInitialized();
    CFont* pOldFont = SelectGlobalFont(pDC, (m_Params.m_bBoldLabel != 0 && bDrawDescr) ? kGdOffFontBoldHandle : kGdOffFontTooltipHandle);

    const wchar_t* psz = static_cast<const wchar_t*>(strText);
    const int nLen = strText.GetLength();
    RECT rc = ToRECT(rect);
    if (nLen > 0 && ::wcschr(psz, L'\n') != nullptr) {
        const unsigned int nFlags = DT_NOPREFIX | (bCalcOnly ? DT_CALCRECT : 0);
        size.cy = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, psz, nLen, &rc, nFlags);
        size.cx = rc.right - rc.left;
    } else if (bCalcOnly) {
        SIZE sz = { 0, 0 };
        ::GetTextExtentPoint32W(pDC->m_hAttribDC, psz, nLen, &sz);
        size = CSize(sz.cx, sz.cy);
    } else {
        unsigned int nFlags = DT_SINGLELINE | DT_NOCLIP;
        if (!bDrawDescr) nFlags |= DT_VCENTER;
        if (m_pRibbonButton != nullptr) nFlags |= DT_NOPREFIX;
        size.cy = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, psz, nLen, &rc, nFlags);
        size.cx = rc.right - rc.left;
    }
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    return size;
}

// Symbol: ?OnDrawLabel@CMFCToolTipCtrl@@UEAA?AVCSize@@PEAVCDC@@VCRect@@H@Z
extern "C" CSize* MS_ABI impl__OnDrawLabel_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(
        CMFCToolTipCtrl* tooltip, CSize* result, CDC* dc, CRect rect, BOOL calculate) {
    if (result) *result = tooltip ? tooltip->CMFCToolTipCtrl::OnDrawLabel(dc, rect, calculate) : CSize(0, 0);
    return result;
}

// CMFCToolTipCtrl::OnDrawDescription(CDC* pDC, CRect rect, BOOL bCalcOnly) --
// entry RVA 0x181d20 (mfc140u; ordinal 9356), transcribed:
//     if (!m_Params.m_bDrawDescription) return CSize(0,0);                       // +0x148
//     <afxGlobalData init gate>
//     CFont* pOldFont = pDC->SelectObject(&afxGlobalData.fontTooltip);           // CDC vslot +0x60; 0x3c17d8
//     int nFixedWidth = GetFixedWidth();                                         // 0x182250
//     if (nFixedWidth > 0 && m_sizeImage.cx <= 32) {                             // +0x180
//         rect.right = rect.left + nFixedWidth;
//         if (m_sizeImage.cx > 0 && m_Params.m_bDrawIcon)                        // +0x144
//             rect.right = rect.left - m_ptMargin.x - m_sizeImage.cx + nFixedWidth;   // +0x190
//     } else {
//         rect.right = rect.left + m_Params.m_nMaxDescrWidth;                    // +0x15c
//     }
//     UINT nFlags = DT_WORDBREAK | (bCalcOnly ? DT_CALCRECT : DT_NOCLIP);        // 0x110 + (bCalcOnly ? 0x300 : 0)
//     int nHeight = pDC->DrawText(m_strDescription, m_strDescription.GetLength(), &rect, nFlags);   // CDC vslot +0xe0
//     pDC->SelectObject(pOldFont);
//     return CSize(rect.Width(), nHeight);
CSize CMFCToolTipCtrl::OnDrawDescription(CDC* pDC, CRect rect, BOOL bCalcOnly) {
    CSize size(0, 0);
    if (!pDC) return size;
    if (m_Params.m_bDrawDescription == 0) return size;

    EnsureGlobalDataInitialized();
    CFont* pOldFont = SelectGlobalFont(pDC, kGdOffFontTooltipHandle);

    const int nFixedWidth = GetFixedWidth();
    RECT rc = ToRECT(rect);
    if (nFixedWidth > 0 && m_sizeImage.cx <= 32) {
        rc.right = rc.left + nFixedWidth;
        if (m_sizeImage.cx > 0 && m_Params.m_bDrawIcon != 0) {
            rc.right = rc.left - m_ptMargin.x - m_sizeImage.cx + nFixedWidth;
        }
    } else {
        rc.right = rc.left + m_Params.m_nMaxDescrWidth;
    }
    const unsigned int nFlags = DT_WORDBREAK | (bCalcOnly ? DT_CALCRECT : DT_NOCLIP);
    const int nHeight = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
        pDC, static_cast<const wchar_t*>(m_strDescription), m_strDescription.GetLength(), &rc, nFlags);
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    size = CSize(rc.right - rc.left, nHeight);
    return size;
}

// Symbol: ?OnDrawDescription@CMFCToolTipCtrl@@UEAA?AVCSize@@PEAVCDC@@VCRect@@H@Z
extern "C" CSize* MS_ABI impl__OnDrawDescription_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(
        CMFCToolTipCtrl* tooltip, CSize* result, CDC* dc, CRect rect, BOOL calculate) {
    if (result) *result = tooltip ? tooltip->CMFCToolTipCtrl::OnDrawDescription(dc, rect, calculate) : CSize(0, 0);
    return result;
}

// CMFCToolTipCtrl::OnDrawSeparator(CDC* pDC, int x1, int x2, int y) -- entry
// RVA 0x181e40 (mfc140u; ordinal 9596), transcribed:
//     pDC->MoveTo(x1, y);           // 0x2a30c0
//     pDC->LineTo(x2, y);           // tail jump to 0x2a3120
void CMFCToolTipCtrl::OnDrawSeparator(CDC* pDC, int x1, int x2, int y) {
    if (pDC == nullptr) return;   // deviation: retail has no NULL check
    impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x1, y);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, x2, y);
}

// Symbol: ?OnDrawSeparator@CMFCToolTipCtrl@@UEAAXPEAVCDC@@HHH@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCToolTipCtrl__UEAAXPEAVCDC__HHH_Z(CMFCToolTipCtrl* tooltip, CDC* dc, int x1, int x2, int y) {
    if (tooltip) tooltip->CMFCToolTipCtrl::OnDrawSeparator(dc, x1, x2, y);
}

void CMFCToolTipCtrl::DrawTooltip(CDC& dc) {
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(m_hWnd, &rect);
    RECT rectMargin = { 0, 0, 0, 0 };
    ::SendMessage(m_hWnd, kTTM_GETMARGIN, 0, reinterpret_cast<LPARAM>(&rectMargin));
    RECT rectText = rect;
    rectText.left   += rectMargin.left;
    rectText.top    += rectMargin.top;
    rectText.right  -= rectMargin.right;
    rectText.bottom -= rectMargin.bottom;
    ::InflateRect(&rectText, -m_ptMargin.x, -m_ptMargin.y);

    unsigned long clrLine = m_Params.m_clrBorder;
    if (clrLine == static_cast<unsigned long>(-1)) clrLine = ::GetSysColor(COLOR_INFOTEXT);
    unsigned long clrText = m_Params.m_clrText;
    if (clrText == static_cast<unsigned long>(-1)) clrText = ::GetSysColor(COLOR_INFOTEXT);

    const int saved = ::SaveDC(dc.m_hDC);
    OnFillBackground(&dc, CRect(rect), clrText, clrLine);

    const HPEN pen = saved ? ::CreatePen(PS_SOLID, 1, clrLine) : nullptr;
    if (pen) ::SelectObject(dc.m_hDC, pen);
    OnDrawBorder(&dc, CRect(rect), clrLine);

    if ((m_sizeImage.cx != 0 || m_sizeImage.cy != 0) && m_Params.m_bDrawIcon != 0) {
        CRect rectImage(rectText);
        rectImage.right = rectImage.left + m_sizeImage.cx;
        rectImage.bottom = rectImage.top + m_sizeImage.cy;
        OnDrawIcon(&dc, rectImage);
        rectText.left += m_sizeImage.cx + m_ptMargin.x;
    }

    ::SetBkMode(dc.m_hDC, TRANSPARENT);
    ::SetTextColor(dc.m_hDC, clrText);

    const CSize sizeText = OnDrawLabel(&dc, CRect(rectText), FALSE);

    if (!m_strDescription.IsEmpty() && m_Params.m_bDrawDescription != 0) {
        CRect rectDescr(rectText);
        rectDescr.top += sizeText.cy + (3 * m_ptMargin.y) / 2;
        if (m_Params.m_bDrawSeparator != 0) {
            OnDrawSeparator(&dc, rectDescr.left, rectDescr.right, rectDescr.top - m_ptMargin.y / 2);
        }
        OnDrawDescription(&dc, rectDescr, FALSE);
    }

    if (saved) ::RestoreDC(dc.m_hDC, saved);
    if (pen) ::DeleteObject(pen);
}

void CMFCToolTipCtrl::OnPaint() {
    const HWND window = m_hWnd;
    PAINTSTRUCT paint{};
    const HDC handle = ::BeginPaint(window, &paint);
    if (!handle) return;
    CDC dc;
    dc.m_hDC = dc.m_hAttribDC = handle;
    DrawTooltip(dc);
    dc.m_hDC = dc.m_hAttribDC = nullptr;
    ::EndPaint(window, &paint);
}

// Symbol: ?OnPaint@CMFCToolTipCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCToolTipCtrl__IEAAXXZ(CMFCToolTipCtrl* tooltip) {
    if (!tooltip || !tooltip->GetSafeHwnd()) return;
    if (tooltip->GetParams().m_bBalloonTooltip) impl__Default_CWnd__IEAA_JXZ(tooltip);
    else tooltip->OnPaint();
}

// CMFCToolTipCtrl::OnPop(NMHDR*, LRESULT* pResult) -- entry RVA 0x181570
// (mfc140u; ordinal 10793), transcribed (pNMHDR is never read; m_pToolBar is
// NOT reset):
//     m_pHotButton = NULL; m_pToolBarImages = NULL;
//     m_strDescription.Empty();                    // 0x33b0
//     m_pRibbonButton = NULL;
//     m_ptLocation = CPoint(-1,-1);
//     m_nRibbonImageType = 0;
//     *pResult = 0;
void CMFCToolTipCtrl::OnPop(NMHDR*, LRESULT* pResult) {
    m_pHotButton = nullptr;
    m_pToolBarImages = nullptr;
    m_strDescription.Empty();
    m_pRibbonButton = nullptr;
    m_ptLocation = CPoint(-1, -1);
    m_nRibbonImageType = 0;
    if (pResult != nullptr) *pResult = 0;
}

// Symbol: ?OnPop@CMFCToolTipCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnPop_CMFCToolTipCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(CMFCToolTipCtrl* tooltip, NMHDR* notification, LRESULT* result) {
    if (tooltip) tooltip->OnPop(notification, result);
    else if (result) *result = 0;
}

// CMFCToolTipCtrl::OnShow(NMHDR*, LRESULT* pResult) -- entry RVA 0x181130
// (mfc140u; ordinal 11166), transcribed (pNMHDR is never read):
//     *pResult = 0;
//     if (m_Params.m_bVislManagerTheme) {                                                    // +0x158
//         CMFCVisualManager::GetInstance()->GetToolTipInfo(m_Params, (UINT)-1);             // 0x9774, vslot +0x5b8 (deviation (2))
//         m_Params.m_bVislManagerTheme = TRUE;
//     }
//     if (m_Params.m_bBalloonTooltip) return;                                                // +0x140
//     CPoint ptCursor(0,0); ::GetCursorPos(&ptCursor);
//     GetHotButton();                                                                        // 0x182110
//     m_sizeImage = m_Params.m_bDrawIcon ? GetIconSize() : CSize(0,0);                       // +0x144; vslot +0x2f0 (deviation (1))
//     m_ptMargin = CPoint(6, 4);                                                             // +0x190
//     CRect rectMargin(0,0,0,0); ::SendMessageW(m_hWnd, TTM_GETMARGIN, 0, (LPARAM)&rectMargin);   // read, never used afterwards
//     CRect rectClient(0,0,0,0); ::GetClientRect(m_hWnd, &rectClient);
//     CClientDC dc(this);                                                                    // 0x2a3b20
//     CSize sizeText = OnDrawLabel(&dc, rectClient, TRUE);                                   // vslot +0x310
//     int cx = sizeText.cx, cy = sizeText.cy;
//     CSize sizeDescr(0,0);
//     if (m_Params.m_bDrawDescription && !m_strDescription.IsEmpty()) {                      // +0x148, +0x178
//         sizeDescr = OnDrawDescription(&dc, rectClient, TRUE);                              // vslot +0x318
//         cx = max(cx, sizeDescr.cx);
//         cy = max(m_sizeImage.cy, cy + sizeDescr.cy + 2 * m_ptMargin.y);                   // +0x184, +0x194
//     } else {
//         cy = max(cy, m_sizeImage.cy);
//     }
//     if (m_sizeImage.cx > 0 && m_Params.m_bDrawIcon) cx += m_ptMargin.x + m_sizeImage.cx;   // +0x180
//     cx += 2 * m_ptMargin.x;  cy += 2 * m_ptMargin.y;
//     int nFixedWidth = GetFixedWidth();                                                     // 0x182250
//     if (nFixedWidth > 0 && sizeDescr != CSize(0,0)) cx = max(cx, nFixedWidth);
//     CRect rectWindow(0,0,0,0); ::GetWindowRect(m_hWnd, &rectWindow);
//     int x, y;
//     if (m_ptLocation == CPoint(-1,-1)) { x = rectWindow.left; y = rectWindow.top; }        // +0x198 / +0x19c
//     else                               { x = m_ptLocation.x; y = m_ptLocation.y; *pResult = 1; }
//     CRect rectScreen(0,0,0,0); MONITORINFO mi; mi.cbSize = sizeof(MONITORINFO);
//     HMONITOR hMon = ::MonitorFromPoint(rectWindow.TopLeft(), MONITOR_DEFAULTTONEAREST);
//     if (::GetMonitorInfoW(hMon, &mi)) ::CopyRect(&rectScreen, &mi.rcWork);
//     else ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &rectScreen, 0);
//     int nBottom = y + 2 + cy;
//     if (ptCursor.y + ::GetSystemMetrics(SM_CYCURSOR) + cy > nBottom) nBottom = ptCursor.y + ::GetSystemMetrics(SM_CYCURSOR) + cy;
//     if (nBottom > rectScreen.bottom) {
//         y = ptCursor.y - cy - 1;
//         if (m_pRibbonButton != NULL && m_ptLocation != CPoint(-1,-1)) {                    // +0x138
//             CMFCRibbonBar* pBar = m_pRibbonButton->GetTopLevelRibbonBar();                 // 0x10140
//             if (pBar != NULL && pBar->m_hWnd != NULL) { CRect rb; ::GetWindowRect(pBar->m_hWnd, &rb); y = rb.top - cy; }
//         }
//         *pResult = 1;
//     }
//     if (x + 2 + cx > rectScreen.right) {
//         if (*pResult == 1) x = ptCursor.x; else { x = rectScreen.right; *pResult = 1; }
//         x -= cx + 1;
//     }
//     if (*pResult != 1) { x = -1; y = -1; }
//     SetWindowPos(NULL, x, y, cx, cy, *pResult == 1 ? SWP_NOZORDER|SWP_NOACTIVATE : SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE);   // 0x2a9a60; 0x14 / 0x16
//     int r = ::GetWindowTheme(m_hWnd) != NULL ? 3 : 0;  m_sizeCornerRadius = CSize(r, r);   // +0x188
//     SetWindowPos(&CWnd::wndTop, -1, -1, -1, -1, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);   // 0x3c3370; 0x33
//     dc.~CClientDC();                                                                       // 0x2a3be0
// (retail evaluates GetSystemMetrics(SM_CYCURSOR) + cy + ptCursor.y twice, once
// for the compare and once for the assignment; it is computed once here.)
void CMFCToolTipCtrl::OnShow(NMHDR*, LRESULT* pResult) {
    if (!pResult || !m_hWnd) return;   // deviation: retail has no NULL checks
    *pResult = 0;

    if (m_Params.m_bVislManagerTheme != 0) {
        CMFCVisualManager* pVM = VisualManager();
        if (pVM != nullptr) {   // deviation (2): retail creates the default manager here
            impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
                pVM, m_Params, static_cast<unsigned int>(-1));
        }
        m_Params.m_bVislManagerTheme = TRUE;
    }
    if (m_Params.m_bBalloonTooltip != 0) return;

    POINT ptCursor = { 0, 0 };
    ::GetCursorPos(&ptCursor);
    GetHotButton();
    if (m_Params.m_bDrawIcon != 0) {
        m_sizeImage = GetIconSize();
    } else {
        m_sizeImage = CSize(0, 0);
    }
    m_ptMargin = CPoint(6, 4);

    RECT rectMargin = { 0, 0, 0, 0 };
    ::SendMessage(m_hWnd, kTTM_GETMARGIN, 0, reinterpret_cast<LPARAM>(&rectMargin));
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(m_hWnd, &rectClient);

    alignas(void*) unsigned char dcStorage[sizeof(CClientDC)] = {};
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, this);

    const CSize sizeText = OnDrawLabel(pDC, CRect(rectClient), TRUE);
    int cx = sizeText.cx;
    int cy = sizeText.cy;
    CSize sizeDescr(0, 0);
    if (m_Params.m_bDrawDescription != 0 && !m_strDescription.IsEmpty()) {
        sizeDescr = OnDrawDescription(pDC, CRect(rectClient), TRUE);
        const int cyLabelDescr = cy + sizeDescr.cy + 2 * m_ptMargin.y;
        if (sizeDescr.cx > cx) cx = sizeDescr.cx;
        cy = m_sizeImage.cy;
        if (cyLabelDescr > cy) cy = cyLabelDescr;
    } else {
        if (m_sizeImage.cy > cy) cy = m_sizeImage.cy;
    }
    if (m_sizeImage.cx > 0 && m_Params.m_bDrawIcon != 0) cx += m_ptMargin.x + m_sizeImage.cx;
    cx += 2 * m_ptMargin.x;
    cy += 2 * m_ptMargin.y;
    const int nFixedWidth = GetFixedWidth();
    if (nFixedWidth > 0 && (sizeDescr.cx != 0 || sizeDescr.cy != 0)) {
        if (nFixedWidth > cx) cx = nFixedWidth;
    }

    RECT rectWindow = { 0, 0, 0, 0 };
    ::GetWindowRect(m_hWnd, &rectWindow);
    int x, y;
    if (m_ptLocation.x == -1 && m_ptLocation.y == -1) {
        x = rectWindow.left;
        y = rectWindow.top;
    } else {
        x = m_ptLocation.x;
        y = m_ptLocation.y;
        *pResult = 1;
    }

    RECT rectScreen = { 0, 0, 0, 0 };
    MONITORINFO mi;
    std::memset(&mi, 0, sizeof mi);
    mi.cbSize = sizeof(MONITORINFO);
    POINT ptWindow = { rectWindow.left, rectWindow.top };
    HMONITOR hMon = ::MonitorFromPoint(ptWindow, MONITOR_DEFAULTTONEAREST);
    if (::GetMonitorInfoW(hMon, &mi)) {
        rectScreen = mi.rcWork;
    } else {
        ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &rectScreen, 0);
    }

    int nBottom = y + 2 + cy;
    const int nCursorBottom = ptCursor.y + ::GetSystemMetrics(SM_CYCURSOR) + cy;
    if (nCursorBottom > nBottom) nBottom = nCursorBottom;
    if (nBottom > rectScreen.bottom) {
        y = ptCursor.y - cy - 1;
        if (m_pRibbonButton != nullptr && (m_ptLocation.x != -1 || m_ptLocation.y != -1)) {
            void* pBar = impl__GetTopLevelRibbonBar_CMFCRibbonBaseElement__QEBAPEAVCMFCRibbonBar__XZ(m_pRibbonButton);
            if (pBar != nullptr && HwndOf(pBar) != nullptr) {
                RECT rectBar = { 0, 0, 0, 0 };
                ::GetWindowRect(HwndOf(pBar), &rectBar);
                y = rectBar.top - cy;
            }
        }
        *pResult = 1;
    }
    if (x + 2 + cx > rectScreen.right) {
        if (*pResult == 1) {
            x = ptCursor.x;
        } else {
            x = rectScreen.right;
            *pResult = 1;
        }
        x -= cx + 1;
    }
    if (*pResult != 1) {
        x = -1;
        y = -1;
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(this, nullptr, x, y, cx, cy,
        *pResult == 1 ? (SWP_NOZORDER | SWP_NOACTIVATE) : (SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE));

    const int nRadius = ::GetWindowTheme(m_hWnd) != nullptr ? 3 : 0;
    m_sizeCornerRadius = CSize(nRadius, nRadius);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(this, reinterpret_cast<const CWnd*>(impl__wndTop_CWnd__2V1_B), -1, -1, -1, -1,
                                                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    impl___1CClientDC__UEAA_XZ(pDC);
}

// Symbol: ?OnShow@CMFCToolTipCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolTipCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(CMFCToolTipCtrl* tooltip, NMHDR* notification, LRESULT* result) {
    if (tooltip) tooltip->OnShow(notification, result);
}
