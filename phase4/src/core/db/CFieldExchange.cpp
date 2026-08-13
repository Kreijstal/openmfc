// CFieldExchange — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"

// Symbol: ??0CFieldExchange@@QEAA@IPEAVCRecordset@@PEAX@Z
extern "C" void* MS_ABI impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(
    void* pThis, UINT op, CRecordset* pRecordset, void* /*pMapField*/) {
    return new(pThis) CFieldExchange((CFieldExchange::RFX_Operation)op, pRecordset);
}
// Symbol: ?GetLongBinarySize@CFieldExchange@@QEAA_JH@Z
extern "C" long long MS_ABI impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(CFieldExchange* pThis, int nField) {
    if (!pThis || !pThis->m_pRecordset || pThis->m_pRecordset->m_hstmt == SQL_NULL_HSTMT) return 0;

    SQLLEN byteCount = 0;
    RETCODE rc = SQLGetData(
        pThis->m_pRecordset->m_hstmt,
        static_cast<SQLUSMALLINT>(nField),
        SQL_C_BINARY,
        nullptr,
        0,
        &byteCount);
    if (!SqlSucceeded(rc) || byteCount <= 0) return 0;
    return static_cast<long long>(byteCount);
}
// Symbol: ?ReallocLongBinary@CFieldExchange@@QEAAPEAEAEAVCLongBinary@@_J1@Z
extern "C" unsigned char* MS_ABI impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(
    CFieldExchange*, LongBinaryCompat* binary, long long newSize, long long oldSize) {
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
    binary->m_dwDataLength = static_cast<DWORD>(newSize);
    return newData;
}
// Symbol: ?GetLongBinaryData@CFieldExchange@@QEAAXHAEAVCLongBinary@@PEA_J@Z
extern "C" void MS_ABI impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(
    CFieldExchange* pThis, int field, LongBinaryCompat* binary, long long* actualSize) {
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

// === Moved from ManualThunks.cpp ===
// Symbol: ?Default@CFieldExchange@@QEAAXPEB_WPEAXPEA_JH_K3@Z
extern "C" void MS_ABI impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4, void* p5) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
}

// Symbol: ?AfxRFXBulkDefault@@YAXPEAVCFieldExchange@@PEB_WPEAXPEA_JH_K@Z
extern "C" void MS_ABI impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(void* /*class*/* p0, const wchar_t* p1, void* p2, __int64* p3, int p4, unsigned __int64 p5) {}

// Symbol: ?RFX_BigInt@@YAXPEAVCFieldExchange@@PEB_WAEA_J@Z
extern "C" void MS_ABI impl__RFX_BigInt__YAXPEAVCFieldExchange__PEB_WAEA_J_Z(void* /*class*/* p0, const wchar_t* p1, __int64* p2) {}

// Symbol: ?RFX_Binary@@YAXPEAVCFieldExchange@@PEB_WAEAVCByteArray@@_J@Z
extern "C" void MS_ABI impl__RFX_Binary__YAXPEAVCFieldExchange__PEB_WAEAVCByteArray___J_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, __int64 p3) {}

// Symbol: ?RFX_Binary_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAEPEAPEA_JH@Z
extern "C" void MS_ABI impl__RFX_Binary_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_JH_Z(void* /*class*/* p0, const wchar_t* p1, unsigned char** p2, __int64** p3, int p4) {}

// Symbol: ?RFX_Bool@@YAXPEAVCFieldExchange@@PEB_WAEAH@Z
extern "C" void MS_ABI impl__RFX_Bool__YAXPEAVCFieldExchange__PEB_WAEAH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2) {}

// Symbol: ?RFX_Bool_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAHPEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Bool_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, int** p2, __int64** p3) {}

// Symbol: ?RFX_Byte@@YAXPEAVCFieldExchange@@PEB_WAEAE@Z
extern "C" void MS_ABI impl__RFX_Byte__YAXPEAVCFieldExchange__PEB_WAEAE_Z(void* /*class*/* p0, const wchar_t* p1, unsigned char* p2) {}

// Symbol: ?RFX_Byte_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAEPEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Byte_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, unsigned char** p2, __int64** p3) {}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAUtagTIMESTAMP_STRUCT@@@Z
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAUtagTIMESTAMP_STRUCT___Z(void* /*class*/* p0, const wchar_t* p1, void* /*struct*/* p2) {}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAVCOleDateTime@ATL@@@Z
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCOleDateTime_ATL___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2) {}

// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAVCTime@ATL@@@Z
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAVCTime_ATL___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2) {}

// Symbol: ?RFX_Date_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAUtagTIMESTAMP_STRUCT@@PEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Date_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAUtagTIMESTAMP_STRUCT__PEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, void* /*struct*/** p2, __int64** p3) {}

// Symbol: ?RFX_Double@@YAXPEAVCFieldExchange@@PEB_WAEAN@Z
extern "C" void MS_ABI impl__RFX_Double__YAXPEAVCFieldExchange__PEB_WAEAN_Z(void* /*class*/* p0, const wchar_t* p1, double* p2) {}

// Symbol: ?RFX_Double_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEANPEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Double_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEANPEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, double** p2, __int64** p3) {}

// Symbol: ?RFX_Int@@YAXPEAVCFieldExchange@@PEB_WAEAH@Z
extern "C" void MS_ABI impl__RFX_Int__YAXPEAVCFieldExchange__PEB_WAEAH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2) {}

// Symbol: ?RFX_Int_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAHPEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Int_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, int** p2, __int64** p3) {}

// Symbol: ?RFX_Long@@YAXPEAVCFieldExchange@@PEB_WAEAJ@Z
extern "C" void MS_ABI impl__RFX_Long__YAXPEAVCFieldExchange__PEB_WAEAJ_Z(void* /*class*/* p0, const wchar_t* p1, long* p2) {}

// Symbol: ?RFX_LongBinary@@YAXPEAVCFieldExchange@@PEB_WAEAVCLongBinary@@@Z
extern "C" void MS_ABI impl__RFX_LongBinary__YAXPEAVCFieldExchange__PEB_WAEAVCLongBinary___Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2) {}

// Symbol: ?RFX_Long_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAJPEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Long_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAJPEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, long** p2, __int64** p3) {}

// Symbol: ?RFX_Single@@YAXPEAVCFieldExchange@@PEB_WAEAM@Z
extern "C" void MS_ABI impl__RFX_Single__YAXPEAVCFieldExchange__PEB_WAEAM_Z(void* /*class*/* p0, const wchar_t* p1, float* p2) {}

// Symbol: ?RFX_Single_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEAMPEAPEA_J@Z
extern "C" void MS_ABI impl__RFX_Single_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAMPEAPEA_J_Z(void* /*class*/* p0, const wchar_t* p1, float** p2, __int64** p3) {}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WAEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@HHF@Z
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__HHF_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, void** p3, void* p4, int p5, int p6, short p7) {}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HHF@Z
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHF_Z(void* /*class*/* p0, const wchar_t* p1, void* /*class*/* p2, void** p3, void* p4, int p5, int p6, short p7) {}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WPEADHHF@Z
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEADHHF_Z(void* /*class*/* p0, const wchar_t* p1, char* p2, int p3, int p4, short p5) {}

// Symbol: ?RFX_Text@@YAXPEAVCFieldExchange@@PEB_WPEA_WHHF@Z
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEA_WHHF_Z(void* /*class*/* p0, const wchar_t* p1, wchar_t* p2, int p3, int p4, short p5) {}

// Symbol: ?RFX_Text_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEADPEAPEA_JH@Z
extern "C" void MS_ABI impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEADPEAPEA_JH_Z(void* /*class*/* p0, const wchar_t* p1, char** p2, __int64** p3, int p4) {}

// Symbol: ?RFX_Text_Bulk@@YAXPEAVCFieldExchange@@PEB_WPEAPEA_WPEAPEA_JH@Z
extern "C" void MS_ABI impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEA_WPEAPEA_JH_Z(void* /*class*/* p0, const wchar_t* p1, wchar_t** p2, __int64** p3, int p4) {}
