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
// ?Animate@CAnimationGroup@@QEAAHPEAUIUIAnimationManager@@PEAUIUIAnimationTimer@@H@Z
extern "C" int MS_ABI impl__Animate_CAnimationGroup__QEAAHPEAUIUIAnimationManager__PEAUIUIAnimationTimer__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// ?ApplyTransitions@CAnimationGroup@@QEAAXXZ
extern "C" void MS_ABI impl__ApplyTransitions_CAnimationGroup__QEAAXXZ() {}

// ?CreateTransitions@CAnimationGroup@@IEAAHXZ
extern "C" int MS_ABI impl__CreateTransitions_CAnimationGroup__IEAAHXZ() {
    return 0;
}

// ?FindAnimationObject@CAnimationGroup@@QEAAPEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@@Z
extern "C" void* MS_ABI impl__FindAnimationObject_CAnimationGroup__QEAAPEAVCAnimationBaseObject__PEAUIUIAnimationVariable___Z(void* /*struct*/* p0) {
    return nullptr;
}

// ?RemoveKeyframes@CAnimationGroup@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveKeyframes_CAnimationGroup__QEAAXXZ() {}

// ?RemoveTransitions@CAnimationGroup@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveTransitions_CAnimationGroup__QEAAXXZ() {}

// ?Schedule@CAnimationGroup@@QEAAHPEAUIUIAnimationTimer@@N@Z
extern "C" int MS_ABI impl__Schedule_CAnimationGroup__QEAAHPEAUIUIAnimationTimer__N_Z(void* /*struct*/* p0, double p1) {
    return 0;
}

// ?SetAutodestroyTransitions@CAnimationGroup@@QEAAXH@Z
extern "C" void MS_ABI impl__SetAutodestroyTransitions_CAnimationGroup__QEAAXH_Z(int p0) {}

