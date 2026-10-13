#define OPENMFC_APPCORE_IMPL
#include "detail/CommandRoutingSupport.h"
static_assert(sizeof(CFrameWnd)==472 && offsetof(CFrameWnd,m_pViewActive)==0x170,"native frame routing storage");
static_assert(offsetof(CFrameWnd,m_hAccelTable)==0xf8 && offsetof(CFrameWnd,m_nIDHelp)==0x164,"native frame fields");
static_assert(sizeof(CView)==248 && offsetof(CView,m_pDocument)==0xe8,"native document pointer");

extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CCmdTarget*,UINT,int,void*,void*);
namespace {
template<class T> T Field(void* object,size_t offset) {
    return *reinterpret_cast<T*>(static_cast<char*>(object)+offset);
}
int Dispatch(void* target,UINT id,int code,void* extra,void* info) {
    using Handler=int (MS_ABI*)(void*,UINT,int,void*,void*);
    return reinterpret_cast<Handler>((*static_cast<void***>(target))[5])(target,id,code,extra,info);
}
}
// Symbol: ?GetRoutingFrame@CCmdTarget@@IEAAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetRoutingFrame_CCmdTarget__IEAAPEAVCFrameWnd__XZ(CCmdTarget*) {
    return AfxGetThreadState()->m_pRoutingFrame;
}
// Symbol: ?GetRoutingFrame_@CCmdTarget@@KAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ() {
    return AfxGetThreadState()->m_pRoutingFrame;
}
// Symbol: ?GetRoutingView@CCmdTarget@@IEAAPEAVCView@@XZ
extern "C" void* MS_ABI impl__GetRoutingView_CCmdTarget__IEAAPEAVCView__XZ(CCmdTarget*) {
    return AfxGetThreadState()->m_pRoutingView;
}
// Symbol: ?GetRoutingView_@CCmdTarget@@KAPEAVCView@@XZ
extern "C" void* MS_ABI impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ() {
    return AfxGetThreadState()->m_pRoutingView;
}

// Native frame routing (mfc140u 0x29e360): view, own map, current module's app.
// Symbol: ?OnCmdMsg@CFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* frame,UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) {
    CPushRoutingFrame routing(frame);
    void* view=Field<void*>(frame,0x170);
    if(view && Dispatch(view,id,code,extra,info))return TRUE;
    if(impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(frame,id,code,extra,info))return TRUE;
    void* app=Field<void*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ(),8);
    return app && Dispatch(app,id,code,extra,info) ? TRUE : FALSE;
}

// Native view routing (mfc140u 0x277780): own map, then associated document.
// Symbol: ?OnCmdMsg@CView@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CView* view,UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) {
    if(impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(view,id,code,extra,info))return TRUE;
    void* document=Field<void*>(view,0xe8);
    if(!document)return FALSE;
    CPushRoutingView routing(view);
    return Dispatch(document,id,code,extra,info);
}

CFrameWnd* CCmdTarget::GetRoutingFrame() { return static_cast<CFrameWnd*>(impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ()); }
CFrameWnd* CCmdTarget::GetRoutingFrame_() { return static_cast<CFrameWnd*>(impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ()); }
CView* CCmdTarget::GetRoutingView() { return static_cast<CView*>(impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ()); }
CView* CCmdTarget::GetRoutingView_() { return static_cast<CView*>(impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ()); }
int CFrameWnd::OnCmdMsg(UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) {
    return impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(this,id,code,extra,static_cast<AFX_CMDHANDLERINFO*>(info));
}
int CView::OnCmdMsg(UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) {
    return impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(this,id,code,extra,static_cast<AFX_CMDHANDLERINFO*>(info));
}
