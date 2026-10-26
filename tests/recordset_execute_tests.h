#pragma once
extern "C" void MS_ABI impl__PrepareAndExecute_CRecordset__IEAAXXZ(CRecordset*);
namespace recordset_execute_test {
using Execute=decltype(&impl__PrepareAndExecute_CRecordset__IEAAXXZ);
enum Kind {Prepare=1,Direct,Run,Option,Attribute,Check,Throw};
struct Step {Kind kind;RETCODE code;BOOL accepted=69;};
using Event=std::array<INT64,6>;
static std::vector<Event> events;
static std::vector<Step> steps;
static size_t position;
static CRecordset* active;
static SQLULEN reportedConcurrency;
static BOOL accepted;
static bool returningThrow;
static int failure,checkFailure,checks;
using ExecuteHook=SQLRETURN (SQL_API*)(SQLHSTMT);
static ExecuteHook executeHook=nullptr;
using PrepareHook=SQLRETURN (SQL_API*)(SQLHSTMT,SQLWCHAR*,SQLINTEGER);
static PrepareHook prepareHook=nullptr;
static void event(INT64 kind,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0){events.push_back({kind,a,b,c,d,e});}
static RETCODE take(Kind kind) {
    if(position>=steps.size() || steps[position].kind!=kind){std::printf("unexpected execution step %d at %zu\n",kind,position);std::abort();}
    auto step=steps[position++];accepted=step.accepted;
    if(failure==int(position))throw 400+failure;
    return step.code;
}
static SQLRETURN SQL_API prepare(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length) {
    event(Prepare,INT64(UINT_PTR(statement)),length,!wcscmp(reinterpret_cast<wchar_t*>(sql),L"select fixture"));
    longbinary_rfx_test::diagnosticPosition=0;return take(Prepare);
}
static SQLRETURN SQL_API direct(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length) {
    event(Direct,INT64(UINT_PTR(statement)),length,!wcscmp(reinterpret_cast<wchar_t*>(sql),L"select fixture"));
    longbinary_rfx_test::diagnosticPosition=0;return take(Direct);
}
static SQLRETURN SQL_API run(SQLHSTMT statement){event(Run,INT64(UINT_PTR(statement)));return take(Run);}
static SQLRETURN SQL_API option(SQLHSTMT statement,SQLUSMALLINT kind,SQLULEN value) {
    event(Option,INT64(UINT_PTR(statement)),kind,INT64(value),active->m_bUpdatable,active->m_bAppendable);return take(Option);
}
static SQLRETURN SQL_API attribute(SQLHSTMT statement,SQLINTEGER kind,SQLPOINTER output,SQLINTEGER capacity,SQLINTEGER* bytes) {
    event(Attribute,INT64(UINT_PTR(statement)),kind,capacity,active->m_lOpen,bytes!=nullptr);
    check(capacity==0 && kind==SQL_ATTR_CONCURRENCY,"execution warnings query the effective concurrency with a zero-capacity SQLULEN result");
    *static_cast<SQLULEN*>(output)=reportedConcurrency;*bytes=sizeof(SQLULEN);return take(Attribute);
}
static BOOL MS_ABI checkStatus(const CRecordset*,RETCODE code) {event(Check,code);if(++checks==checkFailure)throw 499;return accepted;}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT statement) {event(Throw,code,INT64(UINT_PTR(statement)));if(!returningThrow)throw 700+code;}
static std::vector<INT64> exercise(Execute execute) {
    std::vector<INT64> result;
    auto exerciseCase=[&](std::vector<Step> script,DWORD options=0,DWORD concurrency=SQL_CONCUR_ROWVER,DWORD supported=SQL_SCCO_LOCK,
        SQLULEN effective=SQL_CONCUR_READ_ONLY,std::vector<longbinary_rfx_test::Diagnostic> diagnostics={},bool returns=false,int failAt=0,int checkAt=0,bool noDatabase=false,bool noStatement=false) {
        CDatabase db;CRecordset rs;active=&rs;db.m_hdbc=reinterpret_cast<HDBC>(0xface);rs.m_pDatabase=noDatabase?nullptr:&db;
        rs.m_hstmt=noStatement?nullptr:reinterpret_cast<HSTMT>(0xbeef);rs.m_strSQL=L"select fixture";rs.m_lOpen=37;
        rs.m_dwOptions=options;rs.m_dwConcurrency=concurrency;rs.m_dwDriverConcurrency=supported;rs.m_bUpdatable=69;rs.m_bAppendable=71;
        auto** original=*reinterpret_cast<void***>(&rs);void* table[27];memcpy(table,original,sizeof(table));table[21]=reinterpret_cast<void*>(&checkStatus);table[24]=reinterpret_cast<void*>(&throwStatus);*reinterpret_cast<void***>(&rs)=table;
        steps=std::move(script);position=0;events.clear();reportedConcurrency=effective;returningThrow=returns;failure=failAt;checkFailure=checkAt;checks=0;
        longbinary_rfx_test::reset({},std::move(diagnostics));int caught=0;RETCODE databaseError=0;std::wstring message,origin;
        try{execute(&rs);}catch(CDBException* error){caught=1;databaseError=error->m_nRetCode;message=error->m_strError.GetString();origin=error->m_strStateNativeOrigin.GetString();error->Delete();}catch(int value){caught=value;}
        check(position==steps.size(),"recordset execution consumes only the scripted prepare, retry, execute and warning-query operations");
        for(auto& e:events)result.insert(result.end(),e.begin(),e.end());
        result.insert(result.end(),{90,caught,databaseError,rs.m_lOpen,rs.m_dwConcurrency,rs.m_bUpdatable,rs.m_bAppendable,INT64(position),INT64(longbinary_rfx_test::diagnosticReads.size())});
        for(auto& read:longbinary_rfx_test::diagnosticReads) {
            check(read.database==db.m_hdbc && read.statement==rs.m_hstmt && read.capacity==511,"execution diagnostics retain the owning database and statement");
        }
        for(const auto& value:{message,origin}){for(wchar_t ch:value)result.push_back(ch);result.push_back(-1);}
        rs.m_hstmt=nullptr;rs.m_pDatabase=nullptr;db.m_hdbc=nullptr;*reinterpret_cast<void***>(&rs)=original;longbinary_rfx_test::reset({});
    };
    for(bool directMode:{false,true})for(bool asynchronous:{false,true})for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_NEED_DATA),RETCODE(SQL_ERROR)})for(SQLULEN effective:{SQLULEN(0),SQLULEN(1),SQLULEN(4),SQLULEN(0x100000001ULL)}) {
        std::vector<Step> script;
        Kind first=directMode?Direct:Prepare;if(asynchronous)script.push_back({first,SQL_STILL_EXECUTING});script.push_back({first,RETCODE(directMode?code:SQL_SUCCESS)});
        if(!directMode){if(asynchronous)script.push_back({Run,SQL_STILL_EXECUTING});script.push_back({Run,code});}
        if(code==SQL_SUCCESS_WITH_INFO)script.push_back({Attribute,SQL_SUCCESS});
        exerciseCase(script,directMode?0x2000:0,3,2,effective);
    }
    for(bool directMode:{false,true})for(DWORD concurrency:{1ul,2ul,3ul,4ul,17ul})for(DWORD supported:{0ul,2ul,0xfffffffful})for(bool originMatch:{false,true}) {
        Kind first=directMode?Direct:Prepare;std::vector<Step> script{{first,SQL_ERROR,FALSE}};
        if(originMatch && concurrency!=1) {
            script.push_back({Option,SQL_SUCCESS});script.push_back({first,SQL_SUCCESS});if(!directMode)script.push_back({Run,SQL_SUCCESS});
        }
        exerciseCase(script,directMode?0x2000:0,concurrency,supported,1,{{originMatch?L"S1C00":L"HY000",5,L"[Driver] unsupported option"}});
    }
    for(RETCODE rejected:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_NEED_DATA),RETCODE(SQL_INVALID_HANDLE)})
        exerciseCase({{Prepare,rejected,FALSE}},0,1,2,1,{{L"HY000",7,L"rejected by override"}});
    exerciseCase({{Prepare,SQL_ERROR,FALSE},{Option,SQL_SUCCESS},{Prepare,SQL_ERROR,FALSE},{Option,SQL_SUCCESS},{Prepare,SQL_SUCCESS},{Run,SQL_SUCCESS}},0,3,2,1,{{L"S1C00",5,L"downgrade twice"}});
    exerciseCase({{Direct,SQL_ERROR,FALSE},{Option,SQL_SUCCESS},{Direct,SQL_ERROR,FALSE}},0x2000,2,2,1,{{L"S1C00",5,L"second failure"}});
    for(const wchar_t* state:{L"s1c00",L"HY000"})exerciseCase({{Prepare,SQL_ERROR,FALSE}},0,3,2,1,{{state,5,L"no downgrade"}});
    exerciseCase({{Prepare,SQL_ERROR,FALSE},{Option,SQL_SUCCESS},{Prepare,SQL_SUCCESS},{Run,SQL_SUCCESS}},0,4,2,1,{{L"HY000",5,L"[State:S1C00] origin substring"}});
    for(bool returns:{false,true}) {
        std::vector<Step> script{{Prepare,SQL_SUCCESS},{Run,SQL_ERROR,FALSE}};exerciseCase(script,0,3,2,1,{},returns);
        script={{Prepare,SQL_SUCCESS},{Run,SQL_SUCCESS_WITH_INFO},{Attribute,SQL_ERROR,FALSE}};exerciseCase(script,0,3,2,1,{},returns);
        script={{Prepare,SQL_ERROR,FALSE},{Option,SQL_ERROR,FALSE}};if(returns){script.push_back({Prepare,SQL_SUCCESS});script.push_back({Run,SQL_SUCCESS});}
        exerciseCase(script,0,3,2,1,{{L"S1C00",5,L"downgrade"}},returns);
    }
    exerciseCase({{Prepare,SQL_ERROR,FALSE}},0,3,2,1,{},false,0,0,true);
    exerciseCase({{Prepare,SQL_SUCCESS},{Run,SQL_SUCCESS}},0,3,2,1,{},false,0,0,false,true);
    for(int failure=1;failure<=3;++failure) {
        std::vector<Step> script{{Prepare,SQL_SUCCESS},{Run,SQL_SUCCESS_WITH_INFO},{Attribute,SQL_SUCCESS}};script.resize(failure);exerciseCase(script,0,3,2,1,{},false,failure);
        exerciseCase(script,0,3,2,1,{},false,0,failure);
    }
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLPrepareW(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length){if(recordset_execute_test::prepareHook)return recordset_execute_test::prepareHook(statement,sql,length);return recordset_execute_test::prepare(statement,sql,length);}
extern "C" SQLRETURN SQL_API SQLExecute(SQLHSTMT statement){if(recordset_execute_test::executeHook)return recordset_execute_test::executeHook(statement);return recordset_execute_test::run(statement);}
extern "C" SQLRETURN SQL_API SQLGetStmtAttrW(SQLHSTMT statement,SQLINTEGER kind,SQLPOINTER output,SQLINTEGER capacity,SQLINTEGER* bytes){return recordset_execute_test::attribute(statement,kind,output,capacity,bytes);}
static void recordsetExecute() {
    using namespace recordset_execute_test;
    auto oldDirect=database_execute_test::directHook;auto oldOption=database_layout_test::optionHook;database_execute_test::directHook=&direct;database_layout_test::optionHook=&option;
    auto expected=exercise(&impl__PrepareAndExecute_CRecordset__IEAAXXZ);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_EXECUTE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset execution comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"recordset execution oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e9080,reinterpret_cast<void*>(&SQLSetStmtOption)},{0x3e90c8,reinterpret_cast<void*>(&SQLExecDirectW)},
                    {0x3e90f8,reinterpret_cast<void*>(&SQLPrepareW)},{0x3e9100,reinterpret_cast<void*>(&SQLExecute)},
                    {0x3e90f0,reinterpret_cast<void*>(&SQLGetStmtAttrW)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                auto actual=exercise(reinterpret_cast<Execute>(base+0x1fa3a0));
                if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("execution mismatch %zu local %lld native %lld\n",i,expected[i],actual[i]);break;}
                check(actual==expected,"recordset preparation, direct execution, asynchronous retries, concurrency downgrades, warnings and failure state match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::directHook=oldDirect;database_layout_test::optionHook=oldOption;
}
