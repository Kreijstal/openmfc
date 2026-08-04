// CAnimationVariable — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CAnimationVariable@@QEAA@N@Z
extern "C" CAnimationVariable* MS_ABI impl___0CAnimationVariable__QEAA_N_Z(CAnimationVariable* pThis, double defaultValue) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].value = defaultValue;
    return pThis;
}
// Symbol: ??1CAnimationVariable@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationVariable__UEAA_XZ(CAnimationVariable* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState.erase(pThis);
}
// Symbol: ?Create@CAnimationVariable@@UEAAHPEAUIUIAnimationManager@@@Z
extern "C" int MS_ABI impl__Create_CAnimationVariable__UEAAHPEAUIUIAnimationManager___Z(CAnimationVariable* pThis, void* pAnimationManager) {
    (void)pAnimationManager;
    return pThis ? TRUE : FALSE;
}
// Symbol: ?CreateTransitions@CAnimationVariable@@QEAAHPEAUIUIAnimationTransitionLibrary@@PEAUIUIAnimationTransitionFactory@@@Z
extern "C" int MS_ABI impl__CreateTransitions_CAnimationVariable__QEAAHPEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
    CAnimationVariable* pThis, void* pTransitionLibrary, void* pTransitionFactory) {
    if (!pThis || !pTransitionLibrary || !pTransitionFactory) return FALSE;
    return TRUE;
}
// Symbol: ?AddTransition@CAnimationVariable@@QEAAXPEAVCBaseTransition@@@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(CAnimationVariable* pThis, void* pTransition) {
    if (!pThis || !pTransition) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].transitions.push_back(pTransition);
}
// Symbol: ?ClearTransitions@CAnimationVariable@@QEAAXH@Z
extern "C" void MS_ABI impl__ClearTransitions_CAnimationVariable__QEAAXH_Z(CAnimationVariable* pThis, int bAutodestroy) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].transitions.clear();
    g_animationVariableState[pThis].pendingAutodestroy = bAutodestroy;
}
// Symbol: ?EnableValueChangedEvent@CAnimationVariable@@QEAAXPEAVCAnimationController@@H@Z
extern "C" void MS_ABI impl__EnableValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
    CAnimationVariable* pThis, void* pController, int bEnable) {
    (void)pController;
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].valueChangedEventEnabled = (bEnable != 0);
}
// Symbol: ?EnableIntegerValueChangedEvent@CAnimationVariable@@QEAAXPEAVCAnimationController@@H@Z
extern "C" void MS_ABI impl__EnableIntegerValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(
    CAnimationVariable* pThis, void* pController, int bEnable) {
    (void)pController;
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].integerValueChangedEventEnabled = (bEnable != 0);
}
// Symbol: ?ApplyTransitions@CAnimationVariable@@QEAAXPEAVCAnimationController@@PEAUIUIAnimationStoryboard@@H@Z
extern "C" void MS_ABI impl__ApplyTransitions_CAnimationVariable__QEAAXPEAVCAnimationController__PEAUIUIAnimationStoryboard__H_Z(
    CAnimationVariable* pThis, void* pController, void* pStoryboard, int bAutodestroy) {
    (void)pController;
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    auto& state = g_animationVariableState[pThis];
    state.pendingAutodestroy = bAutodestroy;
    if (state.transitions.empty()) {
        state.value = state.value;
    }
    if (bAutodestroy) {
        state.transitions.clear();
    }
}
// Symbol: ?SetDefaultValue@CAnimationVariable@@QEAAXN@Z
extern "C" void MS_ABI impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(CAnimationVariable* pThis, double value) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    g_animationVariableState[pThis].value = value;
}
// Symbol: ?GetValue@CAnimationVariable@@QEAAJAEAN@Z
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(CAnimationVariable* pThis, double* pValue) {
    if (!pThis || !pValue) return E_POINTER;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    *pValue = g_animationVariableState[pThis].value;
    return S_OK;
}
// Symbol: ?GetValue@CAnimationVariable@@QEAAJAEAH@Z
extern "C" long MS_ABI impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(CAnimationVariable* pThis, int* pValue) {
    if (!pThis || !pValue) return E_POINTER;
    std::lock_guard<std::mutex> lock(g_wave2StateMutex);
    *pValue = (int)g_animationVariableState[pThis].value;
    return S_OK;
}
CAnimationVariable::CAnimationVariable(double defaultValue) {
    impl___0CAnimationVariable__QEAA_N_Z(this, defaultValue);
    memset(_animationvariable_padding, 0, sizeof(_animationvariable_padding));
}
CAnimationVariable::~CAnimationVariable() {
    impl___1CAnimationVariable__UEAA_XZ(this);
}
int CAnimationVariable::Create(void* pAnimationManager) {
    return impl__Create_CAnimationVariable__UEAAHPEAUIUIAnimationManager___Z(this, pAnimationManager);
}
int CAnimationVariable::CreateTransitions(void* pTransitionLibrary, void* pTransitionFactory) {
    return impl__CreateTransitions_CAnimationVariable__QEAAHPEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
        this, pTransitionLibrary, pTransitionFactory);
}
void CAnimationVariable::AddTransition(void* pTransition) {
    impl__AddTransition_CAnimationVariable__QEAAXPEAVCBaseTransition___Z(this, pTransition);
}
void CAnimationVariable::ClearTransitions(int bAutodestroy) {
    impl__ClearTransitions_CAnimationVariable__QEAAXH_Z(this, bAutodestroy);
}
void CAnimationVariable::EnableValueChangedEvent(void* pController, int bEnable) {
    impl__EnableValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(this, pController, bEnable);
}
void CAnimationVariable::EnableIntegerValueChangedEvent(void* pController, int bEnable) {
    impl__EnableIntegerValueChangedEvent_CAnimationVariable__QEAAXPEAVCAnimationController__H_Z(this, pController, bEnable);
}
void CAnimationVariable::ApplyTransitions(void* pController, void* pStoryboard, int bAutodestroy) {
    impl__ApplyTransitions_CAnimationVariable__QEAAXPEAVCAnimationController__PEAUIUIAnimationStoryboard__H_Z(
        this, pController, pStoryboard, bAutodestroy);
}
void CAnimationVariable::SetDefaultValue(double value) {
    impl__SetDefaultValue_CAnimationVariable__QEAAXN_Z(this, value);
}
long CAnimationVariable::GetValue(double& value) {
    return impl__GetValue_CAnimationVariable__QEAAJAEAN_Z(this, &value);
}
long CAnimationVariable::GetValue(int& value) {
    return impl__GetValue_CAnimationVariable__QEAAJAEAH_Z(this, &value);
}
