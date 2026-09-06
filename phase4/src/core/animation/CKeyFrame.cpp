// CKeyFrame — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CKeyFrame@@QEAA@PEAVCBaseKeyFrame@@N@Z
extern "C" void* MS_ABI impl___0CKeyFrame__QEAA_PEAVCBaseKeyFrame__N_Z(
    void* pThis, void* pKeyFrame, double duration) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCKeyFrameState(pThis);
    state.m_creator = pKeyFrame;
    state.m_duration = duration;
    state.m_hasBaseKeyFrame = true;
    return pThis;
}
// Symbol: ??0CKeyFrame@@QEAA@PEAVCBaseTransition@@@Z
extern "C" void* MS_ABI impl___0CKeyFrame__QEAA_PEAVCBaseTransition___Z(void* pThis, void* pTransition) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCKeyFrameState(pThis);
    state.m_creator = pTransition;
    state.m_hasTransition = true;
    return pThis;
}
// Symbol: ?AddToStoryboard@CKeyFrame@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboard_CKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int keyFrameID) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCKeyFrameState(pThis);
    ++state.m_addToStoryboardCalls;
    state.m_lastStoryboard = pStoryboard;
    state.m_lastKeyFrameID = keyFrameID;
    return static_cast<int>(S_OK);
}
// Symbol: ?AddToStoryboardAfterTransition@CKeyFrame@@QEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboardAfterTransition_CKeyFrame__QEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int keyFrameID) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCKeyFrameState(pThis);
    ++state.m_addToStoryboardAfterTransitionCalls;
    state.m_lastStoryboard = pStoryboard;
    state.m_lastKeyFrameID = keyFrameID;
    return static_cast<int>(S_OK);
}
// Symbol: ?AddToStoryboardAtOffset@CKeyFrame@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboardAtOffset_CKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int keyFrameID) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCKeyFrameState(pThis);
    ++state.m_addToStoryboardAtOffsetCalls;
    state.m_lastStoryboard = pStoryboard;
    state.m_lastKeyFrameID = keyFrameID;
    return static_cast<int>(S_OK);
}
