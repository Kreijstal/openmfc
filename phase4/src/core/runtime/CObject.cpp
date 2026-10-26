// CObject — OpenMFC implementation.
// Sources: cobject_impl.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRuntimeClassSupport.h"

// CObject::GetRuntimeClass() - virtual method
// Returns pointer to this object's CRuntimeClass
// Note: This is virtual, so derived classes override it
// Symbol: ?GetRuntimeClass@CObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(
    const CObject* pThis  // RCX = this pointer
) {
    (void)pThis;  // For CObject base, we return classCObject
    InitializeClasses();
    return &CObject::classCObject;
}
// CObject::GetThisClass() - static method
// Returns pointer to CObject's CRuntimeClass (not virtual dispatch)
// Symbol: ?GetThisClass@CObject@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CObject__SAPEAUCRuntimeClass__XZ() {
    InitializeClasses();
    return &CObject::classCObject;
}
// Native runtime ancestry checks live in CObjectTypeChecks.cpp.
// CObject::IsSerializable() - const member function
// Returns TRUE if this class supports serialization
// Symbol: ?IsSerializable@CObject@@QEBAHXZ
extern "C" int MS_ABI impl__IsSerializable_CObject__QEBAHXZ(
    const CObject* pThis  // RCX = this pointer
) {
    const CRuntimeClass* pClass = GetObjectRuntimeClass(pThis);
    return pClass && pClass->m_wSchema != 0xFFFF ? TRUE : FALSE;
}
