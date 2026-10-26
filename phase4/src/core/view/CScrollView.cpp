// CScrollView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(CView* pThis, RECT* rect, unsigned int adjustType);

// Destructor
// Symbol: ??1CScrollView@@UEAA@XZ
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}
// Symbol: ?GetRuntimeClass@CScrollView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CScrollView__UEBAPEAUCRuntimeClass__XZ(const CScrollView*) { return &CScrollView::classCScrollView; }
// Symbol: ?GetThisClass@CScrollView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CScrollView__SAPEAUCRuntimeClass__XZ() { return &CScrollView::classCScrollView; }
// CScrollView residuals.
// Symbol: ?sizeDefault@CScrollView@@2UtagSIZE@@B
extern "C" SIZE impl__sizeDefault_CScrollView__2UtagSIZE__B = {0, 0};
// Symbol: ?CenterOnPoint@CScrollView@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z(CScrollView* pThis, CPoint pt) { if (!pThis) return; RECT rc={}; if (pThis->m_hWnd) ::GetClientRect(pThis->m_hWnd,&rc); POINT dest={pt.x-(rc.right-rc.left)/2, pt.y-(rc.bottom-rc.top)/2}; pThis->ScrollToPosition(dest); }
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
CScrollView::CScrollView() { impl___0CScrollView__QEAA_XZ(this); }
CScrollView::~CScrollView() { impl___1CScrollView__UEAA_XZ(this); }
void CScrollView::SetScrollSizes(int nMapMode, const SIZE& sizeTotal, const SIZE& sizePage, const SIZE& sizeLine) { impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(this, nMapMode, &sizeTotal, &sizePage, &sizeLine); }
void CScrollView::OnInitialUpdate() { impl__OnInitialUpdate_CScrollView__UEAAXXZ(this); }
void CScrollView::OnDraw(void* pDC) { impl__OnDraw_CScrollView__UEAAXPEAX_Z(this, pDC); }
void CScrollView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
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

void CScrollView::OnPrepareDC(CDC* dc,void* info) { impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(this,dc,info); }
