// CMFCToolBarsKeyboardPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
// FindListStorage: the header-only reader of the side table in which
// OpenMFC's CObList keeps its nodes (see CategoryButtons below).
#include "detail/FilecoreSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <new>

// ===========================================================================
// CMFCToolBarsKeyboardPropertyPage -- the "Keyboard" page of the feature-pack
// Customize sheet (CPropertyPage).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  All RVAs
// and absolute addresses in this file are mfc140u.  Only the ctor, dtor,
// CreateObject, DoDataExchange, OnAssign and AddKeyEntry have entries in
// mfc140u_rva_symbols.json; every export's RVA below was resolved from its
// ordinal (mfc_complete_ordinal_mapping.json) through mfc140u.dll's export
// address table, and cross-checked against the class vftable 0x180319598
// (installed by the ctor at 0x179b90): slot 0 -> GetRuntimeClass 0x179b80,
// slot 66 (+0x210) -> DoDataExchange 0x179f00, slot 96 (+0x300) ->
// OnInitDialog 0x17a010.
//
//   ctor 0x179b90   dtor 0x179dd0   CreateObject 0x179b50   DoDataExchange 0x179f00
//   OnInitDialog 0x17a010   OnAssign 0x17a370   OnSelchangeCategory 0x17a5d0
//   OnSelchangeCommandsList 0x17a8e0   OnSelchangeCurrentKeysList 0x17aa20
//   OnRemove 0x17aaa0   OnResetAll 0x17aca0   OnSelchangeViewType 0x17ad90
//   AddKeyEntry 0x17af40   OnUpdateNewShortcutKey 0x17b010   SetAllCategory 0x17b190
//
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives.  S_KbdPage pins it: the ctor (0x179b90) builds
// CPropertyPage at +0, then the members at the offsets below; CreateObject
// (0x179b50) and the scalar deleting destructor (0x179d80) both use 0x9f8 as
// the object size, and the harvested descriptor in
// featurepack/customize/RuntimeClasses.cpp agrees (2552).  The member names
// are those of afxtoolbarskeyboardpropertypage.h (14.51 SDK on this host),
// whose declaration order matches the offsets.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftable 0x180319598 at +0x00 and
//      the CStatic / CComboBox / CButton / CListBox vftables in the plain
//      control members after ??0CWnd@@QEAA@XZ.  OpenMFC has no MSVC-layout
//      vtable for this class: +0x00 keeps what the CPropertyPage ctor thunk
//      installs and the plain controls are built with the exported CWnd
//      constructor alone, as featurepack/customize/CMFCKeyMapDialog.cpp does.
//      So a virtual OnInitDialog / DoDataExchange call through this object's
//      vtable would reach CPropertyPage's entries, not the thunks below; the
//      retail bodies' own calls on `this` are made to this file's thunks.
//
//  (2) m_wndNewKey (CMFCAcceleratorKeyAssignCtrl).  Retail calls its exported
//      ctor (0x3440).  OpenMFC's ctor / dtor thunks for that class
//      (featurepack/customize/CMFCAcceleratorKeyAssignCtrl.cpp) are still
//      empty placeholders, so the member is built here the way the retail
//      0x3440 body builds it (CWnd ctor, m_Helper = CMFCAcceleratorKey(&m_Accel),
//      m_bIsDefined = m_bIsFocused = 0, ResetKey()) and torn down the way the
//      retail dtor 0x179dd0 inlines its destructor (m_Helper vftable reset, then
//      ??1CEdit).  See headerRequests.
//
//  (3) Frame / template accelerator tables.  Retail reads CFrameWnd::
//      m_hAccelTable and CMultiDocTemplate::m_hAccelTable at +0xf8.  The frame's
//      table is read through OpenMFC's own CFrameWnd::m_hAccelTable C++ member,
//      as CMFCKeyMapDialog.cpp and CMFCToolBarsCustomizeDialog.cpp do.
//      OpenMFC's CMultiDocTemplate declares no accelerator table at all (see
//      OnInitDialog / OnSelchangeViewType).
//
//  (4) OpenMFC's CObList keeps its nodes in a side table, so the retail walk
//      of a category list (m_pNodeHead +0x08, node data +0x10, pNext +0x00)
//      becomes a FindListStorage walk over the same object, as in
//      CMFCKeyMapDialog.cpp.
//
//  (5) The parent sheet.  Retail reaches its CMFCToolBarsCustomizeDialog as
//      DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent())
//      (::GetParent + CWnd::FromHandle 0x28ad70 + CObject::IsKindOf 0x234cf0
//      against the descriptor 0x180319428).  The same test is made here through
//      the thunks.  For a customize dialog OpenMFC itself constructed, the
//      object is a placement-new CPropertySheet (core/dialog/CPropertySheet.cpp,
//      ??0CPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z), whose vtable slot 0 reports
//      CPropertySheet, so there this downcast yields NULL.
//
//  (6) The parent's OnAssignKey / OnRemoveKey hooks (vftable 0x1803190d8
//      slots 102 / 103, +0x330 / +0x338; the retail base body of both is
//      0x3a60 `mov $0x1,%eax ; ret`) are dispatched through the parent's vtable
//      only when that vtable lies outside this image (an MSVC client class that
//      may override them); otherwise the base body's TRUE stands, as in
//      CMFCToolBarsCustomizeDialog.cpp's ToolsPageBlocksClose.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the definitions in the tree
// (file named on each line) unless the line says otherwise.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);                                  // core/dialog/RuntimeClasses.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                   // detail/DlgcoreSupport.cpp

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis);                    // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);                        // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis);                  // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CComboBox__UEAA_XZ(void* pThis);                     // core/controls/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                       // core/controls/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CComboBox* pThis, int nIndex, CString* pText);                              // core/controls/CComboBox.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                        // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                                            // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                                // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                               // core/window/CWnd.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);                  // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                  // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CFrameWnd* pThis, unsigned int nID, CString* rMessage);   // core/frame/CFrameWnd.cpp

extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();                          // core/frame/CFrameWnd.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ();        // featurepack/customize/RuntimeClasses.cpp

extern "C" void  MS_ABI impl__FillCategoriesComboBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCComboBox__H_Z(const void* pThis, CWnd* pWndCategory, int bAddEmpty);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
extern "C" int   MS_ABI impl__GetCountInCategory_CMFCToolBarsCustomizeDialog__QEBAHPEB_WAEBVCObList___Z(const void* pThis, const wchar_t* lpszItemName, const CObList* pListCommands);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
extern "C" const wchar_t* MS_ABI impl__GetCommandName_CMFCToolBarsCustomizeDialog__QEBAPEB_WI_Z(const void* pThis, unsigned int uiCmd);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp

extern "C" void* MS_ABI impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(void* pThis, LPACCEL lpAccel);           // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis);                                           // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const void* pThis, CString* str);   // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(void* pThis);                          // featurepack/customize/CMFCAcceleratorKeyAssignCtrl.cpp

extern "C" void  MS_ABI impl__ResetAll_CKeyboardManager__QEAAXXZ(CKeyboardManager* pThis);                         // featurepack/customize/CKeyboardManager.cpp
// ?UpdateAccelTable@CKeyboardManager@@QEAAHPEAVCMultiDocTemplate@@PEAUtagACCEL@@HPEAVCFrameWnd@@@Z, declared
// with the parameter list its mangled name describes (this, CMultiDocTemplate*, ACCEL*, int, CFrameWnd*).
// The definition in featurepack/customize/CKeyboardManager.cpp is still an
// auto-generated placeholder that drops `this` (p0..p3) and returns 0; see
// headerRequests.
extern "C" int   MS_ABI impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUtagACCEL__HPEAVCFrameWnd___Z(
    void* pThis, void* pTemplate, LPACCEL lpAccel, int nSize, CFrameWnd* pDefaultFrame);

extern "C" int   MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, unsigned int nType, unsigned int nIDHelp);   // core/collections/Globals.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszName, const wchar_t* lpszType);   // core/runtime/Globals.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);            // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowResourceException__YAXXZ();                  // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                     // detail/MemcoreSupport.cpp
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);                             // detail/MemcoreSupport.cpp

// Exported data objects, read the way retail reads them.
extern "C" void* impl__afxKeyboardManager__3PEAVCKeyboardManager__EA;   // featurepack/CMFC_misc_stubs.cpp (NULL in this tree)
extern "C" unsigned int impl__AFX_WM_RESETKEYBOARD__3IA;                // core/runtime/Globals.cpp

// The mingw linker's image base symbol (deviation (6)).
extern "C" IMAGE_DOS_HEADER __ImageBase;

// This file's own thunks that earlier bodies call.
extern "C" void* MS_ABI impl___0CMFCToolBarsKeyboardPropertyPage__QEAA_PEAVCFrameWnd__H_Z(void* pThis, CFrameWnd* pParentFrame, int bAutoSet);
extern "C" void  MS_ABI impl__AddKeyEntry_CMFCToolBarsKeyboardPropertyPage__IEAAXPEAUtagACCEL___Z(void* pThis, LPACCEL pEntry);
extern "C" void  MS_ABI impl__OnSelchangeCategory_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnSelchangeCommandsList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnSelchangeCurrentKeysList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnSelchangeViewType_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnUpdateNewShortcutKey_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// CMFCAcceleratorKeyAssignCtrl as the retail ctor 0x3440 lays it out
// (afxacceleratorkeyassignctrl.h member order): CEdit (0xe8), then
//   +0xe8  BOOL  m_bIsDefined   (`movl $0x0,0xe8(%rbx)`)
//   +0xec  ACCEL m_Accel        (`lea 0xec(%rbx),%rax` -> m_Helper.m_lpAccel)
//   +0xf8  CMFCAcceleratorKey m_Helper (vftable 0x1802da2c8 at +0xf8, ACCEL* at +0x100)
//   +0x108 BOOL  m_bIsFocused   (`movl $0x0,0x108(%rbx)`)
// sizeof 0x110 (the next page member starts at +0x350 = +0x240 + 0x110).
// detail/CMFCAcceleratorKeyAssignCtrlSupport.h mirrors the same +0xe8 / +0xec /
// +0x108 words under other names.
// ---------------------------------------------------------------------------
struct S_KeyAssignCtrl {
    alignas(8) unsigned char m_base[0xe8];    // +0x000 CEdit (a bare CWnd here, deviation (2))
    int           m_bIsDefined;               // +0x0e8
    ACCEL         m_Accel;                    // +0x0ec
    unsigned char m_pad0f2[0xf8 - 0xf2];
    struct { void* vfptr; LPACCEL m_lpAccel; } m_Helper;   // +0x0f8 CMFCAcceleratorKey
    int           m_bIsFocused;               // +0x108
};
static_assert(sizeof(ACCEL) == 6 && offsetof(ACCEL, key) == 2 && offsetof(ACCEL, cmd) == 4, "ACCEL: fVirt +0, key +2, cmd +4");
static_assert(offsetof(S_KeyAssignCtrl, m_bIsDefined) == 0xe8, "ctor 0x3440: movl $0x0,0xe8(%rbx)");
static_assert(offsetof(S_KeyAssignCtrl, m_Accel) == 0xec, "ctor 0x3440: lea 0xec(%rbx)");
static_assert(offsetof(S_KeyAssignCtrl, m_Helper) == 0xf8, "ctor 0x3440: vftable store at 0xf8(%rbx)");
static_assert(offsetof(S_KeyAssignCtrl, m_bIsFocused) == 0x108, "ctor 0x3440: movl $0x0,0x108(%rbx)");
static_assert(sizeof(S_KeyAssignCtrl) == 0x110, "m_wndNewKey spans +0x240..+0x350");

// ---------------------------------------------------------------------------
// The retail object (0x9f8 bytes).  Offsets from the ctor (0x179b90), the dtor
// (0x179dd0) and DoDataExchange (0x179f00).
// ---------------------------------------------------------------------------
struct S_KbdPage {
    alignas(8) unsigned char m_base[0x158];              // +0x000 CPropertyPage (OpenMFC sizeof(CPropertyPage) == 0x158, asserted below)
    alignas(8) unsigned char m_wndAssignedToTitle[0xe8]; // +0x158 CStatic   (DDX 16528 IDC_AFXBARRES_ASSIGNED_TO_TITLE)
    S_KeyAssignCtrl m_wndNewKey;                         // +0x240 CMFCAcceleratorKeyAssignCtrl (DDX 16644 IDC_AFXBARRES_NEW_SHORTCUT_KEY)
    alignas(8) unsigned char m_wndViewTypeList[0xe8];    // +0x350 CComboBox (DDX 16642 IDC_AFXBARRES_VIEW_TYPE)
    alignas(8) unsigned char m_wndViewIcon[0xe8];        // +0x438 CStatic   (DDX 16526 IDC_AFXBARRES_VIEW_ICON)
    alignas(8) unsigned char m_wndRemoveButton[0xe8];    // +0x520 CButton   (DDX 16646 IDC_AFXBARRES_REMOVE)
    alignas(8) unsigned char m_wndCurrentKeysList[0xe8]; // +0x608 CListBox  (DDX 16643 IDC_AFXBARRES_CURRENT_KEYS_LIST)
    alignas(8) unsigned char m_wndCommandsList[0xe8];    // +0x6f0 CListBox  (DDX 16641 IDC_AFXBARRES_COMMANDS_LIST)
    alignas(8) unsigned char m_wndCategoryList[0xe8];    // +0x7d8 CComboBox (DDX 16601 IDC_AFXBARRES_CATEGORY)
    alignas(8) unsigned char m_wndAssignButton[0xe8];    // +0x8c0 CButton   (DDX 16645 IDC_AFXBARRES_ASSIGN)
    CString            m_strDescription;                 // +0x9a8 (DDX_Text 16514 IDC_AFXBARRES_COMMAND_DESCRIPTION)
    CString            m_strAssignedTo;                  // +0x9b0 (DDX_Text 16527 IDC_AFXBARRES_ASSIGNED_TO)
    HACCEL             m_hAccelTable;                    // +0x9b8
    LPACCEL            m_lpAccel;                        // +0x9c0
    LPACCEL            m_pSelEntry;                      // +0x9c8
    int                m_nAccelSize;                     // +0x9d0
    int                m_bAutoSet;                       // +0x9d4
    void*              m_pSelTemplate;                   // +0x9d8 CMultiDocTemplate*
    CMFCToolBarButton* m_pSelButton;                     // +0x9e0
    CFrameWnd*         m_pParentFrame;                   // +0x9e8
    CString            m_strAllCategory;                 // +0x9f0
};
static_assert(offsetof(S_KbdPage, m_wndAssignedToTitle) == 0x158, "ctor: CWnd ctor on this+0x158");
static_assert(offsetof(S_KbdPage, m_wndNewKey) == 0x240, "ctor: CMFCAcceleratorKeyAssignCtrl ctor on this+0x240");
static_assert(offsetof(S_KbdPage, m_wndViewTypeList) == 0x350, "ctor: CWnd ctor on this+0x350");
static_assert(offsetof(S_KbdPage, m_wndViewIcon) == 0x438, "ctor: CWnd ctor on this+0x438");
static_assert(offsetof(S_KbdPage, m_wndRemoveButton) == 0x520, "ctor: CWnd ctor on this+0x520");
static_assert(offsetof(S_KbdPage, m_wndCurrentKeysList) == 0x608, "ctor: CWnd ctor on this+0x608");
static_assert(offsetof(S_KbdPage, m_wndCommandsList) == 0x6f0, "ctor: CWnd ctor on this+0x6f0");
static_assert(offsetof(S_KbdPage, m_wndCategoryList) == 0x7d8, "ctor: CWnd ctor on this+0x7d8");
static_assert(offsetof(S_KbdPage, m_wndAssignButton) == 0x8c0, "ctor: CWnd ctor on this+0x8c0");
static_assert(offsetof(S_KbdPage, m_strDescription) == 0x9a8, "ctor: nil string at 0x9a8");
static_assert(offsetof(S_KbdPage, m_strAssignedTo) == 0x9b0, "ctor: nil string at 0x9b0");
static_assert(offsetof(S_KbdPage, m_hAccelTable) == 0x9b8, "ctor: mov %rax,0x9b8");
static_assert(offsetof(S_KbdPage, m_lpAccel) == 0x9c0, "ctor: mov %rax,0x9c0");
static_assert(offsetof(S_KbdPage, m_pSelEntry) == 0x9c8, "ctor: mov %rax,0x9c8");
static_assert(offsetof(S_KbdPage, m_nAccelSize) == 0x9d0, "ctor: mov %eax,0x9d0");
static_assert(offsetof(S_KbdPage, m_bAutoSet) == 0x9d4, "ctor: mov %esi,0x9d4");
static_assert(offsetof(S_KbdPage, m_pSelTemplate) == 0x9d8, "ctor: mov %rax,0x9d8");
static_assert(offsetof(S_KbdPage, m_pSelButton) == 0x9e0, "ctor: mov %rax,0x9e0");
static_assert(offsetof(S_KbdPage, m_pParentFrame) == 0x9e8, "ctor: mov %rbp,0x9e8");
static_assert(offsetof(S_KbdPage, m_strAllCategory) == 0x9f0, "ctor: nil string at 0x9f0");
static_assert(sizeof(S_KbdPage) == 0x9f8, "CreateObject 0x179b50 / scalar deleting dtor 0x179d80: 0x9f8");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CStatic) == 0xe8 && sizeof(CComboBox) == 0xe8 &&
              sizeof(CListBox) == 0xe8 && sizeof(CButton) == 0xe8 && sizeof(CEdit) == 0xe8, "plain controls are bare CWnds");
static_assert(sizeof(CString) == 8, "retail CString is one pointer");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "every retail body reads the control HWNDs at member+0x40");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "OnSelchangeCategory / OnAssign: 0x24(button)");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "OnSelchangeCategory: 0x38(button)");
static_assert(offsetof(CMFCToolBarButton, m_strTextCustom) == 0x40, "OnSelchangeCategory: 0x40(button)");

// Resource IDs (afxribbonres.h, 14.51 SDK).
constexpr UINT kIddPropPage5          = 16136;   // IDD_AFXBARRES_PROPPAGE5 (0x3f08)
constexpr UINT kIdcAssignedToTitle    = 16528;   // IDC_AFXBARRES_ASSIGNED_TO_TITLE (0x4090)
constexpr UINT kIdcNewShortcutKey     = 16644;   // IDC_AFXBARRES_NEW_SHORTCUT_KEY  (0x4104)
constexpr UINT kIdcViewType           = 16642;   // IDC_AFXBARRES_VIEW_TYPE         (0x4102)
constexpr UINT kIdcViewIcon           = 16526;   // IDC_AFXBARRES_VIEW_ICON         (0x408e)
constexpr UINT kIdcRemove             = 16646;   // IDC_AFXBARRES_REMOVE            (0x4106)
constexpr UINT kIdcCurrentKeysList    = 16643;   // IDC_AFXBARRES_CURRENT_KEYS_LIST (0x4103)
constexpr UINT kIdcCommandsList       = 16641;   // IDC_AFXBARRES_COMMANDS_LIST     (0x4101)
constexpr UINT kIdcCategory           = 16601;   // IDC_AFXBARRES_CATEGORY          (0x40d9)
constexpr UINT kIdcAssign             = 16645;   // IDC_AFXBARRES_ASSIGN            (0x4105)
constexpr UINT kIdcCommandDescription = 16514;   // IDC_AFXBARRES_COMMAND_DESCRIPTION (0x4082)
constexpr UINT kIdcAssignedTo         = 16527;   // IDC_AFXBARRES_ASSIGNED_TO       (0x408f)
constexpr UINT kIdsDefaultView        = 16114;   // IDS_AFXBARRES_DEFAULT_VIEW      (0x3ef2)
constexpr UINT kIdsResetKeyboard      = 16115;   // IDS_AFXBARRES_RESET_KEYBOARD    (0x3ef3)
constexpr UINT kIdpUnassigned         = 16009;   // IDP_AFXBARRES_UNASSIGNED        (0x3e89)

// The retail ctor passes sizeof(PROPSHEETPAGE) = 0x68 as CPropertyPage's dwSize.
constexpr unsigned long kPropSheetPageSize = 0x68;

// Customize-dialog vftable slots of the key hooks (deviation (6)).
constexpr int kSlotOnAssignKey = 102;   // +0x330
constexpr int kSlotOnRemoveKey = 103;   // +0x338

// String literal of the retail image.
const wchar_t kUnknownCommand[] = L"????";   // 0x180341960

inline S_KbdPage* D(void* p) { return static_cast<S_KbdPage*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline HWND HwndOf(const void* pWnd) { return static_cast<const CWnd*>(pWnd)->m_hWnd; }   // member +0x40

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

// `new ACCEL[n]` as retail sizes it: movslq n, `mul` by 6, `cmovb` to
// SIZE_MAX on overflow (a negative n therefore also saturates), then ??2@.
std::size_t AccelArrayBytes(int n) {
    const unsigned long long count = static_cast<unsigned long long>(static_cast<long long>(n));
    if (count > SIZE_MAX / sizeof(ACCEL)) return SIZE_MAX;
    return static_cast<std::size_t>(count * sizeof(ACCEL));
}
inline LPACCEL NewAccelArray(int n) { return static_cast<LPACCEL>(impl___2_YAPEAX_K_Z(AccelArrayBytes(n))); }

// DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent()) -- deviation (5).
void* ParentCustomizeDialog(void* pThis) {
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Wnd(pThis)->m_hWnd));
    if (pParent == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pParent;
}

// A pointer that lies inside this DLL's own image (a mingw vtable) as opposed
// to an MSVC client's image (the helper of CMFCToolBarsCustomizeDialog.cpp).
bool PointsIntoThisImage(const void* p) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* q = static_cast<const unsigned char*>(p);
    return q >= base && q < base + nt->OptionalHeader.SizeOfImage;
}

// pWndParent->OnAssignKey(pAccel) / OnRemoveKey(pAccel) -- deviation (6).
typedef int (MS_ABI *KeyHookFn)(void* pThis, ACCEL* pAccel);
int CallKeyHook(void* pDlg, int nSlot, ACCEL* pAccel) {
    const void* vptr = *reinterpret_cast<void* const*>(pDlg);
    if (vptr != nullptr && !PointsIntoThisImage(vptr)) {
        return reinterpret_cast<KeyHookFn const*>(vptr)[nSlot](pDlg, pAccel);
    }
    return TRUE;   // the retail base body 0x3a60
}

// The category combo box carries CObList* item data (FillCategoriesComboBox);
// deviation (4).
typedef CList<CObject*, CObject*> ObListData;
inline const ObListData* CategoryButtons(const CObList* p) {
    return p ? openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(p) : nullptr;
}

// CClientDC(pWnd) as the retail ctor 0x2a3b20 / dtor 0x2a3be0 behave:
// ::GetDC(pWnd->m_hWnd), AfxThrowResourceException on failure, ::ReleaseDC on
// destruction.  Its m_hDC and m_hAttribDC are the same handle.
struct ClientDC {
    HWND hWnd;
    HDC  hdc;
    explicit ClientDC(HWND h) : hWnd(h), hdc(::GetDC(h)) {
        if (hdc == nullptr) impl__AfxThrowResourceException__YAXXZ();
    }
    ~ClientDC() { if (hdc != nullptr) ::ReleaseDC(hWnd, hdc); }
    ClientDC(const ClientDC&) = delete;
    ClientDC& operator=(const ClientDC&) = delete;
};

} // namespace

// Retail (RVA 0x179b90, mfc140u), transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE5 /*0x3f08*/, 0, 0x68);   // 0x215910
//     vfptr = 0x180319598;
//     m_wndAssignedToTitle: CWnd::CWnd() (0x28a700) + CStatic vftable
//     m_wndNewKey: CMFCAcceleratorKeyAssignCtrl::CMFCAcceleratorKeyAssignCtrl() (0x3440)
//     m_wndViewTypeList, m_wndViewIcon, m_wndRemoveButton, m_wndCurrentKeysList,
//     m_wndCommandsList, m_wndCategoryList, m_wndAssignButton:
//         CWnd::CWnd() + CComboBox / CStatic / CButton / CListBox / CListBox / CComboBox / CButton vftable
//     m_strDescription, m_strAssignedTo: nil string
//     m_bAutoSet = bAutoSet;  m_pParentFrame = pParentFrame;
//     m_strAllCategory: nil string
//     m_strDescription = L"";  m_strAssignedTo = L"";   // SetString(0x18033d19c, wcslen) 0x2e30, twice
//     m_hAccelTable = NULL;  m_lpAccel = NULL;  m_nAccelSize = 0;
//     m_pSelTemplate = NULL;  m_pSelButton = NULL;  m_pSelEntry = NULL;
// DEVIATIONS: the vftable stores are deviation (1); m_wndNewKey is built
// inline (deviation (2)).  Assigning L"" to a nil string leaves it empty, so
// the two CStrings are simply default-constructed.
// Symbol: ??0CMFCToolBarsKeyboardPropertyPage@@QEAA@PEAVCFrameWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsKeyboardPropertyPage__QEAA_PEAVCFrameWnd__H_Z(
    void* pThis, CFrameWnd* pParentFrame, int bAutoSet) {
    if (pThis == nullptr) return nullptr;
    S_KbdPage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage5, 0, kPropSheetPageSize);
    impl___0CWnd__QEAA_XZ(d->m_wndAssignedToTitle);

    // m_wndNewKey: the body of the retail ctor 0x3440 (deviation (2)).
    impl___0CWnd__QEAA_XZ(d->m_wndNewKey.m_base);
    impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(&d->m_wndNewKey.m_Helper, &d->m_wndNewKey.m_Accel);
    d->m_wndNewKey.m_bIsDefined = 0;
    d->m_wndNewKey.m_bIsFocused = 0;
    impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(&d->m_wndNewKey);

    impl___0CWnd__QEAA_XZ(d->m_wndViewTypeList);
    impl___0CWnd__QEAA_XZ(d->m_wndViewIcon);
    impl___0CWnd__QEAA_XZ(d->m_wndRemoveButton);
    impl___0CWnd__QEAA_XZ(d->m_wndCurrentKeysList);
    impl___0CWnd__QEAA_XZ(d->m_wndCommandsList);
    impl___0CWnd__QEAA_XZ(d->m_wndCategoryList);
    impl___0CWnd__QEAA_XZ(d->m_wndAssignButton);
    new (&d->m_strDescription) CString();
    new (&d->m_strAssignedTo) CString();
    d->m_bAutoSet = bAutoSet;
    d->m_pParentFrame = pParentFrame;
    new (&d->m_strAllCategory) CString();
    d->m_hAccelTable = nullptr;
    d->m_lpAccel = nullptr;
    d->m_nAccelSize = 0;
    d->m_pSelTemplate = nullptr;
    d->m_pSelButton = nullptr;
    d->m_pSelEntry = nullptr;
    return pThis;
}

// Retail (RVA 0x179dd0, mfc140u), transcribed:
//     vfptr = 0x180319598;
//     if (m_lpAccel != NULL) free(m_lpAccel);        // import slot 0x1802c74e8 = ucrt free; not cleared
//     ~m_strAllCategory;  ~m_strAssignedTo;  ~m_strDescription;   // inline CStringData release
//     m_wndAssignButton.~CButton();                   // 0x293be0
//     m_wndCategoryList.~CComboBox();                 // 0x2941a0
//     m_wndCommandsList.~CListBox();  m_wndCurrentKeysList.~CListBox();   // 0x293f30
//     m_wndRemoveButton.~CButton();
//     m_wndViewIcon.~CStatic();                       // 0x293b30
//     m_wndViewTypeList.~CComboBox();
//     m_wndNewKey: inline ~CMFCAcceleratorKeyAssignCtrl -- vftable 0x1802da348,
//         m_Helper vftable 0x1802da2c8, then CEdit::~CEdit()   // 0x294370
//     m_wndAssignedToTitle.~CStatic();
//     CPropertyPage::~CPropertyPage();                // tail jump 0x215f40
// DEVIATIONS: m_lpAccel was allocated with ??2@ (OnSelchangeViewType /
// OnAssign / OnRemove) and is released with ??3@ here (retail allocates with
// ??2@ and frees with the CRT free import; pairing OpenMFC's own new/delete
// keeps allocator and deallocator matched).  Retail's reset of m_Helper's
// vftable is spelled as a call to the CMFCAcceleratorKey dtor thunk, which
// does nothing in OpenMFC; retail's reset of m_wndNewKey's own vftable has
// no counterpart (deviation (2)).
// Symbol: ??1CMFCToolBarsKeyboardPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsKeyboardPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    if (d->m_lpAccel != nullptr) {
        impl___3_YAXPEAX_Z(d->m_lpAccel);
    }
    d->m_strAllCategory.~CString();
    d->m_strAssignedTo.~CString();
    d->m_strDescription.~CString();
    impl___1CButton__UEAA_XZ(d->m_wndAssignButton);
    impl___1CComboBox__UEAA_XZ(d->m_wndCategoryList);
    impl___1CListBox__UEAA_XZ(reinterpret_cast<CListBox*>(d->m_wndCommandsList));
    impl___1CListBox__UEAA_XZ(reinterpret_cast<CListBox*>(d->m_wndCurrentKeysList));
    impl___1CButton__UEAA_XZ(d->m_wndRemoveButton);
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndViewIcon));
    impl___1CComboBox__UEAA_XZ(d->m_wndViewTypeList);
    impl___1CMFCAcceleratorKey__UEAA_XZ(&d->m_wndNewKey.m_Helper);
    impl___1CEdit__UEAA_XZ(reinterpret_cast<CEdit*>(d->m_wndNewKey.m_base));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndAssignedToTitle));
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (RVA 0x179b50, mfc140u), fully transcribed:
//     void* p = operator new(0x9f8);                  // 0x27f0
//     return p ? new (p) CMFCToolBarsKeyboardPropertyPage(NULL, FALSE) : NULL;   // ctor 0x179b90
// Symbol: ?CreateObject@CMFCToolBarsKeyboardPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarsKeyboardPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_KbdPage));
    if (p == nullptr) return nullptr;
    return impl___0CMFCToolBarsKeyboardPropertyPage__QEAA_PEAVCFrameWnd__H_Z(p, nullptr, FALSE);
}

// Retail (RVA 0x179f00, mfc140u), fully transcribed: nine DDX_Control calls
// (0x293870) binding, in order, +0x158 to 16528, +0x240 to 16644, +0x350 to
// 16642, +0x438 to 16526, +0x520 to 16646, +0x608 to 16643, +0x6f0 to 16641,
// +0x7d8 to 16601 and +0x8c0 to 16645, then two DDX_Text(CString&) calls
// (0x209450): +0x9a8 to 16514 and +0x9b0 to 16527 (the last a tail jump).  No
// CPropertyPage::DoDataExchange call.
// Symbol: ?DoDataExchange@CMFCToolBarsKeyboardPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarsKeyboardPropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcAssignedToTitle, d->m_wndAssignedToTitle);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcNewShortcutKey, &d->m_wndNewKey);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcViewType, d->m_wndViewTypeList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcViewIcon, d->m_wndViewIcon);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcRemove, d->m_wndRemoveButton);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCurrentKeysList, d->m_wndCurrentKeysList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCommandsList, d->m_wndCommandsList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCategory, d->m_wndCategoryList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcAssign, d->m_wndAssignButton);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcCommandDescription, &d->m_strDescription);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcAssignedTo, &d->m_strAssignedTo);
}

// Retail (RVA 0x17af40, mfc140u), fully transcribed:
//     ENSURE(pEntry != NULL);
//     CMFCAcceleratorKey helper(pEntry);              // stack object: vftable 0x1802da2c8 + the ACCEL*
//     CString str;  helper.Format(str);               // 0x28f0
//     int iIndex = m_wndCurrentKeysList.AddString(str);           // LB_ADDSTRING (0x180) on +0x648
//     m_wndCurrentKeysList.SetItemData(iIndex, (DWORD_PTR)pEntry); // LB_SETITEMDATA (0x19a)
// The helper is built with the exported CMFCAcceleratorKey ctor thunk and
// released with its dtor thunk (a no-op in OpenMFC).  Retail emits no
// destructor code for the stack helper at all: after the LB_SETITEMDATA call
// the body only releases the CString.
// Symbol: ?AddKeyEntry@CMFCToolBarsKeyboardPropertyPage@@IEAAXPEAUtagACCEL@@@Z
extern "C" void MS_ABI impl__AddKeyEntry_CMFCToolBarsKeyboardPropertyPage__IEAAXPEAUtagACCEL___Z(void* pThis, LPACCEL pEntry) {
    if (pThis == nullptr) return;
    if (pEntry == nullptr) { ThrowInvalidArg(); return; }
    S_KbdPage* d = D(pThis);
    struct { void* vfptr; LPACCEL m_lpAccel; } helper;   // CMFCAcceleratorKey (0x10 bytes)
    impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(&helper, pEntry);
    CString str;
    impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(&helper, &str);
    const HWND hKeys = HwndOf(d->m_wndCurrentKeysList);
    const int iIndex = static_cast<int>(::SendMessage(hKeys, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.GetString())));
    ::SendMessage(hKeys, LB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), reinterpret_cast<LPARAM>(pEntry));
    impl___1CMFCAcceleratorKey__UEAA_XZ(&helper);
}

// Retail (RVA 0x17a010, mfc140u), transcribed:
//     CPropertyPage::OnInitDialog();                  // resolves to CDialog::OnInitDialog 0x208d10; result unused
//     ENSURE(afxKeyboardManager != NULL);             // 0x1803be218 = ?afxKeyboardManager@@3PEAVCKeyboardManager@@EA
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent());
//     pWndParent->FillCategoriesComboBox(m_wndCategoryList, FALSE);   // 0x179360, called even when pWndParent is NULL
//     m_wndCategoryList.SetCurSel(0);                 // CB_SETCURSEL (0x14e) on +0x818
//     OnSelchangeCategory();                          // 0x17a5d0
//     if (m_bAutoSet) {
//         CDocManager* pDocManager = AfxGetApp()->m_pDocManager;     // AfxGetModuleState 0x133930, +0x8, +0xb8
//         if (pDocManager != NULL)
//             for (POSITION pos = pDocManager->GetFirstDocTemplatePosition(); pos != NULL;) {   // vslot 6 (+0x30)
//                 CDocTemplate* pTemplate = pDocManager->GetNextDocTemplate(pos);                 // vslot 7 (+0x38)
//                 if (pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)) &&               // 0x180329588
//                     ((CMultiDocTemplate*)pTemplate)->m_hAccelTable != NULL) {              // +0xf8
//                     BOOL bExist = FALSE;       // an entry with the same m_nIDResource (+0x98) already listed?
//                     for (int i = 0; i < m_wndViewTypeList.GetCount(); i++) {            // CB_GETCOUNT (0x146) on +0x390
//                         CMultiDocTemplate* pListTemplate = (CMultiDocTemplate*)m_wndViewTypeList.GetItemData(i);   // CB_GETITEMDATA
//                         if (pListTemplate != NULL && pListTemplate->m_nIDResource == pTemplate->m_nIDResource) { bExist = TRUE; break; }
//                     }
//                     if (!bExist) {
//                         CString strName;  pTemplate->GetDocString(strName, CDocTemplate::fileNewName);   // vslot 27 (+0xd8), index 2
//                         int iIndex = m_wndViewTypeList.AddString(strName);                 // CB_ADDSTRING (0x143)
//                         m_wndViewTypeList.SetItemData(iIndex, (DWORD_PTR)pTemplate);      // CB_SETITEMDATA (0x151)
//                     }
//                 }
//             }
//     }
//     CFrameWnd* pWndMain = DYNAMIC_DOWNCAST(CFrameWnd, m_pParentFrame);   // 0x18033aef0
//     if (pWndMain != NULL && pWndMain->m_hAccelTable != NULL) {            // +0xf8
//         CString strDefaultMenu;  ENSURE(strDefaultMenu.LoadString(IDS_AFXBARRES_DEFAULT_VIEW));   // 0x3ef2
//         int iIndex = m_wndViewTypeList.AddString(strDefaultMenu);
//         m_wndViewTypeList.SetItemData(iIndex, NULL);
//         m_wndViewTypeList.SetCurSel(iIndex);
//         OnSelchangeViewType();                      // 0x17ad90
//     }
//     if (m_wndViewTypeList.GetCurSel() == CB_ERR) {  // CB_GETCURSEL (0x147)
//         m_wndViewTypeList.SetCurSel(0);
//         OnSelchangeViewType();
//     }
//     return TRUE;
// DEVIATIONS:
//  * the m_bAutoSet document-template loop is not reproduced: its only effect
//    is gated on CMultiDocTemplate::m_hAccelTable != NULL, and OpenMFC's
//    CMultiDocTemplate has no accelerator table (deviation (3)), so no
//    template can pass the gate;
//  * FillCategoriesComboBox is called with the possibly-NULL pWndParent, as
//    retail does; OpenMFC's thunk returns at once on a NULL `this` where the
//    retail body would fault.  (See deviation (5) for when it is NULL.)
// Symbol: ?OnInitDialog@CMFCToolBarsKeyboardPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsKeyboardPropertyPage__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_KbdPage* d = D(pThis);

    (void)impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));

    if (impl__afxKeyboardManager__3PEAVCKeyboardManager__EA == nullptr) { ThrowInvalidArg(); return TRUE; }

    void* pWndParent = ParentCustomizeDialog(pThis);
    impl__FillCategoriesComboBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCComboBox__H_Z(pWndParent, Wnd(d->m_wndCategoryList), FALSE);
    ::SendMessage(HwndOf(d->m_wndCategoryList), CB_SETCURSEL, 0, 0);
    impl__OnSelchangeCategory_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);

    // (m_bAutoSet document-template loop: see DEVIATIONS above)

    const HWND hViewType = HwndOf(d->m_wndViewTypeList);
    CFrameWnd* pWndMain = d->m_pParentFrame;
    if (pWndMain != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWndMain, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ()) &&
        pWndMain->m_hAccelTable != nullptr) {
        CString strDefaultMenu;
        if (!EnsureLoadString(strDefaultMenu, kIdsDefaultView)) return TRUE;
        const int iIndex = static_cast<int>(::SendMessage(hViewType, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(strDefaultMenu.GetString())));
        ::SendMessage(hViewType, CB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0);
        ::SendMessage(hViewType, CB_SETCURSEL, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0);
        impl__OnSelchangeViewType_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
    }

    if (static_cast<int>(::SendMessage(hViewType, CB_GETCURSEL, 0, 0)) == CB_ERR) {
        ::SendMessage(hViewType, CB_SETCURSEL, 0, 0);
        impl__OnSelchangeViewType_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
    }
    return TRUE;
}

// Retail (RVA 0x17a370, mfc140u), fully transcribed:
//     ENSURE(m_lpAccel != NULL);
//     ENSURE(m_pSelButton != NULL);
//     ENSURE(m_wndNewKey.IsKeyDefined());             // +0x328 (m_wndNewKey + 0xe8)
//     ACCEL* pAccel = (ACCEL*)m_wndNewKey.GetAccel(); // +0x32c (m_wndNewKey + 0xec)
//     ENSURE(pAccel != NULL);
//     pAccel->cmd = (WORD)m_pSelButton->m_nID;        // 0x24(button) -> 0x4(ACCEL)
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent());
//     if (!pWndParent->OnAssignKey(pAccel)) return;  // vslot 102 (+0x330), no NULL check
//     LPACCEL lpAccelOld = m_lpAccel;
//     m_lpAccel = new ACCEL[m_nAccelSize + 1];         // ??2@ (0x27f0), 6 * n saturated
//     ENSURE(m_lpAccel != NULL);
//     memcpy(m_lpAccel, lpAccelOld, sizeof(ACCEL) * m_nAccelSize);   // import slot 0x1802c7420 = memcpy
//     int listcount = m_wndCurrentKeysList.GetCount();                 // LB_GETCOUNT (0x18b) on +0x648
//     for (int i = 0; i < m_nAccelSize; i++)
//         for (int idx = 0; idx < listcount; idx++)
//             if ((LPACCEL)m_wndCurrentKeysList.GetItemData(idx) == &lpAccelOld[i]) {   // LB_GETITEMDATA (0x199)
//                 m_wndCurrentKeysList.SetItemData(idx, (DWORD_PTR)&m_lpAccel[i]);       // LB_SETITEMDATA (0x19a)
//                 break;
//             }
//     m_lpAccel[m_nAccelSize++] = *pAccel;
//     delete[] lpAccelOld;                            // import slot 0x1802c74e8 = ucrt free
//     afxKeyboardManager->UpdateAccelTable(m_pSelTemplate, m_lpAccel, m_nAccelSize, NULL);   // 0x733a0
//     AddKeyEntry(&m_lpAccel[m_nAccelSize - 1]);      // 0x17af40
//     m_wndNewKey.ResetKey();                         // 0x3700
//     OnUpdateNewShortcutKey();                       // 0x17b010
//     m_wndCommandsList.SetFocus();                   // 0x2a9b60 on +0x6f0
// DEVIATIONS: a NULL pWndParent (retail faults on it) throws the ENSURE
// exception this class uses for the same downcast elsewhere; ENSURE(pAccel !=
// NULL) tests the address of a member, cannot fire, and is omitted; the hook dispatch
// is deviation (6); lpAccelOld is released with ??3@ (see the destructor).
// The UpdateAccelTable call reaches a placeholder definition today (see the
// declaration above).
// Symbol: ?OnAssign@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnAssign_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }
    if (d->m_pSelButton == nullptr) { ThrowInvalidArg(); return; }
    if (d->m_wndNewKey.m_bIsDefined == 0) { ThrowInvalidArg(); return; }
    ACCEL* pAccel = &d->m_wndNewKey.m_Accel;
    pAccel->cmd = static_cast<WORD>(d->m_pSelButton->m_nID);

    void* pWndParent = ParentCustomizeDialog(pThis);
    if (pWndParent == nullptr) { ThrowInvalidArg(); return; }
    if (!CallKeyHook(pWndParent, kSlotOnAssignKey, pAccel)) return;

    LPACCEL lpAccelOld = d->m_lpAccel;
    d->m_lpAccel = NewAccelArray(d->m_nAccelSize + 1);
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }
    std::memcpy(d->m_lpAccel, lpAccelOld, AccelArrayBytes(d->m_nAccelSize));

    const HWND hKeys = HwndOf(d->m_wndCurrentKeysList);
    const int listcount = static_cast<int>(::SendMessage(hKeys, LB_GETCOUNT, 0, 0));
    for (int i = 0; i < d->m_nAccelSize; i++) {
        for (int idx = 0; idx < listcount; idx++) {
            if (reinterpret_cast<LPACCEL>(::SendMessage(hKeys, LB_GETITEMDATA, static_cast<WPARAM>(idx), 0)) == &lpAccelOld[i]) {
                ::SendMessage(hKeys, LB_SETITEMDATA, static_cast<WPARAM>(idx), reinterpret_cast<LPARAM>(&d->m_lpAccel[i]));
                break;
            }
        }
    }

    d->m_lpAccel[d->m_nAccelSize++] = *pAccel;
    impl___3_YAXPEAX_Z(lpAccelOld);

    impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUtagACCEL__HPEAVCFrameWnd___Z(
        impl__afxKeyboardManager__3PEAVCKeyboardManager__EA, d->m_pSelTemplate, d->m_lpAccel, d->m_nAccelSize, nullptr);
    impl__AddKeyEntry_CMFCToolBarsKeyboardPropertyPage__IEAAXPEAUtagACCEL___Z(pThis, &d->m_lpAccel[d->m_nAccelSize - 1]);
    impl__ResetKey_CMFCAcceleratorKeyAssignCtrl__QEAAXXZ(&d->m_wndNewKey);
    impl__OnUpdateNewShortcutKey_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(d->m_wndCommandsList));
}

// Retail (RVA 0x17aaa0, mfc140u), fully transcribed:
//     ENSURE(m_pSelEntry != NULL);
//     ENSURE(m_lpAccel != NULL);
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent());
//     if (!pWndParent->OnRemoveKey(m_pSelEntry)) return;   // vslot 103 (+0x338), no NULL check
//     LPACCEL lpAccelOld = m_lpAccel;
//     m_lpAccel = new ACCEL[m_nAccelSize - 1];         // 6 * n saturated
//     ENSURE(m_lpAccel != NULL);
//     int iNewIndex = 0;
//     for (int i = 0; i < m_nAccelSize; i++) {
//         if (m_pSelEntry != &lpAccelOld[i]) {
//             m_lpAccel[iNewIndex] = lpAccelOld[i];
//             int listcount = m_wndCurrentKeysList.GetCount();          // LB_GETCOUNT, re-read per entry
//             for (int idx = 0; idx < listcount; idx++)
//                 if ((LPACCEL)m_wndCurrentKeysList.GetItemData(idx) == &lpAccelOld[i]) {
//                     m_wndCurrentKeysList.SetItemData(idx, (DWORD_PTR)&m_lpAccel[iNewIndex]);
//                     break;
//                 }
//             iNewIndex++;
//         }
//     }
//     delete[] lpAccelOld;                            // ucrt free
//     m_nAccelSize--;
//     afxKeyboardManager->UpdateAccelTable(m_pSelTemplate, m_lpAccel, m_nAccelSize, NULL);   // 0x733a0
//     OnSelchangeCommandsList();                      // 0x17a8e0
//     m_wndCommandsList.SetFocus();
// DEVIATIONS: as OnAssign (NULL pWndParent, hook dispatch, ??3@).
// Symbol: ?OnRemove@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnRemove_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    if (d->m_pSelEntry == nullptr) { ThrowInvalidArg(); return; }
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }

    void* pWndParent = ParentCustomizeDialog(pThis);
    if (pWndParent == nullptr) { ThrowInvalidArg(); return; }
    if (!CallKeyHook(pWndParent, kSlotOnRemoveKey, d->m_pSelEntry)) return;

    LPACCEL lpAccelOld = d->m_lpAccel;
    d->m_lpAccel = NewAccelArray(d->m_nAccelSize - 1);
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }

    const HWND hKeys = HwndOf(d->m_wndCurrentKeysList);
    int iNewIndex = 0;
    for (int i = 0; i < d->m_nAccelSize; i++) {
        if (d->m_pSelEntry == &lpAccelOld[i]) continue;
        d->m_lpAccel[iNewIndex] = lpAccelOld[i];
        const int listcount = static_cast<int>(::SendMessage(hKeys, LB_GETCOUNT, 0, 0));
        for (int idx = 0; idx < listcount; idx++) {
            if (reinterpret_cast<LPACCEL>(::SendMessage(hKeys, LB_GETITEMDATA, static_cast<WPARAM>(idx), 0)) == &lpAccelOld[i]) {
                ::SendMessage(hKeys, LB_SETITEMDATA, static_cast<WPARAM>(idx), reinterpret_cast<LPARAM>(&d->m_lpAccel[iNewIndex]));
                break;
            }
        }
        iNewIndex++;
    }

    impl___3_YAXPEAX_Z(lpAccelOld);
    d->m_nAccelSize--;

    impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUtagACCEL__HPEAVCFrameWnd___Z(
        impl__afxKeyboardManager__3PEAVCKeyboardManager__EA, d->m_pSelTemplate, d->m_lpAccel, d->m_nAccelSize, nullptr);
    impl__OnSelchangeCommandsList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(d->m_wndCommandsList));
}

// Retail (RVA 0x17aca0, mfc140u), fully transcribed:
//     CString str;  ENSURE(str.LoadString(IDS_AFXBARRES_RESET_KEYBOARD));   // 0x3ef3
//     if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION /*0x24*/) != IDYES) return;   // 0x1cec10, nIDHelp 0
//     afxKeyboardManager->ResetAll();                 // 0x73e00
//     if (m_pParentFrame != NULL)                     // +0x9e8
//         ::SendMessage(m_pParentFrame->m_hWnd, AFX_WM_RESETKEYBOARD, 0, 0);   // ?AFX_WM_RESETKEYBOARD@@3IA (0x1803c24fc)
//     OnSelchangeViewType();                          // 0x17ad90
//     OnSelchangeCommandsList();                      // 0x17a8e0
// ResetAll is called on afxKeyboardManager without a NULL check, as retail
// does; OpenMFC's ResetAll thunk itself returns on a NULL `this`.
// Symbol: ?OnResetAll@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetAll_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    CString str;
    if (!EnsureLoadString(str, kIdsResetKeyboard)) return;
    if (impl__AfxMessageBox__YAHPEB_WII_Z(str.GetString(), MB_YESNO | MB_ICONQUESTION, 0) != IDYES) return;

    impl__ResetAll_CKeyboardManager__QEAAXXZ(static_cast<CKeyboardManager*>(impl__afxKeyboardManager__3PEAVCKeyboardManager__EA));
    if (d->m_pParentFrame != nullptr) {
        ::SendMessage(d->m_pParentFrame->m_hWnd, impl__AFX_WM_RESETKEYBOARD__3IA, 0, 0);
    }
    impl__OnSelchangeViewType_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
    impl__OnSelchangeCommandsList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
}

// Retail (RVA 0x17a5d0, mfc140u), transcribed:
//     UpdateData();                                   // 0x2910d0, TRUE
//     int iSelIndex = m_wndCategoryList.GetCurSel();  // CB_GETCURSEL on +0x818
//     if (iSelIndex == CB_ERR) return;
//     m_wndCommandsList.ResetContent();               // LB_RESETCONTENT (0x184) on +0x730
//     m_wndCurrentKeysList.ResetContent();            // ... on +0x648
//     CObList* pCategoryButtonsList = (CObList*)m_wndCategoryList.GetItemData(iSelIndex);   // CB_GETITEMDATA
//     CString strCategory;  m_wndCategoryList.GetLBText(iSelIndex, strCategory);          // 0x294250
//     BOOL bAllCommands = (strCategory == m_strAllCategory);   // wcscmp (import slot 0x1802c7770)
//     CClientDC dcCommands(&m_wndCommandsList);       // 0x2a3b20
//     CFont* pOldFont = dcCommands.SelectObject(m_wndCommandsList.GetFont());   // WM_GETFONT (0x31), CGdiObject::FromHandle 0x2a3ea0, SelectObject 0x2a2730
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent());
//     ENSURE(pWndParent != NULL);
//     int nMaxWidth = 0;
//     for (POSITION pos = pCategoryButtonsList->GetHeadPosition(); pos != NULL;) {   // m_pNodeHead +0x08, unchecked
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)pCategoryButtonsList->GetNext(pos);   // node data +0x10
//         ENSURE(pButton != NULL);
//         if (pButton->m_nID > 0 && pButton->m_nID != (UINT)-1) {   // `dec ; cmp $0xfffffffd ; ja`
//             CString strText = pButton->m_strText;  // +0x38
//             if (!pButton->m_strTextCustom.IsEmpty() &&                                // +0x40
//                 (bAllCommands || pWndParent->GetCountInCategory(strText, *pCategoryButtonsList) > 1))   // 0x179980
//                 strText = pButton->m_strTextCustom;
//             int iIndex = m_wndCommandsList.AddString(strText);                        // LB_ADDSTRING (0x180)
//             m_wndCommandsList.SetItemData(iIndex, (DWORD_PTR)pButton);                // LB_SETITEMDATA (0x19a)
//             nMaxWidth = max(nMaxWidth, dcCommands.GetTextExtent(strText).cx);        // GetTextExtentPoint32W(m_hAttribDC)
//         }
//     }
//     nMaxWidth += ::GetSystemMetrics(SM_CXHSCROLL);  // 0x15
//     m_wndCommandsList.SetHorizontalExtent(nMaxWidth);   // LB_SETHORIZONTALEXTENT (0x194)
//     dcCommands.SelectObject(pOldFont);
//     m_wndNewKey.EnableWindow(FALSE);                // 0x2a9b30 on +0x240
//     m_wndCommandsList.SetCurSel(0);                 // LB_SETCURSEL (0x186)
//     OnSelchangeCommandsList();                      // 0x17a8e0
// DEVIATIONS: the list walk is deviation (4) (a NULL list, which retail
// dereferences, walks as empty); the CClientDC and CFont temporaries are the
// Win32 calls they wrap (::GetDC / ::SelectObject / ::ReleaseDC; retail's
// SelectObject(CFont*) passes a NULL CFont's handle as NULL, as ::SelectObject
// receives it here); retail's max() macro calls GetTextExtent a second time
// unless the running maximum is strictly greater (`cmp ...,%r13d ; jg` at
// 0x17a7e9), so ties call it twice too; the second call returns the same value
// and is done once here.  CStringT::operator=='s ATLENSURE on m_strAllCategory's buffer (the
// AtlThrow(E_FAIL) at 0x17a8cf) cannot fire: a CString's buffer is never NULL.
// Symbol: ?OnSelchangeCategory@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCategory_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), TRUE);

    const HWND hCategory = HwndOf(d->m_wndCategoryList);
    const HWND hCommands = HwndOf(d->m_wndCommandsList);
    const int iSelIndex = static_cast<int>(::SendMessage(hCategory, CB_GETCURSEL, 0, 0));
    if (iSelIndex == CB_ERR) return;

    ::SendMessage(hCommands, LB_RESETCONTENT, 0, 0);
    ::SendMessage(HwndOf(d->m_wndCurrentKeysList), LB_RESETCONTENT, 0, 0);

    const CObList* pCategoryButtonsList = reinterpret_cast<const CObList*>(
        ::SendMessage(hCategory, CB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iSelIndex)), 0));
    CString strCategory;
    impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        reinterpret_cast<const CComboBox*>(d->m_wndCategoryList), iSelIndex, &strCategory);
    const bool bAllCommands = std::wcscmp(strCategory.GetString(), d->m_strAllCategory.GetString()) == 0;

    ClientDC dcCommands(hCommands);
    if (dcCommands.hdc == nullptr) return;
    HGDIOBJ hOldFont = ::SelectObject(dcCommands.hdc, reinterpret_cast<HGDIOBJ>(::SendMessage(hCommands, WM_GETFONT, 0, 0)));

    void* pWndParent = ParentCustomizeDialog(pThis);
    if (pWndParent == nullptr) { ThrowInvalidArg(); return; }

    int nMaxWidth = 0;
    if (const ObListData* items = CategoryButtons(pCategoryButtonsList)) {
        ObListData::POSITION pos = items->GetHeadPosition();
        while (pos != ObListData::POSITION(nullptr)) {
            CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(items->GetNext(pos));
            if (pButton == nullptr) { ThrowInvalidArg(); return; }
            if (pButton->m_nID == 0 || pButton->m_nID == static_cast<UINT>(-1)) continue;

            CString strText = pButton->m_strText;
            if (!pButton->m_strTextCustom.IsEmpty() &&
                (bAllCommands ||
                 impl__GetCountInCategory_CMFCToolBarsCustomizeDialog__QEBAHPEB_WAEBVCObList___Z(pWndParent, strText.GetString(), pCategoryButtonsList) > 1)) {
                strText = pButton->m_strTextCustom;
            }
            const int iIndex = static_cast<int>(::SendMessage(hCommands, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(strText.GetString())));
            ::SendMessage(hCommands, LB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), reinterpret_cast<LPARAM>(pButton));
            SIZE size = { 0, 0 };
            ::GetTextExtentPoint32W(dcCommands.hdc, strText.GetString(), strText.GetLength(), &size);
            if (size.cx > nMaxWidth) nMaxWidth = size.cx;
        }
    }

    nMaxWidth += ::GetSystemMetrics(SM_CXHSCROLL);
    ::SendMessage(hCommands, LB_SETHORIZONTALEXTENT, static_cast<WPARAM>(static_cast<INT_PTR>(nMaxWidth)), 0);
    ::SelectObject(dcCommands.hdc, hOldFont);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndNewKey.m_base), FALSE);
    ::SendMessage(hCommands, LB_SETCURSEL, 0, 0);
    impl__OnSelchangeCommandsList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
}

// Retail (RVA 0x17a8e0, mfc140u), fully transcribed:
//     m_strDescription.Empty();                       // 0x33b0 on +0x9a8
//     m_wndCurrentKeysList.ResetContent();            // LB_RESETCONTENT on +0x648
//     OnSelchangeCurrentKeysList();                   // 0x17aa20
//     int iIndex = m_wndCommandsList.GetCurSel();     // LB_GETCURSEL (0x188) on +0x730
//     if (iIndex == LB_ERR) {
//         m_pSelButton = NULL;
//         m_wndNewKey.EnableWindow(FALSE);
//     } else {
//         m_pSelButton = (CMFCToolBarButton*)m_wndCommandsList.GetItemData(iIndex);   // LB_GETITEMDATA
//         CFrameWnd* pParent = GetParentFrame();      // 0x28e200
//         if (pParent != NULL && pParent->GetSafeHwnd() != NULL)
//             pParent->GetMessageString(m_pSelButton->m_nID, m_strDescription);   // vslot 95 (+0x2f8)
//         if (m_lpAccel != NULL)
//             for (int i = 0; i < m_nAccelSize; i++)
//                 if (m_pSelButton->m_nID == m_lpAccel[i].cmd)   // 32-bit m_nID vs movzwl cmd
//                     AddKeyEntry(&m_lpAccel[i]);                // 0x17af40
//         m_wndNewKey.EnableWindow(TRUE);             // 0x2a9b30
//     }
//     UpdateData(FALSE);                              // tail jump 0x2910d0
// GetMessageString is devirtualised to the CFrameWnd thunk, so an override in
// a client frame class is not reached.  (The only GetMessageString exports of
// mfc140u are CFrameWnd's and COleControl's, which share one folded body at
// RVA 0x1e2460; unexported overrides were not searched for.)
// Symbol: ?OnSelchangeCommandsList@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCommandsList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    d->m_strDescription.Empty();
    ::SendMessage(HwndOf(d->m_wndCurrentKeysList), LB_RESETCONTENT, 0, 0);
    impl__OnSelchangeCurrentKeysList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);

    const HWND hCommands = HwndOf(d->m_wndCommandsList);
    const int iIndex = static_cast<int>(::SendMessage(hCommands, LB_GETCURSEL, 0, 0));
    int bEnable = FALSE;
    if (iIndex == LB_ERR) {
        d->m_pSelButton = nullptr;
    } else {
        d->m_pSelButton = reinterpret_cast<CMFCToolBarButton*>(
            ::SendMessage(hCommands, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0));
        CFrameWnd* pParent = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(Wnd(pThis));
        if (pParent != nullptr && pParent->m_hWnd != nullptr) {
            impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                pParent, d->m_pSelButton->m_nID, &d->m_strDescription);
        }
        if (d->m_lpAccel != nullptr) {
            for (int i = 0; i < d->m_nAccelSize; i++) {
                if (d->m_pSelButton->m_nID == static_cast<UINT>(d->m_lpAccel[i].cmd)) {
                    impl__AddKeyEntry_CMFCToolBarsKeyboardPropertyPage__IEAAXPEAUtagACCEL___Z(pThis, &d->m_lpAccel[i]);
                }
            }
        }
        bEnable = TRUE;
    }
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndNewKey.m_base), bEnable);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);
}

// Retail (RVA 0x17aa20, mfc140u), fully transcribed:
//     int iIndex = m_wndCurrentKeysList.GetCurSel();  // LB_GETCURSEL on +0x648
//     if (iIndex == LB_ERR) {
//         m_pSelEntry = NULL;
//         m_wndRemoveButton.EnableWindow(FALSE);      // tail jump 0x2a9b30 on +0x520
//         return;
//     }
//     m_pSelEntry = (LPACCEL)m_wndCurrentKeysList.GetItemData(iIndex);   // LB_GETITEMDATA
//     ENSURE(m_pSelEntry != NULL);
//     m_wndRemoveButton.EnableWindow(TRUE);
// Symbol: ?OnSelchangeCurrentKeysList@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCurrentKeysList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    const HWND hKeys = HwndOf(d->m_wndCurrentKeysList);
    const int iIndex = static_cast<int>(::SendMessage(hKeys, LB_GETCURSEL, 0, 0));
    if (iIndex == LB_ERR) {
        d->m_pSelEntry = nullptr;
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndRemoveButton), FALSE);
        return;
    }
    d->m_pSelEntry = reinterpret_cast<LPACCEL>(::SendMessage(hKeys, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0));
    if (d->m_pSelEntry == nullptr) { ThrowInvalidArg(); return; }
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndRemoveButton), TRUE);
}

// Retail (RVA 0x17ad90, mfc140u), fully transcribed:
//     m_hAccelTable = NULL;  m_pSelTemplate = NULL;
//     if (m_lpAccel != NULL) { delete[] m_lpAccel; m_lpAccel = NULL; }   // ucrt free
//     int iIndex = m_wndViewTypeList.GetCurSel();     // CB_GETCURSEL on +0x390
//     if (iIndex == CB_ERR) { m_wndViewIcon.SetIcon(NULL); return; }     // STM_SETICON (0x170) on +0x478
//     HICON hicon = NULL;
//     CMultiDocTemplate* pTemplate = (CMultiDocTemplate*)m_wndViewTypeList.GetItemData(iIndex);   // CB_GETITEMDATA
//     if (pTemplate != NULL) {
//         hicon = AfxGetApp()->LoadIcon(pTemplate->m_nIDResource);     // +0x98 (movzwl); AfxFindResourceHandle(id, RT_GROUP_ICON) 0x2aeb50 + ::LoadIconW
//         m_hAccelTable = pTemplate->m_hAccelTable;                    // +0xf8
//     } else {
//         CFrameWnd* pWndMain = DYNAMIC_DOWNCAST(CFrameWnd, m_pParentFrame);
//         if (pWndMain != NULL) {
//             hicon = (HICON)::GetClassLongPtr(pWndMain->m_hWnd, GCLP_HICON);   // -14
//             m_hAccelTable = pWndMain->m_hAccelTable;                        // +0xf8
//         }
//     }
//     if (hicon == NULL) hicon = ::LoadIcon(NULL, IDI_APPLICATION);   // 0x7f00
//     m_wndViewIcon.SetIcon(hicon);
//     ENSURE(m_hAccelTable != NULL);
//     m_nAccelSize = ::CopyAcceleratorTable(m_hAccelTable, NULL, 0);
//     m_lpAccel = new ACCEL[m_nAccelSize];             // 6 * n saturated
//     ENSURE(m_lpAccel != NULL);
//     ::CopyAcceleratorTable(m_hAccelTable, m_lpAccel, m_nAccelSize);
//     m_pSelTemplate = pTemplate;
//     OnSelchangeCommandsList();                      // 0x17a8e0
// Every import slot was resolved with iatu.py (LoadIconW 0x1802c71c8,
// GetClassLongPtrW 0x1802c6d48, CopyAcceleratorTableW 0x1802c6d68).
// DEVIATIONS: the frame's m_hAccelTable is OpenMFC's member (deviation (3)).
// OpenMFC's CMultiDocTemplate has no accelerator table, so a template entry
// leaves m_hAccelTable NULL and the ENSURE throws; OnInitDialog never adds
// such an entry.  The template icon uses OpenMFC's CDocTemplate::m_nIDResource
// member, truncated to a WORD as retail's movzwl does.  m_lpAccel is released
// with ??3@ (see the destructor).
// Symbol: ?OnSelchangeViewType@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeViewType_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    d->m_hAccelTable = nullptr;
    d->m_pSelTemplate = nullptr;
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
        LPCWSTR lpszIcon = MAKEINTRESOURCEW(static_cast<WORD>(pTemplate->m_nIDResource));
        hicon = ::LoadIconW(impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszIcon, RT_GROUP_ICON), lpszIcon);
        d->m_hAccelTable = nullptr;         // deviation (3): OpenMFC's CMultiDocTemplate has no m_hAccelTable
    } else {
        CFrameWnd* pWndMain = d->m_pParentFrame;
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
    d->m_lpAccel = NewAccelArray(d->m_nAccelSize);
    if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }
    ::CopyAcceleratorTableW(d->m_hAccelTable, d->m_lpAccel, d->m_nAccelSize);
    d->m_pSelTemplate = pTemplate;
    impl__OnSelchangeCommandsList_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(pThis);
}

// Retail (RVA 0x17b010, mfc140u), fully transcribed:
//     ACCEL* pAccel = (ACCEL*)m_wndNewKey.GetAccel(); // +0x32c
//     ENSURE(pAccel != NULL);
//     m_strAssignedTo.Empty();                        // 0x33b0 on +0x9b0
//     m_wndAssignedToTitle.ShowWindow(SW_HIDE);       // 0x2a9ad0 on +0x158
//     m_wndAssignButton.EnableWindow(FALSE);          // 0x2a9b30 on +0x8c0
//     if (m_wndNewKey.IsKeyDefined()) {               // +0x328
//         ENSURE(m_lpAccel != NULL);
//         BOOL bIsAlreadyDefined = FALSE;
//         for (int i = 0; !bIsAlreadyDefined && i < m_nAccelSize; i++) {
//             const BYTE fRelFlags = FCONTROL | FALT | FSHIFT | FVIRTKEY;   // 0x1d
//             if (pAccel->key == m_lpAccel[i].key &&                         // +0x2 (16-bit)
//                 (pAccel->fVirt & fRelFlags) == (m_lpAccel[i].fVirt & fRelFlags)) {   // xor ; test $0x1d
//                 CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent());
//                 ENSURE(pWndParent != NULL);
//                 LPCTSTR lpszCommand = pWndParent->GetCommandName(m_lpAccel[i].cmd);   // 0x179710
//                 m_strAssignedTo = lpszCommand == NULL ? _T("????") : lpszCommand;     // SetString 0x2e30; L"????" at 0x180341960
//                 bIsAlreadyDefined = TRUE;
//             }
//         }
//         if (!bIsAlreadyDefined) {
//             ENSURE(m_strAssignedTo.LoadString(IDP_AFXBARRES_UNASSIGNED));   // 0x3e89
//             m_wndAssignButton.EnableWindow();       // TRUE
//         }
//         m_wndAssignedToTitle.ShowWindow(SW_SHOW);   // 5
//     }
//     UpdateData(FALSE);                              // tail jump 0x2910d0
// The ENSURE(pAccel != NULL) on the address of a member cannot fire and is
// omitted.
// Symbol: ?OnUpdateNewShortcutKey@CMFCToolBarsKeyboardPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnUpdateNewShortcutKey_CMFCToolBarsKeyboardPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_KbdPage* d = D(pThis);
    const ACCEL* pAccel = &d->m_wndNewKey.m_Accel;
    d->m_strAssignedTo.Empty();
    impl__ShowWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndAssignedToTitle), SW_HIDE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndAssignButton), FALSE);

    if (d->m_wndNewKey.m_bIsDefined != 0) {
        if (d->m_lpAccel == nullptr) { ThrowInvalidArg(); return; }
        bool bIsAlreadyDefined = false;
        for (int i = 0; !bIsAlreadyDefined && i < d->m_nAccelSize; i++) {
            const BYTE fRelFlags = FCONTROL | FALT | FSHIFT | FVIRTKEY;
            const ACCEL& entry = d->m_lpAccel[i];
            if (pAccel->key == entry.key && (pAccel->fVirt & fRelFlags) == (entry.fVirt & fRelFlags)) {
                void* pWndParent = ParentCustomizeDialog(pThis);
                if (pWndParent == nullptr) { ThrowInvalidArg(); return; }
                const wchar_t* lpszCommand = impl__GetCommandName_CMFCToolBarsCustomizeDialog__QEBAPEB_WI_Z(pWndParent, entry.cmd);
                d->m_strAssignedTo = (lpszCommand == nullptr) ? kUnknownCommand : lpszCommand;
                bIsAlreadyDefined = true;
            }
        }
        if (!bIsAlreadyDefined) {
            if (!EnsureLoadString(d->m_strAssignedTo, kIdpUnassigned)) return;
            impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndAssignButton), TRUE);
        }
        impl__ShowWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndAssignedToTitle), SW_SHOW);
    }
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);
}

// Retail (RVA 0x17b190, mfc140u), fully transcribed:
//     ENSURE(lpszCategory != NULL);
//     m_strAllCategory = lpszCategory;                // SetString(lpsz, wcslen) 0x2e30, tail jump
// Symbol: ?SetAllCategory@CMFCToolBarsKeyboardPropertyPage@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetAllCategory_CMFCToolBarsKeyboardPropertyPage__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory) {
    if (pThis == nullptr) return;
    if (lpszCategory == nullptr) { ThrowInvalidArg(); return; }
    D(pThis)->m_strAllCategory = lpszCategory;
}
