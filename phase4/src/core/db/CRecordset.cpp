// CRecordset — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"

// Symbol: ??0CRecordset@@QEAA@PEAVCDatabase@@@Z
extern "C" void* MS_ABI impl___0CRecordset__QEAA_PEAVCDatabase___Z(void* pThis, CDatabase* pDatabase) {
    return new(pThis) CRecordset(pDatabase);
}
// Symbol: ?GetRuntimeClass@CRecordset@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRecordset__UEBAPEAUCRuntimeClass__XZ(const CRecordset* pThis) {
    (void)pThis;
    return CRecordset::GetThisClass();
}
// Symbol: ?GetThisClass@CRecordset@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRecordset__SAPEAUCRuntimeClass__XZ() {
    return CRecordset::GetThisClass();
}
// Symbol: ?IsOpen@CRecordset@@QEBAHXZ
extern "C" int MS_ABI impl__IsOpen_CRecordset__QEBAHXZ(const CRecordset* pThis) {
    return (pThis && pThis->IsOpen()) ? TRUE : FALSE;
}
// Symbol: ?Requery@CRecordset@@UEAAHXZ
extern "C" int MS_ABI impl__Requery_CRecordset__UEAAHXZ(CRecordset* pThis) {
    if (!pThis || !pThis->IsOpen()) return FALSE;
    RETCODE rc = SQLCloseCursor(pThis->GetHSTMT());
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        return FALSE;
    }

    rc = SQLExecute(pThis->GetHSTMT());
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        return TRUE;
    }

    CString sql;
    {
        std::lock_guard<std::mutex> lock(g_recordsetStateMutex);
        auto it = g_recordsetSql.find(pThis);
        if (it != g_recordsetSql.end()) {
            sql = it->second;
        }
    }
    if (sql.IsEmpty()) {
        sql = pThis->GetDefaultSQL();
    }
    if (sql.IsEmpty()) {
        return FALSE;
    }
    rc = SQLExecDirectW(pThis->GetHSTMT(), (SQLWCHAR*)(const wchar_t*)sql, SQL_NTS);
    return (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) ? TRUE : FALSE;
}
// Symbol: ?GetDefaultFieldType@CRecordset@@SAFF@Z
extern "C" short MS_ABI impl__GetDefaultFieldType_CRecordset__SAFF_Z(short nSQLType) {
    switch (nSQLType) {
    case SQL_CHAR:
    case SQL_VARCHAR:
    case SQL_LONGVARCHAR:
        return SQL_C_CHAR;
    case SQL_WCHAR:
    case SQL_WVARCHAR:
    case SQL_WLONGVARCHAR:
        return SQL_C_WCHAR;
    case SQL_SMALLINT:
        return SQL_C_SSHORT;
    case SQL_INTEGER:
        return SQL_C_SLONG;
    case SQL_REAL:
        return SQL_C_FLOAT;
    case SQL_DOUBLE:
    case SQL_FLOAT:
        return SQL_C_DOUBLE;
    case SQL_BIT:
        return SQL_C_BIT;
    case SQL_DECIMAL:
    case SQL_NUMERIC:
        return SQL_C_CHAR;
    case SQL_TYPE_DATE:
    case SQL_DATE:
        return SQL_C_TYPE_DATE;
    case SQL_TYPE_TIME:
    case SQL_TIME:
        return SQL_C_TYPE_TIME;
    case SQL_TYPE_TIMESTAMP:
    case SQL_TIMESTAMP:
        return SQL_C_TYPE_TIMESTAMP;
    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
        return SQL_C_BINARY;
    default:
        return nSQLType;
    }
}
// Symbol: ?AllocHstmt@CRecordset@@IEAAHXZ
extern "C" int MS_ABI impl__AllocHstmt_CRecordset__IEAAHXZ(CRecordset* pThis) {
    if (!pThis || !pThis->m_pDatabase || !pThis->m_pDatabase->IsOpen()) return FALSE;
    if (pThis->m_hstmt != SQL_NULL_HSTMT) return TRUE;
    return SqlSucceeded(SQLAllocHandle(SQL_HANDLE_STMT, pThis->m_pDatabase->GetHDBC(), &pThis->m_hstmt)) ? TRUE : FALSE;
}
// Symbol: ?AllocAndCacheFieldInfo@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__AllocAndCacheFieldInfo_CRecordset__IEAAXXZ(CRecordset* pThis) { LoadRecordsetFields(pThis); }
// Symbol: ?AllocDataCache@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__AllocDataCache_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).dataCache.resize(4096); }
// Symbol: ?FreeDataCache@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__FreeDataCache_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).dataCache.clear(); }
// Symbol: ?AllocRowset@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__AllocRowset_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).rowsetAllocated = true; }
// Symbol: ?FreeRowset@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__FreeRowset_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).rowsetAllocated = false; }
// Symbol: ?AllocStatusArrays@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__AllocStatusArrays_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).fieldLengths.resize(static_cast<size_t>(std::max(0, pThis->m_nFields))); }
// Symbol: ?AppendFilterAndSortSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__AppendFilterAndSortSQL_CRecordset__IEAAXXZ(CRecordset* pThis) {
    if (!pThis) return;
    RecordsetState& state = EnsureRecordsetState(pThis);
    CString sql = state.currentSql.IsEmpty() ? pThis->GetDefaultSQL() : state.currentSql;
    if (!pThis->m_strFilter.IsEmpty()) {
        sql += (LowerSql(static_cast<const wchar_t*>(sql)).find(L" where ") == std::wstring::npos) ? L" WHERE " : L" AND ";
        sql += pThis->m_strFilter;
    }
    if (!pThis->m_strSort.IsEmpty()) { sql += L" ORDER BY "; sql += pThis->m_strSort; }
    StoreSql(pThis, sql);
}
// Symbol: ?AppendNames@CRecordset@@QEAAIPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" unsigned int MS_ABI impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*, CString* sql, const wchar_t* name) { return AppendCsvName(sql, name); }
// Symbol: ?AppendValues@CRecordset@@QEAAIPEAXPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" unsigned int MS_ABI impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*, void*, CString* sql, const wchar_t* value) { return AppendCsvName(sql, value ? value : L"?"); }
// Symbol: ?AppendNamesValues@CRecordset@@QEAAIPEAXPEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" unsigned int MS_ABI impl__AppendNamesValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*, void*, CString* sql, const wchar_t* name) {
    if (!sql || !name) return 0;
    if (!sql->IsEmpty()) *sql += L", ";
    *sql += name; *sql += L"=?";
    return 1;
}
// Symbol: ?BuildSelectSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__BuildSelectSQL_CRecordset__IEAAXXZ(CRecordset* pThis) {
    if (!pThis) return;
    CString table = pThis->GetDefaultSQL();
    CString sql = table;
    if (!table.IsEmpty() && LowerSql(static_cast<const wchar_t*>(table)).find(L"select") != 0) sql = CString(L"SELECT * FROM ") + table;
    StoreSql(pThis, sql);
}
// Symbol: ?BuildSQL@CRecordset@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__BuildSQL_CRecordset__IEAAXPEB_W_Z(CRecordset* pThis, const wchar_t* sql) { if (pThis) StoreSql(pThis, CString(sql ? sql : L"")); }
// Symbol: ?BuildUpdateSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__BuildUpdateSQL_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared = true; }
// Symbol: ?BindFieldsForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__BindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared = true; }
// Symbol: ?UnbindFieldsForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared = false; }
// Symbol: ?BindFieldsToColumns@CRecordset@@QEAAIXZ
extern "C" unsigned int MS_ABI impl__BindFieldsToColumns_CRecordset__QEAAIXZ(CRecordset* pThis) { LoadRecordsetFields(pThis); return pThis ? static_cast<unsigned int>(EnsureRecordsetState(pThis).fields.size()) : 0; }
// Symbol: ?BindParams@CRecordset@@QEAAIPEAX@Z
extern "C" unsigned int MS_ABI impl__BindParams_CRecordset__QEAAIPEAX_Z(CRecordset* pThis, void* param) {
    if (!pThis || !param) return 0;
    auto& map = EnsureRecordsetState(pThis).boundParamIndexes;
    map[param] = static_cast<int>(map.size());
    return static_cast<unsigned int>(map[param]);
}
// Symbol: ?RebindParams@CRecordset@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__RebindParams_CRecordset__QEAAXPEAX_Z(CRecordset* pThis, void* param) { (void)impl__BindParams_CRecordset__QEAAIPEAX_Z(pThis, param); }
// Symbol: ?CanBookmark@CRecordset@@QEBAHXZ
extern "C" int MS_ABI impl__CanBookmark_CRecordset__QEBAHXZ(const CRecordset* pThis) { auto it = pThis ? g_recordsetStates.find(pThis) : g_recordsetStates.end(); return it != g_recordsetStates.end() && it->second.bookmarksEnabled ? TRUE : FALSE; }
// Symbol: ?EnableBookmarks@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__EnableBookmarks_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).bookmarksEnabled = true; }
// Symbol: ?Cancel@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__Cancel_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis && pThis->m_hstmt != SQL_NULL_HSTMT) SQLCancel(pThis->m_hstmt); }
// Symbol: ?Check@CRecordset@@UEBAHF@Z
extern "C" int MS_ABI impl__Check_CRecordset__UEBAHF_Z(const CRecordset* pThis, short retCode) { if (pThis) g_recordsetStates[pThis].lastRetCode = retCode; return SqlSucceeded(retCode) ? TRUE : FALSE; }
// Symbol: ?CheckRowsetCurrencyStatus@CRecordset@@QEAAXGJ@Z
extern "C" void MS_ABI impl__CheckRowsetCurrencyStatus_CRecordset__QEAAXGJ_Z(CRecordset* pThis, unsigned short fetchType, long row) {
    if (!pThis) return;
    RecordsetState& state = EnsureRecordsetState(pThis);
    state.rowsetAllocated = true;
    state.bookmark = static_cast<unsigned long>(row);

    if (pThis->m_hstmt != SQL_NULL_HSTMT) {
        RETCODE rc = SQLFetchScroll(pThis->m_hstmt, fetchType, row);
        state.lastRetCode = rc;
        if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
            pThis->m_bBOF = (row <= 0 ? TRUE : FALSE);
            pThis->m_bEOF = FALSE;
            pThis->m_nAbsolutePosition = (row == 0 ? 0 : row);
        } else if (rc == SQL_NO_DATA) {
            pThis->m_bEOF = TRUE;
        }
    } else if (row < 0) {
        pThis->m_bBOF = TRUE;
        pThis->m_bEOF = TRUE;
    }
}
// Symbol: ?CheckRowsetError@CRecordset@@UEAAXF@Z
extern "C" void MS_ABI impl__CheckRowsetError_CRecordset__UEAAXF_Z(CRecordset* pThis, short retCode) { if (pThis) EnsureRecordsetState(pThis).lastRetCode = retCode; }
// Symbol: ?ClearFieldStatus@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__ClearFieldStatus_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) { EnsureRecordsetState(pThis).dirtyFields.clear(); EnsureRecordsetState(pThis).nullFields.clear(); } }
// Symbol: ?ClearNullFieldStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long field) { if (pThis) EnsureRecordsetState(pThis).nullFields.erase(field); }
// Symbol: ?ClearNullParamStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__ClearNullParamStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long param) { if (pThis) EnsureRecordsetState(pThis).nullParams.erase(param); }
// Symbol: ?SetNullFieldStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long field) { if (pThis) EnsureRecordsetState(pThis).nullFields.insert(field); }
// Symbol: ?SetNullParamStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__SetNullParamStatus_CRecordset__QEAAXK_Z(CRecordset* pThis, unsigned long param) { if (pThis) EnsureRecordsetState(pThis).nullParams.insert(param); }
// Symbol: ?SetFieldStatus@CRecordset@@QEAAXKE@Z
extern "C" void MS_ABI impl__SetFieldStatus_CRecordset__QEAAXKE_Z(CRecordset* pThis, unsigned long field, unsigned char status) { if (pThis) { if (status) EnsureRecordsetState(pThis).dirtyFields.insert(field); else EnsureRecordsetState(pThis).dirtyFields.erase(field); } }
// Symbol: ?GetFieldStatus@CRecordset@@QEAAEK@Z
extern "C" unsigned char MS_ABI impl__GetFieldStatus_CRecordset__QEAAEK_Z(CRecordset* pThis, unsigned long field) { if (!pThis) return 0; auto& s=EnsureRecordsetState(pThis); return static_cast<unsigned char>((s.dirtyFields.count(field)?1:0)|(s.nullFields.count(field)?2:0)); }
// Symbol: ?IsFieldStatusDirty@CRecordset@@QEBAHK@Z
extern "C" int MS_ABI impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(const CRecordset* pThis, unsigned long field) { return pThis && g_recordsetStates[pThis].dirtyFields.count(field) ? TRUE : FALSE; }
// Symbol: ?IsFieldStatusNull@CRecordset@@QEBAHK@Z
extern "C" int MS_ABI impl__IsFieldStatusNull_CRecordset__QEBAHK_Z(const CRecordset* pThis, unsigned long field) { return pThis && g_recordsetStates[pThis].nullFields.count(field) ? TRUE : FALSE; }
// Symbol: ?IsParamStatusNull@CRecordset@@QEBAHK@Z
extern "C" int MS_ABI impl__IsParamStatusNull_CRecordset__QEBAHK_Z(const CRecordset* pThis, unsigned long param) { return pThis && g_recordsetStates[pThis].nullParams.count(param) ? TRUE : FALSE; }
// Symbol: ?DoBulkFieldExchange@CRecordset@@UEAAXPEAVCFieldExchange@@@Z
extern "C" void MS_ABI impl__DoBulkFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z(CRecordset* pThis, CFieldExchange* fx) { if (pThis) pThis->DoFieldExchange(fx); }
// Symbol: ?ExecuteSetPosUpdate@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) pThis->Update(); }
// Symbol: ?ExecuteUpdateSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis && pThis->m_pDatabase) { CString sql = GetStoredSql(pThis); if (!sql.IsEmpty()) pThis->m_pDatabase->ExecuteSQL(static_cast<const wchar_t*>(sql)); } }
// Symbol: ?FetchData@CRecordset@@QEAAFGJPEA_K@Z
extern "C" short MS_ABI impl__FetchData_CRecordset__QEAAFGJPEA_K_Z(CRecordset* pThis, unsigned short fetchType, long row, unsigned long long* rowsFetched) {
    if (rowsFetched) *rowsFetched = 0;
    if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT) return SQL_ERROR;
    RETCODE rc = SQLFetchScroll(pThis->m_hstmt, fetchType, row);
    if (SqlSucceeded(rc) && rowsFetched) *rowsFetched = 1;
    return rc;
}
// Symbol: ?FindSQLToken@CRecordset@@SAPEB_WPEB_W0@Z
extern "C" const wchar_t* MS_ABI impl__FindSQLToken_CRecordset__SAPEB_WPEB_W0_Z(const wchar_t* sql, const wchar_t* token) { if (!sql || !token) return nullptr; auto s=LowerSql(sql); auto t=LowerSql(token); auto pos=s.find(t); return pos==std::wstring::npos?nullptr:sql+pos; }
// Symbol: ?Fixups@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__Fixups_CRecordset__QEAAXXZ(CRecordset* pThis) {
    if (!pThis) return;
    LoadRecordsetFields(pThis);
    RecordsetState& state = EnsureRecordsetState(pThis);
    if (state.fieldLengths.size() < static_cast<size_t>(pThis->m_nFields)) {
        state.fieldLengths.resize(static_cast<size_t>(pThis->m_nFields), 0);
    } else {
        for (auto& len : state.fieldLengths) len = 0;
    }
    if (pThis->m_bDeleted) state.dirtyFields.clear();
    state.dataCache.clear();
    if (pThis->m_nFields > 0) {
        state.dataCache.resize(static_cast<size_t>(pThis->m_nFields) * 32, 0);
    }
}
// Symbol: ?FlushResultSet@CRecordset@@QEAAHXZ
extern "C" int MS_ABI impl__FlushResultSet_CRecordset__QEAAHXZ(CRecordset* pThis) { if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT) return FALSE; while (SQLMoreResults(pThis->m_hstmt) == SQL_SUCCESS) {} return TRUE; }
// Symbol: ?GetBookmark@CRecordset@@QEAAXAEAVCDBVariant@@@Z
extern "C" void MS_ABI impl__GetBookmark_CRecordset__QEAAXAEAVCDBVariant___Z(CRecordset*, void* variant) { if (variant) std::memset(variant, 0, 32); }
// Symbol: ?SetBookmark@CRecordset@@QEAAXAEBVCDBVariant@@@Z
extern "C" void MS_ABI impl__SetBookmark_CRecordset__QEAAXAEBVCDBVariant___Z(CRecordset* pThis, const void*) { if (pThis) EnsureRecordsetState(pThis).bookmarksEnabled = true; }
// Symbol: ?GetBoundFieldIndex@CRecordset@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__GetBoundFieldIndex_CRecordset__QEAAHPEAX_Z(CRecordset* pThis, void* field) { if (!pThis || !field) return -1; auto& m=EnsureRecordsetState(pThis).boundFieldIndexes; auto it=m.find(field); return it==m.end()?-1:it->second; }
// Symbol: ?GetBoundParamIndex@CRecordset@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__GetBoundParamIndex_CRecordset__QEAAHPEAX_Z(CRecordset* pThis, void* param) { if (!pThis || !param) return -1; auto& m=EnsureRecordsetState(pThis).boundParamIndexes; auto it=m.find(param); return it==m.end()?-1:it->second; }
// Symbol: ?GetData@CRecordset@@SA_JPEAVCDatabase@@PEAXFF1_JF@Z
extern "C" long long MS_ABI impl__GetData_CRecordset__SA_JPEAVCDatabase__PEAXFF1_JF_Z(CDatabase*, void* hstmt, short field, short cType, void* buffer, long long bufferLen, short) { if (!hstmt || !buffer || bufferLen <= 0) return SQL_ERROR; SQLLEN ind=0; RETCODE rc=SQLGetData(static_cast<HSTMT>(hstmt), static_cast<SQLUSMALLINT>(field), cType, buffer, static_cast<SQLLEN>(bufferLen), &ind); return SqlSucceeded(rc)?ind:rc; }
// Symbol: ?GetDataBuffer@CRecordset@@SAPEAXAEAVCDBVariant@@FPEA_JF_K@Z
extern "C" void* MS_ABI impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z(void* variant, short, long long* length, short, unsigned long) { if (length) *length = 0; return variant; }
// Symbol: ?GetDefaultConnect@CRecordset@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDefaultConnect_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CString* ret, CRecordset* pThis) { new (ret) CString(pThis ? pThis->GetDefaultConnect() : CString(L"ODBC;")); }
// Symbol: ?GetDefaultSQL@CRecordset@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDefaultSQL_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CString* ret, CRecordset* pThis) { new (ret) CString(pThis ? pThis->GetDefaultSQL() : CString()); }
// Symbol: ?GetFieldIndexByName@CRecordset@@QEAAFPEB_W@Z
extern "C" short MS_ABI impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z(CRecordset* pThis, const wchar_t* name) { int i=FindFieldByName(pThis,name); return i<0?static_cast<short>(-1):static_cast<short>(i); }
// Symbol: ?GetFieldLengthBuffer@CRecordset@@QEAAPEA_JKH@Z
extern "C" long long* MS_ABI impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z(CRecordset* pThis, unsigned long field, int grow) { if (!pThis) return nullptr; auto& s=EnsureRecordsetState(pThis); if (field>=s.fieldLengths.size()&&grow) s.fieldLengths.resize(field+1); return field<s.fieldLengths.size()?reinterpret_cast<long long*>(&s.fieldLengths[field]):nullptr; }
// Symbol: ?GetFieldValue@CRecordset@@QEAAXFAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CRecordset* pThis, short index, CString* value) { GetFieldTextByIndex(pThis,index,value); }
// Symbol: ?GetFieldValue@CRecordset@@QEAAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CRecordset* pThis, const wchar_t* name, CString* value) { GetFieldTextByIndex(pThis, static_cast<short>(FindFieldByName(pThis,name)), value); }
// Symbol: ?GetFieldValue@CRecordset@@QEAAXFAEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(CRecordset*, short, void* value) { if (value) *reinterpret_cast<char**>(value) = _strdup(""); }
// Symbol: ?GetFieldValue@CRecordset@@QEAAXPEB_WAEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(CRecordset* pThis, const wchar_t* name, void* value) { impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(pThis, static_cast<short>(FindFieldByName(pThis,name)), value); }
// Symbol: ?GetFieldValue@CRecordset@@QEAAXFAEAVCDBVariant@@F@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAVCDBVariant__F_Z(CRecordset*, short, void* variant, short) { if (variant) std::memset(variant, 0, 32); }
// Symbol: ?GetFieldValue@CRecordset@@QEAAXPEB_WAEAVCDBVariant@@F@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAVCDBVariant__F_Z(CRecordset*, const wchar_t*, void* variant, short) { if (variant) std::memset(variant, 0, 32); }
// Symbol: ?GetLBFetchSize@CRecordset@@MEAA_J_J@Z
extern "C" long long MS_ABI impl__GetLBFetchSize_CRecordset__MEAA_J_J_Z(CRecordset*, long long size) { return size > 0 ? std::min<long long>(size, 65536) : 4096; }
// Symbol: ?GetLBReallocSize@CRecordset@@MEAA_J_J@Z
extern "C" long long MS_ABI impl__GetLBReallocSize_CRecordset__MEAA_J_J_Z(CRecordset*, long long size) { return size > 0 ? size * 2 : 4096; }
// Symbol: ?GetLongBinaryDataAndCleanup@CRecordset@@SAXPEAVCDatabase@@PEAXF_JPEAPEAX2AEAVCDBVariant@@F@Z
extern "C" void MS_ABI impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z(CDatabase*, void*, short, long long, void** data, void*, void* variant, short) { if (data) *data=nullptr; if (variant) std::memset(variant,0,32); }
// Symbol: ?GetLongCharDataAndCleanup@CRecordset@@SAXPEAVCDatabase@@PEAXF_JPEAPEAX2AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@FF@Z
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z(CDatabase*, void*, short, long long, void** data, void*, CString* value, short, short) { if (data) *data=nullptr; if (value) *value=L""; }
// Symbol: ?GetLongCharDataAndCleanup@CRecordset@@SAXPEAVCDatabase@@PEAXF_JPEAPEAX2AEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@FF@Z
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z(CDatabase*, void*, short, long long, void** data, void*, void* value, short, short) { if (data) *data=nullptr; if (value) *reinterpret_cast<char**>(value)=_strdup(""); }
// Symbol: ?GetODBCFieldInfo@CRecordset@@QEAAXFAEAUCODBCFieldInfo@@@Z
extern "C" void MS_ABI impl__GetODBCFieldInfo_CRecordset__QEAAXFAEAUCODBCFieldInfo___Z(CRecordset* pThis, short index, OdbcFieldInfoCompat* info) { if (!pThis||!info) return; LoadRecordsetFields(pThis); auto& f=EnsureRecordsetState(pThis).fields; if (index>=0 && static_cast<size_t>(index)<f.size()) *info=f[index]; }
// Symbol: ?GetODBCFieldInfo@CRecordset@@QEAAXPEB_WAEAUCODBCFieldInfo@@@Z
extern "C" void MS_ABI impl__GetODBCFieldInfo_CRecordset__QEAAXPEB_WAEAUCODBCFieldInfo___Z(CRecordset* pThis, const wchar_t* name, OdbcFieldInfoCompat* info) { impl__GetODBCFieldInfo_CRecordset__QEAAXFAEAUCODBCFieldInfo___Z(pThis, static_cast<short>(FindFieldByName(pThis,name)), info); }
// Symbol: ?GetTextLen@CRecordset@@SA_JF_K@Z
extern "C" long long MS_ABI impl__GetTextLen_CRecordset__SA_JF_K_Z(short sqlType, unsigned long precision) { return (sqlType==SQL_WCHAR||sqlType==SQL_WVARCHAR||sqlType==SQL_WLONGVARCHAR)?static_cast<long long>(precision)*sizeof(wchar_t):precision; }
// Symbol: ?InitRecord@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__InitRecord_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) { pThis->m_bDeleted=FALSE; pThis->m_nEditMode=0; } }
// Symbol: ?IsJoin@CRecordset@@SAHPEB_W@Z
extern "C" int MS_ABI impl__IsJoin_CRecordset__SAHPEB_W_Z(const wchar_t* sql) { return LowerSql(sql).find(L" join ") != std::wstring::npos ? TRUE : FALSE; }
// Symbol: ?IsRecordsetUpdatable@CRecordset@@IEAAHXZ
extern "C" int MS_ABI impl__IsRecordsetUpdatable_CRecordset__IEAAHXZ(CRecordset* pThis) { return pThis && pThis->CanUpdate() ? TRUE : FALSE; }
// Symbol: ?IsSelectQueryUpdatable@CRecordset@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__IsSelectQueryUpdatable_CRecordset__QEAAHPEB_W_Z(CRecordset*, const wchar_t* sql) { auto s=LowerSql(sql); return s.find(L"select")==0 && s.find(L" join ")==std::wstring::npos ? TRUE : FALSE; }
// Symbol: ?IsSQLUpdatable@CRecordset@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__IsSQLUpdatable_CRecordset__QEAAHPEB_W_Z(CRecordset* pThis, const wchar_t* sql) { return impl__IsSelectQueryUpdatable_CRecordset__QEAAHPEB_W_Z(pThis, sql); }
// Symbol: ?LoadFields@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__LoadFields_CRecordset__QEAAXXZ(CRecordset* pThis) { LoadRecordsetFields(pThis); }
// Symbol: ?MarkForAddNew@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__MarkForAddNew_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) pThis->AddNew(); }
// Symbol: ?MarkForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__MarkForUpdate_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) pThis->Edit(); }
// Symbol: ?OnSetUpdateOptions@CRecordset@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z(CRecordset* pThis, void* pUpdateOptions) {
    if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT) return;
    RecordsetState& state = EnsureRecordsetState(pThis);
    state.updatePrepared = TRUE;
    if (!pUpdateOptions) return;
    auto options = static_cast<SQLUINTEGER*>(pUpdateOptions);
        state.lockingMode = static_cast<unsigned int>(*options);
    if (pThis->m_hstmt != SQL_NULL_HSTMT) {
        SQLSetStmtAttr(pThis->m_hstmt, SQL_ATTR_CURSOR_TYPE,
                       *options == 0 ? CastOdbcOptionValue(SQL_CURSOR_FORWARD_ONLY)
                                     : CastOdbcOptionValue(SQL_CURSOR_STATIC),
                       0);
    }
}
// Symbol: ?PreBindFields@CRecordset@@UEAAXXZ
extern "C" void MS_ABI impl__PreBindFields_CRecordset__UEAAXXZ(CRecordset* pThis) { LoadRecordsetFields(pThis); }
// Symbol: ?PrepareAndExecute@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__PrepareAndExecute_CRecordset__IEAAXXZ(CRecordset* pThis) { if (!pThis) return; if (pThis->m_hstmt==SQL_NULL_HSTMT) impl__AllocHstmt_CRecordset__IEAAHXZ(pThis); CString sql=GetStoredSql(pThis); if (!sql.IsEmpty() && pThis->m_hstmt!=SQL_NULL_HSTMT) { SQLPrepareW(pThis->m_hstmt,(SQLWCHAR*)static_cast<const wchar_t*>(sql),SQL_NTS); SQLExecute(pThis->m_hstmt); } }
// Symbol: ?PrepareUpdateHstmt@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared=true; }
// Symbol: ?RefreshRowset@CRecordset@@QEAAXGG@Z
extern "C" void MS_ABI impl__RefreshRowset_CRecordset__QEAAXGG_Z(CRecordset* pThis, unsigned short fetchType, unsigned short) { if (pThis && pThis->m_hstmt!=SQL_NULL_HSTMT) SQLFetchScroll(pThis->m_hstmt, fetchType, 0); }
// Symbol: ?ResetCursor@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__ResetCursor_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis && pThis->m_hstmt!=SQL_NULL_HSTMT) SQLCloseCursor(pThis->m_hstmt); if (pThis) { pThis->m_bBOF=TRUE; pThis->m_bEOF=FALSE; pThis->m_nAbsolutePosition=0; } }
// Symbol: ?SendLongBinaryData@CRecordset@@IEAAXPEAX@Z
extern "C" void MS_ABI impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z(CRecordset* pThis, void* pData) {
    if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT || !pData) return;
    LongBinaryCompat* binary = static_cast<LongBinaryCompat*>(pData);
    if (!binary->m_hData) return;
    void* data = ::GlobalLock(binary->m_hData);
    if (!data) return;
    RETCODE rc = SQLPutData(pThis->m_hstmt, data, binary->m_dwDataLength);
    ::GlobalUnlock(binary->m_hData);
    if (!SqlSucceeded(rc)) {
        EnsureRecordsetState(pThis).lastRetCode = rc;
    }
}
// Symbol: ?SetConcurrencyAndCursorType@CRecordset@@IEAAXPEAXK@Z
extern "C" void MS_ABI impl__SetConcurrencyAndCursorType_CRecordset__IEAAXPEAXK_Z(CRecordset*, void* hstmt, unsigned long options) {
    if (!hstmt) return;
    SQLSetStmtAttr(static_cast<HSTMT>(hstmt), SQL_ATTR_CONCURRENCY,
                   CastOdbcOptionValue((options & CRecordset::readOnly) ? SQL_CONCUR_READ_ONLY : SQL_CONCUR_VALUES), 0);
}
// Symbol: ?SetFieldDirty@CRecordset@@QEAAXPEAXH@Z
extern "C" void MS_ABI impl__SetFieldDirty_CRecordset__QEAAXPEAXH_Z(CRecordset* pThis, void* field, int dirty) { if (!pThis||!field) return; auto& st=EnsureRecordsetState(pThis); int idx=impl__GetBoundFieldIndex_CRecordset__QEAAHPEAX_Z(pThis,field); if (idx<0) { idx=static_cast<int>(st.boundFieldIndexes.size()); st.boundFieldIndexes[field]=idx; } if (dirty) st.dirtyFields.insert(idx); else st.dirtyFields.erase(idx); }
// Symbol: ?SetLockingMode@CRecordset@@QEAAXI@Z
extern "C" void MS_ABI impl__SetLockingMode_CRecordset__QEAAXI_Z(CRecordset* pThis, unsigned int mode) { if (pThis) EnsureRecordsetState(pThis).lockingMode=mode; }
// Symbol: ?SetRowsetCurrencyStatus@CRecordset@@UEAAXFGJ_K@Z
extern "C" void MS_ABI impl__SetRowsetCurrencyStatus_CRecordset__UEAAXFGJ_K_Z(CRecordset* pThis, short direction, unsigned short step, long row, unsigned long long flag) {
    if (!pThis) return;
    RecordsetState& state = EnsureRecordsetState(pThis);
    state.bookmark = static_cast<unsigned long>(row);
    state.lastRetCode = (flag ? SQL_SUCCESS : SQL_SUCCESS);
    if (step == 0 || row < 0 || direction < 0) {
        pThis->m_bEOF = TRUE;
        pThis->m_bBOF = TRUE;
    } else {
        pThis->m_bEOF = FALSE;
        pThis->m_bBOF = FALSE;
        pThis->m_nAbsolutePosition = row;
    }
}
// Symbol: ?SetRowsetCursorPosition@CRecordset@@QEAAXGG@Z
extern "C" void MS_ABI impl__SetRowsetCursorPosition_CRecordset__QEAAXGG_Z(CRecordset* pThis, unsigned short fetchType, unsigned short row) { if (pThis && pThis->m_hstmt!=SQL_NULL_HSTMT) SQLFetchScroll(pThis->m_hstmt, fetchType, row); }
// Symbol: ?SetState@CRecordset@@IEAAXHPEB_WK@Z
extern "C" void MS_ABI impl__SetState_CRecordset__IEAAXHPEB_WK_Z(CRecordset* pThis, int open, const wchar_t* sql, unsigned long options) { if (pThis) { pThis->m_bOpen=open; pThis->m_dwOptions=options; if (sql) StoreSql(pThis,CString(sql)); } }
// Symbol: ?SetUpdateMethod@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__SetUpdateMethod_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared=true; }
// Symbol: ?SkipDeletedRecords@CRecordset@@QEAAXGJPEA_KPEAF@Z
extern "C" void MS_ABI impl__SkipDeletedRecords_CRecordset__QEAAXGJPEA_KPEAF_Z(CRecordset* pThis, unsigned short, long, unsigned long long* rowsFetched, short* status) { if (rowsFetched) *rowsFetched=pThis&&!pThis->m_bDeleted?1:0; if (status) *status=SQL_SUCCESS; }
// Symbol: ?StoreFields@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__StoreFields_CRecordset__QEAAXXZ(CRecordset* pThis) { LoadRecordsetFields(pThis); }
// Symbol: ?UpdateInsertDelete@CRecordset@@IEAAHXZ
extern "C" int MS_ABI impl__UpdateInsertDelete_CRecordset__IEAAHXZ(CRecordset* pThis) { return pThis ? pThis->Update() : FALSE; }
// Symbol: ?VerifyCursorSupport@CRecordset@@IEAAKXZ
extern "C" unsigned long MS_ABI impl__VerifyCursorSupport_CRecordset__IEAAKXZ(CRecordset* pThis) { return pThis && pThis->m_hstmt!=SQL_NULL_HSTMT ? 1 : 0; }
// Symbol: ?VerifyDriverBehavior@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__VerifyDriverBehavior_CRecordset__IEAAXXZ(CRecordset* pThis) {
    if (!pThis) return;
    if (!pThis->m_pDatabase || !pThis->m_pDatabase->IsOpen()) return;
    if (pThis->m_hstmt == SQL_NULL_HSTMT) return;

    SQLUINTEGER supported = 0;
    SQLSMALLINT infoLen = 0;
    RETCODE rc = SQLGetInfo(pThis->m_pDatabase->GetHDBC(),
                            SQL_DYNAMIC_CURSOR_ATTRIBUTES1,
                            &supported, sizeof(supported), &infoLen);
    EnsureRecordsetState(pThis).lastRetCode = rc;
    if (rc == SQL_NO_DATA || rc == SQL_ERROR) {
        rc = SQLGetInfo(pThis->m_pDatabase->GetHDBC(),
                        SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1,
                        &supported, sizeof(supported), &infoLen);
        EnsureRecordsetState(pThis).lastRetCode = rc;
    }
}
CRecordset::CRecordset(CDatabase* pDatabase)
    : m_hstmt(SQL_NULL_HSTMT), m_pDatabase(pDatabase),
      m_bBOF(FALSE), m_bEOF(FALSE), m_bDeleted(FALSE), m_bOpen(FALSE),
      m_nFields(0), m_nParams(0), m_nRecordCount(0),
      m_nAbsolutePosition(0), m_dwRowsetSize(1),
      m_nOpenType(CRecordset::dynaset),
      m_dwOptions(CRecordset::none), m_nEditMode(0) {
    memset(_crecordset_padding, 0, sizeof(_crecordset_padding));
}
CRecordset::~CRecordset() {
    Close();
    std::lock_guard<std::mutex> lock(g_recordsetStateMutex);
    g_recordsetSql.erase(this);
}
BOOL CRecordset::Open(UINT nOpenType, const wchar_t* lpszSQL, DWORD dwOptions) {
    if (m_bOpen) return FALSE;
    if (!m_pDatabase) return FALSE;

    m_nOpenType = nOpenType;
    m_dwOptions = dwOptions;

    if (!m_pDatabase->IsOpen()) {
        return FALSE;
    }

    // Allocate statement handle
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_pDatabase->GetHDBC(), &m_hstmt) != SQL_SUCCESS) {
        return FALSE;
    }

    // Set options
    if (!OnSetOptions(m_hstmt)) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
        return FALSE;
    }

    // Get SQL string
    CString strSQL = lpszSQL ? CString(lpszSQL) : GetDefaultSQL();
    if (strSQL.IsEmpty()) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
        return FALSE;
    }

    // Execute
    RETCODE rc = SQLExecDirectW(m_hstmt, (SQLWCHAR*)(const wchar_t*)strSQL, SQL_NTS);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
        return FALSE;
    }

    // Determine number of result columns
    SQLSMALLINT nCols = 0;
    SQLNumResultCols(m_hstmt, &nCols);
    m_nFields = nCols;
    m_nRecordCount = 0;
    SQLLEN rowCount = 0;
    RETCODE rowRc = SQLRowCount(m_hstmt, &rowCount);
    if (SqlSucceeded(rowRc) && rowCount > 0) {
        m_nRecordCount = static_cast<long>(rowCount);
    }

    m_bOpen = TRUE;
    m_nEditMode = 0;
    m_bDeleted = FALSE;
    m_bBOF = TRUE;
    m_bEOF = FALSE;
    m_nAbsolutePosition = 0;
    {
        std::lock_guard<std::mutex> lock(g_recordsetStateMutex);
        RecordsetState& state = EnsureRecordsetState(this);
        state = RecordsetState{};
        state.currentSql = strSQL;
        if (state.baseSql.IsEmpty()) state.baseSql = strSQL;
        if (state.fieldLengths.size() < static_cast<size_t>(m_nFields)) {
            state.fieldLengths.resize(static_cast<size_t>(m_nFields), 0);
        } else {
            state.fieldLengths.assign(static_cast<size_t>(m_nFields), 0);
        }
        StoreSql(this, strSQL);
    }

    return TRUE;
}
void CRecordset::Close() {
    if (m_hstmt != SQL_NULL_HSTMT) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
    }
    m_bOpen = FALSE;
    std::lock_guard<std::mutex> lock(g_recordsetStateMutex);
    g_recordsetSql.erase(this);
    g_recordsetStates.erase(this);
}
BOOL CRecordset::IsDeleted() const {
    return m_bDeleted;
}
BOOL CRecordset::IsFieldDirty(void* pvField) {
    if (!pvField) return FALSE;
    RecordsetState& state = EnsureRecordsetState(this);
    auto it = state.boundFieldIndexes.find(pvField);
    if (it == state.boundFieldIndexes.end()) {
        return FALSE;
    }
    return state.dirtyFields.count(static_cast<unsigned long>(it->second)) != 0;
}
BOOL CRecordset::IsFieldNull(void* pvField) {
    if (!pvField) return FALSE;
    auto& state = EnsureRecordsetState(this);
    auto it = state.boundFieldIndexes.find(pvField);
    return it != state.boundFieldIndexes.end() &&
           state.nullFields.count(static_cast<unsigned long>(it->second)) != 0;
}
BOOL CRecordset::IsFieldNullable(unsigned long dwField) {
    LoadRecordsetFields(this);
    auto it = g_recordsetStates.find(this);
    if (it == g_recordsetStates.end()) return TRUE;
    auto& fields = it->second.fields;
    if (dwField == 0 || dwField > fields.size()) return TRUE;
    const auto& field = fields[dwField - 1];
    return field.m_nNullability != SQL_NO_NULLS;
}
void CRecordset::MoveFirst() {
    Move(0, SQL_FETCH_FIRST);
}
void CRecordset::MoveLast() {
    if (m_nRecordCount > 0) {
        SetAbsolutePosition(m_nRecordCount);
        return;
    }
    Move(0, SQL_FETCH_LAST);
}
void CRecordset::MoveNext() {
    if (m_bEOF) {
        return;
    }
    Move(0, SQL_FETCH_NEXT);
}
void CRecordset::MovePrev() {
    if (m_bBOF) {
        return;
    }
    Move(0, SQL_FETCH_PRIOR);
}
void CRecordset::Move(long nRows, WORD wFetchType) {
    if (!m_bOpen) return;
    RETCODE rc = SQLFetchScroll(m_hstmt, wFetchType, nRows);
    if (SqlSucceeded(rc)) {
        m_bBOF = FALSE;
        m_bEOF = FALSE;

        if (wFetchType == SQL_FETCH_FIRST) {
            m_nAbsolutePosition = 1;
        } else if (wFetchType == SQL_FETCH_LAST) {
            m_nAbsolutePosition = m_nRecordCount > 0 ? m_nRecordCount : m_nAbsolutePosition;
            if (m_nAbsolutePosition == 0) m_nAbsolutePosition = 1;
        } else if (wFetchType == SQL_FETCH_ABSOLUTE) {
            m_nAbsolutePosition = nRows > 0 ? nRows : (m_nRecordCount + nRows + 1);
        } else if (wFetchType == SQL_FETCH_PRIOR) {
            if (m_nAbsolutePosition > 0) --m_nAbsolutePosition;
        } else if (wFetchType == SQL_FETCH_NEXT) {
            ++m_nAbsolutePosition;
        } else {
            if (nRows != 0) m_nAbsolutePosition += nRows;
        }
        if (m_nRecordCount > 0) {
            m_nAbsolutePosition = std::min(m_nAbsolutePosition, m_nRecordCount);
            if (m_nAbsolutePosition < 1) m_nAbsolutePosition = 1;
        }
    } else if (rc == SQL_NO_DATA) {
        if (wFetchType == SQL_FETCH_ABSOLUTE && nRows < 0) {
            m_nAbsolutePosition = m_nRecordCount > 0
                                     ? std::max<long>(1, m_nRecordCount + nRows + 1)
                                     : 0;
        }
        if (nRows < 0 || wFetchType == SQL_FETCH_PRIOR || wFetchType == SQL_FETCH_FIRST) {
            m_bBOF = TRUE;
            m_nAbsolutePosition = 0;
        } else {
            m_bEOF = TRUE;
        }
    }
}
void CRecordset::SetAbsolutePosition(long nRows) {
    if (!m_bOpen) {
        m_nAbsolutePosition = nRows;
        return;
    }
    Move(nRows, SQL_FETCH_ABSOLUTE);
}
void CRecordset::AddNew() {
    if (!m_bOpen || (m_dwOptions & CRecordset::readOnly)) return;
    m_nEditMode = 1; // addNew
}
void CRecordset::Edit() {
    if (!m_bOpen || (m_dwOptions & CRecordset::readOnly)) return;
    m_nEditMode = 2; // edit
}
void CRecordset::Delete() {
    if (!m_bOpen || (m_dwOptions & CRecordset::readOnly)) return;
    m_nEditMode = 3; // delete
    m_bDeleted = TRUE;
}
BOOL CRecordset::Update() {
    if (!m_bOpen || m_nEditMode == 0 || (m_dwOptions & CRecordset::readOnly)) {
        return FALSE;
    }
    m_bDeleted = FALSE;
    m_nEditMode = 0;
    auto& state = EnsureRecordsetState(this);
    state.dirtyFields.clear();
    state.nullFields.clear();
    return TRUE;
}
void CRecordset::CancelUpdate() {
    if (!m_bOpen) return;
    m_nEditMode = 0;
    EnsureRecordsetState(this).dirtyFields.clear();
    EnsureRecordsetState(this).nullFields.clear();
}
void CRecordset::SetFieldNull(void* pvField, BOOL bNull) {
    if (!pvField) return;
    RecordsetState& state = EnsureRecordsetState(this);
    int index = 0;
    auto it = state.boundFieldIndexes.find(pvField);
    if (it == state.boundFieldIndexes.end()) {
        index = static_cast<int>(state.boundFieldIndexes.size());
        state.boundFieldIndexes[pvField] = index;
    } else {
        index = it->second;
    }
    unsigned long fieldIndex = static_cast<unsigned long>(index);
    if (bNull) {
        state.nullFields.insert(fieldIndex);
    } else {
        state.nullFields.erase(fieldIndex);
    }
    state.dirtyFields.insert(fieldIndex);
}
BOOL CRecordset::CanAppend() const {
    if (!m_pDatabase || !m_pDatabase->IsOpen()) return FALSE;
    return (m_dwOptions & CRecordset::readOnly) == 0;
}
BOOL CRecordset::CanRestart() const {
    return IsOpen();
}
BOOL CRecordset::CanScroll() const {
    return IsOpen() && m_nOpenType != CRecordset::forwardOnly;
}
BOOL CRecordset::CanTransact() const {
    return m_pDatabase ? m_pDatabase->CanTransact() : FALSE;
}
BOOL CRecordset::CanUpdate() const {
    return m_pDatabase && !m_bDeleted && (m_dwOptions & CRecordset::readOnly) == 0;
}
void CRecordset::SetRowsetSize(DWORD dwNewRowsetSize) {
    m_dwRowsetSize = dwNewRowsetSize;
}
CString CRecordset::GetDefaultConnect() {
    return L"ODBC;";
}
CString CRecordset::GetDefaultSQL() {
    return L"";
}
void CRecordset::DoFieldExchange(CFieldExchange* pFX) {
    if (!pFX) return;
    pFX->m_pRecordset = this;
    pFX->m_nFields = 0;
    pFX->m_nParams = 0;

    RecordsetState& state = EnsureRecordsetState(this);
    state.boundFieldIndexes.clear();
    state.boundParamIndexes.clear();
    state.fieldLengths.clear();
    state.updatePrepared = TRUE;
    state.dirtyFields.clear();

    if (m_hstmt != SQL_NULL_HSTMT) {
        SQLSMALLINT count = 0;
        if (SqlSucceeded(SQLNumResultCols(m_hstmt, &count))) {
            state.fieldLengths.resize(static_cast<size_t>(count), 0);
            m_nFields = static_cast<int>(count);
        }
    }

    if (m_nFields > 0 && !state.fieldLengths.empty()) {
        state.fieldLengths.resize(static_cast<size_t>(m_nFields), 0);
    }

    LoadRecordsetFields(this);
}
BOOL CRecordset::OnSetOptions(HSTMT hstmt) {
    if (!hstmt || hstmt == SQL_NULL_HSTMT) return FALSE;

    SQLUINTEGER timeout = static_cast<SQLUINTEGER>(g_queryTimeoutSeconds);
    SQLUINTEGER cursor = SQL_CURSOR_FORWARD_ONLY;
    SQLUINTEGER concurrency = (m_dwOptions & CRecordset::readOnly) ? SQL_CONCUR_READ_ONLY : SQL_CONCUR_VALUES;

    switch (m_nOpenType) {
    case CRecordset::snapshot:
        cursor = SQL_CURSOR_KEYSET_DRIVEN;
        break;
    case CRecordset::dynaset:
    case CRecordset::dynamic:
        cursor = SQL_CURSOR_DYNAMIC;
        break;
    case CRecordset::forwardOnly:
    default:
        cursor = SQL_CURSOR_FORWARD_ONLY;
        break;
    }

    RETCODE rc = SQL_SUCCESS;
    rc = SQLSetStmtAttr(hstmt, SQL_ATTR_QUERY_TIMEOUT,
                        CastOdbcOptionValue(timeout),
                        SQL_IS_UINTEGER);
    rc = (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
             ? SQLSetStmtAttr(hstmt, SQL_ATTR_CURSOR_TYPE, CastOdbcOptionValue(cursor), 0)
             : rc;
    rc = (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
             ? SQLSetStmtAttr(hstmt, SQL_ATTR_CONCURRENCY, CastOdbcOptionValue(concurrency), 0)
             : rc;

    return rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO;
}
void CRecordset::OnFieldChange(void* pvField, LONG* plLength) {
    if (!m_pDatabase || !pvField) return;
    RecordsetState& state = EnsureRecordsetState(this);
    int fieldIndex = 0;
    auto fieldIt = state.boundFieldIndexes.find(pvField);
    if (fieldIt != state.boundFieldIndexes.end()) {
        fieldIndex = fieldIt->second;
    } else {
        fieldIndex = static_cast<int>(state.boundFieldIndexes.size());
        state.boundFieldIndexes[pvField] = fieldIndex;
    }

    if (plLength && *plLength < 0) {
        state.nullFields.insert(static_cast<unsigned long>(fieldIndex));
    } else {
        state.nullFields.erase(static_cast<unsigned long>(fieldIndex));
    }
    state.dirtyFields.insert(static_cast<unsigned long>(fieldIndex));
}
void CRecordset::SetDirtyFieldStatus(DWORD dwFieldStatus) {
    EnsureRecordsetState(this).dirtyFields.insert(dwFieldStatus);
}
void CRecordset::ClearDirtyFieldStatus(DWORD dwFieldStatus) {
    EnsureRecordsetState(this).dirtyFields.erase(dwFieldStatus);
}
void CRecordset::SetParamNull(int nIndex, BOOL bNull) {
    RecordsetState& state = EnsureRecordsetState(this);
    if (bNull) state.nullParams.insert(static_cast<unsigned long>(nIndex));
    else state.nullParams.erase(static_cast<unsigned long>(nIndex));
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?IsFieldNullable@CRecordset@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__IsFieldNullable_CRecordset__QEAAHPEAX_Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}


// Symbol: ?IsFieldNullable@CRecordset@@QEBAHK@Z
extern "C" int MS_ABI impl__IsFieldNullable_CRecordset__QEBAHK_Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}

