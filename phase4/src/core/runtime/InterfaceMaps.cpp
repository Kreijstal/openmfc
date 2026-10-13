// InterfaceMaps — OpenMFC implementation.
// Sources: global_interface_maps.cpp

#include "detail/InterfaceMapsSupport.h"

namespace openmfc { namespace detail { namespace interfacemaps {
const AFX_INTERFACEMAP_ENTRY g_ifaceEnd[] = { { nullptr, (size_t)-1 } };
const AFX_INTERFACEMAP g_imap_CCmdTarget = { nullptr, g_ifaceEnd };
}}}

// Symbol: ?GetThisInterfaceMap@CCmdTarget@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ() { return &g_imap_CCmdTarget; }
// Symbol: ?GetInterfaceMap@CCmdTarget@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CCmdTarget__MEBAPEBUAFX_INTERFACEMAP__XZ(const void* pThis) { (void)pThis; return &g_imap_CCmdTarget; }
