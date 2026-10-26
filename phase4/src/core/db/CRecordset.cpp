// CRecordset — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"
#include "detail/CRecordsetProxies.h"
#include "detail/CPtrListSupport.h"
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);

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
extern "C" BOOL MS_ABI impl__AllocHstmt_CRecordset__IEAAHXZ(CRecordset*);
// Symbol: ?BuildUpdateSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__BuildUpdateSQL_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared = true; }
// Symbol: ?BindFieldsForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__BindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared = true; }
// Symbol: ?UnbindFieldsForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared = false; }
// Symbol: ?Cancel@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__Cancel_CRecordset__QEAAXXZ(CRecordset* pThis) { if (pThis && pThis->m_hstmt != SQL_NULL_HSTMT) SQLCancel(pThis->m_hstmt); }
// Symbol: ?ExecuteSetPosUpdate@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) pThis->Update(); }
// Symbol: ?ExecuteUpdateSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis && pThis->m_pDatabase) { CString sql = GetStoredSql(pThis); if (!sql.IsEmpty()) pThis->m_pDatabase->ExecuteSQL(static_cast<const wchar_t*>(sql)); } }
// Symbol: ?FlushResultSet@CRecordset@@QEAAHXZ
extern "C" int MS_ABI impl__FlushResultSet_CRecordset__QEAAHXZ(CRecordset* pThis) { if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT) return FALSE; while (SQLMoreResults(pThis->m_hstmt) == SQL_SUCCESS) {} return TRUE; }
// Symbol: ?GetBookmark@CRecordset@@QEAAXAEAVCDBVariant@@@Z
extern "C" void MS_ABI impl__GetBookmark_CRecordset__QEAAXAEAVCDBVariant___Z(CRecordset*, void* variant) { if (variant) std::memset(variant, 0, 32); }
// Symbol: ?SetBookmark@CRecordset@@QEAAXAEBVCDBVariant@@@Z
extern "C" void MS_ABI impl__SetBookmark_CRecordset__QEAAXAEBVCDBVariant___Z(CRecordset* pThis, const void*) { if (pThis) EnsureRecordsetState(pThis).bookmarksEnabled = true; }
// Symbol: ?GetData@CRecordset@@SA_JPEAVCDatabase@@PEAXFF1_JF@Z
extern "C" long long MS_ABI impl__GetData_CRecordset__SA_JPEAVCDatabase__PEAXFF1_JF_Z(CDatabase*, void* hstmt, short field, short cType, void* buffer, long long bufferLen, short) { if (!hstmt || !buffer || bufferLen <= 0) return SQL_ERROR; SQLLEN ind=0; RETCODE rc=SQLGetData(static_cast<HSTMT>(hstmt), static_cast<SQLUSMALLINT>(field), cType, buffer, static_cast<SQLLEN>(bufferLen), &ind); return SqlSucceeded(rc)?ind:rc; }
// Symbol: ?GetDataBuffer@CRecordset@@SAPEAXAEAVCDBVariant@@FPEA_JF_K@Z
extern "C" void* MS_ABI impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z(void* variant, short, long long* length, short, unsigned long) { if (length) *length = 0; return variant; }
// Symbol: ?GetFieldIndexByName@CRecordset@@QEAAFPEB_W@Z
extern "C" short MS_ABI impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z(CRecordset* pThis, const wchar_t* name) { int i=FindFieldByName(pThis,name); return i<0?static_cast<short>(-1):static_cast<short>(i); }
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
extern "C" void MS_ABI impl__PreBindFields_CRecordset__UEAAXXZ(CRecordset*) {}
// Symbol: ?PrepareUpdateHstmt@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ(CRecordset* pThis) { if (pThis) EnsureRecordsetState(pThis).updatePrepared=true; }
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
// Symbol: ?UpdateInsertDelete@CRecordset@@IEAAHXZ
extern "C" int MS_ABI impl__UpdateInsertDelete_CRecordset__IEAAHXZ(CRecordset* pThis) { return pThis ? pThis->Update() : FALSE; }
CRecordset::CRecordset(CDatabase* database) : m_pDatabase(database) {
    // Release x64 construction allocates a statement for an existing connection
    // and registers the recordset at the head of the database's list.
    m_hstmt=nullptr;m_nOpenType=snapshot;m_nDefaultType=snapshot;m_lOpen=-1;m_nEditMode=0;
    m_bUseUpdateSQL=FALSE;m_dwOptions=0;m_bUpdatable=FALSE;m_bAppendable=FALSE;m_bScrollable=FALSE;
    m_bRecordsetDb=FALSE;m_bRebindParams=FALSE;m_bLongBinaryColumns=FALSE;m_nLockMode=0;
    m_dwInitialGetDataLen=0;m_rgODBCFieldInfos=nullptr;m_rgFieldInfos=nullptr;m_rgRowStatus=nullptr;
    m_dwRowsetSize=25;m_dwAllocatedRowsetSize=0;m_nFields=0;m_nParams=0;m_nFieldsBound=0;
    m_lCurrentRecord=-2;m_lRecordCount=0;m_bUseODBCCursorLib=FALSE;m_nResultCols=-1;m_bCheckCacheForDirtyFields=TRUE;
    m_pbFieldFlags=nullptr;m_pbParamFlags=nullptr;m_plParamLength=nullptr;
    m_pvFieldProxy=nullptr;m_pvParamProxy=nullptr;m_nProxyFields=0;m_nProxyParams=0;m_hstmtUpdate=nullptr;
    if(database && database->m_hdbc) {
        RETCODE code=::SQLAllocStmt(database->m_hdbc,&m_hstmt);
        try {
            if(code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO && code!=SQL_NEED_DATA && code!=SQL_NO_DATA)
                CRecordset::ThrowDBException(SQL_INVALID_HANDLE,nullptr);
            struct RegistrationLock {
                RegistrationLock() {impl__AfxLockGlobals__YAXH_Z(15);}
                ~RegistrationLock() {impl__AfxUnlockGlobals__YAXH_Z(15);}
            } lock;
            reinterpret_cast<OpenMFC_CPtrList*>(database->m_listRecordsets)->AddHead(this);
        } catch(CException* exception) {
            // Native construction deletes status/registration exceptions and preserves
            // the driver-provided handle, leaving registration incomplete.
            exception->Delete();
        }
    }
}
BOOL CRecordset::IsDeleted() const {
    return m_bDeleted;
}
BOOL CRecordset::IsFieldDirty(void* field) {
    if(!m_nFields || !m_nEditMode)return FALSE;
    if(m_bCheckCacheForDirtyFields) {
        // These exchange passes still need their remaining cache comparison work.
        if(m_nEditMode==1)MarkForUpdate();
        else MarkForAddNew();
    }
    if(field)return IsFieldStatusDirty(GetBoundFieldIndex(field)-1);
    for(UINT i=0;i<m_nFields;++i)if(IsFieldStatusDirty(i))return TRUE;
    return FALSE;
}
BOOL CRecordset::IsFieldNull(void* field) {
    if(!field) {for(UINT i=0;i<m_nFields;++i)if(IsFieldStatusNull(i))return 2;return 0;}
    int index=GetBoundFieldIndex(field)-1;
    if(index<0)ThrowDBException(0x402,nullptr);
    return IsFieldStatusNull(index);
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
    Move(1, SQL_FETCH_FIRST);
}
void CRecordset::MoveLast() {
    Move(-1, SQL_FETCH_LAST);
}
void CRecordset::MoveNext() {
    Move(1, SQL_FETCH_NEXT);
}
void CRecordset::MovePrev() {
    Move(-1, SQL_FETCH_PRIOR);
}
void CRecordset::SetAbsolutePosition(long nRows) {
    Move(nRows, SQL_FETCH_ABSOLUTE);
}
void CRecordset::Delete() {
    if (m_lOpen!=1 || (m_dwOptions & CRecordset::readOnly)) return;
    m_nEditMode = 3; // delete
    m_bDeleted = TRUE;
}
BOOL CRecordset::Update() {
    if (m_lOpen!=1 || m_nEditMode == 0 || (m_dwOptions & CRecordset::readOnly)) {
        return FALSE;
    }
    m_bDeleted = FALSE;
    m_nEditMode = 0;
    auto& state = EnsureRecordsetState(this);
    state.dirtyFields.clear();
    state.nullFields.clear();
    return TRUE;
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
CString CRecordset::GetDefaultConnect() {
    return L"ODBC;";
}
CString CRecordset::GetDefaultSQL() {
    return L"";
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


extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(short,void*,void*);
void CRecordset::OnSetUpdateOptions(HSTMT statement) {impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z(this,statement);}
void CRecordset::PreBindFields() {impl__PreBindFields_CRecordset__UEAAXXZ(this);}
void CRecordset::ThrowDBException(RETCODE code,HSTMT statement) {
    impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,m_pDatabase,statement?statement:m_hstmt);
}
