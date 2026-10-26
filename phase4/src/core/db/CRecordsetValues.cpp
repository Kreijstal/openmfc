// Native release x64 field-value allocation and long-value continuation.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "openmfc/afxdao.h"
#include "detail/DatabaseFieldBuffer.h"
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);
using openmfc::detail::DatabaseFieldBuffer;
namespace {
int compareName(const CString& name,const wchar_t* target) noexcept {
    if(!target)impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
    return std::wcscmp(name.GetString(),target);
}
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
template<class Char> void fieldText(CRecordset* rs,short index,void* string) {
    if(rs->m_nResultCols<=0)return;
    short column=short(uint16_t(index)+1);
    if(column<1 || column>rs->m_nResultCols)rs->ThrowDBException(0x402,nullptr);
    const auto& info=rs->m_rgODBCFieldInfos[index];
    SQLLEN capacity=CRecordset::GetTextLen(info.m_nSQLType,info.m_nPrecision);
    DatabaseFieldBuffer<Char> buffer(string);void* data=buffer.get(capacity);
    short type=sizeof(Char)==2?SQL_C_WCHAR:SQL_C_CHAR;
    SQLLEN bytes=CRecordset::GetData(rs->m_pDatabase,rs->m_hstmt,column,type,data,SQLLEN(SQLULEN(capacity)*sizeof(Char)),info.m_nSQLType);
    if(bytes==SQL_NULL_DATA){buffer.empty();return;}
    SQLLEN needed=sizeof(Char)==2?SQLLEN(SQLULEN(bytes)>>1):bytes;
    longText<Char>(rs->m_pDatabase,rs->m_hstmt,column,needed,&data,capacity,string,info.m_nSQLType,type);
}
template<class Char> void namedText(CRecordset* rs,const wchar_t* name,void* string) {
    if(sizeof(Char)==2 && (!rs || !rs->IsOpen() || !name)){impl__AfxThrowInvalidArgException__YAXXZ();return;}
    if(rs->m_nResultCols>0)fieldText<Char>(rs,rs->GetFieldIndexByName(name),string);
}
}
// RVA 0x1fc8c0. The authoritative metadata array supplies case-sensitive
// names; no driver calls or synthetic sidecar field names are involved.
short CRecordset::GetFieldIndexByName(const wchar_t* name) {
    short index=0;
    for(;index<m_nResultCols;++index)if(!compareName(m_rgODBCFieldInfos[index].m_strName,name))break;
    if(index==m_nResultCols)ThrowDBException(0x402,nullptr);
    return index;
}
// RVAs 0x1f9250/0x1f9290. Copy the fields individually, preserving ABI padding
// and using CString's owning-manager assignment for the destination name.
void CRecordset::GetODBCFieldInfo(short index,CODBCFieldInfo& output) {
    if(m_nResultCols<=0)return;
    const auto& input=m_rgODBCFieldInfos[index];
    output.m_strName=input.m_strName;output.m_nSQLType=input.m_nSQLType;
    output.m_nPrecision=input.m_nPrecision;output.m_nScale=input.m_nScale;output.m_nNullability=input.m_nNullability;
}
void CRecordset::GetODBCFieldInfo(const wchar_t* name,CODBCFieldInfo& output) {
    if(m_nResultCols>0)GetODBCFieldInfo(GetFieldIndexByName(name),output);
}
// RVAs 0x1f92f0/0x1f9350. Clear owned variant data before validating a result
// column; the continuation helper owns any successful binary buffer unlock.
void CRecordset::GetFieldValue(short index,CDBVariant& value,short type) {
    value.Clear();if(m_nResultCols<=0)return;
    short column=short(uint16_t(index)+1);
    if(column<1 || column>m_nResultCols)ThrowDBException(0x402,nullptr);
    const auto& info=m_rgODBCFieldInfos[index];
    if(!type)type=GetDefaultFieldType(info.m_nSQLType);
    SQLLEN capacity=0;void* data=GetDataBuffer(value,type,&capacity,info.m_nSQLType,info.m_nPrecision);
    SQLLEN bytes=GetData(m_pDatabase,m_hstmt,column,type,data,value.m_dwType==DBVT_WSTRING?SQLLEN(SQLULEN(capacity)*2):capacity,info.m_nSQLType);
    if(bytes==SQL_NULL_DATA){value.Clear();return;}
    if(type==SQL_C_CHAR)longText<char>(m_pDatabase,m_hstmt,column,bytes,&data,capacity,value.m_pstringA,info.m_nSQLType,type);
    else if(type==SQL_C_WCHAR)longText<wchar_t>(m_pDatabase,m_hstmt,column,SQLLEN(SQLULEN(bytes)>>1),&data,capacity,value.m_pstringW,info.m_nSQLType,type);
    else if(type==SQL_C_BINARY)longBinary(m_pDatabase,m_hstmt,column,bytes,&data,capacity,&value,info.m_nSQLType);
}
void CRecordset::GetFieldValue(const wchar_t* name,CDBVariant& value,short type) {
    if(m_nResultCols<=0){value.Clear();return;}
    GetFieldValue(GetFieldIndexByName(name),value,type);
}
void CRecordset::GetFieldValue(short index,CString& value){fieldText<wchar_t>(this,index,&value);}
void CRecordset::GetFieldValue(const wchar_t* name,CString& value){namedText<wchar_t>(this,name,&value);}
void CRecordset::GetFieldValue(short index,CStringA& value){fieldText<char>(this,index,&value);}
void CRecordset::GetFieldValue(const wchar_t* name,CStringA& value){namedText<char>(this,name,&value);}

// Symbol: ?GetFieldIndexByName@CRecordset@@QEAAFPEB_W@Z
extern "C" short MS_ABI impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z(CRecordset* rs,const wchar_t* name){return rs->GetFieldIndexByName(name);}
// Symbol: ?GetODBCFieldInfo@CRecordset@@QEAAXFAEAUCODBCFieldInfo@@@Z
extern "C" void MS_ABI impl__GetODBCFieldInfo_CRecordset__QEAAXFAEAUCODBCFieldInfo___Z(CRecordset* rs,short index,CODBCFieldInfo* output){rs->GetODBCFieldInfo(index,*output);}
// Symbol: ?GetODBCFieldInfo@CRecordset@@QEAAXPEB_WAEAUCODBCFieldInfo@@@Z
extern "C" void MS_ABI impl__GetODBCFieldInfo_CRecordset__QEAAXPEB_WAEAUCODBCFieldInfo___Z(CRecordset* rs,const wchar_t* name,CODBCFieldInfo* output){rs->GetODBCFieldInfo(name,*output);}
// Symbol: ?GetFieldValue@CRecordset@@QEAAXFAEAVCDBVariant@@F@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAVCDBVariant__F_Z(CRecordset* rs,short index,CDBVariant* value,short type){rs->GetFieldValue(index,*value,type);}
// Symbol: ?GetFieldValue@CRecordset@@QEAAXPEB_WAEAVCDBVariant@@F@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAVCDBVariant__F_Z(CRecordset* rs,const wchar_t* name,CDBVariant* value,short type){rs->GetFieldValue(name,*value,type);}
// Symbol: ?GetFieldValue@CRecordset@@QEAAXFAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CRecordset* rs,short index,CString* value){fieldText<wchar_t>(rs,index,value);}
// Symbol: ?GetFieldValue@CRecordset@@QEAAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CRecordset* rs,const wchar_t* name,CString* value){namedText<wchar_t>(rs,name,value);}
// Symbol: ?GetFieldValue@CRecordset@@QEAAXFAEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(CRecordset* rs,short index,void* value){fieldText<char>(rs,index,value);}
// Symbol: ?GetFieldValue@CRecordset@@QEAAXPEB_WAEAV?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(CRecordset* rs,const wchar_t* name,void* value){namedText<char>(rs,name,value);}
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
