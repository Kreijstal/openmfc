// Verified release x64 entries: allocation 0x1faab0, resizing 0x1f8e00,
// metadata 0x1fa880, binding 0x1fccd0 and cache exchange 0x1fd110.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/DatabaseStringBuffer.h"
#include <limits>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
namespace {
void* Allocate(size_t bytes) {
    void* result=std::malloc(bytes);
    if(!result)impl__AfxThrowMemoryException__YAXXZ();
    return result;
}
CODBCFieldInfo* AllocateMetadata(SWORD count) {
    size_t elements=static_cast<size_t>(count),limit=(std::numeric_limits<size_t>::max()-sizeof(size_t))/sizeof(CODBCFieldInfo);
    size_t bytes=elements>limit?std::numeric_limits<size_t>::max():sizeof(size_t)+elements*sizeof(CODBCFieldInfo);
    auto* block=static_cast<size_t*>(Allocate(bytes));*block=elements;
    auto* fields=reinterpret_cast<CODBCFieldInfo*>(block+1);
    for(size_t i=0;i<elements;++i)new(fields+i) CODBCFieldInfo;
    return fields;
}
}
void CRecordset::AllocRowset() {
    if(m_dwOptions&useMultiRowFetch)SetRowsetSize(m_dwRowsetSize);
    else {m_rgRowStatus=static_cast<UWORD*>(Allocate(sizeof(UWORD)));m_dwRowsetSize=1;}
}
// Symbol: ?AllocRowset@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__AllocRowset_CRecordset__IEAAXXZ(CRecordset* rs) {rs->AllocRowset();}

UINT CRecordset::BindFieldsToColumns() {
    CFieldExchange exchange(CFieldExchange::BindFieldToColumn,this);exchange.m_hstmt=m_hstmt;
    if(m_dwOptions&useMultiRowFetch)DoBulkFieldExchange(&exchange);
    else DoFieldExchange(&exchange);
    return exchange.m_nFields;
}
// Symbol: ?BindFieldsToColumns@CRecordset@@QEAAIXZ
extern "C" UINT MS_ABI impl__BindFieldsToColumns_CRecordset__QEAAIXZ(CRecordset* rs) {return rs->BindFieldsToColumns();}
void CRecordset::AllocDataCache() {
    CFieldExchange exchange(CFieldExchange::AllocCache,this);DoFieldExchange(&exchange);
}
// Symbol: ?AllocDataCache@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__AllocDataCache_CRecordset__QEAAXXZ(CRecordset* rs) {rs->AllocDataCache();}
void CRecordset::InitRecord() {
    if(m_nFields) {
        m_nFieldsBound=static_cast<int32_t>(BindFieldsToColumns());
        if(m_nFields && m_bCheckCacheForDirtyFields)AllocDataCache();
    }else m_nFieldsBound=-1;
}
// Symbol: ?InitRecord@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__InitRecord_CRecordset__QEAAXXZ(CRecordset* rs) {rs->InitRecord();}

// Symbol: ?SetRowsetSize@CRecordset@@UEAAXK@Z
extern "C" void MS_ABI impl__SetRowsetSize_CRecordset__UEAAXK_Z(CRecordset* rs,DWORD size) {
    if(!rs || !size){impl__AfxThrowInvalidArgException__YAXXZ();return;}
    if(!rs->IsOpen()) {rs->m_dwRowsetSize=size;return;}
    if(!(rs->m_dwOptions&CRecordset::useMultiRowFetch))return;
    if(rs->m_dwAllocatedRowsetSize && rs->m_dwAllocatedRowsetSize>=size)rs->m_dwRowsetSize=size;
    else {
        rs->FreeRowset();rs->m_rgRowStatus=static_cast<UWORD*>(Allocate(size_t(size)*sizeof(UWORD)));
        if(!(rs->m_dwOptions&CRecordset::userAllocMultiRowBuffers)) {
            rs->m_dwRowsetSize=size;
            CFieldExchange exchange(CFieldExchange::AllocMultiRowBuffer,rs);rs->DoBulkFieldExchange(&exchange);
            rs->m_dwAllocatedRowsetSize=size;
            int bound=rs->m_nFieldsBound;rs->m_nFieldsBound=0;rs->InitRecord();rs->m_nFieldsBound=bound;
        }
    }
    ::SQLSetStmtOption(rs->m_hstmt,SQL_ROWSET_SIZE,rs->m_dwRowsetSize);
}
void CRecordset::SetRowsetSize(DWORD size) {impl__SetRowsetSize_CRecordset__UEAAXK_Z(this,size);}

void CRecordset::AllocAndCacheFieldInfo() {
    RETCODE code;
    do {code=::SQLNumResultCols(m_hstmt,&m_nResultCols);}while(code==SQL_STILL_EXECUTING);
    if(!Check(code))ThrowDBException(code,nullptr);
    if(!m_nResultCols)return;
    m_rgODBCFieldInfos=AllocateMetadata(m_nResultCols);
    for(UINT field=1;static_cast<int>(field)<=m_nResultCols;++field) {
        auto& info=m_rgODBCFieldInfos[field-1];
        openmfc::detail::DatabaseStringBuffer buffer(info.m_strName);
        auto* name=buffer.get(257);SQLSMALLINT length;
        do {code=::SQLDescribeColW(m_hstmt,static_cast<SQLUSMALLINT>(field),reinterpret_cast<SQLWCHAR*>(name),256,
            &length,&info.m_nSQLType,&info.m_nPrecision,&info.m_nScale,&info.m_nNullability);}while(code==SQL_STILL_EXECUTING);
        buffer.release(length);
        if(!Check(code))ThrowDBException(code,nullptr);
    }
}
// Symbol: ?AllocAndCacheFieldInfo@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__AllocAndCacheFieldInfo_CRecordset__IEAAXXZ(CRecordset* rs) {rs->AllocAndCacheFieldInfo();}
