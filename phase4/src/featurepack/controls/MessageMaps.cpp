// MessageMaps — OpenMFC implementation.
// Sources: global_mfc_02_msgmap.cpp, global_mfc_03_msgmap.cpp, global_mfc_04_msgmap.cpp, global_mfc_05_msgmap.cpp, global_mfc_06_msgmap.cpp, global_mfc_07_msgmap.cpp, global_mfc_08_msgmap.cpp, global_other_13_msgmap.cpp, global_other_14_msgmap.cpp

#include "detail/Mfc02MsgmapSupport.h"
#include "detail/Mfc03MsgmapSupport.h"
#include "detail/Mfc04MsgmapSupport.h"
#include "detail/Mfc05MsgmapSupport.h"
#include "detail/Mfc06MsgmapSupport.h"
#include "detail/Mfc07MsgmapSupport.h"
#include "detail/Mfc08MsgmapSupport.h"
#include "detail/Other13MsgmapSupport.h"
#include "detail/Other14MsgmapSupport.h"

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCButton,
       impl__GetMessageMap_CMFCButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCCaptionBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCCaptionBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCCaptionBar,
       impl__GetMessageMap_CMFCCaptionBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCCaptionBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorBar,
       impl__GetMessageMap_CMFCColorBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorBar__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorButton@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorButton@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorButton,
       impl__GetMessageMap_CMFCColorButton__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorButton__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCColorDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorDialog,
       impl__GetMessageMap_CMFCColorDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorPickerCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorPickerCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorPickerCtrl,
       impl__GetMessageMap_CMFCColorPickerCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorPickerCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorPopupMenu@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorPopupMenu@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorPopupMenu,
       impl__GetMessageMap_CMFCColorPopupMenu__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorPopupMenu__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCColorPropertySheet@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCColorPropertySheet@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCColorPropertySheet,
       impl__GetMessageMap_CMFCColorPropertySheet__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCColorPropertySheet__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCCustomColorsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCCustomColorsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCCustomColorsPropertyPage,
       impl__GetMessageMap_CMFCCustomColorsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCCustomColorsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCDesktopAlertDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDesktopAlertDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDesktopAlertDialog,
       impl__GetMessageMap_CMFCDesktopAlertDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDesktopAlertDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCDesktopAlertWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDesktopAlertWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDesktopAlertWnd,
       impl__GetMessageMap_CMFCDesktopAlertWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDesktopAlertWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCDropDownFrame@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDropDownFrame@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDropDownFrame,
       impl__GetMessageMap_CMFCDropDownFrame__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDropDownFrame__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCDropDownListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDropDownListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDropDownListBox,
       impl__GetMessageMap_CMFCDropDownListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDropDownListBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCEditBrowseCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCEditBrowseCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCEditBrowseCtrl,
       impl__GetMessageMap_CMFCEditBrowseCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCEditBrowseCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCFontComboBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCFontComboBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCFontComboBox,
       impl__GetMessageMap_CMFCFontComboBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCFontComboBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCHeaderCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCHeaderCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCHeaderCtrl,
       impl__GetMessageMap_CMFCHeaderCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCHeaderCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCImageEditorDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCImageEditorDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCImageEditorDialog,
       impl__GetMessageMap_CMFCImageEditorDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCImageEditorDialog__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCImagePaintArea@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCImagePaintArea@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCImagePaintArea,
       impl__GetMessageMap_CMFCImagePaintArea__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCImagePaintArea__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCLinkCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCLinkCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCLinkCtrl,
       impl__GetMessageMap_CMFCLinkCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCLinkCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCListCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCListCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCListCtrl,
       impl__GetMessageMap_CMFCListCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCListCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCMaskedEdit@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCMaskedEdit@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCMaskedEdit,
       impl__GetMessageMap_CMFCMaskedEdit__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCMaskedEdit__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCPreviewCtrlImpl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCPreviewCtrlImpl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCPreviewCtrlImpl,
       impl__GetMessageMap_CMFCPreviewCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCPreviewCtrlImpl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCReBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCReBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCReBar,
       impl__GetMessageMap_CMFCReBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCReBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCShadowWnd@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCShadowWnd@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCShadowWnd,
       impl__GetMessageMap_CMFCShadowWnd__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCShadowWnd__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCShellListCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCShellListCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCShellListCtrl,
       impl__GetMessageMap_CMFCShellListCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCShellListCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCShellTreeCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCShellTreeCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCShellTreeCtrl,
       impl__GetMessageMap_CMFCShellTreeCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCShellTreeCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCSpinButtonCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCSpinButtonCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCSpinButtonCtrl,
       impl__GetMessageMap_CMFCSpinButtonCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCStandardColorsPropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCStandardColorsPropertyPage@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCStandardColorsPropertyPage,
       impl__GetMessageMap_CMFCStandardColorsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCStandardColorsPropertyPage__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCStatusBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCStatusBar,
       impl__GetMessageMap_CMFCStatusBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCStatusBar__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CMFCToolTipCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCToolTipCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCToolTipCtrl,
       impl__GetMessageMap_CMFCToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCToolTipCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CVSListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CVSListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CVSListBox,
       impl__GetMessageMap_CVSListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CVSListBox__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }
// Symbol: ?GetMessageMap@CVSListBoxBase@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CVSListBoxBase@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CVSListBoxBase,
       impl__GetMessageMap_CVSListBoxBase__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CVSListBoxEditCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CVSListBoxEditCtrl@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CVSListBoxEditCtrl,
       impl__GetMessageMap_CVSListBoxEditCtrl__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CVSListBoxEditCtrl__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
#undef DEF_MM
