// Database Implementation
// Provides MFC wrappers around ODBC API
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxole.h"
#include <algorithm>
#include <cstdint>
#include <cwctype>
#include <cstring>
#include <limits>
#include <mutex>
#include <new>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
std::mutex g_recordsetStateMutex;
std::unordered_map<const CRecordset*, CString> g_recordsetSql;

struct OdbcFieldInfoCompat {
    CString m_strName;
    short m_nSQLType = SQL_VARCHAR;
    unsigned long m_nPrecision = 0;
    short m_nScale = 0;
    short m_nNullability = SQL_NULLABLE_UNKNOWN;
};

struct LongBinaryCompat {
    HGLOBAL m_hData = nullptr;
    DWORD m_dwDataLength = 0;
};

struct DbState {
    CString databaseName;
    CString connectInfo;
    RETCODE lastRetCode = SQL_SUCCESS;
};

struct RecordsetState {
    CString baseSql;
    CString currentSql;
    std::vector<OdbcFieldInfoCompat> fields;
    std::unordered_set<unsigned long> dirtyFields;
    std::unordered_set<unsigned long> nullFields;
    std::unordered_set<unsigned long> nullParams;
    std::unordered_map<void*, int> boundFieldIndexes;
    std::unordered_map<void*, int> boundParamIndexes;
    std::vector<SQLLEN> fieldLengths;
    std::vector<unsigned char> dataCache;
    long bookmark = 0;
    bool bookmarksEnabled = false;
    bool rowsetAllocated = false;
    bool updatePrepared = false;
    bool fieldsLoaded = false;
    RETCODE lastRetCode = SQL_SUCCESS;
    unsigned int lockingMode = 0;
};

std::unordered_map<const CDatabase*, DbState> g_databaseStates;
std::unordered_map<const CRecordset*, RecordsetState> g_recordsetStates;
int g_loginTimeoutSeconds = 15;
int g_queryTimeoutSeconds = 0;

std::wstring WideOf(const wchar_t* value) {
    return value ? std::wstring(value) : std::wstring();
}

std::wstring LowerSql(const wchar_t* value) {
    std::wstring text = WideOf(value);
    std::transform(text.begin(), text.end(), text.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
    return text;
}

bool SqlSucceeded(RETCODE rc) {
    return rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO;
}

inline SQLPOINTER CastOdbcOptionValue(std::uint64_t value) {
    return reinterpret_cast<SQLPOINTER>(static_cast<std::uintptr_t>(value));
}

RecordsetState& EnsureRecordsetState(const CRecordset* recordset) {
    return g_recordsetStates[recordset];
}

void LoadRecordsetFields(CRecordset* recordset) {
    if (!recordset) return;
    RecordsetState& state = EnsureRecordsetState(recordset);
    if (state.fieldsLoaded && !state.fields.empty()) return;
    state.fields.clear();
    SQLSMALLINT count = 0;
    if (recordset->m_hstmt != SQL_NULL_HSTMT && SqlSucceeded(SQLNumResultCols(recordset->m_hstmt, &count))) {
        for (SQLSMALLINT i = 1; i <= count; ++i) {
            SQLWCHAR name[256] = {};
            SQLSMALLINT nameLen = 0;
            SQLSMALLINT dataType = SQL_VARCHAR;
            SQLULEN columnSize = 0;
            SQLSMALLINT decimalDigits = 0;
            SQLSMALLINT nullable = SQL_NULLABLE_UNKNOWN;
            SQLDescribeColW(recordset->m_hstmt, static_cast<SQLUSMALLINT>(i), name, 256, &nameLen,
                            &dataType, &columnSize, &decimalDigits, &nullable);
            OdbcFieldInfoCompat info;
            info.m_strName = reinterpret_cast<const wchar_t*>(name);
            info.m_nSQLType = dataType;
            info.m_nPrecision = static_cast<unsigned long>(columnSize);
            info.m_nScale = decimalDigits;
            info.m_nNullability = nullable;
            state.fields.push_back(info);
        }
    }
    if (state.fields.empty() && recordset->m_nFields > 0) {
        for (int i = 0; i < recordset->m_nFields; ++i) {
            OdbcFieldInfoCompat info;
            wchar_t name[32] = {};
            std::swprintf(name, 32, L"Field%d", i + 1);
            info.m_strName = name;
            state.fields.push_back(info);
        }
    }
    state.fieldLengths.resize(state.fields.size());
    state.fieldsLoaded = true;
}

int FindFieldByName(CRecordset* recordset, const wchar_t* name) {
    if (!recordset || !name) return -1;
    LoadRecordsetFields(recordset);
    const std::wstring target = LowerSql(name);
    const auto& fields = EnsureRecordsetState(recordset).fields;
    for (size_t i = 0; i < fields.size(); ++i) {
        if (LowerSql(static_cast<const wchar_t*>(fields[i].m_strName)) == target) return static_cast<int>(i);
    }
    return -1;
}

CString GetStoredSql(CRecordset* recordset) {
    CString sql;
    if (!recordset) return sql;
    auto itState = g_recordsetStates.find(recordset);
    if (itState != g_recordsetStates.end() && !itState->second.currentSql.IsEmpty()) return itState->second.currentSql;
    auto it = g_recordsetSql.find(recordset);
    return it == g_recordsetSql.end() ? CString() : it->second;
}

void StoreSql(CRecordset* recordset, const CString& sql) {
    if (!recordset) return;
    g_recordsetSql[recordset] = sql;
    RecordsetState& state = EnsureRecordsetState(recordset);
    state.currentSql = sql;
    if (state.baseSql.IsEmpty()) state.baseSql = sql;
}
}

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

//=============================================================================
// CFieldExchange
//=============================================================================
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
    if (!m_pSet || !m_pSet->IsOpen()) return FALSE;

    switch (nIDMoveCommand) {
    case 0xE900: // ID_RECORD_FIRST
        m_pSet->MoveFirst();
        break;
    case 0xE901: // ID_RECORD_LAST
        m_pSet->MoveLast();
        break;
    case 0xE902: // ID_RECORD_NEXT
        m_pSet->MoveNext();
        break;
    case 0xE903: // ID_RECORD_PREV
        m_pSet->MovePrev();
        break;
    default:
        return FALSE;
    }

    m_bOnFirstRecord = m_pSet->IsBOF() || m_pSet->GetAbsolutePosition() <= 1;
    m_bOnLastRecord = m_pSet->IsEOF() || (m_pSet->GetRecordCount() > 0 && m_pSet->GetAbsolutePosition() >= m_pSet->GetRecordCount());
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
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetThisMessageMap@CRecordView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CRecordView__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?IsOpen@CRecordset@@QEBAHXZ
extern "C" int MS_ABI impl__IsOpen_CRecordset__QEBAHXZ(const CRecordset* pThis) {
    return (pThis && pThis->IsOpen()) ? TRUE : FALSE;
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

// Symbol: ?OnInitialUpdate@CRecordView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CRecordView__UEAAXXZ(CRecordView* pThis) {
    if (!pThis) return;
    pThis->m_pSet = pThis->OnGetRecordset();
    pThis->m_bOnFirstRecord = TRUE;
    pThis->m_bOnLastRecord = FALSE;
}

// Symbol: ?OnUpdateRecordFirst@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordFirst_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsBOF());
}

// Symbol: ?OnUpdateRecordLast@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordLast_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsEOF());
}

// Symbol: ?OnUpdateRecordNext@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordNext_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsEOF());
}

// Symbol: ?OnUpdateRecordPrev@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordPrev_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsBOF());
}

// Symbol: ?GetThisClass@CDBException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDBException__SAPEAUCRuntimeClass__XZ() {
    return CException::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CDBException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDBException__UEBAPEAUCRuntimeClass__XZ(const CDBException*) {
    return CException::GetThisClass();
}

// Symbol: ?Empty@CDBException@@QEAAXXZ
extern "C" void MS_ABI impl__Empty_CDBException__QEAAXXZ(CDBException* pThis) {
    if (!pThis) return;
    pThis->m_nRetCode = SQL_SUCCESS;
    pThis->m_strError = L"";
    pThis->m_strStateNativeOrigin = L"";
    pThis->m_strErrorNativeOrigin = L"";
}

// Symbol: ?BuildErrorString@CDBException@@UEAAXPEAVCDatabase@@PEAXH@Z
extern "C" void MS_ABI impl__BuildErrorString_CDBException__UEAAXPEAVCDatabase__PEAXH_Z(
    CDBException* pThis, CDatabase* database, void* hstmt, int) {
    if (!pThis) return;
    SQLSMALLINT handleType = hstmt ? SQL_HANDLE_STMT : SQL_HANDLE_DBC;
    SQLHANDLE handle = hstmt ? static_cast<SQLHANDLE>(hstmt) : (database ? static_cast<SQLHANDLE>(database->m_hdbc) : SQL_NULL_HANDLE);
    SQLWCHAR state[16] = {};
    SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH] = {};
    SQLINTEGER native = 0;
    SQLSMALLINT messageLen = 0;
    RETCODE rc = handle ? SQLGetDiagRecW(handleType, handle, 1, state, &native, message,
                                         SQL_MAX_MESSAGE_LENGTH, &messageLen) : SQL_NO_DATA;
    if (SqlSucceeded(rc)) {
        pThis->m_strStateNativeOrigin = reinterpret_cast<const wchar_t*>(state);
        pThis->m_strError = reinterpret_cast<const wchar_t*>(message);
        wchar_t nativeText[64] = {};
        std::swprintf(nativeText, 64, L"%ld", static_cast<long>(native));
        pThis->m_strErrorNativeOrigin = nativeText;
    } else if (pThis->m_strError.IsEmpty()) {
        pThis->m_strError = L"ODBC database operation failed.";
    }
}

// Symbol: ?GetErrorMessage@CDBException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CDBException__UEBAHPEA_WIPEAI_Z(
    const CDBException* pThis, wchar_t* error, unsigned int maxError, unsigned int* helpContext) {
    if (helpContext) *helpContext = 0;
    if (!error || maxError == 0) return FALSE;
    const wchar_t* text = (pThis && !pThis->m_strError.IsEmpty())
        ? static_cast<const wchar_t*>(pThis->m_strError)
        : L"ODBC database operation failed.";
    wcsncpy(error, text, maxError - 1);
    error[maxError - 1] = L'\0';
    return TRUE;
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

static unsigned int AppendCsvName(CString* sql, const wchar_t* name) {
    if (!sql || !name) return 0;
    if (!sql->IsEmpty()) *sql += L", ";
    *sql += name;
    return 1;
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

static void GetFieldTextByIndex(CRecordset* pThis, short index, CString* out) {
    if (!out) return;
    *out = L"";
    if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT || index < 0) return;
    wchar_t buffer[2048] = {};
    SQLLEN indicator = 0;
    if (SqlSucceeded(SQLGetData(pThis->m_hstmt, static_cast<SQLUSMALLINT>(index + 1), SQL_C_WCHAR, buffer, sizeof(buffer), &indicator)) && indicator != SQL_NULL_DATA) *out = buffer;
}

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
