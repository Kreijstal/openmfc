// CByteArray — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}
// Symbol: ??0CByteArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CByteArray__QEAA_XZ, CByteArray)
#undef OPENMFC_WRAP_CTOR0
#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}
// Symbol: ??1CByteArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CByteArray__UEAA_XZ, CByteArray)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?Append@CByteArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CByteArray__QEAA_JAEBV1__Z(CByteArray* pThis, const CByteArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CByteArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CByteArray__QEAAXAEBV1__Z(CByteArray* pThis, const CByteArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) \
extern "C" CObject* MS_ABI fn_name() { \
    return class_name::CreateObject(); \
}
// Symbol: ?CreateObject@CByteArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CByteArray__SAPEAVCObject__XZ, CByteArray)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeExtra@CByteArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CByteArray__QEAAXXZ(CByteArray* pThis) { if (pThis) pThis->FreeExtra(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetRuntimeClass@CByteArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CByteArray__UEBAPEAUCRuntimeClass__XZ, CByteArray)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetThisClass@CByteArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CByteArray__SAPEAUCRuntimeClass__XZ, CByteArray)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAt@CByteArray@@QEAAX_JE0@Z
extern "C" void MS_ABI impl__InsertAt_CByteArray__QEAAX_JE0_Z(CByteArray* pThis, long long nIndex, unsigned char value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CByteArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CByteArray__QEAAX_JPEAV1__Z(CByteArray* pThis, long long nIndex, CByteArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?RemoveAt@CByteArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CByteArray__QEAAX_J0_Z(CByteArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CByteArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CByteArray__UEAAXAEAVCArchive___Z(CByteArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CByteArray@@QEAAX_JE@Z
extern "C" void MS_ABI impl__SetAtGrow_CByteArray__QEAAX_JE_Z(CByteArray* pThis, long long nIndex, unsigned char value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CByteArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CByteArray__QEAAX_J0_Z(CByteArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }
