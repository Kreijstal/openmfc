// Release-x64 CView painting and print-client behavior.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"

// Symbol: ?OnPaint@CView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CView__IEAAXXZ(CView* self) {
    CPaintDC dc(self);
    self->OnPrepareDC(&dc,nullptr);
    self->OnDraw(&dc);
}
// Symbol: ?OnPrepareDC@CView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CView*,CDC*,void* info) {
    if(!info)return;
    // CPrintInfo::m_pPD, m_bContinuePrinting and m_nCurPage; the dialog's
    // m_pd reference is stored at +0x130. Native code leaves the DC untouched.
    auto* bytes=static_cast<BYTE*>(info);
    auto* dialog=*reinterpret_cast<BYTE**>(bytes);
    const auto* parameters=*reinterpret_cast<const PRINTDLGW**>(dialog+0x130);
    const WORD maximum=parameters->nMaxPage;
    const UINT page=*reinterpret_cast<UINT*>(bytes+0x18);
    *reinterpret_cast<BOOL*>(bytes+0x14)=maximum!=0xffff || page==1;
}
// Symbol: ?OnPrint@CView@@MEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CView* self,CDC* dc,void*) { self->OnDraw(dc); }
// Symbol: ?OnPrintClient@CView@@IEAA_JPEAVCDC@@I@Z
extern "C" LRESULT MS_ABI impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z(CView* self,CDC* dc,UINT flags) {
    if(flags&PRF_ERASEBKGND)::SendMessageW(self->m_hWnd,WM_ERASEBKGND,reinterpret_cast<WPARAM>(dc?dc->m_hDC:nullptr),0);
    if(flags&PRF_CLIENT)self->OnDraw(dc);
    return 0;
}
void CView::OnPrepareDC(CDC* dc,void* info) { impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(this,dc,info); }
void CView::OnPrint(CDC* dc,void* info) { impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(this,dc,info); }
