#pragma once
// Shared internals of the former global_string_buffer_format.cpp translation unit.
// Definitions live in detail/CStringTSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cstringt {} } }
using namespace openmfc::detail::cstringt;
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>
#include <cwchar>
#include <cstdarg>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// This shard implements the wide-character (CStringW / CSimpleStringT<wchar_t>)
// string methods.  The repo's CString class (include/openmfc/afxstr.h) is an
// ABI-faithful CStringW: a single m_pszData pointer with a CStringData header
// stored immediately before the character buffer.  CSimpleStringT<wchar_t,1>
// is the base subobject of CStringT and shares the exact same layout (one
// pointer), so a CSimpleStringT<wchar_t>* and a CStringT<wchar_t>* both alias a
// CString*.  We therefore route every method through the CString public API,
// or, for the few accessors not exposed publicly, through the documented
// CStringData layout.

// Read the (private) m_pszData pointer: it is the first and only data member.
namespace openmfc { namespace detail { namespace cstringt {
inline wchar_t* PszOf(void* pThis) {
    return *reinterpret_cast<wchar_t**>(pThis);
}
} } }
namespace openmfc { namespace detail { namespace cstringt {
inline CStringData* DataOf(void* pThis) {
    return reinterpret_cast<CStringData*>(PszOf(pThis)) - 1;
}
} } }

namespace openmfc { namespace detail { namespace cstringt {
inline bool IsTrimChar(wchar_t ch, const wchar_t* trimSet) {
    return trimSet ? std::wcschr(trimSet, ch) != nullptr : false;
}
} } }

namespace openmfc { namespace detail { namespace cstringt {
CString* TrimLeftWithSet(CString* pThis, const wchar_t* trimSet);
} } }

namespace openmfc { namespace detail { namespace cstringt {
CString* TrimRightWithSet(CString* pThis, const wchar_t* trimSet);
} } }

// ---- CSimpleStringT<wchar_t,1> accessors -------------------------------------







// ---- CStringT<wchar_t> methods ----------------------------------------------




















