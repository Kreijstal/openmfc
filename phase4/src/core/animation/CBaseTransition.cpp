// CBaseTransition — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?AddToStoryboard@CBaseTransition@@QEAAHPEAUIUIAnimationStoryboard@@@Z
extern "C" int MS_ABI impl__AddToStoryboard_CBaseTransition__QEAAHPEAUIUIAnimationStoryboard___Z(
    void* pThis, void* pStoryboard) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCBaseTransitionState(pThis);
    ++state.m_addToStoryboardCalls;
    (void)state;
    return static_cast<int>(S_OK);
}
// Symbol: ?AddToStoryboardAtKeyframes@CBaseTransition@@QEAAHPEAUIUIAnimationStoryboard@@@Z
extern "C" int MS_ABI impl__AddToStoryboardAtKeyframes_CBaseTransition__QEAAHPEAUIUIAnimationStoryboard___Z(
    void* pThis, void* pStoryboard) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCBaseTransitionState(pThis);
    ++state.m_addToStoryboardAtKeyframesCalls;
    (void)state;
    return static_cast<int>(S_OK);
}
// Symbol: ?GetTransition@CBaseTransition@@QEAAPEAUIUIAnimationTransition@@PEAUIUIAnimationTransitionLibrary@@PEAUIUIAnimationTransitionFactory@@@Z
extern "C" void* MS_ABI impl__GetTransition_CBaseTransition__QEAAPEAUIUIAnimationTransition__PEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
    void* pThis, void* pLibrary, void* pFactory) {
    (void)pLibrary;
    (void)pFactory;
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCBaseTransitionState(pThis);
    if (state.m_transition == nullptr) {
        state.m_transition = pThis;
    }
    return state.m_transition;
}
