#pragma once
// Shared internals of the former inetcore.cpp translation unit.
// Definitions live in detail/InetcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace inetcore {} } }
using namespace openmfc::detail::inetcore;
// Internet (WinInet) Implementation
// Provides MFC wrappers around WinInet API for HTTP, FTP, Gopher
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
#include <cwchar>
#include <new>
#include <vector>
#include <cwctype>

#ifdef GetObject
#undef GetObject
#endif
#ifdef SetCurrentDirectory
#undef SetCurrentDirectory
#endif
#ifdef CreateDirectory
#undef CreateDirectory
#endif
#ifdef RemoveDirectory
#undef RemoveDirectory
#endif
#ifdef GetCurrentDirectory
#undef GetCurrentDirectory
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CInternetSession
//=============================================================================


// Map for looking up sessions by handle
#include <unordered_map>
namespace openmfc { namespace detail { namespace inetcore {
extern std::unordered_map<HINTERNET, CInternetSession*> g_sessionMap;
} } }


namespace openmfc { namespace detail { namespace inetcore {
extern thread_local CString g_lastFileName;
} } }

namespace openmfc { namespace detail { namespace inetcore {
CString LocatorToString(const CGopherLocator* pLocator);
} } }

namespace openmfc { namespace detail { namespace inetcore {
void FillLocator_Inetcore(CGopherLocator* pRet, const CString& locator);
} } }

namespace openmfc { namespace detail { namespace inetcore {
CInternetSession* SessionForHandle(HINTERNET hInternet);
} } }

namespace openmfc { namespace detail { namespace inetcore {
void CALLBACK WinInetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus,
                                   LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
} } }

namespace openmfc { namespace detail { namespace inetcore {
CString MapGopherTypeToString(unsigned long dwType);
} } }

namespace openmfc { namespace detail { namespace inetcore {
CInternetSession* SessionForStaticHelpers();
} } }

namespace openmfc { namespace detail { namespace inetcore {
CString LocatorTypeFromApi(const CGopherLocator& refLocator);
} } }

namespace openmfc { namespace detail { namespace inetcore {
bool BuildGopherLocator(CString& out, const CString& host, INTERNET_PORT port,
                       const CString& display, const CString& selector, DWORD gopherType);
} } }

namespace openmfc { namespace detail { namespace inetcore {
void AppendGopherType(CString& locator, unsigned long gopherType);
} } }











//=============================================================================
// CInternetConnection
//=============================================================================








//=============================================================================
// CHttpConnection
//=============================================================================










//=============================================================================
// RTTI for CInternetFile and CInternetException
// CInternetFile is DECLARE_DYNAMIC in real MFC: base CStdioFile, schema 0xFFFF,
// real MSVC sizeof=128 (harvested from mfc140u.dll). The base descriptor lives in
// filecore.cpp (file-internal, not referenceable here), so chain via
// m_pfnGetBaseClass to CStdioFile's exported getter — the real-MFC _AFXDLL
// cross-module base-resolution mechanism.

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStdioFile__SAPEAUCRuntimeClass__XZ();
namespace openmfc { namespace detail { namespace inetcore {
CRuntimeClass* AFXAPI gb_CInternetFile();
} } }

namespace openmfc { namespace detail { namespace inetcore {
extern __attribute__((used)) CRuntimeClass g_classCInternetFile;
} } }

namespace openmfc { namespace detail { namespace inetcore {
extern __attribute__((used)) CRuntimeClass g_classCInternetException;
} } }

namespace openmfc { namespace detail { namespace inetcore {
extern __attribute__((used)) CRuntimeClass g_classCHttpFile;
} } }

namespace openmfc { namespace detail { namespace inetcore {
extern __attribute__((used)) CRuntimeClass g_classCGopherFile;
} } }

//=============================================================================
// CInternetFile
//=============================================================================

// Not using IMPLEMENT_DYNAMIC since base class CStdioFile lacks DECLARE_DYNAMIC






















//=============================================================================
// CHttpFile
//=============================================================================

// CHttpFile RTTI provided manually (CInternetFile doesn't have DECLARE_DYNAMIC)

















//=============================================================================
// CFtpConnection
//=============================================================================






















//=============================================================================
// CInternetException
//=============================================================================






//=============================================================================
// Additional CInternetSession methods
//=============================================================================












//=============================================================================
// Additional CHttpFile methods
//=============================================================================




//=============================================================================
// Additional CFtpConnection methods
//=============================================================================



//=============================================================================
// CGopherLocator — defined in include/openmfc/afxinet.h
//=============================================================================

//=============================================================================
// CFileFind implementations
//=============================================================================









//=============================================================================
// CFtpFileFind implementations
//=============================================================================







//=============================================================================
// CGopherFileFind
//=============================================================================




//=============================================================================
// CGopherConnection
//=============================================================================








// Convenience overload



//=============================================================================
// CGopherFile
//=============================================================================


//=============================================================================
// Internet file finder ABI thunks
//=============================================================================

namespace openmfc { namespace detail { namespace inetcore {
void openmfcConstructString(CString* pRet, const CString& value);
} } }

namespace openmfc { namespace detail { namespace inetcore {
int openmfcFileTimeToCTimeStorage(const FILETIME& fileTime, void* pTime);
} } }













































//=============================================================================
// Manual WinInet thunk implementations for remaining exports in this unit
//=============================================================================

























































