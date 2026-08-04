#pragma once
// Shared internals of the former ole_colestreamfile_ext.cpp translation unit.
// Definitions live in detail/COleStreamFileSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colestreamfile {} } }
using namespace openmfc::detail::colestreamfile;
// OpenMFC: COleStreamFile — IStream-lifecycle methods (ole32-backed).
//
// COleStreamFile : public CFile, adding `LPSTREAM m_lpStream`. The repo layout
// (verified at runtime) is: sizeof(CFile)=24, m_lpStream @ offset 24,
// sizeof(COleStreamFile)=64 (24 base + 8 ptr + 32 padding). olecore.cpp already
// owns GetStream/Attach/Detach/Read/Write/Seek/LockRange/UnlockRange and
// thunks.cpp their impl__ thunks; this TU adds the remaining lifecycle methods
// that drive the underlying IStream directly. Each is a distinct mangled symbol.
//
// All methods operate on m_lpStream via a layout-faithful view, so they need no
// header changes and link/run standalone (the test exercises them under Wine
// against a real CreateStreamOnHGlobal / StgCreateDocfile stream).
//
// Deliberately NOT implemented here (left as honest weak stubs):
// GetRuntimeClass/GetThisClass (the repo's CFile family is intentionally
// non-CObject and returns null RTTI).
//
// Duplicate, GetStatus and GetStorageName WERE listed here as unimplemented,
// but the code below implements all three. Their `// Symbol:` markers carried
// 32-bit mangling (PAV/QBA/UBA) where retail exports the 64-bit form, so the
// .def aliased each export to a correctly-mangled impl__ name that only the
// stub generator defined -- the export returned 0 and this code was
// unreachable. Markers and impl__ names corrected; the header now matches.

#include <windows.h>
#include <objidl.h>
#include <cstddef>   // offsetof — llvm-mingw/clang doesn't pull it in via <windows.h>
#include <cstring>
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view of COleStreamFile: m_lpStream lives at offset 24.
namespace openmfc { namespace detail { namespace colestreamfile {
struct OleStreamFileView {
    char     cfile_base[24];   // CFile subobject (vptr + handle + CString)
    IStream* m_lpStream;       // offset 24
    char     pad[32];          // trailing padding (sizeof == 64)
};
} } }

namespace openmfc { namespace detail { namespace colestreamfile {
inline IStream*& Stream(void* pThis) {
    return reinterpret_cast<OleStreamFileView*>(pThis)->m_lpStream;
}
} } }













