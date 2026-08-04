// CPtrArray — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}
// Symbol: ??0CPtrArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CPtrArray__QEAA_XZ, CPtrArray)
#undef OPENMFC_WRAP_CTOR0
#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}
// Symbol: ??1CPtrArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CPtrArray__UEAA_XZ, CPtrArray)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?Append@CPtrArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CPtrArray__QEAA_JAEBV1__Z(CPtrArray* pThis, const CPtrArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CPtrArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CPtrArray__QEAAXAEBV1__Z(CPtrArray* pThis, const CPtrArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?FreeExtra@CPtrArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CPtrArray__QEAAXXZ(CPtrArray* pThis) { if (pThis) pThis->FreeExtra(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetRuntimeClass@CPtrArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CPtrArray__UEBAPEAUCRuntimeClass__XZ, CPtrArray)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetThisClass@CPtrArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CPtrArray__SAPEAUCRuntimeClass__XZ, CPtrArray)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAt@CPtrArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CPtrArray__QEAAX_JPEAV1__Z(CPtrArray* pThis, long long nIndex, CPtrArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?InsertAt@CPtrArray@@QEAAX_JPEAX0@Z
extern "C" void MS_ABI impl__InsertAt_CPtrArray__QEAAX_JPEAX0_Z(CPtrArray* pThis, long long nIndex, void* value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?RemoveAt@CPtrArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CPtrArray__QEAAX_J0_Z(CPtrArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?SetAtGrow@CPtrArray@@QEAAX_JPEAX@Z
extern "C" void MS_ABI impl__SetAtGrow_CPtrArray__QEAAX_JPEAX_Z(CPtrArray* pThis, long long nIndex, void* value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CPtrArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CPtrArray__QEAAX_J0_Z(CPtrArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }
