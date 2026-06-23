// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 4 classes.
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
#define RIBBON_PANELS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
RIBBON_PANELS_DESC(CMFCRibbonCustomizeCategory, 72, 0xFFFF, &CObject::classCObject);
RIBBON_PANELS_DESC(CMFCRibbonMainPanel, 1832, 0xFFFF, &CMFCRibbonPanel::classCMFCRibbonPanel);
RIBBON_PANELS_DESC(CMFCRibbonRichEditCtrl, 264, 0xFFFF, &CRichEditCtrl::classCRichEditCtrl);
RIBBON_PANELS_DESC(CMFCRibbonStatusBar, 8728, 0xFFFF, &CMFCRibbonBar::classCMFCRibbonBar);
#undef RIBBON_PANELS_DESC
} // namespace

#define RIBBON_PANELS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCRibbonCustomizeCategory@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizeCategory@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonCustomizeCategory,
           impl__GetThisClass_CMFCRibbonCustomizeCategory__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCustomizeCategory__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonMainPanel@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonMainPanel@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonMainPanel,
           impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonMainPanel__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonRichEditCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonRichEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonRichEditCtrl,
           impl__GetThisClass_CMFCRibbonRichEditCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonRichEditCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonStatusBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonStatusBar@@UEBAPEAUCRuntimeClass@@XZ
RIBBON_PANELS_GETTERS(CMFCRibbonStatusBar,
           impl__GetThisClass_CMFCRibbonStatusBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonStatusBar__UEBAPEAUCRuntimeClass__XZ)
#undef RIBBON_PANELS_GETTERS
