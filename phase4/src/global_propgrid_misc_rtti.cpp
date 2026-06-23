// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 2 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define PROPGRID_MISC_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
PROPGRID_MISC_DESC(CMFCPropertyGridToolTipCtrl, 280, 0xFFFF, &CWnd::classCWnd);
PROPGRID_MISC_DESC(CMFCPropertySheetCategoryInfo, 168, 0xFFFF, &CObject::classCObject);
#undef PROPGRID_MISC_DESC
} // namespace

#define PROPGRID_MISC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCPropertyGridToolTipCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridToolTipCtrl@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_MISC_GETTERS(CMFCPropertyGridToolTipCtrl,
           impl__GetThisClass_CMFCPropertyGridToolTipCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridToolTipCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertySheetCategoryInfo@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertySheetCategoryInfo@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_MISC_GETTERS(CMFCPropertySheetCategoryInfo,
           impl__GetThisClass_CMFCPropertySheetCategoryInfo__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertySheetCategoryInfo__UEBAPEAUCRuntimeClass__XZ)
#undef PROPGRID_MISC_GETTERS
