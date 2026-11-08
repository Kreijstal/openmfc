// Thread lifetime and loop checks use real production owners. Unrelated
// command/OLE virtuals are aborting boundaries; native comparison is optional.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/RegcoreSupport.h"
#include "detail/AppDocumentManagerState.h"
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
#ifndef OPENMFC_TEST_REAL_THREAD_STATE
alignas(16) static thread_local BYTE threadBytes[sizeof(_AFX_THREAD_STATE)]{};
alignas(16) static thread_local BYTE moduleBytes[sizeof(AFX_MODULE_THREAD_STATE)]{};
static _AFX_THREAD_STATE* threadState() {return reinterpret_cast<_AFX_THREAD_STATE*>(threadBytes);}
static AFX_MODULE_THREAD_STATE* moduleThreadState() {return reinterpret_cast<AFX_MODULE_THREAD_STATE*>(moduleBytes);}
#endif
#ifndef OPENMFC_TEST_REAL_THREAD_STATE
alignas(16) static BYTE moduleStorage[0x108]{};
static auto* const moduleToken = reinterpret_cast<AFX_MODULE_STATE*>(moduleStorage);
static bool throwModule;
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() {
    if(throwModule)throw 37;return moduleToken;
}
_AFX_THREAD_STATE* AFXAPI AfxGetThreadState() {return threadState();}
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    return moduleThreadState();
}

#else
static auto* const moduleToken=&openmfc::detail::regcore::g_moduleStateStatic;
static AFX_MODULE_THREAD_STATE* moduleThreadState() {
    return impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
}
#endif
// These siblings are outside this fixture; never return invented behavior.
IMPLEMENT_DYNAMIC(CWinThread,CCmdTarget)
IMPLEMENT_DYNAMIC(CWinApp,CWinThread)
#ifndef OPENMFC_TEST_REAL_THREAD_STATE
namespace openmfc { namespace detail { namespace modulethreadstate {
AFX_MODULE_THREAD_STATE* ForModule(AFX_MODULE_STATE* module) {
    check(module==moduleToken,"application registers in its captured module");return moduleThreadState();
}
} } }
#else
LRESULT CALLBACK AfxWndProc(HWND,UINT,WPARAM,LPARAM) {std::abort();}
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() {throw 91;}
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long value) {throw value;}
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() {throw 19;}
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();
extern "C" void MS_ABI impl__AfxGlobalFree__YAXPEAX_Z(void*);

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
    decltype(&impl___0CWinApp__QEAA_PEB_W_Z) appConstruct;
    decltype(&impl___1CWinApp__UEAA_XZ) appDestroy;
    decltype(&impl__AfxGetThread__YAPEAVCWinThread__XZ) current;
    decltype(&impl__AfxGlobalFree__YAXPEAX_Z) globalFree;
    void** staticManager;
    void** staticTemplates;
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
static const API* appAPI;
static CWinApp* appObject;
static DWORD versionMajor,versionMinor;
static BOOL WINAPI appVersion(OSVERSIONINFOW* version) {
    check(version->dwOSVersionInfoSize==sizeof(OSVERSIONINFOW),"application queries the native version structure");
    check(appAPI->current()==appObject && appAPI->module()->m_pCurrentWinApp==appObject,
        "both application registrations precede version callbacks");
    check(field<BOOL>(appObject,0x154)==TRUE && field<DWORD>(appObject,0x15c)==0xa5a5a5a5,
        "restart defaults precede the version callback and COM state follows it");
    version->dwMajorVersion=versionMajor;version->dwMinorVersion=versionMinor;return TRUE;
}
static AFX_MODULE_STATE* replacementConstructionModule;
static DWORD (WINAPI* originalThreadId)();
static DWORD WINAPI changeConstructionModule() {
    check(appAPI->current()==appObject && appObject->m_hThread==GetCurrentThread(),
        "current-thread registration and pseudo handle precede the thread-ID callback");
    appObject->m_pModuleState=replacementConstructionModule;
    return originalThreadId();
}
static wchar_t* AFXAPI failNameDuplication(const wchar_t*) {return nullptr;}
static void applications(const API& api,HMODULE image,bool source) {
    appAPI=&api;
    auto* module=api.module();auto* state=api.state();
    auto* oldApp=module->m_pCurrentWinApp;auto* oldName=module->m_lpszCurrentAppName;
    auto* oldThread=state->m_pCurrentWinThread;
    auto* oldManager=*api.staticManager;auto* oldTemplates=*api.staticTemplates;
    *api.staticManager=nullptr;*api.staticTemplates=nullptr;
    Patch version(image,"GetVersionExW",reinterpret_cast<void*>(&appVersion));
    for(unsigned pass=0;pass<5;++pass) {
        alignas(16) BYTE storage[sizeof(CWinApp)],expected[sizeof(CWinApp)];
        std::memset(storage,0xa5,sizeof(storage));std::memset(expected,0xa5,sizeof(expected));
        appObject=reinterpret_cast<CWinApp*>(storage);
        wchar_t name[]=L"native application \x03a9";
        versionMajor=pass<3 ? 6 : (pass==3 ? 5:10);versionMinor=pass<3 ? pass:9;
        api.thread()->m_msgCur.wParam=WPARAM(0xfedcba9876543210ull);
        check(api.appConstruct(appObject,pass==0 ? nullptr:name)==appObject,"application constructor returns its storage");
        if(pass) {
            check(appObject->m_pszAppName!=name && !std::wcscmp(appObject->m_pszAppName,name),
                "application owns a duplicate of the caller's name");
            name[0]=L'X';check(appObject->m_pszAppName[0]==L'n',"caller name changes do not affect the application");
        }else check(!appObject->m_pszAppName,"null application names remain null");
        check(api.thread()->m_msgCur.wParam==WPARAM(0xfedcba9876543210ull),"startup preserves non-message-number thread state");
        for(size_t offset : {0x40,0x48,0x68,0x70,0x78,0x80,0x88,0x90,0xa8,0xb0,0xb8,0xc8,0xd0,0xd8,
                            0xe8,0xf0,0x100,0x110,0x118,0x120,0x128,0x130,0x140,0x168,0x170})
            field<UINT_PTR>(expected,offset)=0;
        field<BOOL>(expected,0x50)=TRUE;
        field<HANDLE>(expected,0x58)=GetCurrentThread();field<DWORD>(expected,0x60)=GetCurrentThreadId();
        field<const wchar_t*>(expected,0xa0)=appObject->m_pszAppName;
        for(size_t offset : {0xc0,0xe0,0x108,0x14c,0x15c})field<DWORD>(expected,offset)=0;
        field<UINT_PTR>(expected,0x138)=0x200;field<DWORD>(expected,0x150)=300000;
        field<BOOL>(expected,0x154)=TRUE;field<BOOL>(expected,0x160)=TRUE;
        field<BOOL>(expected,0x158)=versionMajor>6 || (versionMajor==6 && versionMinor>=1);
        check(!std::memcmp(storage+0x40,expected+0x40,sizeof(storage)-0x40),
            "application defaults and untouched bytes match the native object layout");
        check(appObject->m_pModuleState==module && api.current()==appObject && module->m_pCurrentWinApp==appObject,
            "application construction installs the captured module and current thread");
        auto* other=reinterpret_cast<CWinApp*>(UINT_PTR(0x12340000));
        state->m_pCurrentWinThread=other;
        check(api.current()==other,"current-thread lookup does not fall back to the registered application");
        if(source) {
            check(AfxGetApp()==appObject,"public application lookup remains independent of the current worker thread");
            auto instance=module->m_hCurrentInstanceHandle,resource=module->m_hCurrentResourceHandle;
            module->m_hCurrentInstanceHandle=reinterpret_cast<HINSTANCE>(UINT_PTR(0x34560000));
            module->m_hCurrentResourceHandle=nullptr;
            check(AfxGetInstanceHandle()==module->m_hCurrentInstanceHandle &&
                  !AfxGetResourceHandle(),
                "module handles do not depend on application fields or resource fallback");
            AfxSetResourceHandle(reinterpret_cast<HINSTANCE>(UINT_PTR(0x789a0000)));
            check(module->m_hCurrentResourceHandle==reinterpret_cast<HINSTANCE>(UINT_PTR(0x789a0000)) && !appObject->m_hInstance,
                "setting the resource module does not overwrite the application instance");
            module->m_hCurrentInstanceHandle=instance;module->m_hCurrentResourceHandle=resource;
        }
        if(pass&1) {module->m_pCurrentWinApp=other;module->m_lpszCurrentAppName=L"retained module name";}
        else {state->m_pCurrentWinThread=appObject;module->m_lpszCurrentAppName=appObject->m_pszAppName;}
        watched=appObject;closes=0;
        {
            Patch close(image,"CloseHandle",reinterpret_cast<void*>(&closeHandle));
            field<void*>(appObject,0)=reinterpret_cast<void*>(UINT_PTR(0xdeadbeef));
            api.appDestroy(appObject);
        }
        check(closes==0,"application teardown relinquishes its pseudo handle before base cleanup");
        check(state->m_pCurrentWinThread==((pass&1) ? other:nullptr),"application teardown preserves replacement thread registration");
        check(module->m_pCurrentWinApp==((pass&1) ? other:nullptr),"application teardown preserves replacement module registration");
        check((pass&1) ? module->m_lpszCurrentAppName!=nullptr : module->m_lpszCurrentAppName==nullptr,
            "application teardown clears only its matching module name");
    }
    {
        alignas(16) BYTE storage[sizeof(CWinApp)],replacementModule[0x108]{};
        std::memset(storage,0xa5,sizeof(storage));appObject=reinterpret_cast<CWinApp*>(storage);
        replacementConstructionModule=reinterpret_cast<AFX_MODULE_STATE*>(replacementModule);
        {
            Patch id(image,"GetCurrentThreadId",reinterpret_cast<void*>(&changeConstructionModule));
            originalThreadId=reinterpret_cast<decltype(originalThreadId)>(id.old);
            api.appConstruct(appObject,L"captured module");
        }
        check(module->m_pCurrentWinApp==appObject && !replacementConstructionModule->m_pCurrentWinApp &&
              appObject->m_pModuleState==replacementConstructionModule,
            "constructor retains its captured module across callbacks that change the object's module pointer");
        appObject->m_pModuleState=module;api.appDestroy(appObject);
        std::memset(storage,0xa5,sizeof(storage));
        {
            Patch duplicate(image,"_wcsdup",reinterpret_cast<void*>(&failNameDuplication));
            api.appConstruct(appObject,L"allocation fails");
        }
        check(!appObject->m_pszAppName && api.current()==appObject && module->m_pCurrentWinApp==appObject,
            "native name-allocation failure preserves successful application registration");
        api.appDestroy(appObject);
    }
    module->m_pCurrentWinApp=oldApp;module->m_lpszCurrentAppName=oldName;state->m_pCurrentWinThread=oldThread;
    *api.staticManager=oldManager;*api.staticTemplates=oldTemplates;
}

static std::vector<int> resourceEvents;
struct Resource {void** table;int id;};
static void resourceEvent(int id) {resourceEvents.push_back(id);}
static void* AFXAPI appResourceDelete(void* self,unsigned flags) {
    auto id=static_cast<Resource*>(self)->id;
    check(flags==1,"application invokes owning virtual deletion with flag one");
    check(field<BOOL>(appObject,0x154)==FALSE && !field<BOOL>(appObject,0x15c) &&
          !field<void*>(appObject,0x168) && !field<void*>(appObject,0x170),
          "COM and restart services are released before application objects");
    check(appAPI->current()==appObject && appAPI->module()->m_pCurrentWinApp==appObject,
        "application objects are disposed before application unregistration");
    if(id==8)check(!*appAPI->staticManager,"static document manager is cleared before template-list deletion");
    resourceEvent(id);return self;
}
static ULONG AFXAPI appRelease(IUnknown* self) {
    auto* resource=reinterpret_cast<Resource*>(self);resourceEvent(resource->id);
    check(!field<BOOL>(appObject,0x154),"restart support is disabled before COM interface release");
    check(field<void*>(appObject,resource->id==1 ? 0x168:0x170)==self,
        "COM interface remains installed during its Release callback");
    return 17;
}
static void WINAPI appCoUninitialize() {
    check(field<BOOL>(appObject,0x15c)!=FALSE && !field<void*>(appObject,0x168) && !field<void*>(appObject,0x170),
        "COM interfaces are cleared before uninitialization and the flag is cleared afterward");
    resourceEvent(3);
}
static UINT WINAPI appGlobalFlags(HGLOBAL memory) {
    if(memory==reinterpret_cast<HGLOBAL>(UINT_PTR(0x11110000))) {resourceEvent(9);return 2;}
    check(memory==reinterpret_cast<HGLOBAL>(UINT_PTR(0x22220000)),"second application global allocation");
    resourceEvent(12);return 0;
}
static BOOL WINAPI appGlobalUnlock(HGLOBAL memory) {
    check(memory==reinterpret_cast<HGLOBAL>(UINT_PTR(0x11110000)),"only the locked global allocation is unlocked");
    resourceEvent(10);return FALSE;
}
static HGLOBAL WINAPI appGlobalFree(HGLOBAL memory) {
    resourceEvent(memory==reinterpret_cast<HGLOBAL>(UINT_PTR(0x11110000)) ? 11:13);return memory;
}
static ATOM WINAPI appDeleteAtom(ATOM atom) {
    check(atom==0xc123 || atom==0xc456,"application frees both stored shell atoms");
    resourceEvent(atom==0xc123 ? 14:15);return atom;
}
static void* ownedStrings[5];
static wchar_t* replacementRegistry;
static void (AFXAPI* realAppFree)(void*);
static void AFXAPI appFree(void* memory) {
    for(unsigned i=0;i<5;++i)if(memory==ownedStrings[i]) {
        check(!appAPI->module()->m_pCurrentWinApp && !appAPI->module()->m_lpszCurrentAppName,
            "application module pointers are cleared before freeing owned strings");
        check(appAPI->current()==appObject && appObject->m_hThread==GetCurrentThread(),
            "base thread cleanup follows application string disposal");
        resourceEvent(17+i);
        if(i==0 && replacementRegistry) {
            auto* previous=const_cast<wchar_t*>(appObject->m_pszRegistryKey);
            appObject->m_pszRegistryKey=replacementRegistry;
            ownedStrings[1]=replacementRegistry;replacementRegistry=nullptr;
            realAppFree(previous);
        }
        break;
    }
    realAppFree(memory);
}
static void applicationResources(const API& api,HMODULE image) {
    appAPI=&api;
    auto* module=api.module();auto* state=api.state();
    auto* oldApp=module->m_pCurrentWinApp;auto* oldName=module->m_lpszCurrentAppName;
    auto* oldThread=state->m_pCurrentWinThread;auto oldDLL=module->m_bDLL;
    auto* oldManager=*api.staticManager;auto* oldTemplates=*api.staticTemplates;
    void* deletingTable[8]{},*releaseTable[3]{};
    deletingTable[1]=deletingTable[7]=reinterpret_cast<void*>(&appResourceDelete);
    releaseTable[2]=reinterpret_cast<void*>(&appRelease);
    resourceEvents.reserve(64);
    for(bool dll : {false,true}) {
        alignas(16) BYTE storage[sizeof(CWinApp)];std::memset(storage,0xa5,sizeof(storage));
        appObject=api.appConstruct(reinterpret_cast<CWinApp*>(storage),L"resource owner");
        Resource objects[]={{releaseTable,1},{releaseTable,2},{deletingTable,4},{deletingTable,5},
            {deletingTable,6},{deletingTable,7},{deletingTable,8},{deletingTable,16}};
        field<void*>(appObject,0x168)=&objects[0];field<void*>(appObject,0x170)=&objects[1];
        field<BOOL>(appObject,0x15c)=TRUE;appObject->m_pDocManager=&objects[2];
        field<void*>(appObject,0x118)=&objects[3];field<void*>(appObject,0x120)=&objects[4];
        *api.staticManager=&objects[5];*api.staticTemplates=&objects[6];
        field<void*>(appObject,0x128)=&objects[7];
        field<HGLOBAL>(appObject,0xe8)=reinterpret_cast<HGLOBAL>(UINT_PTR(0x11110000));
        field<HGLOBAL>(appObject,0xf0)=reinterpret_cast<HGLOBAL>(UINT_PTR(0x22220000));
        field<ATOM>(appObject,0x130)=0xc123;field<ATOM>(appObject,0x132)=0xc456;
        replacementRegistry=::_wcsdup(L"registry replaced by an allocator callback");
        appObject->m_pszRegistryKey=::_wcsdup(L"registry");appObject->m_pszExeName=::_wcsdup(L"executable");
        appObject->m_pszHelpFilePath=::_wcsdup(L"help");appObject->m_pszProfileName=::_wcsdup(L"profile");
        unsigned index=0;
        for(auto text : {appObject->m_pszAppName,appObject->m_pszRegistryKey,appObject->m_pszExeName,
                        appObject->m_pszHelpFilePath,appObject->m_pszProfileName})
            ownedStrings[index++]=const_cast<wchar_t*>(text);
        appObject->m_pszAppID=L"borrowed application ID";
        module->m_lpszCurrentAppName=appObject->m_pszAppName;module->m_bDLL=dll;
        resourceEvents.clear();watched=appObject;closes=0;
        {
            Patch releaseCOM(image,"CoUninitialize",reinterpret_cast<void*>(&appCoUninitialize));
            Patch flags(image,"GlobalFlags",reinterpret_cast<void*>(&appGlobalFlags));
            Patch unlock(image,"GlobalUnlock",reinterpret_cast<void*>(&appGlobalUnlock));
            Patch globalFree(image,"GlobalFree",reinterpret_cast<void*>(&appGlobalFree));
            Patch atom(image,"GlobalDeleteAtom",reinterpret_cast<void*>(&appDeleteAtom));
            Patch free(image,"free",reinterpret_cast<void*>(&appFree));
            realAppFree=reinterpret_cast<decltype(realAppFree)>(free.old);
            Patch close(image,"CloseHandle",reinterpret_cast<void*>(&closeHandle));
            api.appDestroy(appObject);
        }
        std::vector<int> expected{1,2,3,4,5,6};
        if(!dll) {expected.push_back(7);expected.push_back(8);}
        for(int event : {9,10,10,11,12,13,14,15,16,17,18,19,20,21})expected.push_back(event);
        check(resourceEvents==expected,"native application cleanup order, global lock counts and virtual destructor slots");
        check(closes==0 && !state->m_pCurrentWinThread && !module->m_pCurrentWinApp,
            "all application cleanup precedes non-owning thread-handle teardown");
        check(dll ? (*api.staticManager==&objects[5] && *api.staticTemplates==&objects[6]) :
                    (!*api.staticManager && !*api.staticTemplates),
            "DLL application teardown preserves the process document-manager objects");
        check(!std::wcscmp(appObject->m_pszAppID,L"borrowed application ID"),
            "application ID storage is not among the owned destructor strings");
    }
    module->m_pCurrentWinApp=oldApp;module->m_lpszCurrentAppName=oldName;module->m_bDLL=oldDLL;
    state->m_pCurrentWinThread=oldThread;*api.staticManager=oldManager;*api.staticTemplates=oldTemplates;
    HGLOBAL memory=GlobalAlloc(GMEM_MOVEABLE,64);
    check(memory!=nullptr,"allocate global-memory helper fixture");
    if(memory) {
        GlobalLock(memory);GlobalLock(memory);api.globalFree(memory);
        check(!GlobalSize(memory),"global-memory helper releases and frees a real multiply locked block");
    }
    api.globalFree(nullptr);
}

#ifdef OPENMFC_TEST_REAL_THREAD_STATE
struct RegistrationWork {AFX_MODULE_STATE* module;CWinApp* app;};
static DWORD WINAPI registrationWorker(void* opaque) {
    auto* work=static_cast<RegistrationWork*>(opaque);
    auto* previous=impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(work->module);
    check(!AfxGetThread() && AfxGetApp()==work->app,
        "a new OS thread shares the module application and starts without a current framework thread");
    {
        CWinThread worker;
        auto* state=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
        state->m_pCurrentWinThread=&worker;
        check(AfxGetThread()==&worker && AfxGetApp()==work->app,
            "production TLS keeps worker registration independent of module application registration");
    }
    check(!AfxGetThread() && AfxGetApp()==work->app,"worker destruction clears only that OS thread's registration");
    impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(previous);
    return 0;
}
static void applicationModuleIntegration() {
    AFX_MODULE_STATE modules[2];
    auto* previous=impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(&modules[0]);
    alignas(16) BYTE storage[2][sizeof(CWinApp)];
    auto* a=impl___0CWinApp__QEAA_PEB_W_Z(reinterpret_cast<CWinApp*>(storage[0]),L"module A");
    auto* stateA=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(&modules[1]);
    auto* b=impl___0CWinApp__QEAA_PEB_W_Z(reinterpret_cast<CWinApp*>(storage[1]),L"module B");
    auto* stateB=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    check(stateA!=stateB && AfxGetApp()==b && AfxGetThread()==b,
        "two applications occupy distinct real cached module/thread states");
    impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(&modules[0]);
    check(AfxGetApp()==a && AfxGetThread()==a,"module switching selects the registered application and thread");
    RegistrationWork work{&modules[0],a};
    HANDLE worker=CreateThread(nullptr,0,registrationWorker,&work,0,nullptr);
    check(worker!=nullptr,"create thread for production TLS registration integration");
    if(worker) {
        check(WaitForSingleObject(worker,30000)==WAIT_OBJECT_0,"worker registration integration completes");
        CloseHandle(worker);
    }
    check(AfxGetApp()==a && AfxGetThread()==a && stateA->m_pCurrentWinThread==a,
        "worker registration and destruction preserve the application's original OS thread state");
    modules[0].m_lpszCurrentAppName=a->m_pszAppName;
    impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(&modules[1]);
    impl___1CWinApp__UEAA_XZ(a);
    check(!modules[0].m_pCurrentWinApp && !modules[0].m_lpszCurrentAppName && AfxGetApp()==b && AfxGetThread()==b,
        "application cleanup uses its captured module and preserves another ambient application");
    check(stateA->m_pCurrentWinThread==a,"native base teardown checks the ambient module's thread registration");
    stateA->m_pCurrentWinThread=nullptr;
    impl___1CWinApp__UEAA_XZ(b);
    check(!AfxGetApp() && !AfxGetThread(),"application destruction clears real module and thread registrations");
    impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(previous);
}
#endif

static CWnd* AFXAPI mainWindowOverride(CWinThread*) {return reinterpret_cast<CWnd*>(UINT_PTR(0x13570000));}
static void applicationHelpers() {
    auto* state=moduleThreadState();auto* oldThread=state->m_pCurrentWinThread;
    state->m_pCurrentWinThread=nullptr;
    check(!AfxGetMainWnd(),"main-window helpers return null without a current framework thread");
    void* table[32]{};table[31]=reinterpret_cast<void*>(&mainWindowOverride);
    alignas(16) BYTE worker[sizeof(CWinThread)]{};field<void*>(worker,0)=table;
    state->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(worker);
    check(AfxGetMainWnd()==reinterpret_cast<CWnd*>(UINT_PTR(0x13570000)),
        "main-window lookup honors the current thread's virtual GetMainWnd override");
    state->m_pCurrentWinThread=oldThread;
}
static void applicationBaseOwnership() {
    // Both public C++ and exported construction share real application ownership.
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
        check(closes==0 && !moduleThreadState()->m_pCurrentWinThread && !moduleToken->m_pCurrentWinApp,
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
        AfxGetThreadState,impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ,
        impl___0CWinApp__QEAA_PEB_W_Z,impl___1CWinApp__UEAA_XZ,impl__AfxGetThread__YAPEAVCWinThread__XZ,
        impl__AfxGlobalFree__YAXPEAX_Z,&openmfc::detail::docmanager::staticDocumentManager,
        &openmfc::detail::docmanager::staticTemplateList};
    if(argc>1 && !std::strcmp(argv[1],"--source-close"))terminationChild(source,GetModuleHandleW(nullptr),true);
    if(argc>1 && !std::strcmp(argv[1],"--source-dispatch"))terminationChild(source,GetModuleHandleW(nullptr),false);
    construction(source);destruction(source);applicationBaseOwnership();
    applications(source,GetModuleHandleW(nullptr),true);
    applicationResources(source,GetModuleHandleW(nullptr));
    applicationHelpers();
    loops(source,GetModuleHandleW(nullptr));idleMessages(source);pumping(source,GetModuleHandleW(nullptr));
#ifndef OPENMFC_TEST_REAL_THREAD_STATE
    Storage failed,before;failed.reset();before.reset();throwModule=true;bool caught=false;
    try {source.construct(failed.object());}catch(int e) {caught=e==37;}
    throwModule=false;check(caught && !std::memcmp(failed.bytes,before.bytes,sizeof(failed.bytes)),"base module lookup failure preserves unconstructed storage");
#endif
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
                NATIVE(appConstruct,0x1cb720);NATIVE(appDestroy,0x1cc530);NATIVE(current,0x273aa0);NATIVE(globalFree,0x2ae660);
                native.staticManager=reinterpret_cast<void**>(b+0x3be3f0);
                native.staticTemplates=reinterpret_cast<void**>(b+0x3be3f8);
#undef NATIVE
                if(argc>1 && !std::strcmp(argv[1],"--native-close"))terminationChild(native,image,true);
                if(argc>1 && !std::strcmp(argv[1],"--native-dispatch"))terminationChild(native,image,false);
                {Patch p(image,"GetCursorPos",reinterpret_cast<void*>(&cursor));construction(native);}
                {Patch p(image,"CloseHandle",reinterpret_cast<void*>(&closeHandle));destruction(native);}
                applications(native,image,false);
                applicationResources(native,image);
                loops(native,image);idleMessages(native);pumping(native,image);
                if(argc==1) {terminationParent(L"--native-close");terminationParent(L"--native-dispatch");}
            }
            FreeLibrary(image);
        }
    }
#ifdef OPENMFC_TEST_REAL_THREAD_STATE
    applicationModuleIntegration();
    impl___1CThreadLocalObject__QEAA_XZ(&moduleToken->m_moduleThreadState);
#endif
    std::printf("Thread lifetime and message loop: %d failures\n",failures);
    return failures ? 1:0;
}
