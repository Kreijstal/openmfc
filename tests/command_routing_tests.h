#pragma once
#include "detail/CommandRoutingSupport.h"
#include <array>

extern "C" _AFX_THREAD_STATE* AFXAPI impl___0_AFX_THREAD_STATE__QEAA_XZ(_AFX_THREAD_STATE*);
extern "C" void AFXAPI impl___1_AFX_THREAD_STATE__UEAA_XZ(_AFX_THREAD_STATE*);
namespace command_routing_tests {
struct Probe : CCmdTarget {
    using CCmdTarget::GetRoutingFrame;
    using CCmdTarget::GetRoutingFrame_;
    using CCmdTarget::GetRoutingView;
    using CCmdTarget::GetRoutingView_;
};
static LRESULT CALLBACK Hook(int code,WPARAM wparam,LPARAM lparam) { return CallNextHookEx(nullptr,code,wparam,lparam); }
static DWORD WINAPI OtherThread(void* memory) {
    auto** result=static_cast<void**>(memory);
    auto* state=AfxGetThreadState();result[0]=state;
    result[1]=reinterpret_cast<void*>(UINT_PTR(!state->m_pRoutingFrame && !state->m_pRoutingView));
    CPushRoutingFrame routing(reinterpret_cast<CFrameWnd*>(UINT_PTR(0x55)));
    result[2]=impl__GetRoutingFrame__CCmdTarget__KAPEAVCFrameWnd__XZ();return 0;
}
static void storage() {
    auto* state=AfxGetThreadState();Probe target;
    check(state==impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ() && !state->m_pWndInit &&
        !state->m_pRoutingFrame && !state->m_pRoutingView && LocalSize(state)>=sizeof(*state),
        "thread storage uses native zeroed LocalAlloc memory and returns one stable object per thread");
    auto* module=reinterpret_cast<AFX_MODULE_STATE*>(UINT_PTR(0x141));
    auto* previous=impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(module);
    bool switched=state->m_pModuleState==module;
    auto* returned=impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(previous);
    check(switched && returned==module && state->m_pModuleState==previous,
        "module switching writes the native thread slot and returns the exact previous pointer, including null");
    alignas(_AFX_THREAD_STATE) unsigned char bytes[sizeof(_AFX_THREAD_STATE)];memset(bytes,0x5a,sizeof(bytes));
    auto* constructed=impl___0_AFX_THREAD_STATE__QEAA_XZ(reinterpret_cast<_AFX_THREAD_STATE*>(bytes));
    bool preserved=true;
    for(size_t i=8;i<sizeof(bytes);++i) {
        bool cleared=(i>=0x60 && i<0x64) || (i>=0x88 && i<0x94);
        if(bytes[i]!=(cleared ? 0 : 0x5a))preserved=false;
    }
    check(constructed==reinterpret_cast<_AFX_THREAD_STATE*>(bytes) && preserved,
        "the exported native thread constructor preserves caller storage except its explicit initialization fields");
    constructed->m_hHookOldMsgFilter=nullptr;constructed->m_hHookOldCbtFilter=nullptr;constructed->m_pSafetyPoolBuffer=nullptr;
    impl___1_AFX_THREAD_STATE__UEAA_XZ(constructed);
    auto* firstFrame=reinterpret_cast<CFrameWnd*>(UINT_PTR(0x101));
    auto* secondFrame=reinterpret_cast<CFrameWnd*>(UINT_PTR(0x202));
    auto* firstView=reinterpret_cast<CView*>(UINT_PTR(0x303));
    {
        CPushRoutingFrame outer(firstFrame);CPushRoutingView view(firstView);
        check(target.GetRoutingFrame()==firstFrame && Probe::GetRoutingFrame_()==firstFrame &&
            target.GetRoutingView()==firstView && Probe::GetRoutingView_()==firstView &&
            state->m_pPushRoutingFrame==&outer && state->m_pPushRoutingView==&view,
            "routing getters and physical thread fields agree with live frame and view scopes");
        {
            CPushRoutingFrame inner(secondFrame);
            check(target.GetRoutingFrame()==secondFrame && state->m_pPushRoutingFrame==&inner,
                "nested routing scopes replace the current pointer and link their native stack");
            inner.Pop();bool invalid=false;try { inner.Pop(); }catch(int value) { invalid=value==19; }
            check(target.GetRoutingFrame()==firstFrame && state->m_pPushRoutingFrame==&outer && invalid,
                "explicit routing Pop restores its parent once and rejects a second Pop");
        }
        void* result[3]{};HANDLE thread=CreateThread(nullptr,0,OtherThread,result,0,nullptr);
        bool joined=thread && WaitForSingleObject(thread,30000)==WAIT_OBJECT_0;if(thread)CloseHandle(thread);
        check(joined && result[0]!=state && result[1] && result[2]==reinterpret_cast<void*>(UINT_PTR(0x55)) &&
            state->m_pRoutingFrame==firstFrame && state->m_pRoutingView==firstView,
            "routing state and nested scopes remain isolated across native Windows threads");
        bool unwound=false;
        try { CPushRoutingFrame inner(secondFrame);CPushRoutingView nested(nullptr);throw 53; }
        catch(int value) { unwound=value==53; }
        check(unwound && state->m_pRoutingFrame==firstFrame && state->m_pRoutingView==firstView,
            "routing scope destructors restore frame and view state during C++ unwinding");
    }
    check(!state->m_pRoutingFrame && !state->m_pRoutingView && !state->m_pPushRoutingFrame && !state->m_pPushRoutingView,
        "the outer routing scopes restore all native pointers and stack links");
    HHOOK messageHook=SetWindowsHookExW(WH_GETMESSAGE,Hook,nullptr,GetCurrentThreadId());
    HHOOK cbtHook=SetWindowsHookExW(WH_CBT,Hook,nullptr,GetCurrentThreadId());
    auto* cleanup=new _AFX_THREAD_STATE;cleanup->m_hHookOldMsgFilter=messageHook;cleanup->m_hHookOldCbtFilter=cbtHook;
    cleanup->m_pSafetyPoolBuffer=std::malloc(128);delete cleanup;
    check(messageHook && cbtHook && !UnhookWindowsHookEx(messageHook) && !UnhookWindowsHookEx(cbtHook),
        "native thread destruction unhooks both Windows hooks and releases its safety-pool allocation");
}

struct Observation { void* target;CFrameWnd* frame;CView* view;UINT id;int code;void* extra;void* info; };
static std::vector<Observation> observations;
static void* viewTarget=nullptr;
static void* documentTarget=nullptr;
static void* frameTarget=nullptr;
static void* appTarget=nullptr;
static int viewResult=0,frameResult=0,documentResult=0,appResult=0;
static bool raise=false,nest=false;
static CFrameWnd* nestedFrame=nullptr;
static void Record(void* target,UINT id,int code,void* extra,void* info) {
    auto* state=AfxGetThreadState();observations.push_back({target,state->m_pRoutingFrame,state->m_pRoutingView,id,code,extra,info});
}
static int Map(CCmdTarget* target,UINT id,int code,void* extra,void* info) {
    Record(target,id,code,extra,info);
    return target==viewTarget ? viewResult : target==frameTarget ? frameResult : 0;
}
static int MS_ABI Callback(void* target,UINT id,int code,void* extra,void* info) {
    Record(target,id,code,extra,info);
    if(raise)throw 61;
    if(nest && target==documentTarget) {
        nest=false;impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(nestedFrame,id,code,extra,static_cast<AFX_CMDHANDLERINFO*>(info));
        Record(target,id,code,extra,info);
    }
    return target==documentTarget ? documentResult : target==appTarget ? appResult : 1;
}
template<size_t Bytes> struct NativeObject {
    alignas(void*) std::array<unsigned char,Bytes> bytes{};
    void* table[22]{};
    explicit NativeObject(void* handler) { table[5]=handler;Set(0,table); }
    void Set(size_t offset,void* pointer) { memcpy(bytes.data()+offset,&pointer,sizeof(pointer)); }
    template<class T> T* As() { return reinterpret_cast<T*>(bytes.data()); }
};
static void pipeline() {
    NativeObject<472> frame(reinterpret_cast<void*>(impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z));
    NativeObject<248> view(reinterpret_cast<void*>(impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z));
    NativeObject<64> document(reinterpret_cast<void*>(Callback)),app(reinterpret_cast<void*>(Callback));
    NativeObject<472> nested(reinterpret_cast<void*>(impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z));
    NativeObject<248> nestedView(reinterpret_cast<void*>(impl__OnCmdMsg_CView__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z));
    NativeObject<64> nestedDocument(reinterpret_cast<void*>(Callback));
    frame.Set(0x170,view.bytes.data());view.Set(0xe8,document.bytes.data());
    nested.Set(0x170,nestedView.bytes.data());nestedView.Set(0xe8,nestedDocument.bytes.data());nestedFrame=nested.As<CFrameWnd>();
    void* module[4]{};module[1]=app.bytes.data();
    AFX_MODULE_STATE* previousModule=g_cmdTestCurrentModule;g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(module);
    frameTarget=frame.bytes.data();viewTarget=view.bytes.data();documentTarget=document.bytes.data();appTarget=app.bytes.data();
    g_cmdRoutingMapHook=Map;
    auto call=[&]() { return frame.As<CFrameWnd>()->CFrameWnd::OnCmdMsg(42,-1,&frame,reinterpret_cast<AFX_CMDHANDLERINFO*>(&view)); };
    auto* priorFrame=reinterpret_cast<CFrameWnd*>(UINT_PTR(0x71));auto* priorView=reinterpret_cast<CView*>(UINT_PTR(0x72));
    CPushRoutingFrame outerFrame(priorFrame);CPushRoutingView outerView(priorView);
    observations.clear();viewResult=7;
    check(call()==TRUE && observations.size()==1 && observations[0].target==viewTarget &&
        observations[0].frame==frame.As<CFrameWnd>() && observations[0].view==priorView,
        "frame routing first invokes the active view's own command map under the current frame scope");
    observations.clear();viewResult=0;documentResult=9;
    check(call()==TRUE && observations.size()==2 && observations[1].target==documentTarget &&
        observations[1].frame==frame.As<CFrameWnd>() && observations[1].view==view.As<CView>(),
        "view routing forwards unhandled commands to the associated document under its native view scope");
    observations.clear();documentResult=0;frameResult=8;
    check(call()==TRUE && observations.size()==3 && observations[2].target==frameTarget && observations[2].view==priorView,
        "a declined document command restores the view scope before the frame's own map is tried");
    observations.clear();frameResult=0;appResult=6;
    check(call()==TRUE && observations.size()==4 && observations[3].target==appTarget && observations[3].view==priorView,
        "unhandled frame commands reach the current module's application through native virtual slot five");
    bool arguments=true;for(auto& observed:observations)
        arguments=arguments && observed.id==42 && observed.code==-1 && observed.extra==&frame && observed.info==&view;
    check(arguments,"all routing stages preserve command IDs, notification codes and caller-owned handler-query arguments");
    observations.clear();appResult=0;
    check(!call() && observations.size()==4,"fully unhandled commands return FALSE after the complete routing chain");
    observations.clear();view.Set(0xe8,nullptr);module[1]=nullptr;
    check(!call() && observations.size()==2,"missing documents and module applications skip only their respective routing stages");
    view.Set(0xe8,document.bytes.data());module[1]=app.bytes.data();
    documentResult=9;observations.clear();nest=true;
    check(call()==TRUE && observations.size()==5 && observations[3].target==nestedDocument.bytes.data() &&
        observations[3].frame==nested.As<CFrameWnd>() && observations[3].view==nestedView.As<CView>() &&
        observations[4].frame==frame.As<CFrameWnd>() && observations[4].view==view.As<CView>(),
        "reentrant frame/document routing restores both outer scopes after nested command handlers");
    raise=true;bool caught=false;try { call(); }catch(int value) { caught=value==61; }raise=false;
    check(caught && AfxGetThreadState()->m_pRoutingFrame==priorFrame && AfxGetThreadState()->m_pRoutingView==priorView &&
        AfxGetThreadState()->m_pPushRoutingFrame==&outerFrame && AfxGetThreadState()->m_pPushRoutingView==&outerView,
        "handler exceptions propagate while native frame/view scope links unwind to their original owners");
    g_cmdRoutingMapHook=nullptr;g_cmdTestCurrentModule=previousModule;
}
}
static void commandRouting() { command_routing_tests::storage();command_routing_tests::pipeline(); }
