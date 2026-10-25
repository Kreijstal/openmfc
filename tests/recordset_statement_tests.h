#pragma once
extern "C" BOOL MS_ABI impl__AllocHstmt_CRecordset__IEAAHXZ(CRecordset*);
extern "C" void MS_ABI impl__SetState_CRecordset__IEAAXHPEB_WK_Z(CRecordset*,int,const wchar_t*,DWORD);
namespace recordset_statement_test {
struct API {
    decltype(&impl__AllocHstmt_CRecordset__IEAAHXZ) allocate;
    decltype(&impl__SetState_CRecordset__IEAAXHPEB_WK_Z) state;
};
using Event=std::array<INT64,8>;
static std::vector<Event> events;
static void event(INT64 kind,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0){events.push_back({kind,a,b,c,d,e,f,g});}
static void text(std::vector<INT64>& result,const CString& value) {
    result.push_back(value.GetLength());for(int i=0;i<value.GetLength();++i)result.push_back(value.GetString()[i]);
}
static std::vector<INT64> states(const API& api) {
    std::vector<INT64> result;
    for(int type:{-1,0,1,2,3,17})for(DWORD options:{0ul,4ul,8ul,12ul,0x20ul,0x200ul,0x204ul,0x208ul,0x800ul,0xfffffffful})for(BOOL cache:{FALSE,BOOL(69)})for(int input:{0,1,2}) {
        CRecordset rs;rs.m_nDefaultType=3;rs.m_nOpenType=73;rs.m_dwOptions=91;rs.m_lOpen=37;
        rs.m_bAppendable=27;rs.m_bUpdatable=28;rs.m_bCheckCacheForDirtyFields=cache;
        rs.m_strSQL=L"main SQL";rs.m_strFilter=L"filter";rs.m_strSort=L"sort";
        rs.m_strRequerySQL=L"prefix suffix";rs.m_strRequeryFilter=L"old filter";rs.m_strRequerySort=L"old sort";
        const wchar_t* sql=input==0?nullptr:input==1?L"select fixture":rs.m_strRequerySQL.GetString()+7;
        api.state(&rs,type,sql,options);
        check(rs.m_nOpenType==UINT(type==-1?3:type) && rs.m_lOpen==37 && !wcscmp(rs.m_strSQL.GetString(),L"main SQL"),
            "SetState resolves default types without changing open state or the primary SQL string");
        check(rs.m_bAppendable==BOOL(!(options&4) || (options&8)) && rs.m_bUpdatable==BOOL(!(options&12)) &&
            rs.m_bCheckCacheForDirtyFields==((options&0x220)?FALSE:cache),
            "SetState computes write capabilities before forcing forward-only read-only flags and preserves sticky cache state");
        check(!wcscmp(rs.m_strRequerySQL.GetString(),input==0?L"":input==1?L"select fixture":L"suffix") &&
            !wcscmp(rs.m_strRequeryFilter.GetString(),L"filter") && !wcscmp(rs.m_strRequerySort.GetString(),L"sort"),
            "SetState stores null, literal and aliased SQL input and snapshots the filter and sort strings");
        result.insert(result.end(),{rs.m_nOpenType,rs.m_dwOptions,rs.m_lOpen,rs.m_bAppendable,rs.m_bUpdatable,rs.m_bCheckCacheForDirtyFields});
        text(result,rs.m_strRequerySQL);text(result,rs.m_strRequeryFilter);text(result,rs.m_strRequerySort);
    }
    // Custom managers expose the native copy/assignment and locked-buffer paths.
    for(bool locked:{false,true})for(bool cloneElsewhere:{false,true}) {
        database_strings_test::Manager source,target,other;source.clone=cloneElsewhere?&other:&source;
        CRecordset rs;auto* oldFilter=database_strings_test::data(rs.m_strFilter);auto* oldQuery=database_strings_test::data(rs.m_strRequerySQL);
        auto* filter=source.Allocate(6,2);filter->nDataLength=6;filter->nRefs=locked?-1:1;memcpy(filter+1,L"filter",14);
        auto* query=target.Allocate(32,2);query->nDataLength=3;query->nRefs=locked?-1:1;memcpy(query+1,L"old",8);
        database_strings_test::attach(rs.m_strFilter,filter);database_strings_test::attach(rs.m_strRequerySQL,query);
        api.state(&rs,1,L"select fixture",0);
        auto* output=database_strings_test::data(rs.m_strRequeryFilter);auto* sql=database_strings_test::data(rs.m_strRequerySQL);
        result.insert(result.end(),{source.clones,source.allocations,other.allocations,target.allocations,target.reallocations,
            output==filter,output->nRefs,sql==query,sql->nRefs,sql->pStringMgr==&target});
        text(result,rs.m_strRequerySQL);text(result,rs.m_strRequeryFilter);
        rs.m_strRequeryFilter.Empty();
        // Restore detached source storage before its manager leaves scope.
        database_strings_test::attach(rs.m_strFilter,oldFilter);filter->nRefs=1;source.Free(filter);
        rs.m_strRequerySQL.~CString();new(&rs.m_strRequerySQL) CString;
        auto* replacement=database_strings_test::data(rs.m_strRequerySQL);replacement->Release();
        database_strings_test::attach(rs.m_strRequerySQL,oldQuery);
    }
    return result;
}
struct Script {
    bool existingStatement=false,missingDatabase=false,connected=false;
    UINT type=CRecordset::snapshot;DWORD options=0;BOOL cursors=FALSE;
    BOOL firstOpen=69,secondOpen=69,acceptInfo=69,acceptAllocation=69;
    RETCODE infoCode=SQL_SUCCESS,allocationCode=SQL_SUCCESS;
    SQLUINTEGER scroll=SQL_SO_STATIC;
    bool returningThrow=false;int failure=0;
};
static Script script;
static int opens,checks,allocations,infos;
static CRecordset* active;
static void** ownedTable;
static void* databaseTable[12];
static void fail(int kind){if(script.failure==kind)throw 400+kind;}
static BOOL MS_ABI open(CDatabase* db,const wchar_t* dsn,BOOL exclusive,BOOL readOnly,const wchar_t* connect,BOOL cursors) {
    ++opens;event(2,opens,dsn && !*dsn,exclusive,readOnly,connect && !wcscmp(connect,L"ODBC;DSN=fixture;"),cursors,active->m_bRecordsetDb);
    fail(opens==1?2:4);db->m_hdbc=reinterpret_cast<HDBC>(0xface);return opens==1?script.firstOpen:script.secondOpen;
}
static void MS_ABI close(CDatabase* db){event(3,INT64(UINT_PTR(db->m_hdbc)));fail(3);db->m_hdbc=nullptr;}
static void configure(CDatabase* db) {
    auto** original=*reinterpret_cast<void***>(db);memcpy(databaseTable,original,sizeof(databaseTable));
    databaseTable[5]=reinterpret_cast<void*>(&open);databaseTable[7]=reinterpret_cast<void*>(&close);
    *reinterpret_cast<void***>(db)=databaseTable;
    if(script.missingDatabase)ownedTable=original;
}
static CString* MS_ABI connect(CRecordset* rs,CString* output) {
    event(1,rs->m_pDatabase!=nullptr,rs->m_bRecordsetDb);if(script.missingDatabase)configure(rs->m_pDatabase);
    fail(1);return new(output) CString(L"ODBC;DSN=fixture;");
}
static SQLRETURN SQL_API info(SQLHDBC connection,SQLUSMALLINT kind,SQLPOINTER output,SQLSMALLINT capacity,SQLSMALLINT* bytes) {
    ++infos;event(5,INT64(UINT_PTR(connection)),kind,capacity,bytes!=nullptr);fail(5);
    check(kind==SQL_SCROLL_OPTIONS && capacity==4,"statement allocation asks for the driver's four-byte scroll-options capability mask");
    *static_cast<SQLUINTEGER*>(output)=script.scroll;*bytes=4;return script.infoCode;
}
static SQLRETURN SQL_API allocate(SQLHDBC connection,SQLHSTMT* statement) {
    ++allocations;event(6,INT64(UINT_PTR(connection)),statement==&active->m_hstmt);fail(6);
    *statement=reinterpret_cast<HSTMT>(0xbeef);return script.allocationCode;
}
static BOOL MS_ABI checkStatus(const CRecordset*,RETCODE code) {
    ++checks;event(7,code);fail(allocations?8:7);return allocations?script.acceptAllocation:script.acceptInfo;
}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT statement) {
    event(8,code,INT64(UINT_PTR(statement)));fail(9);if(!script.returningThrow)throw 700+code;
}
static std::vector<Event> statements(const API& api) {
    std::vector<Event> result;
    auto run=[&](const Script& next) {
        script=next;opens=checks=allocations=infos=0;ownedTable=nullptr;events.clear();
        CDatabase database;CRecordset rs;active=&rs;
        auto** originalDatabase=*reinterpret_cast<void***>(&database);auto** originalRecordset=*reinterpret_cast<void***>(&rs);
        void* table[27];memcpy(table,originalRecordset,sizeof(table));table[15]=reinterpret_cast<void*>(&connect);
        table[21]=reinterpret_cast<void*>(&checkStatus);table[24]=reinterpret_cast<void*>(&throwStatus);*reinterpret_cast<void***>(&rs)=table;
        if(!script.missingDatabase){configure(&database);rs.m_pDatabase=&database;database.m_hdbc=script.connected?reinterpret_cast<HDBC>(0xface):nullptr;}
        rs.m_hstmt=script.existingStatement?reinterpret_cast<HSTMT>(0xcafe):nullptr;
        rs.m_nOpenType=script.type;rs.m_dwOptions=script.options;rs.m_bUseODBCCursorLib=script.cursors;
        int caught=0;BOOL returned=-1;
        try{returned=api.allocate(&rs);}catch(int error){caught=error;}
        auto* db=rs.m_pDatabase;auto* list=db?reinterpret_cast<OpenMFC_CPtrList*>(db->m_listRecordsets):nullptr;
        bool registered=list && list->m_nCount && list->m_pNodeHead->data==&rs;
        check(!allocations || allocations==1,"statement allocation calls the driver once even for SQL_STILL_EXECUTING");
        if(script.existingStatement)check(returned==TRUE && !caught && events.empty() && !registered,
            "an existing statement bypasses default connection lookup, database creation, ODBC and registration");
        if(returned==TRUE && !script.existingStatement)check(registered && list->m_nCount==1 && rs.m_hstmt==reinterpret_cast<HSTMT>(0xbeef),
            "successful statement allocation registers the instance once, including accepted failures and returning exception hooks");
        result.insert(result.end(),events.begin(),events.end());
        result.push_back({90,returned,caught,rs.m_bRecordsetDb,INT64(UINT_PTR(rs.m_hstmt)),list?list->m_nCount:0,registered,db!=nullptr});
        if(list)list->RemoveAll();
        if(db)db->m_hdbc=nullptr;
        rs.m_hstmt=nullptr;rs.m_pDatabase=nullptr;rs.m_bRecordsetDb=FALSE;
        *reinterpret_cast<void***>(&rs)=originalRecordset;*reinterpret_cast<void***>(&database)=originalDatabase;
        if(db && db!=&database) {
            *reinterpret_cast<void***>(db)=ownedTable;
            using Destroy=void* (MS_ABI*)(CDatabase*,UINT);reinterpret_cast<Destroy>(ownedTable[1])(db,1);
        }
    };
    for(bool missing:{false,true})for(bool existing:{false,true})for(bool connected:{false,true}) {
        Script s;s.missingDatabase=missing;s.existingStatement=existing;s.connected=connected;run(s);
    }
    for(UINT type:{0u,1u,2u,3u})for(DWORD options:{0ul,4ul,8ul})for(BOOL cursors:{FALSE,TRUE,BOOL(69)})for(SQLUINTEGER scroll:{SQLUINTEGER(0),SQLUINTEGER(SQL_SO_STATIC),SQLUINTEGER(0xffffffff)}) {
        Script s;s.type=type;s.options=options;s.cursors=cursors;s.scroll=scroll;run(s);
    }
    for(bool missing:{false,true})for(bool second:{false,true}) {
        Script s;s.missingDatabase=missing;s.options=4;s.scroll=0;if(second)s.secondOpen=FALSE;else s.firstOpen=FALSE;run(s);
    }
    for(bool query:{false,true})for(bool returns:{false,true})for(RETCODE code:{RETCODE(SQL_ERROR),RETCODE(SQL_INVALID_HANDLE),RETCODE(SQL_STILL_EXECUTING),RETCODE(SQL_NEED_DATA)}) {
        Script s;s.options=4;s.returningThrow=returns;
        if(query){s.acceptInfo=FALSE;s.infoCode=code;s.scroll=0;}else{s.acceptAllocation=FALSE;s.allocationCode=code;}
        run(s);
    }
    for(RETCODE code:{RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_STILL_EXECUTING),RETCODE(SQL_ERROR)}) {
        Script s;s.allocationCode=code;run(s);
    }
    for(bool missing:{false,true})for(int failure=1;failure<=9;++failure) {
        Script s;s.missingDatabase=missing;s.options=4;s.scroll=0;s.failure=failure;s.acceptAllocation=failure==9?FALSE:TRUE;run(s);
    }
    return result;
}
}
static void recordsetStatement() {
    using namespace recordset_statement_test;
    auto oldAllocation=database_execute_test::allocationHook;auto oldInfo=database_strings_test::infoHook;
    database_execute_test::allocationHook=&allocate;database_strings_test::infoHook=&info;
    API local{&impl__AllocHstmt_CRecordset__IEAAHXZ,&impl__SetState_CRecordset__IEAAXHPEB_WK_Z};
    auto state=states(local);auto statement=statements(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_STATEMENT_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for statement setup comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"statement setup oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e90d0,reinterpret_cast<void*>(&SQLAllocStmt)},{0x3e9078,reinterpret_cast<void*>(&SQLGetInfoW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                API native{reinterpret_cast<decltype(local.allocate)>(base+0x1f9f20),reinterpret_cast<decltype(local.state)>(base+0x1f9e40)};
                check(states(native)==state,"SetState flags, default types, aliased SQL and custom string ownership match native MFC");
                auto actual=statements(native);
                if(actual!=statement)for(size_t i=0;i<std::min(actual.size(),statement.size());++i)if(actual[i]!=statement[i]) {
                    std::printf("statement mismatch at %zu:",i);for(auto n:statement[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);std::printf("\n");break;
                }
                check(actual==statement,"statement reuse, owned databases, cursor-library negotiation, registration and interrupted state match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::allocationHook=oldAllocation;database_strings_test::infoHook=oldInfo;
}
