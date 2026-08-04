// CUIntArray — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}
// Symbol: ??0CUIntArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CUIntArray__QEAA_XZ, CUIntArray)
#undef OPENMFC_WRAP_CTOR0
#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}
// Symbol: ??1CUIntArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CUIntArray__UEAA_XZ, CUIntArray)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?Append@CUIntArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CUIntArray__QEAA_JAEBV1__Z(CUIntArray* pThis, const CUIntArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CUIntArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CUIntArray__QEAAXAEBV1__Z(CUIntArray* pThis, const CUIntArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
// Symbol: ?FreeExtra@CUIntArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CUIntArray__QEAAXXZ(CUIntArray* pThis) { if (pThis) pThis->FreeExtra(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetRuntimeClass@CUIntArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CUIntArray__UEBAPEAUCRuntimeClass__XZ, CUIntArray)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetThisClass@CUIntArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CUIntArray__SAPEAUCRuntimeClass__XZ, CUIntArray)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAt@CUIntArray@@QEAAX_JI0@Z
extern "C" void MS_ABI impl__InsertAt_CUIntArray__QEAAX_JI0_Z(CUIntArray* pThis, long long nIndex, unsigned int value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CUIntArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CUIntArray__QEAAX_JPEAV1__Z(CUIntArray* pThis, long long nIndex, CUIntArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?RemoveAt@CUIntArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CUIntArray__QEAAX_J0_Z(CUIntArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?SetAtGrow@CUIntArray@@QEAAX_JI@Z
extern "C" void MS_ABI impl__SetAtGrow_CUIntArray__QEAAX_JI_Z(CUIntArray* pThis, long long nIndex, unsigned int value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CUIntArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CUIntArray__QEAAX_J0_Z(CUIntArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }
