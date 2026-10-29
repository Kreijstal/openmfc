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
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(
    const CMemoryException*) {
    // Retail RVA 0x2275c0 (mfc140u) forms the static runtime-class descriptor's address and returns it.
    return &CMemoryException::classCMemoryException;
}
