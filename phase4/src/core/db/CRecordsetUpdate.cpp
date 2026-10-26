// Native release x64 recordset SQL construction and write execution.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "openmfc/afxdao.h"
#include "detail/DatabaseStringBuffer.h"
#include <cstdint>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" void MS_ABI impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(CDatabase*,wchar_t*);
extern "C" UINT MS_ABI impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,CString*,const wchar_t*);
extern "C" UINT MS_ABI impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,void*,CString*,const wchar_t*);
extern "C" UINT MS_ABI impl__AppendNamesValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,void*,CString*,const wchar_t*);
using openmfc::detail::DatabaseStringBuffer;

// RVA 0x1fbf10. Cursor names are cached separately from each SQL string.
void CRecordset::BuildUpdateSQL() {
    DatabaseStringBuffer sql(m_strUpdateSQL);
    auto table=[&]{sql.append(m_strTableName.GetString(),m_strTableName.GetLength());};
    auto finish=[&](wchar_t last) {
        int length=m_strUpdateSQL.GetLength();
        if (length<1) {impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);return;}
        sql.get(length)[length-1]=last;sql.release(length);
    };
    if (m_nEditMode==addnew) {
        sql.set(L"INSERT INTO ");table();sql.append(L" (");
        impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(this,&m_strUpdateSQL,L",");finish(L')');
        sql.append(L" VALUES (");
        impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(this,m_hstmtUpdate,&m_strUpdateSQL,L",");finish(L')');
    } else if (m_nEditMode==edit) {
        sql.set(L"UPDATE ");table();sql.append(L" SET ");
        impl__AppendNamesValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(this,m_hstmtUpdate,&m_strUpdateSQL,L",");finish(L' ');
    } else if (!m_nEditMode) {sql.set(L"DELETE FROM ");table();}
    if (m_nEditMode<=edit) {
        sql.append(L" WHERE CURRENT OF ");
        if (!m_strCursorName.GetLength()) {
            wchar_t cursor[41];SQLSMALLINT length=40;
            RETCODE code=::SQLGetCursorNameW(m_hstmt,reinterpret_cast<SQLWCHAR*>(cursor),41,&length);
            if (code==SQL_SUCCESS_WITH_INFO && length>40) {
                // Native has only a GS unwind handler here, without C++
                // cleanup. Release at the explicit success/rejection points.
                auto* buffer=new wchar_t[size_t(length)+1];
                code=::SQLGetCursorNameW(m_hstmt,reinterpret_cast<SQLWCHAR*>(buffer),SQLSMALLINT(length+1),&length);
                if (!Check(code)) {delete[] buffer;ThrowDBException(code,nullptr);}
                else {DatabaseStringBuffer(m_strCursorName).set(buffer);delete[] buffer;}
            } else if (!Check(code)) ThrowDBException(code,nullptr);
            else DatabaseStringBuffer(m_strCursorName).set(cursor);
        }
        sql.append(m_strCursorName.GetString(),m_strCursorName.GetLength());
    }
    if (!m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(m_pDatabase,sql.get(0));sql.release(-1);
    if (m_dwOptions&firstBulkAdd) {
        auto* text=reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(m_strUpdateSQL.GetString()));
        RETCODE code;
        do {code=::SQLPrepareW(m_hstmtUpdate,text,SQL_NTS);} while(code==SQL_STILL_EXECUTING);
        if (!Check(code)) ThrowDBException(code,m_hstmtUpdate);
    }
}
// Symbol: ?BuildUpdateSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__BuildUpdateSQL_CRecordset__IEAAXXZ(CRecordset* rs) {rs->BuildUpdateSQL();}

// RVA 0x1fd290. Only post-write currency/cache failures are swallowed.
BOOL CRecordset::UpdateInsertDelete() {
    if (m_nEditMode==addnew) {
        if (!m_bAppendable) ThrowDBException(0x3f1,nullptr);
    } else {
        if (!m_bUpdatable) ThrowDBException(0x3f1,nullptr);
        if (m_bEOF || m_bBOF || m_bDeleted) ThrowDBException(0x3ef,nullptr);
    }
    if (m_nEditMode && !IsFieldDirty(nullptr)) return FALSE;
    if (!m_bUseUpdateSQL) ExecuteSetPosUpdate();
    else {
        HSTMT previous=m_hstmtUpdate;
        PrepareUpdateHstmt();
        if ((m_dwOptions&optimizeBulkAdd) && previous)
            impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(this,m_hstmtUpdate,&m_strUpdateSQL,L",");
        else {
            if (m_dwOptions&optimizeBulkAdd) {m_dwOptions&=~optimizeBulkAdd;m_dwOptions|=firstBulkAdd;}
            BuildUpdateSQL();
            if (m_dwOptions&firstBulkAdd) {m_dwOptions&=~firstBulkAdd;m_dwOptions|=optimizeBulkAdd;}
        }
        ExecuteUpdateSQL();
    }
    try {
        if (m_nEditMode==addnew) {
            m_bBOF=m_bEOF=FALSE;
            if (!m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return FALSE;}
            if (m_pDatabase->m_bIncRecordCountOnAdd && m_lCurrentRecord>=0) {
                if (m_lRecordCount!=-1) m_lRecordCount=long(uint32_t(m_lRecordCount)+1);
                m_lCurrentRecord=long(uint32_t(m_lCurrentRecord)+1);
            }
            if (m_bCheckCacheForDirtyFields && m_nFields) LoadFields();
        } else if (!m_nEditMode) {
            if (m_lCurrentRecord>=0) {
                if (m_lRecordCount>0) --m_lRecordCount;
                --m_lCurrentRecord;
            }
            m_bDeleted=TRUE;SetFieldNull(nullptr,TRUE);
        }
        m_nEditMode=noMode;
    } catch (CException* error) {error->Delete();}
    if (m_bCheckCacheForDirtyFields && !(m_dwOptions&optimizeBulkAdd)) SetFieldDirty(nullptr,FALSE);
    return TRUE;
}
// Symbol: ?UpdateInsertDelete@CRecordset@@IEAAHXZ
extern "C" BOOL MS_ABI impl__UpdateInsertDelete_CRecordset__IEAAHXZ(CRecordset* rs) {return rs->UpdateInsertDelete();}
BOOL CRecordset::Update() {
    if (m_dwOptions&useMultiRowFetch) return FALSE;
    if (m_nEditMode!=edit && m_nEditMode!=addnew) ThrowDBException(0x3ed,nullptr);
    return UpdateInsertDelete();
}
void CRecordset::Delete() {
    if (m_dwOptions&useMultiRowFetch) return;
    if (m_nEditMode) ThrowDBException(0x3ed,nullptr);
    UpdateInsertDelete();
}
// Symbol: ?Update@CRecordset@@UEAAHXZ
extern "C" BOOL MS_ABI impl__Update_CRecordset__UEAAHXZ(CRecordset* rs) {return rs->CRecordset::Update();}
// Symbol: ?Delete@CRecordset@@UEAAXXZ
extern "C" void MS_ABI impl__Delete_CRecordset__UEAAXXZ(CRecordset* rs) {rs->CRecordset::Delete();}

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
