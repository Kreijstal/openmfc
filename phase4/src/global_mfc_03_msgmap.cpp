// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for
// 8 classes. Each gets a base-chained AFX_MSGMAP with an empty
// terminator entry array (real handler entries are mfc140u code addresses we
// cannot replicate). pfnGetBaseMap delegates to the base's exported
// GetThisMessageMap (real-MFC _AFXDLL base resolution); base harvested by
// pointer identity from mfc140u.dll. GetMessageMap is virtual-const (takes a
// this ptr), GetThisMessageMap is static; both return the static map. The
// // Symbol: markers drive build_phase4.sh's weak-stub auto-exclude.
// Named global_*_msgmap.cpp so the shard glob compiles it.

#include "openmfc/afxwin.h"   // AFX_MSGMAP, AFX_MSGMAP_ENTRY, AfxSig_end, AFXAPI

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// Shared empty message-map entries (terminator only): the class exposes no
// handlers we implement; routing falls through to the base via pfnGetBaseMap.
const AFX_MSGMAP_ENTRY g_emptyMsgEntries[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
} // namespace

// Base GetThisMessageMap getters (exported elsewhere in this DLL).
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCColorDialog() { return impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorPickerCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorPopupMenu() { return impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorPropertySheet() { return impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCCustomColorsPropertyPage() { return impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCDesktopAlertDialog() { return impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCDesktopAlertWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCDropDownFrame() { return impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCColorDialog_msgmap = { gbm_CMFCColorDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCColorPickerCtrl_msgmap = { gbm_CMFCColorPickerCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCColorPopupMenu_msgmap = { gbm_CMFCColorPopupMenu, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCColorPropertySheet_msgmap = { gbm_CMFCColorPropertySheet, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCCustomColorsPropertyPage_msgmap = { gbm_CMFCCustomColorsPropertyPage, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCDesktopAlertDialog_msgmap = { gbm_CMFCDesktopAlertDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCDesktopAlertWnd_msgmap = { gbm_CMFCDesktopAlertWnd, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCDropDownFrame_msgmap = { gbm_CMFCDropDownFrame, g_emptyMsgEntries };
} // namespace

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
