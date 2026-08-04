#pragma once
// Shared internals of the former global_cmfcoutlookbarpanelist.cpp translation unit.
// Definitions live in detail/CMFCOutlookBarPaneListSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcoutlookbarpanelist {} } }
using namespace openmfc::detail::cmfcoutlookbarpanelist;
// OpenMFC real implementation of CMFCOutlookBarPaneList exports.
//
// CMFCOutlookBarPaneList is the internal toolbar-style pane that
// CMFCPropertySheet uses to render its Outlook-bar navigation buttons.
// Each button switches the active property page of the owning sheet.
//
// The full class is a CMFCOutlookBarPane (ultimately a CWnd), size(11120).
// The only member introduced by this leaf class is m_pParent, the owning
// CMFCPropertySheet*, which sits at the very end of the object.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif


// Layout-faithful view. Harvested size is 11120. The whole polymorphic
// base chain (CObject..CMFCOutlookBarPane) is opaque here; we only need the
// vfptr at offset 0 and this class's own trailing member m_pParent, which is
// the last pointer in the object (offset 11112 = 11120 - sizeof(void*)).
namespace openmfc { namespace detail { namespace cmfcoutlookbarpanelist {
struct S_Cmfcoutlookbarpanelist
{
    void*  vfptr;                          // 0: CObject vfptr
    char   _opaque[11120 - 8 - 8];         // 8..11111: base subobjects
    void*  m_pParent;                      // 11112: CMFCPropertySheet*
};
} } }




