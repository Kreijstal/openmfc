// Native pane navigation and keyboard splitter tracking.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
namespace {
int Add(int a,int b) {return static_cast<int>(static_cast<unsigned>(a)+static_cast<unsigned>(b));}
int Sub(int a,int b) {return static_cast<int>(static_cast<unsigned>(a)-static_cast<unsigned>(b));}
}
BOOL CSplitterWnd::CanActivateNext(BOOL) {
    if(!GetActivePane(nullptr,nullptr))return FALSE;
    return m_nRows>1 || m_nCols>1;
}
void CSplitterWnd::ActivateNext(BOOL previous) {
    int row,col;
    if(!GetActivePane(&row,&col))return;
    if(previous) {
        col=Sub(col,1);
        if(col<0) {col=Sub(m_nCols,1);row=Sub(row,1);if(row<0)row=Sub(m_nRows,1);}
    } else {
        col=Add(col,1);
        if(col>=m_nCols) {col=0;row=Add(row,1);if(row>=m_nRows)row=0;}
    }
    SetActivePane(row,col,nullptr);
}
BOOL CSplitterWnd::DoKeyboardSplit() {
    int hit;
    if(m_nRows>1)hit=m_nCols>1?301:101;
    else if(m_nCols>1)hit=201;
    else if(m_nMaxRows>1)hit=m_nMaxCols>1?3:1;
    else if(m_nMaxCols>1)hit=2;
    else return FALSE;
    StartTracking(hit);
    RECT rectangle={Sub(m_rectTracker.right,m_rectTracker.left)/2,Sub(m_rectTracker.bottom,m_rectTracker.top)/2,0,0};
    if(m_ptTrackOffset.y)rectangle.top=m_rectTracker.top;
    if(m_ptTrackOffset.x)rectangle.left=m_bTracking2?m_rectTracker2.left:m_rectTracker.left;
    ::OffsetRect(&rectangle,Sub(0,m_ptTrackOffset.x),Sub(0,m_ptTrackOffset.y));
    ClientToScreen(&rectangle);
    ::SetCursorPos(rectangle.left,rectangle.top);
    return TRUE;
}
// Symbol: ?CanActivateNext@CSplitterWnd@@UEAAHH@Z
extern "C" BOOL MS_ABI impl__CanActivateNext_CSplitterWnd__UEAAHH_Z(CSplitterWnd* self,BOOL previous) {return self->CSplitterWnd::CanActivateNext(previous);}
// Symbol: ?ActivateNext@CSplitterWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__ActivateNext_CSplitterWnd__UEAAXH_Z(CSplitterWnd* self,BOOL previous) {self->CSplitterWnd::ActivateNext(previous);}
// Symbol: ?DoKeyboardSplit@CSplitterWnd@@UEAAHXZ
extern "C" BOOL MS_ABI impl__DoKeyboardSplit_CSplitterWnd__UEAAHXZ(CSplitterWnd* self) {return self->CSplitterWnd::DoKeyboardSplit();}

// Symbol: ?GetThisClass@CSplitterWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ() {return &CSplitterWnd::classCSplitterWnd;}
// Symbol: ?GetRuntimeClass@CSplitterWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitterWnd__UEBAPEAUCRuntimeClass__XZ(const CSplitterWnd*) {return &CSplitterWnd::classCSplitterWnd;}
