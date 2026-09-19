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
#include "detail/AtlStrTCore.h"   // openmfc_str::NilData<char>() -- the CStringA nil block
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
extern "C" int MS_ABI impl__Check_CRecordset__UEBAHF_Z(const CRecordset* pThis, short retCode);
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

// Symbol: ??0CFieldExchange@@QEAA@IPEAVCRecordset@@PEAX@Z
extern "C" void* MS_ABI impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(
    void* pThis, UINT op, CRecordset* pRecordset, void* /*pMapField*/) {
    return new(pThis) CFieldExchange((CFieldExchange::RFX_Operation)op, pRecordset);
}
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
CFieldExchange::CFieldExchange(RFX_Operation op, CRecordset* pRecordset)
    : m_pRecordset(pRecordset), m_nOperation(op), m_nFields(0), m_nParams(0) {
    memset(_cfieldexchange_padding, 0, sizeof(_cfieldexchange_padding));
}
CFieldExchange::~CFieldExchange() {}
BOOL CFieldExchange::IsFieldType(UINT* pnField) {
    if (!pnField) return FALSE;
    if (m_nOperation == BindParam || m_nOperation == BindParamToField) {
        if (*pnField == 0) {
            *pnField = ++m_nParams;
        }
        return (*pnField != 0);
    }
    if (*pnField == 0) {
        *pnField = ++m_nFields;
    }
    return (*pnField != 0);
}
void CFieldExchange::SetFieldType(UINT* pnField) {
    if (!pnField) return;
    if (m_nOperation == BindParam || m_nOperation == BindParamToField) {
        if (*pnField == 0) {
            *pnField = ++m_nParams;
        }
        return;
    }
    if (*pnField == 0) {
        *pnField = ++m_nFields;
    }
}
void CFieldExchange::Default(const wchar_t* lpszName, void* pv, __int64* pnLen,
                              int nSQLType, unsigned __int64 nLen, void* pvPrecision) {
    (void)lpszName; (void)nSQLType;
    if (!m_pRecordset) return;

    RecordsetState& state = EnsureRecordsetState(m_pRecordset);
    bool isParam = (m_nOperation == BindParam || m_nOperation == BindParamToField);
    unsigned long fieldIndex = 0;
    if (isParam) {
        if (m_nParams == 0) ++m_nParams;
        fieldIndex = static_cast<unsigned long>(m_nParams - 1);
    } else {
        if (m_nFields == 0) ++m_nFields;
        fieldIndex = static_cast<unsigned long>(m_nFields - 1);
    }

    if (pv) {
        if (isParam) state.boundParamIndexes[pv] = static_cast<int>(fieldIndex);
        else state.boundFieldIndexes[pv] = static_cast<int>(fieldIndex);
    }
    if (pnLen) *pnLen = static_cast<__int64>(nLen);
    if (pvPrecision) {
        if (nSQLType == SQL_NUMERIC || nSQLType == SQL_DECIMAL) {
            *static_cast<__int64*>(pvPrecision) = static_cast<__int64>(nLen);
        } else {
            *static_cast<__int64*>(pvPrecision) = 0;
        }
    }
    if (state.fieldLengths.size() <= fieldIndex) {
        state.fieldLengths.resize(fieldIndex + 1, 0);
    }
    if (nLen <= static_cast<unsigned __int64>(std::numeric_limits<long long>::max())) {
        state.fieldLengths[fieldIndex] = static_cast<SQLLEN>(nLen);
    }
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
// The class in include/openmfc/afxdb.h has a vptr plus m_pRecordset (+8),
// m_nOperation (+0x10), m_nFields (+0x14), m_nParams (+0x18) and 32 bytes of
// padding (sizeof 0x40).  Everything retail keys off m_nFieldType, m_pvField,
// m_bField, m_pstr, m_lpszSeparator, m_nParamFields or the per-exchange
// m_hstmt is therefore either approximated (each such point is marked
// "Deviation:") or left unmodelled and said so.  A header fix is requested.
//
// OPERATION NUMBERING.  The header's RFX_Operation enum does not match retail
// (retail: BindParam=0 RebindParam=1 BindFieldToColumn=2 BindFieldForUpdate=3
// UnbindFieldForUpdate=4 Fixup=5 MarkForAddNew=6 MarkForUpdate=7 Name=8
// NameValue=9 Value=10 SetFieldNull=11 StoreField=12 LoadField=13 AllocCache=14
// AllocMultiRowBuffer=15 DeleteMultiRowBuffer=16 -- every switch below was read
// off the retail bodies; the header has Fixup=2, SetFieldNull=3, ...).  The
// numbering is ABI-visible: client code compiled against the real afxdb.h
// tests pFX->m_nOperation against these values, and the exported constructor
// receives them.  The bodies below therefore switch on the RETAIL values via
// rfxop:: and treat m_nOperation as a plain UINT.  Nothing in this DLL's own
// CRecordset constructs a CFieldExchange or drives DoFieldExchange today, so
// no internal caller depends on the header's numbering.  (Also ABI-visible:
// SetFieldType(UINT) is inline in the real afxdb.h and is not exported -- a
// client's DoFieldExchange writes m_nFieldType at +0x10, which in this
// header's layout is m_nOperation.)
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
// This codebase's CRecordset has m_hstmt (+8) and m_pDatabase (+0x10) at the
// retail offsets and none of the arrays; the stand-ins are the exported status
// thunks in core/db/CRecordset.cpp (backed by RecordsetState: dirtyFields /
// nullFields / nullParams / boundFieldIndexes / boundParamIndexes /
// fieldLengths / fields) plus the file-local proxy and parameter-length tables
// below.  Field status bit 1 == dirty, bit 2 == null, exactly as in retail
// m_pbFieldFlags.  Indexes passed to the status thunks are 0-based
// (nField - 1), as in retail.  The two CMapPtrToPtr entries retail makes
// (m_mapFieldIndex[pv] = nField, m_mapParamIndex[pv] = nField, 1-based) are
// stored 0-based in boundFieldIndexes / boundParamIndexes because that is what
// this CRecordset's IsFieldNull / IsFieldDirty look up.
//
// NOT reproduced: the retail range checks "nField - 1 < m_prs->m_nFields"
// (AfxThrowUserException, RVA 0x2a4310) that precede every status-flag access.
// This CRecordset::m_nFields is not maintained by an RFX pass, so the check
// would misfire.  The retail vtable calls m_prs->Check(rc) (slot 21) and
// m_prs->ThrowDBException(rc, hstmt) (slot 24) go through impl__Check_CRecordset
// and the AfxThrowDBException thunk, because this CRecordset's vtable is not
// the retail one.  Retail's SQLBindCol / SQLBindParameter / SQLGetData are the
// ODBC32 delay-load slots 0x3e9038 (ordinal 4), 0x3e9040 (ordinal 72) and
// 0x3e9120 (ordinal 43) -- resolved against ODBC32's export table.
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

// Retail decides field-vs-parameter from m_nFieldType (outputColumn == 0).
// This CFieldExchange has no m_nFieldType; its IsFieldType() counts a
// parameter for the two parameter operations and a column otherwise, so the
// operation is the stand-in.  (Header's BindParam / BindParamToField happen to
// be 0 / 1, the retail BindParam / RebindParam values.)
inline bool IsParamOperation(const CFieldExchange* pFX) {
    return RfxOp(pFX) == rfxop::BindParam || RfxOp(pFX) == rfxop::RebindParam;
}

inline HSTMT RecordsetHstmt(const CFieldExchange* pFX) {
    return pFX->m_pRecordset ? pFX->m_pRecordset->m_hstmt : SQL_NULL_HSTMT;
}
inline void* RecordsetDatabase(const CFieldExchange* pFX) {
    return pFX->m_pRecordset ? static_cast<void*>(pFX->m_pRecordset->m_pDatabase) : nullptr;
}

// Retail: m_prs->ThrowDBException(rc, hstmt) -- CRecordset vtable slot 24, body
// at mfc140u RVA 0x1fdff0: `if (!hstmt) hstmt = m_hstmt; AfxThrowDBException(rc, m_pDatabase, hstmt)`.
void ThrowDBException(const CFieldExchange* pFX, RETCODE rc, HSTMT hstmt) {
    if (!hstmt) hstmt = RecordsetHstmt(pFX);
    impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(rc, RecordsetDatabase(pFX), hstmt);
}

// Retail: the length buffer comes from m_prs->GetFieldLengthBuffer(nField - 1,
// m_nFieldType) (0x1fc950): &m_rgFieldInfos[n].m_nLength for a column,
// &m_plParamLength[n] for a parameter, AfxThrowUserException out of range.  The
// stand-in thunk in core/db/CRecordset.cpp keeps ONE std::vector (fieldLengths)
// and its third argument means "grow", so parameter lengths get a file-local
// table of their own here (std::deque: stable addresses); otherwise field 0 and
// parameter 0 would share an SQLLEN that SQLBindCol and SQLBindParameter both
// write through.
std::unordered_map<const CRecordset*, std::deque<SQLLEN>>& ParamLengthTable() {
    static std::unordered_map<const CRecordset*, std::deque<SQLLEN>> table;
    return table;
}
// RecordsetState::fieldLengths is a std::vector, so growing it through the
// thunk would move every SQLLEN that SQLBindCol already holds.  Loading the
// column descriptions first (LoadRecordsetFields sizes the vector to the
// column count) keeps later lookups inside the existing storage; it is a no-op
// once loaded.  A column bound beyond that count still grows the vector.
void PrepareLengthBuffers(CRecordset* prs) {
    if (prs && prs->m_hstmt != SQL_NULL_HSTMT) LoadRecordsetFields(prs);
}
SQLLEN* LengthBuffer(CFieldExchange* pFX, UINT nField) {
    if (!pFX->m_pRecordset || nField == 0) return nullptr;
    PrepareLengthBuffers(pFX->m_pRecordset);
    const unsigned long n = nField - 1;
    if (!IsParamOperation(pFX)) {
        return reinterpret_cast<SQLLEN*>(impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z(pFX->m_pRecordset, n, 1));
    }
    std::deque<SQLLEN>& lengths = ParamLengthTable()[pFX->m_pRecordset];
    while (lengths.size() <= n) lengths.push_back(0);
    return &lengths[n];
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

// The ODBC column description retail reads from m_rgODBCFieldInfos[nField - 1];
// here it is RecordsetState::fields, filled by LoadRecordsetFields (SQLDescribeCol).
const OdbcFieldInfoCompat* FieldInfo(CRecordset* prs, UINT nField) {
    if (!prs || prs->m_hstmt == SQL_NULL_HSTMT) return nullptr;
    LoadRecordsetFields(prs);
    auto& fields = EnsureRecordsetState(prs).fields;
    if (nField == 0 || nField > fields.size()) return nullptr;
    return &fields[nField - 1];
}

// Text buffer length rule inlined into every retail RFX_Text body (the block
// at 0x1fef7f..0x1feff6 inside RFX_Text(wchar_t*) (0x1feca0); the same block
// sits in 0x1ff140 / 0x1ff5c0 / 0x1ffdb0), keyed on CODBCFieldInfo::m_nSQLType:
//   CHAR/VARCHAR/LONGVARCHAR/WCHAR/WVARCHAR/WLONGVARCHAR -> precision
//   NUMERIC/DECIMAL                                      -> precision + 2
//   INTEGER/SMALLINT/TINYINT/BIGINT                      -> precision + 1
//   anything else                                        -> precision + 10
// (core/db/CRecordset.cpp's GetTextLen thunk implements a different rule and is
// deliberately not used.)
SQLULEN RetailTextLen(short nSQLType, SQLULEN nPrecision) {
    switch (nSQLType) {
    case SQL_CHAR: case SQL_VARCHAR: case SQL_LONGVARCHAR:
    case SQL_WCHAR: case SQL_WVARCHAR: case SQL_WLONGVARCHAR:
        return nPrecision;
    case SQL_NUMERIC: case SQL_DECIMAL:
        return nPrecision + 2;
    case SQL_INTEGER: case SQL_SMALLINT: case SQL_TINYINT: case SQL_BIGINT:
        return nPrecision + 1;
    default:
        return nPrecision + 10;
    }
}
// Followed in every RFX_Text body by: if (nLen > nMaxLength || nLen < 256) nLen = nMaxLength.
SQLULEN TextBindLength(CFieldExchange* pFX, UINT nField, SQLULEN nMaxLength) {
    const OdbcFieldInfoCompat* info = FieldInfo(pFX->m_pRecordset, nField);
    // Deviation: retail always has the column description here; without one
    // (no statement executed yet) nMaxLength is used, which is also what the
    // retail rule yields for every column shorter than 256 characters.
    SQLULEN nLen = info ? RetailTextLen(info->m_nSQLType, info->m_nPrecision) : nMaxLength;
    if (nLen > nMaxLength || nLen < 256) nLen = nMaxLength;
    return nLen;
}

// ---------------------------------------------------------------------------
// ATL CSimpleStringT<CH> buffer protocol over a client CStringT object, which
// is exactly { CH* m_pszData } with the 24-byte CStringData header in front of
// the characters (openmfc/afxstr.h documents the layout).  Retail inlines the
// ATL members into every RFX_Text(CString&) body -- PrepareWrite, SetLength,
// LockBuffer and ReleaseBuffer are inline; only PrepareWrite2 (RVA 0x31c0 for
// wchar_t) and Fork (RVA 0x3090) are out-of-line calls -- so they are
// reproduced here rather than routed through afxstr.h's CString or
// detail/AtlStrTCore.h's AtlStrT: both of those fork a LOCKED buffer on
// GetBuffer() and reset nRefs to 1 on ReleaseBuffer(), whereas ATL forks only
// a SHARED one (nRefs > 1) and never touches nRefs in ReleaseBuffer.  The RFX
// contract depends on exactly that: BindFieldToColumn hands the buffer to
// SQLBindCol and locks it (nRefs = -1), and every later Fixup re-terminates
// that same buffer in place -- a fork there would leave ODBC writing into a
// buffer the CString no longer owns.
//
// One deliberate difference from ATL: ATL's nil string carries nRefs == 2, so
// it is "shared" and any write forks it.  This codebase's nils
// (GetNilStringData() for wchar_t; openmfc_str::NilData<char>() for char -- a
// client CStringA may point at either, since the exported string manager
// hands out the wide one) carry nRefs == -1, so they are recognised by address
// and treated as shared: forked on write, never released or reallocated.
// ---------------------------------------------------------------------------
inline bool IsNilData(const CStringData* pd) {
    return pd == GetNilStringData() || pd == openmfc_str::NilData<char>();
}
inline size_t StrNLen(const wchar_t* p, size_t cap) { return ::wcsnlen(p, cap); }
inline size_t StrNLen(const char* p, size_t cap) { return ::strnlen(p, cap); }

template <class CH>
struct RfxStr {
    CH** m_pp;   // &value.m_pszData

    explicit RfxStr(void* pString) : m_pp(static_cast<CH**>(pString)) {}
    CH* Psz() const { return *m_pp; }
    CStringData* Data() const { return reinterpret_cast<CStringData*>(*m_pp) - 1; }
    int Length() const { return Data()->nDataLength; }
    int AllocLength() const { return Data()->nAllocLength; }

    static CH* Chars(CStringData* pd) { return reinterpret_cast<CH*>(pd + 1); }   // characters follow the 24-byte header
    static IAtlStringMgr* Mgr(CStringData* pd) { return pd->pStringMgr ? pd->pStringMgr : OpenMFC_GetStringMgr(); }
    static bool Shared(const CStringData* pd) { return pd->nRefs > 1 || IsNilData(pd); }

    // CStringData::Release(): if (--nRefs <= 0) pStringMgr->Free(this).  Never on a nil.
    static void ReleaseData(CStringData* pd) {
        if (IsNilData(pd)) return;
        if (--pd->nRefs <= 0) Mgr(pd)->Free(pd);
    }
    // CSimpleStringT::Fork(nLength): allocate nLength, copy min(old, new) + NUL,
    // keep nDataLength, release the old block.
    void Fork(int nLength) {
        CStringData* pOld = Data();
        const int nOldLength = pOld->nDataLength;
        CStringData* pNew = Mgr(pOld)->Allocate(nLength, sizeof(CH));
        if (!pNew) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        const int nCopy = (nOldLength < nLength ? nOldLength : nLength) + 1;
        std::memcpy(Chars(pNew), Chars(pOld), static_cast<size_t>(nCopy) * sizeof(CH));
        pNew->nDataLength = nOldLength;
        ReleaseData(pOld);
        *m_pp = Chars(pNew);
    }
    // CSimpleStringT::Reallocate(nLength): in-place growth of an unshared block.
    void Reallocate(int nLength) {
        CStringData* pOld = Data();
        if (pOld->nAllocLength >= nLength || nLength <= 0) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        CStringData* pNew = Mgr(pOld)->Reallocate(pOld, nLength, sizeof(CH));
        if (!pNew) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        *m_pp = Chars(pNew);
    }
    // CSimpleStringT::PrepareWrite2(nLength) as retail has it at RVA 0x31c0:
    // nLength = max(nLength, nDataLength); shared -> Fork(nLength); else if
    // nAllocLength < nLength -> Reallocate(max(nLength, grown)) where grown is
    // nAllocLength + nAllocLength/2, or + 1M once past 1G.
    void PrepareWrite2(int nLength) {
        CStringData* pOld = Data();
        if (pOld->nDataLength > nLength) nLength = pOld->nDataLength;
        if (Shared(pOld)) {
            Fork(nLength);
        } else if (pOld->nAllocLength < nLength) {
            int nNew = pOld->nAllocLength;
            if (nNew > 1024 * 1024 * 1024) nNew += 1024 * 1024;
            else nNew += nNew / 2;
            if (nNew < nLength) nNew = nLength;
            Reallocate(nNew);
        }
    }
    // CSimpleStringT::PrepareWrite(nLength): if (nRefs > 1 || nLength > nAllocLength) PrepareWrite2(nLength)
    CH* PrepareWrite(int nLength) {
        CStringData* pd = Data();
        if (Shared(pd) || nLength > pd->nAllocLength) PrepareWrite2(nLength);
        return *m_pp;
    }
    // CSimpleStringT::SetLength(n): ATLENSURE_THROW(0 <= n <= nAllocLength, E_INVALIDARG)
    // (retail: AtlThrow(E_INVALIDARG) == AfxThrowOleException, via the stub at RVA 0x333c).
    void SetLength(int nLength) {
        CStringData* pd = Data();
        if (nLength < 0 || nLength > pd->nAllocLength) { impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG); return; }
        pd->nDataLength = nLength;
        (*m_pp)[nLength] = 0;
    }
    CH* GetBuffer(int nMinBufferLength) { return PrepareWrite(nMinBufferLength); }
    CH* GetBufferSetLength(int nLength) { CH* p = PrepareWrite(nLength); SetLength(nLength); return p; }
    // ReleaseBuffer(-1): SetLength(StringLengthN(psz, nAllocLength)).
    void ReleaseBuffer() { SetLength(static_cast<int>(StrNLen(*m_pp, static_cast<size_t>(Data()->nAllocLength)))); }
    // LockBuffer(): if shared Fork(nDataLength); --nRefs; if (nRefs == 0) nRefs = -1.
    void LockBuffer() {
        if (Shared(Data())) Fork(Data()->nDataLength);
        CStringData* pd = Data();
        --pd->nRefs;
        if (pd->nRefs == 0) pd->nRefs = -1;
    }
};

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
// The scalar RFX_* exports (Long, Int, BigInt, Byte, Bool, Single, Double and
// Date(TIMESTAMP_STRUCT&)) are one body each in retail, all with this shape
// (RFX_Long, mfc140u RVA 0x2008e0, is the reference; the others differ only in
// the constants listed at each export, and RFX_Bool in its Fixup):
//
//   if (!pFX || !szName) AfxThrowInvalidArgException();
//   UINT nField; if (!pFX->IsFieldType(&nField)) return;
//   SQLLEN* plLength = pFX->m_prs->GetFieldLengthBuffer(nField - 1, pFX->m_nFieldType);
//   switch (pFX->m_nOperation) {
//   case Fixup:          if (*plLength == SQL_NULL_DATA) { SetNullFieldStatus(n); value = PSEUDO_NULL; } return;
//   case MarkForAddNew:  if (value != PSEUDO_NULL) { SetDirtyFieldStatus(n); ClearNullFieldStatus(n); } return;
//   case MarkForUpdate:  if (value != PSEUDO_NULL) ClearNullFieldStatus(n);  /* falls into default */
//   default:             pFX->Default(szName, &value, plLength, nCFType, sizeof(value), cbPrecision); return;
//   case SetFieldNull:   if ((m_pvField == NULL && m_nFieldType == outputColumn) || m_pvField == &value) {
//                            if (m_bField) { SetNullFieldStatus(n); value = PSEUDO_NULL; *plLength = SQL_NULL_DATA; }
//                            else          { ClearNullFieldStatus(n); *plLength = sizeof(value); } }  return;
//   case AllocCache:     m_prs->m_rgFieldInfos[n].m_nDataType = <AFX_RFX_xxx>; (8/16-byte types also
//                        allocate m_pvDataCache with operator new)  return;
//   }
//
// NOT MODELLED here: SetFieldNull (needs m_pvField / m_bField, which this
// CFieldExchange lacks -- CRecordset::SetFieldNull in this codebase edits the
// side table directly and never issues the operation) and AllocCache (there is
// no m_rgFieldInfos cache; AfxStoreField/AfxLoadField in
// featurepack/CMFC_misc_stubs.cpp document the same gap).  Both return without
// touching anything.
// ---------------------------------------------------------------------------
template <typename T, typename IsPseudoNull, typename SetPseudoNull, typename FixupNotNull>
void RfxScalar(CFieldExchange* pFX, const wchar_t* szName, T* pValue, int nCFType,
               unsigned __int64 cbPrecision, IsPseudoNull isPseudoNull, SetPseudoNull setPseudoNull,
               FixupNotNull fixupNotNull, bool ensureRecordset) {
    if (!pFX || !szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    // RFX_Date(TIMESTAMP_STRUCT&) additionally ENSUREs m_prs and the length buffer.
    if (ensureRecordset && !pFX->m_pRecordset) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    SQLLEN* plLength = LengthBuffer(pFX, nField);
    if (ensureRecordset && !plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    CRecordset* prs = pFX->m_pRecordset;
    const unsigned long n = nField - 1;
    switch (RfxOp(pFX)) {
    case rfxop::Fixup:
        if (plLength && *plLength == SQL_NULL_DATA) {
            impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            setPseudoNull(*pValue);
        } else {
            fixupNotNull(*pValue);
        }
        return;
    case rfxop::MarkForAddNew:
        if (!isPseudoNull(*pValue)) {
            impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        }
        return;
    case rfxop::MarkForUpdate:
        if (!isPseudoNull(*pValue)) impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        break;  // retail falls into the Default call
    case rfxop::SetFieldNull:
    case rfxop::AllocCache:
        return;  // not modelled, see above
    default:
        break;
    }
    impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(pFX, szName, pValue, plLength, nCFType, sizeof(T), cbPrecision);
}
template <typename T> void FixupKeep(T&) {}

// Bit patterns retail stores for the floating-point pseudo-nulls
// (AFX_RFX_SINGLE_PSEUDO_NULL / AFX_RFX_DOUBLE_PSEUDO_NULL == -9.123e19):
// `movl $0xe09e4242,(%rsi)` at 0x1fe0fe inside RFX_Single (0x1fe030) and
// `movabs $0xc413c84846922b8c,%rax` at 0x1fe2b8 inside RFX_Double (0x1fe1e0);
// the ucomiss/ucomisd operands at rdata 0x350118 / 0x350110 hold the same values.
float SinglePseudoNull() { const unsigned bits = 0xe09e4242u; float f; std::memcpy(&f, &bits, sizeof(f)); return f; }
double DoublePseudoNull() { const unsigned long long bits = 0xc413c84846922b8cull; double d; std::memcpy(&d, &bits, sizeof(d)); return d; }

// AFX_RFX_DATE_PSEUDO_NULL: every TIMESTAMP_STRUCT member == 99, fraction included
// (RFX_Date(TIMESTAMP_STRUCT&) (0x201be0), the compares at 0x201d04..0x201d2f).
bool IsTimestampPseudoNull(const TIMESTAMP_STRUCT& ts) {
    return ts.year == 99 && ts.month == 99 && ts.day == 99 && ts.hour == 99 &&
           ts.minute == 99 && ts.second == 99 && ts.fraction == 99;
}
void SetTimestampPseudoNull(TIMESTAMP_STRUCT& ts) {
    ts.year = 99; ts.month = 99; ts.day = 99; ts.hour = 99; ts.minute = 99; ts.second = 99; ts.fraction = 99;
}

// ---------------------------------------------------------------------------
// RFX_Text(CStringT<CH>&): one body for the CStringW export (mfc140u RVA
// 0x1ff5c0, SQL_C_WCHAR, wcsnlen, byte lengths x2) and the CStringA export
// (RVA 0x1ffdb0, SQL_C_CHAR, strnlen, byte lengths x1); they differ in nothing
// else.  pString is the client CStringT object (a single CH* m_pszData).
// ---------------------------------------------------------------------------
template <class CH>
void RfxTextString(CFieldExchange* pFX, const wchar_t* szName, void* pString, int nMaxLength,
                   int nColumnType, short nScale, int nCFType) {
    if (!pFX || !szName || !pString) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    RfxStr<CH> value(pString);
    SQLLEN* plLength = LengthBuffer(pFX, nField);
    CRecordset* prs = pFX->m_pRecordset;
    const unsigned long n = nField - 1;
    switch (RfxOp(pFX)) {
    case rfxop::BindParam: {
        // 0x1ffc3f: value.GetBufferSetLength(nMaxLength) (its inlined SetLength range check --
        // AtlThrow(E_INVALIDARG) for nMaxLength < 0, hoisted ahead of PrepareWrite, and for
        // nMaxLength > nAllocLength after it); value.LockBuffer();
        // *plLength = paramnull ? SQL_NULL_DATA : SQL_NTS;
        // rc = SQLBindParameter(m_hstmt, nField, m_nFieldType, nCFType, nColumnType, nMaxLength, nScale,
        //                       buffer, nMaxLength * sizeof(CH), plLength); value.ReleaseBuffer();
        // if (rc != SQL_SUCCESS) ThrowDBException(rc, m_hstmt); m_mapParamIndex[&value] = nField;
        if (nMaxLength < 0) { impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG); return; }
        if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        CH* buffer = value.GetBufferSetLength(nMaxLength);
        value.LockBuffer();
        *plLength = impl__IsParamStatusNull_CRecordset__QEBAHK_Z(prs, n) ? SQL_NULL_DATA : SQL_NTS;
        // Deviation: m_nFieldType (the SQL_PARAM_* direction) and the per-exchange
        // m_hstmt are absent here; SQL_PARAM_INPUT and the recordset's m_hstmt are used.
        RETCODE rc = ::SQLBindParameter(RecordsetHstmt(pFX), static_cast<SQLUSMALLINT>(nField), SQL_PARAM_INPUT,
                                        static_cast<SQLSMALLINT>(nCFType), static_cast<SQLSMALLINT>(nColumnType),
                                        static_cast<SQLULEN>(nMaxLength), nScale, buffer,
                                        static_cast<SQLLEN>(nMaxLength) * static_cast<SQLLEN>(sizeof(CH)), plLength);
        value.ReleaseBuffer();
        if (rc != SQL_SUCCESS) ThrowDBException(pFX, rc, RecordsetHstmt(pFX));
        EnsureRecordsetState(prs).boundParamIndexes[pString] = static_cast<int>(n);
        return;
    }
    case rfxop::BindFieldToColumn: {
        // 0x1ffa7e: nLen from the column description (RetailTextLen / TextBindLength);
        // if (nLen > 0x7ffffffe) AfxThrowMemoryException(); value.GetBufferSetLength(nLen + 1);
        // value.LockBuffer(); rc = SQLBindCol(m_prs->m_hstmt, nField, nCFType, buffer,
        // (nLen + 1) * sizeof(CH), plLength); value.ReleaseBuffer();
        // if (!Check(rc)) ThrowDBException(rc, SQL_NULL_HSTMT); m_mapFieldIndex[&value] = nField;
        SQLULEN nLen = TextBindLength(pFX, nField, static_cast<SQLULEN>(nMaxLength));
        plLength = LengthBuffer(pFX, nField);  // re-read after the column load (see PrepareLengthBuffers)
        if (nLen > 0x7ffffffeULL) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        CH* buffer = value.GetBufferSetLength(static_cast<int>(nLen) + 1);
        value.LockBuffer();
        RETCODE rc = ::SQLBindCol(RecordsetHstmt(pFX), static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(nCFType),
                                  buffer, static_cast<SQLLEN>((nLen + 1) * sizeof(CH)), plLength);
        value.ReleaseBuffer();
        if (!impl__Check_CRecordset__UEBAHF_Z(prs, rc)) ThrowDBException(pFX, rc, SQL_NULL_HSTMT);
        EnsureRecordsetState(prs).boundFieldIndexes[pString] = static_cast<int>(n);
        return;
    }
    case rfxop::Fixup:
        // 0x1ff933: if (*plLength == SQL_NULL_DATA) { SetNullFieldStatus(n); value.GetBufferSetLength(0);
        // value.ReleaseBuffer(); } else { value.GetBuffer(0); if (m_prs->m_pDatabase->m_bStripTrailingSpaces)
        // <cut the string at its last run of ' '>; value.ReleaseBuffer(); *plLength = value.GetLength() * sizeof(CH); }
        // The buffer must stay the one SQLBindCol holds, so it is only re-terminated, never reallocated.
        if (plLength && *plLength == SQL_NULL_DATA) {
            impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            value.GetBufferSetLength(0);
            value.ReleaseBuffer();
        } else {
            value.GetBuffer(0);
            // Deviation: the BOOL at retail CDatabase+0x10 (m_bStripTrailingSpaces in the MFC
            // source; the retail ctor at RVA 0x1f6b10 zeroes it) has no counterpart in this
            // CDatabase, so the retail default -- nothing stripped -- is the only behaviour.
            value.ReleaseBuffer();
            if (plLength) *plLength = static_cast<SQLLEN>(value.Length()) * static_cast<SQLLEN>(sizeof(CH));
        }
        return;
    case rfxop::MarkForAddNew:
        // 0x1ff8ec: if (value.GetLength() != 0) { SetDirtyFieldStatus(n); ClearNullFieldStatus(n); }
        if (value.Length() != 0) {
            impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        }
        return;
    case rfxop::MarkForUpdate:
        // 0x1ff893: if (value.GetLength() == 0) SetNullFieldStatus(n); else ClearNullFieldStatus(n);
        // Default(szName, &value /* the CString object */, plLength, nCFType, value.GetLength() * sizeof(CH), nMaxLength);
        if (value.Length() == 0) impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        else impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(pFX, szName, pString, plLength, nCFType,
            static_cast<unsigned __int64>(value.Length()) * sizeof(CH), static_cast<unsigned __int64>(nMaxLength));
        return;
    case rfxop::StoreField:
        // 0x1ff7c2: AfxStoreField(*m_prs, nField, &value)
        impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(prs, nField, pString);
        return;
    case rfxop::LoadField:
        // 0x1ff767: m_pbFieldFlags[n] |= m_rgFieldInfos[n].m_bStatus; if null: *plLength = SQL_NULL_DATA;
        // else { value = *(CStringT*)m_rgFieldInfos[n].m_pvDataCache; *plLength = value.GetLength() * sizeof(CH); }
        // Retail does this inline; the AfxLoadField thunk is the closest thing this codebase has
        // (it reproduces the null half only and documents why).
        impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(prs, nField, pString, plLength);
        return;
    case rfxop::SetFieldNull:   // 0x1ff7d6: needs m_pvField / m_bField -- not modelled
    case rfxop::AllocCache:     // 0x1ff70d: m_rgFieldInfos[n].m_pvDataCache = new CString, m_nDataType = AFX_RFX_TEXT (20) -- no cache here
        return;
    default: {
        // 0x1ff67e: cbValue = value.GetLength() * sizeof(CH); value.GetBuffer(0);
        // Default(szName, buffer, plLength, nCFType, cbValue, nMaxLength); value.ReleaseBuffer();
        const unsigned __int64 cbValue = static_cast<unsigned __int64>(value.Length()) * sizeof(CH);
        CH* buffer = value.GetBuffer(0);
        impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(pFX, szName, buffer, plLength, nCFType, cbValue,
                                                                 static_cast<unsigned __int64>(nMaxLength));
        value.ReleaseBuffer();
        return;
    }
    }
}

// ---------------------------------------------------------------------------
// RFX_Text(TCHAR*) body shared by the wchar_t* (mfc140u RVA 0x1feca0) and
// char* (RVA 0x1ff140) exports; they differ only in the C type and the
// character size.
// ---------------------------------------------------------------------------
template <typename CharT>
void RfxTextBuffer(CFieldExchange* pFX, const wchar_t* szName, CharT* value, int nMaxLength, int nColumnType, short nScale, int nCFType) {
    if (!pFX || !szName || !value) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    SQLLEN* plLength = LengthBuffer(pFX, nField);
    CRecordset* prs = pFX->m_pRecordset;
    const unsigned long n = nField - 1;
    auto strLen = [](const CharT* s) -> size_t { size_t i = 0; while (s[i]) ++i; return i; };
    switch (RfxOp(pFX)) {
    case rfxop::BindParam:
        // 0x1ff088: *plLength = paramnull ? SQL_NULL_DATA : SQL_NTS; rc = SQLBindParameter(m_hstmt, nField,
        // m_nFieldType, nCFType, nColumnType, nMaxLength, nScale, value, nMaxLength * sizeof(CharT), plLength);
        // if (rc != SQL_SUCCESS) ThrowDBException(rc, m_hstmt); m_mapParamIndex[value] = nField;
        if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        *plLength = impl__IsParamStatusNull_CRecordset__QEBAHK_Z(prs, n) ? SQL_NULL_DATA : SQL_NTS;
        {
            // Deviation: SQL_PARAM_INPUT for the absent m_nFieldType, the recordset's m_hstmt for the absent fx.m_hstmt.
            RETCODE rc = ::SQLBindParameter(RecordsetHstmt(pFX), static_cast<SQLUSMALLINT>(nField), SQL_PARAM_INPUT,
                                            static_cast<SQLSMALLINT>(nCFType), static_cast<SQLSMALLINT>(nColumnType),
                                            static_cast<SQLULEN>(nMaxLength), nScale, value,
                                            static_cast<SQLLEN>(nMaxLength) * static_cast<SQLLEN>(sizeof(CharT)), plLength);
            if (rc != SQL_SUCCESS) ThrowDBException(pFX, rc, RecordsetHstmt(pFX));
        }
        EnsureRecordsetState(prs).boundParamIndexes[value] = static_cast<int>(n);
        return;
    case rfxop::BindFieldToColumn: {
        // 0x1fef71: nLen as RetailTextLen / TextBindLength; value[nLen] = 0;
        // rc = SQLBindCol(m_prs->m_hstmt, nField, nCFType, value, (nLen + 1) * sizeof(CharT), plLength);
        // if (!Check(rc)) ThrowDBException(rc, SQL_NULL_HSTMT); m_mapFieldIndex[value] = nField;
        SQLULEN nLen = TextBindLength(pFX, nField, static_cast<SQLULEN>(nMaxLength));
        plLength = LengthBuffer(pFX, nField);  // re-read after the column load (see PrepareLengthBuffers)
        value[nLen] = 0;
        RETCODE rc = ::SQLBindCol(RecordsetHstmt(pFX), static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(nCFType),
                                  value, static_cast<SQLLEN>((nLen + 1) * sizeof(CharT)), plLength);
        if (!impl__Check_CRecordset__UEBAHF_Z(prs, rc)) ThrowDBException(pFX, rc, SQL_NULL_HSTMT);
        EnsureRecordsetState(prs).boundFieldIndexes[value] = static_cast<int>(n);
        return;
    }
    case rfxop::Fixup:
        // 0x1feeec: if (*plLength == SQL_NULL_DATA) { SetNullFieldStatus(n); value[0] = 0; }
        // else { if (m_prs->m_pDatabase->m_bStripTrailingSpaces) <strip>; *plLength = strlen(value) * sizeof(CharT); }
        if (plLength && *plLength == SQL_NULL_DATA) {
            impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            value[0] = 0;
        } else if (plLength) {
            // Deviation: no m_bStripTrailingSpaces in this CDatabase (retail default FALSE), nothing stripped.
            *plLength = static_cast<SQLLEN>(strLen(value) * sizeof(CharT));
        }
        return;
    case rfxop::MarkForAddNew:
        // 0x1feead: if (value[0] != 0) { SetDirtyFieldStatus(n); ClearNullFieldStatus(n); }
        if (value[0] != 0) {
            impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(prs, n);
            impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        }
        return;
    case rfxop::MarkForUpdate:
        // 0x1fee81: if (value[0] == 0) SetNullFieldStatus(n); else ClearNullFieldStatus(n); then the default call
        if (value[0] == 0) impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        else impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(prs, n);
        break;
    case rfxop::StoreField:
        // 0x1fee21: AfxStoreField(*m_prs, nField, value)
        impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(prs, nField, value);
        return;
    case rfxop::LoadField:
        // 0x1fedce: m_pbFieldFlags[n] |= m_bStatus; null -> *plLength = SQL_NULL_DATA, else
        // *plLength = strlen(m_pvDataCache) * sizeof(CharT) (retail copies nothing back here).
        // Retail does this inline; only the null half exists here (AfxLoadField thunk).
        impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(prs, nField, value, plLength);
        return;
    case rfxop::SetFieldNull:   // 0x1fee34: needs m_pvField / m_bField -- not modelled
    case rfxop::AllocCache:     // 0x1fed9b: m_pvDataCache = new BYTE[nMaxLength * sizeof(CharT)], m_nDataType = AFX_RFX_LPTSTR (18) -- no cache here
        return;
    default:
        break;
    }
    // 0x1fed58: Default(szName, value, plLength, nCFType, strlen(value) * sizeof(CharT), nMaxLength)
    impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(pFX, szName, value, plLength, nCFType,
        static_cast<unsigned __int64>(strLen(value) * sizeof(CharT)), static_cast<unsigned __int64>(nMaxLength));
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
        EnsureRecordsetState(prs).boundParamIndexes[&value] = static_cast<int>(n);
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
        if (!impl__Check_CRecordset__UEBAHF_Z(prs, rc)) ThrowDBException(pFX, rc, SQL_NULL_HSTMT);
        EnsureRecordsetState(prs).boundFieldIndexes[&value] = static_cast<int>(n);
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

// Symbol: ?Default@CFieldExchange@@QEAAXPEB_WPEAXPEA_JH_K3@Z
// void CFieldExchange::Default(LPCTSTR szName, void* pv, SQLLEN* plLength, int nCFType,
//                              SQLULEN cbValue, SQLULEN cbPrecision)
// Transcribed from mfc140u RVA 0x1fe750.  nField is m_nFields for a column
// operation and m_nParams for a parameter operation (retail: m_nFieldType == 0
// selects +0x38 else +0x3c); n = nField - 1 indexes the status arrays, and the
// retail range check (n >= m_nFields / m_nParams -> AfxThrowUserException)
// precedes every status access.
//   BindParam (0x1fe8ed):   ENSURE(plLength); *plLength = paramnull(n) ? SQL_NULL_DATA : cbValue;
//       rc = SQLBindParameter(m_hstmt, nField, m_nFieldType, nCFType, nCFType /*ParameterType*/,
//                             cbPrecision /*ColumnSize*/, 0, pv, 0 /*BufferLength*/, plLength);
//       if (rc != SQL_SUCCESS) m_prs->ThrowDBException(rc, m_hstmt); m_prs->m_mapParamIndex[pv] = nField;
//   RebindParam (0x1fe8ae): ENSURE(plLength); *plLength = paramnull(n) ? SQL_NULL_DATA : cbValue;
//   BindFieldToColumn (0x1fe83a): rc = SQLBindCol(m_prs->m_hstmt, nField, nCFType, pv, cbValue, plLength);
//       if (!m_prs->Check(rc)) m_prs->ThrowDBException(rc, SQL_NULL_HSTMT); m_prs->m_mapFieldIndex[pv] = nField;
//   BindFieldForUpdate (0x1fe7ee): ENSURE(plLength); *plLength = !dirty(n) ? SQL_IGNORE : null(n) ? SQL_NULL_DATA : cbValue;
//   UnbindFieldForUpdate (0x1fe7b7): if (!dirty(n)) { ENSURE(plLength); *plLength = cbValue; }
//       (`testb $1,flags[n]; jne exit` -- a dirty field keeps the SQL_IGNORE / SQL_NULL_DATA /
//       SQL_LEN_DATA_AT_EXEC length BindFieldForUpdate gave it; only a clean one is reset)
//   Fixup, MarkForAddNew: nothing (0x1fe7ae falls to the epilogue).
//   MarkForUpdate (0x1fe9a6): if the cached value was null (m_rgFieldInfos[n].m_bStatus & 2) set dirty
//       when the field is no longer null; else set dirty when the field is null now or
//       !AfxCompareValueByRef(pv, cache, m_nDataType); then if (m_pvField == NULL || m_pvField == pv)
//       and dirty(n), m_bField = TRUE.
//   Name (0x1fec11): if (dirty(n)) { *m_pstr += szName; *m_pstr += m_lpszSeparator; }
//   NameValue (0x1feab0): if (dirty(n)) { *m_pstr += szName; *m_pstr += '='; } then Value.
//   Value (0x1feaff): if (!dirty(n)) return; ENSURE(plLength); *plLength = null(n) ? SQL_NULL_DATA : cbValue;
//       if (m_prs->[+0x40] & 0x4000) return; *m_pstr += '?'; *m_pstr += m_lpszSeparator; ++m_nParamFields;
//       rc = SQLBindParameter(m_hstmt, m_nParamFields, SQL_PARAM_INPUT, nCFType, m_rgODBCFieldInfos[n].m_nSQLType,
//                             .m_nPrecision, .m_nScale, pv, 0 /*BufferLength*/, plLength); if (rc != SQL_SUCCESS) ThrowDBException(rc, m_hstmt);
//   StoreField (0x1feaa0): AfxStoreField(*m_prs, nField, pv);   LoadField (0x1fea90): AfxLoadField(*m_prs, nField, pv, plLength);
//   every other value: return.
// NOT MODELLED here: MarkForUpdate (no value cache to compare against, no
// m_pvField/m_bField), Name / NameValue / Value (no m_pstr, m_lpszSeparator,
// m_nParamFields or per-exchange m_hstmt).  Those operations return without
// touching anything.  Deviation: the BindParam direction is SQL_PARAM_INPUT and
// its statement handle is the recordset's m_hstmt, for want of m_nFieldType
// and fx.m_hstmt.  The maps store the 0-based index this codebase's
// CRecordset::IsFieldNull / IsFieldDirty expect (retail stores nField).  Only
// the nField == 0 case of the range check is kept (it is out of range in
// retail too).
extern "C" void MS_ABI impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(
    CFieldExchange* pThis, const wchar_t* szName, void* pv, __int64* plLength,
    int nCFType, unsigned __int64 cbValue, unsigned __int64 cbPrecision) {
    (void)szName;
    if (!pThis || !pThis->m_pRecordset) return;
    CRecordset* prs = pThis->m_pRecordset;
    const UINT nField = IsParamOperation(pThis) ? pThis->m_nParams : pThis->m_nFields;
    if (nField == 0) { impl__AfxThrowUserException__YAXXZ(); return; }
    const unsigned long n = nField - 1;
    switch (RfxOp(pThis)) {
    case rfxop::BindParam: {
        if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        *plLength = impl__IsParamStatusNull_CRecordset__QEBAHK_Z(prs, n) ? SQL_NULL_DATA : static_cast<__int64>(cbValue);
        RETCODE rc = ::SQLBindParameter(prs->m_hstmt, static_cast<SQLUSMALLINT>(nField), SQL_PARAM_INPUT,
                                        static_cast<SQLSMALLINT>(nCFType), static_cast<SQLSMALLINT>(nCFType),
                                        static_cast<SQLULEN>(cbPrecision), 0, pv, 0, plLength);
        if (rc != SQL_SUCCESS) ThrowDBException(pThis, rc, prs->m_hstmt);
        EnsureRecordsetState(prs).boundParamIndexes[pv] = static_cast<int>(n);
        return;
    }
    case rfxop::RebindParam:
        if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        *plLength = impl__IsParamStatusNull_CRecordset__QEBAHK_Z(prs, n) ? SQL_NULL_DATA : static_cast<__int64>(cbValue);
        return;
    case rfxop::BindFieldToColumn: {
        RETCODE rc = ::SQLBindCol(prs->m_hstmt, static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(nCFType),
                                  pv, static_cast<SQLLEN>(cbValue), plLength);
        if (!impl__Check_CRecordset__UEBAHF_Z(prs, rc)) ThrowDBException(pThis, rc, SQL_NULL_HSTMT);
        EnsureRecordsetState(prs).boundFieldIndexes[pv] = static_cast<int>(n);
        return;
    }
    case rfxop::BindFieldForUpdate:
        if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        if (!impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(prs, n)) *plLength = SQL_IGNORE;
        else if (impl__IsFieldStatusNull_CRecordset__QEBAHK_Z(prs, n)) *plLength = SQL_NULL_DATA;
        else *plLength = static_cast<__int64>(cbValue);
        return;
    case rfxop::UnbindFieldForUpdate:
        if (!impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(prs, n)) {
            if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
            *plLength = static_cast<__int64>(cbValue);
        }
        return;
    case rfxop::StoreField:
        impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(prs, nField, pv);
        return;
    case rfxop::LoadField:
        impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(prs, nField, pv, plLength);
        return;
    case rfxop::MarkForUpdate:   // not modelled (see above)
    case rfxop::Name:
    case rfxop::NameValue:
    case rfxop::Value:
    default:
        return;
    }
}

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
        if (!impl__Check_CRecordset__UEBAHF_Z(pFX->m_pRecordset, rc)) ThrowDBException(pFX, rc, SQL_NULL_HSTMT);
    } else if (RfxOp(pFX) == rfxop::Name) {
        if (!szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        // *m_pstr += szName; *m_pstr += m_lpszSeparator;  -- no m_pstr here
    }
}

// Symbol: ?RFX_BigInt@@YAXPEAVCFieldExchange@@PEB_WAEA_J@Z
// mfc140u RVA 0x200720.  Pseudo-null 0x4a4d4120 (as __int64), Default(..., SQL_C_SBIGINT (-25), 8, 19),
// AllocCache: m_pvDataCache = operator new(8), m_nDataType = AFX_RFX_BIGINT (22); SetFieldNull clear -> *plLength = 8.
extern "C" void MS_ABI impl__RFX_BigInt__YAXPEAVCFieldExchange__PEB_WAEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, __int64* value) {
    RfxScalar<__int64>(pFX, szName, value, SQL_C_SBIGINT, 19,
        [](const __int64& v) { return v == 0x4a4d4120LL; }, [](__int64& v) { v = 0x4a4d4120LL; }, FixupKeep<__int64>, false);
}

// Symbol: ?RFX_Binary@@YAXPEAVCFieldExchange@@PEB_WAEAVCByteArray@@_J@Z
// mfc140u RVA 0x200dc0.  Left a stub: the retail body reads and writes the
// CByteArray members directly -- m_pData at +8 and m_nSize at +0x10 (e.g. the
// Fixup store `mov %rax,0x10(%rbx)` at 0x201076 sets m_nSize = *plLength, the
// SQLBindCol at 0x2010d2 binds m_pData) -- besides calling the exported
// CByteArray::SetSize.  This codebase's CByteArray (OPENMFC_DECLARE_ARRAY_WRAPPER
// in openmfc/afx.h) is a CObject with no data members; its storage is a side
// table in detail/FilecoreSupport.h that is only reachable through C++ member
// functions defined in core/collections/CByteArray.cpp, which the link audit
// forbids from here.  Needs the retail CByteArray layout (vptr, m_pData @8,
// m_nSize @0x10, m_nMaxSize @0x18, m_nGrowBy @0x20, sizeof 0x28 -- the
// AllocCache path at 0x200e6a allocates 0x28 bytes and zeroes +8..+0x20).
extern "C" void MS_ABI impl__RFX_Binary__YAXPEAVCFieldExchange__PEB_WAEAVCByteArray___J_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, __int64 p3) {
    (void)p0; (void)p1; (void)p2; (void)p3;
}

// Symbol: ?RFX_Binary_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAEPEAPEA_JH@Z
// mfc140u RVA 0x203d30: alloc new BYTE[m_nRowsetSize * nMaxLength]; default AfxRFXBulkDefault(..., SQL_C_BINARY (-2), nMaxLength).
extern "C" void MS_ABI impl__RFX_Binary_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_JH_Z(CFieldExchange* pFX, const wchar_t* szName, unsigned char** prgByteVals, __int64** prgLengths, int nMaxLength) {
    RfxBulk<unsigned char>(pFX, szName, prgByteVals, prgLengths, SQL_C_BINARY, static_cast<unsigned __int64>(nMaxLength),
                           static_cast<size_t>(nMaxLength), false, false);
}

// Symbol: ?RFX_Bool@@YAXPEAVCFieldExchange@@PEB_WAEAH@Z
// mfc140u RVA 0x200c20.  Pseudo-null AFX_RFX_BOOL_PSEUDO_NULL (2), Default(..., SQL_C_BIT (-7), 4, 1),
// AllocCache m_nDataType = AFX_RFX_BOOL (1); SetFieldNull clear -> *plLength = 4.
// Fixup differs from the other scalars (0x200da5): when the column is not null the
// value is normalised from the single byte SQL_C_BIT wrote: value = (BYTE)value.
extern "C" void MS_ABI impl__RFX_Bool__YAXPEAVCFieldExchange__PEB_WAEAH_Z(CFieldExchange* pFX, const wchar_t* szName, int* value) {
    RfxScalar<int>(pFX, szName, value, SQL_C_BIT, 1,
        [](const int& v) { return v == 2; }, [](int& v) { v = 2; },
        [](int& v) { v = static_cast<int>(*reinterpret_cast<unsigned char*>(&v)); }, false);
}

// Symbol: ?RFX_Bool_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAHPEAPEA_J@Z
// mfc140u RVA 0x203a70 (folded with RFX_Long_Bulk / RFX_Int_Bulk): new long[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_LONG (4), 4).
extern "C" void MS_ABI impl__RFX_Bool_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, int** prgBoolVals, __int64** prgLengths) {
    RfxBulk<int>(pFX, szName, prgBoolVals, prgLengths, SQL_C_LONG, 4, 1, false, false);
}

// Symbol: ?RFX_Byte@@YAXPEAVCFieldExchange@@PEB_WAEAE@Z
// mfc140u RVA 0x200a80.  Pseudo-null AFX_RFX_BYTE_PSEUDO_NULL (255), Default(..., SQL_C_TINYINT (-6), 1, 3),
// AllocCache m_nDataType = AFX_RFX_BYTE (2); SetFieldNull clear -> *plLength = 1.
extern "C" void MS_ABI impl__RFX_Byte__YAXPEAVCFieldExchange__PEB_WAEAE_Z(CFieldExchange* pFX, const wchar_t* szName, unsigned char* value) {
    RfxScalar<unsigned char>(pFX, szName, value, SQL_C_TINYINT, 3,
        [](const unsigned char& v) { return v == 0xff; }, [](unsigned char& v) { v = 0xff; }, FixupKeep<unsigned char>, false);
}

// Symbol: ?RFX_Byte_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAEPEAPEA_J@Z
// mfc140u RVA 0x203c50: new BYTE[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_TINYINT (-6), 1).
extern "C" void MS_ABI impl__RFX_Byte_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, unsigned char** prgByteVals, __int64** prgLengths) {
    RfxBulk<unsigned char>(pFX, szName, prgByteVals, prgLengths, SQL_C_TINYINT, 1, 1, false, false);
}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAUtagTIMESTAMP_STRUCT@@@Z
// mfc140u RVA 0x201be0.  Also ENSUREs m_prs and plLength (0x201c1a / 0x201c3b).  Pseudo-null: all seven
// members == 99; Default(..., SQL_C_TIMESTAMP (11), 16, 23); AllocCache: m_pvDataCache = operator new(16),
// m_nDataType = AFX_RFX_TIMESTAMP (15); SetFieldNull clear -> *plLength = 16.
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAUtagTIMESTAMP_STRUCT___Z(CFieldExchange* pFX, const wchar_t* szName, TIMESTAMP_STRUCT* value) {
    RfxScalar<TIMESTAMP_STRUCT>(pFX, szName, value, SQL_C_TIMESTAMP, 23, IsTimestampPseudoNull, SetTimestampPseudoNull,
                                FixupKeep<TIMESTAMP_STRUCT>, true);
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

// Symbol: ?RFX_Double@@YAXPEAVCFieldExchange@@PEB_WAEAN@Z
// mfc140u RVA 0x1fe1e0.  Pseudo-null AFX_RFX_DOUBLE_PSEUDO_NULL (-9.123e19, compared with ucomisd so a
// NaN never matches), Default(..., SQL_C_DOUBLE (8), 8, 22), AllocCache: m_pvDataCache = operator new(8),
// m_nDataType = AFX_RFX_DOUBLE (7); SetFieldNull clear -> *plLength = 8.
extern "C" void MS_ABI impl__RFX_Double__YAXPEAVCFieldExchange__PEB_WAEAN_Z(CFieldExchange* pFX, const wchar_t* szName, double* value) {
    RfxScalar<double>(pFX, szName, value, SQL_C_DOUBLE, 22,
        [](const double& v) { return v == DoublePseudoNull(); }, [](double& v) { v = DoublePseudoNull(); }, FixupKeep<double>, false);
}

// Symbol: ?RFX_Double_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEANPEAPEA_J@Z
// mfc140u RVA 0x1fe4a0: all four pointers ENSUREd first; new double[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_DOUBLE (8), 8).
extern "C" void MS_ABI impl__RFX_Double_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEANPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, double** prgDblVals, __int64** prgLengths) {
    RfxBulk<double>(pFX, szName, prgDblVals, prgLengths, SQL_C_DOUBLE, 8, 1, true, false);
}

// Symbol: ?RFX_Int@@YAXPEAVCFieldExchange@@PEB_WAEAH@Z
// mfc140u RVA 0x200580.  Pseudo-null AFX_RFX_INT_PSEUDO_NULL (0x7EE4), Default(..., SQL_C_LONG (4), 4, 5),
// AllocCache m_nDataType = AFX_RFX_INT (3); SetFieldNull clear -> *plLength = 4.
extern "C" void MS_ABI impl__RFX_Int__YAXPEAVCFieldExchange__PEB_WAEAH_Z(CFieldExchange* pFX, const wchar_t* szName, int* value) {
    RfxScalar<int>(pFX, szName, value, SQL_C_LONG, 5,
        [](const int& v) { return v == 0x7ee4; }, [](int& v) { v = 0x7ee4; }, FixupKeep<int>, false);
}

// Symbol: ?RFX_Int_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAHPEAPEA_J@Z
// mfc140u RVA 0x203a70 (folded with RFX_Long_Bulk / RFX_Bool_Bulk): new long[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_LONG (4), 4).
extern "C" void MS_ABI impl__RFX_Int_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, int** prgIntVals, __int64** prgLengths) {
    RfxBulk<int>(pFX, szName, prgIntVals, prgLengths, SQL_C_LONG, 4, 1, false, false);
}

// Symbol: ?RFX_Long@@YAXPEAVCFieldExchange@@PEB_WAEAJ@Z
// mfc140u RVA 0x2008e0 (the reference body quoted above RfxScalar).  Pseudo-null
// AFX_RFX_LONG_PSEUDO_NULL (0x4a4d4120), Default(..., SQL_C_LONG (4), 4, 10),
// AllocCache m_nDataType = AFX_RFX_LONG (4); SetFieldNull clear -> *plLength = 4.
extern "C" void MS_ABI impl__RFX_Long__YAXPEAVCFieldExchange__PEB_WAEAJ_Z(CFieldExchange* pFX, const wchar_t* szName, long* value) {
    RfxScalar<long>(pFX, szName, value, SQL_C_LONG, 10,
        [](const long& v) { return v == 0x4a4d4120L; }, [](long& v) { v = 0x4a4d4120L; }, FixupKeep<long>, false);
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
        if (prs) EnsureRecordsetState(prs).boundFieldIndexes[value] = static_cast<int>(n);
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

// Symbol: ?RFX_Single@@YAXPEAVCFieldExchange@@PEB_WAEAM@Z
// mfc140u RVA 0x1fe030.  Pseudo-null AFX_RFX_SINGLE_PSEUDO_NULL (-9.123e19f, ucomiss compare),
// Default(..., SQL_C_FLOAT (7), 4, 13), AllocCache m_nDataType = AFX_RFX_SINGLE (6); SetFieldNull clear -> *plLength = 4.
extern "C" void MS_ABI impl__RFX_Single__YAXPEAVCFieldExchange__PEB_WAEAM_Z(CFieldExchange* pFX, const wchar_t* szName, float* value) {
    RfxScalar<float>(pFX, szName, value, SQL_C_FLOAT, 13,
        [](const float& v) { return v == SinglePseudoNull(); }, [](float& v) { v = SinglePseudoNull(); }, FixupKeep<float>, false);
}

// Symbol: ?RFX_Single_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAMPEAPEA_J@Z
// mfc140u RVA 0x1fe3b0: all four pointers ENSUREd first; new float[m_nRowsetSize]; AfxRFXBulkDefault(..., SQL_C_FLOAT (7), 4).
extern "C" void MS_ABI impl__RFX_Single_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAMPEAPEA_J_Z(CFieldExchange* pFX, const wchar_t* szName, float** prgFltVals, __int64** prgLengths) {
    RfxBulk<float>(pFX, szName, prgFltVals, prgLengths, SQL_C_FLOAT, 4, 1, true, false);
}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WAEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@HHF@Z
// void RFX_Text(CFieldExchange*, LPCTSTR, CStringA&, int nMaxLength, int nColumnType, short nScale)
// mfc140u RVA 0x1ffdb0 -- RfxTextString<char>: SQL_C_CHAR (1), strnlen, byte lengths x1.
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__HHF_Z(CFieldExchange* pFX, const wchar_t* szName, void* value, int nMaxLength, int nColumnType, short nScale) {
    RfxTextString<char>(pFX, szName, value, nMaxLength, nColumnType, nScale, SQL_C_CHAR);
}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HHF@Z
// void RFX_Text(CFieldExchange*, LPCTSTR, CStringW&, int nMaxLength, int nColumnType, short nScale)
// mfc140u RVA 0x1ff5c0 -- RfxTextString<wchar_t>: SQL_C_WCHAR (-8), wcsnlen, byte lengths x2.
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHF_Z(CFieldExchange* pFX, const wchar_t* szName, void* value, int nMaxLength, int nColumnType, short nScale) {
    RfxTextString<wchar_t>(pFX, szName, value, nMaxLength, nColumnType, nScale, SQL_C_WCHAR);
}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WPEADHHF@Z
// void RFX_Text(CFieldExchange*, LPCTSTR, char* value, int nMaxLength, int nColumnType, short nScale)
// mfc140u RVA 0x1ff140 -- RfxTextBuffer over char with SQL_C_CHAR (1).
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEADHHF_Z(CFieldExchange* pFX, const wchar_t* szName, char* value, int nMaxLength, int nColumnType, short nScale) {
    RfxTextBuffer<char>(pFX, szName, value, nMaxLength, nColumnType, nScale, SQL_C_CHAR);
}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WPEA_WHHF@Z
// void RFX_Text(CFieldExchange*, LPCTSTR, wchar_t* value, int nMaxLength, int nColumnType, short nScale)
// mfc140u RVA 0x1feca0 -- RfxTextBuffer over wchar_t with SQL_C_WCHAR (-8).
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEA_WHHF_Z(CFieldExchange* pFX, const wchar_t* szName, wchar_t* value, int nMaxLength, int nColumnType, short nScale) {
    RfxTextBuffer<wchar_t>(pFX, szName, value, nMaxLength, nColumnType, nScale, SQL_C_WCHAR);
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
