#include "COleSafeArraySupport.h"

static_assert(sizeof(CSAView) == 32, "COleSafeArray must be 32 bytes (tagVARIANT + 2 DWORD)");
namespace openmfc { namespace detail { namespace colesafearray {
BOOL _AfxCompareSafeArrays_OleCsafearrayExt(SAFEARRAY* parray1, SAFEARRAY* parray2)
{
    if (!parray1 || !parray2) return parray1 == parray2;
    DWORD dwDim1 = SafeArrayGetDim(parray1);
    DWORD dwDim2 = SafeArrayGetDim(parray2);
    if (dwDim1 != dwDim2) return FALSE;
    if (dwDim1 == 0) return TRUE;
    DWORD dwSize1 = SafeArrayGetElemsize(parray1);
    DWORD dwSize2 = SafeArrayGetElemsize(parray2);
    if (dwSize1 != dwSize2) return FALSE;

    BOOL bCompare = FALSE;
    long* pLBound1 = new (std::nothrow) long[dwDim1];
    long* pLBound2 = new (std::nothrow) long[dwDim2];
    long* pUBound1 = new (std::nothrow) long[dwDim1];
    long* pUBound2 = new (std::nothrow) long[dwDim2];
    void* pData1 = nullptr;
    void* pData2 = nullptr;

    if (!pLBound1 || !pLBound2 || !pUBound1 || !pUBound2) goto cleanup;

    {
        size_t nTotalElements = 1;
        for (DWORD i = 0; i < dwDim1; ++i) {
            if (FAILED(SafeArrayGetLBound(parray1, i+1, &pLBound1[i])) ||
                FAILED(SafeArrayGetLBound(parray2, i+1, &pLBound2[i])) ||
                FAILED(SafeArrayGetUBound(parray1, i+1, &pUBound1[i])) ||
                FAILED(SafeArrayGetUBound(parray2, i+1, &pUBound2[i])))
                goto cleanup;
            if (pUBound1[i] - pLBound1[i] != pUBound2[i] - pLBound2[i])
                goto cleanup;
            nTotalElements *= (size_t)(pUBound1[i] - pLBound1[i] + 1);
        }
        if (FAILED(SafeArrayAccessData(parray1, &pData1)) || !pData1) goto cleanup;
        if (FAILED(SafeArrayAccessData(parray2, &pData2)) || !pData2) goto cleanup;
        size_t nSize = nTotalElements * dwSize1;
        bCompare = (memcmp(pData1, pData2, nSize) == 0);
        SafeArrayUnaccessData(parray1);
        SafeArrayUnaccessData(parray2);
    }

cleanup:
    if (pData1) SafeArrayUnaccessData(parray1);
    if (pData2) SafeArrayUnaccessData(parray2);
    delete[] pLBound1;
    delete[] pLBound2;
    delete[] pUBound1;
    delete[] pUBound2;
    return bCompare;
}
} } }  // namespace openmfc::detail::colesafearray
