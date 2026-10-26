#pragma once
extern "C" void MS_ABI impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__BindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z(CRecordset*,HSTMT);
extern "C" void MS_ABI impl__Cancel_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z(CRecordset*,HSTMT);
extern "C" void MS_ABI impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__BuildUpdateSQL_CRecordset__IEAAXXZ(CRecordset*);
extern "C" BOOL MS_ABI impl__UpdateInsertDelete_CRecordset__IEAAHXZ(CRecordset*);
extern "C" BOOL MS_ABI impl__Update_CRecordset__UEAAHXZ(CRecordset*);
extern "C" void MS_ABI impl__Delete_CRecordset__UEAAXXZ(CRecordset*);
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
    decltype(&impl__BuildUpdateSQL_CRecordset__IEAAXXZ) build;
    decltype(&impl__UpdateInsertDelete_CRecordset__IEAAHXZ) write;
    decltype(&impl__Update_CRecordset__UEAAHXZ) update;
    decltype(&impl__Delete_CRecordset__UEAAXXZ) erase;
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
enum WriteKind {Position=40,Execute,Direct,Param,Put,RowCount,Cancel,Prepare=60,Cursor};
struct WriteStep {int kind;RETCODE code;int token=-1;};
static std::vector<WriteStep> writeSteps;
static size_t writePosition;
static CLongBinary* payloads[2];static void* payloadData[2];
static SQLLEN affectedRows;
static bool changeHandles;
static bool autoDrive=false;
static std::wstring cursorName;
static SQLSMALLINT reportedCursor;
static void text(int kind,const wchar_t* value) {
    size_t length=wcslen(value);event(kind,INT64(length));
    for(size_t i=0;i<length;i+=5){Event row{kind,INT64(i)};for(size_t j=0;j<5 && i+j<length;++j)row[j+2]=value[i+j];events.push_back(row);}
}
static WriteStep takeWrite(int kind) {
    if(autoDrive){++writePosition;fail(kind);return {kind,SQL_SUCCESS};}
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
static SQLRETURN SQL_API execute(SQLHSTMT statement){event(Execute,INT64(UINT_PTR(statement)),active->m_rgFieldInfos?active->m_rgFieldInfos[0].m_nLength:0,active->m_rgFieldInfos?fieldValue:0);auto step=takeWrite(Execute);change();return step.code;}
static SQLRETURN SQL_API direct(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length) {
    event(Direct,INT64(UINT_PTR(statement)),length,!wcscmp(reinterpret_cast<wchar_t*>(sql),L"UPDATE fixture"));text(52,reinterpret_cast<wchar_t*>(sql));auto step=takeWrite(Direct);change();return step.code;
}
static SQLRETURN SQL_API prepare(SQLHSTMT statement,SQLWCHAR* sql,SQLINTEGER length) {
    event(Prepare,INT64(UINT_PTR(statement)),length);text(52,reinterpret_cast<wchar_t*>(sql));auto step=takeWrite(Prepare);change();return step.code;
}
static SQLRETURN SQL_API cursor(SQLHSTMT statement,SQLWCHAR* output,SQLSMALLINT capacity,SQLSMALLINT* length) {
    event(Cursor,INT64(UINT_PTR(statement)),capacity,*length);auto step=takeWrite(Cursor);
    if(capacity>0){size_t count=std::min(cursorName.size(),size_t(capacity-1));memcpy(output,cursorName.data(),count*2);output[count]=0;}
    *length=reportedCursor;return step.code;
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
static void MS_ABI sqlExchange(CRecordset* rs,CFieldExchange* fx) {
    ordinary(rs,fx);
    if(fx->m_pstr) {
        text(53,fx->m_pstr->GetString());
        if(fx->m_nOperation==CFieldExchange::Name)*fx->m_pstr+=L"[value],";
        else if(fx->m_nOperation==CFieldExchange::NameValue)*fx->m_pstr+=L"[value]=?,";
        else if(fx->m_nOperation==CFieldExchange::Value && !(rs->m_dwOptions&CRecordset::optimizeBulkAdd))*fx->m_pstr+=L"?,";
        check(fx->m_lpszSeparator && !wcscmp(fx->m_lpszSeparator,L","),"write SQL field exchanges use the native comma separator");
    }
}
struct PostFailure:Failure {explicit PostFailure(int flag){m_bAutoDelete=flag;}};
static int postAutoDelete;
static PostFailure* postFailure;
static void MS_ABI postExchange(CRecordset* rs,CFieldExchange* fx) {
    ordinary(rs,fx);
    if(fx->m_nOperation==CFieldExchange::LoadField){postFailure=new PostFailure(postAutoDelete);throw static_cast<CException*>(postFailure);}
}
static std::vector<Event> construction(const UpdateAPI& functions) {
    std::vector<Event> result;
    auto run=[&](UINT mode,DWORD flags,bool cached,int nameCase,unsigned accepted=~0u,int failAt=0,bool returns=false,bool noDatabase=false) {
        Fixture f;f.rs.m_hstmtUpdate=reinterpret_cast<HSTMT>(0xcafe);f.rs.m_nEditMode=mode;f.rs.m_dwOptions=flags;
        f.rs.m_strUpdateSQL=L"old SQL";f.rs.m_strTableName=L"[table]";f.rs.m_strCursorName=cached?L"cached":L"";
        f.db.m_chIDQuoteChar=L'"';if(noDatabase)f.rs.m_pDatabase=nullptr;
        f.table[19]=reinterpret_cast<void*>(&sqlExchange);f.table[21]=reinterpret_cast<void*>(&checkStatus);
        checks=0;acceptance=accepted;failure=failAt;returningThrow=returns;autoDrive=false;changeHandles=false;writePosition=0;
        cursorName=(nameCase==1 || nameCase==5)?std::wstring(55,L'C'):nameCase==4?L"":L"cursor";reportedCursor=nameCase==2?-7:SQLSMALLINT(cursorName.size());writeSteps.clear();
        if(mode<=1 && !cached) {
            writeSteps.push_back({Cursor,RETCODE((nameCase==1 || nameCase==5)?SQL_SUCCESS_WITH_INFO:nameCase==3?SQL_ERROR:nameCase==6?SQL_STILL_EXECUTING:SQL_SUCCESS)});
            if(nameCase==1 || nameCase==5)writeSteps.push_back({Cursor,RETCODE(nameCase==5?SQL_ERROR:SQL_SUCCESS)});
        }
        if(flags&CRecordset::firstBulkAdd){writeSteps.push_back({Prepare,SQL_STILL_EXECUTING});writeSteps.push_back({Prepare,SQL_SUCCESS});}
        int caught=recordset_cursor_test::invoke([&]{functions.build(&f.rs);});
        text(54,f.rs.m_strUpdateSQL.GetString());text(55,f.rs.m_strCursorName.GetString());event(56,INT64(writePosition));saveState(result,f,caught);
    };
    for(UINT mode:{0u,1u,2u,83u})for(DWORD flags:{0ul,0x200ul,0x4000ul,0x8000ul})for(bool cached:{false,true})for(int nameCase:{0,1,2,3,4,5,6})
        run(mode,flags,cached,nameCase);
    for(UINT mode:{0u,1u,2u})for(unsigned accepted:{0u,1u,2u,3u})for(bool returns:{false,true})for(bool missing:{false,true})
        run(mode,0x8000,false,3,accepted,0,returns,missing);
    for(unsigned accepted:{0u,1u,2u,3u})for(bool returns:{false,true})run(1,0x8000,false,5,accepted,0,returns);
    for(UINT mode:{0u,1u,2u})for(int failAt:{int(Cursor),Cursor+10,int(Prepare),Prepare+10,4,14,5,15,108,109,110,208,209,210})
        run(mode,0x8000,false,1,~0u,failAt);
    for(bool locked:{false,true})for(bool shared:{false,true})for(bool cloneElsewhere:{false,true}) {
        Fixture f;database_strings_test::Manager manager,other;manager.clone=cloneElsewhere?&other:&manager;
        f.rs.m_nEditMode=1;f.rs.m_strTableName=L"[table]";f.rs.m_strCursorName=L"cached";f.rs.m_hstmtUpdate=reinterpret_cast<HSTMT>(0xcafe);f.db.m_chIDQuoteChar=L'"';
        f.table[19]=reinterpret_cast<void*>(&sqlExchange);auto* old=database_strings_test::data(f.rs.m_strUpdateSQL);
        auto* storage=manager.Allocate(256,2);storage->nDataLength=3;storage->nRefs=locked?-1:shared?2:1;memcpy(storage+1,L"old",8);
        database_strings_test::attach(f.rs.m_strUpdateSQL,storage);autoDrive=true;changeHandles=false;
        functions.build(&f.rs);auto* output=database_strings_test::data(f.rs.m_strUpdateSQL);
        event(58,manager.allocations,manager.reallocations,manager.clones,other.allocations,other.reallocations,output==storage);
        event(59,output->nRefs,output->pStringMgr==&manager);text(54,f.rs.m_strUpdateSQL.GetString());saveState(result,f);
        f.rs.m_strUpdateSQL.~CString();new(&f.rs.m_strUpdateSQL)CString;
        auto* nil=database_strings_test::data(f.rs.m_strUpdateSQL);if(nil->Release())nil->pStringMgr->Free(nil);database_strings_test::attach(f.rs.m_strUpdateSQL,old);
        if(shared && !locked)manager.Free(storage);
    }
    autoDrive=false;
    return result;
}
static std::vector<Event> pipeline(const UpdateAPI& functions) {
    std::vector<Event> result;
    auto run=[&](int entry,UINT mode,bool useSQL,DWORD flags,bool existing=true,bool dirty=true,int failAt=0,bool returns=false,
                 long current=1,long count=5,bool increment=true,bool capable=true,bool eof=false,bool real=false) {
        Fixture f;f.table[19]=reinterpret_cast<void*>(&sqlExchange);f.table[18]=reinterpret_cast<void*>(&options);f.table[21]=reinterpret_cast<void*>(&checkStatus);
        f.table[12]=f.table[13]=reinterpret_cast<void*>(&recordset_cursor_test::forbiddenEdit);
        f.rs.m_bUseUpdateSQL=useSQL;f.rs.m_nEditMode=mode;f.rs.m_dwOptions=flags;f.rs.m_bDeleted=FALSE;
        f.rs.m_bUpdatable=f.rs.m_bAppendable=capable?69:0;f.rs.m_bEOF=eof?71:0;f.rs.m_strCursorName=L"cached";f.rs.m_strTableName=L"[table]";f.db.m_chIDQuoteChar=L'"';
        f.rs.m_strUpdateSQL=L"old SQL";f.rs.m_hstmtUpdate=existing?reinterpret_cast<HSTMT>(0xcafe):nullptr;
        f.rs.m_lCurrentRecord=current;f.rs.m_lRecordCount=count;f.db.m_bIncRecordCountOnAdd=increment?69:0;
        for(auto& b:f.flags)b=dirty?1:0;checks=0;acceptance=~0u;closeCode=resetCode=allocationCode=SQL_SUCCESS;emptyHandle=false;
        failure=failAt;returningThrow=returns;autoDrive=true;changeHandles=false;writePosition=0;affectedRows=1;
        CFieldInfo info{};CODBCFieldInfo metadata{};metadata.m_nSQLType=SQL_INTEGER;metadata.m_nPrecision=5;
        if(real){f.rs.m_nFields=1;f.rs.m_rgFieldInfos=&info;f.rs.m_rgODBCFieldInfos=&metadata;info.m_nDataType=3;info.m_nLength=4;
            f.table[19]=reinterpret_cast<void*>(&realExchange);fieldValue=42;binary_rfx_test::resetSQL();f.rs.StoreFields();if(mode==1)fieldValue=91;}
        int caught=0;BOOL returned=-7;
        try{if(entry==0)returned=functions.write(&f.rs);else if(entry==1)returned=functions.update(&f.rs);else{functions.erase(&f.rs);returned=1;}}
        catch(CDBException* value){caught=1000+value->m_nRetCode;value->Delete();}catch(CException* value){caught=normalize(value);}catch(int value){caught=value;}
        if(real && !caught && returned && !mode)check(f.rs.m_bDeleted && fieldValue==0x7ee4 && f.flags[0]==2,"Delete executes a real positioned write and sets bound RFX values to NULL");
        if(real && !caught && returned && mode==1)check(fieldValue==91 && !f.flags[0] && !f.rs.m_nEditMode,"Update writes a changed real RFX field and leaves edit mode only after driver success");
        if(real && !caught && returned && mode==2)check(fieldValue==42 && !f.flags[0] && !f.rs.m_nEditMode,"successful insertion restores the pre-AddNew field snapshot after executing the write");
        text(54,f.rs.m_strUpdateSQL.GetString());event(57,returned,INT64(writePosition),real?fieldValue:0,info.m_nLength);saveState(result,f,caught);
        f.rs.m_rgFieldInfos=nullptr;f.rs.m_rgODBCFieldInfos=nullptr;
    };
    for(int entry:{0,1,2})for(UINT mode:{0u,1u,2u,83u})for(bool useSQL:{false,true})
    for(DWORD flags:{0ul,0x200ul,0x4000ul,0x8000ul,0xc000ul})for(bool existing:{false,true})for(bool dirty:{false,true})
        run(entry,mode,useSQL,flags,existing,dirty);
    for(UINT mode:{0u,1u,2u})for(bool capable:{false,true})for(bool eof:{false,true})for(bool returns:{false,true})
        run(0,mode,false,0,true,true,0,returns,1,5,true,capable,eof);
    for(UINT mode:{0u,2u})for(long current:{-2L,-1L,0L,2147483647L})for(long count:{-2L,-1L,0L,1L,2147483647L})for(bool increment:{false,true})
        run(0,mode,false,0,true,true,0,false,current,count,increment);
    for(UINT mode:{0u,1u,2u})for(bool useSQL:{false,true})for(int failAt:{1,2,3,4,11,12,13,14,int(Position),Position+10,int(Direct),Direct+10,103,104,107,111,113,203,204,207,211,213})
        run(0,mode,useSQL,0,true,true,failAt);
    for(UINT mode:{0u,1u,2u})for(bool useSQL:{false,true})run(mode?1:2,mode,useSQL,0,true,true,0,false,1,5,true,true,false,true);
    for(int autoDelete:{-7,0,1,69}) {
        Fixture f;f.rs.m_nEditMode=2;f.rs.m_bAppendable=TRUE;f.rs.m_bDeleted=FALSE;f.table[19]=reinterpret_cast<void*>(&postExchange);
        autoDrive=true;changeHandles=false;writePosition=0;postAutoDelete=autoDelete;postFailure=nullptr;
        BOOL written=functions.write(&f.rs);
        check(written==TRUE && deleted==int(autoDelete>0) && f.rs.m_nEditMode==2,
            "post-write framework failures are swallowed while respecting the native signed exception ownership flag and interrupted edit mode");
        event(62,written,autoDelete,deleted);saveState(result,f);
        if(autoDelete<=0)delete postFailure;
    }
    {
        Fixture f;CFieldInfo info{};CODBCFieldInfo metadata{};info.m_nDataType=3;info.m_nLength=4;metadata.m_nSQLType=SQL_INTEGER;metadata.m_nPrecision=5;
        f.rs.m_rgFieldInfos=&info;f.rs.m_rgODBCFieldInfos=&metadata;f.rs.m_nFields=1;f.rs.m_nEditMode=0;f.rs.m_bDeleted=FALSE;
        f.rs.m_bUseUpdateSQL=f.rs.m_bUpdatable=f.rs.m_bAppendable=TRUE;f.rs.m_dwOptions=CRecordset::optimizeBulkAdd;f.rs.m_hstmtUpdate=nullptr;
        f.rs.m_strTableName=L"[table]";f.db.m_chIDQuoteChar=L'"';f.db.m_bIncRecordCountOnAdd=TRUE;
        f.table[19]=reinterpret_cast<void*>(&realExchange);f.table[18]=reinterpret_cast<void*>(&options);f.table[21]=reinterpret_cast<void*>(&checkStatus);
        checks=0;acceptance=~0u;failure=0;autoDrive=true;changeHandles=false;writePosition=0;affectedRows=1;allocationCode=closeCode=resetCode=SQL_SUCCESS;emptyHandle=false;
        binary_rfx_test::resetSQL();fieldValue=17;f.flags[0]=0;
        for(int i=0;i<3;++i) {
            f.rs.AddNew();if(i<2)fieldValue=101+i;else f.rs.SetFieldDirty(nullptr,TRUE);
            BOOL written=functions.update(&f.rs);
            check(written==TRUE && fieldValue==17 && f.rs.m_nEditMode==CRecordset::noMode && f.rs.m_dwOptions==CRecordset::optimizeBulkAdd,
                "consecutive real AddNew/Update operations preserve the original value and native prepared-bulk state, including a NULL insertion");
        }
        auto count=[&](int kind){return std::count_if(events.begin(),events.end(),[&](const Event& e){return e[0]==kind;});};
        check(binary_rfx_test::binds==1 && count(Prepare)==1 && count(Execute)==3 && count(RowCount)==3 && f.rs.m_lCurrentRecord==4 && f.rs.m_lRecordCount==8,
            "three bulk insertions bind and prepare once, execute three times and update cursor/count state after each successful write");
        event(62,binary_rfx_test::binds,info.m_nLength,fieldValue);saveState(result,f);f.rs.m_rgFieldInfos=nullptr;f.rs.m_rgODBCFieldInfos=nullptr;
    }
    autoDrive=false;return result;
}
}
extern "C" SQLRETURN SQL_API SQLParamData(SQLHSTMT statement,SQLPOINTER* output){return recordset_update_test::parameter(statement,output);}
extern "C" SQLRETURN SQL_API SQLPutData(SQLHSTMT statement,SQLPOINTER data,SQLLEN length){return recordset_update_test::put(statement,data,length);}
extern "C" SQLRETURN SQL_API SQLRowCount(SQLHSTMT statement,SQLLEN* rows){return recordset_update_test::rowCount(statement,rows);}
extern "C" SQLRETURN SQL_API SQLGetCursorNameW(SQLHSTMT statement,SQLWCHAR* output,SQLSMALLINT capacity,SQLSMALLINT* length){return recordset_update_test::cursor(statement,output,capacity,length);}
static void recordsetUpdate() {
    using namespace recordset_update_test;
    auto oldAllocation=database_execute_test::allocationHook;auto oldFree=database_execute_test::freeHook;
    database_execute_test::allocationHook=&allocate;database_execute_test::freeHook=&freeStatement;
    auto oldDirect=database_execute_test::directHook;auto oldExecute=recordset_execute_test::executeHook;
    auto oldPosition=recordset_cursor_test::positionHook;auto oldCancel=database_lifecycle_test::cancelHook;
    auto oldPrepare=recordset_execute_test::prepareHook;recordset_execute_test::prepareHook=&prepare;
    database_execute_test::directHook=&direct;recordset_execute_test::executeHook=&execute;
    recordset_cursor_test::positionHook=&positioned;database_lifecycle_test::cancelHook=&cancel;
    UpdateAPI local{&impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ,&impl__BindFieldsForUpdate_CRecordset__QEAAXXZ,
        &impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ,&impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z,
        &impl__Cancel_CRecordset__QEAAXXZ,&impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z,
        &impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ,&impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ,
        &impl__BuildUpdateSQL_CRecordset__IEAAXXZ,&impl__UpdateInsertDelete_CRecordset__IEAAHXZ,&impl__Update_CRecordset__UEAAHXZ,&impl__Delete_CRecordset__UEAAXXZ};
    auto prepared=preparation(local);auto bound=binding(local);auto written=execution(local);auto built=construction(local);auto writes=pipeline(local);
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
                    {0x3e9058,reinterpret_cast<void*>(&SQLRowCount)},{0x3e9050,reinterpret_cast<void*>(&SQLParamData)},{0x3e9048,reinterpret_cast<void*>(&SQLPutData)},
                    {0x3e90f8,reinterpret_cast<void*>(&SQLPrepareW)},{0x3e9060,reinterpret_cast<void*>(&SQLGetCursorNameW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                UpdateAPI native{reinterpret_cast<decltype(local.prepare)>(base+0x1fbdc0),reinterpret_cast<decltype(local.bind)>(base+0x1fcd30),
                    reinterpret_cast<decltype(local.unbind)>(base+0x1fcd90),reinterpret_cast<decltype(local.options)>(base+0x27d0),
                    reinterpret_cast<decltype(local.cancel)>(base+0x1f9d30),reinterpret_cast<decltype(local.send)>(base+0x1fc570),
                    reinterpret_cast<decltype(local.positioned)>(base+0x1fbcd0),reinterpret_cast<decltype(local.sql)>(base+0x1fc440),
                    reinterpret_cast<decltype(local.build)>(base+0x1fbf10),reinterpret_cast<decltype(local.write)>(base+0x1fd290),
                    reinterpret_cast<decltype(local.update)>(base+0x1f90e0),reinterpret_cast<decltype(local.erase)>(base+0x1f9130)};
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(preparation(native),prepared,"update statement allocation, reuse, bulk parameter preservation, recreation, options and failures match native MFC");
                compare(binding(native),bound,"update column binding, native edit-mode gates, RFX indicators and the default options hook match native MFC");
                compare(execution(native),written,"positioned and SQL writes, affected-row errors, binary streaming, cancellation, asynchronous retries and failures match native MFC");
                compare(construction(native),built,"insert/update/delete SQL, cursor-name caching and expansion, first-bulk preparation and failures match native MFC");
                compare(pipeline(native),writes,"public Update/Delete and the write pipeline, bulk-insert transitions, currency counts, real RFX values and exception regions match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::allocationHook=oldAllocation;database_execute_test::freeHook=oldFree;
    database_execute_test::directHook=oldDirect;recordset_execute_test::executeHook=oldExecute;
    recordset_cursor_test::positionHook=oldPosition;database_lifecycle_test::cancelHook=oldCancel;
    recordset_execute_test::prepareHook=oldPrepare;
}
