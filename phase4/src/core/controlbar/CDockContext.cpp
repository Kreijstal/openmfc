// CDockContext — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Method: every body below was decoded from the retail mfc140u.dll export
// (disas.py --u), the way core/ole/COleControl.cpp and core/controlbar/
// CDockBar.cpp do; each function names the RVA (mfc140u) it was read from.
// ??0, ??1, CanDock, CancelLoop, DrawFocusRect, EndDrag, EndResize,
// GetDockBar, InitLoop and Move are in the wf2 RVA symbol map; the other
// seven were resolved through the mfc140u export table (ordinal -> RVA,
// ordrva.py):
//   OnKey 0x21ed80        StartDrag 0x21e8d0     StartResize 0x21ef00
//   Stretch 0x21f090      ToggleDocking 0x21f2c0 Track 0x21f990
//   UpdateState 0x21f770
// Cross-checks: StartDrag / StartResize / ToggleDocking are slots 0 / 1 / 2
// of the retail vftable (below); Track and Stretch are direct call targets
// of StartDrag / StartResize; UpdateState is called from OnKey and Track;
// OnKey's body is the same code Track inlines for WM_KEYUP / WM_KEYDOWN.
//
// CDockContext is the classic control-bar drag/dock tracker.  It is NOT a
// CObject: the shipping afxpriv.h (read on this host, line 493) declares it
// with no base class, four virtuals (StartDrag, StartResize, ToggleDocking and
// the destructor) and the member list pinned below.  OpenMFC has no public
// declaration of the class, so the full layout lives only in this file
// (CDockBar.cpp separately hard-codes the +0xac m_rectMRUDockPos offset).
//
// Object layout, read out of the retail constructor
// ??0CDockContext@@QEAA@PEAVCControlBar@@@Z, RVA 0x21e780 (mfc140u), and the
// bodies that touch each member, in afxpriv.h declaration order:
//   +0x00 vfptr                 ctor `mov %rax,(%rcx)` (vftable 0x180328f98, mfc140u)
//   +0x08 CPoint m_ptLast       StartDrag `mov %rbx,0x8(%rsi)` (= pt)
//   +0x10 CRect  m_rectLast     InitLoop ::SetRectEmpty(0x10(%rdi)); DrawFocusRect stores it
//   +0x20 CSize  m_sizeLast     InitLoop `movq $0x0,0x20(%rdi)`
//   +0x28 BOOL   m_bDitherLast  InitLoop `movl $0x0,0x28(%rdi)`
//   +0x2c CRect  m_rectDragHorz        } StartDrag `lea 0x2c/0x3c/0x4c/0x5c(%rsi)`;
//   +0x3c CRect  m_rectDragVert        } the ctor zeroes 0x2c..0x6b with eight
//   +0x4c CRect  m_rectFrameDragHorz   } qword stores
//   +0x5c CRect  m_rectFrameDragVert   }
//   +0x70 CControlBar* m_pBar   ctor `mov %rdx,0x70(%rcx)`
//   +0x78 CFrameWnd* m_pDockSite ctor copies pBar->m_pDockSite (+0x130) here
//   +0x80 DWORD m_dwDockStyle   InitLoop copies pBar->m_dwDockStyle (+0x128)
//   +0x84 DWORD m_dwOverDockStyle  StartDrag/Move/UpdateState store CanDock() here
//   +0x88 DWORD m_dwStyle       InitLoop stores pBar->m_dwStyle & CBRS_ALIGN_ANY
//   +0x8c BOOL  m_bFlip         OnKey VK_SHIFT (0x10) -> UpdateState(0x8c(%rcx))
//   +0x90 BOOL  m_bForceFrame   OnKey VK_CONTROL (0x11) -> UpdateState(0x90(%rcx))
//   +0x98 CDC*  m_pDC           InitLoop stores CDC::FromHandle(::GetDCEx(..)) here
//   +0xa0 BOOL  m_bDragging     StartDrag `movl $0x1,0xa0(%rcx)`, StartResize 0
//   +0xa4 int   m_nHitTest      StartResize `mov %edi,0xa4(%rsi)` (= nHitTest)
//   +0xa8 UINT  m_uMRUDockID    ctor `mov %r8d,0xa8(%rcx)`, EndDrag stores the ID
//   +0xac CRect m_rectMRUDockPos  EndDrag `lea 0xac(%rbx),%rdx` -> ScreenToClient
//                               (also written by CDockBar::CalcFixedLayout, see
//                               kOffDockContextRectMRUDockPos in CDockBar.cpp)
//   +0xbc DWORD m_dwMRUFloatStyle ctor `mov %eax,0xbc(%rcx)`
//   +0xc0 CPoint m_ptMRUFloatPos  ctor `movl $0x80000000,0xc0(%rcx)` (CW_USEDEFAULT)
//   sizeof 0xc8 -- the size the deleting destructor (vftable slot 3, 0x21e830)
//   passes to operator delete (`mov $0xc8,%edx`).
//
// Virtual functions.  The retail vftable at 0x180328f98 (mfc140u) holds four
// slots: 0 StartDrag 0x21e8d0, 1 StartResize 0x21ef00, 2 ToggleDocking
// 0x21f2c0, 3 the scalar deleting destructor 0x21e830.  The constructor and
// destructor here store an MSVC-layout vtable authored in this file
// (g_DockContextVtbl) with the same four slots, so a client's
// `m_pDockContext->StartDrag(pt)` / `delete m_pDockContext` dispatch to the
// bodies below.  KNOWN GAP: the retail vftable is preceded by an RTTI
// Complete Object Locator (vftable[-1] -> 0x365e30, mfc140u, naming
// `.?AVCDockContext@@`); g_DockContextVtbl has none, so a client
// typeid / dynamic_cast on a CDockContext built here would read garbage.
// No body in this file dispatches through the object's own
// vtable (retail does not either: StartDrag calls Track, Move, CanDock etc.
// directly).
//
// Virtual dispatch on OTHER objects.  Retail calls m_pBar->CalcDynamicLayout
// through CControlBar vslot 92 (`call *0x2e0(%rax)`) and m_pDockSite->
// RecalcLayout through CFrameWnd vslot 96 (`call *0x300(%rax)`).  OpenMFC's
// CControlBar / CFrameWnd objects carry a mingw vtable whose numbering is not
// MSVC's, so these go through impl__ thunks instead: CalcDynamicLayout is
// devirtualised on the runtime class exactly as core/controlbar/CDockBar.cpp
// and CControlBar.cpp do (CToolBar's thunk, else CControlBar's; a client-
// derived override is not honoured), and RecalcLayout goes to the CFrameWnd
// thunk, which dispatches C++-virtually to the repo's overrides.
//
// Inlined MFC helpers.  Retail inlines several small CWnd/CFrameWnd members;
// the bodies below expand them the way the disassembly does and name the IAT
// import each one resolves to (iatu.py, mfc140u).

#include "detail/ManualSmallStubImplementationsSupport.h"
#include <cstddef>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Sibling / base thunks, declared with the signature their mangled name
// describes; every definition was located with grep before use.
// ---------------------------------------------------------------------------
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                     // core/window/CWnd.cpp
extern "C" CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);                          // core/gdi/CDC.cpp
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject);          // core/gdi/CGdiObject.cpp
extern "C" CBrush* MS_ABI impl__GetHalftoneBrush_CDC__SAPEAVCBrush__XZ();                             // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size, const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast);                                                              // core/gdi/CDC.cpp
extern "C" unsigned long MS_ABI impl__CanDock_CFrameWnd__QEAAKVCRect__KPEAPEAVCDockBar___Z(
    CFrameWnd* pThis, CRect rect, unsigned long dwDockStyle, CDockBar** ppDockBar);                   // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__FloatControlBar_CFrameWnd__QEAAXPEAVCControlBar__VCPoint__K_Z(
    CFrameWnd* pThis, CControlBar* pBar, CPoint point, unsigned long dwStyle);                        // core/frame/Thunks.cpp
extern "C" CControlBar* MS_ABI impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(CFrameWnd* pThis, unsigned int nID); // core/frame/Thunks.cpp
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);        // core/frame/Thunks.cpp
extern "C" int MS_ABI impl__IsFloating_CControlBar__QEBAHXZ(const CControlBar* pThis);                 // core/controlbar/CControlBar.cpp
extern "C" void* MS_ABI impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(void* pRet, CControlBar* pThis, int nLength, DWORD dwMode); // CControlBar.cpp
extern "C" void MS_ABI impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(
    CSize* pRet, CToolBar* pThis, int nLength, unsigned long dwMode);                                 // core/controlbar/CToolBar.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ();              // core/controlbar/CToolBar.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                               // core/runtime/CObject.cpp
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect); // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect); // core/window/Thunks.cpp
extern "C" void MS_ABI impl__DockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(void* pThis, CControlBar* pBar, const RECT* lpRect);     // CDockBar.cpp
extern "C" void MS_ABI impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(void* pThis, CControlBar* pBar, const RECT* lpRect);   // CDockBar.cpp
extern "C" int MS_ABI impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(void* pThis, CControlBar* pBar, int nPosExclude, int nAddPlaceHolder); // CDockBar.cpp
extern "C" int MS_ABI impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(void* pThis, CControlBar* pBar, int nPosExclude);                           // CDockBar.cpp
extern "C" void MS_ABI impl__AfxPostQuitMessage__YAXH_Z(int nExitCode);                               // core/collections/Globals.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp

// Thunks defined further down in this file that earlier bodies (and the
// vtable) refer to.  CPoint by value is an 8-byte aggregate passed in a
// register under the MS x64 ABI, so it is modelled as `long long`.
extern "C" void MS_ABI impl___1CDockContext__UEAA_XZ(void* pThis);
extern "C" unsigned long MS_ABI impl__CanDock_CDockContext__QEAAKXZ(void* pThis);
extern "C" void MS_ABI impl__CancelLoop_CDockContext__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__DrawFocusRect_CDockContext__QEAAXH_Z(void* pThis, int bRemoveRect);
extern "C" void MS_ABI impl__EndDrag_CDockContext__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__EndResize_CDockContext__QEAAXXZ(void* pThis);
extern "C" CDockBar* MS_ABI impl__GetDockBar_CDockContext__QEAAPEAVCDockBar__K_Z(void* pThis, unsigned long dwOverDockStyle);
extern "C" void MS_ABI impl__InitLoop_CDockContext__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__Move_CDockContext__QEAAXVCPoint___Z(void* pThis, long long pt);
extern "C" void MS_ABI impl__StartDrag_CDockContext__UEAAXVCPoint___Z(void* pThis, long long pt);
extern "C" void MS_ABI impl__StartResize_CDockContext__UEAAXHVCPoint___Z(void* pThis, int nHitTest, long long pt);
extern "C" void MS_ABI impl__Stretch_CDockContext__QEAAXVCPoint___Z(void* pThis, long long pt);
extern "C" void MS_ABI impl__ToggleDocking_CDockContext__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__Track_CDockContext__QEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateState_CDockContext__QEAAXPEAHH_Z(void* pThis, int* pFlag, int bNewValue);

namespace {

// ---------------------------------------------------------------------------
// Layout view (see the file header for where every offset comes from).
// ---------------------------------------------------------------------------
struct DockContext {
    const void*  vfptr;                // 0x00
    POINT        m_ptLast;             // 0x08
    RECT         m_rectLast;           // 0x10
    SIZE         m_sizeLast;           // 0x20
    BOOL         m_bDitherLast;        // 0x28
    RECT         m_rectDragHorz;       // 0x2c
    RECT         m_rectDragVert;       // 0x3c
    RECT         m_rectFrameDragHorz;  // 0x4c
    RECT         m_rectFrameDragVert;  // 0x5c
    CControlBar* m_pBar;               // 0x70
    CFrameWnd*   m_pDockSite;          // 0x78
    DWORD        m_dwDockStyle;        // 0x80
    DWORD        m_dwOverDockStyle;    // 0x84
    DWORD        m_dwStyle;            // 0x88
    BOOL         m_bFlip;              // 0x8c
    BOOL         m_bForceFrame;        // 0x90
    CDC*         m_pDC;                // 0x98
    BOOL         m_bDragging;          // 0xa0
    int          m_nHitTest;           // 0xa4
    UINT         m_uMRUDockID;         // 0xa8
    RECT         m_rectMRUDockPos;     // 0xac
    DWORD        m_dwMRUFloatStyle;    // 0xbc
    POINT        m_ptMRUFloatPos;      // 0xc0
};
static_assert(offsetof(DockContext, m_ptLast) == 0x08, "CDockContext::m_ptLast");
static_assert(offsetof(DockContext, m_rectLast) == 0x10, "CDockContext::m_rectLast");
static_assert(offsetof(DockContext, m_sizeLast) == 0x20, "CDockContext::m_sizeLast");
static_assert(offsetof(DockContext, m_bDitherLast) == 0x28, "CDockContext::m_bDitherLast");
static_assert(offsetof(DockContext, m_rectDragHorz) == 0x2c, "CDockContext::m_rectDragHorz");
static_assert(offsetof(DockContext, m_rectDragVert) == 0x3c, "CDockContext::m_rectDragVert");
static_assert(offsetof(DockContext, m_rectFrameDragHorz) == 0x4c, "CDockContext::m_rectFrameDragHorz");
static_assert(offsetof(DockContext, m_rectFrameDragVert) == 0x5c, "CDockContext::m_rectFrameDragVert");
static_assert(offsetof(DockContext, m_pBar) == 0x70, "CDockContext::m_pBar");
static_assert(offsetof(DockContext, m_pDockSite) == 0x78, "CDockContext::m_pDockSite");
static_assert(offsetof(DockContext, m_dwDockStyle) == 0x80, "CDockContext::m_dwDockStyle");
static_assert(offsetof(DockContext, m_dwOverDockStyle) == 0x84, "CDockContext::m_dwOverDockStyle");
static_assert(offsetof(DockContext, m_dwStyle) == 0x88, "CDockContext::m_dwStyle");
static_assert(offsetof(DockContext, m_bFlip) == 0x8c, "CDockContext::m_bFlip");
static_assert(offsetof(DockContext, m_bForceFrame) == 0x90, "CDockContext::m_bForceFrame");
static_assert(offsetof(DockContext, m_pDC) == 0x98, "CDockContext::m_pDC");
static_assert(offsetof(DockContext, m_bDragging) == 0xa0, "CDockContext::m_bDragging");
static_assert(offsetof(DockContext, m_nHitTest) == 0xa4, "CDockContext::m_nHitTest");
static_assert(offsetof(DockContext, m_uMRUDockID) == 0xa8, "CDockContext::m_uMRUDockID");
static_assert(offsetof(DockContext, m_rectMRUDockPos) == 0xac, "CDockContext::m_rectMRUDockPos");
static_assert(offsetof(DockContext, m_dwMRUFloatStyle) == 0xbc, "CDockContext::m_dwMRUFloatStyle");
static_assert(offsetof(DockContext, m_ptMRUFloatPos) == 0xc0, "CDockContext::m_ptMRUFloatPos");
static_assert(sizeof(DockContext) == 0xc8, "sizeof(CDockContext) == 0xc8 (operator delete size in ??_G 0x21e830)");

// CControlBar / CWnd / CDC members retail touches, at the offsets the
// disassembly uses.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CControlBar, m_dwStyle) == 0x124, "CControlBar::m_dwStyle");
static_assert(offsetof(CControlBar, m_dwDockStyle) == 0x128, "CControlBar::m_dwDockStyle");
static_assert(offsetof(CControlBar, m_pDockSite) == 0x130, "CControlBar::m_pDockSite");
static_assert(offsetof(CControlBar, m_pDockBar) == 0x138, "CControlBar::m_pDockBar");
static_assert(offsetof(CDC, m_hDC) == 0x8, "CDC::m_hDC");

inline DockContext& Ctx(void* pThis) { return *static_cast<DockContext*>(pThis); }

// Retail CBRS_* / LM_* / AFX_IDW_* values as the disassembly tests them (the
// header's CBRS_* macros in include/openmfc/afxole.h do NOT match retail's
// bits; see CControlBar.cpp).
constexpr DWORD kCBRS_SIZE_FIXED    = 0x0002;
constexpr DWORD kCBRS_SIZE_DYNAMIC  = 0x0004;
constexpr DWORD kCBRS_FLOAT_MULTI   = 0x0040;
constexpr DWORD kCBRS_ALIGN_LEFT    = 0x1000;
constexpr DWORD kCBRS_ALIGN_TOP     = 0x2000;
constexpr DWORD kCBRS_ALIGN_ANY     = 0xF000;
constexpr DWORD kCBRS_ORIENT_HORZ   = 0xA000;   // CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM
constexpr DWORD kCBRS_ORIENT_VERT   = 0x5000;   // CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT
constexpr DWORD kLM_HORZ     = 0x02;
constexpr DWORD kLM_MRUWIDTH = 0x04;
constexpr DWORD kLM_HORZDOCK = 0x08;
constexpr DWORD kLM_VERTDOCK = 0x10;
constexpr DWORD kLM_LENGTHY  = 0x20;
constexpr DWORD kLM_COMMIT   = 0x40;
constexpr UINT  kAFX_IDW_DOCKBAR_TOP = 0xE81B;   // `lea -0xe81b(%rax)`; `cmp $0x3` in EndDrag
// wingdi.h LAYOUT_RTL / LAYOUT_LTR (afxpriv.h supplies LAYOUT_LTR itself; the
// mingw headers lack it).  Retail tests `test $0x1,%al` on GetLayout and
// passes `xor %edx,%edx` to SetLayout.
constexpr DWORD kLAYOUT_RTL = 0x1;
constexpr DWORD kLAYOUT_LTR = 0x0;

// CMiniFrameWnd::CalcBorders(lpRect) with its default dwStyle (afxwin.h:
// WS_THICKFRAME | WS_CAPTION), inlined by retail as
// ::AdjustWindowRectEx(lpRect, 0xc40000, FALSE, 0x188).  The exported
// CalcBorders (RVA 0x2a9220, mfc140u) is exactly that tail call, with 0x188
// (WS_EX_PALETTEWINDOW) hard-wired in place of dwExStyle.
constexpr DWORD kMiniFrameStyle   = 0x00C40000;
constexpr DWORD kMiniFrameExStyle = 0x00000188;

// AUX_DATA::cxBorder2 / cyBorder2 (afxData + 0x10 / + 0x14 = 0x3c32d0 /
// 0x3c32d4, mfc140u).  The AUX_DATA static initializer at RVA 0x2270
// (mfc140u) stores 2 to both (`mov $0x2,%eax` at 0x22c3, stored at
// 0x22c8/0x22ce); OpenMFC's exported afxData is a zero blob, so the retail
// values are folded in as literals, as CDockBar.cpp does.
constexpr int kCxBorder2 = 2;
constexpr int kCyBorder2 = 2;

// CFrameWnd::dwDockBarMap, the {dock bar ID, CBRS_ style} table retail's
// inlined CFrameWnd::ReDockControlBar walks in ToggleDocking.  Read from the
// retail image at 0x18033ebe0 (mfc140u, .rdata; the `lea 0x11f87c(%rip)` at
// 0x21f35d): {0xe81b,0x2800} {0xe81e,0x8200} {0xe81c,0x1400} {0xe81d,0x4100}
// -- AFX_IDW_DOCKBAR_TOP/BOTTOM/LEFT/RIGHT with CBRS_TOP/BOTTOM/LEFT/RIGHT.
constexpr DWORD kDockBarMap[4][2] = {
    { 0xE81B, 0x2800 },
    { 0xE81E, 0x8200 },
    { 0xE81C, 0x1400 },
    { 0xE81D, 0x4100 },
};

inline POINT UnpackPoint(long long pt) {
    POINT p;
    p.x = static_cast<LONG>(static_cast<unsigned long long>(pt) & 0xffffffffu);
    p.y = static_cast<LONG>(static_cast<unsigned long long>(pt) >> 32);
    return p;
}

inline int KindOf(const CControlBar* pBar, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(pBar), pClass);
}

// vslot 92 (CalcDynamicLayout) on m_pBar, devirtualised the way CDockBar.cpp
// and CControlBar.cpp do it: CToolBar's override has a correctly typed thunk,
// everything else gets CControlBar's.
inline SIZE CallCalcDynamicLayout(CControlBar* pBar, int nLength, DWORD dwMode) {
    CSize size(0, 0);
    if (KindOf(pBar, impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ())) {
        impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(&size, static_cast<CToolBar*>(pBar), nLength, dwMode);
    } else {
        impl__CalcDynamicLayout_CControlBar__UEAA_AVCSize__HK_Z(&size, pBar, nLength, dwMode);
    }
    SIZE s = { size.cx, size.cy };
    return s;
}

// CFrameWnd::CanDock(CRect, DWORD, CDockBar**) -- 0x2a1990 (mfc140u).
inline DWORD FrameCanDock(CFrameWnd* pFrame, const RECT& rect, DWORD dwDockStyle, CDockBar** ppDockBar) {
    return impl__CanDock_CFrameWnd__QEAAKVCRect__KPEAPEAVCDockBar___Z(pFrame, CRect(rect), dwDockStyle, ppDockBar);
}

inline CWnd* AsWnd(CDockBar* pDockBar) {
    // CDockBar is single-inheritance from CControlBar (CWnd at offset 0); the
    // class is only forward-declared in OpenMFC, hence the reinterpret_cast.
    return static_cast<CWnd*>(reinterpret_cast<CControlBar*>(pDockBar));
}

inline void SetRectXYWH(RECT& r, LONG x, LONG y, LONG cx, LONG cy) {
    r.left = x; r.top = y; r.right = x + cx; r.bottom = y + cy;
}

// _AfxAdjustRectangle -- the file-static helper retail calls from StartDrag,
// RVA 0x21e73c (mfc140u):
//     int dx = (pt.x < rect.left) ? pt.x - rect.left : (pt.x > rect.right)  ? pt.x - rect.right  : 0;
//     int dy = (pt.y < rect.top)  ? pt.y - rect.top  : (pt.y > rect.bottom) ? pt.y - rect.bottom : 0;
//     ::OffsetRect(&rect, dx, dy);                     // IAT OffsetRect (tail jump)
inline void AdjustRectangle(RECT& rect, POINT pt) {
    int dx = 0;
    if (pt.x < rect.left) dx = pt.x - rect.left;
    else if (pt.x > rect.right) dx = pt.x - rect.right;
    int dy = 0;
    if (pt.y < rect.top) dy = pt.y - rect.top;
    else if (pt.y > rect.bottom) dy = pt.y - rect.bottom;
    ::OffsetRect(&rect, dx, dy);
}

// The scalar deleting destructor (retail vftable slot 3, 0x21e830, mfc140u):
//     ~CDockContext();                               // 0x21e870
//     if (flags & 1) operator delete(this, 0xc8);    // 0x2b77b0 -> jmp 0x27c0 -> IAT free
// The retail sized delete is a jump into the CRT's free; OpenMFC's exported
// operator new (detail/MemcoreSupport.cpp) is std::malloc, so std::free here
// pairs with a client `new CDockContext(...)` that went through the DLL's
// operator new.
void* MS_ABI DockContextDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CDockContext__UEAA_XZ(pThis);
    if (flags & 1u) std::free(pThis);
    return pThis;
}

// MSVC-layout vtable, same slot order as the retail vftable 0x180328f98
// (mfc140u; vtdump_u.py): 0 StartDrag 0x21e8d0, 1 StartResize 0x21ef00,
// 2 ToggleDocking 0x21f2c0, 3 scalar deleting destructor 0x21e830.
void* const g_DockContextVtbl[4] = {
    reinterpret_cast<void*>(&impl__StartDrag_CDockContext__UEAAXVCPoint___Z),
    reinterpret_cast<void*>(&impl__StartResize_CDockContext__UEAAXHVCPoint___Z),
    reinterpret_cast<void*>(&impl__ToggleDocking_CDockContext__UEAAXXZ),
    reinterpret_cast<void*>(&DockContextDeletingDtor),
};

} // namespace

// Transcribed from retail RVA 0x21e780 (mfc140u):
//     vfptr = CDockContext vftable;
//     m_pBar = pBar;
//     m_ptLast = m_rectLast = m_sizeLast = 0;                         // qwords 0x08..0x27
//     m_rectDragHorz = m_rectDragVert = m_rectFrameDragHorz/Vert = 0; // qwords 0x2c..0x6b
//     m_rectMRUDockPos = 0;  m_ptMRUFloatPos = 0;                     // 0xac, 0xb4, 0xc0
//     m_pDockSite = pBar->m_pDockSite;                                // +0x130
//     m_uMRUDockID = 0;
//     if (pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
//         m_dwMRUFloatStyle = pBar->m_dwStyle & (CBRS_ALIGN_TOP | CBRS_SIZE_DYNAMIC);
//     else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ)
//         m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (pBar->m_dwStyle & CBRS_FLOAT_MULTI);
//     else
//         m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (pBar->m_dwStyle & CBRS_FLOAT_MULTI);
//     m_ptMRUFloatPos.x = CW_USEDEFAULT;                              // 0x80000000
//     m_pDC = NULL;
// Retail leaves m_bDitherLast, m_dwDockStyle, m_dwOverDockStyle, m_dwStyle,
// m_bFlip, m_bForceFrame, m_bDragging and m_nHitTest unwritten (InitLoop /
// StartDrag / StartResize set them before use); so does this body.  The
// vfptr is this file's MSVC-layout vtable (see the file header).  pBar is
// dereferenced without a NULL test, as in retail.
// Symbol: ??0CDockContext@@QEAA@PEAVCControlBar@@@Z
extern "C" void* MS_ABI impl___0CDockContext__QEAA_PEAVCControlBar___Z(void* pThis, CControlBar* pBar) {
    DockContext& c = Ctx(pThis);
    c.vfptr = g_DockContextVtbl;
    c.m_pBar = pBar;
    c.m_ptLast = POINT{ 0, 0 };
    c.m_rectLast = RECT{ 0, 0, 0, 0 };
    c.m_sizeLast = SIZE{ 0, 0 };
    c.m_rectDragHorz = RECT{ 0, 0, 0, 0 };
    c.m_rectDragVert = RECT{ 0, 0, 0, 0 };
    c.m_rectFrameDragHorz = RECT{ 0, 0, 0, 0 };
    c.m_rectFrameDragVert = RECT{ 0, 0, 0, 0 };
    c.m_rectMRUDockPos = RECT{ 0, 0, 0, 0 };
    c.m_ptMRUFloatPos = POINT{ 0, 0 };
    c.m_pDockSite = static_cast<CFrameWnd*>(pBar->m_pDockSite);
    c.m_uMRUDockID = 0;
    const DWORD dwStyle = pBar->m_dwStyle;
    if (dwStyle & kCBRS_SIZE_DYNAMIC) {
        c.m_dwMRUFloatStyle = dwStyle & (kCBRS_ALIGN_TOP | kCBRS_SIZE_DYNAMIC);
    } else if (dwStyle & kCBRS_ORIENT_HORZ) {
        c.m_dwMRUFloatStyle = (dwStyle & kCBRS_FLOAT_MULTI) | kCBRS_ALIGN_TOP;
    } else {
        c.m_dwMRUFloatStyle = (dwStyle & kCBRS_FLOAT_MULTI) | kCBRS_ALIGN_LEFT;
    }
    c.m_ptMRUFloatPos.x = static_cast<LONG>(CW_USEDEFAULT);
    c.m_pDC = nullptr;
    return pThis;
}

// Transcribed from retail RVA 0x21e870 (mfc140u):
//     vfptr = CDockContext vftable;
//     if (m_pBar->m_pDockBar != NULL)                                 // +0x70 -> +0x138
//         m_pBar->m_pDockBar->RemoveControlBar(m_pBar, -1, FALSE);   // 0x1d8a10
// m_pBar is dereferenced without a NULL test, as in retail.
// Symbol: ??1CDockContext@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockContext__UEAA_XZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    c.vfptr = g_DockContextVtbl;
    if (c.m_pBar->m_pDockBar != nullptr) {
        impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(c.m_pBar->m_pDockBar, c.m_pBar, -1, FALSE);
    }
}

// Transcribed from retail RVA 0x21f7e0 (mfc140u):
//     BOOL bStyleHorz = m_dwStyle & CBRS_ORIENT_HORZ;
//     if (m_bFlip) bStyleHorz = !bStyleHorz;
//     DWORD dwDock = 0;
//     if (bStyleHorz && (m_dwDockStyle & CBRS_ORIENT_HORZ))
//         dwDock = m_pDockSite->CanDock(m_rectDragHorz, m_dwDockStyle & ~CBRS_ORIENT_VERT, NULL);
//     else if (m_dwDockStyle & CBRS_ORIENT_VERT)
//         dwDock = m_pDockSite->CanDock(m_rectDragVert, m_dwDockStyle & ~CBRS_ORIENT_HORZ, NULL);
//     if (!m_bFlip && dwDock == 0) {
//         if (m_dwDockStyle & CBRS_ORIENT_HORZ) {
//             DWORD dwCurr = CanDock(m_rectDragVert, m_dwDockStyle & ~CBRS_ORIENT_VERT, NULL);
//             dwDock       = CanDock(m_rectDragHorz, m_dwDockStyle & ~CBRS_ORIENT_VERT, NULL);
//             dwDock = (dwDock == dwCurr) ? dwDock : 0;
//         }
//         if (dwDock == 0 && (m_dwDockStyle & CBRS_ORIENT_VERT)) {
//             DWORD dwCurr = CanDock(m_rectDragHorz, m_dwDockStyle & ~CBRS_ORIENT_HORZ, NULL);
//             dwDock       = CanDock(m_rectDragVert, m_dwDockStyle & ~CBRS_ORIENT_HORZ, NULL);
//             dwDock = (dwDock == dwCurr) ? dwDock : 0;
//         }
//     }
//     return dwDock;
// (every CanDock is CFrameWnd::CanDock, 0x2a1990, on m_pDockSite, in that call order)
// Symbol: ?CanDock@CDockContext@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__CanDock_CDockContext__QEAAKXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    int bStyleHorz = (c.m_dwStyle & kCBRS_ORIENT_HORZ) != 0;
    if (c.m_bFlip != 0) bStyleHorz = !bStyleHorz;

    DWORD dwDock = 0;
    if (bStyleHorz && (c.m_dwDockStyle & kCBRS_ORIENT_HORZ)) {
        dwDock = FrameCanDock(c.m_pDockSite, c.m_rectDragHorz, c.m_dwDockStyle & ~kCBRS_ORIENT_VERT, nullptr);
    } else if (c.m_dwDockStyle & kCBRS_ORIENT_VERT) {
        dwDock = FrameCanDock(c.m_pDockSite, c.m_rectDragVert, c.m_dwDockStyle & ~kCBRS_ORIENT_HORZ, nullptr);
    }

    if (c.m_bFlip == 0 && dwDock == 0) {
        if (c.m_dwDockStyle & kCBRS_ORIENT_HORZ) {
            const DWORD dwCurr = FrameCanDock(c.m_pDockSite, c.m_rectDragVert, c.m_dwDockStyle & ~kCBRS_ORIENT_VERT, nullptr);
            dwDock = FrameCanDock(c.m_pDockSite, c.m_rectDragHorz, c.m_dwDockStyle & ~kCBRS_ORIENT_VERT, nullptr);
            dwDock = (dwDock == dwCurr) ? dwDock : 0;
        }
        if (dwDock == 0 && (c.m_dwDockStyle & kCBRS_ORIENT_VERT)) {
            const DWORD dwCurr = FrameCanDock(c.m_pDockSite, c.m_rectDragHorz, c.m_dwDockStyle & ~kCBRS_ORIENT_HORZ, nullptr);
            dwDock = FrameCanDock(c.m_pDockSite, c.m_rectDragVert, c.m_dwDockStyle & ~kCBRS_ORIENT_HORZ, nullptr);
            dwDock = (dwDock == dwCurr) ? dwDock : 0;
        }
    }
    return dwDock;
}

// Transcribed from retail RVA 0x21f590 (mfc140u):
//     DrawFocusRect(TRUE);                          // 0x21f600 -- erase the tracker
//     ::ReleaseCapture();                           // IAT ReleaseCapture
//     CWnd* pWnd = CWnd::GetDesktopWindow();        // IAT GetDesktopWindow, CWnd::FromHandle 0x28ad70
//     pWnd->UnlockWindowUpdate();                   // IAT LockWindowUpdate(NULL)
//     if (m_pDC != NULL) {
//         pWnd->ReleaseDC(m_pDC);                   // IAT ReleaseDC(pWnd->m_hWnd, m_pDC->m_hDC)
//         m_pDC = NULL;
//     }
// Retail wraps the desktop HWND in a CWnd only to read its m_hWnd back; this
// body uses the HWND directly (so no temporary CWnd is created).
// Symbol: ?CancelLoop@CDockContext@@QEAAXXZ
extern "C" void MS_ABI impl__CancelLoop_CDockContext__QEAAXXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    impl__DrawFocusRect_CDockContext__QEAAXH_Z(pThis, TRUE);
    ::ReleaseCapture();
    HWND hWndDesktop = ::GetDesktopWindow();
    ::LockWindowUpdate(nullptr);
    if (c.m_pDC != nullptr) {
        ::ReleaseDC(hWndDesktop, c.m_pDC->m_hDC);
        c.m_pDC = nullptr;
    }
}

// Transcribed from retail RVA 0x21f600 (mfc140u):
//     CSize size(CX_BORDER, CY_BORDER);                                  // 1, 1
//     CRect rect;                                                        // zeroed
//     CBrush* pWhiteBrush  = CBrush::FromHandle(::GetStockObject(WHITE_BRUSH)); // IAT GetStockObject(0), 0x2a3ea0
//     CBrush* pDitherBrush = CDC::GetHalftoneBrush();                    // 0x2a5650
//     CBrush* pBrush = pWhiteBrush;
//     if (m_dwOverDockStyle & CBRS_ORIENT_HORZ)      rect = m_rectDragHorz;
//     else if (m_dwOverDockStyle & CBRS_ORIENT_VERT) rect = m_rectDragVert;
//     else {
//         size.cx = ::GetSystemMetrics(SM_CXFRAME) - CX_BORDER;         // IAT GetSystemMetrics(0x20)
//         size.cy = ::GetSystemMetrics(SM_CYFRAME) - CY_BORDER;         // IAT GetSystemMetrics(0x21)
//         if (((m_dwStyle & CBRS_ORIENT_HORZ) && !m_bFlip) || ((m_dwStyle & CBRS_ORIENT_VERT) && m_bFlip))
//             rect = m_rectFrameDragHorz;
//         else
//             rect = m_rectFrameDragVert;
//         pBrush = pDitherBrush;
//     }
//     if (bRemoveRect) size.cx = size.cy = 0;
//     if (m_dwOverDockStyle & CBRS_ALIGN_ANY) ::InflateRect(&rect, -CX_BORDER, -CY_BORDER);
//     m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,         // 0x2a5720
//                         pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
//     m_rectLast = rect;  m_sizeLast = size;  m_bDitherLast = (pBrush == pDitherBrush);
// CBrush::FromHandle is the inline cast of CGdiObject::FromHandle (0x2a3ea0).
// Symbol: ?DrawFocusRect@CDockContext@@QEAAXH@Z
extern "C" void MS_ABI impl__DrawFocusRect_CDockContext__QEAAXH_Z(void* pThis, int bRemoveRect) {
    DockContext& c = Ctx(pThis);
    SIZE size = { 1, 1 };
    RECT rect = { 0, 0, 0, 0 };
    CBrush* pWhiteBrush = reinterpret_cast<CBrush*>(
        impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(::GetStockObject(WHITE_BRUSH)));
    CBrush* pDitherBrush = impl__GetHalftoneBrush_CDC__SAPEAVCBrush__XZ();
    CBrush* pBrush = pWhiteBrush;

    if (c.m_dwOverDockStyle & kCBRS_ORIENT_HORZ) {
        rect = c.m_rectDragHorz;
    } else if (c.m_dwOverDockStyle & kCBRS_ORIENT_VERT) {
        rect = c.m_rectDragVert;
    } else {
        size.cx = ::GetSystemMetrics(SM_CXFRAME) - 1;
        size.cy = ::GetSystemMetrics(SM_CYFRAME) - 1;
        if (((c.m_dwStyle & kCBRS_ORIENT_HORZ) && c.m_bFlip == 0) ||
            ((c.m_dwStyle & kCBRS_ORIENT_VERT) && c.m_bFlip != 0)) {
            rect = c.m_rectFrameDragHorz;
        } else {
            rect = c.m_rectFrameDragVert;
        }
        pBrush = pDitherBrush;
    }
    if (bRemoveRect) {
        size.cx = 0;
        size.cy = 0;
    }
    if (c.m_dwOverDockStyle & kCBRS_ALIGN_ANY) {
        ::InflateRect(&rect, -1, -1);
    }

    impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
        c.m_pDC, &rect, size, &c.m_rectLast, c.m_sizeLast,
        pBrush, c.m_bDitherLast ? pDitherBrush : pWhiteBrush);

    c.m_rectLast = rect;
    c.m_sizeLast = size;
    c.m_bDitherLast = (pBrush == pDitherBrush) ? TRUE : FALSE;
}

// Transcribed from retail RVA 0x21edb0 (mfc140u):
//     CancelLoop();                                                     // 0x21f590
//     if (m_dwOverDockStyle != 0) {
//         CDockBar* pDockBar = GetDockBar(m_dwOverDockStyle);           // 0x21f930
//         CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ? m_rectDragVert : m_rectDragHorz;
//         UINT uID = ::GetDlgCtrlID(pDockBar->m_hWnd);                   // IAT GetDlgCtrlID
//         if (uID >= AFX_IDW_DOCKBAR_TOP && uID <= AFX_IDW_DOCKBAR_BOTTOM) {   // 0xe81b..0xe81e
//             m_uMRUDockID = uID;
//             m_rectMRUDockPos = rect;
//             pDockBar->ScreenToClient(&m_rectMRUDockPos);               // 0x2a32b0
//         }
//         // m_pDockSite->DockControlBar(m_pBar, pDockBar, &rect) (afxwin.h:4246),
//         // inlined; retail keeps only its NULL test and the CDockBar call:
//         if (m_pBar == NULL) AfxThrowInvalidArgException();            // 0x227720
//         pDockBar->DockControlBar(m_pBar, &rect);                      // 0x1d8330
//         m_pDockSite->RecalcLayout(TRUE);                              // CFrameWnd vslot 96
//     } else if ((m_dwStyle & CBRS_SIZE_DYNAMIC) ||
//                ((m_dwStyle & CBRS_ORIENT_HORZ) && !m_bFlip) ||
//                ((m_dwStyle & CBRS_ORIENT_VERT) && m_bFlip)) {
//         m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI);
//         m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
//         m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);   // 0x2a17c0
//     } else {
//         m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (m_dwDockStyle & CBRS_FLOAT_MULTI);
//         m_ptMRUFloatPos = m_rectFrameDragVert.TopLeft();
//         m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
//     }
// DEVIATION: retail dereferences pDockBar with no NULL test (`mov 0x40(%rax)`
// straight after the GetDockBar call at 0x21ede2).  OpenMFC's CFrameWnd::CanDock (core/frame/CFrameWnd.cpp)
// is not the retail algorithm -- it returns 1 whenever the frame has an HWND
// and the style is non-zero, and stores NULL through a non-NULL ppDockBar --
// so GetDockBar can return NULL here where retail would have a dock bar; the
// body returns after CancelLoop in that case instead of faulting.
// Symbol: ?EndDrag@CDockContext@@QEAAXXZ
extern "C" void MS_ABI impl__EndDrag_CDockContext__QEAAXXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    impl__CancelLoop_CDockContext__QEAAXXZ(pThis);

    if (c.m_dwOverDockStyle != 0) {
        CDockBar* pDockBar = impl__GetDockBar_CDockContext__QEAAPEAVCDockBar__K_Z(pThis, c.m_dwOverDockStyle);
        if (pDockBar == nullptr) return;   // DEVIATION (see above)
        RECT rect = (c.m_dwOverDockStyle & kCBRS_ORIENT_VERT) ? c.m_rectDragVert : c.m_rectDragHorz;
        CWnd* pDockWnd = AsWnd(pDockBar);
        const UINT uID = static_cast<UINT>(::GetDlgCtrlID(pDockWnd->m_hWnd));
        if (uID - kAFX_IDW_DOCKBAR_TOP <= 3u) {
            c.m_uMRUDockID = uID;
            c.m_rectMRUDockPos = rect;
            impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pDockWnd, &c.m_rectMRUDockPos);
        }
        if (c.m_pBar == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        impl__DockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(pDockBar, c.m_pBar, &rect);
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(c.m_pDockSite, TRUE);   // vslot 96
        return;
    }

    if ((c.m_dwStyle & kCBRS_SIZE_DYNAMIC) ||
        ((c.m_dwStyle & kCBRS_ORIENT_HORZ) && c.m_bFlip == 0) ||
        ((c.m_dwStyle & kCBRS_ORIENT_VERT) && c.m_bFlip != 0)) {
        c.m_dwMRUFloatStyle = kCBRS_ALIGN_TOP | (c.m_dwDockStyle & kCBRS_FLOAT_MULTI);
        c.m_ptMRUFloatPos.x = c.m_rectFrameDragHorz.left;
        c.m_ptMRUFloatPos.y = c.m_rectFrameDragHorz.top;
    } else {
        c.m_dwMRUFloatStyle = kCBRS_ALIGN_LEFT | (c.m_dwDockStyle & kCBRS_FLOAT_MULTI);
        c.m_ptMRUFloatPos.x = c.m_rectFrameDragVert.left;
        c.m_ptMRUFloatPos.y = c.m_rectFrameDragVert.top;
    }
    impl__FloatControlBar_CFrameWnd__QEAAXPEAVCControlBar__VCPoint__K_Z(
        c.m_pDockSite, c.m_pBar, CPoint(c.m_ptMRUFloatPos.x, c.m_ptMRUFloatPos.y), c.m_dwMRUFloatStyle);
}

// Transcribed from retail RVA 0x21f260 (mfc140u):
//     CancelLoop();                                                     // 0x21f590
//     m_pBar->CalcDynamicLayout(m_rectDragVert.Width(), LM_HORZ | LM_COMMIT);   // vslot 92, result unused
//     m_pDockSite->FloatControlBar(m_pBar, m_rectFrameDragHorz.TopLeft(),     // 0x2a17c0 (tail jump)
//         CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI) | CBRS_SIZE_DYNAMIC);
// (During a resize StartResize seeds m_rectDragVert with the bar's size and
// m_rectFrameDragHorz with the framed size; see StartResize.)
// Symbol: ?EndResize@CDockContext@@QEAAXXZ
extern "C" void MS_ABI impl__EndResize_CDockContext__QEAAXXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    impl__CancelLoop_CDockContext__QEAAXXZ(pThis);
    (void)CallCalcDynamicLayout(c.m_pBar, c.m_rectDragVert.right - c.m_rectDragVert.left,
                                kLM_HORZ | kLM_COMMIT);   // vslot 92
    impl__FloatControlBar_CFrameWnd__QEAAXPEAVCControlBar__VCPoint__K_Z(
        c.m_pDockSite, c.m_pBar, CPoint(c.m_rectFrameDragHorz.left, c.m_rectFrameDragHorz.top),
        (c.m_dwDockStyle & kCBRS_FLOAT_MULTI) | kCBRS_ALIGN_TOP | kCBRS_SIZE_DYNAMIC);
}

// Transcribed from retail RVA 0x21f930 (mfc140u):
//     CDockBar* pBar;
//     if (dwOverDockStyle & CBRS_ORIENT_HORZ) {
//         m_pDockSite->CanDock(m_rectDragHorz, dwOverDockStyle & ~CBRS_ORIENT_VERT, &pBar);  // 0x2a1990
//         return pBar;
//     }
//     if (dwOverDockStyle & CBRS_ORIENT_VERT) {
//         m_pDockSite->CanDock(m_rectDragVert, dwOverDockStyle & ~CBRS_ORIENT_HORZ, &pBar);
//         return pBar;
//     }
//     return NULL;
// Retail's pBar is an uninitialised stack slot that CanDock is trusted to
// fill; this body starts it at NULL, which changes nothing when CanDock
// stores through the pointer (OpenMFC's always does).
// Symbol: ?GetDockBar@CDockContext@@QEAAPEAVCDockBar@@K@Z
extern "C" CDockBar* MS_ABI impl__GetDockBar_CDockContext__QEAAPEAVCDockBar__K_Z(void* pThis, unsigned long dwOverDockStyle) {
    DockContext& c = Ctx(pThis);
    CDockBar* pBar = nullptr;
    if (dwOverDockStyle & kCBRS_ORIENT_HORZ) {
        FrameCanDock(c.m_pDockSite, c.m_rectDragHorz, dwOverDockStyle & ~kCBRS_ORIENT_VERT, &pBar);
        return pBar;
    }
    if (dwOverDockStyle & kCBRS_ORIENT_VERT) {
        FrameCanDock(c.m_pDockSite, c.m_rectDragVert, dwOverDockStyle & ~kCBRS_ORIENT_HORZ, &pBar);
        return pBar;
    }
    return nullptr;
}

// Transcribed from retail RVA 0x21f4a0 (mfc140u):
//     MSG msg;
//     while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {   // IAT PeekMessageW
//         if (!::GetMessage(&msg, NULL, WM_PAINT, WM_PAINT)) return;         // IAT GetMessageW
//         ::DispatchMessage(&msg);                                           // IAT DispatchMessageW
//     }
//     m_dwDockStyle = m_pBar->m_dwDockStyle;                                 // +0x128
//     m_dwStyle = m_pBar->m_dwStyle & CBRS_ALIGN_ANY;                        // +0x124
//     ::SetRectEmpty(&m_rectLast);                                           // IAT SetRectEmpty
//     m_sizeLast.cx = m_sizeLast.cy = 0;  m_bDitherLast = FALSE;
//     m_bFlip = m_bForceFrame = FALSE;
//     CWnd* pWnd = CWnd::GetDesktopWindow();          // IAT GetDesktopWindow, CWnd::FromHandle 0x28ad70
//     if (pWnd->LockWindowUpdate())                   // IAT LockWindowUpdate
//         m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);   // 0x403
//     else
//         m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE);                          // 3
//     // GetDCEx inlines as CDC::FromHandle(::GetDCEx(m_hWnd, NULL, flags)) -- IAT GetDCEx, 0x2a2450
// As in CancelLoop, the desktop HWND is used directly rather than through a
// temporary CWnd.
// Symbol: ?InitLoop@CDockContext@@QEAAXXZ
extern "C" void MS_ABI impl__InitLoop_CDockContext__QEAAXXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    MSG msg;
    while (::PeekMessage(&msg, nullptr, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
        if (!::GetMessage(&msg, nullptr, WM_PAINT, WM_PAINT)) return;
        ::DispatchMessage(&msg);
    }

    c.m_dwDockStyle = c.m_pBar->m_dwDockStyle;
    c.m_dwStyle = c.m_pBar->m_dwStyle & kCBRS_ALIGN_ANY;
    ::SetRectEmpty(&c.m_rectLast);
    c.m_sizeLast.cx = 0;
    c.m_sizeLast.cy = 0;
    c.m_bDitherLast = FALSE;
    c.m_bFlip = FALSE;
    c.m_bForceFrame = FALSE;

    HWND hWndDesktop = ::GetDesktopWindow();
    const DWORD flags = ::LockWindowUpdate(hWndDesktop)
        ? (DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE)
        : (DCX_WINDOW | DCX_CACHE);
    c.m_pDC = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(::GetDCEx(hWndDesktop, nullptr, flags));
}

// Transcribed from retail RVA 0x21ecf0 (mfc140u):
//     CPoint ptOffset = pt - m_ptLast;
//     ::OffsetRect(&m_rectDragHorz,      ptOffset.x, ptOffset.y);   // IAT OffsetRect, in this order
//     ::OffsetRect(&m_rectFrameDragHorz, ptOffset.x, ptOffset.y);
//     ::OffsetRect(&m_rectDragVert,      ptOffset.x, ptOffset.y);
//     ::OffsetRect(&m_rectFrameDragVert, ptOffset.x, ptOffset.y);
//     m_ptLast = pt;
//     m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();           // 0x21f7e0
//     DrawFocusRect(FALSE);                                        // 0x21f600 (tail jump)
// Symbol: ?Move@CDockContext@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__Move_CDockContext__QEAAXVCPoint___Z(void* pThis, long long ptPacked) {
    DockContext& c = Ctx(pThis);
    const POINT pt = UnpackPoint(ptPacked);
    const int dx = pt.x - c.m_ptLast.x;
    const int dy = pt.y - c.m_ptLast.y;
    ::OffsetRect(&c.m_rectDragHorz, dx, dy);
    ::OffsetRect(&c.m_rectFrameDragHorz, dx, dy);
    ::OffsetRect(&c.m_rectDragVert, dx, dy);
    ::OffsetRect(&c.m_rectFrameDragVert, dx, dy);
    c.m_ptLast = pt;
    c.m_dwOverDockStyle = c.m_bForceFrame ? 0 : impl__CanDock_CDockContext__QEAAKXZ(pThis);
    impl__DrawFocusRect_CDockContext__QEAAXH_Z(pThis, FALSE);
}

// Transcribed from retail RVA 0x21ed80 (mfc140u):
//     if (nChar == VK_CONTROL)    UpdateState(&m_bForceFrame, bDown);   // 0x11, +0x90, 0x21f770
//     else if (nChar == VK_SHIFT) UpdateState(&m_bFlip, bDown);         // 0x10, +0x8c
// Symbol: ?OnKey@CDockContext@@QEAAXHH@Z
extern "C" void MS_ABI impl__OnKey_CDockContext__QEAAXHH_Z(void* pThis, int nChar, int bDown) {
    DockContext& c = Ctx(pThis);
    if (nChar == VK_CONTROL) {
        impl__UpdateState_CDockContext__QEAAXPEAHH_Z(pThis, &c.m_bForceFrame, bDown);
    } else if (nChar == VK_SHIFT) {
        impl__UpdateState_CDockContext__QEAAXPEAHH_Z(pThis, &c.m_bFlip, bDown);
    }
}

// Transcribed from retail RVA 0x21e8d0 (mfc140u; vftable slot 0):
//     m_bDragging = TRUE;
//     InitLoop();                                                       // 0x21f4a0
//     if (::GetLayout(m_pDC->m_hDC) & LAYOUT_RTL)                       // IAT GetLayout
//         ::SetLayout(m_pDC->m_hDC, LAYOUT_LTR);                        // IAT SetLayout
//     CRect rect;  ::GetWindowRect(m_pBar->m_hWnd, &rect);              // IAT GetWindowRect (each branch)
//     m_ptLast = pt;
//     if (m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) {
//         CSize sizeHorz  = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);   // vslot 92
//         CSize sizeVert  = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
//         CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);
//         m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
//         m_rectDragVert = CRect(rect.TopLeft(), sizeVert);
//         m_rectFrameDragHorz = m_rectFrameDragVert = CRect(rect.TopLeft(), sizeFloat);
//     } else if (m_pBar->m_dwStyle & CBRS_SIZE_FIXED) {
//         CSize sizeHorz = m_pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
//         CSize sizeVert = m_pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
//         m_rectFrameDragHorz = m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
//         m_rectFrameDragVert = m_rectDragVert = CRect(rect.TopLeft(), sizeVert);
//     } else {
//         BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ;
//         CSize size = m_pBar->CalcDynamicLayout(-1, bHorz ? LM_VERTDOCK : LM_HORZ | LM_HORZDOCK);
//         if (bHorz) { m_rectDragHorz = rect;
//                      m_rectDragVert = CRect(CPoint(pt.x - rect.Height()/2, rect.top), size); }
//         else       { m_rectDragVert = rect;
//                      m_rectDragHorz = CRect(CPoint(rect.left, pt.y - rect.Width()/2), size); }
//         m_rectFrameDragHorz = m_rectDragHorz;  m_rectFrameDragVert = m_rectDragVert;
//     }
//     CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);   // IAT AdjustWindowRectEx(.., 0xc40000, FALSE, 0x188)
//     CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
//     ::InflateRect(&m_rectFrameDragHorz, -afxData.cxBorder2, -afxData.cyBorder2);   // IAT InflateRect
//     ::InflateRect(&m_rectFrameDragVert, -afxData.cxBorder2, -afxData.cyBorder2);
//     _AfxAdjustRectangle(m_rectDragHorz, pt);  _AfxAdjustRectangle(m_rectDragVert, pt);   // 0x21e73c
//     _AfxAdjustRectangle(m_rectFrameDragHorz, pt);  _AfxAdjustRectangle(m_rectFrameDragVert, pt);
//     m_dwOverDockStyle = CanDock();                                    // 0x21f7e0
//     Move(pt);                                                         // 0x21ecf0
//     Track();                                                          // 0x21f990
// (Height()/2 and Width()/2 are signed divisions truncating toward zero:
// `cltd; sub %edx,%eax; sar $1,%eax`.)  DEVIATION: retail reads
// m_pDC->m_hDC without a NULL test; m_pDC is NULL when InitLoop returned early
// (a WM_QUIT came out of its WM_PAINT pump), so the layout check is skipped in
// that case instead of faulting.
// Symbol: ?StartDrag@CDockContext@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__StartDrag_CDockContext__UEAAXVCPoint___Z(void* pThis, long long ptPacked) {
    DockContext& c = Ctx(pThis);
    const POINT pt = UnpackPoint(ptPacked);
    c.m_bDragging = TRUE;
    impl__InitLoop_CDockContext__QEAAXXZ(pThis);
    if (c.m_pDC != nullptr && (::GetLayout(c.m_pDC->m_hDC) & kLAYOUT_RTL)) {
        ::SetLayout(c.m_pDC->m_hDC, kLAYOUT_LTR);
    }

    CControlBar* pBar = c.m_pBar;
    const DWORD dwBarStyle = pBar->m_dwStyle;
    RECT rect = { 0, 0, 0, 0 };
    if (dwBarStyle & kCBRS_SIZE_DYNAMIC) {
        ::GetWindowRect(pBar->m_hWnd, &rect);
        c.m_ptLast = pt;
        const SIZE sizeHorz  = CallCalcDynamicLayout(pBar, 0, kLM_HORZ | kLM_HORZDOCK);   // vslot 92
        const SIZE sizeVert  = CallCalcDynamicLayout(pBar, 0, kLM_VERTDOCK);
        const SIZE sizeFloat = CallCalcDynamicLayout(pBar, 0, kLM_HORZ | kLM_MRUWIDTH);
        SetRectXYWH(c.m_rectDragHorz, rect.left, rect.top, sizeHorz.cx, sizeHorz.cy);
        SetRectXYWH(c.m_rectDragVert, rect.left, rect.top, sizeVert.cx, sizeVert.cy);
        SetRectXYWH(c.m_rectFrameDragHorz, rect.left, rect.top, sizeFloat.cx, sizeFloat.cy);
        SetRectXYWH(c.m_rectFrameDragVert, rect.left, rect.top, sizeFloat.cx, sizeFloat.cy);
    } else if (dwBarStyle & kCBRS_SIZE_FIXED) {
        ::GetWindowRect(pBar->m_hWnd, &rect);
        c.m_ptLast = pt;
        const SIZE sizeHorz = CallCalcDynamicLayout(pBar, -1, kLM_HORZ | kLM_HORZDOCK);   // vslot 92
        const SIZE sizeVert = CallCalcDynamicLayout(pBar, -1, kLM_VERTDOCK);
        SetRectXYWH(c.m_rectDragHorz, rect.left, rect.top, sizeHorz.cx, sizeHorz.cy);
        c.m_rectFrameDragHorz = c.m_rectDragHorz;
        SetRectXYWH(c.m_rectDragVert, rect.left, rect.top, sizeVert.cx, sizeVert.cy);
        c.m_rectFrameDragVert = c.m_rectDragVert;
    } else {
        ::GetWindowRect(pBar->m_hWnd, &rect);
        const DWORD bHorz = c.m_dwStyle & kCBRS_ORIENT_HORZ;
        c.m_ptLast = pt;
        const SIZE size = CallCalcDynamicLayout(pBar, -1, bHorz ? kLM_VERTDOCK : (kLM_HORZ | kLM_HORZDOCK));   // vslot 92
        if (bHorz) {
            c.m_rectDragHorz = rect;
            const int x = pt.x - (rect.bottom - rect.top) / 2;
            SetRectXYWH(c.m_rectDragVert, x, rect.top, size.cx, size.cy);
        } else {
            c.m_rectDragVert = rect;
            const int y = pt.y - (rect.right - rect.left) / 2;
            SetRectXYWH(c.m_rectDragHorz, rect.left, y, size.cx, size.cy);
        }
        c.m_rectFrameDragHorz = c.m_rectDragHorz;
        c.m_rectFrameDragVert = c.m_rectDragVert;
    }

    ::AdjustWindowRectEx(&c.m_rectFrameDragHorz, kMiniFrameStyle, FALSE, kMiniFrameExStyle);
    ::AdjustWindowRectEx(&c.m_rectFrameDragVert, kMiniFrameStyle, FALSE, kMiniFrameExStyle);
    ::InflateRect(&c.m_rectFrameDragHorz, -kCxBorder2, -kCyBorder2);
    ::InflateRect(&c.m_rectFrameDragVert, -kCxBorder2, -kCyBorder2);

    AdjustRectangle(c.m_rectDragHorz, pt);
    AdjustRectangle(c.m_rectDragVert, pt);
    AdjustRectangle(c.m_rectFrameDragHorz, pt);
    AdjustRectangle(c.m_rectFrameDragVert, pt);

    c.m_dwOverDockStyle = impl__CanDock_CDockContext__QEAAKXZ(pThis);
    impl__Move_CDockContext__QEAAXVCPoint___Z(pThis, ptPacked);
    impl__Track_CDockContext__QEAAHXZ(pThis);
}

// Transcribed from retail RVA 0x21ef00 (mfc140u; vftable slot 1):
//     m_bDragging = FALSE;
//     InitLoop();                                                       // 0x21f4a0
//     if (::GetLayout(m_pDC->m_hDC) & LAYOUT_RTL) ::SetLayout(m_pDC->m_hDC, LAYOUT_LTR);
//     CRect rectBar;  ::GetWindowRect(m_pBar->m_hWnd, &rectBar);        // IAT GetWindowRect
//     m_ptLast = pt;
//     m_nHitTest = nHitTest;
//     CSize size = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH); // vslot 92
//     m_rectDragHorz = m_rectDragVert = m_rectFrameDragHorz = CRect(rectBar.TopLeft(), size);
//     CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);   // IAT AdjustWindowRectEx(.., 0xc40000, FALSE, 0x188)
//     ::InflateRect(&m_rectFrameDragHorz, -afxData.cxBorder2, -afxData.cyBorder2);
//     m_rectFrameDragVert = CRect(CPoint(0, 0), m_rectFrameDragHorz.Size() - m_rectDragVert.Size());
//     m_dwOverDockStyle = 0;
//     Stretch(pt);                                                      // 0x21f090
//     Track();                                                          // 0x21f990
// During a resize the drag rectangles are reused: m_rectDragHorz is the
// requested size Stretch grows, m_rectDragVert the size the bar accepted,
// m_rectFrameDragHorz that size with the mini-frame borders, and
// m_rectFrameDragVert the border thickness (its right/bottom are the size
// difference computed above).  DEVIATION: the same m_pDC NULL guard as
// StartDrag.
// Symbol: ?StartResize@CDockContext@@UEAAXHVCPoint@@@Z
extern "C" void MS_ABI impl__StartResize_CDockContext__UEAAXHVCPoint___Z(void* pThis, int nHitTest, long long ptPacked) {
    DockContext& c = Ctx(pThis);
    const POINT pt = UnpackPoint(ptPacked);
    c.m_bDragging = FALSE;
    impl__InitLoop_CDockContext__QEAAXXZ(pThis);
    if (c.m_pDC != nullptr && (::GetLayout(c.m_pDC->m_hDC) & kLAYOUT_RTL)) {
        ::SetLayout(c.m_pDC->m_hDC, kLAYOUT_LTR);
    }

    RECT rectBar = { 0, 0, 0, 0 };
    ::GetWindowRect(c.m_pBar->m_hWnd, &rectBar);
    c.m_ptLast = pt;
    c.m_nHitTest = nHitTest;
    const SIZE size = CallCalcDynamicLayout(c.m_pBar, 0, kLM_HORZ | kLM_MRUWIDTH);   // vslot 92

    SetRectXYWH(c.m_rectDragHorz, rectBar.left, rectBar.top, size.cx, size.cy);
    c.m_rectDragVert = c.m_rectDragHorz;
    c.m_rectFrameDragHorz = c.m_rectDragHorz;
    ::AdjustWindowRectEx(&c.m_rectFrameDragHorz, kMiniFrameStyle, FALSE, kMiniFrameExStyle);
    ::InflateRect(&c.m_rectFrameDragHorz, -kCxBorder2, -kCyBorder2);

    c.m_rectFrameDragVert.left = 0;
    c.m_rectFrameDragVert.top = 0;
    c.m_rectFrameDragVert.right = (c.m_rectFrameDragHorz.right - c.m_rectFrameDragHorz.left) -
                                  (c.m_rectDragVert.right - c.m_rectDragVert.left);
    c.m_rectFrameDragVert.bottom = (c.m_rectFrameDragHorz.bottom - c.m_rectFrameDragHorz.top) -
                                   (c.m_rectDragVert.bottom - c.m_rectDragVert.top);
    c.m_dwOverDockStyle = 0;

    impl__Stretch_CDockContext__QEAAXVCPoint___Z(pThis, ptPacked);
    impl__Track_CDockContext__QEAAHXZ(pThis);
}

// Transcribed from retail RVA 0x21f090 (mfc140u):
//     CPoint ptOffset = pt - m_ptLast;
//     int nLength;  DWORD dwMode = LM_HORZ;
//     if (m_nHitTest == HTLEFT || m_nHitTest == HTRIGHT) {             // 10, 11
//         if (m_nHitTest == HTLEFT) m_rectDragHorz.left += ptOffset.x; else m_rectDragHorz.right += ptOffset.x;
//         nLength = m_rectDragHorz.Width();
//     } else {
//         dwMode |= LM_LENGTHY;
//         if (m_nHitTest == HTTOP) m_rectDragHorz.top += ptOffset.y; else m_rectDragHorz.bottom += ptOffset.y;
//         nLength = m_rectDragHorz.Height();
//     }
//     if (nLength < 0) nLength = 0;
//     CSize size = m_pBar->CalcDynamicLayout(nLength, dwMode);         // vslot 92
//     CRect rectDesk(::GetSystemMetrics(SM_XVIRTUALSCREEN), ::GetSystemMetrics(SM_YVIRTUALSCREEN),   // 0x4c, 0x4d
//                    left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN), top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN)); // 0x4e, 0x4f
//     CRect rectTemp = m_rectFrameDragHorz;
//     CRect rect;                                                       // zeroed
//     if (m_nHitTest == HTLEFT || m_nHitTest == HTTOP) {               // (nHitTest - 10) & ~2 == 0
//         rectTemp.left = rectTemp.right  - (size.cx + m_rectFrameDragVert.Width());
//         rectTemp.top  = rectTemp.bottom - (size.cy + m_rectFrameDragVert.Height());
//         if (::IntersectRect(&rect, &rectDesk, &rectTemp)) {          // IAT IntersectRect
//             m_rectDragVert.left = m_rectDragVert.right - size.cx;
//             m_rectDragVert.top  = m_rectDragVert.bottom - size.cy;
//             m_rectFrameDragHorz.left = rectTemp.left;  m_rectFrameDragHorz.top = rectTemp.top;
//         }
//     } else {
//         rectTemp.right  = rectTemp.left + size.cx + m_rectFrameDragVert.Width();
//         rectTemp.bottom = rectTemp.top  + size.cy + m_rectFrameDragVert.Height();
//         if (::IntersectRect(&rect, &rectDesk, &rectTemp)) {
//             m_rectDragVert.right  = m_rectDragVert.left + size.cx;
//             m_rectDragVert.bottom = m_rectDragVert.top + size.cy;
//             m_rectFrameDragHorz.right = rectTemp.right;  m_rectFrameDragHorz.bottom = rectTemp.bottom;
//         }
//     }
//     m_ptLast = pt;
//     DrawFocusRect(FALSE);                                             // 0x21f600
// Symbol: ?Stretch@CDockContext@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__Stretch_CDockContext__QEAAXVCPoint___Z(void* pThis, long long ptPacked) {
    DockContext& c = Ctx(pThis);
    const POINT pt = UnpackPoint(ptPacked);
    int nLength;
    DWORD dwMode = kLM_HORZ;
    if (c.m_nHitTest == HTLEFT || c.m_nHitTest == HTRIGHT) {
        const int dx = pt.x - c.m_ptLast.x;
        if (c.m_nHitTest == HTLEFT) c.m_rectDragHorz.left += dx;
        else c.m_rectDragHorz.right += dx;
        nLength = c.m_rectDragHorz.right - c.m_rectDragHorz.left;
    } else {
        dwMode |= kLM_LENGTHY;
        const int dy = pt.y - c.m_ptLast.y;
        if (c.m_nHitTest == HTTOP) c.m_rectDragHorz.top += dy;
        else c.m_rectDragHorz.bottom += dy;
        nLength = c.m_rectDragHorz.bottom - c.m_rectDragHorz.top;
    }
    if (nLength < 0) nLength = 0;

    const SIZE size = CallCalcDynamicLayout(c.m_pBar, nLength, dwMode);   // vslot 92

    RECT rectDesk;
    rectDesk.left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
    rectDesk.top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
    rectDesk.right = ::GetSystemMetrics(SM_CXVIRTUALSCREEN) + rectDesk.left;
    rectDesk.bottom = ::GetSystemMetrics(SM_CYVIRTUALSCREEN) + rectDesk.top;
    RECT rectTemp = c.m_rectFrameDragHorz;
    RECT rect = { 0, 0, 0, 0 };
    const RECT& fb = c.m_rectFrameDragVert;   // frame-border size set by StartResize

    if (c.m_nHitTest == HTLEFT || c.m_nHitTest == HTTOP) {
        rectTemp.left = fb.left - fb.right - size.cx + rectTemp.right;
        rectTemp.top = fb.top - fb.bottom - size.cy + rectTemp.bottom;
        if (::IntersectRect(&rect, &rectDesk, &rectTemp)) {
            c.m_rectDragVert.left = c.m_rectDragVert.right - size.cx;
            c.m_rectDragVert.top = c.m_rectDragVert.bottom - size.cy;
            c.m_rectFrameDragHorz.left = rectTemp.left;
            c.m_rectFrameDragHorz.top = rectTemp.top;
        }
    } else {
        rectTemp.right = fb.right - fb.left + rectTemp.left + size.cx;
        rectTemp.bottom = rectTemp.top - fb.top + fb.bottom + size.cy;
        if (::IntersectRect(&rect, &rectDesk, &rectTemp)) {
            c.m_rectDragVert.right = c.m_rectDragVert.left + size.cx;
            c.m_rectDragVert.bottom = c.m_rectDragVert.top + size.cy;
            c.m_rectFrameDragHorz.right = rectTemp.right;
            c.m_rectFrameDragHorz.bottom = rectTemp.bottom;
        }
    }
    c.m_ptLast = pt;
    impl__DrawFocusRect_CDockContext__QEAAXH_Z(pThis, FALSE);
}

// Transcribed from retail RVA 0x21f2c0 (mfc140u; vftable slot 2):
//     if (m_pBar->IsFloating()) {                                       // 0x2a0e80
//         if (!(m_pBar->m_dwDockStyle & CBRS_ALIGN_ANY)) return;        // +0x128
//         CRect rect = m_rectMRUDockPos;
//         CDockBar* pDockBar = NULL;
//         if (m_uMRUDockID != 0) {
//             pDockBar = (CDockBar*)m_pDockSite->GetControlBar(m_uMRUDockID);   // 0x29f7d0
//             if (pDockBar == NULL) AfxThrowInvalidArgException();      // 0x227720
//             pDockBar->ClientToScreen(&rect);                          // 0x2a3310
//         }
//         // m_pDockSite->ReDockControlBar(m_pBar, pDockBar, &rect) (afxwin.h:4248),
//         // inlined (dwDockBarMap is CFrameWnd's static table, afxwin.h:4265):
//         if (m_pBar == NULL) AfxThrowInvalidArgException();
//         if (pDockBar == NULL) {
//             CDockBar* pPossibleBar = NULL;
//             for (int i = 0; i < 4; i++) {
//                 CDockBar* pTempBar = (CDockBar*)m_pDockSite->GetControlBar(dwDockBarMap[i][0]);
//                 if (pTempBar != NULL &&
//                     pTempBar->FindBar((CControlBar*)(UINT_PTR)::GetDlgCtrlID(m_pBar->m_hWnd), -1) > 0) {  // IAT GetDlgCtrlID, 0x1d9520
//                     pDockBar = pTempBar;  break;
//                 }
//                 if (((dwDockBarMap[i][1] ^ m_pBar->m_dwStyle) & CBRS_ALIGN_ANY) == 0)
//                     pPossibleBar = (CDockBar*)m_pDockSite->GetControlBar(dwDockBarMap[i][0]);
//             }
//             if (pDockBar == NULL) pDockBar = pPossibleBar;
//         }
//         if (pDockBar == NULL) AfxThrowInvalidArgException();
//         pDockBar->ReDockControlBar(m_pBar, &rect);                    // 0x1d8680
//         m_pDockSite->RecalcLayout(TRUE);                              // CFrameWnd vslot 96
//     } else {
//         CPoint ptFloat = m_ptMRUFloatPos;
//         if (ptFloat.x < 0 || ptFloat.y < 0) {
//             ptFloat = m_rectMRUDockPos.TopLeft();
//             m_pBar->GetParent()->ClientToScreen(&ptFloat);            // IAT GetParent, 0x28ad70, IAT ClientToScreen
//         }
//         m_pDockSite->FloatControlBar(m_pBar, ptFloat, m_dwMRUFloatStyle);   // 0x2a17c0
//     }
// CWnd::GetParent inlines as CWnd::FromHandle(::GetParent(m_hWnd)) and
// ClientToScreen(LPPOINT) as ::ClientToScreen(m_hWnd, lpPoint); the body calls
// FromHandle and reads the wrapper's m_hWnd back, as retail does.
// Symbol: ?ToggleDocking@CDockContext@@UEAAXXZ
extern "C" void MS_ABI impl__ToggleDocking_CDockContext__UEAAXXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    if (impl__IsFloating_CControlBar__QEBAHXZ(c.m_pBar)) {
        if (!(c.m_pBar->m_dwDockStyle & kCBRS_ALIGN_ANY)) return;
        RECT rect = c.m_rectMRUDockPos;
        CDockBar* pDockBar = nullptr;
        if (c.m_uMRUDockID != 0) {
            pDockBar = reinterpret_cast<CDockBar*>(
                impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(c.m_pDockSite, c.m_uMRUDockID));
            if (pDockBar == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return;
            }
            impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(AsWnd(pDockBar), &rect);
        }
        CControlBar* pBar = c.m_pBar;
        if (pBar == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        if (pDockBar == nullptr) {
            CDockBar* pPossibleBar = nullptr;
            for (int i = 0; i < 4; ++i) {
                CDockBar* pTempBar = reinterpret_cast<CDockBar*>(
                    impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(c.m_pDockSite, kDockBarMap[i][0]));
                if (pTempBar != nullptr) {
                    const UINT uID = static_cast<UINT>(::GetDlgCtrlID(pBar->m_hWnd));
                    CControlBar* pPlaceHolder = reinterpret_cast<CControlBar*>(static_cast<UINT_PTR>(uID));
                    if (impl__FindBar_CDockBar__QEAAHPEAVCControlBar__H_Z(pTempBar, pPlaceHolder, -1) > 0) {
                        pDockBar = pTempBar;
                        break;
                    }
                }
                if (((kDockBarMap[i][1] ^ pBar->m_dwStyle) & kCBRS_ALIGN_ANY) == 0) {
                    pPossibleBar = reinterpret_cast<CDockBar*>(
                        impl__GetControlBar_CFrameWnd__QEAAPEAVCControlBar__I_Z(c.m_pDockSite, kDockBarMap[i][0]));
                }
            }
            if (pDockBar == nullptr) pDockBar = pPossibleBar;
            if (pDockBar == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return;
            }
        }
        impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(pDockBar, pBar, &rect);
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(c.m_pDockSite, TRUE);   // vslot 96
        return;
    }

    POINT ptFloat = c.m_ptMRUFloatPos;
    if (ptFloat.x < 0 || ptFloat.y < 0) {
        ptFloat.x = c.m_rectMRUDockPos.left;
        ptFloat.y = c.m_rectMRUDockPos.top;
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(c.m_pBar->m_hWnd));
        ::ClientToScreen(pParent->m_hWnd, &ptFloat);
    }
    impl__FloatControlBar_CFrameWnd__QEAAXPEAVCControlBar__VCPoint__K_Z(
        c.m_pDockSite, c.m_pBar, CPoint(ptFloat.x, ptFloat.y), c.m_dwMRUFloatStyle);
}

// Transcribed from retail RVA 0x21f990 (mfc140u):
//     if (::GetCapture() != NULL) return FALSE;                         // IAT GetCapture
//     m_pBar->SetCapture();          // CWnd::FromHandle(::SetCapture(m_hWnd)) -- IAT SetCapture, 0x28ad70
//     while (CWnd::FromHandle(::GetCapture()) == m_pBar) {
//         MSG msg;
//         if (!::GetMessage(&msg, NULL, 0, 0)) {                        // IAT GetMessageW
//             AfxPostQuitMessage((int)msg.wParam);                      // 0x1cc850
//             break;
//         }
//         switch (msg.message) {
//         case WM_LBUTTONUP:   if (m_bDragging) EndDrag(); else EndResize();   return TRUE;
//         case WM_MOUSEMOVE:   if (m_bDragging) Move(msg.pt); else Stretch(msg.pt);  break;
//         case WM_KEYUP:       if (m_bDragging) OnKey((int)msg.wParam, FALSE);  break;
//         case WM_KEYDOWN:     if (m_bDragging) OnKey((int)msg.wParam, TRUE);
//                              if (msg.wParam == VK_ESCAPE) { CancelLoop(); return FALSE; }
//                              break;
//         case WM_RBUTTONDOWN: CancelLoop(); return FALSE;
//         default:             ::DispatchMessage(&msg);  break;         // IAT DispatchMessageW
//         }
//     }
//     CancelLoop();
//     return FALSE;
// Retail inlines OnKey (the `cmpl $0x11` / `cmpl $0x10` tests on the low 32
// bits of wParam, then UpdateState 0x21f770); this body calls the OnKey thunk,
// which is the same code.
// Symbol: ?Track@CDockContext@@QEAAHXZ
extern "C" int MS_ABI impl__Track_CDockContext__QEAAHXZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    if (::GetCapture() != nullptr) return FALSE;

    (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(c.m_pBar->m_hWnd));
    while (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture()) == c.m_pBar) {
        MSG msg;
        if (!::GetMessage(&msg, nullptr, 0, 0)) {
            impl__AfxPostQuitMessage__YAXH_Z(static_cast<int>(msg.wParam));
            break;
        }
        switch (msg.message) {
        case WM_LBUTTONUP:
            if (c.m_bDragging) impl__EndDrag_CDockContext__QEAAXXZ(pThis);
            else impl__EndResize_CDockContext__QEAAXXZ(pThis);
            return TRUE;
        case WM_MOUSEMOVE: {
            const long long ptPacked = static_cast<long long>(
                (static_cast<unsigned long long>(static_cast<unsigned int>(msg.pt.y)) << 32) |
                static_cast<unsigned int>(msg.pt.x));
            if (c.m_bDragging) impl__Move_CDockContext__QEAAXVCPoint___Z(pThis, ptPacked);
            else impl__Stretch_CDockContext__QEAAXVCPoint___Z(pThis, ptPacked);
            break;
        }
        case WM_KEYUP:
            if (c.m_bDragging) impl__OnKey_CDockContext__QEAAXHH_Z(pThis, static_cast<int>(msg.wParam), FALSE);
            break;
        case WM_KEYDOWN:
            if (c.m_bDragging) impl__OnKey_CDockContext__QEAAXHH_Z(pThis, static_cast<int>(msg.wParam), TRUE);
            if (msg.wParam == VK_ESCAPE) {
                impl__CancelLoop_CDockContext__QEAAXXZ(pThis);
                return FALSE;
            }
            break;
        case WM_RBUTTONDOWN:
            impl__CancelLoop_CDockContext__QEAAXXZ(pThis);
            return FALSE;
        default:
            ::DispatchMessage(&msg);
            break;
        }
    }
    impl__CancelLoop_CDockContext__QEAAXXZ(pThis);
    return FALSE;
}

// Transcribed from retail RVA 0x21f770 (mfc140u):
//     if (*pFlag != bNewValue) {
//         *pFlag = bNewValue;
//         m_bFlip = (m_dwDockStyle & CBRS_ORIENT_HORZ) && (m_dwDockStyle & CBRS_ORIENT_VERT) && m_bFlip;
//         m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();            // 0x21f7e0
//         DrawFocusRect(FALSE);                                         // 0x21f600
//     }
// Symbol: ?UpdateState@CDockContext@@QEAAXPEAHH@Z
extern "C" void MS_ABI impl__UpdateState_CDockContext__QEAAXPEAHH_Z(void* pThis, int* pFlag, int bNewValue) {
    DockContext& c = Ctx(pThis);
    if (*pFlag == bNewValue) return;
    *pFlag = bNewValue;
    c.m_bFlip = ((c.m_dwDockStyle & kCBRS_ORIENT_VERT) && (c.m_dwDockStyle & kCBRS_ORIENT_HORZ) && c.m_bFlip != 0)
                    ? TRUE : FALSE;
    c.m_dwOverDockStyle = c.m_bForceFrame ? 0 : impl__CanDock_CDockContext__QEAAKXZ(pThis);
    impl__DrawFocusRect_CDockContext__QEAAXH_Z(pThis, FALSE);
}
