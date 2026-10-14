// Native date exchange over recordset-owned timestamp proxy arrays.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxole.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(CRecordset*,UINT,void*,SQLLEN*);
namespace {
// ATL::CTime is { __time64_t m_time }; this codebase declares no CTime, so the
// RFX_Date(CTime&) export sees the object through this view.
struct RetailCTime { __int64 m_time; };
static_assert(sizeof(RetailCTime) == 8, "ATL::CTime is a single __time64_t");

// value -> TIMESTAMP_STRUCT as retail does it (RFX_Date(CTime&) (0x201140), the
// block at 0x2012e9..0x2013ad): six CTime::Get{Year,Month,Day,Hour,Minute,Second}()
// calls, each an _localtime64_s; on failure Year/Month/Day read 0 and
// Hour/Minute/Second -1 (ATL's inline getters); fraction is 0.
void CTimeToTimestamp(const RetailCTime& value,TIMESTAMP_STRUCT* ts) {
    struct tm t;
    ts->year=static_cast<SQLSMALLINT>(_localtime64_s(&t,&value.m_time)==0?t.tm_year+1900:0);
    ts->month=static_cast<SQLUSMALLINT>(_localtime64_s(&t,&value.m_time)==0?t.tm_mon+1:0);
    ts->day=static_cast<SQLUSMALLINT>(_localtime64_s(&t,&value.m_time)==0?t.tm_mday:0);
    ts->hour=static_cast<SQLUSMALLINT>(_localtime64_s(&t,&value.m_time)==0?t.tm_hour:-1);
    ts->minute=static_cast<SQLUSMALLINT>(_localtime64_s(&t,&value.m_time)==0?t.tm_min:-1);
    ts->second=static_cast<SQLUSMALLINT>(_localtime64_s(&t,&value.m_time)==0?t.tm_sec:-1);
    ts->fraction=0;
}
// TIMESTAMP_STRUCT -> CTime: the CTime(int,int,int,int,int,int) constructor
// retail calls at RVA 0x2042f0: range check (year >= 1970, or exactly
// 1969-12-31; month 1..12; day 1..31; hour 0..23; minute/second 0..59) failing
// into AtlThrow(E_FAIL) (== AfxThrowOleException via the stub at 0x333c), then
// _mktime64 with tm_isdst = -1; a -1 result throws E_INVALIDARG.  year is read
// as a signed short, the other fields unsigned.
void TimestampToCTime(const TIMESTAMP_STRUCT& ts, RetailCTime& value) {
    const int year = ts.year, month = ts.month, day = ts.day;
    const unsigned hour = ts.hour, minute = ts.minute, second = ts.second;
    const bool dateOk = (year >= 1970) ? (static_cast<unsigned>(month - 1) <= 11 && static_cast<unsigned>(day - 1) <= 30)
                                       : (year == 1969 && month == 12 && day == 31);
    if (!dateOk || hour > 23 || minute > 59 || second > 59) {
        impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        return;
    }
    struct tm t;
    std::memset(&t, 0, sizeof(t));
    t.tm_sec = static_cast<int>(second);
    t.tm_min = static_cast<int>(minute);
    t.tm_hour = static_cast<int>(hour);
    t.tm_mday = day;
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;
    t.tm_isdst = -1;
    value.m_time = _mktime64(&t);
    if (value.m_time == -1) {
        impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    }
}

// value -> TIMESTAMP_STRUCT as retail does it (RFX_Date(COleDateTime&)
// (0x201e20), the block at 0x201ff3..0x202132): six COleDateTime::Get{Year,...}()
// calls, each `m_status == valid && VariantTimeToSystemTime(m_dt, &st)`
// (OLEAUT32 ordinal 185) and -1 on failure; fraction 0.
void OleDateTimeToTimestamp(const COleDateTime& value,TIMESTAMP_STRUCT* ts) {
    auto part=[&](WORD SYSTEMTIME::*member) {
        SYSTEMTIME st{};
        return value.m_status==COleDateTime::DT_VALID && ::VariantTimeToSystemTime(value.m_dt,&st)==TRUE ? st.*member : WORD(0xffff);
    };
    ts->year=static_cast<SQLSMALLINT>(part(&SYSTEMTIME::wYear));
    ts->month=part(&SYSTEMTIME::wMonth);ts->day=part(&SYSTEMTIME::wDay);
    ts->hour=part(&SYSTEMTIME::wHour);ts->minute=part(&SYSTEMTIME::wMinute);ts->second=part(&SYSTEMTIME::wSecond);ts->fraction=0;
}
// TIMESTAMP_STRUCT -> COleDateTime: the COleDateTime(int x 6) constructor retail
// calls at RVA 0x204254 -> 0x12c8e4: SystemTimeToVariantTime (OLEAUT32 ordinal
// 184), then VariantTimeToSystemTime back and a field-by-field round-trip
// compare (wDayOfWeek excluded); m_status = valid (0) only if everything
// matched, else invalid (1).
void TimestampToOleDateTime(const TIMESTAMP_STRUCT& ts, COleDateTime& value) {
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    st.wYear = static_cast<WORD>(ts.year);
    st.wMonth = static_cast<WORD>(ts.month);
    st.wDay = static_cast<WORD>(ts.day);
    st.wHour = static_cast<WORD>(ts.hour);
    st.wMinute = static_cast<WORD>(ts.minute);
    st.wSecond = static_cast<WORD>(ts.second);
    bool ok = ::SystemTimeToVariantTime(&st, &value.m_dt) != 0;
    if (ok) {
        SYSTEMTIME back;
        std::memset(&back, 0, sizeof(back));
        ok = ::VariantTimeToSystemTime(value.m_dt, &back) != 0 &&
             back.wYear == st.wYear && back.wMonth == st.wMonth && back.wDay == st.wDay &&
             back.wHour == st.wHour && back.wMinute == st.wMinute && back.wSecond == st.wSecond;
    }
    value.m_status = ok ? COleDateTime::DT_VALID : COleDateTime::DT_INVALID;
}

// ---------------------------------------------------------------------------
// RFX_Date(CTime&) (RVA 0x201140) and RFX_Date(COleDateTime&) (RVA 0x201e20)
// share one body shape: the value is never bound directly, a TIMESTAMP_STRUCT
// proxy is, and every operation converts between the two.  The differences are
// the conversion functions and the pseudo-null test (CTime: m_time == 0;
// COleDateTime: m_status == null (2)).
// ---------------------------------------------------------------------------
struct CTimeTraits {
    typedef RetailCTime value_type;
    static constexpr int cacheType=8,bytes=8,initializedBytes=8;
    static constexpr bool ole=false;
    static bool IsPseudoNull(const RetailCTime& v) { return v.m_time == 0; }
    static void SetPseudoNull(RetailCTime& v) { v.m_time = 0; }               // 0x201289: movq $0,(%r14)
    static void ToTimestamp(const RetailCTime& v, TIMESTAMP_STRUCT* ts) { CTimeToTimestamp(v, ts); }
    static void FromTimestamp(const TIMESTAMP_STRUCT& ts, RetailCTime& v) { TimestampToCTime(ts, v); }
};
struct OleDateTimeTraits {
    typedef COleDateTime value_type;
    static constexpr int cacheType=16,bytes=16,initializedBytes=12;
    static constexpr bool ole=true;
    static bool IsPseudoNull(const COleDateTime& v) { return v.m_status == COleDateTime::DT_NULL; }
    static void SetPseudoNull(COleDateTime& v) { v.m_status = COleDateTime::DT_NULL; }  // 0x201f7a: movl $2,0x8(%rsi)
    static void ToTimestamp(const COleDateTime& v, TIMESTAMP_STRUCT* ts) { OleDateTimeToTimestamp(v, ts); }
    static void FromTimestamp(const TIMESTAMP_STRUCT& ts, COleDateTime& v) { TimestampToOleDateTime(ts, v); }
};


void* Allocate(size_t bytes) {
    void* data=std::malloc(bytes);
    if(!data)impl__AfxThrowMemoryException__YAXXZ();return data;
}
TIMESTAMP_STRUCT* CreateProxy(CRecordset* rs,DWORD index,bool parameter) {
    auto*& proxies=parameter?rs->m_pvParamProxy:rs->m_pvFieldProxy;
    auto& count=parameter?rs->m_nProxyParams:rs->m_nProxyFields;
    UINT fields=parameter?rs->m_nParams:rs->m_nFields;
    if(!proxies) {
        proxies=static_cast<void**>(Allocate(size_t(fields)*sizeof(void*)));
        std::memset(proxies,0,size_t(fields)*sizeof(void*));count=fields;
    }
    if(!proxies[index])proxies[index]=Allocate(sizeof(TIMESTAMP_STRUCT));
    return static_cast<TIMESTAMP_STRUCT*>(proxies[index]);
}
TIMESTAMP_STRUCT* Proxy(CRecordset* rs,DWORD index,bool parameter=false) {
    return static_cast<TIMESTAMP_STRUCT*>((parameter?rs->m_pvParamProxy:rs->m_pvFieldProxy)[index]);
}
template<class Traits>
void RfxDateProxied(CFieldExchange* fx,const wchar_t* name,typename Traits::value_type* value) {
    if(!fx || !name) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    UINT field;if(!fx->IsFieldType(&field))return;
    CRecordset* rs=fx->m_prs;
    if(!rs) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    DWORD index=field-1;SQLLEN* length=rs->GetFieldLengthBuffer(index,fx->m_nFieldType);
    if(!length) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    switch(fx->m_nOperation) {
    case CFieldExchange::BindParam: {
        rs->m_bRebindParams=TRUE;
        bool null=rs->IsParamStatusNull(index)!=0;
        TIMESTAMP_STRUCT* proxy=Traits::ole && null ? nullptr:CreateProxy(rs,index,true);
        if(null)*length=SQL_NULL_DATA;
        else {Traits::ToTimestamp(*value,proxy);*length=sizeof(TIMESTAMP_STRUCT);}
        auto code=::SQLBindParameter(fx->m_hstmt,SQLUSMALLINT(field),SQLSMALLINT(fx->m_nFieldType),SQL_C_TIMESTAMP,SQL_TIMESTAMP,23,0,proxy,0,length);
        if(code!=SQL_SUCCESS)rs->ThrowDBException(code,fx->m_hstmt);
        rs->m_mapParamIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(field)));return;
    }
    case CFieldExchange::RebindParam:
        if(rs->IsParamStatusNull(index))*length=SQL_NULL_DATA;
        else *length=sizeof(TIMESTAMP_STRUCT);
        if constexpr(Traits::ole) {
            if(rs->m_nProxyParams)Traits::ToTimestamp(*value,Proxy(rs,index,true));
        } else if(!rs->IsParamStatusNull(index))Traits::ToTimestamp(*value,Proxy(rs,index,true));
        return;
    case CFieldExchange::BindFieldToColumn: {
        auto* proxy=CreateProxy(rs,index,false);
        auto code=::SQLBindCol(rs->m_hstmt,SQLUSMALLINT(field),SQL_C_TIMESTAMP,proxy,sizeof(TIMESTAMP_STRUCT),length);
        if(!rs->Check(code))rs->ThrowDBException(code,nullptr);
        rs->m_mapFieldIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(field)));return;
    }
    case CFieldExchange::BindFieldForUpdate:
        if(rs->m_nProxyFields) {
            auto* proxy=Proxy(rs,index);Traits::ToTimestamp(*value,proxy);
            fx->Default(name,proxy,length,SQL_C_TIMESTAMP,sizeof(TIMESTAMP_STRUCT),23);
        }
        return;
    case CFieldExchange::Fixup:
        if(*length==SQL_NULL_DATA) {rs->SetNullFieldStatus(index);Traits::SetPseudoNull(*value);}
        else if constexpr(Traits::ole)Traits::FromTimestamp(*Proxy(rs,index),*value);
        else {
            try {Traits::FromTimestamp(*Proxy(rs,index),*value);}
            catch(CException* error) {error->Delete();rs->SetNullFieldStatus(index);Traits::SetPseudoNull(*value);}
        }
        return;
    case CFieldExchange::MarkForAddNew:
        if(!Traits::IsPseudoNull(*value)) {rs->SetDirtyFieldStatus(index);rs->ClearNullFieldStatus(index);}return;
    case CFieldExchange::MarkForUpdate:
        if(!Traits::IsPseudoNull(*value))rs->ClearNullFieldStatus(index);
        break;
    case CFieldExchange::SetFieldNull:
        if((!fx->m_pvField && fx->m_nFieldType==0) || fx->m_pvField==value) {
            if(fx->m_bField) {rs->SetNullFieldStatus(index);Traits::SetPseudoNull(*value);*length=SQL_NULL_DATA;}
            else {rs->ClearNullFieldStatus(index);*length=sizeof(TIMESTAMP_STRUCT);}
        }
        return;
    case CFieldExchange::AllocCache: {
        void* cache=Allocate(Traits::bytes);std::memset(cache,0,Traits::initializedBytes);
        rs->m_rgFieldInfos[index].m_pvDataCache=cache;rs->m_rgFieldInfos[index].m_nDataType=Traits::cacheType;return;
    }
    case CFieldExchange::LoadField:
        impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(rs,field,value,length);
        if(!rs->IsFieldStatusNull(index))Traits::ToTimestamp(*value,Proxy(rs,index));
        return;
    case CFieldExchange::NameValue:
        if(rs->IsFieldStatusDirty(index)) {*fx->m_pstr+=name;*fx->m_pstr+=L"=";}
        [[fallthrough]];
    case CFieldExchange::Value: {
        if(!rs->IsFieldStatusDirty(index))return;
        auto* proxy=Proxy(rs,index);
        if(rs->IsFieldStatusNull(index))*length=SQL_NULL_DATA;
        else {Traits::ToTimestamp(*value,proxy);*length=sizeof(TIMESTAMP_STRUCT);}
        if(rs->m_dwOptions&CRecordset::optimizeBulkAdd)return;
        *fx->m_pstr+=L"?";*fx->m_pstr+=fx->m_lpszSeparator;++fx->m_nParamFields;
        // Native date exchange deliberately leaves this return code unchecked.
        ::SQLBindParameter(fx->m_hstmt,SQLUSMALLINT(fx->m_nParamFields),SQL_PARAM_INPUT,SQL_C_TIMESTAMP,
            rs->m_rgODBCFieldInfos[index].m_nSQLType,23,0,proxy,0,length);return;
    }
    default:break;
    }
    fx->Default(name,value,length,SQL_C_TIMESTAMP,Traits::bytes,23);
}
static_assert(sizeof(COleDateTime)==16,"native COleDateTime size");
}
// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAVCOleDateTime@ATL@@@Z
// mfc140u RVA 0x201e20 -- see RfxDateProxied and OleDateTimeTraits.
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCOleDateTime_ATL___Z(CFieldExchange* pFX, const wchar_t* szName, COleDateTime* value) {
    RfxDateProxied<OleDateTimeTraits>(pFX, szName, value);
}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAVCTime@ATL@@@Z
// mfc140u RVA 0x201140 -- see RfxDateProxied and CTimeTraits.
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCTime_ATL___Z(CFieldExchange* pFX, const wchar_t* szName, RetailCTime* value) {
    RfxDateProxied<CTimeTraits>(pFX, szName, value);
}

