// Process-local creation and critical-section lifecycle against the native ABI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
#include "detail/CProcessLocalObjectSupport.h"
#include <array>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
extern "C" int AFXAPI impl__AfxCriticalInit__YAHXZ();
extern "C" void AFXAPI impl__AfxCriticalTerm__YAXXZ();
extern "C" void* MS_ABI impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(void*,CreateFn);
extern "C" void MS_ABI impl___1CProcessLocalObject__QEAA_XZ(void*);
// Only the pointer type is relevant to this algorithm's catch/rethrow boundary.
// A factory exception must remain opaque; the holder never inspects or deletes it.
alignas(void*) unsigned char exceptionMarker[sizeof(CException)];
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ(){throw reinterpret_cast<CException*>(exceptionMarker);}
extern "C" void AFXAPI impl__AfxThrowMemoryException__YAXXZ(){throw reinterpret_cast<CException*>(exceptionMarker);}
extern "C" void AFXAPI impl__AfxThrowOleException__YAXJ_Z(long){std::abort();}
namespace {
int failures;
void check(bool ok,const char* message){if(!ok){++failures;std::printf("FAIL: %s\n",message);}}
struct API {
    void* (MS_ABI* get)(void*,CreateFn);
    void (MS_ABI* destroy)(void*);
    void (AFXAPI* lock)(int);
    void (AFXAPI* unlock)(int);
    int (AFXAPI* initialize)();
    void (AFXAPI* terminate)();
    HMODULE image;
};
const API* api;
void** imported(HMODULE image,const char* wanted){
    auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* desc=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;desc->Name;++desc){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->FirstThunk);
        for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);
        }
    }return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(HMODULE image,const char* name,void* fn){slot=imported(image,name);check(slot!=nullptr,name);if(!slot)std::abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=fn;}
    ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
struct Event {int kind;CRITICAL_SECTION* lock;};
std::vector<Event> events;
std::array<CRITICAL_SECTION*,17> locks;
CRITICAL_SECTION* initializeLock;
bool reenterInitialization=false;
S_Cprocesslocalobject* injectionHolder=nullptr;
void* injectionObject=nullptr;
bool injectEnter=false,injectLeave=false;
decltype(&InitializeCriticalSection) realInit;
decltype(&EnterCriticalSection) realEnter;
decltype(&LeaveCriticalSection) realLeave;
decltype(&DeleteCriticalSection) realDelete;
void WINAPI initialize(CRITICAL_SECTION* lock){events.push_back({0,lock});realInit(lock);}
void WINAPI enter(CRITICAL_SECTION* lock){
    events.push_back({1,lock});
    if(injectEnter && lock==locks[16]){injectEnter=false;injectionHolder->m_pObject=injectionObject;}
    realEnter(lock);
    if(reenterInitialization && lock==initializeLock){reenterInitialization=false;api->lock(3);api->unlock(3);}
}
void WINAPI leave(CRITICAL_SECTION* lock){events.push_back({2,lock});realLeave(lock);if(injectLeave && lock==locks[16]){injectLeave=false;injectionHolder->m_pObject=injectionObject;}}
void WINAPI terminate(CRITICAL_SECTION* lock){events.push_back({3,lock});realDelete(lock);}
void newHandler(){}
void criticalSections(){
    auto previous=std::set_new_handler(newHandler);events.clear();
    check(api->initialize()==1 && events.size()==1 && events[0].kind==0,"critical initialization creates only the initialization lock");initializeLock=events[0].lock;
    check(api->initialize()==1 && events.size()==1 && std::get_new_handler()==newHandler,"repeated critical initialization preserves the handler and existing lock");
    for(int index=0;index<17;++index){
        events.clear();api->lock(index);api->lock(index);api->unlock(index);api->unlock(index);
        check(events.size()==7,"a cold global lock initializes once and supports recursive entry");
        if(events.size()==7){locks[index]=events[3].lock;std::array<int,7> kinds{1,0,2,1,1,2,2};for(size_t i=0;i<kinds.size();++i)check(events[i].kind==kinds[i],"native cold-lock call order");
            check(events[0].lock==initializeLock && events[2].lock==initializeLock && events[1].lock==locks[index] && events[4].lock==locks[index] && events[5].lock==locks[index] && events[6].lock==locks[index],"initialization and recursive entry use the correct actual critical sections");}
    }
    for(int index:{-1,17,INT_MAX,INT_MIN})for(bool unlocking:{false,true}){
        events.clear();bool caught=false;try{if(unlocking)api->unlock(index);else api->lock(index);}catch(CException* error){caught=error!=nullptr;}
        check(caught && events.empty(),"invalid global lock indices throw before touching a critical section");
    }
    events.clear();api->terminate();check(events.size()==18 && events[0].kind==3 && events[0].lock==initializeLock,"critical termination deletes the initialization lock first");
    if(events.size()==18)for(size_t i=0;i<locks.size();++i)check(events[i+1].kind==3 && events[i+1].lock==locks[i],"critical termination deletes initialized locks in index order");
    events.clear();api->terminate();check(events.empty() && std::get_new_handler()==newHandler,"repeated termination is empty and preserves the CRT new handler");
    events.clear();api->lock(16);api->unlock(16);check(events.size()==6 && events[0].kind==0 && events[2].kind==0,"lock sixteen reinitializes after critical termination");
    if(events.size()==6)locks[16]=events[4].lock;
    events.clear();reenterInitialization=true;api->lock(3);api->unlock(3);
    UINT created=0;for(auto event:events)if(event.kind==0){++created;check(event.lock==locks[3],"recursive lazy initialization creates the requested native lock");}
    check(created==1 && !reenterInitialization,"global locking rechecks initialization after recursive entry");
    std::set_new_handler(previous);
}
S_Cprocesslocalobject* holder;
int factoryCalls,depth,destroyed;int mode;
void* replacement;
struct Record:CNoTrackObject {
    int id;
    explicit Record(int value):id(value){}
    ~Record() override{++destroyed;if(replacement)holder->m_pObject=replacement;}
};
Record *outerRecord,*innerRecord;
DWORD WINAPI probe(void* value){auto* lock=static_cast<CRITICAL_SECTION*>(value);if(!TryEnterCriticalSection(lock))return 0;LeaveCriticalSection(lock);return 1;}
bool available(){HANDLE thread=CreateThread(nullptr,0,probe,locks[16],0,nullptr);check(thread!=nullptr,"create a critical-section probe");if(!thread)return false;WaitForSingleObject(thread,INFINITE);DWORD result=0;GetExitCodeThread(thread,&result);CloseHandle(thread);return result!=0;}
void* MS_ABI innerFactory(){++factoryCalls;check(locks[16]->RecursionCount==2,"recursive process-local creation reuses native global lock sixteen");return innerRecord;}
void* MS_ABI factory(){
    ++factoryCalls;check(!available(),"the factory runs while holding the actual process-local critical section");
    if(mode==1)return nullptr;
    if(mode==2){holder->m_pObject=innerRecord;throw reinterpret_cast<CException*>(exceptionMarker);}
    if(mode==3){void* result=api->get(holder,innerFactory);check(result==innerRecord && holder->m_pObject==innerRecord,"recursive creation commits and returns its own factory result");}
    if(mode==4)throw 37;
    if(mode==5)throw reinterpret_cast<CMemoryException*>(exceptionMarker);
    if(mode==6)throw reinterpret_cast<CUserException*>(exceptionMarker);
    return outerRecord;
}
void* MS_ABI forbidden(){check(false,"cached holders must not invoke the factory");return nullptr;}
void processLocal(){
    S_Cprocesslocalobject storage{};holder=&storage;replacement=nullptr;destroyed=0;factoryCalls=0;mode=0;
    outerRecord=new Record(1);innerRecord=new Record(2);events.clear();
    check(api->get(holder,factory)==outerRecord && holder->m_pObject==outerRecord && factoryCalls==1 && available(),"process-local creation commits its object and releases global lock sixteen");
    events.clear();check(api->get(holder,forbidden)==outerRecord && events.empty(),"cached process-local lookup touches neither factory nor locks");
    api->destroy(holder);check(destroyed==1 && holder->m_pObject==outerRecord,"native destruction deletes the object without clearing the holder field");delete innerRecord;
    storage.m_pObject=nullptr;mode=1;factoryCalls=0;check(!api->get(holder,factory) && !api->get(holder,factory) && !holder->m_pObject && factoryCalls==2 && available(),"a null factory result is not cached and leaves the lock released");
    outerRecord=new Record(3);innerRecord=new Record(4);storage.m_pObject=nullptr;mode=2;factoryCalls=0;bool caught=false;
    try{api->get(holder,factory);}catch(CException* error){caught=error==reinterpret_cast<CException*>(exceptionMarker);}
    check(caught && factoryCalls==1 && holder->m_pObject==innerRecord && available(),"framework factory exceptions unlock and rethrow the untouched pointer while preserving callback mutations");
    events.clear();check(api->get(holder,forbidden)==innerRecord && events.empty(),"a factory-written object survives an exceptional exit");api->destroy(holder);delete outerRecord;
    for(int kind:{5,6}){
        storage.m_pObject=nullptr;mode=kind;factoryCalls=0;bool preserved=false;
        try{api->get(holder,factory);}catch(CMemoryException* error){preserved=kind==5 && reinterpret_cast<void*>(error)==exceptionMarker;}
        catch(CUserException* error){preserved=kind==6 && reinterpret_cast<void*>(error)==exceptionMarker;}
        check(preserved && factoryCalls==1 && !holder->m_pObject && available(),"factory rethrow preserves derived framework exception pointer types without acquiring ownership");
    }
    outerRecord=new Record(5);innerRecord=new Record(6);storage.m_pObject=nullptr;mode=3;factoryCalls=0;
    check(api->get(holder,factory)==outerRecord && factoryCalls==2 && holder->m_pObject==outerRecord && available(),"recursive creation preserves native nested and outer commit order");api->destroy(holder);delete innerRecord;
    // The native typed catch intentionally does not handle arbitrary C++ throws.
    storage.m_pObject=nullptr;mode=4;factoryCalls=0;bool other=false;try{api->get(holder,factory);}catch(int value){other=value==37;}
    check(other && !available(),"non-framework factory exceptions preserve the native held-lock behavior");api->unlock(16);check(available(),"the test releases the intentionally retained native lock");
    outerRecord=new Record(7);innerRecord=new Record(8);storage.m_pObject=outerRecord;replacement=innerRecord;int before=destroyed;api->destroy(holder);replacement=nullptr;
    check(destroyed==before+1 && holder->m_pObject==innerRecord,"destruction preserves field mutations made by the object's destructor");delete innerRecord;
    storage.m_pObject=nullptr;events.clear();api->destroy(holder);check(events.empty(),"destroying an empty holder does nothing");
    outerRecord=new Record(9);innerRecord=new Record(10);injectionHolder=holder;injectionObject=innerRecord;injectEnter=true;factoryCalls=0;
    check(api->get(holder,forbidden)==innerRecord && !injectEnter && !factoryCalls,"process-local lookup rechecks a value installed while entering its lock");api->destroy(holder);delete outerRecord;
    outerRecord=new Record(11);innerRecord=new Record(12);storage.m_pObject=nullptr;injectionObject=innerRecord;injectLeave=true;factoryCalls=0;mode=0;
    check(api->get(holder,factory)==innerRecord && !injectLeave && factoryCalls==1 && holder->m_pObject==innerRecord,"lookup reloads the committed pointer after unlocking");api->destroy(holder);delete outerRecord;
    injectionHolder=nullptr;storage.m_pObject=nullptr;
}
void run(const API& functions){
    api=&functions;api->terminate();HMODULE image=api->image ? api->image:GetModuleHandleW(nullptr);
    {Patch a(image,"InitializeCriticalSection",reinterpret_cast<void*>(&initialize)),b(image,"EnterCriticalSection",reinterpret_cast<void*>(&enter)),c(image,"LeaveCriticalSection",reinterpret_cast<void*>(&leave)),d(image,"DeleteCriticalSection",reinterpret_cast<void*>(&terminate));
        realInit=reinterpret_cast<decltype(realInit)>(a.old);realEnter=reinterpret_cast<decltype(realEnter)>(b.old);realLeave=reinterpret_cast<decltype(realLeave)>(c.old);realDelete=reinterpret_cast<decltype(realDelete)>(d.old);
        criticalSections();processLocal();}
    std::printf("%s process-local checks completed\n",api->image ? "Native":"Source");
}
}
int main(){
    setvbuf(stdout,nullptr,_IONBF,0);std::memset(exceptionMarker,0x5a,sizeof(exceptionMarker));events.reserve(256);API source{impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z,impl___1CProcessLocalObject__QEAA_XZ,impl__AfxLockGlobals__YAXH_Z,impl__AfxUnlockGlobals__YAXH_Z,impl__AfxCriticalInit__YAHXZ,impl__AfxCriticalTerm__YAXXZ,nullptr};run(source);
    if(auto* path=std::getenv("OPENMFC_NATIVE_PROCESS_DLL")){auto image=LoadLibraryA(path);check(image!=nullptr,"load native process-local oracle");if(image){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"process-local oracle uses the pinned image");if(pinned){API native=source;native.image=image;
#define RVA(member,value) native.member=reinterpret_cast<decltype(native.member)>(b+value)
        RVA(get,0x14d100);RVA(destroy,0x14d150);RVA(lock,0x33540);RVA(unlock,0x335e0);RVA(initialize,0x334a0);RVA(terminate,0x334d0);
#undef RVA
        run(native);}FreeLibrary(image);}}
    std::printf("Process-local state: %d failures\n",failures);return failures ? 1:0;
}
