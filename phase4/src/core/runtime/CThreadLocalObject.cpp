// CThreadLocalObject — OpenMFC implementation.
// Sources: global_cthreadlocalobject.cpp

#include "detail/CThreadLocalObjectSupport.h"

// Symbol: ??1CThreadLocalObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(void* pThis) {
    S_Cthreadlocalobject* p = reinterpret_cast<S_Cthreadlocalobject*>(pThis);
    if (p->m_nSlot != 0) {
        Store& s = store();
        EnterCriticalSection(&s.cs);
        const DWORD slot = p->m_nSlot;
        for (auto it = s.values.begin(); it != s.values.end(); ) {
            if (static_cast<DWORD>(it->first & 0xFFFFFFFFu) == slot) {
                destroyValue(it->second);
                it = s.values.erase(it);
            } else {
                ++it;
            }
        }
        LeaveCriticalSection(&s.cs);
        p->m_nSlot = 0;
    }
}
// Symbol: ?GetData@CThreadLocalObject@@QEAAPEAVCNoTrackObject@@P6APEAV2@XZ@Z
// CNoTrackObject* GetData(CNoTrackObject* (*pfnCreateObject)());
extern "C" void* MS_ABI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
        void* pThis, void* pfnCreateObject) {
    S_Cthreadlocalobject* p = reinterpret_cast<S_Cthreadlocalobject*>(pThis);

    // Allocate a slot lazily on first use.
    if (p->m_nSlot == 0) {
        p->m_nSlot = allocSlot();
    }

    Store& s = store();
    const uint64_t k = makeKey(p->m_nSlot);

    EnterCriticalSection(&s.cs);
    auto it = s.values.find(k);
    void* val = (it != s.values.end()) ? it->second : nullptr;
    LeaveCriticalSection(&s.cs);

    // Create the per-thread value on first access.
    if (val == nullptr && pfnCreateObject != nullptr) {
        typedef void* (MS_ABI *CreateFn)();
        val = reinterpret_cast<CreateFn>(pfnCreateObject)();
        EnterCriticalSection(&s.cs);
        s.values[k] = val;
        LeaveCriticalSection(&s.cs);
    }
    return val;
}
// Symbol: ?GetDataNA@CThreadLocalObject@@QEAAPEAVCNoTrackObject@@XZ
// CNoTrackObject* GetDataNA();  (no-allocate lookup)
extern "C" void* MS_ABI impl__GetDataNA_CThreadLocalObject__QEAAPEAVCNoTrackObject__XZ(
        void* pThis) {
    S_Cthreadlocalobject* p = reinterpret_cast<S_Cthreadlocalobject*>(pThis);
    if (p->m_nSlot == 0) {
        return nullptr;
    }
    Store& s = store();
    EnterCriticalSection(&s.cs);
    auto it = s.values.find(makeKey(p->m_nSlot));
    void* val = (it != s.values.end()) ? it->second : nullptr;
    LeaveCriticalSection(&s.cs);
    return val;
}
