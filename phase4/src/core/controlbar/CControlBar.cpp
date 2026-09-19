// CControlBar — OpenMFC implementation.
// Sources: olecore.cpp
//
// Method: every body marked "transcribed from retail" below was decoded from
// the mfc140u.dll export (disas.py --u), the way core/ole/COleControl.cpp does.
// Handlers that are not in the RVA symbol map were located through the class's
// retail message map (AFX_MSGMAP at 0x3212a8, 32-byte entries from 0x3212c0,
// mfc140u) and the non-exported virtuals through the CDialogBar (0x321588),
// CStatusBar (0x322438), CToolBar (0x322a28) and CDockBar (0x321dc8) vftables,
// which all share CControlBar's slots 91..107 (CWnd owns 0..90):
//    91 CalcFixedLayout      92 CalcDynamicLayout   93 OnUpdateCmdUI (pure)
//    94 CalcInsideRect       95 DoPaint             96 DrawBorders
//    97 DrawGripper          98 DrawNCGripper       99 DrawThemedGripper
//   100 DrawNonThemedGripper 101 DelayShow         102 IsVisible
//   103 RecalcDelayShow (0x1d71f0)                 104 IsDockBar
//   105 OnBarStyleChange     106 SetStatusText(int) 107 SetStatusText(INT_PTR) (0x1d6450)
// CWnd slots the bodies go through: 31 OnToolHitTest, 69 PreTranslateMessage,
// 86 IsFrameWnd.
//
// Virtual dispatch: OpenMFC's CControlBar objects carry a mingw (Itanium)
// vtable whose slot numbering is not MSVC's, so wherever retail calls through a
// vtable slot the bodies below call the CControlBar impl__ thunk directly, or
// devirtualise on the CRuntimeClass where a correctly typed sibling thunk
// exists.  Every such site is marked "vslot NN" in its comment.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"
#include <uxtheme.h>
#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Sibling thunks (declared with the signature their mangled name describes;
// every definition was located with grep before use).
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                  // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                     // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);            // core/window/CWnd.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                      // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);     // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);   // core/window/Thunks.cpp
extern "C" int MS_ABI impl__IsTopParentActive_CWnd__QEBAHXZ(const CWnd* pThis);                       // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ActivateTopParent_CWnd__QEAAXXZ(CWnd* pThis);                             // core/window/Thunks.cpp
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis);                              // core/window/CWnd.cpp
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI);                                                 // core/window/CWnd.cpp
extern "C" int MS_ABI impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);       // core/window/CWnd.cpp
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);    // core/window/CWnd.cpp
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(CFrameWnd* pThis, MSG* pMsg); // core/frame/CFrameWnd.cpp
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);                                    // core/window/Thunks.cpp
extern "C" int MS_ABI impl__EnableToolTips_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                  // core/window/Thunks.cpp
extern "C" int MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult); // core/window/CWnd.cpp
extern "C" int MS_ABI impl__GrayCtlColor_CWnd__SAHPEAUHDC____PEAUHWND____IPEAUHBRUSH____K_Z(
    HDC hDC, HWND hWnd, unsigned int nCtlColor, HBRUSH hbrGray, unsigned long clrText);               // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect); // core/window/Thunks.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);      // core/window/CWnd.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                               // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ();              // core/controlbar/CToolBar.cpp
extern "C" __int64 MS_ABI impl__OnToolHitTest_CToolBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CToolBar* pThis, CPoint point, TOOLINFOW* pTI);                                             // core/controlbar/CToolBar.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ();              // core/controlbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialogBar__SAPEAUCRuntimeClass__XZ();            // core/controlbar/RuntimeClasses.cpp
extern "C" void MS_ABI impl__RemoveControlBar_CFrameWnd__QEAAXPEAVCControlBar___Z(CFrameWnd* pThis, CControlBar* pBar); // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__OnUpdateCmdUI_CToolBar__UEAAXPEAVCFrameWnd__H_Z(
    CToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler);                                     // core/controlbar/CToolBar.cpp
extern "C" void MS_ABI impl__OnUpdateCmdUI_CDialogBar__UEAAXPEAVCFrameWnd__H_Z(
    CDialogBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler);                                   // core/controlbar/CDialogBar.cpp
extern "C" void MS_ABI impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(
    CSize* pRet, CToolBar* pThis, int nLength, unsigned long dwMode);                                 // core/controlbar/CToolBar.cpp
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);     // core/gdi/CWindowDC.cpp
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);                                  // core/gdi/CWindowDC.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);        // core/gdi/CPaintDC.cpp
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                                    // core/gdi/CPaintDC.cpp
extern "C" int MS_ABI impl__ExcludeClipRect_CDC__QEAAHPEBUtagRECT___Z(CDC* pThis, const RECT* lpRect);   // core/gdi/CDC.cpp
extern "C" int MS_ABI impl__IntersectClipRect_CDC__QEAAHPEBUtagRECT___Z(CDC* pThis, const RECT* lpRect); // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXHHHHK_Z(CDC* pThis, int x, int y, int cx, int cy, unsigned long clr); // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(CDC* pThis, const RECT* lpRect, unsigned long clr);  // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
    void* lpLayout, HWND hWnd, const RECT* lpRect);                                                   // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp

// Thunks defined further down in this file that earlier bodies call.
extern "C" int MS_ABI impl__SetStatusText_CControlBar__UEAAHH_Z(CControlBar* pThis, int nHit);
extern "C" int MS_ABI impl__SetStatusText_CControlBar__UEAAH_J_Z(CControlBar* pThis, __int64 nHit);
extern "C" void MS_ABI impl__DrawBorders_CControlBar__UEAAXPEAVCDC__AEAVCRect___Z(CControlBar* pThis, CDC* pDC, CRect& rect);
extern "C" void MS_ABI impl__DrawGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect);
extern "C" void MS_ABI impl__DrawNCGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect);
extern "C" int MS_ABI impl__DrawNonThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect);
extern "C" int MS_ABI impl__DrawThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect__H_Z(CControlBar* pThis, CDC* pDC, const CRect& rect, int bNCArea);
extern "C" void MS_ABI impl__DoPaint_CControlBar__UEAAXPEAVCDC___Z(CControlBar* pThis, CDC* pDC);
extern "C" int MS_ABI impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* pThis);
extern "C" int MS_ABI impl__IsFloating_CControlBar__QEBAHXZ(const CControlBar* pThis);
extern "C" DWORD MS_ABI impl__RecalcDelayShow_CControlBar__UEAAKPEAUAFX_SIZEPARENTPARAMS___Z(CControlBar* pThis, void* lpLayout);
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam);
extern "C" void MS_ABI impl__ResetTimer_CControlBar__QEAAX_KI_Z(CControlBar* pThis, UINT64 nIDEvent, UINT uElapse);
extern "C" void* MS_ABI impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(void* pRet, CControlBar* pThis, int nLength, DWORD dwMode);

namespace {

// Retail CBRS_* values (afxres.h of the shipping MFC), as the disassembly
// tests them.  include/openmfc/afxole.h defines CBRS_TOP=1 / CBRS_FLOATING=0x10
// etc., which do NOT match the bits retail tests (e.g. DrawGripper masks
// 0x400001 and compares with 0x400000: CBRS_GRIPPER set, CBRS_FLOATING=0x1
// clear), so the bodies below use these constants and never the header macros.
constexpr DWORD kCBRS_FLOATING      = 0x0001;
constexpr DWORD kCBRS_SIZE_DYNAMIC  = 0x0004;
constexpr DWORD kCBRS_TOOLTIPS      = 0x0010;
constexpr DWORD kCBRS_FLYBY         = 0x0020;
constexpr DWORD kCBRS_BORDER_3D     = 0x0080;
constexpr DWORD kCBRS_BORDER_LEFT   = 0x0100;
constexpr DWORD kCBRS_BORDER_TOP    = 0x0200;
constexpr DWORD kCBRS_BORDER_RIGHT  = 0x0400;
constexpr DWORD kCBRS_BORDER_BOTTOM = 0x0800;
constexpr DWORD kCBRS_BORDER_ANY    = 0x0F00;
constexpr DWORD kCBRS_ALIGN_LEFT    = 0x1000;
constexpr DWORD kCBRS_ALIGN_TOP     = 0x2000;
constexpr DWORD kCBRS_ALIGN_RIGHT   = 0x4000;
constexpr DWORD kCBRS_ALIGN_BOTTOM  = 0x8000;
constexpr DWORD kCBRS_ALIGN_ANY     = 0xF000;
constexpr DWORD kCBRS_ORIENT_HORZ   = kCBRS_ALIGN_TOP | kCBRS_ALIGN_BOTTOM;   // 0xA000
constexpr DWORD kCBRS_ORIENT_VERT   = kCBRS_ALIGN_LEFT | kCBRS_ALIGN_RIGHT;   // 0x5000
constexpr DWORD kCBRS_GRIPPER       = 0x00400000;

// m_nStateFlags bits (CControlBar::StateFlags; retail tests them at +0x120).
constexpr int kDelayHide = 1;
constexpr int kDelayShow = 2;
constexpr int kStatusSet = 8;

// Timer ids retail passes to KillTimer/SetTimer (0xEC09 waits before showing
// fly-by status text, 0xEC0A re-checks whether it must go away).
constexpr UINT_PTR kTimerWait  = 0xEC09;
constexpr UINT_PTR kTimerCheck = 0xEC0A;

// MFC private messages / ids (afxpriv.h / afxres.h values, as retail encodes them).
constexpr UINT kWM_SETMESSAGESTRING = 0x0362;
constexpr UINT kWM_POPMESSAGESTRING = 0x0375;
constexpr UINT kAFX_IDS_IDLEMESSAGE = 0xE001;
constexpr UINT_PTR kHID_BASE_COMMAND = 0x00010000;
constexpr UINT_PTR kHID_BASE_CONTROL = 0x00050000;

// CalcDynamicLayout modes (afxext.h LM_*).
constexpr DWORD kLM_STRETCH  = 0x01;
constexpr DWORD kLM_HORZ     = 0x02;
constexpr DWORD kLM_MRUWIDTH = 0x04;
constexpr DWORD kLM_HORZDOCK = 0x08;
constexpr DWORD kLM_VERTDOCK = 0x10;

// REBAR theme parts retail draws with (vssym32.h RP_*).
constexpr int kRP_GRIPPER     = 1;
constexpr int kRP_GRIPPERVERT = 2;
constexpr int kRP_BACKGROUND  = 6;

// AUX_DATA::cxBorder2 / cyBorder2 (afxData + 0x10 / + 0x14, i.e. 0x3c32d0 /
// 0x3c32d4 in mfc140u).  The retail AUX_DATA constructor stores 2 to both (see
// the notes in featurepack/menu/CMFCPopupMenuBar.cpp); OpenMFC's exported
// afxData is a zero blob, so the values are folded in as literals.
constexpr int kCxBorder2 = 2;
constexpr int kCyBorder2 = 2;

// AUX_DATA colour / brush members retail reads (afxData + 0x28 hbrBtnFace,
// + 0x34 clrBtnShadow, + 0x38 clrBtnHilite, + 0x3c clrBtnText -- names per
// their use in OnCtlColor / DrawBorders / DrawNonThemedGripper, which match
// the MFC sources for those functions).  OpenMFC's afxData is never filled, so
// the live system values are used instead.  DEVIATION: retail reads a cached
// copy refreshed on WM_SYSCOLORCHANGE; these are always current.
inline COLORREF ClrBtnShadow() { return ::GetSysColor(COLOR_BTNSHADOW); }
inline COLORREF ClrBtnHilite() { return ::GetSysColor(COLOR_BTNHIGHLIGHT); }
inline COLORREF ClrBtnText()   { return ::GetSysColor(COLOR_BTNTEXT); }
inline HBRUSH   HbrBtnFace()   { return ::GetSysColorBrush(COLOR_BTNFACE); }

// Mirror of MFC's AFX_SIZEPARENTPARAMS (afxpriv.h), the struct WM_SIZEPARENT's
// lParam points at; the same shape featurepack/CMFC_misc_stubs.cpp uses for
// AfxRepositionWindow.  Retail reads hDWP at +0, rect at +8, sizeTotal at
// +0x18 and bStretch at +0x20.
struct SizeParentParams {
    HDWP hDWP;
    RECT rect;
    SIZE sizeTotal;
    BOOL bStretch;
};
static_assert(offsetof(SizeParentParams, hDWP) == 0x00, "AFX_SIZEPARENTPARAMS::hDWP");
static_assert(offsetof(SizeParentParams, rect) == 0x08, "AFX_SIZEPARENTPARAMS::rect");
static_assert(offsetof(SizeParentParams, sizeTotal) == 0x18, "AFX_SIZEPARENTPARAMS::sizeTotal");
static_assert(offsetof(SizeParentParams, bStretch) == 0x20, "AFX_SIZEPARENTPARAMS::bStretch");

// Members retail touches, pinned to the offsets the disassembly uses.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CControlBar, m_cyTopBorder) == 0xfc, "CControlBar::m_cyTopBorder");
static_assert(offsetof(CControlBar, m_cyBottomBorder) == 0x100, "CControlBar::m_cyBottomBorder");
static_assert(offsetof(CControlBar, m_hReBarTheme) == 0x118, "CControlBar::m_hReBarTheme");
static_assert(offsetof(CControlBar, m_nStateFlags) == 0x120, "CControlBar::m_nStateFlags");
static_assert(offsetof(CControlBar, m_dwStyle) == 0x124, "CControlBar::m_dwStyle");
static_assert(offsetof(CControlBar, m_pDockSite) == 0x130, "CControlBar::m_pDockSite");
static_assert(offsetof(CControlBar, m_pDockBar) == 0x138, "CControlBar::m_pDockBar");
static_assert(offsetof(CControlBar, m_pDockContext) == 0x140, "CControlBar::m_pDockContext");

// CWnd::m_hWndOwner lives at CWnd + 0xa0 (retail reads it in every GetOwner()
// inline: `m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)`).  OpenMFC's CWnd
// does not name it -- the slot is inside the zero-initialised _cwnd_padding2 --
// but MSVC clients inline CWnd::SetOwner and write it, so it is read here
// exactly as retail does, with the ::GetParent fallback.
constexpr size_t kOffHWndOwner = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kOffHWndOwner &&
              kOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "m_hWndOwner slot must lie inside CWnd's padding");
inline HWND OwnerHwnd(const CWnd* pWnd) {
    HWND hOwner = nullptr;
    std::memcpy(&hOwner, reinterpret_cast<const unsigned char*>(pWnd) + kOffHWndOwner, sizeof(hOwner));
    return hOwner != nullptr ? hOwner : ::GetParent(pWnd->m_hWnd);
}
inline CWnd* GetOwnerWnd(const CWnd* pWnd) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pWnd));
}

// Transcribed from retail AfxGetParentOwner, RVA 0x28e260 (mfc140u):
//     CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);                      // 0x28adc0
//     if (pWnd != NULL) {                                                // permanent window: its owner
//         CWnd* pOwner = CWnd::FromHandle(pWnd->m_hWndOwner ? pWnd->m_hWndOwner : ::GetParent(pWnd->m_hWnd));
//         return pOwner ? pOwner->m_hWnd : NULL;                         // i.e. that owner HWND
//     }
//     return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
inline HWND ParentOwnerHwnd(HWND hWnd) {
    CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd);
    if (pWnd != nullptr) {
        return OwnerHwnd(pWnd);
    }
    return (::GetWindowLongW(hWnd, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
}
// Transcribed from retail CWnd::GetTopLevelParent, RVA 0x28e2e0 (mfc140u):
//     if (this == NULL || m_hWnd == NULL) return NULL;
//     HWND hWndT = m_hWnd, h;
//     while ((h = AfxGetParentOwner(hWndT)) != NULL) hWndT = h;
//     return CWnd::FromHandle(hWndT);
// Kept as the HWND here (the bodies below only compare the results and pass
// them to ::IsWindowEnabled).  DO NOT route this through the exported thunk
// impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ (core/window/Thunks.cpp): it
// returns CWnd::GetTopLevelParent(), which in this tree is a `void*` holding
// the top-level HWND, not the CWnd* its mangled name promises, so reading
// m_hWnd through its result faults.
inline HWND TopLevelParentHwnd(const CWnd* pWnd) {
    if (pWnd == nullptr || pWnd->m_hWnd == nullptr) return nullptr;
    HWND hTop = pWnd->m_hWnd;
    for (HWND h = ParentOwnerHwnd(hTop); h != nullptr; h = ParentOwnerHwnd(h)) {
        hTop = h;
    }
    return hTop;
}

// Stand-ins for the AFX_MODULE_THREAD_STATE members retail keeps the fly-by
// status machinery in: m_nLastHit (+0x128) and m_pLastStatus (+0x130), both
// read through AfxGetModuleThreadState() (0x133a20).  OpenMFC's
// AFX_MODULE_THREAD_STATE (detail/ManualSmallStubImplementationsSupport.h)
// carries only m_nTempMapLock, so the two fields live here as thread-locals.
// DEVIATION: retail's copies are per module *and* thread; these are per thread
// only, which is the same thing while every classic control bar belongs to
// this module.  Retail's constructor initialises m_nLastHit to -1.
thread_local INT_PTR      t_nLastHit    = -1;
thread_local CControlBar* t_pLastStatus = nullptr;

inline int KindOf(const CControlBar* pThis, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pThis), pClass);
}
// IsKindOf(RUNTIME_CLASS(CToolBar)) || IsKindOf(RUNTIME_CLASS(CDockBar)) --
// the test OnCreate / OnDestroy / OnThemeChanged share (0x3227b0 is
// CToolBar's CRuntimeClass, 0x3218f8 CDockBar's, mfc140u).
inline bool IsToolBarOrDockBar(const CControlBar* pThis) {
    return KindOf(pThis, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ()) ||
           KindOf(pThis, impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ());
}

// vslot 93 (OnUpdateCmdUI, pure in CControlBar), devirtualised on the runtime
// class.  Only the siblings whose thunks carry a real parameter list are
// reachable: CToolBar and CDialogBar.  CStatusBar, CReBar, COleResizeBar and
// CDockBar still define their OnUpdateCmdUI thunk with an auto-generated
// `(void** p0, int p1)` list (see headerRequests), so for them -- and for any
// client-derived class -- the call is dropped.
inline void CallOnUpdateCmdUI(CControlBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (KindOf(pThis, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ())) {
        impl__OnUpdateCmdUI_CToolBar__UEAAXPEAVCFrameWnd__H_Z(static_cast<CToolBar*>(pThis), pTarget, bDisableIfNoHndler);
    } else if (KindOf(pThis, impl__GetThisClass_CDialogBar__SAPEAUCRuntimeClass__XZ())) {
        impl__OnUpdateCmdUI_CDialogBar__UEAAXPEAVCFrameWnd__H_Z(static_cast<CDialogBar*>(pThis), pTarget, bDisableIfNoHndler);
    }
}

// CWnd vslot 31 (OnToolHitTest), devirtualised on the runtime class: CToolBar
// is the only CControlBar sibling whose override carries a correctly typed
// thunk (CReBar's is a placeholder, CStatusBar / CDialogBar have none), so
// everything else goes to CWnd's.  DEVIATION: OpenMFC's CWnd thunk
// (core/window/CWnd.cpp) returns -1 whenever pTI is NULL, whereas retail's
// CWnd::OnToolHitTest hit-tests the child windows and only fills pTI when it
// is given one; the NULL-pTI callers below (OnHelpHitTest, OnTimer,
// OnLButtonDown, OnLButtonDblClk) therefore see "no tool" on a non-CToolBar.
inline __int64 CallOnToolHitTest(const CControlBar* pThis, CPoint point, TOOLINFOW* pTI) {
    if (KindOf(pThis, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ())) {
        return impl__OnToolHitTest_CToolBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(static_cast<const CToolBar*>(pThis), point, pTI);
    }
    return impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pThis, point, pTI);
}

// vslot 92 (CalcDynamicLayout), devirtualised: CToolBar overrides it with a
// correctly typed thunk; everything else gets CControlBar's.  (CReBar's
// override is still a placeholder -- see headerRequests.)
inline CSize CallCalcDynamicLayout(CControlBar* pThis, int nLength, DWORD dwMode) {
    CSize size(0, 0);
    if (KindOf(pThis, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ())) {
        impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(&size, static_cast<CToolBar*>(pThis), nLength, dwMode);
    } else {
        impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(&size, pThis, nLength, dwMode);
    }
    return size;
}

// vslot 69 (PreTranslateMessage) on the owner chain, devirtualised on
// IsFrameWnd: a frame owner goes through the CFrameWnd thunk (which dispatches
// to the repo's C++ overrides), anything else through CWnd's.
inline int OwnerPreTranslateMessage(CWnd* pOwner, MSG* pMsg) {
    if (impl__IsFrameWnd_CWnd__UEBAHXZ(pOwner)) {
        return impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(static_cast<CFrameWnd*>(pOwner), pMsg);
    }
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(pOwner, pMsg);
}

} // namespace

// Retail: vftable slot 104 of the CDialogBar / CStatusBar / CToolBar vftables
// (mfc140u) holds 0x71e0 = `xor %eax,%eax; ret`, i.e. the CControlBar default
// returns FALSE; CDockBar's vftable carries 0x3a60 = `mov $1,%eax; ret` there.
// The mfc140u export table resolves ?IsDockBar@CControlBar@@UEBAHXZ to that
// same ICF-folded 0x71e0 (shared with dozens of other exports, which is why
// the wf2 RVA symbol map lists it under another name).  This body is the
// complete retail behaviour, not a placeholder.
// Symbol: ?IsDockBar@CControlBar@@UEBAHXZ
extern "C" int MS_ABI impl__IsDockBar_CControlBar__UEBAHXZ(const CControlBar* /*pThis*/) {
    return FALSE;
}
// Symbol: ?IsFloating@CControlBar@@QEBAHXZ
extern "C" int MS_ABI impl__IsFloating_CControlBar__QEBAHXZ(const CControlBar* pThis) {
    return (pThis->m_dwStyle & CBRS_FLOATING) ? TRUE : FALSE;
}
// Symbol: ?IsVisible@CControlBar@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CControlBar__UEBAHXZ(const CControlBar* pThis) {
    return (pThis->m_hWnd && ::IsWindowVisible(pThis->m_hWnd)) ? TRUE : FALSE;
}
// Symbol: ?EnableDocking@CControlBar@@QEAAXK@Z
extern "C" void MS_ABI impl__EnableDocking_CControlBar__QEAAXK_Z(CControlBar* pThis, DWORD dwDockStyle) {
    pThis->m_dwDockStyle = dwDockStyle;
}
// Symbol: ?SetBarStyle@CControlBar@@QEAAXK@Z
extern "C" void MS_ABI impl__SetBarStyle_CControlBar__QEAAXK_Z(CControlBar* pThis, DWORD dwStyle) {
    pThis->m_dwStyle = dwStyle;
}
// Symbol: ?GetDockingFrame@CControlBar@@QEBAPEAVCFrameWnd@@XZ
extern "C" CFrameWnd* MS_ABI impl__GetDockingFrame_CControlBar__QEBAPEAVCFrameWnd__XZ(const CControlBar* pThis) {
    return static_cast<CFrameWnd*>(pThis->m_pDockSite);
}
// Symbol: ?SetBorders@CControlBar@@QEAAXHHHH@Z
extern "C" void MS_ABI impl__SetBorders_CControlBar__QEAAXHHHH_Z(CControlBar* pThis, int cxLeft, int cxRight, int cyTop, int cyBottom) {
    pThis->m_cxLeftBorder = cxLeft;
    pThis->m_cxRightBorder = cxRight;
    pThis->m_cyTopBorder = cyTop;
    pThis->m_cyBottomBorder = cyBottom;
}
// Symbol: ?SetInPlaceOwner@CControlBar@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetInPlaceOwner_CControlBar__QEAAXPEAVCWnd___Z(CControlBar* pThis, CWnd* pOwner) {
    pThis->m_pInPlaceOwner = pOwner;
}
// Transcribed from retail RVA 0x1d6e50 (mfc140u):
//     CWindowDC dc(this);                                            // 0x2a3c20
//     CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);      // IAT GetClientRect
//     CRect rectWindow;  ::GetWindowRect(m_hWnd, &rectWindow);      // IAT GetWindowRect
//     CWnd::ScreenToClient(&rectWindow);                             // 0x2a32b0 (RECT overload)
//     ::OffsetRect(&rectClient, -rectWindow.left, -rectWindow.top);  // IAT OffsetRect
//     dc.ExcludeClipRect(&rectClient);                               // 0x2a2ee0
//     ::OffsetRect(&rectWindow, -rectWindow.left, -rectWindow.top);
//     DrawBorders(&dc, rectWindow);                                  // vslot 96
//     dc.IntersectClipRect(&rectWindow);                             // 0x2a2fb0
//     ::SendMessage(m_hWnd, WM_ERASEBKGND, (WPARAM)dc.m_hDC, 0);     // IAT SendMessageW
//     DrawNCGripper(&dc, rectWindow);                                // vslot 98
// The two vslot calls go to this file's CControlBar thunks (see the file
// header); CWindowDC exists in this DLL only as its thunks, so it is built in
// raw storage the way featurepack/docking/CDockablePane.cpp does.
// Symbol: ?EraseNonClient@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__EraseNonClient_CControlBar__QEAAXXZ(CControlBar* pThis) {
    if (!pThis) return;
    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, pThis);

    CRect rectClient;
    ::GetClientRect(pThis->m_hWnd, static_cast<RECT*>(rectClient));
    CRect rectWindow;
    ::GetWindowRect(pThis->m_hWnd, static_cast<RECT*>(rectWindow));
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pThis, static_cast<RECT*>(rectWindow));
    ::OffsetRect(static_cast<RECT*>(rectClient), -rectWindow.left, -rectWindow.top);
    impl__ExcludeClipRect_CDC__QEAAHPEBUtagRECT___Z(pDC, static_cast<const RECT*>(rectClient));

    ::OffsetRect(static_cast<RECT*>(rectWindow), -rectWindow.left, -rectWindow.top);
    impl__DrawBorders_CControlBar__UEAAXPEAVCDC__AEAVCRect___Z(pThis, pDC, rectWindow);   // vslot 96

    impl__IntersectClipRect_CDC__QEAAHPEBUtagRECT___Z(pDC, static_cast<const RECT*>(rectWindow));
    ::SendMessage(pThis->m_hWnd, WM_ERASEBKGND, reinterpret_cast<WPARAM>(pDC->m_hDC), 0);

    impl__DrawNCGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(pThis, pDC, rectWindow); // vslot 98
    impl___1CWindowDC__UEAA_XZ(pDC);
}
// Retail: vftable slot 105 in the CDialogBar and CDockBar vftables (mfc140u)
// holds 0x27d0 = `ret`, i.e. the CControlBar default does nothing; CStatusBar
// (0x1da990) and CToolBar (0x1dcdf0) override the slot.  The mfc140u export
// table resolves ?OnBarStyleChange@CControlBar@@UEAAXKK@Z to that ICF-folded
// 0x27d0 (the wf2 RVA symbol map lists it under another export's name).  This
// empty body is the complete retail behaviour.
// Symbol: ?OnBarStyleChange@CControlBar@@UEAAXKK@Z
extern "C" void MS_ABI impl__OnBarStyleChange_CControlBar__UEAAXKK_Z(CControlBar* /*pThis*/, DWORD /*dwOldStyle*/, DWORD /*dwNewStyle*/) {
}
// Symbol: ?AllocElements@CControlBar@@QEAAHHH@Z
extern "C" int MS_ABI impl__AllocElements_CControlBar__QEAAHHH_Z(CControlBar* pThis, int nCount, int nSize) {
    if (nCount <= 0 || nSize <= 0) return FALSE;
    if (pThis->m_pData) free(pThis->m_pData);
    pThis->m_nCount = nCount;
    pThis->m_pData = (UINT*)calloc(nCount, nSize);
    return pThis->m_pData ? TRUE : FALSE;
}
// Symbol: ?CalcInsideRect@CControlBar@@UEBAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(const CControlBar* pThis, CRect& rect, int bHorz) {
    (void)bHorz;
    rect.left += pThis->m_cxLeftBorder;
    rect.right -= pThis->m_cxRightBorder;
    rect.top += pThis->m_cyTopBorder;
    rect.bottom -= pThis->m_cyBottomBorder;
}
// Symbol: ?DelayShow@CControlBar@@UEAAXH@Z
extern "C" void MS_ABI impl__DelayShow_CControlBar__UEAAXH_Z(CControlBar* pThis, int bShow) {
    if (pThis->m_hWnd) {
        ::ShowWindow(pThis->m_hWnd, bShow ? SW_SHOWNA : SW_HIDE);
    }
}
// Symbol: ?DestroyWindow@CControlBar@@UEAAHXZ
extern "C" int MS_ABI impl__DestroyWindow_CControlBar__UEAAHXZ(CControlBar* pThis) {
    if (!pThis->m_hWnd) return FALSE;
    BOOL result = ::DestroyWindow(pThis->m_hWnd);
    if (result) pThis->m_hWnd = nullptr;
    return result;
}
// Transcribed from retail RVA 0x1d74e0 (mfc140u):
//     CRect rect;  ::GetClientRect(m_hWnd, &rect);   // IAT GetClientRect
//     DrawBorders(pDC, rect);                         // vslot 96
//     DrawGripper(pDC, rect);                         // vslot 97
// Both vslot calls go to this file's CControlBar thunks (file header).
// Symbol: ?DoPaint@CControlBar@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CControlBar__UEAAXPEAVCDC___Z(CControlBar* pThis, CDC* pDC) {
    if (!pThis) return;
    CRect rect;
    ::GetClientRect(pThis->m_hWnd, static_cast<RECT*>(rect));
    impl__DrawBorders_CControlBar__UEAAXPEAVCDC__AEAVCRect___Z(pThis, pDC, rect);   // vslot 96
    impl__DrawGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(pThis, pDC, rect);   // vslot 97
}
// Transcribed from retail RVA 0x1d7570 (mfc140u):
//     DWORD dwStyle = m_dwStyle;                                     // +0x124
//     if (!(dwStyle & CBRS_BORDER_ANY /*0xf00*/)) return;
//     CRect rect1 = rect, rect2 = rect;
//     COLORREF clr = afxData.clrBtnShadow;                           // 0x3c32f4
//     if (dwStyle & CBRS_BORDER_3D /*0x80*/) { rect1.right -= 1; rect1.bottom -= 1; }
//     if (dwStyle & CBRS_BORDER_TOP /*0x200*/)    rect2.top    += afxData.cyBorder2;   // 0x3c32d4
//     if (dwStyle & CBRS_BORDER_BOTTOM /*0x800*/) rect2.bottom -= afxData.cyBorder2;
//     if (IsKindOf(RUNTIME_CLASS(CToolBar)) && (dwStyle & CBRS_BORDER_BOTTOM)) {   // 0x234cf0, 0x3227b0
//         CRect rectTheme(0, rect.top, rect.right, rect.bottom);
//         HRESULT hr = E_FAIL;   // (any failure below falls into the FillSolidRect)
//         if (m_hReBarTheme != NULL) {                                               // +0x118
//             if (::IsThemeBackgroundPartiallyTransparent(m_hReBarTheme, RP_BACKGROUND /*6*/, 0))
//                 ::DrawThemeParentBackground(m_hWnd, pDC->m_hDC, &rectTheme);
//             hr = ::DrawThemeBackground(m_hReBarTheme, pDC->m_hDC, RP_BACKGROUND, 0, &rectTheme, NULL);
//         }
//         if (FAILED(hr)) pDC->FillSolidRect(&rectTheme, ::GetBkColor(pDC->m_hAttribDC));   // 0x2a5aa0
//     }
//     if (dwStyle & CBRS_BORDER_LEFT /*0x100*/)  pDC->FillSolidRect(0, rect2.top, 1, rect2.Height(), clr);         // 0x2a5b20
//     if (dwStyle & CBRS_BORDER_TOP)             pDC->FillSolidRect(0, 0, rect.right, 1, clr);
//     if (dwStyle & CBRS_BORDER_RIGHT /*0x400*/) pDC->FillSolidRect(rect1.right, rect2.top, -1, rect2.Height(), clr);
//     if (dwStyle & CBRS_BORDER_BOTTOM)          pDC->FillSolidRect(0, rect1.bottom, rect.right, -1, clr);
//     if (dwStyle & CBRS_BORDER_3D) {
//         clr = afxData.clrBtnHilite;                                 // 0x3c32f8
//         if (dwStyle & CBRS_BORDER_LEFT)   pDC->FillSolidRect(1, rect2.top, 1, rect2.Height(), clr);
//         if (dwStyle & CBRS_BORDER_TOP)    pDC->FillSolidRect(0, 1, rect.right, 1, clr);
//         if (dwStyle & CBRS_BORDER_RIGHT)  pDC->FillSolidRect(rect.right, rect2.top, -1, rect2.Height(), clr);
//         if (dwStyle & CBRS_BORDER_BOTTOM) pDC->FillSolidRect(0, rect.bottom, rect.right, -1, clr);
//     }
//     if (dwStyle & CBRS_BORDER_LEFT)   rect.left   += afxData.cxBorder2;   // 0x3c32d0
//     if (dwStyle & CBRS_BORDER_TOP)    rect.top    += afxData.cyBorder2;
//     if (dwStyle & CBRS_BORDER_RIGHT)  rect.right  -= afxData.cxBorder2;
//     if (dwStyle & CBRS_BORDER_BOTTOM) rect.bottom -= afxData.cyBorder2;
// The afxData reads are replaced as described at the top of the file.
// Symbol: ?DrawBorders@CControlBar@@UEAAXPEAVCDC@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__DrawBorders_CControlBar__UEAAXPEAVCDC__AEAVCRect___Z(CControlBar* pThis, CDC* pDC, CRect& rect) {
    if (!pThis || !pDC) return;
    const DWORD dwStyle = pThis->m_dwStyle;
    if (!(dwStyle & kCBRS_BORDER_ANY)) return;

    CRect rect1 = rect;
    CRect rect2 = rect;
    COLORREF clr = ClrBtnShadow();
    if (dwStyle & kCBRS_BORDER_3D) {
        rect1.right -= 1;
        rect1.bottom -= 1;
    }
    if (dwStyle & kCBRS_BORDER_TOP) rect2.top += kCyBorder2;
    if (dwStyle & kCBRS_BORDER_BOTTOM) rect2.bottom -= kCyBorder2;

    if (KindOf(pThis, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ()) && (dwStyle & kCBRS_BORDER_BOTTOM)) {
        CRect rectTheme(0, rect.top, rect.right, rect.bottom);
        HRESULT hr = E_FAIL;
        HTHEME hTheme = static_cast<HTHEME>(pThis->m_hReBarTheme);
        if (hTheme != nullptr) {
            if (::IsThemeBackgroundPartiallyTransparent(hTheme, kRP_BACKGROUND, 0)) {
                ::DrawThemeParentBackground(pThis->m_hWnd, pDC->m_hDC, static_cast<const RECT*>(rectTheme));
            }
            hr = ::DrawThemeBackground(hTheme, pDC->m_hDC, kRP_BACKGROUND, 0, static_cast<const RECT*>(rectTheme), nullptr);
        }
        if (FAILED(hr)) {
            impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, static_cast<const RECT*>(rectTheme), ::GetBkColor(pDC->m_hAttribDC));
        }
    }

    if (dwStyle & kCBRS_BORDER_LEFT)   impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, 0, rect2.top, 1, rect2.Height(), clr);
    if (dwStyle & kCBRS_BORDER_TOP)    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, 0, 0, rect.right, 1, clr);
    if (dwStyle & kCBRS_BORDER_RIGHT)  impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, rect1.right, rect2.top, -1, rect2.Height(), clr);
    if (dwStyle & kCBRS_BORDER_BOTTOM) impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, 0, rect1.bottom, rect.right, -1, clr);

    if (dwStyle & kCBRS_BORDER_3D) {
        clr = ClrBtnHilite();
        if (dwStyle & kCBRS_BORDER_LEFT)   impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, 1, rect2.top, 1, rect2.Height(), clr);
        if (dwStyle & kCBRS_BORDER_TOP)    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, 0, 1, rect.right, 1, clr);
        if (dwStyle & kCBRS_BORDER_RIGHT)  impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, rect.right, rect2.top, -1, rect2.Height(), clr);
        if (dwStyle & kCBRS_BORDER_BOTTOM) impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, 0, rect.bottom, rect.right, -1, clr);
    }

    if (dwStyle & kCBRS_BORDER_LEFT)   rect.left += kCxBorder2;
    if (dwStyle & kCBRS_BORDER_TOP)    rect.top += kCyBorder2;
    if (dwStyle & kCBRS_BORDER_RIGHT)  rect.right -= kCxBorder2;
    if (dwStyle & kCBRS_BORDER_BOTTOM) rect.bottom -= kCyBorder2;
}
// Retail: vftable slot 106 in the CDialogBar / CStatusBar / CToolBar / CDockBar
// vftables (mfc140u) holds 0x71e0 = `xor %eax,%eax; ret`: the int overload is
// the pre-Win64 hook that SetStatusText(INT_PTR) (slot 107, 0x1d6450) polls
// first, and the CControlBar default returns FALSE.  The mfc140u export table
// resolves ?SetStatusText@CControlBar@@UEAAHH@Z to that ICF-folded 0x71e0 (the
// wf2 RVA symbol map lists it under another export's name).  This body is the
// complete retail behaviour.
// Symbol: ?SetStatusText@CControlBar@@UEAAHH@Z
extern "C" int MS_ABI impl__SetStatusText_CControlBar__UEAAHH_Z(CControlBar* /*pThis*/, int /*nHit*/) {
    return FALSE;
}
// Transcribed from retail RVA 0x1d6450 (mfc140u; reached from vftable slot 107):
//     if (SetStatusText((int)(nHit > 0 ? 1 : -1)))            // vslot 106: `test %rdi,%rdi; cmovg` picks 1 for nHit > 0, else -1
//         return TRUE;
//     CWnd* pOwner = GetOwner();                               // +0xa0 / IAT GetParent, 0x28ad70
//     AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();   // 0x133a20
//     if (nHit == -1) {
//         pState->m_pLastStatus = NULL;                        // +0x130
//         if (m_nStateFlags & statusSet /*8*/) {               // +0x120
//             ::SendMessage(pOwner->m_hWnd, WM_POPMESSAGESTRING /*0x375*/, AFX_IDS_IDLEMESSAGE /*0xe001*/, 0);
//             m_nStateFlags &= ~statusSet;
//             return TRUE;
//         }
//         ::KillTimer(m_hWnd, ID_TIMER_WAIT /*0xec09*/);
//         return FALSE;
//     }
//     if ((m_nStateFlags & statusSet) && pState->m_nLastHit == nHit)   // +0x128
//         return FALSE;
//     pState->m_pLastStatus = this;
//     ::SendMessage(pOwner->m_hWnd, WM_SETMESSAGESTRING /*0x362*/, nHit, 0);
//     m_nStateFlags |= statusSet;
//     ResetTimer(ID_TIMER_CHECK /*0xec0a*/, 200);              // 0x1d6200
//     return TRUE;
// (The import slots resolve to GetParent, SendMessageW and KillTimer.)  The
// thread-state fields are the thread-locals declared above.  Retail
// dereferences pOwner unguarded; a NULL owner skips the SendMessage here.
// Symbol: ?SetStatusText@CControlBar@@UEAAH_J@Z
extern "C" int MS_ABI impl__SetStatusText_CControlBar__UEAAH_J_Z(CControlBar* pThis, __int64 nHit) {
    if (!pThis) return FALSE;
    if (impl__SetStatusText_CControlBar__UEAAHH_Z(pThis, nHit > 0 ? 1 : -1)) {   // vslot 106
        return TRUE;
    }
    CWnd* pOwner = GetOwnerWnd(pThis);
    const HWND hOwner = pOwner != nullptr ? pOwner->m_hWnd : nullptr;
    if (nHit == -1) {
        t_pLastStatus = nullptr;
        if (pThis->m_nStateFlags & kStatusSet) {
            if (hOwner != nullptr) ::SendMessage(hOwner, kWM_POPMESSAGESTRING, kAFX_IDS_IDLEMESSAGE, 0);
            pThis->m_nStateFlags &= ~kStatusSet;
            return TRUE;
        }
        ::KillTimer(pThis->m_hWnd, kTimerWait);
        return FALSE;
    }
    if ((pThis->m_nStateFlags & kStatusSet) && t_nLastHit == nHit) {
        return FALSE;
    }
    t_pLastStatus = pThis;
    if (hOwner != nullptr) ::SendMessage(hOwner, kWM_SETMESSAGESTRING, static_cast<WPARAM>(nHit), 0);
    pThis->m_nStateFlags |= kStatusSet;
    impl__ResetTimer_CControlBar__QEAAX_KI_Z(pThis, kTimerCheck, 200);
    return TRUE;
}
// Symbol: ?WindowProc@CControlBar@@UEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CControlBar__UEAA_JI_K_J_Z(CControlBar* pThis, unsigned int msg, UINT64 wParam, __int64 lParam) {
    if (!pThis->m_hWnd) return 0;
    return (__int64)::DefWindowProcW(pThis->m_hWnd, msg, (WPARAM)wParam, (LPARAM)lParam);
}
// Symbol: ?CalcFixedLayout@CControlBar@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CControlBar__UEAA_AVCSize__HH_Z(void* pRet, CControlBar* pThis, int bStretch, int bHorz) {
    (void)bStretch;
    (void)bHorz;
    if (pThis && pThis->m_hWnd) {
        RECT rc = {};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        HWND hParent = ::GetParent(pThis->m_hWnd);
        ::ScreenToClient(hParent, (POINT*)&rc.left);
        ::ScreenToClient(hParent, (POINT*)&rc.right);
        new(pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
    } else {
        new(pRet) CSize(0, 0);
    }
    return pRet;
}
// Symbol: ?CalcDynamicLayout@CControlBar@@UEAA?AVCSize@@HK@Z
extern "C" void* MS_ABI impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(void* pRet, CControlBar* pThis, int nLength, DWORD dwMode) {
    (void)nLength;
    (void)dwMode;
    if (pThis && pThis->m_hWnd) {
        RECT rc = {};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        new(pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
    } else {
        new(pRet) CSize(0, 0);
    }
    return pRet;
}
// Transcribed from retail RVA 0x1d6950 (mfc140u; WM_HELPHITTEST (0x366) entry
// of the class message map at 0x3214a0):
//     CPoint point((short)LOWORD(lParam), (short)HIWORD(lParam));
//     INT_PTR nID = OnToolHitTest(point, NULL);                 // vslot 31
//     if (nID != -1) return HID_BASE_COMMAND /*0x10000*/ + nID;
//     nID = (UINT)::GetDlgCtrlID(m_hWnd);                       // IAT GetDlgCtrlID, zero-extended
//     return nID != 0 ? HID_BASE_CONTROL /*0x50000*/ + nID : 0;
// The vslot is devirtualised by CallOnToolHitTest (see its comment).
// Symbol: ?OnHelpHitTest@CControlBar@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnHelpHitTest_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam) {
    (void)wParam;
    if (!pThis) return 0;
    CPoint point(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam)));
    __int64 nID = CallOnToolHitTest(pThis, point, nullptr);   // vslot 31
    if (nID != -1) {
        return static_cast<__int64>(kHID_BASE_COMMAND) + nID;
    }
    const unsigned int nCtrlID = static_cast<unsigned int>(::GetDlgCtrlID(pThis->m_hWnd));
    return nCtrlID != 0 ? static_cast<__int64>(kHID_BASE_CONTROL + nCtrlID) : 0;
}
// Transcribed from retail RVA 0x1d70c0 (mfc140u; WM_IDLEUPDATECMDUI (0x363)
// entry of the class message map at 0x321320; OnInitialUpdate tail-jumps here):
//     BOOL bVis = GetStyle() & WS_VISIBLE;                          // 0x2a9690
//     UINT swpFlags = 0;
//     if ((m_nStateFlags & delayHide /*1*/) && bVis)       swpFlags = SWP_HIDEWINDOW;   // 0x80
//     else if ((m_nStateFlags & delayShow /*2*/) && !bVis) swpFlags = SWP_SHOWWINDOW;   // 0x40
//     m_nStateFlags &= ~(delayShow|delayHide);
//     if (swpFlags != 0)
//         SetWindowPos(NULL, 0, 0, 0, 0, swpFlags|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);   // 0x2a9a60, |0x17
//     if ((GetStyle() & WS_VISIBLE) &&
//         (m_pDockBar == NULL || (m_pDockBar->GetStyle() & WS_VISIBLE))) {   // +0x138
//         CFrameWnd* pTarget = (CFrameWnd*)GetOwner();                   // +0xa0 / IAT GetParent, 0x28ad70
//         if (pTarget == NULL || !pTarget->IsFrameWnd())                 // CWnd vslot 86
//             pTarget = GetParentFrame();                                // 0x28e200
//         if (pTarget != NULL)
//             OnUpdateCmdUI(pTarget, (BOOL)wParam);                      // vslot 93 (pure)
//     }
//     return 0;
// vslot 93 is devirtualised by CallOnUpdateCmdUI (see its comment for which
// derived classes it can reach).
// Symbol: ?OnIdleUpdateCmdUI@CControlBar@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam) {
    (void)lParam;
    if (!pThis) return 0;
    const bool bVis = (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_VISIBLE) != 0;
    UINT swpFlags = 0;
    if ((pThis->m_nStateFlags & kDelayHide) && bVis) {
        swpFlags = SWP_HIDEWINDOW;
    } else if ((pThis->m_nStateFlags & kDelayShow) && !bVis) {
        swpFlags = SWP_SHOWWINDOW;
    }
    pThis->m_nStateFlags &= ~(kDelayShow | kDelayHide);
    if (swpFlags != 0) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pThis, nullptr, 0, 0, 0, 0,
            swpFlags | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }

    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_VISIBLE) {
        CWnd* pDockBar = static_cast<CWnd*>(pThis->m_pDockBar);
        if (pDockBar == nullptr || (impl__GetStyle_CWnd__QEBAKXZ(pDockBar) & WS_VISIBLE)) {
            CWnd* pOwner = GetOwnerWnd(pThis);
            CFrameWnd* pTarget = nullptr;
            if (pOwner != nullptr && impl__IsFrameWnd_CWnd__UEBAHXZ(pOwner)) {   // CWnd vslot 86
                pTarget = static_cast<CFrameWnd*>(pOwner);
            } else {
                pTarget = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
            }
            if (pTarget != nullptr) {
                CallOnUpdateCmdUI(pThis, pTarget, static_cast<int>(wParam));   // vslot 93
            }
        }
    }
    return 0;
}
// Transcribed from retail RVA 0x1d71e0 (mfc140u; WM_INITIALUPDATE (0x364)
// entry of the class message map at 0x321480):
//     xor %r8d,%r8d; lea 0x1(%r8),%edx; jmp 0x1d70c0
// i.e. `OnIdleUpdateCmdUI(TRUE, 0)`.
// Symbol: ?OnInitialUpdate@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CControlBar__QEAAXXZ(CControlBar* pThis) {
    impl__OnIdleUpdateCmdUI_CControlBar__QEAA_J_K_J_Z(pThis, TRUE, 0);
}
// Transcribed from retail RVA 0x1d6c30 (mfc140u; WM_THEMECHANGED (0x31a) entry
// of the class message map at 0x321460):
//     if (IsKindOf(RUNTIME_CLASS(CToolBar)) || IsKindOf(RUNTIME_CLASS(CDockBar))) {   // 0x234cf0; 0x3227b0 / 0x3218f8
//         if (m_hReBarTheme != NULL) ::CloseThemeData(m_hReBarTheme);              // +0x118, IAT CloseThemeData
//         m_hReBarTheme = ::OpenThemeData(m_hWnd, L"REBAR");                       // IAT OpenThemeData, string at 0x3430b0
//     }
//     return TRUE;
// Symbol: ?OnThemeChanged@CControlBar@@QEAA_JXZ
extern "C" __int64 MS_ABI impl__OnThemeChanged_CControlBar__QEAA_JXZ(CControlBar* pThis) {
    if (!pThis) return TRUE;
    if (IsToolBarOrDockBar(pThis)) {
        HTHEME hTheme = static_cast<HTHEME>(pThis->m_hReBarTheme);
        if (hTheme != nullptr) {
            ::CloseThemeData(hTheme);
        }
        pThis->m_hReBarTheme = ::OpenThemeData(pThis->m_hWnd, L"REBAR");
    }
    return TRUE;
}
// Transcribed from retail RVA 0x1d6260 (mfc140u; WM_TIMER (0x113) entry of the
// class message map at 0x3212c0):
//     if (::GetKeyState(VK_LBUTTON) < 0) return;                            // IAT GetKeyState
//     AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();          // 0x133a20
//     CPoint point; ::GetCursorPos(&point); ::ScreenToClient(m_hWnd, &point);
//     INT_PTR nHit = OnToolHitTest(point, NULL);                            // vslot 31
//     if (nHit >= 0) {
//         CWnd* pParent = GetTopLevelParent();                              // 0x28e2e0
//         if (!IsTopParentActive()) nHit = -1;                              // 0x28e3d0
//         else { if (pParent == NULL) AfxThrowInvalidArgException();
//                if (!pParent->IsWindowEnabled()) nHit = -1; }              // 0x2a9b00
//         HWND hWndTip = pState->m_pToolTip ? pState->m_pToolTip->m_hWnd : NULL;   // +0x108
//         CWnd* pCapture = CWnd::FromHandle(::GetCapture());               // IAT GetCapture, 0x28ad70
//         if (pCapture != this && (pCapture ? pCapture->m_hWnd : NULL) != hWndTip &&
//             pCapture->GetTopLevelParent() == pParent)                     // (NULL pCapture yields NULL there)
//             nHit = -1;
//         if (nHit >= 0) {
//             ::ClientToScreen(m_hWnd, &point);
//             HWND hWnd = ::WindowFromPoint(point);                         // IAT WindowFromPoint
//             if (hWnd == NULL ||
//                 (hWnd != m_hWnd && !::IsChild(m_hWnd, hWnd) && hWnd != hWndTip)) {
//                 nHit = -1; pState->m_nLastHit = -1;                       // +0x128
//             }
//         }
//     } else {
//         pState->m_nLastHit = -1;
//     }
//     if (nHit < 0) {
//         if (pState->m_nLastHit == -1) ::KillTimer(m_hWnd, ID_TIMER_CHECK /*0xec0a*/);
//         SetStatusText(-1);                                                // vslot 107
//     }
//     if (nIDEvent == ID_TIMER_WAIT /*0xec09*/) {
//         ::KillTimer(m_hWnd, ID_TIMER_WAIT);
//         if (nHit >= 0) SetStatusText(nHit);                               // vslot 107
//     }
// Note the asymmetry retail has: the "not active / disabled / captured"
// paths clear nHit without storing m_nLastHit, the "no window under the
// cursor" paths store -1 too.  OpenMFC has no module tool-tip window
// (AFX_MODULE_THREAD_STATE::m_pToolTip), so hWndTip is always NULL here; vslot
// 31 is devirtualised by CallOnToolHitTest and vslot 107 goes to this file's
// SetStatusText thunk.
// Symbol: ?OnTimer@CControlBar@@QEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CControlBar__QEAAX_K_Z(CControlBar* pThis, UINT64 nIDEvent) {
    if (!pThis) return;
    if (::GetKeyState(VK_LBUTTON) < 0) return;

    POINT pt = {};
    ::GetCursorPos(&pt);
    ::ScreenToClient(pThis->m_hWnd, &pt);
    __int64 nHit = CallOnToolHitTest(pThis, CPoint(pt), nullptr);   // vslot 31
    if (nHit >= 0) {
        // GetTopLevelParent() / pParent->IsWindowEnabled() (0x2a9b00 =
        // ::IsWindowEnabled(m_hWnd) when m_pCtrlSite is NULL) on the HWND --
        // see TopLevelParentHwnd for why the CWnd thunk is not used.
        const HWND hParentTop = TopLevelParentHwnd(pThis);
        if (!impl__IsTopParentActive_CWnd__QEBAHXZ(pThis)) {
            nHit = -1;
        } else {
            if (hParentTop == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
            if (!::IsWindowEnabled(hParentTop)) nHit = -1;
        }

        const HWND hWndTip = nullptr;   // no AFX_MODULE_THREAD_STATE::m_pToolTip in OpenMFC
        CWnd* pCapture = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture());
        if (pCapture != pThis) {
            const HWND hCapture = pCapture != nullptr ? pCapture->m_hWnd : nullptr;
            if (hCapture != hWndTip) {
                // retail compares the two CWnd::FromHandle results; the same
                // HWND maps to the same object, so HWND equality is equivalent
                // (a NULL pCapture yields NULL in both, as retail's
                // GetTopLevelParent(NULL) does).
                const HWND hCaptureTop = TopLevelParentHwnd(pCapture);
                if (hCaptureTop == hParentTop) nHit = -1;
            }
        }

        if (nHit >= 0) {
            ::ClientToScreen(pThis->m_hWnd, &pt);
            HWND hWnd = ::WindowFromPoint(pt);
            if (hWnd == nullptr ||
                (hWnd != pThis->m_hWnd && !::IsChild(pThis->m_hWnd, hWnd) && hWnd != hWndTip)) {
                nHit = -1;
                t_nLastHit = -1;
            }
        }
    } else {
        t_nLastHit = -1;
    }

    if (nHit < 0) {
        if (t_nLastHit == -1) ::KillTimer(pThis->m_hWnd, kTimerCheck);
        impl__SetStatusText_CControlBar__UEAAH_J_Z(pThis, -1);   // vslot 107
    }
    if (nIDEvent == kTimerWait) {
        ::KillTimer(pThis->m_hWnd, kTimerWait);
        if (nHit >= 0) {
            impl__SetStatusText_CControlBar__UEAAH_J_Z(pThis, nHit);   // vslot 107
        }
    }
}
// Transcribed from retail RVA 0x1d6550 (mfc140u; CWnd vftable slot 69 in the
// CDialogBar / CStatusBar / CToolBar vftables):
//     if (CWnd::PreTranslateMessage(pMsg)) return TRUE;   // 0x28bc00 -- see below
//     UINT message = pMsg->message;
//     CWnd* pOwner = GetOwner();                                             // +0xa0 / IAT GetParent, 0x28ad70
//     if (((m_dwStyle & CBRS_FLYBY /*0x20*/) &&
//          ((message >= 0x200 && message <= 0x209) || (message >= 0xa0 && message <= 0xa9))) ||
//         message == WM_LBUTTONDOWN || message == WM_LBUTTONUP) {
//         AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();      // 0x133a20
//         CPoint point = pMsg->pt; ::ScreenToClient(m_hWnd, &point);        // +0x24
//         TOOLINFOW ti; memset(&ti, 0, 0x40); ti.cbSize = 0x48;
//         INT_PTR nHit = OnToolHitTest(point, &ti);                          // vslot 31
//         if (ti.lpszText != LPSTR_TEXTCALLBACK) free(ti.lpszText);          // IAT free
//         BOOL bNotButton = FALSE;
//         if (message == WM_LBUTTONDOWN) bNotButton = (ti.uFlags & TTF_NOTBUTTON /*0x80000000*/) != 0;
//         else if (::GetKeyState(VK_LBUTTON) < 0) nHit = pState->m_nLastHit; // +0x128
//         if (nHit < 0 || bNotButton) {
//             if (::GetKeyState(VK_LBUTTON) >= 0 || bNotButton) {
//                 SetStatusText(-1);                                         // vslot 107
//                 ::KillTimer(m_hWnd, ID_TIMER_CHECK /*0xec0a*/);
//             }
//         } else if (message == WM_LBUTTONUP) {
//             SetStatusText(-1);
//             ResetTimer(ID_TIMER_CHECK, 200);                               // 0x1d6200
//         } else if ((m_nStateFlags & statusSet /*8*/) || ::GetKeyState(VK_LBUTTON) < 0) {
//             SetStatusText(nHit);
//         } else if (nHit != pState->m_nLastHit) {
//             ResetTimer(ID_TIMER_WAIT /*0xec09*/, 300);
//         }
//         pState->m_nLastHit = nHit;
//     }
//     CFrameWnd* pFrame = GetTopLevelFrame();                                // 0x28e490
//     if (pFrame != NULL && pFrame->m_bHelpMode) return FALSE;               // +0x104
//     while (pOwner != NULL) {
//         if (pOwner->PreTranslateMessage(pMsg)) return TRUE;                // CWnd vslot 69
//         pOwner = pOwner->GetParentFrame();                                 // 0x28e200
//     }
//     // CWnd::PreTranslateInput, inlined:
//     if (!::IsWindow(m_hWnd)) return FALSE;
//     if ((message >= WM_KEYFIRST && message <= 0x109) || (message >= 0x200 && message <= 0x209))
//         return IsDialogMessage(pMsg);                                      // 0x2a9630
//     return FALSE;
// DEVIATIONS: (1) retail's CWnd::PreTranslateMessage (0x28bc00) only invokes
// m_pModuleState->m_pfnFilterToolTipMessage and always returns 0; OpenMFC has
// no such hook and its CWnd thunk is an IsDialogMessage wrapper that would eat
// the owner frame's accelerators, so that first call is omitted.  (2)
// CFrameWnd::m_bHelpMode is not modelled in OpenMFC's CFrameWnd, so the
// Shift+F1 early-out is omitted.  (3) the whole TOOLINFOW is zeroed (retail
// zeroes the first 0x40 bytes and sets cbSize = 0x48).  The owner loop is
// devirtualised by OwnerPreTranslateMessage, vslot 31 by CallOnToolHitTest.
// Symbol: ?PreTranslateMessage@CControlBar@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CControlBar__UEAAHPEAUtagMSG___Z(CControlBar* pThis, MSG* pMsg) {
    if (!pThis || !pMsg) return FALSE;
    const UINT message = pMsg->message;
    CWnd* pOwner = GetOwnerWnd(pThis);

    const bool bFlyByMouse = (pThis->m_dwStyle & kCBRS_FLYBY) &&
        ((message >= 0x200 && message <= 0x209) || (message >= 0xa0 && message <= 0xa9));
    if (bFlyByMouse || message == WM_LBUTTONDOWN || message == WM_LBUTTONUP) {
        POINT pt = pMsg->pt;
        ::ScreenToClient(pThis->m_hWnd, &pt);
        TOOLINFOW ti;
        std::memset(&ti, 0, sizeof(ti));
        ti.cbSize = 0x48;
        __int64 nHit = CallOnToolHitTest(pThis, CPoint(pt), &ti);   // vslot 31
        if (ti.lpszText != LPSTR_TEXTCALLBACKW) {
            free(ti.lpszText);
        }
        bool bNotButton = false;
        if (message == WM_LBUTTONDOWN) {
            bNotButton = (ti.uFlags & 0x80000000u /*TTF_NOTBUTTON*/) != 0;
        } else if (::GetKeyState(VK_LBUTTON) < 0) {
            nHit = t_nLastHit;
        }

        if (nHit < 0 || bNotButton) {
            if (::GetKeyState(VK_LBUTTON) >= 0 || bNotButton) {
                impl__SetStatusText_CControlBar__UEAAH_J_Z(pThis, -1);   // vslot 107
                ::KillTimer(pThis->m_hWnd, kTimerCheck);
            }
        } else if (message == WM_LBUTTONUP) {
            impl__SetStatusText_CControlBar__UEAAH_J_Z(pThis, -1);       // vslot 107
            impl__ResetTimer_CControlBar__QEAAX_KI_Z(pThis, kTimerCheck, 200);
        } else if ((pThis->m_nStateFlags & kStatusSet) || ::GetKeyState(VK_LBUTTON) < 0) {
            impl__SetStatusText_CControlBar__UEAAH_J_Z(pThis, nHit);     // vslot 107
        } else if (nHit != t_nLastHit) {
            impl__ResetTimer_CControlBar__QEAAX_KI_Z(pThis, kTimerWait, 300);
        }
        t_nLastHit = nHit;
    }

    // (retail: `if (GetTopLevelFrame()->m_bHelpMode) return FALSE;` -- not modelled)

    while (pOwner != nullptr) {
        if (OwnerPreTranslateMessage(pOwner, pMsg)) {   // CWnd vslot 69
            return TRUE;
        }
        pOwner = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pOwner);
    }

    if (!::IsWindow(pThis->m_hWnd)) return FALSE;
    if ((message >= WM_KEYFIRST && message <= 0x109) || (message >= 0x200 && message <= 0x209)) {
        return impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(pThis, pMsg);
    }
    return FALSE;
}
// Transcribed from retail RVA 0x1d6da0 (mfc140u; WM_MOUSEACTIVATE (0x21)
// entry of the class message map at 0x3213e0):
//     if (IsFloating()) {                  // 0x2a0e80 (direct call, not a vslot)
//         ActivateTopParent();             // 0x28e430
//         return MA_NOACTIVATE;            // 3
//     }
//     return (int)Default();               // 0x28ac80 (tail jump)
// The arguments are not read.  IsFloating goes to this file's thunk, whose
// pre-existing body tests m_dwStyle against the header's CBRS_FLOATING bit
// (retail 0x2a0e80 instead reads CDockBar::m_bFloating at +0x148 of this or
// of m_pDockBar, a class OpenMFC does not lay out); ActivateTopParent goes to
// the CWnd thunk.
// Symbol: ?OnMouseActivate@CControlBar@@QEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CControlBar__QEAAHPEAVCWnd__II_Z(CControlBar* pThis, CWnd* pDesktopWnd, UINT nHitTest, UINT message) {
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    if (!pThis) return MA_ACTIVATE;
    if (impl__IsFloating_CControlBar__QEBAHXZ(pThis)) {
        impl__ActivateTopParent_CWnd__QEAAXXZ(pThis);
        return MA_NOACTIVATE;
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}
// Transcribed from retail RVA 0x1d6f70 (mfc140u; WM_CTLCOLOR (0x19) entry of
// the class message map at 0x321300):
//     LRESULT lResult;
//     if (pWnd->SendChildNotifyLastMsg(&lResult)) return (HBRUSH)lResult;   // 0x28ef80
//     if (!CWnd::GrayCtlColor(pDC->m_hDC, pWnd ? pWnd->m_hWnd : NULL, nCtlColor,
//                             afxData.hbrBtnFace, afxData.clrBtnText))       // 0x291010; 0x3c32e8 / 0x3c32fc
//         return (HBRUSH)Default();                                          // 0x28ac80
//     return afxData.hbrBtnFace;
// Retail calls SendChildNotifyLastMsg through pWnd without a NULL test (it
// does test pWnd before reading its HWND); a NULL pWnd skips the notify here.
// The afxData reads are replaced as described at the top of the file.
// Symbol: ?OnCtlColor@CControlBar@@QEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CControlBar__QEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(CControlBar* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    if (!pThis) return nullptr;
    LONGLONG lResult = 0;
    if (pWnd != nullptr && impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(pWnd, &lResult)) {
        return reinterpret_cast<HBRUSH>(lResult);
    }
    const HBRUSH hbrBtnFace = HbrBtnFace();
    if (!impl__GrayCtlColor_CWnd__SAHPEAUHDC____PEAUHWND____IPEAUHBRUSH____K_Z(
            pDC != nullptr ? pDC->m_hDC : nullptr, pWnd != nullptr ? pWnd->m_hWnd : nullptr,
            nCtlColor, hbrBtnFace, ClrBtnText())) {
        return reinterpret_cast<HBRUSH>(impl__Default_CWnd__IEAA_JXZ(pThis));
    }
    return hbrBtnFace;
}
// Transcribed from retail RVA 0x1d6ca0 (mfc140u; WM_DESTROY (2) entry of the
// class message map at 0x321440):
//     if (IsKindOf(RUNTIME_CLASS(CToolBar)) || IsKindOf(RUNTIME_CLASS(CDockBar)))   // 0x234cf0; 0x3227b0 / 0x3218f8
//         if (::IsAppThemed()) ::CloseThemeData(m_hReBarTheme);           // IAT IsAppThemed / CloseThemeData; +0x118 (not NULL-tested, not reset)
//     if (AfxGetModuleThreadState()->m_pLastStatus == this)              // 0x133a20, +0x130
//         SetStatusText(-1);                                              // vslot 107
//     if (m_pDockSite != NULL) {                                          // +0x130
//         m_pDockSite->RemoveControlBar(this);                            // 0x29e320 (CFrameWnd::RemoveControlBar)
//         m_pDockSite = NULL;
//     }
//     CWnd::OnDestroy();                                                  // 0x28b840 (tail jump)
// Symbol: ?OnDestroy@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CControlBar__QEAAXXZ(CControlBar* pThis) {
    if (!pThis) return;
    if (IsToolBarOrDockBar(pThis)) {
        if (::IsAppThemed()) {
            ::CloseThemeData(static_cast<HTHEME>(pThis->m_hReBarTheme));
        }
    }
    if (t_pLastStatus == pThis) {
        impl__SetStatusText_CControlBar__UEAAH_J_Z(pThis, -1);   // vslot 107
    }
    if (pThis->m_pDockSite != nullptr) {
        impl__RemoveControlBar_CFrameWnd__QEAAXPEAVCControlBar___Z(static_cast<CFrameWnd*>(pThis->m_pDockSite), pThis);
        pThis->m_pDockSite = nullptr;
    }
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}
// Transcribed from retail RVA 0x1d69c0 (mfc140u; WM_WINDOWPOSCHANGING (0x46)
// entry of the class message map at 0x321360):
//     ::DefWindowProc(m_hWnd, WM_WINDOWPOSCHANGING, 0, (LPARAM)lpWndPos);   // IAT DefWindowProcW (bypasses CWnd::Default on purpose)
//     if (lpWndPos->flags & SWP_NOSIZE) return;                             // +0x20
//     CRect rect; ::GetWindowRect(m_hWnd, &rect);
//     CSize sizePrev = rect.Size();
//     int cx = lpWndPos->cx, cy = lpWndPos->cy;                             // +0x18 / +0x1c
//     if (cx != sizePrev.cx && (m_dwStyle & CBRS_BORDER_RIGHT /*0x400*/)) {
//         ::SetRect(&rect, cx - afxData.cxBorder2, 0, cx, cy);              // 0x3c32d0
//         ::InvalidateRect(m_hWnd, &rect, TRUE);
//         ::SetRect(&rect, sizePrev.cx - afxData.cxBorder2, 0, sizePrev.cx, cy);
//         ::InvalidateRect(m_hWnd, &rect, TRUE);
//     }
//     if (cy != sizePrev.cy && (m_dwStyle & CBRS_BORDER_BOTTOM /*0x800*/)) {
//         ::SetRect(&rect, 0, cy - afxData.cyBorder2, cx, cy);              // 0x3c32d4
//         ::InvalidateRect(m_hWnd, &rect, TRUE);
//         ::SetRect(&rect, 0, sizePrev.cy - afxData.cyBorder2, cx, sizePrev.cy);
//         ::InvalidateRect(m_hWnd, &rect, TRUE);
//     }
// Symbol: ?OnWindowPosChanging@CControlBar@@QEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CControlBar__QEAAXPEAUtagWINDOWPOS___Z(CControlBar* pThis, WINDOWPOS* pWndPos) {
    if (!pThis || !pWndPos) return;
    ::DefWindowProc(pThis->m_hWnd, WM_WINDOWPOSCHANGING, 0, reinterpret_cast<LPARAM>(pWndPos));
    if (pWndPos->flags & SWP_NOSIZE) return;

    RECT rect = {};
    ::GetWindowRect(pThis->m_hWnd, &rect);
    const int cxPrev = rect.right - rect.left;
    const int cyPrev = rect.bottom - rect.top;
    const int cx = pWndPos->cx;
    const int cy = pWndPos->cy;
    if (cx != cxPrev && (pThis->m_dwStyle & kCBRS_BORDER_RIGHT)) {
        ::SetRect(&rect, cx - kCxBorder2, 0, cx, cy);
        ::InvalidateRect(pThis->m_hWnd, &rect, TRUE);
        ::SetRect(&rect, cxPrev - kCxBorder2, 0, cxPrev, cy);
        ::InvalidateRect(pThis->m_hWnd, &rect, TRUE);
    }
    if (cy != cyPrev && (pThis->m_dwStyle & kCBRS_BORDER_BOTTOM)) {
        ::SetRect(&rect, 0, cy - kCyBorder2, cx, cy);
        ::InvalidateRect(pThis->m_hWnd, &rect, TRUE);
        ::SetRect(&rect, 0, cyPrev - kCyBorder2, cx, cyPrev);
        ::InvalidateRect(pThis->m_hWnd, &rect, TRUE);
    }
}
// Symbol: ?PostNcDestroy@CControlBar@@UEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CControlBar__UEAAXXZ(CControlBar* pThis) {
    if (pThis->m_bAutoDelete) {
        delete pThis;
    }
}
// Symbol: ?PreCreateWindow@CControlBar@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CControlBar__UEAAHAEAUtagCREATESTRUCTW___Z(CControlBar* pThis, CREATESTRUCTW& cs) {
    (void)pThis;
    // Default CControlBar::PreCreateWindow: ensure WS_CLIPCHILDREN
    cs.style |= WS_CLIPCHILDREN;
    return TRUE;
}
// Transcribed from retail RVA 0x1d6200 (mfc140u).  The mfc140u export table
// resolves ?ResetTimer@CControlBar@@QEAAX_KI@Z to exactly that address (the
// wf2 RVA symbol map omits it); it is also the callee SetStatusText (0x1d6450)
// and PreTranslateMessage (0x1d6550) reach with (this, 0xec0a, 200) /
// (this, 0xec09, 300):
//     ::KillTimer(m_hWnd, ID_TIMER_WAIT /*0xec09*/);      // IAT KillTimer
//     ::KillTimer(m_hWnd, ID_TIMER_CHECK /*0xec0a*/);
//     ::SetTimer(m_hWnd, nIDEvent, uElapse, NULL);        // IAT SetTimer (tail jump)
// Both timers are always killed and the new one always set, whatever
// nIDEvent / uElapse are.  Retail reads m_hWnd unguarded; a NULL HWND returns
// here (KillTimer/SetTimer on NULL would only fail).
// Symbol: ?ResetTimer@CControlBar@@QEAAX_KI@Z
extern "C" void MS_ABI impl__ResetTimer_CControlBar__QEAAX_KI_Z(CControlBar* pThis, UINT64 nIDEvent, UINT uElapse) {
    if (!pThis || !pThis->m_hWnd) return;
    ::KillTimer(pThis->m_hWnd, kTimerWait);
    ::KillTimer(pThis->m_hWnd, kTimerCheck);
    ::SetTimer(pThis->m_hWnd, static_cast<UINT_PTR>(nIDEvent), uElapse, nullptr);
}
// Transcribed from retail RVA 0x1d6b40 (mfc140u; WM_CREATE (1) entry of the
// class message map at 0x321420):
//     if ((int)Default() == -1) return -1;                                  // 0x28ac80 (CWnd::OnCreate inlined)
//     if (m_dwStyle & CBRS_TOOLTIPS /*0x10*/) EnableToolTips();             // inlined: AfxGetModuleThreadState();
//                                                                            //   if (!(m_nFlags & WF_TOOLTIPS)) { m_pModuleState->m_pfnFilterToolTipMessage = &CWnd::_FilterToolTipMessage (0x2758a0); m_nFlags |= WF_TOOLTIPS; }
//     CFrameWnd* pFrameWnd = (CFrameWnd*)CWnd::FromHandle(::GetParent(m_hWnd));   // IAT GetParent, 0x28ad70
//     if (pFrameWnd->IsFrameWnd()) {                                        // CWnd vslot 86
//         m_pDockSite = pFrameWnd;                                          // +0x130
//         pFrameWnd->m_listControlBars.AddTail(this);                       // CObList at CFrameWnd+0x128, 0x231e70
//     }
//     if (IsKindOf(RUNTIME_CLASS(CToolBar)) || IsKindOf(RUNTIME_CLASS(CDockBar)))   // 0x234cf0; 0x3227b0 / 0x3218f8
//         if (::IsAppThemed()) m_hReBarTheme = ::OpenThemeData(m_hWnd, L"REBAR");   // IAT IsAppThemed / OpenThemeData; +0x118
//     return 0;
// DEVIATIONS: OpenMFC's CFrameWnd has no m_listControlBars (its bar list is a
// runtime-state vector private to core/frame/CFrameWnd.cpp with no exported
// "add" entry point), so the AddTail is dropped -- m_pDockSite is still set.
// EnableToolTips goes through the CWnd thunk (OpenMFC keeps a per-window
// flag; there is no tool-tip filter hook to install).
// Symbol: ?OnCreate@CControlBar@@QEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CControlBar__QEAAHPEAUtagCREATESTRUCTW___Z(CControlBar* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pCreateStruct;
    if (!pThis) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis)) == -1) {
        return -1;
    }
    if (pThis->m_dwStyle & kCBRS_TOOLTIPS) {
        impl__EnableToolTips_CWnd__QEAAHH_Z(pThis, TRUE);
    }
    CWnd* pFrameWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pFrameWnd != nullptr && impl__IsFrameWnd_CWnd__UEBAHXZ(pFrameWnd)) {   // CWnd vslot 86
        pThis->m_pDockSite = pFrameWnd;
        // retail: pFrameWnd->m_listControlBars.AddTail(this) -- not modelled
    }
    if (IsToolBarOrDockBar(pThis)) {
        if (::IsAppThemed()) {
            pThis->m_hReBarTheme = ::OpenThemeData(pThis->m_hWnd, L"REBAR");
        }
    }
    return 0;
}
// Transcribed from retail RVA 0x1d6dd0 (mfc140u; WM_PAINT (0xf) entry of the
// class message map at 0x3212e0):
//     CPaintDC dc(this);                       // 0x2a3d20
//     if (IsVisible()) DoPaint(&dc);           // vslot 102, vslot 95
// Both vslot calls go to this file's CControlBar thunks (CDockBar, whose
// DoPaint slot is a no-op, handles WM_PAINT itself in retail and never gets
// here).  CPaintDC exists in this DLL only as its thunks, so it is built in
// raw storage the way featurepack/docking/CBasePane.cpp does.
// Symbol: ?OnPaint@CControlBar@@QEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CControlBar__QEAAXXZ(CControlBar* pThis) {
    if (!pThis) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, pThis);
    if (impl__IsVisible_CControlBar__UEBAHXZ(pThis)) {           // vslot 102
        impl__DoPaint_CControlBar__UEAAXPEAVCDC___Z(pThis, pDC);   // vslot 95
    }
    impl___1CPaintDC__UEAA_XZ(pDC);
}
// Transcribed from retail RVA 0x1d6ff0 (mfc140u; WM_LBUTTONDOWN (0x201) entry
// of the class message map at 0x3213a0):
//     if (m_pDockBar != NULL) {                                    // +0x138
//         if (OnToolHitTest(point, NULL) == -1) {                  // vslot 31
//             ::ClientToScreen(m_hWnd, &point);                    // IAT ClientToScreen
//             m_pDockContext->StartDrag(point);                    // +0x140, CDockContext vslot 0
//             return;
//         }
//     }
//     Default();                                                   // 0x28ac80 (CWnd::OnLButtonDown)
// DEVIATION: the StartDrag call is not made.  CDockContext exists in OpenMFC
// only as placeholder thunks with auto-generated parameter lists
// (core/controlbar/CDockContext.cpp: `impl__StartDrag_CDockContext__UEAAXVCPoint___Z(void* p0)`),
// so it can neither be declared correctly from here nor does it do anything;
// the branch keeps retail's control flow (no Default() on that path) and is
// listed in headerRequests.
// Symbol: ?OnLButtonDown@CControlBar@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CControlBar__QEAAXIVCPoint___Z(CControlBar* pThis, UINT nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    if (pThis->m_pDockBar != nullptr) {
        if (CallOnToolHitTest(pThis, point, nullptr) == -1) {   // vslot 31
            POINT pt = { point.x, point.y };
            ::ClientToScreen(pThis->m_hWnd, &pt);
            // retail: m_pDockContext->StartDrag(pt) -- see the comment above
            return;
        }
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}
// Transcribed from retail RVA 0x1d7060 (mfc140u; WM_LBUTTONDBLCLK (0x203)
// entry of the class message map at 0x3213c0):
//     if (m_pDockBar != NULL) {                                    // +0x138
//         if (OnToolHitTest(point, NULL) == -1) {                  // vslot 31
//             m_pDockContext->ToggleDocking();                     // +0x140, CDockContext vslot 2 (tail call)
//             return;
//         }
//     }
//     Default();                                                   // 0x28ac80 (CWnd::OnLButtonDblClk, tail jump)
// DEVIATION: the ToggleDocking call is not made, for the reason given at
// OnLButtonDown (its thunk is `impl__ToggleDocking_CDockContext__UEAAXXZ()`
// with no parameters and an empty body); the branch keeps retail's control
// flow and is listed in headerRequests.
// Symbol: ?OnLButtonDblClk@CControlBar@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CControlBar__QEAAXIVCPoint___Z(CControlBar* pThis, UINT nFlags, CPoint point) {
    (void)nFlags;
    if (!pThis) return;
    if (pThis->m_pDockBar != nullptr) {
        if (CallOnToolHitTest(pThis, point, nullptr) == -1) {   // vslot 31
            // retail: m_pDockContext->ToggleDocking() -- see the comment above
            return;
        }
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}
// Transcribed from retail RVA 0x1d72c0 (mfc140u; WM_SIZEPARENT (0x361) entry
// of the class message map at 0x321340):
//     AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
//     DWORD dwStyle = RecalcDelayShow(lpLayout);                            // vslot 103
//     if ((dwStyle & WS_VISIBLE) && (dwStyle & CBRS_ALIGN_ANY /*0xf000*/)) {
//         CRect rect; rect.CopyRect(&lpLayout->rect);                       // IAT CopyRect, +8
//         CSize sizeAvail = rect.Size();
//         DWORD dwMode = lpLayout->bStretch ? LM_STRETCH : 0;               // +0x20
//         if ((m_dwStyle & (CBRS_SIZE_DYNAMIC|CBRS_FLOATING)) == (CBRS_SIZE_DYNAMIC|CBRS_FLOATING))   // +0x124 & 5 == 5
//             dwMode |= LM_HORZ | LM_MRUWIDTH;                              // 6
//         else if (dwStyle & CBRS_ORIENT_HORZ /*0xa000*/)
//             dwMode |= LM_HORZ | LM_HORZDOCK;                              // 0xa
//         else
//             dwMode |= LM_VERTDOCK;                                        // 0x10
//         CSize size = CalcDynamicLayout(-1, dwMode);                       // vslot 92
//         size.cx = min(size.cx, sizeAvail.cx);  size.cy = min(size.cy, sizeAvail.cy);
//         if (dwStyle & CBRS_ORIENT_HORZ) {
//             lpLayout->sizeTotal.cy += size.cy;                            // +0x1c
//             lpLayout->sizeTotal.cx = max(lpLayout->sizeTotal.cx, size.cx);   // +0x18
//             if (dwStyle & CBRS_ALIGN_TOP /*0x2000*/) lpLayout->rect.top += size.cy;
//             else if (dwStyle & CBRS_ALIGN_BOTTOM /*0x8000*/) { rect.top = rect.bottom - size.cy; lpLayout->rect.bottom -= size.cy; }
//         } else if (dwStyle & CBRS_ORIENT_VERT /*0x5000*/) {
//             lpLayout->sizeTotal.cx += size.cx;
//             lpLayout->sizeTotal.cy = max(lpLayout->sizeTotal.cy, size.cy);
//             if (dwStyle & CBRS_ALIGN_LEFT /*0x1000*/) lpLayout->rect.left += size.cx;
//             else if (dwStyle & CBRS_ALIGN_RIGHT /*0x4000*/) { rect.left = rect.right - size.cx; lpLayout->rect.right -= size.cx; }
//         }
//         rect.right = rect.left + size.cx;  rect.bottom = rect.top + size.cy;
//         if (lpLayout->hDWP != NULL) AfxRepositionWindow(lpLayout, m_hWnd, &rect);   // +0, 0x28ecc0
//     }
//     return 0;
// vslot 103 goes to this file's RecalcDelayShow thunk; vslot 92 is
// devirtualised by CallCalcDynamicLayout.
// Symbol: ?OnSizeParent@CControlBar@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSizeParent_CControlBar__QEAA_J_K_J_Z(CControlBar* pThis, UINT64 wParam, __int64 lParam) {
    (void)wParam;
    if (!pThis) return 0;
    SizeParentParams* lpLayout = reinterpret_cast<SizeParentParams*>(lParam);
    const DWORD dwStyle = impl__RecalcDelayShow_CControlBar__UEAAKPEAUAFX_SIZEPARENTPARAMS___Z(pThis, lpLayout);   // vslot 103
    if ((dwStyle & WS_VISIBLE) && (dwStyle & kCBRS_ALIGN_ANY) && lpLayout != nullptr) {
        CRect rect(lpLayout->rect);
        const int cxAvail = rect.Width();
        const int cyAvail = rect.Height();

        DWORD dwMode = lpLayout->bStretch ? kLM_STRETCH : 0;
        if ((pThis->m_dwStyle & (kCBRS_SIZE_DYNAMIC | kCBRS_FLOATING)) == (kCBRS_SIZE_DYNAMIC | kCBRS_FLOATING)) {
            dwMode |= kLM_HORZ | kLM_MRUWIDTH;
        } else if (dwStyle & kCBRS_ORIENT_HORZ) {
            dwMode |= kLM_HORZ | kLM_HORZDOCK;
        } else {
            dwMode |= kLM_VERTDOCK;
        }

        CSize size = CallCalcDynamicLayout(pThis, -1, dwMode);   // vslot 92
        if (size.cx > cxAvail) size.cx = cxAvail;
        if (size.cy > cyAvail) size.cy = cyAvail;

        if (dwStyle & kCBRS_ORIENT_HORZ) {
            lpLayout->sizeTotal.cy += size.cy;
            if (size.cx > lpLayout->sizeTotal.cx) lpLayout->sizeTotal.cx = size.cx;
            if (dwStyle & kCBRS_ALIGN_TOP) {
                lpLayout->rect.top += size.cy;
            } else if (dwStyle & kCBRS_ALIGN_BOTTOM) {
                rect.top = rect.bottom - size.cy;
                lpLayout->rect.bottom -= size.cy;
            }
        } else if (dwStyle & kCBRS_ORIENT_VERT) {
            lpLayout->sizeTotal.cx += size.cx;
            if (size.cy > lpLayout->sizeTotal.cy) lpLayout->sizeTotal.cy = size.cy;
            if (dwStyle & kCBRS_ALIGN_LEFT) {
                lpLayout->rect.left += size.cx;
            } else if (dwStyle & kCBRS_ALIGN_RIGHT) {
                rect.left = rect.right - size.cx;
                lpLayout->rect.right -= size.cx;
            }
        }

        rect.right = rect.left + size.cx;
        rect.bottom = rect.top + size.cy;
        if (lpLayout->hDWP != nullptr) {
            impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
                lpLayout, pThis->m_hWnd, static_cast<const RECT*>(rect));
        }
    }
    return 0;
}
// Transcribed from retail RVA 0x1d7b50 (mfc140u):
//     if (pDC == NULL) AfxThrowInvalidArgException();                             // 0x227720
//     if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)             // +0x124 & 0x400001 == 0x400000
//         if (!DrawThemedGripper(pDC, rect, FALSE)) DrawNonThemedGripper(pDC, rect);   // vslot 99, vslot 100
// Both vslot calls go to this file's CControlBar thunks (file header).
// Symbol: ?DrawGripper@CControlBar@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect) {
    if (!pThis) return;
    if (pDC == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    if ((pThis->m_dwStyle & (kCBRS_GRIPPER | kCBRS_FLOATING)) == kCBRS_GRIPPER) {
        if (!impl__DrawThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect__H_Z(pThis, pDC, rect, FALSE)) {   // vslot 99
            impl__DrawNonThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect___Z(pThis, pDC, rect);           // vslot 100
        }
    }
}
// Transcribed from retail RVA 0x1d7bd0 (mfc140u): identical to DrawGripper
// (0x1d7b50) except that DrawThemedGripper is called with TRUE (`mov $1,%r9d`):
//     if (pDC == NULL) AfxThrowInvalidArgException();
//     if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
//         if (!DrawThemedGripper(pDC, rect, TRUE)) DrawNonThemedGripper(pDC, rect);    // vslot 99, vslot 100
// Symbol: ?DrawNCGripper@CControlBar@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawNCGripper_CControlBar__UEAAXPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect) {
    if (!pThis) return;
    if (pDC == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    if ((pThis->m_dwStyle & (kCBRS_GRIPPER | kCBRS_FLOATING)) == kCBRS_GRIPPER) {
        if (!impl__DrawThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect__H_Z(pThis, pDC, rect, TRUE)) {    // vslot 99
            impl__DrawNonThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect___Z(pThis, pDC, rect);           // vslot 100
        }
    }
}
// Transcribed from retail RVA 0x1d7870 (mfc140u):
//     if (pDC == NULL) AfxThrowInvalidArgException();                              // 0x227720
//     if (m_dwStyle & CBRS_ORIENT_HORZ /*0xa000*/)                                  // +0x124
//         pDC->Draw3dRect(rect.left + 2, rect.top + m_cyTopBorder,                  // 0x2a5c00; +0xfc
//                         3, rect.bottom - m_cyBottomBorder - rect.top - m_cyTopBorder,   // +0x100
//                         afxData.clrBtnHilite, afxData.clrBtnShadow);              // 0x3c32f8 / 0x3c32f4
//     else
//         pDC->Draw3dRect(rect.left + m_cyTopBorder, rect.top + 2,
//                         rect.right - m_cyBottomBorder - rect.left - m_cyTopBorder, 3,
//                         afxData.clrBtnHilite, afxData.clrBtnShadow);
//     return TRUE;
// (Retail uses the *vertical* border members for the horizontal extent of a
// vertical gripper, exactly as shown.)  The afxData reads are replaced as
// described at the top of the file; CDC::Draw3dRect(int,int,int,int,COLORREF,
// COLORREF) exists in this DLL only through CDC's C++ class, so its two
// FillSolidRect halves are drawn with the FillSolidRect thunk -- the same GDI
// calls CDC::Draw3dRect makes (top and left edges in clrTopLeft, bottom and
// right edges in clrBottomRight, each one pixel wide).
// Symbol: ?DrawNonThemedGripper@CControlBar@@UEAAHPEAVCDC@@AEBVCRect@@@Z
extern "C" int MS_ABI impl__DrawNonThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect___Z(CControlBar* pThis, CDC* pDC, const CRect& rect) {
    if (!pThis) return FALSE;
    if (pDC == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    int x, y, cx, cy;
    if (pThis->m_dwStyle & kCBRS_ORIENT_HORZ) {
        x = rect.left + 2;
        y = rect.top + pThis->m_cyTopBorder;
        cx = 3;
        cy = rect.bottom - pThis->m_cyBottomBorder - rect.top - pThis->m_cyTopBorder;
    } else {
        x = rect.left + pThis->m_cyTopBorder;
        y = rect.top + 2;
        cx = rect.right - pThis->m_cyBottomBorder - rect.left - pThis->m_cyTopBorder;
        cy = 3;
    }
    const COLORREF clrTopLeft = ClrBtnHilite();
    const COLORREF clrBottomRight = ClrBtnShadow();
    // CDC::Draw3dRect(x, y, cx, cy, clrTopLeft, clrBottomRight):
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, x, y, cx - 1, 1, clrTopLeft);
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, x, y, 1, cy - 1, clrTopLeft);
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, x + cx, y, -1, cy, clrBottomRight);
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(pDC, x, y + cy, cx, -1, clrBottomRight);
    return TRUE;
}
// Transcribed from retail RVA 0x1d7930 (mfc140u):
//     if (pDC == NULL) AfxThrowInvalidArgException();                        // 0x227720
//     if (m_hReBarTheme == NULL) return FALSE;                                // +0x118
//     CRect rectClient(0,0,0,0), rectWindow(0,0,0,0);
//     if (bNCArea) {
//         ::GetClientRect(m_hWnd, &rectClient);
//         ::GetWindowRect(m_hWnd, &rectWindow); CWnd::ScreenToClient(&rectWindow);   // 0x2a32b0
//         ::OffsetRect(&rectClient, -rectWindow.left, -rectWindow.top);
//     }
//     SIZE sz; int part; CRect rectGripper, rectClip;
//     if (m_dwStyle & CBRS_ORIENT_HORZ /*0xa000*/) {                          // +0x124
//         part = RP_GRIPPERVERT /*2*/;
//         if (FAILED(::GetThemePartSize(m_hReBarTheme, pDC->m_hDC, part, 0, NULL, TS_TRUE /*1*/, &sz))) return FALSE;
//         int x = rect.left;
//         if (bNCArea) { int d = rectClient.left - rect.left; if (d < sz.cx) --d; x = rect.left + (d - sz.cx) / 2; }
//         rectGripper = CRect(x, rect.top + m_cyTopBorder, x + sz.cx, rect.bottom - m_cyBottomBorder);        // +0xfc / +0x100
//         rectClip    = CRect(rect.left, rect.top + m_cyTopBorder, rectClient.left, rect.bottom - m_cyBottomBorder);
//     } else {
//         part = RP_GRIPPER /*1*/;
//         if (FAILED(::GetThemePartSize(m_hReBarTheme, pDC->m_hDC, part, 0, NULL, TS_TRUE, &sz))) return FALSE;
//         int y = rect.top;
//         if (bNCArea) { int d = rectClient.top - rect.top; if (d < sz.cy) --d; y = rect.top + (d - sz.cy) / 2; }
//         rectGripper = CRect(rect.left + m_cyTopBorder, y, rect.right - m_cyBottomBorder, y + sz.cy);
//         rectClip    = CRect(rect.left + m_cyTopBorder, rect.top, rect.right - m_cyBottomBorder, rectClient.top);
//     }
//     return SUCCEEDED(::DrawThemeBackground(m_hReBarTheme, pDC->m_hDC, part, 0, &rectGripper, &rectClip));
// (The `if (d < sz) --d` before the signed /2 is what retail emits:
// `cmp; jge; dec; sub; cltd; sub; sar` -- transcribed literally.)
// Symbol: ?DrawThemedGripper@CControlBar@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__DrawThemedGripper_CControlBar__UEAAHPEAVCDC__AEBVCRect__H_Z(CControlBar* pThis, CDC* pDC, const CRect& rect, int bNCArea) {
    if (!pThis) return FALSE;
    if (pDC == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    HTHEME hTheme = static_cast<HTHEME>(pThis->m_hReBarTheme);
    if (hTheme == nullptr) return FALSE;

    CRect rectClient;
    CRect rectWindow;
    if (bNCArea) {
        ::GetClientRect(pThis->m_hWnd, static_cast<RECT*>(rectClient));
        ::GetWindowRect(pThis->m_hWnd, static_cast<RECT*>(rectWindow));
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pThis, static_cast<RECT*>(rectWindow));
        ::OffsetRect(static_cast<RECT*>(rectClient), -rectWindow.left, -rectWindow.top);
    }

    SIZE sz = {};
    int part;
    CRect rectGripper;
    CRect rectClip;
    if (pThis->m_dwStyle & kCBRS_ORIENT_HORZ) {
        part = kRP_GRIPPERVERT;
        if (FAILED(::GetThemePartSize(hTheme, pDC->m_hDC, part, 0, nullptr, TS_TRUE, &sz))) return FALSE;
        int x = rect.left;
        if (bNCArea) {
            int d = rectClient.left - rect.left;
            if (d < sz.cx) --d;
            x = rect.left + (d - sz.cx) / 2;
        }
        rectGripper = CRect(x, rect.top + pThis->m_cyTopBorder, x + sz.cx, rect.bottom - pThis->m_cyBottomBorder);
        rectClip = CRect(rect.left, rect.top + pThis->m_cyTopBorder, rectClient.left, rect.bottom - pThis->m_cyBottomBorder);
    } else {
        part = kRP_GRIPPER;
        if (FAILED(::GetThemePartSize(hTheme, pDC->m_hDC, part, 0, nullptr, TS_TRUE, &sz))) return FALSE;
        int y = rect.top;
        if (bNCArea) {
            int d = rectClient.top - rect.top;
            if (d < sz.cy) --d;
            y = rect.top + (d - sz.cy) / 2;
        }
        rectGripper = CRect(rect.left + pThis->m_cyTopBorder, y, rect.right - pThis->m_cyBottomBorder, y + sz.cy);
        rectClip = CRect(rect.left + pThis->m_cyTopBorder, rect.top, rect.right - pThis->m_cyBottomBorder, rectClient.top);
    }
    const HRESULT hr = ::DrawThemeBackground(hTheme, pDC->m_hDC, part, 0,
                                             static_cast<const RECT*>(rectGripper), static_cast<const RECT*>(rectClip));
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
// Symbol: ?GetBarInfo@CControlBar@@QEAAXPEAVCControlBarInfo@@@Z
extern "C" void MS_ABI impl__GetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo___Z(CControlBar* pThis, void* pInfo) {
    // CControlBarInfo layout (from global_ccontrolbarinfo.cpp):
    // offset 0: m_nBarID, offset 4: m_bVisible, offset 8: m_bFloating,
    // offset 12: m_bHorz, offset 16: m_bDockBar, offset 20: m_pointPos (x,y),
    // offset 28: m_nMRUWidth, offset 32: m_bDocking, offset 40: m_rectMRUDockPos,
    // offset 56: m_dwMRUFloatStyle, offset 60: m_ptMRUFloatPos, offset 72: m_arrBarID
    if (!pInfo) return;
    unsigned char* p = (unsigned char*)pInfo;
    *(UINT*)(p + 0)  = pThis->m_hWnd ? (UINT)::GetWindowLongPtrW(pThis->m_hWnd, GWLP_ID) : 0;
    *(int*)(p + 4)   = (pThis->m_hWnd && ::IsWindowVisible(pThis->m_hWnd));
    *(int*)(p + 8)   = ((pThis->m_dwStyle & CBRS_FLOATING) != 0);
    *(int*)(p + 12)  = ((pThis->m_dwStyle & (CBRS_TOP | CBRS_BOTTOM)) == 0);  // bHorz
    *(int*)(p + 16)  = FALSE;  // CControlBar is not a dock bar
    // m_pointPos: use current window position
    if (pThis->m_hWnd) {
        RECT rc = {};
        ::GetWindowRect(pThis->m_hWnd, &rc);
        *(LONG*)(p + 20) = rc.left;
        *(LONG*)(p + 24) = rc.top;
    }
    *(UINT*)(p + 28)  = pThis->m_nMRUWidth;
    *(int*)(p + 32)   = (pThis->m_dwDockStyle != 0);
    *(DWORD*)(p + 56) = pThis->m_dwDockStyle;
}
// Symbol: ?SetBarInfo@CControlBar@@QEAAXPEAVCControlBarInfo@@PEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__SetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo__PEAVCFrameWnd___Z(CControlBar* pThis, void* pInfo, CFrameWnd* pFrame) {
    if (!pInfo) return;
    unsigned char* p = (unsigned char*)pInfo;
    int bVisible = *(int*)(p + 4);
    DWORD dwDockStyle = *(DWORD*)(p + 56);
    pThis->m_dwDockStyle = dwDockStyle;
    pThis->m_nMRUWidth = *(UINT*)(p + 28);
    if (pFrame) {
        pThis->m_pDockSite = pFrame;
    }
    if (pThis->m_hWnd && bVisible) {
        ::ShowWindow(pThis->m_hWnd, SW_SHOW);
    }
}
// Transcribed from retail RVA 0x1d71f0 (mfc140u; vftable slot 103 in the
// CDialogBar / CStatusBar / CToolBar / CDockBar vftables):
//     if (lpLayout == NULL) AfxThrowInvalidArgException();                  // 0x227720
//     DWORD dwResult = (GetStyle() & WS_VISIBLE) | (m_dwStyle & 0xFF00);    // 0x2a9690; +0x124
//     if (m_nStateFlags & (delayHide|delayShow)) {                          // +0x120 & 3
//         UINT swpFlags = 0;
//         if (m_nStateFlags & delayHide) { if (dwResult & WS_VISIBLE)  swpFlags = SWP_HIDEWINDOW; }   // 0x80
//         else                           { if (!(dwResult & WS_VISIBLE)) swpFlags = SWP_SHOWWINDOW; } // 0x40
//         if (swpFlags != 0) {
//             dwResult ^= WS_VISIBLE;
//             if (lpLayout->hDWP != NULL) {                                 // +0
//                 m_nStateFlags &= ~(delayShow|delayHide);
//                 lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, m_hWnd, NULL, 0, 0, 0, 0,
//                     swpFlags|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);   // IAT DeferWindowPos, |0x17
//             }
//         } else {
//             m_nStateFlags &= ~(delayShow|delayHide);
//         }
//     }
//     return dwResult;
// (When swpFlags is set but hDWP is NULL retail leaves the delay flags in
// place -- transcribed as is.)
// Symbol: ?RecalcDelayShow@CControlBar@@UEAAKPEAUAFX_SIZEPARENTPARAMS@@@Z
extern "C" DWORD MS_ABI impl__RecalcDelayShow_CControlBar__UEAAKPEAUAFX_SIZEPARENTPARAMS___Z(CControlBar* pThis, void* lpLayout) {
    if (!pThis) return 0;
    if (lpLayout == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    SizeParentParams* pLayout = static_cast<SizeParentParams*>(lpLayout);
    DWORD dwResult = (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_VISIBLE) | (pThis->m_dwStyle & 0xFF00);
    if (pThis->m_nStateFlags & (kDelayHide | kDelayShow)) {
        UINT swpFlags = 0;
        if (pThis->m_nStateFlags & kDelayHide) {
            if (dwResult & WS_VISIBLE) swpFlags = SWP_HIDEWINDOW;
        } else {
            if (!(dwResult & WS_VISIBLE)) swpFlags = SWP_SHOWWINDOW;
        }
        if (swpFlags != 0) {
            dwResult ^= WS_VISIBLE;
            if (pLayout != nullptr && pLayout->hDWP != nullptr) {
                pThis->m_nStateFlags &= ~(kDelayShow | kDelayHide);
                pLayout->hDWP = ::DeferWindowPos(pLayout->hDWP, pThis->m_hWnd, nullptr, 0, 0, 0, 0,
                    swpFlags | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
        } else {
            pThis->m_nStateFlags &= ~(kDelayShow | kDelayHide);
        }
    }
    return dwResult;
}
CControlBar::CControlBar()
    : m_pInPlaceOwner(nullptr), m_bAutoDelete(FALSE),
      m_cxLeftBorder(0), m_cxRightBorder(0), m_cyTopBorder(0), m_cyBottomBorder(0),
      m_cxDefaultGap(0), m_nMRUWidth(0), m_nCount(0), m_pData(nullptr),
      m_hReBarTheme(nullptr), m_nStateFlags(0), m_dwStyle(0), m_dwDockStyle(0),
      m_pDockSite(nullptr), m_pDockBar(nullptr), m_pDockContext(nullptr) {
}
CControlBar::~CControlBar() {}
BOOL CControlBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;
    DWORD winStyle = dwStyle & 0xFFFF;
    if ((winStyle & WS_CHILD) == 0) winStyle |= WS_CHILD;

    m_hWnd = ::CreateWindowExW(0, L"STATIC", nullptr, winStyle,
                               0, 0, 0, 0,
                               pParentWnd->GetSafeHwnd(),
                               (HMENU)(UINT_PTR)nID,
                               AfxGetInstanceHandle(), nullptr);

    if (!m_hWnd) return FALSE;

    ::SetWindowLongPtrW(m_hWnd, GWLP_ID, nID);
    if (dwStyle & WS_VISIBLE) {
        ::ShowWindow(m_hWnd, SW_SHOW);
    }
    return TRUE;
}
