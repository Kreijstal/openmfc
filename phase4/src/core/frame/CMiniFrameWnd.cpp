// CMiniFrameWnd — OpenMFC implementation.
// Sources: mfccore.cpp
//
// Bodies below are transcribed from the retail exports, disassembled in
// mfc140u.dll (RVAs resolved through the export ordinals where the symbol map
// has no entry).  The mfc140.dll ANSI twins have the same instruction stream,
// offsets and constants; only rel32 call/jump targets, RIP-relative IAT
// displacements and the security-cookie displacement differ, because the two
// images are laid out differently.  IAT slots were resolved in mfc140u.dll
// (iatu.py) and match the ANSI twin's imports apart from A/W suffixes; the code
// below calls the UNICODE-selected ::Xxx name.
//
// Retail layout (afxwin.h:4530, and the shipping code itself):
//   CFrameWnd base             0x000 .. 0x1d8
//   BOOL    m_bSysTracking     0x1d8
//   BOOL    m_bInSys           0x1dc
//   BOOL    m_bActive          0x1e0   (ctor at RVA 0x2a8cf0 (mfc140u) zeroes it)
//   CString m_strCaption       0x1e8   (ctor constructs it; CreateEx assigns it)
//   sizeof                     0x1f0   (CreateObject allocates 0x1f0)
// OpenMFC's public declaration (include/openmfc/afxmfc.h) models all four
// members as one `char _pad[32]`.  Under mingw that array starts at 0x1d4 (it
// is folded into CFrameWnd's tail padding) and ends at 0x1f4, so the retail
// offsets 0x1d8..0x1f0 all fall inside it (asserted in the constructor below).
// The public sizeof is 0x1f8, 8 bytes more than retail -- a header-level gap,
// not fixed here.

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <new>

// ---------------------------------------------------------------------------
// Sibling / base-class thunks (defined elsewhere; signatures from the mangled
// names, matching their definitions).
// ---------------------------------------------------------------------------
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                 // core/window/Thunks.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                              // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd* pThis);           // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                // core/window/CWnd.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" int MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);              // core/window/CWnd.cpp
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);              // core/window/CWnd.cpp
extern "C" int MS_ABI impl__HandleFloatingSysCommand_CWnd__QEAAHI_J_Z(
    CWnd* pThis, unsigned int nID, __int64 lParam);                                              // core/window/Thunks.cpp
extern "C" void MS_ABI impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(
    CFrameWnd* pThis, unsigned int nID, __int64 lParam);                                         // core/frame/Thunks.cpp
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs);                                                        // core/frame/CFrameWnd.cpp
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu,
    void* lpParam);                                                                              // core/window/CWnd.cpp
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon);                       // core/runtime/Globals.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                              // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                  // detail/MemcoreSupport.cpp

// Forward declaration of this file's own CreateEx (Create calls it directly).
extern "C" int MS_ABI impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
    CMiniFrameWnd* pThis, DWORD dwExStyle, const wchar_t* lpClassName, const wchar_t* lpWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

namespace {

// Retail member offsets (see file header).
constexpr std::size_t kOffMiniFirst      = 0x1d8;   // retail m_bSysTracking (first CMiniFrameWnd member)
constexpr std::size_t kOffMiniEnd        = 0x1f0;   // retail sizeof(CMiniFrameWnd)
constexpr std::size_t kOffMiniStrCaption = 0x1e8;   // CString m_strCaption
constexpr std::size_t kOffMiniActive     = 0x1e0;   // BOOL m_bActive

// Message / flag values used by the retail bodies (afxpriv.h / afxwin.h).
constexpr UINT  kWM_QUERYCENTERWND = 0x036B;
constexpr UINT  kWM_FLOATSTATUS    = 0x036D;
constexpr DWORD kMFS_SYNCACTIVE    = 0x00000100;
constexpr DWORD kMFS_4THICKFRAME   = 0x00000200;
constexpr DWORD kMFS_THICKFRAME    = 0x00000400;
constexpr DWORD kMFS_MOVEFRAME     = 0x00000800;
constexpr DWORD kMFS_BLOCKSYSMENU  = 0x00001000;
constexpr WPARAM kFS_SHOW          = 0x01;
constexpr WPARAM kFS_HIDE          = 0x02;
constexpr WPARAM kFS_ACTIVATE      = 0x04;
constexpr WPARAM kFS_DEACTIVATE    = 0x08;
constexpr WPARAM kFS_ENABLE        = 0x10;
constexpr WPARAM kFS_DISABLE       = 0x20;
constexpr WPARAM kFS_SYNCACTIVE    = 0x40;
constexpr DWORD kWF_KEEPMINIACTIVE = 0x0200;

inline CString& MiniCaption(CMiniFrameWnd* pThis) {
    CString* p = reinterpret_cast<CString*>(reinterpret_cast<char*>(pThis) + kOffMiniStrCaption);
    // Deviation (defensive): an object whose CMiniFrameWnd constructor never ran
    // (the CMiniDockFrameWnd and CMFCDropDownFrame constructor thunks in this
    // tree are still stubs that return pThis unconstructed) has no valid
    // CString here.  When that storage happens to be zeroed the pointer is
    // NULL, which is detected and replaced by the empty string; uninitialised
    // (non-zero) storage cannot be detected and is not handled.
    if (*reinterpret_cast<void**>(p) == nullptr) {
        new (p) CString();
    }
    return *p;
}

} // namespace

// Retail ctor (RVA 0x2a8cf0 (mfc140u)): CFrameWnd::CFrameWnd, then constructs
// m_strCaption (+0x1e8) as the empty string and sets m_bActive (+0x1e0) = FALSE.
// It leaves m_bSysTracking/m_bInSys uninitialised; OpenMFC zeroes the whole
// block first (harmless deviation).
CMiniFrameWnd::CMiniFrameWnd() {
    // mingw (Itanium ABI) folds _pad into CFrameWnd's 4 bytes of tail padding,
    // so _pad begins at 0x1d4, not 0x1d8; what matters is that the retail
    // member block [0x1d8, 0x1f0) lies wholly inside it.
    static_assert(offsetof(CMiniFrameWnd, _pad) <= kOffMiniFirst,
                  "retail m_bSysTracking (+0x1d8) must lie inside CMiniFrameWnd::_pad");
    static_assert(offsetof(CMiniFrameWnd, _pad) + sizeof(_pad) >= kOffMiniEnd,
                  "retail m_strCaption (+0x1e8..+0x1f0) must lie inside CMiniFrameWnd::_pad");
    memset(_pad, 0, sizeof(_pad));
    new (reinterpret_cast<char*>(this) + kOffMiniStrCaption) CString();
    *reinterpret_cast<BOOL*>(reinterpret_cast<char*>(this) + kOffMiniActive) = FALSE;
}
// Retail dtor (RVA 0x2a8d90 (mfc140u)): DestroyWindow(), release m_strCaption,
// then ~CFrameWnd.  Only the m_strCaption release is reproduced here; window
// teardown is left to OpenMFC's base-class destructors as before.
CMiniFrameWnd::~CMiniFrameWnd() {
    CString* p = reinterpret_cast<CString*>(reinterpret_cast<char*>(this) + kOffMiniStrCaption);
    if (*reinterpret_cast<void**>(p) != nullptr) {
        p->~CString();
        *reinterpret_cast<void**>(p) = nullptr;
    }
}

// Symbol: ?CalcBorders@CMiniFrameWnd@@SAXPEAUtagRECT@@KK@Z
// Transcribed from RVA 0x2a9220 (mfc140u): a tail jump to
// ::AdjustWindowRectEx(lpClientRect, dwStyle, FALSE, 0x188 = WS_EX_PALETTEWINDOW).
// Retail ignores dwExStyle entirely (r9d is overwritten with the constant).
extern "C" void MS_ABI impl__CalcBorders_CMiniFrameWnd__SAXPEAUtagRECT__KK_Z(
    RECT* lpClientRect, unsigned long dwStyle, unsigned long dwExStyle) {
    (void)dwExStyle;
    ::AdjustWindowRectEx(lpClientRect, dwStyle, FALSE, WS_EX_PALETTEWINDOW);
}

// Symbol: ?Create@CMiniFrameWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@I@Z
// Transcribed from RVA 0x2a8de0 (mfc140u): a direct (non-virtual) call to
// CMiniFrameWnd::CreateEx (0x2a8e20) with dwExStyle = 0 and the remaining
// arguments passed through unchanged.
extern "C" int MS_ABI impl__Create_CMiniFrameWnd__UEAAHPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
    CMiniFrameWnd* pThis, const wchar_t* lpClassName, const wchar_t* lpWindowName,
    unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID) {
    return impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
        pThis, 0, lpClassName, lpWindowName, dwStyle, rect, pParentWnd, nID);
}

// Symbol: ?CreateEx@CMiniFrameWnd@@UEAAHKPEB_W0KAEBUtagRECT@@PEAVCWnd@@I@Z
// Transcribed from RVA 0x2a8e20 (mfc140u):
//   m_strCaption (+0x1e8).SetString(lpWindowName, lpWindowName ? wcslen(lpWindowName) : 0);
//   hWndParent = pParentWnd ? pParentWnd->m_hWnd (+0x40) : NULL;
//   if (lpClassName == NULL)
//       lpClassName = AfxRegisterWndClass(CS_DBLCLKS /*8*/, ::LoadCursor(NULL, IDC_ARROW /*0x7f00*/), NULL, NULL);
//   return CWnd::CreateEx(dwExStyle, lpClassName, lpWindowName, dwStyle,
//                         rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
//                         hWndParent, (HMENU)(UINT_PTR)nID, NULL);    // direct call, 0x28b500
// (The rect fields are read before the class-name fallback; neither has side
// effects visible to the other, so the order below is equivalent.)
extern "C" int MS_ABI impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
    CMiniFrameWnd* pThis, DWORD dwExStyle, const wchar_t* lpClassName, const wchar_t* lpWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
    MiniCaption(pThis) = lpWindowName;   // OpenMFC CString: NULL/"" -> empty, as SetString(NULL, 0)
    HWND hWndParent = (pParentWnd != nullptr) ? pParentWnd->m_hWnd : nullptr;
    if (lpClassName == nullptr) {
        lpClassName = impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
            CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr);
    }
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
        pThis, dwExStyle, lpClassName, lpWindowName, dwStyle,
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        hWndParent, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr);
}

// Symbol: ?CreateObject@CMiniFrameWnd@@SAPEAVCObject@@XZ
// Transcribed from RVA 0x2a9270 (mfc140u): operator new(0x1f0), and when that
// returns non-NULL, CMiniFrameWnd::CMiniFrameWnd() on it; the result (possibly
// NULL) is returned.  The allocation goes through the DLL's exported
// ::operator new thunk (??2@YAPEAX_K@Z), as the other hand-transcribed
// allocations in this tree do.  Deviation: OpenMFC requests its own
// sizeof(CMiniFrameWnd) (0x1f8, see file header) rather than retail's 0x1f0,
// because the constructor below writes all of _pad (up to +0x1f4).
extern "C" CObject* MS_ABI impl__CreateObject_CMiniFrameWnd__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(CMiniFrameWnd));
    if (p == nullptr) {
        return nullptr;
    }
    return new (p) CMiniFrameWnd();
}

// Symbol: ?OnFloatStatus@CMiniFrameWnd@@QEAA_J_K_J@Z
// Transcribed from RVA 0xb1790 (mfc140u).  That body is identical-code-folded:
// the export ordinals of CMiniFrameWnd::OnFloatStatus (9952) and
// CPaneFrameWnd::OnFloatStatus (9953) both resolve to it, and the WM_FLOATSTATUS
// (0x36d, sig 14) entry of CMiniFrameWnd's retail message map (AFX_MSGMAP at
// 0x18033c6d0 (mfc140u), returned by GetMessageMap at RVA 0x2a8ce0 (mfc140u))
// points at 0xb1790.
//   LRESULT lResult = 0;
//   if (GetStyle() & MFS_SYNCACTIVE) lResult = (wParam & FS_SYNCACTIVE) != 0;
//   if (wParam & (FS_SHOW|FS_HIDE))
//       SetWindowPos(NULL, 0, 0, 0, 0, ((wParam & FS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW) |
//                    SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);   // 0x57 / 0x97
//   if (wParam & (FS_ENABLE|FS_DISABLE)) EnableWindow((wParam & FS_ENABLE) != 0);
//   if ((wParam & (FS_ACTIVATE|FS_DEACTIVATE)) && (GetStyle() & MFS_SYNCACTIVE)) {  // GetStyle re-read
//       ModifyStyle(MFS_SYNCACTIVE, 0);
//       ::SendMessage(m_hWnd, WM_NCACTIVATE, (wParam & FS_ACTIVATE) != 0, 0);
//       ModifyStyle(0, MFS_SYNCACTIVE);
//   }
//   return lResult;
extern "C" __int64 MS_ABI impl__OnFloatStatus_CMiniFrameWnd__QEAA_J_K_J_Z(
    CMiniFrameWnd* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;
    __int64 lResult = 0;
    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & kMFS_SYNCACTIVE) {
        lResult = (wParam & kFS_SYNCACTIVE) ? 1 : 0;
    }
    if (wParam & (kFS_SHOW | kFS_HIDE)) {
        const unsigned int nFlags = ((wParam & kFS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW) |
                                    SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE;
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pThis, nullptr, 0, 0, 0, 0, nFlags);
    }
    if (wParam & (kFS_ENABLE | kFS_DISABLE)) {
        impl__EnableWindow_CWnd__QEAAHH_Z(pThis, (wParam & kFS_ENABLE) ? TRUE : FALSE);
    }
    if ((wParam & (kFS_ACTIVATE | kFS_DEACTIVATE)) &&
        (impl__GetStyle_CWnd__QEBAKXZ(pThis) & kMFS_SYNCACTIVE)) {
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(pThis, kMFS_SYNCACTIVE, 0, 0);
        ::SendMessage(pThis->m_hWnd, WM_NCACTIVATE, (wParam & kFS_ACTIVATE) ? TRUE : FALSE, 0);
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(pThis, 0, kMFS_SYNCACTIVE, 0);
    }
    return lResult;
}

// Symbol: ?OnGetMinMaxInfo@CMiniFrameWnd@@QEAAXPEAUtagMINMAXINFO@@@Z
// Transcribed from RVA 0x2a8f60 (mfc140u):
//   Default();                                   // result discarded
//   CRect rectWindow, rectClient;                // both zero-initialised
//   ::GetWindowRect(m_hWnd, &rectWindow); ::GetClientRect(m_hWnd, &rectClient);
//   pMMI->ptMinTrackSize.x = rectWindow.Width()  - rectClient.right;
//   pMMI->ptMinTrackSize.y = rectWindow.Height() - rectClient.bottom;
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMiniFrameWnd__QEAAXPEAUtagMINMAXINFO___Z(
    CMiniFrameWnd* pThis, MINMAXINFO* pMMI) {
    impl__Default_CWnd__IEAA_JXZ(pThis);
    RECT rectWindow = {0, 0, 0, 0};
    RECT rectClient = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWindow);
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    pMMI->ptMinTrackSize.x = (rectWindow.right - rectWindow.left) - rectClient.right;
    pMMI->ptMinTrackSize.y = (rectWindow.bottom - rectWindow.top) - rectClient.bottom;
}

// Symbol: ?OnNcActivate@CMiniFrameWnd@@QEAAHH@Z
// Transcribed from RVA 0x2a8ff0 (mfc140u):
//   if (!(GetStyle() & MFS_SYNCACTIVE)) return Default() != 0;
//   return !(m_nFlags & WF_KEEPMINIACTIVE /*0x200*/);        // bActive is not read
// Deviation: retail reads m_nFlags at its retail CWnd offset +0xa8 (the same
// offset CWnd::ContinueModal, RVA 0x291820 (mfc140u), reads); OpenMFC's CWnd
// declares m_nFlags at +0x50 and its own flag bookkeeping (e.g. the modal-loop
// bit in core/window/CWnd.cpp) uses that field, so the named member is read
// here.  Nothing in this tree sets WF_KEEPMINIACTIVE yet, so with
// MFS_SYNCACTIVE this currently always returns TRUE.
extern "C" int MS_ABI impl__OnNcActivate_CMiniFrameWnd__QEAAHH_Z(CMiniFrameWnd* pThis, int bActive) {
    (void)bActive;
    if (!(impl__GetStyle_CWnd__QEBAKXZ(pThis) & kMFS_SYNCACTIVE)) {
        return impl__Default_CWnd__IEAA_JXZ(pThis) != 0 ? TRUE : FALSE;
    }
    return (pThis->m_nFlags & kWF_KEEPMINIACTIVE) ? FALSE : TRUE;
}

// Symbol: ?OnNcCreate@CMiniFrameWnd@@QEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from RVA 0xb16d0 (mfc140u).  Identical-code-folded: the ordinals
// of CMiniFrameWnd::OnNcCreate (10582) and CPaneFrameWnd::OnNcCreate (10584)
// both resolve to it, and the WM_NCCREATE (0x81, sig 13) entry of
// CMiniFrameWnd's retail message map (0x18033c6d0 (mfc140u)) points at 0xb16d0.
//   if ((int)Default() == 0) return FALSE;       // `test eax,eax` -- low 32 bits
//   if (GetStyle() & MFS_SYNCACTIVE) {
//       CWnd* pParentFrame = GetTopLevelParent();
//       ENSURE_VALID(pParentFrame);              // NULL -> AfxThrowInvalidArgException
//       CWnd* pActiveWnd = CWnd::FromHandle(::GetForegroundWindow());
//       BOOL bActive = pParentFrame == pActiveWnd ||
//           (CWnd::FromHandle(::GetLastActivePopup(pParentFrame->m_hWnd)) == pActiveWnd &&
//            ::SendMessage(pActiveWnd->m_hWnd, WM_FLOATSTATUS, FS_SYNCACTIVE, 0) != 0);
//       ::SendMessage(m_hWnd, WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE, 0);
//   }
//   return TRUE;
extern "C" int MS_ABI impl__OnNcCreate_CMiniFrameWnd__QEAAHPEAUtagCREATESTRUCTW___Z(
    CMiniFrameWnd* pThis, CREATESTRUCTW* lpcs) {
    (void)lpcs;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis)) == 0) {
        return FALSE;
    }
    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & kMFS_SYNCACTIVE) {
        CWnd* pParentFrame = static_cast<CWnd*>(impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(pThis));
        if (pParentFrame == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;   // not reached: the call above throws
        }
        CWnd* pActiveWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetForegroundWindow());
        WPARAM nState = kFS_DEACTIVATE;
        if (pParentFrame == pActiveWnd) {
            nState = kFS_ACTIVATE;
        } else if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
                       ::GetLastActivePopup(pParentFrame->m_hWnd)) == pActiveWnd &&
                   ::SendMessage(pActiveWnd->m_hWnd, kWM_FLOATSTATUS, kFS_SYNCACTIVE, 0) != 0) {
            nState = kFS_ACTIVATE;
        }
        ::SendMessage(pThis->m_hWnd, kWM_FLOATSTATUS, nState, 0);
    }
    return TRUE;
}

// Symbol: ?OnNcHitTest@CMiniFrameWnd@@QEAA_JVCPoint@@@Z
// Transcribed from RVA 0x2a9030 (mfc140u).  CPoint arrives by value in RDX
// (x in the low dword, y in the high dword), modelled as `long long`, matching
// the forward declaration in featurepack/menu/CMFCPopupMenu.cpp.
//   DWORD dwStyle = GetStyle();
//   CRect rectWindow; ::GetWindowRect(m_hWnd, &rectWindow);
//   int cyFrame = ::GetSystemMetrics(SM_CYFRAME), cxFrame = ::GetSystemMetrics(SM_CXFRAME);
//   LRESULT nHit = Default();
//   if (dwStyle & MFS_BLOCKSYSMENU) {
//       if (nHit == HTSYSMENU) nHit = HTCAPTION;
//       if (::GetKeyState(VK_RBUTTON) < 0) return HTNOWHERE;
//   }
//   if ((nHit < HTSIZEFIRST || nHit > HTSIZELAST) && nHit != HTGROWBOX) return nHit;
//   if (dwStyle & MFS_MOVEFRAME) return HTCAPTION;
//   ::InflateRect(&rectWindow, -cxFrame, -cyFrame);
//   if (dwStyle & MFS_4THICKFRAME) switch (nHit) {
//       case HTTOPLEFT:     return point.y < rectWindow.top    ? HTTOP    : HTLEFT;
//       case HTTOPRIGHT:    return point.y < rectWindow.top    ? HTTOP    : HTRIGHT;
//       case HTBOTTOMLEFT:  return point.y > rectWindow.bottom ? HTBOTTOM : HTLEFT;
//       case HTGROWBOX: case HTBOTTOMRIGHT:
//                           return point.y > rectWindow.bottom ? HTBOTTOM : HTRIGHT;
//   }
//   return nHit;
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMiniFrameWnd__QEAA_JVCPoint___Z(
    CMiniFrameWnd* pThis, long long point) {
    const int ptY = static_cast<int>(static_cast<unsigned long long>(point) >> 32);
    const DWORD dwStyle = impl__GetStyle_CWnd__QEBAKXZ(pThis);
    RECT rectWindow = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWindow);
    const int cyFrame = ::GetSystemMetrics(SM_CYFRAME);
    const int cxFrame = ::GetSystemMetrics(SM_CXFRAME);
    __int64 nHit = impl__Default_CWnd__IEAA_JXZ(pThis);

    if (dwStyle & kMFS_BLOCKSYSMENU) {
        if (nHit == HTSYSMENU) {
            nHit = HTCAPTION;
        }
        if (::GetKeyState(VK_RBUTTON) < 0) {
            return HTNOWHERE;
        }
    }
    if ((nHit < HTSIZEFIRST || nHit > HTSIZELAST) && nHit != HTGROWBOX) {
        return nHit;
    }
    if (dwStyle & kMFS_MOVEFRAME) {
        return HTCAPTION;
    }
    ::InflateRect(&rectWindow, -cxFrame, -cyFrame);
    if (dwStyle & kMFS_4THICKFRAME) {
        switch (nHit) {
        case HTTOPLEFT:
            return (ptY < rectWindow.top) ? HTTOP : HTLEFT;
        case HTTOPRIGHT:
            return (ptY < rectWindow.top) ? HTTOP : HTRIGHT;
        case HTBOTTOMLEFT:
            return (ptY > rectWindow.bottom) ? HTBOTTOM : HTLEFT;
        case HTGROWBOX:
        case HTBOTTOMRIGHT:
            return (ptY > rectWindow.bottom) ? HTBOTTOM : HTRIGHT;
        default:
            break;
        }
    }
    return nHit;
}

// Symbol: ?OnQueryCenterWnd@CMiniFrameWnd@@QEAA_J_K_J@Z
// Transcribed from RVA 0x2a9230 (mfc140u):
//   HWND hWndParent = ::GetParent(m_hWnd);
//   LRESULT r = ::SendMessage(hWndParent, WM_QUERYCENTERWND, 0, 0);
//   return r != 0 ? r : (LRESULT)hWndParent;
extern "C" __int64 MS_ABI impl__OnQueryCenterWnd_CMiniFrameWnd__QEAA_J_K_J_Z(
    CMiniFrameWnd* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    HWND hWndParent = ::GetParent(pThis->m_hWnd);
    const LRESULT r = ::SendMessage(hWndParent, kWM_QUERYCENTERWND, 0, 0);
    return (r != 0) ? r : reinterpret_cast<__int64>(hWndParent);
}

// Symbol: ?OnSysCommand@CMiniFrameWnd@@QEAAXI_J@Z
// Transcribed from RVA 0x2a91a0 (mfc140u):
//   DWORD dwStyle = GetStyle();
//   if ((dwStyle & WS_POPUP) &&
//       ((nID & 0xFFF0) != SC_CLOSE ||
//        (::GetKeyState(VK_F4) < 0 && ::GetKeyState(VK_MENU) < 0 && (dwStyle & MFS_SYNCACTIVE)))) {
//       if (HandleFloatingSysCommand(nID, lParam)) return;
//   }
//   CFrameWnd::OnSysCommand(nID, lParam);
extern "C" void MS_ABI impl__OnSysCommand_CMiniFrameWnd__QEAAXI_J_Z(
    CMiniFrameWnd* pThis, unsigned int nID, __int64 lParam) {
    const DWORD dwStyle = impl__GetStyle_CWnd__QEBAKXZ(pThis);
    if ((dwStyle & WS_POPUP) &&
        ((nID & 0xFFF0) != SC_CLOSE ||
         (::GetKeyState(VK_F4) < 0 && ::GetKeyState(VK_MENU) < 0 && (dwStyle & kMFS_SYNCACTIVE)))) {
        if (impl__HandleFloatingSysCommand_CWnd__QEAAHI_J_Z(pThis, nID, lParam)) {
            return;
        }
    }
    impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(pThis, nID, lParam);
}

// Symbol: ?PreCreateWindow@CMiniFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z
// Transcribed from RVA 0x2a8f20 (mfc140u):
//   if (cs.style & (MFS_4THICKFRAME|MFS_THICKFRAME)) cs.style |= WS_THICKFRAME;
//   if (cs.style & WS_CAPTION) cs.dwExStyle |= WS_EX_TOOLWINDOW;
//   CFrameWnd::PreCreateWindow(cs);              // direct call; result discarded
//   cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
//   return TRUE;
extern "C" int MS_ABI impl__PreCreateWindow_CMiniFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMiniFrameWnd* pThis, CREATESTRUCTW& cs) {
    if (cs.style & (kMFS_4THICKFRAME | kMFS_THICKFRAME)) {
        cs.style |= WS_THICKFRAME;
    }
    if (cs.style & WS_CAPTION) {
        cs.dwExStyle |= WS_EX_TOOLWINDOW;
    }
    impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    return TRUE;
}
