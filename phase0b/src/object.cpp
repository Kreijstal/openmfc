#include "afx.h"
#include <cstdlib>

// Local runtime class instance for CObject.
static CObject* AFX_PASCAL CreateCObjectInstance();

static CRuntimeClass classCObject = {
    "CObject",
    static_cast<int>(sizeof(CObject)),
    0, // schema (unused for now)
    &CreateCObjectInstance,
    nullptr
};

static CObject* AFX_PASCAL CreateCObjectInstance() {
    return new CObject();
}

CObject::CObject() = default;
CObject::~CObject() = default;

CRuntimeClass* CObject::GetRuntimeClass() const {
    return &classCObject;
}

BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const {
    const CRuntimeClass* current = GetRuntimeClass();
    while (current) {
        if (current == pClass) {
            return TRUE;
        }
        current = current->m_pBaseClass;
    }
    return FALSE;
}

void* CObject::operator new(size_t nSize) {
    return std::malloc(nSize);
}

void CObject::operator delete(void* p) {
    std::free(p);
}

CObject* AFX_PASCAL CObject::CreateObject() {
    return new CObject();
}

