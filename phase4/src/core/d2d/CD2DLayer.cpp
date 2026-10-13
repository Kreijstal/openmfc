// CD2DLayer: native resource lifecycle and Direct2D ABI forwarding.

#include "detail/D2DResourceSupport.h"
#include <cstddef>
#include <cstring>

#ifndef MS_ABI
#  if defined(__GNUC__) || defined(__clang__)
#    define MS_ABI __attribute__((ms_abi))
#  else
#    define MS_ABI
#  endif
#endif

// Base-class thunks, defined in phase4/src/core/d2d/CD2DResource.cpp (parameter lists
// match those definitions and the mangled names).
extern "C" void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(void* self, void* pParentTarget, int bAutoDestroy);
extern "C" void* MS_ABI impl___1CD2DResource__MEAA_XZ(void* self);
// CRenderTarget thunks, defined in core/d2d/CRenderTarget.cpp (there typed
// CRenderTarget*; extern "C", so void* here names the same symbol).
extern "C" void MS_ABI impl__Attach_CRenderTarget__QEAAXPEAUID2D1RenderTarget___Z(void* pThis, void* pRenderTarget);
extern "C" void* MS_ABI impl__Detach_CRenderTarget__QEAAPEAUID2D1RenderTarget__XZ(void* pThis);

// Forward declaration: the destructor calls Destroy directly, as retail does.
extern "C" void MS_ABI impl__Destroy_CD2DLayer__UEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Pinned retail layout.
//   CD2DResource ctor, RVA 0xd2800 (mfc140u): stores its vftable at +0x0,
//     bAutoDestroy (r8d) at +0x8, pParentTarget (rdx) at +0x10.
//   CD2DLayer ctor, RVA 0xd2940 (mfc140u): calls the CD2DResource ctor, then
//     `movq $0x0,0x18(%rbx)` -- m_pLayer at +0x18 -- and stores its own vftable.
//   sizeof == 32: D2D_DESC(CD2DLayer, 32, ...) in core/d2d/RuntimeClasses.cpp.
// ABI view used by the existing COM forwarding methods.
struct CD2DLayerShadow {
    void* vfptr;             // +0x00
    int   m_bIsAutoDestroy;  // +0x08 (CD2DResource, BOOL)
    void* m_pParentTarget;   // +0x10 (CD2DResource, CRenderTarget*)
    void* m_pLayer;          // +0x18 (CD2DLayer, ID2D1Layer*)
};
static_assert(offsetof(CD2DLayerShadow, m_bIsAutoDestroy) == 0x08, "CD2DResource::m_bIsAutoDestroy");
static_assert(offsetof(CD2DLayerShadow, m_pParentTarget) == 0x10, "CD2DResource::m_pParentTarget");
static_assert(offsetof(CD2DLayerShadow, m_pLayer) == 0x18, "CD2DLayer::m_pLayer");
static_assert(sizeof(CD2DLayerShadow) == 32, "sizeof(CD2DLayer)");

inline CD2DLayerShadow* Self(void* pThis) {
    return static_cast<CD2DLayerShadow*>(pThis);
}
inline void* LayerOf(const void* pThis) {
    return static_cast<const CD2DLayerShadow*>(pThis)->m_pLayer;
}

template <typename Fn>
inline Fn ComSlot(void* pInterface, std::size_t slot) {
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(pInterface))[slot]);
}

// COM vtable slots, from d2d1.h declaration order, each confirmed by the byte offset
// the retail body loads from the interface vtable before its CFG-dispatched call
// (`call *0x1802c7b30` in mfc140u, an indirect call through %rax).
//   IUnknown::Release                    slot 2  (0x10)  Destroy (RVA 0xd2a50)
//   ID2D1Layer::GetSize                  slot 4  (0x20)  GetSize (RVA 0xd2a80)
//     -- IUnknown 0-2, ID2D1Resource::GetFactory 3, ID2D1Layer::GetSize 4.
//   ID2D1RenderTarget::CreateLayer       slot 13 (0x68)  Create  (RVA 0xd29f0)
//     -- IUnknown 0-2, GetFactory 3, CreateBitmap 4, CreateBitmapFromWicBitmap 5,
//        CreateSharedBitmap 6, CreateBitmapBrush 7, CreateSolidColorBrush 8,
//        CreateGradientStopCollection 9, CreateLinearGradientBrush 10,
//        CreateRadialGradientBrush 11, CreateCompatibleRenderTarget 12, CreateLayer 13.
constexpr std::size_t kSlotRelease     = 2;
constexpr std::size_t kSlotGetSize     = 4;
constexpr std::size_t kSlotCreateLayer = 13;

struct SizeF {   // D2D1_SIZE_F
    float width;
    float height;
};
static_assert(sizeof(SizeF) == 8, "D2D1_SIZE_F is 8 bytes");

using PfnRelease = unsigned long (MS_ABI*)(void* self);
// ID2D1RenderTarget::CreateLayer(const D2D1_SIZE_F* size, ID2D1Layer** layer).
using PfnCreateLayer = long (MS_ABI*)(void* self, const SizeF* size, void** ppLayer);
// ID2D1Layer::GetSize() returns D2D1_SIZE_F: an MSVC C++ instance method returns a
// UDT through a hidden pointer in RDX and hands it back in RAX (retail passes
// `lea 0x30(%rsp),%rdx` and reads the result through %rax).
using PfnGetSize = SizeF* (MS_ABI*)(void* self, SizeF* ret);

constexpr long kE_FAIL = static_cast<long>(0x80004005L);

} // namespace

// Symbol: ??0CD2DLayer@@QEAA@PEAVCRenderTarget@@H@Z
// Retail RVA 0xd2940 (mfc140u): call CD2DResource::CD2DResource(pParentTarget,
// bAutoDestroy) (RVA 0xd2800), zero m_pLayer (+0x18), store the CD2DLayer vftable,
// return this.
extern "C" void* MS_ABI impl___0CD2DLayer__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int bAutoDestroy) {
    return pThis ? new(pThis) CD2DLayer(static_cast<CRenderTarget*>(pRenderTarget), bAutoDestroy) : nullptr;
}
// Symbol: ??1CD2DLayer@@UEAA@XZ
// Retail RVA 0xd29c0 (mfc140u): store the CD2DLayer vftable, call CD2DLayer::Destroy
// directly (non-virtual `call 0x1800d2a50`, the Destroy export), then tail-jump to
// CD2DResource::~CD2DResource (RVA 0xd28a0).  Destroy is called unconditionally --
// retail does not consult m_bIsAutoDestroy here.
extern "C" void MS_ABI impl___1CD2DLayer__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CD2DLayer*>(pThis)->CD2DLayer::~CD2DLayer();
}
// Symbol: ?Attach@CD2DLayer@@QEAAXPEAUID2D1Layer@@@Z
// Retail RVA 0xd2ac0 (mfc140u, code shared with CD2DBitmap::Attach):
//   mov %rdx,0x18(%rcx); ret
// A plain store: the previous interface is neither released nor is the new one AddRef'd.
extern "C" void MS_ABI impl__Attach_CD2DLayer__QEAAXPEAUID2D1Layer___Z(void* pThis, void* pResource) {
    Self(pThis)->m_pLayer = pResource;
}

// Symbol: ?Create@CD2DLayer@@UEAAJPEAVCRenderTarget@@@Z
// Retail RVA 0xd29f0 (mfc140u), in order:
//   if (pRenderTarget == NULL || pRenderTarget->m_pRenderTarget (+0x8) == NULL
//       || m_pLayer (+0x18) != NULL)
//       return E_FAIL (0x80004005);
//   pNew = NULL;
//   hr = m_pRenderTarget->CreateLayer(NULL, &pNew);    // vtable byte 0x68, slot 13
//   if (hr >= 0) m_pLayer = pNew;
//   return hr;
// The NULL first argument is `xor %edx,%edx` (no D2D1_SIZE_F: the layer takes the
// render target's size).
extern "C" long MS_ABI impl__Create_CD2DLayer__UEAAJPEAVCRenderTarget___Z(void* pThis, void* pRenderTarget) {
    if (pRenderTarget == nullptr) {
        return kE_FAIL;
    }
    void* pD2DTarget = static_cast<CRenderTarget*>(pRenderTarget)->GetRenderTarget();
    if (pD2DTarget == nullptr) {
        return kE_FAIL;
    }
    if (LayerOf(pThis) != nullptr) {
        return kE_FAIL;
    }
    void* pNew = nullptr;
    const long hr = ComSlot<PfnCreateLayer>(pD2DTarget, kSlotCreateLayer)(pD2DTarget, nullptr, &pNew);
    if (hr >= 0) {
        Self(pThis)->m_pLayer = pNew;
    }
    return hr;
}

// Symbol: ?Destroy@CD2DLayer@@UEAAXXZ
// Retail RVA 0xd2a50 (mfc140u, code shared with CD2DBitmap::Destroy):
//   if (m_pLayer != NULL) { m_pLayer->Release() (vtable byte 0x10, slot 2);
//                           m_pLayer = NULL; }
// Retail makes the Release call through the CFG dispatch pointer; here it is a plain
// indirect call.
extern "C" void MS_ABI impl__Destroy_CD2DLayer__UEAAXXZ(void* pThis) {
    void* pLayer = LayerOf(pThis);
    if (pLayer != nullptr) {
        ComSlot<PfnRelease>(pLayer, kSlotRelease)(pLayer);
        Self(pThis)->m_pLayer = nullptr;
    }
}

// Symbol: ?Detach@CD2DLayer@@QEAAPEAUID2D1Layer@@XZ
// Retail RVA 0xd2ad0 (mfc140u, code shared with CD2DBitmap::Detach):
//   mov 0x18(%rcx),%rax; movq $0x0,0x18(%rcx); ret
// Returns the old m_pLayer and nulls it; no Release.
extern "C" void* MS_ABI impl__Detach_CD2DLayer__QEAAPEAUID2D1Layer__XZ(void* pThis) {
    void* pLayer = LayerOf(pThis);
    Self(pThis)->m_pLayer = nullptr;
    return pLayer;
}

// Symbol: ?GetSize@CD2DLayer@@QEBA?AVCD2DSizeF@@XZ
// Retail RVA 0xd2a80 (mfc140u, code shared with CD2DBitmap::GetSize): an instance
// method returning CD2DSizeF, so the caller's return slot arrives in RDX after `this`
// and is handed back in RAX.
//   if (m_pLayer == NULL) the 8-byte slot is zeroed (`mov %rcx,(%rdx)` with
//     %rcx == m_pLayer == 0), i.e. CD2DSizeF(0.0f, 0.0f);
//   else m_pLayer->GetSize() (vtable byte 0x20, slot 4) into a stack temporary, whose
//     width and height are copied to the return slot.
// Retail makes the COM call through the CFG dispatch pointer; here it is a plain
// indirect call.  (The previous placeholder signature had a single parameter; it is
// corrected here to the MSVC ABI: RCX = this, RDX = return slot.)
extern "C" void* MS_ABI impl__GetSize_CD2DLayer__QEBA_AVCD2DSizeF__XZ(const void* pThis, void* pRet) {
    void* pLayer = LayerOf(pThis);
    if (pLayer == nullptr) {
        std::memset(pRet, 0, sizeof(SizeF));
        return pRet;
    }
    SizeF tmp;
    const SizeF* pSrc = ComSlot<PfnGetSize>(pLayer, kSlotGetSize)(pLayer, &tmp);
    std::memcpy(pRet, pSrc, sizeof(SizeF));
    return pRet;
}

CD2DLayer::CD2DLayer(CRenderTarget* parent, BOOL autoDestroy)
    : CD2DResource(parent, autoDestroy), m_pLayer(nullptr) {}
CD2DLayer::~CD2DLayer() { CD2DLayer::Destroy(); }
HRESULT CD2DLayer::Create(CRenderTarget* parent) {
    return impl__Create_CD2DLayer__UEAAJPEAVCRenderTarget___Z(this, parent);
}
void CD2DLayer::Destroy() { impl__Destroy_CD2DLayer__UEAAXXZ(this); }
