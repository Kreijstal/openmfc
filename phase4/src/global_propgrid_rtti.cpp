// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
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
#define PROPGRID_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
PROPGRID_DESC(CMFCPropertyGridProperty, 352, 0xFFFF, &CObject::classCObject);
PROPGRID_DESC(CMFCPropertyGridColorProperty, 440, 0xFFFF, &classCMFCPropertyGridProperty);
PROPGRID_DESC(CMFCPropertyGridFileProperty, 384, 0xFFFF, &classCMFCPropertyGridProperty);
PROPGRID_DESC(CMFCPropertyGridFontProperty, 552, 0xFFFF, &classCMFCPropertyGridProperty);
PROPGRID_DESC(CMFCPropertyPage, 384, 0xFFFF, &CPropertyPage::classCPropertyPage);
PROPGRID_DESC(CMFCRibbonCustomizePropertyPage, 7376, 0xFFFF, &classCMFCPropertyPage);
PROPGRID_DESC(CMFCPropertySheet, 29232, 0xFFFF, &CPropertySheet::classCPropertySheet);
PROPGRID_DESC(CMFCRibbonCustomizeDialog, 29240, 0xFFFF, &classCMFCPropertySheet);
PROPGRID_DESC(CMFCPropertyGridCtrl, 1720, 0xFFFF, &CWnd::classCWnd);
#undef PROPGRID_DESC
} // namespace

#define PROPGRID_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCPropertyGridProperty@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridProperty@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyGridProperty,
           impl__GetThisClass_CMFCPropertyGridProperty__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridProperty__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertyGridColorProperty@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridColorProperty@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyGridColorProperty,
           impl__GetThisClass_CMFCPropertyGridColorProperty__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridColorProperty__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertyGridFileProperty@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridFileProperty@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyGridFileProperty,
           impl__GetThisClass_CMFCPropertyGridFileProperty__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridFileProperty__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertyGridFontProperty@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridFontProperty@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyGridFontProperty,
           impl__GetThisClass_CMFCPropertyGridFontProperty__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridFontProperty__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyPage,
           impl__GetThisClass_CMFCPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonCustomizePropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCRibbonCustomizePropertyPage,
           impl__GetThisClass_CMFCRibbonCustomizePropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCustomizePropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertySheet@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertySheet@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertySheet,
           impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertySheet__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonCustomizeDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizeDialog@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCRibbonCustomizeDialog,
           impl__GetThisClass_CMFCRibbonCustomizeDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonCustomizeDialog__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertyGridCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridCtrl@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyGridCtrl,
           impl__GetThisClass_CMFCPropertyGridCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef PROPGRID_GETTERS
