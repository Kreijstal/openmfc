// CSmartDockingHighlighterWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The translucent rectangle the smart-docking manager shows over the place a
// dragged pane would dock to.  Retail declaration:
// atlmfc/include/afxsmartdockinghighlighterwnd.h:28, `class
// CSmartDockingHighlighterWnd : public CWnd` (friend CSmartDockingManager, no
// DECLARE_DYNAMIC -- slot 0 of its vftable is CWnd::GetRuntimeClass).  OpenMFC
// declares no matching class, so every body works on `void* pThis` through the
// layout pinned in HlLayout below.
//
// Images.  Every body was read in mfc140u.dll (disas.py --u) at the RVA that
// mfc140u.dll's own export address table gives for the ordinal in
// mfc_complete_ordinal_mapping.json (of the bodies here, mfc140u_rva_symbols.json
// names only the ctor, dtor, Create and Hide); ShowAt/Hide/Create/ctor/dtor/
// ShowTabbedAt were also read in mfc140.dll and are the same instruction
// streams.  Every import slot named below was resolved with iatu.py against
// mfc140u.dll.  The three handler RVAs are corroborated by the retail message
// map (VA 0x18030fb30 mfc140u, returned by GetMessageMap at RVA 0x131730, base
// map CWnd's; dumped with msgmap_u.py):
//   WM_PAINT      sig 19  RVA 0x131740  OnPaint
//   WM_CLOSE      sig 19  RVA 0x27d0    OnClose
//   WM_ERASEBKGND sig  1  RVA 0x3a60    OnEraseBkgnd
//
// Layout (sizeof 0x128), read from the retail constructor (RVA 0x131250,
// mfc140u), Hide, ShowAt and ShowTabbedAt; the member order agrees with the
// declaration order in the header:
//   +0x000  CWnd base (0xe8 bytes; the ctor calls ??0CWnd@@QEAA@XZ)
//   +0x040  CWnd::m_hWnd
//   +0x0e8  CWnd* m_pWndOwner          (ctor: 0; Create stores pwndOwner)
//   +0x0f0  CWnd* m_pDockingWnd        (ctor does NOT write it; set by the
//                                       inline SetDockingWnd, i.e. by
//                                       CSmartDockingManager::Start)
//   +0x0f8  CRect m_rectLast
//   +0x108  CRect m_rectTab
//   +0x118  BOOL  m_bTabbed
//   +0x11c  BOOL  m_bShown
//   +0x120  BOOL  m_bUseThemeColorInShading
// CSmartDockingManager.cpp embeds this object as m_wndPlaceMarker[0x128] and
// states that the retail scalar deleting destructor frees 0x128 bytes.
//
// Retail vftable: VA 0x18030fbc8 (mfc140u), dumped with vtdump_u.py.  It
// overrides only the destructor (slot 1) and GetMessageMap (slot 12, RVA
// 0x131730); slot 24 (+0xc0) is ?CreateEx@CWnd@@UEAAHKPEB_W0KAEBUtagRECT@@PEAV1@IPEAX@Z
// (RVA 0x28b440, mfc140u), which Create calls virtually.
//
// Virtual dispatch.  OpenMFC has no MSVC-layout vftable for this class:
// ??0CWnd@@QEAA@XZ (core/window/CtorDtorPlacement.cpp) placement-news
// OpenMFC's CWnd, so an object built by the constructor below carries OpenMFC's
// own CWnd vtable, on which MSVC slot numbers mean something else.  Same
// convention as featurepack/controls/CMFCPreviewCtrlImpl.cpp: the constructor
// records that vptr (g_ownVptr); an object still carrying it is called through
// the exported thunk its retail slot holds, any other vptr (a client subclass
// compiled against the real headers) is dispatched through the MSVC slot as
// retail does.
// LIMITATION, not fixed here: because a DLL-built object carries OpenMFC's
// CWnd vtable, its GetMessageMap is CWnd's, so WM_PAINT / WM_CLOSE /
// WM_ERASEBKGND for the window never reach OnPaint / OnClose / OnEraseBkgnd
// below (and MessageMaps.cpp's map for this class has no entries -- see
// detail/Pane17MsgmapSupport.cpp).  The handlers are correct when reached by
// export.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp
extern "C" int   MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* lpParam);    // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" const unsigned char impl__wndTop_CWnd__2V1_B[];                            // core/window/CWnd.cpp (HWND_TOP)
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon);           // core/runtime/Globals.cpp
extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);      // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];              // featurepack/CMFC_misc_stubs.cpp
extern "C" COLORREF MS_ABI impl__PixelAlpha_CDrawingManager__SAKKH_Z(COLORREF srcPixel, int nPercent); // core/gdi/CDrawingManager.cpp
extern "C" int impl__m_bTabsAlwaysTop_CTabbedPane__2HA;                               // featurepack/docking/CTabbedPane.cpp

// This file's own thunks that other bodies here call directly (defined below).
extern "C" void MS_ABI impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(void* pThis);

namespace {

struct HlLayout {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                      // +0x040  CWnd::m_hWnd
    unsigned char m_cwndTail[0xe8 - 0x48];
    CWnd*         m_pWndOwner;                 // +0x0e8
    CWnd*         m_pDockingWnd;               // +0x0f0
    RECT          m_rectLast;                  // +0x0f8  CRect
    RECT          m_rectTab;                   // +0x108  CRect
    BOOL          m_bTabbed;                   // +0x118
    BOOL          m_bShown;                    // +0x11c
    BOOL          m_bUseThemeColorInShading;   // +0x120
    int           _pad124;                     // +0x124
};
static_assert(offsetof(HlLayout, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(HlLayout, m_pWndOwner) == 0xe8, "m_pWndOwner +0xe8");
static_assert(offsetof(HlLayout, m_pDockingWnd) == 0xf0, "m_pDockingWnd +0xf0");
static_assert(offsetof(HlLayout, m_rectLast) == 0xf8, "m_rectLast +0xf8");
static_assert(offsetof(HlLayout, m_rectTab) == 0x108, "m_rectTab +0x108");
static_assert(offsetof(HlLayout, m_bTabbed) == 0x118, "m_bTabbed +0x118");
static_assert(offsetof(HlLayout, m_bShown) == 0x11c, "m_bShown +0x11c");
static_assert(offsetof(HlLayout, m_bUseThemeColorInShading) == 0x120, "m_bUseThemeColorInShading +0x120");
static_assert(sizeof(HlLayout) == 0x128, "scalar deleting destructor frees 0x128");
// The CWnd constructor thunk placement-news OpenMFC's CWnd over the first 0xe8
// bytes, so it must end exactly where m_pWndOwner starts.
static_assert(sizeof(CWnd) == 0xe8, "CWnd base must end where m_pWndOwner starts");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads HWNDs at +0x40");

inline HlLayout* L(void* p) { return static_cast<HlLayout*>(p); }
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
inline const CWnd* WndTop() { return reinterpret_cast<const CWnd*>(impl__wndTop_CWnd__2V1_B); }

// ---- virtual dispatch on `this` (file header) ------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the constructor
inline bool HasOwnVptr(const void* p) {
    return g_ownVptr != nullptr && *static_cast<void* const*>(p) == g_ownVptr;
}
inline void* SlotAt(const void* p, std::size_t off) {
    return *reinterpret_cast<void* const*>(*static_cast<const unsigned char* const*>(p) + off);
}
// CWnd::CreateEx(DWORD, LPCTSTR, LPCTSTR, DWORD, const RECT&, CWnd*, UINT, LPVOID), slot 24 (+0xc0).
using PFN_CreateExRect = int (MS_ABI*)(void*, DWORD, const wchar_t*, const wchar_t*, DWORD,
                                       const RECT*, CWnd*, UINT, void*);
constexpr std::size_t kSlotCreateExRect = 0xc0;

// ---- afxGlobalData (720-byte exported blob) --------------------------------
// +0x000 m_bInitialized is the gate every retail reader tests
// (`cmpl $0,afxGlobalData; jne; call Initialize; movl $1,afxGlobalData`);
// +0x088 clrActiveCaption and +0x288 m_nBitsPerPixel are the two fields read
// here (retail VAs 0x1803c16a8 / 0x1803c18a8 against ?afxGlobalData@@ at
// 0x1803c1620, all mfc140u).  Both offsets agree with the AfxGlobalData
// transcription in core/runtime/AFX_GLOBAL_DATA.cpp (m_nBitsPerPixel is
// static_asserted there; clrActiveCaption lies between the asserted clrWindow
// +0x078 and clrInactiveBorder +0x0a0 in a run of COLORREFs).
constexpr std::size_t kGdInitGate         = 0x000;
constexpr std::size_t kGdClrActiveCaption = 0x088;
constexpr std::size_t kGdBitsPerPixel     = 0x288;
inline int GdInt(std::size_t off) {
    int v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}
inline void EnsureGlobalDataInitialized() {
    if (GdInt(kGdInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdInitGate, &one, sizeof one);
    }
}

// Retail immediates.
constexpr UINT kShowFlags = 0x58;        // SWP_NOREDRAW | SWP_NOACTIVATE | SWP_SHOWWINDOW
constexpr UINT kRedrawFlags = 0x105;     // RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW
constexpr COLORREF kDefaultShade = 0xbe672f;   // RGB(0x2f, 0x67, 0xbe)

inline int RcWidth(const RECT& r) { return r.right - r.left; }
inline int RcHeight(const RECT& r) { return r.bottom - r.top; }

} // namespace

// Symbol: ??0CSmartDockingHighlighterWnd@@QEAA@XZ
// Transcribed from retail RVA 0x131250 (mfc140u):
//     CWnd::CWnd();                                  // call 0x28a700 (mfc140u)
//     vfptr = &CSmartDockingHighlighterWnd::`vftable';   // VA 0x18030fbc8 (mfc140u)
//     m_pWndOwner = NULL;
//     m_rectLast = m_rectTab = {0,0,0,0};            // two 8-byte zero stores each
//     *(qword*)&m_bTabbed = 0;                       // m_bTabbed = m_bShown = FALSE
//     m_bUseThemeColorInShading = FALSE;
//     ::SetRectEmpty(&m_rectLast); ::SetRectEmpty(&m_rectTab);
//     return this;
// m_pDockingWnd (+0xf0) is not written, as in retail.
// DEVIATION: no MSVC-layout vftable is installed -- `this` keeps the vptr
// ??0CWnd@@ installs, recorded as g_ownVptr (file header).
extern "C" void* MS_ABI impl___0CSmartDockingHighlighterWnd__QEAA_XZ(void* pThis) {
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    HlLayout* self = L(pThis);
    self->m_pWndOwner = nullptr;
    self->m_bTabbed = FALSE;
    self->m_bShown = FALSE;
    self->m_bUseThemeColorInShading = FALSE;
    ::SetRectEmpty(&self->m_rectLast);
    ::SetRectEmpty(&self->m_rectTab);
    return pThis;
}
// Symbol: ??1CSmartDockingHighlighterWnd@@UEAA@XZ
// Transcribed from retail RVA 0x131310 (mfc140u), complete:
//     vfptr = &CSmartDockingHighlighterWnd::`vftable';   // VA 0x18030fbc8 (mfc140u)
//     CWnd::~CWnd();                                      // tail jump to 0x28b740 (mfc140u)
// DEVIATION: the own-vftable store becomes a g_ownVptr store (file header), so
// that ??1CWnd@@'s C++ destructor runs on OpenMFC's table and not on a client's.
// Declared `void` like every other destructor thunk; CSmartDockingManager.cpp
// declares it returning void*, which reads a garbage return value it ignores.
extern "C" void MS_ABI impl___1CSmartDockingHighlighterWnd__UEAA_XZ(void* pThis) {
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CWnd__UEAA_XZ(pThis);
}

// Symbol: ?Create@CSmartDockingHighlighterWnd@@QEAAXPEAVCWnd@@@Z
// Transcribed from retail RVA 0x131320 (mfc140u):
//     m_pWndOwner = pwndOwner;
//     CRect rect; rect.SetRectEmpty();                    // ::SetRectEmpty (IAT 0x1802c7348)
//     <afxGlobalData init gate>
//     int nBpp = afxGlobalData.m_nBitsPerPixel;           // read once, used twice
//     this->CreateEx(nBpp > 8 ? WS_EX_LAYERED : 0,        // vftable +0xc0 (slot 24)
//                    AfxRegisterWndClass(0, 0, 0, 0),     // call 0x28c4a0 (mfc140u)
//                    _T(""),                              // VA 0x18033d19c (mfc140u), empty
//                    WS_POPUP, rect, pwndOwner, 0, NULL); // result not tested
//     if (nBpp > 8)
//         ::SetLayeredWindowAttributes(m_hWnd, 0, 100, LWA_ALPHA);   // IAT 0x1802c6c60
//     m_bUseThemeColorInShading =
//         CDockingManager::m_SDParams.m_bUseThemeColorInShading;     // VA 0x1803c1604 (mfc140u)
// DEVIATION: the last store is not made.  VA 0x1803c1604 is
// ?m_SDParams@CDockingManager@@ (0x1803c15b0) + 0x54, CSmartDockingInfo's
// m_bUseThemeColorInShading (detail/CSmartDockingInfoSupport.h, offset 84); but
// OpenMFC exports m_SDParams as an 8-byte placeholder
// (featurepack/docking/CDockingManager.cpp), so +0x54 lies outside it.  The
// member keeps the FALSE the constructor stored.
extern "C" void MS_ABI impl__Create_CSmartDockingHighlighterWnd__QEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pwndOwner) {
    HlLayout* self = L(pThis);
    self->m_pWndOwner = pwndOwner;
    RECT rect;
    ::SetRectEmpty(&rect);
    EnsureGlobalDataInitialized();
    const int nBitsPerPixel = GdInt(kGdBitsPerPixel);
    const wchar_t* lpszClass = impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(0, nullptr, nullptr, nullptr);
    const DWORD dwExStyle = (nBitsPerPixel > 8) ? WS_EX_LAYERED : 0;
    if (HasOwnVptr(pThis)) {
        impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
            W(pThis), dwExStyle, lpszClass, L"", WS_POPUP, rect, pwndOwner, 0, nullptr);
    } else {
        reinterpret_cast<PFN_CreateExRect>(SlotAt(pThis, kSlotCreateExRect))(
            pThis, dwExStyle, lpszClass, L"", WS_POPUP, &rect, pwndOwner, 0, nullptr);
    }
    if (nBitsPerPixel > 8) {
        ::SetLayeredWindowAttributes(self->m_hWnd, 0, 100, LWA_ALPHA);
    }
}

// Symbol: ?Hide@CSmartDockingHighlighterWnd@@QEAAXXZ
// Transcribed from retail RVA 0x131500 (mfc140u), complete:
//     if (!m_bShown) return;
//     ShowWindow(SW_HIDE);                          // CWnd::ShowWindow, call 0x2a9ad0 (mfc140u)
//     m_bShown = FALSE;
//     if (m_pWndOwner != NULL)   ::UpdateWindow(m_pWndOwner->m_hWnd);     // IAT 0x1802c7300
//     if (m_pDockingWnd != NULL) ::UpdateWindow(m_pDockingWnd->m_hWnd);
//     ::SetRectEmpty(&m_rectLast);                  // IAT 0x1802c7348
//     ::SetRectEmpty(&m_rectTab);
// m_bTabbed is not touched (ShowAt / ShowTabbedAt read it after calling Hide).
extern "C" void MS_ABI impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(void* pThis) {
    HlLayout* self = L(pThis);
    if (self->m_bShown == 0) {
        return;
    }
    impl__ShowWindow_CWnd__QEAAHH_Z(W(pThis), SW_HIDE);
    self->m_bShown = FALSE;
    if (self->m_pWndOwner != nullptr) {
        ::UpdateWindow(self->m_pWndOwner->m_hWnd);
    }
    if (self->m_pDockingWnd != nullptr) {
        ::UpdateWindow(self->m_pDockingWnd->m_hWnd);
    }
    ::SetRectEmpty(&self->m_rectLast);
    ::SetRectEmpty(&self->m_rectTab);
}

// Symbol: ?OnClose@CSmartDockingHighlighterWnd@@IEAAXXZ
// Retail: the export (ordinal 8882) resolves in mfc140u.dll's export table to
// RVA 0x27d0, a COMDAT-folded body that is a single `ret` -- the handler
// deliberately does nothing, so WM_CLOSE never reaches CWnd::OnClose (which
// would destroy the window).  An empty body is the complete transcription.
extern "C" void MS_ABI impl__OnClose_CSmartDockingHighlighterWnd__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnEraseBkgnd@CSmartDockingHighlighterWnd@@IEAAHPEAVCDC@@@Z
// Retail: the export (ordinal 9791) resolves in mfc140u.dll's export table to
// RVA 0x3a60, a COMDAT-folded `mov $1,%eax; ret` -- return TRUE without
// erasing (OnPaint covers the whole client rectangle).  Complete transcription.
extern "C" int MS_ABI impl__OnEraseBkgnd_CSmartDockingHighlighterWnd__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Symbol: ?OnPaint@CSmartDockingHighlighterWnd@@IEAAXXZ
// Transcribed from retail RVA 0x131740 (mfc140u; the export, ordinal 10762,
// resolves there in mfc140u.dll's export table):
//     CPaintDC dc(this);                                   // call 0x2a3d20 (mfc140u)
//     if (m_bShown) {
//         CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);   // IAT 0x1802c7330
//         COLORREF clr;
//         if (m_bUseThemeColorInShading) {
//             <afxGlobalData init gate>
//             clr = afxGlobalData.clrActiveCaption;        // +0x88
//         } else
//             clr = RGB(0x2f, 0x67, 0xbe);                 // 0xbe672f
//         <afxGlobalData init gate>
//         if (afxGlobalData.m_nBitsPerPixel > 8) {         // +0x288
//             CBrush br(CDrawingManager::PixelAlpha(clr, 105));        // 0x5b270 / 0x2a4060 (mfc140u)
//             ::FillRect(dc.m_hDC, &rectClient, (HBRUSH)br.m_hObject);  // IAT 0x1802c7208
//         } else {
//             CBrush br(CDrawingManager::PixelAlpha(
//                 RGB(255 - GetRValue(clr), 255 - GetGValue(clr), 255 - GetBValue(clr)), 50));
//             CBrush* pOld = dc.SelectObject(&br);         // call 0x2a2730 (mfc140u)
//             ::PatBlt(dc.m_hDC, 0, 0, rectClient.Width(), rectClient.Height(),
//                      PATINVERT);                          // 0x5a0049, IAT 0x1802c6140
//             dc.SelectObject(pOld);
//         }
//     }                                                    // ~CBrush: DeleteObject
//                                                          // ~CPaintDC: EndPaint
// DEVIATIONS: the CPaintDC and CBrush objects are replaced by the raw calls
// they wrap (::BeginPaint/::EndPaint, ::CreateSolidBrush/::DeleteObject,
// ::SelectObject), since OpenMFC's GDI wrapper classes exist only behind their
// exported thunks.  Retail's CPaintDC and CBrush constructors (RVAs 0x2a3d20 /
// 0x2a4060, mfc140u) call AfxThrowResourceException (0x2a42e0) when the
// CDC/CGdiObject Attach of the BeginPaint / CreateSolidBrush result fails
// (the throw calls are at 0x2a3d78 inside ??0CPaintDC and 0x2a409f inside
// ??0CBrush@@QEAA@K@Z).  Here nothing is thrown; the observable effects of
// those throws are reproduced instead: a NULL DC returns at once without
// EndPaint (retail's CPaintDC never finished constructing, so its destructor
// does not run), and a NULL brush skips the FillRect / PatBlt but still runs
// EndPaint (retail unwinds through ~CPaintDC).  PatBlt in particular must be
// skipped: with a NULL brush the SelectObject fails and PATINVERT would paint
// with whatever brush the DC already holds.
extern "C" void MS_ABI impl__OnPaint_CSmartDockingHighlighterWnd__IEAAXXZ(void* pThis) {
    HlLayout* self = L(pThis);
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(self->m_hWnd, &ps);
    if (hdc == nullptr) {
        return;   // retail: AfxThrowResourceException out of ??0CPaintDC, no EndPaint
    }
    if (self->m_bShown != 0) {
        RECT rectClient = {0, 0, 0, 0};
        ::GetClientRect(self->m_hWnd, &rectClient);
        COLORREF clr;
        if (self->m_bUseThemeColorInShading != 0) {
            EnsureGlobalDataInitialized();
            clr = static_cast<COLORREF>(GdInt(kGdClrActiveCaption));
        } else {
            clr = kDefaultShade;
        }
        EnsureGlobalDataInitialized();
        if (GdInt(kGdBitsPerPixel) > 8) {
            HBRUSH hbr = ::CreateSolidBrush(impl__PixelAlpha_CDrawingManager__SAKKH_Z(clr, 105));
            if (hbr != nullptr) {   // retail: ??0CBrush throws before FillRect
                ::FillRect(hdc, &rectClient, hbr);
                ::DeleteObject(hbr);
            }
        } else {
            const COLORREF clrInv = RGB(255 - GetRValue(clr), 255 - GetGValue(clr), 255 - GetBValue(clr));
            HBRUSH hbr = ::CreateSolidBrush(impl__PixelAlpha_CDrawingManager__SAKKH_Z(clrInv, 50));
            if (hbr != nullptr) {   // retail: ??0CBrush throws before SelectObject / PatBlt
                HGDIOBJ hOld = ::SelectObject(hdc, hbr);
                ::PatBlt(hdc, 0, 0, RcWidth(rectClient), RcHeight(rectClient), PATINVERT);
                ::SelectObject(hdc, hOld);
                ::DeleteObject(hbr);
            }
        }
    }
    ::EndPaint(self->m_hWnd, &ps);
}

// Symbol: ?ShowAt@CSmartDockingHighlighterWnd@@QEAAXVCRect@@@Z
// Transcribed from retail RVA 0x131430 (mfc140u; ordinal 13793 in
// mfc140u.dll's export table), complete:
//     if (!m_bTabbed && ::EqualRect(&m_rectLast, &rect)) return;   // IAT 0x1802c72c0
//     Hide();                                                        // 0x131500
//     if (m_bTabbed) {
//         ::SetWindowRgn(m_hWnd, NULL, FALSE);                       // IAT 0x1802c6d10
//         m_bTabbed = FALSE;
//     }
//     SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(),
//                  SWP_NOREDRAW | SWP_NOACTIVATE | SWP_SHOWWINDOW);  // call 0x2a9a60, 0x58
//     m_bShown = TRUE;
//     m_rectLast = rect;
//     ::RedrawWindow(m_hWnd, NULL, NULL,
//                    RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);    // IAT 0x1802c7130, 0x105
// `&wndTop` is ?wndTop@CWnd@@2V1@B (RVA 0x3c3370 in mfc140u).  The CRect by
// value arrives as a pointer to the caller's copy under the x64 MS ABI.
extern "C" void MS_ABI impl__ShowAt_CSmartDockingHighlighterWnd__QEAAXVCRect___Z(
    void* pThis, const RECT* pRect) {
    HlLayout* self = L(pThis);
    const RECT rect = *pRect;
    if (self->m_bTabbed == 0 && ::EqualRect(&self->m_rectLast, &rect)) {
        return;
    }
    impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(pThis);
    if (self->m_bTabbed != 0) {
        ::SetWindowRgn(self->m_hWnd, nullptr, FALSE);
        self->m_bTabbed = FALSE;
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
        W(pThis), WndTop(), rect.left, rect.top, RcWidth(rect), RcHeight(rect), kShowFlags);
    self->m_bShown = TRUE;
    self->m_rectLast = rect;
    ::RedrawWindow(self->m_hWnd, nullptr, nullptr, kRedrawFlags);
}

// Symbol: ?ShowTabbedAt@CSmartDockingHighlighterWnd@@QEAAXVCRect@@0@Z
// Transcribed from retail RVA 0x131570 (mfc140u; ordinal 13853 in
// mfc140u.dll's export table), complete:
//     if (m_bTabbed && ::EqualRect(&m_rectLast, &rect)
//                   && ::EqualRect(&m_rectTab, &rectTab)) return;   // IAT 0x1802c72c0
//     Hide();                                                        // 0x131500
//     BOOL bTop = CTabbedPane::m_bTabsAlwaysTop;                     // VA 0x1803be2d8 (mfc140u), re-read at each use
//     CRgn rgnMain;                                                  // CRgn vftable VA 0x1802e29e0 (mfc140u)
//     rgnMain.Attach(::CreateRectRgn(0, bTop ? rectTab.Height() : 0, rect.Width(),
//                                    rect.Height() + (bTop ? rectTab.Height() : 0)));
//     CRgn rgnTab;
//     rgnTab.Attach(bTop ? ::CreateRectRgn(rectTab.left, 0, rectTab.Width(), rectTab.Height())
//                        : ::CreateRectRgnIndirect(&rectTab));      // IAT 0x1802c6138 / 0x1802c61e0
//     ::CombineRgn(rgnMain, rgnMain, rgnTab, RGN_OR);                // IAT 0x1802c6130
//     ::SetWindowRgn(m_hWnd, rgnMain, FALSE);                        // IAT 0x1802c6d10
//     m_bTabbed = TRUE;
//     m_rectLast = rect;  m_rectTab = rectTab;
//     SetWindowPos(&wndTop, rect.left, bTop ? rectTab.top : rect.top,
//                  rect.Width(), rect.Height() + m_rectTab.Height(), 0x58);   // call 0x2a9a60
//     m_bShown = TRUE;
//     ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);                     // IAT 0x1802c7130
//     // ~rgnTab, ~rgnMain: CGdiObject::DeleteObject (call 0x1c6f0 -> 0x2a3f60, mfc140u)
// The odd rectangles are retail's: the always-on-top tab region's right/bottom
// are rectTab's width/height, not its right/bottom.  Retail's ~CRgn also calls
// DeleteObject on rgnMain even though SetWindowRgn has handed it to the system;
// that is transcribed as-is.
// DEVIATION: the two CRgn locals are raw HRGNs; CGdiObject::Attach(NULL)
// leaves a NULL handle and ~CGdiObject deletes only a non-NULL one, so the
// `!= NULL` tests reproduce that.  (Retail's Attach also enters the handle in
// the permanent GDI handle map and DeleteObject removes it again; no net
// effect.)
extern "C" void MS_ABI impl__ShowTabbedAt_CSmartDockingHighlighterWnd__QEAAXVCRect__0_Z(
    void* pThis, const RECT* pRect, const RECT* pRectTab) {
    HlLayout* self = L(pThis);
    const RECT rect = *pRect;
    const RECT rectTab = *pRectTab;
    if (self->m_bTabbed != 0 && ::EqualRect(&self->m_rectLast, &rect) &&
        ::EqualRect(&self->m_rectTab, &rectTab)) {
        return;
    }
    impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(pThis);

    const int tabTopOffset = (impl__m_bTabsAlwaysTop_CTabbedPane__2HA != 0) ? RcHeight(rectTab) : 0;
    HRGN hrgnMain = ::CreateRectRgn(0, tabTopOffset, RcWidth(rect), RcHeight(rect) + tabTopOffset);
    HRGN hrgnTab = (impl__m_bTabsAlwaysTop_CTabbedPane__2HA != 0)
        ? ::CreateRectRgn(rectTab.left, 0, RcWidth(rectTab), RcHeight(rectTab))
        : ::CreateRectRgnIndirect(&rectTab);
    ::CombineRgn(hrgnMain, hrgnMain, hrgnTab, RGN_OR);
    ::SetWindowRgn(self->m_hWnd, hrgnMain, FALSE);

    self->m_bTabbed = TRUE;
    self->m_rectLast = rect;
    self->m_rectTab = rectTab;
    const int y = (impl__m_bTabsAlwaysTop_CTabbedPane__2HA != 0) ? rectTab.top : rect.top;
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
        W(pThis), WndTop(), rect.left, y, RcWidth(rect),
        RcHeight(rect) + RcHeight(self->m_rectTab), kShowFlags);
    self->m_bShown = TRUE;
    ::RedrawWindow(self->m_hWnd, nullptr, nullptr, kRedrawFlags);

    if (hrgnTab != nullptr) ::DeleteObject(hrgnTab);
    if (hrgnMain != nullptr) ::DeleteObject(hrgnMain);
}
