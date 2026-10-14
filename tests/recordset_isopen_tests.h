#pragma once
extern "C" BOOL MS_ABI impl__IsOpen_CRecordset__QEBAHXZ(const CRecordset*);
namespace recordset_isopen_test {
using IsOpen=decltype(&impl__IsOpen_CRecordset__QEBAHXZ);
using Event=std::array<INT64,4>;
static std::vector<Event> events;
static std::vector<RETCODE> codes;
static size_t position;
static SQLSMALLINT columns;
static BOOL accepted;
static int callbackFailure;
static SQLRETURN SQL_API discover(SQLHSTMT statement,SQLSMALLINT* output) {
    if(position>=codes.size())std::abort();RETCODE code=codes[position++];*output=columns;
    events.push_back({1,INT64(UINT_PTR(statement)),code,columns});return code;
}
static BOOL MS_ABI checkResult(const CRecordset*,RETCODE code) {
    events.push_back({2,code,accepted,0});if(callbackFailure)throw 73;return accepted;
}
static void MS_ABI throwResult(CRecordset*,RETCODE,HSTMT) {events.push_back({3,0,0,0});throw 74;}
static std::vector<INT64> exercise(IsOpen invoke) {
    CDatabase database;database.m_hdbc=reinterpret_cast<HDBC>(0xface);
    CRecordset recordset;recordset.m_pDatabase=&database;recordset.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);
    auto** original=*reinterpret_cast<void***>(&recordset);void* table[27];std::memcpy(table,original,sizeof(table));
    table[21]=reinterpret_cast<void*>(&checkResult);table[24]=reinterpret_cast<void*>(&throwResult);*reinterpret_cast<void***>(&recordset)=table;
    std::vector<INT64> result;
    auto run=[&](bool statement,LONG state,std::vector<RETCODE> script,SQLSMALLINT count,BOOL status,
        std::vector<longbinary_rfx_test::Diagnostic> diagnostics={},int foreign=0,bool absentDatabase=false) {
        events.clear();position=0;codes=std::move(script);columns=count;accepted=status;callbackFailure=foreign;
        recordset.m_hstmt=statement?reinterpret_cast<HSTMT>(0xbeef):nullptr;recordset.m_lOpen=state;recordset.m_nResultCols=27;
        recordset.m_pDatabase=absentDatabase?nullptr:&database;longbinary_rfx_test::reset({},std::move(diagnostics));
        RETCODE error=0;int thrown=0;BOOL value=39;bool failed=false;std::wstring message,origin;
        try {value=invoke(&recordset);}catch(CDBException* exception){failed=true;error=exception->m_nRetCode;message=exception->m_strError.GetString();origin=exception->m_strStateNativeOrigin.GetString();exception->Delete();}
        catch(int code){thrown=code;}
        bool queries=statement && state!=1;
        bool suppressed=origin.find(L"State:S1010")!=std::wstring::npos;
        check(recordset.m_lOpen==state && recordset.m_nResultCols==27 && position==(queries?codes.size():0),
            "recordset IsOpen preserves cached state and result-column count while querying only uncached statements");
        check(events.size()==(queries?codes.size()+1:0) && std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==3;}),
            "recordset IsOpen retries asynchronous queries, calls virtual Check once and bypasses virtual ThrowDBException");
        if(!queries)check(value==BOOL(statement),"recordset IsOpen returns false for null statements and true for its native open-state shortcut");
        else if(foreign)check(thrown==73,"recordset IsOpen propagates exceptions from its virtual status check");
        else if(status)check(value==BOOL(count!=0) && longbinary_rfx_test::diagnosticReads.empty(),"accepted recordset IsOpen queries normalize nonzero column counts without requesting diagnostics");
        else if(!failed)check(!value,"recordset IsOpen suppresses function-sequence diagnostics and reports a closed result set");
        else check(error==codes.back() && !suppressed,"recordset IsOpen throws the query return code and diagnostics when no function-sequence status is found");
        if(queries && !status && !foreign) {
            check(absentDatabase?longbinary_rfx_test::diagnosticReads.empty():!longbinary_rfx_test::diagnosticReads.empty(),
                "recordset IsOpen builds diagnostics for any rejected query status only when its database is present");
            for(const auto& read:longbinary_rfx_test::diagnosticReads)
                check(read.database==database.m_hdbc && read.statement==recordset.m_hstmt && read.capacity==511,"recordset IsOpen diagnostics use its owning database and current statement");
        }
        for(auto& event:events)result.insert(result.end(),event.begin(),event.end());result.insert(result.end(),{90,value,failed,error,thrown,INT64(position),INT64(longbinary_rfx_test::diagnosticReads.size())});
        for(const auto& text:{message,origin}){for(wchar_t ch:text)result.push_back(ch);result.push_back(-1);}
    };
    for(bool statement:{false,true})for(LONG state:{-1,0,1,2})for(SQLSMALLINT count:{SQLSMALLINT(0),SQLSMALLINT(3),SQLSMALLINT(-1)})
        run(statement,state,{SQL_STILL_EXECUTING,SQL_STILL_EXECUTING,SQL_SUCCESS},count,69);
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NEED_DATA),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR),RETCODE(SQL_INVALID_HANDLE)}) {
        run(true,0,{code},2,FALSE,{{L"HY000",5,L"[Driver] failed"}});
        run(true,0,{code},2,FALSE,{{L"S1010",5,L"[Driver] no sequence"}});
        run(true,0,{code},2,FALSE,{{L"HY000",5,L"first"},{L"S1010",7,L"last"}});
        run(true,0,{code},2,FALSE,{{L"s1010",5,L"lower case"}});
        run(true,0,{code},2,FALSE,{{L"HY000",5,L"[State:S1010] driver origin"}});
        run(true,0,{code},2,FALSE,{});
        run(true,0,{code},2,FALSE,{},0,true);
    }
    run(true,0,{SQL_SUCCESS},2,TRUE,{},1);
    recordset.m_hstmt=nullptr;recordset.m_pDatabase=nullptr;*reinterpret_cast<void***>(&recordset)=original;database.m_hdbc=nullptr;longbinary_rfx_test::reset({});
    return result;
}
}
static void recordsetIsOpen() {
    using namespace recordset_isopen_test;
    auto previous=database_execute_test::columnsHook;database_execute_test::columnsHook=&discover;
    auto expected=exercise(&impl__IsOpen_CRecordset__QEBAHXZ);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_ISOPEN_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset open-state comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"recordset IsOpen oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch {size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e90c0,reinterpret_cast<void*>(&SQLNumResultCols)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                check(exercise(reinterpret_cast<IsOpen>(base+0x1f8710))==expected,"recordset IsOpen shortcuts, retries, status checks, diagnostics and exceptions match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::columnsHook=previous;
}
