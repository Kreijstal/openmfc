// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 11 classes.
// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested
// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor
// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define MFC_TOOLBARS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
MFC_TOOLBARS_DESC(CMFCBaseToolBar, 1016, 0xFFFF, &CPane::classCPane);
MFC_TOOLBARS_DESC(CMFCToolBar, 4944, 0x80000001, &classCMFCBaseToolBar);
MFC_TOOLBARS_DESC(CMFCPopupMenuBar, 5152, 0x00000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCColorBar, 5384, 0x00000001, &classCMFCPopupMenuBar);
MFC_TOOLBARS_DESC(CMFCDropDownToolBar, 4944, 0x00000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCMenuBar, 5056, 0x80000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCOutlookBarPane, 11112, 0x00000001, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCOutlookBarToolBar, 5008, 0xFFFF, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCPrintPreviewToolBar, 4944, 0xFFFF, &classCMFCToolBar);
MFC_TOOLBARS_DESC(CMFCRibbonPanelMenuBar, 5520, 0xFFFF, &classCMFCPopupMenuBar);
MFC_TOOLBARS_DESC(CMFCTasksPaneToolBar, 4960, 0x00000001, &classCMFCToolBar);
#undef MFC_TOOLBARS_DESC
} // namespace

#define MFC_TOOLBARS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCBaseToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCBaseToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCBaseToolBar,
           impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCBaseToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCToolBar,
           impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPopupMenuBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPopupMenuBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCPopupMenuBar,
           impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPopupMenuBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCColorBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCColorBar,
           impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCDropDownToolBar,
           impl__GetThisClass_CMFCDropDownToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCMenuBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCMenuBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCMenuBar,
           impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCMenuBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBarPane@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarPane@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCOutlookBarPane,
           impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarPane__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCOutlookBarToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCOutlookBarToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCOutlookBarToolBar,
           impl__GetThisClass_CMFCOutlookBarToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCOutlookBarToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCPrintPreviewToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPrintPreviewToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCPrintPreviewToolBar,
           impl__GetThisClass_CMFCPrintPreviewToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPrintPreviewToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonPanelMenuBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonPanelMenuBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCRibbonPanelMenuBar,
           impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonPanelMenuBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCTasksPaneToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCTasksPaneToolBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_TOOLBARS_GETTERS(CMFCTasksPaneToolBar,
           impl__GetThisClass_CMFCTasksPaneToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCTasksPaneToolBar__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_TOOLBARS_GETTERS
