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
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ();

namespace {
// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.
const AFX_MSGMAP* AFXAPI gbm_CMFCDropDownListBox() { return impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCDropDownToolBar() { return impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCEditBrowseCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCFontComboBox() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCHeaderCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCImageEditorDialog() { return impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCImagePaintArea() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCKeyMapDialog() { return impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ(); }

// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).
const AFX_MSGMAP classCMFCDropDownListBox_msgmap = { gbm_CMFCDropDownListBox, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCDropDownToolBar_msgmap = { gbm_CMFCDropDownToolBar, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCEditBrowseCtrl_msgmap = { gbm_CMFCEditBrowseCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCFontComboBox_msgmap = { gbm_CMFCFontComboBox, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCHeaderCtrl_msgmap = { gbm_CMFCHeaderCtrl, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCImageEditorDialog_msgmap = { gbm_CMFCImageEditorDialog, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCImagePaintArea_msgmap = { gbm_CMFCImagePaintArea, g_emptyMsgEntries };
const AFX_MSGMAP classCMFCKeyMapDialog_msgmap = { gbm_CMFCKeyMapDialog, g_emptyMsgEntries };
} // namespace

#define DEF_MM(Cls, GMSym, GTMSym) \
    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \
    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }

// Symbol: ?GetMessageMap@CMFCDropDownListBox@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDropDownListBox@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDropDownListBox,
       impl__GetMessageMap_CMFCDropDownListBox__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDropDownListBox__KAPEBUAFX_MSGMAP__XZ)
// Symbol: ?GetMessageMap@CMFCDropDownToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCDropDownToolBar@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCDropDownToolBar,
       impl__GetMessageMap_CMFCDropDownToolBar__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCDropDownToolBar__KAPEBUAFX_MSGMAP__XZ)
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
// Symbol: ?GetMessageMap@CMFCKeyMapDialog@@MEBAPEBUAFX_MSGMAP@@XZ
// Symbol: ?GetThisMessageMap@CMFCKeyMapDialog@@KAPEBUAFX_MSGMAP@@XZ
DEF_MM(CMFCKeyMapDialog,
       impl__GetMessageMap_CMFCKeyMapDialog__MEBAPEBUAFX_MSGMAP__XZ,
       impl__GetThisMessageMap_CMFCKeyMapDialog__KAPEBUAFX_MSGMAP__XZ)
#undef DEF_MM
