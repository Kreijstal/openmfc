// CEdit — OpenMFC implementation.
// Sources: ctrlcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?Create@CEdit@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
asm(".globl \"?Create@CEdit@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\"\n"
    ".set \"?Create@CEdit@@QEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z\", impl__Create_CEdit__QEAAHKAEBUtagRECT__PEAVCWnd__I_Z\n");
// Symbol: ?GetRuntimeClass@CEdit@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CEdit__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetThisClass@CEdit@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CEdit__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}
