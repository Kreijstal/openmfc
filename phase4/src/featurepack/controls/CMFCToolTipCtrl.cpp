// CMFCToolTipCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMemDCSupport.h"

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
// Object layout.  CMFCToolTipCtrl is not declared in the OpenMFC public headers
// (include/openmfc/afxmfc.h:330 only forward-declares it), so the client
// allocates it with the retail layout and this file is the only place that
// layout lives.  It is CToolTipCtrl -- CWnd (0xe8, pinned in afxwin.h) plus
// the CMapStringToPtr m_mapString that retail's ??0CToolTipCtrl (0x274c70)
// constructs at +0xe8 (0x38 bytes: vftable 0x32a8d8, m_nCount ... m_nBlockSize
// = 10 at +0x118), which OpenMFC models as CToolTipCtrl::_tooltip_padding[56]
// (afxole.h:149) -- followed by the members of the shipping afxtooltipctrl.h
// (read on this host at atlmfc/include/afxtooltipctrl.h; the names and their
// order below are quoted from it).  Every offset was read back out of the
// retail constructor ??0CMFCToolTipCtrl@@QEAA@PEAVCMFCToolTipInfo@@@Z (RVA
// 0x180be0, mfc140u): it calls ??0CToolTipCtrl (0x274c70), stores the
// CMFCToolTipCtrl vftable (0x31bc78), writes the thirteen CMFCToolTipInfo
// defaults at +0x140..+0x170 (0/1/1/0/1/1/0/200/-1/-1/-1/-1/-1), constructs a
// CString from the string manager's nil string at +0x178, zeroes
// +0x180/+0x188/+0x190/+0x198 (the CSize/CPoint member initialisers), calls
// SetParams(pParams) (0x181fc0) and then stores the body's initialisers:
// +0x120/+0x128/+0x130 = NULL, +0x180/+0x188/+0x190 = 0, +0x198 = the 64-bit
// -1 (m_ptLocation = CPoint(-1,-1)), +0x138 = NULL, +0x174 = 0, +0x1a0 = 0
// (one 8-byte store: both fixed widths).  ?CreateObject (0x180b90) allocates
// the object with `mov $0x1a8,%ecx; call ??2@YAPEAX_K@Z`, which pins sizeof
// == 0x1a8, agreeing with the m_nObjectSize 424 that
// detail/DeferredRttiSupport.h:62 already pins for the class descriptor.
//
// Deviations from retail, applied uniformly and marked at each site:
//
//  (1) vtable / virtual dispatch.  Retail's constructor stores the
//      CMFCToolTipCtrl vftable, and OnPaint (0x180db0), OnShow (0x181130)
//      and OnEraseBkgnd (0x181f30) reach GetIconSize / OnFillBackground /
//      OnDrawBorder / OnDrawIcon / OnDrawLabel / OnDrawDescription /
//      OnDrawSeparator VIRTUALLY through slots +0x2f0 / +0x2f8 / +0x300 /
//      +0x308 / +0x310 / +0x318 / +0x320 of it (slot numbers read from the
//      vftable at 0x31bc78 with vtu_full.py).  SetDescription (0x182090) is
//      itself virtual but makes no virtual call: it calls GetHotButton
//      (0x182110) directly.  This DLL's constructor runs CToolTipCtrl's placement constructor
//      (core/controls/CtorDtorPlacement.cpp), which leaves OpenMFC's own
//      (mingw-layout) CToolTipCtrl vtable in the object; those slots are not
//      the tooltip virtuals there, so the bodies below call the sibling
//      thunks in this file DIRECTLY.  A client subclass overriding one of
//      those virtuals is therefore NOT honoured -- the same gap
//      controls/CMFCMaskedEdit.cpp documents for IsMaskedChar.
//
//  (2) the visual manager.  Retail's inlined CMFCVisualManager::GetInstance
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
extern "C" void* MS_ABI impl___0CToolTipCtrl__QEAA_XZ(void* pThis);                            // core/controls/CtorDtorPlacement.cpp:22
extern "C" void  MS_ABI impl___1CToolTipCtrl__UEAA_XZ(void* pThis);                            // core/controls/CtorDtorPlacement.cpp:28
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                 // detail/MemcoreSupport.cpp:5
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                           // core/window/Thunks.cpp:1182
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);     // core/window/CWnd.cpp:926
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass); // core/runtime/CObject.cpp:49
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();          // toolbar/RuntimeClasses.cpp:97
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ();   // outlookbar/RuntimeClasses.cpp:144
extern "C" int   MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags); // core/window/CWnd.cpp:293
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp:868
extern "C" void  MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString); // core/window/CWnd.cpp:898
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);  // core/gdi/CPaintDC.cpp:12
extern "C" void  MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                            // core/gdi/CPaintDC.cpp:30
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd); // core/gdi/CClientDC.cpp:11
extern "C" void  MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);                          // core/gdi/CClientDC.cpp:26
extern "C" CPen* MS_ABI impl___0CPen__QEAA_HHK_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned long crColor); // core/gdi/CPen.cpp:64
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_K_Z(CBrush* pThis, unsigned long crColor);      // core/gdi/CBrush.cpp:12
extern "C" int   MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);             // core/gdi/CGdiObject.cpp:21
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject);    // core/gdi/CGdiObject.cpp:16
extern "C" CPen*  MS_ABI impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(CDC* pThis, CPen* pPen);       // core/gdi/CDC.cpp:425
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);    // core/gdi/CDC.cpp:444
extern "C" int   MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);                // core/gdi/CDC.cpp:80
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor); // core/gdi/CDC.cpp:224
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

// Thunks defined further down this file that earlier bodies call.
extern "C" void  MS_ABI impl__SetParams_CMFCToolTipCtrl__QEAAXPEAVCMFCToolTipInfo___Z(void* pThis, const void* pParams);
extern "C" void  MS_ABI impl__GetHotButton_CMFCToolTipCtrl__IEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__GetFixedWidth_CMFCToolTipCtrl__IEAAHXZ(void* pThis);
extern "C" CSize* MS_ABI impl__GetIconSize_CMFCToolTipCtrl__UEAA_AVCSize__XZ(void* pThis, CSize* pRet);
extern "C" void  MS_ABI impl__OnFillBackground_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__AEAK2_Z(void* pThis, CDC* pDC, CRect rect, unsigned long* pClrText, unsigned long* pClrLine);
extern "C" void  MS_ABI impl__OnDrawBorder_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__K_Z(void* pThis, CDC* pDC, CRect rect, unsigned long clrLine);
extern "C" int   MS_ABI impl__OnDrawIcon_CMFCToolTipCtrl__UEAAHPEAVCDC__VCRect___Z(void* pThis, CDC* pDC, CRect rectImage);
extern "C" CSize* MS_ABI impl__OnDrawLabel_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(void* pThis, CSize* pRet, CDC* pDC, CRect rect, int bCalcOnly);
extern "C" CSize* MS_ABI impl__OnDrawDescription_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(void* pThis, CSize* pRet, CDC* pDC, CRect rect, int bCalcOnly);
extern "C" void  MS_ABI impl__OnDrawSeparator_CMFCToolTipCtrl__UEAAXPEAVCDC__HHH_Z(void* pThis, CDC* pDC, int x1, int x2, int y);

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

// ---- CMFCToolTipInfo, as retail lays it out ---------------------------------
// Thirteen 4-byte members, 0x34 bytes; names and order from afxtooltipctrl.h,
// offsets from the stores in ??0CMFCToolTipCtrl (0x180be0) / SetParams
// (0x181fc0).  OpenMFC's public CMFCToolTipInfo (afxmfc.h:1598) is an opaque
// 96-byte blob, so it is NOT used for the embedded member: this view is.
struct TT_Params {
    int m_bBalloonTooltip;      // +0x00  ctor: FALSE
    int m_bDrawIcon;            // +0x04  ctor: TRUE
    int m_bDrawDescription;     // +0x08  ctor: TRUE
    int m_bRoundedCorners;      // +0x0c  ctor: FALSE (obsolete)
    int m_bBoldLabel;           // +0x10  ctor: TRUE
    int m_bDrawSeparator;       // +0x14  ctor: TRUE
    int m_bVislManagerTheme;    // +0x18  ctor: FALSE
    int m_nMaxDescrWidth;       // +0x1c  ctor: 200
    int m_nGradientAngle;       // +0x20  ctor: -1
    unsigned long m_clrFill;         // +0x24  ctor: (COLORREF)-1
    unsigned long m_clrFillGradient; // +0x28  ctor: (COLORREF)-1
    unsigned long m_clrText;         // +0x2c  ctor: (COLORREF)-1
    unsigned long m_clrBorder;       // +0x30  ctor: (COLORREF)-1
};
static_assert(sizeof(TT_Params) == 0x34, "CMFCToolTipInfo is 13 dwords in retail");

// ---- layout view -----------------------------------------------------------
// Offsets read from ??0CMFCToolTipCtrl (0x180be0) / ??1CMFCToolTipCtrl
// (0x180d50) / OnPop (0x181570); names and order from the shipping
// afxtooltipctrl.h.
struct TT_Layout {
    unsigned char       _base[0x120];       // CToolTipCtrl (CWnd + m_mapString)
    CMFCToolBar*        m_pToolBar;         // 0x120  ctor: NULL; OnCreate: the parent, if it is a CMFCToolBar
    CMFCToolBarImages*  m_pToolBarImages;   // 0x128  ctor: NULL; GetHotButton
    CMFCToolBarButton*  m_pHotButton;       // 0x130  ctor: NULL; GetHotButton
    void*               m_pRibbonButton;    // 0x138  CMFCRibbonButton*; ctor: NULL
    TT_Params           m_Params;           // 0x140..0x173
    int                 m_nRibbonImageType; // 0x174  ctor: 0 (RibbonImageLarge)
    CString             m_strDescription;   // 0x178  ctor: nil string
    CSize               m_sizeImage;        // 0x180  ctor: 0,0
    CSize               m_sizeCornerRadius; // 0x188  ctor: 0,0
    CPoint              m_ptMargin;         // 0x190  ctor: 0,0; OnShow: 6,4
    CPoint              m_ptLocation;       // 0x198  ctor: -1,-1
    int                 m_nFixedWidthRegular;   // 0x1a0  ctor: 0
    int                 m_nFixedWidthWithImage; // 0x1a4  ctor: 0
};
static_assert(sizeof(TT_Layout) == 0x1a8, "CMFCToolTipCtrl is 0x1a8 bytes in retail (CreateObject's operator new size)");
static_assert(offsetof(TT_Layout, m_pToolBar) == 0x120, "m_pToolBar @0x120");
static_assert(offsetof(TT_Layout, m_pToolBarImages) == 0x128, "m_pToolBarImages @0x128");
static_assert(offsetof(TT_Layout, m_pHotButton) == 0x130, "m_pHotButton @0x130");
static_assert(offsetof(TT_Layout, m_pRibbonButton) == 0x138, "m_pRibbonButton @0x138");
static_assert(offsetof(TT_Layout, m_Params) == 0x140, "m_Params @0x140");
static_assert(offsetof(TT_Layout, m_nRibbonImageType) == 0x174, "m_nRibbonImageType @0x174");
static_assert(offsetof(TT_Layout, m_strDescription) == 0x178, "m_strDescription @0x178");
static_assert(offsetof(TT_Layout, m_sizeImage) == 0x180, "m_sizeImage @0x180");
static_assert(offsetof(TT_Layout, m_sizeCornerRadius) == 0x188, "m_sizeCornerRadius @0x188");
static_assert(offsetof(TT_Layout, m_ptMargin) == 0x190, "m_ptMargin @0x190");
static_assert(offsetof(TT_Layout, m_ptLocation) == 0x198, "m_ptLocation @0x198");
static_assert(offsetof(TT_Layout, m_nFixedWidthRegular) == 0x1a0, "m_nFixedWidthRegular @0x1a0");
static_assert(offsetof(TT_Layout, m_nFixedWidthWithImage) == 0x1a4, "m_nFixedWidthWithImage @0x1a4");

inline TT_Layout* TT(void* p) { return static_cast<TT_Layout*>(p); }
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

// ---------------------------------------------------------------------------
// CMFCToolTipCtrl::CMFCToolTipCtrl(CMFCToolTipInfo* pParams) -- RVA 0x180be0
// (mfc140u), transcribed (see the layout note in the file header for the
// store-by-store reading):
//     CToolTipCtrl::CToolTipCtrl();                        // 0x274c70
//     vftable = 0x31bc78;                                  // deviation (1): OpenMFC's CToolTipCtrl vtable stays
//     m_Params = CMFCToolTipInfo();                        // the 13 defaults, +0x140..+0x170
//     m_strDescription = <nil string>;                     // +0x178
//     m_sizeImage = m_sizeCornerRadius = m_ptMargin = m_ptLocation = 0;   // member initialisers
//     SetParams(pParams);                                  // 0x181fc0
//     m_pToolBar = m_pToolBarImages = m_pHotButton = NULL;
//     m_sizeImage = m_sizeCornerRadius = m_ptMargin = CSize(0,0);
//     m_ptLocation = CPoint(-1,-1);                        // one 64-bit store of -1 at +0x198
//     m_pRibbonButton = NULL; m_nRibbonImageType = 0;
//     m_nFixedWidthRegular = m_nFixedWidthWithImage = 0;   // one 64-bit store of 0 at +0x1a0
// The CMFCToolTipInfo() defaults that the member initialiser writes are
// overwritten by SetParams in every case (NULL -> the same defaults, else the
// caller's copy), so they are written once here, by SetParams.
// Symbol: ??0CMFCToolTipCtrl@@QEAA@PEAVCMFCToolTipInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCToolTipCtrl__QEAA_PEAVCMFCToolTipInfo___Z(void* pThis, void* pInfo) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no NULL check
    impl___0CToolTipCtrl__QEAA_XZ(pThis);
    TT_Layout* p = TT(pThis);
    p->m_pToolBar = nullptr;
    p->m_pToolBarImages = nullptr;
    p->m_pHotButton = nullptr;
    p->m_pRibbonButton = nullptr;
    p->m_nRibbonImageType = 0;
    new (&p->m_strDescription) CString();
    p->m_sizeImage = CSize(0, 0);
    p->m_sizeCornerRadius = CSize(0, 0);
    p->m_ptMargin = CPoint(0, 0);
    p->m_ptLocation = CPoint(-1, -1);
    p->m_nFixedWidthRegular = 0;
    p->m_nFixedWidthWithImage = 0;
    impl__SetParams_CMFCToolTipCtrl__QEAAXPEAVCMFCToolTipInfo___Z(pThis, pInfo);
    return pThis;
}

// CMFCToolTipCtrl::~CMFCToolTipCtrl() -- RVA 0x180d50 (mfc140u), transcribed:
//     vftable = 0x31bc78;                                  // deviation (1): not rewritten
//     <release m_strDescription's CStringData: lock xadd on nRefs, Free through its manager at <= 0>
//     CToolTipCtrl::~CToolTipCtrl();                       // tail jump to 0x274de0
// Symbol: ??1CMFCToolTipCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolTipCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    TT(pThis)->m_strDescription.~CString();
    impl___1CToolTipCtrl__UEAA_XZ(pThis);
}

// CMFCToolTipCtrl::CreateObject() -- RVA 0x180b90 (mfc140u), transcribed:
//     void* p = operator new(0x1a8);                       // ??2@YAPEAX_K@Z (0x27f0)
//     if (p != NULL) new (p) CMFCToolTipCtrl(NULL);        // 0x180be0
//     return p;
// Symbol: ?CreateObject@CMFCToolTipCtrl@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolTipCtrl__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(TT_Layout));
    if (p != nullptr) {
        impl___0CMFCToolTipCtrl__QEAA_PEAVCMFCToolTipInfo___Z(p, nullptr);
    }
    return p;
}

// CMFCToolTipCtrl::SetParams(CMFCToolTipInfo* pParams) -- entry RVA 0x181fc0
// (mfc140u; recovered by ordinal 13457), transcribed:
//     if (pParams == NULL) {  m_Params = CMFCToolTipInfo();  }   // the 13 inline defaults
//     else                 {  m_Params = *pParams;           }   // 13 dword copies, +0x00..+0x30
// The client's CMFCToolTipInfo has the 0x34-byte retail layout (afxtooltipctrl.h).
// Symbol: ?SetParams@CMFCToolTipCtrl@@QEAAXPEAVCMFCToolTipInfo@@@Z
extern "C" void MS_ABI impl__SetParams_CMFCToolTipCtrl__QEAAXPEAVCMFCToolTipInfo___Z(void* pThis, const void* pParams) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    TT_Params& P = TT(pThis)->m_Params;
    if (pParams == nullptr) {
        P.m_bBalloonTooltip   = FALSE;
        P.m_bDrawIcon         = TRUE;
        P.m_bDrawDescription  = TRUE;
        P.m_bRoundedCorners   = FALSE;
        P.m_bBoldLabel        = TRUE;
        P.m_bDrawSeparator    = TRUE;
        P.m_bVislManagerTheme = FALSE;
        P.m_nMaxDescrWidth    = 200;
        P.m_nGradientAngle    = -1;
        P.m_clrFill           = static_cast<unsigned long>(-1);
        P.m_clrFillGradient   = static_cast<unsigned long>(-1);
        P.m_clrText           = static_cast<unsigned long>(-1);
        P.m_clrBorder         = static_cast<unsigned long>(-1);
        return;
    }
    std::memcpy(&P, pParams, sizeof(TT_Params));
}

// CMFCToolTipCtrl::SetLocation(CPoint pt) -- entry RVA 0x182230 (mfc140u;
// ordinal 13334): `mov %rdx,0x198(%rcx)` -- m_ptLocation = pt.  CPoint by
// value is the packed 8-byte register (x low, y high).
// Symbol: ?SetLocation@CMFCToolTipCtrl@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__SetLocation_CMFCToolTipCtrl__QEAAXVCPoint___Z(void* pThis, long long ptPacked) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    TT(pThis)->m_ptLocation = CPoint(static_cast<int>(ptPacked & 0xffffffffll), static_cast<int>(ptPacked >> 32));
}

// CMFCToolTipCtrl::SetHotRibbonButton(CMFCRibbonButton* p) -- entry RVA
// 0x182240 (mfc140u; ordinal 13248): `mov %rdx,0x138(%rcx)` -- m_pRibbonButton = p.
// Symbol: ?SetHotRibbonButton@CMFCToolTipCtrl@@QEAAXPEAVCMFCRibbonButton@@@Z
extern "C" void MS_ABI impl__SetHotRibbonButton_CMFCToolTipCtrl__QEAAXPEAVCMFCRibbonButton___Z(void* pThis, void* pRibbonButton) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    TT(pThis)->m_pRibbonButton = pRibbonButton;
}

// CMFCToolTipCtrl::SetDescription(const CString strDescription) -- entry RVA
// 0x182090 (mfc140u; ordinal 13126), transcribed:
//     GetHotButton();                                          // 0x182110
//     m_strDescription = strDescription;                       // 0xde30 CSimpleStringT::operator=
//     m_strDescription.Replace(L"\t", L"    ");                // 0x12440, strings at 0x33f5c8 / 0x342f80
//     <destroy the by-value parameter: its CStringData refcount is released>   // 0x1820d4..0x1820f2
// The by-value CString arrives as a pointer to a temporary the CALLEE owns
// (MSVC x64 passes non-trivial classes by hidden pointer), so it is destroyed
// here exactly as retail destroys it.
// Symbol: ?SetDescription@CMFCToolTipCtrl@@UEAAXV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetDescription_CMFCToolTipCtrl__UEAAXV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pThis, CString* pStrDescription) {
    if (pThis == nullptr || pStrDescription == nullptr) return;   // deviation: retail has no NULL checks
    impl__GetHotButton_CMFCToolTipCtrl__IEAAXXZ(pThis);
    TT(pThis)->m_strDescription = *pStrDescription;
    TT(pThis)->m_strDescription.Replace(L"\t", L"    ");
    pStrDescription->~CString();
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
// Symbol: ?GetHotButton@CMFCToolTipCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__GetHotButton_CMFCToolTipCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    TT_Layout* p = TT(pThis);
    p->m_pHotButton = nullptr;
    p->m_pToolBarImages = nullptr;
    if (p->m_pRibbonButton != nullptr) return;
    CMFCToolBar* pBar = p->m_pToolBar;
    if (pBar == nullptr) return;

    POINT pt = { 0, 0 };
    ::GetCursorPos(&pt);
    ::ScreenToClient(AsWnd(pBar)->m_hWnd, &pt);
    const int nHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, CPoint(pt.x, pt.y));
    p->m_pHotButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, nHit);
    if (p->m_pHotButton == nullptr) return;

    if (RdInt(pBar, kTbOffLocked) != 0) {
        CMFCToolBarImages* pMenuLocked = TbImagesAt(pBar, kTbOffMenuImagesLocked);
        if (RdInt(pMenuLocked, kTiOffCount) > 0) {
            p->m_pToolBarImages = pMenuLocked;
        } else {
            CMFCToolBarImages* pLocked = TbImagesAt(pBar, kTbOffImagesLocked);
            p->m_pToolBarImages = RdInt(pLocked, kTiOffCount) > 0 ? pLocked : nullptr;
        }
    } else if (RdInt(p->m_pHotButton, kTbbOffUserButton) != 0) {
        p->m_pToolBarImages = static_cast<CMFCToolBarImages*>(impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA);
    } else {
        p->m_pToolBarImages = &impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A;
        if (RdInt(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A, kTiOffCount) <= 0) {
            p->m_pToolBarImages = &impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;
        }
    }
}

// CMFCToolTipCtrl::GetFixedWidth() -- RVA 0x182250 (mfc140u), transcribed:
//     <afxGlobalData init gate>                                                  // 0x3c1620, 0x6a790
//     double dblScale = afxGlobalData.m_bIsRibbonImageScale ? afxGlobalData.m_dblRibbonImageScale : 1.0;   // +0x2c0 / +0x2b8 / 0x34ff68
//     int nLargeImageWidth = (int)(dblScale * 32.0);                            // 0x350060, cvttsd2si
//     return m_sizeImage.cx > nLargeImageWidth ? m_nFixedWidthWithImage : m_nFixedWidthRegular;   // +0x180, cmovg +0x1a4 / +0x1a0
// Symbol: ?GetFixedWidth@CMFCToolTipCtrl@@IEAAHXZ
extern "C" int MS_ABI impl__GetFixedWidth_CMFCToolTipCtrl__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    EnsureGlobalDataInitialized();
    const int nLargeImageWidth = static_cast<int>(GlobalDataRibbonImageScale() * 32.0);
    const TT_Layout* p = TT(pThis);
    return p->m_sizeImage.cx > nLargeImageWidth ? p->m_nFixedWidthWithImage : p->m_nFixedWidthRegular;
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
// Symbol: ?GetIconSize@CMFCToolTipCtrl@@UEAA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetIconSize_CMFCToolTipCtrl__UEAA_AVCSize__XZ(void* pThis, CSize* pRet) {
    if (pRet == nullptr) return nullptr;   // deviation: retail has no NULL check
    *pRet = CSize(0, 0);
    if (pThis == nullptr) return pRet;   // deviation: retail has no NULL check
    const TT_Layout* p = TT(pThis);
    if (p->m_pRibbonButton != nullptr) {
        return pRet;   // deviation (3)
    }
    if (p->m_pHotButton == nullptr || p->m_pToolBarImages == nullptr) return pRet;
    if (RdInt(p->m_pToolBarImages, kTiOffCount) == 0) return pRet;
    if (TbbImage(p->m_pHotButton) < 0) return pRet;
    *pRet = RdSize(p->m_pToolBarImages, kTiOffSizeImage);
    return pRet;
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
// Symbol: ?OnCreate@CMFCToolTipCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCToolTipCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (pThis == nullptr || lpCreateStruct == nullptr) return -1;   // deviation: retail has no NULL checks
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis))) == -1) return -1;
    TT_Layout* p = TT(pThis);
    CWnd* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(lpCreateStruct->hwndParent);
    if (pParent != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        p->m_pToolBar = static_cast<CMFCToolBar*>(pParent);
    } else {
        p->m_pToolBar = nullptr;
    }
    if (p->m_pToolBar != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p->m_pToolBar, impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ())) {
        p->m_pToolBar = nullptr;
    }
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(AsWnd(pThis), WS_BORDER, 0, 0);
    if (p->m_Params.m_bBalloonTooltip != 0) {
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(AsWnd(pThis), 0, kTTS_BALLOON, 0);
    }
    return 0;
}

// CMFCToolTipCtrl::OnEraseBkgnd(CDC* pDC) -- entry RVA 0x181f30 (mfc140u;
// ordinal 9780), transcribed:
//     CRect rect(0,0,0,0); ::GetClientRect(m_hWnd, &rect);
//     COLORREF clrText, clrLine;                       // never read afterwards (MSVC folded both into one slot)
//     OnFillBackground(pDC, rect, clrText, clrLine);   // vslot +0x2f8 (deviation (1))
//     return TRUE;
// Symbol: ?OnEraseBkgnd@CMFCToolTipCtrl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCToolTipCtrl__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr) return TRUE;   // deviation: retail has no NULL check
    RECT rc = { 0, 0, 0, 0 };
    ::GetClientRect(HwndOf(pThis), &rc);
    unsigned long clrText = 0, clrLine = 0;
    impl__OnFillBackground_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__AEAK2_Z(pThis, pDC, CRect(rc), &clrText, &clrLine);
    return TRUE;
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
// Symbol: ?OnFillBackground@CMFCToolTipCtrl@@UEAAXPEAVCDC@@VCRect@@AEAK2@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__AEAK2_Z(void* pThis, CDC* pDC, CRect rect, unsigned long* pClrText, unsigned long* pClrLine) {
    if (pThis == nullptr || pDC == nullptr || pClrText == nullptr || pClrLine == nullptr) return;   // deviation: retail has no NULL checks
    const TT_Params& P = TT(pThis)->m_Params;
    if (P.m_clrFill == static_cast<unsigned long>(-1)) {
        CMFCVisualManager* pVM = VisualManager();
        if (pVM != nullptr) {   // deviation (2): retail creates the default manager here
            impl__OnFillToolTip_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolTipCtrl__VCRect__AEAK3_Z(pVM, pDC, pThis, rect, *pClrText, *pClrLine);
        }
        return;
    }
    if (P.m_clrFillGradient == static_cast<unsigned long>(-1)) {
        alignas(void*) unsigned char brushStorage[sizeof(CBrush)] = {};
        CBrush* pBrush = reinterpret_cast<CBrush*>(brushStorage);
        impl___0CBrush__QEAA_K_Z(pBrush, P.m_clrFill);
        RECT rc = ToRECT(rect);
        ::FillRect(pDC->m_hDC, &rc, static_cast<HBRUSH>(pBrush->m_hObject));
        impl__DeleteObject_CGdiObject__QEAAHXZ(pBrush);
        return;
    }
    alignas(void*) unsigned char dmStorage[16] = {};   // CDrawingManager is 0x10 bytes (core/gdi/CDrawingManager.cpp:160)
    impl___0CDrawingManager__QEAA_AEAVCDC___Z(dmStorage, pDC);
    CRect r(rect);
    impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(dmStorage, &r, P.m_clrFillGradient, P.m_clrFill,
                                                            P.m_nGradientAngle == -1 ? 90 : P.m_nGradientAngle);
    impl___1CDrawingManager__UEAA_XZ(dmStorage);
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
// Symbol: ?OnDrawBorder@CMFCToolTipCtrl@@UEAAXPEAVCDC@@VCRect@@K@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__K_Z(void* pThis, CDC* pDC, CRect rect, unsigned long clrLine) {
    if (pThis == nullptr || pDC == nullptr) return;   // deviation: retail has no NULL checks
    const TT_Layout* p = TT(pThis);
    const int cx = p->m_sizeCornerRadius.cx / 2;
    const int cy = p->m_sizeCornerRadius.cy / 2;
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
// Symbol: ?OnDrawIcon@CMFCToolTipCtrl@@UEAAHPEAVCDC@@VCRect@@@Z
extern "C" int MS_ABI impl__OnDrawIcon_CMFCToolTipCtrl__UEAAHPEAVCDC__VCRect___Z(void* pThis, CDC* pDC, CRect rectImage) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    TT_Layout* p = TT(pThis);
    if (p->m_pRibbonButton != nullptr) {
        return FALSE;   // deviation (3)
    }
    if (p->m_pHotButton == nullptr || p->m_pToolBarImages == nullptr) return FALSE;

    HBITMAP ds[3] = { nullptr, nullptr, nullptr };   // CAfxDrawState { hbmMono, hbmMonoOld, hbmOldGlyphs } (as controls/CMFCReBar.cpp models it)
    impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(p->m_pToolBarImages, ds, 0, FALSE);

    CMFCToolBarButton* pBtn = p->m_pHotButton;
    const int bLargeIcons = impl__m_bLargeIcons_CMFCToolBar__1HA;
    const int nStyle = RdInt(pBtn, kTbbOffStyle);
    const int bText  = RdInt(pBtn, kTbbOffText);
    const int bImage = RdInt(pBtn, kTbbOffImage);
    impl__m_bLargeIcons_CMFCToolBar__1HA = 0;
    WrInt(pBtn, kTbbOffText, 0);
    WrInt(pBtn, kTbbOffImage, 1);
    WrInt(pBtn, kTbbOffStyle, 0);

    impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
        pBtn, pDC, &rectImage, p->m_pToolBarImages, TRUE, FALSE, FALSE, TRUE, TRUE);

    WrInt(pBtn, kTbbOffStyle, nStyle);
    WrInt(pBtn, kTbbOffText, bText);
    WrInt(pBtn, kTbbOffImage, bImage);
    impl__m_bLargeIcons_CMFCToolBar__1HA = bLargeIcons;
    impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(p->m_pToolBarImages, ds);
    return TRUE;
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
// Symbol: ?OnDrawLabel@CMFCToolTipCtrl@@UEAA?AVCSize@@PEAVCDC@@VCRect@@H@Z
extern "C" CSize* MS_ABI impl__OnDrawLabel_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(void* pThis, CSize* pRet, CDC* pDC, CRect rect, int bCalcOnly) {
    if (pRet == nullptr) return nullptr;   // deviation: retail has no NULL check
    *pRet = CSize(0, 0);
    if (pThis == nullptr || pDC == nullptr) return pRet;   // deviation: retail has no NULL checks
    const TT_Layout* p = TT(pThis);

    CString strText;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(AsWnd(pThis), &strText);
    strText.Replace(L"\t", L"    ");
    const bool bDrawDescr = p->m_Params.m_bDrawDescription != 0 && !p->m_strDescription.IsEmpty();

    EnsureGlobalDataInitialized();
    CFont* pOldFont = SelectGlobalFont(pDC, (p->m_Params.m_bBoldLabel != 0 && bDrawDescr) ? kGdOffFontBoldHandle : kGdOffFontTooltipHandle);

    const wchar_t* psz = static_cast<const wchar_t*>(strText);
    const int nLen = strText.GetLength();
    RECT rc = ToRECT(rect);
    if (nLen > 0 && ::wcschr(psz, L'\n') != nullptr) {
        const unsigned int nFlags = DT_NOPREFIX | (bCalcOnly ? DT_CALCRECT : 0);
        pRet->cy = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, psz, nLen, &rc, nFlags);
        pRet->cx = rc.right - rc.left;
    } else if (bCalcOnly) {
        SIZE sz = { 0, 0 };
        ::GetTextExtentPoint32W(pDC->m_hAttribDC, psz, nLen, &sz);
        *pRet = CSize(sz.cx, sz.cy);
    } else {
        unsigned int nFlags = DT_SINGLELINE | DT_NOCLIP;
        if (!bDrawDescr) nFlags |= DT_VCENTER;
        if (p->m_pRibbonButton != nullptr) nFlags |= DT_NOPREFIX;
        pRet->cy = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, psz, nLen, &rc, nFlags);
        pRet->cx = rc.right - rc.left;
    }
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    return pRet;
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
// Symbol: ?OnDrawDescription@CMFCToolTipCtrl@@UEAA?AVCSize@@PEAVCDC@@VCRect@@H@Z
extern "C" CSize* MS_ABI impl__OnDrawDescription_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(void* pThis, CSize* pRet, CDC* pDC, CRect rect, int bCalcOnly) {
    if (pRet == nullptr) return nullptr;   // deviation: retail has no NULL check
    *pRet = CSize(0, 0);
    if (pThis == nullptr || pDC == nullptr) return pRet;   // deviation: retail has no NULL checks
    const TT_Layout* p = TT(pThis);
    if (p->m_Params.m_bDrawDescription == 0) return pRet;

    EnsureGlobalDataInitialized();
    CFont* pOldFont = SelectGlobalFont(pDC, kGdOffFontTooltipHandle);

    const int nFixedWidth = impl__GetFixedWidth_CMFCToolTipCtrl__IEAAHXZ(pThis);
    RECT rc = ToRECT(rect);
    if (nFixedWidth > 0 && p->m_sizeImage.cx <= 32) {
        rc.right = rc.left + nFixedWidth;
        if (p->m_sizeImage.cx > 0 && p->m_Params.m_bDrawIcon != 0) {
            rc.right = rc.left - p->m_ptMargin.x - p->m_sizeImage.cx + nFixedWidth;
        }
    } else {
        rc.right = rc.left + p->m_Params.m_nMaxDescrWidth;
    }
    const unsigned int nFlags = DT_WORDBREAK | (bCalcOnly ? DT_CALCRECT : DT_NOCLIP);
    const int nHeight = impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
        pDC, static_cast<const wchar_t*>(p->m_strDescription), p->m_strDescription.GetLength(), &rc, nFlags);
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);
    *pRet = CSize(rc.right - rc.left, nHeight);
    return pRet;
}

// CMFCToolTipCtrl::OnDrawSeparator(CDC* pDC, int x1, int x2, int y) -- entry
// RVA 0x181e40 (mfc140u; ordinal 9596), transcribed:
//     pDC->MoveTo(x1, y);           // 0x2a30c0
//     pDC->LineTo(x2, y);           // tail jump to 0x2a3120
// Symbol: ?OnDrawSeparator@CMFCToolTipCtrl@@UEAAXPEAVCDC@@HHH@Z
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCToolTipCtrl__UEAAXPEAVCDC__HHH_Z(void* pThis, CDC* pDC, int x1, int x2, int y) {
    (void)pThis;   // never read by retail
    if (pDC == nullptr) return;   // deviation: retail has no NULL check
    impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, x1, y);
    impl__LineTo_CDC__QEAAHHH_Z(pDC, x2, y);
}

// CMFCToolTipCtrl::OnPaint() -- entry RVA 0x180db0 (mfc140u; ordinal 10755),
// transcribed:
//     if (m_Params.m_bBalloonTooltip) { CWnd::OnPaint(); return; }              // +0x140; 0x290f10 = ?OnPaint@CWnd@@, whose body is
//                                                                                //   `if (m_pCtrlCont) { CPaintDC dc(this); m_pCtrlCont->OnPaint(&dc); } Default();`
//                                                                                // (the tested member is CWnd::m_pCtrlCont at +0xc8 -- CWnd::m_pCtrlSite
//                                                                                //  is +0xd0, as ?GetControlUnknown@CWnd@@ (0x2a92c0, mfc140u) shows -- and
//                                                                                //  the +0x140 slot it calls is ?OnPaint@COleControlContainer@@ in that
//                                                                                //  class's vftable at 0x32ada8)
//     CPaintDC dcPaint(this);                                                    // 0x2a3d20
//     CMemDC memDC(dcPaint, this);                                               // 0x69f50
//     CDC* pDC = &memDC.GetDC();                                                 // inline: m_bMemDC ? &m_dcMem : &m_dc
//     CRect rect(0,0,0,0); ::GetClientRect(m_hWnd, &rect);
//     CRect rectMargin(0,0,0,0); ::SendMessageW(m_hWnd, TTM_GETMARGIN, 0, (LPARAM)&rectMargin);
//     CRect rectText = rect;
//     rectText.left += rectMargin.left; rectText.top += rectMargin.top; rectText.right -= rectMargin.right; rectText.bottom -= rectMargin.bottom;
//     ::InflateRect(&rectText, -m_ptMargin.x, -m_ptMargin.y);                    // +0x190 / +0x194
//     COLORREF clrLine = m_Params.m_clrBorder == (COLORREF)-1 ? ::GetSysColor(COLOR_INFOTEXT) : m_Params.m_clrBorder;   // +0x170
//     COLORREF clrText = m_Params.m_clrText   == (COLORREF)-1 ? ::GetSysColor(COLOR_INFOTEXT) : m_Params.m_clrText;     // +0x16c
//     OnFillBackground(pDC, rect, clrText, clrLine);                             // vslot +0x2f8 (deviation (1), all seven below)
//     CPen penLine(PS_SOLID, 1, clrLine);                                        // 0x2a3f90
//     CPen* pOldPen = pDC->SelectObject(&penLine);                               // 0x2a2730
//     OnDrawBorder(pDC, rect, clrLine);                                          // vslot +0x300
//     if (m_sizeImage != CSize(0,0) && m_Params.m_bDrawIcon) {                   // +0x180/+0x184, +0x144
//         CRect rectImage = rectText; rectImage.right = rectImage.left + m_sizeImage.cx; rectImage.bottom = rectImage.top + m_sizeImage.cy;
//         OnDrawIcon(pDC, rectImage);                                            // vslot +0x308
//         rectText.left += m_sizeImage.cx + m_ptMargin.x;
//     }
//     pDC->SetBkMode(TRANSPARENT);                                               // 0x2a2860
//     pDC->SetTextColor(clrText);                                                // CDC vslot +0x70
//     CSize sizeText = OnDrawLabel(pDC, rectText, FALSE);                        // vslot +0x310
//     if (!m_strDescription.IsEmpty() && m_Params.m_bDrawDescription) {          // +0x178, +0x148
//         CRect rectDescr = rectText;
//         rectDescr.top += sizeText.cy + (3 * m_ptMargin.y) / 2;
//         if (m_Params.m_bDrawSeparator)                                         // +0x154
//             OnDrawSeparator(pDC, rectDescr.left, rectDescr.right, rectDescr.top - m_ptMargin.y / 2);   // vslot +0x320
//         OnDrawDescription(pDC, rectDescr, FALSE);                              // vslot +0x318
//     }
//     pDC->SelectObject(pOldPen);                                                // 0x2a2730
//     penLine.~CPen(); memDC.~CMemDC(); dcPaint.~CPaintDC();                     // 0x1c6f0, 0x6a380, 0x2a3dd0
// DEVIATION on the balloon path: OpenMFC's ?OnPaint@CWnd@@ thunk
// (core/window/Thunks.cpp:1577 -> CWnd::OnPaint at core/window/CWnd.cpp:3161)
// paints the update region white instead of calling Default(), so the
// Default() that retail's CWnd::OnPaint ends in is called directly here
// (m_pCtrlCont is not modeled -- OpenMFC's CWnd carries padding at +0xc8).
// Symbol: ?OnPaint@CMFCToolTipCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCToolTipCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    TT_Layout* p = TT(pThis);
    if (p->m_Params.m_bBalloonTooltip != 0) {
        impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
        return;
    }

    alignas(void*) unsigned char paintStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(paintStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, AsWnd(pThis));
    alignas(void*) unsigned char memStorage[sizeof(S_Cmemdc)] = {};
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, pPaintDC, AsWnd(pThis));
    CDC* pDC = pMemDC->m_bMemDC ? reinterpret_cast<CDC*>(&pMemDC->m_dcMem) : static_cast<CDC*>(pMemDC->m_dc);

    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(HwndOf(pThis), &rect);
    RECT rectMargin = { 0, 0, 0, 0 };
    ::SendMessage(HwndOf(pThis), kTTM_GETMARGIN, 0, reinterpret_cast<LPARAM>(&rectMargin));
    RECT rectText = rect;
    rectText.left   += rectMargin.left;
    rectText.top    += rectMargin.top;
    rectText.right  -= rectMargin.right;
    rectText.bottom -= rectMargin.bottom;
    ::InflateRect(&rectText, -p->m_ptMargin.x, -p->m_ptMargin.y);

    unsigned long clrLine = p->m_Params.m_clrBorder;
    if (clrLine == static_cast<unsigned long>(-1)) clrLine = ::GetSysColor(COLOR_INFOTEXT);
    unsigned long clrText = p->m_Params.m_clrText;
    if (clrText == static_cast<unsigned long>(-1)) clrText = ::GetSysColor(COLOR_INFOTEXT);

    impl__OnFillBackground_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__AEAK2_Z(pThis, pDC, CRect(rect), &clrText, &clrLine);

    alignas(void*) unsigned char penStorage[sizeof(CPen)] = {};
    CPen* pPenLine = reinterpret_cast<CPen*>(penStorage);
    impl___0CPen__QEAA_HHK_Z(pPenLine, PS_SOLID, 1, clrLine);
    CPen* pOldPen = impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, pPenLine);

    impl__OnDrawBorder_CMFCToolTipCtrl__UEAAXPEAVCDC__VCRect__K_Z(pThis, pDC, CRect(rect), clrLine);

    if ((p->m_sizeImage.cx != 0 || p->m_sizeImage.cy != 0) && p->m_Params.m_bDrawIcon != 0) {
        CRect rectImage(rectText);
        rectImage.right = rectImage.left + p->m_sizeImage.cx;
        rectImage.bottom = rectImage.top + p->m_sizeImage.cy;
        impl__OnDrawIcon_CMFCToolTipCtrl__UEAAHPEAVCDC__VCRect___Z(pThis, pDC, rectImage);
        rectText.left += p->m_sizeImage.cx + p->m_ptMargin.x;
    }

    impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);

    CSize sizeText(0, 0);
    impl__OnDrawLabel_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(pThis, &sizeText, pDC, CRect(rectText), FALSE);

    if (!p->m_strDescription.IsEmpty() && p->m_Params.m_bDrawDescription != 0) {
        CRect rectDescr(rectText);
        rectDescr.top += sizeText.cy + (3 * p->m_ptMargin.y) / 2;
        if (p->m_Params.m_bDrawSeparator != 0) {
            impl__OnDrawSeparator_CMFCToolTipCtrl__UEAAXPEAVCDC__HHH_Z(pThis, pDC, rectDescr.left, rectDescr.right,
                                                                       rectDescr.top - p->m_ptMargin.y / 2);
        }
        CSize sizeDescr(0, 0);
        impl__OnDrawDescription_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(pThis, &sizeDescr, pDC, rectDescr, FALSE);
    }

    impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, pOldPen);
    impl__DeleteObject_CGdiObject__QEAAHXZ(pPenLine);
    impl___1CMemDC__UEAA_XZ(pMemDC);
    impl___1CPaintDC__UEAA_XZ(pPaintDC);
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
// Symbol: ?OnPop@CMFCToolTipCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnPop_CMFCToolTipCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pNMHDR;
    if (pThis == nullptr) { if (pResult) *pResult = 0; return; }   // deviation: retail has no NULL check
    TT_Layout* p = TT(pThis);
    p->m_pHotButton = nullptr;
    p->m_pToolBarImages = nullptr;
    p->m_strDescription.Empty();
    p->m_pRibbonButton = nullptr;
    p->m_ptLocation = CPoint(-1, -1);
    p->m_nRibbonImageType = 0;
    if (pResult != nullptr) *pResult = 0;
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
// Symbol: ?OnShow@CMFCToolTipCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnShow_CMFCToolTipCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    (void)pNMHDR;
    if (pThis == nullptr || pResult == nullptr) return;   // deviation: retail has no NULL checks
    TT_Layout* p = TT(pThis);
    *pResult = 0;

    if (p->m_Params.m_bVislManagerTheme != 0) {
        CMFCVisualManager* pVM = VisualManager();
        if (pVM != nullptr) {   // deviation (2): retail creates the default manager here
            impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
                pVM, *reinterpret_cast<CMFCToolTipInfo*>(&p->m_Params), static_cast<unsigned int>(-1));
        }
        p->m_Params.m_bVislManagerTheme = TRUE;
    }
    if (p->m_Params.m_bBalloonTooltip != 0) return;

    POINT ptCursor = { 0, 0 };
    ::GetCursorPos(&ptCursor);
    impl__GetHotButton_CMFCToolTipCtrl__IEAAXXZ(pThis);
    if (p->m_Params.m_bDrawIcon != 0) {
        CSize sizeIcon(0, 0);
        impl__GetIconSize_CMFCToolTipCtrl__UEAA_AVCSize__XZ(pThis, &sizeIcon);
        p->m_sizeImage = sizeIcon;
    } else {
        p->m_sizeImage = CSize(0, 0);
    }
    p->m_ptMargin = CPoint(6, 4);

    RECT rectMargin = { 0, 0, 0, 0 };
    ::SendMessage(HwndOf(pThis), kTTM_GETMARGIN, 0, reinterpret_cast<LPARAM>(&rectMargin));
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(HwndOf(pThis), &rectClient);

    alignas(void*) unsigned char dcStorage[sizeof(CClientDC)] = {};
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, AsWnd(pThis));

    CSize sizeText(0, 0);
    impl__OnDrawLabel_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(pThis, &sizeText, pDC, CRect(rectClient), TRUE);
    int cx = sizeText.cx;
    int cy = sizeText.cy;
    CSize sizeDescr(0, 0);
    if (p->m_Params.m_bDrawDescription != 0 && !p->m_strDescription.IsEmpty()) {
        impl__OnDrawDescription_CMFCToolTipCtrl__UEAA_AVCSize__PEAVCDC__VCRect__H_Z(pThis, &sizeDescr, pDC, CRect(rectClient), TRUE);
        const int cyLabelDescr = cy + sizeDescr.cy + 2 * p->m_ptMargin.y;
        if (sizeDescr.cx > cx) cx = sizeDescr.cx;
        cy = p->m_sizeImage.cy;
        if (cyLabelDescr > cy) cy = cyLabelDescr;
    } else {
        if (p->m_sizeImage.cy > cy) cy = p->m_sizeImage.cy;
    }
    if (p->m_sizeImage.cx > 0 && p->m_Params.m_bDrawIcon != 0) cx += p->m_ptMargin.x + p->m_sizeImage.cx;
    cx += 2 * p->m_ptMargin.x;
    cy += 2 * p->m_ptMargin.y;
    const int nFixedWidth = impl__GetFixedWidth_CMFCToolTipCtrl__IEAAHXZ(pThis);
    if (nFixedWidth > 0 && (sizeDescr.cx != 0 || sizeDescr.cy != 0)) {
        if (nFixedWidth > cx) cx = nFixedWidth;
    }

    RECT rectWindow = { 0, 0, 0, 0 };
    ::GetWindowRect(HwndOf(pThis), &rectWindow);
    int x, y;
    if (p->m_ptLocation.x == -1 && p->m_ptLocation.y == -1) {
        x = rectWindow.left;
        y = rectWindow.top;
    } else {
        x = p->m_ptLocation.x;
        y = p->m_ptLocation.y;
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
        if (p->m_pRibbonButton != nullptr && (p->m_ptLocation.x != -1 || p->m_ptLocation.y != -1)) {
            void* pBar = impl__GetTopLevelRibbonBar_CMFCRibbonBaseElement__QEBAPEAVCMFCRibbonBar__XZ(p->m_pRibbonButton);
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
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pThis), nullptr, x, y, cx, cy,
        *pResult == 1 ? (SWP_NOZORDER | SWP_NOACTIVATE) : (SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE));

    const int nRadius = ::GetWindowTheme(HwndOf(pThis)) != nullptr ? 3 : 0;
    p->m_sizeCornerRadius = CSize(nRadius, nRadius);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pThis), reinterpret_cast<const CWnd*>(impl__wndTop_CWnd__2V1_B), -1, -1, -1, -1,
                                                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    impl___1CClientDC__UEAA_XZ(pDC);
}
