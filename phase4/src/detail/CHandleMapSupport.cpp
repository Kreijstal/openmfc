#include "CHandleMapSupport.h"
static_assert(sizeof(CHandleMapAllocation) == 24, "native fixed handle allocator");
static_assert(offsetof(CHandleMap, m_pfnConstruct) == 0x18 &&
                  offsetof(CHandleMap, m_pfnDestruct) == 0x20,
              "native handle callbacks");
static_assert(offsetof(CHandleMap, m_permanentMap) == 0x28 &&
                  offsetof(CHandleMap, m_temporaryMap) == 0x60,
              "native embedded handle maps");
static_assert(offsetof(CHandleMap, m_pClass) == 0x98 && offsetof(CHandleMap, m_nOffset) == 0xa0 &&
                  offsetof(CHandleMap, m_nHandles) == 0xa8 && sizeof(CHandleMap) == 176,
              "native handle map metadata");
CHandleMapAllocation::CHandleMapAllocation(CRuntimeClass* type) {
    openmfc::detail::cfixedalloc::fa_construct(this, static_cast<unsigned>(type->m_nObjectSize),
                                               64);
}
CHandleMapAllocation::~CHandleMapAllocation() { openmfc::detail::cfixedalloc::fa_free_all(this); }
