// CView — OpenMFC implementation.
// Sources: docview.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DocviewSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Destructor
// Symbol: ??1CView@@UEAA@XZ
extern "C" void MS_ABI impl___1CView__UEAA_XZ(CView* pThis) {
    if (!pThis) return;
    if (pThis->m_pDocument) {
        pThis->m_pDocument->RemoveView(pThis);
    }
    ForgetView(pThis);
}
// OnInitialUpdate
// Symbol: ?OnInitialUpdate@CView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CView__UEAAXXZ(CView* pThis) {
    if (pThis) {
        pThis->OnUpdate(nullptr, 0, nullptr);
    }
}
// Symbol: ?OnPreparePrinting@CView@@MEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__OnPreparePrinting_CView__MEAAHPEAUCPrintInfo___Z(
    CView* pThis, void* pInfo)
{
    return impl__OnPreparePrinting_CView__UEAAHPEAX_Z(pThis, pInfo);
}
// Symbol: ?OnBeginPrinting@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnBeginPrinting_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CView* pThis, void* pDC, void* pInfo)
{
    impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(pThis, pDC, pInfo);
}
// Symbol: ?OnEndPrinting@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnEndPrinting_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CView* pThis, void* pDC, void* pInfo)
{
    impl__OnEndPrinting_CView__UEAAXPEAX0_Z(pThis, pDC, pInfo);
}
// Symbol: ?OnActivateView@CView@@MEAAXHPEAV1@0@Z
extern "C" void MS_ABI impl__OnActivateView_CView__MEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView)
{
    impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(pThis, bActivate, pActivateView, pDeactiveView);
}
// Symbol: ?GetRuntimeClass@CView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CView__UEBAPEAUCRuntimeClass__XZ(const CView*) { return &CView::classCView; }
// Symbol: ?GetThisClass@CView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CView__SAPEAUCRuntimeClass__XZ() { return &CView::classCView; }
// CView residuals.
// Symbol: ?CalcWindowRect@CView@@UEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CView__UEAAXPEAUtagRECT__I_Z(CView* pThis, RECT* rect, unsigned int adjustType) {
    if (!pThis || !rect) return;
    DWORD style = pThis->m_hWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pThis->m_hWnd, GWL_STYLE)) : WS_CHILD;
    DWORD exStyle = pThis->m_hWnd ? static_cast<DWORD>(::GetWindowLongPtrW(pThis->m_hWnd, GWL_EXSTYLE)) : 0;
    BOOL hasMenu = (adjustType != 0 && pThis->m_hWnd && ::GetMenu(pThis->m_hWnd));
    ::AdjustWindowRectEx(rect, style, hasMenu, exStyle);
}
// Symbol: ?DoPreparePrinting@CView@@QEAAHPEAUCPrintInfo@@@Z
extern "C" int MS_ABI impl__DoPreparePrinting_CView__QEAAHPEAUCPrintInfo___Z(CView* pThis, void* info) { return pThis ? pThis->OnPreparePrinting(info) : FALSE; }
// Symbol: ?DoPrintPreview@CView@@QEAAHIPEAV1@PEAUCRuntimeClass@@PEAUCPrintPreviewState@@@Z
extern "C" int MS_ABI impl__DoPrintPreview_CView__QEAAHIPEAV1_PEAUCRuntimeClass__PEAUCPrintPreviewState___Z(CView* pThis, unsigned int, CView* previewView, CRuntimeClass*, void*) { if (pThis && pThis->m_hWnd) ::ShowWindow(pThis->m_hWnd, SW_HIDE); if (previewView && previewView->m_hWnd) ::ShowWindow(previewView->m_hWnd, SW_SHOW); return previewView != nullptr; }
// Symbol: ?IsSelected@CView@@UEBAHPEBVCObject@@@Z
extern "C" int MS_ABI impl__IsSelected_CView__UEBAHPEBVCObject___Z(const CView* pThis, const CObject* pObject) {
    return (pThis && pObject && (pObject == pThis || pObject == pThis->m_pDocument)) ? TRUE : FALSE;
}
// Symbol: ?OnActivateFrame@CView@@MEAAXIPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(CView* pThis, unsigned int, CFrameWnd*) { if (pThis && pThis->m_hWnd) ::SetFocus(pThis->m_hWnd); }
// Native command routing lives in core/runtime/CommandRouting.cpp.
// Symbol: ?OnCreate@CView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(CView* pThis, CREATESTRUCTW* pCreateStruct) {
    if (!pThis) {
        return -1;
    }

    if (pCreateStruct) {
        pCreateStruct->style |= WS_CHILD | WS_VISIBLE;
        pCreateStruct->style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        pCreateStruct->style &= ~WS_POPUP;
    }

    if (pThis->m_hWnd) {
        DWORD style = static_cast<DWORD>(::GetWindowLongPtrW(pThis->m_hWnd, GWL_STYLE));
        style |= static_cast<DWORD>(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        style &= static_cast<DWORD>(~WS_POPUP);
        ::SetWindowLongPtrW(pThis->m_hWnd, GWL_STYLE, static_cast<LONG_PTR>(style));
    }

    return 0;
}
// Symbol: ?OnDestroy@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CView__IEAAXXZ(CView* pThis) { if (pThis && pThis->m_pDocument) pThis->m_pDocument->RemoveView(pThis); }
// Symbol: ?OnDragLeave@CView@@UEAAXXZ
extern "C" void MS_ABI impl__OnDragLeave_CView__UEAAXXZ(CView* pThis) { if (pThis && pThis->m_hWnd && ::GetCapture() == pThis->m_hWnd) ::ReleaseCapture(); }
// Symbol: ?OnDragEnter@CView@@UEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragEnter_CView__UEAAKPEAVCOleDataObject__KVCPoint___Z(CView* pThis, COleDataObject* p0, unsigned long p1, CPoint p2) {
    return (unsigned long)pThis->OnDragEnter(p0, p1, p2);
}
// Symbol: ?OnDragOver@CView@@UEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CView__UEAAKPEAVCOleDataObject__KVCPoint___Z(CView* pThis, COleDataObject* p0, unsigned long p1, CPoint p2) {
    return (unsigned long)pThis->OnDragOver(p0, p1, p2);
}
// Symbol: ?OnDrop@CView@@UEAAHPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" int MS_ABI impl__OnDrop_CView__UEAAHPEAVCOleDataObject__KVCPoint___Z(CView* pThis, COleDataObject* p0, unsigned long p1, CPoint p2) {
    return (int)pThis->OnDrop(p0, p1, p2);
}
// Symbol: ?OnDropEx@CView@@UEAAKPEAVCOleDataObject@@KKVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDropEx_CView__UEAAKPEAVCOleDataObject__KKVCPoint___Z(CView* pThis, COleDataObject* p0, unsigned long p1, unsigned long p2, CPoint p3) {
    return (unsigned long)pThis->OnDropEx(p0, p1, p2, p3);
}
// Symbol: ?OnDragScroll@CView@@UEAAKKVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragScroll_CView__UEAAKKVCPoint___Z(CView* pThis, unsigned long keyState, CPoint point) {
    if (!pThis || !pThis->m_hWnd) {
        return DROPEFFECT_NONE;
    }

    if ((keyState & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0) {
        return DROPEFFECT_NONE;
    }

    RECT client = {};
    if (!::GetClientRect(pThis->m_hWnd, &client)) {
        return DROPEFFECT_NONE;
    }

    constexpr int kEdge = 16;
    int dy = 0;
    int step = (keyState & MK_CONTROL) ? 3 : 1;
    int line = 16 * step;

    if (point.y <= client.top + kEdge) {
        dy = line;
    } else if (point.y >= client.bottom - kEdge) {
        dy = -line;
    }

    if (dy != 0) {
        ::ScrollWindowEx(pThis->m_hWnd, 0, dy, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE | SW_SCROLLCHILDREN | SW_ERASE);
        ::UpdateWindow(pThis->m_hWnd);
        return DROPEFFECT_SCROLL;
    }

    return DROPEFFECT_NONE;
}
// Symbol: ?OnDraw@CView@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CView__MEAAXPEAVCDC___Z(CView* pThis, CDC* dc) { if (pThis && dc && dc->m_hDC && pThis->m_pDocument) { RECT rc = {0, 0, 10000, 10000}; ::DrawTextW(dc->m_hDC, pThis->m_pDocument->GetTitle(), -1, &rc, DT_LEFT | DT_TOP | DT_SINGLELINE); } }
// Symbol: ?OnEndPrintPreview@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@UtagPOINT@@PEAVCPreviewView@@@Z
extern "C" void MS_ABI impl__OnEndPrintPreview_CView__MEAAXPEAVCDC__PEAUCPrintInfo__UtagPOINT__PEAVCPreviewView___Z(CView* pThis, CDC*, void*, POINT, void*) { if (pThis && pThis->m_hWnd) ::ShowWindow(pThis->m_hWnd, SW_SHOW); }
// Symbol: ?OnFilePrint@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnFilePrint_CView__IEAAXXZ(CView* pThis) { if (pThis && pThis->m_hWnd) ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE); }
// Symbol: ?OnFilePrintPreview@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnFilePrintPreview_CView__IEAAXXZ(CView* pThis) { if (pThis && pThis->m_hWnd) ::ShowWindow(pThis->m_hWnd, SW_SHOW); }
// Symbol: ?OnMouseActivate@CView@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CView__IEAAHPEAVCWnd__II_Z(CView*, CWnd*, unsigned int, unsigned int) { return MA_ACTIVATE; }
// Symbol: ?OnScroll@CView@@UEAAHIIH@Z
extern "C" int MS_ABI impl__OnScroll_CView__UEAAHIIH_Z(CView* pThis, unsigned int scrollCode, unsigned int pos, int doScroll) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    int delta = 0;
    switch (scrollCode) {
    case SB_LINEUP: delta = -16; break;
    case SB_LINEDOWN: delta = 16; break;
    case SB_PAGEUP: delta = -64; break;
    case SB_PAGEDOWN: delta = 64; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK: delta = static_cast<int>(pos); break;
    default: return FALSE;
    }
    if (doScroll) {
        ::ScrollWindowEx(pThis->m_hWnd, 0, -delta, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE | SW_ERASE);
        ::UpdateWindow(pThis->m_hWnd);
    }
    return TRUE;
}
// Symbol: ?OnScrollBy@CView@@UEAAHVCSize@@H@Z
extern "C" int MS_ABI impl__OnScrollBy_CView__UEAAHVCSize__H_Z(CView* pThis, CSize size, int doScroll) {
    if (!pThis || !pThis->m_hWnd) return FALSE;
    if (doScroll) {
        ::ScrollWindowEx(pThis->m_hWnd, -size.cx, -size.cy, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE | SW_ERASE);
        ::UpdateWindow(pThis->m_hWnd);
    }
    return TRUE;
}
// Symbol: ?PostNcDestroy@CView@@MEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CView__MEAAXXZ(CView* pThis) {
    if (pThis && pThis->m_pDocument) pThis->m_pDocument->RemoveView(pThis);
}
// Symbol: ?PreCreateWindow@CView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(CView*, CREATESTRUCTW* cs) {
    if (!cs) return FALSE;
    cs->style |= WS_CHILD | WS_VISIBLE;
    if (!cs->lpszClass) cs->lpszClass = L"OpenMFCView";
    return TRUE;
}
CView::CView() { impl___0CView__QEAA_XZ(this); }
CView::~CView() { impl___1CView__UEAA_XZ(this); }
CDocument* CView::GetDocument() const { return impl__GetDocument_CView__UEBAPEAVCDocument__XZ(this); }
void CView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
void CView::OnInitialUpdate() { impl__OnInitialUpdate_CView__UEAAXXZ(this); }
int CView::OnPreparePrinting(void* pInfo) { return impl__OnPreparePrinting_CView__UEAAHPEAX_Z(this, pInfo); }
void CView::OnBeginPrinting(void* pDC, void* pInfo) { impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(this, pDC, pInfo); }
void CView::OnEndPrinting(void* pDC, void* pInfo) { impl__OnEndPrinting_CView__UEAAXPEAX0_Z(this, pDC, pInfo); }
void CView::OnActivateView(int bActivate, CView* pActivateView, CView* pDeactiveView) { impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(this, bActivate, pActivateView, pDeactiveView); }
CFrameWnd* CView::GetParentFrame() const { return impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(this); }
DWORD CView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
    (void)point;
    if (!pDataObject || !pDataObject->GetIDataObject(FALSE)) return DROPEFFECT_NONE;
    return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}
DWORD CView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) {
    return OnDragEnter(pDataObject, dwKeyState, point);
}
BOOL CView::OnDrop(COleDataObject* pDataObject, DWORD dropEffect, CPoint point) {
    (void)point;
    return pDataObject && pDataObject->GetIDataObject(FALSE) && dropEffect != DROPEFFECT_NONE;
}
DWORD CView::OnDropEx(COleDataObject* pDataObject, DWORD dropDefault, DWORD /*dropList*/, CPoint point) {
    return OnDrop(pDataObject, dropDefault, point);
}
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void*);
// Symbol: ??0CView@@IEAA@XZ
// CView::CView()
extern "C" void* MS_ABI impl___0CView__IEAA_XZ(void* self) {
    impl___0CWnd__QEAA_XZ(self);
    impl___0CView__QEAA_XZ(static_cast<CView*>(self));
    return self;
}

// Symbol: ?OnUpdate@CView@@MEAAXPEAV1@_JPEAVCObject@@@Z
extern "C" void MS_ABI impl__OnUpdate_CView__MEAAXPEAV1__JPEAVCObject___Z(CView* self,CView* sender,LPARAM hint,CObject* object) {
    impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(self,sender,hint,object);
}

BOOL CView::IsSelected(const CObject* item) const { return impl__IsSelected_CView__UEBAHPEBVCObject___Z(this,item); }
BOOL CView::OnScroll(UINT code,UINT pos,BOOL scroll) { return impl__OnScroll_CView__UEAAHIIH_Z(this,code,pos,scroll); }
BOOL CView::OnScrollBy(CSize size,BOOL scroll) { return impl__OnScrollBy_CView__UEAAHVCSize__H_Z(this,size,scroll); }
void CView::OnDragLeave() { impl__OnDragLeave_CView__UEAAXXZ(this); }
DWORD CView::OnDragScroll(DWORD keys,CPoint point) { return impl__OnDragScroll_CView__UEAAKKVCPoint___Z(this,keys,point); }
void CView::OnActivateFrame(UINT state,CFrameWnd* frame) { impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(this,state,frame); }
void CView::OnEndPrintPreview(CDC* dc,void* info,POINT point,CPreviewView* preview) { impl__OnEndPrintPreview_CView__MEAAXPEAVCDC__PEAUCPrintInfo__UtagPOINT__PEAVCPreviewView___Z(this,dc,info,point,preview); }
