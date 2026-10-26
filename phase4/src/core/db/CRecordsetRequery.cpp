// Native release x64 Requery, RVA 0x1f9b60.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include <cwchar>
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" void MS_ABI impl__RebindParams_CRecordset__QEAAXPEAX_Z(CRecordset*,void*);
namespace {
int compare(const CString& current,const CString& cached) noexcept {
    // Native CString::Compare has a no-throw boundary. Its null argument
    // failure terminates even when Requery has an outer framework catch.
    if (!cached.GetString()) impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
    return std::wcscmp(current.GetString(),cached.GetString());
}
}
BOOL CRecordset::Requery() {
    if (m_dwOptions&executeDirect) return FALSE;
    try {
        bool changed=compare(m_strFilter,m_strRequeryFilter)!=0;
        if (!changed) {
            changed=compare(m_strSort,m_strRequerySort)!=0;
        }
        if (changed) {
            m_strRequeryFilter=m_strFilter;
            m_strRequerySort=m_strSort;
            Close();
            return Open(m_nOpenType,m_strRequerySQL.GetLength()?m_strRequerySQL.GetString():nullptr,m_dwOptions);
        }
    } catch (CException*) {
        Close();
        throw;
    }
    // The native exception region starts at RebindParams, after this call and
    // the open-state write. A driver exception here must not trigger Close.
    ::SQLFreeStmt(m_hstmt,SQL_CLOSE);
    m_lOpen=0;
    try {
        impl__RebindParams_CRecordset__QEAAXPEAX_Z(this,m_hstmt);
        RETCODE code;
        do {code=::SQLExecute(m_hstmt);} while(code==SQL_STILL_EXECUTING);
        if (!Check(code)) ThrowDBException(code,nullptr);
        m_lOpen=1;
        m_bBOF=m_bEOF=FALSE;
        m_bEOFSeen=FALSE;m_lRecordCount=0;m_bDeleted=FALSE;m_lCurrentRecord=-1;
        Move(1,SQL_FETCH_NEXT);
        m_bBOF=m_bEOF;
        return TRUE;
    } catch (CException*) {
        Close();
        throw;
    }
}
// Symbol: ?Requery@CRecordset@@UEAAHXZ
extern "C" BOOL MS_ABI impl__Requery_CRecordset__UEAAHXZ(CRecordset* rs) {return rs->CRecordset::Requery();}
