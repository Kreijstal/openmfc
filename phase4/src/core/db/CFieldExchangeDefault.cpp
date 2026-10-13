// Native default RFX operation handling, including cache comparison and SQL.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" BOOL MS_ABI impl__AfxCompareValueByRef__YAHPEAX0H_Z(void*,void*,int);
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(CRecordset*,UINT,void*);
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(CRecordset*,UINT,void*,SQLLEN*);
void CFieldExchange::Default(const wchar_t* name,void* value,SQLLEN* length,int type,SQLULEN bytes,SQLULEN precision) {
    const UINT field=m_nFieldType ? m_nParams : m_nFields;
    const DWORD index=field-1;CRecordset* rs=m_prs;
    auto requireLength=[&] {if(!length)impl__AfxThrowInvalidArgException__YAXXZ();};
    switch(m_nOperation) {
    case BindParam: {
        requireLength();*length=rs->IsParamStatusNull(index)?SQL_NULL_DATA:SQLLEN(bytes);
        auto code=::SQLBindParameter(m_hstmt,SQLUSMALLINT(field),SQLSMALLINT(m_nFieldType),SQLSMALLINT(type),
            SQLSMALLINT(type),precision,0,value,0,length);
        if(code!=SQL_SUCCESS)rs->ThrowDBException(code,m_hstmt);
        rs->m_mapParamIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(field)));return;
    }
    case RebindParam:requireLength();*length=rs->IsParamStatusNull(index)?SQL_NULL_DATA:SQLLEN(bytes);return;
    case BindFieldToColumn: {
        auto code=::SQLBindCol(rs->m_hstmt,SQLUSMALLINT(field),SQLSMALLINT(type),value,SQLLEN(bytes),length);
        if(!rs->Check(code))rs->ThrowDBException(code,nullptr);
        rs->m_mapFieldIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(field)));return;
    }
    case BindFieldForUpdate:
        requireLength();
        *length=!rs->IsFieldStatusDirty(index)?SQL_IGNORE:rs->IsFieldStatusNull(index)?SQL_NULL_DATA:SQLLEN(bytes);return;
    case UnbindFieldForUpdate:
        if(!rs->IsFieldStatusDirty(index)) {requireLength();*length=SQLLEN(bytes);}return;
    case MarkForUpdate: {
        rs->GetFieldStatus(index); // native range check precedes cache access
        auto& info=rs->m_rgFieldInfos[index];
        if(info.m_bStatus&2) {
            if(!rs->IsFieldStatusNull(index))rs->SetDirtyFieldStatus(index);
        } else {
            void* cache=((info.m_nDataType>=1 && info.m_nDataType<=4) || info.m_nDataType==6)?&info:info.m_pvDataCache;
            if(rs->IsFieldStatusNull(index) || !impl__AfxCompareValueByRef__YAHPEAX0H_Z(value,cache,info.m_nDataType))rs->SetDirtyFieldStatus(index);
        }
        if((!m_pvField || m_pvField==value) && rs->IsFieldStatusDirty(index))m_bField=TRUE;
        return;
    }
    case StoreField:impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(rs,field,value);return;
    case LoadField:impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(rs,field,value,length);return;
    case Name:
        if(rs->IsFieldStatusDirty(index)) {*m_pstr+=name;*m_pstr+=m_lpszSeparator;}return;
    case NameValue:
        if(rs->IsFieldStatusDirty(index)) {*m_pstr+=name;*m_pstr+=L"=";}
        [[fallthrough]];
    case Value: {
        if(!rs->IsFieldStatusDirty(index))return;
        requireLength();*length=rs->IsFieldStatusNull(index)?SQL_NULL_DATA:SQLLEN(bytes);
        if(rs->m_dwOptions&CRecordset::optimizeBulkAdd)return;
        *m_pstr+=L"?";*m_pstr+=m_lpszSeparator;++m_nParamFields;
        const auto& info=rs->m_rgODBCFieldInfos[index];
        auto code=::SQLBindParameter(m_hstmt,SQLUSMALLINT(m_nParamFields),SQL_PARAM_INPUT,SQLSMALLINT(type),
            info.m_nSQLType,info.m_nPrecision,info.m_nScale,value,0,length);
        if(code!=SQL_SUCCESS)rs->ThrowDBException(code,m_hstmt);return;
    }
    default:return;
    }
}
// Symbol: ?Default@CFieldExchange@@QEAAXPEB_WPEAXPEA_JH_K3@Z
extern "C" void MS_ABI impl__Default_CFieldExchange__QEAAXPEB_WPEAXPEA_JH_K3_Z(CFieldExchange* self,
    const wchar_t* name,void* value,SQLLEN* length,int type,SQLULEN bytes,SQLULEN precision) {
    self->Default(name,value,length,type,bytes,precision);
}
