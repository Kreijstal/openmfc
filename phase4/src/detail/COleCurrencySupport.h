#pragma once
// Shared internals of the former ole_colecurrency_parse.cpp translation unit.
// Definitions live in detail/COleCurrencySupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colecurrency {} } }
using namespace openmfc::detail::colecurrency;
// OpenMFC: COleCurrency::ParseCurrency (oleaut32-backed).
//
// COleCurrency holds a CURRENCY (CY) value plus a status enum. Layout (verified
// at runtime): CY m_cur @0, CurrencyStatus m_status @8 (CY_VALID=0, CY_INVALID=1,
// CY_NULL=2), sizeof 32. ParseCurrency parses a localized currency string into
// m_cur via VarCyFromStr, mirroring MFC: on success m_status becomes CY_VALID,
// on failure m_cur is zeroed and m_status becomes CY_INVALID.
//
// COleCurrency::SetCurrency is already implemented in olecore.cpp; Format (a
// CStringT-by-value return) is left for a string-coupled tranche.

#include <windows.h>
#include <oleauto.h>
#include "openmfc/afxole.h"
#include <new>
#include <cstddef>   // offsetof — llvm-mingw/clang doesn't pull it in via <windows.h>
#include "openmfc/afxstr.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout-faithful view of COleCurrency (only the two leading members).
namespace openmfc { namespace detail { namespace colecurrency {
struct COleCurrencyView {
    CY  m_cur;       // @0
    int m_status;    // @8  (0=valid, 1=invalid, 2=null)
};
} } }
namespace openmfc { namespace detail { namespace colecurrency {
enum { CY_VALID = 0, CY_INVALID = 1 };
} } }


// ?Format@COleCurrency@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@KK@Z
// COleCurrency::Format
namespace openmfc { namespace detail { namespace colecurrency {
extern "C" void MS_ABI impl__Format_COleCurrency__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__KK_Z(
    CString* pRet, const void* pThis, unsigned long dwFormat, unsigned long lcid);
} } }





namespace openmfc { namespace detail { namespace colecurrency {
enum ComparisonStatus {
    CMP_INVALID = 1,
    CMP_NULL = 2,
    CMP_VALID = 0,
};
} } }

namespace openmfc { namespace detail { namespace colecurrency {
ComparisonStatus CurrencyStatusFromPair(const COleCurrency* lhs, const COleCurrency* rhs);
} } }

namespace openmfc { namespace detail { namespace colecurrency {
ComparisonStatus CurrencyStatusSingle(const COleCurrency* value);
} } }










