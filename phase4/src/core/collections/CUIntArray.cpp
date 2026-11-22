#define OPENMFC_APPCORE_IMPL
#include "detail/PlainArrayStorage.h"

static_assert(sizeof(CUIntArray) == 40, "native CUIntArray layout");
IMPLEMENT_DYNAMIC(CUIntArray, CObject)

// Symbol: ?GetThisClass@CUIntArray@@SAPEAUCRuntimeClass@@XZ
CRuntimeClass* CUIntArray::GetThisClass() { return &classCUIntArray; }
// Symbol: ?GetRuntimeClass@CUIntArray@@UEBAPEAUCRuntimeClass@@XZ
CRuntimeClass* CUIntArray::GetRuntimeClass() const { return GetThisClass(); }

using UIntArrayStorage = openmfc::detail::PlainArrayStorage<CUIntArray, UINT>;

// Symbol: ??0CUIntArray@@QEAA@XZ
CUIntArray::CUIntArray() : m_pData(nullptr), m_nSize(0), m_nMaxSize(0), m_nGrowBy(0) {}
// Symbol: ??1CUIntArray@@UEAA@XZ
CUIntArray::~CUIntArray() { free(m_pData); }

// Symbol: ?SetSize@CUIntArray@@QEAAX_J0@Z
void CUIntArray::SetSize(INT_PTR size, INT_PTR grow) { UIntArrayStorage::SetSize(*this, size, grow); }
// Symbol: ?FreeExtra@CUIntArray@@QEAAXXZ
void CUIntArray::FreeExtra() { UIntArrayStorage::FreeExtra(*this); }
// Symbol: ?SetAtGrow@CUIntArray@@QEAAX_JI@Z
void CUIntArray::SetAtGrow(INT_PTR index, UINT value) { UIntArrayStorage::SetAtGrow(*this, index, value); }
// Symbol: ?Append@CUIntArray@@QEAA_JAEBV1@@Z
INT_PTR CUIntArray::Append(const CUIntArray& source) { return UIntArrayStorage::Append(*this, source); }
// Symbol: ?Copy@CUIntArray@@QEAAXAEBV1@@Z
void CUIntArray::Copy(const CUIntArray& source) { UIntArrayStorage::Copy(*this, source); }
// Symbol: ?InsertAt@CUIntArray@@QEAAX_JI0@Z
void CUIntArray::InsertAt(INT_PTR index, UINT value, INT_PTR count) {
    UIntArrayStorage::InsertAt(*this, index, value, count);
}
// Symbol: ?InsertAt@CUIntArray@@QEAAX_JPEAV1@@Z
void CUIntArray::InsertAt(INT_PTR index, CUIntArray* source) { UIntArrayStorage::InsertAt(*this, index, source); }
// Symbol: ?RemoveAt@CUIntArray@@QEAAX_J0@Z
void CUIntArray::RemoveAt(INT_PTR index, INT_PTR count) { UIntArrayStorage::RemoveAt(*this, index, count); }
