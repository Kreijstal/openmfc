// Native binary RFX, mfc140u 14.51.36231 entry RVA 0x200dc0.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <new>
#include <cstdint>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(CRecordset*,UINT,void*);
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(CRecordset*,UINT,void*,SQLLEN*);
// Symbol: ?RFX_Binary@@YAXPEAVCFieldExchange@@PEB_WAEAVCByteArray@@_J@Z
extern "C" void MS_ABI impl__RFX_Binary__YAXPEAVCFieldExchange__PEB_WAEAVCByteArray___J_Z(
    CFieldExchange* fx,const wchar_t* name,CByteArray* value,SQLLEN maximum) {
    if(!fx || !name) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    UINT field;if(!fx->IsFieldType(&field))return;
    CRecordset* rs=fx->m_prs;DWORD index=field-1;
    SQLLEN* length=rs->GetFieldLengthBuffer(index,fx->m_nFieldType);
    bool object=false;
    auto pseudoNull=[&] {return value->m_nSize==1 && value->m_pData[0]==0xff;};
    auto setPseudoNull=[&] {
        value->SetSize(1,-1);
        if(value->m_nSize<=0)impl__AfxThrowInvalidArgException__YAXXZ();
        value->m_pData[0]=0xff;
    };
    switch(fx->m_nOperation) {
    case CFieldExchange::BindFieldToColumn: {
        SQLULEN size=rs->m_rgODBCFieldInfos[index].m_nPrecision;
        if(size>SQLULEN(maximum))size=SQLULEN(maximum);
        value->SetSize(SQLLEN(size),-1);
        if(value->m_nSize<=0) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
        // Retail truncates the allocation size to a signed DWORD for ODBC.
        auto code=::SQLBindCol(rs->m_hstmt,SQLUSMALLINT(field),SQL_C_BINARY,value->m_pData,SQLLEN(int32_t(size)),length);
        if(!rs->Check(code))rs->ThrowDBException(code,nullptr);
        rs->m_mapFieldIndex.SetAt(value,reinterpret_cast<void*>(UINT_PTR(field)));return;
    }
    case CFieldExchange::Fixup:
        if(*length==SQL_NULL_DATA) {rs->SetNullFieldStatus(index);setPseudoNull();}
        else {
            SQLLEN size=int32_t(*length);
            if(size>value->m_nMaxSize) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
            value->m_nSize=size;
        }
        return;
    case CFieldExchange::MarkForAddNew:
        if(!pseudoNull()) {rs->SetDirtyFieldStatus(index);rs->ClearNullFieldStatus(index);}return;
    case CFieldExchange::MarkForUpdate:
        object=true;if(!pseudoNull())rs->ClearNullFieldStatus(index);break;
    case CFieldExchange::SetFieldNull:
        if((!fx->m_pvField && fx->m_nFieldType==0) || fx->m_pvField==value) {
            if(fx->m_bField) {rs->SetNullFieldStatus(index);setPseudoNull();*length=SQL_NULL_DATA;}
            else {rs->ClearNullFieldStatus(index);*length=value->m_nSize;}
        }
        return;
    case CFieldExchange::StoreField:impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(rs,field,value);return;
    case CFieldExchange::LoadField:impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(rs,field,value,length);return;
    case CFieldExchange::AllocCache: {
        auto* cache=new(std::nothrow) CByteArray;
        if(!cache)impl__AfxThrowMemoryException__YAXXZ();
        rs->m_rgFieldInfos[index].m_pvDataCache=cache;rs->m_rgFieldInfos[index].m_nDataType=9;return;
    }
    default:break;
    }
    void* data=value->m_nSize>0 ? (object?static_cast<void*>(value):value->m_pData) : nullptr;
    fx->Default(name,data,length,SQL_C_BINARY,SQLULEN(int64_t(int32_t(value->m_nSize))),SQLULEN(uint32_t(value->m_nSize)));
}
