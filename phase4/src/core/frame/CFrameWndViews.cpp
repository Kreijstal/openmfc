// Active-view transitions and document accelerator selection use the native
// frame fields and preserve callback changes made during deactivation.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"

CView* CFrameWnd::GetActiveView() const {return static_cast<CView*>(m_pViewActive);}
CDocument* CFrameWnd::GetActiveDocument() {
    auto* view=GetActiveView();
    return view ? view->m_pDocument:nullptr;
}
CFrameWnd* CFrameWnd::GetActiveFrame() {return this;}
void CFrameWnd::SetActivePreviewView(CView* view) {m_pViewActive=view;}
void CFrameWnd::SetActiveView(CView* view,BOOL notify) {
    auto* previous=GetActiveView();
    if(view==previous)return;
    m_pViewActive=nullptr;
    if(previous)previous->OnActivateView(FALSE,view,previous);
    if(m_pViewActive)return;
    m_pViewActive=view;
    if(view && notify)view->OnActivateView(TRUE,view,previous);
}
HACCEL CFrameWnd::GetDefaultAccelerator() {
    HACCEL result=m_hAccelTable;
    if(auto* document=GetActiveDocument()) {
        // CDocument's exported client ABI places this virtual at slot 62.
        // Its public source declaration is still incomplete, so dispatch using
        // the native position rather than the legacy document declaration.
        using GetAccelerator=HACCEL (MS_ABI*)(CDocument*);
        auto accelerator=openmfc::detail::WindowSlot<GetAccelerator>(document,62)(document);
        if(accelerator)result=accelerator;
    }
    return result;
}

// Symbol: ?GetActiveView@CFrameWnd@@QEBAPEAVCView@@XZ
extern "C" CView* MS_ABI impl__GetActiveView_CFrameWnd__QEBAPEAVCView__XZ(const CFrameWnd* self) {return self->GetActiveView();}
// Symbol: ?GetActiveDocument@CFrameWnd@@UEAAPEAVCDocument@@XZ
extern "C" CDocument* MS_ABI impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(CFrameWnd* self) {return self->CFrameWnd::GetActiveDocument();}
// Symbol: ?GetActiveFrame@CFrameWnd@@UEAAPEAV1@XZ
extern "C" CFrameWnd* MS_ABI impl__GetActiveFrame_CFrameWnd__UEAAPEAV1_XZ(CFrameWnd* self) {return self->CFrameWnd::GetActiveFrame();}
// Symbol: ?SetActivePreviewView@CFrameWnd@@QEAAXPEAVCView@@@Z
extern "C" void MS_ABI impl__SetActivePreviewView_CFrameWnd__QEAAXPEAVCView___Z(CFrameWnd* self,CView* view) {self->SetActivePreviewView(view);}
// Symbol: ?SetActiveView@CFrameWnd@@QEAAXPEAVCView@@H@Z
extern "C" void MS_ABI impl__SetActiveView_CFrameWnd__QEAAXPEAVCView__H_Z(CFrameWnd* self,CView* view,BOOL notify) {self->SetActiveView(view,notify);}
// Symbol: ?GetDefaultAccelerator@CFrameWnd@@UEAAPEAUHACCEL__@@XZ
extern "C" HACCEL MS_ABI impl__GetDefaultAccelerator_CFrameWnd__UEAAPEAUHACCEL____XZ(CFrameWnd* self) {return self->CFrameWnd::GetDefaultAccelerator();}
static_assert(offsetof(CFrameWnd,m_pViewActive)==0x170 && offsetof(CView,m_pDocument)==0xe8,
    "native active-view and associated-document storage");
