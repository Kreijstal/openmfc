// CD2DSolidColorBrush — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Layout (retail, afxrendertarget.h: class CD2DSolidColorBrush : public
// CD2DBrush).  include/openmfc does not declare this class, so the thunks take
// void* pThis and the offsets used here are pinned below.  Every offset was
// read from the retail bodies disassembled from mfc140u.dll:
//   +0x18  CD2DBrush::m_pBrush (ID2D1Brush*) -- zeroed by the CD2DBrush ctor
//          (RVA 0xd2b70), written by Attach (RVA 0xd2f70) and Create (RVA
//          0xd2ee0), nulled by Detach (RVA 0xd2f80), released and nulled by
//          CD2DBrush::Destroy (RVA 0xd2c80).
//   +0x20  CD2DBrush::m_pBrushProperties -- passed by Create as the
//          D2D1_BRUSH_PROPERTIES* argument of CreateSolidColorBrush.
//   +0x28  m_pSolidColorBrush (ID2D1SolidColorBrush*) -- nulled by both ctors
//          (RVA 0xd2d60 / 0xd2e00), Destroy (0xd2f50) and the dtor (0xd2eb0).
//   +0x30  m_colorSolid (D2D1_COLOR_F, 16 bytes: r +0x30, g +0x34, b +0x38,
//          a +0x3c) -- written field by field by the COLORREF ctor (0xd2e00).
//   sizeof == 0x40, which matches D2D_DESC(CD2DSolidColorBrush, 64, ...) in
//   core/d2d/RuntimeClasses.cpp.
// All RVAs above are function entries in mfc140u.dll.  SetColor is not in the
// mfc140u symbol map (mfc140u_rva_symbols.json); the repo's mfc140.dll symbol
// map places it at RVA 0xd3470 in the ANSI twin.  Its mfc140u body was located
// in the unmapped gap between Detach (0xd2f80) and GetColor (0xd2fd0) at RVA
// 0xd2fa0 (mfc140u); it matches the mfc140.dll body instruction for
// instruction, differing only in the RIP-relative displacement of the CFG
// dispatch call (identified by body match, not by the symbol map).
//
// ID2D1SolidColorBrush vtable (d2d1.h): IUnknown 0-2, ID2D1Resource::GetFactory
// 3, ID2D1Brush 4-7 (SetOpacity, SetTransform, GetOpacity, GetTransform), then
// SetColor 8 and GetColor 9.  The retail call sites use byte offsets 0x40 and
// 0x48, i.e. exactly those slots.  Retail makes every such call through the CFG
// dispatch pointer (0x1802c7b30 in mfc140u); here they are plain indirect calls.
//
// OpenMFC-wide gaps that shape the deviations below (all outside this file):
//   * No ??_7CD2DSolidColorBrush@@6B@ vftable is defined anywhere in
//     phase4/src, so the ctors/dtor cannot store it (retail stores the mfc140u
//     vftable at 0x1802fe808).
//   * OpenMFC's CD2DBrush (core/d2d/CD2DBrush.cpp) keeps m_pBrush in the
//     g_cd2dBrushStates side table rather than at +0x18, its ctor does not
//     write +0x18/+0x20, and its Destroy erases the side-table entry without
//     releasing the interface or touching +0x18.  CRenderTarget
//     (core/d2d/CRenderTarget.cpp) reads a brush's interface through the
//     CD2DBrush Attach/Detach thunks, i.e. from that side table.
//     Because afxrendertarget.h's inline CD2DBrush::Get()/operator ID2D1Brush*
//     and IsValid() read +0x18 directly in client code, this file compensates
//     for the +0x18 half of that gap: both ctors zero +0x18 (retail's CD2DBrush
//     ctor does), and Destroy/the dtor Release and null a non-null +0x18 after
//     the CD2DBrush::Destroy thunk (retail's CD2DBrush::Destroy does).  If that
//     thunk is later made to release and null +0x18 itself, the compensation
//     sees NULL and does nothing.  +0x20 is not compensated (nothing reads it).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

// CD2DBrush thunks, defined in core/d2d/CD2DBrush.cpp; the parameter lists
// below match those definitions and the mangled names.
extern "C" void* MS_ABI impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pBrushProperties, int unusedFlags);
extern "C" void MS_ABI impl___1CD2DBrush__MEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Destroy_CD2DBrush__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void* pThis, void* pBrush);
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void* pThis);
extern "C" float MS_ABI impl__GetOpacity_CD2DBrush__QEBAMXZ(const void* pThis);
// Defined there with a D2D_MATRIX_3X2_F* (six floats); extern "C", same symbol.
extern "C" void MS_ABI impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(
    const void* pThis, float* pMatrix);
// CRenderTarget thunks, defined in core/d2d/CRenderTarget.cpp (there typed
// CRenderTarget*; extern "C", so void* here names the same symbol).
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(void* pThis, void* pRenderTarget);
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(void* pThis);

namespace {

struct SolidColorF {   // D2D1_COLOR_F
    float r;
    float g;
    float b;
    float a;
};
static_assert(sizeof(SolidColorF) == 16, "D2D1_COLOR_F is 16 bytes");

constexpr std::size_t kOffBrush            = 0x18;  // CD2DBrush::m_pBrush
constexpr std::size_t kOffSolidColorBrush  = 0x28;  // m_pSolidColorBrush
constexpr std::size_t kOffColorSolid       = 0x30;  // m_colorSolid
constexpr std::size_t kSizeofSolidBrush    = 0x40;
static_assert(kOffSolidColorBrush + sizeof(void*) == kOffColorSolid,
              "m_colorSolid (+0x30) immediately follows m_pSolidColorBrush (+0x28)");
static_assert(kOffColorSolid + sizeof(SolidColorF) == kSizeofSolidBrush,
              "m_colorSolid is the last member of the 0x40-byte object");

// ID2D1SolidColorBrush vtable slots (see file header).
constexpr int kSlotSetColor = 8;
constexpr int kSlotGetColor = 9;

inline unsigned char* Bytes(void* p) { return static_cast<unsigned char*>(p); }
inline const unsigned char* Bytes(const void* p) { return static_cast<const unsigned char*>(p); }

inline void*& SolidBrush(void* pThis) {
    return *reinterpret_cast<void**>(Bytes(pThis) + kOffSolidColorBrush);
}
inline void* SolidBrush(const void* pThis) {
    return *reinterpret_cast<void* const*>(Bytes(pThis) + kOffSolidColorBrush);
}
inline void*& BaseBrush(void* pThis) {
    return *reinterpret_cast<void**>(Bytes(pThis) + kOffBrush);
}

template <typename Fn>
inline Fn ComSlot(void* pInterface, int slot) {
    return reinterpret_cast<Fn>((*static_cast<void***>(pInterface))[slot]);
}

// The +0x18 half of retail CD2DBrush::Destroy (RVA 0xd2c80, mfc140u):
// if (m_pBrush) { m_pBrush->Release() (vtable byte 0x10, slot 2); m_pBrush = NULL; }
// OpenMFC's CD2DBrush::Destroy thunk does not do this (see file header).
using PfnRelease = unsigned long (MS_ABI*)(void* self);
inline void ReleaseBaseBrush(void* pThis) {
    void*& pBrush = BaseBrush(pThis);
    if (pBrush != nullptr) {
        ComSlot<PfnRelease>(pBrush, 2)(pBrush);
        pBrush = nullptr;
    }
}

// D2D1_BRUSH_PROPERTIES: FLOAT opacity; D2D1_MATRIX_3X2_F transform -- 0x1c
// bytes, the size retail's CD2DBrush ctor allocates for its copy.
struct BrushPropertiesF {
    float opacity;
    float transform[6];
};
static_assert(sizeof(BrushPropertiesF) == 0x1c, "D2D1_BRUSH_PROPERTIES is 0x1c bytes");

// ID2D1RenderTarget vtable (d2d1.h): IUnknown 0-2, GetFactory 3, CreateBitmap 4,
// CreateBitmapFromWicBitmap 5, CreateSharedBitmap 6, CreateBitmapBrush 7,
// CreateSolidColorBrush 8 -- retail Create calls byte offset 0x40, slot 8.
constexpr int kSlotCreateSolidColorBrush = 8;
using PfnCreateSolidColorBrush = long (MS_ABI*)(void* self, const SolidColorF* color,
                                                const BrushPropertiesF* props, void** ppBrush);

// ID2D1SolidColorBrush::SetColor(const D2D1_COLOR_F*).
using PfnSetColor = void (MS_ABI*)(void* self, const SolidColorF* color);
// ID2D1SolidColorBrush::GetColor() returns D2D1_COLOR_F: a C++ instance method
// returns a UDT through a hidden pointer in RDX and hands it back in RAX.
using PfnGetColor = SolidColorF* (MS_ABI*)(void* self, SolidColorF* ret);

} // namespace

// Symbol: ??0CD2DSolidColorBrush@@QEAA@PEAVCRenderTarget@@KHPEAVCD2DBrushProperties@@H@Z
// Retail (RVA 0xd2e00, mfc140u): call the CD2DBrush ctor (0xd2b70) with
// (pParentTarget, pBrushProperties, bAutoDestroy) -- pBrushProperties and
// bAutoDestroy are the 5th/6th arguments, read from the stack; store the class
// vftable (0x1802fe808 in mfc140u); m_pSolidColorBrush = NULL; then fill
// m_colorSolid with each channel converted to float and divided by 255.0f (the
// .rdata constant at mfc140u RVA 0x3500cc):
//   r = (BYTE)color, g = (BYTE)(color >> 8), b = (BYTE)(color >> 16),
//   a = (float)nAlpha -- nAlpha is converted as a full signed int, not masked.
// Return this.
// The previous placeholder parameter list dropped nAlpha (the `H` after `K`)
// and so misread every later argument; it is corrected here to the mangled name.
// Deviations: no vftable store (see file header); a null-pThis early return
// that retail does not have; +0x18 is zeroed here because OpenMFC's CD2DBrush
// ctor does not do it (retail's does -- see file header).
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__KHPEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, unsigned long color, int nAlpha, void* pBrushProps, int bAutoDestroy) {
    if (pThis == nullptr) {
        return nullptr;
    }
    impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(
        pThis, pRenderTarget, pBrushProps, bAutoDestroy);
    BaseBrush(pThis) = nullptr;
    SolidBrush(pThis) = nullptr;
    SolidColorF c;
    c.r = static_cast<float>(static_cast<int>(color & 0xFFu)) / 255.0f;
    c.g = static_cast<float>(static_cast<int>((color >> 8) & 0xFFu)) / 255.0f;
    c.b = static_cast<float>(static_cast<int>((color >> 16) & 0xFFu)) / 255.0f;
    c.a = static_cast<float>(nAlpha) / 255.0f;
    std::memcpy(Bytes(pThis) + kOffColorSolid, &c, sizeof(c));
    return pThis;
}
// Symbol: ??0CD2DSolidColorBrush@@QEAA@PEAVCRenderTarget@@U_D3DCOLORVALUE@@PEAVCD2DBrushProperties@@H@Z
// Retail (RVA 0xd2d60, mfc140u): call the CD2DBrush ctor (0xd2b70) with
// (pParentTarget, pBrushProperties, bAutoDestroy); store the class vftable
// (0x1802fe808 in mfc140u); copy the 16-byte D2D1_COLOR_F to m_colorSolid
// (+0x30); m_pSolidColorBrush = NULL; return this.  The color is passed by
// value, which MSVC x64 lowers to a pointer to a caller-made copy (R8).
// Deviations: no vftable store (see file header); a null-pThis early return
// that retail does not have; +0x18 is zeroed here because OpenMFC's CD2DBrush
// ctor does not do it (retail's does -- see file header).
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__U_D3DCOLORVALUE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, const void* color, void* pBrushProps, int bAutoDestroy) {
    if (pThis == nullptr) {
        return nullptr;
    }
    impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(
        pThis, pRenderTarget, pBrushProps, bAutoDestroy);
    BaseBrush(pThis) = nullptr;
    std::memcpy(Bytes(pThis) + kOffColorSolid, color, sizeof(SolidColorF));
    SolidBrush(pThis) = nullptr;
    return pThis;
}
// Symbol: ??1CD2DSolidColorBrush@@UEAA@XZ
// Retail (RVA 0xd2eb0, mfc140u): store the class vftable (0x1802fe808 in
// mfc140u); call CD2DBrush::Destroy (0xd2c80) directly (non-virtually);
// m_pSolidColorBrush = NULL; tail-jump to ~CD2DBrush (0xd2c50), which calls
// CD2DBrush::Destroy once more.
// Deviations: no vftable store (see file header); a null-pThis early return
// that retail does not have; the +0x18 release that retail's CD2DBrush::Destroy
// performs is done here by ReleaseBaseBrush (see file header).
extern "C" void MS_ABI impl___1CD2DSolidColorBrush__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    impl__Destroy_CD2DBrush__UEAAXXZ(pThis);
    ReleaseBaseBrush(pThis);
    SolidBrush(pThis) = nullptr;
    impl___1CD2DBrush__MEAA_XZ(pThis);
}
// Symbol: ?Attach@CD2DSolidColorBrush@@QEAAXPEAUID2D1SolidColorBrush@@@Z
// Retail (RVA 0xd2f70, mfc140u): m_pSolidColorBrush (+0x28) = pResource;
// m_pBrush (+0x18) = pResource.  No null check, no AddRef.
// Deviation: the same pointer is also handed to the CD2DBrush Attach thunk,
// because OpenMFC's CD2DBrush keeps m_pBrush in its side table and
// CRenderTarget reads a brush's interface from there (see file header).
extern "C" void MS_ABI impl__Attach_CD2DSolidColorBrush__QEAAXPEAUID2D1SolidColorBrush___Z(
    void* pThis, void* pResource) {
    SolidBrush(pThis) = pResource;
    BaseBrush(pThis) = pResource;
    impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(pThis, pResource);
}

// Symbol: ?Create@CD2DSolidColorBrush@@UEAAJPEAVCRenderTarget@@@Z
// Retail (RVA 0xd2ee0, mfc140u), in order:
//   if (pRenderTarget == NULL || pRenderTarget->m_pRenderTarget (+0x8) == NULL
//       || m_pBrush (+0x18) != NULL || m_pSolidColorBrush (+0x28) != NULL)
//       return E_FAIL (0x80004005);
//   pNew = NULL;
//   hr = m_pRenderTarget->CreateSolidColorBrush(&m_colorSolid (+0x30),
//            m_pBrushProperties (+0x20), &pNew);   // vtable byte 0x40, slot 8
//   if (SUCCEEDED(hr)) { m_pSolidColorBrush = pNew; m_pBrush = pNew; }
//   return hr;
// (Retail's m_pBrushProperties is NULL when the ctor got NULL, else a 0x1c-byte
// heap copy of the caller's D2D1_BRUSH_PROPERTIES -- CD2DBrush ctor, RVA
// 0xd2b70 mfc140u.)
// Deviations, each forced by OpenMFC state living outside the object (see the
// file header):
//   * The ID2D1RenderTarget* is read from OpenMFC's CRenderTarget side table via
//     the CRenderTarget Detach/Attach thunks (Detach returns it and nulls the
//     slot; Attach with the same pointer puts it back and releases nothing
//     because the slot is then NULL).  This is the same non-atomic
//     read-by-round-trip core/d2d/CRenderTarget.cpp uses for brushes.  (That
//     Detach inserts an empty side-table entry for a target it has never seen;
//     the NULL it returns then takes the E_FAIL path, as retail's +0x8 test.)
//   * "m_pBrush != NULL" is tested on +0x18 (zeroed by this file's ctors) AND
//     on the CD2DBrush side-table entry, because a client can also attach an
//     interface through the base CD2DBrush::Attach thunk, which writes only the
//     side table.  The side-table entry is read by the same round trip through
//     the CD2DBrush Detach/Attach thunks.
//   * m_pBrushProperties (+0x20) is never written by OpenMFC's CD2DBrush
//     ctor; the ctor instead copies opacity + transform from pBrushProperties
//     into the side table (defaults 1.0 / identity when NULL, or when there is
//     no entry at all).  A D2D1_BRUSH_PROPERTIES is rebuilt from the CD2DBrush
//     GetOpacity/GetTransform thunks and always passed, never NULL.  For a NULL
//     ctor argument that equals D2D's own defaults for a NULL pointer.  It
//     differs from retail whenever CD2DBrush::SetOpacity/SetTransform have
//     written the side table since construction (or since the last Destroy,
//     which erases the entry): OpenMFC's write the side table, while retail's
//     never touch m_pBrushProperties -- they forward to m_pBrush (vtable bytes
//     0x20 / 0x28) or are no-ops while it is NULL (mfc140.dll RVAs 0xd31a0 /
//     0xd31e0).  After a retail Destroy m_pBrushProperties is freed and NULL
//     (0xd2c80), which the erased side-table entry's 1.0/identity defaults match.
//   * On success pNew is also handed to the CD2DBrush Attach thunk, as Attach in
//     this file does, so CRenderTarget's draw calls can find it.
//   * Retail makes the COM call through the CFG dispatch pointer; here it is a
//     plain indirect call.
extern "C" long MS_ABI impl__Create_CD2DSolidColorBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pRenderTarget == nullptr) {
        return static_cast<long>(E_FAIL);
    }
    void* pD2DTarget = impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(pRenderTarget);
    if (pD2DTarget == nullptr) {
        return static_cast<long>(E_FAIL);
    }
    impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(pRenderTarget, pD2DTarget);

    // m_pBrush test, via the CD2DBrush side table: Detach returns the entry's
    // interface and nulls it; a non-null one is put straight back.
    void* pBaseBrush = impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(pThis);
    if (pBaseBrush != nullptr) {
        impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(pThis, pBaseBrush);
        return static_cast<long>(E_FAIL);
    }
    if (BaseBrush(pThis) != nullptr || SolidBrush(pThis) != nullptr) {
        return static_cast<long>(E_FAIL);
    }

    BrushPropertiesF props;
    props.opacity = impl__GetOpacity_CD2DBrush__QEBAMXZ(pThis);
    impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(pThis, props.transform);

    void* pNew = nullptr;
    const long hr = ComSlot<PfnCreateSolidColorBrush>(pD2DTarget, kSlotCreateSolidColorBrush)(
        pD2DTarget, reinterpret_cast<const SolidColorF*>(Bytes(pThis) + kOffColorSolid), &props, &pNew);
    if (hr >= 0) {
        SolidBrush(pThis) = pNew;
        BaseBrush(pThis) = pNew;
        impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(pThis, pNew);
    }
    return hr;
}

// Symbol: ?Destroy@CD2DSolidColorBrush@@UEAAXXZ
// Retail (RVA 0xd2f50, mfc140u): call CD2DBrush::Destroy (0xd2c80) directly,
// then m_pSolidColorBrush = NULL.  Retail does not release +0x28 itself:
// Attach/Create alias it into CD2DBrush::m_pBrush (+0x18), and retail's
// CD2DBrush::Destroy releases and nulls that.  OpenMFC's CD2DBrush::Destroy
// thunk is a side-table erase that does neither, so ReleaseBaseBrush does the
// +0x18 half here (deviation; see file header).
extern "C" void MS_ABI impl__Destroy_CD2DSolidColorBrush__UEAAXXZ(void* pThis) {
    impl__Destroy_CD2DBrush__UEAAXXZ(pThis);
    ReleaseBaseBrush(pThis);
    SolidBrush(pThis) = nullptr;
}

// Symbol: ?Detach@CD2DSolidColorBrush@@QEAAPEAUID2D1SolidColorBrush@@XZ
// Retail (RVA 0xd2f80, mfc140u): return the old m_pSolidColorBrush after
// nulling both it and m_pBrush (+0x18).  No Release.
// Deviation: the CD2DBrush side-table copy made by Attach is cleared too, via
// the CD2DBrush Detach thunk (its return value is not used).
extern "C" void* MS_ABI impl__Detach_CD2DSolidColorBrush__QEAAPEAUID2D1SolidColorBrush__XZ(void* pThis) {
    void* pResource = SolidBrush(pThis);
    SolidBrush(pThis) = nullptr;
    BaseBrush(pThis) = nullptr;
    (void)impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(pThis);
    return pResource;
}

// Symbol: ?GetColor@CD2DSolidColorBrush@@QEBA?AU_D3DCOLORVALUE@@XZ
// Retail (RVA 0xd2fd0, mfc140u): instance method returning D2D1_COLOR_F, so
// the return slot arrives in RDX after `this`.  If m_pSolidColorBrush is
// non-null the color comes from ID2D1SolidColorBrush::GetColor (slot 9) into a
// stack temporary, otherwise the cached m_colorSolid (+0x30) is copied into
// that temporary; either way the 16 bytes are copied to the caller's return
// slot, whose address is returned.  (Retail's /GS cookie check is omitted.)
extern "C" void* MS_ABI impl__GetColor_CD2DSolidColorBrush__QEBA_AU_D3DCOLORVALUE__XZ(
    const void* pThis, void* pRet) {
    SolidColorF tmp;
    const SolidColorF* pSrc;
    void* pBrush = SolidBrush(pThis);
    if (pBrush != nullptr) {
        pSrc = ComSlot<PfnGetColor>(pBrush, kSlotGetColor)(pBrush, &tmp);
    } else {
        std::memcpy(&tmp, Bytes(pThis) + kOffColorSolid, sizeof(tmp));
        pSrc = &tmp;
    }
    std::memcpy(pRet, pSrc, sizeof(SolidColorF));
    return pRet;
}

// Symbol: ?SetColor@CD2DSolidColorBrush@@QEAAXU_D3DCOLORVALUE@@@Z
// Retail (RVA 0xd2fa0 mfc140u by body match; mapped export RVA 0xd3470 in
// mfc140.dll -- see file header): the D2D1_COLOR_F is passed by value, i.e. as
// a pointer to a caller-made copy in RDX.  Copy those 16 bytes to m_colorSolid
// (+0x30) unconditionally, then, only if m_pSolidColorBrush is non-null, call
// ID2D1SolidColorBrush::SetColor (slot 8) with the same RDX pointer.
extern "C" void MS_ABI impl__SetColor_CD2DSolidColorBrush__QEAAXU_D3DCOLORVALUE___Z(
    void* pThis, const void* color) {
    std::memcpy(Bytes(pThis) + kOffColorSolid, color, sizeof(SolidColorF));
    void* pBrush = SolidBrush(pThis);
    if (pBrush != nullptr) {
        ComSlot<PfnSetColor>(pBrush, kSlotSetColor)(pBrush, static_cast<const SolidColorF*>(color));
    }
}
