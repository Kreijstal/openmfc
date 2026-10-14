#pragma once
#include "detail/DatabaseEnvironment.h"
#include <array>
#include <climits>
extern "C" void MS_ABI impl__AllocConnect_CDatabase__IEAAXK_Z(CDatabase*,DWORD);
extern "C" void MS_ABI impl__Free_CDatabase__IEAAXXZ(CDatabase*);
extern "C" void MS_ABI impl__Close_CDatabase__UEAAXXZ(CDatabase*);
extern "C" BOOL MS_ABI impl__BeginTrans_CDatabase__QEAAHXZ(CDatabase*);
extern "C" BOOL MS_ABI impl__CommitTrans_CDatabase__QEAAHXZ(CDatabase*);
extern "C" BOOL MS_ABI impl__Rollback_CDatabase__QEAAHXZ(CDatabase*);
extern "C" void MS_ABI impl__Cancel_CDatabase__QEAAXXZ(CDatabase*);
extern "C" void MS_ABI impl___1CDatabase__UEAA_XZ(CDatabase*);
namespace database_lifecycle_test {
struct API {
    decltype(&impl__AllocConnect_CDatabase__IEAAXK_Z) allocate;
    decltype(&impl__Free_CDatabase__IEAAXXZ) free;
    decltype(&impl__Close_CDatabase__UEAAXXZ) close;
    decltype(&impl__BeginTrans_CDatabase__QEAAHXZ) begin,commit,rollback;
    decltype(&impl__Cancel_CDatabase__QEAAXXZ) cancel;
};
using Event=std::array<INT64,6>;
static std::vector<Event> events;
static CRITICAL_SECTION* nativeLock;
static const API* active;
static RETCODE envResult,connectionResult,attributeResult,transactionResult;
static BOOL accepted=53;
static bool failCheck,failRecordset;
static int closeException,deleted;
static int derivedDestructions;
static void* MS_ABI derivedDestructor(CDatabase* object,UINT) {++derivedDestructions;return object;}
static CRecordset* recordsets[2];
static int depth() {return nativeLock?nativeLock->RecursionCount:g_databaseLockDepth;}
static void event(int kind,UINT_PTR handle=0,INT64 value=0,INT64 option=0,INT64 size=0) {
    events.push_back({kind,INT64(handle),value,option,size,depth()});
}
static BOOL MS_ABI checkResult(const CDatabase*,RETCODE code) {
    event(50,UINT_PTR(WORD(code)),accepted);
    if(failCheck)throw 77;
    return accepted;
}
static void MS_ABI throwResult(CDatabase*,RETCODE code) {event(51,UINT_PTR(WORD(code)));throw int(code)+1000;}
struct DeleteProbe: CException {~DeleteProbe() override {++deleted;event(53);}};
static void MS_ABI closeDatabase(CDatabase* database) {
    event(52,UINT_PTR(database->m_hdbc));
    if(closeException==1)throw static_cast<CException*>(new DeleteProbe);
    if(closeException==2)throw 81;
    active->close(database);
}
static void MS_ABI closeRecordset(CRecordset* recordset) {
    event(54,recordset==recordsets[0]?1:2);
    if(failRecordset)throw static_cast<CException*>(new DeleteProbe);
    auto* list=reinterpret_cast<OpenMFC_CPtrList*>(recordset->m_pDatabase->m_listRecordsets);
    list->RemoveAt(list->Find(recordset,nullptr));
}
static void reset() {events.clear();envResult=connectionResult=attributeResult=transactionResult=SQL_SUCCESS;accepted=53;failCheck=failRecordset=false;closeException=deleted=0;}
std::vector<Event> exercise(const API& api,HENV& environment,LONG& connections,CRITICAL_SECTION* lock=nullptr) {
    nativeLock=lock;active=&api;auto originalEnvironment=environment;LONG originalConnections=connections;
    environment=nullptr;connections=0;
    CDatabase database;auto** original=*reinterpret_cast<void***>(&database);void* table[12];memcpy(table,original,sizeof(table));
    table[7]=reinterpret_cast<void*>(&closeDatabase);table[10]=reinterpret_cast<void*>(&checkResult);table[11]=reinterpret_cast<void*>(&throwResult);
    *reinterpret_cast<void***>(&database)=table;
    std::vector<Event> result;
    auto save=[&] {result.insert(result.end(),events.begin(),events.end());result.push_back({90,INT64(UINT_PTR(environment)),connections,database.m_bIncRecordCountOnAdd,depth(),0});};
    for(bool existing:{false,true})for(BOOL updatable:{0,17})for(DWORD options:{DWORD(0),DWORD(CDatabase::useCursorLib)}) {
        environment=existing?reinterpret_cast<HENV>(0xabc):nullptr;connections=existing?4:0;
        database.m_hdbc=nullptr;database.m_bUpdatable=updatable;database.m_bIncRecordCountOnAdd=55;database.m_dwLoginTimeout=27;
        reset();connectionResult=SQL_SUCCESS_WITH_INFO;attributeResult=SQL_ERROR;
        api.allocate(&database,options);
        check(database.m_hdbc==reinterpret_cast<HDBC>(0xdef) && environment==reinterpret_cast<HENV>(0xabc) && connections==(existing?5:1) && !depth() &&
            database.m_bIncRecordCountOnAdd==(options?1:55),
            "database allocation shares the environment, checks allocation status, counts connections and caches cursor-library behavior");
        size_t calls=events.size();api.allocate(&database,options);
        check(events.size()==calls,"database allocation with an existing connection preserves handles, count and options without ODBC calls");save();
        if(options)check(events.back()[0]==3 && events.back()[3]==SQL_ODBC_CURSORS && events.back()[2]==2,
            "database cursor-library option preserves the native driver-cursor attribute value");
        events.clear();api.close(&database);
        check(!database.m_hdbc && connections==(existing?4:0) && environment==reinterpret_cast<HENV>(0xabc) && !depth(),
            "database Close disconnects and frees its connection while retaining the shared environment");save();
        events.clear();api.free(&database);
        check(environment==(existing?reinterpret_cast<HENV>(0xabc):nullptr) && !depth(),
            "database Free invokes virtual Close and releases the environment only when no connections remain");save();
    }
    environment=nullptr;connections=0;reset();envResult=SQL_ERROR;accepted=FALSE;bool memory=false;
    try {api.allocate(&database,0);}catch(const std::bad_alloc&){memory=true;}catch(CException* error){memory=true;error->Delete();}
    check(memory && !environment && !database.m_hdbc && !connections && !depth(),"database environment allocation failures release the ODBC lock before throwing memory exceptions");save();
    environment=reinterpret_cast<HENV>(0xabc);connections=2;reset();connectionResult=SQL_ERROR;accepted=FALSE;int code=0;
    try {api.allocate(&database,0);}catch(int value){code=value;}
    check(code==999 && !database.m_hdbc && connections==2 && !depth() && events.back()[0]==51 && !events.back()[5],
        "database connection allocation failures preserve the shared environment and throw through the unlocked virtual callback");save();
    environment=reinterpret_cast<HENV>(0xabc);connections=LONG_MAX;reset();api.allocate(&database,0);
    check(connections==LONG_MIN,"database connection accounting preserves native 32-bit wraparound");api.close(&database);save();
    environment=nullptr;connections=0;database.m_hdbc=nullptr;
    {
        recordset_storage_test::Fixture first(0,0),second(0,0);recordsets[0]=first.recordset;recordsets[1]=second.recordset;
        void* slots[27]{};slots[6]=reinterpret_cast<void*>(&closeRecordset);
        auto* list=reinterpret_cast<OpenMFC_CPtrList*>(database.m_listRecordsets);
        for(auto* recordset:recordsets) {*reinterpret_cast<void***>(recordset)=slots;recordset->m_pDatabase=&database;list->AddTail(recordset);}
        reset();api.close(&database);
        check(!list->m_nCount && !first.recordset->m_pDatabase && !second.recordset->m_pDatabase && events.size()==2 && events[0][1]==1 && events[1][1]==2 && events[0][5]==1 && !depth(),
            "database Close drains registered recordsets through their virtual callbacks under the ODBC lock and clears their owner pointers");save();
        first.recordset->m_pDatabase=&database;list->AddTail(first.recordset);reset();failRecordset=true;bool caught=false;
        try {api.close(&database);}catch(CException* error){caught=true;error->Delete();}
        check(caught && !depth() && list->m_nCount==1 && first.recordset->m_pDatabase==&database,
            "database Close unwinds its ODBC lock and preserves registration when a recordset close callback throws");save();
        list->RemoveAll();first.recordset->m_pDatabase=nullptr;failRecordset=false;
    }
    for(int exception:{0,1,2}) {
        environment=reinterpret_cast<HENV>(0xabc);connections=0;database.m_blobConnect.cbData=23;
        database.m_blobConnect.pbData=static_cast<BYTE*>(::LocalAlloc(LMEM_FIXED,23));reset();closeException=exception;bool propagated=false;
        try {api.free(&database);}catch(int value){propagated=value==81;}
        check(!depth() && (exception==2?propagated && database.m_blobConnect.pbData && environment:!propagated && !database.m_blobConnect.pbData && !environment && deleted==(exception==1)) &&
            database.m_blobConnect.cbData==23,"database Free consumes framework exceptions, preserves other exceptions and frees encrypted connection storage without changing its size field");save();
        if(propagated){closeException=0;api.free(&database);}
    }
    environment=reinterpret_cast<HENV>(0xabc);connections=0;database.m_hdbc=reinterpret_cast<HDBC>(0xdef);
    for(BOOL capable:{0,37})for(RETCODE status:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR)})for(BOOL validation:{0,53}) {
        database.m_bTransactions=capable;
        for(auto method:{api.begin,api.commit,api.rollback}) {
            reset();accepted=validation;attributeResult=transactionResult=status;
            BOOL returned=method(&database);
            check(returned==(capable?validation:0) && (capable?!events.empty():events.empty()) && !depth(),
                "database transactions use the cached capability and preserve the full native virtual Check result");save();
        }
    }
    reset();database.m_bTransactions=TRUE;failCheck=true;bool caught=false;
    try {api.commit(&database);}catch(int value){caught=value==77;}
    check(caught && events.size()==2,"database transaction callback exceptions skip the subsequent autocommit reset");save();
    for(HSTMT statement:{HSTMT(nullptr),reinterpret_cast<HSTMT>(0xcafe)})for(HDBC connection:{HDBC(nullptr),reinterpret_cast<HDBC>(0xdef)}) {
        reset();database.m_hstmt=statement;database.m_hdbc=connection;api.cancel(&database);
        check(events.size()==1 && events[0][0]==8 && events[0][1]==INT64(UINT_PTR(statement)),
            "database cancellation calls SQLCancel with the statement handle even when either handle is null");save();
    }
    database.m_hstmt=database.m_hdbc=nullptr;database.m_bTransactions=FALSE;reset();
    *reinterpret_cast<void***>(&database)=original;environment=originalEnvironment;connections=originalConnections;nativeLock=nullptr;active=nullptr;
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLAllocEnv(SQLHENV* environment) {
    using namespace database_lifecycle_test;event(1,0,envResult);*environment=envResult==SQL_ERROR?nullptr:reinterpret_cast<HENV>(0xabc);return envResult;
}
extern "C" SQLRETURN SQL_API SQLAllocConnect(SQLHENV environment,SQLHDBC* connection) {
    using namespace database_lifecycle_test;event(2,UINT_PTR(environment),connectionResult);*connection=connectionResult==SQL_ERROR?nullptr:reinterpret_cast<HDBC>(0xdef);return connectionResult;
}
extern "C" SQLRETURN SQL_API SQLSetConnectAttr(SQLHDBC connection,SQLINTEGER option,SQLPOINTER value,SQLINTEGER size) {
    using namespace database_lifecycle_test;event(3,UINT_PTR(connection),INT64(UINT_PTR(value)),option,size);return attributeResult;
}
extern "C" SQLRETURN SQL_API SQLDisconnect(SQLHDBC connection) {database_lifecycle_test::event(4,UINT_PTR(connection));return SQL_ERROR;}
extern "C" SQLRETURN SQL_API SQLFreeConnect(SQLHDBC connection) {database_lifecycle_test::event(5,UINT_PTR(connection));return SQL_ERROR;}
extern "C" SQLRETURN SQL_API SQLFreeEnv(SQLHENV environment) {database_lifecycle_test::event(6,UINT_PTR(environment));return SQL_ERROR;}
extern "C" SQLRETURN SQL_API SQLTransact(SQLHENV environment,SQLHDBC connection,SQLUSMALLINT operation) {
    using namespace database_lifecycle_test;event(7,UINT_PTR(environment),INT64(UINT_PTR(connection)),operation);return transactionResult;
}
extern "C" SQLRETURN SQL_API SQLCancel(SQLHSTMT statement) {database_lifecycle_test::event(8,UINT_PTR(statement));return SQL_ERROR;}
static void databaseLifecycle() {
    using namespace database_lifecycle_test;
    API local{&impl__AllocConnect_CDatabase__IEAAXK_Z,&impl__Free_CDatabase__IEAAXXZ,&impl__Close_CDatabase__UEAAXXZ,
        &impl__BeginTrans_CDatabase__QEAAHXZ,&impl__CommitTrans_CDatabase__QEAAHXZ,&impl__Rollback_CDatabase__QEAAHXZ,&impl__Cancel_CDatabase__QEAAXXZ};
    auto expected=exercise(local,OpenMfcDatabaseEnvironment(),OpenMfcAllocatedDatabaseConnections());
    alignas(CDatabase) BYTE storage[168];memset(storage,0xa5,sizeof(storage));auto* database=new(storage) CDatabase;
    void* foreignTable[12];memcpy(foreignTable,*reinterpret_cast<void***>(database),sizeof(foreignTable));foreignTable[1]=reinterpret_cast<void*>(&derivedDestructor);
    *reinterpret_cast<void***>(database)=foreignTable;derivedDestructions=0;impl___1CDatabase__UEAA_XZ(database);
    check(!derivedDestructions && *reinterpret_cast<UINT64*>(storage+160)==0xa5a5a5a5a5a5a5a5ULL,
        "database base destructor entry bypasses a foreign subclass deleting destructor and preserves the caller allocation guard");
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_LIFECYCLE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for database lifecycle comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"native lifecycle oracle uses the verified MFC image build");
            if(!pinned){::FreeLibrary(module);return;}
            reinterpret_cast<HENV (MS_ABI*)()>(base+0x1f6ae0)();
            auto* state=*reinterpret_cast<BYTE**>(base+0x3c4160);
            struct Patch {size_t rva;void* function;void* saved;DWORD protection;};
            Patch patches[]={{0x3e90e8,reinterpret_cast<void*>(&SQLAllocEnv)},{0x3e9110,reinterpret_cast<void*>(&SQLAllocConnect)},
                {0x3e9030,reinterpret_cast<void*>(&SQLSetConnectAttr)},{0x3e9098,reinterpret_cast<void*>(&SQLDisconnect)},
                {0x3e9090,reinterpret_cast<void*>(&SQLFreeConnect)},{0x3e9088,reinterpret_cast<void*>(&SQLFreeEnv)},
                {0x3e9128,reinterpret_cast<void*>(&SQLTransact)},{0x3e90a8,reinterpret_cast<void*>(&SQLCancel)}};
            for(auto& patch:patches){auto** slot=reinterpret_cast<void**>(base+patch.rva);patch.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&patch.protection);*slot=patch.function;}
            API native{reinterpret_cast<decltype(local.allocate)>(base+0x1f76a0),reinterpret_cast<decltype(local.free)>(base+0x1f7160),
                reinterpret_cast<decltype(local.close)>(base+0x1f70a0),reinterpret_cast<decltype(local.begin)>(base+0x1f7470),
                reinterpret_cast<decltype(local.commit)>(base+0x1f74c0),reinterpret_cast<decltype(local.rollback)>(base+0x1f7540),reinterpret_cast<decltype(local.cancel)>(base+0x1f7090)};
            auto actual=exercise(native,*reinterpret_cast<HENV*>(state+8),*reinterpret_cast<LONG*>(state+16),reinterpret_cast<CRITICAL_SECTION*>(base+0x3c3d48));
            check(actual==expected,
                "database allocation, cleanup, callback ordering, locks, transactions and cancellation match native MFC");
            memset(storage,0xa5,sizeof(storage));
            database=reinterpret_cast<CDatabase* (MS_ABI*)(void*)>(base+0x1f6b10)(storage);
            memcpy(foreignTable,*reinterpret_cast<void***>(database),sizeof(foreignTable));foreignTable[1]=reinterpret_cast<void*>(&derivedDestructor);
            *reinterpret_cast<void***>(database)=foreignTable;derivedDestructions=0;
            reinterpret_cast<void (MS_ABI*)(CDatabase*)>(base+0x1f6ba0)(database);
            check(!derivedDestructions && *reinterpret_cast<UINT64*>(storage+160)==0xa5a5a5a5a5a5a5a5ULL,
                "native database base destruction bypasses the foreign subclass deleting destructor and preserves the allocation guard");
            for(auto& patch:patches){auto** slot=reinterpret_cast<void**>(base+patch.rva);*slot=patch.saved;DWORD ignored;::VirtualProtect(slot,8,patch.protection,&ignored);}::FreeLibrary(module);
        }
    }
}
