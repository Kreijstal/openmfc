// Native release x64 PrepareAndExecute, RVA 0x1fa3a0.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include <cwchar>
// Symbol: ?PrepareAndExecute@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__PrepareAndExecute_CRecordset__IEAAXXZ(CRecordset* rs) {
    const wchar_t* sql = rs->m_strSQL.GetString();
    RETCODE code;
    for (;;) {
        if (rs->m_dwOptions & CRecordset::executeDirect) {
            do { code = ::SQLExecDirectW(rs->m_hstmt, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(sql)), SQL_NTS); }
            while (code == SQL_STILL_EXECUTING);
        } else {
            do { code = ::SQLPrepareW(rs->m_hstmt, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(sql)), SQL_NTS); }
            while (code == SQL_STILL_EXECUTING);
        }
        if (rs->Check(code)) break;
        auto* error = new CDBException(code);
        error->BuildErrorString(rs->m_pDatabase, rs->m_hstmt, TRUE);
        if (rs->m_dwConcurrency == SQL_CONCUR_READ_ONLY ||
            !std::wcsstr(error->m_strStateNativeOrigin.GetString(), L"State:S1C00"))
            throw error;
        error->Delete();
        if ((rs->m_dwDriverConcurrency & SQL_SCCO_LOCK) &&
            (rs->m_dwConcurrency == SQL_CONCUR_ROWVER || rs->m_dwConcurrency == SQL_CONCUR_VALUES))
            rs->m_dwConcurrency = SQL_CONCUR_LOCK;
        else {
            rs->m_bUpdatable = rs->m_bAppendable = FALSE;
            rs->m_dwConcurrency = SQL_CONCUR_READ_ONLY;
        }
        code = ::SQLSetStmtOption(rs->m_hstmt, SQL_CONCURRENCY, rs->m_dwConcurrency);
        if (!rs->Check(code)) rs->ThrowDBException(code, nullptr);
    }
    if (!(rs->m_dwOptions & CRecordset::executeDirect)) {
        do { code = ::SQLExecute(rs->m_hstmt); } while (code == SQL_STILL_EXECUTING);
        if (!rs->Check(code)) rs->ThrowDBException(code, nullptr);
    }
    rs->m_lOpen = 1;
    if (code == SQL_SUCCESS_WITH_INFO) {
        SQLULEN concurrency;
        SQLINTEGER bytes;
        code = ::SQLGetStmtAttrW(rs->m_hstmt, SQL_ATTR_CONCURRENCY, &concurrency, 0, &bytes);
        if (!rs->Check(code)) rs->ThrowDBException(code, nullptr);
        if (concurrency == SQL_CONCUR_READ_ONLY && (rs->m_bUpdatable || rs->m_bAppendable))
            rs->m_bUpdatable = rs->m_bAppendable = FALSE;
    }
}
