// COleResizeBar — OpenMFC implementation.
// Sources: olecore.cpp
//
// Retail layout (afxole.h, class COleResizeBar : public CControlBar):
//     CRectTracker m_tracker;          // +0x148 .. +0x198  (sizeof(COleResizeBar) == 0x198)
// That is the ONLY data member retail adds to CControlBar. OpenMFC's
// include/openmfc/afxole.h instead declares `COleClientItem* m_pClientItem`
// (+0x148), `int m_nHandleSize` (+0x150) and `char _oleresizebar_padding[64]`
// (+0x154) -- fabricated names over the same 0x50 bytes. The total size agrees
// with retail, so this file reads m_tracker by offset through the
// ResizeBarTracker shadow below; the header's member names are not used.
//
// Offsets inside m_tracker are confirmed by the retail handler bodies
// transcribed below: m_tracker at +0x148 (`lea 0x148(<this reg>),%rcx` passed
// as `this` to CRectTracker::HitTest/SetCursor/Draw/Track), m_tracker.m_rect at +0x154,
// m_tracker.m_nHandleSize at +0x16c, and m_tracker.m_nStyle at +0x150 (the
// constructor stores 0x15 there). The CRectTracker layout itself is the one
// pinned in core/gdi/CRectTracker.cpp (harvested with cl.exe, size 80).
//
// Retail addresses in this file are mfc140u.dll RVAs unless marked otherwise.
// The mfc140u symbol map has no entries for the COleResizeBar handlers; their
// entries were taken from the WM_* entries of COleResizeBar's message map
// (mfc140u VA 0x18032b858, the AFX_MSGMAP returned by GetMessageMap at
// RVA 0x243ef0 (mfc140u)), and the bodies were read with `disas.py --u --at`.
//
// KNOWN GAP (not in this file): OpenMFC's COleResizeBar message map
// (detail/Ole10MsgmapSupport.cpp) has no entries, so these handlers are only
// reachable by direct call (e.g. from a derived class), not by message
// dispatch. COleResizeBar::Create below also does not match retail (it makes
// a STATIC window, which never routes through the MFC message map).

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Thunks called from this file (every one checked to be DEFINED where noted).
// ---------------------------------------------------------------------------
extern "C" {
// core/gdi/CRectTracker.cpp
void MS_ABI impl__Construct_CRectTracker__IEAAXXZ(void* pThis);
void MS_ABI impl__Draw_CRectTracker__QEBAXPEAVCDC___Z(const void* pThis, void* pDC);
int  MS_ABI impl__HitTest_CRectTracker__QEBAHVCPoint___Z(const void* pThis, unsigned long long point);
int  MS_ABI impl__SetCursor_CRectTracker__QEBAHPEAVCWnd__I_Z(const void* pThis, void* pWnd, unsigned int nHitTest);
int  MS_ABI impl__Track_CRectTracker__QEAAHPEAVCWnd__VCPoint__H0_Z(
        void* pThis, void* pWnd, unsigned long long point, int bAllowInvert, void* pWndClipTo);
// core/gdi/CPaintDC.cpp
CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
void      MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// core/window/CWnd.cpp
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
// core/window/Thunks.cpp
CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
void       MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
void       MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
__int64    MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
// detail/MfcExceptionsSupport.cpp
void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
// featurepack/CMFC_misc_stubs.cpp
void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
        void* lpLayout, HWND hWnd, const RECT* lpRect);
}

namespace {

// Retail CRectTracker (afxext.h), same layout as RT in core/gdi/CRectTracker.cpp.
struct ResizeBarTracker {
    const void* vfptr;        // +0x00
    UINT        m_nStyle;     // +0x08
    RECT        m_rect;       // +0x0c
    SIZE        m_sizeMin;    // +0x1c
    int         m_nHandleSize;// +0x24
    BOOL        m_bAllowInvert;// +0x28
    RECT        m_rectLast;   // +0x2c
    SIZE        m_sizeLast;   // +0x3c
    BOOL        m_bErase;     // +0x44
    BOOL        m_bFinalErase;// +0x48
};
static_assert(sizeof(ResizeBarTracker) == 0x50, "CRectTracker is 0x50 bytes (cl.exe harvest)");
static_assert(offsetof(ResizeBarTracker, m_nStyle) == 0x08, "CRectTracker::m_nStyle @0x08");
static_assert(offsetof(ResizeBarTracker, m_rect) == 0x0c, "CRectTracker::m_rect @0x0c");
static_assert(offsetof(ResizeBarTracker, m_nHandleSize) == 0x24, "CRectTracker::m_nHandleSize @0x24");

constexpr std::size_t kOffTracker = 0x148;   // COleResizeBar::m_tracker
static_assert(sizeof(CControlBar) == kOffTracker, "m_tracker follows the 0x148-byte CControlBar base");
static_assert(sizeof(COleResizeBar) == kOffTracker + sizeof(ResizeBarTracker),
              "COleResizeBar is 0x198 bytes (retail CRuntimeClass m_nObjectSize 408)");
static_assert(offsetof(COleResizeBar, m_pClientItem) == kOffTracker,
              "the header's fabricated members sit over m_tracker");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40");
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC @0x08");

// CWnd::m_hWndOwner (+0xa0), read by the inline CWnd::GetOwner. OpenMFC's CWnd
// does not name it; the slot lies in _cwnd_padding2, which the CWnd constructor
// zero-fills and which MSVC clients' inline CWnd::SetOwner writes. Same approach
// as featurepack/toolbar/CMFCToolBarComboBoxEdit.cpp.
constexpr std::size_t kOffHWndOwner = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kOffHWndOwner &&
              kOffHWndOwner + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "m_hWndOwner slot must lie inside CWnd's padding");

// AFX_SIZEPARENTPARAMS (afxpriv.h): HDWP hDWP @0, RECT rect @8. Retail
// OnSizeParent tests (lParam+0) and passes lParam+8 as the rect.
struct ResizeBarSizeParentParams {
    HDWP hDWP;
    RECT rect;
};
static_assert(offsetof(ResizeBarSizeParentParams, rect) == 8, "AFX_SIZEPARENTPARAMS::rect @8");

constexpr UINT kWM_SIZECHILD = 0x0369;       // afxpriv.h; `mov $0x369,%edx` in OnLButtonDown
constexpr UINT kDefaultTrackerStyle = 0x15;  // solidLine|hatchedBorder|resizeOutside

inline ResizeBarTracker* Tracker(void* pThis) {
    return reinterpret_cast<ResizeBarTracker*>(static_cast<char*>(pThis) + kOffTracker);
}
inline CWnd* AsWnd(void* pThis) { return static_cast<CWnd*>(pThis); }
inline HWND HWndOf(void* pThis) { return AsWnd(pThis)->m_hWnd; }
inline HWND RawOwnerHwnd(void* pThis) {
    HWND h = nullptr;
    std::memcpy(&h, static_cast<const char*>(pThis) + kOffHWndOwner, sizeof h);
    return h;
}

} // namespace

// Retail ??0COleResizeBar@@QEAA@XZ (RVA 0x243de0 mfc140u):
//     CControlBar::CControlBar();
//     vfptr = COleResizeBar::`vftable';
//     m_tracker.vfptr = CRectTracker::`vftable';
//     zero m_tracker +0x0c..+0x24 (m_rect, m_sizeMin) and +0x2c..+0x44 (m_rectLast, m_sizeLast);
//     m_tracker.Construct();
//     m_tracker.m_nStyle = 0x15;           // movl $0x15,0x150(%rbx)
// OpenMFC used to store the header's m_pClientItem = NULL / m_nHandleSize = 4
// there, i.e. a NULL tracker vfptr and m_nStyle = 4, with which the handler
// bodies below would fault (Track dispatches AdjustRect/OnChangedRect through
// the tracker vfptr). The tail is now zeroed and the tracker constructed
// through the OpenMFC CRectTracker::Construct thunk, which installs its own
// vtable (core/gdi/CRectTracker.cpp).
COleResizeBar::COleResizeBar() {
    std::memset(reinterpret_cast<char*>(this) + kOffTracker, 0, sizeof(ResizeBarTracker));
    impl__Construct_CRectTracker__IEAAXXZ(Tracker(this));
    Tracker(this)->m_nStyle = kDefaultTrackerStyle;
}
COleResizeBar::~COleResizeBar() {
    if (m_hWnd) {
        DestroyWindow();
    }
    m_pClientItem = nullptr;
}
BOOL COleResizeBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    (void)dwStyle;
    if (!pParentWnd) return FALSE;
    m_hWnd = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_GRAYRECT,
                           0, 0, 0, 0, pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                           AfxGetInstanceHandle(), nullptr);
    return m_hWnd != nullptr;
}

// Retail: the WM_ERASEBKGND entry of COleResizeBar's message map points at
// RVA 0x3a60 (mfc140u), a COMDAT-folded `mov $0x1,%eax; ret` shared with other
// return-TRUE functions -- the background is never erased (OnPaint below only
// sets the brush origin and draws the tracker).
// Symbol: ?OnEraseBkgnd@COleResizeBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_COleResizeBar__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Retail entry RVA 0x244050 (mfc140u), transcribed:
//     CFrameWnd* pFrameWnd = GetParentFrame();
//     if (pFrameWnd == NULL) AfxThrowInvalidArgException();          // ENSURE_VALID
//     CWnd* pParent = CWnd::FromHandle(::GetParent(pFrameWnd->m_hWnd)); // GetParent, inlined
//     ::UpdateWindow(pFrameWnd->m_hWnd);
//     if (pParent != NULL) {
//         ::UpdateWindow(pParent->m_hWnd);
//         CRect rect(0,0,0,0); ::GetClientRect(pParent->m_hWnd, &rect);
//         pParent->ClientToScreen(&rect);
//         ::ClipCursor(&rect);
//     }
//     CRect rectSave = m_tracker.m_rect;
//     BOOL bNotify = m_tracker.Track(this, point, FALSE, pParent);
//     CRect rectNew = m_tracker.m_rect;
//     m_tracker.m_rect = rectSave;
//     ::ClipCursor(NULL);
//     if (bNotify) {
//         CWnd* pOwner = CWnd::FromHandle(m_hWndOwner ? m_hWndOwner
//                                                     : ::GetParent(m_hWnd)); // GetOwner, inlined
//         ClientToScreen(&rectNew);
//         pOwner->ScreenToClient(&rectNew);
//         ::SendMessageW(pOwner->m_hWnd, WM_SIZECHILD /*0x369*/,
//                        ::GetDlgCtrlID(m_hWnd), (LPARAM)&rectNew);
//     }
// Import slots resolved with iatu.py: GetParent, UpdateWindow, GetClientRect,
// ClipCursor (both calls), GetDlgCtrlID, SendMessageW. nFlags is not read.
// Retail does not NULL-check pOwner before dereferencing it; neither does this.
// Symbol: ?OnLButtonDown@COleResizeBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_COleResizeBar__IEAAXIVCPoint___Z(
        void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(AsWnd(pThis));
    if (pFrameWnd == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    HWND hFrame = reinterpret_cast<CWnd*>(pFrameWnd)->m_hWnd;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hFrame));
    ::UpdateWindow(hFrame);
    if (pParent != nullptr) {
        ::UpdateWindow(pParent->m_hWnd);
        RECT rect = {0, 0, 0, 0};
        ::GetClientRect(pParent->m_hWnd, &rect);
        impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rect);
        ::ClipCursor(&rect);
    }

    ResizeBarTracker* t = Tracker(pThis);
    const RECT rectSave = t->m_rect;
    const int bNotify = impl__Track_CRectTracker__QEAAHPEAVCWnd__VCPoint__H0_Z(
        t, pThis, static_cast<unsigned long long>(point), FALSE, pParent);
    RECT rectNew = t->m_rect;
    t->m_rect = rectSave;
    ::ClipCursor(nullptr);

    if (bNotify) {
        HWND hOwner = RawOwnerHwnd(pThis);
        if (hOwner == nullptr) hOwner = ::GetParent(HWndOf(pThis));
        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOwner);
        impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pThis), &rectNew);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pOwner, &rectNew);
        const int nID = ::GetDlgCtrlID(HWndOf(pThis));
        // `mov %eax,%r8d`: retail zero-extends the int ID into WPARAM.
        ::SendMessage(pOwner->m_hWnd, kWM_SIZECHILD, static_cast<WPARAM>(static_cast<UINT>(nID)),
                      reinterpret_cast<LPARAM>(&rectNew));
    }
}

// Retail entry RVA 0x243f00 (mfc140u), transcribed:
//     CPaintDC dc(this);
//     CRect rect(0,0,0,0); ::GetWindowRect(m_hWnd, &rect);
//     ::SetBrushOrgEx(dc.m_hDC, rect.left & 7, rect.top & 7, &ptOld);   // CDC::SetBrushOrg, inlined
//     m_tracker.Draw(&dc);
//     // ~CPaintDC
// Import slots resolved with iatu.py: GetWindowRect, SetBrushOrgEx.
// Symbol: ?OnPaint@COleResizeBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_COleResizeBar__IEAAXXZ(void* pThis) {
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, AsWnd(pThis));

    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(HWndOf(pThis), &rect);
    POINT ptOld;
    ::SetBrushOrgEx(pDC->m_hDC, rect.left & 7, rect.top & 7, &ptOld);
    impl__Draw_CRectTracker__QEBAXPEAVCDC___Z(Tracker(pThis), pDC);

    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail entry RVA 0x243fe0 (mfc140u), transcribed:
//     CPoint point(0,0); ::GetCursorPos(&point);
//     ::ScreenToClient(m_hWnd, &point);
//     if (m_tracker.HitTest(point) < 0)
//         return (BOOL)Default();
//     return m_tracker.SetCursor(pWnd, nHitTest);
// Import slots resolved with iatu.py: GetCursorPos, ScreenToClient. `message`
// is not read.
// Symbol: ?OnSetCursor@COleResizeBar@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_COleResizeBar__IEAAHPEAVCWnd__II_Z(
        void* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)message;
    POINT point = {0, 0};
    ::GetCursorPos(&point);
    ::ScreenToClient(HWndOf(pThis), &point);
    const unsigned long long packed =
        static_cast<unsigned long long>(static_cast<unsigned int>(point.x)) |
        (static_cast<unsigned long long>(static_cast<unsigned int>(point.y)) << 32);
    ResizeBarTracker* t = Tracker(pThis);
    if (impl__HitTest_CRectTracker__QEBAHVCPoint___Z(t, packed) < 0)
        return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis)));
    return impl__SetCursor_CRectTracker__QEBAHPEAVCWnd__I_Z(t, pWnd, nHitTest);
}

// Retail entry RVA 0x243fa0 (mfc140u), transcribed:
//     ::GetClientRect(m_hWnd, &m_tracker.m_rect);
//     int nHandleSize = m_tracker.m_nHandleSize;         // read after GetClientRect
//     ::InflateRect(&m_tracker.m_rect, -nHandleSize, -nHandleSize);   // tail jump
// Import slots resolved with iatu.py: GetClientRect, InflateRect. nType/cx/cy
// are not read.
// Symbol: ?OnSize@COleResizeBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_COleResizeBar__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    (void)nType;
    (void)cx;
    (void)cy;
    ResizeBarTracker* t = Tracker(pThis);
    ::GetClientRect(HWndOf(pThis), &t->m_rect);
    const int nHandleSize = t->m_nHandleSize;
    ::InflateRect(&t->m_rect, -nHandleSize, -nHandleSize);
}

// Retail entry RVA 0x2441a0 (mfc140u; the WM_SIZEPARENT 0x361 map entry), transcribed:
//     AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
//     if (lpLayout->hDWP != NULL)
//         AfxRepositionWindow(lpLayout, m_hWnd, &lpLayout->rect);
//     int nHandleSize = m_tracker.m_nHandleSize;
//     ::InflateRect(&lpLayout->rect, -nHandleSize, -nHandleSize);   // unconditional
//     return 0;
// Import slot resolved with iatu.py: InflateRect. wParam is not read.
// Symbol: ?OnSizeParent@COleResizeBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSizeParent_COleResizeBar__IEAA_J_K_J_Z(
        void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    ResizeBarSizeParentParams* lpLayout = reinterpret_cast<ResizeBarSizeParentParams*>(lParam);
    if (lpLayout->hDWP != nullptr)
        impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(
            lpLayout, HWndOf(pThis), &lpLayout->rect);
    const int nHandleSize = Tracker(pThis)->m_nHandleSize;
    ::InflateRect(&lpLayout->rect, -nHandleSize, -nHandleSize);
    return 0;
}

// Retail: slot 93 (OnUpdateCmdUI, +0x2e8) of COleResizeBar's vftable (mfc140u
// VA 0x18032b990, installed by the constructor at RVA 0x243de0) points at
// RVA 0x27d0 (mfc140u), a COMDAT-folded bare `ret` -- the retail override does
// nothing. The empty body here is that behaviour, not a placeholder.
// Symbol: ?OnUpdateCmdUI@COleResizeBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_COleResizeBar__UEAAXPEAVCFrameWnd__H_Z(
        void* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    (void)pThis;
    (void)pTarget;
    (void)bDisableIfNoHndler;
}
