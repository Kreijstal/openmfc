// Release x64 IsOpen entry 0x1f8710 in the verified mfc140u image.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include <cwchar>
// Symbol: ?IsOpen@CRecordset@@QEBAHXZ
extern "C" BOOL MS_ABI impl__IsOpen_CRecordset__QEBAHXZ(const CRecordset* recordset) {
    if(!recordset->m_hstmt)return FALSE;
    if(recordset->m_lOpen==1)return TRUE;
    SQLSMALLINT columns;
    RETCODE code;
    do {code=::SQLNumResultCols(recordset->m_hstmt,&columns);}while(code==SQL_STILL_EXECUTING);
    if(recordset->Check(code))return columns!=0;
    auto* exception=new CDBException(code);
    exception->BuildErrorString(recordset->m_pDatabase,recordset->m_hstmt,FALSE);
    auto* data=reinterpret_cast<const CStringData*>(exception->m_strStateNativeOrigin.GetString())-1;
    if(data->nDataLength>=0 && std::wcsstr(exception->m_strStateNativeOrigin.GetString(),L"State:S1010")) {
        exception->Delete();return FALSE;
    }
    throw exception;
}
BOOL CRecordset::IsOpen() const {return impl__IsOpen_CRecordset__QEBAHXZ(this);}
