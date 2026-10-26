// Native release-x64 synchronized fixed allocator (mfc140u 0x22d310..0x22d45e).
#define OPENMFC_APPCORE_IMPL
#include "detail/CFixedAllocSupport.h"
#include "openmfc/afxwin.h"
// Symbol: ??0CFixedAlloc@@QEAA@II@Z
extern "C" CFixedAllocSynchronizedState* MS_ABI impl___0CFixedAlloc__QEAA_II_Z(CFixedAllocSynchronizedState* self,unsigned int size,unsigned int count) {
    fa_construct(self,size,count);::InitializeCriticalSection(&self->m_lock);return self;
}
// Symbol: ??1CFixedAlloc@@QEAA@XZ
extern "C" void MS_ABI impl___1CFixedAlloc__QEAA_XZ(CFixedAllocSynchronizedState* self) {
    ::DeleteCriticalSection(&self->m_lock);fa_free_all(self);
}
// Symbol: ?Alloc@CFixedAlloc@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Alloc_CFixedAlloc__QEAAPEAXXZ(CFixedAllocSynchronizedState* self) {
    auto* section=&self->m_lock;::EnterCriticalSection(section);
    void* result;
    // Native Alloc catches framework pointer exceptions specifically, including
    // failures in its normal LeaveCriticalSection call.
    try {result=fa_alloc(self);::LeaveCriticalSection(section);return result;}
    catch(CException*) {::LeaveCriticalSection(section);throw;}
}
// Symbol: ?Free@CFixedAlloc@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__Free_CFixedAlloc__QEAAXPEAX_Z(CFixedAllocSynchronizedState* self,void* memory) {
    if(!memory)return;::EnterCriticalSection(&self->m_lock);
    __try {fa_free(self,memory);}
    __finally {::LeaveCriticalSection(&self->m_lock);}
}
// Symbol: ?FreeAll@CFixedAlloc@@QEAAXXZ
extern "C" void MS_ABI impl__FreeAll_CFixedAlloc__QEAAXXZ(CFixedAllocSynchronizedState* self) {
    ::EnterCriticalSection(&self->m_lock);
    __try {fa_free_all(self);}
    __finally {::LeaveCriticalSection(&self->m_lock);}
}
