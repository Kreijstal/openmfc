// CListView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"

// Symbol: ?CreateObject@CListView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CListView__SAPEAVCObject__XZ() {
    return new CListView();
}
// Symbol: ?GetRuntimeClass@CListView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CListView__UEBAPEAUCRuntimeClass__XZ(const CListView*) { return &CListView::classCListView; }
// Symbol: ?GetThisClass@CListView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CListView__SAPEAUCRuntimeClass__XZ() { return &CListView::classCListView; }
// Symbol: ?GetMessageMap@CListView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CListView__MEBAPEBUAFX_MSGMAP__XZ(const CListView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CListView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CListView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// CListView/CTreeView residuals.
// Symbol: ?DrawItem@CListView@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CListView__UEAAXPEAUtagDRAWITEMSTRUCT___Z(CListView* pThis, DRAWITEMSTRUCT* pDis) {
    if (!pThis || !pDis || !pDis->hDC) return;
    HDC hdc = pDis->hDC;
    const COLORREF bk = (pDis->itemState & ODS_SELECTED) ? RGB(196, 220, 255) : RGB(255, 255, 255);
    HBRUSH brush = ::CreateSolidBrush(bk);
    if (brush) {
        ::FillRect(hdc, &pDis->rcItem, brush);
        ::DeleteObject(brush);
    }
    if ((pDis->itemState & ODS_FOCUS) && pDis->hwndItem) {
        ::DrawFocusRect(hdc, &pDis->rcItem);
    }
    if (!pThis->m_pListCtrl && pDis->hwndItem) {
        pThis->m_pListCtrl = (CListCtrl*)CWnd::FromHandle(pDis->hwndItem);
    }
}
// Symbol: ?OnChildNotify@CListView@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CListView__MEAAHI_K_JPEA_J_Z(CListView* pThis, unsigned int p0, unsigned long long p1, long long p2, long long* p3) {
    if (!pThis) return FALSE;
    return (int)pThis->OnChildNotify(p0, p1, p2, p3);
}
// Symbol: ?OnNcDestroy@CListView@@QEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CListView__QEAAXXZ(CListView* pThis) { if (pThis) pThis->m_pListCtrl=nullptr; }
// Symbol: ?PreCreateWindow@CListView@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CListView__UEAAHAEAUtagCREATESTRUCTW___Z(CListView*, CREATESTRUCTW* cs) { if (cs) cs->lpszClass = WC_LISTVIEWW; return TRUE; }
// Symbol: ?RemoveImageList@CListView@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CListView__IEAAXH_Z(CListView* pThis, int type) { if (pThis&&pThis->m_hWnd) ListView_SetImageList(pThis->m_hWnd, nullptr, type); }
CListView::CListView() { impl___0CListView__QEAA_XZ(this); }
CListView::~CListView() { impl___1CListView__UEAA_XZ(this); }
CListCtrl* CListView::GetListCtrl() const { return impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(this); }
void CListView::OnDraw(void* pDC) { impl__OnDraw_CListView__UEAAXPEAX_Z(this, pDC); }
void CListView::OnInitialUpdate() { impl__OnInitialUpdate_CListView__UEAAXXZ(this); }
