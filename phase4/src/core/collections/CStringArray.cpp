// CStringArray — OpenMFC implementation.
// Sources: filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

#define OPENMFC_WRAP_CTOR0(fn_name, class_name) \
extern "C" void* MS_ABI fn_name(class_name* pThis) { \
    return pThis ? new (pThis) class_name() : nullptr; \
}
// Symbol: ??0CStringArray@@QEAA@XZ
OPENMFC_WRAP_CTOR0(impl___0CStringArray__QEAA_XZ, CStringArray)
#undef OPENMFC_WRAP_CTOR0
#define OPENMFC_WRAP_DTOR(fn_name, class_name) \
extern "C" void MS_ABI fn_name(class_name* pThis) { \
    if (pThis) pThis->~class_name(); \
}
// Symbol: ??1CStringArray@@UEAA@XZ
OPENMFC_WRAP_DTOR(impl___1CStringArray__UEAA_XZ, CStringArray)
#undef OPENMFC_WRAP_DTOR
// Symbol: ?Append@CStringArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CStringArray__QEAA_JAEBV1__Z(CStringArray* pThis, const CStringArray* pSrc) { return (pThis && pSrc) ? pThis->Append(*pSrc) : 0; }
// Symbol: ?Copy@CStringArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CStringArray__QEAAXAEBV1__Z(CStringArray* pThis, const CStringArray* pSrc) { if (pThis && pSrc) pThis->Copy(*pSrc); }
#define OPENMFC_WRAP_CREATEOBJECT(fn_name, class_name) \
extern "C" CObject* MS_ABI fn_name() { \
    return class_name::CreateObject(); \
}
// Symbol: ?CreateObject@CStringArray@@SAPEAVCObject@@XZ
OPENMFC_WRAP_CREATEOBJECT(impl__CreateObject_CStringArray__SAPEAVCObject__XZ, CStringArray)
#undef OPENMFC_WRAP_CREATEOBJECT
// Symbol: ?FreeExtra@CStringArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CStringArray__QEAAXXZ(CStringArray* pThis) { if (pThis) pThis->FreeExtra(); }
#define OPENMFC_WRAP_GETRUNTIMECLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name(const class_name* pThis) { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetRuntimeClass@CStringArray@@UEBAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETRUNTIMECLASS(impl__GetRuntimeClass_CStringArray__UEBAPEAUCRuntimeClass__XZ, CStringArray)
#undef OPENMFC_WRAP_GETRUNTIMECLASS
#define OPENMFC_WRAP_GETTHISCLASS(fn_name, class_name) \
extern "C" CRuntimeClass* MS_ABI fn_name() { \
    return class_name::GetThisClass(); \
}
// Symbol: ?GetThisClass@CStringArray@@SAPEAUCRuntimeClass@@XZ
OPENMFC_WRAP_GETTHISCLASS(impl__GetThisClass_CStringArray__SAPEAUCRuntimeClass__XZ, CStringArray)
#undef OPENMFC_WRAP_GETTHISCLASS
// Symbol: ?InsertAt@CStringArray@@QEAAX_JAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@0@Z
extern "C" void MS_ABI impl__InsertAt_CStringArray__QEAAX_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__0_Z(CStringArray* pThis, long long nIndex, const CString& value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CStringArray@@QEAAX_JPEB_W0@Z
extern "C" void MS_ABI impl__InsertAt_CStringArray__QEAAX_JPEB_W0_Z(CStringArray* pThis, long long nIndex, const wchar_t* value, long long nCount) { if (pThis) pThis->InsertAt(nIndex, value, nCount); }
// Symbol: ?InsertAt@CStringArray@@QEAAX_JPEBV1@@Z
extern "C" void MS_ABI impl__InsertAt_CStringArray__QEAAX_JPEBV1__Z(CStringArray* pThis, long long nIndex, CStringArray* pNewArray) { if (pThis) pThis->InsertAt(nIndex, pNewArray); }
// Symbol: ?InsertEmpty@CStringArray@@IEAAX_J0@Z
extern "C" void MS_ABI impl__InsertEmpty_CStringArray__IEAAX_J0_Z(CStringArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->InsertEmpty(nIndex, nCount); }
// Symbol: ?RemoveAt@CStringArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CStringArray__QEAAX_J0_Z(CStringArray* pThis, long long nIndex, long long nCount) { if (pThis) pThis->RemoveAt(nIndex, nCount); }
// Symbol: ?Serialize@CStringArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CStringArray__UEAAXAEAVCArchive___Z(CStringArray* pThis, CArchive& ar) { if (pThis) pThis->Serialize(ar); }
// Symbol: ?SetAtGrow@CStringArray@@QEAAX_JAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetAtGrow_CStringArray__QEAAX_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CStringArray* pThis, long long nIndex, const CString& value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetAtGrow@CStringArray@@QEAAX_JPEB_W@Z
extern "C" void MS_ABI impl__SetAtGrow_CStringArray__QEAAX_JPEB_W_Z(CStringArray* pThis, long long nIndex, const wchar_t* value) { if (pThis) pThis->SetAtGrow(nIndex, value); }
// Symbol: ?SetSize@CStringArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CStringArray__QEAAX_J0_Z(CStringArray* pThis, long long nNewSize, long long nGrowBy) { if (pThis) pThis->SetSize(nNewSize, nGrowBy); }
CStringArray::CStringArray() {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    EnsureCollectionState<CStringArray, ArrayWrapperState<CString, const CString&>>(this);
}
CStringArray::~CStringArray() {
    std::lock_guard<std::mutex> lock(g_collectionStateMutex);
    RemoveCollectionState<CStringArray, ArrayWrapperState<CString, const CString&>>(this);
}
INT_PTR CStringArray::GetSize() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); const auto* data = FindArrayStorage<CStringArray, CString, const CString&>(this); return data ? data->GetSize() : 0; }
INT_PTR CStringArray::GetCount() const { return GetSize(); }
BOOL CStringArray::IsEmpty() const { return GetSize() == 0; }
INT_PTR CStringArray::GetUpperBound() const { return GetSize() - 1; }
void CStringArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).SetSize(ClampCollectionSize(nNewSize), ClampCollectionGrow(nGrowBy)); }
void CStringArray::FreeExtra() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).FreeExtra(); }
void CStringArray::RemoveAll() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).RemoveAll(); }
CString CStringArray::GetAt(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).GetAt(ClampCollectionSize(nIndex)); }
void CStringArray::SetAt(INT_PTR nIndex, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).SetAt(ClampCollectionSize(nIndex), newElement); }
CString& CStringArray::ElementAt(INT_PTR nIndex) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).ElementAt(ClampCollectionSize(nIndex)); }
const CString& CStringArray::ElementAt(INT_PTR nIndex) const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).ElementAt(ClampCollectionSize(nIndex)); }
CString CStringArray::operator[](INT_PTR nIndex) const { return GetAt(nIndex); }
CString& CStringArray::operator[](INT_PTR nIndex) { return ElementAt(nIndex); }
CString* CStringArray::GetData() { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).GetData(); }
const CString* CStringArray::GetData() const { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).GetData(); }
void CStringArray::SetAtGrow(INT_PTR nIndex, const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).SetAtGrow(ClampCollectionSize(nIndex), newElement); }
void CStringArray::SetAtGrow(INT_PTR nIndex, const wchar_t* newElement) { SetAtGrow(nIndex, NormalizeStringKey(newElement)); }
INT_PTR CStringArray::Add(const CString& newElement) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); return EnsureArrayStorage<CStringArray, CString, const CString&>(this).Add(newElement); }
INT_PTR CStringArray::Append(const CStringArray& src) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); const auto& srcData = EnsureArrayStorage<CStringArray, CString, const CString&>(&src); return data.Append(srcData); }
void CStringArray::Copy(const CStringArray& src) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); const auto& srcData = EnsureArrayStorage<CStringArray, CString, const CString&>(&src); data.Copy(srcData); }
void CStringArray::InsertAt(INT_PTR nIndex, const CString& newElement, INT_PTR nCount) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).InsertAt(ClampCollectionSize(nIndex), newElement, ClampCollectionSize(nCount)); }
void CStringArray::InsertAt(INT_PTR nIndex, const wchar_t* newElement, INT_PTR nCount) { InsertAt(nIndex, NormalizeStringKey(newElement), nCount); }
void CStringArray::RemoveAt(INT_PTR nIndex, INT_PTR nCount) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).RemoveAt(ClampCollectionSize(nIndex), ClampCollectionSize(nCount)); }
void CStringArray::InsertAt(INT_PTR nStartIndex, CStringArray* pNewArray) { if (!pNewArray) return; std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); auto& srcData = EnsureArrayStorage<CStringArray, CString, const CString&>(pNewArray); data.InsertAt(ClampCollectionSize(nStartIndex), &srcData); }
void CStringArray::Serialize(CArchive& ar) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); auto& data = EnsureArrayStorage<CStringArray, CString, const CString&>(this); if (ar.IsStoring()) ar << data; else ar >> data; }
void CStringArray::InsertEmpty(INT_PTR nIndex, INT_PTR nCount) { std::lock_guard<std::mutex> lock(g_collectionStateMutex); EnsureArrayStorage<CStringArray, CString, const CString&>(this).InsertAt(ClampCollectionSize(nIndex), CString(), ClampCollectionSize(nCount)); }
