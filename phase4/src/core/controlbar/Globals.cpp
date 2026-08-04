// Globals — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ??$CopyElements@VCOleVariant@@@@YAXPEAVCOleVariant@@PEBV0@_J@Z
extern "C" void MS_ABI impl____CopyElements_VCOleVariant____YAXPEAVCOleVariant__PEBV0__J_Z(
    COleVariant* pDest, const COleVariant* pSrc, __int64 nCount) {
    while (nCount--) {
        impl___4COleVariant__QEAAAEBV0_AEBV0__Z(pDest, pSrc);
        ++pDest; ++pSrc;
    }
}
// Symbol: ??$SerializeElements@VCOleVariant@@@@YAXAEAVCArchive@@PEAVCOleVariant@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_VCOleVariant____YAXAEAVCArchive__PEAVCOleVariant___J_Z(
    CArchive* ar, COleVariant* pElements, __int64 nCount) {
    if (!ar || !pElements) return;
    if (ar->IsStoring())
        for (__int64 i = 0; i < nCount; ++i)
            impl___6_YAAEAVCArchive__AEAV0_VCOleVariant___Z(ar, reinterpret_cast<const VARIANT*>(&pElements[i]));
    else
        for (__int64 i = 0; i < nCount; ++i)
            impl___5_YAAEAVCArchive__AEAV0_AEAVCOleVariant___Z(ar, &pElements[i]);
}
