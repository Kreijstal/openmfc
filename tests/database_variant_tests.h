#pragma once
extern "C" void* MS_ABI impl___0CDBVariant__QEAA_XZ(void*);
extern "C" void MS_ABI impl___1CDBVariant__UEAA_XZ(CDBVariant*);
extern "C" void MS_ABI impl__Clear_CDBVariant__QEAAXXZ(CDBVariant*);
namespace database_variant_test {
struct API {
    decltype(&impl___0CDBVariant__QEAA_XZ) construct;
    decltype(&impl___1CDBVariant__UEAA_XZ) destroy;
    decltype(&impl__Clear_CDBVariant__QEAAXXZ) clear;
};
static int destroyed;
struct Binary:CLongBinary {~Binary() override{++destroyed;}};
static std::vector<UINT64> exercise(const API& functions) {
    std::vector<UINT64> result;
    alignas(CDBVariant) BYTE storage[40];
    auto read=[&](size_t offset,size_t length){UINT64 value=0;memcpy(&value,storage+offset,length);return value;};
    auto create=[&] {
        memset(storage,0xa5,sizeof(storage));auto* value=static_cast<CDBVariant*>(functions.construct(storage));
        check(value==reinterpret_cast<CDBVariant*>(storage) && value->m_dwType==DBVT_NULL && read(12,4)==0xa5a5a5a5 && read(16,8)==0xa5a5a5a5a5a5a5a5ULL &&
            read(24,8)==0xa5a5a5a5a5a5a5a5ULL,"CDBVariant construction writes only the native vtable and type, preserving union, padding and adjacent storage");
        return value;
    };
    auto save=[&](void* vtable,UINT64 expected,bool destructor) {
        check(read(8,4)==DBVT_NULL && read(16,8)==expected && read(12,4)==0xa5a5a5a5 && read(24,8)==0xa5a5a5a5a5a5a5a5ULL,
            "variant cleanup resets the type while preserving inactive union bits, padding and adjacent memory");
        check(read(0,8)==UINT_PTR(destructor?vtable:reinterpret_cast<void*>(0x12340000)),"qualified variant destruction restores its own vtable while Clear preserves the current vtable");
        result.insert(result.end(),{read(8,4),read(12,4),read(16,8)==expected,read(24,8),read(0,8)==UINT_PTR(vtable)});
    };
    for(DWORD type:{0ul,1ul,2ul,3ul,4ul,5ul,6ul,12ul,0xfffffffful})for(bool destructor:{false,true}) {
        auto* value=create();void* vtable=*reinterpret_cast<void**>(value);value->m_dwType=type;*reinterpret_cast<void**>(value)=reinterpret_cast<void*>(0x12340000);
        (destructor?functions.destroy:functions.clear)(value);save(vtable,0xa5a5a5a5a5a5a5a5ULL,destructor);
    }
    for(DWORD type:{DWORD(DBVT_STRING),DWORD(DBVT_ASTRING),DWORD(DBVT_WSTRING)})for(int refs:{-1,0,1,2,69})for(bool destructor:{false,true}) {
        auto* value=create();void* vtable=*reinterpret_cast<void**>(value);database_strings_test::Manager manager;
        auto* data=manager.Allocate(6,type==DBVT_ASTRING?1:2);data->nDataLength=6;data->nRefs=refs;
        auto** string=static_cast<void**>(::operator new(sizeof(void*)));*string=data+1;value->m_pstring=reinterpret_cast<CString*>(string);value->m_dwType=type;
        *reinterpret_cast<void**>(value)=reinterpret_cast<void*>(0x12340000);UINT64 pointer=UINT_PTR(string);
        (destructor?functions.destroy:functions.clear)(value);save(vtable,pointer,destructor);
        check(manager.frees==int(refs<=1),"variant strings release their actual ATL manager allocation only after the final reference, including locked strings");
        result.insert(result.end(),{UINT64(manager.frees),UINT64(refs>1?data->nRefs:0)});
        if(refs>1)manager.Free(data);
        if(!destructor){functions.clear(value);check(manager.frees==1,"clearing a NULL variant does not free its inactive string pointer again");}
    }
    for(bool destructor:{false,true})for(bool missing:{false,true}) {
        auto* value=create();void* vtable=*reinterpret_cast<void**>(value);destroyed=0;
        auto* binary=missing?nullptr:new Binary;HGLOBAL handle=nullptr;
        if(binary){binary->m_hData=handle=::GlobalAlloc(GMEM_MOVEABLE,16);binary->m_dwDataLength=16;}
        value->m_dwType=DBVT_BINARY;value->m_pbinary=binary;*reinterpret_cast<void**>(value)=reinterpret_cast<void*>(0x12340000);
        (destructor?functions.destroy:functions.clear)(value);save(vtable,UINT_PTR(binary),destructor);
        check(destroyed==int(!missing) && (!handle || ::GlobalFlags(handle)==GMEM_INVALID_HANDLE),"variant binary cleanup dispatches the actual virtual destructor and releases the owned HGLOBAL");
        result.push_back(destroyed);
    }
    for(bool destructor:{false,true})for(bool missing:{false,true}) {
        auto* value=create();void* vtable=*reinterpret_cast<void**>(value);
        auto* date=missing?nullptr:static_cast<TIMESTAMP_STRUCT*>(::operator new(sizeof(TIMESTAMP_STRUCT)));
        value->m_dwType=DBVT_DATE;value->m_pdate=date;*reinterpret_cast<void**>(value)=reinterpret_cast<void*>(0x12340000);
        (destructor?functions.destroy:functions.clear)(value);save(vtable,UINT_PTR(date),destructor);
    }
    for(bool freeStorage:{false,true}) {
        void* memory=::operator new(sizeof(CDBVariant));auto* value=static_cast<CDBVariant*>(functions.construct(memory));
        destroyed=0;value->m_dwType=DBVT_BINARY;value->m_pbinary=new Binary;
        using Delete=void* (MS_ABI*)(void*,UINT);auto** slots=*reinterpret_cast<void***>(value);
        void* returned=reinterpret_cast<Delete>(slots[0])(value,freeStorage?1:0);
        check(returned==memory && destroyed==1,"variant virtual deleting destructor releases its payload and follows the caller's storage-free flag");
        result.insert(result.end(),{returned==memory,UINT64(destroyed)});if(!freeStorage)::operator delete(memory);
    }
    return result;
}
}
static void databaseVariant() {
    using namespace database_variant_test;
    API local{&impl___0CDBVariant__QEAA_XZ,&impl___1CDBVariant__UEAA_XZ,&impl__Clear_CDBVariant__QEAAXXZ};
    auto expected=exercise(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_VARIANT_DLL",path,1024)) {
        auto module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for variant ownership comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"variant oracle uses the verified MFC image build");
            if(pinned) {
                API native{reinterpret_cast<decltype(local.construct)>(base+0x2043e0),reinterpret_cast<decltype(local.destroy)>(base+0x2044f0),reinterpret_cast<decltype(local.clear)>(base+0x204440)};
                auto actual=exercise(native);
                if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("variant mismatch %zu local %llu native %llu\n",i,expected[i],actual[i]);break;}
                check(actual==expected,"variant construction, scalar bits, string-manager ownership, binary virtual deletion and destructor flags match native MFC");
            }
            ::FreeLibrary(module);
        }
    }
}
