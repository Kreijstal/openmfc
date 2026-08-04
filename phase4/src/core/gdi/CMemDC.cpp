// CMemDC — OpenMFC implementation.
// Sources: global_cmemdc.cpp

#include "detail/CMemDCSupport.h"

// CMemDC::m_bUseMemoryDC — global gate (defaults TRUE) letting an app force straight-to-DC
// drawing by clearing it before constructing a CMemDC.  Exported as data.
// Symbol: ?m_bUseMemoryDC@CMemDC@@2HA
extern "C" int impl__m_bUseMemoryDC_CMemDC__2HA = TRUE;

// CMemDC's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cmemdc {
extern void* const g_CMemDC_vtbl[1] = {
    reinterpret_cast<void*>(&vdtor_Cmemdc)
};
} } }
