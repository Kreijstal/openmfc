// CThreadSlotData — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cthreadslotdata.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CThreadSlotDataSupport.h"

// Symbol: ?AllocSlot@CThreadSlotData@@QEAAHXZ
extern "C" int MS_ABI impl__AllocSlot_CThreadSlotData__QEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_threadSlotMutex);
    auto& state = g_threadSlotStates[pThis];
    if (!state.freeSlots.empty()) {
        int slot = *state.freeSlots.begin();
        state.freeSlots.erase(slot);
        return slot;
    }
    return state.nextSlot++;
}
// Symbol: ?AssignInstance@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(void* pThis, HINSTANCE hInst) {
    (void)pThis;
    (void)hInst;
}
// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUCThreadData@@PEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z(
    void* pThis, void* pThreadData, HINSTANCE hInst) {
    (void)pThreadData;
    (void)hInst;
    std::lock_guard<std::mutex> lock(g_threadSlotMutex);
    auto it = g_threadSlotStates.find(pThis);
    if (it != g_threadSlotStates.end()) {
        it->second.threadValues.erase(::GetCurrentThreadId());
    }
}
// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@H@Z
extern "C" void MS_ABI impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(void* pThis, HINSTANCE hInst, int bAll) {
    (void)hInst;
    std::lock_guard<std::mutex> lock(g_threadSlotMutex);
    auto it = g_threadSlotStates.find(pThis);
    if (it == g_threadSlotStates.end()) return;
    if (bAll) {
        it->second.threadValues.clear();
    } else {
        it->second.threadValues.erase(::GetCurrentThreadId());
    }
}
// Symbol: ?FreeSlot@CThreadSlotData@@QEAAXH@Z
extern "C" void MS_ABI impl__FreeSlot_CThreadSlotData__QEAAXH_Z(void* pThis, int nSlot) {
    std::lock_guard<std::mutex> lock(g_threadSlotMutex);
    auto& state = g_threadSlotStates[pThis];
    state.freeSlots.insert(nSlot);
    for (auto& entry : state.threadValues) {
        entry.second.erase(nSlot);
    }
}
// Symbol: ?SetValue@CThreadSlotData@@QEAAXHPEAX@Z
extern "C" void MS_ABI impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(void* pThis, int nSlot, void* pValue) {
    std::lock_guard<std::mutex> lock(g_threadSlotMutex);
    g_threadSlotStates[pThis].threadValues[::GetCurrentThreadId()][nSlot] = pValue;
}
// Symbol: ??0CThreadSlotData@@QEAA@XZ
extern "C" void MS_ABI impl___0CThreadSlotData__QEAA_XZ(CThreadSlotData* pThis) {
    if (pThis == nullptr)
        return;
    pThis->m_tlsIndex = TlsAlloc();
    pThis->m_nAlloc   = 0;
    pThis->m_nRover   = 1;   // slot 0 is reserved; rover starts at 1
    pThis->m_nMax     = 0;
    pThis->m_list.m_pHead = nullptr;
    pThis->m_list.m_nNextOffset = offsetof(CThreadData, pNext); // == 0
    pThis->m_pSlotData = nullptr;
    InitializeCriticalSection(&pThis->m_sect);
}
// Symbol: ??1CThreadSlotData@@QEAA@XZ
extern "C" void MS_ABI impl___1CThreadSlotData__QEAA_XZ(CThreadSlotData* pThis) {
    if (pThis == nullptr)
        return;
    EnterCriticalSection(&pThis->m_sect);
    // Walk m_list, freeing each thread's CThreadData and its value array.
    void* p = pThis->m_list.m_pHead;
    const size_t off = pThis->m_list.m_nNextOffset;
    while (p != nullptr) {
        void* pNext = *(void**)((BYTE*)p + off);
        CThreadData* pData = (CThreadData*)p;
        free(pData->pData);
        free(pData);
        p = pNext;
    }
    pThis->m_list.m_pHead = nullptr;
    if (pThis->m_tlsIndex != TLS_OUT_OF_INDEXES)
        TlsFree(pThis->m_tlsIndex);
    pThis->m_tlsIndex = TLS_OUT_OF_INDEXES;
    free(pThis->m_pSlotData);
    pThis->m_pSlotData = nullptr;
    pThis->m_nAlloc = 0;
    pThis->m_nMax   = 0;
    LeaveCriticalSection(&pThis->m_sect);
    DeleteCriticalSection(&pThis->m_sect);
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?AllocSlot@CThreadSlotData@@QEAAHXZ
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?AssignInstance@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FreeSlot@CThreadSlotData@@QEAAXH@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetValue@CThreadSlotData@@QEAAXHPEAX@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@H@Z
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUCThreadData@@PEAUHINSTANCE__@@@Z
