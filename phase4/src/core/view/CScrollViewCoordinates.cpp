// Release x64 logical/device scroll conversion and outside-content painting.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
namespace {
int Add(int value,int amount) {return static_cast<int>(static_cast<UINT>(value)+static_cast<UINT>(amount));}
int Subtract(int value,int amount) {return static_cast<int>(static_cast<UINT>(value)-static_cast<UINT>(amount));}
int PositiveY(int value) {return value<0?Subtract(0,value):value;}
}
void CScrollView::SetScrollSizes(int mode,SIZE total,const SIZE& page,const SIZE& line) {
    int previousMode=m_nMapMode;
    m_nMapMode=mode;
    m_totalLog=total;
    {
        // Native metadata has no DC unwind action across these Win32 calls.
        // End the lifetime explicitly on the normal path, as the binary does.
        alignas(CWindowDC) BYTE storage[sizeof(CWindowDC)];
        auto* dc=new(storage) CWindowDC(nullptr);
        dc->CDC::SetMapMode(m_nMapMode);
        m_totalDev=m_totalLog;
        ::LPtoDP(dc->m_hAttribDC,reinterpret_cast<POINT*>(&m_totalDev),1);
        m_pageDev=page;
        ::LPtoDP(dc->m_hAttribDC,reinterpret_cast<POINT*>(&m_pageDev),1);
        m_lineDev=line;
        ::LPtoDP(dc->m_hAttribDC,reinterpret_cast<POINT*>(&m_lineDev),1);
        m_totalDev.cy=PositiveY(m_totalDev.cy);
        m_pageDev.cy=PositiveY(m_pageDev.cy);
        m_lineDev.cy=PositiveY(m_lineDev.cy);
        dc->CWindowDC::~CWindowDC();
    }
    if(!m_pageDev.cx)m_pageDev.cx=m_totalDev.cx/10;
    if(!m_pageDev.cy)m_pageDev.cy=m_totalDev.cy/10;
    if(!m_lineDev.cx)m_lineDev.cx=m_pageDev.cx/10;
    if(!m_lineDev.cy)m_lineDev.cy=m_pageDev.cy/10;
    if(m_hWnd) {
        UpdateBars();
        if(previousMode!=m_nMapMode)::InvalidateRect(m_hWnd,nullptr,TRUE);
    }
}
CPoint CScrollView::GetScrollPosition() const {
    if(m_nMapMode==-1)return CPoint(0,0);
    CPoint device=GetDeviceScrollPosition();
    POINT point{device.x,device.y};
    if(m_bCenter) {
        RECT rectangle{};
        ::GetClientRect(m_hWnd,&rectangle);
        int extent=Subtract(rectangle.right,rectangle.left);
        if(m_totalDev.cx<extent)point.x=Subtract(0,Subtract(extent,m_totalDev.cx)/2);
        extent=Subtract(rectangle.bottom,rectangle.top);
        if(m_totalDev.cy<extent)point.y=Subtract(0,Subtract(extent,m_totalDev.cy)/2);
    }
    if(m_nMapMode!=MM_TEXT) {
        alignas(CWindowDC) BYTE storage[sizeof(CWindowDC)];
        auto* dc=new(storage) CWindowDC(nullptr);
        dc->CDC::SetMapMode(m_nMapMode);
        ::DPtoLP(dc->m_hAttribDC,&point,1);
        dc->CWindowDC::~CWindowDC();
    }
    return CPoint(point.x,point.y);
}
void CScrollView::ScrollToPosition(POINT point) {
    if(m_nMapMode!=MM_TEXT) {
        alignas(CWindowDC) BYTE storage[sizeof(CWindowDC)];
        auto* dc=new(storage) CWindowDC(nullptr);
        dc->CDC::SetMapMode(m_nMapMode);
        ::LPtoDP(dc->m_hAttribDC,&point,1);
        dc->CWindowDC::~CWindowDC();
    }
    int horizontal=GetScrollLimit(SB_HORZ);
    int vertical=GetScrollLimit(SB_VERT);
    if(point.x<0)point.x=0;
    else if(point.x>horizontal)point.x=horizontal;
    if(point.y<0)point.y=0;
    else if(point.y>vertical)point.y=vertical;
    ScrollToDevicePosition(point);
}
void CScrollView::CenterOnPoint(CPoint point) {
    RECT rectangle{};
    ::GetClientRect(m_hWnd,&rectangle);
    int width=Subtract(rectangle.right,rectangle.left);
    int height=Subtract(rectangle.bottom,rectangle.top);
    DWORD style=GetStyle();
    int horizontal=0,vertical=0;
    if(style&WS_HSCROLL) {
        horizontal=Subtract(point.x,width/2);
        if(horizontal<0)horizontal=0;
        else {int maximum=GetScrollLimit(SB_HORZ);if(horizontal>maximum)horizontal=maximum;}
    }
    if(style&WS_VSCROLL) {
        vertical=Subtract(point.y,height/2);
        if(vertical<0)vertical=0;
        else {int maximum=GetScrollLimit(SB_VERT);if(vertical>maximum)vertical=maximum;}
    }
    SetScrollPos(SB_HORZ,horizontal,TRUE);
    SetScrollPos(SB_VERT,vertical,TRUE);
}
void CScrollView::FillOutsideRect(CDC* dc,CBrush* brush) {
    RECT rectangle{};
    ::GetClientRect(m_hWnd,&rectangle);
    rectangle.left=m_totalDev.cx;
    if(!::IsRectEmpty(&rectangle))::FillRect(dc->m_hDC,&rectangle,brush?static_cast<HBRUSH>(brush->m_hObject):nullptr);
    rectangle.right=m_totalDev.cx;
    rectangle.top=m_totalDev.cy;
    rectangle.left=0;
    if(!::IsRectEmpty(&rectangle))::FillRect(dc->m_hDC,&rectangle,brush?static_cast<HBRUSH>(brush->m_hObject):nullptr);
}

// Symbol: ?SetScrollSizes@CScrollView@@QEAAXHUtagSIZE@@AEBU2@1@Z
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z(CScrollView* self,int mode,SIZE total,const SIZE* page,const SIZE* line) {self->SetScrollSizes(mode,total,*page,*line);}
// Symbol: ?GetScrollPosition@CScrollView@@QEBA?AVCPoint@@XZ
extern "C" CPoint* MS_ABI impl__GetScrollPosition_CScrollView__QEBA_AVCPoint__XZ(const CScrollView* self,CPoint* output) {return new(output) CPoint(self->GetScrollPosition());}
// Symbol: ?ScrollToPosition@CScrollView@@QEAAXUtagPOINT@@@Z
extern "C" void MS_ABI impl__ScrollToPosition_CScrollView__QEAAXUtagPOINT___Z(CScrollView* self,POINT point) {self->ScrollToPosition(point);}
// Symbol: ?CenterOnPoint@CScrollView@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z(CScrollView* self,CPoint point) {self->CenterOnPoint(point);}
// Symbol: ?FillOutsideRect@CScrollView@@QEAAXPEAVCDC@@PEAVCBrush@@@Z
extern "C" void MS_ABI impl__FillOutsideRect_CScrollView__QEAAXPEAVCDC__PEAVCBrush___Z(CScrollView* self,CDC* dc,CBrush* brush) {self->FillOutsideRect(dc,brush);}
