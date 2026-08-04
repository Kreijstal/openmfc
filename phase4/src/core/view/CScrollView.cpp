// CScrollView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(CView* pThis, RECT* rect, unsigned int adjustType);

// Symbol: ?GetScrollBarSizes@CScrollView@@IEAAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetScrollBarSizes_CScrollView__IEAAXAEAVCSize___Z(
    CScrollView* pThis, SIZE* pSizeSb)
{
    if (!pSizeSb) {
        return;
    }
    if (pThis) {
        pThis->GetScrollBarSizes(*pSizeSb);
    } else {
        pSizeSb->cx = 0;
        pSizeSb->cy = 0;
    }
}
// Symbol: ?GetTrueClientSize@CScrollView@@IEAAHAEAVCSize@@0@Z
extern "C" int MS_ABI impl__GetTrueClientSize_CScrollView__IEAAHAEAVCSize__0_Z(
    const CScrollView* pThis, SIZE* pSize, SIZE* pSizeSb)
{
    if (!pSize || !pSizeSb) {
        return FALSE;
    }
    if (pThis) {
        pThis->GetTrueClientSize(*pSize, *pSizeSb);
        return TRUE;
    }
    pSize->cx = 0;
    pSize->cy = 0;
    pSizeSb->cx = 0;
    pSizeSb->cy = 0;
    return FALSE;
}
// Destructor
// Symbol: ??1CScrollView@@UEAA@XZ
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}
// Symbol: ?GetRuntimeClass@CScrollView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CScrollView__UEBAPEAUCRuntimeClass__XZ(const CScrollView*) { return &CScrollView::classCScrollView; }
// Symbol: ?GetThisClass@CScrollView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CScrollView__SAPEAUCRuntimeClass__XZ() { return &CScrollView::classCScrollView; }
// Symbol: ?GetMessageMap@CScrollView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CScrollView__MEBAPEBUAFX_MSGMAP__XZ(const CScrollView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CScrollView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CScrollView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// CScrollView residuals.
// Symbol: ?sizeDefault@CScrollView@@2UtagSIZE@@B
extern "C" SIZE impl__sizeDefault_CScrollView__2UtagSIZE__B = {0, 0};
// Symbol: ?CalcWindowRect@CScrollView@@UEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z(CScrollView* pThis, RECT* rect, unsigned int adjustType) { impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(pThis, rect, adjustType); }
// Symbol: ?GetDeviceScrollPosition@CScrollView@@QEBA?AVCPoint@@XZ
extern "C" void MS_ABI impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ(CPoint* ret, const CScrollView* pThis) { new (ret) CPoint(pThis ? pThis->GetScrollPosition() : CPoint(0, 0)); }
// Symbol: ?GetDeviceScrollSizes@CScrollView@@QEBAXAEAHAEAUtagSIZE@@11@Z
extern "C" void MS_ABI impl__GetDeviceScrollSizes_CScrollView__QEBAXAEAHAEAUtagSIZE__11_Z(const CScrollView* pThis, int* mapMode, SIZE* total, SIZE* page, SIZE* line) { if (mapMode) *mapMode=pThis?pThis->m_nMapMode:MM_TEXT; if (total) *total=pThis?pThis->m_totalLog:SIZE{0,0}; if (page) *page=pThis?pThis->m_pageDev:SIZE{0,0}; if (line) *line=pThis?pThis->m_lineDev:SIZE{0,0}; }
// Symbol: ?CheckScrollBars@CScrollView@@QEBAXAEAH0@Z
extern "C" void MS_ABI impl__CheckScrollBars_CScrollView__QEBAXAEAH0_Z(const CScrollView* pThis, int* needH, int* needV) { if (needH) *needH=pThis&&pThis->m_totalLog.cx>0; if (needV) *needV=pThis&&pThis->m_totalLog.cy>0; }
// Symbol: ?ScrollToDevicePosition@CScrollView@@IEAAXUtagPOINT@@@Z
extern "C" void MS_ABI impl__ScrollToDevicePosition_CScrollView__IEAAXUtagPOINT___Z(CScrollView* pThis, POINT pt) { if (pThis) pThis->ScrollToPosition(pt); }
// Symbol: ?CenterOnPoint@CScrollView@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z(CScrollView* pThis, CPoint pt) { if (!pThis) return; RECT rc={}; if (pThis->m_hWnd) ::GetClientRect(pThis->m_hWnd,&rc); POINT dest={pt.x-(rc.right-rc.left)/2, pt.y-(rc.bottom-rc.top)/2}; pThis->ScrollToPosition(dest); }
// Symbol: ?OnScrollBy@CScrollView@@UEAAHVCSize@@H@Z
extern "C" int MS_ABI impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(CScrollView* pThis, CSize size, int) { if (!pThis) return FALSE; CPoint pt=pThis->GetScrollPosition(); pt.x+=size.cx; pt.y+=size.cy; pThis->ScrollToPosition(pt); return TRUE; }
// Symbol: ?OnScroll@CScrollView@@UEAAHIIH@Z
extern "C" int MS_ABI impl__OnScroll_CScrollView__UEAAHIIH_Z(CScrollView* pThis, unsigned int code, unsigned int pos, int doScroll) { if (!pThis) return FALSE; CSize delta(0,0); if (code==SB_LINEUP) delta.cy=-pThis->m_lineDev.cy; else if (code==SB_LINEDOWN) delta.cy=pThis->m_lineDev.cy; else if (code==SB_PAGEUP) delta.cy=-pThis->m_pageDev.cy; else if (code==SB_PAGEDOWN) delta.cy=pThis->m_pageDev.cy; else if (code==SB_THUMBPOSITION||code==SB_THUMBTRACK) { CPoint pt=pThis->GetScrollPosition(); pt.y=pos; pThis->ScrollToPosition(pt); return TRUE; } return doScroll ? impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(pThis, delta, TRUE) : TRUE; }
// Symbol: ?OnHScroll@CScrollView@@QEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView* pThis, unsigned int code, unsigned int pos, CScrollBar*) { if (pThis) { CPoint pt=pThis->GetScrollPosition(); if (code==SB_LINELEFT) pt.x-=pThis->m_lineDev.cx; else if (code==SB_LINERIGHT) pt.x+=pThis->m_lineDev.cx; else if (code==SB_PAGELEFT) pt.x-=pThis->m_pageDev.cx; else if (code==SB_PAGERIGHT) pt.x+=pThis->m_pageDev.cx; else if (code==SB_THUMBPOSITION||code==SB_THUMBTRACK) pt.x=pos; pThis->ScrollToPosition(pt); } }
// Symbol: ?OnVScroll@CScrollView@@QEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView* pThis, unsigned int code, unsigned int pos, CScrollBar*) { impl__OnScroll_CScrollView__UEAAHIIH_Z(pThis, code, pos, TRUE); }
// Symbol: ?DoMouseWheel@CScrollView@@QEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* pThis, unsigned int, short zDelta, CPoint) { if (!pThis) return FALSE; CSize delta(0, zDelta < 0 ? pThis->m_lineDev.cy*3 : -pThis->m_lineDev.cy*3); return impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(pThis, delta, TRUE); }
// Symbol: ?OnMouseWheel@CScrollView@@QEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* pThis, unsigned int flags, short zDelta, CPoint pt) { return impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(pThis, flags, zDelta, pt); }
// Symbol: ?GetWheelScrollDistance@CScrollView@@UEAA?AVCSize@@V2@HH@Z
extern "C" void MS_ABI impl__GetWheelScrollDistance_CScrollView__UEAA_AVCSize__V2_HH_Z(CSize* ret, CScrollView*, CSize size, int, int) { new (ret) CSize(size.cx, size.cy * 3); }
// Symbol: ?OnSize@CScrollView@@QEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CScrollView__QEAAXIHH_Z(CScrollView* pThis, unsigned int, int, int) { if (pThis && pThis->m_hWnd) ::InvalidateRect(pThis->m_hWnd,nullptr,TRUE); }
// Symbol: ?UpdateBars@CScrollView@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateBars_CScrollView__IEAAXXZ(CScrollView* pThis) { if (pThis) pThis->SetScrollSizes(pThis->m_nMapMode, pThis->m_totalLog, pThis->m_pageDev, pThis->m_lineDev); }
// Symbol: ?GetScrollBarState@CScrollView@@IEAAXVCSize@@AEAV2@1AEAVCPoint@@H@Z
extern "C" void MS_ABI impl__GetScrollBarState_CScrollView__IEAAXVCSize__AEAV2_1AEAVCPoint__H_Z(CScrollView* pThis, CSize, CSize* needSb, CSize* range, CPoint* move, int) { if (needSb) *needSb=CSize(pThis&&pThis->m_totalLog.cx>0,pThis&&pThis->m_totalLog.cy>0); if (range) *range=CSize(pThis?pThis->m_totalLog.cx:0,pThis?pThis->m_totalLog.cy:0); if (move) *move=pThis?pThis->GetScrollPosition():CPoint(0,0); }
// Symbol: ?HandleMButtonDown@CScrollView@@QEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleMButtonDown_CScrollView__QEAA_J_K_J_Z(CScrollView* pThis, uintptr_t nFlags, intptr_t point) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    const POINTS pts = MAKEPOINTS(point);
    return static_cast<intptr_t>(::SendMessageW(pThis->m_hWnd, WM_MBUTTONDOWN, nFlags, MAKELPARAM(pts.x, pts.y)));
}
// Symbol: ?OnPrepareDC@CScrollView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CScrollView* pThis, CDC* dc, void*) { if (dc && dc->m_hDC && pThis) ::SetMapMode(dc->m_hDC, pThis->m_nMapMode); }
// Symbol: ?OnPrintClient@CScrollView@@QEAA_JPEAVCDC@@I@Z
extern "C" intptr_t MS_ABI impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(CScrollView* pThis, CDC* dc, unsigned int) { if (pThis) pThis->OnDraw(dc); return TRUE; }
// Symbol: ?SetScaleToFitSize@CScrollView@@QEAAXUtagSIZE@@@Z
extern "C" void MS_ABI impl__SetScaleToFitSize_CScrollView__QEAAXUtagSIZE___Z(CScrollView* pThis, SIZE size) { if (pThis) pThis->SetScrollSizes(MM_TEXT, size, size, SIZE{1,1}); }
CScrollView::CScrollView() { impl___0CScrollView__QEAA_XZ(this); }
CScrollView::~CScrollView() { impl___1CScrollView__UEAA_XZ(this); }
void CScrollView::SetScrollSizes(int nMapMode, const SIZE& sizeTotal, const SIZE& sizePage, const SIZE& sizeLine) { impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(this, nMapMode, &sizeTotal, &sizePage, &sizeLine); }
void CScrollView::OnInitialUpdate() { impl__OnInitialUpdate_CScrollView__UEAAXXZ(this); }
void CScrollView::OnDraw(void* pDC) { impl__OnDraw_CScrollView__UEAAXPEAX_Z(this, pDC); }
void CScrollView::OnUpdate(CView* pSender, unsigned long lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
void CScrollView::GetScrollBarSizes(SIZE& sizeSb) {
    sizeSb.cx = ::GetSystemMetrics(SM_CXVSCROLL);
    sizeSb.cy = ::GetSystemMetrics(SM_CYHSCROLL);
}
void CScrollView::GetTrueClientSize(SIZE& size, SIZE& sizeSb) const {
    if (m_hWnd) {
        RECT rc;
        ::GetClientRect(m_hWnd, &rc);
        size.cx = static_cast<int>(rc.right - rc.left);
        size.cy = static_cast<int>(rc.bottom - rc.top);
    } else {
        size.cx = 0;
        size.cy = 0;
    }
    sizeSb.cx = ::GetSystemMetrics(SM_CXVSCROLL);
    sizeSb.cy = ::GetSystemMetrics(SM_CYHSCROLL);
}
void CScrollView::ScrollToPosition(POINT pt) {
    if (!m_hWnd) return;

    // Clamp to valid range
    if (pt.x < 0) pt.x = 0;
    if (pt.y < 0) pt.y = 0;
    if (pt.x > m_totalLog.cx) pt.x = m_totalLog.cx;
    if (pt.y > m_totalLog.cy) pt.y = m_totalLog.cy;

    // Set scroll positions
    ::SetScrollPos(m_hWnd, SB_HORZ, pt.x, TRUE);
    ::SetScrollPos(m_hWnd, SB_VERT, pt.y, TRUE);

    // Scroll the window content
    ::ScrollWindow(m_hWnd, 0, 0, nullptr, nullptr);
    ::InvalidateRect(m_hWnd, nullptr, TRUE);
}
CPoint CScrollView::GetScrollPosition() const {
    if (!m_hWnd) {
        return CPoint(0, 0);
    }
    return CPoint(::GetScrollPos(m_hWnd, SB_HORZ), ::GetScrollPos(m_hWnd, SB_VERT));
}
void CScrollView::FillOutsideRect(void* pDC, void* pBrush) {
    if (!m_hWnd || !pDC) return;

    HDC hDC = static_cast<HDC>(pDC);
    HBRUSH hBrush = pBrush ? static_cast<HBRUSH>(pBrush) :
                            static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));

    RECT rcClient;
    ::GetClientRect(m_hWnd, &rcClient);

    // Fill areas outside the scroll view content
    RECT rcFill;

    // Right of content
    if (rcClient.right > m_totalLog.cx) {
        rcFill.left = m_totalLog.cx;
        rcFill.top = rcClient.top;
        rcFill.right = rcClient.right;
        rcFill.bottom = rcClient.bottom;
        ::FillRect(hDC, &rcFill, hBrush);
    }

    // Below content
    if (rcClient.bottom > m_totalLog.cy) {
        rcFill.left = rcClient.left;
        rcFill.top = m_totalLog.cy;
        rcFill.right = m_totalLog.cx;
        rcFill.bottom = rcClient.bottom;
        ::FillRect(hDC, &rcFill, hBrush);
    }
}
void CScrollView::ResizeParentToFit(int bShrinkOnly) {
    if (!m_hWnd) return;

    HWND hWndParent = ::GetParent(m_hWnd);
    if (!hWndParent) return;

    // Calculate desired parent size based on scroll view content
    SIZE sizeSb;
    GetScrollBarSizes(sizeSb);

    int nWidth = m_totalLog.cx + sizeSb.cx +
                 ::GetSystemMetrics(SM_CXFRAME) * 2;
    int nHeight = m_totalLog.cy + sizeSb.cy +
                  ::GetSystemMetrics(SM_CYFRAME) * 2 +
                  ::GetSystemMetrics(SM_CYCAPTION);

    RECT rcParent;
    ::GetWindowRect(hWndParent, &rcParent);
    int nCurrentWidth = rcParent.right - rcParent.left;
    int nCurrentHeight = rcParent.bottom - rcParent.top;

    // Apply shrink-only constraint
    if (bShrinkOnly) {
        if (nWidth > nCurrentWidth) nWidth = nCurrentWidth;
        if (nHeight > nCurrentHeight) nHeight = nCurrentHeight;
    }

    ::SetWindowPos(hWndParent, nullptr, 0, 0, nWidth, nHeight,
                   SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
