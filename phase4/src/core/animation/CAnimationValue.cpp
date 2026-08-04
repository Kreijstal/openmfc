// CAnimationValue — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CAnimationValue@@QEAA@NII_K@Z
extern "C" void* MS_ABI impl___0CAnimationValue__QEAA_NII_K_Z(
    void* pThis, double unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused1;
    (void)unused2;
    if (pThis) {
        std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
        g_animationValueStates[pThis].m_value = unused0;
    }
    return pThis;
}
// Symbol: ??0CAnimationValue@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationValue__QEAA_XZ(void* pThis) {
    if (pThis) {
        std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
        g_animationValueStates[pThis].m_value = 0.0;
    }
    return pThis;
}
// Symbol: ?AddTransition@CAnimationValue@@QEAAXPEAVCBaseTransition@@@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationValue__QEAAXPEAVCBaseTransition___Z(
    void* pThis, void* pTransition) {
    if (pThis == nullptr || pTransition == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    g_animationValueStates[pThis].m_transitions.push_back(pTransition);
}
// Symbol: ?GetAnimationVariableList@CAnimationValue@@MEAAXAEAV?$CList@PEAVCAnimationVariable@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetAnimationVariableList_CAnimationValue__MEAAXAEAV__CList_PEAVCAnimationVariable__PEAV1____Z(
    void* pThis, void* pList) {
    if (pThis == nullptr || pList == nullptr) {
        return;
    }

    auto* pVariableList = static_cast<CList<CAnimationVariable*, CAnimationVariable*>*>(pList);
    pVariableList->RemoveAll();

    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    auto& state = g_animationValueStates[pThis];
    if (state.m_variable == nullptr) {
        state.m_variable = new (std::nothrow) CAnimationVariable(state.m_value);
        if (state.m_variable) {
            static_cast<CAnimationVariable*>(state.m_variable)->SetDefaultValue(state.m_value);
        }
    }
    if (state.m_variable) {
        pVariableList->AddTail(static_cast<CAnimationVariable*>(state.m_variable));
    }
}
// Symbol: ?GetValue@CAnimationValue@@QEAAHAEAH@Z
extern "C" int MS_ABI impl__GetValue_CAnimationValue__QEAAHAEAH_Z(void* pThis, int* pValue) {
    if (pThis == nullptr || pValue == nullptr) {
        return E_POINTER;
    }
    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    *pValue = static_cast<int>(g_animationValueStates[pThis].m_value);
    return S_OK;
}
// Symbol: ?GetValue@CAnimationValue@@QEAAHAEAN@Z
extern "C" int MS_ABI impl__GetValue_CAnimationValue__QEAAHAEAN_Z(void* pThis, double* pValue) {
    if (pThis == nullptr || pValue == nullptr) {
        return E_POINTER;
    }
    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    *pValue = g_animationValueStates[pThis].m_value;
    return S_OK;
}
