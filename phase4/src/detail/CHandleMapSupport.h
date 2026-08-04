#pragma once
// Shared internals of the former global_chandlemap.cpp translation unit.
// Definitions live in detail/CHandleMapSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace chandlemap {} } }
using namespace openmfc::detail::chandlemap;
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


// ---------------------------------------------------------------------------
// Concrete CObject wrapper used for temporary handle wrappers. CObject's ctor
// is protected, so we need a tiny public subclass to instantiate. It carries
// the handle so callers/tests can verify identity, and a real CRuntimeClass so
// it participates honestly in MFC RTTI.
// ---------------------------------------------------------------------------
class CTempHandleObject_Chandlemap : public CObject {
public:
    void* m_hHandle = nullptr;
    static CRuntimeClass classCTempHandleObject;
    virtual CRuntimeClass* GetRuntimeClass() const override {
        return &classCTempHandleObject;
    }
};


// ---------------------------------------------------------------------------
// Opaque side table. For each CHandleMap instance we track its permanent and
// temporary handle->CObject* mappings here. Keyed by the CHandleMap* so the
// embedded faithful CMapPtrToPtr blobs stay untouched.
// ---------------------------------------------------------------------------
namespace openmfc { namespace detail { namespace chandlemap {
struct MapState {
    std::unordered_map<void*, CObject*> permanent;
    std::unordered_map<void*, CTempHandleObject_Chandlemap*> temporary;
};
} } }

namespace openmfc { namespace detail { namespace chandlemap {
extern std::mutex g_mapMutex;
} } }
namespace openmfc { namespace detail { namespace chandlemap {
std::unordered_map<const CHandleMap*, MapState>& SideTables();
} } }

namespace openmfc { namespace detail { namespace chandlemap {
MapState& StateFor(const CHandleMap* self);
} } }

// ---------------------------------------------------------------------------
// Public test/helper hooks (not exported symbols). Used by the behavioral test
// to seed the permanent map the way MFC's inlined SetPermanent() would.
// ---------------------------------------------------------------------------
namespace openmfc { namespace detail { namespace chandlemap {
void CHandleMap_SetPermanent(CHandleMap* self, void* h, CObject* permob);
} } }

// Private vptr sentinel for the embedded map blocks — one object, so every
// unit that stamps it compares equal.
namespace openmfc { namespace detail { namespace chandlemap {
extern void* const g_mapVptrSentinel[1];
} } }

namespace openmfc { namespace detail { namespace chandlemap {
CObject* CHandleMap_LookupPermanent(CHandleMap* self, void* h);
} } }

namespace openmfc { namespace detail { namespace chandlemap {
std::size_t CHandleMap_TemporaryCount(CHandleMap* self);
} } }

// ===========================================================================
// EXPORTED SYMBOL IMPLEMENTATIONS
// ===========================================================================




extern "C" CHandleMap* MS_ABI
impl___0CHandleMap__QEAA_PEAUCRuntimeClass__P6AXPEAVCObject___Z2_KH_Z(
    CHandleMap* pThis, CRuntimeClass* pClass,
    void (*pfnDestructor)(CObject*), void (*pfnDestructor2)(CObject*),
    std::size_t nOffset, int nHandles);
