// CD2DGeometry — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CD2DGeometry (afxrendertarget.h) is the ID2D1Geometry wrapper, derived from
// CD2DResource.  Every body below is transcribed from the retail mfc140u.dll export
// (the ctor/dtor vftable stores excepted -- see those two functions); function bodies
// are byte-identical in mfc140.dll, but every RVA quoted here is the mfc140u one.
// Eight of these exports do not resolve by name through the campaign's mfc140u
// RVA-to-symbol map (one name per RVA): Attach, Detach and Destroy share their code
// with CD2DBitmap's (the map names those RVAs after CD2DBitmap), and Outline, Simplify,
// StrokeContainsPoint, Tessellate and Widen are absent from it.  Their RVAs were read
// from the mfc140u.dll export address table by ordinal (ordinals 2466, 3818, 3780,
// 11680, 13867, 13953, 13972, 14196 per mfc_complete_ordinal_mapping.json).
//
// Unlike the side-table D2D wrappers in this directory (CD2DBrush, CD2DBitmap), the
// wrapped interface is kept where retail keeps it: inline at +0x18 of the object.  This
// is required, not just faithful: the retail afxrendertarget.h declares Get(),
// operator ID2D1Geometry*() and IsValid() INLINE on m_pGeometry, so client code reads
// +0x18 directly.  OpenMFC's own headers do not declare the class, so every object is
// laid out by a client compiled against the retail header: retail sizeof(CD2DGeometry)
// is 32 (m_nObjectSize of the retail CRuntimeClass, which D2D_DESC in RuntimeClasses.cpp
// also records); derived objects (CD2DPathGeometry, 40) are larger but share +0x18.
//
// KNOWN HAZARD, outside this file: this tree's CD2DPathGeometry constructor does not
// chain to the CD2DGeometry constructor below, so a CD2DPathGeometry's +0x18 is never
// zeroed.  Retail's CD2DPathGeometry ctor (RVA 0xd6ad0, mfc140u) calls this ctor first.
// Until CD2DPathGeometry.cpp is fixed, the base methods here -- and ~CD2DGeometry, which
// the client's implicit ~CD2DPathGeometry calls -- will read an uninitialised +0x18.

#include <cstddef>
#include <cstring>
#include <windows.h>
#include <d2d1.h>

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

// Base-class thunks, defined in phase4/src/core/d2d/CD2DResource.cpp.
extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void* self, void* pParentTarget, int bAutoDestroy);
extern "C" void* MS_ABI impl___1CD2DResource__MEAA_XZ(void* self);

// Forward declaration: the destructor calls Destroy directly, as retail does.
extern "C" void MS_ABI impl__Destroy_CD2DGeometry__UEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Pinned retail layout.
//   CD2DResource ctor, RVA 0xd2800 (mfc140u): stores the vftable at +0x0,
//     bAutoDestroy (r8d) at +0x8, pParentTarget (rdx) at +0x10.
//   CD2DGeometry ctor, RVA 0xd66d0 (mfc140u): calls the CD2DResource ctor, then
//     `movq $0x0,0x18(%rbx)` -- m_pGeometry at +0x18 -- and stores its own vftable.
//   sizeof == 32: m_nObjectSize in the retail CRuntimeClass returned by
//     CD2DGeometry::GetRuntimeClass (RVA 0xd66c0, mfc140u).
// Only m_pGeometry is touched in this file.  This tree's CD2DResource thunks keep the
// parent target and auto-destroy flag in a side table keyed by `this` and never write
// +0x0, +0x8 or +0x10 in the object itself (a deviation from retail, owned by
// CD2DResource.cpp); those two shadow members are listed only to pin the offsets.
struct CD2DGeometryShadow {
    void*          vfptr;             // +0x00
    BOOL           m_bIsAutoDestroy;  // +0x08 (CD2DResource)
    void*          m_pParentTarget;   // +0x10 (CD2DResource, CRenderTarget*)
    ID2D1Geometry* m_pGeometry;       // +0x18 (CD2DGeometry)
};
static_assert(offsetof(CD2DGeometryShadow, m_bIsAutoDestroy) == 0x08, "CD2DResource::m_bIsAutoDestroy");
static_assert(offsetof(CD2DGeometryShadow, m_pParentTarget) == 0x10, "CD2DResource::m_pParentTarget");
static_assert(offsetof(CD2DGeometryShadow, m_pGeometry) == 0x18, "CD2DGeometry::m_pGeometry");
static_assert(sizeof(CD2DGeometryShadow) == 32, "sizeof(CD2DGeometry)");

inline CD2DGeometryShadow* Self(void* pThis) {
    return static_cast<CD2DGeometryShadow*>(pThis);
}
inline ID2D1Geometry* GeometryOf(const void* pThis) {
    return static_cast<const CD2DGeometryShadow*>(pThis)->m_pGeometry;
}

// ---------------------------------------------------------------------------
// ID2D1Geometry vtable dispatch.  Slot numbers follow d2d1.h declaration order
// (IUnknown 0-2, ID2D1Resource::GetFactory 3, then ID2D1Geometry 4..16), and each one
// is confirmed by the byte offset the retail body loads from the interface vtable
// (`mov 0xNN(%rax),%rax` before the CFG dispatch `call *0x1802c7b30`, which is
// mfc140u's GuardCFDispatchFunctionPointer, i.e. an indirect call through %rax).
enum GeometrySlot : std::size_t {
    kSlotRelease              = 2,   // 0x10  Destroy               (RVA 0xd2a50)
    kSlotGetBounds            = 4,   // 0x20  GetBounds             (RVA 0xd68c0)
    kSlotGetWidenedBounds     = 5,   // 0x28  GetWidenedBounds      (RVA 0xd68f0)
    kSlotStrokeContainsPoint  = 6,   // 0x30  StrokeContainsPoint   (RVA 0xd6970)
    kSlotFillContainsPoint    = 7,   // 0x38  FillContainsPoint     (RVA 0xd6930)
    kSlotCompareWithGeometry  = 8,   // 0x40  CompareWithGeometry   (RVA 0xd67d0)
    kSlotSimplify             = 9,   // 0x48  Simplify              (RVA 0xd6a00)
    kSlotTessellate           = 10,  // 0x50  Tessellate            (RVA 0xd6a40)
    kSlotCombineWithGeometry  = 11,  // 0x58  CombineWithGeometry   (RVA 0xd6780)
    kSlotOutline              = 12,  // 0x60  Outline               (RVA 0xd69d0)
    kSlotComputeArea          = 13,  // 0x68  ComputeArea           (RVA 0xd6820)
    kSlotComputeLength        = 14,  // 0x70  ComputeLength         (RVA 0xd6850)
    kSlotComputePointAtLength = 15,  // 0x78  ComputePointAtLength  (RVA 0xd6880)
    kSlotWiden                = 16,  // 0x80  Widen                 (RVA 0xd6a70)
};

template <typename Fn>
inline Fn Slot(ID2D1Geometry* pGeometry, std::size_t index) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void* const* const*>(pGeometry))[index]);
}

typedef ULONG   (MS_ABI* PfnRelease)(ID2D1Geometry*);
typedef HRESULT (MS_ABI* PfnGetBounds)(ID2D1Geometry*, const D2D1_MATRIX_3X2_F*, D2D1_RECT_F*);
typedef HRESULT (MS_ABI* PfnGetWidenedBounds)(ID2D1Geometry*, FLOAT, ID2D1StrokeStyle*, const D2D1_MATRIX_3X2_F*, FLOAT, D2D1_RECT_F*);
typedef HRESULT (MS_ABI* PfnStrokeContainsPoint)(ID2D1Geometry*, D2D1_POINT_2F, FLOAT, ID2D1StrokeStyle*, const D2D1_MATRIX_3X2_F*, FLOAT, BOOL*);
typedef HRESULT (MS_ABI* PfnFillContainsPoint)(ID2D1Geometry*, D2D1_POINT_2F, const D2D1_MATRIX_3X2_F*, FLOAT, BOOL*);
typedef HRESULT (MS_ABI* PfnCompareWithGeometry)(ID2D1Geometry*, ID2D1Geometry*, const D2D1_MATRIX_3X2_F*, FLOAT, D2D1_GEOMETRY_RELATION*);
typedef HRESULT (MS_ABI* PfnSimplify)(ID2D1Geometry*, D2D1_GEOMETRY_SIMPLIFICATION_OPTION, const D2D1_MATRIX_3X2_F*, FLOAT, ID2D1SimplifiedGeometrySink*);
typedef HRESULT (MS_ABI* PfnTessellate)(ID2D1Geometry*, const D2D1_MATRIX_3X2_F*, FLOAT, ID2D1TessellationSink*);
typedef HRESULT (MS_ABI* PfnCombineWithGeometry)(ID2D1Geometry*, ID2D1Geometry*, D2D1_COMBINE_MODE, const D2D1_MATRIX_3X2_F*, FLOAT, ID2D1SimplifiedGeometrySink*);
typedef HRESULT (MS_ABI* PfnOutline)(ID2D1Geometry*, const D2D1_MATRIX_3X2_F*, FLOAT, ID2D1SimplifiedGeometrySink*);
typedef HRESULT (MS_ABI* PfnComputeScalar)(ID2D1Geometry*, const D2D1_MATRIX_3X2_F*, FLOAT, FLOAT*);
typedef HRESULT (MS_ABI* PfnComputePointAtLength)(ID2D1Geometry*, FLOAT, const D2D1_MATRIX_3X2_F*, FLOAT, D2D1_POINT_2F*, D2D1_POINT_2F*);
typedef HRESULT (MS_ABI* PfnWiden)(ID2D1Geometry*, FLOAT, ID2D1StrokeStyle*, const D2D1_MATRIX_3X2_F*, FLOAT, ID2D1SimplifiedGeometrySink*);

// CD2DPointF is passed BY VALUE (8 bytes: two floats) in an integer register under the
// MS x64 ABI -- retail FillContainsPoint forwards %rdx untouched as the D2D1_POINT_2F
// argument; StrokeContainsPoint parks it in %xmm1 while it reuses %rdx to stage stack
// arguments, then restores it (`movq %rdx,%xmm1` ... `movq %xmm1,%rdx`) before the
// call.  It is modelled here as a 64-bit integer and re-typed.
inline D2D1_POINT_2F PointFromBits(unsigned long long bits) {
    D2D1_POINT_2F pt;
    static_assert(sizeof(pt) == sizeof(bits), "D2D1_POINT_2F is 8 bytes");
    std::memcpy(&pt, &bits, sizeof(pt));
    return pt;
}

// Every BOOL-returning retail body ends in `not %eax; shr $0x1f,%eax`, i.e. hr >= 0.
inline int Succeeded(HRESULT hr) { return hr >= 0 ? TRUE : FALSE; }

} // namespace

// Symbol: ??0CD2DGeometry@@QEAA@PEAVCRenderTarget@@H@Z
// Retail RVA 0xd66d0 (mfc140u): call CD2DResource::CD2DResource(pParentTarget,
// bAutoDestroy) (RVA 0xd2800), zero m_pGeometry (+0x18), store the CD2DGeometry
// vftable, return this.
// Deviation: the vftable store is not reproduced -- this tree has no CD2DGeometry
// vftable to point at (the CD2DBitmap and CD2DPathGeometry ctors here omit it too).
extern "C" void* MS_ABI impl___0CD2DGeometry__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int bAutoDestroy) {
    impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(pThis, pRenderTarget, bAutoDestroy);
    Self(pThis)->m_pGeometry = nullptr;
    return pThis;
}
// Symbol: ??1CD2DGeometry@@UEAA@XZ
// Retail RVA 0xd6750 (mfc140u): store the CD2DGeometry vftable, call
// CD2DGeometry::Destroy directly (non-virtual `call 0x1800d2a50`, the Destroy export),
// then tail-jump to CD2DResource::~CD2DResource (RVA 0xd28a0).  Destroy is called
// unconditionally -- retail does not consult m_bIsAutoDestroy here.
// Deviation: the vftable store is omitted (no vftable in this tree, as in the ctor).
extern "C" void MS_ABI impl___1CD2DGeometry__UEAA_XZ(void* pThis) {
    impl__Destroy_CD2DGeometry__UEAAXXZ(pThis);
    impl___1CD2DResource__MEAA_XZ(pThis);
}
// Symbol: ?Attach@CD2DGeometry@@QEAAXPEAUID2D1Geometry@@@Z
// Retail RVA 0xd2ac0 (mfc140u, code shared with CD2DBitmap::Attach):
//   mov %rdx,0x18(%rcx); ret
// A plain store: the previous interface is neither released nor is the new one AddRef'd.
extern "C" void MS_ABI impl__Attach_CD2DGeometry__QEAAXPEAUID2D1Geometry___Z(void* pThis, ID2D1Geometry* pResource) {
    Self(pThis)->m_pGeometry = pResource;
}

// Symbol: ?CombineWithGeometry@CD2DGeometry@@QEBAHAEAV1@W4D2D1_COMBINE_MODE@@AEBUD2D_MATRIX_3X2_F@@PEAUID2D1SimplifiedGeometrySink@@M@Z
// Retail RVA 0xd6780 (mfc140u): FALSE if this->m_pGeometry is NULL, FALSE if
// inputGeometry.m_pGeometry (+0x18 of the referenced object) is NULL; otherwise
// SUCCEEDED(m_pGeometry->CombineWithGeometry(input.m_pGeometry, mode, &transform,
// flatteningTolerance, geometrySink)) -- vtable offset 0x58, slot 11.
extern "C" int MS_ABI impl__CombineWithGeometry_CD2DGeometry__QEBAHAEAV1_W4D2D1_COMBINE_MODE__AEBUD2D_MATRIX_3X2_F__PEAUID2D1SimplifiedGeometrySink__M_Z(
    const void* pThis, void* pInputGeometry, D2D1_COMBINE_MODE combineMode,
    const D2D1_MATRIX_3X2_F* pInputGeometryTransform, ID2D1SimplifiedGeometrySink* pGeometrySink,
    float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    ID2D1Geometry* pInput = GeometryOf(pInputGeometry);
    if (pInput == nullptr) return FALSE;
    return Succeeded(Slot<PfnCombineWithGeometry>(pGeometry, kSlotCombineWithGeometry)(
        pGeometry, pInput, combineMode, pInputGeometryTransform, flatteningTolerance, pGeometrySink));
}

// Symbol: ?CompareWithGeometry@CD2DGeometry@@QEBA?AW4D2D1_GEOMETRY_RELATION@@AEAV1@AEBUD2D_MATRIX_3X2_F@@M@Z
// Retail RVA 0xd67d0 (mfc140u): returns 0 (D2D1_GEOMETRY_RELATION_UNKNOWN) if either
// this->m_pGeometry or inputGeometry.m_pGeometry is NULL.  Otherwise a local relation is
// initialised to 0, m_pGeometry->CompareWithGeometry(input.m_pGeometry, &transform,
// flatteningTolerance, &relation) is called (vtable offset 0x40, slot 8), and the local
// is returned if the HRESULT is non-negative (`js` to the zero path otherwise).
extern "C" D2D1_GEOMETRY_RELATION MS_ABI impl__CompareWithGeometry_CD2DGeometry__QEBA_AW4D2D1_GEOMETRY_RELATION__AEAV1_AEBUD2D_MATRIX_3X2_F__M_Z(
    const void* pThis, void* pInputGeometry, const D2D1_MATRIX_3X2_F* pInputGeometryTransform,
    float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return D2D1_GEOMETRY_RELATION_UNKNOWN;
    ID2D1Geometry* pInput = GeometryOf(pInputGeometry);
    if (pInput == nullptr) return D2D1_GEOMETRY_RELATION_UNKNOWN;
    D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
    HRESULT hr = Slot<PfnCompareWithGeometry>(pGeometry, kSlotCompareWithGeometry)(
        pGeometry, pInput, pInputGeometryTransform, flatteningTolerance, &relation);
    return hr < 0 ? D2D1_GEOMETRY_RELATION_UNKNOWN : relation;
}

// Symbol: ?ComputeArea@CD2DGeometry@@QEBAHAEBUD2D_MATRIX_3X2_F@@AEAMM@Z
// Retail RVA 0xd6820 (mfc140u): FALSE if m_pGeometry is NULL, else
// SUCCEEDED(m_pGeometry->ComputeArea(&transform, flatteningTolerance, &area)) --
// vtable offset 0x68, slot 13 (retail moves area to r9 and the tolerance to xmm2).
extern "C" int MS_ABI impl__ComputeArea_CD2DGeometry__QEBAHAEBUD2D_MATRIX_3X2_F__AEAMM_Z(
    const void* pThis, const D2D1_MATRIX_3X2_F* pWorldTransform, float* pArea, float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnComputeScalar>(pGeometry, kSlotComputeArea)(
        pGeometry, pWorldTransform, flatteningTolerance, pArea));
}

// Symbol: ?ComputeLength@CD2DGeometry@@QEBAHAEBUD2D_MATRIX_3X2_F@@AEAMM@Z
// Retail RVA 0xd6850 (mfc140u): same shape as ComputeArea, vtable offset 0x70, slot 14:
// FALSE if m_pGeometry is NULL, else SUCCEEDED(ComputeLength(&transform, tol, &length)).
extern "C" int MS_ABI impl__ComputeLength_CD2DGeometry__QEBAHAEBUD2D_MATRIX_3X2_F__AEAMM_Z(
    const void* pThis, const D2D1_MATRIX_3X2_F* pWorldTransform, float* pLength, float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnComputeScalar>(pGeometry, kSlotComputeLength)(
        pGeometry, pWorldTransform, flatteningTolerance, pLength));
}

// Symbol: ?ComputePointAtLength@CD2DGeometry@@QEBAHMAEBUD2D_MATRIX_3X2_F@@AEAVCD2DPointF@@1M@Z
// Retail RVA 0xd6880 (mfc140u): FALSE if m_pGeometry is NULL, else
// SUCCEEDED(m_pGeometry->ComputePointAtLength(length, &transform, flatteningTolerance,
// &point, &unitTangentVector)) -- vtable offset 0x78, slot 15.  CD2DPointF is layout-
// identical to D2D1_POINT_2F (two floats), and retail passes the references through as is.
extern "C" int MS_ABI impl__ComputePointAtLength_CD2DGeometry__QEBAHMAEBUD2D_MATRIX_3X2_F__AEAVCD2DPointF__1M_Z(
    const void* pThis, float length, const D2D1_MATRIX_3X2_F* pWorldTransform, void* pPoint,
    void* pUnitTangentVector, float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnComputePointAtLength>(pGeometry, kSlotComputePointAtLength)(
        pGeometry, length, pWorldTransform, flatteningTolerance,
        static_cast<D2D1_POINT_2F*>(pPoint), static_cast<D2D1_POINT_2F*>(pUnitTangentVector)));
}

// Symbol: ?Destroy@CD2DGeometry@@UEAAXXZ
// Retail RVA 0xd2a50 (mfc140u, code shared with CD2DBitmap::Destroy): if m_pGeometry is
// non-NULL, call its Release (vtable offset 0x10, slot 2) and then store NULL at +0x18.
extern "C" void MS_ABI impl__Destroy_CD2DGeometry__UEAAXXZ(void* pThis) {
    ID2D1Geometry* pGeometry = Self(pThis)->m_pGeometry;
    if (pGeometry != nullptr) {
        Slot<PfnRelease>(pGeometry, kSlotRelease)(pGeometry);
        Self(pThis)->m_pGeometry = nullptr;
    }
}

// Symbol: ?Detach@CD2DGeometry@@QEAAPEAUID2D1Geometry@@XZ
// Retail RVA 0xd2ad0 (mfc140u, code shared with CD2DBitmap::Detach):
//   mov 0x18(%rcx),%rax; movq $0x0,0x18(%rcx); ret
extern "C" ID2D1Geometry* MS_ABI impl__Detach_CD2DGeometry__QEAAPEAUID2D1Geometry__XZ(void* pThis) {
    ID2D1Geometry* pGeometry = Self(pThis)->m_pGeometry;
    Self(pThis)->m_pGeometry = nullptr;
    return pGeometry;
}

// Symbol: ?FillContainsPoint@CD2DGeometry@@QEBAHVCD2DPointF@@AEBUD2D_MATRIX_3X2_F@@PEAHM@Z
// Retail RVA 0xd6930 (mfc140u): FALSE if m_pGeometry is NULL, else
// SUCCEEDED(m_pGeometry->FillContainsPoint(point, &transform, flatteningTolerance,
// contains)) -- vtable offset 0x38, slot 7.  The by-value point stays in %rdx.
extern "C" int MS_ABI impl__FillContainsPoint_CD2DGeometry__QEBAHVCD2DPointF__AEBUD2D_MATRIX_3X2_F__PEAHM_Z(
    const void* pThis, unsigned long long point, const D2D1_MATRIX_3X2_F* pWorldTransform, BOOL* pContains,
    float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnFillContainsPoint>(pGeometry, kSlotFillContainsPoint)(
        pGeometry, PointFromBits(point), pWorldTransform, flatteningTolerance, pContains));
}

// Symbol: ?GetBounds@CD2DGeometry@@QEBAHAEBUD2D_MATRIX_3X2_F@@AEAVCD2DRectF@@@Z
// Retail RVA 0xd68c0 (mfc140u): FALSE if m_pGeometry is NULL, else
// SUCCEEDED(m_pGeometry->GetBounds(&transform, &bounds)) -- vtable offset 0x20, slot 4.
// CD2DRectF is layout-identical to D2D1_RECT_F and is passed through unchanged.
extern "C" int MS_ABI impl__GetBounds_CD2DGeometry__QEBAHAEBUD2D_MATRIX_3X2_F__AEAVCD2DRectF___Z(
    const void* pThis, const D2D1_MATRIX_3X2_F* pWorldTransform, void* pBounds) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnGetBounds>(pGeometry, kSlotGetBounds)(
        pGeometry, pWorldTransform, static_cast<D2D1_RECT_F*>(pBounds)));
}

// Symbol: ?GetWidenedBounds@CD2DGeometry@@QEBAHMPEAUID2D1StrokeStyle@@AEBUD2D_MATRIX_3X2_F@@AEAVCD2DRectF@@M@Z
// Retail RVA 0xd68f0 (mfc140u): FALSE if m_pGeometry is NULL, else
// SUCCEEDED(m_pGeometry->GetWidenedBounds(strokeWidth, strokeStyle, &transform,
// flatteningTolerance, &bounds)) -- vtable offset 0x28, slot 5.
extern "C" int MS_ABI impl__GetWidenedBounds_CD2DGeometry__QEBAHMPEAUID2D1StrokeStyle__AEBUD2D_MATRIX_3X2_F__AEAVCD2DRectF__M_Z(
    const void* pThis, float strokeWidth, ID2D1StrokeStyle* pStrokeStyle, const D2D1_MATRIX_3X2_F* pWorldTransform,
    void* pBounds, float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnGetWidenedBounds>(pGeometry, kSlotGetWidenedBounds)(
        pGeometry, strokeWidth, pStrokeStyle, pWorldTransform, flatteningTolerance,
        static_cast<D2D1_RECT_F*>(pBounds)));
}

// Symbol: ?Outline@CD2DGeometry@@QEBAHAEBUD2D_MATRIX_3X2_F@@PEAUID2D1SimplifiedGeometrySink@@M@Z
// Retail RVA 0xd69d0 (mfc140u; export ordinal 11680, not in the RVA map): FALSE if
// m_pGeometry is NULL, else SUCCEEDED(m_pGeometry->Outline(&transform,
// flatteningTolerance, geometrySink)) -- vtable offset 0x60, slot 12.
extern "C" int MS_ABI impl__Outline_CD2DGeometry__QEBAHAEBUD2D_MATRIX_3X2_F__PEAUID2D1SimplifiedGeometrySink__M_Z(
    const void* pThis, const D2D1_MATRIX_3X2_F* pWorldTransform, ID2D1SimplifiedGeometrySink* pGeometrySink,
    float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnOutline>(pGeometry, kSlotOutline)(
        pGeometry, pWorldTransform, flatteningTolerance, pGeometrySink));
}

// Symbol: ?Simplify@CD2DGeometry@@QEBAHW4D2D1_GEOMETRY_SIMPLIFICATION_OPTION@@AEBUD2D_MATRIX_3X2_F@@PEAUID2D1SimplifiedGeometrySink@@M@Z
// Retail RVA 0xd6a00 (mfc140u; export ordinal 13867, not in the RVA map): FALSE if
// m_pGeometry is NULL, else SUCCEEDED(m_pGeometry->Simplify(option, &transform,
// flatteningTolerance, geometrySink)) -- vtable offset 0x48, slot 9.
extern "C" int MS_ABI impl__Simplify_CD2DGeometry__QEBAHW4D2D1_GEOMETRY_SIMPLIFICATION_OPTION__AEBUD2D_MATRIX_3X2_F__PEAUID2D1SimplifiedGeometrySink__M_Z(
    const void* pThis, D2D1_GEOMETRY_SIMPLIFICATION_OPTION simplificationOption,
    const D2D1_MATRIX_3X2_F* pWorldTransform, ID2D1SimplifiedGeometrySink* pGeometrySink,
    float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnSimplify>(pGeometry, kSlotSimplify)(
        pGeometry, simplificationOption, pWorldTransform, flatteningTolerance, pGeometrySink));
}

// Symbol: ?StrokeContainsPoint@CD2DGeometry@@QEBAHVCD2DPointF@@MPEAUID2D1StrokeStyle@@AEBUD2D_MATRIX_3X2_F@@PEAHM@Z
// Retail RVA 0xd6970 (mfc140u; export ordinal 13953, not in the RVA map): FALSE if
// m_pGeometry is NULL, else SUCCEEDED(m_pGeometry->StrokeContainsPoint(point,
// strokeWidth, strokeStyle, &transform, flatteningTolerance, contains)) -- vtable
// offset 0x30, slot 6.  The by-value point is carried through %rdx.
extern "C" int MS_ABI impl__StrokeContainsPoint_CD2DGeometry__QEBAHVCD2DPointF__MPEAUID2D1StrokeStyle__AEBUD2D_MATRIX_3X2_F__PEAHM_Z(
    const void* pThis, unsigned long long point, float strokeWidth, ID2D1StrokeStyle* pStrokeStyle,
    const D2D1_MATRIX_3X2_F* pWorldTransform, BOOL* pContains, float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnStrokeContainsPoint>(pGeometry, kSlotStrokeContainsPoint)(
        pGeometry, PointFromBits(point), strokeWidth, pStrokeStyle, pWorldTransform,
        flatteningTolerance, pContains));
}

// Symbol: ?Tessellate@CD2DGeometry@@QEBAHAEBUD2D_MATRIX_3X2_F@@PEAUID2D1TessellationSink@@M@Z
// Retail RVA 0xd6a40 (mfc140u; export ordinal 13972, not in the RVA map): FALSE if
// m_pGeometry is NULL, else SUCCEEDED(m_pGeometry->Tessellate(&transform,
// flatteningTolerance, tessellationSink)) -- vtable offset 0x50, slot 10.
extern "C" int MS_ABI impl__Tessellate_CD2DGeometry__QEBAHAEBUD2D_MATRIX_3X2_F__PEAUID2D1TessellationSink__M_Z(
    const void* pThis, const D2D1_MATRIX_3X2_F* pWorldTransform, ID2D1TessellationSink* pTessellationSink,
    float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnTessellate>(pGeometry, kSlotTessellate)(
        pGeometry, pWorldTransform, flatteningTolerance, pTessellationSink));
}

// Symbol: ?Widen@CD2DGeometry@@QEBAHMPEAUID2D1StrokeStyle@@AEBUD2D_MATRIX_3X2_F@@PEAUID2D1SimplifiedGeometrySink@@M@Z
// Retail RVA 0xd6a70 (mfc140u; export ordinal 14196, not in the RVA map): FALSE if
// m_pGeometry is NULL, else SUCCEEDED(m_pGeometry->Widen(strokeWidth, strokeStyle,
// &transform, flatteningTolerance, geometrySink)) -- vtable offset 0x80, slot 16.
extern "C" int MS_ABI impl__Widen_CD2DGeometry__QEBAHMPEAUID2D1StrokeStyle__AEBUD2D_MATRIX_3X2_F__PEAUID2D1SimplifiedGeometrySink__M_Z(
    const void* pThis, float strokeWidth, ID2D1StrokeStyle* pStrokeStyle, const D2D1_MATRIX_3X2_F* pWorldTransform,
    ID2D1SimplifiedGeometrySink* pGeometrySink, float flatteningTolerance) {
    ID2D1Geometry* pGeometry = GeometryOf(pThis);
    if (pGeometry == nullptr) return FALSE;
    return Succeeded(Slot<PfnWiden>(pGeometry, kSlotWiden)(
        pGeometry, strokeWidth, pStrokeStyle, pWorldTransform, flatteningTolerance, pGeometrySink));
}
