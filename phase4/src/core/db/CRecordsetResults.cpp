// Native release x64 result advancement, data retrieval and bookmarks.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);
extern "C" BOOL MS_ABI impl__CanBookmark_CRecordset__QEBAHXZ(const CRecordset*);
extern "C" void MS_ABI impl__Fixups_CRecordset__QEAAXXZ(CRecordset*);

// RVA 0x1fd8e0. Indicator values are full-width and belong to the driver;
// even SQL_NO_DATA is an exception on this static retrieval path.
SQLLEN CRecordset::GetData(CDatabase* db,HSTMT statement,short field,short type,void* buffer,SQLLEN capacity,short) {
    SQLLEN length;RETCODE code;
    do {code=::SQLGetData(statement,SQLUSMALLINT(field),type,buffer,capacity,&length);} while(code==SQL_STILL_EXECUTING);
    if (code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO)
        impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,db,statement);
    return length;
}
// Symbol: ?GetData@CRecordset@@SA_JPEAVCDatabase@@PEAXFF1_JF@Z
extern "C" SQLLEN MS_ABI impl__GetData_CRecordset__SA_JPEAVCDatabase__PEAXFF1_JF_Z(CDatabase* db,HSTMT statement,short field,short type,void* buffer,SQLLEN capacity,short sqlType) {return CRecordset::GetData(db,statement,field,type,buffer,capacity,sqlType);}

// RVA 0x1f91b0. Advance one result, reset currency and fix bound columns,
// including the final SQL_NO_DATA result when the virtual check accepts it.
BOOL CRecordset::FlushResultSet() {
    RETCODE code;
    do {code=::SQLMoreResults(m_hstmt);} while(code==SQL_STILL_EXECUTING);
    if (!Check(code)) impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,m_pDatabase,m_hstmt);
    ResetCursor();
    if (m_nFieldsBound>0) impl__Fixups_CRecordset__QEAAXXZ(this);
    return code!=SQL_NO_DATA;
}
// Symbol: ?FlushResultSet@CRecordset@@QEAAHXZ
extern "C" BOOL MS_ABI impl__FlushResultSet_CRecordset__QEAAHXZ(CRecordset* rs) {return rs->FlushResultSet();}

// RVAs 0x1f8cb0/0x1f8d90. The fixed bookmark is a Win32 long in CDBVariant;
// SetBookmark reads those bits and dispatches Move without type validation.
void CRecordset::GetBookmark(CDBVariant& value) {
    if (!(m_dwOptions&useBookmarks)) ThrowDBException(0x404,nullptr);
    else if (!impl__CanBookmark_CRecordset__QEBAHXZ(this)) ThrowDBException(0x403,nullptr);
    if (value.m_dwType!=DBVT_LONG) {value.Clear();value.m_dwType=DBVT_LONG;value.m_lVal=0;}
    RETCODE code;SQLLEN length;
    do {code=::SQLGetData(m_hstmt,0,SQL_C_BOOKMARK,&value.m_lVal,sizeof(long),&length);} while(code==SQL_STILL_EXECUTING);
    if (!Check(code)) ThrowDBException(code,nullptr);
}
void CRecordset::SetBookmark(const CDBVariant& value) {
    if (!(m_dwOptions&useBookmarks)) ThrowDBException(0x404,nullptr);
    else if (!impl__CanBookmark_CRecordset__QEBAHXZ(this)) ThrowDBException(0x403,nullptr);
    Move(value.m_lVal,SQL_FETCH_BOOKMARK);
}
// Symbol: ?GetBookmark@CRecordset@@QEAAXAEAVCDBVariant@@@Z
extern "C" void MS_ABI impl__GetBookmark_CRecordset__QEAAXAEAVCDBVariant___Z(CRecordset* rs,CDBVariant* value) {rs->GetBookmark(*value);}
// Symbol: ?SetBookmark@CRecordset@@QEAAXAEBVCDBVariant@@@Z
extern "C" void MS_ABI impl__SetBookmark_CRecordset__QEAAXAEBVCDBVariant___Z(CRecordset* rs,const CDBVariant* value) {rs->SetBookmark(*value);}
