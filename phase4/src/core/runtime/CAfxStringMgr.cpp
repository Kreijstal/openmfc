// CAfxStringMgr — OpenMFC implementation.
// Actual 40-byte manager, embedded nil header and native allocation providers.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Five interface slots followed by the native embedded nil-string data.

// Symbol: ?Allocate@CAfxStringMgr@@UEAAPEAUCStringData@ATL@@HH@Z
extern "C" CStringData* MS_ABI impl__Allocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__HH_Z(
    void* pThis, int nAllocLength, int nCharSize) {
    return static_cast<CAfxStringMgr*>(pThis)->CAfxStringMgr::Allocate(nAllocLength,nCharSize);
}

// Symbol: ?Clone@CAfxStringMgr@@UEAAPEAUIAtlStringMgr@ATL@@XZ
extern "C" IAtlStringMgr* MS_ABI impl__Clone_CAfxStringMgr__UEAAPEAUIAtlStringMgr_ATL__XZ(
    void* pThis) {
    return static_cast<CAfxStringMgr*>(pThis)->CAfxStringMgr::Clone();
}

// Symbol: ?Free@CAfxStringMgr@@UEAAXPEAUCStringData@ATL@@@Z
extern "C" void MS_ABI impl__Free_CAfxStringMgr__UEAAXPEAUCStringData_ATL___Z(
    void* pThis, CStringData* pData) {
    static_cast<CAfxStringMgr*>(pThis)->CAfxStringMgr::Free(pData);
}

// Symbol: ?GetNilString@CAfxStringMgr@@UEAAPEAUCStringData@ATL@@XZ
extern "C" CStringData* MS_ABI impl__GetNilString_CAfxStringMgr__UEAAPEAUCStringData_ATL__XZ(
    void* pThis) {
    return static_cast<CAfxStringMgr*>(pThis)->CAfxStringMgr::GetNilString();
}

// Symbol: ?Reallocate@CAfxStringMgr@@UEAAPEAUCStringData@ATL@@PEAU23@HH@Z
extern "C" CStringData* MS_ABI impl__Reallocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__PEAU23_HH_Z(
    void* pThis, CStringData* pData, int nAllocLength, int nCharSize) {
    return static_cast<CAfxStringMgr*>(pThis)->CAfxStringMgr::Reallocate(pData,nAllocLength,nCharSize);
}

IAtlStringMgr* AFXAPI AfxGetStringManager() {return OpenMFC_GetStringMgr();}
// Symbol: ?AfxGetStringManager@@YAPEAUIAtlStringMgr@ATL@@XZ
extern "C" IAtlStringMgr* impl__AfxGetStringManager__YAPEAUIAtlStringMgr_ATL__XZ() {return AfxGetStringManager();}
