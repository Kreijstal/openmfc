#pragma once
extern "C" void* MS_ABI impl___0CRecordset__QEAA_PEAVCDatabase___Z(void*,CDatabase*);
namespace recordset_constructor_test {
using Constructor=decltype(&impl___0CRecordset__QEAA_PEAVCDatabase___Z);
static RETCODE allocationResult;
static bool emptyStatement;
static int throwAllocation,deleted;
struct AllocationException: CDBException {
    ~AllocationException() override {++deleted;}
};
static int allocations,allocationDepth;
static SQLHDBC allocatedDatabase;
static SQLRETURN SQL_API allocate(SQLHDBC database,SQLHSTMT* statement) {
    ++allocations;allocatedDatabase=database;allocationDepth=g_databaseLockDepth;
    if(throwAllocation==1)throw 73;
    if(throwAllocation==2)throw static_cast<CDBException*>(new AllocationException);
    if(throwAllocation==3)throw static_cast<CException*>(new CException);
    *statement=emptyStatement?nullptr:reinterpret_cast<HSTMT>(0xbeef);
    return allocationResult;
}
static std::vector<BYTE> exercise(Constructor construct) {
    CDatabase database;
    auto* list=reinterpret_cast<OpenMFC_CPtrList*>(database.m_listRecordsets);
    std::vector<BYTE> result;
    const size_t stringOffsets[]={24,32,264,336,344,352,416,424,432};
    auto save=[&](CRecordset* recordset) {
        auto* bytes=reinterpret_cast<BYTE*>(recordset);
        // Compare every scalar, map field and untouched padding byte. Vtables,
        // owner pointers and CString manager addresses are image-specific.
        for(size_t i=0;i<sizeof(CRecordset);++i) {
            bool omit=i<8 || (i>=16 && i<24) || (i>=96 && i<104) || (i>=152 && i<160);
            for(size_t offset:stringOffsets)omit=omit || (i>=offset && i<offset+8);
            if(!omit)result.push_back(bytes[i]);
        }
        for(size_t offset:stringOffsets)
            check(reinterpret_cast<CString*>(bytes+offset)->IsEmpty(),"recordset construction initializes every native CString member to an empty value");
    };
    for(int connection:{0,1,2})for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NEED_DATA),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR),RETCODE(SQL_INVALID_HANDLE),RETCODE(SQL_STILL_EXECUTING)})for(bool nullStatement:{false,true}) {
        alignas(CRecordset) BYTE storage[sizeof(CRecordset)+8];std::memset(storage,0xa5,sizeof(storage));
        auto* recordset=reinterpret_cast<CRecordset*>(storage);
        allocations=0;allocationDepth=-1;allocatedDatabase=nullptr;allocationResult=code;emptyStatement=nullStatement;throwAllocation=false;
        database.m_hdbc=connection==2?reinterpret_cast<HDBC>(0xface):nullptr;
        void* sentinel=reinterpret_cast<void*>(0x1234);list->AddTail(sentinel);
        bool failed=false;RETCODE error=0;void* returned=nullptr;
        try {returned=construct(storage,connection?&database:nullptr);}
        catch(CDBException* exception){failed=true;error=exception->m_nRetCode;exception->Delete();}
        bool accepted=connection!=2 || code==SQL_SUCCESS || code==SQL_SUCCESS_WITH_INFO || code==SQL_NEED_DATA || code==SQL_NO_DATA;
        check(!failed && !error,"recordset constructor consumes database allocation exceptions and returns without retrying rejected status");
        check(allocations==(connection==2) && (!allocations || (allocatedDatabase==database.m_hdbc && allocationDepth==0)) && !g_databaseLockDepth,
            "recordset construction allocates only for an existing database connection before acquiring the registration lock");
        check(list->m_nCount==(accepted && connection==2?2:1) && list->m_pNodeHead->data==(accepted && connection==2?static_cast<void*>(recordset):sentinel) && list->m_pNodeTail->data==sentinel,
            "recordset construction inserts at the database-list head only after accepted statement allocation");
        check(std::all_of(storage+sizeof(CRecordset),storage+sizeof(storage),[](BYTE value){return value==0xa5;}),"recordset construction stays inside its native 440-byte allocation");
        result.push_back(BYTE(failed));result.push_back(BYTE(allocations));result.push_back(BYTE(list->m_nCount));
        if(!failed) {
            check(returned==recordset && recordset->m_pDatabase==(connection?&database:nullptr) && recordset->m_hstmt==(connection==2 && !nullStatement?reinterpret_cast<HSTMT>(0xbeef):nullptr),
                "recordset construction returns its placement address and preserves the database and driver-provided statement");
            save(recordset);
            // Release through the constructed image's own deleting destructor.
            // Clearing the statement avoids exercising Close in this constructor test.
            recordset->m_hstmt=nullptr;recordset->m_pDatabase=nullptr;
            auto** table=*reinterpret_cast<void***>(recordset);
            reinterpret_cast<void* (MS_ABI*)(void*,UINT)>(table[1])(recordset,0);
        }
        list->RemoveAll();database.m_hdbc=nullptr;
    }
    alignas(CRecordset) BYTE storage[sizeof(CRecordset)];std::memset(storage,0xa5,sizeof(storage));
    database.m_hdbc=reinterpret_cast<HDBC>(0xface);allocations=0;throwAllocation=true;bool foreign=false;
    try {construct(storage,&database);}catch(int value){foreign=value==73;}
    check(foreign && allocations==1 && !list->m_nCount && !g_databaseLockDepth,"recordset constructor propagates allocator exceptions without registration or an outstanding ODBC lock");
    std::memset(storage,0xa5,sizeof(storage));allocations=deleted=0;throwAllocation=2;
    bool databaseException=false;
    try {construct(storage,&database);}catch(CDBException* exception){databaseException=true;exception->Delete();}
    check(databaseException && deleted==1 && allocations==1 && !list->m_nCount && !g_databaseLockDepth,
        "recordset constructor propagates database exceptions thrown by the allocator before its status and registration catch scope");
    std::memset(storage,0xa5,sizeof(storage));throwAllocation=3;bool framework=false;
    try {construct(storage,&database);}catch(CException* exception){framework=true;exception->Delete();}
    check(framework && !list->m_nCount && !g_databaseLockDepth,"recordset constructor propagates framework exceptions thrown by the allocator without registering");
    database.m_hdbc=nullptr;throwAllocation=false;
    return result;
}
}
static void recordsetConstructor() {
    using namespace recordset_constructor_test;
    auto previous=database_execute_test::allocationHook;database_execute_test::allocationHook=&allocate;
    auto expected=exercise(&impl___0CRecordset__QEAA_PEAVCDatabase___Z);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_CONSTRUCTOR_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset constructor comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"recordset constructor oracle uses the verified MFC image build");
            if(pinned) {
                auto** slot=reinterpret_cast<void**>(base+0x3e90d0);void* saved=*slot;DWORD protection;
                ::VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=reinterpret_cast<void*>(&SQLAllocStmt);
                auto actual=exercise(reinterpret_cast<Constructor>(base+0x1f7e40));
                check(actual==expected,"recordset constructor scalar defaults, padding, allocation status and database-list registration match native MFC");
                *slot=saved;DWORD ignored;::VirtualProtect(slot,8,protection,&ignored);
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::allocationHook=previous;
}
