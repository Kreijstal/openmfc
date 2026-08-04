#pragma once
// Shared internals of the former global_cmfctabbutton.cpp translation unit.
// Definitions live in detail/CMFCTabButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctabbutton {} } }
using namespace openmfc::detail::cmfctabbutton;
// OpenMFC: real implementation of CMFCTabButton exported virtual overrides.
//
// CMFCTabButton (afxtabctrl.h) derives from CMFCButton and exists only as the clickable
// tab in a CMFCTabCtrl. It overrides two protected drawing virtuals so the button does
// NOT paint its own CMFCButton chrome -- the owning tab control is responsible for the
// tab's background and border. In real mfc140u both overrides have deliberately empty
// bodies (they exist purely to shadow the CMFCButton implementations). Reproducing that
// exact suppression behavior here is the faithful implementation, not a stub.
//
// Only two symbols are exported for this class (OnFillBackground, OnDrawBorder); neither
// is a constructor, so no vptr installation is performed here. The class layout is
// mirrored purely to anchor the static_asserts.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view (harvested size(2848); CObject vfptr@0, CCmdTarget m_dwRef@8).
// The two overrides below are pure no-ops, so only the head fields need be modeled.
namespace openmfc { namespace detail { namespace cmfctabbutton {
struct S_Cmfctabbutton {
    void* vfptr;          // 0: CObject vtable pointer
    unsigned long m_dwRef; // 8: CCmdTarget reference count
    char _rest[2848 - 12]; // pad out to the full harvested object size
};
} } }




