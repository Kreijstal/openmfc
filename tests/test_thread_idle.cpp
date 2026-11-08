// Idle processing uses actual TLS/module storage, temporary maps, sent-message
// dispatch and descendant traversal. Raw client objects exercise the MSVC ABI.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <climits>

extern "C" {
BOOL MS_ABI impl__OnIdle_CWinThread__UEAAHJ_Z(CWinThread*,LONG);
int MS_ABI impl__Run_CWinThread__UEAAHXZ(CWinThread*);
LRESULT MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(CWnd*,HWND,UINT,WPARAM,LPARAM);
void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
LRESULT MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(CWinThread*,CException*,const MSG*);
}
namespace {
int failures;
void check(bool value,const char* label) {
    if(!value) {++failures;std::printf("FAIL: %s\n",label);}
}
template<class T> struct alignas(16) Object {
    BYTE bytes[sizeof(T)];
    T* get() {return reinterpret_cast<T*>(bytes);}
    void reset(void** table) {std::memset(bytes,0,sizeof(bytes));*reinterpret_cast<void***>(bytes)=table;}
};
struct API {
    decltype(&impl__OnIdle_CWinThread__UEAAHJ_Z) idle;
    _AFX_THREAD_STATE* (MS_ABI* thread)();
    AFX_MODULE_THREAD_STATE* (MS_ABI* state)();
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) localDestroy;
    bool native;
    HMODULE image;
    decltype(&impl__Run_CWinThread__UEAAHXZ) run;
    decltype(&impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z) baseException;
};
const API* active;
Object<CWinThread> thread;
Object<CFrameWnd> objects[7];
void* threadTable[33]{};
void* windowTable[100]{};
void* siteTable[44]{};
void** site=siteTable;
HWND windows[7]{};
alignas(16) BYTE modules[3][0x110]{};
AFX_MODULE_THREAD_STATE* states[3]{};
std::vector<int> events;
int mutation,throwAt;
int destructed,deletedErrors;
bool throwFramework;
CException* thrownError;
struct Error : CException {
    Error() : CException(TRUE) {}
    ~Error() override {++deletedErrors;}
};
CFrameWnd* frame(unsigned i) {return objects[i].get();}
void select(unsigned i) {active->thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[i]);}
unsigned index(void* object) {
    for(unsigned i=0;i<7;++i)if(object==frame(i))return i;
    std::abort();
}
void event(int value) {
    events.push_back(value);
    if(throwAt==value)throw value;
}
BOOL AFXAPI overriddenIdle(CWinThread*,LONG) {std::abort();}
LRESULT AFXAPI procedure(void* self,UINT message,WPARAM w,LPARAM l) {
    unsigned i=index(self);
    check(message==0x363 && w==1 && l==0,"idle dispatch carries WM_IDLEUPDATECMDUI and disable-if-unhandled");
    const MSG& current=active->thread()->m_lastSentMsg;
    check(current.hwnd==frame(i)->m_hWnd && current.message==message && current.wParam==w && current.lParam==l,
          "idle callbacks run inside the actual sent-message scope");
    event(10+int(i));
    if(throwFramework && i==3) {thrownError=new Error;throw thrownError;}
    if(mutation==1 && i==0) {
        // Native captures the main object before its callback, and resolves
        // the object's module only after updating its descendants.
        thread.get()->m_pMainWnd=frame(3);
        thread.get()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[1]);
        select(2);
    }
    if(mutation==2 && i==3) {
        frame(3)->m_retailMiddle.m_pNextFrameWnd=frame(6);
        frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOWMINNOACTIVE;
    }
    if(mutation==3 && i==0)frame(0)->m_hWnd=windows[4];
    return LRESULT(0xfedcba9876543210ull);
}
LRESULT AFXAPI failingProcessor(CWinThread*,CException*,const MSG* message) {
    check(message==&active->thread()->m_lastSentMsg && message->message==0x363,
          "thread exception routing receives the interrupted idle message");
    event(98);throw 77;
}

BOOL AFXAPI oleShow(void* self,int command) {
    check(self==&site,"delayed show dispatches through the stored OLE control site");
    event(100+command);
    if(mutation==4)frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOWNOACTIVATE;
    return -37;
}
void AFXAPI cleanup(BOOL terminate,BOOL justFree) {
    check(!terminate && !justFree,"idle temporary-map cleanup uses the nontermination OLE callback");
    event(90);
    check(active->state()->m_nTempMapLock==0,"temporary-map cleanup follows the final unlock");
}
struct Temp {void* table;HANDLE handle;};
CRuntimeClass tempClass{"IdleTemporaryWrapper",sizeof(Temp),0xffff,nullptr,nullptr,nullptr,nullptr};
void AFXAPI constructTemp(CObject* object) {std::memset(object,0,sizeof(Temp));}
void AFXAPI destroyTemp(CObject*) {++destructed;}
CRuntimeClass windowClass{"IdleClientWindow",sizeof(CFrameWnd),0xffff,nullptr,nullptr,nullptr,nullptr};
void AFXAPI unexpected(CObject*) {std::abort();}

void expected(std::initializer_list<int> wanted,const char* label) {
    check(events==std::vector<int>(wanted),label);
    if(events!=std::vector<int>(wanted)) {for(int value:events)std::printf(" %d",value);std::puts("");}
}
void reset() {
    mutation=throwAt=destructed=deletedErrors=0;throwFramework=false;
    threadTable[29]=reinterpret_cast<void*>(active->baseException);
    events.clear();thread.reset(threadTable);
    thread.get()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);
    thread.get()->m_lpfnOleTermOrFreeLib=&cleanup;
    for(unsigned i=0;i<3;++i) {
        states[i]->m_pCurrentWinThread=thread.get();states[i]->m_nTempMapLock=0;states[i]->m_frameList.m_pHead=nullptr;
    }
    select(0);
    for(unsigned i=0;i<7;++i) {
        objects[i].reset(windowTable);frame(i)->m_hWnd=windows[i];frame(i)->m_retailMiddle.m_nShowDelay=-1;
        ShowWindow(windows[i],SW_HIDE);
    }
    // Static window messages are intentionally outside the framework callback.
    events.clear();
}

void firstPass() {
    for(LONG count : {LONG(0),LONG(-1),LONG(LONG_MIN)}) {
        reset();thread.get()->m_pMainWnd=frame(0);ShowWindow(windows[0],SW_SHOWNOACTIVATE);
        states[0]->m_frameList.m_pHead=frame(0);
        frame(0)->m_retailMiddle.m_nShowDelay=SW_SHOW;
        frame(0)->m_retailMiddle.m_pNextFrameWnd=frame(3);
        frame(3)->m_retailMiddle.m_pNextFrameWnd=frame(4);
        frame(4)->m_retailMiddle.m_pNextFrameWnd=frame(5);
        frame(5)->m_retailMiddle.m_pNextFrameWnd=frame(6);
        frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOWNOACTIVATE;
        frame(4)->m_retailMiddle.m_nShowDelay=0;ShowWindow(windows[4],SW_SHOWNOACTIVATE);
        frame(5)->m_hWnd=nullptr;frame(5)->m_retailMiddle.m_nShowDelay=SW_SHOW;
        MSG previous=active->thread()->m_lastSentMsg;
        check(active->idle(thread.get(),count)==TRUE,"zero and negative idle counts request another pass");
        expected({10,11,12,13,14},"idle updates main, wrapped descendants, delayed show and delayed hide in order");
        check(frame(0)->m_retailMiddle.m_nShowDelay==SW_SHOW &&
              frame(5)->m_retailMiddle.m_nShowDelay==SW_SHOW,
              "the captured main frame and frames without an HWND preserve their delay commands");
        check(frame(3)->m_retailMiddle.m_nShowDelay==-1 && frame(4)->m_retailMiddle.m_nShowDelay==-1 &&
              frame(6)->m_retailMiddle.m_nShowDelay==-1,"processed frames finish with no pending visibility command");
        check(IsWindowVisible(windows[3]) && !IsWindowVisible(windows[4]),"idle applies delayed frame show and hide");
        check(!std::memcmp(&previous,&active->thread()->m_lastSentMsg,sizeof(MSG)),"idle dispatch restores the caller's sent message");
    }
    reset();thread.get()->m_pMainWnd=frame(0);
    check(active->idle(thread.get(),0)==TRUE && events.empty(),"a hidden main window is skipped");
    frame(0)->m_hWnd=nullptr;
    check(active->idle(thread.get(),0)==TRUE && events.empty(),"a main wrapper without an HWND is skipped");
    thread.get()->m_pMainWnd=nullptr;
    check(active->idle(thread.get(),0)==TRUE && events.empty(),"idle supports a thread without a main window");

    reset();thread.get()->m_pMainWnd=frame(0);ShowWindow(windows[0],SW_SHOWNOACTIVATE);
    states[0]->m_frameList.m_pHead=frame(6);frame(6)->m_retailMiddle.m_nShowDelay=SW_SHOW;
    states[1]->m_frameList.m_pHead=frame(0);frame(0)->m_retailMiddle.m_pNextFrameWnd=frame(3);
    frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOWNOACTIVATE;mutation=1;
    check(active->idle(thread.get(),0)==TRUE,"idle supports module changes during the main-window callback");
    expected({10,11,12,13},"frame lookup uses the object's freshly changed module and the original main object");
    check(!IsWindowVisible(windows[6]),"an ambient module change does not select the old module's frame list");

    reset();states[0]->m_frameList.m_pHead=frame(3);mutation=2;
    frame(3)->m_retailMiddle.m_pNextFrameWnd=frame(4);frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOW;
    frame(4)->m_retailMiddle.m_nShowDelay=SW_SHOW;frame(6)->m_retailMiddle.m_nShowDelay=SW_SHOW;
    active->idle(thread.get(),0);
    expected({13,16},"idle reloads the next-frame link after callbacks");
    check(!IsWindowVisible(windows[4]) && frame(3)->m_retailMiddle.m_nShowDelay==-1,
          "callback changes select the pending show command and final reset still clears it");

    reset();states[0]->m_frameList.m_pHead=frame(3);frame(3)->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);
    frame(3)->m_retailMiddle.m_nShowDelay=0;mutation=4;
    active->idle(thread.get(),0);
    expected({100,13,104},"delayed visibility honors OLE ShowWindow and reloads a changed delay command");
    check(frame(3)->m_retailMiddle.m_nShowDelay==-1,"OLE callbacks cannot retain a completed delay command");

    reset();thread.get()->m_pMainWnd=frame(0);ShowWindow(windows[0],SW_SHOWNOACTIVATE);mutation=3;
    ShowWindow(windows[4],SW_SHOWNOACTIVATE);active->idle(thread.get(),0);
    expected({10},"descendant traversal reloads the main object's HWND after its update callback");

    reset();states[0]->m_frameList.m_pHead=frame(3);frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOW;
    throwAt=13;MSG before=active->thread()->m_lastSentMsg;bool caught=false;
    try {active->idle(thread.get(),0);}catch(int value) {caught=value==13;}
    check(caught && frame(3)->m_retailMiddle.m_nShowDelay==SW_SHOW,
          "a nonframework update exception interrupts pending frame visibility and propagates");
    check(active->thread()->m_lastSentMsg.hwnd==windows[3] && active->thread()->m_lastSentMsg.message==0x363,
          "an escaping nonframework exception retains the interrupted sent message");
    active->thread()->m_lastSentMsg=before;
    reset();states[0]->m_frameList.m_pHead=frame(3);frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOW;
    throwFramework=true;before=active->thread()->m_lastSentMsg;
    check(active->idle(thread.get(),0)==TRUE && deletedErrors==1 &&
          frame(3)->m_retailMiddle.m_nShowDelay==-1 && IsWindowVisible(windows[3]),
          "framework idle failures use the actual default thread handler, delete the error and continue visibility work");
    check(!std::memcmp(&before,&active->thread()->m_lastSentMsg,sizeof(MSG)),
          "a handled framework failure restores the caller's sent-message record");
    reset();states[0]->m_frameList.m_pHead=frame(3);frame(3)->m_retailMiddle.m_nShowDelay=SW_SHOW;
    throwFramework=true;threadTable[29]=reinterpret_cast<void*>(&failingProcessor);caught=false;
    before=active->thread()->m_lastSentMsg;
    try {active->idle(thread.get(),0);}catch(int value) {caught=value==77;}
    expected({13,98},"an idle framework failure dispatches to the registered thread's exception override");
    check(caught && deletedErrors==0 && frame(3)->m_retailMiddle.m_nShowDelay==SW_SHOW &&
          active->thread()->m_lastSentMsg.message==0x363,
          "a throwing exception override skips error deletion and retains the interrupted sent message");
    if(!deletedErrors)thrownError->Delete();
    active->thread()->m_lastSentMsg=before;

    throwAt=0;
}

void laterPasses() {
    for(LONG count : {LONG(1),LONG(2),LONG(LONG_MAX)}) {
        reset();check(active->idle(thread.get(),count)==FALSE,"positive idle counts end idle processing");
        expected({90},"the later pass invokes real final-unlock cleanup");
        check(states[0]->m_nTempMapLock==0,"an idle cleanup lock is balanced");
    }
    for(int lock : {1,2,-1,INT_MIN,INT_MAX}) {
        reset();states[0]->m_nTempMapLock=lock;
        check(!active->idle(thread.get(),1) && events.empty() && states[0]->m_nTempMapLock==lock,
              "a captured-module map lock suppresses idle cleanup for all nonzero values");
    }
    for(int lock : {0,1,-1,INT_MIN,INT_MAX}) {
        reset();select(1);states[1]->m_nTempMapLock=lock;
        active->idle(thread.get(),1);
        check(states[0]->m_nTempMapLock==0 && states[1]->m_nTempMapLock==(lock==-1 ? 0:lock),
              "cleanup gates on the object's module but balances the ambient module's lock, including wrap");
        check(events==std::vector<int>(lock==0 ? std::initializer_list<int>{90}:std::initializer_list<int>{}),
              "only the ambient final unlock performs OLE cleanup");
    }
    reset();CHandleMap temps(&tempClass,constructTemp,destroyTemp,8,1);
    states[0]->m_pmapHMENU=&temps;
    auto* temporary=temps.FromHandle(reinterpret_cast<HANDLE>(UINT_PTR(0x12345000)));
    check(temporary && temps.m_temporaryMap.m_nCount==1,"prepare a real temporary handle wrapper");
    active->idle(thread.get(),0);
    check(temps.m_temporaryMap.m_nCount==1 && !destructed,"first idle pass preserves temporary handle wrappers");
    active->idle(thread.get(),1);
    check(temps.m_temporaryMap.m_nCount==0 && destructed==1,"later idle pass destroys temporary wrappers through the real map backend");
    states[0]->m_pmapHMENU=nullptr;
}

void** import(HMODULE image,const char* wanted) {
    auto* b=reinterpret_cast<BYTE*>(image);
    auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* entries=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;entries->Name;++entries) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(b+entries->OriginalFirstThunk);
        auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(b+entries->FirstThunk);
        for(;names->u1.AddressOfData;++names,++values)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+names->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&values->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(HMODULE image,const char* name,void* callback) {
        slot=import(image,name);check(slot!=nullptr,name);if(!slot)std::abort();old=*slot;
        VirtualProtect(slot,sizeof(void*),PAGE_READWRITE,&protection);*slot=callback;
    }
    ~Patch() {*slot=old;DWORD ignored;VirtualProtect(slot,sizeof(void*),protection,&ignored);}
};
BOOL WINAPI noMessage(MSG* message,HWND h,UINT first,UINT last,UINT remove) {
    check(message==&active->thread()->m_msgCur && !h && !first && !last && remove==PM_NOREMOVE,
          "message loop checks the real thread record before each idle pass");
    event(70);return FALSE;
}
BOOL AFXAPI pumpQuit(CWinThread* self) {check(self==thread.get(),"loop pumps its original thread");event(71);return FALSE;}
int AFXAPI exitLoop(CWinThread* self) {check(self==thread.get(),"loop exits its original thread");event(72);return 0x13579;}
void loopIntegration() {
    reset();thread.get()->m_pMainWnd=frame(0);ShowWindow(windows[0],SW_SHOWNOACTIVATE);
    threadTable[26]=reinterpret_cast<void*>(active->idle);
    threadTable[25]=reinterpret_cast<void*>(&pumpQuit);threadTable[28]=reinterpret_cast<void*>(&exitLoop);
    Patch peek(active->image ? active->image:GetModuleHandleW(nullptr),"PeekMessageW",reinterpret_cast<void*>(&noMessage));
    check(active->run(thread.get())==0x13579,"the production loop returns its virtual exit result after both real idle passes");
    expected({70,10,11,12,70,90,71,72},"message-loop idle processing updates windows, cleans maps, then pumps and exits");
    threadTable[26]=reinterpret_cast<void*>(&overriddenIdle);
}

void run(const API& api) {
    active=&api;auto* saved=api.thread()->m_pModuleState;
    for(unsigned i=0;i<3;++i) {select(i);states[i]=api.state();}
    CHandleMap map(&windowClass,unexpected,unexpected,64,1);
    for(unsigned i=0;i<7;++i)map.m_permanentMap[windows[i]]=frame(i);
    for(auto* state:states)state->m_pmapHWND=&map;
    firstPass();laterPasses();loopIntegration();
    for(auto* state:states) {state->m_pmapHWND=nullptr;state->m_pCurrentWinThread=nullptr;
        state->m_frameList.m_pHead=nullptr;state->m_nTempMapLock=0;}
    api.thread()->m_pModuleState=saved;
    for(auto& module:modules)api.localDestroy(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));
    std::memset(modules,0,sizeof(modules));
    std::printf("%s idle processing completed\n",api.native ? "Native":"Source");
}
} // namespace

// Explicit boundaries cover wrapper construction, diagnostic virtuals,
// translation, centering and allocation failures. Tested paths never use them.
LRESULT CALLBACK AfxWndProc(HWND,UINT,WPARAM,LPARAM) {std::abort();}
extern "C" BOOL MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(MSG*) {std::abort();}
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long value) {throw value;}
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() {throw 91;}
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() {throw 19;}
extern "C" void MS_ABI impl__CenterWindow_CWnd__QEAAXPEAV1__Z(CWnd*,CWnd*) {std::abort();}
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND) {std::abort();}
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {
    auto* map=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pmapHWND;
    return map ? static_cast<CWnd*>(map->m_permanentMap.GetValueAt(handle)) : nullptr;
}

int main() {
    threadTable[26]=reinterpret_cast<void*>(&overriddenIdle);windowTable[71]=reinterpret_cast<void*>(&procedure);
    siteTable[42]=reinterpret_cast<void*>(&oleShow);
    windows[0]=CreateWindowExW(0,L"STATIC",L"idle main",WS_OVERLAPPEDWINDOW,0,0,100,100,nullptr,nullptr,nullptr,nullptr);
    windows[1]=CreateWindowExW(0,L"STATIC",L"wrapped child",WS_CHILD,0,0,50,50,windows[0],nullptr,nullptr,nullptr);
    windows[2]=CreateWindowExW(0,L"STATIC",L"wrapped descendant",WS_CHILD,0,0,25,25,windows[1],nullptr,nullptr,nullptr);
    for(unsigned i=3;i<7;++i)windows[i]=CreateWindowExW(0,L"STATIC",L"idle frame",WS_OVERLAPPEDWINDOW,0,0,100,100,nullptr,nullptr,nullptr,nullptr);
    for(auto h:windows)check(h!=nullptr,"create idle fixture window");
    API source{impl__OnIdle_CWinThread__UEAAHJ_Z,AfxGetThreadState,
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,impl___1CThreadLocalObject__QEAA_XZ,false,nullptr,impl__Run_CWinThread__UEAAHXZ,
        impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z};
    if(!failures)run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_THREAD_IDLE_DLL")) {
        HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native idle oracle");
        if(image) {
            auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"idle oracle uses the pinned native image");
            if(pinned) {
                API native=source;native.native=true;native.image=image;
#define RVA(member,offset) native.member=reinterpret_cast<decltype(native.member)>(b+offset)
                RVA(run,0x274390);RVA(idle,0x2744a0);RVA(thread,0x1332a0);RVA(state,0x133a20);
                RVA(localDestroy,0x14d0d0);RVA(baseException,0x274710);
#undef RVA
                run(native);
            }
            FreeLibrary(image);
        }
    }
    DestroyWindow(windows[0]);for(unsigned i=3;i<7;++i)DestroyWindow(windows[i]);
    std::printf("Thread idle: %d failures\n",failures);return failures ? 1:0;
}

CRuntimeClass CException::classCException{};
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const {std::abort();}
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) {std::abort();}
int CException::ReportError(UINT,UINT) {std::abort();}
void CException::Dump() const {std::abort();}
void CException::AssertValid() const {std::abort();}
