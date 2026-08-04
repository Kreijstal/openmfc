// CMapPtrToWord — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapPtrToWord@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapPtrToWord__QEAA__J_Z, CMapPtrToWord, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapPtrToWord@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapPtrToWord__UEAA_XZ, CMapPtrToWord)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapPtrToWord@@QEAAAEAGPEAX@Z
extern "C" unsigned short* MS_ABI impl___ACMapPtrToWord__QEAAAEAGPEAX_Z(CMapPtrToWord* pThis, void* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapPtrToWord@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1___Z(CMapPtrToWord* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<void*, WORD>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapPtrToWord@@IEBAPEAUCAssoc@1@PEAXAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapPtrToWord__IEBAPEAUCAssoc_1_PEAXAEAI1_Z(const CMapPtrToWord* pThis, void* key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(key) >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; WORD value = 0; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<void*, WORD>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapPtrToWord@@QEBAXAEAPEAU__POSITION@@AEAPEAXAEAG@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapPtrToWord__QEBAXAEAPEAU__POSITION__AEAPEAXAEAG_Z(const CMapPtrToWord* pThis, CMapPtrToWord::POSITION& pos, void*& key, unsigned short& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = nullptr; value = 0; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapPtrToWord@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapPtrToWord__UEBAPEAUCRuntimeClass__XZ, CMapPtrToWord)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapPtrToWord@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapPtrToWord__SAPEAUCRuntimeClass__XZ, CMapPtrToWord)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InitHashTable@CMapPtrToWord@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapPtrToWord__QEAAXIH_Z(CMapPtrToWord* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapPtrToWord@@QEBAHPEAXAEAG@Z
extern "C" int MS_ABI impl__Lookup_CMapPtrToWord__QEBAHPEAXAEAG_Z(const CMapPtrToWord* pThis, void* key, unsigned short& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapPtrToWord@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapPtrToWord__IEAAPEAUCAssoc_1_XZ(CMapPtrToWord* /*pThis*/) { return new AssocSnapshot<void*, WORD>(); }
// Symbol: ?RemoveAll@CMapPtrToWord@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapPtrToWord__QEAAXXZ(CMapPtrToWord* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapPtrToWord@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__RemoveKey_CMapPtrToWord__QEAAHPEAX_Z(CMapPtrToWord* pThis, void* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
