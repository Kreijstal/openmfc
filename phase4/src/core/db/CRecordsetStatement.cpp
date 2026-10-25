// Release x64 SetState 0x1f9e40 and AllocHstmt 0x1f9f20.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/CPtrListSupport.h"
#include "detail/DatabaseStringBuffer.h"
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);
namespace {
struct RegistrationLock {
    RegistrationLock() { impl__AfxLockGlobals__YAXH_Z(15); }
    ~RegistrationLock() { impl__AfxUnlockGlobals__YAXH_Z(15); }
};
}
void CRecordset::SetState(UINT type, const wchar_t* sql, DWORD options) {
    if (type == AFX_DB_USE_DEFAULT_TYPE) type = m_nDefaultType;
    m_nOpenType = type;
    // Native code computes these before forcing forward-only read-only mode.
    m_bAppendable = !(options & readOnly) || (options & appendOnly);
    m_bUpdatable = !(options & (readOnly | appendOnly));
    if (options & (noDirtyFieldCheck | useMultiRowFetch)) m_bCheckCacheForDirtyFields = FALSE;
    if (type == forwardOnly && !(options & readOnly)) {
        options |= readOnly;
        if (options & useMultiRowFetch) options |= useExtendedFetch;
    }
    m_dwOptions = options;
    openmfc::detail::DatabaseStringBuffer(m_strRequerySQL).set(sql);
    m_strRequeryFilter = m_strFilter;
    m_strRequerySort = m_strSort;
}
// Symbol: ?SetState@CRecordset@@IEAAXHPEB_WK@Z
extern "C" void MS_ABI impl__SetState_CRecordset__IEAAXHPEB_WK_Z(CRecordset* rs, int type, const wchar_t* sql, DWORD options) {
    rs->SetState(static_cast<UINT>(type), sql, options);
}
BOOL CRecordset::AllocHstmt() {
    if (m_hstmt) return TRUE;
    CString connection;
    if (!m_pDatabase) {
        m_pDatabase = new CDatabase;
        m_bRecordsetDb = TRUE;
    }
    {
        // SDK overrides use this-first return-buffer arguments. Construct their
        // result directly to avoid compiler-dependent value-return copies.
        alignas(CString) BYTE storage[sizeof(CString)];
        using Getter = CString* (MS_ABI*)(CRecordset*, CString*);
        auto** table = *reinterpret_cast<void***>(this);
        auto* returned = reinterpret_cast<Getter>(table[15])(this, reinterpret_cast<CString*>(storage));
        struct ReleaseResult {
            CString* value;
            ~ReleaseResult() { value->~CString(); }
        } release{returned};
        connection = *returned;
    }
    if (!m_pDatabase->m_hdbc) {
        BOOL cursors = m_bUseODBCCursorLib;
        if (m_nOpenType == snapshot && !(m_dwOptions & readOnly)) cursors = TRUE;
        if (!m_pDatabase->Open(L"", FALSE, FALSE, connection.GetString(), cursors)) return FALSE;
        if (m_nOpenType == snapshot && !cursors) {
            SQLUINTEGER scroll;
            SQLSMALLINT bytes;
            RETCODE code = ::SQLGetInfoW(m_pDatabase->m_hdbc, SQL_SCROLL_OPTIONS, &scroll, sizeof(scroll), &bytes);
            if (!Check(code)) ThrowDBException(code, nullptr);
            if (!(scroll & SQL_SO_STATIC)) {
                m_pDatabase->Close();
                if (!m_pDatabase->Open(L"", FALSE, FALSE, connection.GetString(), TRUE)) return FALSE;
            }
        }
    }
    RETCODE code = ::SQLAllocStmt(m_pDatabase->m_hdbc, &m_hstmt);
    if (!Check(code)) ThrowDBException(SQL_INVALID_HANDLE, nullptr);
    RegistrationLock lock;
    reinterpret_cast<OpenMFC_CPtrList*>(m_pDatabase->m_listRecordsets)->AddHead(this);
    return TRUE;
}
// Symbol: ?AllocHstmt@CRecordset@@IEAAHXZ
extern "C" BOOL MS_ABI impl__AllocHstmt_CRecordset__IEAAHXZ(CRecordset* rs) { return rs->AllocHstmt(); }
