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

// SQL_LEN_DATA_AT_EXEC(len) with the retail clamp: len is limited to
// INT_MAX - |SQL_LEN_DATA_AT_EXEC_OFFSET| first (RFX_LongBinary (0x202bc0), the
// block at 0x202f32..0x202f68).
SQLLEN LongBinaryAtExecLength(SQLULEN dwDataLength) {
    // retail: `cmp %rax,%rcx; jbe` -- an unsigned 64-bit compare of m_dwDataLength against the clamp
    const SQLULEN maxLen = static_cast<SQLULEN>(0x7fffffff) - static_cast<SQLULEN>(std::labs(SQL_LEN_DATA_AT_EXEC_OFFSET));
    const SQLLEN len = static_cast<SQLLEN>(dwDataLength > maxLen ? maxLen : dwDataLength);
    return SQL_LEN_DATA_AT_EXEC(len);
}

} // namespace

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
