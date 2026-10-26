// Native splitter ancestry and view split/next-pane command routing.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxole.h"
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
extern "C" BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject*,const CRuntimeClass*);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
CSplitterWnd* CView::GetParentSplitter(const CWnd* window,BOOL anyState) {
    if(!window)impl__AfxThrowInvalidArgException__YAXXZ();
    CWnd* parent=impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(window->m_hWnd));
    if(!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(parent,&CSplitterWnd::classCSplitterWnd))return nullptr;
    if(!anyState) {
        const CWnd* current=window;
        while((current=impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(current->m_hWnd)))) {
            if(::IsIconic(current->m_hWnd))return nullptr;
        }
    }
    return static_cast<CSplitterWnd*>(parent);
}
// Symbol: ?GetParentSplitter@CView@@SAPEAVCSplitterWnd@@PEBVCWnd@@H@Z
extern "C" CSplitterWnd* MS_ABI impl__GetParentSplitter_CView__SAPEAVCSplitterWnd__PEBVCWnd__H_Z(const CWnd* window,BOOL anyState) {return CView::GetParentSplitter(window,anyState);}
// Symbol: ?OnSplitCmd@CView@@IEAAHI@Z
extern "C" BOOL MS_ABI impl__OnSplitCmd_CView__IEAAHI_Z(CView* self,UINT) {
    auto* splitter=CView::GetParentSplitter(self);
    if(!splitter)return FALSE;
    splitter->DoKeyboardSplit();
    return TRUE;
}
// Symbol: ?OnNextPaneCmd@CView@@IEAAHI@Z
extern "C" BOOL MS_ABI impl__OnNextPaneCmd_CView__IEAAHI_Z(CView* self,UINT command) {
    auto* splitter=CView::GetParentSplitter(self);
    if(!splitter)return FALSE;
    splitter->ActivateNext(command==0xe151);
    return TRUE;
}
// Symbol: ?OnUpdateSplitCmd@CView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateSplitCmd_CView__IEAAXPEAVCCmdUI___Z(CView* self,CCmdUI* ui) {
    if(!ui)impl__AfxThrowInvalidArgException__YAXXZ();
    auto* splitter=CView::GetParentSplitter(self);
    using Enable=void (MS_ABI*)(CCmdUI*,BOOL);
    auto enable=openmfc::detail::WindowSlot<Enable>(ui,0);
    enable(ui,splitter && !splitter->m_bTracking);
}
// Symbol: ?OnUpdateNextPaneMenu@CView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNextPaneMenu_CView__IEAAXPEAVCCmdUI___Z(CView* self,CCmdUI* ui) {
    auto* splitter=CView::GetParentSplitter(self);
    using Enable=void (MS_ABI*)(CCmdUI*,BOOL);
    // Native code retains this callback even if CanActivateNext changes ui.
    auto enable=openmfc::detail::WindowSlot<Enable>(ui,0);
    BOOL available=splitter && splitter->CanActivateNext(ui->m_nID==0xe151);
    enable(ui,available);
}
