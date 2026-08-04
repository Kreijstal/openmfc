// CDatabase — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"

// Symbol: ?GetRuntimeClass@CDatabase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDatabase__UEBAPEAUCRuntimeClass__XZ(const CDatabase* pThis) {
    (void)pThis;
    return CDatabase::GetThisClass();
}
// Symbol: ?GetThisClass@CDatabase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDatabase__SAPEAUCRuntimeClass__XZ() {
    return CDatabase::GetThisClass();
}
// Symbol: ?OpenEx@CDatabase@@UEAAHPEB_WK@Z
extern "C" int MS_ABI impl__OpenEx_CDatabase__UEAAHPEB_WK_Z(CDatabase* pThis, const wchar_t* lpszConnectString, DWORD dwOptions) {
    if (!pThis) return FALSE;
    constexpr DWORD kOpenReadOnlyOption = 0x00000004UL;
    const BOOL bReadOnly = (dwOptions & kOpenReadOnlyOption) ? TRUE : FALSE;
    return pThis->Open(nullptr, FALSE, bReadOnly, lpszConnectString, TRUE);
}
// Symbol: ?OnSetOptions@CDatabase@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnSetOptions_CDatabase__UEAAXPEAX_Z(CDatabase* pThis, void* hstmt) {
    if (!pThis || !hstmt) return;
    HSTMT stmt = reinterpret_cast<HSTMT>(hstmt);
    if (stmt == SQL_NULL_HSTMT) return;
    SQLSetStmtAttr(stmt, SQL_ATTR_QUERY_TIMEOUT,
                   CastOdbcOptionValue(g_queryTimeoutSeconds),
                   SQL_IS_UINTEGER);
    SQLSetStmtAttr(stmt, SQL_ATTR_CURSOR_TYPE, CastOdbcOptionValue(SQL_CURSOR_DYNAMIC), 0);
}
// Symbol: ?ReplaceBrackets@CDatabase@@QEAAXPEA_W@Z
extern "C" void MS_ABI impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(CDatabase* /*pThis*/, wchar_t* lpszSQL) {
    if (!lpszSQL) return;
    for (wchar_t* p = lpszSQL; *p; ++p) {
        if (*p == L'[' || *p == L']') {
            *p = L'"';
        }
    }
}
// Symbol: ?AllocConnect@CDatabase@@IEAAXK@Z
extern "C" void MS_ABI impl__AllocConnect_CDatabase__IEAAXK_Z(CDatabase* pThis, unsigned long) {
    if (!pThis || pThis->m_hdbc != SQL_NULL_HDBC) return;
    if (pThis->m_henv == SQL_NULL_HENV && SqlSucceeded(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &pThis->m_henv))) {
        SQLSetEnvAttr(pThis->m_henv, SQL_ATTR_ODBC_VERSION, CastOdbcOptionValue(SQL_OV_ODBC3), 0);
    }
    if (pThis->m_henv != SQL_NULL_HENV) SQLAllocHandle(SQL_HANDLE_DBC, pThis->m_henv, &pThis->m_hdbc);
}
// Symbol: ?Free@CDatabase@@IEAAXXZ
extern "C" void MS_ABI impl__Free_CDatabase__IEAAXXZ(CDatabase* pThis) {
    if (pThis) pThis->Close();
}
// Symbol: ?Connect@CDatabase@@IEAAHK@Z
extern "C" int MS_ABI impl__Connect_CDatabase__IEAAHK_Z(CDatabase* pThis, unsigned long) {
    if (!pThis) return FALSE;
    if (pThis->m_hdbc == SQL_NULL_HDBC) impl__AllocConnect_CDatabase__IEAAXK_Z(pThis, 0);
    pThis->m_bOpen = pThis->m_hdbc != SQL_NULL_HDBC;
    return pThis->m_bOpen;
}
// Symbol: ?BindParameters@CDatabase@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__BindParameters_CDatabase__UEAAXPEAX_Z(CDatabase* pThis, void* pBind) {
    if (!pThis || !pBind) return;
    HSTMT hstmt = static_cast<HSTMT>(pBind);
    if (hstmt != SQL_NULL_HSTMT) {
        SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
    }
    g_databaseStates[pThis].lastRetCode = SQL_SUCCESS;
}
// Symbol: ?Check@CDatabase@@UEBAHF@Z
extern "C" int MS_ABI impl__Check_CDatabase__UEBAHF_Z(const CDatabase* pThis, short retCode) {
    if (pThis) g_databaseStates[pThis].lastRetCode = retCode;
    return SqlSucceeded(retCode) ? TRUE : FALSE;
}
// Symbol: ?CheckHstmt@CDatabase@@QEBAHFPEAX@Z
extern "C" int MS_ABI impl__CheckHstmt_CDatabase__QEBAHFPEAX_Z(const CDatabase* pThis, short retCode, void*) {
    return impl__Check_CDatabase__UEBAHF_Z(pThis, retCode);
}
// Symbol: ?GetConnect@CDatabase@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetConnect_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* ret, const CDatabase* pThis) {
    new (ret) CString(pThis ? pThis->GetConnect() : CString());
}
// Symbol: ?GetDatabaseName@CDatabase@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDatabaseName_CDatabase__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* ret, const CDatabase* pThis) {
    if (!pThis) { new (ret) CString(); return; }
    auto it = g_databaseStates.find(pThis);
    new (ret) CString((it != g_databaseStates.end() && !it->second.databaseName.IsEmpty()) ? it->second.databaseName : pThis->m_strConnect);
}
// Symbol: ?GetConnectInfo@CDatabase@@IEAAXXZ
extern "C" void MS_ABI impl__GetConnectInfo_CDatabase__IEAAXXZ(CDatabase* pThis) {
    if (pThis) g_databaseStates[pThis].connectInfo = pThis->m_strConnect;
}
// Symbol: ?VerifyConnect@CDatabase@@IEAAXXZ
extern "C" void MS_ABI impl__VerifyConnect_CDatabase__IEAAXXZ(CDatabase* pThis) {
    if (pThis && pThis->m_hdbc == SQL_NULL_HDBC) pThis->m_bOpen = FALSE;
}
CDatabase::CDatabase()
    : m_hdbc(SQL_NULL_HDBC), m_henv(SQL_NULL_HENV),
      m_bOpen(FALSE), m_bReadOnly(FALSE) {
    memset(_cdatabase_padding, 0, sizeof(_cdatabase_padding));
    g_databaseStates[this] = DbState{};
}
CDatabase::~CDatabase() {
    Close();
    g_databaseStates.erase(this);
}
BOOL CDatabase::Open(const wchar_t* lpszDSN, BOOL bExclusive, BOOL bReadOnly,
                      const wchar_t* lpszConnect, BOOL bUseCursorLib) {
    if (m_bOpen) return FALSE;

    m_bReadOnly = bReadOnly;
    (void)bExclusive;
    (void)bUseCursorLib;

    // Allocate environment handle
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv) != SQL_SUCCESS) {
        return FALSE;
    }

    SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, CastOdbcOptionValue(SQL_OV_ODBC3), 0);

    // Allocate connection handle
    if (SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc) != SQL_SUCCESS) {
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_henv = SQL_NULL_HENV;
        return FALSE;
    }

    // Set read-only if specified
    if (m_bReadOnly) {
        SQLSetConnectAttr(m_hdbc, SQL_ATTR_ACCESS_MODE,
                         CastOdbcOptionValue(SQL_MODE_READ_ONLY), 0);
    }

    // Connect
    SQLWCHAR szConnStr[1024] = {};
    if (lpszConnect && lpszConnect[0]) {
        wcscpy(szConnStr, lpszConnect);
    } else {
        swprintf(szConnStr, 1024, L"DSN=%s;", lpszDSN ? lpszDSN : L"");
    }
    m_strConnect = szConnStr;
    g_databaseStates[this].connectInfo = m_strConnect;
    if (lpszDSN) g_databaseStates[this].databaseName = lpszDSN;

    SQLWCHAR szOutStr[1024] = {};
    SQLSMALLINT cbOut = 0;
    RETCODE rc = SQLDriverConnectW(m_hdbc, nullptr, szConnStr, SQL_NTS,
                                    szOutStr, sizeof(szOutStr)/sizeof(SQLWCHAR), &cbOut,
                                    SQL_DRIVER_NOPROMPT);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        g_databaseStates[this].lastRetCode = rc;
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_hdbc = SQL_NULL_HDBC;
        m_henv = SQL_NULL_HENV;
        return FALSE;
    }

    m_bOpen = TRUE;
    g_databaseStates[this].lastRetCode = rc;
    return TRUE;
}
void CDatabase::Close() {
    if (m_hdbc != SQL_NULL_HDBC) {
        SQLDisconnect(m_hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        m_hdbc = SQL_NULL_HDBC;
    }
    if (m_henv != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_henv = SQL_NULL_HENV;
    }
    m_bOpen = FALSE;
    g_databaseStates[this].lastRetCode = SQL_SUCCESS;
}
BOOL CDatabase::BeginTrans() {
    if (!m_bOpen) return FALSE;
    return SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
                            CastOdbcOptionValue(SQL_AUTOCOMMIT_OFF), SQL_IS_UINTEGER) == SQL_SUCCESS;
}
BOOL CDatabase::CommitTrans() {
    if (!m_bOpen) return FALSE;
    BOOL result = SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_COMMIT) == SQL_SUCCESS;
    SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
                     CastOdbcOptionValue(SQL_AUTOCOMMIT_ON), SQL_IS_UINTEGER);
    return result;
}
BOOL CDatabase::Rollback() {
    if (!m_bOpen) return FALSE;
    BOOL result = SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_ROLLBACK) == SQL_SUCCESS;
    SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
                     CastOdbcOptionValue(SQL_AUTOCOMMIT_ON), SQL_IS_UINTEGER);
    return result;
}
BOOL CDatabase::ExecuteSQL(const wchar_t* lpszSQL) {
    if (!m_bOpen || !lpszSQL) return FALSE;
    HSTMT hstmt = SQL_NULL_HSTMT;
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &hstmt) != SQL_SUCCESS) {
        return FALSE;
    }
    RETCODE rc = SQLExecDirectW(hstmt, (SQLWCHAR*)lpszSQL, SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);
}
void CDatabase::Cancel() {
    if (m_hdbc == SQL_NULL_HDBC) return;
    RETCODE rc = SQLCancel(m_hdbc);
    g_databaseStates[this].lastRetCode = rc;
}
BOOL CDatabase::CanTransact() const {
    if (!m_bOpen) return FALSE;
    SQLUINTEGER val = 0;
    SQLGetInfo(m_hdbc, SQL_TXN_CAPABLE, &val, sizeof(val), nullptr);
    return val != SQL_TC_NONE;
}
BOOL CDatabase::CanUpdate() const {
    if (!m_bOpen) return FALSE;
    return !m_bReadOnly;
}
CString CDatabase::GetConnect() const {
    return m_strConnect;
}
int CDatabase::GetLoginTimeout() {
    return g_loginTimeoutSeconds;
}
void CDatabase::SetLoginTimeout(int nSeconds) {
    g_loginTimeoutSeconds = nSeconds >= 0 ? nSeconds : 0;
    for (auto& pair : g_databaseStates) {
        CDatabase* database = const_cast<CDatabase*>(pair.first);
        if (!database || database->m_hdbc == SQL_NULL_HDBC) continue;
        SQLSetConnectAttr(database->m_hdbc, SQL_ATTR_LOGIN_TIMEOUT,
                         CastOdbcOptionValue(g_loginTimeoutSeconds),
                         SQL_IS_UINTEGER);
    }
}
int CDatabase::GetQueryTimeout() {
    return g_queryTimeoutSeconds;
}
void CDatabase::SetQueryTimeout(int nSeconds) {
    g_queryTimeoutSeconds = nSeconds >= 0 ? nSeconds : 0;
    for (auto& pair : g_recordsetStates) {
        if (!pair.first || pair.first->m_hstmt == SQL_NULL_HSTMT) continue;
        SQLSetStmtAttr(pair.first->m_hstmt, SQL_ATTR_QUERY_TIMEOUT,
                       CastOdbcOptionValue(g_queryTimeoutSeconds),
                       SQL_IS_UINTEGER);
    }
}
