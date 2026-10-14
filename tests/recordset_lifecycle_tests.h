#pragma once
extern "C" void MS_ABI impl__Close_CRecordset__UEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__FreeRowset_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl___1CRecordset__UEAA_XZ(CRecordset*);
namespace recordset_lifecycle_test {
struct API {
    recordset_constructor_test::Constructor construct;
    decltype(&impl__Close_CRecordset__UEAAXXZ) close;
    decltype(&impl__FreeRowset_CRecordset__IEAAXXZ) rowset;
    decltype(&impl___1CRecordset__UEAA_XZ) destroy;
};
using Event=std::array<INT64,6>;
static std::vector<Event> events;
static CRecordset* active;
static int failure,deleted,ownerDestructions;
static void event(INT64 kind,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0) {events.push_back({kind,a,b,c,d,e});}
struct Failure: CDBException {~Failure() override {++deleted;}};
struct Manager: database_strings_test::Manager {
    void Free(CStringData* data) override {event(104,data->nDataLength);database_strings_test::Manager::Free(data);}
};
static void setString(CString& value,Manager& manager,const wchar_t* text,int refs=1) {
    auto* old=database_strings_test::data(value);if(old->Release())old->pStringMgr->Free(old);
    int length=int(std::wcslen(text));auto* data=manager.Allocate(length,sizeof(wchar_t));
    std::wmemcpy(data->data(),text,length+1);data->nDataLength=length;data->nRefs=refs;database_strings_test::attach(value,data);
}
static SQLRETURN SQL_API drop(SQLHSTMT statement,SQLUSMALLINT option) {
    event(101,INT64(UINT_PTR(statement)),option,active->m_nEditMode,active->m_lOpen,
        reinterpret_cast<OpenMFC_CPtrList*>(active->m_pDatabase->m_listRecordsets)->m_nCount);
    check(!active->m_rgFieldInfos && !active->m_rgODBCFieldInfos && !active->m_rgRowStatus && !active->m_pbFieldFlags &&
        !active->m_pbParamFlags && !active->m_pvFieldProxy && !active->m_pvParamProxy && !active->m_plParamLength && !active->m_dwAllocatedRowsetSize,
        "recordset Close releases field, metadata, rowset, proxy and status storage before dropping either statement");
    return SQL_ERROR;
}
static void MS_ABI bulk(CRecordset* recordset,CFieldExchange* exchange) {
    check(exchange->m_nOperation==CFieldExchange::DeleteMultiRowBuffer && exchange->m_prs==recordset && exchange->m_nFieldType==UINT(-1) && !exchange->m_pvField,
        "rowset release routes the native DeleteMultiRowBuffer exchange to the derived bulk callback");
    event(100,exchange->m_nOperation,recordset->m_dwAllocatedRowsetSize,recordset->m_rgRowStatus!=nullptr,recordset->m_nEditMode,recordset->m_strCursorName.IsEmpty());
    if(failure==1)throw static_cast<CException*>(new Failure);
    if(failure==2)throw 77;
}
static void* MS_ABI cacheDestructor(void* object,UINT flags) {event(103,flags);return object;}
static void* MS_ABI ownerDestructor(void* object,UINT flags) {
    ++ownerDestructions;event(102,flags,active->m_hstmt!=nullptr,active->m_pDatabase!=nullptr);
    if(failure==3)throw static_cast<CException*>(new Failure);
    return object;
}
static void MS_ABI unexpectedClose(CRecordset*) {event(199);}
static std::vector<INT64> snapshot(CRecordset* rs) {
    std::vector<INT64> result;
    // All scalar fields, including padding and fields Close intentionally preserves.
    auto* bytes=reinterpret_cast<BYTE*>(rs);
    for(auto range:{std::pair<size_t,size_t>{40,80},{224,264},{272,288},{312,336},{360,376},{400,416}})
        for(size_t i=range.first;i<range.second;++i)result.push_back(bytes[i]);
    for(bool live:{rs->m_hstmt!=nullptr,rs->m_hstmtUpdate!=nullptr,rs->m_rgFieldInfos!=nullptr,rs->m_rgODBCFieldInfos!=nullptr,
        rs->m_rgRowStatus!=nullptr,rs->m_pbFieldFlags!=nullptr,rs->m_pbParamFlags!=nullptr,rs->m_plParamLength!=nullptr,
        rs->m_pvFieldProxy!=nullptr,rs->m_pvParamProxy!=nullptr,rs->m_pDatabase!=nullptr})result.push_back(live);
    for(CString* text:{&rs->m_strFilter,&rs->m_strSort,&rs->m_strCursorName,&rs->m_strSQL,&rs->m_strUpdateSQL,&rs->m_strTableName,
        &rs->m_strRequerySQL,&rs->m_strRequeryFilter,&rs->m_strRequerySort}) {
        result.push_back(text->GetLength());for(wchar_t ch:std::wstring(text->GetString(),text->GetLength()))result.push_back(ch);
    }
    return result;
}
struct Fixture {
    Manager manager;
    CDatabase database;
    alignas(CRecordset) BYTE storage[448];
    CRecordset* rs;
    void* table[27];void** original;
    void* cacheTable[5]{};void* cacheObject;
    void* rawCache=nullptr;bool ownsRawCache=true;
    CStringData* sharedCursor=nullptr;
    CStringData* sharedNames[2]{};
    explicit Fixture(const API& api,DWORD options,UINT allocated=7,int cursor=1,BOOL cache=TRUE,UINT fields=2) {
        std::memset(storage,0xa5,sizeof(storage));rs=static_cast<CRecordset*>(api.construct(storage,nullptr));active=rs;
        original=*reinterpret_cast<void***>(rs);std::memcpy(table,original,sizeof(table));table[20]=reinterpret_cast<void*>(&bulk);table[6]=reinterpret_cast<void*>(&unexpectedClose);
        *reinterpret_cast<void***>(rs)=table;rs->m_pDatabase=&database;
        auto* list=reinterpret_cast<OpenMFC_CPtrList*>(database.m_listRecordsets);list->AddTail(reinterpret_cast<void*>(0x1234));list->AddTail(rs);list->AddTail(reinterpret_cast<void*>(0x5678));
        rs->m_dwOptions=options;rs->m_dwAllocatedRowsetSize=allocated;rs->m_dwRowsetSize=37;
        rs->m_hstmt=reinterpret_cast<HSTMT>(0xbeef);rs->m_hstmtUpdate=reinterpret_cast<HSTMT>(0xcafe);
        rs->m_nFields=fields;rs->m_nParams=3;rs->m_nEditMode=27;rs->m_lOpen=1;rs->m_bCheckCacheForDirtyFields=cache;
        rs->m_bBOF=9;rs->m_bEOF=11;rs->m_bUpdatable=13;rs->m_bAppendable=15;rs->m_bScrollable=17;rs->m_bDeleted=19;
        rs->m_bRebindParams=21;rs->m_bLongBinaryColumns=23;rs->m_nLockMode=25;rs->m_nFieldsBound=29;rs->m_nResultCols=31;
        rs->m_dwRowsFetched=41;rs->m_dwDriverConcurrency=43;rs->m_dwConcurrency=47;rs->m_lRecordCount=53;rs->m_lCurrentRecord=59;
        rs->m_strFilter=L"filter";rs->m_strSort=L"sort";rs->m_strSQL=L"select";rs->m_strUpdateSQL=L"update";rs->m_strTableName=L"table";
        rs->m_strRequerySQL=L"again";rs->m_strRequeryFilter=L"refilter";rs->m_strRequerySort=L"resort";
        setString(rs->m_strCursorName,manager,cursor?L"cursor":L"",cursor==2?2:cursor==3?-1:1);
        if(cursor==2)sharedCursor=database_strings_test::data(rs->m_strCursorName);
        rs->m_rgRowStatus=static_cast<UWORD*>(std::calloc(4,sizeof(UWORD)));
        rs->m_rgFieldInfos=static_cast<CFieldInfo*>(std::calloc(2,sizeof(CFieldInfo)));
        cacheTable[1]=reinterpret_cast<void*>(&cacheDestructor);cacheObject=cacheTable;rs->m_rgFieldInfos[0].m_pvDataCache=&cacheObject;rs->m_rgFieldInfos[0].m_nDataType=9;
        rawCache=std::malloc(8);rs->m_rgFieldInfos[1].m_pvDataCache=rawCache;rs->m_rgFieldInfos[1].m_nDataType=7;
        rs->m_rgODBCFieldInfos=new CODBCFieldInfo[2];
        int refs=cursor==2?2:cursor==3?-1:1;
        setString(rs->m_rgODBCFieldInfos[0].m_strName,manager,L"first",refs);setString(rs->m_rgODBCFieldInfos[1].m_strName,manager,L"second",refs);
        if(cursor==2)for(int i=0;i<2;++i)sharedNames[i]=database_strings_test::data(rs->m_rgODBCFieldInfos[i].m_strName);
        rs->m_pbFieldFlags=static_cast<BYTE*>(std::malloc(2));rs->m_pbParamFlags=static_cast<BYTE*>(std::malloc(3));rs->m_plParamLength=static_cast<SQLLEN*>(std::malloc(3*sizeof(SQLLEN)));
        rs->m_pvFieldProxy=static_cast<void**>(std::malloc(2*sizeof(void*)));rs->m_nProxyFields=2;
        rs->m_pvParamProxy=static_cast<void**>(std::malloc(2*sizeof(void*)));rs->m_nProxyParams=2;
        for(int i=0;i<2;++i){rs->m_pvFieldProxy[i]=std::malloc(8);rs->m_pvParamProxy[i]=std::malloc(8);}
        rs->m_mapFieldIndex.SetAt(reinterpret_cast<void*>(0x900),reinterpret_cast<void*>(3));rs->m_mapParamIndex.SetAt(reinterpret_cast<void*>(0x901),reinterpret_cast<void*>(4));
    }
    ~Fixture() {
        if(sharedCursor && sharedCursor->Release())sharedCursor->pStringMgr->Free(sharedCursor);
        for(auto* name:sharedNames)if(name && name->Release())name->pStringMgr->Free(name);
        if(ownsRawCache)std::free(rawCache);
        auto* cursor=database_strings_test::data(rs->m_strCursorName);if(cursor->IsLocked())cursor->nRefs=1;
        *reinterpret_cast<void***>(rs)=original;rs->m_pDatabase=nullptr;rs->m_hstmt=nullptr;rs->m_bRecordsetDb=FALSE;
        using Destroy=void* (MS_ABI*)(void*,UINT);reinterpret_cast<Destroy>(original[1])(rs,0);
        reinterpret_cast<OpenMFC_CPtrList*>(database.m_listRecordsets)->RemoveAll();
    }
};
static std::vector<INT64> exercise(const API& api) {
    std::vector<INT64> result;
    auto save=[&](Fixture& fixture) {
        auto state=snapshot(fixture.rs);result.insert(result.end(),state.begin(),state.end());
        for(auto& value:events)result.insert(result.end(),value.begin(),value.end());result.push_back(-999);events.clear();
    };
    for(DWORD options:{DWORD(0),DWORD(0x200),DWORD(0x400),DWORD(0x600),DWORD(0x1200)})for(UINT allocated:{0u,7u})for(int cursor:{0,1,2,3}) {
        Fixture fixture(api,options,allocated,cursor);auto* rs=fixture.rs;events.clear();failure=0;
        api.close(rs);fixture.ownsRawCache=false;
        auto* list=reinterpret_cast<OpenMFC_CPtrList*>(fixture.database.m_listRecordsets);
        check(!rs->m_hstmt && !rs->m_hstmtUpdate && !rs->m_lOpen && rs->m_bBOF==TRUE && rs->m_bEOF==TRUE && !rs->m_bUpdatable && !rs->m_bAppendable &&
            !rs->m_bScrollable && !rs->m_bDeleted && !rs->m_bRebindParams && !rs->m_bLongBinaryColumns && !rs->m_nLockMode && !rs->m_nEditMode && !rs->m_nFieldsBound && rs->m_nResultCols==-1,
            "recordset Close resets native cursor, edit and capability state even when dropping statements returns an error");
        check(list->m_nCount==2 && list->m_pNodeHead->data==reinterpret_cast<void*>(0x1234) && list->m_pNodeTail->data==reinterpret_cast<void*>(0x5678) && !g_databaseLockDepth,
            "recordset Close unregisters only itself and releases the ODBC registration lock");
        void* value=nullptr;check(rs->m_mapFieldIndex.Lookup(reinterpret_cast<void*>(0x900),value) && value==reinterpret_cast<void*>(3) &&
            rs->m_dwRowsFetched==41 && rs->m_dwDriverConcurrency==43 && rs->m_dwConcurrency==47 && rs->m_lRecordCount==53 && rs->m_lCurrentRecord==59 && rs->m_dwRowsetSize==37,
            "recordset Close preserves index maps, configured rowset size, counters and driver concurrency fields");
        check(rs->m_strCursorName.IsEmpty() && (cursor!=2 || fixture.sharedCursor->nRefs==1) && (cursor!=3 || database_strings_test::data(rs->m_strCursorName)->nRefs==-1),
            "recordset Close empties the cursor name through its own ATL manager while preserving shared and locked buffers");
        check(fixture.manager.frees==(cursor==1?3:cursor==2?0:2) && (cursor!=2 || (fixture.sharedNames[0]->nRefs==1 && fixture.sharedNames[1]->nRefs==1)),
            "recordset Close releases metadata strings through their original managers, frees locked strings and retains shared owners");
        check(*reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL,"recordset cleanup preserves the native allocation guard");save(fixture);
        api.close(rs);check(events.empty(),"repeated recordset Close neither drops statements nor releases cache or metadata twice");save(fixture);
    }
    for(BOOL cache:{FALSE,TRUE})for(UINT fields:{0u,2u}) {
        Fixture fixture(api,0,0,1,cache,fields);events.clear();failure=0;api.close(fixture.rs);fixture.ownsRawCache=!(cache && fields);
        check(std::count_if(events.begin(),events.end(),[](const Event& e){return e[0]==103;})==int(cache && fields),
            "recordset Close releases owned field cache only when fields exist and cache checking is enabled");save(fixture);
    }
    {
        Fixture fixture(api,0);auto* rs=fixture.rs;
        openmfc::detail::FreeRecordsetProxies(*rs);rs->m_nProxyFields=47;rs->m_nProxyParams=53;
        rs->m_hstmt=nullptr;rs->m_hstmtUpdate=nullptr;events.clear();failure=0;api.close(rs);fixture.ownsRawCache=false;
        check(rs->m_nProxyFields==47 && rs->m_nProxyParams==53 && std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==101;}),
            "recordset Close preserves proxy counts without proxy arrays and skips null statement handles");save(fixture);
    }
    for(bool closing:{false,true})for(int throws:{0,1,2}) {
        Fixture fixture(api,0x200);events.clear();failure=throws;deleted=0;bool caught=false;
        try {(closing?api.close:api.rowset)(fixture.rs);}catch(CException* exception){caught=true;exception->Delete();}catch(int value){caught=value==77;}
        check(caught==bool(throws) && deleted==int(throws==1) && !fixture.rs->m_rgRowStatus && fixture.rs->m_dwAllocatedRowsetSize==(throws?7:0),
            "rowset release clears row-status storage before callbacks and preserves allocated size when a bulk callback throws");
        if(throws)check(fixture.rs->m_nEditMode==27 && fixture.rs->m_rgFieldInfos && fixture.rs->m_hstmt && !g_databaseLockDepth,
            "bulk callback failure stops Close before edit reset, metadata release, statement drop or unregistration");
        save(fixture);failure=0;api.close(fixture.rs);fixture.ownsRawCache=false;
    }
    for(bool nullObject:{false,true}) {
        Fixture fixture(api,0);events.clear();auto* rs=fixture.rs;auto before=snapshot(rs);rs->m_pDatabase=nullptr;bool invalid=false;
        try {api.close(nullObject?nullptr:rs);}catch(int value){invalid=value==19;}catch(CException* exception){invalid=true;exception->Delete();}
        rs->m_pDatabase=&fixture.database;
        check(invalid && events.empty() && snapshot(rs)==before,"recordset Close validates its object and database before releasing any resource");
        failure=0;api.close(rs);fixture.ownsRawCache=false;
    }
    return result;
}
static std::vector<Event> destruction(const API& api) {
    std::vector<Event> result;
    for(bool statement:{false,true})for(bool owned:{false,true})for(bool nullOwner:{false,true}) {
        if(statement && nullOwner)continue;
        CDatabase database;auto** ownerTable=*reinterpret_cast<void***>(&database);void* slots[12];std::memcpy(slots,ownerTable,sizeof(slots));slots[1]=reinterpret_cast<void*>(&ownerDestructor);
        *reinterpret_cast<void***>(&database)=slots;
        alignas(CRecordset) BYTE storage[448];std::memset(storage,0xa5,sizeof(storage));auto* rs=static_cast<CRecordset*>(api.construct(storage,nullptr));active=rs;
        rs->m_pDatabase=nullOwner?nullptr:&database;rs->m_bRecordsetDb=owned;rs->m_hstmt=statement?reinterpret_cast<HSTMT>(0xbeef):nullptr;
        rs->m_hstmtUpdate=statement?reinterpret_cast<HSTMT>(0xcafe):nullptr;
        auto** original=*reinterpret_cast<void***>(rs);void* table[27];std::memcpy(table,original,sizeof(table));table[6]=reinterpret_cast<void*>(&unexpectedClose);*reinterpret_cast<void***>(rs)=table;
        events.clear();failure=0;ownerDestructions=0;api.destroy(rs);
        check(ownerDestructions==int(owned && !nullOwner) && !rs->m_pDatabase && std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==199;}),
            "recordset destruction closes through the base implementation, deletes only owned databases and clears its owner pointer");
        result.insert(result.end(),events.begin(),events.end());result.push_back({190,statement,owned,nullOwner,0,0});
        *reinterpret_cast<void***>(&database)=ownerTable;
    }
    {
        CDatabase database;auto** ownerTable=*reinterpret_cast<void***>(&database);void* slots[12];std::memcpy(slots,ownerTable,sizeof(slots));slots[1]=reinterpret_cast<void*>(&ownerDestructor);
        *reinterpret_cast<void***>(&database)=slots;
        alignas(CRecordset) BYTE storage[448];std::memset(storage,0xa5,sizeof(storage));auto* rs=static_cast<CRecordset*>(api.construct(storage,nullptr));active=rs;
        rs->m_pDatabase=&database;rs->m_bRecordsetDb=TRUE;
        events.clear();failure=3;deleted=ownerDestructions=0;api.destroy(rs);
        check(deleted==1 && ownerDestructions==1 && rs->m_pDatabase==&database,
            "recordset destructor deletes framework exceptions from an owned database destructor and preserves the interrupted owner field");
        result.insert(result.end(),events.begin(),events.end());result.push_back({191,deleted,ownerDestructions,0,0,0});
        *reinterpret_cast<void***>(&database)=ownerTable;failure=0;
    }
    for(int refs:{1,2,-1,-3}) {
        Manager manager;
        alignas(CRecordset) BYTE storage[448];std::memset(storage,0xa5,sizeof(storage));auto* rs=static_cast<CRecordset*>(api.construct(storage,nullptr));active=rs;
        CString* strings[]={&rs->m_strFilter,&rs->m_strSort,&rs->m_strCursorName,&rs->m_strSQL,&rs->m_strUpdateSQL,&rs->m_strTableName,
            &rs->m_strRequerySQL,&rs->m_strRequeryFilter,&rs->m_strRequerySort};
        CStringData* buffers[9];
        for(int i=0;i<9;++i) {setString(*strings[i],manager,std::wstring(size_t(i+1),L'x').c_str(),refs);buffers[i]=database_strings_test::data(*strings[i]);}
        events.clear();api.destroy(rs);
        check(manager.frees==(refs==2?0:9),"recordset destruction releases every string member through its manager, including locked buffers");
        for(auto& value:events)result.push_back(value);result.push_back({192,refs,manager.frees,0,0,0});
        // Release shared fixture ownership; reclaim locked buffers only if the
        // implementation failed the check, keeping the failing probe leak-free.
        if(refs==2) {for(auto* data:buffers)if(data->Release())data->pStringMgr->Free(data);}
        else if(refs<0 && manager.frees==0) {for(auto* data:buffers)manager.Free(data);}
    }
    return result;
}
}
static void recordsetLifecycle() {
    using namespace recordset_lifecycle_test;
    auto previous=database_execute_test::freeHook;database_execute_test::freeHook=&drop;
    API local{&impl___0CRecordset__QEAA_PEAVCDatabase___Z,&impl__Close_CRecordset__UEAAXXZ,&impl__FreeRowset_CRecordset__IEAAXXZ,&impl___1CRecordset__UEAA_XZ};
    auto expected=exercise(local);auto destructors=destruction(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_LIFECYCLE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset lifecycle comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"recordset lifecycle oracle uses the verified MFC image build");
            if(pinned) {
                auto** slot=reinterpret_cast<void**>(base+0x3e90a0);void* saved=*slot;DWORD protection;::VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=reinterpret_cast<void*>(&SQLFreeStmt);
                API native{reinterpret_cast<recordset_constructor_test::Constructor>(base+0x1f7e40),reinterpret_cast<decltype(local.close)>(base+0x1f84d0),
                    reinterpret_cast<decltype(local.rowset)>(base+0x1fab00),reinterpret_cast<decltype(local.destroy)>(base+0x1f8170)};
                check(exercise(native)==expected,"recordset Close and FreeRowset resource ownership, callbacks, failures and scalar state match native MFC");
                check(destruction(native)==destructors,"recordset destructor statement cleanup and database ownership match native MFC");
                *slot=saved;DWORD ignored;::VirtualProtect(slot,8,protection,&ignored);
            }
            ::FreeLibrary(module);
        }
    }
    database_execute_test::freeHook=previous;
}
