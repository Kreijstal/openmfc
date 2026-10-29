// CEdit — OpenMFC implementation.
// Sources: ctrlcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Retail mfc140u body at entry RVA 0x5f250 loads the CEdit runtime descriptor
// address and returns it; it does not inspect the instance pointer.
// Symbol: ?GetRuntimeClass@CEdit@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CEdit__UEBAPEAUCRuntimeClass__XZ(
    const CEdit*) {
    return &CEdit::classCEdit;
}
// The retail mfc140.dll (ANSI twin) body at RVA 0x292600 returns the CEdit
// runtime descriptor address. CEdit::classCEdit is declared by DECLARE_DYNAMIC
// and defined by IMPLEMENT_DYNAMIC in CtrlcoreSupport.cpp.
// Symbol: ?GetThisClass@CEdit@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CEdit__SAPEAUCRuntimeClass__XZ() {
    return &CEdit::classCEdit;
}
