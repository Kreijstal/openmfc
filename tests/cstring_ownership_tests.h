#pragma once
namespace cstring_ownership_test {
using Binary=HRESULT (__cdecl*)(void*,const void*);
using Set=HRESULT (__cdecl*)(void*,const wchar_t*);
using Unary=HRESULT (__cdecl*)(void*);
using Destroy=void (__cdecl*)(void*);
struct API {Binary copy,assign;Set set;Unary empty;Destroy destroy;};
template<class Operation> HRESULT invoke(Operation operation) {
    try {operation();return S_OK;}catch(const std::bad_alloc&){return E_OUTOFMEMORY;}catch(CException* exception){exception->Delete();return E_OUTOFMEMORY;}
}
static HRESULT __cdecl copy(void* target,const void* source) {return invoke([&]{new(target) CString(*static_cast<const CString*>(source));});}
static HRESULT __cdecl assign(void* target,const void* source) {return invoke([&]{*static_cast<CString*>(target)=*static_cast<const CString*>(source);});}
static HRESULT __cdecl set(void* target,const wchar_t* source) {return invoke([&]{*static_cast<CString*>(target)=source;});}
static HRESULT __cdecl empty(void* value) {return invoke([&]{static_cast<CString*>(value)->Empty();});}
static void __cdecl destroy(void* value) {static_cast<CString*>(value)->~CString();}
struct Manager: database_strings_test::Manager {
    bool fail=false;
    CStringData* Allocate(int length,int size) override {if(fail){++allocations;return nullptr;}return database_strings_test::Manager::Allocate(length,size);}
    CStringData* Reallocate(CStringData* data,int length,int size) override {if(fail){++reallocations;return nullptr;}return database_strings_test::Manager::Reallocate(data,length,size);}
};
static void attach(CString& value,Manager& manager,const wchar_t* text,int refs=1) {
    auto* old=database_strings_test::data(value);if(old->Release())old->pStringMgr->Free(old);
    int length=int(std::wcslen(text));auto* data=manager.Allocate(length,sizeof(wchar_t));data->nDataLength=length;data->nRefs=refs;
    std::wmemcpy(data->data(),text,length+1);database_strings_test::attach(value,data);
}
static std::vector<INT64> exercise(const API& api) {
    std::vector<INT64> result;
    auto save=[&](const CString& value,Manager& first,Manager& second,Manager& third,HRESULT status) {
        auto* data=database_strings_test::data(value);result.insert(result.end(),{status,data->nDataLength,data->nAllocLength,data->nRefs,
            data->pStringMgr==&first?1:data->pStringMgr==&second?2:3});
        for(wchar_t ch:std::wstring(value.GetString(),value.GetLength()))result.push_back(ch);result.push_back(-1);
        for(auto* m:{&first,&second,&third})result.insert(result.end(),{m->allocations,m->frees,m->reallocations,m->clones});
    };
    for(bool locked:{false,true})for(bool differentClone:{false,true})for(bool fail:{false,true}) {
        Manager sourceManager,cloneManager,unused;sourceManager.clone=differentClone?&cloneManager:&sourceManager;
        CString source;attach(source,sourceManager,L"source",locked?-1:1);auto* original=database_strings_test::data(source);
        sourceManager.fail=cloneManager.fail=fail;
        alignas(CString) BYTE storage[16];std::memset(storage,0xa5,sizeof(storage));auto* target=reinterpret_cast<CString*>(storage);
        HRESULT status=api.copy(target,&source);bool allocates=locked || differentClone;
        check(status==(fail && allocates?E_OUTOFMEMORY:S_OK) && original->nRefs==(locked?-1:allocates?1:2),
            "CString copy construction clones locked or foreign-manager buffers and preserves source ownership on allocation failure");
        if(SUCCEEDED(status)) {
            check(*reinterpret_cast<UINT64*>(storage+8)==0xa5a5a5a5a5a5a5a5ULL,"CString copy construction stays within its native pointer-sized storage");
            save(*target,sourceManager,cloneManager,unused,status);api.destroy(target);
        }else save(source,sourceManager,cloneManager,unused,status);
        sourceManager.fail=cloneManager.fail=false;
        save(source,sourceManager,cloneManager,unused,status);
    }
    for(bool sameManager:{false,true})for(int sourceRefs:{1,-1})for(int targetRefs:{1,2,-1})for(bool fail:{false,true})for(bool longSource:{false,true}) {
        Manager sourceManager,targetManager,cloneManager;
        Manager& destination=sameManager?sourceManager:targetManager;
        CString source,target;attach(source,sourceManager,longSource?L"a considerably longer source":L"src",sourceRefs);attach(target,destination,L"old",targetRefs);
        auto* originalTarget=database_strings_test::data(target);auto* originalSource=database_strings_test::data(source);
        sourceManager.fail=targetManager.fail=cloneManager.fail=fail;
        HRESULT status=api.assign(&target,&source);
        if(FAILED(status))check(database_strings_test::data(target)==originalTarget && target==L"old" && originalSource->nRefs==sourceRefs,
            "CString assignment allocation failure preserves destination storage, text and source reference count");
        else {
            check(target==source.GetString() && (targetRefs!=-1 || (database_strings_test::data(target)->pStringMgr==&destination && database_strings_test::data(target)->nRefs==-1)),
                "CString assignment preserves locked destination ownership and copies source text");
            check(sameManager || database_strings_test::data(target)->pStringMgr==&destination,"CString assignment across managers retains the destination manager");
        }
        save(target,sourceManager,targetManager,cloneManager,status);
        sourceManager.fail=targetManager.fail=cloneManager.fail=false;
        // The fixture retains one reference to shared destination buffers.
        if(targetRefs==2 && originalTarget->Release())originalTarget->pStringMgr->Free(originalTarget);
    }
    for(bool sharedTarget:{false,true})for(bool sameManager:{false,true})for(bool fail:{false,true}) {
        Manager sourceManager,targetManager,cloneManager;Manager& destination=sameManager?sourceManager:targetManager;
        destination.clone=&cloneManager;
        CString source,target;attach(source,sourceManager,L"a longer source");attach(target,destination,L"old",sharedTarget?2:1);
        auto* originalTarget=database_strings_test::data(target);sourceManager.fail=targetManager.fail=cloneManager.fail=fail;
        HRESULT status=api.assign(&target,&source);
        if(FAILED(status))check(database_strings_test::data(target)==originalTarget && target==L"old","CString manager cloning failure preserves its old destination");
        else check(target==source.GetString() && database_strings_test::data(target)->pStringMgr==((sameManager || sharedTarget)?&cloneManager:&destination),
            "CString assignment honors the manager returned by cloning a source or a shared destination");
        save(target,sourceManager,targetManager,cloneManager,status);sourceManager.fail=targetManager.fail=cloneManager.fail=false;
        if(sharedTarget && originalTarget->Release())originalTarget->pStringMgr->Free(originalTarget);
    }
    for(int refs:{1,2,-1})for(int operation:{0,1,2,3,4,5}) {
        Manager manager,second,third;CString value;attach(value,manager,L"old string",refs);auto* original=database_strings_test::data(value);
        HRESULT status=operation==0?api.empty(&value):operation==1?api.set(&value,L""):operation==2?api.set(&value,L"a longer replacement"):
            operation==3?api.set(&value,value.GetString()+4):operation==4?api.assign(&value,&value):api.set(&value,nullptr);
        check(SUCCEEDED(status) && (refs!=-1 || database_strings_test::data(value)->nRefs==-1),"CString clearing, replacement, substring assignment and self-assignment preserve native locked-buffer state");
        save(value,manager,second,third,status);
        if(refs==2 && original->Release())original->pStringMgr->Free(original);
    }
    return result;
}
}
static void cstringOwnership() {
    using namespace cstring_ownership_test;
    API local{&copy,&assign,&set,&empty,&destroy};auto expected=exercise(local);
    wchar_t path[1024];::GetModuleFileNameW(nullptr,path,1024);auto* tail=std::wcsrchr(path,L'\\');
    if(tail)std::wcscpy(tail+1,L"native_string_fixture.dll");
    HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load independently compiled Microsoft ATL string ownership fixture");
    if(module) {
        API native{reinterpret_cast<Binary>(::GetProcAddress(module,"native_string_copy")),reinterpret_cast<Binary>(::GetProcAddress(module,"native_string_assign")),
            reinterpret_cast<Set>(::GetProcAddress(module,"native_string_set")),reinterpret_cast<Unary>(::GetProcAddress(module,"native_string_empty")),
            reinterpret_cast<Destroy>(::GetProcAddress(module,"native_string_destroy"))};
        bool complete=native.copy && native.assign && native.set && native.empty && native.destroy;check(complete,"Microsoft ATL fixture exports all ownership operations");
        if(complete)check(exercise(native)==expected,"CString copying, assignment, clearing, managers, reference counts and allocation failures match Microsoft ATL");
        ::FreeLibrary(module);
    }
}
