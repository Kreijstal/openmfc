// CMFCPropertySheetCategoryInfo — OpenMFC implementation.
// Sources: global_cmfcpropertysheetcategoryinfo.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCPropertySheetCategoryInfoSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CMFCPropertySheetCategoryInfo@@QEAA@PEB_WHHPEBV0@AEAVCMFCPropertySheet@@@Z
extern "C" void* MS_ABI impl___0CMFCPropertySheetCategoryInfo__QEAA_PEB_WHHPEBV0_AEAVCMFCPropertySheet___Z(
    void* pThis, const wchar_t* lpszName, int nIcon, int nSelectedItem,
    const void* pParentCategory, void* pPropSheet) {
    std::memset(pThis, 0, kSizeof_CMFCPropertySheetCategoryInfo);
    // Store name pointer (caller owns the string).
    // Offset 8 (after vfptr): m_strName
    *reinterpret_cast<const wchar_t**>(static_cast<char*>(pThis) + 8) = lpszName;
    // Offset 16: m_nIcon
    *reinterpret_cast<int*>(static_cast<char*>(pThis) + 16) = nIcon;
    // Offset 20: m_nSelectedItem
    *reinterpret_cast<int*>(static_cast<char*>(pThis) + 20) = nSelectedItem;
    // Offset 24: m_pParentCategory
    *reinterpret_cast<const void**>(static_cast<char*>(pThis) + 24) = pParentCategory;
    // Offset 32: m_pSheet (back-reference to owning CMFCPropertySheet)
    *reinterpret_cast<void**>(static_cast<char*>(pThis) + 32) = pPropSheet;
    return pThis;
}
// Symbol: ??1CMFCPropertySheetCategoryInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCPropertySheetCategoryInfo__UEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<CObject*>(pThis)->~CObject();
    }
}
