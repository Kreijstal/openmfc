// RuntimeClasses — OpenMFC implementation.
// Sources: global_mfc_misc_ctrls_rtti.cpp, global_tabctrl_renderer_list_rtti.cpp

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 7 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxmfc.h"
#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define MFC_MISC_CTRLS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
MFC_MISC_CTRLS_DESC(CMFCMousePropertyPage, 1072, 0xFFFF, &CPropertyPage::classCPropertyPage);
MFC_MISC_CTRLS_DESC(CMFCPreviewCtrlImpl, 264, 0xFFFF, &CWnd::classCWnd);
MFC_MISC_CTRLS_DESC(CMFCReBar, 1040, 0xFFFF, &CPane::classCPane);
MFC_MISC_CTRLS_DESC(CMFCShellTreeCtrl, 256, 0xFFFF, &CTreeCtrl::classCTreeCtrl);
MFC_MISC_CTRLS_DESC(CMFCStandardColorsPropertyPage, 1048, 0xFFFF, &CPropertyPage::classCPropertyPage);
MFC_MISC_CTRLS_DESC(CMFCStatusBar, 1056, 0xFFFF, &CPane::classCPane);
MFC_MISC_CTRLS_DESC(CMFCVisualManagerVS2008, 824, 0xFFFF, &CMFCVisualManagerVS2005::classCMFCVisualManagerVS2005);
#undef MFC_MISC_CTRLS_DESC

#define MFC_MISC_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afx.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define TABCTRL_RENDERER_LIST_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
TABCTRL_RENDERER_LIST_DESC(CMFCBaseTabCtrl, 960, 0xFFFF, &CWnd::classCWnd);
TABCTRL_RENDERER_LIST_DESC(CMFCOutlookBarTabCtrl, 11768, 0xFFFF, &classCMFCBaseTabCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCTabCtrl, 15720, 0xFFFF, &classCMFCBaseTabCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCControlRenderer, 512, 0xFFFF, &CObject::classCObject);
TABCTRL_RENDERER_LIST_DESC(CMFCShadowRenderer, 512, 0xFFFF, &classCMFCControlRenderer);
TABCTRL_RENDERER_LIST_DESC(CMFCListCtrl, 576, 0xFFFF, &CListCtrl::classCListCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCShellListCtrl, 632, 0xFFFF, &classCMFCListCtrl);
TABCTRL_RENDERER_LIST_DESC(CMFCAutoHideBar, 1112, 0xFFFF, &CPane::classCPane);
TABCTRL_RENDERER_LIST_DESC(CMFCAutoHideButton, 64, 0xFFFF, &CObject::classCObject);
#undef TABCTRL_RENDERER_LIST_DESC

#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

#define MFC_MISC_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCVisualManagerVS2008@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2008@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCVisualManagerVS2008,
           impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCVisualManagerVS2008__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_MISC_CTRLS_GETTERS
#undef MFC_MISC_CTRLS_GETTERS
#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }
// Symbol: ?GetThisClass@CMFCControlRenderer@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCControlRenderer@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCControlRenderer,
           impl__GetThisClass_CMFCControlRenderer__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCControlRenderer__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCShadowRenderer@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCShadowRenderer@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCShadowRenderer,
           impl__GetThisClass_CMFCShadowRenderer__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCShadowRenderer__UEBAPEAUCRuntimeClass__XZ)
#undef TABCTRL_RENDERER_LIST_GETTERS
