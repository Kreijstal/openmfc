#pragma once
extern "C" BOOL MS_ABI impl__Open_CRecordset__UEAAHIPEB_WK_Z(CRecordset*,UINT,const wchar_t*,DWORD);
extern "C" void MS_ABI impl__PreBindFields_CRecordset__UEAAXXZ(CRecordset*);
namespace recordset_open_test {
using Open=decltype(&impl__Open_CRecordset__UEAAHIPEB_WK_Z);
static std::vector<INT64> events;
struct Script {
    UINT type=CRecordset::snapshot,defaultType=CRecordset::snapshot;DWORD options=0;
    UINT fields=2,params=2;bool existing=false,alreadyOpen=false,lateFields=false,table=false;
    bool closedDatabase=false;BOOL openResult=FALSE;
    int failStage=0,failCheck=1,closeFailure=0;bool framework=false,prepareError=false;
};
static Script script;
static CRecordset* active;
static int checks,closeCalls,deleted;
struct Failure;
static std::vector<Failure*> pending;
struct Failure:CDBException {
    ~Failure() override {++deleted;for(auto& value:pending)if(value==this)value=nullptr;}
};
static void fail(int stage) {
    if(script.failStage!=stage || (stage==10 && checks!=script.failCheck))return;
    if(script.framework){auto* error=new Failure;pending.push_back(error);throw static_cast<CException*>(error);}
    throw 400+stage;
}
static CString* MS_ABI connection(CRecordset*,CString* output) {events.push_back(1);fail(1);return new(output) CString(L"ODBC;");}
static SQLRETURN SQL_API allocate(SQLHDBC db,SQLHSTMT* statement) {
    events.insert(events.end(),{2,INT64(UINT_PTR(db)),statement==&active->m_hstmt});*statement=reinterpret_cast<HSTMT>(0xbeef);fail(2);
    recordset_execute_test::accepted=69;return SQL_SUCCESS;
}
static void MS_ABI options(CRecordset* rs,HSTMT statement) {events.insert(events.end(),{3,INT64(UINT_PTR(statement)),rs->m_nFields,rs->m_nParams});fail(3);}
static CString* MS_ABI sql(CRecordset* rs,CString* output) {events.push_back(4);fail(4);if(script.lateFields)rs->m_nFields=2;return new(output) CString(L"SELECT x,y FROM t");}
static void exchange(CRecordset* rs,CFieldExchange* fx,bool bulk) {
    events.insert(events.end(),{5,fx->m_nOperation,bulk,rs->m_rgFieldInfos!=nullptr,rs->m_pbParamFlags!=nullptr});fail(5);
    if(fx->m_nOperation==CFieldExchange::Name)openmfc::detail::DatabaseStringBuffer(*fx->m_pstr).append(L"x,y,");
    fx->m_nFields=rs->m_nFields;fx->m_nParams=rs->m_nParams;
}
static void MS_ABI ordinary(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,false);}
static void MS_ABI bulk(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,true);}
static SQLRETURN SQL_API columns(SQLHSTMT statement,SQLSMALLINT* count) {
    bool metadata=count==&active->m_nResultCols;events.insert(events.end(),{6,INT64(UINT_PTR(statement)),metadata});fail(6);
    *count=metadata?2:0;recordset_execute_test::accepted=69;return SQL_SUCCESS;
}
static SQLRETURN SQL_API describe(SQLHSTMT statement,SQLUSMALLINT field,SQLWCHAR* name,SQLSMALLINT capacity,SQLSMALLINT* length,SQLSMALLINT* type,SQLULEN* precision,SQLSMALLINT* scale,SQLSMALLINT* nullable) {
    events.insert(events.end(),{7,INT64(UINT_PTR(statement)),field,capacity});fail(7);name[0]=SQLWCHAR(L'a'+field);name[1]=0;
    *length=1;*type=SQL_INTEGER;*precision=10;*scale=0;*nullable=1;recordset_execute_test::accepted=69;return SQL_SUCCESS;
}
static void MS_ABI prebind(CRecordset* rs) {events.insert(events.end(),{8,rs->m_rgFieldInfos!=nullptr,rs->m_pbFieldFlags!=nullptr,rs->m_plParamLength!=nullptr});fail(8);}
static void MS_ABI move(CRecordset* rs,long rows,WORD direction) {events.insert(events.end(),{9,rows,direction,rs->m_lOpen});rs->m_bEOF=69;fail(9);}
static BOOL MS_ABI checkStatus(const CRecordset*,RETCODE code) {
    ++checks;events.insert(events.end(),{10,code,INT64(recordset_execute_test::position)});fail(10);return recordset_execute_test::accepted;
}
static void MS_ABI close(CRecordset* rs) {
    ++closeCalls;events.insert(events.end(),{11,rs->m_lOpen,rs->m_rgFieldInfos!=nullptr,rs->m_rgODBCFieldInfos!=nullptr});rs->m_bBOF=91;
    if(script.closeFailure==1)throw 411;
    if(script.closeFailure==2){auto* error=new Failure;pending.push_back(error);throw static_cast<CException*>(error);}
}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT) {throw new CDBException(code);}
static SQLRETURN SQL_API option(SQLHSTMT statement,SQLUSMALLINT which,SQLULEN value) {events.insert(events.end(),{12,INT64(UINT_PTR(statement)),which,INT64(value)});return SQL_SUCCESS;}
static BOOL MS_ABI databaseOpen(CDatabase* db,const wchar_t*,BOOL,BOOL,const wchar_t*,BOOL cursors) {
    events.insert(events.end(),{13,cursors});if(script.openResult)db->m_hdbc=reinterpret_cast<HDBC>(0xface);return script.openResult;
}
static BOOL MS_ABI forbiddenOpen(CRecordset*,UINT,const wchar_t*,DWORD) {throw 999;}
static void string(std::vector<INT64>& result,const CString& value){result.push_back(value.GetLength());for(int i=0;i<value.GetLength();++i)result.push_back(value.GetString()[i]);}
static void defaultPrebind(void (MS_ABI* callback)(CRecordset*)) {
    CRecordset rs;active=&rs;rs.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);events.clear();
    auto** original=*reinterpret_cast<void***>(&rs);void* table[27];memcpy(table,original,sizeof(table));table[23]=reinterpret_cast<void*>(&prebind);*reinterpret_cast<void***>(&rs)=table;
    callback(&rs);
    check(events.empty() && rs.m_nResultCols==-1 && !rs.m_rgODBCFieldInfos,"default PreBindFields is a qualified no-op and does not reload metadata or call a derived override");
    rs.m_hstmt=nullptr;*reinterpret_cast<void***>(&rs)=original;
}
static std::vector<INT64> exercise(Open open) {
    std::vector<INT64> result;
    auto run=[&](const Script& next) {
        script=next;events.clear();pending.clear();checks=closeCalls=deleted=0;CDatabase db;CRecordset rs;active=&rs;
        rs.m_pDatabase=&db;db.m_hdbc=script.closedDatabase?nullptr:reinterpret_cast<HDBC>(0xface);db.m_chIDQuoteChar=L'"';db.m_dwUpdateOptions=3;db.m_bAddForUpdate=FALSE;
        rs.m_nFields=script.fields;rs.m_nParams=script.params;rs.m_nDefaultType=script.defaultType;rs.m_lOpen=script.alreadyOpen?1:37;
        rs.m_dwConcurrency=SQL_CONCUR_READ_ONLY;rs.m_dwDriverConcurrency=0;
        rs.m_bBOF=55;rs.m_bEOF=57;rs.m_strFilter=L"x>0";rs.m_strSort=L"y";
        rs.m_hstmt=script.existing?reinterpret_cast<HSTMT>(0xbeef):nullptr;
        if(script.existing)reinterpret_cast<OpenMFC_CPtrList*>(db.m_listRecordsets)->AddHead(&rs);
        auto** original=*reinterpret_cast<void***>(&rs);void* table[27];memcpy(table,original,sizeof(table));
        table[5]=reinterpret_cast<void*>(&forbiddenOpen);table[6]=reinterpret_cast<void*>(&close);table[7]=reinterpret_cast<void*>(&move);
        table[15]=reinterpret_cast<void*>(&connection);table[16]=reinterpret_cast<void*>(&sql);table[17]=reinterpret_cast<void*>(&options);
        table[19]=reinterpret_cast<void*>(&ordinary);table[20]=reinterpret_cast<void*>(&bulk);table[21]=reinterpret_cast<void*>(&checkStatus);table[23]=reinterpret_cast<void*>(&prebind);table[24]=reinterpret_cast<void*>(&throwStatus);
        *reinterpret_cast<void***>(&rs)=table;
        auto** originalDB=*reinterpret_cast<void***>(&db);void* dbTable[12];memcpy(dbTable,originalDB,sizeof(dbTable));dbTable[5]=reinterpret_cast<void*>(&databaseOpen);*reinterpret_cast<void***>(&db)=dbTable;
        using namespace recordset_execute_test;
        steps=script.options&CRecordset::executeDirect?std::vector<Step>{{Direct,SQL_SUCCESS}}:
            script.prepareError?std::vector<Step>{{Prepare,SQL_ERROR,FALSE}}:std::vector<Step>{{Prepare,SQL_SUCCESS},{Run,SQL_SUCCESS}};
        position=0;accepted=69;failure=checkFailure=0;recordset_execute_test::events.clear();
        longbinary_rfx_test::reset({},{{L"HY000",5,L"[Driver] failed to prepare"}});
        int error=0;BOOL returned=-1;RETCODE databaseError=0;
        try{returned=open(&rs,script.type,script.table?L"t":nullptr,script.options);}
        catch(CDBException* value){error=3;databaseError=value->m_nRetCode;value->Delete();}
        catch(CException* value){if(std::find(pending.begin(),pending.end(),value)!=pending.end())error=2;else{error=-1;value->Delete();}}
        catch(int value){error=value==19?-1:value;}
        bool success=returned==TRUE;
        if(success)check(rs.m_lOpen==1 && rs.m_bBOF==69 && rs.m_bEOF==69 && rs.m_rgRowStatus && rs.m_rgODBCFieldInfos && position==steps.size(),
            "Open executes and describes the query, allocates its rowset, calls derived movement and copies the full EOF value into BOF");
        auto* list=reinterpret_cast<OpenMFC_CPtrList*>(db.m_listRecordsets);
        result.insert(result.end(),events.begin(),events.end());for(auto& e:recordset_execute_test::events)result.insert(result.end(),e.begin(),e.end());
        result.insert(result.end(),{90,returned,error,databaseError,closeCalls,deleted,rs.m_lOpen,rs.m_nOpenType,rs.m_dwOptions,rs.m_bBOF,rs.m_bEOF,
            rs.m_bUpdatable,rs.m_bAppendable,rs.m_bCheckCacheForDirtyFields,rs.m_nFields,rs.m_nParams,rs.m_nResultCols,rs.m_nFieldsBound,
            rs.m_rgFieldInfos!=nullptr,rs.m_pbFieldFlags!=nullptr,rs.m_pbParamFlags!=nullptr,rs.m_plParamLength!=nullptr,rs.m_rgRowStatus!=nullptr,
            rs.m_dwRowsetSize,rs.m_dwAllocatedRowsetSize,list->m_nCount,INT64(UINT_PTR(rs.m_hstmt)),INT64(position)});
        string(result,rs.m_strSQL);string(result,rs.m_strRequerySQL);string(result,rs.m_strRequeryFilter);string(result,rs.m_strRequerySort);
        for(UINT i=0;rs.m_pbFieldFlags && i<rs.m_nFields;++i)result.push_back(rs.m_pbFieldFlags[i]);result.push_back(-1);
        script.failStage=0;script.closeFailure=0;
        for(auto* value:pending)if(value)value->Delete();pending.clear();
        rs.CRecordset::Close();db.m_hdbc=nullptr;*reinterpret_cast<void***>(&rs)=original;*reinterpret_cast<void***>(&db)=originalDB;
        rs.m_pDatabase=nullptr;longbinary_rfx_test::reset({});
    };
    for(UINT type:{0u,1u,2u,3u,UINT(-1)})for(DWORD options:{0ul,4ul,8ul,0x200ul,0x600ul,0x2000ul})for(bool existing:{false,true})for(bool table:{false,true}) {
        Script s;s.type=type;s.options=options;s.existing=existing;s.table=table;run(s);
    }
    for(UINT fields:{0u,2u})for(UINT params:{0u,2u})for(bool late:{false,true}) {Script s;s.fields=fields;s.params=params;s.lateFields=late;run(s);}
    for(UINT type:{4u,17u,UINT(-2),2u,UINT(-1)})for(DWORD options:{12ul,0x4000ul,0x10ul}) {Script s;s.type=type;s.options=options;run(s);}
    {Script s;s.existing=true;s.alreadyOpen=true;run(s);s.alreadyOpen=false;s.type=UINT(-1);s.defaultType=2;s.options=0x10;run(s);}
    {Script s;s.closedDatabase=true;run(s);s.openResult=69;run(s);}
    for(int stage=1;stage<=10;++stage)for(bool framework:{false,true}) {
        Script s;s.failStage=stage;s.framework=framework;run(s);
    }
    for(int point=1;point<=6;++point) {Script s;s.failStage=10;s.failCheck=point;s.framework=true;run(s);}
    for(int closeFailure:{1,2}) {Script s;s.failStage=5;s.framework=true;s.closeFailure=closeFailure;run(s);}
    {Script s;s.prepareError=true;run(s);}
    return result;
}
}
static void recordsetOpen() {
    using namespace recordset_open_test;
    auto oldAllocation=database_execute_test::allocationHook;auto oldColumns=database_execute_test::columnsHook;
    auto oldDescribe=recordset_allocation_test::describeHook;auto oldOption=database_layout_test::optionHook;
    auto oldDirect=database_execute_test::directHook;database_execute_test::directHook=&recordset_execute_test::direct;
    database_execute_test::allocationHook=&allocate;database_execute_test::columnsHook=&columns;recordset_allocation_test::describeHook=&describe;database_layout_test::optionHook=&option;
    defaultPrebind(&impl__PreBindFields_CRecordset__UEAAXXZ);
    auto expected=exercise(&impl__Open_CRecordset__UEAAHIPEB_WK_Z);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_OPEN_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset Open pipeline comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"recordset Open oracle uses the verified MFC image build");
            if(pinned) {
                defaultPrebind(reinterpret_cast<void (MS_ABI*)(CRecordset*)>(base+0x27d0));
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e90d0,reinterpret_cast<void*>(&SQLAllocStmt)},{0x3e90c0,reinterpret_cast<void*>(&SQLNumResultCols)},
                    {0x3e9070,reinterpret_cast<void*>(&SQLDescribeColW)},{0x3e9080,reinterpret_cast<void*>(&SQLSetStmtOption)},
                    {0x3e90f8,reinterpret_cast<void*>(&SQLPrepareW)},{0x3e9100,reinterpret_cast<void*>(&SQLExecute)},
                    {0x3e90c8,reinterpret_cast<void*>(&SQLExecDirectW)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                auto actual=exercise(reinterpret_cast<Open>(base+0x1f8380));
                if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("Open mismatch %zu local %lld native %lld\n",i,expected[i],actual[i]);break;}
                check(actual==expected,"Open validation, statement reuse, complete helper ordering, late fields, virtual callbacks and exception cleanup match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::allocationHook=oldAllocation;database_execute_test::columnsHook=oldColumns;recordset_allocation_test::describeHook=oldDescribe;database_layout_test::optionHook=oldOption;
    database_execute_test::directHook=oldDirect;
}
