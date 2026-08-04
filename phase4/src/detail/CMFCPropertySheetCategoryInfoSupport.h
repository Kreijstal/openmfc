#pragma once
// Shared internals of the former global_cmfcpropertysheetcategoryinfo.cpp translation unit.
// Definitions live in detail/CMFCPropertySheetCategoryInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcpropertysheetcategoryinfo {} } }
using namespace openmfc::detail::cmfcpropertysheetcategoryinfo;
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


