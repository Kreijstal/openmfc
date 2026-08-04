#pragma once
// Shared internals of the former inet_cfilefind_times.cpp translation unit.
// Definitions live in detail/CFileFindSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cfilefind {} } }
using namespace openmfc::detail::cfilefind;
// OpenMFC: CFileFind — CTime-returning file-time accessors (kernel32-backed).
//
// CFileFind : public CObject caches the last WIN32_FIND_DATAW from FindFirst/
// FindNextFile. The repo layout (verified at runtime) is: CObject vptr @0,
// HANDLE m_hFindFile @8, WIN32_FIND_DATAW m_findData @16. The FILETIME* overloads
// of these getters are already implemented elsewhere; this TU adds the
// ATL::CTime& overloads, which the real exports expose. ATL::CTime is a single
// 8-byte __time64_t (seconds since the 1970 epoch).
//
// Each getter converts the cached FILETIME (100ns ticks since 1601) into the
// CTime epoch and writes it through the reference, returning TRUE when a find is
// active (m_hFindFile valid).

#include <windows.h>
#include <cstddef>   // offsetof — llvm-mingw/clang doesn't pull it in via <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful prefix of CFileFind (only the fields these getters touch).
namespace openmfc { namespace detail { namespace cfilefind {
struct CFileFindView {
    void*             vptr;          // CObject vptr @0
    HANDLE            m_hFindFile;   // @8
    WIN32_FIND_DATAW  m_findData;    // @16
};
} } }

// ATL::CTime is a single __time64_t.
namespace openmfc { namespace detail { namespace cfilefind {
typedef long long CTimeValue;
} } }

// FILETIME (100ns ticks since 1601-01-01) -> __time64_t (seconds since 1970).
// 11644473600 = seconds between 1601 and 1970.
namespace openmfc { namespace detail { namespace cfilefind {
inline CTimeValue FileTimeToCTime(const FILETIME& ft) {
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return (CTimeValue)(u.QuadPart / 10000000ULL) - 11644473600LL;
}
} } }

namespace openmfc { namespace detail { namespace cfilefind {
inline int GetTime(const void* pThis, const FILETIME& ft, CTimeValue* pOut) {
    if (!pThis || !pOut) return 0;
    const CFileFindView* p = reinterpret_cast<const CFileFindView*>(pThis);
    if (p->m_hFindFile == nullptr || p->m_hFindFile == INVALID_HANDLE_VALUE) return 0;
    *pOut = FileTimeToCTime(ft);
    return 1;
}
} } }




