#pragma once
// Shared internals of the former global_cmfcpropertysheet.cpp translation unit.
// Definitions live in detail/CMFCPropertySheetSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcpropertysheet {} } }
using namespace openmfc::detail::cmfcpropertysheet;
// OpenMFC — real implementations for CMFCPropertySheet stub exports.
//
// CMFCPropertySheet (size 29232, base CPropertySheet) adds navigation pane
// (tree/list), page headers, and visual-manager integration. This file provides
// minimal-but-real implementations: ctors/dtors delegate to the base
// CPropertySheet, virtual overrides forward where possible, and CMFC-specific
// helpers return sensible defaults.
//
// Every impl_ name matches the .def alias exactly. // Symbol: markers exclude
// these from auto-generated weak stubs so the linker picks the strong symbol.

#include <windows.h>
#include <cstring>
#include <algorithm>
#include <new>
#include <mutex>
#include <unordered_map>
#include <cwchar>
#include <vector>
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Forward declarations for symbols we delegate to (defined in dlgcore.cpp / thunks.cpp).
extern "C" void MS_ABI impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(CPropertySheet*, CPropertyPage*);
extern "C" void MS_ABI impl__RemovePage_CPropertySheet__QEAAXH_Z(CPropertySheet*, int);
extern "C" void MS_ABI impl__RemovePage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(CPropertySheet*, CPropertyPage*);
extern "C" int MS_ABI impl__OnInitDialog_CPropertySheet__UEAAHXZ(CPropertySheet*);
extern "C" int MS_ABI impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(CPropertySheet*, MSG*);
extern "C" void MS_ABI impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(CPropertySheet*, CWnd*, unsigned int);
extern "C" void MS_ABI impl___1CMFCPropertySheetCategoryInfo__UEAA_XZ(void*);
extern "C" void* MS_ABI impl___0CMFCPropertySheetCategoryInfo__QEAA_PEB_WHHPEBV0_AEAVCMFCPropertySheet___Z(
    void*, const wchar_t*, int, int, const void*, void*);


// Size harvested from mfc140u.dll (tools/harvest).
namespace openmfc { namespace detail { namespace cmfcpropertysheet {
static const size_t kSizeof_CMFCPropertySheet = 29232;
} } }
namespace openmfc { namespace detail { namespace cmfcpropertysheet {
static const size_t kSizeof_CMFCPropertySheetCategoryInfo = 168;
} } }
namespace openmfc { namespace detail { namespace cmfcpropertysheet {
static const int kDefaultNavBarWidth = 180;
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
struct CMFCPropertySheetState {
    bool pageHeaderEnabled = true;
    bool leftNavigationPane = true;
    bool dynamicLayout = false;
    bool tabCreated = false;
    bool navCreated = false;
    int navBarWidth = kDefaultNavBarWidth;
    int look = 0;
    int iconCx = 0;
    unsigned int nImageList = 0;
    void* imagelist = nullptr;
    void* tabCtrl = nullptr;
    void* navWnd = nullptr;
    std::vector<void*> navPages;
    std::vector<void*> pages;
    std::vector<void*> categories;
    std::vector<void*> rootCategories;
    std::unordered_map<void*, std::vector<void*>> categoryPages;
    std::unordered_map<void*, std::vector<void*>> categoryChildren;
    std::unordered_map<void*, void*> categoryParent;
    std::unordered_map<void*, int> pageToNavIndex;
    std::unordered_map<void*, void*> pageToCategory;
};
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
extern std::mutex g_stateMutex;
} } }
namespace openmfc { namespace detail { namespace cmfcpropertysheet {
extern std::unordered_map<const void*, CMFCPropertySheetState> g_sheetStates;
} } }
namespace openmfc { namespace detail { namespace cmfcpropertysheet {
extern std::unordered_map<void*, void*> g_categoryMap;
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
CMFCPropertySheetState* GetSheetState(const void* pThis, bool createIfMissing);
} } }


namespace openmfc { namespace detail { namespace cmfcpropertysheet {
template <typename T>
void EraseFromVector(std::vector<T>& values, const T& value) {
    values.erase(std::remove(values.begin(), values.end(), value), values.end());
}
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
void ReindexNavPages(CMFCPropertySheetState& state);
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
void* CategoryGetParentPtr(const void* pCategory);
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
void* CategoryGetOwnerPtr(const void* pCategory);
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
int RemovePageFromState(CMFCPropertySheetState& state, void* pPage);
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
int AddPageLabelIndex(CMFCPropertySheetState& state, CPropertyPage* pPage, const wchar_t* fallbackLabel);
} } }

namespace openmfc { namespace detail { namespace cmfcpropertysheet {
void BuildPageListText(void* pPage, wchar_t* out, size_t cch, int fallbackIndex);
} } }


// Helper: zero-init the CMFCPropertySheet object and placement-new the base
// CPropertySheet with the given ctor args. The most-derived vtable is installed
// at runtime through the CRuntimeClass / CreateObject path.
namespace openmfc { namespace detail { namespace cmfcpropertysheet {
inline void* InitBase_Cmfcpropertysheet(void* pThis, CWnd* pParentWnd, unsigned int iSelectPage) {
    std::memset(pThis, 0, kSizeof_CMFCPropertySheet);
    auto* base = static_cast<CPropertySheet*>(pThis);
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(base, pParentWnd, iSelectPage);
    if (auto* state = GetSheetState(pThis, true)) {
        state->navBarWidth = kDefaultNavBarWidth;
    }
    return pThis;
}
} } }


// =============================================================================
// Constructors / Destructor (ords 646, 647, 648, 1236)
// =============================================================================





// =============================================================================
// Page management — forward to base CPropertySheet (ords 1903, 12388, 12389)
// =============================================================================




// =============================================================================
// Overrides — forward to base or return default (ords 10172, 11884, 7727, 14336, 14350)
// =============================================================================






// =============================================================================
// CMFC-specific helpers — no-op or sensible defaults (ords 4292, 2887, 4587, 7807, 6738, 14346)
// =============================================================================







// =============================================================================
// Page-header / activation notifications — no-op (ords 9459, 8606, 8626)
// =============================================================================




// =============================================================================
// Tree / navigation-list callbacks — no-op or default (ords 1816, 1906, 2027,
// 12339, 9990, 10895, 10996, 10997, 11145, 11314)
// =============================================================================











// =============================================================================
// Visual / icon / look (ords 13253, 13254, 13338)
// =============================================================================



