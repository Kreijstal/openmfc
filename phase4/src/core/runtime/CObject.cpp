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
// CObject::IsKindOf() - const member function
// Checks if this object is an instance of the given class or derived from it
//
// =============================================================================
// IMPORTANT: MSVC vtable layout assumption
// =============================================================================
// This implementation assumes MSVC-style vtable layout where vtable[0] is
// GetRuntimeClass(). This is CORRECT because:
//
// 1. This function is exported with MSVC mangling and is called from MSVC code
// 2. Objects reaching this function come from one of:
//    a) MSVC-compiled MFC application code (native MSVC vtables)
//    b) Our OpenMFC exception objects with patched MSVC-style vtables
//    c) Our header-defined classes with GetRuntimeClass() declared first
//
// 3. Our CObject header (afx.h) declares virtual methods in MSVC order:
//    - GetRuntimeClass() is declared BEFORE the destructor
//    - This ensures vtable[0] = GetRuntimeClass for MinGW-compiled objects too
//
// This is NOT safe for pure Itanium-ABI objects where destructors come first.
// However, such objects should never reach this MSVC-exported function.
// =============================================================================
// Symbol: ?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis,           // RCX = this pointer
    const CRuntimeClass* pClass     // RDX = class to check against
) {
    if (!pThis || !pClass) {
        return FALSE;
    }

    const CRuntimeClass* pThisClass = GetObjectRuntimeClass(pThis);

    if (!pThisClass) {
        return FALSE;
    }

    // Walk the inheritance chain
    while (pThisClass != nullptr) {
        if (pThisClass == pClass) {
            return TRUE;
        }
        // Prefer m_pfnGetBaseClass: a client compiled against MSVC's MFC headers builds
        // _AFXDLL descriptors, whose base link is the function pointer -- their
        // m_pBaseClass slot is not a base pointer at all. Walking m_pBaseClass alone made
        // IsKindOf return FALSE after one step for every client-defined class, and for the
        // in-tree descriptors that are pfn-only. CRuntimeClass::IsDerivedFrom and
        // AfxDynamicDownCast already do it this way; this walker did not.
        pThisClass = pThisClass->m_pfnGetBaseClass
                         ? pThisClass->m_pfnGetBaseClass()
                         : pThisClass->m_pBaseClass;
    }

    return FALSE;
}
// CObject::IsSerializable() - const member function
// Returns TRUE if this class supports serialization
// Symbol: ?IsSerializable@CObject@@QEBAHXZ
extern "C" int MS_ABI impl__IsSerializable_CObject__QEBAHXZ(
    const CObject* pThis  // RCX = this pointer
) {
    const CRuntimeClass* pClass = GetObjectRuntimeClass(pThis);
    return pClass && pClass->m_wSchema != 0xFFFF ? TRUE : FALSE;
}
