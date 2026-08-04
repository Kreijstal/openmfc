// CStringList — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR1(fn_name, class_name, arg_type) extern "C" void* MS_ABI fn_name(class_name* pThis, arg_type arg0) { return pThis ? new (pThis) class_name(arg0) : nullptr; }
// Symbol: ??0CStringList@@QEAA@_J@Z
OPENMFC_WRAP_CTOR1(impl___0CStringList__QEAA__J_Z, CStringList, long long)
#undef OPENMFC_WRAP_CTOR1
#define OPENMFC_WRAP_DTOR(fn_name, class_name) extern "C" void MS_ABI fn_name(class_name* pThis) { if (pThis) pThis->~class_name(); }
// Symbol: ??1CStringList@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CStringList__UEAA_XZ, CStringList)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?AddHead@CStringList@@QEAAPEAU__POSITION@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__AddHead_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, const CString& value) { return pThis ? pThis->AddHead(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddHead@CStringList@@QEAAPEAU__POSITION@@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__AddHead_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CStringList* pThis, const wchar_t* value) { return pThis ? pThis->AddHead(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddHead@CStringList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddHead_CStringList__QEAAXPEAV1__Z(CStringList* pThis, CStringList* pNewList) { if (pThis) pThis->AddHead(pNewList); }
// Symbol: ?AddTail@CStringList@@QEAAPEAU__POSITION@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, const CString& value) { return pThis ? pThis->AddTail(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddTail@CStringList@@QEAAPEAU__POSITION@@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CStringList* pThis, const wchar_t* value) { return pThis ? pThis->AddTail(value) : CStringList::POSITION(nullptr); }
// Symbol: ?AddTail@CStringList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddTail_CStringList__QEAAXPEAV1__Z(CStringList* pThis, CStringList* pNewList) { if (pThis) pThis->AddTail(pNewList); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) extern "C" CObject* MS_ABI fn_name() { return class_name::CreateObject(); }
// Symbol: ?CreateObject@CStringList@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CStringList__SAPEAVCObject__XZ, CStringList)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?Find@CStringList@@QEBAPEAU__POSITION@@PEB_WPEAU2@@Z
extern "C" CStringList::POSITION MS_ABI impl__Find_CStringList__QEBAPEAU__POSITION__PEB_WPEAU2__Z(const CStringList* pThis, const wchar_t* value, CStringList::POSITION* pStartAfter) { return pThis ? pThis->Find(value, pStartAfter ? *pStartAfter : CStringList::POSITION(nullptr)) : CStringList::POSITION(nullptr); }
// Symbol: ?FindIndex@CStringList@@QEBAPEAU__POSITION@@_J@Z
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const CStringList* pThis, long long nIndex) { return pThis ? pThis->FindIndex(nIndex) : CStringList::POSITION(nullptr); }
// Symbol: ?FreeNode@CStringList@@IEAAXPEAUCNode@1@@Z
extern "C" void MS_ABI impl__FreeNode_CStringList__IEAAXPEAUCNode_1___Z(CStringList* /*pThis*/, void* pNode) { delete static_cast<ListNodeSnapshot<CString>*>(pNode); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { return class_name::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CStringList@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CStringList__UEBAPEAUCRuntimeClass__XZ, CStringList)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) extern "C" CRuntimeClass* MS_ABI fn_name() { return class_name::GetThisClass(); }
// Symbol: ?GetThisClass@CStringList@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CStringList__SAPEAUCRuntimeClass__XZ, CStringList)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAfter@CStringList@@QEAAPEAU__POSITION@@PEAU2@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertAfter_CStringList__QEAAPEAU__POSITION__PEAU2_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, CStringList::POSITION* pPos, const CString& value) { return pThis ? pThis->InsertAfter(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?InsertAfter@CStringList@@QEAAPEAU__POSITION@@PEAU2@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertAfter_CStringList__QEAAPEAU__POSITION__PEAU2_PEB_W_Z(CStringList* pThis, CStringList::POSITION* pPos, const wchar_t* value) { return pThis ? pThis->InsertAfter(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?InsertBefore@CStringList@@QEAAPEAU__POSITION@@PEAU2@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertBefore_CStringList__QEAAPEAU__POSITION__PEAU2_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringList* pThis, CStringList::POSITION* pPos, const CString& value) { return pThis ? pThis->InsertBefore(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?InsertBefore@CStringList@@QEAAPEAU__POSITION@@PEAU2@PEB_W@Z
extern "C" CStringList::POSITION MS_ABI impl__InsertBefore_CStringList__QEAAPEAU__POSITION__PEAU2_PEB_W_Z(CStringList* pThis, CStringList::POSITION* pPos, const wchar_t* value) { return pThis ? pThis->InsertBefore(pPos ? *pPos : CStringList::POSITION(nullptr), value) : CStringList::POSITION(nullptr); }
// Symbol: ?NewNode@CStringList@@IEAAPEAUCNode@1@PEAU21@0@Z
extern "C" void* MS_ABI impl__NewNode_CStringList__IEAAPEAUCNode_1__PEAU21_0_Z(CStringList* /*pThis*/, void* pPrev, void* pNext) { auto* node = new ListNodeSnapshot<CString>(); node->pPrev = static_cast<ListNodeSnapshot<CString>*>(pPrev); node->pNext = static_cast<ListNodeSnapshot<CString>*>(pNext); return node; }
// Symbol: ?RemoveAll@CStringList@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CStringList__QEAAXXZ(CStringList* pThis) { if (pThis) pThis->RemoveAll(); }
// Symbol: ?RemoveAt@CStringList@@QEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__RemoveAt_CStringList__QEAAXPEAU__POSITION___Z(CStringList* pThis, CStringList::POSITION* pPos) { if (pThis && pPos) pThis->RemoveAt(*pPos); }
// Symbol: ?RemoveHead@CStringList@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__RemoveHead_CStringList__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CStringList* pThis, void* ret) { new (ret) CString(pThis ? pThis->RemoveHead() : CString()); }
// Symbol: ?RemoveTail@CStringList@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__RemoveTail_CStringList__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CStringList* pThis, void* ret) { new (ret) CString(pThis ? pThis->RemoveTail() : CString()); }
// Symbol: ?Serialize@CStringList@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CStringList__UEAAXAEAVCArchive___Z(CStringList* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
CStringList::CStringList(INT_PTR nBlockSize) {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    EnsureCollectionState<CStringList, ListWrapperState<CString, const CString&>>(this, ClampCollectionBlockSize(nBlockSize));
}
CStringList::~CStringList() {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    RemoveCollectionState<CStringList, ListWrapperState<CString, const CString&>>(this);
}
INT_PTR CStringList::GetCount() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->GetCount() : 0; }
BOOL CStringList::IsEmpty() const { return GetCount() == 0; }
CString& CStringList::GetHead() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetHead(); }
CString CStringList::GetHead() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetHead(); }
CString& CStringList::GetTail() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetTail(); }
CString CStringList::GetTail() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetTail(); }
CStringList::POSITION CStringList::GetHeadPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->GetHeadPosition() : POSITION(nullptr); }
CStringList::POSITION CStringList::GetTailPosition() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->GetTailPosition() : POSITION(nullptr); }
CString& CStringList::GetNext(POSITION& rPosition) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetNext(rPosition); }
CString CStringList::GetNext(POSITION& rPosition) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetNext(rPosition); }
CString& CStringList::GetPrev(POSITION& rPosition) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetPrev(rPosition); }
CString CStringList::GetPrev(POSITION& rPosition) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetPrev(rPosition); }
CString CStringList::GetAt(POSITION position) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).GetAt(position); }
void CStringList::SetAt(POSITION pos, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<CStringList, CString, const CString&>(this).SetAt(pos, newElement); }
void CStringList::RemoveAt(POSITION position) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<CStringList, CString, const CString&>(this).RemoveAt(position); }
CStringList::POSITION CStringList::FindIndex(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->FindIndex(ClampCollectionSize(nIndex)) : POSITION(nullptr); }
CStringList::POSITION CStringList::Find(const CString& searchValue, POSITION startAfter) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindListStorage<CStringList, CString, const CString&>(this); return data ? data->Find(searchValue, startAfter) : POSITION(nullptr); }
CStringList::POSITION CStringList::Find(const wchar_t* searchValue, POSITION startAfter) const { return Find(NormalizeStringKey(searchValue), startAfter); }
CStringList::POSITION CStringList::AddHead(const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); data.AddHead(newElement); return data.GetHeadPosition(); }
CStringList::POSITION CStringList::AddHead(const wchar_t* newElement) { return AddHead(NormalizeStringKey(newElement)); }
void CStringList::AddHead(CStringList* pNewList) { if (!pNewList || pNewList == this) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); auto& srcData = EnsureListStorage<CStringList, CString, const CString&>(pNewList); data.AddHead(&srcData); }
CStringList::POSITION CStringList::AddTail(const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); data.AddTail(newElement); return data.GetTailPosition(); }
CStringList::POSITION CStringList::AddTail(const wchar_t* newElement) { return AddTail(NormalizeStringKey(newElement)); }
void CStringList::AddTail(CStringList* pNewList) { if (!pNewList || pNewList == this) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); auto& srcData = EnsureListStorage<CStringList, CString, const CString&>(pNewList); data.AddTail(&srcData); }
CString CStringList::RemoveHead() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).RemoveHead(); }
CString CStringList::RemoveTail() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureListStorage<CStringList, CString, const CString&>(this).RemoveTail(); }
CStringList::POSITION CStringList::InsertBefore(POSITION position, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); POSITION oldPos = position; data.InsertBefore(position, newElement); if (oldPos == POSITION(nullptr)) return data.GetTailPosition(); POSITION pos = data.GetHeadPosition(); POSITION prev(nullptr); while (pos != POSITION(nullptr) && pos != oldPos) { prev = pos; data.GetNext(pos); } return prev; }
CStringList::POSITION CStringList::InsertBefore(POSITION position, const wchar_t* newElement) { return InsertBefore(position, NormalizeStringKey(newElement)); }
CStringList::POSITION CStringList::InsertAfter(POSITION position, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); POSITION oldPos = position; data.InsertAfter(position, newElement); if (oldPos == POSITION(nullptr)) return data.GetHeadPosition(); POSITION result = oldPos; data.GetNext(result); return result; }
CStringList::POSITION CStringList::InsertAfter(POSITION position, const wchar_t* newElement) { return InsertAfter(position, NormalizeStringKey(newElement)); }
void CStringList::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureListStorage<CStringList, CString, const CString&>(this).RemoveAll(); }
void CStringList::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureListStorage<CStringList, CString, const CString&>(this); if (ar.IsStoring()) ar << data; else ar >> data; }
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION___J_Z(const CStringList* pThis, long long nIndex) { return impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(pThis, nIndex); }
