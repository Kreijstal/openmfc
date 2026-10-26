// Native release x64 Open, RVA 0x1f8380.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__BuildSQL_CRecordset__IEAAXPEB_W_Z(CRecordset*,const wchar_t*);
extern "C" void MS_ABI impl__PrepareAndExecute_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__AllocAndCacheFieldInfo_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__AllocRowset_CRecordset__IEAAXXZ(CRecordset*);
BOOL CRecordset::Open(UINT type, const wchar_t* sql, DWORD options) {
    if (IsOpen() || !this || (type > 3 && type != UINT(AFX_DB_USE_DEFAULT_TYPE)) ||
        ((options & readOnly) && (options & appendOnly)) ||
        ((options & optimizeBulkAdd) && !(options & appendOnly)) ||
        (type == forwardOnly && (options & skipDeletedRecords))) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    // Native exception cleanup starts only after these two helpers succeed.
    SetState(type, sql, options);
    if (!AllocHstmt()) return FALSE;
    try {
        OnSetOptions(m_hstmt);
        bool noFields = !m_nFields && !m_nParams;
        if (!noFields) AllocStatusArrays();
        impl__BuildSQL_CRecordset__IEAAXPEB_W_Z(this, sql);
        impl__PrepareAndExecute_CRecordset__IEAAXXZ(this);
        impl__AllocAndCacheFieldInfo_CRecordset__IEAAXXZ(this);
        impl__AllocRowset_CRecordset__IEAAXXZ(this);
        if (noFields && (m_nFields || m_nParams)) AllocStatusArrays();
        PreBindFields();
        Move(1, SQL_FETCH_NEXT);
        m_bBOF = m_bEOF;
        return TRUE;
    } catch (CException*) {
        Close();
        throw;
    }
}
// Symbol: ?Open@CRecordset@@UEAAHIPEB_WK@Z
extern "C" BOOL MS_ABI impl__Open_CRecordset__UEAAHIPEB_WK_Z(CRecordset* rs,UINT type,const wchar_t* sql,DWORD options) {
    return rs->CRecordset::Open(type, sql, options);
}
