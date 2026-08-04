#pragma once
// Shared internals of the former global_cmfccolorpropertysheet.cpp translation unit.
// Definitions live in detail/CMFCColorPropertySheetSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccolorpropertysheet {} } }
using namespace openmfc::detail::cmfccolorpropertysheet;
// OpenMFC — real implementations for CMFCColorPropertySheet stub exports.
//
// CMFCColorPropertySheet (size 408, base CPropertySheet) is the colour-picker
// property sheet used by CMFCColorMenuButton and the ribbon colour picker.
// It adds a colour palette control, accelerator table, and resize logic.
// This file provides minimal-but-real implementations that forward to base
// CPropertySheet where possible.

#include <windows.h>
#include <cstring>
#include <new>
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(CPropertySheet*, CWnd*, unsigned int);
extern "C" int MS_ABI impl__OnInitDialog_CPropertySheet__UEAAHXZ(CPropertySheet*);
extern "C" int MS_ABI impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(CPropertySheet*, MSG*);


// Size harvested from mfc140u.dll (tools/harvest).
namespace openmfc { namespace detail { namespace cmfccolorpropertysheet {
static const size_t kSizeof_CMFCColorPropertySheet = 408;
} } }

namespace openmfc { namespace detail { namespace cmfccolorpropertysheet {
inline void* InitBase_Cmfccolorpropertysheet(void* pThis, CWnd* pParentWnd, unsigned int iSelectPage) {
    std::memset(pThis, 0, kSizeof_CMFCColorPropertySheet);
    auto* base = static_cast<CPropertySheet*>(pThis);
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(base, pParentWnd, iSelectPage);
    return pThis;
}
} } }











