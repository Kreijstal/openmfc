#define OPENMFC_APPCORE_IMPL
#include "detail/PlainArrayStorage.h"
#include <new>

static_assert(sizeof(CWordArray) == 40, "native CWordArray layout");
// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCWordArray@@@Z
IMPLEMENT_SERIAL(CWordArray, CObject, 0)

// Symbol: ?GetThisClass@CWordArray@@SAPEAUCRuntimeClass@@XZ
CRuntimeClass* CWordArray::GetThisClass() { return &classCWordArray; }
// Symbol: ?GetRuntimeClass@CWordArray@@UEBAPEAUCRuntimeClass@@XZ
CRuntimeClass* CWordArray::GetRuntimeClass() const { return GetThisClass(); }

using WordArrayStorage = openmfc::detail::PlainArrayStorage<CWordArray, WORD>;

// Symbol: ??0CWordArray@@QEAA@XZ
CWordArray::CWordArray() : m_pData(nullptr), m_nSize(0), m_nMaxSize(0), m_nGrowBy(0) {}
// Symbol: ??1CWordArray@@UEAA@XZ
CWordArray::~CWordArray() { free(m_pData); }

// Symbol: ?SetSize@CWordArray@@QEAAX_J0@Z
void CWordArray::SetSize(INT_PTR size, INT_PTR grow) { WordArrayStorage::SetSize(*this, size, grow); }
// Symbol: ?FreeExtra@CWordArray@@QEAAXXZ
void CWordArray::FreeExtra() { WordArrayStorage::FreeExtra(*this); }
// Symbol: ?SetAtGrow@CWordArray@@QEAAX_JG@Z
void CWordArray::SetAtGrow(INT_PTR index, WORD value) { WordArrayStorage::SetAtGrow(*this, index, value); }
// Symbol: ?Append@CWordArray@@QEAA_JAEBV1@@Z
INT_PTR CWordArray::Append(const CWordArray& source) { return WordArrayStorage::Append(*this, source); }
// Symbol: ?Copy@CWordArray@@QEAAXAEBV1@@Z
void CWordArray::Copy(const CWordArray& source) { WordArrayStorage::Copy(*this, source); }
// Symbol: ?InsertAt@CWordArray@@QEAAX_JG0@Z
void CWordArray::InsertAt(INT_PTR index, WORD value, INT_PTR count) {
    WordArrayStorage::InsertAt(*this, index, value, count);
}
// Symbol: ?InsertAt@CWordArray@@QEAAX_JPEAV1@@Z
void CWordArray::InsertAt(INT_PTR index, CWordArray* source) { WordArrayStorage::InsertAt(*this, index, source); }
// Symbol: ?RemoveAt@CWordArray@@QEAAX_J0@Z
void CWordArray::RemoveAt(INT_PTR index, INT_PTR count) { WordArrayStorage::RemoveAt(*this, index, count); }

// Symbol: ?CreateObject@CWordArray@@SAPEAVCObject@@XZ
CObject* AFXAPI CWordArray::CreateObject() {
    void* memory = impl___2_YAPEAX_K_Z(sizeof(CWordArray));
    return memory ? ::new (memory) CWordArray : nullptr;
}

// Symbol: ?Serialize@CWordArray@@UEAAXAEAVCArchive@@@Z
void CWordArray::Serialize(CArchive& ar) {
    WordArrayStorage::Serialize(*this, ar);
}
