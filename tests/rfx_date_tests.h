#pragma once
#include "detail/CRecordsetProxies.h"
#include <ctime>
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCTime_ATL___Z(CFieldExchange*,const wchar_t*,void*);
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCOleDateTime_ATL___Z(CFieldExchange*,const wchar_t*,COleDateTime*);
namespace date_rfx_test {
using Action=void (MS_ABI*)(CFieldExchange*,const wchar_t*,void*);
struct Type {bool ole;Action action;int bytes,cache;size_t rva;};
std::vector<UINT64> exercise(const Type& type) {
    using namespace binary_rfx_test;
    recordset_storage_test::Fixture fixture(2,2);auto* rs=fixture.recordset;rs->AllocStatusArrays();rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);*reinterpret_cast<void***>(rs)=slots;
    CODBCFieldInfo meta[2]{};meta[0].m_nSQLType=SQL_TYPE_TIMESTAMP;meta[0].m_nPrecision=31;meta[0].m_nScale=7;rs->m_rgODBCFieldInfos=meta;
    alignas(8) BYTE storage[24];memset(storage,0xa5,sizeof(storage));void* value=storage;
    TIMESTAMP_STRUCT row{2026,10,14,12,34,56,999};
    auto real=[&] {
        if(type.ole) {
            auto* date=reinterpret_cast<COleDateTime*>(value);SYSTEMTIME system{};
            system.wYear=row.year;system.wMonth=row.month;system.wDay=row.day;system.wHour=row.hour;system.wMinute=row.minute;system.wSecond=row.second;
            ::SystemTimeToVariantTime(&system,&date->m_dt);date->m_status=COleDateTime::DT_VALID;
        } else {
            tm local{};local.tm_year=row.year-1900;local.tm_mon=row.month-1;local.tm_mday=row.day;local.tm_hour=row.hour;local.tm_min=row.minute;local.tm_sec=row.second;local.tm_isdst=-1;
            *reinterpret_cast<__int64*>(value)=_mktime64(&local);
        }
    };
    auto pseudoNull=[&] {if(type.ole)reinterpret_cast<COleDateTime*>(value)->m_status=COleDateTime::DT_NULL;else *reinterpret_cast<__int64*>(value)=0;};
    auto null=[&] {return type.ole ? reinterpret_cast<COleDateTime*>(value)->m_status==COleDateTime::DT_NULL : *reinterpret_cast<__int64*>(value)==0;};
    auto run=[&](UINT operation,UINT fieldType=0) {CFieldExchange fx(operation,rs);fx.SetFieldType(fieldType);fx.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);type.action(&fx,L"date",value);return fx;};
    auto column=[&] {return static_cast<TIMESTAMP_STRUCT*>(rs->m_pvFieldProxy[0]);};
    auto param=[&] {return static_cast<TIMESTAMP_STRUCT*>(rs->m_pvParamProxy[0]);};
    std::vector<UINT64> result;
    auto save=[&] {
        result.push_back(rs->m_pbFieldFlags[0]);result.push_back(rs->m_rgFieldInfos[0].m_nLength);
        for(int i=0;i<(type.ole?12:8);++i)result.push_back(storage[i]);
        if(rs->m_pvFieldProxy)for(int i=0;i<16;++i)result.push_back(reinterpret_cast<BYTE*>(column())[i]);
    };
    real();resetSQL();run(CFieldExchange::BindFieldToColumn);
    check(rs->m_pvFieldProxy && rs->m_nProxyFields==2 && !rs->m_pvFieldProxy[1] && binding.data==column() && binding.cType==SQL_C_TIMESTAMP && binding.capacity==16 &&
        binding.length==&rs->m_rgFieldInfos[0].m_nLength && rs->GetBoundFieldIndex(value)==1,"date column binding allocates native recordset proxy arrays and registers the value object");
    BYTE expectedValue[16];memcpy(expectedValue,storage,type.bytes);
    *column()=row;rs->m_rgFieldInfos[0].m_nLength=16;run(CFieldExchange::Fixup);
    check(!memcmp(expectedValue,storage,type.ole?12:8),"date Fixup converts a fetched timestamp into its native date value and discards fractions");
    for(TIMESTAMP_STRUCT fetched: {TIMESTAMP_STRUCT{2040,2,29,1,2,3,0},TIMESTAMP_STRUCT{2026,2,31,1,2,3,0},TIMESTAMP_STRUCT{2026,13,1,1,2,3,0}}) {
        *column()=fetched;bool invalid=false;
        try {run(CFieldExchange::Fixup);}catch(COleException* error){invalid=true;error->Delete();}
        if(type.ole)check(reinterpret_cast<COleDateTime*>(value)->m_status==(fetched.month==2 && fetched.year==2040 ? COleDateTime::DT_VALID:COleDateTime::DT_INVALID),
            "OLE date Fixup validates calendar round trips and marks normalized or invalid dates as invalid");
        else check(!invalid && null()==(fetched.month==13) && rs->IsFieldStatusNull(0)==(fetched.month==13?2:0) && rs->m_rgFieldInfos[0].m_nLength==16,
            "CTime Fixup normalizes calendar overflow and converts invalid dates into null values without changing fetched length");
        result.push_back(invalid);if(type.ole)result.push_back(reinterpret_cast<COleDateTime*>(value)->m_status);
    }
    rs->ClearFieldStatus();real();
    *column()=row;run(CFieldExchange::BindFieldForUpdate);
    check(column()->year==row.year && column()->month==row.month && column()->day==row.day && !column()->fraction && rs->m_rgFieldInfos[0].m_nLength==SQL_IGNORE,
        "date update binding converts the live value into the existing proxy and ignores a clean field");save();
    auto* oldColumn=column();resetSQL();run(CFieldExchange::BindFieldToColumn);check(column()==oldColumn,"date repeated column binding reuses the existing proxy slot");
    rs->m_pbParamFlags[0]=2;resetSQL();run(CFieldExchange::BindParam,4);
    check(rs->m_plParamLength[0]==-1 && (type.ole ? !binding.data && !rs->m_pvParamProxy && !rs->m_nProxyParams :
        binding.data==param() && rs->m_nProxyParams==2),
        "initially null OLE date parameters bind a null pointer while CTime parameters allocate a native proxy");
    result.push_back(binding.data==nullptr);result.push_back(rs->m_nProxyParams);
    resetSQL();run(CFieldExchange::RebindParam,4);
    check(!binds && rs->m_plParamLength[0]==-1,"initially null date parameters rebind without requiring or allocating a timestamp proxy");
    rs->m_pbParamFlags[0]=0;resetSQL();run(CFieldExchange::BindParam,4);
    check(rs->m_bRebindParams==TRUE && rs->m_pvParamProxy && rs->m_nProxyParams==2 && !rs->m_pvParamProxy[1] && binding.param && binding.data==param() &&
        binding.statement==reinterpret_cast<HSTMT>(0xbeef) && binding.direction==4 && binding.cType==SQL_C_TIMESTAMP && binding.sqlType==SQL_TIMESTAMP && binding.precision==23 &&
        !binding.scale && !binding.capacity && rs->m_plParamLength[0]==16 && rs->GetBoundParamIndex(value)==1,
        "date parameter binding publishes native proxy ownership, rebinding flag, statement, direction and timestamp metadata");
    check(param()->year==row.year && param()->second==row.second && !param()->fraction,"date parameter conversion fills calendar fields and discards fractions");
    auto* oldParam=param();memset(param(),0x5a,16);rs->m_pbParamFlags[0]=2;resetSQL();run(CFieldExchange::RebindParam,4);
    check(!binds && rs->m_plParamLength[0]==-1 && (type.ole?param()->year==row.year:reinterpret_cast<BYTE*>(param())[0]==0x5a),
        "null date parameter rebinding preserves CTime proxies and refreshes OLE date proxies with native behavior");
    result.push_back(rs->m_plParamLength[0]);for(int i=0;i<16;++i)result.push_back(reinterpret_cast<BYTE*>(param())[i]);
    rs->m_pbParamFlags[0]=0;run(CFieldExchange::RebindParam,4);check(param()==oldParam && rs->m_plParamLength[0]==16 && param()->year==row.year,"nonnull date parameter rebinding refreshes the existing proxy without allocation");
    resetSQL(SQL_SUCCESS_WITH_INFO);int exception=0;try {run(CFieldExchange::BindParam,4);}catch(int code){exception=code;}
    check(exception==701 && throws==1 && binding.statement==reinterpret_cast<HSTMT>(0xbeef),"date parameter bind errors use the exchange statement and native recordset exception callback");
    CFieldExchange fx(CFieldExchange::SetFieldNull,rs);fx.SetFieldType(0);fx.m_bField=TRUE;
    type.action(&fx,L"date",value);check(null() && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1,"date null setting installs the native value sentinel and SQL-null indicator");save();
    fx.m_nFields=0;fx.m_bField=FALSE;type.action(&fx,L"date",value);
    check(null() && !rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==16,"date null clearing preserves its sentinel and restores timestamp byte length");
    real();int other=0;fx.m_nFields=0;fx.m_bField=TRUE;fx.m_pvField=&other;type.action(&fx,L"date",value);check(!null(),"date null operation ignores a different selected value");
    fx.m_nFields=0;fx.m_pvField=value;type.action(&fx,L"date",value);check(null(),"date null operation selects the value object address");
    real();rs->m_rgFieldInfos[0].m_nLength=-1;run(CFieldExchange::Fixup);check(null() && rs->IsFieldStatusNull(0)==2,"date SQL-null Fixup writes the native sentinel");
    rs->ClearFieldStatus();run(CFieldExchange::MarkForAddNew);check(!rs->IsFieldStatusDirty(0),"pseudo-null date remains clean during add-new exchange");
    real();run(CFieldExchange::MarkForAddNew);check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0),"real date becomes dirty and non-null during add-new exchange");
    rs->ClearFieldStatus();run(CFieldExchange::AllocCache);
    check(rs->m_rgFieldInfos[0].m_nDataType==type.cache && rs->m_rgFieldInfos[0].m_pvDataCache && !*reinterpret_cast<UINT64*>(rs->m_rgFieldInfos[0].m_pvDataCache),
        "date cache allocation creates its native value type with initialized date storage");
    run(CFieldExchange::StoreField);if(type.ole)reinterpret_cast<COleDateTime*>(value)->m_dt+=1;else ++*reinterpret_cast<__int64*>(value);
    auto dirty=run(CFieldExchange::MarkForUpdate);check(rs->IsFieldStatusDirty(0)==1 && dirty.m_bField,"date cache comparison marks changed values and reports the selected field");
    rs->ClearFieldStatus();memset(column(),0x5a,16);run(CFieldExchange::LoadField);
    check(column()->year==row.year && column()->month==row.month && column()->second==row.second && !column()->fraction && rs->m_rgFieldInfos[0].m_nLength==16,
        "date cache restore reloads value bytes, native length and the timestamp proxy");save();
    dirty=run(CFieldExchange::MarkForUpdate);check(!rs->IsFieldStatusDirty(0) && !dirty.m_bField,"unchanged date cache values remain clean");
    rs->SetNullFieldStatus(0);run(CFieldExchange::StoreField);rs->ClearFieldStatus();real();memset(column(),0x5a,16);run(CFieldExchange::LoadField);
    check(!null() && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1 && reinterpret_cast<BYTE*>(column())[0]==0x5a,"cached-null date load preserves both the current value and proxy");
    rs->ClearFieldStatus();rs->SetDirtyFieldStatus(0);CString sql;
    auto sqlExchange=[&](UINT operation) {sql.Empty();CFieldExchange query(operation,rs);query.SetFieldType(0);query.m_pstr=&sql;query.m_lpszSeparator=L", ";query.m_nParamFields=3;query.m_hstmt=reinterpret_cast<HSTMT>(0xface);type.action(&query,L"date",value);return query;};
    resetSQL(SQL_ERROR,false);auto query=sqlExchange(CFieldExchange::NameValue);
    check(sql==L"date=?, " && query.m_nParamFields==4 && binding.param && binding.number==4 && binding.data==column() && binding.cType==SQL_C_TIMESTAMP &&
        binding.sqlType==SQL_TYPE_TIMESTAMP && binding.precision==23 && !binding.scale && !binding.capacity && !throws && rs->m_rgFieldInfos[0].m_nLength==16,
        "date NameValue binds its refreshed proxy using native precision and deliberately ignores the SQL return code");save();
    rs->m_dwOptions=CRecordset::optimizeBulkAdd;resetSQL();query=sqlExchange(CFieldExchange::NameValue);
    check(sql==L"date=" && !binds && query.m_nParamFields==3,"optimized date inserts generate the field name without a placeholder or parameter binding");
    rs->m_dwOptions=0;rs->SetNullFieldStatus(0);memset(column(),0x5a,16);resetSQL();sqlExchange(CFieldExchange::Value);
    check(sql==L"?, " && rs->m_rgFieldInfos[0].m_nLength==-1 && reinterpret_cast<BYTE*>(column())[0]==0x5a,"null date SQL values leave proxy bytes untouched and bind SQL_NULL_DATA");
    rs->ClearFieldStatus();resetSQL();sqlExchange(CFieldExchange::Value);check(sql.IsEmpty() && !binds,"clean date fields skip SQL value generation");
    rs->FreeDataCache();openmfc::detail::FreeRecordsetProxies(*rs);
    check(!rs->m_pvFieldProxy && !rs->m_pvParamProxy && !rs->m_nProxyFields && !rs->m_nProxyParams,"recordset proxy cleanup frees both native arrays and resets their ownership fields");
    rs->m_rgFieldInfos[0].m_nLength=111;run(CFieldExchange::BindFieldForUpdate);
    check(rs->m_rgFieldInfos[0].m_nLength==111,"date update binding without a proxy array leaves field state untouched");
    check(*reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL &&
        *reinterpret_cast<UINT64*>(storage+(type.ole?16:8))==0xa5a5a5a5a5a5a5a5ULL,"date exchange preserves recordset and caller value allocation guards");
    return result;
}
}
static void dateFieldExchange() {
    using namespace date_rfx_test;
    static_assert(sizeof(COleDateTime)==16 && offsetof(COleDateTime,m_status)==8,"native OLE date layout");
    Type time{false,&impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCTime_ATL___Z,8,8,0x201140};
    Type ole{true,reinterpret_cast<Action>(&impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCOleDateTime_ATL___Z),16,16,0x201e20};
    auto timeResult=exercise(time);auto oleResult=exercise(ole);wchar_t path[1024];
    if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for date RFX comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);void** column=reinterpret_cast<void**>(base+0x3e9038);void** parameter=reinterpret_cast<void**>(base+0x3e9040);
            auto* savedColumn=*column;auto* savedParameter=*parameter;DWORD protection;::VirtualProtect(column,16,PAGE_READWRITE,&protection);
            *column=reinterpret_cast<void*>(&SQLBindCol);*parameter=reinterpret_cast<void*>(&SQLBindParameter);
            time.action=reinterpret_cast<Action>(base+time.rva);ole.action=reinterpret_cast<Action>(base+ole.rva);
            check(exercise(time)==timeResult,"CTime date exchange matches native values, proxy bytes, null status, caches and bindings");
            check(exercise(ole)==oleResult,"OLE date exchange matches native values, proxy bytes, null status, caches and bindings");
            *column=savedColumn;*parameter=savedParameter;DWORD ignored;::VirtualProtect(column,16,protection,&ignored);::FreeLibrary(module);
        }
    }
}
