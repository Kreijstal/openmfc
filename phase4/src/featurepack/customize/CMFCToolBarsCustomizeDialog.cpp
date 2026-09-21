// CMFCToolBarsCustomizeDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
// FindListStorage / GetCollectionStates: the header-only readers of the side table
// in which OpenMFC's CObList / CStringList wrappers keep their contents
// (OPENMFC_DECLARE_LIST_WRAPPER in include/openmfc/afx.h).  Only the find path is
// instantiated here (see ListData / StringListData below); nothing links a C++
// collection method.
#include "detail/FilecoreSupport.h"

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <new>

// ===========================================================================
// CMFCToolBarsCustomizeDialog -- the feature-pack "Customize" property sheet.
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the RVA it was read from.  All addresses in this file are mfc140u.
//
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives.  It derives from CPropertySheet (the retail ctor
// at 0x176b80 calls ??0CPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z, not the
// CMFCPropertySheet ctor) and the retail object is 0xdb8 = 3512 bytes (the
// scalar deleting destructor at 0x177310 passes 0xdb8 to operator delete; the
// harvested descriptor in featurepack/customize/RuntimeClasses.cpp agrees).
// The member block after the 0x190-byte CPropertySheet base was read out of
// the ctor (0x176b80) and dtor (0x177360) and is pinned by S_CustDlg below;
// the names come from afxtoolbarscustomizedialog.h of the 14.51 SDK on this
// host, whose declaration order matches the offsets exactly.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftables at +0x00 (0x1803190d8),
//      in the CMap at +0x190 (0x180319010), the CStringList at +0x1c8
//      (0x18032a788) and the CList at +0x238 (0x180318fe0).  OpenMFC has no
//      MSVC-layout vtable for this class, so +0x00 keeps whatever the
//      CPropertySheet ctor thunk installs (a mingw vtable); the three
//      collections are OpenMFC objects placed at the retail offsets through
//      their exported ctor thunks (CStringList / CObList), except the CMap,
//      which is a file-local replica of the afxtempl.h layout with a NULL vfptr.
//      Consequences: a virtual call an MSVC client makes on a
//      CMFCToolBarsCustomizeDialog it allocated itself dispatches through the
//      mingw table (the tree-wide gap; see headerRequests), and the retail
//      bodies' own virtual calls are devirtualised here to the exported base
//      thunks (each site says so).
//
//  (2) OpenMFC's CObList / CStringList keep their nodes in a side table, so the
//      retail node walks (m_pNodeHead / +0x10 data) become FindListStorage
//      walks over the same objects.  m_ButtonsByCategory is reproduced with the
//      retail node layout (CatAssoc) because CMap<> is an inline template: a
//      client-derived class that touches this protected member runs its own
//      afxtempl.h code over these bytes.
//
//  (3) The seven built-in pages are allocated with the retail sizes and
//      constructed through the sibling ctor thunks, which are still
//      placeholders (featurepack/customize/CMFCToolBars*PropertyPage.cpp); the
//      blocks are zero-filled first so the unconstructed pages carry null
//      pointers instead of heap garbage.  Retail does not zero them.
//
//  (4) Frame internals retail reads inline are not modelled by OpenMFC's frame
//      classes (core/frame/CFrameWndEx.cpp, CMDIFrameWndEx.cpp: no embedded
//      CFrameImpl / CDockingManager): the CFrameImpl m_pMenuBar and
//      m_uiControlbarsMenuEntryID reads in the ctor, the mini-frame walk in
//      SetFrameCustMode, the menu-bar fallback in SetupFromMenus and the
//      "Windows..." button in AddMenuCommands are dropped, each with a note.
//
//  (5) afxContextMenuManager (0x3be1b0) and afxMouseManager (0x3be268) are
//      DLL-internal globals with no OpenMFC equivalent; following
//      core/frame/CMDIFrameWndEx.cpp the context-menu gate reads
//      CWinAppEx::m_pContextMenuManager and the mouse gate the CWinAppEx
//      "mouseManager" flag cannot be reached from here (see HaveMouseManager),
//      so the Mouse page is never added.  The exported
//      ?afxUserToolsManager@@ / ?afxKeyboardManager@@ pointers are read as
//      retail does; both are NULL in this tree and OpenMFC's CUserToolsManager
//      does not have the retail member layout, so the user-tool walks behind
//      those gates are not reproduced (see AddUserTools / AddButton).
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls (impl__ names copied from build-phase4/openmfc.def).
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage);
extern "C" void  MS_ABI impl___1CPropertySheet__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(CPropertySheet* pThis, CPropertyPage* pPage);
extern "C" void  MS_ABI impl__SetTitle_CPropertySheet__QEAAXPEB_WI_Z(CPropertySheet* pThis, const wchar_t* lpszText, unsigned int nStyle);
extern "C" int   MS_ABI impl__Create_CPropertySheet__UEAAHPEAVCWnd__KK_Z(CPropertySheet* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned long dwExStyle);
extern "C" int   MS_ABI impl__GetActiveIndex_CPropertySheet__QEBAHXZ(const CPropertySheet* pThis);
extern "C" CPropertyPage* MS_ABI impl__GetActivePage_CPropertySheet__QEBAPEAVCPropertyPage__XZ(const CPropertySheet* pThis);
extern "C" int   MS_ABI impl__GetPageIndex_CPropertySheet__QEAAHPEAVCPropertyPage___Z(CPropertySheet* pThis, CPropertyPage* pPage);
extern "C" int   MS_ABI impl__SetActivePage_CPropertySheet__QEAAHH_Z(CPropertySheet* pThis, int nPage);
extern "C" int   MS_ABI impl__OnInitDialog_CPropertySheet__UEAAHXZ(CPropertySheet* pThis);
extern "C" void  MS_ABI impl__OnClose_CPropertySheet__IEAAXXZ(CPropertySheet* pThis);
extern "C" int   MS_ABI impl__OnCommand_CPropertySheet__UEAAH_K_J_Z(CPropertySheet* pThis, unsigned __int64 wParam, __int64 lParam);

extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" int   MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" int   MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);
extern "C" int   MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" void  MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);
extern "C" int   MS_ABI impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(CWnd* pThis, HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int   MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" void* MS_ABI impl__GetActiveFrame_CFrameWnd__UEAAPEAV1_XZ(CFrameWnd* pThis);

extern "C" void* MS_ABI impl___0CMFCButton__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCButton__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(void* pThis, unsigned int nID, unsigned int nImage, int bChecked, unsigned int nCheckedImage);
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(void* pThis, CSize* pRet, int bCalcOnly);

extern "C" void* MS_ABI impl___0CMFCToolBarButton__QEAA_IHPEB_WHH_Z(void* pThis, unsigned int uiID, int iImage, const wchar_t* lpszText, int bUserButton, int bLocked);
extern "C" void  MS_ABI impl___1CMFCToolBarButton__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const void* pSrc);
extern "C" void  MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage);
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(void* pThis, unsigned int uiID, HMENU hMenu, int iImage, const wchar_t* lpszText, int bUserButton);
extern "C" void  MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__SetTearOff_CMFCToolBarMenuButton__UEAAXI_Z(void* pThis, unsigned int uiBarID);

extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void  MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);
extern "C" CObList::POSITION MS_ABI impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(CObList* pThis, CObList::POSITION* pPos, CObject* value);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION___J_Z(const CObList* pThis, long long nIndex);
extern "C" void  MS_ABI impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos);
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
extern "C" void  MS_ABI impl__RemoveAll_CObList__QEAAXXZ(CObList* pThis);
extern "C" void* MS_ABI impl___0CStringList__QEAA__J_Z(CStringList* pThis, long long nBlockSize);
extern "C" void  MS_ABI impl___1CStringList__UEAA_XZ(CStringList* pThis);
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CStringList* pThis, const wchar_t* value);
extern "C" CStringList::POSITION MS_ABI impl__Find_CStringList__QEBAPEAU__POSITION__PEB_WPEAU2__Z(const CStringList* pThis, const wchar_t* value, CStringList::POSITION* pStartAfter);
extern "C" void  MS_ABI impl__RemoveAll_CStringList__QEAAXXZ(CStringList* pThis);

extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex** ppHead, unsigned __int64 nMax, unsigned __int64 cbElement);
extern "C" void  MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* self);
extern "C" unsigned int MS_ABI impl____HashKey_PEB_W__YAIPEB_W_Z(const wchar_t* key);

extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis);
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" int   MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(const CRuntimeClass* pThis, const CRuntimeClass* pBase);
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void  MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc);
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszName, const wchar_t* lpszType);
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);
extern "C" int   MS_ABI impl__Remove___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W_Z(void* pThis, wchar_t ch);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();   // detail/RegcoreSupport.cpp (retail: AfxGetModuleState()->m_pCurrentWinApp)
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(const CWinApp* pThis);
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(const CWinApp* pThis, void** pos);
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU hMenu);
extern "C" int   MS_ABI impl__GetMenuStringW_CMenu__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(const CMenu* pThis, UINT nIDItem, CString& rString, UINT nFlags);
extern "C" int   MS_ABI impl__SetCustomizeMode_CMFCToolBar__SAHH_Z(int bSet);
extern "C" int   MS_ABI impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(CMenu* pThis, HMENU hMenu);      // core/window/CMenu.cpp
extern "C" int   MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu* pThis);                          // core/window/CMenu.cpp

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiDocTemplate__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();

// Sibling page thunks, declared with the signatures their mangled names describe
// (the definitions in featurepack/customize/CMFCToolBars*PropertyPage.cpp are
// still auto-generated placeholders whose parameter lists drop `this`; extern "C"
// linkage makes the mismatch harmless, and those bodies do nothing yet).
extern "C" void* MS_ABI impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarsListPropertyPage__QEAA_PEAVCFrameWnd___Z(void* pThis, void* pParentFrame);
extern "C" void* MS_ABI impl___0CMFCToolBarsKeyboardPropertyPage__QEAA_PEAVCFrameWnd__H_Z(void* pThis, void* pParentFrame, int bAutoSet);
extern "C" void* MS_ABI impl___0CMFCToolBarsMenuPropertyPage__QEAA_PEAVCFrameWnd__H_Z(void* pThis, void* pParentFrame, int bAutoSet);
extern "C" void* MS_ABI impl___0CMFCMousePropertyPage__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarsToolsPropertyPage__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarsOptionsPropertyPage__QEAA_H_Z(void* pThis, int bMenuBar);
extern "C" void  MS_ABI impl___1CMFCToolBarsCommandsPropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarsListPropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarsKeyboardPropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarsMenuPropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCMousePropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarsToolsPropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarsOptionsPropertyPage__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__SetAllCategory_CMFCToolBarsCommandsPropertyPage__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory);
extern "C" void  MS_ABI impl__SetAllCategory_CMFCToolBarsKeyboardPropertyPage__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory);
extern "C" void  MS_ABI impl__SetUserCategory_CMFCToolBarsCommandsPropertyPage__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory);
extern "C" void  MS_ABI impl__ShowToolBar_CMFCToolBarsListPropertyPage__QEAAXPEAVCMFCToolBar__H_Z(void* pThis, void* pToolBar, int bShow);

// This file's own thunks that earlier bodies call.
extern "C" void MS_ABI impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(void* pThis, const wchar_t* lpszCategory, const void* pButton, int iInsertBefore);
extern "C" void MS_ABI impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(void* pThis, const CMenu* pMenu, int bPopup, const wchar_t* lpszCategory, const wchar_t* lpszMenuPath);
extern "C" void MS_ABI impl__AddUserTools_CMFCToolBarsCustomizeDialog__IEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory);
extern "C" int  MS_ABI impl__AddToolBar_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszCategory, unsigned int uiToolbarResId);
extern "C" int  MS_ABI impl__RemoveButton_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszCategory, unsigned int uiCmdId);
extern "C" void MS_ABI impl__SetFrameCustMode_CMFCToolBarsCustomizeDialog__IEAAXH_Z(void* pThis, int bCustMode);
extern "C" void MS_ABI impl__SetupFromMenus_CMFCToolBarsCustomizeDialog__IEAAXXZ(void* pThis);

// Exported data objects read the way retail reads them.
extern "C" unsigned int impl__AFX_WM_CUSTOMIZEHELP__3IA;           // core/runtime/Globals.cpp
extern "C" unsigned int impl__AFX_WM_CUSTOMIZETOOLBAR__3IA;        // core/runtime/Globals.cpp
extern "C" void* impl__afxKeyboardManager__3PEAVCKeyboardManager__EA;   // featurepack/CMFC_misc_stubs.cpp (NULL)
extern "C" void* impl__afxUserToolsManager__3PEAVCUserToolsManager__EA; // featurepack/CMFC_misc_stubs.cpp (NULL)
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720]; // featurepack/CMFC_misc_stubs.cpp (zero blob, retail offsets)
extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);

// ?m_lstUnpermittedCommands@CMFCToolBar@@1V?$CList@II@@A is defined with the
// retail CList<UINT,UINT> layout in featurepack/toolbar/CMFCToolBar.cpp
// (UIntList there); this mirror lets AddButton walk its nodes the way the
// inlined CMFCToolBar::IsCommandPermitted at 0x17781f does.
struct CustUIntListNode {
    CustUIntListNode* pNext;   // +0x00
    CustUIntListNode* pPrev;   // +0x08
    UINT              data;    // +0x10
};
struct CustUIntList {
    void*             vfptr;         // +0x00
    CustUIntListNode* m_pNodeHead;   // +0x08
    CustUIntListNode* m_pNodeTail;   // +0x10
    INT_PTR           m_nCount;      // +0x18
    CustUIntListNode* m_pNodeFree;   // +0x20
    CPlex*            m_pBlocks;     // +0x28
    INT_PTR           m_nBlockSize;  // +0x30
};
static_assert(sizeof(CustUIntList) == 56 && sizeof(CustUIntListNode) == 24, "retail CList<UINT,UINT> shape");
extern "C" CustUIntList impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A;

// The DLL-internal CMFCToolBarsCustomizeDialog* at mfc140u .bss 0x3be3a0:
// OnCreate (0x1776c3) stores `this` into it, PostNcDestroy (0x1776e8) clears
// it, and CMFCToolBar::OnShowWindow (read at 0x15ad1d) and
// CMFCToolBarButtonCustomizeDialog::OnAddImage (0x15f211) test it.  It is not
// exported; nothing else in this tree models it (featurepack/toolbar/
// CMFCToolBar.cpp names it "the manager singleton (0x1803be3a0)"), so it is
// defined here with C linkage for those files to pick up.
extern "C" void* OpenMFC_g_pWndCustomize = nullptr;

// The mingw linker's image base symbol, used by DeleteRuntimeObject below.
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

// ---------------------------------------------------------------------------
// Retail layout (mfc140u).  Offsets: ctor 0x176b80 / dtor 0x177360.
// ---------------------------------------------------------------------------

// CMap<CString, LPCTSTR, CObList*, CObList*>::CAssoc -- afxtempl.h's
// `CAssoc : CPair { KEY key; VALUE value; } { CAssoc* pNext; UINT nHashValue; }`.
// Read out of the dtor's GetNextAssoc walk: key at +0 (CSimpleStringT::operator=
// from the node pointer at 0x177436), value at +8 (0x17743b), pNext at +0x10
// (0x177406), nHashValue at +0x18 (0x177417); the NewAssoc inlined into the
// SetAt instantiation (entry 0x310c4) hands 0x20 to CPlex::Create as the
// element size (the `mov $0x20,%r8d` at 0x31154 inside it).
struct CatAssoc {
    CString   key;          // +0x00
    CObList*  value;        // +0x08
    CatAssoc* pNext;        // +0x10
    UINT      nHashValue;   // +0x18
};
static_assert(sizeof(CatAssoc) == 0x20, "retail CAssoc is 32 bytes");

// The CMap itself (ctor stores at 0x176be7..0x176c23: vfptr, 0, 17, 0, 0, 0, 10).
struct CatMap {
    void*      vfptr;            // +0x00  retail 0x180319010; NULL here (deviation 1)
    CatAssoc** m_pHashTable;     // +0x08
    UINT       m_nHashTableSize; // +0x10  17
    INT_PTR    m_nCount;         // +0x18
    CatAssoc*  m_pFreeList;      // +0x20
    CPlex*     m_pBlocks;        // +0x28
    INT_PTR    m_nBlockSize;     // +0x30  10
};
static_assert(sizeof(CatMap) == 0x38, "retail CMap is 56 bytes");

struct S_CustDlg {
    unsigned char base[0x190];                  // CPropertySheet (OpenMFC sizeof == 400)
    CatMap        m_ButtonsByCategory;          // +0x190
    unsigned char m_strCategoriesList[0x38];    // +0x1c8 CStringList (retail 56 bytes; OpenMFC's 8-byte object sits at +0)
    void*         m_pCustomizePage;             // +0x200 CMFCToolBarsCommandsPropertyPage*
    void*         m_pToolbarsPage;              // +0x208 CMFCToolBarsListPropertyPage*
    void*         m_pKeyboardPage;              // +0x210 CMFCToolBarsKeyboardPropertyPage*
    void*         m_pMenuPage;                  // +0x218 CMFCToolBarsMenuPropertyPage*
    void*         m_pMousePage;                 // +0x220 CMFCMousePropertyPage*
    void*         m_pOptionsPage;               // +0x228 CMFCToolBarsOptionsPropertyPage*
    void*         m_pToolsPage;                 // +0x230 CMFCToolBarsToolsPropertyPage*
    unsigned char m_listCustomPages[0x38];      // +0x238 CList<CPropertyPage*,CPropertyPage*> (CObList-shaped; an OpenMFC CObList, whose live
                                                //        contents sit in the side table -- a client-derived class walking this inline CList
                                                //        with its own afxtempl.h code sees an empty list; deviation 2 applies here, unlike CatMap)
    CString       m_strAllCommands;             // +0x270
    CFrameWnd*    m_pParentFrame;               // +0x278
    int           m_bAutoSetFromMenus;          // +0x280
    unsigned int  m_uiFlags;                    // +0x284
    int           m_bSaveMenuAmps;              // +0x288  (m_uiFlags & AFX_CUSTOMIZE_MENUAMPERS) != 0
    int           _pad28c;                      // +0x28c
    unsigned char m_btnHelp[0xb20];             // +0x290 CMFCButton (retail 2848 bytes; OpenMFC's 296-byte object at +0)
    unsigned int  m_nPaneMenuEntryID;           // +0xdb0
    int           _paddb4;                      // +0xdb4
};
static_assert(sizeof(CPropertySheet) == 0x190, "CPropertySheet base is 0x190 bytes in retail (vfptr store at 0x176bee)");
static_assert(sizeof(CString) == sizeof(void*), "OpenMFC CString is one pointer, like ATL's");
static_assert(offsetof(S_CustDlg, m_ButtonsByCategory) == 0x190, "m_ButtonsByCategory");
static_assert(offsetof(S_CustDlg, m_strCategoriesList) == 0x1c8, "m_strCategoriesList");
static_assert(offsetof(S_CustDlg, m_pCustomizePage) == 0x200, "m_pCustomizePage");
static_assert(offsetof(S_CustDlg, m_pToolsPage) == 0x230, "m_pToolsPage");
static_assert(offsetof(S_CustDlg, m_listCustomPages) == 0x238, "m_listCustomPages");
static_assert(offsetof(S_CustDlg, m_strAllCommands) == 0x270, "m_strAllCommands");
static_assert(offsetof(S_CustDlg, m_pParentFrame) == 0x278, "m_pParentFrame");
static_assert(offsetof(S_CustDlg, m_bAutoSetFromMenus) == 0x280, "m_bAutoSetFromMenus");
static_assert(offsetof(S_CustDlg, m_uiFlags) == 0x284, "m_uiFlags");
static_assert(offsetof(S_CustDlg, m_bSaveMenuAmps) == 0x288, "m_bSaveMenuAmps");
static_assert(offsetof(S_CustDlg, m_btnHelp) == 0x290, "m_btnHelp");
static_assert(offsetof(S_CustDlg, m_nPaneMenuEntryID) == 0xdb0, "m_nPaneMenuEntryID");
static_assert(sizeof(S_CustDlg) == 0xdb8, "retail sizeof(CMFCToolBarsCustomizeDialog) == 3512");

// AFX_CUSTOMIZE_* (afxtoolbarscustomizedialog.h); the retail tests use the
// same masks (0x284 & 0x10 in OnCreate, & 0x20 in the ctor, & 0x8 in OnInitDialog).
constexpr unsigned int kCustNoHelp      = 0x0008;
constexpr unsigned int kCustContextHelp = 0x0010;
constexpr unsigned int kCustNoTools     = 0x0020;
constexpr unsigned int kCustMenuAmpers  = 0x0040;

// String / bitmap resource ids the retail bodies load (afxribbonres.h).
constexpr unsigned int kIdsPropShtCaption = 16103;   // 0x3ee7 IDS_AFXBARRES_PROPSHT_CAPTION
constexpr unsigned int kIdsAllCommands    = 16110;   // 0x3eee IDS_AFXBARRES_ALL_COMMANDS
constexpr unsigned int kIdsNewMenu        = 16014;   // 0x3e8e IDS_AFXBARRES_NEW_MENU
constexpr unsigned int kIdsClose          = 16105;   // 0x3ee9 IDS_AFXBARRES_CLOSE
constexpr unsigned int kIdbHelp           = 16131;   // 0x3f03 IDB_AFXBARRES_HELP
constexpr unsigned int kIdbHelp32         = 17067;   // 0x42ab IDB_AFXBARRES_HELP32

// Retail page sizes: the operator new arguments in the ctor.
constexpr std::size_t kSizeCommandsPage = 0x350;
constexpr std::size_t kSizeToolbarsPage = 0x718;
constexpr std::size_t kSizeKeyboardPage = 0x9f8;
constexpr std::size_t kSizeMenuPage     = 0xac0;
constexpr std::size_t kSizeMousePage    = 0x430;
constexpr std::size_t kSizeToolsPage    = 0x1f20;
constexpr std::size_t kSizeOptionsPage  = 0x7c8;

// The retail image, RT_TOOLBAR resource layout AddToolBar parses.
struct ToolBarResData {
    WORD wVersion;    // +0
    WORD wWidth;      // +2
    WORD wHeight;     // +4
    WORD wItemCount;  // +6
    WORD items[1];    // +8
};

// The client's CList<CRuntimeClass*, CRuntimeClass*> the ctor walks directly
// (afxtempl.h layout; head at +8, node data at +0x10 -- 0x176cf4 / 0x176cfa).
struct RtcListNode {
    RtcListNode*   pNext;
    RtcListNode*   pPrev;
    CRuntimeClass* data;
};
struct RtcList {
    void*        vfptr;
    RtcListNode* m_pNodeHead;
};

inline S_CustDlg* D(void* pThis) { return static_cast<S_CustDlg*>(pThis); }
inline const S_CustDlg* D(const void* pThis) { return static_cast<const S_CustDlg*>(pThis); }
inline CPropertySheet* Sheet(void* pThis) { return static_cast<CPropertySheet*>(pThis); }
inline CWnd* Wnd(void* pThis) { return static_cast<CWnd*>(pThis); }
inline CStringList* CategoryList(S_CustDlg* d) { return reinterpret_cast<CStringList*>(d->m_strCategoriesList); }
inline const CStringList* CategoryList(const S_CustDlg* d) { return reinterpret_cast<const CStringList*>(d->m_strCategoriesList); }
inline CObList* CustomPages(S_CustDlg* d) { return reinterpret_cast<CObList*>(d->m_listCustomPages); }
inline CWnd* HelpButton(S_CustDlg* d) { return reinterpret_cast<CWnd*>(d->m_btnHelp); }
inline HWND WndHandle(const CWnd* p) { return p ? p->m_hWnd : nullptr; }
inline CMFCToolBarButton* Btn(CObject* p) { return static_cast<CMFCToolBarButton*>(p); }

// OpenMFC list contents (deviation 2).
typedef CList<CObject*, CObject*> ObListData;
typedef CList<CString, const CString&> StrListData;
inline const ObListData* ListData(const CObList* p) {
    return p ? openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(p) : nullptr;
}
inline ObListData* ListData(CObList* p) { return const_cast<ObListData*>(ListData(static_cast<const CObList*>(p))); }
inline const StrListData* StringListData(const CStringList* p) {
    return p ? openmfc::detail::filecore::FindListStorage<CStringList, CString, const CString&>(p) : nullptr;
}
inline StrListData* StringListData(CStringList* p) { return const_cast<StrListData*>(StringListData(static_cast<const CStringList*>(p))); }
inline INT_PTR ListCount(const CObList* p) { const ObListData* l = ListData(p); return l ? l->GetCount() : 0; }

// ENSURE(x) / ATLENSURE(x): retail throws through
// ?AfxThrowInvalidArgException@@ (0x227720) or the AtlThrow shim at 0x333c,
// which is AfxThrowOleException(hr).
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }
inline void ThrowOle(long hr) { impl__AfxThrowOleException__YAXJ_Z(hr); }
constexpr long kEFail = static_cast<long>(0x80004005);
constexpr long kEInvalidArg = static_cast<long>(0x80070057);

// Retail's `CString str; ENSURE(str.LoadString(nID))` idiom
// (AfxFindStringResourceHandle 0x2aee00 + CStringT::LoadString 0xdb70, throw on
// either failure).  Returns false after throwing so callers can bail.
bool LoadResString(CString& str, unsigned int nID) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nID)) {
        ThrowInvalidArg();
        return false;
    }
    return true;
}

inline void StrRemoveChar(CString& str, wchar_t ch) {
    impl__Remove___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W_Z(&str, ch);
}

// A pointer that lies inside this DLL's own image (a mingw vtable) as opposed
// to an MSVC client's image.
bool PointsIntoThisImage(const void* p) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* q = static_cast<const unsigned char*>(p);
    return q >= base && q < base + nt->OptionalHeader.SizeOfImage;
}

// Retail deletes the objects it obtained from CRuntimeClass::CreateObject
// (buttons, custom pages) through vftable slot 1 -- the virtual deleting
// destructor -- with flag 1 (e.g. 0x17745b in the dtor, 0x177ee8 in
// RemoveButton).  Two kinds of object reach this DLL: ones a client factory
// built (MSVC vtable: slot 1 is that deleting destructor) and ones an OpenMFC
// factory built (mingw vtable: slot 1 is the complete destructor and slot 2 the
// deleting one, so a raw slot-1 call would leak and `delete` is the right
// spelling).  The vtable's home image tells them apart.
void DeleteRuntimeObject(CObject* p) {
    if (p == nullptr) return;
    const void* vptr = *reinterpret_cast<void* const*>(p);
    if (vptr == nullptr) { impl___3_YAXPEAX_Z(p); return; }   // never constructed: only the block to give back
    if (PointsIntoThisImage(vptr)) {
        delete p;
        return;
    }
    typedef void* (MS_ABI *DeletingDtor)(void*, unsigned int);
    reinterpret_cast<DeletingDtor const*>(vptr)[1](p, 1u);
}

// Retail deletes the seven built-in pages the same way (vslot 1, flag 1 --
// 0x1774c8 .. 0x177570).  They were allocated by the ctor below with
// ??2@YAPEAX_K@Z and constructed by the sibling ctor thunks, so they go back
// through the sibling dtor thunk and ??3@YAXPEAX@Z (deviation 3).
typedef void (MS_ABI *PageDtorThunk)(void*);
void DeletePage(void*& pPage, PageDtorThunk dtor) {
    if (pPage == nullptr) return;
    dtor(pPage);
    impl___3_YAXPEAX_Z(pPage);
    pPage = nullptr;
}

void* NewPage(std::size_t size) {
    void* p = impl___2_YAPEAX_K_Z(size);
    if (p) std::memset(p, 0, size);   // deviation 3
    return p;
}

// Retail `new CObList` (inline ctor at 0x177b00: vftable 0x18032a6f8,
// head/tail/count/free/blocks 0, block size 10) and the matching delete
// through vslot 1 (0x177475).
CObList* NewObList() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(CObList));
    if (p == nullptr) return nullptr;
    std::memset(p, 0, sizeof(CObList));
    impl___0CObList__QEAA__J_Z(static_cast<CObList*>(p), 10);
    return static_cast<CObList*>(p);
}
void DeleteObList(CObList* p) {
    if (p == nullptr) return;
    impl___1CObList__UEAA_XZ(p);
    impl___3_YAXPEAX_Z(p);
}

// ---------------------------------------------------------------------------
// m_ButtonsByCategory: the afxtempl.h CMap<CString,LPCTSTR,CObList*,CObList*>
// code retail inlines / instantiates, over the CatMap / CatAssoc layout above.
// ---------------------------------------------------------------------------

// CMap::GetAssocAt (0x314e0): HashKey<LPCWSTR> (the exported
// ??$HashKey@PEB_W@@ -- RenameCategory calls it at 0x17824e), bucket =
// hash % m_nHashTableSize, then CompareElements<CString,LPCTSTR> on the chain.
CatAssoc* CatGetAssocAt(const CatMap& m, const wchar_t* key, UINT& nHash, UINT& nBucket) {
    nHash = impl____HashKey_PEB_W__YAIPEB_W_Z(key);
    nBucket = m.m_nHashTableSize ? nHash % m.m_nHashTableSize : 0;
    if (m.m_pHashTable == nullptr) return nullptr;
    for (CatAssoc* p = m.m_pHashTable[nBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHash && std::wcscmp(p->key.GetString(), key ? key : L"") == 0) return p;
    }
    return nullptr;
}

// CMap::Lookup (0x179a60): GetAssocAt, then rValue = assoc->value (+8).
bool CatLookup(const CatMap& m, const wchar_t* key, CObList*& rValue) {
    UINT nHash, nBucket;
    CatAssoc* p = CatGetAssocAt(m, key, nHash, nBucket);
    if (p == nullptr) return false;
    rValue = p->value;
    return true;
}

// CMap::RemoveAll (0x3a400): release every key CString, free the bucket
// array, zero count / free list, CPlex::FreeDataChain, zero m_pBlocks.
void CatRemoveAll(CatMap& m) {
    if (m.m_pHashTable != nullptr) {
        for (UINT i = 0; i < m.m_nHashTableSize; i++) {
            for (CatAssoc* p = m.m_pHashTable[i]; p != nullptr; p = p->pNext) {
                p->key.~CString();
            }
        }
        impl___3_YAXPEAX_Z(m.m_pHashTable);
        m.m_pHashTable = nullptr;
    }
    m.m_nCount = 0;
    m.m_pFreeList = nullptr;
    if (m.m_pBlocks != nullptr) impl__FreeDataChain_CPlex__QEAAXXZ(m.m_pBlocks);
    m.m_pBlocks = nullptr;
}

// CMap::SetAt as instantiated at 0x310c4 (operator[] inlined into it; the
// value store `mov %rbp,0x8(%rsi)` is at 0x3111e8): GetAssocAt; if missing,
// InitHashTable(m_nHashTableSize) (operator new of size*8, zeroed) when there is
// no table yet, ENSURE(m_pHashTable), NewAssoc (CPlex::Create(m_pBlocks,
// m_nBlockSize, 0x20) chained into the free list), memset the assoc to 0,
// m_nCount++, construct the key, store the hash, link at the bucket head, then
// assoc->value = value.
CatAssoc* CatSetAt(CatMap& m, const wchar_t* key, CObList* value) {
    UINT nHash, nBucket;
    CatAssoc* p = CatGetAssocAt(m, key, nHash, nBucket);
    if (p == nullptr) {
        if (m.m_pHashTable == nullptr) {
            if (m.m_nHashTableSize == 0) m.m_nHashTableSize = 17;
            const std::size_t bytes = static_cast<std::size_t>(m.m_nHashTableSize) * sizeof(CatAssoc*);
            m.m_pHashTable = static_cast<CatAssoc**>(impl___2_YAPEAX_K_Z(bytes));
            if (m.m_pHashTable == nullptr) { ThrowInvalidArg(); return nullptr; }
            std::memset(m.m_pHashTable, 0, bytes);
            nBucket = nHash % m.m_nHashTableSize;
        }
        if (m.m_pFreeList == nullptr) {
            CPlex* pBlock = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&m.m_pBlocks,
                                                                     static_cast<unsigned __int64>(m.m_nBlockSize),
                                                                     sizeof(CatAssoc));
            if (pBlock == nullptr) { ThrowInvalidArg(); return nullptr; }
            // CPlex::data() == this + 1: the element block follows the 8-byte header
            // (detail/CPlexSupport.h; retail's NewAssoc addresses the last assoc as
            // plex + 0x20 * m_nBlockSize - 0x18 at 0x31166, i.e. the same +8 data start).
            CatAssoc* pAssoc = reinterpret_cast<CatAssoc*>(reinterpret_cast<unsigned char*>(pBlock) + 8) + m.m_nBlockSize - 1;
            for (INT_PTR i = m.m_nBlockSize - 1; i >= 0; i--, pAssoc--) {
                pAssoc->pNext = m.m_pFreeList;
                m.m_pFreeList = pAssoc;
            }
        }
        p = m.m_pFreeList;
        if (p == nullptr) { ThrowInvalidArg(); return nullptr; }
        m.m_pFreeList = p->pNext;
        std::memset(static_cast<void*>(p), 0, sizeof(CatAssoc));
        m.m_nCount++;
        new (&p->key) CString(key ? key : L"");
        p->nHashValue = nHash;
        p->pNext = m.m_pHashTable[nBucket];
        m.m_pHashTable[nBucket] = p;
    }
    p->value = value;
    return p;
}

// CMap::RemoveKey as inlined in RenameCategory (0x17823b .. 0x1782ce): find
// the assoc in its bucket, unlink it, destroy the key, push it on the free list,
// m_nCount--, and RemoveAll once the map is empty.
bool CatRemoveKey(CatMap& m, const wchar_t* key) {
    if (m.m_pHashTable == nullptr) return false;
    const UINT nHash = impl____HashKey_PEB_W__YAIPEB_W_Z(key);
    CatAssoc** ppPrev = &m.m_pHashTable[nHash % m.m_nHashTableSize];
    for (CatAssoc* p = *ppPrev; p != nullptr; ppPrev = &p->pNext, p = p->pNext) {
        if (p->nHashValue == nHash && std::wcscmp(p->key.GetString(), key ? key : L"") == 0) {
            *ppPrev = p->pNext;
            p->key.~CString();
            p->pNext = m.m_pFreeList;
            m.m_pFreeList = p;
            m.m_nCount--;
            if (m.m_nCount == 0) CatRemoveAll(m);
            return true;
        }
    }
    return false;
}

// GetStartPosition / GetNextAssoc as the dtor inlines them (0x1773c2 ..
// 0x17742e): visit every assoc, bucket by bucket.
template <typename F>
void CatForEach(CatMap& m, F&& f) {
    if (m.m_pHashTable == nullptr) return;
    for (UINT i = 0; i < m.m_nHashTableSize; i++) {
        for (CatAssoc* p = m.m_pHashTable[i]; p != nullptr;) {
            CatAssoc* pNext = p->pNext;
            f(p);
            p = pNext;
        }
    }
}

// CMFCToolBarButton::IsEditable() as the retail vftable slot 26 body reads
// (RVA 0x23760, mfc140u); AddButton dispatches it at 0x17780c.  Same
// transcription and the same two dropped tests as
// featurepack/toolbar/CMFCToolBarComboBoxButton.cpp::IsEditableInline:
//     UINT id = m_nID;
//     if (id - 0xE110 <= 0xF)   return FALSE;   // ID_FILE_MRU_FILE1..16
//     if (id - 0xF000 <= 0x1EF) return FALSE;
//     if ((int)id >= 0xFF00)    return FALSE;
//     if (id - 0xE210 <= 0xF)   return FALSE;
//     if (afxUserToolsManager && afxUserToolsManager->m_uiCmdToolsDummy (+0x40) == id) return FALSE;
//     if (CMFCToolBarButton::m_lstProtectedCommands.Find(id)) return FALSE;   // 0x11f1c
//     return TRUE;
// DEVIATION: the last two tests are dropped -- the exported
// ?afxUserToolsManager@@ is always NULL here and OpenMFC's CUserToolsManager has
// no +0x40 member, and ?m_lstProtectedCommands@ is defined as an 8-byte
// placeholder (featurepack/toolbar/CMFCToolBarButton.cpp), not a CList.
bool ButtonIsEditable(const CMFCToolBarButton* p) {
    const unsigned id = p->m_nID;
    if (id - 0xE110u <= 0xFu) return false;
    if (id - 0xF000u <= 0x1EFu) return false;
    if (static_cast<int>(id) >= 0xFF00) return false;
    if (id - 0xE210u <= 0xFu) return false;
    return true;
}

// CMFCToolBar::IsCommandPermitted as inlined at 0x17781f..0x17782e:
// m_lstUnpermittedCommands.Find(uiCmd) == NULL (the CList<UINT,UINT>::Find
// instantiation at 0x11f1c walks the nodes from m_pNodeHead).
bool IsCommandPermitted(UINT uiCmd) {
    for (const CustUIntListNode* n = impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A.m_pNodeHead;
         n != nullptr; n = n->pNext) {
        if (n->data == uiCmd) return false;
    }
    return true;
}

// The retail `CWaitCursor wait;` (AfxGetModuleState()->m_pCurrentWinApp ->
// BeginWaitCursor 0x1de7b0 / EndWaitCursor via the dtor at 0x7687c), the way
// featurepack/controls/CMFCShellTreeCtrl.cpp spells it.
struct WaitCursorScope {
    CWinApp* pApp;
    WaitCursorScope() : pApp(impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    ~WaitCursorScope() {
        if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
};

// A CMFCToolBarButton built on the stack through the exported ctor / dtor, the
// way the retail bodies build theirs (??0CMFCToolBarButton@@QEAA@IHPEB_WHH@Z
// 0x15bf40 / ??1 0x15c130).
struct StackButton {
    alignas(CMFCToolBarButton) unsigned char bytes[sizeof(CMFCToolBarButton)];
    StackButton(unsigned int uiID, int iImage, const wchar_t* lpszText, int bUserButton, int bLocked) {
        std::memset(bytes, 0, sizeof(bytes));
        impl___0CMFCToolBarButton__QEAA_IHPEB_WHH_Z(bytes, uiID, iImage, lpszText, bUserButton, bLocked);
    }
    ~StackButton() { impl___1CMFCToolBarButton__UEAA_XZ(bytes); }
    CMFCToolBarButton* get() { return reinterpret_cast<CMFCToolBarButton*>(bytes); }
};
struct StackMenuButton {
    alignas(CMFCToolBarMenuButton) unsigned char bytes[sizeof(CMFCToolBarMenuButton)];
    StackMenuButton(unsigned int uiID, HMENU hMenu, int iImage, const wchar_t* lpszText, int bUserButton) {
        std::memset(bytes, 0, sizeof(bytes));
        impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(bytes, uiID, hMenu, iImage, lpszText, bUserButton);
    }
    ~StackMenuButton() { impl___1CMFCToolBarMenuButton__UEAA_XZ(bytes); }
    CMFCToolBarButton* get() { return reinterpret_cast<CMFCToolBarButton*>(bytes); }
};

// Retail's afxContextMenuManager (0x3be1b0) / afxMouseManager (0x3be268)
// gates (deviation 5).  CWinAppEx::m_pContextMenuManager is protected;
// core/frame/CMDIFrameWndEx.cpp reads it the same way.
struct WinAppExAccess : CWinAppEx {
    using CWinAppEx::m_pContextMenuManager;
};
CWinAppEx* CurrentWinAppEx() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<CWinAppEx*>(pApp);
}
bool HaveContextMenuManager() {
    CWinAppEx* pApp = CurrentWinAppEx();
    return pApp != nullptr && static_cast<WinAppExAccess*>(pApp)->m_pContextMenuManager != nullptr;
}
// afxMouseManager has no reachable equivalent at all: OpenMFC records
// InitMouseManager only in detail/MfccoreSupport.h's g_winAppExStates, a C++
// global this file cannot reference (the link audit rejects the new C++
// undefined), and the exported ?GetMouseManager@CWinAppEx@@ creates one on
// demand, which would turn retail's "none exists" gate into "always".  The
// gate therefore reads FALSE here (headerRequests asks for a C-linkage reader).
bool HaveMouseManager() { return false; }

// OpenMFC's CWnd keeps m_hWnd where retail does (every retail body below reads
// it as this+0x40); CMenu::m_hMenu likewise (+0x8).
struct WndAccess : CWnd { using CWnd::m_hWnd; };
static_assert(offsetof(WndAccess, m_hWnd) == 0x40, "CWnd::m_hWnd is at +0x40 (retail reads 0x40(%rcx) in every body below)");
struct MenuAccess : CMenu { using CMenu::m_hMenu; };
static_assert(offsetof(MenuAccess, m_hMenu) == 0x8 && sizeof(CMenu) == 0x10, "CMenu is { vfptr, m_hMenu }");
inline HMENU MenuHandle(const CMenu* p) { return p ? static_cast<const MenuAccess*>(p)->m_hMenu : nullptr; }

// Retail CFrameWnd keeps HMENU m_hMenuDefault at +0xf0 and HACCEL m_hAccelTable
// at +0xf8 (SetupFromMenus reads 0xf0(%rcx) at 0x17981b, the ctor 0xf8 at
// 0x176e4c).  OpenMFC's CFrameWnd (include/openmfc/afxwin.h) has NO
// m_hMenuDefault and keeps m_hAccelTable at +0xf0 (featurepack/menu/
// CMFCMenuBar.cpp says the same), and core/frame/CFrameWnd.cpp records the
// loaded menu only by ::SetMenu on the window.  The frame's current window
// menu is therefore the closest reachable equivalent of m_hMenuDefault
// (retail's LoadFrame stores exactly ::GetMenu(m_hWnd) there); it differs when
// a CMFCMenuBar has taken the menu off the window (then retail still has the
// handle, this side gets NULL).  DEVIATION, see headerRequests.
inline HMENU FrameDefaultMenu(const CFrameWnd* pFrame) {
    HWND hWnd = WndHandle(pFrame);
    return hWnd ? ::GetMenu(hWnd) : nullptr;
}

// Retail CMultiDocTemplate::m_hMenuShared is at +0xf0 (SetupFromMenus reads it
// at 0x1797e1).  On this side the slot lies in the unnamed
// _multidoctemplate_padding that no OpenMFC body writes or zeroes
// (featurepack/menu/CMFCMenuBar.cpp, kOffMultiDocTemplateMenuShared), so the
// value is validated with ::IsMenu before use where retail only tests NULL --
// for a retail-valid value (NULL or a live HMENU) the two tests agree.
constexpr std::size_t kOffMultiDocTemplateMenuShared = 0xf0;
static_assert(sizeof(CDocTemplate) == kOffMultiDocTemplateMenuShared, "CMultiDocTemplate's own members start at 0xf0");
static_assert(sizeof(CMultiDocTemplate) == 0x140, "retail CMultiDocTemplate m_nObjectSize");
inline HMENU MultiDocTemplateSharedMenu(const CDocTemplate* pTemplate) {
    HMENU h = nullptr;
    std::memcpy(&h, reinterpret_cast<const unsigned char*>(pTemplate) + kOffMultiDocTemplateMenuShared, sizeof h);
    return (h != nullptr && ::IsMenu(h)) ? h : nullptr;
}

// CMFCToolBarsListPropertyPage::m_bUserDefinedToolbars: retail
// EnableUserDefinedToolbars (0x178d60) stores its argument at page+0x708.  The
// page block is the retail 0x718 bytes (kSizeToolbarsPage), zero-filled by the
// ctor above, so the slot is in bounds; the sibling ctor thunk is still a
// placeholder and models no layout of its own.
constexpr std::size_t kOffToolbarsPageUserDefined = 0x708;
static_assert(kOffToolbarsPageUserDefined + sizeof(int) <= kSizeToolbarsPage, "inside the retail page block");

// RT_TOOLBAR / RT_MENU resource types as the retail bodies pass them
// (0xf1 at 0x177fe4, 4 at 0x1780e9).
constexpr unsigned short kRtToolbar = 241;
constexpr unsigned short kRtMenu    = 4;

// AFX_WM_CUSTOMIZEHELP to the parent frame, as OnHelpInfo (0x1796d0) and the
// IDHELP arm of OnCommand (0x178cb3) both spell it:
//     m_pParentFrame->SendMessage(AFX_WM_CUSTOMIZEHELP, GetActiveIndex(), (LPARAM)this);
// Retail reads m_pParentFrame->m_hWnd unchecked; a NULL frame is skipped here.
inline void SendCustomizeHelp(S_CustDlg* d) {
    HWND hFrame = WndHandle(d->m_pParentFrame);
    if (hFrame == nullptr) return;
    const int nActive = impl__GetActiveIndex_CPropertySheet__QEBAHXZ(Sheet(d));
    ::SendMessage(hFrame, impl__AFX_WM_CUSTOMIZEHELP__3IA,
                  static_cast<WPARAM>(static_cast<INT_PTR>(nActive)), reinterpret_cast<LPARAM>(d));
}

// The retail "tools page must validate before the sheet closes" gate that
// OnClose (0x178c0a..0x178c69) and the IDCANCEL arm of OnCommand
// (0x178cd7..0x178d33) share:
//     if (afxUserToolsManager != NULL && m_pToolsPage != NULL &&
//         !CheckToolsValidity(afxUserToolsManager->m_lstUserTools /* +0x8 */)) {   // vslot 0x328 (101)
//         if (GetActivePage() != m_pToolsPage) { ENSURE(m_pToolsPage); SetActivePage(GetPageIndex(m_pToolsPage)); }
//         return TRUE;   // "handled, do not close"
//     }
// CheckToolsValidity is a virtual whose retail base body is `return TRUE`
// (vftable 0x1803190d8 slot 101 -> 0x3a60, `mov eax,1; ret`).  It is
// dispatched through the object's vtable only when that vtable is a client's
// (outside this image: an MSVC-derived class laid out from the SDK header, so
// slot 101 is its override); the mingw vtable the base ctor thunk leaves at
// +0 has no such slot and stands for the base body.  The exported
// ?afxUserToolsManager@@ is NULL in this tree, so the gate is never taken.
typedef int (MS_ABI *CheckToolsValidityFn)(void* pThis, const void* pListTools);
bool ToolsPageBlocksClose(S_CustDlg* d) {
    void* pMgr = impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
    if (pMgr == nullptr || d->m_pToolsPage == nullptr) return false;
    const void* pListTools = static_cast<const unsigned char*>(pMgr) + 8;
    int bValid = TRUE;
    const void* vptr = *reinterpret_cast<void* const*>(d);
    if (vptr != nullptr && !PointsIntoThisImage(vptr)) {
        bValid = reinterpret_cast<CheckToolsValidityFn const*>(vptr)[101](d, pListTools);
    }
    if (bValid) return false;
    if (impl__GetActivePage_CPropertySheet__QEBAPEAVCPropertyPage__XZ(Sheet(d)) != d->m_pToolsPage) {
        const int nPage = impl__GetPageIndex_CPropertySheet__QEAAHPEAVCPropertyPage___Z(Sheet(d), static_cast<CPropertyPage*>(d->m_pToolsPage));
        impl__SetActivePage_CPropertySheet__QEAAHH_Z(Sheet(d), nPage);
    }
    return true;
}

// A CMenu built on the stack the way retail's inline CMenu ctor does
// (vfptr + NULL m_hMenu, 0x1780d3..0x1780de); the exported Attach and
// DestroyMenu thunks replace the inlined Attach / ~CMenu.  ??0CMenu / ??1CMenu
// are not exports (inline in afxwin.h), and OpenMFC's C++ CMenu ctor/dtor are
// not linkable from here, hence the raw block (NULL vfptr; only m_hMenu is read).
struct StackMenu {
    alignas(CMenu) unsigned char bytes[sizeof(CMenu)];
    StackMenu() { std::memset(bytes, 0, sizeof(bytes)); }
    ~StackMenu() { impl__DestroyMenu_CMenu__QEAAHXZ(get()); }   // retail: if (m_hMenu) { handle-map RemoveKey; ::DestroyMenu } (0x178136..0x178173)
    CMenu* get() { return reinterpret_cast<CMenu*>(bytes); }
};

} // namespace

// ===========================================================================
// Constructor / destructor
// ===========================================================================

// Retail (RVA 0x176b80, mfc140u), transcribed:
//     CPropertySheet(L"" /*0x33d19c*/, pWndParentFrame, 0);              // 0x216c90
//     vfptr = 0x1803190d8;  m_ButtonsByCategory / m_strCategoriesList / m_listCustomPages inline ctors
//     m_strAllCommands = nil string;  m_pParentFrame = pWndParentFrame;   // 0x278
//     m_bAutoSetFromMenus = bAutoSetFromMenus;  m_uiFlags = uiFlags;      // 0x280 / 0x284
//     CMFCButton::CMFCButton(&m_btnHelp);                                 // 0x1c930
//     m_nPaneMenuEntryID = 0;                                             // 0xdb0
//     m_bSaveMenuAmps = (m_uiFlags >> 6) & 1;                             // AFX_CUSTOMIZE_MENUAMPERS
//     if (plistCustomPages) for each CRuntimeClass* in it: m_listCustomPages.AddTail(pRTC->CreateObject());   // 0x234d60 / 0x7908
//     ENSURE(pWndParentFrame != NULL);                                    // 0x1772f5
//     m_pCustomizePage = new CMFCToolBarsCommandsPropertyPage;            // 0x350 bytes, 0x17b6d0
//     m_pToolbarsPage  = new CMFCToolBarsListPropertyPage(m_pParentFrame);            // 0x718, 0x17bf10
//     m_pKeyboardPage  = new CMFCToolBarsKeyboardPropertyPage(m_pParentFrame, m_bAutoSetFromMenus);   // 0x9f8, 0x179b90
//     m_pMenuPage      = new CMFCToolBarsMenuPropertyPage(m_pParentFrame, m_bAutoSetFromMenus);       // 0xac0, 0x17d030
//     m_pMousePage     = new CMFCMousePropertyPage;                       // 0x430, 0x91270
//     AddPage(m_pCustomizePage);  AddPage(m_pToolbarsPage);               // 0x217ce0 at 0x176e1f (+0x200) then 0x176e2e (+0x208)
//     if (m_uiFlags & AFX_CUSTOMIZE_NOTOOLS) m_pToolsPage = NULL;
//     else { m_pToolsPage = new CMFCToolBarsToolsPropertyPage;            // 0x1f20, 0x17fce0
//            if (afxUserToolsManager) { ENSURE(m_pToolsPage); AddPage(m_pToolsPage); } }   // inlined AddPage, 0x176ede..0x177024
//     if (afxKeyboardManager && m_pParentFrame->m_hAccelTable /*+0xf8*/) AddPage(m_pKeyboardPage);
//     BOOL bMenuBar = FALSE;
//     if (m_pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))       // 0x2ee478
//         { bMenuBar = frame->m_Impl.m_pMenuBar (+0x6a0) != NULL; m_nPaneMenuEntryID = frame->m_Impl.m_uiControlbarsMenuEntryID (+0x5ac); }
//     else if (m_pParentFrame->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))     // 0x2e9210
//         { bMenuBar = +0x300 != NULL; m_nPaneMenuEntryID = +0x20c; }
//     if (afxContextMenuManager /*0x3be1b0*/ || bMenuBar) AddPage(m_pMenuPage);
//     if (afxMouseManager /*0x3be268*/) AddPage(m_pMousePage);
//     for each page in m_listCustomPages: AddPage(page);
//     m_pOptionsPage = new CMFCToolBarsOptionsPropertyPage(bMenuBar);    // 0x7c8, 0x17e990
//     AddPage(m_pOptionsPage);
//     CString strTitle;  ENSURE(strTitle.LoadString(IDS_AFXBARRES_PROPSHT_CAPTION));      // 0x3ee7
//     ENSURE(m_strAllCommands.LoadString(IDS_AFXBARRES_ALL_COMMANDS));                     // 0x3eee
//     m_pCustomizePage->SetAllCategory(m_strAllCommands);                                  // 0x17bed0
//     if (m_pKeyboardPage) m_pKeyboardPage->SetAllCategory(m_strAllCommands);              // 0x17b190
//     SetTitle(strTitle, 0);                                                               // 0x217280
//     if (m_bAutoSetFromMenus) SetupFromMenus();                                           // 0x179770
//     CString strNewMenu;  ENSURE(strNewMenu.LoadString(IDS_AFXBARRES_NEW_MENU));         // 0x3e8e
//     CMFCToolBarMenuButton btnNewMenu(0, NULL, -1, strNewMenu, FALSE);                    // 0x172870
//     AddButton(strNewMenu, btnNewMenu, -1);                                               // 0x1777c0
// DEVIATIONS: the two CFrameImpl reads are not modelled by OpenMFC's frame
// classes (deviation 4): bMenuBar stays FALSE and m_nPaneMenuEntryID 0.  The
// m_hAccelTable test reads OpenMFC's CFrameWnd member (at +0xf0 here, retail
// +0xf8 -- see FrameDefaultMenu), i.e. the handle OpenMFC's LoadAccelTable
// stores, which is the same value retail tests.  The
// afxContextMenuManager / afxMouseManager gates are deviation 5.  The page
// blocks are zero-filled (deviation 3).  Retail's `new` failure leaves the
// page pointer NULL and continues; so does this.
// Symbol: ??0CMFCToolBarsCustomizeDialog@@QEAA@PEAVCFrameWnd@@HIPEAV?$CList@PEAUCRuntimeClass@@PEAU1@@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsCustomizeDialog__QEAA_PEAVCFrameWnd__HIPEAV__CList_PEAUCRuntimeClass__PEAU1____Z(
    void* pThis, CFrameWnd* pWndParentFrame, int bAutoSetFromMenus, unsigned int uiFlags, void* plistCustomPages) {
    if (pThis == nullptr) return nullptr;
    S_CustDlg* d = D(pThis);

    impl___0CPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(pThis, L"", pWndParentFrame, 0);

    std::memset(&d->m_ButtonsByCategory, 0, sizeof(S_CustDlg) - offsetof(S_CustDlg, m_ButtonsByCategory));
    d->m_ButtonsByCategory.m_nHashTableSize = 17;
    d->m_ButtonsByCategory.m_nBlockSize = 10;
    impl___0CStringList__QEAA__J_Z(CategoryList(d), 10);
    impl___0CObList__QEAA__J_Z(CustomPages(d), 10);
    new (&d->m_strAllCommands) CString();
    d->m_pParentFrame = pWndParentFrame;
    d->m_bAutoSetFromMenus = bAutoSetFromMenus;
    d->m_uiFlags = uiFlags;
    impl___0CMFCButton__QEAA_XZ(d->m_btnHelp);
    d->m_nPaneMenuEntryID = 0;
    d->m_bSaveMenuAmps = (uiFlags & kCustMenuAmpers) ? 1 : 0;

    if (plistCustomPages != nullptr) {
        for (RtcListNode* n = static_cast<RtcList*>(plistCustomPages)->m_pNodeHead; n != nullptr; n = n->pNext) {
            CObject* pPage = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(n->data);
            impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CustomPages(d), pPage);
        }
    }

    if (pWndParentFrame == nullptr) {
        ThrowInvalidArg();
        return pThis;
    }

    if ((d->m_pCustomizePage = NewPage(kSizeCommandsPage)) != nullptr)
        impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ(d->m_pCustomizePage);
    if ((d->m_pToolbarsPage = NewPage(kSizeToolbarsPage)) != nullptr)
        impl___0CMFCToolBarsListPropertyPage__QEAA_PEAVCFrameWnd___Z(d->m_pToolbarsPage, d->m_pParentFrame);
    if ((d->m_pKeyboardPage = NewPage(kSizeKeyboardPage)) != nullptr)
        impl___0CMFCToolBarsKeyboardPropertyPage__QEAA_PEAVCFrameWnd__H_Z(d->m_pKeyboardPage, d->m_pParentFrame, d->m_bAutoSetFromMenus);
    if ((d->m_pMenuPage = NewPage(kSizeMenuPage)) != nullptr)
        impl___0CMFCToolBarsMenuPropertyPage__QEAA_PEAVCFrameWnd__H_Z(d->m_pMenuPage, d->m_pParentFrame, d->m_bAutoSetFromMenus);
    if ((d->m_pMousePage = NewPage(kSizeMousePage)) != nullptr)
        impl___0CMFCMousePropertyPage__QEAA_XZ(d->m_pMousePage);

    impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pCustomizePage));
    impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pToolbarsPage));

    if (uiFlags & kCustNoTools) {
        d->m_pToolsPage = nullptr;
    } else {
        if ((d->m_pToolsPage = NewPage(kSizeToolsPage)) != nullptr)
            impl___0CMFCToolBarsToolsPropertyPage__QEAA_XZ(d->m_pToolsPage);
        if (impl__afxUserToolsManager__3PEAVCUserToolsManager__EA != nullptr) {
            if (d->m_pToolsPage == nullptr) {
                ThrowInvalidArg();
                return pThis;
            }
            impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pToolsPage));
        }
    }

    if (impl__afxKeyboardManager__3PEAVCKeyboardManager__EA != nullptr && pWndParentFrame->m_hAccelTable != nullptr) {
        impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pKeyboardPage));
    }

    // TODO(clean-room): retail reads the frame's embedded CFrameImpl here
    // (m_pMenuBar -> bMenuBar, m_uiControlbarsMenuEntryID -> m_nPaneMenuEntryID);
    // OpenMFC's CMDIFrameWndEx / CFrameWndEx embed no CFrameImpl (deviation 4).
    const int bMenuBar = FALSE;

    if (HaveContextMenuManager() || bMenuBar) {
        impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pMenuPage));
    }
    if (HaveMouseManager()) {
        impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pMousePage));
    }

    if (const ObListData* pages = ListData(CustomPages(d))) {
        ObListData::POSITION pos = pages->GetHeadPosition();
        while (pos != ObListData::POSITION(nullptr)) {
            CObject* pPage = pages->GetNext(pos);
            impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(pPage));
        }
    }

    if ((d->m_pOptionsPage = NewPage(kSizeOptionsPage)) != nullptr)
        impl___0CMFCToolBarsOptionsPropertyPage__QEAA_H_Z(d->m_pOptionsPage, bMenuBar);
    impl__AddPage_CPropertySheet__QEAAXPEAVCPropertyPage___Z(Sheet(pThis), static_cast<CPropertyPage*>(d->m_pOptionsPage));

    CString strTitle;
    if (!LoadResString(strTitle, kIdsPropShtCaption)) return pThis;
    if (!LoadResString(d->m_strAllCommands, kIdsAllCommands)) return pThis;
    impl__SetAllCategory_CMFCToolBarsCommandsPropertyPage__QEAAXPEB_W_Z(d->m_pCustomizePage, d->m_strAllCommands.GetString());
    if (d->m_pKeyboardPage != nullptr) {
        impl__SetAllCategory_CMFCToolBarsKeyboardPropertyPage__QEAAXPEB_W_Z(d->m_pKeyboardPage, d->m_strAllCommands.GetString());
    }
    impl__SetTitle_CPropertySheet__QEAAXPEB_WI_Z(Sheet(pThis), strTitle.GetString(), 0);

    if (d->m_bAutoSetFromMenus != 0) {
        impl__SetupFromMenus_CMFCToolBarsCustomizeDialog__IEAAXXZ(pThis);
    }

    CString strNewMenu;
    if (!LoadResString(strNewMenu, kIdsNewMenu)) return pThis;
    {
        StackMenuButton btnNewMenu(0, nullptr, -1, strNewMenu.GetString(), FALSE);
        impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(pThis, strNewMenu.GetString(), btnNewMenu.get(), -1);
    }
    return pThis;
}

// Retail (RVA 0x177360, mfc140u), transcribed:
//     vfptr = 0x1803190d8;
//     for (pos = m_ButtonsByCategory.GetStartPosition(); pos; ) {           // inlined GetNextAssoc, 0x1773c2..0x17742e
//         CString strCategory;  CObList* pList;  GetNextAssoc(pos, strCategory, pList);
//         while (!pList->IsEmpty()) { CObject* p = pList->RemoveHead(); if (p) delete p; }   // 0x231f40, vslot 1 flag 1
//         delete pList;                                                        // vslot 1 flag 1
//     }
//     m_ButtonsByCategory.RemoveAll();                                         // 0x3a400
//     delete m_pCustomizePage; ... delete m_pToolsPage;   (each if non-NULL, vslot 1 flag 1; 0x1774b2..0x177570)
//     while (!m_listCustomPages.IsEmpty()) { CPropertyPage* p = m_listCustomPages.RemoveHead(); if (p) delete p; }   // 0x17757f..0x1775e8
//     m_btnHelp.~CMFCButton();                                                 // 0x1cad0
//     m_strAllCommands release;  ~m_listCustomPages (vfptr 0x180318fe0 + CObList::RemoveAll 0x8350);
//     ~m_strCategoriesList (vfptr 0x18032a788 + CStringList::RemoveAll 0x232300);
//     ~m_ButtonsByCategory (vfptr 0x180319010 + RemoveAll 0x3a400);
//     CPropertySheet::~CPropertySheet();                                       // tail jump 0x217300
// The page and button deletes go through DeletePage / DeleteRuntimeObject
// (see those helpers); the ENSURE(pos != NULL) throws retail keeps inside its
// GetNextAssoc walk have no counterpart in CatForEach.
// Symbol: ??1CMFCToolBarsCustomizeDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsCustomizeDialog__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CustDlg* d = D(pThis);

    CatForEach(d->m_ButtonsByCategory, [](CatAssoc* pAssoc) {
        CObList* pList = pAssoc->value;
        if (pList == nullptr) return;
        while (ListCount(pList) != 0) {
            CObject* p = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(pList);
            if (p != nullptr) DeleteRuntimeObject(p);
        }
        DeleteObList(pList);
        pAssoc->value = nullptr;
    });
    CatRemoveAll(d->m_ButtonsByCategory);

    DeletePage(d->m_pCustomizePage, impl___1CMFCToolBarsCommandsPropertyPage__UEAA_XZ);
    DeletePage(d->m_pToolbarsPage,  impl___1CMFCToolBarsListPropertyPage__UEAA_XZ);
    DeletePage(d->m_pKeyboardPage,  impl___1CMFCToolBarsKeyboardPropertyPage__UEAA_XZ);
    DeletePage(d->m_pMenuPage,      impl___1CMFCToolBarsMenuPropertyPage__UEAA_XZ);
    DeletePage(d->m_pMousePage,     impl___1CMFCMousePropertyPage__UEAA_XZ);
    DeletePage(d->m_pOptionsPage,   impl___1CMFCToolBarsOptionsPropertyPage__UEAA_XZ);
    DeletePage(d->m_pToolsPage,     impl___1CMFCToolBarsToolsPropertyPage__UEAA_XZ);

    while (ListCount(CustomPages(d)) != 0) {
        CObject* p = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CustomPages(d));
        if (p != nullptr) DeleteRuntimeObject(p);
    }

    impl___1CMFCButton__UEAA_XZ(d->m_btnHelp);
    d->m_strAllCommands.~CString();
    impl__RemoveAll_CObList__QEAAXXZ(CustomPages(d));
    impl___1CObList__UEAA_XZ(CustomPages(d));
    impl__RemoveAll_CStringList__QEAAXXZ(CategoryList(d));
    impl___1CStringList__UEAA_XZ(CategoryList(d));
    CatRemoveAll(d->m_ButtonsByCategory);

    impl___1CPropertySheet__UEAA_XZ(pThis);
}

// ===========================================================================
// Category / button bookkeeping
// ===========================================================================

// Retail (RVA 0x177720, mfc140u), fully transcribed:
//     CString strCategory;  ENSURE(strCategory.LoadString(uiCategoryId));   // 0x2aee00 / 0xdb70, throw 0x227720
//     AddButton(strCategory, button, iInsertBefore);                        // 0x1777c0
// Symbol: ?AddButton@CMFCToolBarsCustomizeDialog@@QEAAXIAEBVCMFCToolBarButton@@H@Z
extern "C" void MS_ABI impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXIAEBVCMFCToolBarButton__H_Z(
    void* pThis, unsigned int uiCategoryId, const void* pButton, int iInsertBefore) {
    if (pThis == nullptr) return;
    CString strCategory;
    if (!LoadResString(strCategory, uiCategoryId)) return;
    impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(pThis, strCategory.GetString(), pButton, iInsertBefore);
}

// Retail (RVA 0x1777c0, mfc140u), transcribed:
//     if (m_nPaneMenuEntryID != 0 && button.m_nID == m_nPaneMenuEntryID) return;   // 0x1777ed
//     if (!button.IsEditable()) return;                                    // vslot 0xd0 (26), 0x17780c
//     if (!CMFCToolBar::IsCommandPermitted(button.m_nID)) return;          // inlined Find in m_lstUnpermittedCommands, 0x17781f
//     CString strText = button.m_strText;  strText.TrimLeft();  strText.TrimRight();   // 0x177844..0x17785e
//     BOOL bUserTool = FALSE;
//     if (afxUserToolsManager && afxUserToolsManager->IsUserToolCmd(button.m_nID)) {  // +0x44 <= id <= +0x48
//         CUserTool* pTool = the tool in afxUserToolsManager->m_lstUserTools (+0x10 head) whose m_uiCmdId (+0x20) == button.m_nID;
//         if (!pTool) return;                                              // 0x17789e
//         strText = pTool->m_strLabel;  bUserTool = TRUE;                  // +0x8
//     }
//     if (strText.IsEmpty()) {
//         CString strMessage;
//         if (strMessage.LoadString(button.m_nID) && !strMessage.IsEmpty()) {   // 0x177929 / 0x177942
//             int iOffset = strMessage.Find(L'\n');                       // wcschr 0x17795e
//             if (iOffset != -1) strText = strMessage.Mid(iOffset + 1);    // 0x177986
//         }
//         if (strText.IsEmpty()) { ATLENSURE(lpszCategory != NULL); return; }   // 0x1779ce; E_FAIL at 0x177cf1 (the m_strAllCommands compare at 0x1779e1 reaches the same return either way)
//     } else {
//         if (!m_bSaveMenuAmps) strText.Remove(L'&');                      // 0x177a53
//         int iTab = strText.Find(L'\t');  if (iTab != -1) strText = strText.Left(iTab);   // 0x177a6a / 0x177a8a
//         if (strText.IsEmpty()) return;
//     }
//     CObList* pList;
//     if (!m_ButtonsByCategory.Lookup(lpszCategory, pList)) {              // 0x179a60
//         pList = new CObList;  m_ButtonsByCategory.SetAt(lpszCategory, pList);   // 0x177aed / 0x310c4
//         ATLENSURE(lpszCategory != NULL);                                 // E_FAIL at 0x177cfc
//         if (wcscmp(m_strAllCommands, lpszCategory) != 0) m_strCategoriesList.AddTail(lpszCategory);   // 0x232590
//     } else {
//         for each CMFCToolBarButton* pBtn in *pList (ENSURE(pBtn)):
//             if ((pBtn->m_nID == button.m_nID && pBtn->m_nID != (UINT)-1) ||
//                 (pBtn->m_nID == (UINT)-1 && wcscmp(pBtn->m_strText, button.m_strText) == 0))   // 0x177b8c..0x177bc3 (ATLENSURE(button.m_strText) at 0x177d07)
//             { if (pBtn->m_strText.IsEmpty()) pBtn->m_strText = button.m_strText;  return; }   // 0x177c6e
//     }
//     CRuntimeClass* pRTC = button.GetRuntimeClass();  ENSURE(pRTC);       // vslot 0
//     CMFCToolBarButton* pNew = (CMFCToolBarButton*)pRTC->CreateObject();  ENSURE(pNew);   // 0x234d60
//     pNew->CopyFrom(button);                                              // vslot 0x38 (7)
//     pNew->m_strText = strText;
//     if (bUserTool) pNew->SetImage(0);                                    // vslot 0x190 (50), 0x177c34
//     if (iInsertBefore != -1 && iInsertBefore >= 0 && iInsertBefore < pList->GetCount() && (pos = pList->FindIndex(iInsertBefore)))
//          pList->InsertBefore(pos, pNew);  else pList->AddTail(pNew);     // 0x232010 / 0x231e70
//     ENSURE(lpszCategory != NULL);                                        // 0x177ce0 (ATLENSURE, E_FAIL)
//     if (wcscmp(m_strAllCommands, lpszCategory) != 0) AddButton(m_strAllCommands, button, -1);   // 0x177cc3
//     pNew->OnAddToCustomizePage();                                        // vslot 0x98 (19), 0x177cd5
// DEVIATIONS: IsEditable is the ButtonIsEditable transcription (its two
// dropped tests are named there); the user-tool block is not reproduced (the
// exported ?afxUserToolsManager@@ is NULL in this tree and OpenMFC's
// CUserToolsManager has no retail member layout), so bUserTool stays FALSE
// behind the same gate; CopyFrom / SetImage go to the exported CMFCToolBarButton
// base bodies rather than through the vptr (no dispatchable button vtable --
// see featurepack/toolbar/CMFCToolBar.cpp::InsertButton); OnAddToCustomizePage
// is retail's base body 0x27d0, a bare `ret`, and is not exported, so nothing
// is called for it.  The ENSURE throws are kept as thunk calls + return.
// Symbol: ?AddButton@CMFCToolBarsCustomizeDialog@@QEAAXPEB_WAEBVCMFCToolBarButton@@H@Z
extern "C" void MS_ABI impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(
    void* pThis, const wchar_t* lpszCategory, const void* pButtonArg, int iInsertBefore) {
    if (pThis == nullptr || pButtonArg == nullptr) return;
    S_CustDlg* d = D(pThis);
    const CMFCToolBarButton& button = *static_cast<const CMFCToolBarButton*>(pButtonArg);

    if (d->m_nPaneMenuEntryID != 0 && button.m_nID == d->m_nPaneMenuEntryID) return;
    if (!ButtonIsEditable(&button)) return;
    if (!IsCommandPermitted(button.m_nID)) return;

    CString strText = button.m_strText;
    strText.TrimLeft();
    strText.TrimRight();

    int bUserTool = FALSE;
    if (impl__afxUserToolsManager__3PEAVCUserToolsManager__EA != nullptr) {
        // TODO(clean-room): retail resolves button.m_nID against
        // afxUserToolsManager's command range and m_lstUserTools here and
        // takes the tool's label (see the transcription above); the retail
        // CUserToolsManager layout is not modelled by OpenMFC.
    }

    if (strText.IsEmpty()) {
        CString strMessage;
        HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(button.m_nID));
        if (hInst != nullptr &&
            impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&strMessage, hInst, button.m_nID) &&
            !strMessage.IsEmpty()) {
            const int iOffset = strMessage.Find(L'\n');
            if (iOffset != -1) {
                strText = strMessage.Mid(iOffset + 1);
            }
        }
        if (strText.IsEmpty()) {
            if (lpszCategory == nullptr) { ThrowOle(kEFail); return; }
            return;
        }
    } else {
        if (d->m_bSaveMenuAmps == 0) StrRemoveChar(strText, L'&');
        if (!strText.IsEmpty()) {
            const int iTab = strText.Find(L'\t');
            if (iTab != -1) {
                strText = strText.Left(iTab);
            }
        }
        if (strText.IsEmpty()) return;
    }

    CObList* pList = nullptr;
    if (!CatLookup(d->m_ButtonsByCategory, lpszCategory, pList)) {
        pList = NewObList();
        CatSetAt(d->m_ButtonsByCategory, lpszCategory, pList);
        if (lpszCategory == nullptr) { ThrowOle(kEFail); return; }
        if (std::wcscmp(d->m_strAllCommands.GetString(), lpszCategory) != 0) {
            impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(CategoryList(d), lpszCategory);
        }
    } else {
        if (pList == nullptr) { ThrowInvalidArg(); return; }
        if (const ObListData* items = ListData(pList)) {
            ObListData::POSITION pos = items->GetHeadPosition();
            while (pos != ObListData::POSITION(nullptr)) {
                CMFCToolBarButton* pBtn = Btn(items->GetNext(pos));
                if (pBtn == nullptr) { ThrowInvalidArg(); return; }
                bool bSame = false;
                if (pBtn->m_nID == button.m_nID && pBtn->m_nID != static_cast<UINT>(-1)) {
                    bSame = true;
                } else if (pBtn->m_nID == static_cast<UINT>(-1)) {
                    if (button.m_strText.GetString() == nullptr) { ThrowOle(kEFail); return; }
                    bSame = std::wcscmp(pBtn->m_strText.GetString(), button.m_strText.GetString()) == 0;
                }
                if (bSame) {
                    if (pBtn->m_strText.IsEmpty()) pBtn->m_strText = button.m_strText;
                    return;
                }
            }
        }
    }
    if (pList == nullptr) { ThrowInvalidArg(); return; }

    CRuntimeClass* pRTC = button.GetRuntimeClass();   // vslot 0 under both vtable ABIs
    if (pRTC == nullptr) { ThrowInvalidArg(); return; }
    CMFCToolBarButton* pNew = Btn(impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pRTC));
    if (pNew == nullptr) { ThrowInvalidArg(); return; }
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pNew, &button);
    pNew->m_strText = strText;
    if (bUserTool) impl__SetImage_CMFCToolBarButton__UEAAXH_Z(pNew, 0);

    bool bInserted = false;
    if (iInsertBefore != -1 && iInsertBefore >= 0 && iInsertBefore < ListCount(pList)) {
        CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION___J_Z(pList, iInsertBefore);
        if (pos != CObList::POSITION(nullptr)) {
            impl__InsertBefore_CObList__QEAAPEAU__POSITION__PEAU2_PEAVCObject___Z(pList, &pos, pNew);
            bInserted = true;
        }
    }
    if (!bInserted) {
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(pList, pNew);
    }

    if (lpszCategory == nullptr) { ThrowOle(kEFail); return; }
    if (std::wcscmp(d->m_strAllCommands.GetString(), lpszCategory) != 0) {
        impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(pThis, d->m_strAllCommands.GetString(), &button, -1);
    }
    // pNew->OnAddToCustomizePage(): retail base body is `ret` (0x27d0); not dispatchable here.
}

// Retail (RVA 0x177d20, mfc140u), fully transcribed:
//     if (uiCategoryId == (UINT)-1) {
//         for (pos = m_strCategoriesList.GetHeadPosition(); ; ) {          // inline node walk from +0x1d0
//             CString strCategory = pos ? m_strCategoriesList.GetNext(pos) : m_strAllCommands;   // 0x177d72 / 0x177d7e
//             RemoveButton(strCategory, uiCmdId);                          // 0x177e70
//             if (that was m_strAllCommands) break;
//         }
//         return 0;                                                        // 0x177dcb
//     }
//     CString strCategory;  ENSURE(strCategory.LoadString(uiCategoryId));  // 0x177e5f throw
//     return RemoveButton(strCategory, uiCmdId);
// Symbol: ?RemoveButton@CMFCToolBarsCustomizeDialog@@QEAAHII@Z
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBarsCustomizeDialog__QEAAHII_Z(void* pThis, unsigned int uiCategoryId, unsigned int uiCmdId) {
    if (pThis == nullptr) return -1;
    S_CustDlg* d = D(pThis);
    if (uiCategoryId == static_cast<unsigned int>(-1)) {
        if (const StrListData* cats = StringListData(CategoryList(d))) {
            StrListData::POSITION pos = cats->GetHeadPosition();
            while (pos != StrListData::POSITION(nullptr)) {
                CString strCategory = cats->GetNext(pos);
                impl__RemoveButton_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(pThis, strCategory.GetString(), uiCmdId);
            }
        }
        CString strAll = d->m_strAllCommands;
        impl__RemoveButton_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(pThis, strAll.GetString(), uiCmdId);
        return 0;
    }
    CString strCategory;
    if (!LoadResString(strCategory, uiCategoryId)) return -1;
    return impl__RemoveButton_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(pThis, strCategory.GetString(), uiCmdId);
}

// Retail (RVA 0x177e70, mfc140u), fully transcribed:
//     ENSURE(lpszCategory != NULL);                                        // 0x177ef2
//     CObList* pList;  if (!m_ButtonsByCategory.Lookup(lpszCategory, pList)) return -1;   // 0x179a60
//     int i = 0;
//     for (pos = pList->GetHeadPosition(); pos; i++) {
//         POSITION posSave = pos;  CMFCToolBarButton* p = pList->GetNext(pos);  ENSURE(p);
//         if (p->m_nID == uiCmdId) { pList->RemoveAt(posSave); delete p; return i; }   // 0x2320d0, vslot 1 flag 1
//     }
//     return -1;
// Symbol: ?RemoveButton@CMFCToolBarsCustomizeDialog@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__RemoveButton_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszCategory, unsigned int uiCmdId) {
    if (pThis == nullptr) return -1;
    if (lpszCategory == nullptr) { ThrowInvalidArg(); return -1; }
    S_CustDlg* d = D(pThis);
    CObList* pList = nullptr;
    if (!CatLookup(d->m_ButtonsByCategory, lpszCategory, pList) || pList == nullptr) return -1;
    const ObListData* items = ListData(pList);
    if (items == nullptr) return -1;
    int i = 0;
    ObListData::POSITION pos = items->GetHeadPosition();
    while (pos != ObListData::POSITION(nullptr)) {
        CObList::POSITION posSave = pos;
        CMFCToolBarButton* p = Btn(items->GetNext(pos));
        if (p == nullptr) { ThrowInvalidArg(); return -1; }
        if (p->m_nID == uiCmdId) {
            impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(pList, &posSave);
            DeleteRuntimeObject(p);
            return i;
        }
        i++;
    }
    return -1;
}

// Retail (RVA 0x178300, mfc140u), fully transcribed:
//     CRuntimeClass* pRTC = button.GetRuntimeClass();  ENSURE(pRTC);       // vslot 0, 0x178485
//     for (pos = m_strCategoriesList.GetHeadPosition(); ; ) {              // inline walk from +0x1d0, then m_strAllCommands last
//         CString strCategory = pos ? GetNext(pos) : m_strAllCommands;
//         CObList* pList;
//         if (m_ButtonsByCategory.Lookup(strCategory, pList) && pList) {  // 0x179a60
//             for (each node; ENSURE(pBtn)) if (pBtn->m_nID == uiCmd) {
//                 CMFCToolBarButton* pNew = (CMFCToolBarButton*)pRTC->CreateObject();   // 0x234d60 (unchecked in retail)
//                 pNew->CopyFrom(button);                                  // vslot 0x38
//                 if (pNew->m_strText.IsEmpty()) pNew->m_strText = pBtn->m_strText;
//                 node->data = pNew;  delete pBtn;                          // vslot 1 flag 1
//             }
//         }
//         if (that was m_strAllCommands) break;
//     }
// CopyFrom goes to the exported base body (no dispatchable button vtable); the
// in-place node write is CList::SetAt on the OpenMFC storage.
// Symbol: ?ReplaceButton@CMFCToolBarsCustomizeDialog@@QEAAXIAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__ReplaceButton_CMFCToolBarsCustomizeDialog__QEAAXIAEBVCMFCToolBarButton___Z(void* pThis, unsigned int uiCmd, const void* pButtonArg) {
    if (pThis == nullptr || pButtonArg == nullptr) return;
    S_CustDlg* d = D(pThis);
    const CMFCToolBarButton& button = *static_cast<const CMFCToolBarButton*>(pButtonArg);
    CRuntimeClass* pRTC = button.GetRuntimeClass();
    if (pRTC == nullptr) { ThrowInvalidArg(); return; }

    const StrListData* cats = StringListData(CategoryList(d));
    StrListData::POSITION pos = cats ? cats->GetHeadPosition() : StrListData::POSITION(nullptr);
    for (;;) {
        CString strCategory;
        bool bLast = false;
        if (pos != StrListData::POSITION(nullptr)) {
            strCategory = cats->GetNext(pos);
        } else {
            strCategory = d->m_strAllCommands;
            bLast = true;
        }
        CObList* pList = nullptr;
        if (CatLookup(d->m_ButtonsByCategory, strCategory.GetString(), pList) && pList != nullptr) {
            if (ObListData* items = ListData(pList)) {
                ObListData::POSITION posItem = items->GetHeadPosition();
                while (posItem != ObListData::POSITION(nullptr)) {
                    ObListData::POSITION posSave = posItem;
                    CMFCToolBarButton* pBtn = Btn(items->GetNext(posItem));
                    if (pBtn == nullptr) { ThrowInvalidArg(); return; }
                    if (pBtn->m_nID != uiCmd) continue;
                    CMFCToolBarButton* pNew = Btn(impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pRTC));
                    if (pNew == nullptr) continue;   // retail would fault on a NULL CreateObject result
                    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pNew, &button);
                    if (pNew->m_strText.IsEmpty()) pNew->m_strText = pBtn->m_strText;
                    items->SetAt(posSave, pNew);
                    DeleteRuntimeObject(pBtn);
                }
            }
        }
        if (bLast) break;
    }
}

// Retail (RVA 0x178190, mfc140u), fully transcribed:
//     if (m_strCategoriesList.Find(lpszCategoryNew)) return FALSE;         // 0x2329e0
//     POSITION pos = m_strCategoriesList.Find(lpszCategoryOld);  if (!pos) return FALSE;
//     CObList* pList;
//     if (m_ButtonsByCategory.Lookup(lpszCategoryNew, pList)) return FALSE;   // 0x179a60
//     if (!m_ButtonsByCategory.Lookup(lpszCategoryOld, pList)) return FALSE;
//     m_strCategoriesList.GetAt(pos) = lpszCategoryNew;                    // SetString on the node, 0x2e30
//     m_ButtonsByCategory.RemoveKey(lpszCategoryOld);                      // inlined, 0x17823b..0x1782ce
//     m_ButtonsByCategory.SetAt(lpszCategoryNew, pList);                   // 0x310c4
//     return TRUE;
// Symbol: ?RenameCategory@CMFCToolBarsCustomizeDialog@@QEAAHPEB_W0@Z
extern "C" int MS_ABI impl__RenameCategory_CMFCToolBarsCustomizeDialog__QEAAHPEB_W0_Z(void* pThis, const wchar_t* lpszCategoryOld, const wchar_t* lpszCategoryNew) {
    if (pThis == nullptr) return FALSE;
    S_CustDlg* d = D(pThis);
    CStringList* cats = CategoryList(d);
    if (impl__Find_CStringList__QEBAPEAU__POSITION__PEB_WPEAU2__Z(cats, lpszCategoryNew, nullptr) != CStringList::POSITION(nullptr)) return FALSE;
    CStringList::POSITION pos = impl__Find_CStringList__QEBAPEAU__POSITION__PEB_WPEAU2__Z(cats, lpszCategoryOld, nullptr);
    if (pos == CStringList::POSITION(nullptr)) return FALSE;
    CObList* pList = nullptr;
    if (CatLookup(d->m_ButtonsByCategory, lpszCategoryNew, pList)) return FALSE;
    if (!CatLookup(d->m_ButtonsByCategory, lpszCategoryOld, pList)) return FALSE;

    if (StrListData* data = StringListData(cats)) {
        data->SetAt(pos, CString(lpszCategoryNew ? lpszCategoryNew : L""));
    }
    CatRemoveKey(d->m_ButtonsByCategory, lpszCategoryOld);
    CatSetAt(d->m_ButtonsByCategory, lpszCategoryNew, pList);
    return TRUE;
}

// Retail (RVA 0x1784a0, mfc140u), fully transcribed:
//     ENSURE(lpszCategory != NULL);                                        // 0x1784e9
//     CObList* pList;  if (!m_ButtonsByCategory.Lookup(lpszCategory, pList)) return FALSE;
//     m_pCustomizePage->SetUserCategory(lpszCategory);                     // 0x17be90
//     return TRUE;
// Symbol: ?SetUserCategory@CMFCToolBarsCustomizeDialog@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SetUserCategory_CMFCToolBarsCustomizeDialog__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpszCategory) {
    if (pThis == nullptr) return FALSE;
    if (lpszCategory == nullptr) { ThrowInvalidArg(); return FALSE; }
    S_CustDlg* d = D(pThis);
    CObList* pList = nullptr;
    if (!CatLookup(d->m_ButtonsByCategory, lpszCategory, pList)) return FALSE;
    impl__SetUserCategory_CMFCToolBarsCommandsPropertyPage__QEAAXPEB_W_Z(d->m_pCustomizePage, lpszCategory);
    return TRUE;
}

// ===========================================================================
// Window lifecycle / message handlers
// ===========================================================================

// Retail (RVA 0x177690, mfc140u), fully transcribed:
//     if ((int)Default() == -1) return -1;                                 // 0x28ac80 (the CPropertySheet::OnCreate call resolves to CWnd::OnCreate,
//                                                                          //  `return (int)Default();`, inlined; the compare at 0x17769e is 32-bit)
//     if (m_uiFlags & AFX_CUSTOMIZE_CONTEXT_HELP /*0x10*/) ModifyStyleEx(0, WS_EX_CONTEXTHELP /*0x400*/, 0);   // 0x2a9740
//     g_pWndCustomize /*0x3be3a0*/ = this;
//     return 0;
// Symbol: ?OnCreate@CMFCToolBarsCustomizeDialog@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCToolBarsCustomizeDialog__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;   // retail never reads it
    if (pThis == nullptr) return -1;
    S_CustDlg* d = D(pThis);
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(Wnd(pThis))) == -1) return -1;
    if (d->m_uiFlags & kCustContextHelp) {
        impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(Wnd(pThis), 0, WS_EX_CONTEXTHELP, 0);
    }
    OpenMFC_g_pWndCustomize = pThis;
    return 0;
}

// Retail (RVA 0x1776e0, mfc140u), fully transcribed:
//     g_pWndCustomize /*0x3be3a0*/ = NULL;
//     SetFrameCustMode(FALSE);                                             // 0x1784f0
//     delete this;                                                         // vslot 1 (scalar deleting dtor) with flag 1, 0x17770b
// The `delete this` follows the DeleteRuntimeObject rule of this file: a
// client vtable (outside this image -- an MSVC-derived class laid out from the
// SDK header) is dispatched through its slot 1 exactly as retail does, so the
// client's destructor chain and its operator delete run; an object whose
// vptr is the mingw table the CPropertySheet ctor thunk installed was built by
// this file's ctor thunk, so it is destroyed by this file's dtor thunk and its
// block returned through the exported ??3@YAXPEAX@Z -- the deallocator that
// pairs with the exported ??2@YAPEAX_K@Z an _AFXDLL client's `new` resolves to
// (both are OpenMFC's malloc/free, detail/MemcoreSupport.cpp).
// Symbol: ?PostNcDestroy@CMFCToolBarsCustomizeDialog@@UEAAXXZ
extern "C" void MS_ABI impl__PostNcDestroy_CMFCToolBarsCustomizeDialog__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    OpenMFC_g_pWndCustomize = nullptr;
    impl__SetFrameCustMode_CMFCToolBarsCustomizeDialog__IEAAXH_Z(pThis, FALSE);
    const void* vptr = *reinterpret_cast<void* const*>(pThis);
    if (vptr != nullptr && !PointsIntoThisImage(vptr)) {
        typedef void* (MS_ABI *DeletingDtor)(void*, unsigned int);
        reinterpret_cast<DeletingDtor const*>(vptr)[1](pThis, 1u);
        return;
    }
    impl___1CMFCToolBarsCustomizeDialog__UEAA_XZ(pThis);
    impl___3_YAXPEAX_Z(pThis);
}

// Retail (RVA 0x178770, mfc140u), fully transcribed:
//     DWORD dwExStyle = 0;
//     if (m_pParentFrame != NULL) dwExStyle = m_pParentFrame->GetExStyle() & WS_EX_LAYOUTRTL /*0x400000*/;   // 0x2a96c0
//     if (!CPropertySheet::Create(m_pParentFrame, (DWORD)-1, dwExStyle)) return FALSE;   // direct call 0x2177f0
//     SetFrameCustMode(TRUE);                                              // 0x1784f0
//     return TRUE;
// Symbol: ?Create@CMFCToolBarsCustomizeDialog@@UEAAHXZ
extern "C" int MS_ABI impl__Create_CMFCToolBarsCustomizeDialog__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_CustDlg* d = D(pThis);
    unsigned long dwExStyle = 0;
    if (d->m_pParentFrame != nullptr) {
        dwExStyle = impl__GetExStyle_CWnd__QEBAKXZ(d->m_pParentFrame) & WS_EX_LAYOUTRTL;
    }
    if (!impl__Create_CPropertySheet__UEAAHPEAVCWnd__KK_Z(Sheet(pThis), d->m_pParentFrame, static_cast<unsigned long>(-1), dwExStyle)) {
        return FALSE;
    }
    impl__SetFrameCustMode_CMFCToolBarsCustomizeDialog__IEAAXH_Z(pThis, TRUE);
    return TRUE;
}

// Retail (RVA 0x1787e0, mfc140u), transcribed:
//     BOOL bResult = CPropertySheet::OnInitDialog();                       // 0x217ff0
//     CRect rectClient;  GetClientRect(rectClient);                        // ::GetClientRect(m_hWnd)
//     CWnd* pWndCancel = GetDlgItem(IDCANCEL /*2*/);  if (!pWndCancel) return bResult;   // 0x2a9390, 0x178842
//     pWndCancel->ShowWindow(SW_SHOW);  pWndCancel->EnableWindow(TRUE);
//     CRect rectCancel;  pWndCancel->GetClientRect(rectCancel);  ::MapWindowPoints(hCancel, m_hWnd, &rectCancel, 2);
//     CRect rectWindow;  GetWindowRect(rectWindow);
//     SetWindowPos(NULL, 0, 0, rectWindow.Width(), rectWindow.Height() + rectCancel.Height() + 16,
//                  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE /*0x16*/);   // 0x1788d5
//     pWndCancel->SetWindowPos(NULL, rectClient.right - rectCancel.Width() - 8, rectCancel.top + 4, 0, 0,
//                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE /*0x15*/);   // 0x178910
//     CWnd* pWndOk = GetDlgItem(IDOK /*1*/);  if (pWndOk) pWndOk->ModifyStyle(BS_DEFPUSHBUTTON, 0, 0);   // 0x2a96f0
//     pWndCancel->ModifyStyle(0, BS_DEFPUSHBUTTON, 0);
//     CString strCloseText;  ENSURE(strCloseText.LoadString(IDS_AFXBARRES_CLOSE /*0x3ee9*/));   // 0x178bf2 throw
//     pWndCancel->SetWindowText(strCloseText);                             // 0x2a9790
//     CRect rectDlg;  GetWindowRect(rectDlg);
//     int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
//     int cyScreen = ::GetSystemMetrics(SM_CYMAXIMIZED) - (::GetSystemMetrics(SM_CYSCREEN) - ::GetSystemMetrics(SM_CYMAXIMIZED));   // 0x1789bb..0x1789fb
//     if (rectDlg.left < 0 || rectDlg.top < 0)
//         SetWindowPos(NULL, max(rectDlg.left, 0), max(rectDlg.top, 0), 0, 0, SWP_NOSIZE);   // 0x178a56..0x178a78
//     else if (rectDlg.right > cxScreen || rectDlg.bottom > cyScreen)
//         SetWindowPos(NULL, rectDlg.right > cxScreen ? rectDlg.left - (rectDlg.right - cxScreen) : rectDlg.left,
//                            rectDlg.bottom > cyScreen ? rectDlg.top - (rectDlg.bottom - cyScreen) : rectDlg.top,
//                            0, 0, SWP_NOSIZE);                             // 0x178a0e..0x178a4f
//     CWnd* pWndHelp = GetDlgItem(IDHELP /*9*/);  if (!pWndHelp) return bResult;   // 0x178a93
//     if (m_uiFlags & AFX_CUSTOMIZE_NOHELP /*8*/) { pWndHelp->ShowWindow(SW_HIDE); pWndHelp->EnableWindow(FALSE); }   // 0x178ae1
//     else {
//         m_btnHelp.SubclassWindow(pWndHelp->m_hWnd);                      // 0x291d80, this+0x290
//         m_btnHelp.ShowWindow(SW_SHOW);  m_btnHelp.EnableWindow(TRUE);
//         if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }   // 0x6a790
//         BOOL b32 = afxGlobalData.+0x268 && afxGlobalData.+0x288 >= 16 && !afxGlobalData.+0x260 && !afxGlobalData.+0x264;   // 0x178b47..0x178b70
//         m_btnHelp.SetImageInternal(b32 ? IDB_AFXBARRES_HELP32 /*0x42ab*/ : IDB_AFXBARRES_HELP /*0x3f03*/, 0, FALSE, 0);   // 0x1dd00
//         m_btnHelp.SetWindowText(L"");                                    // 0x33d19c
//         CSize size = m_btnHelp.SizeToContent(TRUE);                      // vslot 0x2f0 (94) of the CMFCButton vftable
//         m_btnHelp.SetWindowPos(NULL, rectClient.left + 8, rectCancel.top, size.cx, size.cy,
//                                SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/);   // 0x178be8
//     }
//     return bResult;
// DEVIATIONS: the CMFCButton virtual (SizeToContent) goes to the exported
// base thunk (deviation 1); ?afxGlobalData@@ is a zero blob whose Initialize
// thunk is a no-op on this side, so the four flag reads at +0x268/+0x288/
// +0x260/+0x264 are performed as retail performs them but always select the
// 16-colour IDB_AFXBARRES_HELP; SetImageInternal(UINT,...) is itself still a
// documented partial in featurepack/controls/CMFCButton.cpp (it loads no
// resource image).  Control flow is retail's; the only extra tests are the
// NULL `this` guard and the LoadResString early return after its throw.
// Symbol: ?OnInitDialog@CMFCToolBarsCustomizeDialog@@UEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsCustomizeDialog__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_CustDlg* d = D(pThis);
    const int bResult = impl__OnInitDialog_CPropertySheet__UEAAHXZ(Sheet(pThis));
    const HWND hWnd = WndHandle(Wnd(pThis));

    RECT rectClient = {};
    ::GetClientRect(hWnd, &rectClient);

    CWnd* pWndCancel = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(Wnd(pThis), IDCANCEL);
    if (pWndCancel == nullptr) return bResult;

    impl__ShowWindow_CWnd__QEAAHH_Z(pWndCancel, SW_SHOW);
    impl__EnableWindow_CWnd__QEAAHH_Z(pWndCancel, TRUE);

    RECT rectCancel = {};
    ::GetClientRect(WndHandle(pWndCancel), &rectCancel);
    ::MapWindowPoints(WndHandle(pWndCancel), hWnd, reinterpret_cast<POINT*>(&rectCancel), 2);

    RECT rectWindow = {};
    ::GetWindowRect(hWnd, &rectWindow);

    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(Wnd(pThis), nullptr, 0, 0,
        rectWindow.right - rectWindow.left,
        (rectWindow.bottom - rectWindow.top) + (rectCancel.bottom - rectCancel.top) + 16,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pWndCancel, nullptr,
        rectClient.right - (rectCancel.right - rectCancel.left) - 8, rectCancel.top + 4, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    if (CWnd* pWndOk = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(Wnd(pThis), IDOK)) {
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(pWndOk, BS_DEFPUSHBUTTON, 0, 0);
    }
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(pWndCancel, 0, BS_DEFPUSHBUTTON, 0);

    CString strCloseText;
    if (!LoadResString(strCloseText, kIdsClose)) return bResult;
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pWndCancel, strCloseText.GetString());

    RECT rectDlg = {};
    ::GetWindowRect(hWnd, &rectDlg);
    const int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
    const int cyMax1   = ::GetSystemMetrics(SM_CYMAXIMIZED);
    const int cyFull   = ::GetSystemMetrics(SM_CYSCREEN);
    const int cyMax2   = ::GetSystemMetrics(SM_CYMAXIMIZED);
    if (rectDlg.left < 0 || rectDlg.top < 0) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(Wnd(pThis), nullptr,
            rectDlg.left < 0 ? 0 : rectDlg.left, rectDlg.top < 0 ? 0 : rectDlg.top, 0, 0, SWP_NOSIZE);
    } else {
        const int cyScreen = cyMax2 - cyFull + cyMax1;
        if (rectDlg.right > cxScreen || rectDlg.bottom > cyScreen) {
            const int y = rectDlg.bottom > cyScreen ? cyScreen - rectDlg.bottom + rectDlg.top : rectDlg.top;
            const int x = rectDlg.right > cxScreen ? cxScreen - rectDlg.right + rectDlg.left : rectDlg.left;
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(Wnd(pThis), nullptr, x, y, 0, 0, SWP_NOSIZE);
        }
    }

    CWnd* pWndHelp = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(Wnd(pThis), IDHELP);
    if (pWndHelp == nullptr) return bResult;

    if (d->m_uiFlags & kCustNoHelp) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pWndHelp, SW_HIDE);
        impl__EnableWindow_CWnd__QEAAHH_Z(pWndHelp, FALSE);
        return bResult;
    }

    CWnd* pBtnHelp = HelpButton(d);
    impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(pBtnHelp, WndHandle(pWndHelp));
    impl__ShowWindow_CWnd__QEAAHH_Z(pBtnHelp, SW_SHOW);
    impl__EnableWindow_CWnd__QEAAHH_Z(pBtnHelp, TRUE);

    // afxGlobalData one-time gate and the four flag reads, at the retail offsets.
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x000, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x000, &one, sizeof one);
    }
    int gdLayer = 0, gdBpp = 0, gdBlackHC = 0, gdWhiteHC = 0;
    std::memcpy(&gdLayer,   impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x268, sizeof gdLayer);
    std::memcpy(&gdBpp,     impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x288, sizeof gdBpp);
    std::memcpy(&gdBlackHC, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x260, sizeof gdBlackHC);
    std::memcpy(&gdWhiteHC, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x264, sizeof gdWhiteHC);
    const bool b32 = gdLayer != 0 && gdBpp >= 16 && gdBlackHC == 0 && gdWhiteHC == 0;
    impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(pBtnHelp, b32 ? kIdbHelp32 : kIdbHelp, 0, FALSE, 0);
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pBtnHelp, L"");

    CSize size(0, 0);
    impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(pBtnHelp, &size, TRUE);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pBtnHelp, nullptr, rectClient.left + 8, rectCancel.top,
                                                 size.cx, size.cy, SWP_NOZORDER | SWP_NOACTIVATE);
    return bResult;
}

// Retail (RVA 0x178c00, mfc140u), fully transcribed:
//     if (<the tools-page gate, see ToolsPageBlocksClose>) return;         // 0x178c0a..0x178c69
//     CPropertySheet::OnClose();                                           // 0x217f40
// Symbol: ?OnClose@CMFCToolBarsCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMFCToolBarsCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (ToolsPageBlocksClose(D(pThis))) return;
    impl__OnClose_CPropertySheet__IEAAXXZ(Sheet(pThis));
}

// Retail (RVA 0x178c90, mfc140u), fully transcribed:
//     switch (LOWORD(wParam)) {
//     case IDCANCEL /*2*/:                                                 // 0x178cd7
//         if (<the tools-page gate, see ToolsPageBlocksClose>) return TRUE;
//         DestroyWindow();  return TRUE;                                   // vslot 0xd0 (26) = ?DestroyWindow@CWnd@@UEAAHXZ, 0x178d3b
//     case IDHELP /*9*/:                                                   // 0x178cb3
//         m_pParentFrame->SendMessage(AFX_WM_CUSTOMIZEHELP, GetActiveIndex(), (LPARAM)this);  return TRUE;
//     }
//     return CPropertySheet::OnCommand(wParam, lParam);                    // 0x2184f0
// DestroyWindow goes to the exported CWnd base body (deviation 1).
// Symbol: ?OnCommand@CMFCToolBarsCustomizeDialog@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCToolBarsCustomizeDialog__MEAAH_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return FALSE;
    S_CustDlg* d = D(pThis);
    switch (static_cast<unsigned short>(wParam)) {
    case IDCANCEL:
        if (ToolsPageBlocksClose(d)) return TRUE;
        impl__DestroyWindow_CWnd__UEAAHXZ(Wnd(pThis));
        return TRUE;
    case IDHELP:
        SendCustomizeHelp(d);
        return TRUE;
    default:
        break;
    }
    return impl__OnCommand_CPropertySheet__UEAAH_K_J_Z(Sheet(pThis), wParam, lParam);
}

// Retail (RVA 0x1796d0, mfc140u; absent from mfc140u_rva_symbols.json, so it
// was located as the WM_HELP (0x53) entry of the message map at 0x319038 and
// then confirmed against the full export table, where the ordinal of
// ?OnHelpInfo@CMFCToolBarsCustomizeDialog@@ resolves to the same RVA), fully
// transcribed:
//     m_pParentFrame->SendMessage(AFX_WM_CUSTOMIZEHELP, GetActiveIndex(), (LPARAM)this);
//     return TRUE;
// Symbol: ?OnHelpInfo@CMFCToolBarsCustomizeDialog@@IEAAHPEAUtagHELPINFO@@@Z
extern "C" int MS_ABI impl__OnHelpInfo_CMFCToolBarsCustomizeDialog__IEAAHPEAUtagHELPINFO___Z(void* pThis, HELPINFO* pHelpInfo) {
    (void)pHelpInfo;   // retail never reads it
    if (pThis == nullptr) return FALSE;
    SendCustomizeHelp(D(pThis));
    return TRUE;
}

// Retail (RVA 0x1784f0, mfc140u; called from Create 0x1787b3 and PostNcDestroy
// 0x1776f6), transcribed:
//     CWaitCursor wait;                                                    // 0x1de7b0 / dtor 0x7687c
//     for (CWnd* pWnd = CWnd::FromHandle(::GetWindow(m_pParentFrame->m_hWnd, GW_CHILD)); pWnd;
//          pWnd = CWnd::FromHandle(::GetWindow(pWnd->m_hWnd, GW_HWNDNEXT))) {          // 0x1785b1 / 0x28ad70
//         CRuntimeClass* pRTC = pWnd->GetRuntimeClass();                   // vslot 0
//         if (pRTC != NULL && (pRTC->IsDerivedFrom(RUNTIME_CLASS(CDockBar)) || CDockSite || CMFCOutlookBar || CMFCReBar || CMFCToolBar))
//             continue;                                                    // 0x17853a..0x178597, 0x234e20
//         pWnd->EnableWindow(!bCustMode);                                  // 0x2a9b30
//     }
//     CDockingManager* pDM = m_pParentFrame is CMDIFrameWndEx ? &+0x228 : CFrameWndEx ? &+0x380 : COleIPFrameWndEx / COleDocIPFrameWndEx ? &+0x428 : NULL;
//     if (pDM) for each CWnd* pMiniFrame in pDM->m_lstMiniFrames (+0x180):
//         if (pMiniFrame && pMiniFrame->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) {
//             CWnd* pPane = pMiniFrame->vslot 0x360 (108);
//             if (pPane == NULL || !pPane->IsKindOf(RUNTIME_CLASS(CMFCBaseToolBar))) pMiniFrame->EnableWindow(!bCustMode);
//         }
//     ::LockWindowUpdate(m_pParentFrame->m_hWnd);
//     CMFCToolBar::SetCustomizeMode(bCustMode);                            // 0x1527e0
//     m_pParentFrame->SendMessage(AFX_WM_CUSTOMIZETOOLBAR, bCustMode, 0);
//     ::LockWindowUpdate(NULL);
//     if (!bCustMode) { CFrameWnd* pActive = m_pParentFrame->GetActiveFrame(); if (pActive) pActive->PostMessage(WM_SETFOCUS /*7*/, 0, 0); }   // vslot 0x2f0 (94)
// DEVIATIONS: the mini-frame walk is dropped -- OpenMFC's frames embed no
// CDockingManager (core/frame/CMDIFrameWndEx.cpp keeps it in a side table)
// and CDockingManager::GetMiniFrames is an afxdockingmanager.h inline over a
// list that featurepack/docking/CDockingManager.cpp keeps in its own side
// table (deviation 4).  GetActiveFrame goes to the exported CFrameWnd base
// thunk (deviation 1).  Retail reads m_pParentFrame unchecked; with a NULL
// frame (or one without a window) this body only performs the
// CMFCToolBar::SetCustomizeMode call and skips the child walk, the two
// LockWindowUpdate calls, the AFX_WM_CUSTOMIZETOOLBAR send and the
// WM_SETFOCUS post.
// Symbol: ?SetFrameCustMode@CMFCToolBarsCustomizeDialog@@IEAAXH@Z
extern "C" void MS_ABI impl__SetFrameCustMode_CMFCToolBarsCustomizeDialog__IEAAXH_Z(void* pThis, int bCustMode) {
    if (pThis == nullptr) return;
    S_CustDlg* d = D(pThis);
    WaitCursorScope wait;
    const HWND hFrame = WndHandle(d->m_pParentFrame);
    if (hFrame == nullptr) {
        impl__SetCustomizeMode_CMFCToolBar__SAHH_Z(bCustMode);
        return;
    }

    for (CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(hFrame, GW_CHILD));
         pWnd != nullptr;
         pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(WndHandle(pWnd), GW_HWNDNEXT))) {
        const CRuntimeClass* pRTC = pWnd->GetRuntimeClass();   // vslot 0 under both vtable ABIs
        if (pRTC != nullptr &&
            (impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(pRTC, impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ()) ||
             impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(pRTC, impl__GetThisClass_CDockSite__SAPEAUCRuntimeClass__XZ()) ||
             impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(pRTC, impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ()) ||
             impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(pRTC, impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ()) ||
             impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(pRTC, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ()))) {
            continue;
        }
        impl__EnableWindow_CWnd__QEAAHH_Z(pWnd, bCustMode ? FALSE : TRUE);
    }

    // TODO(clean-room): retail's CDockingManager::m_lstMiniFrames walk (deviation 4).

    ::LockWindowUpdate(hFrame);
    impl__SetCustomizeMode_CMFCToolBar__SAHH_Z(bCustMode);
    ::SendMessage(hFrame, impl__AFX_WM_CUSTOMIZETOOLBAR__3IA, static_cast<WPARAM>(static_cast<INT_PTR>(bCustMode)), 0);
    ::LockWindowUpdate(nullptr);

    if (bCustMode == 0) {
        CWnd* pActive = static_cast<CWnd*>(impl__GetActiveFrame_CFrameWnd__UEAAPEAV1_XZ(d->m_pParentFrame));
        if (pActive != nullptr) {
            ::PostMessage(WndHandle(pActive), WM_SETFOCUS, 0, 0);
        }
    }
}

// Retail (RVA 0x1787d0, mfc140u; absent from mfc140u_rva_symbols.json -- the
// body right before OnInitDialog, confirmed through the full export table),
// fully transcribed:
//     m_pToolbarsPage->ShowToolBar(pToolBar, bShow);                       // tail jump 0x17c600
// The sibling thunk is still a placeholder (featurepack/customize/
// CMFCToolBarsListPropertyPage.cpp); retail dereferences m_pToolbarsPage
// unchecked, a NULL page is skipped here.
// Symbol: ?ShowToolBar@CMFCToolBarsCustomizeDialog@@IEAAXPEAVCMFCToolBar@@H@Z
extern "C" void MS_ABI impl__ShowToolBar_CMFCToolBarsCustomizeDialog__IEAAXPEAVCMFCToolBar__H_Z(void* pThis, void* pToolBar, int bShow) {
    if (pThis == nullptr) return;
    S_CustDlg* d = D(pThis);
    if (d->m_pToolbarsPage == nullptr) return;
    impl__ShowToolBar_CMFCToolBarsListPropertyPage__QEAAXPEAVCMFCToolBar__H_Z(d->m_pToolbarsPage, pToolBar, bShow);
}

// Retail (RVA 0x178d60, mfc140u), fully transcribed:
//     m_pToolbarsPage->m_bUserDefinedToolbars /*+0x708*/ = bEnable;
// Retail dereferences m_pToolbarsPage unchecked; a NULL page is skipped here.
// Symbol: ?EnableUserDefinedToolbars@CMFCToolBarsCustomizeDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableUserDefinedToolbars_CMFCToolBarsCustomizeDialog__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    S_CustDlg* d = D(pThis);
    if (d->m_pToolbarsPage == nullptr) return;
    std::memcpy(static_cast<unsigned char*>(d->m_pToolbarsPage) + kOffToolbarsPageUserDefined, &bEnable, sizeof bEnable);
}

// ===========================================================================
// Populating the categories from menus / toolbars
// ===========================================================================

// Retail (RVA 0x177f00, mfc140u), fully transcribed:
//     CString strCategory;  ENSURE(strCategory.LoadString(uiCategoryId));   // 0x2aee00 / 0xdb70, throw 0x177fa1
//     return AddToolBar(strCategory, uiToolbarResId);                       // 0x177fb0
// Symbol: ?AddToolBar@CMFCToolBarsCustomizeDialog@@QEAAHII@Z
extern "C" int MS_ABI impl__AddToolBar_CMFCToolBarsCustomizeDialog__QEAAHII_Z(void* pThis, unsigned int uiCategoryId, unsigned int uiToolbarResId) {
    if (pThis == nullptr) return FALSE;
    CString strCategory;
    if (!LoadResString(strCategory, uiCategoryId)) return FALSE;
    return impl__AddToolBar_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(pThis, strCategory.GetString(), uiToolbarResId);
}

// Retail (RVA 0x177fb0, mfc140u), fully transcribed:
//     ENSURE(uiToolbarResId != 0);                                          // 0x1780ba
//     HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(uiToolbarResId), RT_TOOLBAR /*0xf1*/);   // 0x2aeb50
//     HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(uiToolbarResId), RT_TOOLBAR);  if (!hRsrc) return FALSE;
//     HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);  if (!hGlobal) return FALSE;
//     CToolBarData* pData = (CToolBarData*)::LockResource(hGlobal);  if (!pData) return FALSE;
//     for (int i = 0; i < pData->wItemCount; i++) {
//         UINT uiCmd = pData->items()[i];
//         if (uiCmd != 0) {                                                 // separators are skipped
//             CMFCToolBarButton button(uiCmd, -1, NULL, FALSE, FALSE);      // 0x15bf40
//             AddButton(lpszCategory, button, -1);                          // 0x1777c0
//         }
//     }
//     return TRUE;
// (uiToolbarResId is passed through MAKEINTRESOURCE's low word, movzwl at 0x177fe0.)
// Symbol: ?AddToolBar@CMFCToolBarsCustomizeDialog@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__AddToolBar_CMFCToolBarsCustomizeDialog__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszCategory, unsigned int uiToolbarResId) {
    if (pThis == nullptr) return FALSE;
    if (uiToolbarResId == 0) { ThrowInvalidArg(); return FALSE; }
    const wchar_t* lpszRes = MAKEINTRESOURCEW(static_cast<unsigned short>(uiToolbarResId));
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszRes, MAKEINTRESOURCEW(kRtToolbar));
    HRSRC hRsrc = ::FindResourceW(hInst, lpszRes, MAKEINTRESOURCEW(kRtToolbar));
    if (hRsrc == nullptr) return FALSE;
    HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
    if (hGlobal == nullptr) return FALSE;
    const ToolBarResData* pData = static_cast<const ToolBarResData*>(::LockResource(hGlobal));
    if (pData == nullptr) return FALSE;
    for (int i = 0; i < static_cast<int>(pData->wItemCount); i++) {
        const unsigned int uiCmd = pData->items[i];
        if (uiCmd == 0) continue;
        StackButton button(uiCmd, -1, nullptr, FALSE, FALSE);
        impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(pThis, lpszCategory, button.get(), -1);
    }
    return TRUE;
}

// Retail (RVA 0x1780c0, mfc140u), fully transcribed:
//     CMenu menu;                                                           // inline ctor: vfptr 0x1802e2f58, m_hMenu = NULL
//     HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(uiMenuResId), RT_MENU /*4*/);   // 0x2aeb50
//     if (!menu.Attach(::LoadMenu(hInst, MAKEINTRESOURCE(uiMenuResId)))) return FALSE;       // 0x2a8100
//     AddMenuCommands(&menu, FALSE, NULL, NULL);                            // 0x178d70
//     return TRUE;
//     // ~CMenu (inlined 0x178136..0x178173): if (m_hMenu) { AfxGetModuleThreadState()->m_pmapHMENU->RemoveKey(m_hMenu); ::DestroyMenu(m_hMenu); }
// (uiMenuResId goes through MAKEINTRESOURCE's low word, movzwl at 0x1780e6.)
// Symbol: ?AddMenu@CMFCToolBarsCustomizeDialog@@QEAAHI@Z
extern "C" int MS_ABI impl__AddMenu_CMFCToolBarsCustomizeDialog__QEAAHI_Z(void* pThis, unsigned int uiMenuResId) {
    if (pThis == nullptr) return FALSE;
    const wchar_t* lpszRes = MAKEINTRESOURCEW(static_cast<unsigned short>(uiMenuResId));
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszRes, MAKEINTRESOURCEW(kRtMenu));
    StackMenu menu;
    if (!impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(menu.get(), ::LoadMenuW(hInst, lpszRes))) return FALSE;
    impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(pThis, menu.get(), FALSE, nullptr, nullptr);
    return TRUE;
}

// Retail (RVA 0x178d70, mfc140u), transcribed:
//     ENSURE(pMenu != NULL);                                                // 0x179357
//     BOOL bWindowsMenu = FALSE;
//     int iCount = ::GetMenuItemCount(pMenu->m_hMenu);
//     for (int i = 0; i < iCount; i++) {
//         UINT uiCmd = ::GetMenuItemID(pMenu->m_hMenu, i);
//         CString strText;  pMenu->GetMenuString(i, strText, MF_BYPOSITION);   // 0x2a81c0
//         if (!m_bSaveMenuAmps) strText.Remove(L'&');                       // 0x12370
//         if (uiCmd == 0) continue;                                         // separator, 0x178e4b
//         if (uiCmd == (UINT)-1) {                                          // popup, 0x1790af
//             CMenu* pSubMenu = CMenu::FromHandle(::GetSubMenu(pMenu->m_hMenu, i));   // 0x2a80a0
//             UINT uiTearOffId = 0;
//             if (afxMenuTearOffManager /*0x3be260*/) uiTearOffId = afxMenuTearOffManager->Parse(strText);   // 0x900d0
//             CString strCategory = strText;  strCategory.Remove(L'&');
//             if (lpszCategory != NULL) strCategory = lpszCategory;
//             if (m_bAutoSetFromMenus) {                                    // 0x17912a
//                 if (bPopup) {
//                     CMFCToolBarMenuButton btn((UINT)-1, pSubMenu ? pSubMenu->m_hMenu : NULL, -1, strText, FALSE);   // 0x172870
//                     btn.SetTearOff(uiTearOffId);                          // 0x175f10
//                     AddButton(strCategory, btn, -1);
//                 }
//                 CString strPath = lpszMenuPath ? lpszMenuPath : L"";  strPath += strText;
//                 AddMenuCommands(pSubMenu, bPopup, strCategory, strPath);
//             } else {
//                 AddMenuCommands(pSubMenu, bPopup, strCategory, NULL);
//             }
//             continue;
//         }
//         if (bPopup && uiCmd - 0xE130 <= 0xF) bWindowsMenu = TRUE;         // ID_WINDOW_NEW .. ID_WINDOW_NEW+15, 0x178e5d
//         if (lpszCategory != NULL && afxUserToolsManager != NULL && afxUserToolsManager->m_uiCmdToolsDummy /*+0x40*/ == uiCmd) {
//             AddUserTools(lpszCategory);  continue;                        // 0x1798c0
//         }
//         CMFCToolBarButton button(uiCmd, -1, strText, FALSE, FALSE);       // 0x15bf40
//         if (lpszMenuPath != NULL) {                                       // 0x178ec7
//             CString strCustomText = CString(lpszMenuPath) + button.m_strText;   // 0xdcb0 / 0x78c90
//             LPWSTR lpsz = strCustomText.GetBuffer();                      // 0x31c0 when shared
//             int len = wcslen(lpsz);
//             for (int j = 0; j < len - 1; j++) if (lpsz[j] == L' ') ::CharUpperBuffW(lpsz + j + 1, 1);   // 0x178f80..0x178fa2
//             strCustomText.ReleaseBuffer();                                // wcsnlen + ATLENSURE (E_INVALIDARG at 0x17934c)
//             strCustomText.Remove(L' ');
//             button.m_strTextCustom = strCustomText.Left(wcscspn(strCustomText, L"\t"));   // 0x33f5c8, 0x128f0, +0x40
//         }
//         AddButton(lpszCategory ? lpszCategory : m_strAllCommands, button, -1);   // 0x17908a..0x17909b
//     }
//     if (bWindowsMenu && lpszCategory != NULL && m_pParentFrame && m_pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))   // 0x1792ab
//         && frame->m_uiWindowsDlgMenuId /*+0x1f0*/ != 0 && frame->m_bShowWindowsDlgAlways /*+0x200*/) {
//         CMFCToolBarButton button(frame->m_uiWindowsDlgMenuId, -1, frame->m_strWindowsDlgMenuText /*+0x1e0*/, FALSE, FALSE);
//         AddButton(lpszCategory, button, -1);
//     }
// DEVIATIONS: afxMenuTearOffManager is a DLL-internal global with no OpenMFC
// equivalent (core/app/CWinAppEx.cpp EnableTearOffMenus records only a flag),
// so uiTearOffId stays 0 -- and the sibling SetTearOff thunk is itself still
// an empty placeholder whose parameter list drops `this`
// (featurepack/toolbar/CMFCToolBarMenuButton.cpp).
// The user-tools arm is gated on the exported ?afxUserToolsManager@@ (NULL in
// this tree) and AddUserTools is a documented stub (see below).  The trailing
// "Windows..." button is dropped: m_uiWindowsDlgMenuId / m_bShowWindowsDlgAlways
// sit at the retail offsets in OpenMFC's CMDIFrameWndEx, but the menu text
// (retail +0x1e0) lives in core/frame/CMDIFrameWndEx.cpp's side table, which
// this file cannot reach (deviation 4, headerRequests).
// Symbol: ?AddMenuCommands@CMFCToolBarsCustomizeDialog@@QEAAXPEBVCMenu@@HPEB_W1@Z
extern "C" void MS_ABI impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(
    void* pThis, const CMenu* pMenu, int bPopup, const wchar_t* lpszCategory, const wchar_t* lpszMenuPath) {
    if (pThis == nullptr) return;
    if (pMenu == nullptr) { ThrowInvalidArg(); return; }
    S_CustDlg* d = D(pThis);
    const HMENU hMenu = MenuHandle(pMenu);

    bool bWindowsMenu = false;
    const int iCount = ::GetMenuItemCount(hMenu);
    for (int i = 0; i < iCount; i++) {
        const unsigned int uiCmd = ::GetMenuItemID(hMenu, i);
        CString strText;
        impl__GetMenuStringW_CMenu__QEBAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__I_Z(pMenu, static_cast<UINT>(i), strText, MF_BYPOSITION);
        if (d->m_bSaveMenuAmps == 0) StrRemoveChar(strText, L'&');

        if (uiCmd == 0) continue;

        if (uiCmd == static_cast<unsigned int>(-1)) {
            CMenu* pSubMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(::GetSubMenu(hMenu, i));
            const unsigned int uiTearOffId = 0;   // DEVIATION: no afxMenuTearOffManager on this side
            CString strCategory = strText;
            StrRemoveChar(strCategory, L'&');
            if (lpszCategory != nullptr) strCategory = lpszCategory;

            if (d->m_bAutoSetFromMenus != 0) {
                if (bPopup) {
                    StackMenuButton btn(static_cast<unsigned int>(-1), MenuHandle(pSubMenu), -1, strText.GetString(), FALSE);
                    impl__SetTearOff_CMFCToolBarMenuButton__UEAAXI_Z(btn.get(), uiTearOffId);
                    impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(pThis, strCategory.GetString(), btn.get(), -1);
                }
                CString strPath(lpszMenuPath != nullptr ? lpszMenuPath : L"");
                strPath += strText;
                impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(pThis, pSubMenu, bPopup, strCategory.GetString(), strPath.GetString());
            } else {
                impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(pThis, pSubMenu, bPopup, strCategory.GetString(), nullptr);
            }
            continue;
        }

        if (bPopup && uiCmd - 0xE130u <= 0xFu) bWindowsMenu = true;

        if (lpszCategory != nullptr && impl__afxUserToolsManager__3PEAVCUserToolsManager__EA != nullptr) {
            // TODO(clean-room): retail compares uiCmd with afxUserToolsManager->m_uiCmdToolsDummy (+0x40)
            // and calls AddUserTools(lpszCategory) on a match; the manager's retail layout is not
            // modelled by OpenMFC and the pointer is NULL in this tree (deviation 5).
        }

        StackButton button(uiCmd, -1, strText.GetString(), FALSE, FALSE);
        if (lpszMenuPath != nullptr) {
            CString strCustomText(lpszMenuPath);
            strCustomText += button.get()->m_strText;
            {
                wchar_t* lpsz = strCustomText.GetBuffer();
                const int len = lpsz != nullptr ? static_cast<int>(std::wcslen(lpsz)) : 0;
                for (int j = 0; j < len - 1; j++) {
                    if (lpsz[j] == L' ') ::CharUpperBuffW(lpsz + j + 1, 1);
                }
                strCustomText.ReleaseBuffer();
            }
            StrRemoveChar(strCustomText, L' ');
            const int iTab = static_cast<int>(std::wcscspn(strCustomText.GetString(), L"\t"));
            button.get()->m_strTextCustom = strCustomText.Left(iTab);
        }
        impl__AddButton_CMFCToolBarsCustomizeDialog__QEAAXPEB_WAEBVCMFCToolBarButton__H_Z(
            pThis, lpszCategory != nullptr ? lpszCategory : d->m_strAllCommands.GetString(), button.get(), -1);
    }

    if (bWindowsMenu && lpszCategory != nullptr) {
        // TODO(clean-room): retail's CMDIFrameWndEx "Windows..." button (deviation 4, see above).
    }
}

// Retail (RVA 0x1798c0, mfc140u):
//     ENSURE(lpszCategory != NULL);                                         // 0x179973
//     for (POSITION pos = afxUserToolsManager->m_lstUserTools.GetHeadPosition(); pos; ) {   // +0x8 CObList, head at +0x10
//         CUserTool* pTool = (CUserTool*)GetNext(pos);
//         CMFCToolBarButton button(pTool->m_uiCmdId /*+0x20*/, 0, pTool->m_strLabel /*+0x8*/, FALSE, FALSE);   // 0x15bf40
//         AddButton(lpszCategory, button, -1);                              // 0x1777c0
//     }
// STUB beyond the ENSURE: retail reads afxUserToolsManager unchecked (its
// callers gate on it), and the walk needs the retail CUserToolsManager /
// CUserTool layouts, which OpenMFC's classes do not have; the exported
// ?afxUserToolsManager@@ is NULL in this tree, so the walk is not reproduced
// and the body returns after the argument check (deviation 5).
// Symbol: ?AddUserTools@CMFCToolBarsCustomizeDialog@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__AddUserTools_CMFCToolBarsCustomizeDialog__IEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory) {
    if (pThis == nullptr) return;
    if (lpszCategory == nullptr) { ThrowInvalidArg(); return; }
    // TODO(clean-room): the m_lstUserTools walk (see above).
}

// Retail (RVA 0x179770, mfc140u; the ctor calls it at 0x17720b), transcribed:
//     CDocManager* pDocManager = AfxGetModuleState()->m_pCurrentWinApp->m_pDocManager;   // 0x133930, +0x8, +0xb8
//     if (pDocManager) {
//         for (POSITION pos = pDocManager->GetFirstDocTemplatePosition(); pos; ) {   // vslot 0x30 (6)
//             CDocTemplate* pTemplate = pDocManager->GetNextDocTemplate(pos);       // vslot 0x38 (7)
//             if (pTemplate && pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate))) {   // 0x234cf0, 0x329588
//                 CMenu* pMenu = CMenu::FromHandle(((CMultiDocTemplate*)pTemplate)->m_hMenuShared /*+0xf0*/);   // 0x2a80a0
//                 if (pMenu) AddMenuCommands(pMenu, FALSE, NULL, NULL);     // 0x178d70
//             }
//         }
//     }
//     CMenu* pFrameMenu = CMenu::FromHandle(m_pParentFrame->m_hMenuDefault /*+0xf0*/);
//     if (pFrameMenu == NULL) {
//         CMFCMenuBar* pMenuBar = m_pParentFrame is CMDIFrameWndEx ? +0x6a0 : CFrameWndEx ? +0x300 : NULL;   // CFrameImpl::m_pMenuBar
//         if (pMenuBar) pFrameMenu = CMenu::FromHandle(pMenuBar->m_hDefaultMenu /*+0x1368*/);
//     }
//     if (pFrameMenu) AddMenuCommands(pFrameMenu, FALSE, NULL, NULL);
// DEVIATIONS: the CDocManager vslots are reached through the exported
// CWinApp::GetFirstDocTemplatePosition / GetNextDocTemplate (the way
// featurepack/menu/CMFCMenuBar.cpp::ResetImages does; a NULL AfxGetApp() skips
// the walk instead of faulting); m_hMenuShared is read through
// MultiDocTemplateSharedMenu (::IsMenu-validated, see its note -- the slot is
// never written on this side, so the branch is effectively dead); the frame's
// m_hMenuDefault is FrameDefaultMenu (::GetMenu of the frame window, see its
// note); the CFrameImpl m_pMenuBar fallback is dropped (deviation 4).
// Symbol: ?SetupFromMenus@CMFCToolBarsCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__SetupFromMenus_CMFCToolBarsCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CustDlg* d = D(pThis);

    if (CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        void* pos = impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(pApp);
        while (pos != nullptr) {
            CDocTemplate* pTemplate = impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(pApp, &pos);
            if (pTemplate == nullptr ||
                !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pTemplate, impl__GetThisClass_CMultiDocTemplate__SAPEAUCRuntimeClass__XZ())) {
                continue;
            }
            CMenu* pMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(MultiDocTemplateSharedMenu(pTemplate));
            if (pMenu != nullptr) {
                impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(pThis, pMenu, FALSE, nullptr, nullptr);
            }
        }
    }

    CMenu* pFrameMenu = impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(FrameDefaultMenu(d->m_pParentFrame));
    // TODO(clean-room): retail's CFrameImpl::m_pMenuBar->m_hDefaultMenu fallback (deviation 4).
    if (pFrameMenu != nullptr) {
        impl__AddMenuCommands_CMFCToolBarsCustomizeDialog__QEAAXPEBVCMenu__HPEB_W1_Z(pThis, pFrameMenu, FALSE, nullptr, nullptr);
    }
}

// ===========================================================================
// Category / command list population and lookups
// ===========================================================================

namespace {

// FillCategoriesComboBox (0x179360) and FillCategoriesListBox (0x1794c0) are
// the same body over CB_ADDSTRING/CB_SETITEMDATA (0x143/0x151) versus
// LB_ADDSTRING/LB_SETITEMDATA (0x180/0x19a); retail (transcribed):
//     for (POSITION pos = m_strCategoriesList.GetHeadPosition(); pos; ) {  // inline node walk from +0x1d0
//         CString strCategory = m_strCategoriesList.GetNext(pos);         // CloneData 0xdd40
//         CObList* pList;  m_ButtonsByCategory.Lookup(strCategory, pList);   // 0x179a60, result ignored
//         if (!bAddEmpty) {                                                // 0x1793c4
//             BOOL bHasCommands = FALSE;
//             for (each button in *pList)                                  // head +0x8, data +0x10, m_nID +0x24
//                 if (pButton->m_nID != 0 && pButton->m_nID != (UINT)-1) { bHasCommands = TRUE; break; }   // ((id + 1) & ~1) != 0
//             if (!bHasCommands) continue;
//         }
//         int iIndex = ::SendMessage(wnd.m_hWnd, xx_ADDSTRING, 0, (LPARAM)(LPCTSTR)strCategory);
//         ::SendMessage(wnd.m_hWnd, xx_SETITEMDATA, iIndex, (LPARAM)pList);
//     }
//     CObList* pAll;  m_ButtonsByCategory.Lookup(m_strAllCommands, pAll);   // result ignored
//     int iIndex = ::SendMessage(wnd.m_hWnd, xx_ADDSTRING, 0, (LPARAM)(LPCTSTR)m_strAllCommands);
//     ::SendMessage(wnd.m_hWnd, xx_SETITEMDATA, iIndex, (LPARAM)pAll);    // tail jump
// Retail leaves pList indeterminate when Lookup fails and walks it unchecked;
// here it starts NULL and a NULL list counts as "no commands".
void FillCategories(const S_CustDlg* d, HWND hWnd, int bAddEmpty, UINT msgAddString, UINT msgSetItemData) {
    if (const StrListData* cats = StringListData(CategoryList(d))) {
        StrListData::POSITION pos = cats->GetHeadPosition();
        while (pos != StrListData::POSITION(nullptr)) {
            CString strCategory = cats->GetNext(pos);
            CObList* pList = nullptr;
            CatLookup(d->m_ButtonsByCategory, strCategory.GetString(), pList);
            if (!bAddEmpty) {
                bool bHasCommands = false;
                if (const ObListData* items = ListData(pList)) {
                    ObListData::POSITION posItem = items->GetHeadPosition();
                    while (posItem != ObListData::POSITION(nullptr)) {
                        const CMFCToolBarButton* pButton = Btn(items->GetNext(posItem));
                        if (pButton != nullptr && pButton->m_nID != 0 && pButton->m_nID != static_cast<UINT>(-1)) {
                            bHasCommands = true;
                            break;
                        }
                    }
                }
                if (!bHasCommands) continue;
            }
            const LRESULT iIndex = ::SendMessage(hWnd, msgAddString, 0, reinterpret_cast<LPARAM>(strCategory.GetString()));
            ::SendMessage(hWnd, msgSetItemData, static_cast<WPARAM>(static_cast<int>(iIndex)), reinterpret_cast<LPARAM>(pList));
        }
    }
    CObList* pAll = nullptr;
    CatLookup(d->m_ButtonsByCategory, d->m_strAllCommands.GetString(), pAll);
    const LRESULT iIndex = ::SendMessage(hWnd, msgAddString, 0, reinterpret_cast<LPARAM>(d->m_strAllCommands.GetString()));
    ::SendMessage(hWnd, msgSetItemData, static_cast<WPARAM>(static_cast<int>(iIndex)), reinterpret_cast<LPARAM>(pAll));
}

} // namespace

// Retail (RVA 0x179360, mfc140u): see FillCategories.
// Symbol: ?FillCategoriesComboBox@CMFCToolBarsCustomizeDialog@@QEBAXAEAVCComboBox@@H@Z
extern "C" void MS_ABI impl__FillCategoriesComboBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCComboBox__H_Z(const void* pThis, CWnd* pWndCategory, int bAddEmpty) {
    if (pThis == nullptr || pWndCategory == nullptr) return;
    FillCategories(D(pThis), WndHandle(pWndCategory), bAddEmpty, CB_ADDSTRING, CB_SETITEMDATA);
}

// Retail (RVA 0x1794c0, mfc140u): see FillCategories.
// Symbol: ?FillCategoriesListBox@CMFCToolBarsCustomizeDialog@@QEBAXAEAVCListBox@@H@Z
extern "C" void MS_ABI impl__FillCategoriesListBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCListBox__H_Z(const void* pThis, CWnd* pWndCategory, int bAddEmpty) {
    if (pThis == nullptr || pWndCategory == nullptr) return;
    FillCategories(D(pThis), WndHandle(pWndCategory), bAddEmpty, LB_ADDSTRING, LB_SETITEMDATA);
}

// Retail (RVA 0x179620, mfc140u), fully transcribed:
//     ::SendMessage(wndListOfCommands.m_hWnd, LB_RESETCONTENT /*0x184*/, 0, 0);
//     CObList* pAll;
//     if (!m_ButtonsByCategory.Lookup(m_strAllCommands, pAll)) return;     // 0x179a60
//     for (each CMFCToolBarButton* pButton in *pAll) {                     // head +0x8, data +0x10, unchecked
//         LPCTSTR lpszText = pButton->m_strTextCustom /*+0x40*/.IsEmpty() ? pButton->m_strText /*+0x38*/ : pButton->m_strTextCustom;
//         int iIndex = ::SendMessage(hWnd, LB_ADDSTRING /*0x180*/, 0, (LPARAM)lpszText);
//         ::SendMessage(hWnd, LB_SETITEMDATA /*0x19a*/, iIndex, pButton->m_nID /*+0x24*/);
//     }
// Symbol: ?FillAllCommandsList@CMFCToolBarsCustomizeDialog@@UEBAXAEAVCListBox@@@Z
extern "C" void MS_ABI impl__FillAllCommandsList_CMFCToolBarsCustomizeDialog__UEBAXAEAVCListBox___Z(const void* pThis, CWnd* pWndListOfCommands) {
    if (pThis == nullptr || pWndListOfCommands == nullptr) return;
    const S_CustDlg* d = D(pThis);
    const HWND hWnd = WndHandle(pWndListOfCommands);
    ::SendMessage(hWnd, LB_RESETCONTENT, 0, 0);
    CObList* pAll = nullptr;
    if (!CatLookup(d->m_ButtonsByCategory, d->m_strAllCommands.GetString(), pAll)) return;
    const ObListData* items = ListData(pAll);
    if (items == nullptr) return;
    ObListData::POSITION pos = items->GetHeadPosition();
    while (pos != ObListData::POSITION(nullptr)) {
        const CMFCToolBarButton* pButton = Btn(items->GetNext(pos));
        if (pButton == nullptr) continue;   // retail would fault on a NULL node
        const wchar_t* lpszText = pButton->m_strTextCustom.IsEmpty() ? pButton->m_strText.GetString() : pButton->m_strTextCustom.GetString();
        const LRESULT iIndex = ::SendMessage(hWnd, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(lpszText));
        ::SendMessage(hWnd, LB_SETITEMDATA, static_cast<WPARAM>(static_cast<int>(iIndex)), static_cast<LPARAM>(pButton->m_nID));
    }
}

// Retail (RVA 0x179710, mfc140u), fully transcribed:
//     CObList* pAll;
//     if (!m_ButtonsByCategory.Lookup(m_strAllCommands, pAll)) return NULL;   // 0x179a60
//     ENSURE(pAll != NULL);                                                 // 0x179768
//     for (each node) { CMFCToolBarButton* p = node->data;  ENSURE(p);  if (p->m_nID == uiCmd) return p->m_strText; }   // +0x24 / +0x38
//     return NULL;
// Symbol: ?GetCommandName@CMFCToolBarsCustomizeDialog@@QEBAPEB_WI@Z
extern "C" const wchar_t* MS_ABI impl__GetCommandName_CMFCToolBarsCustomizeDialog__QEBAPEB_WI_Z(const void* pThis, unsigned int uiCmd) {
    if (pThis == nullptr) return nullptr;
    const S_CustDlg* d = D(pThis);
    CObList* pAll = nullptr;
    if (!CatLookup(d->m_ButtonsByCategory, d->m_strAllCommands.GetString(), pAll)) return nullptr;
    if (pAll == nullptr) { ThrowInvalidArg(); return nullptr; }
    const ObListData* items = ListData(pAll);
    if (items == nullptr) return nullptr;
    ObListData::POSITION pos = items->GetHeadPosition();
    while (pos != ObListData::POSITION(nullptr)) {
        const CMFCToolBarButton* p = Btn(items->GetNext(pos));
        if (p == nullptr) { ThrowInvalidArg(); return nullptr; }
        if (p->m_nID == uiCmd) return p->m_strText.GetString();
    }
    return nullptr;
}

// Retail (RVA 0x179980, mfc140u), fully transcribed:
//     int nCount = 0;
//     for (each node of lstCommands) {                                     // head +0x8, data +0x10
//         CMFCToolBarButton* p = node->data;  ENSURE(p);                   // 0x1799e6
//         ATLENSURE(lpszItemName != NULL);                                 // E_FAIL at 0x1799db (tested per node, after the first ENSURE)
//         if (wcscmp(p->m_strText /*+0x38*/, lpszItemName) == 0) nCount++;
//     }
//     return nCount;
// The caller's CObList reaches this DLL through the exported CObList thunks
// (MFC does not inline them), so its contents are in the OpenMFC side table
// (deviation 2).
// Symbol: ?GetCountInCategory@CMFCToolBarsCustomizeDialog@@QEBAHPEB_WAEBVCObList@@@Z
extern "C" int MS_ABI impl__GetCountInCategory_CMFCToolBarsCustomizeDialog__QEBAHPEB_WAEBVCObList___Z(const void* pThis, const wchar_t* lpszItemName, const CObList* pListCommands) {
    (void)pThis;   // retail never reads this
    if (pListCommands == nullptr) return 0;
    const ObListData* items = ListData(pListCommands);
    if (items == nullptr) return 0;
    int nCount = 0;
    ObListData::POSITION pos = items->GetHeadPosition();
    while (pos != ObListData::POSITION(nullptr)) {
        const CMFCToolBarButton* p = Btn(items->GetNext(pos));
        if (p == nullptr) { ThrowInvalidArg(); return nCount; }
        if (lpszItemName == nullptr) { ThrowOle(kEFail); return nCount; }
        if (std::wcscmp(p->m_strText.GetString(), lpszItemName) == 0) nCount++;
    }
    return nCount;
}

// Retail (RVA 0x1799f0, mfc140u; vftable 0x1803190d8 slot 105, absent from
// mfc140u_rva_symbols.json but confirmed through the full export table), fully
// transcribed:
//     CMFCImageEditorDialog dlg(&bitmap, pWndParent, nBitsPerPixel);       // 0x6f260, a 0x2e60-byte stack object
//     BOOL bResult = dlg.DoModal() == IDOK;                                // 0x2088b0
//     return bResult;                                                      // ~dlg at 0x6f3d4 (not exported)
// STUB: OpenMFC's ??0CMFCImageEditorDialog thunk is an argument-silencing
// placeholder (featurepack/controls/CMFCImageEditorDialog.cpp) that
// constructs nothing, so CDialog::DoModal cannot be run on the block and the
// editor cannot be shown; FALSE ("not edited") is the safe result.
// Symbol: ?OnEditToolbarMenuImage@CMFCToolBarsCustomizeDialog@@UEAAHPEAVCWnd@@AEAVCBitmap@@H@Z
extern "C" int MS_ABI impl__OnEditToolbarMenuImage_CMFCToolBarsCustomizeDialog__UEAAHPEAVCWnd__AEAVCBitmap__H_Z(void* pThis, CWnd* pWndParent, void* pBitmap, int nBitsPerPixel) {
    (void)pThis; (void)pWndParent; (void)pBitmap; (void)nBitsPerPixel;
    return FALSE;
}
