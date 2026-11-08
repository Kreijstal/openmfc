// Real filter, frame traversal/tracking, map storage and shared thread state.
// Raw client objects let the same ABI probes run against pinned native MFC.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <vector>
#include <type_traits>
#include <utility>
extern "C" {
BOOL MS_ABI impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(CWinThread*,int,MSG*);
BOOL MS_ABI impl__IsTracking_CFrameWnd__QEAAHXZ(CFrameWnd*);
void MS_ABI impl__OnShowMenuBar_CFrameWnd__MEAAXXZ(CFrameWnd*);
void MS_ABI impl__OnHideMenuBar_CFrameWnd__MEAAXXZ(CFrameWnd*);
void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
}
namespace openmfc { namespace detail { namespace winthread {
LRESULT CALLBACK MessageFilterHook(int,WPARAM,LPARAM);
}}}
namespace {
int failures;
void check(bool value,const char* label) {if(!value) {++failures;std::printf("FAIL: %s\n",label);}}
template<class T> struct alignas(16) Object {
    BYTE bytes[sizeof(T)];
    T* get() {return reinterpret_cast<T*>(bytes);}
    void reset(void** table) {std::memset(bytes,0,sizeof(bytes));*reinterpret_cast<void***>(bytes)=table;}
};
struct API {
    decltype(&impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z) filter;
    decltype(&impl__IsTracking_CFrameWnd__QEAAHXZ) tracking;
    UINT (MS_ABI* trackingID)(CFrameWnd*);
    _AFX_THREAD_STATE* (MS_ABI* thread)();
    AFX_MODULE_THREAD_STATE* (MS_ABI* state)();
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) destroyLocal;
    decltype(&openmfc::detail::winthread::MessageFilterHook) hook;
    bool native;
    decltype(&impl__OnShowMenuBar_CFrameWnd__MEAAXXZ) showBase;
    decltype(&impl__OnHideMenuBar_CFrameWnd__MEAAXXZ) hideBase;
};
const API* active;
Object<CWinThread> threads[2];
Object<CFrameWnd> objects[5];
HWND handles[5]{};
void* threadTable[33]{};
void* frameTable[114]{};
void* siteTable[44]{};void** site=siteTable;
alignas(16) BYTE module[0x110]{};
AFX_MODULE_THREAD_STATE* state;
CHandleMap* windowMap;
CObject* temporary;
std::vector<int> events;
MSG* original;
MSG observed,expectedCopy;
UINT trackingID;
int mutation,fault,preResult,enabledResult,destroyedTemps;
bool mutateCopy,recurse,inRecursion,captureCommand;
CWnd* mainWindow;
void event(int id) {events.push_back(id);if(fault==id)throw id;}
unsigned index(const void* p) {
    for(unsigned i=0;i<5;++i)if(p==objects[i].get())return i;
    if(p==temporary)return 5;
    std::abort();
}
BOOL AFXAPI isFrame(const CWnd* self) {unsigned i=index(self);event(100+int(i));return i==0 || i==3;}
UINT AFXAPI tracking(CFrameWnd* self) {
    check(self==objects[0].get(),"menu filtering queries the top-level frame's tracking override");event(2);
    if(mutation==6)self->m_retailMiddle.m_bHelpMode=-7;
    if(mutation==7)original->message=WM_LBUTTONDOWN;
    return trackingID;
}
CWnd* AFXAPI getMain(CWinThread* self) {
    event(self==threads[0].get() ? 10:11);
    if(!inRecursion) {
        if(mutation==1)threads[0].get()->m_pActiveWnd=nullptr;
        if(mutation==2)active->thread()->m_bInMsgFilter=-1;
        if(mutation==3) {original->message=WM_LBUTTONUP;original->wParam=0;}
        if(mutation==4)threads[0].get()->m_pMainWnd=nullptr;
        if(mutation==5)state->m_pCurrentWinThread=threads[1].get();
    }
    return mainWindow;
}
BOOL AFXAPI enabled(void* self) {
    check(self==&site && active->thread()->m_bInMsgFilter==1,
          "OLE enable query occurs inside the shared filter reentrancy guard");event(20);
    if(mutation==8) {original->message=WM_USER;original->wParam=UINT_PTR(0xfedcba9876543210ull);}
    if(mutation==9)threads[0].get()->m_pActiveWnd=nullptr;
    return enabledResult;
}
BOOL AFXAPI pre(CWinThread* self,MSG* message) {
    check(self==threads[0].get() && message!=original,"dialog filtering calls its own thread override with a copied message");
    check(active->thread()->m_bInMsgFilter==1,"pretranslation sees the active filter guard");
    observed=*message;check(!std::memcmp(message,&expectedCopy,sizeof(MSG)),"pretranslation receives the complete pre-enable-query message copy");
    event(30);
    if(recurse && !inRecursion) {
        inRecursion=true;
        check(active->filter(self,MSGF_DIALOGBOX,original)==FALSE,"recursive filtering is suppressed by real shared thread state");
        inRecursion=false;
    }
    if(mutateCopy) {message->message=WM_USER;message->hwnd=nullptr;message->wParam=0;message->pt={-91,37};}
    return preResult;
}
BOOL AFXAPI unexpectedFilter(CWinThread*,int,MSG*) {std::abort();}
LRESULT CALLBACK procedure(HWND hwnd,UINT message,WPARAM w,LPARAM l) {
    if(captureCommand && message==WM_COMMAND) {
        check(hwnd==mainWindow->m_hWnd && w==0xe146 && l==0,
              "menu help sends ID_HELP to virtual GetMainWnd's HWND with full native arguments");
        events.push_back(40);
    }
    return DefWindowProcW(hwnd,message,w,l);
}
void AFXAPI construct(CObject* p) {
    std::memset(p,0,sizeof(CFrameWnd));*reinterpret_cast<void***>(p)=frameTable;temporary=p;
}
void AFXAPI destroy(CObject*) {++destroyedTemps;}
CRuntimeClass windowClass{"FilterClientWindow",sizeof(CFrameWnd),0xffff,nullptr,nullptr,nullptr,nullptr};
void reset(MSG& message) {
    events.clear();mutation=fault=0;preResult=69;enabledResult=TRUE;
    mutateCopy=recurse=inRecursion=captureCommand=false;destroyedTemps=0;mainWindow=objects[3].get();
    threads[0].reset(threadTable);threads[1].reset(threadTable);
    for(unsigned i=0;i<5;++i) {objects[i].reset(frameTable);objects[i].get()->m_hWnd=handles[i];EnableWindow(handles[i],TRUE);}
    threads[0].get()->m_pActiveWnd=objects[2].get();threads[0].get()->m_pMainWnd=objects[0].get();
    state->m_pCurrentWinThread=threads[0].get();active->thread()->m_bInMsgFilter=FALSE;
    trackingID=1;objects[0].get()->m_retailMiddle.m_bHelpMode=TRUE;
    std::memset(&message,0xa5,sizeof(message));message.hwnd=handles[1];message.message=WM_KEYDOWN;message.wParam=VK_RETURN;
    message.lParam=LPARAM(0xfedcba9876543210ull);message.time=0xabcdef01;message.pt={-1234567,7654321};
    original=&message;std::memcpy(&expectedCopy,&message,sizeof(MSG));observed={};
}
BOOL filter(MSG& message,int code) {
    captureCommand=true;BOOL result=active->filter(threads[0].get(),code,&message);captureCommand=false;return result;
}
void expected(std::initializer_list<int> list,const char* label) {
    check(events==std::vector<int>(list),label);
    if(events!=std::vector<int>(list)) {for(int value:events)std::printf(" %d",value);std::puts("");}
}

void dialogs() {
    MSG message;
    for(int code : {INT_MIN,-1,1,3,5,INT_MAX}) {
        reset(message);
        check(!active->filter(reinterpret_cast<CWinThread*>(UINT_PTR(1)),code,&message) && events.empty(),
              "unrelated filter codes ignore the thread object and never pretranslate");
    }
    for(int code : {MSGF_DIALOGBOX,MSGF_MENU,-1,INT_MAX}) {
        reset(message);
        check(!active->filter(reinterpret_cast<CWinThread*>(UINT_PTR(1)),code,nullptr) && events.empty(),
              "NULL messages are rejected before touching the thread object");
    }
    for(UINT id : {UINT(0xff),UINT(0x100),UINT(0x101),UINT(0x102),UINT(0x104),UINT(0x108),UINT(0x109),UINT(0x10a),UINT(WM_LBUTTONUP),UINT(-1)}) {
        reset(message);message.message=id;std::memcpy(&expectedCopy,&message,sizeof(MSG));MSG before=message;mutateCopy=true;
        BOOL result=filter(message,MSGF_DIALOGBOX);bool keyboard=id>=0x100 && id<=0x109;
        check(result==BOOL(keyboard),"only dialog keyboard messages are offered for pretranslation");
        if(keyboard)expected({10,30},"current-main lookup precedes dialog pretranslation");
        else expected({10},"nonkeyboard dialog messages still invoke the virtual main-window getter");
        check(!std::memcmp(&message,&before,sizeof(MSG)),"pretranslation cannot overwrite the caller's message through its local copy");
        check(!active->thread()->m_bInMsgFilter,"normal dialog filtering releases its reentrancy guard");
    }
    for(int value : {0,1,-1,69,INT_MIN}) {
        reset(message);preResult=value;check(filter(message,MSGF_DIALOGBOX)==BOOL(value!=0),"pretranslation's BOOL result is normalized");
    }
    reset(message);threads[0].get()->m_pActiveWnd=nullptr;
    check(!filter(message,MSGF_DIALOGBOX),"dialog filtering requires an active window");expected({10},"inactive dialog filtering still consults GetMainWnd");
    reset(message);EnableWindow(handles[2],FALSE);
    check(!filter(message,MSGF_DIALOGBOX),"a disabled active window suppresses dialog pretranslation");expected({10},"disabled Win32 windows do not reach pretranslation");
    reset(message);state->m_pCurrentWinThread=nullptr;
    check(filter(message,MSGF_DIALOGBOX)==TRUE,"dialog filtering uses its own thread even with no registered current thread");expected({30},"a missing current thread skips only the main-window getter");
    reset(message);state->m_pCurrentWinThread=threads[1].get();
    check(filter(message,MSGF_DIALOGBOX)==TRUE,"main lookup and dialog pretranslation can use different threads");expected({11,30},"the current thread selects main lookup while this selects pretranslation");
    for(int flag : {1,-1,INT_MIN,INT_MAX}) {
        reset(message);active->thread()->m_bInMsgFilter=flag;
        check(!filter(message,MSGF_DIALOGBOX) && active->thread()->m_bInMsgFilter==flag,
              "all nonzero reentrancy flags suppress filtering without being overwritten");expected({10},"reentrant filtering still performs main lookup first");
    }
    for(int change : {1,2}) {
        reset(message);mutation=change;
        check(!filter(message,MSGF_DIALOGBOX),"main-window overrides can suppress subsequent dialog filtering");expected({10},"active-window and filter-flag fields are read after main lookup");
    }
    for(int value : {0,-1,69}) {
        reset(message);objects[2].get()->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);enabledResult=value;
        check(filter(message,MSGF_DIALOGBOX)==BOOL(value!=0),"OLE window enablement controls filtering independently of HWND enablement");
        if(value)expected({10,20,30},"OLE enablement precedes pretranslation");else expected({10,20},"a disabled OLE site suppresses pretranslation");
    }
    for(int change : {8,9}) {
        reset(message);objects[2].get()->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);mutation=change;
        check(filter(message,MSGF_DIALOGBOX)==TRUE,"the enabled query can change original inputs without changing the captured dialog operation");
        expected({10,20,30},"pretranslation uses its earlier message copy even if enablement changes the original or active window");
    }
    reset(message);recurse=true;check(filter(message,MSGF_DIALOGBOX)==TRUE,"an outer dialog filter can finish after suppressed recursion");expected({10,30,10},"recursive entry consults main but never invokes pretranslation twice");
    for(int failure : {10,20,30}) {
        reset(message);if(failure==20)objects[2].get()->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);
        fault=failure;bool caught=false;
        try {filter(message,MSGF_DIALOGBOX);}catch(int value) {caught=value==failure;}
        check(caught,"main lookup, enablement and pretranslation failures propagate");
        check(active->thread()->m_bInMsgFilter==BOOL(failure!=10),"native leaves an acquired filter guard set when a callback throws");
        fault=0;if(failure!=10)check(!filter(message,MSGF_DIALOGBOX),"interrupted filtering remains guarded until the caller resets shared state");
    }
}

void menus() {
    MSG message;
    struct Input {UINT message;WPARAM key;bool help;};
    Input inputs[]={{WM_KEYDOWN,VK_RETURN,true},{WM_KEYDOWN,VK_SPACE,false},{WM_SYSKEYDOWN,VK_RETURN,false},
                    {WM_LBUTTONUP,UINT_PTR(0xfedcba9876543210ull),true},{WM_LBUTTONDOWN,0,false},
                    {WM_KEYDOWN,WPARAM(0x10000000dull),false}};
    for(UINT id : {0u,1u,0xe000u,0xe001u,0xe002u,0xe003u,UINT_MAX})for(int help : {0,1,-1})for(const auto& input:inputs) {
        reset(message);trackingID=id;objects[0].get()->m_retailMiddle.m_bHelpMode=help;
        message.message=input.message;message.wParam=input.key;
        bool tracking=(id!=0 && id!=0xe001 && id!=0xe002);
        bool handled=tracking && help && input.help;
        check(filter(message,MSGF_MENU)==BOOL(handled),"menu help requires a tracked command, help mode, and Enter or left-button release");
        if(handled)expected({101,100,2,10,40},"menu help uses the outer frame and virtual main-window target");
        else if(tracking && help)expected({101,100,2,10,10},"unhandled help-mode menu messages perform both native main lookups");
        else expected({101,100,2,10},"non-help menus still invoke the general main-window lookup");
    }
    reset(message);message.hwnd=nullptr;
    check(!filter(message,MSGF_MENU),"a NULL menu target cannot activate frame help");expected({10},"a missing wrapper falls through to the general main lookup");
    reset(message);message.hwnd=handles[2];
    check(!filter(message,MSGF_MENU),"a target without a frame ancestor is not a help menu");expected({102,10},"frame lookup stops when the parent chain ends");
    reset(message);threads[0].get()->m_pMainWnd=nullptr;
    check(!filter(message,MSGF_MENU),"menu help also requires this thread's main-window field");expected({101,100,2,10,10},"a NULL this.main gate preserves both native main-window side effects");
    reset(message);mutation=3;message.message=WM_KEYUP;
    check(filter(message,MSGF_MENU)==TRUE,"menu help rereads message contents after GetMainWnd");expected({101,100,2,10,40},"getter mutation can select the help command");
    reset(message);mutation=4;
    check(!filter(message,MSGF_MENU),"clearing this.main during lookup suppresses menu help");expected({101,100,2,10,10},"menu help rereads this.main after its virtual getter");
    reset(message);mutation=5;message.message=WM_KEYUP;
    check(!filter(message,MSGF_MENU),"a getter can change current-thread registration before general lookup");expected({101,100,2,10,11},"the general menu lookup reloads the current thread");
    reset(message);mutation=6;objects[0].get()->m_retailMiddle.m_bHelpMode=0;
    check(filter(message,MSGF_MENU)==TRUE,"tracking overrides can enable frame help before its help flag is read");
    reset(message);mutation=7;message.message=WM_LBUTTONUP;
    check(!filter(message,MSGF_MENU),"tracking overrides can change which input message is filtered");expected({101,100,2,10,10},"message contents are read after frame tracking");
    for(int failure : {101,100,2,10}) {
        reset(message);fault=failure;bool caught=false;
        try {filter(message,MSGF_MENU);}catch(int value) {caught=value==failure;}
        check(caught && !active->thread()->m_bInMsgFilter,"menu callback failures propagate without acquiring the dialog guard");
    }
    reset(message);windowMap->m_permanentMap.RemoveKey(handles[1]);
    check(filter(message,MSGF_MENU)==TRUE && temporary && windowMap->m_temporaryMap.m_nCount==1,
          "menu filtering can construct a real temporary wrapper and traverse to a permanent frame");
    expected({105,100,2,10,40},"temporary targets use the same frame/help path as permanent targets");
    windowMap->DeleteTemp();check(destroyedTemps==1,"temporary menu-target wrappers retain real map ownership");
    windowMap->m_permanentMap[handles[1]]=objects[1].get();temporary=nullptr;
}

unsigned slotCalls;
CFrameWnd* slotObject;
template<class R,class Self,class... Args> struct Slot {
    static R AFXAPI call(Self* self,Args...) {
        check(self==slotObject,"frame virtual calls preserve this");++slotCalls;
        if constexpr(!std::is_void_v<R>)return R{};
    }
};
template<size_t Index,class R,class... Args,class... Values>
void probe(R (CFrameWnd::*method)(Args...),Values&&... values) {
    auto* old=frameTable[Index];frameTable[Index]=reinterpret_cast<void*>(&Slot<R,CFrameWnd,Args...>::call);
    slotCalls=0;(slotObject->*method)(std::forward<Values>(values)...);
    check(slotCalls==1,"source frame declarations use their native virtual positions");frameTable[Index]=old;
}
template<size_t Index,class R,class... Args,class... Values>
void probe(R (CFrameWnd::*method)(Args...) const,Values&&... values) {
    auto* old=frameTable[Index];frameTable[Index]=reinterpret_cast<void*>(&Slot<R,const CFrameWnd,Args...>::call);
    slotCalls=0;(slotObject->*method)(std::forward<Values>(values)...);
    check(slotCalls==1,"const frame declarations use their native virtual positions");frameTable[Index]=old;
}
struct ProtectedFrame:CFrameWnd {
    using CFrameWnd::OnCreateClient;
    using CFrameWnd::OnShowMenuBar;
    using CFrameWnd::OnHideMenuBar;
};
void frameABI() {
    MSG message;reset(message);slotObject=objects[0].get();
    RECT rect{};alignas(CString) BYTE stringBytes[sizeof(CString)]{};auto& string=*reinterpret_cast<CString*>(stringBytes);
    probe<91>(&CFrameWnd::Create,L"class",L"caption",DWORD(0),rect,nullptr,nullptr,DWORD(0),nullptr);
    probe<92>(&CFrameWnd::LoadFrame,UINT(0),DWORD(0),nullptr,nullptr);
    probe<93>(&CFrameWnd::GetActiveDocument);probe<94>(&CFrameWnd::GetActiveFrame);
    probe<95>(&CFrameWnd::GetMessageString,UINT(0),string);probe<96>(&CFrameWnd::RecalcLayout,TRUE);
    probe<97>(&CFrameWnd::ActivateFrame,SW_SHOW);probe<98>(&CFrameWnd::GetTrackingID);
    probe<99>(&CFrameWnd::SetMenuBarVisibility,DWORD(0));probe<100>(&CFrameWnd::GetMenuBarVisibility);
    probe<101>(&CFrameWnd::SetMenuBarState,DWORD(0));probe<102>(&CFrameWnd::GetMenuBarState);
    probe<103>(&CFrameWnd::OnSetPreviewMode,FALSE,nullptr);probe<104>(&CFrameWnd::GetMessageBar);
    probe<105>(&CFrameWnd::NegotiateBorderSpace,UINT(0),&rect);probe<106>(&ProtectedFrame::OnCreateClient,nullptr,nullptr);
    probe<107>(&CFrameWnd::OnUpdateFrameTitle,FALSE);probe<108>(&CFrameWnd::OnUpdateFrameMenu,nullptr);
    probe<109>(&CFrameWnd::GetDefaultAccelerator);probe<110>(&CFrameWnd::DelayUpdateFrameMenu,nullptr);
    probe<111>(&CFrameWnd::ExitHelpMode);probe<112>(&ProtectedFrame::OnShowMenuBar);probe<113>(&ProtectedFrame::OnHideMenuBar);
    for(UINT id : {0u,1u,0xdfffu,0xe000u,0xe001u,0xe002u,0xe003u,0xffffu,0x10000u,0x80000000u,UINT_MAX}) {
        reset(message);trackingID=id;objects[0].get()->m_nIDTracking=id;
        check(active->trackingID(objects[0].get())==id,"the base tracking identifier uses the native 0x168 field");
        bool wanted=id!=0 && id!=0xe001 && id!=0xe002;
        check(active->tracking(objects[0].get())==BOOL(wanted),"tracking excludes exactly the native noncommand identifiers");
        expected({2},"tracking consults virtual GetTrackingID exactly once, independently of HWND capture");
    }
}
void visibilityCallbacks() {
    MSG message;reset(message);slotObject=objects[0].get();
    auto* oldShow=frameTable[112];auto* oldHide=frameTable[113];
    frameTable[112]=frameTable[113]=reinterpret_cast<void*>(&Slot<void,CFrameWnd>::call);slotCalls=0;
    BYTE before[sizeof(CFrameWnd)];std::memcpy(before,slotObject,sizeof(before));
    HMENU menu=CreateMenu();check(menu!=nullptr,"create menu for base visibility callbacks");SetMenu(handles[0],menu);
    active->showBase(slotObject);active->hideBase(slotObject);
    check(slotCalls==0 && !std::memcmp(before,slotObject,sizeof(before)) && GetMenu(handles[0])==menu,
          "base menu visibility exports bypass overrides and leave window and frame state unchanged");
    active->showBase(reinterpret_cast<CFrameWnd*>(UINT_PTR(1)));
    active->hideBase(reinterpret_cast<CFrameWnd*>(UINT_PTR(1)));
    SetMenu(handles[0],nullptr);DestroyMenu(menu);frameTable[112]=oldShow;frameTable[113]=oldHide;
}

void hooks() {
    MSG message;reset(message);threadTable[30]=reinterpret_cast<void*>(active->filter);
    check(active->hook(MSGF_DIALOGBOX,UINT_PTR(0xfedcba9876543210ull),reinterpret_cast<LPARAM>(&message))==1,
          "the production/native filter hook reaches the real base filter with shared module and thread state");
    expected({10,30},"hook integration preserves default filter routing");
    reset(message);message.message=WM_PAINT;
    check(active->hook(MSGF_DIALOGBOX,0,reinterpret_cast<LPARAM>(&message))==0,
          "the integrated hook rejects nonkeyboard dialog messages");expected({10},"an integrated default filter does not invoke pretranslation for paint");
    if(!active->native) {
        reset(message);threadTable[30]=reinterpret_cast<void*>(&unexpectedFilter);
        check(threads[0].get()->CWinThread::ProcessMessageFilter(MSGF_DIALOGBOX,&message)==TRUE,
              "the natural qualified base method reaches the production filter without virtual redispatch");
        expected({10,30},"the natural base method retains the filter's virtual callback semantics");
    }
    threadTable[30]=reinterpret_cast<void*>(&unexpectedFilter);
}
void run(const API& api) {
    active=&api;auto* oldModule=api.thread()->m_pModuleState;
    api.thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(module);state=api.state();
    CHandleMap map(&windowClass,construct,destroy,64,1);windowMap=&map;state->m_pmapHWND=&map;
    for(unsigned i=0;i<5;++i)map.m_permanentMap[handles[i]]=objects[i].get();
    frameABI();dialogs();menus();hooks();visibilityCallbacks();
    state->m_pmapHWND=nullptr;state->m_pCurrentWinThread=nullptr;api.thread()->m_bInMsgFilter=FALSE;
    api.thread()->m_pModuleState=oldModule;api.destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));
    std::memset(module,0,sizeof(module));std::printf("%s message filtering completed\n",api.native ? "Native":"Source");
}
UINT MS_ABI baseTrackingID(CFrameWnd* self) {return self->CFrameWnd::GetTrackingID();}
[[noreturn]] void AFXAPI unexpectedVirtual() {std::abort();}
} // namespace
// Wrapper access uses real maps. Unexercised window construction, thread
// deletion and allocation failures are explicit aborting/throwing boundaries.
LRESULT CALLBACK AfxWndProc(HWND,UINT,WPARAM,LPARAM) {std::abort();}
extern "C" void MS_ABI impl__Delete_CWinThread__UEAAXXZ(CWinThread*) {std::abort();}
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() {throw 91;}
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() {throw 19;}
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long value) {throw value;}
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {
    return static_cast<CWnd*>(impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pmapHWND->FromHandle(handle));
}
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {
    return static_cast<CWnd*>(impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pmapHWND->m_permanentMap.GetValueAt(handle));
}
int main() {
    for(auto& entry:frameTable)entry=reinterpret_cast<void*>(&unexpectedVirtual);
    frameTable[86]=reinterpret_cast<void*>(&isFrame);frameTable[98]=reinterpret_cast<void*>(&tracking);
    threadTable[24]=reinterpret_cast<void*>(&pre);threadTable[31]=reinterpret_cast<void*>(&getMain);
    threadTable[30]=reinterpret_cast<void*>(&unexpectedFilter);siteTable[43]=reinterpret_cast<void*>(&enabled);
    WNDCLASSW cls{};cls.hInstance=GetModuleHandleW(nullptr);cls.lpfnWndProc=procedure;cls.lpszClassName=L"OpenMfcMessageFilterFixture";
    check(RegisterClassW(&cls)!=0,"register filter fixture windows");
    for(unsigned i=0;i<5;++i) {
        HWND parent=i==1 || i==4 ? handles[0]:nullptr;
        handles[i]=CreateWindowExW(0,cls.lpszClassName,L"filter",parent ? WS_CHILD:WS_OVERLAPPEDWINDOW,
                                 0,0,50,50,parent,nullptr,cls.hInstance,nullptr);
        check(handles[i]!=nullptr,"create real filter fixture window");
    }
    API source{impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z,impl__IsTracking_CFrameWnd__QEAAHXZ,
        baseTrackingID,AfxGetThreadState,impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,
        impl___1CThreadLocalObject__QEAA_XZ,openmfc::detail::winthread::MessageFilterHook,false,
        impl__OnShowMenuBar_CFrameWnd__MEAAXXZ,impl__OnHideMenuBar_CFrameWnd__MEAAXXZ};
    if(!failures)run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_MESSAGE_FILTER_DLL")) {
        HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native message-filter oracle");
        if(image) {
            auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"message-filter oracle uses the pinned native image");
            if(pinned) {
                API native=source;native.native=true;
#define RVA(member,offset) native.member=reinterpret_cast<decltype(native.member)>(b+offset)
                RVA(filter,0x2747b0);RVA(tracking,0x291d40);RVA(trackingID,0x29340);
                RVA(thread,0x1332a0);RVA(state,0x133a20);RVA(destroyLocal,0x14d0d0);RVA(hook,0x274720);RVA(showBase,0x27d0);RVA(hideBase,0x27d0);
#undef RVA
                run(native);
            }
            FreeLibrary(image);
        }
    }
    DestroyWindow(handles[0]);DestroyWindow(handles[2]);DestroyWindow(handles[3]);UnregisterClassW(cls.lpszClassName,cls.hInstance);
    std::printf("Thread message filtering: %d failures\n",failures);return failures ? 1:0;
}
