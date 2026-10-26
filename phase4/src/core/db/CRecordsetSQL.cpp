// Native release x64 SQL construction and query analysis.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/DatabaseStringBuffer.h"
#include <cwchar>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" UINT MS_ABI impl__BindParams_CRecordset__QEAAIPEAX_Z(CRecordset*,void*);
extern "C" UINT MS_ABI impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,CString*,const wchar_t*);
extern "C" void MS_ABI impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(CDatabase*,wchar_t*);
namespace {
using openmfc::detail::DatabaseStringBuffer;
const wchar_t* findToken(const wchar_t* sql, const wchar_t* token) {
    if (!sql || !token) { impl__AfxThrowInvalidArgException__YAXXZ(); return nullptr; }
    CString upper(sql);
    auto* buffer = DatabaseStringBuffer(upper).get(upper.GetLength());
    ::_wcsupr_s(buffer, size_t(upper.GetLength()) + 1);
    size_t offset = 0, length = std::wcslen(token);
    for (;;) {
        const wchar_t* found = std::wcsstr(buffer + offset, token);
        if (!found) return nullptr;
        bool quoted = false;
        int opened = 0, closed = 0;
        for (const wchar_t* at = buffer; at < found; ++at) {
            if (*at == L'\'') {
                if (at[1] == L'\'') ++at;
                else quoted = !quoted;
            } else if (!quoted && (*at == L'[' || *at == L']')) {
                if (at[1] == *at) ++at;
                else if (*at == L'[') ++opened;
                else ++closed;
            }
        }
        if (!quoted && opened <= closed) return sql + (found - buffer);
        offset = size_t(found - buffer) + length;
    }
}
BOOL isJoin(const wchar_t* sql) {
    return findToken(sql, L",") || findToken(sql, L" JOIN ");
}
BOOL selectUpdatable(CRecordset* rs, const wchar_t* sql) {
    CString fragment(sql);
    const wchar_t* from = findToken(fragment.GetString(), L" FROM ");
    if (!from || findToken(fragment.GetString(), L" GROUP BY ") || findToken(fragment.GetString(), L" UNION ")) return FALSE;
    const wchar_t* where = findToken(fragment.GetString(), L" WHERE ");
    const wchar_t* order = findToken(fragment.GetString(), L" ORDER BY ");
    const wchar_t* end = !order || (where && where < order) ? where : order;
    if (end && end < from) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
    int length = int(end ? end - from : std::wcslen(from));
    auto* buffer = DatabaseStringBuffer(fragment).get(0);
    std::memmove(buffer, from, size_t(length) * sizeof(wchar_t));
    DatabaseStringBuffer(fragment).release(length);
    if (isJoin(fragment.GetString())) return FALSE;
    CString table(fragment.GetString() + (length < 6 ? length : 6));
    rs->m_strTableName = table;
    return TRUE;
}
BOOL sqlUpdatable(CRecordset* rs, const wchar_t* sql) {
    if (!sql) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
    // Native prefix comparisons omit each literal's final character.
    if (!::_wcsnicmp(sql, L"{CALL ", 5) || !::_wcsnicmp(sql, L"{?", 1)) return FALSE;
    return selectUpdatable(rs, sql);
}
BOOL recordsetUpdatable(CRecordset* rs) {
    if (!sqlUpdatable(rs, rs->m_strSQL.GetString())) return FALSE;
    if (!rs->m_bLongBinaryColumns || rs->m_bUseUpdateSQL) return TRUE;
    if (!rs->m_pDatabase) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
    DWORD options = rs->m_pDatabase->m_dwUpdateOptions;
    if (options & 4) return TRUE;
    if (!(options & 2)) return FALSE;
    rs->m_bUseUpdateSQL = TRUE;
    return TRUE;
}
void appendFilter(CRecordset* rs) {
    DatabaseStringBuffer sql(rs->m_strSQL);
    if (rs->m_strFilter.GetLength()) {
        sql.append(L" WHERE ");
        sql.append(rs->m_strFilter.GetString(), rs->m_strFilter.GetLength());
    }
    if (rs->m_strSort.GetLength()) {
        sql.append(L" ORDER BY ");
        sql.append(rs->m_strSort.GetString(), rs->m_strSort.GetLength());
    }
}
void buildSelect(CRecordset* rs) {
    if (!::_wcsnicmp(rs->m_strSQL.GetString(), L"{CALL ", 5) ||
        !::_wcsnicmp(rs->m_strSQL.GetString(), L"{?", 1) ||
        !::_wcsnicmp(rs->m_strSQL.GetString(), L"SELECT ", 6)) return;
    CString table;
    table = rs->m_strSQL;
    DatabaseStringBuffer sql(rs->m_strSQL);
    sql.empty();
    sql.set(L"SELECT ");
    rs->SetFieldDirty(nullptr, TRUE);
    if (!impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(rs, &rs->m_strSQL, L","))
        rs->ThrowDBException(0x3eb, nullptr);
    int length = rs->m_strSQL.GetLength();
    if (length < 1) { impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG); return; }
    sql.get(length)[length - 1] = L' ';
    sql.release(length);
    sql.append(L" FROM ");
    sql.append(table.GetString(), table.GetLength());
}
void buildSQL(CRecordset* rs, const wchar_t* source) {
    if (source) DatabaseStringBuffer(rs->m_strSQL).set(source);
    else {
        alignas(CString) BYTE storage[sizeof(CString)];
        using Getter = CString* (MS_ABI*)(CRecordset*, CString*);
        auto** table = *reinterpret_cast<void***>(rs);
        auto* result = reinterpret_cast<Getter>(table[16])(rs, reinterpret_cast<CString*>(storage));
        struct Release { CString* value; ~Release() { value->~CString(); } } release{result};
        rs->m_strSQL = *result;
    }
    if (rs->m_nParams) impl__BindParams_CRecordset__QEAAIPEAX_Z(rs, rs->m_hstmt);
    buildSelect(rs);
    appendFilter(rs);
    if ((rs->m_bUpdatable || rs->m_bAppendable) && !recordsetUpdatable(rs)) rs->m_bUpdatable = rs->m_bAppendable = FALSE;
    if (!rs->m_pDatabase) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    if (rs->m_bUpdatable && rs->m_bUseUpdateSQL && rs->m_pDatabase->m_bAddForUpdate)
        DatabaseStringBuffer(rs->m_strSQL).append(L" FOR UPDATE ");
    auto* buffer = DatabaseStringBuffer(rs->m_strSQL).get(0);
    impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(rs->m_pDatabase, buffer);
    DatabaseStringBuffer(rs->m_strSQL).release(-1);
}
}
// MSVC member string returns pass this first and caller storage second.
// These exports implement the qualified base bodies without virtual dispatch.
// Symbol: ?GetDefaultConnect@CRecordset@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetDefaultConnect_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CRecordset*, CString* output) {
    return new(output) CString(L"ODBC;");
}
// Symbol: ?GetDefaultSQL@CRecordset@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetDefaultSQL_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CRecordset*, CString* output) {
    return new(output) CString(L"");
}
// Symbol: ?FindSQLToken@CRecordset@@SAPEB_WPEB_W0@Z
extern "C" const wchar_t* MS_ABI impl__FindSQLToken_CRecordset__SAPEB_WPEB_W0_Z(const wchar_t* sql,const wchar_t* token) { return findToken(sql, token); }
// Symbol: ?IsJoin@CRecordset@@SAHPEB_W@Z
extern "C" BOOL MS_ABI impl__IsJoin_CRecordset__SAHPEB_W_Z(const wchar_t* sql) { return isJoin(sql); }
// Symbol: ?IsSelectQueryUpdatable@CRecordset@@QEAAHPEB_W@Z
extern "C" BOOL MS_ABI impl__IsSelectQueryUpdatable_CRecordset__QEAAHPEB_W_Z(CRecordset* rs,const wchar_t* sql) { return selectUpdatable(rs, sql); }
// Symbol: ?IsSQLUpdatable@CRecordset@@QEAAHPEB_W@Z
extern "C" BOOL MS_ABI impl__IsSQLUpdatable_CRecordset__QEAAHPEB_W_Z(CRecordset* rs,const wchar_t* sql) { return sqlUpdatable(rs, sql); }
// Symbol: ?IsRecordsetUpdatable@CRecordset@@IEAAHXZ
extern "C" BOOL MS_ABI impl__IsRecordsetUpdatable_CRecordset__IEAAHXZ(CRecordset* rs) { return recordsetUpdatable(rs); }
// Symbol: ?AppendFilterAndSortSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__AppendFilterAndSortSQL_CRecordset__IEAAXXZ(CRecordset* rs) { appendFilter(rs); }
// Symbol: ?BuildSelectSQL@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__BuildSelectSQL_CRecordset__IEAAXXZ(CRecordset* rs) { buildSelect(rs); }
// Symbol: ?BuildSQL@CRecordset@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__BuildSQL_CRecordset__IEAAXPEB_W_Z(CRecordset* rs,const wchar_t* sql) { buildSQL(rs, sql); }
