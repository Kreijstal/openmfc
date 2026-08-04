// CDWordArray — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}
// Symbol: ??0CDWordArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CDWordArray__QEAA_XZ, CDWordArray)
#undef OPENMFC_WRAP_CTOR0
#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}
// Symbol: ??1CDWordArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CDWordArray__UEAA_XZ, CDWordArray)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?Append@CDWordArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CDWordArray__QEAA_JAEBV1__Z(CDWordArray* pThis, const CDWordArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CDWordArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CDWordArray__QEAAXAEBV1__Z(CDWordArray* pThis, const CDWordArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) \
extern "C" CObject* MS_ABI fn_name() { \
    return class_name::CreateObject(); \
}
// Symbol: ?CreateObject@CDWordArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CDWordArray__SAPEAVCObject__XZ, CDWordArray)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeExtra@CDWordArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CDWordArray__QEAAXXZ(CDWordArray* pThis) { if (pThis) pThis->FreeExtra(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetRuntimeClass@CDWordArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CDWordArray__UEBAPEAUCRuntimeClass__XZ, CDWordArray)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetThisClass@CDWordArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CDWordArray__SAPEAUCRuntimeClass__XZ, CDWordArray)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAt@CDWordArray@@QEAAX_JK0@Z
extern "C" void MS_ABI impl__InsertAt_CDWordArray__QEAAX_JK0_Z(CDWordArray* pThis, long long nIndex, unsigned long value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CDWordArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CDWordArray__QEAAX_JPEAV1__Z(CDWordArray* pThis, long long nIndex, CDWordArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?RemoveAt@CDWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CDWordArray__QEAAX_J0_Z(CDWordArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CDWordArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDWordArray__UEAAXAEAVCArchive___Z(CDWordArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CDWordArray@@QEAAX_JK@Z
extern "C" void MS_ABI impl__SetAtGrow_CDWordArray__QEAAX_JK_Z(CDWordArray* pThis, long long nIndex, unsigned long value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CDWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CDWordArray__QEAAX_J0_Z(CDWordArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }
