// CMapStringToOb — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapStringToOb@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapStringToOb__QEAA__J_Z, CMapStringToOb, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapStringToOb@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapStringToOb__UEAA_XZ, CMapStringToOb)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapStringToOb@@QEAAAEAPEAVCObject@@PEB_W@Z
extern "C" CObject** MS_ABI impl___ACMapStringToOb__QEAAAEAPEAVCObject__PEB_W_Z(CMapStringToOb* pThis, const wchar_t* key) { return pThis ? &((*pThis)[key]) : nullptr; }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
// Symbol: ?CreateObject@CMapStringToOb@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CMapStringToOb__SAPEAVCObject__XZ, CMapStringToOb)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeAssoc@CMapStringToOb@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1___Z(CMapStringToOb* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<CString, CObject*>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapStringToOb@@IEBAPEAUCAssoc@1@PEB_WAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapStringToOb__IEBAPEAUCAssoc_1_PEB_WAEAI1_Z(const CMapStringToOb* pThis, const wchar_t* key, unsigned int& nHash, unsigned int& nHashBucket) { CString k = NormalizeStringKey(key); nHash = HashCStringKey(k); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; CObject* value = nullptr; if (!pThis || !pThis->Lookup(k, value)) return nullptr; auto* assoc = new AssocSnapshot<CString, CObject*>(); assoc->nHashValue = nHash; assoc->key = k; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapStringToOb@@QEBAXAEAPEAU__POSITION@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAPEAVCObject@@@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapStringToOb__QEBAXAEAPEAU__POSITION__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAVCObject___Z(const CMapStringToOb* pThis, CMapStringToOb::POSITION& pos, CString& key, CObject*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = L""; value = nullptr; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapStringToOb@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapStringToOb__UEBAPEAUCRuntimeClass__XZ, CMapStringToOb)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapStringToOb@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapStringToOb__SAPEAUCRuntimeClass__XZ, CMapStringToOb)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InitHashTable@CMapStringToOb@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapStringToOb__QEAAXIH_Z(CMapStringToOb* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapStringToOb@@QEBAHPEB_WAEAPEAVCObject@@@Z
extern "C" int MS_ABI impl__Lookup_CMapStringToOb__QEBAHPEB_WAEAPEAVCObject___Z(const CMapStringToOb* pThis, const wchar_t* key, CObject*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?LookupKey@CMapStringToOb@@QEBAHPEB_WAEAPEB_W@Z
extern "C" int MS_ABI impl__LookupKey_CMapStringToOb__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToOb* pThis, const wchar_t* key, const wchar_t*& actualKey) { return (pThis && pThis->LookupKey(key, actualKey)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapStringToOb@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapStringToOb__IEAAPEAUCAssoc_1_XZ(CMapStringToOb* /*pThis*/) { return new AssocSnapshot<CString, CObject*>(); }
// Symbol: ?RemoveAll@CMapStringToOb@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapStringToOb__QEAAXXZ(CMapStringToOb* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapStringToOb@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveKey_CMapStringToOb__QEAAHPEB_W_Z(CMapStringToOb* pThis, const wchar_t* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
// Symbol: ?Serialize@CMapStringToOb@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMapStringToOb__UEAAXAEAVCArchive___Z(CMapStringToOb* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
CMapStringToOb::CMapStringToOb(INT_PTR nBlockSize) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this, ClampCollectionBlockSize(nBlockSize)); }
CMapStringToOb::~CMapStringToOb() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); RemoveCollectionState<CMapStringToOb, CStringKeyMapState<CObject*, CObject*>>(this); }
INT_PTR CMapStringToOb::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state ? state->data.GetCount() : 0; }
BOOL CMapStringToOb::IsEmpty() const { return GetCount() == 0; }
BOOL CMapStringToOb::Lookup(const CString& key, CObject*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return (state && state->data.Lookup(key, rValue)) ? TRUE : FALSE; }
BOOL CMapStringToOb::Lookup(const wchar_t* key, CObject*& rValue) const { return Lookup(NormalizeStringKey(key), rValue); }
BOOL CMapStringToOb::LookupKey(const CString& key, const wchar_t*& rKey) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); CString actualKey; if (!state.data.LookupKey(key, actualKey)) return FALSE; state.lookupKeyScratch = actualKey; rKey = static_cast<const wchar_t*>(state.lookupKeyScratch); return TRUE; }
BOOL CMapStringToOb::LookupKey(const wchar_t* key, const wchar_t*& rKey) const { return LookupKey(NormalizeStringKey(key), rKey); }
CObject*& CMapStringToOb::operator[](const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data[key]; }
CObject*& CMapStringToOb::operator[](const wchar_t* key) { return (*this)[NormalizeStringKey(key)]; }
const CObject* CMapStringToOb::operator[](const wchar_t* key) const { CObject* value = nullptr; Lookup(key, value); return value; }
void CMapStringToOb::SetAt(const CString& key, CObject* newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.SetAt(key, newValue); }
void CMapStringToOb::SetAt(const wchar_t* key, CObject* newValue) { SetAt(NormalizeStringKey(key), newValue); }
BOOL CMapStringToOb::RemoveKey(const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state.data.RemoveKey(key) ? TRUE : FALSE; }
BOOL CMapStringToOb::RemoveKey(const wchar_t* key) { return RemoveKey(NormalizeStringKey(key)); }
void CMapStringToOb::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.RemoveAll(); }
CMapStringToOb::POSITION CMapStringToOb::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state ? state->data.GetStartPosition() : POSITION(nullptr, 0); }
void CMapStringToOb::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CObject*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.GetNextAssoc(rNextPosition, rKey, rValue); }
UINT CMapStringToOb::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); return state ? state->data.GetHashTableSize() : 17; }
void CMapStringToOb::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this).data.InitHashTable(hashSize, bAllocNow != FALSE); }
void CMapStringToOb::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToOb, CObject*, CObject*>(this); if (ar.IsStoring()) ar << state.data; else ar >> state.data; }
