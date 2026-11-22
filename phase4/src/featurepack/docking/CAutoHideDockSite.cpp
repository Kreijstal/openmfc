// CAutoHideDockSite — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The bodies marked "Retail 0x....." below were decoded from the retail export
// the way phase4/src/core/ole/COleControl.cpp describes.  Unless an address is
// explicitly labelled "(mfc140u)", every address quoted is in mfc140.dll, and
// every one quoted for a function is that function's ENTRY; addresses of data
// (vftables, CRuntimeClass descriptors, statics, import slots) are named as
// such where they appear.  mfc140.dll is the ANSI twin of mfc140u.dll -- function bodies are
// byte-identical between the two images, so the control flow, member offsets
// and constants read off it are valid, but its addresses are NOT mfc140u
// addresses.  Import slots were resolved with iat.py, not guessed.
//
// The dock site layout in detail/FeaturePackDockSite.h retains the retail offsets.
// Retail CAutoHideDockSite vftable (0x1802d9868, mfc140) -- the overridden
// slots the bodies below implement, and the inherited slots they use.  Diffed
// against the CDockSite vftable (0x1802e4388, mfc140), the
// complete set of differing slots is +0x000 GetRuntimeClass, +0x008 scalar
// deleting dtor, +0x060 GetMessageMap, the three overrides below, and one new
// slot +0x588 AllowShowOnPaneMenu (0x7260, `xor eax,eax; ret`, inline in
// afxautohidedocksite.h:55):
//   +0x2d8 IsHorizontal (0x88e0, inherited: GetCurrentAlignment() & 0xA000)
//   +0x318 CanAcceptPane (0xab70)   +0x338 CBasePane::GetCurrentAlignment (0xce30)
//   +0x4f8 DockPane (0xa640)        +0x540 RepositionPanes (0xa7f0)
//   +0x560 CDockSite::AdjustDockingLayout (0x54ed0, inherited)
//
// Systematic deviation, stated once: OpenMFC installs no retail-shaped vftable
// on these objects, so a virtual retail dispatches through `this` is called
// here as the impl__ thunk of the class whose body sits in that slot (a
// devirtualisation), exactly as CDockSite.cpp does.

#define OPENMFC_APPCORE_IMPL
#include "detail/FeaturePackDockSite.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMemDCSupport.h"   // S_Cmemdc view + the CMemDC thunk declarations

#include <cstddef>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Thunks defined elsewhere in this DLL (briefing S1).  Each was located with
// grep before being declared; the defining file is named per group.
// ---------------------------------------------------------------------------
// featurepack/docking/CDockSite.cpp (real bodies)
extern "C" void* MS_ABI impl__RowFromPane_CDockSite__IEBAPEAVCDockingPanesRow__PEAVCBasePane___Z(
    const void* pThis, CBasePane* pBar);
extern "C" void* MS_ABI impl__AddRow_CDockSite__QEAAPEAVCDockingPanesRow__PEAU__POSITION__H_Z(
    void* site, void* position, int height);
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockSite__UEAAXXZ(void* site);
// featurepack/docking/CDockingPanesRow.cpp (real body)
extern "C" void MS_ABI impl__GetGroupFromPane_CDockingPanesRow__QEAAXPEAVCPane__AEAVCObList___Z(
    void* pThis, CPane* pBar, CObList* pList);
extern "C" void MS_ABI impl__AddPane_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT__H_Z(
    void* row, CPane* pane, int dockMethod, const RECT* rect, BOOL addLast);
// featurepack/docking/CBasePane.cpp:841
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
// featurepack/docking/CMFCAutoHideBar.cpp:194 / :423 and RuntimeClasses.cpp:291
extern "C" void* MS_ABI impl__GetFirstAHWindow_CMFCAutoHideBar__QEAAPEAVCDockablePane__XZ(void* pThis);
extern "C" void MS_ABI impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(
    CMFCAutoHideBar* pThis, CDockablePane* pFirstBarInGroup);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCAutoHideBar__SAPEAUCRuntimeClass__XZ();
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
    CMFCAutoHideBar* bar, CSize* result, BOOL stretch, BOOL horizontal);
// core/runtime/CObject.cpp:49
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis,
                                                                        const CRuntimeClass* pClass);
// core/collections/CObList.cpp
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI
    impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
extern "C" void MS_ABI impl__AddTail_CObList__QEAAXPEAV1__Z(CObList* pThis, CObList* pNewList);
extern "C" void MS_ABI impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos);
// core/gdi/CPaintDC.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// featurepack/visualmanager/Thunks.cpp:1498 and the exported mirror of
// CMFCVisualManager::m_pVisManager (core/runtime/StaticData.cpp) -- the same
// pair CDockSite::OnEraseBkgnd uses to reach the visual manager.
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rect, CRect rectClip, int bNCArea);
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;

// Same-file thunks used before their definition.
extern "C" void MS_ABI impl__UnSetAutoHideMode_CAutoHideDockSite__QEAAXPEAVCMFCAutoHideBar___Z(
    void* pThis, CMFCAutoHideBar* pAutohideToolbar);
extern "C" int impl__m_nExtraSpace_CAutoHideDockSite__1HA;

// ---------------------------------------------------------------------------
// Shadow layout + local helpers
// ---------------------------------------------------------------------------
namespace {

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at CWnd+0x40");
static_assert(offsetof(CPane, m_bFirstInGroup) == 0x1AC, "retail UnSetAutoHideMode reads +0x1AC");

inline CAutoHideDockSite* Site(void* p) { return static_cast<CAutoHideDockSite*>(p); }
inline const CAutoHideDockSite* Site(const void* p) { return static_cast<const CAutoHideDockSite*>(p); }
inline CBasePane* AsPane(void* p) { return static_cast<CBasePane*>(p); }
inline const CBasePane* AsPane(const void* p) { return static_cast<const CBasePane*>(p); }

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h) -- what a
// CObList::POSITION points at.  GetHeadPosition / GetNext / GetCount are inline
// in retail MFC and have no export; this read-only view plus the exported
// FindIndex thunk is how every list here is walked, as in CDockSite.cpp.
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

inline ObNode* HeadNode(const CObList* pList) {
    CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0);
    ObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}
inline CObList::POSITION PosFromNode(const ObNode* n) {
    CObList::POSITION pos(nullptr);
    std::memcpy(&pos, &n, sizeof(n));
    return pos;
}

// A stack CObList constructed and destroyed through the exported thunks, so its
// side-table storage (include/openmfc/afx.h) is registered and released.  Retail
// builds its locals inline: CObList vftable + zeroed head/tail/count/free/blocks
// and m_nBlockSize = 10 (the 16-byte constant at 0x180348da0 is {0, 10}).
struct LocalObList {
    alignas(CObList) unsigned char storage[sizeof(CObList)];
    LocalObList() { impl___0CObList__QEAA__J_Z(get(), 10); }
    ~LocalObList() { impl___1CObList__UEAA_XZ(get()); }
    LocalObList(const LocalObList&) = delete;
    LocalObList& operator=(const LocalObList&) = delete;
    CObList* get() { return reinterpret_cast<CObList*>(storage); }
};

// Retail vslot +0x2d8 (IsHorizontal, 0x88e0) is `GetCurrentAlignment() & 0xA000`,
// GetCurrentAlignment being vslot +0x338 (CBasePane::GetCurrentAlignment, 0xce30:
// m_dwStyle & 0xF000).  CAutoHideDockSite overrides neither slot.  Devirtualised
// onto the CBasePane thunk, as CDockSite.cpp does.
inline bool SiteIsHorz(const void* pThis) {
    return (impl__GetCurrentAlignment_CBasePane__UEBAKXZ(AsPane(pThis)) & 0xA000UL) != 0;
}

} // namespace

IMPLEMENT_DYNCREATE(CAutoHideDockSite, CDockSite)

// Symbol: ??0CAutoHideDockSite@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAutoHideDockSite__QEAA_XZ(void* storage) {
    return storage ? ::new (storage) CAutoHideDockSite : nullptr;
}
// Symbol: ??1CAutoHideDockSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CAutoHideDockSite__UEAA_XZ(void* site) {
    if (site) static_cast<CAutoHideDockSite*>(site)->CAutoHideDockSite::~CAutoHideDockSite();
}
// Symbol: ?CanAcceptPane@CAutoHideDockSite@@UEBAHPEBVCBasePane@@@Z
// Retail 0xab70 (0xaaf0 in mfc140u), transcribed in full -- `this` is unused:
//   return pBar->IsKindOf(RUNTIME_CLASS(CMFCAutoHideBar));
// i.e. `mov %rdx,%rcx; lea 0x1802d9728,%rdx; jmp 0x233310` (CObject::IsKindOf,
// which tests pBar for NULL itself); 0x1802d9728 is the CMFCAutoHideBar
// CRuntimeClass (mfc140; its name field reads "CMFCAutoHideBar", size 0x458).
extern "C" int MS_ABI impl__CanAcceptPane_CAutoHideDockSite__UEBAHPEBVCBasePane___Z(
    const void* pThis, const CBasePane* pBar) {
    return pThis && Site(pThis)->CAutoHideDockSite::CanAcceptPane(pBar);
}

BOOL CAutoHideDockSite::CanAcceptPane(const CBasePane* pane) const {
    return pane && (pane->IsKindOf(RUNTIME_CLASS(CMFCAutoHideBar)) ||
        pane->IsKindOf(impl__GetThisClass_CMFCAutoHideBar__SAPEAUCRuntimeClass__XZ()));
}

// Symbol: ?CreateObject@CAutoHideDockSite@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CAutoHideDockSite__SAPEAVCObject__XZ() {
    return CAutoHideDockSite::CreateObject();
}

// Symbol: ?DockPane@CAutoHideDockSite@@UEAAXPEAVCPane@@W4AFX_DOCK_METHOD@@PEBUtagRECT@@@Z
// Retail 0xa640 (0xa5c0 in mfc140u) creates a single row for the site's bars.
extern "C" void MS_ABI impl__DockPane_CAutoHideDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT___Z(
    void* pThis, CPane* pane, int, const RECT* rect) {
    auto* site = Site(pThis);
    if (!site || !site->m_hWnd || !site->CanAcceptPane(pane) || !pane->m_hWnd ||
        ::GetParent(pane->m_hWnd) != site->m_hWnd ||
        site->m_lstControlBars.Find(pane) != nullptr) return;

    auto* row = site->m_lstDockBarRows.IsEmpty() ? nullptr : site->m_lstDockBarRows.GetHead();
    if (!row) {
        const DWORD alignment = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(site);
        const BOOL horizontal = SiteIsHorz(site);
        CSize size(0, 0);
        impl__CalcFixedLayout_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
            static_cast<CMFCAutoHideBar*>(pane), &size, FALSE, horizontal);
        const int padding = impl__m_nExtraSpace_CAutoHideDockSite__1HA;
        const int thickness = (horizontal ? size.cy : size.cx) + padding;
        row = static_cast<CObject*>(
            impl__AddRow_CDockSite__QEAAPEAVCDockingPanesRow__PEAU__POSITION__H_Z(site, nullptr, thickness));
        if (!row) return;
        const BOOL trailingEdge = (alignment & (CBRS_ALIGN_RIGHT | CBRS_ALIGN_BOTTOM)) != 0;
        openmfc::detail::mfccore::ConfigureAutoHideRow(row, padding, trailingEdge);
    }

    constexpr int dockByRectangle = 4; // AFX_DOCK_METHOD::DM_RECT
    pane->m_pParentDockBar = reinterpret_cast<CDockBar*>(site);
    impl__AddPane_CDockingPanesRow__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT__H_Z(
        row, pane, dockByRectangle, rect, TRUE);
    ::ShowWindow(site->m_hWnd, SW_SHOW);
    site->m_lstControlBars.AddTail(pane);
    impl__AdjustDockingLayout_CDockSite__UEAAXXZ(site);
    openmfc::detail::mfccore::ArrangeAutoHideRow(row, site->m_nOffsetLeft);
}

// Symbol: ?GetAlignRect@CAutoHideDockSite@@QEBAXAEAVCRect@@@Z
// Retail 0xab10 (0xaa90 in mfc140u), transcribed in full:
//   ::GetWindowRect(m_hWnd, &rect);                  // import 0x1802c5370, result ignored
//   if (IsHorizontal()) {                            // vslot +0x2d8
//       rect.left  += m_nOffsetLeft;                 // +0x220
//       rect.right -= m_nOffsetRight;                // +0x224
//   } else {
//       rect.top    += m_nOffsetLeft;
//       rect.bottom -= m_nOffsetRight;
//   }
// The pThis / &rect NULL guard is ours.
extern "C" void MS_ABI impl__GetAlignRect_CAutoHideDockSite__QEBAXAEAVCRect___Z(const void* pThis, CRect* pRect) {
    if (pThis == nullptr || pRect == nullptr) return;
    ::GetWindowRect(AsPane(pThis)->m_hWnd, reinterpret_cast<RECT*>(pRect));
    const CAutoHideDockSite* s = Site(pThis);
    if (SiteIsHorz(pThis)) {
        pRect->left += s->m_nOffsetLeft;
        pRect->right -= s->m_nOffsetRight;
    } else {
        pRect->top += s->m_nOffsetLeft;
        pRect->bottom -= s->m_nOffsetRight;
    }
}

// Symbol: ?OnPaint@CAutoHideDockSite@@IEAAXXZ
// The export has no RVA in either map; its body was located through the
// retail CAutoHideDockSite message map (0x1802d9808, mfc140: base map =
// CDockSite's, one entry WM_PAINT 0x000f, sig 19) whose pfn is 0xaa30 -- a body
// the linker folded with CMFCOutlookBar::OnPaint.  Transcribed in full:
//   CPaintDC dc(this);                                   // 0x2a1c60
//   CMemDC memDC(dc, this);                              // 0x69d80
//   CDC* pDC = &memDC.GetDC();     // inline: m_bMemDC (+0x10) ? &m_dcMem (+0x20) : m_dc (+0x08)
//   CRect rectClient(0,0,0,0);
//   ::GetClientRect(m_hWnd, &rectClient);                // import 0x1802c5358
//   CMFCVisualManager::GetInstance()->OnFillBarBackground(  // 0x97f4 (inline GetInstance), vslot +0x78
//       pDC, this, rectClient, rectClient, FALSE);       // same rect twice; bNCArea = 0
//   memDC.~CMemDC(); dc.~CPaintDC();                     // 0x6a1b0 / 0x2a1d10
// Deviation, shared with CDockSite::OnEraseBkgnd / CPaneDivider::OnPaint:
// GetInstance (0x97f4 reads m_pVisManager and otherwise creates the default
// manager) exists in OpenMFC only as the C++ static
// CMFCVisualManager::GetInstance(), which this TU may not call; the exported
// mirror of m_pVisManager is read instead, so the fill happens only when a
// manager already exists.  The pThis NULL guard is ours.
// Signature corrected: the generated stub had dropped `this`.
extern "C" void MS_ABI impl__OnPaint_CAutoHideDockSite__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, static_cast<CWnd*>(AsPane(pThis)));
    alignas(void*) unsigned char memStorage[sizeof(S_Cmemdc)] = {};
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, pPaintDC, pThis);
    CDC* pDC = pMemDC->m_bMemDC ? reinterpret_cast<CDC*>(&pMemDC->m_dcMem) : static_cast<CDC*>(pMemDC->m_dc);

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(AsPane(pThis)->m_hWnd, &rectClient);
    CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVM != nullptr) {   // deviation: retail's GetInstance() creates the default manager here
        impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
            pVM, pDC, AsPane(pThis), CRect(rectClient), CRect(rectClient), FALSE);
    }

    impl___1CMemDC__UEAA_XZ(pMemDC);
    impl___1CPaintDC__UEAA_XZ(pPaintDC);
}

// Symbol: ?RepositionPanes@CAutoHideDockSite@@UEAAXAEAVCRect@@@Z
// Retail 0xa7f0 takes the geometry from the existing row and toolbar metrics.
extern "C" void MS_ABI impl__RepositionPanes_CAutoHideDockSite__UEAAXAEAVCRect___Z(void* pThis, CRect*) {
    auto* site = Site(pThis);
    if (!site || site->m_lstDockBarRows.IsEmpty()) return;
    openmfc::detail::mfccore::ArrangeAutoHideRow(site->m_lstDockBarRows.GetHead(), site->m_nOffsetLeft);
}

// Symbol: ?UnSetAutoHideMode@CAutoHideDockSite@@QEAAXPEAVCMFCAutoHideBar@@@Z
// Retail 0xa8b0 (no mfc140u RVA in the map), transcribed in full:
//   if (pAutohideToolbar == NULL) {
//       CObList lstBars;                                        // stack, nBlockSize 10
//       lstBars.AddTail(&m_lstControlBars);                     // 0x230520 (CPtrList::AddTail(CPtrList*), folded)
//       for (POSITION pos = lstBars.GetHeadPosition(); pos != NULL; ) {
//           POSITION posSave = pos;
//           CPane* pBar = (CPane*)lstBars.GetNext(pos);
//           if (!pBar->m_bFirstInGroup)                         // +0x1AC, no NULL test
//               lstBars.RemoveAt(posSave);                      // 0x2306f0
//       }
//       for (POSITION pos = lstBars.GetHeadPosition(); pos != NULL; )
//           UnSetAutoHideMode((CMFCAutoHideBar*)lstBars.GetNext(pos));   // recursive call 0xa8b0
//   } else {
//       CDockingPanesRow* pRow = RowFromPane(pAutohideToolbar); // 0x54770, before the list exists
//       CObList lstGroup;                                        // stack, nBlockSize 10
//       if (pRow != NULL) pRow->GetGroupFromPane(pAutohideToolbar, lstGroup);  // 0x52380
//       if (pRow == NULL || lstGroup.GetCount() == 0) {          // count word +0x18
//           pAutohideToolbar->UnSetAutoHideMode(NULL);           // 0x8ed0
//       } else {
//           BOOL bFirst = TRUE; CDockablePane* pFirstBar = NULL;
//           for (POSITION pos = lstGroup.GetHeadPosition(); pos != NULL; ) {
//               CObject* p = lstGroup.GetNext(pos);
//               if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCAutoHideBar))) continue;  // 0x233310
//               if (bFirst) {
//                   pFirstBar = ((CMFCAutoHideBar*)p)->GetFirstAHWindow();   // 0x9720
//                   ((CMFCAutoHideBar*)p)->UnSetAutoHideMode(NULL);         // 0x8ed0
//                   bFirst = FALSE;
//               } else {
//                   ((CMFCAutoHideBar*)p)->UnSetAutoHideMode(pFirstBar);
//               }
//           }
//       }
//   }
//   <local list destroyed: CObList vftable store + RemoveAll 0x83d0>
// OpenMFC's CObList never updates its in-object count word (see CDockSite.cpp),
// so "GetCount() == 0" is tested as "no head node".  The pThis NULL guard is ours.
extern "C" void MS_ABI impl__UnSetAutoHideMode_CAutoHideDockSite__QEAAXPEAVCMFCAutoHideBar___Z(
    void* pThis, CMFCAutoHideBar* pAutohideToolbar) {
    if (pThis == nullptr) return;
    if (pAutohideToolbar == nullptr) {
        LocalObList lstBars;
        impl__AddTail_CObList__QEAAXPEAV1__Z(lstBars.get(), &Site(pThis)->m_lstControlBars);
        for (ObNode* n = HeadNode(lstBars.get()); n != nullptr; ) {
            ObNode* nSave = n;
            CPane* pBar = reinterpret_cast<CPane*>(n->data);
            n = n->pNext;
            if (!pBar->m_bFirstInGroup) {
                CObList::POSITION posSave = PosFromNode(nSave);
                impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(lstBars.get(), &posSave);
            }
        }
        for (ObNode* n = HeadNode(lstBars.get()); n != nullptr; ) {
            CMFCAutoHideBar* pBar = reinterpret_cast<CMFCAutoHideBar*>(n->data);
            n = n->pNext;
            impl__UnSetAutoHideMode_CAutoHideDockSite__QEAAXPEAVCMFCAutoHideBar___Z(pThis, pBar);
        }
        return;
    }

    void* pRow = impl__RowFromPane_CDockSite__IEBAPEAVCDockingPanesRow__PEAVCBasePane___Z(
        pThis, reinterpret_cast<CBasePane*>(pAutohideToolbar));
    LocalObList lstGroup;
    if (pRow != nullptr) {
        impl__GetGroupFromPane_CDockingPanesRow__QEAAXPEAVCPane__AEAVCObList___Z(
            pRow, reinterpret_cast<CPane*>(pAutohideToolbar), lstGroup.get());
    }
    if (pRow == nullptr || HeadNode(lstGroup.get()) == nullptr) {
        impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(pAutohideToolbar, nullptr);
        return;
    }

    bool bFirst = true;
    CDockablePane* pFirstBar = nullptr;
    for (ObNode* n = HeadNode(lstGroup.get()); n != nullptr; ) {
        CObject* p = n->data;
        n = n->pNext;
        if (p == nullptr ||
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                p, impl__GetThisClass_CMFCAutoHideBar__SAPEAUCRuntimeClass__XZ()))
            continue;
        CMFCAutoHideBar* pToolbar = reinterpret_cast<CMFCAutoHideBar*>(p);
        if (bFirst) {
            pFirstBar = static_cast<CDockablePane*>(
                impl__GetFirstAHWindow_CMFCAutoHideBar__QEAAPEAVCDockablePane__XZ(pToolbar));
            impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(pToolbar, nullptr);
            bFirst = false;
        } else {
            impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(pToolbar, pFirstBar);
        }
    }
}

// Symbol: ?m_nExtraSpace@CAutoHideDockSite@@1HA
// Retail: an initialised .data int at 0x3aaaa0 (mfc140) / RVA 0x3b1aa0 (mfc140u);
// both images hold the value 2 there.
extern "C" int impl__m_nExtraSpace_CAutoHideDockSite__1HA = 2;
