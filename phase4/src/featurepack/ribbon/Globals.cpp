// Globals — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCComBSTR_ATL___Z(
    CArchive* ar, BSTR* pBstr);
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCComBSTR_ATL___Z(
    CArchive* ar, BSTR* pBstr);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

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
// Transcribed from retail RVA 0x273110 (mfc140u): zero count returns and a
// nonzero count with a null element buffer throws invalid-argument; loading
// delegates each element to CArchive::operator>>, while storing duplicates each
// non-null BSTR by byte length and passes the owned copy to operator<<. Retail
// checks m_nMode at archive+0x20. OpenMFC places m_nMode at +0x10, so this code
// uses its CArchive::IsStoring() accessor instead of hardcoding the retail
// offset; the layout discrepancy remains an integration issue for the archive.
extern "C" void MS_ABI impl____SerializeElements_VCComBSTR_ATL____YAXAEAVCArchive__PEAVCComBSTR_ATL___J_Z(
    CArchive* ar, BSTR* pElements, long long nCount) {
    unsigned long long remaining = static_cast<unsigned long long>(nCount);
    if (remaining == 0) {
        return;
    }
    if (pElements == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }

    if (!ar->IsStoring()) {
        do {
            impl___5_YAAEAVCArchive__AEAV0_AEAVCComBSTR_ATL___Z(ar, pElements);
            ++pElements;
        } while (--remaining != 0);
        return;
    }

    do {
        BSTR copy = nullptr;
        if (*pElements != nullptr) {
            const unsigned int byteLength = ::SysStringByteLen(*pElements);
            copy = ::SysAllocStringByteLen(reinterpret_cast<const char*>(*pElements), byteLength);
            if (copy == nullptr) {
                impl__AfxThrowMemoryException__YAXXZ();
                return;
            }
        }
        try {
            impl___6_YAAEAVCArchive__AEAV0_VCComBSTR_ATL___Z(ar, &copy);
            // operator<< frees the by-value BSTR on its normal return.
            copy = nullptr;
        } catch (...) {
            if (copy != nullptr) {
                ::SysFreeString(copy);
            }
            throw;
        }
        ++pElements;
    } while (--remaining != 0);
}
