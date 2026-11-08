// Real dispatch/pretranslation owners, real CHandleMap storage, and explicit
// boundaries for module selection and window-wrapper lifetime.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
BOOL MS_ABI impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z(CWinThread*,MSG*);
void MS_ABI impl__DispatchThreadMessage_CWinThread__IEAAXPEAUtagMSG___Z(CWinThread*,MSG*);
BOOL MS_ABI impl__WalkPreTranslateTree_CWnd__SAHPEAUHWND____PEAUtagMSG___Z(HWND,MSG*);
BOOL MS_ABI impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(MSG*);
BOOL MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(MSG*);
BOOL MS_ABI impl__PreTranslateMessage_CWinThread__UEAAHPEAUtagMSG___Z(CWinThread*,MSG*);
LRESULT MS_ABI impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(CException*,const MSG*);
LRESULT MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(CException*,const MSG*);
LRESULT MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(CWinThread*,CException*,const MSG*);
}
namespace {
int failures;
void check(bool condition,const char* label) {
    if(!condition) {++failures;std::printf("FAIL: %s\n",label);}
}
struct API {
    decltype(&impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z) dispatch;
    decltype(&impl__DispatchThreadMessage_CWinThread__IEAAXPEAUtagMSG___Z) dispatchVoid;
    decltype(&impl__WalkPreTranslateTree_CWnd__SAHPEAUHWND____PEAUtagMSG___Z) walk;
    decltype(&impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z) internal;
    decltype(&impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z) pre;
    decltype(&impl__PreTranslateMessage_CWinThread__UEAAHPEAUtagMSG___Z) base;
    AFX_MODULE_THREAD_STATE* (MS_ABI* state)();
    _AFX_THREAD_STATE* (MS_ABI* threadState)();
    bool native;
    decltype(&impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z) exceptionInternal;
    decltype(&impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z) exceptionPublic;
    decltype(&impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z) exceptionBase;
};
struct Event {
    int kind;UINT_PTR a,b;
    bool operator==(const Event& rhs) const {return kind==rhs.kind && a==rhs.a && b==rhs.b;}
};
std::vector<Event> events;
int throwAt;
void event(int kind,UINT_PTR a=0,UINT_PTR b=0) {
    events.push_back({kind,a,b});
    if(throwAt==kind)throw kind;
}
template<size_t Size> struct alignas(16) Object {
    BYTE bytes[Size];
    template<class T> T* as() {return reinterpret_cast<T*>(bytes);}
    void reset(void** table) {std::memset(bytes,0xa5,Size);*reinterpret_cast<void***>(bytes)=table;}
};
Object<sizeof(CWinThread)> threadA,threadB;
Object<sizeof(CWnd)> windows[5];
void* threadTable[33]{};
void* windowTable[100]{};
HWND handles[5]{};
const API* active;
AFX_MODULE_THREAD_STATE* routingState;
alignas(16) BYTE sourceStateBytes[sizeof(AFX_MODULE_THREAD_STATE)]{};
alignas(16) BYTE sourceThreadBytes[sizeof(_AFX_THREAD_STATE)]{};
AFX_MODULE_THREAD_STATE* sourceState() {return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(sourceStateBytes);}
_AFX_THREAD_STATE* sourceThreadState() {return reinterpret_cast<_AFX_THREAD_STATE*>(sourceThreadBytes);}
const AFX_MSGMAP *selectedMap,*selectedBase;
const AFX_MSGMAP_ENTRY endEntry{};
const AFX_MSGMAP rootMap{nullptr,&endEntry};
MSG* currentMessage;
bool changeThread,changeMessage,changeWindowHandle;
CWinThread* replacementThread;
BOOL windowResults[5]{};
BOOL threadResult;
CWnd* mainWindow;
unsigned sourceStateGets,sourcePermanentGets,sourceTemporaryGets;
unsigned windowIndex(const CWnd* p) {
    for(unsigned i=0;i<5;++i)if(p==windows[i].as<CWnd>())return i;
    std::abort();
}
const AFX_MSGMAP* AFXAPI getMap(const CWinThread* thread) {
    event(thread==threadA.as<CWinThread>() ? 10:11);
    if(changeThread)routingState->m_pCurrentWinThread=replacementThread;
    if(changeMessage) {
        currentMessage->message=WM_APP+7;
        currentMessage->wParam=UINT_PTR(0xabcdef0123456789ull);
        currentMessage->lParam=LPARAM(0xfedcba9876543210ull);
    }
    return selectedMap;
}
const AFX_MSGMAP* AFXAPI baseMap() {event(12);return selectedBase;}
const AFX_MSGMAP* AFXAPI root() {event(13);return &rootMap;}
void AFXAPI handler(CWinThread* self,WPARAM w,LPARAM l) {
    check(self==threadA.as<CWinThread>(),"thread handler receives the original thread object");
    event(20,w,UINT_PTR(l));
}
AFX_PMSG memberHandler() {
    AFX_PMSG result;
    auto fn=&handler;
    static_assert(sizeof(fn)==sizeof(result),"single-inheritance member handler storage");
    std::memcpy(&result,&fn,sizeof(result));return result;
}
BOOL AFXAPI preWindow(CWnd* window,MSG* message) {
    unsigned i=windowIndex(window);event(100+int(i));
    if(changeWindowHandle && i==2) {
        message->hwnd=handles[4];
        window->m_hWnd=handles[4];
    }
    return windowResults[i];
}
CWnd* AFXAPI getMain(CWinThread* thread) {
    event(thread==threadA.as<CWinThread>() ? 30:31);return mainWindow;
}
BOOL AFXAPI preThread(CWinThread*,MSG*) {event(40);return threadResult;}
LRESULT exceptionResult;
CException* expectedException;
const MSG* expectedExceptionMessage;
LRESULT AFXAPI threadException(CWinThread* thread,CException* error,const MSG* message) {
    check(thread==threadA.as<CWinThread>(),"exception dispatch uses the registered thread");
    check(error==expectedException && message==expectedExceptionMessage,
          "exception dispatch preserves both original arguments");
    event(50);
    return exceptionResult;
}

CRuntimeClass windowClass{"RoutingFixtureWindow",sizeof(CWnd),0xffff,nullptr,nullptr,nullptr,nullptr};
void AFXAPI unexpectedConstruction(CObject*) {std::abort();}
void AFXAPI unexpectedDestruction(CObject*) {std::abort();}

void reset(const API& api) {
    active=&api;routingState=api.state();events.clear();throwAt=0;
    selectedMap=&rootMap;selectedBase=&rootMap;currentMessage=nullptr;
    changeThread=changeMessage=changeWindowHandle=false;
    replacementThread=threadB.as<CWinThread>();
    std::memset(windowResults,0,sizeof(windowResults));threadResult=0;mainWindow=nullptr;
    threadA.reset(threadTable);threadB.reset(threadTable);
    routingState->m_pCurrentWinThread=nullptr;
    for(unsigned i=0;i<5;++i) {
        std::memset(windows[i].bytes,0,sizeof(windows[i].bytes));
        *reinterpret_cast<void***>(windows[i].bytes)=windowTable;
        windows[i].as<CWnd>()->m_hWnd=handles[i];
    }
    sourceStateGets=sourcePermanentGets=sourceTemporaryGets=0;
}
void expected(std::initializer_list<int> kinds,const char* label) {
    std::vector<int> actual;for(const auto& e:events)actual.push_back(e.kind);
    check(actual==std::vector<int>(kinds),label);
}

void threadMaps(const API& api) {
    for(UINT id : {UINT(WM_NULL),UINT(WM_APP+7),UINT(0xbfff),UINT(0xc001),UINT(0xffffffff)}) {
        reset(api);MSG message{};message.message=id;message.hwnd=handles[2];
        message.wParam=UINT_PTR(0xfedcba9876543210ull);message.lParam=LPARAM(0xabcdef0123456789ull);
        UINT registered=id;
        UINT_PTR sig=id>=0xc000 ? reinterpret_cast<UINT_PTR>(&registered) : UINT_PTR(0xdeadbeef);
        UINT mapID=id>=0xc000 ? 0xc000:id;
        AFX_MSGMAP_ENTRY entries[]={{mapID,1,0,0,sig,memberHandler()},
            {mapID,0,1,0,sig,memberHandler()},{mapID,0,0,0xffffffff,sig,memberHandler()},endEntry};
        AFX_MSGMAP map{root,entries};selectedMap=&map;
        BYTE before[sizeof(threadA.bytes)];std::memcpy(before,threadA.bytes,sizeof(before));MSG old=message;
        MSG pumped=api.threadState()->m_msgCur;
        check(api.dispatch(threadA.as<CWinThread>(),&message)==TRUE,"thread map selects a normal or registered message");
        expected({10,20},"thread dispatch skips control entries and calls one handler");
        check(events.size()==2 && events.back().a==message.wParam && events.back().b==UINT_PTR(message.lParam),
              "thread handler preserves full-width parameters");
        check(!std::memcmp(before,threadA.bytes,sizeof(before)) && !std::memcmp(&old,&message,sizeof(old)),
              "dispatch does not overwrite thread or message storage");
        check(!std::memcmp(&pumped,&api.threadState()->m_msgCur,sizeof(pumped)),
              "direct thread dispatch leaves the pumped-message record unchanged");
        events.clear();api.dispatchVoid(threadA.as<CWinThread>(),&message);
        expected({10,20},"void dispatcher uses the same handler lookup");
    }
    reset(api);MSG message{};message.message=WM_APP+7;message.wParam=5;message.lParam=-9;
    AFX_MSGMAP_ENTRY match[]={{WM_APP+7,0,0,0,1,memberHandler()},endEntry};
    AFX_MSGMAP parent{root,match},derived{baseMap,&endEntry};selectedMap=&derived;selectedBase=&parent;
    check(api.dispatch(threadA.as<CWinThread>(),&message)==TRUE,"thread dispatch searches inherited maps");
    expected({10,12,20},"inherited-map getter precedes handler dispatch");
    AFX_MSGMAP overriding{baseMap,match};selectedMap=&overriding;events.clear();
    check(api.dispatch(threadA.as<CWinThread>(),&message)==TRUE,"a derived handler overrides its inherited map");
    expected({10,20},"a derived match bypasses the base getter");
    events.clear();selectedMap=&rootMap;
    check(api.dispatch(threadA.as<CWinThread>(),&message)==FALSE,"unhandled thread messages return FALSE");
    expected({10},"the terminal root map is not searched");
    AFX_MSGMAP terminal{nullptr,match};selectedMap=&terminal;events.clear();
    check(!api.dispatch(threadA.as<CWinThread>(),&message),"terminal map entries are ignored even if they match");
    AFX_MSGMAP_ENTRY terminated[]={{WM_APP+7,0,0,0,0,memberHandler()},match[0],endEntry};
    AFX_MSGMAP stopped{root,terminated};selectedMap=&stopped;events.clear();
    check(!api.dispatch(threadA.as<CWinThread>(),&message),"zero signature terminates the entry array");
    reset(api);selectedMap=&parent;message.message=1;currentMessage=&message;changeMessage=true;
    check(api.dispatch(threadA.as<CWinThread>(),&message)==TRUE && events.back().a==message.wParam &&
          events.back().b==UINT_PTR(message.lParam),"dispatch reads message contents after the virtual map getter");
    reset(api);message.message=0xc123;
    UINT registered=0xc123,wrong=0xc234;
    AFX_MSGMAP_ENTRY registeredEntries[]={{0xc123,0,0,0,reinterpret_cast<UINT_PTR>(&registered),memberHandler()},
        {0xc000,0,0,0,reinterpret_cast<UINT_PTR>(&wrong),memberHandler()},
        {0xc000,0,0,0,reinterpret_cast<UINT_PTR>(&registered),memberHandler()},endEntry};
    AFX_MSGMAP registeredMap{root,registeredEntries};selectedMap=&registeredMap;
    check(api.dispatch(threadA.as<CWinThread>(),&message)==TRUE,"registered messages use the sentinel and referenced runtime identifier");
    registered=0xc456;events.clear();
    check(!api.dispatch(threadA.as<CWinThread>(),&message),"changing the registered-message variable changes lookup without rebuilding the map");
    message.message=WM_APP+7;
    for(int fault : {10,12,20}) {
        reset(api);selectedMap=&derived;selectedBase=&parent;throwAt=fault;bool caught=false;
        try {api.dispatch(threadA.as<CWinThread>(),&message);}catch(int value) {caught=value==fault;}
        check(caught,"thread map and handler exceptions propagate to the caller");
    }
    throwAt=0;
}

void windowWalk(const API& api,CHandleMap& map) {
    reset(api);MSG message{};message.hwnd=handles[2];
    check(!api.walk(nullptr,&message),"NULL stop still permits walking all ancestors");
    expected({102,101,100},"window pretranslation walks target, parent, then root");
    events.clear();check(!api.walk(handles[1],&message),"walk ends after testing the stop window");
    expected({102,101},"the stop window is included without scanning its ancestors or siblings");
    events.clear();windowResults[1]=69;
    check(api.walk(nullptr,&message)==TRUE,"handled window messages return normalized TRUE");
    expected({102,101},"a handled message stops parent traversal");
    windowResults[1]=0;events.clear();map.m_permanentMap.RemoveKey(handles[1]);
    check(!api.walk(nullptr,&message),"windows without permanent wrappers are skipped");
    expected({102,100},"parent lookup does not create temporary wrappers");
    map.m_permanentMap[handles[1]]=windows[1].as<CWnd>();
    events.clear();map.m_permanentMap.RemoveKey(handles[1]);
    check(!api.walk(handles[1],&message),"an unwrapped stop window still ends traversal");
    expected({102},"an unwrapped stop does not permit traversal to its parent");
    map.m_permanentMap[handles[1]]=windows[1].as<CWnd>();
    events.clear();changeWindowHandle=true;
    check(!api.walk(nullptr,&message),"callback mutations do not change the captured ancestor chain");
    expected({102,101,100},"walk uses its original target handle after callback mutations");
    check(map.m_temporaryMap.m_nCount==0,"tree walking leaves the temporary map empty");
    changeWindowHandle=false;message.hwnd=handles[2];events.clear();throwAt=101;bool caught=false;
    try {api.walk(nullptr,&message);}catch(int value) {caught=value==101;}
    check(caught,"pretranslation callback exceptions propagate");throwAt=0;
}

void pretranslation(const API& api) {
    reset(api);MSG message{};message.hwnd=handles[2];
    check(!api.internal(&message),"internal pretranslation works without a thread object");
    expected({102,101,100},"no-thread path walks the window parent chain");
    events.clear();windowResults[1]=69;
    check(api.pre(&message)==TRUE,"global fallback uses internal parent pretranslation");
    expected({102,101},"global fallback terminates at the handled parent");

    reset(api);routingState->m_pCurrentWinThread=threadA.as<CWinThread>();mainWindow=windows[0].as<CWnd>();
    message.hwnd=handles[2];
    check(!api.internal(&message),"an unhandled message under the main window remains unhandled");
    expected({30,102,101,100},"main-window pretranslation is not repeated for its own descendants");
    events.clear();message.hwnd=handles[3];windowResults[0]=69;
    check(api.internal(&message)==69,"foreign top-level fallback preserves the main callback's raw BOOL");
    expected({30,103,100},"foreign window is tried before main-window fallback");
    events.clear();windowResults[3]=69;
    check(api.internal(&message)==TRUE,"parent-tree handling is normalized before main fallback");
    expected({30,103},"handled foreign windows skip main-window fallback");
    events.clear();threadResult=69;
    check(api.pre(&message)==69,"global pretranslation preserves virtual thread result");
    expected({40},"global path calls virtual thread pretranslation");
    events.clear();windowResults[3]=0;
    check(api.base(nullptr,&message)==69,"base pretranslation ignores its this pointer and uses module thread state");
    expected({30,103,100},"base export bypasses the thread's overriding pretranslation method");

    reset(api);routingState->m_pCurrentWinThread=threadA.as<CWinThread>();
    AFX_MSGMAP_ENTRY entries[]={{WM_APP+7,0,0,0,1,memberHandler()},endEntry};
    AFX_MSGMAP match{root,entries};selectedMap=&match;message={};message.message=WM_APP+7;
    check(api.internal(&message)==TRUE,"thread messages are consumed by their message map");
    expected({10,20},"handled thread messages skip main-window lookup");
    events.clear();selectedMap=&rootMap;mainWindow=windows[0].as<CWnd>();windowResults[0]=69;
    check(api.internal(&message)==69,"unhandled NULL-HWND messages can reach main-window fallback");
    expected({10,30,100},"NULL-HWND fallback calls the main window after map lookup");
    events.clear();changeThread=true;
    check(api.internal(&message)==69,"pretranslation reloads the active thread after map callbacks");
    expected({10,31,100},"a changed module-thread registration controls the following main-window lookup");
    events.clear();routingState->m_pCurrentWinThread=threadA.as<CWinThread>();replacementThread=nullptr;
    check(!api.internal(&message),"clearing the active thread during map lookup suppresses following main-window routing");
    expected({10},"a cleared thread registration is not reused");
    for(int fault : {30,103,100,40}) {
        reset(api);routingState->m_pCurrentWinThread=threadA.as<CWinThread>();mainWindow=windows[0].as<CWnd>();
        message.hwnd=handles[3];throwAt=fault;bool caught=false;
        try {if(fault==40)api.pre(&message);else api.internal(&message);}catch(int value) {caught=value==fault;}
        check(caught,"pretranslation lookup and window exceptions propagate");
    }
    throwAt=0;
}

void exceptionRouting(const API& api) {
    // These release handlers never inspect or own the exception. A poisoned
    // pointer also detects accidental diagnostic/deletion calls.
    auto* error=reinterpret_cast<CException*>(UINT_PTR(1));
    MSG message{};message.hwnd=handles[0];message.wParam=UINT_PTR(0xfedcba9876543210ull);
    message.lParam=LPARAM(0xabcdef0123456789ull);
    for(bool registered : {false,true}) {
        for(int route=0;route<3;++route) {
            for(UINT id : {UINT(WM_CREATE),UINT(WM_PAINT),UINT(WM_NCCREATE),UINT(WM_APP+7),UINT(-1)}) {
                reset(api);routingState->m_pCurrentWinThread=registered ? threadA.as<CWinThread>() : nullptr;
                message.message=id;MSG before=message;
                // Paint failures must consume the update region; other
                // failures must leave it for the normal message path.
                ShowWindow(handles[0],SW_SHOWNOACTIVATE);
                ValidateRect(handles[0],nullptr);
                RECT dirty{1,1,8,8};InvalidateRect(handles[0],&dirty,FALSE);
                check(GetUpdateRect(handles[0],nullptr,FALSE)!=FALSE,"prepare exception paint region");
                sourceStateGets=0;
                LRESULT result=0;
                if(route==0)result=api.exceptionInternal(error,&message);
                else if(route==1)result=api.exceptionBase(reinterpret_cast<CWinThread*>(UINT_PTR(1)),error,&message);
                else {
                    // The no-thread branch of the public export uses the
                    // same internal behavior. Registered overrides are below.
                    routingState->m_pCurrentWinThread=nullptr;
                    result=api.exceptionPublic(error,&message);
                }
                check(result==(id==WM_CREATE ? -1:0),"release exception result follows the message kind");
                check((GetUpdateRect(handles[0],nullptr,FALSE)==FALSE)==(id==WM_PAINT),
                      "only a paint exception validates the update region");
                check(events.empty(),"internal and base exception handling bypass thread overrides");
                check(!std::memcmp(&before,&message,sizeof(message)),"exception handling preserves the input message");
                if(!api.native)check(sourceStateGets==unsigned(route==2),
                    "only the public exception export consults module thread state");
                ValidateRect(handles[0],nullptr);
            }
        }
    }
    reset(api);routingState->m_pCurrentWinThread=threadA.as<CWinThread>();
    expectedException=error;expectedExceptionMessage=&message;
    for(LRESULT result : {LRESULT(0),LRESULT(69),LRESULT(-17),LRESULT(0xfedcba9876543210ull)}) {
        events.clear();exceptionResult=result;message.message=WM_CREATE;
        check(api.exceptionPublic(error,&message)==result,"public exception dispatch preserves a full-width virtual result");
        expected({50},"the public export invokes the current thread's exception slot exactly once");
    }
    events.clear();throwAt=50;bool caught=false;
    try {api.exceptionPublic(error,&message);}catch(int value) {caught=value==50;}
    check(caught,"exceptions from the thread override propagate");
    throwAt=0;
}

void run(const API& api) {
    CHandleMap map(&windowClass,unexpectedConstruction,unexpectedDestruction,64,1);
    auto* state=api.state();auto* oldMap=state->m_pmapHWND;auto* oldThread=state->m_pCurrentWinThread;
    state->m_pmapHWND=&map;
    reset(api);
    for(unsigned i=0;i<5;++i)map.m_permanentMap[handles[i]]=windows[i].as<CWnd>();
    threadMaps(api);windowWalk(api,map);pretranslation(api);exceptionRouting(api);
    check(map.m_temporaryMap.m_nCount==0,"routing never materializes unnecessary temporary windows");
    state->m_pmapHWND=oldMap;state->m_pCurrentWinThread=oldThread;
}
}

// Actual wrapper storage is shared with native MFC during oracle runs. These
// boundaries supply the tested caller's module and already-registered windows.
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    ++sourceStateGets;return sourceState();
}
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND h) {
    ++sourcePermanentGets;auto* map=sourceState()->m_pmapHWND;
    return map ? static_cast<CWnd*>(map->m_permanentMap.GetValueAt(h)) : nullptr;
}
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND h) {
    ++sourceTemporaryGets;return static_cast<CWnd*>(sourceState()->m_pmapHWND->FromHandle(h));
}
extern "C" CWnd* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd* window) {
    if(!window || !window->m_hWnd)return nullptr;
    HWND handle=window->m_hWnd;
    while(HWND parent=GetParent(handle))handle=parent;
    return static_cast<CWnd*>(sourceState()->m_pmapHWND->m_permanentMap.GetValueAt(handle));
}
extern "C" void* AFXAPI impl___2_YAPEAX_K_Z(size_t n) {return std::malloc(n);}
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() {throw 91;}
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() {throw 19;}
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() {return sourceThreadState();}
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(
    AFX_MODULE_THREAD_STATE::NewHandler next) {
    auto* state=sourceState();auto previous=state->m_pfnNewHandler;state->m_pfnNewHandler=next;return previous;
}

int main() {
    threadTable[12]=reinterpret_cast<void*>(&getMap);threadTable[24]=reinterpret_cast<void*>(&preThread);
    threadTable[29]=reinterpret_cast<void*>(&threadException);
    threadTable[31]=reinterpret_cast<void*>(&getMain);windowTable[69]=reinterpret_cast<void*>(&preWindow);
    handles[0]=CreateWindowExW(0,L"STATIC",L"routing root",WS_OVERLAPPEDWINDOW,0,0,100,100,nullptr,nullptr,nullptr,nullptr);
    handles[1]=CreateWindowExW(0,L"STATIC",L"routing child",WS_CHILD,0,0,50,50,handles[0],nullptr,nullptr,nullptr);
    handles[2]=CreateWindowExW(0,L"STATIC",L"routing grandchild",WS_CHILD,0,0,25,25,handles[1],nullptr,nullptr,nullptr);
    handles[3]=CreateWindowExW(0,L"STATIC",L"foreign root",WS_OVERLAPPEDWINDOW,0,0,100,100,nullptr,nullptr,nullptr,nullptr);
    handles[4]=CreateWindowExW(0,L"STATIC",L"unrelated sibling",WS_CHILD,0,0,25,25,handles[0],nullptr,nullptr,nullptr);
    for(HWND h:handles)check(h!=nullptr,"create routing fixture window");
    API source{impl__DispatchThreadMessageEx_CWinThread__IEAAHPEAUtagMSG___Z,
        impl__DispatchThreadMessage_CWinThread__IEAAXPEAUtagMSG___Z,
        impl__WalkPreTranslateTree_CWnd__SAHPEAUHWND____PEAUtagMSG___Z,
        impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z,impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z,
        impl__PreTranslateMessage_CWinThread__UEAAHPEAUtagMSG___Z,
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,sourceThreadState,false,
        impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z,
        impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z,
        impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z};
    if(!failures)run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_MESSAGE_ROUTING_DLL")) {
        HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native message-routing oracle");
        if(image) {
            auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"message-routing oracle uses the pinned native image");
            if(pinned) {
                API native=source;native.native=true;
#define RVA(member,offset) native.member=reinterpret_cast<decltype(native.member)>(b+offset)
                RVA(dispatch,0x274640);RVA(dispatchVoid,0x274630);RVA(walk,0x28ef00);
                RVA(internal,0x273c00);RVA(pre,0x273cb0);RVA(base,0x274700);RVA(state,0x133a20);
                RVA(threadState,0x1332a0);RVA(exceptionInternal,0x273b80);RVA(exceptionPublic,0x273bb0);
                RVA(exceptionBase,0x274710);
#undef RVA
                run(native);
            }
            FreeLibrary(image);
        }
    }
    DestroyWindow(handles[0]);DestroyWindow(handles[3]);
    std::printf("Thread message routing: %d failures\n",failures);
    return failures ? 1:0;
}
