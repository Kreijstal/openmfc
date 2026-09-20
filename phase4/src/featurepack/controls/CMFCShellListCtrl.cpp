// CMFCShellListCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <shlobj.h>
#include <shellapi.h>
#include <oleauto.h>
#include <time.h>
#include <cstring>
#include <cwchar>

// ===========================================================================
// CMFCShellListCtrl -- the shell-namespace list control.
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the entry RVA it was read from (all RVAs in this file are mfc140u).
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives; client (MSVC) code allocates the object with
// the retail member layout, which is pinned by ShellListCtrlLayout below.
//
// Retail object layout, read from the constructor (RVA 0x12a760) and cross-
// checked against the shipping afxshelllistctrl.h on this host
// (atlmfc/include/afxshelllistctrl.h: m_psfCurFolder, m_pidlCurFQ, m_nTypes,
// m_hwndRelatedTree, m_bContextMenu, m_bIsDesktop, m_bNoNotify, in that order):
//
//   +0x000  CMFCListCtrl base, 0x248 bytes: CWnd is 0xe8 (m_hWnd at +0x40);
//           CMFCHeaderCtrl m_wndHeader sits at +0xe8 (GetHeaderCtrl, RVA
//           0x77900, returns this+0xe8, and EnumObjects reads its m_hWnd at
//           +0x128); the CMFCListCtrl ctor (RVA 0x77970) then initialises the
//           qwords at +0x228 (-1), +0x230 (1) and +0x238 (0) -- afxlistctrl.h
//           declares m_clrSortedColumn, m_iSortedColumn, m_bAscending,
//           m_bMarkSortedColumn, m_hOldFont after m_wndHeader; nothing in this
//           file reads them.
//   +0x248  IShellFolder*  m_psfCurFolder     (ctor stores NULL)
//   +0x250  LPITEMIDLIST   m_pidlCurFQ        (ctor stores NULL)
//   +0x258  SHCONTF        m_nTypes           (ctor stores 0x60 =
//                                              SHCONTF_FOLDERS|SHCONTF_NONFOLDERS)
//   +0x260  HWND           m_hwndRelatedTree  (ctor stores NULL)
//   +0x268  BOOL           m_bContextMenu     (ctor stores TRUE)
//   +0x26c  BOOL           m_bIsDesktop       (ctor stores FALSE)
//   +0x270  BOOL           m_bNoNotify        (ctor stores FALSE)
//   sizeof == 0x278  (m_nObjectSize of the retail CRuntimeClass at
//                     0x18030eb88 in mfc140u, the descriptor
//                     ?GetRuntimeClass@CMFCShellListCtrl (RVA 0x12a750) returns)
//
// Retail vtable (0x18030e818 in mfc140u; slots read with the workflow's
// vtable dumper).  The slots the bodies below dispatch through are:
//   slot  92 (+0x2e0)  CMFCListCtrl::GetHeaderCtrl   (0x77900: returns this+0xe8)
//   slot  93 (+0x2e8)  CMFCListCtrl::Sort
//   slot  99 (+0x318)  Refresh
//   slot 100 (+0x320)  DisplayFolder(LPAFX_SHELLITEMINFO)
//   slot 103 (+0x338)  OnSetColumns
//   slot 104 (+0x340)  OnGetItemText
//   slot 105 (+0x348)  OnGetItemIcon
//   slot 106 (+0x350)  OnFormatFileSize
//   slot 107 (+0x358)  OnFormatFileDate
//   slot 108 (+0x360)  EnumObjects
//   slot 109 (+0x368)  DoDefault
//
// Deviations from retail, applied uniformly and repeated at each site:
//
//  (1) Virtual dispatch on `this` is devirtualized.  OpenMFC models no
//      MSVC-layout vtable for this class (the constructor below does NOT
//      store the retail vftable pointer -- see its comment), so every retail
//      `this->vslot` call above is made as a direct call to this file's own
//      thunk.  A client class that overrides one of those virtuals is
//      therefore not honoured from inside the DLL.  That is the convention
//      used throughout featurepack/ (e.g. CMFCPropertyGridCtrl::
//      PreSubclassWindow calls impl__Init_... directly).
//
//  (2) afxShellManager->m_pMalloc.  Retail keeps an IMalloc* at +0x8 of the
//      CShellManager (obtained through SHGetMalloc) and frees / allocates
//      every PIDL through it (IMalloc::Free = vslot 5, IMalloc::Alloc =
//      vslot 3), after an ENSURE(m_pMalloc != NULL).  OpenMFC's CShellManager
//      (include/openmfc/afxmfc.h:1685) carries no such member -- its PIDL
//      helpers in core/app/CShellManager.cpp use CoTaskMemAlloc/CoTaskMemFree,
//      which IS the allocator SHGetMalloc hands out -- so at every such site
//      the test collapses to `afxShellManager != NULL` (throwing the same
//      CInvalidArgException retail's ENSURE throws) and the call becomes
//      ::CoTaskMemFree / ::CoTaskMemAlloc.  Retail dereferences afxShellManager
//      without a NULL test at those sites; here a NULL afxShellManager throws
//      instead of faulting.
//
//  (3) GetHeaderCtrl().  Retail reads m_wndHeader.m_hWnd (+0x128) -- the
//      CMFCHeaderCtrl that CMFCListCtrl::InitHeader subclasses onto the list
//      view's real header window.  OpenMFC's CMFCListCtrl constructor and
//      InitHeader thunks (featurepack/controls/CMFCListCtrl.cpp) are still
//      placeholders, so +0x128 is never initialised here; the same window is
//      fetched with LVM_GETHEADER instead.
//
//  (4) CListCtrl / CWnd inline wrappers that retail expands to a raw
//      SendMessage (GetItemCount, DeleteAllItems, SetRedraw, GetNextItem,
//      GetItem, SetItemText, InsertItem, DeleteColumn, GetSelectedCount,
//      GetItemRect, HitTest, SetImageList) are written as the same
//      ::SendMessage on m_hWnd, exactly as the retail code does.  The one
//      wrapper with a side effect, SetImageList's CImageList::FromHandle
//      round trip (it only registers a temporary CImageList in the handle
//      map), is dropped and the raw HIMAGELIST is passed; the message sent is
//      identical.
//
//  (5) CString.  Retail calls the exported CSimpleStringT::Empty / SetString
//      / CStringT ctor(PCXSTR) / CloneData; OpenMFC's ABI-compatible CString
//      (include/openmfc/afxstr.h) performs the same operations inline, and is
//      used directly.  CStringT::MakeUpper and LoadString are called through
//      their exported thunks as retail does.
//
// Known dependency gaps outside this file (all reported as headerRequests):
//   * impl__afxShellManager (featurepack/CMFC_misc_stubs.cpp) is a NULL export
//     and CWinAppEx::InitShellManager (core/app/CWinAppEx.cpp) never sets it,
//     so -- exactly as retail behaves for an application that never called
//     InitShellManager() -- DisplayFolder returns E_FAIL and InitList returns
//     FALSE until that is fixed.
//   * CFile::GetStatus (core/file/CFile.cpp) stores raw FILETIME ticks in
//     CFileStatus::m_mtime where retail stores a CTime (__time64_t seconds);
//     OnFormatFileDate receives whatever GetStatus stored.
//   * The CMFCListCtrl (OnCreate, PreSubclassWindow, Sort, ctor/dtor) and
//     CMFCShellTreeCtrl::SelectPath thunks called from here are still
//     auto-generated placeholders with wrong parameter lists; they ignore
//     every argument, so calling them with the correct signature is
//     harmless, but they do nothing.
// ===========================================================================

namespace {

// File-local layout view of the retail object (offsets cited in the header).
struct ShellListCtrlLayout {
    unsigned char base[0x248];       // CMFCListCtrl (m_hWnd at +0x40)
    IShellFolder* m_psfCurFolder;    // +0x248
    LPITEMIDLIST  m_pidlCurFQ;       // +0x250
    DWORD         m_nTypes;          // +0x258  SHCONTF
    DWORD         pad_25c;
    HWND          m_hwndRelatedTree; // +0x260
    BOOL          m_bContextMenu;    // +0x268
    BOOL          m_bIsDesktop;      // +0x26c
    BOOL          m_bNoNotify;       // +0x270
    DWORD         pad_274;
};
static_assert(sizeof(ShellListCtrlLayout) == 0x278, "CMFCShellListCtrl: retail sizeof 0x278 (CRuntimeClass m_nObjectSize)");
static_assert(offsetof(ShellListCtrlLayout, m_psfCurFolder) == 0x248, "m_psfCurFolder +0x248");
static_assert(offsetof(ShellListCtrlLayout, m_pidlCurFQ) == 0x250, "m_pidlCurFQ +0x250");
static_assert(offsetof(ShellListCtrlLayout, m_nTypes) == 0x258, "m_nTypes +0x258");
static_assert(offsetof(ShellListCtrlLayout, m_hwndRelatedTree) == 0x260, "m_hwndRelatedTree +0x260");
static_assert(offsetof(ShellListCtrlLayout, m_bContextMenu) == 0x268, "m_bContextMenu +0x268");
static_assert(offsetof(ShellListCtrlLayout, m_bIsDesktop) == 0x26c, "m_bIsDesktop +0x26c");
static_assert(offsetof(ShellListCtrlLayout, m_bNoNotify) == 0x270, "m_bNoNotify +0x270");

// CWnd::m_hWnd (include/openmfc/afxwin.h places it at 64 == 0x40; every retail
// body in this file reads it as 0x40(%rcx)).
constexpr size_t kHWndOffset = 0x40;
static_assert(offsetof(CWnd, m_hWnd) == kHWndOffset, "CWnd::m_hWnd +0x40");

// CMFCShellTreeCtrl::m_hwndRelatedList.  Retail ?SetRelatedList@CMFCShellTreeCtrl
// (RVA 0x12d110) stores pList->m_hWnd at tree+0xf0 and pTree->m_hWnd at
// list+0x260; the tree ctor (RVA 0x12d010) zeroes +0xf0.  OnDestroy below
// clears it.
constexpr size_t kTreeRelatedListHwndOffset = 0xf0;

// AFX_SHELLITEMINFO (afxshellmanager.h: pParentFolder, pidlFQ, pidlRel).  Not
// declared in OpenMFC's headers.  The offsets are the ones every retail body
// below reads: (%rax) parent folder, 0x8(%rax) pidlFQ, 0x10(%rax) pidlRel.
struct ShellItemInfo {
    IShellFolder* pParentFolder;
    LPITEMIDLIST  pidlFQ;
    LPITEMIDLIST  pidlRel;
};
static_assert(sizeof(ShellItemInfo) == 0x18, "AFX_SHELLITEMINFO is 24 bytes (EnumObjects GlobalAlloc(GPTR, 0x18))");
static_assert(offsetof(ShellItemInfo, pidlFQ) == 0x8, "AFX_SHELLITEMINFO::pidlFQ +0x8");
static_assert(offsetof(ShellItemInfo, pidlRel) == 0x10, "AFX_SHELLITEMINFO::pidlRel +0x10");

// Field-for-field mirror of detail/CWinAppSupport.h's `struct _AFX_THREAD_STATE`
// (int nTempMapLock; int nWndCreateLock; void* pModuleState; void* pWndInit;
// void* hHookOldCbtFilter;), used only to read pWndInit in PreSubclassWindow --
// the same mirror featurepack/propertygrid/CMFCPropertyGridCtrl.cpp uses.
struct AfxThreadStateMirror {
    int nTempMapLock;
    int nWndCreateLock;
    void* pModuleState;
    void* pWndInit;
    void* hHookOldCbtFilter;
};
static_assert(sizeof(AfxThreadStateMirror) == 32, "AfxThreadStateMirror: five-field _AFX_THREAD_STATE");
static_assert(offsetof(AfxThreadStateMirror, pWndInit) == 16, "AfxThreadStateMirror::pWndInit");

inline ShellListCtrlLayout* L(void* p) { return static_cast<ShellListCtrlLayout*>(p); }
inline HWND HWndOf(const void* p) {
    return *reinterpret_cast<const HWND*>(static_cast<const unsigned char*>(p) + kHWndOffset);
}

// Resource ids: IDS_AFX_SHELLLISTCTRL_NAME..MODIFIED = 17128..17131
// (atlmfc/include/afxribbonres.h), the four UINTs retail OnSetColumns loads
// from 0x180350220 (read: 0x42e8 0x42e9 0x42ea 0x42eb).
constexpr UINT kColumnStringIds[4] = { 0x42e8, 0x42e9, 0x42ea, 0x42eb };
// ATL_IDS_DATETIME_INVALID = ATL_RESID_BASE + 0 = 0xD800 (atlmfc/include/atlrc.h).
constexpr UINT kAtlIdsDateTimeInvalid = 0xD800;

// shlwapi!StrFormatKBSizeW.  Retail imports it statically (IAT slot
// 0x1802c6b70 -> SHLWAPI.dll!StrFormatKBSizeW); the phase4 link line
// (phase4/scripts/build_phase4.sh LDLIBS) carries no -lshlwapi, so it is
// resolved at run time here.
typedef LPWSTR (WINAPI* StrFormatKBSizeWFn)(LONGLONG, LPWSTR, UINT);
StrFormatKBSizeWFn LoadStrFormatKBSizeW() {
    HMODULE h = ::GetModuleHandleW(L"shlwapi.dll");
    if (h == nullptr) h = ::LoadLibraryW(L"shlwapi.dll");
    return h ? reinterpret_cast<StrFormatKBSizeWFn>(::GetProcAddress(h, "StrFormatKBSizeW")) : nullptr;
}

} // namespace

// ---- sibling impl__ exports called by the bodies in this file -------------
// (see the link rule in the campaign briefing: C++ methods of other classes
// exist in this DLL only as impl__ thunks.)  Every declaration matches a
// definition that exists today; the file that holds it is named.
extern "C" void* MS_ABI impl___0CMFCListCtrl__QEAA_XZ(void* pThis);                          // controls/CMFCListCtrl.cpp (placeholder)
extern "C" void* MS_ABI impl___1CMFCListCtrl__UEAA_XZ(void* pThis);                          // controls/CMFCListCtrl.cpp (placeholder)
extern "C" int   MS_ABI impl__OnCreate_CMFCListCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpcs); // controls/CMFCListCtrl.cpp (placeholder)
extern "C" void  MS_ABI impl__PreSubclassWindow_CMFCListCtrl__MEAAXXZ(void* pThis);          // controls/CMFCListCtrl.cpp (placeholder)
extern "C" void  MS_ABI impl__Sort_CMFCListCtrl__UEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd); // controls/CMFCListCtrl.cpp (placeholder)
extern "C" int   MS_ABI impl__SelectPath_CMFCShellTreeCtrl__QEAAHPEFBU_ITEMIDLIST__H_Z(void* pThis, const ITEMIDLIST* pidl, int bExpandChildren); // controls/CMFCShellTreeCtrl.cpp (placeholder)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShellTreeCtrl__SAPEAUCRuntimeClass__XZ(); // controls/RuntimeClasses.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);            // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass); // core/runtime/CObject.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);            // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                          // core/window/Thunks.cpp
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam); // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);                          // core/window/Thunks.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                             // detail/RegcoreSupport.cpp
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);        // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);          // core/runtime/CCmdTarget.cpp
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ(); // detail/CWinAppSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                          // detail/MfcExceptionsSupport.cpp
extern "C" uintptr_t MS_ABI impl__GetItemData_CListCtrl__QEBA_KH_Z(const CListCtrl* pThis, int nItem); // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(CListCtrl* pThis, int nCol, const wchar_t* lpszColumnHeading, int nFormat, int nWidth, int nSubItem); // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(const wchar_t* lpszFileName, CFileStatus& rStatus, void* pTM); // core/file/CFile.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID); // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID); // core/collections/CStringT.cpp
extern "C" CString* MS_ABI impl__MakeUpper___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis); // core/collections/CStringT.cpp
extern "C" int   MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const char* pszUTF8, CString* pStrResult, int cbUTF8); // controls/CMFCControlContainer.cpp
extern "C" void  MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer);  // controls/CTagManager.cpp
extern "C" void  MS_ABI impl___1CTagManager__UEAA_XZ(void* self);                             // controls/CTagManager.cpp
extern "C" int   MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(void* self, const wchar_t* tag, CString* value, int trim); // controls/CTagManager.cpp
extern "C" HRESULT MS_ABI impl__ItemFromPath_CShellManager__QEAAJPEB_WAEAPEFAU_ITEMIDLIST___Z(void* pThis, const wchar_t* path, ITEMIDLIST** pidl); // core/app/CShellManager.cpp
extern "C" ITEMIDLIST* MS_ABI impl__ConcatenateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2_0_Z(void* pThis, const ITEMIDLIST* left, const ITEMIDLIST* right); // core/app/CShellManager.cpp
extern "C" ITEMIDLIST* MS_ABI impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(void* pThis, const ITEMIDLIST* pidl); // core/app/CShellManager.cpp
extern "C" int   MS_ABI impl__GetParentItem_CShellManager__QEAAHPEFBU_ITEMIDLIST__AEAPEFAU2__Z(void* pThis, const ITEMIDLIST* pidl, ITEMIDLIST** parent); // core/app/CShellManager.cpp
// Exported data this file reads.
extern "C" void* impl__afxShellManager__3PEAVCShellManager__EA;                  // featurepack/CMFC_misc_stubs.cpp (CShellManager*, NULL today)
extern "C" void* impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA; // core/runtime/StaticData.cpp (IContextMenu2*)
extern "C" unsigned int impl__AFX_WM_CHANGE_CURRENT_FOLDER__3IA;                 // core/runtime/Globals.cpp
extern "C" unsigned int impl__AFX_WM_ON_AFTER_SHELL_COMMAND__3IA;                // core/runtime/Globals.cpp

// Thunks defined further down this file that earlier bodies call.
extern "C" void  MS_ABI impl__ReleaseCurrFolder_CMFCShellListCtrl__IEAAXXZ(void* pThis);
extern "C" long  MS_ABI impl__LockCurrentFolder_CMFCShellListCtrl__IEAAJPEAU_AFX_SHELLITEMINFO___Z(void* pThis, ShellItemInfo* pItemInfo);
extern "C" long  MS_ABI impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(void* pThis, ShellItemInfo* lpItemInfo);
extern "C" long  MS_ABI impl__EnumObjects_CMFCShellListCtrl__MEAAJPEAUIShellFolder__PEFAU_ITEMIDLIST___Z(void* pThis, IShellFolder* pParentFolder, LPITEMIDLIST pidlParent);
extern "C" void* MS_ABI impl__GetRelatedTree_CMFCShellListCtrl__IEBAPEAVCMFCShellTreeCtrl__XZ(const void* pThis);
extern "C" void* MS_ABI impl__GetShellImageList_CMFCShellListCtrl__IEAAPEAU_IMAGELIST__H_Z(void* pThis, int bLarge);
extern "C" int   MS_ABI impl__InitList_CMFCShellListCtrl__IEAAHXZ(void* pThis);
extern "C" void  MS_ABI impl__OnSetColumns_CMFCShellListCtrl__UEAAXXZ(void* pThis);
extern "C" CString* MS_ABI impl__OnGetItemText_CMFCShellListCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHPEAU_AFX_SHELLITEMINFO___Z(void* pThis, CString* pRet, int iItem, int iColumn, ShellItemInfo* pItem);
extern "C" int   MS_ABI impl__OnGetItemIcon_CMFCShellListCtrl__UEAAHHPEAU_AFX_SHELLITEMINFO___Z(void* pThis, int iItem, ShellItemInfo* pItem);
extern "C" void  MS_ABI impl__OnFormatFileSize_CMFCShellListCtrl__UEAAX_JAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pThis, __int64 lFileSize, CString* str);
extern "C" void  MS_ABI impl__OnFormatFileDate_CMFCShellListCtrl__UEAAXAEBVCTime_ATL__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____3__Z(void* pThis, const __int64* tmFile, CString* str);
extern "C" void  MS_ABI impl__DoDefault_CMFCShellListCtrl__MEAAXH_Z(void* pThis, int iItem);
extern "C" long  MS_ABI impl__Refresh_CMFCShellListCtrl__UEAAJXZ(void* pThis);

namespace {

// CWaitCursor: retail's `CWaitCursor wait;` is AfxGetModuleState()->
// m_pCurrentWinApp->BeginWaitCursor() (0x1801de7b0) with EndWaitCursor in
// the destructor (the local helper at 0x7687c); AfxGetApp() is that same
// pointer.  Same helper shape as CMFCPropertyGridCtrl.cpp's WaitCursorScope.
struct WaitCursorScope {
    CWinApp* pApp;
    WaitCursorScope() : pApp(impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
        if (pApp) impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
    ~WaitCursorScope() {
        if (pApp) impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    }
};

// Deviation (2): retail's `ENSURE(afxShellManager->m_pMalloc != NULL)`.
void EnsureShellAllocator() {
    if (impl__afxShellManager__3PEAVCShellManager__EA == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
}

// Deviation (3): the list view's header window.  Retail: GetHeaderCtrl()
// (vslot 92 -> this+0xe8) then m_wndHeader.m_hWnd (+0x128).
HWND HeaderHWnd(const void* pThis) {
    return reinterpret_cast<HWND>(::SendMessage(HWndOf(pThis), LVM_GETHEADER, 0, 0));
}

// The parent CWnd's m_hWnd the way retail reads it: CWnd::FromHandle(
// ::GetParent(m_hWnd)) then ->m_hWnd (0x40) when non-NULL, else NULL.
HWND ParentHWndViaFromHandle(const void* pThis) {
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
    return pParent ? HWndOf(pParent) : nullptr;
}

} // namespace

// Symbol: ??0CMFCShellListCtrl@@QEAA@XZ
// Transcribed from retail entry RVA 0x12a760 (mfc140u):
//     CMFCListCtrl::CMFCListCtrl();                         // 0x77970
//     vfptr = 0x18030e818;  (NOT reproduced -- see below)
//     m_bContextMenu = TRUE;  m_bIsDesktop = FALSE;         // movq $1,0x268
//     m_psfCurFolder = NULL;  m_pidlCurFQ = NULL;  m_hwndRelatedTree = NULL;
//     m_bNoNotify = FALSE;
//     m_nTypes = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;       // 0x60
// The vftable store is omitted: OpenMFC has no MSVC-layout vtable for this
// class, and whatever the base-class constructor thunk installs at +0x00 is
// left alone so that a future real CMFCListCtrl constructor is not clobbered.
// (Today that base thunk is a placeholder that returns pThis and constructs
// nothing, so the CWnd part -- m_hWnd included -- is left exactly as the
// client's memory was; reported as a headerRequest against CMFCListCtrl.cpp.)
extern "C" void* MS_ABI impl___0CMFCShellListCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CMFCListCtrl__QEAA_XZ(pThis);
    ShellListCtrlLayout* s = L(pThis);
    s->m_bContextMenu = TRUE;
    s->m_bIsDesktop = FALSE;
    s->m_psfCurFolder = nullptr;
    s->m_pidlCurFQ = nullptr;
    s->m_hwndRelatedTree = nullptr;
    s->m_bNoNotify = FALSE;
    s->m_nTypes = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
    return pThis;
}

// Symbol: ??1CMFCShellListCtrl@@UEAA@XZ
// Transcribed from retail entry RVA 0x12a810 (mfc140u): stores the class
// vftable (0x18030e818; omitted here, see the constructor) and tail-jumps to
// ??1CMFCListCtrl@@UEAA@XZ (0x77a20).  It does not touch m_psfCurFolder /
// m_pidlCurFQ -- those are released in OnDestroy.
extern "C" void MS_ABI impl___1CMFCShellListCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    impl___1CMFCListCtrl__UEAA_XZ(pThis);
}

// Symbol: ?EnableShellContextMenu@CMFCShellListCtrl@@QEAAXH@Z
// Retail entry RVA 0x12c2c0 (mfc140u): m_bContextMenu = bEnable.
extern "C" void MS_ABI impl__EnableShellContextMenu_CMFCShellListCtrl__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    L(pThis)->m_bContextMenu = bEnable;
}

// Symbol: ?SetItemTypes@CMFCShellListCtrl@@QEAAXK@Z
// Transcribed from retail entry RVA 0x12c360 (mfc140u):
//     if (m_nTypes == nTypes) return;
//     BOOL bWnd = (m_hWnd != NULL);  m_nTypes = nTypes;
//     if (bWnd) Refresh();                                  // vslot 99, devirtualized (1)
extern "C" void MS_ABI impl__SetItemTypes_CMFCShellListCtrl__QEAAXK_Z(void* pThis, unsigned long nTypes) {
    if (pThis == nullptr) return;
    ShellListCtrlLayout* s = L(pThis);
    if (s->m_nTypes == nTypes) return;
    const bool bHasWnd = HWndOf(pThis) != nullptr;
    s->m_nTypes = nTypes;
    if (bHasWnd) {
        impl__Refresh_CMFCShellListCtrl__UEAAJXZ(pThis);
    }
}

// Symbol: ?Refresh@CMFCShellListCtrl@@UEAAJXZ
// Retail entry RVA 0x12ad90 (mfc140u): tail-jumps to vslot 100 with a NULL
// item -- `return DisplayFolder((LPAFX_SHELLITEMINFO)NULL)` -- devirtualized (1).
extern "C" long MS_ABI impl__Refresh_CMFCShellListCtrl__UEAAJXZ(void* pThis) {
    if (pThis == nullptr) return E_FAIL;
    return impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, nullptr);
}

// Symbol: ?GetRelatedTree@CMFCShellListCtrl@@IEBAPEAVCMFCShellTreeCtrl@@XZ
// Transcribed from retail entry RVA 0x12c310 (mfc140u):
//     if (m_hwndRelatedTree == NULL) return NULL;
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndRelatedTree);      // 0x28adc0
//     if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCShellTreeCtrl))) return NULL;  // 0x234cf0, 0x18030ebe8
//     return (CMFCShellTreeCtrl*)p;
extern "C" void* MS_ABI impl__GetRelatedTree_CMFCShellListCtrl__IEBAPEAVCMFCShellTreeCtrl__XZ(const void* pThis) {
    if (pThis == nullptr) return nullptr;
    const ShellListCtrlLayout* s = static_cast<const ShellListCtrlLayout*>(pThis);
    if (s->m_hwndRelatedTree == nullptr) return nullptr;
    CWnd* p = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(s->m_hwndRelatedTree);
    if (p == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            reinterpret_cast<const CObject*>(p),
            impl__GetThisClass_CMFCShellTreeCtrl__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return p;
}

// Symbol: ?GetShellImageList@CMFCShellListCtrl@@IEAAPEAU_IMAGELIST@@H@Z
// Transcribed from retail entry RVA 0x12a8d0 (mfc140u):
//     TCHAR szDir[MAX_PATH];
//     if (!GetWindowsDirectory(szDir, MAX_PATH)) return NULL;      // IAT 0x1802c66e0 GetWindowsDirectoryW
//     SHFILEINFO sfi;
//     return (HIMAGELIST)SHGetFileInfo(szDir, 0, &sfi, sizeof(sfi),
//                SHGFI_SYSICONINDEX | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON));  // 0x4000 / 0x4001
// (`this` is not read.)
extern "C" void* MS_ABI impl__GetShellImageList_CMFCShellListCtrl__IEAAPEAU_IMAGELIST__H_Z(void* /*pThis*/, int bLarge) {
    wchar_t szDir[MAX_PATH];
    if (!::GetWindowsDirectoryW(szDir, MAX_PATH)) return nullptr;
    SHFILEINFOW sfi;
    return reinterpret_cast<void*>(::SHGetFileInfoW(szDir, 0, &sfi, sizeof(sfi),
        SHGFI_SYSICONINDEX | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON)));
}

// Symbol: ?ReleaseCurrFolder@CMFCShellListCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x12a830 (mfc140u):
//     if (m_psfCurFolder == NULL) return;
//     m_psfCurFolder->Release();  m_psfCurFolder = NULL;
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     if (m_pidlCurFQ != NULL) m_pMalloc->Free(m_pidlCurFQ);        // IMalloc vslot 5 -> CoTaskMemFree
//     m_pidlCurFQ = NULL;
extern "C" void MS_ABI impl__ReleaseCurrFolder_CMFCShellListCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    ShellListCtrlLayout* s = L(pThis);
    if (s->m_psfCurFolder == nullptr) return;
    s->m_psfCurFolder->Release();
    s->m_psfCurFolder = nullptr;
    EnsureShellAllocator();
    if (s->m_pidlCurFQ != nullptr) {
        ::CoTaskMemFree(s->m_pidlCurFQ);
    }
    s->m_pidlCurFQ = nullptr;
}

// Symbol: ?LockCurrentFolder@CMFCShellListCtrl@@IEAAJPEAU_AFX_SHELLITEMINFO@@@Z
// Transcribed from retail entry RVA 0x12a950 (mfc140u):
//     m_pidlCurFQ = NULL;
//     HRESULT hr;
//     if (pItemInfo != NULL && pItemInfo->pParentFolder != NULL) {
//         ENSURE(pItemInfo->pidlRel != NULL);
//         hr = pItemInfo->pParentFolder->BindToObject(pItemInfo->pidlRel, NULL,
//                  IID_IShellFolder /*0x1802d9858*/, (void**)&m_psfCurFolder);   // IShellFolder vslot 5
//         m_bIsDesktop = FALSE;
//     } else {
//         hr = SHGetDesktopFolder(&m_psfCurFolder);                 // delay-load 0x1803e9198
//         m_bIsDesktop = TRUE;
//     }
//     if (SUCCEEDED(hr) && pItemInfo != NULL)
//         m_pidlCurFQ = afxShellManager->CopyItem(pItemInfo->pidlFQ);  // 0x12cdc0
//     return hr;
extern "C" long MS_ABI impl__LockCurrentFolder_CMFCShellListCtrl__IEAAJPEAU_AFX_SHELLITEMINFO___Z(void* pThis, ShellItemInfo* pItemInfo) {
    if (pThis == nullptr) return E_FAIL;
    ShellListCtrlLayout* s = L(pThis);
    s->m_pidlCurFQ = nullptr;
    HRESULT hr;
    if (pItemInfo != nullptr && pItemInfo->pParentFolder != nullptr) {
        if (pItemInfo->pidlRel == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
        }
        hr = pItemInfo->pParentFolder->BindToObject(pItemInfo->pidlRel, nullptr, IID_IShellFolder,
                                                    reinterpret_cast<void**>(&s->m_psfCurFolder));
        s->m_bIsDesktop = FALSE;
    } else {
        hr = ::SHGetDesktopFolder(&s->m_psfCurFolder);
        s->m_bIsDesktop = TRUE;
    }
    if (SUCCEEDED(hr) && pItemInfo != nullptr) {
        s->m_pidlCurFQ = impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(
            impl__afxShellManager__3PEAVCShellManager__EA, pItemInfo->pidlFQ);
    }
    return hr;
}

// Symbol: ?DisplayFolder@CMFCShellListCtrl@@UEAAJPEAU_AFX_SHELLITEMINFO@@@Z
// Transcribed from retail entry RVA 0x12aa10 (mfc140u):
//     if (afxShellManager == NULL) return E_FAIL;
//     HRESULT hr = E_FAIL;
//     if (lpItemInfo != NULL) {
//         ReleaseCurrFolder();                                      // 0x12a830
//         hr = LockCurrentFolder(lpItemInfo);                       // 0x12a950
//         if (FAILED(hr)) return hr;
//     }
//     DeleteAllItems();                                             // LVM_DELETEALLITEMS
//     if (m_psfCurFolder != NULL) {
//         CWaitCursor wait;
//         SetRedraw(FALSE);                                         // WM_SETREDRAW
//         hr = EnumObjects(m_psfCurFolder, m_pidlCurFQ);            // vslot 108, devirtualized (1)
//         if (GetStyle() & LVS_REPORT) Sort(0, TRUE, FALSE);        // 0x2a9690; vslot 93, devirtualized (1)
//         SetRedraw(TRUE);
//         RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);  // 0x105
//     }
//     if (SUCCEEDED(hr) && lpItemInfo != NULL) {
//         CMFCShellTreeCtrl* pTree = GetRelatedTree();              // 0x12c310
//         if (pTree != NULL && !m_bNoNotify) pTree->SelectPath(m_pidlCurFQ, TRUE);  // 0x12df80
//         if (CWnd::FromHandle(GetParent(m_hWnd)) != NULL)
//             ::SendMessage(CWnd::FromHandle(GetParent(m_hWnd))->m_hWnd, AFX_WM_CHANGE_CURRENT_FOLDER, 0, 0);
//     }
//     return hr;
// Note the style test is the single bit LVS_REPORT (0x1), as retail has it; it
// is also set for LVS_LIST (0x3).
extern "C" long MS_ABI impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(void* pThis, ShellItemInfo* lpItemInfo) {
    if (pThis == nullptr) return E_FAIL;
    if (impl__afxShellManager__3PEAVCShellManager__EA == nullptr) return E_FAIL;
    ShellListCtrlLayout* s = L(pThis);
    HRESULT hr = E_FAIL;
    if (lpItemInfo != nullptr) {
        impl__ReleaseCurrFolder_CMFCShellListCtrl__IEAAXXZ(pThis);
        hr = impl__LockCurrentFolder_CMFCShellListCtrl__IEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, lpItemInfo);
        if (FAILED(hr)) return hr;
    }
    const HWND hWnd = HWndOf(pThis);
    ::SendMessage(hWnd, LVM_DELETEALLITEMS, 0, 0);
    if (s->m_psfCurFolder != nullptr) {
        WaitCursorScope wait;
        ::SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
        hr = impl__EnumObjects_CMFCShellListCtrl__MEAAJPEAUIShellFolder__PEFAU_ITEMIDLIST___Z(pThis, s->m_psfCurFolder, s->m_pidlCurFQ);
        if (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pThis)) & LVS_REPORT) {
            impl__Sort_CMFCListCtrl__UEAAXHHH_Z(pThis, 0, TRUE, FALSE);
        }
        ::SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
        ::RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    if (SUCCEEDED(hr) && lpItemInfo != nullptr) {
        void* pTree = impl__GetRelatedTree_CMFCShellListCtrl__IEBAPEAVCMFCShellTreeCtrl__XZ(pThis);
        if (pTree != nullptr && !s->m_bNoNotify) {
            impl__SelectPath_CMFCShellTreeCtrl__QEAAHPEFBU_ITEMIDLIST__H_Z(pTree, s->m_pidlCurFQ, TRUE);
        }
        if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd)) != nullptr) {
            ::SendMessage(HWndOf(impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd))),
                          impl__AFX_WM_CHANGE_CURRENT_FOLDER__3IA, 0, 0);
        }
    }
    return hr;
}

// Symbol: ?DisplayFolder@CMFCShellListCtrl@@UEAAJPEB_W@Z
// Transcribed from retail entry RVA 0x12acb0 (mfc140u):
//     if (afxShellManager == NULL) return E_FAIL;
//     ENSURE(lpszPath != NULL);
//     AFX_SHELLITEMINFO info = {};
//     HRESULT hr = afxShellManager->ItemFromPath(lpszPath, info.pidlRel);   // 0x12ce70
//     if (FAILED(hr)) return hr;
//     IShellFolder* psfDesktop;
//     hr = SHGetDesktopFolder(&psfDesktop);                         // delay-load 0x1803e9198
//     if (SUCCEEDED(hr)) {
//         info.pParentFolder = psfDesktop;  info.pidlFQ = info.pidlRel;
//         hr = DisplayFolder(&info);                                // vslot 100, devirtualized (1)
//         psfDesktop->Release();
//     }
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     if (info.pidlFQ != NULL) m_pMalloc->Free(info.pidlFQ);        // -> CoTaskMemFree
//     return hr;
// (As in retail, only pidlFQ is freed: when SHGetDesktopFolder fails pidlFQ is
// still NULL and the PIDL held in pidlRel is not released.)
extern "C" long MS_ABI impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEB_W_Z(void* pThis, const wchar_t* lpszPath) {
    if (pThis == nullptr) return E_FAIL;
    if (impl__afxShellManager__3PEAVCShellManager__EA == nullptr) return E_FAIL;
    if (lpszPath == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return E_FAIL;
    }
    ShellItemInfo info = { nullptr, nullptr, nullptr };
    HRESULT hr = impl__ItemFromPath_CShellManager__QEAAJPEB_WAEAPEFAU_ITEMIDLIST___Z(
        impl__afxShellManager__3PEAVCShellManager__EA, lpszPath, &info.pidlRel);
    if (FAILED(hr)) return hr;
    IShellFolder* psfDesktop = nullptr;
    hr = ::SHGetDesktopFolder(&psfDesktop);
    if (SUCCEEDED(hr)) {
        info.pParentFolder = psfDesktop;
        info.pidlFQ = info.pidlRel;
        hr = impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, &info);
        psfDesktop->Release();
    }
    EnsureShellAllocator();
    if (info.pidlFQ != nullptr) {
        ::CoTaskMemFree(info.pidlFQ);
    }
    return hr;
}

// Symbol: ?DisplayParentFolder@CMFCShellListCtrl@@UEAAJXZ
// Transcribed from retail entry RVA 0x12abb0 (mfc140u):
//     if (m_pidlCurFQ == NULL) return E_FAIL;
//     AFX_SHELLITEMINFO info = {};
//     int r = afxShellManager->GetParentItem(m_pidlCurFQ, info.pidlFQ);   // 0x12cf30
//     if (r < 0) return E_FAIL;
//     HRESULT hr;
//     if (r == 0) {                       // parent is the desktop (see below)
//         hr = DisplayFolder(&info);                                // vslot 100, devirtualized (1)
//     } else {
//         IShellFolder* psfDesktop;
//         hr = SHGetDesktopFolder(&psfDesktop);
//         if (SUCCEEDED(hr)) {
//             info.pParentFolder = psfDesktop;  info.pidlRel = info.pidlFQ;
//             hr = DisplayFolder(&info);
//             psfDesktop->Release();
//         }
//     }
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     if (info.pidlFQ != NULL) m_pMalloc->Free(info.pidlFQ);        // -> CoTaskMemFree
//     return hr;
// Retail ?GetParentItem@CShellManager (RVA 0x12cf30, mfc140u) returns -1 for a
// NULL/empty pidl, 0 for a single-element pidl -- in which case it has stored
// the desktop PIDL obtained from SHGetSpecialFolderLocation(CSIDL_DESKTOP)
// in info.pidlFQ, so on the r == 0 path pidlFQ is NOT NULL: DisplayFolder
// sees pParentFolder == NULL (-> SHGetDesktopFolder, m_bIsDesktop = TRUE)
// and copies that desktop PIDL into m_pidlCurFQ -- and (count - 1) with a
// truncated copy otherwise.  OpenMFC's thunk (core/app/CShellManager.cpp)
// returns TRUE/FALSE instead and hands a single-element pidl back as an empty
// (terminator-only) parent, so the r == 0 branch is not reached here for a
// non-empty pidl; the body transcribes retail's dispatch on r regardless.
extern "C" long MS_ABI impl__DisplayParentFolder_CMFCShellListCtrl__UEAAJXZ(void* pThis) {
    if (pThis == nullptr) return E_FAIL;
    ShellListCtrlLayout* s = L(pThis);
    if (s->m_pidlCurFQ == nullptr) return E_FAIL;
    ShellItemInfo info = { nullptr, nullptr, nullptr };
    const int r = impl__GetParentItem_CShellManager__QEAAHPEFBU_ITEMIDLIST__AEAPEFAU2__Z(
        impl__afxShellManager__3PEAVCShellManager__EA, s->m_pidlCurFQ, &info.pidlFQ);
    if (r < 0) return E_FAIL;
    HRESULT hr;
    if (r == 0) {
        hr = impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, &info);
    } else {
        IShellFolder* psfDesktop = nullptr;
        hr = ::SHGetDesktopFolder(&psfDesktop);
        if (SUCCEEDED(hr)) {
            info.pParentFolder = psfDesktop;
            info.pidlRel = info.pidlFQ;
            hr = impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, &info);
            psfDesktop->Release();
        }
    }
    EnsureShellAllocator();
    if (info.pidlFQ != nullptr) {
        ::CoTaskMemFree(info.pidlFQ);
    }
    return hr;
}

// Symbol: ?EnumObjects@CMFCShellListCtrl@@MEAAJPEAUIShellFolder@@PEFAU_ITEMIDLIST@@@Z
// Transcribed from retail entry RVA 0x12adb0 (mfc140u):
//     LPENUMIDLIST pEnum = NULL;
//     HRESULT hr = pParentFolder->EnumObjects(NULL, m_nTypes, &pEnum);   // IShellFolder vslot 4
//     if (FAILED(hr) || pEnum == NULL) return hr;
//     LPITEMIDLIST pidlRel;  ULONG fetched = 1;
//     while (pEnum->Next(1, &pidlRel, &fetched) == S_OK) {          // IEnumIDList vslot 3
//         if (fetched == 0) break;
//         LVITEM lvi = {};  lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
//         pParentFolder->AddRef();
//         AFX_SHELLITEMINFO* pInfo = (AFX_SHELLITEMINFO*)GlobalAlloc(GPTR, sizeof(*pInfo));  // IAT GlobalAlloc, 0x40/0x18
//         if (pInfo == NULL) return E_FAIL;                         // (pEnum and pidlRel are not released -- retail)
//         pInfo->pidlRel = pidlRel;
//         pInfo->pidlFQ = afxShellManager->ConcatenateItem(pidlParent, pidlRel);  // 0x12cd00
//         pInfo->pParentFolder = pParentFolder;
//         lvi.pszText = _T("");   lvi.lParam = (LPARAM)pInfo;      // 0x18033d19c is L""
//         lvi.iImage = OnGetItemIcon(GetItemCount(), pInfo);        // LVM_GETITEMCOUNT; vslot 105, devirtualized (1)
//         // NOTE: lvi.iItem is never written -- it stays 0 from the memset, so
//         // every LVM_INSERTITEMW below inserts at index 0 (no store to lvi+4
//         // anywhere in 0x12adb0..0x12b088; the item count only feeds
//         // OnGetItemIcon).  In LVS_REPORT mode DisplayFolder sorts afterwards.
//         ULONG dwAttr = SFGAO_DISPLAYATTRMASK;                     // 0xfc000
//         pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlRel, &dwAttr);  // IShellFolder vslot 9
//         if (dwAttr & SFGAO_SHARE)   { lvi.mask |= LVIF_STATE; lvi.state |= INDEXTOOVERLAYMASK(1); lvi.stateMask |= LVIS_OVERLAYMASK; }
//         if (dwAttr & SFGAO_GHOSTED) { lvi.mask |= LVIF_STATE; lvi.state |= LVIS_CUT; lvi.stateMask |= LVIS_CUT; }
//         int iItem = InsertItem(&lvi);                             // LVM_INSERTITEMW
//         if (iItem >= 0) {
//             int nCols = GetHeaderCtrl().GetItemCount();          // +0x128, HDM_GETITEMCOUNT -- deviation (3)
//             for (int i = 0; i < nCols; i++) {
//                 CString str = OnGetItemText(iItem, i, pInfo);     // vslot 104, devirtualized (1)
//                 SetItemText(iItem, i, str);                       // LVM_SETITEMTEXTW
//             }
//         }
//         fetched = 0;
//     }
//     pEnum->Release();
//     return hr;                                                    // the EnumObjects HRESULT
extern "C" long MS_ABI impl__EnumObjects_CMFCShellListCtrl__MEAAJPEAUIShellFolder__PEFAU_ITEMIDLIST___Z(void* pThis, IShellFolder* pParentFolder, LPITEMIDLIST pidlParent) {
    if (pThis == nullptr || pParentFolder == nullptr) return E_FAIL;
    ShellListCtrlLayout* s = L(pThis);
    const HWND hWnd = HWndOf(pThis);
    IEnumIDList* pEnum = nullptr;
    const HRESULT hr = pParentFolder->EnumObjects(nullptr, s->m_nTypes, &pEnum);
    if (FAILED(hr) || pEnum == nullptr) return hr;

    LPITEMIDLIST pidlRel = nullptr;
    ULONG fetched = 1;
    while (pEnum->Next(1, &pidlRel, &fetched) == S_OK) {
        if (fetched == 0) break;
        LVITEMW lvi;
        std::memset(&lvi, 0, sizeof(lvi));
        lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
        pParentFolder->AddRef();
        ShellItemInfo* pInfo = static_cast<ShellItemInfo*>(::GlobalAlloc(GPTR, sizeof(ShellItemInfo)));
        if (pInfo == nullptr) return E_FAIL;
        pInfo->pidlRel = pidlRel;
        pInfo->pidlFQ = impl__ConcatenateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2_0_Z(
            impl__afxShellManager__3PEAVCShellManager__EA, pidlParent, pidlRel);
        pInfo->pParentFolder = pParentFolder;
        lvi.pszText = const_cast<wchar_t*>(L"");
        lvi.lParam = reinterpret_cast<LPARAM>(pInfo);
        // lvi.iItem deliberately left at 0 (see the transcription above).
        const int nCount = static_cast<int>(::SendMessage(hWnd, LVM_GETITEMCOUNT, 0, 0));
        lvi.iImage = impl__OnGetItemIcon_CMFCShellListCtrl__UEAAHHPEAU_AFX_SHELLITEMINFO___Z(pThis, nCount, pInfo);
        ULONG dwAttr = SFGAO_DISPLAYATTRMASK;
        LPCITEMIDLIST pidlAttr = pidlRel;
        pParentFolder->GetAttributesOf(1, &pidlAttr, &dwAttr);
        if (dwAttr & SFGAO_SHARE) {
            lvi.mask |= LVIF_STATE;
            lvi.state |= INDEXTOOVERLAYMASK(1);
            lvi.stateMask |= LVIS_OVERLAYMASK;
        }
        if (dwAttr & SFGAO_GHOSTED) {
            lvi.mask |= LVIF_STATE;
            lvi.state |= LVIS_CUT;
            lvi.stateMask |= LVIS_CUT;
        }
        const int iItem = static_cast<int>(::SendMessage(hWnd, LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&lvi)));
        if (iItem >= 0) {
            const int nCols = static_cast<int>(::SendMessage(HeaderHWnd(pThis), HDM_GETITEMCOUNT, 0, 0));
            for (int i = 0; i < nCols; i++) {
                CString str;
                impl__OnGetItemText_CMFCShellListCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHPEAU_AFX_SHELLITEMINFO___Z(
                    pThis, &str, iItem, i, pInfo);
                LVITEMW lviText;
                std::memset(&lviText, 0, sizeof(lviText));
                lviText.iSubItem = i;
                lviText.pszText = const_cast<wchar_t*>(str.GetString());
                ::SendMessage(hWnd, LVM_SETITEMTEXTW, static_cast<WPARAM>(iItem), reinterpret_cast<LPARAM>(&lviText));
            }
        }
        fetched = 0;
    }
    pEnum->Release();
    return hr;
}

// Symbol: ?InitList@CMFCShellListCtrl@@IEAAHXZ
// Transcribed from retail entry RVA 0x12bbb0 (mfc140u):
//     if (afxShellManager == NULL) return FALSE;
//     ModifyStyle(0, LVS_SHAREIMAGELISTS);                          // 0x2a96f0
//     SetImageList(CImageList::FromHandle(GetShellImageList(TRUE)),  LVSIL_NORMAL);  // LVM_SETIMAGELIST -- deviation (4)
//     SetImageList(CImageList::FromHandle(GetShellImageList(FALSE)), LVSIL_SMALL);
//     OnSetColumns();                                               // vslot 103, devirtualized (1)
//     if (m_psfCurFolder == NULL) {
//         AFX_SHELLITEMINFO info = {};
//         if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &info.pidlFQ))) {  // delay-load 0x1803e9188
//             DisplayFolder(&info);                                 // vslot 100, devirtualized (1)
//             ENSURE(afxShellManager->m_pMalloc != NULL);           // deviation (2)
//             if (info.pidlFQ != NULL) m_pMalloc->Free(info.pidlFQ);
//         }
//     }
//     return TRUE;
extern "C" int MS_ABI impl__InitList_CMFCShellListCtrl__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    if (impl__afxShellManager__3PEAVCShellManager__EA == nullptr) return FALSE;
    ShellListCtrlLayout* s = L(pThis);
    const HWND hWnd = HWndOf(pThis);
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(static_cast<CWnd*>(pThis), 0, LVS_SHAREIMAGELISTS, 0);
    ::SendMessage(hWnd, LVM_SETIMAGELIST, LVSIL_NORMAL,
                  reinterpret_cast<LPARAM>(impl__GetShellImageList_CMFCShellListCtrl__IEAAPEAU_IMAGELIST__H_Z(pThis, TRUE)));
    ::SendMessage(hWnd, LVM_SETIMAGELIST, LVSIL_SMALL,
                  reinterpret_cast<LPARAM>(impl__GetShellImageList_CMFCShellListCtrl__IEAAPEAU_IMAGELIST__H_Z(pThis, FALSE)));
    impl__OnSetColumns_CMFCShellListCtrl__UEAAXXZ(pThis);
    if (s->m_psfCurFolder == nullptr) {
        ShellItemInfo info = { nullptr, nullptr, nullptr };
        if (SUCCEEDED(::SHGetSpecialFolderLocation(nullptr, CSIDL_DESKTOP, &info.pidlFQ))) {
            impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, &info);
            EnsureShellAllocator();
            if (info.pidlFQ != nullptr) {
                ::CoTaskMemFree(info.pidlFQ);
            }
        }
    }
    return TRUE;
}

// Symbol: ?OnCreate@CMFCShellListCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from retail entry RVA 0x12a8a0 (mfc140u):
//     if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1) return -1;  // 0x77af0
//     return InitList() ? 0 : -1;                                   // 0x12bbb0
extern "C" int MS_ABI impl__OnCreate_CMFCShellListCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (pThis == nullptr) return -1;
    if (impl__OnCreate_CMFCListCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, lpCreateStruct) == -1) return -1;
    return impl__InitList_CMFCShellListCtrl__IEAAHXZ(pThis) ? 0 : -1;
}

// Symbol: ?PreSubclassWindow@CMFCShellListCtrl@@MEAAXXZ
// Transcribed from retail entry RVA 0x12bb80 (mfc140u):
//     CMFCListCtrl::PreSubclassWindow();                            // 0x77ac0
//     _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();        // 0x1332a0
//     if (pThreadState->m_pWndInit == NULL) InitList();             // +0x28; 0x12bbb0
// OpenMFC keeps the same member as _AFX_THREAD_STATE::pWndInit (read through
// AfxThreadStateMirror above).  Retail dereferences the thread state
// unconditionally; a NULL guard is added.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCShellListCtrl__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__PreSubclassWindow_CMFCListCtrl__MEAAXXZ(pThis);
    const AfxThreadStateMirror* pThreadState =
        reinterpret_cast<const AfxThreadStateMirror*>(impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ());
    if (pThreadState == nullptr || pThreadState->pWndInit == nullptr) {
        impl__InitList_CMFCShellListCtrl__IEAAHXZ(pThis);
    }
}

// Symbol: ?OnDestroy@CMFCShellListCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x12c2d0 (mfc140u):
//     CMFCShellTreeCtrl* pTree = GetRelatedTree();                  // 0x12c310
//     if (pTree != NULL) pTree->m_hwndRelatedList = NULL;           // tree +0xf0
//     ReleaseCurrFolder();                                          // 0x12a830
//     CWnd::OnDestroy();                                            // 0x28b840
extern "C" void MS_ABI impl__OnDestroy_CMFCShellListCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pTree = impl__GetRelatedTree_CMFCShellListCtrl__IEBAPEAVCMFCShellTreeCtrl__XZ(pThis);
    if (pTree != nullptr) {
        *reinterpret_cast<HWND*>(static_cast<unsigned char*>(pTree) + kTreeRelatedListHwndOffset) = nullptr;
    }
    impl__ReleaseCurrFolder_CMFCShellListCtrl__IEAAXXZ(pThis);
    impl__OnDestroy_CWnd__IEAAXXZ(static_cast<CWnd*>(pThis));
}

// Symbol: ?OnDeleteitem@CMFCShellListCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0x12b2f0 (mfc140u):
//     ENSURE(pNMHDR != NULL);
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     AFX_SHELLITEMINFO* pInfo = (AFX_SHELLITEMINFO*)((NMLISTVIEW*)pNMHDR)->lParam;   // +0x38 (not NULL-tested)
//     if (pInfo->pidlFQ != NULL)  m_pMalloc->Free(pInfo->pidlFQ);   // -> CoTaskMemFree
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // re-read, re-tested
//     if (pInfo->pidlRel != NULL) m_pMalloc->Free(pInfo->pidlRel);
//     if (pInfo->pParentFolder != NULL) { pInfo->pParentFolder->Release(); pInfo->pParentFolder = NULL; }
//     GlobalFree(pInfo);                                            // IAT GlobalFree
//     *pResult = 0;
extern "C" void MS_ABI impl__OnDeleteitem_CMFCShellListCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    if (pThis == nullptr) return;
    if (pNMHDR == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    EnsureShellAllocator();
    ShellItemInfo* pInfo = reinterpret_cast<ShellItemInfo*>(reinterpret_cast<NMLISTVIEW*>(pNMHDR)->lParam);
    if (pInfo->pidlFQ != nullptr) {
        ::CoTaskMemFree(pInfo->pidlFQ);
    }
    EnsureShellAllocator();
    if (pInfo->pidlRel != nullptr) {
        ::CoTaskMemFree(pInfo->pidlRel);
    }
    if (pInfo->pParentFolder != nullptr) {
        pInfo->pParentFolder->Release();
        pInfo->pParentFolder = nullptr;
    }
    ::GlobalFree(pInfo);
    if (pResult != nullptr) *pResult = 0;
}

namespace {
// OnDblClk and OnReturn are one folded body in retail (both exports resolve to
// entry RVA 0x12b3a0 in mfc140u):
//     int iItem = GetNextItem(-1, LVNI_FOCUSED);                    // LVM_GETNEXTITEM, wParam -1, lParam 1
//     if (iItem != -1) DoDefault(iItem);                            // vslot 109, devirtualized (1)
//     *pResult = 0;
void ShellListDefaultAction(void* pThis, __int64* pResult) {
    const int iItem = static_cast<int>(::SendMessage(HWndOf(pThis), LVM_GETNEXTITEM, static_cast<WPARAM>(-1), LVNI_FOCUSED));
    if (iItem != -1) {
        impl__DoDefault_CMFCShellListCtrl__MEAAXH_Z(pThis, iItem);
    }
    if (pResult != nullptr) *pResult = 0;
}
} // namespace

// Symbol: ?OnDblClk@CMFCShellListCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Retail entry RVA 0x12b3a0 (mfc140u) -- see ShellListDefaultAction.
extern "C" void MS_ABI impl__OnDblClk_CMFCShellListCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* /*pNMHDR*/, __int64* pResult) {
    if (pThis == nullptr) return;
    ShellListDefaultAction(pThis, pResult);
}

// Symbol: ?OnReturn@CMFCShellListCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Retail entry RVA 0x12b3a0 (mfc140u), the same folded body as OnDblClk --
// see ShellListDefaultAction.
extern "C" void MS_ABI impl__OnReturn_CMFCShellListCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* /*pNMHDR*/, __int64* pResult) {
    if (pThis == nullptr) return;
    ShellListDefaultAction(pThis, pResult);
}

// Symbol: ?DoDefault@CMFCShellListCtrl@@MEAAXH@Z
// Transcribed from retail entry RVA 0x12b090 (mfc140u):
//     LVITEM lvi = {};  lvi.mask = LVIF_PARAM;  lvi.iItem = iItem;
//     if (!GetItem(&lvi)) return;                                   // LVM_GETITEMW
//     AFX_SHELLITEMINFO* pInfo = (AFX_SHELLITEMINFO*)lvi.lParam;
//     if (pInfo == NULL || pInfo->pParentFolder == NULL || pInfo->pidlRel == NULL) return;
//     IShellFolder* psf = pInfo->pParentFolder;  psf->AddRef();
//     ULONG dwAttr = SFGAO_FOLDER;
//     psf->GetAttributesOf(1, (LPCITEMIDLIST*)&pInfo->pidlRel, &dwAttr);   // vslot 9
//     if (dwAttr & SFGAO_FOLDER) {
//         DisplayFolder(pInfo);                                     // vslot 100, devirtualized (1)
//     } else {
//         IContextMenu* pcm;
//         if (SUCCEEDED(psf->GetUIObjectOf(m_hWnd, 1, (LPCITEMIDLIST*)&pInfo->pidlRel,
//                                          IID_IContextMenu /*0x1802d9848*/, NULL, (void**)&pcm))) {   // vslot 10
//             HMENU hMenu = CreatePopupMenu();
//             if (hMenu != NULL &&
//                 SUCCEEDED(pcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE))) {  // vslot 3
//                 UINT idCmd = GetMenuDefaultItem(hMenu, FALSE, 0);
//                 if (idCmd != 0 && idCmd != (UINT)-1) {
//                     CMINVOKECOMMANDINFO ici = {};  ici.cbSize = sizeof(ici);
//                     ici.hwnd = CWnd::FromHandle(GetParent(m_hWnd)) ? ->m_hWnd : NULL;
//                     ici.lpVerb = MAKEINTRESOURCEA(idCmd - 1);  ici.nShow = SW_SHOWNORMAL;
//                     if (SUCCEEDED(pcm->InvokeCommand(&ici)) &&                 // vslot 4
//                         CWnd::FromHandle(GetParent(m_hWnd)) != NULL)
//                         ::SendMessage(CWnd::FromHandle(GetParent(m_hWnd))->m_hWnd,
//                                       AFX_WM_ON_AFTER_SHELL_COMMAND, idCmd, 0);
//                 }
//             }
//             pcm->Release();
//         }
//     }
//     psf->Release();
// (hMenu is not destroyed -- retail has no DestroyMenu call in this body.)
extern "C" void MS_ABI impl__DoDefault_CMFCShellListCtrl__MEAAXH_Z(void* pThis, int iItem) {
    if (pThis == nullptr) return;
    const HWND hWnd = HWndOf(pThis);
    LVITEMW lvi;
    std::memset(&lvi, 0, sizeof(lvi));
    lvi.mask = LVIF_PARAM;
    lvi.iItem = iItem;
    if (!::SendMessage(hWnd, LVM_GETITEMW, 0, reinterpret_cast<LPARAM>(&lvi))) return;
    ShellItemInfo* pInfo = reinterpret_cast<ShellItemInfo*>(lvi.lParam);
    if (pInfo == nullptr) return;
    IShellFolder* psf = pInfo->pParentFolder;
    if (psf == nullptr) return;
    if (pInfo->pidlRel == nullptr) return;
    psf->AddRef();
    ULONG dwAttr = SFGAO_FOLDER;
    LPCITEMIDLIST pidlAttr = pInfo->pidlRel;
    psf->GetAttributesOf(1, &pidlAttr, &dwAttr);
    if (dwAttr & SFGAO_FOLDER) {
        impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, pInfo);
    } else {
        IContextMenu* pcm = nullptr;
        LPCITEMIDLIST pidlUI = pInfo->pidlRel;
        if (SUCCEEDED(psf->GetUIObjectOf(hWnd, 1, &pidlUI, IID_IContextMenu, nullptr, reinterpret_cast<void**>(&pcm)))) {
            HMENU hMenu = ::CreatePopupMenu();
            if (hMenu != nullptr &&
                SUCCEEDED(pcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE))) {
                const UINT idCmd = ::GetMenuDefaultItem(hMenu, FALSE, 0);
                if (idCmd != 0 && idCmd != static_cast<UINT>(-1)) {
                    CMINVOKECOMMANDINFO ici;
                    std::memset(&ici, 0, sizeof(ici));
                    ici.cbSize = sizeof(ici);
                    ici.fMask = 0;
                    ici.hwnd = ParentHWndViaFromHandle(pThis);
                    ici.lpVerb = MAKEINTRESOURCEA(idCmd - 1);
                    ici.lpParameters = nullptr;
                    ici.lpDirectory = nullptr;
                    ici.nShow = SW_SHOWNORMAL;
                    ici.dwHotKey = 0;
                    ici.hIcon = nullptr;
                    if (SUCCEEDED(pcm->InvokeCommand(&ici)) &&
                        impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd)) != nullptr) {
                        ::SendMessage(HWndOf(impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd))),
                                      impl__AFX_WM_ON_AFTER_SHELL_COMMAND__3IA, static_cast<WPARAM>(idCmd), 0);
                    }
                }
            }
            pcm->Release();
        }
    }
    psf->Release();
}

// Symbol: ?GetCurrentFolder@CMFCShellListCtrl@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Transcribed from retail entry RVA 0x12b4a0 (mfc140u):
//     strPath.Empty();
//     if (m_pidlCurFQ == NULL) return FALSE;
//     TCHAR szPath[MAX_PATH];
//     if (!SHGetPathFromIDList(m_pidlCurFQ, szPath)) return FALSE;  // delay-load 0x1803e9190
//     strPath = szPath;                                             // SetString(szPath, wcslen)
//     return TRUE;
extern "C" int MS_ABI impl__GetCurrentFolder_CMFCShellListCtrl__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const void* pThis, CString* strPath) {
    if (pThis == nullptr || strPath == nullptr) return FALSE;
    strPath->Empty();
    const ShellListCtrlLayout* s = static_cast<const ShellListCtrlLayout*>(pThis);
    if (s->m_pidlCurFQ == nullptr) return FALSE;
    wchar_t szPath[MAX_PATH];
    if (!::SHGetPathFromIDListW(s->m_pidlCurFQ, szPath)) return FALSE;
    *strPath = szPath;
    return TRUE;
}

// Symbol: ?GetCurrentFolderName@CMFCShellListCtrl@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Transcribed from retail entry RVA 0x12b530 (mfc140u):
//     strName.Empty();
//     if (m_pidlCurFQ == NULL) return FALSE;
//     SHFILEINFO sfi;
//     if (!SHGetFileInfo((LPCTSTR)m_pidlCurFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME)) return FALSE;  // 0x208
//     strName = sfi.szDisplayName;
//     return TRUE;
extern "C" int MS_ABI impl__GetCurrentFolderName_CMFCShellListCtrl__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const void* pThis, CString* strName) {
    if (pThis == nullptr || strName == nullptr) return FALSE;
    strName->Empty();
    const ShellListCtrlLayout* s = static_cast<const ShellListCtrlLayout*>(pThis);
    if (s->m_pidlCurFQ == nullptr) return FALSE;
    SHFILEINFOW sfi;
    if (!::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(s->m_pidlCurFQ), 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME)) return FALSE;
    *strName = sfi.szDisplayName;
    return TRUE;
}

// Symbol: ?GetItemPath@CMFCShellListCtrl@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
// Transcribed from retail entry RVA 0x12b400 (mfc140u):
//     strPath.Empty();
//     AFX_SHELLITEMINFO* pInfo = (AFX_SHELLITEMINFO*)GetItemData(iItem);   // 0x295790
//     if (pInfo == NULL || pInfo->pidlFQ == NULL) return FALSE;
//     TCHAR szPath[MAX_PATH];
//     if (!SHGetPathFromIDList(pInfo->pidlFQ, szPath)) return FALSE;
//     strPath = szPath;
//     return TRUE;
extern "C" int MS_ABI impl__GetItemPath_CMFCShellListCtrl__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const void* pThis, CString* strPath, int iItem) {
    if (pThis == nullptr || strPath == nullptr) return FALSE;
    strPath->Empty();
    const ShellItemInfo* pInfo = reinterpret_cast<const ShellItemInfo*>(
        impl__GetItemData_CListCtrl__QEBA_KH_Z(static_cast<const CListCtrl*>(pThis), iItem));
    if (pInfo == nullptr || pInfo->pidlFQ == nullptr) return FALSE;
    wchar_t szPath[MAX_PATH];
    if (!::SHGetPathFromIDListW(pInfo->pidlFQ, szPath)) return FALSE;
    *strPath = szPath;
    return TRUE;
}

// Symbol: ?OnSetColumns@CMFCShellListCtrl@@UEAAXXZ
// Transcribed from retail entry RVA 0x12ba50 (mfc140u):
//     int nCols = GetHeaderCtrl().GetItemCount();                   // vslot 92 + HDM_GETITEMCOUNT -- deviation (3)
//     for (int i = 0; i < nCols; i++) DeleteColumn(0);              // LVM_DELETECOLUMN
//     static const UINT uiColumns[4] = { IDS_AFX_SHELLLISTCTRL_NAME .. _MODIFIED };  // 0x180350220
//     for (int i = 0; i < 4; i++) {
//         CString strColumn;
//         ENSURE(strColumn.LoadString(uiColumns[i]));               // AfxFindStringResourceHandle 0x2aee00 + LoadStringW 0xdb70
//         InsertColumn(i, strColumn, i == 1 ? LVCFMT_RIGHT : LVCFMT_LEFT, i == 1 ? 75 : 140, i);   // 0x295390
//     }
extern "C" void MS_ABI impl__OnSetColumns_CMFCShellListCtrl__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    const HWND hWnd = HWndOf(pThis);
    const int nCols = static_cast<int>(::SendMessage(HeaderHWnd(pThis), HDM_GETITEMCOUNT, 0, 0));
    for (int i = 0; i < nCols; i++) {
        ::SendMessage(hWnd, LVM_DELETECOLUMN, 0, 0);
    }
    for (int i = 0; i < 4; i++) {
        CString strColumn;
        HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(kColumnStringIds[i]));
        if (hInst == nullptr ||
            !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
                &strColumn, hInst, kColumnStringIds[i])) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(static_cast<CListCtrl*>(pThis), i, strColumn.GetString(),
                                                        (i == 1) ? LVCFMT_RIGHT : LVCFMT_LEFT,
                                                        (i == 1) ? 75 : 140, i);
    }
}

// Symbol: ?OnGetItemText@CMFCShellListCtrl@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HHPEAU_AFX_SHELLITEMINFO@@@Z
// Transcribed from retail entry RVA 0x12b5d0 (mfc140u).  The CString is
// returned through the hidden slot in RDX (this in RCX), the convention this
// tree uses for every CString-by-value export.
//     ENSURE(pItem != NULL);
//     switch (iColumn) {
//     case AFX_ShellList_ColumnName: {                              // 0
//         SHFILEINFO sfi;
//         if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))  // 0x208
//             return sfi.szDisplayName;
//         break; }
//     case AFX_ShellList_ColumnType: {                              // 2
//         SHFILEINFO sfi;
//         if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_TYPENAME))     // 0x408
//             return sfi.szTypeName;
//         break; }
//     case AFX_ShellList_ColumnSize:                                // 1
//     case AFX_ShellList_ColumnModified: {                          // 3
//         TCHAR szPath[MAX_PATH];
//         if (!SHGetPathFromIDList(pItem->pidlFQ, szPath)) break;
//         CFileStatus fs;
//         if (!CFile::GetStatus(szPath, fs, NULL)) break;           // 0x22bac0
//         CString str;
//         if (iColumn == AFX_ShellList_ColumnSize) {
//             if (!(fs.m_attribute & (CFile::directory | CFile::volume)))   // 0x18
//                 OnFormatFileSize(fs.m_size, str);                 // vslot 106, devirtualized (1)
//         } else {
//             OnFormatFileDate(fs.m_mtime, str);                    // vslot 107, devirtualized (1)
//         }
//         return str; }
//     }
//     return _T("");
// (iItem is not read.)
extern "C" CString* MS_ABI impl__OnGetItemText_CMFCShellListCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHPEAU_AFX_SHELLITEMINFO___Z(void* pThis, CString* pRet, int /*iItem*/, int iColumn, ShellItemInfo* pItem) {
    if (pRet == nullptr) return nullptr;
    if (pThis == nullptr || pItem == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return new (pRet) CString();
    }
    switch (iColumn) {
    case 0: {
        SHFILEINFOW sfi;
        if (::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pItem->pidlFQ), 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME)) {
            return new (pRet) CString(sfi.szDisplayName);
        }
        break;
    }
    case 2: {
        SHFILEINFOW sfi;
        if (::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pItem->pidlFQ), 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_TYPENAME)) {
            return new (pRet) CString(sfi.szTypeName);
        }
        break;
    }
    case 1:
    case 3: {
        wchar_t szPath[MAX_PATH];
        if (!::SHGetPathFromIDListW(pItem->pidlFQ, szPath)) break;
        CFileStatus fs;
        std::memset(&fs, 0, sizeof(fs));
        if (!impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(szPath, fs, nullptr)) break;
        CString* pStr = new (pRet) CString();
        if (iColumn == 1) {
            if (!(fs.m_attribute & (FILE_ATTRIBUTE_DIRECTORY | 0x08))) {   // CFile::directory | CFile::volume
                impl__OnFormatFileSize_CMFCShellListCtrl__UEAAX_JAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                    pThis, static_cast<__int64>(fs.m_size), pStr);
            }
        } else {
            const __int64 mtime = static_cast<__int64>(fs.m_mtime);
            impl__OnFormatFileDate_CMFCShellListCtrl__UEAAXAEBVCTime_ATL__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____3__Z(
                pThis, &mtime, pStr);
        }
        return pStr;
    }
    default:
        break;
    }
    return new (pRet) CString();
}

// Symbol: ?OnGetItemIcon@CMFCShellListCtrl@@UEAAHHPEAU_AFX_SHELLITEMINFO@@@Z
// Transcribed from retail entry RVA 0x12b7c0 (mfc140u):
//     if (pItem == NULL) return -1;
//     SHFILEINFO sfi;
//     if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi),
//             SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))   // 0xc009
//         return sfi.iIcon;
//     return -1;
// (this and iItem are not read.)
extern "C" int MS_ABI impl__OnGetItemIcon_CMFCShellListCtrl__UEAAHHPEAU_AFX_SHELLITEMINFO___Z(void* /*pThis*/, int /*iItem*/, ShellItemInfo* pItem) {
    if (pItem == nullptr) return -1;
    SHFILEINFOW sfi;
    if (::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pItem->pidlFQ), 0, &sfi, sizeof(sfi),
                         SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY)) {
        return sfi.iIcon;
    }
    return -1;
}

// Symbol: ?OnFormatFileSize@CMFCShellListCtrl@@UEAAX_JAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Transcribed from retail entry RVA 0x12bcf0 (mfc140u):
//     TCHAR szBuf[256];
//     StrFormatKBSize(lFileSize, szBuf, 255);                       // IAT 0x1802c6b70 SHLWAPI!StrFormatKBSizeW
//     str = szBuf;                                                  // SetString(szBuf, wcslen)
// (this is not read.)  StrFormatKBSizeW is resolved at run time here (see
// LoadStrFormatKBSizeW); if shlwapi cannot be loaded str is left unchanged.
extern "C" void MS_ABI impl__OnFormatFileSize_CMFCShellListCtrl__UEAAX_JAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*pThis*/, __int64 lFileSize, CString* str) {
    if (str == nullptr) return;
    StrFormatKBSizeWFn pfn = LoadStrFormatKBSizeW();
    if (pfn == nullptr) return;
    wchar_t szBuf[256];
    szBuf[0] = L'\0';
    pfn(lFileSize, szBuf, 255);
    *str = szBuf;
}

// Symbol: ?OnFormatFileDate@CMFCShellListCtrl@@UEAAXAEBVCTime@ATL@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@3@@Z
// Transcribed from retail entry RVA 0x12bd60 (mfc140u), which is the inlined
// ATL `COleDateTime dt(tmFile.GetTime()); str = dt.Format();`:
//     SYSTEMTIME st;  double dt = 0;  int status = valid;
//     // CTime::GetAsSystemTime (local helper 0x12c850): _localtime64_s then
//     // st.wYear = tm_year+1900, wMonth = tm_mon+1, wDayOfWeek = tm_wday, wDay,
//     // wHour, wMinute, wSecond, wMilliseconds = 0.
//     // COleDateTime::operator=(__time64_t): status = GetAsSystemTime(st) &&
//     //   ConvertSystemTimeToVariantTime(st) ? valid : invalid, where the latter
//     //   (local helper 0x12c8e4) is SystemTimeToVariantTime (oleaut32 #184) and
//     //   a VariantTimeToSystemTime (#185) round trip that must reproduce
//     //   wYear/wMonth/wDay/wHour/wMinute/wSecond.
//     str = dt.Format();   // local helper 0x12c508 = COleDateTime::Format(0, LANG_USER_DEFAULT):
//         // status == null    -> L""
//         // status == invalid -> LoadString(ATL_IDS_DATETIME_INVALID) else L"Invalid DateTime"
//         // else VarBstrFromDate(dt, 0x400, 0, &bstr) (oleaut32 #114); on failure the
//         //   same invalid-string path; on success str = bstr.
// (this is not read.)  OpenMFC's COleDateTime (afxole.h) has no Format, so the
// oleaut32 calls are made here directly.  NOTE: OpenMFC's CFile::GetStatus
// stores FILETIME ticks, not CTime seconds, in m_mtime (see the file header),
// so the value OnGetItemText passes in today does not convert.
extern "C" void MS_ABI impl__OnFormatFileDate_CMFCShellListCtrl__UEAAXAEBVCTime_ATL__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____3__Z(void* /*pThis*/, const __int64* tmFile, CString* str) {
    if (str == nullptr || tmFile == nullptr) return;
    bool valid = false;
    double dt = 0.0;
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    struct tm ptm;
    const __time64_t t = static_cast<__time64_t>(*tmFile);
    if (_localtime64_s(&ptm, &t) == 0) {
        st.wYear = static_cast<WORD>(ptm.tm_year + 1900);
        st.wMonth = static_cast<WORD>(ptm.tm_mon + 1);
        st.wDayOfWeek = static_cast<WORD>(ptm.tm_wday);
        st.wDay = static_cast<WORD>(ptm.tm_mday);
        st.wHour = static_cast<WORD>(ptm.tm_hour);
        st.wMinute = static_cast<WORD>(ptm.tm_min);
        st.wSecond = static_cast<WORD>(ptm.tm_sec);
        st.wMilliseconds = 0;
        if (::SystemTimeToVariantTime(&st, &dt)) {
            SYSTEMTIME stBack;
            std::memset(&stBack, 0, sizeof(stBack));
            if (::VariantTimeToSystemTime(dt, &stBack) &&
                stBack.wYear == st.wYear && stBack.wMonth == st.wMonth && stBack.wDay == st.wDay &&
                stBack.wHour == st.wHour && stBack.wMinute == st.wMinute && stBack.wSecond == st.wSecond) {
                valid = true;
            }
        }
    }
    BSTR bstr = nullptr;
    if (valid && SUCCEEDED(::VarBstrFromDate(dt, LANG_USER_DEFAULT, 0, &bstr))) {
        *str = bstr ? bstr : L"";
        if (bstr) ::SysFreeString(bstr);
        return;
    }
    CString strInvalid;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(kAtlIdsDateTimeInvalid));
    if (hInst != nullptr &&
        impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
            &strInvalid, hInst, kAtlIdsDateTimeInvalid)) {
        *str = strInvalid;
    } else {
        *str = L"Invalid DateTime";
    }
}

// Symbol: ?OnCompareItems@CMFCShellListCtrl@@UEAAH_J0H@Z
// Transcribed from retail entry RVA 0x12b830 (mfc140u):
//     AFX_SHELLITEMINFO* p1 = (AFX_SHELLITEMINFO*)lParam1, *p2 = ...lParam2;
//     ENSURE(p1 != NULL && p2 != NULL);
//     switch (iColumn) {
//     case AFX_ShellList_ColumnName: {                              // 0
//         HRESULT hr = p1->pParentFolder->CompareIDs(0, p1->pidlRel, p2->pidlRel);   // vslot 7
//         return FAILED(hr) ? 0 : (short)LOWORD(hr); }
//     case AFX_ShellList_ColumnSize:                                // 1
//     case AFX_ShellList_ColumnModified: {                          // 3
//         TCHAR szPath1[MAX_PATH], szPath2[MAX_PATH];  CFileStatus fs1, fs2;
//         if (!SHGetPathFromIDList(p1->pidlFQ, szPath1) || !CFile::GetStatus(szPath1, fs1, NULL)) return -1;
//         if (!SHGetPathFromIDList(p2->pidlFQ, szPath2) || !CFile::GetStatus(szPath2, fs2, NULL)) return 1;
//         if (iColumn == AFX_ShellList_ColumnSize) {
//             if (fs1.m_attribute & CFile::directory) return -1;     // 0x10
//             if (fs2.m_attribute & CFile::directory) return 1;
//             return fs1.m_size > fs2.m_size ? 1 : (fs1.m_size < fs2.m_size ? -1 : 0);   // unsigned compare
//         }
//         if (fs1.m_mtime < fs2.m_mtime) return -1;                 // signed compare
//         return fs1.m_mtime > fs2.m_mtime ? 1 : 0; }
//     case AFX_ShellList_ColumnType: {                              // 2
//         SHFILEINFO sfi1, sfi2;
//         if (!SHGetFileInfo((LPCTSTR)p1->pidlFQ, 0, &sfi1, sizeof(sfi1), SHGFI_PIDL | SHGFI_TYPENAME)) return 0;
//         if (!SHGetFileInfo((LPCTSTR)p2->pidlFQ, 0, &sfi2, sizeof(sfi2), SHGFI_PIDL | SHGFI_TYPENAME)) return 0;
//         return lstrcmpi(sfi1.szTypeName, sfi2.szTypeName); }      // IAT lstrcmpiW
//     }
//     return 0;
// (this is not read.)  Retail dereferences p1->pParentFolder without a NULL
// test in the column-0 case; a NULL guard returning 0 is added.
extern "C" int MS_ABI impl__OnCompareItems_CMFCShellListCtrl__UEAAH_J0H_Z(void* /*pThis*/, __int64 lParam1, __int64 lParam2, int iColumn) {
    const ShellItemInfo* p1 = reinterpret_cast<const ShellItemInfo*>(lParam1);
    const ShellItemInfo* p2 = reinterpret_cast<const ShellItemInfo*>(lParam2);
    if (p1 == nullptr || p2 == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;
    }
    switch (iColumn) {
    case 0: {
        if (p1->pParentFolder == nullptr) return 0;
        const HRESULT hr = p1->pParentFolder->CompareIDs(0, p1->pidlRel, p2->pidlRel);
        if (FAILED(hr)) return 0;
        return static_cast<short>(LOWORD(hr));
    }
    case 1:
    case 3: {
        wchar_t szPath1[MAX_PATH];
        wchar_t szPath2[MAX_PATH];
        CFileStatus fs1;
        CFileStatus fs2;
        std::memset(&fs1, 0, sizeof(fs1));
        std::memset(&fs2, 0, sizeof(fs2));
        if (!::SHGetPathFromIDListW(p1->pidlFQ, szPath1) ||
            !impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(szPath1, fs1, nullptr)) {
            return -1;
        }
        if (!::SHGetPathFromIDListW(p2->pidlFQ, szPath2) ||
            !impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(szPath2, fs2, nullptr)) {
            return 1;
        }
        if (iColumn == 1) {
            if (fs1.m_attribute & FILE_ATTRIBUTE_DIRECTORY) return -1;
            if (fs2.m_attribute & FILE_ATTRIBUTE_DIRECTORY) return 1;
            if (fs1.m_size > fs2.m_size) return 1;
            if (fs1.m_size < fs2.m_size) return -1;
            return 0;
        }
        const __int64 t1 = static_cast<__int64>(fs1.m_mtime);
        const __int64 t2 = static_cast<__int64>(fs2.m_mtime);
        if (t1 < t2) return -1;
        return t1 > t2 ? 1 : 0;
    }
    case 2: {
        SHFILEINFOW sfi1;
        SHFILEINFOW sfi2;
        if (!::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(p1->pidlFQ), 0, &sfi1, sizeof(sfi1), SHGFI_PIDL | SHGFI_TYPENAME)) return 0;
        if (!::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(p2->pidlFQ), 0, &sfi2, sizeof(sfi2), SHGFI_PIDL | SHGFI_TYPENAME)) return 0;
        return ::lstrcmpiW(sfi1.szTypeName, sfi2.szTypeName);
    }
    default:
        break;
    }
    return 0;
}

// Symbol: ?OnContextMenu@CMFCShellListCtrl@@IEAAXPEAVCWnd@@VCPoint@@@Z
// CPoint arrives by value as an 8-byte aggregate in R8 (x in the low dword,
// y in the high dword), modelled as `long long`.  Transcribed from retail
// entry RVA 0x12be20 (mfc140u):
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     if (m_pContextMenu2 != NULL) return;
//     if (!m_bContextMenu) { Default(); return; }                   // 0x28ac80
//     if (m_psfCurFolder == NULL) return;
//     UINT nSelItems = GetSelectedCount();                          // LVM_GETSELECTEDCOUNT
//     int iItem;
//     if (point.x == -1 && point.y == -1) {                         // keyboard
//         if (nSelItems == 0) return;
//         int iLast = -1;
//         for (UINT i = 0; i < nSelItems; i++) iLast = GetNextItem(iLast, LVNI_SELECTED);   // last selected
//         RECT rc = { 0, 0, 0, 0 };   // rc.left = LVIR_BOUNDS (== 0), as CListCtrl::GetItemRect stores it
//         if (GetItemRect(iLast, rc, LVIR_BOUNDS)) { point = CPoint(rc.left, rc.bottom + 1); ClientToScreen(&point); }
//         iItem = -1;
//     } else {
//         LVHITTESTINFO ht;  ht.pt = point;  ScreenToClient(&ht.pt);  ht.flags = LVHT_NOWHERE;
//         HitTest(&ht);                                             // LVM_HITTEST
//         if (!(ht.flags & LVHT_ONITEM)) return;
//         iItem = ht.iItem;
//     }
//     LPCITEMIDLIST* pPidls = (LPCITEMIDLIST*)m_pMalloc->Alloc(nSelItems * sizeof(LPCITEMIDLIST));   // IMalloc vslot 3 -> CoTaskMemAlloc
//     ENSURE(pPidls != NULL);
//     LVITEM lvi = {};  lvi.mask = LVIF_PARAM;
//     AFX_SHELLITEMINFO* pInfo = NULL;
//     if (iItem >= 0) { lvi.iItem = iItem; if (GetItem(&lvi)) { pInfo = (AFX_SHELLITEMINFO*)lvi.lParam; pPidls[0] = pInfo->pidlRel; } }
//     int iCur = -1;
//     for (UINT i = (iItem >= 0 ? 1 : 0); i < nSelItems; i++) {
//         iCur = GetNextItem(iCur, LVNI_SELECTED);
//         if (iCur == iItem) { i--; continue; }
//         lvi.iItem = iCur;
//         if (GetItem(&lvi)) { AFX_SHELLITEMINFO* p = (AFX_SHELLITEMINFO*)lvi.lParam; pPidls[i] = p->pidlRel; if (pInfo == NULL) pInfo = p; }
//     }
//     if (pPidls[0] != NULL) {
//         IContextMenu* pcm;
//         if (SUCCEEDED(m_psfCurFolder->GetUIObjectOf(m_hWnd, nSelItems, pPidls, IID_IContextMenu, NULL, (void**)&pcm))) {   // vslot 10
//             if (SUCCEEDED(pcm->QueryInterface(IID_IContextMenu2 /*0x1802d9868*/, (void**)&m_pContextMenu2))) {
//                 HMENU hMenu = CreatePopupMenu();
//                 if (hMenu != NULL && SUCCEEDED(m_pContextMenu2->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {
//                     UINT idCmd = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD /*0x102*/, point.x, point.y, 0, m_hWnd, NULL);
//                     if (idCmd != 0) {
//                         BOOL bDone = FALSE;
//                         if (nSelItems == 1 && idCmd == GetMenuDefaultItem(hMenu, FALSE, 0)) {
//                             ULONG dwAttr = SFGAO_FOLDER;
//                             m_psfCurFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pInfo->pidlRel, &dwAttr);
//                             if (dwAttr & SFGAO_FOLDER) { DisplayFolder(pInfo); bDone = TRUE; }   // vslot 100, devirtualized (1)
//                         }
//                         if (!bDone) {
//                             CMINVOKECOMMANDINFO ici = {};  ici.cbSize = sizeof(ici);
//                             ici.hwnd = CWnd::FromHandle(GetParent(m_hWnd)) ? ->m_hWnd : NULL;
//                             ici.lpVerb = MAKEINTRESOURCEA(idCmd - 1);  ici.nShow = SW_SHOWNORMAL;
//                             if (SUCCEEDED(pcm->InvokeCommand(&ici)) && CWnd::FromHandle(GetParent(m_hWnd)) != NULL)
//                                 ::SendMessage(CWnd::FromHandle(GetParent(m_hWnd))->m_hWnd, AFX_WM_ON_AFTER_SHELL_COMMAND, idCmd, 0);
//                         }
//                     }
//                 }
//                 if (m_pContextMenu2 != NULL) { m_pContextMenu2->Release(); m_pContextMenu2 = NULL; }
//             }
//             pcm->Release();
//         }
//     }
//     m_pMalloc->Free(pPidls);                                      // -> CoTaskMemFree
// (hMenu is not destroyed -- retail has no DestroyMenu call in this body.  As
// in retail, pPidls is sized from GetSelectedCount even on the mouse path,
// where a hit item that is not selected gives nSelItems == 0.)
extern "C" void MS_ABI impl__OnContextMenu_CMFCShellListCtrl__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* /*pWnd*/, long long pointPacked) {
    if (pThis == nullptr) return;
    ShellListCtrlLayout* s = L(pThis);
    const HWND hWnd = HWndOf(pThis);
    EnsureShellAllocator();
    if (impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA != nullptr) return;
    if (!s->m_bContextMenu) {
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
        return;
    }
    if (s->m_psfCurFolder == nullptr) return;

    POINT point;
    point.x = static_cast<LONG>(static_cast<unsigned long long>(pointPacked) & 0xffffffffULL);
    point.y = static_cast<LONG>(static_cast<unsigned long long>(pointPacked) >> 32);

    const UINT nSelItems = static_cast<UINT>(::SendMessage(hWnd, LVM_GETSELECTEDCOUNT, 0, 0));
    int iItem;
    if (point.x == -1 && point.y == -1) {
        if (nSelItems == 0) return;
        int iLast = -1;
        for (UINT i = 0; i < nSelItems; i++) {
            iLast = static_cast<int>(::SendMessage(hWnd, LVM_GETNEXTITEM, static_cast<WPARAM>(iLast), LVNI_SELECTED));
        }
        RECT rc;
        rc.left = LVIR_BOUNDS;
        rc.top = 0;
        rc.right = 0;
        rc.bottom = 0;
        if (::SendMessage(hWnd, LVM_GETITEMRECT, static_cast<WPARAM>(iLast), reinterpret_cast<LPARAM>(&rc))) {
            point.x = rc.left;
            point.y = rc.bottom + 1;
            ::ClientToScreen(hWnd, &point);
        }
        iItem = -1;
    } else {
        LVHITTESTINFO ht;
        std::memset(&ht, 0, sizeof(ht));
        ht.pt = point;
        ::ScreenToClient(hWnd, &ht.pt);
        ht.flags = LVHT_NOWHERE;
        ::SendMessage(hWnd, LVM_HITTEST, 0, reinterpret_cast<LPARAM>(&ht));
        if (!(ht.flags & LVHT_ONITEM)) return;
        iItem = ht.iItem;
    }

    LPCITEMIDLIST* pPidls = static_cast<LPCITEMIDLIST*>(::CoTaskMemAlloc(static_cast<SIZE_T>(nSelItems) * sizeof(LPCITEMIDLIST)));
    if (pPidls == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    LVITEMW lvi;
    std::memset(&lvi, 0, sizeof(lvi));
    lvi.mask = LVIF_PARAM;
    ShellItemInfo* pInfo = nullptr;
    if (iItem >= 0) {
        lvi.iItem = iItem;
        if (::SendMessage(hWnd, LVM_GETITEMW, 0, reinterpret_cast<LPARAM>(&lvi))) {
            pInfo = reinterpret_cast<ShellItemInfo*>(lvi.lParam);
            pPidls[0] = pInfo->pidlRel;
        }
    }
    int iCur = -1;
    for (UINT i = (iItem >= 0) ? 1u : 0u; i < nSelItems; i++) {
        iCur = static_cast<int>(::SendMessage(hWnd, LVM_GETNEXTITEM, static_cast<WPARAM>(iCur), LVNI_SELECTED));
        if (iCur == iItem) {
            i--;
            continue;
        }
        lvi.iItem = iCur;
        if (::SendMessage(hWnd, LVM_GETITEMW, 0, reinterpret_cast<LPARAM>(&lvi))) {
            ShellItemInfo* p = reinterpret_cast<ShellItemInfo*>(lvi.lParam);
            pPidls[i] = p->pidlRel;
            if (pInfo == nullptr) pInfo = p;
        }
    }

    if (pPidls[0] != nullptr) {
        IContextMenu* pcm = nullptr;
        if (SUCCEEDED(s->m_psfCurFolder->GetUIObjectOf(hWnd, nSelItems, pPidls, IID_IContextMenu, nullptr,
                                                       reinterpret_cast<void**>(&pcm)))) {
            if (SUCCEEDED(pcm->QueryInterface(IID_IContextMenu2,
                                              &impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA))) {
                IContextMenu2* pcm2 = static_cast<IContextMenu2*>(impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA);
                HMENU hMenu = ::CreatePopupMenu();
                if (hMenu != nullptr && SUCCEEDED(pcm2->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {
                    const UINT idCmd = static_cast<UINT>(::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
                                                                          point.x, point.y, 0, hWnd, nullptr));
                    if (idCmd != 0) {
                        bool bDone = false;
                        if (nSelItems == 1 && idCmd == ::GetMenuDefaultItem(hMenu, FALSE, 0)) {
                            ULONG dwAttr = SFGAO_FOLDER;
                            LPCITEMIDLIST pidlAttr = pInfo->pidlRel;
                            s->m_psfCurFolder->GetAttributesOf(1, &pidlAttr, &dwAttr);
                            if (dwAttr & SFGAO_FOLDER) {
                                impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pThis, pInfo);
                                bDone = true;
                            }
                        }
                        if (!bDone) {
                            CMINVOKECOMMANDINFO ici;
                            std::memset(&ici, 0, sizeof(ici));
                            ici.cbSize = sizeof(ici);
                            ici.fMask = 0;
                            ici.hwnd = ParentHWndViaFromHandle(pThis);
                            ici.lpVerb = MAKEINTRESOURCEA(idCmd - 1);
                            ici.lpParameters = nullptr;
                            ici.lpDirectory = nullptr;
                            ici.nShow = SW_SHOWNORMAL;
                            ici.dwHotKey = 0;
                            ici.hIcon = nullptr;
                            if (SUCCEEDED(pcm->InvokeCommand(&ici)) &&
                                impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd)) != nullptr) {
                                ::SendMessage(HWndOf(impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd))),
                                              impl__AFX_WM_ON_AFTER_SHELL_COMMAND__3IA, static_cast<WPARAM>(idCmd), 0);
                            }
                        }
                    }
                }
                if (impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA != nullptr) {
                    static_cast<IContextMenu2*>(impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA)->Release();
                    impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA = nullptr;
                }
            }
            pcm->Release();
        }
    }
    ::CoTaskMemFree(pPidls);
}

// Symbol: ?WindowProc@CMFCShellListCtrl@@MEAA_JI_K_J@Z
// Transcribed from retail entry RVA 0x12c390 (mfc140u):
//     if ((message == WM_DRAWITEM || message == WM_MEASUREITEM || message == WM_INITMENUPOPUP) &&
//         m_pContextMenu2 != NULL) {
//         m_pContextMenu2->HandleMenuMsg(message, wParam, lParam);  // IContextMenu2 vslot 6
//         return 0;
//     }
//     return CWnd::WindowProc(message, wParam, lParam);             // 0x28d140 (CWnd's, not CMFCListCtrl's)
extern "C" LRESULT MS_ABI impl__WindowProc_CMFCShellListCtrl__MEAA_JI_K_J_Z(void* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    if (pThis == nullptr) return 0;
    if ((message == WM_DRAWITEM || message == WM_MEASUREITEM || message == WM_INITMENUPOPUP) &&
        impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA != nullptr) {
        static_cast<IContextMenu2*>(impl__m_pContextMenu2_CMFCShellListCtrl__1PEAUIContextMenu2__EA)->HandleMenuMsg(message, wParam, lParam);
        return 0;
    }
    return impl__WindowProc_CWnd__MEAA_JI_K_J_Z(static_cast<CWnd*>(pThis), message, wParam, lParam);
}

// Symbol: ?OnInitControl@CMFCShellListCtrl@@IEAA_J_K_J@Z
// Transcribed from retail entry RVA 0x12c3e0 (mfc140u):
//     CString strDst;
//     CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                               // 0x13d840
//     CString strValue;
//     if (tagManager.ExcludeTag(_T("MFCShellListCtrl_EnableShellContextMenu"), strValue, FALSE) &&  // 0x13df50
//         !strValue.IsEmpty()) {
//         strValue.MakeUpper();                                     // 0x1fb80
//         m_bContextMenu = (wcscmp(strValue, _T("TRUE")) == 0);
//     }
//     return 0;                                                     // (~strValue, ~tagManager, ~strDst)
// The CTagManager thunks in controls/CTagManager.cpp key their state by the
// object address and never touch its storage, so an opaque local buffer
// stands in for the retail object.
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCShellListCtrl__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));
    alignas(16) unsigned char tagManagerStorage[64];
    std::memset(tagManagerStorage, 0, sizeof(tagManagerStorage));
    impl___0CTagManager__QEAA_PEB_W_Z(tagManagerStorage, strDst.GetString());
    {
        CString strValue;
        if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
                tagManagerStorage, L"MFCShellListCtrl_EnableShellContextMenu", &strValue, FALSE) &&
            !strValue.IsEmpty()) {
            impl__MakeUpper___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&strValue);
            L(pThis)->m_bContextMenu = (std::wcscmp(strValue.GetString(), L"TRUE") == 0) ? TRUE : FALSE;
        }
    }
    impl___1CTagManager__UEAA_XZ(tagManagerStorage);
    return 0;
}
