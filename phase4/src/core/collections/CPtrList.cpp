// CPtrList — OpenMFC implementation.
// Sources: collections_cptrlist.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CPtrListSupport.h"

// Symbol: ??0CPtrList@@QEAA@_J@Z
extern "C" OpenMFC_CPtrList* MS_ABI impl___0CPtrList__QEAA__J_Z(OpenMFC_CPtrList* pThis, INT_PTR nBlockSize) {
    if (pThis == nullptr) return nullptr;
    new (pThis) OpenMFC_CPtrList(nBlockSize);
    return pThis;
}
// Symbol: ??1CPtrList@@UEAA@XZ
extern "C" void MS_ABI impl___1CPtrList__UEAA_XZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return;
    pThis->~OpenMFC_CPtrList();
}
// Symbol: ?AddHead@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
extern "C" __POSITION* MS_ABI impl__AddHead_CPtrList__QEAAPEAU__POSITION__PEAX_Z(OpenMFC_CPtrList* pThis, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->AddHead(newElement);
}
// Symbol: ?AddHead@CPtrList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddHead_CPtrList__QEAAXPEAV1__Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList* pNewList) {
    if (pThis == nullptr) return;
    pThis->AddHead(pNewList);
}
// Symbol: ?AddTail@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
extern "C" __POSITION* MS_ABI impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(OpenMFC_CPtrList* pThis, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->AddTail(newElement);
}
// Symbol: ?AddTail@CPtrList@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddTail_CPtrList__QEAAXPEAV1__Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList* pNewList) {
    if (pThis == nullptr) return;
    pThis->AddTail(pNewList);
}
// Symbol: ?Find@CPtrList@@QEBAPEAU__POSITION@@PEAXPEAU2@@Z
extern "C" __POSITION* MS_ABI impl__Find_CPtrList__QEBAPEAU__POSITION__PEAXPEAU2__Z(const OpenMFC_CPtrList* pThis, void* searchValue, __POSITION* startAfter) {
    if (pThis == nullptr) return nullptr;
    return pThis->Find(searchValue, startAfter);
}
// Symbol: ?FindIndex@CPtrList@@QEBAPEAU__POSITION@@_J@Z
extern "C" __POSITION* MS_ABI impl__FindIndex_CPtrList__QEBAPEAU__POSITION___J_Z(const OpenMFC_CPtrList* pThis, INT_PTR nIndex) {
    if (pThis == nullptr) return nullptr;
    return pThis->FindIndex(nIndex);
}
// Symbol: ?FreeNode@CPtrList@@IEAAXPEAUCNode@1@@Z
extern "C" void MS_ABI impl__FreeNode_CPtrList__IEAAXPEAUCNode_1__Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList::CNode* pNode) {
    if (pThis == nullptr) return;
    pThis->FreeNode(pNode);
}
// Symbol: ?GetRuntimeClass@CPtrList@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPtrList__UEBAPEAUCRuntimeClass__XZ(const OpenMFC_CPtrList* pThis) {
    return OpenMFC_CPtrList::GetThisClass();
}
// Symbol: ?GetThisClass@CPtrList@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPtrList__SAPEAUCRuntimeClass__XZ() {
    return OpenMFC_CPtrList::GetThisClass();
}
// Symbol: ?InsertAfter@CPtrList@@QEAAPEAU__POSITION@@PEAU2@PEAX@Z
extern "C" __POSITION* MS_ABI impl__InsertAfter_CPtrList__QEAAPEAU__POSITION__PEAU2_PEAX_Z(OpenMFC_CPtrList* pThis, __POSITION* position, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->InsertAfter(position, newElement);
}
// Symbol: ?InsertBefore@CPtrList@@QEAAPEAU__POSITION@@PEAU2@PEAX@Z
extern "C" __POSITION* MS_ABI impl__InsertBefore_CPtrList__QEAAPEAU__POSITION__PEAU2_PEAX_Z(OpenMFC_CPtrList* pThis, __POSITION* position, void* newElement) {
    if (pThis == nullptr) return nullptr;
    return pThis->InsertBefore(position, newElement);
}
// Symbol: ?NewNode@CPtrList@@IEAAPEAUCNode@1@PEAU21@0@Z
extern "C" OpenMFC_CPtrList::CNode* MS_ABI impl__NewNode_CPtrList__IEAAPEAUCNode_1_PEAU21_0_Z(OpenMFC_CPtrList* pThis, OpenMFC_CPtrList::CNode* pPrev, OpenMFC_CPtrList::CNode* pNext) {
    if (pThis == nullptr) return nullptr;
    return pThis->NewNode(pPrev, pNext);
}
// Symbol: ?RemoveAll@CPtrList@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CPtrList__QEAAXXZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return;
    pThis->RemoveAll();
}
// Symbol: ?RemoveAt@CPtrList@@QEAAXPEAU__POSITION@@@Z
extern "C" void MS_ABI impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(OpenMFC_CPtrList* pThis, __POSITION* position) {
    if (pThis == nullptr) return;
    pThis->RemoveAt(position);
}
// Symbol: ?RemoveHead@CPtrList@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__RemoveHead_CPtrList__QEAAPEAXXZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return nullptr;
    return pThis->RemoveHead();
}
// Symbol: ?RemoveTail@CPtrList@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__RemoveTail_CPtrList__QEAAPEAXXZ(OpenMFC_CPtrList* pThis) {
    if (pThis == nullptr) return nullptr;
    return pThis->RemoveTail();
}
