// Native ANSI and Unicode CString field exchange, including locked buffers.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "detail/RFXTextFixup.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(CRecordset*,UINT,void*);
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(CRecordset*,UINT,void*,SQLLEN*);
namespace {
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
SQLULEN TextBindLength(CFieldExchange* fx,UINT field,SQLULEN maximum) {
    const auto& info=fx->m_prs->m_rgODBCFieldInfos[field-1];
    SQLULEN size=RetailTextLen(info.m_nSQLType,info.m_nPrecision);
    return size>maximum || size<256 ? maximum:size;
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
    static IAtlStringMgr* Mgr(CStringData* pd) { return pd->pStringMgr; }
    static bool Shared(const CStringData* pd) { return pd->nRefs > 1; }

    // Release every owned reference, including shared empty-string references.
    static void ReleaseData(CStringData* pd) {
        if (pd->Release()) Mgr(pd)->Free(pd);
    }
    // CSimpleStringT::Fork(nLength): allocate nLength, copy min(old, new) + NUL,
    // keep nDataLength, release the old block.
    void Fork(int nLength) {
        CStringData* pOld = Data();
        const int nOldLength = pOld->nDataLength;
        CStringData* pNew = Mgr(pOld)->Clone()->Allocate(nLength, sizeof(CH));
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
            int capacity = pOld->nAllocLength;
            int extra = capacity > 1024 * 1024 * 1024 ? 1024 * 1024 : capacity / 2;
            int nNew = static_cast<int32_t>(static_cast<uint32_t>(capacity) + static_cast<uint32_t>(extra));
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

template <class CH>
void RfxTextString(CFieldExchange* pFX, const wchar_t* szName, void* pString, int nMaxLength,
                   int nColumnType, short nScale, int nCFType) {
    if (!pFX || !szName || !pString) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    RfxStr<CH> value(pString);
    CRecordset* prs = pFX->m_prs;
    SQLLEN* plLength = prs->GetFieldLengthBuffer(nField - 1, pFX->m_nFieldType);
    const unsigned long n = nField - 1;
    switch (pFX->m_nOperation) {
    case CFieldExchange::BindParam: {
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
        *plLength = SQLLEN(prs->m_pbParamFlags[n]) | SQL_NTS;
        RETCODE rc = ::SQLBindParameter(pFX->m_hstmt, static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(pFX->m_nFieldType),
                                        static_cast<SQLSMALLINT>(nCFType), static_cast<SQLSMALLINT>(nColumnType),
                                        static_cast<SQLULEN>(nMaxLength), nScale, buffer,
                                        static_cast<SQLLEN>(nMaxLength) * static_cast<SQLLEN>(sizeof(CH)), plLength);
        value.ReleaseBuffer();
        if (rc != SQL_SUCCESS) prs->ThrowDBException(rc, pFX->m_hstmt);
        prs->m_mapParamIndex.SetAt(pString,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    }
    case CFieldExchange::BindFieldToColumn: {
        // 0x1ffa7e: nLen from the column description (RetailTextLen / TextBindLength);
        // if (nLen > 0x7ffffffe) AfxThrowMemoryException(); value.GetBufferSetLength(nLen + 1);
        // value.LockBuffer(); rc = SQLBindCol(m_prs->m_hstmt, nField, nCFType, buffer,
        // (nLen + 1) * sizeof(CH), plLength); value.ReleaseBuffer();
        // if (!Check(rc)) ThrowDBException(rc, SQL_NULL_HSTMT); m_mapFieldIndex[&value] = nField;
        SQLULEN nLen = TextBindLength(pFX, nField, static_cast<SQLULEN>(nMaxLength));
        plLength = prs->GetFieldLengthBuffer(nField - 1, pFX->m_nFieldType);  // native field-info storage retains the bound address
        if (nLen > 0x7ffffffeULL) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        CH* buffer = value.GetBufferSetLength(static_cast<int>(nLen) + 1);
        value.LockBuffer();
        RETCODE rc = ::SQLBindCol(prs->m_hstmt, static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(nCFType),
                                  buffer, static_cast<SQLLEN>((nLen + 1) * sizeof(CH)), plLength);
        value.ReleaseBuffer();
        if (!prs->Check(rc)) prs->ThrowDBException(rc, SQL_NULL_HSTMT);
        prs->m_mapFieldIndex.SetAt(pString,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    }
    case CFieldExchange::Fixup:
        // 0x1ff933: if (*plLength == SQL_NULL_DATA) { SetNullFieldStatus(n); value.GetBufferSetLength(0);
        // value.ReleaseBuffer(); } else { value.GetBuffer(0); if (m_prs->m_pDatabase->m_bStripTrailingSpaces)
        // <cut the string at its last run of ' '>; value.ReleaseBuffer(); *plLength = value.GetLength() * sizeof(CH); }
        // The buffer must stay the one SQLBindCol holds, so it is only re-terminated, never reallocated.
        if (plLength && *plLength == SQL_NULL_DATA) {
            prs->SetNullFieldStatus(n);
            value.GetBufferSetLength(0);
            value.ReleaseBuffer();
        } else {
            CH* buffer = value.GetBuffer(0);
            if (prs->m_pDatabase->m_bStripTrailingSpaces)
                openmfc::detail::StripTrailingTextSpaces(buffer);
            value.ReleaseBuffer();
            if (plLength) *plLength = static_cast<SQLLEN>(value.Length()) * static_cast<SQLLEN>(sizeof(CH));
        }
        return;
    case CFieldExchange::MarkForAddNew:
        // 0x1ff8ec: if (value.GetLength() != 0) { SetDirtyFieldStatus(n); ClearNullFieldStatus(n); }
        if (value.Length() != 0) {
            prs->SetDirtyFieldStatus(n);
            prs->ClearNullFieldStatus(n);
        }
        return;
    case CFieldExchange::MarkForUpdate:
        // 0x1ff893: if (value.GetLength() == 0) SetNullFieldStatus(n); else ClearNullFieldStatus(n);
        // Default(szName, &value /* the CString object */, plLength, nCFType, value.GetLength() * sizeof(CH), nMaxLength);
        if (value.Length() == 0) prs->SetNullFieldStatus(n);
        else prs->ClearNullFieldStatus(n);
        pFX->Default( szName, pString, plLength, nCFType,
            static_cast<unsigned __int64>(value.Length()) * sizeof(CH), static_cast<unsigned __int64>(nMaxLength));
        return;
    case CFieldExchange::StoreField:
        // 0x1ff7c2: AfxStoreField(*m_prs, nField, &value)
        impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(prs, nField, pString);
        return;
    case CFieldExchange::LoadField:
        // 0x1ff767: m_pbFieldFlags[n] |= m_rgFieldInfos[n].m_bStatus; if null: *plLength = SQL_NULL_DATA;
        // else { value = *(CStringT*)m_rgFieldInfos[n].m_pvDataCache; *plLength = value.GetLength() * sizeof(CH); }
        impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(prs, nField, pString, plLength);
        return;
    case CFieldExchange::SetFieldNull:
        if ((!pFX->m_pvField && pFX->m_nFieldType==0) || pFX->m_pvField==pString) {
            if (pFX->m_bField) {
                prs->SetNullFieldStatus(n);
                value.GetBufferSetLength(0);value.ReleaseBuffer();
                *plLength=SQL_NULL_DATA;
            } else {
                prs->ClearNullFieldStatus(n);*plLength=SQL_NTS;
            }
        }
        return;
    case CFieldExchange::AllocCache: {
        auto* cache=static_cast<void**>(std::malloc(sizeof(void*)));
        if(!cache) {impl__AfxThrowMemoryException__YAXXZ();return;}
        *cache=OpenMFC_GetStringMgr()->GetNilString()+1;
        prs->m_rgFieldInfos[n].m_pvDataCache=cache;
        prs->m_rgFieldInfos[n].m_nDataType=sizeof(CH)==2 ? 20:21;
        return;
    }
    default: {
        // 0x1ff67e: cbValue = value.GetLength() * sizeof(CH); value.GetBuffer(0);
        // Default(szName, buffer, plLength, nCFType, cbValue, nMaxLength); value.ReleaseBuffer();
        const unsigned __int64 cbValue = static_cast<unsigned __int64>(value.Length()) * sizeof(CH);
        CH* buffer = value.GetBuffer(0);
        pFX->Default( szName, buffer, plLength, nCFType, cbValue,
                                                                 static_cast<unsigned __int64>(nMaxLength));
        value.ReleaseBuffer();
        return;
    }
    }
}

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

