// CAnimationController — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ??0CAnimationController@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationController__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CAnimationController@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationController__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?IsAnimationInProgress@CAnimationController@@UEAAHXZ
extern "C" int MS_ABI impl__IsAnimationInProgress_CAnimationController__UEAAHXZ(void* /*pThis*/) {
    return 0;  // no animation system; always report no animation in progress
}
