// Native release x64 driver negotiation and statement options.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
CDatabase* database(CRecordset* rs) {
    if (!rs->m_pDatabase) impl__AfxThrowInvalidArgException__YAXXZ();
    return rs->m_pDatabase;
}
void checked(CRecordset* rs, RETCODE code) {
    if (!rs->Check(code)) rs->ThrowDBException(code, nullptr);
}
void enableBookmarks(CRecordset* rs) {
    if (!(rs->m_dwOptions & CRecordset::useBookmarks)) return;
    auto* db = database(rs);
    if (db->m_dwBookmarkAttributes & SQL_BP_SCROLL)
        checked(rs, ::SQLSetStmtOption(rs->m_hstmt, SQL_USE_BOOKMARKS, SQL_UB_ON));
}
void verifyDriver(CRecordset* rs) {
    auto* db = database(rs);
    SQLUSMALLINT supported;
    checked(rs, ::SQLGetFunctions(db->m_hdbc, SQL_API_SQLEXTENDEDFETCH, &supported));
    rs->m_bScrollable = supported;
    if (!supported) {
        rs->m_bUpdatable = FALSE;
        return;
    }
    SQLWCHAR version[30];
    SQLSMALLINT bytes;
    checked(rs, ::SQLGetInfoW(db->m_hdbc, SQL_ODBC_VER, version, sizeof(version), &bytes));
    if (version[0] == L'0' && version[1] < L'2') rs->ThrowDBException(0x3fa, nullptr);
}
DWORD verifyCursor(CRecordset* rs) {
    auto* db = database(rs);
    SQLUINTEGER supported;
    SQLSMALLINT bytes;
    checked(rs, ::SQLGetInfoW(db->m_hdbc, SQL_SCROLL_OPTIONS, &supported, sizeof(supported), &bytes));
    if (rs->m_nOpenType == CRecordset::dynaset) {
        if (!(supported & SQL_SO_KEYSET_DRIVEN)) rs->ThrowDBException(0x3f4, nullptr);
        return SQL_CURSOR_KEYSET_DRIVEN;
    }
    if (rs->m_nOpenType == CRecordset::snapshot) {
        if (!(supported & SQL_SO_STATIC)) rs->ThrowDBException(0x3f5, nullptr);
        return SQL_CURSOR_STATIC;
    }
    if (!(supported & SQL_SO_DYNAMIC)) rs->ThrowDBException(0x401, nullptr);
    return SQL_CURSOR_DYNAMIC;
}
void setUpdateMethod(CRecordset* rs) {
    auto* db = database(rs);
    if (db->m_dwUpdateOptions & 1) rs->m_bUseUpdateSQL = FALSE;
    else if (db->m_dwUpdateOptions & 2) rs->m_bUseUpdateSQL = TRUE;
    else rs->m_bUpdatable = FALSE;
}
void setConcurrency(CRecordset* rs, HSTMT statement, DWORD cursor) {
    auto* db = database(rs);
    rs->m_dwConcurrency = SQL_CONCUR_READ_ONLY;
    if ((rs->m_bUpdatable || rs->m_bAppendable) && db->m_bUpdatable) {
        SQLSMALLINT bytes;
        checked(rs, ::SQLGetInfoW(db->m_hdbc, SQL_SCROLL_CONCURRENCY, &rs->m_dwDriverConcurrency,
                                  sizeof(rs->m_dwDriverConcurrency), &bytes));
        if (rs->m_nLockMode != 1 && (rs->m_dwDriverConcurrency & SQL_SCCO_OPT_ROWVER))
            rs->m_dwConcurrency = SQL_CONCUR_ROWVER;
        else if (rs->m_nLockMode != 1 && (rs->m_dwDriverConcurrency & SQL_SCCO_OPT_VALUES))
            rs->m_dwConcurrency = SQL_CONCUR_VALUES;
        else if (rs->m_dwDriverConcurrency & SQL_SCCO_LOCK)
            rs->m_dwConcurrency = SQL_CONCUR_LOCK;
    }
    checked(rs, ::SQLSetStmtOption(statement, SQL_CURSOR_TYPE, cursor));
    checked(rs, ::SQLSetStmtOption(statement, SQL_CONCURRENCY, rs->m_dwConcurrency));
}
}
// Symbol: ?CanBookmark@CRecordset@@QEBAHXZ
extern "C" BOOL MS_ABI impl__CanBookmark_CRecordset__QEBAHXZ(const CRecordset* rs) {
    auto* db = rs->m_pDatabase;
    if (!db || !db->m_hdbc) impl__AfxThrowInvalidArgException__YAXXZ();
    if (!(rs->m_dwOptions & CRecordset::useBookmarks) ||
        (rs->m_nOpenType == CRecordset::forwardOnly && !(rs->m_dwOptions & CRecordset::useExtendedFetch)))
        return FALSE;
    return db->m_dwBookmarkAttributes & SQL_BP_SCROLL;
}
// Symbol: ?EnableBookmarks@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__EnableBookmarks_CRecordset__IEAAXXZ(CRecordset* rs) { enableBookmarks(rs); }
// Symbol: ?VerifyDriverBehavior@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__VerifyDriverBehavior_CRecordset__IEAAXXZ(CRecordset* rs) { verifyDriver(rs); }
// Symbol: ?VerifyCursorSupport@CRecordset@@IEAAKXZ
extern "C" DWORD MS_ABI impl__VerifyCursorSupport_CRecordset__IEAAKXZ(CRecordset* rs) { return verifyCursor(rs); }
// Symbol: ?SetUpdateMethod@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__SetUpdateMethod_CRecordset__IEAAXXZ(CRecordset* rs) { setUpdateMethod(rs); }
// Symbol: ?SetConcurrencyAndCursorType@CRecordset@@IEAAXPEAXK@Z
extern "C" void MS_ABI impl__SetConcurrencyAndCursorType_CRecordset__IEAAXPEAXK_Z(CRecordset* rs, void* statement, DWORD cursor) {
    setConcurrency(rs, static_cast<HSTMT>(statement), cursor);
}
void CRecordset::OnSetOptions(HSTMT statement) {
    database(this)->OnSetOptions(statement);
    if (m_nOpenType == forwardOnly && !(m_dwOptions & useExtendedFetch)) return;
    enableBookmarks(this);
    if (m_nOpenType == forwardOnly) return;
    verifyDriver(this);
    DWORD cursor = verifyCursor(this);
    setUpdateMethod(this);
    setConcurrency(this, statement, cursor);
}
// Symbol: ?OnSetOptions@CRecordset@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnSetOptions_CRecordset__UEAAXPEAX_Z(CRecordset* rs, void* statement) {
    rs->CRecordset::OnSetOptions(static_cast<HSTMT>(statement));
}
