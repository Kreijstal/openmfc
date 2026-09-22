// CMFCKeyMapDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
// FindListStorage: the header-only reader of the side table in which
// OpenMFC's CObList keeps its nodes (see CategoryButtons below).
#include "detail/FilecoreSupport.h"

#include <commctrl.h>
#include <cstddef>
#include <cstring>
#include <cwchar>
#include <new>

// ===========================================================================
// CMFCKeyMapDialog -- the feature-pack keyboard-map dialog (CDialogEx).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  All RVAs
// and absolute addresses in this file are mfc140u.  Most of these exports
// have no entry in the RVA symbol map (mfc140u_rva_symbols.json); they were
// located through the class vftable 0x1802ebef8 (installed by the constructor
// at 0x746d0) and the message map 0x1802ebe00 (returned by GetMessageMap,
// 0x749a0), and every one of the RVAs below was then confirmed by resolving
// the export's ordinal (mfc_complete_ordinal_mapping.json) through
// mfc140u.dll's export address table:
//
//   vslot  64 (+0x200) OnNotify          0x755c0
//   vslot  66 (+0x210) DoDataExchange    0x748f0
//   vslot  96 (+0x300) OnInitDialog      0x749b0
//   vslot 101 (+0x328) CopyKeyMap        0x75670
//   vslot 102 (+0x330) PrintKeyMap       0x75880
//   vslot 103 (+0x338) OnPrintHeader     0x75b00
//   vslot 104 (+0x340) OnPrintItem       0x75cc0
//   vslot 105 (+0x348) FormatItem        0x76260
//   vslot 106 (+0x350) OnSetColumns      0x763d0
//   vslot 107 (+0x358) OnInsertItem      0x76520
//   vslot 108 (+0x360) SetColumnsWidth   0x75fb0
//   vslot 109 (+0x368) GetCommandKeys    0x76720
//   WM_SIZE                               OnSize              0x754c0
//   WM_DESTROY                            OnDestroy           0x76130
//   CBN_SELCHANGE 16642 (VIEW_TYPE)       OnSelchangeViewType 0x75110
//   CBN_SELCHANGE 16601 (CATEGORY)        OnSelchangeCategory 0x75370
//   BN_CLICKED   16991 (COPY_KEYMAP)      OnCopy              0x75460
//   BN_CLICKED   16990 (PRINT_KEYMAP)     OnPrint             0x75490
// The virtuals 101..109 follow the declaration order of afxkeymapdialog.h in
// the 14.51 SDK on this host.  The list-view sort callback that OnNotify and
// OnSelchange* hand to LVM_SORTITEMS is the non-exported 0x74590.
//
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives.  S_KeyMapDlg pins it: the constructor (0x746d0)
// builds CDialogEx at +0, then the members at the offsets below, and the
// scalar deleting destructor (0x747d0) loads 0x1dc8 as the object size
// (`mov $0x1dc8,%edx` before its sized-delete call at 0x74800).  The
// member names are those of afxkeymapdialog.h, whose declaration order matches
// the offsets.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftable 0x1802ebef8 at +0x00 and
//      the CStatic / CComboBox vftables in the plain control members after
//      ??0CWnd@@QEAA@XZ.  OpenMFC has no MSVC-layout vtable for this class,
//      so +0x00 keeps whatever the CDialogEx constructor thunk leaves there
//      (it installs none) and the plain controls are built with the exported
//      CWnd constructor alone, as featurepack/toolbar/CMFCToolBarComboBoxButton.cpp
//      does for its CComboBox.  Consequently the retail bodies' virtual calls
//      on `this` are devirtualised to this file's own thunks, and the virtual
//      calls on other objects (CMFCButton::SizeToContent, CFrameWnd::
//      GetMessageString, CWinThread::GetMainWnd, CDC::DrawText, the
//      CSettingsStore methods) to the exported thunks / Win32 calls they
//      resolve to in retail; each site says so.
//
//  (2) Frame / template accelerator tables.  Retail reads CFrameWnd::
//      m_hAccelTable and CMultiDocTemplate::m_hAccelTable at +0xf8.  OpenMFC's
//      CFrameWnd keeps m_hAccelTable at +0xf0 (the member its LoadAccelTable
//      writes), which is read through the C++ member here, as
//      CMFCToolBarsCustomizeDialog.cpp does; OpenMFC's CMultiDocTemplate has
//      no accelerator table at all (see OnInitDialog / OnSelchangeViewType).
//
//  (3) OpenMFC's CObList keeps its nodes in a side table, so the retail walk of
//      a category list (m_pNodeHead at +0x08, node data at +0x10) becomes a
//      FindListStorage walk over the same object, as in
//      CMFCToolBarsCustomizeDialog.cpp (whose FillCategoriesComboBox put the
//      list pointers into the combo box).
//
//  (4) CDC arguments.  OnPrintHeader / OnPrintItem receive a CDC&; retail
//      reads m_hAttribDC (+0x10) directly and draws through CDC::DrawText
//      (vslot 28), whose body (0x1e6ce0) is `::DrawTextW(m_hDC /*+0x08*/, ...)`.
//      That call is made directly here, so a CDC subclass that overrides
//      DrawText (CPreviewDC, CMetaFileDC) is bypassed.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the definitions in the tree
// (file named on each line); where a definition's parameter order departs
// from the member-function ABI, the line says so and headerRequests reports it.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_IPEAVCWnd___Z(CDialogEx* pThis, UINT nIDTemplate, CWnd* pParentWnd);   // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis);                                                      // detail/DlgcoreSupport.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                           // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl__OnDestroy_CDialogEx__IEAAXXZ(CDialogEx* pThis);                                         // core/dialog/CDialogEx.cpp

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                         // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis);                   // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CComboBox__UEAA_XZ(void* pThis);                    // core/controls/CtorDtorPlacement.cpp
extern "C" void* MS_ABI impl___0CMFCListCtrl__QEAA_XZ(void* pThis);                 // featurepack/controls/CMFCListCtrl.cpp
extern "C" void* MS_ABI impl___1CMFCListCtrl__UEAA_XZ(void* pThis);                 // featurepack/controls/CMFCListCtrl.cpp
extern "C" void  MS_ABI impl__SetSortColumn_CMFCListCtrl__QEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd);   // featurepack/controls/CMFCListCtrl.cpp
extern "C" void* MS_ABI impl___0CMFCButton__QEAA_XZ(void* pThis);                   // featurepack/controls/Thunks.cpp
extern "C" void  MS_ABI impl___1CMFCButton__UEAA_XZ(void* pThis);                   // featurepack/controls/Thunks.cpp
extern "C" void  MS_ABI impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(void* pThis, unsigned int uiBmpResId, unsigned int uiBmpHotResId, int bChecked, unsigned int uiBmpDsblResID);   // featurepack/controls/CMFCButton.cpp
extern "C" void  MS_ABI impl__SetTooltip_CMFCButton__QEAAXPEB_W_Z(CMFCButton* pThis, const wchar_t* lpszToolTipText);       // featurepack/controls/Thunks.cpp
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(void* pThis, CSize* pRet, int bCalcOnly);       // featurepack/controls/CMFCButton.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                               // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                                            // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__MoveWindow_CWnd__QEAAXHHHHH_Z(CWnd* pThis, int x, int y, int nWidth, int nHeight, int bRepaint);   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult);    // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                        // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);                  // core/window/Thunks.cpp
extern "C" void  MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString);   // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);          // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                                // core/window/CWnd.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);                                 // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CFrameWnd* pThis, unsigned int nID, CString* rMessage);   // core/frame/CFrameWnd.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                                           // core/app/Globals.cpp
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);                          // core/app/CWinThread.cpp

extern "C" int   MS_ABI impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(CListCtrl* pThis, int nCol, const wchar_t* lpszColumnHeading, int nFormat, int nWidth, int nSubItem);   // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__InsertItem_CListCtrl__QEAAHIHPEB_WIIH_J_Z(CListCtrl* pThis, UINT nMask, int nItem, const wchar_t* lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);   // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__SetItem_CListCtrl__QEAAHHHIPEB_WHII_J_Z(CListCtrl* pThis, int nItem, int nSubItem, UINT nMask, const wchar_t* lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam);   // core/controls/CListCtrl.cpp
extern "C" uintptr_t MS_ABI impl__GetItemData_CListCtrl__QEBA_KH_Z(const CListCtrl* pThis, int nItem);            // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__GetItemText_CListCtrl__QEBAHHHPEA_WH_Z(const CListCtrl* pThis, int nItem, int nSubItem, wchar_t* lpszText, int nLen);   // core/controls/CListCtrl.cpp

extern "C" void* MS_ABI impl___0CMFCToolBarsCustomizeDialog__QEAA_PEAVCFrameWnd__HIPEAV__CList_PEAUCRuntimeClass__PEAU1____Z(void* pThis, CFrameWnd* pWndParentFrame, int bAutoSetFromMenus, unsigned int uiFlags, void* plistCustomPages);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
extern "C" void  MS_ABI impl___1CMFCToolBarsCustomizeDialog__UEAA_XZ(void* pThis);                                  // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
extern "C" void  MS_ABI impl__EnableUserDefinedToolbars_CMFCToolBarsCustomizeDialog__QEAAXH_Z(void* pThis, int bEnable);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
extern "C" void  MS_ABI impl__FillCategoriesComboBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCComboBox__H_Z(const void* pThis, CWnd* pWndCategory, int bAddEmpty);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp

extern "C" void* MS_ABI impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(void* pThis, LPACCEL lpAccel);           // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis);                                           // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const void* pThis, CString* str);   // featurepack/customize/CMFCAcceleratorKey.cpp

extern "C" CPrintDialog* MS_ABI impl___0CPrintDialog__QEAA_HKPEAVCWnd___Z(CPrintDialog* pThis, int bPrintSetupOnly, unsigned long dwFlags, CWnd* pParentWnd);   // core/dialog/CPrintDialog.cpp
extern "C" __int64 MS_ABI impl__DoModal_CPrintDialog__UEAA_JXZ(CPrintDialog* pThis);                               // core/dialog/Thunks.cpp

extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(void* pThis, int bAdmin, int bReadOnly);   // core/app/CSettingsStoreSP.cpp
extern "C" int   MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* path);                  // core/app/CSettingsStore.cpp
extern "C" int   MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* key);              // core/app/CSettingsStore.cpp
extern "C" int   MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAVCRect___Z(void* self, const wchar_t* name, CRect* rect);         // core/app/CSettingsStore.cpp
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(void* self, const wchar_t* name, const CRect* rect);  // core/app/CSettingsStore.cpp
// core/app/CWinAppEx.cpp defines this by-value CString return with the result
// slot FIRST and `this` second (the member-function ABI is this=RCX, result=RDX);
// it is declared -- and called -- exactly as that definition and
// core/frame/CFrameImpl.cpp have it.  See headerRequests.
extern "C" void  MS_ABI impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CString* ret, CWinAppEx* pThis, const wchar_t* add);

extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                  // detail/RegcoreSupport.cpp (retail: AfxGetModuleState()->m_pCurrentWinApp)
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);   // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);     // core/runtime/CCmdTarget.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();                       // core/frame/CFrameWnd.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();                       // core/app/CWinAppEx.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszName, const wchar_t* lpszType);   // core/runtime/Globals.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);            // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowResourceException__YAXXZ();                  // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                     // detail/MemcoreSupport.cpp
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);                             // detail/MemcoreSupport.cpp

// afxGlobalData (RVA 0x3c1620, mfc140u) and its Initialize (RVA 0x6a790,
// mfc140u).  The definition in core/runtime/AFX_GLOBAL_DATA.cpp takes
// `void* pThis` but deliberately ignores it and always initialises the
// exported blob (see the DELIBERATE DEVIATION note there); the blob's address
// is passed here, as retail passes it.
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);

// This file's own thunks that earlier bodies call (the retail call sites are
// virtual; see deviation (1)).
extern "C" void  MS_ABI impl__CopyKeyMap_CMFCKeyMapDialog__MEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__PrintKeyMap_CMFCKeyMapDialog__MEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__OnPrintHeader_CMFCKeyMapDialog__MEBAHAEAVCDC__HH_Z(const void* pThis, CDC* pDC, int nPage, int cx);
extern "C" int   MS_ABI impl__OnPrintItem_CMFCKeyMapDialog__MEBAHAEAVCDC__HHHH_Z(const void* pThis, CDC* pDC, int nItem, int y, int cx, int bCalcHeight);
extern "C" CString* MS_ABI impl__FormatItem_CMFCKeyMapDialog__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const void* pThis, CString* pRet, int nItem);
extern "C" void  MS_ABI impl__OnSetColumns_CMFCKeyMapDialog__MEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnInsertItem_CMFCKeyMapDialog__MEAAXPEAVCMFCToolBarButton__H_Z(void* pThis, CMFCToolBarButton* pButton, int nItem);
extern "C" void  MS_ABI impl__SetColumnsWidth_CMFCKeyMapDialog__MEAAXXZ(void* pThis);
extern "C" CString* MS_ABI impl__GetCommandKeys_CMFCKeyMapDialog__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(const void* pThis, CString* pRet, unsigned int uiCmdID);
extern "C" void  MS_ABI impl__OnSelchangeViewType_CMFCKeyMapDialog__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnSelchangeCategory_CMFCKeyMapDialog__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// The retail object (0x1dc8 bytes).  Offsets from the ctor (0x746d0), the dtor
// (0x74820) and DoDataExchange (0x748f0).
// ---------------------------------------------------------------------------
struct S_KeyMapDlg {
    alignas(8) unsigned char m_base[0x170];     // +0x000 CDialogEx (OpenMFC's sizeof(CDialogEx) == 0x170, asserted below)
    alignas(8) unsigned char m_wndAccelLabel[0xe8];   // +0x170 CStatic       (DDX 16996 IDC_AFXBARRES_ACCEL_LABEL)
    alignas(8) unsigned char m_KeymapList[0x240];     // +0x258 CMFCListCtrl  (DDX 16992 IDC_AFXBARRES_KEYLIST)
    alignas(8) unsigned char m_wndCategoryList[0xe8]; // +0x498 CComboBox     (DDX 16601 IDC_AFXBARRES_CATEGORY)
    alignas(8) unsigned char m_wndViewIcon[0xe8];     // +0x580 CStatic       (DDX 16526 IDC_AFXBARRES_VIEW_ICON)
    alignas(8) unsigned char m_wndViewTypeList[0xe8]; // +0x668 CComboBox     (DDX 16642 IDC_AFXBARRES_VIEW_TYPE)
    alignas(8) unsigned char m_ButtonPrint[0xb20];    // +0x750 CMFCButton    (DDX 16990 IDC_AFXBARRES_PRINT_KEYMAP)
    alignas(8) unsigned char m_ButtonCopy[0xb20];     // +0x1270 CMFCButton   (DDX 16991 IDC_AFXBARRES_COPY_KEYMAP)
    CFrameWnd*    m_pWndParentFrame;            // +0x1d90
    HACCEL        m_hAccelTable;                // +0x1d98
    LPACCEL       m_lpAccel;                    // +0x1da0
    int           m_nAccelSize;                 // +0x1da8
    void*         m_pDlgCust;                   // +0x1db0 CMFCToolBarsCustomizeDialog*
    int           m_nSortedCol;                 // +0x1db8
    int           m_bSortAscending;             // +0x1dbc
    int           m_bEnablePrint;               // +0x1dc0 (const BOOL)
};
static_assert(offsetof(S_KeyMapDlg, m_wndAccelLabel) == 0x170, "ctor: CWnd ctor on this+0x170");
static_assert(offsetof(S_KeyMapDlg, m_KeymapList) == 0x258, "ctor: CMFCListCtrl ctor on this+0x258");
static_assert(offsetof(S_KeyMapDlg, m_wndCategoryList) == 0x498, "ctor: CWnd ctor on this+0x498");
static_assert(offsetof(S_KeyMapDlg, m_wndViewIcon) == 0x580, "ctor: CWnd ctor on this+0x580");
static_assert(offsetof(S_KeyMapDlg, m_wndViewTypeList) == 0x668, "ctor: CWnd ctor on this+0x668");
static_assert(offsetof(S_KeyMapDlg, m_ButtonPrint) == 0x750, "ctor: CMFCButton ctor on this+0x750");
static_assert(offsetof(S_KeyMapDlg, m_ButtonCopy) == 0x1270, "ctor: CMFCButton ctor on this+0x1270");
static_assert(offsetof(S_KeyMapDlg, m_pWndParentFrame) == 0x1d90, "ctor: mov %rsi,0x1d90");
static_assert(offsetof(S_KeyMapDlg, m_hAccelTable) == 0x1d98, "ctor: mov %rax,0x1d98");
static_assert(offsetof(S_KeyMapDlg, m_lpAccel) == 0x1da0, "ctor: mov %rax,0x1da0");
static_assert(offsetof(S_KeyMapDlg, m_nAccelSize) == 0x1da8, "ctor: mov %eax,0x1da8");
static_assert(offsetof(S_KeyMapDlg, m_pDlgCust) == 0x1db0, "dtor: mov 0x1db0(%rcx),%rcx");
static_assert(offsetof(S_KeyMapDlg, m_nSortedCol) == 0x1db8, "ctor: mov %eax,0x1db8");
static_assert(offsetof(S_KeyMapDlg, m_bSortAscending) == 0x1dbc, "ctor: movl $0x1,0x1dbc");
static_assert(offsetof(S_KeyMapDlg, m_bEnablePrint) == 0x1dc0, "ctor: mov %edi,0x1dc0");
static_assert(sizeof(S_KeyMapDlg) == 0x1dc8, "scalar deleting dtor 0x747d0: mov $0x1dc8,%edx");
static_assert(sizeof(CDialogEx) == 0x170, "OpenMFC CDialogEx fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CStatic) == 0xe8 && sizeof(CComboBox) == 0xe8, "plain controls are bare CWnds");
static_assert(sizeof(CMFCButton) <= 0xb20, "the exported CMFCButton ctor thunk placement-constructs OpenMFC's CMFCButton");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "every retail body reads the control HWNDs at member+0x40");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "OnInsertItem / OnSelchange*: 0x24(%rdx)");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "OnInsertItem / OnPrintItem: +0x38");
static_assert(offsetof(CMFCToolBarButton, m_strTextCustom) == 0x40, "OnInsertItem: +0x40");

// CMFCButton members the retail OnInitDialog writes directly (public in
// afxbutton.h: m_nFlatStyle is the first member after the 0xe8-byte CButton,
// m_bDrawFocus the sixth).  featurepack/controls/CMFCButton.cpp reads the same
// two words through S_Cmfcbutton (declared in detail/CMFCButtonSupport.h, which
// names +0xe8 "m_nStyle" and +0xfc m_bDrawFocus).
constexpr std::size_t kOffButtonFlatStyle = 0xe8;
constexpr std::size_t kOffButtonDrawFocus = 0xfc;
constexpr int kButtonStyleFlat = 1;             // CMFCButton::BUTTONSTYLE_FLAT

// Resource IDs (afxribbonres.h, 14.51 SDK).
constexpr UINT kIddKeyMap        = 16982;       // IDD_AFXBARRES_KEYMAP (0x4256)
constexpr UINT kIdcAccelLabel    = 16996;       // IDC_AFXBARRES_ACCEL_LABEL
constexpr UINT kIdcKeyList       = 16992;       // IDC_AFXBARRES_KEYLIST
constexpr UINT kIdcCategory      = 16601;       // IDC_AFXBARRES_CATEGORY
constexpr UINT kIdcViewIcon      = 16526;       // IDC_AFXBARRES_VIEW_ICON
constexpr UINT kIdcViewType      = 16642;       // IDC_AFXBARRES_VIEW_TYPE
constexpr UINT kIdcPrintKeyMap   = 16990;       // IDC_AFXBARRES_PRINT_KEYMAP
constexpr UINT kIdcCopyKeyMap    = 16991;       // IDC_AFXBARRES_COPY_KEYMAP
constexpr UINT kIdbPrint         = 16993;       // IDB_AFXBARRES_PRINT   (0x4261)
constexpr UINT kIdbPrint32       = 17050;       // IDB_AFXBARRES_PRINT32 (0x4261 + 0x39)
constexpr UINT kIdbCopy          = 16994;       // IDB_AFXBARRES_COPY    (0x4262)
constexpr UINT kIdbCopy32        = 17051;       // IDB_AFXBARRES_COPY32  (0x4262 + 0x39)
constexpr UINT kIdiDialogIcon    = 16995;       // 0x4263 (named IDI_AFXBARRES_HELP in afxribbonres.h)
constexpr UINT kIdsCommand       = 16029;       // IDS_AFXBARRES_COMMAND
constexpr UINT kIdsKeys          = 16030;       // IDS_AFXBARRES_KEYS
constexpr UINT kIdsDescription   = 16031;       // IDS_AFXBARRES_DESCRIPTION
constexpr UINT kIdsDefaultView   = 16114;       // IDS_AFXBARRES_DEFAULT_VIEW

// String literals of the retail image.
const wchar_t kLongestKeys[]     = L"Ctrl+Shift+W";            // 0x18033f7c8
const wchar_t kRegPlacement[]    = L"KeyMapWindowPlacement";   // 0x18033f728
const wchar_t kRegRect[]         = L"KeyMapWindowRect";        // 0x18033f758
const wchar_t kPrintDocName[]    = L"AfxKeyMapDlg";            // 0x18033f780

inline S_KeyMapDlg* D(void* p) { return static_cast<S_KeyMapDlg*>(p); }
inline const S_KeyMapDlg* D(const void* p) { return static_cast<const S_KeyMapDlg*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline const CWnd* Wnd(const void* p) { return static_cast<const CWnd*>(p); }
inline HWND HwndOf(const void* pWnd) { return static_cast<const CWnd*>(pWnd)->m_hWnd; }   // member +0x40
inline HWND ListHwnd(const S_KeyMapDlg* d) { return HwndOf(d->m_KeymapList); }                // this+0x298
// OpenMFC's CRect is four ints with no RECT base; the settings store takes
// CRect, the Win32 calls take RECT.
static_assert(sizeof(CRect) == sizeof(RECT) && offsetof(CRect, left) == offsetof(RECT, left) &&
              offsetof(CRect, bottom) == offsetof(RECT, bottom), "CRect has the RECT layout");
inline RECT* AsRect(CRect* p) { return reinterpret_cast<RECT*>(p); }

// OpenMFC's CPrintDialog keeps the DC DoModal returns in its protected m_hDC
// (core/dialog/CPrintDialog.cpp); the same accessor as
// detail/DlgcommonSupport.h's CPrintDialogAccess, which is not included here
// because that header also references CFileDialog's runtime class.
struct PrintDialogAccess : CPrintDialog {
    using CPrintDialog::m_hDC;
};
inline CListCtrl* List(S_KeyMapDlg* d) { return reinterpret_cast<CListCtrl*>(d->m_KeymapList); }
inline const CListCtrl* List(const S_KeyMapDlg* d) { return reinterpret_cast<const CListCtrl*>(d->m_KeymapList); }

// ENSURE(x): retail throws through ?AfxThrowInvalidArgException@@ (0x227720).
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// CStringT::LoadString(UINT) as retail inlines it (AfxFindStringResourceHandle
// 0x2aee00, then the exported LoadString(HINSTANCE, UINT) 0xdb70), under
// ENSURE: a missing resource or a failed load throws.
bool EnsureLoadString(CString& str, UINT nID) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nID)) {
        ThrowInvalidArg();
        return false;
    }
    return true;
}

// CListCtrl::GetItemText(int, int) returning CString -- retail calls the
// exported overload ?GetItemText@CListCtrl@@QEBA?AV?$CStringT...@@HH@Z
// (0x295610).  That overload's definition in core/controls/CListCtrl.cpp
// takes (result, this) in the wrong order for a member function (see
// headerRequests), so the buffer overload ?GetItemText@CListCtrl@@QEBAHHHPEA_WH@Z,
// which has the member-function order, is called instead with a growing
// buffer until the text fits.
CString ListItemText(const S_KeyMapDlg* d, int nItem, int nSubItem) {
    CString str;
    int capacity = 128;
    for (;;) {
        wchar_t* buffer = str.GetBuffer(capacity);
        const int copied = impl__GetItemText_CListCtrl__QEBAHHHPEA_WH_Z(List(d), nItem, nSubItem, buffer, capacity);
        str.ReleaseBuffer(copied > 0 ? copied : 0);
        if (copied < capacity - 1 || capacity >= 32768) return str;
        capacity *= 2;
    }
}

// CListCtrl::SetItemText(int, int, LPCTSTR) as retail inlines it in
// OnInsertItem: an LVITEM with only iSubItem and pszText filled, sent with
// LVM_SETITEMTEXTW (0x1074).  Retail leaves the other LVITEM fields as stack
// garbage; they are zeroed here (LVM_SETITEMTEXT reads only those two).
void ListSetItemText(const S_KeyMapDlg* d, int nItem, int nSubItem, const wchar_t* lpszText) {
    LVITEMW lvi = {};
    lvi.iSubItem = nSubItem;
    lvi.pszText = const_cast<wchar_t*>(lpszText);
    ::SendMessage(ListHwnd(d), LVM_SETITEMTEXTW, static_cast<WPARAM>(static_cast<INT_PTR>(nItem)), reinterpret_cast<LPARAM>(&lvi));
}

// The category combo box carries CObList* item data (FillCategoriesComboBox);
// deviation (3).
typedef CList<CObject*, CObject*> ObListData;
inline const ObListData* CategoryButtons(const CObList* p) {
    return p ? openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(p) : nullptr;
}

// A retail-layout CDC (vfptr, m_hDC +0x08, m_hAttribDC +0x10, m_bPrinting
// +0x18) -- the view OnPrintHeader / OnPrintItem take of their CDC& argument
// (deviation (4)), and the stack object PrintKeyMap builds.
struct DCView {
    void* vfptr;
    HDC   m_hDC;
    HDC   m_hAttribDC;
    int   m_bPrinting;
};
static_assert(offsetof(DCView, m_hDC) == 0x08 && offsetof(DCView, m_hAttribDC) == 0x10, "CDC layout");
inline const DCView* DC(const CDC* p) { return reinterpret_cast<const DCView*>(p); }

// The inline CDC::DrawText(LPCTSTR, int, LPRECT, UINT) -- vslot 28 of the CDC
// vftable, body 0x1e6ce0 `mov 0x8(%rcx),%rcx ; jmp *DrawTextW` (import slot
// 0x1802c6f08 resolves to USER32!DrawTextW).  Deviation (4).
inline int DcDrawText(const CDC* pDC, const CString& str, RECT* pRect, UINT nFormat) {
    return ::DrawTextW(DC(pDC)->m_hDC, str.GetString(), str.GetLength(), pRect, nFormat);
}

// AFX_GLOBAL_DATA::Is32BitIcons() (inline in afxglobals.h) behind the
// accessor that initialises afxGlobalData on first use -- read at the same
// offsets as featurepack/controls/CMFCEditBrowseCtrl.cpp (+0x268
// m_bUseBuiltIn32BitIcons, +0x288 m_nBitsPerPixel, +0x260 / +0x264 the two
// high-contrast flags).  OpenMFC's Initialize has a real body (it fills
// m_nBitsPerPixel and the high-contrast flags through UpdateSysColors), but
// nothing in OpenMFC ever writes +0x268 m_bUseBuiltIn32BitIcons (the exported
// AFX_GLOBAL_DATA constructor is empty), so today this reads FALSE and the
// 16-colour bitmaps are chosen.
inline int GlobalDataInt(std::size_t off) {
    int v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}
bool GlobalIs32BitIcons() {
    if (GlobalDataInt(0x000) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
    return GlobalDataInt(0x268) != 0 && GlobalDataInt(0x288) >= 16 &&
           GlobalDataInt(0x260) == 0 && GlobalDataInt(0x264) == 0;
}

// DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp()).
CWinAppEx* AppEx() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<CWinAppEx*>(pApp);
}

// pApp->GetRegSectionPath(L"KeyMapWindowPlacement") (0x1c7ad0).
CString RegPlacementPath(CWinAppEx* pApp) {
    alignas(CString) unsigned char storage[sizeof(CString)];
    CString* pPath = reinterpret_cast<CString*>(storage);
    impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
        pPath, pApp, kRegPlacement);
    CString strPath(*pPath);
    pPath->~CString();
    return strPath;
}

// The list-view sort callback (non-exported, RVA 0x74590 mfc140u), fully
// transcribed:
//     CMFCKeyMapDialog* pDlg = (CMFCKeyMapDialog*)lParamSort;
//     LVFINDINFO info;  info.flags = LVFI_PARAM;
//     info.lParam = lParam1;  int iIndex1 = pDlg->m_KeymapList.FindItem(&info);   // LVM_FINDITEMW (0x1053), wParam -1
//     info.lParam = lParam2;  int iIndex2 = pDlg->m_KeymapList.FindItem(&info);
//     CString strItem1 = pDlg->m_KeymapList.GetItemText(iIndex1, pDlg->m_nSortedCol);   // 0x295610
//     CString strItem2 = pDlg->m_KeymapList.GetItemText(iIndex2, pDlg->m_nSortedCol);
//     return pDlg->m_bSortAscending ? strItem1.Compare(strItem2) : strItem2.Compare(strItem1);   // wcscmp
// Retail leaves the unused LVFINDINFO fields as stack garbage (zeroed here).
// CStringT::Compare's ATLENSURE on a NULL argument (the AtlThrow(E_FAIL) path
// that begins at the instruction 0x746bd inside this callback, 0x74590) cannot
// fire: a CString's buffer is never NULL.
int CALLBACK KeyMapCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
    const S_KeyMapDlg* d = reinterpret_cast<const S_KeyMapDlg*>(lParamSort);
    LVFINDINFOW info = {};
    info.flags = LVFI_PARAM;
    info.lParam = lParam1;
    const int iIndex1 = static_cast<int>(::SendMessage(ListHwnd(d), LVM_FINDITEMW, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(&info)));
    info.lParam = lParam2;
    const int iIndex2 = static_cast<int>(::SendMessage(ListHwnd(d), LVM_FINDITEMW, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(&info)));
    CString strItem1 = ListItemText(d, iIndex1, d->m_nSortedCol);
    CString strItem2 = ListItemText(d, iIndex2, d->m_nSortedCol);
    return d->m_bSortAscending ? std::wcscmp(strItem1.GetString(), strItem2.GetString())
                               : std::wcscmp(strItem2.GetString(), strItem1.GetString());
}

// m_KeymapList.SortItems(listCompareFunc, (LPARAM)this): LVM_SORTITEMS
// (0x1030) with wParam = this (the lParamSort) and lParam = the callback.
inline void SortKeyList(S_KeyMapDlg* d) {
    ::SendMessage(ListHwnd(d), LVM_SORTITEMS, reinterpret_cast<WPARAM>(d), reinterpret_cast<LPARAM>(&KeyMapCompareProc));
}

} // namespace

// Retail (RVA 0x746d0, mfc140u), transcribed:
//     CDialogEx::CDialogEx(IDD_AFXBARRES_KEYMAP /*0x4256*/, pWndParentFrame);   // 0x3ec90
//     vfptr = 0x1802ebef8;
//     m_wndAccelLabel: CWnd::CWnd() (0x28a700) + CStatic vftable
//     m_KeymapList:    CMFCListCtrl::CMFCListCtrl() (0x77970)
//     m_wndCategoryList, m_wndViewIcon, m_wndViewTypeList: CWnd::CWnd() + CComboBox / CStatic / CComboBox vftable
//     m_ButtonPrint, m_ButtonCopy: CMFCButton::CMFCButton() (0x1c930)
//     m_bEnablePrint = bEnablePrint;  m_pWndParentFrame = pWndParentFrame;
//     m_hAccelTable = NULL;  m_lpAccel = NULL;  m_nAccelSize = 0;
//     m_nSortedCol = 0;  m_bSortAscending = TRUE;
// DEVIATIONS: the vftable stores are deviation (1).  Retail does NOT
// initialise m_pDlgCust (+0x1db0), which the destructor deletes when non-NULL;
// it is set to NULL here so that a dialog destroyed without OnInitDialog
// having run does not delete heap garbage.
// Symbol: ??0CMFCKeyMapDialog@@QEAA@PEAVCFrameWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCKeyMapDialog__QEAA_PEAVCFrameWnd__H_Z(
    void* pThis, CFrameWnd* pWndParentFrame, int bEnablePrint) {
    if (pThis == nullptr) return nullptr;
    S_KeyMapDlg* d = D(pThis);
    impl___0CDialogEx__QEAA_IPEAVCWnd___Z(static_cast<CDialogEx*>(pThis), kIddKeyMap, pWndParentFrame);
    impl___0CWnd__QEAA_XZ(d->m_wndAccelLabel);
    impl___0CMFCListCtrl__QEAA_XZ(d->m_KeymapList);
    impl___0CWnd__QEAA_XZ(d->m_wndCategoryList);
    impl___0CWnd__QEAA_XZ(d->m_wndViewIcon);
    impl___0CWnd__QEAA_XZ(d->m_wndViewTypeList);
    impl___0CMFCButton__QEAA_XZ(d->m_ButtonPrint);
    impl___0CMFCButton__QEAA_XZ(d->m_ButtonCopy);
    d->m_bEnablePrint = bEnablePrint;
    d->m_pWndParentFrame = pWndParentFrame;
    d->m_hAccelTable = nullptr;
    d->m_lpAccel = nullptr;
    d->m_nAccelSize = 0;
    d->m_pDlgCust = nullptr;        // deviation: retail leaves it uninitialised
    d->m_nSortedCol = 0;
    d->m_bSortAscending = TRUE;
    return pThis;
}

// Retail (RVA 0x74820, mfc140u), transcribed:
//     vfptr = 0x1802ebef8;
//     if (m_pDlgCust != NULL) delete m_pDlgCust;          // vslot 1 (scalar deleting dtor), flag 1
//     if (m_lpAccel != NULL) free(m_lpAccel);             // import slot 0x1802c74e8 = ucrt free
//     m_ButtonCopy.~CMFCButton();  m_ButtonPrint.~CMFCButton();          // 0x1cad0
//     m_wndViewTypeList.~CComboBox();                                    // 0x2941a0
//     m_wndViewIcon.~CStatic();                                          // 0x293b30
//     m_wndCategoryList.~CComboBox();
//     m_KeymapList.~CMFCListCtrl();                                      // 0x77a20
//     m_wndAccelLabel.~CStatic();
//     ~CDialogEx() inline: m_Impl (+0x158) vftable reset, m_brBkgr (+0x140)
//         ~CBrush -> CGdiObject::DeleteObject (via 0x1c6f0);
//     CDialog::~CDialog();                                               // tail jump 0x207eb0
// DEVIATIONS: m_pDlgCust is always an object OnInitDialog built with
// ??2@ + the CMFCToolBarsCustomizeDialog ctor thunk, and it carries no
// MSVC vftable (that class's deviation (1)), so the scalar deleting
// destructor is spelled out as the exported destructor thunk + ??3@.
// m_lpAccel is allocated with ??2@ in OnSelchangeViewType and released with
// ??3@ here (retail allocates with ??2@ and frees with the CRT free import;
// pairing OpenMFC's own new/delete keeps allocator and deallocator matched).
// OpenMFC's CDialogEx keeps its background brush in a side table
// (g_dialogExBackgroundState, detail/DlgcoreSupport.cpp) that
// CDialogEx::OnDestroy releases, so there is no m_brBkgr to tear down here.
// OpenMFC has no ~CDialogEx thunk: if OnDestroy never ran (e.g. the dialog was
// never created), the entry CommonConstruct made for this object stays in that
// table, and any brush it holds leaks.
// Symbol: ??1CMFCKeyMapDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCKeyMapDialog__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    if (d->m_pDlgCust != nullptr) {
        impl___1CMFCToolBarsCustomizeDialog__UEAA_XZ(d->m_pDlgCust);
        impl___3_YAXPEAX_Z(d->m_pDlgCust);
    }
    if (d->m_lpAccel != nullptr) {
        impl___3_YAXPEAX_Z(d->m_lpAccel);
    }
    impl___1CMFCButton__UEAA_XZ(d->m_ButtonCopy);
    impl___1CMFCButton__UEAA_XZ(d->m_ButtonPrint);
    impl___1CComboBox__UEAA_XZ(d->m_wndViewTypeList);
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndViewIcon));
    impl___1CComboBox__UEAA_XZ(d->m_wndCategoryList);
    impl___1CMFCListCtrl__UEAA_XZ(d->m_KeymapList);
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndAccelLabel));
    impl___1CDialog__UEAA_XZ(static_cast<CDialog*>(pThis));
}

// Retail (RVA 0x75670, mfc140u), fully transcribed:
//     if ((int)m_KeymapList.GetSelectedCount() <= 0) {     // LVM_GETSELECTEDCOUNT (0x1032); `test %eax,%eax ; jg`
//         MessageBeep((UINT)-1);  return;
//     }
//     CString strText;
//     UINT nFlags = m_KeymapList.GetSelectedCount() != 0 ? LVNI_SELECTED : LVNI_ALL;   // second 0x1032, sbb/and 2
//     for (int nItem = m_KeymapList.GetNextItem(-1, nFlags); nItem >= 0;              // LVM_GETNEXTITEM (0x100c)
//          nItem = m_KeymapList.GetNextItem(nItem, nFlags))
//         strText += FormatItem(nItem) + _T("\r\n");        // vslot 105; operator+ 0x12114 with L"\r\n" (0x18033ef28)
//     if (!strText.IsEmpty() && OpenClipboard(m_hWnd)) {
//         EmptyClipboard();
//         HGLOBAL h = GlobalAlloc(GHND /*0x42*/, (strText.GetLength() + 1) * sizeof(TCHAR));
//         ENSURE(h != NULL);
//         LPTSTR p = (LPTSTR)GlobalLock(h);
//         ENSURE(p != NULL);
//         wcscpy_s(p, (strText.GetLength() + 1) * 2, strText);   // the size argument is doubled in retail too
//         GlobalUnlock(h);
//         SetClipboardData(CF_UNICODETEXT /*13*/, h);
//         CloseClipboard();
//     }
// Every import slot was resolved with iatu.py.  The FormatItem call is
// devirtualised to this file's thunk (deviation (1)).  The ENSURE throws leave
// the clipboard open, as retail does.
// Symbol: ?CopyKeyMap@CMFCKeyMapDialog@@MEAAXXZ
extern "C" void MS_ABI impl__CopyKeyMap_CMFCKeyMapDialog__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    const HWND hList = ListHwnd(d);
    if (static_cast<int>(::SendMessage(hList, LVM_GETSELECTEDCOUNT, 0, 0)) <= 0) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }

    CString strText;
    const UINT nFlags = static_cast<UINT>(::SendMessage(hList, LVM_GETSELECTEDCOUNT, 0, 0)) != 0 ? LVNI_SELECTED : LVNI_ALL;
    for (int nItem = static_cast<int>(::SendMessage(hList, LVM_GETNEXTITEM, static_cast<WPARAM>(-1), nFlags));
         nItem >= 0;
         nItem = static_cast<int>(::SendMessage(hList, LVM_GETNEXTITEM, static_cast<WPARAM>(static_cast<INT_PTR>(nItem)), nFlags))) {
        alignas(CString) unsigned char storage[sizeof(CString)];
        CString* pItem = impl__FormatItem_CMFCKeyMapDialog__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pThis, reinterpret_cast<CString*>(storage), nItem);
        strText += *pItem + L"\r\n";
        pItem->~CString();
    }

    if (strText.IsEmpty()) return;
    if (!::OpenClipboard(Wnd(pThis)->m_hWnd)) return;
    ::EmptyClipboard();
    HGLOBAL hClipbuffer = ::GlobalAlloc(GHND, static_cast<SIZE_T>(strText.GetLength() + 1) * sizeof(wchar_t));
    if (hClipbuffer == nullptr) { ThrowInvalidArg(); return; }
    wchar_t* lpszBuffer = static_cast<wchar_t*>(::GlobalLock(hClipbuffer));
    if (lpszBuffer == nullptr) { ThrowInvalidArg(); return; }
    wcscpy_s(lpszBuffer, static_cast<std::size_t>(strText.GetLength() + 1) * 2, strText.GetString());
    ::GlobalUnlock(hClipbuffer);
    ::SetClipboardData(CF_UNICODETEXT, hClipbuffer);
    ::CloseClipboard();
}

// Retail (RVA 0x748f0, mfc140u), fully transcribed: seven DDX_Control calls
// (0x293870) binding, in order, +0x170 to 16996, +0x258 to 16992, +0x498 to
// 16601, +0x580 to 16526, +0x668 to 16642, +0x750 to 16990 and +0x1270 to
// 16991 (the last one a tail jump).  No CDialogEx::DoDataExchange call.
// Symbol: ?DoDataExchange@CMFCKeyMapDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCKeyMapDialog__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcAccelLabel, d->m_wndAccelLabel);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcKeyList, d->m_KeymapList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCategory, d->m_wndCategoryList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcViewIcon, d->m_wndViewIcon);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcViewType, d->m_wndViewTypeList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcPrintKeyMap, d->m_ButtonPrint);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCopyKeyMap, d->m_ButtonCopy);
}

// Retail (RVA 0x76260, mfc140u), fully transcribed:
//     CString strKeys = m_KeymapList.GetItemText(nItem, 1);          // 0x295610
//     if (strKeys.IsEmpty()) strKeys = _T("-");                       // SetString, L"-" at 0x18033f7e4
//     CString strItem;
//     strItem.Format(_T("%-30s\t%-20s\t%Ts"),                          // 0xda00; format at 0x18033f7e8
//                    (LPCTSTR)m_KeymapList.GetItemText(nItem, 0), (LPCTSTR)strKeys,
//                    (LPCTSTR)m_KeymapList.GetItemText(nItem, 2));
//     return strItem;
// DEVIATION: the format is spelled "%-30ls\t%-20ls\t%ls" -- the same widths,
// with the `l` modifier so the three arguments are read as wide strings
// whichever %s convention the CRT behind OpenMFC's CString::Format
// (vswprintf) follows.  GetItemText goes through ListItemText (see there).
// Symbol: ?FormatItem@CMFCKeyMapDialog@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" CString* MS_ABI impl__FormatItem_CMFCKeyMapDialog__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const void* pThis, CString* pRet, int nItem) {
    if (pRet == nullptr) return pRet;
    new (pRet) CString();
    if (pThis == nullptr) return pRet;
    const S_KeyMapDlg* d = D(pThis);
    CString strKeys = ListItemText(d, nItem, 1);
    if (strKeys.IsEmpty()) strKeys = L"-";
    CString strName = ListItemText(d, nItem, 0);
    CString strDescr = ListItemText(d, nItem, 2);
    pRet->Format(L"%-30ls\t%-20ls\t%ls", strName.GetString(), strKeys.GetString(), strDescr.GetString());
    return pRet;
}

// Retail (RVA 0x76720, mfc140u), fully transcribed:
//     CString str;
//     for (int i = 0; i < m_nAccelSize; i++) {
//         if (uiCmdID == m_lpAccel[i].cmd) {               // movzwl 0x4(ACCEL) vs the full 32-bit uiCmdID
//             ENSURE(&m_lpAccel[i] != NULL);
//             CMFCAcceleratorKey helper(&m_lpAccel[i]);     // stack object: vftable 0x1802da2c8 + the ACCEL*
//             CString strKey;  helper.Format(strKey);       // 0x28f0
//             if (!str.IsEmpty()) str += _T("; ");          // L"; " at 0x18033f720
//             str += strKey;
//         }
//     }
//     return str;
// The helper is built with the exported CMFCAcceleratorKey ctor thunk and
// released with its dtor thunk (retail's inline destructor only resets the
// vftable).
// Symbol: ?GetCommandKeys@CMFCKeyMapDialog@@MEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@I@Z
extern "C" CString* MS_ABI impl__GetCommandKeys_CMFCKeyMapDialog__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
    const void* pThis, CString* pRet, unsigned int uiCmdID) {
    if (pRet == nullptr) return pRet;
    new (pRet) CString();
    if (pThis == nullptr) return pRet;
    const S_KeyMapDlg* d = D(pThis);
    for (int i = 0; i < d->m_nAccelSize; i++) {
        LPACCEL lpAccel = d->m_lpAccel + i;
        if (uiCmdID != static_cast<unsigned int>(lpAccel->cmd)) continue;
        if (lpAccel == nullptr) { ThrowInvalidArg(); return pRet; }
        struct { void* vfptr; LPACCEL m_lpAccel; } helper;   // CMFCAcceleratorKey (0x10 bytes)
        impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(&helper, lpAccel);
        CString strKey;
        impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(&helper, &strKey);
        if (!pRet->IsEmpty()) *pRet += L"; ";
        *pRet += strKey;
        impl___1CMFCAcceleratorKey__UEAA_XZ(&helper);
    }
    return pRet;
}

// Retail (RVA 0x75460, mfc140u), fully transcribed:
//     m_KeymapList.SetFocus();                               // 0x2a9b60
//     CopyKeyMap();                                          // vslot 101 (+0x328), tail jump
// The CopyKeyMap call is devirtualised to this file's thunk (deviation (1)).
// Symbol: ?OnCopy@CMFCKeyMapDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnCopy_CMFCKeyMapDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(D(pThis)->m_KeymapList));
    impl__CopyKeyMap_CMFCKeyMapDialog__MEAAXXZ(pThis);
}

// Retail (RVA 0x76130, mfc140u), fully transcribed:
//     CWinAppEx* pApp = DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp());   // m_pCurrentWinApp + IsKindOf 0x234cf0
//     if (pApp != NULL) {
//         CRect rectPosition;  GetWindowRect(rectPosition);          // ::GetWindowRect(m_hWnd)
//         CSettingsStoreSP regSP;
//         CSettingsStore& reg = regSP.Create(FALSE, FALSE);          // 0x12a550
//         if (reg.CreateKey(pApp->GetRegSectionPath(_T("KeyMapWindowPlacement"))))   // vslot 5 (+0x28); 0x1c7ad0
//             reg.Write(_T("KeyMapWindowRect"), rectPosition);       // vslot 13 (+0x68): Write(LPCTSTR, const CRect&)
//         // ~CSettingsStoreSP: if (m_pRegistry) delete m_pRegistry;   vslot 1, flag 1
//     }
//     CDialogEx::OnDestroy();                                        // 0x3f1e0
// The CSettingsStore virtuals are devirtualised to the exported thunks; the
// slot numbers follow afxsettingsstore.h with MSVC's reverse ordering of
// overloads (Write(const CRect&) is the 4th of 7 Write overloads -> slot 13).
// The CSettingsStoreSP is the two-word local that
// featurepack/menu/CMFCPopupMenuBar.cpp also uses, and its registry object is
// released the same way.
// Symbol: ?OnDestroy@CMFCKeyMapDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCKeyMapDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (CWinAppEx* pApp = AppEx()) {
        CRect rectPosition(0, 0, 0, 0);
        ::GetWindowRect(Wnd(pThis)->m_hWnd, AsRect(&rectPosition));
        void* regSP[2] = { nullptr, nullptr };                 // CSettingsStoreSP: m_pRegistry, m_dwUserData
        void* pReg = impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(regSP, FALSE, FALSE);
        if (pReg != nullptr) {
            CString strPath = RegPlacementPath(pApp);
            if (impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pReg, strPath.GetString())) {
                impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(pReg, kRegRect, &rectPosition);
            }
        }
        if (regSP[0] != nullptr) {
            delete static_cast<CObject*>(regSP[0]);            // ~CSettingsStoreSP
        }
    }
    impl__OnDestroy_CDialogEx__IEAAXXZ(static_cast<CDialogEx*>(pThis));
}

// Retail (RVA 0x749b0, mfc140u), transcribed:
//     CDialogEx::OnInitDialog();                                     // resolves to CDialog::OnInitDialog 0x208d10; result unused
//     if (AfxGetMainWnd() != NULL && (AfxGetMainWnd()->GetExStyle() & WS_EX_LAYOUTRTL))   // AfxGetThread()->GetMainWnd(), vslot 31
//         ModifyStyleEx(0, WS_EX_LAYOUTRTL);
//     SetIcon((HICON)LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(0x4263), RT_GROUP_ICON), MAKEINTRESOURCE(0x4263),
//             IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_SHARED), FALSE);   // WM_SETICON
//     m_ButtonPrint.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;  m_ButtonCopy.m_nFlatStyle = BUTTONSTYLE_FLAT;   // +0xe8
//     CString strTooltip;
//     if (m_bEnablePrint) {
//         m_ButtonPrint.SetImage(afxGlobalData.Is32BitIcons() ? IDB_AFXBARRES_PRINT32 : IDB_AFXBARRES_PRINT);   // SetImageInternal 0x1dd00 (id, 0, FALSE, 0)
//         m_ButtonPrint.GetWindowText(strTooltip);  m_ButtonPrint.SetWindowText(_T(""));
//         m_ButtonPrint.SetTooltip(strTooltip);                        // 0x1e700
//         m_ButtonPrint.SizeToContent();                               // vslot 94 (+0x2f0)
//         m_ButtonPrint.m_bDrawFocus = FALSE;                          // +0xfc
//     } else {
//         m_ButtonPrint.ShowWindow(SW_HIDE);
//     }
//     (the same five steps for m_ButtonCopy with IDB_AFXBARRES_COPY32 / _COPY, unconditionally)
//     OnSetColumns();  SetColumnsWidth();                              // vslots 106, 108
//     CDocManager* pDocManager = AfxGetApp()->m_pDocManager;           // +0xb8
//     if (pDocManager != NULL)
//         for (POSITION pos = pDocManager->GetFirstDocTemplatePosition(); pos != NULL;) {   // vslots 6, 7
//             CDocTemplate* pTemplate = pDocManager->GetNextDocTemplate(pos);
//             if (pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)) &&
//                 ((CMultiDocTemplate*)pTemplate)->m_hAccelTable != NULL) {             // +0xf8
//                 CString strName;  pTemplate->GetDocString(strName, CDocTemplate::fileNewName);   // vslot 27
//                 int iIndex = m_wndViewTypeList.AddString(strName);                     // CB_ADDSTRING
//                 m_wndViewTypeList.SetItemData(iIndex, (DWORD_PTR)pTemplate);            // CB_SETITEMDATA
//             }
//         }
//     CFrameWnd* pWndMain = DYNAMIC_DOWNCAST(CFrameWnd, m_pWndParentFrame);
//     if (pWndMain != NULL && pWndMain->m_hAccelTable != NULL) {
//         CString strDefaultMenu;  ENSURE(strDefaultMenu.LoadString(IDS_AFXBARRES_DEFAULT_VIEW));   // 0x3ef2
//         int iIndex = m_wndViewTypeList.AddString(strDefaultMenu);
//         m_wndViewTypeList.SetItemData(iIndex, NULL);  m_wndViewTypeList.SetCurSel(iIndex);
//         OnSelchangeViewType();
//     }
//     m_KeymapList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   // LVM_SETEXTENDEDLISTVIEWSTYLE, 0, 0x21
//     OnSelchangeViewType();
//     m_pDlgCust = new CMFCToolBarsCustomizeDialog(m_pWndParentFrame, TRUE, 0xf);   // 0xdb8 bytes, ctor 0x176b80
//     m_pDlgCust->EnableUserDefinedToolbars();                     // inline: m_pToolbarsPage(+0x208)->+0x708 = TRUE
//     m_pDlgCust->FillCategoriesComboBox(m_wndCategoryList, TRUE); // 0x179360
//     m_wndCategoryList.SetCurSel(0);  OnSelchangeCategory();
//     CWinAppEx* pApp = DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp());
//     if (pApp != NULL) {
//         CSettingsStoreSP regSP;  CSettingsStore& reg = regSP.Create(FALSE, TRUE);
//         CRect rectPosition(0, 0, 0, 0);
//         if (reg.Open(pApp->GetRegSectionPath(_T("KeyMapWindowPlacement"))) &&   // vslot 6 (+0x30)
//             reg.Read(_T("KeyMapWindowRect"), rectPosition)) {                    // vslot 20 (+0xa0): Read(LPCTSTR, CRect&)
//             CRect rectScreen(0, 0, 0, 0);  MONITORINFO mi = { sizeof(MONITORINFO) };
//             if (GetMonitorInfo(MonitorFromPoint(rectPosition.TopLeft(), MONITOR_DEFAULTTONEAREST), &mi))
//                 rectScreen = mi.rcWork;                                          // CopyRect
//             else
//                 SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
//             CRect rectDummy(0, 0, 0, 0);
//             if (rectDummy.IntersectRect(rectScreen, rectPosition))
//                 MoveWindow(rectPosition.left, rectPosition.top, rectPosition.Width(), rectPosition.Height(), TRUE);
//         }
//     }
//     return TRUE;
// DEVIATIONS (beyond the devirtualisations of deviation (1)):
//  * the document-template loop is not reproduced: its gate is
//    CMultiDocTemplate::m_hAccelTable != NULL, and OpenMFC's CMultiDocTemplate
//    has no accelerator table (deviation (2)), so no template can pass it;
//  * EnableUserDefinedToolbars, inline in retail, goes through the exported
//    thunk (the same store, with a NULL-page check);
//  * a failed `new` of the customize dialog is checked here; retail
//    dereferences the NULL result at +0x208.
// Symbol: ?OnInitDialog@CMFCKeyMapDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCKeyMapDialog__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_KeyMapDlg* d = D(pThis);
    CWnd* pWnd = Wnd(pThis);

    (void)impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));

    if (CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ()) {
        CWnd* pMainWnd = impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread);
        if (pMainWnd != nullptr && (impl__GetExStyle_CWnd__QEBAKXZ(pMainWnd) & WS_EX_LAYOUTRTL) != 0) {
            impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(pWnd, 0, WS_EX_LAYOUTRTL, 0);
        }
    }

    const int cyIcon = ::GetSystemMetrics(SM_CYSMICON);
    const int cxIcon = ::GetSystemMetrics(SM_CXSMICON);
    HINSTANCE hInstIcon = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(MAKEINTRESOURCEW(kIdiDialogIcon), RT_GROUP_ICON);
    HANDLE hIcon = ::LoadImageW(hInstIcon, MAKEINTRESOURCEW(kIdiDialogIcon), IMAGE_ICON, cxIcon, cyIcon, LR_SHARED);
    ::SendMessage(pWnd->m_hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));

    const int flat = kButtonStyleFlat;
    std::memcpy(d->m_ButtonPrint + kOffButtonFlatStyle, &flat, sizeof flat);
    std::memcpy(d->m_ButtonCopy + kOffButtonFlatStyle, &flat, sizeof flat);

    CString strTooltip;
    const int noFocus = FALSE;
    if (d->m_bEnablePrint != 0) {
        impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(d->m_ButtonPrint, GlobalIs32BitIcons() ? kIdbPrint32 : kIdbPrint, 0, FALSE, 0);
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(Wnd(d->m_ButtonPrint), &strTooltip);
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(Wnd(d->m_ButtonPrint), L"");
        impl__SetTooltip_CMFCButton__QEAAXPEB_W_Z(reinterpret_cast<CMFCButton*>(d->m_ButtonPrint), strTooltip.GetString());
        CSize size;
        impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(d->m_ButtonPrint, &size, FALSE);
        std::memcpy(d->m_ButtonPrint + kOffButtonDrawFocus, &noFocus, sizeof noFocus);
    } else {
        impl__ShowWindow_CWnd__QEAAHH_Z(Wnd(d->m_ButtonPrint), SW_HIDE);
    }

    impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(d->m_ButtonCopy, GlobalIs32BitIcons() ? kIdbCopy32 : kIdbCopy, 0, FALSE, 0);
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(Wnd(d->m_ButtonCopy), &strTooltip);
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(Wnd(d->m_ButtonCopy), L"");
    impl__SetTooltip_CMFCButton__QEAAXPEB_W_Z(reinterpret_cast<CMFCButton*>(d->m_ButtonCopy), strTooltip.GetString());
    {
        CSize size;
        impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(d->m_ButtonCopy, &size, FALSE);
    }
    std::memcpy(d->m_ButtonCopy + kOffButtonDrawFocus, &noFocus, sizeof noFocus);

    impl__OnSetColumns_CMFCKeyMapDialog__MEAAXXZ(pThis);
    impl__SetColumnsWidth_CMFCKeyMapDialog__MEAAXXZ(pThis);

    // (document-template loop: see DEVIATIONS above)

    const HWND hViewType = HwndOf(d->m_wndViewTypeList);
    CFrameWnd* pWndMain = d->m_pWndParentFrame;
    if (pWndMain != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWndMain, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ()) &&
        pWndMain->m_hAccelTable != nullptr) {
        CString strDefaultMenu;
        if (!EnsureLoadString(strDefaultMenu, kIdsDefaultView)) return TRUE;
        const int iIndex = static_cast<int>(::SendMessage(hViewType, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(strDefaultMenu.GetString())));
        ::SendMessage(hViewType, CB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0);
        ::SendMessage(hViewType, CB_SETCURSEL, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0);
        impl__OnSelchangeViewType_CMFCKeyMapDialog__IEAAXXZ(pThis);
    }

    ::SendMessage(ListHwnd(d), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    impl__OnSelchangeViewType_CMFCKeyMapDialog__IEAAXXZ(pThis);

    void* pDlgCust = impl___2_YAPEAX_K_Z(0xdb8);             // sizeof(CMFCToolBarsCustomizeDialog)
    if (pDlgCust != nullptr) {
        pDlgCust = impl___0CMFCToolBarsCustomizeDialog__QEAA_PEAVCFrameWnd__HIPEAV__CList_PEAUCRuntimeClass__PEAU1____Z(
            pDlgCust, d->m_pWndParentFrame, TRUE, 0xf, nullptr);
    }
    d->m_pDlgCust = pDlgCust;
    if (pDlgCust != nullptr) {
        impl__EnableUserDefinedToolbars_CMFCToolBarsCustomizeDialog__QEAAXH_Z(pDlgCust, TRUE);
        impl__FillCategoriesComboBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCComboBox__H_Z(pDlgCust, Wnd(d->m_wndCategoryList), TRUE);
    }
    ::SendMessage(HwndOf(d->m_wndCategoryList), CB_SETCURSEL, 0, 0);
    impl__OnSelchangeCategory_CMFCKeyMapDialog__IEAAXXZ(pThis);

    if (CWinAppEx* pApp = AppEx()) {
        void* regSP[2] = { nullptr, nullptr };                 // CSettingsStoreSP: m_pRegistry, m_dwUserData
        void* pReg = impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(regSP, FALSE, TRUE);
        CRect rectPosition(0, 0, 0, 0);
        bool bOK = false;
        if (pReg != nullptr) {
            CString strPath = RegPlacementPath(pApp);
            bOK = impl__Open_CSettingsStore__UEAAHPEB_W_Z(pReg, strPath.GetString()) &&
                  impl__Read_CSettingsStore__UEAAHPEB_WAEAVCRect___Z(pReg, kRegRect, &rectPosition);
        }
        if (bOK) {
            RECT rectScreen = { 0, 0, 0, 0 };
            MONITORINFO mi;
            std::memset(&mi, 0, sizeof mi);
            mi.cbSize = sizeof(MONITORINFO);
            POINT ptTopLeft = { rectPosition.left, rectPosition.top };
            if (::GetMonitorInfoW(::MonitorFromPoint(ptTopLeft, MONITOR_DEFAULTTONEAREST), &mi)) {
                ::CopyRect(&rectScreen, &mi.rcWork);
            } else {
                ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &rectScreen, 0);
            }
            RECT rectDummy = { 0, 0, 0, 0 };
            if (::IntersectRect(&rectDummy, &rectScreen, AsRect(&rectPosition))) {
                impl__MoveWindow_CWnd__QEAAXHHHHH_Z(pWnd, rectPosition.left, rectPosition.top,
                                                    rectPosition.right - rectPosition.left,
                                                    rectPosition.bottom - rectPosition.top, TRUE);
            }
        }
        if (regSP[0] != nullptr) {
            delete static_cast<CObject*>(regSP[0]);            // ~CSettingsStoreSP
        }
    }
    return TRUE;
}

// Retail (RVA 0x76520, mfc140u), fully transcribed:
//     CString strText = pButton->m_strTextCustom.IsEmpty() ? pButton->m_strText    // +0x38
//                                                          : pButton->m_strTextCustom;   // +0x40 (CloneData 0xdd40)
//     int iItem = m_KeymapList.InsertItem(LVIF_TEXT | LVIF_IMAGE, nItem, strText, 0, 0, -1, 0);   // 0x295400
//     m_KeymapList.SetItem(iItem, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)pButton);                 // 0x2954d0
//     CString strKeys = GetCommandKeys(pButton->m_nID);                                             // vslot 109 (+0x368)
//     m_KeymapList.SetItemText(iItem, 1, strKeys);                                                  // inline LVM_SETITEMTEXTW
//     CString strDescr;
//     CFrameWnd* pParent = GetParentFrame();                                                        // 0x28e200
//     if (pParent != NULL && pParent->GetSafeHwnd() != NULL)
//         pParent->GetMessageString(pButton->m_nID, strDescr);                                      // vslot 95 (+0x2f8)
//     m_KeymapList.SetItemText(iItem, 2, strDescr);
// GetCommandKeys and CFrameWnd::GetMessageString are devirtualised to the
// thunks (deviation (1)).
// Symbol: ?OnInsertItem@CMFCKeyMapDialog@@MEAAXPEAVCMFCToolBarButton@@H@Z
extern "C" void MS_ABI impl__OnInsertItem_CMFCKeyMapDialog__MEAAXPEAVCMFCToolBarButton__H_Z(
    void* pThis, CMFCToolBarButton* pButton, int nItem) {
    if (pThis == nullptr || pButton == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    CString strText = pButton->m_strTextCustom.IsEmpty() ? pButton->m_strText : pButton->m_strTextCustom;
    const int iItem = impl__InsertItem_CListCtrl__QEAAHIHPEB_WIIH_J_Z(List(d), LVIF_TEXT | LVIF_IMAGE, nItem, strText.GetString(), 0, 0, -1, 0);
    impl__SetItem_CListCtrl__QEAAHHHIPEB_WHII_J_Z(List(d), iItem, 0, LVIF_PARAM, nullptr, 0, 0, 0, reinterpret_cast<LPARAM>(pButton));

    {
        alignas(CString) unsigned char storage[sizeof(CString)];
        CString* pKeys = impl__GetCommandKeys_CMFCKeyMapDialog__MEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(
            pThis, reinterpret_cast<CString*>(storage), pButton->m_nID);
        ListSetItemText(d, iItem, 1, pKeys->GetString());
        pKeys->~CString();
    }

    CString strDescr;
    CFrameWnd* pParent = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(Wnd(pThis));
    if (pParent != nullptr && pParent->m_hWnd != nullptr) {
        impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pParent, pButton->m_nID, &strDescr);
    }
    ListSetItemText(d, iItem, 2, strDescr.GetString());
}

// Retail (RVA 0x755c0, mfc140u), fully transcribed:
//     NMHEADER* pNMHeader = (NMHEADER*)lParam;
//     if (pNMHeader != NULL && pNMHeader->hdr.code == HDN_ITEMCLICKW /*-322*/ && pNMHeader->iButton == 0) {   // +0x10, +0x1c
//         int nColumn = pNMHeader->iItem;                                          // +0x18
//         m_bSortAscending = (nColumn == m_nSortedCol) ? !m_bSortAscending : TRUE;
//         m_nSortedCol = nColumn;
//         m_KeymapList.SortItems(listCompareFunc /*0x74590*/, (DWORD_PTR)this);   // LVM_SORTITEMS
//         m_KeymapList.SetSortColumn(m_nSortedCol, m_bSortAscending);              // 0x77ca0, bAdd FALSE
//     }
//     return CDialogEx::OnNotify(wParam, lParam, pResult);                          // tail jump to CWnd::OnNotify 0x28e150
// Symbol: ?OnNotify@CMFCKeyMapDialog@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CMFCKeyMapDialog__MEAAH_K_JPEA_J_Z(
    void* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (pThis == nullptr) return FALSE;
    S_KeyMapDlg* d = D(pThis);
    const NMHEADERW* pNMHeader = reinterpret_cast<const NMHEADERW*>(lParam);
    if (pNMHeader != nullptr && pNMHeader->hdr.code == static_cast<UINT>(HDN_ITEMCLICKW) && pNMHeader->iButton == 0) {
        const int nColumn = pNMHeader->iItem;
        d->m_bSortAscending = (nColumn == d->m_nSortedCol) ? (d->m_bSortAscending == 0) : TRUE;
        d->m_nSortedCol = nColumn;
        SortKeyList(d);
        impl__SetSortColumn_CMFCListCtrl__QEAAXHHH_Z(d->m_KeymapList, d->m_nSortedCol, d->m_bSortAscending, FALSE);
    }
    return impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(Wnd(pThis), wParam, lParam, pResult);
}

// Retail (RVA 0x75490, mfc140u), fully transcribed:
//     m_KeymapList.SetFocus();                               // 0x2a9b60
//     PrintKeyMap();                                         // vslot 102 (+0x330), tail jump
// The PrintKeyMap call is devirtualised to this file's thunk (deviation (1)).
// Symbol: ?OnPrint@CMFCKeyMapDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPrint_CMFCKeyMapDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(D(pThis)->m_KeymapList));
    impl__PrintKeyMap_CMFCKeyMapDialog__MEAAXXZ(pThis);
}

// Retail (RVA 0x75b00, mfc140u), fully transcribed:
//     TEXTMETRIC tm;  dc.GetTextMetrics(&tm);                          // ::GetTextMetricsW(dc.m_hAttribDC)
//     int nYMargin = tm.tmHeight * 2;
//     CString strAppName = AfxGetApp()->m_pszAppName == NULL ? _T("") : AfxGetApp()->m_pszAppName;   // app +0xa0
//     CString strTitle;  GetWindowText(strTitle);                      // 0x28be00
//     CString strCaption;
//     strCaption.Format(_T("- %d -\r\n%Ts: %Ts"), nPage, (LPCTSTR)strAppName, (LPCTSTR)strTitle);   // format at 0x18033f7a0
//     CRect rectText(0, nYMargin, cx, 32767);
//     int y = dc.DrawText(strCaption, rectText, DT_WORDBREAK | DT_CENTER);   // vslot 28 (+0xe0), length passed explicitly
//     return y + nYMargin * 2;
// DEVIATIONS: DrawText per deviation (4); the format is spelled with %ls
// (see FormatItem); retail dereferences AfxGetApp() unchecked, a NULL app
// yields an empty name here.
// Symbol: ?OnPrintHeader@CMFCKeyMapDialog@@MEBAHAEAVCDC@@HH@Z
extern "C" int MS_ABI impl__OnPrintHeader_CMFCKeyMapDialog__MEBAHAEAVCDC__HH_Z(
    const void* pThis, CDC* pDC, int nPage, int cx) {
    if (pThis == nullptr || pDC == nullptr) return 0;
    TEXTMETRICW tm;
    std::memset(&tm, 0, sizeof tm);
    ::GetTextMetricsW(DC(pDC)->m_hAttribDC, &tm);
    const int nYMargin = tm.tmHeight * 2;

    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    CString strAppName((pApp != nullptr && pApp->m_pszAppName != nullptr) ? pApp->m_pszAppName : L"");
    CString strTitle;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(Wnd(pThis), &strTitle);
    CString strCaption;
    strCaption.Format(L"- %d -\r\n%ls: %ls", nPage, strAppName.GetString(), strTitle.GetString());

    RECT rectText = { 0, nYMargin, cx, 32767 };
    const int y = DcDrawText(pDC, strCaption, &rectText, DT_WORDBREAK | DT_CENTER);
    return y + nYMargin * 2;
}

// Retail (RVA 0x75cc0, mfc140u), fully transcribed:
//     TEXTMETRIC tm;  dc.GetTextMetrics(&tm);                          // m_hAttribDC
//     int nXMargin = tm.tmMaxCharWidth * 2;                            // +0x18
//     CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_KeymapList.GetItemData(nItem);   // 0x295790
//     CString strName = pButton->m_strText;                            // +0x38 (not m_strTextCustom)
//     CString strKeys = m_KeymapList.GetItemText(nItem, 1);
//     CString strDescr = m_KeymapList.GetItemText(nItem, 2);
//     CSize szKeys = dc.GetTextExtent(CString(_T("Ctrl+Shift+W")));    // GetTextExtentPoint32W(m_hAttribDC)
//     int nKeyColumnWidth = szKeys.cx + nXMargin;
//     int nNameColumnWidth = (cx - nXMargin * 2 - nKeyColumnWidth) / 3 + nXMargin;   // imul 0x55555556
//     int nHeight = 0;
//     for (int i = 0; i < (bCalcHeight ? 1 : 2); i++) {
//         UINT uiFormat = i == 0 ? DT_CALCRECT | DT_WORDBREAK : DT_WORDBREAK | DT_END_ELLIPSIS;   // 0x410 / 0x8010
//         CRect rectName(nXMargin, y, nNameColumnWidth, y + 32676);
//         int nNameHeight = dc.DrawText(strName, rectName, uiFormat);
//         CRect rectKeys(rectName.right + nXMargin, y, rectName.right + nXMargin + nKeyColumnWidth, y + 32676);
//         int nKeysHeight = dc.DrawText(strKeys, rectKeys, uiFormat);
//         CRect rectDescr(rectKeys.right + nXMargin, y, cx, 32676);     // bottom is 32676, not y + 32676
//         int nDescrHeight = dc.DrawText(strDescr, rectDescr, uiFormat);
//         nHeight = max(nNameHeight, nKeysHeight, nDescrHeight);        // cmovg chain
//     }
//     return nHeight;
// rectKeys / rectDescr are placed from rectName.right / rectKeys.right AFTER
// the preceding DrawText, which DT_CALCRECT narrows on the first pass.
// DEVIATION: DrawText per deviation (4); pButton is checked for NULL (retail
// dereferences it unchecked).
// Symbol: ?OnPrintItem@CMFCKeyMapDialog@@MEBAHAEAVCDC@@HHHH@Z
extern "C" int MS_ABI impl__OnPrintItem_CMFCKeyMapDialog__MEBAHAEAVCDC__HHHH_Z(
    const void* pThis, CDC* pDC, int nItem, int y, int cx, int bCalcHeight) {
    if (pThis == nullptr || pDC == nullptr) return 0;
    const S_KeyMapDlg* d = D(pThis);
    TEXTMETRICW tm;
    std::memset(&tm, 0, sizeof tm);
    ::GetTextMetricsW(DC(pDC)->m_hAttribDC, &tm);
    const int nXMargin = tm.tmMaxCharWidth * 2;

    const CMFCToolBarButton* pButton = reinterpret_cast<const CMFCToolBarButton*>(impl__GetItemData_CListCtrl__QEBA_KH_Z(List(d), nItem));
    if (pButton == nullptr) return 0;
    CString strName = pButton->m_strText;
    CString strKeys = ListItemText(d, nItem, 1);
    CString strDescr = ListItemText(d, nItem, 2);

    SIZE szKeys = { 0, 0 };
    ::GetTextExtentPoint32W(DC(pDC)->m_hAttribDC, kLongestKeys, static_cast<int>(std::wcslen(kLongestKeys)), &szKeys);
    const int nKeyColumnWidth = szKeys.cx + nXMargin;
    const int nNameColumnWidth = (cx - nXMargin * 2 - nKeyColumnWidth) / 3 + nXMargin;

    int nHeight = 0;
    const int nPasses = bCalcHeight ? 1 : 2;
    for (int i = 0; i < nPasses; i++) {
        const UINT uiFormat = (i == 0) ? (DT_CALCRECT | DT_WORDBREAK) : (DT_WORDBREAK | DT_END_ELLIPSIS);

        RECT rectName = { nXMargin, y, nNameColumnWidth, y + 32676 };
        const int nNameHeight = DcDrawText(pDC, strName, &rectName, uiFormat);

        RECT rectKeys = { rectName.right + nXMargin, y, rectName.right + nXMargin + nKeyColumnWidth, y + 32676 };
        const int nKeysHeight = DcDrawText(pDC, strKeys, &rectKeys, uiFormat);

        RECT rectDescr = { rectKeys.right + nXMargin, y, cx, 32676 };
        const int nDescrHeight = DcDrawText(pDC, strDescr, &rectDescr, uiFormat);

        nHeight = nDescrHeight;
        if (nKeysHeight > nHeight) nHeight = nKeysHeight;
        if (nNameHeight > nHeight) nHeight = nNameHeight;
    }
    return nHeight;
}

// Retail (RVA 0x75370, mfc140u), fully transcribed:
//     UpdateData();                                          // 0x2910d0, TRUE
//     ENSURE(m_lpAccel != NULL);
//     int iIndex = m_wndCategoryList.GetCurSel();            // CB_GETCURSEL on +0x4d8
//     if (iIndex == CB_ERR) return;
//     CObList* pCategoryButtonsList = (CObList*)m_wndCategoryList.GetItemData(iIndex);   // CB_GETITEMDATA
//     m_KeymapList.DeleteAllItems();                         // LVM_DELETEALLITEMS (0x1009)
//     int nItem = 0;
//     for (POSITION pos = pCategoryButtonsList->GetHeadPosition(); pos != NULL;) {   // m_pNodeHead +0x08
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)pCategoryButtonsList->GetNext(pos);   // node data +0x10
//         ENSURE(pButton != NULL);
//         if (pButton->m_nID > 0 && pButton->m_nID != (UINT)-1)   // `dec ; cmp $0xfffffffd ; ja`
//             OnInsertItem(pButton, nItem++);                // vslot 107 (+0x358)
//     }
//     m_KeymapList.SortItems(listCompareFunc, (LPARAM)this);
// The list walk is deviation (3); OnInsertItem is devirtualised (deviation (1)).
// Symbol: ?OnSelchangeCategory@CMFCKeyMapDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCategory_CMFCKeyMapDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), TRUE);
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }

    const HWND hCategory = HwndOf(d->m_wndCategoryList);
    const int iIndex = static_cast<int>(::SendMessage(hCategory, CB_GETCURSEL, 0, 0));
    if (iIndex == CB_ERR) return;
    const CObList* pCategoryButtonsList = reinterpret_cast<const CObList*>(
        ::SendMessage(hCategory, CB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0));

    ::SendMessage(ListHwnd(d), LVM_DELETEALLITEMS, 0, 0);

    int nItem = 0;
    if (const ObListData* items = CategoryButtons(pCategoryButtonsList)) {
        ObListData::POSITION pos = items->GetHeadPosition();
        while (pos != ObListData::POSITION(nullptr)) {
            CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(items->GetNext(pos));
            if (pButton == nullptr) { ThrowInvalidArg(); return; }
            if (pButton->m_nID != 0 && pButton->m_nID != static_cast<UINT>(-1)) {
                impl__OnInsertItem_CMFCKeyMapDialog__MEAAXPEAVCMFCToolBarButton__H_Z(pThis, pButton, nItem++);
            }
        }
    }

    SortKeyList(d);
}

// Retail (RVA 0x75110, mfc140u), fully transcribed:
//     m_hAccelTable = NULL;
//     if (m_lpAccel != NULL) { free(m_lpAccel);  m_lpAccel = NULL; }   // import slot 0x1802c74e8 = ucrt free
//     int iIndex = m_wndViewTypeList.GetCurSel();                       // CB_GETCURSEL on +0x6a8
//     if (iIndex == CB_ERR) { m_wndViewIcon.SetIcon(NULL); return; }   // STM_SETICON (0x170) on +0x5c0
//     HICON hicon = NULL;
//     CMultiDocTemplate* pTemplate = (CMultiDocTemplate*)m_wndViewTypeList.GetItemData(iIndex);   // CB_GETITEMDATA
//     if (pTemplate != NULL) {
//         hicon = AfxGetApp()->LoadIcon(pTemplate->m_nIDResource);     // AfxFindResourceHandle(id, RT_GROUP_ICON) + ::LoadIconW; id at +0x98
//         m_hAccelTable = pTemplate->m_hAccelTable;                    // +0xf8
//     } else {
//         CFrameWnd* pWndMain = DYNAMIC_DOWNCAST(CFrameWnd, m_pWndParentFrame);
//         if (pWndMain != NULL) {
//             hicon = (HICON)GetClassLongPtr(pWndMain->m_hWnd, GCLP_HICON);   // -14
//             m_hAccelTable = pWndMain->m_hAccelTable;                        // +0xf8
//         }
//     }
//     if (hicon == NULL) hicon = ::LoadIcon(NULL, IDI_APPLICATION);
//     m_wndViewIcon.SetIcon(hicon);
//     ENSURE(m_hAccelTable != NULL);
//     m_nAccelSize = ::CopyAcceleratorTable(m_hAccelTable, NULL, 0);
//     m_lpAccel = new ACCEL[m_nAccelSize];                             // ??2@ (0x27f0); 6 * n, saturated on overflow
//     ENSURE(m_lpAccel != NULL);
//     ::CopyAcceleratorTable(m_hAccelTable, m_lpAccel, m_nAccelSize);
//     OnSelchangeCategory();                                           // inlined in retail (0x75296..0x75356)
// DEVIATIONS: the frame's m_hAccelTable is OpenMFC's member (deviation (2)).
// OpenMFC's CMultiDocTemplate has no accelerator table, so a template entry
// leaves m_hAccelTable NULL and the ENSURE throws; OnInitDialog never adds
// such an entry (see there).  The template icon uses OpenMFC's
// CDocTemplate::m_nIDResource member.  m_lpAccel is released with ??3@ (see
// the destructor).  The inlined OnSelchangeCategory is the thunk.
// Symbol: ?OnSelchangeViewType@CMFCKeyMapDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeViewType_CMFCKeyMapDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    d->m_hAccelTable = nullptr;
    if (d->m_lpAccel != nullptr) {
        impl___3_YAXPEAX_Z(d->m_lpAccel);
        d->m_lpAccel = nullptr;
    }

    const HWND hViewType = HwndOf(d->m_wndViewTypeList);
    const HWND hViewIcon = HwndOf(d->m_wndViewIcon);
    const int iIndex = static_cast<int>(::SendMessage(hViewType, CB_GETCURSEL, 0, 0));
    if (iIndex == CB_ERR) {
        ::SendMessage(hViewIcon, STM_SETICON, 0, 0);
        return;
    }

    HICON hicon = nullptr;
    CDocTemplate* pTemplate = reinterpret_cast<CDocTemplate*>(
        ::SendMessage(hViewType, CB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0));
    if (pTemplate != nullptr) {
        LPCWSTR lpszIcon = MAKEINTRESOURCEW(pTemplate->m_nIDResource);
        hicon = ::LoadIconW(impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszIcon, RT_GROUP_ICON), lpszIcon);
        d->m_hAccelTable = nullptr;         // deviation (2): OpenMFC's CMultiDocTemplate has no m_hAccelTable
    } else {
        CFrameWnd* pWndMain = d->m_pWndParentFrame;
        if (pWndMain != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWndMain, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            hicon = reinterpret_cast<HICON>(::GetClassLongPtrW(pWndMain->m_hWnd, GCLP_HICON));
            d->m_hAccelTable = pWndMain->m_hAccelTable;
        }
    }
    if (hicon == nullptr) hicon = ::LoadIconW(nullptr, IDI_APPLICATION);
    ::SendMessage(hViewIcon, STM_SETICON, reinterpret_cast<WPARAM>(hicon), 0);

    if (d->m_hAccelTable == nullptr) { ThrowInvalidArg(); return; }
    d->m_nAccelSize = ::CopyAcceleratorTableW(d->m_hAccelTable, nullptr, 0);
    const unsigned long long cb = static_cast<unsigned long long>(static_cast<long long>(d->m_nAccelSize)) * sizeof(ACCEL);
    d->m_lpAccel = static_cast<LPACCEL>(impl___2_YAPEAX_K_Z(cb / sizeof(ACCEL) == static_cast<unsigned long long>(static_cast<long long>(d->m_nAccelSize))
                                                               ? static_cast<std::size_t>(cb) : static_cast<std::size_t>(-1)));
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }
    ::CopyAcceleratorTableW(d->m_hAccelTable, d->m_lpAccel, d->m_nAccelSize);

    impl__OnSelchangeCategory_CMFCKeyMapDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x763d0, mfc140u), fully transcribed:
//     CString strColumn;
//     ENSURE(strColumn.LoadString(IDS_AFXBARRES_COMMAND));      // 0x3e9d
//     m_KeymapList.InsertColumn(0, strColumn);                   // 0x295390 (LVCFMT_LEFT, -1, -1)
//     ENSURE(strColumn.LoadString(IDS_AFXBARRES_KEYS));         // 0x3e9e
//     m_KeymapList.InsertColumn(1, strColumn);
//     ENSURE(strColumn.LoadString(IDS_AFXBARRES_DESCRIPTION));  // 0x3e9f
//     m_KeymapList.InsertColumn(2, strColumn);
// Symbol: ?OnSetColumns@CMFCKeyMapDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnSetColumns_CMFCKeyMapDialog__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    CString strColumn;
    if (!EnsureLoadString(strColumn, kIdsCommand)) return;
    impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(List(d), 0, strColumn.GetString(), LVCFMT_LEFT, -1, -1);
    if (!EnsureLoadString(strColumn, kIdsKeys)) return;
    impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(List(d), 1, strColumn.GetString(), LVCFMT_LEFT, -1, -1);
    if (!EnsureLoadString(strColumn, kIdsDescription)) return;
    impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(List(d), 2, strColumn.GetString(), LVCFMT_LEFT, -1, -1);
}

// Retail (RVA 0x754c0, mfc140u), fully transcribed:
//     CDialogEx::OnSize(nType, cx, cy);                     // resolves to CWnd::Default() 0x28ac80
//     if (m_KeymapList.GetSafeHwnd() != NULL) {
//         CRect rectList;  m_KeymapList.GetClientRect(rectList);
//         m_KeymapList.MapWindowPoints(this, rectList);     // ::MapWindowPoints(list, m_hWnd, rect, 2)
//         CRect rectClient;  GetClientRect(rectClient);
//         rectList.right = rectClient.right;  rectList.bottom = rectClient.bottom;
//         m_KeymapList.SetWindowPos(NULL, -1, -1, rectList.Width(), rectList.Height(),
//                                   SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);   // 0x16, 0x2a9a60
//         SetColumnsWidth();                                // vslot 108 (+0x360)
//     }
// SetColumnsWidth is devirtualised (deviation (1)).
// Symbol: ?OnSize@CMFCKeyMapDialog@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCKeyMapDialog__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    (void)nType;
    (void)cx;
    (void)cy;
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    (void)impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
    const HWND hList = ListHwnd(d);
    if (hList == nullptr) return;

    RECT rectList = { 0, 0, 0, 0 };
    ::GetClientRect(hList, &rectList);
    ::MapWindowPoints(hList, Wnd(pThis)->m_hWnd, reinterpret_cast<POINT*>(&rectList), 2);
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Wnd(pThis)->m_hWnd, &rectClient);
    rectList.right = rectClient.right;
    rectList.bottom = rectClient.bottom;
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(Wnd(d->m_KeymapList), nullptr, -1, -1,
                                               rectList.right - rectList.left, rectList.bottom - rectList.top,
                                               SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
    impl__SetColumnsWidth_CMFCKeyMapDialog__MEAAXXZ(pThis);
}

// Retail (RVA 0x75880, mfc140u), fully transcribed:
//     CWaitCursor wait;                                     // AfxGetApp()->BeginWaitCursor() 0x1de7b0 / End 0x1de7e0
//     int nItemsSelected = m_KeymapList.GetSelectedCount(); // LVM_GETSELECTEDCOUNT, before the dialog
//     CPrintDialog dlgPrint(FALSE, PD_ALLPAGES | PD_RETURNDC | PD_NOSELECTION /*0x104*/, NULL);   // 0x218e40
//     if (dlgPrint.DoModal() == IDOK && dlgPrint.m_pd.hDC != NULL) {                           // 0x219080
//         CDC dc;  dc.Attach(dlgPrint.m_pd.hDC);            // 0x2a2480
//         int cyPage = GetDeviceCaps(dc.m_hAttribDC, VERTRES);
//         int cxPage = GetDeviceCaps(dc.m_hAttribDC, HORZRES);
//         dc.StartDoc(_T("AfxKeyMapDlg"));                  // 0x2a25b0: memset DOCINFO, cbSize 0x28, ::StartDocW(m_hDC)
//         ::StartPage(dc.m_hDC);
//         int nPage = 1;
//         int y = OnPrintHeader(dc, nPage, cxPage);         // vslot 103
//         UINT nFlags = nItemsSelected != 0 ? LVNI_SELECTED : LVNI_ALL;
//         for (int nItem = m_KeymapList.GetNextItem(-1, nFlags); nItem >= 0;
//              nItem = m_KeymapList.GetNextItem(nItem, nFlags)) {
//             int cyItem = OnPrintItem(dc, nItem, y, cxPage, TRUE);   // vslot 104
//             if (y + cyItem > cyPage) {
//                 ::EndPage(dc.m_hDC);  ::StartPage(dc.m_hDC);
//                 y = OnPrintHeader(dc, ++nPage, cxPage);
//             }
//             y += OnPrintItem(dc, nItem, y, cxPage, FALSE);
//         }
//         ::EndPage(dc.m_hDC);  ::EndDoc(dc.m_hDC);
//         // ~CDC: if (m_hDC != NULL) ::DeleteDC(Detach());
//     }
//     // ~CPrintDialog inline (vftable store, then CDialog::~CDialog 0x207eb0); ~CWaitCursor (0x7687c)
// DEVIATIONS: OpenMFC's CPrintDialog has its own layout (no m_pd reference);
// the DC DoModal returns is its m_hDC (core/dialog/CPrintDialog.cpp), read
// through PrintDialogAccess.  Because OpenMFC's ~CPrintDialog also DeleteDCs
// m_hDC, the member is cleared after the CDC-equivalent DeleteDC so the DC is
// deleted exactly once, as in retail.  The dialog object is built in local
// storage through the exported ctor thunk and destroyed through its virtual
// destructor.  The stack CDC is the retail-layout DCView (deviation (4)); the
// virtual OnPrintHeader / OnPrintItem calls are devirtualised (deviation (1)).
// Symbol: ?PrintKeyMap@CMFCKeyMapDialog@@MEAAXXZ
extern "C" void MS_ABI impl__PrintKeyMap_CMFCKeyMapDialog__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    CWinApp* pWaitApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pWaitApp != nullptr) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pWaitApp);

    const HWND hList = ListHwnd(d);
    const UINT nItemsSelected = static_cast<UINT>(::SendMessage(hList, LVM_GETSELECTEDCOUNT, 0, 0));

    alignas(CPrintDialog) unsigned char dlgStorage[sizeof(CPrintDialog)];
    CPrintDialog* pDlgPrint = impl___0CPrintDialog__QEAA_HKPEAVCWnd___Z(
        reinterpret_cast<CPrintDialog*>(dlgStorage), FALSE, PD_ALLPAGES | PD_RETURNDC | PD_NOSELECTION, nullptr);
    if (pDlgPrint != nullptr) {
        PrintDialogAccess* pAccess = static_cast<PrintDialogAccess*>(pDlgPrint);
        if (impl__DoModal_CPrintDialog__UEAA_JXZ(pDlgPrint) == IDOK && pAccess->m_hDC != nullptr) {
            DCView dc = { nullptr, static_cast<HDC>(pAccess->m_hDC), static_cast<HDC>(pAccess->m_hDC), FALSE };
            CDC* pDC = reinterpret_cast<CDC*>(&dc);

            const int cyPage = ::GetDeviceCaps(dc.m_hAttribDC, VERTRES);
            const int cxPage = ::GetDeviceCaps(dc.m_hAttribDC, HORZRES);

            DOCINFOW di;
            std::memset(&di, 0, sizeof di);
            di.cbSize = sizeof(DOCINFOW);
            di.lpszDocName = kPrintDocName;
            ::StartDocW(dc.m_hDC, &di);
            ::StartPage(dc.m_hDC);

            int nPage = 1;
            int y = impl__OnPrintHeader_CMFCKeyMapDialog__MEBAHAEAVCDC__HH_Z(pThis, pDC, nPage, cxPage);
            const UINT nFlags = nItemsSelected != 0 ? LVNI_SELECTED : LVNI_ALL;
            for (int nItem = static_cast<int>(::SendMessage(hList, LVM_GETNEXTITEM, static_cast<WPARAM>(-1), nFlags));
                 nItem >= 0;
                 nItem = static_cast<int>(::SendMessage(hList, LVM_GETNEXTITEM, static_cast<WPARAM>(static_cast<INT_PTR>(nItem)), nFlags))) {
                const int cyItem = impl__OnPrintItem_CMFCKeyMapDialog__MEBAHAEAVCDC__HHHH_Z(pThis, pDC, nItem, y, cxPage, TRUE);
                if (y + cyItem > cyPage) {
                    ::EndPage(dc.m_hDC);
                    ::StartPage(dc.m_hDC);
                    nPage++;
                    y = impl__OnPrintHeader_CMFCKeyMapDialog__MEBAHAEAVCDC__HH_Z(pThis, pDC, nPage, cxPage);
                }
                y += impl__OnPrintItem_CMFCKeyMapDialog__MEBAHAEAVCDC__HHHH_Z(pThis, pDC, nItem, y, cxPage, FALSE);
            }

            ::EndPage(dc.m_hDC);
            ::EndDoc(dc.m_hDC);
            ::DeleteDC(dc.m_hDC);               // ~CDC
            pAccess->m_hDC = nullptr;           // see DEVIATIONS: not deleted a second time by ~CPrintDialog
        }
        pDlgPrint->~CPrintDialog();
    }

    if (pWaitApp != nullptr) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pWaitApp);
}

// Retail (RVA 0x75fb0, mfc140u), fully transcribed:
//     CRect rectList;  m_KeymapList.GetClientRect(rectList);
//     CClientDC dc(this);                                                // 0x2a3b20
//     CFont* pOldFont = dc.SelectObject(m_KeymapList.GetFont());         // WM_GETFONT, CGdiObject::FromHandle, 0x2a2730
//     int nKeysColumnWidth = dc.GetTextExtent(CString(_T("Ctrl+Shift+W"))).cx + 10;   // GetTextExtentPoint32W(m_hAttribDC)
//     dc.SelectObject(pOldFont);
//     int nRestOfWidth = rectList.Width() - nKeysColumnWidth - ::GetSystemMetrics(SM_CXHSCROLL);   // 0x15
//     m_KeymapList.SetColumnWidth(0, nRestOfWidth / 3);                  // LVM_SETCOLUMNWIDTH (0x101e), MAKELPARAM(w, 0)
//     m_KeymapList.SetColumnWidth(1, nKeysColumnWidth);
//     m_KeymapList.SetColumnWidth(2, nRestOfWidth * 2 / 3);
//     // ~CClientDC (0x2a3be0)
// DEVIATION: the CClientDC and the CFont / CGdiObject temporaries are the
// Win32 calls they wrap (::GetDC / ::SelectObject / ::ReleaseDC on m_hWnd);
// a failed ::GetDC throws a resource exception, as CClientDC's constructor
// does.
// Symbol: ?SetColumnsWidth@CMFCKeyMapDialog@@MEAAXXZ
extern "C" void MS_ABI impl__SetColumnsWidth_CMFCKeyMapDialog__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KeyMapDlg* d = D(pThis);
    const HWND hList = ListHwnd(d);
    RECT rectList = { 0, 0, 0, 0 };
    ::GetClientRect(hList, &rectList);

    const HWND hWnd = Wnd(pThis)->m_hWnd;
    HDC hdc = ::GetDC(hWnd);
    if (hdc == nullptr) {
        impl__AfxThrowResourceException__YAXXZ();
        return;
    }
    HGDIOBJ hOldFont = ::SelectObject(hdc, reinterpret_cast<HGDIOBJ>(::SendMessage(hList, WM_GETFONT, 0, 0)));
    SIZE sz = { 0, 0 };
    ::GetTextExtentPoint32W(hdc, kLongestKeys, static_cast<int>(std::wcslen(kLongestKeys)), &sz);
    const int nKeysColumnWidth = sz.cx + 10;
    ::SelectObject(hdc, hOldFont);

    const int nRestOfWidth = (rectList.right - rectList.left) - nKeysColumnWidth - ::GetSystemMetrics(SM_CXHSCROLL);
    ::SendMessage(hList, LVM_SETCOLUMNWIDTH, 0, MAKELPARAM(nRestOfWidth / 3, 0));
    ::SendMessage(hList, LVM_SETCOLUMNWIDTH, 1, MAKELPARAM(nKeysColumnWidth, 0));
    ::SendMessage(hList, LVM_SETCOLUMNWIDTH, 2, MAKELPARAM(nRestOfWidth * 2 / 3, 0));

    ::ReleaseDC(hWnd, hdc);
}
