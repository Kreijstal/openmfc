// CD2DMesh — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CD2DMesh (retail afxrendertarget.h) is the ID2D1Mesh wrapper, derived directly from
// CD2DResource, adding one member, m_pMesh.  Every body below is transcribed from the
// retail mfc140u.dll export (the ctor/dtor vftable stores excepted, and Create's read
// of the render target -- see those functions; retail also routes every COM call through
// the CFG dispatch pointer, made here as a plain indirect call).  Function bodies are byte-identical in
// mfc140.dll; every RVA quoted here is the mfc140u one and is a function entry.
//
// Four of these exports do not resolve by name through the campaign's mfc140u
// RVA-to-symbol map (one name per RVA).  Their RVAs were read from the mfc140u.dll
// export address table by ordinal (per mfc_complete_ordinal_mapping.json):
//   Attach  ordinal 2469  -> RVA 0xd2ac0 (code shared with CD2DBitmap::Attach)
//   Destroy ordinal 3784  -> RVA 0xd2a50 (code shared with CD2DBitmap::Destroy)
//   Detach  ordinal 3821  -> RVA 0xd2ad0 (code shared with CD2DBitmap::Detach)
//   Open    ordinal 11636 -> RVA 0xd70d0 (absent from the map; disassembled with
//                            `disas.py --u --at 0xd70d0`)
//
// Layout: OpenMFC's own headers do not declare this class, so every object is laid out
// by a client compiled against the retail header.  afxrendertarget.h declares Get() and
// operator ID2D1Mesh*() INLINE on m_pMesh, so client code reads +0x18 directly; the
// member is therefore kept inline, where retail keeps it, not in a side table.

#include <cstddef>
#include <windows.h>
#include <d2d1.h>

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

// Base-class thunks, defined in phase4/src/core/d2d/CD2DResource.cpp (signatures
// copied from those definitions).
extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void* self, void* pParentTarget, int bAutoDestroy);
extern "C" void* MS_ABI impl___1CD2DResource__MEAA_XZ(void* self);

// CRenderTarget thunks, defined in phase4/src/core/d2d/CRenderTarget.cpp (there typed
// CRenderTarget*; extern "C", so void* here names the same symbol).
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(void* pThis, void* pRenderTarget);
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(void* pThis);

// Forward declaration: the destructor calls Destroy directly, as retail does.
extern "C" void MS_ABI impl__Destroy_CD2DMesh__UEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Pinned retail layout.
//   CD2DResource ctor, RVA 0xd2800 (mfc140u): stores the vftable at +0x0,
//     bAutoDestroy (r8d) at +0x8, pParentTarget (rdx) at +0x10.
//   CD2DMesh ctor, RVA 0xd6ff0 (mfc140u): calls the CD2DResource ctor, then
//     `movq $0x0,0x18(%rbx)` -- m_pMesh at +0x18 -- and stores its own vftable.
//   sizeof == 32: D2D_DESC(CD2DMesh, 32, ...) in core/d2d/RuntimeClasses.cpp, and the
//     retail scalar deleting destructor (RVA 0xd7020, mfc140u, unexported; it calls
//     ~CD2DMesh at 0xd7070) loads `mov $0x20,%edx` as the size argument on its
//     flags&4 path.
// Only m_pMesh is touched in this file.  This tree's CD2DResource thunks keep the parent
// target and auto-destroy flag in a side table keyed by `this` and never write +0x0,
// +0x8 or +0x10 in the object itself (a deviation owned by CD2DResource.cpp); those
// shadow members are listed only to pin the offsets.
struct CD2DMeshShadow {
    void*      vfptr;             // +0x00
    BOOL       m_bIsAutoDestroy;  // +0x08 (CD2DResource)
    void*      m_pParentTarget;   // +0x10 (CD2DResource, CRenderTarget*)
    ID2D1Mesh* m_pMesh;           // +0x18 (CD2DMesh)
};
static_assert(offsetof(CD2DMeshShadow, m_bIsAutoDestroy) == 0x08, "CD2DResource::m_bIsAutoDestroy");
static_assert(offsetof(CD2DMeshShadow, m_pParentTarget) == 0x10, "CD2DResource::m_pParentTarget");
static_assert(offsetof(CD2DMeshShadow, m_pMesh) == 0x18, "CD2DMesh::m_pMesh");
static_assert(sizeof(CD2DMeshShadow) == 32, "sizeof(CD2DMesh)");

inline CD2DMeshShadow* Self(void* pThis) {
    return static_cast<CD2DMeshShadow*>(pThis);
}

// ---------------------------------------------------------------------------
// COM vtable dispatch.  Slot numbers follow d2d1.h declaration order and each is
// confirmed by the byte offset the retail body loads from the interface vtable
// (`mov 0xNN(%rax),%rax` before the CFG dispatch through 0x1802c7b30 (mfc140u VA; the
// load config's GuardCFDispatchFunctionPointer), i.e. an indirect call/jump through %rax).
//   ID2D1Mesh: IUnknown 0-2, ID2D1Resource::GetFactory 3, Open 4.
//   ID2D1RenderTarget: IUnknown 0-2, GetFactory 3, CreateBitmap 4 ... CreateLayer 13,
//     CreateMesh 14.
enum : std::size_t {
    kSlotRelease    = 2,   // 0x10  Destroy (RVA 0xd2a50)
    kSlotMeshOpen   = 4,   // 0x20  Open    (RVA 0xd70d0)
    kSlotCreateMesh = 14,  // 0x70  Create  (RVA 0xd70a0)
};

template <typename Fn>
inline Fn Slot(void* pInterface, std::size_t index) {
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(pInterface))[index]);
}

typedef ULONG   (MS_ABI* PfnRelease)(void*);
typedef HRESULT (MS_ABI* PfnMeshOpen)(ID2D1Mesh*, ID2D1TessellationSink**);
typedef HRESULT (MS_ABI* PfnCreateMesh)(void* /*ID2D1RenderTarget*/, ID2D1Mesh**);

} // namespace

// Symbol: ??0CD2DMesh@@QEAA@PEAVCRenderTarget@@H@Z
// Retail RVA 0xd6ff0 (mfc140u): call CD2DResource::CD2DResource (RVA 0xd2800) with
// pParentTarget and bAutoDestroy passed through untouched in rdx/r8, zero m_pMesh
// (+0x18), store the CD2DMesh vftable, return this.
// Deviation: the vftable store is not reproduced -- this tree has no CD2DMesh vftable
// to point at (the CD2DGeometry and CD2DPathGeometry ctors here omit theirs too).
// (The previous placeholder chained to nothing and left +0x18 uninitialised, which
// Create's m_pMesh test below depends on.)
extern "C" void* MS_ABI impl___0CD2DMesh__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int bAutoDestroy) {
    impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(pThis, pRenderTarget, bAutoDestroy);
    Self(pThis)->m_pMesh = nullptr;
    return pThis;
}
// Symbol: ??1CD2DMesh@@UEAA@XZ
// Retail RVA 0xd7070 (mfc140u): store the CD2DMesh vftable, call CD2DMesh::Destroy
// directly (non-virtual `call 0x1800d2a50`, the Destroy export's code), then tail-jump
// to CD2DResource::~CD2DResource (RVA 0xd28a0).  Destroy is called unconditionally --
// retail does not consult m_bIsAutoDestroy here.
// Deviation: the vftable store is omitted (no vftable in this tree, as in the ctor).
extern "C" void MS_ABI impl___1CD2DMesh__UEAA_XZ(void* pThis) {
    impl__Destroy_CD2DMesh__UEAAXXZ(pThis);
    impl___1CD2DResource__MEAA_XZ(pThis);
}
// Symbol: ?Attach@CD2DMesh@@QEAAXPEAUID2D1Mesh@@@Z
// Retail RVA 0xd2ac0 (mfc140u, ordinal 2469, code shared with CD2DBitmap::Attach):
//   mov %rdx,0x18(%rcx); ret
// A plain store: the previous interface is neither released nor is the new one AddRef'd.
extern "C" void MS_ABI impl__Attach_CD2DMesh__QEAAXPEAUID2D1Mesh___Z(void* pThis, ID2D1Mesh* pResource) {
    Self(pThis)->m_pMesh = pResource;
}

// Symbol: ?Create@CD2DMesh@@UEAAJPEAVCRenderTarget@@@Z
// Retail RVA 0xd70a0 (mfc140u), in order:
//   if (pRenderTarget == NULL || pRenderTarget->m_pRenderTarget (+0x8) == NULL
//       || m_pMesh (+0x18) != NULL)
//       return E_FAIL (0x80004005);
//   return m_pRenderTarget->CreateMesh(&m_pMesh);   // vtable byte 0x70, slot 14
// The COM call is a tail jump with &this->m_pMesh as the out-pointer, so D2D writes the
// new mesh straight into +0x18 and its HRESULT is returned unchanged.
// Deviations:
//   * The ID2D1RenderTarget* is not read from +0x8 of the CRenderTarget: this tree's
//     CRenderTarget (core/d2d/CRenderTarget.cpp) keeps it in a side table keyed by the
//     object.  It is read through the CRenderTarget Detach/Attach thunks (Detach returns
//     it and nulls the slot; Attach with the same pointer puts it back and releases
//     nothing because the slot is then NULL) -- the same non-atomic read-by-round-trip
//     core/d2d/CD2DSolidColorBrush.cpp uses.  A NULL result takes the E_FAIL path, as
//     retail's +0x8 test does.
//   * Retail makes the COM call through the CFG dispatch pointer; here it is a plain
//     indirect call.
extern "C" long MS_ABI impl__Create_CD2DMesh__UEAAJPEAVCRenderTarget___Z(void* pThis, void* pRenderTarget) {
    if (pRenderTarget == nullptr) {
        return static_cast<long>(E_FAIL);
    }
    void* pD2DTarget = impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(pRenderTarget);
    if (pD2DTarget == nullptr) {
        return static_cast<long>(E_FAIL);
    }
    impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(pRenderTarget, pD2DTarget);

    if (Self(pThis)->m_pMesh != nullptr) {
        return static_cast<long>(E_FAIL);
    }
    return static_cast<long>(Slot<PfnCreateMesh>(pD2DTarget, kSlotCreateMesh)(pD2DTarget, &Self(pThis)->m_pMesh));
}

// Symbol: ?Destroy@CD2DMesh@@UEAAXXZ
// Retail RVA 0xd2a50 (mfc140u, ordinal 3784, code shared with CD2DBitmap::Destroy):
// if m_pMesh (+0x18) is non-NULL, call its Release (vtable offset 0x10, slot 2) and
// store NULL at +0x18; otherwise do nothing.
extern "C" void MS_ABI impl__Destroy_CD2DMesh__UEAAXXZ(void* pThis) {
    ID2D1Mesh* pMesh = Self(pThis)->m_pMesh;
    if (pMesh != nullptr) {
        Slot<PfnRelease>(pMesh, kSlotRelease)(pMesh);
        Self(pThis)->m_pMesh = nullptr;
    }
}

// Symbol: ?Detach@CD2DMesh@@QEAAPEAUID2D1Mesh@@XZ
// Retail RVA 0xd2ad0 (mfc140u, ordinal 3821, code shared with CD2DBitmap::Detach):
//   mov 0x18(%rcx),%rax; movq $0x0,0x18(%rcx); ret
// Returns m_pMesh and clears it, without a Release.
extern "C" ID2D1Mesh* MS_ABI impl__Detach_CD2DMesh__QEAAPEAUID2D1Mesh__XZ(void* pThis) {
    ID2D1Mesh* pMesh = Self(pThis)->m_pMesh;
    Self(pThis)->m_pMesh = nullptr;
    return pMesh;
}

// Symbol: ?Open@CD2DMesh@@QEAAPEAUID2D1TessellationSink@@XZ
// Retail RVA 0xd70d0 (mfc140u, ordinal 11636): NULL if m_pMesh (+0x18) is NULL.  Otherwise a local sink pointer is
// initialised to NULL, m_pMesh->Open(&sink) is called (vtable offset 0x20, slot 4), and
// the local is returned if the HRESULT is non-negative, NULL otherwise (`js` to the
// zero path).
extern "C" ID2D1TessellationSink* MS_ABI impl__Open_CD2DMesh__QEAAPEAUID2D1TessellationSink__XZ(void* pThis) {
    ID2D1Mesh* pMesh = Self(pThis)->m_pMesh;
    if (pMesh == nullptr) return nullptr;
    ID2D1TessellationSink* pSink = nullptr;
    HRESULT hr = Slot<PfnMeshOpen>(pMesh, kSlotMeshOpen)(pMesh, &pSink);
    return hr < 0 ? nullptr : pSink;
}
