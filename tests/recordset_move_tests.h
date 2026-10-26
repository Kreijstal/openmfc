#pragma once
extern "C" RETCODE MS_ABI impl__FetchData_CRecordset__QEAAFGJPEA_K_Z(CRecordset*,WORD,long,SQLULEN*);
extern "C" void MS_ABI impl__SkipDeletedRecords_CRecordset__QEAAXGJPEA_KPEAF_Z(CRecordset*,WORD,long,SQLULEN*,RETCODE*);
extern "C" void MS_ABI impl__CheckRowsetCurrencyStatus_CRecordset__QEAAXGJ_Z(CRecordset*,WORD,long);
extern "C" void MS_ABI impl__CheckRowsetError_CRecordset__UEAAXF_Z(CRecordset*,RETCODE);
extern "C" void MS_ABI impl__SetRowsetCurrencyStatus_CRecordset__UEAAXFGJ_K_Z(CRecordset*,RETCODE,WORD,long,SQLULEN);
extern "C" void MS_ABI impl__Fixups_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__Move_CRecordset__UEAAXJG_Z(CRecordset*,long,WORD);
namespace recordset_move_test {
struct API {
    decltype(&impl__FetchData_CRecordset__QEAAFGJPEA_K_Z) fetch;
    decltype(&impl__SkipDeletedRecords_CRecordset__QEAAXGJPEA_KPEAF_Z) skip;
    decltype(&impl__CheckRowsetCurrencyStatus_CRecordset__QEAAXGJ_Z) checkCurrency;
    decltype(&impl__CheckRowsetError_CRecordset__UEAAXF_Z) checkError;
    decltype(&impl__SetRowsetCurrencyStatus_CRecordset__UEAAXFGJ_K_Z) currency;
    decltype(&impl__Fixups_CRecordset__QEAAXXZ) fixups;
    decltype(&impl__Move_CRecordset__UEAAXJG_Z) move;
};
using Event=std::array<INT64,7>;
static std::vector<Event> events;
static void event(INT64 kind,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0){events.push_back({kind,a,b,c,d,e,f});}
struct Step {RETCODE code;UWORD status=SQL_ROW_SUCCESS;SQLULEN rows=3;SQLLEN length=sizeof(int);int value=42;};
static std::vector<Step> steps;
static size_t position;
static CRecordset* active;
static const API* api;
static bool accept,returningThrow,delegateError,delegateCurrency;
static int failure,deleted;
static UINT boundCount;
static bool populate;
static int fieldValue;
struct Failure:CDBException {~Failure() override{++deleted;}};
static void fail(int kind) {if(failure==kind)throw 77;if(failure==kind+10)throw static_cast<CException*>(new Failure);}
static Step take() {if(position>=steps.size()){std::printf("fetch script exhausted at %zu\n",position);std::abort();}auto result=steps[position++];fail(1);return result;}
static void fill(const Step& step) {
    if(populate && (step.code==SQL_SUCCESS || step.code==SQL_SUCCESS_WITH_INFO)) {
        auto& binding=binary_rfx_test::binding;
        check(binding.data==&fieldValue && binding.length && binding.cType==SQL_C_LONG && binding.capacity==4,
            "base movement fetches into a real RFX-bound integer and its recordset length buffer");
        *static_cast<int*>(binding.data)=step.value;*binding.length=step.length;
    }
}
static SQLRETURN SQL_API fetch(SQLHSTMT statement){event(1,INT64(UINT_PTR(statement)));auto step=take();fill(step);return step.code;}
static SQLRETURN SQL_API extended(SQLHSTMT statement,SQLUSMALLINT direction,SQLLEN rows,SQLULEN* fetched,SQLUSMALLINT* status) {
    event(2,INT64(UINT_PTR(statement)),direction,rows,fetched==&active->m_dwRowsFetched,status==active->m_rgRowStatus,fetched!=nullptr);
    auto step=take();if(fetched)*fetched=step.rows;if(status)status[0]=step.status;fill(step);return step.code;
}
static void MS_ABI error(CRecordset* rs,RETCODE code){event(3,code,rs->m_bDeleted,rs->m_dwRowsFetched);fail(2);if(delegateError)api->checkError(rs,code);}
static void MS_ABI currency(CRecordset* rs,RETCODE code,WORD direction,long rows,SQLULEN fetched) {
    event(4,code,direction,rows,INT64(fetched),rs->m_nFieldsBound,rs->m_bDeleted);fail(3);
    if(delegateCurrency)api->currency(rs,code,direction,rows,fetched);
}
static BOOL MS_ABI checkStatus(const CRecordset*,RETCODE code){event(5,code);fail(4);return accept?69:0;}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT statement){event(6,code,INT64(UINT_PTR(statement)));fail(5);if(!returningThrow)throw 700+code;}
static void exchange(CRecordset* rs,CFieldExchange* fx,bool bulk) {
    event(7,fx->m_nOperation,bulk,INT64(UINT_PTR(fx->m_hstmt)),rs->m_bBOF,rs->m_bEOF,rs->m_nEditMode);
    check(fx->m_prs==rs && fx->m_nFieldType==UINT(-1) && !fx->m_pvField,"movement field exchanges preserve the native owner, field defaults and null field selector");
    if(failure==int(fx->m_nOperation)+100)throw 78;
    if(failure==int(fx->m_nOperation)+200)throw static_cast<CException*>(new Failure);
    fx->m_nFields=boundCount;
}
static void MS_ABI ordinary(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,false);}
static void MS_ABI bulk(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,true);}
static void MS_ABI realExchange(CRecordset*,CFieldExchange* fx) {
    event(10,fx->m_nOperation);fx->SetFieldType(CFieldExchange::outputColumn);
    impl__RFX_Int__YAXPEAVCFieldExchange__PEB_WAEAH_Z(fx,L"value",&fieldValue);
}
static void MS_ABI navigation(CRecordset*,long rows,WORD direction) {event(12,rows,direction);}
struct Fixture {
    CDatabase db;CRecordset rs;void** original;void* table[27];BYTE flags[3]{0x81,0x82,0x83};UWORD status[2]{0,0};
    Fixture(){active=&rs;original=*reinterpret_cast<void***>(&rs);memcpy(table,original,sizeof(table));table[9]=reinterpret_cast<void*>(&error);table[19]=reinterpret_cast<void*>(&ordinary);table[20]=reinterpret_cast<void*>(&bulk);table[21]=reinterpret_cast<void*>(&checkStatus);table[22]=reinterpret_cast<void*>(&currency);table[24]=reinterpret_cast<void*>(&throwStatus);*reinterpret_cast<void***>(&rs)=table;
        rs.m_pDatabase=&db;db.m_hdbc=reinterpret_cast<HDBC>(0xface);db.m_dwUpdateOptions=0;
        rs.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);rs.m_nFields=3;rs.m_nFieldsBound=3;rs.m_pbFieldFlags=flags;rs.m_rgRowStatus=status;
        rs.m_nOpenType=CRecordset::snapshot;rs.m_bScrollable=TRUE;rs.m_bBOF=FALSE;rs.m_bEOF=FALSE;rs.m_bEOFSeen=FALSE;rs.m_lRecordCount=5;rs.m_lCurrentRecord=1;
        rs.m_bDeleted=69;rs.m_dwRowsFetched=91;rs.m_nEditMode=2;rs.m_lOpen=1;rs.m_dwOptions=0;rs.m_bCheckCacheForDirtyFields=TRUE;
        events.clear();steps={{SQL_SUCCESS}};position=0;accept=true;returningThrow=false;delegateError=delegateCurrency=false;failure=deleted=0;boundCount=3;populate=false;
    }
    ~Fixture(){rs.m_hstmt=nullptr;rs.m_pDatabase=nullptr;rs.m_rgRowStatus=nullptr;rs.m_pbFieldFlags=nullptr;*reinterpret_cast<void***>(&rs)=original;db.m_hdbc=nullptr;}
};
static int normalize(CException* exception){exception->Delete();return -1;}
static void save(std::vector<Event>& result,Fixture& f,int caught=0) {
    result.insert(result.end(),events.begin(),events.end());auto& rs=f.rs;
    result.push_back({90,caught,rs.m_lCurrentRecord,rs.m_lRecordCount,rs.m_bEOFSeen,rs.m_bBOF,rs.m_bEOF});
    result.push_back({91,rs.m_bDeleted,INT64(rs.m_dwRowsFetched),rs.m_nFieldsBound,rs.m_nEditMode,INT64(position),deleted});
    result.push_back({92,f.flags[0],f.flags[1],f.flags[2],rs.m_nFields,rs.m_dwOptions,rs.m_nOpenType});events.clear();
}
static std::vector<Event> currencies(const API& functions) {
    std::vector<Event> result;
    for(WORD direction:{WORD(0),WORD(1),WORD(2),WORD(3),WORD(4),WORD(5),WORD(6),WORD(8),WORD(17)})
    for(long rows:{-2147483647L-1,-3L,-1L,0L,1L,3L,2147483647L})for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR)})
    for(long current:{-2L,-1L,0L,4L,2147483647L})for(BOOL seen:{FALSE,BOOL(69)}) {
        Fixture f;f.rs.m_lCurrentRecord=current;f.rs.m_bEOFSeen=seen;functions.currency(&f.rs,code,direction,rows,0x100000001ULL);save(result,f);
    }
    for(WORD direction:{WORD(0),WORD(1),WORD(2),WORD(4),WORD(5),WORD(6),WORD(8)})for(long rows:{-1L,0L,1L})for(bool scroll:{false,true})for(bool eof:{false,true})for(bool bof:{false,true})for(UINT type:{1u,3u})for(bool returns:{false,true}) {
        Fixture f;f.rs.m_bScrollable=scroll;f.rs.m_bEOF=eof;f.rs.m_bBOF=bof;f.rs.m_nOpenType=type;returningThrow=returns;int caught=0;
        try{functions.checkCurrency(&f.rs,direction,rows);}catch(int value){caught=value;}
        save(result,f,caught);
    }
    return result;
}
static std::vector<Event> errors(const API& functions) {
    std::vector<Event> result;
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA),RETCODE(SQL_NEED_DATA),RETCODE(SQL_INVALID_HANDLE)})
    for(int diagnostic=0;diagnostic<6;++diagnostic)for(DWORD options:{0ul,1ul,2ul,3ul})for(bool binary:{false,true})for(bool accepted:{false,true})for(bool returns:{false,true}) {
        Fixture f;f.db.m_dwUpdateOptions=options;f.rs.m_bLongBinaryColumns=binary;accept=accepted;returningThrow=returns;
        using D=longbinary_rfx_test::Diagnostic;std::vector<D> diagnostics;
        if(diagnostic==1)diagnostics={{L"01004",5,L"truncated"}};
        else if(diagnostic==2)diagnostics={{L"01S01",5,L"row error"}};
        else if(diagnostic==3)diagnostics={{L"01004",5,L"truncated"},{L"01S01",7,L"row error"}};
        else if(diagnostic==4)diagnostics={{L"HY000",5,L"[State:01004] driver origin"}};
        else if(diagnostic==5)diagnostics={{L"01s01",5,L"lowercase"}};
        longbinary_rfx_test::reset({},diagnostics);int caught=0;
        try{functions.checkError(&f.rs,code);}catch(int value){caught=value;}catch(CException* value){caught=normalize(value);}
        event(8,INT64(longbinary_rfx_test::diagnosticReads.size()));save(result,f,caught);
    }
    longbinary_rfx_test::reset({});return result;
}
static std::vector<Event> fetching(const API& functions) {
    api=&functions;std::vector<Event> result;
    for(UINT type:{0u,1u,2u,3u})for(DWORD options:{0ul,0x800ul})for(WORD direction:{WORD(1),WORD(5),WORD(6)})for(long rows:{-2147483647L-1,-3L,0L,3L,2147483647L})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR)})for(UWORD status:{UWORD(SQL_ROW_SUCCESS),UWORD(SQL_ROW_DELETED)}) {
        Fixture f;f.rs.m_nOpenType=type;f.rs.m_dwOptions=options;steps={{SQL_STILL_EXECUTING},{code,status,0x100000002ULL}};
        RETCODE returned=functions.fetch(&f.rs,direction,rows,&f.rs.m_dwRowsFetched);event(9,returned);save(result,f);
    }
    for(bool extendedMode:{false,true})for(int failAt:{1,2,11,12}) {
        Fixture f;f.rs.m_nOpenType=extendedMode?1:2;failure=failAt;int caught=0;
        try{functions.fetch(&f.rs,1,1,&f.rs.m_dwRowsFetched);}catch(int value){caught=value;}catch(CException* value){caught=normalize(value);}save(result,f,caught);
    }
    for(bool extendedMode:{false,true}) {
        Fixture f;f.rs.m_nOpenType=extendedMode?1:2;int caught=0;
        try{functions.fetch(&f.rs,6,-3,nullptr);}catch(int value){caught=value==19?-1:value;}catch(CException* value){caught=normalize(value);}save(result,f,caught);
    }
    for(WORD direction:{WORD(1),WORD(2),WORD(3),WORD(4),WORD(5),WORD(6),WORD(8)})for(long rows:{-3L,-1L,0L,1L,3L})for(int deletedCount:{0,1,2}) {
        Fixture f;steps.clear();for(int i=0;i<deletedCount;++i)steps.push_back({SQL_SUCCESS,SQL_ROW_DELETED});
        for(int i=0;i<4;++i)steps.push_back({SQL_SUCCESS});steps.push_back({SQL_NO_DATA});RETCODE returned=-8;
        functions.skip(&f.rs,direction,rows,&f.rs.m_dwRowsFetched,&returned);event(9,returned);save(result,f);
    }
    return result;
}
static std::vector<Event> moving(const API& functions) {
    api=&functions;std::vector<Event> result;
    for(int bound:{-1,0,3})for(UINT fields:{0u,3u})for(bool cache:{false,true})for(DWORD options:{0ul,0x10ul,0x200ul})for(WORD direction:{WORD(1),WORD(2),WORD(4),WORD(5),WORD(6),WORD(8)})for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_NO_DATA)}) {
        Fixture f;f.rs.m_nFieldsBound=bound;f.rs.m_nFields=fields;f.rs.m_bCheckCacheForDirtyFields=cache;f.rs.m_dwOptions=options;
        steps={{code,SQL_ROW_SUCCESS}};delegateCurrency=true;
        functions.move(&f.rs,direction==4?-1:1,direction);save(result,f);
    }
    for(int failAt:{1,2,3,11,12,13,102,105,114,202,205,214}) {
        Fixture f;f.rs.m_nFieldsBound=0;failure=failAt;delegateCurrency=true;int caught=0;
        try{functions.move(&f.rs,1,1);}catch(int value){caught=value;}catch(CException* value){caught=normalize(value);}save(result,f,caught);
    }
    {Fixture f;functions.fixups(&f.rs);save(result,f);}
    for(UINT type:{1u,2u})for(bool nullValue:{false,true}) {
        Fixture f;CFieldInfo info{};CODBCFieldInfo metadata{};metadata.m_nSQLType=SQL_INTEGER;metadata.m_nPrecision=5;
        f.rs.m_nFields=1;f.rs.m_nFieldsBound=0;f.rs.m_rgFieldInfos=&info;f.rs.m_rgODBCFieldInfos=&metadata;f.rs.m_nOpenType=type;
        f.table[19]=reinterpret_cast<void*>(&realExchange);populate=delegateCurrency=delegateError=true;fieldValue=17;binary_rfx_test::resetSQL();
        steps={{SQL_SUCCESS,SQL_ROW_SUCCESS,1,nullValue?SQL_NULL_DATA:SQLLEN(sizeof(int)),42}};
        functions.move(&f.rs,1,SQL_FETCH_NEXT);
        check(f.rs.m_nFieldsBound==1 && binary_rfx_test::binds==1 && fieldValue==(nullValue?0x7ee4:42) &&
            f.rs.IsFieldStatusNull(0)==(nullValue?2:0) && f.rs.m_lCurrentRecord==0 && !f.rs.m_bBOF && !f.rs.m_bEOF && info.m_nDataType==3,
            "base Move binds declared fields, initializes their cache, fetches values and applies native RFX NULL fixups before returning");
        event(11,fieldValue,info.m_nLength,info.m_nDataType,f.rs.GetBoundFieldIndex(&fieldValue));save(result,f);
        f.rs.m_rgFieldInfos=nullptr;f.rs.m_rgODBCFieldInfos=nullptr;
    }
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLExtendedFetch(SQLHSTMT statement,SQLUSMALLINT direction,SQLLEN rows,SQLULEN* fetched,SQLUSMALLINT* status) {
    return recordset_move_test::extended(statement,direction,rows,fetched,status);
}
static void recordsetMove() {
    using namespace recordset_move_test;
    auto previous=database_execute_test::fetchHook;database_execute_test::fetchHook=&fetch;
    {
        Fixture f;f.table[7]=reinterpret_cast<void*>(&navigation);f.rs.m_bEOF=f.rs.m_bBOF=69;f.rs.m_lOpen=0;f.rs.m_lCurrentRecord=-2;
        f.rs.MoveFirst();f.rs.MoveLast();f.rs.MoveNext();f.rs.MovePrev();f.rs.SetAbsolutePosition(-7);
        check(events==std::vector<Event>{{12,1,SQL_FETCH_FIRST},{12,-1,SQL_FETCH_LAST},{12,1,SQL_FETCH_NEXT},{12,-1,SQL_FETCH_PRIOR},{12,-7,SQL_FETCH_ABSOLUTE}} && f.rs.m_lCurrentRecord==-2,
            "navigation shortcuts match the SDK-compiled caller's virtual Move arguments without bypassing EOF errors or assigning a closed cursor position");
    }
    API local{&impl__FetchData_CRecordset__QEAAFGJPEA_K_Z,&impl__SkipDeletedRecords_CRecordset__QEAAXGJPEA_KPEAF_Z,
        &impl__CheckRowsetCurrencyStatus_CRecordset__QEAAXGJ_Z,&impl__CheckRowsetError_CRecordset__UEAAXF_Z,
        &impl__SetRowsetCurrencyStatus_CRecordset__UEAAXFGJ_K_Z,&impl__Fixups_CRecordset__QEAAXXZ,&impl__Move_CRecordset__UEAAXJG_Z};
    auto states=currencies(local);auto warnings=errors(local);auto reads=fetching(local);auto moves=moving(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_MOVE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset movement comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"movement oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};Patch patches[]={
                    {0x3e90b8,reinterpret_cast<void*>(&SQLFetch)},{0x3e9068,reinterpret_cast<void*>(&SQLExtendedFetch)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                API native{reinterpret_cast<decltype(local.fetch)>(base+0x1fb4b0),reinterpret_cast<decltype(local.skip)>(base+0x1fb580),
                    reinterpret_cast<decltype(local.checkCurrency)>(base+0x1fb3d0),reinterpret_cast<decltype(local.checkError)>(base+0x1f8b80),
                    reinterpret_cast<decltype(local.currency)>(base+0x1fb680),reinterpret_cast<decltype(local.fixups)>(base+0x1fcdf0),reinterpret_cast<decltype(local.move)>(base+0x1f8a30)};
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(currencies(native),states,"cursor currency, boundaries, record counts and integer wrap match native MFC");
                compare(errors(native),warnings,"rowset warnings, truncation exceptions and virtual status handling match native MFC");
                compare(fetching(native),reads,"forward and extended fetching, deleted rows, asynchronous retries and failures match native MFC");
                compare(moving(native),moves,"base Move initialization, cache binding, currency dispatch, field fixup and interrupted state match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::fetchHook=previous;
}
