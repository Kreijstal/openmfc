// CMapPtrToPtr — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapPtrToPtr@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapPtrToPtr__QEAA__J_Z, CMapPtrToPtr, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapPtrToPtr@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapPtrToPtr__UEAA_XZ, CMapPtrToPtr)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapPtrToPtr@@QEAAAEAPEAXPEAX@Z
extern "C" void** MS_ABI impl___ACMapPtrToPtr__QEAAAEAPEAXPEAX_Z(CMapPtrToPtr* pThis, void* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapPtrToPtr@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToPtr__IEAAXPEAUCAssoc_1___Z(CMapPtrToPtr* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<void*, void*>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapPtrToPtr@@IEBAPEAUCAssoc@1@PEAXAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapPtrToPtr__IEBAPEAUCAssoc_1_PEAXAEAI1_Z(const CMapPtrToPtr* pThis, void* key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(key) >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; void* value = nullptr; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<void*, void*>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapPtrToPtr@@QEBAXAEAPEAU__POSITION@@AEAPEAX1@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapPtrToPtr__QEBAXAEAPEAU__POSITION__AEAPEAX1_Z(const CMapPtrToPtr* pThis, CMapPtrToPtr::POSITION& pos, void*& key, void*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = nullptr; value = nullptr; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapPtrToPtr@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapPtrToPtr__UEBAPEAUCRuntimeClass__XZ, CMapPtrToPtr)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapPtrToPtr@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapPtrToPtr__SAPEAUCRuntimeClass__XZ, CMapPtrToPtr)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?GetValueAt@CMapPtrToPtr@@QEBAPEAXPEAX@Z
extern "C" void* MS_ABI impl__GetValueAt_CMapPtrToPtr__QEBAPEAXPEAX_Z(const CMapPtrToPtr* pThis, void* key) { void* value = nullptr; return (pThis && pThis->Lookup(key, value)) ? value : nullptr; }
// Symbol: ?InitHashTable@CMapPtrToPtr@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapPtrToPtr__QEAAXIH_Z(CMapPtrToPtr* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapPtrToPtr@@QEBAHPEAXAEAPEAX@Z
extern "C" int MS_ABI impl__Lookup_CMapPtrToPtr__QEBAHPEAXAEAPEAX_Z(const CMapPtrToPtr* pThis, void* key, void*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapPtrToPtr@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapPtrToPtr__IEAAPEAUCAssoc_1_XZ(CMapPtrToPtr* /*pThis*/) { return new AssocSnapshot<void*, void*>(); }
// Symbol: ?RemoveAll@CMapPtrToPtr@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapPtrToPtr__QEAAXXZ(CMapPtrToPtr* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapPtrToPtr@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__RemoveKey_CMapPtrToPtr__QEAAHPEAX_Z(CMapPtrToPtr* pThis, void* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
