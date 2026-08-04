// CWordArray — OpenMFC implementation.
// Sources: collections_cwordarray.cpp, filecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWordArraySupport.h"
#include "detail/FilecoreSupport.h"

// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ??0CWordArray@@QEAA@XZ
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ??1CWordArray@@UEAA@XZ
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?Append@CWordArray@@QEAA_JAEBV1@@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?Copy@CWordArray@@QEAAXAEBV1@@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?CreateObject@CWordArray@@SAPEAVCObject@@XZ
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?FreeExtra@CWordArray@@QEAAXXZ
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CWordArray@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CWordArray@@SAPEAUCRuntimeClass@@XZ
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?InsertAt@CWordArray@@QEAAX_JG0@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?InsertAt@CWordArray@@QEAAX_JPEAV1@@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?RemoveAt@CWordArray@@QEAAX_J0@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?Serialize@CWordArray@@UEAAXAEAVCArchive@@@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetAtGrow@CWordArray@@QEAAX_JG@Z
// Implemented in filecore.cpp; this unit had a second, unlinked copy.
// Symbol: ?SetSize@CWordArray@@QEAAX_J0@Z
// Symbol: ??0CWordArray@@QEAA@XZ
extern "C" CWordArray* MS_ABI impl___0CWordArray__QEAA_XZ(CWordArray* pThis) {
    return pThis ? new (pThis) CWordArray() : nullptr;
}
// Symbol: ??1CWordArray@@UEAA@XZ
extern "C" void MS_ABI impl___1CWordArray__UEAA_XZ(CWordArray* pThis) {
    if (pThis) pThis->~CWordArray();
}
// Symbol: ?Append@CWordArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CWordArray__QEAA_JAEBV1__Z(CWordArray* pThis, const CWordArray* pSrc) {
    return (pThis && pSrc) ? pThis->Append(*pSrc) : 0;
}
// Symbol: ?Copy@CWordArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CWordArray__QEAAXAEBV1__Z(CWordArray* pThis, const CWordArray* pSrc) {
    if (pThis && pSrc) pThis->Copy(*pSrc);
}
// Symbol: ?CreateObject@CWordArray@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CWordArray__SAPEAVCObject__XZ() {
    return reinterpret_cast<CObject*>(new CWordArray());
}
// Symbol: ?FreeExtra@CWordArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CWordArray__QEAAXXZ(CWordArray* pThis) {
    if (pThis) pThis->FreeExtra();
}
// Symbol: ?GetRuntimeClass@CWordArray@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWordArray__UEBAPEAUCRuntimeClass__XZ(const CWordArray* pThis) {
    (void)pThis;
    return &g_classCWordArray;
}
// Symbol: ?GetThisClass@CWordArray@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWordArray__SAPEAUCRuntimeClass__XZ() {
    return &g_classCWordArray;
}
// Symbol: ?InsertAt@CWordArray@@QEAAX_JG0@Z
extern "C" void MS_ABI impl__InsertAt_CWordArray__QEAAX_JG0_Z(CWordArray* pThis, long long nIndex, unsigned short value, long long nCount) {
    if (pThis) pThis->InsertAt((int)nIndex, value, (int)nCount);
}
// Symbol: ?InsertAt@CWordArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CWordArray__QEAAX_JPEAV1__Z(CWordArray* pThis, long long nIndex, CWordArray* pNewArray) {
    if (pThis && pNewArray) pThis->InsertAt((int)nIndex, pNewArray);
}
// Symbol: ?RemoveAt@CWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CWordArray__QEAAX_J0_Z(CWordArray* pThis, long long nIndex, long long nCount) {
    if (pThis) pThis->RemoveAt((int)nIndex, (int)nCount);
}
// Symbol: ?Serialize@CWordArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CWordArray__UEAAXAEAVCArchive___Z(CWordArray* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    if (ar->IsStoring()) {
        unsigned int count = (unsigned int)pThis->GetSize();
        *ar << count;
        for (int i = 0; i < pThis->GetSize(); ++i) {
            unsigned short value = pThis->GetAt(i);
            *ar << value;
        }
    } else {
        unsigned int count = 0;
        *ar >> count;
        pThis->SetSize((int)count);
        for (unsigned int i = 0; i < count; ++i) {
            unsigned short value = 0;
            *ar >> value;
            pThis->SetAt((int)i, value);
        }
    }
}
// Symbol: ?SetAtGrow@CWordArray@@QEAAX_JG@Z
extern "C" void MS_ABI impl__SetAtGrow_CWordArray__QEAAX_JG_Z(CWordArray* pThis, long long nIndex, unsigned short value) {
    if (pThis) pThis->SetAtGrow((int)nIndex, value);
}
// Symbol: ?SetSize@CWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CWordArray__QEAAX_J0_Z(CWordArray* pThis, long long nNewSize, long long nGrowBy) {
    if (pThis) pThis->SetSize((int)nNewSize, (int)nGrowBy);
}
