#pragma once
#include <exception>
extern "C" BOOL MS_ABI impl__Requery_CRecordset__UEAAHXZ(CRecordset*);
namespace recordset_requery_test {
using Requery=decltype(&impl__Requery_CRecordset__UEAAHXZ);
static std::vector<INT64> events;
struct Script {
    bool filter=false,sort=false,direct=false,rebind=true,emptySQL=false,missingStatement=false,mutateClose=false;
    int missingCache=0,fail=0,closeFailure=0;bool framework=false,accept=true,returningThrow=false;
    BOOL openResult=69;std::vector<RETCODE> codes{SQL_SUCCESS};
};
static Script script;
static CRecordset* active;
static size_t position;
static int closes,deleted;
struct Failure;
static std::vector<Failure*> pending;
struct Failure:CDBException {~Failure() override {++deleted;for(auto& p:pending)if(p==this)p=nullptr;}};
static void fail(int kind) {
    if(script.fail!=kind || (kind==6 && closes!=1))return;
    if(script.framework){auto* value=new Failure;pending.push_back(value);throw static_cast<CException*>(value);}
    throw 400+kind;
}
static SQLRETURN SQL_API free(SQLHSTMT statement,SQLUSMALLINT option) {
    events.insert(events.end(),{1,INT64(UINT_PTR(statement)),option,active->m_lOpen});fail(1);return SQL_ERROR;
}
static void MS_ABI exchange(CRecordset* rs,CFieldExchange* fx) {
    events.insert(events.end(),{2,fx->m_nOperation,INT64(UINT_PTR(fx->m_hstmt)),rs->m_lOpen});fail(2);
    check(fx->m_nOperation==CFieldExchange::RebindParam && fx->m_prs==rs && !fx->m_nParams && !fx->m_nFields && fx->m_nFieldType==UINT(-1),
        "Requery rebinds parameters with the existing statement and native exchange defaults");
}
static SQLRETURN SQL_API execute(SQLHSTMT statement) {
    events.insert(events.end(),{3,INT64(UINT_PTR(statement)),active->m_lOpen});fail(3);
    if(position>=script.codes.size())std::abort();return script.codes[position++];
}
static BOOL MS_ABI checkStatus(const CRecordset*,RETCODE code){events.insert(events.end(),{4,code});fail(4);return script.accept?69:0;}
static void MS_ABI move(CRecordset* rs,long rows,WORD direction) {
    events.insert(events.end(),{5,rows,direction,rs->m_lOpen,rs->m_bBOF,rs->m_bEOF,rs->m_bEOFSeen,rs->m_lRecordCount,rs->m_lCurrentRecord,rs->m_bDeleted,rs->m_nFieldsBound,rs->m_nEditMode});
    rs->m_bEOF=69;fail(5);
}
static void MS_ABI close(CRecordset* rs) {
    ++closes;events.insert(events.end(),{6,closes,rs->m_lOpen});fail(6);
    if(script.closeFailure && closes==1){auto* value=new Failure;pending.push_back(value);throw static_cast<CException*>(value);}
    rs->m_lOpen=0;
    if(script.mutateClose){rs->m_nOpenType=3;rs->m_dwOptions=0x800;rs->m_strRequerySQL=L"from close";}
}
static BOOL MS_ABI open(CRecordset*,UINT type,const wchar_t* sql,DWORD options) {
    events.insert(events.end(),{7,type,options,sql!=nullptr});if(sql){for(wchar_t c:std::wstring(sql))events.push_back(c);}events.push_back(-1);fail(7);return script.openResult;
}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT statement){events.insert(events.end(),{8,code,INT64(UINT_PTR(statement))});if(!script.returningThrow)throw 700+code;}
static BOOL MS_ABI forbidden(CRecordset*) {throw 999;}
static void text(std::vector<INT64>& result,const CString& value) {
    if(!value.GetString()){result.push_back(-1);return;}result.push_back(value.GetLength());for(int i=0;i<value.GetLength();++i)result.push_back(value.GetString()[i]);
}
static std::vector<INT64> exercise(Requery requery) {
    std::vector<INT64> result;
    auto run=[&](const Script& input) {
        script=input;position=0;closes=deleted=0;events.clear();pending.clear();CRecordset rs;active=&rs;
        rs.m_hstmt=script.missingStatement?nullptr:reinterpret_cast<HSTMT>(0xbeef);rs.m_nOpenType=1;rs.m_dwOptions=script.direct?0x2000:0;
        rs.m_bRebindParams=script.rebind?69:0;rs.m_lOpen=37;rs.m_nFieldsBound=5;rs.m_nEditMode=83;
        rs.m_bBOF=55;rs.m_bEOF=57;rs.m_bEOFSeen=69;rs.m_lRecordCount=7;rs.m_lCurrentRecord=6;rs.m_bDeleted=71;
        rs.m_strFilter=script.filter?L"new filter":L"filter";rs.m_strSort=script.sort?L"new sort":L"sort";
        rs.m_strRequeryFilter=L"filter";rs.m_strRequerySort=L"sort";rs.m_strRequerySQL=script.emptySQL?L"":L"SELECT value FROM t";
        void** original=*reinterpret_cast<void***>(&rs);void* table[27];memcpy(table,original,sizeof(table));table[5]=reinterpret_cast<void*>(&open);table[6]=reinterpret_cast<void*>(&close);
        table[7]=reinterpret_cast<void*>(&move);table[14]=reinterpret_cast<void*>(&forbidden);table[19]=reinterpret_cast<void*>(&exchange);table[21]=reinterpret_cast<void*>(&checkStatus);table[24]=reinterpret_cast<void*>(&throwStatus);*reinterpret_cast<void***>(&rs)=table;
        CString* missing=script.missingCache==1?&rs.m_strRequeryFilter:script.missingCache==2?&rs.m_strRequerySort:nullptr;
        auto* detached=missing?database_strings_test::data(*missing):nullptr;if(missing)*reinterpret_cast<wchar_t**>(missing)=nullptr;
        BOOL returned=-1;int error=0;HRESULT ole=0;
        try{returned=requery(&rs);}catch(COleException* value){error=3;ole=value->m_sc;value->Delete();}
        catch(CException* value){error=2;if(std::find(pending.begin(),pending.end(),value)==pending.end())value->Delete();}
        catch(int value){error=value;}
        if(returned==TRUE && !script.filter && !script.sort)check(rs.m_bBOF==69 && rs.m_bEOF==69 && rs.m_lOpen==1 && rs.m_nFieldsBound==5,
            "same-statement Requery preserves bindings, resets cursor state, moves to the first row and copies the full EOF value");
        if(script.direct)check(returned==FALSE && events.empty(),"direct-execution recordsets reject Requery before inspecting cached strings or calling the driver");
        result.insert(result.end(),events.begin(),events.end());result.insert(result.end(),{90,returned,error,ole,closes,deleted,INT64(position),rs.m_lOpen,rs.m_bBOF,rs.m_bEOF,
            rs.m_bEOFSeen,rs.m_lRecordCount,rs.m_lCurrentRecord,rs.m_bDeleted,rs.m_nFieldsBound,rs.m_nEditMode,rs.m_nOpenType,rs.m_dwOptions});
        text(result,rs.m_strRequerySQL);text(result,rs.m_strRequeryFilter);text(result,rs.m_strRequerySort);
        if(missing)database_strings_test::attach(*missing,detached);
        for(auto* value:pending)if(value)value->Delete();pending.clear();
        rs.m_hstmt=nullptr;*reinterpret_cast<void***>(&rs)=original;
    };
    for(bool filter:{false,true})for(bool sort:{false,true})for(bool direct:{false,true})for(bool rebind:{false,true})for(bool empty:{false,true})for(BOOL returned:{FALSE,BOOL(69),BOOL(-7)}) {
        Script s;s.filter=filter;s.sort=sort;s.direct=direct;s.rebind=rebind;s.emptySQL=empty;s.openResult=returned;run(s);
    }
    for(bool mutated:{false,true})for(bool filter:{false,true}){Script s;s.filter=filter;s.sort=true;s.mutateClose=mutated;run(s);}
    for(int missing:{1,2}) {Script s;s.missingCache=missing;s.direct=true;run(s);}
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR),RETCODE(SQL_NEED_DATA)})for(bool accept:{false,true})for(bool returns:{false,true}) {
        Script s;s.codes={SQL_STILL_EXECUTING,SQL_STILL_EXECUTING,code};s.accept=accept;s.returningThrow=returns;run(s);
    }
    for(int failAt=1;failAt<=7;++failAt)for(bool framework:{false,true}) {
        Script s;s.fail=failAt;s.framework=framework;s.filter=failAt>=6;run(s);
    }
    {Script s;s.fail=7;s.framework=true;s.filter=true;s.closeFailure=1;run(s);}
    {Script s;s.fail=2;s.framework=true;s.closeFailure=1;run(s);}
    {Script s;s.missingStatement=true;s.rebind=false;run(s);}
    return result;
}
static void MS_ABI terminationClose(CRecordset*) {::ExitProcess(99);}
static void termination(bool native) {
    wchar_t path[2048];::GetModuleFileNameW(nullptr,path,2048);
    for(int missing:{1,2}) {
        std::wstring flag=std::to_wstring(missing+(native?2:0));::SetEnvironmentVariableW(L"OPENMFC_REQUERY_TERMINATION_CHILD",flag.c_str());
        std::wstring command=L"\""+std::wstring(path)+L"\"";std::vector<wchar_t> writable(command.begin(),command.end());writable.push_back(0);
        STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION child{};
        BOOL started=::CreateProcessW(path,writable.data(),nullptr,nullptr,FALSE,CREATE_NO_WINDOW,nullptr,nullptr,&startup,&child);
        ::SetEnvironmentVariableW(L"OPENMFC_REQUERY_TERMINATION_CHILD",nullptr);
        check(started!=FALSE,"start an isolated process for an invalid cached CString comparison");
        if(started) {
            DWORD waited=::WaitForSingleObject(child.hProcess,10000);if(waited!=WAIT_OBJECT_0)::TerminateProcess(child.hProcess,96);
            DWORD code=0;::GetExitCodeProcess(child.hProcess,&code);::CloseHandle(child.hThread);::CloseHandle(child.hProcess);
            check(waited==WAIT_OBJECT_0 && code==3,"invalid cached Requery strings terminate at the native no-throw comparison boundary before Close or an outer catch");
        }
    }
}
}
static bool recordsetRequeryTerminationChild() {
    wchar_t mode[8];if(!::GetEnvironmentVariableW(L"OPENMFC_REQUERY_TERMINATION_CHILD",mode,8))return false;
    int which=_wtoi(mode);::SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);
    std::set_terminate([]{::ExitProcess(3);});
    using namespace recordset_requery_test;Requery query=&impl__Requery_CRecordset__UEAAHXZ;
    if(which>2) {
        wchar_t path[2048];if(!::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_REQUERY_DLL",path,2048))::ExitProcess(95);
        auto module=::LoadLibraryW(path);if(!module)::ExitProcess(94);query=reinterpret_cast<Requery>(reinterpret_cast<char*>(module)+0x1f9b60);
    }
    CRecordset rs;rs.m_strFilter=L"filter";rs.m_strRequeryFilter=L"filter";rs.m_strSort=L"sort";rs.m_strRequerySort=L"sort";
    auto** original=*reinterpret_cast<void***>(&rs);void* table[27];memcpy(table,original,sizeof(table));table[6]=reinterpret_cast<void*>(&terminationClose);*reinterpret_cast<void***>(&rs)=table;
    *reinterpret_cast<wchar_t**>((which==1 || which==3)?&rs.m_strRequeryFilter:&rs.m_strRequerySort)=nullptr;
    try{query(&rs);}catch(...){::ExitProcess(97);}
    ::ExitProcess(98);
}
static void recordsetRequery() {
    using namespace recordset_requery_test;
    auto oldFree=database_execute_test::freeHook;auto oldExecute=recordset_execute_test::executeHook;
    database_execute_test::freeHook=&free;recordset_execute_test::executeHook=&execute;
    auto expected=exercise(&impl__Requery_CRecordset__UEAAHXZ);
    termination(false);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_REQUERY_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for Requery comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"Requery oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};Patch patches[]={{0x3e90a0,reinterpret_cast<void*>(&SQLFreeStmt)},{0x3e9100,reinterpret_cast<void*>(&SQLExecute)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                auto actual=exercise(reinterpret_cast<Requery>(base+0x1f9b60));
                if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("Requery mismatch %zu local %lld native %lld\n",i,expected[i],actual[i]);break;}
                check(actual==expected,"Requery statement reuse, asynchronous execution, changed SQL settings, cached strings, return values and exception cleanup match native MFC");
                termination(true);
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::freeHook=oldFree;recordset_execute_test::executeHook=oldExecute;
}
