// CD2DResource — OpenMFC implementation.
// Sources: global_d2d_resource.cpp

// Real CD2DResource (afxrendertarget.h) lifecycle: ctor, dtor, ReCreate,
// and CRenderTarget::VerifyResource. CD2DResource is the root of the D2D
// resource wrapper hierarchy (sizeof=24: vptr + m_pResource + m_pParentTarget).
// State is held in a side map keyed by `this` (repo convention, avoids ABI
// layout risk), guarded by a mutex.
//
// Named global_*.cpp so build_phase4.sh's shard glob auto-compiles it.

#include <cstdint>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


struct CD2DResourceState {
    void* resource = nullptr;           // ID2D1Resource*
    const void* parentTarget = nullptr; // CRenderTarget*
    int autoDestroy = 0;                // bAutoDestroy
    int resourceValid = 0;              // m_bIsResourceValid
};

std::unordered_map<const void*, CD2DResourceState> g_d2dResourceStates;
std::mutex g_d2dResourceMutex;


extern "C" {

// Symbol: ??0CD2DResource@@IEAA@PEAVCRenderTarget@@H@Z
// protected ctor: CD2DResource(CRenderTarget* pParentTarget, BOOL bAutoDestroy)
// MS x64: RCX=this, RDX=pParentTarget, R8=bAutoDestroy; returns this in RAX.
void* MS_ABI impl___0CD2DResource__IEAA_PEAVCRenderTarget__H_Z(
    void* self, void* pParentTarget, int bAutoDestroy)
{
    if (!self) return nullptr;
    std::lock_guard<std::mutex> lock(g_d2dResourceMutex);
    CD2DResourceState& state = g_d2dResourceStates[self];
    state.resource = nullptr;
    state.parentTarget = pParentTarget;
    state.autoDestroy = bAutoDestroy;
    state.resourceValid = 0;
    return self;
}

// Symbol: ??1CD2DResource@@MEAA@XZ
// protected virtual dtor: ~CD2DResource()
// Real MFC: if (m_bAutoDestroy && m_pResource) Destroy(); then erase.
// MS x64: RCX=this; returns this in RAX.
void* MS_ABI impl___1CD2DResource__MEAA_XZ(void* self)
{
    if (!self) return nullptr;
    std::lock_guard<std::mutex> lock(g_d2dResourceMutex);
    auto it = g_d2dResourceStates.find(self);
    if (it != g_d2dResourceStates.end()) {
        // If auto-destroy is on and we have a resource, release it.
        // (Real MFC calls virtual Destroy() here; we just drop the state.)
        it->second.resource = nullptr;
        g_d2dResourceStates.erase(it);
    }
    return self;
}

// Symbol: ?ReCreate@CD2DResource@@MEAAJPEAVCRenderTarget@@@Z
// protected virtual: long CD2DResource::ReCreate(CRenderTarget* pParentTarget)
// Real MFC: m_pParentTarget = pParentTarget; m_bIsResourceValid = FALSE;
//           Destroy(); return TRUE;
// MS x64: RCX=this, RDX=pParentTarget; returns long in RAX.
long MS_ABI impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(
    void* self, void* pParentTarget)
{
    if (!self) return 0;
    std::lock_guard<std::mutex> lock(g_d2dResourceMutex);
    auto it = g_d2dResourceStates.find(self);
    if (it == g_d2dResourceStates.end()) return 0;
    CD2DResourceState& state = it->second;
    state.parentTarget = pParentTarget;
    state.resourceValid = 0;
    // Real MFC calls Destroy() here (virtual) to release the old ID2D1Resource.
    // We just drop the pointer.
    state.resource = nullptr;
    return 1; // real MFC returns TRUE on success
}

// Symbol: ?VerifyResource@CRenderTarget@@IEAAHPEAVCD2DResource@@@Z
// protected: int CRenderTarget::VerifyResource(CD2DResource* pResource)
// Real MFC:
//   if (pResource && (!pResource->m_bIsResourceValid ||
//                     pResource->m_pParentTarget != this))
//       if (!pResource->ReCreate(this)) return FALSE;
//   return TRUE;
// MS x64: RCX=this(CRenderTarget*), RDX=pResource(CD2DResource*); returns int in EAX.
int MS_ABI impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(
    void* self, void* pResource)
{
    if (!pResource) return 1; // null resource is trivially valid
    std::lock_guard<std::mutex> lock(g_d2dResourceMutex);
    auto it = g_d2dResourceStates.find(pResource);
    if (it == g_d2dResourceStates.end()) return 1; // no state = assume valid
    CD2DResourceState& state = it->second;
    if (!state.resourceValid || state.parentTarget != self) {
        // Resource needs to be (re)created for this render target.
        // Update parent and invalidate (real MFC calls pResource->ReCreate(this)).
        state.parentTarget = self;
        state.resourceValid = 0;
        state.resource = nullptr;
    }
    return 1;
}

} // extern "C"
