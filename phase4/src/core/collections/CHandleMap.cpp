// Native release-x64 temporary handle ownership (mfc140u 0x2a5fb0..0x2a6207).
#include "detail/CHandleMapSupport.h"
#include "detail/ModuleThreadStateSupport.h"
#include <malloc.h>
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
namespace openmfc {
namespace detail {
namespace modulethreadstate {
int MS_ABI HandleAllocationFailure(size_t size) {
    auto* thread = AfxGetThreadState();
    if (!thread || !thread->m_pSafetyPoolBuffer)
        impl__AfxThrowMemoryException__YAXXZ();
    size_t capacity = ::_msize(thread->m_pSafetyPoolBuffer);
    if (capacity <= size + size_t(4)) {
        std::free(thread->m_pSafetyPoolBuffer);
        thread->m_pSafetyPoolBuffer = nullptr;
    } else {
        ::_expand(thread->m_pSafetyPoolBuffer, capacity - size - 4);
    }
    return TRUE;
}
} // namespace modulethreadstate
} // namespace detail
} // namespace openmfc
CHandleMap::CHandleMap(CRuntimeClass* type, ObjectCallback construct, ObjectCallback destruct,
                       size_t offset, int handles)
    : CHandleMapAllocation(type), m_permanentMap(10), m_temporaryMap(4) {
    if (!construct || !destruct)
        impl__AfxThrowInvalidArgException__YAXXZ();
    m_temporaryMap.m_nHashTableSize = 7;
    m_pClass = type;
    m_pfnConstruct = construct;
    m_pfnDestruct = destruct;
    m_nOffset = offset;
    m_nHandles = static_cast<unsigned>(handles);
}
CHandleMap::~CHandleMap() { DeleteTemp(); }
CObject* CHandleMap::FromHandle(void* handle) {
    if (!handle)
        return nullptr;
    auto* object = static_cast<CObject*>(m_permanentMap.GetValueAt(handle));
    if (object)
        return object;
    object = static_cast<CObject*>(m_temporaryMap.GetValueAt(handle));
    if (!object) {
        auto* thread = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
        auto previous = thread->m_pfnNewHandler;
        thread->m_pfnNewHandler = &openmfc::detail::modulethreadstate::HandleAllocationFailure;
        // The normal restoration lies outside the native typed catch region.
        // Foreign exceptions leave the pressure handler installed.
        try {
            object = static_cast<CObject*>(openmfc::detail::cfixedalloc::fa_alloc(this));
            if (!object)
                impl__AfxThrowMemoryException__YAXXZ();
            m_pfnConstruct(object);
            m_temporaryMap[handle] = object;
        } catch (CException*) {
            impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(previous);
            throw;
        }
        impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pfnNewHandler =
            previous;
    }
    auto* fields = reinterpret_cast<void**>(reinterpret_cast<BYTE*>(object) + m_nOffset);
    fields[0] = handle;
    if (m_nHandles == 2)
        fields[1] = handle;
    return object;
}
void CHandleMap::DeleteTemp() {
    auto position = m_temporaryMap.GetStartPosition();
    while (position) {
        void* key;
        void* object;
        m_temporaryMap.GetNextAssoc(position, key, object);
        auto* fields = reinterpret_cast<void**>(static_cast<BYTE*>(object) + m_nOffset);
        fields[0] = nullptr;
        if (m_nHandles == 2)
            fields[1] = nullptr;
        m_pfnDestruct(static_cast<CObject*>(object));
    }
    m_temporaryMap.RemoveAll();
    openmfc::detail::cfixedalloc::fa_free_all(this);
}
// Symbol: ??0CHandleMap@@QEAA@PEAUCRuntimeClass@@P6AXPEAVCObject@@@Z2_KH@Z
extern "C" CHandleMap* MS_ABI impl___0CHandleMap__QEAA_PEAUCRuntimeClass__P6AXPEAVCObject___Z2_KH_Z(
    CHandleMap* self, CRuntimeClass* type, CHandleMap::ObjectCallback construct,
    CHandleMap::ObjectCallback destruct, size_t offset, int handles) {
    return ::new (self) CHandleMap(type, construct, destruct, offset, handles);
}
// Symbol: ?FromHandle@CHandleMap@@QEAAPEAVCObject@@PEAX@Z
extern "C" CObject* MS_ABI impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(CHandleMap* self,
                                                                                void* handle) {
    return self->FromHandle(handle);
}
// Symbol: ?DeleteTemp@CHandleMap@@QEAAXXZ
extern "C" void MS_ABI impl__DeleteTemp_CHandleMap__QEAAXXZ(CHandleMap* self) {
    if (self)
        self->DeleteTemp();
}
