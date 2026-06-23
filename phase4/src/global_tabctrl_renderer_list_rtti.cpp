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

namespace {
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
} // namespace

#define TABCTRL_RENDERER_LIST_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCBaseTabCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCBaseTabCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCBaseTabCtrl,
           impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCBaseTabCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBarTabCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarTabCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCOutlookBarTabCtrl,
           impl__GetThisClass_CMFCOutlookBarTabCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarTabCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCTabCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCTabCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCTabCtrl,
           impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCTabCtrl__UEBAPEAUCRuntimeClass__XZ)
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
// Symbol: ?GetThisClass@CMFCListCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCListCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCListCtrl,
           impl__GetThisClass_CMFCListCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCListCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCShellListCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCShellListCtrl@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCShellListCtrl,
           impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCShellListCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCAutoHideBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCAutoHideBar@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCAutoHideBar,
           impl__GetThisClass_CMFCAutoHideBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCAutoHideBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCAutoHideButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCAutoHideButton@@UEBAPEAUCRuntimeClass@@XZ
TABCTRL_RENDERER_LIST_GETTERS(CMFCAutoHideButton,
           impl__GetThisClass_CMFCAutoHideButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCAutoHideButton__UEBAPEAUCRuntimeClass__XZ)
#undef TABCTRL_RENDERER_LIST_GETTERS
