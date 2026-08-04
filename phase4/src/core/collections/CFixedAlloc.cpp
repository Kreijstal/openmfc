// CFixedAlloc — OpenMFC implementation.
// Sources: collections_cfixedalloc.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CFixedAllocSupport.h"

// Symbol: ??0CFixedAlloc@@QEAA@II@Z
extern "C" void MS_ABI impl___0CFixedAlloc__QEAA_II_Z(CFixedAllocState* pThis, unsigned int nAllocSize, unsigned int nBlockSize) {
    fa_construct(pThis, nAllocSize, nBlockSize);
}
// Symbol: ??1CFixedAlloc@@QEAA@XZ
extern "C" void MS_ABI impl___1CFixedAlloc__QEAA_XZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}
// Symbol: ?Alloc@CFixedAlloc@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Alloc_CFixedAlloc__QEAAPEAXXZ(CFixedAllocState* pThis) {
    return fa_alloc(pThis);
}
// Symbol: ?Free@CFixedAlloc@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__Free_CFixedAlloc__QEAAXPEAX_Z(CFixedAllocState* pThis, void* p) {
    fa_free(pThis, p);
}
// Symbol: ?FreeAll@CFixedAlloc@@QEAAXXZ
extern "C" void MS_ABI impl__FreeAll_CFixedAlloc__QEAAXXZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}
