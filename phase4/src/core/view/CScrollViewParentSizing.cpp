// Native parent-frame sizing from the scroll view's device extents.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
int Add(int value,int amount) {return static_cast<int>(static_cast<UINT>(value)+static_cast<UINT>(amount));}
int Subtract(int value,int amount) {return static_cast<int>(static_cast<UINT>(value)-static_cast<UINT>(amount));}
}
void CScrollView::ResizeParentToFit(BOOL shrinkOnly) {
    RECT client{};
    ::GetWindowRect(m_hWnd,&client);
    RECT adjusted=client;
    CalcWindowRect(&adjusted,0);
    client.left=Subtract(Add(client.left,client.left),adjusted.left);
    client.top=Subtract(Add(client.top,client.top),adjusted.top);
    client.right=Subtract(Add(client.right,client.right),adjusted.right);
    client.bottom=Subtract(Add(client.bottom,client.bottom),adjusted.bottom);
    ::OffsetRect(&client,Subtract(0,client.left),Subtract(0,client.top));

    RECT desired{0,0,m_totalDev.cx,m_totalDev.cy};
    if(shrinkOnly) {
        if(client.right<=desired.right)desired.right=client.right;
        if(client.bottom<=desired.bottom)desired.bottom=client.bottom;
    }
    CalcWindowRect(&desired,1);
    ::OffsetRect(&desired,Subtract(0,desired.left),Subtract(0,desired.top));
    if(shrinkOnly) {
        // The second virtual calculation can change the device extents.
        if(client.right<=m_totalDev.cx)desired.right=client.right;
        if(client.bottom<=m_totalDev.cy)desired.bottom=client.bottom;
    }
    RECT rectangle{};
    CFrameWnd* frame=CWnd::GetParentFrame();
    if(!frame)impl__AfxThrowInvalidArgException__YAXXZ();
    ::GetWindowRect(frame->m_hWnd,&rectangle);
    frame->SetWindowPos(nullptr,0,0,
        Add(Subtract(Subtract(rectangle.right,rectangle.left),client.right),desired.right),
        Add(Subtract(Subtract(rectangle.bottom,rectangle.top),client.bottom),desired.bottom),
        SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}
// Symbol: ?ResizeParentToFit@CScrollView@@QEAAXH@Z
extern "C" void MS_ABI impl__ResizeParentToFit_CScrollView__QEAAXH_Z(CScrollView* self,BOOL shrinkOnly) {
    self->ResizeParentToFit(shrinkOnly);
}
