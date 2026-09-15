// CMDIChildWndEx — OpenMFC implementation.
// Sources: cbarcore.cpp, global_other-3_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC — CMDIFrameWndEx / CMDIChildWndEx method stubs (wave1_other-3).
//
// Every function here was decoded from the retail mfc140u export (ordinal ->
// RVA -> objdump, see /tmp/openmfc_remaining/disasm.py) rather than inferred.
//
// Retail CMDIFrameWndEx embeds a CDockingManager sub-object at this+0x228 and a
// CMDIClientAreaWnd at this+0x720; retail CMDIChildWndEx embeds a CFrameImpl at
// this+0x218 and a CDockingManager at this+0x3a8. OpenMFC's simplified headers
// (afxmfc.h) model neither class beyond a padding blob.  Since the 2026-09-15
// pass the child's two sub-objects (and its m_pMDIFrame) are constructed on the
// heap and reached through the per-child ChildExtra record (see the layout note
// in the anonymous namespace); the dock-manager forwarders, OnChangeVisualManager
// and OnMDIActivate use them.  Every OTHER retail access to an unmodelled offset
// -- the tab proxy at +0x720, m_pTabbedControlBar at +0x710, the scalar flags,
// and the CFrameImpl::On* mouse/non-client forwards in the message handlers,
// which are NOT wired to the ChildExtra CFrameImpl yet -- is still skipped with
// a `TODO(clean-room)` note; no offsets are invented and no header is edited.
//
// Where a retail body is a bare `ret` / `xor eax,eax; ret`, the function is
// transcribed faithfully. Message handlers that end in a tail call to a base
// export are routed through the corresponding sibling impl_ function. The rest
// are type-correct conservative versions (safe defaults, no state corruption).

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#include <windows.h>
#include <shobjidl.h>
#include <cstddef>
#include <cstring>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
// CWnd::Default (thunks.cpp)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
// CFrameWnd::OnSysCommand (thunks.cpp)
extern "C" void MS_ABI impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(
    CFrameWnd* pThis, unsigned int nID, __int64 lParam);
// CMDIChildWnd::OnUpdateFrameTitle / PreCreateWindow / PreTranslateMessage
// (frame_font_exports.cpp)
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(
    CMDIChildWnd* pThis, int bAddToTitle);
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(
    CMDIChildWnd* pThis, MSG* pMsg);
// CMDIChildWnd::ActivateFrame / OnMDIActivate / OnSize
// (frame/Thunks.cpp, frame/CMDIChildWnd.cpp)
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWnd__UEAAXH_Z(
    CMDIChildWnd* pThis, int nCmdShow);
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWnd__IEAAXHPEAVCWnd__0_Z(
    CMDIChildWnd* pThis, int bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
extern "C" void MS_ABI impl__OnSize_CMDIChildWnd__IEAAXIHH_Z(
    CMDIChildWnd* pThis, unsigned int nType, int cx, int cy);
// CWnd helpers (window/Thunks.cpp, window/CWnd.cpp)
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__GetDescendantWindow_CWnd__SAPEAV1_PEAUHWND____HH_Z(
    HWND hWnd, int nID, int bOnlyPerm);
extern "C" void MS_ABI
impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* rString);
// CObject::IsKindOf (runtime/CObject.cpp)
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
// CFrameWnd::GetActiveDocument (frame/Thunks.cpp), CDocument::GetPathName
// (detail/DocviewSupport.cpp)
extern "C" CDocument* MS_ABI impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(
    CFrameWnd* pThis);
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
    const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);
// RUNTIME_CLASS descriptors (frame/RuntimeClasses.cpp, frame/CMDIFrameWndEx.cpp,
// ole/RuntimeClasses.cpp)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ();
// AFX_WM_AFTER_TASKBAR_ACTIVATE -- registered message id (runtime/Globals.cpp)
extern "C" unsigned int impl__AFX_WM_AFTER_TASKBAR_ACTIVATE__3IA;
// CMDIFrameWndEx::m_bDisableSetRedraw -- exported static BOOL (frame/StaticData.cpp).
// NOTE: retail mfc140u initialises this static to 1; OpenMFC currently
// initialises it to 0 (see the headerRequest that accompanies this file).
extern "C" int impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA;

// ---- forward declarations of exports defined further down in THIS file ----
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(
    CMDIChildWndEx* pThis, int bCheckRegisteredTab);
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(
    CMDIChildWndEx* pThis, CMDIChildWndEx* pTabWnd);
extern "C" int MS_ABI impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis);
extern "C" int MS_ABI impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis);
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(
    CMDIChildWndEx* pThis, void* pLayout);
extern "C" void MS_ABI impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ(
    CMDIChildWndEx* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bNotify);
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int nCmdShow);
extern "C" void MS_ABI impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ(
    CMDIChildWndEx* pThis);
extern "C" CRect* MS_ABI impl__GetTaskbarThumbnailClipRect_CMDIChildWndEx__UEBA_AVCRect__XZ(
    const CMDIChildWndEx* pThis, CRect* pRet);
extern "C" int MS_ABI impl__SetTaskbarThumbnailClipRect_CMDIChildWndEx__MEAAHVCRect___Z(
    CMDIChildWndEx* pThis, const CRect& rectClip);

// ---- more sibling exports (2026-09-14 pass) ----
// CWnd::IsWindowEnabled (window/CWnd.cpp), CWnd::ScreenToClient(LPRECT)
// (window/Thunks.cpp)
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
    const CWnd* pThis, RECT* lpRect);
// CDocument::IsModified -- OpenMFC-internal helper (detail/DocviewSupport.cpp)
// that reads CDocument::m_bModified; retail's ?IsModified@CDocument@@ is an
// inline virtual with no export, dispatched through CDocument vtable slot
// +0xd0 (the first thing ?SaveModified@CDocument@@UEAAHXZ, mfc140u 0x21b5b0,
// does is `call vtbl[0xd0]; test eax; je` -- the documented `if (!IsModified())
// return TRUE;`).
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis);
// CFrameWnd::OnCreateHelper / OnSetPreviewMode (frame/Thunks.cpp)
extern "C" int MS_ABI impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
    CFrameWnd* pThis, CREATESTRUCTW* pcs, CCreateContext* pContext);
extern "C" void MS_ABI impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(
    CFrameWnd* pThis, int bPreview, CPrintPreviewState* pState);
// CMDIFrameWnd::MDIGetActive (frame/Thunks.cpp)
extern "C" CMDIChildWnd* MS_ABI impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
    const CMDIFrameWnd* pThis, int* pbMaximized);
// CFrameImpl::AddFrame (frame/CFrameImpl.cpp)
extern "C" void MS_ABI impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(CFrameWnd* pFrame);
// AFX_WM_CHANGEVISUALMANAGER -- registered message id (runtime/Globals.cpp)
extern "C" unsigned int impl__AFX_WM_CHANGEVISUALMANAGER__3IA;
// afxGlobalData -- the 720-byte blob (featurepack/CMFC_misc_stubs.cpp), its
// Initialize export and GetITaskbarList3 (runtime/AFX_GLOBAL_DATA.cpp; the
// latter returns NULL in this tree).
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
extern "C" void* MS_ABI impl__GetITaskbarList3_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList3__XZ();

// ---- sibling exports for the embedded sub-objects (2026-09-15 pass) ----
// CDockingManager ctor/dtor (featurepack/docking/Thunks.cpp) and the exports
// the retail `this += 0x3a8` forwarders below reach
// (featurepack/docking/CDockingManager.cpp, featurepack/docking/Thunks.cpp).
// CPoint passed by value is an 8-byte aggregate in a register under MS_ABI and
// is modelled as `long long` here, the same way core/frame/CMDIFrameWndEx.cpp
// declares these.
extern "C" void* MS_ABI impl___0CDockingManager__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CDockingManager__UEAA_XZ(void* pThis);
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int bTail, int bAutoHide, int bInsertForOuterEdge);
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter);
extern "C" void MS_ABI impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CDockingManager* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect);
extern "C" int MS_ABI impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
    CDockingManager* pThis, CPane* pBarToDock, CPane* pBar);
extern "C" int MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(
    CDockingManager* pThis, unsigned long dwStyle);
extern "C" int MS_ABI impl__EnableDocking_CDockingManager__QEAAHK_Z(
    CDockingManager* pThis, unsigned long dwDockStyle);
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames);
// NOTE: the definition of this one in featurepack/docking/CDockingManager.cpp
// still carries an auto-generated `(void* p0, unsigned long* p1, int* p2)`
// list (no `point` argument); it is a `return 0` that touches nothing, so the
// call is harmless today -- see the headerRequest filed with this file.
extern "C" int MS_ABI impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
    const CDockingManager* pThis, long long point, unsigned long* pdwBarAlignment, int* pbOuterEdge);
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(
    CDockingManager* pThis, CWnd* pFrame);
extern "C" void MS_ABI impl__OnActivateFrame_CDockingManager__UEAAXH_Z(
    CDockingManager* pThis, int bActivate);
extern "C" int MS_ABI impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
    CDockingManager* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam);
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(
    CDockingManager* pThis, void* hdwp);
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide,
    CBasePane* pBarReplacement);
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* pThis, long long point, int nSensitivity, unsigned long* pdwAlignment,
    CRuntimeClass* pRTCBarType, const CDockingManager* pDockManager);
// (the H_N PaneFromPoint variant is declared by detail/MfccoreSupport.h)
// CFrameImpl ctor/dtor and OnChangeVisualManager (core/frame/CFrameImpl.cpp;
// that file keys every member on the object pointer, so a heap block of the
// retail size is all it needs).
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame);
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis);
// CBasePane::CanFloat (featurepack/docking/Thunks.cpp; dispatches the C++ virtual)
extern "C" int MS_ABI impl__CanFloat_CBasePane__UEBAHXZ(const CBasePane* pThis);
// CMDIChildWnd::GetMDIFrame (frame/Thunks.cpp)
extern "C" CMDIFrameWnd* MS_ABI impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(
    CMDIChildWnd* pThis);
// CFrameImpl::RemoveFrame (frame/CFrameImpl.cpp), CMDIChildWnd::UpdateClientEdge
// (frame/CMDIChildWnd.cpp; a `return FALSE` stub there) and CFrameWnd::OnDestroy
// (frame/Thunks.cpp) -- the OnDestroy tail.
extern "C" void MS_ABI impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(CFrameWnd* pFrame);
extern "C" int MS_ABI impl__UpdateClientEdge_CMDIChildWnd__IEAAHPEAUtagRECT___Z(
    CMDIChildWnd* pThis, RECT* lpRect);
extern "C" void MS_ABI impl__OnDestroy_CFrameWnd__IEAAXXZ(CFrameWnd* pThis);
// ?m_bEnableFloatingBars@CMDIChildWndEx@@2HA -- exported static BOOL
// (core/frame/StaticData.cpp), mfc140u data RVA 0x3be24c.
extern "C" int impl__m_bEnableFloatingBars_CMDIChildWndEx__2HA;

// GDI+ flat API, resolved at link time against the gdiplus import library the
// phase4 link already carries (featurepack/visualmanager/CPngImage.cpp declares
// the same surface the same way).  Used by OnTaskbarTabThumbnailStretch.
struct GdiplusStartupInput {
    UINT32 GdiplusVersion;
    void*  DebugEventCallback;
    BOOL   SuppressBackgroundThread;
    BOOL   SuppressExternalCodecs;
};
extern "C" {
int  WINAPI GdiplusStartup(ULONG_PTR*, const GdiplusStartupInput*, void*);
void WINAPI GdiplusShutdown(ULONG_PTR);
int  WINAPI GdipCreateBitmapFromHBITMAP(HBITMAP, HPALETTE, void**);
int  WINAPI GdipDisposeImage(void*);
int  WINAPI GdipCreateFromHDC(HDC, void**);
int  WINAPI GdipDeleteGraphics(void*);
int  WINAPI GdipSetInterpolationMode(void*, int);
int  WINAPI GdipDrawImageRectI(void*, void*, int, int, int, int);
}

namespace {
// Retail's `if (!afxGlobalData.<+0x00>) { afxGlobalData.Initialize();
// <+0x00> = 1; }` prologue, emitted inline before every AFX_GLOBAL_DATA use in
// this class (e.g. at 0x8687c..0x86914 inside SetTaskbarThumbnailClipRect,
// entry 0x868e0, mfc140u).  Same helper shape as core/frame/CFrameImpl.cpp.
void EnsureGlobalDataInitialized() {
    int* pInitialized = reinterpret_cast<int*>(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
    if (*pInitialized == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ();
        *pInitialized = 1;
    }
}

int GlobalDataInt(size_t offset) {
    int value = 0;
    memcpy(&value, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + offset, sizeof(value));
    return value;
}

// afxGlobalData +0x258 (mfc140u data 0x3c1878 == 0x3c1620 + 0x258): the flag
// the taskbar-tab bodies of this class test right after the init prologue
// (IsTaskbarTabsSupportEnabled 0x84082, SetTaskbarThumbnailClipRect 0x86914).
// It is AFX_GLOBAL_DATA::bIsWindows7 of the shipping afxglobals.h: the ten
// CFonts start at +0x1a8 (core/frame/CMDIClientAreaWnd.cpp reads that out of
// the retail AFX_GLOBAL_DATA ctor) and end at +0x248, `CRect m_rectVirtual`
// occupies +0x248..+0x258 and `BOOL bIsWindows7` is declared right after it.
// core/frame/CMDIClientAreaWnd.cpp reads the same slot as kGlobalTaskbarFlag.
// OpenMFC's Initialize export is a no-op, so this reads 0 in this tree.
constexpr size_t kGlobalTaskbarFlag = 0x258;

// The CMFCVisualManager vftable slot +0x3f8 (mfc140u vftable 0x31c128, installed
// by ??0CMFCVisualManager@@QEAA@H@Z at 0x184030; the slot points at the
// COMDAT-folded `xor eax,eax; ret` at 0x71e0).  OnNcCalcSize (0x86430) and
// OnNcPaint (0x85f20) test it on the instance returned by the inlined
// GetInstance helper at 0x9774.  OpenMFC's CMFCVisualManager (afxmfc.h) does
// not declare the virtual, so no manager here can answer TRUE; kept as one
// helper -- the same shape core/frame/CFrameImpl.cpp uses -- so a future
// manager can be wired in.
BOOL VisualManagerIsOwnerDrawCaption() {
    return FALSE;
}

// Models retail's process-wide re-entrancy latch for
// CMDIChildWndEx::OnMDIActivate (mfc140u data RVA 0x3c3e54, an int that the
// handler sets to 1 around the base dispatch and clears afterwards).  Retail's
// is a plain global with no synchronisation; this mirrors that.
bool g_bInMDIActivate = false;

// ---------------------------------------------------------------------------
// Retail CMDIChildWndEx layout (sizeof 0x818; ctor ??0CMDIChildWndEx@@IEAA@XZ,
// entry RVA 0x83820, mfc140u), after the 0x1e8-byte CMDIChildWnd base:
//   +0x1e8  m_pRelatedTabGroup   (zeroed by the ctor; core/frame/CMDIClientAreaWnd.cpp writes it)
//   +0x1f0  8 bytes zeroed, +0x1f8 int zeroed, +0x1fc CRect (::SetRectEmpty),
//   +0x20c  8 bytes zeroed (+0x20c / +0x210)
//   +0x218  CFrameImpl      m_Impl(this)       (ctor 0x61ef0 mfc140u, 0x190 bytes)
//   +0x3a8  CDockingManager m_dockManager      (default ctor 0x46ef0 mfc140u, 0x368
//                                               bytes; its m_pParentWnd is +0x1b0,
//                                               i.e. this+0x558, stored by PreCreateWindow)
//   +0x710  CDockablePane*  m_pTabbedControlBar (zeroed)
//   +0x718  CMDIFrameWndEx* m_pMDIFrame         (zeroed; OnCreate stores it)
//   +0x720  CMDITabProxyWnd m_wndTaskbarTabProxy (ctor 0x86bb0; its m_hWnd is +0x760)
//   +0x810  BOOL zeroed, +0x814 BOOL set to 1 (the taskbar thumbnail-clip enable)
// OpenMFC's CMDIChildWndEx is 0x228 bytes (CMDIChildWnd + a 64-byte padding
// blob; sizeof measured with this tree's headers), so nothing from +0x218 on
// fits in the object.  Exactly as core/frame/CMDIFrameWndEx.cpp does for its
// frame, m_Impl and m_dockManager are constructed on the heap through their
// ctor thunks and reached through the per-child record below, which also
// carries m_pMDIFrame.  The tab proxy at +0x720 is NOT constructed (see
// IsTaskbarTabsSupportEnabled for why), and +0x710/+0x810/+0x814 stay
// unmodelled.
// ---------------------------------------------------------------------------
// Retail's m_pRelatedTabGroup slot; core/frame/CMDIClientAreaWnd.cpp writes it
// raw at the same offset and OnDestroy clears it the same way.  Note that under
// the Itanium ABI this build compiles with, _mdichildwndex_padding itself
// starts at 0x1e4 (it is folded into CMDIChildWnd's tail padding), so the slot
// is addressed by absolute offset, never through the array; only the object
// size is relied on and pinned.
constexpr size_t kRelatedTabGroupOffset = 0x1e8;
static_assert(sizeof(CMDIChildWndEx) == 0x228, "OpenMFC CMDIChildWndEx is 0x228 bytes");
static_assert(kRelatedTabGroupOffset + sizeof(void*) <= sizeof(CMDIChildWndEx),
              "m_pRelatedTabGroup slot must lie inside the object");

struct FrameImplBlock { alignas(16) unsigned char bytes[0x190]; };
struct DockManagerBlock { alignas(16) unsigned char bytes[sizeof(CDockingManager)]; };

struct ChildExtra {
    const CMDIChildWndEx* pChild;
    FrameImplBlock* pImpl;            // retail m_Impl (this+0x218)
    DockManagerBlock* pDockManager;   // retail m_dockManager (this+0x3a8)
    CMDIFrameWndEx* pMDIFrame;        // retail m_pMDIFrame (this+0x718)
    ChildExtra* pNext;
};

// Hand-rolled list under an SRW lock, the same shape CMDIFrameWndEx.cpp uses
// (a std::unordered_map would add libstdc++ undefineds to this file).
SRWLOCK g_childExtraLock = SRWLOCK_INIT;
ChildExtra* g_childExtraHead = nullptr;

ChildExtra* FindExtra(const CMDIChildWndEx* pChild) {
    AcquireSRWLockExclusive(&g_childExtraLock);
    ChildExtra* p = g_childExtraHead;
    while (p != nullptr && p->pChild != pChild) {
        p = p->pNext;
    }
    ReleaseSRWLockExclusive(&g_childExtraLock);
    return p;
}

// Retail constructs m_Impl with (this) and m_dockManager with its default
// constructor (the call at 0x8386e inside the ctor, entry 0x83820, passes only
// rcx); m_dockManager's parent pointer is written later by PreCreateWindow
// (see there).
ChildExtra* AttachExtra(CMDIChildWndEx* pChild) {
    ChildExtra* p = FindExtra(pChild);
    if (p != nullptr) {
        return p;
    }
    p = new ChildExtra();
    p->pChild = pChild;
    p->pImpl = new FrameImplBlock();
    memset(p->pImpl->bytes, 0, sizeof(p->pImpl->bytes));
    impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(p->pImpl, pChild);
    p->pDockManager = new DockManagerBlock();
    memset(p->pDockManager->bytes, 0, sizeof(p->pDockManager->bytes));
    impl___0CDockingManager__QEAA_XZ(p->pDockManager);
    p->pMDIFrame = nullptr;
    AcquireSRWLockExclusive(&g_childExtraLock);
    p->pNext = g_childExtraHead;
    g_childExtraHead = p;
    ReleaseSRWLockExclusive(&g_childExtraLock);
    return p;
}

// Retail's destructor (0x83930) order is ~m_wndTaskbarTabProxy, ~m_dockManager,
// ~m_Impl; the two we construct are torn down in that order here.
void DetachExtra(CMDIChildWndEx* pChild) {
    AcquireSRWLockExclusive(&g_childExtraLock);
    ChildExtra** pp = &g_childExtraHead;
    while (*pp != nullptr && (*pp)->pChild != pChild) {
        pp = &(*pp)->pNext;
    }
    ChildExtra* p = *pp;
    if (p != nullptr) {
        *pp = p->pNext;
    }
    ReleaseSRWLockExclusive(&g_childExtraLock);
    if (p == nullptr) {
        return;
    }
    impl___1CDockingManager__UEAA_XZ(p->pDockManager);
    delete p->pDockManager;
    impl___1CFrameImpl__UEAA_XZ(p->pImpl);
    delete p->pImpl;
    delete p;
}

// The m_dockManager / m_Impl / m_pMDIFrame of a child, the record being created
// on first use if the child was built without this file's constructor (nothing
// in the tree does that today).
CDockingManager* DockManagerOf(CMDIChildWndEx* pThis) {
    ChildExtra* p = AttachExtra(pThis);
    return p != nullptr ? reinterpret_cast<CDockingManager*>(p->pDockManager) : nullptr;
}
void* FrameImplOf(CMDIChildWndEx* pThis) {
    ChildExtra* p = AttachExtra(pThis);
    return p != nullptr ? static_cast<void*>(p->pImpl) : nullptr;
}
CMDIFrameWndEx* MDIFrameOf(const CMDIChildWndEx* pThis) {
    ChildExtra* p = FindExtra(pThis);
    return p != nullptr ? p->pMDIFrame : nullptr;
}
void SetMDIFrame(CMDIChildWndEx* pThis, CMDIFrameWndEx* pFrame) {
    ChildExtra* p = AttachExtra(pThis);
    if (p != nullptr) {
        p->pMDIFrame = pFrame;
    }
}

// OpenMFC's CDockingManager keeps its parent frame in a protected member; the
// default-ctor thunk (what retail's ctor calls) leaves it NULL, and retail's
// PreCreateWindow then stores `this` into m_dockManager+0x1b0 (this+0x558)
// itself.  That store goes through this accessor (CMDIFrameWndEx.cpp does the
// same for its OnCreate).
struct DockManagerAccess : CDockingManager {
    using CDockingManager::m_pParentWnd;
};
void SetDockManagerParent(CDockingManager* pDock, CFrameWnd* pFrame) {
    if (pDock != nullptr) {
        static_cast<DockManagerAccess*>(pDock)->m_pParentWnd = pFrame;
    }
}

// The two CBasePane members the retail DockPane body touches through unexported
// inline virtuals (see that export); pinned against the retail layout that
// afxmfc.h carries for CBasePane.
static_assert(offsetof(CBasePane, m_dwEnabledAlignment) == 0x100, "CBasePane::m_dwEnabledAlignment");
static_assert(offsetof(CBasePane, m_dwControlBarStyle) == 0x108, "CBasePane::m_dwControlBarStyle");
}  // namespace

//=============================================================================
// CMDIChildWndEx — message handlers
//=============================================================================



















//=============================================================================
// CMDIFrameWndEx — docking / MDI-tab operations
//=============================================================================












// Symbol: ?CreateObject@CMDIChildWndEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDIChildWndEx__SAPEAVCObject__XZ() {
    return new CMDIChildWndEx();
}
// Symbol: ?GetThisClass@CMDIChildWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ() {
    return &CMDIChildWndEx::classCMDIChildWndEx;
}
// Symbol: ?GetRuntimeClass@CMDIChildWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDIChildWndEx__UEBAPEAUCRuntimeClass__XZ(CMDIChildWndEx* pThis) {
    return pThis ? &CMDIChildWndEx::classCMDIChildWndEx : nullptr;
}
// CMDIChildWndEx::EnableDocking(DWORD) -- retail mfc140u.dll entry RVA 0x85290
// is a two-instruction tail jump: `this += 0x3a8` then
// ?EnableDocking@CDockingManager@@QEAAHK@Z (0x473f0), whose BOOL result is
// returned directly.  Forwarded to the ChildExtra dock manager (see the layout
// note above).  The previous body called CFrameWnd::EnableDocking on the frame
// itself and returned a constant TRUE.
// Symbol: ?EnableDocking@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_CMDIChildWndEx__QEAAHK_Z(CMDIChildWndEx* pThis, DWORD dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    return impl__EnableDocking_CDockingManager__QEAAHK_Z(DockManagerOf(pThis), dwDockStyle);
}
// CMDIChildWndEx::RegisterTaskbarTab(CMDIChildWndEx*) -- retail mfc140u.dll
// entry RVA 0x83ca0:
//     if (!IsTaskbarTabsSupportEnabled()) return;                 // 0x84000
//     if (m_tabProxyWnd.m_hWnd != NULL) return;                   // this+0x720 +0x40
//     m_tabProxyWnd.m_pRelatedMDIChildFrame = this;               // this+0x808
//     CRect rc(-32000, -32000, -31990, -31990);
//     <afxGlobalData init prologue>
//     CString strClass = afxGlobalData.RegisterWindowClass(L"AFX_SUPERBAR_TAB");  // 0x6bed0,
//                                                          // the literal at mfc140u 0x33fbb8
//     CString strText; CWnd::GetWindowTextW(strText);             // 0x28be00
//     if (!m_tabProxyWnd.vtbl[0xc0](0x08000080 /*WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE*/,
//             strClass, strText, 0x80cf0000, &rc, NULL, 0, NULL)) goto done;   // CreateEx
//     <init prologue>; ITaskbarList3* p = afxGlobalData.GetITaskbarList3();  // 0x6ba00
//     if (p == NULL) goto registered;
//     frame = GetTopLevelFrame() as CMDIFrameWndEx (else NULL);   // 0x28e490 / 0x234cf0
//     if (FAILED(p->RegisterTab(proxyHwnd, frameHwnd))) goto fail;   // slot 0x58
//     if (pWndBefore == NULL) {
//         if (m_pMDIFrame == NULL) goto fail;                     // this+0x718
//         pWndBefore = m_pMDIFrame->m_wndClientArea.
//             FindNextRegisteredWithTaskbarMDIChild(this);        // 0x7e400 on frame+0x720
//         if (pWndBefore == NULL) goto fail;
//     }
//     if (FAILED(p->SetTabOrder(proxyHwnd, pWndBefore->m_tabProxyWnd.m_hWnd))) goto fail;  // slot 0x68
//     if (m_pMDIFrame && m_pMDIFrame->MDIGetActive(NULL) == this) SetTaskbarTabActive();  // 0x2a6ad0 / 0x843c0
//     BOOL bTrue = TRUE;
//     <0x1cadcc>(proxyHwnd, 10 /*DWMWA_HAS_ICONIC_BITMAP*/, &bTrue);            // dwmapi loader
//     <0x1cadcc>(proxyHwnd, 7 /*DWMWA_FORCE_ICONIC_REPRESENTATION*/, &bTrue);   //   wrapper
//     SetTaskbarTabProperties(m_dwDefaultTaskbarTabPropertyFlags);   // 0x842c0, data 0x3b1af8
//     this->vtbl[0x458](strText);                                 // SetTaskbarTabText
//   registered:
//     m_bTabRegistered = TRUE;                                    // this+0x810
//     if (m_tabProxyWnd.m_hWnd) InvalidateIconicBitmaps();        // 0x840b0
//   done:
//     <strText / strClass destructors>; return;
//   fail:
//     UnregisterTaskbarTab(TRUE); goto done;                      // 0x855d0
// Only the first gate is reachable in OpenMFC: IsTaskbarTabsSupportEnabled()
// returns FALSE here (see that export), and past it retail needs the
// CMDITabProxyWnd embedded at this+0x720 (OpenMFC's CMDIChildWndEx is 0x228
// bytes, retail's is 0x818; the proxy is never constructed), its back-pointer
// at +0x808, the flag at +0x810, ITaskbarList3 and the owning frame at +0x718.
// The gate is transcribed; everything after it is left out deliberately.
// STUB past the gate.
// Symbol: ?RegisterTaskbarTab@CMDIChildWndEx@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pTabWnd) {
    (void)pTabWnd;
    if (!pThis) {
        return;
    }
    if (!impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        return;
    }
    // TODO(clean-room): proxy-window creation and ITaskbarList3::RegisterTab /
    // SetTabOrder need the never-constructed tab proxy (this+0x720/+0x808)
    // and the frame's unconstructed m_wndClientArea; skipped.
}
// CMDIChildWndEx::UnregisterTaskbarTab(BOOL) -- retail mfc140u.dll entry RVA
// 0x855d0:
//     if (m_tabProxyWnd.m_hWnd == NULL) return;                   // this+0x720 +0x40
//     <afxGlobalData init prologue>
//     ITaskbarList3* p = afxGlobalData.GetITaskbarList3();       // 0x6ba00
//     if (p) p->UnregisterTab(m_tabProxyWnd.m_hWnd);              // slot 0x60
//     if (m_tabProxyWnd.m_hWnd) m_tabProxyWnd.vtbl[0xd0]();       // DestroyWindow
//     if (!bCheckRegisteredTab) return;
//     CFrameWnd* f = GetTopLevelFrame();                          // 0x28e490
//     if (!f || !f->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return;
//     if (((CMDIFrameWndEx*)f)->GetRegisteredWithTaskBarMDIChildCount() != 0) return;  // 0x8a560
//     <init prologue>; p = GetITaskbarList3(); if (p) p->SetThumbnailClip(f->m_hWnd, NULL);  // slot 0xa0
// (An earlier version of this comment named the last call SetTabActive; the
// slot read at 0x856cc is +0xa0, SetThumbnailClip, with R8 zeroed and RDX =
// the frame's m_hWnd.)
// The very first test is on the proxy window's HWND at this+0x720+0x40, which
// OpenMFC does not model (the proxy is never constructed, see
// RegisterTaskbarTab), so the faithful reachable behaviour is the immediate
// return.  STUB -- left a no-op deliberately.
// Symbol: ?UnregisterTaskbarTab@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(CMDIChildWndEx* pThis, int bCheckRegisteredTab) {
    (void)pThis;
    (void)bCheckRegisteredTab;
}
// CMDIChildWndEx::SetTaskbarTabActive() -- retail mfc140u.dll entry RVA 0x843c0:
//     if (!IsTaskbarTabsSupportEnabled()) return;                 // 0x84000
//     <afxGlobalData init prologue>
//     ITaskbarList3* p = afxGlobalData.GetITaskbarList3();       // 0x6ba00
//     if (p == NULL) return;
//     CFrameWnd* f = GetTopLevelFrame();                          // 0x28e490
//     if (f && !f->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) f = NULL;
//     p->SetTabActive(m_tabProxyWnd.m_hWnd, f ? f->m_hWnd : NULL, 0);   // slot 0x70
// The gate is transcribed; past it the proxy HWND at this+0x720+0x40 is
// unmodeled (and GetITaskbarList3 returns NULL in this tree anyway).
// STUB past the gate.
// Symbol: ?SetTaskbarTabActive@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__SetTaskbarTabActive_CMDIChildWndEx__QEAAXXZ(CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    if (!impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        return;
    }
    // TODO(clean-room): ITaskbarList3::SetTabActive on the proxy HWND at
    // this+0x720+0x40; unmodeled.
}
// CMDIChildWndEx::SetTaskbarTabOrder(CMDIChildWndEx*) -- retail mfc140u.dll
// entry RVA 0x84220:
//     if (!IsTaskbarTabsSupportEnabled()) return;                 // 0x84000
//     if (m_tabProxyWnd.m_hWnd == NULL) return;                   // this+0x720 +0x40
//     <afxGlobalData init prologue>
//     ITaskbarList3* p = afxGlobalData.GetITaskbarList3();       // 0x6ba00
//     HWND hBefore = pWndInsertBefore ? pWndInsertBefore->m_tabProxyWnd.m_hWnd : NULL;
//     if (p) p->SetTabOrder(m_tabProxyWnd.m_hWnd, hBefore);       // slot 0x68
// The gate is transcribed; the proxy HWNDs are unmodeled.  STUB past the gate.
// Symbol: ?SetTaskbarTabOrder@CMDIChildWndEx@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetTaskbarTabOrder_CMDIChildWndEx__QEAAXPEAV1__Z(CMDIChildWndEx* pThis, CMDIChildWndEx* pWndInsertBefore) {
    (void)pWndInsertBefore;
    if (!pThis) {
        return;
    }
    if (!impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        return;
    }
    // TODO(clean-room): ITaskbarList3::SetTabOrder on the two proxy HWNDs at
    // +0x720+0x40; unmodeled.
}
// CMDIChildWndEx::SetTaskbarTabProperties(DWORD) -- retail mfc140u.dll entry
// RVA 0x842c0:
//     if (!IsTaskbarTabsSupportEnabled()) return;                 // 0x84000
//     if (m_tabProxyWnd.m_hWnd == NULL) return;                   // this+0x720 +0x40
//     <afxGlobalData init prologue>
//     ITaskbarList3* p = afxGlobalData.GetITaskbarList3();       // 0x6ba00
//     ITaskbarList4* p4 = NULL;
//     if (p && FAILED(p->QueryInterface(<IID at mfc140u 0x33fbf8>, &p4))) p4 = NULL;
//     if (p4) { p4->SetTabProperties(m_tabProxyWnd.m_hWnd, dwFlags);   // slot 0xa8
//               p4->Release(); }                                   // slot 0x10
// Slot 0xa8 is the 22nd method: IUnknown(3) + ITaskbarList(5) +
// ITaskbarList2(1) + ITaskbarList3(12) = 21 slots before it, i.e. the first
// ITaskbarList4 method, SetTabProperties; the sibling exports' slots
// (0x58 RegisterTab, 0x60 UnregisterTab, 0x68 SetTabOrder, 0x70 SetTabActive,
// 0xa0 SetThumbnailClip) line up with ITaskbarList3 the same way.
// The gate is transcribed; the proxy HWND is unmodeled.  STUB past the gate.
// Symbol: ?SetTaskbarTabProperties@CMDIChildWndEx@@QEAAXK@Z
extern "C" void MS_ABI impl__SetTaskbarTabProperties_CMDIChildWndEx__QEAAXK_Z(CMDIChildWndEx* pThis, DWORD dwFlags) {
    (void)dwFlags;
    if (!pThis) {
        return;
    }
    if (!impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        return;
    }
    // TODO(clean-room): ITaskbarList4::SetTabProperties on the proxy HWND at
    // this+0x720+0x40; unmodeled.
}
// Symbol: ?SetTaskbarTabText@CMDIChildWndEx@@MEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTaskbarTabText_CMDIChildWndEx__MEAAXPEB_W_Z(CMDIChildWndEx* pThis, const wchar_t* lpszTabText) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_taskbarTabText[pThis] = lpszTabText ? lpszTabText : L"";
}
// CMDIChildWndEx::SetTaskbarThumbnailClipRect(CRect) -- transcribed from retail
// mfc140u.dll entry RVA 0x868e0.  `this` arrives in RCX and the by-value CRect
// as a pointer to the caller's copy in RDX (a 16-byte aggregate under MS_ABI),
// which retail then modifies in place:
//     <afxGlobalData init prologue>
//     if (afxGlobalData.<+0x258> == 0) return FALSE;              // data 0x3c1878
//     CFrameWnd* f = GetTopLevelFrame();                          // 0x28e490
//     if (f == NULL || !f->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return FALSE;
//     if (((CMDIFrameWnd*)f)->MDIGetActive(NULL) != this) return FALSE;   // 0x2a6ad0
//     if (rect.left | rect.right | rect.top | rect.bottom)        // any non-zero
//         f->ScreenToClient(&rect);                               // 0x2a32b0, on the FRAME
//     <init prologue>; ITaskbarList3* p = afxGlobalData.GetITaskbarList3();  // 0x6ba00
//     if (p == NULL) return FALSE;
//     LPRECT prc = &rect;
//     if (!(rect.left | rect.right | rect.top | rect.bottom) || ::IsRectEmpty(&rect))
//         prc = NULL;                                             // IAT 0x2c72a0
//     return SUCCEEDED(p->SetThumbnailClip(f->m_hWnd, prc)) ? TRUE : FALSE;   // slot 0xa0
// Slot 0xa0 is the 21st method of the interface (IUnknown 3 + ITaskbarList 5 +
// ITaskbarList2 1 + ITaskbarList3 12 = 21 slots up to and including it), i.e.
// ITaskbarList3::SetThumbnailClip in shobjidl.h's declaration order.
// Every input exists here: the +0x258 flag is read out of the afxGlobalData
// blob (0 in this tree, because AFX_GLOBAL_DATA::Initialize is a no-op), and
// GetITaskbarList3 returns NULL in this tree, so the result today is FALSE on
// the same early-outs retail takes when the taskbar is unavailable.  The
// previous body was a bare `return FALSE`.
// Symbol: ?SetTaskbarThumbnailClipRect@CMDIChildWndEx@@MEAAHVCRect@@@Z
extern "C" int MS_ABI impl__SetTaskbarThumbnailClipRect_CMDIChildWndEx__MEAAHVCRect___Z(CMDIChildWndEx* pThis, const CRect& rectClip) {
    if (!pThis) {
        return FALSE;
    }
    EnsureGlobalDataInitialized();
    if (GlobalDataInt(kGlobalTaskbarFlag) == 0) {
        return FALSE;
    }
    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrame == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pFrame, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }
    if (impl__MDIGetActive_CMDIFrameWnd__QEBAPEAVCMDIChildWnd__PEAH_Z(
            static_cast<CMDIFrameWnd*>(pFrame), nullptr) != pThis) {
        return FALSE;
    }
    // OpenMFC's CRect is layout-compatible with RECT (four ints, same order),
    // which is what retail relies on when it hands the copy to USER32.
    RECT rect = {rectClip.left, rectClip.top, rectClip.right, rectClip.bottom};
    const bool bAnyNonZero = (rect.left | rect.right | rect.top | rect.bottom) != 0;
    if (bAnyNonZero) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pFrame, &rect);
    }
    EnsureGlobalDataInitialized();
    ITaskbarList3* pTaskbar = static_cast<ITaskbarList3*>(
        impl__GetITaskbarList3_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList3__XZ());
    if (pTaskbar == nullptr) {
        return FALSE;
    }
    RECT* prc = &rect;
    if (!((rect.left | rect.right | rect.top | rect.bottom) != 0) || ::IsRectEmpty(&rect)) {
        prc = nullptr;
    }
    return SUCCEEDED(pTaskbar->SetThumbnailClip(pFrame->m_hWnd, prc)) ? TRUE : FALSE;
}
// CMDIChildWndEx::CanShowOnMDITabs() -- transcribed from retail mfc140u.dll
// RVA 0x86b00:
//     if (GetStyle() & WS_VISIBLE) return TRUE;              // bt $0x1c
//     CDocument* pDoc = GetActiveDocument();                 // vtable +0x2e8
//     if (pDoc == NULL) return FALSE;
//     if (!pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc))) return FALSE;
//     return pDoc->[+0x124] != 0;
// CDocument+0x124 is m_bEmbedded: ?SetPathName@CDocument@@UEAAXPEBDH@Z (entry
// RVA 0x2194c0 in the ANSI twin mfc140.dll) stores the path string at this+0x48
// and clears this+0x124 in the same breath, which is MFC's documented
// `m_strPathName = ...; m_bEmbedded = FALSE;` pair.  OpenMFC keeps m_bEmbedded
// on COleServerDoc (afxole.h) rather than on CDocument, so the downcast above
// reaches the same flag.
// Symbol: ?CanShowOnMDITabs@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__CanShowOnMDITabs_CMDIChildWndEx__UEAAHXZ(CMDIChildWndEx* pThis) {
    if (!pThis) {
        return FALSE;
    }
    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_VISIBLE) {
        return TRUE;
    }
    CDocument* pDoc = impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(pThis);
    if (!pDoc) {
        return FALSE;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pDoc, impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }
    return static_cast<COleServerDoc*>(pDoc)->IsEmbedded() ? TRUE : FALSE;
}
// CMDIChildWndEx::OnSetText(const wchar_t*) — retail (RVA 0x84720) calls
// CWnd::Default() and propagates its result, then forwards the text to the
// parent MDI frame's client area (this+0x718/+0x720 UpdateTabs), runs
// CFrameImpl::OnSetText (this+0x218) and a vtable-0x458 virtual. The base
// result is preserved; the unmodeled sub-object updates are skipped.
// Symbol: ?OnSetText@CMDIChildWndEx@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMDIChildWndEx__IEAAHPEB_W_Z(
    CMDIChildWndEx* pThis, const wchar_t* lpszText) {
    if (!pThis) return 0;
    int nResult = static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
    // TODO(clean-room): transcribed partially — retail also updates the MDI
    // frame client area (this+0x718 +0x720), CFrameImpl (this+0x218) and a
    // vtable-0x458 virtual with lpszText; unmodeled.
    return nResult;
}
// CMDIChildWndEx::OnSize(UINT, int, int) — transcribed from retail mfc140u.dll
// RVA 0x84c60.  Retail:
//     if (this->[0x1f0] != 0) return;                    // suspend-layout flag
//     InvalidateIconicBitmaps();                          // 0x840b0
//     this->[0x1f8] = (nType == SIZE_MINIMIZED);
//     if (pVM->vtbl[0x3f8]()) {   // pVM = the inlined CMFCVisualManager
//                                 // singleton accessor at mfc140.dll 0x97f4,
//                                 // which lazily news 0x108 bytes and runs
//                                 // ??0CMFCVisualManager@@QEAA@H@Z (0x182640)
//         if (this->[0x718] && ((char*)this->[0x718])[0x4668] == 0)
//              CFrameImpl(this+0x218).OnWindowPosChanging(&wp);   // 0x652b0
//         else if (this->[0x23c]) { this->[0x23c] = 0;
//              ::SetWindowRgn(m_hWnd, NULL, TRUE); }
//     }
//     if (this->[0x1f8] == 0 && nType != SIZE_MAXIMIZED && this->[0x1f4] == 0) {
//         this->[0x3b8] = 1;
//         CMDIChildWnd::OnSize(nType, cx, cy);            // 0x2a72b0
//         vtbl[0x3a8](NULL);                              // AdjustDockingLayout
//         this->[0x3b8] = 0;
//     } else {
//         CMDIChildWnd::OnSize(nType, cx, cy);
//         if (nType == SIZE_MAXIMIZED ||
//             (nType == SIZE_RESTORED && this->[0x1f4] != 0)) {
//             vtbl[0x300](TRUE);                          // RecalcLayout(TRUE)
//             if (this->[0x120] && nType == SIZE_RESTORED) { ...
//                  ModifyStyle(0, 0x00cf0000, 0); }
//         }
//         this->[0x1f4] = (nType == SIZE_MAXIMIZED);
//     }
//     CFrameImpl(this+0x218).<0x65ee0>();
// OpenMFC has no slot for this+0x120/0x1f0/0x1f4/0x1f8/0x23c/0x3b8, and the
// ChildExtra CFrameImpl / m_pMDIFrame are not wired into this handler (the
// frame's +0x4668 flag is unreadable anyway), so those flags are taken as zero
// here.  With this->[0x1f4] (the "was maximized"
// latch) read as zero the SIZE_RESTORED arm of the second branch is
// unreachable, which is the one deviation worth naming: retail relayouts on the
// restore that follows a maximize, this build does not.  RecalcLayout and
// AdjustDockingLayout are dispatched virtually by retail (slots 0x300/0x3a8);
// here they go straight to this class's own exports, so an override in a
// further-derived class is not honoured.
// Symbol: ?OnSize@CMDIChildWndEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMDIChildWndEx__IEAAXIHH_Z(
    CMDIChildWndEx* pThis, unsigned int nType, int cx, int cy) {
    if (!pThis) {
        return;
    }
    impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(pThis);
    impl__OnSize_CMDIChildWnd__IEAAXIHH_Z(pThis, nType, cx, cy);
    if (nType != SIZE_MINIMIZED && nType != SIZE_MAXIMIZED) {
        impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(pThis, nullptr);
    } else if (nType == SIZE_MAXIMIZED) {
        impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(pThis, TRUE);
    }
}
// CMDIChildWndEx::OnSizing(UINT, LPRECT) — retail (RVA 0x85520) calls
// CWnd::Default(), compares the current window rect against the proposed rect
// and calls RecalcLayout(FALSE) when the sizes differ. The comparison is
// faithful (m_hWnd at 0x40 is modeled); the virtual RecalcLayout dispatch is
// routed to the base layout instead of the derived vtable slot.
// Symbol: ?OnSizing@CMDIChildWndEx@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_CMDIChildWndEx__IEAAXIPEAUtagRECT___Z(
    CMDIChildWndEx* pThis, unsigned int fwSide, RECT* pRect) {
    (void)fwSide;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (pThis->GetSafeHwnd() && pRect) {
        RECT rectWnd;
        ::GetWindowRect(pThis->GetSafeHwnd(), &rectWnd);
        const int nWndW = rectWnd.right - rectWnd.left;
        const int nWndH = rectWnd.bottom - rectWnd.top;
        const int nRectW = pRect->right - pRect->left;
        const int nRectH = pRect->bottom - pRect->top;
        if (nWndW != nRectW || nWndH != nRectH) {
            // TODO(clean-room): retail dispatches RecalcLayout(FALSE) through
            // the derived vtable; routed to the base layout conservatively.
            pThis->CFrameWnd::RecalcLayout(FALSE);
        }
    }
}
// CMDIChildWndEx::OnStyleChanged(int, LPSTYLESTRUCT) — transcribed from retail
// mfc140u.dll RVA 0x859c0.  Retail, in order:
//     CWnd::Default();                                        // 0x28ac80
//     if (IsTaskbarTabsSupportEnabled())  vtbl[0x3c0](NULL);   // RegisterTaskbarTab
//     else                                UnregisterTaskbarTab(TRUE);  // 0x855d0
//     if ((lpss->styleOld & WS_SYSMENU) == (lpss->styleNew & WS_SYSMENU)) return;
//     ... MDI-tabbed-group work driven by this+0x718 / this+0x1fc..0x208 /
//         this+0x23c, ModifyStyleEx, ::GetClientRect on the group window at
//         (this->[0x718])+0x760 and a final CWnd::SetWindowPos.
// The style-change tail needs the owning frame's m_wndClientArea (frame+0x720,
// reached through m_pMDIFrame; not constructed in this tree) and the cached
// client rect at this+0x1fc..0x208 / the flag at +0x23c (no slots here), so
// only the head is reproduced.  Retail dispatches RegisterTaskbarTab through
// vtable slot 0x3c0 (confirmed against the retail CMDIChildWndEx vtable at
// mfc140.dll 0x2ebf68); this build calls the class's own export directly, so an
// override in a further-derived class is not honoured.
// Symbol: ?OnStyleChanged@CMDIChildWndEx@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanged_CMDIChildWndEx__IEAAXHPEAUtagSTYLESTRUCT___Z(
    CMDIChildWndEx* pThis, int nStyleType, void* lpStyleStruct) {
    (void)nStyleType;
    (void)lpStyleStruct;
    if (!pThis) {
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(pThis, nullptr);
    } else {
        impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(pThis, TRUE);
    }
    // TODO(clean-room): the WS_SYSMENU-change tail needs the owning frame's
    // unconstructed m_wndClientArea and the cached client rect at
    // this+0x1fc..0x208.
}
// CMDIChildWndEx::OnSysCommand(UINT, LPARAM) — transcribed from retail
// mfc140u.dll entry RVA 0x85e20, whose whole body is:
//     if (nID == SC_CLOSE /*0xf100*/ && m_pMDIFrame != NULL              // this+0x718
//         && (m_pMDIFrame->m_wndClientArea.m_bTabIsEnabled                // frame+0x4574
//             || m_pMDIFrame->m_wndClientArea.m_bIsMDITabbedGroup))       // frame+0x4668
//         return;                       // i.e. `if (nID == SC_CLOSE && IsTabbedMDIChild()) return;`
//     CFrameWnd::OnSysCommand(nID, lParam);                               // 0x29e770
// (An earlier version of this comment described the gate as "a tabbed control
// bar attached and non-auto-hidden"; it is the owning frame's MDI-tabs /
// tabbed-groups flags, the same pair IsTabbedMDIChild reads.)  m_pMDIFrame is
// in ChildExtra, but the frame's m_wndClientArea is not constructed in this
// tree, so the two flags are unreadable and taken as FALSE -- the base dispatch
// is always performed.
// Symbol: ?OnSysCommand@CMDIChildWndEx@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMDIChildWndEx__IEAAXI_J_Z(
    CMDIChildWndEx* pThis, unsigned int nID, __int64 lParam) {
    if (!pThis) return;
    // TODO(clean-room): retail skips the close for SC_CLOSE when the owning
    // frame's MDI tabs / tabbed groups are on (frame+0x4574 / +0x4668, an
    // unconstructed frame sub-object here).
    impl__OnSysCommand_CFrameWnd__IEAAXI_J_Z(pThis, nID, lParam);
}
// CMDIChildWndEx::OnTaskbarTabThumbnailActivate(UINT, CWnd*, BOOL) — retail
// (RVA 0x866f0): when nState == 1 it activates the top-level frame through
// vtable slot 0x450 (ActivateTopLevelFrame). The vtable slot is not modeled in
// the simplified headers. Conservative no-op.
// Symbol: ?OnTaskbarTabThumbnailActivate@CMDIChildWndEx@@UEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnTaskbarTabThumbnailActivate_CMDIChildWndEx__UEAAXIPEAVCWnd__H_Z(
    CMDIChildWndEx* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    (void)pWndOther;
    (void)bMinimized;
    if (!pThis) return;
    if (nState == 1) {
        // TODO(clean-room): retail calls ActivateTopLevelFrame (vtable 0x450);
        // not modeled in the simplified headers.
    }
}
// CMDIChildWndEx::OnTaskbarTabThumbnailMouseActivate(CWnd*, UINT, UINT) —
// retail (RVA 0x86710) returns TRUE unconditionally; on WM_LBUTTONUP (0x202)
// it also activates the top-level frame (vtable 0x450). The constant result is
// kept; the vtable dispatch is unmodeled.
// Symbol: ?OnTaskbarTabThumbnailMouseActivate@CMDIChildWndEx@@UEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnTaskbarTabThumbnailMouseActivate_CMDIChildWndEx__UEAAHPEAVCWnd__II_Z(
    CMDIChildWndEx* pThis, CWnd* pDesktopWnd, unsigned int nHitTest, unsigned int message) {
    (void)pDesktopWnd;
    (void)nHitTest;
    (void)message;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — retail also activates the
    // top-level frame (vtable 0x450) on WM_LBUTTONUP.
    return TRUE;
}
// CMDIChildWndEx::OnTaskbarTabThumbnailStretch(HBITMAP, const CRect&, HBITMAP,
// const CRect&) — transcribed from retail mfc140u.dll entry RVA 0x86270
// (rectSrc, the stack argument, is never read):
//     if (hBmpSrc == NULL || hBmpDst == NULL) return FALSE;       // 0x86403: xor eax,eax
//     ATL::CImage imageSrc;                    // ctor 0x87a90: vftable 0x2edcf8, fields
//                                              // zeroed, s_initGDIPlus refcount++ under its
//                                              // critical section (singleton via 0x87a1c)
//     imageSrc.Attach(hBmpSrc, DIBOR_DEFAULT); // 0x87824: stores the HBITMAP at +0x8 and
//                                              // reads its DIBSECTION (::GetObject, 0x68)
//                                              // -- the CImage OWNS hBmpSrc from here on
//     CClientDC dc(this);                      // 0x2a3b20
//     CDC dcMem; dcMem.Attach(::CreateCompatibleDC(dc.m_hDC));   // vftable 0x33b510; 0x2a2480;
//                                                                // IAT 0x2c6288
//     HGDIOBJ hOld = ::SelectObject(dcMem.m_hDC, hBmpDst);        // IAT 0x2c64f8
//     BOOL bRes;
//     if (!CImage::InitGDIPlus()) bRes = FALSE;   // 0x876dc: lazy GdiplusStartup({1,0,0,0})
//     else {                                     //   on the refcounted singleton
//         GpBitmap* pBmp = NULL;
//         if (GdipCreateBitmapFromHBITMAP(imageSrc.m_hBitmap, NULL, &pBmp) != Ok) {
//             GdipDisposeImage(pBmp); bRes = FALSE;
//         } else {
//             GpGraphics* pG = NULL;
//             GdipCreateFromHDC(dcMem.m_hDC, &pG);
//             GdipSetInterpolationMode(pG, 7 /*InterpolationModeHighQualityBicubic*/);
//             bRes = GdipDrawImageRectI(pG, pBmp, rectDst.left, rectDst.top,
//                                      rectDst.right - rectDst.left,
//                                      rectDst.bottom - rectDst.top) == Ok;
//             GdipDeleteGraphics(pG); GdipDisposeImage(pBmp);
//         }
//     }
//     if (hOld) ::SelectObject(dcMem.m_hDC, hOld);
//     ~dcMem (DeleteDC), ~dc (ReleaseDC), ~imageSrc (0x8792c: ::DeleteObject(m_hBitmap)
//                                          -- i.e. hBmpSrc is DESTROYED -- then
//                                          s_initGDIPlus refcount--, GdiplusShutdown at 0)
//     return bRes;
// The Gdip* calls are gdiplus.dll delay-load slots 0x3e9478 / 0x3e94a8 /
// 0x3e94a0 / 0x3e9498 / 0x3e9490 / 0x3e9470 (mfc140u), resolved from the
// delay-import directory; the CImage identification rests on the field layout
// (+0x8 HBITMAP, +0x10 pBits, +0x18/+0x1c width/height, +0x20 pitch, +0x24 bpp,
// +0x28 bIsDIBSection, +0x2c transparent colour = -1) and the
// EnterCriticalSection-guarded refcount at singleton+0x30.
// Every step is reproduced with the raw GDI / GDI+ flat API (the gdiplus import
// library is already on the phase4 link; featurepack/visualmanager/CPngImage.cpp
// uses it the same way).  DEVIATIONS: GDI+ is started and shut down around this
// one call instead of through CImage's process-wide refcount (identical when no
// other CImage is alive, which is the retail steady state); GdipDisposeImage is
// not called with a NULL bitmap on the FromHBITMAP failure path (GDI+ would
// just return InvalidParameter); the CClientDC / CDC wrappers are replaced by
// ::GetDC / ::ReleaseDC / ::DeleteDC, and a failed ::GetDC (where CClientDC's
// ctor would throw) returns FALSE after destroying hBmpSrc, as the unwinding
// ~CImage would.  Note the ownership rule: like retail, this function DESTROYS
// hBmpSrc before returning.
// Symbol: ?OnTaskbarTabThumbnailStretch@CMDIChildWndEx@@UEAAHPEAUHBITMAP__@@AEBVCRect@@01@Z
extern "C" int MS_ABI impl__OnTaskbarTabThumbnailStretch_CMDIChildWndEx__UEAAHPEAUHBITMAP____AEBVCRect__01_Z(
    CMDIChildWndEx* pThis, HBITMAP hBmpDst, const CRect& rectDst, HBITMAP hBmpSrc, const CRect& rectSrc) {
    (void)rectSrc;   // retail never reads it
    if (hBmpSrc == nullptr || hBmpDst == nullptr) {
        return FALSE;
    }
    // Retail does not null-check `this` (CClientDC reads m_hWnd); ::GetDC(NULL)
    // would hand back the screen DC, so a NULL frame is treated like a failed
    // CClientDC construction instead.
    const HWND hWnd = pThis ? pThis->m_hWnd : nullptr;
    HDC hdcClient = pThis ? ::GetDC(hWnd) : nullptr;
    if (hdcClient == nullptr) {
        ::DeleteObject(hBmpSrc);   // ~CImage on the unwinding path
        return FALSE;
    }
    HDC hdcMem = ::CreateCompatibleDC(hdcClient);
    HGDIOBJ hOld = hdcMem ? ::SelectObject(hdcMem, hBmpDst) : nullptr;
    BOOL bResult = FALSE;
    GdiplusStartupInput input = { 1, nullptr, FALSE, FALSE };
    ULONG_PTR token = 0;
    if (GdiplusStartup(&token, &input, nullptr) == 0 /*Ok*/) {
        void* pBitmap = nullptr;
        if (GdipCreateBitmapFromHBITMAP(hBmpSrc, nullptr, &pBitmap) != 0 /*Ok*/) {
            if (pBitmap != nullptr) {
                GdipDisposeImage(pBitmap);
            }
            bResult = FALSE;
        } else {
            void* pGraphics = nullptr;
            GdipCreateFromHDC(hdcMem, &pGraphics);
            GdipSetInterpolationMode(pGraphics, 7 /*InterpolationModeHighQualityBicubic*/);
            bResult = GdipDrawImageRectI(pGraphics, pBitmap, rectDst.left, rectDst.top,
                                         rectDst.right - rectDst.left,
                                         rectDst.bottom - rectDst.top) == 0 /*Ok*/
                          ? TRUE
                          : FALSE;
            GdipDeleteGraphics(pGraphics);
            GdipDisposeImage(pBitmap);
        }
        GdiplusShutdown(token);
    }
    if (hOld != nullptr) {
        ::SelectObject(hdcMem, hOld);
    }
    if (hdcMem != nullptr) {
        ::DeleteDC(hdcMem);
    }
    ::ReleaseDC(hWnd, hdcClient);
    ::DeleteObject(hBmpSrc);   // ~CImage: the attached source bitmap is owned and destroyed
    return bResult;
}
// CMDIChildWndEx::OnUpdateFrameTitle(BOOL) — retail (RVA 0x84ad0) consults the
// CWinApp frame state, this+0x248 flag, GetStyle() bit 0x10000 and the
// taskbar-tab text before WM_NCACTIVATE / MDI-client UpdateTabs. The base
// CMDIChildWnd::OnUpdateFrameTitle carries the documented contract; extras are
// skipped.
// Symbol: ?OnUpdateFrameTitle@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__OnUpdateFrameTitle_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bAddToTitle) {
    if (!pThis) return;
    // TODO(clean-room): transcribed partially — retail also handles
    // WM_NCACTIVATE, MDI-client UpdateTabs and taskbar-tab text via unmodeled
    // state.
    impl__OnUpdateFrameTitle_CMDIChildWnd__MEAAXH_Z(pThis, bAddToTitle);
}
// CMDIChildWndEx::OnWindowPosChanged(WINDOWPOS*) — retail (RVA 0x85ec0): when
// lpWndPos->flags has SWP_FRAMECHANGED (0x20) it updates the tabbed-bar layout
// (this+0x218 / this+0x718) and clears the autohide flag at this+0x23c, then
// unconditionally tail-calls CWnd::Default(). The base dispatch is kept.
// Symbol: ?OnWindowPosChanged@CMDIChildWndEx@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMDIChildWndEx__IEAAXPEAUtagWINDOWPOS___Z(
    CMDIChildWndEx* pThis, WINDOWPOS* lpWndPos) {
    if (!pThis) return;
    if (lpWndPos && (lpWndPos->flags & SWP_FRAMECHANGED)) {
        // TODO(clean-room): retail updates the tabbed-bar layout (this+0x218 /
        // this+0x718) and clears the autohide flag at this+0x23c here.
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}
// CMDIChildWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const —
// transcribed from retail mfc140u.dll entry RVA 0x850c0: `this += 0x3a8`, then
// the dock manager's vtable slot 0x28 with (point, nSensitivity, &dwAlignment,
// pRTCBarType, NULL) -- the fifth argument copied from the caller's
// 0x70(%rsp) into the 0x20(%rsp) home slot and a literal 0 in 0x28(%rsp).
// Slot 0x28 of the retail CDockingManager vftable (0x2e6120, mfc140u) is
// ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@PEBV2@@Z
// (0x47cb0), called here as a direct call to that export on the ChildExtra
// dock manager.
// Symbol: ?PaneFromPoint@CMDIChildWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIChildWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const CMDIChildWndEx* pThis, unsigned long long pt, int nSensitivity,
    unsigned long* dwAlignment, CRuntimeClass* pRTCBarType) {
    if (!pThis) return nullptr;
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
        DockManagerOf(const_cast<CMDIChildWndEx*>(pThis)), static_cast<long long>(pt),
        nSensitivity, dwAlignment, pRTCBarType, nullptr);
}
// CMDIChildWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const —
// transcribed from retail mfc140u.dll entry RVA 0x85080: `this += 0x3a8`, then
// the dock manager's vtable slot 0x30 with (point, nSensitivity, bExactBar,
// pRTCBarType, 0, NULL) -- pRTCBarType copied from 0x70(%rsp) into 0x20(%rsp),
// a literal 0 in 0x28(%rsp) and a literal NULL in 0x30(%rsp).  Slot 0x30 is
// ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@HPEBV2@@Z
// (0x47b00), called here as a direct call to that export (declared by
// detail/MfccoreSupport.h with CPoint by value, which is the same 8-byte
// register argument) on the ChildExtra dock manager.
// Symbol: ?PaneFromPoint@CMDIChildWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CMDIChildWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const CMDIChildWndEx* pThis, unsigned long long pt, int nSensitivity,
    bool bExactBar, CRuntimeClass* pRTCBarType) {
    if (!pThis) return nullptr;
    CPoint point(static_cast<int>(pt & 0xffffffffu), static_cast<int>(pt >> 32));
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        DockManagerOf(const_cast<CMDIChildWndEx*>(pThis)), point, nSensitivity,
        bExactBar ? 1 : 0, pRTCBarType, 0, nullptr);
}
// CMDIChildWndEx::PreCreateWindow(CREATESTRUCTW&) — transcribed from retail
// mfc140u.dll entry RVA 0x84e80, whose whole body is:
//     mov %rcx,0x558(%rcx)      ; m_dockManager.m_pParentWnd = this  (+0x3a8 + 0x1b0)
//     jmp 0x29d8d0              ; ?PreCreateWindow@CFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z
// The jump target is CFrameWnd's export, not CMDIChildWnd's --
// ?PreCreateWindow@CMDIChildWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z (0x2a6f00) is
// itself a bare `jmp 0x29d8d0`, so the CMDIChildWnd thunk used here reaches
// the same body.  The parent-pointer store goes to the ChildExtra dock manager
// (OpenMFC's CDockingManager keeps that pointer in m_pParentWnd).
// Symbol: ?PreCreateWindow@CMDIChildWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMDIChildWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(
    CMDIChildWndEx* pThis, CREATESTRUCTW& cs) {
    if (!pThis) return FALSE;
    SetDockManagerParent(DockManagerOf(pThis), pThis);
    return impl__PreCreateWindow_CMDIChildWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, cs);
}
// CMDIChildWndEx::PreTranslateMessage(MSG*) — retail (RVA 0x83990) has keyboard
// fast paths (0x100..0x109 suppression when a tabbed bar is present, and a
// WM_KEYDOWN+VK_ESCAPE close path through this+0x718 (the frame's
// unconstructed m_wndClientArea behind it), this+0x1e8 and a global flag)
// before falling back to CMDIChildWnd::PreTranslateMessage. The base dispatch
// is kept.
// Symbol: ?PreTranslateMessage@CMDIChildWndEx@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMDIChildWndEx__UEAAHPEAUtagMSG___Z(
    CMDIChildWndEx* pThis, MSG* pMsg) {
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially — retail keyboard fast paths
    // rely on the frame's unconstructed m_wndClientArea (via this+0x718),
    // this+0x1e8 and a global flag.
    return impl__PreTranslateMessage_CMDIChildWnd__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
// CMDIChildWndEx::RecalcLayout(BOOL) — retail (RVA 0x852b0) drives the embedded
// CDockingManager (this+0x3a8), the MDI client (this+0x558) and the internal
// AdjustClientArea (0x180085140) behind reentrancy flags (this+0x1a0/0x1f8).
// The base frame relayout is performed; the unmodeled dock/client state is
// skipped.
// Symbol: ?RecalcLayout@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int bNotify) {
    if (!pThis) return;
    pThis->CFrameWnd::RecalcLayout(bNotify);
    // TODO(clean-room): transcribed partially — retail also runs the embedded
    // CDockingManager (this+0x3a8) layout and AdjustClientArea behind the
    // reentrancy flags at this+0x1a0/0x1f8.
}
// CMDIChildWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL,
// CBasePane*) — transcribed from retail mfc140u.dll entry RVA 0x84ec0, a
// two-instruction tail jump: `this += 0x3a8` then
// ?RemovePaneFromDockManager@CDockingManager@@QEAAXPEAVCBasePane@@HHH0@Z
// (0x479c0), all five arguments unchanged.  Forwarded to the ChildExtra dock
// manager.
// Symbol: ?RemovePaneFromDockManager@CMDIChildWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CMDIChildWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, int bDestroy,
    int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement) {
    if (!pThis) return;
    impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
        DockManagerOf(pThis), pControlBar, bDestroy, bAdjustLayout, bAutoHide, pBarReplacement);
}
// CMDIChildWndEx::ShowPane(CBasePane*, BOOL, BOOL, BOOL) — retail (RVA 0x68050)
// dispatches a CBasePane virtual (vtable 0x458) to show/hide the pane. The
// retail pane vtable is not modeled. Conservative no-op.
// Symbol: ?ShowPane@CMDIChildWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_CMDIChildWndEx__QEAAXPEAVCBasePane__HHH_Z(
    CMDIChildWndEx* pThis, CBasePane* pBar, int bShow, int bDelay, int bActivate) {
    (void)pBar;
    (void)bShow;
    (void)bDelay;
    (void)bActivate;
    if (!pThis) return;
    // TODO(clean-room): retail dispatches a CBasePane virtual (vtable 0x458);
    // the retail pane vtable is not modeled.
}
// CMDIChildWndEx::UpdateTaskbarTabIcon(HICON) — retail (RVA 0x841e0) posts
// WM_SETICON (0x80) to the taskbar-tab proxy window (this+0x720/+0x760). The
// proxy window is not modeled. Conservative no-op.
// Symbol: ?UpdateTaskbarTabIcon@CMDIChildWndEx@@UEAAXPEAUHICON__@@@Z
extern "C" void MS_ABI impl__UpdateTaskbarTabIcon_CMDIChildWndEx__UEAAXPEAUHICON_____Z(
    CMDIChildWndEx* pThis, HICON hIcon) {
    (void)hIcon;
    if (!pThis) return;
    // TODO(clean-room): retail sends WM_SETICON to the tab-proxy window
    // (this+0x720/+0x760); not modeled in the simplified headers.
}
// The C++ constructor behind ??0CMDIChildWndEx@@IEAA@XZ (core/frame/Thunks.cpp
// placement-news it) and CreateObject.  Retail (entry RVA 0x83820, mfc140u)
// runs the CMDIChildWnd ctor (0x2a6d00), stores the vftable (0x2ee018), zeroes
// +0x1fc..+0x20b, constructs m_Impl(this) at +0x218 (0x61ef0), m_dockManager at
// +0x3a8 (default ctor 0x46ef0) and m_wndTaskbarTabProxy at +0x720 (0x86bb0),
// zeroes +0x718, +0x1f0 (8 bytes) and +0x1f8, ::SetRectEmpty(+0x1fc), zeroes
// +0x20c (8 bytes), +0x1e8, +0x710 and +0x810, and sets +0x814 = 1.
// DEVIATIONS: the zero-fill covers the whole 64-byte padding (a superset of the
// retail scalars that fall inside it); m_Impl / m_dockManager live in
// ChildExtra (see AttachExtra); the tab proxy is not constructed and the
// +0x710 / +0x810 / +0x814 slots have no home.
CMDIChildWndEx::CMDIChildWndEx() {
    memset(_mdichildwndex_padding, 0, sizeof(_mdichildwndex_padding));
    AttachExtra(this);
}
// Retail (entry RVA 0x83930, mfc140u): stores the vftable, runs ~CWnd (0x28b740)
// on m_wndTaskbarTabProxy (this+0x720), ~CDockingManager (0x472a0) on
// this+0x3a8, ~CFrameImpl (0x620b0) on this+0x218, then tail-jumps to
// ~CFrameWnd (0x29cc60).  The two sub-objects this file constructs are torn
// down in that order by DetachExtra; the base destructors run implicitly.
CMDIChildWndEx::~CMDIChildWndEx() {
    DetachExtra(this);
}
// CMDIChildWndEx::ActivateFrame(int) — transcribed from retail mfc140u.dll
// RVA 0x84650:
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (CMDIFrameWndEx::m_bDisableSetRedraw == 0 &&
//         pParent != NULL && pParent->m_hWnd != NULL) {
//         ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, FALSE, 0);
//         CMDIChildWnd::ActivateFrame(nCmdShow);
//         ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, TRUE, 0);
//         ::RedrawWindow(pParent->m_hWnd, NULL, NULL, 0x185);
//     } else {
//         int nCmd = (GetStyle() & WS_SYSMENU) ? nCmdShow : SW_SHOWMAXIMIZED;
//         if (this->[0x718] && ((char*)this->[0x718])[0x4668] != 0)
//             nCmd = SW_SHOWNORMAL;
//         CMDIChildWnd::ActivateFrame(nCmd);
//     }
// 0x185 == RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_NOCHILDREN
// (0x1|0x4|0x80|0x100); the style test is `bt $0x13` on GetStyle()'s result,
// i.e. bit 19 == WS_SYSMENU (0x00080000).  The gate is the exported static
// ?m_bDisableSetRedraw@CMDIFrameWndEx@@2HA (mfc140u data RVA 0x3b1afc, which
// sits directly after m_dwExcludeStyle/m_dwDefaultTaskbarTabPropertyFlags in
// both twins); retail initialises it to 1, so the SetRedraw-bracketed arm is
// NOT the default path in a real mfc140u.  OpenMFC's copy in
// core/frame/StaticData.cpp is currently initialised to 0 -- see the
// headerRequest filed with this file.  m_pMDIFrame (this+0x718) is in
// ChildExtra, but the frame's m_wndClientArea.m_bIsMDITabbedGroup
// (frame+0x4668) is unreadable in this tree (that sub-object is not
// constructed), so the SW_SHOWNORMAL override is skipped; everything else is
// transcribed.
// Symbol: ?ActivateFrame@CMDIChildWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(
    CMDIChildWndEx* pThis, int nCmdShow) {
    if (!pThis) {
        return;
    }
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
        ::GetParent(pThis->m_hWnd));
    if (impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA == 0 &&
        pParent != nullptr && pParent->m_hWnd != nullptr) {
        ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, FALSE, 0);
        impl__ActivateFrame_CMDIChildWnd__UEAAXH_Z(pThis, nCmdShow);
        ::SendMessage(pParent->m_hWnd, WM_SETREDRAW, TRUE, 0);
        ::RedrawWindow(pParent->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_NOCHILDREN);
    } else {
        const int nCmd =
            (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_SYSMENU) ? nCmdShow
                                                               : SW_SHOWMAXIMIZED;
        // TODO(clean-room): retail forces SW_SHOWNORMAL when the owning
        // frame's m_wndClientArea.m_bIsMDITabbedGroup (frame+0x4668) is set;
        // that frame sub-object is not constructed in this tree.
        impl__ActivateFrame_CMDIChildWnd__UEAAXH_Z(pThis, nCmd);
    }
}

// CMDIChildWndEx::ActivateTopLevelFrame() — transcribed from retail mfc140u.dll
// RVA 0x86a10, whose whole body is:
//     CFrameWnd* pFrame = GetTopLevelFrame();                  // 0x28e490
//     if (pFrame == NULL) return;
//     if (!pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return;
//     this->vtbl[0x308](-1);                                   // ActivateFrame(-1)
//     ::SetForegroundWindow(pFrame->m_hWnd);
//     BOOL bIconic = ::IsIconic(pFrame->m_hWnd);
//     pFrame->ShowWindow(bIconic ? SW_RESTORE : SW_SHOW);      // 9 : 5
//     ::PostMessageW(pFrame->m_hWnd, AFX_WM_AFTER_TASKBAR_ACTIVATE,
//                    bIconic, (LPARAM)this->m_hWnd);
//     bIconic = ::IsIconic(pFrame->m_hWnd);                    // re-read
//     pFrame->ShowWindow(bIconic ? SW_RESTORE : SW_SHOW);
// The 9/5 pair is built branchlessly as `neg; sbb edx,edx; and $4; add $5` the
// first time and as an explicit `mov $9 / mov $5` the second; the CRuntimeClass
// at mfc140u 0x2ee478 spells "CMDIFrameWndEx", and the message id is read from
// ?AFX_WM_AFTER_TASKBAR_ACTIVATE@@3IA (mfc140u data 0x3c18f4).
// Deviation: retail dispatches ActivateFrame through vtable slot 0x308, this
// build calls this class's own export directly, so an override in a
// further-derived class is not honoured.
// Symbol: ?ActivateTopLevelFrame@CMDIChildWndEx@@MEAAXXZ
extern "C" void MS_ABI impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrame == nullptr) {
        return;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pFrame, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    impl__ActivateFrame_CMDIChildWndEx__UEAAXH_Z(pThis, -1);
    ::SetForegroundWindow(pFrame->m_hWnd);
    int bIconic = ::IsIconic(pFrame->m_hWnd) ? 1 : 0;
    impl__ShowWindow_CWnd__QEAAHH_Z(pFrame, bIconic ? SW_RESTORE : SW_SHOW);
    ::PostMessage(pFrame->m_hWnd, impl__AFX_WM_AFTER_TASKBAR_ACTIVATE__3IA,
                  static_cast<WPARAM>(bIconic),
                  reinterpret_cast<LPARAM>(pThis->m_hWnd));
    bIconic = ::IsIconic(pFrame->m_hWnd) ? 1 : 0;
    impl__ShowWindow_CWnd__QEAAHH_Z(pFrame, bIconic ? SW_RESTORE : SW_SHOW);
}

// CMDIChildWndEx::AddDockSite() — transcribed from retail mfc140u.dll
// RVA 0x27d0, whose entire body is a single `ret $0x0`.  It reads no member,
// calls nothing and returns nothing, so an empty body is the faithful
// implementation here -- not a placeholder.
// Two caveats for whoever re-checks this: (1) the export has no entry in
// EITHER rva-symbol map (mfc140_rva_symbols.json nor mfc140u_rva_symbols.json),
// so `disas.py --u '<name>'` reports NOT FOUND; resolve it through the export
// directory instead (ordinal 1838 -> 0x27d0 in mfc140u, ures.py); (2) 0x27d0
// is an identical-COMDAT-folded address shared by 158 exports, so a
// disassembler that labels RVAs from a reverse map will print some other name
// (e.g. ?AddDockSite@CFrameWndEx@@QEAAXXZ) at it.  The folding is itself the
// evidence that this body is a bare `ret`.
// Symbol: ?AddDockSite@CMDIChildWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_CMDIChildWndEx__QEAAXXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
}

// CMDIChildWndEx::AddPane(CBasePane*, BOOL) — transcribed from retail
// mfc140u.dll entry RVA 0x84e90, a forwarder: `this += 0x3a8` then a (non-tail)
// call to ?AddPane@CDockingManager@@QEAAHPEAVCBasePane@@HHH@Z (mfc140u 0x47860)
// with (pControlBar, bTail, 0, 0) -- the third argument in R9D and the fourth
// in the 0x20(%rsp) home slot -- whose BOOL result is returned.  Forwarded to
// the ChildExtra dock manager (see the layout note above).
// Symbol: ?AddPane@CMDIChildWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_CMDIChildWndEx__QEAAHPEAVCBasePane__H_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, int bTail) {
    if (!pThis) {
        return FALSE;
    }
    return impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
        DockManagerOf(pThis), pControlBar, bTail, FALSE, FALSE);
}

// CMDIChildWndEx::AddTabbedPane(CDockablePane*) — retail mfc140u.dll entry
// RVA 0x86540:
//     m_pTabbedControlBar = pTabbedPane;                          // this+0x710
//     pTabbedPane->vtbl[0x448](this, FALSE);                      // CDockablePane::OnBeforeChangeParent
//     pTabbedPane->vtbl[0x3f0](FALSE);                            // CDockablePane::EnableGripper
//     CWnd::FromHandle(::SetParent(pTabbedPane->m_hWnd, m_hWnd)); // 0x28ad70; IAT 0x2c7288
//     pTabbedPane->ShowWindow(SW_SHOW);                           // 0x2a9ad0
//     pTabbedPane->m_bIsMDITabbed = TRUE;                         // pane+0xf8
//     m_pMDIFrame->m_dockManager.RemoveHiddenMDITabbedBar(pTabbedPane);   // 0x4ebf0 on
//                                                                 // (this+0x718)+0x228, NO null test
//     AdjustClientArea();                                         // tail jump 0x85140
// (slots 0x448 / 0x3f0 of the retail CDockablePane vftable are
// ?OnBeforeChangeParent@CDockablePane@@UEAAXPEAVCWnd@@H@Z and
// ?EnableGripper@CDockablePane@@UEAAXH@Z, per the vftable dump this campaign
// keeps; m_bIsMDITabbed is a public member of OpenMFC's CBasePane at the same
// +0xf8.)  Two things stop this from being reproduced honestly: the owning
// frame's m_dockManager (frame+0x228) is reachable only through a helper local
// to core/frame/CMDIFrameWndEx.cpp, and OnDestroy's counterpart -- retail
// reparents the tabbed pane back to the frame's dock site before the child
// window dies (0x85744..0x8581b inside OnDestroy, entry 0x856f0) -- is not
// implemented in this file.  Reparenting an application's CDockablePane into a
// child whose teardown never hands it back would destroy the pane's HWND with
// the child, a worse outcome than today's no-op.
// STUB -- left a no-op deliberately; see the headerRequest filed with this file.
// Symbol: ?AddTabbedPane@CMDIChildWndEx@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddTabbedPane_CMDIChildWndEx__QEAAXPEAVCDockablePane___Z(
    CMDIChildWndEx* pThis, CDockablePane* pPane) {
    (void)pThis;
    (void)pPane;
}

// CMDIChildWndEx::AdjustClientArea() — partially transcribed from retail
// mfc140u.dll RVA 0x85140, whose whole body is:
//     CWnd* pWnd = NULL;
//     if (this->[0x710] && ((CWnd*)this->[0x710])->vtbl[0x2f8]() &&
//         CWnd::FromHandle(::GetParent(this->[0x710]->m_hWnd)) == this)
//         pWnd = this->[0x710];                       // the tabbed pane
//     else
//         pWnd = CWnd::GetDlgItem(AFX_IDW_PANE_FIRST);  // 0x2a9390
//     if (pWnd == NULL) return;
//     DWORD dwAdd = (pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)) ||   // 0x332860
//                    pWnd->IsKindOf(RUNTIME_CLASS(CFormView)))        // 0x333410
//                   ? 0x04000000                       // WS_CLIPSIBLINGS
//                   : 0x06000000;                      // + WS_CLIPCHILDREN
//     pWnd->ModifyStyle(0, dwAdd, 0);                  // 0x2a96f0
//     if (this->[0x210]) return;
//     if (CDockingManager::m_bFullScreenMode) return;  // mfc140u data 0x3be204,
//                                                      // = mfc140.dll 0x3b6f64
//     pWnd->SetWindowPos(&CWnd::wndBottom,             // 0x2a9a60; the RDX
//                                                      // operand is the global
//                                                      // ?wndBottom@CWnd@@2V1@B
//                                                      // at mfc140u 0x3c3460
//         this->[0x110] + this->[0x570], this->[0x114] + this->[0x574],
//         this->[0x578] - this->[0x118] - (this->[0x110] + this->[0x570]),
//         this->[0x57c] - this->[0x11c] - (this->[0x114] + this->[0x574]),
//         SWP_NOZORDER /*0x10*/);   // the z-order arg is inert under NOZORDER
// The tabbed-pane slot at this+0x710 is not modeled by OpenMFC (CMDIChildWndEx
// is a 64-byte padding blob), so the GetDlgItem arm is the one retail takes
// here -- and it, the two IsKindOf tests and the ModifyStyle call need nothing
// this build lacks, so that whole leg is reproduced faithfully.
// Only the trailing geometry pass is skipped: it needs this+0x110..0x11c
// (the cached frame borders), this+0x210 and this+0x570..0x57c, none of which
// have slots here, and CDockingManager::m_bFullScreenMode is not modeled
// either.  The consequence is that the client pane still gets its clip styles
// but is never re-fitted to the docking borders.
// Symbol: ?AdjustClientArea@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail prefers the tabbed pane cached at this+0x710
    // when it accepts the client area (its vtable slot 0x2f8) and this frame is
    // its parent; that slot is unmodeled, so retail's other arm is taken.
    CWnd* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, AFX_IDW_PANE_FIRST);
    if (pWnd == nullptr) {
        return;
    }
    const unsigned long dwAdd =
        (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
             pWnd, impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ()) ||
         impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
             pWnd, impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ()))
            ? static_cast<unsigned long>(WS_CLIPSIBLINGS)
            : static_cast<unsigned long>(WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(pWnd, 0, dwAdd, 0);
    // TODO(clean-room): retail then re-fits the pane with CWnd::SetWindowPos
    // using this+0x110..0x11c and this+0x570..0x57c behind the this+0x210 and
    // CDockingManager::m_bFullScreenMode gates; none of those are modeled.
}

// CMDIChildWndEx::AdjustDockingLayout(HDWP) — transcribed from retail
// mfc140u.dll RVA 0x85100:
//     CDockingManager& dm = *(this + 0x3a8);
//     if (dm.[0x250] != 0) return;      // "layout in progress" latch
//     dm.vtbl[0x80](hdwp);              // ?AdjustDockingLayout@CDockingManager@@UEAAXPEAX@Z
//                                       // (slot 0x80 of the retail CDockingManager vftable
//                                       // 0x2e6120, mfc140u); RDX is untouched from entry
//     AdjustClientArea();               // 0x85140
// The dock manager is the ChildExtra one; its relayout is reproduced as a
// direct call to the CDockingManager export.  NOT reproduced: the +0x250 latch
// (OpenMFC's 144-byte CDockingManager has no such member) -- so a re-entrant
// call relayouts again where retail returns.  The AdjustClientArea() leg is
// reproduced (see that function below -- it applies the client pane's clip
// styles but not retail's trailing geometry pass).
// Symbol: ?AdjustDockingLayout@CMDIChildWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(
    CMDIChildWndEx* pThis, void* pLayout) {
    if (!pThis) {
        return;
    }
    impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(DockManagerOf(pThis), pLayout);
    impl__AdjustClientArea_CMDIChildWndEx__IEAAXXZ(pThis);
}

// CMDIChildWndEx::DockPane(CBasePane*, UINT, LPCRECT) — transcribed from retail
// mfc140u.dll entry RVA 0x84ed0 (lpRect in R9 is never saved and never used):
//     if (pBar->vtbl[0x3a8]() /*CanFloat*/ && !m_bEnableFloatingBars /*data 0x3be24c*/)
//         pBar->m_dwControlBarStyle &= ~1 /*AFX_CBRS_FLOAT*/;     // pane+0x108
//     if (pBar->vtbl[0x3b0]() || pBar->vtbl[0x3a8]()) {
//         pBar->vtbl[0x3e8](0xf000 /*CBRS_ALIGN_ANY*/);
//         m_dockManager.DockPane(pBar, dwAlignment, NULL);        // 0x48570 on this+0x3a8
//     } else {
//         m_dockManager.AddPane(pBar, TRUE, FALSE, FALSE);        // 0x47860 on this+0x3a8;
//     }                                                           // R8D = 1, R9D = 0, [rsp+0x20] = 0
// Slot 0x3a8 of the retail CBasePane vftable is ?CanFloat@CBasePane@@UEBAHXZ
// (dispatched through its thunk here).  Slots 0x3b0 and 0x3e8 are unexported
// inline virtuals; their bodies (mfc140.dll 0x8870 / 0x88d0, byte-identical in
// mfc140u) are `mov 0x108(%rcx),%eax; and $0x4,%eax; ret` -- i.e.
// `m_dwControlBarStyle & AFX_CBRS_RESIZE`, the shipping afxbasepane.h's inline
// CanBeResized() -- and `mov %edx,0x100(%rcx); ret` -- i.e.
// `m_dwEnabledAlignment = dwAlignment`, its inline EnableDocking(DWORD).  Both
// members are public on OpenMFC's CBasePane at the same offsets (pinned by the
// static_asserts near the top), so the two slots are reproduced as the direct
// member accesses their bodies are; an override of either in a further-derived
// pane is not honoured.  The dock manager is the ChildExtra one.
// Symbol: ?DockPane@CMDIChildWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_CMDIChildWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CMDIChildWndEx* pThis, CBasePane* pBar, unsigned int dwAlignment, const RECT* lpRect) {
    (void)lpRect;   // retail never reads it: the dock manager gets NULL
    if (!pThis || !pBar) {   // retail has no null test on pBar (it would fault)
        return;
    }
    CDockingManager* pDock = DockManagerOf(pThis);
    if (impl__CanFloat_CBasePane__UEBAHXZ(pBar) &&
        impl__m_bEnableFloatingBars_CMDIChildWndEx__2HA == 0) {
        pBar->m_dwControlBarStyle &= ~static_cast<DWORD>(0x1);   // AFX_CBRS_FLOAT
    }
    const bool bCanBeResized = (pBar->m_dwControlBarStyle & 0x4) != 0;   // AFX_CBRS_RESIZE
    if (bCanBeResized || impl__CanFloat_CBasePane__UEBAHXZ(pBar)) {
        pBar->m_dwEnabledAlignment = 0xF000;   // CBRS_ALIGN_ANY (retail's literal;
                                               // afxole.h's macro of that name is 0x000F)
        impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
            pDock, pBar, dwAlignment, nullptr);
    } else {
        impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(pDock, pBar, TRUE, FALSE, FALSE);
    }
}

// CMDIChildWndEx::DockPaneLeftOf(CPane*, CPane*) — transcribed from retail
// mfc140u.dll entry RVA 0x843a0: `this += 0x3a8;
// ?DockPaneLeftOf@CDockingManager@@QEAAHPEAVCPane@@0@Z (0x48430); mov $1,%eax`
// -- the manager's result is discarded and TRUE is returned unconditionally.
// Forwarded to the ChildExtra dock manager, constant result kept.
// Symbol: ?DockPaneLeftOf@CMDIChildWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_CMDIChildWndEx__QEAAHPEAVCPane__0_Z(
    CMDIChildWndEx* pThis, CPane* pBar, CPane* pLeftOf) {
    if (!pThis) {
        return FALSE;
    }
    impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(DockManagerOf(pThis), pBar, pLeftOf);
    return TRUE;
}

// CMDIChildWndEx::EnableAutoHidePanes(DWORD) — transcribed from retail
// mfc140u.dll entry RVA 0x852a0, a two-instruction tail jump: `this += 0x3a8`
// then ?EnableAutoHidePanes@CDockingManager@@QEAAHK@Z (0x47500), whose BOOL
// result is returned directly.  Forwarded to the ChildExtra dock manager.
// Symbol: ?EnableAutoHidePanes@CMDIChildWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_CMDIChildWndEx__QEAAHK_Z(
    CMDIChildWndEx* pThis, unsigned long dwDockStyle) {
    if (!pThis) {
        return FALSE;
    }
    return impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(DockManagerOf(pThis), dwDockStyle);
}

// CMDIChildWndEx::EnableTaskbarThumbnailClipRect(BOOL) — transcribed from
// retail mfc140u.dll entry RVA 0x86810:
//     m_bEnableTaskbarThumbnailClip = bEnable;                    // this+0x814
//     if (m_hWnd == NULL) return;
//     CRect rect(0, 0, 0, 0);
//     if (bEnable) rect = this->vtbl[0x438]();                    // GetTaskbarThumbnailClipRect
//     this->vtbl[0x448](rect);                                    // SetTaskbarThumbnailClipRect
// Reproduced: the m_hWnd gate and both calls, as direct calls to this class's
// own exports (no MSVC vtable here, so an override in a further-derived class
// is not honoured).  NOT reproduced: the store to m_bEnableTaskbarThumbnailClip
// -- OpenMFC's CMDIChildWndEx is 0x228 bytes and has no slot for the retail
// +0x814 flag, so the shipping header's inline
// IsTaskbarThumbnailClipRectEnabled() has nothing to read back here.
// Symbol: ?EnableTaskbarThumbnailClipRect@CMDIChildWndEx@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTaskbarThumbnailClipRect_CMDIChildWndEx__QEAAXH_Z(
    CMDIChildWndEx* pThis, int bEnable) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): m_bEnableTaskbarThumbnailClip (retail this+0x814) has
    // no slot in OpenMFC's CMDIChildWndEx; the store is skipped.
    if (pThis->m_hWnd == nullptr) {
        return;
    }
    CRect rect(0, 0, 0, 0);
    if (bEnable) {
        impl__GetTaskbarThumbnailClipRect_CMDIChildWndEx__UEBA_AVCRect__XZ(pThis, &rect);
    }
    impl__SetTaskbarThumbnailClipRect_CMDIChildWndEx__MEAAHVCRect___Z(pThis, rect);
}

// CMDIChildWndEx::GetDocumentName(CObject**) — transcribed from retail
// mfc140u.dll RVA 0x85e90, whose whole body is:
//     CDocument* pDoc = GetActiveDocument();     // vtable slot +0x2e8
//     if (pDoc == NULL) return NULL;
//     return *(LPCTSTR*)((char*)pDoc + 0x48);    // pDoc->m_strPathName
// Note that retail never writes through pObj -- RDX is untouched from entry to
// ret -- so this build does not touch it either.  CDocument+0x48 is
// m_strPathName: ?SetPathName@CDocument@@UEAAXPEBDH@Z (mfc140.dll 0x2194c0)
// stores the path there, clears m_bEmbedded at +0x124 right after, and passes
// the same +0x48 pointer to a CWinApp virtual (slot 0x160) when bAddToMRU is
// set.  Slot +0x2e8 of the retail CMDIChildWndEx vtable
// (mfc140.dll 0x2ebf68) is ?GetActiveDocument@CFrameWnd@@UEAAPEAVCDocument@@XZ,
// so calling the CFrameWnd export here matches -- but unlike retail this is a
// direct call, so an override in a further-derived class is not honoured.
// Symbol: ?GetDocumentName@CMDIChildWndEx@@UEAAPEB_WPEAPEAVCObject@@@Z
extern "C" const wchar_t* MS_ABI impl__GetDocumentName_CMDIChildWndEx__UEAAPEB_WPEAPEAVCObject___Z(
    CMDIChildWndEx* pThis, CObject** pObj) {
    (void)pObj;  // retail leaves the out-parameter alone
    if (!pThis) {
        return nullptr;
    }
    CDocument* pDoc = impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(pThis);
    if (!pDoc) {
        return nullptr;
    }
    return impl__GetPathName_CDocument__UEBAPEB_WXZ(pDoc);
}

// CMDIChildWndEx::GetFrameIcon() const — transcribed from retail mfc140u.dll
// RVA 0x84a90, whose whole body is:
//     HICON h = (HICON)::SendMessageW(m_hWnd, WM_GETICON /*0x7f*/, 0, 0);
//     if (h == NULL) h = (HICON)::GetClassLongPtrW(m_hWnd, -34 /*GCLP_HICONSM*/);
//     return h;
// (0x7f is materialised as `lea 0x7f(%r9)` with r9 == 0, and -34 as
// `lea -0x22(%rax)` with rax == 0 on that path; the two call slots resolve to
// USER32!SendMessageW and USER32!GetClassLongPtrW.)  Retail does not null-check
// m_hWnd; both APIs simply fail on a NULL window and the function returns NULL.
// Symbol: ?GetFrameIcon@CMDIChildWndEx@@UEBAPEAUHICON__@@XZ
extern "C" HICON MS_ABI impl__GetFrameIcon_CMDIChildWndEx__UEBAPEAUHICON____XZ(
    const CMDIChildWndEx* pThis) {
    if (!pThis) {
        return nullptr;
    }
    const HWND hWnd = pThis->m_hWnd;
    HICON hIcon = reinterpret_cast<HICON>(::SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0));
    if (hIcon == nullptr) {
        hIcon = reinterpret_cast<HICON>(::GetClassLongPtr(hWnd, GCLP_HICONSM));
    }
    return hIcon;
}

// CMDIChildWndEx::GetFrameText() const — transcribed from retail mfc140u.dll
// RVA 0x84a30, whose whole body is:
//     CString str;                          // nil-string data from the string
//                                           // manager at mfc140u 0x3b25e8,
//                                           // vtable slot 3 + 0x18
//     CWnd::GetWindowTextW(str);            // 0x28be00, direct (non-virtual) call
//     return str;
// The CString comes back through the hidden return slot: retail takes `this` in
// RCX and the return buffer in RDX (`mov %rdx,%rdi ... mov %rax,(%rdi)` writes
// the string data pointer into the buffer) and returns that pointer in RAX,
// which is the parameter order used below.
// Symbol: ?GetFrameText@CMDIChildWndEx@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" CString* MS_ABI impl__GetFrameText_CMDIChildWndEx__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CMDIChildWndEx* pThis, CString* pRet) {
    if (!pRet) {
        return pRet;
    }
    new (pRet) CString();
    if (pThis) {
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pThis, pRet);
    }
    return pRet;
}

// CMDIChildWndEx::GetPane(UINT) — transcribed from retail mfc140u.dll entry
// RVA 0x85060, a tail dispatch: `this += 0x3a8` then the dock manager's vtable
// slot 0x58 with (nID, TRUE).  Slot 0x58 of the retail CDockingManager vftable
// (0x2e6120, mfc140u) is ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z
// (0x47fd0), called here as a direct call to that export on the ChildExtra
// dock manager (an override in a derived dock manager is not honoured).
// Symbol: ?GetPane@CMDIChildWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" CBasePane* MS_ABI impl__GetPane_CMDIChildWndEx__QEAAPEAVCBasePane__I_Z(
    CMDIChildWndEx* pThis, unsigned int nID) {
    if (!pThis) {
        return nullptr;
    }
    return impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(DockManagerOf(pThis), nID, TRUE);
}

// CMDIChildWndEx::GetTaskbarPreviewWnd() — transcribed from retail mfc140u.dll
// RVA 0x86650, whose whole body is:
//     CWnd* pWnd = CWnd::GetDescendantWindow(m_hWnd, AFX_IDW_PANE_FIRST, FALSE);
//     if (pWnd != NULL && pWnd->m_hWnd != NULL) {
//         CWnd* pParent = CWnd::FromHandle(::GetParent(pWnd->m_hWnd));
//         if (pParent != this && pParent != NULL && pParent->m_hWnd != NULL &&
//             pParent->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
//             pWnd = pParent;
//     } else {
//         pWnd = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD));
//     }
//     return pWnd;
// Note the shape of the two guards: the `je` at 0x86679 and the `je` at 0x86682
// (instructions inside GetTaskbarPreviewWnd, entry 0x86650) BOTH branch to the
// same GW_CHILD block at 0x866be -- a descendant that exists but carries no HWND
// takes the fallback exactly like a missing descendant does.  (An earlier
// version of this file nested the m_hWnd test inside the non-NULL arm and
// returned the handle-less CWnd instead; that was wrong.)
// 0xe900 is AFX_IDW_PANE_FIRST, the third GetDescendantWindow argument is 0
// (bOnlyPerm == FALSE), GW_CHILD is the literal 5, and the CRuntimeClass at
// mfc140u 0x332860 spells "CSplitterWnd" in its m_lpszClassName field.
// Symbol: ?GetTaskbarPreviewWnd@CMDIChildWndEx@@UEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetTaskbarPreviewWnd_CMDIChildWndEx__UEAAPEAVCWnd__XZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return nullptr;
    }
    CWnd* pWnd = impl__GetDescendantWindow_CWnd__SAPEAV1_PEAUHWND____HH_Z(
        pThis->m_hWnd, AFX_IDW_PANE_FIRST, FALSE);
    if (pWnd != nullptr && pWnd->m_hWnd != nullptr) {
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetParent(pWnd->m_hWnd));
        if (pParent != static_cast<CWnd*>(pThis) && pParent != nullptr &&
            pParent->m_hWnd != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pParent, impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ())) {
            pWnd = pParent;
        }
    } else {
        pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetWindow(pThis->m_hWnd, GW_CHILD));
    }
    return pWnd;
}

// CMDIChildWndEx::GetTaskbarThumbnailClipRect() const — transcribed from retail
// mfc140u.dll RVA 0x868b0, whose whole body is:
//     CRect rect;                       // both halves zeroed first
//     ::GetWindowRect(m_hWnd, &rect);   // USER32, into the return buffer
//     return rect;
// `this` arrives in RCX and the CRect return slot in RDX (retail zeroes (%rdx)
// and 0x8(%rdx), passes RDX straight to ::GetWindowRect and returns it in RAX),
// which is the parameter order used below.  Retail does not null-check m_hWnd;
// ::GetWindowRect then fails and the caller sees the zeroed rect.
// Symbol: ?GetTaskbarThumbnailClipRect@CMDIChildWndEx@@UEBA?AVCRect@@XZ
extern "C" CRect* MS_ABI impl__GetTaskbarThumbnailClipRect_CMDIChildWndEx__UEBA_AVCRect__XZ(
    const CMDIChildWndEx* pThis, CRect* pRet) {
    if (!pRet) {
        return pRet;
    }
    pRet->left = 0;
    pRet->top = 0;
    pRet->right = 0;
    pRet->bottom = 0;
    if (pThis) {
        // OpenMFC's CRect is not derived from RECT, but is layout-compatible
        // with it (four ints, same order), which is what retail relies on.
        ::GetWindowRect(pThis->m_hWnd, reinterpret_cast<LPRECT>(pRet));
    }
    return pRet;
}

// CMDIChildWndEx::InsertPane(CBasePane*, CBasePane*, BOOL) — transcribed from
// retail mfc140u.dll entry RVA 0x84eb0, a two-instruction tail jump:
// `this += 0x3a8` then ?InsertPane@CDockingManager@@QEAAHPEAVCBasePane@@0H@Z
// (0x47950) with (pControlBar, pTarget, bAfter), whose BOOL result is returned
// directly.  Forwarded to the ChildExtra dock manager.
// Symbol: ?InsertPane@CMDIChildWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CMDIChildWndEx__QEAAHPEAVCBasePane__0H_Z(
    CMDIChildWndEx* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter) {
    if (!pThis) {
        return FALSE;
    }
    return impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
        DockManagerOf(pThis), pControlBar, pTarget, bAfter);
}

// CMDIChildWndEx::InvalidateIconicBitmaps() — retail mfc140u.dll entry RVA
// 0x840b0:
//     if (!IsTaskbarTabsSupportEnabled()) return FALSE;           // 0x84000
//     if (m_tabProxyWnd.m_hWnd == NULL) return FALSE;             // this+0x720 +0x40
//     CRect rect(0, 0, 0, 0);
//     if (m_bEnableTaskbarThumbnailClip) rect = this->vtbl[0x438]();   // +0x814;
//                                                                 // GetTaskbarThumbnailClipRect
//     this->vtbl[0x448](rect);                                    // SetTaskbarThumbnailClipRect
//     pfn = DwmInvalidateIconicBitmaps, resolved once from dwmapi.dll
//           (string at mfc140u 0x3478a0, cached at data 0x3c3ed0 through
//           EncodePointer/DecodePointer);
//     if (pfn == NULL) return TRUE;   // 0x841b8: `mov $1,%eax` -- note TRUE
//     pfn(m_tabProxyWnd.m_hWnd);
//     return TRUE;
// The first gate is transcribed (IsTaskbarTabsSupportEnabled() returns FALSE
// in this tree); the second reads the proxy window's HWND, which OpenMFC does
// not model, so nothing past the gate is reachable.  STUB past the gate.
// Symbol: ?InvalidateIconicBitmaps@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__InvalidateIconicBitmaps_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return FALSE;
    }
    if (!impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(pThis)) {
        return FALSE;
    }
    // TODO(clean-room): the proxy-HWND gate at this+0x720+0x40, the clip-rect
    // refresh and DwmInvalidateIconicBitmaps are unmodeled.
    return FALSE;
}

// CMDIChildWndEx::IsPointNearDockSite(CPoint, DWORD&, BOOL&) const —
// transcribed from retail mfc140u.dll entry RVA 0x850f0, a two-instruction tail
// jump: `this += 0x3a8` then
// ?IsPointNearDockSite@CDockingManager@@QEBAHVCPoint@@AEAKAEAH@Z (0x48310) with
// (point, dwBarAlignment, bOuterEdge) unchanged; the two out-parameters go
// straight through.  Forwarded to the ChildExtra dock manager.  NOTE: the
// CDockingManager body in this tree is still a generated `return 0` with a
// placeholder parameter list (see the declaration note near the top), so today
// this reports "not near a dock site" and leaves the out-parameters untouched.
// Symbol: ?IsPointNearDockSite@CMDIChildWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CMDIChildWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const CMDIChildWndEx* pThis, unsigned long long point,
    unsigned long* dwBarAlignment, int* bOuterEdge) {
    if (!pThis) {
        return FALSE;
    }
    return impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
        DockManagerOf(const_cast<CMDIChildWndEx*>(pThis)), static_cast<long long>(point),
        dwBarAlignment, bOuterEdge);
}

// CMDIChildWndEx::IsRegisteredWithTaskbarTabs() — retail mfc140u.dll
// RVA 0x83fe0 is `return m_wndTaskbarTabProxy.GetSafeHwnd() != NULL;` -- it
// reads the HWND at this+0x720+0x40 (the tab-proxy CWnd's m_hWnd) and returns
// whether it is non-NULL.  OpenMFC's CMDIChildWndEx has no proxy window at
// +0x720 (the object is 0x228 bytes; see the layout note above) and
// RegisterTaskbarTab is a no-op, so no tab is ever registered and FALSE is the
// consistent answer.
// STUB -- only the constant reachable result is transcribed.
// Symbol: ?IsRegisteredWithTaskbarTabs@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__IsRegisteredWithTaskbarTabs_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
    return FALSE;
}

// CMDIChildWndEx::IsTabbedMDIChild() — retail mfc140u.dll entry RVA 0x86ad0:
//     if (m_pMDIFrame == NULL) return FALSE;                        // this+0x718
//     return m_pMDIFrame->m_wndClientArea.m_bTabIsEnabled            // frame+0x4574
//         || m_pMDIFrame->m_wndClientArea.m_bIsMDITabbedGroup;       // frame+0x4668
// (frame+0x4574 / +0x4668 are m_wndClientArea (frame+0x720) + 0x3e54 / + 0x3f48,
// the two offsets core/frame/CMDIClientAreaWnd.cpp pins for those members.)
// The m_pMDIFrame test is reproduced from ChildExtra (OnCreate stores it).
// The two flags live in the frame's m_wndClientArea, which
// core/frame/CMDIFrameWndEx.cpp does not construct, so they cannot be read
// here; FALSE is the value both hold while that file's EnableMDITabs /
// EnableMDITabbedGroups stay no-ops (true as of this pass).
// STUB past the null test -- the two flag reads are missing.
// Symbol: ?IsTabbedMDIChild@CMDIChildWndEx@@UEAAHXZ
extern "C" int MS_ABI impl__IsTabbedMDIChild_CMDIChildWndEx__UEAAHXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis || MDIFrameOf(pThis) == nullptr) {
        return FALSE;
    }
    // TODO(clean-room): m_wndClientArea.m_bTabIsEnabled / m_bIsMDITabbedGroup
    // of the owning frame are not constructed in this tree.
    return FALSE;
}

// CMDIChildWndEx::IsTaskbarTabsSupportEnabled() — retail mfc140u.dll entry RVA
// 0x84000:
//     CFrameWnd* f = GetTopLevelFrame();                          // 0x28e490
//     if (f == NULL || !f->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return FALSE;
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;     // 0x133930, +0x8
//     if (pApp == NULL || !pApp->vtbl[0x210]()) return FALSE;     // IsTaskbarInteractionEnabled
//     if (!this->vtbl[0x3f8]()) return FALSE;    // slot 127 of the CMDIChildWndEx vftable
//                                                // (0x2ee018): CanShowOnTaskBarTabs(), whose
//                                                // inline default is `return TRUE` (folded 0x3a60)
//     <afxGlobalData init prologue>
//     if (afxGlobalData.<+0x258> == 0) return FALSE;              // data 0x3c1878
//     return !((GetStyle() >> 19) & 1);                           // TRUE iff WS_SYSMENU clear
// (The slot-127 name comes from the shipping afxmdichildwndex.h declaration
// order: CanShowOnMDITabs 0x3d8, CanShowOnWindowsList 0x3e0, IsReadOnly 0x3e8,
// UpdateTaskbarTabIcon 0x3f0, CanShowOnTaskBarTabs 0x3f8, OnGetIconicThumbnail
// 0x400, OnGetIconicLivePreviewBitmap 0x408, OnTaskbarTabThumbnailStretch 0x410;
// the exported names at 0x3d8/0x3f0/0x410 pin the sequence.)
// Every input of that chain reads FALSE in this tree today (the CWinApp
// taskbar flag defaults to false in detail/CWinAppSupport.h and the +0x258 byte
// is never set because AFX_GLOBAL_DATA::Initialize is a no-op), so the constant
// below is the value retail would compute here.  It is kept a constant on
// purpose rather than transcribed: core/frame/CMDIClientAreaWnd.cpp reads the
// tab-proxy HWND at CMDIChildWndEx+0x760 only behind this export returning
// FALSE, and that offset is past the end of OpenMFC's 0x228-byte object (the
// proxy CMDITabProxyWnd at retail +0x720 is never constructed here).  Flipping
// this to TRUE without modelling the proxy window would turn those reads into
// out-of-bounds accesses.  STUB -- see the headerRequest filed with this file.
// Symbol: ?IsTaskbarTabsSupportEnabled@CMDIChildWndEx@@QEAAHXZ
extern "C" int MS_ABI impl__IsTaskbarTabsSupportEnabled_CMDIChildWndEx__QEAAHXZ(
    CMDIChildWndEx* pThis) {
    (void)pThis;
    return FALSE;
}

// CMDIChildWndEx::OnChangeVisualManager(WPARAM, LPARAM) — transcribed from
// retail mfc140u.dll entry RVA 0x860b0 (neither argument is read):
//     if (m_pMDIFrame != NULL                                        // this+0x718
//         && !m_pMDIFrame->m_wndClientArea.m_bIsMDITabbedGroup)      // frame+0x4668
//         m_Impl.OnChangeVisualManager();   // ?OnChangeVisualManager@CFrameImpl@@QEAAXXZ
//                                           // (0x668d0, mfc140u) on this+0x218
//     return 0;
// m_pMDIFrame and m_Impl are the ChildExtra ones (OnCreate stores the frame;
// the ctor builds the CFrameImpl).  DEVIATION: frame+0x4668 is
// m_wndClientArea.m_bIsMDITabbedGroup, and core/frame/CMDIFrameWndEx.cpp does
// not construct m_wndClientArea, so the flag cannot be read; it is taken as
// FALSE -- the value it holds while that file's EnableMDITabbedGroups is a
// no-op (true as of this pass) -- which selects the arm that makes the
// CFrameImpl call.
// Symbol: ?OnChangeVisualManager@CMDIChildWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeVisualManager_CMDIChildWndEx__IEAA_J_K_J_Z(
    CMDIChildWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    if (!pThis) {
        return 0;
    }
    if (MDIFrameOf(pThis) != nullptr) {
        // TODO(clean-room): retail also requires the owning frame's
        // m_wndClientArea.m_bIsMDITabbedGroup to be clear; unreadable here.
        impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(FrameImplOf(pThis));
    }
    return 0;
}

// CMDIChildWndEx::OnCreate(LPCREATESTRUCT) — transcribed from retail
// mfc140u.dll entry RVA 0x83a30:
//     CMDIFrameWnd* f = GetMDIFrame();                            // 0x2a7870
//     m_pMDIFrame = (f && f->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) ? f : NULL;  // +0x718
//     m_bInOnCreate = TRUE;                                       // +0x210
//     m_Impl.<+0x28> = (cs->style >> 23) & 1;                     // this+0x240: WS_BORDER
//     if (!(GetStyle() & WS_SYSMENU)) {                           // bt $0x13
//         CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));  // 0x28ad70
//         ::SendMessageW(pParent->m_hWnd, WM_SETREDRAW, FALSE, 0);
//         m_rectOriginal = {cs->x, cs->y, cs->x + cs->cx, cs->y + cs->cy};  // +0x1fc..0x208
//         if (m_pMDIFrame && !m_pMDIFrame->m_wndClientArea.m_bIsMDITabbedGroup /*+0x4668*/) {
//             RECT rcClient; ::GetClientRect(m_pMDIFrame->m_wndClientArea.m_hWnd /*+0x760*/, &rcClient);
//             RECT rcMine;   ::GetClientRect(m_hWnd, &rcMine); ClientToScreen(&rcMine);  // 0x2a3310
//             RECT rcWnd;    ::GetWindowRect(m_hWnd, &rcWnd);
//             <grow rcClient by the (rcWnd - rcMine) frame borders on each side>
//             SetWindowPos(NULL, rcClient.left, rcClient.top, width, height,
//                          SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/);   // 0x2a9a60
//         }
//         ::SendMessageW(pParent->m_hWnd, WM_SETREDRAW, TRUE, 0);
//     }
//     // 0x83bef: `mov (%rdi),%r8; mov 0x30(%r8),%r8` -- lpCreateParams is the
//     // MDICREATESTRUCTW Windows hands an MDI child, and its lParam (+0x30) is
//     // the CCreateContext* (the same unpacking CMDIChildWnd::OnCreate does)
//     if (CFrameWnd::OnCreateHelper(cs,
//             (CCreateContext*)((MDICREATESTRUCTW*)cs->lpCreateParams)->lParam) == -1)  // 0x29dba0
//         { m_bInOnCreate = FALSE; return -1; }
//     CDockingManager* dm = m_pMDIFrame->m_Impl.<+0x120>;        // frame+0x6b0, NO null test
//                                                                // on m_pMDIFrame at 0x83c0a
//     if (dm && dm-><+0x208>) ::SendMessageW(m_pMDIFrame->m_hWnd, WM_CLOSE, 0, 0);
//     CFrameImpl::AddFrame(this);                                 // 0x66110
//     this->vtbl[0x3c0](NULL);                                    // RegisterTaskbarTab(NULL)
//     ::PostMessageW(m_hWnd, AFX_WM_CHANGEVISUALMANAGER, 0, 0);   // data 0x3c2508
//     m_bInOnCreate = FALSE;
//     return 0;
// Reproduced: the m_pMDIFrame store (GetMDIFrame() when it IsKindOf
// CMDIFrameWndEx, else NULL -- kept in ChildExtra, see the layout note above),
// the OnCreateHelper call with the MDICREATESTRUCTW's lParam as the create
// context (an earlier revision of this body passed lpCreateParams itself,
// i.e. the MDICREATESTRUCTW, as the CCreateContext*; retail reads +0x30 of
// it), the -1 propagation, the static
// CFrameImpl::AddFrame registration, RegisterTaskbarTab(NULL) as a direct call
// to this class's export (no MSVC vtable here), the AFX_WM_CHANGEVISUALMANAGER
// post and the 0 result.  NOT reproduced, all for want of a slot in OpenMFC's
// 0x228-byte CMDIChildWndEx: m_bInOnCreate, the m_Impl WS_BORDER flag, the
// WS_SYSMENU-clear geometry pass (it needs m_rectOriginal and the frame's
// client-area window at frame+0x760) and the frame-side print-preview probe
// (frame+0x6b0 is m_Impl.m_pDockManager of the CMDIFrameWndEx; the WM_CLOSE it
// sends is CFrameWnd::OnClose's print-preview exit path, not an application
// close).  Deviations: a NULL lpCreateStruct returns -1 here where retail would
// fault reading cs->style, and a NULL lpCreateParams passes a NULL context
// where retail would fault reading its lParam.  The previous body was a bare
// `return 0`, which skipped CFrameWnd::OnCreateHelper (and so the frame-list
// registration and client creation) for every application that calls
// CMDIChildWndEx::OnCreate from its own handler.
// Symbol: ?OnCreate@CMDIChildWndEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMDIChildWndEx__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMDIChildWndEx* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (!pThis || !lpCreateStruct) {
        return -1;
    }
    CMDIFrameWnd* pMDIFrame = impl__GetMDIFrame_CMDIChildWnd__QEAAPEAVCMDIFrameWnd__XZ(pThis);
    SetMDIFrame(pThis,
                (pMDIFrame != nullptr &&
                 impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                     pMDIFrame, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ()))
                    ? static_cast<CMDIFrameWndEx*>(pMDIFrame)
                    : nullptr);
    // TODO(clean-room): m_bInOnCreate (+0x210), the m_Impl WS_BORDER flag
    // (+0x240) and the WS_SYSMENU-clear geometry pass (+0x1fc..0x208,
    // frame+0x760) are unmodeled.
    // lpCreateParams of an MDI child is the MDICREATESTRUCTW; retail takes the
    // CCreateContext* from its lParam (0x83bef..0x83bf8 inside OnCreate).
    const MDICREATESTRUCTW* pMDICreate =
        static_cast<const MDICREATESTRUCTW*>(lpCreateStruct->lpCreateParams);
    CCreateContext* pContext =
        pMDICreate != nullptr ? reinterpret_cast<CCreateContext*>(pMDICreate->lParam) : nullptr;
    if (impl__OnCreateHelper_CFrameWnd__IEAAHPEAUtagCREATESTRUCTW__PEAUCCreateContext___Z(
            pThis, lpCreateStruct, pContext) == -1) {
        return -1;
    }
    // TODO(clean-room): retail ends the owning CMDIFrameWndEx's print preview
    // here (frame+0x6b0 -> +0x208 probe, then WM_CLOSE to the frame); the
    // frame-side state is unmodeled.
    impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(pThis);
    impl__RegisterTaskbarTab_CMDIChildWndEx__UEAAXPEAV1__Z(pThis, nullptr);
    ::PostMessage(pThis->m_hWnd, impl__AFX_WM_CHANGEVISUALMANAGER__3IA, 0, 0);
    return 0;
}

// CMDIChildWndEx::OnDestroy() — partially transcribed from retail mfc140u.dll
// entry RVA 0x856f0, which is, in order:
//     UnregisterTaskbarTab(TRUE);                                 // 0x855d0
//     if (m_pMDIFrame && m_pMDIFrame->m_Impl.m_pDockManager /*frame+0x6b0*/
//         && m_pMDIFrame->m_Impl.m_pDockManager-><+0x208>)        // print-preview latch
//         ::SendMessageW(m_pMDIFrame->m_hWnd, WM_CLOSE, 0, 0);
//     if (m_pTabbedControlBar /*+0x710*/ && CWnd::FromHandlePermanent(its m_hWnd)) {  // 0x28adc0
//         if (CWnd::FromHandle(::GetParent(bar->m_hWnd)) == this
//             && m_pMDIFrame && !m_pMDIFrame-><+0x214>) {
//             bar->ShowWindow(SW_HIDE);                           // 0x2a9ad0
//             ::SetParent(bar->m_hWnd, bar->vtbl[0x348]() ? its m_hWnd : NULL);   // GetDockSiteFrameWnd
//             m_pMDIFrame->m_dockManager.<0x4f198>(&bar->m_hWnd); // (frame+0x228)+0x2d0
//         }
//         m_pTabbedControlBar = NULL;
//     }
//     CFrameImpl::RemoveFrame(this);                              // 0x66160
//     for (node = m_dockManager.m_lstMiniFrames.head /*this+0x528*/; node; node = node->next)
//         if (node->data && node->data->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd) /*0x3b1418*/))
//             node->data->vtbl[0xd0]();                           // CWnd::DestroyWindow
//     CObList lstChildren;                                        // vftable 0x2e9248
//     for (HWND h = ::GetTopWindow(m_hWnd); h; h = ::GetWindow(h, GW_HWNDNEXT))
//         lstChildren.AddTail(CWnd::FromHandle(h)->m_hWnd);       // 0x7908
//     for (each HWND h in lstChildren)
//         if (::IsWindow(h) && ::GetParent(h) == m_hWnd) ::DestroyWindow(h);   // IAT 0x2c7138 /
//                                                                 // 0x2c72d8 / 0x2c6d28
//     m_pRelatedTabGroup = NULL;                                  // this+0x1e8
//     CMDIChildWnd::UpdateClientEdge(NULL);                       // 0x2a72f0  } inlined
//     CFrameWnd::OnDestroy();                                     // 0x29e2a0  } CMDIChildWnd::OnDestroy
// Reproduced: UnregisterTaskbarTab(TRUE); CFrameImpl::RemoveFrame(this) (the
// counterpart of the AddFrame that OnCreate performs -- without it the
// frame list in core/frame/CFrameImpl.cpp keeps a dangling pointer that the
// next AddFrame/RemoveFrame dereferences); the explicit destruction of every
// direct child window, done on raw HWNDs instead of through CWnd::FromHandle
// temporaries and a CObList (same windows, same order); the
// m_pRelatedTabGroup reset (+0x1e8 lies inside OpenMFC's 0x228-byte object;
// core/frame/CMDIClientAreaWnd.cpp writes the same slot raw); and the inlined
// CMDIChildWnd::OnDestroy tail (UpdateClientEdge(NULL) then
// CFrameWnd::OnDestroy, both as direct calls to those exports -- an earlier
// version of this comment claimed retail does NOT chain to
// CMDIChildWnd::OnDestroy; the calls at 0x8592b / 0x85933 are exactly that
// chain, inlined).  NOT reproduced: the frame-side print-preview probe
// (frame+0x6b0 is the CMDIFrameWndEx's m_Impl.m_pDockManager, unreadable from
// here), the tabbed-pane hand-back (m_pTabbedControlBar at +0x710 is
// unmodelled; AddTabbedPane is a no-op so there is never one to hand back), and
// the mini-frame DestroyWindow walk (OpenMFC's CDockingManager keeps its
// mini-frames in featurepack/docking/CDockingManager.cpp's side table, which
// this file cannot reach).  Deviation: the child walk is skipped when m_hWnd
// is NULL -- retail would call ::GetTopWindow(NULL) there and test top-level
// windows against a NULL parent, a path no WM_DESTROY ever takes.
// Symbol: ?OnDestroy@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMDIChildWndEx__IEAAXXZ(CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    impl__UnregisterTaskbarTab_CMDIChildWndEx__QEAAXH_Z(pThis, TRUE);
    // TODO(clean-room): the owning frame's print-preview probe (frame+0x6b0 ->
    // +0x208) and the tabbed-pane hand-back (+0x710) are unmodeled.
    impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(pThis);
    // TODO(clean-room): the DestroyWindow walk over m_dockManager's mini-frame
    // list (this+0x528) is unreachable from this file.
    const HWND hWnd = pThis->m_hWnd;
    if (hWnd != nullptr) {
        // Snapshot first, destroy second, exactly as retail's CObList pass does
        // (a hand-rolled list: array new/delete would add libstdc++ undefineds
        // that this file's link audit rejects).
        struct HwndNode { HWND hWnd; HwndNode* pNext; };
        HwndNode* pHead = nullptr;
        HwndNode** ppTail = &pHead;
        for (HWND hChild = ::GetTopWindow(hWnd); hChild != nullptr;
             hChild = ::GetWindow(hChild, GW_HWNDNEXT)) {
            HwndNode* pNode = new HwndNode{hChild, nullptr};
            *ppTail = pNode;
            ppTail = &pNode->pNext;
        }
        while (pHead != nullptr) {
            HwndNode* pNode = pHead;
            pHead = pNode->pNext;
            if (::IsWindow(pNode->hWnd) && ::GetParent(pNode->hWnd) == hWnd) {
                ::DestroyWindow(pNode->hWnd);
            }
            delete pNode;
        }
    }
    *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(pThis) + kRelatedTabGroupOffset) =
        nullptr;   // m_pRelatedTabGroup = NULL
    impl__UpdateClientEdge_CMDIChildWnd__IEAAHPEAUtagRECT___Z(pThis, nullptr);
    impl__OnDestroy_CFrameWnd__IEAAXXZ(pThis);
}

// CMDIChildWndEx::OnEraseBkgnd(CDC*) — transcribed from retail mfc140u.dll
// RVA 0x3a60, whose entire body is `mov $1,%eax; ret`: it claims the erase
// unconditionally and never touches the DC.  The previous body returned 0,
// which told Windows to erase the background with the class brush.  The body is
// intentionally a single constant, because retail's is.
// Two caveats for whoever re-checks this: (1) the export has no entry in
// EITHER rva-symbol map, so `disas.py --u '<name>'` reports NOT FOUND; resolve
// it through the export directory (ordinal 9752 -> 0x3a60 in mfc140u,
// ures.py); the ANSI twin's folded body at mfc140.dll 0x3ae0 is the same
// `mov $1,%eax; ret`.  (2) 0x3a60 is an identical-COMDAT-folded address shared
// by 101 exports, so a reverse-map disassembler prints some other name at it.
// Symbol: ?OnEraseBkgnd@CMDIChildWndEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMDIChildWndEx__IEAAHPEAVCDC___Z(
    CMDIChildWndEx* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// CMDIChildWndEx::OnGetMinMaxInfo(MINMAXINFO*) — transcribed from retail
// mfc140u.dll RVA 0x85960, whose whole body is:
//     void* p = this->[0x718];
//     if (p != NULL && ((int*)p)[0x6d8/4] != 0 && ((int*)p)[0x4574/4] == 0 &&
//         ((int*)p)[0x4668/4] == 0) {
//         int cx = ((int*)p)[0x6c8/4] - ((int*)p)[0x6c0/4];
//         lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x = cx;   // +0x20, +0x8
//         int cy = ((int*)p)[0x6cc/4] - ((int*)p)[0x6c4/4];
//         lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y = cy;   // +0x24, +0xc
//         return;
//     }
//     CWnd::Default();      // tail jump
// m_pMDIFrame (this+0x718) is in ChildExtra, but the four frame members the
// guard and the clamp read (frame+0x6d8, the two m_wndClientArea flags at
// +0x4574/+0x4668 and the rect at +0x6c0..+0x6cc) are unreadable in this tree,
// so the guarded clamp is not reproduced and the CWnd::Default() tail is the
// whole of the reachable behaviour.
// Symbol: ?OnGetMinMaxInfo@CMDIChildWndEx@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CMDIChildWndEx__IEAAXPEAUtagMINMAXINFO___Z(
    CMDIChildWndEx* pThis, MINMAXINFO* lpMMI) {
    (void)lpMMI;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail clamps ptMaxSize/ptMaxTrackSize to the owning
    // frame's +0x6c0..+0x6cc rect behind its +0x6d8/+0x4574/+0x4668 gates;
    // those frame members are unreadable here.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnIdleUpdateCmdUI(WPARAM, LPARAM) — transcribed from retail
// mfc140u.dll entry RVA 0x85e60 (neither argument is read):
//     m_dockManager.SendMessageToMiniFrames(0x363 /*WM_IDLEUPDATECMDUI*/, 0, 0);
//                        // ?SendMessageToMiniFrames@CDockingManager@@QEAAHI_K_J@Z
//                        // (0x4d2a0) on this+0x3a8; its result is discarded
//     return 0;
// Forwarded to the ChildExtra dock manager.
// Symbol: ?OnIdleUpdateCmdUI@CMDIChildWndEx@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMDIChildWndEx__IEAA_J_K_J_Z(
    CMDIChildWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    if (!pThis) {
        return 0;
    }
    impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
        DockManagerOf(pThis), 0x0363 /*WM_IDLEUPDATECMDUI*/, 0, 0);
    return 0;
}

// CMDIChildWndEx::OnLButtonDown(UINT, CPoint) — transcribed from retail
// mfc140u.dll RVA 0x86510, whose whole body is:
//     CFrameImpl(this+0x218).OnLButtonDown(point);   // 0x66490
//     CWnd::Default();                               // tail jump to 0x28ac80
// The ChildExtra CFrameImpl (retail this+0x218; it drives the caption-button
// hit testing) is not wired into this handler yet, so only the base dispatch
// is reproduced.
// Symbol: ?OnLButtonDown@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnLButtonDown on the
    // sub-object at this+0x218; the ChildExtra CFrameImpl is not wired in here yet.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnLButtonUp(UINT, CPoint) — transcribed from retail
// mfc140u.dll RVA 0x864b0, whose whole body is:
//     CFrameImpl(this+0x218).OnLButtonUp(point);     // 0x664e0
//     CWnd::Default();                               // tail jump to 0x28ac80
// The ChildExtra CFrameImpl (retail this+0x218) is not wired into this
// handler yet, so only the base dispatch is reproduced.
// Symbol: ?OnLButtonUp@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnLButtonUp on the
    // sub-object at this+0x218; the ChildExtra CFrameImpl is not wired in here yet.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnMDIActivate(BOOL, CWnd*, CWnd*) — partially transcribed
// from retail mfc140u.dll RVA 0x84480:
//     m_dockManager.vtbl[0xc0](bActivate);        // slot 0xc0 of the retail CDockingManager
//                                                 // vftable 0x2e6120 = ?OnActivateFrame@CDockingManager@@UEAAXH@Z;
//                                                 // EDX still holds bActivate at the call
//     this->[0x20c] = bActivate;
//     if (g_bInMDIActivate == 0) {                // process-wide latch,
//                                                 // mfc140u data 0x3c3e54
//         g_bInMDIActivate = TRUE;
//         CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
//         if (bActivate && this->[0x718])
//              CMDIClientAreaWnd::SetActiveTab((this->[0x718])+0x720,
//                                              pActivateWnd ? pActivateWnd->m_hWnd
//                                                           : NULL);   // 0x7e540
//         <taskbar-tab refresh: InvalidateIconicBitmaps() on this and on
//          pDeactivateWnd when it is a CMDIChildWndEx, then SetTaskbarTabActive()>
//         g_bInMDIActivate = FALSE;
//         <a second bActivate-gated block driven by this+0x718>
//     }
//  tail:                                          // 0x845fc / 0x84600 /
//                                                 // 0x8460c / 0x8461f
//     if (!bActivate || IsTaskbarTabsSupportEnabled()) {   // 0x84600
//         if (m_wndTaskbarTabProxy(this+0x720).m_hWnd != NULL)  // 0x8460c
//             return;
//     }
//     this->vtbl[0x448](CRect(0,0,0,0));          // SetTaskbarThumbnailClipRect
// Both the latched and the unlatched path fall into that shared tail: the
// latch's `jne` at 0x844b8 targets 0x845fc, not the ret at 0x84640.  (An
// earlier version of this comment drew the re-entrant path as a plain return;
// it is not.)
// The dock manager's OnActivateFrame (the ChildExtra manager, direct call to
// the export), the recursion latch and the base dispatch are reproduced;
// this+0x20c has no slot, and the two m_pMDIFrame-gated blocks run on the
// frame's m_wndClientArea (frame+0x720), which is not constructed in this
// tree, so they are skipped.  The tail is omitted here because
// SetTaskbarThumbnailClipRect returns FALSE on its first gates in this build
// and the call would have no observable effect.  The latch is kept in a
// file-local static, matching retail's single process-wide (non-thread-safe)
// variable.
// Symbol: ?OnMDIActivate@CMDIChildWndEx@@IEAAXHPEAVCWnd@@0@Z
extern "C" void MS_ABI impl__OnMDIActivate_CMDIChildWndEx__IEAAXHPEAVCWnd__0_Z(
    CMDIChildWndEx* pThis, int bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) {
    if (!pThis) {
        return;
    }
    impl__OnActivateFrame_CDockingManager__UEAAXH_Z(DockManagerOf(pThis), bActivate);
    if (g_bInMDIActivate) {
        return;
    }
    g_bInMDIActivate = true;
    impl__OnMDIActivate_CMDIChildWnd__IEAAXHPEAVCWnd__0_Z(
        pThis, bActivate, pActivateWnd, pDeactivateWnd);
    // TODO(clean-room): the MDI-tab activation (SetActiveTab on the owning
    // frame's unconstructed m_wndClientArea) and the taskbar-tab refresh (the
    // never-constructed tab proxy) are skipped.
    g_bInMDIActivate = false;
}

// CMDIChildWndEx::OnMouseMove(UINT, CPoint) — transcribed from retail
// mfc140u.dll RVA 0x864e0, whose whole body is:
//     CFrameImpl(this+0x218).OnMouseMove(point);     // 0x665e0
//     CWnd::Default();                               // tail jump to 0x28ac80
// The ChildExtra CFrameImpl (retail this+0x218) is not wired into this
// handler yet, so only the base dispatch is reproduced.
// Symbol: ?OnMouseMove@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nFlags, unsigned long long point) {
    (void)nFlags;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnMouseMove on the
    // sub-object at this+0x218; the ChildExtra CFrameImpl is not wired in here yet.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnMoveMiniFrame(CWnd*) — transcribed from retail mfc140u.dll
// entry RVA 0x85270, a four-instruction tail dispatch: `this += 0x3a8` then the
// dock manager's vtable slot 0x88 with the same CWnd*, returning its BOOL
// result.  Slot 0x88 of the retail CDockingManager vftable (0x2e6120, mfc140u)
// is ?OnMoveMiniFrame@CDockingManager@@UEAAHPEAVCWnd@@@Z (0x492a0), called
// here as a direct call to that export on the ChildExtra dock manager (an
// override in a derived dock manager is not honoured).
// Symbol: ?OnMoveMiniFrame@CMDIChildWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CMDIChildWndEx__UEAAHPEAVCWnd___Z(
    CMDIChildWndEx* pThis, CWnd* pFrame) {
    if (!pThis) {
        return FALSE;
    }
    return impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(DockManagerOf(pThis), pFrame);
}

// CMDIChildWndEx::OnNcActivate(BOOL) — partially transcribed from retail
// mfc140u.dll RVA 0x85f90:
//     BOOL bRepaint = CFrameImpl(this+0x218).OnNcActivate(bActive);  // 0x65ba0
//     if (bRepaint) ::SendMessageW(m_hWnd, WM_SETREDRAW, FALSE, 0);
//     LRESULT lRes = CWnd::Default();                                // 0x28ac80
//     if (bRepaint) {
//         ::SendMessageW(m_hWnd, WM_SETREDRAW, TRUE, 0);
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x585);
//     }
//     <AfxGetModuleState()->[+0x8] slot-0x208 / slot-0x70 probe; when it says
//      no, InvalidateIconicBitmaps()>
//     return (BOOL)lRes;
// 0x585 == RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_NOCHILDREN|RDW_FRAME
// (0x1|0x4|0x80|0x100|0x400).  The ChildExtra CFrameImpl is not wired into
// this handler yet, so bRepaint is not computed and the SetRedraw bracket is
// skipped entirely --
// note that this means the WM_SETREDRAW pair is omitted on BOTH paths, not
// conditionally.  InvalidateIconicBitmaps() is itself a stub, so the module-
// state probe is skipped too.  The previous body returned 0 unconditionally,
// which suppressed the non-client active-state change.
// Symbol: ?OnNcActivate@CMDIChildWndEx@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMDIChildWndEx__IEAAHH_Z(
    CMDIChildWndEx* pThis, int bActive) {
    (void)bActive;
    if (!pThis) {
        return FALSE;
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// CMDIChildWndEx::OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*) — transcribed from
// retail mfc140u.dll entry RVA 0x86430 (bCalcValidRects is never read):
//     if (!(m_pMDIFrame && m_pMDIFrame->m_wndClientArea.m_bIsMDITabbedGroup)   // +0x718, +0x4668
//         && !::IsZoomed(m_hWnd)                                  // IAT 0x2c6cf8
//         && CMFCVisualManager::GetInstance()->vtbl[0x3f8]()      // 0x9774; IsOwnerDrawCaption
//         && !(GetStyle() & WS_BORDER))                           // bt $0x17
//         lpncsp->rgrc[0].top += ::GetSystemMetrics(SM_CYCAPTION);   // +0x4; IAT 0x2c6bd8, arg 4
//     CWnd::Default();                                            // tail jump 0x28ac80
// The tabbed-group test reads the frame's m_wndClientArea (not constructed in
// this tree; m_pMDIFrame itself is in ChildExtra) and is taken as "not a tabbed
// group", which is the arm that continues to the other tests; those are reproduced, with the
// manager predicate answered by VisualManagerIsOwnerDrawCaption() (FALSE for
// every OpenMFC manager, see that helper), so today the caption-height insert
// is never applied and the body reduces to the unconditional Default().
// Symbol: ?OnNcCalcSize@CMDIChildWndEx@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMDIChildWndEx__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CMDIChildWndEx* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail skips the insert when the owning frame's MDI
    // tabbed groups are on (m_pMDIFrame at +0x718 -> +0x4668); the frame's
    // m_wndClientArea is not constructed in this tree.
    if (!::IsZoomed(pThis->m_hWnd) && VisualManagerIsOwnerDrawCaption() &&
        !(impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_BORDER) && lpncsp != nullptr) {
        lpncsp->rgrc[0].top += ::GetSystemMetrics(SM_CYCAPTION);
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnNcHitTest(CPoint) — transcribed from retail mfc140u.dll
// entry RVA 0x86080:
//     UINT nHit = m_Impl.OnNcHitTest(point);                      // 0x66210 on this+0x218
//     if (nHit != 0) return nHit;                                 // zero-extended
//     return CWnd::Default();                                     // tail jump 0x28ac80
// The ChildExtra CFrameImpl is not wired into this handler yet (see the file
// header), so only the Default() arm --
// the one retail takes when the caption-button hit test misses -- is
// reproduced.  The CPoint arrives packed in RDX (an 8-byte aggregate under
// MS_ABI), modeled as `long long`.
// Symbol: ?OnNcHitTest@CMDIChildWndEx@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMDIChildWndEx__IEAA_JVCPoint___Z(
    CMDIChildWndEx* pThis, long long point) {
    (void)point;
    if (!pThis) {
        return 0;
    }
    // TODO(clean-room): retail first asks CFrameImpl::OnNcHitTest (this+0x218)
    // and returns its non-zero answer; the ChildExtra CFrameImpl is not wired
    // in here yet.
    return impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnNcMouseMove(UINT, CPoint) — transcribed from retail
// mfc140u.dll entry RVA 0x86050, whose whole body is:
//     m_Impl.OnNcMouseMove(nHitTest, point);                      // 0x66440 on this+0x218
//     CWnd::Default();                                            // tail jump 0x28ac80
// (retail passes R8 -- the packed CPoint -- as the sub-object call's second
// argument and never reads nHitTest itself.)  The ChildExtra CFrameImpl is
// not wired into this handler yet, so only the unconditional Default() is
// reproduced.
// Symbol: ?OnNcMouseMove@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nHitTest, long long point) {
    (void)nHitTest;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail first runs CFrameImpl::OnNcMouseMove on the
    // sub-object at this+0x218; the ChildExtra CFrameImpl is not wired in here yet.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnNcPaint() — transcribed from retail mfc140u.dll entry RVA
// 0x85f20:
//     if (!(m_pMDIFrame && m_pMDIFrame->m_wndClientArea.m_bIsMDITabbedGroup)   // +0x718, +0x4668
//         && !::IsZoomed(m_hWnd)                                  // IAT 0x2c6cf8
//         && CMFCVisualManager::GetInstance()->vtbl[0x3f8]()      // 0x9774; IsOwnerDrawCaption
//         && m_Impl.OnNcPaint())                                  // 0x653a0 on this+0x218
//         return;
//     CWnd::Default();                                            // 0x28ac80
// The manager predicate is FALSE for every OpenMFC manager
// (VisualManagerIsOwnerDrawCaption), which short-circuits the chain before the
// unmodeled CFrameImpl call, so the reachable behaviour is Default() alone.
// Symbol: ?OnNcPaint@CMDIChildWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMDIChildWndEx__IEAAXXZ(CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): the tabbed-group test (+0x718 -> frame+0x4668, an
    // unconstructed frame sub-object) and CFrameImpl::OnNcPaint (the ChildExtra
    // CFrameImpl is not wired into this handler yet) are skipped; with
    // VisualManagerIsOwnerDrawCaption() FALSE the chain never reaches them, so
    // the ::IsZoomed test is not repeated here either -- Default() is the only
    // reachable arm.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnNcRButtonUp(UINT, CPoint) — transcribed from retail
// mfc140u.dll entry RVA 0x865f0:
//     if (m_pTabbedControlBar != NULL                             // this+0x710
//         && nHitTest == HTCAPTION /*2*/
//         && !::IsZoomed(m_hWnd)) {                               // IAT 0x2c6cf8
//         m_pTabbedControlBar->vtbl[0x5d8](point);                // CPane::OnShowControlBarMenu
//         return;
//     }
//     CWnd::Default();                                            // 0x28ac80
// (slot 0x5d8 of the retail CDockablePane vftable is
// ?OnShowControlBarMenu@CPane@@UEAAHVCPoint@@@Z.)  The tabbed-pane slot at
// this+0x710 is not modeled by OpenMFC, so the guarded arm can never be taken
// and the Default() arm is reproduced.
// Symbol: ?OnNcRButtonUp@CMDIChildWndEx@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcRButtonUp_CMDIChildWndEx__IEAAXIVCPoint___Z(
    CMDIChildWndEx* pThis, unsigned int nHitTest, long long point) {
    (void)nHitTest;
    (void)point;
    if (!pThis) {
        return;
    }
    // TODO(clean-room): retail shows the tabbed pane's control-bar menu on a
    // caption right-click when m_pTabbedControlBar (+0x710) is set; unmodeled.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMDIChildWndEx::OnPressTaskbarThmbnailCloseButton() — transcribed from
// retail mfc140u.dll entry RVA 0x86770, whose whole body is:
//     CFrameWnd* f = GetTopLevelFrame();                          // 0x28e490
//     if (f == NULL) return;
//     if (!f->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) return;    // 0x234cf0, 0x2ee478
//     if (!f->IsWindowEnabled()) return;                          // 0x2a9b00
//     CDocument* pDoc = this->vtbl[0x2e8]();                      // GetActiveDocument
//     if (pDoc != NULL && pDoc->vtbl[0xd0]())                     // IsModified
//         this->vtbl[0x450]();                                    // ActivateTopLevelFrame
//     ::PostMessageW(m_hWnd, WM_CLOSE /*0x10*/, 0, 0);            // IAT 0x2c72b0
// Every input exists here.  The three virtuals are direct calls to the
// exports the retail CMDIChildWndEx vftable (0x2ee018) carries at +0x2e8
// (?GetActiveDocument@CFrameWnd@@) and +0x450 (this class's
// ActivateTopLevelFrame), and to the OpenMFC helper for CDocument slot +0xd0
// (see its declaration above); an override in a further-derived class is not
// honoured by any of the three.
// Symbol: ?OnPressTaskbarThmbnailCloseButton@CMDIChildWndEx@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressTaskbarThmbnailCloseButton_CMDIChildWndEx__UEAAXXZ(
    CMDIChildWndEx* pThis) {
    if (!pThis) {
        return;
    }
    CFrameWnd* pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrame == nullptr) {
        return;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pFrame, impl__GetThisClass_CMDIFrameWndEx__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(pFrame)) {
        return;
    }
    CDocument* pDoc = impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(pThis);
    if (pDoc != nullptr && impl__IsModified_CDocument__UEBAHXZ(pDoc)) {
        impl__ActivateTopLevelFrame_CMDIChildWndEx__MEAAXXZ(pThis);
    }
    ::PostMessage(pThis->m_hWnd, WM_CLOSE, 0, 0);
}

// CMDIChildWndEx::OnSendIconicLivePreviewBitmap(WPARAM, LPARAM) — retail
// mfc140u.dll entry RVA 0x86180 (neither argument is read):
//     CDC dc;                              // vftable 0x33b510; m_hDC = m_hAttribDC = NULL
//     ::SendMessageW(m_hWnd, WM_PRINTCLIENT /*0x318*/, 0, PRF_CLIENT /*4*/);
//     RECT rc; ::GetClientRect(m_hWnd, &rc);                      // IAT 0x2c7330
//     POINT pt = { rc.left, rc.top };
//     CBitmap* pBmp = CGdiObject::FromHandle(                     // 0x2a3ea0
//         ::GetCurrentObject(dc.m_hAttribDC /*NULL*/, OBJ_BITMAP /*7*/));   // IAT 0x2c6150
//     <DwmSetIconicLivePreviewBitmap loader at 0x1caed8>(m_hWnd, pBmp->m_hObject /*+0x8*/, &pt, 1);
//     if (dc.m_hDC) ::DeleteDC(dc.Detach());                      // 0x2a24d0, IAT 0x2c6148
// The DC is never created, so ::GetCurrentObject(NULL, OBJ_BITMAP) yields NULL,
// CGdiObject::FromHandle(NULL) yields NULL, and the `mov 0x8(%rax)` at 0x86218
// dereferences it: this handler can only be safe because DWM never sends
// WM_DWMSENDICONICLIVEPREVIEWBITMAP to the child frame itself (the
// DWMWA_HAS_ICONIC_BITMAP attribute is set on the tab proxy window in
// RegisterTaskbarTab, never on this window).  Not reproduced: a faithful body
// would fault, and a WM_PRINTCLIENT with a NULL HDC has no useful effect.
// STUB -- left a no-op deliberately.
// Symbol: ?OnSendIconicLivePreviewBitmap@CMDIChildWndEx@@IEAAX_K_J@Z
extern "C" void MS_ABI impl__OnSendIconicLivePreviewBitmap_CMDIChildWndEx__IEAAX_K_J_Z(
    CMDIChildWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
}

// CMDIChildWndEx::OnSendIconicThumbnail(WPARAM, LPARAM) — retail mfc140u.dll
// entry RVA 0x860e0 (neither argument is read):
//     CDC dc;                              // vftable 0x33b510; m_hDC = m_hAttribDC = NULL
//     ::SendMessageW(m_hWnd, WM_PRINTCLIENT /*0x318*/, 0, PRF_CLIENT /*4*/);
//     CBitmap* pBmp = CGdiObject::FromHandle(                     // 0x2a3ea0
//         ::GetCurrentObject(dc.m_hAttribDC /*NULL*/, OBJ_BITMAP /*7*/));   // IAT 0x2c6150
//     <DwmSetIconicThumbnail loader at 0x1cae54>(m_hWnd, pBmp->m_hObject /*+0x8*/, 1);
//     if (dc.m_hDC) ::DeleteDC(dc.Detach());                      // 0x2a24d0, IAT 0x2c6148
// Same shape and same NULL dereference (at 0x8613f) as
// OnSendIconicLivePreviewBitmap above, and unreachable in practice for the same
// reason.  STUB -- left a no-op deliberately.
// Symbol: ?OnSendIconicThumbnail@CMDIChildWndEx@@IEAAX_K_J@Z
extern "C" void MS_ABI impl__OnSendIconicThumbnail_CMDIChildWndEx__IEAAX_K_J_Z(
    CMDIChildWndEx* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
}

// CMDIChildWndEx::OnSetIcon(BOOL, HICON) — transcribed from retail mfc140u.dll
// entry RVA 0x849f0 (neither argument is read; CWnd::Default re-dispatches
// the current message with its original wParam/lParam):
//     LRESULT lRes = CWnd::Default();                             // 0x28ac80
//     if (m_pMDIFrame != NULL)                                    // this+0x718
//         m_pMDIFrame->m_wndClientArea.UpdateTabs(FALSE);         // 0x7ea10 on frame+0x720
//     return (HICON)lRes;
// The Default() dispatch and its result are reproduced; the tab refresh runs
// on the owning frame's m_wndClientArea (frame+0x720), which
// core/frame/CMDIFrameWndEx.cpp does not construct, so it is skipped even
// though m_pMDIFrame itself is in ChildExtra.
// Symbol: ?OnSetIcon@CMDIChildWndEx@@IEAAPEAUHICON__@@HPEAU2@@Z
extern "C" HICON MS_ABI impl__OnSetIcon_CMDIChildWndEx__IEAAPEAUHICON____HPEAU2__Z(
    CMDIChildWndEx* pThis, int bIsLarge, HICON hIcon) {
    (void)bIsLarge;
    (void)hIcon;
    if (!pThis) {
        return nullptr;
    }
    const __int64 lRes = impl__Default_CWnd__IEAA_JXZ(pThis);
    // TODO(clean-room): retail then runs CMDIClientAreaWnd::UpdateTabs(FALSE)
    // on the owning frame's client area (this+0x718 -> +0x720); that frame
    // sub-object is not constructed in this tree.
    return reinterpret_cast<HICON>(lRes);
}

// CMDIChildWndEx::OnSetPreviewMode(BOOL, CPrintPreviewState*) — transcribed
// from retail mfc140u.dll entry RVA 0x84fa0:
//     CWnd* pMain = AfxGetModuleThreadState()->m_pCurrentWinThread->vtbl[0xf8]();  // 0x133a20, +0x8;
//                                                                 // slot 31 = CWinThread::GetMainWnd
//     if (pMain && pMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
//         ((CMDIFrameWndEx*)pMain)-><+0x220> = bPreview ? this : NULL;   // m_pPrintPreviewFrame
//     m_dockManager.SetPrintPreviewMode(bPreview, pState);        // 0x4d080 on this+0x3a8
//     DWORD dwSaved = pState-><+0x10>;                            // dwStates
//     CFrameWnd::OnSetPreviewMode(bPreview, pState);              // 0x29fe10
//     pState-><+0x10> = dwSaved;
//     this->vtbl[0x3a8](NULL);                                    // AdjustDockingLayout(NULL)
//     this->vtbl[0x300](TRUE);                                    // RecalcLayout(TRUE), tail
// Reproduced: the base OnSetPreviewMode, AdjustDockingLayout(NULL) and
// RecalcLayout(TRUE), the last two as direct calls to this class's exports.
// NOT reproduced: the main frame's m_pPrintPreviewFrame back-pointer (a
// CMDIFrameWndEx member OpenMFC does not model), the embedded
// CDockingManager's SetPrintPreviewMode (no dock manager at this+0x3a8 here)
// and the dwStates save/restore (CPrintPreviewState is an incomplete type in
// OpenMFC and its CFrameWnd::OnSetPreviewMode never touches pState) -- the
// same three omissions core/frame/CFrameWndEx.cpp makes for its twin.
// Symbol: ?OnSetPreviewMode@CMDIChildWndEx@@UEAAXHPEAUCPrintPreviewState@@@Z
extern "C" void MS_ABI impl__OnSetPreviewMode_CMDIChildWndEx__UEAAXHPEAUCPrintPreviewState___Z(
    CMDIChildWndEx* pThis, int bPreview, CPrintPreviewState* pState) {
    if (!pThis) {
        return;
    }
    // TODO(clean-room): the main frame's m_pPrintPreviewFrame write and
    // CDockingManager::SetPrintPreviewMode on this+0x3a8 are unmodeled.
    impl__OnSetPreviewMode_CFrameWnd__UEAAXHPEAUCPrintPreviewState___Z(pThis, bPreview, pState);
    impl__AdjustDockingLayout_CMDIChildWndEx__UEAAXPEAX_Z(pThis, nullptr);
    impl__RecalcLayout_CMDIChildWndEx__UEAAXH_Z(pThis, TRUE);
}
