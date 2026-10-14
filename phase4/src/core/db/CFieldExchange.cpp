// CFieldExchange — OpenMFC implementation.
// Sources: dbcore.cpp, dbrfx.cpp
//
// The RFX_* bodies and CFieldExchange::Default below are transcribed from the
// retail mfc140u.dll exports (ordinal -> RVA via the export directory; read
// with the workflow's annotated disassembler).  Every RVA quoted in this file
// is an mfc140u.dll function ENTRY unless it is explicitly called an
// instruction address.

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"
#include "openmfc/afxdao.h"       // CLongBinary: CObject vptr @0, m_hData @8, m_dwDataLength @0x10
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <memory>

// Layouts the bodies below depend on.  CLongBinary's member OFFSETS match
// retail (GetLongBinaryData at mfc140u RVA 0x2030a0 reads m_hData at +8 and
// m_dwDataLength at +0x10 through the CLongBinary& argument), but the WIDTH of
// m_dwDataLength does not: the real afxdb_.h declares it SQLULEN (8 bytes on
// x64) and every retail access is 64-bit (`mov %rcx,0x10(%rbx)` at 0x203248
// inside GetLongBinaryData; `mov 0x10(%rbp),%rax` at 0x202f4c and the 8-byte
// zero stores at 0x202eb7 / 0x202edd inside RFX_LongBinary (0x202bc0)), while
// openmfc/afxdao.h declares a DWORD.  A client compiled against the real
// header therefore owns an 8-byte field; LongBinaryLength() below reads and
// writes all 8 bytes so that (a) a real client's high dword is honoured and
// (b) a clear really clears it.  sizeof(CLongBinary) is 0x18 either way, so
// the access stays inside the object.  A header fix is requested (and the
// exported ctor in core/file/RuntimeClasses.cpp zeroes only the DWORD).
static_assert(offsetof(CLongBinary, m_hData) == 8, "CLongBinary::m_hData must sit at +8 (retail)");
static_assert(offsetof(CLongBinary, m_dwDataLength) == 0x10, "CLongBinary::m_dwDataLength must sit at +0x10 (retail)");
static_assert(sizeof(CLongBinary) >= 0x18, "CLongBinary must hold the 8-byte retail m_dwDataLength at +0x10");
static inline SQLULEN& LongBinaryLength(CLongBinary* lb) {
    return *reinterpret_cast<SQLULEN*>(reinterpret_cast<unsigned char*>(lb) + 0x10);
}
static_assert(sizeof(TIMESTAMP_STRUCT) == 16, "TIMESTAMP_STRUCT is bound with cbValue 16");
static_assert(offsetof(COleDateTime, m_status) == 8, "COleDateTime::m_status must follow m_dt (retail reads +8)");
static_assert(sizeof(CStringData) == 24, "ATL CStringData header");

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc);
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(short nRetCode, void* pdb, void* hstmt);
extern "C" long long* MS_ABI impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z(CRecordset* pThis, unsigned long field, int grow);
extern "C" void MS_ABI impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long field);
extern "C" void MS_ABI impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long field);
extern "C" void MS_ABI impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long field);
extern "C" int MS_ABI impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(const CRecordset* pThis, unsigned long field);
extern "C" int MS_ABI impl__IsFieldStatusNull_CRecordset__QEBAHK_Z(const CRecordset* pThis, unsigned long field);
extern "C" int MS_ABI impl__IsParamStatusNull_CRecordset__QEBAHK_Z(const CRecordset* pThis, unsigned long param);
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(void* rs, unsigned int nField, void* pvField);
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(void* rs, unsigned int nField, void* pvField, __int64* plLength);
extern "C" long long MS_ABI impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(CFieldExchange* pThis, int nField);
extern "C" unsigned char* MS_ABI impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(
    CFieldExchange*, CLongBinary* binary, long long newSize, long long oldSize);
extern "C" void MS_ABI impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(
    CFieldExchange* pThis, int field, CLongBinary* binary, long long* actualSize);
extern "C" void MS_ABI impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(
    CFieldExchange* pThis, const wchar_t* szName, void* pv, __int64* plLength,
    int nCFType, unsigned __int64 cbValue, unsigned __int64 cbPrecision);
extern "C" void MS_ABI impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(
    CFieldExchange* pFX, const wchar_t* szName, void* pv, __int64* plLength, int nCFType, unsigned __int64 cbValue);

// Symbol: ?GetLongBinarySize@CFieldExchange@@QEAA_JH@Z
// SQLLEN CFieldExchange::GetLongBinarySize(int nField)
// Transcribed from mfc140u RVA 0x203020:
//   do rc = SQLGetData(m_prs->m_hstmt, nField, SQL_C_DEFAULT, &dummy, 0, &length);
//   while (rc == SQL_STILL_EXECUTING);
//   if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) m_prs->ThrowDBException(rc, SQL_NULL_HSTMT);
//   return length;
// The indicator is returned as ODBC wrote it, so a NULL column yields
// SQL_NULL_DATA (-1); RFX_LongBinary's Fixup relies on that.  (A previous
// version of this thunk folded NULL and errors into 0.)  Deviation: the
// retail SQLGetData target is ODBC32 ordinal 43 through the delay-load slot at
// 0x3e9120; the vtable call m_prs->ThrowDBException (slot 24) is routed through
// the AfxThrowDBException thunk because this CRecordset's vtable is not retail's.
extern "C" long long MS_ABI impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(CFieldExchange* pThis, int nField) {
    if (!pThis || !pThis->m_pRecordset) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
    CRecordset* prs = pThis->m_pRecordset;
    SQLLEN length = 0;
    unsigned char dummy[8] = {};
    RETCODE rc;
    do {
        rc = SQLGetData(prs->m_hstmt, static_cast<SQLUSMALLINT>(nField), SQL_C_DEFAULT, dummy, 0, &length);
    } while (rc == SQL_STILL_EXECUTING);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(rc, prs->m_pDatabase, prs->m_hstmt);
    }
    return static_cast<long long>(length);
}
// Symbol: ?ReallocLongBinary@CFieldExchange@@QEAAPEAEAEAVCLongBinary@@_J1@Z
// The argument is the client's CLongBinary (retail body at mfc140u RVA 0x2032f0
// reads and writes only m_hData at +8 -- it never touches m_dwDataLength; the
// caller GetLongBinaryData stores the length), not the header-less
// LongBinaryCompat view.  The GlobalAlloc/copy/GlobalFree body below is a
// pre-existing simplification of retail's GlobalAlloc-or-GlobalReAlloc; it is
// not re-transcribed here.
extern "C" unsigned char* MS_ABI impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(
    CFieldExchange*, CLongBinary* binary, long long newSize, long long oldSize) {
    if (!binary || newSize < 0) return nullptr;
    HGLOBAL newHandle = ::GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(newSize));
    if (!newHandle && newSize != 0) return nullptr;
    unsigned char* newData = static_cast<unsigned char*>(newHandle ? ::GlobalLock(newHandle) : nullptr);
    if (binary->m_hData && newData) {
        void* oldData = ::GlobalLock(binary->m_hData);
        if (oldData) {
            std::memcpy(newData, oldData, static_cast<size_t>(std::min<long long>(oldSize, newSize)));
            ::GlobalUnlock(binary->m_hData);
        }
        ::GlobalFree(binary->m_hData);
    }
    binary->m_hData = newHandle;
    return newData;
}
// Symbol: ?GetLongBinaryData@CFieldExchange@@QEAAXHAEAVCLongBinary@@PEA_J@Z
// Argument type corrected to the client's CLongBinary (retail body at mfc140u
// RVA 0x2030a0: m_dwDataLength at +0x10 as an 8-byte SQLULEN, m_hData at +8;
// retail stores the fetched length into m_dwDataLength itself, at 0x203248 /
// 0x203255).  The single 4096-byte SQLGetData below is a pre-existing
// simplification of retail's chunked fetch (m_lDefaultLBFetchSize /
// GetLBFetchSize / GetLBReallocSize with the SQLSTATE 01004 truncation loop)
// and is not re-transcribed here.
extern "C" void MS_ABI impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(
    CFieldExchange* pThis, int field, CLongBinary* binary, long long* actualSize) {
    if (actualSize) *actualSize = 0;
    if (!pThis || !pThis->m_pRecordset || !binary || pThis->m_pRecordset->m_hstmt == SQL_NULL_HSTMT) return;
    SQLLEN indicator = 0;
    std::vector<unsigned char> buffer(4096);
    RETCODE rc = SQLGetData(pThis->m_pRecordset->m_hstmt, static_cast<SQLUSMALLINT>(field), SQL_C_BINARY,
                            buffer.data(), static_cast<SQLLEN>(buffer.size()), &indicator);
    if (!SqlSucceeded(rc) || indicator <= 0) return;
    unsigned char* dest = impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(pThis, binary, indicator, 0);
    if (!dest) return;
    std::memcpy(dest, buffer.data(), static_cast<size_t>(std::min<SQLLEN>(indicator, buffer.size())));
    ::GlobalUnlock(binary->m_hData);
    LongBinaryLength(binary) = static_cast<SQLULEN>(indicator);
    if (actualSize) *actualSize = indicator;
}
SQLLEN CFieldExchange::GetLongBinarySize(int field) {
    return impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(this,field);
}
void CFieldExchange::GetLongBinaryData(int field,CLongBinary& binary,SQLLEN* size) {
    impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(this,field,&binary,size);
}
BYTE* CFieldExchange::ReallocLongBinary(CLongBinary& binary,SQLLEN required,SQLLEN reallocation) {
    return impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(this,&binary,required,reallocation);
}
void CFieldExchange::SetNull(const wchar_t* lpszName) {
    if (!m_pRecordset || !lpszName) return;
    LoadRecordsetFields(m_pRecordset);
    int index = FindFieldByName(m_pRecordset, lpszName);
    if (index < 0) return;
    EnsureRecordsetState(m_pRecordset).nullFields.insert(static_cast<unsigned long>(index));
}

// =============================================================================
// Record field exchange (RFX).
//
// Retail CFieldExchange layout (ctor at mfc140u RVA 0x1fe690, IsFieldType at
// 0x1fe6e0, Default at 0x1fe750; the ctor ENSUREs pRecordset && pRecordset->m_hstmt):
//   +0x00 UINT       m_nOperation      +0x08 CRecordset* m_prs
//   +0x10 UINT       m_nFieldType (ctor stores -1)
//   +0x14 UINT       m_nFieldFound (not written by the ctor)
//   +0x18 CString*   m_pstr            +0x20 BOOL        m_bField
//   +0x28 void*      m_pvField (the ctor's 4th argument)
//   +0x30 LPCTSTR    m_lpszSeparator   +0x38 UINT        m_nFields
//   +0x3c UINT       m_nParams         +0x40 UINT        m_nParamFields
//   +0x48 HSTMT      m_hstmt
//   +0x50/+0x58 SQLLEN m_lDefaultLB{Fetch,Realloc}Size (8-byte stores, both 0x10000)
//   (no vptr; sizeof 0x60 by the last member)
// The public declaration now reproduces this 96-byte layout and the native
// operation numbering. CRecordset's backing storage remains incomplete as
// described below; RFX behavior still needs the remaining recordset work.
//
// Retail CRecordset members these bodies read (retail ctor at RVA 0x1f7e40;
// offsets read off the bodies, names from the SDK afxdb.h declaration order,
// which reproduces every read offset exactly):
//   +0x08 m_hstmt, +0x10 m_pDatabase, +0x28 m_nFields, +0x2c m_nParams,
//   +0x30 m_bCheckCacheForDirtyFields, +0x34 m_bRebindParams and
//   +0x38 m_bLongBinaryColumns (the two BOOLs the RFX bodies set to 1),
//   +0x3c m_bUseUpdateSQL, +0x40 m_dwOptions (Default's Value tests bit 0x4000,
//   optimizeBulkAdd), +0x50 m_rgODBCFieldInfos (32-byte CODBCFieldInfo:
//   +8 m_nSQLType, +0x10 m_nPrecision, +0x18 m_nScale), +0x58 m_rgFieldInfos
//   (24-byte CFieldInfo: +0 m_pvDataCache, +8 m_nLength -- the SQLLEN that
//   GetFieldLengthBuffer (0x1fc950) hands out for a column --, +0x10 m_nDataType,
//   +0x14 m_bStatus), +0x60 m_mapFieldIndex, +0x98 m_mapParamIndex,
//   +0xd0 m_pvFieldProxy, +0xd8 m_pvParamProxy, +0xe0/+0xe4 m_nProxyFields/Params,
//   +0x144 m_bUpdatable, +0x148 m_bAppendable, +0x178 m_pbFieldFlags,
//   +0x180 m_pbParamFlags, +0x188 m_plParamLength (GetFieldLengthBuffer's
//   parameter array), +0x194 m_dwRowsetSize (ctor stores 25).
// Native recordset arrays and one-based binding maps are now represented
// directly. RFX-specific proxy lifetime, metadata loading, cache comparison,
// and SQL-building operations still require their remaining implementation.
// Status and length helpers enforce the native field/parameter bounds.
// =============================================================================

namespace {

// Retail RFX_Operation values (see the header comment above).
namespace rfxop {
enum : unsigned {
    BindParam = 0, RebindParam = 1, BindFieldToColumn = 2, BindFieldForUpdate = 3,
    UnbindFieldForUpdate = 4, Fixup = 5, MarkForAddNew = 6, MarkForUpdate = 7,
    Name = 8, NameValue = 9, Value = 10, SetFieldNull = 11, StoreField = 12,
    LoadField = 13, AllocCache = 14, AllocMultiRowBuffer = 15,
    DeleteMultiRowBuffer = 16
};
}

inline unsigned RfxOp(const CFieldExchange* pFX) { return static_cast<unsigned>(pFX->m_nOperation); }

inline bool IsParamOperation(const CFieldExchange* pFX) {
    return pFX->m_nFieldType != CFieldExchange::outputColumn;
}

inline HSTMT RecordsetHstmt(const CFieldExchange* pFX) {
    return pFX->m_pRecordset ? pFX->m_pRecordset->m_hstmt : SQL_NULL_HSTMT;
}
inline void* RecordsetDatabase(const CFieldExchange* pFX) {
    return pFX->m_pRecordset ? static_cast<void*>(pFX->m_pRecordset->m_pDatabase) : nullptr;
}

// Retail: m_prs->ThrowDBException(rc, hstmt) -- CRecordset vtable slot 24, body
// at mfc140u RVA 0x1fdff0: `if (!hstmt) hstmt = m_hstmt; AfxThrowDBException(rc, m_pDatabase, hstmt)`.
void ThrowDBException(const CFieldExchange* fx,RETCODE code,HSTMT statement) {
    fx->m_prs->ThrowDBException(code,statement);
}

SQLLEN* LengthBuffer(CFieldExchange* fx,UINT field) {
    return reinterpret_cast<SQLLEN*>(impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z(fx->m_prs,field-1,fx->m_nFieldType));
}

// Retail m_pvFieldProxy / m_pvParamProxy: one heap TIMESTAMP_STRUCT per
// CTime/COleDateTime column or parameter, allocated on first bind
// (operator new(16), not zeroed) and bound in the value's place.  CRecordset
// here has no such members, so they live in a file-local table (never freed;
// retail frees them in the CRecordset dtor).  Zero-initialised here.
struct DateProxies {
    std::vector<std::unique_ptr<TIMESTAMP_STRUCT>> fields;
    std::vector<std::unique_ptr<TIMESTAMP_STRUCT>> params;
};
std::unordered_map<const CRecordset*, DateProxies>& ProxyTable() {
    static std::unordered_map<const CRecordset*, DateProxies> table;
    return table;
}
TIMESTAMP_STRUCT* DateProxy(const CRecordset* prs, UINT nField, bool param, bool create) {
    if (!prs || nField == 0) return nullptr;
    DateProxies& p = ProxyTable()[prs];
    auto& slots = param ? p.params : p.fields;
    const size_t n = nField - 1;
    if (slots.size() <= n) {
        if (!create) return nullptr;
        slots.resize(n + 1);
    }
    if (!slots[n]) {
        if (!create) return nullptr;
        slots[n].reset(new TIMESTAMP_STRUCT());
        std::memset(slots[n].get(), 0, sizeof(TIMESTAMP_STRUCT));
    }
    return slots[n].get();
}

// SQL_LEN_DATA_AT_EXEC(len) with the retail clamp: len is limited to
// INT_MAX - |SQL_LEN_DATA_AT_EXEC_OFFSET| first (RFX_LongBinary (0x202bc0), the
// block at 0x202f32..0x202f68).
SQLLEN LongBinaryAtExecLength(SQLULEN dwDataLength) {
    // retail: `cmp %rax,%rcx; jbe` -- an unsigned 64-bit compare of m_dwDataLength against the clamp
    const SQLULEN maxLen = static_cast<SQLULEN>(0x7fffffff) - static_cast<SQLULEN>(std::labs(SQL_LEN_DATA_AT_EXEC_OFFSET));
    const SQLLEN len = static_cast<SQLLEN>(dwDataLength > maxLen ? maxLen : dwDataLength);
    return SQL_LEN_DATA_AT_EXEC(len);
}

// ATL::CTime is { __time64_t m_time }; this codebase declares no CTime, so the
// RFX_Date(CTime&) export sees the object through this view.
struct RetailCTime { __int64 m_time; };
static_assert(sizeof(RetailCTime) == 8, "ATL::CTime is a single __time64_t");

// value -> TIMESTAMP_STRUCT as retail does it (RFX_Date(CTime&) (0x201140), the
// block at 0x2012e9..0x2013ad): six CTime::Get{Year,Month,Day,Hour,Minute,Second}()
// calls, each an _localtime64_s; on failure Year/Month/Day read 0 and
// Hour/Minute/Second -1 (ATL's inline getters); fraction is 0.
void CTimeToTimestamp(const RetailCTime& value, TIMESTAMP_STRUCT* ts) {
    struct tm t;
    const bool ok = _localtime64_s(&t, &value.m_time) == 0;
    ts->year = static_cast<SQLSMALLINT>(ok ? t.tm_year + 1900 : 0);
    ts->month = static_cast<SQLUSMALLINT>(ok ? t.tm_mon + 1 : 0);
    ts->day = static_cast<SQLUSMALLINT>(ok ? t.tm_mday : 0);
    ts->hour = static_cast<SQLUSMALLINT>(ok ? t.tm_hour : -1);
    ts->minute = static_cast<SQLUSMALLINT>(ok ? t.tm_min : -1);
    ts->second = static_cast<SQLUSMALLINT>(ok ? t.tm_sec : -1);
    ts->fraction = 0;
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
void OleDateTimeToTimestamp(const COleDateTime& value, TIMESTAMP_STRUCT* ts) {
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    const bool ok = value.m_status == COleDateTime::DT_VALID && ::VariantTimeToSystemTime(value.m_dt, &st) != 0;
    ts->year = static_cast<SQLSMALLINT>(ok ? st.wYear : -1);
    ts->month = static_cast<SQLUSMALLINT>(ok ? st.wMonth : -1);
    ts->day = static_cast<SQLUSMALLINT>(ok ? st.wDay : -1);
    ts->hour = static_cast<SQLUSMALLINT>(ok ? st.wHour : -1);
    ts->minute = static_cast<SQLUSMALLINT>(ok ? st.wMinute : -1);
    ts->second = static_cast<SQLUSMALLINT>(ok ? st.wSecond : -1);
    ts->fraction = 0;
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
    static bool IsPseudoNull(const RetailCTime& v) { return v.m_time == 0; }
    static void SetPseudoNull(RetailCTime& v) { v.m_time = 0; }               // 0x201289: movq $0,(%r14)
    static void ToTimestamp(const RetailCTime& v, TIMESTAMP_STRUCT* ts) { CTimeToTimestamp(v, ts); }
    static void FromTimestamp(const TIMESTAMP_STRUCT& ts, RetailCTime& v) { TimestampToCTime(ts, v); }
};
struct OleDateTimeTraits {
    typedef COleDateTime value_type;
    static bool IsPseudoNull(const COleDateTime& v) { return v.m_status == COleDateTime::DT_NULL; }
    static void SetPseudoNull(COleDateTime& v) { v.m_status = COleDateTime::DT_NULL; }  // 0x201f7a: movl $2,0x8(%rsi)
    static void ToTimestamp(const COleDateTime& v, TIMESTAMP_STRUCT* ts) { OleDateTimeToTimestamp(v, ts); }
    static void FromTimestamp(const TIMESTAMP_STRUCT& ts, COleDateTime& v) { TimestampToOleDateTime(ts, v); }
};

template <typename Traits>
void RfxDateProxied(CFieldExchange* pFX, const wchar_t* szName, typename Traits::value_type& value) {
    if (!pFX || !szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    if (!pFX->m_pRecordset) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    SQLLEN* plLength = LengthBuffer(pFX, nField);
    if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    CRecordset* prs = pFX->m_pRecordset;
    const unsigned long n = nField - 1;
    switch (RfxOp(pFX)) {
    case rfxop::BindParam: {
        // CTime body 0x2015be: m_prs->[+0x34] (m_bRebindParams) = TRUE; allocate m_pvParamProxy[] / [n] on demand;
        // if (paramnull) *plLength = SQL_NULL_DATA; else { convert value -> proxy; *plLength = 16; }
        // rc = SQLBindParameter(m_hstmt, nField, m_nFieldType, SQL_C_TIMESTAMP, SQL_TIMESTAMP, 23, 0, proxy, 0, plLength);
        // if (rc != SQL_SUCCESS) ThrowDBException(rc, m_hstmt); m_mapParamIndex[&value] = nField;
        // (m_bRebindParams has no counterpart in this CRecordset and is dropped.)
        TIMESTAMP_STRUCT* proxy = DateProxy(prs, nField, true, true);
        if (impl__IsParamStatusNull_CRecordset__QEBAHK_Z(prs, n)) {
            *plLength = SQL_NULL_DATA;
        } else {
            Traits::ToTimestamp(value, proxy);
            *plLength = sizeof(TIMESTAMP_STRUCT);
        }
        // Deviation: SQL_PARAM_INPUT for the absent m_nFieldType, the recordset's m_hstmt for the absent fx.m_hstmt.
        RETCODE rc = ::SQLBindParameter(RecordsetHstmt(pFX), static_cast<SQLUSMALLINT>(nField), SQL_PARAM_INPUT,
                                        SQL_C_TIMESTAMP, SQL_TIMESTAMP, 23, 0, proxy, 0, plLength);
        if (rc != SQL_SUCCESS) ThrowDBException(pFX, rc, RecordsetHstmt(pFX));
        prs->m_mapParamIndex.SetAt(&value,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    }
    case rfxop::RebindParam: {
        // 0x2014ca: if (paramnull) *plLength = SQL_NULL_DATA; else { convert value -> m_pvParamProxy[n]; *plLength = 16; }
        if (impl__IsParamStatusNull_CRecordset__QEBAHK_Z(prs, n)) {
            *plLength = SQL_NULL_DATA;
        } else {
            TIMESTAMP_STRUCT* proxy = DateProxy(prs, nField, true, true);
            Traits::ToTimestamp(value, proxy);
            *plLength = sizeof(TIMESTAMP_STRUCT);
        }
        return;
    }
    case rfxop::BindFieldToColumn: {
        // 0x2013cb: allocate m_pvFieldProxy[] (m_nProxyFields = m_nFields) and [n] on demand;
        // rc = SQLBindCol(m_prs->m_hstmt, nField, SQL_C_TIMESTAMP, proxy, 16, plLength);
        // if (!Check(rc)) ThrowDBException(rc, SQL_NULL_HSTMT); m_mapFieldIndex[&value] = nField;
        TIMESTAMP_STRUCT* proxy = DateProxy(prs, nField, false, true);
        RETCODE rc = ::SQLBindCol(RecordsetHstmt(pFX), static_cast<SQLUSMALLINT>(nField), SQL_C_TIMESTAMP, proxy,
                                  sizeof(TIMESTAMP_STRUCT), plLength);
        if (!prs->Check(rc)) ThrowDBException(pFX, rc, SQL_NULL_HSTMT);
        prs->m_mapFieldIndex.SetAt(&value,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    }
    case rfxop::BindFieldForUpdate: {
        // 0x2012d9: if (m_nProxyFields == 0) return; convert value -> m_pvFieldProxy[n];
        // Default(szName, proxy, plLength, SQL_C_TIMESTAMP, 16, 23)
        TIMESTAMP_STRUCT* proxy = DateProxy(prs, nField, false, false);
        if (!proxy) return;
        Traits::ToTimestamp(value, proxy);
        impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(pFX, szName, proxy, plLength, SQL_C_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), 23);
        return;
    }
    case rfxop::Fixup: {
        // 0x201268: if (*plLength == SQL_NULL_DATA) { SetNullFieldStatus(n); value = PSEUDO_NULL; }
        // else value = <from *m_pvFieldProxy[n]>;
        if (*plLength == SQL_NULL_DATA) {
            impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            Traits::SetPseudoNull(value);
        } else {
            TIMESTAMP_STRUCT* proxy = DateProxy(prs, nField, false, false);
            if (proxy) Traits::FromTimestamp(*proxy, value);
        }
        return;
    }
    case rfxop::MarkForAddNew:
        // 0x201211: if (value != PSEUDO_NULL) { SetDirtyFieldStatus(n); ClearNullFieldStatus(n); }
        if (!Traits::IsPseudoNull(value)) {
            impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        }
        return;
    case rfxop::MarkForUpdate:
        // 0x2017d0: if (value != PSEUDO_NULL) ClearNullFieldStatus(n); then the default call
        if (!Traits::IsPseudoNull(value)) impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        break;
    case rfxop::LoadField:
        // 0x20187a: m_pbFieldFlags[n] |= m_bStatus; if null -> *plLength = SQL_NULL_DATA; else
        // AfxCopyValueByRef(cache, &value, plLength, type) and refresh the field proxy from value.
        // Retail does this inline; only the null half exists here (AfxLoadField thunk).
        impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(prs, nField, &value, plLength);
        return;
    case rfxop::NameValue:      // 0x2019ea / 0x201a2f: need m_pstr / m_lpszSeparator / m_nParamFields / fx.m_hstmt -- not modelled
    case rfxop::Value:
    case rfxop::SetFieldNull:   // 0x2019a0: needs m_pvField / m_bField -- not modelled
    case rfxop::AllocCache:     // 0x201844: m_pvDataCache = new (zeroed) 8 bytes, m_nDataType = AFX_RFX_DATE (8) -- no cache here
        return;
    default:
        break;
    }
    // 0x201814: Default(szName, &value, plLength, SQL_C_TIMESTAMP, 8, 23) -- Name and StoreField
    // reach this call too (Default handles both).
    impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(pFX, szName, &value, plLength, SQL_C_TIMESTAMP, 8, 23);
}

// ---------------------------------------------------------------------------
// Bulk RFX bodies.  RFX_Long_Bulk / RFX_Int_Bulk / RFX_Bool_Bulk are literally
// one folded function in retail (all three ordinals resolve to RVA 0x203a70);
// the others differ only in the element size, C type and the extra nMaxLength
// factor for text/binary.  Shape (RFX_Long_Bulk, 0x203a70):
//   if (!pFX || !szName) AfxThrowInvalidArgException();
//   UINT nField; if (!pFX->IsFieldType(&nField)) return;      (RFX_Text_Bulk nulls both out-pointers first)
//   if (!prgVals || !prgLengths) AfxThrowInvalidArgException();
//   switch (m_nOperation) {
//   case AllocMultiRowBuffer:  *prgVals = new T[m_prs->m_nRowsetSize (* nMaxLength)]; *prgLengths = new SQLLEN[m_nRowsetSize]; return;
//   case DeleteMultiRowBuffer: delete[] *prgVals; *prgVals = NULL; delete[] *prgLengths; *prgLengths = NULL; return;
//   default: AfxRFXBulkDefault(pFX, szName, *prgVals, *prgLengths, nCFType, cbValue); return; }
// RFX_Single_Bulk / RFX_Double_Bulk (0x1fe3b0 / 0x1fe4a0) validate all four
// pointers before IsFieldType instead of after.  Retail's allocation is
// `new T[n]` of a trivial T -- operator new (malloc-backed in the MSVC CRT,
// no array cookie) with the compiler's multiply-overflow guard (size -> -1,
// which operator new rejects) -- and the delete is the CRT `free` import.
// malloc/free with an explicit overflow check and AfxThrowMemoryException on
// failure is the equivalent pair here.  m_nRowsetSize (retail +0x194, read as
// a signed int) is this CRecordset's m_dwRowsetSize.
// ---------------------------------------------------------------------------
void* BulkAlloc(size_t nCount, size_t nElemSize) {
    if (nElemSize != 0 && nCount > (~static_cast<size_t>(0)) / nElemSize) { impl__AfxThrowMemoryException__YAXXZ(); return nullptr; }
    void* p = std::malloc(nCount * nElemSize);
    if (!p) { impl__AfxThrowMemoryException__YAXXZ(); return nullptr; }
    return p;
}
template <typename T>
void RfxBulk(CFieldExchange* pFX, const wchar_t* szName, T** prgVals, __int64** prgLengths,
             int nCFType, unsigned __int64 cbValue, size_t nElemFactor, bool checkPointersFirst, bool nullOnNotFieldType) {
    if (!pFX || !szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    if (checkPointersFirst && (!prgVals || !prgLengths)) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) {
        if (nullOnNotFieldType) {
            // RFX_Text_Bulk (the store at 0x2038a5 inside 0x203870) writes through both pointers here
            // without checking them.
            *prgVals = nullptr;
            *prgLengths = nullptr;
        }
        return;
    }
    if (!prgVals || !prgLengths) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    switch (RfxOp(pFX)) {
    case rfxop::AllocMultiRowBuffer: {
        const size_t nRowsetSize = pFX->m_pRecordset ? static_cast<size_t>(static_cast<int>(pFX->m_pRecordset->m_dwRowsetSize)) : 0;
        *prgVals = static_cast<T*>(BulkAlloc(nRowsetSize * nElemFactor, sizeof(T)));
        *prgLengths = static_cast<__int64*>(BulkAlloc(nRowsetSize, sizeof(__int64)));
        return;
    }
    case rfxop::DeleteMultiRowBuffer:
        std::free(*prgVals);
        *prgVals = nullptr;
        std::free(*prgLengths);
        *prgLengths = nullptr;
        return;
    default:
        impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(pFX, szName, *prgVals, *prgLengths, nCFType, cbValue);
        return;
    }
}

} // namespace

// Symbol: ?AfxRFXBulkDefault@@YAXPEAVCFieldExchange@@PEB_WPEAXPEA_JH_K@Z
// void AfxRFXBulkDefault(CFieldExchange* pFX, LPCTSTR szName, void* pv, SQLLEN* plLength, int nCFType, SQLULEN cbValue)
// Transcribed from mfc140u RVA 0x203e20:
//   if (!pFX) AfxThrowInvalidArgException();
//   if (m_nOperation == BindFieldToColumn) { rc = SQLBindCol(m_prs->m_hstmt, m_nFields, nCFType, pv, cbValue, plLength);
//       if (!m_prs->Check(rc)) m_prs->ThrowDBException(rc, SQL_NULL_HSTMT); }
//   else if (m_nOperation == Name) { if (!szName) AfxThrowInvalidArgException(); *m_pstr += szName; *m_pstr += m_lpszSeparator; }
// Note the column number is m_nFields itself (the caller's IsFieldType already
// counted this column).  The Name branch needs m_pstr / m_lpszSeparator, which
// this CFieldExchange lacks: only its argument check is kept.
extern "C" void MS_ABI impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(
    CFieldExchange* pFX, const wchar_t* szName, void* pv, __int64* plLength, int nCFType, unsigned __int64 cbValue) {
    if (!pFX) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    if (RfxOp(pFX) == rfxop::BindFieldToColumn) {
        if (!pFX->m_pRecordset) return;
        RETCODE rc = ::SQLBindCol(pFX->m_pRecordset->m_hstmt, static_cast<SQLUSMALLINT>(pFX->m_nFields),
                                  static_cast<SQLSMALLINT>(nCFType), pv, static_cast<SQLLEN>(cbValue), plLength);
        if (!pFX->m_pRecordset->Check(rc)) ThrowDBException(pFX, rc, SQL_NULL_HSTMT);
    } else if (RfxOp(pFX) == rfxop::Name) {
        if (!szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        // *m_pstr += szName; *m_pstr += m_lpszSeparator;  -- no m_pstr here
    }
}

// Symbol: ?RFX_Binary_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAEPEAPEA_JH@Z
// mfc140u RVA 0x203d30: alloc new BYTE[m_nRowsetSize * nMaxLength]; default AfxRFXBulkDefault(..., SQL_C_BINARY (-2), nMaxLength).
extern "C" void MS_ABI impl__RFX_Binary_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_JH_Z(CFieldExchange* pFX, const wchar_t* szName, unsigned char** prgByteVals, __int64** prgLengths, int nMaxLength) {
    RfxBulk<unsigned char>(pFX, szName, prgByteVals, prgLengths, SQL_C_BINARY, static_cast<unsigned __int64>(nMaxLength),
                           static_cast<size_t>(nMaxLength), false, false);
}

// Symbol: ?RFX_Bool_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAHPEAPEA_J@Z
// mfc140u RVA 0x203a70 (folded with RFX_Long_Bulk / RFX_Int_Bulk): new long[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_LONG (4), 4).
extern "C" void MS_ABI impl__RFX_Bool_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, int** prgBoolVals, __int64** prgLengths) {
    RfxBulk<int>(pFX, szName, prgBoolVals, prgLengths, SQL_C_LONG, 4, 1, false, false);
}

// Symbol: ?RFX_Byte_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAEPEAPEA_J@Z
// mfc140u RVA 0x203c50: new BYTE[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_TINYINT (-6), 1).
extern "C" void MS_ABI impl__RFX_Byte_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, unsigned char** prgByteVals, __int64** prgLengths) {
    RfxBulk<unsigned char>(pFX, szName, prgByteVals, prgLengths, SQL_C_TINYINT, 1, 1, false, false);
}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAVCOleDateTime@ATL@@@Z
// mfc140u RVA 0x201e20 -- see RfxDateProxied and OleDateTimeTraits.
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCOleDateTime_ATL___Z(CFieldExchange* pFX, const wchar_t* szName, COleDateTime* value) {
    if (!value) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    RfxDateProxied<OleDateTimeTraits>(pFX, szName, *value);
}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAVCTime@ATL@@@Z
// mfc140u RVA 0x201140 -- see RfxDateProxied and CTimeTraits.
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCTime_ATL___Z(CFieldExchange* pFX, const wchar_t* szName, RetailCTime* value) {
    if (!value) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    RfxDateProxied<CTimeTraits>(pFX, szName, *value);
}

// Symbol: ?RFX_Date_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAUtagTIMESTAMP_STRUCT@@PEAPEA_J@Z
// mfc140u RVA 0x203b60: new TIMESTAMP_STRUCT[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_TIMESTAMP (11), 16).
extern "C" void MS_ABI impl__RFX_Date_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAUtagTIMESTAMP_STRUCT__PEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, TIMESTAMP_STRUCT** prgTSVals, __int64** prgLengths) {
    RfxBulk<TIMESTAMP_STRUCT>(pFX, szName, prgTSVals, prgLengths, SQL_C_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), 1, false, false);
}

// Symbol: ?RFX_Double_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEANPEAPEA_J@Z
// mfc140u RVA 0x1fe4a0: all four pointers ENSUREd first; new double[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_DOUBLE (8), 8).
extern "C" void MS_ABI impl__RFX_Double_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEANPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, double** prgDblVals, __int64** prgLengths) {
    RfxBulk<double>(pFX, szName, prgDblVals, prgLengths, SQL_C_DOUBLE, 8, 1, true, false);
}

// Symbol: ?RFX_Int_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAHPEAPEA_J@Z
// mfc140u RVA 0x203a70 (folded with RFX_Long_Bulk / RFX_Bool_Bulk): new long[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_LONG (4), 4).
extern "C" void MS_ABI impl__RFX_Int_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, int** prgIntVals, __int64** prgLengths) {
    RfxBulk<int>(pFX, szName, prgIntVals, prgLengths, SQL_C_LONG, 4, 1, false, false);
}

// Symbol: ?RFX_LongBinary@@YAXPEAVCFieldExchange@@PEB_WAEAVCLongBinary@@@Z
// void RFX_LongBinary(CFieldExchange* pFX, LPCTSTR szName, CLongBinary& value)
// Transcribed from mfc140u RVA 0x202bc0:
//   if (!pFX || !szName) AfxThrowInvalidArgException(); IsFieldType; plLength = GetFieldLengthBuffer(n, m_nFieldType);
//   BindFieldToColumn (0x202f79): if (!m_prs->m_bUseUpdateSQL (+0x3c) && (m_prs->m_bUpdatable (+0x144) || m_prs->m_bAppendable (+0x148)))
//       { rc = SQLBindCol(m_prs->m_hstmt, nField, SQL_C_DEFAULT, &value, 0, plLength); if (!Check(rc)) ThrowDBException(rc, 0); }
//       m_mapFieldIndex[&value] = nField;
//   BindFieldForUpdate (0x202f0a): *plLength = !dirty ? SQL_IGNORE : null ? SQL_NULL_DATA : SQL_LEN_DATA_AT_EXEC(clamped m_dwDataLength);
//   UnbindFieldForUpdate (0x202efe): *plLength = value.m_dwDataLength;  (8-byte load -- see LongBinaryLength)
//   Fixup (0x202e88): *plLength = GetLongBinarySize(nField); if (*plLength != SQL_NULL_DATA && *plLength != 0)
//       GetLongBinaryData(nField, value, plLength); if (*plLength == SQL_NULL_DATA) { value.m_dwDataLength = 0; SetNull(n); }
//       else { if (*plLength == 0) value.m_dwDataLength = 0; ClearNull(n); }
//   Name (0x202e58): m_prs->[+0x38] (m_bLongBinaryColumns) = TRUE; Default(szName, &value, plLength, SQL_C_DEFAULT (99), 0, 0);
//   NameValue / Value (0x202ce2 / 0x202d25): dirty-name and SQL_LEN_DATA_AT_EXEC parameter binding through m_pstr;
//   SetFieldNull (0x202c68): as the scalars, with m_dwDataLength = 0 / SQL_LEN_DATA_AT_EXEC(clamped);
//   every other operation (BindParam, RebindParam, MarkForAddNew, MarkForUpdate, StoreField, LoadField, AllocCache): return.
// The three CRecordset BOOLs gating the BindFieldToColumn SQLBindCol are
// m_bUseUpdateSQL (+0x3c), m_bUpdatable (+0x144) and m_bAppendable (+0x148):
// the SDK afxdb.h declaration order laid over the retail-verified anchors
// (m_nFields +0x28, m_pbFieldFlags +0x178 -- see the header comment); no
// export pins them because CanUpdate / CanAppend are inline in real MFC.  That
// zero-length SQL_C_DEFAULT bind exists only so that SQLSetPos-based updates
// (m_bUseUpdateSQL == FALSE) can carry the column; this CRecordset has none of
// the three members and its Update() always issues SQL statements (the
// m_bUseUpdateSQL == TRUE branch), so the bind is skipped and only the
// field-index map entry is made.  Name / NameValue / Value / SetFieldNull are
// not modelled (m_pstr, m_bLongBinaryColumns, m_pvField / m_bField are
// absent).  Deviation: the Fixup path calls this file's GetLongBinaryData,
// whose single-read body is a simplification of retail's (see its comment).
extern "C" void MS_ABI impl__RFX_LongBinary__YAXPEAVCFieldExchange__PEB_WAEAVCLongBinary___Z(CFieldExchange* pFX, const wchar_t* szName, CLongBinary* value) {
    if (!pFX || !szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    if (!value) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    SQLLEN* plLength = LengthBuffer(pFX, nField);
    CRecordset* prs = pFX->m_pRecordset;
    const unsigned long n = nField - 1;
    switch (RfxOp(pFX)) {
    case rfxop::BindFieldToColumn:
        if (prs) prs->m_mapFieldIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    case rfxop::BindFieldForUpdate:
        if (!plLength) return;
        if (!impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(prs, n)) *plLength = SQL_IGNORE;
        else if (impl__IsFieldStatusNull_CRecordset__QEBAHK_Z(prs, n)) *plLength = SQL_NULL_DATA;
        else *plLength = LongBinaryAtExecLength(LongBinaryLength(value));
        return;
    case rfxop::UnbindFieldForUpdate:
        if (plLength) *plLength = static_cast<SQLLEN>(LongBinaryLength(value));
        return;
    case rfxop::Fixup: {
        if (!plLength || !prs) return;
        *plLength = impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(pFX, static_cast<int>(nField));
        if (*plLength != SQL_NULL_DATA && *plLength != 0) {
            impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(pFX, static_cast<int>(nField), value, plLength);
        }
        if (*plLength == SQL_NULL_DATA) {
            LongBinaryLength(value) = 0;
            impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        } else {
            if (*plLength == 0) LongBinaryLength(value) = 0;
            impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        }
        return;
    }
    default:
        return;
    }
}

// Symbol: ?RFX_Long_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAJPEAPEA_J@Z
// mfc140u RVA 0x203a70 (the reference bulk body quoted above RfxBulk): new long[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_LONG (4), 4).
extern "C" void MS_ABI impl__RFX_Long_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAJPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, long** prgLongVals, __int64** prgLengths) {
    RfxBulk<long>(pFX, szName, prgLongVals, prgLengths, SQL_C_LONG, 4, 1, false, false);
}

// Symbol: ?RFX_Single_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAMPEAPEA_J@Z
// mfc140u RVA 0x1fe3b0: all four pointers ENSUREd first; new float[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_FLOAT (7), 4).
extern "C" void MS_ABI impl__RFX_Single_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAMPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, float** prgFltVals, __int64** prgLengths) {
    RfxBulk<float>(pFX, szName, prgFltVals, prgLengths, SQL_C_FLOAT, 4, 1, true, false);
}

// Symbol: ?RFX_Text_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEADPEAPEA_JH@Z
// mfc140u RVA 0x203970: IsFieldType false -> both out-pointers nulled; new char[m_nRowsetSize * nMaxLength];
// AfxRFXBulkDefault(..., SQL_C_CHAR (1), nMaxLength).
extern "C" void MS_ABI impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEADPEAPEA_JH_Z(CFieldExchange* pFX, const wchar_t* szName, char** prgStrVals, __int64** prgLengths, int nMaxLength) {
    RfxBulk<char>(pFX, szName, prgStrVals, prgLengths, SQL_C_CHAR, static_cast<unsigned __int64>(nMaxLength),
                  static_cast<size_t>(nMaxLength), false, true);
}

// Symbol: ?RFX_Text_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEA_WPEAPEA_JH@Z
// mfc140u RVA 0x203870: IsFieldType false -> both out-pointers nulled; new wchar_t[m_nRowsetSize * nMaxLength];
// AfxRFXBulkDefault(..., SQL_C_WCHAR (-8), nMaxLength) -- retail passes nMaxLength CHARACTERS as the byte
// length of each row's buffer (the well-known MFC Unicode quirk); transcribed as is.
extern "C" void MS_ABI impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEA_WPEAPEA_JH_Z(CFieldExchange* pFX, const wchar_t* szName, wchar_t** prgStrVals, __int64** prgLengths, int nMaxLength) {
    RfxBulk<wchar_t>(pFX, szName, prgStrVals, prgLengths, SQL_C_WCHAR, static_cast<unsigned __int64>(nMaxLength),
                     static_cast<size_t>(nMaxLength), false, true);
}
