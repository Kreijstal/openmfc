// RuntimeClasses — OpenMFC implementation.
// Sources: global_propgrid_misc_rtti.cpp, global_propgrid_rtti.cpp

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

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define PROPGRID_MISC_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
PROPGRID_MISC_DESC(CMFCPropertyGridToolTipCtrl, 280, 0xFFFF, &CWnd::classCWnd);
PROPGRID_MISC_DESC(CMFCPropertySheetCategoryInfo, 168, 0xFFFF, &CObject::classCObject);
#undef PROPGRID_MISC_DESC

#define PROPGRID_MISC_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pNextClass, m_pClassInit.
#define PROPGRID_DESC(Cls, Size, Schema, BaseDesc) \
    static CRuntimeClass* AFXAPI _openmfc_gb_##Cls() { return (BaseDesc); } \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, &_openmfc_gb_##Cls, nullptr, nullptr }
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

#define PROPGRID_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

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
#undef PROPGRID_MISC_GETTERS
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
// Symbol: ?GetThisClass@CMFCPropertySheet@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertySheet@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertySheet,
           impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertySheet__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPropertyGridCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPropertyGridCtrl@@UEBAPEAUCRuntimeClass@@XZ
PROPGRID_GETTERS(CMFCPropertyGridCtrl,
           impl__GetThisClass_CMFCPropertyGridCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPropertyGridCtrl__UEBAPEAUCRuntimeClass__XZ)
#undef PROPGRID_GETTERS
#undef PROPGRID_GETTERS
