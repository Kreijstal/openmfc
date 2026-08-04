// CProcessLocalObject — OpenMFC implementation.
// Sources: global_cprocesslocalobject.cpp

#include "detail/CProcessLocalObjectSupport.h"

// Symbol: ??1CProcessLocalObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CProcessLocalObject__QEAA_XZ(void* pThis)
{
    S_Cprocesslocalobject* self = static_cast<S_Cprocesslocalobject*>(pThis);
    if (self->m_pObject != nullptr) {
        void** vt = *reinterpret_cast<void***>(self->m_pObject);
        DeletingDtor dtor = reinterpret_cast<DeletingDtor>(vt[0]);
        dtor(self->m_pObject, 1u);   // run virtual dtor + free
        self->m_pObject = nullptr;
    }
}
// Symbol: ?GetData@CProcessLocalObject@@QEAAPEAVCNoTrackObject@@P6APEAV2@XZ@Z
extern "C" void* MS_ABI impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
    void* pThis, CreateFn pfnCreateObject)
{
    S_Cprocesslocalobject* self = static_cast<S_Cprocesslocalobject*>(pThis);

    // Double-checked lazy creation, matching CProcessLocalObject::GetData.
    if (self->m_pObject == nullptr) {
        AcquireSRWLockExclusive(&g_processLocalLock);
        if (self->m_pObject == nullptr) {
            self->m_pObject = pfnCreateObject();
        }
        ReleaseSRWLockExclusive(&g_processLocalLock);
    }
    return self->m_pObject;
}
