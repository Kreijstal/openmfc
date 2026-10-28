// Thread lifetime and loop checks use real production owners. Unrelated
// command/OLE virtuals are aborting boundaries; native comparison is optional.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <exception>

extern "C" {
CWinThread* MS_ABI impl___0CWinThread__QEAA_XZ(CWinThread*);
CWinThread* MS_ABI impl___0CWinThread__QEAA_P6AIPEAX_Z0_Z(CWinThread*,UINT (AFXAPI*)(void*),void*);
void MS_ABI impl___1CWinThread__UEAA_XZ(CWinThread*);
void MS_ABI impl__CommonConstruct_CWinThread__QEAAXXZ(CWinThread*);
void MS_ABI impl__Delete_CWinThread__UEAAXXZ(CWinThread*);
BOOL MS_ABI impl__InitInstance_CWinThread__UEAAHXZ(CWinThread*);
int MS_ABI impl__ExitInstance_CWinThread__UEAAHXZ(CWinThread*);
int MS_ABI impl__Run_CWinThread__UEAAHXZ(CWinThread*);
BOOL MS_ABI impl__AfxInternalPumpMessage__YAHXZ();
BOOL MS_ABI impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z(MSG*);
BOOL MS_ABI impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z(MSG*);
MSG* MS_ABI impl__AfxGetCurrentMessage__YAPEAUtagMSG__XZ();
CWinApp* MS_ABI impl___0CWinApp__QEAA_PEB_W_Z(CWinApp*,const wchar_t*);
void MS_ABI impl___1CWinApp__UEAA_XZ(CWinApp*);
}
static int failures;
static void check(bool condition,const char* label) {
    if(!condition) {++failures;std::printf("FAIL: %s\n",label);}
}
template<class T> static T& field(void* p,size_t offset) {
    return *reinterpret_cast<T*>(static_cast<BYTE*>(p)+offset);
}
struct alignas(16) Storage {
    BYTE bytes[168];
    CWinThread* object() {return reinterpret_cast<CWinThread*>(bytes+16);}
    void reset() {std::memset(bytes,0xa5,sizeof(bytes));}
};
alignas(16) static thread_local BYTE threadBytes[sizeof(_AFX_THREAD_STATE)]{};
alignas(16) static thread_local BYTE moduleBytes[sizeof(AFX_MODULE_THREAD_STATE)]{};
static _AFX_THREAD_STATE* threadState() {return reinterpret_cast<_AFX_THREAD_STATE*>(threadBytes);}
static AFX_MODULE_THREAD_STATE* moduleThreadState() {return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleBytes);}
static auto* const moduleToken = reinterpret_cast<AFX_MODULE_STATE*>(UINT_PTR(0x12345000));
static int moduleGets,threadGets;
static bool throwModule;
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    ++moduleGets;if(throwModule)throw 37;return moduleToken;
}
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() {++threadGets;return threadState();}
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    return moduleThreadState();
}

// These siblings are outside this fixture; never return invented behavior.
IMPLEMENT_DYNAMIC(CWinThread,CCmdTarget)
IMPLEMENT_DYNAMIC(CWinApp,CWinThread)
CWinApp* g_pApp=nullptr;
CRuntimeClass CCmdTarget::classCCmdTarget{};
int CCmdTarget::OnCmdMsg(UINT,int,void*,AFX_CMDHANDLERINFO*) {std::abort();}
void CCmdTarget::OnFinalRelease() {std::abort();}
BOOL CCmdTarget::IsInvokeAllowed(DISPID) {std::abort();}
BOOL CCmdTarget::GetDispatchIID(IID*) {std::abort();}
UINT CCmdTarget::GetTypeInfoCount() {std::abort();}
CTypeLibCache* CCmdTarget::GetTypeLibCache() {std::abort();}
HRESULT CCmdTarget::GetTypeLib(LCID,ITypeLib**) {std::abort();}
const AFX_MSGMAP* CCmdTarget::GetMessageMap() const {std::abort();}
const AFX_OLECMDMAP* CCmdTarget::GetCommandMap() const {std::abort();}
const AFX_DISPMAP* CCmdTarget::GetDispatchMap() const {std::abort();}
const AFX_CONNECTIONMAP* CCmdTarget::GetConnectionMap() const {std::abort();}
const AFX_INTERFACEMAP* CCmdTarget::GetInterfaceMap() const {std::abort();}
const AFX_EVENTSINKMAP* CCmdTarget::GetEventSinkMap() const {std::abort();}
BOOL CCmdTarget::OnCreateAggregates() {std::abort();}
IUnknown* CCmdTarget::GetInterfaceHook(const void*) {std::abort();}
BOOL CCmdTarget::GetExtraConnectionPoints(CPtrArray*) {std::abort();}
IConnectionPoint* CCmdTarget::GetConnectionHook(const IID&) {std::abort();}
BOOL CWinThread::PreTranslateMessage(MSG*) {std::abort();}
BOOL CWinThread::OnIdle(LONG) {std::abort();}
LRESULT CWinThread::ProcessWndProcException(CException*,const MSG*) {std::abort();}
BOOL CWinThread::ProcessMessageFilter(int,MSG*) {std::abort();}
CWnd* CWinThread::GetMainWnd() {std::abort();}
void CWinThread::Delete() {impl__Delete_CWinThread__UEAAXXZ(this);}
int CWinApp::InitApplication() {std::abort();}
BOOL CWinApp::InitInstance() {std::abort();}
int CWinApp::ExitInstance() {std::abort();}
int CWinApp::Run() {std::abort();}

struct API {
    decltype(&impl___0CWinThread__QEAA_XZ) construct;
    decltype(&impl___0CWinThread__QEAA_P6AIPEAX_Z0_Z) worker;
    decltype(&impl___1CWinThread__UEAA_XZ) destroy;
    decltype(&impl__CommonConstruct_CWinThread__QEAAXXZ) common;
    decltype(&impl__Delete_CWinThread__UEAAXXZ) dispose;
    decltype(&impl__InitInstance_CWinThread__UEAAHXZ) init;
    decltype(&impl__ExitInstance_CWinThread__UEAAHXZ) exit;
    decltype(&impl__Run_CWinThread__UEAAHXZ) run;
    decltype(&impl__AfxInternalPumpMessage__YAHXZ) pump;
    decltype(&impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z) idle;
    decltype(&impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z) globalIdle;
    decltype(&impl__AfxGetCurrentMessage__YAPEAUtagMSG__XZ) message;
    _AFX_THREAD_STATE* (AFXAPI* thread)();
    AFX_MODULE_THREAD_STATE* (AFXAPI* state)();
    AFX_MODULE_STATE* (AFXAPI* module)();
};
static const API* active;
static void* watched;
static void* expectedRoot;
static int closes,dispatches,deletions;
static bool capture,mutateClose,throwDispatch,throwClose;
static BOOL WINAPI cursor(POINT* point) {
    if(capture) {
        check(active->thread()->m_msgCur.message==0 && active->thread()->m_nMsgLast==0,
            "cursor query follows message-number resets");
        check(field<BOOL>(watched,0x50)==BOOL(0xa5a5a5a5),"auto-delete initialization follows cursor query");
    }
    point->x=17;point->y=-29;return FALSE;
}
static BOOL WINAPI closeHandle(HANDLE handle) {
    ++closes;
    check(handle==field<HANDLE>(watched,0x58),"destructor closes its stored handle");
    check(field<void*>(watched,0)==expectedRoot,"destructor replaces the incoming root before CloseHandle");
    if(throwClose)throw 42;
    if(mutateClose)active->state()->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(UINT_PTR(0x11110000));
    return FALSE;
}
static void AFXAPI disconnect(void* embedded) {
    ++dispatches;
    check(embedded==static_cast<BYTE*>(watched)+0x20,"base destructor receives its embedded dispatch interface");
    check(active->state()->m_pCurrentWinThread!=watched,"thread unregisters before base dispatch cleanup");
    check(field<AFX_MODULE_STATE*>(watched,0x38)!=nullptr,"module pointer survives through base dispatch cleanup");
    if(throwDispatch)throw 41;
}
static void* AFXAPI deleteCallback(void* self,unsigned flags) {
    ++deletions;check(self==watched && flags==1,"Delete calls scalar deleting destructor with flag one");return self;
}
static UINT AFXAPI worker(void* p) {return UINT(reinterpret_cast<UINT_PTR>(p));}

static void** import(HMODULE image,const char* wanted) {
    auto* bytes=reinterpret_cast<BYTE*>(image);
    auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(bytes+reinterpret_cast<IMAGE_DOS_HEADER*>(bytes)->e_lfanew);
    auto* entries=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(bytes+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;entries->Name;++entries) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(bytes+entries->OriginalFirstThunk);
        auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(bytes+entries->FirstThunk);
        for(;names->u1.AddressOfData;++names,++values)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(bytes+names->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&values->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot=nullptr;void* old=nullptr;DWORD protection=0;
    Patch(HMODULE image,const char* name,void* callback) {
        slot=import(image,name);check(slot!=nullptr,name);if(!slot)std::abort();
        old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=callback;
    }
    ~Patch() {*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
static void construction(const API& api) {
    active=&api;
    Patch patch(GetModuleHandleW(nullptr),"GetCursorPos",reinterpret_cast<void*>(&cursor));
    for(int mode=0;mode!=3;++mode) {
        Storage actual,expected;actual.reset();expected.reset();watched=actual.object();
        auto* state=api.thread();BYTE saved[0x40];std::memcpy(saved,reinterpret_cast<BYTE*>(state)+0x58,sizeof(saved));
        std::memset(reinterpret_cast<BYTE*>(state)+0x58,0xa5,sizeof(saved));
        capture=true;
        if(mode==0)check(api.construct(actual.object())==actual.object(),"default constructor uses supplied storage");
        else if(mode==1)check(api.worker(actual.object(),worker,reinterpret_cast<void*>(UINT_PTR(0x3210)))==actual.object(),
            "worker constructor uses callback and third argument in supplied storage");
        else api.common(actual.object());
        capture=false;
        auto* e=expected.object();
        if(mode!=2) {
            field<void*>(e,0)=field<void*>(actual.object(),0);
            field<LONG>(e,8)=1;field<void*>(e,16)=nullptr;field<UINT_PTR>(e,24)=0;
            field<UINT_PTR>(e,32)=0;field<BOOL>(e,40)=TRUE;field<UINT_PTR>(e,48)=0;
            field<AFX_MODULE_STATE*>(e,56)=api.module();
            field<void*>(e,0x68)=mode==1 ? reinterpret_cast<void*>(UINT_PTR(0x3210)) : nullptr;
            field<void*>(e,0x70)=mode==1 ? reinterpret_cast<void*>(&worker) : nullptr;
        }
        field<void*>(e,0x40)=nullptr;field<void*>(e,0x48)=nullptr;field<BOOL>(e,0x50)=TRUE;
        field<HANDLE>(e,0x58)=nullptr;field<DWORD>(e,0x60)=0;
        field<void*>(e,0x78)=nullptr;field<void*>(e,0x80)=nullptr;
        check(!std::memcmp(actual.bytes,expected.bytes,sizeof(actual.bytes)),"native thread fields, padding and canaries");
        BYTE wanted[0x40];std::memset(wanted,0xa5,sizeof(wanted));
        field<UINT>(wanted,8)=0;field<LONG>(wanted,0x30)=17;field<LONG>(wanted,0x34)=-29;field<UINT>(wanted,0x38)=0;
        check(!std::memcmp(reinterpret_cast<BYTE*>(state)+0x58,wanted,sizeof(wanted)),"construction resets only native thread-state fields");
        std::memcpy(reinterpret_cast<BYTE*>(state)+0x58,saved,sizeof(saved));
        if(mode!=2)api.destroy(actual.object());
    }
}
static void destruction(const API& api) {
    active=&api;Storage value;value.reset();api.construct(value.object());expectedRoot=field<void*>(value.object(),0);
    Patch patch(GetModuleHandleW(nullptr),"CloseHandle",reinterpret_cast<void*>(&closeHandle));
    for(int mode=0;mode!=4;++mode) {
        value.reset();api.construct(value.object());watched=value.object();
        field<void*>(watched,0)=reinterpret_cast<void*>(UINT_PTR(0xdeadbeef));
        field<HANDLE>(watched,0x58)=mode==0 ? nullptr : mode==3 ? GetCurrentThread() : reinterpret_cast<HANDLE>(UINT_PTR(0x1234));
        void* dispatchTable[8]{};dispatchTable[7]=reinterpret_cast<void*>(&disconnect);
        field<void*>(watched,0x20)=dispatchTable;
        api.state()->m_pCurrentWinThread=value.object();mutateClose=mode==2;closes=dispatches=0;
        BYTE expected[sizeof(CWinThread)];std::memcpy(expected,watched,sizeof(expected));
        field<void*>(expected,0)=expectedRoot;field<void*>(expected,0x38)=nullptr;
        api.destroy(value.object());
        check(closes==(mode!=0) && dispatches==1,"destructor handle and dispatch callback counts");
        check(api.state()->m_pCurrentWinThread==(mutateClose ? reinterpret_cast<CWinThread*>(UINT_PTR(0x11110000)):nullptr),
            "destructor compares the current registration after CloseHandle");
        check(!std::memcmp(watched,expected,sizeof(expected)),"destructor preserves stored handle and all non-owned bytes");
    }
    void* table[2]{};table[1]=reinterpret_cast<void*>(&deleteCallback);
    for(BOOL autoDelete : {FALSE,TRUE,BOOL(69)}) {
        value.reset();watched=value.object();field<void*>(watched,0)=table;
        field<BOOL>(watched,0x50)=autoDelete;deletions=closes=0;api.dispose(value.object());
        check(deletions==(autoDelete!=FALSE) && closes==0,"Delete retains handles for non-auto-deleting objects");
    }
    api.state()->m_pCurrentWinThread=nullptr;
}
static void applicationBaseOwnership() {
    // The existing application constructor is still partial. This integration
    // check guards the handle-ownership change introduced by the real base dtor.
    for(int exported=0;exported!=2;++exported) {
        alignas(16) BYTE storage[sizeof(CWinApp)];std::memset(storage,0xa5,sizeof(storage));
        auto* application=exported ? impl___0CWinApp__QEAA_PEB_W_Z(reinterpret_cast<CWinApp*>(storage),L"thread ownership integration")
            : ::new(storage) CWinApp(L"thread ownership integration");
        check(application==reinterpret_cast<CWinApp*>(storage) && application->m_hThread==GetCurrentThread(),
            "application startup initializes base storage and stores a pseudo thread handle");
        moduleThreadState()->m_pCurrentWinThread=application;
        watched=application;closes=0;
        Patch patch(GetModuleHandleW(nullptr),"CloseHandle",reinterpret_cast<void*>(&closeHandle));
        if(exported) {
            field<void*>(application,0)=reinterpret_cast<void*>(UINT_PTR(0xdeadbeef));
            impl___1CWinApp__UEAA_XZ(application);
        }else application->CWinApp::~CWinApp();
        check(closes==0 && !moduleThreadState()->m_pCurrentWinThread && !g_pApp,
            "application teardown relinquishes its pseudo handle before owned base cleanup");
    }
}

static std::vector<int> events;
static int peeks,pumps,idles,scenario,throwAt;
static MSG* loopMessage;
static void event(int value) {events.push_back(value);if(value==throwAt)throw value;}
static BOOL WINAPI peek(MSG* message,HWND window,UINT first,UINT last,UINT remove) {
    check(message==loopMessage && !window && !first && !last && !remove,"Run peeks into the captured pumped message without removal");
    event(10);return scenario==1 && peeks++==0;
}
static BOOL AFXAPI idleCallback(CWinThread*,LONG count) {event(100+count);return idles++==0 ? 69:FALSE;}
static BOOL AFXAPI pumpCallback(CWinThread*) {event(20);return pumps++<2 ? 69:FALSE;}
static BOOL AFXAPI isIdleCallback(CWinThread*,MSG* message) {check(message==loopMessage,"idle classification receives the captured message");event(30);return pumps==2 ? 69:FALSE;}
static int AFXAPI exitCallback(CWinThread*) {event(40);return 77;}
static void loops(const API& api,HMODULE image) {
    Patch patch(image,"PeekMessageW",reinterpret_cast<void*>(&peek));
    for(scenario=0;scenario!=2;++scenario)for(int fault : {0,100,20,30,40}) {
        Storage value;value.reset();void* table[33]{};
        table[25]=reinterpret_cast<void*>(&pumpCallback);table[26]=reinterpret_cast<void*>(&idleCallback);
        table[27]=reinterpret_cast<void*>(&isIdleCallback);table[28]=reinterpret_cast<void*>(&exitCallback);
        field<void*>(value.object(),0)=table;loopMessage=&api.thread()->m_msgCur;
        peeks=pumps=idles=0;events.clear();throwAt=fault;int result=0,caught=0;
        try {result=api.run(value.object());}catch(int error) {caught=error;}
        throwAt=0;
        if(fault)check(caught==fault,"Run propagates callback failures");
        else {
            check(result==77,"Run returns virtual ExitInstance result");
            std::vector<int> expected=scenario==0 ? std::vector<int>{10,100,10,101,20,30,10,20,30,10,10,100,20,40}
                : std::vector<int>{10,20,30,10,10,100,10,101,20,30,10,10,100,20,40};
            check(events==expected,"native Run idle/count/pump ordering");
        }
    }
    Storage value;value.reset();loopMessage=&api.thread()->m_msgCur;loopMessage->wParam=UINT_PTR(0x12345678abcdef01ull);
    check(api.init(value.object())==FALSE && api.exit(value.object())==int(0xabcdef01u),"base InitInstance and ExitInstance use native values without virtual redispatch");
}
static void idleMessages(const API& api) {
    auto* state=api.thread();auto oldPoint=state->m_ptCursorLast;auto oldMessage=state->m_nMsgLast;
    state->m_ptCursorLast.x=5;state->m_ptCursorLast.y=6;state->m_nMsgLast=WM_MOUSEMOVE;
    MSG msg{};msg.pt.x=5;msg.pt.y=6;msg.message=WM_MOUSEMOVE;
    check(!api.idle(&msg),"repeated mouse moves do not restart idle processing");
    msg.pt.x=7;check(api.idle(&msg) && !api.idle(&msg),"a changed mouse position restarts idle exactly once");
    msg.message=WM_NCMOUSEMOVE;check(api.idle(&msg) && !api.idle(&msg),"client/nonclient transition restarts idle");
    for(UINT id : {UINT(WM_PAINT),UINT(0x118)}) {msg.message=id;check(!api.idle(&msg),"paint and system timer are not idle messages");}
    for(UINT id : {UINT(WM_MOUSEWHEEL),UINT(WM_TIMER),UINT(WM_KEYDOWN),UINT(WM_QUIT)}) {
        msg.message=id;check(api.idle(&msg),"wheel, timer, key and quit use native default idle classification");
    }
    api.state()->m_pCurrentWinThread=nullptr;msg.message=WM_KEYDOWN;
    check(api.globalIdle(&msg),"global idle classification works without a CWinThread object");
    check(api.message()==&state->m_msgCur,"AfxGetCurrentMessage exposes pumped storage without a thread object");
    state->m_ptCursorLast=oldPoint;state->m_nMsgLast=oldMessage;
}
static int getResult,preResult,preCalls,translations,deliveries;
static UINT nextMessage;
static BOOL WINAPI getMessage(MSG* message,HWND,UINT,UINT) {
    check(message==&active->thread()->m_msgCur,"PumpMessage passes shared thread storage directly to GetMessage");
    message->message=nextMessage;message->wParam=123;return getResult;
}
static BOOL WINAPI translate(const MSG* message) {check(message==&active->thread()->m_msgCur,"translation receives shared message");++translations;return FALSE;}
static LRESULT WINAPI dispatch(const MSG* message) {check(message==&active->thread()->m_msgCur,"dispatch receives shared message");++deliveries;return 19;}
static BOOL AFXAPI pretranslate(CWinThread*,MSG* message) {check(message==&active->thread()->m_msgCur,"pretranslation receives shared message");++preCalls;return preResult;}
extern "C" BOOL MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(MSG* message) {
    auto* thread=active->state()->m_pCurrentWinThread;
    return thread ? thread->PreTranslateMessage(message) : FALSE;
}
static void pumping(const API& api,HMODULE image) {
    active=&api;
    Patch get(image,"GetMessageW",reinterpret_cast<void*>(&getMessage));
    Patch trans(image,"TranslateMessage",reinterpret_cast<void*>(&translate));
    Patch send(image,"DispatchMessageW",reinterpret_cast<void*>(&dispatch));
    Storage value;value.reset();void* table[33]{};table[24]=reinterpret_cast<void*>(&pretranslate);
    field<void*>(value.object(),0)=table;api.state()->m_pCurrentWinThread=value.object();
    for(int result : {0,1,-1})for(UINT msg : {UINT(WM_KEYDOWN),UINT(0x36a)})for(int pre : {0,69}) {
        getResult=result;nextMessage=msg;preResult=pre;preCalls=translations=deliveries=0;
        check(api.pump()==(result!=0),"native GetMessage zero/nonzero semantics");
        bool shouldPre=result!=0 && msg!=0x36a,shouldDispatch=shouldPre && !pre;
        check(preCalls==int(shouldPre) && translations==int(shouldDispatch) && deliveries==int(shouldDispatch),
            "pump translation and private idle-message filtering");
    }
    api.state()->m_pCurrentWinThread=nullptr;
}
[[noreturn]] static void terminationChild(const API& api,HMODULE image,bool handleFailure) {
    active=&api;Storage object;object.reset();api.construct(object.object());
    watched=object.object();expectedRoot=field<void*>(watched,0);
    field<void*>(watched,0)=reinterpret_cast<void*>(UINT_PTR(0xdeadbeef));
    void* table[8]{};table[7]=reinterpret_cast<void*>(&disconnect);
    field<void*>(watched,0x20)=table;
    field<HANDLE>(watched,0x58)=handleFailure ? reinterpret_cast<HANDLE>(UINT_PTR(0x1234)) : nullptr;
    api.state()->m_pCurrentWinThread=object.object();closes=dispatches=0;
    throwClose=handleFailure;throwDispatch=!handleFailure;
    Patch patch(image,"CloseHandle",reinterpret_cast<void*>(&closeHandle));
    std::set_terminate([] {
        bool intact=!failures && field<void*>(watched,0)==expectedRoot && field<void*>(watched,0x38)!=nullptr;
        if(throwClose)intact=intact && closes==1 && dispatches==0 && active->state()->m_pCurrentWinThread==watched;
        else intact=intact && closes==0 && dispatches==1 && active->state()->m_pCurrentWinThread==nullptr;
        ExitProcess(intact ? 86:87);
    });
    try {api.destroy(object.object());}catch(...) {ExitProcess(88);}
    ExitProcess(89);
}
static void terminationParent(const wchar_t* argument) {
    wchar_t path[1024],command[1200];GetModuleFileNameW(nullptr,path,1024);
    _snwprintf_s(command,_countof(command),_TRUNCATE,L"\"%ls\" %ls",path,argument);
    STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION child{};
    bool created=CreateProcessW(nullptr,command,nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&child)!=FALSE;
    check(created,"launch destructor exception-contract child");if(!created)return;
    DWORD wait=WaitForSingleObject(child.hProcess,30000),code=0;
    if(wait!=WAIT_OBJECT_0)TerminateProcess(child.hProcess,90);
    GetExitCodeProcess(child.hProcess,&code);
    check(wait==WAIT_OBJECT_0 && code==86,"destructor callback failure terminates at the native cleanup boundary");
    CloseHandle(child.hThread);CloseHandle(child.hProcess);
}
int main(int argc,char** argv) {
    API source{impl___0CWinThread__QEAA_XZ,impl___0CWinThread__QEAA_P6AIPEAX_Z0_Z,impl___1CWinThread__UEAA_XZ,
        impl__CommonConstruct_CWinThread__QEAAXXZ,impl__Delete_CWinThread__UEAAXXZ,
        impl__InitInstance_CWinThread__UEAAHXZ,impl__ExitInstance_CWinThread__UEAAHXZ,impl__Run_CWinThread__UEAAHXZ,
        impl__AfxInternalPumpMessage__YAHXZ,impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z,
        impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z,impl__AfxGetCurrentMessage__YAPEAUtagMSG__XZ,
        AfxGetThreadState,impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ};
    if(argc>1 && !std::strcmp(argv[1],"--source-close"))terminationChild(source,GetModuleHandleW(nullptr),true);
    if(argc>1 && !std::strcmp(argv[1],"--source-dispatch"))terminationChild(source,GetModuleHandleW(nullptr),false);
    construction(source);destruction(source);applicationBaseOwnership();
    loops(source,GetModuleHandleW(nullptr));idleMessages(source);pumping(source,GetModuleHandleW(nullptr));
    Storage failed,before;failed.reset();before.reset();throwModule=true;bool caught=false;
    try {source.construct(failed.object());}catch(int e) {caught=e==37;}
    throwModule=false;check(caught && !std::memcmp(failed.bytes,before.bytes,sizeof(failed.bytes)),"base module lookup failure preserves unconstructed storage");
    if(argc==1) {terminationParent(L"--source-close");terminationParent(L"--source-dispatch");}
    if(const char* path=std::getenv("OPENMFC_NATIVE_WINTHREAD_DLL")) {
        HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native thread oracle");
        if(image) {
            auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"native thread oracle matches verified RVAs");
            if(pinned) {
                API native=source;
#define NATIVE(member,rva) native.member=reinterpret_cast<decltype(native.member)>(b+rva)
                NATIVE(construct,0x274100);NATIVE(worker,0x274050);NATIVE(destroy,0x2741b0);NATIVE(common,0x274140);
                NATIVE(dispose,0x274360);NATIVE(init,0x71e0);NATIVE(exit,0x274480);NATIVE(run,0x274390);
                NATIVE(pump,0x273ae0);NATIVE(idle,0x273cf0);NATIVE(globalIdle,0x273d70);NATIVE(message,0x273ac0);
                NATIVE(thread,0x1332a0);NATIVE(state,0x133a20);NATIVE(module,0x133930);
#undef NATIVE
                if(argc>1 && !std::strcmp(argv[1],"--native-close"))terminationChild(native,image,true);
                if(argc>1 && !std::strcmp(argv[1],"--native-dispatch"))terminationChild(native,image,false);
                {Patch p(image,"GetCursorPos",reinterpret_cast<void*>(&cursor));construction(native);}
                {Patch p(image,"CloseHandle",reinterpret_cast<void*>(&closeHandle));destruction(native);}
                loops(native,image);idleMessages(native);pumping(native,image);
                if(argc==1) {terminationParent(L"--native-close");terminationParent(L"--native-dispatch");}
            }
            FreeLibrary(image);
        }
    }
    std::printf("Thread lifetime and message loop: %d failures\n",failures);
    return failures ? 1:0;
}
