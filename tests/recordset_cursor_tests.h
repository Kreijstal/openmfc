#pragma once
extern "C" void MS_ABI impl__ResetCursor_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__RefreshRowset_CRecordset__QEAAXGG_Z(CRecordset*,WORD,WORD);
extern "C" void MS_ABI impl__SetRowsetCursorPosition_CRecordset__QEAAXGG_Z(CRecordset*,WORD,WORD);
extern "C" void MS_ABI impl__SetLockingMode_CRecordset__QEAAXI_Z(CRecordset*,UINT);
extern "C" void MS_ABI impl__StoreFields_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__LoadFields_CRecordset__QEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__CancelUpdate_CRecordset__QEAAXXZ(CRecordset*);
namespace recordset_cursor_test {
using namespace recordset_move_test;
struct API {
    decltype(&impl__ResetCursor_CRecordset__QEAAXXZ) reset;
    decltype(&impl__RefreshRowset_CRecordset__QEAAXGG_Z) refresh;
    decltype(&impl__SetRowsetCursorPosition_CRecordset__QEAAXGG_Z) position;
    decltype(&impl__SetLockingMode_CRecordset__QEAAXI_Z) lock;
    decltype(&impl__StoreFields_CRecordset__QEAAXXZ) store;
    decltype(&impl__LoadFields_CRecordset__QEAAXXZ) load;
    decltype(&impl__CancelUpdate_CRecordset__QEAAXXZ) cancel;
};
static bool changeHandle;
static SQLUINTEGER supported;
static RETCODE infoCode;
static SQLRETURN SQL_API setPosition(SQLHSTMT statement,SQLSETPOSIROW row,SQLUSMALLINT operation,SQLUSMALLINT lock) {
    event(20,INT64(UINT_PTR(statement)),INT64(row),operation,lock);
    auto step=take();fill(step);
    if(changeHandle)active->m_hstmt=reinterpret_cast<HSTMT>(UINT_PTR(active->m_hstmt)+1);
    return step.code;
}
static SQLRETURN SQL_API lockInfo(SQLHDBC connection,SQLUSMALLINT kind,SQLPOINTER output,SQLSMALLINT capacity,SQLSMALLINT* length) {
    event(21,INT64(UINT_PTR(connection)),kind,capacity,length!=nullptr);fail(1);
    check(kind==SQL_LOCK_TYPES && output && capacity==4 && length,"pessimistic locking queries the driver's exact SQL_LOCK_TYPES buffer");
    *static_cast<SQLUINTEGER*>(output)=supported;*length=4;return infoCode;
}
static void MS_ABI cachedExchange(CRecordset* rs,CFieldExchange* fx) {
    event(22,fx->m_nOperation,rs->m_pbFieldFlags[0],rs->m_pbFieldFlags[1],rs->m_pbFieldFlags[2],rs->m_nEditMode,INT64(UINT_PTR(fx->m_hstmt)));
    ordinary(rs,fx);
}
static void saveState(std::vector<Event>& result,Fixture& fixture,int caught=0) {
    event(23,fixture.rs.m_nLockMode,fixture.rs.m_lOpen,INT64(UINT_PTR(fixture.rs.m_hstmt)),fixture.rs.m_bCheckCacheForDirtyFields);
    save(result,fixture,caught);
}
template<class Operation> static int invoke(Operation operation) {
    try{operation();return 0;}catch(int value){return value==19?-1:value;}catch(CException* value){return normalize(value);}
}
static std::vector<Event> cursors(const API& functions) {
    std::vector<Event> result;
    for(long open:{-1L,0L,1L,37L})for(int edit:{0,1,2,83}) {
        Fixture f;f.rs.m_lOpen=open;f.rs.m_nEditMode=edit;f.rs.m_bBOF=55;f.rs.m_bEOF=57;f.rs.m_bEOFSeen=69;
        functions.reset(&f.rs);
        check(events.empty() && f.rs.m_lCurrentRecord==-1 && !f.rs.m_bBOF && !f.rs.m_bEOF && !f.rs.m_bEOFSeen &&
            !f.rs.m_bDeleted && !f.rs.m_lRecordCount && f.rs.m_lOpen==open && f.rs.m_nEditMode==UINT(edit),
            "ResetCursor resets currency bookkeeping without closing the statement, changing edit state or calling ODBC");
        saveState(result,f);
    }
    for(bool refresh:{false,true})for(WORD row:{WORD(0),WORD(1),WORD(65535)})for(WORD lock:{WORD(0),WORD(1),WORD(2),WORD(65535)})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR)})for(bool changed:{false,true}) {
        Fixture f;changeHandle=changed;steps={{SQL_STILL_EXECUTING},{SQL_STILL_EXECUTING},{code}};
        (refresh?functions.refresh:functions.position)(&f.rs,row,lock);saveState(result,f);
    }
    for(UINT fields:{0u,3u})for(BOOL eof:{FALSE,BOOL(69)})for(BOOL bof:{FALSE,BOOL(71)})for(DWORD options:{0ul,0x200ul})for(bool missing:{false,true}) {
        Fixture f;changeHandle=false;f.rs.m_nFields=fields;f.rs.m_bEOF=eof;f.rs.m_bBOF=bof;f.rs.m_dwOptions=options;
        if(missing)f.rs.m_hstmt=nullptr;steps={{SQL_ERROR}};
        int caught=invoke([&]{functions.refresh(&f.rs,17,2);});saveState(result,f,caught);
    }
    for(bool refresh:{false,true})for(int failAt:{1,11,105,205}) {
        Fixture f;changeHandle=false;failure=failAt;
        int caught=invoke([&]{(refresh?functions.refresh:functions.position)(&f.rs,1,2);});saveState(result,f,caught);
    }
    return result;
}
static std::vector<Event> locks(const API& functions) {
    std::vector<Event> result;
    for(UINT mode:{0u,1u,2u,0xffffffffu})for(SQLUINTEGER capabilities:{0ul,1ul,2ul,3ul})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR)})for(bool accepted:{false,true})for(bool returns:{false,true}) {
        Fixture f;supported=capabilities;infoCode=code;recordset_move_test::accept=accepted;returningThrow=returns;f.rs.m_nLockMode=73;
        int caught=invoke([&]{functions.lock(&f.rs,mode);});saveState(result,f,caught);
    }
    for(bool noDatabase:{false,true})for(UINT mode:{0u,1u,2u})for(int failAt:{0,1,4,5,11,14,15}) {
        Fixture f;if(noDatabase)f.rs.m_pDatabase=nullptr;failure=failAt;supported=0;infoCode=SQL_ERROR;recordset_move_test::accept=false;f.rs.m_nLockMode=73;
        int caught=invoke([&]{functions.lock(&f.rs,mode);});saveState(result,f,caught);
    }
    return result;
}
static std::vector<Event> caching(const API& functions) {
    std::vector<Event> result;
    for(int operation:{0,1,2})for(UINT fields:{0u,3u})for(UINT edit:{0u,1u,2u,83u})for(BOOL cache:{FALSE,BOOL(69)})for(long open:{0L,1L})for(bool missing:{false,true}) {
        Fixture f;f.table[19]=reinterpret_cast<void*>(&cachedExchange);f.rs.m_nFields=fields;f.rs.m_nEditMode=edit;f.rs.m_lOpen=open;
        f.rs.m_bCheckCacheForDirtyFields=cache;f.rs.m_dwOptions=CRecordset::readOnly|CRecordset::useMultiRowFetch;
        if(missing)f.rs.m_hstmt=nullptr;
        auto action=operation==0?functions.store:operation==1?functions.load:functions.cancel;
        int caught=invoke([&]{action(&f.rs);});saveState(result,f,caught);
    }
    for(int operation:{0,1,2})for(int failAt:{112,113,212,213}) {
        Fixture f;f.table[19]=reinterpret_cast<void*>(&cachedExchange);failure=failAt;
        auto action=operation==0?functions.store:operation==1?functions.load:functions.cancel;
        int caught=invoke([&]{action(&f.rs);});saveState(result,f,caught);
    }
    // Exercise the real RFX cache path, not just the virtual callback protocol.
    for(bool nullValue:{false,true}) {
        Fixture f;CFieldInfo info{};info.m_nDataType=3;info.m_nLength=sizeof(int);f.rs.m_rgFieldInfos=&info;f.rs.m_nFields=1;
        f.table[19]=reinterpret_cast<void*>(&realExchange);fieldValue=42;f.flags[0]=nullValue?2:0;
        functions.store(&f.rs);fieldValue=91;info.m_nLength=sizeof(int);f.flags[0]=0x81;f.rs.m_nEditMode=CRecordset::edit;
        functions.cancel(&f.rs);
        check(f.rs.m_nEditMode==CRecordset::noMode && f.flags[0]==(nullValue?2:0) && fieldValue==(nullValue?91:42) &&
            info.m_nLength==(nullValue?SQL_NULL_DATA:SQLLEN(sizeof(int))),
            "CancelUpdate restores a real RFX cached value or NULL status and discards dirty flags");
        event(24,fieldValue,info.m_nLength,info.m_bStatus);saveState(result,f);f.rs.m_rgFieldInfos=nullptr;
    }
    // Exercise driver-updated bound values and their real RFX NULL fixups.
    for(bool nullValue:{false,true}) {
        Fixture f;CFieldInfo info{};info.m_nDataType=3;info.m_nLength=sizeof(int);f.rs.m_rgFieldInfos=&info;f.rs.m_nFields=1;
        f.table[19]=reinterpret_cast<void*>(&realExchange);fieldValue=17;f.flags[0]=0;populate=true;changeHandle=false;
        binary_rfx_test::binding.data=&fieldValue;binary_rfx_test::binding.length=&info.m_nLength;
        binary_rfx_test::binding.cType=SQL_C_LONG;binary_rfx_test::binding.capacity=4;
        steps={{SQL_SUCCESS,SQL_ROW_SUCCESS,1,nullValue?SQL_NULL_DATA:SQLLEN(sizeof(int)),42}};
        functions.refresh(&f.rs,1,0);
        check(fieldValue==(nullValue?0x7ee4:42) && f.rs.IsFieldStatusNull(0)==(nullValue?2:0),
            "RefreshRowset applies real RFX fixups to driver-populated values and NULL indicators");
        event(24,fieldValue,info.m_nLength);saveState(result,f);f.rs.m_rgFieldInfos=nullptr;
    }
    return result;
}
}
extern "C" SQLRETURN SQL_API SQLSetPos(SQLHSTMT statement,SQLSETPOSIROW row,SQLUSMALLINT operation,SQLUSMALLINT lock) {
    return recordset_cursor_test::setPosition(statement,row,operation,lock);
}
static void recordsetCursor() {
    using namespace recordset_cursor_test;
    auto oldInfo=database_strings_test::infoHook;database_strings_test::infoHook=&lockInfo;
    recordset_cursor_test::API local{&impl__ResetCursor_CRecordset__QEAAXXZ,&impl__RefreshRowset_CRecordset__QEAAXGG_Z,&impl__SetRowsetCursorPosition_CRecordset__QEAAXGG_Z,
        &impl__SetLockingMode_CRecordset__QEAAXI_Z,&impl__StoreFields_CRecordset__QEAAXXZ,&impl__LoadFields_CRecordset__QEAAXXZ,&impl__CancelUpdate_CRecordset__QEAAXXZ};
    auto states=cursors(local);auto locking=locks(local);auto cache=caching(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_CURSOR_DLL",path,1024)) {
        auto module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for cursor and cached-field comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"cursor oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};Patch patches[]={{0x3e9108,reinterpret_cast<void*>(&SQLSetPos)},{0x3e9078,reinterpret_cast<void*>(&SQLGetInfoW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                recordset_cursor_test::API native{reinterpret_cast<decltype(local.reset)>(base+0x1fb3a0),reinterpret_cast<decltype(local.refresh)>(base+0x1fb930),
                    reinterpret_cast<decltype(local.position)>(base+0x1fb9a0),reinterpret_cast<decltype(local.lock)>(base+0x1f9ac0),
                    reinterpret_cast<decltype(local.store)>(base+0x1fcfc0),reinterpret_cast<decltype(local.load)>(base+0x1fd010),reinterpret_cast<decltype(local.cancel)>(base+0x1f9180)};
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(cursors(native),states,"cursor reset, positioning, asynchronous refresh, fixup gates and interrupted state match native MFC");
                compare(locks(native),locking,"locking capabilities, virtual status handling and assignment after returning throws match native MFC");
                compare(caching(native),cache,"field snapshot, restore and CancelUpdate behavior including RFX values, NULLs and failures match native MFC");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_strings_test::infoHook=oldInfo;
}
