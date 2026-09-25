// CD2DPathGeometry — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CD2DPathGeometry (retail afxrendertarget.h) is the ID2D1PathGeometry wrapper, derived
// from CD2DGeometry, adding one member, m_pPathGeometry.  Every body below except Create
// is transcribed from the retail mfc140u.dll export (the ctor's vftable store excepted --
// see the ctor).  Function bodies are byte-identical in mfc140.dll; every RVA quoted here
// is the mfc140u one.
//
// Open (ordinal 11637) and Stream (ordinal 13938, per mfc_complete_ordinal_mapping.json)
// are missing from the campaign's mfc140u_rva_symbols.json, so `disas.py --u` says NOT
// FOUND for them.  Their RVAs, 0xd6c40 and 0xd6c80, were read from the mfc140u.dll
// export address table by ordinal and disassembled with `disas.py --u --at`.
//
// Layout: OpenMFC's own headers do not declare this class, so every object is laid out by
// a client compiled against the retail header.  CD2DGeometry's m_pGeometry is read INLINE
// there (Get(), operator ID2D1Geometry*(), IsValid()), so it must stay at +0x18, and this
// class's m_pPathGeometry follows it at +0x20 -- see the shadow struct below.

#include <cstddef>
#include <windows.h>
#include <d2d1.h>

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

// Base-class constructor, defined in phase4/src/core/d2d/CD2DGeometry.cpp
// (??0CD2DGeometry@@QEAA@PEAVCRenderTarget@@H@Z: this, CRenderTarget*, BOOL).
extern "C" void* MS_ABI impl___0CD2DGeometry__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int bAutoDestroy);

namespace {

// ---------------------------------------------------------------------------
// Pinned retail layout.
//   CD2DPathGeometry ctor, RVA 0xd6ad0 (mfc140u): calls the CD2DGeometry ctor
//     (RVA 0xd66d0), then `movq $0x0,0x20(%rbx)` -- m_pPathGeometry at +0x20.
//   CD2DGeometry keeps m_pGeometry at +0x18 (see CD2DGeometry.cpp); Attach below writes
//     both +0x20 and +0x18.
//   sizeof == 40: m_nObjectSize of the retail CRuntimeClass that
//     CD2DPathGeometry::GetRuntimeClass (RVA 0xd6ac0, mfc140u) returns (the descriptor at
//     RVA 0x2fe250, mfc140u, reads 40), and D2D_DESC in RuntimeClasses.cpp records the same
//     40.  The deleting destructor (RVA 0xd6b00, mfc140u, slot 1 of the vftable the ctor
//     stores) also loads `mov $0x28,%edx` as the size argument on its flags&4 path (the
//     callee there, RVA 0x27d0, is a folded bare `ret` in mfc140u).
// The CD2DResource members are listed only to pin the offsets; this tree keeps them in a
// side table owned by CD2DResource.cpp and this file never touches them.
struct CD2DPathGeometryShadow {
    void*              vfptr;             // +0x00
    BOOL               m_bIsAutoDestroy;  // +0x08 (CD2DResource)
    void*              m_pParentTarget;   // +0x10 (CD2DResource, CRenderTarget*)
    ID2D1Geometry*     m_pGeometry;       // +0x18 (CD2DGeometry)
    ID2D1PathGeometry* m_pPathGeometry;   // +0x20 (CD2DPathGeometry)
};
static_assert(offsetof(CD2DPathGeometryShadow, m_pGeometry) == 0x18, "CD2DGeometry::m_pGeometry");
static_assert(offsetof(CD2DPathGeometryShadow, m_pPathGeometry) == 0x20, "CD2DPathGeometry::m_pPathGeometry");
static_assert(sizeof(CD2DPathGeometryShadow) == 40, "sizeof(CD2DPathGeometry)");

inline CD2DPathGeometryShadow* Self(void* pThis) {
    return static_cast<CD2DPathGeometryShadow*>(pThis);
}
inline ID2D1PathGeometry* PathGeometryOf(const void* pThis) {
    return static_cast<const CD2DPathGeometryShadow*>(pThis)->m_pPathGeometry;
}

// ---------------------------------------------------------------------------
// ID2D1PathGeometry vtable dispatch.  Slot numbers follow d2d1.h declaration order
// (IUnknown 0-2, ID2D1Resource 3, ID2D1Geometry 4-16, ID2D1PathGeometry 17-20), and each
// one is confirmed by the byte offset the retail body loads from the interface vtable
// (`mov 0xNN(%rax),%rax` before the CFG dispatch `call *0x1802c7b30` (mfc140u VA), an
// indirect call through %rax).
enum PathGeometrySlot : std::size_t {
    kSlotRelease         = 2,   // 0x10  Destroy          (RVA 0xd6c10)
    kSlotOpen            = 17,  // 0x88  Open             (RVA 0xd6c40)
    kSlotStream          = 18,  // 0x90  Stream           (RVA 0xd6c80)
    kSlotGetSegmentCount = 19,  // 0x98  GetSegmentCount  (RVA 0xd6cb0)
    kSlotGetFigureCount  = 20,  // 0xa0  GetFigureCount   (RVA 0xd6cf0)
};

template <typename Fn>
inline Fn Slot(ID2D1PathGeometry* pPath, std::size_t index) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void* const* const*>(pPath))[index]);
}

typedef ULONG   (MS_ABI* PfnRelease)(ID2D1PathGeometry*);
typedef HRESULT (MS_ABI* PfnOpen)(ID2D1PathGeometry*, ID2D1GeometrySink**);
typedef HRESULT (MS_ABI* PfnStream)(ID2D1PathGeometry*, ID2D1GeometrySink*);
typedef HRESULT (MS_ABI* PfnGetCount)(ID2D1PathGeometry*, UINT32*);

} // namespace

// Symbol: ??0CD2DPathGeometry@@QEAA@PEAVCRenderTarget@@H@Z
// Retail RVA 0xd6ad0 (mfc140u): call CD2DGeometry::CD2DGeometry (RVA 0xd66d0) with
// pParentTarget and bAutoDestroy passed through untouched in rdx/r8, store the
// CD2DPathGeometry vftable, zero m_pPathGeometry (+0x20), return this.
// Deviation: the vftable store is not reproduced -- this tree has no CD2DPathGeometry
// vftable to point at (the CD2DGeometry ctor here omits its own for the same reason).
extern "C" void* MS_ABI impl___0CD2DPathGeometry__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int bAutoDestroy) {
    impl___0CD2DGeometry__QEAA_PEAVCRenderTarget__H_Z(pThis, pRenderTarget, bAutoDestroy);
    Self(pThis)->m_pPathGeometry = nullptr;
    return pThis;
}
// Symbol: ?Attach@CD2DPathGeometry@@QEAAXPEAUID2D1PathGeometry@@@Z
// Retail RVA 0xd6d30 (mfc140u):
//   mov %rdx,0x20(%rcx); mov %rdx,0x18(%rcx); ret
// Two plain stores, m_pPathGeometry then m_pGeometry: no Release of the previous
// interface, no AddRef of the new one (both members alias one reference).
extern "C" void MS_ABI impl__Attach_CD2DPathGeometry__QEAAXPEAUID2D1PathGeometry___Z(void* pThis, ID2D1PathGeometry* pResource) {
    Self(pThis)->m_pPathGeometry = pResource;
    Self(pThis)->m_pGeometry = pResource;
}

// Symbol: ?Create@CD2DPathGeometry@@UEAAJPEAVCRenderTarget@@@Z
// STUB.  Retail RVA 0xd6b50 (mfc140u) ignores pRenderTarget and builds the geometry from
// the process-wide Direct2D factory: an inlined AfxGetD2DState() -- a call to
// CProcessLocalObject::GetData (RVA 0x14d100) whose NULL result branches to
// AfxThrowInvalidArgException (RVA 0x227720), as the exported AfxGetD2DState (RVA 0xd24c0)
// does -- then _AFX_D2D_STATE::InitD2D(0, 0) (RVA 0xd22e0, which stores the
// D2D1CreateFactory result at +0x18 of the state); E_FAIL if that factory pointer is NULL
// or if m_pPathGeometry is already non-NULL; otherwise it fetches the state again (same
// inlined NULL -> throw), calls InitD2D(0, 0) again and
// factory->CreatePathGeometry(&m_pPathGeometry) (ID2D1Factory vtable offset 0x50,
// slot 10); on success it copies m_pPathGeometry into m_pGeometry (+0x18) and returns
// S_OK, else it returns that HRESULT.
// Not transcribed: this tree's _AFX_D2D_STATE (detail/CbarcoreSupport.h) holds no
// ID2D1Factory and its InitD2D creates none, and a geometry made from a private factory
// would be rejected by any render target from another factory (D2DERR_WRONG_FACTORY).
// Returns E_FAIL, which is retail's own result when the state holds no factory -- the
// situation this tree is always in.  (The previous placeholder returned 0, i.e. S_OK,
// while creating nothing.)
extern "C" long MS_ABI impl__Create_CD2DPathGeometry__UEAAJPEAVCRenderTarget___Z(void* pThis, void* pRenderTarget) {
    (void)pThis;
    (void)pRenderTarget;
    return E_FAIL;
}

// Symbol: ?Destroy@CD2DPathGeometry@@UEAAXXZ
// Retail RVA 0xd6c10 (mfc140u): if m_pPathGeometry (+0x20) is non-NULL, call its Release
// (vtable offset 0x10, slot 2) and store NULL at +0x20.  m_pGeometry (+0x18) is neither
// released nor cleared, and CD2DGeometry::Destroy is not called.  (So after Create/Attach
// +0x18 still holds the released pointer; retail's ~CD2DGeometry, RVA 0xd6750, calls the
// +0x18 Release-and-clear routine at RVA 0xd2a50 -- retail behaviour, not reproduced
// differently here.)
extern "C" void MS_ABI impl__Destroy_CD2DPathGeometry__UEAAXXZ(void* pThis) {
    ID2D1PathGeometry* pPath = Self(pThis)->m_pPathGeometry;
    if (pPath != nullptr) {
        Slot<PfnRelease>(pPath, kSlotRelease)(pPath);
        Self(pThis)->m_pPathGeometry = nullptr;
    }
}

// Symbol: ?Detach@CD2DPathGeometry@@QEAAPEAUID2D1PathGeometry@@XZ
// Retail RVA 0xd6d40 (mfc140u):
//   mov 0x20(%rcx),%rax; movq $0x0,0x20(%rcx); movq $0x0,0x18(%rcx); ret
// Returns m_pPathGeometry and clears both it and m_pGeometry, without a Release.
extern "C" ID2D1PathGeometry* MS_ABI impl__Detach_CD2DPathGeometry__QEAAPEAUID2D1PathGeometry__XZ(void* pThis) {
    ID2D1PathGeometry* pPath = Self(pThis)->m_pPathGeometry;
    Self(pThis)->m_pPathGeometry = nullptr;
    Self(pThis)->m_pGeometry = nullptr;
    return pPath;
}

// Symbol: ?GetFigureCount@CD2DPathGeometry@@QEBAHXZ
// Retail RVA 0xd6cf0 (mfc140u): -1 if m_pPathGeometry is NULL.  Otherwise a local count
// is initialised to 0xFFFFFFFF, m_pPathGeometry->GetFigureCount(&count) is called
// (vtable offset 0xa0, slot 20), and the local is returned -- the HRESULT is ignored.
extern "C" int MS_ABI impl__GetFigureCount_CD2DPathGeometry__QEBAHXZ(const void* pThis) {
    ID2D1PathGeometry* pPath = PathGeometryOf(pThis);
    if (pPath == nullptr) return -1;
    UINT32 count = 0xFFFFFFFFu;
    Slot<PfnGetCount>(pPath, kSlotGetFigureCount)(pPath, &count);
    return static_cast<int>(count);
}

// Symbol: ?GetSegmentCount@CD2DPathGeometry@@QEBAHXZ
// Retail RVA 0xd6cb0 (mfc140u): same shape as GetFigureCount, vtable offset 0x98,
// slot 19: -1 if m_pPathGeometry is NULL, else the count written by
// GetSegmentCount(&count) into a local pre-set to 0xFFFFFFFF; the HRESULT is ignored.
extern "C" int MS_ABI impl__GetSegmentCount_CD2DPathGeometry__QEBAHXZ(const void* pThis) {
    ID2D1PathGeometry* pPath = PathGeometryOf(pThis);
    if (pPath == nullptr) return -1;
    UINT32 count = 0xFFFFFFFFu;
    Slot<PfnGetCount>(pPath, kSlotGetSegmentCount)(pPath, &count);
    return static_cast<int>(count);
}

// Symbol: ?Open@CD2DPathGeometry@@QEAAPEAUID2D1GeometrySink@@XZ
// Retail RVA 0xd6c40 (mfc140u, ordinal 11637): NULL if m_pPathGeometry is NULL.
// Otherwise a local sink pointer is initialised to NULL, m_pPathGeometry->Open(&sink) is
// called (vtable offset 0x88, slot 17), and the local is returned if the HRESULT is
// non-negative, NULL otherwise (`js` to the zero path).
extern "C" ID2D1GeometrySink* MS_ABI impl__Open_CD2DPathGeometry__QEAAPEAUID2D1GeometrySink__XZ(void* pThis) {
    ID2D1PathGeometry* pPath = PathGeometryOf(pThis);
    if (pPath == nullptr) return nullptr;
    ID2D1GeometrySink* pSink = nullptr;
    HRESULT hr = Slot<PfnOpen>(pPath, kSlotOpen)(pPath, &pSink);
    return hr < 0 ? nullptr : pSink;
}

// Symbol: ?Stream@CD2DPathGeometry@@QEAAHPEAUID2D1GeometrySink@@@Z
// Retail RVA 0xd6c80 (mfc140u, ordinal 13938): FALSE if m_pPathGeometry is NULL, else
// SUCCEEDED(m_pPathGeometry->Stream(geometrySink)) -- vtable offset 0x90, slot 18, with
// geometrySink forwarded untouched in rdx; the result is `not %eax; shr $0x1f,%eax`,
// i.e. hr >= 0.  geometrySink is not NULL-checked.
extern "C" int MS_ABI impl__Stream_CD2DPathGeometry__QEAAHPEAUID2D1GeometrySink___Z(void* pThis, ID2D1GeometrySink* pGeometrySink) {
    ID2D1PathGeometry* pPath = PathGeometryOf(pThis);
    if (pPath == nullptr) return FALSE;
    HRESULT hr = Slot<PfnStream>(pPath, kSlotStream)(pPath, pGeometrySink);
    return hr >= 0 ? TRUE : FALSE;
}
