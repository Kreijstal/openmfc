#include "CRuntimeClassSupport.h"

namespace openmfc { namespace detail { namespace cruntimeclass {
const CRuntimeClass* GetObjectRuntimeClass(const CObject* pObject) {
    if (!pObject) {
        return nullptr;
    }

    void* const* vtable = *reinterpret_cast<void* const* const*>(pObject);
    if (!vtable || !vtable[0]) {
        return nullptr;
    }

    using GetRuntimeClassFn = CRuntimeClass* (MS_ABI *)(const CObject*);
    GetRuntimeClassFn getRuntimeClass =
        reinterpret_cast<GetRuntimeClassFn>(vtable[0]);
    return getRuntimeClass(pObject);
}
extern "C" void MS_ABI impl__Serialize_CObject__UEAAXAEAVCArchive___Z(
    CObject* pThis,      // RCX = this pointer
    CArchive* ar         // RDX = archive
) {
    (void)pThis;
    (void)ar;
    // Base CObject::Serialize does nothing
}
// Symbol: ?AfxDynamicDownCast@@YAPEAVCObject@@PEAUCRuntimeClass@@PEAV1@@Z
extern "C" CObject* MS_ABI impl__AfxDynamicDownCast__YAPEAVCObject__PEAUCRuntimeClass__PEAV1__Z(
    CRuntimeClass* pClass,  // RCX = target class
    CObject* pObject        // RDX = object to cast
) {
    if (!pObject || !pClass) {
        return nullptr;
    }

    const CRuntimeClass* pObjectClass = GetObjectRuntimeClass(pObject);
    while (pObjectClass) {
        if (pObjectClass == pClass) {
            return pObject;
        }
        pObjectClass = pObjectClass->m_pfnGetBaseClass
            ? pObjectClass->m_pfnGetBaseClass()
            : nullptr;
    }

    return nullptr;
}
} } }  // namespace openmfc::detail::cruntimeclass
