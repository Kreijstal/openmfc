// CFixedAllocNoSync — OpenMFC implementation.
// Sources: collections_cfixedalloc.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CFixedAllocSupport.h"

// Symbol: ??0CFixedAllocNoSync@@QEAA@II@Z
extern "C" void MS_ABI impl___0CFixedAllocNoSync__QEAA_II_Z(CFixedAllocState* pThis, unsigned int nAllocSize, unsigned int nBlockSize) {
    fa_construct(pThis, nAllocSize, nBlockSize);
}
// Symbol: ??1CFixedAllocNoSync@@QEAA@XZ
extern "C" void MS_ABI impl___1CFixedAllocNoSync__QEAA_XZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}
// Symbol: ?Alloc@CFixedAllocNoSync@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Alloc_CFixedAllocNoSync__QEAAPEAXXZ(CFixedAllocState* pThis) {
    return fa_alloc(pThis);
}
// Symbol: ?Free@CFixedAllocNoSync@@QEAAXPEAX@Z
extern "C" void MS_ABI impl__Free_CFixedAllocNoSync__QEAAXPEAX_Z(CFixedAllocState* pThis, void* p) {
    fa_free(pThis, p);
}
// Symbol: ?FreeAll@CFixedAllocNoSync@@QEAAXXZ
extern "C" void MS_ABI impl__FreeAll_CFixedAllocNoSync__QEAAXXZ(CFixedAllocState* pThis) {
    fa_free_all(pThis);
}
