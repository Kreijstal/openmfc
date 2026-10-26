// Release-x64 pane validation and shared-scrollbar virtual dispatch.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxole.h"

int CSplitterWnd::IdFromRowCol(int row,int col) const {
    return static_cast<int>(0xe900u+static_cast<UINT>(row)*16u+static_cast<UINT>(col));
}
BOOL CSplitterWnd::IsChildPane(CWnd* child,int* row,int* col) {
    UINT id=static_cast<UINT>(::GetDlgCtrlID(child->m_hWnd));
    if(::IsChild(m_hWnd,child->m_hWnd) && id-0xe900u<=0xffu) {
        if(row)*row=static_cast<int>((id-0xe900u)>>4);
        if(col)*col=static_cast<int>(id&15u);
        return TRUE;
    }
    if(row)*row=-1;
    if(col)*col=-1;
    return FALSE;
}
namespace {
template<class Scroll> BOOL ScrollPanes(CSplitterWnd* splitter,CView* from,Scroll scroll) {
    int row,col;
    if(!splitter->IsChildPane(from,&row,&col))return FALSE;
    CScrollBar* vertical=from->GetScrollBarCtrl(SB_VERT);
    int verticalPosition=vertical?::GetScrollPos(vertical->m_hWnd,SB_CTL):0;
    CScrollBar* horizontal=from->GetScrollBarCtrl(SB_HORZ);
    int horizontalPosition=horizontal?::GetScrollPos(horizontal->m_hWnd,SB_CTL):0;
    BOOL result=scroll(from,SB_BOTH)!=0;
    if(vertical)for(int c=0;c<splitter->m_nCols;++c) {
        if(c==col)continue;
        ::SetScrollPos(vertical->m_hWnd,SB_CTL,verticalPosition,FALSE);
        auto* pane=static_cast<CView*>(splitter->GetDlgItem(splitter->IdFromRowCol(row,c)));
        if(scroll(pane,SB_VERT))result=TRUE;
    }
    if(horizontal)for(int r=0;r<splitter->m_nRows;++r) {
        if(r==row)continue;
        ::SetScrollPos(horizontal->m_hWnd,SB_CTL,horizontalPosition,FALSE);
        auto* pane=static_cast<CView*>(splitter->GetDlgItem(splitter->IdFromRowCol(r,col)));
        if(scroll(pane,SB_HORZ))result=TRUE;
    }
    return result;
}
}
BOOL CSplitterWnd::DoScroll(CView* view,UINT code,BOOL perform) {
    return ScrollPanes(this,view,[&](CView* pane,int axis) {
        UINT command=axis==SB_BOTH?code:axis==SB_VERT?(code&0xff00u)|0xffu:(code&0xffu)|0xff00u;
        return pane->OnScroll(command,0,perform);
    });
}
BOOL CSplitterWnd::DoScrollBy(CView* view,CSize distance,BOOL perform) {
    return ScrollPanes(this,view,[&](CView* pane,int axis) {
        return pane->OnScrollBy(CSize(axis==SB_VERT?0:distance.cx,axis==SB_HORZ?0:distance.cy),perform);
    });
}
// Symbol: ?IdFromRowCol@CSplitterWnd@@QEBAHHH@Z
extern "C" int MS_ABI impl__IdFromRowCol_CSplitterWnd__QEBAHHH_Z(const CSplitterWnd* self,int row,int col) {return self->IdFromRowCol(row,col);}
// Symbol: ?IsChildPane@CSplitterWnd@@QEAAHPEAVCWnd@@PEAH1@Z
extern "C" BOOL MS_ABI impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(CSplitterWnd* self,CWnd* child,int* row,int* col) {return self->IsChildPane(child,row,col);}
// Symbol: ?DoScroll@CSplitterWnd@@UEAAHPEAVCView@@IH@Z
extern "C" BOOL MS_ABI impl__DoScroll_CSplitterWnd__UEAAHPEAVCView__IH_Z(CSplitterWnd* self,CView* view,UINT code,BOOL perform) {return self->CSplitterWnd::DoScroll(view,code,perform);}
// Symbol: ?DoScrollBy@CSplitterWnd@@UEAAHPEAVCView@@VCSize@@H@Z
extern "C" BOOL MS_ABI impl__DoScrollBy_CSplitterWnd__UEAAHPEAVCView__VCSize__H_Z(CSplitterWnd* self,CView* view,CSize distance,BOOL perform) {return self->CSplitterWnd::DoScrollBy(view,distance,perform);}
