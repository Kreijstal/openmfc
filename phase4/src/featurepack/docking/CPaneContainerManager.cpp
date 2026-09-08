// CPaneContainerManager — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// The bodies marked "decoded from retail" below were transcribed from the retail
// export disassembly (disas.py against
// /home/kreijstal/msvc/VC/Redist/MSVC/14.51.36231/x64/Microsoft.VC145.MFC/mfc140.dll,
// the ANSI twin of the mfc140u.dll OpenMFC reimplements). Function bodies are
// byte-identical between the two images; only the export RVAs differ, so every
// RVA quoted here is an mfc140.dll RVA and is named as such.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

#include <cstddef>

// ---------------------------------------------------------------------------
// Retail CPaneContainerManager instance layout
//
// CPaneContainerManager has no declaration in include/openmfc/, so this
// translation unit models the retail object directly. Every offset below was
// read out of the retail constructor ??0CPaneContainerManager@@QEAA@XZ (entry
// RVA 0xa80b0, mfc140.dll), which writes exactly these slots, and matches the
// shipping atlmfc header afxpanecontainermanager.h member order:
//
//   +0x00  CObject vfptr
//   +0x08  CObList m_lstControlBars      (sizeof(CObList) == 0x38)
//   +0x40  CObList m_lstSliders
//   +0x78  CPaneContainer* m_pRootContainer
//   +0x80  CRuntimeClass*  m_pContainerRTC
//   +0x88  CPaneDivider*   m_pDefaultSlider
//   +0x90  BOOL            m_bDestroyRootContainer   (ctor stores 1)
//   +0x98  CWnd*           m_pDockSite
//   sizeof == 0xa0
//
// sizeof is not a guess: ?CreateObject@CPaneContainerManager@@SAPEAVCObject@@XZ
// (entry RVA 0xa8070) does `mov $0xa0,%ecx; call operator new` before running
// the constructor, and docking/RuntimeClasses.cpp already records 160 for this
// class.
//
// The two embedded CObLists are confirmed by every list-walking body in this
// file: they load the head pointer from +0x10 (= list base 0x08 + CObList's
// m_pNodeHead at +0x08) and from +0x48 (= 0x40 + 0x08), and step nodes with
// `node->pNext` at +0x00 / `node->data` at +0x10. ?IsEmpty@ (entry RVA 0xaa8e0)
// tests +0x20, which is m_lstControlBars.m_nCount.
//
// LIST REPRESENTATION -- deliberate, file-wide deviation.
// OpenMFC's CObList keeps its contents in a side table keyed by `this` and never
// populates the raw m_pNodeHead/m_nCount words (see the comment on
// OPENMFC_DECLARE_LIST_WRAPPER in include/openmfc/afx.h), and its iteration
// accessors (GetHeadPosition/GetNext/GetAt) are inline in retail so no impl__
// thunk exists to reach them from here. Reading the retail offsets would
// therefore always report an empty list. Instead this file keeps the pane list
// where the already-implemented entry points in it keep it -- the shared
// g_containerManagerPanes side table that ??0CPaneContainerManager, AddPane,
// GetFirstPane and CPaneContainer's constructor all use -- and keeps the slider
// list in the file-local table below. Both are ordered vectors, so the
// head-to-tail walk order retail relies on is preserved.
// ---------------------------------------------------------------------------

namespace {

constexpr size_t kOffRootContainer        = 0x78;
constexpr size_t kOffContainerRTC         = 0x80;
constexpr size_t kOffDefaultSlider        = 0x88;
constexpr size_t kOffDestroyRootContainer = 0x90;
constexpr size_t kOffDockSite             = 0x98;
constexpr size_t kOffLstControlBars       = 0x08;
constexpr size_t kOffLstSliders           = 0x40;
constexpr size_t kSizeCPaneContainerManager = 0xa0;
static_assert(kOffLstSliders - kOffLstControlBars == 0x38, "sizeof(CObList) == 0x38");
static_assert(kOffDockSite + 8 == kSizeCPaneContainerManager, "CPaneContainerManager tail");

// CPaneContainer::m_pSlider, +0x18. Same offset the sibling shadow struct in
// docking/CPaneContainer.cpp pins; retail ?RemovePaneDivider@ (entry RVA
// 0xa9400) both compares and clears it there.
constexpr size_t kOffContainerSlider = 0x18;
// NOTE -- CPaneDivider::m_pContainerManager (+0x208 in retail) is deliberately
// NOT modelled here, and there is no kOffDividerManager constant any more.
// Retail ?CreatePaneDivider@ (entry RVA 0xa92e0) stores `this` there and
// ?RemovePaneDivider@ (0xa9400) clears it, and RuntimeClasses.cpp does record
// 536 (0x218) as CPaneDivider's m_nObjectSize -- but that is the RETAIL size.
// The CPaneDivider this DLL actually builds is the one declared in
// include/openmfc/afxmfc.h (`class CPaneDivider : public CObject` plus a
// 16-byte pad), and sizeof of that is 24. Writing at +0x208 into an object
// DynCreateFactories.cpp allocated with `new CPaneDivider()` overruns it by
// half a kilobyte. An earlier revision of this file did exactly that, in both
// entry points, justified by the 0x218 figure; both stores are gone.
// CDockablePane::m_bIsResizing, +0x46c. The member order in the shipping
// afxdockablepane.h puts it right after CRect m_rectRestored (+0x458) and
// BOOL m_bIsSliding (+0x468); docking/CDockablePane.cpp's layout comment pins
// the same offset. Retail ?SetResizeMode@ (entry RVA 0xa9f20) stores its
// argument there for every entry of m_lstControlBars.
constexpr size_t kOffPaneIsResizing = 0x46c;
// CPaneContainer::m_pContainerManager, +0x38, and sizeof(CPaneContainer) ==
// 0xf8. Both are read straight out of retail ?Create@CPaneContainerManager@@
// (entry RVA 0xa8230, mfc140.dll): the CRuntimeClass path stores `this` at
// 0x38(%rax) of the freshly created container (0xa8266) and the default path
// does `mov $0xf8,%ecx; call operator new` (0xa826c). The sibling shadow struct
// in docking/CPaneContainer.cpp pins both with static_asserts.
constexpr size_t kOffContainerManagerOfContainer = 0x38;
constexpr size_t kSizeCPaneContainer            = 0xf8;

// CBRS_ALIGN_*, as spelled in the shipping atlmfc afxres.h and as tested by
// retail ?CalcRects@ (entry RVA 0xa95d0: `test $0xa000,%r11d`, then
// `and $0xf000,%r11d` and four compares against 0x1000/0x2000/0x4000/0x8000).
// They are NOT taken from include/openmfc/afxole.h: that header defines
// CBRS_ALIGN_ANY as 0x000F, which is not the value retail masks with.
constexpr unsigned long kCbrsAlignLeft   = 0x1000UL;
constexpr unsigned long kCbrsAlignTop    = 0x2000UL;
constexpr unsigned long kCbrsAlignRight  = 0x4000UL;
constexpr unsigned long kCbrsAlignBottom = 0x8000UL;
constexpr unsigned long kCbrsAlignAny    = 0xF000UL;

// CPaneDivider::SS_STYLE, from the shipping afxpanedivider.h
// (SS_HORZ = 0x1, SS_VERT = 0x2); retail ?CalcRects@ writes 1 on the
// top/bottom path and 2 on the left/right path.
constexpr unsigned long kSliderStyleHorz = 0x1UL;
constexpr unsigned long kSliderStyleVert = 0x2UL;

// BC_FIND_CRITERIA::BC_FIND_BY_SLIDER, as dispatched by
// ?FindSubPaneContainer@CPaneContainer@@ (entry RVA 0xa5230).
constexpr int kFindBySlider = 2;

// WS_EX_LAYOUTRTL. Retail ?OnPaneDividerMove@ (entry RVA 0xa8e10) masks
// CWnd::GetExStyle() with 0x400000 at 0xa8eb9 and uses the result to pick which
// edge of the root rectangle the offset is applied to. The Windows SDK spells
// the same bit WS_EX_LAYOUTRTL; it is named locally because the mingw headers
// this file compiles against do not always define it.
constexpr unsigned long kExLayoutRtl = 0x00400000UL;

template <class T> inline T& Field(void* p, size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& Field(const void* p, size_t off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

// m_lstSliders. Nothing outside this translation unit needs it, and nothing in
// this one appends to it: both retail entry points that would -- ?CreatePaneDivider@
// and ?AddPaneContainerManager@ -- are stubs here (see ?CreatePaneDivider@ for
// why building a CPaneDivider is not safe in this DLL). So the table is only
// ever read and erased. It is kept anyway so the loops below have the shape
// retail has, rather than silently dropping the slider half of each walk.
//
// It must stay empty until CPaneDivider has retail-shaped storage: the four
// slider walks in this file (?AddPanesToList@, ?HideAll@, ?GetAvailableSpace@,
// ?RemoveAllPanesAndPaneDividers@) hand each entry to ?GetStyle@CWnd@@,
// ?ShowWindow@CWnd@@ or CWnd::GetSafeHwnd, all of which read CWnd::m_hWnd --
// far past the end of the 24-byte CPaneDivider this DLL actually allocates.
std::unordered_map<const void*, PaneState>& SliderStates() {
    static std::unordered_map<const void*, PaneState> s_sliders;
    return s_sliders;
}
std::vector<void*>& SliderList(const void* pThis) {
    return SliderStates()[pThis].panes;
}
void EraseSliderList(const void* pThis) {
    SliderList(pThis).clear();
}

// Live-list accessors. The list walks below run with g_wave2Mutex held for the
// whole loop rather than over a copy: g_wave2Mutex is not recursive, and none of
// the thunks these loops call takes it (?IsKindOf@CObject@, ?GetStyle@CWnd@,
// ?ShowWindow@CWnd@, ?EnableGripper@CDockablePane@,
// ?SetDefaultPaneDivider@CDockablePane@, ?AddTail@CObList@ -- which uses
// g_collectionStateMutex -- and the USER32 rect calls). The two entry points
// that do have to call a g_wave2Mutex taker (ReplacePane -> FindPaneContainer,
// RemovePaneDivider -> nothing locking, but it re-reads the manager) drop the
// lock first; that is called out where it happens.
std::vector<void*>* PaneListLocked(const void* pThis) {
    auto it = g_containerManagerPanes.find(const_cast<void*>(pThis));
    return it == g_containerManagerPanes.end() ? nullptr : &it->second.panes;
}

} // namespace

// ---------------------------------------------------------------------------
// Cross-class entry points. BRIEFING S1: the C++ methods do not exist inside
// this DLL, only these exports, so every call goes through the thunk.
//
// Every list below is the retail-correct one, derived from the mangled name.
// Most of the sibling definitions now match it. Three still do not -- they are
// auto-generated placeholders whose parameter list omits the implicit `this`:
// ?EnableGripper@CDockablePane@@ (CDockablePane.cpp), ?SetDefaultPaneDivider@
// CDockablePane@@ (CDockablePane.cpp) and ?NotifyAboutRelease@CPaneDivider@@
// (CPaneDivider.cpp). The retail exports all take `this` in %rcx and those three
// placeholder bodies read none of their arguments, so calling them through the
// retail-correct list is harmless under the MS x64 ABI: the extra register
// argument is simply ignored. (Line numbers are deliberately not quoted here --
// the sibling files are edited independently and any number would go stale.)
// ---------------------------------------------------------------------------
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();

extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);

extern "C" int MS_ABI impl__CanFloat_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__EnableGripper_CDockablePane__UEAAXH_Z(void* pThis, int bEnable);
extern "C" void MS_ABI impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(
    void* pThis, HWND hSliderWnd);

extern "C" int MS_ABI impl__GetNodeCount_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__IsVisible_CPaneContainer__QEBAHXZ(const void* pThis);
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(
    void* pThis, unsigned int nID);
extern "C" void MS_ABI impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(
    const void* pThis, CSize& size);
extern "C" void MS_ABI impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(
    const void* pThis, CRect& rect, int bIgnoreVisibility);
extern "C" void MS_ABI impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(
    void* pThis, const RECT* pRect, void** phdwp, int bRedraw);
extern "C" int MS_ABI impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
    void* pThis, int nOffset, int bStretchHorz, int bLeftBar, int bMoveSlider, void** phdwp);
extern "C" void* MS_ABI impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
    void* pThis, const void* pObject, int criteria);
extern "C" void MS_ABI impl__SetPane_CPaneContainer__QEAAXPEAVCDockablePane__H_Z(
    void* pThis, void* pPane, int bLeftPane);

extern "C" void MS_ABI impl__NotifyAboutRelease_CPaneDivider__UEAAXXZ(void* pThis);

extern "C" void MS_ABI impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneContainer__UEAAXPEAVCDockablePane___Z(
    void* pThis, void* pBar);
extern "C" void* MS_ABI impl___0CPaneContainer__QEAA_PEAVCPaneContainerManager__PEAVCDockablePane__1PEAVCPaneDivider___Z(
    void* pThis, void* pManager, void* pFirstPane, void* pSecondPane, void* pDivider);

extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// ?m_nDefaultWidth@CPaneDivider@@2HA -- the exported static that retail
// ?CalcRects@ reads at 0x1803aab0c (mfc140.dll) when it reserves the divider
// band. Its definition is docking/StaticData.cpp:82.
extern "C" int impl__m_nDefaultWidth_CPaneDivider__2HA;

extern "C" void* MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
    CObList* pThis, CObject* value);

// Same-file entry point, forward declared so ReplacePane can reach it the way
// retail does (retail calls it through this->vftable slot 0x128, which for an
// unsubclassed CPaneContainerManager is exactly this body).
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pIndex);

// --- Added for the OnPaneDividerMove / PaneFromPoint bodies below.
// ?OnMoveInternalPaneDivider@CPaneContainer@@ used to be an auto-generated
// placeholder whose list dropped the implicit `this`; docking/CPaneContainer.cpp
// now defines it with the (this, nOffset, phdwp) list used here, and with a real
// body. (The CPaneDivider declarations that used to sit in this block --
// ?Init@, ?CreateEx@, ??1CPaneDivider@@, ??3@YAXPEAX@Z and
// ?m_pSliderRTC@CPaneDivider@@ -- went away with the ?CreatePaneDivider@ body
// they served; see that function's comment.)
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);

extern "C" int MS_ABI impl__OnMoveInternalPaneDivider_CPaneContainer__UEAAHHAEAPEAX_Z(
    void* pThis, int nOffset, void** phdwp);

extern "C" int MS_ABI impl__GetCaptionHeight_CDockablePane__UEBAHXZ(const void* pThis);
extern "C" int MS_ABI impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(
    void* pThis, void* point, int bDetectCaption);

// Same-file entry point, forward declared so OnPaneDividerMove can reach it the
// way retail does (retail calls it through this->vftable slot 0x80, which for an
// unsubclassed CPaneContainerManager is exactly that body).
extern "C" void MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(
    void* pThis, const RECT* pRect, void** phdwp);

namespace {

// DYNAMIC_DOWNCAST, the way every list walk in the retail bodies open-codes it:
// `if (p != NULL && p->IsKindOf(pRTC)) use p; else use NULL;`.
void* DynDowncast(void* p, CRuntimeClass* pRTC) {
    if (p == nullptr || pRTC == nullptr) return nullptr;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               static_cast<const CObject*>(p), pRTC) ? p : nullptr;
}

} // namespace

// Symbol: ??0CPaneContainerManager@@QEAA@XZ
// Decoded from retail ??0CPaneContainerManager@@QEAA@XZ, entry RVA 0xa80b0
// (mfc140.dll): the ctor stores the CPaneContainerManager vftable at +0x00 and
// the CObList vftable at +0x08 / +0x40, zeroes both lists' head/tail/count/free/
// blocks words, stores 10 (CObList's default block size) into both m_nBlockSize
// slots (+0x38, +0x70), sets m_bDestroyRootContainer (+0x90) to 1 and NULLs
// m_pRootContainer (+0x78), m_pContainerRTC (+0x80), m_pDefaultSlider (+0x88)
// and m_pDockSite (+0x98).
//
// DEVIATION: the three vftable slots are left NULL instead. OpenMFC publishes no
// vtable for CPaneContainerManager (the class has no C++ declaration) and none
// for the CObList sub-objects that a caller could reach from here, so there is
// nothing correct to store; NULL at least turns a stray virtual call into a
// clean null dereference instead of a jump through uninitialised memory. Zeroing
// the scalar members is what makes every body below that reads m_pRootContainer
// and friends safe -- before this the object was left entirely uninitialised.
extern "C" void* MS_ABI impl___0CPaneContainerManager__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    std::memset(pThis, 0, kSizeCPaneContainerManager);
    Field<INT_PTR>(pThis, kOffLstControlBars + 0x30) = 10;  // m_lstControlBars.m_nBlockSize
    Field<INT_PTR>(pThis, kOffLstSliders + 0x30)     = 10;  // m_lstSliders.m_nBlockSize
    Field<int>(pThis, kOffDestroyRootContainer)      = TRUE;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerManagerPanes[pThis] = {};
    EraseSliderList(pThis);
    return pThis;
}
// Symbol: ??1CPaneContainerManager@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPaneContainerManager__UEAA_XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_containerManagerPanes.erase(pThis);
    EraseSliderList(pThis);
    return pThis;
}
// Symbol: ?AddPane@CPaneContainerManager@@UEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__AddPane_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(void* pThis, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_containerManagerPanes[pThis], pPane);
}
// Symbol: ?FindPane@CPaneContainerManager@@IEAAIVCPoint@@PEAPEAVCPane@@AEAPEAU__POSITION@@@Z
extern "C" unsigned int MS_ABI impl__FindPane_CPaneContainerManager__IEAAIVCPoint__PEAPEAVCPane__AEAPEAU__POSITION___Z(
    void* pThis, CPoint, void** ppPane, void** ppPos) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    void* foundPane = FirstPane(g_containerManagerPanes[pThis]);
    if (ppPane) *ppPane = foundPane;
    if (ppPos) *ppPos = nullptr;
    return foundPane != nullptr ? 1U : 0U;
}
// Symbol: ?FindPane@CPaneContainerManager@@IEAAIVCRect@@PEAPEAVCPane@@AEAPEAU__POSITION@@@Z
extern "C" unsigned int MS_ABI impl__FindPane_CPaneContainerManager__IEAAIVCRect__PEAPEAVCPane__AEAPEAU__POSITION___Z(
    void* pThis, const CRect&, void** ppPane, void** ppPos) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    void* foundPane = FirstPane(g_containerManagerPanes[pThis]);
    if (ppPane) *ppPane = foundPane;
    if (ppPos) *ppPos = nullptr;
    return foundPane != nullptr ? 1U : 0U;
}
// Symbol: ?FindPaneContainer@CPaneContainerManager@@UEAAPEAVCPaneContainer@@PEAVCDockablePane@@AEAH@Z
extern "C" void* MS_ABI impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
    void* pThis, void* pPane, int* pIndex) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)pThis;
    if (pIndex) *pIndex = 0;
    auto it = g_paneToContainer.find(pPane);
    return it == g_paneToContainer.end() ? nullptr : it->second;
}
// Symbol: ?GetFirstPane@CPaneContainerManager@@UEBAPEAVCBasePane@@XZ
extern "C" void* MS_ABI impl__GetFirstPane_CPaneContainerManager__UEBAPEAVCBasePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_containerManagerPanes[pThis]);
}
// Symbol: ?GetFirstVisiblePane@CPaneContainerManager@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneContainerManager__UEBAPEAVCWnd__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_containerManagerPanes[pThis]);
}

// Symbol: ?AddPaneAndPaneContainer@CPaneContainerManager@@IEAAHPEAVCDockablePane@@PEAVCPaneContainer@@K@Z
// Retail entry RVA 0xa9770 (mfc140.dll), 163 instructions up to the next export
// at 0xa9a00. (An earlier revision
// of this comment cited 0xa8080; that is not this function's entry -- disas.py
// resolves the export to 0xa9770.) It returns FALSE at once when
// m_pRootContainer is NULL, then collects pBarOriginal's minimum size
// (CDockablePane vtable +0x4f0) and window rect and pContainerToInsert's
// (CPaneContainer vtable +0x30 / +0x28), maps them into the dock site's client
// space (CWnd::ScreenToClient, 0x2a11f0), calls CalcRects (0xa95d0) to split
// them and CreatePaneDivider (0xa92e0) to build the new divider, then
// repositions both halves through CDockablePane vtable +0x478 and
// CPaneContainer vtable +0xa0 / +0x70 before splicing the node in.
// Left a stub: ?CreatePaneDivider@ below is itself a documented stub returning
// NULL -- this DLL cannot build a CPaneDivider at all (see the three reasons in
// that function's comment) -- so the divider this body exists to insert between
// the two halves cannot be made. ?Resize@CPaneContainer@@ -- the CPaneContainer vtable +0xa0
// call that re-lays the inserted container -- is also still a stub in
// docking/CPaneContainer.cpp. The geometry inputs, on the other hand, are all
// available now: ?GetWindowRect@CPaneContainer@@, ?GetMinSize@CPaneContainer@@,
// this file's ?CalcRects@, CPaneContainer vtable +0x70
// (?Move@CPaneContainer@@) and CDockablePane vtable +0x478
// (?MoveWindow@CBasePane@@) all have real bodies. An earlier revision of this
// comment called four of those stubs; only Resize still is.
extern "C" int MS_ABI impl__AddPaneAndPaneContainer_CPaneContainerManager__IEAAHPEAVCDockablePane__PEAVCPaneContainer__K_Z(
    void* pThis, void* pBarOriginal, void* pContainerToInsert, unsigned long dwAlignment) {
    (void)pThis; (void)pBarOriginal; (void)pContainerToInsert; (void)dwAlignment;
    return 0;   // STUB -- see above
}

// Symbol: ?AddPaneAndPaneDivider@CPaneContainerManager@@IEAAHPEAVCDockablePane@@0PEAU__POSITION@@K@Z
// Retail entry RVA 0xa9a00 (mfc140.dll), 293 instructions. (An earlier revision
// of this comment cited 0xa9770; that RVA is AddPaneAndPaneContainer.) It
// returns FALSE when m_pRootContainer is NULL or when dwAlignment masked with
// CBRS_ALIGN_ANY is neither LEFT/TOP nor RIGHT/BOTTOM, otherwise splices
// pBarToInsert into m_lstControlBars with CObList::InsertBefore (0x230630) or
// InsertAfter (0x230690) at posNearestBar, takes both panes' minimum sizes
// (CDockablePane vtable +0x4f0) and window rects, and then runs the same
// CalcRects / CreatePaneDivider geometry as AddPaneAndPaneContainer.
// Left a stub for the same reason: ?CreatePaneDivider@ is a stub returning NULL
// (see its comment), so the divider this body exists to insert cannot be made.
extern "C" int MS_ABI impl__AddPaneAndPaneDivider_CPaneContainerManager__IEAAHPEAVCDockablePane__0PEAU__POSITION__K_Z(
    void* pThis, void* pBarOriginal, void* pBarToInsert, void* posNearestBar, unsigned long dwAlignment) {
    (void)pThis; (void)pBarOriginal; (void)pBarToInsert; (void)posNearestBar; (void)dwAlignment;
    return 0;   // STUB -- see above
}

// Symbol: ?AddPaneContainerManager@CPaneContainerManager@@UEAAHAEAV1@H@Z
// STUB. Retail entry RVA 0xa82e0 (mfc140.dll), body 0xa82e0..0xa853e with the
// throw tail at 0xa853f. The shipping afxpanecontainermanager.h declares it
// `BOOL AddPaneContainerManager(CPaneContainerManager& srcManager, BOOL bOuterEdge)`.
// What it actually does (an earlier revision of this comment said it builds two
// temporary CObLists on the stack -- it does not; it clears and reuses its own
// two member lists):
//
//   if (m_pRootContainer == NULL) AfxThrowInvalidArgException();   // 0xa8313
//   if (!m_pRootContainer->IsEmpty()) return FALSE;                // 0xa8319
//   CDockingManager* pMgr = afxGlobalUtils.GetDockingManager(m_pDockSite); // 0x6ccc0
//   if (pMgr == NULL) return FALSE;
//   m_lstControlBars.RemoveAll();  m_lstSliders.RemoveAll();       // 0xa834f/0xa835b
//   srcManager.AddPanesToList(&m_lstControlBars, &m_lstSliders);   // 0xa8369
//   srcManager.RemoveAllPanesAndPaneDividers();                    // 0xa8371
//   CPaneContainer* pCopy =
//       srcManager.m_pRootContainer->Copy(m_pRootContainer);       // vtable +0xb0
//   m_pRootContainer->m_pLeftPaneContainer = pCopy;                // +0x20
//   if (pCopy != NULL) pCopy->m_pParentContainer = m_pRootContainer;  // +0x30
//   pCopy->SetPaneContainerManager(this, TRUE);                    // 0xa7c00
//   afxGlobalUtils.SetNewParent(m_lstControlBars, m_pDockSite, TRUE);  // 0x6cef0
//   afxGlobalUtils.SetNewParent(m_lstSliders,     m_pDockSite, TRUE);
//   for (each o in m_lstControlBars) {                             // 0xa83dd
//       CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, o);
//       CRect r; ::GetWindowRect(p->m_hWnd, &r);
//       CWnd::FromHandle(::GetParent(p->m_hWnd))->ScreenToClient(&r);  // r discarded
//   }
//   for (each o in m_lstSliders) o->m_pContainerManager = this;    // +0x208
//   for (each o in m_lstControlBars) {                             // 0xa8458
//       CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, o);
//       p->SetDefaultPaneDivider(m_pDefaultSlider->m_hWnd);        // 0x455f0
//       p->{vtable +0x3d0}(m_pDefaultSlider->GetCurrentAlignment());  // vt +0x338
//       pMgr->AddPane(p, bOuterEdge == 0, FALSE, bOuterEdge);      // 0x476b0
//       p->EnableGripper(TRUE);                                    // vtable +0x3f0
//   }
//   m_pRootContainer->CheckPaneDividerVisibility();  m_pRootContainer->CalculateRecentSize();
//   return TRUE;
//
// Left a stub: ?Copy@CPaneContainer@@ (the vtable +0xb0 target, 0xa7ca0) is a
// stub in docking/CPaneContainer.cpp -- it returns NULL, so the clone this merge
// exists to produce is never made. (?SetPaneContainerManager@CPaneContainer@@ is
// implemented there now; an earlier revision of this comment called it a stub
// too.) ?SetNewParent@CGlobalUtils@@ and
// ?AddPane@CDockingManager@@ are not reachable from here, and CDockablePane
// vtable +0x3d0 is not identified -- so the copy that is the whole point of the
// merge cannot be made.
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHAEAV1_H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?AddPaneContainerManager@CPaneContainerManager@@UEAAHPEAVCDockablePane@@KAEAV1@H@Z
// STUB. Retail entry RVA 0xa8550 (mfc140.dll); the body runs to the `ret` at
// 0xa88d9 by way of the early `ret` at 0xa8639. (An earlier revision of this
// comment said 401 instructions; that is how many disas.py prints before the
// next known export, not this function's length.) The shipping header declares
// `BOOL AddPaneContainerManager(CDockablePane* pTargetControlBar, DWORD dwAlignment,
//                               CPaneContainerManager& srcManager, BOOL bCopy)`.
// Unlike the two-argument overload this one really does build two CObLists on
// the stack (their vftable, 0x180328538, is stored at 0xa8580/0xa85a4), fills
// them with srcManager.AddPanesToList (0xa85c5), then locates the target with
// this->FindPaneContainer (vtable +0x128, 0xa85e2) and a walk of
// m_lstControlBars; when either lookup fails it empties both stack lists and
// returns at 0xa8639. The success path runs the same
// SetPaneContainerManager / RemoveAllPanesAndPaneDividers / SetNewParent
// sequence as the two-argument overload, calls AddPaneAndPaneContainer
// (0xa9770), splices the moved panes in with CObList InsertBefore (0x230630),
// InsertAfter (0x230690) and AddTail (0x230490), then ::RedrawWindow's the dock
// site and finishes with CheckPaneDividerVisibility and CalculateRecentSize.
// Left a stub for the same reasons as the overload above, plus
// AddPaneAndPaneContainer, which is itself a stub in this file.
extern "C" int MS_ABI impl__AddPaneContainerManager_CPaneContainerManager__UEAAHPEAVCDockablePane__KAEAV1_H_Z(void* /*class*/* p0, unsigned long p1, void* /*class*/* p2, int p3) {
    return 0;
}

// Symbol: ?AddPaneContainerManagerToDockablePane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@AEAV1@@Z
// STUB. Retail entry RVA 0xa8820 (mfc140.dll), body 0xa8820..0xa89c1 with the
// throw tail at 0xa89c2. (An earlier revision of this comment said 313
// instructions and named a CPaneContainer::Copy dependency; 313 is the
// print-until-next-export count, and there is no Copy call in this body.)
// The shipping header declares
// `BOOL AddPaneContainerManagerToDockablePane(CDockablePane* pTargetControlBar,
//                                             CPaneContainerManager& srcManager)`.
//
//   CObList lstBars;                                   // stack, vftable 0x180328538
//   srcManager.AddPanesToList(&lstBars, NULL);         // 0xa8876, r8 = NULL
//   BOOL bLeftBar = FALSE;
//   if (this->FindPaneContainer(pTargetControlBar, bLeftBar) == NULL ||   // vt +0x128
//       <pTargetControlBar is not in m_lstControlBars>)
//       { lstBars.RemoveAll(); return FALSE; }         // 0xa88b5..0xa88d9
//   ... two IsKindOf tests (0xa88ee, 0xa8923) and one virtual call (0xa8954) ...
//   return AddPaneAndPaneDivider(...);                 // 0xa89b4 -> 0xa9a00
//
// Left a stub: AddPaneAndPaneDivider is itself a stub in this file, and the
// middle section depends on the same CPaneContainer plumbing the overloads above
// need. The two IsKindOf descriptors and the virtual slot were not resolved, so
// nothing is claimed about them here.
extern "C" int MS_ABI impl__AddPaneContainerManagerToDockablePane_CPaneContainerManager__UEAAHPEAVCDockablePane__AEAV1__Z(void* /*class*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?AddPanesToList@CPaneContainerManager@@QEAAXPEAVCObList@@0@Z
// Decoded from retail ?AddPanesToList@CPaneContainerManager@@QEAAXPEAVCObList@@0@Z,
// entry RVA 0xaa960 (mfc140.dll):
//
//   if (plstControlBars != NULL)
//       for (each object o in m_lstControlBars) {
//           CWnd* p = DYNAMIC_DOWNCAST(CWnd, o);
//           if (p->GetStyle() & WS_VISIBLE) plstControlBars->AddTail(p);
//       }
//   if (plstSliders != NULL)
//       for (each object o in m_lstSliders) { ... same, into plstSliders ... }
//
// (the RTTI pointer loaded before each IsKindOf is RUNTIME_CLASS(CWnd) at
// 0x1803352d0 in mfc140.dll; the WS_VISIBLE test is `bt $0x1c,%eax` on the
// GetStyle result).
//
// DEVIATION: retail calls GetStyle on the downcast result even when the downcast
// produced NULL; this skips those entries instead of faulting. The destination
// lists are real caller-owned CObLists, so those AddTail calls go through the
// CObList export.
extern "C" void MS_ABI impl__AddPanesToList_CPaneContainerManager__QEAAXPEAVCObList__0_Z(
    void* pThis, void* plstControlBars, void* plstSliders) {
    if (pThis == nullptr) return;
    CRuntimeClass* pWndClass = impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    if (plstControlBars != nullptr) {
        std::vector<void*>* panes = PaneListLocked(pThis);
        if (panes != nullptr) {
            for (void* obj : *panes) {
                void* pWnd = DynDowncast(obj, pWndClass);
                if (pWnd == nullptr) continue;
                if (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pWnd)) & WS_VISIBLE) {
                    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
                        static_cast<CObList*>(plstControlBars), static_cast<CObject*>(pWnd));
                }
            }
        }
    }
    if (plstSliders != nullptr) {
        for (void* obj : SliderList(pThis)) {
            void* pWnd = DynDowncast(obj, pWndClass);
            if (pWnd == nullptr) continue;
            if (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pWnd)) & WS_VISIBLE) {
                impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
                    static_cast<CObList*>(plstSliders), static_cast<CObject*>(pWnd));
            }
        }
    }
}

// Symbol: ?AddPaneToList@CPaneContainerManager@@QEAAXPEAVCDockablePane@@@Z
// Decoded from retail ?AddPaneToList@CPaneContainerManager@@QEAAXPEAVCDockablePane@@@Z,
// entry RVA 0xaad0 -- correction, entry RVA 0xaaad0 (mfc140.dll). The whole body
// is `add $0x8,%rcx; jmp CObList::AddTail`, i.e. m_lstControlBars.AddTail(pBar).
//
// DEVIATION: AddUniquePane, the helper the rest of this file's pane bookkeeping
// uses, drops a pane that is already in the list; retail's AddTail would append
// a duplicate.
extern "C" void MS_ABI impl__AddPaneToList_CPaneContainerManager__QEAAXPEAVCDockablePane___Z(
    void* pThis, void* pPane) {
    if (pThis == nullptr) return;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    AddUniquePane(g_containerManagerPanes[pThis], pPane);
}

// Symbol: ?AddPaneToRecentPaneContainer@CPaneContainerManager@@UEAAPEAVCDockablePane@@PEAV2@PEAVCPaneContainer@@@Z
// STUB. Retail entry RVA 0xa89d0 (mfc140.dll), body 0xa89d0..0xa8ba5 with the
// throw tail at 0xa8bb4. (An earlier revision of this comment attributed the
// body to CRecentPaneContainerInfo; no call into that class appears in it. The
// "recent" state it uses lives in CPaneContainer itself, at +0x60 and +0x64.)
// The shipping header declares
// `CDockablePane* AddPaneToRecentPaneContainer(CDockablePane* pBarToAdd,
//                                              CPaneContainer* pRecentContainer)`.
//
//   if (pRecentContainer == NULL) AfxThrowInvalidArgException();   // 0xa89ec
//   CPaneContainer* p = m_pRootContainer;
//   if (p != pRecentContainer) {                                   // 0xa89f6
//       p = NULL;
//       if (m_pRootContainer->m_pLeftPaneContainer)                // +0x20
//           p = ...->FindSubPaneContainer(pRecentContainer, 3);    // 0xa5230
//       if (p == NULL && m_pRootContainer->m_pRightPaneContainer)  // +0x28
//           p = ...->FindSubPaneContainer(pRecentContainer, 3);
//   }
//   if (p == NULL) return NULL;
//   if (!p->IsEmpty() && p->m_pSlider == NULL)                     // 0xa8a40/+0x18
//       p->m_pSlider = CreatePaneDivider(*(CRect*)((char*)p + 0x64),
//                                        *(DWORD*)((char*)p + 0x60), -1);
//   if (p->IsEmpty())                                              // 0xa8a77
//       for (CPaneContainer* q = p->m_pParentContainer;            // +0x30
//            q != NULL && q != m_pRootContainer; q = q->m_pParentContainer)
//           if (q->m_pSlider == NULL && *(DWORD*)((char*)q + 0x60) != 0)
//               q->m_pSlider = CreatePaneDivider(*(CRect*)((char*)q + 0x64),
//                                                *(DWORD*)((char*)q + 0x60), -1);
//   CObList lst;                                                   // stack
//   lst.AddTail((char*)pBarToAdd +
//               (m_pDockSite->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)) ? 0x388 : 0x320));
//                                                  // 0xa8b04 neg/sbb/and $0x68
//   ...                                                            // 0xa8b1f loop
//   CDockablePane* pRet = p->AddPane(pBarToAdd);                   // 0xa4080
//   if (pRet == pBarToAdd) {
//       m_pRootContainer->CheckPaneDividerVisibility();            // 0xa71c0
//       <insert pRet into m_lstControlBars with AddHead (0x230440) or
//        InsertAfter (0x230690), depending on where the walk stopped>
//   }
//   lst.RemoveAll();
//   return pRet;
//
// Left a stub, but the reason has changed and the old one should not be repeated:
// ?AddPane@CPaneContainer@@ and ?FindSubPaneContainer@CPaneContainer@@ both have
// real bodies in docking/CPaneContainer.cpp now, and an earlier revision of this
// comment called them generated stubs. What actually blocks it:
//
//  * The divider. ?CreatePaneDivider@ above is a stub returning NULL (this DLL
//    cannot build a CPaneDivider -- see that comment), so the two calls here
//    produce nothing. That is harmless as a store (both sites only write an
//    m_pSlider the guard just found NULL), but it means a container that retail
//    would have given a divider is left without one, and ?AddPane@CPaneContainer@@
//    then puts a second pane into it. The resulting layout is wrong, not merely
//    incomplete.
//  * The recent-divider geometry. The CPaneContainer members at +0x60 (style) and
//    +0x64 (rect) that feed both CreatePaneDivider calls are never written in this
//    repo: docking/CPaneContainer.cpp zeroes them in the constructor, and their
//    only retail writer (0xa7de8) is a stub there.
//  * The insertion point. The stack CObList declared at 0xa8adb is filled by the
//    single CPtrList::AddTail at 0xa8b1a with ONE element -- `(char*)pBarToAdd +
//    (m_pDockSite is a CPaneFrameWnd ? 0x388 : 0x320)`, the neg/sbb/and $0x68/
//    add $0x320 sequence at 0xa8b04 -- and the walk at 0xa8b1f then looks in that
//    list for a node whose data equals pBarToAdd. Those two CDockablePane members
//    are not modelled here. (For whoever picks this up: 0x320 != 0, so in retail
//    that walk can never match, so the result always reaches the CObList::AddHead
//    call at 0xa8b83 rather than the InsertAfter at 0xa8bad. That is an observation about
//    retail's own arithmetic, not a licence to hard-code AddHead here -- this
//    file keeps m_lstControlBars in a side table with no POSITION to insert at.)
extern "C" void* MS_ABI impl__AddPaneToRecentPaneContainer_CPaneContainerManager__UEAAPEAVCDockablePane__PEAV2_PEAVCPaneContainer___Z(void* /*class*/* p0) {
    return nullptr;
}

// Symbol: ?CalcRects@CPaneContainerManager@@QEAAXAEAVCRect@@00AEAKKVCSize@@2@Z
// Decoded from retail ?CalcRects@CPaneContainerManager@@QEAAXAEAVCRect@@00AEAKKVCSize@@2@Z,
// entry RVA 0xa95d0 (mfc140.dll), 113 instructions, all of them pure integer
// geometry -- no call, no member of `this` is touched. The shipping
// afxpanecontainermanager.h declares it
//   void CalcRects(CRect& rectOriginal, CRect& rectInserted, CRect& rectSlider,
//                  DWORD& dwSliderStyle, DWORD dwAlignment,
//                  CSize sizeMinOriginal, CSize sizeMinInserted);
// and the argument slots the body reads match that exactly: rdx = rectOriginal,
// r8 = rectInserted, r9 = rectSlider, [rsp+0x28] = &dwSliderStyle,
// [rsp+0x30] = dwAlignment, [rsp+0x40] = sizeMinInserted. The sizeMinOriginal
// slot at [rsp+0x38] is never read -- that parameter is dead in retail.
//
//   // 0xa95d5..0xa960a -- grow rectInserted to sizeMinInserted
//   if (rectInserted.Width()  < sizeMinInserted.cx)
//       rectInserted.right  = rectInserted.left + sizeMinInserted.cx;
//   if (rectInserted.Height() < sizeMinInserted.cy)
//       rectInserted.bottom = rectInserted.top  + sizeMinInserted.cy;
//
//   int nSize; DWORD dwStyle;
//   if (dwAlignment & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM))   // test $0xa000
//   {                                                         // 0xa9618
//       dwStyle = CPaneDivider::SS_HORZ;                      // ebx = 1
//       rectInserted.left  = rectSlider.left  = rectOriginal.left;
//       rectInserted.right = rectSlider.right = rectOriginal.right;
//       nSize = min(rectInserted.Height(), rectOriginal.Height() / 2);
//   }
//   else                                                      // 0xa9650
//   {
//       dwStyle = CPaneDivider::SS_VERT;                      // ebx = 2
//       rectInserted.top    = rectSlider.top    = rectOriginal.top;
//       rectInserted.bottom = rectSlider.bottom = rectOriginal.bottom;
//       nSize = min(rectInserted.Width(), rectOriginal.Width() / 2);
//   }
//   dwSliderStyle = dwStyle;                                  // 0xa9693, once,
//                                                             // after the branch
//   const int nDiv = CPaneDivider::m_nDefaultWidth;           // 0x1803aab0c
//   switch (dwAlignment & CBRS_ALIGN_ANY) {                   // 0xa968c
//   case CBRS_ALIGN_LEFT:                                     // 0xa9737
//       rectInserted.left  = rectOriginal.left;
//       rectInserted.right = rectOriginal.left + nSize;
//       rectOriginal.left  = rectOriginal.left + nDiv + nSize;
//       rectSlider.left    = rectInserted.right;
//       rectSlider.right   = rectOriginal.left;   break;
//   case CBRS_ALIGN_TOP:                                      // 0xa970a
//       rectInserted.top    = rectOriginal.top;
//       rectInserted.bottom = rectOriginal.top + nSize;
//       rectOriginal.top    = rectOriginal.top + nDiv + nSize;
//       rectSlider.top      = rectInserted.bottom;
//       rectSlider.bottom   = rectOriginal.top;   break;
//   case CBRS_ALIGN_RIGHT:                                    // 0xa96e7
//       rectInserted.right = rectOriginal.right;
//       rectInserted.left  = rectOriginal.right - nSize;
//       rectOriginal.right = rectOriginal.right - nDiv - nSize;
//       rectSlider.left    = rectOriginal.right;
//       rectSlider.right   = rectInserted.left;   break;
//   case CBRS_ALIGN_BOTTOM:                                   // 0xa96c1
//       rectInserted.top    = rectOriginal.bottom - nSize;
//       rectInserted.bottom = rectOriginal.bottom;
//       rectOriginal.bottom = rectOriginal.bottom - nSize - nDiv;
//       rectSlider.top      = rectOriginal.bottom;
//       rectSlider.bottom   = rectInserted.top;   break;
//   default: break;                                           // 0xa975d
//   }
//
// Note the asymmetry in the two `nSize` computations is retail's, not a
// transcription slip: the top/bottom path compares rectInserted.Height()
// against rectOriginal.Height()/2 with `cmovle` on the inserted value, the
// left/right path compares rectInserted.Width() against rectOriginal.Width()/2
// with `cmovle` on the same side. Both are min(); the halving is the signed
// `cltd; sub %edx,%eax; sar $1,%eax` idiom, i.e. division truncating toward
// zero, which is what C++ `/ 2` does.
//
// DEVIATION: the three CRect& parameters are NULL-checked (retail would fault),
// and &dwSliderStyle is too. The generated stub's parameter list omitted the
// implicit `this` and mistyped both CSize slots; the list below is the
// retail-correct one, with each 8-byte CSize taken as a packed 64-bit value the
// way retail reads it (`mov 0x40(%rsp),%rax` for cx, `shr $0x20,%rax` for cy).
extern "C" void MS_ABI impl__CalcRects_CPaneContainerManager__QEAAXAEAVCRect__00AEAKKVCSize__2_Z(
    void* pThis, RECT* pRectOriginal, RECT* pRectInserted, RECT* pRectSlider,
    unsigned long* pdwSliderStyle, unsigned long dwAlignment,
    unsigned long long sizeMinOriginal, unsigned long long sizeMinInserted) {
    (void)pThis;             // retail never dereferences `this` here either
    (void)sizeMinOriginal;   // dead parameter in retail -- see above
    if (pRectOriginal == nullptr || pRectInserted == nullptr || pRectSlider == nullptr) return;

    const LONG cxMinInserted = static_cast<LONG>(sizeMinInserted & 0xffffffffULL);
    const LONG cyMinInserted = static_cast<LONG>(sizeMinInserted >> 32);
    if (pRectInserted->right - pRectInserted->left < cxMinInserted) {
        pRectInserted->right = pRectInserted->left + cxMinInserted;
    }
    if (pRectInserted->bottom - pRectInserted->top < cyMinInserted) {
        pRectInserted->bottom = pRectInserted->top + cyMinInserted;
    }

    unsigned long dwStyle;
    LONG nSize;
    if ((dwAlignment & (kCbrsAlignTop | kCbrsAlignBottom)) != 0) {
        dwStyle = kSliderStyleHorz;
        pRectSlider->left  = pRectInserted->left  = pRectOriginal->left;
        pRectSlider->right = pRectInserted->right = pRectOriginal->right;
        const LONG nHalf = (pRectOriginal->bottom - pRectOriginal->top) / 2;
        const LONG nHave = pRectInserted->bottom - pRectInserted->top;
        nSize = nHave <= nHalf ? nHave : nHalf;
    } else {
        dwStyle = kSliderStyleVert;
        pRectSlider->top    = pRectInserted->top    = pRectOriginal->top;
        pRectSlider->bottom = pRectInserted->bottom = pRectOriginal->bottom;
        const LONG nHalf = (pRectOriginal->right - pRectOriginal->left) / 2;
        const LONG nHave = pRectInserted->right - pRectInserted->left;
        nSize = nHave <= nHalf ? nHave : nHalf;
    }
    if (pdwSliderStyle != nullptr) *pdwSliderStyle = dwStyle;

    const LONG nDiv = impl__m_nDefaultWidth_CPaneDivider__2HA;
    switch (dwAlignment & kCbrsAlignAny) {
    case kCbrsAlignLeft:
        pRectInserted->left  = pRectOriginal->left;
        pRectInserted->right = pRectOriginal->left + nSize;
        pRectOriginal->left  = pRectOriginal->left + nDiv + nSize;
        pRectSlider->left    = pRectInserted->right;
        pRectSlider->right   = pRectOriginal->left;
        break;
    case kCbrsAlignTop:
        pRectInserted->top    = pRectOriginal->top;
        pRectInserted->bottom = pRectOriginal->top + nSize;
        pRectOriginal->top    = pRectOriginal->top + nDiv + nSize;
        pRectSlider->top      = pRectInserted->bottom;
        pRectSlider->bottom   = pRectOriginal->top;
        break;
    case kCbrsAlignRight:
        pRectInserted->right = pRectOriginal->right;
        pRectInserted->left  = pRectOriginal->right - nSize;
        pRectOriginal->right = pRectOriginal->right - nDiv - nSize;
        pRectSlider->left    = pRectOriginal->right;
        pRectSlider->right   = pRectInserted->left;
        break;
    case kCbrsAlignBottom:
        pRectInserted->top    = pRectOriginal->bottom - nSize;
        pRectInserted->bottom = pRectOriginal->bottom;
        pRectOriginal->bottom = pRectOriginal->bottom - nSize - nDiv;
        pRectSlider->top      = pRectOriginal->bottom;
        pRectSlider->bottom   = pRectInserted->top;
        break;
    default:
        break;
    }
}

// Symbol: ?CanBeAttached@CPaneContainerManager@@UEBAHXZ
// Decoded from retail ?CanBeAttached@CPaneContainerManager@@UEBAHXZ, entry RVA
// 0xaada0 (mfc140.dll):
//
//   for (each object o in m_lstControlBars) {
//       CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, o);
//       if (!p->CanBeAttached()) return FALSE;      // CBasePane vtable +0x328
//   }
//   return TRUE;                                    // TRUE for an empty list too
//
// CBasePane vtable slot +0x328 is CanBeAttached: in mfc140.dll's CBasePane
// vftable (0x1802d9fa8) it is the shared `xor %eax,%eax; ret` body at 0x7260,
// matching afxbasepane.h's inline `virtual BOOL CanBeAttached() const
// { return FALSE; }`, and CDockablePane's vftable (0x1802e3068) overrides the
// same slot with the shared `mov $1,%eax; ret` body at 0x3ae0.
//
// DEVIATION: called through OpenMFC's own CBasePane vtable rather than by slot
// index -- OpenMFC's CBasePane declares CanBeAttached as a virtual but the retail
// method is inline, so no impl__ thunk for it exists. Retail also calls through a
// NULL `this` when the downcast fails; that case is skipped here.
extern "C" int MS_ABI impl__CanBeAttached_CPaneContainerManager__UEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return TRUE;
    CRuntimeClass* pBasePaneClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return TRUE;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pBasePaneClass);
        if (p == nullptr) continue;
        if (!static_cast<const CBasePane*>(p)->CanBeAttached()) return FALSE;
    }
    return TRUE;
}

// Symbol: ?CheckAndRemoveNonValidPane@CPaneContainerManager@@QEAAHPEAVCWnd@@@Z
// Decoded from retail ?CheckAndRemoveNonValidPane@CPaneContainerManager@@QEAAHPEAVCWnd@@@Z,
// entry RVA 0xaad00 (mfc140.dll):
//
//   if (pWnd != NULL) {
//       int nID = pWnd->GetDlgCtrlID();
//       if (::IsWindow(pWnd->m_hWnd) && nID != -1) return TRUE;
//       if (pWnd->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)) &&
//           ((CBaseTabbedPane*)pWnd)->GetTabsNum() > 0)   // vtable +0x758
//           return TRUE;
//   }
//   <remove pWnd from m_lstControlBars if present>
//   return FALSE;
//
// (::IsWindow is the import at 0x1802c5390, resolved with iat.py; the RTTI
// pointer is RUNTIME_CLASS(CBaseTabbedPane) at 0x1802db168; vtable +0x758 in
// CBaseTabbedPane's vftable 0x1802db2a8 is the body at 0x12eb0, which is exactly
// afxbasetabbedpane.h's inline `GetTabsNum()` --
// `m_pTabWnd == NULL ? 0 : m_pTabWnd->GetTabsNum()`.)
//
// DEVIATION: the GetTabsNum branch is NOT implemented. OpenMFC's
// CBaseTabbedPane (include/openmfc/afxmfc.h) is a 96-byte padding blob with no
// m_pTabWnd and no GetTabsNum virtual, and CBaseTabbedPane::GetTabsNum is inline
// in retail so there is no thunk either. This body therefore behaves as if
// GetTabsNum() returned 0, i.e. a destroyed-or-unidentified tabbed pane is
// removed here where retail would keep it while it still has tabs.
extern "C" int MS_ABI impl__CheckAndRemoveNonValidPane_CPaneContainerManager__QEAAHPEAVCWnd___Z(
    void* pThis, void* pWnd) {
    if (pThis == nullptr) return FALSE;
    if (pWnd != nullptr) {
        const int nID = impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pWnd));
        if (::IsWindow(static_cast<const CWnd*>(pWnd)->GetSafeHwnd()) && nID != -1) return TRUE;
        // The GetTabsNum() half of retail's second test is unavailable -- see above.
        (void)impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
    }
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes != nullptr) {
        panes->erase(std::remove(panes->begin(), panes->end(), pWnd), panes->end());
    }
    return FALSE;
}

// Symbol: ?CheckForMiniFrameAndCaption@CPaneContainerManager@@UEAAHVCPoint@@PEAPEAVCDockablePane@@@Z
// Retail entry RVA 0xaab80 (mfc140.dll), 87 instructions. It writes
// *ppTargetControlBar = NULL and returns FALSE unless m_pDockSite (+0x98) is a
// CMultiPaneFrameWnd (IsKindOf against the CRuntimeClass at 0x1803aa2f8), then
// gives up when GetVisiblePaneCount() (this->vtable +0xb8) is greater than 1,
// and otherwise asks that mini-frame for its caption geometry through
// CPaneFrameWnd vtable slots +0x3a0, +0x388 and +0x2f0, offsets the frame's
// ::GetWindowRect by it and ::PtInRect-tests `point`; on a hit it takes
// GetFirstVisiblePane() (this->vtable +0x100) and returns it only if it is a
// CDockablePane (RTTI at 0x1803aa178).
// Left a stub: none of those three CPaneFrameWnd virtuals is identified here and
// OpenMFC has no CMultiPaneFrameWnd runtime class to test m_pDockSite against,
// so the IsKindOf guard at the top could not be reproduced faithfully. (?Create@
// in this file now does set m_pDockSite, so the guard is no longer unreachable
// for the reason an earlier revision of this comment gave.)
extern "C" int MS_ABI impl__CheckForMiniFrameAndCaption_CPaneContainerManager__UEAAHVCPoint__PEAPEAVCDockablePane___Z(void* /*class*/ p0, void* /*class*/** p1) {
    return 0;
}

// Symbol: ?Create@CPaneContainerManager@@UEAAHPEAVCWnd@@PEAVCPaneDivider@@PEAUCRuntimeClass@@@Z
// Decoded from retail ?Create@CPaneContainerManager@@UEAAHPEAVCWnd@@PEAVCPaneDivider@@PEAUCRuntimeClass@@@Z,
// entry RVA 0xa8230 (mfc140.dll). The body is 34 instructions, 0xa8230 through
// the `ret` at 0xa82b4, plus the throw path at 0xa82b5. (An earlier revision of
// this comment said 401 instructions; that is how many disas.py prints before
// the next known export, not this function's length.)
//
//   m_pDockSite     = pParentWnd;      // 0x98, stored at 0xa8245
//   m_pContainerRTC = pContainerRTC;   // 0x80, stored at 0xa824c
//   if (m_pRootContainer != NULL)      // tested at 0xa823a, BEFORE those two
//       AfxThrowInvalidArgException(); // 0xa82b5 -> 0x225b80, no return
//   if (pContainerRTC != NULL) {
//       m_pRootContainer = pContainerRTC->CreateObject();   // 0x233380
//       m_pRootContainer->m_pContainerManager = this;       // +0x38, unguarded
//   } else {
//       void* p = operator new(0xf8);                       // 0x002840
//       m_pRootContainer = p ? new (p) CPaneContainer(this, NULL, NULL, NULL)
//                            : NULL;                        // ctor 0xa3750
//   }
//   m_pDefaultSlider = pDefaultSlider; // 0x88, stored at 0xa829e
//   return TRUE;
//
// The two member stores at 0xa8245/0xa824c are unconditional and happen whether
// or not the m_pRootContainer test then throws; the ordering above matches the
// instruction order.
//
// DEVIATIONS, all called out because they are visible:
//  * The CreateObject result is NULL-checked before its +0x38 is written; retail
//    would fault. Likewise pThis.
//  * AfxThrowInvalidArgException is called through its export and this body then
//    returns FALSE. Retail never reaches the return, but the OpenMFC helper is
//    not guaranteed to be noreturn, so the FALSE keeps the object untouched.
//  * The freshly allocated block is zeroed before the CPaneContainer constructor
//    runs. Retail's constructor writes the CPaneContainer vftable at +0x00;
//    OpenMFC's exported constructor (docking/CPaneContainer.cpp) deliberately
//    does not, so without the memset the root container would carry whatever
//    operator new left at +0x00 and a client virtual call would jump through it.
//    NULL at least faults cleanly.
//  * Nothing frees the root container: retail's destructor honours
//    m_bDestroyRootContainer, and this file's destructor does not model that
//    yet. CPaneContainer::CleanUp is a stub in the sibling file for the same
//    reason (no emitted vtable to run a virtual destructor through).
extern "C" int MS_ABI impl__Create_CPaneContainerManager__UEAAHPEAVCWnd__PEAVCPaneDivider__PEAUCRuntimeClass___Z(
    void* pThis, void* pParentWnd, void* pDefaultSlider, CRuntimeClass* pContainerRTC) {
    if (pThis == nullptr) return FALSE;

    Field<void*>(pThis, kOffDockSite)     = pParentWnd;
    Field<void*>(pThis, kOffContainerRTC) = pContainerRTC;

    if (Field<void*>(pThis, kOffRootContainer) != nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }

    void* pRoot = nullptr;
    if (pContainerRTC != nullptr) {
        pRoot = impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pContainerRTC);
        if (pRoot != nullptr) {
            Field<void*>(pRoot, kOffContainerManagerOfContainer) = pThis;
        }
    } else {
        void* pMem = ::operator new(kSizeCPaneContainer);
        if (pMem != nullptr) {
            std::memset(pMem, 0, kSizeCPaneContainer);
            pRoot = impl___0CPaneContainer__QEAA_PEAVCPaneContainerManager__PEAVCDockablePane__1PEAVCPaneDivider___Z(
                pMem, pThis, nullptr, nullptr, nullptr);
        }
    }
    Field<void*>(pThis, kOffRootContainer) = pRoot;
    Field<void*>(pThis, kOffDefaultSlider) = pDefaultSlider;
    return TRUE;
}

// Symbol: ?CreateObject@CPaneContainerManager@@SAPEAVCObject@@XZ
// Retail entry RVA 0xa8070 is `operator new(0xa0)` followed by the constructor.
// Left a stub returning NULL: OpenMFC publishes no CPaneContainerManager vtable
// (see the constructor note above), so a DYNCREATE-produced object would carry a
// NULL vfptr and fault on the first virtual call its creator makes. Returning
// NULL is the failure the callers already test for.
extern "C" void* MS_ABI impl__CreateObject_CPaneContainerManager__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?CreatePaneDivider@CPaneContainerManager@@IEAAPEAVCPaneDivider@@VCRect@@KH@Z
// STUB -- but fully decoded first; the reasons for not landing the body are at
// the end of this comment. Retail
// ?CreatePaneDivider@CPaneContainerManager@@IEAAPEAVCPaneDivider@@VCRect@@KH@Z,
// entry RVA 0xa92e0 (mfc140.dll), 0xa92e0..0xa93fd:
//
//   CPaneDivider* pSlider = DYNAMIC_DOWNCAST(CPaneDivider,
//                               CPaneDivider::m_pSliderRTC->CreateObject());
//   pSlider->Init(0, NULL);                          // 0xa932c -> 0xab720
//   if (nID == -1) nID = m_nSliderCounter++;         // the DWORD at 0x1803aabf8
//   else if (nID >= m_nSliderCounter) m_nSliderCounter = nID + 1;
//   for (each object o in m_lstSliders)              // head at +0x48
//       if (o->GetDlgCtrlID() == nID) nID = m_nSliderCounter++;
//   if (!pSlider->CreateEx(0, dwStyle, rect, m_pDockSite, nID, NULL))  // vt +0x560
//   {
//       delete pSlider;                              // vtable slot 1, flag 1
//       return NULL;
//   }
//   pSlider->ShowWindow(SW_SHOW);                    // 0xa93d4, edx = 5
//   pSlider->m_pContainerManager = this;             // +0x208
//   m_lstSliders.AddTail(pSlider);                   // 0xa93e7, rcx = this + 0x40
//   return pSlider;
//
// The two loaded globals are named, not guessed: 0x1803aab10 holds the pointer
// the CreateObject call is made on and its .data word is statically initialised
// to 0x1802f4b58, a CRuntimeClass whose name string reads "CPaneDivider" and
// whose m_nObjectSize is 0x218 -- i.e. it is ?m_pSliderRTC@CPaneDivider@@, which
// this DLL also exports. 0x1803aabf8 is a plain DWORD initialised to 1.
// Vtable slot +0x560 of CPaneDivider's vftable (0x1802f4d28, mfc140.dll) is
// ?CreateEx@CPaneDivider@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@IPEAUCCreateContext@@@Z
// at 0xab810 -- CreateEx, not Create, and the six argument slots line up with
// CreateEx's parameter list exactly (rdx = dwStyleEx = 0).
//
// The id-fixup loop is transcribed literally: retail does NOT restart the walk
// after bumping nID, it keeps stepping from the node it was on.
//
// LEFT A STUB, deliberately, and NOT because the retail body is unclear -- the
// transcription above is complete and was read instruction by instruction. It is
// left a stub because this DLL cannot build the object the body needs. Three
// independent reasons, each verified in this tree:
//
//  1. THE OBJECT IS THE WRONG SIZE. ?m_pSliderRTC@CPaneDivider@@ would have to
//     point at RUNTIME_CLASS(CPaneDivider), whose CreateObject reaches
//     ?CreateObject@CPaneDivider@@ (featurepack/docking/DynCreateFactories.cpp),
//     i.e. `new CPaneDivider()` on the class declared in
//     include/openmfc/afxmfc.h -- CObject plus a 16-byte pad, sizeof 24. Retail's
//     CPaneDivider is 0x218. So the `pSlider->m_pContainerManager = this` store
//     at +0x208 that this body ends with is a ~500-byte heap overrun, and so is
//     every ?GetDlgCtrlID@CWnd@@ / ?GetStyle@CWnd@@ / GetSafeHwnd call the
//     m_lstSliders walks in this file would then make on the entry (they read
//     CWnd::m_hWnd). An earlier revision of this file did land that store, and
//     filed a request to initialise m_pSliderRTC, which would have armed it.
//     DO NOT initialise that static until CPaneDivider carries retail storage.
//  2. ?CreateEx@CPaneDivider@@ is an auto-generated stub returning FALSE
//     (featurepack/docking/CPaneDivider.cpp), so every call would take the
//     failure path: allocate a divider and immediately destroy it. The window
//     the whole function exists to create is never made.
//  3. That failure path would free with MFC's exported ??3@YAXPEAX@Z
//     (phase4/src/detail/MemcoreSupport.cpp, a plain std::free) a block that
//     DynCreateFactories allocated with the global ::operator new. Formally
//     mismatched, and there is no emitted vtable here to reach the scalar
//     deleting destructor retail actually calls.
//
// Returning NULL is what retail's own failure path returns, and it is what the
// two callers in this file (?AddPaneToRecentPaneContainer@ and, indirectly,
// ?AddPaneAndPaneContainer@ / ?AddPaneAndPaneDivider@) already test for.
//
// The generated stub's parameter list omitted the implicit `this` and the CRect;
// the list below is the retail-correct one, with the by-value CRect taken as the
// pointer retail receives in %rdx and forwards to CreateEx's `const RECT&`. It
// is kept correct so that whoever gives CPaneDivider real storage can drop the
// transcribed body straight in.
extern "C" void* MS_ABI impl__CreatePaneDivider_CPaneContainerManager__IEAAPEAVCPaneDivider__VCRect__KH_Z(
    void* pThis, const RECT* pRect, unsigned long dwStyle, int nID) {
    (void)pThis; (void)pRect; (void)dwStyle; (void)nID;
    return nullptr;   // STUB -- see the three reasons above
}

// Symbol: ?DoesAllowDynInsertBefore@CPaneContainerManager@@UEBAHXZ
// Decoded from retail ?DoesAllowDynInsertBefore@CPaneContainerManager@@UEBAHXZ,
// entry RVA 0xaaae0 (mfc140.dll):
//
//   for (each object o in m_lstControlBars) {
//       CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, o);
//       if (p->DoesAllowDynInsertBefore()) return TRUE;   // CBasePane vtable +0x310
//   }
//   return FALSE;
//
// DEVIATION: dispatched as CanFloat(). CBasePane vtable slot +0x310 holds the
// forwarder at 0x87b0, whose entire body is `jmp *this->vftable[0x3a8]`, and
// +0x3a8 is ?CanFloat@CBasePane@@UEBAHXZ -- exactly afxbasepane.h's inline
// `virtual BOOL DoesAllowDynInsertBefore() const { return CanFloat(); }`. Neither
// CBasePane nor CDockablePane overrides +0x310, but a derived class that did
// would be missed here, because OpenMFC's CBasePane has no
// DoesAllowDynInsertBefore virtual to dispatch on.
extern "C" int MS_ABI impl__DoesAllowDynInsertBefore_CPaneContainerManager__UEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return FALSE;
    CRuntimeClass* pBasePaneClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return FALSE;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pBasePaneClass);
        if (p == nullptr) continue;
        if (impl__CanFloat_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(p))) return TRUE;
    }
    return FALSE;
}

// Symbol: ?DoesContainFloatingPane@CPaneContainerManager@@UEAAHXZ
// Decoded from retail ?DoesContainFloatingPane@CPaneContainerManager@@UEAAHXZ,
// entry RVA 0xaa850 (mfc140.dll):
//
//   for (each object o in m_lstControlBars) {
//       CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, o);
//       if (p->CanFloat()) return TRUE;                   // CBasePane vtable +0x3a8
//   }
//   return FALSE;
//
// Slot +0x3a8 is ?CanFloat@CBasePane@@UEBAHXZ (0xc840 in mfc140.dll), which is
// exported, so this one dispatches through the real thunk.
extern "C" int MS_ABI impl__DoesContainFloatingPane_CPaneContainerManager__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    CRuntimeClass* pBasePaneClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return FALSE;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pBasePaneClass);
        if (p == nullptr) continue;
        if (impl__CanFloat_CBasePane__UEBAHXZ(static_cast<const CBasePane*>(p))) return TRUE;
    }
    return FALSE;
}

// Symbol: ?EnableGrippers@CPaneContainerManager@@UEAAXH@Z
// Decoded from retail ?EnableGrippers@CPaneContainerManager@@UEAAXH@Z, entry RVA
// 0xaa760 (mfc140.dll):
//
//   for (each object o in m_lstControlBars)
//       if (o != NULL && o->IsKindOf(RUNTIME_CLASS(CDockablePane)))
//           ((CDockablePane*)o)->EnableGripper(bEnable);   // vtable +0x3f0
//
// (+0x3f0 in CDockablePane's vftable 0x1802e3068 is
// ?EnableGripper@CDockablePane@@UEAAXH@Z at 0x40160, which is exported.)
extern "C" void MS_ABI impl__EnableGrippers_CPaneContainerManager__UEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    CRuntimeClass* pDockableClass = impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pDockableClass);
        if (p != nullptr) impl__EnableGripper_CDockablePane__UEAAXH_Z(p, bEnable);
    }
}

// Symbol: ?FindTabbedPane@CPaneContainerManager@@QEAAPEAVCDockablePane@@I@Z
// Decoded from retail ?FindTabbedPane@CPaneContainerManager@@QEAAPEAVCDockablePane@@I@Z,
// entry RVA 0xaa350 (mfc140.dll): `m_pRootContainer == NULL ? NULL :
// m_pRootContainer->FindTabbedPane(nID)` -- the body is a load of +0x78, a NULL
// test and a tail jump to ?FindTabbedPane@CPaneContainer@@ (0xa7b50).
extern "C" void* MS_ABI impl__FindTabbedPane_CPaneContainerManager__QEAAPEAVCDockablePane__I_Z(
    void* pThis, unsigned int nID) {
    if (pThis == nullptr) return nullptr;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    return pRoot == nullptr ? nullptr
                            : impl__FindTabbedPane_CPaneContainer__QEAAPEAVCDockablePane__I_Z(pRoot, nID);
}

// Symbol: ?GetAvailableSpace@CPaneContainerManager@@UEBAXAEAVCRect@@@Z
// Decoded from retail ?GetAvailableSpace@CPaneContainerManager@@UEBAXAEAVCRect@@@Z,
// entry RVA 0xa94e0 (mfc140.dll):
//
//   CRect rectOccupied; ::SetRectEmpty(&rectOccupied);
//   CRect rectTmp;      ::SetRectEmpty(&rectTmp);
//   for (each object o in m_lstControlBars) {
//       ::GetWindowRect(o->m_hWnd, &rectTmp);
//       ::UnionRect(&rectOccupied, &rectOccupied, &rectTmp);
//   }
//   for (each object o in m_lstSliders) { ... same ... }
//   GetWindowRect(rect);                      // this->vftable +0x68, i.e. the
//                                             // GetWindowRect below
//   ::SubtractRect(&rect, &rect, &rectOccupied);
//
// (SetRectEmpty / GetWindowRect / UnionRect / SubtractRect are the USER32
// imports at 0x1802c5368, 0x1802c5370, 0x1802c4d08 and 0x1802c4e50, all
// resolved with iat.py. The HWND is read straight from +0x40 of the list entry,
// i.e. CWnd::m_hWnd, without any downcast.)
//
// DEVIATION: entries with a NULL HWND are skipped rather than passed to
// ::GetWindowRect, and the recursive GetWindowRect call is made directly instead
// of through the vtable (retail's slot +0x68 is this same body for an
// unsubclassed manager).
extern "C" void MS_ABI impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(
    const void* pThis, CRect& rect);
// Symbol: ?GetWindowRect@CPaneContainerManager@@UEBAXAEAVCRect@@@Z
// Decoded from retail ?GetWindowRect@CPaneContainerManager@@UEBAXAEAVCRect@@@Z,
// entry RVA 0xa94c0 (mfc140.dll): the whole body is
// `m_pRootContainer->GetWindowRect(rect, FALSE)` through CPaneContainer vtable
// slot +0x28 (?GetWindowRect@CPaneContainer@@UEBAXAEAVCRect@@H@Z at 0xa3a70).
//
// DEVIATION: retail does not NULL-check m_pRootContainer and would fault; this
// leaves `rect` untouched instead.
extern "C" void MS_ABI impl__GetWindowRect_CPaneContainerManager__UEBAXAEAVCRect___Z(
    const void* pThis, CRect& rect) {
    if (pThis == nullptr) return;
    const void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot == nullptr) return;
    impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(pRoot, rect, FALSE);
}

extern "C" void MS_ABI impl__GetAvailableSpace_CPaneContainerManager__UEBAXAEAVCRect___Z(
    const void* pThis, CRect& rect) {
    if (pThis == nullptr) return;
    RECT rectOccupied;
    ::SetRectEmpty(&rectOccupied);
    RECT rectTmp;
    ::SetRectEmpty(&rectTmp);
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        std::vector<void*>* panes = PaneListLocked(pThis);
        if (panes != nullptr) {
            for (void* obj : *panes) {
                HWND hWnd = obj ? static_cast<const CWnd*>(obj)->GetSafeHwnd() : nullptr;
                if (hWnd == nullptr) continue;
                ::GetWindowRect(hWnd, &rectTmp);
                ::UnionRect(&rectOccupied, &rectOccupied, &rectTmp);
            }
        }
        for (void* obj : SliderList(pThis)) {
            HWND hWnd = obj ? static_cast<const CWnd*>(obj)->GetSafeHwnd() : nullptr;
            if (hWnd == nullptr) continue;
            ::GetWindowRect(hWnd, &rectTmp);
            ::UnionRect(&rectOccupied, &rectOccupied, &rectTmp);
        }
    }
    impl__GetWindowRect_CPaneContainerManager__UEBAXAEAVCRect___Z(pThis, rect);
    ::SubtractRect(rect, rect, &rectOccupied);
}

// Symbol: ?GetMinMaxOffset@CPaneContainerManager@@UEAAXPEAVCPaneDivider@@AEAH11@Z
// Retail entry RVA 0xa9030 (mfc140.dll), 193 instructions. It first sets
// nMinOffset = nMaxOffset = 0 and nStep = -1 and reads ::GetWindowRect of
// pSlider's own window, then splits on the BOOL at CPaneDivider +0x1c4 -- the
// default-divider flag, which ?Init@CPaneDivider@@QEAAXHPEAVCWnd@@@Z (entry RVA
// 0xab720) stores its `bDefaultSlider` argument into:
//   * default divider: it takes the pane's dock-site window from the CBasePane
//     virtual at vtable +0x348, hands it to
//     ?GetDockingManager@CGlobalUtils@@ (0x6ccc0), inflates the docking
//     manager's rect at +0x1c8 and clamps against it, switching on the
//     CBRS_ALIGN_* bits 12..15 returned by the CBasePane virtual at +0x338.
//   * internal divider: it finds the owning CPaneContainer -- m_pRootContainer
//     itself when its m_pSlider matches, otherwise
//     CPaneContainer::FindSubPaneContainer(pSlider, BC_FIND_BY_SLIDER)
//     (0xa5230) on its two children -- and derives the limits from that node's
//     GetWindowRect (vtable +0x28), GetMinSizeLeft (+0x38) and GetMinSizeRight
//     (+0x40), choosing the vertical or horizontal formula on the CPaneDivider
//     virtual at +0x2d8.
// Both paths finish with nStep = <container>->GetResizeStep() (vtable +0x48).
// Left a stub, for a reason that is upstream of all of that: BOTH paths begin by
// reading the divider's own flag word at CPaneDivider +0x1c4 to choose between
// them, and every subsequent step reads more of the divider (its m_hWnd at +0x40,
// its orientation virtual at +0x2d8). OpenMFC's CPaneDivider
// (include/openmfc/afxmfc.h) is CObject plus a 16-byte pad -- sizeof 24 -- so
// none of those offsets is inside the object; see ?CreatePaneDivider@ above for
// the same problem stated in full. On top of that the default-divider path needs
// CGlobalUtils/CDockingManager infrastructure this repo does not model.
// (Two earlier revisions of this comment were wrong about the rest: one named
// CalcAvailablePaneSpace, which is not called here; the other said the four
// CPaneContainer accessors this would use -- GetWindowRect, GetMinSizeLeft,
// GetMinSizeRight, GetResizeStep -- are stubs, and all four now have real bodies
// in docking/CPaneContainer.cpp.)
extern "C" void MS_ABI impl__GetMinMaxOffset_CPaneContainerManager__UEAAXPEAVCPaneDivider__AEAH11_Z(void* /*class*/* p0, int* p1, int* p2, int* p3) {}

// Symbol: ?GetMinSize@CPaneContainerManager@@UEAAXAEAVCSize@@@Z
// Decoded from retail ?GetMinSize@CPaneContainerManager@@UEAAXAEAVCSize@@@Z,
// entry RVA 0xaa660 (mfc140.dll):
//
//   size = CSize(0, 0);                                 // movq $0,(%rdx)
//   if (m_pRootContainer != NULL) m_pRootContainer->GetMinSize(size);
//
// (CPaneContainer vtable +0x30 is ?GetMinSize@CPaneContainer@@UEBAXAEAVCSize@@@Z
// at 0xa3ca0.)
extern "C" void MS_ABI impl__GetMinSize_CPaneContainerManager__UEAAXAEAVCSize___Z(
    void* pThis, CSize& size) {
    size.cx = 0;
    size.cy = 0;
    if (pThis == nullptr) return;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot != nullptr) impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(pRoot, size);
}

// Symbol: ?GetNodeCount@CPaneContainerManager@@QEBAHXZ
// Decoded from retail ?GetNodeCount@CPaneContainerManager@@QEBAHXZ, entry RVA
// 0xaa8c0 (mfc140.dll): `m_pRootContainer == NULL ? 0 :
// m_pRootContainer->GetNodeCount()` -- a load of +0x78, a NULL test and a tail
// jump to ?GetNodeCount@CPaneContainer@@ (0xa7c60).
extern "C" int MS_ABI impl__GetNodeCount_CPaneContainerManager__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return 0;
    const void* pRoot = Field<void*>(pThis, kOffRootContainer);
    return pRoot == nullptr ? 0 : impl__GetNodeCount_CPaneContainer__QEBAHXZ(pRoot);
}

// Symbol: ?GetTotalRefCount@CPaneContainerManager@@QEBAHXZ
// Decoded from retail ?GetTotalRefCount@CPaneContainerManager@@QEBAHXZ, entry
// RVA 0xaa8f0 (mfc140.dll): `m_pRootContainer == NULL ? 0 :
// m_pRootContainer->GetTotalReferenceCount()` -- tail jump to 0xa7e70.
extern "C" int MS_ABI impl__GetTotalRefCount_CPaneContainerManager__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return 0;
    const void* pRoot = Field<void*>(pThis, kOffRootContainer);
    return pRoot == nullptr ? 0 : impl__GetTotalReferenceCount_CPaneContainer__QEBAHXZ(pRoot);
}

// Symbol: ?GetVisiblePaneCount@CPaneContainerManager@@UEBAHXZ
// Decoded from retail ?GetVisiblePaneCount@CPaneContainerManager@@UEBAHXZ, entry
// RVA 0xaa6a0 (mfc140.dll):
//
//   int n = 0;
//   for (each object o in m_lstControlBars) {
//       CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, o);
//       if (p->GetStyle() & WS_VISIBLE) n++;
//   }
//   return n;
//
// (the RTTI pointer is RUNTIME_CLASS(CBasePane) at 0x1802da490; the call is
// ?GetStyle@CWnd@@QEBAKXZ at 0x2a75a0 and the test is `bt $0x1c,%eax`.)
//
// DEVIATION: retail calls GetStyle through the NULL the downcast produced for a
// non-CBasePane entry; those entries are skipped here.
extern "C" int MS_ABI impl__GetVisiblePaneCount_CPaneContainerManager__UEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return 0;
    CRuntimeClass* pBasePaneClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    int nCount = 0;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return 0;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pBasePaneClass);
        if (p == nullptr) continue;
        if (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(p)) & WS_VISIBLE) nCount++;
    }
    return nCount;
}

// Symbol: ?HideAll@CPaneContainerManager@@UEAAXXZ
// Decoded from retail ?HideAll@CPaneContainerManager@@UEAAXXZ, entry RVA
// 0xaa7c0 (mfc140.dll):
//
//   for (each object o in m_lstControlBars)
//       if (o != NULL && o->IsKindOf(RUNTIME_CLASS(CWnd))) ((CWnd*)o)->ShowWindow(SW_HIDE);
//   for (each object o in m_lstSliders)
//       ... same ...
//
// (RUNTIME_CLASS(CWnd) is at 0x1803352d0; the call is ?ShowWindow@CWnd@@QEAAHH@Z
// at 0x2a79e0 with edx = 0 = SW_HIDE.)
extern "C" void MS_ABI impl__HideAll_CPaneContainerManager__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CRuntimeClass* pWndClass = impl__GetThisClass_CWnd__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes != nullptr) {
        for (void* obj : *panes) {
            void* p = DynDowncast(obj, pWndClass);
            if (p != nullptr) impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(p), SW_HIDE);
        }
    }
    for (void* obj : SliderList(pThis)) {
        void* p = DynDowncast(obj, pWndClass);
        if (p != nullptr) impl__ShowWindow_CWnd__QEAAHH_Z(static_cast<CWnd*>(p), SW_HIDE);
    }
}

// Symbol: ?InsertPane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@0KPEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
// Decoded from retail ?InsertPane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@0KPEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z,
// entry RVA 0xa8970 (mfc140.dll). An earlier revision of this comment called it
// 209 instructions that call CalcRects and AddPaneAndPaneContainer; that is the
// count disas.py prints before the next known export, and neither of those two
// is called from here. The real body is 29 instructions, 0xa8970 through the
// `ret` at 0xa89c1 plus the throw at 0xa89c2 (0xa89d0 starts
// ?AddPaneToRecentPaneContainer@):
//
//   if (m_pRootContainer == NULL) AfxThrowInvalidArgException();  // 0x225b80
//   BOOL bResult = FALSE;
//   if (pTargetControlBar != NULL) {
//       POSITION pos = <first node of m_lstControlBars whose data ==
//                       pTargetControlBar>;                        // 0xa898b
//       if (pos != NULL)
//           bResult = AddPaneAndPaneDivider(pTargetControlBar,     // 0xa9a00
//                                           pControlBarToInsert, pos,
//                                           dwAlignment);
//   }
//   return bResult;
//
// `lpRect` and `dockMethod` are never read: their stack slots are untouched and
// the two registers holding them at entry are dead.
//
// The argument order into AddPaneAndPaneDivider is retail's (0xa89a6..0xa89b4):
// rdx = pTargetControlBar becomes pBarOriginal and r8 = pControlBarToInsert
// becomes pBarToInsert, i.e. the two are swapped relative to this function's
// own parameter order.
//
// DEVIATION: OpenMFC keeps m_lstControlBars in the side table described at the
// top of this file and has no POSITION to hand on, so NULL is passed for
// posNearestBar where retail passes the found node. AddPaneAndPaneDivider is
// still a stub in this file and ignores it; whoever implements that body must
// not assume posNearestBar is meaningful until this list keeps real nodes.
extern "C" int MS_ABI impl__InsertPane_CPaneContainerManager__UEAAHPEAVCDockablePane__0KPEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    void* pThis, void* pControlBarToInsert, void* pTargetControlBar,
    unsigned long dwAlignment, const RECT* lpRect, int dockMethod) {
    (void)lpRect;      // dead in retail too -- see above
    (void)dockMethod;  // dead in retail too -- see above
    if (pThis == nullptr) return FALSE;
    if (Field<void*>(pThis, kOffRootContainer) == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    if (pTargetControlBar == nullptr) return FALSE;

    bool bFound = false;
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        std::vector<void*>* panes = PaneListLocked(pThis);
        bFound = panes != nullptr &&
                 std::find(panes->begin(), panes->end(), pTargetControlBar) != panes->end();
    }
    if (!bFound) return FALSE;
    return impl__AddPaneAndPaneDivider_CPaneContainerManager__IEAAHPEAVCDockablePane__0PEAU__POSITION__K_Z(
        pThis, pTargetControlBar, pControlBarToInsert, nullptr, dwAlignment);
}

// Symbol: ?IsEmpty@CPaneContainerManager@@QEBAHXZ
// Decoded from retail ?IsEmpty@CPaneContainerManager@@QEBAHXZ, entry RVA 0xaa8e0
// (mfc140.dll) / 0xaa320 (mfc140u.dll): the whole body is
// `xor %eax,%eax; cmp %rax,0x20(%rcx); sete %al`, i.e.
// `return m_lstControlBars.GetCount() == 0;` -- +0x20 is m_nCount of the CObList
// embedded at +0x08.
//
// DEVIATION: reads the side-table pane list rather than that raw word, for the
// reason given in the layout comment at the top of this file.
extern "C" int MS_ABI impl__IsEmpty_CPaneContainerManager__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return TRUE;
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    auto it = g_containerManagerPanes.find(const_cast<void*>(pThis));
    return (it == g_containerManagerPanes.end() || it->second.panes.empty()) ? TRUE : FALSE;
}

// Symbol: ?IsRootPaneContainerVisible@CPaneContainerManager@@UEBAHXZ
// Decoded from retail ?IsRootPaneContainerVisible@CPaneContainerManager@@UEBAHXZ,
// entry RVA 0xaa690 (mfc140.dll): `m_pRootContainer == NULL ? FALSE :
// m_pRootContainer->IsVisible()` -- tail jump to ?IsVisible@CPaneContainer@@
// (0xa7150).
extern "C" int MS_ABI impl__IsRootPaneContainerVisible_CPaneContainerManager__UEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return FALSE;
    const void* pRoot = Field<void*>(pThis, kOffRootContainer);
    return pRoot == nullptr ? FALSE : impl__IsVisible_CPaneContainer__QEBAHXZ(pRoot);
}

// Symbol: ?NotifyPaneDivider@CPaneContainerManager@@QEAAXXZ
// Decoded from retail ?NotifyPaneDivider@CPaneContainerManager@@QEAAXXZ, entry
// RVA 0xaab50 (mfc140.dll):
//
//   if (m_pDefaultSlider != NULL) m_pDefaultSlider->NotifyAboutRelease();
//
// (+0x550 in CPaneDivider's vftable 0x1802f4d28 is
// ?NotifyAboutRelease@CPaneDivider@@UEAAXXZ at 0xad460, which is exported.)
extern "C" void MS_ABI impl__NotifyPaneDivider_CPaneContainerManager__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pSlider = Field<void*>(pThis, kOffDefaultSlider);
    if (pSlider != nullptr) impl__NotifyAboutRelease_CPaneDivider__UEAAXXZ(pSlider);
}

// Symbol: ?OnPaneDividerMove@CPaneContainerManager@@UEAAHPEAVCPaneDivider@@IHAEAPEAX@Z
// Decoded from retail ?OnPaneDividerMove@CPaneContainerManager@@UEAAHPEAVCPaneDivider@@IHAEAPEAX@Z,
// entry RVA 0xa8e10 (mfc140.dll), 0xa8e10..0xa901d. (An earlier revision of this
// comment said 364 instructions and described a GetMinMaxOffset clamp; 364 is
// how many instructions disas.py prints before the next known export, and no
// GetMinMaxOffset call appears anywhere in the body.)
//
//   CRect rect(0,0,0,0);  CSize sizeMin(0,0);
//   m_pRootContainer->GetWindowRect(rect, FALSE);    // vtable +0x28, 0xa8e60
//   m_pRootContainer->GetMinSize(sizeMin);           // vtable +0x30, 0xa8e75
//   if (pSlider == m_pDefaultSlider) {               // +0x88, tested at 0xa8e7b
//       DWORD dwAlign = pSlider->GetCurrentAlignment();   // vtable +0x338
//       m_pDockSite->ScreenToClient(&rect);               // 0xa8ea8 -> 0x2a11f0
//       BOOL bRTL = m_pDockSite->GetExStyle() & WS_EX_LAYOUTRTL;  // 0xa8eb4/0xa8eb9
//       switch (dwAlign) {
//       case CBRS_ALIGN_LEFT:                             // 0xa8f41
//           if (bRTL) rect.left += nOffset; else rect.right += nOffset;
//           if (rect.Width()  < sizeMin.cx) rect.right  = rect.left + sizeMin.cx;
//           break;
//       case CBRS_ALIGN_TOP:                              // 0xa8f25
//           rect.bottom += nOffset;
//           if (rect.Height() < sizeMin.cy) rect.bottom = rect.top + sizeMin.cy;
//           break;
//       case CBRS_ALIGN_RIGHT:                            // 0xa8efe
//           if (bRTL) rect.right += nOffset; else rect.left += nOffset;
//           if (rect.Width()  < sizeMin.cx) rect.left   = rect.right - sizeMin.cx;
//           break;
//       case CBRS_ALIGN_BOTTOM:                           // 0xa8ee2
//           rect.top += nOffset;
//           if (rect.Height() < sizeMin.cy) rect.top    = rect.bottom - sizeMin.cy;
//           break;
//       }
//       this->ResizePaneContainers(rect, hdwp);      // vtable +0x80, 0xa8f83
//       return FALSE;                                // 0xa8f89
//   }
//   CRect rectSlider(0,0,0,0);
//   ::GetWindowRect(pSlider->m_hWnd, &rectSlider);   // 0xa8fba -- never read again
//   CPaneContainer* p = m_pRootContainer;
//   if (p->m_pSlider != pSlider) {                   // +0x18
//       CPaneContainer* pFound = NULL;
//       if (p->m_pLeftPaneContainer  != NULL)        // +0x20
//           pFound = p->m_pLeftPaneContainer->FindSubPaneContainer(pSlider, BC_FIND_BY_SLIDER);
//       if (pFound == NULL && p->m_pRightPaneContainer != NULL)   // +0x28
//           pFound = p->m_pRightPaneContainer->FindSubPaneContainer(pSlider, BC_FIND_BY_SLIDER);
//       p = pFound;
//   }
//   if (p == NULL) return FALSE;
//   return p->OnMoveInternalPaneDivider(nOffset, hdwp);   // vtable +0x80
//
// Two details that are easy to get backwards and were re-read to be sure:
//  * The `uFlags` (UINT) parameter is dead. It arrives in %r8d and %r8d is zeroed
//    at 0xa8e53 to be the second argument of the GetWindowRect call without ever
//    being spilled, so nothing in the body can see it.
//  * The two exits return different things. The default-slider path falls into
//    `xor %eax,%eax` at 0xa8f89 and returns FALSE; the internal-divider path
//    jumps from 0xa901d to 0xa8f8b, i.e. PAST that xor, so it returns whatever
//    OnMoveInternalPaneDivider returned. The `p == NULL` exit returns FALSE.
// The slot numbers are read out of the retail vftables, not inferred:
// CPaneContainer's vftable (0x1802f4070) has ?GetWindowRect@CPaneContainer@@ at
// +0x28, ?GetMinSize@CPaneContainer@@ at +0x30 and
// ?OnMoveInternalPaneDivider@CPaneContainer@@ at +0x80; CPaneDivider's
// (0x1802f4d28) has ?GetCurrentAlignment@CBasePane@@ at +0x338; this class's
// (0x1802f41a8) has ?ResizePaneContainers@CPaneContainerManager@@UEAAXVCRect@@AEAPEAX@Z
// at +0x80.
//
// DEVIATIONS:
//  * HAZARD, not a deviation -- flagged here because the call is retail's and is
//    reproduced faithfully: the default-divider path hands `pSlider` to
//    ?GetCurrentAlignment@CBasePane@@, which reads CBasePane::m_dwStyle. In this
//    DLL that member sits at offset 260 of a 424-byte CBasePane, while
//    CPaneDivider (include/openmfc/afxmfc.h) is CObject plus a 16-byte pad --
//    sizeof 24. So on a real OpenMFC CPaneDivider that read runs ~236 bytes past
//    the end of the object and the alignment it returns is garbage; the switch
//    below then usually falls through its `default`. Retail's CPaneDivider is
//    0x218 bytes and derives from CBasePane, so retail is reading its own field.
//    The fix belongs in the CPaneDivider declaration, not here.
//  * `this` and m_pRootContainer are NULL-checked; retail dereferences both.
//  * The dead ::GetWindowRect(pSlider->m_hWnd) call is not made. It writes a
//    stack CRect that no later instruction reads.
//  * ResizePaneContainers is called as this file's own body rather than through
//    vtable slot +0x80; for an unsubclassed manager that slot is exactly this
//    body, but a derived override would be missed.
//  * All three CPaneContainer entry points this dispatches to -- GetWindowRect,
//    GetMinSize and OnMoveInternalPaneDivider -- have real bodies in
//    docking/CPaneContainer.cpp, so both paths do real work. (An earlier
//    revision of this comment called all three generated stubs and said the
//    geometry this computes is necessarily all zeroes and the internal-divider
//    path necessarily returns FALSE; that has not been true since those bodies
//    landed.) One arm one level down is still inert: OnMoveInternalPaneDivider
//    applies its pane rectangles through ?MoveWindow@CBasePane@@, which is
//    implemented, but its sub-container rectangles through
//    ?Resize@CPaneContainer@@, which is still a stub.
// The generated stub's parameter list omitted the implicit `this`.
extern "C" int MS_ABI impl__OnPaneDividerMove_CPaneContainerManager__UEAAHPEAVCPaneDivider__IHAEAPEAX_Z(
    void* pThis, void* pSlider, unsigned int uFlags, int nOffset, void** phdwp) {
    (void)uFlags;   // dead in retail too -- see above
    if (pThis == nullptr) return FALSE;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot == nullptr) return FALSE;

    CRect rect(0, 0, 0, 0);
    impl__GetWindowRect_CPaneContainer__UEBAXAEAVCRect__H_Z(pRoot, rect, FALSE);
    CSize sizeMin(0, 0);
    impl__GetMinSize_CPaneContainer__UEBAXAEAVCSize___Z(pRoot, sizeMin);

    if (pSlider == Field<void*>(pThis, kOffDefaultSlider)) {
        const unsigned long dwAlign =
            impl__GetCurrentAlignment_CBasePane__UEBAKXZ(static_cast<const CBasePane*>(pSlider));
        CWnd* pDockSite = static_cast<CWnd*>(Field<void*>(pThis, kOffDockSite));
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pDockSite, static_cast<RECT*>(rect));
        const bool bRTL = (impl__GetExStyle_CWnd__QEBAKXZ(pDockSite) & kExLayoutRtl) != 0;
        switch (dwAlign) {
        case kCbrsAlignLeft:
            if (bRTL) rect.left += nOffset; else rect.right += nOffset;
            if (rect.right - rect.left < sizeMin.cx) rect.right = rect.left + sizeMin.cx;
            break;
        case kCbrsAlignTop:
            rect.bottom += nOffset;
            if (rect.bottom - rect.top < sizeMin.cy) rect.bottom = rect.top + sizeMin.cy;
            break;
        case kCbrsAlignRight:
            if (bRTL) rect.right += nOffset; else rect.left += nOffset;
            if (rect.right - rect.left < sizeMin.cx) rect.left = rect.right - sizeMin.cx;
            break;
        case kCbrsAlignBottom:
            rect.top += nOffset;
            if (rect.bottom - rect.top < sizeMin.cy) rect.top = rect.bottom - sizeMin.cy;
            break;
        default:
            break;
        }
        RECT rectCopy = {rect.left, rect.top, rect.right, rect.bottom};
        impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(
            pThis, &rectCopy, phdwp);
        return FALSE;
    }

    void* pContainer = pRoot;
    if (Field<void*>(pRoot, kOffContainerSlider) != pSlider) {
        void* pFound = nullptr;
        void* pLeft  = Field<void*>(pRoot, 0x20);   // CPaneContainer::m_pLeftPaneContainer
        void* pRight = Field<void*>(pRoot, 0x28);   // CPaneContainer::m_pRightPaneContainer
        if (pLeft != nullptr) {
            pFound = impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                pLeft, pSlider, kFindBySlider);
        }
        if (pFound == nullptr && pRight != nullptr) {
            pFound = impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                pRight, pSlider, kFindBySlider);
        }
        pContainer = pFound;
    }
    if (pContainer == nullptr) return FALSE;
    return impl__OnMoveInternalPaneDivider_CPaneContainer__UEAAHHAEAPEAX_Z(
        pContainer, nOffset, phdwp);
}

// Symbol: ?OnShowPane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@H@Z
// Decoded from retail ?OnShowPane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@H@Z,
// entry RVA 0xa8de0 (mfc140.dll). An earlier revision of this comment described
// this as 377 instructions driven by CPaneContainer::OnShowPane; that is wrong
// on both counts. The export is thirteen instructions long, 0xa8de0 through the
// tail jump at 0xa8e09 (0xa8e10 is where ?OnPaneDividerMove@ begins); 377 is
// how many instructions disas.py prints before the next known export:
//
//   if (m_pRootContainer != NULL)                       // +0x78
//       m_pRootContainer->CheckPaneDividerVisibility();  // direct call 0xa71c0
//   return this->IsRootPaneContainerVisible();           // vtable +0xa8, tail jmp
//
// Neither `pBar` (rdx) nor `bShow` (r8d) is read anywhere in the body -- both
// registers are dead from entry. CPaneContainer::CheckPaneDividerVisibility is
// really implemented in docking/CPaneContainer.cpp, so this does real work.
//
// DEVIATION: IsRootPaneContainerVisible is called as this file's own body rather
// than through vtable slot +0xa8; for an unsubclassed CPaneContainerManager that
// slot is exactly this body, but an override in a derived class would be missed.
extern "C" int MS_ABI impl__OnShowPane_CPaneContainerManager__UEAAHPEAVCDockablePane__H_Z(
    void* pThis, void* pBar, int bShow) {
    (void)pBar;   // dead in retail too -- see above
    (void)bShow;  // dead in retail too -- see above
    if (pThis == nullptr) return FALSE;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot != nullptr) {
        impl__CheckPaneDividerVisibility_CPaneContainer__QEAAXXZ(pRoot);
    }
    return impl__IsRootPaneContainerVisible_CPaneContainerManager__UEBAHXZ(pThis);
}

// Symbol: ?PaneFromPoint@CPaneContainerManager@@UEAAPEAVCDockablePane@@VCPoint@@HHAEAH1@Z
// Decoded from retail ?PaneFromPoint@CPaneContainerManager@@UEAAPEAVCDockablePane@@VCPoint@@HHAEAH1@Z,
// entry RVA 0xaa3d0 (mfc140.dll), 0xaa3d0..0xaa650. The shipping
// afxpanecontainermanager.h declares it
//   virtual CDockablePane* PaneFromPoint(CPoint point, int nSensitivity,
//                                        BOOL bExactBar, BOOL& bIsTabArea,
//                                        BOOL& bCaption);
// and the argument slots match: rdx = point, r8d = nSensitivity,
// r9d = bExactBar, [rsp+0x28] = &bIsTabArea, [rsp+0x30] = &bCaption.
//
//   bIsTabArea = FALSE;                                   // 0xaa40a; bCaption
//                                                         // is NOT initialised
//   for (each object o in m_lstControlBars) {             // head at +0x10
//       CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, o);
//       if (p == NULL) continue;
//       if (!(p->GetStyle() & WS_VISIBLE)) continue;      // bt $0x1c
//       CRect rect;   ::GetWindowRect(p->m_hWnd, &rect);
//       CRect rectTabAreaTop, rectTabAreaBottom;
//       p->GetTabArea(rectTabAreaTop, rectTabAreaBottom); // vtable +0x668
//       if (::PtInRect(&rectTabAreaTop, point) ||
//           ::PtInRect(&rectTabAreaBottom, point)) { bIsTabArea = TRUE; return p; }
//       if (p->HitTest(point, TRUE) == HTCAPTION)         // vtable +0x678
//           { bCaption = TRUE; return p; }
//       rect.top    += p->GetCaptionHeight();             // vtable +0x358
//       rect.bottom -= rectTabAreaBottom.Height();        // 0xaa4fc..0xaa506
//       if (!::PtInRect(&rect, point)) continue;
//       <smart-docking-manager branch -- see DEVIATIONS>   // 0xaa513..0xaa54f
//       ::InflateRect(&rect, -nSensitivity, -nSensitivity);
//       if (!::PtInRect(&rect, point)) return p;          // in the border band
//       if (nSensitivity == 0) return p;
//   }
//   if (bExactBar) return NULL;                           // 0xaa58f
//   for (each object o in m_lstControlBars) {             // second pass
//       CDockablePane* p = DYNAMIC_DOWNCAST(CDockablePane, o);
//       CRect rect;   ::GetWindowRect(p->m_hWnd, &rect);
//       ::InflateRect(&rect, nSensitivity, nSensitivity);
//       if (::PtInRect(&rect, point)) return p;
//   }
//   return NULL;
//
// The sense of the deflated test is retail's and was read twice: 0xaa575 is
// `je` on the ::PtInRect result, so it is the point landing OUTSIDE the rect
// shrunk by nSensitivity -- i.e. within nSensitivity of the pane's border --
// that selects the pane; a point deep inside keeps searching.
//
// Slot identifications, read out of the retail CDockablePane vftable at
// 0x1802e3068 (mfc140.dll): +0x358 is ?GetCaptionHeight@CDockablePane@@UEBAHXZ
// (0x46520), +0x678 is ?HitTest@CDockablePane@@UEAAHVCPoint@@H@Z (0x40e20), and
// +0x668 is the unexported body at 0x3fc20, whose whole text is
// `::SetRectEmpty(rectTabAreaTop); ::SetRectEmpty(rectTabAreaBottom);` -- that is
// afxdockablepane.h's base GetTabArea, which CTabbedPane overrides. HTCAPTION is
// 2, which is what the `cmp $0x2,%eax` at 0xaa4da tests.
//
// DEVIATIONS:
//  * GetTabArea is NOT dispatched; both tab rects stay empty, which is exactly
//    the retail base-class behaviour for a plain CDockablePane but wrong for a
//    CTabbedPane. OpenMFC's only GetTabArea body for this hierarchy,
//    ?GetTabArea@CTabbedPane@@ (docking/CTabbedPane.cpp:54), fills its second
//    out-parameter with the *pane* area rather than the bottom tab strip, so
//    wiring it in here would report a hit anywhere in the pane as a tab hit and
//    would subtract the pane height from rect.bottom. Leaving both rects empty
//    loses the tab-area report; using that body would produce wrong answers.
//    See the headerRequest filed with this change.
//  * The smart-docking branch at 0xaa513 is not reproduced. Retail takes
//    ::GetParent(p->m_hWnd), wraps it with CWnd::FromHandle (0x289180), asks
//    afxGlobalUtils (0x1803aacf8) for the CDockingManager that owns it
//    (?GetDockingManager@CGlobalUtils@@, 0x6ccc0) and, when that manager's
//    +0x308 object has non-zero +0x8 and +0xc, sets bCaption from its +0x1b8
//    field (`bCaption = (v == 8)`, taken only when (unsigned)(v-4) <= 4) and
//    returns the pane. Those three CDockingManager offsets are not modelled in
//    this repo and inventing them is not an option, so this body always takes
//    retail's fall-through at 0xaa555, which is the path retail itself takes
//    when no docking manager is found.
//  * Entries whose CDockablePane downcast fails are skipped in the SECOND pass
//    too; retail passes the NULL through to ::GetWindowRect there.
//  * `this` is NULL-checked.
// The generated stub's parameter list omitted the implicit `this` and both
// out-parameters. The by-value CPoint is one 8-byte register argument under the
// MS x64 ABI (retail hands it straight to ::PtInRect as a POINT), so it is taken
// here as an opaque 64-bit value, exactly as CDockablePane::HitTest does.
extern "C" void* MS_ABI impl__PaneFromPoint_CPaneContainerManager__UEAAPEAVCDockablePane__VCPoint__HHAEAH1_Z(
    void* pThis, void* point, int nSensitivity, int bExactBar,
    int* pbIsTabArea, int* pbCaption) {
    POINT pt;
    std::memcpy(&pt, &point, sizeof(pt));
    if (pbIsTabArea != nullptr) *pbIsTabArea = FALSE;
    if (pThis == nullptr) return nullptr;

    CRuntimeClass* pDockableClass = impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return nullptr;

    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pDockableClass);
        if (p == nullptr) continue;
        if (!(impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(p)) & WS_VISIBLE)) continue;

        RECT rect = {0, 0, 0, 0};
        ::GetWindowRect(static_cast<const CWnd*>(p)->GetSafeHwnd(), &rect);

        // GetTabArea (retail: CDockablePane vtable +0x668) -- base behaviour only,
        // see the DEVIATIONS note above.
        const RECT rectTabAreaTop    = {0, 0, 0, 0};
        const RECT rectTabAreaBottom = {0, 0, 0, 0};
        if (::PtInRect(&rectTabAreaTop, pt) || ::PtInRect(&rectTabAreaBottom, pt)) {
            if (pbIsTabArea != nullptr) *pbIsTabArea = TRUE;
            return p;
        }

        if (impl__HitTest_CDockablePane__UEAAHVCPoint__H_Z(p, point, TRUE) == HTCAPTION) {
            if (pbCaption != nullptr) *pbCaption = TRUE;
            return p;
        }

        rect.top    += impl__GetCaptionHeight_CDockablePane__UEBAHXZ(p);
        rect.bottom += rectTabAreaBottom.top - rectTabAreaBottom.bottom;
        if (!::PtInRect(&rect, pt)) continue;

        // retail's smart-docking branch sits here -- see the DEVIATIONS note.
        ::InflateRect(&rect, -nSensitivity, -nSensitivity);
        if (!::PtInRect(&rect, pt)) return p;
        if (nSensitivity == 0) return p;
    }

    if (bExactBar) return nullptr;

    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pDockableClass);
        if (p == nullptr) continue;
        RECT rect = {0, 0, 0, 0};
        ::GetWindowRect(static_cast<const CWnd*>(p)->GetSafeHwnd(), &rect);
        ::InflateRect(&rect, nSensitivity, nSensitivity);
        if (::PtInRect(&rect, pt)) return p;
    }
    return nullptr;
}

// Symbol: ?ReleaseEmptyPaneContainers@CPaneContainerManager@@QEAAXXZ
// Decoded from retail ?ReleaseEmptyPaneContainers@CPaneContainerManager@@QEAAXXZ,
// entry RVA 0xaae10 (mfc140.dll):
//
//   if (m_pRootContainer != NULL) m_pRootContainer->ReleaseEmptyPaneContainer();
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CPaneContainerManager__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot != nullptr) impl__ReleaseEmptyPaneContainer_CPaneContainer__QEAAXXZ(pRoot);
}

// Symbol: ?RemoveAllPanesAndPaneDividers@CPaneContainerManager@@QEAAXXZ
// Decoded from retail ?RemoveAllPanesAndPaneDividers@CPaneContainerManager@@QEAAXXZ,
// entry RVA 0xaaa20 (mfc140.dll):
//
//   for (each node in m_lstControlBars) {
//       CBasePane* p = DYNAMIC_DOWNCAST(CBasePane, node->data);
//       if (p->GetStyle() & WS_VISIBLE) m_lstControlBars.RemoveAt(node);
//   }
//   for (each node in m_lstSliders) { ... same, m_lstSliders.RemoveAt(node) ... }
//
// The WS_VISIBLE sense is deliberate and was read twice: `bt $0x1c,%eax` followed
// by `jae <continue>` skips the RemoveAt when bit 28 is clear, so it is the
// VISIBLE entries that get unlinked and hidden ones that stay. Neither list's
// elements are destroyed -- only unlinked.
//
// DEVIATION: entries whose CBasePane downcast fails are skipped instead of being
// passed to GetStyle as NULL.
extern "C" void MS_ABI impl__RemoveAllPanesAndPaneDividers_CPaneContainerManager__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CRuntimeClass* pBasePaneClass = impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes != nullptr) {
        for (size_t i = 0; i < panes->size();) {
            void* p = DynDowncast((*panes)[i], pBasePaneClass);
            if (p != nullptr &&
                (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(p)) & WS_VISIBLE)) {
                panes->erase(panes->begin() + static_cast<long>(i));
            } else {
                ++i;
            }
        }
    }
    std::vector<void*>& sliders = SliderList(pThis);
    for (size_t i = 0; i < sliders.size();) {
        void* p = DynDowncast(sliders[i], pBasePaneClass);
        if (p != nullptr &&
            (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(p)) & WS_VISIBLE)) {
            sliders.erase(sliders.begin() + static_cast<long>(i));
        } else {
            ++i;
        }
    }
}

// Symbol: ?RemoveNonValidPanes@CPaneContainerManager@@QEAAXXZ
// Decoded from retail ?RemoveNonValidPanes@CPaneContainerManager@@QEAAXXZ, entry
// RVA 0xaace0 (mfc140.dll):
//
//   if (m_pRootContainer != NULL) m_pRootContainer->RemoveNonValidPanes();
extern "C" void MS_ABI impl__RemoveNonValidPanes_CPaneContainerManager__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot != nullptr) impl__RemoveNonValidPanes_CPaneContainer__QEAAXXZ(pRoot);
}

// Symbol: ?RemovePaneDivider@CPaneContainerManager@@UEAAXPEAVCPaneDivider@@@Z
// Decoded from retail ?RemovePaneDivider@CPaneContainerManager@@UEAAXPEAVCPaneDivider@@@Z,
// entry RVA 0xa9400 (mfc140.dll):
//
//   POSITION pos = <first node of m_lstSliders whose data == pSlider>;
//   if (pos != NULL) { m_lstSliders.RemoveAt(pos); pSlider->m_pContainerManager = NULL; }
//   if (m_pRootContainer != NULL) {
//       ASSERT(pSlider != NULL);                       // je -> AfxThrowInvalidArg (0x225b80)
//       CPaneContainer* pFound;
//       if (m_pRootContainer->m_pSlider == pSlider) pFound = m_pRootContainer;
//       else {
//           pFound = NULL;
//           if (m_pRootContainer->m_pLeftPaneContainer != NULL)
//               pFound = m_pRootContainer->m_pLeftPaneContainer
//                            ->FindSubPaneContainer(pSlider, BC_FIND_BY_SLIDER);
//           if (pFound == NULL && m_pRootContainer->m_pRightPaneContainer != NULL)
//               pFound = m_pRootContainer->m_pRightPaneContainer
//                            ->FindSubPaneContainer(pSlider, BC_FIND_BY_SLIDER);
//           if (pFound == NULL) return;
//       }
//       pFound->m_pSlider = NULL;                      // +0x18
//   }
//
// DEVIATION: the retail null-pSlider path raises the invalid-argument exception;
// this returns instead.
extern "C" void MS_ABI impl__RemovePaneDivider_CPaneContainerManager__UEAAXPEAVCPaneDivider___Z(
    void* pThis, void* pSlider) {
    if (pThis == nullptr) return;
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        std::vector<void*>& sliders = SliderList(pThis);
        auto it = std::find(sliders.begin(), sliders.end(), pSlider);
        if (it != sliders.end()) sliders.erase(it);
    }
    // DEVIATION: retail also clears pSlider->m_pContainerManager (+0x208) when
    // the node was found. That store is NOT made here -- see the note where
    // kOffDividerManager used to be defined: OpenMFC's CPaneDivider is 24 bytes,
    // so +0x208 is past the end of it. Nothing in this DLL reads that member.
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot == nullptr || pSlider == nullptr) return;
    void* pFound = nullptr;
    if (Field<void*>(pRoot, kOffContainerSlider) == pSlider) {
        pFound = pRoot;
    } else {
        void* pLeft = Field<void*>(pRoot, 0x20);   // CPaneContainer::m_pLeftPaneContainer
        void* pRight = Field<void*>(pRoot, 0x28);  // CPaneContainer::m_pRightPaneContainer
        if (pLeft != nullptr) {
            pFound = impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                pLeft, pSlider, kFindBySlider);
        }
        if (pFound == nullptr && pRight != nullptr) {
            pFound = impl__FindSubPaneContainer_CPaneContainer__QEAAPEAV1_PEBVCObject__W4BC_FIND_CRITERIA_1__Z(
                pRight, pSlider, kFindBySlider);
        }
    }
    if (pFound != nullptr) Field<void*>(pFound, kOffContainerSlider) = nullptr;
}

// Symbol: ?RemovePaneFromPaneContainer@CPaneContainerManager@@UEAAHPEAVCDockablePane@@@Z
// Retail entry RVA 0xa8c30 (mfc140.dll), 112 instructions -- an earlier revision
// of this comment said 401, which is what disas.py prints before the next known
// export. What it actually does:
//
//   if (m_pRootContainer == NULL) return FALSE;
//   BOOL bLeftBar = FALSE;
//   CPaneContainer* p = FindPaneContainer(pControlBar, bLeftBar);  // vtable +0x128
//   if (p == NULL) return FALSE;
//   p->DeletePane(pControlBar, bLeftBar ? BC_FIND_BY_LEFT_BAR       // vtable +0x60
//                                       : BC_FIND_BY_RIGHT_BAR);
//   m_pRootContainer->CheckPaneDividerVisibility();                 // 0xa71c0
//   if (p->m_pSlider != NULL) {                                     // +0x18
//       if (m_lstSliders does not contain p->m_pSlider)
//           AfxThrowInvalidArgException();                          // 0x225b80
//       p->m_pSlider->ShowWindow(SW_HIDE);                          // 0x2a79e0
//   }
//   POSITION pos = <node of m_lstControlBars holding pControlBar>;
//   if (pos != NULL) {
//       CList<HWND,HWND> lst;                    // stack local, vftable 0x1802e7198
//       for (each o in m_lstControlBars)
//           lst.AddTail(DYNAMIC_DOWNCAST(CWnd, o) ? o->m_hWnd : NULL);
//       pControlBar->m_recentDockInfo.SaveListOfRecentPanes(          // 0xd2310
//           lst, !m_pDockSite->IsKindOf(<CRuntimeClass at 0x1803aa418>));
//       m_lstControlBars.RemoveAt(pos);                             // 0x2306f0
//   }
//   return TRUE;
//
// (+0x2b8 of CDockablePane is the CRecentDockSiteInfo the save call is made on;
// 0xd2310 resolves to ?SaveListOfRecentPanes@CRecentDockSiteInfo@@QEAAXAEAV?$CList@PEAUHWND__@@PEAU1@@@H@Z.)
// There is no ReleaseEmptyPaneContainer call, no sibling re-parenting and no
// divider destruction anywhere in the body; the earlier description of those was
// wrong.
// Left a stub: ?SaveListOfRecentPanes@CRecentDockSiteInfo@@ has no reachable
// implementation here, and the m_lstSliders membership test would raise the
// invalid-argument exception on every call whose container has a divider,
// because this file's slider table is never populated: the only entry point that
// would fill it, ?CreatePaneDivider@, is a stub (see its comment).
// (An earlier revision of this comment also named CPaneContainer::DeletePane as
// a blocker; docking/CPaneContainer.cpp implements it now.)
extern "C" int MS_ABI impl__RemovePaneFromPaneContainer_CPaneContainerManager__UEAAHPEAVCDockablePane___Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ReplacePane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@0@Z
// Decoded from retail ?ReplacePane@CPaneContainerManager@@UEAAHPEAVCDockablePane@@0@Z,
// entry RVA 0xa9e80 (mfc140.dll):
//
//   POSITION pos = <first node of m_lstControlBars whose data == pBarOld>;
//   if (pos == NULL) { m_lstControlBars.AddTail(pBarNew); return TRUE; }
//   BOOL bLeftBar = FALSE;
//   CPaneContainer* pContainer = FindPaneContainer(pBarOld, bLeftBar);  // vtable +0x128
//   if (pContainer == NULL) return TRUE;                                // nothing else happens
//   if (bLeftBar) pContainer->m_pBarLeftTop = pBarNew;                  // +0x08
//   else          pContainer->m_pBarRightBottom = pBarNew;              // +0x10
//   m_lstControlBars.InsertAfter(pos, pBarNew);                         // 0x230690
//   m_lstControlBars.RemoveAt(pos);
//   return TRUE;                                                       // every path
//
// The two container stores are exactly what ?SetPane@CPaneContainer@@ (entry RVA
// 0xa5200) does, so they go through that export here.
//
// NOTE: this file's FindPaneContainer always reports bLeftBar == FALSE, so the
// left-pane store is currently unreachable. That imprecision is in
// FindPaneContainer, not here.
//
// DEVIATION: FindPaneContainer is called directly rather than through vtable
// slot +0x128 (for an unsubclassed manager that slot is this same body), and the
// g_paneToContainer side table is re-pointed at pBarNew so the rest of this
// file's container lookups stay consistent; retail has no such table.
extern "C" int MS_ABI impl__ReplacePane_CPaneContainerManager__UEAAHPEAVCDockablePane__0_Z(
    void* pThis, void* pBarOld, void* pBarNew) {
    if (pThis == nullptr) return TRUE;
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        std::vector<void*>* panes = PaneListLocked(pThis);
        const bool bFound = panes != nullptr &&
                            std::find(panes->begin(), panes->end(), pBarOld) != panes->end();
        if (!bFound) {
            AddUniquePane(g_containerManagerPanes[pThis], pBarNew);
            return TRUE;
        }
    }
    int bLeftBar = FALSE;
    void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
        pThis, pBarOld, &bLeftBar);
    if (pContainer == nullptr) return TRUE;
    impl__SetPane_CPaneContainer__QEAAXPEAVCDockablePane__H_Z(pContainer, pBarNew, bLeftBar);
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        std::vector<void*>* panes = PaneListLocked(pThis);
        if (panes != nullptr) {
            auto pos = std::find(panes->begin(), panes->end(), pBarOld);
            if (pos != panes->end()) *pos = pBarNew;
        }
        g_paneToContainer.erase(pBarOld);
        if (pBarNew != nullptr) g_paneToContainer[pBarNew] = pContainer;
    }
    return TRUE;
}

// Symbol: ?ResizePaneContainers@CPaneContainerManager@@UEAAXIHHAEAPEAX@Z
// Decoded from retail ?ResizePaneContainers@CPaneContainerManager@@UEAAXIHHAEAPEAX@Z,
// entry RVA 0xa9de0 (mfc140.dll):
//
//   if (m_pRootContainer == NULL) return;
//   m_pRootContainer->StretchPaneContainer(                 // vtable +0x78
//       bExpand ? nOffset : -nOffset,
//       (UINT)(nSide - 1) <= 1,      // setbe on (nSide-1) vs 1 -> nSide is 1 or 2
//       TRUE,                        // bLeftBar
//       TRUE,                        // bMoveSlider
//       hdwp);
extern "C" void MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXIHHAEAPEAX_Z(
    void* pThis, unsigned int nSide, int bExpand, int nOffset, void** phdwp) {
    if (pThis == nullptr) return;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot == nullptr) return;
    const int bStretchHorz = (nSide - 1u) <= 1u ? TRUE : FALSE;
    impl__StretchPaneContainer_CPaneContainer__UEAAHHHHHAEAPEAX_Z(
        pRoot, bExpand ? nOffset : -nOffset, bStretchHorz, TRUE, TRUE, phdwp);
}

// Symbol: ?ResizePaneContainers@CPaneContainerManager@@UEAAXVCRect@@AEAPEAX@Z
// Decoded from retail ?ResizePaneContainers@CPaneContainerManager@@UEAAXVCRect@@AEAPEAX@Z,
// entry RVA 0xa9e40 (mfc140.dll):
//
//   if (m_pRootContainer == NULL) return;
//   m_pRootContainer->Resize(rect, hdwp, FALSE);            // vtable +0xa0
//
// (`rect` is a by-value CRect, so it arrives as a pointer and retail copies the
// 16 bytes into its own outgoing slot before the call.)
extern "C" void MS_ABI impl__ResizePaneContainers_CPaneContainerManager__UEAAXVCRect__AEAPEAX_Z(
    void* pThis, const RECT* pRect, void** phdwp) {
    if (pThis == nullptr) return;
    void* pRoot = Field<void*>(pThis, kOffRootContainer);
    if (pRoot == nullptr) return;
    RECT rectCopy = pRect ? *pRect : RECT{0, 0, 0, 0};
    impl__Resize_CPaneContainer__UEAAXVCRect__AEAPEAXH_Z(pRoot, &rectCopy, phdwp, FALSE);
}

// Symbol: ?Serialize@CPaneContainerManager@@UEAAXAEAVCArchive@@@Z
// STUB. Retail entry RVA 0xa9f40 (mfc140.dll). (An earlier revision of this
// comment cited "310 instructions" -- the print-until-next-export count -- and
// named a CPaneContainer::SetUpByID dependency that does not appear in the body.)
// The two halves, split on `test $1,ar.m_nMode` at 0xa9f64 -- MFC spells
// CArchive::store as 0 and load as 1, so the FALL-THROUGH is the storing path:
//
//   store (0xa9f6e):
//       m_pRootContainer->ReleaseEmptyPaneContainer();      // 0xa4e90
//       m_pRootContainer->Serialize(ar);                    // vtable +0x10
//       ar << m_lstControlBars.GetCount();                  // inlined buffer write
//       for (each o in m_lstControlBars) {
//           int nID = ((CWnd*)o)->GetDlgCtrlID();           // 0x2a78b0
//           if (nID != -1) ar << nID;
//           else {
//               CBaseTabbedPane* t = DYNAMIC_DOWNCAST(CBaseTabbedPane, o); // 0x1802db168
//               CWnd* w = t->{vtable +0x7a8}(NULL, 0);
//               if (w != NULL) { ar << (int)-1; ar << w->GetDlgCtrlID(); }
//           }
//       }
//   load (0xaa0c3):
//       m_pRootContainer->Serialize(ar);                    // vtable +0x10
//       CWnd* w;
//       if (m_pDefaultSlider != NULL)                                  // +0x88
//           w = m_pDefaultSlider->GetDockSiteFrameWnd();               // vtable +0x348
//       else {
//           // 0xaa0f7: not a CPaneFrameWnd -> throw, there is no fallback
//           if (!m_pDockSite->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) throw;
//           w = CWnd::FromHandlePermanent(m_pDockSite->{+0x130});      // 0x2891d0
//       }
//       CDockingManager* pMgr = afxGlobalUtils.GetDockingManager(w);   // 0x6ccc0
//       if (pMgr == NULL) throw;    // both bail-outs land on 0xaa288, which
//                                   // allocates a 0x20-byte exception object and
//                                   // hands it to _CxxThrowException
//       int nCount; ar >> nCount;
//       for (nCount times) { int nID; ar >> nID; if (nID != -1) pMgr->{vt +0x58}(nID, TRUE); ... }
//
// Left a stub. The storing half writes straight into CArchive's m_lpBufCur /
// m_lpBufMax (+0x38 / +0x40) with inlined bounds checks against
// ?Flush@CArchive@@ and ?FillBuffer@CArchive@@; reproducing that from here means
// reaching into CArchive's buffer, and ?Serialize@CPaneContainer@@ -- the first
// thing both halves call -- is a generated stub in docking/CPaneContainer.cpp,
// so the stream would be truncated on the very first element. A half-written
// dock-state stream is worse than none, because it would be read back.
extern "C" void MS_ABI impl__Serialize_CPaneContainerManager__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?SetDefaultPaneDividerForPanes@CPaneContainerManager@@QEAAXPEAVCPaneDivider@@@Z
// Decoded from retail ?SetDefaultPaneDividerForPanes@CPaneContainerManager@@QEAAXPEAVCPaneDivider@@@Z,
// entry RVA 0xaa910 (mfc140.dll):
//
//   for (each object o in m_lstControlBars)
//       if (o != NULL && o->IsKindOf(RUNTIME_CLASS(CDockablePane)))
//           ((CDockablePane*)o)->SetDefaultPaneDivider(NULL);
//
// The NULL is not a transcription slip: the `pSlider` parameter arrives in %rdx,
// is never spilled and never re-read -- %rdx is overwritten with the
// RUNTIME_CLASS pointer for the IsKindOf call and then zeroed with
// `xor %edx,%edx` (0xaa93f) immediately before the call to
// ?SetDefaultPaneDivider@CDockablePane@@QEAAXPEAUHWND__@@@Z (0x455f0), on the
// single unconditional path that reaches it. Retail really does clear every
// pane's default divider here regardless of the argument.
extern "C" void MS_ABI impl__SetDefaultPaneDividerForPanes_CPaneContainerManager__QEAAXPEAVCPaneDivider___Z(
    void* pThis, void* pSlider) {
    if (pThis == nullptr) return;
    (void)pSlider;  // dead in retail too -- see above
    CRuntimeClass* pDockableClass = impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pDockableClass);
        if (p != nullptr) {
            impl__SetDefaultPaneDivider_CDockablePane__QEAAXPEAUHWND_____Z(p, nullptr);
        }
    }
}

// Symbol: ?SetResizeMode@CPaneContainerManager@@UEAAXH@Z
// Decoded from retail ?SetResizeMode@CPaneContainerManager@@UEAAXH@Z, entry RVA
// 0xa9f20 (mfc140.dll):
//
//   for (each node in m_lstControlBars)
//       *(int*)((char*)node->data + 0x46c) = bResize;
//
// +0x46c is CDockablePane::m_bIsResizing (see kOffPaneIsResizing above). Retail
// writes it with no type check and no NULL check at all -- the loop body is
// literally `mov 0x10(%r8),%rax; mov (%r8),%r8; mov %edx,0x46c(%rax)`.
//
// DEVIATION: guarded with IsKindOf(CDockablePane). m_lstControlBars only ever
// receives CDockablePane* through AddPaneToList/AddPane, so the guard changes
// nothing for well-formed state, but without it a shorter CBasePane-derived
// object reaching this list would be corrupted -- OpenMFC's panes are not all
// the retail size.
extern "C" void MS_ABI impl__SetResizeMode_CPaneContainerManager__UEAAXH_Z(void* pThis, int bResize) {
    if (pThis == nullptr) return;
    CRuntimeClass* pDockableClass = impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    std::vector<void*>* panes = PaneListLocked(pThis);
    if (panes == nullptr) return;
    for (void* obj : *panes) {
        void* p = DynDowncast(obj, pDockableClass);
        if (p != nullptr) Field<int>(p, kOffPaneIsResizing) = bResize;
    }
}

// Symbol: ?StoreRecentDockSiteInfo@CPaneContainerManager@@UEAAXPEAVCDockablePane@@@Z
// Decoded from retail ?StoreRecentDockSiteInfo@CPaneContainerManager@@UEAAXPEAVCDockablePane@@@Z,
// entry RVA 0xa8bc0 (mfc140.dll). Nineteen instructions, 0xa8bc0..0xa8c06:
//
//   BOOL bLeftBar = TRUE;                                  // movl $1, 0x30(%rsp)
//   CPaneContainer* p = this->FindPaneContainer(pBar, bLeftBar);  // vtable +0x128
//   if (p != NULL) p->StoreRecentDockSiteInfo(pBar);        // vtable +0x68
//
// The `1` seeded into the bLeftBar local at 0xa8bd1 is not a typo in this
// transcription: it really is TRUE, not FALSE, and it is written before the
// FindPaneContainer call, which overwrites it.
//
// CPaneContainerManager vtable +0x128 is FindPaneContainer and CPaneContainer
// vtable +0x68 is StoreRecentDockSiteInfo; both slot numbers follow from the
// declaration order in the shipping afxpanecontainermanager.h /
// afxpanecontainer.h after CObject's five slots, and +0x128 is corroborated by
// ?RemovePaneFromPaneContainer@ (0xa8c68) using the same slot for the same call.
//
// DEVIATIONS: FindPaneContainer is called as this file's own body rather than
// through the vtable, so a derived override would be missed. And
// ?StoreRecentDockSiteInfo@CPaneContainer@@ is implemented in
// docking/CPaneContainer.cpp, but it ends in ?StoreDockInfo@CRecentDockSiteInfo@@,
// which is still an empty stub -- so the call is structurally faithful and
// currently records nothing. (An earlier revision of this comment called the
// CPaneContainer half itself a generated stub; it is not.)
extern "C" void MS_ABI impl__StoreRecentDockSiteInfo_CPaneContainerManager__UEAAXPEAVCDockablePane___Z(
    void* pThis, void* pBar) {
    if (pThis == nullptr) return;
    int bLeftBar = TRUE;
    void* pContainer = impl__FindPaneContainer_CPaneContainerManager__UEAAPEAVCPaneContainer__PEAVCDockablePane__AEAH_Z(
        pThis, pBar, &bLeftBar);
    if (pContainer != nullptr) {
        impl__StoreRecentDockSiteInfo_CPaneContainer__UEAAXPEAVCDockablePane___Z(pContainer, pBar);
    }
}
