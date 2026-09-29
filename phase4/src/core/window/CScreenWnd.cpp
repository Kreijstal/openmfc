// CScreenWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CScreenWnd is a private MFC helper (not declared in include/openmfc): the
// full-screen, transparent popup CMFCColorDialog raises while its eyedropper
// is active.  It forwards mouse moves and the terminating left click to the
// colour dialog in the dialog's client coordinates.  Everything below is
// transcribed from the retail mfc140u.dll bodies (function bodies are
// byte-identical to the mfc140.dll twin; every RVA quoted names its image).
//
// Layout (pinned in-file because no header declares the class):
//   +0x000  CWnd base (sizeof(CWnd) == 0xe8, include/openmfc/afxwin.h)
//   +0x040  CWnd::m_hWnd
//   +0x0e8  CMFCColorDialog* m_pColorDlg -- stored by Create, read by both
//           mouse handlers (mov 0xe8(%rcx) in each).  The retail ctor does
//           NOT initialise it.
// The retail message map (GetThisMessageMap, entry 0x2a110 (mfc140u), returns
// 0x1802e1808 (mfc140u); base map CWnd) has four entries, all RVAs mfc140u:
//   WM_MOUSEMOVE   (sig 54) -> 0x2a250  OnMouseMove
//   WM_LBUTTONDOWN (sig 54) -> 0x2a2d0  OnLButtonDown
//   WM_SETCURSOR   (sig  5) -> 0xda30   whose whole body is `jmp 0x28ac80`
//                                       (CWnd::Default); no CScreenWnd export
//   WM_ERASEBKGND  (sig  1) -> 0x3a60   OnEraseBkgnd (`mov $1,%eax; ret`)
// 0xda30 and 0x3a60 are both identical-COMDAT-folded bodies shared with many
// unrelated exports.  This repo's CScreenWnd message map
// (detail/Other12MsgmapSupport.cpp) is empty, so the handlers below are
// correct when reached by export but are not yet dispatched by messages.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);    // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* lpParam);    // core/window/CWnd.cpp
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon);           // core/runtime/Globals.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszName, const wchar_t* lpszType);                               // core/runtime/Globals.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                 // core/window/Thunks.cpp

namespace {

struct ScreenWndLayout {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                      // +0x040  CWnd::m_hWnd
    unsigned char m_cwndTail[0xe8 - 0x48];
    void*         m_pColorDlg;                 // +0x0e8  CMFCColorDialog*
};
static_assert(offsetof(ScreenWndLayout, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(ScreenWndLayout, m_pColorDlg) == 0xe8, "CScreenWnd::m_pColorDlg");
static_assert(sizeof(CWnd) == 0xe8, "m_pColorDlg directly follows the CWnd base");

// CMFCColorDialog derives from CWnd, so its HWND is at +0x40 too (retail reads
// 0x40(%rax) off m_pColorDlg).
struct ColorDlgHead {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                      // +0x040
};

inline ScreenWndLayout* L(void* p) { return static_cast<ScreenWndLayout*>(p); }
inline HWND DlgHwnd(void* pDlg) { return static_cast<ColorDlgHead*>(pDlg)->m_hWnd; }

// IDC_AFXBARRES_COLOR, the eyedropper cursor (same immediate as
// CMFCImagePaintArea's kIdcColor).
constexpr UINT kIdcAfxBarresColor = 0x3f11;

// Shared body of the two retail mouse handlers (they differ only in the
// message id and in OnMouseMove's trailing Default()).
//     HWND hTo = m_pColorDlg ? m_pColorDlg->m_hWnd : NULL;
//     ::MapWindowPoints(m_hWnd, hTo, &point, 1);
//     ::SendMessage(m_pColorDlg->m_hWnd, msg, nFlags, MAKELPARAM(point.x, point.y));
// DEVIATION: retail tests m_pColorDlg only for the MapWindowPoints target and
// then dereferences it unconditionally for SendMessage (a fault when NULL);
// here the SendMessage is skipped when m_pColorDlg is NULL (OnMouseMove then
// still calls Default(), which retail never reaches on that path).
void ForwardMouse(void* pThis, UINT msg, unsigned int nFlags, long long point) {
    ScreenWndLayout* self = L(pThis);
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<unsigned long long>(point) & 0xffffffffu);
    pt.y = static_cast<LONG>(static_cast<unsigned long long>(point) >> 32);
    HWND hTo = self->m_pColorDlg ? DlgHwnd(self->m_pColorDlg) : nullptr;
    ::MapWindowPoints(self->m_hWnd, hTo, &pt, 1);
    if (self->m_pColorDlg == nullptr) return;   // deviation, see above
    ::SendMessage(DlgHwnd(self->m_pColorDlg), msg, static_cast<WPARAM>(nFlags),
                  MAKELPARAM(pt.x, pt.y));
}

} // namespace

// Retail ??0CScreenWnd@@QEAA@XZ (entry RVA 0x2a080, mfc140u):
//     CWnd::CWnd();                                  // call 0x28a700 (mfc140u)
//     vfptr = &CScreenWnd::`vftable';                // 0x1802e18c8 (mfc140u)
//     return this;                                   // m_pColorDlg left untouched
// DEVIATION: OpenMFC has no CScreenWnd vftable, so the object keeps the CWnd
// vftable the base ctor installed.
// Symbol: ??0CScreenWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CScreenWnd__QEAA_XZ(void* pThis) {
    impl___0CWnd__QEAA_XZ(pThis);
    return pThis;
}

// Retail ??1CScreenWnd@@UEAA@XZ (entry RVA 0x2a100, mfc140u), complete:
//     vfptr = &CScreenWnd::`vftable';                // 0x1802e18c8 (mfc140u)
//     CWnd::~CWnd();                                 // tail jump to 0x28b740 (mfc140u)
// DEVIATION: the own-vftable store is omitted (no CScreenWnd vftable here).
// Symbol: ??1CScreenWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CScreenWnd__UEAA_XZ(void* pThis) {
    impl___1CWnd__UEAA_XZ(pThis);
}

// Retail ?Create@CScreenWnd@@UEAAHPEAVCMFCColorDialog@@@Z (entry RVA 0x2a120,
// mfc140u):
//     CWnd* pDesktop = CWnd::FromHandle(::GetDesktopWindow());
//     if (pDesktop == NULL) return FALSE;
//     m_pColorDlg = pColorDlg;
//     CRect rect(0, 0, 0, 0);
//     ::GetWindowRect(pDesktop->m_hWnd, &rect);
//     HINSTANCE h = AfxFindResourceHandle(MAKEINTRESOURCE(0x3f11), RT_GROUP_CURSOR);
//     CString strClass = AfxRegisterWndClass(CS_SAVEBITS,
//                          ::LoadCursor(h, MAKEINTRESOURCE(0x3f11)),
//                          (HBRUSH)(COLOR_BTNFACE + 1), NULL);
//     return CWnd::CreateEx(WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,   // 0xa0
//                           strClass, L"", WS_POPUP | WS_VISIBLE,    // 0x90000000
//                           rect, NULL, 0, NULL);                    // non-virtual call 0x28b440 (mfc140u)
// Retail also calls AfxGetModuleState() (0x133930, mfc140u) just before
// AfxFindResourceHandle and discards the result; that call is not reproduced.
// DEVIATION: retail copies the class name into a CString; here the pointer
// from AfxRegisterWndClass is passed straight to CreateEx.  OpenMFC's
// AfxRegisterWndClass (core/runtime/Globals.cpp) returns a slot of a
// thread-local ring of kWndClassNameSlots (16) buffers
// (detail/RegcoreSupport.cpp), so the name stays valid for this call.
// Symbol: ?Create@CScreenWnd@@UEAAHPEAVCMFCColorDialog@@@Z
extern "C" int MS_ABI impl__Create_CScreenWnd__UEAAHPEAVCMFCColorDialog___Z(void* pThis, void* pColorDlg) {
    ScreenWndLayout* self = L(pThis);
    CWnd* pDesktop = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDesktopWindow());
    if (pDesktop == nullptr) return FALSE;
    self->m_pColorDlg = pColorDlg;
    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(pDesktop->m_hWnd, &rect);
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
        MAKEINTRESOURCEW(kIdcAfxBarresColor), MAKEINTRESOURCEW(12) /* RT_GROUP_CURSOR */);
    HCURSOR hCursor = ::LoadCursorW(hInst, MAKEINTRESOURCEW(kIdcAfxBarresColor));
    const wchar_t* lpszClass = impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
        CS_SAVEBITS, hCursor, reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(COLOR_BTNFACE + 1)), nullptr);
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
        static_cast<CWnd*>(pThis), WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, lpszClass, L"",
        WS_POPUP | WS_VISIBLE, rect, nullptr, 0, nullptr);
}

// ?OnEraseBkgnd@CScreenWnd@@ is absent from both RVA symbol maps, but its
// export ordinal resolves through the mfc140u export table to 0x3a60
// (mfc140u) -- the same address the retail message map's WM_ERASEBKGND entry
// (AfxSig_bD) points at.  That identical-COMDAT-folded body is, in full,
//     mov $0x1,%eax ; ret
// i.e. return TRUE (background never erased).
// Symbol: ?OnEraseBkgnd@CScreenWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CScreenWnd__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// Retail OnLButtonDown: entry 0x2a2d0 (mfc140u; absent from the mfc140u RVA
// symbol map, but both its export ordinal and the message map's
// WM_LBUTTONDOWN entry resolve there) / 0x2a390 (mfc140.dll).  Forwards
// WM_LBUTTONDOWN (0x201) as described at ForwardMouse; no Default() call.
// Symbol: ?OnLButtonDown@CScreenWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CScreenWnd__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    ForwardMouse(pThis, WM_LBUTTONDOWN, nFlags, point);
}

// Retail OnMouseMove: entry 0x2a250 (mfc140u; export ordinal and message-map
// entry both resolve there) / 0x2a310 (mfc140.dll).  Forwards WM_MOUSEMOVE (0x200) as described at
// ForwardMouse, then calls CWnd::Default() (0x28ac80, mfc140u).
// Symbol: ?OnMouseMove@CScreenWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CScreenWnd__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    ForwardMouse(pThis, WM_MOUSEMOVE, nFlags, point);
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}
