// CRecordset — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"
#include "detail/CRecordsetProxies.h"
#include "detail/CPtrListSupport.h"
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();

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
extern "C" BOOL MS_ABI impl__AllocHstmt_CRecordset__IEAAHXZ(CRecordset*);
// Stub: disas.py could not resolve this export; the SDK only documents when this hook is called.
// Symbol: ?PreBindFields@CRecordset@@UEAAXXZ
extern "C" void MS_ABI impl__PreBindFields_CRecordset__UEAAXXZ(CRecordset*) {}
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
BOOL CRecordset::CanAppend() const {
    return m_bAppendable;
}
BOOL CRecordset::CanRestart() const {
    return !(m_dwOptions & executeDirect);
}
BOOL CRecordset::CanScroll() const {
    return m_bScrollable;
}
BOOL CRecordset::CanTransact() const {
    if(!m_pDatabase)impl__AfxThrowUserException__YAXXZ();
    return m_pDatabase->m_bTransactions;
}
BOOL CRecordset::CanUpdate() const {
    return m_bUpdatable;
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

extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(short,void*,void*);
void CRecordset::PreBindFields() {impl__PreBindFields_CRecordset__UEAAXXZ(this);}
void CRecordset::ThrowDBException(RETCODE code,HSTMT statement) {
    impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,m_pDatabase,statement?statement:m_hstmt);
}
