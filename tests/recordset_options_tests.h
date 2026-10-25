#pragma once
extern "C" void MS_ABI impl__EnableBookmarks_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__VerifyDriverBehavior_CRecordset__IEAAXXZ(CRecordset*);
extern "C" DWORD MS_ABI impl__VerifyCursorSupport_CRecordset__IEAAKXZ(CRecordset*);
extern "C" void MS_ABI impl__SetUpdateMethod_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__SetConcurrencyAndCursorType_CRecordset__IEAAXPEAXK_Z(CRecordset*,void*,DWORD);
extern "C" void MS_ABI impl__OnSetOptions_CRecordset__UEAAXPEAX_Z(CRecordset*,void*);
extern "C" BOOL MS_ABI impl__CanBookmark_CRecordset__QEBAHXZ(const CRecordset*);
namespace recordset_options_test {
struct API {
    decltype(&impl__EnableBookmarks_CRecordset__IEAAXXZ) bookmarks,driver;
    decltype(&impl__VerifyCursorSupport_CRecordset__IEAAKXZ) cursor;
    decltype(&impl__SetUpdateMethod_CRecordset__IEAAXXZ) update;
    decltype(&impl__SetConcurrencyAndCursorType_CRecordset__IEAAXPEAXK_Z) concurrency;
    decltype(&impl__OnSetOptions_CRecordset__UEAAXPEAX_Z) options;
    decltype(&impl__CanBookmark_CRecordset__QEBAHXZ) canBookmark;
};
using Event=std::array<INT64,8>;
static std::vector<Event> events;
static void event(INT64 kind,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0){events.push_back({kind,a,b,c,d,e,f,g});}
struct Script {
    UINT type=CRecordset::snapshot;DWORD options=0x100,bookmarks=SQL_BP_SCROLL,updates=3;
    BOOL updatable=69,appendable=71,databaseUpdatable=73;int lock=0;
    SQLUSMALLINT extended=1;SQLUINTEGER scroll=0xffffffff,concurrency=0xffffffff;
    const wchar_t* version=L"03.80";
    bool missing=false,returns=false,nullStatement=false,disconnected=false;
    int reject=0,failure=0;RETCODE code=SQL_ERROR;
    DWORD cursor=0xfedcba98;
};
static Script script;
static CRecordset* active;
static int calls,checks;
static RETCODE called() {++calls;if(script.failure==calls)throw 400+calls;return script.reject==calls?script.code:SQL_SUCCESS_WITH_INFO;}
static SQLRETURN SQL_API functions(SQLHDBC connection,SQLUSMALLINT function,SQLUSMALLINT* output) {
    event(1,INT64(UINT_PTR(connection)),function);*output=script.extended;return called();
}
static SQLRETURN SQL_API info(SQLHDBC connection,SQLUSMALLINT kind,SQLPOINTER output,SQLSMALLINT capacity,SQLSMALLINT* bytes) {
    event(2,INT64(UINT_PTR(connection)),kind,capacity,bytes!=nullptr,output==&active->m_dwDriverConcurrency);
    if(kind==SQL_ODBC_VER) {
        check(capacity==60,"driver verification gives ODBC a 60-byte UTF-16 version buffer");
        wcscpy(static_cast<wchar_t*>(output),script.version);*bytes=SQLSMALLINT(wcslen(script.version)*2);
    } else {
        check(capacity==4 && (kind==SQL_SCROLL_OPTIONS || kind==SQL_SCROLL_CONCURRENCY),"cursor and concurrency negotiation use four-byte driver masks");
        *static_cast<SQLUINTEGER*>(output)=kind==SQL_SCROLL_OPTIONS?script.scroll:script.concurrency;*bytes=4;
    }
    return called();
}
static SQLRETURN SQL_API option(SQLHSTMT statement,SQLUSMALLINT kind,SQLULEN value) {
    event(3,INT64(UINT_PTR(statement)),kind,INT64(value),active->m_dwConcurrency);return called();
}
static BOOL MS_ABI checkStatus(const CRecordset*,RETCODE code) {++checks;event(4,code);return script.reject==calls?FALSE:69;}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT statement) {
    event(5,code,INT64(UINT_PTR(statement)));if(!script.returns)throw 700+code;
}
static void MS_ABI databaseOptions(CDatabase*,HSTMT statement) {event(6,INT64(UINT_PTR(statement)));if(script.failure==-1)throw 399;}
static std::vector<Event> exercise(const API& api) {
    std::vector<Event> result;
    auto run=[&](int method,const Script& next) {
        script=next;calls=checks=0;events.clear();CDatabase db;CRecordset rs;active=&rs;
        db.m_hdbc=script.disconnected?nullptr:reinterpret_cast<HDBC>(0xface);db.m_bUpdatable=script.databaseUpdatable;
        db.m_dwUpdateOptions=script.updates;db.m_dwBookmarkAttributes=script.bookmarks;
        auto** originalDB=*reinterpret_cast<void***>(&db);void* dbTable[12];memcpy(dbTable,originalDB,sizeof(dbTable));dbTable[8]=reinterpret_cast<void*>(&databaseOptions);*reinterpret_cast<void***>(&db)=dbTable;
        auto** originalRS=*reinterpret_cast<void***>(&rs);void* rsTable[27];memcpy(rsTable,originalRS,sizeof(rsTable));rsTable[21]=reinterpret_cast<void*>(&checkStatus);rsTable[24]=reinterpret_cast<void*>(&throwStatus);*reinterpret_cast<void***>(&rs)=rsTable;
        rs.m_pDatabase=script.missing?nullptr:&db;rs.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);
        rs.m_nOpenType=script.type;rs.m_dwOptions=script.options;rs.m_bUpdatable=script.updatable;rs.m_bAppendable=script.appendable;
        rs.m_bUseUpdateSQL=81;rs.m_bScrollable=83;rs.m_dwConcurrency=85;rs.m_dwDriverConcurrency=87;rs.m_nLockMode=script.lock;
        DWORD returned=0xcccccccc;int caught=0;HSTMT supplied=script.nullStatement?nullptr:reinterpret_cast<HSTMT>(0xcafe);
        try {
            switch(method) {
            case 0:api.bookmarks(&rs);break;case 1:api.driver(&rs);break;case 2:returned=api.cursor(&rs);break;
            case 3:api.update(&rs);break;case 4:api.concurrency(&rs,supplied,script.cursor);break;case 5:api.options(&rs,supplied);break;
            case 6:returned=api.canBookmark(&rs);break;
            }
        }catch(int error){caught=error==19?-1:error;}catch(CException* error){caught=-1;error->Delete();}
        if(!script.reject && !script.failure && !script.missing && method==3) {
            check(rs.m_bUseUpdateSQL==((script.updates&1)?FALSE:(script.updates&2)?TRUE:81) &&
                rs.m_bUpdatable==((script.updates&3)?script.updatable:FALSE) && rs.m_bAppendable==script.appendable,
                "update-method negotiation prioritizes positioned updates, then SQL updates, preserving appendability");
        }
        if(method==0 && !(script.options&0x100))check(events.empty() && !caught,"bookmark setup bypasses database validation when bookmarks were not requested");
        if(method==5 && !script.missing && !script.failure && script.type==2 && !(script.options&0x800))
            check(events.size()==1 && events[0][0]==6,"forward-only statements without extended fetching delegate only the database option callback");
        if(method==6) {
            DWORD expected=(script.options&0x100) && (script.type!=2 || (script.options&0x800)) ? script.bookmarks&0x40 : 0;
            check(events.empty() && ((script.missing || script.disconnected)?caught==-1:!caught && returned==expected),
                "bookmark capability requires an open database, honors extended-fetch flags and returns the native unnormalized capability bit");
        }
        check(rs.m_hstmt==reinterpret_cast<HSTMT>(0xbeef),"option negotiation preserves the instance statement while configuring the supplied statement");
        result.insert(result.end(),events.begin(),events.end());
        result.push_back({90,method,returned,caught,rs.m_bScrollable,rs.m_bUpdatable,rs.m_bAppendable,rs.m_bUseUpdateSQL});
        result.push_back({91,rs.m_dwDriverConcurrency,rs.m_dwConcurrency,calls,checks,rs.m_nLockMode,rs.m_dwOptions,rs.m_nOpenType});
        rs.m_hstmt=nullptr;rs.m_pDatabase=nullptr;db.m_hdbc=nullptr;*reinterpret_cast<void***>(&rs)=originalRS;*reinterpret_cast<void***>(&db)=originalDB;
    };
    for(DWORD options:{0ul,0x100ul,0x800ul,0x900ul})for(DWORD attributes:{0ul,0x40ul,0xfffffffful}) {Script s;s.options=options;s.bookmarks=attributes;run(0,s);}
    for(SQLUSMALLINT supported:{SQLUSMALLINT(0),SQLUSMALLINT(1),SQLUSMALLINT(69),SQLUSMALLINT(65535)})for(const wchar_t* version:{L"01.00",L"02.00",L"03.80",L"00",L"0/",L"10.00"}) {
        Script s;s.extended=supported;s.version=version;run(1,s);
    }
    for(UINT type:{0u,1u,2u,3u,17u,UINT(-1)})for(SQLUINTEGER supported:{SQLUINTEGER(0),SQLUINTEGER(1),SQLUINTEGER(2),SQLUINTEGER(4),SQLUINTEGER(16),SQLUINTEGER(0xffffffff)})for(bool returns:{false,true}) {
        Script s;s.type=type;s.scroll=supported;s.returns=returns;run(2,s);
    }
    for(DWORD updates:{0ul,1ul,2ul,3ul,4ul,0xfffffffful})for(BOOL writable:{FALSE,BOOL(69)}) {Script s;s.updates=updates;s.updatable=writable;run(3,s);}
    for(BOOL writable:{FALSE,BOOL(69)})for(BOOL appendable:{FALSE,BOOL(71)})for(BOOL dbWritable:{FALSE,BOOL(73)})for(int lock:{0,1,2,-1})for(SQLUINTEGER mask:{SQLUINTEGER(0),SQLUINTEGER(1),SQLUINTEGER(2),SQLUINTEGER(4),SQLUINTEGER(8),SQLUINTEGER(12),SQLUINTEGER(14),SQLUINTEGER(0xffffffff)}) {
        Script s;s.updatable=writable;s.appendable=appendable;s.databaseUpdatable=dbWritable;s.lock=lock;s.concurrency=mask;run(4,s);
    }
    for(UINT type:{0u,1u,2u,3u})for(DWORD options:{0ul,0x100ul,0x800ul,0x900ul})for(DWORD updates:{0ul,1ul,2ul})for(SQLUSMALLINT supported:{SQLUSMALLINT(0),SQLUSMALLINT(1)}) {
        Script s;s.type=type;s.options=options;s.updates=updates;s.extended=supported;run(5,s);
    }
    for(int method=0;method<6;++method)for(bool nullStatement:{false,true}) {
        Script s;s.nullStatement=nullStatement;s.missing=true;run(method,s);s.options=0;run(method,s);
    }
    for(UINT type:{0u,1u,2u,3u})for(DWORD options:{0ul,0x100ul,0x800ul,0x900ul})for(DWORD attributes:{0ul,0x40ul,0xfffffffful})for(bool missing:{false,true})for(bool disconnected:{false,true}) {
        Script s;s.type=type;s.options=options;s.bookmarks=attributes;s.missing=missing;s.disconnected=disconnected;run(6,s);
    }
    for(int method:{0,1,2,4,5})for(int point=1;point<=7;++point)for(bool returns:{false,true}) {
        Script s;s.reject=point;s.returns=returns;run(method,s);
    }
    for(int method:{0,1,2,4,5})for(int point=-1;point<=7;++point) {Script s;s.failure=point;run(method,s);}
    for(RETCODE code:{RETCODE(SQL_STILL_EXECUTING),RETCODE(SQL_NO_DATA),RETCODE(SQL_INVALID_HANDLE),RETCODE(SQL_NEED_DATA)}) {
        Script s;s.reject=1;s.code=code;s.returns=true;run(5,s);
    }
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLGetFunctions(SQLHDBC db,SQLUSMALLINT function,SQLUSMALLINT* output) {
    return recordset_options_test::functions(db,function,output);
}
static void recordsetOptions() {
    using namespace recordset_options_test;
    auto oldInfo=database_strings_test::infoHook;auto oldOption=database_layout_test::optionHook;
    database_strings_test::infoHook=&info;database_layout_test::optionHook=&option;
    API local{&impl__EnableBookmarks_CRecordset__IEAAXXZ,&impl__VerifyDriverBehavior_CRecordset__IEAAXXZ,&impl__VerifyCursorSupport_CRecordset__IEAAKXZ,
        &impl__SetUpdateMethod_CRecordset__IEAAXXZ,&impl__SetConcurrencyAndCursorType_CRecordset__IEAAXPEAXK_Z,&impl__OnSetOptions_CRecordset__UEAAXPEAX_Z,
        &impl__CanBookmark_CRecordset__QEBAHXZ};
    auto expected=exercise(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_OPTIONS_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset driver-option comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"recordset option oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e9080,reinterpret_cast<void*>(&SQLSetStmtOption)},{0x3e9078,reinterpret_cast<void*>(&SQLGetInfoW)},{0x3e90d8,reinterpret_cast<void*>(&SQLGetFunctions)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                API native{reinterpret_cast<decltype(local.bookmarks)>(base+0x1fab80),reinterpret_cast<decltype(local.driver)>(base+0x1fa630),
                    reinterpret_cast<decltype(local.cursor)>(base+0x1fa770),reinterpret_cast<decltype(local.update)>(base+0x1fac10),
                    reinterpret_cast<decltype(local.concurrency)>(base+0x1fac60),reinterpret_cast<decltype(local.options)>(base+0x1f9d90),
                    reinterpret_cast<decltype(local.canBookmark)>(base+0x1f89e0)};
                auto actual=exercise(native);
                if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
                    std::printf("options mismatch at %zu:",i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);std::printf("\n");break;
                }
                check(actual==expected,"bookmark enabling, driver versions, cursor support, update methods, concurrency preference and interrupted option setup match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_strings_test::infoHook=oldInfo;database_layout_test::optionHook=oldOption;
}
