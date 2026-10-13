// Raw ANSI and Unicode text exchange over native recordset storage.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstdlib>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(CRecordset*,UINT,void*);
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
template <typename CharT>
void RfxTextBuffer(CFieldExchange* pFX, const wchar_t* szName, CharT* value, int nMaxLength, int nColumnType, short nScale, int nCFType) {
    if (!pFX || !szName || !value) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    UINT nField = 0;
    if (!pFX->IsFieldType(&nField)) return;
    CRecordset* prs = pFX->m_pRecordset;
    SQLLEN* plLength = prs->GetFieldLengthBuffer(nField - 1, pFX->m_nFieldType);
    const unsigned long n = nField - 1;
    auto strLen = [](const CharT* s) -> size_t { size_t i = 0; while (s[i]) ++i; return i; };
    switch (pFX->m_nOperation) {
    case CFieldExchange::BindParam:
        // 0x1ff088: *plLength = paramFlags[n] | SQL_NTS; rc = SQLBindParameter(m_hstmt, nField,
        // m_nFieldType, nCFType, nColumnType, nMaxLength, nScale, value, nMaxLength * sizeof(CharT), plLength);
        // if (rc != SQL_SUCCESS) ThrowDBException(rc, m_hstmt); m_mapParamIndex[value] = nField;
        if (!plLength) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        *plLength = static_cast<SQLLEN>(prs->m_pbParamFlags[n]) | SQL_NTS;
        {
            RETCODE rc = ::SQLBindParameter(pFX->m_hstmt, static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(pFX->m_nFieldType),
                                            static_cast<SQLSMALLINT>(nCFType), static_cast<SQLSMALLINT>(nColumnType),
                                            static_cast<SQLULEN>(nMaxLength), nScale, value,
                                            static_cast<SQLLEN>(nMaxLength) * static_cast<SQLLEN>(sizeof(CharT)), plLength);
            if (rc != SQL_SUCCESS) prs->ThrowDBException(rc, pFX->m_hstmt);
        }
        prs->m_mapParamIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    case CFieldExchange::BindFieldToColumn: {
        // 0x1fef71: nLen as RetailTextLen / TextBindLength; value[nLen] = 0;
        // rc = SQLBindCol(m_prs->m_hstmt, nField, nCFType, value, (nLen + 1) * sizeof(CharT), plLength);
        // if (!Check(rc)) ThrowDBException(rc, SQL_NULL_HSTMT); m_mapFieldIndex[value] = nField;
        const auto& info = prs->m_rgODBCFieldInfos[n];
        SQLULEN nLen = RetailTextLen(info.m_nSQLType, info.m_nPrecision);
        if (nLen > static_cast<SQLULEN>(nMaxLength) || nLen < 256)
            nLen = static_cast<SQLULEN>(nMaxLength);
        plLength = prs->GetFieldLengthBuffer(nField - 1, pFX->m_nFieldType);  // native field-info storage retains the bound address
        value[nLen] = 0;
        RETCODE rc = ::SQLBindCol(prs->m_hstmt, static_cast<SQLUSMALLINT>(nField), static_cast<SQLSMALLINT>(nCFType),
                                  value, static_cast<SQLLEN>((nLen + 1) * sizeof(CharT)), plLength);
        if (!prs->Check(rc)) prs->ThrowDBException(rc, SQL_NULL_HSTMT);
        prs->m_mapFieldIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(nField)));
        return;
    }
    case CFieldExchange::Fixup:
        // 0x1feeec: if (*plLength == SQL_NULL_DATA) { SetNullFieldStatus(n); value[0] = 0; }
        // else { if (m_prs->m_pDatabase->m_bStripTrailingSpaces) <strip>; *plLength = strlen(value) * sizeof(CharT); }
        if (plLength && *plLength == SQL_NULL_DATA) {
            prs->SetNullFieldStatus(n);
            value[0] = 0;
        } else if (plLength) {
            // Deviation: no m_bStripTrailingSpaces in this CDatabase (retail default FALSE), nothing stripped.
            *plLength = static_cast<SQLLEN>(strLen(value) * sizeof(CharT));
        }
        return;
    case CFieldExchange::MarkForAddNew:
        // 0x1feead: if (value[0] != 0) { SetDirtyFieldStatus(n); ClearNullFieldStatus(n); }
        if (value[0] != 0) {
            prs->SetDirtyFieldStatus(n);
            prs->ClearNullFieldStatus(n);
        }
        return;
    case CFieldExchange::MarkForUpdate:
        // 0x1fee81: if (value[0] == 0) SetNullFieldStatus(n); else ClearNullFieldStatus(n); then the default call
        if (value[0] == 0) prs->SetNullFieldStatus(n);
        else prs->ClearNullFieldStatus(n);
        break;
    case CFieldExchange::StoreField:
        // 0x1fee21: AfxStoreField(*m_prs, nField, value)
        impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(prs, nField, value);
        return;
    case CFieldExchange::LoadField: {
        // 0x1fedce: m_pbFieldFlags[n] |= m_bStatus; null -> *plLength = SQL_NULL_DATA, else
        // *plLength = strlen(m_pvDataCache) * sizeof(CharT) (retail copies nothing back here).
        const auto& cache = prs->m_rgFieldInfos[n];
        prs->SetFieldStatus(n, cache.m_bStatus);
        *plLength = prs->IsFieldStatusNull(n) ? SQL_NULL_DATA :
            static_cast<SQLLEN>(static_cast<int>(cache.m_pvDataCache ?
                strLen(static_cast<const CharT*>(cache.m_pvDataCache)) : 0)) * sizeof(CharT);
        return;
    }
    case CFieldExchange::SetFieldNull:
        if ((!pFX->m_pvField && pFX->m_nFieldType == 0) || pFX->m_pvField == value) {
            if (pFX->m_bField) {
                prs->SetNullFieldStatus(n);
                *plLength = SQL_NULL_DATA;
            } else {
                prs->ClearNullFieldStatus(n);
                *plLength = SQL_NTS;
            }
        }
        return;
    case CFieldExchange::AllocCache: {
        size_t bytes = static_cast<size_t>(nMaxLength);
        if (bytes > static_cast<size_t>(-1) / sizeof(CharT)) {
            impl__AfxThrowMemoryException__YAXXZ(); return;
        }
        void* cache = std::malloc(bytes * sizeof(CharT));
        if (!cache) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        prs->m_rgFieldInfos[n].m_pvDataCache = cache;
        prs->m_rgFieldInfos[n].m_nDataType = sizeof(CharT) == 1 ? 19 : 18;
        return;
    }
    default:
        break;
    }
    // 0x1fed58: Default(szName, value, plLength, nCFType, strlen(value) * sizeof(CharT), nMaxLength)
    pFX->Default(szName, value, plLength, nCFType,
        static_cast<unsigned __int64>(strLen(value) * sizeof(CharT)), static_cast<unsigned __int64>(nMaxLength));
}

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

