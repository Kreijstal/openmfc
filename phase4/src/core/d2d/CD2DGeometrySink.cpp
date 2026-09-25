// CD2DGeometrySink — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CD2DGeometrySink (retail afxrendertarget.h) is the ID2D1GeometrySink wrapper: a
// root class (no base) with a virtual destructor and one data member, m_pSink.  Every
// body below is transcribed from the retail code (the ctor/dtor vftable stores
// excepted -- see those two functions).  Function bodies are byte-identical in
// mfc140.dll; every RVA quoted here is the mfc140u.dll one unless labelled "(mfc140)".
//
// All thirteen methods, the ctor/dtor and CD2DPathGeometry::Open ARE exported by the
// retail mfc140u.dll on this host.  SetFillMode (ordinal 13192 -> RVA 0xd6fa0),
// SetSegmentFlags (13576 -> 0xd6fc0) and CD2DPathGeometry::Open (11637 -> 0xd6c40) are
// merely missing from mfc140u_rva_symbols.json, which holds only 8192 entries.  (pefile
// also under-reads this ordinal-only image -- 9307 filled slots ending at ordinal 9566 --
// whereas reading the export address table directly gives 14103 of 14109 filled, these
// three included; how the json was built was not checked.)  `disas.py --u` therefore
// says NOT FOUND for them; `disas.py --u --at 0x<rva>` reads them.  Cross-checks, all
// identical to the mfc140u bodies: mfc140.dll RVA 0xd7b60 / 0xd7b80 / 0xd7800 (mfc140,
// via mfc140_rva_symbols.json), and the static library atlmfc/lib/x64/uafxcw.lib member
// afxrendertarget.obj (ctor, dtor, Close, BeginFigure, SetFillMode, SetSegmentFlags
// compared instruction for instruction).
//
// Layout: OpenMFC's own headers do not declare this class, so every object is laid out
// by a client compiled against the retail header -- m_pSink is read INLINE there by
// IsValid(), Get() and operator ID2D1GeometrySink*(), so it must live where retail puts
// it (+0x8), not in a side table.

#include <cstddef>
#include <cstring>
#include <windows.h>
#include <d2d1.h>

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

// CD2DPathGeometry::Open, defined in phase4/src/core/d2d/CD2DPathGeometry.cpp.  The
// declaration here follows the mangled name (?Open@CD2DPathGeometry@@QEAAPEAUID2D1GeometrySink@@XZ:
// public, non-const member, no arguments besides `this`, returns ID2D1GeometrySink*).
// At the time of writing the definition in that file still carries an auto-generated
// placeholder parameter list `()` and returns nullptr; calling it with `this` in RCX is
// harmless under the MS x64 ABI, and the sink then stays NULL, which every method below
// treats as "not valid".
extern "C" ID2D1GeometrySink* MS_ABI impl__Open_CD2DPathGeometry__QEAAPEAUID2D1GeometrySink__XZ(void* pPathGeometry);

namespace {

// ---------------------------------------------------------------------------
// Pinned retail layout.
//   ctor, RVA 0xd6d60 (mfc140u): stores the vftable at +0x0, then
//     `mov %rax,0x8(%rbx)` -- the result of CD2DPathGeometry::Open -> m_pSink at +0x8.
//   Every method loads `mov 0x8(%rcx),%rcx` as its first instruction.
//   sizeof == 16: vfptr + one pointer; the class has no base and no other members in
//     the retail afxrendertarget.h (it declares only `ID2D1GeometrySink* m_pSink;`).
struct CD2DGeometrySinkShadow {
    void*              vfptr;    // +0x00
    ID2D1GeometrySink* m_pSink;  // +0x08
};
static_assert(offsetof(CD2DGeometrySinkShadow, m_pSink) == 0x08, "CD2DGeometrySink::m_pSink");
static_assert(sizeof(CD2DGeometrySinkShadow) == 16, "sizeof(CD2DGeometrySink)");

inline CD2DGeometrySinkShadow* Self(void* pThis) {
    return static_cast<CD2DGeometrySinkShadow*>(pThis);
}

// CArray<TYPE, ARG_TYPE> as the retail afxtempl.h lays it out: CObject vfptr, then
// TYPE* m_pData, INT_PTR m_nSize, ...  The three array-forwarding bodies below load
// `mov 0x8(%rdx),%rdx` (m_pData) and `mov 0x10(%rdx),%r8d` (low 32 bits of m_nSize)
// from the referenced array and pass them straight to the sink as (pointer, UINT32 count).
struct CArrayShadow {
    void*    vfptr;      // +0x00 (CObject)
    void*    m_pData;    // +0x08
    INT_PTR  m_nSize;    // +0x10
};
static_assert(offsetof(CArrayShadow, m_pData) == 0x08, "CArray::m_pData");
static_assert(offsetof(CArrayShadow, m_nSize) == 0x10, "CArray::m_nSize");

// ---------------------------------------------------------------------------
// ID2D1GeometrySink vtable dispatch.  Slot numbers follow d2d1.h declaration order
// (IUnknown 0-2, ID2D1SimplifiedGeometrySink 3-9, ID2D1GeometrySink 10-14), and each
// one is confirmed by the byte offset the retail body loads from the interface vtable
// (`mov 0xNN(%rax),%rax` before the CFG dispatch `call *0x1802c7b30` (mfc140u VA), which the
// uafxcw.lib object's relocation names as __guard_dispatch_icall_fptr, i.e. an indirect
// call through %rax).
enum SinkSlot : std::size_t {
    kSlotRelease             = 2,   // 0x10  ~CD2DGeometrySink    (RVA 0xd6dd0)
    kSlotSetFillMode         = 3,   // 0x18  SetFillMode          (RVA 0xd6fa0)
    kSlotSetSegmentFlags     = 4,   // 0x20  SetSegmentFlags      (RVA 0xd6fc0)
    kSlotBeginFigure         = 5,   // 0x28  BeginFigure          (RVA 0xd6e20)
    kSlotAddLines            = 6,   // 0x30  AddLines             (RVA 0xd6f40)
    kSlotAddBeziers          = 7,   // 0x38  AddBeziers           (RVA 0xd6ea0)
    kSlotEndFigure           = 8,   // 0x40  EndFigure            (RVA 0xd6e40)
    kSlotClose               = 9,   // 0x48  Close (RVA 0xd6f70), ~CD2DGeometrySink
    kSlotAddLine             = 10,  // 0x50  AddLine              (RVA 0xd6f20)
    kSlotAddBezier           = 11,  // 0x58  AddBezier            (RVA 0xd6e80)
    kSlotAddQuadraticBezier  = 12,  // 0x60  AddQuadraticBezier   (RVA 0xd6ed0)
    kSlotAddQuadraticBeziers = 13,  // 0x68  AddQuadraticBeziers  (RVA 0xd6ef0)
    kSlotAddArc              = 14,  // 0x70  AddArc               (RVA 0xd6e60)
};

template <typename Fn>
inline Fn Slot(ID2D1GeometrySink* pSink, std::size_t index) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void* const* const*>(pSink))[index]);
}

typedef ULONG   (MS_ABI* PfnRelease)(ID2D1GeometrySink*);
typedef void    (MS_ABI* PfnSetFillMode)(ID2D1GeometrySink*, D2D1_FILL_MODE);
typedef void    (MS_ABI* PfnSetSegmentFlags)(ID2D1GeometrySink*, D2D1_PATH_SEGMENT);
typedef void    (MS_ABI* PfnBeginFigure)(ID2D1GeometrySink*, D2D1_POINT_2F, D2D1_FIGURE_BEGIN);
typedef void    (MS_ABI* PfnAddLines)(ID2D1GeometrySink*, const D2D1_POINT_2F*, UINT32);
typedef void    (MS_ABI* PfnAddBeziers)(ID2D1GeometrySink*, const D2D1_BEZIER_SEGMENT*, UINT32);
typedef void    (MS_ABI* PfnEndFigure)(ID2D1GeometrySink*, D2D1_FIGURE_END);
typedef HRESULT (MS_ABI* PfnClose)(ID2D1GeometrySink*);
typedef void    (MS_ABI* PfnAddLine)(ID2D1GeometrySink*, D2D1_POINT_2F);
typedef void    (MS_ABI* PfnAddBezier)(ID2D1GeometrySink*, const D2D1_BEZIER_SEGMENT*);
typedef void    (MS_ABI* PfnAddQuadraticBezier)(ID2D1GeometrySink*, const D2D1_QUADRATIC_BEZIER_SEGMENT*);
typedef void    (MS_ABI* PfnAddQuadraticBeziers)(ID2D1GeometrySink*, const D2D1_QUADRATIC_BEZIER_SEGMENT*, UINT32);
typedef void    (MS_ABI* PfnAddArc)(ID2D1GeometrySink*, const D2D1_ARC_SEGMENT*);

// CD2DPointF is passed BY VALUE (8 bytes: two floats, derived from D2D1_POINT_2F with no
// members of its own) in an integer register under the MS x64 ABI.  Retail BeginFigure
// and AddLine forward %rdx untouched as the D2D1_POINT_2F argument.  It is modelled here
// as a 64-bit integer and re-typed, as core/d2d/CD2DGeometry.cpp does.
inline D2D1_POINT_2F PointFromBits(unsigned long long bits) {
    D2D1_POINT_2F pt;
    static_assert(sizeof(pt) == sizeof(bits), "D2D1_POINT_2F is 8 bytes");
    std::memcpy(&pt, &bits, sizeof(pt));
    return pt;
}

inline const CArrayShadow* ArrayOf(const void* pArray) {
    return static_cast<const CArrayShadow*>(pArray);
}

} // namespace

// Symbol: ??0CD2DGeometrySink@@QEAA@AEAVCD2DPathGeometry@@@Z
// Retail RVA 0xd6d60 (mfc140u): store the CD2DGeometrySink vftable at +0x0, call
// CD2DPathGeometry::Open on the referenced path geometry (a direct `call 0x1800d6c40`,
// mfc140u VA; RVA 0xd6c40 is the mfc140u export for ordinal 11637,
// ?Open@CD2DPathGeometry@@QEAAPEAUID2D1GeometrySink@@XZ, and the uafxcw.lib object's
// relocation for that call names the same symbol), store the result in m_pSink, return
// this.  Open (0xd6c40) returns NULL when the ID2D1PathGeometry* at path-geometry+0x20 is
// NULL or when its Open (vtable offset 0x88) fails.
// Deviation: the vftable store is not reproduced -- this tree has no CD2DGeometrySink
// vftable to point at (the CD2DGeometry, CD2DBitmap and CD2DPathGeometry ctors here
// omit theirs too).  A client that `delete`s a heap CD2DGeometrySink dispatches through
// +0x0 and is therefore not supported yet.
extern "C" void* MS_ABI impl___0CD2DGeometrySink__QEAA_AEAVCD2DPathGeometry___Z(void* pThis, void* pPathGeometry) {
    Self(pThis)->m_pSink = impl__Open_CD2DPathGeometry__QEAAPEAUID2D1GeometrySink__XZ(pPathGeometry);
    return pThis;
}
// Symbol: ??1CD2DGeometrySink@@UEAA@XZ
// Retail RVA 0xd6dd0 (mfc140u): store the CD2DGeometrySink vftable, then, only if
// m_pSink is non-NULL: call m_pSink->Close() (vtable offset 0x48, slot 9; the HRESULT
// is discarded), reload m_pSink and call its Release (offset 0x10, slot 2), and store
// NULL at +0x8.  When m_pSink is NULL nothing else happens.
// Deviation: the vftable store is omitted (no vftable in this tree, as in the ctor).
extern "C" void MS_ABI impl___1CD2DGeometrySink__UEAA_XZ(void* pThis) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnClose>(pSink, kSlotClose)(pSink);
        pSink = Self(pThis)->m_pSink;
        Slot<PfnRelease>(pSink, kSlotRelease)(pSink);
        Self(pThis)->m_pSink = nullptr;
    }
}
// Symbol: ?AddArc@CD2DGeometrySink@@QEAAXAEBUD2D1_ARC_SEGMENT@@@Z
// Retail RVA 0xd6e60 (mfc140u): if m_pSink is non-NULL, m_pSink->AddArc(&arc) --
// vtable offset 0x70, slot 14; %rdx (the reference) is forwarded untouched.
extern "C" void MS_ABI impl__AddArc_CD2DGeometrySink__QEAAXAEBUD2D1_ARC_SEGMENT___Z(void* pThis, const D2D1_ARC_SEGMENT* pArc) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnAddArc>(pSink, kSlotAddArc)(pSink, pArc);
    }
}

// Symbol: ?AddBezier@CD2DGeometrySink@@QEAAXAEBUD2D1_BEZIER_SEGMENT@@@Z
// Retail RVA 0xd6e80 (mfc140u): if m_pSink is non-NULL, m_pSink->AddBezier(&bezier) --
// vtable offset 0x58, slot 11; %rdx forwarded untouched.
extern "C" void MS_ABI impl__AddBezier_CD2DGeometrySink__QEAAXAEBUD2D1_BEZIER_SEGMENT___Z(void* pThis, const D2D1_BEZIER_SEGMENT* pBezier) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnAddBezier>(pSink, kSlotAddBezier)(pSink, pBezier);
    }
}

// Symbol: ?AddBeziers@CD2DGeometrySink@@QEAAXAEBV?$CArray@UD2D1_BEZIER_SEGMENT@@U1@@@@Z
// Retail RVA 0xd6ea0 (mfc140u): if m_pSink is non-NULL,
// m_pSink->AddBeziers(beziers.m_pData, (UINT32)beziers.m_nSize) -- vtable offset 0x38,
// slot 7.  The array fields are read directly (+0x8, low dword of +0x10); there is no
// empty-array check, so an empty array is forwarded with its (possibly NULL) m_pData
// and a count of 0.
extern "C" void MS_ABI impl__AddBeziers_CD2DGeometrySink__QEAAXAEBV__CArray_UD2D1_BEZIER_SEGMENT__U1____Z(void* pThis, const void* pBeziers) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        const CArrayShadow* pArray = ArrayOf(pBeziers);
        Slot<PfnAddBeziers>(pSink, kSlotAddBeziers)(
            pSink, static_cast<const D2D1_BEZIER_SEGMENT*>(pArray->m_pData), static_cast<UINT32>(pArray->m_nSize));
    }
}

// Symbol: ?AddLine@CD2DGeometrySink@@QEAAXVCD2DPointF@@@Z
// Retail RVA 0xd6f20 (mfc140u): if m_pSink is non-NULL, m_pSink->AddLine(point) --
// vtable offset 0x50, slot 10; the by-value point in %rdx is forwarded untouched.
extern "C" void MS_ABI impl__AddLine_CD2DGeometrySink__QEAAXVCD2DPointF___Z(void* pThis, unsigned long long pointBits) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnAddLine>(pSink, kSlotAddLine)(pSink, PointFromBits(pointBits));
    }
}

// Symbol: ?AddLines@CD2DGeometrySink@@QEAAXAEBV?$CArray@VCD2DPointF@@V1@@@@Z
// Retail RVA 0xd6f40 (mfc140u): if m_pSink is non-NULL,
// m_pSink->AddLines(points.m_pData, (UINT32)points.m_nSize) -- vtable offset 0x30,
// slot 6.  m_pData is a CD2DPointF*, passed as the D2D1_POINT_2F* it derives from; no
// empty-array check (as AddBeziers).
extern "C" void MS_ABI impl__AddLines_CD2DGeometrySink__QEAAXAEBV__CArray_VCD2DPointF__V1____Z(void* pThis, const void* pPoints) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        const CArrayShadow* pArray = ArrayOf(pPoints);
        Slot<PfnAddLines>(pSink, kSlotAddLines)(
            pSink, static_cast<const D2D1_POINT_2F*>(pArray->m_pData), static_cast<UINT32>(pArray->m_nSize));
    }
}

// Symbol: ?AddQuadraticBezier@CD2DGeometrySink@@QEAAXAEBUD2D1_QUADRATIC_BEZIER_SEGMENT@@@Z
// Retail RVA 0xd6ed0 (mfc140u): if m_pSink is non-NULL, m_pSink->AddQuadraticBezier(&bezier)
// -- vtable offset 0x60, slot 12; %rdx forwarded untouched.
extern "C" void MS_ABI impl__AddQuadraticBezier_CD2DGeometrySink__QEAAXAEBUD2D1_QUADRATIC_BEZIER_SEGMENT___Z(void* pThis, const D2D1_QUADRATIC_BEZIER_SEGMENT* pBezier) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnAddQuadraticBezier>(pSink, kSlotAddQuadraticBezier)(pSink, pBezier);
    }
}

// Symbol: ?AddQuadraticBeziers@CD2DGeometrySink@@QEAAXAEBV?$CArray@UD2D1_QUADRATIC_BEZIER_SEGMENT@@U1@@@@Z
// Retail RVA 0xd6ef0 (mfc140u): if m_pSink is non-NULL,
// m_pSink->AddQuadraticBeziers(beziers.m_pData, (UINT32)beziers.m_nSize) -- vtable offset
// 0x68, slot 13; no empty-array check (as AddBeziers).
extern "C" void MS_ABI impl__AddQuadraticBeziers_CD2DGeometrySink__QEAAXAEBV__CArray_UD2D1_QUADRATIC_BEZIER_SEGMENT__U1____Z(void* pThis, const void* pBeziers) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        const CArrayShadow* pArray = ArrayOf(pBeziers);
        Slot<PfnAddQuadraticBeziers>(pSink, kSlotAddQuadraticBeziers)(
            pSink, static_cast<const D2D1_QUADRATIC_BEZIER_SEGMENT*>(pArray->m_pData),
            static_cast<UINT32>(pArray->m_nSize));
    }
}

// Symbol: ?BeginFigure@CD2DGeometrySink@@QEAAXVCD2DPointF@@W4D2D1_FIGURE_BEGIN@@@Z
// Retail RVA 0xd6e20 (mfc140u): if m_pSink is non-NULL,
// m_pSink->BeginFigure(startPoint, figureBegin) -- vtable offset 0x28, slot 5; the
// by-value point (%rdx) and the enum (%r8d) are forwarded untouched.
extern "C" void MS_ABI impl__BeginFigure_CD2DGeometrySink__QEAAXVCD2DPointF__W4D2D1_FIGURE_BEGIN___Z(void* pThis, unsigned long long startPointBits, D2D1_FIGURE_BEGIN figureBegin) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnBeginFigure>(pSink, kSlotBeginFigure)(pSink, PointFromBits(startPointBits), figureBegin);
    }
}

// Symbol: ?Close@CD2DGeometrySink@@QEAAHXZ
// Retail RVA 0xd6f70 (mfc140u): FALSE if m_pSink is NULL; otherwise
// SUCCEEDED(m_pSink->Close()) (vtable offset 0x48, slot 9; `not %eax; shr $0x1f,%eax`,
// i.e. hr >= 0).  m_pSink is NOT released or cleared here -- that is left to the dtor,
// which calls Close unconditionally (again, if the client already did) before Release.
extern "C" int MS_ABI impl__Close_CD2DGeometrySink__QEAAHXZ(void* pThis) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink == nullptr) return FALSE;
    HRESULT hr = Slot<PfnClose>(pSink, kSlotClose)(pSink);
    return hr >= 0 ? TRUE : FALSE;
}

// Symbol: ?EndFigure@CD2DGeometrySink@@QEAAXW4D2D1_FIGURE_END@@@Z
// Retail RVA 0xd6e40 (mfc140u): if m_pSink is non-NULL, m_pSink->EndFigure(figureEnd) --
// vtable offset 0x40, slot 8; %edx forwarded untouched.
extern "C" void MS_ABI impl__EndFigure_CD2DGeometrySink__QEAAXW4D2D1_FIGURE_END___Z(void* pThis, D2D1_FIGURE_END figureEnd) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnEndFigure>(pSink, kSlotEndFigure)(pSink, figureEnd);
    }
}

// Symbol: ?SetFillMode@CD2DGeometrySink@@QEAAXW4D2D1_FILL_MODE@@@Z
// Retail RVA 0xd6fa0 (mfc140u, ordinal 13192; identical at mfc140 0xd7b60 and in
// uafxcw.lib(afxrendertarget.obj)):
// if m_pSink is non-NULL, m_pSink->SetFillMode(fillMode) -- vtable offset 0x18, slot 3,
// dispatched through __guard_dispatch_icall_fptr; %edx forwarded untouched.
extern "C" void MS_ABI impl__SetFillMode_CD2DGeometrySink__QEAAXW4D2D1_FILL_MODE___Z(void* pThis, D2D1_FILL_MODE fillMode) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnSetFillMode>(pSink, kSlotSetFillMode)(pSink, fillMode);
    }
}

// Symbol: ?SetSegmentFlags@CD2DGeometrySink@@QEAAXW4D2D1_PATH_SEGMENT@@@Z
// Retail RVA 0xd6fc0 (mfc140u, ordinal 13576; identical at mfc140 0xd7b80 and in
// uafxcw.lib(afxrendertarget.obj)):
// if m_pSink is non-NULL, m_pSink->SetSegmentFlags(vertexFlags) -- vtable offset 0x20,
// slot 4, dispatched through __guard_dispatch_icall_fptr; %edx forwarded untouched.
extern "C" void MS_ABI impl__SetSegmentFlags_CD2DGeometrySink__QEAAXW4D2D1_PATH_SEGMENT___Z(void* pThis, D2D1_PATH_SEGMENT vertexFlags) {
    ID2D1GeometrySink* pSink = Self(pThis)->m_pSink;
    if (pSink != nullptr) {
        Slot<PfnSetSegmentFlags>(pSink, kSlotSetSegmentFlags)(pSink, vertexFlags);
    }
}
