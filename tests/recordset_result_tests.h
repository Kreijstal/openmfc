#pragma once
extern "C" SQLLEN MS_ABI impl__GetData_CRecordset__SA_JPEAVCDatabase__PEAXFF1_JF_Z(CDatabase*,HSTMT,short,short,void*,SQLLEN,short);
extern "C" BOOL MS_ABI impl__FlushResultSet_CRecordset__QEAAHXZ(CRecordset*);
extern "C" void MS_ABI impl__GetBookmark_CRecordset__QEAAXAEAVCDBVariant___Z(CRecordset*,CDBVariant*);
extern "C" void MS_ABI impl__SetBookmark_CRecordset__QEAAXAEBVCDBVariant___Z(CRecordset*,const CDBVariant*);
extern "C" short MS_ABI impl__GetDefaultFieldType_CRecordset__SAFF_Z(short);
extern "C" SQLLEN MS_ABI impl__GetTextLen_CRecordset__SA_JF_K_Z(short,SQLULEN);
namespace recordset_result_test {
using namespace recordset_move_test;
struct ResultsAPI {
    decltype(&impl__GetData_CRecordset__SA_JPEAVCDatabase__PEAXFF1_JF_Z) data;
    decltype(&impl__FlushResultSet_CRecordset__QEAAHXZ) more;
    decltype(&impl__GetBookmark_CRecordset__QEAAXAEAVCDBVariant___Z) get;
    decltype(&impl__SetBookmark_CRecordset__QEAAXAEBVCDBVariant___Z) set;
    decltype(&impl__GetDefaultFieldType_CRecordset__SAFF_Z) fieldType;
    decltype(&impl__GetTextLen_CRecordset__SA_JF_K_Z) textLength;
};
static std::vector<UINT64> formats(const ResultsAPI& functions) {
    std::vector<UINT64> result;result.reserve(65536*8);
    for(unsigned bits=0;bits<65536;++bits) {
        short type=short(bits);result.push_back(UINT64(functions.fieldType(type)));
        for(SQLULEN precision:{SQLULEN(0),SQLULEN(1),SQLULEN(42),SQLULEN(1ULL<<32),SQLULEN((1ULL<<63)-1),SQLULEN(1ULL<<63),SQLULEN(-1)})
            result.push_back(UINT64(functions.textLength(type,precision)));
    }
    check(functions.fieldType(SQL_BIGINT)==SQL_C_WCHAR && functions.fieldType(SQL_VARCHAR)==SQL_C_CHAR &&
        functions.textLength(SQL_INTEGER,1ULL<<40)==SQLLEN((1ULL<<40)+2),
        "field formatting preserves native SQL type choices and 64-bit character-buffer precision");
    return result;
}
struct Step {RETCODE code;SQLLEN length=4;DWORD word=0x81234567;};
static std::vector<Step> dataSteps;
static size_t dataPosition;
static bool changeHandle;
static Step next() {
    if(dataPosition>=dataSteps.size()){puts("result script exhausted");std::abort();}
    auto step=dataSteps[dataPosition++];fail(1);if(changeHandle)active->m_hstmt=reinterpret_cast<HSTMT>(UINT_PTR(active->m_hstmt)+1);return step;
}
static SQLRETURN SQL_API data(SQLHSTMT statement,SQLUSMALLINT field,SQLSMALLINT type,SQLPOINTER output,SQLLEN capacity,SQLLEN* indicator) {
    event(70,INT64(UINT_PTR(statement)),field,type,capacity,output!=nullptr);auto step=next();*indicator=step.length;
    if(output && capacity>=4)memcpy(output,&step.word,4);return step.code;
}
static SQLRETURN SQL_API more(SQLHSTMT statement){event(71,INT64(UINT_PTR(statement)));return next().code;}
static void MS_ABI move(CRecordset*,long rows,WORD direction){event(72,rows,direction);fail(3);}
template<class F> static int invoke(F action) {
    try{action();return 0;}catch(CDBException* error){int code=1000+error->m_nRetCode;error->Delete();return code;}
    catch(CException* error){return normalize(error);}catch(int code){return code==19?-1:code;}
}
static void reset() {dataPosition=0;changeHandle=false;longbinary_rfx_test::reset({},{{L"HY000",5,L"[Driver] retrieval failed"}});}
static void saveState(std::vector<Event>& result,Fixture& f,int caught) {
    event(73,INT64(dataPosition),INT64(longbinary_rfx_test::diagnosticReads.size()),INT64(UINT_PTR(f.rs.m_hstmt)));save(result,f,caught);
}
static std::vector<Event> retrieval(const ResultsAPI& functions) {
    std::vector<Event> result;
    for(short field:{short(-1),short(0),short(2)})for(short type:{short(SQL_C_BOOKMARK),short(SQL_C_BINARY),short(SQL_C_CHAR),short(SQL_C_LONG)})
    for(SQLLEN capacity:{SQLLEN(-1),SQLLEN(0),SQLLEN(4),SQLLEN(1ULL<<40)})for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR)})
    for(SQLLEN length:{SQLLEN(SQL_NULL_DATA),SQLLEN(SQL_NO_TOTAL),SQLLEN(1ULL<<40)}) {
        Fixture f;reset();DWORD output=0xa5a5a5a5;SQLLEN returned=77;
        dataSteps={{SQL_STILL_EXECUTING},{SQL_STILL_EXECUTING},{code,length}};
        int caught=invoke([&]{returned=functions.data(&f.db,f.rs.m_hstmt,field,type,&output,capacity,-4);});
        event(74,returned,output);saveState(result,f,caught);
    }
    for(bool missing:{false,true})for(int failAt:{0,1,11}) {
        Fixture f;reset();failure=failAt;dataSteps={{SQL_SUCCESS,SQL_NULL_DATA}};SQLLEN returned=77;
        int caught=invoke([&]{returned=functions.data(missing?nullptr:&f.db,nullptr,0,4,nullptr,0,0);});event(74,returned);saveState(result,f,caught);
    }
    longbinary_rfx_test::reset({});return result;
}
static std::vector<Event> advancing(const ResultsAPI& functions) {
    std::vector<Event> result;
    for(int bound:{-1,0,3})for(UINT fields:{0u,3u})for(DWORD flags:{0ul,0x200ul})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR)})for(bool accepted:{false,true})for(bool missing:{false,true}) {
        Fixture f;reset();f.rs.m_nFieldsBound=bound;f.rs.m_nFields=fields;f.rs.m_dwOptions=flags;if(missing)f.rs.m_hstmt=nullptr;
        recordset_move_test::accept=accepted;dataSteps={{SQL_STILL_EXECUTING},{code}};BOOL returned=-7;
        int caught=invoke([&]{returned=functions.more(&f.rs);});event(74,returned);saveState(result,f,caught);
    }
    for(int failAt:{1,4,11,14,105,205}) {
        Fixture f;reset();failure=failAt;changeHandle=true;dataSteps={{SQL_STILL_EXECUTING},{SQL_SUCCESS}};
        int caught=invoke([&]{functions.more(&f.rs);});saveState(result,f,caught);
    }
    longbinary_rfx_test::reset({});return result;
}
static std::vector<Event> bookmarks(const ResultsAPI& functions) {
    std::vector<Event> result;
    for(bool set:{false,true})for(UINT type:{1u,2u})for(DWORD flags:{0ul,0x100ul,0x900ul})for(bool supported:{false,true})
    for(bool returns:{false,true})for(DWORD valueType:{0ul,1ul,4ul,12ul}) {
        Fixture f;reset();f.rs.m_nOpenType=type;f.rs.m_dwOptions=flags;f.db.m_dwBookmarkAttributes=supported?SQL_BP_SCROLL:0;
        f.table[7]=reinterpret_cast<void*>(&move);returningThrow=returns;CDBVariant value;value.m_dwType=valueType;
        UINT64 bits=0xa5a5a5a5ffffffffULL;memcpy(&value.m_lVal,&bits,8);dataSteps={{SQL_STILL_EXECUTING},{SQL_SUCCESS}};
        int caught=invoke([&]{if(set)functions.set(&f.rs,&value);else functions.get(&f.rs,&value);});
        UINT64 after;memcpy(&after,&value.m_lVal,8);event(75,value.m_dwType,value.m_lVal,after>>32);saveState(result,f,caught);
    }
    for(bool set:{false,true})for(bool noDatabase:{false,true})for(bool noConnection:{false,true})for(DWORD flags:{0ul,0x100ul})for(bool returns:{false,true}) {
        Fixture f;reset();f.table[7]=reinterpret_cast<void*>(&move);if(noDatabase)f.rs.m_pDatabase=nullptr;if(noConnection)f.db.m_hdbc=nullptr;
        f.rs.m_dwOptions=flags;f.db.m_dwBookmarkAttributes=SQL_BP_SCROLL;returningThrow=returns;CDBVariant value;value.m_dwType=DBVT_LONG;value.m_lVal=-7;
        dataSteps={{SQL_SUCCESS}};int caught=invoke([&]{if(set)functions.set(&f.rs,&value);else functions.get(&f.rs,&value);});saveState(result,f,caught);
    }
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_NO_DATA),RETCODE(SQL_ERROR)})
    for(bool accepted:{false,true})for(bool returns:{false,true})for(int failAt:{0,1,4,5,11,14,15}) {
        Fixture f;reset();f.rs.m_dwOptions=CRecordset::useBookmarks;f.db.m_dwBookmarkAttributes=SQL_BP_SCROLL;
        recordset_move_test::accept=accepted;returningThrow=returns;failure=failAt;dataSteps={{code}};CDBVariant value;value.m_dwType=DBVT_LONG;value.m_lVal=7;
        int caught=invoke([&]{functions.get(&f.rs,&value);});event(75,value.m_dwType,value.m_lVal);saveState(result,f,caught);
    }
    for(bool set:{false,true})for(long bookmark:{-2147483647L-1,-7L,0L,2147483647L}) {
        Fixture f;reset();f.table[7]=reinterpret_cast<void*>(&move);f.rs.m_dwOptions=CRecordset::useBookmarks;f.db.m_dwBookmarkAttributes=SQL_BP_SCROLL;
        CDBVariant value;value.m_dwType=DBVT_LONG;value.m_lVal=bookmark;dataSteps={{SQL_SUCCESS}};
        int caught=invoke([&]{if(set)functions.set(&f.rs,&value);else functions.get(&f.rs,&value);});saveState(result,f,caught);
    }
    for(bool accepted:{false,true}) {
        Fixture f;reset();f.rs.m_dwOptions=CRecordset::useBookmarks;f.db.m_dwBookmarkAttributes=SQL_BP_SCROLL;recordset_move_test::accept=accepted;
        database_strings_test::Manager manager;auto* bytes=manager.Allocate(6,2);bytes->nDataLength=6;
        auto** string=static_cast<void**>(::operator new(sizeof(void*)));*string=bytes+1;
        CDBVariant value;value.m_dwType=DBVT_WSTRING;value.m_pstring=reinterpret_cast<CString*>(string);dataSteps={{SQL_SUCCESS}};
        int caught=invoke([&]{functions.get(&f.rs,&value);});
        check(manager.frees==1 && value.m_dwType==DBVT_LONG,"GetBookmark releases an existing owned string through its manager before installing the native long value");
        event(76,manager.frees,value.m_dwType,value.m_lVal);saveState(result,f,caught);
    }
    longbinary_rfx_test::reset({});return result;
}
}
static void recordsetResults() {
    using namespace recordset_result_test;
    auto oldData=longbinary_rfx_test::dataHook;auto oldMore=database_execute_test::moreHook;
    longbinary_rfx_test::dataHook=&data;database_execute_test::moreHook=&more;
    ResultsAPI local{&impl__GetData_CRecordset__SA_JPEAVCDatabase__PEAXFF1_JF_Z,&impl__FlushResultSet_CRecordset__QEAAHXZ,
        &impl__GetBookmark_CRecordset__QEAAXAEAVCDBVariant___Z,&impl__SetBookmark_CRecordset__QEAAXAEBVCDBVariant___Z,
        &impl__GetDefaultFieldType_CRecordset__SAFF_Z,&impl__GetTextLen_CRecordset__SA_JF_K_Z};
    auto values=retrieval(local);auto results=advancing(local);auto marks=bookmarks(local);auto formatted=formats(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_RESULTS_DLL",path,1024)) {
        auto module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for recordset retrieval and bookmark comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"result oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch{size_t rva;void* function;void* saved;DWORD protection;};Patch patches[]={{0x3e9120,reinterpret_cast<void*>(&SQLGetData)},
                    {0x3e90b0,reinterpret_cast<void*>(&SQLMoreResults)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)}};
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);p.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&p.protection);*slot=p.function;}
                ResultsAPI native{reinterpret_cast<decltype(local.data)>(base+0x1fd8e0),reinterpret_cast<decltype(local.more)>(base+0x1f91b0),
                    reinterpret_cast<decltype(local.get)>(base+0x1f8cb0),reinterpret_cast<decltype(local.set)>(base+0x1f8d90),
                    reinterpret_cast<decltype(local.fieldType)>(base+0x1fd4b0),reinterpret_cast<decltype(local.textLength)>(base+0x1fd880)};
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(retrieval(native),values,"static GetData driver arguments, full-width indicators, asynchronous retries and exception behavior match native MFC");
                compare(advancing(native),results,"single-result advancement, final-result currency reset, field fixup gates and failures match native MFC");
                compare(bookmarks(native),marks,"bookmark validation, variant ownership, long-value bits, driver status and virtual movement match native MFC");
                check(formats(native)==formatted,"default field types and text-buffer sizes match native MFC for all 65536 SQL type codes and full-width precision boundaries");
                for(auto& p:patches){auto** slot=reinterpret_cast<void**>(base+p.rva);*slot=p.saved;DWORD ignored;::VirtualProtect(slot,8,p.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    longbinary_rfx_test::dataHook=oldData;database_execute_test::moreHook=oldMore;
}
