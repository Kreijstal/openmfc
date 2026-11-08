// RuntimeClasses — OpenMFC implementation.
// Sources: global_oleresidual_rtti.cpp, ole_clientitem_dialogs_rtti.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleControlModuleSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// This shard implements MFC RTTI accessors (GetThisClass / GetRuntimeClass)
// for OLE/COM/ActiveX classes with known static CRuntimeClass descriptors.
// GetThisClass returns the static descriptor for the class, and GetRuntimeClass
// returns the same virtual value without dereferencing the this pointer.

//-----------------------------------------------------------------------------
// CCmdTarget
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// COleBusyDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleChangeIconDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleChangeSourceDialog
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// COleClientItem
//-----------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CCmdTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(
        const CCmdTarget* pThis) {
    if (!pThis) return CCmdTarget::GetThisClass();
    return CCmdTarget::GetThisClass();
}
