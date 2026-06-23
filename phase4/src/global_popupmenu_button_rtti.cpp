// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for 9 classes.
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
#define POPUPMENU_BUTTON_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
POPUPMENU_BUTTON_DESC(CMFCPopupMenu, 6600, 0x80000001, &CMiniFrameWnd::classCMiniFrameWnd);
POPUPMENU_BUTTON_DESC(CMFCColorPopupMenu, 11992, 0xFFFF, &classCMFCPopupMenu);
POPUPMENU_BUTTON_DESC(CMFCDropDownListBox, 6640, 0xFFFF, &classCMFCPopupMenu);
POPUPMENU_BUTTON_DESC(CMFCRibbonPanelMenu, 12128, 0xFFFF, &classCMFCPopupMenu);
POPUPMENU_BUTTON_DESC(CMFCRibbonMiniToolBar, 12144, 0xFFFF, &classCMFCRibbonPanelMenu);
POPUPMENU_BUTTON_DESC(CMFCButton, 2848, 0xFFFF, &CButton::classCButton);
POPUPMENU_BUTTON_DESC(CMFCColorButton, 3016, 0xFFFF, &classCMFCButton);
POPUPMENU_BUTTON_DESC(CMFCLinkCtrl, 2880, 0xFFFF, &classCMFCButton);
POPUPMENU_BUTTON_DESC(CMFCMenuButton, 2888, 0xFFFF, &classCMFCButton);
#undef POPUPMENU_BUTTON_DESC
} // namespace

#define POPUPMENU_BUTTON_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCPopupMenu@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCPopupMenu@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCPopupMenu,
           impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCPopupMenu__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCColorPopupMenu@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorPopupMenu@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCColorPopupMenu,
           impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorPopupMenu__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownListBox@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownListBox@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCDropDownListBox,
           impl__GetThisClass_CMFCDropDownListBox__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownListBox__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonPanelMenu@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonPanelMenu@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCRibbonPanelMenu,
           impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonPanelMenu__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCRibbonMiniToolBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCRibbonMiniToolBar@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCRibbonMiniToolBar,
           impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCRibbonMiniToolBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCButton@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCButton,
           impl__GetThisClass_CMFCButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCColorButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorButton@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCColorButton,
           impl__GetThisClass_CMFCColorButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorButton__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCLinkCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCLinkCtrl@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCLinkCtrl,
           impl__GetThisClass_CMFCLinkCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCLinkCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCMenuButton@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCMenuButton@@UEBAPEAUCRuntimeClass@@XZ
POPUPMENU_BUTTON_GETTERS(CMFCMenuButton,
           impl__GetThisClass_CMFCMenuButton__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCMenuButton__UEBAPEAUCRuntimeClass__XZ)
#undef POPUPMENU_BUTTON_GETTERS
