// Native process-local factory, recursive global lock 16 and framework rethrow.
#define OPENMFC_APPCORE_IMPL
#include "detail/CProcessLocalObjectSupport.h"
#include "openmfc/afxwin.h"
extern "C" void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
extern "C" void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
// Symbol: ??1CProcessLocalObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CProcessLocalObject__QEAA_XZ(void* memory){
    auto* self=static_cast<S_Cprocesslocalobject*>(memory);
    if(void* object=self->m_pObject){auto** table=*static_cast<void***>(object);reinterpret_cast<DeletingDtor>(table[0])(object,1);}
}
// Symbol: ?GetData@CProcessLocalObject@@QEAAPEAVCNoTrackObject@@P6APEAV2@XZ@Z
extern "C" void* MS_ABI impl__GetData_CProcessLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(void* memory,CreateFn create){
    auto* self=static_cast<S_Cprocesslocalobject*>(memory);
    if(!self->m_pObject){
        impl__AfxLockGlobals__YAXH_Z(16);
        try{if(!self->m_pObject)self->m_pObject=create();}
        catch(CException*){impl__AfxUnlockGlobals__YAXH_Z(16);throw;}
        impl__AfxUnlockGlobals__YAXH_Z(16);
    }
    return self->m_pObject;
}
