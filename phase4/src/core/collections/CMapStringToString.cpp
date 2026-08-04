// CMapStringToString — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CMapStringToString@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CMapStringToString__QEAA__J_Z, CMapStringToString, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CMapStringToString@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CMapStringToString__UEAA_XZ, CMapStringToString)
#undef OPENMFC_WRAP_DTOR
// Symbol: ??ACMapStringToString@@QEAAAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" CString* MS_ABI impl___ACMapStringToString__QEAAAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CMapStringToString* pThis, const wchar_t* key) { return pThis ? &((*pThis)[key]) : nullptr; }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
// Symbol: ?CreateObject@CMapStringToString@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CMapStringToString__SAPEAVCObject__XZ, CMapStringToString)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeAssoc@CMapStringToString@@IEAAXPEAVCAssoc@1@@Z
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1___Z(CMapStringToString* /*pThis*/, void* pAssoc) { delete static_cast<AssocSnapshot<CString, CString>*>(pAssoc); }
// Symbol: ?GetAssocAt@CMapStringToString@@IEBAPEAVCAssoc@1@PEB_WAEAI1@Z
extern "C" void* MS_ABI impl__GetAssocAt_CMapStringToString__IEBAPEAVCAssoc_1_PEB_WAEAI1_Z(const CMapStringToString* pThis, const wchar_t* key, unsigned int& nHash, unsigned int& nHashBucket) { CString k = NormalizeStringKey(key); nHash = HashCStringKey(k); nHashBucket = pThis ? (nHash % pThis->GetHashTableSize()) : 0; CString value; if (!pThis || !pThis->Lookup(k, value)) return nullptr; auto* assoc = new AssocSnapshot<CString, CString>(); assoc->nHashValue = nHash; assoc->key = k; assoc->value = value; return assoc; }
// Symbol: ?GetNextAssoc@CMapStringToString@@QEBAXAEAPEAU__POSITION@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@1@Z
extern "C" void MS_ABI impl__GetNextAssoc_CMapStringToString__QEBAXAEAPEAU__POSITION__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__1_Z(const CMapStringToString* pThis, CMapStringToString::POSITION& pos, CString& key, CString& value) { if (pThis) pThis->GetNextAssoc(pos, key, value); else { key = L""; value = L""; } }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMapStringToString@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CMapStringToString__UEBAPEAUCRuntimeClass__XZ, CMapStringToString)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CMapStringToString@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CMapStringToString__SAPEAUCRuntimeClass__XZ, CMapStringToString)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InitHashTable@CMapStringToString@@QEAAXIH@Z
extern "C" void MS_ABI impl__InitHashTable_CMapStringToString__QEAAXIH_Z(CMapStringToString* pThis, unsigned int hashSize, int bAllocNow) { if (pThis) pThis->InitHashTable(hashSize, bAllocNow); }
// Symbol: ?Lookup@CMapStringToString@@QEBAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__Lookup_CMapStringToString__QEBAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CMapStringToString* pThis, const wchar_t* key, CString& value) { return (pThis && pThis->Lookup(key, value)) ? 1 : 0; }
// Symbol: ?LookupKey@CMapStringToString@@QEBAHPEB_WAEAPEB_W@Z
extern "C" int MS_ABI impl__LookupKey_CMapStringToString__QEBAHPEB_WAEAPEB_W_Z(const CMapStringToString* pThis, const wchar_t* key, const wchar_t*& actualKey) { return (pThis && pThis->LookupKey(key, actualKey)) ? 1 : 0; }
// Symbol: ?NewAssoc@CMapStringToString@@IEAAPEAVCAssoc@1@PEB_W@Z
extern "C" void* MS_ABI impl__NewAssoc_CMapStringToString__IEAAPEAVCAssoc_1_PEB_W_Z(CMapStringToString* /*pThis*/, const wchar_t* key) { auto* assoc = new AssocSnapshot<CString, CString>(); assoc->key = NormalizeStringKey(key); assoc->nHashValue = HashCStringKey(assoc->key); return assoc; }
// Symbol: ?PGetFirstAssoc@CMapStringToString@@QEAAPEAUCPair@1@XZ
extern "C" CMapStringToString::CPair* MS_ABI impl__PGetFirstAssoc_CMapStringToString__QEAAPEAUCPair_1_XZ(CMapStringToString* pThis) { return pThis ? pThis->PGetFirstAssoc() : nullptr; }
// Symbol: ?PGetFirstAssoc@CMapStringToString@@QEBAPEBUCPair@1@XZ
extern "C" const CMapStringToString::CPair* MS_ABI impl__PGetFirstAssoc_CMapStringToString__QEBAPEBUCPair_1_XZ(const CMapStringToString* pThis) { return pThis ? pThis->PGetFirstAssoc() : nullptr; }
// Symbol: ?PGetNextAssoc@CMapStringToString@@QEAAPEAUCPair@1@PEBU21@@Z
extern "C" CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEAAPEAUCPair_1_PEBU21___Z(CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return pThis ? pThis->PGetNextAssoc(pAssoc) : nullptr; }
// Symbol: ?PGetNextAssoc@CMapStringToString@@QEBAPEBUCPair@1@PEBU21@@Z
extern "C" const CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEBAPEBUCPair_1_PEBU21___Z(const CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return pThis ? pThis->PGetNextAssoc(pAssoc) : nullptr; }
// Symbol: ?PLookup@CMapStringToString@@QEAAPEAUCPair@1@PEB_W@Z
extern "C" CMapStringToString::CPair* MS_ABI impl__PLookup_CMapStringToString__QEAAPEAUCPair_1_PEB_W_Z(CMapStringToString* pThis, const wchar_t* key) { return pThis ? pThis->PLookup(key) : nullptr; }
// Symbol: ?PLookup@CMapStringToString@@QEBAPEBUCPair@1@PEB_W@Z
extern "C" const CMapStringToString::CPair* MS_ABI impl__PLookup_CMapStringToString__QEBAPEBUCPair_1_PEB_W_Z(const CMapStringToString* pThis, const wchar_t* key) { return pThis ? pThis->PLookup(key) : nullptr; }
// Symbol: ?RemoveAll@CMapStringToString@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CMapStringToString__QEAAXXZ(CMapStringToString* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveKey@CMapStringToString@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveKey_CMapStringToString__QEAAHPEB_W_Z(CMapStringToString* pThis, const wchar_t* key) { return (pThis && pThis->RemoveKey(key)) ? 1 : 0; }
// Symbol: ?Serialize@CMapStringToString@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMapStringToString__UEAAXAEAVCArchive___Z(CMapStringToString* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
CMapStringToString::CMapStringToString(INT_PTR nBlockSize) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringMapStringStorage(this, ClampCollectionBlockSize(nBlockSize)); }
CMapStringToString::~CMapStringToString() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); RemoveCollectionState<CMapStringToString, CStringMapStringState>(this); }
INT_PTR CMapStringToString::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return state ? state->data.GetCount() : 0; }
BOOL CMapStringToString::IsEmpty() const { return GetCount() == 0; }
BOOL CMapStringToString::Lookup(const CString& key, CString& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return (state && state->data.Lookup(key, rValue)) ? TRUE : FALSE; }
BOOL CMapStringToString::Lookup(const wchar_t* key, CString& rValue) const { return Lookup(NormalizeStringKey(key), rValue); }
BOOL CMapStringToString::LookupKey(const CString& key, const wchar_t*& rKey) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); CString actualKey; if (!state.data.LookupKey(key, actualKey)) return FALSE; state.lookupKeyScratch = actualKey; rKey = static_cast<const wchar_t*>(state.lookupKeyScratch); return TRUE; }
BOOL CMapStringToString::LookupKey(const wchar_t* key, const wchar_t*& rKey) const { return LookupKey(NormalizeStringKey(key), rKey); }
CString& CMapStringToString::operator[](const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.MarkDirty(); return state.data[key]; }
CString& CMapStringToString::operator[](const wchar_t* key) { return (*this)[NormalizeStringKey(key)]; }
const CString& CMapStringToString::operator[](const wchar_t* key) const { return const_cast<CMapStringToString*>(this)->operator[](key); }
void CMapStringToString::SetAt(const CString& key, const CString& newValue) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.data.SetAt(key, newValue); state.MarkDirty(); }
void CMapStringToString::SetAt(const wchar_t* key, const CString& newValue) { SetAt(NormalizeStringKey(key), newValue); }
void CMapStringToString::SetAt(const wchar_t* key, const wchar_t* newValue) { SetAt(NormalizeStringKey(key), NormalizeStringKey(newValue)); }
BOOL CMapStringToString::RemoveKey(const CString& key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); BOOL removed = state.data.RemoveKey(key) ? TRUE : FALSE; if (removed) state.MarkDirty(); return removed; }
BOOL CMapStringToString::RemoveKey(const wchar_t* key) { return RemoveKey(NormalizeStringKey(key)); }
void CMapStringToString::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.data.RemoveAll(); state.MarkDirty(); }
CMapStringToString::POSITION CMapStringToString::GetStartPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return state ? state->data.GetStartPosition() : POSITION(nullptr, 0); }
void CMapStringToString::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CString& rValue) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureCStringMapStringStorage(this).data.GetNextAssoc(rNextPosition, rKey, rValue); }
UINT CMapStringToString::GetHashTableSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* state = FindCStringMapStringStorage(this); return state ? state->data.GetHashTableSize() : 17; }
void CMapStringToString::InitHashTable(UINT hashSize, BOOL bAllocNow) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.data.InitHashTable(hashSize, bAllocNow != FALSE); state.MarkDirty(); }
CMapStringToString::CPair* CMapStringToString::PLookup(const wchar_t* key) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringMapStringStorage(this).PLookup(key); }
const CMapStringToString::CPair* CMapStringToString::PLookup(const wchar_t* key) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureCStringMapStringStorage(this).PLookup(key); }
CMapStringToString::CPair* CMapStringToString::PGetFirstAssoc() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.EnsurePairs(); return state.pairs.empty() ? nullptr : state.pairs.front().get(); }
const CMapStringToString::CPair* CMapStringToString::PGetFirstAssoc() const { return const_cast<CMapStringToString*>(this)->PGetFirstAssoc(); }
CMapStringToString::CPair* CMapStringToString::PGetNextAssoc(const CPair* pAssocRet) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); state.EnsurePairs(); if (!pAssocRet) return nullptr; for (size_t i = 0; i < state.pairs.size(); ++i) { if (state.pairs[i].get() == pAssocRet) return (i + 1 < state.pairs.size()) ? state.pairs[i + 1].get() : nullptr; } return nullptr; }
const CMapStringToString::CPair* CMapStringToString::PGetNextAssoc(const CPair* pAssocRet) const { return const_cast<CMapStringToString*>(this)->PGetNextAssoc(pAssocRet); }
void CMapStringToString::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& state = EnsureCStringMapStringStorage(this); if (ar.IsStoring()) ar << state.data; else { ar >> state.data; state.MarkDirty(); } }
extern "C" CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEAAPEAUCPair_1_PEBU21__Z(CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return impl__PGetNextAssoc_CMapStringToString__QEAAPEAUCPair_1_PEBU21___Z(pThis, pAssoc); }
extern "C" const CMapStringToString::CPair* MS_ABI impl__PGetNextAssoc_CMapStringToString__QEBAPEBUCPair_1_PEBU21__Z(const CMapStringToString* pThis, const CMapStringToString::CPair* pAssoc) { return impl__PGetNextAssoc_CMapStringToString__QEBAPEBUCPair_1_PEBU21___Z(pThis, pAssoc); }
