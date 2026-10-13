// Native release recordset flags, length buffers, and field/parameter indexes.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstdlib>
#include <cstring>
#include <cstddef>
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();
namespace {
template<class T> T* Allocate(UINT count) {
    auto* memory=static_cast<T*>(std::malloc(size_t(count)*sizeof(T)));
    if(!memory)impl__AfxThrowMemoryException__YAXXZ();
    std::memset(memory,0,size_t(count)*sizeof(T));return memory;
}
void CheckIndex(DWORD index,UINT count) {if(index>=count)impl__AfxThrowUserException__YAXXZ();}
}
void CRecordset::AllocStatusArrays() {
    if(m_nFields) {
        if(!m_rgFieldInfos)m_rgFieldInfos=Allocate<CFieldInfo>(m_nFields);
        if(!m_pbFieldFlags)m_pbFieldFlags=Allocate<BYTE>(m_nFields);
    }
    if(m_nParams) {
        if(!m_pbParamFlags)m_pbParamFlags=Allocate<BYTE>(m_nParams);
        if(!m_plParamLength)m_plParamLength=Allocate<SQLLEN>(m_nParams);
    }
}
SQLLEN* CRecordset::GetFieldLengthBuffer(DWORD index,int type) {
    CheckIndex(index,type?m_nParams:m_nFields);
    return type ? m_plParamLength+index : &m_rgFieldInfos[index].m_nLength;
}
BYTE CRecordset::GetFieldStatus(DWORD index) {CheckIndex(index,m_nFields);return m_pbFieldFlags[index];}
void CRecordset::SetFieldStatus(DWORD index,BYTE bits) {CheckIndex(index,m_nFields);m_pbFieldFlags[index]|=bits;}
void CRecordset::ClearFieldStatus() {std::memset(m_pbFieldFlags,0,m_nFields);}
void CRecordset::SetDirtyFieldStatus(DWORD index) {SetFieldStatus(index,1);}
void CRecordset::ClearDirtyFieldStatus(DWORD index) {CheckIndex(index,m_nFields);m_pbFieldFlags[index]&=BYTE(~1);}
BOOL CRecordset::IsFieldStatusDirty(DWORD index) const {CheckIndex(index,m_nFields);return m_pbFieldFlags[index]&1;}
BOOL CRecordset::IsFieldStatusNull(DWORD index) const {CheckIndex(index,m_nFields);return m_pbFieldFlags[index]&2;}
void CRecordset::SetNullFieldStatus(DWORD index) {SetFieldStatus(index,2);}
void CRecordset::ClearNullFieldStatus(DWORD index) {CheckIndex(index,m_nFields);m_pbFieldFlags[index]&=BYTE(~2);}
BOOL CRecordset::IsParamStatusNull(DWORD index) const {CheckIndex(index,m_nParams);return m_pbParamFlags[index]&2;}
void CRecordset::SetNullParamStatus(DWORD index) {CheckIndex(index,m_nParams);m_pbParamFlags[index]|=2;}
void CRecordset::ClearNullParamStatus(DWORD index) {CheckIndex(index,m_nParams);m_pbParamFlags[index]&=BYTE(~2);}
int CRecordset::GetBoundFieldIndex(void* field) {
    void* index;return m_mapFieldIndex.Lookup(field,index)?int(reinterpret_cast<UINT_PTR>(index)):-1;
}
int CRecordset::GetBoundParamIndex(void* param) {
    void* index;return m_mapParamIndex.Lookup(param,index)?int(reinterpret_cast<UINT_PTR>(index)):-1;
}
void CRecordset::SetParamNull(int index,BOOL null) {
    if(!IsOpen())AllocStatusArrays();
    if(null)SetNullParamStatus(index);else ClearNullParamStatus(index);
}
void CRecordset::SetFieldDirty(void* field,BOOL dirty) {
    int first=0,last=int(m_nFields)-1;
    if(field) {first=last=GetBoundFieldIndex(field)-1;if(first<0)return;}
    for(int i=first;i<=last;++i) {if(dirty)SetDirtyFieldStatus(i);else ClearDirtyFieldStatus(i);}
}
void CRecordset::FreeDataCache() {
    // Native release type IDs from afxdb_.h; unknown types remain untouched.
    for(UINT i=0;i<m_nFields;++i) {
        auto& field=m_rgFieldInfos[i];bool released=true;
        switch(field.m_nDataType) {
        case 1:case 2:case 3:case 4:case 6:break; // cache value is inline, not an allocation
        case 7:case 8:case 15:case 16:case 18:case 19:case 22:
            std::free(field.m_pvDataCache);break;
        case 9:
            if(field.m_pvDataCache) {
                using Destroy=void* (MS_ABI*)(void*,UINT);
                auto* slots=*static_cast<void***>(field.m_pvDataCache);
                reinterpret_cast<Destroy>(slots[1])(field.m_pvDataCache,1);
            }
            break;
        case 20:case 21:
            if(field.m_pvDataCache) {
                auto* data=reinterpret_cast<CStringData*>(*static_cast<char**>(field.m_pvDataCache))-1;
                if(::InterlockedDecrement(&data->nRefs)<=0)data->pStringMgr->Free(data);
                std::free(field.m_pvDataCache);
            }
            break;
        default:released=false;break;
        }
        if(released)field.m_pvDataCache=nullptr;
    }
}
void CRecordset::MarkForUpdate() {
    CFieldExchange exchange(CFieldExchange::MarkForUpdate,this);
    DoFieldExchange(&exchange);
}
void CRecordset::MarkForAddNew() {
    CFieldExchange exchange(CFieldExchange::MarkForAddNew,this);
    DoFieldExchange(&exchange);
}
// Symbol: ?MarkForUpdate@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__MarkForUpdate_CRecordset__QEAAXXZ(CRecordset* self) {self->MarkForUpdate();}
// Symbol: ?MarkForAddNew@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__MarkForAddNew_CRecordset__QEAAXXZ(CRecordset* self) {self->MarkForAddNew();}
BOOL CRecordset::Check(RETCODE code) const {
    return code==SQL_SUCCESS || code==SQL_SUCCESS_WITH_INFO || code==SQL_NEED_DATA || code==SQL_NO_DATA;
}
void CRecordset::DoFieldExchange(CFieldExchange*) {}
void CRecordset::DoBulkFieldExchange(CFieldExchange*) {}
SQLLEN CRecordset::GetLBFetchSize(SQLLEN size) {return static_cast<SQLLEN>(static_cast<SQLULEN>(size)*2);}
SQLLEN CRecordset::GetLBReallocSize(SQLLEN size) {return static_cast<SQLLEN>(static_cast<SQLULEN>(size)*2);}
// Symbol: ?Check@CRecordset@@UEBAHF@Z
extern "C" BOOL MS_ABI impl__Check_CRecordset__UEBAHF_Z(const CRecordset* self,RETCODE code) {return self->CRecordset::Check(code);}
// Symbol: ?DoFieldExchange@CRecordset@@UEAAXPEAVCFieldExchange@@@Z
extern "C" void MS_ABI impl__DoFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z(CRecordset* self,CFieldExchange* fx) {self->CRecordset::DoFieldExchange(fx);}
// Symbol: ?DoBulkFieldExchange@CRecordset@@UEAAXPEAVCFieldExchange@@@Z
extern "C" void MS_ABI impl__DoBulkFieldExchange_CRecordset__UEAAXPEAVCFieldExchange___Z(CRecordset* self,CFieldExchange* fx) {self->CRecordset::DoBulkFieldExchange(fx);}
// Symbol: ?GetLBFetchSize@CRecordset@@MEAA_J_J@Z
extern "C" SQLLEN MS_ABI impl__GetLBFetchSize_CRecordset__MEAA_J_J_Z(CRecordset* self,SQLLEN size) {return self->CRecordset::GetLBFetchSize(size);}
// Symbol: ?GetLBReallocSize@CRecordset@@MEAA_J_J@Z
extern "C" SQLLEN MS_ABI impl__GetLBReallocSize_CRecordset__MEAA_J_J_Z(CRecordset* self,SQLLEN size) {return self->CRecordset::GetLBReallocSize(size);}
// Symbol: ?AllocStatusArrays@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__AllocStatusArrays_CRecordset__QEAAXXZ(CRecordset* self) {self->AllocStatusArrays();}
// Symbol: ?GetFieldLengthBuffer@CRecordset@@QEAAPEA_JKH@Z
extern "C" SQLLEN* MS_ABI impl__GetFieldLengthBuffer_CRecordset__QEAAPEA_JKH_Z(CRecordset* self,DWORD field,int type) {return self->GetFieldLengthBuffer(field,type);}
// Symbol: ?GetFieldStatus@CRecordset@@QEAAEK@Z
extern "C" BYTE MS_ABI impl__GetFieldStatus_CRecordset__QEAAEK_Z(CRecordset* self,DWORD field) {return self->GetFieldStatus(field);}
// Symbol: ?SetFieldStatus@CRecordset@@QEAAXKE@Z
extern "C" void MS_ABI impl__SetFieldStatus_CRecordset__QEAAXKE_Z(CRecordset* self,DWORD field,BYTE bits) {self->SetFieldStatus(field,bits);}
// Symbol: ?ClearFieldStatus@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__ClearFieldStatus_CRecordset__QEAAXXZ(CRecordset* self) {self->ClearFieldStatus();}
// Symbol: ?SetDirtyFieldStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__SetDirtyFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field) {self->SetDirtyFieldStatus(field);}
// Symbol: ?ClearDirtyFieldStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__ClearDirtyFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field) {self->ClearDirtyFieldStatus(field);}
// Symbol: ?IsFieldStatusDirty@CRecordset@@QEBAHK@Z
extern "C" BOOL MS_ABI impl__IsFieldStatusDirty_CRecordset__QEBAHK_Z(const CRecordset* self,DWORD field) {return self->IsFieldStatusDirty(field);}
// Symbol: ?IsFieldStatusNull@CRecordset@@QEBAHK@Z
extern "C" BOOL MS_ABI impl__IsFieldStatusNull_CRecordset__QEBAHK_Z(const CRecordset* self,DWORD field) {return self->IsFieldStatusNull(field);}
// Symbol: ?SetNullFieldStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__SetNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field) {self->SetNullFieldStatus(field);}
// Symbol: ?ClearNullFieldStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__ClearNullFieldStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD field) {self->ClearNullFieldStatus(field);}
// Symbol: ?IsParamStatusNull@CRecordset@@QEBAHK@Z
extern "C" BOOL MS_ABI impl__IsParamStatusNull_CRecordset__QEBAHK_Z(const CRecordset* self,DWORD param) {return self->IsParamStatusNull(param);}
// Symbol: ?SetNullParamStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__SetNullParamStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD param) {self->SetNullParamStatus(param);}
// Symbol: ?ClearNullParamStatus@CRecordset@@QEAAXK@Z
extern "C" void MS_ABI impl__ClearNullParamStatus_CRecordset__QEAAXK_Z(CRecordset* self,DWORD param) {self->ClearNullParamStatus(param);}
// Symbol: ?GetBoundFieldIndex@CRecordset@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__GetBoundFieldIndex_CRecordset__QEAAHPEAX_Z(CRecordset* self,void* field) {return self->GetBoundFieldIndex(field);}
// Symbol: ?GetBoundParamIndex@CRecordset@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__GetBoundParamIndex_CRecordset__QEAAHPEAX_Z(CRecordset* self,void* param) {return self->GetBoundParamIndex(param);}
// Symbol: ?FreeDataCache@CRecordset@@QEAAXXZ
extern "C" void MS_ABI impl__FreeDataCache_CRecordset__QEAAXXZ(CRecordset* self) {self->FreeDataCache();}
// Symbol: ?SetParamNull@CRecordset@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetParamNull_CRecordset__QEAAXHH_Z(CRecordset* self,int index,BOOL null) {self->SetParamNull(index,null);}
// Symbol: ?SetFieldDirty@CRecordset@@QEAAXPEAXH@Z
extern "C" void MS_ABI impl__SetFieldDirty_CRecordset__QEAAXPEAXH_Z(CRecordset* self,void* field,BOOL dirty) {self->SetFieldDirty(field,dirty);}
static_assert(sizeof(CRecordset)==440 && offsetof(CRecordset,m_nFields)==40 && offsetof(CRecordset,m_nParams)==44 &&
    offsetof(CRecordset,m_rgFieldInfos)==88 && offsetof(CRecordset,m_mapFieldIndex)==96 && offsetof(CRecordset,m_mapParamIndex)==152 &&
    offsetof(CRecordset,m_pbFieldFlags)==376 && offsetof(CRecordset,m_pbParamFlags)==384 && offsetof(CRecordset,m_plParamLength)==392,"native recordset field storage offsets");
static_assert(sizeof(CFieldInfo)==24 && offsetof(CFieldInfo,m_nLength)==8 && offsetof(CFieldInfo,m_nDataType)==16 && offsetof(CFieldInfo,m_bStatus)==20,"native recordset cache entry layout");
