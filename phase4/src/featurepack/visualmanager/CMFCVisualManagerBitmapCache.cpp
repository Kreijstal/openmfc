// CMFCVisualManagerBitmapCache — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Retail declares this class (and its nested CMFCVisualManagerBitmapCacheItem)
// in afxvisualmanageroffice2007.h:
//
//   class CMFCVisualManagerBitmapCache {
//     class CMFCVisualManagerBitmapCacheItem {       // no base, virtual dtor
//       CMFCToolBarImages m_Images; BOOL m_bMirror;
//     };
//     CArray<CMFCVisualManagerBitmapCacheItem*, CMFCVisualManagerBitmapCacheItem*> m_Cache;
//     CArray<CSize, CSize> m_Sizes;
//   };                                               // no base, virtual dtor
//
// OpenMFC's public headers do not declare either class, so the layout is
// pinned here from the retail constructors/destructors, as shadow structs.
// Every implemented body below is transcribed from the retail export's
// disassembly (deviations are marked where they occur); CacheY and
// CMFCVisualManagerBitmapCacheItem::Cache remain stubs, see their comments.
// RVAs are for mfc140u.dll unless marked "(mfc140)", which names the ANSI
// twin whose function bodies are byte-identical but whose addresses differ.
//
// Vtables.  Neither class derives from anything, so each MSVC vftable holds
// exactly one slot, the scalar deleting destructor.  The two CArray members
// are CObject-derived template instances whose five-slot vftables live in
// retail .rdata; OpenMFC has no C++ instantiation of them, so this file
// hand-authors all four MSVC-layout vftables and the constructors install
// them, as retail's constructors do.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstdlib>
#include <cstring>

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each signature matches its definition in the tree.
// ---------------------------------------------------------------------------
// featurepack/toolbar/CMFCToolBarImages.cpp
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarImages__UEAA_XZ(void* pThis);
extern "C" int   MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(CMFCToolBarImages* pThis, HBITMAP hBmp, int bSet);
extern "C" int   MS_ABI impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
    CMFCToolBarImages* pThis, CDC* pDC, CRect rect, int iImageIndex,
    int horzAlign, int vertAlign, CRect rectSrc, unsigned char alphaSrc);
// core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject* pThis);
// detail/MemcoreSupport.cpp -- MFC's exported operator new (malloc-backed)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
// detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// Siblings in this file that are called before their definition.
extern "C" void* MS_ABI impl___0CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCVisualManagerBitmapCache__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__Clear_CMFCVisualManagerBitmapCache__QEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__FindIndex_CMFCVisualManagerBitmapCache__QEBAHAEBVCSize___Z(const void* pThis, const CSize* pSize);
extern "C" void  MS_ABI impl__Cache_CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAAXAEBVCSize__AEAVCMFCControlRenderer___Z(
    void* pThis, const CSize* pSize, CMFCControlRenderer* pRenderer);
extern "C" int   MS_ABI impl__Cache_CMFCVisualManagerBitmapCache__QEAAHAEBVCSize__AEAVCMFCControlRenderer___Z(
    void* pThis, const CSize* pSize, CMFCControlRenderer* pRenderer);

namespace {

// ---------------------------------------------------------------------------
// Shadow layouts.
// ---------------------------------------------------------------------------
// CArray<TYPE, ARG_TYPE> (afxtempl.h): CObject vfptr, m_pData, m_nSize,
// m_nMaxSize, m_nGrowBy -- 0x28 bytes.  Both instances here have 8-byte
// elements (a pointer; a CSize).
template <typename T>
struct S_Array8 {
    const void* const* vfptr;  // 0x00
    T*       m_pData;          // 0x08
    INT_PTR  m_nSize;          // 0x10
    INT_PTR  m_nMaxSize;       // 0x18
    INT_PTR  m_nGrowBy;        // 0x20
};
typedef S_Array8<void*> S_ItemArray;   // XCache
typedef S_Array8<SIZE>  S_SizeArray;   // XSizes
static_assert(sizeof(SIZE) == 8, "CSize elements are 8 bytes");
static_assert(offsetof(S_ItemArray, m_pData)   == 0x08, "CArray::m_pData");
static_assert(offsetof(S_ItemArray, m_nSize)   == 0x10, "CArray::m_nSize");
static_assert(offsetof(S_ItemArray, m_nMaxSize) == 0x18, "CArray::m_nMaxSize");
static_assert(offsetof(S_ItemArray, m_nGrowBy) == 0x20, "CArray::m_nGrowBy");
static_assert(sizeof(S_ItemArray) == 0x28, "sizeof(CArray) (both CArray scalar deleting dtors pass 0x28)");

// Constructor 0x199270 stores the class vfptr at +0x00, m_Cache's CArray
// vfptr at +0x08 and m_Sizes' at +0x30, and zeroes +0x10..+0x28 and
// +0x38..+0x50.  The scalar deleting destructor 0x1992c0 frees 0x58 bytes.
struct S_BitmapCache {
    const void* const* vfptr;  // 0x00
    S_ItemArray m_Cache;       // 0x08
    S_SizeArray m_Sizes;       // 0x30
};
static_assert(offsetof(S_BitmapCache, m_Cache) == 0x08, "ctor 0x199270: m_Cache vfptr store at +0x08");
static_assert(offsetof(S_BitmapCache, m_Sizes) == 0x30, "ctor 0x199270: m_Sizes vfptr store at +0x30");
static_assert(offsetof(S_BitmapCache, m_Sizes) + offsetof(S_SizeArray, m_nSize) == 0x40,
              "FindIndex 0x199510 reads m_Sizes.m_nSize at +0x40");
static_assert(sizeof(S_BitmapCache) == 0x58, "scalar deleting dtor 0x1992c0: mov $0x58,%edx");

// Item constructor 0x198d70 stores the vfptr, constructs m_Images at +0x08
// and zeroes m_bMirror at +0x1a0; its scalar deleting destructor 0x198da0
// frees 0x1a8 bytes, and Cache 0x199400 allocates 0x1a8.
struct S_BitmapCacheItem {
    const void* const* vfptr;                 // 0x000
    alignas(8) unsigned char m_Images[0x198]; // 0x008  CMFCToolBarImages
    BOOL m_bMirror;                           // 0x1a0
};
static_assert(sizeof(CMFCToolBarImages) == 0x198, "CMFCToolBarImages retail size (afxmfc.h harvested layout)");
static_assert(offsetof(S_BitmapCacheItem, m_Images) == 0x008, "item ctor 0x198d70: m_Images at +0x08");
static_assert(offsetof(S_BitmapCacheItem, m_bMirror) == 0x1a0, "item ctor 0x198d70: m_bMirror at +0x1a0");
static_assert(sizeof(S_BitmapCacheItem) == 0x1a8, "item scalar deleting dtor 0x198da0 / Cache 0x199400: 0x1a8");

// CMFCToolBarImages::m_sizeImage, read by Draw (0x199060) and DrawY
// (0x1990e0) as `mov 0x68(this+8)`; afxmfc.h documents the member at 0x068.
const std::size_t kImages_m_sizeImage = 0x68;

inline CMFCToolBarImages* ItemImages(void* pItem) {
    return reinterpret_cast<CMFCToolBarImages*>(static_cast<S_BitmapCacheItem*>(pItem)->m_Images);
}
inline SIZE ItemImageSize(void* pItem) {
    SIZE s;
    std::memcpy(&s, static_cast<S_BitmapCacheItem*>(pItem)->m_Images + kImages_m_sizeImage, sizeof(s));
    return s;
}

// ---------------------------------------------------------------------------
// CArray<TYPE,ARG_TYPE>::SetSize(nNewSize, -1), 8-byte elements.
// Both retail bodies are unexported.  XCache calls 0x11f48, a body shared
// (identical-code-folded) with other 8-byte-element CArray instances -- it
// has 103 call sites in the image; XSizes calls its own body at 0x1ad570
// (mfc140u; 0x11f68 / 0x1abb70 in mfc140), which differs only by the CSize
// constructor loop noted below.  Both are afxtempl.h's SetSize with
// nGrowBy == -1:
//   nNewSize < 0                  -> AfxThrowInvalidArgException
//   nNewSize == 0                 -> free(m_pData); m_pData = NULL; m_nSize = m_nMaxSize = 0
//   m_pData == NULL               -> alloc max(nNewSize, m_nGrowBy) elements, zero them all
//   nNewSize <= m_nMaxSize        -> zero the newly exposed tail, m_nSize = nNewSize
//   otherwise grow by m_nGrowBy, or when it is 0 by clamp(m_nSize / 8, 4, 1024);
//     ENSURE(nNewMax >= m_nMaxSize); copy, zero the new tail, free the old block.
// The XSizes copy additionally runs CSize's constructor (store 0) over the
// new elements after the memset; that is the same bytes, so one helper serves
// both.  Retail allocates with MFC's operator new (0x27f0), which retries
// through the module thread state's new-handler and then returns NULL;
// OpenMFC's impl___2_YAPEAX_K_Z is a plain malloc.  Release is the CRT free.
// Deviation: when the allocation fails, retail either faults in memset
// (first-allocation path) or, on the growth path, hits the inlined memcpy_s
// check (errno = EINVAL, _invalid_parameter_noinfo, then
// AfxThrowInvalidArgException); this helper throws CInvalidArgException.
// ---------------------------------------------------------------------------
template <typename T>
void ArraySetSize(S_Array8<T>* a, INT_PTR nNewSize) {
    static_assert(sizeof(T) == 8, "helper assumes 8-byte elements");
    if (nNewSize < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (nNewSize == 0) {
        if (a->m_pData != nullptr) {
            std::free(a->m_pData);
            a->m_pData = nullptr;
        }
        a->m_nMaxSize = 0;
        a->m_nSize = 0;
        return;
    }
    if (a->m_pData == nullptr) {
        INT_PTR nAlloc = a->m_nGrowBy;
        if (nNewSize > nAlloc) nAlloc = nNewSize;
        T* p = static_cast<T*>(impl___2_YAPEAX_K_Z(static_cast<std::size_t>(nAlloc) * sizeof(T)));
        if (p == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        a->m_pData = p;
        std::memset(p, 0, static_cast<std::size_t>(nAlloc) * sizeof(T));
        a->m_nSize = nNewSize;
        a->m_nMaxSize = nAlloc;
        return;
    }
    if (nNewSize <= a->m_nMaxSize) {
        if (nNewSize > a->m_nSize) {
            std::memset(a->m_pData + a->m_nSize, 0,
                        static_cast<std::size_t>(nNewSize - a->m_nSize) * sizeof(T));
        }
        a->m_nSize = nNewSize;
        return;
    }
    INT_PTR nGrowBy = a->m_nGrowBy;
    if (nGrowBy == 0) {
        nGrowBy = a->m_nSize / 8;
        nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
    }
    INT_PTR nNewMax;
    if (nNewSize < a->m_nMaxSize + nGrowBy) {
        nNewMax = a->m_nMaxSize + nGrowBy;
        if (nNewMax < a->m_nMaxSize) {   // ENSURE_ARG(nNewMax >= m_nMaxSize)
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
    } else {
        nNewMax = nNewSize;
    }
    T* pNew = static_cast<T*>(impl___2_YAPEAX_K_Z(static_cast<std::size_t>(nNewMax) * sizeof(T)));
    if (pNew == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (a->m_nSize != 0) {
        std::memcpy(pNew, a->m_pData, static_cast<std::size_t>(a->m_nSize) * sizeof(T));
    }
    std::memset(pNew + a->m_nSize, 0, static_cast<std::size_t>(nNewSize - a->m_nSize) * sizeof(T));
    std::free(a->m_pData);
    a->m_pData = pNew;
    a->m_nSize = nNewSize;
    a->m_nMaxSize = nNewMax;
}

// CArray destructor.  Retail has one out-of-line body per instance (XCache
// 0x1ad13c, XSizes 0x1ad160), called -- not inlined -- from the class
// destructor 0x199300: re-store the CArray vfptr, free m_pData when non-NULL.
// Deviation: retail re-stores its own vfptr first; that store is not
// observable once destruction completes and is omitted here.
template <typename T>
void ArrayDestroy(S_Array8<T>* a) {
    if (a->m_pData != nullptr) std::free(a->m_pData);
}

// ---------------------------------------------------------------------------
// Hand-authored vftable slots.
// ---------------------------------------------------------------------------
// CMFCVisualManagerBitmapCache scalar deleting destructor, 0x1992c0:
//   ~CMFCVisualManagerBitmapCache(); if (flags & 1) operator delete(this, 0x58);
// The sized delete (0x2b77b0; 0x2b5200 in mfc140) is a jmp to 0x27c0, the
// folded operator delete / delete[] body, itself a jmp through the CRT free
// import; there is no flags&4 branch in this one.
void* MS_ABI Cache_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CMFCVisualManagerBitmapCache__UEAA_XZ(pThis);
    if (flags & 1) std::free(pThis);
    return pThis;
}
// CMFCVisualManagerBitmapCacheItem scalar deleting destructor, 0x198da0:
// the item destructor inlined (vfptr store, then ~CMFCToolBarImages on
// m_Images), then operator delete(this, 0x1a8) under flags & 1.
void* MS_ABI Item_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__UEAA_XZ(pThis);
    if (flags & 1) std::free(pThis);
    return pThis;
}
// CArray scalar deleting destructors, XCache 0x1ad480 / XSizes 0x1ad4d0:
//   ~CArray(); if (flags & 1) { if (flags & 4) 0x27d0(this, 0x28); else free(this); }
// 0x27d0 is the folded `ret` body (see CMFCBaseAccessibleObject.cpp), so the
// flags & 4 branch frees nothing.
void* MS_ABI ItemArray_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    ArrayDestroy(static_cast<S_ItemArray*>(pThis));
    if ((flags & 1) && !(flags & 4)) std::free(pThis);
    return pThis;
}
void* MS_ABI SizeArray_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    ArrayDestroy(static_cast<S_SizeArray*>(pThis));
    if ((flags & 1) && !(flags & 4)) std::free(pThis);
    return pThis;
}
// Slots 2..4 of the CArray vftables.  Retail slot 2 is CArray::Serialize
// (XCache 0xe4920, XSizes 0x1ad190: WriteCount/ReadCount + SetSize, then raw
// CArchive::Write/Read of the element block).  Deviation: OpenMFC fills slot
// 2 with a no-op -- m_Cache and m_Sizes are protected members no retail code
// path serializes, and a stored m_Cache would be raw heap pointers.  Slots 3
// and 4 (AssertValid / Dump) are the folded `ret` 0x27d0 in retail too.
void MS_ABI Array_NoOp(void* /*pThis*/, void* /*arg*/) {}

#define VT(fn) reinterpret_cast<const void*>(&fn)
// .rdata 0x31d538 (mfc140u): one slot.
const void* const g_BitmapCache_vtbl[1] = {
    VT(Cache_ScalarDeletingDtor),
};
// .rdata 0x31d548 (mfc140u): one slot.
const void* const g_BitmapCacheItem_vtbl[1] = {
    VT(Item_ScalarDeletingDtor),
};
// .rdata 0x31ce68 (mfc140u), XCache: CObject::GetRuntimeClass (0x37a0),
// scalar deleting dtor, Serialize, AssertValid, Dump.
const void* const g_ItemArray_vtbl[5] = {
    VT(impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),
    VT(ItemArray_ScalarDeletingDtor),
    VT(Array_NoOp),
    VT(Array_NoOp),
    VT(Array_NoOp),
};
// .rdata 0x31ce38 (mfc140u), XSizes: same shape.
const void* const g_SizeArray_vtbl[5] = {
    VT(impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),
    VT(SizeArray_ScalarDeletingDtor),
    VT(Array_NoOp),
    VT(Array_NoOp),
    VT(Array_NoOp),
};
#undef VT

typedef void* (MS_ABI* ScalarDeletingDtorFn)(void* pThis, unsigned int flags);

} // namespace

// RVA 0x199270 (mfc140u).  Stores the three vfptrs and zeroes both arrays'
// m_pData / m_nSize / m_nMaxSize / m_nGrowBy.
// Symbol: ??0CMFCVisualManagerBitmapCache@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCVisualManagerBitmapCache__QEAA_XZ(void* pThis) {
    S_BitmapCache* p = static_cast<S_BitmapCache*>(pThis);
    p->vfptr = g_BitmapCache_vtbl;
    p->m_Cache.vfptr = g_ItemArray_vtbl;
    p->m_Sizes.vfptr = g_SizeArray_vtbl;
    p->m_Cache.m_pData = nullptr;
    p->m_Cache.m_nSize = 0;
    p->m_Cache.m_nMaxSize = 0;
    p->m_Cache.m_nGrowBy = 0;
    p->m_Sizes.m_pData = nullptr;
    p->m_Sizes.m_nSize = 0;
    p->m_Sizes.m_nMaxSize = 0;
    p->m_Sizes.m_nGrowBy = 0;
    return pThis;
}
// RVA 0x198d70 (mfc140u).  vfptr, CMFCToolBarImages() on m_Images (+0x08),
// m_bMirror = FALSE.
// Symbol: ??0CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAA_XZ(void* pThis) {
    S_BitmapCacheItem* p = static_cast<S_BitmapCacheItem*>(pThis);
    p->vfptr = g_BitmapCacheItem_vtbl;
    impl___0CMFCToolBarImages__QEAA_XZ(p->m_Images);
    p->m_bMirror = FALSE;
    return pThis;
}
// RVA 0x199300 (mfc140u):
//   vfptr = CMFCVisualManagerBitmapCache vftable;
//   Clear();                       // call 0x199330
//   m_Sizes.~CArray();             // call 0x1ad160
//   m_Cache.~CArray();             // tail-jmp 0x1ad13c
// Symbol: ??1CMFCVisualManagerBitmapCache@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCVisualManagerBitmapCache__UEAA_XZ(void* pThis) {
    S_BitmapCache* p = static_cast<S_BitmapCache*>(pThis);
    p->vfptr = g_BitmapCache_vtbl;
    impl__Clear_CMFCVisualManagerBitmapCache__QEAAXXZ(pThis);
    ArrayDestroy(&p->m_Sizes);
    ArrayDestroy(&p->m_Cache);
}

// RVA 0x199400 (mfc140u):
//   if (FindIndex(size) != -1) return -1;            // already cached: -1, not the index
//   CMFCVisualManagerBitmapCacheItem* pItem = new CMFCVisualManagerBitmapCacheItem;  // 0x1a8
//   pItem->Cache(size, renderer);                    // unconditional call 0x198e20
//   int nIndex = (int)m_Cache.Add(pItem);            // SetSize(n + 1) 0x11f48, store
//   m_Sizes.Add(size);                               // SetSize(n + 1) 0x1ad570, store
//   return nIndex;
// Each inlined Add throws CInvalidArgException when m_nSize < 0.  retail
// passes a NULL pItem (failed allocation) to Item::Cache unchanged; so does
// this body.  The item's image content comes from Item::Cache, which is
// still a stub (see below), so a cached item currently holds no images.
// Symbol: ?Cache@CMFCVisualManagerBitmapCache@@QEAAHAEBVCSize@@AEAVCMFCControlRenderer@@@Z
extern "C" int MS_ABI impl__Cache_CMFCVisualManagerBitmapCache__QEAAHAEBVCSize__AEAVCMFCControlRenderer___Z(
    void* pThis, const CSize* pSize, CMFCControlRenderer* pRenderer) {
    S_BitmapCache* p = static_cast<S_BitmapCache*>(pThis);
    if (impl__FindIndex_CMFCVisualManagerBitmapCache__QEBAHAEBVCSize___Z(pThis, pSize) != -1) {
        return -1;
    }
    void* pItem = impl___2_YAPEAX_K_Z(sizeof(S_BitmapCacheItem));
    if (pItem != nullptr) {
        pItem = impl___0CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAA_XZ(pItem);
    }
    impl__Cache_CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAAXAEBVCSize__AEAVCMFCControlRenderer___Z(
        pItem, pSize, pRenderer);

    const INT_PTR nIndex = p->m_Cache.m_nSize;
    if (nIndex < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    ArraySetSize(&p->m_Cache, nIndex + 1);
    p->m_Cache.m_pData[nIndex] = pItem;

    const INT_PTR nSizeIndex = p->m_Sizes.m_nSize;
    if (nSizeIndex < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    SIZE size;
    size.cx = pSize->cx;       // read before SetSize, as retail does
    size.cy = pSize->cy;
    ArraySetSize(&p->m_Sizes, nSizeIndex + 1);
    p->m_Sizes.m_pData[nSizeIndex] = size;
    return static_cast<int>(nIndex);
}

// STUB.  Retail (RVA 0x1994c0, mfc140u) is
//   return Cache(CSize(renderer.m_Params.m_rectImage.Width(), height), renderer);
// reading m_rectImage.right/left at renderer+0x1b8/+0x1b0.  OpenMFC's
// CMFCControlRenderer (include/openmfc/afxmfc.h) is 0x68 bytes with a padding
// blob instead of retail's 0x200-byte layout, so those reads would run past
// an OpenMFC-constructed object.  Returns -1, retail's "no new entry" value,
// rather than an index that was never cached.
// Symbol: ?CacheY@CMFCVisualManagerBitmapCache@@QEAAHHAEAVCMFCControlRenderer@@@Z
extern "C" int MS_ABI impl__CacheY_CMFCVisualManagerBitmapCache__QEAAHHAEAVCMFCControlRenderer___Z(
    void* pThis, int nHeight, CMFCControlRenderer* pRenderer) {
    (void)pThis; (void)nHeight; (void)pRenderer;
    return -1;
}

// RVA 0x199330 (mfc140u):
//   for (int i = 0; i < m_Cache.GetSize(); i++) {    // m_nSize re-read each pass
//       CMFCVisualManagerBitmapCacheItem* p = m_Cache[i];   // ENSURE bounds
//       if (p != NULL) delete p;                     // vslot 0, flags 1
//   }
//   m_Cache.RemoveAll();   // free(m_pData); m_pData = NULL; m_nMaxSize = m_nSize = 0
//   m_Sizes.RemoveAll();   // same, on +0x38/+0x48/+0x40
// The per-element ENSURE (throw when i < 0 || i >= m_nSize) cannot fire
// under the loop condition and is not reproduced.
// Symbol: ?Clear@CMFCVisualManagerBitmapCache@@QEAAXXZ
extern "C" void MS_ABI impl__Clear_CMFCVisualManagerBitmapCache__QEAAXXZ(void* pThis) {
    S_BitmapCache* p = static_cast<S_BitmapCache*>(pThis);
    for (int i = 0; i < p->m_Cache.m_nSize; i++) {
        void* pItem = p->m_Cache.m_pData[i];
        if (pItem != nullptr) {
            const void* const* vtbl = *static_cast<const void* const**>(pItem);
            reinterpret_cast<ScalarDeletingDtorFn>(const_cast<void*>(vtbl[0]))(pItem, 1);
        }
    }
    if (p->m_Cache.m_pData != nullptr) {
        std::free(p->m_Cache.m_pData);
        p->m_Cache.m_pData = nullptr;
    }
    p->m_Cache.m_nMaxSize = 0;
    p->m_Cache.m_nSize = 0;
    if (p->m_Sizes.m_pData != nullptr) {
        std::free(p->m_Sizes.m_pData);
        p->m_Sizes.m_pData = nullptr;
    }
    p->m_Sizes.m_nMaxSize = 0;
    p->m_Sizes.m_nSize = 0;
}

// RVA 0x199510 (mfc140u): linear search of m_Sizes for an exact cx/cy match;
// returns its index, or -1.
// Symbol: ?FindIndex@CMFCVisualManagerBitmapCache@@QEBAHAEBVCSize@@@Z
extern "C" int MS_ABI impl__FindIndex_CMFCVisualManagerBitmapCache__QEBAHAEBVCSize___Z(const void* pThis, const CSize* pSize) {
    const S_BitmapCache* p = static_cast<const S_BitmapCache*>(pThis);
    for (int i = 0; i < p->m_Sizes.m_nSize; i++) {
        const SIZE& s = p->m_Sizes.m_pData[i];
        if (pSize->cx == s.cx && pSize->cy == s.cy) return i;
    }
    return -1;
}

// RVA 0x199570 (mfc140u):
//   int n = FindIndex(size);
//   if (n == -1) return NULL;
//   return m_Cache[n];          // ENSURE(n >= 0 && n < m_Cache.m_nSize) -> AfxThrowInvalidArgException
// Symbol: ?Get@CMFCVisualManagerBitmapCache@@QEAAPEAVCMFCVisualManagerBitmapCacheItem@1@AEBVCSize@@@Z
extern "C" void* MS_ABI impl__Get_CMFCVisualManagerBitmapCache__QEAAPEAVCMFCVisualManagerBitmapCacheItem_1_AEBVCSize___Z(
    void* pThis, const CSize* pSize) {
    S_BitmapCache* p = static_cast<S_BitmapCache*>(pThis);
    const int n = impl__FindIndex_CMFCVisualManagerBitmapCache__QEBAHAEBVCSize___Z(pThis, pSize);
    if (n == -1) return nullptr;
    if (n < 0 || n >= p->m_Cache.m_nSize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    return p->m_Cache.m_pData[n];
}

// RVA 0x1995b0 (mfc140u): out-of-range nIndex returns NULL (no throw).
// Symbol: ?Get@CMFCVisualManagerBitmapCache@@QEAAPEAVCMFCVisualManagerBitmapCacheItem@1@H@Z
extern "C" void* MS_ABI impl__Get_CMFCVisualManagerBitmapCache__QEAAPEAVCMFCVisualManagerBitmapCacheItem_1_H_Z(
    void* pThis, int nIndex) {
    S_BitmapCache* p = static_cast<S_BitmapCache*>(pThis);
    if (nIndex < 0 || nIndex >= p->m_Cache.m_nSize) return nullptr;
    return p->m_Cache.m_pData[nIndex];
}

// RVA 0x1994f0 (mfc140u): FindIndex(size) != -1.
// Symbol: ?IsCached@CMFCVisualManagerBitmapCache@@QEBAHAEBVCSize@@@Z
extern "C" int MS_ABI impl__IsCached_CMFCVisualManagerBitmapCache__QEBAHAEBVCSize___Z(const void* pThis, const CSize* pSize) {
    return impl__FindIndex_CMFCVisualManagerBitmapCache__QEBAHAEBVCSize___Z(pThis, pSize) != -1;
}

// RVA 0x198df0 (mfc140u): vfptr store, then tail-jmp to ~CMFCToolBarImages
// on m_Images (+0x08).
// Symbol: ??1CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__UEAA_XZ(void* pThis) {
    S_BitmapCacheItem* p = static_cast<S_BitmapCacheItem*>(pThis);
    p->vfptr = g_BitmapCacheItem_vtbl;
    impl___1CMFCToolBarImages__UEAA_XZ(p->m_Images);
}

// RVA 0x198e10 (mfc140u): tail-jmp to m_Images.AddImage(hBmp, TRUE); the
// returned index is discarded.
// Symbol: ?AddImage@CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@QEAAXPEAUHBITMAP__@@@Z
extern "C" void MS_ABI impl__AddImage_CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAAXPEAUHBITMAP_____Z(
    void* pThis, HBITMAP hBmp) {
    impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(ItemImages(pThis), hBmp, TRUE);
}

// STUB.  Retail (RVA 0x198e20, mfc140u) clears m_Images, derives the frame
// count from renderer.GetImageCount() (inlined: renderer+0x10 is
// m_Bitmap.m_iCount; when it is 1, GetObject on m_Bitmap's image well at
// renderer+0xa8 divided by m_Params.m_rectImage.Height() at +0x1bc/+0x1b4),
// sets m_Images' image size and transparent colour, and for each frame
// renders into a 32bpp CreateDIBSection through a CDC, copying
// renderer.m_bMirror (+0x1f8) and bracketing the draw with renderer.Mirror()
// (vslot 11) around renderer.Draw (vslot 6), then AddImage()s the bitmap.
// All of that reads CMFCControlRenderer members at retail offsets, and
// OpenMFC's CMFCControlRenderer is a 0x68-byte padding blob (afxmfc.h) whose
// state lives in a side table; the reads would run past an OpenMFC object.
// Symbol: ?Cache@CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@QEAAXAEBVCSize@@AEAVCMFCControlRenderer@@@Z
extern "C" void MS_ABI impl__Cache_CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAAXAEBVCSize__AEAVCMFCControlRenderer___Z(
    void* pThis, const CSize* pSize, CMFCControlRenderer* pRenderer) {
    (void)pThis; (void)pSize; (void)pRenderer;
}

// RVA 0x199060 (mfc140u):
//   m_Images.DrawEx(pDC, CRect(rect.TopLeft(), m_Images.m_sizeImage), iImageIndex,
//                   ImageAlignHorzLeft, ImageAlignVertTop, CRect(0, 0, 0, 0), alphaSrc);
// Symbol: ?Draw@CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@QEAAXPEAVCDC@@VCRect@@HE@Z
extern "C" void MS_ABI impl__Draw_CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAAXPEAVCDC__VCRect__HE_Z(
    void* pThis, CDC* pDC, CRect rect, int iImageIndex, unsigned char alphaSrc) {
    const SIZE sizeImage = ItemImageSize(pThis);
    CRect rectDest(rect.left, rect.top, rect.left + sizeImage.cx, rect.top + sizeImage.cy);
    impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
        ItemImages(pThis), pDC, rectDest, iImageIndex,
        0 /*ImageAlignHorzLeft*/, 0 /*ImageAlignVertTop*/, CRect(0, 0, 0, 0), alphaSrc);
}

// RVA 0x1990e0 (mfc140u).  Three-slice horizontal draw; sides.cx is the
// left-slice width and sides.cy the right-slice width.  With the image width
// W = m_Images.m_sizeImage.cx and H = its cy, and [l, r) the source columns
// still unused (initially [0, W)):
//   if (sides.cx > 0) {                       // left slice, dest rect as given
//       src = m_bMirror ? [W - sides.cx, W) (r = W - sides.cx) : [0, sides.cx) (l = sides.cx);
//       DrawEx(pDC, rect, i, ImageAlignHorzLeft, ImageAlignVertTop, CRect(src, 0..H), alpha);
//   }
//   if (sides.cy > 0) {                       // right slice, dest rect as given
//       src = m_bMirror ? [l, l + sides.cy) (l += sides.cy) : [r - sides.cy, r) (r -= sides.cy);
//       DrawEx(..., ImageAlignHorzRight, ImageAlignVertTop, ...);
//   }
//   if (r - l > 0) {                          // centre, stretched
//       rect.left += sides.cx; rect.right -= sides.cy;   // unconditional adjust
//       DrawEx(..., ImageAlignHorzStretch, ImageAlignVertTop, CRect(l, 0, r, H), alpha);
//   }
// CSize by value arrives as one 8-byte register (cx low, cy high).
// Symbol: ?DrawY@CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@QEAAXPEAVCDC@@VCRect@@VCSize@@HE@Z
extern "C" void MS_ABI impl__DrawY_CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAAXPEAVCDC__VCRect__VCSize__HE_Z(
    void* pThis, CDC* pDC, CRect rect, long long sides, int iImageIndex, unsigned char alphaSrc) {
    S_BitmapCacheItem* p = static_cast<S_BitmapCacheItem*>(pThis);
    CMFCToolBarImages* pImages = ItemImages(pThis);
    const SIZE sizeImage = ItemImageSize(pThis);
    const int nLeft  = static_cast<int>(static_cast<unsigned long long>(sides) & 0xffffffffu);
    const int nRight = static_cast<int>(static_cast<unsigned long long>(sides) >> 32);
    int l = 0;
    int r = sizeImage.cx;

    if (nLeft > 0) {
        CRect rectSrc;
        if (p->m_bMirror != 0) {
            r = sizeImage.cx - nLeft;
            rectSrc = CRect(r, 0, sizeImage.cx, sizeImage.cy);
        } else {
            l = nLeft;
            rectSrc = CRect(0, 0, nLeft, sizeImage.cy);
        }
        impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
            pImages, pDC, rect, iImageIndex, 0 /*ImageAlignHorzLeft*/, 0 /*ImageAlignVertTop*/, rectSrc, alphaSrc);
    }
    if (nRight > 0) {
        CRect rectSrc;
        if (p->m_bMirror != 0) {
            rectSrc = CRect(l, 0, l + nRight, sizeImage.cy);
            l += nRight;
        } else {
            rectSrc = CRect(r - nRight, 0, r, sizeImage.cy);
            r -= nRight;
        }
        impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
            pImages, pDC, rect, iImageIndex, 2 /*ImageAlignHorzRight*/, 0 /*ImageAlignVertTop*/, rectSrc, alphaSrc);
    }
    if (r - l > 0) {
        rect.left += nLeft;
        rect.right -= nRight;
        impl__DrawEx_CMFCToolBarImages__QEAAHPEAVCDC__VCRect__HW4ImageAlignHorz_1_W4ImageAlignVert_1_1E_Z(
            pImages, pDC, rect, iImageIndex, 3 /*ImageAlignHorzStretch*/, 0 /*ImageAlignVertTop*/,
            CRect(l, 0, r, sizeImage.cy), alphaSrc);
    }
}
