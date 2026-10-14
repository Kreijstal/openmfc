// Bulk RFX buffers and shared column binding/name generation.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstdlib>
#include <cstdint>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(CFieldExchange*,const wchar_t*,void*,SQLLEN*,int,SQLULEN);
namespace {
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
    switch (pFX->m_nOperation) {
    case CFieldExchange::AllocMultiRowBuffer: {
        const size_t nRowsetSize = static_cast<size_t>(static_cast<int32_t>(pFX->m_prs->m_dwRowsetSize));
        uint32_t product = uint32_t(nRowsetSize) * uint32_t(nElemFactor);
        size_t count = static_cast<size_t>(static_cast<int32_t>(product));
        *prgVals = static_cast<T*>(BulkAlloc(count, sizeof(T)));
        *prgLengths = static_cast<__int64*>(BulkAlloc(nRowsetSize, sizeof(__int64)));
        return;
    }
    case CFieldExchange::DeleteMultiRowBuffer:
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
// counted this column).  Name appends the column and the configured separator.
extern "C" void MS_ABI impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(
    CFieldExchange* pFX, const wchar_t* szName, void* pv, __int64* plLength, int nCFType, unsigned __int64 cbValue) {
    if (!pFX) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    if (pFX->m_nOperation == CFieldExchange::BindFieldToColumn) {
        RETCODE rc = ::SQLBindCol(pFX->m_pRecordset->m_hstmt, static_cast<SQLUSMALLINT>(pFX->m_nFields),
                                  static_cast<SQLSMALLINT>(nCFType), pv, static_cast<SQLLEN>(cbValue), plLength);
        if (!pFX->m_pRecordset->Check(rc)) pFX->m_prs->ThrowDBException(rc, SQL_NULL_HSTMT);
    } else if (pFX->m_nOperation == CFieldExchange::Name) {
        if (!szName) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        *pFX->m_pstr += szName;
        *pFX->m_pstr += pFX->m_lpszSeparator;
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
