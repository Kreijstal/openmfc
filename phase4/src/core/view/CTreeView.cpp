// CTreeView — OpenMFC implementation.
// Sources: docview.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"

// Symbol: ?CreateObject@CTreeView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTreeView__SAPEAVCObject__XZ() {
    return new CTreeView();
}
// Symbol: ?GetRuntimeClass@CTreeView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTreeView__UEBAPEAUCRuntimeClass__XZ(const CTreeView*) { return &CTreeView::classCTreeView; }
// Symbol: ?GetThisClass@CTreeView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTreeView__SAPEAUCRuntimeClass__XZ() { return &CTreeView::classCTreeView; }
// Symbol: ?GetMessageMap@CTreeView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTreeView__MEBAPEBUAFX_MSGMAP__XZ(const CTreeView*) { return &g_docviewEmptyMap; }
// Symbol: ?GetThisMessageMap@CTreeView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTreeView__KAPEBUAFX_MSGMAP__XZ() { return &g_docviewEmptyMap; }
// Symbol: ?OnDestroy@CTreeView@@QEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CTreeView__QEAAXXZ(CTreeView* pThis) { if (pThis) pThis->m_pTreeCtrl=nullptr; }
// Symbol: ?PreCreateWindow@CTreeView@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CTreeView__UEAAHAEAUtagCREATESTRUCTW___Z(CTreeView*, CREATESTRUCTW* cs) { if (cs) cs->lpszClass = WC_TREEVIEWW; return TRUE; }
// Symbol: ?RemoveImageList@CTreeView@@IEAAXH@Z
extern "C" void MS_ABI impl__RemoveImageList_CTreeView__IEAAXH_Z(CTreeView* pThis, int type) { if (pThis&&pThis->m_hWnd) TreeView_SetImageList(pThis->m_hWnd, nullptr, type); }
CTreeView::CTreeView() { impl___0CTreeView__QEAA_XZ(this); }
CTreeView::~CTreeView() { impl___1CTreeView__UEAA_XZ(this); }
CTreeCtrl* CTreeView::GetTreeCtrl() const { return impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(this); }
void CTreeView::OnDraw(void* pDC) { impl__OnDraw_CTreeView__UEAAXPEAX_Z(this, pDC); }
void CTreeView::OnInitialUpdate() { impl__OnInitialUpdate_CTreeView__UEAAXXZ(this); }
