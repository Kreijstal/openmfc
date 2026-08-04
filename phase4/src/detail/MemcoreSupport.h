#pragma once
// Shared internals of the former memcore.cpp translation unit.
// Definitions live in detail/MemcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace memcore {} } }
using namespace openmfc::detail::memcore;
// Memory core (global new/delete exported by MFC)
//
// MSVC clients may resolve global operator new/delete from mfc140u.dll.
// If we leave these as auto-generated "void" stubs, MSVC code will treat the
// uninitialized RAX value as a pointer and crash immediately.
//
// Also implements: AfxLoadLibrary, AfxFreeLibrary, AfxFindResourceHandle,
//   AfxIsValidAddress, AfxIsValidString, AfxIsValidAtom

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Global new/delete
// =============================================================================

namespace openmfc { namespace detail { namespace memcore {
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
} } }

namespace openmfc { namespace detail { namespace memcore {
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);
} } }

namespace openmfc { namespace detail { namespace memcore {
extern "C" void* MS_ABI impl___U_YAPEAX_K_Z(std::size_t size);
} } }

namespace openmfc { namespace detail { namespace memcore {
extern "C" void MS_ABI impl___V_YAXPEAX_Z(void* ptr);
} } }

// Array new/delete use a slightly different mangling and stub naming in our generator.
namespace openmfc { namespace detail { namespace memcore {
extern "C" void* MS_ABI impl____U_YAPEAX_K_Z(std::size_t size);
} } }

namespace openmfc { namespace detail { namespace memcore {
extern "C" void MS_ABI impl____V_YAXPEAX_Z(void* ptr);
} } }

// =============================================================================
// Resource Library Management
// =============================================================================





// =============================================================================
// Validation Functions (Debug)
// =============================================================================






//=============================================================================
// Debug/Diagnostic Helpers (not DLL exports, debug-build only)
//=============================================================================

// AfxDebugBreak - Triggers a breakpoint in the debugger
namespace openmfc { namespace detail { namespace memcore {
extern "C" void impl__AfxDebugBreak();
} } }

// AfxCheckMemory - Validates heap integrity (debug builds)
// Returns TRUE if memory is valid, FALSE if corruption detected
namespace openmfc { namespace detail { namespace memcore {
extern "C" int impl__AfxCheckMemory();
} } }

// AfxDump - Global CDumpContext for debug output
// In real MFC this is a global CDumpContext object; here we provide
// a stub that outputs to stderr via OutputDebugString.
namespace openmfc { namespace detail { namespace memcore {
void DumpToDebugger(const wchar_t* psz);
} } }

namespace openmfc { namespace detail { namespace memcore {
extern "C" void impl__AfxDump__PB_W(const wchar_t* psz);
} } }

// AfxDumpStack - Prints stack trace to debug output
namespace openmfc { namespace detail { namespace memcore {
extern "C" void impl__AfxDumpStack(unsigned long dwFlags);
} } }
