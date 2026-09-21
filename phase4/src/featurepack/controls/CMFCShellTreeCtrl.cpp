// CMFCShellTreeCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <shlobj.h>
#include <shellapi.h>
#include <cstring>
#include <cwchar>

// ===========================================================================
// CMFCShellTreeCtrl -- the shell-namespace tree control.
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the entry RVA it was read from (all RVAs in this file are mfc140u).
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives; client (MSVC) code allocates the object with
// the retail member layout, which is pinned by ShellTreeCtrlLayout below.
//
// Base class.  The shipping afxshelltreectrl.h on this host
// (atlmfc/include/afxshelltreectrl.h) declares
// `class CMFCShellTreeCtrl : public CTreeCtrl` -- there is no CMFCTreeCtrl
// in between (mfc_complete_ordinal_mapping.json has no CMFCTreeCtrl export
// either).  The retail constructor (RVA 0x12d010) calls ??0CWnd@@QEAA@XZ
// (0x28a700) directly and the destructor (0x12d0b0) tail-jumps to
// ??1CTreeCtrl@@UEAA@XZ (0x295930), which is consistent with that: CTreeCtrl
// adds no members to CWnd (include/openmfc/afxwin.h:3123, sizeof 0xe8).
//
// Retail object layout, read from the constructor (RVA 0x12d010) and cross-
// checked against afxshelltreectrl.h (m_bContextMenu, m_bNoNotify,
// m_hwndRelatedList, m_dwFlags, in that order after the static
// m_pContextMenu2):
//
//   +0x000  CTreeCtrl base == CWnd, 0xe8 bytes (m_hWnd at +0x40)
//   +0x0e8  BOOL  m_bContextMenu     (ctor: movq $1,0xe8 -> TRUE ...)
//   +0x0ec  BOOL  m_bNoNotify        (... and FALSE, one qword store)
//   +0x0f0  HWND  m_hwndRelatedList  (ctor stores NULL)
//   +0x0f8  DWORD m_dwFlags          (ctor stores 0x20 = SHCONTF_FOLDERS)
//   sizeof == 0x100  (m_nObjectSize of the retail CRuntimeClass at
//                     0x18030ebe8 in mfc140u, the descriptor
//                     ?GetRuntimeClass@CMFCShellTreeCtrl (RVA 0x12d000) returns)
//
// Retail vtable (0x18030ed68 in mfc140u; slots read with the workflow's
// vtable dumper).  The slots the bodies below dispatch through are:
//   slot  91 (+0x2d8)  OnGetItemText(LPAFX_SHELLITEMINFO)         (0x12dc10)
//   slot  92 (+0x2e0)  OnGetItemIcon(LPAFX_SHELLITEMINFO, BOOL)   (0x12dca0)
//   slot  93 (+0x2e8)  EnumObjects(HTREEITEM, IShellFolder*, LPITEMIDLIST) (0x12d570)
// and, on the related CMFCShellListCtrl (vtable 0x18030e818):
//   slot 100 (+0x320)  DisplayFolder(LPAFX_SHELLITEMINFO)
//
// Deviations from retail, applied uniformly and repeated at each site (four
// of the five the sibling controls/CMFCShellListCtrl.cpp documents; its
// list-specific GetHeaderCtrl deviation does not arise here):
//
//  (1) Virtual dispatch on `this` is devirtualized.  OpenMFC models no
//      MSVC-layout vtable for this class (the constructor below does NOT
//      store the retail vftable pointer -- see its comment), so every retail
//      `this->vslot` call above is made as a direct call to this file's own
//      thunk, and the list control's DisplayFolder is called through its
//      impl__ thunk.  A client class that overrides one of those virtuals is
//      therefore not honoured from inside the DLL.
//
//  (2) afxShellManager->m_pMalloc.  Retail keeps an IMalloc* at +0x8 of the
//      CShellManager and frees every PIDL through it (IMalloc::Free = vslot
//      5) after an ENSURE(m_pMalloc != NULL).  OpenMFC's CShellManager
//      (include/openmfc/afxmfc.h) carries no such member -- its PIDL helpers
//      in core/app/CShellManager.cpp use CoTaskMemAlloc/CoTaskMemFree, which
//      IS the allocator SHGetMalloc hands out -- so at every such site the
//      test collapses to `afxShellManager != NULL` (throwing the same
//      CInvalidArgException retail's ENSURE throws) and the call becomes
//      ::CoTaskMemFree.
//
//  (3) CTreeCtrl / CWnd inline wrappers that retail expands to a raw
//      SendMessage (GetRootItem, GetSelectedItem, GetChildItem,
//      GetParentItem, GetNextSiblingItem, SelectItem, Expand, EnsureVisible,
//      GetItem, SetItem, InsertItem, DeleteAllItems, SortChildrenCB,
//      SetScrollTime, SetImageList, SetRedraw, GetItemRect) are written as
//      the same ::SendMessage on m_hWnd, exactly as the retail code does.
//      SetImageList's CImageList::FromHandle round trip (it only registers a
//      temporary CImageList in the handle map) is dropped and the raw
//      HIMAGELIST is passed; the message sent is identical.
//
//  (4) CString.  Retail calls the exported CStringT ctor(PCXSTR) (0xdcb0),
//      CSimpleStringT::Empty (0x33b0) and SetString (0x2e30), and inlines
//      GetBuffer() down to a call of the non-exported buffer-grow helper at
//      0x31c0; OpenMFC's ABI-compatible CString (include/openmfc/afxstr.h)
//      performs the same operations inline and is used directly.  Where
//      retail hands `strItem.GetBuffer()` to TVM_INSERTITEMW,
//      `strItem.GetString()` is passed: the message only reads the text.
//
// Known dependency gaps outside this file (all reported as headerRequests):
//   * impl__afxShellManager (featurepack/CMFC_misc_stubs.cpp) is a NULL export
//     and CWinAppEx::InitShellManager (core/app/CWinAppEx.cpp) never sets it,
//     so -- exactly as retail behaves for an application that never called
//     InitShellManager() -- OnCreate returns -1, and GetRootItems (reached
//     from PreSubclassWindow -> InitTree -> Refresh) throws
//     CInvalidArgException, until that is fixed.
//   * ?GetParentItem@CShellManager (core/app/CShellManager.cpp) returns
//     TRUE/FALSE where retail returns the parent's element count (0 for a
//     one-element PIDL, when it also hands back the empty desktop PIDL); the
//     loop in SelectPath(LPCITEMIDLIST) is written so both agree (see there).
//   * ?OnDestroy@CTreeCtrl (core/controls/CTreeCtrl.cpp) is a no-op stub;
//     retail's (0x295d80) is RemoveImageList(TVSIL_NORMAL),
//     RemoveImageList(TVSIL_STATE), CWnd::OnDestroy().  OnDestroy below calls
//     it as retail does, so WM_DESTROY currently gets no default handling.
//   * The CMFCShellTreeCtrl message map (detail/Mfc07MsgmapSupport.cpp) has
//     no entries; retail's (0x18030ec48) has seven (see the headerRequest).
// ===========================================================================

namespace {

// File-local layout view of the retail object (offsets cited in the header).
struct ShellTreeCtrlLayout {
    unsigned char base[0xe8];      // CTreeCtrl == CWnd (m_hWnd at +0x40)
    BOOL  m_bContextMenu;          // +0xe8
    BOOL  m_bNoNotify;             // +0xec
    HWND  m_hwndRelatedList;       // +0xf0
    DWORD m_dwFlags;               // +0xf8  SHCONTF flags for IShellFolder::EnumObjects
    DWORD pad_fc;
};
static_assert(sizeof(ShellTreeCtrlLayout) == 0x100, "CMFCShellTreeCtrl: retail sizeof 0x100 (CRuntimeClass m_nObjectSize)");
static_assert(offsetof(ShellTreeCtrlLayout, m_bContextMenu) == 0xe8, "m_bContextMenu +0xe8");
static_assert(offsetof(ShellTreeCtrlLayout, m_bNoNotify) == 0xec, "m_bNoNotify +0xec");
static_assert(offsetof(ShellTreeCtrlLayout, m_hwndRelatedList) == 0xf0, "m_hwndRelatedList +0xf0");
static_assert(offsetof(ShellTreeCtrlLayout, m_dwFlags) == 0xf8, "m_dwFlags +0xf8");
static_assert(sizeof(CTreeCtrl) == 0xe8, "CTreeCtrl base must be 0xe8 bytes (retail CWnd)");

// CWnd::m_hWnd (include/openmfc/afxwin.h places it at 64 == 0x40; every retail
// body in this file reads it as 0x40(%rcx)).
constexpr size_t kHWndOffset = 0x40;
static_assert(offsetof(CWnd, m_hWnd) == kHWndOffset, "CWnd::m_hWnd +0x40");

// CMFCShellListCtrl members this file touches, at the offsets the sibling
// controls/CMFCShellListCtrl.cpp pins in its ShellListCtrlLayout:
//   +0x260 HWND m_hwndRelatedTree  -- written by SetRelatedList (RVA 0x12d110,
//                                     `mov %rax,0x260(%rdx)`) and OnDestroy
//                                     (0x12e590, `movq $0,0x260(%rax)`)
//   +0x270 BOOL m_bNoNotify        -- written by OnChildNotify (0x12e4a0,
//                                     `mov %ebx,0x270(%rsi)` around the
//                                     DisplayFolder call)
constexpr size_t kListRelatedTreeHwndOffset = 0x260;
constexpr size_t kListNoNotifyOffset = 0x270;

// AFX_SHELLITEMINFO (afxshellmanager.h: pParentFolder, pidlFQ, pidlRel).  Not
// declared in OpenMFC's headers.  The offsets are the ones every retail body
// below reads: (%rax) parent folder, 0x8(%rax) pidlFQ, 0x10(%rax) pidlRel.
struct ShellItemInfo {
    IShellFolder* pParentFolder;
    LPITEMIDLIST  pidlFQ;
    LPITEMIDLIST  pidlRel;
};
static_assert(sizeof(ShellItemInfo) == 0x18, "AFX_SHELLITEMINFO is 24 bytes (GlobalAlloc(GPTR, 0x18))");
static_assert(offsetof(ShellItemInfo, pidlFQ) == 0x8, "AFX_SHELLITEMINFO::pidlFQ +0x8");
static_assert(offsetof(ShellItemInfo, pidlRel) == 0x10, "AFX_SHELLITEMINFO::pidlRel +0x10");

// Field-for-field mirror of detail/CWinAppSupport.h's `struct _AFX_THREAD_STATE`
// (int nTempMapLock; int nWndCreateLock; void* pModuleState; void* pWndInit;
// void* hHookOldCbtFilter;), used only to read pWndInit in PreSubclassWindow --
// the same mirror controls/CMFCShellListCtrl.cpp uses.
struct AfxThreadStateMirror {
    int nTempMapLock;
    int nWndCreateLock;
    void* pModuleState;
    void* pWndInit;
    void* hHookOldCbtFilter;
};
static_assert(sizeof(AfxThreadStateMirror) == 32, "AfxThreadStateMirror: five-field _AFX_THREAD_STATE");
static_assert(offsetof(AfxThreadStateMirror, pWndInit) == 16, "AfxThreadStateMirror::pWndInit");

inline ShellTreeCtrlLayout* L(void* p) { return static_cast<ShellTreeCtrlLayout*>(p); }
inline const ShellTreeCtrlLayout* L(const void* p) { return static_cast<const ShellTreeCtrlLayout*>(p); }
inline HWND HWndOf(const void* p) {
    return *reinterpret_cast<const HWND*>(static_cast<const unsigned char*>(p) + kHWndOffset);
}

// CPoint arrives by value as an 8-byte aggregate in a register (x in the low
// dword, y in the high dword), modelled as `long long` in the thunk
// signatures below.
inline POINT PointFromPacked(long long packed) {
    POINT pt;
    pt.x = static_cast<LONG>(static_cast<unsigned long long>(packed) & 0xffffffffULL);
    pt.y = static_cast<LONG>(static_cast<unsigned long long>(packed) >> 32);
    return pt;
}

} // namespace

// ---- sibling impl__ exports called by the bodies in this file -------------
// (see the link rule in the campaign briefing: C++ methods of other classes
// exist in this DLL only as impl__ thunks.)  Every declaration matches a
// definition that exists today; the file that holds it is named.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                   // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CTreeCtrl__UEAA_XZ(void* pThis);                              // core/controls/CtorDtorPlacement.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                          // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                       // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(CWnd* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam, __int64* pLResult); // core/window/Thunks.cpp
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam); // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__OnDestroy_CTreeCtrl__QEAAXXZ(CTreeCtrl* pThis);                // core/controls/CTreeCtrl.cpp (no-op stub today)
extern "C" HTREEITEM MS_ABI impl__HitTest_CTreeCtrl__QEBAPEAU_TREEITEM__VCPoint__PEAI_Z(const CTreeCtrl* pThis, CPoint pt, UINT* pFlags); // core/controls/CTreeCtrl.cpp
extern "C" uintptr_t MS_ABI impl__GetItemData_CTreeCtrl__QEBA_KPEAU_TREEITEM___Z(const CTreeCtrl* pThis, HTREEITEM hItem); // core/controls/CTreeCtrl.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);            // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass); // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ(); // controls/RuntimeClasses.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                             // detail/RegcoreSupport.cpp
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);        // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);          // core/runtime/CCmdTarget.cpp
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ(); // detail/CWinAppSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                          // detail/MfcExceptionsSupport.cpp
extern "C" int   MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const char* pszUTF8, CString* pStrResult, int cbUTF8); // controls/CMFCControlContainer.cpp
extern "C" void  MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer);  // controls/CTagManager.cpp
extern "C" void  MS_ABI impl___1CTagManager__UEAA_XZ(void* self);                             // controls/CTagManager.cpp
extern "C" int   MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(void* self, const wchar_t* tag, CString* value, int trim); // controls/CTagManager.cpp
extern "C" CString* MS_ABI impl__MakeUpper___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis); // core/collections/CStringT.cpp
extern "C" HRESULT MS_ABI impl__ItemFromPath_CShellManager__QEAAJPEB_WAEAPEFAU_ITEMIDLIST___Z(void* pThis, const wchar_t* path, ITEMIDLIST** pidl); // core/app/CShellManager.cpp
extern "C" ITEMIDLIST* MS_ABI impl__ConcatenateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2_0_Z(void* pThis, const ITEMIDLIST* left, const ITEMIDLIST* right); // core/app/CShellManager.cpp
extern "C" ITEMIDLIST* MS_ABI impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(void* pThis, const ITEMIDLIST* pidl); // core/app/CShellManager.cpp
extern "C" int   MS_ABI impl__GetParentItem_CShellManager__QEAAHPEFBU_ITEMIDLIST__AEAPEFAU2__Z(void* pThis, const ITEMIDLIST* pidl, ITEMIDLIST** parent); // core/app/CShellManager.cpp
extern "C" long  MS_ABI impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(void* pThis, ShellItemInfo* lpItemInfo); // controls/CMFCShellListCtrl.cpp
// Exported data this file reads.
extern "C" void* impl__afxShellManager__3PEAVCShellManager__EA;                  // featurepack/CMFC_misc_stubs.cpp (CShellManager*, NULL today)
extern "C" void* impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA; // core/runtime/StaticData.cpp (IContextMenu2*)
extern "C" unsigned int impl__AFX_WM_ON_AFTER_SHELL_COMMAND__3IA;                // core/runtime/Globals.cpp

// Thunks defined further down this file that earlier bodies call.
extern "C" int   MS_ABI impl__GetRootItems_CMFCShellTreeCtrl__IEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__GetChildItems_CMFCShellTreeCtrl__IEAAHPEAU_TREEITEM___Z(void* pThis, HTREEITEM hParentItem);
extern "C" long  MS_ABI impl__EnumObjects_CMFCShellTreeCtrl__MEAAJPEAU_TREEITEM__PEAUIShellFolder__PEFAU_ITEMIDLIST___Z(void* pThis, HTREEITEM hParentItem, IShellFolder* pParentFolder, LPITEMIDLIST pidlParent);
extern "C" int   MS_ABI impl__CompareProc_CMFCShellTreeCtrl__KAH_J00_Z(__int64 lParam1, __int64 lParam2, __int64 lParamSort);
extern "C" CString* MS_ABI impl__OnGetItemText_CMFCShellTreeCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAU_AFX_SHELLITEMINFO___Z(void* pThis, CString* pRet, ShellItemInfo* pItem);
extern "C" int   MS_ABI impl__OnGetItemIcon_CMFCShellTreeCtrl__UEAAHPEAU_AFX_SHELLITEMINFO__H_Z(void* pThis, ShellItemInfo* pItem, int bSelected);
extern "C" void* MS_ABI impl__GetRelatedList_CMFCShellTreeCtrl__QEBAPEAVCMFCShellListCtrl__XZ(const void* pThis);
extern "C" void  MS_ABI impl__Refresh_CMFCShellTreeCtrl__QEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__InitTree_CMFCShellTreeCtrl__IEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__OnShowContextMenu_CMFCShellTreeCtrl__IEAAXVCPoint___Z(void* pThis, long long pointPacked);
extern "C" int   MS_ABI impl__SelectPath_CMFCShellTreeCtrl__QEAAHPEFBU_ITEMIDLIST__H_Z(void* pThis, const ITEMIDLIST* pidl, int bExpandChildren);

namespace {

// CWaitCursor: retail's `CWaitCursor wait;` is AfxGetModuleState()->
// m_pCurrentWinApp->BeginWaitCursor() (0x1de7b0) with EndWaitCursor in the
// destructor (the local helper at 0x7687c); AfxGetApp() is that same
// pointer.  Same helper shape as CMFCShellListCtrl.cpp's WaitCursorScope.
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

// The parent CWnd's m_hWnd the way retail reads it: CWnd::FromHandle(
// ::GetParent(m_hWnd)) then ->m_hWnd (0x40) when non-NULL, else NULL.
HWND ParentHWndViaFromHandle(const void* pThis) {
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
    return pParent ? HWndOf(pParent) : nullptr;
}

// TVM_GETNEXTITEM wrappers (deviation (3)).
inline HTREEITEM TreeNext(HWND hWnd, UINT nCode, HTREEITEM hItem) {
    return reinterpret_cast<HTREEITEM>(::SendMessage(hWnd, TVM_GETNEXTITEM, static_cast<WPARAM>(nCode), reinterpret_cast<LPARAM>(hItem)));
}

} // namespace

// Symbol: ??0CMFCShellTreeCtrl@@QEAA@XZ
// Transcribed from retail entry RVA 0x12d010 (mfc140u):
//     CWnd::CWnd();                                                 // 0x28a700 (CTreeCtrl's ctor is trivial and not called)
//     vfptr = 0x18030ed68;  (NOT reproduced -- see below)
//     m_bContextMenu = TRUE;  m_bNoNotify = FALSE;                  // movq $1,0xe8
//     m_hwndRelatedList = NULL;                                     // movq $0,0xf0
//     m_dwFlags = SHCONTF_FOLDERS;                                  // movl $0x20,0xf8
// The vftable store is omitted: OpenMFC has no MSVC-layout vtable for this
// class, and whatever the CWnd constructor thunk installs at +0x00 is left
// alone.
extern "C" void* MS_ABI impl___0CMFCShellTreeCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CWnd__QEAA_XZ(pThis);
    ShellTreeCtrlLayout* s = L(pThis);
    s->m_bContextMenu = TRUE;
    s->m_bNoNotify = FALSE;
    s->m_hwndRelatedList = nullptr;
    s->m_dwFlags = SHCONTF_FOLDERS;
    return pThis;
}

// Symbol: ??1CMFCShellTreeCtrl@@UEAA@XZ
// Transcribed from retail entry RVA 0x12d0b0 (mfc140u): stores the class
// vftable (0x18030ed68; omitted here, see the constructor) and tail-jumps to
// ??1CTreeCtrl@@UEAA@XZ (0x295930).  It does not touch m_hwndRelatedList --
// the cross-link is cleared in OnDestroy.
extern "C" void MS_ABI impl___1CMFCShellTreeCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    impl___1CTreeCtrl__UEAA_XZ(pThis);
}

// Symbol: ?EnableShellContextMenu@CMFCShellTreeCtrl@@QEAAXH@Z
// Retail entry RVA 0x2d790 (mfc140u): m_bContextMenu = bEnable.
extern "C" void MS_ABI impl__EnableShellContextMenu_CMFCShellTreeCtrl__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    L(pThis)->m_bContextMenu = bEnable;
}

// Symbol: ?SetFlags@CMFCShellTreeCtrl@@QEAAXKH@Z
// Transcribed from retail entry RVA 0x12e6f0 (mfc140u):
//     m_dwFlags = dwFlags;
//     if (bRefresh && m_hWnd != NULL) Refresh();                    // 0x12d1a0
extern "C" void MS_ABI impl__SetFlags_CMFCShellTreeCtrl__QEAAXKH_Z(void* pThis, unsigned long dwFlags, int bRefresh) {
    if (pThis == nullptr) return;
    L(pThis)->m_dwFlags = static_cast<DWORD>(dwFlags);
    if (bRefresh && HWndOf(pThis) != nullptr) {
        impl__Refresh_CMFCShellTreeCtrl__QEAAXXZ(pThis);
    }
}

// Symbol: ?SetRelatedList@CMFCShellTreeCtrl@@QEAAXPEAVCMFCShellListCtrl@@@Z
// Transcribed from retail entry RVA 0x12d110 (mfc140u):
//     m_hwndRelatedList = (pShellList == NULL) ? NULL : pShellList->m_hWnd;   // +0x40
//     if (pShellList != NULL) pShellList->m_hwndRelatedTree = m_hWnd;         // list +0x260
extern "C" void MS_ABI impl__SetRelatedList_CMFCShellTreeCtrl__QEAAXPEAVCMFCShellListCtrl___Z(void* pThis, void* pShellList) {
    if (pThis == nullptr) return;
    L(pThis)->m_hwndRelatedList = (pShellList == nullptr) ? nullptr : HWndOf(pShellList);
    if (pShellList != nullptr) {
        *reinterpret_cast<HWND*>(static_cast<unsigned char*>(pShellList) + kListRelatedTreeHwndOffset) = HWndOf(pThis);
    }
}

// Symbol: ?GetRelatedList@CMFCShellTreeCtrl@@QEBAPEAVCMFCShellListCtrl@@XZ
// Transcribed from retail entry RVA 0x12d140 (mfc140u):
//     if (m_hwndRelatedList == NULL || !IsWindow(m_hwndRelatedList)) return NULL;   // IAT 0x1802c7138 IsWindow
//     CWnd* p = CWnd::FromHandlePermanent(m_hwndRelatedList);       // 0x28adc0
//     if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCShellListCtrl))) return NULL;  // 0x234cf0, 0x18030eb88
//     return (CMFCShellListCtrl*)p;
extern "C" void* MS_ABI impl__GetRelatedList_CMFCShellTreeCtrl__QEBAPEAVCMFCShellListCtrl__XZ(const void* pThis) {
    if (pThis == nullptr) return nullptr;
    const ShellTreeCtrlLayout* s = L(pThis);
    if (s->m_hwndRelatedList == nullptr || !::IsWindow(s->m_hwndRelatedList)) return nullptr;
    CWnd* p = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(s->m_hwndRelatedList);
    if (p == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            reinterpret_cast<const CObject*>(p),
            impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return p;
}

// Symbol: ?Refresh@CMFCShellTreeCtrl@@QEAAXXZ
// Transcribed from retail entry RVA 0x12d1a0 (mfc140u):
//     DeleteAllItems();                                             // TVM_DELETEITEM, 0, TVI_ROOT
//     GetRootItems();                                               // 0x12d1f0
//     SetScrollTime(100);                                           // TVM_SETSCROLLTIME (0x1121), 100, 0 (tail-jump)
extern "C" void MS_ABI impl__Refresh_CMFCShellTreeCtrl__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    const HWND hWnd = HWndOf(pThis);
    ::SendMessage(hWnd, TVM_DELETEITEM, 0, reinterpret_cast<LPARAM>(TVI_ROOT));
    impl__GetRootItems_CMFCShellTreeCtrl__IEAAHXZ(pThis);
    ::SendMessage(hWnd, TVM_SETSCROLLTIME, 100, 0);
}

// Symbol: ?GetRootItems@CMFCShellTreeCtrl@@IEAAHXZ
// Transcribed from retail entry RVA 0x12d1f0 (mfc140u):
//     ENSURE(afxShellManager != NULL);
//     LPITEMIDLIST pidl;
//     if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl))) return FALSE;   // delay-load 0x1803e9188
//     IShellFolder* psfDesktop;
//     if (FAILED(SHGetDesktopFolder(&psfDesktop))) return FALSE;    // delay-load 0x1803e9198
//     AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)GlobalAlloc(GPTR, sizeof(*pItem));   // IAT GlobalAlloc, 0x40/0x18
//     ENSURE(pItem != NULL);
//     pItem->pidlRel = pidl;
//     pItem->pidlFQ = afxShellManager->CopyItem(pidl);              // 0x12cdc0
//     pItem->pParentFolder = NULL;
//     CString strItem = OnGetItemText(pItem);                       // vslot 91, devirtualized (1)
//     TVITEM tvItem;   (hItem/state/stateMask/cchTextMax left uninitialised in retail; zeroed here)
//     tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;   // 0x67
//     tvItem.pszText = strItem.GetBuffer();                         // deviation (4): GetString()
//     tvItem.iImage = OnGetItemIcon(pItem, FALSE);                  // vslot 92, devirtualized (1)
//     tvItem.iSelectedImage = OnGetItemIcon(pItem, TRUE);
//     tvItem.cChildren = 1;  tvItem.lParam = (LPARAM)pItem;
//     TVINSERTSTRUCT tvInsert;  tvInsert.item = tvItem;
//     tvInsert.hInsertAfter = TVI_LAST;  tvInsert.hParent = TVI_ROOT;
//     HTREEITEM hItem = InsertItem(&tvInsert);                      // TVM_INSERTITEMW
//     Expand(hItem, TVE_EXPAND);                                    // TVM_EXPAND, 2, hItem
//     psfDesktop->Release();
//     return TRUE;
// (pidl itself is not freed here -- it is owned by pItem and released by
// OnDeleteitem.)  The ENSURE on afxShellManager throws in retail; the thunk
// throws here too, with a FALSE return behind it in case it does not.
extern "C" int MS_ABI impl__GetRootItems_CMFCShellTreeCtrl__IEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    if (impl__afxShellManager__3PEAVCShellManager__EA == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    const HWND hWnd = HWndOf(pThis);
    LPITEMIDLIST pidl = nullptr;
    if (FAILED(::SHGetSpecialFolderLocation(nullptr, CSIDL_DESKTOP, &pidl))) return FALSE;
    IShellFolder* psfDesktop = nullptr;
    if (FAILED(::SHGetDesktopFolder(&psfDesktop))) return FALSE;
    ShellItemInfo* pItem = static_cast<ShellItemInfo*>(::GlobalAlloc(GPTR, sizeof(ShellItemInfo)));
    if (pItem == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    pItem->pidlRel = pidl;
    pItem->pidlFQ = impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(
        impl__afxShellManager__3PEAVCShellManager__EA, pidl);
    pItem->pParentFolder = nullptr;

    CString strItem;
    impl__OnGetItemText_CMFCShellTreeCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAU_AFX_SHELLITEMINFO___Z(
        pThis, &strItem, pItem);

    TVINSERTSTRUCTW tvInsert;
    std::memset(&tvInsert, 0, sizeof(tvInsert));
    tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;
    tvInsert.item.pszText = const_cast<wchar_t*>(strItem.GetString());
    tvInsert.item.iImage = impl__OnGetItemIcon_CMFCShellTreeCtrl__UEAAHPEAU_AFX_SHELLITEMINFO__H_Z(pThis, pItem, FALSE);
    tvInsert.item.iSelectedImage = impl__OnGetItemIcon_CMFCShellTreeCtrl__UEAAHPEAU_AFX_SHELLITEMINFO__H_Z(pThis, pItem, TRUE);
    tvInsert.item.cChildren = 1;
    tvInsert.item.lParam = reinterpret_cast<LPARAM>(pItem);
    tvInsert.hInsertAfter = TVI_LAST;
    tvInsert.hParent = TVI_ROOT;
    HTREEITEM hItem = reinterpret_cast<HTREEITEM>(::SendMessage(hWnd, TVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&tvInsert)));
    ::SendMessage(hWnd, TVM_EXPAND, TVE_EXPAND, reinterpret_cast<LPARAM>(hItem));
    psfDesktop->Release();
    return TRUE;
}

// Symbol: ?GetChildItems@CMFCShellTreeCtrl@@IEAAHPEAU_TREEITEM@@@Z
// Transcribed from retail entry RVA 0x12d3d0 (mfc140u):
//     CWaitCursor wait;
//     TVITEM tvItem = {};  tvItem.mask = TVIF_PARAM;  tvItem.hItem = hParentItem;
//     if (!GetItem(&tvItem)) return FALSE;                          // TVM_GETITEMW
//     SetRedraw(FALSE);                                             // WM_SETREDRAW
//     AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)tvItem.lParam;
//     ENSURE(pItem != NULL);
//     IShellFolder* psfFolder = NULL;
//     HRESULT hr;
//     if (pItem->pParentFolder == NULL) hr = SHGetDesktopFolder(&psfFolder);   // delay-load 0x1803e9198
//     else hr = pItem->pParentFolder->BindToObject(pItem->pidlRel, NULL,
//                   IID_IShellFolder /*0x1802d9858*/, (void**)&psfFolder);      // IShellFolder vslot 5
//     if (FAILED(hr)) { SetRedraw(TRUE); return FALSE; }
//     EnumObjects(hParentItem, psfFolder, pItem->pidlFQ);           // vslot 93, devirtualized (1)
//     TVSORTCB tvscb;  tvscb.hParent = hParentItem;
//     tvscb.lpfnCompare = CompareProc;  tvscb.lParam = 0;           // 0x12d830; NOTE lParam is 0, not the folder
//     SortChildrenCB(&tvscb);                                       // TVM_SORTCHILDRENCB
//     SetRedraw(TRUE);
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);   // 0x105
//     psfFolder->Release();
//     return TRUE;
extern "C" int MS_ABI impl__GetChildItems_CMFCShellTreeCtrl__IEAAHPEAU_TREEITEM___Z(void* pThis, HTREEITEM hParentItem) {
    if (pThis == nullptr) return FALSE;
    const HWND hWnd = HWndOf(pThis);
    WaitCursorScope wait;
    TVITEMW tvItem;
    std::memset(&tvItem, 0, sizeof(tvItem));
    tvItem.mask = TVIF_PARAM;
    tvItem.hItem = hParentItem;
    if (!::SendMessage(hWnd, TVM_GETITEMW, 0, reinterpret_cast<LPARAM>(&tvItem))) return FALSE;
    ::SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    ShellItemInfo* pItem = reinterpret_cast<ShellItemInfo*>(tvItem.lParam);
    if (pItem == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    IShellFolder* psfFolder = nullptr;
    HRESULT hr;
    if (pItem->pParentFolder == nullptr) {
        hr = ::SHGetDesktopFolder(&psfFolder);
    } else {
        hr = pItem->pParentFolder->BindToObject(pItem->pidlRel, nullptr, IID_IShellFolder,
                                                reinterpret_cast<void**>(&psfFolder));
    }
    if (FAILED(hr)) {
        ::SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
        return FALSE;
    }
    impl__EnumObjects_CMFCShellTreeCtrl__MEAAJPEAU_TREEITEM__PEAUIShellFolder__PEFAU_ITEMIDLIST___Z(pThis, hParentItem, psfFolder, pItem->pidlFQ);
    TVSORTCB tvscb;
    tvscb.hParent = hParentItem;
    tvscb.lpfnCompare = reinterpret_cast<PFNTVCOMPARE>(impl__CompareProc_CMFCShellTreeCtrl__KAH_J00_Z);
    tvscb.lParam = 0;
    ::SendMessage(hWnd, TVM_SORTCHILDRENCB, 0, reinterpret_cast<LPARAM>(&tvscb));
    ::SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    ::RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    psfFolder->Release();
    return TRUE;
}

// Symbol: ?EnumObjects@CMFCShellTreeCtrl@@MEAAJPEAU_TREEITEM@@PEAUIShellFolder@@PEFAU_ITEMIDLIST@@@Z
// Transcribed from retail entry RVA 0x12d570 (mfc140u):
//     LPENUMIDLIST pEnum = NULL;
//     HRESULT hr = pParentFolder->EnumObjects(NULL, m_dwFlags, &pEnum);   // IShellFolder vslot 4
//     if (FAILED(hr) || pEnum == NULL) return hr;
//     LPITEMIDLIST pidlTemp;  ULONG fetched = 1;
//     while (SUCCEEDED(pEnum->Next(1, &pidlTemp, &fetched))) {      // IEnumIDList vslot 3 (jns)
//         if (fetched == 0) break;
//         TVITEM tvItem = {};
//         tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;   // 0x67
//         pParentFolder->AddRef();
//         AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)GlobalAlloc(GPTR, sizeof(*pItem));
//         ENSURE(pItem != NULL);
//         pItem->pidlRel = pidlTemp;
//         pItem->pidlFQ = afxShellManager->ConcatenateItem(pidlParent, pidlTemp);   // 0x12cd00
//         pItem->pParentFolder = pParentFolder;
//         tvItem.lParam = (LPARAM)pItem;
//         CString strItem = OnGetItemText(pItem);                   // vslot 91, devirtualized (1)
//         tvItem.pszText = strItem.GetBuffer();                     // deviation (4): GetString()
//         tvItem.iImage = OnGetItemIcon(pItem, FALSE);              // vslot 92, devirtualized (1)
//         tvItem.iSelectedImage = OnGetItemIcon(pItem, TRUE);
//         ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_FILESYSANCESTOR
//                       | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME;  // 0xb00fc010
//         pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &ulAttrs);   // vslot 9
//         tvItem.cChildren = ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FILESYSANCESTOR);   // & 0x90000000
//         if (ulAttrs & SFGAO_SHARE) { tvItem.mask |= TVIF_STATE; tvItem.stateMask |= TVIS_OVERLAYMASK;
//                                      tvItem.state |= INDEXTOOVERLAYMASK(1); }
//         TVINSERTSTRUCT tvInsert;  tvInsert.item = tvItem;
//         tvInsert.hInsertAfter = TVI_LAST;  tvInsert.hParent = hParentItem;
//         InsertItem(&tvInsert);                                    // TVM_INSERTITEMW
//         fetched = 0;
//     }
//     pEnum->Release();
//     return S_OK;
// Retail dereferences pParentFolder without a NULL test (the first
// instruction after the prologue is `mov (%r8),%rax`); the E_FAIL return for
// a NULL pParentFolder below is an added guard.
extern "C" long MS_ABI impl__EnumObjects_CMFCShellTreeCtrl__MEAAJPEAU_TREEITEM__PEAUIShellFolder__PEFAU_ITEMIDLIST___Z(void* pThis, HTREEITEM hParentItem, IShellFolder* pParentFolder, LPITEMIDLIST pidlParent) {
    if (pThis == nullptr || pParentFolder == nullptr) return E_FAIL;
    ShellTreeCtrlLayout* s = L(pThis);
    const HWND hWnd = HWndOf(pThis);
    IEnumIDList* pEnum = nullptr;
    const HRESULT hr = pParentFolder->EnumObjects(nullptr, s->m_dwFlags, &pEnum);
    if (FAILED(hr) || pEnum == nullptr) return hr;

    LPITEMIDLIST pidlTemp = nullptr;
    ULONG fetched = 1;
    while (SUCCEEDED(pEnum->Next(1, &pidlTemp, &fetched))) {
        if (fetched == 0) break;
        TVINSERTSTRUCTW tvInsert;
        std::memset(&tvInsert, 0, sizeof(tvInsert));
        tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;
        pParentFolder->AddRef();
        ShellItemInfo* pItem = static_cast<ShellItemInfo*>(::GlobalAlloc(GPTR, sizeof(ShellItemInfo)));
        if (pItem == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return E_FAIL;
        }
        pItem->pidlRel = pidlTemp;
        pItem->pidlFQ = impl__ConcatenateItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2_0_Z(
            impl__afxShellManager__3PEAVCShellManager__EA, pidlParent, pidlTemp);
        pItem->pParentFolder = pParentFolder;
        tvInsert.item.lParam = reinterpret_cast<LPARAM>(pItem);

        CString strItem;
        impl__OnGetItemText_CMFCShellTreeCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAU_AFX_SHELLITEMINFO___Z(
            pThis, &strItem, pItem);
        tvInsert.item.pszText = const_cast<wchar_t*>(strItem.GetString());
        tvInsert.item.iImage = impl__OnGetItemIcon_CMFCShellTreeCtrl__UEAAHPEAU_AFX_SHELLITEMINFO__H_Z(pThis, pItem, FALSE);
        tvInsert.item.iSelectedImage = impl__OnGetItemIcon_CMFCShellTreeCtrl__UEAAHPEAU_AFX_SHELLITEMINFO__H_Z(pThis, pItem, TRUE);

        ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_FILESYSANCESTOR | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME;
        LPCITEMIDLIST pidlAttr = pidlTemp;
        pParentFolder->GetAttributesOf(1, &pidlAttr, &ulAttrs);
        tvInsert.item.cChildren = static_cast<int>(ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FILESYSANCESTOR));
        if (ulAttrs & SFGAO_SHARE) {
            tvInsert.item.mask |= TVIF_STATE;
            tvInsert.item.stateMask |= TVIS_OVERLAYMASK;
            tvInsert.item.state |= INDEXTOOVERLAYMASK(1);
        }
        tvInsert.hInsertAfter = TVI_LAST;
        tvInsert.hParent = hParentItem;
        ::SendMessage(hWnd, TVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&tvInsert));
        fetched = 0;
    }
    pEnum->Release();
    return S_OK;
}

// Symbol: ?CompareProc@CMFCShellTreeCtrl@@KAH_J00@Z
// Transcribed from retail entry RVA 0x12d830 (mfc140u), the TVSORTCB callback
// GetChildItems installs:
//     AFX_SHELLITEMINFO* p1 = (AFX_SHELLITEMINFO*)lParam1, *p2 = ...lParam2;
//     HRESULT hr = p1->pParentFolder->CompareIDs(0, p1->pidlRel, p2->pidlRel);   // IShellFolder vslot 7
//     return FAILED(hr) ? 0 : (short)LOWORD(hr);                     // cwtl
// (lParamSort is not read -- GetChildItems passes 0.)  Retail dereferences
// lParam1/lParam2/pParentFolder without a NULL test; guards returning 0 are
// added.
extern "C" int MS_ABI impl__CompareProc_CMFCShellTreeCtrl__KAH_J00_Z(__int64 lParam1, __int64 lParam2, __int64 /*lParamSort*/) {
    const ShellItemInfo* p1 = reinterpret_cast<const ShellItemInfo*>(lParam1);
    const ShellItemInfo* p2 = reinterpret_cast<const ShellItemInfo*>(lParam2);
    if (p1 == nullptr || p2 == nullptr || p1->pParentFolder == nullptr) return 0;
    const HRESULT hr = p1->pParentFolder->CompareIDs(0, p1->pidlRel, p2->pidlRel);
    if (FAILED(hr)) return 0;
    return static_cast<short>(LOWORD(hr));
}

// Symbol: ?OnGetItemText@CMFCShellTreeCtrl@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAU_AFX_SHELLITEMINFO@@@Z
// Transcribed from retail entry RVA 0x12dc10 (mfc140u).  The CString is
// returned through the hidden slot in RDX (this in RCX, pItem in R8), the
// convention this tree uses for every CString-by-value export.
//     ENSURE(pItem != NULL);
//     SHFILEINFO sfi;
//     if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))   // 0x208, delay-load 0x1803e91a8
//         return sfi.szDisplayName;
//     return _T("???");                                             // 0x180341e10
// (this is not read.)
extern "C" CString* MS_ABI impl__OnGetItemText_CMFCShellTreeCtrl__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAU_AFX_SHELLITEMINFO___Z(void* /*pThis*/, CString* pRet, ShellItemInfo* pItem) {
    if (pRet == nullptr) return nullptr;
    if (pItem == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return new (pRet) CString();
    }
    SHFILEINFOW sfi;
    if (::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pItem->pidlFQ), 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME)) {
        return new (pRet) CString(sfi.szDisplayName);
    }
    return new (pRet) CString(L"???");
}

// Symbol: ?OnGetItemIcon@CMFCShellTreeCtrl@@UEAAHPEAU_AFX_SHELLITEMINFO@@H@Z
// Transcribed from retail entry RVA 0x12dca0 (mfc140u):
//     ENSURE(pItem != NULL);
//     SHFILEINFO sfi;
//     UINT flags = bSelected ? SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON      // 0x400b
//                            : SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY;  // 0xc009
//     return SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), flags) ? sfi.iIcon : -1;
// (this is not read.)
extern "C" int MS_ABI impl__OnGetItemIcon_CMFCShellTreeCtrl__UEAAHPEAU_AFX_SHELLITEMINFO__H_Z(void* /*pThis*/, ShellItemInfo* pItem, int bSelected) {
    if (pItem == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    SHFILEINFOW sfi;
    const UINT flags = bSelected
        ? (SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON)
        : (SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY);
    if (::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pItem->pidlFQ), 0, &sfi, sizeof(sfi), flags)) {
        return sfi.iIcon;
    }
    return -1;
}

// Symbol: ?GetItemPath@CMFCShellTreeCtrl@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAU_TREEITEM@@@Z
// Transcribed from retail entry RVA 0x12e310 (mfc140u):
//     strPath.Empty();
//     if (hItem == NULL) { hItem = GetSelectedItem(); if (hItem == NULL) return FALSE; }   // TVGN_CARET
//     AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)GetItemData(hItem);   // 0x295b40
//     if (pItem == NULL || pItem->pidlFQ == NULL || pItem->pidlRel == NULL) return FALSE;
//     IShellFolder* psfFolder = NULL;
//     HRESULT hr = (pItem->pParentFolder == NULL)
//         ? SHGetDesktopFolder(&psfFolder)
//         : pItem->pParentFolder->BindToObject(pItem->pidlRel, NULL, IID_IShellFolder, (void**)&psfFolder);
//     if (FAILED(hr)) return FALSE;
//     BOOL bRes = FALSE;
//     TCHAR szPath[MAX_PATH];
//     if (SHGetPathFromIDList(pItem->pidlFQ, szPath)) { strPath = szPath; bRes = TRUE; }   // delay-load 0x1803e9190
//     if (psfFolder != NULL) psfFolder->Release();
//     return bRes;
// (psfFolder is bound and released without being used otherwise -- retail.)
extern "C" int MS_ABI impl__GetItemPath_CMFCShellTreeCtrl__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAU_TREEITEM___Z(const void* pThis, CString* strPath, HTREEITEM hItem) {
    if (pThis == nullptr || strPath == nullptr) return FALSE;
    strPath->Empty();
    if (hItem == nullptr) {
        hItem = TreeNext(HWndOf(pThis), TVGN_CARET, nullptr);
        if (hItem == nullptr) return FALSE;
    }
    const ShellItemInfo* pItem = reinterpret_cast<const ShellItemInfo*>(
        impl__GetItemData_CTreeCtrl__QEBA_KPEAU_TREEITEM___Z(static_cast<const CTreeCtrl*>(pThis), hItem));
    if (pItem == nullptr || pItem->pidlFQ == nullptr || pItem->pidlRel == nullptr) return FALSE;
    IShellFolder* psfFolder = nullptr;
    HRESULT hr;
    if (pItem->pParentFolder == nullptr) {
        hr = ::SHGetDesktopFolder(&psfFolder);
    } else {
        hr = pItem->pParentFolder->BindToObject(pItem->pidlRel, nullptr, IID_IShellFolder,
                                                reinterpret_cast<void**>(&psfFolder));
    }
    if (FAILED(hr)) return FALSE;
    BOOL bRes = FALSE;
    wchar_t szPath[MAX_PATH];
    if (::SHGetPathFromIDListW(pItem->pidlFQ, szPath)) {
        *strPath = szPath;
        bRes = TRUE;
    }
    if (psfFolder != nullptr) psfFolder->Release();
    return bRes;
}

// Symbol: ?InitTree@CMFCShellTreeCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x12e640 (mfc140u):
//     TCHAR szDir[MAX_PATH];
//     if (GetWindowsDirectory(szDir, MAX_PATH)) {                   // IAT 0x1802c66e0 GetWindowsDirectoryW
//         SHFILEINFO sfi;
//         HIMAGELIST h = (HIMAGELIST)SHGetFileInfo(szDir, 0, &sfi, sizeof(sfi),
//                            SHGFI_SYSICONINDEX | SHGFI_SMALLICON);   // 0x4001
//         SetImageList(CImageList::FromHandle(h), TVSIL_NORMAL);   // TVM_SETIMAGELIST -- deviation (3)
//     }
//     Refresh();                                                    // 0x12d1a0
extern "C" void MS_ABI impl__InitTree_CMFCShellTreeCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    wchar_t szDir[MAX_PATH];
    if (::GetWindowsDirectoryW(szDir, MAX_PATH)) {
        SHFILEINFOW sfi;
        const DWORD_PTR hImageList = ::SHGetFileInfoW(szDir, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
        ::SendMessage(HWndOf(pThis), TVM_SETIMAGELIST, TVSIL_NORMAL, static_cast<LPARAM>(hImageList));
    }
    impl__Refresh_CMFCShellTreeCtrl__QEAAXXZ(pThis);
}

// Symbol: ?OnCreate@CMFCShellTreeCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from retail entry RVA 0x12d0d0 (mfc140u):
//     if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;          // inlined as CWnd::Default(), 0x28ac80
//     if (afxShellManager == NULL) return -1;
//     InitTree();                                                   // 0x12e640
//     return 0;
// (lpCreateStruct is not read.)
extern "C" int MS_ABI impl__OnCreate_CMFCShellTreeCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* /*lpCreateStruct*/) {
    if (pThis == nullptr) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis))) == -1) return -1;
    if (impl__afxShellManager__3PEAVCShellManager__EA == nullptr) return -1;
    impl__InitTree_CMFCShellTreeCtrl__IEAAXXZ(pThis);
    return 0;
}

// Symbol: ?PreSubclassWindow@CMFCShellTreeCtrl@@MEAAXXZ
// Transcribed from retail entry RVA 0x12e610 (mfc140u):
//     _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();        // 0x1332a0
//     if (pThreadState->m_pWndInit == NULL) InitTree();             // +0x28; 0x12e640
// No base-class PreSubclassWindow call is made (retail makes none).  OpenMFC
// keeps the same member as _AFX_THREAD_STATE::pWndInit (read through
// AfxThreadStateMirror above).  Retail dereferences the thread state
// unconditionally; a NULL guard is added.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCShellTreeCtrl__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    const AfxThreadStateMirror* pThreadState =
        reinterpret_cast<const AfxThreadStateMirror*>(impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ());
    if (pThreadState == nullptr || pThreadState->pWndInit == nullptr) {
        impl__InitTree_CMFCShellTreeCtrl__IEAAXXZ(pThis);
    }
}

// Symbol: ?OnDestroy@CMFCShellTreeCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x12e590 (mfc140u):
//     CMFCShellListCtrl* pList = GetRelatedList();                  // 0x12d140
//     if (pList != NULL) pList->m_hwndRelatedTree = NULL;           // list +0x260
//     CTreeCtrl::OnDestroy();                                       // 0x295d80 (tail-jump)
extern "C" void MS_ABI impl__OnDestroy_CMFCShellTreeCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pList = impl__GetRelatedList_CMFCShellTreeCtrl__QEBAPEAVCMFCShellListCtrl__XZ(pThis);
    if (pList != nullptr) {
        *reinterpret_cast<HWND*>(static_cast<unsigned char*>(pList) + kListRelatedTreeHwndOffset) = nullptr;
    }
    impl__OnDestroy_CTreeCtrl__QEAAXXZ(static_cast<CTreeCtrl*>(pThis));
}

// Symbol: ?OnDeleteitem@CMFCShellTreeCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0x12de40 (mfc140u):
//     ENSURE(pNMHDR != NULL);
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)((NMTREEVIEW*)pNMHDR)->itemOld.lParam;   // +0x50 (not NULL-tested)
//     if (pItem->pidlFQ != NULL)  m_pMalloc->Free(pItem->pidlFQ);   // -> CoTaskMemFree
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // re-read, re-tested
//     if (pItem->pidlRel != NULL) m_pMalloc->Free(pItem->pidlRel);
//     if (pItem->pParentFolder != NULL) pItem->pParentFolder->Release();
//     GlobalFree(pItem);                                            // IAT GlobalFree
//     *pResult = 0;
// Retail dereferences pItem without a NULL test; a guard that skips to
// `*pResult = 0` is added.
extern "C" void MS_ABI impl__OnDeleteitem_CMFCShellTreeCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    if (pThis == nullptr) return;
    if (pNMHDR == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    EnsureShellAllocator();
    ShellItemInfo* pItem = reinterpret_cast<ShellItemInfo*>(reinterpret_cast<NMTREEVIEWW*>(pNMHDR)->itemOld.lParam);
    if (pItem != nullptr) {
        if (pItem->pidlFQ != nullptr) {
            ::CoTaskMemFree(pItem->pidlFQ);
        }
        EnsureShellAllocator();
        if (pItem->pidlRel != nullptr) {
            ::CoTaskMemFree(pItem->pidlRel);
        }
        if (pItem->pParentFolder != nullptr) {
            pItem->pParentFolder->Release();
        }
        ::GlobalFree(pItem);
    }
    if (pResult != nullptr) *pResult = 0;
}

// Symbol: ?OnItemexpanding@CMFCShellTreeCtrl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
// Transcribed from retail entry RVA 0x12dd20 (mfc140u):
//     ENSURE(pNMHDR != NULL);
//     NMTREEVIEW* pnmtv = (NMTREEVIEW*)pNMHDR;
//     HTREEITEM hItem = pnmtv->itemNew.hItem;                       // +0x60
//     ENSURE(hItem != NULL);
//     switch (pnmtv->action) {                                      // +0x18
//     case TVE_EXPAND:                                              // 2
//         GetChildItems(hItem);                                     // 0x12d3d0
//         if (GetChildItem(hItem) == NULL) {                        // TVGN_CHILD
//             TVITEM tvi = {};  tvi.mask = TVIF_CHILDREN;  tvi.hItem = hItem;  (cChildren = 0)
//             SetItem(&tvi);                                        // TVM_SETITEMW
//         }
//         break;
//     case TVE_COLLAPSE: {                                          // 1
//         HTREEITEM hSel = GetSelectedItem();                       // TVGN_CARET
//         while (hSel != NULL) {
//             hSel = GetParentItem(hSel);                           // TVGN_PARENT
//             if (hSel == hItem) { SelectItem(hItem); break; }      // TVM_SELECTITEM, TVGN_CARET
//         }
//         Expand(hItem, TVE_COLLAPSE | TVE_COLLAPSERESET);          // TVM_EXPAND, 0x8001
//         break; }
//     }
//     *pResult = 0;
extern "C" void MS_ABI impl__OnItemexpanding_CMFCShellTreeCtrl__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, __int64* pResult) {
    if (pThis == nullptr) return;
    if (pNMHDR == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    NMTREEVIEWW* pnmtv = reinterpret_cast<NMTREEVIEWW*>(pNMHDR);
    HTREEITEM hItem = pnmtv->itemNew.hItem;
    if (hItem == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    const HWND hWnd = HWndOf(pThis);
    switch (pnmtv->action) {
    case TVE_EXPAND:
        impl__GetChildItems_CMFCShellTreeCtrl__IEAAHPEAU_TREEITEM___Z(pThis, hItem);
        if (TreeNext(hWnd, TVGN_CHILD, hItem) == nullptr) {
            TVITEMW tvi;
            std::memset(&tvi, 0, sizeof(tvi));
            tvi.mask = TVIF_CHILDREN;
            tvi.hItem = hItem;
            ::SendMessage(hWnd, TVM_SETITEMW, 0, reinterpret_cast<LPARAM>(&tvi));
        }
        break;
    case TVE_COLLAPSE: {
        HTREEITEM hSel = TreeNext(hWnd, TVGN_CARET, nullptr);
        while (hSel != nullptr) {
            hSel = TreeNext(hWnd, TVGN_PARENT, hSel);
            if (hSel == hItem) {
                ::SendMessage(hWnd, TVM_SELECTITEM, TVGN_CARET, reinterpret_cast<LPARAM>(hItem));
                break;
            }
        }
        ::SendMessage(hWnd, TVM_EXPAND, TVE_COLLAPSE | TVE_COLLAPSERESET, reinterpret_cast<LPARAM>(hItem));
        break;
    }
    default:
        break;
    }
    if (pResult != nullptr) *pResult = 0;
}

// Symbol: ?OnChildNotify@CMFCShellTreeCtrl@@UEAAHI_K_JPEA_J@Z
// Transcribed from retail entry RVA 0x12e4a0 (mfc140u).  Retail is a chain
// of tests that each jump to the base-call block at 0x12e555 (the call to
// CWnd::OnChildNotify itself is the instruction at 0x12e56d) on
// failure; written here in that early-out shape:
//     if (message != WM_NOTIFY || m_bNoNotify) goto base;          // 0x4e; +0xec
//     ENSURE(lParam != NULL);                                       // 0x227720 AfxThrowInvalidArgException
//     if (((NMHDR*)lParam)->code != TVN_SELCHANGED) goto base;     // 0xfffffe3d (TVN_SELCHANGEDW)
//     CMFCShellListCtrl* pList = GetRelatedList();                  // 0x12d140
//     if (pList == NULL || GetSelectedItem() == NULL) goto base;    // TVM_GETNEXTITEM/TVGN_CARET, IAT 0x1802c7120 SendMessageW
//     AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)GetItemData(GetSelectedItem());   // 0x295b40 (GetSelectedItem sent again)
//     pList->m_bNoNotify = TRUE;                                    // list +0x270
//     pList->DisplayFolder(pItem);                                  // list vslot 100 (+0x320) via the CFG dispatcher 0x2c7b30; devirtualized (1)
//     pList->m_bNoNotify = FALSE;
//     return TRUE;
// base:
//     return CWnd::OnChildNotify(message, wParam, lParam, pLResult);   // 0x28f0d0
// The nested-if body below is the same control flow.
extern "C" int MS_ABI impl__OnChildNotify_CMFCShellTreeCtrl__UEAAHI_K_JPEA_J_Z(void* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam, __int64* pLResult) {
    if (pThis == nullptr) return FALSE;
    if (message == WM_NOTIFY && !L(pThis)->m_bNoNotify) {
        if (lParam == 0) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;
        }
        if (reinterpret_cast<const NMHDR*>(lParam)->code == static_cast<UINT>(TVN_SELCHANGEDW)) {
            void* pList = impl__GetRelatedList_CMFCShellTreeCtrl__QEBAPEAVCMFCShellListCtrl__XZ(pThis);
            if (pList != nullptr) {
                const HWND hWnd = HWndOf(pThis);
                if (TreeNext(hWnd, TVGN_CARET, nullptr) != nullptr) {
                    ShellItemInfo* pItem = reinterpret_cast<ShellItemInfo*>(
                        impl__GetItemData_CTreeCtrl__QEBA_KPEAU_TREEITEM___Z(static_cast<const CTreeCtrl*>(pThis),
                                                                             TreeNext(hWnd, TVGN_CARET, nullptr)));
                    BOOL* pListNoNotify = reinterpret_cast<BOOL*>(static_cast<unsigned char*>(pList) + kListNoNotifyOffset);
                    *pListNoNotify = TRUE;
                    impl__DisplayFolder_CMFCShellListCtrl__UEAAJPEAU_AFX_SHELLITEMINFO___Z(pList, pItem);
                    *pListNoNotify = FALSE;
                    return TRUE;
                }
            }
        }
    }
    return impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(static_cast<CWnd*>(pThis), message, wParam, lParam, pLResult);
}

// Symbol: ?OnContextMenu@CMFCShellTreeCtrl@@IEAAXPEAVCWnd@@VCPoint@@@Z
// Transcribed from retail entry RVA 0x12dee0 (mfc140u):
//     if (m_bContextMenu) OnShowContextMenu(point);                 // 0x12d860 (tail-jump)
//     else Default();                                               // 0x28ac80 (tail-jump)
// (pWnd is not read.)
extern "C" void MS_ABI impl__OnContextMenu_CMFCShellTreeCtrl__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* /*pWnd*/, long long pointPacked) {
    if (pThis == nullptr) return;
    if (L(pThis)->m_bContextMenu) {
        impl__OnShowContextMenu_CMFCShellTreeCtrl__IEAAXVCPoint___Z(pThis, pointPacked);
    } else {
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    }
}

// Symbol: ?OnRButtonDown@CMFCShellTreeCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x12e450 (mfc140u):
//     SetFocus();                                                   // 0x2a9b60
//     UINT flags;
//     HTREEITEM hItem = HitTest(point, &flags);                     // 0x295d40
//     SelectItem(hItem);                                            // TVM_SELECTITEM, TVGN_CARET (tail-jump)
// (nFlags is not read.)
extern "C" void MS_ABI impl__OnRButtonDown_CMFCShellTreeCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int /*nFlags*/, long long pointPacked) {
    if (pThis == nullptr) return;
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));
    const POINT pt = PointFromPacked(pointPacked);
    UINT flags = 0;
    HTREEITEM hItem = impl__HitTest_CTreeCtrl__QEBAPEAU_TREEITEM__VCPoint__PEAI_Z(
        static_cast<const CTreeCtrl*>(pThis), CPoint(pt.x, pt.y), &flags);
    ::SendMessage(HWndOf(pThis), TVM_SELECTITEM, TVGN_CARET, reinterpret_cast<LPARAM>(hItem));
}

// Symbol: ?OnShowContextMenu@CMFCShellTreeCtrl@@IEAAXVCPoint@@@Z
// Transcribed from retail entry RVA 0x12d860 (mfc140u):
//     if (m_pContextMenu2 != NULL) return;
//     if (!m_bContextMenu) { Default(); return; }                   // 0x28ac80
//     HTREEITEM hItem;
//     if (point.x == -1 && point.y == -1) {                         // keyboard
//         hItem = GetSelectedItem();                                // TVGN_CARET
//         if (hItem == NULL) return;
//         RECT rc;  *(HTREEITEM*)&rc = hItem;  rc.right = rc.bottom = 0;
//         if (SendMessage(TVM_GETITEMRECT, FALSE, &rc)) { point = CPoint(rc.left, rc.bottom + 1); ClientToScreen(&point); }
//     } else {
//         CPoint ptClient = point;  ScreenToClient(&ptClient);
//         UINT flags;  hItem = HitTest(ptClient, &flags);           // 0x295d40
//     }
//     if (hItem == NULL) return;
//     TVITEM tvi = {};  tvi.mask = TVIF_PARAM;  tvi.hItem = hItem;
//     if (!GetItem(&tvi)) return;                                   // TVM_GETITEMW
//     AFX_SHELLITEMINFO* pInfo = (AFX_SHELLITEMINFO*)tvi.lParam;
//     if (pInfo == NULL) return;
//     IShellFolder* psfFolder = pInfo->pParentFolder;
//     if (psfFolder == NULL) ENSURE(SUCCEEDED(SHGetDesktopFolder(&psfFolder)));   // delay-load 0x1803e9198
//     else psfFolder->AddRef();
//     if (psfFolder == NULL) return;
//     HWND hwndParent = CWnd::FromHandle(GetParent(m_hWnd)) ? ->m_hWnd : NULL;
//     IContextMenu* pcm = NULL;
//     if (SUCCEEDED(psfFolder->GetUIObjectOf(hwndParent, 1, (LPCITEMIDLIST*)&pInfo->pidlRel,
//                                            IID_IContextMenu /*0x1802d9848*/, NULL, (void**)&pcm))) {   // vslot 10
//         HMENU hMenu = CreatePopupMenu();
//         if (hMenu != NULL && SUCCEEDED(pcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {   // vslot 3
//             pcm->QueryInterface(IID_IContextMenu2 /*0x1802d9868*/, (void**)&m_pContextMenu2);   // result not tested
//             HWND hwnd = m_hWnd;
//             UINT idCmd = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD /*0x102*/,
//                                         point.x, point.y, 0, hwnd, NULL);
//             if (IsWindow(hwnd)) {
//                 if (m_pContextMenu2 != NULL) { m_pContextMenu2->Release(); m_pContextMenu2 = NULL; }
//                 if (idCmd != 0) {
//                     CWaitCursor wait;
//                     CMINVOKECOMMANDINFO ici = {};  ici.cbSize = sizeof(ici);
//                     ici.hwnd = hwndParent;  ici.lpVerb = MAKEINTRESOURCEA(idCmd - 1);  ici.nShow = SW_SHOWNORMAL;
//                     if (SUCCEEDED(pcm->InvokeCommand(&ici)) &&    // vslot 4
//                         CWnd::FromHandle(GetParent(m_hWnd)) != NULL)
//                         ::SendMessage(CWnd::FromHandle(GetParent(m_hWnd))->m_hWnd,
//                                       AFX_WM_ON_AFTER_SHELL_COMMAND, idCmd, 0);
//                     SetFocus();                                   // 0x2a9b60
//                 }
//             }
//         }
//         if (pcm != NULL) { pcm->Release(); pcm = NULL; }
//     }
//     psfFolder->Release();
// (hMenu is not destroyed -- retail has no DestroyMenu call in this body.)
extern "C" void MS_ABI impl__OnShowContextMenu_CMFCShellTreeCtrl__IEAAXVCPoint___Z(void* pThis, long long pointPacked) {
    if (pThis == nullptr) return;
    if (impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA != nullptr) return;
    if (!L(pThis)->m_bContextMenu) {
        impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
        return;
    }
    const HWND hWnd = HWndOf(pThis);
    POINT point = PointFromPacked(pointPacked);
    HTREEITEM hItem;
    if (point.x == -1 && point.y == -1) {
        hItem = TreeNext(hWnd, TVGN_CARET, nullptr);
        if (hItem == nullptr) return;
        RECT rc;
        std::memset(&rc, 0, sizeof(rc));
        *reinterpret_cast<HTREEITEM*>(&rc) = hItem;
        if (::SendMessage(hWnd, TVM_GETITEMRECT, FALSE, reinterpret_cast<LPARAM>(&rc))) {
            point.x = rc.left;
            point.y = rc.bottom + 1;
            ::ClientToScreen(hWnd, &point);
        }
    } else {
        POINT ptClient = point;
        ::ScreenToClient(hWnd, &ptClient);
        UINT flags = 0;
        hItem = impl__HitTest_CTreeCtrl__QEBAPEAU_TREEITEM__VCPoint__PEAI_Z(
            static_cast<const CTreeCtrl*>(pThis), CPoint(ptClient.x, ptClient.y), &flags);
    }
    if (hItem == nullptr) return;

    TVITEMW tvi;
    std::memset(&tvi, 0, sizeof(tvi));
    tvi.mask = TVIF_PARAM;
    tvi.hItem = hItem;
    if (!::SendMessage(hWnd, TVM_GETITEMW, 0, reinterpret_cast<LPARAM>(&tvi))) return;
    ShellItemInfo* pInfo = reinterpret_cast<ShellItemInfo*>(tvi.lParam);
    if (pInfo == nullptr) return;

    IShellFolder* psfFolder = pInfo->pParentFolder;
    if (psfFolder == nullptr) {
        if (FAILED(::SHGetDesktopFolder(&psfFolder))) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
    } else {
        psfFolder->AddRef();
    }
    if (psfFolder == nullptr) return;

    const HWND hwndParent = ParentHWndViaFromHandle(pThis);
    IContextMenu* pcm = nullptr;
    LPCITEMIDLIST pidlRel = pInfo->pidlRel;
    if (SUCCEEDED(psfFolder->GetUIObjectOf(hwndParent, 1, &pidlRel, IID_IContextMenu, nullptr,
                                           reinterpret_cast<void**>(&pcm)))) {
        HMENU hMenu = ::CreatePopupMenu();
        if (hMenu != nullptr && SUCCEEDED(pcm->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {
            pcm->QueryInterface(IID_IContextMenu2, &impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA);
            const UINT idCmd = static_cast<UINT>(::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
                                                                  point.x, point.y, 0, hWnd, nullptr));
            if (::IsWindow(hWnd)) {
                if (impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA != nullptr) {
                    static_cast<IContextMenu2*>(impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA)->Release();
                    impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA = nullptr;
                }
                if (idCmd != 0) {
                    WaitCursorScope wait;
                    CMINVOKECOMMANDINFO ici;
                    std::memset(&ici, 0, sizeof(ici));
                    ici.cbSize = sizeof(ici);
                    ici.fMask = 0;
                    ici.hwnd = hwndParent;
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
                    impl__SetFocus_CWnd__QEAAPEAV1_XZ(static_cast<CWnd*>(pThis));
                }
            }
        }
        if (pcm != nullptr) {
            pcm->Release();
            pcm = nullptr;
        }
    }
    psfFolder->Release();
}

// Symbol: ?SelectPath@CMFCShellTreeCtrl@@QEAAHPEB_WH@Z
// Transcribed from retail entry RVA 0x12df00 (mfc140u):
//     ENSURE(lpszPath != NULL);
//     LPITEMIDLIST pidl;
//     if (FAILED(afxShellManager->ItemFromPath(lpszPath, pidl))) return FALSE;   // 0x12ce70
//     BOOL bRes = SelectPath(pidl, fExpandToShowChildren);          // 0x12df80
//     ENSURE(afxShellManager->m_pMalloc != NULL);                   // deviation (2)
//     if (pidl != NULL) m_pMalloc->Free(pidl);                      // -> CoTaskMemFree
//     return bRes;
extern "C" int MS_ABI impl__SelectPath_CMFCShellTreeCtrl__QEAAHPEB_WH_Z(void* pThis, const wchar_t* lpszPath, int fExpandToShowChildren) {
    if (pThis == nullptr) return FALSE;
    if (lpszPath == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    LPITEMIDLIST pidl = nullptr;
    if (FAILED(impl__ItemFromPath_CShellManager__QEAAJPEB_WAEAPEFAU_ITEMIDLIST___Z(
            impl__afxShellManager__3PEAVCShellManager__EA, lpszPath, &pidl))) {
        return FALSE;
    }
    const BOOL bRes = impl__SelectPath_CMFCShellTreeCtrl__QEAAHPEFBU_ITEMIDLIST__H_Z(pThis, pidl, fExpandToShowChildren);
    EnsureShellAllocator();
    if (pidl != nullptr) {
        ::CoTaskMemFree(pidl);
    }
    return bRes;
}

// Symbol: ?SelectPath@CMFCShellTreeCtrl@@QEAAHPEFBU_ITEMIDLIST@@H@Z
// Transcribed from retail entry RVA 0x12df80 (mfc140u):
//     if (lpidl == NULL) return FALSE;
//     HTREEITEM hTreeItem = GetRootItem();                          // TVGN_ROOT
//     SetRedraw(FALSE);
//     int nCount = 0;                                               // element count of lpidl (inline cb walk)
//     for (LPCITEMIDLIST p = lpidl; p->mkid.cb != 0; p = (LPCITEMIDLIST)((BYTE*)p + p->mkid.cb)) nCount++;
//     if (nCount != 0) {
//         CList<LPITEMIDLIST, LPITEMIDLIST> lstParents;             // vftable 0x18030ec20
//         lstParents.AddHead(afxShellManager->CopyItem(lpidl));      // 0x12cdc0
//         LPCITEMIDLIST pidlCurr = lpidl;  LPITEMIDLIST pidlParent;
//         while (afxShellManager->GetParentItem(pidlCurr, pidlParent) > 0) {   // 0x12cf30
//             lstParents.AddHead(pidlParent);  pidlCurr = pidlParent;
//         }
//         for (POSITION pos = lstParents.GetHeadPosition(); pos != NULL; ) {   // outermost parent first
//             LPITEMIDLIST pidl = lstParents.GetNext(pos);
//             if (hTreeItem != NULL) {
//                 if (GetChildItem(hTreeItem) == NULL) Expand(hTreeItem, TVE_EXPAND);   // TVGN_CHILD; TVM_EXPAND
//                 BOOL bFound = FALSE;
//                 for (HTREEITEM hChild = GetChildItem(hTreeItem); hChild != NULL && !bFound;
//                      hChild = GetNextSiblingItem(hChild)) {         // TVGN_NEXT
//                     AFX_SHELLITEMINFO* pItem = (AFX_SHELLITEMINFO*)GetItemData(hChild);   // 0x295b40
//                     if (pItem == NULL) continue;
//                     SHFILEINFO sfi1, sfi2;
//                     if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi1, sizeof(sfi1), SHGFI_PIDL | SHGFI_DISPLAYNAME) &&
//                         SHGetFileInfo((LPCTSTR)pidl, 0, &sfi2, sizeof(sfi2), SHGFI_PIDL | SHGFI_DISPLAYNAME) &&
//                         lstrcmpi(sfi1.szDisplayName, sfi2.szDisplayName) == 0) {   // IAT lstrcmpiW
//                         bFound = TRUE;  hTreeItem = hChild;
//                     }
//                 }
//                 if (!bFound) hTreeItem = NULL;
//             }
//             ENSURE(afxShellManager->m_pMalloc != NULL);           // deviation (2)
//             if (pidl != NULL) m_pMalloc->Free(pidl);              // -> CoTaskMemFree
//         }
//     }                                                             // (~lstParents, 0x12e838)
//     BOOL bRes = FALSE;
//     if (hTreeItem != NULL) {
//         m_bNoNotify = TRUE;
//         SelectItem(hTreeItem);                                    // TVM_SELECTITEM, TVGN_CARET
//         if (fExpandToShowChildren && GetChildItem(hTreeItem) == NULL) Expand(hTreeItem, TVE_EXPAND);
//         EnsureVisible(hTreeItem);                                 // TVM_ENSUREVISIBLE
//         m_bNoNotify = FALSE;
//         bRes = TRUE;
//     }
//     SetRedraw(TRUE);
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);   // 0x105
//     return bRes;
// Two deviations here:
//  * The CList is a CoTaskMemAlloc'd array of nCount + 1 slots (a parent
//    chain of an n-element PIDL has at most n - 1 members, plus the copy of
//    lpidl) filled in the same order and walked from its last entry, which
//    is the same head-first traversal; a chain that would overflow it stops
//    there.  If the array cannot be allocated the copy is freed and no walk
//    is made, so hTreeItem stays the root item.
//  * The parent loop.  Retail ?GetParentItem@CShellManager (0x12cf30) returns
//    the parent's element count (n-1 for an n-element PIDL) and -1 for an
//    empty one; for a ONE-element PIDL it returns 0 and hands back the
//    desktop PIDL from SHGetSpecialFolderLocation(CSIDL_DESKTOP), which is the
//    empty (terminator-only) PIDL, and which retail neither lists nor frees.
//    OpenMFC's thunk (core/app/CShellManager.cpp) returns TRUE/FALSE and hands
//    a one-element PIDL back as that same empty parent.  So the loop below
//    stops on a positive return whose parent is empty -- a no-op under the
//    retail return convention, and the retail stopping point under
//    OpenMFC's -- and frees that empty PIDL instead of leaking it.
extern "C" int MS_ABI impl__SelectPath_CMFCShellTreeCtrl__QEAAHPEFBU_ITEMIDLIST__H_Z(void* pThis, const ITEMIDLIST* lpidl, int fExpandToShowChildren) {
    if (pThis == nullptr) return FALSE;
    if (lpidl == nullptr) return FALSE;
    ShellTreeCtrlLayout* s = L(pThis);
    const HWND hWnd = HWndOf(pThis);
    HTREEITEM hTreeItem = TreeNext(hWnd, TVGN_ROOT, nullptr);
    ::SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

    int nCount = 0;
    for (const ITEMIDLIST* p = lpidl; p->mkid.cb != 0;
         p = reinterpret_cast<const ITEMIDLIST*>(reinterpret_cast<const BYTE*>(p) + p->mkid.cb)) {
        nCount++;
    }
    if (nCount != 0) {
        const size_t nSlots = static_cast<size_t>(nCount) + 1;
        LPITEMIDLIST* lstParents = static_cast<LPITEMIDLIST*>(::CoTaskMemAlloc(nSlots * sizeof(LPITEMIDLIST)));
        size_t nParents = 0;   // lstParents[nParents - 1] is the list head
        LPITEMIDLIST pidlCopy = impl__CopyItem_CShellManager__QEAAPEFAU_ITEMIDLIST__PEFBU2__Z(
            impl__afxShellManager__3PEAVCShellManager__EA, lpidl);
        if (lstParents == nullptr) {
            if (pidlCopy != nullptr) ::CoTaskMemFree(pidlCopy);
        } else {
            lstParents[nParents++] = pidlCopy;
            const ITEMIDLIST* pidlCurr = lpidl;
            while (nParents < nSlots) {
                LPITEMIDLIST pidlParent = nullptr;
                if (impl__GetParentItem_CShellManager__QEAAHPEFBU_ITEMIDLIST__AEAPEFAU2__Z(
                        impl__afxShellManager__3PEAVCShellManager__EA, pidlCurr, &pidlParent) <= 0) {
                    break;
                }
                if (pidlParent == nullptr || pidlParent->mkid.cb == 0) {   // see the note above
                    if (pidlParent != nullptr) ::CoTaskMemFree(pidlParent);
                    break;
                }
                lstParents[nParents++] = pidlParent;
                pidlCurr = pidlParent;
            }
        }
        for (size_t i = nParents; i-- > 0;) {
            LPITEMIDLIST pidl = lstParents[i];
            if (hTreeItem != nullptr) {
                if (TreeNext(hWnd, TVGN_CHILD, hTreeItem) == nullptr) {
                    ::SendMessage(hWnd, TVM_EXPAND, TVE_EXPAND, reinterpret_cast<LPARAM>(hTreeItem));
                }
                BOOL bFound = FALSE;
                for (HTREEITEM hChild = TreeNext(hWnd, TVGN_CHILD, hTreeItem); hChild != nullptr && !bFound;
                     hChild = TreeNext(hWnd, TVGN_NEXT, hChild)) {
                    const ShellItemInfo* pItem = reinterpret_cast<const ShellItemInfo*>(
                        impl__GetItemData_CTreeCtrl__QEBA_KPEAU_TREEITEM___Z(static_cast<const CTreeCtrl*>(pThis), hChild));
                    if (pItem == nullptr) continue;
                    SHFILEINFOW sfi1;
                    SHFILEINFOW sfi2;
                    if (::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pItem->pidlFQ), 0, &sfi1, sizeof(sfi1), SHGFI_PIDL | SHGFI_DISPLAYNAME) &&
                        ::SHGetFileInfoW(reinterpret_cast<LPCWSTR>(pidl), 0, &sfi2, sizeof(sfi2), SHGFI_PIDL | SHGFI_DISPLAYNAME) &&
                        ::lstrcmpiW(sfi1.szDisplayName, sfi2.szDisplayName) == 0) {
                        bFound = TRUE;
                        hTreeItem = hChild;
                    }
                }
                if (!bFound) hTreeItem = nullptr;
            }
            EnsureShellAllocator();
            if (pidl != nullptr) {
                ::CoTaskMemFree(pidl);
            }
        }
        if (lstParents != nullptr) ::CoTaskMemFree(lstParents);
    }

    BOOL bRes = FALSE;
    if (hTreeItem != nullptr) {
        s->m_bNoNotify = TRUE;
        ::SendMessage(hWnd, TVM_SELECTITEM, TVGN_CARET, reinterpret_cast<LPARAM>(hTreeItem));
        if (fExpandToShowChildren && TreeNext(hWnd, TVGN_CHILD, hTreeItem) == nullptr) {
            ::SendMessage(hWnd, TVM_EXPAND, TVE_EXPAND, reinterpret_cast<LPARAM>(hTreeItem));
        }
        ::SendMessage(hWnd, TVM_ENSUREVISIBLE, 0, reinterpret_cast<LPARAM>(hTreeItem));
        s->m_bNoNotify = FALSE;
        bRes = TRUE;
    }
    ::SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    ::RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    return bRes;
}

// Symbol: ?WindowProc@CMFCShellTreeCtrl@@MEAA_JI_K_J@Z
// Transcribed from retail entry RVA 0x12e5c0 (mfc140u).  Retail tests the
// message against three constants and the static pointer, and tail-jumps to
// CWnd::WindowProc from 0x12e601 when any test fails:
//     if (message != WM_DRAWITEM && message != WM_MEASUREITEM && message != WM_INITMENUPOPUP)   // 0x2b, 0x2c, 0x117
//         return CWnd::WindowProc(message, wParam, lParam);         // 0x28d140 = ordinal 14209 ?WindowProc@CWnd (see below)
//     if (m_pContextMenu2 == NULL)                                  // static at 0x1803be2c8 (mfc140u)
//         return CWnd::WindowProc(message, wParam, lParam);
//     m_pContextMenu2->HandleMenuMsg(message, wParam, lParam);      // IContextMenu2 vslot 6 (+0x30) via the CFG dispatcher 0x2c7b30
//     return 0;
// 0x28d140 is absent from mfc140u_rva_symbols.json but the ordinal table
// resolves ?WindowProc@CWnd@@MEAA_JI_K_J@Z (ord 14209) to it, and its body is
// CWnd::WindowProc's (`lResult = 0; if (!OnWndMsg(..., &lResult)) lResult =
// DefWindowProc(...)`, vslots 72/73).  CTreeCtrl does not override WindowProc.
extern "C" LRESULT MS_ABI impl__WindowProc_CMFCShellTreeCtrl__MEAA_JI_K_J_Z(void* pThis, UINT message, WPARAM wParam, LPARAM lParam) {
    if (pThis == nullptr) return 0;
    if ((message == WM_DRAWITEM || message == WM_MEASUREITEM || message == WM_INITMENUPOPUP) &&
        impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA != nullptr) {
        static_cast<IContextMenu2*>(impl__m_pContextMenu2_CMFCShellTreeCtrl__1PEAUIContextMenu2__EA)->HandleMenuMsg(message, wParam, lParam);
        return 0;
    }
    return impl__WindowProc_CWnd__MEAA_JI_K_J_Z(static_cast<CWnd*>(pThis), message, wParam, lParam);
}

// Symbol: ?OnInitControl@CMFCShellTreeCtrl@@IEAA_J_K_J@Z
// Transcribed from retail entry RVA 0x12e710 (mfc140u):
//     CString strDst;
//     CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                               // 0x13d840
//     CString strValue;
//     if (tagManager.ExcludeTag(_T("MFCShellTreeCtrl_EnableShellContextMenu"), strValue, FALSE) &&  // 0x13df50, 0x180341e20
//         !strValue.IsEmpty()) {
//         strValue.MakeUpper();                                     // 0x1fb80
//         m_bContextMenu = (wcscmp(strValue, _T("TRUE")) == 0);     // IAT wcscmp, 0x18033d930
//     }
//     return 0;                                                     // (~strValue, ~tagManager, ~strDst)
// The CTagManager thunks in controls/CTagManager.cpp key their state by the
// object address and never touch its storage, so an opaque local buffer
// stands in for the retail object (the same shape CMFCShellListCtrl.cpp uses).
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCShellTreeCtrl__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
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
                tagManagerStorage, L"MFCShellTreeCtrl_EnableShellContextMenu", &strValue, FALSE) &&
            !strValue.IsEmpty()) {
            impl__MakeUpper___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&strValue);
            L(pThis)->m_bContextMenu = (std::wcscmp(strValue.GetString(), L"TRUE") == 0) ? TRUE : FALSE;
        }
    }
    impl___1CTagManager__UEAA_XZ(tagManagerStorage);
    return 0;
}
