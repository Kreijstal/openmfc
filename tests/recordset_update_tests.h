#pragma once
extern "C" void MS_ABI impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__BindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z(CRecordset*,HSTMT);
extern "C" void MS_ABI impl__Cancel_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z(CRecordset*,HSTMT);
extern "C" void MS_ABI impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ(CRecordset*);
namespace recordset_update_test {
using namespace recordset_move_test;
struct UpdateAPI {
    decltype(&impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ) prepare;
    decltype(&impl__BindFieldsForUpdate_CRecordset__QEAAXXZ) bind;
    decltype(&impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ) unbind;
    decltype(&impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z) options;
    decltype(&impl__Cancel_CRecordset__QEAAXXZ) cancel;
    decltype(&impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z) send;
    decltype(&impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ) positioned;
    decltype(&impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ) sql;
};
static RETCODE closeCode,resetCode,allocationCode;
static unsigned acceptance,checks;
static bool emptyHandle;
static SQLRETURN SQL_API allocate(SQLHDBC database,SQLHSTMT* statement) {
    event(30,INT64(UINT_PTR(database)),statement==&active->m_hstmtUpdate,INT64(UINT_PTR(*statement)));fail(1);
    *statement=emptyHandle?nullptr:reinterpret_cast<HSTMT>(0xdead);return allocationCode;
}
static SQLRETURN SQL_API freeStatement(SQLHSTMT statement,SQLUSMALLINT option) {
    event(31,INT64(UINT_PTR(statement)),option);fail(2);return option==SQL_CLOSE?closeCode:option==SQL_RESET_PARAMS?resetCode:SQL_ERROR;
}
static BOOL MS_ABI checkStatus(const CRecordset* rs,RETCODE code) {
    event(32,code,checks,INT64(UINT_PTR(rs->m_hstmtUpdate)));fail(4);
    return (acceptance&(1u<<checks++))?69:0;
}
static void MS_ABI options(CRecordset* rs,HSTMT statement) {event(33,INT64(UINT_PTR(statement)),INT64(UINT_PTR(rs->m_hstmtUpdate)));fail(3);}
static void saveState(std::vector<Event>& result,Fixture& fixture,int caught=0) {
    event(34,INT64(UINT_PTR(fixture.rs.m_hstmtUpdate)),checks);
    save(result,fixture,caught);
    fixture.rs.m_hstmtUpdate=nullptr;
}
static std::vector<Event> preparation(const UpdateAPI& functions) {
    std::vector<Event> result;
    auto run=[&](bool existing,bool missingDatabase,DWORD flags,RETCODE first,RETCODE second,RETCODE allocated,unsigned accepted,int failAt=0,bool empty=false) {
        Fixture f;f.rs.m_hstmtUpdate=existing?reinterpret_cast<HSTMT>(0xcafe):nullptr;
        f.rs.m_dwOptions=flags;if(missingDatabase)f.rs.m_pDatabase=nullptr;
        f.table[18]=reinterpret_cast<void*>(&options);f.table[21]=reinterpret_cast<void*>(&checkStatus);
        checks=0;acceptance=accepted;closeCode=first;resetCode=second;allocationCode=allocated;failure=failAt;emptyHandle=empty;
        longbinary_rfx_test::reset({},{{L"HY000",5,L"[Driver] update statement allocation failed"}});
        int caught=0;
        try{functions.prepare(&f.rs);}catch(CDBException* value){caught=1000+value->m_nRetCode;value->Delete();}
        catch(CException* value){caught=normalize(value);}catch(int value){caught=value==19?-1:value;}
        event(35,INT64(longbinary_rfx_test::diagnosticReads.size()));saveState(result,f,caught);
    };
    for(bool existing:{false,true})for(DWORD flags:{0ul,0x4000ul})for(unsigned accepted=0;accepted<8;++accepted)
    for(RETCODE first:{RETCODE(SQL_SUCCESS),RETCODE(SQL_ERROR),RETCODE(SQL_STILL_EXECUTING)})
    for(RETCODE second:{RETCODE(SQL_SUCCESS),RETCODE(SQL_ERROR)})for(RETCODE allocated:{RETCODE(SQL_SUCCESS),RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA)})
        run(existing,false,flags,first,second,allocated,accepted);
    for(bool existing:{false,true})for(bool missing:{false,true})for(int failAt:{0,1,2,3,4,11,12,13,14})
        run(existing,missing,0,SQL_ERROR,SQL_ERROR,SQL_ERROR,4,failAt);
    run(false,false,0,SQL_SUCCESS,SQL_SUCCESS,SQL_ERROR,0,0,true);
    run(true,false,0,SQL_ERROR,SQL_ERROR,SQL_SUCCESS,2,0,true);
    longbinary_rfx_test::reset({});return result;
}
static std::vector<Event> binding(const UpdateAPI& functions) {
    std::vector<Event> result;
    for(bool unbind:{false,true})for(UINT edit:{0u,1u,2u,3u,0xffffffffu})for(UINT fields:{0u,3u})
    for(DWORD flags:{0ul,0x200ul})for(bool missing:{false,true})for(int failAt:{0,103,104,203,204}) {
        Fixture f;f.rs.m_nEditMode=edit;f.rs.m_nFields=fields;f.rs.m_dwOptions=flags;failure=failAt;
        if(missing)f.rs.m_hstmt=nullptr;
        int caught=recordset_cursor_test::invoke([&]{(unbind?functions.unbind:functions.bind)(&f.rs);});saveState(result,f,caught);
    }
    for(bool unbind:{false,true})for(BYTE flags:{BYTE(0),BYTE(1),BYTE(2),BYTE(3)}) {
        Fixture f;CFieldInfo info{};info.m_nDataType=3;info.m_nLength=91;f.rs.m_rgFieldInfos=&info;f.rs.m_nFields=1;
        f.rs.m_nEditMode=CRecordset::edit;f.flags[0]=flags;f.table[19]=reinterpret_cast<void*>(&realExchange);fieldValue=42;
        (unbind?functions.unbind:functions.bind)(&f.rs);
        SQLLEN expected=unbind?(flags&1?91:4):!(flags&1)?SQL_IGNORE:flags&2?SQL_NULL_DATA:4;
        check(info.m_nLength==expected,"update field binding uses real RFX dirty/NULL indicators and restores only the ignored column lengths on unbind");
        event(36,info.m_nLength,fieldValue);saveState(result,f);f.rs.m_rgFieldInfos=nullptr;
    }
    for(HSTMT statement:{HSTMT(nullptr),reinterpret_cast<HSTMT>(1)}) {
        Fixture f;f.table[18]=reinterpret_cast<void*>(&options);functions.options(&f.rs,statement);
        check(events.empty(),"base OnSetUpdateOptions is the native empty hook and does not interpret or dereference the statement handle");saveState(result,f);
    }
    return result;
}
enum WriteKind {Position=40,Execute,Direct,Param,Put,RowCount,Cancel};
struct WriteStep {int kind;RETCODE code;int token=-1;};
static std::vector<WriteStep> writeSteps;
static size_t writePosition;
static CLongBinary* payloads[2];static void* payloadData[2];
static SQLLEN affectedRows;
static bool changeHandles;
static WriteStep takeWrite(int kind) {
    if(writePosition>=writeSteps.size() || writeSteps[writePosition].kind!=kind){std::printf("unexpected update driver call %d at %zu\n",kind,writePosition);std::abort();}
    auto step=writeSteps[writePosition++];fail(kind);return step;
}
static void change() {
    if(changeHandles){active->m_hstmt=reinterpret_cast<HSTMT>(UINT_PTR(active->m_hstmt)+1);active->m_hstmtUpdate=reinterpret_cast<HSTMT>(UINT_PTR(active->m_hstmtUpdate)+1);}
}
static SQLRETURN SQL_API positioned(SQLHSTMT statement,SQLSETPOSIROW row,SQLUSMALLINT operation,SQLUSMALLINT lock) {
    event(Position,INT64(UINT_PTR(statement)),INT64(row),operation,lock,active->m_rgFieldInfos?active->m_rgFieldInfos[0].m_nLength:0);
    auto step=takeWrite(Position);change();return step.code;
}
static SQLRETURN SQL_API execute(SQLHSTMT statement){event(Execute,INT64(UINT_PTR(statement)));auto step=takeWrite(Execute);change();return step.code;}
static SQLRETURN SQL_API direct(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length) {
    event(Direct,INT64(UINT_PTR(statement)),length,!wcscmp(reinterpret_cast<wchar_t*>(sql),L"UPDATE fixture"));auto step=takeWrite(Direct);change();return step.code;
}
static SQLRETURN SQL_API parameter(SQLHSTMT statement,SQLPOINTER* output) {
    event(Param,INT64(UINT_PTR(statement)));auto step=takeWrite(Param);if(step.token>=0)*output=payloads[step.token];change();return step.code;
}
static SQLRETURN SQL_API put(SQLHSTMT statement,SQLPOINTER data,SQLLEN length) {
    event(Put,INT64(UINT_PTR(statement)),length,data==payloadData[0],data==payloadData[1],data!=nullptr);
    auto step=takeWrite(Put);change();return step.code;
}
static SQLRETURN SQL_API rowCount(SQLHSTMT statement,SQLLEN* rows){event(RowCount,INT64(UINT_PTR(statement)),*rows);auto step=takeWrite(RowCount);*rows=affectedRows;return step.code;}
static SQLRETURN SQL_API cancel(SQLHSTMT statement){event(Cancel,INT64(UINT_PTR(statement)));fail(Cancel);return SQL_ERROR;}
static std::vector<Event> execution(const UpdateAPI& functions) {
    std::vector<Event> result;
    auto run=[&](int operation,std::vector<WriteStep> script,UINT mode=1,DWORD flags=0,UWORD status=SQL_ROW_UPDATED,
                 SQLLEN rows=1,unsigned accepted=0xffffffffu,int failAt=0,bool returns=false,bool changed=false,bool missing=false,bool real=false,SQLULEN bytes=7) {
        Fixture f;f.rs.m_hstmtUpdate=missing?nullptr:reinterpret_cast<HSTMT>(0xcafe);if(missing)f.rs.m_hstmt=nullptr;
        f.rs.m_nEditMode=mode;f.rs.m_dwOptions=flags;f.status[0]=status;f.rs.m_strUpdateSQL=L"UPDATE fixture";
        f.table[21]=reinterpret_cast<void*>(&checkStatus);checks=0;acceptance=accepted;failure=failAt;returningThrow=returns;
        writeSteps=std::move(script);writePosition=0;affectedRows=rows;changeHandles=changed;
        CLongBinary first,second;payloads[0]=&first;payloads[1]=&second;
        for(int i=0;i<2;++i) {
            payloads[i]->m_dwDataLength=bytes+i;payloads[i]->m_hData=missing ? nullptr : ::GlobalAlloc(GMEM_MOVEABLE,16);
            payloadData[i]=missing ? nullptr : ::GlobalLock(payloads[i]->m_hData);
            if(payloadData[i]){memset(payloadData[i],0x41+i,16);::GlobalUnlock(payloads[i]->m_hData);}
        }
        CFieldInfo info{};info.m_nDataType=3;info.m_nLength=91;
        if(real){f.rs.m_nFields=1;f.rs.m_rgFieldInfos=&info;f.table[19]=reinterpret_cast<void*>(&realExchange);fieldValue=42;f.flags[0]=BYTE(flags);f.rs.m_dwOptions=0;}
        longbinary_rfx_test::reset({},{{L"HY000",5,L"[Driver] update failed"}});
        int caught=0;
        try{if(operation==0)functions.cancel(&f.rs);else if(operation==1)functions.send(&f.rs,reinterpret_cast<HSTMT>(0xabcd));
            else if(operation==2)functions.positioned(&f.rs);else functions.sql(&f.rs);}
        catch(CDBException* value){caught=1000+value->m_nRetCode;value->Delete();}
        catch(CException* value){caught=normalize(value);}catch(int value){caught=value==19?-1:value;}
        event(47,INT64(writePosition),INT64(longbinary_rfx_test::diagnosticReads.size()),
            first.m_hData?::GlobalFlags(first.m_hData)&GMEM_LOCKCOUNT:0,second.m_hData?::GlobalFlags(second.m_hData)&GMEM_LOCKCOUNT:0,info.m_nLength);
        event(48,f.rs.m_strUpdateSQL.GetLength(),!wcscmp(f.rs.m_strUpdateSQL.GetString(),L"UPDATE fixture"));saveState(result,f,caught);
        if(real && !caught)check(info.m_nLength==((flags&3)==3?SQL_NULL_DATA:SQLLEN(4)) && fieldValue==42,"positioned writes use real RFX bound length indicators and unbind ignored fields after success");
        f.rs.m_rgFieldInfos=nullptr;
        for(auto* value:payloads)if(value->m_hData)while(::GlobalUnlock(value->m_hData)){}
    };
    for(bool missing:{false,true})for(int failAt:{0,int(Cancel),Cancel+10})run(0,{},1,0,0,1,~0u,failAt,false,false,missing);
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA)})for(bool accepted:{false,true})
        run(1,{{Param,SQL_STILL_EXECUTING},{Param,code}},1,0,0,1,accepted?~0u:0);
    for(SQLULEN bytes:{SQLULEN(0),SQLULEN(7),SQLULEN(1ULL<<40),SQLULEN(-1)})
        run(1,{{Param,SQL_STILL_EXECUTING},{Param,SQL_NEED_DATA,0},{Put,SQL_STILL_EXECUTING},{Put,SQL_SUCCESS},
            {Param,SQL_NEED_DATA,1},{Put,SQL_SUCCESS_WITH_INFO},{Param,SQL_SUCCESS}},1,0,0,1,~0u,0,false,false,false,false,bytes);
    for(bool returns:{false,true})run(1,{{Param,SQL_NEED_DATA,0},{Put,SQL_SUCCESS},{Param,SQL_SUCCESS}},1,0,0,1,~0u,0,returns,false,true);
    for(int failAt:{int(Param),int(Put),Param+10,Put+10,4,14,5,15})
        run(1,{{Param,SQL_NEED_DATA,0},{Put,SQL_ERROR},{Param,SQL_SUCCESS}},1,0,0,1,1,failAt);
    run(1,{{Param,SQL_NEED_DATA,0},{Put,SQL_ERROR}},1,0,0,1,1);
    for(bool returns:{false,true})run(1,{{Param,SQL_NEED_DATA,0},{Put,SQL_SUCCESS},{Param,SQL_ERROR}},1,0,0,1,3,0,returns);
    for(UINT mode:{0u,1u,2u,83u})for(UWORD status:{UWORD(SQL_ROW_UPDATED),UWORD(SQL_ROW_ADDED),UWORD(SQL_ROW_DELETED),UWORD(SQL_ROW_ERROR)})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR)})for(bool accepted:{false,true})for(bool returns:{false,true})
        run(2,{{Position,SQL_STILL_EXECUTING},{Position,code}},mode,0,status,1,accepted?~0u:0,0,returns);
    run(2,{{Position,SQL_NEED_DATA},{Param,SQL_NEED_DATA,0},{Put,SQL_SUCCESS},{Param,SQL_SUCCESS}});
    for(int failAt:{int(Position),Position+10,4,14,5,15,103,104,203,204})run(2,{{Position,SQL_SUCCESS_WITH_INFO}},1,0,SQL_ROW_ERROR,1,~0u,failAt);
    for(BYTE bits:{BYTE(0),BYTE(1),BYTE(2),BYTE(3)})run(2,{{Position,SQL_SUCCESS}},1,bits,SQL_ROW_UPDATED,1,~0u,0,false,false,false,true);
    for(DWORD flags:{0ul,0x4000ul})for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR)})
    for(RETCODE counted:{RETCODE(SQL_SUCCESS),RETCODE(SQL_ERROR)})for(SQLLEN rows:{SQLLEN(-2),SQLLEN(-1),SQLLEN(0),SQLLEN(1),SQLLEN(2),SQLLEN(1ULL<<40)})
    for(unsigned accepted:{0u,1u,2u,3u})for(bool returns:{false,true}) {
        int kind=flags?Execute:Direct;run(3,{{kind,SQL_STILL_EXECUTING},{kind,code},{RowCount,counted}},1,flags,0,rows,accepted,0,returns);
    }
    for(DWORD flags:{0ul,0x4000ul}) {
        int kind=flags?Execute:Direct;
        run(3,{{kind,SQL_NEED_DATA},{Param,SQL_NEED_DATA,0},{Put,SQL_SUCCESS},{Param,SQL_SUCCESS},{RowCount,SQL_SUCCESS}},1,flags);
        for(int failAt:{kind,kind+10,int(RowCount),RowCount+10,4,14,5,15})run(3,{{kind,SQL_ERROR},{RowCount,SQL_ERROR}},1,flags,0,1,~0u,failAt,true);
        run(3,{{kind,SQL_STILL_EXECUTING},{kind,SQL_SUCCESS},{RowCount,SQL_SUCCESS}},1,flags,0,1,~0u,0,false,true);
    }
    longbinary_rfx_test::reset({});return result;
}
}
extern "C" SQLRETURN SQL_API SQLParamData(SQLHSTMT statement,SQLPOINTER* output){return recordset_update_test::parameter(statement,output);}
extern "C" SQLRETURN SQL_API SQLPutData(SQLHSTMT statement,SQLPOINTER data,SQLLEN length){return recordset_update_test::put(statement,data,length);}
extern "C" SQLRETURN SQL_API SQLRowCount(SQLHSTMT statement,SQLLEN* rows){return recordset_update_test::rowCount(statement,rows);}
static void recordsetUpdate() {
    using namespace recordset_update_test;
    auto oldAllocation=database_execute_test::allocationHook;auto oldFree=database_execute_test::freeHook;
    database_execute_test::allocationHook=&allocate;database_execute_test::freeHook=&freeStatement;
    auto oldDirect=database_execute_test::directHook;auto oldExecute=recordset_execute_test::executeHook;
    auto oldPosition=recordset_cursor_test::positionHook;auto oldCancel=database_lifecycle_test::cancelHook;
    database_execute_test::directHook=&direct;recordset_execute_test::executeHook=&execute;
    recordset_cursor_test::positionHook=&positioned;database_lifecycle_test::cancelHook=&cancel;
    UpdateAPI local{&impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ,&impl__BindFieldsForUpdate_CRecordset__QEAAXXZ,
        &impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ,&impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z,
        &impl__Cancel_CRecordset__QEAAXXZ,&impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z,
        &impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ,&impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ};
    auto prepared=preparation(local);auto bound=binding(local);auto written=execution(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_UPDATE_DLL",path,1024)) {
        auto module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset write-engine comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"write-engine oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};Patch patches[]={{0x3e90d0,reinterpret_cast<void*>(&SQLAllocStmt)},
                    {0x3e90a0,reinterpret_cast<void*>(&SQLFreeStmt)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)},
                    {0x3e9108,reinterpret_cast<void*>(&SQLSetPos)},{0x3e9100,reinterpret_cast<void*>(&SQLExecute)},
                    {0x3e90c8,reinterpret_cast<void*>(&SQLExecDirectW)},{0x3e90a8,reinterpret_cast<void*>(&SQLCancel)},
                    {0x3e9058,reinterpret_cast<void*>(&SQLRowCount)},{0x3e9050,reinterpret_cast<void*>(&SQLParamData)},{0x3e9048,reinterpret_cast<void*>(&SQLPutData)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                UpdateAPI native{reinterpret_cast<decltype(local.prepare)>(base+0x1fbdc0),reinterpret_cast<decltype(local.bind)>(base+0x1fcd30),
                    reinterpret_cast<decltype(local.unbind)>(base+0x1fcd90),reinterpret_cast<decltype(local.options)>(base+0x27d0),
                    reinterpret_cast<decltype(local.cancel)>(base+0x1f9d30),reinterpret_cast<decltype(local.send)>(base+0x1fc570),
                    reinterpret_cast<decltype(local.positioned)>(base+0x1fbcd0),reinterpret_cast<decltype(local.sql)>(base+0x1fc440)};
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(preparation(native),prepared,"update statement allocation, reuse, bulk parameter preservation, recreation, options and failures match native MFC");
                compare(binding(native),bound,"update column binding, native edit-mode gates, RFX indicators and the default options hook match native MFC");
                compare(execution(native),written,"positioned and SQL writes, affected-row errors, binary streaming, cancellation, asynchronous retries and failures match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::allocationHook=oldAllocation;database_execute_test::freeHook=oldFree;
    database_execute_test::directHook=oldDirect;recordset_execute_test::executeHook=oldExecute;
    recordset_cursor_test::positionHook=oldPosition;database_lifecycle_test::cancelHook=oldCancel;
}
