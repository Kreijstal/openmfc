// CGlobalUtils + CPreviewView exports, wave 22 ("other-22").
//
// 2 CGlobalUtils numeric conversions + 28 CPreviewView methods transcribed
// from the retail mfc140u export (ordinal -> RVA -> objdump).
//
// CPreviewView is NOT declared in the clean-room headers (see
// global_controlbar_keyframe_rtti.cpp: the class is registered with a 536
// = 0x218 byte layout, and the message-map base chains to CScrollView).  All
// CPreviewView methods therefore take `void* pThis` and touch state through
// the member offsets harvested from retail disassembly:
//     0x1DC  int          page-state latch (DoZoom fit mode / OnNumPageChange)
//     0x1E0  int          zoom state (0 = fit page, 1 = zoom, 2 = two-up)
//     0x1E4  int          page count
//     0x1E8  unsigned int current page (1-based)
//     0x1EC  int          two-up / page-change mode flag
//     0x1F0  int          page display width (CalcPageDisplaySize)
//     0x130  void*        CCreateContext::m_pCurrentFrame stored in OnCreate
//     0x138  CView*       m_pPrintView (SetPrintView)
//     0x180  void*        per-page state array (stride 40, unmodeled)
//     0x210  void*        m_pPDC (print-dialog wrapper, unmodeled)
// CWnd::m_hWnd lives at +0x40 (matches the rest of this codebase).
//
// Where the retail body depends on unmodeled state (the page-state array at
// 0x180, the PDC object at 0x210, CScrollView scroll machinery, font/DC
// helpers) the implementation keeps the observable member writes and marks the
// gaps with `// TODO(clean-room)`.

#include "openmfc/afxmfc.h"
#include <oleauto.h>

#include <cstddef>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CWnd / CView / CScrollView forward-declared exports used as bases below.
// CPreviewView::OnCreate (0x180280500) calls CView::OnCreate directly.
extern "C" int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(
    CView* pThis, CREATESTRUCTW* pCreateStruct);

// CPreviewView ctor (0x1802802d0): defined in
// manual_small_stub_implementations.cpp; used by CreateObject.
extern "C" void* MS_ABI impl___0CPreviewView__QEAA_XZ(void* pThis);

// Same-file helper (defined below): DoZoom drives the page transition after a
// zoom-state change; OnZoomIn/OnZoomOut/SetZoomState call it.
extern "C" void MS_ABI impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nPage, CPoint pt);

// Field access helpers for the not-yet-declared CPreviewView layout.  All
// offsets come from retail disassembly (see file header).
static inline int& PV_INT(void* p, size_t off) {
    return *reinterpret_cast<int*>(reinterpret_cast<char*>(p) + off);
}
static inline unsigned int& PV_UINT(void* p, size_t off) {
    return *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(p) + off);
}
static inline void*& PV_PTR(void* p, size_t off) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(p) + off);
}
static inline HWND PV_HWND(void* p) {
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(p) + 0x40);
}

//=============================================================================
// CGlobalUtils::StringFromCy
//=============================================================================
// Retail (0x18006dac0): build a VT_CY VARIANT from the CY value, convert it to
// a BSTR with VariantChangeType, assign the BSTR into the CString (helper
// 0x180002e30 = CStringT assignment), VariantClear both VARIANTs, return
// TRUE on success.  VarBstrFromCy covers the same conversion; the BSTR is
// freed by SysFreeString (retail frees it via VariantClear).
// Symbol: ?StringFromCy@CGlobalUtils@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEATtagCY@@@Z
extern "C" int MS_ABI impl__StringFromCy_CGlobalUtils__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEATtagCY___Z(
    void* /*pThis*/, CString* pstr, CY* pcy)
{
    if (!pstr) return 0;
    if (!pcy) return 0;

    BSTR bstr = nullptr;
    HRESULT hr = ::VarBstrFromCy(*pcy, ::GetThreadLocale(), 0, &bstr);
    if (FAILED(hr)) return 0;

    if (bstr) {
        *pstr = static_cast<const wchar_t*>(bstr);
        ::SysFreeString(bstr);
    } else {
        *pstr = L"";
    }
    return 1;
}

//=============================================================================
// CGlobalUtils::StringFromDecimal
//=============================================================================
// Retail (0x18006dc50): same shape as StringFromCy but with a VT_DECIMAL
// source -- VariantChangeType(&result, &src, 0, VT_BSTR) followed by the
// CString assignment and VariantClear cleanup.
// Symbol: ?StringFromDecimal@CGlobalUtils@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagDEC@@@Z
extern "C" int MS_ABI impl__StringFromDecimal_CGlobalUtils__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagDEC___Z(
    void* /*pThis*/, CString* pstr, DECIMAL* pdec)
{
    if (!pstr) return 0;
    if (!pdec) return 0;

    BSTR bstr = nullptr;
    HRESULT hr = ::VarBstrFromDec(pdec, ::GetThreadLocale(), 0, &bstr);
    if (FAILED(hr)) return 0;

    if (bstr) {
        *pstr = static_cast<const wchar_t*>(bstr);
        ::SysFreeString(bstr);
    } else {
        *pstr = L"";
    }
    return 1;
}

//=============================================================================
// CPreviewView::CalcScaleRatio
//=============================================================================
// Retail (0x180280b60), fully transcribed.  CSize is returned through the
// hidden return pointer (cbarcore.cpp BuildCSizeResult convention):
//     x = MulDiv(totalSize.cx, pageSize.cy, totalSize.cy)
//     if pageSize.cx > x  -> (pageSize.cx, totalSize.cx)
//     else                -> (x, totalSize.cy)
// Symbol: ?CalcScaleRatio@CPreviewView@@MEAA?AVCSize@@V2@0@Z
extern "C" void MS_ABI impl__CalcScaleRatio_CPreviewView__MEAA_AVCSize__V2_0_Z(
    CSize* pRet, void* /*pThis*/, CSize pageSize, CSize totalSize)
{
    if (!pRet) return;

    const int x = ::MulDiv(totalSize.cx, pageSize.cy, totalSize.cy);
    if (pageSize.cx > x) {
        new(pRet) CSize(pageSize.cx, totalSize.cx);
    } else {
        new(pRet) CSize(x, totalSize.cy);
    }
}

//=============================================================================
// CPreviewView::CalcPageDisplaySize
//=============================================================================
// Retail (0x180280c90): CSize returned through the hidden pointer.  Starts
// from the client size (helper 0x180289000); in fit mode (zoom state 0x1E0 ==
// 0) with a valid PDC page range the width is reduced by the client width.
// Then m_1F0 = (cx - 8) / 2 and the returned size is shrunk by fixed page
// margins (cy -= 16; cx -= 24 then halved for two-up, cx -= 16 otherwise).
// The client-size helper and PDC range are unmodeled, so the fit-mode
// subtraction is skipped.
// Symbol: ?CalcPageDisplaySize@CPreviewView@@IEAA?AVCSize@@XZ
extern "C" void MS_ABI impl__CalcPageDisplaySize_CPreviewView__IEAA_AVCSize__XZ(
    CSize* pRet, void* pThis)
{
    if (!pRet) return;
    new(pRet) CSize(0, 0);
    if (!pThis) return;

    // TODO(clean-room): retail starts from the client size (0x180289000) and,
    // in fit mode with a valid PDC page range, subtracts the client width from
    // the result; the helper and PDC range (0x210 vslot 0x130) are unmodeled.
    PV_INT(pThis, 0x1F0) = (pRet->cx - 8) / 2;   // m_nPageDisplayWidth
    if (PV_INT(pThis, 0x1EC) == 2) {
        pRet->cx = (pRet->cx - 24) / 2;          // two-up margins
    } else {
        pRet->cx -= 16;
    }
    pRet->cy -= 16;
}

//=============================================================================
// CPreviewView::SetCurrentPage
//=============================================================================
// Retail (0x180281cc0): store nPage into m_nCurrentPage (0x1E8), clamp it to
// the PDC page range (words at +0x30/+0x32 of the vslot 0x130 object), and in
// fit mode run a recalc helper (0x18028e6c0).  When bUpdate and the page count
// (0x1E4) is non-zero the per-page flag array (0x180, stride 40, flag at
// +0x18) is cleared.  Finally the window is invalidated.  The PDC object and
// the per-page array are unmodeled; the clamp is approximated with
// [1, pageCount] and the flags/recalc are skipped.
// Symbol: ?SetCurrentPage@CPreviewView@@IEAAXIH@Z
extern "C" void MS_ABI impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(
    void* pThis, unsigned int nPage, int bUpdate)
{
    if (!pThis) return;

    unsigned int& cur = PV_UINT(pThis, 0x1E8);
    cur = nPage;

    const int nCount = PV_INT(pThis, 0x1E4);
    if (nCount > 0) {
        if (cur > static_cast<unsigned int>(nCount)) cur = static_cast<unsigned int>(nCount);
        if (cur < 1) cur = 1;
    }

    // TODO(clean-room): retail clamps against the PDC page range (0x210 vslot
    // 0x130, words +0x30/+0x32), runs the recalc helper (0x18028e6c0) when the
    // zoom state (0x1E0) is zero, and clears the per-page flag array at 0x180
    // when bUpdate; those structures are not modeled.
    (void)bUpdate;

    HWND hwnd = PV_HWND(pThis);
    if (hwnd) ::InvalidateRect(hwnd, nullptr, TRUE);
}

//=============================================================================
// CPreviewView::OnNextPage
//=============================================================================
// Retail (0x180281780): tail-jump to SetCurrentPage(cur + 1, TRUE).
// Symbol: ?OnNextPage@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnNextPage_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(
        pThis, PV_UINT(pThis, 0x1E8) + 1, 1);
}

//=============================================================================
// CPreviewView::OnPrevPage
//=============================================================================
// Retail (0x1802817a0): tail-jump to SetCurrentPage(cur - 1, TRUE).
// Symbol: ?OnPrevPage@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPrevPage_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(
        pThis, PV_UINT(pThis, 0x1E8) - 1, 1);
}

//=============================================================================
// CPreviewView::OnNumPageChange
//=============================================================================
// Retail (0x180281720): toggle the two-up/page-change flag at 0x1EC
// (0x1EC = 3 - 0x1EC), latch it into 0x1DC, mirror it into the frame window
// state ([frame + 0x134], frame from the 0x180133930 singleton), then
// tail-jump to SetCurrentPage(cur, TRUE).  The frame mirror is unmodeled.
// Symbol: ?OnNumPageChange@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnNumPageChange_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;

    PV_INT(pThis, 0x1EC) = 3 - PV_INT(pThis, 0x1EC);
    PV_INT(pThis, 0x1DC) = PV_INT(pThis, 0x1EC);

    // TODO(clean-room): retail also copies 0x1EC into the main-frame state
    // ([frame + 0x134], frame resolved through the 0x180133930 singleton);
    // not modeled.

    impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(
        pThis, PV_UINT(pThis, 0x1E8), 1);
}

//=============================================================================
// CPreviewView::OnZoomIn
//=============================================================================
// Retail (0x180281a60): if the zoom state (0x1E0) is not 2, increment it and
// call DoZoom(0, CPoint(0,0)); a saturated state (2) is left alone.
// Symbol: ?OnZoomIn@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnZoomIn_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    const int zoom = PV_INT(pThis, 0x1E0);
    if (zoom == 2) return;
    PV_INT(pThis, 0x1E0) = zoom + 1;
    impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(pThis, 0, CPoint(0, 0));
}

//=============================================================================
// CPreviewView::OnZoomOut
//=============================================================================
// Retail (0x180281aa0): if the zoom state (0x1E0) is not 0, decrement it and
// call DoZoom(0, CPoint(0,0)); a saturated state (0) is left alone.
// Symbol: ?OnZoomOut@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnZoomOut_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    const int zoom = PV_INT(pThis, 0x1E0);
    if (zoom == 0) return;
    PV_INT(pThis, 0x1E0) = zoom - 1;
    impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(pThis, 0, CPoint(0, 0));
}

//=============================================================================
// CPreviewView::SetZoomState
//=============================================================================
// Retail (0x180281a30): if the new zoom state differs from the stored one
// (0x1E0), store it and call DoZoom(nPage, pt).
// Symbol: ?SetZoomState@CPreviewView@@IEAAXIIVCPoint@@@Z
extern "C" void MS_ABI impl__SetZoomState_CPreviewView__IEAAXIIVCPoint___Z(
    void* pThis, unsigned int nNewState, unsigned int nPage, CPoint pt)
{
    if (!pThis) return;
    if (PV_INT(pThis, 0x1E0) == static_cast<int>(nNewState)) return;
    PV_INT(pThis, 0x1E0) = static_cast<int>(nNewState);
    impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(pThis, nPage, pt);
}

//=============================================================================
// CPreviewView::DoZoom
//=============================================================================
// Retail (0x180281ae0): in fit mode (zoom state 0x1E0 == 0) the 0x1DC latch is
// copied into 0x1EC and the scroll bars are configured from the PDC page range
// (ShowScrollBar calls); the function then runs SetCurrentPage(cur, TRUE).  In
// zoomed mode 0x1EC is set to 1, the per-page state array (0x180) is shuffled
// and SetCurrentPage(cur + nPage, TRUE) is called.  The scroll/array plumbing
// is unmodeled; the member writes and page transitions are transcribed.
// Symbol: ?DoZoom@CPreviewView@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nPage, CPoint /*pt*/)
{
    if (!pThis) return;

    // TODO(clean-room): retail also configures the scroll ranges through the
    // PDC object (0x210 vslot 0x130) and ShowScrollBar (0x1802c6e48), and in
    // zoomed mode reshuffles the per-page state array at 0x180; unmodeled.
    if (PV_INT(pThis, 0x1E0) == 0) {
        // Fit mode: latch 0x1DC into 0x1EC, then re-show the current page.
        PV_INT(pThis, 0x1EC) = PV_INT(pThis, 0x1DC);
        impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(
            pThis, PV_UINT(pThis, 0x1E8), 1);
    } else {
        // Zoomed mode: mark the page-change flag and jump nPage from the
        // current page.
        PV_INT(pThis, 0x1EC) = 1;
        impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(
            pThis, PV_UINT(pThis, 0x1E8) + nPage, 1);
    }
}

//=============================================================================
// CPreviewView::OnCreate
//=============================================================================
// Retail (0x180280500): delegate to CView::OnCreate (0x180277660); on success
// store lpCreateStruct->lpCreateParams[0x18] (CCreateContext::m_pCurrentFrame)
// at +0x130 and return the base result.
// Symbol: ?OnCreate@CPreviewView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CPreviewView__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    if (!pThis) return -1;

    const int nRet = impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(
        static_cast<CView*>(pThis), lpCreateStruct);
    if (nRet == -1) return -1;

    if (lpCreateStruct && lpCreateStruct->lpCreateParams) {
        // Retail reads [lpCreateParams + 0x18] = CCreateContext::m_pCurrentFrame
        // and stores it at +0x130 (used by the preview-frame navigation).
        PV_PTR(pThis, 0x130) = *reinterpret_cast<void**>(
            static_cast<char*>(lpCreateStruct->lpCreateParams) + 0x18);
    }
    return nRet;
}

//=============================================================================
// CPreviewView::CreateObject (static)
//=============================================================================
// Retail (0x180282130): allocate sizeof(CPreviewView) = 0x218 bytes and call
// the constructor (0x1802802d0, the exported ??0CPreviewView stub).  The
// clean-room class is not declared, so the allocation is a raw 0x218 buffer.
// Symbol: ?CreateObject@CPreviewView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPreviewView__SAPEAVCObject__XZ()
{
    void* p = ::operator new(0x218, std::nothrow);
    if (p) {
        impl___0CPreviewView__QEAA_XZ(p);
    }
    return static_cast<CObject*>(p);
}

//=============================================================================
// CPreviewView::SetPrintView
//=============================================================================
// Retail (0x180280540): store m_pPrintView at +0x138, allocate the print-info
// wrapper (0x50 bytes) at +0x210 and a preview-font object at +0x140, seed the
// PDC page range flags, then drive the print view's setup virtuals.  Only the
// +0x138 store is modeled; the allocated objects and setup are unmodeled.
// Symbol: ?SetPrintView@CPreviewView@@QEAAHPEAVCView@@@Z
extern "C" int MS_ABI impl__SetPrintView_CPreviewView__QEAAHPEAVCView___Z(
    void* pThis, CView* pPrintView)
{
    if (!pThis) return 0;

    PV_PTR(pThis, 0x138) = pPrintView;

    // TODO(clean-room): retail also allocates and initialises the print-info
    // wrapper (0x210, 0x50 bytes), the preview-font object (0x140, 0x57 bytes)
    // and configures the PDC page-range flags and print-view setup virtuals;
    // not modeled.  Returns TRUE on success.
    return 1;
}

//=============================================================================
// CPreviewView::OnHScroll
//=============================================================================
// Retail (0x1802815e0): only does anything in zoom mode (zoom state 0x1E0 != 0),
// where it delegates to the scroll helper 0x1802894a0.
// Symbol: ?OnHScroll@CPreviewView@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CPreviewView__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int /*nSBCode*/, unsigned int /*nPos*/, void* /*pScrollBar*/)
{
    if (!pThis) return;
    if (PV_INT(pThis, 0x1E0) != 0) {
        // TODO(clean-room): retail delegates to the zoom-mode scroll helper
        // (0x1802894a0); not modeled.
    }
}

//=============================================================================
// CPreviewView::OnSetCursor
//=============================================================================
// Retail (0x180282080): for HTCLIENT hits resolves the page under the cursor
// via FindPageRect and installs a cached zoom-hand cursor (loaded lazily into
// 0x1F8), otherwise falls back to IDC_ARROW; non-client hits go to the base
// CWnd::OnSetCursor.  FindPageRect and the base are unmodeled, so the arrow
// cursor is always installed for client hits.
// Symbol: ?OnSetCursor@CPreviewView@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CPreviewView__IEAAHPEAVCWnd__II_Z(
    void* pThis, void* /*pWnd*/, unsigned int nHitTest, unsigned int /*nMessage*/)
{
    if (!pThis) return 0;

    if (nHitTest == 1 /*HTCLIENT*/) {
        // TODO(clean-room): retail resolves the page under the cursor with
        // FindPageRect and uses a zoom-hand cursor (0x7902) when the cursor is
        // over a page; the page array is unmodeled, so always use IDC_ARROW.
        ::SetCursor(::LoadCursorW(nullptr, MAKEINTRESOURCEW(0x7F00)));
    } else {
        // TODO(clean-room): retail forwards to the base CWnd::OnSetCursor
        // (0x18028ac80); not modeled.
    }
    return 0;
}

//=============================================================================
// CPreviewView::OnSize
//=============================================================================
// Retail (0x1802808b0): in fit mode clears the per-page flag array (0x180,
// stride 40, +0x18) and tail-jumps to CScrollView::OnSize (0x18028ac80); in
// zoom mode stores the client size (0x114/0x118) and tenths (0x11C/0x120) and
// tail-jumps to CView::OnSize (0x1802888f0).  Both the base handlers and the
// array are unmodeled.
// Symbol: ?OnSize@CPreviewView@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPreviewView__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the per-page flag reset and
    // the base OnSize dispatches (0x18028ac80 / 0x1802888f0) depend on
    // unmodeled scroll state; not modeled.
}

//=============================================================================
// CPreviewView::OnActivateView
//=============================================================================
// Retail (0x180280940): window-activation management (SetActiveWindow /
// IsWindowEnabled / SetFocus) for the preview frame chain; unmodeled.
// Symbol: ?OnActivateView@CPreviewView@@MEAAXHPEAVCView@@0@Z
extern "C" void MS_ABI impl__OnActivateView_CPreviewView__MEAAXHPEAVCView__0_Z(
    void* pThis, int /*bActivate*/, void* /*pActivateView*/, void* /*pDeactiveView*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180280940) manages
    // the activation focus through the preview frame chain; not modeled.
}

//=============================================================================
// CPreviewView::OnPreviewClose
//=============================================================================
// Retail (0x1802809d0): resolves the preview frame from the current app state,
// checks its runtime class, posts the close notification and releases the
// stored references; unmodeled.
// Symbol: ?OnPreviewClose@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPreviewClose_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x1802809d0) walks the
    // frame chain and posts the close message; not modeled.
}

//=============================================================================
// CPreviewView::OnPreviewPrint
//=============================================================================
// Retail (0x1802817c0): resolves the preview frame from +0x130, calls
// OnPreviewClose and drives the frame's print command; unmodeled.
// Symbol: ?OnPreviewPrint@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPreviewPrint_CPreviewView__IEAAXXZ(void* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x1802817c0) resolves
    // the preview frame (+0x130) and routes the print command; not modeled.
}

//=============================================================================
// CPreviewView::OnDraw
//=============================================================================
// Retail (0x180280ff0): full page rendering (scaled page bitmap + header text)
// through the m_pPrintView (0x138) and the page-state array (0x180); unmodeled.
// Symbol: ?OnDraw@CPreviewView@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CPreviewView__MEAAXPEAVCDC___Z(
    void* pThis, void* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180280ff0) renders
    // the scaled page through the print-view and page-state array; not modeled.
}

//=============================================================================
// CPreviewView::OnEraseBkgnd
//=============================================================================
// Retail (0x180280f10): fills the background with a cached brush and returns
// TRUE; the brush and DC machinery are unmodeled.
// Symbol: ?OnEraseBkgnd@CPreviewView@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewView__IEAAHPEAVCDC___Z(
    void* pThis, void* /*pDC*/)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x180280f10) fills the
    // background through GDI helpers; not modeled.
    return 1;
}

//=============================================================================
// CPreviewView::OnPrepareDC
//=============================================================================
// Retail (0x180280eb0): in fit mode seeds CPrintInfo::m_bContinuePrinting from
// the PDC page range; in zoom mode checks the page-state array before calling
// a layout helper (0x180287f20).  The PDC/array are unmodeled.
// Symbol: ?OnPrepareDC@CPreviewView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CPreviewView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(
    void* pThis, void* /*pDC*/, void* /*pInfo*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180280eb0) drives
    // print-continue state and page layout; not modeled.
}

//=============================================================================
// CPreviewView::OnDisplayPageNumber
//=============================================================================
// Retail (0x180281d80): formats "page N of M" from the PDC title and posts
// WM_SETMESSAGESTRING (0x362) to the frame; the formatting helpers are
// unmodeled.
// Symbol: ?OnDisplayPageNumber@CPreviewView@@MEAAXII@Z
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewView__MEAAXII_Z(
    void* pThis, unsigned int /*nCurrent*/, unsigned int /*nLast*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180281d80) formats
    // the page string and posts WM_SETMESSAGESTRING; not modeled.
}

//=============================================================================
// CPreviewView::FindPageRect
//=============================================================================
// Retail (0x180281830): walks the per-page state array (0x180, stride 40)
// testing each page rect (PtInRect) against the point; on a hit converts the
// point into page coordinates via MulDiv against the page scale.  The page
// array is unmodeled, so no page can be resolved.
// Symbol: ?FindPageRect@CPreviewView@@IEAAHAEAVCPoint@@AEAI@Z
extern "C" int MS_ABI impl__FindPageRect_CPreviewView__IEAAHAEAVCPoint__AEAI_Z(
    void* pThis, CPoint* /*pPoint*/, unsigned int* /*pPage*/)
{
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail (0x180281830) walks the
    // per-page rect array (0x180); not modeled.
    return 0;
}

//=============================================================================
// CPreviewView::OnLButtonDown
//=============================================================================
// Retail (0x1802819d0): resolves the clicked page with FindPageRect, toggles
// the zoom state (0x1E0, 2 -> 0 else +1) and runs DoZoom(page, point).  The
// page resolution depends on the unmodeled page array.
// Symbol: ?OnLButtonDown@CPreviewView@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CPreviewView__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x1802819d0) resolves
    // the clicked page via FindPageRect before toggling the zoom state; the
    // page array is unmodeled.
}

//=============================================================================
// CPreviewView::OnVScroll
//=============================================================================
// Retail (0x180281600): zoom mode delegates to the scroll helper (0x180289510);
// fit mode runs a scroll-code switch that moves the current page and calls
// SetCurrentPage.  The page movement is coupled to the unmodeled page array.
// Symbol: ?OnVScroll@CPreviewView@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CPreviewView__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int /*nSBCode*/, unsigned int /*nPos*/, void* /*pScrollBar*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180281600) handles
    // the scroll-bar codes through the page-state array and scroll helpers
    // (0x180289510); not modeled.
}

//=============================================================================
// CPreviewView::PositionPage
//=============================================================================
// Retail (0x180280bb0): computes the page display size (CalcPageDisplaySize),
// scales the page dimensions with MulDiv against the client metrics
// (0x200/0x204), writes the per-page state array (0x180) and tail-jumps to
// SetScaledSize.  The page array and the print-view setup virtuals are
// unmodeled.
// Symbol: ?PositionPage@CPreviewView@@MEAAXI@Z
extern "C" void MS_ABI impl__PositionPage_CPreviewView__MEAAXI_Z(
    void* pThis, unsigned int /*nPage*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180280bb0) drives
    // the page layout through the per-page array (0x180) and print-view
    // virtuals; not modeled.
}

//=============================================================================
// CPreviewView::SetScaledSize
//=============================================================================
// Retail (0x180280d30): recomputes the page display size and rescales the
// per-page state array entries (0x180) according to the zoom state; the array
// is unmodeled.
// Symbol: ?SetScaledSize@CPreviewView@@IEAAXI@Z
extern "C" void MS_ABI impl__SetScaledSize_CPreviewView__IEAAXI_Z(
    void* pThis, unsigned int /*nPage*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180280d30) rescales
    // the per-page array (0x180) via CalcPageDisplaySize and MulDiv; the array
    // is unmodeled.
}
