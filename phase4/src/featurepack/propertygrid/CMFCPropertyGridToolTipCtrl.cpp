// CMFCPropertyGridToolTipCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The in-place tooltip window of CMFCPropertyGridCtrl.  Shipping declaration:
// atlmfc/include/afxpropertygridtooltipctrl.h:28 (read on this host):
//     class CMFCPropertyGridToolTipCtrl : public CWnd
//     { static CString m_strClassName;
//       CString m_strText; CRect m_rectLast; int m_nTextMargin; HFONT m_hFont; CWnd* m_pWndParent; ... };
// OpenMFC's headers do not declare the class, so `this` is a void* and the
// layout is pinned below (S_PGToolTip).
//
// Every body is transcribed from the retail mfc140u.dll disassembly.  The ctor,
// dtor, Create, Hide and Deactivate are named by mfc140u_rva_symbols.json.  The
// other five are missing from that JSON map, but mfc140u's own export address
// table resolves every one of them by ordinal (mfc_complete_ordinal_mapping.json):
// OnEraseBkgnd #9768 -> 0x3a60, OnPaint #10744 -> 0xcc720, OnSetFont #11091 ->
// 0xccdc0, PreTranslateMessage #11882 -> 0xcce10, Track #13992 -> 0xcc9f0.
// Those RVAs agree with retail's own tables:
//   * vftable 0x1802fb6e8 (mfc140u) -- the address the ctor (0xcc500) and dtor
//     (0xcc5d0) store at +0 -- dumped slot by slot:
//       slot  0 (+0x000)  GetRuntimeClass            0xcc4f0
//       slot 12 (+0x060)  GetMessageMap              0xcc620
//       slot 25 (+0x0c8)  CWnd::CreateEx (11 args)   0x28b500  (not overridden)
//       slot 69 (+0x228)  PreTranslateMessage        0xcce10   (not in the map; see below)
//       slot 91 (+0x2d8)  Create                     0xcc630
//   * the message map 0x1802fb650 (mfc140u; returned by GetMessageMap 0xcc620),
//     pfnGetBaseMap 0x28d0e0 (named ?GetMessageMap@CWnd@@ by the symbol map):
//       WM_ERASEBKGND  sig  1  0x3a60    OnEraseBkgnd  (a folded `mov $1,%eax; ret`)
//       WM_PAINT       sig 19  0xcc720   OnPaint
//       WM_SETFONT     sig 88  0xccdc0   OnSetFont
//   * Track is in neither the vftable nor the message map; its export entry
//     0xcc9f0 (mfc140u) is also the one CMFCPropertyGridCtrl.cpp cites for
//     `m_IPToolTip.Track(...)`, and its body reads exactly this class's
//     members (+0xe8 .. +0x110).
// Slot 69's body (0xcce10, the PreTranslateMessage export) also falls through
// to a direct call of ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z (0x28bc00)
// on every message it does not handle.
//
// ---------------------------------------------------------------------------
// Retail layout, from the constructor ??0CMFCPropertyGridToolTipCtrl@@QEAA@XZ
// (entry RVA 0xcc500, mfc140u) and the scalar-deleting destructor that follows
// it (0xcc580, vftable slot 1), whose `flags & 4` branch passes 0x118 (the
// object size) in EDX to a call of 0x27d0 -- in mfc140u a folded bare `ret 0`
// body, so that branch frees nothing; the other branch calls the CRT free()
// import (0x1802c74e8).  The 0x118 constant is the size evidence:
//   +0x000  CWnd base (0xe8 bytes; ctor calls ??0CWnd@@QEAA@XZ, 0x28a700)
//   +0x040  CWnd::m_hWnd
//   +0x0e8  CString m_strText     ctor: afxStringManager->GetNilString() + 0x18
//   +0x0f0  CRect   m_rectLast    ctor: two zero qword stores, then ::SetRectEmpty
//   +0x100  int     m_nTextMargin ctor: 10
//   +0x108  HFONT   m_hFont       ctor: NULL
//   +0x110  CWnd*   m_pWndParent  ctor: NULL; Create stores its argument here
//   sizeof == 0x118 (280, the object size featurepack/propertygrid/RuntimeClasses.cpp
//   also gives this class's descriptor)
//
// ---------------------------------------------------------------------------
// Virtual dispatch.  Only Create makes a virtual call on `this` (CreateEx,
// slot 25).  Same convention as featurepack/controls/CMFCPreviewCtrlImpl.cpp:
// ??0CWnd@@QEAA@XZ (core/window/CtorDtorPlacement.cpp) placement-news OpenMFC's
// CWnd, so an object built by the constructor here carries OpenMFC's mingw CWnd
// vtable (recorded as g_ownVptr); while `this` still carries it the call goes
// straight to the CWnd::CreateEx thunk the retail slot holds, otherwise (a
// client subclass compiled against the real headers installed its own MSVC
// vftable) the call is made through that table's slot +0xc8 exactly as retail
// does.
// LIMITATION, not fixed here: an object built by this constructor keeps
// OpenMFC's CWnd vtable, whose GetMessageMap/PreTranslateMessage are CWnd's,
// and detail/Propgrid18MsgmapSupport.cpp gives this class an EMPTY message map.
// So OnEraseBkgnd / OnPaint / OnSetFont / PreTranslateMessage below are
// reachable as exported entry points but are not yet wired to the window's
// message routing.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <new>

// Sibling thunks, each declared with the signature its mangled name describes;
// the file that defines each one (with a // Symbol: marker) is named.
extern "C" void*    MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                   // core/window/CtorDtorPlacement.cpp
extern "C" void     MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);                                   // core/window/CtorDtorPlacement.cpp
extern "C" int      MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu,
    void* lpParam);                                                                               // core/window/CWnd.cpp
extern "C" const wchar_t* MS_ABI impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
    unsigned int nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon);                // core/runtime/Globals.cpp
extern "C" int      MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);           // core/window/CWnd.cpp
extern "C" int      MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);               // core/window/Thunks.cpp
extern "C" int      MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg); // core/window/CWnd.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);   // core/gdi/CPaintDC.cpp
extern "C" void     MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                           // core/gdi/CPaintDC.cpp
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd); // core/gdi/CClientDC.cpp
extern "C" void     MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);                         // core/gdi/CClientDC.cpp
extern "C" CBrush*  MS_ABI impl___0CBrush__QEAA_K_Z(CBrush* pThis, unsigned long crColor);       // core/gdi/CBrush.cpp
extern "C" int      MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);            // core/gdi/CGdiObject.cpp
extern "C" void     MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);      // core/gdi/CDC.cpp
extern "C" void*    MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC);    // core/gdi/CDrawingManager.cpp
extern "C" void     MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis);                        // core/gdi/CDrawingManager.cpp
extern "C" void     MS_ABI impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(
    void* pThis, CRect* pRect, unsigned long colorStart, unsigned long colorFinish, int nAngle);  // core/gdi/CDrawingManager.cpp
extern "C" int      MS_ABI impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
    CMFCVisualManager* pThis, CMFCToolTipInfo& params, unsigned int nType);                       // visualmanager/CMFCVisualManager.cpp
extern "C" void     MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                          // detail/MfcExceptionsSupport.cpp
extern "C" void     MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);               // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                         // featurepack/CMFC_misc_stubs.cpp; 0x3c1620 (mfc140u)
extern "C" void*    impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                            // core/runtime/StaticData.cpp; 0x3be3c0 (mfc140u)
extern "C" const unsigned char impl__wndTop_CWnd__2V1_B[];                                       // core/window/CWnd.cpp (?wndTop@CWnd@@2V1@B, 0x3c3370 mfc140u; m_hWnd == HWND_TOP)

// This file's own thunk that another body here calls directly (defined below).
extern "C" void MS_ABI impl__Hide_CMFCPropertyGridToolTipCtrl__QEAAXXZ(void* pThis);

// The static class-name string (defined at the bottom of this file).
extern "C" CString impl__m_strClassName_CMFCPropertyGridToolTipCtrl__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A;

namespace {

struct S_PGToolTip {
    unsigned char m_cwndHead[0x40];
    HWND          m_hWnd;                     // +0x040  CWnd::m_hWnd
    unsigned char m_cwndTail[0xe8 - 0x48];
    CString       m_strText;                  // +0x0e8
    RECT          m_rectLast;                 // +0x0f0
    int           m_nTextMargin;              // +0x100
    HFONT         m_hFont;                    // +0x108
    void*         m_pWndParent;               // +0x110  CWnd*
};
static_assert(offsetof(S_PGToolTip, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_PGToolTip, m_strText) == 0xe8, "ctor: mov %rax,0xe8(%rbx)");
static_assert(offsetof(S_PGToolTip, m_rectLast) == 0xf0, "ctor: lea 0xf0(%rbx),%rcx; SetRectEmpty");
static_assert(offsetof(S_PGToolTip, m_nTextMargin) == 0x100, "ctor: movl $0xa,0x100(%rbx)");
static_assert(offsetof(S_PGToolTip, m_hFont) == 0x108, "ctor: movq $0,0x108(%rbx); OnSetFont stores here");
static_assert(offsetof(S_PGToolTip, m_pWndParent) == 0x110, "ctor: movq $0,0x110(%rbx); Create stores its argument here");
static_assert(sizeof(S_PGToolTip) == 0x118, "retail deleting dtor frees 0x118 bytes");
static_assert(sizeof(CWnd) == 0xe8, "CWnd base must end where m_strText starts");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");
static_assert(sizeof(CString) == 8, "CString is one CStringT data pointer");
static_assert(offsetof(CDC, m_hDC) == 0x8 && offsetof(CDC, m_hAttribDC) == 0x10, "CDC::m_hDC at +0x8, m_hAttribDC at +0x10");
static_assert(sizeof(CGdiObject) == 0x10, "CGdiObject = vptr + m_hObject (m_hObject at +0x8, as OnSetFont reads it)");
static_assert(sizeof(CRect) == sizeof(RECT), "CRect mirrors RECT");

inline S_PGToolTip* L(void* p) { return static_cast<S_PGToolTip*>(p); }
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
// CWnd::GetSafeHwnd() as retail inlines it on a CWnd* member: `p ? p->m_hWnd : NULL`.
inline HWND SafeHwnd(const void* pWnd) {
    return pWnd ? static_cast<const S_PGToolTip*>(pWnd)->m_hWnd : nullptr;   // +0x40 on any CWnd
}

// ---------------------------------------------------------------------------
// Virtual dispatch on `this` (file header).
// ---------------------------------------------------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run
inline bool HasOwnVptr(const void* p) {
    return g_ownVptr != nullptr && *static_cast<void* const*>(p) == g_ownVptr;
}
inline void* SlotAt(const void* p, std::size_t off) {
    return *reinterpret_cast<void* const*>(*static_cast<const unsigned char* const*>(p) + off);
}
using PFN_CreateEx = int (MS_ABI*)(void*, DWORD, const wchar_t*, const wchar_t*, DWORD,
                                   int, int, int, int, HWND, HMENU, void*);   // CWnd slot 25 (+0xc8)

// CDC::SelectObject(CFont*) / SelectStockObject as retail implements them
// (0x2a2730 / 0x2a26d0, mfc140u): select into m_hDC when it differs from
// m_hAttribDC, then into m_hAttribDC when non-NULL; the result is the handle
// the LAST ::SelectObject returned (NULL when neither ran), which retail wraps
// with CGdiObject::FromHandle (NULL stays NULL).
HGDIOBJ DcSelect(const CDC* pDC, HGDIOBJ h) {
    HGDIOBJ hOld = nullptr;
    if (pDC->m_hDC != pDC->m_hAttribDC) hOld = ::SelectObject(pDC->m_hDC, h);
    if (pDC->m_hAttribDC != nullptr) hOld = ::SelectObject(pDC->m_hAttribDC, h);
    return hOld;
}
// The font selection OnPaint and Track share (0xcc8b9.. / 0xcca8d..):
//     pOldFont = m_hFont == NULL ? dc.SelectStockObject(DEFAULT_GUI_FONT /*0x11*/)
//                                : dc.SelectObject(CFont::FromHandle(m_hFont));
HGDIOBJ SelectToolTipFont(void* pThis, const CDC* pDC) {
    HFONT hFont = L(pThis)->m_hFont;
    return DcSelect(pDC, hFont == nullptr ? ::GetStockObject(DEFAULT_GUI_FONT) : hFont);
}

// ---- CMFCToolTipInfo, as retail lays it out (13 dwords, afxtooltipctrl.h) ----
// OnPaint builds one on the stack with the inline CMFCToolTipInfo() defaults:
// the first eight dwords are copied from 0x180350140 = {0,1,1,0} and
// 0x1803501f0 = {1,1,0,200} (mfc140u, read from the image), the last five are
// stored as -1.  Same view as featurepack/controls/CMFCToolTipCtrl.cpp.
struct TT_Params {
    int m_bBalloonTooltip;           // +0x00  FALSE
    int m_bDrawIcon;                 // +0x04  TRUE
    int m_bDrawDescription;          // +0x08  TRUE
    int m_bRoundedCorners;           // +0x0c  FALSE
    int m_bBoldLabel;                // +0x10  TRUE
    int m_bDrawSeparator;            // +0x14  TRUE
    int m_bVislManagerTheme;         // +0x18  FALSE
    int m_nMaxDescrWidth;            // +0x1c  200
    int m_nGradientAngle;            // +0x20  -1
    unsigned long m_clrFill;         // +0x24  (COLORREF)-1
    unsigned long m_clrFillGradient; // +0x28  (COLORREF)-1
    unsigned long m_clrText;         // +0x2c  (COLORREF)-1
    unsigned long m_clrBorder;       // +0x30  (COLORREF)-1
};
static_assert(sizeof(TT_Params) == 0x34, "CMFCToolTipInfo is 13 dwords in retail");
constexpr unsigned long kNoColor = static_cast<unsigned long>(-1);

// afxGlobalData (0x3c1620 mfc140u) as Track reads it: the inlined
// GetGlobalData() gate `if (+0 == 0) { Initialize(); +0 = 1; }` (0xccaf0..0xccb05),
// then m_nTextHeightHorz at +0x2a8 (the load from 0x1803c18c8 = 0x3c1620 + 0x2a8;
// core/runtime/AFX_GLOBAL_DATA.cpp pins the same offset).
constexpr std::size_t kGlobalDataInitGate = 0x000;
constexpr std::size_t kGlobalDataTextHeightHorz = 0x2a8;
int GlobalTextHeightHorz() {
    unsigned char* gd = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    int gate = 0;
    std::memcpy(&gate, gd + kGlobalDataInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(gd);
        const int one = 1;
        std::memcpy(gd + kGlobalDataInitGate, &one, sizeof one);
    }
    int h = 0;
    std::memcpy(&h, gd + kGlobalDataTextHeightHorz, sizeof h);
    return h;
}

constexpr unsigned int kCS_SAVEBITS = 0x0800;          // the retail immediate
constexpr DWORD        kWS_POPUP    = 0x80000000;      // the retail immediate
constexpr int          kDefaultMultiLineWidth = 200;   // Track: movl $0xc8,-0x29(%rbp)

} // namespace

// Retail ??0CMFCPropertyGridToolTipCtrl@@QEAA@XZ (entry RVA 0xcc500, mfc140u), fully transcribed:
//     CWnd::CWnd();                                        // 0x28a700
//     vfptr = &CMFCPropertyGridToolTipCtrl::`vftable';      // 0x1802fb6e8
//     m_strText = <nil>;   // afxStringManager (0x1803b25e8) vslot 3 GetNilString() + 0x18
//     m_rectLast = {0,0,0,0}; ::SetRectEmpty(&m_rectLast);  // import slot 0x1802c7348
//     m_nTextMargin = 10; m_hFont = NULL; m_pWndParent = NULL;
//     return this;
// DEVIATIONS: no MSVC-layout vftable exists for this class, so `this` keeps the
// vptr ??0CWnd@@ installs (recorded as g_ownVptr, file header); the nil string
// is OpenMFC's (afxstr.h), whose CStringData is ATL-layout.
// Symbol: ??0CMFCPropertyGridToolTipCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertyGridToolTipCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;   // deviation: retail has no NULL check
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    S_PGToolTip* self = L(pThis);
    new (&self->m_strText) CString();
    self->m_rectLast.left = self->m_rectLast.top = 0;
    self->m_rectLast.right = self->m_rectLast.bottom = 0;
    ::SetRectEmpty(&self->m_rectLast);
    self->m_nTextMargin = 10;
    self->m_hFont = nullptr;
    self->m_pWndParent = nullptr;
    return pThis;
}

// Retail ??1CMFCPropertyGridToolTipCtrl@@UEAA@XZ (entry RVA 0xcc5d0, mfc140u), fully transcribed:
//     vfptr = &CMFCPropertyGridToolTipCtrl::`vftable';   // 0x1802fb6e8
//     m_strText.~CString();   // inlined: CStringData at m_pszData - 0x18; lock xadd -1 on nRefs
//                             // (+0x10); when the result is <= 0, pStringMgr->Free(pData) (vslot 1)
//     CWnd::~CWnd();          // 0x28b740 (tail jump)
// DEVIATIONS: the own-vftable store becomes the g_ownVptr store (so ??1CWnd@@'s
// C++ virtual-destructor call dispatches on OpenMFC's table, not a client's);
// m_strText is released by OpenMFC's CString destructor (afxstr.h), whose
// refcount decrement is not interlocked and which leaves the locked nil string alone.
// Symbol: ??1CMFCPropertyGridToolTipCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCPropertyGridToolTipCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    L(pThis)->m_strText.~CString();
    impl___1CWnd__UEAA_XZ(pThis);
}

// Retail entry RVA 0xcc630 (mfc140u), fully transcribed:
//     m_pWndParent = pWndParent;                                        // +0x110, stored first
//     m_strClassName = AfxRegisterWndClass(CS_SAVEBITS /*0x800*/,       // 0x28c4a0
//         ::LoadCursor(NULL, IDC_ARROW /*0x7f00*/),                     // import slot 0x1802c71a8
//         (HBRUSH)(COLOR_BTNFACE + 1) /*0x10*/, NULL);
//         // (inlined operator=: SetString(psz, psz ? wcslen(psz) : 0), 0x2e30)
//     return this->CreateEx(0, m_strClassName, L"" /*0x18033d19c*/, WS_POPUP /*0x80000000*/,
//                           0, 0, 0, 0, pWndParent->GetSafeHwnd(), NULL, NULL);   // vftable slot 25 (+0xc8)
// Symbol: ?Create@CMFCPropertyGridToolTipCtrl@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__Create_CMFCPropertyGridToolTipCtrl__UEAAHPEAVCWnd___Z(void* pThis, CWnd* pWndParent) {
    S_PGToolTip* self = L(pThis);
    self->m_pWndParent = pWndParent;

    CString& strClassName =
        impl__m_strClassName_CMFCPropertyGridToolTipCtrl__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A;
    strClassName = impl__AfxRegisterWndClass__YAPEB_WIPEAUHICON____PEAUHBRUSH____0_Z(
        kCS_SAVEBITS, ::LoadCursor(nullptr, IDC_ARROW),
        reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(COLOR_BTNFACE + 1)), nullptr);

    HWND hWndParent = SafeHwnd(pWndParent);
    if (HasOwnVptr(pThis)) {
        return impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
            W(pThis), 0, strClassName.GetString(), L"", kWS_POPUP, 0, 0, 0, 0, hWndParent, nullptr, nullptr);
    }
    return reinterpret_cast<PFN_CreateEx>(SlotAt(pThis, 0xc8))(
        pThis, 0, strClassName.GetString(), L"", kWS_POPUP, 0, 0, 0, 0, hWndParent, nullptr, nullptr);
}

// Retail entry RVA 0xccd80 (mfc140u), fully transcribed:
//     m_strText.Empty();                      // 0x33b0 (CSimpleStringT::Empty)
//     ::SetRectEmpty(&m_rectLast);            // import slot 0x1802c7348
//     Hide();                                 // inlined: if (this && m_hWnd) ShowWindow(SW_HIDE) (0x2a9ad0)
// Symbol: ?Deactivate@CMFCPropertyGridToolTipCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__Deactivate_CMFCPropertyGridToolTipCtrl__QEAAXXZ(void* pThis) {
    S_PGToolTip* self = L(pThis);
    self->m_strText.Empty();
    ::SetRectEmpty(&self->m_rectLast);
    impl__Hide_CMFCPropertyGridToolTipCtrl__QEAAXXZ(pThis);
}

// Retail entry RVA 0xccd60 (mfc140u), fully transcribed:
//     if (GetSafeHwnd() != NULL) ShowWindow(SW_HIDE);   // this != NULL && m_hWnd != NULL; 0x2a9ad0
// Symbol: ?Hide@CMFCPropertyGridToolTipCtrl@@QEAAXXZ
extern "C" void MS_ABI impl__Hide_CMFCPropertyGridToolTipCtrl__QEAAXXZ(void* pThis) {
    if (SafeHwnd(pThis) != nullptr) {
        (void)impl__ShowWindow_CWnd__QEAAHH_Z(W(pThis), SW_HIDE);
    }
}

// Retail: the WM_ERASEBKGND entry of the message map 0x1802fb650 (mfc140u)
// points at 0x3a60, a body shared by every folded `return TRUE` (the map names
// it ?accDoDefaultAction@CMFCBaseAccessibleObject@@): `mov $0x1,%eax; ret`.
// The background is painted by OnPaint instead.
// Symbol: ?OnEraseBkgnd@CMFCPropertyGridToolTipCtrl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCPropertyGridToolTipCtrl__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Retail entry RVA 0xcc720 (mfc140u; the WM_PAINT entry of the message map), transcribed:
//     CPaintDC dc(this);                                                   // 0x2a3d20
//     CRect rect; ::GetClientRect(m_hWnd, &rect);                          // import 0x1802c7330
//     CMFCToolTipInfo params;                                              // inline defaults (TT_Params)
//     CMFCVisualManager::GetInstance()->GetToolTipInfo(params, (UINT)-1);  // helper 0x9774, vslot +0x5b8
//     if (params.m_clrFill == -1)
//         ::FillRect(dc.m_hDC, &rect, ::GetSysColorBrush(COLOR_INFOBK /*0x18*/));
//     else if (params.m_clrFillGradient == -1) {
//         CBrush br(params.m_clrFill);                                     // 0x2a4060
//         ::FillRect(dc.m_hDC, &rect, (HBRUSH)br.m_hObject);               // ~CBrush -> ~CGdiObject (0x1c6f0)
//     } else {
//         CDrawingManager dm(dc);                                          // inline (vftable 0x1802e7188)
//         dm.FillGradient2(rect, params.m_clrFillGradient, params.m_clrFill,
//             params.m_nGradientAngle == -1 ? 90 : params.m_nGradientAngle);   // 0x57dd0
//     }
//     COLORREF clrBorder = params.m_clrBorder == -1 ? ::GetSysColor(COLOR_INFOTEXT /*0x17*/) : params.m_clrBorder;
//     COLORREF clrText   = params.m_clrText   == -1 ? ::GetSysColor(COLOR_INFOTEXT)          : params.m_clrText;
//     dc.Draw3dRect(rect, clrBorder, clrBorder);                           // 0x2a5bc0
//     CFont* pOldFont = <SelectToolTipFont>;  ENSURE(pOldFont != NULL);    // else AfxThrowInvalidArgException (0x227720)
//     dc.SetBkMode(TRANSPARENT);                                           // 0x2a2860
//     dc.SetTextColor(clrText);                                            // 0x2a2960
//     UINT nFormat;
//     if (m_strText.FindOneOf(L"\n") != -1) {                              // wcspbrk, import 0x1802c7790
//         rect.DeflateRect(m_nTextMargin, m_nTextMargin);                  // ::InflateRect(&rect, -m, -m)
//         if (rect.Height() < m_rectLast.Height())
//             rect.top += (m_rectLast.Height() - rect.Height()) / 2;
//         nFormat = DT_WORDBREAK /*0x10*/;
//     } else {
//         rect.DeflateRect(m_nTextMargin, 0);                              // ::InflateRect(&rect, -m, 0)
//         nFormat = DT_SINGLELINE | DT_VCENTER /*0x24*/;
//     }
//     dc.DrawText(m_strText, m_strText.GetLength(), rect, nFormat);        // CDC vslot 28 (+0xe0)
// (Slot 28 of the CPaintDC vftable 0x18033b1f8 and of the CClientDC vftable
// 0x18033b300 (both mfc140u) is 0x1e6ce0: `mov 0x8(%rcx),%rcx` then a tail
// jump through import slot 0x1802c6f08 = USER32!DrawTextW -- ::DrawText on m_hDC.)
//     dc.SelectObject(pOldFont);
// DEVIATIONS: (1) the visual manager: retail's inlined GetInstance (0x9774)
// CREATES the default manager when ?m_pVisManager@CMFCVisualManager@@ is NULL
// and calls GetToolTipInfo through its vtable; as in
// featurepack/controls/CMFCToolTipCtrl.cpp, the exported static is read, the
// call is skipped while it is NULL (params keep the defaults above, i.e. the
// system tooltip colours), and the base CMFCVisualManager thunk is called, so
// a derived manager's override is not reached.  (2) The GDI calls the CDC
// members make (SelectObject, SetBkMode, SetTextColor, and ::DrawTextW on
// m_hDC for vslot 28) are issued directly on the paint DC's handles; the
// CGdiObject::FromHandle temporaries are not created.  (3) On the ENSURE
// failure the paint DC is ended before the throw instead of by unwinding.
// Symbol: ?OnPaint@CMFCPropertyGridToolTipCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCPropertyGridToolTipCtrl__IEAAXXZ(void* pThis) {
    S_PGToolTip* self = L(pThis);

    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, W(pThis));
    HDC hDC = pDC->m_hDC;

    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(self->m_hWnd, &rect);

    TT_Params params = { FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, 200,
                         -1, kNoColor, kNoColor, kNoColor, kNoColor };
    CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVM != nullptr) {   // deviation (1)
        impl__GetToolTipInfo_CMFCVisualManager__UEAAHAEAVCMFCToolTipInfo__I_Z(
            pVM, *reinterpret_cast<CMFCToolTipInfo*>(&params), static_cast<unsigned int>(-1));
    }

    if (params.m_clrFill == kNoColor) {
        ::FillRect(hDC, &rect, ::GetSysColorBrush(COLOR_INFOBK));
    } else if (params.m_clrFillGradient == kNoColor) {
        alignas(void*) unsigned char brushStorage[sizeof(CBrush)] = {};
        CBrush* pBrush = reinterpret_cast<CBrush*>(brushStorage);
        impl___0CBrush__QEAA_K_Z(pBrush, params.m_clrFill);
        ::FillRect(hDC, &rect, static_cast<HBRUSH>(pBrush->m_hObject));
        impl__DeleteObject_CGdiObject__QEAAHXZ(pBrush);
    } else {
        alignas(void*) unsigned char dmStorage[16] = {};   // CDrawingManager is vptr + CDC& (0x10 bytes)
        impl___0CDrawingManager__QEAA_AEAVCDC___Z(dmStorage, pDC);
        CRect rectFill(rect);
        impl__FillGradient2_CDrawingManager__QEAAXVCRect__KKH_Z(
            dmStorage, &rectFill, params.m_clrFillGradient, params.m_clrFill,
            params.m_nGradientAngle == -1 ? 90 : params.m_nGradientAngle);
        impl___1CDrawingManager__UEAA_XZ(dmStorage);
    }

    const COLORREF clrBorder = params.m_clrBorder == kNoColor ? ::GetSysColor(COLOR_INFOTEXT) : params.m_clrBorder;
    const COLORREF clrText = params.m_clrText == kNoColor ? ::GetSysColor(COLOR_INFOTEXT) : params.m_clrText;
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, clrBorder, clrBorder);

    HGDIOBJ hOldFont = SelectToolTipFont(pThis, pDC);
    if (hOldFont == nullptr) {
        impl___1CPaintDC__UEAA_XZ(pDC);   // deviation (3)
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (pDC->m_hDC != pDC->m_hAttribDC) ::SetBkMode(pDC->m_hDC, TRANSPARENT);
    if (pDC->m_hAttribDC != nullptr) ::SetBkMode(pDC->m_hAttribDC, TRANSPARENT);
    if (pDC->m_hDC != pDC->m_hAttribDC) ::SetTextColor(pDC->m_hDC, clrText);
    if (pDC->m_hAttribDC != nullptr) ::SetTextColor(pDC->m_hAttribDC, clrText);

    const wchar_t* pszText = self->m_strText.GetString();
    UINT nFormat;
    if (std::wcspbrk(pszText, L"\n") != nullptr) {
        ::InflateRect(&rect, -self->m_nTextMargin, -self->m_nTextMargin);
        const int cyLast = self->m_rectLast.bottom - self->m_rectLast.top;
        const int cy = rect.bottom - rect.top;
        if (cy < cyLast) rect.top += (cyLast - cy) / 2;
        nFormat = DT_WORDBREAK;
    } else {
        ::InflateRect(&rect, -self->m_nTextMargin, 0);
        nFormat = DT_SINGLELINE | DT_VCENTER;
    }
    ::DrawTextW(hDC, pszText, self->m_strText.GetLength(), &rect, nFormat);

    (void)DcSelect(pDC, hOldFont);
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail entry RVA 0xccdc0 (mfc140u; the WM_SETFONT entry of the message map), fully transcribed:
//     m_hFont = (HFONT)pFont->GetSafeHandle();          // pFont ? pFont->m_hObject (+0x8) : NULL
//     if (bRedraw) { ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd); }
//                                                       // import slots 0x1802c7128 / 0x1802c7300
// Symbol: ?OnSetFont@CMFCPropertyGridToolTipCtrl@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCPropertyGridToolTipCtrl__IEAAXPEAVCFont__H_Z(void* pThis, CFont* pFont, int bRedraw) {
    S_PGToolTip* self = L(pThis);
    self->m_hFont = pFont != nullptr ? static_cast<HFONT>(pFont->m_hObject) : nullptr;
    if (bRedraw) {
        ::InvalidateRect(self->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(self->m_hWnd);
    }
}

// Retail entry RVA 0xcce10 (mfc140u; vftable slot 69), transcribed:
//     if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSEFIRST + 0xe) {   // 0x200..0x20e
//         if (pMsg->message != WM_MOUSEMOVE) Hide();       // inlined; 0x2a9ad0
//         CPoint pt(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));   // movzwl: ZERO-extended
//         ::MapWindowPoints(m_hWnd, m_pWndParent->GetSafeHwnd(), &pt, 1);   // import 0x1802c7228
//         ::SendMessage(m_pWndParent->m_hWnd, pMsg->message, pMsg->wParam,
//                       MAKELPARAM(pt.x, pt.y));           // import 0x1802c7120 (SendMessageW)
//         return TRUE;
//     }
//     return CWnd::PreTranslateMessage(pMsg);              // direct call, 0x28bc00
// DEVIATION: retail reads m_pWndParent->m_hWnd for the SendMessage without a
// NULL test (it faults when Create was given no parent); here the forward is
// skipped when m_pWndParent is NULL, and TRUE is still returned.
// Symbol: ?PreTranslateMessage@CMFCPropertyGridToolTipCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCPropertyGridToolTipCtrl__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    S_PGToolTip* self = L(pThis);
    const UINT message = pMsg->message;
    if (message - WM_MOUSEFIRST > 0xe) {
        return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(W(pThis), pMsg);
    }
    if (message != WM_MOUSEMOVE) {
        impl__Hide_CMFCPropertyGridToolTipCtrl__QEAAXXZ(pThis);
    }
    POINT pt;
    pt.x = LOWORD(pMsg->lParam);
    pt.y = HIWORD(pMsg->lParam);
    ::MapWindowPoints(self->m_hWnd, SafeHwnd(self->m_pWndParent), &pt, 1);
    if (self->m_pWndParent != nullptr) {   // deviation (see above)
        ::SendMessage(SafeHwnd(self->m_pWndParent), message, pMsg->wParam,
                      MAKELPARAM(static_cast<WORD>(pt.x), static_cast<WORD>(pt.y)));
    }
    return TRUE;
}

// Retail entry RVA 0xcc9f0 (mfc140u), transcribed.  `rect` is the CRect passed
// by value, i.e. (MSVC x64, 16 bytes) a pointer to the caller's temporary,
// which retail modifies in place:
//     if (GetSafeHwnd() == NULL) return;                                // this == NULL || m_hWnd == NULL
//     if (::EqualRect(&m_rectLast, &rect) && m_strText == strText) return;   // imports 0x1802c72c0, wcscmp 0x1802c7770
//     m_rectLast = rect;                                                // 16-byte copy to +0xf0
//     m_strText = strText;                                              // 0xde30
//     CClientDC dc(this);                                               // 0x2a3b20
//     CFont* pOldFont = <SelectToolTipFont>;  ENSURE(pOldFont != NULL);  // else AfxThrowInvalidArgException
//     int nTextHeight, nTextWidth;
//     if (m_strText.FindOneOf(L"\n") != -1) {                           // wcspbrk
//         CRect rectText(0, 0, 200, GetGlobalData()->GetTextHeight());  // afxGlobalData +0x2a8
//         nTextHeight = dc.DrawText(m_strText, m_strText.GetLength(), rectText,
//                                   DT_CALCRECT | DT_WORDBREAK /*0x410*/) + 2 * m_nTextMargin;   // CDC vslot 28
//         nTextWidth  = rectText.Width() + 2 * m_nTextMargin;
//     } else {
//         nTextHeight = rect.Height();
//         nTextWidth  = dc.GetTextExtent(m_strText).cx + 2 * m_nTextMargin;   // GetTextExtentPoint32W on m_hAttribDC
//     }
//     dc.SelectObject(pOldFont);
//     if (m_pWndParent->GetExStyle() & WS_EX_LAYOUTRTL) rect.left = rect.right - nTextWidth;   // 0x2a96c0; bt $0x16
//     else                                              rect.right = rect.left + nTextWidth;
//     rect.bottom = rect.top + nTextHeight;
//     if (nTextHeight < m_rectLast.Height()) { rect.top = m_rectLast.top; rect.bottom = m_rectLast.bottom; }
//     CRect rectScreen; MONITORINFO mi; mi.cbSize = sizeof(MONITORINFO);
//     if (::GetMonitorInfo(::MonitorFromPoint(rect.TopLeft(), MONITOR_DEFAULTTONEAREST), &mi))
//         ::CopyRect(&rectScreen, &mi.rcWork);
//     else ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
//     if (rect.Width() > rectScreen.Width())      { rect.left = rectScreen.left; rect.right = rectScreen.right; }
//     else if (rect.right > rectScreen.right)     { rect.right = rectScreen.right; rect.left = rectScreen.right - nTextWidth; }
//     else if (rect.left < rectScreen.left)       { rect.left = rectScreen.left; rect.right = rectScreen.left + nTextWidth; }
//     if (rect.Height() > rectScreen.Height())    { rect.top = rectScreen.top; rect.bottom = rectScreen.bottom; }
//     else if (rect.bottom > rectScreen.bottom)   { rect.bottom = rectScreen.bottom; rect.top = rectScreen.bottom - nTextHeight; }
//     else if (rect.top < rectScreen.top)         { rect.top = rectScreen.top; rect.bottom = rect.bottom + nTextHeight; }
//     // ^ NOT symmetric with the horizontal case: retail computes the new bottom
//     //   as `lea (%r11,%r14,1),%edx` at 0xcccb1, r11 = the current rect.bottom
//     //   (loaded at 0xccc74), r14 = nTextHeight -- it does not start from
//     //   rectScreen.top (that is r9, which goes to rect.top).  Kept as retail does it.
//     SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(),
//                  SWP_NOACTIVATE | SWP_NOOWNERZORDER /*0x210*/);        // 0x2a9a60; &wndTop = 0x1803c3370
//     ShowWindow(SW_SHOWNOACTIVATE);                                     // 0x2a9ad0
//     ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd);
//     ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));           // see below
//     // ~CClientDC (0x2a3be0)
// The last line: retail calls AfxGetModuleState (0x133930), discards the
// result, then ::SetCursor(::LoadCursor(NULL, IDC_ARROW)) -- the shape an
// inlined AfxGetApp()->LoadStandardCursor(IDC_ARROW) compiles to.  The
// AfxGetModuleState call has no effect on the result and is not reproduced.
// DEVIATIONS: (1) the CDC member calls are issued directly on the client DC's
// handles, as in OnPaint.  (2) Retail's CString comparison ENSUREs strText's
// buffer is non-NULL (AtlThrow(E_FAIL)); a CString's buffer is never NULL, so
// that check is not reproduced.  (3) Retail reads m_pWndParent->GetExStyle()
// without a NULL test (it faults when Create was given no parent); a NULL
// parent is treated here as having no WS_EX_LAYOUTRTL.  (4) On the ENSURE
// failure the client DC is released before the throw instead of by unwinding.
// Symbol: ?Track@CMFCPropertyGridToolTipCtrl@@QEAAXVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__Track_CMFCPropertyGridToolTipCtrl__QEAAXVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, RECT* pRect, const CString* pStrText) {
    if (SafeHwnd(pThis) == nullptr) return;
    S_PGToolTip* self = L(pThis);
    RECT& rect = *pRect;

    if (::EqualRect(&self->m_rectLast, &rect) &&
        std::wcscmp(self->m_strText.GetString(), pStrText->GetString()) == 0) {
        return;
    }
    self->m_rectLast = rect;
    self->m_strText = *pStrText;

    alignas(void*) unsigned char dcStorage[sizeof(CClientDC)] = {};
    CClientDC* pDC = reinterpret_cast<CClientDC*>(dcStorage);
    impl___0CClientDC__QEAA_PEAVCWnd___Z(pDC, W(pThis));

    HGDIOBJ hOldFont = SelectToolTipFont(pThis, pDC);
    if (hOldFont == nullptr) {
        impl___1CClientDC__UEAA_XZ(pDC);   // deviation (4)
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }

    const wchar_t* pszText = self->m_strText.GetString();
    const int nLength = self->m_strText.GetLength();
    int nTextHeight;
    int nTextWidth;
    if (std::wcspbrk(pszText, L"\n") != nullptr) {
        RECT rectText = { 0, 0, kDefaultMultiLineWidth, GlobalTextHeightHorz() };
        const int cyText = ::DrawTextW(pDC->m_hDC, pszText, nLength, &rectText, DT_CALCRECT | DT_WORDBREAK);
        nTextHeight = cyText + 2 * self->m_nTextMargin;
        nTextWidth = rectText.right - rectText.left + 2 * self->m_nTextMargin;
    } else {
        nTextHeight = rect.bottom - rect.top;
        SIZE size = { 0, 0 };
        ::GetTextExtentPoint32W(pDC->m_hAttribDC, pszText, nLength, &size);
        nTextWidth = size.cx + 2 * self->m_nTextMargin;
    }
    (void)DcSelect(pDC, hOldFont);

    const unsigned long dwParentExStyle =
        self->m_pWndParent != nullptr   // deviation (3)
            ? impl__GetExStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(self->m_pWndParent))
            : 0;
    if (dwParentExStyle & WS_EX_LAYOUTRTL) {
        rect.left = rect.right - nTextWidth;
    } else {
        rect.right = rect.left + nTextWidth;
    }
    rect.bottom = rect.top + nTextHeight;
    if (nTextHeight < self->m_rectLast.bottom - self->m_rectLast.top) {
        rect.top = self->m_rectLast.top;
        rect.bottom = self->m_rectLast.bottom;
    }

    RECT rectScreen = { 0, 0, 0, 0 };
    MONITORINFO mi;
    std::memset(&mi, 0, sizeof mi);
    mi.cbSize = sizeof(MONITORINFO);
    POINT ptTopLeft = { rect.left, rect.top };
    if (::GetMonitorInfoW(::MonitorFromPoint(ptTopLeft, MONITOR_DEFAULTTONEAREST), &mi)) {
        ::CopyRect(&rectScreen, &mi.rcWork);
    } else {
        ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &rectScreen, 0);
    }

    if (rect.right - rect.left > rectScreen.right - rectScreen.left) {
        rect.left = rectScreen.left;
        rect.right = rectScreen.right;
    } else if (rect.right > rectScreen.right) {
        rect.right = rectScreen.right;
        rect.left = rectScreen.right - nTextWidth;
    } else if (rect.left < rectScreen.left) {
        rect.left = rectScreen.left;
        rect.right = rectScreen.left + nTextWidth;
    }
    if (rect.bottom - rect.top > rectScreen.bottom - rectScreen.top) {
        rect.top = rectScreen.top;
        rect.bottom = rectScreen.bottom;
    } else if (rect.bottom > rectScreen.bottom) {
        rect.bottom = rectScreen.bottom;
        rect.top = rectScreen.bottom - nTextHeight;
    } else if (rect.top < rectScreen.top) {
        rect.top = rectScreen.top;
        rect.bottom = rect.bottom + nTextHeight;   // retail 0xcccb1: old bottom + height (see above)
    }

    (void)impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
        W(pThis), reinterpret_cast<const CWnd*>(impl__wndTop_CWnd__2V1_B),
        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
        SWP_NOACTIVATE | SWP_NOOWNERZORDER);
    (void)impl__ShowWindow_CWnd__QEAAHH_Z(W(pThis), SW_SHOWNOACTIVATE);
    ::InvalidateRect(self->m_hWnd, nullptr, TRUE);
    ::UpdateWindow(self->m_hWnd);
    ::SetCursor(::LoadCursor(nullptr, IDC_ARROW));

    impl___1CClientDC__UEAA_XZ(pDC);
}

// Data export: the protected static CString (mfc140u VA 0x1803c2470, the
// address Create stores through).  In retail a dynamic initialiser stores
// GetNilString() + 0x18 into it (the store at 0x180001760, mfc140u) and
// registers the atexit destructor 0x1802c53a0 (the inlined refcount release
// on the same address); Create assigns it the AfxRegisterWndClass result
// before every CreateEx.  Defined here as an OpenMFC CString (afxstr.h: one
// ATL-layout data pointer, the same 8 bytes as the retail CStringT), so it
// likewise starts as the nil string and is destroyed at DLL unload.
// Symbol: ?m_strClassName@CMFCPropertyGridToolTipCtrl@@1V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" CString impl__m_strClassName_CMFCPropertyGridToolTipCtrl__1V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A{};
