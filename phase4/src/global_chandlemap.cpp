// CHandleMap real implementation
//
// CHandleMap is an MFC-internal helper that maps OS handles (HWND, HMENU, HDC,
// ...) to their CObject* wrappers. MFC keeps two maps: a "permanent" map of
// wrappers owned by the application, and a "temporary" map of throw-away
// wrappers that are recreated on demand and flushed by DeleteTemp() (called
// during idle processing).
//
// Only two methods are exported from openmfc.def for CHandleMap:
//   public: class CObject* __cdecl CHandleMap::FromHandle(void*)
//   public: void          __cdecl CHandleMap::DeleteTemp(void)
// SetPermanent / RemoveHandle / LookupPermanent / LookupTemporary are inlined
// into their callers in real MFC and therefore are not separate exports.
//
// Faithful x64 layout (mfc140u): standalone class, NO base, NO vtable. Member
// order mirrors MFC's <afximpl.h> CHandleMap. Because CHandleMap instances are
// MFC-internal globals that never cross the public ABI as raw layout, the actual
// handle->CObject* bookkeeping is kept in an opaque side table keyed by the
// CHandleMap* (same strategy as gdicore.cpp's temp GDI map). The embedded
// CMapPtrToPtr members are present only to keep sizeof/layout faithful.

#define OPENMFC_APPCORE_IMPL
// Only CObject / CRuntimeClass are required; afx.h provides both. We avoid the
// heavier afxmfc.h umbrella so the self-contained behavioral test can link
// without pulling in CWnd/CCmdTarget vtables.
#include "openmfc/afx.h"

#include <cstddef>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Faithful CMapPtrToPtr storage block (x64 / mfc140u layout = 56 bytes).
// We only need it for binary layout fidelity; the live data lives in a side
// table, so this is a plain POD-ish blob with the correct field widths.
// ---------------------------------------------------------------------------
// Not in an anonymous namespace so it can serve as a member type of CHandleMap
// without -Wsubobject-linkage; it carries no external definitions.
struct OpenMfc_CMapPtrToPtr_Layout {
    void*       m_vptr;            // off 0  : CObject vptr (CMapPtrToPtr is dynamic)
    void*       m_pHashTable;      // off 8  : CAssoc**
    unsigned    m_nHashTableSize;  // off 16 : UINT
    unsigned    m_pad;             // off 20 : padding
    long long   m_nCount;          // off 24 : INT_PTR
    void*       m_pFreeList;       // off 32 : CAssoc*
    void*       m_pBlocks;         // off 40 : CPlex*
    long long   m_nBlockSize;      // off 48 : INT_PTR
};
static_assert(sizeof(OpenMfc_CMapPtrToPtr_Layout) == 56,
              "CMapPtrToPtr faithful layout must be 56 bytes");

// ---------------------------------------------------------------------------
// CHandleMap with the faithful 136-byte layout. Standalone, no base, no vtable.
// ---------------------------------------------------------------------------
// Field offsets and initial values below are read directly out of the retail
// mfc140u constructor (??0CHandleMap@@QEAA@...): it stores a DWORD object size
// and temp-cap in the header, embeds the two CMapPtrToPtr blocks at 0x28 and
// 0x60, and closes with m_pClass/m_nOffset/m_nHandles at 0x98/0xA0/0xA8.
class CHandleMap {
public:
    unsigned            m_nObjectSize;   // off 0x00 : max(pClass->m_nObjectSize, 8)
    unsigned            m_nMaxTemp;      // off 0x04 : 0x40
    void*               m_pReserved0;    // off 0x08 : zeroed by retail
    void*               m_pReserved1;    // off 0x10 : zeroed by retail
    void              (*m_pfnDestructor)(CObject*);  // off 0x18
    void              (*m_pfnDestructor2)(CObject*); // off 0x20
    OpenMfc_CMapPtrToPtr_Layout m_permanentMap;      // off 0x28 (block size 10)
    OpenMfc_CMapPtrToPtr_Layout m_temporaryMap;      // off 0x60 (block size 4)
    CRuntimeClass*      m_pClass;        // off 0x98
    std::size_t         m_nOffset;       // off 0xA0
    unsigned            m_nHandles;      // off 0xA8
};

static_assert(offsetof(CHandleMap, m_pfnDestructor) == 0x18, "m_pfnDestructor @0x18");
static_assert(offsetof(CHandleMap, m_permanentMap)  == 0x28, "m_permanentMap @0x28");
static_assert(offsetof(CHandleMap, m_temporaryMap)  == 0x60, "m_temporaryMap @0x60");
static_assert(offsetof(CHandleMap, m_pClass)        == 0x98, "m_pClass @0x98");
static_assert(offsetof(CHandleMap, m_nOffset)       == 0xA0, "m_nOffset @0xA0");
static_assert(offsetof(CHandleMap, m_nHandles)      == 0xA8, "m_nHandles @0xA8");
static_assert(sizeof(CHandleMap) == 176, "CHandleMap must be 176 bytes (faithful x64)");

// ---------------------------------------------------------------------------
// Concrete CObject wrapper used for temporary handle wrappers. CObject's ctor
// is protected, so we need a tiny public subclass to instantiate. It carries
// the handle so callers/tests can verify identity, and a real CRuntimeClass so
// it participates honestly in MFC RTTI.
// ---------------------------------------------------------------------------
class CTempHandleObject : public CObject {
public:
    void* m_hHandle = nullptr;
    static CRuntimeClass classCTempHandleObject;
    virtual CRuntimeClass* GetRuntimeClass() const override {
        return &classCTempHandleObject;
    }
};

CRuntimeClass CTempHandleObject::classCTempHandleObject = {
    "CTempHandleObject",
    sizeof(CTempHandleObject),
    0xFFFF,
    nullptr,
    nullptr,
    &CObject::classCObject,
    nullptr
};

// ---------------------------------------------------------------------------
// Opaque side table. For each CHandleMap instance we track its permanent and
// temporary handle->CObject* mappings here. Keyed by the CHandleMap* so the
// embedded faithful CMapPtrToPtr blobs stay untouched.
// ---------------------------------------------------------------------------
namespace {
struct MapState {
    std::unordered_map<void*, CObject*> permanent;
    std::unordered_map<void*, CTempHandleObject*> temporary;
};

std::mutex g_mapMutex;
std::unordered_map<const CHandleMap*, MapState>& SideTables() {
    static std::unordered_map<const CHandleMap*, MapState> tables;
    return tables;
}

MapState& StateFor(const CHandleMap* self) {
    return SideTables()[self];
}
} // namespace

// ---------------------------------------------------------------------------
// Public test/helper hooks (not exported symbols). Used by the behavioral test
// to seed the permanent map the way MFC's inlined SetPermanent() would.
// ---------------------------------------------------------------------------
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

// ===========================================================================
// EXPORTED SYMBOL IMPLEMENTATIONS
// ===========================================================================

// public: class CObject* __cdecl CHandleMap::FromHandle(void* h)
//
// Looks the handle up in the permanent map first; if found, returns the
// application-owned wrapper. Otherwise it returns an existing temporary
// wrapper for that handle, creating one on demand (mirrors GetTempGdiObject /
// real MFC CHandleMap::FromHandle). Returns nullptr for a null handle.
//
// Symbol: ?FromHandle@CHandleMap@@QEAAPEAVCObject@@PEAX@Z
extern "C" CObject* MS_ABI
impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(CHandleMap* pThis, void* h)
{
    if (pThis == nullptr || h == nullptr)
        return nullptr;

    std::lock_guard<std::mutex> lk(g_mapMutex);
    MapState& st = StateFor(pThis);

    // 1) Permanent map takes precedence.
    auto perm = st.permanent.find(h);
    if (perm != st.permanent.end())
        return perm->second;

    // 2) Reuse an existing temporary wrapper if present.
    auto tmp = st.temporary.find(h);
    if (tmp != st.temporary.end())
        return tmp->second;

    // 3) Create a fresh temporary wrapper bound to this handle.
    auto* wrapper = new CTempHandleObject();
    wrapper->m_hHandle = h;
    st.temporary.emplace(h, wrapper);
    return wrapper;
}

// public: void __cdecl CHandleMap::DeleteTemp(void)
//
// Destroys every temporary wrapper and clears the temporary map. Permanent
// wrappers are left untouched (mirrors DeleteTempGdiMap / real MFC
// CHandleMap::DeleteTemp). Safe on a null this.
//
// Symbol: ?DeleteTemp@CHandleMap@@QEAAXXZ
extern "C" void MS_ABI
impl__DeleteTemp_CHandleMap__QEAAXXZ(CHandleMap* pThis)
{
    if (pThis == nullptr)
        return;

    std::lock_guard<std::mutex> lk(g_mapMutex);
    MapState& st = StateFor(pThis);
    for (auto& kv : st.temporary) {
        delete kv.second;
    }
    st.temporary.clear();
}

// ---------------------------------------------------------------------------
// public: __cdecl CHandleMap::CHandleMap(CRuntimeClass*, void (*)(CObject*),
//                                        void (*)(CObject*), size_t, int)
//
// Field-for-field reproduction of the retail mfc140u constructor. Retail:
//   m_nObjectSize          = max(pClass->m_nObjectSize, 8)
//   m_nMaxTemp             = 0x40
//   reserved 0x08 / 0x10   = 0
//   permanent map          : hash size 17, block size 10
//   temporary map          : hash size 17, block size 4  -- then forced to 7
//                            once both destructor callbacks are known good
//   m_pClass/m_nOffset/m_nHandles from the arguments
// Retail treats a null callback as a hard error (it branches to a noreturn
// helper); we mirror that as a defensive early-out instead of faulting.
//
// The embedded map blocks exist purely for binary-layout fidelity -- the live
// handle bookkeeping is the side table above -- so their vptr slot is pointed
// at a private sentinel. Nothing in OpenMFC virtual-dispatches through them.
// Symbol: ??0CHandleMap@@QEAA@PEAUCRuntimeClass@@P6AXPEAVCObject@@@Z2_KH@Z
namespace { void* const g_mapVptrSentinel[1] = { nullptr }; }

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
    initMap(pThis->m_temporaryMap, 17, 4);

    if (pfnDestructor == nullptr || pfnDestructor2 == nullptr)
        return pThis;                       // retail: noreturn error helper
    pThis->m_temporaryMap.m_nHashTableSize = 7;

    pThis->m_pClass         = pClass;
    pThis->m_pfnDestructor  = pfnDestructor;
    pThis->m_pfnDestructor2 = pfnDestructor2;
    pThis->m_nOffset        = nOffset;
    pThis->m_nHandles       = static_cast<unsigned>(nHandles);
    return pThis;
}
