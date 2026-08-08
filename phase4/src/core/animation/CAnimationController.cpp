// CAnimationController — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

namespace {
struct AnimationControllerGroupToken {
    unsigned int id = 0;
};

struct AnimationControllerObjectToken {};

struct AnimationControllerKeyframeStoryboardToken {};

struct AnimationControllerUIAnimationManager {};
struct AnimationControllerUIAnimationTimer {};
struct AnimationControllerUIAnimationTransitionFactory {};
struct AnimationControllerUIAnimationTransitionLibrary {};

struct AnimationControllerKeyframeToken {
    void* creator = nullptr;
};

struct CAnimationControllerObjectImpl : CObject {};

std::mutex g_animationControllerStateMutex;
std::uint32_t g_animationControllerNextGroupId = 1;
std::unordered_set<unsigned int> g_animationControllerGroupIds;
std::unordered_map<void*, unsigned int> g_animationControllerGroupByHandle;
std::unordered_map<unsigned int, AnimationControllerGroupToken*> g_animationControllerGroupById;
std::unordered_map<void*, void*> g_animationControllerGroupByObject;
std::unordered_map<void*, void*> g_animationControllerGroupByStoryboard;
std::unordered_map<void*, AnimationControllerKeyframeToken> g_animationControllerKeyframes;
std::unordered_map<unsigned int, double> g_animationControllerGroupScheduleTimes;
AnimationControllerKeyframeStoryboardToken g_animationControllerKeyframeStoryboardStart;
AnimationControllerUIAnimationManager g_animationManager;
AnimationControllerUIAnimationTimer g_animationTimer;
AnimationControllerUIAnimationTransitionFactory g_transitionFactory;
AnimationControllerUIAnimationTransitionLibrary g_transitionLibrary;

unsigned int AllocateAnimationControllerGroupId() {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    const unsigned int id = g_animationControllerNextGroupId++;
    g_animationControllerGroupIds.insert(id);
    return id;
}

void RegisterAnimationControllerGroup(AnimationControllerGroupToken* token) {
    if (!token) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupByHandle[token] = token->id;
    g_animationControllerGroupById[token->id] = token;
}

void UnregisterAnimationControllerGroup(AnimationControllerGroupToken* token) {
    if (!token) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto handleIt = g_animationControllerGroupByHandle.find(token);
    if (handleIt == g_animationControllerGroupByHandle.end()) {
        return;
    }
    const unsigned int groupId = handleIt->second;
    g_animationControllerGroupById.erase(groupId);
    g_animationControllerGroupIds.erase(groupId);
    g_animationControllerGroupByHandle.erase(handleIt);
    g_animationControllerGroupScheduleTimes.erase(groupId);

    for (auto it = g_animationControllerGroupByObject.begin();
         it != g_animationControllerGroupByObject.end();) {
        if (it->second == token) {
            it = g_animationControllerGroupByObject.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = g_animationControllerGroupByStoryboard.begin();
         it != g_animationControllerGroupByStoryboard.end();) {
        if (it->second == token) {
            it = g_animationControllerGroupByStoryboard.erase(it);
        } else {
            ++it;
        }
    }
}

bool IsValidAnimationControllerGroup(unsigned int groupId) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    return groupId != 0 && g_animationControllerGroupIds.count(groupId) != 0;
}

void* FindAnimationControllerGroupById(unsigned int groupId) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto it = g_animationControllerGroupById.find(groupId);
    return it == g_animationControllerGroupById.end() ? nullptr : it->second;
}

void RegisterAnimationControllerObject(void* pAnimationObject, void* pGroup) {
    if (!pAnimationObject || !pGroup) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupByObject[pAnimationObject] = pGroup;
}

void UnregisterKeyframesForGroupId(unsigned int groupId) {
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    for (auto it = g_animationControllerKeyframes.begin(); it != g_animationControllerKeyframes.end();) {
        if (reinterpret_cast<uintptr_t>(it->second.creator) == groupId) {
            it = g_animationControllerKeyframes.erase(it);
        } else {
            ++it;
        }
    }
}

void RemoveAnimationControllerGroup(AnimationControllerGroupToken* token) {
    if (!token) {
        return;
    }
    UnregisterAnimationControllerGroup(token);
    UnregisterKeyframesForGroupId(token->id);
}

void RemoveAnimationControllerGroupById(unsigned int groupId) {
    if (groupId == 0) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
        auto it = g_animationControllerGroupById.find(groupId);
        if (it == g_animationControllerGroupById.end()) {
            return;
        }
        auto* token = it->second;
        if (token != nullptr) {
            g_animationControllerGroupIds.erase(groupId);
            g_animationControllerGroupById.erase(it);
            g_animationControllerGroupByHandle.erase(token);
            g_animationControllerGroupScheduleTimes.erase(groupId);
            for (auto objectIt = g_animationControllerGroupByObject.begin();
                 objectIt != g_animationControllerGroupByObject.end();) {
                if (objectIt->second == token) {
                    objectIt = g_animationControllerGroupByObject.erase(objectIt);
                } else {
                    ++objectIt;
                }
            }
            for (auto storyboardIt = g_animationControllerGroupByStoryboard.begin();
                 storyboardIt != g_animationControllerGroupByStoryboard.end();) {
                if (storyboardIt->second == token) {
                    storyboardIt = g_animationControllerGroupByStoryboard.erase(storyboardIt);
                } else {
                    ++storyboardIt;
                }
            }
        }
    }
    UnregisterKeyframesForGroupId(groupId);
}

void RegisterAnimationControllerKeyframe(void* pKeyframe, void* creator) {
    if (!pKeyframe) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerKeyframes[pKeyframe] = AnimationControllerKeyframeToken{creator};
}
} // namespace

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
    return 0;
}

// Symbol: ?AddAnimationObject@CAnimationController@@QEAAPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@@Z
extern "C" void* MS_ABI impl__AddAnimationObject_CAnimationController__QEAAPEAVCAnimationGroup__PEAVCAnimationBaseObject___Z(void* pAnimationObject) {
    if (pAnimationObject == nullptr) {
        return nullptr;
    }
    auto* pGroup = new (std::nothrow) AnimationControllerGroupToken();
    if (pGroup == nullptr) {
        return nullptr;
    }
    pGroup->id = AllocateAnimationControllerGroupId();
    RegisterAnimationControllerGroup(pGroup);
    RegisterAnimationControllerObject(pAnimationObject, pGroup);
    return pGroup;
}

// Symbol: ?AddKeyframeToGroup@CAnimationController@@QEAAHIPEAVCBaseKeyFrame@@@Z
extern "C" int MS_ABI impl__AddKeyframeToGroup_CAnimationController__QEAAHIPEAVCBaseKeyFrame___Z(unsigned int groupId, void* pKeyframe) {
    if (groupId == 0 || pKeyframe == nullptr) {
        return FALSE;
    }
    if (!IsValidAnimationControllerGroup(groupId)) {
        return FALSE;
    }
    RegisterAnimationControllerKeyframe(pKeyframe,
        reinterpret_cast<void*>(static_cast<uintptr_t>(groupId)));
    return TRUE;
}

// Symbol: ?AnimateGroup@CAnimationController@@QEAAHIH@Z
extern "C" int MS_ABI impl__AnimateGroup_CAnimationController__QEAAHIH_Z(unsigned int groupId, int /*bAnimateNow*/) {
    if (!IsValidAnimationControllerGroup(groupId)) {
        return FALSE;
    }
    return TRUE;
}

// Symbol: ?CleanUpGroup@CAnimationController@@IEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__CleanUpGroup_CAnimationController__IEAAXPEAVCAnimationGroup___Z(void* pGroup) {
    auto* pToken = static_cast<AnimationControllerGroupToken*>(pGroup);
    RemoveAnimationControllerGroup(pToken);
    delete pToken;
}

// Symbol: ?CleanUpGroup@CAnimationController@@QEAAXI@Z
extern "C" void MS_ABI impl__CleanUpGroup_CAnimationController__QEAAXI_Z(unsigned int groupId) {
    RemoveAnimationControllerGroupById(groupId);
}

// Symbol: ?CreateKeyframe@CAnimationController@@QEAAPEAVCKeyFrame@@IPEAVCBaseKeyFrame@@N@Z
extern "C" void* MS_ABI impl__CreateKeyframe_CAnimationController__QEAAPEAVCKeyFrame__IPEAVCBaseKeyFrame__N_Z(
    unsigned int groupId, void* pBaseKeyframe, double /*time*/) {
    if (!pBaseKeyframe || !IsValidAnimationControllerGroup(groupId)) {
        return nullptr;
    }
    auto* keyframe = new (std::nothrow) AnimationControllerKeyframeToken();
    if (keyframe == nullptr) {
        return nullptr;
    }
    RegisterAnimationControllerKeyframe(keyframe,
        reinterpret_cast<void*>(static_cast<uintptr_t>(groupId)));
    return keyframe;
}

// Symbol: ?CreateKeyframe@CAnimationController@@QEAAPEAVCKeyFrame@@IPEAVCBaseTransition@@@Z
extern "C" void* MS_ABI impl__CreateKeyframe_CAnimationController__QEAAPEAVCKeyFrame__IPEAVCBaseTransition___Z(void* pTransition) {
    if (!pTransition) {
        return nullptr;
    }
    auto* keyframe = new (std::nothrow) AnimationControllerKeyframeToken();
    if (keyframe == nullptr) {
        return nullptr;
    }
    RegisterAnimationControllerKeyframe(keyframe, pTransition);
    return keyframe;
}

// Symbol: ?CreateObject@CAnimationController@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CAnimationController__SAPEAVCObject__XZ() {
    return new (std::nothrow) CAnimationControllerObjectImpl();
}

// Symbol: ?EnableAnimationManagerEvent@CAnimationController@@UEAAHH@Z
extern "C" int MS_ABI impl__EnableAnimationManagerEvent_CAnimationController__UEAAHH_Z(int /*eventId*/, int /*enable*/) {
    return TRUE;
}

// Symbol: ?EnableAnimationTimerEventHandler@CAnimationController@@UEAAHHW4__MIDL___MIDL_itf_UIAnimation_0000_0012_0001@@@Z
extern "C" int MS_ABI impl__EnableAnimationTimerEventHandler_CAnimationController__UEAAHHW4__MIDL___MIDL_itf_UIAnimation_0000_0012_0001___Z(
    int /*handler*/, int /*enum*/ /*priority*/,
    int /*fAllowDelay*/, int /*nTimerId*/, float /*rate*/, unsigned int /*arg*/, char /*reserved*/,
    void* /*pTimer*/, int /*nFrames*/, int /*id*/,
    int /*fPriority*/, float /*duration*/, unsigned int /*arg2*/, char /*reserved2*/, void* /*pVariable*/,
    int /*nGroup*/, void* /*pCallback*/, void* /*pContext*/, void* /*pStatus*/, int /*event*/,
    void* /*pStatusCallback*/) {
    return TRUE;
}

// Symbol: ?EnablePriorityComparisonHandler@CAnimationController@@UEAAH_K@Z
extern "C" int MS_ABI impl__EnablePriorityComparisonHandler_CAnimationController__UEAAH_K_Z(
    unsigned long long /*priority*/) {
    return TRUE;
}

// Symbol: ?EnableStoryboardEventHandler@CAnimationController@@UEAAHIH@Z
extern "C" int MS_ABI impl__EnableStoryboardEventHandler_CAnimationController__UEAAHIH_Z(
    int /*handler*/, int /*enable*/) {
    return TRUE;
}

// Symbol: ?FindAnimationGroup@CAnimationController@@QEAAPEAVCAnimationGroup@@I@Z
extern "C" void* MS_ABI impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__I_Z(
    unsigned int groupId) {
    return FindAnimationControllerGroupById(groupId);
}

// Symbol: ?FindAnimationGroup@CAnimationController@@QEAAPEAVCAnimationGroup@@PEAUIUIAnimationStoryboard@@@Z
extern "C" void* MS_ABI impl__FindAnimationGroup_CAnimationController__QEAAPEAVCAnimationGroup__PEAUIUIAnimationStoryboard___Z(
    void* pStoryboard) {
    if (pStoryboard == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    auto it = g_animationControllerGroupByStoryboard.find(pStoryboard);
    return it == g_animationControllerGroupByStoryboard.end() ? nullptr : it->second;
}

// Symbol: ?FindAnimationObject@CAnimationController@@QEAAHPEAUIUIAnimationVariable@@PEAPEAVCAnimationBaseObject@@PEAPEAVCAnimationGroup@@@Z
extern "C" int MS_ABI impl__FindAnimationObject_CAnimationController__QEAAHPEAUIUIAnimationVariable__PEAPEAVCAnimationBaseObject__PEAPEAVCAnimationGroup___Z(
    void* /*pAnimationVariable*/, void** pAnimationObject, void** pAnimationGroup) {
    if (pAnimationObject == nullptr || pAnimationGroup == nullptr) {
        return E_POINTER;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    if (g_animationControllerGroupByObject.empty()) {
        return FALSE;
    }
    auto* group = g_animationControllerGroupByObject.begin()->second;
    *pAnimationObject = g_animationControllerGroupByObject.begin()->first;
    *pAnimationGroup = group;
    return TRUE;
}

// Symbol: ?GetKeyframeStoryboardStart@CAnimationController@@SAPEAVCBaseKeyFrame@@XZ
extern "C" void* MS_ABI impl__GetKeyframeStoryboardStart_CAnimationController__SAPEAVCBaseKeyFrame__XZ() {
    return &g_animationControllerKeyframeStoryboardStart;
}

// Symbol: ?GetUIAnimationManager@CAnimationController@@QEAAPEAUIUIAnimationManager@@XZ
extern "C" void* MS_ABI impl__GetUIAnimationManager_CAnimationController__QEAAPEAUIUIAnimationManager__XZ() {
    return &g_animationManager;
}

// Symbol: ?GetUIAnimationTimer@CAnimationController@@QEAAPEAUIUIAnimationTimer@@XZ
extern "C" void* MS_ABI impl__GetUIAnimationTimer_CAnimationController__QEAAPEAUIUIAnimationTimer__XZ() {
    return &g_animationTimer;
}

// Symbol: ?GetUITransitionFactory@CAnimationController@@QEAAPEAUIUIAnimationTransitionFactory@@XZ
extern "C" void* MS_ABI impl__GetUITransitionFactory_CAnimationController__QEAAPEAUIUIAnimationTransitionFactory__XZ() {
    return &g_transitionFactory;
}

// Symbol: ?GetUITransitionLibrary@CAnimationController@@QEAAPEAUIUIAnimationTransitionLibrary@@XZ
extern "C" void* MS_ABI impl__GetUITransitionLibrary_CAnimationController__QEAAPEAUIUIAnimationTransitionLibrary__XZ() {
    return &g_transitionLibrary;
}

// Symbol: ?OnAfterSchedule@CAnimationController@@MEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__OnAfterSchedule_CAnimationController__MEAAXPEAVCAnimationGroup___Z(void* /*pAnimationGroup*/) {}

// Symbol: ?OnAnimationIntegerValueChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@HH@Z
extern "C" void MS_ABI impl__OnAnimationIntegerValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__HH_Z(
    void* /*pAnimationGroup*/, void* /*pAnimationObject*/,
    void* /*pAnimationVariable*/, int /*oldValue*/, int /*newValue*/) {}

// Symbol: ?OnAnimationManagerStatusChanged@CAnimationController@@UEAAXW4__MIDL___MIDL_itf_UIAnimation_0000_0000_0002@@0@Z
extern "C" void MS_ABI impl__OnAnimationManagerStatusChanged_CAnimationController__UEAAXW4__MIDL___MIDL_itf_UIAnimation_0000_0000_0002__0_Z(
    int /*status*/) {}

// Symbol: ?OnAnimationTimerPostUpdate@CAnimationController@@UEAAXXZ
extern "C" void MS_ABI impl__OnAnimationTimerPostUpdate_CAnimationController__UEAAXXZ() {}

// Symbol: ?OnAnimationTimerPreUpdate@CAnimationController@@UEAAXXZ
extern "C" void MS_ABI impl__OnAnimationTimerPreUpdate_CAnimationController__UEAAXXZ() {}

// Symbol: ?OnAnimationTimerRenderingTooSlow@CAnimationController@@UEAAXI@Z
extern "C" void MS_ABI impl__OnAnimationTimerRenderingTooSlow_CAnimationController__UEAAXI_Z(
    unsigned int /*nFrames*/) {}

// Symbol: ?OnAnimationValueChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@NN@Z
extern "C" void MS_ABI impl__OnAnimationValueChanged_CAnimationController__UEAAXPEAVCAnimationGroup__PEAVCAnimationBaseObject__PEAUIUIAnimationVariable__NN_Z(
    void* /*pAnimationGroup*/, void* /*pAnimationObject*/,
    void* /*pAnimationVariable*/, double /*oldValue*/, double /*newValue*/) {}

// Symbol: ?OnBeforeAnimationStart@CAnimationController@@UEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__OnBeforeAnimationStart_CAnimationController__UEAAXPEAVCAnimationGroup___Z(
    void* /*pAnimationGroup*/) {}

// Symbol: ?OnHasPriorityCancel@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityCancel_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pAnimationGroup*/, int /*priority*/) {
    return TRUE;
}

// Symbol: ?OnHasPriorityCompress@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityCompress_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pAnimationGroup*/, int /*priority*/) {
    return TRUE;
}

// Symbol: ?OnHasPriorityConclude@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityConclude_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pAnimationGroup*/, int /*priority*/) {
    return TRUE;
}

// Symbol: ?OnHasPriorityTrim@CAnimationController@@UEAAHPEAVCAnimationGroup@@0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001@@@Z
extern "C" int MS_ABI impl__OnHasPriorityTrim_CAnimationController__UEAAHPEAVCAnimationGroup__0W4__MIDL___MIDL_itf_UIAnimation_0000_0008_0001___Z(
    void* /*pAnimationGroup*/, int /*priority*/) {
    return TRUE;
}

// Symbol: ?OnStoryboardStatusChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@W4__MIDL___MIDL_itf_UIAnimation_0000_0002_0001@@1@Z
extern "C" void MS_ABI impl__OnStoryboardStatusChanged_CAnimationController__UEAAXPEAVCAnimationGroup__W4__MIDL___MIDL_itf_UIAnimation_0000_0002_0001__1_Z(
    void* /*pAnimationGroup*/, int /*status*/) {}

// Symbol: ?OnStoryboardUpdated@CAnimationController@@UEAAXPEAVCAnimationGroup@@@Z
extern "C" void MS_ABI impl__OnStoryboardUpdated_CAnimationController__UEAAXPEAVCAnimationGroup___Z(
    void* /*pAnimationGroup*/) {}

// Symbol: ?RemoveAllAnimationGroups@CAnimationController@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllAnimationGroups_CAnimationController__QEAAXXZ() {
    std::vector<AnimationControllerGroupToken*> groupsToDelete;
    {
        std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
        groupsToDelete.reserve(g_animationControllerGroupByHandle.size());
        for (auto& entry : g_animationControllerGroupByHandle) {
            auto* pGroup = static_cast<AnimationControllerGroupToken*>(entry.first);
            if (pGroup != nullptr) {
                groupsToDelete.push_back(pGroup);
            }
        }
        g_animationControllerGroupIds.clear();
        g_animationControllerGroupByHandle.clear();
        g_animationControllerGroupById.clear();
        g_animationControllerGroupByObject.clear();
        g_animationControllerGroupByStoryboard.clear();
        g_animationControllerGroupScheduleTimes.clear();
        g_animationControllerKeyframes.clear();
    }
    for (auto* pGroup : groupsToDelete) {
        delete pGroup;
    }
}

// Symbol: ?RemoveAnimationGroup@CAnimationController@@QEAAXI@Z
extern "C" void MS_ABI impl__RemoveAnimationGroup_CAnimationController__QEAAXI_Z(unsigned int groupId) {
    if (groupId == 0) {
        return;
    }
    void* pGroupHandle = FindAnimationControllerGroupById(groupId);
    if (pGroupHandle == nullptr) {
        return;
    }
    RemoveAnimationControllerGroupById(groupId);
    delete static_cast<AnimationControllerGroupToken*>(pGroupHandle);
}

// Symbol: ?RemoveAnimationObject@CAnimationController@@QEAAXPEAVCAnimationBaseObject@@H@Z
extern "C" void MS_ABI impl__RemoveAnimationObject_CAnimationController__QEAAXPEAVCAnimationBaseObject__H_Z(
    void* pAnimationObject, int /*bTransition*/) {
    if (pAnimationObject == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupByObject.erase(pAnimationObject);
}

// Symbol: ?RemoveTransitions@CAnimationController@@QEAAXI@Z
extern "C" void MS_ABI impl__RemoveTransitions_CAnimationController__QEAAXI_Z(unsigned int groupId) {
    if (!IsValidAnimationControllerGroup(groupId)) {
        return;
    }
    UnregisterKeyframesForGroupId(groupId);
}

// Symbol: ?ScheduleGroup@CAnimationController@@QEAAHIN@Z
extern "C" int MS_ABI impl__ScheduleGroup_CAnimationController__QEAAHIN_Z(unsigned int groupId, double time) {
    if (!IsValidAnimationControllerGroup(groupId)) {
        return FALSE;
    }
    std::lock_guard<std::mutex> lock(g_animationControllerStateMutex);
    g_animationControllerGroupScheduleTimes[groupId] = time;
    return TRUE;
}

// Symbol: ?UpdateAnimationManager@CAnimationController@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateAnimationManager_CAnimationController__UEAAXXZ() {}
