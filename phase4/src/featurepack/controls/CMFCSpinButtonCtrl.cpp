// CMFCSpinButtonCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ===========================================================================
// How this class is modelled here.
//
// include/openmfc/afxmfc.h only forward-declares CMFCSpinButtonCtrl; there is
// no OpenMFC C++ class and no detail/ shadow header, so every thunk below
// takes `void* pThis` and reaches the members through the in-file SpinLayout
// view.  Every body was transcribed from the retail disassembly (disas.py;
// function bodies are byte-identical between mfc140.dll and mfc140u.dll, the
// entry RVAs quoted below are mfc140u's).
//
// Retail object layout (shipping afxspinbuttonctrl.h:30-42 for the member
// ORDER; offsets from the ctor, entry RVA 0x132520 (mfc140u), and from the
// handlers below, all of which address these fields off %rcx):
//   +0x000  CSpinButtonCtrl base == a bare CWnd, 0xe8 bytes (the retail ctor
//           calls ??0CWnd@@QEAA@XZ (RVA 0x28a700, mfc140u) directly; m_hWnd at +0x40)
//   +0x0e8  BOOL m_bIsButtonPressedUp
//   +0x0ec  BOOL m_bIsButtonPressedDown
//   +0x0f0  BOOL m_bIsButtonHighlightedUp
//   +0x0f4  BOOL m_bIsButtonHighlightedDown
//   +0x0f8  BOOL m_bTracked
//   sizeof == 0x100 (featurepack/toolbar/CMFCToolBarSpinEditBoxButton.cpp
//   embeds one at +0xb0 and its next member sits at +0x1b0).
//
// Retail message map (0x18030fed0 in mfc140u, dumped with msgmap_u.py; base
// map = CWnd): WM_PAINT OnPaint, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_CANCELMODE,
// WM_MOUSEMOVE, WM_ERASEBKGND, WM_MOUSELEAVE.  OpenMFC's map for this class
// (featurepack/controls/MessageMaps.cpp / detail/Mfc07MsgmapSupport.cpp) is
// empty, so OpenMFC's own routing does not reach these handlers yet.
//
// VFTABLE.  Retail installs &CMFCSpinButtonCtrl::`vftable' (0x18030ffe8,
// mfc140u).  OpenMFC has no C++ class and hence no vftable for this type, so
// the constructor leaves the vptr that ??0CWnd@@ installed (OpenMFC's CWnd
// one) and records it, and the destructor reinstalls it -- the same
// convention as featurepack/toolbar/CMFCToolBarSpinEditBoxButton.cpp's
// g_ownVptr.  Consequently the one virtual call retail makes through that
// table (OnPaint -> OnDraw, vslot 93 / +0x2e8) is made as a direct call to
// this file's OnDraw thunk; a client override of OnDraw is not reached.
// ===========================================================================

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMemDCSupport.h"   // S_Cmemdc view + the CMemDC thunk declarations

#include <atomic>
#include <cstring>

// ---- external thunks (definitions checked in the named files) --------------
// Signatures follow the mangled names and match those definitions.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                  // core/window/CtorDtorPlacement.cpp:22
extern "C" void MS_ABI impl___1CSpinButtonCtrl__UEAA_XZ(CSpinButtonCtrl* pThis);             // core/controls/RuntimeClasses.cpp:479
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                         // core/window/Thunks.cpp:1183
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);              // core/window/Thunks.cpp:1378
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);                 // core/window/CWnd.cpp:786
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd); // core/gdi/CPaintDC.cpp:12
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                           // core/gdi/CPaintDC.cpp:29
extern "C" void MS_ABI impl__DrawRect_CDrawingManager__QEAAXAEBVCRect__KK_Z(
    void* pThis, const CRect* pRect, unsigned long clrFill, unsigned long clrLine);            // core/gdi/CDrawingManager.cpp:1246
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);                // core/runtime/AFX_GLOBAL_DATA.cpp:675
extern "C" void MS_ABI impl__OnDrawSpinButtons_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int nState, BOOL bOrientation,
    CMFCSpinButtonCtrl* pSpinCtrl);                                                            // featurepack/visualmanager/CMFCVisualManager.cpp:691
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                      // featurepack/CMFC_misc_stubs.cpp:3652
extern "C" std::int32_t impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA;                        // featurepack/toolbar/StaticData.cpp:72
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                            // core/runtime/StaticData.cpp:56

extern "C" void MS_ABI impl__OnDraw_CMFCSpinButtonCtrl__UEAAXPEAVCDC___Z(void* pThis, CDC* pDC);

namespace {

// Retail layout (see the file header).
struct SpinLayout {
    unsigned char cwnd[0xe8];          // CSpinButtonCtrl / CWnd base
    BOOL m_bIsButtonPressedUp;         // +0xe8
    BOOL m_bIsButtonPressedDown;       // +0xec
    BOOL m_bIsButtonHighlightedUp;     // +0xf0
    BOOL m_bIsButtonHighlightedDown;   // +0xf4
    BOOL m_bTracked;                   // +0xf8
    int  _pad;                         // +0xfc
};
static_assert(sizeof(CWnd) == 0xe8, "OpenMFC CWnd is 0xe8 bytes, as retail's (CMFCSpinButtonCtrl state starts at +0xe8)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd at +0x40 (retail reads 0x40(%rcx))");
static_assert(offsetof(SpinLayout, m_bIsButtonPressedUp) == 0xe8, "");
static_assert(offsetof(SpinLayout, m_bIsButtonPressedDown) == 0xec, "");
static_assert(offsetof(SpinLayout, m_bIsButtonHighlightedUp) == 0xf0, "");
static_assert(offsetof(SpinLayout, m_bIsButtonHighlightedDown) == 0xf4, "");
static_assert(offsetof(SpinLayout, m_bTracked) == 0xf8, "");
static_assert(sizeof(SpinLayout) == 0x100, "retail sizeof(CMFCSpinButtonCtrl) == 0x100");
static_assert(offsetof(CDC, m_hDC) == 0x8, "CDC::m_hDC at +0x8 (OnDraw's FillRect reads 0x8(%rbp))");

inline SpinLayout* L(void* pThis) { return static_cast<SpinLayout*>(pThis); }
inline CWnd* W(void* pThis) { return static_cast<CWnd*>(pThis); }
inline HWND HwndOf(void* pThis) { return W(pThis)->m_hWnd; }

// The vptr OpenMFC's ??0CWnd@@ installs, recorded by the constructor and
// reinstalled by the destructor (see the file header, VFTABLE).
std::atomic<void*> g_ownVptr{nullptr};

// CPoint passed by value is an 8-byte aggregate in a register under MS_ABI.
inline POINT PointOf(long long point) {
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<std::int32_t>(point & 0xffffffffLL));
    pt.y = static_cast<LONG>(static_cast<std::int32_t>(static_cast<unsigned long long>(point) >> 32));
    return pt;
}

// The client-rect split both OnLButtonDown and OnMouseMove perform inline
// (identical code in each):
//     CRect rect; ::GetClientRect(m_hWnd, &rect);          // zero-initialised first
//     CRect rectUp = rect, rectDown = rect;
//     if (GetStyle() & UDS_HORZ (0x40)) {                    // test $0x40,%al
//         int x = (rect.left + rect.right) / 2;              // cltd; sub; sar -> C division
//         rectUp.left = x;  rectDown.right = x;
//     } else {
//         int y = (rect.top + rect.bottom) / 2;
//         rectUp.bottom = y;  rectDown.top = y;
//     }
void SplitClientRect(void* pThis, RECT& rectUp, RECT& rectDown) {
    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(HwndOf(pThis), &rect);
    rectUp = rect;
    rectDown = rect;
    if (impl__GetStyle_CWnd__QEBAKXZ(W(pThis)) & UDS_HORZ) {
        const int x = (rect.right + rect.left) / 2;
        rectUp.left = x;
        rectDown.right = x;
    } else {
        const int y = (rect.bottom + rect.top) / 2;
        rectUp.bottom = y;
        rectDown.top = y;
    }
}

// CWnd::RedrawWindow() with its default arguments as retail expands it:
// ::RedrawWindow(m_hWnd, NULL, NULL, 0x105) -- RDW_INVALIDATE | RDW_UPDATENOW
// | RDW_ERASE (IAT 0x1802c7130 (mfc140u) = USER32!RedrawWindow); no m_hWnd test.
inline void RedrawSelf(void* pThis) {
    ::RedrawWindow(HwndOf(pThis), nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

// The inlined afxGlobalData gate: if (!m_bInitialized) { Initialize(); m_bInitialized = 1; }
// (OnDraw 0x1326ca..0x1326de / 0x132705..0x132719, mfc140u).
inline void EnsureGlobalDataInitialized() {
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
}

} // namespace

// Symbol: ??0CMFCSpinButtonCtrl@@QEAA@XZ
// Transcribed from retail entry RVA 0x132520 (mfc140u):
//     CWnd::CWnd();                                   // call 0x28a700 = ??0CWnd@@QEAA@XZ (CSpinButtonCtrl's own ctor is inline and adds nothing)
//     vfptr = &CMFCSpinButtonCtrl::`vftable';         // 0x18030ffe8
//     8 bytes at +0xf4 = 0; 8 bytes at +0xe8 = 0; 4 bytes at +0xf0 = 0;
//     return this;
// i.e. all five BOOLs start FALSE.  DEVIATION: the vftable store is not
// reproduced (none exists in OpenMFC); the CWnd vptr is recorded instead.
extern "C" void* MS_ABI impl___0CMFCSpinButtonCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;   // deviation: retail has no NULL check
    impl___0CWnd__QEAA_XZ(pThis);
    g_ownVptr.store(*static_cast<void**>(pThis), std::memory_order_relaxed);
    SpinLayout* d = L(pThis);
    d->m_bIsButtonPressedUp = FALSE;
    d->m_bIsButtonPressedDown = FALSE;
    d->m_bIsButtonHighlightedUp = FALSE;
    d->m_bIsButtonHighlightedDown = FALSE;
    d->m_bTracked = FALSE;
    return pThis;
}

// Symbol: ??1CMFCSpinButtonCtrl@@UEAA@XZ
// Transcribed from retail entry RVA 0x1325b0 (mfc140u):
//     vfptr = &CMFCSpinButtonCtrl::`vftable';         // 0x18030ffe8
//     CSpinButtonCtrl::~CSpinButtonCtrl();            // tail jump to 0x296040
// DEVIATION: the vptr reinstalled is the one the constructor recorded (file
// header).  That matters because OpenMFC's ??1CSpinButtonCtrl@@ runs a
// virtual `pThis->~CSpinButtonCtrl()`, which must not dispatch through a
// derived class's table left behind by the derived destructor.
extern "C" void MS_ABI impl___1CMFCSpinButtonCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    void* vptr = g_ownVptr.load(std::memory_order_relaxed);
    if (vptr != nullptr) *static_cast<void**>(pThis) = vptr;
    impl___1CSpinButtonCtrl__UEAA_XZ(static_cast<CSpinButtonCtrl*>(W(pThis)));
}

// Symbol: ?OnCancelMode@CMFCSpinButtonCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x1328f0 (mfc140u):
//     CWnd::Default();                                // call 0x28ac80, first
//     8 bytes at +0xe8 = 0;  8 bytes at +0xf0 = 0;    // both Pressed and both Highlighted = FALSE
// No redraw and m_bTracked is left alone.
extern "C" void MS_ABI impl__OnCancelMode_CMFCSpinButtonCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    SpinLayout* d = L(pThis);
    d->m_bIsButtonPressedUp = FALSE;
    d->m_bIsButtonPressedDown = FALSE;
    d->m_bIsButtonHighlightedUp = FALSE;
    d->m_bIsButtonHighlightedDown = FALSE;
}

// Symbol: ?OnDraw@CMFCSpinButtonCtrl@@UEAAXPEAVCDC@@@Z
// Transcribed from retail entry RVA 0x132670 (mfc140u):
//     CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);          // zeroed first; IAT 0x1802c7330
//     if (CMFCToolBarImages::m_bIsDrawOnGlass) {                          // 0x1803be394
//         CDrawingManager dm(*pDC);                                       // vftable 0x1802e7188 + &CDC, on the stack
//         <afxGlobalData gate>
//         dm.DrawRect(rectClient, afxGlobalData.clrWindow (+0x78), (COLORREF)-1);   // call 0x5ada0
//     } else {
//         <afxGlobalData gate>
//         ::FillRect(pDC->m_hDC, &rectClient, afxGlobalData.brWindow);    // HBRUSH at +0x110; IAT 0x1802c7208
//     }
//     int nState = 0;
//     if (m_bIsButtonPressedUp)       nState |= 1;                        // +0xe8
//     if (m_bIsButtonPressedDown)     nState |= 2;                        // +0xec
//     if (m_bIsButtonHighlightedUp)   nState |= 4;                        // +0xf0
//     if (m_bIsButtonHighlightedDown) nState |= 8;                        // +0xf4
//     if (!IsWindowEnabled())         nState |= 0x10;                     // call 0x2a9b00
//     CMFCVisualManager::GetInstance()->OnDrawSpinButtons(pDC, rectClient, nState,
//         (GetStyle() & UDS_HORZ) == UDS_HORZ, this);                     // call 0x9774; GetStyle 0x2a9690 (shr $6; and $1); vslot 114 (+0x390)
// (0x9774 (mfc140u) is not exported: it is the out-of-line copy of the
// afxvisualmanager.h inline GetInstance -- it reads ?m_pVisManager@ and, when
// NULL, creates one from ?m_pRTIDefault@ or with new CMFCVisualManager.)
// DEVIATIONS:
//  * GetInstance: retail's helper lazily creates the default manager when
//    ?m_pVisManager@CMFCVisualManager@@ is NULL.  That creation exists in
//    this tree only as the C++ static CMFCVisualManager::GetInstance()
//    (visualmanager/CMFCVisualManager.cpp:715); there is no impl__ thunk for
//    it, and calling the C++ method from this translation unit adds a new C++
//    undefined that checkfile.sh's link audit rejects.  So -- as
//    featurepack/docking/CPaneDivider.cpp::OnPaint does -- the exported
//    pointer is read and the manager call is skipped while it is NULL.
//  * The manager call goes through the exported
//    ?OnDrawSpinButtons@CMFCVisualManager@@ thunk, not vslot 114.  That export
//    in featurepack/visualmanager/CMFCVisualManager.cpp is the BASE body (a
//    qualified call), so an installed OfficeXP / Windows manager's override is
//    not reached.  In addition, that export currently forwards its 5th
//    argument (bOrientation per the mangled name) as the C++ body's
//    bIsHovered and hard-codes bOrientation=TRUE, so the bHorz passed here
//    does not yet mean "horizontal" on the other side; both are reported for
//    the owner of CMFCVisualManager.cpp to fix.
//  * The CDrawingManager is the { vptr, CDC* } shape with a NULL vptr, as
//    featurepack/visualmanager/CMFCVisualManagerOffice2003.cpp builds it;
//    OpenMFC's DrawRect reads only the CDC*.
//  * NULL pThis / pDC return early; retail has no such checks.
extern "C" void MS_ABI impl__OnDraw_CMFCSpinButtonCtrl__UEAAXPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr) return;
    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(HwndOf(pThis), &rectClient);

    if (impl__m_bIsDrawOnGlass_CMFCToolBarImages__2HA != 0) {
        struct { const void* vptr; CDC* pDC; } dm = { nullptr, pDC };   // CDrawingManager { vptr, CDC& }
        EnsureGlobalDataInitialized();
        COLORREF clrWindow = 0;
        std::memcpy(&clrWindow, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x78, sizeof clrWindow);
        const CRect rect(rectClient);
        impl__DrawRect_CDrawingManager__QEAAXAEBVCRect__KK_Z(&dm, &rect, clrWindow, static_cast<unsigned long>(-1));
    } else {
        EnsureGlobalDataInitialized();
        HBRUSH hbrWindow = nullptr;
        std::memcpy(&hbrWindow, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x110, sizeof hbrWindow);
        ::FillRect(pDC->m_hDC, &rectClient, hbrWindow);
    }

    const SpinLayout* d = L(pThis);
    int nState = 0;
    if (d->m_bIsButtonPressedUp) nState |= 1;
    if (d->m_bIsButtonPressedDown) nState |= 2;
    if (d->m_bIsButtonHighlightedUp) nState |= 4;
    if (d->m_bIsButtonHighlightedDown) nState |= 8;
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(W(pThis))) nState |= 0x10;

    CMFCVisualManager* pVisManager = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVisManager != nullptr) {   // deviation: retail creates the default manager here
        const BOOL bHorz = (impl__GetStyle_CWnd__QEBAKXZ(W(pThis)) & UDS_HORZ) == UDS_HORZ;
        impl__OnDrawSpinButtons_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHPEAVCMFCSpinButtonCtrl___Z(
            pVisManager, pDC, CRect(rectClient), nState, bHorz, static_cast<CMFCSpinButtonCtrl*>(pThis));
    }
}

// Symbol: ?OnEraseBkgnd@CMFCSpinButtonCtrl@@IEAAHPEAVCDC@@@Z
// The export (ordinal 9774 in mfc140u) resolves to RVA 0x3a60 (mfc140u), a
// COMDAT-folded `mov $0x1,%eax; ret` shared with other trivial bodies; the
// WM_ERASEBKGND entry of the retail message map points at the same address.
// So: return TRUE without erasing (OnPaint paints the whole client area).
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCSpinButtonCtrl__IEAAHPEAVCDC___Z(void* /*pThis*/, CDC* /*pDC*/) {
    return TRUE;
}

// Symbol: ?OnLButtonDown@CMFCSpinButtonCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x132800 (mfc140u); nFlags is never read:
//     <split the client rect into rectUp / rectDown -- SplitClientRect above>
//     m_bIsButtonPressedUp   = ::PtInRect(&rectUp, point);     // +0xe8; IAT 0x1802c72f8
//     m_bIsButtonPressedDown = ::PtInRect(&rectDown, point);   // +0xec
//     CWnd::Default();                                         // call 0x28ac80
// Retail issues no RedrawWindow of its own here.
extern "C" void MS_ABI impl__OnLButtonDown_CMFCSpinButtonCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int /*nFlags*/, long long point) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    RECT rectUp, rectDown;
    SplitClientRect(pThis, rectUp, rectDown);
    const POINT pt = PointOf(point);
    SpinLayout* d = L(pThis);
    d->m_bIsButtonPressedUp = ::PtInRect(&rectUp, pt);
    d->m_bIsButtonPressedDown = ::PtInRect(&rectDown, pt);
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Symbol: ?OnLButtonUp@CMFCSpinButtonCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x1328d0 (mfc140u); nFlags / point are never read:
//     8 bytes at +0xe8 = 0;  8 bytes at +0xf0 = 0;    // both Pressed and both Highlighted = FALSE
//     CWnd::Default();                                // tail jump to 0x28ac80
extern "C" void MS_ABI impl__OnLButtonUp_CMFCSpinButtonCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int /*nFlags*/, long long /*point*/) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    SpinLayout* d = L(pThis);
    d->m_bIsButtonPressedUp = FALSE;
    d->m_bIsButtonPressedDown = FALSE;
    d->m_bIsButtonHighlightedUp = FALSE;
    d->m_bIsButtonHighlightedDown = FALSE;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Symbol: ?OnMouseLeave@CMFCSpinButtonCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x132a70 (mfc140u); no Default() call:
//     m_bTracked = FALSE;                                               // +0xf8
//     if (m_bIsButtonPressedUp || m_bIsButtonPressedDown ||
//         m_bIsButtonHighlightedUp || m_bIsButtonHighlightedDown) {     // +0xe8/+0xec/+0xf0/+0xf4
//         8 bytes at +0xf0 = 0;          // only the two Highlighted flags; Pressed is kept
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);                    // IAT 0x1802c7130
//     }
extern "C" void MS_ABI impl__OnMouseLeave_CMFCSpinButtonCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    SpinLayout* d = L(pThis);
    d->m_bTracked = FALSE;
    if (d->m_bIsButtonPressedUp || d->m_bIsButtonPressedDown ||
        d->m_bIsButtonHighlightedUp || d->m_bIsButtonHighlightedDown) {
        d->m_bIsButtonHighlightedUp = FALSE;
        d->m_bIsButtonHighlightedDown = FALSE;
        RedrawSelf(pThis);
    }
}

// Symbol: ?OnMouseMove@CMFCSpinButtonCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x132920 (mfc140u):
//     BOOL bWasHighlightedUp = m_bIsButtonHighlightedUp;                 // +0xf0, read first
//     BOOL bWasHighlightedDown = m_bIsButtonHighlightedDown;             // +0xf4
//     <split the client rect into rectUp / rectDown -- SplitClientRect above>
//     m_bIsButtonHighlightedUp   = ::PtInRect(&rectUp, point);          // IAT 0x1802c72f8
//     m_bIsButtonHighlightedDown = ::PtInRect(&rectDown, point);
//     if (nFlags & MK_LBUTTON) {                                        // test $0x1,%r15b
//         m_bIsButtonPressedUp   = m_bIsButtonHighlightedUp;
//         m_bIsButtonPressedDown = m_bIsButtonHighlightedDown;
//     }
//     CWnd::Default();                                                  // call 0x28ac80
//     if (bWasHighlightedUp != m_bIsButtonHighlightedUp ||
//         bWasHighlightedDown != m_bIsButtonHighlightedDown)
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);                    // IAT 0x1802c7130
//     if (!m_bTracked) {                                                // +0xf8
//         m_bTracked = TRUE;
//         TRACKMOUSEEVENT tme; tme.cbSize = 0x18; tme.dwFlags = TME_LEAVE (2);
//         tme.hwndTrack = m_hWnd;  ::TrackMouseEvent(&tme);             // IAT 0x1802c7310; dwHoverTime left unset
//     }
extern "C" void MS_ABI impl__OnMouseMove_CMFCSpinButtonCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    SpinLayout* d = L(pThis);
    const BOOL bWasHighlightedUp = d->m_bIsButtonHighlightedUp;
    const BOOL bWasHighlightedDown = d->m_bIsButtonHighlightedDown;

    RECT rectUp, rectDown;
    SplitClientRect(pThis, rectUp, rectDown);
    const POINT pt = PointOf(point);
    d->m_bIsButtonHighlightedUp = ::PtInRect(&rectUp, pt);
    d->m_bIsButtonHighlightedDown = ::PtInRect(&rectDown, pt);
    if (nFlags & MK_LBUTTON) {
        d->m_bIsButtonPressedUp = d->m_bIsButtonHighlightedUp;
        d->m_bIsButtonPressedDown = d->m_bIsButtonHighlightedDown;
    }

    impl__Default_CWnd__IEAA_JXZ(W(pThis));

    if (bWasHighlightedUp != d->m_bIsButtonHighlightedUp ||
        bWasHighlightedDown != d->m_bIsButtonHighlightedDown) {
        RedrawSelf(pThis);
    }
    if (d->m_bTracked == 0) {
        d->m_bTracked = TRUE;
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = HwndOf(pThis);
        tme.dwHoverTime = 0;   // retail never writes this field; unused with TME_LEAVE
        ::TrackMouseEvent(&tme);
    }
}

// Symbol: ?OnPaint@CMFCSpinButtonCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x1325d0 (mfc140u):
//     CPaintDC dc(this);                              // call 0x2a3d20
//     CMemDC memDC(dc, this);                         // call 0x69f50
//     OnDraw(&memDC.GetDC());                         // inline: m_bMemDC ? &m_dcMem : m_dc (cmpl $0,0x30(%rsp); cmove);
//                                                     // vslot 93 (+0x2e8), CFG-dispatched
//     memDC.~CMemDC();  dc.~CPaintDC();               // 0x6a380 / 0x2a3dd0
// DEVIATION: OnDraw is called directly, not through vslot 93 (file header).
extern "C" void MS_ABI impl__OnPaint_CMFCSpinButtonCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, W(pThis));
    alignas(void*) unsigned char memStorage[sizeof(S_Cmemdc)] = {};
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, pPaintDC, pThis);
    CDC* pDC = pMemDC->m_bMemDC ? reinterpret_cast<CDC*>(&pMemDC->m_dcMem) : static_cast<CDC*>(pMemDC->m_dc);
    impl__OnDraw_CMFCSpinButtonCtrl__UEAAXPEAVCDC___Z(pThis, pDC);
    impl___1CMemDC__UEAA_XZ(pMemDC);
    impl___1CPaintDC__UEAA_XZ(pPaintDC);
}
