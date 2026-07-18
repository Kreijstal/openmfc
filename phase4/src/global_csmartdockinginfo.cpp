// CSmartDockingInfo — real OpenMFC implementation.
//
// A CObject-derived value/settings class describing the smart-docking marker
// appearance used by the MFC docking framework (CMFCVisualManager). It carries
// bitmap resource IDs, tone/shading colors and geometry for the docking markers.
//
// Layout (cl.exe /d1reportSingleClassLayout), size(96):
//   0  {vfptr}                         (CObject base)
//   8  CSize    m_sizeTotal            (cx@8, cy@12)
//  16  int      m_nCentralGroupOffset
//  20  COLORREF m_clrTransparent
//  24  COLORREF m_clrToneSrc
//  28  COLORREF m_clrToneDest
//  32  COLORREF m_clrBaseBackground
//  36  COLORREF m_clrBaseBorder
//  40  UINT     m_uiMarkerBmpResID[5]
//  60  UINT     m_uiMarkerLightBmpResID[5]
//  80  UINT     m_uiBaseBmpResID
//  84  BOOL     m_bUseThemeColorInShading
//  88  BOOL     m_bIsAlphaMarkers
//  92  (tail padding to 8-byte alignment -> 96)

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#define AFX_SD_MARKERS_NUM 5

namespace {

struct S {
    void*    vfptr;                                        // 0
    LONG     cx;                                           // 8  (m_sizeTotal.cx)
    LONG     cy;                                           // 12 (m_sizeTotal.cy)
    int      m_nCentralGroupOffset;                        // 16
    COLORREF m_clrTransparent;                             // 20
    COLORREF m_clrToneSrc;                                 // 24
    COLORREF m_clrToneDest;                                // 28
    COLORREF m_clrBaseBackground;                          // 32
    COLORREF m_clrBaseBorder;                              // 36
    UINT     m_uiMarkerBmpResID[AFX_SD_MARKERS_NUM];       // 40
    UINT     m_uiMarkerLightBmpResID[AFX_SD_MARKERS_NUM];  // 60
    UINT     m_uiBaseBmpResID;                             // 80
    BOOL     m_bUseThemeColorInShading;                    // 84
    BOOL     m_bIsAlphaMarkers;                            // 88
};

static_assert(sizeof(S) == 96, "CSmartDockingInfo must be size(96)");
static_assert(offsetof(S, cy) == 12, "m_sizeTotal.cy at 12");
static_assert(offsetof(S, m_clrTransparent) == 20, "m_clrTransparent at 20");
static_assert(offsetof(S, m_uiMarkerBmpResID) == 40, "marker array at 40");
static_assert(offsetof(S, m_bIsAlphaMarkers) == 88, "m_bIsAlphaMarkers at 88");

// ---- vtable slot wrappers -------------------------------------------------
// CSmartDockingInfo is not DYNAMIC, so its vtable entries are the inherited
// CObject implementations (GetRuntimeClass, Serialize, AssertValid, Dump).

// slot 0: CObject::GetRuntimeClass (not overridden) — delegate to the real
// exported CObject impl so the descriptor pointer is identical to the base
// class's, without referencing the '?'-mangled data symbol directly (mingw
// cannot emit a valid .refptr for such a name; a link-time undefined ref
// results). Same idiom as global_cmfcacceleratorkey.cpp.
extern "C" void* MS_ABI
impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

void* MS_ABI sd_GetRuntimeClass(void* pThis) {
    return impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(pThis);
}

// slot 1: vector deleting destructor. CSmartDockingInfo owns no resources.
void* MS_ABI sd_VectorDeletingDtor(void* p, unsigned int flags) {
    if (flags & 1) {
        ::operator delete(p);
    }
    return p;
}

// slot 2: CObject::Serialize — base implementation is a no-op.
void MS_ABI sd_Serialize(void* /*pThis*/, void* /*ar*/) {
}

// slot 3: CObject::AssertValid — no-op in release.
void MS_ABI sd_AssertValid(void* /*pThis*/) {
}

// slot 4: CObject::Dump — no-op in release.
void MS_ABI sd_Dump(void* /*pThis*/, void* /*dc*/) {
}

void* const g_CSmartDockingInfo_vtbl[5] = {
    reinterpret_cast<void*>(&sd_GetRuntimeClass),
    reinterpret_cast<void*>(&sd_VectorDeletingDtor),
    reinterpret_cast<void*>(&sd_Serialize),
    reinterpret_cast<void*>(&sd_AssertValid),
    reinterpret_cast<void*>(&sd_Dump),
};

} // namespace

// ===========================================================================
// Exports
// ===========================================================================

// Symbol: ??0CSmartDockingInfo@@QEAA@XZ
// CSmartDockingInfo::CSmartDockingInfo()
extern "C" void* MS_ABI impl___0CSmartDockingInfo__QEAA_XZ(void* pThis) {
    S* p = static_cast<S*>(pThis);
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

// Symbol: ?CopyTo@CSmartDockingInfo@@QEAAXAEAV1@@Z
// void CSmartDockingInfo::CopyTo(CSmartDockingInfo& params)
// Member-wise copy of *this into params (vfptr untouched).
extern "C" void MS_ABI impl__CopyTo_CSmartDockingInfo__QEAAXAEAV1__Z(void* pThis, void* pParams) {
    const S* src = static_cast<const S*>(pThis);
    S*       dst = static_cast<S*>(pParams);

    dst->cx = src->cx;
    dst->cy = src->cy;
    dst->m_nCentralGroupOffset = src->m_nCentralGroupOffset;

    dst->m_clrTransparent    = src->m_clrTransparent;
    dst->m_clrToneSrc        = src->m_clrToneSrc;
    dst->m_clrToneDest       = src->m_clrToneDest;
    dst->m_clrBaseBackground = src->m_clrBaseBackground;
    dst->m_clrBaseBorder     = src->m_clrBaseBorder;

    for (int i = 0; i < AFX_SD_MARKERS_NUM; ++i) {
        dst->m_uiMarkerBmpResID[i]      = src->m_uiMarkerBmpResID[i];
        dst->m_uiMarkerLightBmpResID[i] = src->m_uiMarkerLightBmpResID[i];
    }
    dst->m_uiBaseBmpResID = src->m_uiBaseBmpResID;

    dst->m_bUseThemeColorInShading = src->m_bUseThemeColorInShading;
    dst->m_bIsAlphaMarkers         = src->m_bIsAlphaMarkers;
}
