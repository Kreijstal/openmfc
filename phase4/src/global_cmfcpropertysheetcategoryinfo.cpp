// OpenMFC — real implementations for CMFCPropertySheetCategoryInfo stub exports.
//
// CMFCPropertySheetCategoryInfo (base CObject) holds a single tree-category node
// in the CMFCPropertySheet navigation tree. It stores a name, icon indices,
// parent-category pointer, and back-reference to the owning CMFCPropertySheet.
//
// Harvested metadata indicates a 168-byte CMFCPropertySheetCategoryInfo object.

#include <windows.h>
#include <cstring>
#include <new>
#include "openmfc/afx.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CMFCPropertySheetCategoryInfo ctors / dtor (ords 649, 1237)
// =============================================================================

static const size_t kSizeof_CMFCPropertySheetCategoryInfo = 168;

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
