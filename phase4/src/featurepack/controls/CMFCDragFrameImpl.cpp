// CMFCDragFrameImpl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// ---------------------------------------------------------------------------
// Retail decode notes.
//
// Every body below was transcribed from the retail disassembly
// (python3 <wf>/disas.py '<mangled>').  The tool reads mfc140.dll, the ANSI
// twin of mfc140u.dll; function bodies are byte-identical between the two, so
// offsets and control flow are correct for mfc140u, but the two images place
// every export at a different RVA.  Each citation below therefore names its
// image.  Where the mfc140u symbol map resolves the export, both RVAs are given.
// `call *0x1802c5bd0` in the retail listings is the CFG dispatcher
// (__guard_dispatch_icall_fptr), i.e. an ordinary indirect call -- not an
// import.  The USER32 imports used here were resolved with iat.py:
//   0x1802c5368 SetRectEmpty   0x1802c52c8 IsRectEmpty   0x1802c5398 SetRect
//   0x1802c5370 GetWindowRect  0x1802c4c00 GetDesktopWindow
// (mfc140.dll import slots; they are quoted only to identify the call.)
//
// CMFCDragFrameImpl is not a CObject.  OpenMFC models it as an opaque 128-byte
// blob (include/openmfc/afxmfc.h:318), so its fields are reached through the
// mirror below.  The field order is the one declared in the shipping SDK
// header atlmfc/include/afxdragframeimpl.h:30, and the offsets it produces
// are the ones the retail bodies use: the constructor (mfc140 0x55210)
// stores -1 to +0x38 (m_nInsertedTabID; ResetState resets the same field to
// -1), Init (mfc140 0x55350) stores its CWnd* argument to
// +0x58 (m_pDraggedWnd) and the GetDockingManager result to +0x60
// (m_pDockManager), and the destructor tears down the CWnd at +0x70
// (m_pWndDummy).  Retail sizeof is 0x80, matching the opaque blob.
//
// The retail object starts with a vfptr (virtual ~CMFCDragFrameImpl; the
// mfc140 vftable at 0x1802e5070 has a single slot, the non-exported scalar
// deleting destructor at mfc140 0x552c0).  OpenMFC has no MSVC-layout vftable
// for this class, so NOTHING here writes the vfptr slot; see the constructor.
//
// Smart docking.  DrawDragFrame, DrawFrameTab, EndDrawDragFrame and
// RemoveTabPreDocking probe
//     CSmartDockingManager* pSD = m_pDockManager->m_pSDManager;   // +0x308
//     bSDockingIsOn = pSD && pSD->m_bCreated(+0xc) && pSD->m_bStarted(+0x8);
// and, when it is on, drive the smart-docking highlighter
// (CSmartDockingHighlighterWnd::Hide, CSmartDockingManager::ShowPlaceAt /
// ShowTabbedPlaceAt) or skip LockUpdate / show the dragged window, instead of
// XOR-drawing on the desktop.  (The retail PlaceTabPreDocking(CBaseTabbedPane*)
// probes it too, to hide m_pDraggedWnd -- that overload is a stub here.
// PlaceTabPreDocking(CWnd*), Init, ResetState, the constructor and the
// destructor do not read +0x308 at all.)  OpenMFC's
// CDockingManager (include/openmfc/afxmfc.h:1381) declares only m_pParentWnd
// and a 128-byte padding block, with no smart-docking manager pointer at all
// -- reading +0x308 would run off the object -- so these bodies take bSDockingIsOn == FALSE unconditionally.  That
// is the state OpenMFC is actually in (no smart-docking manager is ever
// attached to a docking manager); the same deviation is taken in
// CDockablePane::GetDockingStatus (featurepack/docking/CDockablePane.cpp).
// ---------------------------------------------------------------------------

namespace {

struct DragFrameLayout {
    void*            vfptr;                // 0x00 (never written here, see above)
    POINT            m_ptHot;              // 0x08
    RECT             m_rectDrag;           // 0x10
    RECT             m_rectExpectedDocked; // 0x20
    int              m_bDockToTab;         // 0x30
    int              m_bDragStarted;       // 0x34
    int              m_nInsertedTabID;     // 0x38
    void*            m_pFinalTargetBar;    // 0x40  CDockablePane*
    void*            m_pOldTargetBar;      // 0x48  CDockablePane*
    int              m_nOldThickness;      // 0x50
    CWnd*            m_pDraggedWnd;        // 0x58
    CDockingManager* m_pDockManager;       // 0x60
    void*            m_pTargetBar;         // 0x68  CDockablePane*
    CWnd*            m_pWndDummy;          // 0x70  (retail: a CDummyDockablePane)
    int              m_bFrameTabDrawn;     // 0x78
};
static_assert(offsetof(DragFrameLayout, m_ptHot)              == 0x08, "m_ptHot");
static_assert(offsetof(DragFrameLayout, m_rectDrag)           == 0x10, "m_rectDrag");
static_assert(offsetof(DragFrameLayout, m_rectExpectedDocked) == 0x20, "m_rectExpectedDocked");
static_assert(offsetof(DragFrameLayout, m_bDockToTab)         == 0x30, "m_bDockToTab");
static_assert(offsetof(DragFrameLayout, m_bDragStarted)       == 0x34, "m_bDragStarted");
static_assert(offsetof(DragFrameLayout, m_nInsertedTabID)     == 0x38, "m_nInsertedTabID");
static_assert(offsetof(DragFrameLayout, m_pFinalTargetBar)    == 0x40, "m_pFinalTargetBar");
static_assert(offsetof(DragFrameLayout, m_pOldTargetBar)      == 0x48, "m_pOldTargetBar");
static_assert(offsetof(DragFrameLayout, m_nOldThickness)      == 0x50, "m_nOldThickness");
static_assert(offsetof(DragFrameLayout, m_pDraggedWnd)        == 0x58, "m_pDraggedWnd");
static_assert(offsetof(DragFrameLayout, m_pDockManager)       == 0x60, "m_pDockManager");
static_assert(offsetof(DragFrameLayout, m_pTargetBar)         == 0x68, "m_pTargetBar");
static_assert(offsetof(DragFrameLayout, m_pWndDummy)          == 0x70, "m_pWndDummy");
static_assert(offsetof(DragFrameLayout, m_bFrameTabDrawn)     == 0x78, "m_bFrameTabDrawn");
static_assert(sizeof(DragFrameLayout) == 0x80, "retail sizeof(CMFCDragFrameImpl)");
static_assert(sizeof(CMFCDragFrameImpl) == sizeof(DragFrameLayout),
              "the opaque blob in afxmfc.h must stay the retail size");

inline DragFrameLayout* DF(void* pThis) { return static_cast<DragFrameLayout*>(pThis); }

// AFX_GLOBAL_DATA offsets (afxGlobalData is mfc140 .data 0x1803ba380).  The
// retail bodies read them directly: the constructor loads 0x1803ba60c (+0x28c),
// DrawFrameTab loads 0x1803ba610 (+0x290) and 0x1803ba628 (+0x2a8), all mfc140
// addresses.  The shadow struct in core/runtime/AFX_GLOBAL_DATA.cpp declares
// m_nDragFrameThicknessFloat at 0x28c, m_nDragFrameThicknessDock at 0x290 and
// m_nTextHeightHorz at 0x2a8, matching the SDK afxglobals.h member order; of
// the three only m_nTextHeightHorz has its own static_assert there (the other
// two sit between the asserted m_nBitsPerPixel @0x288 and
// m_nShellAutohideBars @0x2a4).
constexpr std::size_t kGlobalDataInitialized        = 0x000;  // m_bInitialized
constexpr std::size_t kGlobalDataDragThicknessFloat = 0x28c;  // m_nDragFrameThicknessFloat
constexpr std::size_t kGlobalDataDragThicknessDock  = 0x290;  // m_nDragFrameThicknessDock
constexpr std::size_t kGlobalDataTextHeightHorz     = 0x2a8;  // m_nTextHeightHorz

// afxGlobalUtils (mfc140 .data 0x1803aacf8): vfptr at 0, BOOL m_bDialogApp at 8
// (Init reads it at 0x1803aad00).
constexpr std::size_t kGlobalUtilsDialogApp = 0x008;

} // namespace

// Sibling exports defined in other translation units (each located with grep).
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];      // featurepack/CMFC_misc_stubs.cpp
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[16];          // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);  // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" int impl__m_bTabsAlwaysTop_CTabbedPane__2HA;                        // featurepack/docking/CTabbedPane.cpp
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
    CGlobalUtils* pThis, CWnd* pWnd);                                          // core/runtime/CGlobalUtils.cpp
extern "C" void MS_ABI impl__LockUpdate_CDockingManager__QEAAXH_Z(
    CDockingManager* pThis, int bLock);                                        // featurepack/docking/CDockingManager.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();             // detail/MfcExceptionsSupport.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);          // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd); // core/window/CWnd.cpp
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);          // core/window/CWnd.cpp
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);                      // core/window/CWnd.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                        // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();   // docking/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ(); // docking/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ();           // docking/CPane.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ();   // docking/CDockablePane.cpp
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert);                                    // docking/CBasePane.cpp
extern "C" void* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(
    const CBasePane* pThis);                                                   // docking/CBasePane.cpp
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd); // core/gdi/CWindowDC.cpp
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);                              // core/gdi/CWindowDC.cpp
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size, const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast);                                       // core/gdi/CDC.cpp

// Siblings defined further down in THIS file.
extern "C" void MS_ABI impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
    void* pThis, const RECT* lpRectOld, const RECT* lpRectNew, int bFirstTime,
    int nNewThickness, int nOldThickness);
extern "C" void MS_ABI impl__DrawFrameTab_CMFCDragFrameImpl__IEAAXPEAVCDockablePane__H_Z(
    void* pThis, void* pTargetBar, int bErase);
extern "C" void MS_ABI impl__RemoveTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCDockablePane___Z(
    void* pThis, void* pOldTargetBar);
extern "C" void MS_ABI impl__PlaceTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCBaseTabbedPane__H_Z(
    void* pThis, void* pTabbedBar, int bFirstTime);

namespace {

// Inline AFX_GLOBAL_DATA* GetGlobalData() (afxglobals.h): every retail use is
// the same four instructions -- test m_bInitialized, call
// AFX_GLOBAL_DATA::Initialize (mfc140 0x6a5c0) and set m_bInitialized = TRUE.
unsigned char* GlobalData() {
    unsigned char* g = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    int bInit = 0;
    std::memcpy(&bInit, g + kGlobalDataInitialized, sizeof bInit);
    if (!bInit) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(g);
        const int one = 1;
        std::memcpy(g + kGlobalDataInitialized, &one, sizeof one);
    }
    return g;
}

int GlobalInt(std::size_t off) {
    int v = 0;
    std::memcpy(&v, GlobalData() + off, sizeof v);
    return v;
}

bool IsKindOfClass(const void* p, CRuntimeClass* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
               static_cast<const CObject*>(static_cast<const CWnd*>(p)), pClass) != FALSE;
}

} // namespace

// Symbol: ??0CMFCDragFrameImpl@@QEAA@XZ
// Transcribed from retail entry RVA 0x55210 (mfc140) / 0x553c0 (mfc140u):
//     vfptr = &CMFCDragFrameImpl::`vftable';                 // NOT reproduced
//     m_ptHot = CPoint(0,0);
//     m_rectDrag.SetRectEmpty(); m_rectExpectedDocked.SetRectEmpty();
//     m_nOldThickness = GetGlobalData()->m_nDragFrameThicknessFloat;   // +0x28c
//     m_pDraggedWnd = m_pDockManager = m_pTargetBar = NULL;
//     m_bDockToTab = m_bDragStarted = FALSE;                 // one qword store at +0x30
//     m_pFinalTargetBar = m_pOldTargetBar = m_pWndDummy = NULL;
//     m_bFrameTabDrawn = FALSE;
//     m_nInsertedTabID = -1;
// (The CRect constructors zero both rects before SetRectEmpty runs; the
// four-byte gaps at +0x3c, +0x54 and +0x7c are not written.)
// Deviation: the vfptr slot at +0 is left untouched -- OpenMFC has no
// MSVC-layout vftable for this class.  Note too that OpenMFC's
// AFX_GLOBAL_DATA constructor is empty, so m_nDragFrameThicknessFloat reads 0
// until something initialises it; this body reads the field exactly as
// retail does.
extern "C" void* MS_ABI impl___0CMFCDragFrameImpl__QEAA_XZ(void* pThis) {
    DragFrameLayout* p = DF(pThis);
    p->m_ptHot.x = 0;
    p->m_ptHot.y = 0;
    ::SetRectEmpty(&p->m_rectDrag);
    ::SetRectEmpty(&p->m_rectExpectedDocked);
    p->m_nOldThickness = GlobalInt(kGlobalDataDragThicknessFloat);
    p->m_pDraggedWnd = nullptr;
    p->m_pDockManager = nullptr;
    p->m_pTargetBar = nullptr;
    p->m_bDockToTab = FALSE;
    p->m_bDragStarted = FALSE;
    p->m_pFinalTargetBar = nullptr;
    p->m_bFrameTabDrawn = FALSE;
    p->m_pOldTargetBar = nullptr;
    p->m_pWndDummy = nullptr;
    p->m_nInsertedTabID = -1;
    return pThis;
}

// Symbol: ??1CMFCDragFrameImpl@@UEAA@XZ
// Transcribed from retail entry RVA 0x55300 (mfc140) / 0x554b0 (mfc140u):
//     vfptr = &CMFCDragFrameImpl::`vftable';
//     if (m_pWndDummy != NULL) {                    // +0x70
//         m_pWndDummy->DestroyWindow();             // vtable +0xd0 (slot 26)
//         delete m_pWndDummy;                       // vtable +0x08, EDX = 1
//     }
// m_pWndDummy is only ever created by this class (MoveDragFrame /
// PlaceTabPreDocking allocate a 0x4e0-byte CDummyDockablePane, whose mfc140
// vftable 0x1802e4918 has CWnd::DestroyWindow in slot 26), so the static
// CWnd::DestroyWindow thunk is exactly the function retail reaches.
// Deviations: the vfptr store is not reproduced (see the constructor), and
// the `delete` is NOT reproduced -- no OpenMFC code creates the dummy pane
// yet (both creators are stubs), so how it would have to be freed is not
// known; the wrapper is leaked rather than freed through a guessed path.
// The retail signature is `void (this)`; the generated placeholder took no
// arguments and returned void*, and is corrected here.
extern "C" void MS_ABI impl___1CMFCDragFrameImpl__UEAA_XZ(void* pThis) {
    DragFrameLayout* p = DF(pThis);
    if (p->m_pWndDummy != nullptr) {
        impl__DestroyWindow_CWnd__UEAAHXZ(p->m_pWndDummy);
        // TODO(clean-room): retail then deletes m_pWndDummy (vtable slot 1).
    }
}

// Symbol: ?DrawDragFrame@CMFCDragFrameImpl@@IEAAXPEBUtagRECT@@0HHH@Z
// Transcribed from retail entry RVA 0x55de0 (mfc140) / 0x55f90 (mfc140u):
//     CWindowDC dc(CWnd::FromHandle(::GetDesktopWindow()));
//     CSmartDockingManager* pSD;
//     if (m_pDockManager && (pSD = m_pDockManager->m_pSDManager) && pSD->IsStarted())
//         pSD->ShowPlaceAt(CRect(lpRectNew));               // mfc140 0x132d00
//     else
//         dc.DrawDragRect(lpRectNew, CSize(nNewThickness, nNewThickness),
//                         bFirstTime ? NULL : lpRectOld,
//                         CSize(nOldThickness, nOldThickness), NULL, NULL);
// The smart-docking arm is dropped (see the file header); everything else is
// retail's.  The CWindowDC is built on the stack through its exported thunks
// because the class exists in this DLL only as those thunks.
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
    void* pThis, const RECT* lpRectOld, const RECT* lpRectNew, int bFirstTime,
    int nNewThickness, int nOldThickness) {
    (void)pThis;  // only the dropped smart-docking arm reads `this`
    CWnd* pDesktop = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetDesktopWindow());

    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, pDesktop);

    SIZE sizeNew;
    sizeNew.cx = nNewThickness;
    sizeNew.cy = nNewThickness;
    SIZE sizeOld;
    sizeOld.cx = nOldThickness;
    sizeOld.cy = nOldThickness;
    impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
        pDC, lpRectNew, sizeNew, bFirstTime ? nullptr : lpRectOld, sizeOld, nullptr, nullptr);

    impl___1CWindowDC__UEAA_XZ(pDC);
}

// Symbol: ?DrawFrameTab@CMFCDragFrameImpl@@IEAAXPEAVCDockablePane@@H@Z
// Transcribed from retail entry RVA 0x559c0 (mfc140) / 0x55b70 (mfc140u):
//     CRect rectWnd(0,0,0,0);
//     ::GetWindowRect(pTargetBar->m_hWnd, &rectWnd);
//     BOOL bSD = <smart docking on>;                         // dropped: FALSE
//     int nThick = GetGlobalData()->m_nDragFrameThicknessDock;     // +0x290
//     CRect rectTab = rectWnd;
//     int h = GetGlobalData()->m_nTextHeightHorz;                  // +0x2a8
//     if (CTabbedPane::m_bTabsAlwaysTop) { rectWnd.top += h;    rectTab.bottom = rectTab.top + h; }
//     else                               { rectWnd.bottom -= h; rectTab.top = rectTab.bottom - h; }
//     rectTab.left += 10;
//     rectTab.right = rectTab.left + 40;
//     if (rectTab.right >= rectWnd.right) rectTab.right = rectWnd.right - nThick - 4;
//     CRect rectEmpty; rectEmpty.SetRectEmpty();
//     int y = CTabbedPane::m_bTabsAlwaysTop ? rectTab.bottom : rectTab.top;
//     CRect rectLine; rectLine.SetRect(rectTab.left + nThick, y - nThick,
//                                      rectTab.right - nThick, y + nThick);
//     if (bErase) {
//         if (bSD) { if (pSD->m_bStarted) pSD->m_wndDockHighlighter.Hide(); }
//         else { DrawDragFrame(rectEmpty, rectTab,  FALSE, nThick, nThick);
//                DrawDragFrame(rectEmpty, rectWnd,  FALSE, nThick, nThick);
//                DrawDragFrame(rectEmpty, rectLine, FALSE, nThick, nThick);
//                m_bFrameTabDrawn = FALSE; }
//     } else {
//         if (bSD) pSD->ShowTabbedPlaceAt(rectWnd, 10, rectTab.Width(), rectTab.Height());
//         else { the same three DrawDragFrame calls with bFirstTime = TRUE;
//                m_bFrameTabDrawn = TRUE; }
//     }
// (CTabbedPane::m_bTabsAlwaysTop is mfc140 .data 0x1803b7038; retail reads
// it twice, and this body does too.)  The smart-docking arms are dropped (see
// the file header).  Deviation: a NULL pTargetBar returns immediately; retail
// has no check and would fault reading m_hWnd.
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__DrawFrameTab_CMFCDragFrameImpl__IEAAXPEAVCDockablePane__H_Z(
    void* pThis, void* pTargetBar, int bErase) {
    if (pTargetBar == nullptr) return;
    DragFrameLayout* p = DF(pThis);

    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(static_cast<CWnd*>(pTargetBar)->m_hWnd, &rectWnd);

    const int nThick = GlobalInt(kGlobalDataDragThicknessDock);
    RECT rectTab = rectWnd;
    if (impl__m_bTabsAlwaysTop_CTabbedPane__2HA != 0) {
        rectWnd.top += GlobalInt(kGlobalDataTextHeightHorz);
        rectTab.bottom = rectTab.top + GlobalInt(kGlobalDataTextHeightHorz);
    } else {
        rectWnd.bottom -= GlobalInt(kGlobalDataTextHeightHorz);
        rectTab.top = rectTab.bottom - GlobalInt(kGlobalDataTextHeightHorz);
    }
    rectTab.left += 10;
    rectTab.right = rectTab.left + 40;
    if (rectTab.right >= rectWnd.right) {
        rectTab.right = rectWnd.right - nThick - 4;
    }

    RECT rectEmpty;
    ::SetRectEmpty(&rectEmpty);
    RECT rectLine = {0, 0, 0, 0};
    const int y = (impl__m_bTabsAlwaysTop_CTabbedPane__2HA != 0) ? rectTab.bottom : rectTab.top;
    ::SetRect(&rectLine, rectTab.left + nThick, y - nThick, rectTab.right - nThick, y + nThick);

    const int bFirstTime = bErase ? FALSE : TRUE;
    impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
        pThis, &rectEmpty, &rectTab, bFirstTime, nThick, nThick);
    impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
        pThis, &rectEmpty, &rectWnd, bFirstTime, nThick, nThick);
    impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
        pThis, &rectEmpty, &rectLine, bFirstTime, nThick, nThick);
    p->m_bFrameTabDrawn = bErase ? FALSE : TRUE;
}

// Symbol: ?EndDrawDragFrame@CMFCDragFrameImpl@@QEAAXH@Z
// Transcribed from retail entry RVA 0x55cb0 (mfc140) / 0x55e60 (mfc140u):
//     if (m_pDockManager == NULL) return;                    // +0x60
//     BOOL bSD = FALSE;
//     if (<smart docking on>) { bSD = TRUE; pSD->m_wndDockHighlighter(+0x40).Hide(); }
//     CRect rectEmpty; rectEmpty.SetRectEmpty();
//     CRect rectDocked = m_rectExpectedDocked.IsRectEmpty() ? m_rectDrag
//                                                          : m_rectExpectedDocked;
//     if (m_nInsertedTabID == -1) {
//         if (!bSD) DrawDragFrame(rectEmpty, rectDocked, FALSE, m_nOldThickness, 4);
//     } else {
//         m_bDockToTab = TRUE;
//     }
//     if (bClearInternalRects) {
//         RemoveTabPreDocking(NULL);
//         m_rectExpectedDocked.SetRectEmpty();
//         m_rectDrag.SetRectEmpty();
//         m_pFinalTargetBar = m_pTargetBar;
//         m_pTargetBar = NULL;
//     }
//     m_bDragStarted = FALSE;
//     ENSURE(m_pDockManager != NULL);                        // AfxThrowInvalidArgException
//     if (!bSD) m_pDockManager->LockUpdate(FALSE);           // mfc140 0x4d160
// The smart-docking probe is dropped (see the file header), so bSD is FALSE.
// Note the thickness arguments really are (m_nOldThickness, 4): the fifth
// argument is the NEW thickness.
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(
    void* pThis, int bClearInternalRects) {
    DragFrameLayout* p = DF(pThis);
    if (p->m_pDockManager == nullptr) return;

    RECT rectEmpty = {0, 0, 0, 0};
    ::SetRectEmpty(&rectEmpty);
    const RECT rectDocked = ::IsRectEmpty(&p->m_rectExpectedDocked) ? p->m_rectDrag
                                                                    : p->m_rectExpectedDocked;
    if (p->m_nInsertedTabID == -1) {
        impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
            pThis, &rectEmpty, &rectDocked, FALSE, p->m_nOldThickness, 4);
    } else {
        p->m_bDockToTab = TRUE;
    }

    if (bClearInternalRects) {
        impl__RemoveTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCDockablePane___Z(pThis, nullptr);
        ::SetRectEmpty(&p->m_rectExpectedDocked);
        ::SetRectEmpty(&p->m_rectDrag);
        p->m_pFinalTargetBar = p->m_pTargetBar;
        p->m_pTargetBar = nullptr;
    }

    p->m_bDragStarted = FALSE;
    if (p->m_pDockManager == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    impl__LockUpdate_CDockingManager__QEAAXH_Z(p->m_pDockManager, FALSE);
}

// Symbol: ?Init@CMFCDragFrameImpl@@QEAAXPEAVCWnd@@@Z
// Transcribed from retail entry RVA 0x55350 (mfc140) / 0x55500 (mfc140u):
//     m_pDraggedWnd = pDraggedWnd;                           // +0x58
//     CWnd* pDockSite = NULL;
//     if (m_pDraggedWnd->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) {
//         CPaneFrameWnd* pMini = DYNAMIC_DOWNCAST(CPaneFrameWnd, m_pDraggedWnd);
//         pDockSite = CWnd::FromHandlePermanent(pMini->m_hwndOwner);   // +0x130
//     } else if (m_pDraggedWnd->IsKindOf(RUNTIME_CLASS(CPane))) {
//         CPane* pBar = DYNAMIC_DOWNCAST(CPane, m_pDraggedWnd);
//         CPaneFrameWnd* pMini = pBar->GetParentMiniFrame(FALSE);       // vtable +0x460
//         pDockSite = pMini ? CWnd::FromHandlePermanent(pMini->m_hwndOwner)
//                           : pBar->GetDockSiteFrameWnd();              // vtable +0x348
//     }
//     m_pDockManager = afxGlobalUtils.GetDockingManager(pDockSite);    // +0x60
//     if (afxGlobalUtils.m_bDialogApp) return;
//     ENSURE(m_pDockManager != NULL);                        // AfxThrowInvalidArgException
// (Runtime-class descriptors resolved with rtcname.py: 0x1803aa418 is
// CPaneFrameWnd, 0x1802f3868 is CPane.  Callees: 0x2891d0 FromHandlePermanent,
// 0x6ccc0 CGlobalUtils::GetDockingManager, 0x225b80 AfxThrowInvalidArgException,
// all mfc140 RVAs; the descriptors are mfc140 VAs.)  The inline
// CPaneFrameWnd::GetParent (SDK: FromHandlePermanent(m_hParentWnd)) reads the owner HWND
// retail CPaneFrameWnd::CreateEx stores at +0x130 (pParentWnd->m_hWnd).
// Deviations: (1) OpenMFC does not model that member (CPaneFrameWnd is a thin
// class in afxmfc.h), so the HWND is read back from the window itself with
// ::GetParent, which returns the owner of a WS_POPUP window and the parent of
// a child window.  OpenMFC's mini-frame creation passes pParentWnd as the
// CreateWindow parent, so for the retail case (CreateEx forces WS_POPUP) and a
// top-level dock site this is the HWND retail recorded.  It differs when the
// dock site is itself a child window (Windows then makes the child's
// top-level ancestor the owner) or when the mini frame is a top-level window
// without WS_POPUP (::GetParent returns NULL).  (2) GetParentMiniFrame and GetDockSiteFrameWnd dispatch
// statically to CBasePane's thunks.  Those are what the retail CPane,
// CDockablePane, CBaseTabbedPane and CMFCToolBar vftables (mfc140 RVAs
// 0x2f3a18, 0x2e3068, 0x2db2a8, 0x2e7a68) hold in both slots; a user class
// overriding either virtual is not reached.  (3) OpenMFC's
// CGlobalUtils::GetDockingManager only recognises CFrameWndEx, so a pane
// docked in any other frame type gets NULL here and -- exactly as retail does
// for NULL -- reaches the ENSURE throw unless m_bDialogApp is set.
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__Init_CMFCDragFrameImpl__QEAAXPEAVCWnd___Z(void* pThis, CWnd* pDraggedWnd) {
    DragFrameLayout* p = DF(pThis);
    p->m_pDraggedWnd = pDraggedWnd;

    CWnd* pDockSite = nullptr;
    // Stand-in for pMini->m_hwndOwner, see deviation (1) above.
    auto ownerOfMiniFrame = [](CWnd* pMini) -> CWnd* {
        return impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetParent(pMini->m_hWnd));
    };

    if (IsKindOfClass(p->m_pDraggedWnd, impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
        pDockSite = ownerOfMiniFrame(p->m_pDraggedWnd);
    } else if (IsKindOfClass(p->m_pDraggedWnd, impl__GetThisClass_CPane__SAPEAUCRuntimeClass__XZ())) {
        const CBasePane* pBar = static_cast<const CBasePane*>(p->m_pDraggedWnd);
        CWnd* pMini = static_cast<CWnd*>(
            impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pBar, FALSE));
        if (pMini != nullptr) {
            pDockSite = ownerOfMiniFrame(pMini);
        } else {
            pDockSite = static_cast<CWnd*>(impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(pBar));
        }
    }

    p->m_pDockManager = impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
        reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A), pDockSite);

    int bDialogApp = 0;
    std::memcpy(&bDialogApp, impl__afxGlobalUtils__3VCGlobalUtils__A + kGlobalUtilsDialogApp,
                sizeof bDialogApp);
    if (bDialogApp) return;
    if (p->m_pDockManager == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
}

// Symbol: ?PlaceTabPreDocking@CMFCDragFrameImpl@@QEAAXPEAVCBaseTabbedPane@@H@Z
// STUB.  Retail entry RVA 0x55ed0 (mfc140; not in the mfc140u symbol map).
// Checked: after `if (m_nInsertedTabID != -1) return; if (!bFirstTime)
// EndDrawDragFrame(FALSE);` it reads the caption of m_pDraggedWnd (or, when
// that is a CMultiPaneFrameWnd, of its vtable +0x368 first visible pane) into
// a CString, lazily allocates m_pWndDummy as a 0x4e0-byte CDummyDockablePane
// (mfc140 CDockablePane ctor 0x3fcb0 plus the non-exported mfc140 vftable
// 0x1802e4918) and
// creates it through vtable +0x660, then adds it to the tabbed pane's tab
// control -- pTabbedBar->GetUnderlyingWindow() (vtable +0x750, the inline read
// of m_pTabWnd at CBaseTabbedPane+0x4e8) -> tab vtable +0x328
// (CMFCBaseTabCtrl::AddTab(pDummy, strCaption, (UINT)-1, TRUE)) -- hides
// m_pDraggedWnd when smart docking is on, then records
// GetTabFromHwnd(m_pWndDummy->m_hWnd) (tab vtable +0x448) in m_nInsertedTabID
// and sets m_pOldTargetBar = pTabbedBar.  (Tab-control slot names are from the
// CMFCBaseTabCtrl vftable at mfc140 RVA 0x2dbd88.)  Missing here: the
// CDummyDockablePane class and vftable, and any model of CBaseTabbedPane's
// tab window (OpenMFC's CBaseTabbedPane is a 96-byte padding block).
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__PlaceTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCBaseTabbedPane__H_Z(
    void* pThis, void* pTabbedBar, int bFirstTime) {
    (void)pThis;
    (void)pTabbedBar;
    (void)bFirstTime;
}

// Symbol: ?PlaceTabPreDocking@CMFCDragFrameImpl@@QEAAXPEAVCWnd@@@Z
// Transcribed from retail entry RVA 0x56130 (mfc140; not in the mfc140u
// symbol map):
//     if (pCBarToPlaceOn && pCBarToPlaceOn->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) {
//         CBaseTabbedPane* pTabbed = (CBaseTabbedPane*)pCBarToPlaceOn;
//         int n = pTabbed->vtable[+0x760]();   // m_pTabWnd ? m_pTabWnd->GetVisibleTabsNum() : 0
//         if ((n > 1 && pTabbed->vtable[+0x768]()) ||
//             (pTabbed->vtable[+0x760]() > 0 && !pTabbed->vtable[+0x768]())) {
//             m_pTargetBar = pTabbed;                        // +0x68
//             PlaceTabPreDocking(pTabbed, TRUE);
//             return;
//         }
//     }
//     if (m_nInsertedTabID == -1 && pCBarToPlaceOn &&
//         pCBarToPlaceOn->IsKindOf(RUNTIME_CLASS(CDockablePane))) {
//         DrawFrameTab((CDockablePane*)pCBarToPlaceOn, FALSE);
//         m_pTargetBar = m_pOldTargetBar = pCBarToPlaceOn;  // +0x68, +0x48
//         m_nInsertedTabID = 1;
//     }
// (Descriptors, mfc140 addresses: 0x1802db168 CBaseTabbedPane, 0x1803aa178
// CDockablePane.  The +0x760 / +0x768 bodies are the non-exported mfc140
// 0x12ed0 / 0x12ef0, which forward to m_pTabWnd (+0x4e8) vtable +0x358
// GetVisibleTabsNum and +0x550, returning 0 when m_pTabWnd is NULL.)
// Deviation: for a CBaseTabbedPane target the tab-count test cannot be
// evaluated -- OpenMFC does not model CBaseTabbedPane::m_pTabWnd -- and the
// overload it would call is itself a stub above, so a CBaseTabbedPane target
// returns with no effect.  That differs from retail in two ways: when the test
// passes retail also sets m_pTargetBar; when it fails retail falls through to
// the CDockablePane arm (a CBaseTabbedPane is a CDockablePane) and draws the
// frame tab.  Every non-tabbed target follows retail exactly.
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__PlaceTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pCBarToPlaceOn) {
    DragFrameLayout* p = DF(pThis);
    if (pCBarToPlaceOn != nullptr &&
        IsKindOfClass(pCBarToPlaceOn, impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ())) {
        // TODO(clean-room): tab-count test on m_pTabWnd not modelled; see above.
        return;
    }
    if (p->m_nInsertedTabID == -1 && pCBarToPlaceOn != nullptr &&
        IsKindOfClass(pCBarToPlaceOn, impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ())) {
        impl__DrawFrameTab_CMFCDragFrameImpl__IEAAXPEAVCDockablePane__H_Z(pThis, pCBarToPlaceOn, FALSE);
        p->m_pTargetBar = pCBarToPlaceOn;
        p->m_pOldTargetBar = pCBarToPlaceOn;
        p->m_nInsertedTabID = 1;
    }
}

// Symbol: ?RemoveTabPreDocking@CMFCDragFrameImpl@@QEAAXPEAVCDockablePane@@@Z
// Transcribed from retail entry RVA 0x56220 (mfc140; not in the mfc140u
// symbol map):
//     if (pOldTargetBar == NULL) pOldTargetBar = m_pOldTargetBar;       // +0x48
//     if (pOldTargetBar != NULL && m_nInsertedTabID != -1) {
//         if (pOldTargetBar->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)) &&
//             !m_bFrameTabDrawn && m_pWndDummy != NULL &&               // +0x78, +0x70
//             m_pWndDummy->m_hWnd != NULL) {
//             BOOL bSD = <smart docking on>;
//             m_pWndDummy->ShowWindow(SW_HIDE);
//             if (!bSD) m_pDockManager->LockUpdate(FALSE);
//             CMFCBaseTabCtrl* pTabs = pOldTargetBar->GetUnderlyingWindow(); // vtable +0x750
//             if (pTabs->GetTabWnd(m_nInsertedTabID) == m_pWndDummy)          // tab vtable +0x370
//                 pOldTargetBar->GetUnderlyingWindow()->RemoveTab(m_nInsertedTabID, TRUE); // +0x340
//             if (!bSD) m_pDockManager->LockUpdate(TRUE);
//         } else {
//             DrawFrameTab(pOldTargetBar, TRUE);
//         }
//         if (m_pDockManager->m_pSDManager && <started>)                // no NULL check
//             m_pDraggedWnd->ShowWindow(SW_SHOW);
//     }
//     m_nInsertedTabID = -1;
//     m_pOldTargetBar = NULL;
// Deviations: (1) the smart-docking probes are dropped (see the file header),
// including the trailing ShowWindow(SW_SHOW), which only runs when smart
// docking is on; (2) in the tabbed arm the tab lookup / RemoveTab step is not
// reproduced -- it goes through CBaseTabbedPane::m_pTabWnd (+0x4e8), which
// OpenMFC does not model.  That arm is only reachable with a live m_pWndDummy,
// and nothing in OpenMFC creates one yet (both creators are stubs).
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__RemoveTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCDockablePane___Z(
    void* pThis, void* pOldTargetBar) {
    DragFrameLayout* p = DF(pThis);
    if (pOldTargetBar == nullptr) pOldTargetBar = p->m_pOldTargetBar;
    if (pOldTargetBar != nullptr && p->m_nInsertedTabID != -1) {
        if (IsKindOfClass(pOldTargetBar, impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ()) &&
            p->m_bFrameTabDrawn == FALSE && p->m_pWndDummy != nullptr &&
            p->m_pWndDummy->m_hWnd != nullptr) {
            impl__ShowWindow_CWnd__QEAAHH_Z(p->m_pWndDummy, SW_HIDE);
            impl__LockUpdate_CDockingManager__QEAAXH_Z(p->m_pDockManager, FALSE);
            // TODO(clean-room): GetTabWnd / RemoveTab on m_pTabWnd not modelled.
            impl__LockUpdate_CDockingManager__QEAAXH_Z(p->m_pDockManager, TRUE);
        } else {
            impl__DrawFrameTab_CMFCDragFrameImpl__IEAAXPEAVCDockablePane__H_Z(pThis, pOldTargetBar, TRUE);
        }
    }
    p->m_nInsertedTabID = -1;
    p->m_pOldTargetBar = nullptr;
}

// Symbol: ?ResetState@CMFCDragFrameImpl@@QEAAXXZ
// Transcribed from retail entry RVA 0x56390 (mfc140; not in the mfc140u
// symbol map):
//     m_ptHot = CPoint(-1, -1);                   // one qword store of -1 at +0x08
//     m_rectDrag.SetRectEmpty();
//     m_rectExpectedDocked.SetRectEmpty();
//     m_nInsertedTabID = -1;
//     m_pFinalTargetBar = NULL;
//     m_pOldTargetBar = NULL;
//     m_bDockToTab = m_bDragStarted = FALSE;      // one qword store of 0 at +0x30
// Signature corrected: the generated placeholder had no `this`.
extern "C" void MS_ABI impl__ResetState_CMFCDragFrameImpl__QEAAXXZ(void* pThis) {
    DragFrameLayout* p = DF(pThis);
    p->m_ptHot.x = -1;
    p->m_ptHot.y = -1;
    ::SetRectEmpty(&p->m_rectDrag);
    ::SetRectEmpty(&p->m_rectExpectedDocked);
    p->m_nInsertedTabID = -1;
    p->m_pFinalTargetBar = nullptr;
    p->m_pOldTargetBar = nullptr;
    p->m_bDockToTab = FALSE;
    p->m_bDragStarted = FALSE;
}
