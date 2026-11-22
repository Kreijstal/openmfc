#define OPENMFC_APPCORE_IMPL
#include "detail/PlainArrayStorage.h"

static_assert(sizeof(CPtrArray) == 40, "native CPtrArray layout");
IMPLEMENT_DYNAMIC(CPtrArray, CObject)

using Storage = openmfc::detail::PlainArrayStorage<CPtrArray, void*>;

CPtrArray::CPtrArray() : m_pData(nullptr), m_nSize(0), m_nMaxSize(0), m_nGrowBy(0) {}
CPtrArray::~CPtrArray() { free(m_pData); }

void CPtrArray::SetSize(INT_PTR size, INT_PTR grow) { Storage::SetSize(*this, size, grow); }
void CPtrArray::FreeExtra() { Storage::FreeExtra(*this); }
void CPtrArray::SetAtGrow(INT_PTR index, void* value) { Storage::SetAtGrow(*this, index, value); }
INT_PTR CPtrArray::Append(const CPtrArray& source) { return Storage::Append(*this, source); }
void CPtrArray::Copy(const CPtrArray& source) { Storage::Copy(*this, source); }
void CPtrArray::InsertAt(INT_PTR index, void* value, INT_PTR count) {
    Storage::InsertAt(*this, index, value, count);
}
void CPtrArray::InsertAt(INT_PTR index, CPtrArray* source) { Storage::InsertAt(*this, index, source); }
void CPtrArray::RemoveAt(INT_PTR index, INT_PTR count) { Storage::RemoveAt(*this, index, count); }
