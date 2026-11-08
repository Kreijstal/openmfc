#pragma once
// Window lifecycle is a boundary here; its production constructor/handle maps
// have independent native window tests. Thread/TLS and map storage are real.
#include "detail/ThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <process.h>
#include <malloc.h>
IMPLEMENT_DYNAMIC(CException,CObject)
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const {std::abort();}
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) {std::abort();}
int CException::ReportError(UINT,UINT) {std::abort();}
void CException::Dump() const {std::abort();}
void CException::AssertValid() const {std::abort();}
extern "C" CWnd* MS_ABI impl___0CWnd__QEAA_XZ(CWnd* window) {
    std::memset(window,0,sizeof(CWnd));window->m_pModuleState=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();return window;
}
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(CWnd* window) {check(!window->m_hWnd,"startup releases the fallback window before destruction");}
extern "C" BOOL MS_ABI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd* window,HWND handle) {window->m_hWnd=handle;return TRUE;}
extern "C" HWND MS_ABI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd* window) {auto h=window->m_hWnd;window->m_hWnd=nullptr;return h;}
extern "C" {
CWinThread* AFXAPI impl__AfxBeginThread__YAPEAVCWinThread__P6AIPEAX_Z0HIKPEAU_SECURITY_ATTRIBUTES___Z(AFX_THREADPROC,void*,int,UINT,DWORD,SECURITY_ATTRIBUTES*);
CWinThread* AFXAPI impl__AfxBeginThread__YAPEAVCWinThread__PEAUCRuntimeClass__HIKPEAU_SECURITY_ATTRIBUTES___Z(CRuntimeClass*,int,UINT,DWORD,SECURITY_ATTRIBUTES*);
void* MS_ABI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(S_Cthreadlocalobject*,void*);
void* MS_ABI impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(S_Cthreadlocalobject*);
void MS_ABI impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z(HINSTANCE);
BOOL MS_ABI impl__CreateThread_CWinThread__QEAAHKIPEAU_SECURITY_ATTRIBUTES___Z(CWinThread*,DWORD,UINT,SECURITY_ATTRIBUTES*);
void MS_ABI impl__AfxLockTempMaps__YAXXZ();
BOOL MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int);
void MS_ABI impl__AfxInitThread__YAXXZ();
void MS_ABI impl__AfxTermThread__YAXPEAUHINSTANCE_____Z(HINSTANCE);
void MS_ABI impl__AfxEndThread__YAXIH_Z(UINT,BOOL);
}
namespace openmfc {namespace detail {namespace winthread {
LRESULT CALLBACK MessageFilterHook(int,WPARAM,LPARAM);
}}}
namespace startup_test {
struct API {
    const ::API* core;
    decltype(&impl__CreateThread_CWinThread__QEAAHKIPEAU_SECURITY_ATTRIBUTES___Z) create;
    decltype(&impl__AfxLockTempMaps__YAXXZ) lock;
    decltype(&impl__AfxUnlockTempMaps__YAHH_Z) unlock;
    decltype(&impl__AfxInitThread__YAXXZ) init;
    decltype(&impl__AfxTermThread__YAXPEAUHINSTANCE_____Z) term;
    decltype(&impl__AfxEndThread__YAXIH_Z) end;
    CWinThread* (AFXAPI* beginWorker)(AFX_THREADPROC,void*,int,UINT,DWORD,SECURITY_ATTRIBUTES*);
    CWinThread* (AFXAPI* beginUI)(CRuntimeClass*,int,UINT,DWORD,SECURITY_ATTRIBUTES*);
    void* (AFXAPI* get)(S_Cthreadlocalobject*,void*);
    void (AFXAPI* destroyLocal)(S_Cthreadlocalobject*);
    LRESULT (CALLBACK* hook)(int,WPARAM,LPARAM);
    HMODULE image;
    void* (AFXAPI* peek)(S_Cthreadlocalobject*);
    void (AFXAPI* assign)(HINSTANCE);
};
static const API* active;
static std::vector<int> trace;
static void record(int event) {trace.push_back(event);}
struct Startup { _AFX_THREAD_STATE* parent;CWinThread* thread;DWORD flags,reserved14;void* reserved18;HANDLE ready,start;BOOL failed;DWORD padding;};
static Startup* parameters;
static CWinThread* watched;
static unsigned eventCalls,scenario,resumeCalls;
static bool failFinalResume,spyWorkerDelete;
static void* copiedThreadTable[33];
static void (AFXAPI* originalDelete)(CWinThread*);
static void AFXAPI workerDeleteSpy(CWinThread*);
static DWORD expectedFlags;
static UINT expectedStack;
static SECURITY_ATTRIBUTES* expectedSecurity;
static HANDLE first=reinterpret_cast<HANDLE>(UINT_PTR(0x11110000)),second=reinterpret_cast<HANDLE>(UINT_PTR(0x22220000));
static HANDLE child=reinterpret_cast<HANDLE>(UINT_PTR(0x33330000));
static HANDLE WINAPI createEvent(SECURITY_ATTRIBUTES* security,BOOL manual,BOOL signaled,const wchar_t* name) {
    check(!security && manual && !signaled && !name,"startup events are private, manual-reset and initially nonsignaled");
    record(eventCalls ? 2:1);++eventCalls;
    if(scenario==eventCalls)return nullptr;
    return eventCalls==1 ? first:second;
}
static uintptr_t AFXAPI begin(void* security,unsigned stack,unsigned (__stdcall* entry)(void*),void* opaque,unsigned flags,unsigned* id) {
    record(3);parameters=static_cast<Startup*>(opaque);watched=parameters->thread;
    check(security==expectedSecurity && stack==expectedStack && flags==(expectedFlags|CREATE_SUSPENDED) && entry,
        "CRT creation forwards security/stack/flags and always starts suspended");
    check(parameters->parent==active->core->thread() && parameters->flags==expectedFlags &&
          parameters->ready==first && parameters->start==second && !parameters->failed &&
          !parameters->reserved14 && !parameters->reserved18 && !parameters->padding,
          "parent publishes the native zero-initialized startup record");
    check(id==reinterpret_cast<unsigned*>(&watched->m_nThreadID),"CRT receives the actual thread-ID member");
    if(spyWorkerDelete) {
        std::memcpy(copiedThreadTable,*reinterpret_cast<void***>(watched),sizeof(copiedThreadTable));
        originalDelete=reinterpret_cast<decltype(originalDelete)>(copiedThreadTable[32]);
        copiedThreadTable[32]=reinterpret_cast<void*>(&workerDeleteSpy);*reinterpret_cast<void***>(watched)=copiedThreadTable;
    }
    *id=0x4567;return scenario==3 ? 0:reinterpret_cast<uintptr_t>(child);
}
static DWORD WINAPI resume(HANDLE handle) {check(handle==child,"resume uses the stored CRT thread handle");record(4);++resumeCalls;return failFinalResume && resumeCalls>1 ? DWORD(-1):1;}
static DWORD WINAPI suspend(HANDLE handle) {check(handle==child,"suspend uses the stored CRT thread handle");record(7);return 0;}
static DWORD WINAPI wait(HANDLE handle,DWORD timeout) {
    check(timeout==INFINITE,"startup synchronization waits without a timeout");
    if(handle==first) {record(5);if(scenario==4)parameters->failed=TRUE;}
    else {check(handle==child,"failed initialization joins its child");record(8);}
    return WAIT_OBJECT_0;
}
static BOOL WINAPI close(HANDLE handle) {record(handle==first ? 6:handle==second ? 10:9);return FALSE;}
static BOOL WINAPI signal(HANDLE handle) {check(handle==second,"success signals the child's continuation event");record(11);return FALSE;}
static void createProtocol(const API& api) {
    active=&api;trace.reserve(64);
    for(scenario=0;scenario<5;++scenario)for(DWORD flags : {DWORD(0),DWORD(CREATE_SUSPENDED|STACK_SIZE_PARAM_IS_A_RESERVATION)}) {
        Storage storage;storage.reset();auto* thread=api.core->construct(storage.object());
        SECURITY_ATTRIBUTES security{sizeof(security),reinterpret_cast<void*>(UINT_PTR(0x1234)),TRUE};
        expectedFlags=flags;expectedStack=0x234000;expectedSecurity=&security;eventCalls=0;trace.clear();
        BOOL result;
        {
            Patch event(api.image,"CreateEventW",reinterpret_cast<void*>(&createEvent));
            Patch crt(api.image,"_beginthreadex",reinterpret_cast<void*>(&begin));
            Patch r(api.image,"ResumeThread",reinterpret_cast<void*>(&resume));
            Patch s(api.image,"SuspendThread",reinterpret_cast<void*>(&suspend));
            Patch w(api.image,"WaitForSingleObject",reinterpret_cast<void*>(&wait));
            Patch c(api.image,"CloseHandle",reinterpret_cast<void*>(&close));
            Patch e(api.image,"SetEvent",reinterpret_cast<void*>(&signal));
            result=api.create(thread,flags,expectedStack,&security);
        }
        std::vector<int> expected{1,2};
        if(scenario==1)expected.push_back(10);
        else if(scenario==2)expected.push_back(6);
        else {
            expected.push_back(3);
            if(scenario==3) {expected.push_back(6);expected.push_back(10);}
            else {
                for(int event : {4,5,6})expected.push_back(event);
                if(flags&CREATE_SUSPENDED)expected.push_back(7);
                if(scenario==4)for(int event : {8,9,10})expected.push_back(event);
                else expected.push_back(11);
            }
        }
        check(trace==expected,"native startup handshake and every failed-event/CRT/child cleanup sequence");
        check(result==(scenario==0) && thread->m_hThread==(scenario==0 ? child:nullptr),
            "creation reports only initialized children and relinquishes failed thread handles");
        check(thread->m_nThreadID==(scenario>=3 || scenario==0 ? 0x4567:0),"startup preserves CRT thread IDs after creation failure");
        thread->m_hThread=nullptr;api.core->destroy(thread);
    }
    Storage storage;storage.reset();auto* thread=api.core->construct(storage.object());thread->m_hThread=child;
    BYTE before[sizeof(CWinThread)];std::memcpy(before,thread,sizeof(before));bool caught=false;
    try {api.create(thread,0,0,nullptr);}catch(CException* error) {caught=true;error->Delete();}
    check(caught && !std::memcmp(before,thread,sizeof(before)),"repeated creation raises a framework error without modifying its existing thread");
    thread->m_hThread=nullptr;api.core->destroy(thread);
}
struct Work {
    const API* api;CWinThread* expected;AFX_MODULE_STATE* module;S_Cthreadlocalobject slot{};
    LONG owned=0,deleted=0,init=0,run=0,exit=0,hookCount=0,unhookCount=0;UINT result=0;bool initialize=true,fallback=false,parametersCleared=false;
    HWND parentWindow=nullptr;
};
static Work* work;
static HHOOK realInstalledHook=reinterpret_cast<HHOOK>(UINT_PTR(0x56780000));
static HHOOK WINAPI installRealHook(int code,HOOKPROC hook,HINSTANCE module,DWORD thread) {
    check(code==WH_MSGFILTER && hook==work->api->hook && !module && thread==GetCurrentThreadId(),"real child installs its own native message-filter hook");
    InterlockedIncrement(&work->hookCount);
    if(scenario==6) {static CException error(FALSE);throw &error;}
    return realInstalledHook;
}
static BOOL WINAPI removeRealHook(HHOOK hook) {
    check(hook==realInstalledHook,"termination removes the installed child message-filter hook");
    InterlockedIncrement(&work->unhookCount);return TRUE;
}
struct Owned : CNoTrackObject {Work* owner;explicit Owned(Work* value):owner(value){} ~Owned() override {InterlockedIncrement(&owner->owned);} };
static void* AFXAPI createOwned() {return new Owned(work);}
static void enter(Work& current,CWinThread* thread) {
    check(thread==current.expected && current.api->core->state()->m_pCurrentWinThread==thread &&
          thread->m_pModuleState==current.module && current.api->core->module()==current.module,
          "child inherits the parent's module and registers its framework thread before user callbacks");
    check(current.parametersCleared ? thread->m_pThreadParams==nullptr : thread->m_pThreadParams==&current,
        "worker/UI callbacks receive the native thread-parameter semantics");
    if(current.fallback)check(thread->m_pMainWnd && thread->m_pMainWnd->m_hWnd==current.parentWindow,
        "worker receives an attached fallback wrapper for the module application's main window");
    if(current.hookCount)check(current.api->core->thread()->m_hHookOldMsgFilter==realInstalledHook,"user code sees the hook installed during startup");
    current.api->get(&current.slot,reinterpret_cast<void*>(&createOwned));
}
static UINT AFXAPI worker(void* opaque) {auto& current=*static_cast<Work*>(opaque);enter(current,current.api->core->current());current.result=37;return 37;}
static BOOL AFXAPI initialize(CWinThread* thread) {enter(*work,thread);InterlockedIncrement(&work->init);return work->initialize ? 69:FALSE;}
static int AFXAPI run(CWinThread*) {InterlockedIncrement(&work->run);work->result=41;return 41;}
static int AFXAPI exit(CWinThread*) {InterlockedIncrement(&work->exit);work->result=43;return 43;}
static void AFXAPI dispose(CWinThread* thread) {
    check(work->api->core->current()==thread,"virtual Delete executes while the child is still registered");
    if(work->fallback)check(thread->m_pMainWnd && !thread->m_pMainWnd->m_hWnd,"fallback window detaches before virtual thread deletion");
    InterlockedIncrement(&work->deleted);
}
static void realChildren(const API& api) {
    for(unsigned mode=0;mode<6;++mode) {
        alignas(16) BYTE module[0x100]{};module[0x28]=mode>=4 ? 0:1;
        auto* parent=api.core->thread();auto* previous=parent->m_pModuleState;
        Storage storage;storage.reset();auto* thread=api.core->worker(storage.object(),mode==0 || mode>=4 ? worker:nullptr,nullptr);
        void* table[33]{};table[22]=reinterpret_cast<void*>(&initialize);table[23]=reinterpret_cast<void*>(&run);
        table[28]=reinterpret_cast<void*>(&exit);table[32]=reinterpret_cast<void*>(&dispose);
        *reinterpret_cast<void***>(thread)=table;thread->m_bAutoDelete=FALSE;
        Work current{&api,thread,reinterpret_cast<AFX_MODULE_STATE*>(module)};work=&current;
        current.initialize=mode!=2;thread->m_pThreadParams=&current;
        alignas(16) BYTE app[sizeof(CWinApp)]{},window[sizeof(CWnd)]{};
        if(mode==3) {
            current.fallback=true;current.parentWindow=CreateWindowExW(0,L"STATIC",L"parent",WS_OVERLAPPED,0,0,20,20,nullptr,nullptr,nullptr,nullptr);
            field<HWND>(window,0x40)=current.parentWindow;field<void*>(app,0x40)=window;
            field<void*>(module,8)=app;
        }
        parent->m_pModuleState=current.module;
        scenario=mode==5 ? 6:0;
        Patch installed(api.image,"SetWindowsHookExW",reinterpret_cast<void*>(&installRealHook));
        Patch removed(api.image,"UnhookWindowsHookEx",reinterpret_cast<void*>(&removeRealHook));
        bool created=api.create(thread,mode==5 ? 0:CREATE_SUSPENDED,0,nullptr)!=FALSE;
        check(created==(mode!=5) && !current.owned && !current.init && !current.result,
            "requested suspension holds user code and failed initialization returns after child termination");
        if(mode==5)check(!thread->m_hThread && current.hookCount==1 && !current.unhookCount && !current.deleted,
            "framework initialization failure signals the creator, closes the thread and preserves caller ownership");
        if(created) {
            ResumeThread(thread->m_hThread);
            DWORD completed=WaitForSingleObject(thread->m_hThread,30000),result=0;
            check(completed==WAIT_OBJECT_0,"real CRT worker completes its startup and termination protocol");
            GetExitCodeThread(thread->m_hThread,&result);
            check(result==(mode==0 || mode>=4 ? 37:mode==2 ? 43:41) && current.result==result,
                "worker/UI return values reach the CRT thread exit code without an extra ExitInstance call");
            check(current.owned==1 && current.deleted==1 && current.init==(mode>0 && mode<4) &&
                  current.run==(mode>0 && mode<4 && mode!=2) && current.exit==(mode==2),
                  "termination calls virtual Delete once and disposes real thread-local owned objects");
            check(current.hookCount==(mode==4) && current.unhookCount==(mode==4),"real termination destroys the shared thread-state object and removes its hook");
        }
        api.core->destroy(thread);parent=api.core->thread();parent->m_pModuleState=previous;
        api.destroyLocal(&current.slot);api.destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));
        if(current.parentWindow)DestroyWindow(current.parentWindow);
    }
}
struct TempObject {void* root;HANDLE handle;int id;};
static int constructing,oleCalls;
static bool captureMaps;
static void AFXAPI constructTemp(CObject* object) {auto* value=reinterpret_cast<TempObject*>(object);std::memset(value,0,sizeof(*value));value->id=constructing;}
static void AFXAPI destroyTemp(CObject* object) {auto* value=reinterpret_cast<TempObject*>(object);if(captureMaps) {check(!value->handle,"map disposal clears native handles before destruction callbacks");record(value->id);}}
static void AFXAPI ole(BOOL exiting,BOOL libraries) {check(!exiting && !libraries,"temporary cleanup uses the native OLE callback flags");++oleCalls;}
static void temporaryMaps(const API& api) {
    active=&api;auto* module=api.core->module();auto* state=api.core->state();auto* context=api.core->thread();
    auto* oldApp=module->m_pCurrentWinApp;auto* oldThread=state->m_pCurrentWinThread;auto* oldPool=context->m_pSafetyPoolBuffer;
    CHandleMap** fields[]={&state->m_pmapHGDIOBJ,&state->m_pmapHDC,&state->m_pmapHMENU,&state->m_pmapHWND,&state->m_pmapHIMAGELIST};
    CHandleMap* oldMaps[5];for(unsigned i=0;i<5;++i)oldMaps[i]=*fields[i];auto oldCount=state->m_nTempMapLock;
    alignas(16) BYTE app[sizeof(CWinApp)]{},thread[sizeof(CWinThread)]{};
    module->m_pCurrentWinApp=reinterpret_cast<CWinApp*>(app);state->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(thread);
    field<void*>(thread,0x78)=reinterpret_cast<void*>(&ole);
    CRuntimeClass type{"temporary startup object",sizeof(TempObject),0xffff,nullptr,nullptr,nullptr,nullptr};
    for(int count : {0,1,2,-1})for(int cleanup : {0,1,-1,69}) {
        alignas(CHandleMap) BYTE storage[5][sizeof(CHandleMap)];
        for(unsigned i=0;i<5;++i) {
            auto* map=::new(storage[i]) CHandleMap(&type,constructTemp,destroyTemp,8,1);
            *fields[i]=map;constructing=i+1;map->FromHandle(reinterpret_cast<void*>(UINT_PTR(0x10000+256*i)));
        }
        state->m_nTempMapLock=count;context->m_pSafetyPoolBuffer=nullptr;field<size_t>(app,0x138)=64;
        trace.clear();oleCalls=0;captureMaps=true;BOOL result=api.unlock(cleanup);captureMaps=false;
        bool flush=count==1 && cleanup!=0;
        check(trace==(flush ? std::vector<int>{1,2,3,4,5}:std::vector<int>{}),"map cleanup follows the native lock transition, delete flag and class order");
        check(oleCalls==(flush && cleanup!=-1) && result==(count!=0 && count!=1) && state->m_nTempMapLock==(count ? count-1:0),
            "unlock preserves nested/negative locks and reports the final native lock state");
        check((context->m_pSafetyPoolBuffer!=nullptr)==(count==1),"safety-pool replenishment follows the zero-lock transition even without deleting maps");
        std::free(context->m_pSafetyPoolBuffer);context->m_pSafetyPoolBuffer=nullptr;
        for(unsigned i=0;i<5;++i)(*fields[i])->~CHandleMap();
    }
    for(int count : {0,-1,INT_MAX}) {
        state->m_nTempMapLock=count;api.lock();
        check(state->m_nTempMapLock==static_cast<int>(static_cast<unsigned>(count)+1),"locking uses the actual module count and native integer wraparound");
    }
    state->m_nTempMapLock=oldCount;module->m_pCurrentWinApp=oldApp;state->m_pCurrentWinThread=oldThread;
    context->m_pSafetyPoolBuffer=oldPool;for(unsigned i=0;i<5;++i)*fields[i]=oldMaps[i];
}
static CWinApp* poolApp;
static int poolSizes,poolAllocations,poolFrees,poolScenario;
static size_t (AFXAPI* realPoolSize)(void*);
static void* (AFXAPI* realPoolMalloc)(size_t);
static void (AFXAPI* realPoolFree)(void*);
static size_t AFXAPI poolSize(void* memory) {++poolSizes;auto size=realPoolSize(memory);if(poolScenario==2 && poolSizes==1)field<size_t>(poolApp,0x138)=16;return size;}
static void* AFXAPI poolMalloc(size_t bytes) {
    ++poolAllocations;if(poolScenario==1 && poolAllocations==1)return nullptr;return realPoolMalloc(bytes);
}
static void AFXAPI poolFree(void* memory) {++poolFrees;realPoolFree(memory);}
static void safetyPool(const API& api) {
    auto* module=api.core->module();auto* state=api.core->state();auto* context=api.core->thread();
    auto* oldApp=module->m_pCurrentWinApp;auto oldCount=state->m_nTempMapLock;auto* oldPool=context->m_pSafetyPoolBuffer;
    alignas(16) BYTE app[sizeof(CWinApp)]{};poolApp=reinterpret_cast<CWinApp*>(app);module->m_pCurrentWinApp=poolApp;
    for(poolScenario=0;poolScenario<4;++poolScenario) {
        auto* memory=std::malloc(32);auto originalSize=::_msize(memory);context->m_pSafetyPoolBuffer=memory;
        field<size_t>(app,0x138)=poolScenario==3 ? 16:originalSize+128;state->m_nTempMapLock=1;
        poolSizes=poolAllocations=poolFrees=0;
        {
            Patch s(api.image,"_msize",reinterpret_cast<void*>(&poolSize));realPoolSize=reinterpret_cast<decltype(realPoolSize)>(s.old);
            Patch a(api.image,"malloc",reinterpret_cast<void*>(&poolMalloc));realPoolMalloc=reinterpret_cast<decltype(realPoolMalloc)>(a.old);
            Patch f(api.image,"free",reinterpret_cast<void*>(&poolFree));realPoolFree=reinterpret_cast<decltype(realPoolFree)>(f.old);
            check(!api.unlock(0),"safety-pool refill leaves the lock count at zero");
        }
        check(context->m_pSafetyPoolBuffer && poolSizes==(poolScenario==3 ? 1:2) &&
              poolFrees==(poolScenario!=3) && poolAllocations==(poolScenario==3 ? 0:poolScenario==1 ? 2:1),
              "pool resizing rechecks sizes, retains sufficient buffers and falls back to the old allocation size");
        if(poolScenario==1)check(::_msize(context->m_pSafetyPoolBuffer)>=originalSize,"failed growth retains the previous safety capacity");
        if(poolScenario==2)check(::_msize(context->m_pSafetyPoolBuffer)>=16 && field<size_t>(app,0x138)==16,
            "size callbacks may change the freshly loaded allocation request after the original growth decision");
        std::free(context->m_pSafetyPoolBuffer);context->m_pSafetyPoolBuffer=nullptr;
    }
    module->m_pCurrentWinApp=oldApp;state->m_nTempMapLock=oldCount;context->m_pSafetyPoolBuffer=oldPool;
}
static AFX_MODULE_THREAD_STATE* endingState;
static CWinThread* endingThread;
static AFX_MODULE_STATE* endingModule;
static UINT endingCode;
static void AFXAPI endingOle(BOOL exiting,BOOL libraries) {
    check(exiting && !libraries && endingState->m_pCurrentWinThread==endingThread,"thread exit invokes its captured OLE service before unregistering");
    record(1);endingState->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(UINT_PTR(0x1000));
    active->core->thread()->m_pModuleState=endingModule;
}
static void AFXAPI endingDelete(CWinThread* thread) {check(thread==endingThread,"termination deletes the originally captured thread after callbacks replace its registration");record(2);}
static BOOL AFXAPI tooltipDestroy(CWnd*) {check(!endingState->m_pCurrentWinThread,"thread exit clears its captured state before terminating the new ambient module");record(3);return FALSE;}
static void* AFXAPI tooltipDelete(void* object,unsigned flags) {check(flags==1,"thread termination owns its tooltip through scalar deletion");record(4);return object;}
static void AFXAPI endingCRT(unsigned code) {endingCode=code;record(5);throw 117;}
static _AFX_THREAD_STATE* existing(const API& api) {
    if(api.image==GetModuleHandleW(nullptr))return openmfc::detail::threadstate::Existing();
    auto* base=reinterpret_cast<BYTE*>(api.image);
    using Peek=void* (AFXAPI*)(S_Cthreadlocalobject*);
    return static_cast<_AFX_THREAD_STATE*>(reinterpret_cast<Peek>(base+0x14d040)(reinterpret_cast<S_Cthreadlocalobject*>(base+0x3c4110)));
}
static void termination(const API& api) {
    active=&api;
    for(BOOL deletion : {FALSE,TRUE}) {
        auto* before=api.core->thread()->m_pModuleState;
        endingState=api.core->state();
        alignas(16) BYTE thread[sizeof(CWinThread)]{},module[0x100]{},tooltip[sizeof(CWnd)]{};
        void* threadTable[33]{},*tooltipTable[27]{};
        endingThread=reinterpret_cast<CWinThread*>(thread);endingModule=reinterpret_cast<AFX_MODULE_STATE*>(module);
        threadTable[32]=reinterpret_cast<void*>(&endingDelete);field<void*>(thread,0)=threadTable;
        field<void*>(thread,0x78)=reinterpret_cast<void*>(&endingOle);endingState->m_pCurrentWinThread=endingThread;
        api.core->thread()->m_pModuleState=endingModule;auto* next=api.core->state();
        tooltipTable[26]=reinterpret_cast<void*>(&tooltipDestroy);tooltipTable[1]=reinterpret_cast<void*>(&tooltipDelete);
        field<void*>(tooltip,0)=tooltipTable;next->m_pToolTip=reinterpret_cast<CToolTipCtrl*>(tooltip);
        api.core->thread()->m_pModuleState=before;trace.clear();bool caught=false;endingCode=0;
        {
            Patch end(api.image,"_endthreadex",reinterpret_cast<void*>(&endingCRT));
            try {api.end(0xfedcba98u,deletion);}catch(int code) {caught=code==117;}
        }
        check(caught && endingCode==0xfedcba98u && trace==(deletion ? std::vector<int>{1,2,3,4,5}:std::vector<int>{1,3,4,5}),
            "termination follows native OLE/delete/tooltip/CRT order and preserves the full exit code");
        check(!existing(api),"termination releases the actual global thread-state slot without allocating a replacement");
        api.core->thread()->m_pModuleState=before;
        api.destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));
    }
}
static int hookCalls;
static HHOOK installedHook=reinterpret_cast<HHOOK>(UINT_PTR(0x45670000));
static HHOOK WINAPI install(int code,HOOKPROC hook,HINSTANCE module,DWORD thread) {
    ++hookCalls;check(code==WH_MSGFILTER && hook==active->hook && !module && thread==GetCurrentThreadId(),
        "initialization installs the native message-filter hook on the calling OS thread");return installedHook;
}
static LRESULT WINAPI nextHook(HHOOK hook,int code,WPARAM wparam,LPARAM lparam) {
    check(hook==installedHook && code==-1 && wparam==WPARAM(0xfedcba9876543210ull) && lparam==LPARAM(0x123456789abcdef0ull),
        "filter forwarding preserves the previous hook, code and full-width parameters");return -0x123456789ll;
}
static BOOL AFXAPI filter(CWinThread*,int code,MSG* message) {
    check(code==7 && message==reinterpret_cast<MSG*>(UINT_PTR(0x12340000)),"message hooks use the thread's native virtual filter slot");return -69;
}
static void hooks(const API& api) {
    active=&api;auto* module=api.core->module();auto* context=api.core->thread();auto* state=api.core->state();
    auto oldDLL=module->m_bDLL;auto* oldThread=state->m_pCurrentWinThread;auto oldHook=context->m_hHookOldMsgFilter;
    auto count=state->m_nTempMapLock;state->m_nTempMapLock=3;hookCalls=0;
    {
        Patch setter(api.image,"SetWindowsHookExW",reinterpret_cast<void*>(&install));
        module->m_bDLL=TRUE;api.init();check(!hookCalls,"DLL module initialization does not install an executable message filter");
        module->m_bDLL=FALSE;api.init();check(hookCalls==1 && context->m_hHookOldMsgFilter==installedHook && state->m_nTempMapLock==3,
            "thread initialization stores its hook without resetting module bookkeeping");
    }
    {
        Patch next(api.image,"CallNextHookEx",reinterpret_cast<void*>(&nextHook));
        check(api.hook(-1,WPARAM(0xfedcba9876543210ull),LPARAM(0x123456789abcdef0ull))==-0x123456789ll,
            "negative hook codes forward the native LRESULT without truncation");
    }
    void* table[33]{};table[30]=reinterpret_cast<void*>(&filter);alignas(16) BYTE thread[sizeof(CWinThread)]{};
    field<void*>(thread,0)=table;state->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(thread);
    check(api.hook(7,0,reinterpret_cast<LPARAM>(reinterpret_cast<MSG*>(UINT_PTR(0x12340000))))==-69,
        "hook filters preserve the signed virtual BOOL result");
    state->m_pCurrentWinThread=oldThread;context->m_hHookOldMsgFilter=oldHook;module->m_bDLL=oldDLL;state->m_nTempMapLock=count;
}

static void AFXAPI workerDeleteSpy(CWinThread* thread) {record(13);originalDelete(thread);}
static BOOL WINAPI priority(HANDLE handle,int value) {check(handle==child && value==THREAD_PRIORITY_BELOW_NORMAL,"public creation forwards priority even while the child is held suspended");record(12);return FALSE;}
static int factoryMode,factoryCalls;
static bool realFactory;
static CWinThread* factoryObject;
static void AFXAPI factoryDelete(CWinThread*) {record(13);}
static CObject* AFXAPI factory() {
    ++factoryCalls;
    if(factoryMode==1)return nullptr;
    if(factoryMode==2) {static CException retained(-1);throw &retained;}
    if(factoryMode==3)throw 31;
    alignas(16) static BYTE storage[sizeof(CWinThread)];
    factoryObject=active->core->construct(reinterpret_cast<CWinThread*>(storage));
    factoryObject->m_bAutoDelete=FALSE;factoryObject->m_pThreadParams=reinterpret_cast<void*>(UINT_PTR(0x12340000));
    factoryObject->m_pfnThreadProc=realFactory ? nullptr:reinterpret_cast<AFX_THREADPROC>(UINT_PTR(0x23450000));
    std::memcpy(copiedThreadTable,*reinterpret_cast<void***>(factoryObject),sizeof(copiedThreadTable));
    copiedThreadTable[32]=reinterpret_cast<void*>(realFactory ? dispose:factoryDelete);
    if(realFactory) {
        copiedThreadTable[22]=reinterpret_cast<void*>(&initialize);copiedThreadTable[23]=reinterpret_cast<void*>(&run);
        copiedThreadTable[28]=reinterpret_cast<void*>(&exit);work->expected=factoryObject;
    }
    *reinterpret_cast<void***>(factoryObject)=copiedThreadTable;
    return factoryObject;
}
static void publicProtocol(const API& api) {
    active=&api;expectedStack=0x34000;SECURITY_ATTRIBUTES security{sizeof(security),nullptr,FALSE};expectedSecurity=&security;
    for(bool ui : {false,true})for(unsigned failure : {0u,3u,4u,5u})for(DWORD flags : {DWORD(0),DWORD(CREATE_SUSPENDED)}) {
        scenario=failure==5 ? 0:failure;expectedFlags=flags|CREATE_SUSPENDED;
        eventCalls=resumeCalls=0;failFinalResume=failure==5;spyWorkerDelete=!ui && scenario!=0;
        factoryMode=factoryCalls=0;factoryObject=nullptr;trace.clear();
        CRuntimeClass type{"startup factory",sizeof(CWinThread),0xffff,factory,nullptr,nullptr,nullptr};
        CWinThread* result=nullptr;bool caught=false;
        {
            Patch event(api.image,"CreateEventW",reinterpret_cast<void*>(&createEvent));
            Patch crt(api.image,"_beginthreadex",reinterpret_cast<void*>(&begin));
            Patch r(api.image,"ResumeThread",reinterpret_cast<void*>(&resume));
            Patch s(api.image,"SuspendThread",reinterpret_cast<void*>(&suspend));
            Patch w(api.image,"WaitForSingleObject",reinterpret_cast<void*>(&wait));
            Patch c(api.image,"CloseHandle",reinterpret_cast<void*>(&close));
            Patch e(api.image,"SetEvent",reinterpret_cast<void*>(&signal));
            Patch p(api.image,"SetThreadPriority",reinterpret_cast<void*>(&priority));
            try {
                if(ui)result=api.beginUI(&type,THREAD_PRIORITY_BELOW_NORMAL,expectedStack,flags,&security);
                else result=api.beginWorker(worker,reinterpret_cast<void*>(UINT_PTR(0x12345678)),THREAD_PRIORITY_BELOW_NORMAL,expectedStack,flags,&security);
            }catch(CException* error) {caught=true;error->Delete();}
        }
        bool failureResume=ui && failure==5 && !(flags&CREATE_SUSPENDED);
        check(caught==failureResume && (result!=nullptr)==(scenario==0 && !failureResume),
            "public worker/UI creation matches native creation failures and UI-only resume error reporting");
        std::vector<int> expected{1,2,3};
        if(scenario==3)for(int event : {6,10,13})expected.push_back(event);
        else {
            for(int event : {4,5,6,7})expected.push_back(event);
            if(scenario==4)for(int event : {8,9,10,13})expected.push_back(event);
            else {expected.push_back(11);expected.push_back(12);if(!(flags&CREATE_SUSPENDED))expected.push_back(4);}
        }
        check(trace==expected,"public creation initializes suspended children, applies priority, then resumes or deletes on failure");
        if(ui) {
            check(factoryCalls==1 && factoryObject && !factoryObject->m_pThreadParams &&
                  factoryObject->m_pfnThreadProc==reinterpret_cast<AFX_THREADPROC>(UINT_PTR(0x23450000)),
                  "UI factory creation clears thread arguments while preserving the factory's procedure member");
            factoryObject->m_hThread=nullptr;api.core->destroy(factoryObject);
        }else if(result) {result->m_hThread=nullptr;api.core->dispose(result);}
    }
    spyWorkerDelete=failFinalResume=false;
    for(factoryMode=1;factoryMode<6;++factoryMode) {
        factoryCalls=0;eventCalls=0;factoryObject=nullptr;
        CRuntimeClass type{"failed startup factory",sizeof(CWinThread),0xffff,factory,nullptr,nullptr,nullptr};
        if(factoryMode==5)type.m_pfnCreateObject=nullptr;
        bool caught=false;
        try {api.beginUI(factoryMode==4 ? nullptr:&type,0,0,0,nullptr);}
        catch(CException* error) {caught=factoryMode!=3;error->Delete();}
        catch(int value) {caught=factoryMode==3 && value==31;}
        check(caught && factoryCalls==(factoryMode<4),"UI creation consumes framework factory failures, propagates other exceptions and validates missing factories");
    }
    factoryMode=0;
}

static void publicChildren(const API& api) {
    active=&api;scenario=0;factoryMode=0;
    for(bool ui : {false,true}) {
        alignas(16) BYTE module[0x100]{};module[0x28]=1;
        auto* parent=api.core->thread();auto* previous=parent->m_pModuleState;
        Work current{&api,nullptr,reinterpret_cast<AFX_MODULE_STATE*>(module)};work=&current;current.parametersCleared=ui;
        parent->m_pModuleState=current.module;
        realFactory=ui;CRuntimeClass type{"real UI startup",sizeof(CWinThread),0xffff,factory,nullptr,nullptr,nullptr};
        auto* thread=ui ? api.beginUI(&type,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,nullptr):
            api.beginWorker(worker,&current,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,nullptr);
        check(thread!=nullptr,"public creation returns a real initialized CRT thread");
        if(thread) {
            current.expected=thread;thread->m_bAutoDelete=FALSE;
            if(!ui) {
                std::memcpy(copiedThreadTable,*reinterpret_cast<void***>(thread),sizeof(copiedThreadTable));
                copiedThreadTable[32]=reinterpret_cast<void*>(&dispose);*reinterpret_cast<void***>(thread)=copiedThreadTable;
            }
            ResumeThread(thread->m_hThread);DWORD result=0;
            check(WaitForSingleObject(thread->m_hThread,30000)==WAIT_OBJECT_0,"publicly created worker/UI thread reaches complete termination");
            GetExitCodeThread(thread->m_hThread,&result);
            check(result==(ui ? 41:37) && current.owned==1 && current.deleted==1 && !current.exit,
                "public creation connects the initialized child to native callback dispatch and actual TLS disposal");
            api.core->destroy(thread);if(!ui)std::free(thread);
        }
        api.core->thread()->m_pModuleState=previous;
        api.destroyLocal(&current.slot);api.destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));
    }
    realFactory=false;
}

static LONG localDestructions;
struct LocalOwned : CNoTrackObject {~LocalOwned() override {InterlockedIncrement(&localDestructions);} };
static void* AFXAPI newLocalOwned() {return new LocalOwned;}
static int tipDestroys,tipDeletes,tipFailure;
static BOOL AFXAPI termTipDestroy(CWnd*) {
    ++tipDestroys;
    int failure=tipFailure;tipFailure=0;
    if(failure==1) {static CException retained(-1);throw &retained;}
    if(failure==2)throw 31;
    return FALSE;
}
static void* AFXAPI termTipDelete(void* object,unsigned flags) {check(flags==1,"tooltip cleanup uses the owning scalar-destructor flag");++tipDeletes;return object;}
static void selectiveTermination(const API& api) {
    auto* state=api.core->state();auto* context=api.core->thread();auto* oldTip=state->m_pToolTip;auto oldCount=state->m_nTempMapLock;
    auto rootOwner=reinterpret_cast<HINSTANCE>(UINT_PTR(0x20000000));
    auto ownerA=reinterpret_cast<HINSTANCE>(UINT_PTR(0x20010000)),ownerB=reinterpret_cast<HINSTANCE>(UINT_PTR(0x20020000));
    api.assign(rootOwner);
    S_Cthreadlocalobject a{},b{};localDestructions=0;
    api.get(&a,reinterpret_cast<void*>(&newLocalOwned));api.assign(ownerA);
    auto* retained=api.get(&b,reinterpret_cast<void*>(&newLocalOwned));api.assign(ownerB);
    alignas(16) BYTE tooltip[sizeof(CWnd)]{};void* table[27]{};
    table[26]=reinterpret_cast<void*>(&termTipDestroy);table[1]=reinterpret_cast<void*>(&termTipDelete);field<void*>(tooltip,0)=table;
    state->m_pToolTip=reinterpret_cast<CToolTipCtrl*>(tooltip);state->m_nTempMapLock=3;tipDestroys=tipDeletes=0;
    api.term(ownerA);
    check(localDestructions==1 && !api.peek(&a) && api.peek(&b)==retained && existing(api)==context,
        "module-specific termination deletes matching TLS values while retaining foreign and global state");
    check(!tipDestroys && !tipDeletes && state->m_pToolTip==reinterpret_cast<CToolTipCtrl*>(tooltip) && state->m_nTempMapLock==3,
        "non-null module termination preserves tooltips and nested temporary-map locks");
    state->m_pToolTip=oldTip;state->m_nTempMapLock=oldCount;api.destroyLocal(&a);api.destroyLocal(&b);
    for(int failure : {1,2}) {
        state=api.core->state();auto* module=api.core->module();
        oldTip=state->m_pToolTip;oldCount=state->m_nTempMapLock;
        S_Cthreadlocalobject value{};api.get(&value,reinterpret_cast<void*>(&newLocalOwned));localDestructions=0;
        field<AFX_MODULE_STATE*>(tooltip,0x38)=module;state->m_pToolTip=reinterpret_cast<CToolTipCtrl*>(tooltip);
        state->m_nTempMapLock=0;tipDestroys=tipDeletes=0;tipFailure=failure;bool caught=false;
        try {api.term(nullptr);}catch(int code) {caught=code==31;}
        if(failure==1) {
            check(!caught && localDestructions==1 && tipDestroys==2 && tipDeletes==1,
                "framework tooltip errors are consumed and remaining TLS destruction retries native owned cleanup");
        }else {
            check(caught && !localDestructions && tipDestroys==1 && !tipDeletes && api.peek(&value),
                "unrelated C++ exceptions propagate and leave unperformed TLS cleanup to the caller");
            state->m_pToolTip=oldTip;state->m_nTempMapLock=oldCount;
        }
        api.destroyLocal(&value);
    }
}

static void execute(const API& api) {createProtocol(api);realChildren(api);publicProtocol(api);publicChildren(api);temporaryMaps(api);safetyPool(api);hooks(api);selectiveTermination(api);termination(api);}
}
static void winthreadStartupTests(const API& core,HMODULE image,bool native) {
    startup_test::API api{&core,impl__CreateThread_CWinThread__QEAAHKIPEAU_SECURITY_ATTRIBUTES___Z,
        impl__AfxLockTempMaps__YAXXZ,impl__AfxUnlockTempMaps__YAHH_Z,impl__AfxInitThread__YAXXZ,
        impl__AfxTermThread__YAXPEAUHINSTANCE_____Z,impl__AfxEndThread__YAXIH_Z,nullptr,nullptr,nullptr,
        impl___1CThreadLocalObject__QEAA_XZ,&openmfc::detail::winthread::MessageFilterHook,image};
    api.beginWorker=impl__AfxBeginThread__YAPEAVCWinThread__P6AIPEAX_Z0HIKPEAU_SECURITY_ATTRIBUTES___Z;
    api.beginUI=impl__AfxBeginThread__YAPEAVCWinThread__PEAUCRuntimeClass__HIKPEAU_SECURITY_ATTRIBUTES___Z;
    api.peek=impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ;
    api.assign=impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z;
    api.get=&impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z;
    if(native) {
        auto* b=reinterpret_cast<BYTE*>(image);
#define STARTUP_RVA(member,rva) api.member=reinterpret_cast<decltype(api.member)>(b+rva)
        STARTUP_RVA(create,0x274200);STARTUP_RVA(lock,0x2a5e70);STARTUP_RVA(unlock,0x2a5e90);
        STARTUP_RVA(init,0x273f60);STARTUP_RVA(term,0x273fa0);STARTUP_RVA(end,0x273ef0);
        STARTUP_RVA(beginWorker,0x273db0);STARTUP_RVA(beginUI,0x273e60);
        STARTUP_RVA(peek,0x14d040);STARTUP_RVA(assign,0x14d190);
        STARTUP_RVA(get,0x14cf40);STARTUP_RVA(destroyLocal,0x14d0d0);STARTUP_RVA(hook,0x274720);
#undef STARTUP_RVA
    }
    startup_test::execute(api);
}
