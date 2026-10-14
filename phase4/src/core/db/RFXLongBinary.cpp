#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxdao.h"
#include <algorithm>
#include <cstdint>
#include <climits>
#include <cwchar>
#include <cstddef>
#include <string>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
SQLLEN AtExec(SQLULEN length) {
    constexpr SQLULEN maximum=INT_MAX-100;
    return SQL_LEN_DATA_AT_EXEC(SQLLEN(std::min(length,maximum)));
}
SQLLEN AddSize(SQLULEN current,SQLLEN extra) {
    SQLLEN total=SQLLEN(current+SQLULEN(extra));
    if(total<SQLLEN(current) || total<extra)impl__AfxThrowMemoryException__YAXXZ();
    return total;
}
// The fetcher only consumes BuildErrorString's state/native/origin text.
// Read the same legacy diagnostic stream, including all records and origins.
bool Truncated(CRecordset* recordset) {
    if(!recordset->m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return false;}
    std::wstring diagnostics;
    for(;;) {
        wchar_t state[64]{},message[512]{};SQLINTEGER native=0;SQLSMALLINT length=0;
        RETCODE code=::SQLErrorW(recordset->m_pDatabase->GetHENV(),recordset->m_pDatabase->m_hdbc,recordset->m_hstmt,
            reinterpret_cast<SQLWCHAR*>(state),&native,reinterpret_cast<SQLWCHAR*>(message),511,&length);
        if((code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO) || !std::wcscmp(state,L"00000"))break;
        diagnostics+=L"State:";diagnostics+=state;wchar_t number[50];std::swprintf(number,50,L",Native:%ld,Origin:",long(native));diagnostics+=number;
        const wchar_t* text=message;
        while(*text==L'[') {
            auto* end=std::wcschr(text,L']');if(!end)break;
            diagnostics.append(text,size_t(end-text+1));text=end+1;if(*text==L' ')++text;
        }
        diagnostics+=L"\n";
    }
    return diagnostics.find(L"State:01004")!=std::wstring::npos;
}
}
// Symbol: ?GetLongBinarySize@CFieldExchange@@QEAA_JH@Z
extern "C" SQLLEN MS_ABI impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(CFieldExchange* fx,int field) {
    SQLLEN length;BYTE dummy[8];RETCODE code;
    do {code=::SQLGetData(fx->m_prs->m_hstmt,SQLUSMALLINT(field),SQL_C_DEFAULT,dummy,0,&length);}while(code==SQL_STILL_EXECUTING);
    if(code!=SQL_SUCCESS && code!=SQL_SUCCESS_WITH_INFO)fx->m_prs->ThrowDBException(code,nullptr);
    return length;
}
// Symbol: ?ReallocLongBinary@CFieldExchange@@QEAAPEAEAEAVCLongBinary@@_J1@Z
extern "C" BYTE* MS_ABI impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(
    CFieldExchange*,CLongBinary* binary,SQLLEN required,SQLLEN reallocation) {
    required=std::max<SQLLEN>(0,required);HGLOBAL old=nullptr;
    if(!binary->m_hData)binary->m_hData=::GlobalAlloc(GMEM_MOVEABLE,SIZE_T(reallocation));
    else if(::GlobalSize(binary->m_hData)<SIZE_T(uint32_t(required))) {
        old=binary->m_hData;
        binary->m_hData=::GlobalReAlloc(old,SIZE_T(std::max(required,reallocation)),GMEM_MOVEABLE);
    }
    if(!binary->m_hData) {binary->m_hData=old;impl__AfxThrowMemoryException__YAXXZ();return nullptr;}
    auto* data=static_cast<BYTE*>(::GlobalLock(binary->m_hData));
    if(!data) {::GlobalFree(binary->m_hData);binary->m_hData=nullptr;impl__AfxThrowMemoryException__YAXXZ();}
    return data;
}
// Symbol: ?GetLongBinaryData@CFieldExchange@@QEAAXHAEAVCLongBinary@@PEA_J@Z
extern "C" void MS_ABI impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(
    CFieldExchange* fx,int field,CLongBinary* binary,SQLLEN* length) {
    if(!length) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    binary->m_dwDataLength=0;
    SQLLEN fetch=*length,reallocation=fetch;
    if(fetch==SQL_NO_TOTAL) {fetch=fx->m_lDefaultLBFetchSize;reallocation=fx->m_lDefaultLBReallocSize;}
    SQLLEN indicator=0;
    do {
        auto* data=fx->ReallocLongBinary(*binary,AddSize(binary->m_dwDataLength,fetch),AddSize(binary->m_dwDataLength,reallocation));
        data+=binary->m_dwDataLength;RETCODE code;
        do {code=::SQLGetData(fx->m_prs->m_hstmt,SQLUSMALLINT(field),SQL_C_BINARY,data,fetch,&indicator);}while(code==SQL_STILL_EXECUTING);
        ::GlobalUnlock(binary->m_hData);
        if(code==SQL_SUCCESS) {binary->m_dwDataLength+=SQLULEN(indicator);*length=SQLLEN(binary->m_dwDataLength);return;}
        if(code==SQL_NO_DATA) {fx->m_prs->SetNullFieldStatus(DWORD(field-1));*length=SQL_NULL_DATA;}
        else if(code==SQL_SUCCESS_WITH_INFO) {
            if(Truncated(fx->m_prs)) {
                binary->m_dwDataLength+=SQLULEN(fetch);
                fetch=fx->m_prs->GetLBFetchSize(fetch);reallocation=fx->m_prs->GetLBReallocSize(reallocation);indicator=SQL_NO_TOTAL;
            } else binary->m_dwDataLength+=SQLULEN(indicator);
            *length=SQLLEN(binary->m_dwDataLength);
        } else fx->m_prs->ThrowDBException(code,nullptr);
    }while(indicator==SQL_NO_TOTAL);
}
SQLLEN CFieldExchange::GetLongBinarySize(int field) {return impl__GetLongBinarySize_CFieldExchange__QEAA_JH_Z(this,field);}
BYTE* CFieldExchange::ReallocLongBinary(CLongBinary& binary,SQLLEN required,SQLLEN reallocation) {return impl__ReallocLongBinary_CFieldExchange__QEAAPEAEAEAVCLongBinary___J1_Z(this,&binary,required,reallocation);}
void CFieldExchange::GetLongBinaryData(int field,CLongBinary& binary,SQLLEN* length) {impl__GetLongBinaryData_CFieldExchange__QEAAXHAEAVCLongBinary__PEA_J_Z(this,field,&binary,length);}
// Symbol: ?RFX_LongBinary@@YAXPEAVCFieldExchange@@PEB_WAEAVCLongBinary@@@Z
extern "C" void MS_ABI impl__RFX_LongBinary__YAXPEAVCFieldExchange__PEB_WAEAVCLongBinary___Z(CFieldExchange* fx,const wchar_t* name,CLongBinary* value) {
    if(!fx || !name) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    UINT field;if(!fx->IsFieldType(&field))return;
    auto* rs=fx->m_prs;DWORD index=field-1;SQLLEN* length=rs->GetFieldLengthBuffer(index,fx->m_nFieldType);
    switch(fx->m_nOperation) {
    case CFieldExchange::BindFieldToColumn:
        if(!rs->m_bUseUpdateSQL && (rs->m_bUpdatable || rs->m_bAppendable)) {
            auto code=::SQLBindCol(rs->m_hstmt,SQLUSMALLINT(field),SQL_C_DEFAULT,value,0,length);
            if(!rs->Check(code))rs->ThrowDBException(code,nullptr);
        }
        rs->m_mapFieldIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(field)));return;
    case CFieldExchange::BindFieldForUpdate:
        *length=!rs->IsFieldStatusDirty(index)?SQL_IGNORE:rs->IsFieldStatusNull(index)?SQL_NULL_DATA:AtExec(value->m_dwDataLength);return;
    case CFieldExchange::UnbindFieldForUpdate:*length=SQLLEN(value->m_dwDataLength);return;
    case CFieldExchange::Fixup:
        *length=fx->GetLongBinarySize(field);
        if(*length!=SQL_NULL_DATA && *length)fx->GetLongBinaryData(field,*value,length);
        if(*length==SQL_NULL_DATA) {value->m_dwDataLength=0;rs->SetNullFieldStatus(index);}
        else {if(!*length)value->m_dwDataLength=0;rs->ClearNullFieldStatus(index);}return;
    case CFieldExchange::Name:
        rs->m_bLongBinaryColumns=TRUE;fx->Default(name,value,length,SQL_C_DEFAULT,0,0);return;
    case CFieldExchange::SetFieldNull:
        if((!fx->m_pvField && !fx->m_nFieldType) || fx->m_pvField==value) {
            if(fx->m_bField) {rs->SetNullFieldStatus(index);value->m_dwDataLength=0;*length=SQL_NULL_DATA;}
            else {rs->ClearNullFieldStatus(index);*length=AtExec(value->m_dwDataLength);}
        }
        return;
    case CFieldExchange::NameValue:
        if(rs->IsFieldStatusDirty(index)) {*fx->m_pstr+=name;*fx->m_pstr+=L"=";}
        [[fallthrough]];
    case CFieldExchange::Value: {
        if(!rs->IsFieldStatusDirty(index))return;
        *length=rs->IsFieldStatusNull(index)?SQL_NULL_DATA:AtExec(value->m_dwDataLength);
        if(rs->m_dwOptions&CRecordset::optimizeBulkAdd)return;
        *fx->m_pstr+=L"?";*fx->m_pstr+=fx->m_lpszSeparator;++fx->m_nParamFields;
        auto code=::SQLBindParameter(fx->m_hstmt,SQLUSMALLINT(fx->m_nParamFields),SQL_PARAM_INPUT,SQL_C_DEFAULT,
            rs->m_rgODBCFieldInfos[index].m_nSQLType,value->m_dwDataLength,0,value,0,length);
        if(code!=SQL_SUCCESS)rs->ThrowDBException(code,fx->m_hstmt);return;
    }
    default:return;
    }
}
static_assert(sizeof(CLongBinary)==24 && offsetof(CLongBinary,m_dwDataLength)==16 && sizeof(CLongBinary::m_dwDataLength)==8,"native long binary layout");
