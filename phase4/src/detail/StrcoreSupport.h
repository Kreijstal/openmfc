#pragma once
// Shared internals of the former strcore.cpp translation unit.
// Definitions live in detail/StrcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace strcore {} } }
using namespace openmfc::detail::strcore;
// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstdint>
#include <cwctype>

// MS ABI calling convention for x64
#if defined(__GNUC__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Helper to get resource handle
namespace openmfc { namespace detail { namespace strcore {
HINSTANCE GetResourceHandle();
} } }

namespace openmfc { namespace detail { namespace strcore {
int CStringLoadStringImpl(CString* pThis, HINSTANCE hInst, UINT nID);
} } }







namespace openmfc { namespace detail { namespace strcore {
void AfxFormatStringsCore(CString* rString, const wchar_t* lpszFormat, const wchar_t* const* rglpsz, int nString);
} } }






namespace openmfc { namespace detail { namespace strcore {
wchar_t NormalizePathChar(wchar_t ch);
} } }



