#pragma once
// Shared internals of the former global_static_utils.cpp translation unit.
// Definitions live in detail/CMFCDynamicLayoutSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcdynamiclayout {} } }
using namespace openmfc::detail::cmfcdynamiclayout;
// Real implementations of layout-independent (no-`this`) static member-function
// exports.  Every algorithm here was reverse-engineered by differentially probing
// the real mfc140u.dll (via wine, calling the exports by ordinal) and verified to
// match bit-for-bit across an exhaustive input sweep.  See the tools/harvest probes.
//
// Only functions confirmed to reproduce the real DLL EXACTLY (0 mismatches over the
// documented input domain) are implemented here; anything that merely came "close"
// (off-by-one LSB rounding, GDI drawing, unknown gray-case quirks) is left as a weak
// stub deliberately.
//
// Coverage note: a Symbol marker comment sits immediately above each thunk so
// tools/gen_manifest.py counts it.
#include <windows.h>
#include <cmath>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// CDrawingManager color-space helpers
// ---------------------------------------------------------------------------

// Standard HSL "hue -> component" helper.  m1/m2 are the two "magic" numbers,
// h is the hue expressed as a fraction in the [0,1] turn.  Verified 0/1512
// mismatches against the real ?HuetoRGB@CDrawingManager@@SANNNN@Z.
namespace openmfc { namespace detail { namespace cmfcdynamiclayout {
double DM_HuetoRGB(double m1, double m2, double h);
} } }






// ---------------------------------------------------------------------------
// CMFCDynamicLayout::MoveSettings / SizeSettings factories.
//
// Both nested structs are exactly two ints { m_nXRatio, m_nYRatio } (8 bytes),
// confirmed by dumping the returned bytes from the real DLL.  They have a
// user-declared constructor, so per the MS x64 ABI they are returned via a hidden
// first pointer argument (which is also returned in RAX).
// ---------------------------------------------------------------------------
namespace openmfc { namespace detail { namespace cmfcdynamiclayout {
struct DynRatio { int x; int y; };
} } }








