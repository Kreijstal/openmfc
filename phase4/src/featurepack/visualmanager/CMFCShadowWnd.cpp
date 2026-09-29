// CMFCShadowWnd — OpenMFC implementation.
//
// CMFCShadowWnd is the private layered window that draws the drop shadow of a
// CMFCPopupMenu.  It is not declared in include/openmfc, so there is no C++ type
// for it in this DLL; the bodies below take `void* pThis` and address the RETAIL
// layout, reaching base-class and sibling behaviour through exported impl__
// thunks (same convention as featurepack/controls/CMFCDropDownFrame.cpp).
//
// Retail layout.  The class has no exported constructor: it is inlined into
// CMFCPopupMenu::OnCreate (entry 0xb5790 (mfc140u)), whose shadow-creation
// block starts at the `mov $0x408,%ecx` at 0xb58d9 (mfc140u) and reads:
//   p = operator new(0x408);
//   CMiniFrameWnd::CMiniFrameWnd(p);                     // 0x2a8cf0 (mfc140u)
//   p->vfptr = CMFCShadowWnd vftable (0x1802f8a68 (mfc140u));
//   CMFCShadowRenderer::CMFCShadowRenderer(p + 0x200);   // 0x32dc0 (mfc140u)
//   p->+0x1f0 = popup (this);  p->+0x1f8 = popup->m_iShadowSize (+0x1810);  p->+0x400 = 0;
//   popup->m_pWndShadow (+0x1838) = p; popup->m_iShadowSize = 0;
//   p->Create();                                         // vslot 0x3a0/8 = 116
// and the non-exported function at 0xb4750 (mfc140u) -- a scalar deleting
// destructor by shape: it re-installs the same two vftables, runs
// ??1CMFCControlRenderer on +0x200 (0x32150 (mfc140u)) and ??1CMiniFrameWnd
// (0x2a8d90 (mfc140u)), then on flag bit 0 frees the object -- through the sized
// operator delete with 0x408 when flag bit 2 is also set, otherwise through the
// CRT `free` import.  It is vtable slot 1 of the CMFCShadowWnd vftable.  So:
//   CMiniFrameWnd base                 0x000 .. 0x1f0  (retail sizeof; see core/frame/CMiniFrameWnd.cpp)
//   CWnd*              m_pOwner        0x1f0   (the popup menu)
//   int                m_nDepth        0x1f8   (shadow depth)
//   CMFCShadowRenderer m_Shadow        0x200   (embedded, 0x200 bytes)
//   BOOL               m_bIsRTL        0x400
//   sizeof                             0x408
// (Member names are this file's; retail only fixes offsets and types.)
// Caveat for any future OpenMFC-side construction: OpenMFC's own CMiniFrameWnd
// is 0x1f8 bytes and its constructor writes its `_pad` up to +0x1f4, i.e. over
// the low half of retail's m_pOwner slot.  Retail stores m_pOwner only after
// the base constructor has run, so that order must be kept.
//
// Retail message map: AFX_MSGMAP at 0x1802f6938 (mfc140.dll) -- base
// CMiniFrameWnd::GetThisMessageMap -- has exactly two entries:
// WM_ERASEBKGND (sig 1) -> 0x3ae0 (mfc140.dll) and WM_SIZE (sig 26) -> OnSize
// (0xb4ee0 (mfc140.dll)).  The same two entries in mfc140u.dll (entry array
// holding the WM_SIZE pfn at RVA 0x2f8a38 (mfc140u)) point at 0x3a60 and
// 0xb4920 (mfc140u).
//
// m_Shadow is a by-value member, so its dynamic type is exactly
// CMFCShadowRenderer; retail's virtual calls on it (slot 13 Create, slot 6 Draw)
// are bound statically here to that class's exported thunks
// (featurepack/visualmanager/CMFCShadowRenderer.cpp).  That is not a behavioural
// deviation, but note both of those thunks are themselves still documented
// stubs (Create returns 0, Draw paints nothing).
//
// Retail addresses below are mfc140u RVAs unless marked (mfc140.dll); the
// instruction streams were read from the mfc140.dll twin, whose bodies are
// byte-identical.  IAT slots were resolved with iat.py (ANSI names; the code
// calls the UNICODE-selected ::Xxx).

// This TU deliberately includes only <windows.h>: every MFC object is handled
// as `void*` at retail offsets, and the thunk declarations below use the
// equivalent plain types (the extern "C" names, not the C++ types, are what
// link).  Including the OpenMFC class headers would pull C++ symbol references
// into a file that needs none.

#include <windows.h>

#include <cstddef>
#include <cstring>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Cross-file thunks, each grepped to its // Symbol: definition (file named);
// parameter lists follow the mangled names.  CWnd* / CFrameWnd* /
// CMiniFrameWnd* / CDC* are passed as void*; `const CSize&` as `const SIZE*`
// (CSize is a tagSIZE); `const RECT&` stays a reference.
// ---------------------------------------------------------------------------
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);            // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                   // featurepack/CMFC_misc_stubs.cpp
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon);                // core/runtime/Globals.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const void* pThis);        // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);         // core/window/CWnd.cpp
extern "C" int MS_ABI impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, DWORD dwExStyle, const wchar_t* lpClassName, const wchar_t* lpWindowName,
    DWORD dwStyle, const RECT& rect, void* pParentWnd, UINT nID);                         // core/frame/CMiniFrameWnd.cpp
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(void* pThis, int nCmdShow);         // core/window/CWnd.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    void* pThis, const void* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" const unsigned char impl__wndTop_CWnd__2V1_B[];                                 // core/window/CWnd.cpp
extern "C" void MS_ABI impl__OnSize_CFrameWnd__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy); // core/frame/Thunks.cpp
extern "C" HBITMAP MS_ABI impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(
    const SIZE* pSize, void** pBits);                                                     // core/gdi/CDrawingManager.cpp
extern "C" void* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);             // core/gdi/CDC.cpp
// CMFCShadowRenderer (featurepack/visualmanager/CMFCShadowRenderer.cpp); the
// parameter types match that file's definitions.
extern "C" int MS_ABI impl__Create_CMFCShadowRenderer__UEAAHHKHH_Z(
    void* pThis, int nDepth, unsigned long clrBase, int iMinBrightness, int iMaxBrightness);
extern "C" void MS_ABI impl__Draw_CMFCShadowRenderer__UEAAXPEAVCDC__VCRect__IE_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int index, unsigned char alphaSrc);


namespace {

constexpr int kOffHWnd    = 0x40;    // CWnd::m_hWnd (retail; the CWnd thunks read it there too)
constexpr int kOffOwner   = 0x1f0;   // CWnd* m_pOwner
constexpr int kOffDepth   = 0x1f8;   // int   m_nDepth
constexpr int kOffShadow  = 0x200;   // CMFCShadowRenderer m_Shadow (embedded)
constexpr int kOffIsRTL   = 0x400;   // BOOL  m_bIsRTL
constexpr int kSizeofShadowWnd      = 0x408;   // operator new size / scalar-deleting-dtor free size
constexpr int kSizeofShadowRenderer = 0x200;   // CMFCShadowRenderer::CreateObject's operator new size
static_assert(kOffShadow + kSizeofShadowRenderer == kOffIsRTL, "m_Shadow spans 0x200..0x400");
static_assert(kOffIsRTL + 8 == kSizeofShadowWnd, "m_bIsRTL (+tail padding) is the last member");
static_assert(kOffOwner + 8 == kOffDepth, "layout order");

// afxGlobalData: +0 is the one-time init gate (inline GetGlobalData());
// m_nBitsPerPixel is pinned at 0x288 by core/runtime/AFX_GLOBAL_DATA.cpp
// (static_assert(offsetof(AfxGlobalData, m_nBitsPerPixel) == 0x288)).
constexpr int kGdInitGate     = 0x000;
constexpr int kGdBitsPerPixel = 0x288;

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}

inline unsigned char* GlobalData() {
    unsigned char* g = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    int init;
    std::memcpy(&init, g + kGdInitGate, sizeof init);
    if (init == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(g);
        const int one = 1;
        std::memcpy(g + kGdInitGate, &one, sizeof one);
    }
    return g;
}

inline const void* WndTop() { return impl__wndTop_CWnd__2V1_B; }

} // namespace

// Create.  Retail RVA 0xb47d0 (mfc140u), transcribed in full:
//   BOOL bRes = TRUE;
//   if (GetGlobalData()->m_nBitsPerPixel <= 8) return FALSE;      // afxGlobalData +0x288
//   CString strClassName = ::AfxRegisterWndClass(CS_SAVEBITS /*0x800*/,
//           ::LoadCursor(NULL, IDC_ARROW /*0x7f00*/), (HBRUSH)(COLOR_BTNFACE + 1) /*0x10*/, NULL);
//   CRect rectDummy(0, 0, 0, 0);
//   if (m_pOwner->GetExStyle() & WS_EX_LAYOUTRTL)                 // bt $0x16 on CWnd::GetExStyle
//       m_bIsRTL = TRUE;                                          // +0x400
//   if (!CMiniFrameWnd::CreateEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW /*0x80080*/, strClassName,
//           _T(""), WS_POPUP, rectDummy,
//           CWnd::FromHandle(::GetParent(m_pOwner->m_hWnd)), 0))  // direct call, not virtual
//       bRes = FALSE;
//   else
//       m_Shadow.Create(m_nDepth, RGB(0x5a,0x5a,0x5a), 0, 50);  // vslot 13 (+0x68); result ignored
//   return bRes;
// IAT: LoadCursor, GetParent.  The class name is copied into a local buffer
// where retail copies it into a CString (OpenMFC's AfxRegisterWndClass returns
// a thread-local ring buffer).
// DEVIATION: a NULL pThis or NULL m_pOwner returns FALSE; retail would fault.
// Symbol: ?Create@CMFCShadowWnd@@UEAAHXZ
extern "C" int MS_ABI impl__Create_CMFCShadowWnd__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;

    int bitsPerPixel;
    std::memcpy(&bitsPerPixel, GlobalData() + kGdBitsPerPixel, sizeof bitsPerPixel);
    if (bitsPerPixel <= 8) return FALSE;

    void* pOwner = At<void*>(pThis, kOffOwner);
    if (pOwner == nullptr) return FALSE;

    wchar_t szClassName[256];
    const wchar_t* pszRegistered = impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
        CS_SAVEBITS, ::LoadCursor(nullptr, IDC_ARROW),
        reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(COLOR_BTNFACE + 1)), nullptr);
    szClassName[0] = L'\0';
    if (pszRegistered != nullptr) {
        std::wcsncpy(szClassName, pszRegistered, 255);
        szClassName[255] = L'\0';
    }

    RECT rectDummy = {0, 0, 0, 0};

    if (impl__GetExStyle_CWnd__QEBAKXZ(pOwner) & WS_EX_LAYOUTRTL) {
        At<BOOL>(pThis, kOffIsRTL) = TRUE;
    }

    void* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
        ::GetParent(At<HWND>(pOwner, kOffHWnd)));
    if (!impl__CreateEx_CMiniFrameWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__I_Z(
            pThis, WS_EX_LAYERED | WS_EX_TOOLWINDOW, szClassName, L"", WS_POPUP, rectDummy,
            pParent, 0)) {
        return FALSE;
    }

    impl__Create_CMFCShadowRenderer__UEAAHHKHH_Z(
        static_cast<char*>(pThis) + kOffShadow, At<int>(pThis, kOffDepth), RGB(0x5a, 0x5a, 0x5a), 0, 50);
    return TRUE;
}

// OnEraseBkgnd.  In neither symbol map: the WM_ERASEBKGND entry of the retail
// message map (see file header) points at 0x3ae0 (mfc140.dll) / 0x3a60
// (mfc140u), which is `mov $0x1,%eax; ret` -- a COMDAT-folded body (the
// mfc140.dll map names it ?OnEraseBkgnd@CPaneTrackingWnd@@IEAAHPEAVCDC@@@Z).
// Retail: return TRUE.
// Symbol: ?OnEraseBkgnd@CMFCShadowWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCShadowWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// OnSize.  Retail RVA 0xb4920 (mfc140u) = 0xb4ee0 (mfc140.dll).  The mfc140u
// symbol map has no entry for this export; the address is the WM_SIZE entry of
// the mfc140u message map (see file header), and the mfc140.dll map does name
// 0xb4ee0 as this symbol.  Transcribed:
//   CMiniFrameWnd::OnSize(nType, cx, cy);        // direct call; resolves to CFrameWnd::OnSize
//   if (cx == 0 || cy == 0) return;
//   CSize size(cx, cy);  LPVOID pBits = NULL;
//   HBITMAP hbmp = CDrawingManager::CreateBitmap_32(size, &pBits);
//   if (hbmp == NULL) return;
//   CBitmap bmp;  bmp.Attach(hbmp);
//   CClientDC clientDC(this);                    // ::GetDC(m_hWnd); throws if NULL
//   CDC dc;  dc.Attach(::CreateCompatibleDC(clientDC.m_hDC));
//   CBitmap* pBmpOld = (CBitmap*)CGdiObject::FromHandle(::SelectObject(dc.m_hDC, bmp.m_hObject));
//   m_Shadow.Draw(&dc, CRect(CPoint(0, 0), size), 0, 255);  // vslot 6 (+0x30) on this+0x200
//   CPoint point(0, 0);
//   BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };  // stored as 0x01ff0000
//   ::UpdateLayeredWindow(m_hWnd, NULL, NULL, &size, dc.m_hDC, &point, 0, &bf, ULW_ALPHA /*2*/);
//   CGdiObject::FromHandle(::SelectObject(dc.m_hDC, pBmpOld ? pBmpOld->m_hObject : NULL));
//   // then destructors: ~CDC (if m_hDC: ::DeleteDC(Detach())), ~CClientDC (ReleaseDC),
//   // ~CBitmap (CGdiObject::DeleteObject)
// IAT: CreateCompatibleDC, SelectObject (x2), UpdateLayeredWindow, DeleteDC.
// DEVIATIONS -- (1)-(3) concern how the wrappers are realised, not the GDI
// calls made; (4)-(5) are error paths:
// (1) the CBitmap, CClientDC and stack CDC wrappers are replaced by the raw
// handles they carry (::GetDC / ::ReleaseDC on m_hWnd for CClientDC, as its
// retail ctor 0x2a1a60 and dtor 0x2a1b20 (mfc140.dll) do; ::DeleteDC and ::DeleteObject
// for the ~CDC / ~CBitmap steps).  (2) the CDC* handed to m_Shadow.Draw is
// CDC::FromHandle(hdcMem) -- a temporary-map CDC wrapping the same memory DC --
// rather than a stack CDC; this TU has no CDC type, and the temp object is a
// fully constructed OpenMFC CDC.  Its temp-map entry outlives the DC until the
// next temp-map cleanup.  If ::CreateCompatibleDC fails, CDC::FromHandle(NULL)
// returns NULL, so Draw receives a NULL CDC* where retail passes its stack CDC
// with a NULL m_hDC (the remaining GDI calls then get a NULL HDC in both).  (3) the two CGdiObject::FromHandle round-trips are
// dropped: FromHandle(h)->m_hObject is h and a NULL handle maps to NULL, so the
// restoring ::SelectObject receives the same handle.  (4) where retail's
// CClientDC ctor throws on a NULL ::GetDC, this releases the bitmap and
// returns.  (5) a NULL pThis returns immediately.
// Symbol: ?OnSize@CMFCShadowWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCShadowWnd__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    if (pThis == nullptr) return;
    impl__OnSize_CFrameWnd__IEAAXIHH_Z(pThis, nType, cx, cy);
    if (cx == 0 || cy == 0) return;

    SIZE size = {cx, cy};
    void* pBits = nullptr;
    HBITMAP hbmp = impl__CreateBitmap_32_CDrawingManager__SAPEAUHBITMAP____AEBVCSize__PEAPEAX_Z(&size, &pBits);
    if (hbmp == nullptr) return;

    HWND hWnd = At<HWND>(pThis, kOffHWnd);
    HDC hdcClient = ::GetDC(hWnd);
    if (hdcClient == nullptr) {
        ::DeleteObject(hbmp);
        return;
    }
    HDC hdcMem = ::CreateCompatibleDC(hdcClient);

    HGDIOBJ hBmpOld = ::SelectObject(hdcMem, hbmp);

    RECT rect = {0, 0, cx, cy};   // CRect(CPoint(0, 0), size)
    impl__Draw_CMFCShadowRenderer__UEAAXPEAVCDC__VCRect__IE_Z(
        static_cast<char*>(pThis) + kOffShadow, impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(hdcMem),
        &rect, 0, 255);

    POINT point = {0, 0};
    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    ::UpdateLayeredWindow(hWnd, nullptr, nullptr, &size, hdcMem, &point, 0, &bf, ULW_ALPHA);

    ::SelectObject(hdcMem, hBmpOld);

    if (hdcMem != nullptr) {
        ::DeleteDC(hdcMem);
    }
    ::ReleaseDC(hWnd, hdcClient);
    ::DeleteObject(hbmp);
}

// Repos.  Retail RVA 0xb4af0 (mfc140u) = 0xb50b0 (mfc140.dll).  The mfc140u
// symbol map has no entry for this export (the mfc140.dll map does); 0xb4af0
// is 0xb50b0 shifted by the same -0x5c0 as Create/OnSize, its bytes match, and
// CMFCPopupMenu.cpp cites the same address.  Transcribed in full:
//   CRect rectWindow(0, 0, 0, 0);
//   ::GetWindowRect(m_pOwner->m_hWnd, &rectWindow);             // +0x1f0, hwnd +0x40
//   ::OffsetRect(&rectWindow, m_bIsRTL ? -m_nDepth : m_nDepth, m_nDepth);  // +0x400, +0x1f8
//   ::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
//   if (!::IsWindowVisible(m_hWnd)) {
//       ShowWindow(SW_SHOWNOACTIVATE /*4*/);
//       SetWindowPos(&wndTop, rectWindow.left, rectWindow.top, rectWindow.Width(),
//                    rectWindow.Height(), SWP_NOACTIVATE /*0x10*/);
//       m_pOwner->SetWindowPos(&wndTop, -1, -1, -1, -1,
//                    SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW /*0x53*/);
//   } else {
//       SetWindowPos(NULL, rectWindow.left, rectWindow.top, rectWindow.Width(),
//                    rectWindow.Height(), SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/);
//   }
//   ::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
//   ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW /*0x105*/);
// IAT: GetWindowRect, OffsetRect, SendMessage (x2), IsWindowVisible, RedrawWindow.
// ShowWindow / SetWindowPos are direct calls to the CWnd members.
// DEVIATION: a NULL pThis or NULL m_pOwner returns immediately; retail would fault.
// Symbol: ?Repos@CMFCShadowWnd@@AEAAXXZ
extern "C" void MS_ABI impl__Repos_CMFCShadowWnd__AEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pOwner = At<void*>(pThis, kOffOwner);
    if (pOwner == nullptr) return;
    HWND hWnd = At<HWND>(pThis, kOffHWnd);

    RECT rectWindow = {0, 0, 0, 0};
    ::GetWindowRect(At<HWND>(pOwner, kOffHWnd), &rectWindow);

    const int nDepth = At<int>(pThis, kOffDepth);
    ::OffsetRect(&rectWindow, At<BOOL>(pThis, kOffIsRTL) ? -nDepth : nDepth, nDepth);

    ::SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

    const int cx = rectWindow.right - rectWindow.left;
    const int cy = rectWindow.bottom - rectWindow.top;
    if (!::IsWindowVisible(hWnd)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pThis, SW_SHOWNOACTIVATE);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pThis, WndTop(), rectWindow.left, rectWindow.top,
                                                    cx, cy, SWP_NOACTIVATE);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pOwner, WndTop(), -1, -1, -1, -1,
                                                    SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    } else {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pThis, nullptr, rectWindow.left, rectWindow.top,
                                                    cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    ::SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    ::RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}
