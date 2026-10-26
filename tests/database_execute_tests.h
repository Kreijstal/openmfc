#pragma once
#include <array>
extern "C" void MS_ABI impl__ExecuteSQL_CDatabase__QEAAXPEB_W_Z(CDatabase*,const wchar_t*);
namespace database_execute_test {
enum Kind {Alloc=1,Execute,Columns,Fetch,More,Drop,Options,Bind,Check,Throw,Cancel};
struct Step {Kind kind;RETCODE code;SQLSMALLINT columns=0;};
using Event=std::array<INT64,4>;
static std::vector<Step> steps;
static size_t position;
static std::vector<Event> events;
static bool emptyStatement;
static int callbackFailure;
using AllocationHook=SQLRETURN (SQL_API*)(SQLHDBC,SQLHSTMT*);
static AllocationHook allocationHook=nullptr;
using FreeHook=SQLRETURN (SQL_API*)(SQLHSTMT,SQLUSMALLINT);
static FreeHook freeHook=nullptr;
using ColumnsHook=SQLRETURN (SQL_API*)(SQLHSTMT,SQLSMALLINT*);
static ColumnsHook columnsHook=nullptr;
using DirectHook=SQLRETURN (SQL_API*)(SQLHSTMT,SQLWCHAR*,SQLINTEGER);
static DirectHook directHook=nullptr;
using FetchHook=SQLRETURN (SQL_API*)(SQLHSTMT);
static FetchHook fetchHook=nullptr;
static HSTMT handle() {return emptyStatement?nullptr:reinterpret_cast<HSTMT>(0xbeef);}
static Step take(Kind kind) {
    if(position>=steps.size() || steps[position].kind!=kind)std::abort();return steps[position++];
}
static void record(Kind kind,UINT_PTR first=0,INT64 second=0,INT64 third=0) {events.push_back({kind,INT64(first),second,third});}
static void fail(int callback) {
    if(callbackFailure==callback)throw new CDBException(-7);
    if(callbackFailure==callback+2)throw 77;
}
static void MS_ABI options(CDatabase* database,HSTMT statement) {record(Options,UINT_PTR(statement),UINT_PTR(database->m_hstmt));fail(1);}
static void MS_ABI bind(CDatabase* database,HSTMT statement) {record(Bind,UINT_PTR(statement),UINT_PTR(database->m_hstmt));fail(2);}
static BOOL MS_ABI checkStatus(const CDatabase*,RETCODE code) {record(Check,WORD(code));return FALSE;}
static void MS_ABI throwStatus(CDatabase*,RETCODE code) {record(Throw,WORD(code));throw 88;}
static SQLRETURN SQL_API cancel(SQLHSTMT statement) {record(Cancel,UINT_PTR(statement));return SQL_ERROR;}
using ExecuteSQL=decltype(&impl__ExecuteSQL_CDatabase__QEAAXPEB_W_Z);
static std::vector<Event> exercise(ExecuteSQL execute) {
    CDatabase database;database.m_hdbc=reinterpret_cast<HDBC>(0xface);database.m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    auto** original=*reinterpret_cast<void***>(&database);void* table[12];std::memcpy(table,original,sizeof(table));
    table[8]=reinterpret_cast<void*>(&options);table[9]=reinterpret_cast<void*>(&bind);table[10]=reinterpret_cast<void*>(&checkStatus);table[11]=reinterpret_cast<void*>(&throwStatus);
    *reinterpret_cast<void***>(&database)=table;
    std::vector<Event> result;
    auto run=[&](std::vector<Step> script,RETCODE expected=0,int callback=0,bool nullHandle=false) {
        steps=std::move(script);position=0;events.clear();callbackFailure=callback;emptyStatement=nullHandle;
        longbinary_rfx_test::reset({},{{L"HY000",5,L"[Driver] execution failed"}});
        RETCODE error=0;bool caught=false;int foreign=0;
        try {execute(&database,L"select fixture");}
        catch(CDBException* exception) {caught=true;error=exception->m_nRetCode;exception->Delete();}
        catch(int value){foreign=value;}
        check((expected?caught && error==expected:!caught) && foreign==(callback>2?77:0) && position==steps.size() && database.m_hstmt==reinterpret_cast<HSTMT>(0xcafe),
            "SQL execution preserves the instance statement and forwards driver or callback failures without returning a synthetic BOOL");
        auto dropped=std::count_if(events.begin(),events.end(),[](const Event& value){return value[0]==Drop;});
        bool allocated=steps[0].code==SQL_SUCCESS || steps[0].code==SQL_SUCCESS_WITH_INFO || steps[0].code==SQL_NO_DATA;
        bool cleanup=allocated && callback<=2;
        check(dropped==int(cleanup) && (!cleanup || (events.back()[0]==Drop && events.back()[1]==INT64(UINT_PTR(handle())) && events.back()[2]==SQL_DROP)),
            "SQL execution drops allocated local statements on completion and framework failures without freeing failed allocations or foreign exceptions");
        auto cancelled=std::count_if(events.begin(),events.end(),[](const Event& value){return value[0]==Cancel;});
        check(cancelled==int(cleanup && expected!=0) && (!cancelled || (events[events.size()-2][0]==Cancel && events[events.size()-2][1]==INT64(UINT_PTR(handle())))),
            "SQL execution cancels its local statement before dropping it when a framework failure unwinds execution");
        check(std::none_of(events.begin(),events.end(),[](const Event& value){return value[0]==Check || value[0]==Throw;}),
            "SQL execution uses native return-code checks and the standalone statement exception path");
        if(expected==SQL_ERROR) {
            check(longbinary_rfx_test::diagnosticReads.size()==2 && longbinary_rfx_test::diagnosticReads[0].statement==handle() &&
                longbinary_rfx_test::diagnosticReads[0].database==database.m_hdbc,"SQL execution diagnostics use its local statement rather than the database instance statement");
        }
        result.insert(result.end(),events.begin(),events.end());result.push_back({90,error,foreign,INT64(position)});
    };
    run({{Alloc,SQL_SUCCESS},{Execute,SQL_SUCCESS},{Columns,SQL_SUCCESS,0},{More,SQL_NO_DATA}});
    run({{Alloc,SQL_SUCCESS_WITH_INFO},{Execute,SQL_STILL_EXECUTING},{Execute,SQL_SUCCESS_WITH_INFO},
        {Columns,SQL_STILL_EXECUTING},{Columns,SQL_SUCCESS,2},{Fetch,SQL_STILL_EXECUTING},{Fetch,SQL_SUCCESS},{Fetch,SQL_SUCCESS_WITH_INFO},{Fetch,SQL_NO_DATA},
        {More,SQL_STILL_EXECUTING},{More,SQL_SUCCESS},{Columns,SQL_NO_DATA,0},{More,SQL_NO_DATA}});
    for(RETCODE accepted:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA)}) {
        run({{Alloc,accepted},{Execute,accepted},{Columns,accepted,0},{More,SQL_NO_DATA}});
        run({{Alloc,SQL_SUCCESS},{Execute,SQL_SUCCESS},{Columns,accepted,-1},{Fetch,SQL_NO_DATA},{More,SQL_NO_DATA}});
    }
    for(Kind point:{Alloc,Execute,Columns,Fetch,More})for(RETCODE failure:{RETCODE(SQL_ERROR),RETCODE(SQL_INVALID_HANDLE),RETCODE(SQL_NEED_DATA)}) {
        std::vector<Step> script{{Alloc,SQL_SUCCESS}};
        if(point!=Alloc)script.push_back({Execute,SQL_SUCCESS});
        if(point==Columns || point==Fetch || point==More)script.push_back({Columns,SQL_SUCCESS,SQLSMALLINT(point==Fetch?1:0)});
        if(point==Fetch)script.push_back({Fetch,SQL_SUCCESS});
        if(point==More)script.push_back({More,SQL_SUCCESS});
        script.back().code=failure;run(script,failure);
    }
    run({{Alloc,SQL_ERROR}},SQL_ERROR,0,true);
    run({{Alloc,SQL_STILL_EXECUTING}},SQL_STILL_EXECUTING);
    for(int callback:{1,2,3,4})run({{Alloc,SQL_SUCCESS}},callback<3?RETCODE(-7):RETCODE(0),callback);
    database.m_hdbc=nullptr;run({{Alloc,SQL_SUCCESS},{Execute,SQL_SUCCESS},{Columns,SQL_SUCCESS,0},{More,SQL_NO_DATA}});
    for(bool missingDatabase:{false,true}) {
        events.clear();bool invalid=false;
        try {execute(missingDatabase?nullptr:&database,missingDatabase?L"query":nullptr);}catch(int value){invalid=value==19;}catch(CException* exception){invalid=true;exception->Delete();}
        check(invalid && events.empty(),"SQL execution rejects null database or SQL pointers before allocating a statement");
        result.push_back({91,missingDatabase,invalid,0});
    }
    database.m_hstmt=nullptr;*reinterpret_cast<void***>(&database)=original;longbinary_rfx_test::reset({});events.clear();return result;
}
}
extern "C" SQLRETURN SQL_API SQLAllocStmt(SQLHDBC database,SQLHSTMT* statement) {
    using namespace database_execute_test;if(allocationHook)return allocationHook(database,statement);
    record(Alloc,UINT_PTR(database));auto step=take(Alloc);*statement=handle();return step.code;
}
extern "C" SQLRETURN SQL_API SQLExecDirectW(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length) {
    if(database_execute_test::directHook)return database_execute_test::directHook(statement,sql,length);
    using namespace database_execute_test;record(Execute,UINT_PTR(statement),length,std::wcscmp(reinterpret_cast<wchar_t*>(sql),L"select fixture"));return take(Execute).code;
}
extern "C" SQLRETURN SQL_API SQLNumResultCols(SQLHSTMT statement,SQLSMALLINT* columns) {
    if(database_execute_test::columnsHook)return database_execute_test::columnsHook(statement,columns);
    using namespace database_execute_test;auto step=take(Columns);record(Columns,UINT_PTR(statement),step.columns);*columns=step.columns;return step.code;
}
extern "C" SQLRETURN SQL_API SQLFetch(SQLHSTMT statement) {using namespace database_execute_test;if(fetchHook)return fetchHook(statement);record(Fetch,UINT_PTR(statement));return take(Fetch).code;}
extern "C" SQLRETURN SQL_API SQLMoreResults(SQLHSTMT statement) {using namespace database_execute_test;record(More,UINT_PTR(statement));return take(More).code;}
extern "C" SQLRETURN SQL_API SQLFreeStmt(SQLHSTMT statement,SQLUSMALLINT option) {
    if(database_execute_test::freeHook)return database_execute_test::freeHook(statement,option);
    database_execute_test::record(database_execute_test::Drop,UINT_PTR(statement),option);return SQL_ERROR;
}
static void databaseExecute() {
    using namespace database_execute_test;
    auto previousCancel=database_lifecycle_test::cancelHook;database_lifecycle_test::cancelHook=&cancel;
    auto expected=exercise(&impl__ExecuteSQL_CDatabase__QEAAXPEB_W_Z);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_EXECUTE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for SQL execution comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"SQL execution oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch {size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e90d0,reinterpret_cast<void*>(&SQLAllocStmt)},{0x3e90c8,reinterpret_cast<void*>(&SQLExecDirectW)},
                    {0x3e90c0,reinterpret_cast<void*>(&SQLNumResultCols)},{0x3e90b8,reinterpret_cast<void*>(&SQLFetch)},
                    {0x3e90b0,reinterpret_cast<void*>(&SQLMoreResults)},{0x3e90a0,reinterpret_cast<void*>(&SQLFreeStmt)},
                    {0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)},{0x3e90a8,reinterpret_cast<void*>(&SQLCancel)}};
                for(auto& patch:patches){auto** slot=reinterpret_cast<void**>(base+patch.rva);patch.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&patch.protection);*slot=patch.function;}
                check(exercise(reinterpret_cast<ExecuteSQL>(base+0x1f6ee0))==expected,
                    "SQL execution callbacks, retries, row and result draining, errors and statement cleanup match native MFC");
                for(auto& patch:patches){auto** slot=reinterpret_cast<void**>(base+patch.rva);*slot=patch.saved;DWORD ignored;::VirtualProtect(slot,8,patch.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_lifecycle_test::cancelHook=previousCancel;
}
