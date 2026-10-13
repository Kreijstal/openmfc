// Native release x64 CFieldExchange layout and enumeration contract.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstddef>
#include <type_traits>
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
static_assert(sizeof(CFieldExchange)==96 && !std::is_polymorphic<CFieldExchange>::value,"native field exchange has no vtable");
static_assert(offsetof(CFieldExchange,m_nOperation)==0 && offsetof(CFieldExchange,m_prs)==8 &&
    offsetof(CFieldExchange,m_nFieldType)==16 && offsetof(CFieldExchange,m_pstr)==24 &&
    offsetof(CFieldExchange,m_pvField)==40 && offsetof(CFieldExchange,m_nFields)==56 &&
    offsetof(CFieldExchange,m_nParams)==60 && offsetof(CFieldExchange,m_hstmt)==72 &&
    offsetof(CFieldExchange,m_lDefaultLBFetchSize)==80 && offsetof(CFieldExchange,m_lDefaultLBReallocSize)==88,"native field exchange offsets");
static_assert(offsetof(CRecordset,m_hstmt)==8 && offsetof(CRecordset,m_pDatabase)==16 &&
    offsetof(CRecordset,m_strFilter)==24 && offsetof(CRecordset,m_strSort)==32,"native recordset prefix used by the constructor");
CFieldExchange::CFieldExchange(UINT operation,CRecordset* recordset,void* field) {
    // mfc140u 14.51.36231 RVA 0x1fe690: validate before writing this object.
    if(!recordset || !recordset->m_hstmt) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    m_nOperation=operation;m_prs=recordset;m_nFieldType=noFieldType;
    m_pstr=nullptr;m_bField=FALSE;m_pvField=field;
    m_nFields=0;m_nParams=0;m_nParamFields=0;m_hstmt=nullptr;
    m_lDefaultLBFetchSize=0x10000;m_lDefaultLBReallocSize=0x10000;
    // Native leaves m_nFieldFound, m_lpszSeparator and padding untouched.
}
BOOL CFieldExchange::IsFieldType(UINT* field) {
    // mfc140u RVA 0x1fe6e0. Counts advance for every enumerated field,
    // including fields that the current operation does not process.
    if(!field) {impl__AfxThrowInvalidArgException__YAXXZ();return FALSE;}
    *field=m_nFieldType==outputColumn ? ++m_nFields : ++m_nParams;
    if(m_nOperation<BindFieldToColumn)return m_nFieldType!=outputColumn;
    if(m_nOperation==Fixup)return (m_nFieldType & ~UINT(6))==0 && m_nFieldType!=6;
    return m_nFieldType==outputColumn;
}
// Symbol: ??0CFieldExchange@@QEAA@IPEAVCRecordset@@PEAX@Z
extern "C" void* MS_ABI impl___0CFieldExchange__QEAA_IPEAVCRecordset__PEAX_Z(void* self,UINT operation,CRecordset* recordset,void* field) {
    return new(self) CFieldExchange(operation,recordset,field);
}
// Symbol: ?IsFieldType@CFieldExchange@@QEAAHPEAI@Z
extern "C" BOOL MS_ABI impl__IsFieldType_CFieldExchange__QEAAHPEAI_Z(CFieldExchange* self,UINT* field) {
    return self->IsFieldType(field);
}
