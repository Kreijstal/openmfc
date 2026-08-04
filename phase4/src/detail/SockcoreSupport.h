#pragma once
// Shared internals of the former sockcore.cpp translation unit.
// Definitions live in detail/SockcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace sockcore {} } }
using namespace openmfc::detail::sockcore;
// Socket Implementation
// Provides MFC wrappers around WinSock2 API
//
// All exports use MSVC ABI via extern "C" + .def aliasing.
// impl_xxx functions are the exported names as mapped in openmfc.def.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxsock.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <deque>
#include <map>
#include <mutex>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// WinSock Initialization State
//=============================================================================
namespace openmfc { namespace detail { namespace sockcore {
extern int g_bSocketsInitialized;
} } }
namespace openmfc { namespace detail { namespace sockcore {
extern WSADATA g_wsaData;
} } }

// Simple socket handle map for LookupHandle/KillSocket/AttachHandle
namespace openmfc { namespace detail { namespace sockcore {
extern std::map<SOCKET, CAsyncSocket*> g_socketMap;
} } }
namespace openmfc { namespace detail { namespace sockcore {
extern std::mutex g_socketMapMutex;
} } }
static constexpr UINT kSocketNotifyMessage = WM_USER + 0;
static constexpr UINT kSocketDeadMessage = WM_USER + 1;

struct AuxSocketMessage {
    UINT message;
    SOCKET socket;
    long lParam;
};

namespace openmfc { namespace detail { namespace sockcore {
extern std::deque<AuxSocketMessage> g_auxQueue;
} } }
namespace openmfc { namespace detail { namespace sockcore {
extern std::mutex g_auxQueueMutex;
} } }

//=============================================================================
// AfxSocketInit (existing public export - symbol already in thunks.cpp)
//=============================================================================
int AfxSocketInit(WSADATA* lpwsaData);



//=============================================================================
// CAsyncSocket C++ implementation
//=============================================================================









































//=============================================================================
// CSocket C++ implementation
//=============================================================================




















//=============================================================================
// CSocketFile C++ implementation
//=============================================================================

// Manual CRuntimeClass definition (CFile doesn't derive from CObject)














//=============================================================================
// CSocketWnd C++ implementation
//=============================================================================





//=============================================================================
// impl_xxx Export Wrappers
// The .def file maps MSVC_NAME=impl_xxx for each symbol below.
// Symbols marked with "// Symbol:" are counted as real by gen_manifest.py.
//=============================================================================









































