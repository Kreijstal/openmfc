// Real base lifetime, module/TLS state, intrusive lists and process locks.
#define OPENMFC_TEST_WINDOW_HANDLES
#define OPENMFC_TEST_REAL_MODULE_THREAD_STATE
#define OPENMFC_TEST_REAL_GLOBAL_LOCKS
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include "openmfc/afxdll_.h"
#include "detail/RegcoreSupport.h"
#include <array>
#include <vector>
#include <cstdio>
#include <cstring>
#include <type_traits>
extern "C" {
CDynLinkLibrary* AFXAPI impl___0CDynLinkLibrary__QEAA_AEAUAFX_EXTENSION_MODULE__H_Z(CDynLinkLibrary*,AFX_EXTENSION_MODULE&,BOOL);
CDynLinkLibrary* AFXAPI impl___0CDynLinkLibrary__QEAA_PEAUHINSTANCE____0_Z(CDynLinkLibrary*,HINSTANCE,HINSTANCE);
void AFXAPI impl___1CDynLinkLibrary__UEAA_XZ(CDynLinkLibrary*);
CRuntimeClass* AFXAPI impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* AFXAPI impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ(const CDynLinkLibrary*);
void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
HINSTANCE AFXAPI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*,const wchar_t*);
void* AFXAPI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
int AFXAPI impl__AfxLoadString__YAHIPEA_WI_Z(UINT,wchar_t*,UINT);
}
namespace {
int failures;
void check(bool condition,const char* text) {if(!condition){++failures;std::printf("FAIL: %s\n",text);}}
struct API {
    decltype(&impl___0CDynLinkLibrary__QEAA_AEAUAFX_EXTENSION_MODULE__H_Z) extension;
    decltype(&impl___0CDynLinkLibrary__QEAA_PEAUHINSTANCE____0_Z) handles;
    decltype(&impl___1CDynLinkLibrary__UEAA_XZ) destroy;
    decltype(&impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ) descriptor;
    decltype(&impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ) runtime;
    _AFX_THREAD_STATE* (AFXAPI* thread)();
    decltype(&impl__AfxLockGlobals__YAXH_Z) lock;
    decltype(&impl__AfxUnlockGlobals__YAXH_Z) unlock;
    decltype(&impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z) resource;
    decltype(&impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z) string;
    decltype(&impl__AfxLoadString__YAHIPEA_WI_Z) load;
    HMODULE image;
};
struct alignas(16) Object {
    BYTE bytes[160];
    CDynLinkLibrary* get() {return reinterpret_cast<CDynLinkLibrary*>(bytes+16);}
    void reset() {std::memset(bytes,0xa5,sizeof(bytes));}
};
AFX_MODULE_STATE modules[3];
const API* api;
CDynLinkLibrary* watched;
std::vector<int> events;
bool recording,constructing,inside,captureLock;
bool resourceRecording;
int resourceLockDepth;
int mutation;
HINSTANCE expectedModule,expectedResources;
void *expectedClasses,*expectedFactories;
BOOL expectedSystem;
void* expectedRoot;
CRITICAL_SECTION *entered,*libraryLock;
decltype(&::EnterCriticalSection) realEnter;
decltype(&::LeaveCriticalSection) realLeave;
void select(unsigned index) {api->thread()->m_pModuleState=&modules[index];}
template<class T> T& field(void* object,size_t offset) {return *reinterpret_cast<T*>(static_cast<BYTE*>(object)+offset);}
void WINAPI enter(CRITICAL_SECTION* lock) {
    if(captureLock)libraryLock=lock;
    if(recording && lock==libraryLock) {
        events.push_back(1);entered=lock;
        auto* table=*reinterpret_cast<void***>(watched);
        using Runtime=CRuntimeClass* (AFXAPI*)(const CDynLinkLibrary*);
        auto* type=reinterpret_cast<Runtime>(table[0])(watched);
        check(type && !std::strcmp(type->m_lpszClassName,"CDynLinkLibrary") && type->m_nObjectSize==128,
              "the library installs its own working root before entering the registration lock");
        if(constructing) {
            check(watched->m_hModule==expectedModule && watched->m_hResource==expectedResources &&
                  watched->m_classList.m_pHead==expectedClasses && watched->m_factoryList.m_pHead==expectedFactories &&
                  watched->m_classList.m_nNextOffset==0x20 && watched->m_factoryList.m_nNextOffset==0x40 &&
                  watched->m_bSystem==expectedSystem && watched->m_pModuleState==&modules[0],
                  "constructor fields and captured base module precede the external lock call");
            check(field<UINT_PTR>(watched,0x78)==0xa5a5a5a5a5a5a5a5ULL,"the library link remains untouched until registration");
        } else check(*reinterpret_cast<void**>(watched)==expectedRoot,"destruction restores the library root before locking");
        if(mutation==1)select(1);
        if(mutation==2)watched->m_pModuleState=&modules[1];
        if(mutation==3)throw 77;
    }
    realEnter(lock);if(recording && lock==libraryLock)inside=true;
    if(resourceRecording && lock==libraryLock)++resourceLockDepth;
}
void WINAPI leave(CRITICAL_SECTION* lock) {
    if(resourceRecording && lock==libraryLock)--resourceLockDepth;
    if(recording && lock==libraryLock) {
        events.push_back(2);check(inside && entered==lock,"library registration and removal balance the same recursive process lock");
        inside=false;
    }
    realLeave(lock);
}
void** import(HMODULE image,const char* wanted) {
    auto* base=reinterpret_cast<BYTE*>(image);
    auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
    auto* descriptor=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;descriptor->Name;++descriptor) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(base+descriptor->OriginalFirstThunk);
        auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(base+descriptor->FirstThunk);
        for(;names->u1.AddressOfData;++names,++values)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base+names->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&values->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot;void* previous;DWORD protection;
    Patch(HMODULE image,const char* name,void* replacement) {
        slot=import(image,name);check(slot!=nullptr,name);if(!slot)std::abort();previous=*slot;
        ::VirtualProtect(slot,sizeof(void*),PAGE_READWRITE,&protection);*slot=replacement;
    }
    ~Patch(){*slot=previous;DWORD ignored;::VirtualProtect(slot,sizeof(void*),protection,&ignored);}
};
void clear() {
    for(auto& module:modules){module.m_libraryList.RemoveAll();module.m_libraryList.Construct(0x78);
        module.m_bSystem=FALSE;module.m_hCurrentResourceHandle=nullptr;module.m_appLangDLL=nullptr;}
    select(0);inside=false;recording=false;events.clear();
}
std::vector<std::array<BYTE,160>> golden;
void constructors(bool native) {
    unsigned sample=0;
    for(bool extension:{false,true})for(BOOL system:{0,1,-1,17})for(int change:{0,1,2,3})for(int offset:{0x78,0x50}) {
        clear();modules[0].m_libraryList.Construct(offset);modules[1].m_libraryList.Construct(offset);
        Object object;object.reset();watched=object.get();constructing=true;mutation=change;
        AFX_EXTENSION_MODULE state{int(0xa5a5a5a5),reinterpret_cast<HINSTANCE>(UINT_PTR(0x1111)),reinterpret_cast<HINSTANCE>(UINT_PTR(0x2222)),
            reinterpret_cast<CRuntimeClass*>(UINT_PTR(0x3333)),reinterpret_cast<COleObjectFactory*>(UINT_PTR(0x4444))};
        auto stateBefore=state;
        expectedModule=state.hModule;expectedResources=state.hResource;expectedSystem=extension ? system:FALSE;
        expectedClasses=extension ? state.pFirstSharedClass:nullptr;expectedFactories=extension ? state.pFirstSharedFactory:nullptr;
        recording=true;int caught=0;
        try {
            auto* result=extension ? api->extension(watched,state,system):api->handles(watched,state.hModule,state.hResource);
            check(result==watched,"both constructors return the caller's placement storage");
        } catch(int error) {caught=error;}
        recording=false;
        check(caught==(change==3 ? 77:0) && events==(change==3 ? std::vector<int>{1}:std::vector<int>{1,2}),
              "constructor failure unwinds the base without unlocking a lock that was never entered");
        check(!std::memcmp(&state,&stateBefore,sizeof(state)),"extension registration borrows its input module record");
        if(caught) {
            check(!watched->m_pModuleState && !modules[0].m_libraryList.m_pHead && !modules[1].m_libraryList.m_pHead,
                  "a failed constructor clears the base module and leaves no partially registered library");
        } else {
            unsigned owner=change==2 ? 1:0;
            check(modules[owner].m_libraryList.m_pHead==watched && field<void*>(watched,offset)==nullptr &&
                  !modules[1-owner].m_libraryList.m_pHead,"registration reloads the object's module after callbacks and uses the list's configured link offset");
        }
        std::array<BYTE,160> bytes;std::memcpy(bytes.data(),object.bytes,bytes.size());std::memset(bytes.data()+16,0,sizeof(void*));
        if(!native)golden.push_back(bytes);
        else check(sample<golden.size() && bytes==golden[sample],"native and source constructors match every data, padding and guard byte");
        ++sample;
        if(!caught)api->destroy(watched);
        check(!modules[0].m_libraryList.m_pHead && !modules[1].m_libraryList.m_pHead,"destruction removes registration even under a different ambient module");
    }
}
void AFXAPI disconnect(void* dispatch) {
    events.push_back(3);
    check(dispatch==reinterpret_cast<BYTE*>(watched)+32 && !inside && watched->m_pModuleState,
          "base dispatch shutdown follows unlocking while the captured module is still available");
    if(mutation==5) {
        watched->m_pModuleState=&modules[2];field<void*>(watched,0)=reinterpret_cast<void*>(UINT_PTR(0x12345678));
    }
}
void lifetime() {
    for(int change:{0,1,2,5}) {
        clear();Object objects[3];
        for(auto& object:objects) {object.reset();api->handles(object.get(),nullptr,nullptr);}
        check(modules[0].m_libraryList.m_pHead==objects[2].get() && objects[2].get()->m_pNextDLL==objects[1].get() &&
              objects[1].get()->m_pNextDLL==objects[0].get(),"multiple libraries form the actual module's intrusive LIFO chain");
        watched=objects[1].get();expectedRoot=field<void*>(watched,0);field<void*>(watched,0)=reinterpret_cast<void*>(UINT_PTR(1));
        void* dispatchTable[8]{};dispatchTable[7]=reinterpret_cast<void*>(&disconnect);watched->m_xDispatch=reinterpret_cast<UINT_PTR>(dispatchTable);
        select(1);constructing=false;mutation=change;events.clear();recording=true;
        api->destroy(watched);recording=false;
        check(events==std::vector<int>({1,2,3}) && !watched->m_pModuleState && !inside,
              "library removal, unlocking, dispatch disconnection and module clearing follow native order");
        if(change==2) {
            check(objects[2].get()->m_pNextDLL==watched,"a lock callback's module replacement controls which list is searched");
            modules[0].m_libraryList.Remove(watched);
        } else check(objects[2].get()->m_pNextDLL==objects[0].get(),"interior removal updates its predecessor's real link");
        check(field<void*>(watched,0)==(change==5 ? reinterpret_cast<void*>(UINT_PTR(0x12345678)):expectedRoot),
              "base destruction preserves callback changes to the derived root");
        api->destroy(objects[0].get());api->destroy(objects[2].get());
        check(modules[0].m_libraryList.IsEmpty(),"head and tail removal empty the module library list");
    }
}
#include "resource_search_tests.h"
void run(const API& functions) {
    api=&functions;auto* old=api->thread()->m_pModuleState;clear();
    api->lock(0);api->unlock(0); // initialize the real lock before installing observers
    {
        HMODULE image=api->image ? api->image:GetModuleHandleW(nullptr);
        Patch first(image,"EnterCriticalSection",reinterpret_cast<void*>(&enter));
        Patch second(image,"LeaveCriticalSection",reinterpret_cast<void*>(&leave));
        realEnter=reinterpret_cast<decltype(realEnter)>(first.previous);realLeave=reinterpret_cast<decltype(realLeave)>(second.previous);
        captureLock=true;api->lock(0);captureLock=false;api->unlock(0);
        check(libraryLock!=nullptr,"observe the production library lock separately from TLS allocation locks");
        constructors(api->image!=nullptr);lifetime();resourceSearch();
    }
    auto* type=api->descriptor();
    auto* parent=type ? type->BaseClass():nullptr;
    check(type && !std::strcmp(type->m_lpszClassName,"CDynLinkLibrary") && type->m_nObjectSize==128 && type->m_wSchema==0xffff &&
          !type->m_pfnCreateObject && parent && (api->image ? parent->m_lpszClassName && !std::strcmp(parent->m_lpszClassName,"CCmdTarget"):
          parent==CCmdTarget::GetThisClass()),
          "the real library runtime descriptor has native size, schema, factory and base ancestry");
    check(api->runtime(reinterpret_cast<CDynLinkLibrary*>(UINT_PTR(1)))==type,"runtime export is a qualified base getter without reading the incoming object");
    api->thread()->m_pModuleState=old;
    std::printf("%s extension library lifetime completed\n",api->image ? "Native":"Source");
}
}
int main() {
    setvbuf(stdout,nullptr,_IONBF,0);events.reserve(32);golden.reserve(64);
    static_assert(std::is_same<decltype(std::declval<CSimpleList>().GetNextPtr(nullptr)),void**>::value,"SDK list link API");
    API source{impl___0CDynLinkLibrary__QEAA_AEAUAFX_EXTENSION_MODULE__H_Z,impl___0CDynLinkLibrary__QEAA_PEAUHINSTANCE____0_Z,
        impl___1CDynLinkLibrary__UEAA_XZ,impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ,
        impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ,AfxGetThreadState,impl__AfxLockGlobals__YAXH_Z,impl__AfxUnlockGlobals__YAXH_Z,
        impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z,impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z,
        impl__AfxLoadString__YAHIPEA_WI_Z,nullptr};
    run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_DYNLINK_DLL")) {
        auto image=LoadLibraryA(path);check(image!=nullptr,"load native extension-library oracle");
        if(image) {
            auto* base=reinterpret_cast<BYTE*>(image);
            auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"extension-library oracle uses the pinned native image");
            if(pinned) {
                API native=source;native.image=image;
#define RVA(member,value) native.member=reinterpret_cast<decltype(native.member)>(base+value)
                RVA(extension,0x2ae800);RVA(handles,0x2ae900);RVA(destroy,0x2ae9a0);RVA(descriptor,0x2ae7f0);RVA(runtime,0x2ae7f0);
                RVA(thread,0x1332a0);RVA(lock,0x33540);RVA(unlock,0x335e0);
                RVA(resource,0x2aeb50);RVA(string,0x2aee00);RVA(load,0x2af0b0);
#undef RVA
                run(native);
            }
            FreeLibrary(image);
        }
    }
    std::printf("Extension libraries: %d failures\n",failures);return failures ? 1:0;
}
