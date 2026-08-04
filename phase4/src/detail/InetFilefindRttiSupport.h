#pragma once
// Shared internals of the former inet_filefind_rtti.cpp translation unit.
// Definitions live in detail/InetFilefindRttiSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace inetfilefindrtti {} } }
using namespace openmfc::detail::inetfilefindrtti;
// OpenMFC shard: internet_wininet 01
// Real implementations for selected CFileFind / CFtpFileFind / CGopherConnection /
// CGopherFile exports. Each function overrides the corresponding weak stub.
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxstr.h"
#include <windows.h>
#include <wininet.h>
#include <cstring>
#include <cstdlib>
#include <new>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef GetObject
#undef GetObject
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


namespace openmfc { namespace detail { namespace inetfilefindrtti {
struct GopherFindState {
    GOPHER_FIND_DATAW data;
    CString locator;
};
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
std::unordered_map<const void*, GopherFindState>& GopherStates();
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
bool HasFileTime(const FILETIME& ft);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
long long FileTimeToCTimeValue(const FILETIME& ft);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
const wchar_t* LocatorString(const CGopherLocator* pLocator);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
CString FinderName(const CFtpFileFind* pThis);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
CString FinderLocator(const CFtpFileFind* pThis);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
FILETIME FinderWriteTime(const CFtpFileFind* pThis);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
ULONGLONG FinderLength(const CFtpFileFind* pThis);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
CString FinderUrl(const CFtpFileFind* pThis);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
CString FormatGopherAttribute(const GOPHER_ATTRIBUTE_TYPE& attr);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
void FillLocator_InetFilefindRtti(CGopherLocator* pRet, const CString& locator);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
void CloseGopherFinder(CFtpFileFind* pThis);
} } }

namespace openmfc { namespace detail { namespace inetfilefindrtti {
int FindGopher(CFtpFileFind* pThis, const wchar_t* locator,
               const wchar_t* search, DWORD flags);
} } }









































