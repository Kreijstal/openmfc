// CPaneDivider — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// The bodies marked "decoded from retail" below were transcribed from the retail
// export disassembly (disas.py). Two images were read: mfc140.dll (the ANSI twin)
// for the instruction stream, and mfc140u.dll -- the image OpenMFC reimplements --
// for every address quoted as an mfc140u RVA. Function bodies are byte-identical
// between the two, so control flow, member offsets and constants are the same;
// only the RVAs differ, and every RVA below names the image it belongs to.
//
// ---------------------------------------------------------------------------
// Retail CPaneDivider instance layout (mfc140u)
//
// Retail's CPaneDivider derives from CBasePane (sizeof 0x1a8) and is 0x218 bytes
// -- RuntimeClasses.cpp already records 536 as its m_nObjectSize. The member
// block was read out of ?Init@CPaneDivider@@QEAAXHPEAVCWnd@@@Z (entry RVA
// 0xab160 mfc140u / 0xab720 mfc140), which writes exactly these slots, and it
// matches the shipping atlmfc afxpanedivider.h member order:
//
//   +0x1a8  UINT   m_nID                    Init stores -1
//   +0x1ac  DWORD  m_dwDividerStyle         SS_HORZ = 1, SS_VERT = 2 (afxpanedivider.h)
//   +0x1b0  int    m_nWidth
//   +0x1b4  int    m_nMinOffset
//   +0x1b8  int    m_nMaxOffset
//   +0x1bc  int    m_nStep                  Init stores -1
//   +0x1c0  bool   m_bCaptured              a single byte (movb)
//   +0x1c4  BOOL   m_bDefaultDivider        Init stores its first argument
//   +0x1c8  BOOL   m_bAutoHideMode
//   +0x1cc  CRect  m_rectLastDragRect       Init ::SetRectEmpty's it
//   +0x1dc  CRect  m_rectDragBounds         Init ::SetRectEmpty's it
//   +0x1ec  CPoint m_ptHotTotal
//   +0x1f4  CPoint m_ptHotDrag              (+4 tail pad)
//   +0x200  CWnd*  m_pParentWndForSerialize Init stores its second argument
//   +0x208  CPaneContainerManager* m_pContainerManager
//   +0x210  CPaneTrackingWnd*      m_pWndTrack
//   sizeof == 0x218
//
// STORAGE -- deliberate, file-wide deviation, and the reason every body below
// keeps its divider-own state in a side table instead of at those offsets.
// The CPaneDivider this DLL actually builds is the one declared in
// include/openmfc/afxmfc.h: `class CPaneDivider : public CObject` plus a 16-byte
// pad, sizeof 24, allocated by docking/DynCreateFactories.cpp with
// `new CPaneDivider()`. A store at +0x1a8..+0x217 into that object is a heap
// overrun (docking/CPaneContainerManager.cpp's ?CreatePaneDivider@ comment
// records an earlier revision that did exactly that and was reverted). So the
// sixteen members above live in the file-local DividerState table keyed by
// `this` (see namespace below); a static_assert pins the retail size so the
// layout comment cannot drift silently. Moving the table back into the object
// is mechanical once the header carries retail storage -- see headerRequests.
//
// PRECONDITION shared by every entry point that touches the window: retail's
// CPaneDivider IS-A CWnd, and the bodies read CWnd::m_hWnd (+0x40) through the
// exported CWnd thunks. Those thunks are only ever reached on an object that
// really has CWnd storage in front of the divider block -- the message handlers
// because a WM_* only arrives for an HWND that maps to such an object, and
// CreateEx/Move/RepositionPanes/... because their retail callers
// (?CreatePaneDivider@CPaneContainerManager@@, ?CreateDefaultPaneDivider@
// CDockablePane@@) are both stubs in this tree and a client compiled against
// the shipping headers allocates the full 0x218 bytes. No body here can tell a
// 24-byte object from a retail one; the cast below (AsWnd/AsBasePane) documents
// the assumption rather than hiding it.
//
// VIRTUAL DISPATCH: OpenMFC publishes no CPaneDivider or CPaneContainerManager
// vftable (the manager object is built with a NULL vfptr, see
// ??0CPaneContainerManager@@ in CPaneContainerManager.cpp). Every `this->vslot`
// and `m_pContainerManager->vslot` call retail makes is therefore dispatched
// statically to the export that occupies that slot in the retail vftable
// (slot numbers are quoted from the mfc140u CPaneDivider vftable at
// 0x1802f6dd8 and the mfc140u CPaneContainerManager vftable at 0x1802f6258).
// A derived-class override would not run. Each site says so.
// ---------------------------------------------------------------------------

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMemDCSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Cross-class entry points. BRIEFING S1: the C++ methods do not exist inside
// this DLL, only these exports, so every call goes through the thunk. Each
// declaration below carries the retail-correct parameter list derived from the
// mangled name; every definition was located with grep before being declared
// (file:line as of this edit). Where the sibling definition is still an
// auto-generated placeholder whose list omits `this` or mistypes an argument
// (marked "placeholder"), calling it through the correct list is harmless under
// the MS x64 ABI -- those bodies read none of their arguments -- and the fix
// belongs in the sibling file (see headerRequests).
// ---------------------------------------------------------------------------

// core/window/Thunks.cpp:1181, :1501, :1566, :1311, :1656
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
// core/window/CWnd.cpp:590, :732, :758, :781, :856
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* lpParam);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
// core/window/CWnd.cpp:575 / :587 -- the CWnd pseudo-window statics; only their
// m_hWnd slot (HWND_BOTTOM / HWND_TOPMOST) is ever read, by the SetWindowPos thunk.
extern "C" const unsigned char impl__wndBottom_CWnd__2V1_B[];
extern "C" const unsigned char impl__wndTopMost_CWnd__2V1_B[];
// core/window/CtorDtorPlacement.cpp:22 / :24 -- placement ctor/dtor used to
// build the tracking window (see OnLButtonDown); detail/MemcoreSupport.cpp:5 /
// :12 -- MFC's exported operator new / delete (??2@YAPEAX_K@Z / ??3@YAXPEAX@Z),
// the pair retail's `new CPaneTrackingWnd` and `delete this` resolve to.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);
// core/runtime/CRuntimeClass.cpp:25, core/runtime/CObject.cpp:49
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
// core/runtime/Globals.cpp:339, core/runtime/AFX_GLOBAL_DATA.cpp:14 / :98,
// featurepack/CMFC_misc_stubs.cpp:3646 (720-byte zeroed block), :711
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);
extern "C" void MS_ABI impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* pRet, void* pThis, const wchar_t* lpszClassNamePrefix);
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);   // placeholder: no `this`
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
// core/runtime/StaticData.cpp:60 / :62 -- both NULL, nothing initialises them.
extern "C" void* impl__m_pContainerManagerRTC_CPaneDivider__2PEAUCRuntimeClass__EA;
// docking/StaticData.cpp:76 -- ?m_bHandleMinSize@CPane@@2HA
extern "C" std::int32_t impl__m_bHandleMinSize_CPane__2HA;
// core/runtime/StaticData.cpp:55 -- ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA,
// mirrored from the C++ instance by detail/MfccoreSupport.cpp:606.
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
// featurepack/visualmanager/Thunks.cpp:1288
extern "C" void MS_ABI impl__OnDrawPaneDivider_CMFCVisualManager__UEAAXPEAVCDC__PEAVCPaneDivider__VCRect__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CPaneDivider* pSlider, CRect rect, int bAutoHideMode);
// core/gdi/CPaintDC.cpp:11 / :29; detail/CMemDCSupport.h:151 / :155
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// core/runtime/Thunks.cpp:1172 / :1182; detail/MfcExceptionsSupport.cpp:618
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* lpBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* lpBuf, unsigned int nMax);
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName);
// core/collections/CObList.cpp:10 / :14 / :31
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(
    const CObList* pThis, long long nIndex);
// docking/RuntimeClasses.cpp:262 / :268, core/frame/CFrameWnd.cpp:47,
// docking/CDockablePane.cpp:450
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneContainerManager__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
// docking/CBasePane.cpp:841, :1171, :419, :1428 (placeholder), :1436 (placeholder)
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
    CBasePane* pThis, const RECT* lpRect, int bRepaint, void* hdwp);
extern "C" void MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(CBasePane* pThis, void* hdwp);
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(
    CBasePane* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide,
    CBasePane* pBarReplacement);
extern "C" void MS_ABI impl__Serialize_CBasePane__UEAAXAEAVCArchive___Z(CBasePane* pThis, CArchive& ar);
// docking/CDockablePane.cpp:3055
extern "C" void MS_ABI impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(
    CDockablePane* pThis, HWND hDivider);
// docking/CRecentDockSiteInfo.cpp:64 (placeholder)
extern "C" void MS_ABI impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
    void* pThis, void* pRecentContainer, void* pTabbedBar);
// docking/CPaneContainer.cpp:1169, :2015, :1449, :1954
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(void* pThis, unsigned int nID);
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__OnShowPane_CPaneContainer__UEAAXPEAVCDockablePane__H_Z(void* pThis, void* pPane, int bShow);
// docking/CPaneContainerManager.cpp -- the manager entry points the retail
// bodies reach through the manager vftable (slot offsets in the comments below):
//   :319 ??1 (destructor; erases the manager's side tables, the tree's whole
//   destructor -- what ?OnNcDestroy@'s `delete m_pContainerManager` runs),
//   :938 Create, :1869 RemovePaneDivider, :1544 OnPaneDividerMove,
//   :2033 ResizePaneContainers(CRect), :1263 GetMinSize, :534 AddPanesToList,
//   :2172 StoreRecentDockSiteInfo, :1627 OnShowPane, :348 FindPaneContainer,
//   :1386 InsertPane, :1972 ReplacePane, :2135 SetResizeMode,
//   :1074 DoesAllowDynInsertBefore, :1100 DoesContainFloatingPane, :1410 IsEmpty,
//   placeholders: :1252 GetMinMaxOffset, :460 / :484 AddPaneContainerManager,
//   :647 AddPaneToRecentPaneContainer, :2086 Serialize.
extern "C" int MS_ABI impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(
    void* pThis, void* pParentWnd, void* pDefaultSlider, CRuntimeClass* pContainerRTC);
extern "C" void* MS_ABI impl___1CPaneContainerManager__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__RemovePaneDivider_CPaneContainerManager__UEAAXPEAVCPaneDivider___Z(void* pThis, void* pSlider);
extern "C" int MS_ABI impl__OnPaneDividerMove_CPaneContainerManager__UEAAHPEAVCPaneDivider__IHAEAPEAX_Z(
    void* pThis, void* pSlider, unsigned int uFlags, int nOffset, void** phdwp);
extern "C" void MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(
    void* pThis, const RECT* pRect, void** phdwp);
extern "C" void MS_ABI impl__GetMinSize_CPaneContainerManager__UEAAXAEAVCSize___Z(void* pThis, CSize& size);
extern "C" void MS_ABI impl__AddPanesToList_CPaneContainerManager__QEAAXPEAVCObList__0_Z(
    void* pThis, void* plstControlBars, void* plstSliders);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(void* pThis, void* pBar);
extern "C" int MS_ABI impl__OnShowPane_CPaneContainerManager__UEAAHPEAVCDockablePane__H_Z(void* pThis, void* pBar, int bShow);
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pbLeftBar);
extern "C" int MS_ABI impl__InsertPane_CPaneContainerManager__UEAAHPEAVCDockablePane__0KPEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    void* pThis, void* pBarToInsert, void* pTargetBar, unsigned long dwAlignment, const RECT* lpRect, int dockMethod);
extern "C" int MS_ABI impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(void* pThis, void* pBarOld, void* pBarNew);
extern "C" void MS_ABI impl__SetResizeMode_CPaneContainerManager__UEAAXH_Z(void* pThis, int bResize);
extern "C" int MS_ABI impl__DoesAllowDynInsertBefore_CPaneContainerManager__UEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__DoesContainFloatingPane_CPaneContainerManager__UEAAHXZ(void* pThis);
extern "C" int MS_ABI impl__IsEmpty_CPaneContainerManager__QEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__GetMinMaxOffset_CPaneContainerManager__UEAAXPEAVCPaneDivider__AEAH11_Z(
    void* pThis, void* pSlider, int* pnMinOffset, int* pnMaxOffset, int* pnStep);
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHAEAV1_H_Z(
    void* pThis, void* pSrcManager, int bOuterEdge);
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHPEAVCDockablePane__KAEAV1_H_Z(
    void* pThis, void* pTargetBar, unsigned long dwAlignment, void* pSrcManager, int bOuterEdge);
extern "C" void* MS_ABI impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(
    void* pThis, void* pBar, void* pRecentContainer);
extern "C" void MS_ABI impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(void* pThis, CArchive& ar);
// core/runtime/CGlobalUtils.cpp:104 / :106 / :157 (placeholder)
extern "C" void* MS_ABI impl___0CGlobalUtils__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CGlobalUtils__UEAA_XZ(CGlobalUtils* pThis);
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CGlobalUtils__QEAAXAEAVCPaneContainerManager__PEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void* pThis, void* pContainerManager, CWnd* pWndToDock, long long ptMouse, CRect* pRectResult,
    int* pbDrawTab, void** ppTargetBar);
// Same-file entry points, forward declared so the bodies below can reach them
// the way retail does through its own vftable (slot given at each call site).
extern "C" int MS_ABI impl__CheckVisibility_CPaneDivider__UEAAHXZ(void* pThis);
extern "C" void MS_ABI impl__Move_CPaneDivider__UEAAXAEAVCPoint__H_Z(void* pThis, POINT* pptOffset, int bAdjustLayout);
extern "C" void MS_ABI impl__StopTracking_CPaneDivider__MEAAXH_Z(void* pThis, int bMoveSlider);
extern "C" int MS_ABI impl__CreateEx_CPaneDivider__UEAAHKKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    void* pThis, unsigned long dwStyleEx, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd,
    unsigned int nID, CCreateContext* pContext);
extern "C" void MS_ABI impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(void* pThis, int bDefaultDivider, CWnd* pParent);

namespace {

// --- the divider-own member block (see the layout comment at the top) --------
struct DividerState {
    UINT   nID              = static_cast<UINT>(-1);   // +0x1a8
    DWORD  dwDividerStyle   = 0;                        // +0x1ac
    int    nWidth           = 0;                        // +0x1b0
    int    nMinOffset       = 0;                        // +0x1b4
    int    nMaxOffset       = 0;                        // +0x1b8
    int    nStep            = -1;                       // +0x1bc
    bool   bCaptured        = false;                    // +0x1c0
    BOOL   bDefaultDivider  = FALSE;                    // +0x1c4
    BOOL   bAutoHideMode    = FALSE;                    // +0x1c8
    RECT   rectLastDragRect = {0, 0, 0, 0};             // +0x1cc
    RECT   rectDragBounds   = {0, 0, 0, 0};             // +0x1dc
    POINT  ptHotTotal       = {0, 0};                   // +0x1ec
    POINT  ptHotDrag        = {0, 0};                   // +0x1f4
    CWnd*  pParentWndForSerialize = nullptr;            // +0x200
    void*  pContainerManager = nullptr;                 // +0x208  CPaneContainerManager*
    CWnd*  pWndTrack        = nullptr;                  // +0x210  CPaneTrackingWnd* (built as a bare CWnd here, see OnLButtonDown)
};
constexpr size_t kSizeCPaneDivider = 0x218;
constexpr size_t kOffFirstOwnMember = 0x1a8;
static_assert(sizeof(CBasePane) == kOffFirstOwnMember, "retail CPaneDivider members start right after CBasePane (0x1a8)");
static_assert(kOffFirstOwnMember + 0x70 == kSizeCPaneDivider, "retail CPaneDivider is 0x218 bytes");

std::mutex& StateMutex() {
    static std::mutex s_mutex;
    return s_mutex;
}
std::unordered_map<const void*, DividerState>& States() {
    static std::unordered_map<const void*, DividerState> s_states;
    return s_states;
}
// Copy-out / copy-in accessors. The mutex is never held across a thunk call:
// several of the manager entry points call back into this file
// (?RemovePaneDivider@ -> ?NotifyAboutRelease@, for one), and std::mutex is not
// recursive.
DividerState LoadState(const void* pThis) {
    std::lock_guard<std::mutex> lock(StateMutex());
    return States()[pThis];
}
template <class F> void WithState(const void* pThis, F f) {
    std::lock_guard<std::mutex> lock(StateMutex());
    f(States()[pThis]);
}
void EraseState(const void* pThis) {
    std::lock_guard<std::mutex> lock(StateMutex());
    States().erase(pThis);
}

// The `this` casts. Retail's CPaneDivider is a CBasePane, whose CWnd subobject
// sits at offset 0; OpenMFC's header says CObject, so the cast is a
// reinterpret_cast and is only valid under the PRECONDITION in the file header.
inline CWnd* AsWnd(void* pThis) { return reinterpret_cast<CWnd*>(pThis); }
inline CBasePane* AsBasePane(void* pThis) { return reinterpret_cast<CBasePane*>(pThis); }
inline HWND HwndOf(void* pThis) { return AsWnd(pThis)->m_hWnd; }   // CWnd::m_hWnd, +0x40

template <class T> inline T& Field(void* p, size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}

// CPaneDivider::SS_STYLE (afxpanedivider.h): SS_HORZ = 0x1, SS_VERT = 0x2.
constexpr DWORD kStyleHorz = 0x1;
constexpr DWORD kStyleVert = 0x2;
// CBRS_ALIGN_* as the shipping afxres.h spells them and as retail
// ?RepositionPanes@ compares them (0x1000/0x2000/0x4000/0x8000). Not taken from
// include/openmfc/afxole.h, whose CBRS_ALIGN_ANY is 0x000F.
constexpr unsigned long kCbrsAlignLeft   = 0x1000UL;
constexpr unsigned long kCbrsAlignTop    = 0x2000UL;
constexpr unsigned long kCbrsAlignRight  = 0x4000UL;
constexpr unsigned long kCbrsAlignBottom = 0x8000UL;
// MFS_SYNCACTIVE (afxwin.h mini-frame style), OR'ed into the tracking window's
// WS_POPUP by retail ?OnLButtonDown@ (`movl $0x80000100,0x20(%rsp)`).
constexpr DWORD kMfsSyncActive = 0x0100;
// AFX_IDC_HSPLITBAR / AFX_IDC_VSPLITBAR (afxres.h: 30980 / 30981), the two
// cursor resource ids retail ?OnCreate@ loads (`mov $0x7904,%ebx` / `$0x7905`).
constexpr WORD kIdcHSplitBar = 30980;
constexpr WORD kIdcVSplitBar = 30981;

// CPaneContainerManager::m_pRootContainer, +0x78 (the layout pinned in
// docking/CPaneContainerManager.cpp).
constexpr size_t kOffManagerRootContainer = 0x78;
// CDockablePane: m_recentDockInfo (+0x2b8, CPane) . m_recentSliderInfo (+0x48,
// per docking/CPane.cpp's CRecentDockSiteInfo layout) . m_pRecentBarContainer
// (+0x58) / m_pRecentContainerOfTabWnd (+0x60), the two fields of
// detail/CRecentPaneContainerInfoSupport.h's S_Crecentpanecontainerinfo at 88 /
// 96. Retail ?AddRecentPane@ reads them as pBar+0x358 and pBar+0x360.
constexpr size_t kOffRecentSliderContainer = 0x358;
constexpr size_t kOffRecentTabContainer    = 0x360;
static_assert(sizeof(CPane) > kOffRecentTabContainer, "the two CRecentPaneContainerInfo pointers lie inside CPane");
// (0x2b8 is the offset include/openmfc/afxmfc.h documents for CPane::m_recentDockInfo;
// 0x2b8 + 0x48 + 88 == 0x358 and 0x2b8 + 0x48 + 96 == 0x360.)

// afxGlobalData slots. +0x000 is the one-time init gate every retail reader
// tests (`cmpl $0,afxGlobalData; jne; call Initialize; movl $1,afxGlobalData`);
// +0x158 / +0x160 are the two HCURSORs retail ?OnCreate@ fills and
// ?OnSetCursor@ hands to USER32!SetCursor: mfc140.dll reads them at
// 0x1803ba4d8 / 0x1803ba4e0 against ?afxGlobalData@@ at 0x1803ba380, i.e.
// m_hcurStretch / m_hcurStretchVert in the afxglobals.h declaration order
// (+0x158 is the slot toolbar/CMFCToolBar.cpp names kGlobalDataCursorStretch;
// +0x160 is named nowhere else in the tree and rests on the two stores in
// ?OnCreate@ alone).
constexpr int kGlobalDataInitGate       = 0x000;
constexpr int kGlobalDataCurStretch     = 0x158;
constexpr int kGlobalDataCurStretchVert = 0x160;
inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline HCURSOR GlobalDataCursor(int off) { HCURSOR h; std::memcpy(&h, GlobalDataBytes() + off, sizeof h); return h; }
inline void SetGlobalDataCursor(int off, HCURSOR h) { std::memcpy(GlobalDataBytes() + off, &h, sizeof h); }
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
// CWinApp::LoadCursor(UINT) as retail inlines it in ?OnCreate@:
// ::LoadCursorW(AfxFindResourceHandle(MAKEINTRESOURCE(id), RT_GROUP_CURSOR), MAKEINTRESOURCE(id)).
inline HCURSOR LoadAfxCursor(WORD id) {
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
        MAKEINTRESOURCEW(id), MAKEINTRESOURCEW(12) /* RT_GROUP_CURSOR */);
    return ::LoadCursorW(hInst, MAKEINTRESOURCEW(id));
}

// DYNAMIC_DOWNCAST, the way the retail bodies open-code it:
// `if (p != NULL && p->IsKindOf(pRTC)) use p; else use NULL;`.
void* DynDowncast(void* p, CRuntimeClass* pRTC) {
    if (p == nullptr || pRTC == nullptr) return nullptr;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(p), pRTC) ? p : nullptr;
}

// CWnd::FromHandle(::GetParent(m_hWnd)) -- the parent lookup retail repeats in
// ?Move@, ?RepositionPanes@ and ?Serialize@.
inline CWnd* ParentWndOf(void* pThis) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HwndOf(pThis)));
}

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h), the node
// a CObList::POSITION points at. GetHeadPosition / GetNext are inline in retail
// (no export, no thunk), so a CObList is walked from another translation unit
// through the exported FindIndex thunk plus this read-only view -- the pattern
// docking/CDockingPanesRow.cpp and visualmanager/CMFCVisualManager.cpp use.
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
inline ObNode* HeadNode(const CObList* pList) {
    ObNode* p = nullptr;
    CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0);
    std::memcpy(&p, &pos, sizeof p);
    return p;
}
// A stack CObList built and torn down through the exported ctor/dtor (retail
// constructs it in place with the CObList vftable; OpenMFC's C++ ctor is not
// reachable from here without a new C++ undefined).
struct StackObList {
    alignas(void*) unsigned char storage[sizeof(CObList)] = {};
    StackObList() { impl___0CObList__QEAA__J_Z(Get(), 10); }
    ~StackObList() { impl___1CObList__UEAA_XZ(Get()); }
    CObList* Get() { return reinterpret_cast<CObList*>(storage); }
    StackObList(const StackObList&) = delete;
    StackObList& operator=(const StackObList&) = delete;
};

// The tracking window. Retail's CPaneTrackingWnd is a CWnd with no members of
// its own (?OnLButtonDown@ allocates exactly sizeof(CWnd) == 0xe8 for it, runs
// ??0CWnd@@QEAA@XZ and stores the CPaneTrackingWnd vftable). OpenMFC declares
// no CPaneTrackingWnd class and publishes no vftable for it, so the object is
// built here as a bare CWnd through MFC's exported operator new (the
// `call ??2@YAPEAX_K@Z` retail makes, `mov $0xe8,%ecx`) and the placement ctor
// thunk, and destroyed through the placement dtor thunk plus MFC's exported
// operator delete -- what a retail deleting destructor ends with.
// DEVIATION that follows from it: WM_PAINT / WM_ERASEBKGND on that window go to
// CWnd's default handling rather than to ?OnPaint@CPaneTrackingWnd@@ /
// ?OnEraseBkgnd@CPaneTrackingWnd@@ (docking/CPaneTrackingWnd.cpp), so the drag
// rectangle is painted with the registered class brush instead of retail's
// tracking-window paint.
CWnd* NewTrackingWnd() {
    void* mem = impl___2_YAPEAX_K_Z(sizeof(CWnd));
    if (mem == nullptr) return nullptr;
    return static_cast<CWnd*>(impl___0CWnd__QEAA_XZ(mem));
}
void DeleteTrackingWnd(CWnd* pWnd) {
    if (pWnd == nullptr) return;
    impl___1CWnd__UEAA_XZ(pWnd);
    impl___3_YAXPEAX_Z(pWnd);
}
// `if (m_pWndTrack != NULL && m_pWndTrack->m_hWnd != NULL) { m_pWndTrack->
// DestroyWindow(); delete m_pWndTrack; m_pWndTrack = NULL; }` -- the block
// ?StopTracking@ and ?OnDestroy@ share (retail dispatches DestroyWindow through
// CWnd vftable +0xd0 and the delete through slot 1; both static here).
void DestroyTrackingWnd(void* pThis) {
    CWnd* pTrack = LoadState(pThis).pWndTrack;
    if (pTrack == nullptr || pTrack->m_hWnd == nullptr) return;
    impl__DestroyWindow_CWnd__UEAAHXZ(pTrack);
    pTrack = LoadState(pThis).pWndTrack;   // re-read: retail re-loads +0x210 after the call
    if (pTrack != nullptr) DeleteTrackingWnd(pTrack);
    WithState(pThis, [](DividerState& s) { s.pWndTrack = nullptr; });
}

} // namespace


// Retail ??0CPaneDivider@@QEAA@HPEAVCWnd@@@Z, entry RVA 0xab0e0 (mfc140u; 0xab6a0
// mfc140): runs the CBasePane constructor, stores the CPaneDivider vftable,
// zeroes the 48 bytes at +0x1cc..+0x1fb (m_rectLastDragRect, m_rectDragBounds,
// m_ptHotTotal, m_ptHotDrag -- six 8-byte stores) and calls
// Init(bDefaultDivider, pParent). The default constructor
// ??0CPaneDivider@@QEAA@XZ (entry RVA 0xab030 mfc140u; 0xab5f0 mfc140) is the
// same sequence with Init(FALSE, NULL).
// Only the Init call and the pre-existing side-table registration are done
// here: the CBasePane base constructor and the vftable store need the retail
// object shape (see the STORAGE note at the top), and the 48-byte zeroing is
// what DividerState's defaults already are.
// Symbol: ??0CPaneDivider@@QEAA@HPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CPaneDivider__QEAA_HPEAVCWnd___Z(void* pThis, int bDefaultDivider, void* pParent) {
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        g_dividerPanes[pThis] = {};
    }
    impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(pThis, bDefaultDivider, static_cast<CWnd*>(pParent));
    return pThis;
}
// Symbol: ??0CPaneDivider@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneDivider__QEAA_XZ(void* pThis) {
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        g_dividerPanes[pThis] = {};
    }
    impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(pThis, FALSE, nullptr);
    return pThis;
}
// Retail ??1CPaneDivider@@UEAA@XZ (entry RVA 0xab200 mfc140u) stores the
// vftable and tail-jumps to ??1CBasePane@@; the base destructor is not run
// here for the STORAGE reason above. Both side tables are released.
// Symbol: ??1CPaneDivider@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneDivider__UEAA_XZ(void* pThis) {
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        g_dividerPanes.erase(pThis);
    }
    EraseState(pThis);
    return pThis;
}
// Symbol: ?AddPane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_dividerPanes[pThis];
    AddUniquePane(state, pPane);
    const int paneCount = static_cast<int>(state.panes.size());
    state.recentRect = CRect(0, 0, std::max(80, paneCount * kPaneDividerUnitWidth), kPaneDividerThickness);
}
// Symbol: ?RemovePane@CPaneDivider@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    PaneState& state = g_dividerPanes[pThis];
    RemovePane(state, pPane);
    if (state.panes.empty()) {
        state.recentRect.SetRectEmpty();
    } else {
        const int paneCount = static_cast<int>(state.panes.size());
        state.recentRect = CRect(0, 0, std::max(80, paneCount * kPaneDividerUnitWidth), kPaneDividerThickness);
    }
}
// Symbol: ?CheckVisibility@CPaneDivider@@UEAAHXZ
extern "C" int MS_ABI impl__CheckVisibility_CPaneDivider__UEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return g_dividerPanes[pThis].panes.empty() ? FALSE : TRUE;
}
// Symbol: ?FindPaneContainer@CPaneDivider@@QEAAPEAVCPaneContainer@@PEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneDivider__QEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pIndex) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)pThis;
    if (pIndex) *pIndex = 0;
    auto it = g_paneToContainer.find(pPane);
    return it == g_paneToContainer.end() ? nullptr : it->second;
}
// Symbol: ?GetFirstPane@CPaneDivider@@QEBAPEBVCBasePane@@XZ
extern "C" void* MS_ABI impl__GetFirstPane_CPaneDivider__QEBAPEBVCBasePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_dividerPanes[pThis]);
}
// Symbol: ?GetPaneDividers@CPaneDivider@@QEAAXAEAVCObList@@@Z
extern "C" void MS_ABI impl__GetPaneDividers_CPaneDivider__QEAAXAEAVCObList___Z(void* pThis, CObList* pList) {
    if (!pList) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pList->RemoveAll();
    const auto it = g_dividerPanes.find(pThis);
    if (it != g_dividerPanes.end() && pThis != nullptr) {
        pList->AddTail(reinterpret_cast<CObject*>(pThis));
    }
}
// Symbol: ?GetPanes@CPaneDivider@@QEAAXAEAVCObList@@@Z
extern "C" void MS_ABI impl__GetPanes_CPaneDivider__QEAAXAEAVCObList___Z(void* pThis, CObList* pList) {
    if (!pList) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    pList->RemoveAll();
    const auto it = g_dividerPanes.find(pThis);
    if (it == g_dividerPanes.end()) return;
    for (void* pane : it->second.panes) {
        if (pane != nullptr) {
            pList->AddTail(reinterpret_cast<CObject*>(pane));
        }
    }
}
// Symbol: ?GetRootContainerRect@CPaneDivider@@QEAA?AVCRect@@XZ
extern "C" void MS_ABI impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(CRect* pRet, void* pThis) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_dividerPanes[pThis];
    if (state.recentRect.Width() > 0 && state.recentRect.Height() > 0) {
        *pRet = state.recentRect;
    } else {
        pRet->SetRect(0, 0, state.panes.empty() ? 0 : 240, state.panes.empty() ? 0 : 140);
    }
}
// Symbol: ?CalcFixedLayout@CPaneDivider@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CPaneDivider__UEAA_AVCSize__HH_Z(CSize* pRet, void* pThis, int, int) {
    if (!pRet) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    const PaneState& state = g_dividerPanes[pThis];
    if (state.recentRect.Width() > 0 && state.recentRect.Height() > 0) {
        pRet->cx = state.recentRect.Width();
        pRet->cy = state.recentRect.Height();
        return;
    }
    const int paneCount = static_cast<int>(state.panes.size());
    pRet->cx = paneCount > 0 ? std::max(80, 80 * paneCount) : 0;
    pRet->cy = paneCount > 0 ? kPaneDividerThickness : 0;
}
void CPaneDivider::AddPane(CDockablePane* pPane) {
    impl__AddPane_CPaneDivider__UEAAXPEAVCDockablePane___Z(this, pPane);
}
void CPaneDivider::RemovePane(CDockablePane* pPane) {
    impl__RemovePane_CPaneDivider__UEAAXPEAVCDockablePane___Z(this, pPane);
}
BOOL CPaneDivider::CheckVisibility() {
    return impl__CheckVisibility_CPaneDivider__UEAAHXZ(this);
}
CPaneContainer* CPaneDivider::FindPaneContainer(CDockablePane* pBar, int& nIndex) {
    return reinterpret_cast<CPaneContainer*>(
        impl__FindPaneContainer_CPaneDivider__QEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(this, pBar, &nIndex));
}
const CBasePane* CPaneDivider::GetFirstPane() const {
    return reinterpret_cast<const CBasePane*>(
        impl__GetFirstPane_CPaneDivider__QEBAPEBVCBasePane__XZ(const_cast<CPaneDivider*>(this)));
}
void CPaneDivider::GetPaneDividers(CObList& lst) {
    impl__GetPaneDividers_CPaneDivider__QEAAXAEAVCObList___Z(this, &lst);
}
void CPaneDivider::GetPanes(CObList& lst) {
    impl__GetPanes_CPaneDivider__QEAAXAEAVCObList___Z(this, &lst);
}
CRect CPaneDivider::GetRootContainerRect() {
    CRect rect;
    impl__GetRootContainerRect_CPaneDivider__QEAA_AVCRect__XZ(&rect, this);
    return rect;
}
CSize CPaneDivider::CalcFixedLayout(BOOL bStretch, BOOL bHorz) {
    CSize size;
    impl__CalcFixedLayout_CPaneDivider__UEAA_AVCSize__HH_Z(&size, this, bStretch, bHorz);
    return size;
}

// ===========================================================================
// Bodies decoded from retail. Convention for the transcriptions below: the
// pseudo-code is what the retail instruction stream does; "vslot +0xNNN" names
// the vftable offset retail dispatches through (static here, see the header);
// "DEVIATION" marks anything this body does differently and why.
// ===========================================================================

// Symbol: ?Init@CPaneDivider@@QEAAXHPEAVCWnd@@@Z
// Decoded from retail ?Init@CPaneDivider@@QEAAXHPEAVCWnd@@@Z, entry RVA 0xab160
// (mfc140u; 0xab720 mfc140), straight-line:
//     m_nID = (UINT)-1;  m_dwDividerStyle = 0;  m_nWidth = 0;   // +0x1a8, movq $0 at +0x1ac
//     m_bCaptured = false;                                       // movb +0x1c0
//     m_pContainerManager = NULL;                                // +0x208
//     m_bDefaultDivider = bDefaultDivider;                       // +0x1c4 <- %edx
//     ::SetRectEmpty(&m_rectLastDragRect);                       // +0x1cc, IAT 0x1802c5368 (mfc140)
//     ::SetRectEmpty(&m_rectDragBounds);                         // +0x1dc
//     m_pWndTrack = NULL;                                        // +0x210
//     m_nMinOffset = 0;  m_nMaxOffset = 0;                       // movq $0 at +0x1b4
//     m_nStep = -1;                                              // +0x1bc
//     m_bAutoHideMode = FALSE;                                   // +0x1c8
//     m_pParentWndForSerialize = pParent;                        // +0x200 <- %r8
// m_ptHotTotal / m_ptHotDrag are not touched by Init (the constructors zero them).
extern "C" void MS_ABI impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(void* pThis, int bDefaultDivider, CWnd* pParent) {
    WithState(pThis, [&](DividerState& s) {
        s.nID = static_cast<UINT>(-1);
        s.dwDividerStyle = 0;
        s.nWidth = 0;
        s.bCaptured = false;
        s.pContainerManager = nullptr;
        s.bDefaultDivider = bDefaultDivider;
        ::SetRectEmpty(&s.rectLastDragRect);
        ::SetRectEmpty(&s.rectDragBounds);
        s.pWndTrack = nullptr;
        s.nMinOffset = 0;
        s.nMaxOffset = 0;
        s.nStep = -1;
        s.bAutoHideMode = FALSE;
        s.pParentWndForSerialize = pParent;
    });
}

// Symbol: ?AddPaneContainer@CPaneDivider@@UEAAHAEAVCPaneContainerManager@@H@Z
// Decoded from retail entry RVA 0xacc00 (mfc140u; 0xad1c0 mfc140):
//     BOOL bResult = FALSE;
//     if (m_pContainerManager != NULL) {                                   // +0x208
//         bResult = m_pContainerManager->AddPaneContainerManager(barContainerManager, bOuterEdge);   // mgr vslot +0x38
//         CheckVisibility();                                               // this vslot +0x530
//     }
//     return bResult;
// Manager vslot +0x38 is ?AddPaneContainerManager@CPaneContainerManager@@UEAAHAEAV1@H@Z,
// a placeholder in CPaneContainerManager.cpp (returns 0, reads nothing).
extern "C" int MS_ABI impl__AddPaneContainer_CPaneDivider__UEAAHAEAVCPaneContainerManager__H_Z(
    void* pThis, void* pBarContainerManager, int bOuterEdge) {
    int bResult = FALSE;
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        bResult = impl__AddPaneContainerManager_CPaneContainerManager__UEAAHAEAV1_H_Z(
            pManager, pBarContainerManager, bOuterEdge);
        impl__CheckVisibility_CPaneDivider__UEAAHXZ(pThis);
    }
    return bResult;
}

// Symbol: ?AddPaneContainer@CPaneDivider@@UEAAHPEAVCDockablePane@@AEAVCPaneContainerManager@@K@Z
// Decoded from retail entry RVA 0xacc50 (mfc140u; 0xad210 mfc140):
//     BOOL bResult = FALSE;
//     if (m_pContainerManager != NULL) {
//         bResult = m_pContainerManager->AddPaneContainerManager(
//                       pTargetBar, dwAlignment, barContainerManager, TRUE);   // mgr vslot +0x30, `movl $1,0x20(%rsp)`
//         CheckVisibility();                                                  // this vslot +0x530
//     }
//     return bResult;
// Manager vslot +0x30 is
// ?AddPaneContainerManager@CPaneContainerManager@@UEAAHPEAVCDockablePane@@KAEAV1@H@Z,
// a placeholder in CPaneContainerManager.cpp (returns 0, reads nothing).
extern "C" int MS_ABI impl__AddPaneContainer_CPaneDivider__UEAAHPEAVCDockablePane__AEAVCPaneContainerManager__K_Z(
    void* pThis, void* pTargetBar, void* pBarContainerManager, unsigned long dwAlignment) {
    int bResult = FALSE;
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        bResult = impl__AddPaneContainerManager_CPaneContainerManager__UEAAHPEAVCDockablePane__KAEAV1_H_Z(
            pManager, pTargetBar, dwAlignment, pBarContainerManager, TRUE);
        impl__CheckVisibility_CPaneDivider__UEAAHXZ(pThis);
    }
    return bResult;
}

// Symbol: ?AddRecentPane@CPaneDivider@@UEAAPEAVCDockablePane@@PEAV2@@Z
// Decoded from retail entry RVA 0xac250 (mfc140u; 0xac810 mfc140):
//     CDockablePane* pResult;
//     CPaneContainer* pRecent = pBar->m_recentDockInfo.m_recentSliderInfo.m_pRecentBarContainer;   // pBar+0x358
//     if (pRecent != NULL) {
//         pResult = m_pContainerManager->AddPaneToRecentPaneContainer(pBar, pRecent);   // mgr vslot +0x50
//     } else {
//         pRecent = pBar->m_recentDockInfo.m_recentSliderInfo.m_pRecentContainerOfTabWnd;   // pBar+0x360
//         if (pRecent == NULL) return NULL;                     // CheckVisibility is NOT called on this path
//         pResult = m_pContainerManager->AddPaneToRecentPaneContainer(pBar, pRecent);
//     }
//     CheckVisibility();                                        // this vslot +0x530
//     return pResult;
// Retail dereferences m_pContainerManager unchecked on both call paths.
// Manager vslot +0x50 is ?AddPaneToRecentPaneContainer@CPaneContainerManager@@,
// a placeholder in CPaneContainerManager.cpp (returns NULL, reads nothing).
// DEVIATION: a NULL m_pContainerManager returns NULL instead of faulting.
extern "C" void* MS_ABI impl__AddRecentPane_CPaneDivider__UEAAPEAVCDockablePane__PEAV2__Z(void* pThis, void* pBar) {
    if (pBar == nullptr) return nullptr;   // deviation: retail reads pBar+0x358 unchecked
    void* pRecent = Field<void*>(pBar, kOffRecentSliderContainer);
    if (pRecent == nullptr) {
        pRecent = Field<void*>(pBar, kOffRecentTabContainer);
        if (pRecent == nullptr) return nullptr;
    }
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager == nullptr) return nullptr;   // deviation: retail would fault here
    void* pResult = impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(
        pManager, pBar, pRecent);
    impl__CheckVisibility_CPaneDivider__UEAAHXZ(pThis);
    return pResult;
}

// Symbol: ?CalcExpectedDockedRect@CPaneDivider@@UEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
// Decoded from retail entry RVA 0xace40 (mfc140u; 0xad400 mfc140):
//     CGlobalUtils utils;                       // stack: vftable + one zeroed 8-byte member
//     if (m_pContainerManager != NULL)
//         utils.CalcExpectedDockedRect(*m_pContainerManager, pWndToDock, ptMouse,
//                                      rectResult, bDrawTab, ppTargetBar);   // 0x6d1a0 mfc140u
// (ptMouse is the 8-byte CPoint in %r8, forwarded in %r9; the two stack
// arguments move from [rsp+0x80]/[rsp+0x88] to [rsp+0x28]/[rsp+0x30].)
// ?CalcExpectedDockedRect@CGlobalUtils@@ is a placeholder in
// core/runtime/CGlobalUtils.cpp:157 (empty body), so today this leaves
// rectResult / bDrawTab / *ppTargetBar untouched, exactly as that stub does.
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CPaneDivider__UEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    void* pThis, CWnd* pWndToDock, long long ptMouse, CRect* pRectResult, int* pbDrawTab, void** ppTargetBar) {
    alignas(void*) unsigned char utilsStorage[sizeof(CGlobalUtils)] = {};
    CGlobalUtils* pUtils = static_cast<CGlobalUtils*>(impl___0CGlobalUtils__QEAA_XZ(utilsStorage));
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        impl__CalcExpectedDockedRect_CGlobalUtils__QEAAXAEAVCPaneContainerManager__PEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
            pUtils, pManager, pWndToDock, ptMouse, pRectResult, pbDrawTab, ppTargetBar);
    }
    impl___1CGlobalUtils__UEAA_XZ(pUtils);
}

// Symbol: ?Create@CPaneDivider@@UEAAHKAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
// Decoded from retail entry RVA 0xab220 (mfc140u; 0xab7e0 mfc140): a direct
// (non-virtual) `return CreateEx(0, dwStyle, rect, pParentWnd, nID, pContext);`
// -- the call target is ?CreateEx@CPaneDivider@@ itself, not the vftable.
extern "C" int MS_ABI impl__Create_CPaneDivider__UEAAHKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    void* pThis, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID, CCreateContext* pContext) {
    return impl__CreateEx_CPaneDivider__UEAAHKKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
        pThis, 0, dwStyle, rect, pParentWnd, nID, pContext);
}

// Symbol: ?CreateEx@CPaneDivider@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
// Decoded from retail entry RVA 0xab250 (mfc140u; 0xab810 mfc140):
//     m_nID = nID;                                                   // +0x1a8
//     m_dwDividerStyle = dwStyle;                                    // +0x1ac
//     if (dwStyle & SS_VERT)      m_nWidth = rect.right - rect.left; // +0x1b0
//     else if (dwStyle & SS_HORZ) m_nWidth = rect.bottom - rect.top;
//     if (m_bDefaultDivider) {                                       // +0x1c4
//         CRuntimeClass* pRTC = CPaneDivider::m_pContainerManagerRTC; // 0x3b1b18 mfc140u
//         if (pRTC == NULL) AfxThrowInvalidArgException();           // 0xab9c9 (mfc140) -> 0x225b80
//         m_pContainerManager = DYNAMIC_DOWNCAST(CPaneContainerManager, pRTC->CreateObject());
//         if (m_pContainerManager == NULL) AfxThrowInvalidArgException();
//         m_pContainerManager->Create(pParentWnd, this, NULL);       // mgr vslot +0x28
//     }
//     dwStyle |= WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;        // or $0x46000000
//     m_pDockSite = DYNAMIC_DOWNCAST(CFrameWnd, pParentWnd);         // CBasePane +0x110; RTC 0x338990 (mfc140) reads "CFrameWnd"
//     if (m_pDockSite == NULL) m_pDockSite = AFXGetParentFrame(pParentWnd);   // 0x6bd00 mfc140u
//     <afxGlobalData init gate>
//     CString strClassName = afxGlobalData.RegisterWindowClass(_T("Afx:Slider"));   // 0x33cf78 (mfc140) "Afx:Slider"
//     BOOL bResult = CWnd::CreateEx(dwStyleEx, strClassName, NULL, dwStyle, rect,
//                                   pParentWnd, nID, pContext);      // direct call to ?CreateEx@CWnd@@ (LPVOID overload), NOT the vftable
//     return bResult;                                                // after strClassName's dtor
// DEVIATIONS:
//  * The default-divider arm returns FALSE instead of throwing. In this tree
//    ?m_pContainerManagerRTC@CPaneDivider@@ is NULL (core/runtime/StaticData.cpp:60,
//    nothing initialises it) and ?CreateObject@CPaneContainerManager@@ returns
//    NULL by design (CPaneContainerManager.cpp:969), so retail's two ENSURE-style
//    throws would fire on every default divider; FALSE is the failure its retail
//    callers (?CreateDefaultPaneDivider@CDockablePane@@) already test for. The
//    RTC read and the downcast are kept so the arm becomes live the moment the
//    static and the factory are.
//  * m_pDockSite is a real CBasePane member here (afxmfc.h pins +0x110), so that
//    store is made on the object -- the one place this file writes into the
//    CBasePane block; it is inside the file-header PRECONDITION.
extern "C" int MS_ABI impl__CreateEx_CPaneDivider__UEAAHKKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
    void* pThis, unsigned long dwStyleEx, unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd,
    unsigned int nID, CCreateContext* pContext) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    WithState(pThis, [&](DividerState& s) {
        s.nID = nID;
        s.dwDividerStyle = static_cast<DWORD>(dwStyle);
        if (dwStyle & kStyleVert)      s.nWidth = rect.right - rect.left;
        else if (dwStyle & kStyleHorz) s.nWidth = rect.bottom - rect.top;
    });
    if (LoadState(pThis).bDefaultDivider) {
        CRuntimeClass* pRTC = static_cast<CRuntimeClass*>(impl__m_pContainerManagerRTC_CPaneDivider__2PEAUCRuntimeClass__EA);
        if (pRTC == nullptr) return FALSE;   // deviation: retail AfxThrowInvalidArgException()
        void* pManager = DynDowncast(impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pRTC),
                                     impl__GetThisClass_CPaneContainerManager__SAPEAUCRuntimeClass__XZ());
        WithState(pThis, [&](DividerState& s) { s.pContainerManager = pManager; });
        if (pManager == nullptr) return FALSE;   // deviation: retail AfxThrowInvalidArgException()
        impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(
            pManager, pParentWnd, pThis, nullptr);
    }
    dwStyle |= WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    CWnd* pDockSite = static_cast<CWnd*>(DynDowncast(pParentWnd, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ()));
    if (pDockSite == nullptr) {
        pDockSite = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pParentWnd);
    }
    AsBasePane(pThis)->m_pDockSite = reinterpret_cast<CDockSite*>(pDockSite);
    EnsureGlobalDataInitialized();
    alignas(CString) unsigned char nameStorage[sizeof(CString)] = {};
    CString* pClassName = reinterpret_cast<CString*>(nameStorage);
    impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
        pClassName, GlobalDataBytes(), L"Afx:Slider");
    const int bResult = impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
        AsWnd(pThis), dwStyleEx, static_cast<const wchar_t*>(*pClassName), nullptr, dwStyle, rect,
        pParentWnd, nID, pContext);
    pClassName->~CString();
    return bResult;
}

// Symbol: ?DoesAllowDynInsertBefore@CPaneDivider@@UEBAHXZ
// Decoded from retail entry RVA 0xace10 (mfc140u; 0xad3d0 mfc140):
//     if (m_pContainerManager != NULL)
//         return m_pContainerManager->DoesAllowDynInsertBefore();   // mgr vslot +0xd8, tail-jump
//     return TRUE;
extern "C" int MS_ABI impl__DoesAllowDynInsertBefore_CPaneDivider__UEBAHXZ(const void* pThis) {
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        return impl__DoesAllowDynInsertBefore_CPaneContainerManager__UEBAHXZ(pManager);
    }
    return TRUE;
}

// Symbol: ?DoesContainFloatingPane@CPaneDivider@@UEAAHXZ
// Decoded from retail entry RVA 0xacdf0 (mfc140u; 0xad3b0 mfc140):
//     if (m_pContainerManager != NULL)
//         return m_pContainerManager->DoesContainFloatingPane();   // mgr vslot +0xd0, tail-jump
//     return FALSE;
extern "C" int MS_ABI impl__DoesContainFloatingPane_CPaneDivider__UEAAHXZ(void* pThis) {
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        return impl__DoesContainFloatingPane_CPaneContainerManager__UEAAHXZ(pManager);
    }
    return FALSE;
}

// Symbol: ?FindTabbedPane@CPaneDivider@@QEAAPEAVCDockablePane@@I@Z
// Decoded from retail entry RVA 0xab9e0 (mfc140u; 0xabfa0 mfc140):
//     if (m_pContainerManager != NULL && m_pContainerManager->m_pRootContainer != NULL)   // +0x208, mgr +0x78
//         return m_pContainerManager->m_pRootContainer->FindTabbedPane(nID);   // tail-jump to ?FindTabbedPane@CPaneContainer@@ (0xa7590 mfc140u)
//     return NULL;
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneDivider__QEAAPEAVCDockablePane__I_Z(void* pThis, unsigned int nID) {
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        void* pRoot = Field<void*>(pManager, kOffManagerRootContainer);
        if (pRoot != nullptr) {
            return impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(pRoot, nID);
        }
    }
    return nullptr;
}

// Symbol: ?InsertPane@CPaneDivider@@UEAAHPEAVCDockablePane@@0KPEBUtagRECT@@@Z
// Decoded from retail entry RVA 0xac2e0 (mfc140u; 0xac8a0 mfc140):
//     BOOL bResult = FALSE;
//     if (m_pContainerManager != NULL) {
//         bResult = m_pContainerManager->InsertPane(pBarToInsert, pTargetBar, dwAlignment,
//                                                   lpRect, DM_UNKNOWN /* 0, `mov %ebx,0x28(%rsp)` */);   // mgr vslot +0x58
//         CheckVisibility();                                                                             // this vslot +0x530
//     }
//     return bResult;
extern "C" int MS_ABI impl__InsertPane_CPaneDivider__UEAAHPEAVCDockablePane__0KPEBUtagRECT___Z(
    void* pThis, void* pBarToInsert, void* pTargetBar, unsigned long dwAlignment, const RECT* lpRect) {
    int bResult = FALSE;
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        bResult = impl__InsertPane_CPaneContainerManager__UEAAHPEAVCDockablePane__0KPEBUtagRECT__W4AFX_DOCK_METHOD___Z(
            pManager, pBarToInsert, pTargetBar, dwAlignment, lpRect, 0);
        impl__CheckVisibility_CPaneDivider__UEAAHXZ(pThis);
    }
    return bResult;
}

// Symbol: ?Move@CPaneDivider@@UEAAXAEAVCPoint@@H@Z
// Decoded from retail entry RVA 0xac090 (mfc140u; 0xac650 mfc140). bAdjustLayout
// (%r8d) is never read -- dead in retail.
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     DWORD dwExStyle = pParent->GetExStyle();                       // %esi
//     CRect rect; ::GetWindowRect(m_hWnd, &rect);
//     CWnd::FromHandle(::GetParent(m_hWnd))->ScreenToClient(&rect);  // the lookup is repeated
//     int nOffset;
//     if (m_dwDividerStyle & SS_VERT) {                              // +0x1ac, `test $2`
//         nOffset = (dwExStyle & WS_EX_LAYOUTRTL) ? -ptOffset.x : ptOffset.x;   // `bt $0x16,%esi ; cmovae`
//         ::OffsetRect(&rect, nOffset, 0);
//     } else if (m_dwDividerStyle & SS_HORZ) {
//         nOffset = ptOffset.y;
//         ::OffsetRect(&rect, 0, nOffset);
//     } else return;
//     HDWP hdwp = ::BeginDeferWindowPos(50);
//     if (m_pContainerManager != NULL)
//         m_pContainerManager->OnPaneDividerMove(this, 0, nOffset, hdwp);   // mgr vslot +0x60; hdwp by reference
//     ::EndDeferWindowPos(hdwp);                                    // re-read after the call
//     AdjustDockingLayout(NULL);                                    // this vslot +0x4e0
//     SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
//                  SWP_NOZORDER | SWP_NOACTIVATE, NULL);             // this vslot +0x480 (CBasePane::SetWindowPos)
// DEVIATION: vslot +0x480 is dispatched as the exported CWnd::SetWindowPos.
// The pExtra (HDWP) argument retail passes is NULL, and ?SetWindowPos@CBasePane@@
// (0xb620 mfc140u) with a NULL hdwp forwards its other six arguments to
// ?SetWindowPos@CWnd@@ (0x2a9a60 mfc140u) and returns NULL -- so the call made
// here is the one retail makes one frame deeper. Vslot +0x4e0 goes to the
// exported CBasePane::AdjustDockingLayout, which is an empty body in this tree.
extern "C" void MS_ABI impl__Move_CPaneDivider__UEAAXAEAVCPoint__H_Z(void* pThis, POINT* pptOffset, int bAdjustLayout) {
    (void)bAdjustLayout;   // dead in retail too
    if (pThis == nullptr || pptOffset == nullptr) return;   // deviation: retail has no NULL checks
    CWnd* pParent = ParentWndOf(pThis);
    const unsigned long dwExStyle = pParent != nullptr ? impl__GetExStyle_CWnd__QEBAKXZ(pParent) : 0UL;
    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(HwndOf(pThis), &rect);
    pParent = ParentWndOf(pThis);
    if (pParent != nullptr) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rect);
    const DWORD dwDividerStyle = LoadState(pThis).dwDividerStyle;
    int nOffset;
    if (dwDividerStyle & kStyleVert) {
        nOffset = (dwExStyle & WS_EX_LAYOUTRTL) ? -pptOffset->x : pptOffset->x;
        ::OffsetRect(&rect, nOffset, 0);
    } else if (dwDividerStyle & kStyleHorz) {
        nOffset = pptOffset->y;
        ::OffsetRect(&rect, 0, nOffset);
    } else {
        return;
    }
    void* hdwp = ::BeginDeferWindowPos(50);
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        impl__OnPaneDividerMove_CPaneContainerManager__UEAAHPEAVCPaneDivider__IHAEAPEAX_Z(pManager, pThis, 0, nOffset, &hdwp);
    }
    ::EndDeferWindowPos(static_cast<HDWP>(hdwp));
    impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(AsBasePane(pThis), nullptr);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(AsWnd(pThis), nullptr, rect.left, rect.top,
                                                rect.right - rect.left, rect.bottom - rect.top,
                                                SWP_NOZORDER | SWP_NOACTIVATE);
}

// Symbol: ?NotifyAboutRelease@CPaneDivider@@UEAAXXZ
// Decoded from retail entry RVA 0xacea0 (mfc140u; 0xad460 mfc140):
//     CPaneContainerManager* pMgr = m_pContainerManager;           // dereferenced unchecked
//     if (pMgr->m_lstControlBars.GetCount() != 0) return;          // mgr +0x20 (the CObList at +0x08, m_nCount)
//     if (pMgr->m_pRootContainer != NULL &&                        // mgr +0x78
//         pMgr->m_pRootContainer->GetTotalReferenceCount() != 0) return;   // 0xa78b0 mfc140u
//     RemovePaneFromDockManager(this, TRUE, FALSE, m_bAutoHideMode, NULL);   // CBasePane, 0xb8a0 mfc140: (pBar=this, bDestroy=1, bAdjustLayout=0, bAutoHide=+0x1c8, pBarReplacement=NULL)
// The +0x20 test is the same word ?IsEmpty@CPaneContainerManager@@ tests, so it
// is read through that export (which in this tree reads the side-table list
// rather than the raw word). ?RemovePaneFromDockManager@CBasePane@@ is a
// placeholder in CBasePane.cpp:1428 (empty body).
// DEVIATION: a NULL m_pContainerManager returns instead of faulting.
extern "C" void MS_ABI impl__NotifyAboutRelease_CPaneDivider__UEAAXXZ(void* pThis) {
    const DividerState s = LoadState(pThis);
    if (s.pContainerManager == nullptr) return;   // deviation: retail dereferences it
    if (!impl__IsEmpty_CPaneContainerManager__QEBAHXZ(s.pContainerManager)) return;
    void* pRoot = Field<void*>(s.pContainerManager, kOffManagerRootContainer);
    if (pRoot != nullptr && impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(pRoot) != 0) return;
    impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(
        AsBasePane(pThis), AsBasePane(pThis), TRUE, FALSE, s.bAutoHideMode, nullptr);
}

// Symbol: ?OnCancelMode@CPaneDivider@@IEAAXXZ
// Decoded from retail entry RVA 0xabe50 (mfc140u; 0xac410 mfc140):
//     StopTracking(FALSE);      // this vslot +0x568
//     CWnd::Default();          // tail-jump to ?Default@CWnd@@ (CWnd::OnCancelMode)
extern "C" void MS_ABI impl__OnCancelMode_CPaneDivider__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__StopTracking_CPaneDivider__MEAAXH_Z(pThis, FALSE);
    (void)impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Symbol: ?OnCreate@CPaneDivider@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Decoded from retail entry RVA 0xaca30 (mfc140u; 0xacff0 mfc140). lpCreateStruct
// is never read.
//     if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;     // ?Default@CWnd@@, `cmp $-1,%eax`
//     <afxGlobalData init gate>
//     if (afxGlobalData.m_hcurStretch == NULL)                 // +0x158
//         afxGlobalData.m_hcurStretch = AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR);       // 0x7904; AfxFindResourceHandle(id, RT_GROUP_CURSOR) + LoadCursorW
//     <init gate again>
//     if (afxGlobalData.m_hcurStretchVert == NULL)             // +0x160
//         afxGlobalData.m_hcurStretchVert = AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR);   // 0x7905
//     return 0;
// (Each LoadCursor is preceded by a call to ?AfxGetModuleState@@ whose result
// is overwritten unused -- consistent with the shipping source's
// `AfxGetApp()->LoadCursor(id)`, where CWinApp::LoadCursor is an inline that
// never touches `this`.)
extern "C" int MS_ABI impl__OnCreate_CPaneDivider__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;   // unused in retail too
    if (pThis == nullptr) return -1;   // deviation: retail has no NULL check
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis))) == -1) return -1;
    EnsureGlobalDataInitialized();
    if (GlobalDataCursor(kGlobalDataCurStretch) == nullptr) {
        HCURSOR h = LoadAfxCursor(kIdcHSplitBar);
        EnsureGlobalDataInitialized();
        SetGlobalDataCursor(kGlobalDataCurStretch, h);
    }
    EnsureGlobalDataInitialized();
    if (GlobalDataCursor(kGlobalDataCurStretchVert) == nullptr) {
        HCURSOR h = LoadAfxCursor(kIdcVSplitBar);
        EnsureGlobalDataInitialized();
        SetGlobalDataCursor(kGlobalDataCurStretchVert, h);
    }
    return 0;
}

// Symbol: ?OnDestroy@CPaneDivider@@IEAAXXZ
// Decoded from retail entry RVA 0xac930 (mfc140u; 0xacef0 mfc140):
//     if (m_pWndTrack != NULL && m_pWndTrack->m_hWnd != NULL) {   // +0x210, +0x40
//         m_pWndTrack->DestroyWindow();                            // CWnd vslot +0xd0
//         if (m_pWndTrack != NULL) delete m_pWndTrack;             // vslot 1 (deleting dtor, flag 1)
//         m_pWndTrack = NULL;
//     }
//     CWnd::OnDestroy();                                           // tail-jump to 0x289cc0 (mfc140)
extern "C" void MS_ABI impl__OnDestroy_CPaneDivider__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    DestroyTrackingWnd(pThis);
    impl__OnDestroy_CWnd__IEAAXXZ(AsWnd(pThis));
}

// Symbol: ?OnEraseBkgnd@CPaneDivider@@IEAAHPEAVCDC@@@Z
// This export has no RVA of its own in either map because the linker folded it:
// the WM_ERASEBKGND entry (msg 0x14) of CPaneDivider's message map (mfc140u
// AFX_MSGMAP at 0x1802f6c38, entries at 0x1802f6c50) points at 0x3a60, the
// shared `mov $0x1,%eax ; ret` body -- `return TRUE;`.
extern "C" int MS_ABI impl__OnEraseBkgnd_CPaneDivider__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// Symbol: ?OnLButtonDown@CPaneDivider@@IEAAXIVCPoint@@@Z
// Decoded from retail entry RVA 0xaba80 (mfc140u; 0xac040 mfc140). nFlags and
// point are never read.
//     if (!m_bCaptured) {                                                     // +0x1c0
//         m_bCaptured = true;
//         CWnd::FromHandle(::SetCapture(m_hWnd));                             // SetCapture(); result unused
//         CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//         m_pWndTrack = new CPaneTrackingWnd;                                 // operator new(0xe8) + ??0CWnd@@ + vftable; NULL on alloc failure
//         <afxGlobalData init gate>
//         CString strClassName = afxGlobalData.RegisterWindowClass(_T("Afx:TrackingWnd"));   // 0x33cf58 (mfc140)
//         if (m_pWndTrack->CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW /* 0x88 */, strClassName, _T(""),
//                                   WS_POPUP | MFS_SYNCACTIVE /* 0x80000100 */, rectWnd, this, 0, NULL)) {   // direct ?CreateEx@CWnd@@
//             m_pWndTrack->SetOwner(this);                                    // inline: m_hWndOwner (+0xa0) = m_hWnd
//             m_pWndTrack->ShowWindow(SW_SHOWNOACTIVATE /* 4 */);
//         }
//         m_rectLastDragRect = rectWnd;  m_rectDragBounds = rectWnd;          // +0x1cc, +0x1dc
//         if (m_pContainerManager != NULL) {
//             m_pContainerManager->GetMinMaxOffset(this, m_nMinOffset, m_nMaxOffset, m_nStep);   // mgr vslot +0x98 -> +0x1b4/+0x1b8/+0x1bc
//             if (IsHorizontal()) {                                           // this vslot +0x2d8: `m_dwDividerStyle & SS_HORZ`
//                 m_rectDragBounds.top    = rectWnd.top    + m_nMinOffset;    // +0x1e0
//                 m_rectDragBounds.bottom = rectWnd.bottom + m_nMaxOffset;    // +0x1e8
//             } else {
//                 m_rectDragBounds.left   = rectWnd.left   + m_nMinOffset;    // +0x1dc
//                 m_rectDragBounds.right  = rectWnd.right  + m_nMaxOffset;    // +0x1e4
//             }
//             m_pContainerManager->SetResizeMode(TRUE);                       // mgr vslot +0x110
//         }
//     }
//     CWnd::Default();                                                        // CWnd::OnLButtonDown
// Retail dereferences m_pWndTrack unchecked after the `new`.
// ?GetMinMaxOffset@CPaneContainerManager@@ is a placeholder in
// CPaneContainerManager.cpp:1252 (empty), so the three offsets keep their
// Init values until it is implemented. See NewTrackingWnd for the tracking
// window deviation.
extern "C" void MS_ABI impl__OnLButtonDown_CPaneDivider__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    (void)point;
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    if (!LoadState(pThis).bCaptured) {
        WithState(pThis, [](DividerState& s) { s.bCaptured = true; });
        (void)impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(HwndOf(pThis)));
        RECT rectWnd = {0, 0, 0, 0};
        ::GetWindowRect(HwndOf(pThis), &rectWnd);
        CWnd* pTrack = NewTrackingWnd();
        WithState(pThis, [&](DividerState& s) { s.pWndTrack = pTrack; });
        EnsureGlobalDataInitialized();
        alignas(CString) unsigned char nameStorage[sizeof(CString)] = {};
        CString* pClassName = reinterpret_cast<CString*>(nameStorage);
        impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
            pClassName, GlobalDataBytes(), L"Afx:TrackingWnd");
        if (pTrack != nullptr &&   // deviation: retail dereferences the allocation unchecked
            impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
                pTrack, WS_EX_TOPMOST | WS_EX_TOOLWINDOW, static_cast<const wchar_t*>(*pClassName), L"",
                WS_POPUP | kMfsSyncActive, rectWnd, AsWnd(pThis), 0, nullptr)) {
            // deviation: SetOwner(this) is retail's inline `m_hWndOwner = m_hWnd`
            // (CWnd +0xa0); OpenMFC's CWnd has no m_hWndOwner member, that byte
            // range is unmodelled padding, so the owner is not recorded.
            impl__ShowWindow_CWnd__QEAAHH_Z(pTrack, SW_SHOWNOACTIVATE);
        }
        pClassName->~CString();
        WithState(pThis, [&](DividerState& s) {
            s.rectLastDragRect = rectWnd;
            s.rectDragBounds = rectWnd;
        });
        void* pManager = LoadState(pThis).pContainerManager;
        if (pManager != nullptr) {
            DividerState cur = LoadState(pThis);
            impl__GetMinMaxOffset_CPaneContainerManager__UEAAXPEAVCPaneDivider__AEAH11_Z(
                pManager, pThis, &cur.nMinOffset, &cur.nMaxOffset, &cur.nStep);
            const bool bHorz = (cur.dwDividerStyle & kStyleHorz) != 0;
            WithState(pThis, [&](DividerState& s) {
                s.nMinOffset = cur.nMinOffset;
                s.nMaxOffset = cur.nMaxOffset;
                s.nStep = cur.nStep;
                if (bHorz) {
                    s.rectDragBounds.top    = rectWnd.top    + s.nMinOffset;
                    s.rectDragBounds.bottom = rectWnd.bottom + s.nMaxOffset;
                } else {
                    s.rectDragBounds.left   = rectWnd.left   + s.nMinOffset;
                    s.rectDragBounds.right  = rectWnd.right  + s.nMaxOffset;
                }
            });
            impl__SetResizeMode_CPaneContainerManager__UEAAXH_Z(pManager, TRUE);
        }
    }
    (void)impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Symbol: ?OnLButtonUp@CPaneDivider@@IEAAXIVCPoint@@@Z
// Decoded from retail entry RVA 0xabca0 (mfc140u; 0xac260 mfc140). nFlags and
// point are never read.
//     StopTracking(TRUE);                     // this vslot +0x568
//     ::Sleep(20);                            // KERNEL32!Sleep, `mov $0x14,%ecx`
//     CWnd::Default();                        // CWnd::OnLButtonUp
//     ::InvalidateRect(m_hWnd, NULL, TRUE);
//     ::UpdateWindow(m_hWnd);                 // tail-jump
extern "C" void MS_ABI impl__OnLButtonUp_CPaneDivider__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    (void)point;
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__StopTracking_CPaneDivider__MEAAXH_Z(pThis, TRUE);
    ::Sleep(20);
    (void)impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    ::InvalidateRect(HwndOf(pThis), nullptr, TRUE);
    ::UpdateWindow(HwndOf(pThis));
}

// Symbol: ?OnMouseMove@CPaneDivider@@IEAAXIVCPoint@@@Z
// Decoded from retail entry RVA 0xabd00 (mfc140u; 0xac2c0 mfc140). nFlags and
// point are never read: the position comes from ::GetCursorPos.
//     if (m_bCaptured) {                                              // +0x1c0
//         CRect rectNew = m_rectLastDragRect;                         // +0x1cc
//         CPoint ptCursor; ::GetCursorPos(&ptCursor);
//         int nHalf = m_nWidth / 2;                                   // +0x1b0, `cltd ; sub ; sar`
//         if (m_dwDividerStyle & SS_VERT) {                           // +0x1ac
//             rectNew.left  = ptCursor.x - nHalf;
//             rectNew.right = rectNew.left + m_nWidth;
//             if (rectNew.left < m_rectDragBounds.left) {             // +0x1dc
//                 rectNew.right = m_rectDragBounds.left + m_rectLastDragRect.Width();
//                 rectNew.left  = m_rectDragBounds.left;
//             }
//             if (rectNew.right > m_rectDragBounds.right) {           // +0x1e4
//                 rectNew.left  = m_rectDragBounds.right - m_rectLastDragRect.Width();
//                 rectNew.right = m_rectDragBounds.right;
//             }
//         } else {
//             int y = ptCursor.y - nHalf;
//             if (y < m_rectDragBounds.top) y = m_rectDragBounds.top;  // +0x1e0, `cmovge`
//             if (y + m_nWidth > m_rectDragBounds.bottom) {           // +0x1e8
//                 rectNew.bottom = m_rectDragBounds.bottom;
//                 rectNew.top    = m_rectDragBounds.bottom - m_nWidth;
//             } else {
//                 rectNew.top    = y;
//                 rectNew.bottom = y + m_nWidth;
//             }
//         }
//         if (m_pWndTrack != NULL && m_pWndTrack->m_hWnd != NULL)     // +0x210
//             m_pWndTrack->SetWindowPos(&CWnd::wndTopMost, rectNew.left, rectNew.top, -1, -1,
//                                       SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER /* 0x211 */);   // ?SetWindowPos@CWnd@@, wndTopMost at 0x3bc290 (mfc140)
//         m_rectLastDragRect = rectNew;
//     }
//     CWnd::Default();                                                // CWnd::OnMouseMove, tail-jump
extern "C" void MS_ABI impl__OnMouseMove_CPaneDivider__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    (void)point;
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    const DividerState s = LoadState(pThis);
    if (s.bCaptured) {
        RECT rectNew = s.rectLastDragRect;
        POINT ptCursor = {0, 0};
        ::GetCursorPos(&ptCursor);
        const int nHalf = s.nWidth / 2;
        if (s.dwDividerStyle & kStyleVert) {
            rectNew.left = ptCursor.x - nHalf;
            rectNew.right = rectNew.left + s.nWidth;
            if (rectNew.left < s.rectDragBounds.left) {
                rectNew.right = s.rectDragBounds.left + (s.rectLastDragRect.right - s.rectLastDragRect.left);
                rectNew.left = s.rectDragBounds.left;
            }
            if (rectNew.right > s.rectDragBounds.right) {
                rectNew.left = s.rectDragBounds.right - (s.rectLastDragRect.right - s.rectLastDragRect.left);
                rectNew.right = s.rectDragBounds.right;
            }
        } else {
            int y = ptCursor.y - nHalf;
            if (y < s.rectDragBounds.top) y = s.rectDragBounds.top;
            if (y + s.nWidth > s.rectDragBounds.bottom) {
                rectNew.bottom = s.rectDragBounds.bottom;
                rectNew.top = s.rectDragBounds.bottom - s.nWidth;
            } else {
                rectNew.top = y;
                rectNew.bottom = y + s.nWidth;
            }
        }
        if (s.pWndTrack != nullptr && s.pWndTrack->m_hWnd != nullptr) {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
                s.pWndTrack, reinterpret_cast<const CWnd*>(impl__wndTopMost_CWnd__2V1_B),
                rectNew.left, rectNew.top, -1, -1, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
        }
        WithState(pThis, [&](DividerState& st) { st.rectLastDragRect = rectNew; });
    }
    (void)impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Symbol: ?OnNcDestroy@CPaneDivider@@IEAAXXZ
// Decoded from retail entry RVA 0xac9a0 (mfc140u; 0xacf60 mfc140):
//     if (m_pContainerManager != NULL)
//         m_pContainerManager->RemovePaneDivider(this);          // mgr vslot +0x118
//     if (m_pContainerManager != NULL && m_bDefaultDivider) {    // +0x208 re-read, +0x1c4
//         delete m_pContainerManager;                            // mgr vslot 1 (deleting dtor, flag 1)
//         m_pContainerManager = NULL;
//     }
//     CWnd::OnNcDestroy();                                       // 0x289d80 (mfc140)
//     delete this;                                               // this vslot 1 (deleting dtor, flag 1), tail-jump
// Both `delete`s go through retail's vslot 1 (the scalar deleting destructor,
// flag 1): run the destructor, then operator delete. The manager object carries
// a NULL vfptr in this tree (see ??0CPaneContainerManager@@ in
// CPaneContainerManager.cpp), so neither dispatch can be virtual; each is made
// statically as the exported destructor thunk -- ??1CPaneContainerManager@@
// (CPaneContainerManager.cpp:319, the pattern CMultiPaneFrameWnd.cpp:542 uses)
// and ??1CPaneDivider@@ -- followed by MFC's exported operator delete
// (??3@YAXPEAX@Z), the pair ?OnLButtonDown@'s `new` is matched with. The
// manager can only be here when CreateEx's default-divider arm produced it from
// ?CreateObject@CRuntimeClass@@, i.e. heap-allocated, so the free is right;
// today that arm is dead (see CreateEx) and this branch never runs.
// DEVIATION: callers that keep a CPaneDivider on the stack or embedded --
// retail forbids that too -- would be freed twice by the `delete this`.
extern "C" void MS_ABI impl__OnNcDestroy_CPaneDivider__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        impl__RemovePaneDivider_CPaneContainerManager__UEAAXPEAVCPaneDivider___Z(pManager, pThis);
    }
    const DividerState s = LoadState(pThis);
    if (s.pContainerManager != nullptr && s.bDefaultDivider) {
        impl___1CPaneContainerManager__UEAA_XZ(s.pContainerManager);
        impl___3_YAXPEAX_Z(s.pContainerManager);
        WithState(pThis, [](DividerState& st) { st.pContainerManager = nullptr; });
    }
    impl__OnNcDestroy_CWnd__IEAAXXZ(AsWnd(pThis));
    impl___1CPaneDivider__UEAA_XZ(pThis);
    impl___3_YAXPEAX_Z(pThis);
}

// Symbol: ?OnPaint@CPaneDivider@@IEAAXXZ
// Decoded from retail entry RVA 0xabfb0 (mfc140u; 0xac570 mfc140):
//     CPaintDC dc(this);                                          // ??0CPaintDC@@
//     CMemDC memDC(dc, this);                                     // ??0CMemDC@@QEAA@AEAVCDC@@PEAVCWnd@@@Z
//     CDC& pDC = memDC.GetDC();                                   // inline: m_bMemDC ? m_dcMem : *m_dc  (`cmpl $0,0x60(%rsp) ; cmove`)
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     CMFCVisualManager::GetInstance()->OnDrawPaneDivider(&pDC, this, rectClient, m_bAutoHideMode);   // inline GetInstance (0x97f4 mfc140) then vslot +0x318; +0x1c8
//     memDC.~CMemDC();  dc.~CPaintDC();
// DEVIATION: retail's GetInstance (the afxvisualmanager.h inline, emitted as
// the non-exported helper at 0x97f4 mfc140) reads ?m_pVisManager@CMFCVisualManager@@
// and lazily creates the default manager when it is NULL (m_pRTIDefault->
// CreateObject() or new CMFCVisualManager, m_bAutoDestroy = TRUE, vslot +0x70).
// That creation exists in this tree only as the C++ static
// CMFCVisualManager::GetInstance() (visualmanager/CMFCVisualManager.cpp:715);
// there is no impl__ thunk for it, and a call to the C++ method from this
// translation unit is a new C++ undefined that the campaign's link audit
// (checkfile.sh) rejects -- the same trade-off toolbar/CMFCToolBar.cpp's
// DrawSeparator makes. So the draw happens only when a visual manager already
// exists (the exported mirror of that static, kept in sync by
// detail/MfccoreSupport.cpp:606) and is skipped otherwise. The BeginPaint /
// EndPaint pair always runs, so the update region is validated either way.
extern "C" void MS_ABI impl__OnPaint_CPaneDivider__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, AsWnd(pThis));
    alignas(void*) unsigned char memStorage[sizeof(S_Cmemdc)] = {};
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, pPaintDC, AsWnd(pThis));
    CDC* pDC = pMemDC->m_bMemDC ? reinterpret_cast<CDC*>(&pMemDC->m_dcMem) : static_cast<CDC*>(pMemDC->m_dc);
    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(HwndOf(pThis), &rectClient);
    CMFCVisualManager* pVisManager = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVisManager != nullptr) {   // deviation: retail creates the default manager here
        impl__OnDrawPaneDivider_CMFCVisualManager__UEAAXPEAVCDC__PEAVCPaneDivider__VCRect__H_Z(
            pVisManager, pDC, reinterpret_cast<CPaneDivider*>(pThis), CRect(rectClient),
            LoadState(pThis).bAutoHideMode);
    }
    impl___1CMemDC__UEAA_XZ(pMemDC);
    impl___1CPaintDC__UEAA_XZ(pPaintDC);
}

// Symbol: ?OnSetCursor@CPaneDivider@@IEAAHPEAVCWnd@@II@Z
// Decoded from retail entry RVA 0xaba00 (mfc140u; 0xabfc0 mfc140). pWnd and
// message are never read.
//     if (nHitTest != HTCLIENT) return (BOOL)CWnd::Default();     // tail-jump to ?Default@CWnd@@
//     if (m_dwDividerStyle & SS_HORZ) {                           // +0x1ac, `test $1`
//         <afxGlobalData init gate>
//         ::SetCursor(afxGlobalData.m_hcurStretchVert);           // +0x160
//     } else if (m_dwDividerStyle & SS_VERT) {
//         <afxGlobalData init gate>
//         ::SetCursor(afxGlobalData.m_hcurStretch);               // +0x158
//     }
//     return TRUE;
extern "C" int MS_ABI impl__OnSetCursor_CPaneDivider__IEAAHPEAVCWnd__II_Z(void* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)pWnd;
    (void)message;
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    if (nHitTest != HTCLIENT) {
        return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis)));
    }
    const DWORD dwDividerStyle = LoadState(pThis).dwDividerStyle;
    if (dwDividerStyle & kStyleHorz) {
        EnsureGlobalDataInitialized();
        ::SetCursor(GlobalDataCursor(kGlobalDataCurStretchVert));
    } else if (dwDividerStyle & kStyleVert) {
        EnsureGlobalDataInitialized();
        ::SetCursor(GlobalDataCursor(kGlobalDataCurStretch));
    }
    return TRUE;
}

// Symbol: ?OnShowPane@CPaneDivider@@UEAAXPEAVCDockablePane@@H@Z
// Decoded from retail entry RVA 0xaccb0 (mfc140u; 0xad270 mfc140):
//     if (m_pContainerManager == NULL) return;
//     if (IsAutoHideMode()) return;                                  // this vslot +0x3c8: `return m_bAutoHideMode` (+0x1c8)
//     BOOL bVisible = m_pContainerManager->OnShowPane(pBar, bShow);   // mgr vslot +0xa0
//     if (bShow) {
//         ShowWindow(SW_SHOW);                                        // ?ShowWindow@CWnd@@, direct
//         return;
//     }
//     ShowWindow(bVisible ? SW_SHOW : SW_HIDE);                       // `neg ; sbb ; and $5`
//     BOOL bLeft = bShow;   /* == FALSE */
//     CPaneContainer* pContainer = m_pContainerManager->FindPaneContainer(pBar, bLeft);   // mgr vslot +0x128
//     if (pContainer != NULL) pContainer->OnShowPane(pBar, FALSE);   // container vslot +0x50
extern "C" void MS_ABI impl__OnShowPane_CPaneDivider__UEAAXPEAVCDockablePane__H_Z(void* pThis, void* pBar, int bShow) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    const DividerState s = LoadState(pThis);
    if (s.pContainerManager == nullptr) return;
    if (s.bAutoHideMode) return;
    const int bVisible = impl__OnShowPane_CPaneContainerManager__UEAAHPEAVCDockablePane__H_Z(s.pContainerManager, pBar, bShow);
    if (bShow) {
        impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), SW_SHOW);
        return;
    }
    impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), bVisible ? SW_SHOW : SW_HIDE);
    int bLeft = bShow;
    void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
        s.pContainerManager, pBar, &bLeft);
    if (pContainer != nullptr) {
        impl__OnShowPane_CPaneContainer__UEAAXPEAVCDockablePane__H_Z(pContainer, pBar, FALSE);
    }
}

// Symbol: ?OnSize@CPaneDivider@@IEAAXIHH@Z
// This export has no RVA of its own in either map because the linker folded it:
// the WM_SIZE entry (msg 0x5) of CPaneDivider's message map (mfc140u AFX_MSGMAP
// at 0x1802f6c38) points at 0xda30, whose whole body is `jmp ?Default@CWnd@@`
// -- i.e. `CWnd::OnSize(nType, cx, cy)`, which is Default(). The three
// arguments are never read.
extern "C" void MS_ABI impl__OnSize_CPaneDivider__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    (void)nType;
    (void)cx;
    (void)cy;
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    (void)impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
}

// Symbol: ?ReleaseEmptyPaneContainers@CPaneDivider@@QEAAXXZ
// Decoded from retail entry RVA 0xacf40 (mfc140u; 0xad500 mfc140):
//     if (m_pContainerManager != NULL && m_pContainerManager->m_pRootContainer != NULL)   // +0x208, mgr +0x78
//         m_pContainerManager->m_pRootContainer->ReleaseEmptyPaneContainer();   // 0xa4e90 (mfc140)
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CPaneDivider__QEAAXXZ(void* pThis) {
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        void* pRoot = Field<void*>(pManager, kOffManagerRootContainer);
        if (pRoot != nullptr) {
            impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(pRoot);
        }
    }
}

// Symbol: ?ReplacePane@CPaneDivider@@UEAAHPEAVCDockablePane@@0@Z
// Decoded from retail entry RVA 0xac440 (mfc140u; 0xaca00 mfc140):
//     if (m_pContainerManager == NULL) return FALSE;
//     pBarToReplaceWith->SetDefaultPaneDivider(m_hWnd);              // 0x455f0 (mfc140); pBarToReplaceWith unchecked
//     BOOL bResult = m_pContainerManager->ReplacePane(pBarToReplace, pBarToReplaceWith);   // mgr vslot +0x90
//     CheckVisibility();                                              // this vslot +0x530
//     return bResult;
extern "C" int MS_ABI impl__ReplacePane_CPaneDivider__UEAAHPEAVCDockablePane__0_Z(void* pThis, void* pBarToReplace, void* pBarToReplaceWith) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager == nullptr) return FALSE;
    if (pBarToReplaceWith != nullptr) {   // deviation: retail passes it to SetDefaultPaneDivider unchecked
        impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(
            static_cast<CDockablePane*>(pBarToReplaceWith), HwndOf(pThis));
    }
    const int bResult = impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(pManager, pBarToReplace, pBarToReplaceWith);
    impl__CheckVisibility_CPaneDivider__UEAAHXZ(pThis);
    return bResult;
}

// Symbol: ?RepositionPanes@CPaneDivider@@UEAAXAEAVCRect@@AEAPEAX@Z
// Decoded from retail entry RVA 0xac570 (mfc140u; 0xacb30 mfc140):
//     CRect rectSlider = rectNew, rectContainer = rectNew;     // rbp-0x10, rbp-0x20
//     switch (GetCurrentAlignment()) {                          // this vslot +0x338
//     case CBRS_ALIGN_LEFT:   rectSlider.left   = rectNew.right  - m_nWidth; rectContainer.right  = rectSlider.left;
//                             rectContainer.top = rectSlider.top; rectContainer.bottom = rectSlider.bottom; break;
//     case CBRS_ALIGN_TOP:    rectSlider.top    = rectNew.bottom - m_nWidth; rectContainer.bottom = rectSlider.top;
//                             rectContainer.left = rectSlider.left; rectContainer.right = rectSlider.right; break;
//     case CBRS_ALIGN_RIGHT:  rectSlider.right  = rectNew.left   + m_nWidth; rectContainer.left   = rectSlider.right;
//                             rectContainer.top = rectSlider.top; rectContainer.bottom = rectSlider.bottom; break;
//     case CBRS_ALIGN_BOTTOM: rectSlider.bottom = rectNew.top    + m_nWidth; rectContainer.top    = rectSlider.bottom;
//                             rectContainer.left = rectSlider.left; rectContainer.right = rectSlider.right; break;
//     }                                                          // any other value: both rects stay == rectNew
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     pParent->ScreenToClient(&rectNew); pParent->ScreenToClient(&rectSlider); pParent->ScreenToClient(&rectContainer);
//     hdwp = MoveWindow(rectSlider, TRUE, hdwp);                // this vslot +0x478 (CBasePane::MoveWindow), result stored back
//     if (m_pContainerManager == NULL) return;
//     m_pContainerManager->ResizePaneContainers(rectContainer, hdwp);   // mgr vslot +0x80 (CRect by value, HDWP&)
//     CSize sizeMin(0, 0); m_pContainerManager->GetMinSize(sizeMin);  // mgr vslot +0xb0
//     if (!CPane::m_bHandleMinSize) return;                       // 0x3be270 (mfc140u)
//     CObList lstControlBars, lstSliders;
//     m_pContainerManager->AddPanesToList(&lstControlBars, &lstSliders);   // 0xaa960 (mfc140), direct
//     if (rectContainer.Width() >= sizeMin.cx && rectContainer.Height() >= sizeMin.cy) {
//         for (each CWnd* pWnd in lstControlBars) ::SetWindowRgn(pWnd->m_hWnd, NULL, TRUE);
//     } else {
//         for (each o in lstControlBars) {
//             CDockablePane* pBar = DYNAMIC_DOWNCAST(CDockablePane, o);   // RTC 0x3aa178 (mfc140); then dereferenced unchecked
//             CRect rectBar; ::GetWindowRect(pBar->m_hWnd, &rectBar); pParent->ScreenToClient(&rectBar);
//             rectBar.right  = min(rectBar.right,  rectContainer.right);    // `cmovg`
//             rectBar.bottom = min(rectBar.bottom, rectContainer.bottom);
//             rectBar.OffsetRect(-rectBar.left, -rectBar.top);
//             CRgn rgn; rgn.CreateRectRgn(rectBar.left, rectBar.top, rectBar.right, rectBar.bottom);   // GDI32!CreateRectRgn + CGdiObject::Attach
//             ::SetWindowRgn(pBar->m_hWnd, (HRGN)rgn.m_hObject, TRUE);
//             rgn.~CRgn();                                              // -> CGdiObject::DeleteObject on the region the window now owns (retail does this)
//         }
//         for (each o in lstSliders) {
//             CPaneDivider* pSlider = DYNAMIC_DOWNCAST(CPaneDivider, o);   // RTC 0x2f4b58 (mfc140); then dereferenced unchecked
//             pSlider->SetWindowPos(&CWnd::wndBottom, -1, -1, -1, -1,
//                                   SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE /* 0x13 */, NULL);   // slider vslot +0x480; wndBottom 0x3bc1a0 (mfc140)
//         }
//     }
// DEVIATIONS:
//  * The two CObLists are stack objects built through the exported ctor/dtor
//    and walked through FindIndex + the node view (see StackObList / HeadNode).
//  * The region is created and handed over with raw GDI (no CRgn object):
//    ::SetWindowRgn(h, ::CreateRectRgn(...), TRUE) followed by ::DeleteObject on
//    that HRGN, which is exactly the sequence retail's CRgn temporary produces.
//  * Slider vslot +0x480 is dispatched as the exported CWnd::SetWindowPos
//    (retail's pExtra is NULL, see ?Move@).
//  * NULL downcast results are skipped instead of faulting.
extern "C" void MS_ABI impl__RepositionPanes_CPaneDivider__UEAAXAEAVCRect__AEAPEAX_Z(void* pThis, RECT* pRectNew, void** phdwp) {
    if (pThis == nullptr || pRectNew == nullptr || phdwp == nullptr) return;   // deviation: retail has no NULL checks
    RECT rectNew = *pRectNew;
    RECT rectSlider = rectNew;
    RECT rectContainer = rectNew;
    const int nWidth = LoadState(pThis).nWidth;
    switch (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(AsBasePane(pThis))) {
    case kCbrsAlignLeft:
        rectSlider.left = rectNew.right - nWidth;
        rectContainer.right = rectSlider.left;
        rectContainer.top = rectSlider.top;
        rectContainer.bottom = rectSlider.bottom;
        break;
    case kCbrsAlignTop:
        rectSlider.top = rectNew.bottom - nWidth;
        rectContainer.bottom = rectSlider.top;
        rectContainer.left = rectSlider.left;
        rectContainer.right = rectSlider.right;
        break;
    case kCbrsAlignRight:
        rectSlider.right = rectNew.left + nWidth;
        rectContainer.left = rectSlider.right;
        rectContainer.top = rectSlider.top;
        rectContainer.bottom = rectSlider.bottom;
        break;
    case kCbrsAlignBottom:
        rectSlider.bottom = rectNew.top + nWidth;
        rectContainer.top = rectSlider.bottom;
        rectContainer.left = rectSlider.left;
        rectContainer.right = rectSlider.right;
        break;
    default:
        break;
    }
    CWnd* pParent = ParentWndOf(pThis);
    if (pParent != nullptr) {   // deviation: retail dereferences the parent unchecked
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectNew);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectSlider);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectContainer);
    }
    *pRectNew = rectNew;
    *phdwp = impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(AsBasePane(pThis), &rectSlider, TRUE, *phdwp);
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager == nullptr) return;
    RECT rectContainerCopy = rectContainer;
    impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(pManager, &rectContainerCopy, phdwp);
    CSize sizeMin(0, 0);
    impl__GetMinSize_CPaneContainerManager__UEAAXAEAVCSize___Z(pManager, sizeMin);
    if (impl__m_bHandleMinSize_CPane__2HA == 0) return;
    StackObList lstControlBars;
    StackObList lstSliders;
    impl__AddPanesToList_CPaneContainerManager__QEAAXPEAVCObList__0_Z(pManager, lstControlBars.Get(), lstSliders.Get());
    if (rectContainer.right - rectContainer.left >= sizeMin.cx &&
        rectContainer.bottom - rectContainer.top >= sizeMin.cy) {
        for (ObNode* n = HeadNode(lstControlBars.Get()); n != nullptr; n = n->pNext) {
            CWnd* pWnd = reinterpret_cast<CWnd*>(n->data);
            if (pWnd == nullptr) continue;   // deviation: retail dereferences it unchecked
            ::SetWindowRgn(pWnd->m_hWnd, nullptr, TRUE);
        }
        return;
    }
    CRuntimeClass* pDockablePaneRTC = impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
    for (ObNode* n = HeadNode(lstControlBars.Get()); n != nullptr; n = n->pNext) {
        CWnd* pBar = static_cast<CWnd*>(DynDowncast(n->data, pDockablePaneRTC));
        if (pBar == nullptr) continue;   // deviation: retail dereferences the NULL downcast
        RECT rectBar = {0, 0, 0, 0};
        ::GetWindowRect(pBar->m_hWnd, &rectBar);
        if (pParent != nullptr) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectBar);
        if (rectBar.right > rectContainer.right) rectBar.right = rectContainer.right;
        if (rectBar.bottom > rectContainer.bottom) rectBar.bottom = rectContainer.bottom;
        ::OffsetRect(&rectBar, -rectBar.left, -rectBar.top);
        HRGN hRgn = ::CreateRectRgn(rectBar.left, rectBar.top, rectBar.right, rectBar.bottom);
        ::SetWindowRgn(pBar->m_hWnd, hRgn, TRUE);
        if (hRgn != nullptr) ::DeleteObject(hRgn);   // what retail's CRgn destructor does
    }
    CRuntimeClass* pDividerRTC = impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ();
    for (ObNode* n = HeadNode(lstSliders.Get()); n != nullptr; n = n->pNext) {
        void* pSlider = DynDowncast(n->data, pDividerRTC);
        if (pSlider == nullptr) continue;   // deviation: retail dereferences the NULL downcast
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
            AsWnd(pSlider), reinterpret_cast<const CWnd*>(impl__wndBottom_CWnd__2V1_B),
            -1, -1, -1, -1, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
}

// Symbol: ?Serialize@CPaneDivider@@UEAAXAEAVCArchive@@@Z
// Decoded from retail entry RVA 0xab410 (mfc140u; 0xab9d0 mfc140):
//     CBasePane::Serialize(ar);                                   // 0xc360 (mfc140), direct
//     CRect rect(0, 0, 0, 0);
//     if (ar.IsStoring()) {                                       // `test $1,0x20(%rbx)` == m_nMode & CArchive::load
//         ::GetWindowRect(m_hWnd, &rect);
//         CWnd::FromHandle(::GetParent(m_hWnd))->ScreenToClient(&rect);
//         ar << m_nID;                                            // +0x1a8  (each `<<` is the inlined 4-byte buffer write)
//         ar << m_nStep;                                          // +0x1bc
//         ar.Write(&rect, sizeof(RECT));                          // 0x1cfa20 (mfc140)
//         ar << (int)::IsWindowVisible(m_hWnd);
//         ar << m_dwDividerStyle;                                 // +0x1ac
//         ar << m_nWidth;                                         // +0x1b0
//         ar << m_bDefaultDivider;                                // +0x1c4
//         ar << m_nMinOffset;                                     // +0x1b4
//         ar << m_nMaxOffset;                                     // +0x1b8
//     } else {
//         ar >> m_nID;
//         ar >> m_nStep;
//         if (ar.Read(&rect, sizeof(RECT)) != sizeof(RECT))       // 0x1cf7f0 (mfc140)
//             AfxThrowArchiveException(CArchiveException::endOfFile /* 3 */, NULL);
//         int bVisible; ar >> bVisible;
//         ar >> m_dwDividerStyle;  ar >> m_nWidth;  ar >> m_bDefaultDivider;
//         ar >> m_nMinOffset;      ar >> m_nMaxOffset;
//         if (bVisible) m_dwDividerStyle |=  WS_VISIBLE;          // `btr/bts $0x1c ; cmove`
//         else          m_dwDividerStyle &= ~WS_VISIBLE;
//         CreateEx(0, m_dwDividerStyle, rect, m_pParentWndForSerialize, m_nID, NULL);   // this vslot +0x560; +0x200
//     }
//     if (m_pContainerManager != NULL && m_bDefaultDivider)       // +0x208, +0x1c4
//         m_pContainerManager->Serialize(ar);                     // mgr vslot +0x10
// (The per-operator readOnly / writeOnly throws retail inlines into each `<<`
// and `>>` cannot fire once the IsStoring branch has been taken; they are not
// reproduced. A short read of one of the ints is turned into the same
// endOfFile exception CArchive::FillBuffer raises in retail.)
// ?Serialize@CBasePane@@ (CBasePane.cpp:1436) and
// ?Serialize@CPaneContainerManager@@ (CPaneContainerManager.cpp:2086) are both
// placeholders (empty bodies).
extern "C" void MS_ABI impl__Serialize_CPaneDivider__UEAAXAEAVCArchive___Z(void* pThis, CArchive& ar) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__Serialize_CBasePane__UEAAXAEAVCArchive___Z(AsBasePane(pThis), ar);
    RECT rect = {0, 0, 0, 0};
    if (ar.IsStoring()) {
        ::GetWindowRect(HwndOf(pThis), &rect);
        CWnd* pParent = ParentWndOf(pThis);
        if (pParent != nullptr) impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rect);
        const DividerState s = LoadState(pThis);
        auto write4 = [&](int v) { impl__Write_CArchive__QEAAXPEBXI_Z(&ar, &v, sizeof v); };
        write4(static_cast<int>(s.nID));
        write4(s.nStep);
        impl__Write_CArchive__QEAAXPEBXI_Z(&ar, &rect, sizeof rect);
        write4(::IsWindowVisible(HwndOf(pThis)) ? 1 : 0);
        write4(static_cast<int>(s.dwDividerStyle));
        write4(s.nWidth);
        write4(s.bDefaultDivider);
        write4(s.nMinOffset);
        write4(s.nMaxOffset);
    } else {
        auto read4 = [&]() {
            int v = 0;
            if (impl__Read_CArchive__QEAAIPEAXI_Z(&ar, &v, sizeof v) != sizeof v) {
                impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /* CArchiveException::endOfFile */, nullptr);
            }
            return v;
        };
        DividerState s = LoadState(pThis);
        s.nID = static_cast<UINT>(read4());
        s.nStep = read4();
        if (impl__Read_CArchive__QEAAIPEAXI_Z(&ar, &rect, sizeof rect) != sizeof rect) {
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /* CArchiveException::endOfFile */, nullptr);
        }
        const int bVisible = read4();
        s.dwDividerStyle = static_cast<DWORD>(read4());
        s.nWidth = read4();
        s.bDefaultDivider = read4();
        s.nMinOffset = read4();
        s.nMaxOffset = read4();
        if (bVisible) s.dwDividerStyle |= WS_VISIBLE;
        else          s.dwDividerStyle &= ~static_cast<DWORD>(WS_VISIBLE);
        WithState(pThis, [&](DividerState& st) {
            st.nID = s.nID;
            st.nStep = s.nStep;
            st.dwDividerStyle = s.dwDividerStyle;
            st.nWidth = s.nWidth;
            st.bDefaultDivider = s.bDefaultDivider;
            st.nMinOffset = s.nMinOffset;
            st.nMaxOffset = s.nMaxOffset;
        });
        impl__CreateEx_CPaneDivider__UEAAHKKAEBUtagRECT__PEAVCWnd__IPEAUCCreateContext___Z(
            pThis, 0, s.dwDividerStyle, rect, s.pParentWndForSerialize, s.nID, nullptr);
    }
    const DividerState s = LoadState(pThis);
    if (s.pContainerManager != nullptr && s.bDefaultDivider) {
        impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(s.pContainerManager, ar);
    }
}

// Symbol: ?ShowWindow@CPaneDivider@@QEAAXH@Z
// Decoded from retail entry RVA 0xaca20 (mfc140u; 0xacfe0 mfc140): the whole
// body is `jmp ?ShowWindow@CWnd@@QEAAHH@Z` -- `CWnd::ShowWindow(nCmdShow);`.
extern "C" void MS_ABI impl__ShowWindow_CPaneDivider__QEAAXH_Z(void* pThis, int nCmdShow) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__ShowWindow_CWnd__QEAAHH_Z(AsWnd(pThis), nCmdShow);
}

// Symbol: ?StopTracking@CPaneDivider@@MEAAXH@Z
// Decoded from retail entry RVA 0xabe80 (mfc140u; 0xac440 mfc140):
//     if (!m_bCaptured) return;                                       // +0x1c0
//     CRect rectWnd; ::GetWindowRect(m_hWnd, &rectWnd);
//     CPoint ptOffset(m_rectLastDragRect.left - rectWnd.left,
//                     m_rectLastDragRect.top  - rectWnd.top);         // +0x1cc
//     if (m_pWndTrack != NULL && m_pWndTrack->m_hWnd != NULL) {       // +0x210
//         m_pWndTrack->DestroyWindow();                                // CWnd vslot +0xd0
//         if (m_pWndTrack != NULL) delete m_pWndTrack;                 // vslot 1
//         m_pWndTrack = NULL;
//     }
//     if (bMoveSlider) Move(ptOffset, TRUE);                          // this vslot +0x4e8
//     ::SetRectEmpty(&m_rectLastDragRect);
//     ::ReleaseCapture();
//     m_bCaptured = false;
//     if (m_pContainerManager != NULL)
//         m_pContainerManager->SetResizeMode(FALSE);                  // mgr vslot +0x110
extern "C" void MS_ABI impl__StopTracking_CPaneDivider__MEAAXH_Z(void* pThis, int bMoveSlider) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    DividerState s = LoadState(pThis);
    if (!s.bCaptured) return;
    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(HwndOf(pThis), &rectWnd);
    POINT ptOffset = { s.rectLastDragRect.left - rectWnd.left, s.rectLastDragRect.top - rectWnd.top };
    DestroyTrackingWnd(pThis);
    if (bMoveSlider) {
        impl__Move_CPaneDivider__UEAAXAEAVCPoint__H_Z(pThis, &ptOffset, TRUE);
    }
    WithState(pThis, [](DividerState& st) { ::SetRectEmpty(&st.rectLastDragRect); });
    ::ReleaseCapture();
    WithState(pThis, [](DividerState& st) { st.bCaptured = false; });
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        impl__SetResizeMode_CPaneContainerManager__UEAAXH_Z(pManager, FALSE);
    }
}

// Symbol: ?StoreRecentDockSiteInfo@CPaneDivider@@QEAAXPEAVCDockablePane@@@Z
// Decoded from retail entry RVA 0xacb40 (mfc140u; 0xad100 mfc140):
//     if (m_pContainerManager != NULL)
//         m_pContainerManager->StoreRecentDockSiteInfo(pBar);   // mgr vslot +0x120
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneDivider__QEAAXPEAVCDockablePane___Z(void* pThis, void* pBar) {
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager != nullptr) {
        impl__StoreRecentDockSiteInfo_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(pManager, pBar);
    }
}

// Symbol: ?StoreRecentTabRelatedInfo@CPaneDivider@@QEAAXPEAVCDockablePane@@0@Z
// Decoded from retail entry RVA 0xacb70 (mfc140u; 0xad130 mfc140):
//     if (m_pContainerManager == NULL) return;
//     BOOL bLeft = FALSE;
//     CPaneContainer* pContainer = m_pContainerManager->FindPaneContainer(pTabbedBar, bLeft);   // mgr vslot +0x128
//     if (pContainer != NULL)
//         pDockingBar->m_recentDockInfo.StoreDockInfo(pContainer, pTabbedBar);   // CRecentDockSiteInfo (CPane +0x2b8) vslot +0x28
// ?StoreDockInfo@CRecentDockSiteInfo@@ is a placeholder in
// CRecentDockSiteInfo.cpp:64 (empty body).
extern "C" void MS_ABI impl__StoreRecentTabRelatedInfo_CPaneDivider__QEAAXPEAVCDockablePane__0_Z(void* pThis, void* pDockingBar, void* pTabbedBar) {
    void* pManager = LoadState(pThis).pContainerManager;
    if (pManager == nullptr) return;
    int bLeft = FALSE;
    void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
        pManager, pTabbedBar, &bLeft);
    if (pContainer != nullptr && pDockingBar != nullptr) {   // deviation: retail dereferences pDockingBar unchecked
        impl__StoreDockInfo_CRecentDockSiteInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane___Z(
            &static_cast<CPane*>(static_cast<CDockablePane*>(pDockingBar))->m_recentDockInfo, pContainer, pTabbedBar);
    }
}
