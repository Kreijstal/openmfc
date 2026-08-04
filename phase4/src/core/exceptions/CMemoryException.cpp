// CMemoryException — OpenMFC implementation.
// Sources: appcore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ() {
    return &CMemoryException::classCMemoryException;
}
// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}
