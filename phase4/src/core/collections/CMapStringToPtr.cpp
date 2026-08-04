// CMapStringToPtr — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapStringToPtr@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapStringToPtr__QEAA__J_Z, CMapStringToPtr, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapStringToPtr@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapStringToPtr__UEAA_XZ, CMapStringToPtr)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapStringToPtr@@QEAAAEAPEAXPEB_W@Z
extern "C" void** MS_ABI impl___ACMapStringToPtr__QEAAAEAPEAXPEB_W_Z(CMapStringToPtr* pThis, const wchar_t* key) { return pThis ? &((*pThis)[key]) : nullptr; }
// Symbol: ?FreeAssoc@CMapStringToPtr@@IEAAXPEAUCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1___Z(CMapStringToPtr* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<CString, void*>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapStringToPtr@@IEBAPEAUCAssoc@1@PEB_WAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapStringToPtr__IEBAPEAUCAssoc_1_PEB_WAEAI1_Z(const CMapStringToPtr* pThis, const wchar_t* key, unsigned int& nHash, unsigned int& nHashBucket) { CString k = NormalizeStringKey(key); nHash = HashCStringKey(k); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; void* value = nullptr; if (!pThis || !pThis->Lookup(k, value)) return nullptr; auto* assoc = new AssocSnapshot<CString, void*>(); assoc->nHashValue = nHash; assoc->key = k; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapStringToPtr@@QEBAXAEAPEAU__POSITION@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAPEAX@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapStringToPtr__QEBAXAEAPEAU__POSITION__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAX_Z(const CMapStringToPtr* pThis, CMapStringToPtr::POSITION& pos, CString& key, void*& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = L""; value = nullptr; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapStringToPtr@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapStringToPtr__UEBAPEAUCRuntimeClass__XZ, CMapStringToPtr)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapStringToPtr@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapStringToPtr__SAPEAUCRuntimeClass__XZ, CMapStringToPtr)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InitHashTable@CMapStringToPtr@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapStringToPtr__QEAAXIH_Z(CMapStringToPtr* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapStringToPtr@@QEBAHPEB_WAEAPEAX@Z
extern "C" int MS_ABI impl__Lookup_CMapStringToPtr__QEBAHPEB_WAEAPEAX_Z(const CMapStringToPtr* pThis, const wchar_t* key, void*& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?LookupKey@CMapStringToPtr@@QEBAHPEB_WAEAPEB_W@Z
extern "C" int MS_ABI impl__LookupKey_CMapStringToPtr__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToPtr* pThis, const wchar_t* key, const wchar_t*& actualKey) { return (pThis && pThis->LookupKey(key, actualKey)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapStringToPtr@@IEAAPEAUCAssoc@1@XZ
extern "C" void* MS_ABI impl__NewAssoc_CMapStringToPtr__IEAAPEAUCAssoc_1_XZ(CMapStringToPtr* /*pThis*/) { return new AssocSnapshot<CString, void*>(); }
// Symbol: ?RemoveAll@CMapStringToPtr@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapStringToPtr__QEAAXXZ(CMapStringToPtr* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapStringToPtr@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveKey_CMapStringToPtr__QEAAHPEB_W_Z(CMapStringToPtr* pThis, const wchar_t* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
CMapStringToPtr::CMapStringToPtr(INT_PTR nBlockSize) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this, ClampCollectionBlockSize(nBlockSize)); }
CMapStringToPtr::~CMapStringToPtr() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); RemoveCollectionState<CMapStringToPtr, CStringKeyMapState<void*, void*>>(this); }
INT_PTR CMapStringToPtr::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state ? state->data.GetCount() : 0; }
BOOL CMapStringToPtr::IsEmpty() const { return GetCount() == 0; }
BOOL CMapStringToPtr::Lookup(const CString& key, void*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return (state && state->data.Lookup(key, rValue)) ? TRUE : FALSE; }
BOOL CMapStringToPtr::Lookup(const wchar_t* key, void*& rValue) const { return Lookup(NormalizeStringKey(key), rValue); }
BOOL CMapStringToPtr::LookupKey(const CString& key, const wchar_t*& rKey) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); CString actualKey; if (!state.data.LookupKey(key, actualKey)) return FALSE; state.lookupKeyScratch = actualKey; rKey = static_cast<const wchar_t*>(state.lookupKeyScratch); return TRUE; }
BOOL CMapStringToPtr::LookupKey(const wchar_t* key, const wchar_t*& rKey) const { return LookupKey(NormalizeStringKey(key), rKey); }
void*& CMapStringToPtr::operator[](const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data[key]; }
void*& CMapStringToPtr::operator[](const wchar_t* key) { return (*this)[NormalizeStringKey(key)]; }
const void* CMapStringToPtr::operator[](const wchar_t* key) const { void* value = nullptr; Lookup(key, value); return value; }
void CMapStringToPtr::SetAt(const CString& key, void* newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.SetAt(key, newValue); }
void CMapStringToPtr::SetAt(const wchar_t* key, void* newValue) { SetAt(NormalizeStringKey(key), newValue); }
BOOL CMapStringToPtr::RemoveKey(const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state.data.RemoveKey(key) ? TRUE : FALSE; }
BOOL CMapStringToPtr::RemoveKey(const wchar_t* key) { return RemoveKey(NormalizeStringKey(key)); }
void CMapStringToPtr::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.RemoveAll(); }
CMapStringToPtr::POSITION CMapStringToPtr::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state ? state->data.GetStartPosition() : POSITION(nullptr, 0); }
void CMapStringToPtr::GetNextAssoc(POSITION& rNextPosition, CString& rKey, void*& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.GetNextAssoc(rNextPosition, rKey, rValue); }
UINT CMapStringToPtr::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); return state ? state->data.GetHashTableSize() : 17; }
void CMapStringToPtr::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this).data.InitHashTable(hashSize, bAllocNow != FALSE); }
void CMapStringToPtr::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringKeyMapStorage<CMapStringToPtr, void*, void*>(this); if (ar.IsStoring()) ar << state.data; else ar >> state.data; }
