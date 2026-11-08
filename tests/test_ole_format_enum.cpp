#define OPENMFC_TEST_WINDOW_HANDLES
#define OPENMFC_TEST_REAL_MODULE_THREAD_STATE
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include "openmfc/afxole.h"
#include "detail/CThreadLocalObjectSupport.h"
#include "detail/InterfaceMapsSupport.h"
#include <vector>
#include <array>
#include <cstdio>
#include <cstring>
extern "C" void* MS_ABI impl___0CEnumArray__QEAA__KPEBXIH_Z(void*,size_t,const void*,UINT,BOOL);
extern "C" void MS_ABI impl___1CEnumArray__UEAA_XZ(void*);
extern "C" BOOL MS_ABI impl__OnNext_CEnumArray__MEAAHPEAX_Z(void*,void*);
extern "C" BOOL MS_ABI impl__OnSkip_CEnumArray__MEAAHXZ(void*);
extern "C" void MS_ABI impl__OnReset_CEnumArray__MEAAXXZ(void*);
extern "C" void* MS_ABI impl__OnClone_CEnumArray__MEAAPEAV1_XZ(void*);
extern "C" void* MS_ABI impl___0CEnumFormatEtc__QEAA_XZ(void*);
extern "C" void MS_ABI impl___1CEnumFormatEtc__UEAA_XZ(void*);
extern "C" void MS_ABI impl__AddFormat_CEnumFormatEtc__QEAAXPEBUtagFORMATETC___Z(CEnumFormatEtc*,const FORMATETC*);
extern "C" BOOL MS_ABI impl__OnNext_CEnumFormatEtc__MEAAHPEAX_Z(CEnumFormatEtc*,void*);
namespace {
int failures;
void check(bool ok,const char* text){if(!ok){++failures;std::printf("FAIL: %s\n",text);}}
template<class T>T& field(void* self,size_t off){return *reinterpret_cast<T*>(static_cast<BYTE*>(self)+off);}
struct API {
    void* (MS_ABI* ctor)(void*);
    void (MS_ABI* dtor)(void*);
    void (MS_ABI* add)(CEnumFormatEtc*,const FORMATETC*);
    BOOL (MS_ABI* next)(CEnumFormatEtc*,void*);
    void* (MS_ABI* arrayCtor)(void*,size_t,const void*,UINT,BOOL);
    void (MS_ABI* arrayDtor)(void*);
    BOOL (MS_ABI* arrayNext)(void*,void*);
    BOOL (MS_ABI* arraySkip)(void*);
    void (MS_ABI* arrayReset)(void*);
    void* (MS_ABI* arrayClone)(void*);
    _AFX_THREAD_STATE* (MS_ABI* thread)();
    void (MS_ABI* destroyLocal)(S_Cthreadlocalobject*);
    HMODULE image;
};
const API* api;
alignas(16) BYTE modules[3][0x110]{};
void select(unsigned n){api->thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[n]);}
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
struct Storage {
    alignas(16) std::array<BYTE,160> bytes;
    Storage(){bytes.fill(0xa5);}
    CEnumFormatEtc* get(){return reinterpret_cast<CEnumFormatEtc*>(bytes.data()+16);}
    IEnumFORMATETC* part(){return reinterpret_cast<IEnumFORMATETC*>(bytes.data()+16+0x68);}
};
std::array<BYTE,160> constructorBytes;
void construction(){
    Storage object;api->ctor(object.get());auto* self=object.get();
    check(self->m_dwRef==1 && !self->m_pOuterUnknown && self->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]),"format enumerator constructs its actual command-target base");
    check(self->m_nSizeElem==sizeof(FORMATETC) && !self->m_pClonedFrom && !self->m_pvEnum && !self->m_nCurPos && !self->m_nSize && self->m_bNeedFree==TRUE && !self->m_nMaxSize,"format enumerator initializes the native 120-byte model");
    auto bytes=object.bytes;std::memset(bytes.data()+16,0,8);std::memset(bytes.data()+16+0x68,0,8);
    if(!api->image)constructorBytes=bytes;else check(bytes==constructorBytes,"constructor preserves every native padding and guard byte");
    auto* unknown=object.part();void* result=nullptr;
    check(unknown->QueryInterface(IID_IEnumFORMATETC,&result)==S_OK && result==unknown && self->m_dwRef==2 && unknown->Release()==1,"FORMATETC identity uses the actual interface map and owner reference count");
    result=nullptr;check(unknown->QueryInterface(IID_IUnknown,&result)==S_OK && result==unknown && unknown->Release()==1,"controlling IUnknown identifies the embedded interface");
    GUID missing={1,2,3,{4}};result=reinterpret_cast<void*>(UINT_PTR(1));check(unknown->QueryInterface(missing,&result)==E_NOINTERFACE && !result,"unknown interfaces are rejected and cleared");
    api->dtor(self);check(!self->m_pModuleState,"destruction reaches the real command-target destructor");
}
DVTARGETDEVICE* device(UINT id){
    constexpr UINT size=32;auto* result=static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(size));std::memset(result,int(id),size);result->tdSize=size;return result;
}
void fill(Storage& storage,UINT count){
    api->ctor(storage.get());
    for(UINT i=0;i<count;++i){
        FORMATETC format{CLIPFORMAT(CF_TEXT+i),i%2 ? device(i):nullptr,DVASPECT_CONTENT,LONG(i),TYMED_HGLOBAL};api->add(storage.get(),&format);
        auto* self=storage.get();auto* values=static_cast<const FORMATETC*>(self->m_pvEnum);
        check(self->m_nSize==i+1 && self->m_nMaxSize==((i/10)+1)*10 && values[i].cfFormat==format.cfFormat && values[i].ptd==format.ptd && values[i].lindex==format.lindex,"AddFormat grows by ten and transfers the supplied format fields and target-device pointer");
    }
}
struct Sample {HRESULT result;UINT position;ULONG fetched;std::vector<DWORD> data;bool operator==(const Sample& s)const{return result==s.result && position==s.position && fetched==s.fetched && data==s.data;}};
std::vector<Sample> samples;
void enumerate(){
    size_t sample=0;
    for(UINT count:{0u,1u,2u,10u,11u,31u})for(ULONG request:{0ul,1ul,2ul,4ul,32ul})for(bool skip:{false,true}){
        Storage object;fill(object,count);auto* self=object.get();auto* com=object.part();
        ULONG fetched=UINT_MAX;std::array<FORMATETC,32> out{};select(1);
        HRESULT skipped=skip ? com->Skip(request):S_OK;
        HRESULT result=com->Next(request,out.data(),&fetched);
        check(api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"enumeration restores the caller's module");
        Sample value{result,self->m_nCurPos,fetched,{DWORD(skipped)}};
        check(fetched<=request && fetched<=count,"Next reports only copied elements");
        for(ULONG i=0;i<fetched;++i){
            value.data.push_back(out[i].cfFormat);value.data.push_back(out[i].lindex);value.data.push_back(out[i].ptd ? out[i].ptd->tdSize:0);
            if(out[i].ptd){auto* original=static_cast<const FORMATETC*>(self->m_pvEnum)[out[i].lindex].ptd;check(out[i].ptd!=original && !std::memcmp(out[i].ptd,original,original->tdSize),"Next deep-copies each task-allocated target device");CoTaskMemFree(out[i].ptd);}
        }
        if(!api->image)samples.push_back(value);else check(value==samples[sample],"native enumeration, boundary skips and cursor state match");++sample;
        check(com->Reset()==S_OK && !self->m_nCurPos,"Reset restores the cursor");
        if(!count){fetched=42;check(com->Next(1,nullptr,&fetched)==S_FALSE && !fetched && !self->m_nCurPos,"an exhausted enumerator returns before touching the output buffer");}
        check(com->Next(0,nullptr,nullptr)==S_OK,"zero-element requests accept null output and count");
        select(0);api->dtor(self);
    }
}
std::vector<std::array<LONG,6>> cloneSamples;
void clone(){
    size_t sample=0;
    for(UINT count:{0u,1u,2u,11u})for(ULONG skip:{0ul,1ul,2ul,10ul}){
        Storage object;fill(object,count);auto* self=object.get();auto* com=object.part();com->Skip(skip);IEnumFORMATETC* copy=nullptr;select(1);
        HRESULT hr=com->Clone(&copy);check(hr==S_OK && copy && self->m_dwRef==2,"clone shares storage and retains its original owner");
        auto* owner=reinterpret_cast<CEnumArray*>(reinterpret_cast<BYTE*>(copy)-0x68);
        check(owner->m_pClonedFrom==self && owner->m_pvEnum==self->m_pvEnum && owner->m_nCurPos==self->m_nCurPos && !owner->m_bNeedFree,"generic clone uses the native shared-array model");
        IEnumFORMATETC* second=nullptr;check(copy->Clone(&second)==S_OK && self->m_dwRef==3,"a clone of a clone retains the original array owner");
        auto* secondOwner=reinterpret_cast<CEnumArray*>(reinterpret_cast<BYTE*>(second)-0x68);check(secondOwner->m_pClonedFrom==self && owner->m_dwRef==1,"nested clone points directly to the original");
        void* qi=nullptr;HRESULT identity=copy->QueryInterface(IID_IEnumFORMATETC,&qi);if(qi)reinterpret_cast<IUnknown*>(qi)->Release();
        FORMATETC result{};ULONG fetched=0;HRESULT next=copy->Next(1,&result,&fetched);
        bool shallow=!fetched || !result.ptd || result.ptd==static_cast<const FORMATETC*>(self->m_pvEnum)[result.lindex].ptd;
        std::array<LONG,6> value{identity,next,LONG(fetched),LONG(owner->m_nCurPos),LONG(shallow),LONG(self->m_dwRef)};
        if(!api->image)cloneSamples.push_back(value);else check(value==cloneSamples[sample],"native generic clone keeps its interface identity and shallow element behavior");++sample;
        check(shallow,"the generic clone copies existing target-device pointers without allocating");
        second->Release();copy->Release();check(self->m_dwRef==1 && api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"clone destruction balances original references and restores the caller's module");select(0);api->dtor(self);
    }
}
int destroyed;int callbackCalls;bool ownedError;
struct Error:CException {Error(BOOL owned):CException(owned){}~Error() override{++destroyed;}};
Error* lastError;
void callbackState(void* self){
    check(api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[callbackCalls ? 2:0]),"enumerator callbacks enter with the owning module and retain callback mutations inside the call");
    field<AFX_MODULE_STATE*>(self,0x38)=reinterpret_cast<AFX_MODULE_STATE*>(modules[2]);select(2);
}
BOOL MS_ABI nextCallback(void* self,void* out){callbackState(self);if(!callbackCalls++)return api->arrayNext(self,out);lastError=new Error(ownedError);throw static_cast<CException*>(lastError);}
BOOL MS_ABI skipCallback(void* self){callbackState(self);if(!callbackCalls++)return api->arraySkip(self);lastError=new Error(ownedError);throw static_cast<CException*>(lastError);}
void* MS_ABI cloneCallback(void* self){callbackState(self);++callbackCalls;lastError=new Error(ownedError);throw static_cast<CException*>(lastError);}
void MS_ABI resetCallback(void* self){callbackState(self);++callbackCalls;api->arrayReset(self);}
void exceptions(){
    for(int operation:{0,1,2,3})for(BOOL owned:{FALSE,TRUE}){
        Storage object;fill(object,2);auto* self=object.get();auto* com=object.part();void* root[26];std::memcpy(root,*reinterpret_cast<void***>(self),sizeof(root));
        root[22]=reinterpret_cast<void*>(&nextCallback);root[23]=reinterpret_cast<void*>(&skipCallback);root[24]=reinterpret_cast<void*>(&resetCallback);root[25]=reinterpret_cast<void*>(&cloneCallback);field<void*>(self,0)=root;
        callbackCalls=0;destroyed=0;ownedError=owned!=FALSE;lastError=nullptr;FORMATETC out[3]{};ULONG fetched=77;IEnumFORMATETC* copy=reinterpret_cast<IEnumFORMATETC*>(UINT_PTR(1));select(1);
        HRESULT result=operation==0 ? com->Next(3,out,&fetched):operation==1 ? com->Skip(3):operation==2 ? com->Clone(&copy):com->Reset();
        check(result==(operation==1 ? S_FALSE:operation==3 ? S_OK:E_UNEXPECTED) && api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"native COM catches framework callback exceptions and restores the original module");
        check(destroyed==(operation==3 ? 0:int(owned)) && (operation!=0 || (!fetched && self->m_nCurPos==1)) && (operation!=1 || self->m_nCurPos==1) && (operation!=2 || !copy),"exceptions preserve partial progress, output initialization and conditional exception ownership");
        if(lastError && !destroyed)delete lastError;select(0);api->dtor(self);
    }
}
void* WINAPI allocationFailure(SIZE_T){return nullptr;}
void allocationFailures(){
    Storage object;fill(object,2);auto* self=object.get();auto* com=object.part();FORMATETC result[2]{};ULONG fetched=99;select(1);
    {Patch fail(api->image ? api->image:GetModuleHandleW(nullptr),"CoTaskMemAlloc",reinterpret_cast<void*>(&allocationFailure));
        check(com->Next(2,result,&fetched)==E_UNEXPECTED && !fetched && self->m_nCurPos==2 && !result[1].ptd,"a target-device allocation failure keeps native partial output and cursor progress");}
    check(api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"allocation failure restores the caller's module");select(0);api->dtor(self);
}
void arrays(){
    for(UINT count:{0u,1u,2u,5u})for(size_t size:{size_t(0),size_t(1),size_t(4)}){
        Storage object;DWORD values[]={1,2,3,4,5};api->arrayCtor(object.get(),size,values,count,FALSE);
        auto* owner=static_cast<CEnumArray*>(object.get());check(owner->m_nSizeElem==size && owner->m_pvEnum==values && owner->m_nSize==count,"generic constructor preserves array arguments");
        DWORD out=UINT_MAX;for(UINT i=0;i<count;++i){DWORD expected=out;if(size)std::memcpy(&expected,reinterpret_cast<BYTE*>(values)+i*size,size);check(api->arrayNext(owner,&out)==TRUE && owner->m_nCurPos==i+1 && out==expected,"qualified generic Next copies exactly the configured element size");}
        check(api->arrayNext(owner,&out)==FALSE,"generic Next stops at the end");api->arrayReset(owner);
        for(UINT i=0;i<count;++i)check(bool(api->arraySkip(owner))==(i+1<count),"generic skip retains the native last-element return value");
        api->arrayDtor(owner);check(!owner->m_pModuleState,"generic destruction clears its command-target base");
    }
}
void sharedVerbEnumeration(){
    auto* verbs=static_cast<OLEVERB*>(std::malloc(2*sizeof(OLEVERB)));
    for(int i=0;i<2;++i){verbs[i]={i,static_cast<wchar_t*>(CoTaskMemAlloc(8)),0,0};wcscpy_s(verbs[i].lpszVerbName,4,i ? L"two":L"one");}
    auto* owner=new CEnumOleVerb;owner->SetVerbs(verbs,2);auto* source=owner->Interface();
    IEnumOLEVERB* copy=nullptr;check(source->Clone(&copy)==S_OK && copy && owner->m_dwRef==2,"the restored generic base still clones native verb enumerators");
    source->Release();OLEVERB out[2]{};ULONG fetched=0;
    check(copy->Next(2,out,&fetched)==S_OK && fetched==2 && !wcscmp(out[0].lpszVerbName,L"one") && !wcscmp(out[1].lpszVerbName,L"two"),"generic verb clones keep their original names alive after releasing the original interface");copy->Release();
}
void run(const API& functions){
    api=&functions;auto* previous=api->thread()->m_pModuleState;select(0);construction();enumerate();clone();arrays();exceptions();allocationFailures();if(!api->image)sharedVerbEnumeration();
    api->thread()->m_pModuleState=previous;
    for(auto& module:modules)api->destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));std::memset(modules,0,sizeof(modules));
    std::printf("%s FORMATETC checks completed\n",api->image ? "Native":"Source");
}
}
int main(){
    setvbuf(stdout,nullptr,_IONBF,0);CoInitialize(nullptr);
    API source{impl___0CEnumFormatEtc__QEAA_XZ,impl___1CEnumFormatEtc__UEAA_XZ,impl__AddFormat_CEnumFormatEtc__QEAAXPEBUtagFORMATETC___Z,impl__OnNext_CEnumFormatEtc__MEAAHPEAX_Z,
        impl___0CEnumArray__QEAA__KPEBXIH_Z,impl___1CEnumArray__UEAA_XZ,impl__OnNext_CEnumArray__MEAAHPEAX_Z,impl__OnSkip_CEnumArray__MEAAHXZ,impl__OnReset_CEnumArray__MEAAXXZ,impl__OnClone_CEnumArray__MEAAPEAV1_XZ,AfxGetThreadState,impl___1CThreadLocalObject__QEAA_XZ,nullptr};run(source);
    if(auto* path=std::getenv("OPENMFC_NATIVE_ENUM_DLL")){
        HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native enumeration oracle");if(image){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"native enumeration oracle is the pinned image");if(pinned){API native=source;native.image=image;
#define RVA(member,value) native.member=reinterpret_cast<decltype(native.member)>(b+value)
                RVA(ctor,0x253750);RVA(dtor,0x2537e0);RVA(add,0x253880);RVA(next,0x253840);RVA(arrayCtor,0x25b660);RVA(arrayDtor,0x25b720);RVA(arrayNext,0x25b770);RVA(arraySkip,0x25b7e0);RVA(arrayReset,0x25b800);RVA(arrayClone,0x25b810);RVA(thread,0x1332a0);RVA(destroyLocal,0x14d0d0);
#undef RVA
                run(native);
            }FreeLibrary(image);
        }
    }
    CoUninitialize();std::printf("FORMATETC enumeration: %d failures\n",failures);return failures ? 1:0;
}
