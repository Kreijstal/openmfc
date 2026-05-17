// Database Implementation
// Provides MFC wrappers around ODBC API
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CDBException
//=============================================================================
// No IMPLEMENT_DYNAMIC needed - CDBException doesn't derive from CObject

//=============================================================================
// CDatabase
//=============================================================================
IMPLEMENT_DYNAMIC(CDatabase, CObject)

CDatabase::CDatabase()
    : m_hdbc(SQL_NULL_HDBC), m_henv(SQL_NULL_HENV),
      m_bOpen(FALSE), m_bReadOnly(FALSE) {
    memset(_cdatabase_padding, 0, sizeof(_cdatabase_padding));
}

CDatabase::~CDatabase() {
    Close();
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

    SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Allocate connection handle
    if (SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc) != SQL_SUCCESS) {
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_henv = SQL_NULL_HENV;
        return FALSE;
    }

    // Set read-only if specified
    if (m_bReadOnly) {
        SQLSetConnectAttr(m_hdbc, SQL_ATTR_ACCESS_MODE,
                         (SQLPOINTER)SQL_MODE_READ_ONLY, 0);
    }

    // Connect
    SQLWCHAR szConnStr[1024] = {};
    if (lpszConnect && lpszConnect[0]) {
        wcscpy(szConnStr, lpszConnect);
    } else {
        swprintf(szConnStr, 1024, L"DSN=%s;", lpszDSN ? lpszDSN : L"");
    }
    m_strConnect = szConnStr;

    SQLWCHAR szOutStr[1024] = {};
    SQLSMALLINT cbOut = 0;
    RETCODE rc = SQLDriverConnectW(m_hdbc, nullptr, szConnStr, SQL_NTS,
                                    szOutStr, sizeof(szOutStr)/sizeof(SQLWCHAR), &cbOut,
                                    SQL_DRIVER_NOPROMPT);
    if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_hdbc = SQL_NULL_HDBC;
        m_henv = SQL_NULL_HENV;
        return FALSE;
    }

    m_bOpen = TRUE;
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
}

BOOL CDatabase::BeginTrans() {
    if (!m_bOpen) return FALSE;
    return SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
                            (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER) == SQL_SUCCESS;
}

BOOL CDatabase::CommitTrans() {
    if (!m_bOpen) return FALSE;
    BOOL result = SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_COMMIT) == SQL_SUCCESS;
    SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
                     (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
    return result;
}

BOOL CDatabase::Rollback() {
    if (!m_bOpen) return FALSE;
    BOOL result = SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_ROLLBACK) == SQL_SUCCESS;
    SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
                     (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
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
    return 15;
}

void CDatabase::SetLoginTimeout(int nSeconds) {
    (void)nSeconds;
}

int CDatabase::GetQueryTimeout() {
    return 0;
}

void CDatabase::SetQueryTimeout(int nSeconds) {
    (void)nSeconds;
}

//=============================================================================
// CFieldExchange
//=============================================================================
CFieldExchange::CFieldExchange(RFX_Operation op, CRecordset* pRecordset)
    : m_pRecordset(pRecordset), m_nOperation(op), m_nFields(0), m_nParams(0) {
    memset(_cfieldexchange_padding, 0, sizeof(_cfieldexchange_padding));
}

CFieldExchange::~CFieldExchange() {}

BOOL CFieldExchange::IsFieldType(UINT* pnField) {
    (void)pnField;
    return TRUE;
}

void CFieldExchange::SetFieldType(UINT* pnField) {
    (void)pnField;
}

void CFieldExchange::Default(const wchar_t* lpszName, void* pv, __int64* pnLen,
                              int nSQLType, unsigned __int64 nLen, void* pvPrecision) {
    (void)lpszName; (void)pv; (void)pnLen;
    (void)nSQLType; (void)nLen; (void)pvPrecision;
}

void CFieldExchange::SetNull(const wchar_t* lpszName) {
    (void)lpszName;
}

//=============================================================================
// CRecordset
//=============================================================================
IMPLEMENT_DYNAMIC(CRecordset, CObject)

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
}

BOOL CRecordset::Open(UINT nOpenType, const wchar_t* lpszSQL, DWORD dwOptions) {
    if (m_bOpen) return FALSE;

    m_nOpenType = nOpenType;
    m_dwOptions = dwOptions;

    if (!m_pDatabase || !m_pDatabase->IsOpen()) {
        return FALSE;
    }

    // Allocate statement handle
    if (SQLAllocHandle(SQL_HANDLE_STMT, m_pDatabase->GetHDBC(), &m_hstmt) != SQL_SUCCESS) {
        return FALSE;
    }

    // Set options
    OnSetOptions(m_hstmt);

    // Get SQL string
    CString strSQL = lpszSQL ? CString(lpszSQL) : GetDefaultSQL();
    if (strSQL.IsEmpty()) {
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

    m_bOpen = TRUE;
    m_bBOF = TRUE;
    m_bEOF = FALSE;
    m_nAbsolutePosition = 0;
    m_nRecordCount = 0;

    return TRUE;
}

void CRecordset::Close() {
    if (m_hstmt != SQL_NULL_HSTMT) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
    }
    m_bOpen = FALSE;
}

BOOL CRecordset::IsDeleted() const {
    return m_bDeleted;
}

BOOL CRecordset::IsFieldDirty(void* pvField) {
    (void)pvField;
    return FALSE;
}

BOOL CRecordset::IsFieldNull(void* pvField) {
    (void)pvField;
    return FALSE;
}

BOOL CRecordset::IsFieldNullable(unsigned long dwField) {
    (void)dwField;
    return TRUE;
}

void CRecordset::MoveFirst() {
    if (!m_bOpen) return;
    if (SQLFetchScroll(m_hstmt, SQL_FETCH_FIRST, 0) == SQL_SUCCESS) {
        m_bBOF = FALSE;
        m_bEOF = FALSE;
        m_nAbsolutePosition = 1;
    }
}

void CRecordset::MoveLast() {
    if (!m_bOpen) return;
    if (SQLFetchScroll(m_hstmt, SQL_FETCH_LAST, 0) == SQL_SUCCESS) {
        m_bBOF = FALSE;
        m_bEOF = FALSE;
    }
}

void CRecordset::MoveNext() {
    if (!m_bOpen || m_bEOF) return;
    RETCODE rc = SQLFetchScroll(m_hstmt, SQL_FETCH_NEXT, 0);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        m_bBOF = FALSE;
        m_nAbsolutePosition++;
    } else {
        m_bEOF = TRUE;
    }
}

void CRecordset::MovePrev() {
    if (!m_bOpen || m_bBOF) return;
    RETCODE rc = SQLFetchScroll(m_hstmt, SQL_FETCH_PRIOR, 0);
    if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) {
        m_bEOF = FALSE;
        if (m_nAbsolutePosition > 0) m_nAbsolutePosition--;
    } else {
        m_bBOF = TRUE;
    }
}

void CRecordset::Move(long nRows, WORD wFetchType) {
    if (!m_bOpen) return;
    SQLFetchScroll(m_hstmt, wFetchType, nRows);
}

void CRecordset::SetAbsolutePosition(long nRows) {
    m_nAbsolutePosition = nRows;
}

void CRecordset::AddNew() {
    m_nEditMode = 1; // addNew
}

void CRecordset::Edit() {
    m_nEditMode = 2; // edit
}

void CRecordset::Delete() {
    m_nEditMode = 3; // delete
    m_bDeleted = TRUE;
}

BOOL CRecordset::Update() {
    m_nEditMode = 0;
    return TRUE;
}

void CRecordset::CancelUpdate() {
    m_nEditMode = 0;
}

void CRecordset::SetFieldNull(void* pvField, BOOL bNull) {
    (void)pvField; (void)bNull;
}

BOOL CRecordset::CanAppend() const {
    return TRUE;
}

BOOL CRecordset::CanRestart() const {
    return TRUE;
}

BOOL CRecordset::CanScroll() const {
    return TRUE;
}

BOOL CRecordset::CanTransact() const {
    return m_pDatabase ? m_pDatabase->CanTransact() : FALSE;
}

BOOL CRecordset::CanUpdate() const {
    return TRUE;
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
    (void)pFX;
}

BOOL CRecordset::OnSetOptions(HSTMT hstmt) {
    (void)hstmt;
    return TRUE;
}

void CRecordset::OnFieldChange(void* pvField, LONG* plLength) {
    (void)pvField; (void)plLength;
}

void CRecordset::SetDirtyFieldStatus(DWORD dwFieldStatus) {
    (void)dwFieldStatus;
}

void CRecordset::ClearDirtyFieldStatus(DWORD dwFieldStatus) {
    (void)dwFieldStatus;
}

void CRecordset::SetParamNull(int nIndex, BOOL bNull) {
    (void)nIndex; (void)bNull;
}

//=============================================================================
// CRecordView
//=============================================================================
IMPLEMENT_DYNAMIC(CRecordView, CFormView)

CRecordView::CRecordView(UINT nIDTemplate)
    : CFormView(nIDTemplate), m_pSet(nullptr),
      m_bOnFirstRecord(FALSE), m_bOnLastRecord(FALSE) {
    memset(_crecordview_padding, 0, sizeof(_crecordview_padding));
}

CRecordView::CRecordView(const wchar_t* lpszTemplateName)
    : CFormView(lpszTemplateName), m_pSet(nullptr),
      m_bOnFirstRecord(FALSE), m_bOnLastRecord(FALSE) {
    memset(_crecordview_padding, 0, sizeof(_crecordview_padding));
}

CRecordView::~CRecordView() {
}

BOOL CRecordView::OnMove(UINT nIDMoveCommand) {
    (void)nIDMoveCommand;
    return TRUE;
}

CRecordset* CRecordView::OnGetRecordset() {
    return m_pSet;
}

BOOL CRecordView::IsOnFirstRecord() {
    return m_bOnFirstRecord;
}

BOOL CRecordView::IsOnLastRecord() {
    return m_bOnLastRecord;
}

// Symbol: ??0CRecordset@@QEAA@PEAVCDatabase@@@Z
extern "C" void* MS_ABI impl___0CRecordset__QEAA_PEAVCDatabase___Z(void* pThis, CDatabase* pDatabase) {
    return new(pThis) CRecordset(pDatabase);
}

// Symbol: ??0CFieldExchange@@QEAA@IPEAVCRecordset@@PEAX@Z
extern "C" void* MS_ABI impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(
    void* pThis, UINT op, CRecordset* pRecordset, void* /*pMapField*/) {
    return new(pThis) CFieldExchange((CFieldExchange::RFX_Operation)op, pRecordset);
}

// Symbol: ?GetRuntimeClass@CDatabase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDatabase__UEBAPEAUCRuntimeClass__XZ(const CDatabase* pThis) {
    (void)pThis;
    return CDatabase::GetThisClass();
}

// Symbol: ?GetThisClass@CDatabase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDatabase__SAPEAUCRuntimeClass__XZ() {
    return CDatabase::GetThisClass();
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

// Symbol: ?GetRuntimeClass@CRecordView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRecordView__UEBAPEAUCRuntimeClass__XZ(const CRecordView* pThis) {
    (void)pThis;
    return CRecordView::GetThisClass();
}

// Symbol: ?GetThisClass@CRecordView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRecordView__SAPEAUCRuntimeClass__XZ() {
    return CRecordView::GetThisClass();
}

// Symbol: ?GetMessageMap@CRecordView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CRecordView__MEBAPEBUAFX_MSGMAP__XZ(const CRecordView* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CRecordView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CRecordView__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}

// Symbol: ?IsOpen@CRecordset@@QEBAHXZ
extern "C" int MS_ABI impl__IsOpen_CRecordset__QEBAHXZ(const CRecordset* pThis) {
    return (pThis && pThis->IsOpen()) ? TRUE : FALSE;
}

// Symbol: ?OpenEx@CDatabase@@UEAAHPEB_WK@Z
extern "C" int MS_ABI impl__OpenEx_CDatabase__UEAAHPEB_WK_Z(CDatabase* pThis, const wchar_t* lpszConnectString, DWORD dwOptions) {
    if (!pThis) return FALSE;
    constexpr DWORD kOpenReadOnlyOption = 0x00000001UL;
    const BOOL bReadOnly = (dwOptions & kOpenReadOnlyOption) ? TRUE : FALSE;
    return pThis->Open(nullptr, FALSE, bReadOnly, lpszConnectString, TRUE);
}

// Symbol: ?OnSetOptions@CDatabase@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnSetOptions_CDatabase__UEAAXPEAX_Z(CDatabase* /*pThis*/, void* /*hstmt*/) {
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

    CString sql = pThis->GetDefaultSQL();
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

// Symbol: ?OnInitialUpdate@CRecordView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CRecordView__UEAAXXZ(CRecordView* pThis) {
    if (!pThis) return;
    pThis->m_pSet = pThis->OnGetRecordset();
    pThis->m_bOnFirstRecord = TRUE;
    pThis->m_bOnLastRecord = FALSE;
}

// Symbol: ?OnUpdateRecordFirst@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordFirst_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* /*pThis*/, CCmdUI* /*pCmdUI*/) {
}

// Symbol: ?OnUpdateRecordLast@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordLast_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* /*pThis*/, CCmdUI* /*pCmdUI*/) {
}

// Symbol: ?OnUpdateRecordNext@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordNext_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* /*pThis*/, CCmdUI* /*pCmdUI*/) {
}

// Symbol: ?OnUpdateRecordPrev@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordPrev_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* /*pThis*/, CCmdUI* /*pCmdUI*/) {
}
