// Globals — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??$HashKey@AEBUtagVARIANT@@@@YAIAEBUtagVARIANT@@@Z
extern "C" long long MS_ABI impl____HashKey_AEBUtagVARIANT____YAIAEBUtagVARIANT___Z(
    void* pThis, const void* value) {
    (void)pThis;
    if (value == nullptr) {
        return 0;
    }
    const void* firstField = value;
    return static_cast<long long>(
        HashRawBytes(firstField, sizeof(unsigned long long) * 4));
}
// Symbol: ??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z
extern "C" long long MS_ABI impl____HashKey_VCComBSTR_ATL____YAIVCComBSTR_ATL___Z(
    void* pThis, const void* value) {
    (void)pThis;
    if (value == nullptr) {
        return 0;
    }
    const void* firstField = value;
    unsigned long long hash = HashRawBytes(firstField, sizeof(unsigned long long) * 2);
    if (hash == 0) {
        hash = 1;
    }
    return static_cast<long long>(hash);
}
// Symbol: ??$SerializeElements@VCComBSTR@ATL@@@@YAXAEAVCArchive@@PEAVCComBSTR@ATL@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_VCComBSTR_ATL____YAXAEAVCArchive__PEAVCComBSTR_ATL___J_Z(
    void* pThis, void* pArchive, void* pList, long long* pHash) {
    (void)pThis;
    (void)pArchive;
    (void)pList;
    (void)pHash;
}
