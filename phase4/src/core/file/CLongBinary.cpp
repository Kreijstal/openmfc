// CLongBinary — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/ClongbinaryRttiSupport.h"
#include "detail/CRuntimeClassSupport.h"

// Symbol: ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    InitializeClasses();
    return &classCLongBinary;
}
// Symbol: ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ() {
    InitializeClasses();
    return &classCLongBinary;
}
