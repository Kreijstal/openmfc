// Exercise the actual drag source against real TLS, handle maps, locks and Win32
// message/capture APIs. A pinned native DLL supplies an independent local oracle.
#define OPENMFC_TEST_WINDOW_HANDLES
#define OPENMFC_TEST_REAL_MODULE_THREAD_STATE
#define OPENMFC_TEST_REAL_GLOBAL_LOCKS
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include "detail/InterfaceMapsSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include "detail/OleDropSourceSupport.h"
#include <array>
#include <vector>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cwchar>
extern "C" {
void* AFXAPI impl___0COleDropSource__QEAA_XZ(void*);
SCODE AFXAPI impl__QueryContinueDrag_COleDropSource__UEAAJHK_Z(COleDropSource*,BOOL,DWORD);
SCODE AFXAPI impl__GiveFeedback_COleDropSource__UEAAJK_Z(COleDropSource*,DWORD);
BOOL AFXAPI impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z(COleDropSource*,CWnd*);
HRESULT AFXAPI impl__QueryContinueDrag_XDropSource_COleDropSource__UEAAJHK_Z(COleDropSource::XDropSource*,BOOL,DWORD);
HRESULT AFXAPI impl__GiveFeedback_XDropSource_COleDropSource__UEAAJK_Z(COleDropSource::XDropSource*,DWORD);
void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
void* AFXAPI impl___0CWnd__QEAA_XZ(void*);
void AFXAPI impl___1CWnd__UEAA_XZ(CWnd*);
BOOL AFXAPI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
HWND AFXAPI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd*);
const AFX_INTERFACEMAP* AFXAPI impl__GetInterfaceMap_COleDropSource__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*);
}
namespace {
using Access=openmfc::detail::OleDropSourceAccess;
int failures;
void check(bool ok,const char* message) {if(!ok){++failures;std::printf("FAIL: %s\n",message);}}
struct API {
    decltype(&impl___0COleDropSource__QEAA_XZ) construct;
    void (AFXAPI* destroy)(COleDropSource*);
    decltype(&impl__QueryContinueDrag_COleDropSource__UEAAJHK_Z) query;
    decltype(&impl__GiveFeedback_COleDropSource__UEAAJK_Z) feedback;
    decltype(&impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z) begin;
    decltype(&impl__QueryContinueDrag_XDropSource_COleDropSource__UEAAJHK_Z) comQuery;
    decltype(&impl__GiveFeedback_XDropSource_COleDropSource__UEAAJK_Z) comFeedback;
    const AFX_INTERFACEMAP* (AFXAPI* map)();
    const AFX_INTERFACEMAP* (AFXAPI* instanceMap)(const void*);
    _AFX_THREAD_STATE* (AFXAPI* thread)();
    AFX_MODULE_THREAD_STATE* (AFXAPI* state)();
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) destroyLocal;
    decltype(&impl___0CWnd__QEAA_XZ) windowConstruct;
    decltype(&impl___1CWnd__UEAA_XZ) windowDestroy;
    decltype(&impl__Attach_CWnd__QEAAHPEAUHWND_____Z) attach;
    decltype(&impl__Detach_CWnd__QEAAPEAUHWND____XZ) detach;
    decltype(&impl__AfxLockGlobals__YAXH_Z) lock;
    decltype(&impl__AfxUnlockGlobals__YAXH_Z) unlock;
    UINT *delay,*distance;
    HMODULE image;
};
struct alignas(16) Object {
    std::array<BYTE,136> bytes;
    COleDropSource* get() {return reinterpret_cast<COleDropSource*>(bytes.data()+16);}
    void reset() {bytes.fill(0xa5);}
};
const API* active;
alignas(16) BYTE modules[3][0x110]{};
void select(unsigned i) {active->thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[i]);}
template<class T> T& field(void* p,size_t offset) {return *reinterpret_cast<T*>(static_cast<BYTE*>(p)+offset);}
std::vector<int> events;
void** imported(HMODULE image,const char* wanted) {
    auto* base=reinterpret_cast<BYTE*>(image);
    auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
    auto* item=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;item->Name;++item) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(base+item->OriginalFirstThunk);
        auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(base+item->FirstThunk);
        for(;names->u1.AddressOfData;++names,++values)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base+names->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&values->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(HMODULE image,const char* name,void* replacement) {
        slot=imported(image,name);check(slot!=nullptr,name);if(!slot)std::abort();old=*slot;
        VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=replacement;
    }
    ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
std::array<BYTE,136> snapshot(Object& object) {
    auto value=object.bytes;
    std::memset(value.data()+16,0,8);std::memset(value.data()+16+0x40,0,8);
    return value;
}
std::vector<std::array<BYTE,136>> ctorSamples;
COleDropSource* watched;
Object nested;
bool constructorRecord,captureLock,reentered,throwConstructor;
CRITICAL_SECTION* metricsLock;
decltype(&EnterCriticalSection) realEnter;
decltype(&LeaveCriticalSection) realLeave;
void WINAPI enter(CRITICAL_SECTION* lock) {
    if(captureLock)metricsLock=lock;
    if(constructorRecord && lock==metricsLock) {
        events.push_back(1);
        auto* self=watched;
        check(!Access::Rectangle(self).left && !Access::Rectangle(self).top && !Access::Rectangle(self).right && !Access::Rectangle(self).bottom &&
              !Access::Started(self) && !Access::Drop(self) && !Access::Cancel(self) && self->m_dwRef==1 &&
              self->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]),"base and drag fields initialize before locking");
        check(*reinterpret_cast<void**>(&self->m_xDropSource)!=nullptr,"the real COM interface exists before the settings callback");
        using Runtime=CRuntimeClass* (AFXAPI*)(const CCmdTarget*);
        auto* type=reinterpret_cast<Runtime>((*reinterpret_cast<void***>(self))[0])(self);
        check(active->image ? type && type->m_lpszClassName && !std::strcmp(type->m_lpszClassName,"CCmdTarget"):
              type==CCmdTarget::GetThisClass(),"the working native-order root is installed before the settings lock");
        check(self->m_xDropSource.AddRef()==2 && self->m_xDropSource.Release()==1 && self->m_dwRef==1,
              "the embedded interface already owns the real base reference count before locking");
        if(throwConstructor)throw 77;
    }
    realEnter(lock);
}
void WINAPI leave(CRITICAL_SECTION* lock) {if(constructorRecord && lock==metricsLock)events.push_back(4);realLeave(lock);}
UINT WINAPI profile(LPCWSTR section,LPCWSTR key,INT fallback) {
    bool distance=!std::wcscmp(key,L"DragMinDist");
    check(constructorRecord && !std::wcscmp(section,L"windows") && (distance || !std::wcscmp(key,L"DragDelay")) &&
          fallback==(distance ? 2:200),"settings use the native profile section, keys and defaults");
    events.push_back(distance ? 2:3);
    if(distance && !reentered) {
        reentered=true;auto* outer=watched;nested.reset();watched=nested.get();active->construct(watched);watched=outer;
    }
    if(!distance)check(*active->distance==7,"distance is committed before reading the delay setting");
    return distance ? 7:11;
}
void constructors() {
    Object object;object.reset();watched=object.get();reentered=false;events.clear();constructorRecord=true;
    check(active->construct(watched)==watched,"constructor returns placement storage");constructorRecord=false;
    check(events==std::vector<int>({1,2,1,2,3,4,3,4}) && *active->distance==7 && *active->delay==11,
          "recursive initialization uses the recursive global lock and publishes each profile result in native order");
    for(auto* item:{&object,&nested}) {
        if(!active->image)ctorSamples.push_back(snapshot(*item));
        else check(snapshot(*item)==ctorSamples[item==&object ? 0:1],"source and native construction match all data, padding and guard bytes");
        auto* value=item->get();auto* type=value->GetRuntimeClass();
        check(active->image ? type && type->m_lpszClassName && !std::strcmp(type->m_lpszClassName,"CCmdTarget") && type->m_nObjectSize==64:
              type==CCmdTarget::GetThisClass(),"drag source inherits command-target runtime metadata");
        active->destroy(value);
    }
    *active->distance=31;*active->delay=29;events.clear();object.reset();watched=object.get();constructorRecord=true;
    active->construct(watched);constructorRecord=false;
    check(events==std::vector<int>({1,4}) && *active->distance==31 && *active->delay==29,"later construction preserves writable cached settings");
    active->destroy(watched);
    object.reset();watched=object.get();events.clear();throwConstructor=true;constructorRecord=true;int caught=0;
    try{active->construct(watched);}catch(int value){caught=value;}
    constructorRecord=false;throwConstructor=false;
    check(caught==77 && events==std::vector<int>({1}) && !watched->m_pModuleState,"failed lock entry unwinds the actual base without publishing or unlocking");
}
struct QueryResult {SCODE code;BOOL started;bool operator==(const QueryResult& other)const{return code==other.code && started==other.started;}};
std::vector<QueryResult> querySamples;
void decisions() {
    Object object;object.reset();auto* self=object.get();active->construct(self);
    field<void*>(self,0)=reinterpret_cast<void*>(UINT_PTR(1));size_t sample=0;select(1);
    for(BOOL escape:{0,1,-1})for(BOOL started:{0,1,-1,17})for(DWORD cancel:{0u,1u,2u,4u,0x80000000u})
    for(DWORD drop:{0u,1u,2u,4u,0x80000000u})for(DWORD keys:{0u,1u,2u,3u,4u,5u,UINT_MAX,0x80000000u}) {
        Access::Started(self)=started;Access::Cancel(self)=cancel;Access::Drop(self)=drop;
        QueryResult result{active->query(self,escape,keys),Access::Started(self)};
        if(!active->image)querySamples.push_back(result);else check(result==querySamples[sample],"native and source drag decisions match all flags and button masks");
        ++sample;
        check(active->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"base drag queries preserve ambient module state");
        if(escape || (cancel&keys))check(result.code==DRAGDROP_S_CANCEL && !result.started,"cancel clears the drag-started flag");
    }
    for(BOOL started:{0,1,-1,17})for(DWORD effect:{0u,1u,2u,4u,UINT_MAX}) {
        Access::Started(self)=started;
        check(active->feedback(self,effect)==(started ? DRAGDROP_S_USEDEFAULTCURSORS:S_OK) && Access::Started(self)==started,
              "qualified feedback uses the stored start flag, ignores the incoming root and preserves the raw flag");
    }
    active->destroy(self);select(0);
}
COleDropSource* expectedOwner;
SCODE AFXAPI queryOverride(COleDropSource* self,BOOL escape,DWORD keys) {
    check(self==expectedOwner && escape==-17 && keys==0x12345678 && active->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),
          "COM query forwards to the owner override without switching module state");return 0x1234;
}
SCODE AFXAPI feedbackOverride(COleDropSource* self,DWORD effect) {
    check(self==expectedOwner && effect==0xabcdef01 && active->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]),
          "COM feedback enters the captured owner module before invoking the override");
    select(2);self->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[2]);return 0x5678;
}
void AFXAPI finalRelease(CCmdTarget* self) {
    check(self==expectedOwner && !self->m_dwRef && active->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]),
          "the last interface release reaches the actual owner final-release hook in its module");events.push_back(6);
}
struct Outer : IUnknown {
    int calls=0;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void** result) override {++calls;*result=this;return S_FALSE;}
    ULONG STDMETHODCALLTYPE AddRef() override {++calls;return 95;}
    ULONG STDMETHODCALLTYPE Release() override {++calls;return 94;}
};
void interfaces() {
    select(0);Object object;object.reset();auto* self=object.get();active->construct(self);expectedOwner=self;
    auto* map=active->map();
    check(active->instanceMap(reinterpret_cast<void*>(UINT_PTR(1)))==map && map && map->pfnGetBaseMap &&
          map->pEntries && IsEqualIID(*static_cast<const IID*>(map->pEntries[0].piid),IID_IDropSource) && map->pEntries[0].nOffset==0x40 &&
          !map->pEntries[1].piid && map->pEntries[1].nOffset==SIZE_MAX && map->pfnGetBaseMap(),"the actual interface map exposes IDropSource at its native offset with a command-target base");
    auto* com=&self->m_xDropSource;void* result=nullptr;
    for(const IID* iid:{&IID_IUnknown,&IID_IDropSource}) {
        check(com->QueryInterface(*iid,&result)==S_OK && result==com && self->m_dwRef==2,"interface identity and references belong to the command target");
        check(com->Release()==1 && self->m_dwRef==1,"interface release decrements the owner rather than a private counter");
    }
    result=reinterpret_cast<void*>(UINT_PTR(1));check(com->QueryInterface(IID_NULL,&result)==E_NOINTERFACE && !result && self->m_dwRef==1,"unsupported interfaces clear the result without adding a reference");
    Outer outer;self->m_pOuterUnknown=&outer;
    check(com->AddRef()==95 && com->Release()==94 && com->QueryInterface(IID_IDropSource,&result)==S_FALSE && result==&outer && outer.calls==3 && self->m_dwRef==1,
          "all three controlling-unknown operations delegate when aggregated");self->m_pOuterUnknown=nullptr;
    void* root[25];std::memcpy(root,*reinterpret_cast<void***>(self),sizeof(root));root[22]=reinterpret_cast<void*>(&queryOverride);root[23]=reinterpret_cast<void*>(&feedbackOverride);root[6]=reinterpret_cast<void*>(&finalRelease);field<void*>(self,0)=root;
    select(1);check(com->QueryContinueDrag(-17,0x12345678)==0x1234 && active->comQuery(com,-17,0x12345678)==0x1234,"interface and exported query both perform virtual owner dispatch");
    for(int mode:{0,1}) {
        self->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);select(1);
        HRESULT value=mode ? active->comFeedback(com,0xabcdef01):com->GiveFeedback(0xabcdef01);
        check(value==0x5678 && active->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),
              "feedback restores the saved module after callbacks change both current and owner state");
    }
    self->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);events.clear();
    check(com->Release()==0 && events==std::vector<int>({6}) && !self->m_dwRef && active->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),
          "final interface release has native owner lifetime and balanced module scope");
    active->destroy(self);select(0);
}
// Keep the actual capture and queue behavior while making key/tick input
// deterministic. The observers also verify API order and callback mutations.
bool dragging;int dragMode,tickIndex,captureReads,mouseReads,keyReads,pointReads,releases;
SHORT leftKey,rightKey;std::array<ULONGLONG,3> ticks;
CWnd* dragWindow;HWND alternateWindow;
decltype(&SetCapture) realSetCapture;decltype(&GetCapture) realGetCapture;decltype(&ReleaseCapture) realReleaseCapture;
decltype(&PeekMessageW) realPeek;decltype(&PtInRect) realPoint;
SHORT WINAPI keyState(int key) {
    if(!dragging)return 0;
    events.push_back(key==VK_LBUTTON ? 10:11);
    check(key==VK_LBUTTON || key==VK_RBUTTON,"only mouse-button key state is queried");
    return key==VK_LBUTTON ? leftKey:rightKey;
}
ULONGLONG WINAPI tickCount() {
    if(!dragging)return 0;
    events.push_back(12);int index=tickIndex++;
    if(index==0 && dragMode==3) {active->detach(dragWindow);check(active->attach(dragWindow,alternateWindow)!=FALSE,"tick callback reattaches the actual window wrapper");}
    if(index==1 && dragMode==4)*active->delay=0;
    check(index<3,"drag loop completes with the bounded clock sequence");
    return ticks[index<3 ? index:2]+(index>=3 ? ULONGLONG(index):0);
}
HWND WINAPI setCapture(HWND window) {
    if(dragging) {events.push_back(13);check(window==dragWindow->m_hWnd,"capture reads the window handle after input callbacks");}
    HWND previous=realSetCapture(window);
    if(dragging && dragMode==1)Access::Started(watched)=-17;
    return previous;
}
HWND WINAPI getCapture() {
    if(dragging) {events.push_back(14);++captureReads;if(dragMode==2)realReleaseCapture();}
    return realGetCapture();
}
BOOL WINAPI peek(MSG* message,HWND window,UINT first,UINT last,UINT remove) {
    if(dragging) {
        check(!window && remove==PM_REMOVE && ((first==0x200 && last==0x209)||(first==0x100 && last==0x109)),"drag input uses the native mouse and keyboard filter ranges");
        if(first==0x200){events.push_back(15);++mouseReads;}else{events.push_back(16);++keyReads;}
    }
    return realPeek(message,window,first,last,remove);
}
BOOL WINAPI point(const RECT* rect,POINT position) {
    if(dragging) {events.push_back(17);++pointReads;check(rect==reinterpret_cast<RECT*>(&Access::Rectangle(watched)),"drag boundary uses the owner's live rectangle");}
    return realPoint(rect,position);
}
BOOL WINAPI releaseCapture() {if(dragging){events.push_back(18);++releases;}return realReleaseCapture();}
struct DragResult {BOOL value;DWORD cancel,drop;int ticks,captures,mouse,keys,points;std::vector<int> trace;
 bool operator==(const DragResult& b)const{return value==b.value && cancel==b.cancel && drop==b.drop && ticks==b.ticks && captures==b.captures && mouse==b.mouse && keys==b.keys && points==b.points && trace==b.trace;}};
std::vector<DragResult> dragSamples;
void starts() {
    select(0);Object object;object.reset();watched=object.get();active->construct(watched);field<void*>(watched,0)=reinterpret_cast<void*>(UINT_PTR(1));
    alignas(16) BYTE windowBytes[sizeof(CWnd)];auto* window=reinterpret_cast<CWnd*>(windowBytes);active->windowConstruct(window);dragWindow=window;
    HWND handles[3];for(auto& handle:handles)handle=CreateWindowExW(0,L"STATIC",L"drag source",WS_POPUP,500,500,50,50,nullptr,nullptr,nullptr,nullptr);
    check(handles[0] && handles[1] && handles[2] && active->attach(window,handles[0]),"create and permanently map real capture windows");alternateWindow=handles[1];
    check(SetCursorPos(50,50)!=FALSE,"set a deterministic queue-message position");
    const UINT messages[]={0,WM_MOUSEMOVE,WM_LBUTTONDOWN,WM_LBUTTONUP,WM_LBUTTONDBLCLK,WM_RBUTTONDOWN,WM_RBUTTONUP,WM_RBUTTONDBLCLK,
        WM_MBUTTONDOWN,WM_MBUTTONUP,WM_MBUTTONDBLCLK,WM_KEYDOWN,WM_CHAR,WM_SYSKEYDOWN,WM_MOUSEWHEEL};
    size_t sample=0;
    for(UINT message:messages)for(int mode:{0,1,2,3,4,5,6})for(int buttons:{0,1,2,3}) {
        MSG discard;while(realPeek(&discard,nullptr,0,0,PM_REMOVE)){}
        if(window->m_hWnd!=handles[0]){active->detach(window);active->attach(window,handles[0]);}
        dragMode=mode;leftKey=(buttons&1) ? SHORT(0x8001):SHORT(1);rightKey=(buttons&2) ? SHORT(0x8000):SHORT(1);
        *active->delay=11;Access::Started(watched)=19;Access::Cancel(watched)=UINT_MAX;Access::Drop(watched)=UINT_MAX;
        Access::Rectangle(watched)=mode==5 ? CRect(0,0,1,1):CRect(40,40,60,60);
        ticks={100,111,112};if(mode==6)ticks={ULLONG_MAX-5,5,6};
        tickIndex=captureReads=mouseReads=keyReads=pointReads=releases=0;events.clear();
        if(message)check(PostThreadMessageW(GetCurrentThreadId(),message,message==WM_KEYDOWN ? VK_ESCAPE:0,0)!=FALSE,"post real drag input");
        dragging=true;BOOL value=active->begin(watched,window);dragging=false;
        DragResult result{value,Access::Cancel(watched),Access::Drop(watched),tickIndex,captureReads,mouseReads,keyReads,pointReads,events};
        check(value==Access::Started(watched) && releases==1 && !realGetCapture(),"every normal drag-start exit releases capture and returns the physical start flag");
        const DWORD dropMasks[]={0,1,2,1},cancelMasks[]={0,2,1,2};
        check(result.drop==dropMasks[buttons] && result.cancel==cancelMasks[buttons],"left-button precedence selects the native confirmation and cancellation masks");
        if(mode==1)check(value==-17 && !captureReads && tickIndex==1,"a capture callback's raw start flag bypasses polling");
        if(mode==2)check(!value && captureReads==1 && !mouseReads,"capture loss cancels before reading queued input");
        if(!message && mode==0)check(value==1 && tickIndex==3 && captureReads==2,"drag timeout waits through equality and starts only after the delay");
        if(!message && mode==4)check(value==1 && tickIndex==2,"the clock callback's changed delay is read before deciding to start");
        if(!message && mode==6)check(value==1 && tickIndex==3,"drag elapsed time uses unsigned 64-bit wrapping arithmetic");
        if(message==WM_KEYDOWN && mode!=1 && mode!=2)check(!value && tickIndex==1 && !pointReads,"escape cancels without a rectangle or second clock call");
        if(message==WM_MOUSEWHEEL && mode==0)check(realPeek(&discard,nullptr,WM_MOUSEWHEEL,WM_MOUSEWHEEL,PM_REMOVE)!=FALSE,"messages above the native mouse range stay queued");
        if(!active->image)dragSamples.push_back(result);else check(result==dragSamples[sample],"source and native drag starts match return state and the full Win32 callback trace");
        ++sample;
    }
    // The apparently unused FromHandle(previousCapture) call must still create
    // its actual temporary wrapper, before the loop begins.
    if(window->m_hWnd!=handles[0]){active->detach(window);active->attach(window,handles[0]);}
    auto* map=active->state()->m_pmapHWND;void* previous=nullptr;
    check(map && !map->m_temporaryMap.Lookup(handles[2],previous),"previous capture begins without a temporary wrapper");
    realSetCapture(handles[2]);dragMode=1;tickIndex=0;ticks={100,111,112};dragging=true;active->begin(watched,window);dragging=false;
    check(map->m_temporaryMap.Lookup(handles[2],previous) && previous && static_cast<CWnd*>(previous)->m_hWnd==handles[2],"previous capture creates a real cached temporary window wrapper even when polling is skipped");
    active->detach(window);active->windowDestroy(window);for(auto handle:handles)DestroyWindow(handle);active->destroy(watched);
}
void run(const API& api) {
    active=&api;auto* old=api.thread()->m_pModuleState;select(0);api.lock(3);api.unlock(3);
    HMODULE image=api.image ? api.image:GetModuleHandleW(nullptr);
    {
        Patch a(image,"EnterCriticalSection",reinterpret_cast<void*>(&enter)),b(image,"LeaveCriticalSection",reinterpret_cast<void*>(&leave)),c(image,"GetProfileIntW",reinterpret_cast<void*>(&profile));
        realEnter=reinterpret_cast<decltype(realEnter)>(a.old);realLeave=reinterpret_cast<decltype(realLeave)>(b.old);
        captureLock=true;api.lock(3);captureLock=false;api.unlock(3);constructors();
    }
    std::puts("Construction completed");decisions();std::puts("Button decisions completed");interfaces();std::puts("COM lifetime completed");
    {
        Patch a(image,"GetKeyState",reinterpret_cast<void*>(&keyState)),b(image,"GetTickCount64",reinterpret_cast<void*>(&tickCount)),
            c(image,"SetCapture",reinterpret_cast<void*>(&setCapture)),d(image,"GetCapture",reinterpret_cast<void*>(&getCapture)),
            e(image,"ReleaseCapture",reinterpret_cast<void*>(&releaseCapture)),f(image,"PeekMessageW",reinterpret_cast<void*>(&peek)),g(image,"PtInRect",reinterpret_cast<void*>(&point));
        realSetCapture=reinterpret_cast<decltype(realSetCapture)>(c.old);realGetCapture=reinterpret_cast<decltype(realGetCapture)>(d.old);
        realReleaseCapture=reinterpret_cast<decltype(realReleaseCapture)>(e.old);realPeek=reinterpret_cast<decltype(realPeek)>(f.old);realPoint=reinterpret_cast<decltype(realPoint)>(g.old);starts();
    }
    api.thread()->m_pModuleState=old;for(auto& module:modules)api.destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));std::memset(modules,0,sizeof(modules));
    std::printf("%s drag source: 2400 button cases, 420 message/callback cases completed\n",api.image ? "Native":"Source");
}
}
int main() {
    setvbuf(stdout,nullptr,_IONBF,0);events.reserve(64);ctorSamples.reserve(2);querySamples.reserve(2400);dragSamples.reserve(420);
    API source{impl___0COleDropSource__QEAA_XZ,[](COleDropSource* self){self->COleDropSource::~COleDropSource();},impl__QueryContinueDrag_COleDropSource__UEAAJHK_Z,
        impl__GiveFeedback_COleDropSource__UEAAJK_Z,impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z,
        impl__QueryContinueDrag_XDropSource_COleDropSource__UEAAJHK_Z,impl__GiveFeedback_XDropSource_COleDropSource__UEAAJK_Z,
        impl__GetThisInterfaceMap_COleDropSource__KAPEBUAFX_INTERFACEMAP__XZ,impl__GetInterfaceMap_COleDropSource__MEBAPEBUAFX_INTERFACEMAP__XZ,
        AfxGetThreadState,impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,impl___1CThreadLocalObject__QEAA_XZ,
        impl___0CWnd__QEAA_XZ,impl___1CWnd__UEAA_XZ,impl__Attach_CWnd__QEAAHPEAUHWND_____Z,impl__Detach_CWnd__QEAAPEAUHWND____XZ,
        impl__AfxLockGlobals__YAXH_Z,impl__AfxUnlockGlobals__YAXH_Z,&Access::Delay(),&Access::Distance(),nullptr};
    run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_DROP_DLL")) {
        auto image=LoadLibraryA(path);check(image!=nullptr,"load native drag source oracle");
        if(image) {
            auto* base=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"native oracle is the pinned image");
            if(pinned) {
                API native=source;native.image=image;
#define RVA(member,value) native.member=reinterpret_cast<decltype(native.member)>(base+value)
                RVA(construct,0x25a350);RVA(destroy,0x1de430);RVA(query,0x25a410);RVA(feedback,0x25a440);RVA(begin,0x25a450);
                RVA(comQuery,0x25a610);RVA(comFeedback,0x25a630);RVA(map,0x25a590);RVA(instanceMap,0x25a590);
                RVA(thread,0x1332a0);RVA(state,0x133a20);RVA(destroyLocal,0x14d0d0);RVA(windowConstruct,0x28a700);RVA(windowDestroy,0x28b740);
                RVA(attach,0x28adf0);RVA(detach,0x28ae50);RVA(lock,0x33540);RVA(unlock,0x335e0);RVA(delay,0x3be418);RVA(distance,0x3be41c);
#undef RVA
                field<DWORD>(base,0x3c41b0)=0;*native.delay=0;*native.distance=0;run(native);
            }
            FreeLibrary(image);
        }
    }
    std::printf("Drag source: %d failures\n",failures);return failures ? 1:0;
}
