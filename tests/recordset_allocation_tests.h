#pragma once
extern "C" void MS_ABI impl__AllocRowset_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__SetRowsetSize_CRecordset__UEAAXK_Z(CRecordset*,DWORD);
extern "C" UINT MS_ABI impl__BindFieldsToColumns_CRecordset__QEAAIXZ(CRecordset*);
extern "C" void MS_ABI impl__AllocDataCache_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__InitRecord_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__AllocAndCacheFieldInfo_CRecordset__IEAAXXZ(CRecordset*);
namespace recordset_allocation_test {
using DescribeHook=SQLRETURN (SQL_API*)(SQLHSTMT,SQLUSMALLINT,SQLWCHAR*,SQLSMALLINT,SQLSMALLINT*,SQLSMALLINT*,SQLULEN*,SQLSMALLINT*,SQLSMALLINT*);
static DescribeHook describeHook=nullptr;
struct API {
    decltype(&impl__AllocRowset_CRecordset__IEAAXXZ) allocate;
    decltype(&impl__SetRowsetSize_CRecordset__UEAAXK_Z) resize;
    decltype(&impl__BindFieldsToColumns_CRecordset__QEAAIXZ) bind;
    decltype(&impl__AllocDataCache_CRecordset__QEAAXXZ) cache,initialize,metadata;
};
using Event=std::array<INT64,6>;
static std::vector<Event> events;
static int failOperation,deleted;
static UINT exchangeCount=5;
static CRecordset* active;
static void event(INT64 kind,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0){events.push_back({kind,a,b,c,d,e});}
struct Failure: CDBException {~Failure() override {++deleted;}};
static void exchange(CRecordset* rs,CFieldExchange* fx,bool bulk) {
    check(fx->m_prs==rs && fx->m_nFieldType==UINT(-1) && !fx->m_pvField &&
        fx->m_hstmt==(fx->m_nOperation==CFieldExchange::BindFieldToColumn?rs->m_hstmt:nullptr),
        "recordset allocation exchanges use native operations, field defaults and binding statement handles");
    event(bulk?1:2,fx->m_nOperation,rs->m_dwRowsetSize,rs->m_dwAllocatedRowsetSize,rs->m_rgRowStatus!=nullptr,rs->m_nFieldsBound);
    if(failOperation==int(fx->m_nOperation)+1)throw static_cast<CException*>(new Failure);
    if(failOperation==int(fx->m_nOperation)+101)throw 77;
    fx->m_nFields=exchangeCount;
}
static void MS_ABI bulk(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,true);}
static void MS_ABI ordinary(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,false);}
static void MS_ABI resizeOverride(CRecordset* rs,DWORD size){event(3,size,rs->m_rgRowStatus!=nullptr);rs->m_dwRowsetSize=size+1;}
static SQLRETURN SQL_API option(SQLHSTMT statement,SQLUSMALLINT kind,SQLULEN value){event(4,INT64(UINT_PTR(statement)),kind,INT64(value),active->m_nFieldsBound,active->m_dwAllocatedRowsetSize);return SQL_ERROR;}
struct Fixture {
    CRecordset rs;
    void** original;void* table[27];
    Fixture(){active=&rs;original=*reinterpret_cast<void***>(&rs);std::memcpy(table,original,sizeof(table));table[8]=reinterpret_cast<void*>(&resizeOverride);table[19]=reinterpret_cast<void*>(&ordinary);table[20]=reinterpret_cast<void*>(&bulk);*reinterpret_cast<void***>(&rs)=table;
        rs.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);rs.m_lOpen=1;rs.m_nFieldsBound=19;}
    ~Fixture(){*reinterpret_cast<void***>(&rs)=original;std::free(rs.m_rgRowStatus);rs.m_rgRowStatus=nullptr;delete[] rs.m_rgODBCFieldInfos;rs.m_rgODBCFieldInfos=nullptr;rs.m_hstmt=nullptr;}
};
static std::vector<Event> exercise(const API& api) {
    std::vector<Event> result;
    auto save=[&](CRecordset& rs,int caught=0){result.insert(result.end(),events.begin(),events.end());result.push_back({90,rs.m_dwRowsetSize,rs.m_dwAllocatedRowsetSize,rs.m_rgRowStatus!=nullptr,rs.m_nFieldsBound,caught});events.clear();};
    for(DWORD options:{DWORD(0),DWORD(0x200),DWORD(0x400),DWORD(0x600)}) {
        Fixture fixture;auto& rs=fixture.rs;rs.m_dwOptions=options;rs.m_dwRowsetSize=7;rs.m_dwAllocatedRowsetSize=13;events.clear();failOperation=0;api.allocate(&rs);
        check(options&0x200?rs.m_dwRowsetSize==8 && !rs.m_rgRowStatus && events.size()==1:rs.m_dwRowsetSize==1 && rs.m_rgRowStatus && events.empty(),
            "AllocRowset allocates one status for single-row fetching or delegates the configured size through the derived resize virtual");save(rs);
    }
    for(bool open:{false,true})for(DWORD options:{DWORD(0),DWORD(0x200),DWORD(0x600)})for(DWORD allocated:{0ul,3ul,8ul})for(DWORD size:{1ul,3ul,7ul})for(bool cache:{false,true}) {
        Fixture fixture;auto& rs=fixture.rs;rs.m_hstmt=open?reinterpret_cast<HSTMT>(0xbeef):nullptr;rs.m_dwOptions=options;rs.m_dwAllocatedRowsetSize=allocated;rs.m_dwRowsetSize=3;
        rs.m_rgRowStatus=static_cast<UWORD*>(std::malloc((allocated?allocated:1)*2));rs.m_nFields=2;rs.m_bCheckCacheForDirtyFields=cache;auto* old=rs.m_rgRowStatus;
        events.clear();failOperation=0;api.resize(&rs,size);
        if(!open)check(rs.m_dwRowsetSize==size && rs.m_rgRowStatus==old && rs.m_dwAllocatedRowsetSize==allocated && events.empty(),"closed recordsets configure rowset size without releasing or allocating buffers");
        else if(!(options&0x200))check(rs.m_dwRowsetSize==3 && rs.m_rgRowStatus==old && events.empty(),"open single-row recordsets ignore resize requests");
        else {
            check(events.back()[0]==4 && events.back()[1]==0xbeef && events.back()[2]==SQL_ROWSET_SIZE && rs.m_nFieldsBound==19,
                "bulk resizing restores its old binding count before setting the ODBC rowset size and ignores option return status");
            if((options&0x400) && (!allocated || allocated<size))check(rs.m_dwRowsetSize==3 && !rs.m_dwAllocatedRowsetSize,
                "user-owned bulk buffers retain their configured size and do not run allocation or binding exchanges");
            else check(rs.m_dwRowsetSize==size && rs.m_dwAllocatedRowsetSize==(allocated>=size?allocated:size),"bulk resizing reuses sufficient capacity and grows owned buffers when needed");
        }
        save(rs);
    }
    for(UINT fields:{0u,2u})for(bool bulkMode:{false,true})for(bool cache:{false,true}) {
        Fixture fixture;auto& rs=fixture.rs;rs.m_nFields=fields;rs.m_dwOptions=bulkMode?0x200:0;rs.m_bCheckCacheForDirtyFields=cache;
        events.clear();failOperation=0;api.initialize(&rs);
        check(rs.m_nFieldsBound==(fields?5:-1) && events.size()==(fields?1+int(cache):0),"recordset initialization binds declared fields, allocates enabled caches and uses the native no-fields sentinel");save(rs);
        events.clear();check(api.bind(&rs)==exchangeCount,"field binding returns the derived exchange's field count");api.cache(&rs);save(rs);
    }
    for(UINT count:{0u,0x80000000u,0xffffffffu}) {
        Fixture fixture;fixture.rs.m_nFields=2;fixture.rs.m_dwOptions=0x200;events.clear();failOperation=0;exchangeCount=count;
        api.initialize(&fixture.rs);
        check(UINT(fixture.rs.m_nFieldsBound)==count && api.bind(&fixture.rs)==count,"binding and initialization preserve the full native unsigned field count");save(fixture.rs);
    }
    exchangeCount=5;
    for(UINT op:{UINT(CFieldExchange::DeleteMultiRowBuffer),UINT(CFieldExchange::AllocMultiRowBuffer),UINT(CFieldExchange::BindFieldToColumn),UINT(CFieldExchange::AllocCache)})for(bool foreign:{false,true}) {
        Fixture fixture;auto& rs=fixture.rs;rs.m_dwOptions=0x200;rs.m_dwRowsetSize=3;rs.m_dwAllocatedRowsetSize=3;rs.m_rgRowStatus=static_cast<UWORD*>(std::malloc(6));rs.m_nFields=2;
        events.clear();deleted=0;failOperation=int(op)+(foreign?101:1);int caught=0;
        try{api.resize(&rs,7);}catch(CException* error){caught=1;error->Delete();}catch(int value){caught=value;}
        check(caught==(foreign?77:1) && deleted==int(!foreign) && std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==4;}),
            "rowset resizing propagates callback failures without setting the driver option or synthesizing rollback state");save(rs,caught);failOperation=0;
    }
    for(bool nullObject:{false,true}) {
        Fixture fixture;auto before=fixture.rs.m_dwRowsetSize;bool caught=false;
        try{api.resize(nullObject?nullptr:&fixture.rs,nullObject?1:0);}catch(int value){caught=value==19;}catch(CException* error){caught=true;error->Delete();}
        check(caught && fixture.rs.m_dwRowsetSize==before,"rowset resizing rejects null objects and zero sizes before querying the statement");
    }
    return result;
}
// Metadata-specific ODBC scripting observes the real allocation and name-buffer paths.
static RETCODE numberCode,describeCode;
static SQLSMALLINT number,reported;
static BOOL accepted;
static bool returningThrow;
static bool acceptNumberSuccess;
static int numberCalls,describeCalls;
static bool retry;
static SQLRETURN SQL_API columns(SQLHSTMT statement,SQLSMALLINT* out){++numberCalls;*out=number;RETCODE code=retry && numberCalls==1?SQL_STILL_EXECUTING:numberCode;event(10,INT64(UINT_PTR(statement)),out==&active->m_nResultCols,code,number);return code;}
static SQLRETURN SQL_API describe(SQLHSTMT statement,SQLUSMALLINT field,SQLWCHAR* name,SQLSMALLINT capacity,SQLSMALLINT* length,SQLSMALLINT* type,SQLULEN* precision,SQLSMALLINT* scale,SQLSMALLINT* nullable) {
    ++describeCalls;std::wcscpy(reinterpret_cast<wchar_t*>(name),field==1?L"first":L"second");*length=reported;*type=SQL_VARCHAR;*precision=0x123456789abcdef0ULL+field;*scale=-7;*nullable=SQL_NULLABLE;
    auto* data=reinterpret_cast<CStringData*>(name)-1;RETCODE code=retry && describeCalls%2?SQL_STILL_EXECUTING:describeCode;
    event(11,field,capacity,data->nAllocLength,data->nRefs,code);check(statement==active->m_hstmt && capacity==256 && data->nAllocLength>=257,"metadata descriptions receive the current statement and native 256-character driver capacity backed by 257-character writable storage");return code;
}
static BOOL MS_ABI checkMetadata(const CRecordset* rs,RETCODE code){event(12,code,rs->m_nResultCols);return accepted || (acceptNumberSuccess && code==SQL_SUCCESS);}
static void MS_ABI throwMetadata(CRecordset*,RETCODE code,HSTMT stmt){event(13,code,INT64(UINT_PTR(stmt)));if(!returningThrow)throw 700+code;}
static std::vector<INT64> metadata(const API& api) {
    std::vector<INT64> result;
    for(SQLSMALLINT count:{SQLSMALLINT(0),SQLSMALLINT(1),SQLSMALLINT(2)})for(SQLSMALLINT length:{SQLSMALLINT(0),SQLSMALLINT(3),SQLSMALLINT(-1),SQLSMALLINT(-2),SQLSMALLINT(256),SQLSMALLINT(257),SQLSMALLINT(258)})for(bool asynchronous:{false,true}) {
        Fixture fixture;auto& rs=fixture.rs;fixture.table[21]=reinterpret_cast<void*>(&checkMetadata);fixture.table[24]=reinterpret_cast<void*>(&throwMetadata);
        number=count;reported=length;retry=asynchronous;numberCode=SQL_SUCCESS;describeCode=SQL_SUCCESS_WITH_INFO;accepted=69;acceptNumberSuccess=false;returningThrow=false;numberCalls=describeCalls=0;events.clear();int error=0;
        try{api.metadata(&rs);}catch(COleException* exception){error=exception->m_sc;exception->Delete();}
        check(rs.m_nResultCols==count && (count?rs.m_rgODBCFieldInfos!=nullptr:rs.m_rgODBCFieldInfos==nullptr),"metadata discovery writes the native column-count member and creates an array only for nonzero result sets");
        check(error==((count && (length==-2 || length==258))?E_INVALIDARG:0),"metadata name-length validation happens before status checking and rejects negative or oversized driver lengths");
        for(auto& e:events)result.insert(result.end(),e.begin(),e.end());result.insert(result.end(),{90,error,rs.m_nResultCols,numberCalls,describeCalls});
        if(!error)for(int i=0;i<count;++i) {
            auto& info=rs.m_rgODBCFieldInfos[i];result.insert(result.end(),{info.m_strName.GetLength(),database_strings_test::data(info.m_strName)->nAllocLength,info.m_nSQLType,INT64(info.m_nPrecision),info.m_nScale,info.m_nNullability});
            // A driver may report more characters than it wrote. Compare its
            // terminated payload and the stored length without reading bytes
            // that the driver left uninitialized beyond the terminator.
            for(wchar_t ch:std::wstring(info.m_strName.GetString()))result.push_back(ch);result.push_back(-1);
        }
    }
    for(bool numberFails:{false,true})for(bool returns:{false,true})for(RETCODE code:{RETCODE(SQL_ERROR),RETCODE(SQL_INVALID_HANDLE),RETCODE(SQL_NEED_DATA)}) {
        Fixture fixture;auto& rs=fixture.rs;fixture.table[21]=reinterpret_cast<void*>(&checkMetadata);fixture.table[24]=reinterpret_cast<void*>(&throwMetadata);
        number=1;reported=3;retry=false;numberCode=numberFails?code:SQL_SUCCESS;describeCode=code;accepted=FALSE;acceptNumberSuccess=!numberFails;returningThrow=returns;numberCalls=describeCalls=0;events.clear();int error=0;
        try{api.metadata(&rs);}catch(int value){error=value;}
        check(error==(returns?0:700+code),"metadata allocation routes rejected statuses through the recordset's virtual exception hook and honors returning overrides");
        for(auto& e:events)result.insert(result.end(),e.begin(),e.end());result.insert(result.end(),{91,error,rs.m_nResultCols,rs.m_rgODBCFieldInfos!=nullptr});
    }
    {
        Fixture fixture;auto& rs=fixture.rs;fixture.table[21]=reinterpret_cast<void*>(&checkMetadata);
        number=-1;retry=false;numberCode=SQL_SUCCESS;accepted=TRUE;acceptNumberSuccess=false;numberCalls=0;events.clear();bool memory=false;
        try{api.metadata(&rs);}catch(const std::bad_alloc&){memory=true;}catch(CException* error){memory=true;error->Delete();}
        check(rs.m_nResultCols==-1 && !rs.m_rgODBCFieldInfos,"negative metadata counts preserve the driver count without installing a partially constructed array");
        result.insert(result.end(),{92,memory,rs.m_nResultCols,rs.m_rgODBCFieldInfos!=nullptr});
    }
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLDescribeColW(SQLHSTMT statement,SQLUSMALLINT field,SQLWCHAR* name,SQLSMALLINT capacity,SQLSMALLINT* length,SQLSMALLINT* type,SQLULEN* precision,SQLSMALLINT* scale,SQLSMALLINT* nullable) {
    if(recordset_allocation_test::describeHook)return recordset_allocation_test::describeHook(statement,field,name,capacity,length,type,precision,scale,nullable);
    return recordset_allocation_test::describe(statement,field,name,capacity,length,type,precision,scale,nullable);
}
static void recordsetAllocation() {
    using namespace recordset_allocation_test;
    auto oldOption=database_layout_test::optionHook;database_layout_test::optionHook=&option;
    auto oldColumns=database_execute_test::columnsHook;
    API local{&impl__AllocRowset_CRecordset__IEAAXXZ,&impl__SetRowsetSize_CRecordset__UEAAXK_Z,&impl__BindFieldsToColumns_CRecordset__QEAAIXZ,
        &impl__AllocDataCache_CRecordset__QEAAXXZ,&impl__InitRecord_CRecordset__QEAAXXZ,&impl__AllocAndCacheFieldInfo_CRecordset__IEAAXXZ};
    auto expected=exercise(local);database_execute_test::columnsHook=&columns;auto fields=metadata(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_ALLOCATION_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset allocation comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"recordset allocation oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};Patch patches[]={{0x3e9080,reinterpret_cast<void*>(&SQLSetStmtOption)},
                    {0x3e90c0,reinterpret_cast<void*>(&SQLNumResultCols)},{0x3e9070,reinterpret_cast<void*>(&SQLDescribeColW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                API native{reinterpret_cast<decltype(local.allocate)>(base+0x1faab0),reinterpret_cast<decltype(local.resize)>(base+0x1f8e00),
                    reinterpret_cast<decltype(local.bind)>(base+0x1fccd0),reinterpret_cast<decltype(local.cache)>(base+0x1fd110),
                    reinterpret_cast<decltype(local.initialize)>(base+0x1fb350),reinterpret_cast<decltype(local.metadata)>(base+0x1fa880)};
                check(exercise(native)==expected,"recordset allocation, resizing, binding, caches and interrupted state match native MFC");
                check(metadata(native)==fields,"recordset metadata discovery, asynchronous descriptions, name lengths, field precision and virtual errors match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_layout_test::optionHook=oldOption;database_execute_test::columnsHook=oldColumns;
}
