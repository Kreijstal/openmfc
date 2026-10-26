#pragma once
#include "detail/CFixedAllocSupport.h"

// Release-x64 CHandleMap embeds its allocator and both live pointer maps.
// Members are destroyed before the allocator, in the native cleanup order.
struct CHandleMapAllocation : openmfc::detail::cfixedalloc::CFixedAllocState {
    explicit CHandleMapAllocation(CRuntimeClass*);
    ~CHandleMapAllocation();
};
class CHandleMap : public CHandleMapAllocation {
  public:
    using ObjectCallback = void(MS_ABI*)(CObject*);
    ObjectCallback m_pfnConstruct;
    ObjectCallback m_pfnDestruct;
    CMapPtrToPtr m_permanentMap;
    CMapPtrToPtr m_temporaryMap;
    CRuntimeClass* m_pClass;
    size_t m_nOffset;
    unsigned m_nHandles;
    CHandleMap(CRuntimeClass*, ObjectCallback, ObjectCallback, size_t, int);
    ~CHandleMap();
    CObject* FromHandle(void*);
    void DeleteTemp();
};
extern "C" CHandleMap* MS_ABI impl___0CHandleMap__QEAA_PEAUCRuntimeClass__P6AXPEAVCObject___Z2_KH_Z(
    CHandleMap*, CRuntimeClass*, CHandleMap::ObjectCallback, CHandleMap::ObjectCallback, size_t,
    int);
extern "C" CObject* MS_ABI impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(CHandleMap*, void*);
extern "C" void MS_ABI impl__DeleteTemp_CHandleMap__QEAAXXZ(CHandleMap*);
