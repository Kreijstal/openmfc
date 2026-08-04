// CMapWordToOb — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapWordToOb@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapWordToOb__QEAA__J_Z, CMapWordToOb, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapWordToOb@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapWordToOb__UEAA_XZ, CMapWordToOb)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapWordToOb@@QEAAAEAPEAVCObject@@G@Z
extern "C" CObject** MS_ABI impl___ACMapWordToOb__QEAAAEAPEAVCObject__G_Z(CMapWordToOb* pThis, unsigned short key) { return pThis ? &((*pThis)[key]) : nullptr; }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
// Symbol: ?CreateObject@CMapWordToOb@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CMapWordToOb__SAPEAVCObject__XZ, CMapWordToOb)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeAssoc@CMapWordToOb@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToOb__IEAAXPEAUCAssoc_1___Z(CMapWordToOb* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<WORD, CObject*>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapWordToOb@@IEBAPEAUCAssoc@1@GAEAI0@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapWordToOb__IEBAPEAUCAssoc_1_GAEAI0_Z(const CMapWordToOb* pThis, unsigned short key, unsigned int& nHash, unsigned int& nHashBucket) { nHash = static_cast<unsigned int>(key >> 4); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; CObject* value = nullptr; if (!pThis || !pThis->Lookup(key, value)) return nullptr; auto* assoc = new AssocSnapshot<WORD, CObject*>(); assoc->nHashValue = nHash; assoc->key = key; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapWordToOb@@QEBAXAEAPEAU__POSITION@@AEAGAEAPEAVCObject@@@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapWordToOb__QEBAXAEAPEAU__POSITION__AEAGAEAPEAVCObject___Z(const CMapWordToOb* pThis, CMapWordToOb::POSITION& pos, unsigned short& key, CObject*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = 0; value = nullptr; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapWordToOb@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapWordToOb__UEBAPEAUCRuntimeClass__XZ, CMapWordToOb)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapWordToOb@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapWordToOb__SAPEAUCRuntimeClass__XZ, CMapWordToOb)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InitHashTable@CMapWordToOb@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapWordToOb__QEAAXIH_Z(CMapWordToOb* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapWordToOb@@QEBAHGAEAPEAVCObject@@@Z
extern "C" int MS_ABI impl__Lookup_CMapWordToOb__QEBAHGAEAPEAVCObject___Z(const CMapWordToOb* pThis, unsigned short key, CObject*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapWordToOb@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapWordToOb__IEAAPEAUCAssoc_1_XZ(CMapWordToOb* /*pThis*/) { return new AssocSnapshot<WORD, CObject*>(); }
// Symbol: ?RemoveAll@CMapWordToOb@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapWordToOb__QEAAXXZ(CMapWordToOb* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapWordToOb@@QEAAHG@Z
extern "C" int MS_ABI impl__RemoveKey_CMapWordToOb__QEAAHG_Z(CMapWordToOb* pThis, unsigned short key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
// Symbol: ?Serialize@CMapWordToOb@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMapWordToOb__UEAAXAEAVCArchive___Z(CMapWordToOb* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
