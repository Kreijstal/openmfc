// Native release x64 recordset update statement and field binding.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "openmfc/afxdao.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);

// RVA 0x1f9d30: the primary cancellation is unconditional; the optional
// update handle is read again after that driver call.
void CRecordset::Cancel() {
    ::SQLCancel(m_hstmt);
    if (m_hstmtUpdate) ::SQLCancel(m_hstmtUpdate);
}
// Symbol: ?Cancel@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__Cancel_CRecordset__QEAAXXZ(CRecordset* rs) {rs->Cancel();}

// RVA 0x1fc570. ODBC data-at-execution tokens are CLongBinary objects, not
// byte buffers. Keep their full-width sizes and each driver's async retries.
void CRecordset::SendLongBinaryData(HSTMT statement) {
    CLongBinary* binary;
    RETCODE code;
    auto fail=[&] {
        auto* error=new CDBException(code);
        error->BuildErrorString(m_pDatabase,statement,TRUE);
        Cancel();
        throw error;
    };
    do {code=::SQLParamData(statement,reinterpret_cast<SQLPOINTER*>(&binary));} while(code==SQL_STILL_EXECUTING);
    if (!Check(code)) fail();
    while (code==SQL_NEED_DATA) {
        void* data=::GlobalLock(binary->m_hData);
        if (!data) ThrowDBException(code,statement);
        do {code=::SQLPutData(statement,data,static_cast<SQLLEN>(binary->m_dwDataLength));} while(code==SQL_STILL_EXECUTING);
        ::GlobalUnlock(binary->m_hData);
        if (!Check(code)) fail();
        do {code=::SQLParamData(statement,reinterpret_cast<SQLPOINTER*>(&binary));} while(code==SQL_STILL_EXECUTING);
        if (!Check(code)) ThrowDBException(code,statement);
    }
}
// Symbol: ?SendLongBinaryData@CRecordset@@IEAAXPEAX@Z
extern "C" void MS_ABI impl__SendLongBinaryData_CRecordset__IEAAXPEAX_Z(CRecordset* rs,HSTMT statement) {rs->SendLongBinaryData(statement);}

// RVA 0x1fbcd0. Bound updates/deletes/inserts use SQLSetPos and leave column
// buffers bound if a driver/validation callback interrupts the operation.
void CRecordset::ExecuteSetPosUpdate() {
    WORD operation,status;
    if (!m_nEditMode) {operation=SQL_DELETE;status=SQL_ROW_DELETED;}
    else if (m_nEditMode==edit) {operation=SQL_UPDATE;status=SQL_ROW_UPDATED;}
    else {operation=SQL_ADD;status=SQL_ROW_ADDED;}
    BindFieldsForUpdate();
    RETCODE code;
    do {code=::SQLSetPos(m_hstmt,1,operation,SQL_LOCK_NO_CHANGE);} while(code==SQL_STILL_EXECUTING);
    if (!Check(code)) impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,m_pDatabase,m_hstmt);
    if (code==SQL_NEED_DATA) SendLongBinaryData(m_hstmt);
    else if (code==SQL_SUCCESS_WITH_INFO && m_rgRowStatus[0]!=status) ThrowDBException(0x400,nullptr);
    UnbindFieldsForUpdate();
}
// Symbol: ?ExecuteSetPosUpdate@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__ExecuteSetPosUpdate_CRecordset__IEAAXXZ(CRecordset* rs) {rs->ExecuteSetPosUpdate();}

// RVA 0x1fc440. Bulk inserts execute the prepared statement; other writes
// execute the constructed SQL and require one affected row when reported.
void CRecordset::ExecuteUpdateSQL() {
    RETCODE code;
    if (m_dwOptions&optimizeBulkAdd) {
        do {code=::SQLExecute(m_hstmtUpdate);} while(code==SQL_STILL_EXECUTING);
    } else {
        auto* sql=reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(m_strUpdateSQL.GetString()));
        do {code=::SQLExecDirectW(m_hstmtUpdate,sql,SQL_NTS);} while(code==SQL_STILL_EXECUTING);
    }
    if (!Check(code)) ThrowDBException(code,m_hstmtUpdate);
    if (code==SQL_NEED_DATA) SendLongBinaryData(m_hstmtUpdate);
    SQLLEN rows=0;
    code=::SQLRowCount(m_hstmtUpdate,&rows);
    if (Check(code) && rows!=-1 && rows!=1) ThrowDBException(rows?0x3ee:0x3f0,nullptr);
    m_strUpdateSQL.Empty();
}
// Symbol: ?ExecuteUpdateSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__ExecuteUpdateSQL_CRecordset__IEAAXXZ(CRecordset* rs) {rs->ExecuteUpdateSQL();}

// Native base hook shares the empty release body at RVA 0x27d0.
void CRecordset::OnSetUpdateOptions(HSTMT) {}
// Symbol: ?OnSetUpdateOptions@CRecordset@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnSetUpdateOptions_CRecordset__UEAAXPEAX_Z(CRecordset* rs,HSTMT statement) {rs->CRecordset::OnSetUpdateOptions(statement);}

// RVA 0x1fbdc0. Reusing a statement preserves bulk-add parameter bindings.
// Failed close/reset operations recreate the handle; only a newly allocated
// statement receives the virtual options callback.
void CRecordset::PrepareUpdateHstmt() {
    if (!m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    if (m_hstmtUpdate) {
        RETCODE code=::SQLFreeStmt(m_hstmtUpdate,SQL_CLOSE);
        if (Check(code)) {
            if (m_dwOptions&optimizeBulkAdd) return;
            code=::SQLFreeStmt(m_hstmtUpdate,SQL_RESET_PARAMS);
            if (Check(code)) return;
        }
        ::SQLFreeStmt(m_hstmtUpdate,SQL_DROP);
        m_hstmtUpdate=nullptr;
    }
    RETCODE code=::SQLAllocStmt(m_pDatabase->m_hdbc,&m_hstmtUpdate);
    if (!Check(code)) impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,m_pDatabase,m_hstmtUpdate);
    OnSetUpdateOptions(m_hstmtUpdate);
}
// Symbol: ?PrepareUpdateHstmt@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__PrepareUpdateHstmt_CRecordset__IEAAXXZ(CRecordset* rs) {rs->PrepareUpdateHstmt();}

// RVAs 0x1fcd30/0x1fcd90. Delete has no bound-value exchange; Edit/AddNew
// use the primary statement's existing column bindings and length buffers.
void CRecordset::BindFieldsForUpdate() {
    if (m_nEditMode!=edit && m_nEditMode!=addnew) return;
    CFieldExchange exchange(CFieldExchange::BindFieldForUpdate,this);
    exchange.m_hstmt=m_hstmt;
    DoFieldExchange(&exchange);
}
void CRecordset::UnbindFieldsForUpdate() {
    if (m_nEditMode!=edit && m_nEditMode!=addnew) return;
    CFieldExchange exchange(CFieldExchange::UnbindFieldForUpdate,this);
    exchange.m_hstmt=m_hstmt;
    DoFieldExchange(&exchange);
}
// Symbol: ?BindFieldsForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__BindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset* rs) {rs->BindFieldsForUpdate();}
// Symbol: ?UnbindFieldsForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__UnbindFieldsForUpdate_CRecordset__QEAAXXZ(CRecordset* rs) {rs->UnbindFieldsForUpdate();}
