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
// Instances only exist when this file's constructor thunk ran.  The only
// object that embeds one, CMDIFrameWndEx::m_wndClientArea at frame+0x720
// (the `lea 0x720(%rbx); call 0x7c900` at 0x87c2e inside the retail frame
// ctor 0x87b90, mfc140u), is NOT constructed by OpenMFC's CMDIFrameWndEx ctor
// (core/frame/Thunks.cpp), so the frame-embedded instance is uninitialised
// memory.  Every body therefore resolves `this` through a registry that the
// ctor fills and the dtor drains (Self() below) and behaves as a no-op for an
// unregistered instance -- a deliberate deviation from retail, which would
// read garbage there.  See headerRequests for the CMDIFrameWndEx ctor/dtor
// change that lights the embedded instance up.
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
// memory.  CMDIChildWndEx objects are app-allocated and OpenMFC's ctor does not
// initialise +0x1e8/+0x720, so those are only WRITTEN here (never read; see
// SetActiveTab(HWND) and the taskbar bodies for how the reads are avoided).

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

#include <cmath>
#include <cstddef>
#include <cstring>

namespace openmfc { namespace detail { namespace cmdiclientarea {} } }

// ---------------------------------------------------------------------------
// Thunks for DLL-internal calls (BRIEFING S1: C++ methods exist only as their
// extern "C" impl__ thunks).  Every one below has a definition in the tree;
// the ones whose definition still carries an auto-generated parameter list
// are flagged where they are called.
// ---------------------------------------------------------------------------
// This file (forward declarations of exports defined further down).
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
extern "C" void* MS_ABI impl___0CMDITabInfo__QEAA_XZ(void* pThis);
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
// CMDITabInfo, field order from afxmdiclientareawnd.h; the same 48-byte layout
// detail/CMDITabInfoSupport.h pins for core/frame/CMDITabInfo.cpp.
struct RetailMDITabInfo {
    int m_tabLocation;             // +0
    int m_style;                   // +4
    BOOL m_bTabIcons;              // +8
    BOOL m_bTabCloseButton;        // +12
    BOOL m_bTabCustomTooltips;     // +16
    BOOL m_bAutoColor;             // +20
    BOOL m_bDocumentMenu;          // +24
    BOOL m_bEnableTabSwap;         // +28
    BOOL m_bFlatFrame;             // +32
    BOOL m_bActiveTabCloseButton;  // +36
    int m_nTabBorderSize;          // +40
    BOOL m_bReuseRemovedTabGroups; // +44
};
static_assert(sizeof(RetailMDITabInfo) == 0x30, "CMDITabInfo is 0x30 bytes");

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
    RetailMDITabInfo m_mdiTabParams;                // +0x3ea8
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
struct InstanceRecord {
    void* pThis;
    TabIconEntry* pTabIcons;       // m_mapTabIcons contents (m_mapIcons is never populated here: UpdateTabs is a stub)
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

// Retail CMFCTabCtrl vslot +0x368, GetTabsNum, is the inline `m_iTabsNum`
// (+0x150) and has no export.  OpenMFC's tab controls keep their tabs in a
// side table that only the thunks can see; AddTab there rejects a NULL window,
// so the first index whose GetTabWnd is NULL is the count.
int TabsNum(const void* pTab) {
    const CMFCBaseTabCtrl* p = static_cast<const CMFCBaseTabCtrl*>(static_cast<const CMFCTabCtrl*>(pTab));
    int n = 0;
    while (impl__GetTabWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__H_Z(p, n) != nullptr) {
        ++n;
    }
    return n;
}
// Retail vslot +0x428, GetActiveTab, is the inline `m_iActiveTab` (+0x154):
// the index of GetActiveWnd()'s window, -1 when there is none.
int ActiveTab(const void* pTab) {
    const CMFCBaseTabCtrl* p = static_cast<const CMFCBaseTabCtrl*>(static_cast<const CMFCTabCtrl*>(pTab));
    CWnd* pActive = impl__GetActiveWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__XZ(p);
    if (pActive == nullptr) {
        return -1;
    }
    for (int i = 0;; ++i) {
        CWnd* pWnd = impl__GetTabWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__H_Z(p, i);
        if (pWnd == nullptr) {
            return -1;
        }
        if (pWnd == pActive) {
            return i;
        }
    }
}
CWnd* TabWnd(const void* pTab, int i) {
    return impl__GetTabWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__H_Z(
        static_cast<const CMFCBaseTabCtrl*>(static_cast<const CMFCTabCtrl*>(pTab)), i);
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
    impl___0CMDITabInfo__QEAA_XZ(&self->m_mdiTabParams);
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
// CMDIClientAreaWnd::UpdateTabs — retail entry RVA 0x7efa0 (mfc140) / 0x7ea10
// (mfc140u).  Returns at once if m_bDisableUpdateTabs; if m_bIsMDITabbedGroup
// it is `UpdateMDITabbedGroups(bSetActiveTabVisible)` (0x7f5b0 mfc140) and
// returns.  Otherwise it bails unless m_wndTab has an HWND and
// m_bTabIsVisible, then walks the MDI children (::GetWindow GW_CHILD /
// GW_HWNDNEXT + CWnd::FromHandle), and for every CMDIChildWndEx that passes
// its virtual at +0x3d8 (CanShowOnMDITabs) synchronises the tab strip: icon
// via the child's virtual +0x3d0 / WM_GETICON / GetClassLongPtr(GCLP_HICON),
// looked up in m_mapIcons and added to m_TabIcons (CImageList::Add) with
// m_wndTab's SetImageList (vslot +0x2e8) / ClearImageList (+0x2f8), label via
// the child's +0x3c8 (GetFrameText) compared with GetTabLabel (vslot +0x388)
// and stored with SetTabLabel (+0x390), icon index via GetTabIcon (+0x398) /
// SetTabIcon (+0x3a0), missing tabs added with AddTab (+0x328) and activated
// (+0x438), removed ones with RemoveTab (+0x340), then the parent frame's
// RecalcLayout(TRUE) (vslot +0x300), EnsureVisible (+0x458) of the active
// tab, and a final ::RedrawWindow(m_hWnd, NULL, NULL, 0x507) when the tab
// count crossed zero.
// STUB: not transcribed -- it needs m_mapIcons/m_TabIcons bookkeeping plus
// seven CMFCTabCtrl virtuals (SetImageList, ClearImageList, GetTabLabel,
// SetTabLabel, GetTabIcon, SetTabIcon, EnsureVisible) and three CMDIChildWndEx
// virtuals (+0x3c8 GetFrameText / +0x3d0 GetFrameIcon / +0x3d8
// CanShowOnMDITabs).  Those exist in featurepack/tabs and core/frame only as
// non-virtual thunks, and the tab-side ones are placeholders or partial
// (SetImageList returns 0; GetTabLabel/GetTabIcon/SetTabIcon do not model the
// per-tab record), so a port would add and remove tabs without labels or
// icons and never honour an app override.
// Symbol: ?UpdateTabs@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible) {
    (void)pThis;
    (void)bSetActiveTabVisible;
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
// CMDIClientAreaWnd::AdjustMDIChildren — retail entry RVA 0x7e060 (mfc140u).
// Proceeds when ::IsWindowVisible(pTabWnd->m_hWnd) or the global at 0x3b1afc
// (?m_bDisableSetRedraw@CMDIFrameWndEx@@2HA per the mfc140u export table) is
// zero; copies pTabWnd->m_rectWndArea (CMFCTabCtrl +0x45c, "child window
// area"), ::MapWindowPoints(pTabWnd->m_hWnd, m_hWnd, &rect, 2), then for each
// tab i of pTabWnd (vslots +0x428/+0x368/+0x370): a minimised child
// (GWL_STYLE bit 29) is ShowWindow(SW_RESTORE)'d, then
// SetWindowPos(&wndTop, rect, i == active ? SWP_NOACTIVATE : 0x1c) and, for a
// CMDIChildWndEx with taskbar-tab support, the taskbar thumbnail is refreshed
// (InvalidateIconicBitmaps 0x840b0, then SetTaskbarTabOrder(NULL) 0x84220 --
// byte-identical to ?SetTaskbarTabOrder@CMDIChildWndEx@@ at 0x847b0, mfc140)
// unless AfxGetApp()'s virtual +0x208 object answers TRUE at its +0x70.
// STUB: every rectangle comes from m_rectWndArea, which OpenMFC's CMFCTabCtrl
// never computes (its RecalcLayout is a stub); positioning the children from
// a zero rect would collapse them, which is worse than leaving them alone.
// Symbol: ?AdjustMDIChildren@CMDIClientAreaWnd@@AEAAXPEAVCMFCTabCtrl@@@Z
extern "C" void MS_ABI impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(void* pThis, void* pTabGroup) {
    (void)pThis;
    (void)pTabGroup;
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
    const RetailMDITabInfo& params = self->m_mdiTabParams;
    impl__ModifyTabStyle_CMFCTabCtrl__QEAAHW4Style_1__Z(pTab, params.m_style);
    impl__SetLocation_CMFCBaseTabCtrl__UEAAXW4Location_1__Z(pBase, params.m_tabLocation);
    impl__EnableTabDocumentsMenu_CMFCTabCtrl__QEAAXH_Z(pTab, params.m_bDocumentMenu);
    impl__EnableAutoColor_CMFCBaseTabCtrl__QEAAXH_Z(pBase, params.m_bAutoColor);
    impl__SetTabBorderSize_CMFCBaseTabCtrl__UEAAXHH_Z(pBase, params.m_nTabBorderSize, TRUE);
    impl__EnableCustomToolTips_CMFCBaseTabCtrl__QEAAHH_Z(pBase, params.m_bTabCustomTooltips);
    impl__HideNoTabs_CMFCTabCtrl__QEAAXH_Z(pTab, TRUE);
    impl__SetActiveTabBoldFont_CMFCTabCtrl__QEAAXH_Z(pTab, TRUE);
}

// CMDIClientAreaWnd::CalcWindowRect — retail entry RVA 0x7d900 (mfc140u,
// virtual).  Returns without touching lpRect if m_bDisableUpdateTabs.  If
// m_bIsMDITabbedGroup: CalcWindowRectForMDITabbedGroups(lpRect, nAdjustType)
// then CWnd::CalcWindowRect (0x28edc0) and return.  Otherwise, when m_wndTab
// has an HWND: if m_bTabIsVisible it positions m_wndTab over lpRect
// (SetWindowPos flags 0x14), then deflates lpRect by the difference between
// m_wndTab.m_rectWndArea (CMFCTabCtrl +0x45c, i.e. this+0x544) and
// m_wndTab's client rect, and ShowWindow(SW_SHOWNA); else ShowWindow(SW_HIDE).
// Then SetWindowPos(this, lpRect, 0x14), lets a CMDIFrameWndEx parent adjust
// through its virtual +0x410 (a `ret` in the retail base), and when the tab
// strip is hidden re-parks every minimised MDI child (walking
// pFrame->m_hWndMDIClient, CMDIFrameWnd +0x1d8, with ::GetWindow) below the
// client area.  Finally CWnd::CalcWindowRect(lpRect, nAdjustType) and a loop
// over m_wndTab's tabs restoring minimised children and SetWindowPos'ing each
// into lpRect (flags 0x10 for the active tab, 0x1c otherwise).
// STUB: the tab-visible path is the one that matters and it is driven by
// m_rectWndArea, which OpenMFC's CMFCTabCtrl never computes; a layout made
// from a zero rect would collapse the MDI children.  Not transcribed.
// Symbol: ?CalcWindowRect@CMDIClientAreaWnd@@MEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRect_CMDIClientAreaWnd__MEAAXPEAUtagRECT__I_Z(void* pThis, RECT* lpClientRect, unsigned int nAdjustType) {
    (void)pThis;
    (void)lpClientRect;
    (void)nAdjustType;
}

// CMDIClientAreaWnd::CalcWindowRectForMDITabbedGroups — transcribed from
// retail entry RVA 0x7dd90 (mfc140u):
//     if (m_bInsideDragComplete) return;
//     SetWindowPos(&CWnd::wndBottom, lp->left, lp->top, w, h, SWP_NOACTIVATE);   // 0x2a9a60, 0x3c3460
//     if (m_lstTabbedGroups.GetCount() == 0) return;
//     if (GetCount() == 1) {
//         CMFCTabCtrl* p = DYNAMIC_DOWNCAST(CMFCTabCtrl, head->data);
//         p->ShowWindow(SW_SHOWNA);  p->SetWindowPos(NULL, 0, 0, w, h, SWP_NOZORDER|SWP_NOACTIVATE);
//         AdjustMDIChildren(p);  return;
//     }
//     int nTotal = 0;
//     for (each node) { ::GetWindowRect(group->m_hWnd, &rc); nTotal += vert ? rc.Width() : rc.Height(); }
//     int nFull = vert ? lp->Width() : lp->Height();   // vert == (m_groupAlignment == GROUP_VERT_ALIGN)
//     int nOffset = 0;  const int nMin = m_nResizeMargin;
//     for (each node) {
//         group = DYNAMIC_DOWNCAST(...); if (!group || !group->m_hWnd) continue;
//         ::GetWindowRect(group->m_hWnd, &rc);  int cur = vert ? rc.Width() : rc.Height();
//         int nSize = (int)(ceil((double)cur / nTotal * 100.0) * nFull / 100.0);   // ceil = import 0x2c7508
//         if (this is the last node && nSize - nOffset + nFull != 0) nSize = nFull - nOffset;
//         if (nSize < nMin) nSize = nMin;
//         vert ? group->SetWindowPos(NULL, nOffset, 0, nSize, lp->Height(), 0x14)
//              : group->SetWindowPos(NULL, 0, nOffset, lp->Width(), nSize, 0x14);
//         AdjustMDIChildren(group);  nOffset += nSize;
//         ::RedrawWindow(group->m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
//     }
// (The `nSize - nOffset + nFull != 0` test is retail's, transcribed as is.)
// Retail reads m_hWnd through a NULL group in the first loop when the
// downcast fails; that node is skipped here.
// Symbol: ?CalcWindowRectForMDITabbedGroups@CMDIClientAreaWnd@@QEAAXPEAUtagRECT@@I@Z
extern "C" void MS_ABI impl__CalcWindowRectForMDITabbedGroups_CMDIClientAreaWnd__QEAAXPEAUtagRECT__I_Z(void* pThis, RECT* lpRect, unsigned int nAdjustType) {
    (void)nAdjustType;   // not read by retail
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || lpRect == nullptr || self->m_bInsideDragComplete) {
        return;
    }
    const int nWidth = lpRect->right - lpRect->left;
    const int nHeight = lpRect->bottom - lpRect->top;
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(SelfWnd(self), WndBottom(), lpRect->left, lpRect->top,
                                                nWidth, nHeight, SWP_NOACTIVATE);
    if (self->m_lstTabbedGroups.nCount == 0) {
        return;
    }
    if (self->m_lstTabbedGroups.nCount == 1) {
        CMFCTabCtrl* pGroup = AsTabCtrl(self->m_lstTabbedGroups.pNodeHead->data);
        if (pGroup == nullptr) {
            return;   // retail would call ShowWindow through NULL here
        }
        impl__ShowWindow_CWnd__QEAAHH_Z(pGroup, SW_SHOWNA);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pGroup, nullptr, 0, 0, nWidth, nHeight, SWP_NOZORDER | SWP_NOACTIVATE);
        impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pGroup);
        return;
    }
    const bool bVert = self->m_groupAlignment == kGroupVertAlign;
    int nTotal = 0;
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (pGroup == nullptr) {
            continue;
        }
        RECT rc = {};
        ::GetWindowRect(WndHandle(pGroup), &rc);
        nTotal += bVert ? (rc.right - rc.left) : (rc.bottom - rc.top);
    }
    const int nFull = bVert ? nWidth : nHeight;
    const int nMin = self->m_nResizeMargin;
    int nOffset = 0;
    for (RetailNode* node = self->m_lstTabbedGroups.pNodeHead; node != nullptr; node = node->pNext) {
        CMFCTabCtrl* pGroup = AsTabCtrl(node->data);
        if (pGroup == nullptr || WndHandle(pGroup) == nullptr) {
            continue;
        }
        RECT rc = {};
        ::GetWindowRect(WndHandle(pGroup), &rc);
        const int nCur = bVert ? (rc.right - rc.left) : (rc.bottom - rc.top);
        const double dPercent = ceil(static_cast<double>(nCur) / static_cast<double>(nTotal) * 100.0);
        int nSize = static_cast<int>(dPercent * static_cast<double>(nFull) / 100.0);
        if (node->pNext == nullptr && (nSize - nOffset + nFull) != 0) {
            nSize = nFull - nOffset;
        }
        if (nSize < nMin) {
            nSize = nMin;
        }
        if (bVert) {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pGroup, nullptr, nOffset, 0, nSize, nHeight, SWP_NOZORDER | SWP_NOACTIVATE);
        } else {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pGroup, nullptr, 0, nOffset, nWidth, nSize, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pGroup);
        nOffset += nSize;
        ::RedrawWindow(WndHandle(pGroup), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
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
// In OpenMFC the CMFCTabCtrl::Create thunk (featurepack/tabs/CMFCTabCtrl.cpp)
// is still a generated placeholder that returns 0, so a fresh group takes the
// retail "Create failed" path (deleted, NULL returned) until that thunk is
// implemented.  Retail dereferences a NULL pNew/pImages when operator new
// fails; those are guarded here.
// Symbol: ?CreateTabGroup@CMDIClientAreaWnd@@UEAAPEAVCMFCTabCtrl@@PEAV2@@Z
extern "C" void* MS_ABI impl__CreateTabGroup_CMDIClientAreaWnd__UEAAPEAVCMFCTabCtrl__PEAV2__Z(void* pThis, void* pTabGroup) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return nullptr;
    }
    const RetailMDITabInfo& params = self->m_mdiTabParams;
    bool bRecycled = false;
    void* pNew = pTabGroup;
    if (pNew == nullptr) {
        if (params.m_bReuseRemovedTabGroups && self->m_lstRemovedTabbedGroups.nCount > 0 && self->m_bIsMDITabbedGroup) {
            void* p = ListRemoveTail(self->m_lstRemovedTabbedGroups);
            if (p != nullptr && AsTabCtrl(p) != nullptr && ::IsWindow(WndHandle(p))) {
                pNew = p;
                bRecycled = true;
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

// CMDIClientAreaWnd::EnableMDITabbedGroups — retail entry RVA 0x7ce60 (mfc140u).
//     if (m_bTabIsEnabled) EnableMDITabs(FALSE, mdiTabParams);              // 0x7cd20
//     m_wndTab.ShowWindow(SW_HIDE);
//     HWND hActive = (HWND)::SendMessage(m_hWnd, WM_MDIGETACTIVE, 0, 0);
//     if (m_bIsMDITabbedGroup != bEnable) {
//         m_bIsMDITabbedGroup = bEnable;
//         if (!bEnable) for (each group) { group->ShowWindow(SW_HIDE);
//             for (each tab i) group->GetTabWnd(i)->ModifyStyle(0,
//                                  CMDIChildWndEx::m_dwExcludeStyle | WS_SYSMENU, 0x24); }   // 0x2a96f0
//     }
//     m_bTabIsVisible = bEnable;
//     if (!m_bIsMDITabbedGroup) {
//         if (!IsKeepClientEdge()) ModifyStyleEx(0, WS_EX_CLIENTEDGE);
//         collect every MDI child (::GetWindow GW_CHILD/HWNDNEXT, FromHandle) that is a
//         CMDIChildWndEx passing its virtual +0x3d8 into a local CList (vftable 0x2ed378, 0x7908);
//         m_bDisableUpdateTabs = TRUE;
//         for (each collected child) child->SetWindowPos(NULL, -1, -1, -1, -1, 0x27);   // 0x2a9a60
//         m_bDisableUpdateTabs = FALSE;
//         UpdateTabs(FALSE);                                                  // 0x7ea10
//         ~CList (0x83600)
//     } else {
//         m_mdiTabParams = mdiTabParams (three 16-byte moves);
//         if (!IsKeepClientEdge()) ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
//         for (each group) { group->ShowWindow(SW_SHOWNA); ApplyParams(group); }
//         UpdateMDITabbedGroups(TRUE);                                        // 0x7f020
//         for (each group) group->RecalcLayout();                             // vslot +0x318
//         if (m_bIsMDITabbedGroup) SetActiveTab(hActive);                     // 0x7e540
//     }
// (the ModifyStyle at 0x7cf46 ORs bit 19 == WS_SYSMENU (`bts $0x13`) into
// the dword global at 0x3b1af4, which the mfc140u export table names
// ?m_dwExcludeStyle@CMDIChildWndEx@@2KA; OpenMFC exports it too, as
// impl__m_dwExcludeStyle_CMDIChildWndEx__2KA in core/frame/StaticData.cpp.)
// STUB: both branches end in UpdateTabs / UpdateMDITabbedGroups, which are
// stubs here, and the enable path's child collection goes through the
// CMDIChildWndEx virtual +0x3d8 (CanShowOnMDITabs), which OpenMFC can only
// reach as a non-virtual thunk; toggling the flag without the group/tab
// bookkeeping those stubs own would leave the frame in a half-converted
// state.  Not transcribed.
// Symbol: ?EnableMDITabbedGroups@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabbedGroups_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(void* pThis, int bEnable, const void* pParams) {
    (void)pThis;
    (void)bEnable;
    (void)pParams;
}

// CMDIClientAreaWnd::EnableMDITabs — transcribed from retail entry RVA
// 0x7cd20 (mfc140u):
//     if (m_bIsMDITabbedGroup) EnableMDITabbedGroups(FALSE, params);      // 0x7ce60, same params reference
//     m_bTabIsEnabled = bEnable;  m_bTabIsVisible = bEnable;
//     m_mdiTabParams = params;                                             // three 16-byte moves
//     ApplyParams(&m_wndTab);                                              // 0x7d1b0
//     if (bEnable) { UpdateTabs(FALSE);                                    // 0x7ea10
//                    if (!IsKeepClientEdge()) ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0); }   // 0x7f6a0 / 0x2a9740
//     else         { if (!IsKeepClientEdge()) ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0); }
//     if (m_wndTab.m_hWnd) m_wndTab.ShowWindow(SW_SHOW);                   // 0x2a9ad0 (the `test %rsi` on &m_wndTab is always true)
//     ::BringWindowToTop(m_hWnd);                                          // import (BringWindowToTop in the mfc140 twin, 0x2c5290)
//     if (m_hWnd && GetParentFrame()) {                                    // 0x28e200, called twice
//         GetParentFrame()->RecalcLayout(TRUE);                            // vslot +0x300
//         if (m_wndTab.m_hWnd) ::RedrawWindow(m_wndTab.m_hWnd, NULL, NULL, 0x585);   // RDW_INVALIDATE|ERASE|FRAME|UPDATENOW|ALLCHILDREN
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x585);
//     }
// Symbol: ?EnableMDITabs@CMDIClientAreaWnd@@QEAAXHAEBVCMDITabInfo@@@Z
extern "C" void MS_ABI impl__EnableMDITabs_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(void* pThis, int bEnable, const void* pParams) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || pParams == nullptr) {
        return;
    }
    if (self->m_bIsMDITabbedGroup) {
        impl__EnableMDITabbedGroups_CMDIClientAreaWnd__QEAAXHAEBVCMDITabInfo___Z(pThis, FALSE, pParams);
    }
    self->m_bTabIsEnabled = bEnable;
    self->m_bTabIsVisible = bEnable;
    memcpy(&self->m_mdiTabParams, pParams, sizeof(RetailMDITabInfo));
    impl__ApplyParams_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, self->m_wndTab);
    if (bEnable) {
        impl__UpdateTabs_CMDIClientAreaWnd__QEAAXH_Z(pThis, FALSE);
        if (!impl__IsKeepClientEdge_CMDIClientAreaWnd__AEAAHXZ(pThis)) {
            impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(SelfWnd(self), WS_EX_CLIENTEDGE, 0, 0);
        }
    } else {
        if (!impl__IsKeepClientEdge_CMDIClientAreaWnd__AEAAHXZ(pThis)) {
            impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(SelfWnd(self), 0, WS_EX_CLIENTEDGE, 0);
        }
    }
    if (WndHandle(self->m_wndTab) != nullptr) {
        impl__ShowWindow_CWnd__QEAAHH_Z(EmbeddedTab(self), SW_SHOW);
    }
    ::BringWindowToTop(WndHandle(self));
    if (WndHandle(self) != nullptr) {
        CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self));
        if (pFrame != nullptr) {
            FrameRecalcLayout(pFrame, TRUE);
            const UINT flags = RDW_INVALIDATE | RDW_ERASE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN;   // 0x585
            if (WndHandle(self->m_wndTab) != nullptr) {
                ::RedrawWindow(WndHandle(self->m_wndTab), nullptr, nullptr, flags);
            }
            ::RedrawWindow(WndHandle(self), nullptr, nullptr, flags);
        }
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
    // The retail downcasts are performed (they have no side effect beyond
    // IsKindOf) but the 0x10 decision they feed cannot be made here.
    (void)AsChildEx(impl__GetActiveWnd_CMFCBaseTabCtrl__UEBAPEAVCWnd__XZ(AsBaseTab(pActive)));
    (void)AsFrameEx(impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self)));
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

// CMDIClientAreaWnd::LoadState — retail entry RVA 0x82810 (mfc140u; the ANSI
// twin ?LoadState@CMDIClientAreaWnd@@QEAAHPEBDI@Z is 0x82da0 in mfc140).
// Builds the registry path with AFXGetRegPath (0xd2540 mfc140) and
// CString::Format from lpszProfileName and nFrameID, opens it through
// CSettingsStoreSP::Create (0x12b320 mfc140), reads the blob, wraps it in a
// CMemFile/CArchive and drives Serialize (this->vtable[+0x10]).
// STUB: Serialize below is itself a stub (its payload is the tab-group
// document list, which needs the document-template machinery), so there is
// nothing a load could deliver into; not transcribed.
// Symbol: ?LoadState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CMDIClientAreaWnd__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)uiID;
    return 0;
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
    if (pGroup == nullptr) {
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

// CMDIClientAreaWnd::MoveWindowToTabGroup — transcribed from retail entry
// RVA 0x7ff80 (mfc140u):
//     HWND hFrom = pFrom ? pFrom->m_hWnd : NULL;  HWND hTo = pTo ? pTo->m_hWnd : NULL;
//     if (nIdxFrom == -1) { nIdxFrom = pFrom->GetActiveTab(); if (nIdxFrom == -1) return FALSE; }   // vslot +0x428
//     CMDIChildWndEx* pWnd = DYNAMIC_DOWNCAST(CMDIChildWndEx, pFrom->GetTabWnd(nIdxFrom));       // +0x370
//     if (!pWnd) return FALSE;
//     CString strLabel = pWnd->GetFrameText();                                 // vslot +0x3c8, hidden return slot
//     pFrom->RemoveTab(nIdxFrom, TRUE);                                        // +0x340
//     pWnd->m_pRelatedTabGroup = pTo;                                          // +0x1e8
//     pTo->AddTab(pWnd, strLabel, (UINT)-1, TRUE);                             // +0x328, the -1 is hard-coded
//     if (pFrom->GetTabsNum() == 0) { RemoveTabGroup(pFrom, TRUE); UpdateMDITabbedGroups(TRUE); pTo->RecalcLayout(); }   // 0x80320 / 0x7f020 / +0x318
//     else AdjustMDIChildren(pFrom);
//     AdjustMDIChildren(pTo);                                                  // 0x7e060
//     SetActiveTab(pWnd->m_hWnd);                                              // 0x7e540
//     if (CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame()))
//         ::SendMessage(pFrame->m_hWnd, AFX_WM_ON_MOVETOTABGROUP, (WPARAM)hFrom, (LPARAM)hTo);   // 0x3c18f0
//     ~strLabel;  return TRUE;
// Retail dereferences pTo without a check (pTo comes from a downcast that can
// fail in MDITabMoveToNextGroup); a NULL pTo returns FALSE here after the
// same early checks.  The CString comes back through the GetFrameText thunk's
// hidden return slot and is released through the CStringT destructor thunk.
// Symbol: ?MoveWindowToTabGroup@CMDIClientAreaWnd@@QEAAHPEAVCMFCTabCtrl@@0H@Z
extern "C" int MS_ABI impl__MoveWindowToTabGroup_CMDIClientAreaWnd__QEAAHPEAVCMFCTabCtrl__0H_Z(void* pThis, void* pFrom, void* pTo, int nIdxFrom) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || pFrom == nullptr) {
        return FALSE;
    }
    HWND hFrom = WndHandle(pFrom);
    HWND hTo = WndHandle(pTo);
    if (nIdxFrom == -1) {
        nIdxFrom = ActiveTab(pFrom);
        if (nIdxFrom == -1) {
            return FALSE;
        }
    }
    CMDIChildWndEx* pWnd = AsChildEx(TabWnd(pFrom, nIdxFrom));
    if (pWnd == nullptr || pTo == nullptr) {
        return FALSE;
    }
    alignas(void*) unsigned char strStorage[sizeof(CString)] = {};
    CString* pLabel = impl__GetFrameText_CMDIChildWndEx__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
        pWnd, reinterpret_cast<CString*>(strStorage));
    impl__RemoveTab_CMFCBaseTabCtrl__UEAAHHH_Z(AsBaseTab(pFrom), nIdxFrom, TRUE);
    SetChildRelatedTabGroup(pWnd, pTo);
    impl__AddTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__PEB_WIH_Z(AsBaseTab(pTo), pWnd,
                                                          pLabel != nullptr ? pLabel->GetString() : nullptr,
                                                          static_cast<unsigned int>(-1), TRUE);
    if (TabsNum(pFrom) == 0) {
        impl__RemoveTabGroup_CMDIClientAreaWnd__QEAAXPEAVCMFCTabCtrl__H_Z(pThis, pFrom, TRUE);
        impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(pThis, TRUE);
        impl__RecalcLayout_CMFCTabCtrl__UEAAXXZ(static_cast<CMFCTabCtrl*>(pTo));
    } else {
        impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pFrom);
    }
    impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pTo);
    impl__SetActiveTab_CMDIClientAreaWnd__QEAAXPEAUHWND_____Z(pThis, WndHandle(pWnd));
    CMDIFrameWndEx* pFrame = AsFrameEx(impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(SelfWnd(self)));
    if (pFrame != nullptr) {
        ::SendMessage(WndHandle(pFrame), impl__AFX_WM_ON_MOVETOTABGROUP__3IA,
                      reinterpret_cast<WPARAM>(hFrom), reinterpret_cast<LPARAM>(hTo));
    }
    if (pLabel != nullptr) {
        impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(pLabel);
    }
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

// CMDIClientAreaWnd::OnCancelTabMove — transcribed from retail entry RVA
// 0x805b0 (mfc140u):
//     DrawNewGroupRect(NULL, &m_rectNewTabGroup);    // 0x809a0
//     ::SetRectEmpty(&m_rectNewTabGroup);
//     return 0;
// Symbol: ?OnCancelTabMove@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnCancelTabMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return 0;
    }
    impl__DrawNewGroupRect_CMDIClientAreaWnd__AEAAXPEBUtagRECT__0_Z(pThis, nullptr, &self->m_rectNewTabGroup);
    ::SetRectEmpty(&self->m_rectNewTabGroup);
    return 0;
}

// CMDIClientAreaWnd::OnDragComplete — transcribed from retail entry RVA
// 0x7f800 (mfc140u):
//     if (!m_bIsMDITabbedGroup || m_lstTabbedGroups.GetCount() == 0) return 0;
//     CMFCTabCtrl* pNext = GetNextTabWnd((CMFCTabCtrl*)wp, FALSE);  if (!pNext) return 0;   // 0x80890
//     m_bInsideDragComplete = TRUE;
//     CRect* pRect = (CRect*)lp;  ScreenToClient(pRect);                     // 0x2a32b0
//     wp->SetWindowPos(NULL, -1, -1, pRect->Width(), pRect->Height(), SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);   // 0x2a9a60
//     CRect rcNext; ::GetWindowRect(pNext->m_hWnd, &rcNext); ScreenToClient(&rcNext);
//     if (m_groupAlignment == GROUP_VERT_ALIGN) rcNext.left = pRect->right; else rcNext.top = pRect->bottom;
//     pNext->SetWindowPos(NULL, rcNext.left, rcNext.top, rcNext.Width(), rcNext.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
//     AdjustMDIChildren(wp);  AdjustMDIChildren(pNext);
//     ::RedrawWindow(wp->m_hWnd, NULL, NULL, 0x105);  ::RedrawWindow(pNext->m_hWnd, NULL, NULL, 0x105);
//     m_bInsideDragComplete = FALSE;  return 1;
// (retail passes a NULL wp straight into GetNextTabWnd, which then returns
// NULL; a NULL lp would be dereferenced -- guarded here.)
// Symbol: ?OnDragComplete@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnDragComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr || !self->m_bIsMDITabbedGroup || self->m_lstTabbedGroups.nCount == 0) {
        return 0;
    }
    void* pGroup = reinterpret_cast<void*>(wParam);
    RECT* pRect = reinterpret_cast<RECT*>(lParam);
    void* pNext = impl__GetNextTabWnd_CMDIClientAreaWnd__AEAAPEAVCMFCTabCtrl__PEAV2_H_Z(pThis, pGroup, FALSE);
    if (pNext == nullptr || pRect == nullptr) {
        return 0;
    }
    self->m_bInsideDragComplete = TRUE;
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(SelfWnd(self), pRect);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pGroup), nullptr, -1, -1, pRect->right - pRect->left,
                                                pRect->bottom - pRect->top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    RECT rcNext = {};
    ::GetWindowRect(WndHandle(pNext), &rcNext);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(SelfWnd(self), &rcNext);
    if (self->m_groupAlignment == kGroupVertAlign) {
        rcNext.left = pRect->right;
    } else {
        rcNext.top = pRect->bottom;
    }
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pNext), nullptr, rcNext.left, rcNext.top,
                                                rcNext.right - rcNext.left, rcNext.bottom - rcNext.top,
                                                SWP_NOZORDER | SWP_NOACTIVATE);
    impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pGroup);
    impl__AdjustMDIChildren_CMDIClientAreaWnd__AEAAXPEAVCMFCTabCtrl___Z(pThis, pNext);
    ::RedrawWindow(WndHandle(pGroup), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    ::RedrawWindow(WndHandle(pNext), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    self->m_bInsideDragComplete = FALSE;
    return 1;
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

// CMDIClientAreaWnd::OnMDIDestroy — retail entry RVA 0x7dae0 (mfc140).
//     CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetParentFrame());
//     CMDIChildWndEx* pChild = DYNAMIC_DOWNCAST(CMDIChildWndEx, pWnd);
//     if (pFrame->[+0x214] == 0 && !CMDIFrameWndEx::m_bDisableSetRedraw)      // 0x3aaafc (mfc140) is ?m_bDisableSetRedraw@CMDIFrameWndEx@@2HA; pFrame unchecked
//         ::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
//     HWND hActivate = NULL;  BOOL bChanged = FALSE;
//     if (pChild) {
//         CMFCTabCtrl* pTab = pChild->m_pRelatedTabGroup;  pChild->m_pRelatedTabGroup = NULL;   // +0x1e8
//         if (pTab) {
//             int nBefore = pTab->vslot(+0x308)();  int iTab = pTab->GetTabFromHwnd(pWnd->m_hWnd);
//             if (iTab >= 0) pChild->[+0x1f0] = TRUE;
//             BOOL bRecalc = !(AfxGetApp()->vslot(+0x208)() && that->vslot(+0x70)());
//             pTab->RemoveTab(iTab, bRecalc);
//             if (pTab->GetTabsNum() == 0) { if (Find(pTab) && count > 1 && pTab->m_bIsActiveInMDITabGroup)
//                     hActivate = (next-or-head group)->GetTabWnd(max(GetActiveTab(), 0))->m_hWnd;
//                 RemoveTabGroup(pTab, TRUE); }
//             else bChanged = nBefore != pTab->vslot(+0x308)();
//         }
//     }
//     if (m_wndTab.m_hWnd) { nBefore = m_wndTab.vslot(+0x308)(); iTab = m_wndTab.GetTabFromHwnd(pWnd->m_hWnd);
//         if (iTab >= 0) { if (CMDIChildWndEx) child->[+0x1f0] = TRUE; m_wndTab.RemoveTab(iTab, TRUE); }
//         bChanged = nBefore != m_wndTab.vslot(+0x308)(); }
//     Default();
//     if (bChanged) pFrame->RecalcLayout(TRUE);
//     if (!pFrame->[+0x214]) { if (::IsWindow(hActivate)) SetActiveTab(hActivate);
//         if (!pFrame->[+0x214] && !CMDIFrameWndEx::m_bDisableSetRedraw) { ::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
//             ::RedrawWindow(CWnd::FromHandle(::GetParent(m_hWnd))->m_hWnd, NULL, NULL, 0x185); } }
// STUB: the body is gated on CMDIFrameWndEx +0x214 (a frame BOOL OpenMFC's
// padded CMDIFrameWndEx does not carry), CMFCTabCtrl vslot +0x308 (an
// unexported virtual, no thunk) and a CWinApp virtual chain (+0x208
// GetDataRecoveryHandler -> its +0x70); it also READS
// CMDIChildWndEx::m_pRelatedTabGroup (+0x1e8), which OpenMFC's child ctor
// never initialises.  (m_bDisableSetRedraw itself IS available:
// impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA in core/frame/StaticData.cpp.)
// Assuming the remaining gates would either skip the redraw or dereference
// uninitialised memory; not transcribed.
// Symbol: ?OnMDIDestroy@CMDIClientAreaWnd@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnMDIDestroy_CMDIClientAreaWnd__IEAAXPEAVCWnd___Z(void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
}

// CMDIClientAreaWnd::OnMDINext — transcribed from retail entry RVA 0x83760
// (mfc140):
//     CMFCTabCtrl* pTab;  POSITION pos = NULL;  INT_PTR nGroups = 0;
//     if (m_bIsMDITabbedGroup) pTab = FindActiveTabWnd();                    // 0x80d40
//     else if (m_bTabIsEnabled) pTab = &m_wndTab;
//     else { Default(); return; }                                            // 0x289090
//     if (m_bIsMDITabbedGroup) { pos = m_lstTabbedGroups.Find(pTab); nGroups = GetCount();
//                                if (!pos) AfxThrowInvalidArgException(); }   // 0x225b80
//     int iNext = pTab->GetActiveTab() + (bIsPrev ? -1 : 1);                  // vslot +0x428; `neg/sbb/and -2/+1`
//     if (iNext < 0) {
//         if (nGroups > 0) { if (!pos) throw; pos = pos->pPrev;
//                            pTab = DYNAMIC_DOWNCAST(CMFCTabCtrl, pos ? pos->data : m_lstTabbedGroups.GetTail()); }
//         if (!pTab) throw;
//         iNext = pTab->GetTabsNum() - 1;                                    // vslot +0x368; wraps the flat strip too
//     }
//     if (iNext >= pTab->GetTabsNum()) {
//         if (nGroups > 0) { if (!pos) throw; (downcast of pos->data, result unused); pos = pos->pNext;
//                            pTab = DYNAMIC_DOWNCAST(CMFCTabCtrl, pos ? pos->data : m_lstTabbedGroups.GetHead()); }
//         if (!pTab) throw;
//         iNext = 0;
//     }
//     CWnd* pNext = pTab->GetTabWnd(iNext);                                   // vslot +0x370
//     if ((pNext ? pNext->m_hWnd : NULL) != (pWndMDIChild ? pWndMDIChild->m_hWnd : NULL))
//         SetActiveTab(pNext ? pNext->m_hWnd : NULL);                         // 0x7ead0
// (retail passes a NULL pTab from FindActiveTabWnd straight into the vslot
// calls; that case throws here through the same AfxThrowInvalidArgException
// the surrounding checks use.)
// Symbol: ?OnMDINext@CMDIClientAreaWnd@@IEAAXPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnMDINext_CMDIClientAreaWnd__IEAAXPEAVCWnd__H_Z(void* pThis, void* pWnd, int bIsPrev) {
    S_CMDIClientAreaWnd* self = Self(pThis);
    if (self == nullptr) {
        return;
    }
    void* pTab = nullptr;
    RetailNode* pos = nullptr;
    INT_PTR nGroups = 0;
    if (self->m_bIsMDITabbedGroup) {
        pTab = impl__FindActiveTabWnd_CMDIClientAreaWnd__QEAAPEAVCMFCTabCtrl__XZ(pThis);
        pos = ListFind(self->m_lstTabbedGroups, pTab);
        nGroups = self->m_lstTabbedGroups.nCount;
        if (pos == nullptr || pTab == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
    } else if (self->m_bTabIsEnabled) {
        pTab = self->m_wndTab;
    } else {
        impl__Default_CWnd__IEAA_JXZ(SelfWnd(self));
        return;
    }
    int iNext = ActiveTab(pTab) + (bIsPrev ? -1 : 1);
    if (iNext < 0) {
        if (nGroups > 0) {
            pos = pos->pPrev;
            pTab = AsTabCtrl(pos != nullptr ? pos->data : self->m_lstTabbedGroups.pNodeTail->data);
            if (pTab == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return;
            }
        }
        iNext = TabsNum(pTab) - 1;
    }
    if (iNext >= TabsNum(pTab)) {
        if (nGroups > 0) {
            if (pos == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return;
            }
            pos = pos->pNext;
            pTab = AsTabCtrl(pos != nullptr ? pos->data : self->m_lstTabbedGroups.pNodeHead->data);
            if (pTab == nullptr) {
                impl__AfxThrowInvalidArgException__YAXXZ();
                return;
            }
        }
        iNext = 0;
    }
    CWnd* pNext = TabWnd(pTab, iNext);
    if (WndHandle(pNext) != WndHandle(pWnd)) {
        impl__SetActiveTab_CMDIClientAreaWnd__QEAAXPEAUHWND_____Z(pThis, WndHandle(pNext));
    }
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

// CMDIClientAreaWnd::OnMoveTabComplete — retail entry RVA 0x80300 (mfc140).
//     CRect rcNew = m_rectNewTabGroup;  DrawNewGroupRect(NULL, &m_rectNewTabGroup); ::SetRectEmpty(&m_rectNewTabGroup);
//     CMFCTabCtrl* pFrom = (CMFCTabCtrl*)wp;  if (!pFrom) return 0;
//     CPoint pt(lp); ::ClientToScreen(pFrom->m_hWnd, &pt);
//     CMFCTabCtrl* pTo = TabWndFromPoint(pt);  if (!pTo) return 0;                  // 0x80e80
//     BOOL bMenu = TRUE;
//     if (::IsRectEmpty(&rcNew)) {
//         CMDIFrameWndEx* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, CWnd::FromHandle(::GetParent(m_hWnd)));
//         CPoint ptMenu(0,0)?; DWORD dw = GetMDITabsContextMenuAllowedItems();          // 0x80b70
//         if (dw) bMenu = pFrame->vslot(+0x3b0)(ptMenu, dw, TRUE);                    // OnShowMDITabContextMenu, pFrame unchecked
//     }
//     CRect rcWnd = pTo->m_rectWndArea; pTo->ClientToScreen(&rcWnd);                   // CMFCTabCtrl +0x45c
//     if (!bMenu) { ... halve rcWnd along m_groupAlignment and CreateNewTabGroup(pTo, rcWnd, ...) }
//     else if (!::IsRectEmpty(&rcNew) && ::EqualRect(&rcNew, &rcWnd)?) ...
//     MoveWindowToTabGroup(pFrom, <target>, -1);
//     return 0;
// STUB: needs pTo->m_rectWndArea (never computed by OpenMFC's CMFCTabCtrl)
// and the CMDIFrameWndEx virtual +0x3b0 (OnShowMDITabContextMenu, no thunk);
// the tail was not fully decoded because of that.  Not transcribed.
// Symbol: ?OnMoveTabComplete@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnMoveTabComplete_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
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

// CMDIClientAreaWnd::OnTabGroupMouseMove — retail entry RVA 0x7ff10 (mfc140).
// Resolves ::GetCapture() through CWnd::FromHandle and requires a CMFCTabCtrl;
// with a single group holding a single tab it returns 0.  Otherwise, for the
// POINT packed in lp: if the tab control's IsPtInTabArea (vslot +0x2d8) is
// TRUE, ::SetCursor(::LoadCursor(NULL, IDC_ARROW)); else it converts the
// point to this window's client space, lazily loads afxGlobalData's two
// tab-move cursors (resources 0x4297/0x4298 through AfxFindResourceHandle /
// LoadCursor, stored at afxGlobalData+0x178/+0x180), picks one by
// ::PtInRect against the client rect, and rubber-bands a prospective group
// rectangle (TabWndFromPoint, m_rectWndArea of the hit group, m_groupAlignment)
// through DrawNewGroupRect into m_rectNewTabGroup.
// STUB: needs the hit group's m_rectWndArea (unmodeled) and the two cursor
// slots of AFX_GLOBAL_DATA, which OpenMFC's zero-filled blob never loads.
// Symbol: ?OnTabGroupMouseMove@CMDIClientAreaWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnTabGroupMouseMove_CMDIClientAreaWnd__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
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
    RetailNode* neighbourNode = (pos == self->m_lstTabbedGroups.pNodeHead) ? pos->pNext : pos->pPrev;
    CMFCTabCtrl* pNeighbour = neighbourNode != nullptr ? AsTabCtrl(neighbourNode->data) : nullptr;
    ListRemoveAt(self->m_lstTabbedGroups, pos);
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

// CMDIClientAreaWnd::SaveState — the Unicode export is missing from both RVA
// maps, but the mfc140u export table places ?SaveState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
// at entry RVA 0x825e0 (mfc140u), byte-identical to the ANSI twin
// ?SaveState@CMDIClientAreaWnd@@QEAAHPEBDI@Z (entry RVA 0x82b70, mfc140),
// which is the copy that was decoded.  It formats the registry path with
// AFXGetRegPath (0xd2540) / CString::Format (0xda80), serialises into a
// CMemFile through a storing CArchive (0x229510 / 0x1cf500) by calling
// this->vtable[+0x10] (Serialize), then writes the resulting blob to the
// settings store.
// STUB: Serialize below is a stub, so the blob would be empty; not
// transcribed.
// Symbol: ?SaveState@CMDIClientAreaWnd@@QEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CMDIClientAreaWnd__QEAAHPEB_WI_Z(void* pThis, const wchar_t* lpszProfileName, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)uiID;
    return 0;
}

// CMDIClientAreaWnd::Serialize — retail entry RVA 0x83050 (mfc140, virtual).
//     m_mdiTabParams.Serialize(ar);                  // 0x7ca40
//     if (ar.IsStoring()) {                          // (*(BYTE*)(ar+0x20) & 1) == 0
//         ar << m_bTabIsEnabled << m_bIsMDITabbedGroup << m_bTabIsVisible
//            << m_groupAlignment << m_nResizeMargin << m_nNewGroupMargin;
//         if (m_bTabIsEnabled) SerializeTabGroup(ar, &m_wndTab, FALSE);          // 0x81320
//         else if (m_bIsMDITabbedGroup) { ar << (int)m_lstTabbedGroups.GetCount();
//                                         for (each group) SerializeTabGroup(ar, pGroup, FALSE); }
//         else SerializeOpenChildren(ar);            // 0x821e0, the branch at 0x83246
//     } else {                                       // loading, from 0x83256
//         CloseAllWindows(NULL);                     // 0x811b0
//         m_lstLoadedTabDocuments.RemoveAll();       // 0x230920
//         m_bDisableUpdateTabs = TRUE;
//         ... the mirrored reads, then (re)creation of the tab groups and of
//         the documents they held through the app's document templates ...
//     }
//   (the `ar << int` sequences are the inlined CArchive fast path: bounds-check
//    m_lpBufCur (+0x38) against m_lpBufMax (+0x40), CArchive::Flush (0x1cfb90)
//    when it would overflow, store, advance by 4.)
// STUB: the load path re-opens documents through CDocTemplate / CDocManager
// virtuals and rebuilds groups through UpdateMDITabbedGroups, none of which
// this tree implements; a store-only half would write a state that could
// never be read back.  Not transcribed.
// Symbol: ?Serialize@CMDIClientAreaWnd@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDIClientAreaWnd__UEAAXAEAVCArchive___Z(void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// CMDIClientAreaWnd::SerializeOpenChildren — retail entry RVA 0x821e0
// (mfc140).  Walks the MDI children (::GetWindow GW_CHILD / GW_HWNDNEXT,
// CWnd::FromHandle), keeps the CMDIChildWndEx ones whose virtual +0x3d8
// (CanShowOnMDITabs) answers TRUE, and writes each child's document path
// (CDocument::GetPathName via the frame's active document) as a CString
// element, preceded by the count.
// STUB: stubbed alongside Serialize (its only caller).
// Symbol: ?SerializeOpenChildren@CMDIClientAreaWnd@@AEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__SerializeOpenChildren_CMDIClientAreaWnd__AEAAXAEAVCArchive___Z(void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// CMDIClientAreaWnd::SerializeTabGroup — retail entry RVA 0x81320 (mfc140);
// argument order verified from the call sites inside Serialize (this,
// CArchive&, CMFCTabCtrl*, BOOL).  Stores the group's m_ResizeMode, its
// window rect and, per tab, the document path of the tab's CMDIChildWndEx;
// on load it recreates the group (CreateTabGroup), reopens each document
// through the app's document templates and re-attaches the MDI child.
// STUB: stubbed alongside Serialize (its only caller).
// Symbol: ?SerializeTabGroup@CMDIClientAreaWnd@@AEAAXAEAVCArchive@@PEAVCMFCTabCtrl@@H@Z
extern "C" void MS_ABI impl__SerializeTabGroup_CMDIClientAreaWnd__AEAAXAEAVCArchive__PEAVCMFCTabCtrl__H_Z(void* pThis, void* pArchive, void* pTabGroup, int bSetRelation) {
    (void)pThis;
    (void)pArchive;
    (void)pTabGroup;
    (void)bSetRelation;
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
// Deviation: retail reads pChild->m_pRelatedTabGroup, which OpenMFC's
// CMDIChildWndEx ctor never initialises.  The group is derived instead with
// FindTabWndByChild(hwnd) -- the invariant retail maintains for that pointer
// (MoveWindowToTabGroup writes it as it adds the tab) -- which also yields the
// tab index, so GetTabFromHwnd is not repeated.
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
        if (pGroup == nullptr) {
            continue;
        }
        RECT rc = {};
        ::GetWindowRect(WndHandle(pGroup), &rc);
        if (::PtInRect(&rc, pt)) {
            return pGroup;
        }
    }
    return nullptr;
}

// CMDIClientAreaWnd::UpdateMDITabbedGroups — retail entry RVA 0x7f5b0
// (mfc140; 0x7f020 in mfc140u).  Not on this pass's assignment list.  The
// tabbed-groups counterpart of UpdateTabs: walks the MDI children, assigns
// each CMDIChildWndEx to a group (creating the first one with CreateTabGroup
// when none exists), synchronises labels/icons through the same CMFCTabCtrl
// virtuals UpdateTabs uses and re-lays the groups out.
// STUB: blocked on the same CMFCTabCtrl/CMDIChildWndEx virtuals as
// UpdateTabs; not transcribed.
// Symbol: ?UpdateMDITabbedGroups@CMDIClientAreaWnd@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateMDITabbedGroups_CMDIClientAreaWnd__QEAAXH_Z(void* pThis, int bSetActiveTabVisible) {
    (void)pThis;
    (void)bSetActiveTabVisible;
}
