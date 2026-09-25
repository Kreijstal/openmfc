// CMFCDropDownFrame — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Object model (read before adding a body).  CMFCDropDownFrame is the popup
// frame a CMFCDropDownToolbarButton opens; it derives CMiniFrameWnd and EMBEDS
// the CMFCDropDownToolBar it shows (afxdropdowntoolbar.h:67).  It is not
// declared in include/openmfc, so there is no C++ type, no constructor state
// and no vftable for it in this DLL: the exported ctor below is still the
// generated `return pThis;` stub.  The bodies in this file follow the
// convention of CMFCPopupMenu.cpp -- they address the RETAIL layout on the
// assumption that the caller's object is retail-shaped -- and they reach every
// sibling / base behaviour through exported impl__ thunks.
//
// Retail layout, pinned from the constructor (RVA 0x5d2a0 (mfc140u); its body
// was read from the mfc140.dll twin at 0x5d0f0, which has the same instruction
// stream with different RIP-relative displacements) and from the
// bodies cited at each function:
//   CMiniFrameWnd base                       0x000 .. 0x1f0 (see core/frame/CMiniFrameWnd.cpp)
//   BOOL  m_bAutoDestroyParent               0x1f0   (ctor stores 1)
//   CMFCDropDownToolbarButton* m_pParentBtn  0x1f8   (ctor stores NULL)
//   CMFCDropDownToolBar m_wndToolBar         0x200   (ctor runs ??0CMFCToolBar@@QEAA@XZ on
//                                                     this+0x200, then stores the
//                                                     CMFCDropDownToolBar vftable -- mfc140.dll
//                                                     VA 0x1802e5cb8 -- and 1 at toolbar+0x10b8)
//   CMFCDropDownToolBar* m_pWndOriginToolbar 0x1550  (ctor stores NULL; Create stores its 5th arg)
//   int   m_x                                0x1558
//   int   m_y                                0x155c
//   BOOL  m_bAutoDestroy                     0x1560  (ctor stores 1; the dtor tests it)
//   CString m_strCaption                     0x1568  (ctor builds an empty CString)
//   sizeof                                   0x1570  (CreateObject allocates 0x1570; the
//                                                     CRuntimeClass in mfc140.dll says 0x1570)
// The embedded toolbar is a retail CMFCToolBar (0x1350 bytes, per its
// CRuntimeClass), so it spans exactly 0x200..0x1550.  Its HWND is at
// 0x200 + 0x40.
//
// CMFCDropDownToolbarButton members read here (its ctor, mfc140.dll 0x5de30,
// stores them): m_pWndParent 0x80 (CMFCToolBarButton, afxmfc.h pins it @128),
// m_pToolBar 0x88, m_strName 0x90, m_pPopupMenu 0x98.  CMFCToolBarButton
// m_nStyle is at 0x28 and m_rect at 0x68 (afxmfc.h @40 / @104).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition (file named); the
// parameter lists are derived from the mangled names and match those
// definitions.
// ---------------------------------------------------------------------------
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                  // core/runtime/CObject.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis); // core/window/Thunks.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" void MS_ABI impl__OnSize_CFrameWnd__IEAAXIHH_Z(CFrameWnd* pThis, unsigned int nType, int cx, int cy); // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__OnDestroy_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);            // core/frame/Thunks.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd); // core/gdi/CPaintDC.cpp
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                     // core/gdi/CPaintDC.cpp
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clr);                                  // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight); // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);          // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis); // featurepack/docking/CBasePane.cpp
extern "C" CSize* MS_ABI impl__CalcSize_CMFCToolBar__UEAA_AVCSize__H_Z(
    CMFCToolBar* pThis, CSize* pRet, int bVertDock);                                     // featurepack/toolbar/CMFCToolBar.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ();      // featurepack/menu/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ(); // featurepack/menu/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();      // featurepack/toolbar/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCDropDownFrame__SAPEAUCRuntimeClass__XZ(); // featurepack/controls/RuntimeClasses.cpp
extern "C" void MS_ABI impl__OnCancelMode_CMFCDropDownToolbarButton__UEAAXXZ(void* pThis);     // featurepack/toolbar/CMFCDropDownToolbarButton.cpp

// Exported data (storage in the files named).  Retail addresses are mfc140u.
extern "C" std::int32_t  impl__m_bCustomizeMode_CMFCToolBar__1HA;          // featurepack/toolbar/StaticData.cpp (0x1803be35c)
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];   // featurepack/CMFC_misc_stubs.cpp (0x1803c1620)

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads HWNDs at +0x40");

namespace {

// CMFCDropDownFrame (see the file header).
constexpr int kOffHWnd                = 0x40;     // CWnd::m_hWnd
constexpr int kOffAutoDestroyParent   = 0x1f0;    // BOOL m_bAutoDestroyParent
constexpr int kOffParentBtn           = 0x1f8;    // CMFCDropDownToolbarButton* m_pParentBtn
constexpr int kOffWndToolBar          = 0x200;    // CMFCDropDownToolBar m_wndToolBar (embedded)
constexpr int kOffWndOriginToolbar    = 0x1550;   // CMFCDropDownToolBar* m_pWndOriginToolbar
constexpr int kOffX                   = 0x1558;   // int m_x
constexpr int kOffY                   = 0x155c;   // int m_y
constexpr int kOffAutoDestroy         = 0x1560;   // BOOL m_bAutoDestroy
constexpr int kOffStrCaption          = 0x1568;   // CString m_strCaption
constexpr int kSizeofDropDownFrame    = 0x1570;
static_assert(kOffWndToolBar + 0x1350 == kOffWndOriginToolbar, "embedded CMFCToolBar is 0x1350 bytes");
static_assert(kOffStrCaption + 8 == kSizeofDropDownFrame, "m_strCaption is the last member");
static_assert(kOffAutoDestroyParent < kOffParentBtn && kOffAutoDestroy < kOffStrCaption, "layout order");

// CMFCToolBarButton / CMFCDropDownToolbarButton members read by the bodies.
constexpr int kOffBtnStyle            = 0x28;     // UINT  m_nStyle
constexpr int kOffBtnRectTop          = 0x6c;     // CRect m_rect (+0x68) .top
constexpr int kOffBtnRectBottom       = 0x74;     // CRect m_rect .bottom
constexpr int kOffBtnWndParent        = 0x80;     // CWnd* m_pWndParent
constexpr int kOffBtnPopupMenu        = 0x98;     // CMFCDropDownFrame* m_pPopupMenu (CMFCDropDownToolbarButton)
constexpr unsigned kTbbsPressed       = 0x20000;  // TBBS_PRESSED: retail `btrl $0x11, 0x28(btn)`

// afxGlobalData: +0 is the one-time init gate every retail reader tests
// (`if (!m_bInitialized) { Initialize(); m_bInitialized = TRUE; }`, the inline
// GetGlobalData()); the colour slots are the ones pinned by
// core/runtime/AFX_GLOBAL_DATA.cpp (clrBarFace static_assert == 0x060).
constexpr int kGdInitGate             = 0x000;
constexpr int kGdClrBarFace           = 0x060;
constexpr int kGdClrBarShadow         = 0x064;
constexpr int kGdClrBarHilite         = 0x068;
constexpr int kGdClrBarDkShadow       = 0x06c;
constexpr int kGdClrBarLight          = 0x070;

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}
inline HWND HWndOf(const void* pWnd) { return At<HWND>(pWnd, kOffHWnd); }
inline void* ToolBarOf(void* pThis) { return static_cast<char*>(pThis) + kOffWndToolBar; }

inline void EnsureGlobalData() {
    unsigned char* g = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    int init;
    std::memcpy(&init, g + kGdInitGate, sizeof init);
    if (init == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(g);
        const int one = 1;
        std::memcpy(g + kGdInitGate, &one, sizeof one);
    }
}
inline COLORREF GlobalColor(int off) {
    EnsureGlobalData();
    COLORREF c;
    std::memcpy(&c, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof c);
    return c;
}

// DYNAMIC_DOWNCAST as retail inlines it: NULL stays NULL, otherwise IsKindOf.
inline void* DownCast(void* pObj, CRuntimeClass* pClass) {
    if (pObj == nullptr) return nullptr;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pObj), pClass)
               ? pObj : nullptr;
}

// CBasePane::IsHorizontal() -- slot 0x2d8 of the retail CMFCToolBar vftable
// (mfc140.dll VA 0x180313618) is the non-exported 0x88e0 (mfc140.dll), whose
// whole body is `return GetCurrentAlignment() /*vslot 0x338*/ & 0xa000;`
// (CBRS_ORIENT_HORZ).  Slot 0x338 of that vftable is
// ?GetCurrentAlignment@CBasePane@@UEBAKXZ, reached here through its thunk.
// DEVIATION: both calls are statically bound, so a parent toolbar class that
// overrides IsHorizontal / GetCurrentAlignment is not honoured.
// Returns the raw masked value, as retail does (RecalcLayout passes it
// straight through as CalcSize's BOOL argument).
inline BOOL ParentBarIsHorizontal(void* pBar) {
    return static_cast<BOOL>(
        impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pBar)) & 0xa000u);
}

} // namespace

// Symbol: ??0CMFCDropDownFrame@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownFrame__QEAA_XZ(void* pThis) {
    return pThis;
}
// Retail (RVA 0x5d3a0 (mfc140u); body read from the mfc140.dll twin at
// 0x5d1f0), decoded:
//     vfptr = CMFCDropDownFrame vftable;
//     m_wndToolBar.m_Buttons.RemoveAll();          // on this+0x1388 (toolbar+0x1188); the
//                                                  // call at 0x5d3be targets 0x8350 (mfc140u),
//                                                  // exported as both ?RemoveAll@CObList@@QEAAXXZ
//                                                  // and ?RemoveAll@CPtrList@@QEAAXXZ (folded).
//                                                  // OnCreate fills the list with the origin
//                                                  // toolbar's buttons, which it does not own
//     if (m_bAutoDestroy) m_wndToolBar.DestroyWindow();   // +0x1560; toolbar vslot 0xd0 =
//                                                  // ?DestroyWindow@CWnd@@UEAAHXZ
//     m_strCaption.~CString();                     // +0x1568, inlined refcount release
//     m_wndToolBar vfptr = CMFCDropDownToolBar vftable; ~CMFCToolBar(&m_wndToolBar);
//     tail-jump ~CMiniFrameWnd(this);
// STUB: the exported constructor above does not build any of that state (no
// vftable, no embedded CMFCToolBar, no CString), so tearing it down here would
// run ~CMFCToolBar / ~CString / ~CMiniFrameWnd over memory nothing
// constructed.  The parameter list is corrected to the mangled name's
// (this only, void return); the body stays empty until the ctor is real.
// Symbol: ??1CMFCDropDownFrame@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCDropDownFrame__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x5d450 (mfc140u); body read from the mfc140.dll twin at
// 0x5d2a0), decoded:
//     ENSURE(pWndParent != NULL);                       // else AfxThrowInvalidArgException
//     AFXPlaySystemSound(AFX_SOUND_MENU_POPUP /*2*/);
//     m_strClassName = AfxRegisterWndClass(CS_SAVEBITS /*0x800*/,
//         ::LoadCursor(NULL, IDC_ARROW /*0x7f00*/), (HBRUSH)(COLOR_BTNFACE + 1) /*0x10*/, NULL);
//     m_pWndOriginToolbar = pWndOriginToolbar;          // +0x1550
//     if (x == -1 && y == -1) {                         // centre-less default position
//         CRect rc; ::GetClientRect(pWndParent->m_hWnd, &rc); pWndParent->ClientToScreen(&rc);
//         m_x = rc.left + 5; m_y = rc.top + 5;
//     } else { m_x = x; m_y = y; }                      // +0x1558 / +0x155c
//     DWORD dwExStyle = pWndParent->m_hWnd ? (pWndParent->GetExStyle() & WS_EX_LAYOUTRTL) : 0;
//     CRect rect(x, y, x, y);                           // the ORIGINAL x/y, not m_x/m_y
//     if (pWndParent->GetOwner() != NULL) pWndParent = pWndParent->GetOwner();
//                                                       // (m_hWndOwner +0xa0, else ::GetParent)
//     if (!CMiniFrameWnd::CreateEx(dwExStyle, m_strClassName, m_strCaption, WS_POPUP,
//                                  rect, pWndParent, 0)) return FALSE;
//     ShowWindow(SW_SHOWNOACTIVATE /*4*/);
//     return TRUE;
// STUB: the class-name static (m_strClassName, below) is not a usable CString
// in this build, m_strCaption is never constructed (see the ctor), and
// creating the HWND would bind it to an object that has no vftable, so the
// first message routed to it could not dispatch.  Returns FALSE (Create
// failed); the parameter list is corrected to the mangled name's.
// Symbol: ?Create@CMFCDropDownFrame@@UEAAHPEAVCWnd@@HHPEAVCMFCDropDownToolBar@@@Z
extern "C" int MS_ABI impl__Create_CMFCDropDownFrame__UEAAHPEAVCWnd__HHPEAVCMFCDropDownToolBar___Z(
    void* pThis, CWnd* pWndParent, int x, int y, void* pWndOriginToolbar) {
    (void)pThis; (void)pWndParent; (void)x; (void)y; (void)pWndOriginToolbar;
    return FALSE;
}

// Retail (RVA 0x5d220 (mfc140u); mfc140.dll twin 0x5d070): operator new(0x1570)
// and, when that succeeds, the constructor.  STUB: there is no constructor
// state or vftable to install (see the file header), so no usable object can
// be produced; nullptr -- the allocation-failure result -- is returned, as
// CMFCColorBar / CMFCAutoHideBar::CreateObject do for the same reason.
// Symbol: ?CreateObject@CMFCDropDownFrame@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCDropDownFrame__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail (RVA 0x5ddd0 (mfc140u); body read from the mfc140.dll twin at
// 0x5dc10), fully transcribed:
//     if (m_pParentBtn == NULL) return NULL;                         // +0x1f8
//     return DYNAMIC_DOWNCAST(CMFCMenuBar, m_pParentBtn->m_pWndParent); // +0x80, IsKindOf
// Symbol: ?GetParentMenuBar@CMFCDropDownFrame@@QEBAPEAVCMFCMenuBar@@XZ
extern "C" void* MS_ABI impl__GetParentMenuBar_CMFCDropDownFrame__QEBAPEAVCMFCMenuBar__XZ(const void* pThis) {
    if (!pThis) return nullptr;
    void* pBtn = At<void*>(pThis, kOffParentBtn);
    if (pBtn == nullptr) return nullptr;
    return DownCast(At<void*>(pBtn, kOffBtnWndParent), impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ());
}

// Retail (RVA 0x5dd60 (mfc140u); body read from the mfc140.dll twin at
// 0x5dba0), fully transcribed:
//     if (m_pParentBtn == NULL) return NULL;
//     CMFCPopupMenuBar* pBar = DYNAMIC_DOWNCAST(CMFCPopupMenuBar, m_pParentBtn->m_pWndParent);
//     if (pBar == NULL) return NULL;
//     return DYNAMIC_DOWNCAST(CMFCDropDownFrame, pBar->GetParentFrame());
// Symbol: ?GetParentPopupMenu@CMFCDropDownFrame@@QEBAPEAV1@XZ
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCDropDownFrame__QEBAPEAV1_XZ(const void* pThis) {
    if (!pThis) return nullptr;
    void* pBtn = At<void*>(pThis, kOffParentBtn);
    if (pBtn == nullptr) return nullptr;
    void* pBar = DownCast(At<void*>(pBtn, kOffBtnWndParent),
                          impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ());
    if (pBar == nullptr) return nullptr;
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pBar));
    return DownCast(pFrame, impl__GetThisClass_CMFCDropDownFrame__SAPEAUCRuntimeClass__XZ());
}

// Retail (RVA 0x5de90 (mfc140u)), fully transcribed:
//     if (!bActive && !CMFCToolBar::m_bCustomizeMode)                // 0x1803be35c (mfc140u)
//         ::SendMessageW(m_hWnd, WM_CLOSE, 0, 0);                    // IAT 0x1802c7120 (mfc140u)
// Symbol: ?OnActivateApp@CMFCDropDownFrame@@IEAAXHK@Z
extern "C" void MS_ABI impl__OnActivateApp_CMFCDropDownFrame__IEAAXHK_Z(void* pThis, int bActive, unsigned long dwThreadID) {
    (void)dwThreadID;
    if (!pThis) return;
    if (bActive == 0 && impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        ::SendMessage(HWndOf(pThis), WM_CLOSE, 0, 0);
    }
}

// Retail (RVA 0x5d610 (mfc140u)), decoded:
//     if (lpCreateStruct == NULL) AfxThrowInvalidArgException();
//     if (CFrameWnd::OnCreateHelper(lpCreateStruct, lpCreateStruct->lpCreateParams) == -1)
//         return -1;
//     pParentBar = DYNAMIC_DOWNCAST(CMFCToolBar, m_pParentBtn ? m_pParentBtn->m_pWndParent : NULL);
//     BOOL bHorz = pParentBar ? pParentBar->IsHorizontal() /*vslot 0x2d8*/ : TRUE;
//     if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT /*0x800*/,
//             bHorz ? 0x50005000 : 0x5000a000,            // neg/sbb/and 0xffffb000/add 0x5000a000:
//                                                         // WS_CHILD|WS_VISIBLE|CBRS_ALIGN_LEFT|RIGHT
//                                                         // for a horizontal parent, TOP|BOTTOM else
//             CRect(<16-byte constant>), 0xe827)) return -1; // toolbar vslot 0x658 = CMFCToolBar::CreateEx
//     m_wndToolBar.<+0x10b8> = TRUE;                     // this+0x12b8
//     six CMFCToolBarImages::CopyTemp calls copying m_pWndOriginToolbar's
//         image lists (+0x3f8/+0x590/+0x728/+0x8c0/+0xa58/+0xbf0) into m_wndToolBar's;
//     four QWORDs copied from origin +0x1158..+0x1170 to toolbar +0x1158..+0x1170;
//     clear bit 22 of toolbar +0x104 (m_dwStyle);
//     toolbar +0xa0 (m_hWndOwner) = origin's owner-or-parent HWND (via CWnd::FromHandle);
//     toolbar +0x10f4 = origin +0x10f4;
//     m_wndToolBar.m_Buttons.AddTail(&m_pWndOriginToolbar->m_Buttons); // CObList::AddTail(CObList*)
//     RecalcLayout(TRUE);                                // this vslot 0x300
//     ::ReleaseCapture(); CWnd::FromHandle(::SetCapture(m_wndToolBar.m_hWnd));
//     return 0;
// STUB: every step after the base call works on the embedded
// CMFCDropDownToolBar, which the stub constructor never builds, and the
// CMFCToolBar internals it copies (image lists, button list, the +0x10b8 /
// +0x10f4 / +0x1158 fields) are not modelled by any shadow struct here.
// Returns -1 (creation refused) rather than 0, so that a window whose toolbar
// was never created is not left up; the parameter list is corrected.
// Symbol: ?OnCreate@CMFCDropDownFrame@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCDropDownFrame__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)pThis; (void)lpCreateStruct;
    return -1;
}

// Retail (RVA 0x5dc60 (mfc140u); same instruction stream as the mfc140.dll
// twin at 0x5daa0, only the RIP-relative displacements differ), fully
// transcribed:
//     if (m_pParentBtn != NULL) {                                   // +0x1f8
//         m_pParentBtn->m_pPopupMenu = NULL;                        // +0x98
//         m_pParentBtn->m_nStyle &= ~TBBS_PRESSED;                  // btrl $0x11, +0x28
//         CMFCToolBar* pBar = DYNAMIC_DOWNCAST(CMFCToolBar, m_pParentBtn->m_pWndParent);
//         if (pBar != NULL) {
//             CPoint pt(0, 0);
//             ::GetCursorPos(&pt);                                   // IAT 0x1802c7320 (mfc140u)
//             ::ScreenToClient(pBar->m_hWnd, &pt);                   // IAT 0x1802c7318
//             ::SendMessageW(pBar->m_hWnd, WM_LBUTTONUP, 0,          // IAT 0x1802c7120
//                            MAKELPARAM(pt.x, pt.y));                // sign-extended (movslq)
//         }
//     }
//     CFrameWnd::OnDestroy();                                        // tail-jump
// Symbol: ?OnDestroy@CMFCDropDownFrame@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCDropDownFrame__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    void* pBtn = At<void*>(pThis, kOffParentBtn);
    if (pBtn != nullptr) {
        At<void*>(pBtn, kOffBtnPopupMenu) = nullptr;
        At<unsigned int>(pBtn, kOffBtnStyle) &= ~kTbbsPressed;
        void* pBar = DownCast(At<void*>(pBtn, kOffBtnWndParent),
                              impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ());
        if (pBar != nullptr) {
            POINT pt = {0, 0};
            ::GetCursorPos(&pt);
            ::ScreenToClient(HWndOf(pBar), &pt);
            const LPARAM lp = static_cast<LPARAM>(static_cast<LONG>(MAKELONG(pt.x, pt.y)));
            ::SendMessage(HWndOf(pBar), WM_LBUTTONUP, 0, lp);
        }
    }
    impl__OnDestroy_CFrameWnd__IEAAXXZ(reinterpret_cast<CFrameWnd*>(pThis));
}

// Retail (RVA 0x5de10 (mfc140u); body read from the mfc140.dll twin at
// 0x5dc50), fully transcribed:
//     CRect rect(0, 0, 0, 0);
//     ::GetClientRect(m_hWnd, &rect);                                // IAT 0x1802c7330 (mfc140u)
//     pDC->FillSolidRect(&rect, GetGlobalData()->clrBarFace);        // afxGlobalData +0x60
//     return TRUE;
// Symbol: ?OnEraseBkgnd@CMFCDropDownFrame@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCDropDownFrame__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (!pThis) return TRUE;
    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(HWndOf(pThis), &rect);
    impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rect, GlobalColor(kGdClrBarFace));
    return TRUE;
}

// ?OnMouseActivate@CMFCDropDownFrame@@IEAAHPEAVCWnd@@II@Z has no entry in either
// symbol map; resolved through the mfc140u export table by ordinal (urva.py)
// to RVA 0x5da20 (mfc140u), whose whole body is
//     mov $0x3,%eax ; ret           -> return MA_NOACTIVATE;
// (COMDAT-folded: the same bytes are also ?GetBindInfo@CAsyncMonikerFile@@MEBAKXZ.)
// Symbol: ?OnMouseActivate@CMFCDropDownFrame@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CMFCDropDownFrame__IEAAHPEAVCWnd__II_Z(
    void* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    (void)pThis; (void)pDesktopWnd; (void)nHitTest; (void)message;
    return MA_NOACTIVATE;
}

// Retail (RVA 0x5d8f0 (mfc140u); body read from the mfc140.dll twin at
// 0x5d740), fully transcribed:
//     CPaintDC dc(this);
//     CRect rect(0, 0, 0, 0);
//     ::GetClientRect(m_hWnd, &rect);                                // IAT 0x1802c7330 (mfc140u)
//     dc.Draw3dRect(&rect, GetGlobalData()->clrBarLight,             // +0x70
//                          GetGlobalData()->clrBarDkShadow);         // +0x6c
//     ::InflateRect(&rect, -1, -1);                                  // IAT 0x1802c72e8
//     dc.Draw3dRect(&rect, GetGlobalData()->clrBarHilite,            // +0x68
//                          GetGlobalData()->clrBarShadow);           // +0x64
// CPaintDC exists in this DLL only as its thunks, so it is built in raw
// storage and torn down through the matching thunk (as CMFCPopupMenu::OnPaint
// does).
// Symbol: ?OnPaint@CMFCDropDownFrame@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCDropDownFrame__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, reinterpret_cast<CWnd*>(pThis));

    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(HWndOf(pThis), &rect);
    const COLORREF clrDkShadow = GlobalColor(kGdClrBarDkShadow);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, GlobalColor(kGdClrBarLight), clrDkShadow);
    ::InflateRect(&rect, -1, -1);
    const COLORREF clrShadow = GlobalColor(kGdClrBarShadow);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, GlobalColor(kGdClrBarHilite), clrShadow);

    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail (RVA 0x5d870 (mfc140u); body read from the mfc140.dll twin at
// 0x5d6c0), fully transcribed:
//     CFrameWnd::OnSize(nType, cx, cy);
//     if (m_wndToolBar.GetSafeHwnd() != NULL)                        // this+0x200, hwnd +0x40
//         m_wndToolBar.SetWindowPos(NULL, 2, 2, cx - 4, cy - 4,
//                                   SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/, NULL);
// The SetWindowPos is toolbar vslot 0x480; in the retail CMFCDropDownToolBar
// vftable (mfc140.dll VA 0x1802e5cb8) that slot is
// ?SetWindowPos@CBasePane@@UEAAPEAXPEBVCWnd@@HHHHIPEAX@Z, whose hdwp == NULL
// path (mfc140.dll 0xb6a0) is exactly `CWnd::SetWindowPos(pWndInsertAfter, x,
// y, cx, cy, nFlags); return NULL;`.  m_wndToolBar is embedded by value, so
// its dynamic type is exactly CMFCDropDownToolBar and binding that path
// statically is not a deviation.  (The exported CBasePane::SetWindowPos thunk
// is not used: it re-dispatches through the object's C++ vptr.)
// Symbol: ?OnSize@CMFCDropDownFrame@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCDropDownFrame__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    if (!pThis) return;
    impl__OnSize_CFrameWnd__IEAAXIHH_Z(reinterpret_cast<CFrameWnd*>(pThis), nType, cx, cy);
    void* pToolBar = ToolBarOf(pThis);
    if (HWndOf(pToolBar) != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(static_cast<CWnd*>(pToolBar), nullptr,
                                                    2, 2, cx - 4, cy - 4,
                                                    SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

// Retail (RVA 0x5dd20 (mfc140u); same instruction stream as the mfc140.dll
// twin at 0x5db60, only the RIP-relative displacements differ), decoded:
//     if (m_pParentBtn != NULL) m_pParentBtn->OnCancelMode();       // +0x1f8, button vslot 0xc0
//     delete this;                                                   // tail-jump, this vslot 1, flag 1
// There is no base-class call.  Slot 0xc0 of the retail
// CMFCDropDownToolbarButton vftable (mfc140u VA 0x1802e7688; mfc140.dll VA
// 0x1802e55d8) is ?OnCancelMode@CMFCDropDownToolbarButton@@UEAAXXZ.
// PARTIAL: the notification is forwarded to that class's exported thunk
// (featurepack/toolbar/CMFCDropDownToolbarButton.cpp) instead of through the
// button's vtable -- DEVIATION for a client class that overrides OnCancelMode
// on a CMFCDropDownToolbarButton subclass.  The `delete this` is deliberately
// OMITTED: this class has no vftable here, so neither the scalar deleting
// destructor slot nor the matching allocator is reachable (the same choice,
// for the same reason, as CMFCPopupMenu::PostNcDestroy).
// Symbol: ?PostNcDestroy@CMFCDropDownFrame@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CMFCDropDownFrame__MEAAXXZ(void* pThis) {
    if (!pThis) return;
    void* pBtn = At<void*>(pThis, kOffParentBtn);
    if (pBtn != nullptr) {
        impl__OnCancelMode_CMFCDropDownToolbarButton__UEAAXXZ(pBtn);
    }
}

// Retail (RVA 0x5da30 (mfc140u); body read from the mfc140.dll twin at
// 0x5d870), fully transcribed (bNotify is not read):
//     if (!::IsWindow(m_hWnd) || !::IsWindow(m_wndToolBar.m_hWnd)) return;
//     CMFCToolBar* pParentBar = m_pParentBtn
//         ? DYNAMIC_DOWNCAST(CMFCToolBar, m_pParentBtn->m_pWndParent) : NULL;
//     CSize size = m_wndToolBar.CalcSize(pParentBar ? pParentBar->IsHorizontal() : TRUE);
//                                                    // toolbar vslot 0x558 = CMFCToolBar::CalcSize
//                                                    // (CMFCDropDownToolBar vftable, mfc140.dll
//                                                    // VA 0x1802e5cb8); parent vslot 0x2d8
//     size.cx += 6; size.cy += 6;
//     CRect rectScreen(0, 0, 0, 0);
//     MONITORINFO mi; mi.cbSize = sizeof(MONITORINFO) /*0x28*/;
//     if (::GetMonitorInfoW(::MonitorFromPoint(CPoint(m_x, m_y), MONITOR_DEFAULTTONEAREST), &mi))
//         ::CopyRect(&rectScreen, &mi.rcWork);
//     else
//         ::SystemParametersInfoW(SPI_GETWORKAREA /*0x30*/, 0, &rectScreen, 0);
//     if (m_x + size.cx > rectScreen.right) {
//         if (pParentBar != NULL && !pParentBar->IsHorizontal()) {
//             CRect rectParent(0, 0, 0, 0); ::GetWindowRect(pParentBar->m_hWnd, &rectParent);
//             m_x = rectParent.left - size.cx;
//         } else
//             m_x = rectScreen.right - rectScreen.left - size.cx - 1;
//     }
//     if (m_y + size.cy > rectScreen.bottom) {
//         m_y -= size.cy;
//         if (m_pParentBtn != NULL)
//             m_y += m_pParentBtn->m_rect.top - m_pParentBtn->m_rect.bottom - 4;
//         else if (m_y < 0)
//             m_y = 0;
//     }
//     CWnd::SetWindowPos(NULL, m_x, m_y, size.cx, size.cy, SWP_NOZORDER | SWP_NOACTIVATE);
// (IAT slots, mfc140u: IsWindow 0x1802c7138, MonitorFromPoint 0x1802c6c48,
// GetMonitorInfoW 0x1802c6c40, CopyRect 0x1802c7218, SystemParametersInfoW
// 0x1802c6c50, GetWindowRect 0x1802c6c08.)
// m_wndToolBar.CalcSize is bound statically to the CMFCToolBar::CalcSize
// thunk: the member is embedded by value, so that IS its final overrider.
// DEVIATION: pParentBar->IsHorizontal() is bound statically too (see
// ParentBarIsHorizontal above), which ignores a derived parent toolbar's
// override.  Note that the CMFCToolBar::CalcSize thunk is itself still
// partial (it returns CSize(0,0), retail's empty-toolbar answer).
// Symbol: ?RecalcLayout@CMFCDropDownFrame@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMFCDropDownFrame__UEAAXH_Z(void* pThis, int bNotify) {
    (void)bNotify;
    if (!pThis) return;
    if (!::IsWindow(HWndOf(pThis))) return;
    void* pToolBar = ToolBarOf(pThis);
    if (!::IsWindow(HWndOf(pToolBar))) return;

    void* pBtn = At<void*>(pThis, kOffParentBtn);
    CRuntimeClass* pToolBarClass = impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
    void* pParentBar = pBtn ? DownCast(At<void*>(pBtn, kOffBtnWndParent), pToolBarClass) : nullptr;

    CSize size(0, 0);
    impl__CalcSize_CMFCToolBar__UEAA_AVCSize__H_Z(static_cast<CMFCToolBar*>(pToolBar), &size,
                                                  pParentBar ? ParentBarIsHorizontal(pParentBar) : TRUE);
    size.cx += 6;
    size.cy += 6;

    RECT rectScreen = {0, 0, 0, 0};
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);
    POINT pt = {At<int>(pThis, kOffX), At<int>(pThis, kOffY)};
    if (::GetMonitorInfo(::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &mi)) {
        ::CopyRect(&rectScreen, &mi.rcWork);
    } else {
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
    }

    int& x = At<int>(pThis, kOffX);
    int& y = At<int>(pThis, kOffY);
    if (x + size.cx > rectScreen.right) {
        // Retail re-reads m_pParentBtn->m_pWndParent and repeats the downcast
        // here; the fields cannot change in between, so the result is reused.
        if (pParentBar != nullptr && !ParentBarIsHorizontal(pParentBar)) {
            RECT rectParent = {0, 0, 0, 0};
            ::GetWindowRect(HWndOf(pParentBar), &rectParent);
            x = rectParent.left - size.cx;
        } else {
            x = rectScreen.right - rectScreen.left - size.cx - 1;
        }
    }
    if (y + size.cy > rectScreen.bottom) {
        y -= size.cy;
        if (pBtn != nullptr) {
            y += At<int>(pBtn, kOffBtnRectTop) - At<int>(pBtn, kOffBtnRectBottom) - 4;
        } else if (y < 0) {
            y = 0;
        }
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(reinterpret_cast<CWnd*>(pThis), nullptr,
                                                x, y, size.cx, size.cy,
                                                SWP_NOZORDER | SWP_NOACTIVATE);
}

// Data export (a protected static CString, mfc140u VA 0x1803c1610), not a
// function.  Retail's Create assigns it the AfxRegisterWndClass result before
// every CreateEx.  STUB: left as the null CStringT slot it was, like the other
// m_strClassName exports (CMFCRibbonKeyTip, CMFCPropertyGridToolTipCtrl):
// handing out a CStringData this build's CStringT does not own would be worse
// than an untouched slot, and Create (above) is itself still a stub.
// Symbol: ?m_strClassName@CMFCDropDownFrame@@1V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__m_strClassName_CMFCDropDownFrame__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = {};
