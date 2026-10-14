#pragma once
#include "openmfc/afxdao.h"
extern "C" void MS_ABI impl__RFX_LongBinary__YAXPEAVCFieldExchange__PEB_WAEAVCLongBinary___Z(CFieldExchange*,const wchar_t*,CLongBinary*);
extern "C" SQLLEN MS_ABI impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(CFieldExchange*,int);
extern "C" BYTE* MS_ABI impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(CFieldExchange*,CLongBinary*,SQLLEN,SQLLEN);
extern "C" void MS_ABI impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(CFieldExchange*,int,CLongBinary*,SQLLEN*);
extern "C" CLongBinary* MS_ABI impl___0CLongBinary__QEAA_XZ(void*);
extern "C" void MS_ABI impl___1CLongBinary__UEAA_XZ(CLongBinary*);
namespace longbinary_rfx_test {
struct API {
    decltype(&impl__RFX_LongBinary__YAXPEAVCFieldExchange__PEB_WAEAVCLongBinary___Z) exchange;
    decltype(&impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z) size;
    decltype(&impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z) realloc;
    decltype(&impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z) fetch;
};
struct Step {RETCODE code;SQLLEN length;std::vector<BYTE> data;};
struct Diagnostic {const wchar_t* state;SQLINTEGER native;const wchar_t* message;};
struct Read {SQLHSTMT statement;SQLUSMALLINT field;SQLSMALLINT type;SQLLEN capacity;};
static std::vector<Step> script;static size_t position;
static std::vector<Read> reads;static std::vector<Diagnostic> diagnostics;static size_t diagnosticPosition;
static int diagCalls,fetchGrowth,reallocGrowth;
static SQLLEN MS_ABI growFetch(CRecordset*,SQLLEN size) {++fetchGrowth;return size*2;}
static SQLLEN MS_ABI growRealloc(CRecordset*,SQLLEN size) {++reallocGrowth;return size*2;}
static std::vector<BYTE> payload(size_t size,int seed=1) {std::vector<BYTE> bytes(size);for(size_t i=0;i<size;++i)bytes[i]=BYTE(i+seed);return bytes;}
static void reset(std::vector<Step> steps,std::vector<Diagnostic> errors={}) {script=std::move(steps);position=0;reads.clear();diagnostics=std::move(errors);diagnosticPosition=0;diagCalls=fetchGrowth=reallocGrowth=0;}
std::vector<UINT64> exercise(const API& api) {
    using namespace binary_rfx_test;
    recordset_storage_test::Fixture fixture(1,1);auto* rs=fixture.recordset;rs->AllocStatusArrays();rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    CDatabase database;database.m_hdbc=reinterpret_cast<HDBC>(0xfeed);rs->m_pDatabase=&database;
    CODBCFieldInfo metadata{};metadata.m_nSQLType=SQL_LONGVARBINARY;rs->m_rgODBCFieldInfos=&metadata;
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);slots[25]=reinterpret_cast<void*>(&growFetch);slots[26]=reinterpret_cast<void*>(&growRealloc);
    *reinterpret_cast<void***>(rs)=slots;
    CLongBinary binary;std::vector<UINT64> result;
    auto run=[&](UINT operation) {CFieldExchange fx(operation,rs);fx.SetFieldType(0);fx.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);fx.m_lDefaultLBFetchSize=4;fx.m_lDefaultLBReallocSize=4;
        api.exchange(&fx,L"blob",&binary);return fx;};
    auto save=[&] {result.push_back(rs->m_pbFieldFlags[0]);result.push_back(rs->m_rgFieldInfos[0].m_nLength);result.push_back(binary.m_dwDataLength);};
    for(bool updateSQL: {false,true})for(bool updatable: {false,true})for(bool appendable: {false,true}) {
        rs->m_bUseUpdateSQL=updateSQL;rs->m_bUpdatable=updatable;rs->m_bAppendable=appendable;resetSQL();run(CFieldExchange::BindFieldToColumn);
        bool bind=!updateSQL && (updatable || appendable);
        check(binds==int(bind) && checks==int(bind) && rs->GetBoundFieldIndex(&binary)==1 &&
            (!bind || (binding.cType==SQL_C_DEFAULT && binding.data==&binary && !binding.capacity && binding.length==&rs->m_rgFieldInfos[0].m_nLength)),
            "long binary column binding follows native update mode and capability gates");result.push_back(binds);
    }
    rs->ClearFieldStatus();binary.m_dwDataLength=9;run(CFieldExchange::BindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==SQL_IGNORE,"clean long binary update fields are ignored");
    rs->SetDirtyFieldStatus(0);
    for(SQLULEN size: {SQLULEN(0),SQLULEN(9),SQLULEN(1ULL<<40),SQLULEN(-1)}) {
        binary.m_dwDataLength=size;run(CFieldExchange::BindFieldForUpdate);
        check(rs->m_rgFieldInfos[0].m_nLength==SQL_LEN_DATA_AT_EXEC(SQLLEN(std::min<SQLULEN>(size,INT_MAX-100))),
            "long binary update indicators clamp full-width lengths with native data-at-execution arithmetic");save();
        run(CFieldExchange::UnbindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==SQLLEN(size),"long binary unbind restores the full 64-bit value length");
    }
    CFieldExchange fx(CFieldExchange::SetFieldNull,rs);fx.SetFieldType(0);fx.m_bField=TRUE;api.exchange(&fx,L"blob",&binary);
    check(!binary.m_dwDataLength && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1,"long binary null setting clears all length bits and installs SQL_NULL_DATA");save();
    fx.m_nFields=0;fx.m_bField=FALSE;binary.m_dwDataLength=5;api.exchange(&fx,L"blob",&binary);
    check(!rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==SQL_LEN_DATA_AT_EXEC(5),"long binary null clearing restores its data-at-execution indicator");
    CString sql;auto query=[&](UINT operation) {sql.Empty();CFieldExchange exchange(operation,rs);exchange.SetFieldType(0);exchange.m_pstr=&sql;exchange.m_lpszSeparator=L", ";exchange.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);api.exchange(&exchange,L"blob",&binary);return exchange;};
    rs->SetDirtyFieldStatus(0);query(CFieldExchange::Name);check(sql==L"blob, " && rs->m_bLongBinaryColumns==TRUE,"long binary Name records its column capability and generates SQL");
    resetSQL();auto named=query(CFieldExchange::NameValue);
    check(sql==L"blob=?, " && named.m_nParamFields==1 && binding.param && binding.data==&binary && binding.cType==SQL_C_DEFAULT && binding.sqlType==SQL_LONGVARBINARY &&
        binding.precision==5 && !binding.scale && !binding.capacity && rs->m_rgFieldInfos[0].m_nLength==SQL_LEN_DATA_AT_EXEC(5),
        "long binary SQL values bind the object as a data-at-execution token with native precision");save();
    rs->m_dwOptions=CRecordset::optimizeBulkAdd;resetSQL();query(CFieldExchange::NameValue);check(sql==L"blob=" && !binds,"optimized long binary inserts skip placeholder and binding");rs->m_dwOptions=0;
    resetSQL(SQL_ERROR,false);int error=0;try {query(CFieldExchange::Value);}catch(int code){error=code;}
    check(error==699 && throws==1 && binding.statement==reinterpret_cast<HSTMT>(0xbeef),"long binary parameter errors reach the native statement exception callback");
    CFieldExchange memory(CFieldExchange::Fixup,rs);
    auto* first=api.realloc(&memory,&binary,100,16);check(first && ::GlobalSize(binary.m_hData)>=16 && ::GlobalSize(binary.m_hData)<100,"initial long binary allocation uses the reallocation-size argument");
    memset(first,0x5a,16);::GlobalUnlock(binary.m_hData);auto handle=binary.m_hData;
    auto* again=api.realloc(&memory,&binary,SQLLEN((1ULL<<32)+1),0);
    check(binary.m_hData==handle && again[0]==0x5a,"long binary reuse follows the native DWORD capacity comparison without losing data");::GlobalUnlock(binary.m_hData);
    again=api.realloc(&memory,&binary,100,120);check(again && ::GlobalSize(binary.m_hData)>=120 && again[15]==0x5a,"long binary growth preserves bytes through GlobalReAlloc");::GlobalUnlock(binary.m_hData);
    reset({{SQL_STILL_EXECUTING,0,{}},{SQL_SUCCESS_WITH_INFO,SQL_NO_TOTAL,{}}});
    check(api.size(&memory,1)==SQL_NO_TOTAL && reads.size()==2 && reads[0].type==SQL_C_DEFAULT && !reads[0].capacity,"long binary size probes retry pending calls and preserve ODBC indicators");
    auto content=payload(6000);reset({{SQL_SUCCESS,6000,content}});SQLLEN length=6000;api.fetch(&memory,1,&binary,&length);
    auto* bytes=static_cast<BYTE*>(::GlobalLock(binary.m_hData));bool equal=!memcmp(bytes,content.data(),content.size());::GlobalUnlock(binary.m_hData);
    check(equal && binary.m_dwDataLength==6000 && length==6000 && reads[0].capacity==6000,"long binary fetch retrieves the complete known-size payload beyond the former 4096-byte limit");
    result.push_back(length);result.push_back(binary.m_dwDataLength);
    memory.m_lDefaultLBFetchSize=4;memory.m_lDefaultLBReallocSize=4;
    for(const auto& diag: std::vector<std::vector<Diagnostic>>{{{L"01004",0,L"truncated"}},{{L"01000",0,L"warning"},{L"01004",0,L"truncated"}}}) {
        auto one=payload(4,1),two=payload(5,5);reset({{SQL_SUCCESS_WITH_INFO,SQL_NO_TOTAL,one},{SQL_STILL_EXECUTING,0,{}},{SQL_SUCCESS,5,two}},diag);
        length=SQL_NO_TOTAL;
        api.fetch(&memory,1,&binary,&length);
        bytes=static_cast<BYTE*>(::GlobalLock(binary.m_hData));equal=!memcmp(bytes,one.data(),4) && !memcmp(bytes+4,two.data(),5);::GlobalUnlock(binary.m_hData);
        check(equal && length==9 && binary.m_dwDataLength==9 && fetchGrowth==1 && reallocGrowth==1 && reads.size()==3 && reads[0].capacity==4 && reads[1].capacity==8 && diagCalls==int(diag.size()+1),
            "long binary truncated fetch grows through native callbacks, reads every diagnostic record and appends all payload bytes");
        result.push_back(length);result.push_back(diagCalls);result.push_back(fetchGrowth);
    }
    reset({{SQL_SUCCESS_WITH_INFO,3,payload(3)}},{{L"01000",0,L"warning"}});length=4;api.fetch(&memory,1,&binary,&length);
    check(length==3 && binary.m_dwDataLength==3 && !fetchGrowth,"nontruncation long binary warnings preserve the returned length and stop fetching");
    reset({{SQL_SUCCESS_WITH_INFO,3,payload(3)}},{{L"01000",0,L"[01004] origin"}});length=4;api.fetch(&memory,1,&binary,&length);
    check(length==3 && !fetchGrowth,"long binary diagnostics distinguish the SQLSTATE from identical digits in its origin");
    reset({{SQL_SUCCESS_WITH_INFO,SQL_NO_TOTAL,payload(4)}},{{L"01000",0,L"[01004] origin"}});length=SQL_NO_TOTAL;bool memoryError=false;
    try {api.fetch(&memory,1,&binary,&length);}catch(CException* exception){memoryError=true;exception->Delete();}catch(const std::bad_alloc&){memoryError=true;}
    check(memoryError && !fetchGrowth && reads.size()==1 && length==-4 && binary.m_dwDataLength==SQLULEN(-4),
        "unknown-length nontruncation warnings preserve native arithmetic and reject the next invalid allocation size");result.push_back(memoryError);
    reset({{SQL_NO_DATA,0,{}}});length=4;api.fetch(&memory,1,&binary,&length);check(length==-1 && rs->IsFieldStatusNull(0)==2,"long binary end-of-data marks the field null");
    reset({{SQL_ERROR,0,{}}});length=4;error=0;try {api.fetch(&memory,1,&binary,&length);}catch(int code){error=code;}
    check(error==699 && !binding.statement && !(::GlobalFlags(binary.m_hData)&GMEM_LOCKCOUNT),"long binary fetch errors unlock global storage before throwing through the recordset");
    rs->ClearFieldStatus();reset({{SQL_SUCCESS,SQL_NULL_DATA,{}}});binary.m_dwDataLength=SQLULEN(1ULL<<40);run(CFieldExchange::Fixup);
    check(binary.m_dwDataLength==0 && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1,"long binary Fixup handles SQL NULL without fetching and clears the 64-bit value length");save();
    reset({{SQL_SUCCESS,6000,{}},{SQL_SUCCESS,6000,content}});run(CFieldExchange::Fixup);check(binary.m_dwDataLength==6000 && !rs->IsFieldStatusNull(0),"long binary Fixup queries size, fetches bytes and clears null status");save();
    database.m_hdbc=nullptr;check(*reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL,"long binary exchange preserves native recordset allocation guards");return result;
}
}
extern "C" SQLRETURN SQL_API SQLGetData(SQLHSTMT statement,SQLUSMALLINT field,SQLSMALLINT type,SQLPOINTER output,SQLLEN capacity,SQLLEN* indicator) {
    using namespace longbinary_rfx_test;
    reads.push_back({statement,field,type,capacity});if(position>=script.size())return SQL_ERROR;
    const auto& step=script[position++];*indicator=step.length;
    if(capacity>0 && output && !step.data.empty())memcpy(output,step.data.data(),std::min<size_t>(size_t(capacity),step.data.size()));
    if(step.code==SQL_SUCCESS_WITH_INFO)diagnosticPosition=0;return step.code;
}
extern "C" SQLRETURN SQL_API SQLErrorW(SQLHENV,SQLHDBC,SQLHSTMT,SQLWCHAR* state,SQLINTEGER* native,SQLWCHAR* message,SQLSMALLINT capacity,SQLSMALLINT* length) {
    using namespace longbinary_rfx_test;++diagCalls;
    if(diagnosticPosition>=diagnostics.size()) {wcscpy(reinterpret_cast<wchar_t*>(state),L"00000");*length=0;return SQL_NO_DATA;}
    const auto& row=diagnostics[diagnosticPosition++];wcscpy(reinterpret_cast<wchar_t*>(state),row.state);*native=row.native;
    wcsncpy(reinterpret_cast<wchar_t*>(message),row.message,capacity-1);message[capacity-1]=0;*length=SQLSMALLINT(wcslen(row.message));return SQL_SUCCESS;
}
static void longBinaryFieldExchange() {
    using namespace longbinary_rfx_test;
    alignas(CLongBinary) BYTE storage[32];memset(storage,0xa5,sizeof(storage));auto* value=impl___0CLongBinary__QEAA_XZ(storage);
    check(sizeof(CLongBinary)==24 && !value->m_dwDataLength && !value->m_hData && value->GetRuntimeClass()->m_nObjectSize==24 && !strcmp(value->GetRuntimeClass()->m_lpszClassName,"CLongBinary") &&
        *reinterpret_cast<UINT64*>(storage+24)==0xa5a5a5a5a5a5a5a5ULL,"long binary constructor exposes native layout, runtime identity and full-width zero initialization");impl___1CLongBinary__UEAA_XZ(value);
    API api{&impl__RFX_LongBinary__YAXPEAVCFieldExchange__PEB_WAEAVCLongBinary___Z,&impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z,
        &impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z,&impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z};
    auto expected=exercise(api);wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_LONGBINARY_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for long binary comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);struct Patch {size_t rva;void* target;void* saved;DWORD protection;};
            Patch patches[]={{0x3e9038,reinterpret_cast<void*>(&SQLBindCol)},{0x3e9040,reinterpret_cast<void*>(&SQLBindParameter)},
                {0x3e9120,reinterpret_cast<void*>(&SQLGetData)},{0x3e90e0,reinterpret_cast<void*>(&SQLErrorW)}};
            for(auto& patch:patches) {auto** slot=reinterpret_cast<void**>(base+patch.rva);patch.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&patch.protection);*slot=patch.target;}
            API native{reinterpret_cast<decltype(api.exchange)>(base+0x202bc0),reinterpret_cast<decltype(api.size)>(base+0x203020),
                reinterpret_cast<decltype(api.realloc)>(base+0x2032f0),reinterpret_cast<decltype(api.fetch)>(base+0x2030a0)};
            check(exercise(native)==expected,"long binary values, null indicators, SQL generation, chunked fetch and diagnostics match native MFC");
            for(auto& patch:patches) {auto** slot=reinterpret_cast<void**>(base+patch.rva);*slot=patch.saved;DWORD ignored;::VirtualProtect(slot,8,patch.protection,&ignored);}::FreeLibrary(module);
        }
    }
}
