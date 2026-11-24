// CMDIClientAreaWnd — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// ---------------------------------------------------------------------------
// Status of this translation unit (2026-09-14 implementation pass, on top of
// the 2026-09-07 decode pass)
// ---------------------------------------------------------------------------
// Every export below was disassembled out of the retail image; the entry RVA
// is cited on each body and the image it belongs to is named every time
// ("mfc140u" for the Unicode DLL OpenMFC reimplements, "mfc140" for the ANSI
// twin, whose function BODIES are byte-identical but whose addresses differ).
// Symbols absent from the mfc140u map were decoded from the mfc140 twin.
//
// How this class is modelled here.  OpenMFC declares no `class
// CMDIClientAreaWnd` in include/openmfc/ and has no detail/*Support.h shadow
// for it, so this file carries the retail layout itself (S_CMDIClientAreaWnd
// below, pinned by static_asserts).  Which bytes are ABI-load-bearing follows
// from the shipping afxmdiclientareawnd.h: apps compiled against it inline
//   GetMDITabs()          -> &m_wndTab            (+0xe8)
//   DoesMDITabExist()     -> m_bTabIsEnabled      (+0x3e54)
//   IsMDITabbedGroup()    -> m_bIsMDITabbedGroup  (+0x3f48)
//   GetMDITabGroups()     -> m_lstTabbedGroups    (+0x3ed8), which CObList's
//                            inline GetCount/GetHeadPosition/GetNext walk as
//                            {m_nCount, m_pNodeHead, node->pNext, node->data}
// so those live in the object's own bytes at the retail offsets; the group
// list is a hand-maintained CObList-layout list (OpenMFC's CObList keeps its
// contents in a side table, which those inline walkers cannot see).  The two
// CMap members and the CStringList are private to the DLL (no inline accessor
// reaches them), so their contents live in a per-instance companion record
// and only their POD headers are written at the retail offsets.
//
// The frame owns a separately allocated client wrapper and binds it to the
// native MDICLIENT window. The same constructor and registry also support
// client-area objects constructed through the individual exports.
//
// ---------------------------------------------------------------------------
// Retail CMDIClientAreaWnd layout, transcribed from the constructor
// ??0CMDIClientAreaWnd@@QEAA@XZ (entry RVA 0x7c900, mfc140u) and cross-checked
// against the destructor (entry RVA 0x7cb20, mfc140u); member names from the
// shipping afxmdiclientareawnd.h, in declaration order.
// ---------------------------------------------------------------------------
//   +0x0000  CWnd base                (ctor calls ??0CWnd@@QEAA@XZ, 0x28a700)
//            vftable stored = 0x2ed5a8 (mfc140u); OpenMFC keeps CWnd's
//   +0x0040  CWnd::m_hWnd
//   +0x00e8  CMFCTabCtrl m_wndTab     (ctor 0x136d80, dtor 0x137030; sizeof
//                                      0x3d68 == the operator new size in
//                                      CreateTabGroup)
//   +0x3e50  BOOL m_bTabIsVisible     (8-byte zero store covers +0x3e50/+0x3e54)
//   +0x3e54  BOOL m_bTabIsEnabled
//   +0x3e58  BOOL m_bLastActiveTab
//   +0x3e60  CImageList m_TabIcons    (vftable 0x337b38 + NULL handle, inlined)
//   +0x3e70  CMap<HICON,HICON,int,int> m_mapIcons (vftable 0x2ddbb0, hash
//                                      size 17, block size 10; 0x38 bytes)
//   +0x3ea8  CMDITabInfo m_mdiTabParams (ctor 0x7c440; 0x30 bytes, copied as
//                                      three 16-byte moves by EnableMDITabs)
//   +0x3ed8  CObList m_lstTabbedGroups  (vftable 0x32a6f8; m_pNodeHead +8,
//                                      m_pNodeTail +0x10, m_nCount +0x18,
//                                      m_pNodeFree +0x20, m_pBlocks +0x28,
//                                      m_nBlockSize +0x30 == 10; nodes are
//                                      {pNext +0, pPrev +8, data +0x10})
//   +0x3f10  CMap<CWnd*,CWnd*,CImageList*,CImageList*> m_mapTabIcons
//                                     (vftable 0x2ed3a8; assoc layout
//                                      {key +0, value +8, pNext +0x10,
//                                      nHashValue +0x18} per the dtor's walk)
//   +0x3f48  BOOL m_bIsMDITabbedGroup
//   +0x3f4c  BOOL m_bNewVericalGroup  (NOT written by the retail ctor)
//   +0x3f50  BOOL m_bDisableUpdateTabs
//   +0x3f58  CObList m_lstRemovedTabbedGroups (vftable 0x32a6f8)
//   +0x3f90  int  m_nResizeMargin     == 40
//   +0x3f94  int  m_nNewGroupMargin   == 40
//   +0x3f98  int  m_nTotalResizeRest  == 0
//   +0x3f9c  CRect m_rectNewTabGroup  (zeroed, then ::SetRectEmpty'd)
//   +0x3fb0  CStringList m_lstLoadedTabDocuments (vftable 0x32a788)
//   +0x3fe8  GROUP_ALIGNMENT m_groupAlignment (8-byte zero store covers
//                                      +0x3fe8/+0x3fec)
//   +0x3fec  BOOL m_bInsideDragComplete
//   sizeof == 0x3ff0
//
// Retail indirect calls in this class go through the CFG dispatcher
// (`call *0x2c7b30(%rip)` in mfc140u / `*0x2c5bd0` in mfc140 with the callee
// already in %rax); that cell is not an import slot, so such sequences are
// plain virtual calls.  The CMFCTabCtrl slots they hit were read out of the
// CMFCTabCtrl vftable (0x310520, mfc140):
//   +0x2d8 IsPtInTabArea   +0x308 (unexported, tests +0x3d0)
//   +0x318 RecalcLayout    +0x328 AddTab(CWnd*,LPCTSTR,UINT,BOOL)
//   +0x340 RemoveTab       +0x368 GetTabsNum  (`mov 0x150(%rcx),%eax; ret`)
//   +0x370 GetTabWnd       +0x428 GetActiveTab(`mov 0x154(%rcx),%eax; ret`)
//   +0x430 GetActiveWnd    +0x438 CMFCTabCtrl::SetActiveTab
//   +0x448 GetTabFromHwnd  +0x498 SetTabBorderSize   +0x500 SetLocation
// OpenMFC has no MSVC vtable for CMFCTabCtrl, so every one of those is reached
// through the corresponding impl__ thunk instead.  GetTabsNum/GetActiveTab are
// inline in the shipping headers (no export, so no thunk): TabsNum()/ActiveTab()
// below derive them from the GetTabWnd/GetActiveWnd thunks.
//
// Raw fields of OTHER classes that these bodies touch, all of which the
// shipping headers expose through inline accessors (so they are part of the
// app-visible contract) but OpenMFC's own classes do not model:
//   CMFCTabCtrl   +0x334 m_bActivateLastActiveTab  (EnableActivateLastActive)
//                 +0x408 m_bIsActiveInMDITabGroup  (Set/IsActiveInMDITabGroup)
//                 +0x3d28 m_ResizeMode             (GetResizeMode)
//   CMDIChildWndEx +0x1e8 m_pRelatedTabGroup       (Get/SetRelatedTabGroup;
//                        zeroed at 0x838a4 inside the retail ctor
//                        ??0CMDIChildWndEx@@IEAA@XZ, entry 0x83820, mfc140u;
//                        0x83db0 is that ctor's entry in the mfc140 twin)
//                 +0x720 m_tabProxyWnd (CMDITabProxyWnd, ctor 0x86bb0 called
//                        at 0x8387b of the same ctor); its m_hWnd is +0x760
// Groups this file allocates are 0x3d68 bytes and zero-filled before the
// OpenMFC CMFCTabCtrl ctor runs, so the CMFCTabCtrl fields are defined (zero)
// memory. OpenMFC's CMDIChildWndEx ctor zero-fills its padding, which covers
// +0x1e8 and initializes m_pRelatedTabGroup to NULL; MoveWindowToTabGroup
// writes that link. The retail tab proxy at +0x720 is not constructed, so its
// HWND at +0x760 remains unavailable; taskbar paths gate reads of it through
// OpenMFC's IsTaskbarTabsSupportEnabled stub.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MDIClientSupport.h"
#include "detail/MfccoreSupport.h"

#include <algorithm>
#include <commctrl.h>
#include <windowsx.h>

#include <cmath>
#include <cstddef>
#include <cstring>
#include <memory>

namespace openmfc { namespace detail { namespace cmdiclientarea {} } }

// ---------------------------------------------------------------------------
// Thunks for DLL-internal calls (BRIEFING S1: C++ methods exist only as their
// extern "C" impl__ thunks).  Every one below has a definition in the tree;
// the ones whose definition still carries an auto-generated parameter list
// are flagged where they are called.
// ---------------------------------------------------------------------------
extern "C" CString* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
    CString*, const wchar_t*, const wchar_t*);
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(CDockingManager*, UINT, int);
extern "C" void MS_ABI impl__Serialize_CMDIClientAreaWnd__UEAAXAEAVCArchive___Z(void*, void*);
extern "C" void MS_ABI impl__SetAutoColors_CMFCBaseTabCtrl__QEAAXAEBV__CArray_KK___Z(
    CMFCBaseTabCtrl*, const CArray<COLORREF, COLORREF>&);

// This file (forward declarations of exports defined further down).
extern "C" int MS_ABI impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(CMDIChildWndEx*);
extern "C" void MS_ABI impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible);
extern "C" void MS_ABI impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible);
extern "C" void MS_ABI impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup);
extern "C" void* MS_ABI impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(void* pThis, void* pTabGroup);
extern "C" void* MS_ABI impl__CreateNewTabGroup_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_VCRect__H_Z(void* pThis, void* pTabGroup, const RECT* pRect, int bVertical);
extern "C" void MS_ABI impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(void* pThis, void* pTabGroup, int bRecalcLayout);
extern "C" void MS_ABI impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup);
extern "C" void* MS_ABI impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(void* pThis, HWND hWnd, int* pnTabIndex);
extern "C" void* MS_ABI impl__FindActiveTabWndByActiveChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis);
extern "C" void* MS_ABI impl__FindActiveTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis);
extern "C" void* MS_ABI impl__GetNextTabWnd_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_H_Z(void* pThis, void* pTabGroup, int bWithoutAsserts);
extern "C" int MS_ABI impl__IsKeepClientEdge_CMDIClientAreaWnd__AEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__DrawNewGroupRect_CMDIClientAreaWnd__AEAAXPEBUtagRECT__0_Z(void* pThis, const RECT* pRectNew, const RECT* pRectOld);
extern "C" void* MS_ABI impl__TabWndFromPoint_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__VCPoint___Z(void* pThis, POINT pt);
extern "C" int MS_ABI impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(void* pThis, void* pFrom, void* pTo, int nIdxFrom);
extern "C" void MS_ABI impl__SetActiveTab_CMDIClientAreaWnd__QEAAXPEAUHWND_____Z(void* pThis, HWND hWnd);
extern "C" void MS_ABI impl__EnableMDITabbedGroups_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(void* pThis, int bEnable, const void* pParams);
extern "C" void* MS_ABI impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAVCMFCTabCtrl__H_Z(void* pThis, void* pTabGroup, int nStartFrom);
extern "C" void* MS_ABI impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAV2__Z(void* pThis, void* pMDIChild);
extern "C" unsigned long MS_ABI impl__GetMDITabsContextMenuAllowedItems_CMDIClientAreaWnd__QEAAKXZ(void* pThis);

// core/runtime, core/window
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);          // ??2@YAPEAX_K@Z  (detail/MemcoreSupport.cpp)
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                    // ??3@YAXPEAX@Z
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__IsTaskbarInteractionEnabled_CWinApp__UEAAHXZ(CWinApp* pThis);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" unsigned int impl__AFX_WM_ON_MOVETOTABGROUP__3IA;
extern "C" const unsigned char impl__wndBottom_CWnd__2V1_B[];            // ?wndBottom@CWnd@@2V1@B (core/window/CWnd.cpp)
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, unsigned int message, WPARAM wParam, LPARAM lParam);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(CWnd* pThis, RECT* lpClientRect, unsigned int nAdjustType);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(CFrameWnd* pThis, int bNotify);
extern "C" void MS_ABI impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(CMDIFrameWndEx* pThis, int bNotify);   // definition still carries a generated `(int)` list
extern "C" int MS_ABI impl__OnSetMenu_CMDIFrameWndEx__MEAAHPEAUHMENU_____Z(CMDIFrameWndEx* pThis, HMENU hMenu);   // definition still carries a generated `(void**)` list
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();
// core/gdi/CImageList.cpp
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* pThis);
extern "C" void MS_ABI impl___1CImageList__UEAA_XZ(CImageList* pThis);
extern "C" int MS_ABI impl__DeleteImageList_CImageList__QEAAHXZ(CImageList* pThis);
extern "C" int MS_ABI impl__Create_CImageList__QEAAHHHIHH_Z(CImageList* pThis, int cx, int cy, unsigned int nFlags, int nInitial, int nGrow);
// core/frame/CMDITabInfo.cpp, CMDIChildWndEx.cpp
extern "C" int MS_ABI impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(CMDIChildWndEx* pThis);
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pWndInsertBefore);
extern "C" CString* MS_ABI impl__GetFrameText_CMDIChildWndEx__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const CMDIChildWndEx* pThis, CString* pRet);
extern "C" void MS_ABI impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* pThis);
// featurepack/tabs
extern "C" void* MS_ABI impl___0CMFCTabCtrl__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCTabCtrl__UEAA_XZ(void* pThis);
extern "C" CWnd* MS_ABI impl__GetTabWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__H_Z(const CMFCBaseTabCtrl* pThis, int iTab);
extern "C" CWnd* MS_ABI impl__GetActiveWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__XZ(const CMFCBaseTabCtrl* pThis);
extern "C" int MS_ABI impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(const CMFCBaseTabCtrl* pThis, HWND hWnd);
extern "C" int MS_ABI impl__RemoveTab_CMFCBaseTabCtrl__UEAAHHH_Z(CMFCBaseTabCtrl* pThis, int iTab, int bRecalcLayout);
extern "C" void MS_ABI impl__AddTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__PEB_WIH_Z(CMFCBaseTabCtrl* pThis, CWnd* pTabWnd, const wchar_t* lpszLabel, unsigned int uiImageId, int bDetachable);
extern "C" void MS_ABI impl__InvalidateTab_CMFCBaseTabCtrl__QEAAXH_Z(CMFCBaseTabCtrl* pThis, int iTab);
extern "C" int MS_ABI impl__EnableCustomToolTips_CMFCBaseTabCtrl__QEAAHH_Z(CMFCBaseTabCtrl* pThis, int bEnable);
extern "C" void MS_ABI impl__EnableAutoColor_CMFCBaseTabCtrl__QEAAXH_Z(CMFCBaseTabCtrl* pThis, int bEnable);
extern "C" void MS_ABI impl__SetTabBorderSize_CMFCBaseTabCtrl__UEAAXHH_Z(CMFCBaseTabCtrl* pThis, int nSize, int bRepaint);
extern "C" void MS_ABI impl__SetLocation_CMFCBaseTabCtrl__UEAAXW4Location_1__Z(CMFCBaseTabCtrl* pThis, int location);
extern "C" int MS_ABI impl__ModifyTabStyle_CMFCTabCtrl__QEAAHW4Style_1__Z(CMFCTabCtrl* pThis, int style);
extern "C" void MS_ABI impl__EnableTabDocumentsMenu_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, int bEnable);
extern "C" void MS_ABI impl__EnableActiveTabCloseButton_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, int bEnable);
extern "C" void MS_ABI impl__HideNoTabs_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, int bHide);
extern "C" void MS_ABI impl__SetActiveTabBoldFont_CMFCTabCtrl__QEAAXH_Z(CMFCTabCtrl* pThis, int bBold);   // definition still carries a generated `(int)` list
extern "C" int MS_ABI impl__SetActiveTab_CMFCTabCtrl__UEAAHH_Z(CMFCTabCtrl* pThis, int iTab);              // definition still carries a generated `(int)` list
extern "C" void MS_ABI impl__RecalcLayout_CMFCTabCtrl__UEAAXXZ(CMFCTabCtrl* pThis);                        // definition still carries a generated `()` list
extern "C" int MS_ABI impl__Create_CMFCTabCtrl__QEAAHW4Style_1_AEBUtagRECT__PEAVCWnd__IW4Location_CMFCBaseTabCtrl__H_Z(
    CMFCTabCtrl* pThis, int style, const RECT* pRect, CWnd* pParentWnd, unsigned int nID, int location, int bCloseBtn);   // definition still carries a generated 35-entry list
// Thunks used by DrawNewGroupRect.
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size, const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast);

namespace {

// ---------------------------------------------------------------------------
// Retail layout (see the map in the file header).
// ---------------------------------------------------------------------------
// CObList::CNode as the shipping afxcoll.h lays it out.
struct RetailNode {
    RetailNode* pNext;
    RetailNode* pPrev;
    void* data;
};
// CObList / CStringList header (0x38 bytes).  The vptr stays NULL here: no
// OpenMFC vtable exists for the retail object, and nothing inline in the
// shipping headers reads it.
struct RetailObList {
    void* vptr;
    RetailNode* pNodeHead;
    RetailNode* pNodeTail;
    INT_PTR nCount;
    void* pNodeFree;
    void* pBlocks;
    INT_PTR nBlockSize;
};
static_assert(sizeof(RetailObList) == 0x38, "CObList header is 0x38 bytes");
// CMap<> header (0x38 bytes).  Contents are DLL-private (no inline accessor
// reaches either map), so only this header is kept at the retail offset; the
// entries live in the companion record below.
struct RetailMap {
    void* vptr;
    void* pHashTable;
    unsigned int nHashTableSize;
    INT_PTR nCount;
    void* pFreeList;
    void* pBlocks;
    INT_PTR nBlockSize;
};
static_assert(sizeof(RetailMap) == 0x38, "CMap header is 0x38 bytes");
static_assert(sizeof(CMDITabInfo) == 0x30, "CMDITabInfo is 0x30 bytes");

constexpr std::size_t kRetailTabCtrlSize = 0x3d68;   // sizeof(CMFCTabCtrl), retail

struct S_CMDIClientAreaWnd {
    unsigned char cwnd[0xe8];                       // CWnd base
    unsigned char m_wndTab[kRetailTabCtrlSize];     // +0xe8   CMFCTabCtrl
    BOOL m_bTabIsVisible;                           // +0x3e50
    BOOL m_bTabIsEnabled;                           // +0x3e54
    BOOL m_bLastActiveTab;                          // +0x3e58
    int pad_3e5c;
    unsigned char m_TabIcons[0x10];                 // +0x3e60 CImageList
    RetailMap m_mapIcons;                           // +0x3e70
    CMDITabInfo m_mdiTabParams;                // +0x3ea8
    RetailObList m_lstTabbedGroups;                 // +0x3ed8
    RetailMap m_mapTabIcons;                        // +0x3f10
    BOOL m_bIsMDITabbedGroup;                       // +0x3f48
    BOOL m_bNewVericalGroup;                        // +0x3f4c
    BOOL m_bDisableUpdateTabs;                      // +0x3f50
    int pad_3f54;
    RetailObList m_lstRemovedTabbedGroups;          // +0x3f58
    int m_nResizeMargin;                            // +0x3f90
    int m_nNewGroupMargin;                          // +0x3f94
    int m_nTotalResizeRest;                         // +0x3f98
    RECT m_rectNewTabGroup;                         // +0x3f9c
    int pad_3fac;
    RetailObList m_lstLoadedTabDocuments;           // +0x3fb0 CStringList
    int m_groupAlignment;                           // +0x3fe8
    BOOL m_bInsideDragComplete;                     // +0x3fec
};
static_assert(sizeof(S_CMDIClientAreaWnd) == 0x3ff0, "retail sizeof(CMDIClientAreaWnd)");
static_assert(offsetof(S_CMDIClientAreaWnd, m_wndTab) == 0xe8, "m_wndTab");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bTabIsVisible) == 0x3e50, "m_bTabIsVisible");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bTabIsEnabled) == 0x3e54, "m_bTabIsEnabled");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bLastActiveTab) == 0x3e58, "m_bLastActiveTab");
static_assert(offsetof(S_CMDIClientAreaWnd, m_TabIcons) == 0x3e60, "m_TabIcons");
static_assert(offsetof(S_CMDIClientAreaWnd, m_mapIcons) == 0x3e70, "m_mapIcons");
static_assert(offsetof(S_CMDIClientAreaWnd, m_mdiTabParams) == 0x3ea8, "m_mdiTabParams");
static_assert(offsetof(S_CMDIClientAreaWnd, m_lstTabbedGroups) == 0x3ed8, "m_lstTabbedGroups");
static_assert(offsetof(S_CMDIClientAreaWnd, m_mapTabIcons) == 0x3f10, "m_mapTabIcons");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bIsMDITabbedGroup) == 0x3f48, "m_bIsMDITabbedGroup");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bNewVericalGroup) == 0x3f4c, "m_bNewVericalGroup");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bDisableUpdateTabs) == 0x3f50, "m_bDisableUpdateTabs");
static_assert(offsetof(S_CMDIClientAreaWnd, m_lstRemovedTabbedGroups) == 0x3f58, "m_lstRemovedTabbedGroups");
static_assert(offsetof(S_CMDIClientAreaWnd, m_nResizeMargin) == 0x3f90, "m_nResizeMargin");
static_assert(offsetof(S_CMDIClientAreaWnd, m_nNewGroupMargin) == 0x3f94, "m_nNewGroupMargin");
static_assert(offsetof(S_CMDIClientAreaWnd, m_nTotalResizeRest) == 0x3f98, "m_nTotalResizeRest");
static_assert(offsetof(S_CMDIClientAreaWnd, m_rectNewTabGroup) == 0x3f9c, "m_rectNewTabGroup");
static_assert(offsetof(S_CMDIClientAreaWnd, m_lstLoadedTabDocuments) == 0x3fb0, "m_lstLoadedTabDocuments");
static_assert(offsetof(S_CMDIClientAreaWnd, m_groupAlignment) == 0x3fe8, "m_groupAlignment");
static_assert(offsetof(S_CMDIClientAreaWnd, m_bInsideDragComplete) == 0x3fec, "m_bInsideDragComplete");
// The OpenMFC objects placed into the retail-sized slots must fit them.
static_assert(sizeof(CWnd) <= 0xe8, "OpenMFC CWnd must fit the retail base");
static_assert(sizeof(CMFCTabCtrl) <= kRetailTabCtrlSize, "OpenMFC CMFCTabCtrl must fit m_wndTab");
static_assert(sizeof(CImageList) <= 0x10, "OpenMFC CImageList must fit m_TabIcons");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd at +0x40");

// GROUP_ALIGNMENT (afxmdiclientareawnd.h) and CMFCTabCtrl::ResizeMode
// (afxtabctrl.h) share the values retail stores into both fields.
constexpr int kGroupNoAlign = 0;
constexpr int kGroupVertAlign = 1;
constexpr int kGroupHorzAlign = 2;

// ---------------------------------------------------------------------------
// Per-instance companion record + registry.  Storage goes through the DLL's
// own operator new/delete exports (the retail bodies allocate through the
// same ??2@YAPEAX_K@Z), which keeps this translation unit free of libstdc++
// link dependencies.
// ---------------------------------------------------------------------------
struct TabIconEntry {              // one m_mapTabIcons association
    CWnd* key;
    CImageList* value;
    TabIconEntry* pNext;
};
struct DocumentPlacement {
    HWND window;
    WINDOWPLACEMENT placement;
    LONG_PTR removedStyle = 0;
};

struct TabDrag {
    HWND source = nullptr;
    HWND document = nullptr;
    POINT origin{};
    HWND previousFocus = nullptr;
    bool started = false;
};

struct ClientRuntime {
    TabDrag drag;
    std::vector<DocumentPlacement> placements;
    CObList groups;
    HWND resizingGroup = nullptr;
    int resizeOffset = 0;
    bool updating = false;
};

struct InstanceRecord {
    void* pThis;
    TabIconEntry* pTabIcons;
    ClientRuntime* runtime;
    InstanceRecord* pNext;
};

SRWLOCK g_registryLock = SRWLOCK_INIT;
InstanceRecord* g_records = nullptr;

void* RawAlloc(std::size_t n) {
    void* p = impl___2_YAPEAX_K_Z(n);
    if (p != nullptr) {
        memset(p, 0, n);
    }
    return p;
}

InstanceRecord* Record(const void* pThis) {
    AcquireSRWLockExclusive(&g_registryLock);
    InstanceRecord* r = g_records;
    while (r != nullptr && r->pThis != pThis) {
        r = r->pNext;
    }
    ReleaseSRWLockExclusive(&g_registryLock);
    return r;
}

// The instance behind `this`, or NULL when this file's constructor never ran
// on it (see the file header): every export treats NULL as "do nothing".
S_CMDIClientAreaWnd* Self(void* pThis) {
    return Record(pThis) != nullptr ? static_cast<S_CMDIClientAreaWnd*>(pThis) : nullptr;
}

void RegisterInstance(void* pThis) {
    InstanceRecord* r = static_cast<InstanceRecord*>(RawAlloc(sizeof(InstanceRecord)));
    if (r == nullptr) {
        return;
    }
    r->pThis = pThis;
    r->runtime = new ClientRuntime;
    AcquireSRWLockExclusive(&g_registryLock);
    r->pNext = g_records;
    g_records = r;
    ReleaseSRWLockExclusive(&g_registryLock);
}

void UnregisterInstance(void* pThis) {
    AcquireSRWLockExclusive(&g_registryLock);
    InstanceRecord** pp = &g_records;
    while (*pp != nullptr && (*pp)->pThis != pThis) {
        pp = &(*pp)->pNext;
    }
    InstanceRecord* r = *pp;
    if (r != nullptr) {
        *pp = r->pNext;
    }
    ReleaseSRWLockExclusive(&g_registryLock);
    if (r != nullptr) {
        delete r->runtime;
        impl___3_YAXPEAX_Z(r);
    }
}

// ---------------------------------------------------------------------------
// CObList-layout list maintenance (m_lstTabbedGroups & co).  Retail carves
// nodes out of CPlex blocks; apps never allocate into these lists, only walk
// them, so individually allocated nodes are indistinguishable to them.
// ---------------------------------------------------------------------------
void ListInit(RetailObList& l) {
    l.vptr = nullptr;
    l.pNodeHead = nullptr;
    l.pNodeTail = nullptr;
    l.nCount = 0;
    l.pNodeFree = nullptr;
    l.pBlocks = nullptr;
    l.nBlockSize = 10;
}

RetailNode* ListFind(const RetailObList& l, const void* data) {
    for (RetailNode* n = l.pNodeHead; n != nullptr; n = n->pNext) {
        if (n->data == data) {
            return n;
        }
    }
    return nullptr;
}

void ListAddTail(RetailObList& l, void* data) {
    RetailNode* n = static_cast<RetailNode*>(RawAlloc(sizeof(RetailNode)));
    if (n == nullptr) {
        return;
    }
    n->data = data;
    n->pPrev = l.pNodeTail;
    if (l.pNodeTail != nullptr) {
        l.pNodeTail->pNext = n;
    } else {
        l.pNodeHead = n;
    }
    l.pNodeTail = n;
    ++l.nCount;
}

void ListInsertAfter(RetailObList& l, RetailNode* pos, void* data) {
    if (pos == nullptr) {
        ListAddTail(l, data);
        return;
    }
    RetailNode* n = static_cast<RetailNode*>(RawAlloc(sizeof(RetailNode)));
    if (n == nullptr) {
        return;
    }
    n->data = data;
    n->pPrev = pos;
    n->pNext = pos->pNext;
    if (pos->pNext != nullptr) {
        pos->pNext->pPrev = n;
    } else {
        l.pNodeTail = n;
    }
    pos->pNext = n;
    ++l.nCount;
}

void ListRemoveAt(RetailObList& l, RetailNode* n) {
    if (n == nullptr) {
        return;
    }
    if (n->pPrev != nullptr) {
        n->pPrev->pNext = n->pNext;
    } else {
        l.pNodeHead = n->pNext;
    }
    if (n->pNext != nullptr) {
        n->pNext->pPrev = n->pPrev;
    } else {
        l.pNodeTail = n->pPrev;
    }
    --l.nCount;
    impl___3_YAXPEAX_Z(n);
}

void* ListRemoveTail(RetailObList& l) {
    RetailNode* n = l.pNodeTail;
    if (n == nullptr) {
        return nullptr;
    }
    void* data = n->data;
    ListRemoveAt(l, n);
    return data;
}

void ListRemoveAll(RetailObList& l) {
    while (l.pNodeHead != nullptr) {
        ListRemoveAt(l, l.pNodeHead);
    }
}

void MapInit(RetailMap& m) {
    m.vptr = nullptr;
    m.pHashTable = nullptr;
    m.nHashTableSize = 17;
    m.nCount = 0;
    m.pFreeList = nullptr;
    m.pBlocks = nullptr;
    m.nBlockSize = 10;
}

// m_mapTabIcons operations on the companion record.
TabIconEntry* TabIconsLookup(InstanceRecord* r, const CWnd* key) {
    for (TabIconEntry* e = r != nullptr ? r->pTabIcons : nullptr; e != nullptr; e = e->pNext) {
        if (e->key == key) {
            return e;
        }
    }
    return nullptr;
}

void TabIconsSetAt(InstanceRecord* r, S_CMDIClientAreaWnd* self, CWnd* key, CImageList* value) {
    if (r == nullptr) {
        return;
    }
    TabIconEntry* e = TabIconsLookup(r, key);
    if (e == nullptr) {
        e = static_cast<TabIconEntry*>(RawAlloc(sizeof(TabIconEntry)));
        if (e == nullptr) {
            return;
        }
        e->key = key;
        e->pNext = r->pTabIcons;
        r->pTabIcons = e;
        ++self->m_mapTabIcons.nCount;
    }
    e->value = value;
}

void TabIconsRemoveKey(InstanceRecord* r, S_CMDIClientAreaWnd* self, const CWnd* key) {
    if (r == nullptr) {
        return;
    }
    TabIconEntry** pp = &r->pTabIcons;
    while (*pp != nullptr && (*pp)->key != key) {
        pp = &(*pp)->pNext;
    }
    TabIconEntry* e = *pp;
    if (e != nullptr) {
        *pp = e->pNext;
        impl___3_YAXPEAX_Z(e);
        --self->m_mapTabIcons.nCount;
    }
}

void TabIconsRemoveAll(InstanceRecord* r, S_CMDIClientAreaWnd* self) {
    if (r == nullptr) {
        return;
    }
    while (r->pTabIcons != nullptr) {
        TabIconEntry* e = r->pTabIcons;
        r->pTabIcons = e->pNext;
        impl___3_YAXPEAX_Z(e);
    }
    self->m_mapTabIcons.nCount = 0;
}

// `delete pImages` for a CImageList this file allocated (retail dispatches
// the scalar deleting destructor through the object's vtable).
void DeleteImageListObject(CImageList* p) {
    if (p != nullptr) {
        impl___1CImageList__UEAA_XZ(p);
        impl___3_YAXPEAX_Z(p);
    }
}

// `delete pTabCtrl` for a group.  Retail dispatches vtable slot 1 (the scalar
// deleting destructor) so an app override of CreateTabGroup returning a
// derived class would run the derived destructor; OpenMFC has no such
// dispatch, so the CMFCTabCtrl destructor thunk is what runs.
void DeleteTabCtrlObject(void* p) {
    if (p != nullptr) {
        impl___1CMFCTabCtrl__UEAA_XZ(p);
        impl___3_YAXPEAX_Z(p);
    }
}

// ---------------------------------------------------------------------------
// Small views.
// ---------------------------------------------------------------------------
CWnd* AsWnd(void* p) { return static_cast<CWnd*>(p); }
const CWnd* AsWnd(const void* p) { return static_cast<const CWnd*>(p); }
CWnd* SelfWnd(S_CMDIClientAreaWnd* self) { return reinterpret_cast<CWnd*>(self); }
HWND WndHandle(const void* pWnd) { return pWnd != nullptr ? AsWnd(pWnd)->m_hWnd : nullptr; }
CMFCTabCtrl* EmbeddedTab(S_CMDIClientAreaWnd* self) { return reinterpret_cast<CMFCTabCtrl*>(self->m_wndTab); }
CMFCBaseTabCtrl* AsBaseTab(void* p) { return static_cast<CMFCBaseTabCtrl*>(static_cast<CMFCTabCtrl*>(p)); }
const CWnd* WndBottom() { return reinterpret_cast<const CWnd*>(impl__wndBottom_CWnd__2V1_B); }

// DYNAMIC_DOWNCAST(CMFCTabCtrl, p) etc., exactly as retail inlines them
// (a NULL input yields NULL without calling IsKindOf).
CMFCTabCtrl* AsTabCtrl(void* p) {
    if (p == nullptr) {
        return nullptr;
    }
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<CObject*>(AsWnd(p)),
                                                              impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ())
               ? static_cast<CMFCTabCtrl*>(p) : nullptr;
}
CMDIChildWndEx* AsChildEx(CWnd* p) {
    if (p == nullptr) {
        return nullptr;
    }
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ())
               ? static_cast<CMDIChildWndEx*>(p) : nullptr;
}
CMDIFrameWndEx* AsFrameEx(CFrameWnd* p) {
    if (p == nullptr) {
        return nullptr;
    }
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())
               ? static_cast<CMDIFrameWndEx*>(p) : nullptr;
}

int TabsNum(const void* pTab) {
    return pTab ? static_cast<const CMFCTabCtrl*>(pTab)->GetTabsCount() : 0;
}

int ActiveTab(const void* pTab) {
    return pTab ? static_cast<const CMFCTabCtrl*>(pTab)->GetActiveTab() : -1;
}

CWnd* TabWnd(const void* pTab, int index) {
    return pTab ? static_cast<const CMFCTabCtrl*>(pTab)->GetTabWnd(index) : nullptr;
}
int TabFromHwnd(const void* pTab, HWND hWnd) {
    return impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(
        static_cast<const CMFCBaseTabCtrl*>(static_cast<const CMFCTabCtrl*>(pTab)), hWnd);
}

// Inline-exposed CMFCTabCtrl / CMDIChildWndEx fields (see the file header).
BOOL& TabActivateLastActive(void* pTab) { return *reinterpret_cast<BOOL*>(static_cast<unsigned char*>(pTab) + 0x334); }
BOOL& TabActiveInMDIGroup(void* pTab) { return *reinterpret_cast<BOOL*>(static_cast<unsigned char*>(pTab) + 0x408); }
int& TabResizeMode(void* pTab) { return *reinterpret_cast<int*>(static_cast<unsigned char*>(pTab) + 0x3d28); }
void SetChildRelatedTabGroup(CMDIChildWndEx* pChild, void* pGroup) {
    *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(pChild) + 0x1e8) = pGroup;
}
// m_tabProxyWnd.m_hWnd (+0x720 + 0x40).  Only ever read behind
// IsTaskbarTabsSupportEnabled(), which core/frame/CMDIChildWndEx.cpp keeps at
// FALSE, because OpenMFC's CMDIChildWndEx ctor never constructs the proxy
// window: the bytes are uninitialised until that class models it.
HWND ChildTabProxyHwnd(CMDIChildWndEx* pChild) {
    return *reinterpret_cast<HWND*>(reinterpret_cast<unsigned char*>(pChild) + 0x720 + 0x40);
}

// Retail's inline `if (!afxGlobalData.m_bInitialized) { Initialize(); m_bInitialized = 1; }`
// prologue (e.g. at 0x7e98c inside CreateTabGroup, mfc140u), reading OpenMFC's
// afxGlobalData blob the way core/frame/CFrameImpl.cpp does.
void EnsureGlobalDataInitialized() {
    int* pInitialized = reinterpret_cast<int*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
    if (*pInitialized == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
        *pInitialized = 1;
    }
}
int GlobalDataInt(std::size_t offset) {
    int value = 0;
    memcpy(&value, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + offset, sizeof(value));
    return value;
}
// AFX_GLOBAL_DATA::m_sizeSmallIcon is at +0x1a0/+0x1a4: the retail
// AFX_GLOBAL_DATA ctor (entry 0x6a590, mfc140u) stores the CFont vftable
// (0x2e8ad0) at +0x1a8, +0x1b8, ... +0x238 -- ten CFonts starting at +0x1a8 --
// and zeroes +0x1a0 as an 8-byte store (at 0x6a6a8) in the same run that
// zeroes each font's m_hObject; afxglobals.h declares `CSize m_sizeSmallIcon;`
// immediately before `CFont fontRegular;`, and CreateTabGroup (0x7e740) reads
// the pair at 0x3c17c0/0x3c17c4 == afxGlobalData (0x3c1620) + 0x1a0/+0x1a4.
constexpr std::size_t kGlobalSmallIconCx = 0x1a0;
constexpr std::size_t kGlobalSmallIconCy = 0x1a4;
// +0x258 is the flag ?Initialize@AFX_GLOBAL_DATA@@ stores at 0x6a81a
// (mfc140u); core/frame/CMDIChildWndEx.cpp documents it as the Windows-7
// taskbar flag.
constexpr std::size_t kGlobalTaskbarFlag = 0x258;

// Retail's `pFrame->RecalcLayout(TRUE)` is vslot +0x300 of the frame, which
// resolves to ?RecalcLayout@CMDIFrameWndEx@@UEAAXH@Z (0x893a0 mfc140u /
// 0x89950 mfc140; slot +0x300 of the CMDIFrameWndEx vftable 0x2ec8f8, mfc140) for
// the frames this class lives in and to CFrameWnd::RecalcLayout otherwise;
// with no MSVC vtable the dispatch is done by IsKindOf.
void FrameRecalcLayout(CFrameWnd* pFrame, BOOL bNotify) {
    if (pFrame == nullptr) {
        return;
    }
    if (CMDIFrameWndEx* pEx = AsFrameEx(pFrame)) {
        impl__RecalcLayout_CMDIFrameWndEx__UEAAXH_Z(pEx, bNotify);
    } else {
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(pFrame, bNotify);
    }
}

} // namespace

// CMDIClientAreaWnd::CMDIClientAreaWnd — transcribed from retail entry RVA
// 0x7c900 (mfc140u), in the retail order:
//     CWnd::CWnd();  vftable = 0x2ed5a8            (OpenMFC keeps CWnd's vtable:
//                                                   there is no CMDIClientAreaWnd
//                                                   vtable to install)
//     CMFCTabCtrl::CMFCTabCtrl(&m_wndTab);          (0x136d80)
//     m_TabIcons: vftable 0x337b38, m_hImageList = NULL  (inlined CImageList ctor)
//     m_mapIcons: vftable 0x2ddbb0, table NULL, hash size 0x11, count 0, free NULL,
//                 blocks NULL, block size 10
//     CMDITabInfo::CMDITabInfo(&m_mdiTabParams);    (0x7c440)
//     m_lstTabbedGroups: vftable 0x32a6f8, empty, block size 10
//     m_mapTabIcons: vftable 0x2ed3a8, same shape as m_mapIcons
//     m_lstRemovedTabbedGroups: vftable 0x32a6f8, empty, block size 10
//     m_rectNewTabGroup = {0,0,0,0}
//     m_lstLoadedTabDocuments: vftable 0x32a788, empty, block size 10
//     m_bTabIsVisible = m_bTabIsEnabled = 0 (one 8-byte store); m_bLastActiveTab = 0;
//     m_bIsMDITabbedGroup = 0; m_groupAlignment = m_bInsideDragComplete = 0 (8-byte);
//     m_nResizeMargin = m_nNewGroupMargin = 40; m_bDisableUpdateTabs = 0;
//     ::SetRectEmpty(&m_rectNewTabGroup);           (import 0x2c7348, mfc140u)
//     m_nTotalResizeRest = 0;
// Deviations: the retail ctor never writes m_bNewVericalGroup (+0x3f4c); it is
// zeroed here.  The 0x3d68 bytes of m_wndTab are zero-filled before OpenMFC's
// CMFCTabCtrl ctor runs, because that ctor only initialises OpenMFC's 424-byte
// object and the inline-exposed fields beyond it must not be left undefined.
// The instance is then registered so the other exports can find it (file
// header).
// Symbol: ??0CMDIClientAreaWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDIClientAreaWnd__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return pThis;
    }
    S_CMDIClientAreaWnd* self = static_cast<S_CMDIClientAreaWnd*>(pThis);

    impl___0CWnd__QEAA_XZ(pThis);

    memset(self->m_wndTab, 0, sizeof(self->m_wndTab));
    impl___0CMFCTabCtrl__QEAA_XZ(self->m_wndTab);

    memset(self->m_TabIcons, 0, sizeof(self->m_TabIcons));
    impl___0CImageList__QEAA_XZ(reinterpret_cast<CImageList*>(self->m_TabIcons));

    MapInit(self->m_mapIcons);
    self->m_mdiTabParams = CMDITabInfo{};
    ListInit(self->m_lstTabbedGroups);
    MapInit(self->m_mapTabIcons);
    self->m_bIsMDITabbedGroup = FALSE;
    self->m_bNewVericalGroup = FALSE;     // not written by retail (see above)
    self->m_bDisableUpdateTabs = FALSE;
    ListInit(self->m_lstRemovedTabbedGroups);
    self->m_rectNewTabGroup.left = self->m_rectNewTabGroup.top = 0;
    self->m_rectNewTabGroup.right = self->m_rectNewTabGroup.bottom = 0;
    ListInit(self->m_lstLoadedTabDocuments);
    self->m_bTabIsVisible = FALSE;
    self->m_bTabIsEnabled = FALSE;
    self->m_bLastActiveTab = FALSE;
    self->m_groupAlignment = kGroupNoAlign;
    self->m_bInsideDragComplete = FALSE;
    self->m_nResizeMargin = 40;
    self->m_nNewGroupMargin = 40;
    self->pad_3e5c = 0;
    self->pad_3f54 = 0;
    self->pad_3fac = 0;
    ::SetRectEmpty(&self->m_rectNewTabGroup);
    self->m_nTotalResizeRest = 0;

    RegisterInstance(pThis);
    return pThis;
}

// CMDIClientAreaWnd::~CMDIClientAreaWnd — transcribed from retail entry RVA
// 0x7cb20 (mfc140u):
//     vftable = 0x2ed5a8;
//     while (m_lstTabbedGroups.m_nCount != 0) {
//         CObject* p = m_lstTabbedGroups.RemoveTail();        // 0x231fb0
//         if (p) delete p;                                     // vtable slot 1, flag 1
//     }
//     while (m_lstRemovedTabbedGroups.m_nCount != 0) {
//         CObject* p = RemoveTail();
//         if (p && p->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl))) delete p;   // 0x234cf0 / 0x312be0
//     }
//     if (m_mapTabIcons.m_nCount) { for each assoc: if (value) delete value; m_mapTabIcons.RemoveAll(); }
//     ~CStringList(&m_lstLoadedTabDocuments);   ~CObList(&m_lstRemovedTabbedGroups);
//     ~CMap(&m_mapTabIcons);  ~CObList(&m_lstTabbedGroups);  ~CMap(&m_mapIcons);
//     ~CImageList(&m_TabIcons) (0x296be0);  ~CMFCTabCtrl(&m_wndTab) (0x137030);
//     tail-jump ~CWnd() (0x28b740).
// (The lists' ~CObList only frees CPlex blocks, which is ListRemoveAll here.)
// Deviation: an instance this file never constructed (file header) is left
// untouched -- there is nothing of ours to tear down and its bytes are
// unknown.  Groups are deleted through the CMFCTabCtrl destructor thunk, not
// through a vtable slot (DeleteTabCtrlObject).
// Symbol: ??1CMDIClientAreaWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CMDIClientAreaWnd__UEAA_XZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    InstanceRecord* rec = Record(pThis);

    while (self->m_lstTabbedGroups.nCount != 0) {
        void* p = ListRemoveTail(self->m_lstTabbedGroups);
        if (p != nullptr) {
            DeleteTabCtrlObject(p);
        }
    }
    while (self->m_lstRemovedTabbedGroups.nCount != 0) {
        void* p = ListRemoveTail(self->m_lstRemovedTabbedGroups);
        if (p != nullptr && AsTabCtrl(p) != nullptr) {
            DeleteTabCtrlObject(p);
        }
    }
    if (self->m_mapTabIcons.nCount != 0) {
        for (TabIconEntry* e = rec != nullptr ? rec->pTabIcons : nullptr; e != nullptr; e = e->pNext) {
            DeleteImageListObject(e->value);
            e->value = nullptr;
        }
        TabIconsRemoveAll(rec, self);
    }
    ListRemoveAll(self->m_lstLoadedTabDocuments);
    ListRemoveAll(self->m_lstRemovedTabbedGroups);
    TabIconsRemoveAll(rec, self);
    ListRemoveAll(self->m_lstTabbedGroups);
    self->m_mapIcons.nCount = 0;   // never populated here (UpdateTabs is a stub)
    impl___1CImageList__UEAA_XZ(reinterpret_cast<CImageList*>(self->m_TabIcons));
    impl___1CMFCTabCtrl__UEAA_XZ(self->m_wndTab);

    UnregisterInstance(pThis);
    impl___1CWnd__UEAA_XZ(pThis);
}
// Symbol: ?GetThisClass@CMDIClientAreaWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIClientAreaWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_cmdiClientAreaRuntimeClass;
}
// Symbol: ?GetRuntimeClass@CMDIClientAreaWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIClientAreaWnd__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    return pThis ? &g_cmdiClientAreaRuntimeClass : nullptr;
}
// CMDIClientAreaWnd::OnUpdateTabs — retail entry RVA 0x7ec90 (mfc140). The
// whole body is
//     sub $0x28,%rsp ; xor %edx,%edx ; call 0x7efa0 ; xor %eax,%eax ; ret
// i.e. `UpdateTabs(FALSE); return 0;` — %rcx (this) is passed through
// untouched and no member is read.
// Symbol: ?OnUpdateTabs@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnUpdateTabs_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(pThis, FALSE);
    return 0;
}
namespace {
constexpr UINT_PTR clientSubclassId = 1;
constexpr UINT_PTR documentSubclassId = 2;
constexpr UINT_PTR groupTabSubclassId = 3;
extern "C" unsigned int impl__AFX_WM_CHANGE_ACTIVE_TAB__3IA;
extern "C" unsigned int impl__AFX_WM_ON_CANCELTABMOVE__3IA;
extern "C" unsigned int impl__AFX_WM_ON_TABGROUPMOUSEMOVE__3IA;
extern "C" unsigned int impl__AFX_WM_ON_MOVETABCOMPLETE__3IA;
extern "C" __int64 MS_ABI impl__OnCancelTabMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void*, unsigned __int64, __int64);
extern "C" __int64 MS_ABI impl__OnTabGroupMouseMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void*, unsigned __int64, __int64);
extern "C" __int64 MS_ABI impl__OnMoveTabComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(void*, unsigned __int64, __int64);
extern "C" std::uint32_t impl__m_dwExcludeStyle_CMDIChildWndEx__2KA;

UINT RefreshDocumentTabsMessage() {
    static const UINT message = ::RegisterWindowMessageW(L"OpenMFC.MDI.RefreshDocumentTabs");
    return message;
}

void PublishGroupList(S_CMDIClientAreaWnd& self) {
    auto& groups = Record(&self)->runtime->groups;
    groups.RemoveAll();
    for (auto* node = self.m_lstTabbedGroups.pNodeHead; node; node = node->pNext)
        groups.AddTail(static_cast<CMFCTabCtrl*>(node->data));
}

void CancelTabDrag(S_CMDIClientAreaWnd& self, bool restoreFocus = true);

void RemoveDocumentTab(S_CMDIClientAreaWnd& self, HWND window) {
    if (Record(&self)->runtime->drag.document == window) CancelTabDrag(self, false);
    auto* tabs = EmbeddedTab(&self);
    const int index = TabFromHwnd(tabs, window);
    if (index >= 0) tabs->RemoveTab(index);
    int groupIndex = -1;
    if (auto* group = static_cast<CMFCTabCtrl*>(impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(
            &self, window, &groupIndex))) {
        if (auto* child = AsChildEx(group->GetTabWnd(groupIndex))) SetChildRelatedTabGroup(child, nullptr);
        group->RemoveTab(groupIndex);
    }
}

LRESULT CALLBACK DocumentSubclass(HWND window, UINT message, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR data) {
    auto* self = reinterpret_cast<S_CMDIClientAreaWnd*>(data);
    HWND client = WndHandle(self);
    if (message == WM_NCDESTROY) {
        ::RemoveWindowSubclass(window, DocumentSubclass, documentSubclassId);
        RemoveDocumentTab(*self, window);
        auto& placements = Record(self)->runtime->placements;
        placements.erase(std::remove_if(placements.begin(), placements.end(),
            [window](const DocumentPlacement& placement) { return placement.window == window; }), placements.end());
    }
    const auto result = ::DefSubclassProc(window, message, wp, lp);
    if (message == WM_SETTEXT || message == WM_EXITSIZEMOVE || message == WM_SETICON || message == WM_SHOWWINDOW ||
        (message == WM_WINDOWPOSCHANGED &&
            (reinterpret_cast<const WINDOWPOS*>(lp)->flags & (SWP_SHOWWINDOW | SWP_HIDEWINDOW))) ||
        message == WM_NCDESTROY)
        ::PostMessageW(client, RefreshDocumentTabsMessage(), 0, 0);
    return result;
}

void FitTabDocuments(S_CMDIClientAreaWnd& self, CMFCTabCtrl& tabs) {
    if (!tabs.m_hWnd) return;
    tabs.RecalcLayout();
    const HWND client = WndHandle(&self);
    RECT page = *static_cast<const RECT*>(openmfc::detail::mfccore::EnsureTabCtrlState(&tabs).page);
    ::MapWindowPoints(tabs.m_hWnd, client, reinterpret_cast<POINT*>(&page), 2);
    auto& placements = Record(&self)->runtime->placements;
    for (int index = 0; index < tabs.GetTabsCount(); ++index) {
        const HWND window = tabs.GetTabWnd(index)->m_hWnd;
        if (::GetParent(window) != client) continue;
        auto position = std::find_if(placements.begin(), placements.end(),
            [window](const DocumentPlacement& saved) { return saved.window == window; });
        if (position == placements.end()) {
            DocumentPlacement saved{window, {sizeof(WINDOWPLACEMENT)}};
            if (!::GetWindowPlacement(window, &saved.placement)) continue;
            placements.push_back(saved);
            position = placements.end() - 1;
        }
        if (::IsIconic(window) || ::IsZoomed(window))
            ::SendMessageW(client, WM_MDIRESTORE, reinterpret_cast<WPARAM>(window), 0);
        UINT flags = SWP_NOACTIVATE | SWP_NOZORDER;
        if (self.m_bIsMDITabbedGroup) {
            const LONG_PTR style = ::GetWindowLongPtrW(window, GWL_STYLE);
            const LONG_PTR excluded = style & (impl__m_dwExcludeStyle_CMDIChildWndEx__2KA | WS_SYSMENU);
            if (excluded) {
                position->removedStyle |= excluded;
                ::SetWindowLongPtrW(window, GWL_STYLE, style & ~excluded);
                flags |= SWP_FRAMECHANGED;
            }
        }
        ::SetWindowPos(window, nullptr, page.left, page.top, page.right - page.left, page.bottom - page.top, flags);
    }
    auto* selected = tabs.GetTabWnd(tabs.GetActiveTab());
    if (selected && (self.m_bIsMDITabbedGroup ||
        reinterpret_cast<HWND>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0)) == selected->m_hWnd))
        ::SetWindowPos(selected->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

void LayoutDocumentTabs(S_CMDIClientAreaWnd& self) {
    auto* tabs = EmbeddedTab(&self);
    if (!tabs->m_hWnd) return;
    const HWND client = WndHandle(&self);
    const bool visible = self.m_bTabIsVisible && self.m_bTabIsEnabled && tabs->GetTabsCount() > 0;
    ::ShowWindow(tabs->m_hWnd, visible ? SW_SHOWNOACTIVATE : SW_HIDE);
    if (!visible) return;
    RECT bounds{};
    if (!::GetClientRect(client, &bounds)) return;
    ::SetWindowPos(tabs->m_hWnd, HWND_BOTTOM, 0, 0, bounds.right, bounds.bottom, SWP_NOACTIVATE);
    FitTabDocuments(self, *tabs);
}

constexpr int groupGap = 6;

void LayoutDocumentGroups(S_CMDIClientAreaWnd& self) {
    if (self.m_bInsideDragComplete || !self.m_bIsMDITabbedGroup || !self.m_bTabIsVisible) return;
    const HWND client = WndHandle(&self);
    RECT bounds{};
    if (!::GetClientRect(client, &bounds)) return;

    const bool vertical = self.m_groupAlignment == kGroupVertAlign;
    const int full = vertical ? bounds.right : bounds.bottom;
    struct GroupSize { CMFCTabCtrl* tabs; int length; };
    std::vector<GroupSize> groups;
    int total = 0;
    for (auto* node = self.m_lstTabbedGroups.pNodeHead; node; node = node->pNext) {
        auto* tabs = static_cast<CMFCTabCtrl*>(node->data);
        RECT rect{};
        if (!::GetWindowRect(tabs->m_hWnd, &rect)) continue;
        const int length = vertical ? rect.right - rect.left : rect.bottom - rect.top;
        groups.push_back({tabs, length});
        total += length;
    }
    const int count = static_cast<int>(groups.size());
    if (count == 0) return;
    const int gap = (std::min)(groupGap, full / count);
    int available = full - (count - 1) * gap;
    const int minimum = (std::min)(self.m_nResizeMargin, available / count);
    int offset = 0;
    for (int i = 0; i < count; ++i) {
        const auto& group = groups[i];
        const int remaining = count - i;
        const int desired = total > 0 ? ::MulDiv(available, group.length, total) : available / remaining;
        const int length = remaining == 1 ? available :
            (std::clamp)(desired, minimum, available - minimum * (remaining - 1));
        ::SetWindowPos(group.tabs->m_hWnd, HWND_BOTTOM, vertical ? offset : 0, vertical ? 0 : offset,
            vertical ? length : bounds.right, vertical ? bounds.bottom : length,
            SWP_NOACTIVATE | SWP_SHOWWINDOW);
        FitTabDocuments(self, *group.tabs);
        offset += length + gap;
        available -= length;
        total -= group.length;
    }
}

void RestoreDocumentPlacements(S_CMDIClientAreaWnd& self) {
    auto placements = std::move(Record(&self)->runtime->placements);
    if (placements.empty()) return;
    const HWND client = WndHandle(&self);
    const WPARAM active = static_cast<WPARAM>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0));
    for (const auto& p : placements) {
        if (::GetParent(p.window) != client) continue;
        if (p.removedStyle) {
            const LONG_PTR style = ::GetWindowLongPtrW(p.window, GWL_STYLE);
            ::SetWindowLongPtrW(p.window, GWL_STYLE, style | p.removedStyle);
            ::SetWindowPos(p.window, nullptr, 0, 0, 0, 0,
                SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        }
        ::SetWindowPlacement(p.window, &p.placement);
    }
    if (active) ::SendMessageW(client, WM_MDIACTIVATE, active, 0);
}

bool IsVerticalSplit(const S_CMDIClientAreaWnd& self) {
    return self.m_groupAlignment == kGroupVertAlign;
}

RECT GroupBounds(const S_CMDIClientAreaWnd& self, const CMFCTabCtrl& tabs) {
    RECT bounds{};
    ::GetWindowRect(tabs.m_hWnd, &bounds);
    ::MapWindowPoints(nullptr, WndHandle(&self), reinterpret_cast<POINT*>(&bounds), 2);
    return bounds;
}

CMFCTabCtrl* SplitterAt(S_CMDIClientAreaWnd& self, POINT point) {
    const bool vertical = IsVerticalSplit(self);
    for (auto* node = self.m_lstTabbedGroups.pNodeHead; node && node->pNext; node = node->pNext) {
        auto* tabs = static_cast<CMFCTabCtrl*>(node->data);
        auto* next = static_cast<CMFCTabCtrl*>(node->pNext->data);
        auto bounds = GroupBounds(self, *tabs);
        const auto adjacent = GroupBounds(self, *next);
        if (vertical) {
            bounds.left = bounds.right;
            bounds.right = adjacent.left;
        } else {
            bounds.top = bounds.bottom;
            bounds.bottom = adjacent.top;
        }
        if (::PtInRect(&bounds, point)) return tabs;
    }
    return nullptr;
}

bool ResizeAdjacentGroups(S_CMDIClientAreaWnd& self, CMFCTabCtrl& first, int split) {
    auto* node = ListFind(self.m_lstTabbedGroups, &first);
    if (!node || !node->pNext) return false;
    auto* second = static_cast<CMFCTabCtrl*>(node->pNext->data);
    RECT a = GroupBounds(self, first);
    RECT b = GroupBounds(self, *second);
    const bool vertical = IsVerticalSplit(self);
    const int gap = vertical ? b.left - a.right : b.top - a.bottom;
    const int start = vertical ? a.left : a.top;
    const int end = vertical ? b.right : b.bottom;
    const int minimum = (std::min)(self.m_nResizeMargin, (end - start - gap) / 2);
    if (minimum <= 0) return false;
    split = (std::clamp)(split, start + minimum, end - gap - minimum);
    if (vertical) {
        a.right = split;
        b.left = split + gap;
    } else {
        a.bottom = split;
        b.top = split + gap;
    }
    self.m_bInsideDragComplete = TRUE;
    ::SetWindowPos(first.m_hWnd, nullptr, a.left, a.top, a.right - a.left, a.bottom - a.top,
        SWP_NOACTIVATE | SWP_NOZORDER);
    ::SetWindowPos(second->m_hWnd, nullptr, b.left, b.top, b.right - b.left, b.bottom - b.top,
        SWP_NOACTIVATE | SWP_NOZORDER);
    FitTabDocuments(self, first);
    FitTabDocuments(self, *second);
    self.m_bInsideDragComplete = FALSE;
    return true;
}

void CancelSplitterDrag(S_CMDIClientAreaWnd& self) {
    auto& runtime = *Record(&self)->runtime;
    if (!runtime.resizingGroup) return;
    runtime.resizingGroup = nullptr;
    if (::GetCapture() == WndHandle(&self)) ::ReleaseCapture();
}

bool HandleGroupSplitter(S_CMDIClientAreaWnd& self, HWND window, UINT message, LPARAM lp) {
    auto& runtime = *Record(&self)->runtime;
    if (message == WM_CAPTURECHANGED) runtime.resizingGroup = nullptr;
    if (!self.m_bIsMDITabbedGroup || !self.m_bTabIsVisible) return false;
    const bool vertical = IsVerticalSplit(self);
    if (message == WM_SETCURSOR && LOWORD(lp) == HTCLIENT) {
        POINT point{};
        ::GetCursorPos(&point);
        ::ScreenToClient(window, &point);
        if (runtime.resizingGroup || SplitterAt(self, point)) {
            ::SetCursor(::LoadCursorW(nullptr, vertical ? IDC_SIZEWE : IDC_SIZENS));
            return true;
        }
    }
    if (message == WM_LBUTTONDOWN) {
        const POINT point{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
        if (auto* tabs = SplitterAt(self, point)) {
            const auto bounds = GroupBounds(self, *tabs);
            runtime.resizingGroup = tabs->m_hWnd;
            runtime.resizeOffset = vertical ? point.x - bounds.right : point.y - bounds.bottom;
            ::SetCapture(window);
            return true;
        }
    }
    if (!runtime.resizingGroup || (message != WM_MOUSEMOVE && message != WM_LBUTTONUP)) return false;
    auto* tabs = AsTabCtrl(CWnd::FromHandlePermanent(runtime.resizingGroup));
    if (tabs) {
        const int coordinate = vertical ? GET_X_LPARAM(lp) : GET_Y_LPARAM(lp);
        ResizeAdjacentGroups(self, *tabs, coordinate - runtime.resizeOffset);
    }
    if (!tabs || message == WM_LBUTTONUP) CancelSplitterDrag(self);
    return true;
}

bool ActivateAdjacentTab(S_CMDIClientAreaWnd& self, HWND window, bool previous) {
    const HWND client = WndHandle(&self);
    if (!window) window = reinterpret_cast<HWND>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0));
    int index = -1;
    auto* group = impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(
        &self, window, &index);
    auto* node = ListFind(self.m_lstTabbedGroups, group);
    if (!node) return false;

    auto* tabs = static_cast<CMFCTabCtrl*>(group);
    index += previous ? -1 : 1;
    if (index < 0 || index >= tabs->GetTabsCount()) {
        node = previous ? node->pPrev : node->pNext;
        if (!node) node = previous ? self.m_lstTabbedGroups.pNodeTail : self.m_lstTabbedGroups.pNodeHead;
        tabs = static_cast<CMFCTabCtrl*>(node->data);
        index = previous ? tabs->GetTabsCount() - 1 : 0;
    }
    if (auto* next = tabs->GetTabWnd(index)) {
        ::SendMessageW(client, WM_MDIACTIVATE, reinterpret_cast<WPARAM>(next->m_hWnd), 0);
        return true;
    }
    return false;
}

enum class TabDropAction { none, reorder, join, split };

struct TabDrop {
    TabDropAction action = TabDropAction::none;
    CMFCTabCtrl* target = nullptr;
    RECT outline{};
    int index = -1;
    bool vertical = false;
};

CMFCTabCtrl* GroupTabWindow(S_CMDIClientAreaWnd& client, HWND window) {
    auto* tabs = AsTabCtrl(CWnd::FromHandlePermanent(window));
    return tabs && ListFind(client.m_lstTabbedGroups, tabs) ? tabs : nullptr;
}

int FindTabAt(const openmfc::detail::mfccore::TabCtrlState& state, POINT point) {
    for (int i = 0; i < static_cast<int>(state.tabs.size()); ++i) {
        if (::PtInRect(static_cast<const RECT*>(state.tabs[i].rect), point)) return i;
    }
    return -1;
}

TabDrop FindTabDrop(S_CMDIClientAreaWnd& client, CMFCTabCtrl& source, int sourceIndex, POINT screen) {
    using namespace openmfc::detail::mfccore;
    if (sourceIndex < 0) return {};
    auto* target = static_cast<CMFCTabCtrl*>(
        impl__TabWndFromPoint_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__VCPoint___Z(&client, screen));
    const auto* state = target ? FindTabCtrlState(target) : nullptr;
    if (!state) return {};

    POINT cursor = screen;
    ::ScreenToClient(target->m_hWnd, &cursor);
    const bool inStrip = ::PtInRect(static_cast<const RECT*>(state->strip), cursor);
    if (inStrip && target == &source) {
        const int index = FindTabAt(*state, cursor);
        if (!state->tabSwapEnabled || index < 0 || index == sourceIndex) return {};
        RECT marker = *static_cast<const RECT*>(state->tabs[index].rect);
        if (index > sourceIndex) marker.left = marker.right - 4;
        else marker.right = marker.left + 4;
        ::MapWindowPoints(target->m_hWnd, nullptr, reinterpret_cast<POINT*>(&marker), 2);
        return {TabDropAction::reorder, target, marker, index};
    }

    if (!source.IsTabDetachable(sourceIndex)) return {};

    RECT bounds{};
    ::GetWindowRect(target->m_hWnd, &bounds);
    const int right = bounds.right - screen.x;
    const int bottom = bounds.bottom - screen.y;
    const bool vertical = client.m_groupAlignment == kGroupNoAlign ? right < bottom : IsVerticalSplit(client);
    const bool nearEdge = (vertical ? right : bottom) <= client.m_nNewGroupMargin;
    if (!inStrip && (target != &source || source.GetTabsCount() > 1) && nearEdge) {
        const int span = vertical ? bounds.right - bounds.left : bounds.bottom - bounds.top;
        if (span >= 2 * client.m_nResizeMargin + groupGap) {
            if (vertical) bounds.left += span / 2;
            else bounds.top += span / 2;
            return {TabDropAction::split, target, bounds, -1, vertical};
        }
    }
    if (target == &source) return {};

    RECT page = *static_cast<const RECT*>(state->page);
    ::MapWindowPoints(target->m_hWnd, nullptr, reinterpret_cast<POINT*>(&page), 2);
    return {TabDropAction::join, target, page};
}

void PreviewTabDrop(S_CMDIClientAreaWnd& client, const RECT& outline) {
    if (::EqualRect(&outline, &client.m_rectNewTabGroup)) return;
    impl__DrawNewGroupRect_CMDIClientAreaWnd__AEAAXPEBUtagRECT__0_Z(&client, &outline, &client.m_rectNewTabGroup);
    client.m_rectNewTabGroup = outline;
}

void CancelTabDrag(S_CMDIClientAreaWnd& self, bool restoreFocus) {
    auto& drag = Record(&self)->runtime->drag;
    const TabDrag previous = drag;
    drag = {};
    PreviewTabDrop(self, RECT{});
    if (previous.source && ::GetCapture() == previous.source) ::ReleaseCapture();
    if (restoreFocus && ::GetFocus() == previous.source && ::IsWindow(previous.previousFocus))
        ::SetFocus(previous.previousFocus);
}

void ReorderGroupTab(CMFCTabCtrl& tabs, int from, int to) {
    auto& state = openmfc::detail::mfccore::EnsureTabCtrlState(&tabs);
    const auto* active = tabs.GetTabWnd(tabs.GetActiveTab());
    auto first = state.tabs.begin();
    if (from < to) std::rotate(first + from, first + from + 1, first + to + 1);
    else std::rotate(first + to, first + from, first + from + 1);
    const auto selected = std::find_if(state.tabs.begin(), state.tabs.end(),
        [active](const auto& tab) { return tab.window == active; });
    tabs.SetActiveTab(selected == state.tabs.end() ? -1 : static_cast<int>(selected - first));
}

LRESULT CALLBACK GroupTabSubclass(HWND window, UINT message, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR data) {
    using namespace openmfc::detail::mfccore;
    auto* self = reinterpret_cast<S_CMDIClientAreaWnd*>(data);
    auto& drag = Record(self)->runtime->drag;
    if (message == WM_NCDESTROY) {
        ::RemoveWindowSubclass(window, GroupTabSubclass, groupTabSubclassId);
        if (drag.source == window) CancelTabDrag(*self, false);
        return ::DefSubclassProc(window, message, wp, lp);
    }
    if (drag.source == window &&
        (message == WM_CAPTURECHANGED || message == WM_CANCELMODE ||
            (message == WM_KEYDOWN && wp == VK_ESCAPE))) {
        CancelTabDrag(*self);
        return message == WM_KEYDOWN ? 0 : ::DefSubclassProc(window, message, wp, lp);
    }

    if (message != WM_LBUTTONDOWN && drag.source != window)
        return ::DefSubclassProc(window, message, wp, lp);
    auto* tabs = GroupTabWindow(*self, window);
    if (!tabs || !self->m_bIsMDITabbedGroup || !self->m_bTabIsVisible)
        return ::DefSubclassProc(window, message, wp, lp);

    POINT point{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
    if (message == WM_LBUTTONDOWN) {
        const auto* state = FindTabCtrlState(tabs);
        const RECT close = state ? TabCloseButtonBounds(*state) : RECT{};
        const int index = state && !::PtInRect(&close, point) ? FindTabAt(*state, point) : -1;
        auto* child = tabs->GetTabWnd(index);
        const HWND document = child ? child->m_hWnd : nullptr;
        ::ClientToScreen(window, &point);

        const auto result = ::DefSubclassProc(window, message, wp, lp);
        DWORD_PTR current = 0;
        if (document && ::GetWindowSubclass(window, GroupTabSubclass, groupTabSubclassId, &current) && current == data &&
            self->m_bIsMDITabbedGroup && self->m_bTabIsVisible && GroupTabWindow(*self, window) == tabs &&
            tabs->GetTabWnd(tabs->GetActiveTab()) == CWnd::FromHandlePermanent(document)) {
            drag = {window, document, point};
            ::SetCapture(window);
            if (::GetCapture() != window) drag = {};
        }
        return result;
    }
    if (message == WM_MOUSEMOVE) {
        if (!(wp & MK_LBUTTON)) {
            CancelTabDrag(*self);
            return 0;
        }
        ::ClientToScreen(window, &point);
        if (!drag.started) {
            const int dx = std::abs(point.x - drag.origin.x);
            const int dy = std::abs(point.y - drag.origin.y);
            if (dx < ::GetSystemMetrics(SM_CXDRAG) && dy < ::GetSystemMetrics(SM_CYDRAG)) return 0;
            drag.started = true;
            drag.previousFocus = ::GetFocus();
            ::SetFocus(window);
        }
        if (::GetCapture() == window)
            impl__OnTabGroupMouseMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(self, 0, lp);
        return 0;
    }
    if (message == WM_LBUTTONUP) {
        if (drag.started)
            impl__OnMoveTabComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(self, reinterpret_cast<WPARAM>(tabs), lp);
        else CancelTabDrag(*self);
        return 0;
    }
    return ::DefSubclassProc(window, message, wp, lp);
}

LRESULT CALLBACK ClientSubclass(HWND window, UINT message, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR data) {
    auto* self = reinterpret_cast<S_CMDIClientAreaWnd*>(data);
    if (message == impl__AFX_WM_ON_TABGROUPMOUSEMOVE__3IA)
        return impl__OnTabGroupMouseMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(self, wp, lp);
    if (message == impl__AFX_WM_ON_MOVETABCOMPLETE__3IA)
        return impl__OnMoveTabComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(self, wp, lp);
    if (message == impl__AFX_WM_ON_CANCELTABMOVE__3IA)
        return impl__OnCancelTabMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(self, wp, lp);
    if (HandleGroupSplitter(*self, window, message, lp)) return 1;
    if (message == WM_MDINEXT && self->m_bIsMDITabbedGroup && self->m_bTabIsVisible &&
        ActivateAdjacentTab(*self, reinterpret_cast<HWND>(wp), lp != 0)) return 0;
    if (message == impl__AFX_WM_CHANGE_ACTIVE_TAB__3IA) {
        auto* tabs = reinterpret_cast<CMFCTabCtrl*>(lp);
        if ((self->m_bTabIsEnabled && tabs == EmbeddedTab(self)) ||
            (self->m_bIsMDITabbedGroup && ListFind(self->m_lstTabbedGroups, tabs))) {
            if (auto* child = tabs->GetTabWnd(static_cast<int>(wp)))
                ::SendMessageW(window, WM_MDIACTIVATE, reinterpret_cast<WPARAM>(child->m_hWnd), 0);
            return 0;
        }
    }
    if (message == WM_NCDESTROY) {
        CancelTabDrag(*self, false);
        ::RemoveWindowSubclass(window, ClientSubclass, clientSubclassId);
        SelfWnd(self)->Detach();
        return ::DefSubclassProc(window, message, wp, lp);
    }
    if (message == WM_MDIDESTROY) RemoveDocumentTab(*self, reinterpret_cast<HWND>(wp));
    const auto result = ::DefSubclassProc(window, message, wp, lp);
    DWORD_PTR boundArea = 0;
    if (!::GetWindowSubclass(window, ClientSubclass, clientSubclassId, &boundArea) || boundArea != data)
        return result;
    switch (message) {
    case WM_SIZE:
    case WM_MDIACTIVATE:
    case WM_MDICREATE:
    case WM_MDIDESTROY:
    case WM_MDINEXT:
    case WM_MDIRESTORE:
    case WM_MDIMAXIMIZE:
    case WM_MDITILE:
    case WM_MDICASCADE:
        impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(self, TRUE);
        break;
    default:
        if (message == RefreshDocumentTabsMessage()) impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(self, FALSE);
        break;
    }
    return result;
}
}

void* openmfc::detail::mdi::CreateClientArea() {
    auto* area = new S_CMDIClientAreaWnd{};
    impl___0CMDIClientAreaWnd__QEAA_XZ(area);
    return area;
}

void openmfc::detail::mdi::DeleteClientArea(void* area) {
    auto* self = Self(area);
    if (!self) return;
    if (HWND client = WndHandle(self)) {
        CancelTabDrag(*self, false);
        ::RemoveWindowSubclass(client, ClientSubclass, clientSubclassId);
        for (HWND window = ::GetWindow(client, GW_CHILD); window; window = ::GetWindow(window, GW_HWNDNEXT)) {
            ::RemoveWindowSubclass(window, DocumentSubclass, documentSubclassId);
            ::RemoveWindowSubclass(window, GroupTabSubclass, groupTabSubclassId);
        }
        SelfWnd(self)->Detach();
    }
    impl___1CMDIClientAreaWnd__UEAA_XZ(self);
    delete self;
}

bool openmfc::detail::mdi::AttachClientArea(void* area, HWND window) {
    auto* self = Self(area);
    if (!self || !::IsWindow(window)) return false;
    if (WndHandle(self) == window) return true;
    if (WndHandle(self) || !SelfWnd(self)->Attach(window)) return false;
    if (!::SetWindowSubclass(window, ClientSubclass, clientSubclassId, reinterpret_cast<DWORD_PTR>(self))) {
        SelfWnd(self)->Detach();
        return false;
    }
    impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(self, FALSE);
    return true;
}

int openmfc::detail::mdi::TabMode(void* area) {
    const auto* self = Self(area);
    if (!self) return 0;
    return self->m_bTabIsEnabled ? 1 : self->m_bIsMDITabbedGroup ? 2 : 0;
}

CMFCTabCtrl& openmfc::detail::mdi::Tabs(void* area) {
    return *EmbeddedTab(Self(area));
}

const CObList& openmfc::detail::mdi::Groups(void* area) {
    if (auto* rec = Record(area)) return rec->runtime->groups;
    static const CObList empty;
    return empty;
}

void openmfc::detail::mdi::SetTabsVisible(void* area, bool visible) {
    auto* self = Self(area);
    if (!self || (!self->m_bTabIsEnabled && !self->m_bIsMDITabbedGroup)) return;
    self->m_bTabIsVisible = visible;
    if (visible) {
        impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(self, FALSE);
        return;
    }
    CancelTabDrag(*self);
    CancelSplitterDrag(*self);
    LayoutDocumentTabs(*self);
    for (auto* node = self->m_lstTabbedGroups.pNodeHead; node; node = node->pNext)
        ::ShowWindow(WndHandle(node->data), SW_HIDE);
    RestoreDocumentPlacements(*self);
}

namespace {
class TabUpdate {
public:
    explicit TabUpdate(ClientRuntime& state) : m_state(state) { m_state.updating = true; }
    ~TabUpdate() { m_state.updating = false; }
private:
    ClientRuntime& m_state;
};

std::vector<CMDIChildWndEx*> CollectTabbedDocuments(S_CMDIClientAreaWnd& self) {
    std::vector<CMDIChildWndEx*> children;
    for (HWND window = ::GetWindow(WndHandle(&self), GW_CHILD); window; window = ::GetWindow(window, GW_HWNDNEXT)) {
        auto* child = AsChildEx(static_cast<CWnd*>(CWnd::FromHandlePermanent(window)));
        if (!child) continue;
        ::SetWindowSubclass(window, DocumentSubclass, documentSubclassId, reinterpret_cast<DWORD_PTR>(&self));
        if (impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(child)) children.push_back(child);
    }
    return children;
}

void RemoveMissingTabs(CMFCTabCtrl& tabs, const std::vector<CMDIChildWndEx*>& children, bool grouped) {
    for (int i = tabs.GetTabsCount() - 1; i >= 0; --i) {
        auto* window = tabs.GetTabWnd(i);
        if (std::find(children.begin(), children.end(), window) != children.end()) continue;
        if (grouped) {
            if (auto* child = AsChildEx(window)) SetChildRelatedTabGroup(child, nullptr);
        }
        tabs.RemoveTab(i);
    }
}

void SyncDocumentTab(const S_CMDIClientAreaWnd& self, CMFCTabCtrl& tabs, CMDIChildWndEx& child) {
    int index = TabFromHwnd(&tabs, child.m_hWnd);
    if (index < 0) {
        tabs.AddTab(&child, nullptr, UINT(-1), self.m_bIsMDITabbedGroup);
        index = TabFromHwnd(&tabs, child.m_hWnd);
    }
    if (index < 0) return;
    std::wstring label(::GetWindowTextLengthW(child.m_hWnd) + 1, L'\0');
    label.resize(::GetWindowTextW(child.m_hWnd, label.data(), static_cast<int>(label.size())));
    HICON icon = nullptr;
    if (self.m_mdiTabParams.m_bTabIcons) {
        icon = reinterpret_cast<HICON>(::SendMessageW(child.m_hWnd, WM_GETICON, ICON_SMALL, 0));
        if (!icon) icon = reinterpret_cast<HICON>(::GetClassLongPtrW(child.m_hWnd, GCLP_HICONSM));
    }
    auto& tab = openmfc::detail::mfccore::EnsureTabCtrlState(&tabs).tabs[index];
    tab.label = std::move(label);
    tab.windowIcon = icon;
}
}

// Symbol: ?UpdateTabs@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int visible) {
    auto* self = Self(pThis);
    if (!self || self->m_bDisableUpdateTabs) return;
    if (self->m_bIsMDITabbedGroup) {
        impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(self, visible);
        return;
    }
    const HWND client = WndHandle(self);
    if (!client || !self->m_bTabIsEnabled || !self->m_bTabIsVisible) return;
    auto& runtime = *Record(self)->runtime;
    if (runtime.updating) return;
    TabUpdate update(runtime);

    auto* tabs = EmbeddedTab(self);
    if (!tabs->m_hWnd) {
        const auto& params = self->m_mdiTabParams;
        if (!tabs->CreateTabs(params.m_style, RECT{}, SelfWnd(self), UINT(-1),
                params.m_tabLocation, params.m_bTabCloseButton)) return;
        impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(self, tabs);
    }
    auto children = CollectTabbedDocuments(*self);
    RemoveMissingTabs(*tabs, children, false);
    for (auto* child : children) SyncDocumentTab(*self, *tabs, *child);
    const HWND active = reinterpret_cast<HWND>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0));
    const int index = TabFromHwnd(tabs, active);
    if (index >= 0 && index != tabs->GetActiveTab()) tabs->SetActiveTab(index);
    LayoutDocumentTabs(*self);
}

// CMDIClientAreaWnd::SetTaskbarTabOrder — transcribed from retail entry RVA
// 0x7e7d0 (mfc140):
//     for (node = m_lstTabbedGroups.head; node; node = node->pNext) {
//         CMFCTabCtrl* pGroup = DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data);
//         for (int i = 0; i < pGroup->GetTabsNum(); i++) {         // vslot +0x368
//             CMDIChildWndEx* pChild = DYNAMIC_DOWNCAST(CMDIChildWndEx, pGroup->GetTabWnd(i));  // +0x370
//             if (pChild && pChild->m_hWnd && pChild->IsTaskbarTabsSupportEnabled()   // 0x84590
//                 && pChild->m_tabProxyWnd.m_hWnd)      // `lea 0x720(%rsi)` is never NULL; the real test is +0x760
//                 pChild->SetTaskbarTabOrder(NULL);      // 0x847b0
//         }
//     }
// Retail calls GetTabsNum through a NULL group when the downcast fails; the
// NULL is skipped here instead.
// Symbol: ?SetTaskbarTabOrder@CMDIClientAreaWnd@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIClientAreaWnd__QEAAXXZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (pGroup == nullptr) {
            continue;
        }
        for (int i = 0; i < TabsNum(pGroup); ++i) {
            CMDIChildWndEx* pChild = AsChildEx(TabWnd(pGroup, i));
            if (pChild != nullptr && WndHandle(pChild) != nullptr
                && impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pChild)
                && ChildTabProxyHwnd(pChild) != nullptr) {
                impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(pChild, nullptr);
            }
        }
    }
}
// Symbol: ?AdjustMDIChildren@CMDIClientAreaWnd@@AEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    auto* self = Self(pThis);
    if (self && self->m_bTabIsVisible && pTabGroup)
        FitTabDocuments(*self, *static_cast<CMFCTabCtrl*>(pTabGroup));
}

// CMDIClientAreaWnd::ApplyParams — retail entry RVA 0x7d1b0 (mfc140u), which
// pushes m_mdiTabParams onto one tab group:
//     pTab->ModifyTabStyle(m_style)                     (0x13c380)
//     pTab->SetLocation(m_tabLocation)                  (vslot +0x500)
//     pTab->m_bCloseBtn (+0x3d8) = m_bTabCloseButton
//     pTab->m_bActiveTabCloseButton (+0x3fc) = m_bActiveTabCloseButton
//     pTab->EnableTabDocumentsMenu(m_bDocumentMenu)     (0x13c7b0)
//     pTab->EnableAutoColor(m_bAutoColor)               (0x1a190)
//     pTab->m_bEnableTabSwap (CMFCBaseTabCtrl +0x1f0) = m_bEnableTabSwap
//     pTab->SetTabBorderSize(m_nTabBorderSize, TRUE)    (vslot +0x498)
//     pTab->EnableCustomToolTips(m_bTabCustomTooltips)  (0x1a9d0)
//     pTab->[+0x1e8] = 0;  pTab->HideNoTabs(TRUE)       (0x13ba30)
//     pTab->m_bAutoSizeWindow (+0x3e4) = 0;  pTab->[+0x1e4] = 0
//     if (pTab->m_bFlatFrame (+0x3e0) != m_bFlatFrame) { store it;
//         if (pTab->m_hWnd) { ::InvalidateRect(hwnd, NULL, TRUE); ::UpdateWindow(hwnd); } }
//     pTab->m_bTransparent (+0x3e8) = pTab->m_bTopEdge (+0x3ec) = pTab->[+0x1f8] = 1
//     pTab->SetActiveTabBoldFont(TRUE)                  (0x13c150)
//     pTab->[+0xec] = pTab->[+0xf4] = pTab->[+0x200] = 1
// (CMFCTabCtrl member names from the BOOL block of afxtabctrl.h counted back
// from m_bIsActiveInMDITabGroup at +0x408; the CMFCBaseTabCtrl offsets +0x1e8,
// +0x1e4, +0x1f8, +0xec, +0xf4, +0x200 were not identified.)
// TODO(clean-room): partially transcribed.  The exported calls are forwarded
// through their thunks (several of which are themselves partial in
// featurepack/tabs); the thirteen raw CMFCTabCtrl stores are NOT made, because
// OpenMFC's CMFCTabCtrl does not model those members and nothing on this side
// would read them.  The one visible side effect of a skipped store, the
// InvalidateRect/UpdateWindow on a changed m_bFlatFrame, is skipped with it.
// Symbol: ?ApplyParams@CMDIClientAreaWnd@@AEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || pTabGroup == nullptr) {
        return;
    }
    CMFCTabCtrl* pTab = static_cast<CMFCTabCtrl*>(pTabGroup);
    CMFCBaseTabCtrl* pBase = AsBaseTab(pTabGroup);
    const CMDITabInfo& params = self->m_mdiTabParams;
    impl__ModifyTabStyle_CMFCTabCtrl__QEAAHW4Style_1__Z(pTab, params.m_style);
    impl__SetLocation_CMFCBaseTabCtrl__UEAAXW4Location_1__Z(pBase, params.m_tabLocation);
    impl__EnableTabDocumentsMenu_CMFCTabCtrl__QEAAXH_Z(pTab, params.m_bDocumentMenu);
    impl__EnableAutoColor_CMFCBaseTabCtrl__QEAAXH_Z(pBase, params.m_bAutoColor);
    pTab->SetTabBorderSize(params.m_nTabBorderSize);
    impl__EnableCustomToolTips_CMFCBaseTabCtrl__QEAAHH_Z(pBase, params.m_bTabCustomTooltips);
    impl__HideNoTabs_CMFCTabCtrl__QEAAXH_Z(pTab, TRUE);
    impl__SetActiveTabBoldFont_CMFCTabCtrl__QEAAXH_Z(pTab, TRUE);
    pTab->EnableTabSwap(params.m_bEnableTabSwap);
    auto& state = openmfc::detail::mfccore::EnsureTabCtrlState(pTab);
    state.mdiCloseButton = params.m_bTabCloseButton;
    state.mdiActiveCloseButton = params.m_bActiveTabCloseButton;
}

// Symbol: ?CalcWindowRect@CMDIClientAreaWnd@@MEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CMDIClientAreaWnd__MEAAXPEAUtagRECT__I_Z(
    void* pThis, RECT* bounds, unsigned int adjustType) {
    auto* self = Self(pThis);
    if (!self || !bounds || self->m_bDisableUpdateTabs) return;
    if (HWND client = WndHandle(self)) {
        ::SetWindowPos(client, nullptr, bounds->left, bounds->top, bounds->right - bounds->left,
            bounds->bottom - bounds->top, SWP_NOZORDER | SWP_NOACTIVATE);
        if (self->m_bIsMDITabbedGroup) LayoutDocumentGroups(*self);
        else LayoutDocumentTabs(*self);
    }
    SelfWnd(self)->CalcWindowRect(bounds, adjustType);
}

// Symbol: ?CalcWindowRectForMDITabbedGroups@CMDIClientAreaWnd@@QEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRectForMDITabbedGroups_CMDIClientAreaWnd__QEAAXPEAUtagRECT__I_Z(
    void* pThis, RECT* bounds, unsigned int) {
    auto* self = Self(pThis);
    if (!self || !bounds || !self->m_bIsMDITabbedGroup || self->m_bInsideDragComplete) return;
    ::SetWindowPos(WndHandle(self), HWND_BOTTOM, bounds->left, bounds->top,
        bounds->right - bounds->left, bounds->bottom - bounds->top, SWP_NOACTIVATE);
    LayoutDocumentGroups(*self);
}

// CMDIClientAreaWnd::CloseAllWindows — transcribed from retail entry RVA
// 0x80c20 (mfc140u):
//     if (pTabWnd) {
//         for (int i = pTabWnd->GetTabsNum() - 1; i >= 0; i--) {                 // vslot +0x368
//             CMDIChildWndEx* p = DYNAMIC_DOWNCAST(CMDIChildWndEx, pTabWnd->GetTabWnd(i));  // +0x370
//             if (p) ::SendMessage(p->m_hWnd, WM_CLOSE, 0, 0);                    // import 0x2c7120
//         }
//         return;
//     }
//     CObList lst;   // local, vftable 0x32a6f8, block size 10
//     for (CWnd* p = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD)); p;
//          p = CWnd::FromHandle(::GetWindow(p->m_hWnd, GW_HWNDNEXT)))
//         if (p->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx))) lst.AddTail(p);       // 0x231e70
//     for (each node) { CMDIChildWndEx* p = DYNAMIC_DOWNCAST(..., node->data);
//                       ::SendMessage(p->m_hWnd, WM_CLOSE, 0, 0); }   // retail sends through NULL when the downcast fails
//     ~CObList(&lst);
// The local list is a file-local RetailObList here (OpenMFC's CObList is a
// side-table wrapper this translation unit cannot call).
// Symbol: ?CloseAllWindows@CMDIClientAreaWnd@@QEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__CloseAllWindows_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    if (pTabGroup != nullptr) {
        for (int i = TabsNum(pTabGroup) - 1; i >= 0; --i) {
            CMDIChildWndEx* pChild = AsChildEx(TabWnd(pTabGroup, i));
            if (pChild != nullptr) {
                ::SendMessage(WndHandle(pChild), WM_CLOSE, 0, 0);
            }
        }
        return;
    }
    RetailObList lst;
    ListInit(lst);
    for (CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(WndHandle(self), GW_CHILD));
         pWnd != nullptr;
         pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(WndHandle(pWnd), GW_HWNDNEXT))) {
        if (AsChildEx(pWnd) != nullptr) {
            ListAddTail(lst, pWnd);
        }
    }
    for (RetailNode* node = lst.pNodeHead; node != nullptr; node = node->pNext) {
        CMDIChildWndEx* pChild = AsChildEx(AsWnd(node->data));
        if (pChild != nullptr) {   // every element passed IsKindOf above; retail does not re-check
            ::SendMessage(WndHandle(pChild), WM_CLOSE, 0, 0);
        }
    }
    ListRemoveAll(lst);
}

// CMDIClientAreaWnd::CreateNewTabGroup — transcribed from retail entry RVA
// 0x80160 (mfc140u).  Argument order verified from the call site inside
// MDITabNewGroup (0x80bea inside 0x80b40, mfc140u): %rdx = the group to
// split, %r8 = a pointer to the caller's CRect copy, %r9d = bVertical.
//     POSITION pos = m_lstTabbedGroups.Find(pTabWndAfter); if (!pos) return NULL;
//     CMFCTabCtrl* pNew = CreateTabGroup(NULL);                    // this->vslot +0x2d8
//     int nAlign = bVertical ? GROUP_VERT_ALIGN : GROUP_HORZ_ALIGN;   // neg/sbb/add 2
//     pTabWndAfter->m_ResizeMode = nAlign;  pTabWndAfter->RecalcLayout();   // +0x3d28, vslot +0x318
//     m_lstTabbedGroups.InsertAfter(pos, pNew);                    // 0x232070
//     if (pNew != m_lstTabbedGroups.GetTail()) { pNew->m_ResizeMode = nAlign; pNew->RecalcLayout(); }
//     m_groupAlignment = nAlign;
//     CRect rcAfter; ::GetWindowRect(pTabWndAfter->m_hWnd, &rcAfter);
//     ScreenToClient(&rectGroup); ScreenToClient(&rcAfter);         // 0x2a32b0
//     if (bVertical) { rcAfter.right -= rectGroup.Width(); rectGroup.top = rcAfter.top; rectGroup.bottom = rcAfter.bottom; }
//     else           { rcAfter.bottom -= rectGroup.Height(); rectGroup.left = rcAfter.left; rectGroup.right = rcAfter.right; }
//     pTabWndAfter->SetWindowPos(NULL, -1, -1, rcAfter.Width(), rcAfter.Height(), SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);   // x/y are the literal -1 (`or $-1,%r8d`)
//     pNew->SetWindowPos(NULL, rectGroup.left, rectGroup.top, rectGroup.Width(), rectGroup.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
//     AdjustMDIChildren(pTabWndAfter);  return pNew;
// Deviations: retail never checks pNew for NULL (its operator new throws);
// here CreateTabGroup returns NULL whenever CMFCTabCtrl::Create fails, so a
// NULL result returns NULL without touching the list.  CreateTabGroup is
// reached through its thunk rather than the object's vtable (no MSVC vtable),
// so an app override is not honoured.
// Symbol: ?CreateNewTabGroup@CMDIClientAreaWnd@@AEAAPEAVCMFCTabCtrl@@PEAV2@VCRect@@H@Z
extern "C" void* MS_ABI impl__CreateNewTabGroup_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_VCRect__H_Z(void* pThis, void* pTabGroup, const RECT* pRect, int bVertical) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || pRect == nullptr) {
        return nullptr;
    }
    RetailNode* pos = ListFind(self->m_lstTabbedGroups, pTabGroup);
    if (pos == nullptr) {
        return nullptr;
    }
    void* pNew = impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(pThis, nullptr);
    if (pNew == nullptr) {
        return nullptr;   // deviation, see above
    }
    const int nAlign = bVertical ? kGroupVertAlign : kGroupHorzAlign;
    TabResizeMode(pTabGroup) = nAlign;
    impl__RecalcLayout_CMFCTabCtrl__UEAAXXZ(static_cast<CMFCTabCtrl*>(pTabGroup));
    ListInsertAfter(self->m_lstTabbedGroups, pos, pNew);
    PublishGroupList(*self);
    if (self->m_lstTabbedGroups.pNodeTail->data != pNew) {
        TabResizeMode(pNew) = nAlign;
        impl__RecalcLayout_CMFCTabCtrl__UEAAXXZ(static_cast<CMFCTabCtrl*>(pNew));
    }
    self->m_groupAlignment = nAlign;

    RECT rectGroup = *pRect;   // MSVC passes the by-value CRect as a pointer to a copy
    RECT rcAfter = {};
    ::GetWindowRect(WndHandle(pTabGroup), &rcAfter);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(SelfWnd(self), &rectGroup);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(SelfWnd(self), &rcAfter);
    if (bVertical) {
        rcAfter.right -= rectGroup.right - rectGroup.left;
        rectGroup.top = rcAfter.top;
        rectGroup.bottom = rcAfter.bottom;
    } else {
        rcAfter.bottom -= rectGroup.bottom - rectGroup.top;
        rectGroup.left = rcAfter.left;
        rectGroup.right = rcAfter.right;
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pTabGroup), nullptr, -1, -1,
                                                rcAfter.right - rcAfter.left, rcAfter.bottom - rcAfter.top,
                                                SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pNew), nullptr, rectGroup.left, rectGroup.top,
                                                rectGroup.right - rectGroup.left, rectGroup.bottom - rectGroup.top,
                                                SWP_NOZORDER | SWP_NOACTIVATE);
    impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pTabGroup);
    return pNew;
}

// CMDIClientAreaWnd::CreateTabGroup — transcribed from retail entry RVA
// 0x7e740 (mfc140u, virtual):
//     BOOL bRecycled = FALSE;  CMFCTabCtrl* pNew = pWndTab;
//     if (!pWndTab) {
//         if (m_mdiTabParams.m_bReuseRemovedTabGroups && m_lstRemovedTabbedGroups.GetCount() > 0
//             && m_bIsMDITabbedGroup) {
//             CObject* p = m_lstRemovedTabbedGroups.RemoveTail();               // 0x231fb0
//             if (p && p->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)) && ::IsWindow(p->m_hWnd)) { pNew = p; bRecycled = TRUE; }
//         }                                                                    // (a popped object that fails the test is dropped)
//         if (!bRecycled) { pNew = operator new(0x3d68) ? CMFCTabCtrl::CMFCTabCtrl(mem) : NULL; }   // 0x27f0 / 0x136d80
//     }
//     if (m_mdiTabParams.m_bTabCustomTooltips) pNew->EnableCustomToolTips(TRUE);          // 0x1a9d0
//     CWnd* pParent = m_bIsMDITabbedGroup ? this : GetParentFrame();                      // 0x28e200
//     if (!bRecycled) {
//         RECT rc = {0};
//         if (!pNew->Create(m_style, rc, pParent, (UINT)-1, m_tabLocation, m_bTabCloseButton)) {   // 0x1370c0
//             if (pNew != &m_wndTab && pNew) delete pNew;                                  // vtable slot 1
//             return NULL;
//         }
//         if (m_bLastActiveTab) pNew->m_bActivateLastActiveTab = TRUE;                   // +0x334
//     } else {
//         if (CWnd::FromHandle(::GetParent(pNew->m_hWnd)) != pParent)                     // 0x2c72d8 / 0x28ad70
//             CWnd::FromHandle(::SetParent(pNew->m_hWnd, pParent ? pParent->m_hWnd : NULL));   // 0x2c7288, result unused
//         pNew->ModifyTabStyle(m_style); pNew->SetLocation(m_tabLocation);              // 0x13c380, vslot +0x500
//         pNew->EnableActiveTabCloseButton(m_bTabCloseButton);                            // 0x13c8e0
//     }
//     ApplyParams(pNew);                                                                  // 0x7d1b0
//     if (!m_bTabIsVisible) pNew->ShowWindow(SW_HIDE);
//     CImageList* pImages;
//     if (!m_bIsMDITabbedGroup) pImages = &m_TabIcons;
//     else if (m_mapTabIcons.Lookup(pNew, pImages) && pImages) pImages->DeleteImageList();   // 0x1bfec / 0x296c80
//     else { pImages = new CImageList (0x10 bytes, vftable 0x337b38, handle NULL); m_mapTabIcons.SetAt(pNew, pImages); }   // 0x8340c
//     if (!afxGlobalData.m_bInitialized) { Initialize(); m_bInitialized = 1; }           // twice, 0x3c1620
//     pImages->Create(afxGlobalData.m_sizeSmallIcon.cx, .cy, ILC_COLOR32|ILC_MASK, 0, 1);   // 0x296d10, 0x3c17c0/0x3c17c4
//     return pNew;
// Symbol: ?CreateTabGroup@CMDIClientAreaWnd@@UEAAPEAVCMFCTabCtrl@@PEAV2@@Z
extern "C" void* MS_ABI impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(void* pThis, void* pTabGroup) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    const CMDITabInfo& params = self->m_mdiTabParams;
    bool bRecycled = false;
    void* pNew = pTabGroup;
    if (pNew == nullptr) {
        if (params.m_bReuseRemovedTabGroups && self->m_lstRemovedTabbedGroups.nCount > 0 && self->m_bIsMDITabbedGroup) {
            void* p = ListRemoveTail(self->m_lstRemovedTabbedGroups);
            if (p != nullptr && AsTabCtrl(p) != nullptr && ::IsWindow(WndHandle(p))) {
                pNew = p;
                bRecycled = true;
            } else if (p) {
                DeleteTabCtrlObject(p);
            }
        }
        if (!bRecycled) {
            pNew = RawAlloc(kRetailTabCtrlSize);
            if (pNew != nullptr) {
                impl___0CMFCTabCtrl__QEAA_XZ(pNew);
            }
        }
    }
    if (pNew == nullptr) {
        return nullptr;   // retail would crash on the NULL below
    }
    if (params.m_bTabCustomTooltips) {
        impl__EnableCustomToolTips_CMFCBaseTabCtrl__QEAAHH_Z(AsBaseTab(pNew), TRUE);
    }
    CWnd* pParent = self->m_bIsMDITabbedGroup
                        ? SelfWnd(self)
                        : static_cast<CWnd*>(impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self)));
    if (!bRecycled) {
        RECT rc = {};
        if (!impl__Create_CMFCTabCtrl__QEAAHW4Style_1_AEBUtagRECT__PEAVCWnd__IW4Location_CMFCBaseTabCtrl__H_Z(
                static_cast<CMFCTabCtrl*>(pNew), params.m_style, &rc, pParent, static_cast<unsigned int>(-1),
                params.m_tabLocation, params.m_bTabCloseButton)) {
            if (pNew != self->m_wndTab) {
                DeleteTabCtrlObject(pNew);
            }
            return nullptr;
        }
        if (self->m_bLastActiveTab) {
            TabActivateLastActive(pNew) = TRUE;
        }
    } else {
        CWnd* pCurParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(WndHandle(pNew)));
        if (pCurParent != pParent) {
            impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetParent(WndHandle(pNew), WndHandle(pParent)));
        }
        impl__ModifyTabStyle_CMFCTabCtrl__QEAAHW4Style_1__Z(static_cast<CMFCTabCtrl*>(pNew), params.m_style);
        impl__SetLocation_CMFCBaseTabCtrl__UEAAXW4Location_1__Z(AsBaseTab(pNew), params.m_tabLocation);
        impl__EnableActiveTabCloseButton_CMFCTabCtrl__QEAAXH_Z(static_cast<CMFCTabCtrl*>(pNew), params.m_bTabCloseButton);
    }
    impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pNew);
    if (self->m_bIsMDITabbedGroup)
        ::SetWindowSubclass(WndHandle(pNew), GroupTabSubclass, groupTabSubclassId, reinterpret_cast<DWORD_PTR>(self));
    if (!self->m_bTabIsVisible) {
        impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pNew), SW_HIDE);
    }
    CImageList* pImages = nullptr;
    if (!self->m_bIsMDITabbedGroup) {
        pImages = reinterpret_cast<CImageList*>(self->m_TabIcons);
    } else {
        InstanceRecord* rec = Record(pThis);
        TabIconEntry* e = TabIconsLookup(rec, AsWnd(pNew));
        if (e != nullptr && e->value != nullptr) {
            pImages = e->value;
            impl__DeleteImageList_CImageList__QEAAHXZ(pImages);
        } else {
            pImages = static_cast<CImageList*>(RawAlloc(0x10));
            if (pImages != nullptr) {
                impl___0CImageList__QEAA_XZ(pImages);
            }
            TabIconsSetAt(rec, self, AsWnd(pNew), pImages);
        }
    }
    EnsureGlobalDataInitialized();
    if (pImages != nullptr) {
        impl__Create_CImageList__QEAAHHHIHH_Z(pImages, GlobalDataInt(kGlobalSmallIconCx), GlobalDataInt(kGlobalSmallIconCy),
                                              ILC_COLOR32 | ILC_MASK, 0, 1);
    }
    return pNew;
}

// CMDIClientAreaWnd::DrawNewGroupRect — transcribed from retail entry RVA
// 0x809a0 (mfc140u; 0x80f30 in mfc140). This is the one body in this class
// that is member-free: %rcx is overwritten without ever being read, so `this`
// is unused. Retail does, in order:
//     CWindowDC dc(CWnd::FromHandle(::GetDesktopWindow()));   // 0x2a3c20 / 0x28ad70
//     RECT rcNew, rcOld;
//     ::SetRectEmpty(&rcNew); ::SetRectEmpty(&rcOld);
//     if (pRectNew) ::CopyRect(&rcNew, pRectNew);
//     if (pRectOld) ::CopyRect(&rcOld, pRectOld);
//     dc.DrawDragRect(&rcNew, CSize(4,4), &rcOld, CSize(4,4), NULL, NULL);   // 0x2a5720
// (the 4x4 size is the qword at rbp+0x17 and it is passed BOTH in %r8 as
// `size` and at rsp+0x20 as `sizeLast` — the two stack slots that are zeroed,
// rsp+0x28 and rsp+0x30, are the two CBrush* arguments.)
// Note that a NULL argument is never forwarded: retail turns it into an empty
// rect, so DrawDragRect always receives two non-null pointers. That is
// reproduced here rather than passing the caller's pointers straight through.
// Symbol: ?DrawNewGroupRect@CMDIClientAreaWnd@@AEAAXPEBUtagRECT@@0@Z
extern "C" void MS_ABI impl__DrawNewGroupRect_CMDIClientAreaWnd__AEAAXPEBUtagRECT__0_Z(void* pThis, const RECT* pRectNew, const RECT* pRectOld) {
    (void)pThis;  // retail never reads `this` here

    CWnd* pDesktop = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDesktopWindow());

    // CWindowDC exists in this DLL only as its impl__ thunks, so the object is
    // built in raw storage and torn down through the matching thunk.
    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, pDesktop);

    RECT rectNew;
    RECT rectOld;
    ::SetRectEmpty(&rectNew);
    ::SetRectEmpty(&rectOld);
    if (pRectNew != nullptr) {
        ::CopyRect(&rectNew, pRectNew);
    }
    if (pRectOld != nullptr) {
        ::CopyRect(&rectOld, pRectOld);
    }

    SIZE size;
    size.cx = 4;
    size.cy = 4;
    impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
        pDC, &rectNew, size, &rectOld, size, nullptr, nullptr);

    impl___1CWindowDC__UEAA_XZ(pDC);
}

// Symbol: ?EnableMDITabbedGroups@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabbedGroups_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(
    void* pThis, int enable, const void* params) {
    auto* self = Self(pThis);
    if (!self || !params) return;
    if (self->m_bTabIsEnabled) {
        self->m_bTabIsEnabled = FALSE;
        LayoutDocumentTabs(*self);
    }
    CancelTabDrag(*self);
    self->m_bTabIsVisible = enable;
    self->m_bIsMDITabbedGroup = enable;
    if (enable) {
        const auto& options = *static_cast<const CMDITabInfo*>(params);
        if (&options != &self->m_mdiTabParams) self->m_mdiTabParams = options;
        for (auto* node = self->m_lstTabbedGroups.pNodeHead; node; node = node->pNext)
            impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(self, node->data);
        impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(self, TRUE);
    } else {
        CancelSplitterDrag(*self);
        for (auto* node = self->m_lstTabbedGroups.pNodeHead; node; node = node->pNext)
            ::ShowWindow(WndHandle(node->data), SW_HIDE);
        RestoreDocumentPlacements(*self);
    }
}

// Symbol: ?EnableMDITabs@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabs_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(
    void* pThis, int enable, const void* params) {
    auto* self = Self(pThis);
    if (!self || !params) return;
    if (self->m_bIsMDITabbedGroup)
        impl__EnableMDITabbedGroups_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(pThis, FALSE, params);
    self->m_bTabIsEnabled = enable;
    const auto& options = *static_cast<const CMDITabInfo*>(params);
    if (&options != &self->m_mdiTabParams) self->m_mdiTabParams = options;
    impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(self, self->m_wndTab);
    self->m_bTabIsVisible = enable;
    if (enable) impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(pThis, FALSE);
    else {
        LayoutDocumentTabs(*self);
        RestoreDocumentPlacements(*self);
    }
}

// CMDIClientAreaWnd::EnableMDITabsLastActiveActivation — transcribed from
// retail entry RVA 0x833a0 (mfc140u):
//     if (m_bLastActiveTab == bLastActiveTab) return;
//     m_bLastActiveTab = bLastActiveTab;
//     m_wndTab.m_bActivateLastActiveTab = bLastActiveTab;      // this+0x41c == m_wndTab+0x334
//     for (each group node) DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data)->m_bActivateLastActiveTab = bLastActiveTab;
// Retail stores through the NULL a failed downcast yields (0x833ef is not
// guarded); the store is skipped for that node here.
// Symbol: ?EnableMDITabsLastActiveActivation@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableMDITabsLastActiveActivation_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bLastActiveTab) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || self->m_bLastActiveTab == bLastActiveTab) {
        return;
    }
    self->m_bLastActiveTab = bLastActiveTab;
    TabActivateLastActive(self->m_wndTab) = bLastActiveTab;
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (pGroup != nullptr) {
            TabActivateLastActive(pGroup) = bLastActiveTab;
        }
    }
}

// CMDIClientAreaWnd::FindActiveTabWnd — transcribed from retail entry RVA
// 0x807b0 (mfc140u):
//     for (node = head; node; node = node->pNext) {
//         CMFCTabCtrl* p = DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data);
//         if (p->m_bIsActiveInMDITabGroup) return p;                 // +0x408, read through NULL when the downcast fails
//     }
//     return NULL;
// Symbol: ?FindActiveTabWnd@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__FindActiveTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (pGroup != nullptr && TabActiveInMDIGroup(pGroup)) {
            return pGroup;
        }
    }
    return nullptr;
}

// CMDIClientAreaWnd::FindActiveTabWndByActiveChild — transcribed from retail
// entry RVA 0x80770 (mfc140u):
//     int nTab = -1;
//     return FindTabWndByChild((HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0), nTab);   // 0x806f0
// Symbol: ?FindActiveTabWndByActiveChild@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__FindActiveTabWndByActiveChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    int nTab = -1;
    HWND hActive = reinterpret_cast<HWND>(::SendMessage(WndHandle(self), WM_MDIGETACTIVE, 0, 0));
    return impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(pThis, hActive, &nTab);
}

// CMDIClientAreaWnd::FindNextRegisteredWithTaskbarMDIChild(CMDIChildWndEx*) —
// transcribed from retail entry RVA 0x7e990 (mfc140; 0x7e400 in mfc140u, the
// callee at 0x7e3de inside OnActiveTabChanged):
//     int nTab = -1;
//     CMFCTabCtrl* pGroup = FindTabWndByChild(pOrgWnd ? pOrgWnd->m_hWnd : NULL, nTab);
//     if (!pGroup) return NULL;
//     CMDIChildWndEx* pFound = FindNextRegisteredWithTaskbarMDIChild(pGroup, nTab + 1);
//     if (pFound) return pFound;
//     while ((pGroup = GetNextTabWnd(pGroup, TRUE)) != NULL) {
//         if (pFound) return pFound;
//         pFound = FindNextRegisteredWithTaskbarMDIChild(pGroup, 0);
//     }
//     return pFound;
// (retail steps to the following group before returning a hit found in the
// current one; transcribed as is, the result is the same.)
// Symbol: ?FindNextRegisteredWithTaskbarMDIChild@CMDIClientAreaWnd@@QEAAPEAVCMDIChildWndEx@@PEAV2@@Z
extern "C" void* MS_ABI impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAV2__Z(void* pThis, void* pMDIChild) {
    if (Self(pThis) == nullptr) {
        return nullptr;
    }
    int nTab = -1;
    void* pGroup = impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(
        pThis, WndHandle(pMDIChild), &nTab);
    if (pGroup == nullptr) {
        return nullptr;
    }
    void* pFound = impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAVCMFCTabCtrl__H_Z(
        pThis, pGroup, nTab + 1);
    if (pFound != nullptr) {
        return pFound;
    }
    while ((pGroup = impl__GetNextTabWnd_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_H_Z(pThis, pGroup, TRUE)) != nullptr) {
        if (pFound != nullptr) {
            return pFound;
        }
        pFound = impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAVCMFCTabCtrl__H_Z(
            pThis, pGroup, 0);
    }
    return pFound;
}

// CMDIClientAreaWnd::FindNextRegisteredWithTaskbarMDIChild(CMFCTabCtrl*, int)
// — transcribed from retail entry RVA 0x7ea30 (mfc140; 0x7e4a0 in mfc140u):
//     if (!pTabCtrl) return NULL;
//     for (int i = max(iStartFrom, 0); i < pTabCtrl->GetTabsNum(); i++) {        // vslot +0x368
//         CMDIChildWndEx* p = DYNAMIC_DOWNCAST(CMDIChildWndEx, pTabCtrl->GetTabWnd(i));   // +0x370
//         if (p && p->m_hWnd && p->m_tabProxyWnd.m_hWnd) return p;   // `lea 0x720(%rbx); test` is always true; +0x760 is the real test
//     }
//     return NULL;
// Deviation: this overload never calls IsTaskbarTabsSupportEnabled in retail;
// here the +0x760 read is placed behind that (always-FALSE) gate for the
// reason given at ChildTabProxyHwnd, so no child is ever reported until
// CMDIChildWndEx models its proxy window.
// Symbol: ?FindNextRegisteredWithTaskbarMDIChild@CMDIClientAreaWnd@@QEAAPEAVCMDIChildWndEx@@PEAVCMFCTabCtrl@@H@Z
extern "C" void* MS_ABI impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAVCMFCTabCtrl__H_Z(void* pThis, void* pTabGroup, int nStartFrom) {
    if (Self(pThis) == nullptr || pTabGroup == nullptr) {
        return nullptr;
    }
    for (int i = nStartFrom < 0 ? 0 : nStartFrom; i < TabsNum(pTabGroup); ++i) {
        CMDIChildWndEx* pChild = AsChildEx(TabWnd(pTabGroup, i));
        if (pChild != nullptr && WndHandle(pChild) != nullptr
            && impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pChild)   // OpenMFC-only gate, see above
            && ChildTabProxyHwnd(pChild) != nullptr) {
            return pChild;
        }
    }
    return nullptr;
}

// CMDIClientAreaWnd::FindTabWndByChild — transcribed from retail entry RVA
// 0x806f0 (mfc140u):
//     if (!hWndChild) return NULL;
//     for (node = head; node; node = node->pNext) {
//         CMFCTabCtrl* p = DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data);
//         iIndex = p->GetTabFromHwnd(hWndChild);                       // vslot +0x448, through NULL on a failed downcast
//         if (iIndex >= 0) return p;
//     }
//     return NULL;
// Symbol: ?FindTabWndByChild@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@PEAUHWND__@@AEAH@Z
extern "C" void* MS_ABI impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(void* pThis, HWND hWnd, int* pnTabIndex) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || hWnd == nullptr || pnTabIndex == nullptr) {
        return nullptr;
    }
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (pGroup == nullptr) {
            continue;
        }
        *pnTabIndex = TabFromHwnd(pGroup, hWnd);
        if (*pnTabIndex >= 0) {
            return pGroup;
        }
    }
    return nullptr;
}

// CMDIClientAreaWnd::GetFirstTabWnd — transcribed from retail entry RVA
// 0x80810 (mfc140u):
//     if (m_lstTabbedGroups.GetCount() == 0) return NULL;
//     return DYNAMIC_DOWNCAST(CMFCTabCtrl, m_lstTabbedGroups.GetHead());
// Symbol: ?GetFirstTabWnd@CMDIClientAreaWnd@@QEAAPEAVCMFCTabCtrl@@XZ
extern "C" void* MS_ABI impl__GetFirstTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || self->m_lstTabbedGroups.nCount == 0 || self->m_lstTabbedGroups.pNodeHead == nullptr) {
        return nullptr;
    }
    return AsTabCtrl(self->m_lstTabbedGroups.pNodeHead->data);
}

// CMDIClientAreaWnd::GetMDITabsContextMenuAllowedItems — retail entry RVA
// 0x805e0 (mfc140u):
//     CMFCTabCtrl* pActive = FindActiveTabWndByActiveChild();  if (!pActive) return 0;
//     DWORD dw = 0;
//     if (pActive->GetTabsNum() > 1)                                    // vslot +0x368
//         dw = (m_lstTabbedGroups.GetCount() > 1)
//                ? ((m_groupAlignment != GROUP_VERT_ALIGN) + 1)       // 1 = vert group, 2 = horz group
//                : 3;
//     if (pActive != m_lstTabbedGroups.GetHead()) dw |= 4;
//     if (pActive != m_lstTabbedGroups.GetTail()) dw |= 8;
//     CMDIChildWndEx* pChild = DYNAMIC_DOWNCAST(CMDIChildWndEx, pActive->GetActiveWnd());   // vslot +0x430
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     if (pChild && pChild->m_pTabbedControlBar /*+0x710, IsTabbedPane()*/ && pFrame && pFrame->[+0x6d8] == 0)
//         dw |= 0x10;
//     return dw;
// TODO(clean-room): partially transcribed -- the 0x10 bit is never set here.
// It needs CMDIChildWndEx +0x710 (zeroed at 0x838ab in the retail ctor but
// never initialised by OpenMFC's, so it reads uninitialised app memory) and
// CMDIFrameWndEx +0x6d8, which OpenMFC's frame does not model at all.
// Symbol: ?GetMDITabsContextMenuAllowedItems@CMDIClientAreaWnd@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetMDITabsContextMenuAllowedItems_CMDIClientAreaWnd__QEAAKXZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return 0;
    }
    void* pActive = impl__FindActiveTabWndByActiveChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(pThis);
    if (pActive == nullptr) {
        return 0;
    }
    unsigned long dwAllowed = 0;
    if (TabsNum(pActive) > 1) {
        if (self->m_lstTabbedGroups.nCount > 1) {
            dwAllowed = (self->m_groupAlignment != kGroupVertAlign ? 1u : 0u) + 1u;
        } else {
            dwAllowed = 3;
        }
    }
    if (self->m_lstTabbedGroups.pNodeHead == nullptr || self->m_lstTabbedGroups.pNodeHead->data != pActive) {
        dwAllowed |= 4;
    }
    if (self->m_lstTabbedGroups.pNodeTail == nullptr || self->m_lstTabbedGroups.pNodeTail->data != pActive) {
        dwAllowed |= 8;
    }
    auto* child = AsChildEx(TabWnd(pActive, ActiveTab(pActive)));
    if (child && child->GetTabbedPane()) dwAllowed |= 0x10;
    return dwAllowed;
}

// CMDIClientAreaWnd::GetNextTabWnd — transcribed from retail entry RVA
// 0x80890 (mfc140u):
//     POSITION pos = m_lstTabbedGroups.Find(pOrgTabWnd);  if (!pos) return NULL;
//     node = pos->pNext;  if (!node || !node->data) return NULL;
//     return DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data);          // NULL when the downcast fails
// bWithoutAsserts is not read on this path.
// Symbol: ?GetNextTabWnd@CMDIClientAreaWnd@@AEAAPEAVCMFCTabCtrl@@PEAV2@H@Z
extern "C" void* MS_ABI impl__GetNextTabWnd_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_H_Z(void* pThis, void* pTabGroup, int bWithoutAsserts) {
    (void)bWithoutAsserts;
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    RetailNode* node = ListFind(self->m_lstTabbedGroups, pTabGroup);
    if (node == nullptr || node->pNext == nullptr || node->pNext->data == nullptr) {
        return nullptr;
    }
    return AsTabCtrl(node->pNext->data);
}

// CMDIClientAreaWnd::IsKeepClientEdge — transcribed from retail entry RVA
// 0x7f6a0 (mfc140u):
//     BOOL bFlag = TRUE;
//     HWND hActive = (HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0);
//     if (hActive) {
//         CWnd* p = CWnd::FromHandle(hActive);                      // 0x28ad70
//         if (p && ::IsWindow(p->m_hWnd))
//             bFlag = (p->GetStyle() >> 19) & 1;                    // 0x2a9690; `shr $0x13; and $1`
//     }
//     return (!m_bIsMDITabbedGroup && bFlag) ? TRUE : FALSE;
// Bit 19 of the style word is WS_SYSMENU (0x00080000), NOT WS_MAXIMIZE
// (0x01000000, bit 24): the retail test is on the active child's WS_SYSMENU
// bit (the same `bt $0x13` core/frame/CMDIChildWndEx.cpp documents for
// CMDIChildWndEx::IsTaskbarTabsSupportEnabled).  A previous version of this
// body tested WS_MAXIMIZE; corrected on review.
// Symbol: ?IsKeepClientEdge@CMDIClientAreaWnd@@AEAAHXZ
extern "C" int MS_ABI impl__IsKeepClientEdge_CMDIClientAreaWnd__AEAAHXZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return FALSE;
    }
    BOOL bFlag = TRUE;
    HWND hActive = reinterpret_cast<HWND>(::SendMessage(WndHandle(self), WM_MDIGETACTIVE, 0, 0));
    if (hActive != nullptr) {
        CWnd* pActive = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hActive);
        if (pActive != nullptr && ::IsWindow(WndHandle(pActive))) {
            bFlag = (impl__GetStyle_CWnd__QEBAKXZ(pActive) & WS_SYSMENU) ? TRUE : FALSE;   // bit 19, see above
        }
    }
    return (!self->m_bIsMDITabbedGroup && bFlag) ? TRUE : FALSE;
}

// CMDIClientAreaWnd::IsMemberOfMDITabGroup — transcribed from retail entry
// RVA 0x80860 (mfc140u):
//     if (!m_bIsMDITabbedGroup) return FALSE;
//     for (node = head; node; node = node->pNext) if (node->data == pWnd) break;
//     return node != NULL;
// Symbol: ?IsMemberOfMDITabGroup@CMDIClientAreaWnd@@QEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__IsMemberOfMDITabGroup_CMDIClientAreaWnd__QEAAHPEAVCWnd___Z(void* pThis, void* pWnd) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || !self->m_bIsMDITabbedGroup) {
        return FALSE;
    }
    return ListFind(self->m_lstTabbedGroups, pWnd) != nullptr ? TRUE : FALSE;
}

// CMDIClientAreaWnd::MDITabMoveToNextGroup — transcribed from retail entry
// RVA 0x80aa0 (mfc140u):
//     CMFCTabCtrl* pGroup = FindActiveTabWndByActiveChild();  if (!pGroup) return;
//     POSITION pos = m_lstTabbedGroups.Find(pGroup);
//     if (!pos) AfxThrowInvalidArgException();                             // 0x227720
//     node = bNext ? pos->pNext : pos->pPrev;  if (!node) return;
//     MoveWindowToTabGroup(pGroup, DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data), -1);   // 0x7ff80, pTo may be NULL
// Symbol: ?MDITabMoveToNextGroup@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__MDITabMoveToNextGroup_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bNext) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    void* pGroup = impl__FindActiveTabWndByActiveChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(pThis);
    if (pGroup == nullptr) {
        return;
    }
    RetailNode* pos = ListFind(self->m_lstTabbedGroups, pGroup);
    if (pos == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    RetailNode* node = bNext ? pos->pNext : pos->pPrev;
    if (node == nullptr) {
        return;
    }
    impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(pThis, pGroup, AsTabCtrl(node->data), -1);
}

// CMDIClientAreaWnd::MDITabNewGroup — transcribed from retail entry RVA
// 0x80b40 (mfc140u):
//     BOOL bVert = (m_groupAlignment == GROUP_VERT_ALIGN) ? TRUE
//                : (m_groupAlignment == GROUP_HORZ_ALIGN) ? FALSE : bVertical;
//     CMFCTabCtrl* pGroup = FindActiveTabWndByActiveChild();  if (!pGroup) return;
//     CRect rc; ::GetWindowRect(pGroup->m_hWnd, &rc);
//     if (bVert) rc.left += (rc.right - rc.left) / 2; else rc.top += (rc.bottom - rc.top) / 2;   // cltd/sub/sar: C division
//     CMFCTabCtrl* pNew = CreateNewTabGroup(pGroup, rc, bVert);            // 0x80160
//     MoveWindowToTabGroup(pGroup, pNew, -1);                              // 0x7ff80
// Symbol: ?MDITabNewGroup@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__MDITabNewGroup_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bVertical) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    BOOL bVert = bVertical;
    if (self->m_groupAlignment == kGroupVertAlign) {
        bVert = TRUE;
    } else if (self->m_groupAlignment == kGroupHorzAlign) {
        bVert = FALSE;
    }
    void* pGroup = impl__FindActiveTabWndByActiveChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(pThis);
    if (!pGroup || TabsNum(pGroup) < 2) {
        return;
    }
    RECT rc = {};
    ::GetWindowRect(WndHandle(pGroup), &rc);
    if (bVert) {
        rc.left += (rc.right - rc.left) / 2;
    } else {
        rc.top += (rc.bottom - rc.top) / 2;
    }
    void* pNew = impl__CreateNewTabGroup_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_VCRect__H_Z(pThis, pGroup, &rc, bVert);
    impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(pThis, pGroup, pNew, -1);
}

// Symbol: ?MoveWindowToTabGroup@CMDIClientAreaWnd@@QEAAHPEAVCMFCTabCtrl@@0H@Z
extern "C" int MS_ABI impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(
    void* pThis, void* pFrom, void* pTo, int index) {
    auto* self = Self(pThis);
    if (!self || !pFrom || !pTo || pFrom == pTo || !self->m_bIsMDITabbedGroup ||
        !ListFind(self->m_lstTabbedGroups, pFrom) || !ListFind(self->m_lstTabbedGroups, pTo)) return FALSE;
    auto* source = static_cast<CMFCTabCtrl*>(pFrom);
    auto* target = static_cast<CMFCTabCtrl*>(pTo);
    if (index == -1) index = source->GetActiveTab();
    auto* child = AsChildEx(source->GetTabWnd(index));
    if (!child || !target->InsertTab(child, nullptr, target->GetTabsCount(), UINT(-1), TRUE)) return FALSE;

    const HWND fromWindow = source->m_hWnd;
    const HWND toWindow = target->m_hWnd;
    source->RemoveTab(index);
    SetChildRelatedTabGroup(child, target);
    if (source->GetTabsCount() == 0)
        impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(self, source, TRUE);
    const HWND client = WndHandle(self);
    ::SendMessageW(client, WM_MDIACTIVATE, reinterpret_cast<WPARAM>(child->m_hWnd), 0);
    impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(self, TRUE);
    ::SendMessageW(::GetParent(client), impl__AFX_WM_ON_MOVETOTABGROUP__3IA,
        reinterpret_cast<WPARAM>(fromWindow), reinterpret_cast<LPARAM>(toWindow));
    return TRUE;
}

// CMDIClientAreaWnd::OnActiveTabChanged — transcribed from retail entry RVA
// 0x7e320 (mfc140u):
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;   if (!pApp) return 0;   // 0x133930, +0x8
//     if (!afxGlobalData.m_bInitialized) { Initialize(); m_bInitialized = 1; }
//     if (!afxGlobalData.<+0x258> || !m_bIsMDITabbedGroup) return 0;
//     if (!pApp->IsTaskbarInteractionEnabled()) return 0;                   // CWinApp vslot +0x210
//     CMFCTabCtrl* pTab = (CMFCTabCtrl*)lp;  if (!pTab) return 0;
//     CMDIChildWndEx* p = DYNAMIC_DOWNCAST(CMDIChildWndEx, pTab->GetTabWnd((int)wp));   // +0x370
//     if (!p || !p->m_tabProxyWnd.m_hWnd) return 0;                          // +0x760
//     p->SetTaskbarTabOrder(FindNextRegisteredWithTaskbarMDIChild(p));      // 0x7e400 / 0x84220
//     return 1;
// `AfxGetModuleState()->m_pCurrentWinApp` is AfxGetApp(), reached through
// that thunk.  Deviation: the +0x760 read sits behind IsTaskbarTabsSupportEnabled
// (see ChildTabProxyHwnd); OpenMFC's afxGlobalData blob is zero-filled, so the
// +0x258 test returns 0 before either is reached today.
// Symbol: ?OnActiveTabChanged@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnActiveTabChanged_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return 0;
    }
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr) {
        return 0;
    }
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(kGlobalTaskbarFlag) == 0 || !self->m_bIsMDITabbedGroup) {
        return 0;
    }
    if (!impl__IsTaskbarInteractionEnabled_CWinApp__UEAAHXZ(pApp)) {
        return 0;
    }
    void* pTab = reinterpret_cast<void*>(lParam);
    if (pTab == nullptr) {
        return 0;
    }
    CMDIChildWndEx* pChild = AsChildEx(TabWnd(pTab, static_cast<int>(wParam)));
    if (pChild == nullptr
        || !impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pChild)   // OpenMFC-only gate, see above
        || ChildTabProxyHwnd(pChild) == nullptr) {
        return 0;
    }
    void* pNext = impl__FindNextRegisteredWithTaskbarMDIChild_CMDIClientAreaWnd__QEAAPEAVCMDIChildWndEx__PEAV2__Z(pThis, pChild);
    impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(pChild, static_cast<CMDIChildWndEx*>(pNext));
    return 1;
}

// Symbol: ?OnCancelTabMove@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnCancelTabMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64, __int64) {
    if (auto* self = Self(pThis)) CancelTabDrag(*self);
    return 0;
}

// Symbol: ?OnDragComplete@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnDragComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    auto* self = Self(pThis);
    auto* group = reinterpret_cast<CMFCTabCtrl*>(wParam);
    const auto* rect = reinterpret_cast<const RECT*>(lParam);
    if (!self || !self->m_bIsMDITabbedGroup || !group || !rect) return 0;
    POINT end{rect->right, rect->bottom};
    ::ScreenToClient(WndHandle(self), &end);
    return ResizeAdjacentGroups(*self, *group, IsVerticalSplit(*self) ? end.x : end.y);
}

// CMDIClientAreaWnd::OnEraseBkgnd — retail entry RVA 0x7d9d0 (mfc140):
//     if (m_bIsMDITabbedGroup) {
//         HWND h = (HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0);
//         if (h && ::IsWindowVisible(h)) return TRUE;
//     }
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     if (pFrame && pFrame->OnEraseMDIClientBackground(pDC)) return TRUE;   // vslot +0x408; the retail base is `xor eax,eax; ret`
//     CRect rc; ::GetClientRect(m_hWnd, &rc);
//     if (CMFCVisualManager::GetInstance()->OnEraseMDIClientArea(pDC, rc)) return TRUE;   // 0x97f4, vslot +0x5b0; base is `xor eax,eax; ret`
//     return (BOOL)Default();                                                // 0x289090
// TODO(clean-room): partially transcribed.  The two virtuals are not reached:
// CMDIFrameWndEx::OnEraseMDIClientBackground has no thunk in this tree at all
// and the CMFCVisualManager singleton is only reachable as a C++ static
// (see featurepack/docking/CBasePane.cpp's DoPaint note).  Both retail base
// implementations return FALSE, so with the default visual manager and an
// un-overridden frame this body is exact; a custom visual manager's MDI
// background is not painted here.
// Symbol: ?OnEraseBkgnd@CMDIClientAreaWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMDIClientAreaWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pDC;
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return FALSE;
    }
    if (self->m_bIsMDITabbedGroup) {
        HWND hActive = reinterpret_cast<HWND>(::SendMessage(WndHandle(self), WM_MDIGETACTIVE, 0, 0));
        if (hActive != nullptr && ::IsWindowVisible(hActive)) {
            return TRUE;
        }
    }
    (void)AsFrameEx(impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self)));   // the +0x408 hook is unreachable, see above
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(SelfWnd(self)));
}

// CMDIClientAreaWnd::OnGetDragBounds — transcribed from retail entry RVA
// 0x7fcb0 (mfc140):
//     if (!m_bIsMDITabbedGroup || m_lstTabbedGroups.GetCount() == 0 || !wp) return 0;
//     CMFCTabCtrl* pNext = GetNextTabWnd((CMFCTabCtrl*)wp, FALSE);  if (!pNext) return 0;   // 0x80e20
//     CRect a, b; ::GetWindowRect(wp->m_hWnd, &a); ::GetWindowRect(pNext->m_hWnd, &b);
//     ::UnionRect(&a, &a, &b);
//     if (m_groupAlignment == GROUP_VERT_ALIGN) { a.left += m_nResizeMargin; a.right -= m_nResizeMargin; }
//     else                                       { a.top += m_nResizeMargin; a.bottom -= m_nResizeMargin; }
//     ::CopyRect((RECT*)lp, &a);  return 1;
// Symbol: ?OnGetDragBounds@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetDragBounds_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    void* pGroup = reinterpret_cast<void*>(wParam);
    RECT* pOut = reinterpret_cast<RECT*>(lParam);
    if (self == nullptr || !self->m_bIsMDITabbedGroup || self->m_lstTabbedGroups.nCount == 0 || pGroup == nullptr || pOut == nullptr) {
        return 0;
    }
    void* pNext = impl__GetNextTabWnd_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_H_Z(pThis, pGroup, FALSE);
    if (pNext == nullptr) {
        return 0;
    }
    RECT rcA = {};
    RECT rcB = {};
    ::GetWindowRect(WndHandle(pGroup), &rcA);
    ::GetWindowRect(WndHandle(pNext), &rcB);
    ::UnionRect(&rcA, &rcA, &rcB);
    if (self->m_groupAlignment == kGroupVertAlign) {
        rcA.left += self->m_nResizeMargin;
        rcA.right -= self->m_nResizeMargin;
    } else {
        rcA.top += self->m_nResizeMargin;
        rcA.bottom -= self->m_nResizeMargin;
    }
    ::CopyRect(pOut, &rcA);
    return 1;
}

// Symbol: ?OnMDIDestroy@CMDIClientAreaWnd@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnMDIDestroy_CMDIClientAreaWnd__IEAAXPEAVCWnd___Z(void* pThis, void* pWnd) {
    auto* self = Self(pThis);
    if (!self) return;
    if (pWnd) RemoveDocumentTab(*self, WndHandle(pWnd));
    impl__Default_CWnd__IEAA_JXZ(SelfWnd(self));
    impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(self, TRUE);
}

// Symbol: ?OnMDINext@CMDIClientAreaWnd@@IEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnMDINext_CMDIClientAreaWnd__IEAAXPEAVCWnd__H_Z(void* pThis, void* pWnd, int previous) {
    auto* self = Self(pThis);
    if (!self || !WndHandle(self)) return;
    if (self->m_bIsMDITabbedGroup && ActivateAdjacentTab(*self, WndHandle(pWnd), previous != 0)) return;
    impl__Default_CWnd__IEAA_JXZ(SelfWnd(self));
}

// CMDIClientAreaWnd::OnMDIRefreshMenu — retail entry RVA 0x7d930 (mfc140):
//     LRESULT lres = Default();                                             // 0x289090
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     if (pFrame) {
//         if (pFrame->[+0x6a0]) pFrame->[+0x1e8] = pFrame->vslot(+0x398)(pFrame->[+0x6a0]->[+0x1360]);   // GetWindowMenuPopup(HMENU)
//         if (pFrame->[+0x710]) CMFCRibbonBar::SetActiveMDIChild(pFrame->[+0x710], pFrame->MDIGetActive(NULL));   // 0xe0e70 / 0x2a4a00
//     }
//     return (HMENU)lres;
// TODO(clean-room): partially transcribed -- only the Default() forward and
// the frame downcast are made.  The menu-bar (+0x6a0/+0x1e8) and ribbon-bar
// (+0x710) refreshes need CMDIFrameWndEx members OpenMFC's padded frame does
// not carry.
// Symbol: ?OnMDIRefreshMenu@CMDIClientAreaWnd@@IEAAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__OnMDIRefreshMenu_CMDIClientAreaWnd__IEAAPEAUHMENU____XZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    LRESULT lres = impl__Default_CWnd__IEAA_JXZ(SelfWnd(self));
    (void)AsFrameEx(impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self)));
    return reinterpret_cast<HMENU>(lres);
}

// CMDIClientAreaWnd::OnMDISetMenu — transcribed from retail entry RVA
// 0x7d8a0 (mfc140):
//     HMENU hFrameMenu = NULL;
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     if (pFrame && ::IsWindow(pFrame->m_hWnd) && !pFrame->OnSetMenu(hmenuFrame))   // vslot +0x3d0; `cmove` picks hmenuFrame when it returned 0
//         hFrameMenu = hmenuFrame;
//     return (HMENU)DefWindowProc(WM_MDISETMENU, (WPARAM)hFrameMenu, (LPARAM)hmenuWindow);   // vslot +0x248
// i.e. the frame menu is forwarded to the MDI client only when the frame did
// NOT take it (no CMDIFrameWndEx parent -> nothing is forwarded either).
// OpenMFC's ?OnSetMenu@CMDIFrameWndEx@@ thunk is a generated placeholder that
// returns 0, so today hmenuFrame is always forwarded.
// Symbol: ?OnMDISetMenu@CMDIClientAreaWnd@@IEAAPEAUHMENU__@@PEAU2@0@Z
extern "C" HMENU MS_ABI impl__OnMDISetMenu_CMDIClientAreaWnd__IEAAPEAUHMENU____PEAU2_0_Z(void* pThis, HMENU hmenuFrame, HMENU hmenuWindow) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    HMENU hFrameMenu = nullptr;
    CMDIFrameWndEx* pFrame = AsFrameEx(impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self)));
    if (pFrame != nullptr && ::IsWindow(WndHandle(pFrame))
        && !impl__OnSetMenu_CMDIFrameWndEx__MEAAHPEAUHMENU_____Z(pFrame, hmenuFrame)) {
        hFrameMenu = hmenuFrame;
    }
    return reinterpret_cast<HMENU>(impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
        SelfWnd(self), WM_MDISETMENU, reinterpret_cast<WPARAM>(hFrameMenu), reinterpret_cast<LPARAM>(hmenuWindow)));
}

// Symbol: ?OnMoveTabComplete@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnMoveTabComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wp, __int64 lp) {
    auto* self = Self(pThis);
    if (!self) return 0;
    const TabDrag drag = Record(self)->runtime->drag;
    const HWND client = WndHandle(self);
    CancelTabDrag(*self);
    if (Self(pThis) != self || WndHandle(self) != client) return 0;
    auto* source = reinterpret_cast<CMFCTabCtrl*>(wp);
    if (!self->m_bIsMDITabbedGroup || !self->m_bTabIsVisible ||
        !ListFind(self->m_lstTabbedGroups, source)) return 0;
    const int index = drag.source == source->m_hWnd ? TabFromHwnd(source, drag.document) : source->GetActiveTab();
    POINT point{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
    ::ClientToScreen(source->m_hWnd, &point);
    const TabDrop drop = FindTabDrop(*self, *source, index, point);
    switch (drop.action) {
    case TabDropAction::reorder:
        ReorderGroupTab(*source, index, drop.index);
        break;
    case TabDropAction::split:
        if (auto* target = impl__CreateNewTabGroup_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_VCRect__H_Z(
                self, drop.target, &drop.outline, drop.vertical)) {
            if (!impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(self, source, target, index))
                impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(self, target, TRUE);
        }
        break;
    case TabDropAction::join:
        impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(self, source, drop.target, index);
        break;
    case TabDropAction::none:
        break;
    }
    return 0;
}

// CMDIClientAreaWnd::OnStyleChanging — transcribed from retail entry RVA
// 0x7fbf0 (mfc140):
//     if (nStyleType == GWL_EXSTYLE && !IsKeepClientEdge())                  // 0x7fc30
//         lpss->styleNew = lpss->styleOld & ~WS_EX_CLIENTEDGE;               // `btr $9` on styleOld (+0), stored to styleNew (+4)
//     Default();                                                             // tail jmp 0x289090
// Symbol: ?OnStyleChanging@CMDIClientAreaWnd@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanging_CMDIClientAreaWnd__IEAAXHPEAUtagSTYLESTRUCT___Z(void* pThis, int nStyleType, STYLESTRUCT* lpss) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    if (nStyleType == GWL_EXSTYLE && lpss != nullptr && !impl__IsKeepClientEdge_CMDIClientAreaWnd__AEAAHXZ(pThis)) {
        lpss->styleNew = lpss->styleOld & ~static_cast<DWORD>(WS_EX_CLIENTEDGE);
    }
    impl__Default_CWnd__IEAA_JXZ(SelfWnd(self));
}

// Symbol: ?OnTabGroupMouseMove@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnTabGroupMouseMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64, __int64 lp) {
    auto* self = Self(pThis);
    if (!self || !self->m_bIsMDITabbedGroup || !self->m_bTabIsVisible) return 0;
    auto* source = GroupTabWindow(*self, ::GetCapture());
    if (!source) return 0;
    const auto& drag = Record(self)->runtime->drag;
    const int index = drag.source == source->m_hWnd ? TabFromHwnd(source, drag.document) : source->GetActiveTab();
    POINT point{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
    ::ClientToScreen(source->m_hWnd, &point);
    const TabDrop drop = FindTabDrop(*self, *source, index, point);
    PreviewTabDrop(*self, drop.outline);
    ::SetCursor(::LoadCursorW(nullptr, drop.action == TabDropAction::none ? IDC_NO : IDC_SIZEALL));
    return 0;
}

// CMDIClientAreaWnd::PreSubclassWindow — transcribed from retail entry RVA
// 0x7ecb0 (mfc140, virtual). The whole body is a tail call through this
// object's own vtable:
//     mov (%rcx),%rax ; lea 0xe8(%rcx),%rdx ; mov 0x2d8(%rax),%rax ; jmp CFG
// i.e. `CreateTabGroup(&m_wndTab)` -- vslot +0x2d8 of the CMDIClientAreaWnd
// vftable (0x2eb4f8, mfc140) is ?CreateTabGroup@CMDIClientAreaWnd@@.  With
// no MSVC vtable the call goes to the thunk directly (an app override of
// CreateTabGroup is not honoured).
// Symbol: ?PreSubclassWindow@CMDIClientAreaWnd@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CMDIClientAreaWnd__MEAAXXZ(void* pThis) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(pThis, self->m_wndTab);
}

// CMDIClientAreaWnd::RemoveTabGroup — transcribed from retail entry RVA
// 0x808b0 (mfc140; 0x80320 in mfc140u):
//     POSITION pos = m_lstTabbedGroups.Find(pTabWnd);  if (!pos) return;
//     node = (pos == m_lstTabbedGroups.m_pNodeHead) ? pos->pNext : pos->pPrev;   // `cmp %rbx,0x8(%r15)` is the HEAD
//     CMFCTabCtrl* pNeighbour = node ? DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data) : NULL;
//     m_lstTabbedGroups.RemoveAt(pos);                                       // 0x2306f0
//     pTabWnd->ShowWindow(SW_HIDE);                                          // 0x2a79e0
//     if (m_lstTabbedGroups.GetCount() > 0) {
//         CMFCTabCtrl* pLast = DYNAMIC_DOWNCAST(CMFCTabCtrl, GetTail());
//         pLast->m_ResizeMode = RESIZE_NO;  pLast->RecalcLayout();          // +0x3d28 (through NULL on a failed downcast), vslot +0x318
//     }
//     if (m_lstTabbedGroups.GetCount() <= 1) m_groupAlignment = GROUP_NO_ALIGN;
//     if (pNeighbour) {
//         CRect a, b; ::GetWindowRect(pTabWnd->m_hWnd, &a); ::GetWindowRect(pNeighbour->m_hWnd, &b);
//         ::UnionRect(&b, &b, &a);  ScreenToClient(&b);                       // 0x2a11f0
//         pNeighbour->SetWindowPos(NULL, b.left, b.top, b.Width(), b.Height(), SWP_NOZORDER);   // 0x2a7970
//         AdjustMDIChildren(pNeighbour);                                     // 0x7e5f0
//     }
//     CImageList* pImages;
//     if (m_mapTabIcons.Lookup(pTabWnd, pImages) && pImages) {              // 0x1c07c
//         delete pImages;                                                    // vtable slot 1
//         if (m_mapTabIcons.m_pHashTable) { (inlined RemoveKey: hash, unlink, --count) if (count == 0) RemoveAll(); }   // 0x1bad0
//     }
//     m_lstRemovedTabbedGroups.AddTail(pTabWnd);                             // 0x230490
//     pTabWnd->ShowWindow(SW_HIDE);
// The group is NEVER deleted here (only the destructor deletes groups, via
// the removed list) and the BOOL argument is dead: no instruction reads %r8d.
// Symbol: ?RemoveTabGroup@CMDIClientAreaWnd@@QEAAXPEAVCMFCTabCtrl@@H@Z
extern "C" void MS_ABI impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(void* pThis, void* pTabGroup, int bRecalcLayout) {
    (void)bRecalcLayout;   // dead in retail
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || pTabGroup == nullptr) {
        return;
    }
    RetailNode* pos = ListFind(self->m_lstTabbedGroups, pTabGroup);
    if (pos == nullptr) {
        return;
    }
    CancelTabDrag(*self);
    RetailNode* neighbourNode = (pos == self->m_lstTabbedGroups.pNodeHead) ? pos->pNext : pos->pPrev;
    CMFCTabCtrl* pNeighbour = neighbourNode != nullptr ? AsTabCtrl(neighbourNode->data) : nullptr;
    ListRemoveAt(self->m_lstTabbedGroups, pos);
    PublishGroupList(*self);
    impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pTabGroup), SW_HIDE);
    if (self->m_lstTabbedGroups.nCount > 0) {
        CMFCTabCtrl* pLast = AsTabCtrl(self->m_lstTabbedGroups.pNodeTail->data);
        if (pLast != nullptr) {   // retail stores through NULL here
            TabResizeMode(pLast) = kGroupNoAlign;
            impl__RecalcLayout_CMFCTabCtrl__UEAAXXZ(pLast);
        }
    }
    if (self->m_lstTabbedGroups.nCount <= 1) {
        self->m_groupAlignment = kGroupNoAlign;
    }
    if (pNeighbour != nullptr) {
        RECT rcGroup = {};
        RECT rcNeighbour = {};
        ::GetWindowRect(WndHandle(pTabGroup), &rcGroup);
        ::GetWindowRect(WndHandle(pNeighbour), &rcNeighbour);
        ::UnionRect(&rcNeighbour, &rcNeighbour, &rcGroup);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(SelfWnd(self), &rcNeighbour);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pNeighbour, nullptr, rcNeighbour.left, rcNeighbour.top,
                                                    rcNeighbour.right - rcNeighbour.left, rcNeighbour.bottom - rcNeighbour.top,
                                                    SWP_NOZORDER);
        impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pNeighbour);
    }
    InstanceRecord* rec = Record(pThis);
    TabIconEntry* e = TabIconsLookup(rec, AsWnd(pTabGroup));
    if (e != nullptr && e->value != nullptr) {
        DeleteImageListObject(e->value);
        e->value = nullptr;
        TabIconsRemoveKey(rec, self, AsWnd(pTabGroup));
    }
    ListAddTail(self->m_lstRemovedTabbedGroups, pTabGroup);
    impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pTabGroup), SW_HIDE);
}

namespace {
constexpr wchar_t mdiProfileValue[] = L"MDITabsState";
constexpr DWORD maxMdiProfileBytes = 64 * 1024 * 1024;

struct SessionDocument {
    CString path;
    CString label;
    CObject* state = nullptr;
    int paneId = -1;
    int resizeMode = kGroupNoAlign;
    COLORREF color = COLORREF(-1);
    WINDOWPLACEMENT placement{sizeof(WINDOWPLACEMENT)};
    BOOL active = FALSE;
};

struct SessionGroup {
    std::vector<SessionDocument> documents;
    std::vector<COLORREF> autoColors;
    RECT bounds{};
    int selected = -1;
    BOOL active = FALSE;
    BOOL autoColor = FALSE;
    BOOL documentMenu = FALSE;
    BOOL swapTabs = TRUE;
    int border = -1;
};

struct MdiSession {
    CMDITabInfo options;
    BOOL tabs = FALSE;
    BOOL groups = FALSE;
    BOOL visible = FALSE;
    int alignment = kGroupNoAlign;
    int resizeMargin = 40;
    int newGroupMargin = 40;
    std::vector<SessionDocument> windows;
    std::vector<SessionGroup> tabGroups;
    std::unordered_map<CObject*, std::unique_ptr<CObject>> objects;

    CObject* Own(CObject* object) {
        if (object && !objects.count(object)) {
            std::unique_ptr<CObject> ownership(object);
            objects.emplace(object, std::move(ownership));
        }
        return object;
    }
};

CString MdiProfilePath(const wchar_t* profile, UINT id) {
    alignas(CString) unsigned char storage[sizeof(CString)];
    auto* root = reinterpret_cast<CString*>(storage);
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        root, L"MDIClientArea", profile);
    CString prefix(std::move(*root));
    root->~CString();
    if (!prefix.IsEmpty() && prefix.GetString()[prefix.GetLength() - 1] != L'\\') prefix += L'\\';
    CString section;
    section.Format(L"%sMDIClientArea-%d", prefix.GetString(), static_cast<int>(id));
    return section;
}

int ReadSessionCount(CArchive& ar) {
    int count;
    ar >> count;
    if (count < 0 || count > 65535) AfxThrowArchiveException(CArchiveException::badIndex);
    return count;
}

CString ReadSessionString(CArchive& ar) {
    int characterSize = 0;
    const auto length = AfxReadStringLength(ar, characterSize);
    if (length > 65535 || (characterSize != 1 && characterSize != sizeof(wchar_t)))
        AfxThrowArchiveException(CArchiveException::badIndex);
    const auto count = static_cast<int>(length);
    if (!count) return CString{};
    if (characterSize == 1) {
        std::vector<char> text(count);
        ar.EnsureRead(text.data(), count);
        return CString(text.data(), count);
    }
    std::vector<wchar_t> text(count);
    ar.EnsureRead(text.data(), count * sizeof(wchar_t));
    return CString(text.data(), count);
}

SessionDocument CaptureSessionDocument(MdiSession& session, CMDIChildWndEx& child) {
    SessionDocument document;
    CObject* state = nullptr;
    const auto* path = child.GetDocumentName(&state);
    if (path) document.path = path;
    document.state = session.Own(state);
    if (auto* pane = child.GetTabbedPane(); pane && pane->m_hWnd)
        document.paneId = ::GetDlgCtrlID(pane->m_hWnd);
    return document;
}

void WriteDocumentIdentity(CArchive& ar, const SessionDocument& document) {
    ar << document.path << BOOL(document.state != nullptr);
    if (document.state) ar.WriteObject(document.state);
}

SessionDocument ReadDocumentIdentity(CArchive& ar, MdiSession& session) {
    SessionDocument document;
    document.path = ReadSessionString(ar);
    BOOL hasState;
    ar >> hasState;
    if (hasState) document.state = session.Own(ar.ReadObject(nullptr));
    return document;
}

void WriteSessionWindows(CArchive& ar, S_CMDIClientAreaWnd& self, MdiSession& session) {
    const HWND client = WndHandle(&self);
    const HWND active = reinterpret_cast<HWND>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0));
    std::vector<SessionDocument> windows;
    for (HWND hwnd = ::GetWindow(client, GW_CHILD); hwnd; hwnd = ::GetWindow(hwnd, GW_HWNDNEXT)) {
        auto* child = AsChildEx(static_cast<CWnd*>(CWnd::FromHandlePermanent(hwnd)));
        if (!child || !impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(child)) continue;
        auto document = CaptureSessionDocument(session, *child);
        if (!::GetWindowPlacement(hwnd, &document.placement)) continue;
        document.active = hwnd == active;
        windows.push_back(std::move(document));
    }
    ar << static_cast<int>(windows.size());
    for (auto it = windows.rbegin(); it != windows.rend(); ++it) {
        const auto& doc = *it;
        const auto& wp = doc.placement;
        WriteDocumentIdentity(ar, doc);
        ar << wp.flags << wp.length;
        ar.Write(&wp.ptMaxPosition, sizeof(POINT));
        ar.Write(&wp.ptMinPosition, sizeof(POINT));
        ar.Write(&wp.rcNormalPosition, sizeof(RECT));
        ar << wp.showCmd << doc.active << doc.paneId;
    }
}

std::vector<SessionDocument> ReadSessionWindows(CArchive& ar, MdiSession& session) {
    std::vector<SessionDocument> windows;
    const int count = ReadSessionCount(ar);
    for (int i = 0; i < count; ++i) {
        auto doc = ReadDocumentIdentity(ar, session);
        auto& wp = doc.placement;
        ar >> wp.flags >> wp.length;
        if (wp.length != sizeof(WINDOWPLACEMENT)) AfxThrowArchiveException(CArchiveException::badIndex);
        ar.EnsureRead(&wp.ptMaxPosition, sizeof(POINT));
        ar.EnsureRead(&wp.ptMinPosition, sizeof(POINT));
        ar.EnsureRead(&wp.rcNormalPosition, sizeof(RECT));
        ar >> wp.showCmd >> doc.active >> doc.paneId;
        windows.push_back(std::move(doc));
    }
    return windows;
}

void WriteSessionGroup(CArchive& ar, S_CMDIClientAreaWnd& self, CMFCTabCtrl& tabs, MdiSession& session) {
    const auto* state = openmfc::detail::mfccore::FindTabCtrlState(&tabs);
    std::vector<SessionDocument> documents;
    int selected = -1;
    for (int i = 0; state && i < tabs.GetTabsCount(); ++i) {
        auto* child = AsChildEx(tabs.GetTabWnd(i));
        if (!child || state->tabs[i].temporary) continue;
        if (tabs.GetActiveTab() == i) selected = static_cast<int>(documents.size());
        auto doc = CaptureSessionDocument(session, *child);
        doc.label = state->tabs[i].label.c_str();
        doc.color = state->tabs[i].backgroundColor;
        doc.resizeMode = TabResizeMode(&tabs);
        documents.push_back(std::move(doc));
    }
    ar << static_cast<int>(documents.size()) << selected << TabActiveInMDIGroup(&tabs);
    for (const auto& doc : documents) {
        WriteDocumentIdentity(ar, doc);
        ar << doc.label << doc.resizeMode << doc.color << doc.paneId;
    }
    ar << (state ? state->autoColor : self.m_mdiTabParams.m_bAutoColor)
        << (state ? static_cast<int>(state->autoColors.size()) : 0);
    if (state) {
        for (auto color : state->autoColors) ar << color;
    }
    ar << (state ? state->documentsMenu : self.m_mdiTabParams.m_bDocumentMenu)
        << (state ? state->tabSwapEnabled : self.m_mdiTabParams.m_bEnableTabSwap)
        << (state ? state->tabBorderSize : self.m_mdiTabParams.m_nTabBorderSize);
    RECT bounds{};
    ::GetWindowRect(tabs.m_hWnd, &bounds);
    ar.Write(&bounds, sizeof(bounds));
}

SessionGroup ReadSessionGroup(CArchive& ar, MdiSession& session) {
    SessionGroup group;
    const int count = ReadSessionCount(ar);
    ar >> group.selected >> group.active;
    if (group.selected < -1 || group.selected >= count)
        AfxThrowArchiveException(CArchiveException::badIndex);
    for (int i = 0; i < count; ++i) {
        auto doc = ReadDocumentIdentity(ar, session);
        doc.label = ReadSessionString(ar);
        ar >> doc.resizeMode >> doc.color >> doc.paneId;
        group.documents.push_back(std::move(doc));
    }
    ar >> group.autoColor;
    for (int colors = ReadSessionCount(ar); colors > 0; --colors) {
        COLORREF color;
        ar >> color;
        group.autoColors.push_back(color);
    }
    ar >> group.documentMenu >> group.swapTabs >> group.border;
    ar.EnsureRead(&group.bounds, sizeof(group.bounds));
    return group;
}

CMDIFrameWndEx* SessionFrame(S_CMDIClientAreaWnd& self) {
    auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(WndHandle(&self))));
    return window && window->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))
        ? static_cast<CMDIFrameWndEx*>(window) : nullptr;
}

CMDIChildWndEx* ReopenSessionDocument(S_CMDIClientAreaWnd& self, const SessionDocument& doc) {
    auto* frame = SessionFrame(self);
    if (!frame) return nullptr;
    CMDIChildWndEx* child = nullptr;
    if (!doc.path.IsEmpty()) {
        child = frame->CreateDocumentWindow(doc.path, doc.state);
        if (!child) child = frame->CreateNewWindow(doc.path, doc.state);
    } else if (doc.paneId != -1) {
        auto* manager = openmfc::detail::mfccore::DockingManagerForMdiFrame(frame);
        auto* pane = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(manager, doc.paneId, TRUE);
        if (!pane) {
            auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetDlgItem(frame->m_hWnd, doc.paneId)));
            if (window && window->IsKindOf(RUNTIME_CLASS(CDockablePane))) pane = static_cast<CDockablePane*>(window);
        }
        if (pane && pane->IsKindOf(RUNTIME_CLASS(CDockablePane)))
            child = frame->ControlBarToTabbedDocument(static_cast<CDockablePane*>(pane));
    }
    return child && ::GetParent(child->m_hWnd) == WndHandle(&self) ? child : nullptr;
}

HWND RestoreSessionWindows(S_CMDIClientAreaWnd& self, const std::vector<SessionDocument>& windows) {
    HWND active = nullptr;
    for (const auto& entry : windows) {
        auto* child = ReopenSessionDocument(self, entry);
        if (!child) continue;
        ::SetWindowPlacement(child->m_hWnd, &entry.placement);
        if (entry.active) active = child->m_hWnd;
    }
    return active;
}

HWND RestoreSessionGroup(S_CMDIClientAreaWnd& self, CMFCTabCtrl& tabs, const SessionGroup& group, bool related) {
    auto& state = openmfc::detail::mfccore::EnsureTabCtrlState(&tabs);
    HWND selected = nullptr;
    auto options = self.m_mdiTabParams;
    options.m_bDocumentMenu = group.documentMenu;
    options.m_bAutoColor = group.autoColor;
    options.m_bEnableTabSwap = group.swapTabs;
    options.m_nTabBorderSize = group.border;
    tabs.EnableTabSwap(options.m_bEnableTabSwap);
    tabs.SetTabBorderSize(options.m_nTabBorderSize);
    impl__EnableTabDocumentsMenu_CMFCTabCtrl__QEAAXH_Z(&tabs, options.m_bDocumentMenu);
    impl__EnableAutoColor_CMFCBaseTabCtrl__QEAAXH_Z(&tabs, options.m_bAutoColor);
    if (!group.autoColors.empty()) {
        CArray<COLORREF, COLORREF> palette;
        for (auto color : group.autoColors) palette.Add(color);
        impl__SetAutoColors_CMFCBaseTabCtrl__QEAAXAEBV__CArray_KK___Z(&tabs, palette);
    }

    for (int i = 0; i < static_cast<int>(group.documents.size()); ++i) {
        const auto& saved = group.documents[i];
        auto* child = ReopenSessionDocument(self, saved);
        if (!child || !tabs.InsertTab(child, saved.label, tabs.GetTabsCount(), UINT(-1), TRUE)) continue;
        ::ShowWindow(child->m_hWnd, SW_SHOWNOACTIVATE);
        SyncDocumentTab(self, tabs, *child);
        state.tabs.back().label = static_cast<const wchar_t*>(saved.label);
        state.tabs.back().backgroundColor = saved.color;
        TabResizeMode(&tabs) = saved.resizeMode;
        if (related) SetChildRelatedTabGroup(child, &tabs);
        if (i == group.selected) selected = child->m_hWnd;
    }
    if (selected) tabs.SetActiveTab(TabFromHwnd(&tabs, selected));
    else if (auto* child = tabs.GetTabWnd(tabs.GetActiveTab())) selected = child->m_hWnd;
    TabActiveInMDIGroup(&tabs) = group.active;
    if (related) {
        // Group layout uses the extents as weights and fits them to the current client.
        const auto width = (std::clamp)(LONGLONG(group.bounds.right) - group.bounds.left, 1LL, 32767LL);
        const auto height = (std::clamp)(LONGLONG(group.bounds.bottom) - group.bounds.top, 1LL, 32767LL);
        ::SetWindowPos(tabs.m_hWnd, nullptr, 0, 0, static_cast<int>(width), static_cast<int>(height),
            SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
    }
    return selected;
}

bool CloseSessionWindows(S_CMDIClientAreaWnd& self) {
    const HWND client = WndHandle(&self);
    std::vector<HWND> windows;
    std::unordered_set<CDocument*> checked;
    std::vector<std::pair<CDocTemplate*, CDocument*>> documents;
    for (HWND child = ::GetWindow(client, GW_CHILD); child; child = ::GetWindow(child, GW_HWNDNEXT)) {
        auto* frame = AsChildEx(static_cast<CWnd*>(CWnd::FromHandlePermanent(child)));
        if (!frame) continue;
        windows.push_back(child);
    }
    for (HWND child : windows) {
        auto* frame = AsChildEx(static_cast<CWnd*>(CWnd::FromHandlePermanent(child)));
        auto* doc = frame ? frame->GetActiveDocument() : nullptr;
        if (!doc || !checked.insert(doc).second) continue;
        if (doc->IsModified() && !doc->SaveModified()) return false;
        if (auto* type = doc->GetDocTemplate()) documents.emplace_back(type, doc);
    }
    bool closed = true;
    for (HWND child : windows) {
        if (::GetParent(child) != client) continue;
        ::SendMessageW(child, WM_CLOSE, 0, 0);
        if (::IsWindow(child)) {
            closed = false;
            break;
        }
    }
    // A window can close its document itself. Check the template before
    // disposing of an auto-delete document whose final view was closed here.
    for (const auto& [type, closedDoc] : documents) {
        for (auto pos = type->GetFirstDocPosition(); pos;) {
            auto* doc = type->GetNextDoc(pos);
            if (doc != closedDoc) continue;
            if (doc->m_bAutoDelete && !doc->GetFirstViewPosition()) doc->OnCloseDocument();
            break;
        }
    }
    return closed;
}

void RestoreSession(S_CMDIClientAreaWnd& self, const MdiSession& session) {
    if (!SessionFrame(self)) AfxThrowArchiveException(CArchiveException::generic);
    CancelTabDrag(self);
    CancelSplitterDrag(self);
    const HWND client = WndHandle(&self);
    struct PauseUpdates {
        BOOL& flag;
        BOOL previous;
        ~PauseUpdates() { flag = previous; }
    } pause{self.m_bDisableUpdateTabs, self.m_bDisableUpdateTabs};
    self.m_bDisableUpdateTabs = TRUE;
    if (!CloseSessionWindows(self)) AfxThrowArchiveException(CArchiveException::generic);

    while (self.m_lstTabbedGroups.pNodeHead)
        impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(
            &self, self.m_lstTabbedGroups.pNodeHead->data, FALSE);
    auto* strip = EmbeddedTab(&self);
    for (int i = strip->GetTabsCount() - 1; i >= 0; --i) strip->RemoveTab(i);
    self.m_mdiTabParams = session.options;
    self.m_bTabIsEnabled = session.tabs;
    self.m_bIsMDITabbedGroup = session.groups;
    self.m_bTabIsVisible = session.visible;
    self.m_groupAlignment = session.alignment;
    self.m_nResizeMargin = session.resizeMargin;
    self.m_nNewGroupMargin = session.newGroupMargin;
    if (!session.tabs && strip->m_hWnd) ::ShowWindow(strip->m_hWnd, SW_HIDE);

    HWND active = nullptr;
    if (session.tabs) {
        const auto& options = session.options;
        if (!strip->m_hWnd && !strip->CreateTabs(options.m_style, RECT{}, SelfWnd(&self), UINT(-1),
                options.m_tabLocation, options.m_bTabCloseButton))
            AfxThrowArchiveException(CArchiveException::generic);
        impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(&self, strip);
        active = RestoreSessionGroup(self, *strip, session.tabGroups.front(), false);
    } else if (session.groups) {
        HWND fallback = nullptr;
        for (const auto& group : session.tabGroups) {
            auto* tabs = static_cast<CMFCTabCtrl*>(impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(&self, nullptr));
            if (!tabs) break;
            const HWND current = RestoreSessionGroup(self, *tabs, group, true);
            if (tabs->GetTabsCount() == 0) {
                auto* record = Record(&self);
                if (auto* icons = TabIconsLookup(record, AsWnd(tabs))) {
                    if (icons->value) DeleteImageListObject(icons->value);
                    TabIconsRemoveKey(record, &self, AsWnd(tabs));
                }
                DeleteTabCtrlObject(tabs);
                continue;
            }
            ListAddTail(self.m_lstTabbedGroups, tabs);
            if (!fallback) fallback = current;
            if (group.active) active = current;
        }
        PublishGroupList(self);
        if (!active) active = fallback;
    } else {
        active = RestoreSessionWindows(self, session.windows);
    }

    if (active) ::SendMessageW(client, WM_MDIACTIVATE, reinterpret_cast<WPARAM>(active), 0);
    // Register destruction and text notifications before a restored child can close.
    if (session.tabs || session.groups) CollectTabbedDocuments(self);
    if (session.groups) LayoutDocumentGroups(self);
    else if (session.tabs) LayoutDocumentTabs(self);
}
}

// Symbol: ?SaveState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CMDIClientAreaWnd__QEAAHPEB_WI_Z(void* pThis, const wchar_t* profile, unsigned int id) {
    auto* self = Self(pThis);
    if (!self || !WndHandle(self)) return FALSE;
    try {
        CMemFile file;
        CArchive archive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
        impl__Serialize_CMDIClientAreaWnd__UEAAXAEAVCArchive___Z(self, &archive);
        archive.Close();
        const auto size = file.GetLength();
        if (!size || size > maxMdiProfileBytes) return FALSE;
        std::unique_ptr<BYTE, decltype(&std::free)> bytes(file.Detach(), &std::free);
        const auto section = MdiProfilePath(profile, id);
        HKEY key = nullptr;
        if (::RegCreateKeyExW(HKEY_CURRENT_USER, section, 0, nullptr, 0, KEY_SET_VALUE, nullptr, &key, nullptr) != ERROR_SUCCESS)
            return FALSE;
        const auto result = ::RegSetValueExW(key, mdiProfileValue, 0, REG_BINARY, bytes.get(), static_cast<DWORD>(size));
        ::RegCloseKey(key);
        return result == ERROR_SUCCESS;
    } catch (CException* error) {
        error->Delete();
        return FALSE;
    }
}

// Symbol: ?LoadState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CMDIClientAreaWnd__QEAAHPEB_WI_Z(void* pThis, const wchar_t* profile, unsigned int id) {
    auto* self = Self(pThis);
    if (!self || !WndHandle(self)) return FALSE;
    const auto section = MdiProfilePath(profile, id);
    DWORD size = 0;
    if (::RegGetValueW(HKEY_CURRENT_USER, section, mdiProfileValue, RRF_RT_REG_BINARY, nullptr, nullptr, &size) != ERROR_SUCCESS
        || !size || size > maxMdiProfileBytes) return FALSE;
    std::vector<BYTE> bytes(size);
    if (::RegGetValueW(HKEY_CURRENT_USER, section, mdiProfileValue, RRF_RT_REG_BINARY, nullptr, bytes.data(), &size) != ERROR_SUCCESS
        || !size || size > bytes.size()) return FALSE;
    try {
        CMemFile file(bytes.data(), size);
        CArchive archive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
        impl__Serialize_CMDIClientAreaWnd__UEAAXAEAVCArchive___Z(self, &archive);
        archive.Close();
        return TRUE;
    } catch (CException* error) {
        error->Delete();
        return FALSE;
    }
}

// Symbol: ?Serialize@CMDIClientAreaWnd@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDIClientAreaWnd__UEAAXAEAVCArchive___Z(void* pThis, void* archive) {
    auto* self = Self(pThis);
    if (!self || !archive) return;
    auto& ar = *static_cast<CArchive*>(archive);
    MdiSession session;
    if (ar.IsStoring()) {
        self->m_mdiTabParams.Serialize(ar);
        ar << self->m_bTabIsEnabled << self->m_bIsMDITabbedGroup << self->m_bTabIsVisible
            << self->m_groupAlignment << self->m_nResizeMargin << self->m_nNewGroupMargin;
        if (self->m_bTabIsEnabled) {
            WriteSessionGroup(ar, *self, *EmbeddedTab(self), session);
        } else if (self->m_bIsMDITabbedGroup) {
            ar << static_cast<int>(self->m_lstTabbedGroups.nCount);
            for (auto* node = self->m_lstTabbedGroups.pNodeHead; node; node = node->pNext)
                WriteSessionGroup(ar, *self, *static_cast<CMFCTabCtrl*>(node->data), session);
        } else {
            WriteSessionWindows(ar, *self, session);
        }
    } else {
        session.options = self->m_mdiTabParams;
        session.options.Serialize(ar);
        ar >> session.tabs >> session.groups >> session.visible
            >> session.alignment >> session.resizeMargin >> session.newGroupMargin;
        if ((session.tabs && session.groups) || session.alignment < kGroupNoAlign || session.alignment > kGroupHorzAlign
            || session.resizeMargin < 0 || session.resizeMargin > 32767
            || session.newGroupMargin < 0 || session.newGroupMargin > 32767)
            AfxThrowArchiveException(CArchiveException::badIndex);
        if (session.tabs || session.groups) {
            const int count = session.tabs ? 1 : ReadSessionCount(ar);
            for (int i = 0; i < count; ++i) session.tabGroups.push_back(ReadSessionGroup(ar, session));
        } else {
            session.windows = ReadSessionWindows(ar, session);
        }
        RestoreSession(*self, session);
    }
}

// Symbol: ?SerializeOpenChildren@CMDIClientAreaWnd@@AEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeOpenChildren_CMDIClientAreaWnd__AEAAXAEAVCArchive___Z(void* pThis, void* archive) {
    auto* self = Self(pThis);
    if (!self || !archive) return;
    auto& ar = *static_cast<CArchive*>(archive);
    MdiSession session;
    if (ar.IsStoring()) {
        WriteSessionWindows(ar, *self, session);
    } else {
        const auto windows = ReadSessionWindows(ar, session);
        if (auto active = RestoreSessionWindows(*self, windows))
            ::SendMessageW(WndHandle(self), WM_MDIACTIVATE, reinterpret_cast<WPARAM>(active), 0);
    }
}

// Symbol: ?SerializeTabGroup@CMDIClientAreaWnd@@AEAAXAEAVCArchive@@PEAVCMFCTabCtrl@@H@Z
extern "C" void MS_ABI impl__SerializeTabGroup_CMDIClientAreaWnd__AEAAXAEAVCArchive__PEAVCMFCTabCtrl__H_Z(
    void* pThis, void* archive, void* tabGroup, int related) {
    auto* self = Self(pThis);
    if (!self || !archive || !tabGroup) return;
    auto& ar = *static_cast<CArchive*>(archive);
    auto& tabs = *static_cast<CMFCTabCtrl*>(tabGroup);
    MdiSession session;
    if (ar.IsStoring()) WriteSessionGroup(ar, *self, tabs, session);
    else RestoreSessionGroup(*self, tabs, ReadSessionGroup(ar, session), related != FALSE);
}

// CMDIClientAreaWnd::SetActiveTab(HWND) — transcribed from retail entry RVA
// 0x7ead0 (mfc140; 0x7e540 in mfc140u).  Not on this pass's assignment list
// but implemented because MoveWindowToTabGroup and OnMDINext end in it:
//     if (m_bDisableUpdateTabs) return;
//     if (m_bIsMDITabbedGroup) {
//         CMDIChildWndEx* pChild = DYNAMIC_DOWNCAST(CMDIChildWndEx, CWnd::FromHandle(hwnd));  if (!pChild) return;
//         CMFCTabCtrl* pGroup = pChild->m_pRelatedTabGroup;  if (!pGroup) return;              // +0x1e8
//         int iTab = pGroup->GetTabFromHwnd(hwnd);  if (iTab < 0) return;                     // vslot +0x448
//         CRect rc; ::GetClientRect(pGroup->m_hWnd, &rc);
//         if (::IsRectEmpty(&rc)) if (CFrameWnd* pTop = pChild->GetTopLevelFrame()) pTop->RecalcLayout(TRUE);   // 0x28c910, vslot +0x300
//         if (CMFCTabCtrl* pActive = FindActiveTabWnd()) {                                    // 0x80d40
//             pActive->m_bIsActiveInMDITabGroup = FALSE;  pActive->InvalidateTab(pActive->GetActiveTab()); }   // +0x408, 0x18230
//         pGroup->m_bIsActiveInMDITabGroup = TRUE;
//         pGroup->SetActiveTab(iTab);  pGroup->InvalidateTab(pGroup->GetActiveTab());       // vslot +0x438 (CMFCTabCtrl::SetActiveTab)
//     } else if (m_bTabIsVisible) {
//         int iTab = m_wndTab.GetTabFromHwnd(hwnd);  if (iTab >= 0) m_wndTab.SetActiveTab(iTab);
//     }
// Deviation: retail reads pChild->m_pRelatedTabGroup. OpenMFC's
// CMDIChildWndEx ctor zero-fills the padding containing +0x1e8, and
// MoveWindowToTabGroup writes the link; this implementation instead derives
// the group with FindTabWndByChild(hwnd), which also yields the tab index, so
// GetTabFromHwnd is not repeated.
// Symbol: ?SetActiveTab@CMDIClientAreaWnd@@QEAAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__SetActiveTab_CMDIClientAreaWnd__QEAAXPEAUHWND_____Z(void* pThis, HWND hWnd) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || self->m_bDisableUpdateTabs) {
        return;
    }
    if (self->m_bIsMDITabbedGroup) {
        CMDIChildWndEx* pChild = AsChildEx(impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd));
        if (pChild == nullptr) {
            return;
        }
        int iTab = -1;
        void* pGroup = impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(pThis, hWnd, &iTab);
        if (pGroup == nullptr || iTab < 0) {
            return;
        }
        RECT rc = {};
        ::GetClientRect(WndHandle(pGroup), &rc);
        if (::IsRectEmpty(&rc)) {
            FrameRecalcLayout(impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pChild), TRUE);
        }
        void* pActive = impl__FindActiveTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(pThis);
        if (pActive != nullptr) {
            TabActiveInMDIGroup(pActive) = FALSE;
            impl__InvalidateTab_CMFCBaseTabCtrl__QEAAXH_Z(AsBaseTab(pActive), ActiveTab(pActive));
        }
        TabActiveInMDIGroup(pGroup) = TRUE;
        impl__SetActiveTab_CMFCTabCtrl__UEAAHH_Z(static_cast<CMFCTabCtrl*>(pGroup), iTab);
        impl__InvalidateTab_CMFCBaseTabCtrl__QEAAXH_Z(AsBaseTab(pGroup), ActiveTab(pGroup));
    } else if (self->m_bTabIsVisible) {
        int iTab = TabFromHwnd(self->m_wndTab, hWnd);
        if (iTab >= 0) {
            impl__SetActiveTab_CMFCTabCtrl__UEAAHH_Z(EmbeddedTab(self), iTab);
        }
    }
}

// CMDIClientAreaWnd::TabWndFromPoint — transcribed from retail entry RVA
// 0x80e80 (mfc140; 0x808f0 in mfc140u).  CPoint is an 8-byte POD passed by
// value in a register.  Not on this pass's assignment list.
//     for (node = head; node; node = node->pNext) {
//         CMFCTabCtrl* p = DYNAMIC_DOWNCAST(CMFCTabCtrl, node->data);
//         CRect rc; ::GetWindowRect(p->m_hWnd, &rc);      // through NULL when the downcast fails
//         if (::PtInRect(&rc, ptScreen)) return p;
//     }
//     return NULL;
// Symbol: ?TabWndFromPoint@CMDIClientAreaWnd@@AEAAPEAVCMFCTabCtrl@@VCPoint@@@Z
extern "C" void* MS_ABI impl__TabWndFromPoint_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__VCPoint___Z(void* pThis, POINT pt) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (!pGroup || !::IsWindowVisible(pGroup->m_hWnd)) continue;
        RECT rc = {};
        ::GetWindowRect(WndHandle(pGroup), &rc);
        if (::PtInRect(&rc, pt)) {
            return pGroup;
        }
    }
    return nullptr;
}

// Symbol: ?UpdateMDITabbedGroups@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int) {
    auto* self = Self(pThis);
    const HWND client = WndHandle(self);
    if (!client || !self->m_bIsMDITabbedGroup || !self->m_bTabIsVisible || self->m_bDisableUpdateTabs) return;
    auto& runtime = *Record(self)->runtime;
    if (runtime.updating) return;
    TabUpdate update(runtime);
    auto children = CollectTabbedDocuments(*self);
    auto& groups = self->m_lstTabbedGroups;
    for (auto* node = groups.pNodeHead; node;) {
        auto* next = node->pNext;
        auto* tabs = static_cast<CMFCTabCtrl*>(node->data);
        RemoveMissingTabs(*tabs, children, true);
        if (!tabs->GetTabsCount())
            impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(self, tabs, FALSE);
        node = next;
    }
    if (children.empty()) return;
    if (!groups.pNodeHead) {
        auto* tabs = impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(self, nullptr);
        if (!tabs) return;
        ListAddTail(groups, tabs);
        PublishGroupList(*self);
    }

    const HWND active = reinterpret_cast<HWND>(::SendMessageW(client, WM_MDIGETACTIVE, 0, 0));
    int activeIndex = -1;
    auto* selected = static_cast<CMFCTabCtrl*>(impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(
        self, active, &activeIndex));
    auto* destination = selected;
    if (!destination) destination = static_cast<CMFCTabCtrl*>(groups.pNodeHead->data);
    if (auto* previous = static_cast<CMFCTabCtrl*>(impl__FindActiveTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(self));
        previous && !selected)
        destination = previous;
    for (auto* child : children) {
        int index = -1;
        auto* tabs = static_cast<CMFCTabCtrl*>(impl__FindTabWndByChild_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__PEAUHWND____AEAH_Z(
            self, child->m_hWnd, &index));
        if (!tabs) tabs = destination;
        SyncDocumentTab(*self, *tabs, *child);
        SetChildRelatedTabGroup(child, tabs);
        if (child->m_hWnd == active) {
            selected = tabs;
            activeIndex = TabFromHwnd(tabs, active);
        }
    }
    for (auto* node = groups.pNodeHead; node; node = node->pNext)
        TabActiveInMDIGroup(node->data) = node->data == selected;
    if (selected && activeIndex >= 0 && selected->GetActiveTab() != activeIndex)
        selected->SetActiveTab(activeIndex);
    LayoutDocumentGroups(*self);
}
