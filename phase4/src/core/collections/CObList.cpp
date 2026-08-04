// CObList — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CObList@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CObList__QEAA__J_Z, CObList, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CObList@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CObList__UEAA_XZ, CObList)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?AddHead@CObList@@QEAAPEAU__POSITION@@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__AddHead_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value) { return pThis ? pThis->AddHead(value) : CObList::POSITION(nullptr); }
// Symbol: ?AddHead@CObList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddHead_CObList__QEAAXPEAV1__Z(CObList* pThis, CObList* pNewList) { if (pThis) pThis->AddHead(pNewList); }
// Symbol: ?AddTail@CObList@@QEAAPEAU__POSITION@@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value) { return pThis ? pThis->AddTail(value) : CObList::POSITION(nullptr); }
// Symbol: ?AddTail@CObList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddTail_CObList__QEAAXPEAV1__Z(CObList* pThis, CObList* pNewList) { if (pThis) pThis->AddTail(pNewList); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
// Symbol: ?CreateObject@CObList@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CObList__SAPEAVCObject__XZ, CObList)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?Find@CObList@@QEBAPEAU__POSITION@@PEAVCObject@@PEAU2@@Z
extern "C" CObList::POSITION MS_ABI impl__Find_CObList__QEBAPEAU__POSITION__PEAVCObject__PEAU2__Z(const CObList* pThis, CObject* value, CObList::POSITION* pStartAfter) { return pThis ? pThis->Find(value, pStartAfter ? *pStartAfter : CObList::POSITION(nullptr)) : CObList::POSITION(nullptr); }
// Symbol: ?FindIndex@CObList@@QEBAPEAU__POSITION@@_J@Z
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex) { return pThis ? pThis->FindIndex(nIndex) : CObList::POSITION(nullptr); }
// Symbol: ?FreeNode@CObList@@IEAAXPEAUCNode@1@@Z
extern "C" void MS_ABI impl__FreeNode_CObList__IEAAXPEAUCNode_1___Z(CObList* /*pThis*/, void* pNode) { delete static_cast<ListNodeSnapshot<CObject*>*>(pNode); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CObList@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CObList__UEBAPEAUCRuntimeClass__XZ, CObList)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CObList@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CObList__SAPEAUCRuntimeClass__XZ, CObList)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAfter@CObList@@QEAAPEAU__POSITION@@PEAU2@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__InsertAfter_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(CObList* pThis, CObList::POSITION* pPos, CObject* value) { return pThis ? pThis->InsertAfter(pPos ? *pPos : CObList::POSITION(nullptr), value) : CObList::POSITION(nullptr); }
// Symbol: ?InsertBefore@CObList@@QEAAPEAU__POSITION@@PEAU2@PEAVCObject@@@Z
extern "C" CObList::POSITION MS_ABI impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(CObList* pThis, CObList::POSITION* pPos, CObject* value) { return pThis ? pThis->InsertBefore(pPos ? *pPos : CObList::POSITION(nullptr), value) : CObList::POSITION(nullptr); }
// Symbol: ?NewNode@CObList@@IEAAPEAUCNode@1@PEAU21@0@Z
extern "C" void* MS_ABI impl__NewNode_CObList__IEAAPEAUCNode_1__PEAU21_0_Z(CObList* /*pThis*/, void* pPrev, void* pNext) { auto* node = new ListNodeSnapshot<CObject*>(); node->pPrev = static_cast<ListNodeSnapshot<CObject*>*>(pPrev); node->pNext = static_cast<ListNodeSnapshot<CObject*>*>(pNext); return node; }
// Symbol: ?RemoveAll@CObList@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveAt@CObList@@QEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos) { if (pThis && pPos) pThis->RemoveAt(*pPos); }
// Symbol: ?RemoveHead@CObList@@QEAAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis) { return pThis ? pThis->RemoveHead() : nullptr; }
// Symbol: ?RemoveTail@CObList@@QEAAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__RemoveTail_CObList__QEAAPEAVCObject__XZ(CObList* pThis) { return pThis ? pThis->RemoveTail() : nullptr; }
// Symbol: ?Serialize@CObList@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CObList__UEAAXAEAVCArchive___Z(CObList* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION___J_Z(const CObList* pThis, long long nIndex) { return impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pThis, nIndex); }
