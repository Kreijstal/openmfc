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
#define MFC_DIALOGS_CTRLS_DESC(Cls, Size, Schema, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }
MFC_DIALOGS_CTRLS_DESC(CMFCCaptionBar, 1720, 0xFFFF, &CPane::classCPane);
MFC_DIALOGS_CTRLS_DESC(CMFCColorPropertySheet, 408, 0xFFFF, &CPropertySheet::classCPropertySheet);
MFC_DIALOGS_CTRLS_DESC(CMFCCustomColorsPropertyPage, 1072, 0xFFFF, &CPropertyPage::classCPropertyPage);
MFC_DIALOGS_CTRLS_DESC(CMFCDesktopAlertDialog, 3776, 0xFFFF, &CDialogEx::classCDialogEx);
MFC_DIALOGS_CTRLS_DESC(CMFCDesktopAlertWnd, 6160, 0xFFFF, &CWnd::classCWnd);
MFC_DIALOGS_CTRLS_DESC(CMFCDropDownFrame, 5488, 0x80000001, &CMiniFrameWnd::classCMiniFrameWnd);
MFC_DIALOGS_CTRLS_DESC(CMFCDynamicLayout, 80, 0xFFFF, &CObject::classCObject);
MFC_DIALOGS_CTRLS_DESC(CMFCEditBrowseCtrl, 336, 0xFFFF, &CEdit::classCEdit);
MFC_DIALOGS_CTRLS_DESC(CMFCMaskedEdit, 296, 0xFFFF, &CEdit::classCEdit);
#undef MFC_DIALOGS_CTRLS_DESC
} // namespace

#define MFC_DIALOGS_CTRLS_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CMFCCaptionBar@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCCaptionBar@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCCaptionBar,
           impl__GetThisClass_CMFCCaptionBar__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCCaptionBar__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCColorPropertySheet@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCColorPropertySheet@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCColorPropertySheet,
           impl__GetThisClass_CMFCColorPropertySheet__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCColorPropertySheet__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCCustomColorsPropertyPage@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCCustomColorsPropertyPage@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCCustomColorsPropertyPage,
           impl__GetThisClass_CMFCCustomColorsPropertyPage__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCCustomColorsPropertyPage__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDesktopAlertDialog@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDesktopAlertDialog@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDesktopAlertDialog,
           impl__GetThisClass_CMFCDesktopAlertDialog__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDesktopAlertDialog__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDesktopAlertWnd@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDesktopAlertWnd@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDesktopAlertWnd,
           impl__GetThisClass_CMFCDesktopAlertWnd__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDesktopAlertWnd__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDropDownFrame@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDropDownFrame@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDropDownFrame,
           impl__GetThisClass_CMFCDropDownFrame__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDropDownFrame__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCDynamicLayout@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCDynamicLayout@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCDynamicLayout,
           impl__GetThisClass_CMFCDynamicLayout__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCDynamicLayout__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCEditBrowseCtrl@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCEditBrowseCtrl@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCEditBrowseCtrl,
           impl__GetThisClass_CMFCEditBrowseCtrl__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCEditBrowseCtrl__UEBAPEAUCRuntimeClass__XZ)
// Symbol: ?GetThisClass@CMFCMaskedEdit@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CMFCMaskedEdit@@UEBAPEAUCRuntimeClass@@XZ
MFC_DIALOGS_CTRLS_GETTERS(CMFCMaskedEdit,
           impl__GetThisClass_CMFCMaskedEdit__SAPEAUCRuntimeClass__XZ,
           impl__GetRuntimeClass_CMFCMaskedEdit__UEBAPEAUCRuntimeClass__XZ)
#undef MFC_DIALOGS_CTRLS_GETTERS
