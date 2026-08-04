// CSmartDockingInfo — OpenMFC implementation.
// Sources: global_csmartdockinginfo.cpp, manual_small_stub_implementations.cpp

#include "detail/CSmartDockingInfoSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#define AFX_SD_MARKERS_NUM 5
// Symbol: ??0CSmartDockingInfo@@QEAA@XZ
// CSmartDockingInfo::CSmartDockingInfo()
extern "C" void* MS_ABI impl___0CSmartDockingInfo__QEAA_XZ(void* pThis) {
    S_Csmartdockinginfo* p = static_cast<S_Csmartdockinginfo*>(pThis);
    p->vfptr = static_cast<void*>(const_cast<void**>(&g_CSmartDockingInfo_vtbl[0]));

    // Real mfc140u default-construction values (harvested via differential dump
    // against the real DLL): a 93x93 marker cluster, central-group offset 5, a
    // magenta (0x00FF00FF) transparency color key, and CLR_NONE (0xFFFFFFFF) for
    // the four tone/base colors.
    p->cx = 93;
    p->cy = 93;
    p->m_nCentralGroupOffset = 5;

    p->m_clrTransparent    = 0x00FF00FF;
    p->m_clrToneSrc        = 0xFFFFFFFF;
    p->m_clrToneDest       = 0xFFFFFFFF;
    p->m_clrBaseBackground = 0xFFFFFFFF;
    p->m_clrBaseBorder     = 0xFFFFFFFF;

    for (int i = 0; i < AFX_SD_MARKERS_NUM; ++i) {
        p->m_uiMarkerBmpResID[i]      = 0;
        p->m_uiMarkerLightBmpResID[i] = 0;
    }
    p->m_uiBaseBmpResID = 0;

    p->m_bUseThemeColorInShading = FALSE;
    p->m_bIsAlphaMarkers         = FALSE;

    return pThis;
}
#undef AFX_SD_MARKERS_NUM
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?CopyTo@CSmartDockingInfo@@QEAAXAEAV1@@Z
// Symbol: ?CopyTo@CSmartDockingInfo@@QEAAXAEAV1@@Z
extern "C" void MS_ABI impl__CopyTo_CSmartDockingInfo__QEAAXAEAV1__Z(
    void* pThis, void* pSrc) {
    (void)pThis;
    (void)pSrc;
}

// CSmartDockingInfo's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace csmartdockinginfo {
extern void* const g_CSmartDockingInfo_vtbl[5] = {
    reinterpret_cast<void*>(&sd_GetRuntimeClass),
    reinterpret_cast<void*>(&sd_VectorDeletingDtor),
    reinterpret_cast<void*>(&sd_Serialize),
    reinterpret_cast<void*>(&sd_AssertValid),
    reinterpret_cast<void*>(&sd_Dump),
};
} } }
