// Release x64 Close 0x1f84d0, FreeRowset 0x1fab00, destructor 0x1f8170.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/CRecordsetProxies.h"
#include "detail/CPtrListSupport.h"
#include "detail/DatabaseStringBuffer.h"
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
struct RegistrationLock {
    RegistrationLock() {impl__AfxLockGlobals__YAXH_Z(15);}
    ~RegistrationLock() {impl__AfxUnlockGlobals__YAXH_Z(15);}
};
void DeleteFieldMetadata(CODBCFieldInfo* fields) {
    if(!fields)return;
    // Native new[] keeps the element count in an eight-byte cookie. Release
    // each ATL string through its own manager, including locked strings.
    auto* allocation=reinterpret_cast<size_t*>(fields)-1;
    for(size_t i=*allocation;i;--i) {
        auto* data=reinterpret_cast<CStringData*>(*reinterpret_cast<wchar_t**>(&fields[i-1].m_strName))-1;
        if(data->Release())data->pStringMgr->Free(data);
    }
    ::operator delete[](allocation);
}
}
void CRecordset::FreeRowset() {
    std::free(m_rgRowStatus);m_rgRowStatus=nullptr;
    if((m_dwOptions&(useMultiRowFetch|userAllocMultiRowBuffers))==useMultiRowFetch && m_dwAllocatedRowsetSize) {
        CFieldExchange exchange(CFieldExchange::DeleteMultiRowBuffer,this);
        DoBulkFieldExchange(&exchange);
    }
    m_dwAllocatedRowsetSize=0;
}
// Symbol: ?FreeRowset@CRecordset@@IEAAXXZ
extern "C" void MS_ABI impl__FreeRowset_CRecordset__IEAAXXZ(CRecordset* recordset) {recordset->FreeRowset();}

// Symbol: ?Close@CRecordset@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CRecordset__UEAAXXZ(CRecordset* recordset) {
    if(!recordset || !recordset->m_pDatabase) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    auto& self=*recordset;
    openmfc::detail::DatabaseStringBuffer(self.m_strCursorName).empty();
    if(self.m_rgFieldInfos && self.m_nFields && self.m_bCheckCacheForDirtyFields)self.FreeDataCache();
    self.FreeRowset();
    self.m_nEditMode=0;
    std::free(self.m_rgFieldInfos);self.m_rgFieldInfos=nullptr;
    DeleteFieldMetadata(self.m_rgODBCFieldInfos);self.m_rgODBCFieldInfos=nullptr;
    std::free(self.m_pbFieldFlags);self.m_pbFieldFlags=nullptr;
    std::free(self.m_pbParamFlags);self.m_pbParamFlags=nullptr;
    openmfc::detail::FreeRecordsetProxies(self);
    std::free(self.m_plParamLength);self.m_plParamLength=nullptr;
    if(self.m_hstmt) {::SQLFreeStmt(self.m_hstmt,SQL_DROP);self.m_hstmt=nullptr;}
    if(self.m_hstmtUpdate) {::SQLFreeStmt(self.m_hstmtUpdate,SQL_DROP);self.m_hstmtUpdate=nullptr;}
    {
        RegistrationLock lock;
        auto* list=reinterpret_cast<OpenMFC_CPtrList*>(self.m_pDatabase->m_listRecordsets);
        if(auto* position=list->Find(recordset,nullptr))list->RemoveAt(position);
    }
    self.m_lOpen=0;self.m_bBOF=TRUE;self.m_bEOF=TRUE;
    self.m_bUpdatable=FALSE;self.m_bAppendable=FALSE;self.m_bScrollable=FALSE;self.m_bDeleted=FALSE;
    self.m_bRebindParams=FALSE;self.m_bLongBinaryColumns=FALSE;self.m_nLockMode=0;self.m_nFieldsBound=0;self.m_nResultCols=-1;
    std::lock_guard<std::mutex> lock(g_recordsetStateMutex);
    g_recordsetSql.erase(recordset);g_recordsetStates.erase(recordset);
}
void CRecordset::Close() {impl__Close_CRecordset__UEAAXXZ(this);}
CRecordset::~CRecordset() {
    try {
        if(m_hstmt)CRecordset::Close();
        if(m_bRecordsetDb && m_pDatabase) {
            // Call the owner's deleting destructor through its native slot so
            // foreign SDK subclasses retain their ownership and exception path.
            auto** table=*reinterpret_cast<void***>(m_pDatabase);
            reinterpret_cast<void* (MS_ABI*)(void*,UINT)>(table[1])(m_pDatabase,1);
        }
        m_pDatabase=nullptr;
    }catch(CException* exception) {exception->Delete();}
    std::lock_guard<std::mutex> lock(g_recordsetStateMutex);
    g_recordsetSql.erase(this);g_recordsetStates.erase(this);
}
// Symbol: ??1CRecordset@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecordset__UEAA_XZ(CRecordset* recordset) {recordset->CRecordset::~CRecordset();}
