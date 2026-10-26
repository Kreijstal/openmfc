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
// Symbol: ?HandleMButtonDown@CScrollView@@QEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleMButtonDown_CScrollView__QEAA_J_K_J_Z(CScrollView* pThis, uintptr_t nFlags, intptr_t point) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    const POINTS pts = MAKEPOINTS(point);
    return static_cast<intptr_t>(::SendMessageW(pThis->m_hWnd, WM_MBUTTONDOWN, nFlags, MAKELPARAM(pts.x, pts.y)));
}
CScrollView::CScrollView() { impl___0CScrollView__QEAA_XZ(this); }
CScrollView::~CScrollView() { impl___1CScrollView__UEAA_XZ(this); }
void CScrollView::OnInitialUpdate() { impl__OnInitialUpdate_CScrollView__UEAAXXZ(this); }
void CScrollView::OnDraw(void* pDC) { impl__OnDraw_CScrollView__UEAAXPEAX_Z(this, pDC); }
void CScrollView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
