#include "CRuntimeClassSupport.h"

namespace openmfc { namespace detail { namespace cruntimeclass {
std::vector<CRuntimeClass*>& GetClassRegistry() {
    static std::vector<CRuntimeClass*> registry;
    return registry;
}
CRuntimeClass*& GetClassRegistryHead() {
    static CRuntimeClass* head = nullptr;
    return head;
}
void RegisterRuntimeClass(CRuntimeClass* pClass) {
    if (pClass) {
        auto& registry = GetClassRegistry();
        // Avoid duplicates
        for (auto* c : registry) {
            if (c == pClass) return;
        }
        pClass->m_pNextClass = GetClassRegistryHead();
        GetClassRegistryHead() = pClass;
        registry.push_back(pClass);
    }
}
CRuntimeClass* FindRuntimeClass(const char* lpszClassName) {
    if (!lpszClassName) return nullptr;

    auto& registry = GetClassRegistry();
    for (auto* pClass : registry) {
        if (pClass && pClass->m_lpszClassName) {
            if (strcmp(pClass->m_lpszClassName, lpszClassName) == 0) {
                return pClass;
            }
        }
    }
    return nullptr;
}
bool g_classesInitialized = false;
void InitializeClasses() {
    if (g_classesInitialized) return;
    RegisterRuntimeClass(&CObject::classCObject);
    RegisterRuntimeClass(&CException::classCException);
    RegisterRuntimeClass(&CMemoryException::classCMemoryException);
    RegisterRuntimeClass(&CFileException::classCFileException);
    RegisterRuntimeClass(&CArchiveException::classCArchiveException);
    RegisterRuntimeClass(&CCmdTarget::classCCmdTarget);
    RegisterRuntimeClass(&CWinThread::classCWinThread);
    RegisterRuntimeClass(&CWinApp::classCWinApp);
    RegisterRuntimeClass(&CWnd::classCWnd);
    RegisterRuntimeClass(&CFrameWnd::classCFrameWnd);
    g_classesInitialized = true;
}
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
// Symbol: ?AfxClassInit@@YAXPEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__AfxClassInit__YAXPEAUCRuntimeClass___Z(
    CRuntimeClass* pNewClass  // RCX = class to register
) {
    RegisterRuntimeClass(pNewClass);
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
