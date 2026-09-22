// COleCntrFrameWndEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// COleCntrFrameWndEx is the feature-pack container frame that COleIPFrameWndEx
// creates around an in-place-active server object.  It is COleCntrFrameWnd plus
// one embedded CDockingManager, and almost every export below is a thin forward
// to the same-named CDockingManager method on that sub-object.
//
// EVERY RVA QUOTED IN THIS FILE IS AN mfc140u.dll RVA (disas.py --u; where the
// mfc140u symbol map had no entry the RVA was taken from the mfc140u export
// table by ordinal, `objdump -p mfc140u.dll`, and the body disassembled with
// disas.py --u --at).  Bodies were transcribed from that disassembly; each
// function comment cites the entry RVA it came from.
//
// Retail layout, established from ??0COleCntrFrameWndEx@@QEAA@PEAVCOleIPFrameWnd@@@Z
// (RVA 0x949c0) and ??1COleCntrFrameWndEx@@MEAA@XZ (RVA 0x94a40):
//   * the constructor calls COleCntrFrameWnd::COleCntrFrameWnd (0x25cd00) with
//     %rcx unchanged, stores the class vftable (0x2f0d78) at +0x00, then calls
//     CDockingManager::CDockingManager (0x46ef0) on this+0x1e0; the destructor
//     runs CDockingManager::~CDockingManager (0x472a0) on this+0x1e0 and tail-
//     jumps to COleCntrFrameWnd::~COleCntrFrameWnd (0x25cda0).  So
//         +0x000  COleCntrFrameWnd base (CFrameWnd body, m_pInPlaceFrame at 0x1d8)
//         +0x1e0  CDockingManager m_dockManager
//   * classCOleCntrFrameWndEx.m_nObjectSize is 0x548 (read out of the retail
//     descriptor at 0x2f0cb0; core/ole/RuntimeClasses.cpp records the same 1352),
//     and m_dockManager is the only member the retail declaration adds
//     (atlmfc/include/afxolecntrframewndex.h:44), so sizeof(CDockingManager)
//     == 0x548 - 0x1e0 == 0x368.  Cross-check: classCFrameWndEx is 0x6f0 with
//     m_dockManager at 0x380 followed by one pointer -> 0x6f0 - 8 - 0x380 == 0x368.
//   * inside the sub-object the retail bodies touch (offsets relative to the
//     CDockingManager, named from atlmfc/include/afxdockingmanager.h:288-364):
//         +0x028  CObList m_lstControlBars   (head node pointer at +0x30)
//         +0x178  CObList m_lstMiniFrames    (head node pointer at +0x180)
//         +0x1b0  CFrameWnd* m_pParentWnd
//         +0x1c8  CRect m_rectClientAreaBounds
//         +0x208  BOOL m_bIsPrintPreviewMode
//
// How the sub-object is modelled here.  OpenMFC's CDockingManager
// (include/openmfc/afxmfc.h:1381) is 144 bytes and keeps its live state in a
// side table keyed by `this`; it is placement-constructed at the retail offset
// +0x1e0 through its own export thunk, so the 0x368-byte retail range holds a
// genuine OpenMFC CDockingManager and every forward below hands
// (this + 0x1e0) to the CDockingManager export thunks exactly as retail does.
// The retail members listed above are NOT modelled by that class: where a body
// needs one, the comment says what retail does with it and what is done instead.
//
// vftable: retail stores its own MSVC vtable (0x2f0d78; slots 0..116 were read,
// slot 116 being OnMoveMiniFrame at +0x3a0) in both the constructor and
// destructor.  OpenMFC builds no vtable for this class (it is
// not declared in include/openmfc/), so the vfptr is left as the base
// COleCntrFrameWnd constructor sets it (g_COleCntrFrameWnd_vtbl, 10 slots, see
// detail/COleCntrFrameWndSupport.h).  Consequently the virtual calls retail
// makes on `this` (slot 115 = AdjustDockingLayout at vtable +0x398, slot 96 =
// RecalcLayout at +0x300) are made as direct calls to this file's own thunks:
// the dynamic type is always exactly COleCntrFrameWndEx on retail's creation
// path (COleIPFrameWndEx::OnCreateControlBars does `new COleCntrFrameWndEx`),
// so the direct call resolves to the same body.  Virtual calls on OTHER
// objects (the in-place frame, the active document) are dispatched through
// those objects' vtables by MSVC slot index, as retail does: those objects are
// client-constructed and carry MSVC-layout vtables.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/COleCntrFrameWndSupport.h"

#include <cstdlib>
#include <cstring>
#include <type_traits>

namespace {

// ---- retail layout view --------------------------------------------------
enum : size_t {
    kOff_m_dockManager        = 0x1e0,   // ctor 0x949c0: `lea 0x1e0(%rbx),%rcx; call ??0CDockingManager`
    kSize_COleCntrFrameWndEx  = 0x548,   // classCOleCntrFrameWndEx.m_nObjectSize
    kSize_CDockingManagerRetail = kSize_COleCntrFrameWndEx - kOff_m_dockManager,   // 0x368
    kOff_CFrameWnd_m_hWnd     = 0x40,    // dtor 0x94a40: `mov 0x40(%rsi),%rcx` -> GetTopWindow
    kOff_COleIPFrameWnd_m_lpFrame = 0x200,   // AdjustClientArea 0x95030: `mov 0x200(%r8),%r8`
};

struct S_COleCntrFrameWndEx {
    S_Colecntrframewnd base;                                   // 0x000 .. 0x1e0
    unsigned char      m_dockManager[kSize_CDockingManagerRetail];   // 0x1e0 .. 0x548
};
static_assert(sizeof(S_Colecntrframewnd) == kOff_m_dockManager,
              "COleCntrFrameWnd body must end exactly where retail places m_dockManager");
static_assert(offsetof(S_COleCntrFrameWndEx, m_dockManager) == kOff_m_dockManager, "m_dockManager @ 0x1e0");
static_assert(sizeof(S_COleCntrFrameWndEx) == kSize_COleCntrFrameWndEx, "sizeof(COleCntrFrameWndEx) == 0x548");
static_assert(sizeof(CDockingManager) <= kSize_CDockingManagerRetail,
              "OpenMFC CDockingManager must fit the retail sub-object range");
static_assert(offsetof(CWnd, m_hWnd) == kOff_CFrameWnd_m_hWnd, "CWnd::m_hWnd @ 0x40 (matches retail)");
static_assert(sizeof(CPoint) == 8 && std::is_trivially_copyable<CPoint>::value,
              "CPoint must be an 8-byte register aggregate under MS x64 for the by-value thunks below");

// COleIPFrameWnd retail layout (atlmfc/include/afxole.h:1446-1470): CFrameWnd body
// 0x1d8, BOOL m_bUIActive 0x1d8 (+4 pad), OLEINPLACEFRAMEINFO m_frameInfo 0x1e0
// (32 bytes), LPOLEINPLACEFRAME m_lpFrame 0x200.  Only +0x200 is read here.
inline void* IPFrame_lpFrame(void* pIPFrame) {
    return *reinterpret_cast<void**>(static_cast<unsigned char*>(pIPFrame) + kOff_COleIPFrameWnd_m_lpFrame);
}

inline S_COleCntrFrameWndEx* Self(void* pThis) { return static_cast<S_COleCntrFrameWndEx*>(pThis); }
inline CDockingManager* DockMgr(void* pThis) {
    return reinterpret_cast<CDockingManager*>(Self(pThis)->m_dockManager);
}
inline HWND FrameHwnd(void* pThis) { return static_cast<CWnd*>(static_cast<CFrameWnd*>(pThis))->m_hWnd; }

// Protected-member access to OpenMFC's CDockingManager::m_pParentWnd (the
// retail m_pParentWnd at sub-object +0x1b0).  The OpenMFC member is what its
// own RecalcLayout uses for the parent client rectangle, so it is the right
// thing to seed.  Never instantiated: it exists only for the access rule.
struct DockingManagerAccess : CDockingManager {
    static void SetParentFrame(CDockingManager* p, CFrameWnd* pFrame) {
        static_cast<DockingManagerAccess*>(p)->m_pParentWnd = pFrame;
    }
};

// Virtual-slot dispatch on objects that carry MSVC-layout vtables.
typedef void* (MS_ABI *Fn_GetActiveDocument)(void*);                       // CFrameWnd vtable slot 93 (+0x2e8)
typedef void  (MS_ABI *Fn_RecalcLayout)(void*, int);                        // CFrameWnd vtable slot 96 (+0x300)
typedef void  (MS_ABI *Fn_OnResizeBorder)(void*, const RECT*, void*, int);  // COleServerDoc vtable slot 101 (+0x328)
enum { kSlot_GetActiveDocument = 0x2e8 / 8, kSlot_RecalcLayout = 0x300 / 8, kSlot_OnResizeBorder = 0x328 / 8 };
template <typename Fn> inline Fn VSlot(void* pObj, int slot) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(pObj))[slot]);
}

enum { kWM_IDLEUPDATECMDUI = 0x363 };   // afxpriv.h WM_IDLEUPDATECMDUI (edx in OnIdleUpdateCmdUI, 0x94f70)

}  // namespace

// ---- sibling impl_ exports we delegate to (all definitions verified by grep) ----
// COleCntrFrameWnd (core/ole/COleCntrFrameWnd.cpp)
extern "C" void* MS_ABI impl___0COleCntrFrameWnd__QEAA_PEAVCOleIPFrameWnd___Z(void* pThis, void* pInPlaceFrame);
// (impl___1COleCntrFrameWnd__UEAA_XZ is declared by detail/COleCntrFrameWndSupport.h;
//  both it and OnIdleUpdateCmdUI are defined in COleCntrFrameWnd.cpp)
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleCntrFrameWnd__QEAAXXZ(void* pThis);
// CDockingManager (featurepack/docking/Thunks.cpp, featurepack/docking/CDockingManager.cpp)
extern "C" void* MS_ABI impl___0CDockingManager__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CDockingManager__UEAA_XZ(void* pThis);
extern "C" int   MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int bTail, int bAutoHide, int bInsertForOuterEdge);
extern "C" int   MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* pThis, CBasePane* pPane, CBasePane* pTarget, int bAfter);
extern "C" void  MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int bDestroy, int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement);
extern "C" void  MS_ABI impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    CDockingManager* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect);
extern "C" int   MS_ABI impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(
    CDockingManager* pThis, CPane* pBarToDock, CPane* pBar);
extern "C" int   MS_ABI impl__EnableDocking_CDockingManager__QEAAHK_Z(CDockingManager* pThis, unsigned long dwDockStyle);
extern "C" int   MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(CDockingManager* pThis, unsigned long dwDockStyle);
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames);
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
    const CDockingManager* pThis, CPoint point, int nSensitivity, int bExactBar,
    CRuntimeClass* pRTCBarType, int bCheckVisibility, const CDockingManager* pDockManager);
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* pThis, CPoint point, int nSensitivity, unsigned long* pdwAlignment,
    CRuntimeClass* pRTCBarType, const CDockingManager* pDockManager);
extern "C" void  MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(CDockingManager* pThis, void* hdwp);
extern "C" void  MS_ABI impl__RecalcLayout_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int bNotify);
extern "C" int   MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager* pThis, CWnd* pFrame);
extern "C" int   MS_ABI impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
    CDockingManager* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam);
extern "C" void  MS_ABI impl__GetPaneList_CDockingManager__QEAAXAEAVCObList__HPEAUCRuntimeClass__H_Z(
    CDockingManager* pThis, CObList* pList, int bIncludeAutohide, CRuntimeClass* pRTCFilter, int bIncludeTabbed);
// Two CDockingManager exports that featurepack/docking/CDockingManager.cpp still
// defines as GENERATED placeholders whose parameter lists omit `this`
// (`impl__ShowPanes_...(int p0) { return 0; }`,
// `impl__IsPointNearDockSite_...(void* p0, unsigned long* p1, int* p2) { return 0; }`).
// Both ignore every argument, so calling them with the real MSVC signature is
// safe; they are declared with that signature here -- as
// core/frame/CFrameWndEx.cpp:120-123 already does for both, and
// core/frame/CMDIFrameWndEx.cpp:181 for IsPointNearDockSite -- so the forwards
// retail performs are expressed structurally and start working the moment
// those placeholders get real bodies.
extern "C" int   MS_ABI impl__ShowPanes_CDockingManager__UEAAHH_Z(CDockingManager* pThis, int bShow);
extern "C" int   MS_ABI impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
    const CDockingManager* pThis, CPoint point, unsigned long* pdwBarAlignment, int* pbOuterEdge);
// CObList (core/collections/CObList.cpp)
extern "C" void*    MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void     MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" void*    MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
// CWnd / CFrameWnd (core/window/*.cpp, core/frame/*.cpp)
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__SendMessageToDescendants_CWnd__SAXPEAUHWND____I_K_JHH_Z(
    HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam, int bDeep, int bOnlyPerm);
extern "C" void MS_ABI impl__OnSize_CFrameWnd__IEAAXIHH_Z(CFrameWnd* pThis, unsigned int nType, int cx, int cy);
extern "C" int  MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(CFrameWnd* pThis, CREATESTRUCTW& cs);
// CBasePane (featurepack/docking/CBasePane.cpp)
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int bDelay, int bActivate);

// ---- forward declarations of this file's own thunks that other bodies call ----
extern "C" void MS_ABI impl__AdjustClientArea_COleCntrFrameWndEx__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(void* pThis, void* hdwp);

// COleCntrFrameWndEx::COleCntrFrameWndEx(COleIPFrameWnd*) -- retail (RVA 0x949c0):
//     COleCntrFrameWnd::COleCntrFrameWnd(this, pInPlaceFrame);   // 0x25cd00
//     *(void**)this = &COleCntrFrameWndEx_vftable;               // 0x2f0d78
//     CDockingManager::CDockingManager(this + 0x1e0);            // 0x46ef0
//     return this;
// Deviations: the vftable store is dropped (see the header); the 0x368-byte
// sub-object range is zeroed before the OpenMFC CDockingManager (144 bytes) is
// placement-constructed in it, so the bytes retail's larger CDockingManager
// would have initialised do not hold caller garbage.  A null `this` returns
// null instead of faulting.
// Symbol: ??0COleCntrFrameWndEx@@QEAA@PEAVCOleIPFrameWnd@@@Z
extern "C" void* MS_ABI impl___0COleCntrFrameWndEx__QEAA_PEAVCOleIPFrameWnd___Z(void* pThis, void* pInPlaceFrame) {
    if (pThis == nullptr) return nullptr;
    impl___0COleCntrFrameWnd__QEAA_PEAVCOleIPFrameWnd___Z(pThis, pInPlaceFrame);
    std::memset(Self(pThis)->m_dockManager, 0, sizeof(Self(pThis)->m_dockManager));
    impl___0CDockingManager__QEAA_XZ(Self(pThis)->m_dockManager);
    return pThis;
}

// COleCntrFrameWndEx::~COleCntrFrameWndEx() -- retail (RVA 0x94a40):
//     *(void**)this = &COleCntrFrameWndEx_vftable;                       // 0x2f0d78
//     for (node = m_dockManager.m_lstMiniFrames.head /*this+0x360*/; node; node = node->next) {
//         CObject* p = node->data;
//         if (p && p->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd) /*0x3b1418*/))  // 0x234cf0
//             p->vtable[26](p);                 // CWnd::DestroyWindow, vtable +0xd0
//     }
//     CList<HWND, HWND> lstChildren;      // local; vftable 0x2e9248 (slot 0 is CObject::GetRuntimeClass,
//                                         // i.e. a CList<> instantiation, not CPtrList); block size 10
//     for (CWnd* w = CWnd::FromHandle(::GetTopWindow(m_hWnd)); w; w = CWnd::FromHandle(::GetWindow(w->m_hWnd, GW_HWNDNEXT)))
//         lstChildren.AddTail(w->m_hWnd);                                  // 0x7908, the unexported CList<HWND,HWND>::AddTail
//     for (each HWND h in lstChildren)
//         if (::IsWindow(h) && ::GetParent(h) == m_hWnd) ::DestroyWindow(h);
//     for (node = afxAllToolBars.head /*0x3b2098*/; node; ) {              // unexported CObList at 0x3b2090
//         CWnd* pToolBar = node->data;  if (!pToolBar) AfxThrowInvalidArgException();   // 0x227720
//         posSave = node; node = node->next;
//         if (CWnd::FromHandlePermanent(pToolBar->m_hWnd) == NULL)         // 0x28adc0
//             afxAllToolBars.RemoveAt(posSave);                            // 0x2320d0
//     }
//     lstChildren.RemoveAll();                                             // 0x8350 (folded with CObList/CPtrList::RemoveAll)
//     CDockingManager::~CDockingManager(this + 0x1e0);                     // 0x472a0
//     jmp COleCntrFrameWnd::~COleCntrFrameWnd(this);                       // 0x25cda0
// IAT slots resolved with iatu.py: 0x2c6d20 GetTopWindow, 0x2c6c10 GetWindow,
// 0x2c7138 IsWindow, 0x2c72d8 GetParent, 0x2c6d28 DestroyWindow.
// Reproduced: the child-window teardown loop, the CDockingManager destruction and
// the base destructor call.
// Deviations, each because the state does not exist in this build:
//   * the mini-frame loop needs m_dockManager.m_lstMiniFrames; OpenMFC's
//     CDockingManager keeps its mini frames in a side table with no exported
//     enumerator, so nothing is destroyed here (OpenMFC's own
//     CDockingManager::~CDockingManager drops that side-table entry);
//   * afxAllToolBars is not a global in OpenMFC (CMFCToolBar::GetAllToolbars
//     rebuilds a list from a side table on every call), so there is no list to
//     prune;
//   * the child HWNDs are collected in a malloc'd array instead of a
//     CList<HWND,HWND> and without CWnd::FromHandle's temporary-map side effect -- same handles,
//     same order (if the allocation fails nothing is destroyed);
//   * the loop is skipped when m_hWnd is NULL.  Retail would call
//     ::GetTopWindow(NULL), i.e. enumerate the desktop's top-level windows, and
//     then destroy every one whose ::GetParent() is NULL; that is judged an
//     unintended retail hazard, not behaviour to reproduce;
//   * the vftable store is dropped (see the header) and a null `this` returns.
// Symbol: ??1COleCntrFrameWndEx@@MEAA@XZ
extern "C" void MS_ABI impl___1COleCntrFrameWndEx__MEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;

    const HWND hFrame = FrameHwnd(pThis);
    if (hFrame != nullptr) {   // deviation: retail has no such guard (see above)
        size_t count = 0;
        for (HWND h = ::GetTopWindow(hFrame); h != nullptr; h = ::GetWindow(h, GW_HWNDNEXT))
            ++count;
        HWND* children = count ? static_cast<HWND*>(std::malloc(count * sizeof(HWND))) : nullptr;
        if (children != nullptr) {
            size_t n = 0;
            for (HWND h = ::GetTopWindow(hFrame); h != nullptr && n < count; h = ::GetWindow(h, GW_HWNDNEXT))
                children[n++] = h;
            for (size_t i = 0; i < n; ++i) {
                if (::IsWindow(children[i]) && ::GetParent(children[i]) == hFrame)
                    ::DestroyWindow(children[i]);
            }
            std::free(children);
        }
    }

    impl___1CDockingManager__UEAA_XZ(Self(pThis)->m_dockManager);
    impl___1COleCntrFrameWnd__UEAA_XZ(pThis);
}

// COleCntrFrameWndEx::AddDockSite() -- retail: export ordinal 1840 resolves to
// RVA 0x27d0 (mfc140u), a bare `ret` that the linker folded with
// CFrameWndEx::AddDockSite, CMDIFrameWndEx::AddDockSite and the rest of the 158
// exports (virtual and non-virtual alike) whose bodies are empty.  The empty
// body IS the retail behaviour (inv.py's stub metric still counts it as a stub).
// Symbol: ?AddDockSite@COleCntrFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AddDockSite_COleCntrFrameWndEx__QEAAXXZ(void* pThis) {
    (void)pThis;   // retail: `ret` (RVA 0x27d0)
}

// COleCntrFrameWndEx::AddPane(CBasePane*, BOOL bTail) -- retail (RVA 0x94c40):
//     return CDockingManager::AddPane(this + 0x1e0, pBar, bTail, FALSE, FALSE);   // 0x47860
// Symbol: ?AddPane@COleCntrFrameWndEx@@QEAAHPEAVCBasePane@@H@Z
extern "C" int MS_ABI impl__AddPane_COleCntrFrameWndEx__QEAAHPEAVCBasePane__H_Z(void* pThis, CBasePane* pBar, int bTail) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    return impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(DockMgr(pThis), pBar, bTail, FALSE, FALSE);
}

// COleCntrFrameWndEx::AdjustClientArea() -- retail (RVA 0x95030):
//     CDocument* pDoc = m_pInPlaceFrame->vtable[93](m_pInPlaceFrame);       // CFrameWnd::GetActiveDocument, +0x2e8
//     if (pDoc)
//         pDoc->vtable[101](pDoc, NULL, m_pInPlaceFrame->m_lpFrame /*+0x200*/, TRUE);
//                                          // COleServerDoc::OnResizeBorder(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL), +0x328
// Slot 101 was read out of the COleServerDocEx vtable at 0x2f1ed8 (+0x328 ->
// 0x963b0 ?OnResizeBorder@COleServerDocEx@@MEAAXPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z);
// slot 93 out of this class's own vtable at 0x2f0d78 (+0x2e8 -> 0x29ede0
// ?GetActiveDocument@CFrameWnd@@UEAAPEAVCDocument@@XZ).  m_pInPlaceFrame is the
// COleCntrFrameWnd member at +0x1d8 (S_Colecntrframewnd::m_pInPlaceFrame).
// Both calls are virtual dispatches by MSVC slot on client-constructed objects,
// exactly as retail.  Deviation: a null `this` or a null m_pInPlaceFrame
// returns (retail would fault).
// Symbol: ?AdjustClientArea@COleCntrFrameWndEx@@QEAAXXZ
extern "C" void MS_ABI impl__AdjustClientArea_COleCntrFrameWndEx__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pIPFrame = Self(pThis)->base.m_pInPlaceFrame;
    if (pIPFrame == nullptr) return;
    void* pDoc = VSlot<Fn_GetActiveDocument>(pIPFrame, kSlot_GetActiveDocument)(pIPFrame);
    if (pDoc != nullptr) {
        VSlot<Fn_OnResizeBorder>(pDoc, kSlot_OnResizeBorder)(pDoc, nullptr, IPFrame_lpFrame(pIPFrame), TRUE);
    }
}

// COleCntrFrameWndEx::AdjustDockingLayout(HDWP) -- retail (RVA 0x94d90):
//     jmp COleCntrFrameWndEx::AdjustClientArea      // 0x95030; hdwp is never read
// Symbol: ?AdjustDockingLayout@COleCntrFrameWndEx@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(void* pThis, void* hdwp) {
    (void)hdwp;
    impl__AdjustClientArea_COleCntrFrameWndEx__QEAAXXZ(pThis);
}

// COleCntrFrameWndEx::DockPane(CBasePane*, UINT, LPCRECT) -- retail (RVA 0x94cc0):
//     CDockingManager::DockPane(this + 0x1e0, pBar, nDockBarID, lpRect);   // 0x48570
//     this->vtable[115](this, NULL);                                        // AdjustDockingLayout, +0x398
// The vtable call is made directly (see the header).
// Symbol: ?DockPane@COleCntrFrameWndEx@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__DockPane_COleCntrFrameWndEx__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(
    void* pThis, CBasePane* pBar, unsigned int nDockBarID, const RECT* lpRect) {
    if (pThis == nullptr) return;   // deviation: retail has no guard
    impl__DockPane_CDockingManager__QEAAXPEAVCBasePane__IPEBUtagRECT___Z(DockMgr(pThis), pBar, nDockBarID, lpRect);
    impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
}

// COleCntrFrameWndEx::DockPaneLeftOf(CPane*, CPane*) -- retail (RVA 0x94bd0):
//     CDockingManager::DockPaneLeftOf(this + 0x1e0, pBar, pLeftOf);   // 0x48430, result discarded
//     return TRUE;
// Symbol: ?DockPaneLeftOf@COleCntrFrameWndEx@@QEAAHPEAVCPane@@0@Z
extern "C" int MS_ABI impl__DockPaneLeftOf_COleCntrFrameWndEx__QEAAHPEAVCPane__0_Z(void* pThis, CPane* pBar, CPane* pLeftOf) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    (void)impl__DockPaneLeftOf_CDockingManager__QEAAHPEAVCPane__0_Z(DockMgr(pThis), pBar, pLeftOf);
    return TRUE;
}

// COleCntrFrameWndEx::EnableAutoHidePanes(DWORD) -- retail (RVA 0x94dd0):
//     jmp CDockingManager::EnableAutoHidePanes(this + 0x1e0, dwDockStyle)   // 0x47500
// Symbol: ?EnableAutoHidePanes@COleCntrFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableAutoHidePanes_COleCntrFrameWndEx__QEAAHK_Z(void* pThis, unsigned long dwDockStyle) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    return impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(DockMgr(pThis), dwDockStyle);
}

// COleCntrFrameWndEx::EnableDocking(DWORD) -- retail (RVA 0x94dc0):
//     jmp CDockingManager::EnableDocking(this + 0x1e0, dwDockStyle)   // 0x473f0
// Symbol: ?EnableDocking@COleCntrFrameWndEx@@QEAAHK@Z
extern "C" int MS_ABI impl__EnableDocking_COleCntrFrameWndEx__QEAAHK_Z(void* pThis, unsigned long dwDockStyle) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    return impl__EnableDocking_CDockingManager__QEAAHK_Z(DockMgr(pThis), dwDockStyle);
}

// COleCntrFrameWndEx::GetPane(UINT) -- retail (RVA 0x94cf0):
//     jmp (this + 0x1e0)->vtable[11](this + 0x1e0, nID, TRUE)   // CDockingManager::FindPaneByID(nID, bSearchMiniFrames=TRUE), +0x58
// The sub-object's dynamic type is exactly CDockingManager, so the slot call is
// made as a direct call to the CDockingManager thunk.
// Symbol: ?GetPane@COleCntrFrameWndEx@@QEAAPEAVCBasePane@@I@Z
extern "C" CBasePane* MS_ABI impl__GetPane_COleCntrFrameWndEx__QEAAPEAVCBasePane__I_Z(void* pThis, unsigned int nID) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no guard
    return impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(DockMgr(pThis), nID, TRUE);
}

// COleCntrFrameWndEx::InsertPane(CBasePane*, CBasePane*, BOOL) -- retail (RVA 0x94c60):
//     jmp CDockingManager::InsertPane(this + 0x1e0, pBar, pTarget, bAfter)   // 0x47950
// Symbol: ?InsertPane@COleCntrFrameWndEx@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_COleCntrFrameWndEx__QEAAHPEAVCBasePane__0H_Z(
    void* pThis, CBasePane* pBar, CBasePane* pTarget, int bAfter) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    return impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(DockMgr(pThis), pBar, pTarget, bAfter);
}

// COleCntrFrameWndEx::IsPointNearDockSite(CPoint, DWORD&, BOOL&) const -- retail (RVA 0x94d80):
//     jmp CDockingManager::IsPointNearDockSite(this + 0x1e0, point, dwBarAlignment, bOuterEdge)   // 0x48310
// Transcribed as that forward.  NOTE: OpenMFC's CDockingManager::IsPointNearDockSite
// is still a generated placeholder that returns 0 without touching the
// out-parameters (see the declaration block above), so until it gets a body
// this reports "not near a dock site" with dwBarAlignment / bOuterEdge left as
// the caller passed them.  That is NOT what retail's callee does on its FALSE
// path: CDockingManager::IsPointNearDockSite (RVA 0x48310 mfc140u) stores 0 to
// dwBarAlignment on entry (`movl $0,(%r8)`), sets bOuterEdge to TRUE for the
// outer-edge probe and back to FALSE for the inner one, so a retail FALSE comes
// back with dwBarAlignment == 0 and bOuterEdge == FALSE; a caller that reads
// the out-parameters without checking the return sees the difference until the
// placeholder is implemented.  Deviation: a null `this` returns FALSE (retail
// has no guard).
// Symbol: ?IsPointNearDockSite@COleCntrFrameWndEx@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_COleCntrFrameWndEx__QEBAHVCPoint__AEAKAEAH_Z(
    const void* pThis, CPoint point, unsigned long* pdwBarAlignment, int* pbOuterEdge) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    return impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
        DockMgr(const_cast<void*>(pThis)), point, pdwBarAlignment, pbOuterEdge);
}

// COleCntrFrameWndEx::OnIdleUpdateCmdUI() -- retail (RVA 0x94f70):
//     COleCntrFrameWnd::OnIdleUpdateCmdUI(this);                                   // 0x25cef0
//     CDockingManager::SendMessageToMiniFrames(this + 0x1e0, WM_IDLEUPDATECMDUI, 0, 0);   // 0x4d2a0
//     for (node = m_dockManager.m_lstControlBars.head /*this+0x210*/; node; node = node->next) {
//         CWnd* pBar = node->data;  if (!pBar) AfxThrowInvalidArgException();      // 0x227720
//         CWnd::SendMessageToDescendants(pBar->m_hWnd, WM_IDLEUPDATECMDUI, TRUE, 0, TRUE, FALSE);   // 0x28e600
//     }
// Deviation: OpenMFC's CDockingManager keeps its panes in a side table rather
// than m_lstControlBars, so the pane set is fetched through the exported
// CDockingManager::GetPaneList (all panes, no filter) into a CObList driven
// purely by CObList export thunks, and drained with RemoveHead; every pane
// registered with the OpenMFC manager receives the message (retail's
// m_lstControlBars and OpenMFC's side-table pane set are populated by the
// same AddPane / RemovePaneFromDockManager traffic, but are not guaranteed
// to be identical).  Null entries are skipped rather than thrown on
// (GetPaneList never emits them).  A null `this` returns.
// Symbol: ?OnIdleUpdateCmdUI@COleCntrFrameWndEx@@IEAAXXZ
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleCntrFrameWndEx__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__OnIdleUpdateCmdUI_COleCntrFrameWnd__QEAAXXZ(pThis);
    impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(DockMgr(pThis), kWM_IDLEUPDATECMDUI, 0, 0);

    alignas(8) unsigned char listStorage[sizeof(CObList)];
    CObList* pList = static_cast<CObList*>(impl___0CObList__QEAA__J_Z(reinterpret_cast<CObList*>(listStorage), 10));
    impl__GetPaneList_CDockingManager__QEAAXAEAVCObList__HPEAUCRuntimeClass__H_Z(DockMgr(pThis), pList, TRUE, nullptr, TRUE);
    while (impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0) != nullptr) {
        CObject* pObj = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(pList);
        if (pObj == nullptr) continue;
        CWnd* pBar = static_cast<CWnd*>(static_cast<CBasePane*>(pObj));
        impl__SendMessageToDescendants_CWnd__SAXPEAUHWND____I_K_JHH_Z(pBar->m_hWnd, kWM_IDLEUPDATECMDUI, TRUE, 0, TRUE, FALSE);
    }
    impl___1CObList__UEAA_XZ(pList);
}

// COleCntrFrameWndEx::OnMoveMiniFrame(CWnd*) -- retail (RVA 0x94da0):
//     jmp (this + 0x1e0)->vtable[17](this + 0x1e0, pFrame)   // CDockingManager::OnMoveMiniFrame, +0x88
// Direct call to the CDockingManager thunk (dynamic type is exactly CDockingManager).
// Symbol: ?OnMoveMiniFrame@COleCntrFrameWndEx@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_COleCntrFrameWndEx__UEAAHPEAVCWnd___Z(void* pThis, CWnd* pFrame) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    return impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(DockMgr(pThis), pFrame);
}

// COleCntrFrameWndEx::OnShowPanes(BOOL) -- retail (RVA 0x94fe0):
//     BOOL b = (this + 0x1e0)->vtable[22](this + 0x1e0, bShow);   // CDockingManager::ShowPanes, +0xb0
//     this->vtable[115](this, NULL);                              // AdjustDockingLayout, +0x398
//     return b;
// Both calls are made directly (see the header: the sub-object's dynamic type
// is exactly CDockingManager, and this class carries no OpenMFC vtable).
// NOTE: OpenMFC's CDockingManager::ShowPanes is still a generated placeholder
// returning 0 (see the declaration block above), so until it gets a body this
// runs only the AdjustDockingLayout half and returns FALSE.  The return value
// happens to match retail regardless: CDockingManager::ShowPanes (RVA 0x4d640
// mfc140u) ends every path -- early exits included -- at `xor %eax,%eax`, i.e.
// it always returns FALSE, so OnShowPanes always returns FALSE in retail too.
// What is missing is ShowPanes' side effect (hiding / re-showing the docked
// panes and mini frames).  Deviation: a null `this` returns FALSE (retail has
// no guard).
// Symbol: ?OnShowPanes@COleCntrFrameWndEx@@UEAAHH@Z
extern "C" int MS_ABI impl__OnShowPanes_COleCntrFrameWndEx__UEAAHH_Z(void* pThis, int bShow) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no guard
    const int bResult = impl__ShowPanes_CDockingManager__UEAAHH_Z(DockMgr(pThis), bShow);
    impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    return bResult;
}

// COleCntrFrameWndEx::OnSize(UINT, int, int) -- retail (RVA 0x94bf0):
//     CFrameWnd::OnSize(this, nType, cx, cy);          // 0x2a0450
//     if (nType != SIZE_MINIMIZED /*1*/)
//         this->vtable[115](this, NULL);               // AdjustDockingLayout, +0x398 (direct call here)
// Symbol: ?OnSize@COleCntrFrameWndEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_COleCntrFrameWndEx__IEAAXIHH_Z(void* pThis, unsigned int nType, int cx, int cy) {
    if (pThis == nullptr) return;   // deviation: retail has no guard
    impl__OnSize_CFrameWnd__IEAAXIHH_Z(static_cast<CFrameWnd*>(pThis), nType, cx, cy);
    if (nType != SIZE_MINIMIZED) {
        impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    }
}

// COleCntrFrameWndEx::OnSizing(UINT, LPRECT) -- retail (RVA 0x94ec0):
//     CWnd::Default(this);                              // 0x28ac80
//     RECT rcWnd = {0};  ::GetWindowRect(m_hWnd, &rcWnd);           // IAT 0x2c6c08 GetWindowRect
//     RECT rcNew;        ::CopyRect(&rcNew, pRect);                  // IAT 0x2c7218 CopyRect
//     if ((rcWnd.right - rcWnd.left) != (rcNew.right - rcNew.left) ||
//         (rcWnd.bottom - rcWnd.top) != (rcNew.bottom - rcNew.top))
//         this->vtable[115](this, NULL);                // AdjustDockingLayout, +0x398 (direct call here)
// (fwSide is never read.)  Deviation: a null `this` or a null pRect returns;
// retail has no guard.
// Symbol: ?OnSizing@COleCntrFrameWndEx@@IEAAXIPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__OnSizing_COleCntrFrameWndEx__IEAAXIPEAUtagRECT___Z(void* pThis, unsigned int fwSide, RECT* pRect) {
    (void)fwSide;
    if (pThis == nullptr || pRect == nullptr) return;
    (void)impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(static_cast<CFrameWnd*>(pThis)));
    RECT rcWnd = {0, 0, 0, 0};
    ::GetWindowRect(FrameHwnd(pThis), &rcWnd);
    RECT rcNew;
    ::CopyRect(&rcNew, pRect);
    if ((rcWnd.right - rcWnd.left) != (rcNew.right - rcNew.left) ||
        (rcWnd.bottom - rcWnd.top) != (rcNew.bottom - rcNew.top)) {
        impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
    }
}

// COleCntrFrameWndEx::PaneFromPoint(CPoint, int, bool, CRuntimeClass*) const -- retail (RVA 0x94d10):
//     return (this + 0x1e0)->vtable[6](this + 0x1e0, point, nSensitivity, bExactBar, pRTCBarType,
//                                      /*bCheckVisibility*/ FALSE, /*pDockManager*/ NULL);   // +0x30
// Direct call to the CDockingManager thunk (dynamic type is exactly CDockingManager).
// CPoint arrives by value in one register; it is forwarded unchanged.
// Symbol: ?PaneFromPoint@COleCntrFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_COleCntrFrameWndEx__QEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass___Z(
    const void* pThis, CPoint point, int nSensitivity, bool bExactBar, CRuntimeClass* pRTCBarType) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no guard
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        DockMgr(const_cast<void*>(pThis)), point, nSensitivity, bExactBar ? 1 : 0, pRTCBarType, FALSE, nullptr);
}

// COleCntrFrameWndEx::PaneFromPoint(CPoint, int, DWORD&, CRuntimeClass*) const -- retail (RVA 0x94d50):
//     return (this + 0x1e0)->vtable[5](this + 0x1e0, point, nSensitivity, &dwAlignment, pRTCBarType,
//                                      /*pDockManager*/ NULL);   // +0x28
// Symbol: ?PaneFromPoint@COleCntrFrameWndEx@@QEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_COleCntrFrameWndEx__QEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass___Z(
    const void* pThis, CPoint point, int nSensitivity, unsigned long* pdwAlignment, CRuntimeClass* pRTCBarType) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no guard
    return impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
        DockMgr(const_cast<void*>(pThis)), point, nSensitivity, pdwAlignment, pRTCBarType, nullptr);
}

// COleCntrFrameWndEx::PreCreateWindow(CREATESTRUCT&) -- retail (RVA 0x94c30):
//     m_dockManager.m_pParentWnd = this;               // this+0x390 == sub-object +0x1b0
//     jmp CFrameWnd::PreCreateWindow(this, cs);        // 0x29d8d0
// The parent-frame store goes to OpenMFC's own CDockingManager::m_pParentWnd
// member (its RecalcLayout lays panes out against that frame's client area).
// Symbol: ?PreCreateWindow@COleCntrFrameWndEx@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_COleCntrFrameWndEx__MEAAHAEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* pcs) {
    if (pThis == nullptr || pcs == nullptr) return FALSE;   // deviation: retail has no guard
    DockingManagerAccess::SetParentFrame(DockMgr(pThis), static_cast<CFrameWnd*>(pThis));
    return impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(static_cast<CFrameWnd*>(pThis), *pcs);
}

// COleCntrFrameWndEx::RecalcLayout(BOOL bNotify) -- retail (RVA 0x94de0):
//     COleCntrFrameWndEx::AdjustClientArea(this);                            // 0x95030
//     (this + 0x1e0)->vtable[16](this + 0x1e0, NULL);                       // CDockingManager::AdjustDockingLayout, +0x80
//     (this + 0x1e0)->vtable[15](this + 0x1e0, bNotify);                    // CDockingManager::RecalcLayout, +0x78
//     CWnd* pView = m_pViewActive;                                          // this+0x170
//     if (pView && pView->IsKindOf(RUNTIME_CLASS(CPreviewViewEx) /*0x2f98a8*/)   // 0x234cf0
//         && m_dockManager.m_bIsPrintPreviewMode /*this+0x3e8*/) {
//         const CRect& r = m_dockManager.m_rectClientAreaBounds;            // this+0x3a8
//         CWnd::SetWindowPos(pView, NULL, r.left, r.top, r.Width(), r.Height(),
//                            SWP_NOZORDER | SWP_NOACTIVATE /*0x14*/);        // 0x2a9a60
//     }
//     jmp m_pInPlaceFrame->vtable[96](m_pInPlaceFrame, bNotify);            // RecalcLayout on the in-place frame, +0x300
// The three docking calls are made directly (sub-object dynamic type is exactly
// CDockingManager); the in-place frame call is a virtual dispatch by MSVC slot
// on that client-constructed object, as retail.
// Deviation: the print-preview block (the m_pViewActive / CPreviewViewEx test
// and the SetWindowPos) is NOT reproduced.  Its effect needs
// m_dockManager.m_bIsPrintPreviewMode and m_rectClientAreaBounds, neither of
// which OpenMFC's CDockingManager models.  Retail raises that flag in
// CDockingManager::SetPrintPreviewMode (RVA 0x4d080 mfc140u; the constructor
// zeroes it), and OpenMFC's export of SetPrintPreviewMode is a no-op
// placeholder, so the block could never fire in this build; the IsKindOf test
// that guards it is dropped with it.
// Null `this` / null m_pInPlaceFrame return instead of faulting.
// Symbol: ?RecalcLayout@COleCntrFrameWndEx@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_COleCntrFrameWndEx__UEAAXH_Z(void* pThis, int bNotify) {
    if (pThis == nullptr) return;
    impl__AdjustClientArea_COleCntrFrameWndEx__QEAAXXZ(pThis);
    impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(DockMgr(pThis), nullptr);
    impl__RecalcLayout_CDockingManager__UEAAXH_Z(DockMgr(pThis), bNotify);
    // retail: print-preview repositioning of m_pViewActive -- not modelled (see above)
    void* pIPFrame = Self(pThis)->base.m_pInPlaceFrame;
    if (pIPFrame == nullptr) return;
    VSlot<Fn_RecalcLayout>(pIPFrame, kSlot_RecalcLayout)(pIPFrame, bNotify);
}

// COleCntrFrameWndEx::RemovePaneFromDockManager(CBasePane*, BOOL, BOOL, BOOL, CBasePane*) -- retail (RVA 0x94c70):
//     CDockingManager::RemovePaneFromDockManager(this + 0x1e0, pBar, bDestroy, bAdjustLayout,
//                                                bAutoHide, pBarReplacement);   // 0x479c0
//     jmp this->vtable[115](this, NULL);                                        // AdjustDockingLayout, +0x398 (direct call here)
// Symbol: ?RemovePaneFromDockManager@COleCntrFrameWndEx@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_COleCntrFrameWndEx__QEAAXPEAVCBasePane__HHH0_Z(
    void* pThis, CBasePane* pBar, int bDestroy, int bAdjustLayout, int bAutoHide, CBasePane* pBarReplacement) {
    if (pThis == nullptr) return;   // deviation: retail has no guard
    impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
        DockMgr(pThis), pBar, bDestroy, bAdjustLayout, bAutoHide, pBarReplacement);
    impl__AdjustDockingLayout_COleCntrFrameWndEx__UEAAXPEAX_Z(pThis, nullptr);
}

// COleCntrFrameWndEx::ShowPane(CBasePane*, BOOL, BOOL, BOOL) -- retail: export
// ordinal 13834 resolves to RVA 0x68050, one body the linker folded for
// CFrameWndEx / CMDIFrameWndEx / CMDIChildWndEx / COleIPFrameWndEx /
// COleDocIPFrameWndEx / COleCntrFrameWndEx::ShowPane:
//     pBar->vtable[139](pBar, bShow, bDelay, bActivate);   // CBasePane::ShowPane, +0x458
// `this` is never read.  The pane is a client-side object whose dynamic type
// may be a derived pane; OpenMFC ships two bodies for that slot,
// CBasePane::ShowPane (featurepack/docking/CBasePane.cpp) and
// CDockablePane::ShowPane, and the latter is a pure forward to the former, so
// the CBasePane thunk is called directly -- the same substitution
// core/frame/CFrameWndEx.cpp makes in OnPaneCheck for this slot.  A client
// class that overrides ShowPane would not be reached (retail dispatches
// virtually).  Null pBar returns (retail would fault).
// Symbol: ?ShowPane@COleCntrFrameWndEx@@QEAAXPEAVCBasePane@@HHH@Z
extern "C" void MS_ABI impl__ShowPane_COleCntrFrameWndEx__QEAAXPEAVCBasePane__HHH_Z(
    void* pThis, CBasePane* pBar, int bShow, int bDelay, int bActivate) {
    (void)pThis;
    if (pBar == nullptr) return;
    impl__ShowPane_CBasePane__UEAAXHHH_Z(pBar, bShow, bDelay, bActivate);
}
