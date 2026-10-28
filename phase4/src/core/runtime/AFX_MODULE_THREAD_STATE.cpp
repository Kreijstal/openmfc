// Native per-module thread state, factory and allocation handlers.
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CHandleMapSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <cstdlib>
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int MS_ABI impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ(CToolTipCtrl*);
extern "C" void* MS_ABI impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
    S_Cthreadlocalobject*, void*);
namespace openmfc {
namespace detail {
namespace modulethreadstate {
int MS_ABI DefaultAllocationFailure(size_t) {
    impl__AfxThrowMemoryException__YAXXZ();
    __assume(0);
}
} // namespace modulethreadstate
} // namespace detail
} // namespace openmfc
AFX_MODULE_THREAD_STATE::AFX_MODULE_THREAD_STATE()
    : m_pReserved10(nullptr), m_nSafetyPoolSize(0x108),
      m_pfnNewHandler(&openmfc::detail::modulethreadstate::DefaultAllocationFailure),
      m_mapSocketHandle(10), m_mapDeadSockets(10), m_listSocketNotifications(10),
      m_nLastHit(UINT_PTR(-1)), m_pLastInfo(nullptr), m_nLastHitMessage(UINT_PTR(-1)) {
    // Other fields are zeroed by CNoTrackObject allocation; placement
    // construction preserves their existing contents as native MFC does.
}
AFX_MODULE_THREAD_STATE::~AFX_MODULE_THREAD_STATE() {
    if (m_pToolTip)
        impl__DestroyToolTipCtrl_CToolTipCtrl__QEAAHXZ(m_pToolTip);
    std::free(m_pLastInfo);
    CHandleMap** maps[] = {&m_pmapHWND, &m_pmapHMENU, &m_pmapHDC, &m_pmapHGDIOBJ,
                           &m_pmapHIMAGELIST};
    for (auto field : maps) {
        auto* map = *field;
        if (map) {
            map->~CHandleMap();
            std::free(map);
        }
    }
    while (m_listSocketNotifications.m_nCount)
        std::free(m_listSocketNotifications.RemoveHead());
}
namespace {
void* MS_ABI CreateModuleThreadState() { return new AFX_MODULE_THREAD_STATE; }
} // namespace
// Symbol: ??0AFX_MODULE_THREAD_STATE@@QEAA@XZ
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI
impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ(AFX_MODULE_THREAD_STATE* self) {
    return ::new (self) AFX_MODULE_THREAD_STATE;
}
// Symbol: ??1AFX_MODULE_THREAD_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ(AFX_MODULE_THREAD_STATE* self) {
    // This export names a destructor body, independent of the incoming vtable.
    self->AFX_MODULE_THREAD_STATE::~AFX_MODULE_THREAD_STATE();
}
AFX_MODULE_THREAD_STATE* openmfc::detail::modulethreadstate::ForModule(AFX_MODULE_STATE* module) {
    auto* slot = reinterpret_cast<S_Cthreadlocalobject*>(reinterpret_cast<BYTE*>(module) + 0xf8);
    auto* state = static_cast<AFX_MODULE_THREAD_STATE*>(
        impl__GetData_CThreadLocalObject__QEAAPEAVCNoTrackObject__P6APEAV2_XZ_Z(
            slot, reinterpret_cast<void*>(&CreateModuleThreadState)));
    if (!state)
        impl__AfxThrowInvalidArgException__YAXXZ();
    return state;
}
// Symbol: ?AfxGetModuleThreadState@@YAPEAVAFX_MODULE_THREAD_STATE@@XZ
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI
impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ() {
    return openmfc::detail::modulethreadstate::ForModule(
        impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ());
}
// Symbol: ?AfxGetNewHandler@@YAP6AH_K@ZXZ
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI impl__AfxGetNewHandler__YAP6AH_K_ZXZ() {
    return impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pfnNewHandler;
}
// Symbol: ?AfxSetNewHandler@@YAP6AH_K@ZP6AH0@Z@Z
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI
impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(AFX_MODULE_THREAD_STATE::NewHandler handler) {
    auto* state = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    auto previous = state->m_pfnNewHandler;
    state->m_pfnNewHandler = handler;
    return previous;
}
static_assert(sizeof(AFX_MODULE_THREAD_STATE) == 0x138, "native module thread size");
static_assert(offsetof(AFX_MODULE_THREAD_STATE, m_pCurrentWinThread) == 8 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_nTempMapLock) == 0x20 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_pfnNewHandler) == 0x50 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_mapSocketHandle) == 0x60 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_mapDeadSockets) == 0x98 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_listSocketNotifications) == 0xd0 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_pToolTip) == 0x108 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_pLastInfo) == 0x120 &&
                  offsetof(AFX_MODULE_THREAD_STATE, m_bLastStatus) == 0x130,
              "native module thread fields");
