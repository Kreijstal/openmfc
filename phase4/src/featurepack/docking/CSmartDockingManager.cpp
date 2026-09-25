// CSmartDockingManager — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The smart-docking controller.  Retail declaration:
// atlmfc/include/afxsmartdockingmanager.h:28 (class CSmartDockingManager :
// public CObject, no DECLARE_DYNAMIC).  OpenMFC declares no matching class, so
// every body below works on `void* pThis` through the layout pinned in
// SdManagerLayout, which was read from the retail constructor, destructor,
// scalar deleting destructor and the member functions themselves.
//
// Images.  Every body was read in mfc140.dll with disas.py and each one cited
// below was checked against mfc140u.dll: the instruction streams are identical
// once rip-relative displacements are masked out (first 60 instructions of each
// compared).  mfc140u_rva_symbols.json maps only seven of these exports (ctor,
// dtor, Create, Destroy, HidePlace, MoveCentralGroup, CauseCancelMode); every
// mfc140u RVA cited below, including the other nine, was read from mfc140u.dll's
// own export address table through the export ordinal in
// mfc_complete_ordinal_mapping.json.  In this region each one happens to be the
// mfc140.dll RVA minus 0xc80.  Review re-compared all 16 bodies in full with
// addresses masked, and they are identical.
//
// Objects this class talks to:
//   * m_arMarkers[0..3] / m_pCentralGroup are produced by the CRuntimeClass
//     factories passed to Create (retail calls m_pfnCreateObject directly), and
//     m_arMarkers[4..8] come from m_pCentralGroup->GetGuide().  Retail calls
//     their virtuals through their own vftables, and so does this file: OpenMFC
//     has no C++ class for either guide type, so any such object that exists
//     was built by an MSVC-compiled factory and carries an MSVC vftable whose
//     slot numbers are the retail ones (verified below).  See Create for what
//     happens when OpenMFC's own descriptors are chosen.
//   * the docking window passed to Start is an ordinary CWnd-family object that
//     OpenMFC itself may have created, so its virtuals are NOT dispatched by MSVC
//     slot number; Start calls the exported thunk instead (Deviation noted there).
//   * The member functions of m_wndPlaceMarker (an embedded
//     CSmartDockingHighlighterWnd) and the non-virtual
//     CSmartDockingGroupGuidesManager::GetWindowRect / ShowGuide are called only
//     through their exported impl__ thunks.  Two highlighter FIELDS are accessed
//     directly, as retail does inline: Start writes m_pDockingWnd (+0xf0, the inline
//     SetDockingWnd) and Show reads m_bTabbed (+0x118, friend access).  At the time
//     of writing, every one of those thunk DEFINITIONS except the highlighter
//     constructor (in CSmartDockingHighlighterWnd.cpp and
//     CSmartDockingGroupGuidesManager.cpp) is an auto-generated placeholder with an
//     EMPTY body and a parameter list that lacks `this` (ShowGuide's has 43 bogus
//     parameters).  The constructor takes `this` but only returns it.  The
//     declarations below follow the mangled names; the calls are
//     therefore correct as soon as those definitions are repaired, and until then
//     they reach an empty body, which reads none of its arguments, so passing the
//     real ones is harmless under the x64 MS ABI.  The repairs are reported as
//     header/file requests; this file does not touch those files.
//
// Retail guide vftable slots used here, read from the mfc140.dll vftables that
// the retail constructors install (VA 0x18030d9b8 CSmartDockingStandaloneGuide,
// VA 0x18030d568 CSmartDockingGroupGuidesManager -- mfc140.dll addresses):
//   slot 1 (+0x08) scalar deleting destructor (both)
//   slot 5 (+0x28) Create      slot 6 (+0x30) Destroy     slot 7 (+0x38) Show
//   slot 8 (+0x40) AdjustPos   (guide: void(CRect); group: BOOL(CRect, int))
//   slot 9 (+0x48) guide: Highlight(BOOL); group: GetGuide(SDMarkerPlace) (the
//                  inline body at mfc140.dll RVA 0x12fa60: &m_arMarkers[n-4] at
//                  +0x118 + (n-4)*0x4a8, NULL outside sdCLEFT..sdCMIDDLE)
//   slot 10 (+0x50) guide: IsPtIn(CPoint) const

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

// ---- featurepack/docking/CSmartDockingHighlighterWnd.cpp (m_wndPlaceMarker).
// Declarations follow the mangled names.  Except for the constructor, the
// definitions there are placeholder stubs without `this` (see header comment).
extern "C" void* MS_ABI impl___0CSmartDockingHighlighterWnd__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___1CSmartDockingHighlighterWnd__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Create_CSmartDockingHighlighterWnd__QEAAXPEAVCWnd___Z(void* pThis, CWnd* pwndOwner);
extern "C" void MS_ABI impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__ShowAt_CSmartDockingHighlighterWnd__QEAAXVCRect___Z(void* pThis, const RECT* pRect);
extern "C" void MS_ABI impl__ShowTabbedAt_CSmartDockingHighlighterWnd__QEAAXVCRect__0_Z(
    void* pThis, const RECT* pRect, const RECT* pRectTab);
// ---- featurepack/docking/CSmartDockingGroupGuidesManager.cpp (non-virtual
// members called on m_pCentralGroup).  Placeholder definitions there (see header).
extern "C" void MS_ABI impl__GetWindowRect_CSmartDockingGroupGuidesManager__QEAAXAEAVCRect___Z(void* pThis, RECT* pRect);
extern "C" void MS_ABI impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
    void* pThis, int nMarkerNo, int bShow, int bRedraw);
// ---- featurepack/docking/RuntimeClasses.cpp -- the default class descriptors.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSmartDockingStandaloneGuide__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSmartDockingGroupGuidesManager__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
// ---- featurepack/docking/CDockablePane.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();
// ---- featurepack/docking/CPaneFrameWnd.cpp
extern "C" void* MS_ABI impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(void* pThis);
// ---- featurepack/docking/CTabbedPane.cpp
extern "C" int impl__m_bTabsAlwaysTop_CTabbedPane__2HA;
// ---- core/runtime/CRuntimeClass.cpp, core/runtime/CObject.cpp
extern "C" int MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(
    const CRuntimeClass* pThis, const CRuntimeClass* pBaseClass);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
// ---- core/window/Thunks.cpp
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
// ---- detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// Declared here because Create/Destroy/the destructor call them (defined below).
extern "C" void MS_ABI impl__Destroy_CSmartDockingManager__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__Stop_CSmartDockingManager__QEAAXXZ(void* pThis);

namespace {

// Retail object layout, sizeof == 0x1c0.
//   * The scalar deleting destructor (vftable slot 1, unexported; mfc140.dll RVA
//     0x1325c0) passes 0x1c0 on its sized-delete path.
//   * The constructor (RVA 0x1318e0 mfc140u) zeroes the qwords at +0x08, +0x10,
//     +0x18, +0x20, +0x2c and +0x34 (so +0x08..+0x27 and +0x2c..+0x3b;
//     m_dwEnabledAlignment at +0x28 is NOT initialised), constructs the
//     CSmartDockingHighlighterWnd at +0x40, stores -1 (sdNONE) at +0x1b8 and
//     memset()s the 0x48 bytes at +0x168 (m_arMarkers[9]); m_pCentralGroup at
//     +0x1b0 is not initialised.
//   * Create (RVA 0x1319c0 mfc140u) stores m_pCentralGroup at +0x1b0,
//     m_arMarkers[i] at +0x168 + 8*i, m_pwndOwner at +0x18 and m_bCreated = 1 at
//     +0x0c; Start (RVA 0x131b70 mfc140u) stores m_pDockingWnd at +0x20,
//     m_dwEnabledAlignment at +0x28, m_bShown/m_bCentralGroupShown at +0x10/+0x14
//     and m_bStarted at +0x08; SetOuterRect copies the CRect to +0x2c.
// The declaration order in afxsmartdockingmanager.h:75-88 matches these offsets.
struct SdManagerLayout {
    void*         vfptr;                   // +0x000
    BOOL          m_bStarted;              // +0x008
    BOOL          m_bCreated;              // +0x00c
    BOOL          m_bShown;                // +0x010
    BOOL          m_bCentralGroupShown;    // +0x014
    CWnd*         m_pwndOwner;             // +0x018
    CWnd*         m_pDockingWnd;           // +0x020
    DWORD         m_dwEnabledAlignment;    // +0x028
    RECT          m_rcOuter;               // +0x02c  CRect
    int           _pad3c;                  // +0x03c
    unsigned char m_wndPlaceMarker[0x128]; // +0x040  CSmartDockingHighlighterWnd
    void*         m_arMarkers[9];          // +0x168  CSmartDockingStandaloneGuide*
    void*         m_pCentralGroup;         // +0x1b0  CSmartDockingGroupGuidesManager*
    int           m_nHiliteSideNo;         // +0x1b8  SDMarkerPlace
    int           _pad1bc;                 // +0x1bc
};
static_assert(offsetof(SdManagerLayout, m_bStarted) == 0x08, "m_bStarted +0x08");
static_assert(offsetof(SdManagerLayout, m_bCreated) == 0x0c, "m_bCreated +0x0c");
static_assert(offsetof(SdManagerLayout, m_bShown) == 0x10, "m_bShown +0x10");
static_assert(offsetof(SdManagerLayout, m_bCentralGroupShown) == 0x14, "m_bCentralGroupShown +0x14");
static_assert(offsetof(SdManagerLayout, m_pwndOwner) == 0x18, "m_pwndOwner +0x18");
static_assert(offsetof(SdManagerLayout, m_pDockingWnd) == 0x20, "m_pDockingWnd +0x20");
static_assert(offsetof(SdManagerLayout, m_dwEnabledAlignment) == 0x28, "m_dwEnabledAlignment +0x28");
static_assert(offsetof(SdManagerLayout, m_rcOuter) == 0x2c, "m_rcOuter +0x2c");
static_assert(offsetof(SdManagerLayout, m_wndPlaceMarker) == 0x40, "m_wndPlaceMarker +0x40");
static_assert(offsetof(SdManagerLayout, m_arMarkers) == 0x168, "m_arMarkers +0x168");
static_assert(offsetof(SdManagerLayout, m_pCentralGroup) == 0x1b0, "m_pCentralGroup +0x1b0");
static_assert(offsetof(SdManagerLayout, m_nHiliteSideNo) == 0x1b8, "m_nHiliteSideNo +0x1b8");
static_assert(sizeof(SdManagerLayout) == 0x1c0, "scalar deleting dtor frees 0x1c0");

// CSmartDockingHighlighterWnd members used here (offsets within the embedded
// object).  The names come from the declaration order in
// afxsmartdockinghighlighterwnd.h (after CWnd's 0xe8 bytes: m_pWndOwner +0xe8,
// m_pDockingWnd +0xf0, m_rectLast +0xf8, m_rectTab +0x108, m_bTabbed +0x118,
// m_bShown +0x11c, m_bUseThemeColorInShading +0x120).  That order agrees with the
// retail highlighter constructor (mfc140.dll RVA 0x131ed0), which zeroes +0xe8,
// +0xf8..+0x117, the qword at +0x118 and the dword at +0x120, but NOT +0xf0
// (m_pDockingWnd is left uninitialised).  It also agrees with its Hide
// (mfc140.dll RVA 0x132180), which tests and clears +0x11c and reads the two
// window pointers at +0xe8 / +0xf0.  The scalar deleting destructor (mfc140.dll
// RVA 0x131f40) frees 0x128 bytes.
// NOTE: OpenMFC's highlighter constructor thunk initialises none of these fields,
// so m_bTabbed is indeterminate in a manager built here.  Show reads it only to
// decide whether to call Hide, which is currently an empty placeholder.
constexpr std::size_t kHlPDockingWnd = 0xf0;   // m_pDockingWnd (inline SetDockingWnd)
constexpr std::size_t kHlBTabbed     = 0x118;  // m_bTabbed

// CSmartDockingGroupGuidesManager::m_bMiddleIsOn.  The retail constructor
// (mfc140.dll RVA 0x130fc0) builds m_rgnBase (CRgn) at +0x19f8 and zeroes the
// qword at +0x1a08 (m_bCreated, m_bMiddleIsOn) and the dword at +0x1a10
// (m_bLayered); afxsmartdockingguide.h declares them in that order.
constexpr std::size_t kGgmBMiddleIsOn = 0x1a0c;

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd +0x40");
static_assert(offsetof(CBasePane, m_dwEnabledAlignment) == 0x100, "CBasePane::m_dwEnabledAlignment +0x100");

// afxsmartdockingguide.h SDMarkerPlace.
enum : int { kSdNone = -1, kSdLeft = 0, kSdBottom = 3, kSdCLeft = 4, kSdCRight = 5,
             kSdCTop = 6, kSdCBottom = 7, kSdCMiddle = 8 };

inline SdManagerLayout* Mgr(void* pThis) { return static_cast<SdManagerLayout*>(pThis); }
inline void* PlaceMarker(void* pThis) { return Mgr(pThis)->m_wndPlaceMarker; }

// CBRS_ALIGN_* values from atlmfc/include/afxres.h:64-68.  Spelled out here
// because OpenMFC's headers do not define the four side bits, and the
// CBRS_ALIGN_ANY they do define (include/openmfc/afxole.h:129) is 0x000F, not
// retail's 0xF000 -- Start stores the literal 0xf000 (`movl $0xf000,0x28(%rcx)`).
constexpr DWORD kAlignLeft   = 0x1000;
constexpr DWORD kAlignTop    = 0x2000;
constexpr DWORD kAlignRight  = 0x4000;
constexpr DWORD kAlignBottom = 0x8000;
constexpr DWORD kAlignAny    = 0xF000;

// Retail tests one CBRS_ALIGN_* bit per side marker: sdLEFT <-> bit 12
// (CBRS_ALIGN_LEFT), sdRIGHT <-> bit 14 (CBRS_ALIGN_RIGHT), sdTOP <-> bit 13
// (CBRS_ALIGN_TOP), sdBOTTOM <-> bit 15 (CBRS_ALIGN_BOTTOM) -- the four `bt`
// tests in Start and Show.
inline bool SideEnabled(DWORD dwAlign, int i) {
    return ((dwAlign & kAlignLeft) != 0 && i == 0) ||
           ((dwAlign & kAlignRight) != 0 && i == 1) ||
           ((dwAlign & kAlignTop) != 0 && i == 2) ||
           ((dwAlign & kAlignBottom) != 0 && i == 3);
}

// ---- MSVC-vftable dispatch on the guide objects (slots listed in the header).
inline void* Slot(void* p, int slot) { return (*static_cast<void***>(p))[slot]; }
inline void VDelete(void* p) {                        // slot 1, flags = 1
    using Fn = void* (MS_ABI*)(void*, unsigned int);
    reinterpret_cast<Fn>(Slot(p, 1))(p, 1);
}
inline void VGuideCreate(void* p, int nSideNo, CWnd* pwndOwner) {   // slot 5
    using Fn = void (MS_ABI*)(void*, int, CWnd*);
    reinterpret_cast<Fn>(Slot(p, 5))(p, nSideNo, pwndOwner);
}
inline void VGroupCreate(void* p, CWnd* pwndOwner) {                // slot 5
    using Fn = void (MS_ABI*)(void*, CWnd*);
    reinterpret_cast<Fn>(Slot(p, 5))(p, pwndOwner);
}
inline void VDestroy(void* p) {                                     // slot 6
    using Fn = void (MS_ABI*)(void*);
    reinterpret_cast<Fn>(Slot(p, 6))(p);
}
inline void VShow(void* p, BOOL bShow) {                            // slot 7
    using Fn = void (MS_ABI*)(void*, BOOL);
    reinterpret_cast<Fn>(Slot(p, 7))(p, bShow);
}
// CRect by value = pointer to a caller-owned copy under the x64 MS ABI; each
// call gets its own copy, as retail's CRect temporaries do.
inline void VGuideAdjustPos(void* p, const RECT& rc) {              // slot 8
    using Fn = void (MS_ABI*)(void*, RECT*);
    RECT copy = rc;
    reinterpret_cast<Fn>(Slot(p, 8))(p, &copy);
}
inline BOOL VGroupAdjustPos(void* p, const RECT& rc, int nMiddleIsOn) {  // slot 8
    using Fn = BOOL (MS_ABI*)(void*, RECT*, int);
    RECT copy = rc;
    return reinterpret_cast<Fn>(Slot(p, 8))(p, &copy, nMiddleIsOn);
}
inline void VHighlight(void* p, BOOL bHiLite) {                     // slot 9
    using Fn = void (MS_ABI*)(void*, BOOL);
    reinterpret_cast<Fn>(Slot(p, 9))(p, bHiLite);
}
inline void* VGetGuide(void* p, int nMarkerNo) {                    // slot 9
    using Fn = void* (MS_ABI*)(void*, int);
    return reinterpret_cast<Fn>(Slot(p, 9))(p, nMarkerNo);
}
inline BOOL VIsPtIn(void* p, long long point) {                     // slot 10
    using Fn = BOOL (MS_ABI*)(void*, long long);
    return reinterpret_cast<Fn>(Slot(p, 10))(p, point);
}

} // namespace

// Symbol: ??0CSmartDockingManager@@QEAA@XZ
// Transcribed from retail RVA 0x1318e0 (mfc140u):
//     vfptr = CSmartDockingManager::`vftable';
//     *(qword*)+0x08 = *(qword*)+0x10 = *(qword*)+0x18 = *(qword*)+0x20 = 0;
//     *(qword*)+0x2c = *(qword*)+0x34 = 0;             // m_rcOuter
//     ??0CSmartDockingHighlighterWnd(&m_wndPlaceMarker);
//     m_nHiliteSideNo = sdNONE;
//     memset(m_arMarkers, 0, 0x48);
//     return this;
// Deviation: no vftable is installed -- OpenMFC models none for this class
// (the header declares no virtuals beyond CObject's and the virtual destructor;
// in the retail vftable, mfc140.dll VA 0x18030ddf8, slot 1 is the scalar
// deleting destructor).  m_dwEnabledAlignment and m_pCentralGroup are left
// uninitialised, as in retail.
extern "C" void* MS_ABI impl___0CSmartDockingManager__QEAA_XZ(void* pThis) {
    SdManagerLayout* m = Mgr(pThis);
    m->m_bStarted = FALSE;
    m->m_bCreated = FALSE;
    m->m_bShown = FALSE;
    m->m_bCentralGroupShown = FALSE;
    m->m_pwndOwner = nullptr;
    m->m_pDockingWnd = nullptr;
    m->m_rcOuter.left = m->m_rcOuter.top = m->m_rcOuter.right = m->m_rcOuter.bottom = 0;
    impl___0CSmartDockingHighlighterWnd__QEAA_XZ(PlaceMarker(pThis));
    m->m_nHiliteSideNo = kSdNone;
    for (void*& p : m->m_arMarkers) {
        p = nullptr;
    }
    return pThis;
}
// Symbol: ??1CSmartDockingManager@@UEAA@XZ
// Transcribed from retail RVA 0x131990 (mfc140u):
//     vfptr = CSmartDockingManager::`vftable';
//     Destroy();
//     m_wndPlaceMarker.~CSmartDockingHighlighterWnd();   // inlined: installs the
//                                                        // highlighter vftable at
//                                                        // +0x40, tail-jumps ??1CWnd
// Deviation: no vftable store (none is modelled, see the constructor); the
// member destructor is reached through the exported
// ??1CSmartDockingHighlighterWnd thunk instead of being inlined.  That thunk is
// currently a placeholder with an empty body, which matches OpenMFC's equally
// empty highlighter constructor (it constructs no CWnd for ~CWnd to undo).
extern "C" void MS_ABI impl___1CSmartDockingManager__UEAA_XZ(void* pThis) {
    impl__Destroy_CSmartDockingManager__QEAAXXZ(pThis);
    impl___1CSmartDockingHighlighterWnd__UEAA_XZ(PlaceMarker(pThis));
}

// Symbol: ?CauseCancelMode@CSmartDockingManager@@QEAAXXZ
// Transcribed from retail RVA 0x132390 (mfc140u):
//     if (m_bStarted)
//         ::SendMessage(m_pDockingWnd->m_hWnd, WM_CANCELMODE, 0, 0);
// The import slot (mfc140.dll 0x1802c5378) resolves to SendMessageA, i.e.
// SendMessageW in mfc140u.  Retail does not NULL-check m_pDockingWnd.
extern "C" void MS_ABI impl__CauseCancelMode_CSmartDockingManager__QEAAXXZ(void* pThis) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted != 0) {
        ::SendMessage(m->m_pDockingWnd->m_hWnd, WM_CANCELMODE, 0, 0);
    }
}

// Symbol: ?Create@CSmartDockingManager@@QEAAXPEAVCWnd@@PEAUCRuntimeClass@@1@Z
// Transcribed from retail RVA 0x1319c0 (mfc140u):
//     CRuntimeClass* rtMarker  = prtMarker ? prtMarker : RUNTIME_CLASS(CSmartDockingStandaloneGuide);
//     CRuntimeClass* rtCentral = prtCentralGroup ? prtCentralGroup : RUNTIME_CLASS(CSmartDockingGroupGuidesManager);
//     if (!rtMarker->IsDerivedFrom(RUNTIME_CLASS(CSmartDockingStandaloneGuide)))       AfxThrowInvalidArgException();
//     if (!rtCentral->IsDerivedFrom(RUNTIME_CLASS(CSmartDockingGroupGuidesManager))) AfxThrowInvalidArgException();
//     Destroy();
//     m_pCentralGroup = rtCentral->m_pfnCreateObject();     // called directly
//     for (i = sdLEFT; i <= sdBOTTOM; ++i) {
//         m_arMarkers[i] = rtMarker->m_pfnCreateObject();
//         m_arMarkers[i]->Create(i, pwndOwner);             // slot 5
//     }
//     m_pCentralGroup->Create(pwndOwner);                   // slot 5
//     for (i = sdCLEFT; i <= sdCMIDDLE; ++i)
//         m_arMarkers[i] = m_pCentralGroup->GetGuide(i);     // slot 9
//     m_pwndOwner = pwndOwner;
//     m_wndPlaceMarker.Create(pwndOwner);
//     m_bCreated = TRUE;
// Deviation: after Destroy(), if either chosen descriptor has a NULL
// m_pfnCreateObject, this returns without creating anything (m_bCreated stays
// FALSE).  Retail would call through that NULL pointer, but its own descriptors
// carry a factory (in mfc140.dll both m_pfnCreateObject fields point at the
// classes' exported CreateObject); OpenMFC's default descriptors for both guide classes
// (featurepack/docking/RuntimeClasses.cpp) currently have none, so without this
// guard every Create with a NULL prtMarker / prtCentralGroup would fault.
// A factory that returns NULL is not guarded, as in retail.
extern "C" void MS_ABI impl__Create_CSmartDockingManager__QEAAXPEAVCWnd__PEAUCRuntimeClass__1_Z(
    void* pThis, CWnd* pwndOwner, CRuntimeClass* prtMarker, CRuntimeClass* prtCentralGroup) {
    SdManagerLayout* m = Mgr(pThis);
    CRuntimeClass* const pGuideClass =
        impl__GetThisClass_CSmartDockingStandaloneGuide__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* const pGroupClass =
        impl__GetThisClass_CSmartDockingGroupGuidesManager__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* const rtMarker = prtMarker != nullptr ? prtMarker : pGuideClass;
    CRuntimeClass* const rtCentral = prtCentralGroup != nullptr ? prtCentralGroup : pGroupClass;
    if (!impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(rtMarker, pGuideClass)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    if (!impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(rtCentral, pGroupClass)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    impl__Destroy_CSmartDockingManager__QEAAXXZ(pThis);
    if (rtCentral->m_pfnCreateObject == nullptr || rtMarker->m_pfnCreateObject == nullptr) {
        return;   // Deviation (see above).
    }
    m->m_pCentralGroup = rtCentral->m_pfnCreateObject();
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        m->m_arMarkers[i] = rtMarker->m_pfnCreateObject();
        VGuideCreate(m->m_arMarkers[i], i, pwndOwner);
    }
    VGroupCreate(m->m_pCentralGroup, pwndOwner);
    for (int i = kSdCLeft; i <= kSdCMiddle; ++i) {
        m->m_arMarkers[i] = VGetGuide(m->m_pCentralGroup, i);
    }
    m->m_pwndOwner = pwndOwner;
    impl__Create_CSmartDockingHighlighterWnd__QEAAXPEAVCWnd___Z(PlaceMarker(pThis), pwndOwner);
    m->m_bCreated = TRUE;
}

// Symbol: ?Destroy@CSmartDockingManager@@QEAAXXZ
// Transcribed from retail RVA 0x131ad0 (mfc140u):
//     if (!m_bCreated) return;
//     Stop();
//     for (i = sdLEFT; i <= sdBOTTOM; ++i) {
//         if (m_arMarkers[i] != NULL) delete m_arMarkers[i];   // slot 1, flags 1
//         m_arMarkers[i] = NULL;
//     }
//     m_pCentralGroup->Destroy();                              // slot 6, no NULL test
//     if (m_pCentralGroup != NULL) delete m_pCentralGroup;     // slot 1, flags 1
//     m_pCentralGroup = NULL;
//     m_bCreated = FALSE;
// m_arMarkers[4..8] (owned by the central group) are left as they are, as in
// retail.
extern "C" void MS_ABI impl__Destroy_CSmartDockingManager__QEAAXXZ(void* pThis) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bCreated == 0) {
        return;
    }
    impl__Stop_CSmartDockingManager__QEAAXXZ(pThis);
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        if (m->m_arMarkers[i] != nullptr) {
            VDelete(m->m_arMarkers[i]);
        }
        m->m_arMarkers[i] = nullptr;
    }
    VDestroy(m->m_pCentralGroup);
    if (m->m_pCentralGroup != nullptr) {
        VDelete(m->m_pCentralGroup);
    }
    m->m_pCentralGroup = nullptr;
    m->m_bCreated = FALSE;
}

// Symbol: ?HidePlace@CSmartDockingManager@@QEAAXXZ
// Transcribed from retail RVA 0x1320c0 (mfc140u):
//     if (m_bStarted) m_wndPlaceMarker.Hide();
extern "C" void MS_ABI impl__HidePlace_CSmartDockingManager__QEAAXXZ(void* pThis) {
    if (Mgr(pThis)->m_bStarted != 0) {
        impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(PlaceMarker(pThis));
    }
}

// Symbol: ?MoveCentralGroup@CSmartDockingManager@@QEAAXVCRect@@HK@Z
// Transcribed from retail RVA 0x132190 (mfc140u):
//     if (!m_bStarted || m_pCentralGroup == NULL) return;
//     CRect rcCur(0, 0, 0, 0);
//     m_pCentralGroup->GetWindowRect(rcCur);
//     if (::EqualRect(&rcCur, &rect)) return;
//     m_pCentralGroup->ShowGuide(sdCLEFT,   (dwEnabledAlignment >> 12) & 1, TRUE);
//     m_pCentralGroup->ShowGuide(sdCTOP,    (dwEnabledAlignment >> 13) & 1, TRUE);
//     m_pCentralGroup->ShowGuide(sdCRIGHT,  (dwEnabledAlignment >> 14) & 1, TRUE);
//     m_pCentralGroup->ShowGuide(sdCBOTTOM, (dwEnabledAlignment >> 15) & 1, TRUE);
//     if (m_pCentralGroup->AdjustPos(rect, nMiddleIsOn))    // slot 8
//         m_nHiliteSideNo = sdNONE;
// dwEnabledAlignment is not stored in m_dwEnabledAlignment.  GetWindowRect and
// ShowGuide are direct (non-virtual) calls in retail.
extern "C" void MS_ABI impl__MoveCentralGroup_CSmartDockingManager__QEAAXVCRect__HK_Z(
    void* pThis, const RECT* pRect, int nMiddleIsOn, DWORD dwEnabledAlignment) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted == 0 || m->m_pCentralGroup == nullptr) {
        return;
    }
    RECT rcCur = {0, 0, 0, 0};
    impl__GetWindowRect_CSmartDockingGroupGuidesManager__QEAAXAEAVCRect___Z(m->m_pCentralGroup, &rcCur);
    if (::EqualRect(&rcCur, pRect)) {
        return;
    }
    impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
        m->m_pCentralGroup, kSdCLeft, (dwEnabledAlignment >> 12) & 1, TRUE);
    impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
        m->m_pCentralGroup, kSdCTop, (dwEnabledAlignment >> 13) & 1, TRUE);
    impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
        m->m_pCentralGroup, kSdCRight, (dwEnabledAlignment >> 14) & 1, TRUE);
    impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
        m->m_pCentralGroup, kSdCBottom, (dwEnabledAlignment >> 15) & 1, TRUE);
    if (VGroupAdjustPos(m->m_pCentralGroup, *pRect, nMiddleIsOn)) {
        m->m_nHiliteSideNo = kSdNone;
    }
}

// Symbol: ?OnMouseMove@CSmartDockingManager@@QEAAXVCPoint@@@Z
// Transcribed from retail RVA 0x131df0 (mfc140u).  CPoint arrives by value in
// RDX as an 8-byte aggregate and is passed on unchanged.
//     if (!m_bStarted) return;
//     m_nHiliteSideNo = sdNONE;
//     BOOL bFound = FALSE;
//     for (i = m_pCentralGroup->m_bMiddleIsOn ? sdCMIDDLE : sdCBOTTOM; i >= 0; --i) {
//         if (!bFound && m_arMarkers[i] != NULL && m_arMarkers[i]->IsPtIn(point)) {  // slot 10
//             bFound = TRUE;
//             m_arMarkers[i]->Highlight(TRUE);                                        // slot 9
//             m_nHiliteSideNo = i;
//         } else if (m_arMarkers[i] != NULL) {
//             m_arMarkers[i]->Highlight(FALSE);
//         }
//     }
// m_pCentralGroup is dereferenced without a NULL test (m_bMiddleIsOn at +0x1a0c),
// as in retail.
extern "C" void MS_ABI impl__OnMouseMove_CSmartDockingManager__QEAAXVCPoint___Z(void* pThis, long long point) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted == 0) {
        return;
    }
    m->m_nHiliteSideNo = kSdNone;
    BOOL bFound = FALSE;
    const BOOL bMiddleIsOn =
        *reinterpret_cast<const BOOL*>(static_cast<const unsigned char*>(m->m_pCentralGroup) + kGgmBMiddleIsOn);
    for (int i = bMiddleIsOn != 0 ? kSdCMiddle : kSdCBottom; i >= 0; --i) {
        if (!bFound && m->m_arMarkers[i] != nullptr && VIsPtIn(m->m_arMarkers[i], point)) {
            bFound = TRUE;
            VHighlight(m->m_arMarkers[i], TRUE);
            m->m_nHiliteSideNo = i;
        } else if (m->m_arMarkers[i] != nullptr) {
            VHighlight(m->m_arMarkers[i], FALSE);
        }
    }
}

// Symbol: ?OnPosChange@CSmartDockingManager@@QEAAXXZ
// Transcribed from retail RVA 0x131eb0 (mfc140u):
//     if (!m_bStarted) return;
//     CRect rcOwner;
//     ::GetClientRect(m_pwndOwner->m_hWnd, &rcOwner);        // the import, directly
//     m_pwndOwner->ClientToScreen(&rcOwner);
//     for (i = sdLEFT; i <= sdBOTTOM; ++i)
//         m_arMarkers[i]->AdjustPos(rcOwner);                // slot 8, no NULL test
//     m_pCentralGroup->AdjustPos(rcOwner, -1);               // slot 8, no NULL test
// m_rcOuter is neither read nor updated.
extern "C" void MS_ABI impl__OnPosChange_CSmartDockingManager__QEAAXXZ(void* pThis) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted == 0) {
        return;
    }
    RECT rcOwner;
    ::GetClientRect(m->m_pwndOwner->m_hWnd, &rcOwner);
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(m->m_pwndOwner, &rcOwner);
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        VGuideAdjustPos(m->m_arMarkers[i], rcOwner);
    }
    VGroupAdjustPos(m->m_pCentralGroup, rcOwner, -1);
}

// Symbol: ?SetOuterRect@CSmartDockingManager@@QEAAXVCRect@@@Z
// Transcribed from retail RVA 0x131fa0 (mfc140u):
//     m_rcOuter = rcOuter;
//     m_pwndOwner->ClientToScreen(&m_rcOuter);               // before the m_bStarted test
//     if (!m_bStarted) return;
//     for (i = sdLEFT; i <= sdBOTTOM; ++i) {
//         m_arMarkers[i]->AdjustPos(m_rcOuter);              // slot 8
//         m_arMarkers[i]->Show(TRUE);                        // slot 7, whatever the alignment
//     }
//     m_pCentralGroup->AdjustPos(m_rcOuter, -1);             // slot 8
// Retail does not NULL-check m_pwndOwner, the markers or m_pCentralGroup.
extern "C" void MS_ABI impl__SetOuterRect_CSmartDockingManager__QEAAXVCRect___Z(void* pThis, const RECT* pRcOuter) {
    SdManagerLayout* m = Mgr(pThis);
    m->m_rcOuter = *pRcOuter;
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(m->m_pwndOwner, &m->m_rcOuter);
    if (m->m_bStarted == 0) {
        return;
    }
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        VGuideAdjustPos(m->m_arMarkers[i], m->m_rcOuter);
        VShow(m->m_arMarkers[i], TRUE);
    }
    VGroupAdjustPos(m->m_pCentralGroup, m->m_rcOuter, -1);
}

// Symbol: ?Show@CSmartDockingManager@@QEAAXH@Z
// Transcribed from retail RVA 0x131d30 (mfc140u):
//     if (!m_bStarted || m_bShown == bShow) return;
//     m_bShown = bShow;
//     if (m_bCentralGroupShown) m_pCentralGroup->Show(bShow);   // slot 7
//     for (i = sdLEFT; i <= sdBOTTOM; ++i)
//         if (side i enabled in m_dwEnabledAlignment)            // see SideEnabled
//             m_arMarkers[i]->Show(bShow);                       // slot 7
//     if (!bShow && !m_wndPlaceMarker.m_bTabbed)                 // +0x118 of the highlighter
//         m_wndPlaceMarker.Hide();
extern "C" void MS_ABI impl__Show_CSmartDockingManager__QEAAXH_Z(void* pThis, int bShow) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted == 0 || m->m_bShown == bShow) {
        return;
    }
    m->m_bShown = bShow;
    if (m->m_bCentralGroupShown != 0) {
        VShow(m->m_pCentralGroup, bShow);
    }
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        if (SideEnabled(m->m_dwEnabledAlignment, i)) {
            VShow(m->m_arMarkers[i], bShow);
        }
    }
    if (bShow == 0 &&
        *reinterpret_cast<const BOOL*>(m->m_wndPlaceMarker + kHlBTabbed) == 0) {
        impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(PlaceMarker(pThis));
    }
}

// Symbol: ?ShowCentralGroup@CSmartDockingManager@@QEAAXHK@Z
// Transcribed from retail RVA 0x1322c0 (mfc140u):
//     if (m_bStarted && m_pCentralGroup != NULL && m_bShown && m_bCentralGroupShown != bShow) {
//         m_pCentralGroup->ShowGuide(sdCLEFT,   (dwEnabledAlignment >> 12) & 1, TRUE);
//         m_pCentralGroup->ShowGuide(sdCTOP,    (dwEnabledAlignment >> 13) & 1, TRUE);
//         m_pCentralGroup->ShowGuide(sdCRIGHT,  (dwEnabledAlignment >> 14) & 1, TRUE);
//         m_pCentralGroup->ShowGuide(sdCBOTTOM, (dwEnabledAlignment >> 15) & 1, TRUE);
//         m_pCentralGroup->Show(bShow);                          // slot 7
//     }
//     m_bCentralGroupShown = bShow;                              // on every path
extern "C" void MS_ABI impl__ShowCentralGroup_CSmartDockingManager__QEAAXHK_Z(
    void* pThis, int bShow, DWORD dwEnabledAlignment) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted != 0 && m->m_pCentralGroup != nullptr && m->m_bShown != 0 &&
        m->m_bCentralGroupShown != bShow) {
        impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
            m->m_pCentralGroup, kSdCLeft, (dwEnabledAlignment >> 12) & 1, TRUE);
        impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
            m->m_pCentralGroup, kSdCTop, (dwEnabledAlignment >> 13) & 1, TRUE);
        impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
            m->m_pCentralGroup, kSdCRight, (dwEnabledAlignment >> 14) & 1, TRUE);
        impl__ShowGuide_CSmartDockingGroupGuidesManager__QEAAXW4SDMarkerPlace_CSmartDockingStandaloneGuide__HH_Z(
            m->m_pCentralGroup, kSdCBottom, (dwEnabledAlignment >> 15) & 1, TRUE);
        VShow(m->m_pCentralGroup, bShow);
    }
    m->m_bCentralGroupShown = bShow;
}

// Symbol: ?ShowPlaceAt@CSmartDockingManager@@QEAAXVCRect@@@Z
// Transcribed from retail RVA 0x132080 (mfc140u):
//     if (m_bStarted && m_bShown && m_nHiliteSideNo != sdNONE)
//         m_wndPlaceMarker.ShowAt(rect);                         // by-value copy
extern "C" void MS_ABI impl__ShowPlaceAt_CSmartDockingManager__QEAAXVCRect___Z(void* pThis, const RECT* pRect) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted != 0 && m->m_bShown != 0 && m->m_nHiliteSideNo != kSdNone) {
        RECT copy = *pRect;
        impl__ShowAt_CSmartDockingHighlighterWnd__QEAAXVCRect___Z(PlaceMarker(pThis), &copy);
    }
}

// Symbol: ?ShowTabbedPlaceAt@CSmartDockingManager@@QEAAXVCRect@@HHH@Z
// Transcribed from retail RVA 0x1320e0 (mfc140u); nTabHeight is the stack
// argument.
//     if (!m_bStarted) return;
//     CRect rectTab(0, 0, 0, 0);
//     if (CTabbedPane::m_bTabsAlwaysTop)
//         ::SetRect(&rectTab, nTabXOffset, rect.top - nTabHeight,
//                   nTabXOffset + nTabWidth, rect.top);
//     else
//         ::SetRect(&rectTab, nTabXOffset, rect.bottom - rect.top,
//                   nTabXOffset + nTabWidth, rect.bottom - rect.top + nTabHeight);
//     m_wndPlaceMarker.ShowTabbedAt(rect, rectTab);              // two by-value copies
// (Import slot mfc140.dll 0x1802c5398 = SetRect.)
extern "C" void MS_ABI impl__ShowTabbedPlaceAt_CSmartDockingManager__QEAAXVCRect__HHH_Z(
    void* pThis, const RECT* pRect, int nTabXOffset, int nTabWidth, int nTabHeight) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted == 0) {
        return;
    }
    RECT rectTab = {0, 0, 0, 0};
    if (impl__m_bTabsAlwaysTop_CTabbedPane__2HA != 0) {
        ::SetRect(&rectTab, nTabXOffset, pRect->top - nTabHeight,
                  nTabXOffset + nTabWidth, pRect->top);
    } else {
        const int h = pRect->bottom - pRect->top;
        ::SetRect(&rectTab, nTabXOffset, h, nTabXOffset + nTabWidth, h + nTabHeight);
    }
    RECT rcCopy = *pRect;
    RECT rcTabCopy = rectTab;
    impl__ShowTabbedAt_CSmartDockingHighlighterWnd__QEAAXVCRect__0_Z(PlaceMarker(pThis), &rcCopy, &rcTabCopy);
}

// Symbol: ?Start@CSmartDockingManager@@QEAAXPEAVCWnd@@@Z
// Transcribed from retail RVA 0x131b70 (mfc140u):
//     if (!m_bCreated || m_bStarted) return;
//     m_pDockingWnd = pDockingWnd;
//     m_wndPlaceMarker.SetDockingWnd(pDockingWnd);          // inline: highlighter +0xf0
//     m_dwEnabledAlignment = CBRS_ALIGN_ANY;                // 0xf000
//     m_nHiliteSideNo = sdNONE;
//     if (pDockingWnd != NULL && pDockingWnd->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) {
//         CWnd* pBar = pDockingWnd->GetFirstVisiblePane();   // vftable +0x368 (slot 109)
//         if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(CDockablePane)))
//             m_dwEnabledAlignment = pBar->GetEnabledAlignment();   // vftable +0x340 (slot 104)
//     }
//     for (i = sdLEFT; i <= sdBOTTOM; ++i) {
//         m_arMarkers[i]->AdjustPos(m_rcOuter);             // slot 8
//         if (side i enabled in m_dwEnabledAlignment) m_arMarkers[i]->Show(TRUE);   // slot 7
//     }
//     m_bShown = TRUE; m_bCentralGroupShown = FALSE;        // one qword store of 1 at +0x10
//     m_bStarted = TRUE;
// The two runtime classes are the mfc140.dll descriptors named CPaneFrameWnd and
// CDockablePane.  Slot 109 of the CPaneFrameWnd vftable is
// ?GetFirstVisiblePane@CPaneFrameWnd@@UEBAPEAVCWnd@@XZ; slot 104 of the
// CDockablePane vftable is an unexported body that returns the dword at +0x100
// (mfc140.dll RVA 0x87d0), i.e. the inline CBasePane::GetEnabledAlignment.
// Deviation: both calls are made non-virtually -- GetFirstVisiblePane through
// the exported CPaneFrameWnd thunk (so CMultiPaneFrameWnd's override, slot 109
// of its mfc140.dll vftable, is not reached) and GetEnabledAlignment as a direct
// read of CBasePane::m_dwEnabledAlignment (so an override in a client-derived
// pane is not reached either) -- because the docking window may be an
// OpenMFC-built object whose vtable is not laid out in MSVC slot order.
extern "C" void MS_ABI impl__Start_CSmartDockingManager__QEAAXPEAVCWnd___Z(void* pThis, CWnd* pDockingWnd) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bCreated == 0 || m->m_bStarted != 0) {
        return;
    }
    m->m_pDockingWnd = pDockingWnd;
    *reinterpret_cast<CWnd**>(m->m_wndPlaceMarker + kHlPDockingWnd) = pDockingWnd;
    m->m_dwEnabledAlignment = kAlignAny;
    m->m_nHiliteSideNo = kSdNone;
    if (pDockingWnd != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pDockingWnd, impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
        CWnd* pBar = static_cast<CWnd*>(impl__GetFirstVisiblePane_CPaneFrameWnd__UEBAPEAVCWnd__XZ(pDockingWnd));
        if (pBar != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pBar, impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
            m->m_dwEnabledAlignment = reinterpret_cast<CBasePane*>(pBar)->m_dwEnabledAlignment;
        }
    }
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        VGuideAdjustPos(m->m_arMarkers[i], m->m_rcOuter);
        if (SideEnabled(m->m_dwEnabledAlignment, i)) {
            VShow(m->m_arMarkers[i], TRUE);
        }
    }
    m->m_bShown = TRUE;
    m->m_bCentralGroupShown = FALSE;
    m->m_bStarted = TRUE;
}

// Symbol: ?Stop@CSmartDockingManager@@QEAAXXZ
// Transcribed from retail RVA 0x131cc0 (mfc140u):
//     if (!m_bStarted) return;
//     m_nHiliteSideNo = sdNONE;
//     m_wndPlaceMarker.Hide();
//     for (i = sdLEFT; i <= sdBOTTOM; ++i) m_arMarkers[i]->Show(FALSE);   // slot 7
//     m_pCentralGroup->Show(FALSE);                                       // slot 7
//     m_bStarted = FALSE;
extern "C" void MS_ABI impl__Stop_CSmartDockingManager__QEAAXXZ(void* pThis) {
    SdManagerLayout* m = Mgr(pThis);
    if (m->m_bStarted == 0) {
        return;
    }
    m->m_nHiliteSideNo = kSdNone;
    impl__Hide_CSmartDockingHighlighterWnd__QEAAXXZ(PlaceMarker(pThis));
    for (int i = kSdLeft; i <= kSdBottom; ++i) {
        VShow(m->m_arMarkers[i], FALSE);
    }
    VShow(m->m_pCentralGroup, FALSE);
    m->m_bStarted = FALSE;
}
