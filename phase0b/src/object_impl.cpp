#include "afx.h"
#include <cstdlib>

extern "C" {
    void* Impl_CObject_New(size_t nSize);
    void  Impl_CObject_Delete(void* p);
    CRuntimeClass* Impl_CObject_GetRuntimeClass(const CObject* self);
    BOOL Impl_CObject_IsKindOf(const CObject* self, const CRuntimeClass* cls);

    // Constructor from assembly (sets vptr)
    CObject* CObject_ctor(CObject* self) asm("CObject_ctor");

    // Factory used by CRuntimeClass
    void* CreateCObjectInstance();
}

static CRuntimeClass classCObject = {
    "CObject",
    static_cast<int>(sizeof(void*)), // only vptr
    0xFFFF,
    reinterpret_cast<CObject* (AFX_PASCAL *)()>(&CreateCObjectInstance),
    nullptr
};

extern "C" void* CreateCObjectInstance() {
    void* mem = Impl_CObject_New(sizeof(CObject));
    if (!mem) return nullptr;
    return CObject_ctor(static_cast<CObject*>(mem));
}

extern "C" void* Impl_CObject_New(size_t nSize) {
    return std::malloc(nSize);
}

extern "C" void Impl_CObject_Delete(void* p) {
    if (p) {
        std::free(p);
    }
}

extern "C" CRuntimeClass* Impl_CObject_GetRuntimeClass(const CObject* /*self*/) {
    return &classCObject;
}

extern "C" BOOL Impl_CObject_IsKindOf(const CObject* self, const CRuntimeClass* cls) {
    const CRuntimeClass* current = self ? self->GetRuntimeClass() : nullptr;
    while (current) {
        if (current == cls) {
            return TRUE;
        }
        current = current->m_pBaseClass;
    }
    return FALSE;
}

CObject* AFX_PASCAL CObject::CreateObject() {
    return static_cast<CObject*>(CreateCObjectInstance());
}
