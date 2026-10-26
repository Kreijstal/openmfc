// Release x64 scroll-view sizing and scrollbar range/page updates.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "detail/AuxDataSupport.h"
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);

namespace {
int Add(int value,int amount) { return static_cast<int>(static_cast<UINT>(value)+static_cast<UINT>(amount)); }
int Subtract(int value,int amount) { return static_cast<int>(static_cast<UINT>(value)-static_cast<UINT>(amount)); }
}
void CScrollView::GetScrollBarSizes(SIZE& size) {
    size={0,0};
    DWORD style=GetStyle();
    if(!GetScrollBarCtrl(SB_VERT)) {
        size.cx=openmfc::detail::GetAuxData().cxVScroll;
        if(style&WS_BORDER)size.cx=Subtract(size.cx,1);
    }
    if(!GetScrollBarCtrl(SB_HORZ)) {
        size.cy=openmfc::detail::GetAuxData().cyHScroll;
        if(style&WS_BORDER)size.cy=Subtract(size.cy,1);
    }
}
BOOL CScrollView::GetTrueClientSize(SIZE& size,SIZE& scrollbars) {
    RECT rectangle{};
    ::GetClientRect(m_hWnd,&rectangle);
    size.cx=rectangle.right;
    size.cy=rectangle.bottom;
    DWORD style=GetStyle();
    GetScrollBarSizes(scrollbars);
    if(scrollbars.cx && (style&WS_VSCROLL))size.cx=Add(size.cx,scrollbars.cx);
    if(scrollbars.cy && (style&WS_HSCROLL))size.cy=Add(size.cy,scrollbars.cy);
    return size.cx>scrollbars.cx && size.cy>scrollbars.cy;
}
void CScrollView::GetDeviceScrollSizes(int& mode,SIZE& total,SIZE& page,SIZE& line) const {
    mode=m_nMapMode;
    total=m_totalDev;
    page=m_pageDev;
    line=m_lineDev;
}
void CScrollView::ScrollToDevicePosition(POINT point) {
    int oldX=GetScrollPos(SB_HORZ);
    SetScrollPos(SB_HORZ,point.x,TRUE);
    int oldY=GetScrollPos(SB_VERT);
    SetScrollPos(SB_VERT,point.y,TRUE);
    ScrollWindow(Subtract(oldX,point.x),Subtract(oldY,point.y),nullptr,nullptr);
}
void CScrollView::GetScrollBarState(CSize client,CSize& need,CSize& range,CPoint& move,BOOL inside) {
    SIZE scrollbars{};
    GetScrollBarSizes(scrollbars);
    range.cy=Subtract(m_totalDev.cy,client.cy);
    range.cx=Subtract(m_totalDev.cx,client.cx);
    move=GetDeviceScrollPosition();
    int originalX=range.cx;
    BOOL horizontal=originalX>0;
    if(!horizontal)move.x=0;
    else if(inside)range.cy=Add(range.cy,scrollbars.cy);
    BOOL vertical=range.cy>0;
    if(!vertical)move.y=0;
    else {
        if(inside)range.cx=Add(range.cx,scrollbars.cx);
        if(originalX<=0 && range.cx>0) {
            range.cy=Add(range.cy,scrollbars.cy);
            horizontal=TRUE;
        }
    }
    int maximum=range.cx;
    if(maximum>0 && move.x>=maximum)move.x=maximum;
    maximum=range.cy;
    if(maximum>0 && move.y>=maximum)move.y=maximum;
    need.cx=horizontal;
    need.cy=vertical;
}
void CScrollView::UpdateBars() {
    if(m_bInsideUpdate)return;
    m_bInsideUpdate=TRUE;
    RECT parentRectangle{};
    BOOL calculateClient=TRUE;
    CWnd* parent=impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(m_hWnd));
    if(parent)calculateClient=static_cast<int>(::SendMessageW(parent->m_hWnd,0x368,0,reinterpret_cast<LPARAM>(&parentRectangle)))==0;
    SIZE client{},scrollbars{};
    if(calculateClient) {
        if(!GetTrueClientSize(client,scrollbars)) {
            RECT rectangle{};
            ::GetClientRect(m_hWnd,&rectangle);
            if(rectangle.right>0 && rectangle.bottom>0)EnableScrollBarCtrl(SB_BOTH,FALSE);
            m_bInsideUpdate=FALSE;
            return;
        }
    } else {
        GetScrollBarSizes(scrollbars);
        client.cx=Subtract(parentRectangle.right,parentRectangle.left);
        client.cy=Subtract(parentRectangle.bottom,parentRectangle.top);
    }
    CSize need(0,0),range(0,0);
    CPoint move(0,0);
    GetScrollBarState(CSize(client),need,range,move,calculateClient);
    if(need.cx)client.cy=Subtract(client.cy,scrollbars.cy);
    BOOL vertical=need.cy;
    if(vertical)client.cx=Subtract(client.cx,scrollbars.cx);
    ScrollToDevicePosition(move);
    SCROLLINFO info;
    info.fMask=SIF_RANGE|SIF_PAGE;
    info.nMin=0;
    EnableScrollBarCtrl(SB_HORZ,need.cx);
    if(need.cx) {
        info.nMax=Subtract(m_totalDev.cx,1);
        info.nPage=static_cast<UINT>(client.cx);
        if(!SetScrollInfo(SB_HORZ,&info,TRUE))SetScrollRange(SB_HORZ,0,range.cx,TRUE);
    }
    EnableScrollBarCtrl(SB_VERT,vertical);
    if(vertical) {
        info.nMax=Subtract(m_totalDev.cy,1);
        info.nPage=static_cast<UINT>(client.cy);
        if(!SetScrollInfo(SB_VERT,&info,TRUE))SetScrollRange(SB_VERT,0,range.cy,TRUE);
    }
    m_bInsideUpdate=FALSE;
}
void CScrollView::SetScaleToFitSize(SIZE logical) {
    m_nMapMode=-1;
    m_totalLog=logical;
    if(m_hWnd && (GetStyle()&(WS_HSCROLL|WS_VSCROLL))) {
        SetScrollPos(SB_HORZ,0,TRUE);
        SetScrollPos(SB_VERT,0,TRUE);
        EnableScrollBarCtrl(SB_BOTH,FALSE);
    }
    RECT rectangle{};
    ::GetClientRect(m_hWnd,&rectangle);
    m_totalDev.cx=Subtract(rectangle.right,rectangle.left);
    m_totalDev.cy=Subtract(rectangle.bottom,rectangle.top);
    if(m_hWnd) {
        UpdateBars();
        ::InvalidateRect(m_hWnd,nullptr,TRUE);
    }
}
void CScrollView::OnSize(UINT,int,int) {
    Default();
    if(m_nMapMode==-1)SetScaleToFitSize(m_totalLog);
    else UpdateBars();
}
void CScrollView::CalcWindowRect(RECT* rectangle,UINT adjust) {
    if(adjust==1) {
        ::AdjustWindowRectEx(rectangle,0,FALSE,GetExStyle());
        if(m_nMapMode==-1)return;
        int horizontal=Add(Subtract(m_totalDev.cx,rectangle->right),rectangle->left);
        int vertical=Subtract(Add(m_totalDev.cy,rectangle->top),rectangle->bottom);
        SIZE scrollbars{};
        GetScrollBarSizes(scrollbars);
        if(vertical>0)rectangle->right=Add(rectangle->right,scrollbars.cx);
        if(horizontal>0)rectangle->bottom=Add(rectangle->bottom,scrollbars.cy);
    } else {
        DWORD extended=GetExStyle()&~WS_EX_CLIENTEDGE;
        DWORD style=GetStyle();
        ::AdjustWindowRectEx(rectangle,style,FALSE,extended);
    }
}

// Symbol: ?GetScrollBarSizes@CScrollView@@IEAAXAEAVCSize@@@Z
extern "C" void MS_ABI impl__GetScrollBarSizes_CScrollView__IEAAXAEAVCSize___Z(CScrollView* self,SIZE* size) {self->GetScrollBarSizes(*size);}
// Symbol: ?GetTrueClientSize@CScrollView@@IEAAHAEAVCSize@@0@Z
extern "C" BOOL MS_ABI impl__GetTrueClientSize_CScrollView__IEAAHAEAVCSize__0_Z(CScrollView* self,SIZE* size,SIZE* scrollbars) {return self->GetTrueClientSize(*size,*scrollbars);}
// Symbol: ?GetDeviceScrollSizes@CScrollView@@QEBAXAEAHAEAUtagSIZE@@11@Z
extern "C" void MS_ABI impl__GetDeviceScrollSizes_CScrollView__QEBAXAEAHAEAUtagSIZE__11_Z(const CScrollView* self,int* mode,SIZE* total,SIZE* page,SIZE* line) {self->GetDeviceScrollSizes(*mode,*total,*page,*line);}
// Symbol: ?ScrollToDevicePosition@CScrollView@@IEAAXUtagPOINT@@@Z
extern "C" void MS_ABI impl__ScrollToDevicePosition_CScrollView__IEAAXUtagPOINT___Z(CScrollView* self,POINT point) {self->ScrollToDevicePosition(point);}
// Symbol: ?GetScrollBarState@CScrollView@@IEAAXVCSize@@AEAV2@1AEAVCPoint@@H@Z
extern "C" void MS_ABI impl__GetScrollBarState_CScrollView__IEAAXVCSize__AEAV2_1AEAVCPoint__H_Z(CScrollView* self,CSize client,CSize* need,CSize* range,CPoint* move,BOOL inside) {self->GetScrollBarState(client,*need,*range,*move,inside);}
// Symbol: ?UpdateBars@CScrollView@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateBars_CScrollView__IEAAXXZ(CScrollView* self) {self->UpdateBars();}
// Symbol: ?SetScaleToFitSize@CScrollView@@QEAAXUtagSIZE@@@Z
extern "C" void MS_ABI impl__SetScaleToFitSize_CScrollView__QEAAXUtagSIZE___Z(CScrollView* self,SIZE size) {self->SetScaleToFitSize(size);}
// Symbol: ?OnSize@CScrollView@@QEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CScrollView__QEAAXIHH_Z(CScrollView* self,UINT type,int width,int height) {self->OnSize(type,width,height);}
// Symbol: ?CalcWindowRect@CScrollView@@UEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z(CScrollView* self,RECT* rectangle,UINT adjust) {self->CScrollView::CalcWindowRect(rectangle,adjust);}
