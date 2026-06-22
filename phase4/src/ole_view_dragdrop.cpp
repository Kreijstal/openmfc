// OpenMFC - ole_com_activex shard 06/06
// Real implementations overriding weak stubs for a conservative subset of
// COleUpdateDialog (RTTI) and CView (OLE drag/drop default behaviors).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?GetThisClass@COleUpdateDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_COleUpdateDialog__SAPEAUCRuntimeClass__XZ() {
    return COleUpdateDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleUpdateDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_COleUpdateDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleUpdateDialog* pThis) {
    (void)pThis;
    return COleUpdateDialog::GetThisClass();
}
