#include "CHandleMapSupport.h"

namespace openmfc { namespace detail { namespace chandlemap {
void* const g_mapVptrSentinel[1] = { nullptr };
} } }

static_assert(sizeof(OpenMfc_CMapPtrToPtr_Layout) == 56,
              "CMapPtrToPtr faithful layout must be 56 bytes");
static_assert(offsetof(CHandleMap, m_pfnDestructor) == 0x18, "m_pfnDestructor @0x18");
static_assert(offsetof(CHandleMap, m_permanentMap)  == 0x28, "m_permanentMap @0x28");
static_assert(offsetof(CHandleMap, m_temporaryMap)  == 0x60, "m_temporaryMap @0x60");
static_assert(offsetof(CHandleMap, m_pClass)        == 0x98, "m_pClass @0x98");
static_assert(offsetof(CHandleMap, m_nOffset)       == 0xA0, "m_nOffset @0xA0");
static_assert(offsetof(CHandleMap, m_nHandles)      == 0xA8, "m_nHandles @0xA8");
static_assert(sizeof(CHandleMap) == 176, "CHandleMap must be 176 bytes (faithful x64)");
CRuntimeClass CTempHandleObject_Chandlemap::classCTempHandleObject = {
    "CTempHandleObject_Chandlemap",
    sizeof(CTempHandleObject_Chandlemap),
    0xFFFF,
    nullptr,
    nullptr,
    &CObject::classCObject,
    nullptr
};
extern "C" CHandleMap* MS_ABI
impl___0CHandleMap__QEAA_PEAUCRuntimeClass__P6AXPEAVCObject___Z2_KH_Z(
    CHandleMap* pThis, CRuntimeClass* pClass,
    void (*pfnDestructor)(CObject*), void (*pfnDestructor2)(CObject*),
    std::size_t nOffset, int nHandles)
{
    if (pThis == nullptr)
        return nullptr;

    unsigned objectSize = 8;
    if (pClass != nullptr && static_cast<unsigned>(pClass->m_nObjectSize) > objectSize)
        objectSize = static_cast<unsigned>(pClass->m_nObjectSize);

    pThis->m_nObjectSize   = objectSize;
    pThis->m_nMaxTemp      = 0x40;
    pThis->m_pReserved0    = nullptr;
    pThis->m_pReserved1    = nullptr;

    auto initMap = [](OpenMfc_CMapPtrToPtr_Layout& m, unsigned hashSize, long long blockSize) {
        m.m_vptr           = const_cast<void**>(g_mapVptrSentinel);
        m.m_pHashTable     = nullptr;
        m.m_nHashTableSize = hashSize;
        m.m_pad            = 0;
        m.m_nCount         = 0;
        m.m_pFreeList      = nullptr;
        m.m_pBlocks        = nullptr;
        m.m_nBlockSize     = blockSize;
    };
    initMap(pThis->m_permanentMap, 17, 10);
    initMap(pThis->m_temporaryMap, 7, 4);

    // Retail routes null destructor callbacks into a non-returning error
    // helper, so no half-built CHandleMap can ever be observed. OpenMFC cannot
    // abort the caller's process from inside a DLL export, so it does the next
    // best thing: finish initialization unconditionally. The previous early
    // return handed back an object with m_pClass, both destructor slots,
    // m_nOffset and m_nHandles still unset AND a temporary map left at the
    // permanent map's hash size -- strictly worse than a consistent object
    // whose (null) callbacks fail loudly at first use.
    pThis->m_pClass         = pClass;
    pThis->m_pfnDestructor  = pfnDestructor;
    pThis->m_pfnDestructor2 = pfnDestructor2;
    pThis->m_nOffset        = nOffset;
    pThis->m_nHandles       = static_cast<unsigned>(nHandles);
    return pThis;
}
namespace openmfc { namespace detail { namespace chandlemap {
std::mutex g_mapMutex;
std::unordered_map<const CHandleMap*, MapState>& SideTables() {
    std::unordered_map<const CHandleMap*, MapState> tables;
    return tables;
}
MapState& StateFor(const CHandleMap* self) {
    return SideTables()[self];
}
void CHandleMap_SetPermanent(CHandleMap* self, void* h, CObject* permob) {
    if (self == nullptr || h == nullptr) return;
    std::lock_guard<std::mutex> lk(g_mapMutex);
    StateFor(self).permanent[h] = permob;
}
CObject* CHandleMap_LookupPermanent(CHandleMap* self, void* h) {
    if (self == nullptr || h == nullptr) return nullptr;
    std::lock_guard<std::mutex> lk(g_mapMutex);
    MapState& st = StateFor(self);
    auto it = st.permanent.find(h);
    return it == st.permanent.end() ? nullptr : it->second;
}
std::size_t CHandleMap_TemporaryCount(CHandleMap* self) {
    if (self == nullptr) return 0;
    std::lock_guard<std::mutex> lk(g_mapMutex);
    return StateFor(self).temporary.size();
}
} } }  // namespace openmfc::detail::chandlemap
