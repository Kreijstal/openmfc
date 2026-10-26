// The rendering support supplies unrelated virtual siblings. The window
// constructors, destructors, maps, allocator and lifetime handlers are real.
#define OPENMFC_TEST_WINDOW_HANDLES
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include <array>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <exception>
#include <atomic>
#include <thread>
#include <type_traits>
extern "C" void* AFXAPI impl___0CWnd__AEAA_PEAUHWND_____Z(void*,HWND);
extern "C" void AFXAPI impl__OnDestroy_CWnd__IEAAXXZ(CWnd*);
extern "C" void AFXAPI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd*);
extern "C" void AFXAPI impl__PostNcDestroy_CWnd__MEAAXXZ(CWnd*);
extern "C" void AFXAPI impl__EnableD2DSupport_CWnd__QEAAXHH_Z(CWnd*,int,int);
static int failures;
static void check(bool ok,const char* message) {
    if(!ok){++failures;std::printf("FAIL: %s\n",message);}
}
template<class T> static T& field(void* p,size_t offset) {
    return *reinterpret_cast<T*>(static_cast<BYTE*>(p)+offset);
}
struct API {
    const char* name;
    void* (AFXAPI* construct)(void*);
    void* (AFXAPI* constructHandle)(void*,HWND);
    void (AFXAPI* destroy)(CWnd*);
    CWnd* (AFXAPI* from)(HWND);
    CWnd* (AFXAPI* permanent)(HWND);
    BOOL (AFXAPI* attach)(CWnd*,HWND);
    HWND (AFXAPI* detach)(CWnd*);
    void (AFXAPI* clear)();
    BOOL (AFXAPI* destroyWindow)(CWnd*);
    void (AFXAPI* onDestroy)(CWnd*);
    void (AFXAPI* onNcDestroy)(CWnd*);
    void (AFXAPI* post)(CWnd*);
    AFX_MODULE_THREAD_STATE* (AFXAPI* state)();
    AFX_MODULE_STATE* (AFXAPI* module)();
    void* (AFXAPI* stateConstruct)(void*);
    void (AFXAPI* stateDestroy)(void*);
    _AFX_THREAD_STATE* (AFXAPI* thread)();
    void (AFXAPI* localDestroy)(void*);
    void (AFXAPI* enableD2D)(CWnd*,BOOL,BOOL);
    AFX_MODULE_THREAD_STATE::NewHandler pressure;
};
static thread_local AFX_MODULE_THREAD_STATE* selected;
static thread_local int stateReads;
static thread_local CWnd* mutateWindow;
static thread_local HWND mutatedHandle;
static thread_local COleControlSite* mutatedSite;
static void mutateHandle() {
    if(mutateWindow){mutateWindow->m_hWnd=mutatedHandle;mutateWindow->m_pCtrlSite=mutatedSite;mutateWindow=nullptr;}
}
static AFX_MODULE_THREAD_STATE* readState() {++stateReads;mutateHandle();return selected;}
struct Context {
    const API& api;
    alignas(8) BYTE storage[sizeof(AFX_MODULE_THREAD_STATE)]{};
    AFX_MODULE_THREAD_STATE* state;
    AFX_MODULE_THREAD_STATE* previousSelected;
    decltype(g_windowModuleThreadStateHook) previousHook;
    CHandleMap* oldMap;CWinThread* oldThread;CToolTipCtrl* oldTooltip;
    AFX_MODULE_THREAD_STATE::NewHandler oldHandler;
    explicit Context(const API& a):api(a) {
        previousSelected=selected;previousHook=g_windowModuleThreadStateHook;
        state=api.stateConstruct ? api.state() : reinterpret_cast<AFX_MODULE_THREAD_STATE*>(storage);
        oldMap=state->m_pmapHWND;oldThread=state->m_pCurrentWinThread;oldTooltip=state->m_pToolTip;oldHandler=state->m_pfnNewHandler;
        state->m_pmapHWND=nullptr;state->m_pCurrentWinThread=nullptr;state->m_pToolTip=nullptr;
        if(!api.stateConstruct)state->m_pfnNewHandler=TestAllocationFailure;
        selected=state;g_windowModuleThreadStateHook=readState;stateReads=0;
    }
    ~Context() {
        auto* owned=state->m_pmapHWND;
        state->m_pmapHWND=oldMap;state->m_pCurrentWinThread=oldThread;state->m_pToolTip=oldTooltip;state->m_pfnNewHandler=oldHandler;
        if(owned) {
            if(api.stateConstruct) {
                alignas(8) BYTE teardown[sizeof(AFX_MODULE_THREAD_STATE)]{};
                api.stateConstruct(teardown);
                field<CHandleMap*>(teardown,0x28)=owned;
                api.stateDestroy(teardown);
            } else {owned->~CHandleMap();std::free(owned);}
        }
        g_windowModuleThreadStateHook=previousHook;selected=previousSelected;
    }
};
struct Object {
    const API& api;
    alignas(8) BYTE bytes[264];
    CWnd* window;
    void* root;
    bool destroyed=false;
    explicit Object(const API& a,HWND handle=nullptr):api(a) {
        std::memset(bytes,0xa5,sizeof(bytes));window=reinterpret_cast<CWnd*>(bytes+16);
        api.constructHandle(window,handle);root=*reinterpret_cast<void**>(window);
    }
    ~Object() {
      if(!destroyed) {
        window->m_hWnd=nullptr;window->m_pCtrlSite=nullptr;
        window->m_pCtrlCont=nullptr;window->m_pMFCCtrlContainer=nullptr;
        window->m_pRenderTarget=nullptr;window->m_pProxy=nullptr;window->m_pStdObject=nullptr;
        window->m_pDropTarget=nullptr;window->m_pDynamicLayout=nullptr;window->m_pCurrentGestureInfo=nullptr;
        window->m_xDispatch=0;
        api.destroy(window);
      }
        for(int i=0;i<16;++i)check(bytes[i]==0xa5 && bytes[248+i]==0xa5,"window lifetime preserves storage canaries");
    }
};
static void constructors(const API& api) {
    for(HWND handle:{HWND(nullptr),reinterpret_cast<HWND>(UINT_PTR(0x7788))}) {
        Object object(api,handle);BYTE expected[232];std::memset(expected,0xa5,sizeof(expected));
        field<DWORD>(expected,8)=1;
        for(size_t offset:{size_t(16),size_t(24),size_t(32),size_t(48)})field<UINT_PTR>(expected,offset)=0;
        field<BOOL>(expected,40)=TRUE;field<AFX_MODULE_STATE*>(expected,56)=api.module();
        field<HWND>(expected,64)=handle;expected[72]=0;
        field<DWORD>(expected,112)=0;field<LONG>(expected,116)=-1;field<LONG>(expected,120)=-1;
        field<DWORD>(expected,136)=0;field<DWORD>(expected,168)=0;field<DWORD>(expected,184)=0;
        for(size_t offset:{size_t(80),size_t(88),size_t(128),size_t(144),size_t(152),size_t(160),size_t(176),size_t(192),size_t(200),size_t(208),size_t(216),size_t(224)})
            field<UINT_PTR>(expected,offset)=0;
        for(size_t offset:{size_t(0),size_t(96),size_t(104)}) {
            void* table=field<void*>(object.window,offset);
            check(table!=nullptr,"constructor installs root and embedded accessibility tables");
            field<void*>(expected,offset)=table;
        }
        check(!std::memcmp(expected,object.window,sizeof(expected)),"constructor matches native named fields and preserves every padding byte");
    }
}
static void specialWindows(const std::array<const CWnd*,4>& windows) {
    HWND handles[]={HWND_BOTTOM,HWND_NOTOPMOST,HWND_TOP,HWND_TOPMOST};
    for(size_t i=0;i<windows.size();++i) {
        const auto* object=windows[i];auto* bytes=reinterpret_cast<const BYTE*>(object);
        void* root;void* accessible;void* server;std::memcpy(&root,bytes,8);
        std::memcpy(&accessible,bytes+96,8);std::memcpy(&server,bytes+104,8);
        check(object->m_hWnd==handles[i] && root && accessible && server && object->m_dwRef==1 &&
            object->m_bResultExpected==TRUE && !object->m_pRenderTarget && !object->m_pCtrlCont,
            "special HWND exports are full constructed windows with native handles and interface tables");
    }
}
static void maps(const API& api) {
    Context context(api);
    check(!api.permanent(nullptr) && !context.state->m_pmapHWND,"permanent lookup does not create a map, including NULL HWND");
    check(!api.attach(nullptr,nullptr) && !context.state->m_pmapHWND,"Attach(NULL) fails before touching the object or map");
    api.clear();check(!context.state->m_pmapHWND,"temporary cleanup does not create a map");
    auto handler=context.state->m_pfnNewHandler;
    check(!api.from(nullptr) && context.state->m_pmapHWND,"FromHandle(NULL) still creates the HWND map");
    auto* map=context.state->m_pmapHWND;
    check(map->m_nOffset==64 && map->m_nHandles==1 && map->m_pClass->m_nObjectSize==232 &&
        map->m_permanentMap.m_nHashTableSize==17 && map->m_temporaryMap.m_nHashTableSize==7 &&
        map->m_pfnConstruct && map->m_pfnDestruct,"HWND factory publishes a native CHandleMap with real callbacks");
    check(context.state->m_pfnNewHandler==handler,"HWND factory restores the allocation handler on success");
    HWND first=reinterpret_cast<HWND>(UINT_PTR(0x1122)),second=reinterpret_cast<HWND>(UINT_PTR(0x3344));
    CWnd* temporary=api.from(first);
    check(temporary && temporary->m_hWnd==first && api.from(first)==temporary && !api.permanent(first),
        "temporary identity is stable and remains separate from the permanent map");
    Object permanent(api);
    check(api.attach(permanent.window,first) && api.permanent(first)==permanent.window && api.from(first)==permanent.window,
        "a permanent wrapper takes precedence while its temporary sibling remains owned by the pool");
    check(api.attach(permanent.window,second) && api.permanent(first)==permanent.window && api.permanent(second)==permanent.window,
        "reattachment preserves the previous key, matching native Attach semantics");
    permanent.window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(UINT_PTR(0x7788));
    check(api.detach(permanent.window)==second && !permanent.window->m_hWnd && !permanent.window->m_pCtrlSite &&
        !api.permanent(second) && api.permanent(first)==permanent.window,"Detach removes the current key and clears only the two native links");
    permanent.window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(UINT_PTR(0x7788));
    check(!api.detach(permanent.window) && !permanent.window->m_pCtrlSite,"Detach clears the control-site link even without an HWND");
    map->m_permanentMap.RemoveKey(first);
    HWND actual=CreateWindowExW(0,L"STATIC",L"temporary ownership",WS_POPUP,0,0,30,30,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    check(actual!=nullptr,"create a real HWND for temporary ownership");
    api.from(actual);api.from(second);api.clear();
    check(!map->m_temporaryMap.GetCount() && IsWindow(actual),"DeleteTempMap deletes all temporary wrappers without destroying their HWNDs");
    api.attach(permanent.window,actual);api.clear();
    check(api.permanent(actual)==permanent.window && permanent.window->m_hWnd==actual,
        "temporary cleanup preserves permanent objects and their live HWNDs");
    api.detach(permanent.window);DestroyWindow(actual);
}
struct Node {
    void** vptr;
    int id;
    BYTE tail[80]{};
};
static CWnd* activeWindow;
static std::vector<int> events;
static int failureAt;
static void event(int id) {events.push_back(id);if(id==failureAt)throw 41;}
static void* AFXAPI deleting(Node* node,unsigned flags) {
    check(flags==1,"owned members use the scalar deleting destructor flag");
    if(node->id==4)check(activeWindow->m_pRenderTarget==reinterpret_cast<CHwndRenderTarget*>(node),"render-target deletion precedes clearing its CWnd field");
    event(node->id);return node;
}
static ULONG AFXAPI releasing(Node* node) {event(node->id);return 1;}
static HRESULT AFXAPI server(Node* node,void* first,void* second) {check(!first && !second,"accessibility proxy is disconnected with two NULL arguments");event(node->id);return S_OK;}
static BOOL AFXAPI destroySite(Node* node) {event(node->id);return 69;}
static void AFXAPI revoke(Node* node) {event(node->id);}
static WNDPROC defaultReplacement;
static LRESULT AFXAPI defaultProc(CWnd* self,UINT,WPARAM,LPARAM) {
    event(7);
    if(defaultReplacement)SetWindowLongPtrW(self->m_hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(defaultReplacement));
    return 73;
}
static WNDPROC superProcedure;
static WNDPROC* AFXAPI superProc(CWnd*) {event(8);return &superProcedure;}
static void AFXAPI post(CWnd* self) {check(!self->m_hWnd && !self->m_pCtrlSite,"PostNcDestroy observes a detached wrapper");event(9);}
static void AFXAPI oleTerm(BOOL first,BOOL second) {check(first==TRUE && second==TRUE,"WM_QUIT terminates OLE with native flags");event(10);}
static void* expectedRoot;
static AFX_MODULE_STATE* expectedModule;
static void AFXAPI disconnect(void* embedded) {
    check(embedded==reinterpret_cast<BYTE*>(activeWindow)+32 && activeWindow->m_pModuleState==expectedModule &&
        *reinterpret_cast<void**>(activeWindow)==expectedRoot,"CWnd destructor retains its own root and module during base dispatch disconnection");
    check(!activeWindow->m_pDynamicLayout,"base destruction follows dynamic-layout deletion");event(12);
}

// Observe physical deallocation without changing the production allocation
// path. The source and pinned DLL both use the same /MD CRT in this harness.
static void** imported(HMODULE module,const char* name) {
    auto* b=reinterpret_cast<BYTE*>(module);
    auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto rva=pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    for(auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+rva);rva && d->Name;++d) {
        if(!d->OriginalFirstThunk)continue;
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);
        auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);
        for(size_t i=0;names[i].u1.AddressOfData;++i)if(!IMAGE_SNAP_BY_ORDINAL(names[i].u1.Ordinal)) {
            auto* entry=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+names[i].u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<const char*>(entry->Name),name))return reinterpret_cast<void**>(&values[i].u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** address;void* original;
    Patch(void** a,void* replacement):address(a),original(*a) {
        DWORD old;VirtualProtect(address,sizeof(void*),PAGE_READWRITE,&old);*address=replacement;
        DWORD ignored;VirtualProtect(address,sizeof(void*),old,&ignored);
    }
    ~Patch() {
        DWORD old;VirtualProtect(address,sizeof(void*),PAGE_READWRITE,&old);*address=original;
        DWORD ignored;VirtualProtect(address,sizeof(void*),old,&ignored);
    }
};
static void (__cdecl* realFree)(void*);
static void* (__cdecl* realMalloc)(size_t);
static void* (WINAPI* realTls)(DWORD);
static void* watchedFree;
static int watchedFrees;
static void __cdecl freeSpy(void* p) {if(p && p==watchedFree){++watchedFrees;watchedFree=nullptr;}realFree(p);}
static int mallocMode;
static size_t mallocSize=176;
static CWnd* allocationWindow;
static HWND allocationHandle;
static int allocationObservations;
static AFX_MODULE_THREAD_STATE* mallocState;
static AFX_MODULE_THREAD_STATE* switchedState;
static _AFX_THREAD_STATE* switchedThread;
static AFX_MODULE_STATE* switchedModule;
static void* __cdecl mallocSpy(size_t bytes) {
    if(allocationWindow){++allocationObservations;check(allocationWindow->m_hWnd==allocationHandle,"Attach publishes its HWND before map allocation callbacks");}
    int mode=bytes==mallocSize ? mallocMode:0;
    if(mode)mallocMode=0;
    if(mode==1)throw 41;
    if(mode==2){mallocState->m_pfnNewHandler=nullptr;return nullptr;}
    if(mode==3){if(switchedThread)switchedThread->m_pModuleState=switchedModule;else selected=switchedState;}
    return realMalloc(bytes);
}
static void* WINAPI tlsSpy(DWORD index) {mutateHandle();return realTls(index);}
struct Imports {
    std::vector<std::unique_ptr<Patch>> patches;
    explicit Imports(HMODULE image,bool originals) {
        auto** f=imported(image,"free");auto** m=imported(image,"malloc");
        check(f && m,"locate CRT imports for window lifetime observation");
        if(!f || !m)std::abort();
        if(originals){realFree=reinterpret_cast<decltype(realFree)>(*f);realMalloc=reinterpret_cast<decltype(realMalloc)>(*m);}
        patches.emplace_back(new Patch(f,reinterpret_cast<void*>(&freeSpy)));
        patches.emplace_back(new Patch(m,reinterpret_cast<void*>(&mallocSpy)));
        if(!originals) {
            auto** tls=imported(image,"TlsGetValue");check(tls!=nullptr,"locate native TLS lookup for reentrant handle observation");
            if(tls){realTls=reinterpret_cast<decltype(realTls)>(*tls);patches.emplace_back(new Patch(tls,reinterpret_cast<void*>(&tlsSpy)));}
        }
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
static void destructors(const API& api) {
    Context context(api);Object object(api);activeWindow=object.window;
    void* table[32]{};table[1]=reinterpret_cast<void*>(&deleting);table[7]=reinterpret_cast<void*>(&disconnect);
    Node container{table,1},control{table,2},layout{table,3},site{table,4};
    field<CWnd*>(&site,0x50)=object.window;
    object.window->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
    object.window->m_pMFCCtrlContainer=reinterpret_cast<CMFCControlContainer*>(&control);
    object.window->m_pDynamicLayout=reinterpret_cast<CMFCDynamicLayout*>(&layout);
    object.window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);
    object.window->m_xDispatch=reinterpret_cast<UINT_PTR>(table);
    object.window->m_pCurrentGestureInfo=static_cast<GESTUREINFO*>(std::malloc(56));
    watchedFree=object.window->m_pCurrentGestureInfo;watchedFrees=0;
    BYTE expected[232];std::memcpy(expected,object.window,sizeof(expected));
    field<void*>(expected,0)=object.root;field<void*>(expected,56)=nullptr;field<void*>(expected,152)=nullptr;
    *reinterpret_cast<void**>(object.window)=reinterpret_cast<void*>(UINT_PTR(0x778899));
    expectedRoot=object.root;expectedModule=object.window->m_pModuleState;events.clear();failureAt=0;
    api.destroy(object.window);object.destroyed=true;
    check(events==std::vector<int>({1,2,3,12}) && watchedFrees==1 && !field<void*>(&site,0x50),
        "destructor deletes native owned members, unlinks the site and frees gesture storage before the base");
    check(!std::memcmp(expected,object.window,sizeof(expected)),
        "qualified destructor replaces a foreign root and preserves native dangling-member/padding bytes");
}
static void crossLifetime(const API& construct,const API& destroy) {
    Object object(construct);Object reference(destroy);
    BYTE expected[232];std::memcpy(expected,object.window,sizeof(expected));
    field<void*>(expected,0)=reference.root;field<void*>(expected,56)=nullptr;
    destroy.destroy(object.window);object.destroyed=true;
    check(!std::memcmp(expected,object.window,sizeof(expected)),
        "window construction and qualified destruction interoperate across DLLs without rewriting embedded interfaces");
}
static void allocation(const API& api) {
    for(int mode:{1,2}) {
        Context context(api);auto previous=context.state->m_pfnNewHandler;
        mallocMode=mode;mallocSize=176;mallocState=context.state;bool threw=false;
        try{check(api.from(nullptr)==nullptr,"a NULL map allocation still permits FromHandle(NULL)");}
        catch(int value){check(value==41,"allocation exception propagates unchanged");threw=true;}
        check(!mallocMode && !context.state->m_pmapHWND && threw==(mode==1),"factory failure leaves no published HWND map");
        check(mode==1 ? context.state->m_pfnNewHandler==api.pressure
                      : context.state->m_pfnNewHandler==previous,
            "factory restores the old handler on normal NULL return and leaves pressure handling installed on exception");
    }
}
static void allocationSwitch(const API& api) {
    Context context(api);
    alignas(8) BYTE otherStorage[sizeof(AFX_MODULE_THREAD_STATE)]{},moduleStorage[256]{};
    auto* original=context.state;
    _AFX_THREAD_STATE* thread=api.thread ? api.thread():nullptr;
    AFX_MODULE_STATE* previousModule=thread ? thread->m_pModuleState:nullptr;
    if(thread) {
        thread->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(moduleStorage);
        switchedState=api.state();thread->m_pModuleState=previousModule;
    } else switchedState=reinterpret_cast<AFX_MODULE_THREAD_STATE*>(otherStorage);
    auto previousOther=switchedState->m_pfnNewHandler;
    original->m_pfnNewHandler=TestAllocationFailure;switchedState->m_pfnNewHandler=nullptr;
    switchedThread=thread;switchedModule=reinterpret_cast<AFX_MODULE_STATE*>(moduleStorage);
    mallocMode=3;mallocSize=176;
    check(!api.from(nullptr) && original->m_pmapHWND && !switchedState->m_pmapHWND &&
        switchedState->m_pfnNewHandler==TestAllocationFailure && original->m_pfnNewHandler!=TestAllocationFailure,
        "map factory publishes into the original module and restores the handler through a fresh module lookup");
    if(thread)thread->m_pModuleState=previousModule;else selected=original;
    switchedState->m_pfnNewHandler=previousOther;
    if(thread)api.localDestroy(moduleStorage+0xf8);
    switchedThread=nullptr;switchedState=nullptr;switchedModule=nullptr;
}
static void d2dLifetime(const API& api) {
    Object object(api);activeWindow=object.window;Node target{};void* table[2]{};table[1]=reinterpret_cast<void*>(&deleting);
    target.vptr=table;target.id=4;object.window->m_pRenderTarget=reinterpret_cast<CHwndRenderTarget*>(&target);
    events.clear();failureAt=0;api.enableD2D(object.window,FALSE,FALSE);
    check(events==std::vector<int>{4} && !object.window->m_pRenderTarget,
        "disabling Direct2D uses the native deleting destructor and clears the field after the callback");
}
static BYTE* observedConstruction;
static bool failModule;
static int moduleReads;
static AFX_MODULE_STATE* observeModule() {
    ++moduleReads;
    bool untouched=true;for(size_t i=0;i<264;++i)untouched=untouched && observedConstruction[i]==0xa5;
    check(untouched,"CWnd module lookup precedes root/interface/field writes");
    if(failModule)throw 41;
    return g_cmdTestCurrentModule;
}
static void constructionFailure(const API& source) {
    alignas(8) BYTE bytes[264];std::memset(bytes,0xa5,sizeof(bytes));observedConstruction=bytes;moduleReads=0;
    g_windowModuleStateHook=observeModule;failModule=true;bool threw=false;
    try{source.constructHandle(bytes+16,reinterpret_cast<HWND>(UINT_PTR(0x1122)));}catch(int n){threw=n==41;}
    g_windowModuleStateHook=nullptr;failModule=false;
    bool untouched=true;for(BYTE byte:bytes)untouched=untouched && byte==0xa5;
    check(threw && moduleReads==1 && untouched,"failed CWnd base construction preserves all caller storage");
    moduleReads=0;g_windowModuleStateHook=observeModule;
    source.construct(bytes+16);g_windowModuleStateHook=nullptr;
    check(moduleReads==1,"CWnd construction captures module state once");
    source.destroy(reinterpret_cast<CWnd*>(bytes+16));
}
static void handleMutation(const API& api) {
    Context context(api);api.from(nullptr);Object object(api);
    HWND first=reinterpret_cast<HWND>(UINT_PTR(0x1122)),second=reinterpret_cast<HWND>(UINT_PTR(0x3344));
    api.attach(object.window,first);api.attach(object.window,second);object.window->m_hWnd=first;
    mutateWindow=object.window;mutatedHandle=second;mutatedSite=nullptr;
    check(api.detach(object.window)==first && !mutateWindow && !api.permanent(second) && api.permanent(first)==object.window,
        "Detach snapshots its return handle before lookup and removes the freshly reloaded HWND");
    context.state->m_pmapHWND->m_permanentMap.RemoveKey(first);
    object.window->m_hWnd=first;
    mutateWindow=object.window;mutatedHandle=nullptr;mutatedSite=nullptr;
    check(!api.destroyWindow(object.window) && !mutateWindow && !object.window->m_hWnd,
        "DestroyWindow rechecks HWND/site after module lookup before delegating destruction");
    bool caught=false;allocationWindow=object.window;allocationHandle=first;allocationObservations=0;
    mallocSize=136;mallocMode=1;
    // RemoveAll drops the old hash allocation so insertion must allocate it.
    context.state->m_pmapHWND->m_permanentMap.RemoveAll();
    try{api.attach(object.window,first);}catch(int n){caught=n==41;}
    allocationWindow=nullptr;mallocMode=0;mallocSize=176;
    check(caught && allocationObservations && object.window->m_hWnd==first && !api.permanent(first),
        "failed permanent insertion leaves the HWND published without inventing a mapping rollback");
    object.window->m_hWnd=nullptr;
}
static void threadIsolation(const API& api) {
    std::atomic<int> ready{0};std::atomic<bool> finish{false};
    CWnd* temporary[2]{};CWnd* permanent[2]{};bool valid[2]{};
    std::thread workers[2];
    for(int i=0;i<2;++i)workers[i]=std::thread([&,i] {
        alignas(8) BYTE module[256]{};g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(module);
        Context context(api);Object object(api);
        auto h=reinterpret_cast<HWND>(UINT_PTR(0x432112345678ull));
        temporary[i]=api.from(h);api.attach(object.window,h);permanent[i]=api.permanent(h);
        valid[i]=temporary[i] && temporary[i]!=object.window && permanent[i]==object.window && api.from(h)==object.window;
        ++ready;while(!finish.load())std::this_thread::yield();
        api.detach(object.window);g_cmdTestCurrentModule=nullptr;
    });
    while(ready.load()!=2)std::this_thread::yield();
    check(valid[0] && valid[1] && temporary[0]!=temporary[1] && permanent[0]!=permanent[1],
        "the same HWND has independent temporary and permanent wrappers on two UI threads");
    finish=true;for(auto& worker:workers)worker.join();
}
static void moduleIsolation(const API& api) {
    Context first(api);auto h=reinterpret_cast<HWND>(UINT_PTR(0x432112345678ull));
    CWnd* original=api.from(h);
    alignas(8) BYTE otherModule[256]{};
    auto* thread=api.thread ? api.thread():nullptr;
    auto* previous=thread ? thread->m_pModuleState:g_cmdTestCurrentModule;
    if(thread)thread->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(otherModule);
    else g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(otherModule);
    {
        Context second(api);CWnd* other=api.from(h);
        check(original!=other && other && api.from(h)==other,"two module states keep distinct HWND map identities on one thread");
    }
    if(thread)thread->m_pModuleState=previous;else g_cmdTestCurrentModule=previous;
    check(api.from(h)==original,"module switching restores the original temporary identity");
    if(thread)api.localDestroy(otherModule+0xf8);
}
static void crossMaps(const API& source,const API& native) {
    // The native and source maps have compatible live allocator/map members,
    // so either implementation can consume a map constructed by the other.
    for(bool nativeOwns:{false,true}) {
        Context sourceContext(source);Context nativeContext(native);
        auto h=reinterpret_cast<HWND>(UINT_PTR(0x432112345678ull));
        auto* owner=nativeOwns ? nativeContext.state:sourceContext.state;
        auto* borrower=nativeOwns ? sourceContext.state:nativeContext.state;
        if(nativeOwns)native.from(nullptr);
        else {selected=sourceContext.state;source.from(nullptr);}
        borrower->m_pmapHWND=owner->m_pmapHWND;
        selected=sourceContext.state;
        auto* first=source.from(h);auto* second=native.from(h);
        check(first && first==second,"native and source FromHandle share a map constructed by either DLL");
        Object object(source);source.attach(object.window,h);
        check(native.permanent(h)==object.window && native.from(h)==object.window,
            "native lookup recognizes a source-attached permanent object in either map");
        native.detach(object.window);native.clear();
        check(!owner->m_pmapHWND->m_temporaryMap.GetCount(),"native temporary cleanup invokes either DLL's construction callbacks safely");
        borrower->m_pmapHWND=nullptr;
    }
}
[[noreturn]] static void terminateChild(const API& api) {
    Object object(api);activeWindow=object.window;expectedRoot=object.root;expectedModule=object.window->m_pModuleState;
    void* table[8]{};table[1]=reinterpret_cast<void*>(&deleting);table[7]=reinterpret_cast<void*>(&disconnect);
    Node container{table,1};object.window->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
    object.window->m_xDispatch=reinterpret_cast<UINT_PTR>(table);events.clear();failureAt=1;
    std::set_terminate([] {
        bool intact=!failures && events==std::vector<int>{1} && activeWindow->m_pModuleState==expectedModule &&
            *reinterpret_cast<void**>(activeWindow)==expectedRoot && activeWindow->m_pCtrlCont;
        ExitProcess(intact ? 86:87);
    });
    try{api.destroy(object.window);}catch(...){ExitProcess(88);}
    ExitProcess(89);
}
static void terminationCheck(const wchar_t* argument) {
    wchar_t path[1024],command[1200];GetModuleFileNameW(nullptr,path,1024);
    _snwprintf_s(command,_countof(command),_TRUNCATE,L"\"%ls\" %ls",path,argument);
    STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION child{};
    bool created=CreateProcessW(nullptr,command,nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&child)!=FALSE;
    check(created,"launch window destructor termination regression");if(!created)return;
    DWORD wait=WaitForSingleObject(child.hProcess,30000),code=0;
    if(wait!=WAIT_OBJECT_0)TerminateProcess(child.hProcess,90);GetExitCodeProcess(child.hProcess,&code);
    check(wait==WAIT_OBJECT_0 && code==86,"window destruction terminates before base cleanup when an owned destructor throws");
    CloseHandle(child.hThread);CloseHandle(child.hProcess);
}
static bool invoke(void (AFXAPI* action)(CWnd*),CWnd* self) {
    try{action(self);return true;}catch(int n){check(n==41,"foreign callback exception propagates unchanged");return false;}
}
static void messages(const API& api) {
    Context context(api);
    void* table[32]{};table[1]=reinterpret_cast<void*>(&deleting);table[2]=reinterpret_cast<void*>(&releasing);
    table[3]=reinterpret_cast<void*>(&server);table[22]=reinterpret_cast<void*>(&revoke);table[24]=reinterpret_cast<void*>(&destroySite);
    for(int failure:{0,1,2,3,4,7}) {
        Object object(api);activeWindow=object.window;void* root[91];std::memcpy(root,object.root,sizeof(root));
        root[73]=reinterpret_cast<void*>(&defaultProc);*reinterpret_cast<void***>(object.window)=root;
        Node container{table,1},proxy{table,2},standard{table,3},render{table,4};
        object.window->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
        object.window->m_pProxy=reinterpret_cast<IAccessibleProxy*>(&proxy);
        object.window->m_pStdObject=reinterpret_cast<IAccessible*>(&standard);
        object.window->m_pRenderTarget=reinterpret_cast<CHwndRenderTarget*>(&render);
        object.window->m_bIsTouchWindowRegistered=TRUE;
        events.clear();failureAt=failure;bool succeeded=invoke(api.onDestroy,object.window);failureAt=0;
        std::vector<int> expected{1,2,3,4,7};
        if(failure){expected.resize(std::find(expected.begin(),expected.end(),failure)-expected.begin()+1);}
        check(events==expected && succeeded==!failure,"WM_DESTROY preserves callback order and exception boundaries");
        check((object.window->m_pCtrlCont==nullptr)==(failure!=1) &&
            (object.window->m_pProxy==nullptr)==(!failure || failure>2) &&
            (object.window->m_pStdObject==nullptr)==(!failure || failure>3) &&
            (object.window->m_pRenderTarget==nullptr)==(!failure || failure>4),
            "WM_DESTROY clears each field only after its callback returns");
        check((object.window->m_bIsTouchWindowRegistered==FALSE)==(!failure || failure>3),
            "WM_DESTROY unregisters touch after accessibility and before render-target deletion");
    }
    Object object(api);activeWindow=object.window;void* root[91];std::memcpy(root,object.root,sizeof(root));
    root[65]=reinterpret_cast<void*>(&superProc);root[73]=reinterpret_cast<void*>(&defaultProc);root[74]=reinterpret_cast<void*>(&post);
    *reinterpret_cast<void***>(object.window)=root;
    Node drop{table,5},container{table,6},site{table,11};
    alignas(8) BYTE thread[128]{};field<CWnd*>(thread,0x40)=object.window;field<CWnd*>(thread,0x48)=object.window;
    field<void*>(thread,0x78)=reinterpret_cast<void*>(&oleTerm);
    context.state->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(thread);
    auto* module=api.module();BYTE oldDll=field<BYTE>(module,0x28);field<BYTE>(module,0x28)=0;
    auto* oldApp=field<void*>(module,8);field<void*>(module,8)=nullptr;
    object.window->m_pDropTarget=reinterpret_cast<COleDropTarget*>(&drop);
    object.window->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
    object.window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);
    superProcedure=nullptr;events.clear();failureAt=0;
    check(invoke(api.onNcDestroy,object.window),"WM_NCDESTROY completes cleanup");
    check(events==std::vector<int>({10,5,6,7,8,9}) && !field<void*>(thread,0x40) && !field<void*>(thread,0x48) &&
        !object.window->m_pDropTarget && !object.window->m_pCtrlCont,"WM_NCDESTROY clears thread owners, revokes drop services and then detaches");
    MSG quit{};check(PeekMessageW(&quit,nullptr,WM_QUIT,WM_QUIT,PM_REMOVE) && quit.wParam==0,"main-window destruction posts WM_QUIT");
    field<BYTE>(module,0x28)=oldDll;field<void*>(module,8)=oldApp;
}
static void threadWindowOwners(const API& api) {
    Context context(api);auto* module=api.module();
    BYTE oldDll=field<BYTE>(module,0x28);DWORD oldTerm=field<DWORD>(module,0x60);void* oldApp=field<void*>(module,8);
    for(bool dll:{false,true})for(bool sameApp:{false,true})for(bool terminating:{false,true})
      for(bool main:{false,true})for(bool active:{false,true}) {
        Object object(api);void* root[91];std::memcpy(root,object.root,sizeof(root));
        root[65]=reinterpret_cast<void*>(&superProc);root[73]=reinterpret_cast<void*>(&defaultProc);root[74]=reinterpret_cast<void*>(&post);
        *reinterpret_cast<void***>(object.window)=root;
        alignas(8) BYTE thread[128]{};
        field<CWnd*>(thread,0x40)=main ? object.window:reinterpret_cast<CWnd*>(UINT_PTR(1));
        field<CWnd*>(thread,0x48)=active ? object.window:reinterpret_cast<CWnd*>(UINT_PTR(2));
        field<void*>(thread,0x78)=reinterpret_cast<void*>(&oleTerm);
        context.state->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(thread);
        field<BYTE>(module,0x28)=dll ? 1:0;field<DWORD>(module,0x60)=terminating ? 69:0;
        field<void*>(module,8)=sameApp ? thread:nullptr;events.clear();superProcedure=nullptr;
        api.onNcDestroy(object.window);
        bool expectedQuit=main && !dll && (!sameApp || !terminating);
        MSG quit{};bool gotQuit=PeekMessageW(&quit,nullptr,WM_QUIT,WM_QUIT,PM_REMOVE)!=FALSE;
        std::vector<int> expected=expectedQuit ? std::vector<int>{10,7,8,9}:std::vector<int>{7,8,9};
        check(gotQuit==expectedQuit && events==expected &&
            field<CWnd*>(thread,0x40)==(main ? nullptr:reinterpret_cast<CWnd*>(UINT_PTR(1))) &&
            field<CWnd*>(thread,0x48)==(active ? nullptr:reinterpret_cast<CWnd*>(UINT_PTR(2))),
            "WM_NCDESTROY preserves native DLL/application/termination gates and clears only matching thread windows");
    }
    context.state->m_pCurrentWinThread=nullptr;
    field<BYTE>(module,0x28)=oldDll;field<DWORD>(module,0x60)=oldTerm;field<void*>(module,8)=oldApp;
}
static HWND tooltipOwner;
static int tooltipDeletions;
static LRESULT CALLBACK lifetimeProcedure(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam) {
    if(message==0x433) {
        ++tooltipDeletions;TOOLINFOW expected{};expected.cbSize=64;expected.uFlags=TTF_IDISHWND;
        expected.hwnd=tooltipOwner;expected.uId=reinterpret_cast<UINT_PTR>(tooltipOwner);
        check(wparam==0 && lparam && !std::memcmp(&expected,reinterpret_cast<void*>(lparam),sizeof(expected)),
            "tooltip removal uses the native 64-byte cbSize and zeroed 72-byte TOOLINFO with HWND identity");
        return 69;
    }
    return DefWindowProcW(hwnd,message,wparam,lparam);
}
static LRESULT CALLBACK restoredProcedure(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam) {
    return DefWindowProcW(hwnd,message,wparam,lparam);
}
static LRESULT CALLBACK changedProcedure(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam) {
    return DefWindowProcW(hwnd,message,wparam,lparam);
}
static void nonclientProcedure(const API& api) {
    WNDCLASSW cls{};cls.lpfnWndProc=lifetimeProcedure;cls.hInstance=GetModuleHandleW(nullptr);cls.lpszClassName=L"OpenMFC_Lifetime_Procedure_Test";
    check(RegisterClassW(&cls)!=0,"register nonclient destruction observation class");
    for(bool changed:{false,true}) {
        Context context(api);Object object(api);
        HWND target=CreateWindowExW(0,cls.lpszClassName,L"",WS_POPUP,0,0,10,10,nullptr,nullptr,cls.hInstance,nullptr);
        HWND tooltip=CreateWindowExW(0,cls.lpszClassName,L"",WS_POPUP,0,0,10,10,nullptr,nullptr,cls.hInstance,nullptr);
        check(target && tooltip,"create real HWNDs for nonclient procedure and tooltip cleanup");
        alignas(8) BYTE tip[232]{};field<HWND>(tip,64)=tooltip;
        context.state->m_pToolTip=reinterpret_cast<CToolTipCtrl*>(tip);
        api.attach(object.window,target);object.window->m_nFlags=1;
        void* root[91];std::memcpy(root,object.root,sizeof(root));root[65]=reinterpret_cast<void*>(&superProc);
        root[73]=reinterpret_cast<void*>(&defaultProc);root[74]=reinterpret_cast<void*>(&post);*reinterpret_cast<void***>(object.window)=root;
        tooltipOwner=target;tooltipDeletions=0;superProcedure=restoredProcedure;
        defaultReplacement=changed ? changedProcedure:nullptr;events.clear();api.onNcDestroy(object.window);defaultReplacement=nullptr;
        check(tooltipDeletions==1 && events==(changed ? std::vector<int>{7,9}:std::vector<int>{7,8,9}) &&
            GetWindowLongPtrW(target,GWLP_WNDPROC)==reinterpret_cast<LONG_PTR>(changed ? changedProcedure:restoredProcedure),
            "WM_NCDESTROY restores the superclass procedure only when Default left the current procedure unchanged");
        check(!api.permanent(target) && !object.window->m_hWnd,"nonclient cleanup removes the permanent window identity");
        context.state->m_pToolTip=nullptr;DestroyWindow(target);DestroyWindow(tooltip);
    }
    UnregisterClassW(cls.lpszClassName,cls.hInstance);
}
static void postNoop(const API& api) {
    Object object(api,reinterpret_cast<HWND>(UINT_PTR(0x7788)));void* root[91];std::memcpy(root,object.root,sizeof(root));
    root[74]=reinterpret_cast<void*>(&post);*reinterpret_cast<void***>(object.window)=root;
    object.window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(UINT_PTR(0x1122));
    BYTE before[232];std::memcpy(before,object.window,sizeof(before));events.clear();api.post(object.window);
    check(events.empty() && !std::memcmp(before,object.window,sizeof(before)),
        "exported base PostNcDestroy is a qualified no-op, including on a still-attached derived object");
}
static void destroyWindows(const API& api) {
    Context context(api);Object object(api);
    check(!api.destroyWindow(object.window),"DestroyWindow returns FALSE for an unattached window");
    void* table[25]{};table[24]=reinterpret_cast<void*>(&destroySite);Node site{table,11};
    object.window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);events.clear();failureAt=0;
    check(api.destroyWindow(object.window)==69 && events==std::vector<int>{11} && object.window->m_pCtrlSite,
        "control-site destruction preserves the raw BOOL result and site ownership");
    object.window->m_pCtrlSite=nullptr;
    HWND window=CreateWindowExW(0,L"STATIC",L"permanent destruction",WS_POPUP,0,0,20,20,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    api.attach(object.window,window);
    check(api.destroyWindow(object.window) && !IsWindow(window) && object.window->m_hWnd==window && api.permanent(window)==object.window,
        "DestroyWindow delegates HWND clearing and map removal to destruction messages");
    api.detach(object.window);
}
int main(int argc,char** argv) {
    static_assert(!std::is_constructible<CWnd,HWND>::value,"HWND construction remains private, matching the SDK");
    std::setvbuf(stdout,nullptr,_IONBF,0);CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    alignas(8) BYTE sourceModule[256]{};g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(sourceModule);
    API source{"source",&impl___0CWnd__QEAA_XZ,&impl___0CWnd__AEAA_PEAUHWND_____Z,&impl___1CWnd__UEAA_XZ,
        &impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z,&impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z,
        &impl__Attach_CWnd__QEAAHPEAUHWND_____Z,&impl__Detach_CWnd__QEAAPEAUHWND____XZ,&impl__DeleteTempMap_CWnd__SAXXZ,
        &impl__DestroyWindow_CWnd__UEAAHXZ,&impl__OnDestroy_CWnd__IEAAXXZ,&impl__OnNcDestroy_CWnd__IEAAXXZ,
        &impl__PostNcDestroy_CWnd__MEAAXXZ,&impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,
        &impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ,nullptr,nullptr,nullptr,nullptr,
        &impl__EnableD2DSupport_CWnd__QEAAXHH_Z,&openmfc::detail::modulethreadstate::HandleAllocationFailure};
    if(argc>1 && !std::strcmp(argv[1],"--terminate-source"))terminateChild(source);
    Imports imports(GetModuleHandleW(nullptr),true);
    specialWindows({&impl__wndBottom_CWnd__2V1_B,&impl__wndNoTopMost_CWnd__2V1_B,&impl__wndTop_CWnd__2V1_B,&impl__wndTopMost_CWnd__2V1_B});
    constructors(source);constructionFailure(source);maps(source);destroyWindows(source);messages(source);threadWindowOwners(source);nonclientProcedure(source);postNoop(source);destructors(source);handleMutation(source);
    crossLifetime(source,source);allocation(source);allocationSwitch(source);d2dLifetime(source);
    moduleIsolation(source);threadIsolation(source);terminationCheck(L"--terminate-source");
    if(const char* path=std::getenv("OPENMFC_NATIVE_WINDOW_DLL")) {
        HMODULE dll=LoadLibraryA(path);check(dll!=nullptr,"load the native window comparison DLL");
        if(dll) {
            auto* bytes=reinterpret_cast<BYTE*>(dll);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(bytes+reinterpret_cast<IMAGE_DOS_HEADER*>(bytes)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"native window oracle matches the verified MFC build");
            if(pinned) {
                Imports nativeImports(dll,false);
                API native=source;native.name="native";
                specialWindows({reinterpret_cast<CWnd*>(bytes+0x3c3460),reinterpret_cast<CWnd*>(bytes+0x3c3640),
                    reinterpret_cast<CWnd*>(bytes+0x3c3370),reinterpret_cast<CWnd*>(bytes+0x3c3550)});
#define RVA(name,offset) native.name=reinterpret_cast<decltype(native.name)>(bytes+offset)
                RVA(construct,0x28a700);RVA(constructHandle,0x28a730);RVA(destroy,0x28b740);
                RVA(from,0x28ad70);RVA(permanent,0x28adc0);RVA(attach,0x28adf0);RVA(detach,0x28ae50);RVA(clear,0x2a6210);
                RVA(destroyWindow,0x28baf0);RVA(onDestroy,0x28b840);RVA(onNcDestroy,0x28b900);
                RVA(state,0x133a20);RVA(module,0x133930);RVA(stateConstruct,0x133580);RVA(stateDestroy,0x1336b0);
                RVA(thread,0x1332a0);RVA(localDestroy,0x14d0d0);RVA(enableD2D,0x291f30);
                RVA(pressure,0x2ae6b0);
                RVA(post,0x27d0);
#undef RVA
                if(argc>1 && !std::strcmp(argv[1],"--terminate-native"))terminateChild(native);
                constructors(native);maps(native);destroyWindows(native);messages(native);threadWindowOwners(native);nonclientProcedure(native);postNoop(native);destructors(native);handleMutation(native);
                crossLifetime(source,native);crossLifetime(native,source);
                allocation(native);allocationSwitch(native);d2dLifetime(native);terminationCheck(L"--terminate-native");
                moduleIsolation(native);threadIsolation(native);crossMaps(source,native);
            }
            FreeLibrary(dll);
        }
    }
    g_cmdTestCurrentModule=nullptr;CoUninitialize();
    std::printf("Window handle/lifetime: %d failures\n",failures);return failures?1:0;
}
