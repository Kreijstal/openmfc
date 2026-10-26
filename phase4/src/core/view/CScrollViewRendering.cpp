// Scroll-view DC mapping and the native offscreen print-client pipeline.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "detail/PrintBitmapSupport.h"
#include <exception>
extern "C" void MS_ABI impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CView*,CDC*,void*);
namespace {
int Subtract(int value,int amount) {return static_cast<int>(static_cast<UINT>(value)-static_cast<UINT>(amount));}
// CDC destruction retains its native noexcept boundary even when inlined.
void DestroyBuffer(CDC* dc,void* table) noexcept {
    *reinterpret_cast<void**>(dc)=table;
    if(dc->m_hDC)::DeleteDC(dc->Detach());
}
}
void CScrollView::OnPrepareDC(CDC* dc,void* info) {
    if(m_bInitialRedraw)return;
    if(m_nMapMode!=-1)dc->SetMapMode(m_nMapMode);
    else {
        dc->SetMapMode(MM_ANISOTROPIC);
        dc->SetWindowExt(m_totalLog.cx,m_totalLog.cy);
        dc->SetViewportExt(m_totalDev.cx,m_totalDev.cy);
    }
    int x=0,y=0;
    if(!dc->m_bPrinting) {
        CPoint position=GetDeviceScrollPosition();
        x=Subtract(0,position.x);y=Subtract(0,position.y);
        if(m_bCenter) {
            RECT rectangle{};
            ::GetClientRect(m_hWnd,&rectangle);
            int extent=Subtract(rectangle.right,rectangle.left);
            if(m_totalDev.cx<extent)x=Subtract(extent,m_totalDev.cx)/2;
            extent=Subtract(rectangle.bottom,rectangle.top);
            if(m_totalDev.cy<extent)y=Subtract(extent,m_totalDev.cy)/2;
        }
    }
    dc->SetViewportOrg(x,y);
    if(info)impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(this,dc,info);
}
LRESULT CScrollView::OnPrintClient(CDC* target,UINT) {
    if(!target || !target->m_hDC)return 0;
    alignas(CDC) BYTE storage[sizeof(CDC)];
    auto* buffer=new(storage) CDC;
    void* table=*reinterpret_cast<void**>(buffer);
    bool bufferAlive=true,imageAlive=false;
    openmfc::detail::PrintBitmap image;
    try {
        buffer->Attach(::CreateCompatibleDC(target->m_hDC));
        RECT rectangle{};
        ::GetClientRect(m_hWnd,&rectangle);
        image.Initialize();
        image.Create(Subtract(rectangle.right,rectangle.left),Subtract(rectangle.bottom,rectangle.top));
        HGDIOBJ previous=nullptr;
        if(image.Handle())previous=::SelectObject(buffer->m_hDC,image.Handle());
        ::BitBlt(buffer->m_hDC,0,0,Subtract(rectangle.right,rectangle.left),Subtract(rectangle.bottom,rectangle.top),target->m_hDC,0,0,SRCCOPY);
        buffer->CDC::SetViewportOrg(0,0);
        buffer->SetWindowOrg(0,0);
        buffer->CDC::SetMapMode(MM_TEXT);
        // Native /EHsc metadata starts image unwinding at the first C++
        // virtual callback. Earlier foreign API exceptions abandon the image.
        imageAlive=true;
        OnPrepareDC(buffer,nullptr);
        OnDraw(buffer);
        ::BitBlt(target->m_hDC,0,0,Subtract(rectangle.right,rectangle.left),Subtract(rectangle.bottom,rectangle.top),buffer->m_hDC,0,0,SRCCOPY);
        if(previous)::SelectObject(buffer->m_hDC,previous);
        imageAlive=false;image.Destroy();
        bufferAlive=false;DestroyBuffer(buffer,table);
    } catch(...) {
        try {
            if(imageAlive)image.Destroy();
            if(bufferAlive)DestroyBuffer(buffer,table);
        } catch(...) {std::terminate();}
        throw;
    }
    return 0;
}
// Symbol: ?OnPrepareDC@CScrollView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CScrollView* self,CDC* dc,void* info) {self->CScrollView::OnPrepareDC(dc,info);}
// Symbol: ?OnPrintClient@CScrollView@@QEAA_JPEAVCDC@@I@Z
extern "C" LRESULT MS_ABI impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(CScrollView* self,CDC* dc,UINT flags) {return self->OnPrintClient(dc,flags);}
