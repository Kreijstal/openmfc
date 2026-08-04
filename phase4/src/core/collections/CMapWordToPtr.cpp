// CMapWordToPtr — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapWordToPtr@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapWordToPtr__QEAA__J_Z, CMapWordToPtr, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapWordToPtr@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapWordToPtr__UEAA_XZ, CMapWordToPtr)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapWordToPtr@@QEAAAEAPEAXG@Z
extern "C" void** MS_ABI impl___ACMapWordToPtr__QEAAAEAPEAXG_Z(CMapWordToPtr* pThis, unsigned short key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapWordToPtr@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1___Z(CMapWordToPtr* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<WORD, void*>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapWordToPtr@@IEBAPEAUCAssoc@1@GAEAI0@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapWordToPtr__IEBAPEAUCAssoc_1_GAEAI0_Z(const CMapWordToPtr* pThis, unsigned short key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(key >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; void* value = nullptr; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<WORD, void*>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapWordToPtr@@QEBAXAEAPEAU__POSITION@@AEAGAEAPEAX@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapWordToPtr__QEBAXAEAPEAU__POSITION__AEAGAEAPEAX_Z(const CMapWordToPtr* pThis, CMapWordToPtr::POSITION& pos, unsigned short& key, void*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = 0; value = nullptr; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapWordToPtr@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapWordToPtr__UEBAPEAUCRuntimeClass__XZ, CMapWordToPtr)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapWordToPtr@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapWordToPtr__SAPEAUCRuntimeClass__XZ, CMapWordToPtr)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InitHashTable@CMapWordToPtr@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapWordToPtr__QEAAXIH_Z(CMapWordToPtr* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapWordToPtr@@QEBAHGAEAPEAX@Z
extern "C" int MS_ABI impl__Lookup_CMapWordToPtr__QEBAHGAEAPEAX_Z(const CMapWordToPtr* pThis, unsigned short key, void*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapWordToPtr@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapWordToPtr__IEAAPEAUCAssoc_1_XZ(CMapWordToPtr* /*pThis*/) { return new AssocSnapshot<WORD, void*>(); }
// Symbol: ?RemoveAll@CMapWordToPtr@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapWordToPtr__QEAAXXZ(CMapWordToPtr* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapWordToPtr@@QEAAHG@Z
extern "C" int MS_ABI impl__RemoveKey_CMapWordToPtr__QEAAHG_Z(CMapWordToPtr* pThis, unsigned short key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
