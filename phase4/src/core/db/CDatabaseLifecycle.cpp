// Release x64 connection ownership and transaction calls, compared with
// mfc140u: AllocConnect 0x1f76a0, Close 0x1f70a0, Free 0x1f7160.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/CPtrListSupport.h"
#include "detail/DatabaseEnvironment.h"
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
namespace {
struct OdbcLock {
    OdbcLock() {impl__AfxLockGlobals__YAXH_Z(15);}
    ~OdbcLock() {impl__AfxUnlockGlobals__YAXH_Z(15);}
};
}

// Symbol: ?AllocConnect@CDatabase@@IEAAXK@Z
extern "C" void MS_ABI impl__AllocConnect_CDatabase__IEAAXK_Z(CDatabase* database,DWORD options) {
    if(database->m_hdbc)return;
    auto& environment=OpenMfcDatabaseEnvironment();
    impl__AfxLockGlobals__YAXH_Z(15);
    if(!environment && !database->Check(::SQLAllocEnv(&environment))) {
        impl__AfxUnlockGlobals__YAXH_Z(15);impl__AfxThrowMemoryException__YAXXZ();return;
    }
    RETCODE code=::SQLAllocConnect(environment,&database->m_hdbc);
    if(!database->Check(code)) {
        impl__AfxUnlockGlobals__YAXH_Z(15);database->ThrowDBException(code);
    }
    auto& count=OpenMfcAllocatedDatabaseConnections();count=LONG(DWORD(count)+1);
    impl__AfxUnlockGlobals__YAXH_Z(15);
    ::SQLSetConnectAttr(database->m_hdbc,SQL_LOGIN_TIMEOUT,reinterpret_cast<SQLPOINTER>(UINT_PTR(database->m_dwLoginTimeout)),0);
    if(!database->m_bUpdatable)::SQLSetConnectAttr(database->m_hdbc,SQL_ACCESS_MODE,reinterpret_cast<SQLPOINTER>(UINT_PTR(SQL_MODE_READ_ONLY)),0);
    if(options&CDatabase::useCursorLib) {
        // Retail sets value 2 for this option bit, despite its public name.
        ::SQLSetConnectAttr(database->m_hdbc,SQL_ODBC_CURSORS,reinterpret_cast<SQLPOINTER>(UINT_PTR(SQL_CUR_USE_DRIVER)),0);
        database->m_bIncRecordCountOnAdd=TRUE;
    }
}
void CDatabase::Close() {
    {
        OdbcLock lock;
        auto* list=reinterpret_cast<OpenMFC_CPtrList*>(m_listRecordsets);
        while(list->m_nCount) {
            auto* recordset=static_cast<CRecordset*>(list->m_pNodeHead->data);
            recordset->Close();recordset->m_pDatabase=nullptr;
        }
    }
    if(m_hdbc) {
        ::SQLDisconnect(m_hdbc);::SQLFreeConnect(m_hdbc);m_hdbc=nullptr;
        impl__AfxLockGlobals__YAXH_Z(15);
        auto& count=OpenMfcAllocatedDatabaseConnections();count=LONG(DWORD(count)-1);
        impl__AfxUnlockGlobals__YAXH_Z(15);
    }
}
// Symbol: ?Close@CDatabase@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CDatabase__UEAAXXZ(CDatabase* database) {database->CDatabase::Close();}
// Symbol: ?Free@CDatabase@@IEAAXXZ
extern "C" void MS_ABI impl__Free_CDatabase__IEAAXXZ(CDatabase* database) {
    try {database->Close();}catch(CException* error){error->Delete();}
    if(database->m_blobConnect.pbData) {::LocalFree(database->m_blobConnect.pbData);database->m_blobConnect.pbData=nullptr;}
    impl__AfxLockGlobals__YAXH_Z(15);
    auto& environment=OpenMfcDatabaseEnvironment();
    if(environment && !OpenMfcAllocatedDatabaseConnections()) {::SQLFreeEnv(environment);environment=nullptr;}
    impl__AfxUnlockGlobals__YAXH_Z(15);
}
BOOL CDatabase::BeginTrans() {
    if(!m_bTransactions)return FALSE;
    return Check(::SQLSetConnectAttr(m_hdbc,SQL_AUTOCOMMIT,reinterpret_cast<SQLPOINTER>(UINT_PTR(SQL_AUTOCOMMIT_OFF)),0));
}
BOOL CDatabase::CommitTrans() {
    if(!m_bTransactions)return FALSE;
    BOOL result=Check(::SQLTransact(AfxGetHENV(),m_hdbc,SQL_COMMIT));
    ::SQLSetConnectAttr(m_hdbc,SQL_AUTOCOMMIT,reinterpret_cast<SQLPOINTER>(UINT_PTR(SQL_AUTOCOMMIT_ON)),0);
    return result;
}
BOOL CDatabase::Rollback() {
    if(!m_bTransactions)return FALSE;
    BOOL result=Check(::SQLTransact(AfxGetHENV(),m_hdbc,SQL_ROLLBACK));
    ::SQLSetConnectAttr(m_hdbc,SQL_AUTOCOMMIT,reinterpret_cast<SQLPOINTER>(UINT_PTR(SQL_AUTOCOMMIT_ON)),0);
    return result;
}
void CDatabase::Cancel() {::SQLCancel(m_hstmt);}
// Symbol: ?BeginTrans@CDatabase@@QEAAHXZ
extern "C" BOOL MS_ABI impl__BeginTrans_CDatabase__QEAAHXZ(CDatabase* database) {return database->BeginTrans();}
// Symbol: ?CommitTrans@CDatabase@@QEAAHXZ
extern "C" BOOL MS_ABI impl__CommitTrans_CDatabase__QEAAHXZ(CDatabase* database) {return database->CommitTrans();}
// Symbol: ?Rollback@CDatabase@@QEAAHXZ
extern "C" BOOL MS_ABI impl__Rollback_CDatabase__QEAAHXZ(CDatabase* database) {return database->Rollback();}
// Symbol: ?Cancel@CDatabase@@QEAAXXZ
extern "C" void MS_ABI impl__Cancel_CDatabase__QEAAXXZ(CDatabase* database) {database->Cancel();}
