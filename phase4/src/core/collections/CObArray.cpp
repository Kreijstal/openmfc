// CObArray — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}
// Symbol: ??0CObArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CObArray__QEAA_XZ, CObArray)
#undef OPENMFC_WRAP_CTOR0
#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}
// Symbol: ??1CObArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CObArray__UEAA_XZ, CObArray)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?Append@CObArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CObArray__QEAA_JAEBV1__Z(CObArray* pThis, const CObArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CObArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CObArray__QEAAXAEBV1__Z(CObArray* pThis, const CObArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) \
extern "C" CObject* MS_ABI fn_name() { \
    return class_name::CreateObject(); \
}
// Symbol: ?CreateObject@CObArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CObArray__SAPEAVCObject__XZ, CObArray)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeExtra@CObArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CObArray__QEAAXXZ(CObArray* pThis) { if (pThis) pThis->FreeExtra(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetRuntimeClass@CObArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CObArray__UEBAPEAUCRuntimeClass__XZ, CObArray)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetThisClass@CObArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CObArray__SAPEAUCRuntimeClass__XZ, CObArray)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAt@CObArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CObArray__QEAAX_JPEAV1__Z(CObArray* pThis, long long nIndex, CObArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?InsertAt@CObArray@@QEAAX_JPEAVCObject@@0@Z
extern "C" void MS_ABI impl__InsertAt_CObArray__QEAAX_JPEAVCObject__0_Z(CObArray* pThis, long long nIndex, CObject* value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?RemoveAt@CObArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CObArray__QEAAX_J0_Z(CObArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CObArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CObArray__UEAAXAEAVCArchive___Z(CObArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CObArray@@QEAAX_JPEAVCObject@@@Z
extern "C" void MS_ABI impl__SetAtGrow_CObArray__QEAAX_JPEAVCObject___Z(CObArray* pThis, long long nIndex, CObject* value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CObArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CObArray__QEAAX_J0_Z(CObArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }
