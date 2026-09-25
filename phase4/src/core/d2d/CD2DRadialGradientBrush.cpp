// CD2DRadialGradientBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Layout (retail, afxrendertarget.h: class CD2DRadialGradientBrush : public
// CD2DGradientBrush).  include/openmfc does not declare this class, so the
// thunks take void* pThis and the offsets used here are pinned below.  Every
// offset was read from the retail bodies disassembled from mfc140u.dll:
//   +0x18  CD2DBrush::m_pBrush (ID2D1Brush*) -- written by Attach (RVA 0xd3bd0)
//          and Create (RVA 0xd3af0), nulled by Detach (RVA 0xd3be0), released
//          by CD2DBrush::Destroy (RVA 0xd2c80).
//   +0x60  m_RadialGradientBrushProperties (D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES,
//          24 bytes): center +0x60, gradientOriginOffset +0x68, radiusX +0x70,
//          radiusY +0x74 -- copied in by the ctor (RVA 0xd3a00), read by
//          GetCenter (0xd3cc0) / GetGradientOriginOffset (0xd3d10) /
//          GetRadiusX (0xd3d60) / GetRadiusY (0xd3d80).
//   +0x78  m_pRadialGradientBrush (ID2D1RadialGradientBrush*).
//   sizeof == 0x80 (last member is the pointer at +0x78).
// All RVAs above are function entries in mfc140u.dll.  The four setters are
// not in the mfc140u symbol map; the export map resolves them only in the ANSI
// twin mfc140.dll (0xd40d0/0xd4100/0xd4130/0xd4160).  Their mfc140u bodies were
// located by disassembling the unmapped gap before GetCenter (the same 0x4d0
// displacement as GetCenter: mfc140 0xd4190 vs mfc140u 0xd3cc0) and are
// byte-identical: SetCenter 0xd3c00, SetGradientOriginOffset 0xd3c30,
// SetRadiusX 0xd3c60, SetRadiusY 0xd3c90 (mfc140u, identified by body match,
// not by the symbol map).  The class's 0x80 size also matches the
// CRuntimeClass descriptor in core/d2d/RuntimeClasses.cpp (D2D_DESC(..., 128, ...)).
//
// ID2D1RadialGradientBrush vtable (d2d1.h): IUnknown 0-2, ID2D1Resource 3,
// ID2D1Brush 4-7, then SetCenter 8, SetGradientOriginOffset 9, SetRadiusX 10,
// SetRadiusY 11, GetCenter 12, GetGradientOriginOffset 13, GetRadiusX 14,
// GetRadiusY 15.  The retail call sites use byte offsets 0x40..0x78, i.e.
// exactly those slots.  Retail makes every such call through the CFG
// dispatch pointer (0x1802c7b30 in mfc140u = its load-config
// GuardCFDispatchFunctionPointer); here they are plain indirect calls.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

// CD2DGradientBrush thunks (defined in core/d2d/CD2DGradientBrush.cpp; the
// parameter lists below follow the mangled names).  Destroy is also declared
// in ManualSmallStubImplementationsSupport.h.
extern "C" void* MS_ABI impl___0CD2DGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IW4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pParentTarget, const void* gradientStops, unsigned int gradientStopsCount,
    int colorInterpolationGamma, int extendMode, void* pBrushProperties, int bAutoDestroy);
extern "C" void MS_ABI impl___1CD2DGradientBrush__UEAA_XZ(void* pThis);

namespace {

struct RadialPoint2F {   // D2D1_POINT_2F / CD2DPointF payload
    float x;
    float y;
};
static_assert(sizeof(RadialPoint2F) == 8, "D2D1_POINT_2F is 8 bytes");

constexpr std::size_t kOffBrush                = 0x18;  // CD2DBrush::m_pBrush
constexpr std::size_t kOffCenter               = 0x60;  // m_RadialGradientBrushProperties.center
constexpr std::size_t kOffGradientOriginOffset = 0x68;  // .gradientOriginOffset
constexpr std::size_t kOffRadiusX              = 0x70;  // .radiusX
constexpr std::size_t kOffRadiusY              = 0x74;  // .radiusY
constexpr std::size_t kOffRadialBrush          = 0x78;  // m_pRadialGradientBrush
constexpr std::size_t kSizeofRadialBrush       = 0x80;
constexpr std::size_t kSizeofRadialProperties  = 0x18;  // D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES
static_assert(kOffCenter + kSizeofRadialProperties == kOffRadialBrush,
              "properties (+0x60, 24 bytes) end exactly at m_pRadialGradientBrush (+0x78)");
static_assert(kOffRadialBrush + sizeof(void*) == kSizeofRadialBrush,
              "m_pRadialGradientBrush is the last member of the 0x80-byte object");

// ID2D1RadialGradientBrush vtable slots (see file header).
constexpr int kSlotSetCenter               = 8;
constexpr int kSlotSetGradientOriginOffset = 9;
constexpr int kSlotSetRadiusX              = 10;
constexpr int kSlotSetRadiusY              = 11;
constexpr int kSlotGetCenter               = 12;
constexpr int kSlotGetGradientOriginOffset = 13;
constexpr int kSlotGetRadiusX              = 14;
constexpr int kSlotGetRadiusY              = 15;

inline unsigned char* Bytes(void* p) { return static_cast<unsigned char*>(p); }
inline const unsigned char* Bytes(const void* p) { return static_cast<const unsigned char*>(p); }

inline void*& RadialBrush(void* pThis) {
    return *reinterpret_cast<void**>(Bytes(pThis) + kOffRadialBrush);
}
inline void* RadialBrush(const void* pThis) {
    return *reinterpret_cast<void* const*>(Bytes(pThis) + kOffRadialBrush);
}
inline void*& BaseBrush(void* pThis) {
    return *reinterpret_cast<void**>(Bytes(pThis) + kOffBrush);
}

template <typename Fn>
inline Fn ComSlot(void* pInterface, int slot) {
    return reinterpret_cast<Fn>((*static_cast<void***>(pInterface))[slot]);
}

// COM setters taking a D2D1_POINT_2F by value: an 8-byte aggregate travels in
// RDX under the x64 MS ABI, which is what retail relies on (it forwards the
// caller's RDX unchanged), so it is modelled as a 64-bit integer.
using PfnSetPoint = void (MS_ABI*)(void* self, unsigned long long point);
using PfnSetFloat = void (MS_ABI*)(void* self, float value);
// COM getters returning D2D1_POINT_2F: a C++ instance method returns a UDT
// through a hidden pointer in RDX and hands that pointer back in RAX.
using PfnGetPoint = RadialPoint2F* (MS_ABI*)(void* self, RadialPoint2F* ret);
using PfnGetFloat = float (MS_ABI*)(void* self);

// Shared shape of SetCenter / SetGradientOriginOffset (mfc140u 0xd3c00 /
// 0xd3c30): store the 8-byte point into the cached properties unconditionally,
// then, only if m_pRadialGradientBrush is non-null, forward it to the COM slot.
inline void SetPointMember(void* pThis, std::size_t off, int slot, unsigned long long point) {
    std::memcpy(Bytes(pThis) + off, &point, sizeof(point));
    void* pBrush = RadialBrush(pThis);
    if (pBrush != nullptr) {
        ComSlot<PfnSetPoint>(pBrush, slot)(pBrush, point);
    }
}

// Shared shape of SetRadiusX / SetRadiusY (mfc140u 0xd3c60 / 0xd3c90).
inline void SetFloatMember(void* pThis, std::size_t off, int slot, float value) {
    std::memcpy(Bytes(pThis) + off, &value, sizeof(value));
    void* pBrush = RadialBrush(pThis);
    if (pBrush != nullptr) {
        ComSlot<PfnSetFloat>(pBrush, slot)(pBrush, value);
    }
}

// Shared shape of GetCenter / GetGradientOriginOffset: if
// m_pRadialGradientBrush is non-null the value comes from the COM getter,
// otherwise from the cached properties; either way the 8 bytes are copied to
// the caller's return slot, whose address is returned.
inline void* GetPointMember(const void* pThis, void* pRet, std::size_t off, int slot) {
    RadialPoint2F tmp;
    const RadialPoint2F* pSrc;
    void* pBrush = RadialBrush(pThis);
    if (pBrush != nullptr) {
        pSrc = ComSlot<PfnGetPoint>(pBrush, slot)(pBrush, &tmp);
    } else {
        std::memcpy(&tmp, Bytes(pThis) + off, sizeof(tmp));
        pSrc = &tmp;
    }
    std::memcpy(pRet, pSrc, sizeof(RadialPoint2F));
    return pRet;
}

// Shared shape of GetRadiusX / GetRadiusY: tail-call the COM getter when the
// interface is present, else return the cached float.
inline float GetFloatMember(const void* pThis, std::size_t off, int slot) {
    void* pBrush = RadialBrush(pThis);
    if (pBrush != nullptr) {
        return ComSlot<PfnGetFloat>(pBrush, slot)(pBrush);
    }
    float value;
    std::memcpy(&value, Bytes(pThis) + off, sizeof(value));
    return value;
}

} // namespace

// Symbol: ?Attach@CD2DRadialGradientBrush@@QEAAXPEAUID2D1RadialGradientBrush@@@Z
// Retail (RVA 0xd3bd0, mfc140u): m_pRadialGradientBrush = pResource;
// m_pBrush = pResource.  No null check, no AddRef.
extern "C" void MS_ABI impl__Attach_CD2DRadialGradientBrush__QEAAXPEAUID2D1RadialGradientBrush___Z(
    void* pThis, void* pBrush) {
    RadialBrush(pThis) = pBrush;
    BaseBrush(pThis) = pBrush;
}
// Symbol: ?Create@CD2DRadialGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
// NOT the retail body.  Retail (RVA 0xd3af0, mfc140u) returns E_FAIL unless
// pRenderTarget and its ID2D1RenderTarget* (+0x8) are non-null, m_pBrush (+0x18)
// is null and the gradient-stop count (+0x38) is non-zero; then it calls
// ID2D1RenderTarget::CreateGradientStopCollection (slot 9) into +0x58 and
// CreateRadialGradientBrush (slot 11) with &m_RadialGradientBrushProperties
// (+0x60) and m_pBrushProperties (+0x20), storing the result in both +0x78 and
// +0x18.  OpenMFC's CD2DGradientBrush ctor does not populate the stop array at
// +0x30/+0x38, so that path cannot be transcribed yet; this legacy side-table
// body is kept unchanged (it never touches +0x78, so m_pRadialGradientBrush
// stays null and the getters below serve the cached properties).
extern "C" long MS_ABI impl__Create_CD2DRadialGradientBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}
// Symbol: ??0CD2DRadialGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
// Retail (RVA 0xd3a00, mfc140u): call the CD2DGradientBrush ctor (0xd3520) with
// (pParentTarget, gradientStops, gradientStopsCount, colorInterpolationGamma,
// extendMode, pBrushProperties, bAutoDestroy); m_pRadialGradientBrush = NULL;
// store the class vftable (0x1802fe5d8 in mfc140u); copy the 24-byte
// D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES to +0x60 (movups 16 + movsd 8).  The
// struct is passed by value, which MSVC x64 lowers to a pointer to a caller-
// made copy in the first stack argument slot (0x28(%rsp) on entry); return this.
// Deviations: the vftable store is omitted -- OpenMFC defines no
// ??_7CD2DRadialGradientBrush@@6B@ anywhere in phase4/src; the null-pThis early
// return is OpenMFC's (retail has none); the legacy side-table registration is
// kept for Create/~dtor.
extern "C" void* MS_ABI impl___0CD2DRadialGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, unsigned int stopCount, const void* properties,
    int gammaMode, int extendMode, void* pBrushProps, int bAutoDestroy) {
    if (pThis == nullptr) {
        return nullptr;
    }
    impl___0CD2DGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IW4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
        pThis, pRenderTarget, pStops, stopCount,
        gammaMode, extendMode, pBrushProps, bAutoDestroy);
    RadialBrush(pThis) = nullptr;
    std::memcpy(Bytes(pThis) + kOffCenter, properties, kSizeofRadialProperties);

    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return pThis;
}
// Symbol: ??1CD2DRadialGradientBrush@@UEAA@XZ
// Retail (RVA 0xd3ac0, mfc140u): store the class vftable (0x1802fe5d8 in
// mfc140u); call CD2DGradientBrush::Destroy (0xd36a0) directly (non-virtually);
// m_pRadialGradientBrush = NULL; tail-jump to ~CD2DGradientBrush (0xd3670).
// Deviations: no vftable store (see ctor); a null-pThis early return that
// retail does not have; the legacy side-table entry is also erased.
extern "C" void MS_ABI impl___1CD2DRadialGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    impl__Destroy_CD2DGradientBrush__MEAAXXZ(pThis);
    RadialBrush(pThis) = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
        g_cd2dRadialGradientBrushStates.erase(pThis);
    }
    impl___1CD2DGradientBrush__UEAA_XZ(pThis);
}
// Symbol: ?Destroy@CD2DRadialGradientBrush@@UEAAXXZ
// Retail (RVA 0xd3bb0, mfc140u): call CD2DGradientBrush::Destroy (0xd36a0)
// directly, then m_pRadialGradientBrush = NULL.  The interface itself is not
// released here: Attach/Create alias it into CD2DBrush::m_pBrush (+0x18), and
// retail's CD2DBrush::Destroy (0xd2c80, reached from 0xd36a0) releases that.
// (OpenMFC's CD2DGradientBrush::Destroy is currently a side-table erase that
// does not release +0x18; that gap belongs to the base-class file.)
extern "C" void MS_ABI impl__Destroy_CD2DRadialGradientBrush__UEAAXXZ(void* pThis) {
    impl__Destroy_CD2DGradientBrush__MEAAXXZ(pThis);
    RadialBrush(pThis) = nullptr;
}

// Symbol: ?Detach@CD2DRadialGradientBrush@@QEAAPEAUID2D1RadialGradientBrush@@XZ
// Retail (RVA 0xd3be0, mfc140u): return the old m_pRadialGradientBrush after
// nulling both it and m_pBrush (+0x18).  No Release.
extern "C" void* MS_ABI impl__Detach_CD2DRadialGradientBrush__QEAAPEAUID2D1RadialGradientBrush__XZ(void* pThis) {
    void* pBrush = RadialBrush(pThis);
    RadialBrush(pThis) = nullptr;
    BaseBrush(pThis) = nullptr;
    return pBrush;
}

// Symbol: ?GetCenter@CD2DRadialGradientBrush@@QEBA?AVCD2DPointF@@XZ
// Retail (RVA 0xd3cc0, mfc140u): instance method returning CD2DPointF, so the
// return slot arrives in RDX after `this`.  Uses ID2D1RadialGradientBrush::
// GetCenter (slot 12) when m_pRadialGradientBrush is set, else the cached
// center at +0x60.
extern "C" void* MS_ABI impl__GetCenter_CD2DRadialGradientBrush__QEBA_AVCD2DPointF__XZ(
    const void* pThis, void* pRet) {
    return GetPointMember(pThis, pRet, kOffCenter, kSlotGetCenter);
}

// Symbol: ?GetGradientOriginOffset@CD2DRadialGradientBrush@@QEBA?AVCD2DPointF@@XZ
// Retail (RVA 0xd3d10, mfc140u): as GetCenter, with COM slot 13 and the cached
// gradientOriginOffset at +0x68.
extern "C" void* MS_ABI impl__GetGradientOriginOffset_CD2DRadialGradientBrush__QEBA_AVCD2DPointF__XZ(
    const void* pThis, void* pRet) {
    return GetPointMember(pThis, pRet, kOffGradientOriginOffset, kSlotGetGradientOriginOffset);
}

// Symbol: ?GetRadiusX@CD2DRadialGradientBrush@@QEBAMXZ
// Retail (RVA 0xd3d60, mfc140u): tail-call ID2D1RadialGradientBrush::GetRadiusX
// (slot 14) when m_pRadialGradientBrush is set, else return the float at +0x70.
extern "C" float MS_ABI impl__GetRadiusX_CD2DRadialGradientBrush__QEBAMXZ(const void* pThis) {
    return GetFloatMember(pThis, kOffRadiusX, kSlotGetRadiusX);
}

// Symbol: ?GetRadiusY@CD2DRadialGradientBrush@@QEBAMXZ
// Retail (RVA 0xd3d80, mfc140u): as GetRadiusX, with slot 15 and +0x74.
extern "C" float MS_ABI impl__GetRadiusY_CD2DRadialGradientBrush__QEBAMXZ(const void* pThis) {
    return GetFloatMember(pThis, kOffRadiusY, kSlotGetRadiusY);
}

// Symbol: ?SetCenter@CD2DRadialGradientBrush@@QEAAXVCD2DPointF@@@Z
// Retail (RVA 0xd3c00 mfc140u by body match; mapped export RVA 0xd40d0 in
// mfc140.dll -- see file header): store the point
// (RDX) to +0x60, then if m_pRadialGradientBrush is non-null call
// ID2D1RadialGradientBrush::SetCenter (slot 8) with the same RDX.
extern "C" void MS_ABI impl__SetCenter_CD2DRadialGradientBrush__QEAAXVCD2DPointF___Z(
    void* pThis, unsigned long long point) {
    SetPointMember(pThis, kOffCenter, kSlotSetCenter, point);
}

// Symbol: ?SetGradientOriginOffset@CD2DRadialGradientBrush@@QEAAXVCD2DPointF@@@Z
// Retail (RVA 0xd3c30 mfc140u by body match; 0xd4100 in mfc140.dll): as SetCenter, with
// +0x68 and slot 9.
extern "C" void MS_ABI impl__SetGradientOriginOffset_CD2DRadialGradientBrush__QEAAXVCD2DPointF___Z(
    void* pThis, unsigned long long gradientOriginOffset) {
    SetPointMember(pThis, kOffGradientOriginOffset, kSlotSetGradientOriginOffset, gradientOriginOffset);
}

// Symbol: ?SetRadiusX@CD2DRadialGradientBrush@@QEAAXM@Z
// Retail (RVA 0xd3c60 mfc140u by body match; 0xd4130 in mfc140.dll): store XMM1 to
// +0x70, then if m_pRadialGradientBrush is non-null call
// ID2D1RadialGradientBrush::SetRadiusX (slot 10) with the same XMM1.
extern "C" void MS_ABI impl__SetRadiusX_CD2DRadialGradientBrush__QEAAXM_Z(void* pThis, float radiusX) {
    SetFloatMember(pThis, kOffRadiusX, kSlotSetRadiusX, radiusX);
}

// Symbol: ?SetRadiusY@CD2DRadialGradientBrush@@QEAAXM@Z
// Retail (RVA 0xd3c90 mfc140u by body match; 0xd4160 in mfc140.dll): as SetRadiusX,
// with +0x74 and slot 11.
extern "C" void MS_ABI impl__SetRadiusY_CD2DRadialGradientBrush__QEAAXM_Z(void* pThis, float radiusY) {
    SetFloatMember(pThis, kOffRadiusY, kSlotSetRadiusY, radiusY);
}
