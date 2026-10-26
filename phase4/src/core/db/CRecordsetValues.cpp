// Native release x64 field-value allocation and long-value continuation.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "openmfc/afxdao.h"
#include "detail/DatabaseFieldBuffer.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);
using openmfc::detail::DatabaseFieldBuffer;
namespace {
void initNarrow(void* object) noexcept {
    *static_cast<char**>(object)=reinterpret_cast<char*>(OpenMFC_GetStringMgr()->GetNilString()+1);
}
bool variableText(short type) {
    return type==SQL_WLONGVARCHAR || type==SQL_WVARCHAR || type==SQL_LONGVARBINARY || type==SQL_LONGVARCHAR || type==SQL_VARCHAR;
}
template<class Char> void longText(CDatabase* db,HSTMT statement,short field,SQLLEN needed,void** position,SQLLEN initial,void* string,short sqlType,short cType) {
    DatabaseFieldBuffer<Char> buffer(string);
    buffer.release(int32_t(std::min(needed,initial)));
    if(initial>needed || !variableText(sqlType))return;
    if(needed>INT_MAX-1)impl__AfxThrowMemoryException__YAXXZ();
    if(!position)impl__AfxThrowInvalidArgException__YAXXZ();
    auto* start=buffer.get(int32_t(uint32_t(needed)+1));
    SQLULEN offset=SQLULEN(initial)-1;
    // Both native overloads use two-byte continuation offsets/capacities,
    // including the CStringA specialization (RVA 0x1fdb60).
    *position=reinterpret_cast<void*>(UINT_PTR(start)+offset*2);
    SQLLEN capacity=SQLLEN((SQLULEN(needed)-offset)*2+2),read=0;RETCODE code;
    do {code=::SQLGetData(statement,SQLUSMALLINT(field),cType,*position,capacity,&read);}while(code==SQL_STILL_EXECUTING);
    if(code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO)
        impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,db,statement);
    buffer.release(int32_t(uint32_t(SQLULEN(read)>>1)+uint32_t(offset)));
}
void longBinary(CDatabase* db,HSTMT statement,short field,SQLLEN needed,void** position,SQLLEN initial,CDBVariant* value,short sqlType) {
    auto* binary=value->m_pbinary;
    ::GlobalUnlock(binary->m_hData);
    if(initial>=needed || sqlType!=SQL_LONGVARBINARY)return;
    HGLOBAL old=binary->m_hData;
    binary->m_hData=::GlobalReAlloc(old,SIZE_T(needed),GMEM_MOVEABLE);
    if(!binary->m_hData){binary->m_hData=old;impl__AfxThrowMemoryException__YAXXZ();}
    binary->m_dwDataLength=SQLULEN(needed);
    if(!position)impl__AfxThrowInvalidArgException__YAXXZ();
    *position=reinterpret_cast<void*>(UINT_PTR(::GlobalLock(binary->m_hData))+SQLULEN(initial));
    SQLLEN capacity=SQLLEN(SQLULEN(needed)-SQLULEN(initial)),read;RETCODE code;
    do {code=::SQLGetData(statement,SQLUSMALLINT(field),SQL_C_BINARY,*position,capacity,&read);}while(code==SQL_STILL_EXECUTING);
    if(code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO)
        impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(code,db,statement);
    ::GlobalUnlock(binary->m_hData);
}
}
// RVA 0x1fd570. The caller clears the old variant before selecting a buffer.
void* CRecordset::GetDataBuffer(CDBVariant& value,short cType,SQLLEN* length,short sqlType,SQLULEN precision) {
    if(!length){impl__AfxThrowInvalidArgException__YAXXZ();return nullptr;}
    switch(cType) {
    case SQL_C_BIT:value.m_dwType=DBVT_BOOL;value.m_boolVal=0;*length=4;return &value.m_boolVal;
    case SQL_C_UTINYINT:value.m_dwType=DBVT_UCHAR;*length=1;return &value.m_chVal;
    case SQL_C_SSHORT:value.m_dwType=DBVT_SHORT;*length=2;return &value.m_iVal;
    case SQL_C_SLONG:value.m_dwType=DBVT_LONG;*length=4;return &value.m_lVal;
    case SQL_C_FLOAT:value.m_dwType=DBVT_SINGLE;*length=4;return &value.m_fltVal;
    case SQL_C_DOUBLE:value.m_dwType=DBVT_DOUBLE;*length=8;return &value.m_dblVal;
    case SQL_C_TIMESTAMP:value.m_pdate=static_cast<TIMESTAMP_STRUCT*>(::operator new(sizeof(TIMESTAMP_STRUCT)));value.m_dwType=DBVT_DATE;*length=sizeof(TIMESTAMP_STRUCT);return value.m_pdate;
    case SQL_C_CHAR: {
        auto* string=::operator new(sizeof(void*));initNarrow(string);
        value.m_pstringA=static_cast<CStringA*>(string);value.m_dwType=DBVT_ASTRING;*length=GetTextLen(sqlType,precision);
        return DatabaseFieldBuffer<char>(string).get(*length);
    }
    case SQL_C_WCHAR:
        value.m_pstringW=new CString;value.m_dwType=DBVT_WSTRING;*length=GetTextLen(sqlType,precision);
        return DatabaseFieldBuffer<wchar_t>(value.m_pstringW).get(*length);
    case SQL_C_BINARY:
        value.m_pbinary=new CLongBinary;value.m_dwType=DBVT_BINARY;
        *length=sqlType==SQL_LONGVARBINARY || !precision?1:SQLLEN(precision);
        value.m_pbinary->m_hData=::GlobalAlloc(GMEM_MOVEABLE,SIZE_T(*length));
        if(!value.m_pbinary->m_hData)impl__AfxThrowMemoryException__YAXXZ();
        value.m_pbinary->m_dwDataLength=SQLULEN(*length);return ::GlobalLock(value.m_pbinary->m_hData);
    default:return nullptr;
    }
}
// Symbol: ?GetDataBuffer@CRecordset@@SAPEAXAEAVCDBVariant@@FPEA_JF_K@Z
extern "C" void* MS_ABI impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z(CDBVariant* value,short type,SQLLEN* length,short sqlType,SQLULEN precision){return CRecordset::GetDataBuffer(*value,type,length,sqlType,precision);}
// Symbol: ?GetLongCharDataAndCleanup@CRecordset@@SAXPEAVCDatabase@@PEAXF_JPEAPEAX2AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@FF@Z
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z(CDatabase* db,HSTMT statement,short field,SQLLEN needed,void** position,SQLLEN initial,CString* string,short sqlType,short cType){longText<wchar_t>(db,statement,field,needed,position,initial,string,sqlType,cType);}
// Symbol: ?GetLongCharDataAndCleanup@CRecordset@@SAXPEAVCDatabase@@PEAXF_JPEAPEAX2AEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@FF@Z
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z(CDatabase* db,HSTMT statement,short field,SQLLEN needed,void** position,SQLLEN initial,void* string,short sqlType,short cType){longText<char>(db,statement,field,needed,position,initial,string,sqlType,cType);}
// Symbol: ?GetLongBinaryDataAndCleanup@CRecordset@@SAXPEAVCDatabase@@PEAXF_JPEAPEAX2AEAVCDBVariant@@F@Z
extern "C" void MS_ABI impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z(CDatabase* db,HSTMT statement,short field,SQLLEN needed,void** position,SQLLEN initial,CDBVariant* value,short sqlType){longBinary(db,statement,field,needed,position,initial,value,sqlType);}
