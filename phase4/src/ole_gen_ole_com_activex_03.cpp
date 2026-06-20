// OpenMFC export shard: ole_com_activex 03
//
// This shard implements only the MFC RTTI accessors (GetThisClass /
// GetRuntimeClass) for classes whose full definition with DECLARE_DYNAMIC is
// available in the repo headers. These are unambiguous:
//   - GetThisClass() is a static accessor returning the class's CRuntimeClass.
//   - GetRuntimeClass() is the virtual override returning the same.
// Everything else in this shard (control-site/container/in-place-frame methods,
// data-source/object internals, currency parsing, etc.) depends on internal
// MFC state or memory layout the headers do not model faithfully, so those
// weak stubs are intentionally left in place.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// COleDialog
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@COleDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDialog__SAPEAUCRuntimeClass__XZ() {
    return COleDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleDialog* pThis) {
    if (!pThis) return COleDialog::GetThisClass();
    return pThis->GetRuntimeClass();
}

// ---------------------------------------------------------------------------
// COleConvertDialog
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@COleConvertDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleConvertDialog__SAPEAUCRuntimeClass__XZ() {
    return COleConvertDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleConvertDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleConvertDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleConvertDialog* pThis) {
    if (!pThis) return COleConvertDialog::GetThisClass();
    return pThis->GetRuntimeClass();
}
