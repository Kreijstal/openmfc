#pragma once
// Shared internals of the former global_core-windows_impl.cpp translation unit.
// Definitions live in detail/CWndSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cwnd {} } }
using namespace openmfc::detail::cwnd;
// CWnd core-window exports (wave 1: core-windows)
//
// Implements the remaining CWnd exports in the Windows/MSVC x64 ABI. Every
// function is a plain extern "C" ms_abi function with `this` passed as an
// explicit first parameter. Semantics are transcribed from the retail
// mfc140u.dll disassembly (see /tmp/openmfc_remaining).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <map>
#include <mutex>
#include <utility>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Exported stubs used internally (avoid re-implementing)
// ---------------------------------------------------------------------------
extern "C" int MS_ABI impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(HWND__* p0, unsigned long p1, unsigned long p2, unsigned int p3);
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__SAHPEAUHWND____KKI_Z(HWND__* p0, unsigned long p1, unsigned long p2, unsigned int p3);

// ---------------------------------------------------------------------------
// File-local per-window state
// ---------------------------------------------------------------------------
// Sibling implementations (wincore.cpp) keep per-window state in
// mutex-guarded std::map containers; follow the same pattern here for the two
// values that have no declared member in the (simplified) CWnd layout:
//   - SetProxy  stores the accessibility proxy (retail offset 0x58)
//   - EndModalLoop stores the modal result (retail offset 0xB8, m_nModalResult)
namespace openmfc { namespace detail { namespace cwnd {
extern std::mutex g_coreWndStateMutex;
} } }
namespace openmfc { namespace detail { namespace cwnd {
extern std::map<const CWnd*, void*> g_coreWndProxies;
} } }
namespace openmfc { namespace detail { namespace cwnd {
extern std::map<const CWnd*, int> g_coreWndModalResults;
} } }

// ===========================================================================
// CWnd
// ===========================================================================






























