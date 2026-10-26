// Native form activation and keyboard/parent-frame routing.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
BOOL CFormView::PreTranslateMessage(MSG* message) {
    if(CWnd::PreTranslateMessage(message))return TRUE;
    CFrameWnd* top=GetTopLevelFrame();
    if(top && *reinterpret_cast<BOOL*>(reinterpret_cast<char*>(top)+0x104))return FALSE;
    const CWnd* current=this;
    while(CFrameWnd* frame=current->GetParentFrame()) {
        using Translate=BOOL (MS_ABI*)(CWnd*,MSG*);
        if(openmfc::detail::WindowSlot<Translate>(frame,69)(frame,message))return TRUE;
        current=frame;
    }
    if(!::GetWindow(m_hWnd,GW_CHILD))return FALSE;
    const UINT code=message->message;
    if(code-0x100u<=9u || code-0x200u<=9u)return IsDialogMessageW(message);
    return FALSE;
}
void CFormView::OnActivateView(BOOL activate,CView*,CView*) {
    if(!SaveFocusControl() && activate && IsTopParentActive())SetFocus();
}
// Symbol: ?PreTranslateMessage@CFormView@@MEAAHPEAUtagMSG@@@Z
extern "C" BOOL MS_ABI impl__PreTranslateMessage_CFormView__MEAAHPEAUtagMSG___Z(CFormView* self,MSG* message) { return self->CFormView::PreTranslateMessage(message); }
// Symbol: ?OnActivateView@CFormView@@MEAAXHPEAVCView@@0@Z
extern "C" void MS_ABI impl__OnActivateView_CFormView__MEAAXHPEAVCView__0_Z(CFormView* self,BOOL active,CView* activate,CView* deactivate) { self->CFormView::OnActivateView(active,activate,deactivate); }
