#include "afx.h"
#include <cstdlib>

extern "C" {
    void* Impl_CObject_New(size_t nSize);
    void  Impl_CObject_Delete(void* p);
    CRuntimeClass* Impl_CObject_GetRuntimeClass(const CObject* self);
    BOOL Impl_CObject_IsKindOf(const CObject* self, const CRuntimeClass* cls);
    struct CArchive;
    void Impl_CObject_Serialize(CObject* self, CArchive& ar);

    // Constructor from assembly (sets vptr)
    CObject* CObject_ctor(CObject* self) asm("CObject_ctor");
}

static CRuntimeClass classCObject = {
    "CObject",
    8, // harvested layout size: vptr only
    0xFFFF,
    nullptr, // real MFC: not dyncreate; CreateObject returns nullptr
    nullptr
};

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
    // Manually dispatch virtual GetRuntimeClass using MSVC slot layout.
    const CRuntimeClass* current = nullptr;
    if (self) {
        void** vptr = *reinterpret_cast<void***>(const_cast<CObject*>(self));
        using GetClassFn = CRuntimeClass* (*)(const CObject*);
        auto fn = reinterpret_cast<GetClassFn>(vptr[1]);
        current = fn(self);
    }
    while (current) {
        if (current == cls) {
            return TRUE;
        }
        current = current->m_pBaseClass;
    }
    return FALSE;
}

extern "C" void Impl_CObject_Serialize(CObject* /*self*/, CArchive& /*ar*/) {
    // Default stub: no-op
}

CObject* AFX_PASCAL CObject::CreateObject() {
    // Match MFC: CObject is not dyncreate.
    return nullptr;
}
