#pragma once
// Shared internals of the former global_cmfcrebarstate.cpp translation unit.
// Definitions live in detail/CMFCReBarStateSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcrebarstate {} } }
using namespace openmfc::detail::cmfcrebarstate;
// CMFCReBarState -- real OpenMFC implementation.
//
// A stateless helper class (size 1, no data members, no vtable) that persists
// the band layout of every ReBar (ReBarWindow32) child of a frame window.
//
//   SaveState / LoadState  -- public __stdcall entry points. They fetch the
//       frame's HWND (CWnd::m_hWnd @ +64) and EnumChildWindows() the frame,
//       invoking a per-window callback that detects rebar controls.
//   SaveRebarStateProc / LoadRebarStateProc -- the private EnumChildWindows
//       callbacks. For each rebar they read/write every band's style, sizes,
//       ideal width, child-size and command id, so that a Save followed by a
//       Load faithfully restores band ordering and widths.
//
// The real MFC routes persistence through CWinApp profile calls
// (Write/GetProfileBinary); here we persist the identical band data through the
// Win32 registry under HKCU\Software\OpenMFC\ReBarState\<key>, which yields the
// same observable Save->Load round-trip behavior.
//
// Layout (cl.exe /d1reportSingleClassLayout): class CMFCReBarState size(1).

#include <windows.h>
#include <commctrl.h>
#include <cstddef>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Empty class -> 1 byte. No members, no vfptr.
namespace openmfc { namespace detail { namespace cmfcrebarstate {
struct S_Cmfcrebarstate { char dummy; };
} } }

// CWnd::m_hWnd sits at offset 64 in the MFC layout.
namespace openmfc { namespace detail { namespace cmfcrebarstate {
static const size_t kCWndHWndOffset = 64;
} } }

// Per-band record stored in the registry blob.
namespace openmfc { namespace detail { namespace cmfcrebarstate {
struct BandRec {
    UINT fStyle;
    UINT cx;
    UINT cxIdeal;
    UINT cxMinChild;
    UINT cyMinChild;
    UINT wID;
};
} } }

// Context threaded through EnumChildWindows.
namespace openmfc { namespace detail { namespace cmfcrebarstate {
struct RebarCtx {
    const wchar_t* pszKey;  // profile section base (CString buffer)
    int            nIndex;  // running index of rebars encountered
};
} } }

// Build HKCU subkey path:  Software\OpenMFC\ReBarState\<key>
namespace openmfc { namespace detail { namespace cmfcrebarstate {
void BuildKeyPath(const wchar_t* pszKey, wchar_t* out, size_t cch);
} } }

namespace openmfc { namespace detail { namespace cmfcrebarstate {
bool IsRebar(HWND hwnd);
} } }


// Forward decls (callbacks referenced by the entry points).
extern "C" int MS_ABI
impl__SaveRebarStateProc_CMFCReBarState__CAHPEAUHWND_____J_Z(HWND hwnd, LPARAM lParam);
extern "C" int MS_ABI
impl__LoadRebarStateProc_CMFCReBarState__CAHPEAUHWND_____J_Z(HWND hwnd, LPARAM lParam);




