// CAnimationGroup — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CAnimationControllerSupport.h"

// Symbol: ??0CAnimationGroup@@QEAA@PEAVCAnimationController@@I@Z
extern "C" void* MS_ABI impl___0CAnimationGroup__QEAA_PEAVCAnimationController__I_Z(
    void* pThis, void* /*pController*/, unsigned int unusedFlags) {
    (void)unusedFlags;
    return pThis;
}
// Symbol: ??1CAnimationGroup@@QEAA@XZ
extern "C" void MS_ABI impl___1CAnimationGroup__QEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddKeyframes@CAnimationGroup@@IEAAXPEAUIUIAnimationStoryboard@@H@Z
extern "C" void MS_ABI impl__AddKeyframes_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int /*bDeepAdd*/) {
    openmfc::detail::animationcontroller::RegisterStoryboard(pThis, pStoryboard);
}

// Symbol: ?AddTransitions@CAnimationGroup@@IEAAXPEAUIUIAnimationStoryboard@@H@Z
extern "C" void MS_ABI impl__AddTransitions_CAnimationGroup__IEAAXPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int /*bDeepAdd*/) {
    openmfc::detail::animationcontroller::RegisterStoryboard(pThis, pStoryboard);
}
