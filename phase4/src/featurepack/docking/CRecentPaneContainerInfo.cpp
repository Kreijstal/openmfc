// CRecentPaneContainerInfo — OpenMFC implementation.
// Sources: global_crecentpanecontainerinfo.cpp

#include "detail/CRecentPaneContainerInfoSupport.h"

// Symbol: ?Init@CRecentPaneContainerInfo@@QEAAXXZ
extern "C" void MS_ABI impl__Init_CRecentPaneContainerInfo__QEAAXXZ(void* pThis) {
    S_Crecentpanecontainerinfo* s = static_cast<S_Crecentpanecontainerinfo*>(pThis);
    // Verified byte-exact against real mfc140u (differential dump): the docked
    // rect defaults to {0,0,30,30}, recent percent to 50, left-bar flag TRUE.
    s->m_rectDockedRect.left = s->m_rectDockedRect.top = 0;
    s->m_rectDockedRect.right = s->m_rectDockedRect.bottom = 30;
    s->m_nRecentPercent = 50;
    s->m_bIsRecentLeftBar = TRUE;
    s->m_pRecentBarContainer = nullptr;
    s->m_pRecentContainerOfTabWnd = nullptr;
    list_removeall(s);
}
// Symbol: ??0CRecentPaneContainerInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CRecentPaneContainerInfo__QEAA_XZ(void* pThis) {
    S_Crecentpanecontainerinfo* s = static_cast<S_Crecentpanecontainerinfo*>(pThis);
    s->vfptr = (void*)&g_CRecentPaneContainerInfo_vtbl[0];
    list_init_empty(s);
    impl__Init_CRecentPaneContainerInfo__QEAAXXZ(pThis);
    return pThis;
}
// Symbol: ??1CRecentPaneContainerInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecentPaneContainerInfo__UEAA_XZ(void* pThis) {
    S_Crecentpanecontainerinfo* s = static_cast<S_Crecentpanecontainerinfo*>(pThis);
    list_removeall(s);
}
// Symbol: ??4CRecentPaneContainerInfo@@QEAAAEAV0@AEAV0@@Z
extern "C" void* MS_ABI impl___4CRecentPaneContainerInfo__QEAAAEAV0_AEAV0__Z(void* pThis, void* pSrc) {
    S_Crecentpanecontainerinfo* s = static_cast<S_Crecentpanecontainerinfo*>(pThis);
    S_Crecentpanecontainerinfo* src = static_cast<S_Crecentpanecontainerinfo*>(pSrc);
    if (s != src) copy_from(s, src);
    return pThis;
}
// Symbol: ?SetInfo@CRecentPaneContainerInfo@@UEAAXAEAV1@@Z
extern "C" void MS_ABI impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(void* pThis, void* pSrc) {
    S_Crecentpanecontainerinfo* s = static_cast<S_Crecentpanecontainerinfo*>(pThis);
    S_Crecentpanecontainerinfo* src = static_cast<S_Crecentpanecontainerinfo*>(pSrc);
    if (s != src) copy_from(s, src);
}
// Symbol: ?StoreDockInfo@CRecentPaneContainerInfo@@UEAAXPEAVCPaneContainer@@PEAVCDockablePane@@1@Z
// Records the recent container of the pane. The rect/percent/sibling-list
// derivation requires CPaneContainer/CDockablePane internals not modeled here,
// so only the container linkage is captured faithfully (see notes).
extern "C" void MS_ABI impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
    void* pThis, void* pRecentContainer, void* /*pBar*/, void* /*pTabbedBar*/) {
    S_Crecentpanecontainerinfo* s = static_cast<S_Crecentpanecontainerinfo*>(pThis);
    s->m_pRecentBarContainer = pRecentContainer;
}

// CRecentPaneContainerInfo's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
extern void* const g_CRecentPaneContainerInfo_vtbl[7] = {
    (void*)&vslot_GetRuntimeClass_Crecentpanecontainerinfo,
    (void*)&vslot_deleting_dtor,
    (void*)&vslot_Serialize_Crecentpanecontainerinfo,
    (void*)&vslot_AssertValid_Crecentpanecontainerinfo,
    (void*)&vslot_Dump_Crecentpanecontainerinfo,
    (void*)&vslot_StoreDockInfo,
    (void*)&vslot_SetInfo,
};
} } }
