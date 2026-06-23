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

namespace {
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
} // namespace

#define MFC_MISC_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCMousePropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCMousePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCMousePropertyPage,
           impl__GetThisClass_CMFCMousePropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCMousePropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPreviewCtrlImpl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPreviewCtrlImpl@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCPreviewCtrlImpl,
           impl__GetThisClass_CMFCPreviewCtrlImpl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPreviewCtrlImpl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCReBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCReBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCReBar,
           impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCReBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCShellTreeCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCShellTreeCtrl@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCShellTreeCtrl,
           impl__GetThisClass_CMFCShellTreeCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCShellTreeCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCStandardColorsPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCStandardColorsPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCStandardColorsPropertyPage,
           impl__GetThisClass_CMFCStandardColorsPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCStandardColorsPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCStatusBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCStatusBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCStatusBar,
           impl__GetThisClass_CMFCStatusBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCStatusBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCVisualManagerVS2008@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2008@@UEBAPEAUCRuntimeClass@@XZ
MFC_MISC_CTRLS_GETTERS(CMFCVisualManagerVS2008,
           impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCVisualManagerVS2008__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_MISC_CTRLS_GETTERS
