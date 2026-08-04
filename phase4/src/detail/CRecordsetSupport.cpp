#include "CRecordsetSupport.h"

IMPLEMENT_DYNAMIC(CDatabase, CObject)
IMPLEMENT_DYNAMIC(CRecordset, CObject)
IMPLEMENT_DYNAMIC(CRecordView, CFormView)
namespace openmfc { namespace detail { namespace crecordset {
std::mutex g_recordsetStateMutex;
std::unordered_map<const CRecordset*, CString> g_recordsetSql;
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
unsigned int AppendCsvName(CString* sql, const wchar_t* name) {
    if (!sql || !name) return 0;
    if (!sql->IsEmpty()) *sql += L", ";
    *sql += name;
    return 1;
}
void GetFieldTextByIndex(CRecordset* pThis, short index, CString* out) {
    if (!out) return;
    *out = L"";
    if (!pThis || pThis->m_hstmt == SQL_NULL_HSTMT || index < 0) return;
    wchar_t buffer[2048] = {};
    SQLLEN indicator = 0;
    if (SqlSucceeded(SQLGetData(pThis->m_hstmt, static_cast<SQLUSMALLINT>(index + 1), SQL_C_WCHAR, buffer, sizeof(buffer), &indicator)) && indicator != SQL_NULL_DATA) *out = buffer;
}
} } }  // namespace openmfc::detail::crecordset
