// Release-x64 fixed allocator ABI, exception and contention comparisons.
#include "detail/CFixedAllocSupport.h"
#include "openmfc/afxwin.h"
#include "fixed_alloc_test_runtime.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <initializer_list>
extern "C" {
CFixedAllocSynchronizedState* MS_ABI impl___0CFixedAlloc__QEAA_II_Z(CFixedAllocSynchronizedState*,UINT,UINT);
void MS_ABI impl___1CFixedAlloc__QEAA_XZ(CFixedAllocSynchronizedState*);
void* MS_ABI impl__Alloc_CFixedAlloc__QEAAPEAXXZ(CFixedAllocSynchronizedState*);
void MS_ABI impl__Free_CFixedAlloc__QEAAXPEAX_Z(CFixedAllocSynchronizedState*,void*);
void MS_ABI impl__FreeAll_CFixedAlloc__QEAAXXZ(CFixedAllocSynchronizedState*);
CFixedAllocState* MS_ABI impl___0CFixedAllocNoSync__QEAA_II_Z(CFixedAllocState*,UINT,UINT);
void MS_ABI impl___1CFixedAllocNoSync__QEAA_XZ(CFixedAllocState*);
void* MS_ABI impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ(CFixedAllocState*);
void MS_ABI impl__Free_CFixedAllocNoSync__QEAAXPEAX_Z(CFixedAllocState*,void*);
void MS_ABI impl__FreeAll_CFixedAllocNoSync__QEAAXXZ(CFixedAllocState*);
CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex**,UINT64,UINT64);
void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex*);
}
namespace {
int failures;
void check(bool value,const char* message){printf("%s: %s\n",value?"PASS":"FAIL",message);if(!value)++failures;}
struct API {
    void* (*construct)(void*,UINT,UINT);
    void (*destroy)(void*);
    void* (*allocate)(void*);
    void (*release)(void*,void*);
    void (*clear)(void*);
};
struct Collections {
    API sync,plain;
    decltype(&impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z) plex;
    decltype(&impl__FreeDataChain_CPlex__QEAAXXZ) chain;
};
struct Event {INT64 v[8];};
struct Trace {Event records[32768];size_t count=0;};
Trace localTrace,nativeTrace;Trace* trace;
alignas(16) BYTE storage[72],arena[65536];size_t used;
CFixedAllocState* state=reinterpret_cast<CFixedAllocState*>(storage);
bool capture,pointerFailure,structuredFailure;alignas(8) BYTE frameworkError[24]{};int failure,mutation,freeCalls;
using Malloc=void* (__cdecl*)(size_t);using Free=void (__cdecl*)(void*);
Malloc realMalloc;Free realFree;
decltype(&::InitializeCriticalSection) realInitialize;
decltype(&::DeleteCriticalSection) realDelete;
decltype(&::EnterCriticalSection) realEnter;
decltype(&::LeaveCriticalSection) realLeave;
INT64 id(const void* p) {
    if(!p)return 0;auto n=reinterpret_cast<uintptr_t>(p),begin=reinterpret_cast<uintptr_t>(arena);
    return n>=begin && n<begin+sizeof(arena)?INT64(n-begin)+0x10000:INT64(n);
}
void event(INT64 a,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0,INT64 h=0) {
    if(trace->count>=32768)std::abort();trace->records[trace->count++]={{a,b,c,d,e,f,g,h}};if(failure==a){if(structuredFailure)RaiseException(0xe0424242,0,0,nullptr);if(pointerFailure)throw reinterpret_cast<CException*>(frameworkError);throw 91;}
}
void reset(Trace& out) {
    trace=&out;capture=true;pointerFailure=structuredFailure=false;used=0;failure=mutation=freeCalls=0;
    memset(storage,0xcd,sizeof(storage));memset(arena,0xa5,sizeof(arena));
}
void snapshot(INT64 result=0,int caught=0) {
    event(10,result,caught,state->m_nAllocSize,state->m_nBlockSize,id(state->m_pBlocks),id(state->m_pNodeFree));
    UINT64 guard=0;memcpy(&guard,storage+64,8);event(11,INT64(guard),storage[24],storage[63]);
}
void* __cdecl allocate(size_t bytes) {
    if(!capture)return realMalloc(bytes);
    event(1,INT64(bytes));size_t offset=used;used+=4096;if(used>sizeof(arena))std::abort();
    if(mutation==1){state->m_nAllocSize=9;state->m_nBlockSize=3;}
    if(mutation==2){auto* previous=reinterpret_cast<CPlex*>(arena+61440);previous->pNext=nullptr;state->m_pBlocks=previous;}
    return arena+offset;
}
void __cdecl release(void* p) {
    if(!capture){realFree(p);return;}
    event(2,id(p),id(state->m_pBlocks),id(state->m_pNodeFree));++freeCalls;
    if(mutation==3 && p)static_cast<CPlex*>(p)->pNext=nullptr;
    if(mutation==4)state->m_pNodeFree=nullptr;
}
void WINAPI initialize(CRITICAL_SECTION* section) {
    if(!capture){realInitialize(section);return;}event(3,reinterpret_cast<BYTE*>(section)-storage);memset(section,0x6a,sizeof(*section));
}
void WINAPI destroy(CRITICAL_SECTION* section) {
    if(!capture){realDelete(section);return;}event(4,reinterpret_cast<BYTE*>(section)-storage);
}
void WINAPI enter(CRITICAL_SECTION* section) {
    if(!capture){realEnter(section);return;}event(5,reinterpret_cast<BYTE*>(section)-storage,id(state->m_pNodeFree));
    if(mutation==5){state->m_nAllocSize=8;state->m_nBlockSize=2;}
}
void WINAPI leave(CRITICAL_SECTION* section) {
    if(!capture){realLeave(section);return;}event(6,reinterpret_cast<BYTE*>(section)-storage,id(state->m_pNodeFree));
}
void** import(HMODULE image,const char* name) {
    auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* desc=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;desc->Name;++desc) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->OriginalFirstThunk);auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->FirstThunk);
        for(;names->u1.AddressOfData;++names,++values)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* entry=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+names->u1.AddressOfData);
            if(!strcmp(reinterpret_cast<char*>(entry->Name),name))return reinterpret_cast<void**>(&values->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot;void* previous;DWORD protection;
    void set(void** s,void* value){slot=s;previous=*s;VirtualProtect(s,sizeof(*s),PAGE_READWRITE,&protection);*s=value;}
    void restore(){*slot=previous;DWORD ignored;VirtualProtect(slot,sizeof(*slot),protection,&ignored);}
};
struct Imports {
    Patch patches[6];int count=0;
    Imports(HMODULE image,bool save) {
#define HOOK(name,callback,original) {auto** s=import(image,name);check(s!=nullptr,"locate fixed allocator import " name);if(!s)std::abort();if(save)original=reinterpret_cast<decltype(original)>(*s);patches[count++].set(s,reinterpret_cast<void*>(&callback));}
        HOOK("malloc",allocate,realMalloc);HOOK("free",release,realFree);
        HOOK("InitializeCriticalSection",initialize,realInitialize);HOOK("DeleteCriticalSection",destroy,realDelete);
        HOOK("EnterCriticalSection",enter,realEnter);HOOK("LeaveCriticalSection",leave,realLeave);
#undef HOOK
    }
    ~Imports(){while(count)patches[--count].restore();}
};
template<class F>int invoke(F action) {try{action();return 0;}catch(int n){return n;}catch(CException* e){e->Delete();return 19;}}
void constructors(const Collections& api,Trace& out) {
    out.count=0;
    for(auto* kind:{&api.plain,&api.sync})for(UINT size:{0u,1u,7u,8u,9u,16u,UINT_MAX})for(UINT count:{0u,1u,2u,3u,64u,UINT_MAX})for(int at:{0,3}) {
        reset(out);failure=at;INT64 result=17;int caught=invoke([&]{result=kind->construct(storage,size,count)==storage;});snapshot(result,caught);
    }
    capture=false;
}
void sequences(const Collections& api,Trace& out) {
    out.count=0;
    for(auto* kind:{&api.plain,&api.sync})for(UINT size:{8u,9u,16u,37u})for(UINT count:{0u,1u,2u,3u,7u}) {
        reset(out);kind->construct(storage,size,count);void* values[12];
        for(int i=0;i<10;++i){values[i]=kind->allocate(storage);snapshot(id(values[i]));memset(values[i],i,state->m_nAllocSize);}
        kind->release(storage,values[1]);snapshot();kind->release(storage,values[4]);snapshot();
        snapshot(id(kind->allocate(storage)));snapshot(id(kind->allocate(storage)));kind->release(storage,nullptr);snapshot();
        kind->clear(storage);snapshot();snapshot(id(kind->allocate(storage)));kind->destroy(storage);snapshot();
    }
    capture=false;
}
void exceptions(const Collections& api,Trace& out) {
    out.count=0;
    for(auto* kind:{&api.plain,&api.sync})for(int operation:{0,1,2,3})for(int at:{0,1,2,4,5,6})for(int mode:{0,1,2,3,4,5})for(bool framework:{false,true}) {
        reset(out);kind->construct(storage,16,2);void* first=nullptr;
        for(int i=0;i<5;++i){void* value=kind->allocate(storage);if(!i)first=value;}
        failure=at;mutation=mode;pointerFailure=framework;
        if(operation==0)state->m_pNodeFree=nullptr;
        INT64 result=17;int caught=invoke([&]{switch(operation){case 0:result=id(kind->allocate(storage));break;case 1:kind->release(storage,first);result=0;break;case 2:kind->clear(storage);result=0;break;case 3:kind->destroy(storage);result=0;break;}});
        snapshot(result,caught);
    }
    capture=false;
}
void plexes(const Collections& api,Trace& out) {
    out.count=0;
    const UINT64 cases[][2]={{0,16},{4,0},{4,16},{1,1},{UINT64_MAX,2},{UINT64_MAX,8},{UINT64_MAX,UINT64_MAX},{1ULL<<63,2},{1ULL<<32,1ULL<<32}};
    for(auto& values:cases)for(int at:{0,1,2}) {
        reset(out);CPlex* head=nullptr;failure=at;INT64 result=17;
        int caught=invoke([&]{auto* p=api.plex(&head,values[0],values[1]);result=id(p);});
        event(12,result,caught,id(head));if(head){failure=0;api.chain(head);}
    }
    for(int at:{0,2})for(int mode:{0,3})for(bool framework:{false,true}) {
        reset(out);CPlex* head=nullptr;for(int i=0;i<3;++i)api.plex(&head,2,16);
        failure=at;mutation=mode;pointerFailure=framework;int caught=invoke([&]{api.chain(head);});event(12,caught);
    }
    capture=false;
}
int invokeStructured(const API* api,int operation,void* first) {
    __try {
        switch(operation){case 0:api->allocate(storage);break;case 1:api->release(storage,first);break;case 2:api->clear(storage);break;case 3:api->destroy(storage);break;}
        return 0;
    } __except(EXCEPTION_EXECUTE_HANDLER) {return GetExceptionCode();}
}
void structured(const Collections& api,Trace& out) {
    out.count=0;
    for(auto* kind:{&api.plain,&api.sync})for(int operation:{0,1,2,3})for(int at:{0,1,2,4,5,6}) {
        reset(out);kind->construct(storage,16,2);void* first=nullptr;
        for(int i=0;i<5;++i){void* value=kind->allocate(storage);if(!i)first=value;}
        if(operation==0)state->m_pNodeFree=nullptr;failure=at;structuredFailure=true;
        int caught=invokeStructured(kind,operation,first);snapshot(0,caught);
    }
    capture=false;
}
void compare(const Trace& expected,const Trace& actual,const char* message) {
    bool same=expected.count==actual.count;size_t i=0;
    for(;i<expected.count && i<actual.count;++i)if(memcmp(&expected.records[i],&actual.records[i],sizeof(Event))){same=false;break;}
    if(!same){printf("first mismatch at %zu / %zu local / %zu native\n",i,expected.count,actual.count);for(size_t j=i>2?i-2:0;j<i+4;++j){printf("%zu local:",j);if(j<expected.count)for(auto n:expected.records[j].v)printf(" %lld",n);printf(" native:");if(j<actual.count)for(auto n:actual.records[j].v)printf(" %lld",n);puts("");}}
    check(same,message);
}
void expectations(const Collections& api) {
    localTrace.count=0;reset(localTrace);auto* result=api.sync.construct(storage,1,0);
    check(result==storage && state->m_nAllocSize==8 && state->m_nBlockSize==64 && state->m_pBlocks==nullptr && state->m_pNodeFree==nullptr && localTrace.records[0].v[0]==3 && localTrace.records[0].v[1]==24,
        "synchronized constructor uses the native 24-byte prefix, 64-node default and embedded critical section");
    void* first=api.sync.allocate(storage);void* second=api.sync.allocate(storage);
    check(first==arena+8 && second==arena+16 && state->m_pBlocks==reinterpret_cast<CPlex*>(arena),"new plex nodes are handed out in ascending address order");
    size_t before=localTrace.count;api.sync.release(nullptr,nullptr);check(localTrace.count==before,"Free(nullptr) returns before accessing or locking the allocator");
    failure=1;state->m_pNodeFree=nullptr;int caught=invoke([&]{api.sync.allocate(storage);});failure=0;
    check(caught==91 && localTrace.records[localTrace.count-1].v[0]==1,"foreign allocation exceptions propagate outside the framework-specific unlock handler");
    failure=1;pointerFailure=true;caught=invoke([&]{api.sync.allocate(storage);});failure=0;
    check(caught==19 && localTrace.records[localTrace.count-1].v[0]==6,"framework allocation exceptions leave the critical section and rethrow without deleting the error");
    before=localTrace.count;api.sync.destroy(storage);check(localTrace.records[before].v[0]==4 && localTrace.records[before+1].v[0]==2,"allocator destruction deletes the critical section before freeing its plex chain");
    reset(localTrace);api.sync.construct(storage,16,2);api.sync.allocate(storage);
    failure=2;structuredFailure=true;caught=invokeStructured(&api.sync,2,nullptr);failure=0;
    check(caught==int(0xe0424242u) && localTrace.records[localTrace.count-1].v[0]==6,
        "FreeAll releases its critical section when chain cleanup raises a structured exception");
    capture=false;
}
struct Stress {
    API api;void* allocator;CRITICAL_SECTION guard;void* live[8]{};LONG errors=0;
};
struct Worker {Stress* stress;int index;};
DWORD WINAPI worker(void* p) {
    auto& w=*static_cast<Worker*>(p);auto& s=*w.stress;
    for(int n=0;n<3000;++n) {
        void* value=s.api.allocate(s.allocator);EnterCriticalSection(&s.guard);
        for(int i=0;i<8;++i)if(s.live[i]==value)InterlockedIncrement(&s.errors);
        s.live[w.index]=value;LeaveCriticalSection(&s.guard);
        memset(value,w.index+1,32);SwitchToThread();
        EnterCriticalSection(&s.guard);s.live[w.index]=nullptr;LeaveCriticalSection(&s.guard);s.api.release(s.allocator,value);
    }
    return 0;
}
void stress(const Collections& api) {
    capture=false;alignas(8) BYTE allocator[72];memset(allocator,0xcd,sizeof(allocator));api.sync.construct(allocator,32,3);
    Stress state{api.sync,allocator};InitializeCriticalSection(&state.guard);Worker workers[8];HANDLE threads[8];
    for(int i=0;i<8;++i){workers[i]={&state,i};threads[i]=CreateThread(nullptr,0,worker,&workers[i],0,nullptr);if(!threads[i])std::abort();}
    WaitForMultipleObjects(8,threads,TRUE,INFINITE);for(auto thread:threads)CloseHandle(thread);
    check(state.errors==0,"contended allocator never hands one live block to two of eight threads");
    api.sync.clear(allocator);void* p=api.sync.allocate(allocator);check(p!=nullptr,"synchronized allocator remains reusable after contention and FreeAll");
    api.sync.destroy(allocator);DeleteCriticalSection(&state.guard);UINT64 guard;memcpy(&guard,allocator+64,8);check(guard==0xcdcdcdcdcdcdcdcdULL,"synchronized operations preserve the native 64-byte object boundary");
}
void interoperability(const Collections& source,const Collections& native) {
    capture=false;
    for(bool sync:{false,true}) {
        auto& a=sync?source.sync:source.plain;auto& b=sync?native.sync:native.plain;alignas(8) BYTE object[72]{};
        a.construct(object,24,3);void* p=b.allocate(object);a.release(object,p);void* q=b.allocate(object);
        check(p==q,"native allocation reuses a node released through OpenMFC's ABI");
        b.release(object,q);p=a.allocate(object);check(p==q,"OpenMFC allocation reuses a node released through native MFC's ABI");b.destroy(object);
        b.construct(object,37,2);p=a.allocate(object);b.release(object,p);q=a.allocate(object);check(p==q,"native-created allocators accept OpenMFC growth and native release");a.destroy(object);
    }
}
}
int main() {
    setvbuf(stdout,nullptr,_IONBF,0);
    Collections source{
        {reinterpret_cast<decltype(API::construct)>(&impl___0CFixedAlloc__QEAA_II_Z),reinterpret_cast<decltype(API::destroy)>(&impl___1CFixedAlloc__QEAA_XZ),reinterpret_cast<decltype(API::allocate)>(&impl__Alloc_CFixedAlloc__QEAAPEAXXZ),reinterpret_cast<decltype(API::release)>(&impl__Free_CFixedAlloc__QEAAXPEAX_Z),reinterpret_cast<decltype(API::clear)>(&impl__FreeAll_CFixedAlloc__QEAAXXZ)},
        {reinterpret_cast<decltype(API::construct)>(&impl___0CFixedAllocNoSync__QEAA_II_Z),reinterpret_cast<decltype(API::destroy)>(&impl___1CFixedAllocNoSync__QEAA_XZ),reinterpret_cast<decltype(API::allocate)>(&impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ),reinterpret_cast<decltype(API::release)>(&impl__Free_CFixedAllocNoSync__QEAAXPEAX_Z),reinterpret_cast<decltype(API::clear)>(&impl__FreeAll_CFixedAllocNoSync__QEAAXXZ)},
        &impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z,&impl__FreeDataChain_CPlex__QEAAXXZ};
    HMODULE native=nullptr;wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_FIXED_ALLOC_DLL",path,1024)) {
        native=LoadLibraryW(path);check(native!=nullptr,"load native MFC allocator oracle");
        if(native){auto* b=reinterpret_cast<BYTE*>(native);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"allocator oracle uses the verified MFC image build");if(!pinned){FreeLibrary(native);native=nullptr;}}
    }
    {Imports imports(GetModuleHandleW(nullptr),true);expectations(source);
        if(native){auto* b=reinterpret_cast<BYTE*>(native);Collections oracle=source;
#define RVA(field,rva) oracle.field=reinterpret_cast<decltype(oracle.field)>(b+rva)
            RVA(sync.construct,0x22d310);RVA(sync.destroy,0x22d360);RVA(sync.allocate,0x22d3c0);RVA(sync.release,0x22d410);RVA(sync.clear,0x22d380);
            RVA(plain.construct,0x22d200);RVA(plain.destroy,0x22d230);RVA(plain.allocate,0x22d290);RVA(plain.release,0x22d2f0);RVA(plain.clear,0x22d240);RVA(plex,0x271300);RVA(chain,0x271340);
#undef RVA
            {Imports nativeImports(native,false);
                constructors(source,localTrace);constructors(oracle,nativeTrace);compare(localTrace,nativeTrace,"allocator constructor fields, defaults, untouched padding and critical-section failures match native MFC");
                sequences(source,localTrace);sequences(oracle,nativeTrace);compare(localTrace,nativeTrace,"allocator growth, node order, reuse and full-chain cleanup match native MFC");
                exceptions(source,localTrace);exceptions(oracle,nativeTrace);compare(localTrace,nativeTrace,"allocator locks, exception cleanup and callback mutations match native MFC");
                structured(source,localTrace);structured(oracle,nativeTrace);compare(localTrace,nativeTrace,"structured exceptions preserve the distinct native catch and finally cleanup boundaries");
                plexes(source,localTrace);plexes(oracle,nativeTrace);compare(localTrace,nativeTrace,"plex sizes, wrapping arithmetic, invalid arguments and chain release match native MFC");
            }
            stress(oracle);interoperability(source,oracle);
        }
        stress(source);
    }
    if(native)FreeLibrary(native);printf("%d failures\n",failures);return failures?1:0;
}
