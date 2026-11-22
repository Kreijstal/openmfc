#define OPENMFC_APPCORE_IMPL
#include "detail/PlainArrayStorage.h"
#include <new>

static_assert(sizeof(CDWordArray) == 40, "native CDWordArray layout");
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDWordArray@@@Z
IMPLEMENT_SERIAL(CDWordArray, CObject, 0)

// Symbol: ?GetThisClass@CDWordArray@@SAPEAUCRuntimeClass@@XZ
CRuntimeClass* CDWordArray::GetThisClass() { return &classCDWordArray; }
// Symbol: ?GetRuntimeClass@CDWordArray@@UEBAPEAUCRuntimeClass@@XZ
CRuntimeClass* CDWordArray::GetRuntimeClass() const { return GetThisClass(); }

using DWordArrayStorage = openmfc::detail::PlainArrayStorage<CDWordArray, DWORD>;

// Symbol: ??0CDWordArray@@QEAA@XZ
CDWordArray::CDWordArray() : m_pData(nullptr), m_nSize(0), m_nMaxSize(0), m_nGrowBy(0) {}
// Symbol: ??1CDWordArray@@UEAA@XZ
CDWordArray::~CDWordArray() { free(m_pData); }

// Symbol: ?SetSize@CDWordArray@@QEAAX_J0@Z
void CDWordArray::SetSize(INT_PTR size, INT_PTR grow) { DWordArrayStorage::SetSize(*this, size, grow); }
// Symbol: ?FreeExtra@CDWordArray@@QEAAXXZ
void CDWordArray::FreeExtra() { DWordArrayStorage::FreeExtra(*this); }
// Symbol: ?SetAtGrow@CDWordArray@@QEAAX_JK@Z
void CDWordArray::SetAtGrow(INT_PTR index, DWORD value) { DWordArrayStorage::SetAtGrow(*this, index, value); }
// Symbol: ?Append@CDWordArray@@QEAA_JAEBV1@@Z
INT_PTR CDWordArray::Append(const CDWordArray& source) { return DWordArrayStorage::Append(*this, source); }
// Symbol: ?Copy@CDWordArray@@QEAAXAEBV1@@Z
void CDWordArray::Copy(const CDWordArray& source) { DWordArrayStorage::Copy(*this, source); }
// Symbol: ?InsertAt@CDWordArray@@QEAAX_JK0@Z
void CDWordArray::InsertAt(INT_PTR index, DWORD value, INT_PTR count) {
    DWordArrayStorage::InsertAt(*this, index, value, count);
}
// Symbol: ?InsertAt@CDWordArray@@QEAAX_JPEAV1@@Z
void CDWordArray::InsertAt(INT_PTR index, CDWordArray* source) { DWordArrayStorage::InsertAt(*this, index, source); }
// Symbol: ?RemoveAt@CDWordArray@@QEAAX_J0@Z
void CDWordArray::RemoveAt(INT_PTR index, INT_PTR count) { DWordArrayStorage::RemoveAt(*this, index, count); }

// Symbol: ?CreateObject@CDWordArray@@SAPEAVCObject@@XZ
CObject* AFXAPI CDWordArray::CreateObject() {
    void* memory = impl___2_YAPEAX_K_Z(sizeof(CDWordArray));
    return memory ? ::new (memory) CDWordArray : nullptr;
}

// Symbol: ?Serialize@CDWordArray@@UEAAXAEAVCArchive@@@Z
void CDWordArray::Serialize(CArchive& ar) {
    DWordArrayStorage::Serialize(*this, ar);
}

// Internal callers use the same array lifetime and mutation entry points.
extern "C" void* impl___0CDWordArray__QEAA_XZ(CDWordArray* array) {
    return ::new (array) CDWordArray;
}
extern "C" void impl___1CDWordArray__UEAA_XZ(CDWordArray* array) {
    array->CDWordArray::~CDWordArray();
}
extern "C" void impl__SetAtGrow_CDWordArray__QEAAX_JK_Z(
    CDWordArray* array, INT_PTR index, DWORD value) {
    array->SetAtGrow(index, value);
}
extern "C" INT_PTR impl__Append_CDWordArray__QEAA_JAEBV1__Z(
    CDWordArray* array, const CDWordArray* source) {
    return array->Append(*source);
}
