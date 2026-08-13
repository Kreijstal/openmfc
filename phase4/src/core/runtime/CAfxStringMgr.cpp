// CAfxStringMgr — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; ATL IAtlStringMgr-compatible manager.

#include "openmfc/afxstr.h"
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CAfxStringMgr's vtable matches IAtlStringMgr (single inheritance, no extra
// data members), so `this` can be handed back to CStringData as pStringMgr.

// Symbol: ?Allocate@CAfxStringMgr@@UEAAPEAUCStringData@ATL@@HH@Z
extern "C" CStringData* MS_ABI impl__Allocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__HH_Z(
    void* pThis, int nAllocLength, int nCharSize) {
    size_t nBytes = sizeof(CStringData) + static_cast<size_t>(nAllocLength + 1) * nCharSize;
    CStringData* pData = static_cast<CStringData*>(malloc(nBytes));
    if (!pData) return nullptr;
    pData->pStringMgr = reinterpret_cast<IAtlStringMgr*>(pThis);
    pData->nDataLength = 0;
    pData->nAllocLength = nAllocLength;
    pData->nRefs = 1;
    return pData;
}

// Symbol: ?Clone@CAfxStringMgr@@UEAAPEAUIAtlStringMgr@ATL@@XZ
extern "C" IAtlStringMgr* MS_ABI impl__Clone_CAfxStringMgr__UEAAPEAUIAtlStringMgr_ATL__XZ(
    void* pThis) {
    return reinterpret_cast<IAtlStringMgr*>(pThis);
}

// Symbol: ?Free@CAfxStringMgr@@UEAAXPEAUCStringData@ATL@@@Z
extern "C" void MS_ABI impl__Free_CAfxStringMgr__UEAAXPEAUCStringData_ATL___Z(
    void* pThis, CStringData* pData) {
    (void)pThis;
    free(pData);
}

// Symbol: ?GetNilString@CAfxStringMgr@@UEAAPEAUCStringData@ATL@@XZ
extern "C" CStringData* MS_ABI impl__GetNilString_CAfxStringMgr__UEAAPEAUCStringData_ATL__XZ(
    void* pThis) {
    (void)pThis;
    return GetNilStringData();
}

// Symbol: ?Reallocate@CAfxStringMgr@@UEAAPEAUCStringData@ATL@@PEAU23@HH@Z
extern "C" CStringData* MS_ABI impl__Reallocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__PEAU23_HH_Z(
    void* pThis, CStringData* pData, int nAllocLength, int nCharSize) {
    size_t nBytes = sizeof(CStringData) + static_cast<size_t>(nAllocLength + 1) * nCharSize;
    CStringData* pNew = static_cast<CStringData*>(realloc(pData, nBytes));
    if (!pNew) return nullptr;
    pNew->pStringMgr = reinterpret_cast<IAtlStringMgr*>(pThis);
    pNew->nAllocLength = nAllocLength;
    return pNew;
}
